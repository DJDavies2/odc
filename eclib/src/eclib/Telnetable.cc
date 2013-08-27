/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include "eclib/Telnet.h"
#include "eclib/Telnetable.h"


//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

Telnetable::Telnetable(int port):
	telnet_(new Telnet(port))
{
	if(port != 0)
		telnet_.start();
}

Telnetable::~Telnetable()
{
	telnet_.stop();
}

//-----------------------------------------------------------------------------

} // namespace eclib

