/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

//#include "odblib/odb_api.h"

//#include "odblib/Tool.h"
//#include "odblib/ToolFactory.h"
#include "ODAHeaderTool.h"
#include "odblib/MetaDataReaderIterator.h"
#include "odblib/MetaDataReader.h"
//#include "eckit/io/Offset.h"
//#include "eckit/io/Length.h"

using namespace eckit;

namespace odb {
namespace tool {

typedef odb::MetaDataReader<odb::MetaDataReaderIterator> MDReader;

class MDPrinter {
public:
	virtual void print(std::ostream&, MDReader::iterator &) = 0;
};

class VerbosePrinter : public MDPrinter {
public:
	VerbosePrinter() : headerCount_() {}
	void print(std::ostream& o, MDReader::iterator &r)
	{
        o << std::endl << "Header " << ++headerCount_ << ". "
			<< "Begin offset: " << (**r).blockStartOffset() << ", end offset: " << (**r).blockEndOffset()
			<< ", number of rows in block: " << r->columns().rowsNumber() 
			<< ", byteOrder: " << (((**r).byteOrder() == 1) ? "same" : "other")
            << std::endl
			<< r->columns();
	}
private:
	unsigned long headerCount_;
};

class OffsetsPrinter : public MDPrinter {
public:
	OffsetsPrinter() {}
	void print(std::ostream& o, MDReader::iterator &r)
	{
		Offset offset ((**r).blockStartOffset());
		Length length ((**r).blockEndOffset() - (**r).blockStartOffset());
		o << offset << " " << length << " " << r->columns().rowsNumber() << " " << r->columns().size() << std::endl;
	}
private:
	unsigned long headerCount_;
};

HeaderTool::HeaderTool (int argc, char *argv[]) : Tool(argc, argv) {}


void HeaderTool::run()
{
	if (parameters().size() < 2)
	{
		Log::error() << "Usage: ";
		usage(parameters(0), Log::error());
		Log::error() << std::endl;
		return;
	}

	std::string db = parameters(1);

	ostream& o = cout;
	VerbosePrinter verbosePrinter;
	OffsetsPrinter offsetsPrinter;
	MDPrinter& printer(* (optionIsSet("-offsets")
		? static_cast<MDPrinter*>(&offsetsPrinter)
		: static_cast<MDPrinter*>(&verbosePrinter)));

	MDReader oda(db);
	MDReader::iterator r(oda.begin());
	MDReader::iterator end(oda.end());

	odb::MetaData metaData(r->columns());
	for(; r != end; ++r)
	{
		ASSERT (r->isNewDataset());
		printer.print(o, r);
		metaData = r->columns();
	}
}

} // namespace tool 
} // namespace odb 

