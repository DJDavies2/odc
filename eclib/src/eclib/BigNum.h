/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File BigNum.h
// Baudouin Raoult - ECMWF Nov 96

#ifndef eclib_BigNum_h
#define eclib_BigNum_h

#include "eclib/machine.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

/// Class used to print large numbers

class BigNum {
public:

// -- Contructors

	BigNum(long long v) : value_(v) {}

private:

// There is no private copy constructor as this will confuse g++ 4.x.x

// -- Methods

	void print(ostream&) const; 

// -- Class methods

	static void print(ostream&,long long);

// -- Members

	long long value_;

// -- Friends

	friend ostream& operator<<(ostream& s,const BigNum& p)
		{ p.print(s); return s; }

};

//-----------------------------------------------------------------------------

} // namespace eclib

#endif
