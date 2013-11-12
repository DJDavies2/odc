/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File Time.h
// Manuel Fuentes - ECMWF Oct 96

#ifndef eclib_Time_h
#define eclib_Time_h

#include "eclib/BadValue.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class DumpLoad;
class Bless;

// Forwarded declarations

typedef double Second;

class Time {

public:

// -- Exceptions
	// None

// -- Contructors

	Time(long, long, long);
	Time(long seconds = 0);
	Time(const string&);

#include "eclib/Time.b"

// -- Copy

	Time(const Time&);
	Time& operator=(const Time&);

// -- Destructor

	~Time();

// -- Convertors

	operator string() const;
	operator Second() const { return seconds_; }

// -- Operators

	bool operator==(const Time& other) const
		{ return seconds_ == other.seconds_; }

	bool operator!=(const Time& other) const
		{ return (seconds_ != other.seconds_); }

	bool operator>(const Time& other) const
		{ return (seconds_ > other.seconds_); }

	bool operator<(const Time& other) const
		{ return (seconds_ < other.seconds_); }

	bool operator>=(const Time& other) const
		{ return (seconds_ >= other.seconds_); }

	bool operator<=(const Time& other) const
		{ return (seconds_ <= other.seconds_); }

	Second operator-(const Time& other) const
		{ return seconds_ - other.seconds_; }

//  Does not make sens
//	Time operator+(const Time& other) const
//		{ return seconds_ + other.seconds_; }

	Time& operator+=(const Second& sec)
		{ seconds_ += sec; return *this; }	

	Time& operator-=(const Second& sec)
		{ seconds_ -= sec; return *this; }	

// -- Methods
	long hours() const;
	long minutes() const;
	long seconds() const;
	long hhmmss() const;

	void dump(DumpLoad&) const;
	void load(DumpLoad&);

// -- Class Methods

	static Time now();

protected:

// -- Methods

	void print(ostream&) const;

private:

// -- Members

	Second seconds_;

	friend ostream& operator<<(ostream& s,const Time& t)
		{ t.print(s); return s; }

};


class BadTime: public BadValue {
public:
	BadTime(const string& t);

};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif
