/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef eclib_Plural_h
#define eclib_Plural_h

#include "eclib/BigNum.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class Plural {
public:

// -- Contructors

	Plural(int count,const string& s) : s_(s),count_(count) {}

// -- Destructor

	~Plural() {}

protected:

// -- Methods
	
	void print(ostream& s) const 
	{
		s << BigNum(count_) << ' ' << s_;
		if(count_>1) s << 's';
	}

private:

// There is no private copy constructor as this will confuse g++ 4.x.x

	string s_;
	int count_;

// -- Friends

	friend ostream& operator<<(ostream& s,const Plural& p)
		{ p.print(s); return s; }

};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif
