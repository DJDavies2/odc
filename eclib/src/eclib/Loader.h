/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File Loader.h
// Baudouin Raoult - ECMWF Jul 96

#ifndef eclib_Loader_h
#define eclib_Loader_h

#include "eclib/machine.h"

#include "eclib/ClassExtent.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class Loader : public ClassExtent<Loader> {

public: // methods

	Loader();
	virtual ~Loader();

	virtual void execute() = 0;

private: // methods

// There is no private copy constructor as this will confuse g++ 4.x.x

};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif
