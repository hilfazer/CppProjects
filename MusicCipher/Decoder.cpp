#include "Decoder.hpp"
#include "Octave.hpp"

#include <QString>
#include <QRegularExpression>

#include  <stdexcept>

std::map<int, QString> extractStringsWithPositions(const QString& text, QStringList const& toExtract)
{
	std::map<int, QString> extracted;

	for(auto const& str : toExtract)
		for (int i = 0, lastIdx = 0; i < text.count(str); ++i)
		{
			lastIdx = text.indexOf(str, lastIdx);
			extracted[lastIdx++] = str;
		}

	return extracted;
}

std::vector< std::map<int, QString> > filteredTextWithPositions(const QString& encodedText)
{
	QChar const space(' ');
	QString text = encodedText;
	text.replace(QRegularExpression("\\h+"), space);

	std::map<int, QString> const textWithPositions = extractStringsWithPositions(
	            text, AllSemitones + QStringList{space, "\n"});

	QString filteredText;
	QString filteredLine;
	bool lineHasSemitone = false;

	for(auto const& pos2str : textWithPositions)
	{
		if(AllSemitones.contains(pos2str.second))
		{
			filteredLine.append(pos2str.second);
			lineHasSemitone = true;
		}
		else if(pos2str.second == '\n')
		{
			if(lineHasSemitone)
			{
				filteredLine = filteredLine.trimmed();
				filteredLine.append('\n');
				filteredText.append(filteredLine);
				filteredLine.clear();
				lineHasSemitone = false;
			}
		}
		else {
			assert(pos2str.second == space);
			filteredLine = filteredLine.trimmed();
			filteredLine.append(space);
		}
	}

	if(lineHasSemitone)
		filteredText.append(filteredLine);

	std::vector< std::map<int, QString> > outputLines;

	QStringList filteredLines = filteredText.trimmed().split('\n');
	for(QString const& line : filteredLines)
		outputLines.push_back( extractStringsWithPositions(line, AllSemitones + QStringList{space}) );

	return outputLines;
}


std::map<QChar, std::map<QString, QChar>> mergeOctaves(std::vector<Octave> const& octaves)
{
	std::map<QChar, std::map<QString, QChar>> mergedOctaves;

	for(Octave const& octave : octaves)
	{
		for(unsigned i = 0; i < octave.length; ++i)
		{
			QChar number = QString::number(octave.getOctaveNumber(i)).at(0);
			mergedOctaves[ number ][octave.getSemitone(i)] = octave.getSymbol(i);
		}
	}

	return mergedOctaves;
}


QString decodeLineWithNumberOctave(Octave const& numberOctave, std::map<int, QString> encodedLine)
{
	QString line;
	for ( auto pos2str : encodedLine )
	{
		if(AllSemitones.contains(pos2str.second))
		{
			QString semitone = Octave::normalize(pos2str.second);
			char symbol = numberOctave.getSymbol( numberOctave.findSemitone(semitone) );
			line.append( symbol );
		}
	}

	return line.size() == 4 ? line : QString();
}

QString decodeText(
        QString const& encodedText
        , std::vector<Octave> const& octaves
        , const QStringList& textAsOctaveNumbers
        )
{
	auto const mergedOctaves = mergeOctaves(octaves);
	Octave const* numberOctave = (octaves.size() > 2) ? &octaves[2] : nullptr;
	std::vector< std::map<int, QString> > filteredLines = filteredTextWithPositions(encodedText);

	QStringList decodedLines;

	unsigned const linesNumber = filteredLines.size();
	for(std::size_t lineIdx = 0; lineIdx < linesNumber; ++lineIdx)
	{
		if( int(lineIdx) >= textAsOctaveNumbers.size() )
		{
			if(!numberOctave)
				continue;

			QString line = decodeLineWithNumberOctave(*numberOctave, filteredLines[lineIdx]);
			if(line.size() != 0)
				decodedLines.append(line);
			continue;
		}

		auto octaveNumberIt = textAsOctaveNumbers[int(lineIdx)].begin();
		QString line;
		for(auto const& pos2str : filteredLines[lineIdx])
		{
			QChar output(' ');
			QString const str = Octave::normalize(pos2str.second);
			if(AllSemitones.contains(str) && octaveNumberIt != textAsOctaveNumbers[int(lineIdx)].end())
			{
				try {
					output = mergedOctaves.at(*octaveNumberIt++).at(str);
				}
				catch (std::out_of_range const&) {
					output = '?';
				}
			}
			else {
				output = '_';
			}
			line.append(output);
		}

		decodedLines.append(line);
	}

	QString decodedText;
	for( QString const& st : decodedLines)
		decodedText.append(st + "\n");

	return decodedText;
}

