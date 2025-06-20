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

	void on_actionToggle_Easy_Price_Modifier_toggled(bool arg1);
	void on_actionToggle_Apo_Price_Modifier_toggled(bool arg1);
	void on_actionStartNewDay_triggered();
	void on_actionBuild_Gold_Mine_triggered();
	void on_buttonMineProd_pressed();
	void on_buttonMineWork_pressed();
	void on_comboBoxWealthyHaven_currentIndexChanged(int index);
	void on_buttonUpgradeShop_pressed();
	void on_actionGenerateDefaultGameDataFile_triggered();
	void on_actionBuildHouse_triggered();
	void on_pushButtonHouseUpgrade_pressed();
	void on_pushButtonWork_pressed();

	void handleMessage(Message msg);

	void on_buttonBuildShop_pressed();

	void on_buttonScavengeRuins_pressed();

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
