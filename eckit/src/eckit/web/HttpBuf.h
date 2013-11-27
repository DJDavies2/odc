/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File HttpBuf.h
// Baudouin Raoult - ECMWF Oct 96

#ifndef HttpBuf_H
#define HttpBuf_H

#include "eckit/web/Url.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

class HttpStream;

class HttpBuf : public streambuf {


	char    out_[4096];
	virtual int overflow(int c);
	virtual int sync();

	HttpStream& owner_;

public:

	HttpBuf(HttpStream&);
	~HttpBuf();

	void write(ostream&, Url&);

	static ostream& dontEncode(ostream&);
	static ostream& doEncode(ostream&);

private:
	
	vector<char>  buffer_;
};

//-----------------------------------------------------------------------------

class HttpStream : public ostream {
	HttpBuf* buf_;
public:
	HttpStream();
	~HttpStream();
	void write(ostream&,Url&);
};


//-----------------------------------------------------------------------------

} // namespace eckit

#endif
