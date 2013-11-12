/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef eclib_LibBehavior_h
#define eclib_LibBehavior_h

#include "eclib/CallbackLogger.h"
#include "eclib/StandardBehavior.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class LibBehavior : public StandardBehavior {

public: // methods

    /// Contructors

	LibBehavior();

    /// Destructor

	~LibBehavior();
        
    void register_logger_callback(CallbackLogger::callback c, void* ctxt = 0 );
    
private: // interface methods
        
    virtual Logger* createInfoLogger();
    virtual Logger* createDebugLogger();
    virtual Logger* createWarningLogger();
    virtual Logger* createErrorLogger();

protected: // members
    
    CallbackLogger* info_;
    CallbackLogger* debug_;
    CallbackLogger* warn_;
    CallbackLogger* error_;
};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif // eclib_LibBehavior_h
