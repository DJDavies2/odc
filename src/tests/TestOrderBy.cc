/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// \file TestOrderBy.h
///
/// @author Piotr Kuchta, ECMWF, September 2010

#include "eckit/filesystem/PathName.h"
#include "eckit/utils/StringTools.h"
#include "odblib/ImportTool.h"
#include "odblib/Select.h"
#include "odblib/ToolFactory.h"
#include "TestOrderBy.h"

using namespace std;
using namespace eckit;

namespace odb {
namespace tool {
namespace test {

ToolFactory<TestOrderBy> _TestOrderBy("TestOrderBy");

TestOrderBy::TestOrderBy(int argc, char **argv)
: TestCase(argc, argv)
{}

TestOrderBy::~TestOrderBy() {}

///
void TestOrderBy::test()
{
	{
		string sql = "select distinct a from \"a1to10twice.odb\" order by a;";
		Log::info() << "Executing: '" << sql << "'" << std::endl;

		odb::Select sel(sql);
		odb::Select::iterator it = sel.begin();
		odb::Select::iterator end = sel.end();

		Log::info()  << "TestOrderBy::test: entering the loop" << std::endl;
		int i = 0;
		for (; it != end; ++it)
		{
			int v = (*it)[0];
			Log::info()  << "TestOrderBy::test:" <<  v  << std::endl;
			ASSERT(v == ++i);
		}
		Log::info()  << "TestOrderBy::test: i = " <<  i  << std::endl;
		ASSERT(i == 10);
	}

	{
		string sql = "select a from \"a1to10twice.odb\" order by a;";
		Log::info() << "Executing: '" << sql << "'" << std::endl;

		odb::Select sel(sql);
		odb::Select::iterator it = sel.begin();
		odb::Select::iterator end = sel.end();

		Log::info()  << "TestOrderBy::test: entering the loop" << std::endl;
		int i = 0, j = 0;
		for (; it != end; ++it, ++j)
		{
			int v = (*it)[0];
			Log::info()  << "TestOrderBy::test:" <<  v  << std::endl;
			ASSERT(i <= v);
			i = v;
		}
		Log::info()  << "TestOrderBy::test: i=" <<  i  << ", j=" << j << std::endl;
		ASSERT(i == 10);
		ASSERT(j == 20);
	}

	{
		string sql = "select distinct a from \"a1to10twice.odb\" order by a desc;";
		Log::info() << "Executing: '" << sql << "'" << std::endl;

		odb::Select sel(sql);
		odb::Select::iterator it = sel.begin();
		odb::Select::iterator end = sel.end();

		Log::info()  << "TestOrderBy::test: entering the loop" << std::endl;
		int i = 10, j = 0;
		for (; it != end; ++it, ++j)
		{
			int v = (*it)[0];
			Log::info()  << "TestOrderBy::test:" <<  v  << std::endl;
			ASSERT(i-- == v);
		}
		Log::info()  << "TestOrderBy::test: i = " <<  i  << std::endl;
		ASSERT(i == 0);
		ASSERT(j == 10);
	}

	{
		const char *in =
		"a:REAL,b:REAL,c:STRING\n"
		"1,10,'one'\n"
		"1,20,'two'\n"
		"2,30,'three'\n"
		"2,40,'four'\n";
		ImportTool::importText(in, "TestOrderBy4.odb");

		string sql = "select distinct a,b,c from \"TestOrderBy4.odb\" order by a desc, b asc;";
		Log::info() << "Executing: '" << sql << "'" << std::endl;

		odb::Select sel(sql);
		odb::Select::iterator it = sel.begin();
		odb::Select::iterator end = sel.end();

		Log::info()  << "TestOrderBy::test: entering the loop" << std::endl;
        int i = 0, v1 = 0 , v2 = 0;
		string s;
		for (; it != end; ++it, ++i)
		{
			v1 = (*it)[0];
			v2 = (*it)[1];
			s = (*it).string(2);
			Log::info() << "TestOrderBy::test:" <<  v1  << ", " << v2 << ", '" << s << "'" << std::endl;
		}
		Log::info()  << "TestOrderBy::test: i = " <<  i  << std::endl;
		ASSERT(i == 4);
		ASSERT(v1 == 1 && v2 == 20 && StringTools::trim(s) == "two");
	}
}

void TestOrderBy::setUp() {}

void TestOrderBy::tearDown() {}

} // namespace test 
} // namespace tool 
} // namespace odb 

