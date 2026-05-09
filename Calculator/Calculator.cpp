#include "Calculator.hpp"
#include "./ui_Calculator.h"

#include <QRegularExpression>


namespace {

std::map<QChar, std::function<double(double, double)>> const BinaryOperations =
{
	{'+', [](auto a, auto b) -> auto { return a + b; }},
	{'-', [](auto a, auto b) -> auto { return a - b; }},
	{'*', [](auto a, auto b) -> auto { return a * b; }},
	{'/', [](auto a, auto b) -> auto { return a / b; }},
};

}

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
		connect( numButtons[i], &QPushButton::released,
				this, &Calculator::numPressed );
	}

	connect( ui->add, &QPushButton::released,
			this, &Calculator::mathButtonPressed );
	connect( ui->substract, &QPushButton::released,
			this, &Calculator::mathButtonPressed );
	connect( ui->multiply, &QPushButton::released,
			this, &Calculator::mathButtonPressed );
	connect( ui->divide, &QPushButton::released,
			this, &Calculator::mathButtonPressed );

	connect( ui->equals, &QPushButton::released,
			this, &Calculator::equalButtonPressed );

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
	currentTrigger = NoTriggerChar;
	QString displayVal = ui->display->text();
	calcVal = displayVal.toDouble();
	QPushButton* button = (QPushButton*)sender();
	QString butVal = button->text();

	assert(butVal.size() == 1);
	currentTrigger = butVal[0];
	ui->display->setText("");
}

void Calculator::equalButtonPressed()
{
	double solution = 0.0;
	QString displayVal = ui->display->text();
	double dblDisplayVal = displayVal.toDouble();

	if (BinaryOperations.contains(currentTrigger) ) {
		solution = BinaryOperations.at(currentTrigger)(calcVal, dblDisplayVal);
	}

	ui->display->setText(QString::number(solution));
}

void Calculator::changeNumberSign()
{
	QString const displayVal = ui->display->text();
	static const QRegularExpression reg("[-]?[0-9.]*");
	const QRegularExpressionMatch match = reg.match(displayVal);

	if(match.hasMatch()) {
		double dblDisplayVal = displayVal.toDouble();
		double dblDisplayValSign = -1 * dblDisplayVal;
		ui->display->setText(QString::number(dblDisplayValSign));
	}
}

void Calculator::clearDisplay()
{
	ui->display->setText("0");
}
