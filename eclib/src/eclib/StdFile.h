/*
 * (C) Copyright 1996-2012 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File StdFile.h
// Baudouin Raoult - ECMWF Jun 96

#ifndef eclib_StdFile_h
#define eclib_StdFile_h

#include <stdio.h>

#include "eclib/PathName.h"


//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

// Simple wrapper around a stdio file

class PathName;

class StdFile {
public:

// -- Contructors

	StdFile(const PathName& name,const string& mode = "r");

// -- Destructor

	~StdFile();

// -- Convertors

	operator FILE*() { return file_; } // don't call fclose !!!

private:

// No copy allowed

	StdFile(const StdFile&);
	StdFile& operator=(const StdFile&);

// -- Members

	FILE *file_;

};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif
