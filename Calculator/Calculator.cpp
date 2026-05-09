#include "Calculator.hpp"
#include "./ui_Calculator.h"


double calcVal = 0.0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;

Calculator::Calculator(
	QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Calculator)
{
	ui->setupUi(this);

	ui->display->setText(QString::number(calcVal));
	QPushButton* numButtons[10];

	for(std::size_t i = 0; i < 10; ++i) {
		QString butName = "button" + QString::number(i);
		numButtons[i] = Calculator::findChild<QPushButton*>(butName);
		connect( numButtons[i], SIGNAL(released()), this, SLOT(numPressed()) );
	}
}

Calculator::~Calculator()
{
	delete ui;
}

void Calculator::numPressed()
{
	QPushButton* button = (QPushButton*)sender();
	QString butVal = button->text();
	QString displayVal = ui->display->text();

	if ( (displayVal.toDouble() == 0) || (displayVal.toDouble() == 0.0) ) {
		ui->display->setText(butVal);
	} else {
		QString newVal = displayVal + butVal;
		double doubleNewVal = newVal.toDouble();
		ui->display->setText(QString::number(doubleNewVal, 'g', 16));
	}
}

void Calculator::mathButtonPressed()
{

}

void Calculator::equalButton()
{

}

void Calculator::changeNumberSign()
{

}
