#include "OctaveKeysParser.hpp"

#include <QStringList>

#include <vector>


namespace
{

QString const KeyStart = "key*:";
unsigned const KeyMinMatched = 3;
int const PositionForKeyPrefix = KeyStart.size();
static unsigned const TranspositionDigitsNumber = 3;


bool isNumbersLine(QString const& lineNoWs);
int findOctaveNumbersPositionKeyPrefix(QString const& lineNoWs);
OctaveParsingResults getOctaveParsingResults(QStringList const& lines);

}


OctaveParsingResults parseOctaveKeys(QString const& keyText)
{
	QStringList keyLines;
	QStringList lines = keyText.split("\n");

	while (!lines.empty())
	{
		auto lineNoWs = lines.first().remove(QRegExp("\\s+"));
		lines.removeFirst();

		if(lineNoWs.size() <= KeyStart.size() || !isNumbersLine(lineNoWs))
			continue;

		int firstNumberPosition = findOctaveNumbersPositionKeyPrefix(lineNoWs);

		firstNumberPosition = std::max(firstNumberPosition, 0);

		if (lineNoWs.length() <= TranspositionDigitsNumber)
			continue;

		lineNoWs = lineNoWs.mid(firstNumberPosition);
		keyLines.append(lineNoWs);
	}

	return getOctaveParsingResults(keyLines);
}


namespace
{

bool isNumbersLine(QString const& lineNoWs)
{
	for(auto it = lineNoWs.begin(); it != lineNoWs.end(); ++it)
	{
		if( it->isDigit() )
			return true;
	}

	return false;
}

int findOctaveNumbersPositionKeyPrefix(QString const& lineNoWs)
{
	unsigned matchedChars = 0;
	for(int i = 0; i < KeyStart.size(); ++i)
		if(KeyStart[i] == lineNoWs[i].toLower())
			++matchedChars;

	if(matchedChars < KeyMinMatched)
		return -1;
	else
		return PositionForKeyPrefix;
}

OctaveParsingResults getOctaveParsingResults(QStringList const& lines)
{
	OctaveParsingResults results;

	for(QString const& line : lines)
	{
		assert(line.length() > TranspositionDigitsNumber);
		results.textAsOctaveNumbers.append( line.mid(TranspositionDigitsNumber) );
	}

	std::vector<int*> numbers {&results.firstOctaveNumber, &results.secondOctaveNumber, &results.thirdOctaveNumber};

	for (unsigned position = 0; position < numbers.size(); ++position)
		for (QString const& line : lines)
		{
			if (line[position].isDigit())
			{
				*(numbers[position]) = line[position].digitValue();
				break;
			}
		}

	return results;
}

}
