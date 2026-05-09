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
		connect( numButtons[i], SIGNAL(released()),
				this, SLOT(numPressed()) );
	}

	connect( ui->add, SIGNAL(released()),
			this, SLOT(mathButtonPressed()) );
	connect( ui->substract, SIGNAL(released()),
			this, SLOT(mathButtonPressed()) );
	connect( ui->multiply, SIGNAL(released()),
			this, SLOT(mathButtonPressed()) );
	connect( ui->divide, SIGNAL(released()),
			this, SLOT(mathButtonPressed()) );

	connect( ui->equals, SIGNAL(released()),
			this, SLOT(equalButton()) );

	connect( ui->changeSign, &QPushButton::released,
			this, &Calculator::changeNumberSign );

	connect( ui->clear, &QPushButton::released,
			this, &Calculator::clearDisplay );
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
	divTrigger = false;
	multTrigger = false;
	addTrigger = false;
	subTrigger = false;
	QString displayVal = ui->display->text();
	calcVal = displayVal.toDouble();
	QPushButton* button = (QPushButton*)sender();
	QString butVal = button->text();

	if(QString::compare(butVal, "/", Qt::CaseInsensitive) == 0) {
		divTrigger = true;
	} else if(QString::compare(butVal, "*", Qt::CaseInsensitive) == 0) {
		multTrigger = true;
	} else if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0) {
		addTrigger = true;
	} else if(QString::compare(butVal, "-", Qt::CaseInsensitive) == 0) {
		subTrigger = true;
	}
	ui->display->setText("");
}

void Calculator::equalButton()
{
	double solution = 0.0;
	QString displayVal = ui->display->text();
	double dblDisplayVal = displayVal.toDouble();

	if(addTrigger || subTrigger || multTrigger || divTrigger) {
		if(addTrigger){
			solution = calcVal + dblDisplayVal;
		} else if(subTrigger){
			solution = calcVal - dblDisplayVal;
		} else if(multTrigger){
			solution = calcVal * dblDisplayVal;
		} else if(divTrigger){
			solution = calcVal / dblDisplayVal;
		}
	}
	ui->display->setText(QString::number(solution));
}

void Calculator::changeNumberSign()
{
	QString displayVal = ui->display->text();
	QRegExp reg("[-]?[0-9.]*");

	if(reg.exactMatch(displayVal)) {
		double dblDisplayVal = displayVal.toDouble();
		double dblDisplayValSign = -1 * dblDisplayVal;
		ui->display->setText(QString::number(dblDisplayValSign));
	}
}

void Calculator::clearDisplay()
{
	ui->display->setText("0");
}
