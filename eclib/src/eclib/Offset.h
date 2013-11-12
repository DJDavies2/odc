/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File Offset.h
// Baudouin Raoult - ECMWF Jul 96

#ifndef eclib_Offset_h
#define eclib_Offset_h

#include "eclib/Length.h"
#include "eclib/Stream.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

// Forwarded declarations

class Bless;

// But because the compiler aligns long longs
// on 64bits boundaries and longs on 32 bits boundaries,
// we need the help of a little pragma here, to make ObjectStore happy

#ifdef AIX
#pragma options align=twobyte
#endif

class Offset {
public:

	friend ostream& operator<<(ostream& s,const Offset& x)
		{ return s << x.value_; }

	friend Stream& operator<<(Stream& s,const Offset& x)
		{ return s << x.value_; }

	friend Stream& operator>>(Stream& s,Offset& x)
		{ s >> x.value_; return s;}

	//Offset(fpos_t); <- To implement
	Offset(long long l = 0) : value_(l) {}
	Offset(const Offset& other) : value_(other.value_) {}

#include "eclib/Offset.b"

	Offset& operator=(const Offset& other) 
		{ value_ = other.value_; return *this;}

	bool operator==(const Offset& other) const
		{ return value_ == other.value_; }

	bool operator!=(const Offset& other) const
		{ return value_ != other.value_; }

	bool operator<(const Offset& other) const
		{ return value_ < other.value_; }

	bool operator>(const Offset& other) const
		{ return value_ > other.value_; }

	bool operator<=(const Offset& other) const
		{ return value_ <= other.value_; }

	bool operator>=(const Offset& other) const
		{ return value_ >= other.value_; }

	Offset operator+(const Length& other) const
		{ return Offset(value_ + other.value_);}

	void operator+=(const Length& other)
		{ value_ += other.value_;}

	void operator-=(const Length& other)
		{ value_ -= other.value_;}

	Length operator-(const Offset& other) const
		{ return value_ - other.value_;}

	
	operator long long() const { return value_; }		

	void dump(DumpLoad&) const;
	void load(DumpLoad&);

private:
    
// -- Members

	long long value_;

	friend class Length;
};

typedef vector<Offset> OffsetList;

//-----------------------------------------------------------------------------

// Global routines

// Sort both vector according to offset
void sort(OffsetList&,LengthList&);
bool compress(OffsetList&,LengthList&);
void accumulate(const LengthList&,OffsetList&,const Offset& = 0);

#ifdef AIX
#pragma options align=reset
#endif

//-----------------------------------------------------------------------------

} // namespace eclib

#endif
