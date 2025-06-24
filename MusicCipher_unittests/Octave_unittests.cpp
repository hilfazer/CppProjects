
#include <boost/test/unit_test.hpp>

#include "Octave.hpp"

BOOST_AUTO_TEST_SUITE(
	OctaveSuite)

BOOST_AUTO_TEST_CASE(
	NormalizeSemitone)
{
	QString const normalized = Octave::normalize(QString("sda"));
	BOOST_CHECK_EQUAL( "sda", normalized.toStdString() );
}

BOOST_AUTO_TEST_SUITE_END()
