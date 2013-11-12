/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File Month.h
// Baudouin Raoult - ECMWF Sep 96

#ifndef eclib_Month_h
#define eclib_Month_h

#include "eclib/Date.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class Month {
public:

// -- Contructors

	Month(const Date& = Date(0));
	Month(long,long);
	Month(const string&);

#include "eclib/Month.b"

// -- Destructor

	~Month() {}

// -- Convertors
	
	operator string() const;

// -- Operators

	bool operator==(const Month& other) const
		{ return date_ == other.date_ ;}

	bool operator!=(const Month& other) const
		{ return date_ != other.date_ ;}

	bool operator<(const Month& other)  const
		{ return date_ <  other.date_ ;}

	bool operator>(const Month& other)  const
		{ return date_ >  other.date_ ;}

	bool operator<=(const Month& other)  const
		{ return date_ <=  other.date_ ;}

	bool operator>=(const Month& other)  const
		{ return date_ >=  other.date_ ;}


// -- Methods

	long year() const  { return date_.year(); }
	long month() const { return date_.month(); }

	void dump(DumpLoad&) const;
	void load(DumpLoad&);

// -- Class methods


// -- Friends

	friend ostream& operator<< (ostream& s, const Month& month)
		{ month.print(s); return s; }

protected:


// -- Members
	// None

// -- Methods
	// None

// -- Overridden methods
	// None

// -- Class members
	// None

// -- Class methods
	// None

private:

// -- Members
	
	Date date_;

// -- Methods

	void print(ostream&) const;

// -- Class methods

};

//-----------------------------------------------------------------------------

} // namespace eclib

#endif
