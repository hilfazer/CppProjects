#include "Controller.hpp"
#include "history/History.hpp"
#include "history/HistoryModel.hpp"
#include "history/HistorySerialization.hpp"
#include "history/action/DayStartAction.hpp"
#include "history/action/HouseActions.hpp"
#include "history/action/MineActions.hpp"
#include "history/action/RuinsActions.hpp"
#include "history/action/ShopActions.hpp"
#include "stage/Stage.hpp"
#include "stage/StageModel.hpp"

namespace GameDataXml
{

extern GameData readGameDataFromXml(QString const& dataFile, QString& error);

}

namespace
{

QString const MsgUnknownFailure = "Operation failed";

QString const MsgNoDataFile        = "No data file has been loaded";
QString const MsgPlanWrittenToFile = "Plan written to file %1";
QString const MsgPlanReadFromFile  = "Plan read from file %1";

QString const MsgMineBuild             = "Mine #%1 built";
QString const MsgMineLimitReached      = "Mine limit reached";
QString const MsgMineCantUpgrade       = "Mine #%1 could not be upgraded";
QString const MsgNoMineWithIndex       = "Mine with index of #%1 doesn't exist";
QString const MsgNotEnoughWorkers      = "Not enough workers";
QString const MsgNotEnoughBuildingUses = "Not enough building uses";

QString const MsgNewDayStarted   = "New day started";
QString const MsgDayLimitReached = "No data for next day";

QString const MsgShopAlreadyBuilt      = "Shop already built";
QString const MsgShopCantUpgradePrices = "Couldn't upgrade shop's selling prices";

QString const MsgHouseBuild        = "House #%1 built";
QString const MsgHouseUpgraded     = "House #%1 upgraded";
QString const MsgHouseLimitReached = "House limit reached";
QString const MsgHouseCantUpgrade  = "House #%1 could not be upgraded";

QString const MsgRuinsDontExist = "Ruins of size %1 don't exist";

} // namespace

Controller::Controller() : m_history(new History)
{
	m_historyModel.reset(new HistoryModel(*m_history.get()));
}

Controller::~Controller()
{
}

void Controller::buildGoldMine()
{
	if (checkIfDataExists() != 0)
		return;

	if (!buildGoldMineAndAddToHistory())
		return;

	updateStageAndHistory();
}

void Controller::buildShop()
{
	if (checkIfDataExists() != 0)
		return;

	if (!buildShopAndAddToHistory())
		return;

	updateStageAndHistory();
}

void Controller::upgradeShopSellingPrices()
{
	if (checkIfDataExists() != 0)
		return;

	if (!upgradeShopPricesAndAddToHistory())
		return;

	updateStageAndHistory();
}

void Controller::scavengeRuins(unsigned size)
{
	if (checkIfDataExists() != 0)
		return;

	if (!scavengeRuinsAndAddToHistory(size))
		return;

	updateStageAndHistory();
}

void Controller::buildHouse()
{
	if (checkIfDataExists() != 0)
		return;

	if (!buildHouseAndAddToHistory())
		return;

	updateStageAndHistory();
}

void Controller::upgradeHouse(Stage::BuildingIndex idx)
{
	if (checkIfDataExists() != 0)
		return;

	if (!upgradeHouseAndAddToHistory(idx))
		return;

	updateStageAndHistory();
}

void Controller::upgradeGoldMineProduction(Stage::BuildingIndex idx)
{
	if (checkIfDataExists() != 0)
		return;

	if (!upgradeMineAndAddToHistory(idx, UpgradePath::PRODUCTION))
		return;

	updateStageAndHistory();
}

void Controller::upgradeGoldMineWorkers(Stage::BuildingIndex idx)
{
	if (checkIfDataExists() != 0)
		return;

	if (!upgradeMineAndAddToHistory(idx, UpgradePath::WORKERS))
		return;

	updateStageAndHistory();
}

void Controller::workMine(Stage::BuildingIndex idx)
{
	if (checkIfDataExists() != 0)
		return;

	if (!workMineAndAddToHistory(idx))
		return;

	updateStageAndHistory();
}

void Controller::startNewDay()
{
	if (checkIfDataExists() != 0)
		return;

	// TODO: variable panic rank
	if (!startNewDayAndAddToHistory(PanicRank::S))
		return;

	updateStageAndHistory();
	emitMessage(MsgNewDayStarted, Message::Severity::INFO);
}

bool Controller::resetWithGameDataFile(QString const& dataFilename)
{
	QString  error;
	GameData gameData = GameDataXml::readGameDataFromXml(dataFilename, error);
	if (!error.isEmpty()) {
		emitMessage(error, Message::Severity::ERROR);
		return false;
	}

	resetWithGameData(std::move(gameData), dataFilename);

	emit stageModelChanged(m_stageModel.get());
	emit historyModelChanged(m_historyModel.get());
	emit gameDataReset(dataFilename);
	return true;
}

void Controller::initiatePlan()
{
	assert(m_history->getNumberOfDays() == 0);
	assert(m_gameData != nullptr);

	m_stage->constructStartingBuildings(m_gameData->startingBuildings);
	startNewDayAndAddToHistory(PanicRank::S);

	updateStageAndHistory();
}

void Controller::savePlan(QString const& filename) const
{
	auto error = writePlanToFile(filename, *m_history, m_gameDataFilename, m_priceModifiers);

	if (error)
		emitMessage(*error, Message::Severity::ERROR);
	else
		emitMessage(MsgPlanWrittenToFile.arg(filename), Message::Severity::INFO);
}

void Controller::loadPlan(QString const& planFilename)
{
	SerializedHistory serializedHistory;
	auto              error = readPlanFromFile(planFilename, serializedHistory);
	if (error) {
		emitMessage(*error, Message::Severity::ERROR);
		return;
	}

	QString  gameDataReadError;
	GameData newGameData =
	        GameDataXml::readGameDataFromXml(serializedHistory.gameDataFile, gameDataReadError);

	if (!gameDataReadError.isEmpty()) {
		// reset with current game data
		emitMessage(gameDataReadError);

		GameData currentGameData = std::move(*m_gameData.get());
		resetWithGameData(std::move(currentGameData), m_gameDataFilename);
	}
	else {
		// reset with new game data
		resetWithGameData(std::move(newGameData), serializedHistory.gameDataFile);
		m_priceModifiers = serializedHistory.priceModifiers;
		emit priceModifiersChanged(m_priceModifiers);
	}

	emit stageModelChanged(m_stageModel.get());
	emit historyModelChanged(m_historyModel.get());
	emit gameDataReset(m_gameDataFilename);

	m_stage->constructStartingBuildings(m_gameData->startingBuildings);
	executeSerializedActions(serializedHistory.actions);

	updateStageAndHistory();
	emitMessage(MsgPlanReadFromFile.arg(planFilename), Message::Severity::INFO);
}

void Controller::setPriceModifiers(PriceModsFlags modifiers)
{
	m_priceModifiers = modifiers;

	m_history->setPriceModifiers(modifiers, m_gameData->prices);
	m_historyModel->update();
	emit historyModified(m_history.get());
}

void Controller::resetHistoryToIndex(int rowIdx)
{
	m_history->resetToIndex(rowIdx);

	m_stageModel->update();
	emit stageModified(m_stage.get());
	m_historyModel->update();
	emit historyModified(m_history.get());
}

void Controller::resetWithGameData(GameData&& gameData, QString const& filename)
{
	StageParams stageParams{ gameData.maxBuildings, gameData.startingBuildings,
		                     gameData.prices.mine,  gameData.goldMineUpgrades,
		                     gameData.shopUpgrades, gameData.ruinsValues };

	emit stageModelChanged(nullptr);
	emit historyModelChanged(nullptr);

	m_stage.reset(new Stage(stageParams));
	m_stageModel.reset(new StageModel(*m_stage.get()));
	m_history.reset(new History());
	m_historyModel.reset(new HistoryModel(*m_history.get()));
	m_gameData.reset(new GameData(gameData));
	m_gameDataFilename = filename;
}

int Controller::checkIfDataExists() const
{
	if (m_gameData == nullptr) {
		emitMessage(MsgNoDataFile);
		return -1;
	}
	else
		return 0;
}

bool Controller::startNewDayAndAddToHistory(PanicRank rank)
{
	assert(m_gameData);

	if (m_history->getNumberOfDays() >= m_gameData->night.panicRewards.size()) {
		emitMessage(MsgDayLimitReached);
		return false;
	}

	auto const dayNumber = static_cast<unsigned>( m_history->getNumberOfDays() + 1 );

	auto action = std::make_unique<DayStartAction>(*m_stage, dayNumber, rank, *m_gameData);

	switch (action->execute()) {
	case StageError::OK:
		m_history->addAction(std::move(action), m_gameData->prices);
		return true;
	default:
		emitMessage(MsgUnknownFailure, Message::Severity::CODE_ERROR);
		return false;
	}
}

bool Controller::buildGoldMineAndAddToHistory()
{
	auto action = std::make_unique<BuildGoldMineAction>(*m_stage);

	switch (action->execute()) {
	case StageError::OK:
		emitMessage(MsgMineBuild.arg(action->getIndex()), Message::Severity::INFO);
		m_history->addAction(std::move(action), m_gameData->prices);
		return true;
	case StageError::BUILDING_LIMIT_REACHED:
		emitMessage(MsgMineLimitReached);
		return false;
	default:
		emitMessage(MsgUnknownFailure, Message::Severity::CODE_ERROR);
		return false;
	}
}

bool Controller::upgradeMineAndAddToHistory(Stage::BuildingIndex idx, UpgradePath path)
{
	assert(m_stage);

	auto action = std::make_unique<UpgradeGoldMineAction>(*m_stage, idx, path);

	switch (action->execute()) {
	case StageError::OK:
		m_history->addAction(std::move(action), m_gameData->prices);
		return true;
	case StageError::UPGRADE_LIMIT_REACHED:
		emitMessage(MsgMineCantUpgrade.arg(idx));
		return false;
	case StageError::NO_BUILDING_WITH_IDX:
		emitMessage(MsgNoMineWithIndex.arg(idx));
		return false;
	default:
		emitMessage(MsgUnknownFailure, Message::Severity::CODE_ERROR);
		return false;
	}
}

bool Controller::workMineAndAddToHistory(Stage::BuildingIndex idx)
{
	auto action = std::make_unique<WorkGoldMineAction>(*m_stage, idx);

	switch (action->execute()) {
	case StageError::OK:
		m_history->addAction(std::move(action), m_gameData->prices);
		return true;
	case StageError::UPGRADE_LIMIT_REACHED:
		emitMessage(MsgMineCantUpgrade.arg(idx));
		return false;
	case StageError::NOT_ENOUGH_WORKERS:
		emitMessage(MsgNotEnoughWorkers);
		return false;
	case StageError::NOT_ENOUGH_BUILDING_USES:
		emitMessage(MsgNotEnoughBuildingUses);
		return false;
	default:
		emitMessage(MsgUnknownFailure, Message::Severity::CODE_ERROR);
		return false;
	}
}

bool Controller::upgradeShopPricesAndAddToHistory()
{
	auto action = std::make_unique<UpgradeShopSellingPricesAction>(*m_stage);

	switch (action->execute()) {
	case StageError::OK:
		m_history->addAction(std::move(action), m_gameData->prices);
		return true;
	case StageError::UPGRADE_LIMIT_REACHED:
		emitMessage(MsgShopCantUpgradePrices);
		return false;
	default:
		emitMessage(MsgUnknownFailure, Message::Severity::CODE_ERROR);
		return false;
	}
}

bool Controller::scavengeRuinsAndAddToHistory(unsigned size)
{
	auto action = std::make_unique<ScavengeRuinsAction>(*m_stage, size);

	switch (action->execute()) {
	case StageError::OK:
		m_history->addAction(std::move(action), m_gameData->prices);
		return true;
	case StageError::BUILDING_DOESNT_EXIST:
		emitMessage(MsgRuinsDontExist.arg(size));
		return false;
	case StageError::NOT_ENOUGH_WORKERS:
		emitMessage(MsgNotEnoughWorkers.arg(size));
		return false;
	default:
		emitMessage(MsgUnknownFailure, Message::Severity::CODE_ERROR);
		return false;
	}
}

bool Controller::buildHouseAndAddToHistory()
{
	auto action = std::make_unique<BuildHouseAction>(*m_stage);

	switch (action->execute()) {
	case StageError::OK:
		m_history->addAction(std::move(action), m_gameData->prices);
		return true;
	case StageError::BUILDING_LIMIT_REACHED:
		emitMessage(MsgHouseLimitReached);
		return false;
	default:
		emitMessage(MsgUnknownFailure, Message::Severity::CODE_ERROR);
		return false;
	}
}

bool Controller::upgradeHouseAndAddToHistory(Stage::BuildingIndex idx)
{
	auto action = std::make_unique<UpgradeHouseAction>(*m_stage, idx);

	switch (action->execute()) {
	case StageError::OK:
		m_history->addAction(std::move(action), m_gameData->prices);
		return true;
	case StageError::UPGRADE_LIMIT_REACHED:
		emitMessage(MsgHouseCantUpgrade.arg(idx));
		return false;
	default:
		emitMessage(MsgUnknownFailure, Message::Severity::CODE_ERROR);
		return false;
	}
}

bool Controller::buildShopAndAddToHistory()
{
	auto action = std::make_unique<BuildShopAction>(*m_stage);

	switch (action->execute()) {
	case StageError::OK:
		m_history->addAction(std::move(action), m_gameData->prices);
		return true;
	case StageError::BUILDING_LIMIT_REACHED:
		emitMessage(MsgShopAlreadyBuilt);
		return false;
	default:
		emitMessage(MsgUnknownFailure, Message::Severity::CODE_ERROR);
		return false;
	}
}

void Controller::emitMessage(QString text, Message::Severity severity) const
{
	emit message(Message{ text, severity });
}

void Controller::updateStageAndHistory()
{
	assert(m_stageModel != nullptr && m_historyModel != nullptr);

	m_stageModel->update();
	emit stageModified(m_stage.get());
	m_historyModel->update();
	emit historyModified(m_history.get());
}

void Controller::executeSerializedActions(QVector<SerializedHistoryAction> const& serializedActions)
{
	using AE = ActionElement;

	for (auto const& state : serializedActions) {
		switch (state.actionType) {
		case AE::DAY_START: {
			PanicRank const rank =
			        stringToRank(state.arguments[PanicRankAttribute].toString().toStdString());
			startNewDayAndAddToHistory(rank);
			break;
		}
		case AE::BUILD_MINE: {
			buildGoldMineAndAddToHistory();
			break;
		}
		case AE::UPGRADE_MINE: {
			Stage::BuildingIndex const idx = state.arguments[MineIndexAttribute].toUInt();
			UpgradePath const          path =
			        stringToUpgradePath(state.arguments[UpgradePathAttribute].toString());
			upgradeMineAndAddToHistory(idx, path);
			break;
		}
		case AE::WORK_MINE: {
			Stage::BuildingIndex const idx = state.arguments[MineIndexAttribute].toUInt();
			workMineAndAddToHistory(idx);
			break;
		}
		case AE::BUILD_HOUSE: {
			buildHouseAndAddToHistory();
			break;
		}
		case AE::UPGRADE_HOUSE: {
			Stage::BuildingIndex idx = state.arguments[HouseIndexAttribute].toUInt();
			upgradeHouseAndAddToHistory(idx);
			break;
		}
		case AE::BUILD_SHOP: {
			buildShopAndAddToHistory();
			break;
		}
		case AE::UPGRADE_SHOP: {
			upgradeShopPricesAndAddToHistory();
			break;
		}
		case AE::SCAVENGE_RUINS: {
			unsigned size = state.arguments[RuinsSizeAttribute].toUInt();
			scavengeRuinsAndAddToHistory(size);
			break;
		}
		default:
			assert(state.actionType != AE::INVALID);
			assert(!bool("Serialized action not handled"));
		}
	}
}
