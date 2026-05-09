#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
	Q_OBJECT

public:
	Calculator(QWidget *parent = nullptr);
	~Calculator();

private slots:
	void numPressed();
	void mathButtonPressed();
	void equalButtonPressed();
	void changeNumberSign();
	void clearDisplay();

private:
	Ui::Calculator *ui;
	double calcVal = 0.0;
	const QChar NoTriggerChar = '\0';
	QChar currentTrigger = NoTriggerChar;
};
#endif // CALCULATOR_HPP
