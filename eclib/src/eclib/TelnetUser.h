/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File TelnetUser.h
// Manuel Fuentes - ECMWF Jul 96

#ifndef eclib_TelnetUser_h
#define eclib_TelnetUser_h

#include "eclib/NetUser.h"


//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class TelnetUser : public NetUser {
public:
	TelnetUser(TCPSocket&);
	virtual ~TelnetUser();
static void terminate(TelnetUser& other)	{ other.stop(); }

private:
	virtual void serve(Stream&, istream&,ostream&);
	string from_;
};



//-----------------------------------------------------------------------------

} // namespace eclib

#endif
