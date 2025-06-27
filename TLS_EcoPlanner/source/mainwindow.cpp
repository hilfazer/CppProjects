#include "mainwindow.hpp"
#include "./ui_mainwindow.h"

#include "Controller.hpp"
#include "game_data/GameData.hpp"
#include "history/History.hpp"
#include "history/HistoryModel.hpp"
#include "history/HistoryResetColumnDelegate.hpp"
#include "stage/StageModel.hpp"

#include <QFileDialog>
#include <QKeyEvent>

extern GameData createDefaultGameData();

namespace GameDataXml
{
extern StringError writeGameDataToXml(GameData const& data, QString const& fileName);
}

namespace
{
char const*   DefaultDataFileName         = "default_game_data.xml";
char const*   DefaultSelectedDataFileName = "game_data.xml";
QString const DefaultGameDataFileWritten  = "default game data written to %1";

unsigned const HistoryDescriptionColumnWidth = 280;
unsigned const MessageTimeout                = 6500;

QString const OpenGameDataDialogTitle   = "Open Game Data File";
QString const OpenGameDataDialogFilters = "XML Files (*.xml )";
QString const OpenGameDataDialogPath    = ".";

QString const OpenHistoryDialogTitle = "Open Plan File";
QString const SaveHistoryDialogTitle = "Save Plan File";
QString const HistoryDialogFilters   = "XML Files (*.xml )";
QString const HistoryDialogPath      = ".";

QString const MsgGameDataFileNameEmpty = "Game data file name is empty";

QString const WorkerCountDisplayString = "%1 / %2";

} // namespace

MainWindow::MainWindow(QWidget* parent) :
        QMainWindow(parent), ui(new Ui::MainWindow), m_controller(new Controller)
{
	ui->setupUi(this);
	connectSignalsAndSlots();

	HistoryResetColumnDelegate* delegate = new HistoryResetColumnDelegate(ui->tableViewHistory);
	connect(delegate,
	        &HistoryResetColumnDelegate::resetHistoryToIndexRequested,
	        m_controller.get(),
	        &Controller::resetHistoryToIndex);
	ui->tableViewHistory->setItemDelegateForColumn(HistoryModel::ColumnIdx::Reset, delegate);

	m_controller->setPriceModifiers({ ui->actionToggleOmenOfDiscount->isChecked(),
									  ui->actionToggleApoPriceModifier->isChecked() });

	resetWithDataFile(DefaultSelectedDataFileName);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::connectSignalsAndSlots()
{
	assert(m_controller != nullptr);

	connect(m_controller.get(), &Controller::message, this, &MainWindow::handleMessage);

	connect(m_controller.get(),
	        &Controller::stageModelChanged,
	        ui->listViewStage,
	        &QListView::setModel);

	connect(m_controller.get(),
	        &Controller::historyModelChanged,
	        ui->tableViewHistory,
	        &QListView::setModel);

	connect(m_controller.get(), &Controller::stageModified, this, &MainWindow::onStageModified);

	connect(m_controller.get(), &Controller::historyModified, this, &MainWindow::onHistoryModified);

	connect(m_controller.get(), &Controller::gameDataReset, this, &MainWindow::onGameDataReset);

	connect(ui->actionLoadDataFile, &QAction::triggered, this, &MainWindow::selectGameDataFile);

	connect(ui->actionSavePlanToFile,
	        &QAction::triggered,
	        this,
	        &MainWindow::writeHistoryToFile);

	connect(ui->actionLoadPlanFromFile,
	        &QAction::triggered,
	        this,
	        &MainWindow::readHistoryFromFile);

	connect(m_controller.get(),
	        &Controller::priceModifiersChanged,
	        this,
	        &MainWindow::onPriceModifiersChanged);

	connect(ui->actionToggleOmenOfDiscount,
			&QAction::toggled,
			this,
			&MainWindow::setOmenOfDiscountPriceModifier);

	connect(ui->actionToggleApoPriceModifier,
			&QAction::toggled,
			this,
			&MainWindow::setApoPriceModifier);
}

void MainWindow::updateGoldMineControls(std::size_t mineCount)
{
	ui->spinBoxMineIndex->setMinimum(0);
	ui->spinBoxMineIndex->setMaximum(static_cast<int>(mineCount) - 1);
	ui->buttonMineProd->setDisabled(mineCount == 0);
	ui->buttonMineWork->setDisabled(mineCount == 0);
}

void MainWindow::updateHouseControls(std::size_t houseCount)
{
	ui->spinBoxHouseIdx->setMinimum(0);
	ui->spinBoxHouseIdx->setMaximum(static_cast<int>(houseCount) - 1);
	ui->pushButtonHouseUpgrade->setDisabled(houseCount == 0);
}

void MainWindow::updateWorkerCounts(unsigned remaining, unsigned max)
{
	ui->labelWorkerCounts->setText(WorkerCountDisplayString.arg(remaining).arg(max));
}

void MainWindow::onGameDataReset(QString const& filename)
{
	updateGoldMineControls(0u);
	updateHouseControls(0u);
	updateWorkerCounts(0, 0);
	ui->tableViewHistory->resizeColumnsToContents();
	ui->tableViewHistory->setColumnWidth(0, HistoryDescriptionColumnWidth);
	ui->lineEditLoadedDataFile->setText(filename);
}

void MainWindow::onStageModified(Stage const* stage)
{
	updateGoldMineControls(stage->getGoldMines().size());
	updateHouseControls(stage->getHouses().size());
	updateWorkerCounts(stage->getRemainingWorkers(), stage->getMaxWorkers());
}

void MainWindow::onHistoryModified(History const* history)
{
	Q_UNUSED(history) // implement when needed
}

void MainWindow::onPriceModifiersChanged(PriceModsFlags modifiers)
{
	ui->actionToggleOmenOfDiscount->setChecked(modifiers.easy);
	ui->actionToggleApoPriceModifier->setChecked(modifiers.apoc);
}

void MainWindow::setOmenOfDiscountPriceModifier(bool toggled)
{
	m_controller->setPriceModifiers({ toggled, ui->actionToggleApoPriceModifier->isChecked() });
}

void MainWindow::setApoPriceModifier(bool toggled)
{
	m_controller->setPriceModifiers({ ui->actionToggleOmenOfDiscount->isChecked(), toggled });
}

void MainWindow::selectGameDataFile()
{
	auto const dataFilename = QFileDialog::getOpenFileName(
	        this, OpenGameDataDialogTitle, OpenGameDataDialogPath, OpenGameDataDialogFilters);

	resetWithDataFile(dataFilename);
}

void MainWindow::resetWithDataFile(QString const& dataFilename)
{
	QFileInfo fileInfo(dataFilename);

	if (fileInfo.filePath().isEmpty()) {
		Message msg{ MsgGameDataFileNameEmpty, Message::Severity::ERROR };
		handleMessage(msg);
		return;
	}

	if (m_controller->resetWithGameDataFile(fileInfo.absoluteFilePath()))
		m_controller->initiatePlan();
}

void MainWindow::writeHistoryToFile()
{
	auto const filename = QFileDialog::getSaveFileName(
	        this, SaveHistoryDialogTitle, HistoryDialogPath, HistoryDialogFilters);

	m_controller->savePlan(filename);
}

void MainWindow::readHistoryFromFile()
{
	auto const filename = QFileDialog::getOpenFileName(
	        this, OpenHistoryDialogTitle, HistoryDialogPath, HistoryDialogFilters);

	m_controller->loadPlan(filename);
}

void MainWindow::startNewDay()
{
	m_controller->startNewDay();
}

void MainWindow::upgradeGoldMineProduction()
{
	int mineIdx = ui->spinBoxMineIndex->value();
	m_controller->upgradeGoldMineProduction(static_cast<Stage::BuildingIndex>(mineIdx));
}

void MainWindow::upgradeGoldMineWorkers()
{
	int mineIdx = ui->spinBoxMineIndex->value();
	m_controller->upgradeGoldMineWorkers(static_cast<Stage::BuildingIndex>(mineIdx));
}

void MainWindow::buildGoldMine()
{
	assert(m_controller != nullptr);
	m_controller->buildGoldMine();
}

void MainWindow::buildHouse()
{
	assert(m_controller != nullptr);
	m_controller->buildHouse();
}

void MainWindow::on_comboBoxWealthyHaven_currentIndexChanged(int index)
{
	m_controller->setWealthyHavenLevel(static_cast<unsigned>(index));
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key::Key_Escape) {
		qApp->quit();
	}
	else {
		QMainWindow::keyPressEvent(event);
	}
}

void MainWindow::upgradeShop()
{
	m_controller->upgradeShopSellingPrices();
}

void MainWindow::generateDefaultGameDataFile()
{
	QFileInfo fileInfo(DefaultDataFileName);
	assert(fileInfo.filePath().isEmpty() == false);
	auto error =
	        GameDataXml::writeGameDataToXml(createDefaultGameData(), fileInfo.absoluteFilePath());

	Message msg = error ? Message{ *error, Message::Severity::ERROR }
	                    : Message{ DefaultGameDataFileWritten.arg(fileInfo.absoluteFilePath()) };

	handleMessage(msg);
}

void MainWindow::handleMessage(Message msg)
{
	if (msg.severity != Message::Severity::INFO)
		showMessageInStatusBar(msg.text);

	addMessageToMessageLog(msg);
}

void MainWindow::showMessageInStatusBar(QString msg)
{
	statusBar()->showMessage(msg, MessageTimeout);
}

void MainWindow::addMessageToMessageLog(Message msg)
{
	QString text = msg.text;

	if (msg.severity == Message::Severity::ERROR)
		text = "<b>" + text + "</b>";

	ui->textEditMessageLog->append(text);
}

void MainWindow::upgradeHouse()
{
	m_controller->upgradeHouse(ui->spinBoxHouseIdx->text().toUInt());
}

void MainWindow::workMine()
{
	m_controller->workMine(ui->spinBoxMineIndex->text().toUInt());
}

void MainWindow::buildShop()
{
	m_controller->buildShop();
}

void MainWindow::scavengeRuins()
{
	m_controller->scavengeRuins(ui->spinBoxRuinsSize->text().toUInt());
}
