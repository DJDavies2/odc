/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File BufferCache.h
// Baudouin Raoult - (c) ECMWF Jul 11

#ifndef eclib_BufferCache_h
#define eclib_BufferCache_h

#include "eclib/machine.h"

#include "eclib/ResizableBuffer.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class BufferCache {
public:

// -- Exceptions
	// None

// -- Contructors

	BufferCache(size_t = 1024);
	BufferCache(const BufferCache&);
	BufferCache& operator=(const BufferCache&);

// -- Destructor

	~BufferCache(); // Change to virtual if base class

// -- Convertors
	// None

// -- Operators

    bool operator<(const BufferCache& other) const;

// -- Methods
	// None

    void add(const void*, size_t);
	void reset();

	size_t count() const { return count_; }

	const void* buffer() const { return buffer_; }

	time_t updated() const { return updated_; }

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


// -- Members
    
    size_t          count_;
    ResizableBuffer buffer_;
	time_t          updated_;

// -- Methods
	// None

// -- Overridden methods
	// None

// -- Class members
	// None

// -- Class methods
	// None

// -- Friends

	friend ostream& operator<<(ostream& s,const BufferCache& p)
		{ p.print(s); return s; }

};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif
