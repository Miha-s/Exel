/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_TestParser_init = false;
#include "/home/miha/KNU/OOP_homework/Exel/tests.h"

static TestParser suite_TestParser;

static CxxTest::List Tests_TestParser = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_TestParser( "tests.h", 16, "TestParser", suite_TestParser, Tests_TestParser );

static class TestDescription_suite_TestParser_testMmaxMmin : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestParser_testMmaxMmin() : CxxTest::RealTestDescription( Tests_TestParser, suiteDescription_TestParser, 19, "testMmaxMmin" ) {}
 void runTest() { suite_TestParser.testMmaxMmin(); }
} testDescription_suite_TestParser_testMmaxMmin;

static class TestDescription_suite_TestParser_testDivision : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestParser_testDivision() : CxxTest::RealTestDescription( Tests_TestParser, suiteDescription_TestParser, 25, "testDivision" ) {}
 void runTest() { suite_TestParser.testDivision(); }
} testDescription_suite_TestParser_testDivision;

static class TestDescription_suite_TestParser_testComplex : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_TestParser_testComplex() : CxxTest::RealTestDescription( Tests_TestParser, suiteDescription_TestParser, 32, "testComplex" ) {}
 void runTest() { suite_TestParser.testComplex(); }
} testDescription_suite_TestParser_testComplex;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
