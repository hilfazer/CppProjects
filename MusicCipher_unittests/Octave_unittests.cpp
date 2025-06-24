#include "Octave.hpp"

#include <boost/test/unit_test.hpp>

#include <map>


BOOST_AUTO_TEST_SUITE(
	OctaveSuite)

BOOST_AUTO_TEST_CASE(
	NormalizeSemitone)
{
	std::map<std::string, std::string> const semitonePairs =
		{ {"ab", "ab"}, {"Db", "C#"}, {"", ""}, {"Gb", "F#"}, {"Ab", "G#"}, };

	for (auto const& semitonePair : semitonePairs)
	{
		QString const normalized = Octave::normalize(semitonePair.first.c_str());
		BOOST_CHECK_EQUAL( semitonePair.second, normalized.toStdString() );
	}
}

BOOST_AUTO_TEST_SUITE_END()
