#ifndef INTERVALKEYPARSER_HPP
#define INTERVALKEYPARSER_HPP

#include <QString>

struct IntervalParsingResults
{
	int m_transposition = -1;
	int m_firstModulation = -1;
	int m_secondModulation = -1;
};

IntervalParsingResults parseIntervalKey(QString const& text);


#endif // INTERVALKEYPARSER_HPP
