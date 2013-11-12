/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File MarsFSFile.h
// Baudouin Raoult - (c) ECMWF Jun 11

#ifndef eclib_MarsFSFile_h
#define eclib_MarsFSFile_h

#include "eclib/AutoLock.h"
#include "eclib/MarsFSClient.h"
#include "eclib/MarsFSPath.h"
#include "eclib/TCPClient.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class MarsFSFile : private MarsFSClient {
public:

// -- Exceptions
	// None

// -- Contructors

	MarsFSFile(const MarsFSPath&);

// -- Destructor

	~MarsFSFile(); // Change to virtual if base class

// -- Convertors
	// None

// -- Operators
	// None

// -- Methods
//
    Length open(const char* mode, bool overwrite = false);
    long read(void*,long);
    long write(const void*,long);
    void close();
    void skip(size_t);
    Offset seek(const Offset&);
    Length length();
	

// -- Overridden methods
	// None

// -- Class members
	// None

// -- Class methods
	// None

protected:

// -- Members
	// None

// -- Methods
	
	void print(ostream&) const; // Change to virtual if base class

// -- Overridden methods
	// None

// -- Class members
	// None

// -- Class methods
	// None

private:

// No copy allowed

	MarsFSFile(const MarsFSFile&);
	MarsFSFile& operator=(const MarsFSFile&);

// -- Members
//
    TCPClient data_;
    MarsFSPath path_;
	AutoLock<Connector> lock_;

// -- Methods
	// None

// -- Overridden methods
	// None

// -- Class members
	// None

// -- Class methods
	// None

// -- Friends

	friend ostream& operator<<(ostream& s,const MarsFSFile& p)
		{ p.print(s); return s; }

};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif
