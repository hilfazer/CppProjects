#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

#include <memory>

namespace Ui {
class MainWindow;
}

class Octave;
class QLineEdit;
class QTableWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

signals:
	void octaveKeysParsed();
	void intervalKeysParsed();
	void octavesUpdated();

private slots:
	void updateOctaves();
	void updateDecodeButton();
	void updateEncodedTextWithOctaveNumbers();
	void onDecodeButtonPressed();
	void onOctaveAndIntervalKeysTextChanged();
	void onDecodedTextChanged();
	void onClearDecodedButtonClicked();

private:
	void setNumber( int number, QLineEdit* lineEdit);
	void setOctave(Octave const* octave, QTableWidget* octaveTable, const char symbols[]);
	void setDefaultOctave(QTableWidget* octaveTable, const char symbols[]);

private:
	Ui::MainWindow *ui;

	int m_transposition = -1;
	int m_firstModulation = -1;
	int m_secondModulation = -1;

	int m_firstOctaveNumber = -1;
	int m_secondOctaveNumber = -1;
	int m_thirdOctaveNumber = -1;

	std::unique_ptr<Octave> m_firstOctave;
	std::unique_ptr<Octave> m_secondOctave;
	std::unique_ptr<Octave> m_thirdOctave;

	QStringList m_textAsOctaveNumbers;
};

#endif // MAINWINDOW_H
