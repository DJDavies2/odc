/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef eclib_RotLogger_h
#define eclib_RotLogger_h

#include "eclib/DHSLogger.h"


//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class RotLogger : public DHSLogger {
public:
    
    // -- Constructor

    RotLogger();

    // -- Destructor

    virtual ~RotLogger();
    
    // -- Methods
    
    virtual ostream& out();

};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif // eclib_RotLogger_h
