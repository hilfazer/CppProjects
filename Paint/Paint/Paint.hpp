#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Paint.h"


class ScribbleArea;

class Paint : public QMainWindow, private Ui::PaintClass
{
    Q_OBJECT

public:
    Paint(QWidget *parent = nullptr);
    ~Paint();

protected:
	void closeEvent(QCloseEvent* event) override;

private slots:
	void open();
	void save();
	void penColor();
	void penWidth();
	void about();

private:
	void createActions();
	void createMenus();
	bool maybeSave();
	bool saveFile(const QByteArray& fileFormat);
	ScribbleArea* scribbleArea;
	QMenu* saveAsMenu;
	QMenu* fileMenu;
	QMenu* optionMenu;
	QMenu* helpMenu;
	QAction* openAct;
	QList<QAction*> saveAsActs;
	QAction* exitAct;
	QAction* penColorAct;
	QAction* penWidthAct;
	QAction* aboutAct;
	QAction* aboutQtAct;
	QAction* printAct;
};

