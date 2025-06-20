#include "IntervalKeyParser.hpp"

#include <map>
#include <string>

namespace
{

std::map<const QString, int> const NamesToNumber =
{
    {"perfect unison", 0},
    {"minor second", 1},
    {"major second", 2},
    {"minor third", 3},
    {"major third", 4},
    {"perfect fourth", 5},
    {"augmented fourth", 6},
    {"diminished fifth", 6},
    {"perfect fifth", 7},
    {"minor sixth", 8},
    {"major sixth", 9},
    {"minor seventh", 10},
    {"major seventh", 11},
    {"perfect octave", 12},
};

}


IntervalParsingResults parseIntervalKey(QString const& text)
{
	std::map<int, QString const> semitoneIndices;
	for (auto const& semitonePair : NamesToNumber)
	{
		QString const& name = semitonePair.first;

		for (int i = 0, lastIdx = 0; i < text.count(name, Qt::CaseInsensitive); ++i)
		{
			lastIdx = text.indexOf(name, lastIdx, Qt::CaseInsensitive);
			semitoneIndices.insert({ lastIdx, name });
			lastIdx++;
		}
	}

	IntervalParsingResults results;

	auto semitoneIt = semitoneIndices.begin();
	if (semitoneIt != semitoneIndices.end())
	{
		results.m_transposition = NamesToNumber.at( semitoneIt->second );
		++semitoneIt;
	}
	if (semitoneIt != semitoneIndices.end())
	{
		results.m_firstModulation = NamesToNumber.at( semitoneIt->second );
		++semitoneIt;
	}
	if (semitoneIt != semitoneIndices.end())
	{
		results.m_secondModulation = NamesToNumber.at( semitoneIt->second );
		++semitoneIt;
	}

	return results;
}
