/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eclib/Tokenizer.h"
#include "eclib/Types.h"
#include "eclib/StringTools.h"

#include "odblib/odb_api.h"
#include "odblib/Tool.h"
#include "odblib/ToolFactory.h"
#include "odblib/ImportTool.h"
#include "odblib/SQLSelectFactory.h"

using namespace std;
using namespace eclib;

namespace odb {
namespace tool {

string ImportTool::defaultDelimiter_ = ",";

ImportTool::ImportTool(int argc, char *parameters[])
: Tool(argc, parameters)
{
	registerOptionWithArgument("-d"); // Delimiter
}

void ImportTool::run()
{
	Log::info() << "parameters().size(): " << parameters().size() << endl;
	Log::info() << "ImportTool::run: parameters(): " << *static_cast<CommandLineParser*>(this) << endl;

	if (parameters().size() != 3)
	{
		Log::error() << "Usage: ";
		usage(parameters(0), Log::error());
		Log::error() << endl;
		return;
	}

	PathName inFile = parameters(1);
	PathName outFile = parameters(2);

	Log::info() << "ImportTool::run: inFile: " << inFile << ", outFile: " << outFile << endl;

	string delimiter = StringTools::upper(optionArgument("-d", defaultDelimiter()));
	if (delimiter == "TAB")
		delimiter = "\t";
	
	importFile(inFile, outFile, delimiter);
}

void ImportTool::importFile(const PathName& in, const PathName& out, const string& delimiter)
{
	odb::sql::SQLSelectFactory::instance().csvDelimiter(delimiter);

	ifstream fs( in.asString().c_str() );
	odb::Select input("select * ;", fs, delimiter);

	odb::Writer<> writer(out);
	odb::Writer<>::iterator output(writer.begin());

	odb::Select::iterator it = input.begin();
	odb::Select::iterator end = input.end();

	unsigned long long n = output->pass1(it, end);

    Log::info() << "ImportTool::importFile: = copied " << n << " rows." << endl;
}

void ImportTool::importText(const string& s, const PathName& out, const string& delimiter)
{
	odb::sql::SQLSelectFactory::instance().csvDelimiter(delimiter);

	stringstream fs(s);
	odb::Select input("select *;", fs, delimiter);

	odb::Writer<> writer(out);
	odb::Writer<>::iterator output(writer.begin());

	odb::Select::iterator it = input.begin();
	odb::Select::iterator end = input.end();

	unsigned long long n = output->pass1(it, end);

    Log::info() << "ImportTool::importText: = copied " << n << " rows." << endl;
}

} // namespace tool 
} // namespace odb 

