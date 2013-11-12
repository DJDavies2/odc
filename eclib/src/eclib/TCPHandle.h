/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File TCPHandle.h
// Baudouin Raoult - ECMWF Jul 96

#ifndef eclib_TCPHandle_h
#define eclib_TCPHandle_h

#include "eclib/DataHandle.h"
#include "eclib/TCPClient.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class TCPHandle : public DataHandle {
public:

// -- Contructors
	
	TCPHandle(Stream&);
	TCPHandle(const string& host,int port);

// -- Destructor

	~TCPHandle();

// -- Overridden methods

	// From DataHandle

    virtual Length openForRead();
    virtual void openForWrite(const Length&);
    virtual void openForAppend(const Length&);

	virtual long read(void*,long);
	virtual long write(const void*,long);
	virtual void close();
	virtual void rewind();
	virtual void print(ostream&) const;
	virtual string title() const;
    virtual bool moveable() const { return true; }

	// From Streamable

	virtual void encode(Stream&) const;
	virtual const ReanimatorBase& reanimator() const { return reanimator_; }

// -- Class methods

	static  const ClassSpec&  classSpec()         { return classSpec_;}

protected:

// -- Members

	string      host_;
	int         port_;
	TCPClient   connection_;

private:


// -- Class members

    static  ClassSpec               classSpec_;
	static  Reanimator<TCPHandle>  reanimator_;

};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif
