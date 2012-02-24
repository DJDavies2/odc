/// \file TestTextSelect3.h
///
// @author Piotr Kuchta, ECMWF, Oct 2010

#include <iostream>

using namespace std;

#include "oda.h"
#include "Tool.h"
#include "TestCase.h"
#include "TestTextSelect3.h"
#include "ToolFactory.h"
#include "SimpleFilterIterator.h"
#include "TmpFile.h"
#define SRC __FILE__, __LINE__

namespace odb {
namespace tool {
namespace test {

// TODO: (test not finished yet)
//ToolFactory<TestTextSelect3> _TestTextSelect3("TestTextSelect3");

TestTextSelect3::TestTextSelect3(int argc, char **argv)
: TestCase(argc, argv)
{}

TestTextSelect3::~TestTextSelect3() { }

void TestTextSelect3::setUp() { }

/// Tests syntax 'select lat, lon' (no file name)
///
void TestTextSelect3::test()
{
	selectStarOneColumn();
	selectSumOneColumn();
}

void TestTextSelect3::selectStarOneColumn()
{
	string sql = "select *";
	const string fileName = "2000010106.4.0.oda.csv";//"TestTextSelect3.txt";
	ifstream fs(fileName.c_str());
	
	odb::Select oda(sql, fs);
	
	Log::info(SRC) << "TestTextSelect3::selectStarOneColumn: Execute '" << sql << "'" << endl;
	odb::Select::iterator it = oda.begin();
	odb::Select::iterator end = oda.end();

	Log::info() << "TestTextSelect3::selectStarOneColumn: columns().size():  " << it->columns().size() << endl;

	unsigned long n = 0;
	for( ; it != end; ++it, ++n)
	{
	}

	Log::info() << "TestTextSelect3::selectStarOneColumn: number of rows:  " << n << endl;
}

void TestTextSelect3::selectSumOneColumn()
{
/*
	string sql = "select sum(a), sum(b)";
	const string fileName = "TestTextSelect3.txt";
	ifstream fs(fileName.c_str());
	
	odb::Select oda(sql, fs);
	
	Log::info(SRC) << "TestTextSelect3::selectSumOneColumn: Execute '" << sql << "'" << endl;
	odb::Select::iterator it = oda.begin();
	odb::Select::iterator end = oda.end();

	ASSERT((*it).columns().size() == 2);

	++it;
	ASSERT(! (it != end));
	ASSERT((*it)[0] == 55);
	ASSERT((*it)[1] == 55);
*/
}


void TestTextSelect3::tearDown() {}

} // namespace test
} // namespace tool 
} // namespace odb 

