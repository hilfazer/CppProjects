#include "Octave.hpp"

#include <boost/test/unit_test.hpp>

#include <array>
#include <map>


BOOST_AUTO_TEST_SUITE(
	OctaveSuite)

BOOST_AUTO_TEST_CASE(
	TestNormalizeReturnSharpenedForFlattenedSemitones)
{
	std::map<std::string, std::string> const semitonePairs =
		{ {"Db", "C#"}, {"Eb", "D#"}, {"Gb", "F#"}, {"Ab", "G#"}, };

	for (auto const& semitonePair : semitonePairs)
	{
		QString const normalized = Octave::normalize(semitonePair.first.c_str());
		BOOST_CHECK_EQUAL( semitonePair.second, normalized.toStdString() );
	}
}

BOOST_AUTO_TEST_CASE(
	TestNormalizeReturnSameTextForStandardSemitones)
{
	std::array<QString, 12> standardSemitones
		= { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

	for (auto const& semitone : standardSemitones)
	{
		QString const normalized = Octave::normalize(semitone);
		BOOST_CHECK_EQUAL( semitone.toStdString(), normalized.toStdString() );
	}
}

BOOST_AUTO_TEST_CASE(
	TestNormalizeThrowsIfNotASemitonePassed)
{
	QString incorrectInputs[] = {"", "gb", "?", "  ", "D #"};

	for (auto const& incorrectSemitone : incorrectInputs)
	{
		BOOST_CHECK_THROW( Octave::normalize(incorrectSemitone), NotASemitoneException);
	}
}

BOOST_AUTO_TEST_SUITE_END()
