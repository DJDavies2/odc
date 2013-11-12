/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef eclib_DHSStdLogger_h
#define eclib_DHSStdLogger_h

#include "eclib/DHSLogger.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class DHSStdLogger : public DHSLogger {
public:
    
    // -- Constructor

    DHSStdLogger( std::ostream& out );

    // -- Destructor

    virtual ~DHSStdLogger();
    
    // -- Methods
    
    virtual ostream& out();

private:
    
    std::ostream* out_;    
    
};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif // eclib_DHSStdLogger_h
