/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File SockBuf.h
// Baudouin Raoult - ECMWF Mar 97

#ifndef eclib_SockBuf_h
#define eclib_SockBuf_h

#include "eclib/TCPSocket.h"


//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class SockBuf : public streambuf  {
public:

// -- Contructors

	SockBuf(TCPSocket& proto);

// -- Destructor

	~SockBuf();

private:

// No copy allowed

	SockBuf(const SockBuf&);
	SockBuf& operator=(const SockBuf&);

// -- Members

	char in_[1];
	char out_[80];
	TCPSocket& protocol_;

// -- Overridden methods

	// From streambuf

	virtual int overflow(int c);
	virtual int underflow();
	virtual int sync();
//	virtual int uflow();

};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif
