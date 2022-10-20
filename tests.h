#include <cxxtest/TestSuite.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Parser.hpp"

std::string tostring(double num)
{
	std::stringstream strg;
	strg << std::fixed;
	strg << std::setprecision(2);
	strg << num;
	return strg.str();
}

class TestParser : public CxxTest::TestSuite
{
public:
	void testMmaxMmin(void)
	{
		parser::Parser pars([](std::string)
							{ return 1.0; });
		TS_ASSERT_EQUALS(tostring(pars.parse("mmax(12, 67, 1)")), "67.00");
	}
	void testDivision(void)
	{
		parser::Parser pars([](std::string){
			return 1.0;
		});
		TS_ASSERT_EQUALS(tostring(pars.parse("123/3")), "41.00");
	}
	void testComplex(void)
	{
		parser::Parser pars([](std::string){
			return 1.0;
		});
		TS_ASSERT_EQUALS(tostring(pars.parse("mmax(1-23, (mmin(34, 3-1, 5^7)-44), 5^5)")), "3125.00");
	}
};