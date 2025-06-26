#include "OctaveKeysParser.hpp"

#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_SUITE(OctaveKeysParserSuite)

BOOST_AUTO_TEST_CASE(TestParseOctaveKeys4KeyLines)
{
	QString textWith4KeyLines =
		"octave keys: \n"
		"Key 1: 6304777*44471*734774*7474744\n"
		"Key *: 6304744*74444*464747*44777\n"
		"Key *: 6304777347742434477473\n"
		"Key *: 6304347*47474*447447*4447::\n"
		"\n"
		;

	QStringList numbersLines;
	numbersLines << "4777*44471*734774*7474744";
	numbersLines << "4744*74444*464747*44777";
	numbersLines << "4777347742434477473";
	numbersLines << "4347*47474*447447*4447::";

	OctaveParsingResults receivedResult = parseOctaveKeys(textWith4KeyLines);
	OctaveParsingResults expectedResult{6, 3, 0, numbersLines};

	BOOST_CHECK(expectedResult == receivedResult);
}

BOOST_AUTO_TEST_CASE(TestParseOctaveKeys2KeyLines)
{
	QString textWith4KeyLines =
		"octave keys: \n"
		"Key 1: 1504777*44471*734774*7474744\n"
		"Key *: 1504744*74444*464747*44777\n"
		"\n"
		;

	QStringList numbersLines;
	numbersLines << "4777*44471*734774*7474744";
	numbersLines << "4744*74444*464747*44777";

	OctaveParsingResults receivedResult = parseOctaveKeys(textWith4KeyLines);
	OctaveParsingResults expectedResult{1, 5, 0, numbersLines};

	BOOST_CHECK(expectedResult == receivedResult);
}



BOOST_AUTO_TEST_SUITE_END()
