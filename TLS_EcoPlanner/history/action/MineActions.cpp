#include "history/action/MineActions.hpp"

namespace
{

QString const MineBuiltText        = "Build gold mine %1";
QString const MineProdUpgradedText = "Gold mine #%1 Production lvl %2";
QString const MineWorkedText       = "Work gold mine #%1";
QString const MineWorkUpgradedText = "Gold mine #%1 Workers lvl %2";

} // namespace

BuildGoldMineAction::BuildGoldMineAction(Stage& stage) : HistoryActionBase(stage)
{
}

StageError BuildGoldMineAction::doImpl()
{
	auto mineIdx = m_stage.buildMine();

	if (!mineIdx) {
		return StageError::BUILDING_LIMIT_REACHED;
	}

	m_idx = *mineIdx;
	return StageError::OK;
}

void BuildGoldMineAction::undo()
{
	m_stage.undoBuildMine(m_idx);
}

void BuildGoldMineAction::postDoInit()
{
	// empty
}

int BuildGoldMineAction::getCost(PriceData const& priceData, PriceModsFlags modifiers) const
{
	return applyModifiers(priceData.mine.build, priceData, modifiers);
}

QString BuildGoldMineAction::getDescription() const
{
	return MineBuiltText.arg(m_idx);
}

SerializedHistoryAction BuildGoldMineAction::serialize() const
{
	return { ActionElement::BUILD_MINE, {} };
}

Stage::BuildingIndex BuildGoldMineAction::getIndex() const
{
	assert(isExecuted());
	return m_idx;
}

//----------------------------------------------------------------------------

UpgradeGoldMineAction::UpgradeGoldMineAction(
        Stage& stage, Stage::BuildingIndex idx, UpgradePath path) :
        HistoryActionBase(stage), m_idx(idx), m_upgradePath(path)
{
	assert(stage.getGoldMines().size() > idx);
}

StageError UpgradeGoldMineAction::doImpl()
{
	if (m_idx >= m_stage.getGoldMines().size())
		return StageError::NO_BUILDING_WITH_IDX;

	return m_upgradePath == UpgradePath::WORKERS ? m_stage.upgradeGoldMineWorkers(m_idx)
	                                             : m_stage.upgradeGoldMineProduction(m_idx);
}

void UpgradeGoldMineAction::undo()
{
	m_upgradePath == UpgradePath::WORKERS ? m_stage.undoUpgradeGoldMineWorkers(m_idx)
	                                      : m_stage.undoUpgradeGoldMineProduction(m_idx);
}

void UpgradeGoldMineAction::postDoInit()
{
	m_upgradeLevel = m_upgradePath == UpgradePath::WORKERS
	                         ? m_stage.getGoldMines()[m_idx].getWorkLevel()
	                         : m_stage.getGoldMines()[m_idx].getProdLevel();
}

int UpgradeGoldMineAction::getCost(PriceData const& priceData, PriceModsFlags modifiers) const
{
	unsigned const cost = m_upgradePath == UpgradePath::PRODUCTION
	                              ? priceData.mine.productionUpgrades[m_upgradeLevel - 1]
	                              : priceData.mine.workerUpgrades[m_upgradeLevel - 1];

	return applyModifiers(cost, priceData, modifiers);
}

QString UpgradeGoldMineAction::getDescription() const
{
	static std::map<UpgradePath, QString> const Path2text{
		{UpgradePath::PRODUCTION, MineProdUpgradedText},
		{   UpgradePath::WORKERS, MineWorkUpgradedText},
	};

	return Path2text.at(m_upgradePath).arg(m_idx).arg(m_upgradeLevel);
}

SerializedHistoryAction UpgradeGoldMineAction::serialize() const
{
	return {
		ActionElement::UPGRADE_MINE,
		{{ UpgradePathAttribute, upgradePathToString(m_upgradePath) },
		                { MineIndexAttribute, m_idx }}
	};
}

//----------------------------------------------------------------------------

WorkGoldMineAction::WorkGoldMineAction(Stage& stage, Stage::BuildingIndex mineIdx) :
        HistoryActionBase(stage), m_idx(mineIdx)
{
}

StageError WorkGoldMineAction::doImpl()
{
	return m_stage.workMine(m_idx);
}

void WorkGoldMineAction::undo()
{
	m_stage.undoWorkMine(m_idx);
}

void WorkGoldMineAction::postDoInit()
{
	m_goldIncome = m_stage.getGoldMines()[m_idx].getDailyIncome(); // TODO: check if correct
}

int WorkGoldMineAction::getCost(PriceData const& priceData, PriceModsFlags modifiers) const
{
	return -applyModifiers(m_goldIncome, priceData, modifiers);
}

QString WorkGoldMineAction::getDescription() const
{
	return MineWorkedText.arg(m_idx);
}

SerializedHistoryAction WorkGoldMineAction::serialize() const
{
	return { ActionElement::WORK_MINE, { { MineIndexAttribute, m_idx } } };
}
