#ifndef DECODEDTEXTPARSER_H
#define DECODEDTEXTPARSER_H

#include <QStringList>

#include <vector>
#include <map>


class QString;
class Octave;

QString decodeText(
	        QString const& encodedText
	        , const std::vector<Octave>& octaves
	        , QStringList const& textAsOctaveNumbers
	        );

std::vector< std::map<int, QString> > filteredTextWithPositions(const QString& encodedText);


#endif // DECODEDTEXTPARSER_H
