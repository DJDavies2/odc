/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File Port.h
// Baudouin Raoult - ECMWF Jul 96

#ifndef eclib_Port_h
#define eclib_Port_h

#include "eclib/machine.h"


//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------



class Port {
public:

// -- Contructors

	Port(const string&, int );

// -- Operators

	operator int() const
        { return port_; }

private:

// There is no private copy constructor as this will confuse g++ 4.x.x

// -- Members

	int   port_;

};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif
