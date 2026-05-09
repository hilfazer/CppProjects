#include "Calculator.hpp"
#include "./ui_Calculator.h"

Calculator::Calculator(
	QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Calculator)
{
	ui->setupUi(this);
}

Calculator::~Calculator()
{
	delete ui;
}
