#include "mainwindow.hpp"
#include "ui_mainwindow.h"

#include "Octave.hpp"
#include "OctaveKeysParser.hpp"
#include "IntervalKeyParser.hpp"
#include "Decoder.hpp"

#include <vector>
#include <map>
#include <memory>


QString const OctaveAndIntervalPlaceholderText =
        "Mickey\n"
        "\n"
        "Interval key: foo foo, bar bar, baz baz.\n"
        "\n"
        "Octave keys:\n"
        "\n"
        "Key *: 000000000\n"
        "Key *: 0000000\n"
        "Key *: 00000000\n"
        "Key *: 00000000000\n";

QString const EncodedPlaceholderText =
        "Mickey,\n"
        "\n"
        "Here's your text:\n"
        "AA A\n"
        "B BB\n"
        "C C C\n"
        "DDD\n";

QString const decodedNumbersHint = "Decoded numbers can be digits or letters. "
        "In the latter case they can contain question marks and be separated with space.";

const unsigned OctaveNumbersRow = 1;
const unsigned SymbolsRow = 2;
const unsigned SemitonesRow = 0;

void clearOctave(QTableWidget* octaveTable, const char symbolArray[]);


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->octaveAndIntervalKeysText->setPlaceholderText(OctaveAndIntervalPlaceholderText);
	ui->encodedText->setPlaceholderText(EncodedPlaceholderText);

	setOctave( nullptr, ui->firstOctave,  SymbolArrays[0]);
	ui->firstOctave->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->firstOctave->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	setOctave( nullptr, ui->secondOctave, SymbolArrays[1]);
	ui->secondOctave->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->secondOctave->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	setOctave( nullptr, ui->thirdOctave,  SymbolArrays[2]);
	ui->thirdOctave->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->thirdOctave->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	ui->clearDecodedButton->setEnabled(ui->decodedText->toPlainText().size() > 0);

	connect( this, SIGNAL(intervalKeysParsed()), this, SLOT(updateOctaves()) );
	connect( this, SIGNAL(octaveKeysParsed()), this, SLOT(updateOctaves()) );
	connect( this, SIGNAL(octavesUpdated()), this, SLOT(updateDecodeButton()) );
	connect( ui->encodedText, SIGNAL(textChanged()), this, SLOT(updateDecodeButton()) );
	connect( ui->encodedText, SIGNAL(textChanged()), this, SLOT(updateEncodedTextWithOctaveNumbers()) );
	connect( this, SIGNAL(octaveKeysParsed()), this, SLOT(updateEncodedTextWithOctaveNumbers()) );
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::setNumber(int number, QLineEdit* lineEdit)
{
	if ( number >= 0 )
		lineEdit->setText(QString::number(number));
	else
		lineEdit->setText("");
}

void MainWindow::updateOctaves()
{
	QString transposedBy;
	m_firstOctave = m_firstOctaveNumber >= 0 && m_transposition >= 0
	        ? std::make_unique<Octave>(m_firstOctaveNumber, m_transposition, 0)
	        : nullptr;
	setOctave( m_firstOctave.get(), ui->firstOctave,  SymbolArrays[0]);
	transposedBy = m_firstOctave ? QString::number(m_transposition) : "";
	ui->transposedBy->setText(transposedBy);

	m_secondOctave = m_secondOctaveNumber >= 0 && m_transposition >= 0 && m_firstModulation >= 0
	        ? std::make_unique<Octave>(m_secondOctaveNumber, m_transposition + m_firstModulation, 1)
	        : nullptr;
	setOctave( m_secondOctave.get(), ui->secondOctave, SymbolArrays[1]);
	transposedBy = m_firstOctave ? QString::number(m_transposition) + " + " + QString::number(m_firstModulation) : "";
	ui->transposedBy_2->setText(transposedBy);

	m_thirdOctave = m_thirdOctaveNumber >= 0 && m_transposition >= 0 && m_secondModulation >= 0
	        ? std::make_unique<Octave>(m_thirdOctaveNumber, m_transposition + m_secondModulation, 2)
	        : nullptr;
	setOctave( m_thirdOctave.get(), ui->thirdOctave,  SymbolArrays[2]);
	transposedBy = m_firstOctave ? QString::number(m_transposition)+ " + " + QString::number(m_secondModulation) : "";
	ui->transposedBy_3->setText(transposedBy);

	emit octavesUpdated();
}

void MainWindow::setOctave(const Octave* octave, QTableWidget* octaveTable, const char symbols[])
{
	assert(octaveTable);
	octaveTable->clearContents();

	for (unsigned i = 0; i < 12; ++i)
	{
		QString symbol(symbols[i]);
		octaveTable->setCellWidget( SymbolsRow, int(i), new QLineEdit( symbol ) );

		if(octave)
		{
			octaveTable->setCellWidget(
				SemitonesRow, int(i), new QLineEdit( octave->getSemitone(i) ) );
			octaveTable->setCellWidget(
				OctaveNumbersRow, int(i), new QLineEdit(QString::number(octave->getOctaveNumber(i))) );
		}
		else {
			auto le = new QLineEdit(StandardOctave[int(i)]);
			QPalette palette;
			palette.setColor(QPalette::Text, Qt::gray);
			le->setPalette(palette);

			octaveTable->setCellWidget(
				SemitonesRow, int(i), le );
		}
	}
}

void MainWindow::setDefaultOctave(QTableWidget* octaveTable, const char symbols[])
{

	assert(octaveTable);
	octaveTable->clearContents();

	for (unsigned i = 0; i < 12; ++i)
	{
		QString symbol(symbols[i]);
		octaveTable->setCellWidget( SymbolsRow, int(i), new QLineEdit( symbol ) );

		octaveTable->setCellWidget(
			SemitonesRow, int(i), new QLineEdit( StandardOctave[int(i)] ) );
	}
}

void MainWindow::updateDecodeButton()
{
	ui->decodeButton->setEnabled(
		ui->encodedText->toPlainText().size() != 0
		&& (m_firstOctave || m_secondOctave || m_thirdOctave)
		&& m_textAsOctaveNumbers.size() != 0
		);
}

void MainWindow::updateEncodedTextWithOctaveNumbers()
{
	std::vector< std::map<int, QString> > const filteredLines =
	        filteredTextWithPositions(ui->encodedText->toPlainText());

	QString allLines;
	int octaveLineIdx = 0;
	for(std::map<int, QString> const& stringMap : filteredLines)
	{
		if(stringMap.size() == 0)
			continue;

		int lineLength = stringMap.rbegin()->first + stringMap.rbegin()->second.size();
		QString semitoneLine(lineLength, ' ');
		QString octaveNumberLine(lineLength, ' ');

		int octaveCharIdx = 0;
		for(auto const& pos2sem : stringMap)
		{
			if(!AllSemitones.contains(pos2sem.second))
				continue;

			semitoneLine.replace(pos2sem.first, pos2sem.second.size(), pos2sem.second);

			if(octaveLineIdx < m_textAsOctaveNumbers.size())
				if(octaveCharIdx < m_textAsOctaveNumbers[octaveLineIdx].length())
					octaveNumberLine[pos2sem.first] = m_textAsOctaveNumbers[octaveLineIdx][octaveCharIdx++];
		}
		++octaveLineIdx;

		if(allLines.size() > 0)
			allLines.append('\n');

		allLines.append(semitoneLine).append('\n').append(octaveNumberLine).append('\n');
	}

	ui->encodedTextOctaveKeys->setText(allLines);
	updateDecodeButton();

}

void MainWindow::on_decodeButton_pressed()
{
	ui->decodedText->clear();

	std::vector<Octave> validOctaves;
	if(m_firstOctave)
		validOctaves.push_back(*m_firstOctave.get());
	if(m_secondOctave)
		validOctaves.push_back(*m_secondOctave.get());
	if(m_thirdOctave)
		validOctaves.push_back(*m_thirdOctave.get());

	QString decoded = decodeText(
		ui->encodedText->toPlainText(), validOctaves, m_textAsOctaveNumbers);

	if(decoded.size() != 0)
	decoded.append( "\n\n\n\n\nHint:\n" + decodedNumbersHint );

	ui->decodedText->setText(decoded);
}

void MainWindow::on_octaveAndIntervalKeysText_textChanged()
{
	IntervalParsingResults intervalResults = parseIntervalKey( ui->octaveAndIntervalKeysText->toPlainText() );

	m_transposition = intervalResults.m_transposition;
	setNumber(m_transposition, ui->transpositionNumber);

	m_firstModulation = intervalResults.m_firstModulation;
	setNumber(m_firstModulation, ui->firstModulationNumber);

	m_secondModulation = intervalResults.m_secondModulation;
	setNumber(m_secondModulation, ui->secondModulationNumber);

	emit intervalKeysParsed();

	OctaveParsingResults octaveResults = parseOctaveKeys(ui->octaveAndIntervalKeysText->toPlainText());

	m_firstOctaveNumber = octaveResults.firstOctaveNumber;
	setNumber(m_firstOctaveNumber, ui->startingNumber);

	m_secondOctaveNumber = octaveResults.secondOctaveNumber;
	setNumber(m_secondOctaveNumber, ui->startingNumber_2);

	m_thirdOctaveNumber = octaveResults.thirdOctaveNumber;
	setNumber(m_thirdOctaveNumber, ui->startingNumber_3);

	m_textAsOctaveNumbers = octaveResults.textAsOctaveNumbers;

	emit octaveKeysParsed();
}

void MainWindow::on_decodedText_textChanged()
{
    ui->clearDecodedButton->setEnabled( ui->decodedText->toPlainText().size() > 0 );
	ui->decodedText->setEnabled(ui->decodedText->toPlainText().size() > 0);
}

void MainWindow::on_clearDecodedButton_clicked()
{
	ui->decodedText->clear();
}
