#pragma once

#include "history/HistorySerialization.hpp"
#include "source/Message.hpp"
#include "source/Misc.hpp"
#include "stage/Stage.hpp"

#include <QObject>

#include <memory>

class StageModel;
class History;
class HistoryModel;
struct GameData;
struct StartingBuildingsData;

class Controller : public QObject
{
	Q_OBJECT

public:
	Controller();
	~Controller() override;

public:
	bool resetWithGameDataFile(QString const& filename);
	void initiatePlan();
	void savePlan(QString const& filename) const;
	void loadPlan(QString const& filename);

	void setPriceModifiers(PriceModsFlags modifiers);
	void startNewDay();

	void buildGoldMine();
	void upgradeGoldMineProduction(Stage::BuildingIndex idx);
	void upgradeGoldMineWorkers(Stage::BuildingIndex idx);
	void workMine(Stage::BuildingIndex idx);

	void buildShop();
	void upgradeShopSellingPrices();

	void scavengeRuins(unsigned size);

	void buildHouse();
	void upgradeHouse(Stage::BuildingIndex idx);

	GameData const* getGameData() const { return m_gameData.get(); }

signals:
	void message(Message msg) const;
	void stageModelChanged(QAbstractItemModel* model);
	void historyModelChanged(HistoryModel* model);
	void stageModified(Stage const* stage);
	void historyModified(History const* history);
	void gameDataReset(QString filename);
	void priceModifiersChanged(PriceModsFlags modifiers);

public slots:
	void resetHistoryToIndex(int rowIdx);

private:
	void resetWithGameData(GameData&& gameData, QString const& filename);
	int  checkIfDataExists() const;
	void executeSerializedActions(QVector<SerializedHistoryAction> const& serializedActions);

	bool startNewDayAndAddToHistory(PanicRank rank);
	bool buildGoldMineAndAddToHistory();
	bool upgradeMineAndAddToHistory(Stage::BuildingIndex idx, UpgradePath path);
	bool workMineAndAddToHistory(Stage::BuildingIndex idx);
	bool buildHouseAndAddToHistory();
	bool upgradeHouseAndAddToHistory(Stage::BuildingIndex idx);
	bool buildShopAndAddToHistory();
	bool upgradeShopPricesAndAddToHistory();
	bool scavengeRuinsAndAddToHistory(unsigned size);

	void emitMessage(QString text, Message::Severity severity = Message::Severity::WARNING) const;
	void updateStageAndHistory();

private:
	PriceModsFlags m_priceModifiers;
	QString        m_gameDataFilename;

	std::unique_ptr<Stage>        m_stage;
	std::unique_ptr<StageModel>   m_stageModel;
	std::unique_ptr<History>      m_history;
	std::unique_ptr<HistoryModel> m_historyModel;
	std::unique_ptr<GameData>     m_gameData;
};
