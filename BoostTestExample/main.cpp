#define BOOST_TEST_MODULE My test module
#include <boost/test/included/unit_test.hpp>

int add(int a, int b)
{
	return a + b;
}

BOOST_AUTO_TEST_SUITE(SuiteExample)

BOOST_AUTO_TEST_CASE(CaseExample)
{
	BOOST_TEST(true /* test assertion */);
}

BOOST_AUTO_TEST_CASE(TestAdd)
{
	BOOST_TEST(add(2,3) == 5);
}

BOOST_AUTO_TEST_CASE(TestAddFail)
{
	BOOST_TEST(add(2,3) == 3);
}


BOOST_AUTO_TEST_SUITE_END()
