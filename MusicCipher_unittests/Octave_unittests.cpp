#include "Octave.hpp"

#include <boost/test/unit_test.hpp>

#include <array>
#include <map>


BOOST_AUTO_TEST_SUITE(OctaveSuite)

BOOST_AUTO_TEST_CASE(TestNormalizeReturnSharpenedForFlattenedSemitones)
{
	std::map<std::string, std::string> const semitonePairs =
		{ {"Db", "C#"}, {"Eb", "D#"}, {"Gb", "F#"}, {"Ab", "G#"}, };

	for (auto const& semitonePair : semitonePairs)
	{
		QString const normalized = Octave::normalize(semitonePair.first.c_str());
		BOOST_CHECK_EQUAL( semitonePair.second, normalized.toStdString() );
	}
}

BOOST_AUTO_TEST_CASE(TestNormalizeReturnSameTextForStandardSemitones)
{
	std::array<QString, 12> standardSemitones
		= { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };

	for (auto const& semitone : standardSemitones)
	{
		QString const normalized = Octave::normalize(semitone);
		BOOST_CHECK_EQUAL( semitone.toStdString(), normalized.toStdString() );
	}
}

BOOST_AUTO_TEST_CASE(TestNormalizeThrowsIfNotASemitonePassed)
{
	QString incorrectInputs[] = {"", "gb", "?", "  ", "D #"};

	for (auto const& incorrectSemitone : incorrectInputs)
	{
		BOOST_CHECK_THROW( Octave::normalize(incorrectSemitone), NotASemitoneException);
	}
}

BOOST_AUTO_TEST_CASE(TestGetSemitoneFromOctave)
{
	Octave o1{2, 0, 0};
	BOOST_CHECK_EQUAL( o1.getSemitone(0).toStdString(), "C" );
	BOOST_CHECK_EQUAL( o1.getSemitone(3).toStdString(), "D#" );

	Octave o2{2, 11, 0};
	BOOST_CHECK_EQUAL( o2.getSemitone(0).toStdString(), "B" );
	BOOST_CHECK_EQUAL( o2.getSemitone(1).toStdString(), "C" );
}

BOOST_AUTO_TEST_CASE(TestGetOctaveNumberTransposition0)
{
	Octave octave{4, 0, 1};
	std::map<unsigned, unsigned> index2octaveNumber
	{
		{0, 4}, {1, 4}, {11, 4}, {8, 4},
	};

	for (auto const& idx2num : index2octaveNumber)
	{
		BOOST_CHECK_EQUAL(idx2num.second, octave.getOctaveNumber(idx2num.first));
	}
}

BOOST_AUTO_TEST_CASE(TestGetOctaveNumberTransposition11)
{
	Octave octave{1, 11, 1};
	std::map<unsigned, unsigned> index2octaveNumber
	{
		{0, 1}, {1, 2}, {11, 2}, {8, 2},
	};

	for (auto const& idx2num : index2octaveNumber)
	{
		BOOST_CHECK_EQUAL(idx2num.second, octave.getOctaveNumber(idx2num.first));
	}
}

BOOST_AUTO_TEST_CASE(TestFindSemitone)
{
	Octave o1{2, 2, 0};
	BOOST_CHECK_EQUAL( o1.findSemitone("D"), 0 );
	BOOST_CHECK_EQUAL( o1.findSemitone("G#"), 6 );

	Octave o2{2, 4, 0};
	BOOST_CHECK_EQUAL( o2.findSemitone("D"), 10 );
}

BOOST_AUTO_TEST_SUITE_END()
