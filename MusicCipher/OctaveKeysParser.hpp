#ifndef OCTAVEKEYSPARSER_H
#define OCTAVEKEYSPARSER_H

#include <QString>
#include <QStringList>


struct OctaveParsingResults
{
	int firstOctaveNumber = -1;
	int secondOctaveNumber = -1;
	int thirdOctaveNumber = -1;

	QStringList textAsOctaveNumbers;

	bool operator==(OctaveParsingResults const& other) const;
};

OctaveParsingResults parseOctaveKeys( QString const& keyText );


#endif // OCTAVEKEYSPARSER_H
