/// \file TestSimpleFilterIterator2.h
///
/// @author Piotr Kuchta, ECMWF, June 2009

#include <iostream>

using namespace std;

#include "oda.h"
#include "Tool.h"
#include "TestCase.h"
#include "TestSimpleFilterIterator2.h"
#include "ToolFactory.h"
#include "SimpleFilter.h"
#include "SimpleFilterIterator.h"
#define SRC __FILE__, __LINE__

namespace odb {
namespace tool {
namespace test {

ToolFactory<TestSimpleFilterIterator2> _TestSimpleFilterIterator2("TestSimpleFilterIterator2");

TestSimpleFilterIterator2::TestSimpleFilterIterator2(int argc, char **argv)
: TestCase(argc, argv)
{}

TestSimpleFilterIterator2::~TestSimpleFilterIterator2() { }

/// Tests DispatchingWriter
///
void TestSimpleFilterIterator2::test()
{
	const string fileName = "../odb2oda/2000010106/ECMA.odb";
	string sql = string("select * from \"") + fileName + "\" where obstype = 7 and sensor = 1;";
	odb::Reader oda(fileName);
	odb::Select odas(sql, fileName);
	long n1 = 0;
	long n2 = 0;

	{
		Timer t("TestSimpleFilterIterator2::test: selecting rows where obstype == 7 and sensor = 1");
		typedef odb::SimpleFilter<odb::Reader::iterator> Filter;
		typedef odb::SimpleFilter<Filter::iterator> Filter2;

		Filter filterObstype(oda.begin(), oda.end(), "obstype", 7.0);
		Filter2 filterObstypeAndSensor(filterObstype.begin(), filterObstype.end(), "sensor", 1.0);

		Filter2::iterator it = filterObstypeAndSensor.begin();
		const Filter2::iterator end = filterObstypeAndSensor.end();

		for (; it != end; ++it)
			++n1;

		Log::info() << "TestSimpleFilterIterator2::test: selected " << n1 << " rows." << endl;
	}

	{
		Timer t("TestSimpleFilterIterator2::test: selecting rows using SQL where obstype == 7 and sensor = 1");
		Log::info() << "TestSimpleFilterIterator2::test: Execute '" << sql << "'" << endl;
		odb::Select::iterator end = odas.end();
		for(odb::Select::iterator it = odas.begin();
			it != end; ++it)
			++n2;
		Log::info() << "TestSimpleFilterIterator2::test: selected " << n2 << " rows." << endl;
	}

	Log::info() << "TestSimpleFilterIterator2::test: n1=" << n1 << ", n2=" << n2 << endl;

	ASSERT(n1 == n2);
}

void TestSimpleFilterIterator2::setUp() {}
void TestSimpleFilterIterator2::tearDown() {}

} // namespace test
} // namespace tool 
} // namespace odb 

