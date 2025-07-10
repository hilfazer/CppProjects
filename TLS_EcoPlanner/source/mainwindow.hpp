#pragma once

#include "source/Message.hpp"
#include "source/Misc.hpp"

#include <QtWidgets/QMainWindow>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

struct GameData;
class Controller;
class Stage;
class History;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void onGameDataReset(QString const& filename);
	void onStageModified(Stage const* stage);
	void onHistoryModified(History const* history);
	void onPriceModifiersChanged(PriceModsFlags modifiers);

	void setOmenOfDiscountPriceModifier(bool toggled);
	void setApoPriceModifier(bool toggled);
	// TODO apo price modifiers; there are 3 stages: 20, 40 and 60 %
	void startNewDay();
	void buildGoldMine();
	void upgradeGoldMineProduction();
	void upgradeGoldMineWorkers();
	void upgradeShop();
	void buildHouse();
	void upgradeHouse();
	void workGoldMine();
	void generateDefaultGameDataFile();

	void handleMessage(Message msg);

	void buildShop();

	void scavengeRuins();

private:
	void selectGameDataFile();
	void resetWithDataFile(QString const& dataFilename);
	void writeHistoryToFile();
	void readHistoryFromFile();
	void connectSignalsAndSlots();
	void updateGoldMineControls(std::size_t mineCount);
	void updateHouseControls(std::size_t houseCount);
	void updateWorkerCounts(unsigned remaining, unsigned max);

	void showMessageInStatusBar(QString msg);
	void addMessageToMessageLog(Message msg);

private:
	Ui::MainWindow*             ui;
	std::unique_ptr<Controller> m_controller;

	// QWidget interface
protected:
	void keyPressEvent(QKeyEvent* event) override;
};
