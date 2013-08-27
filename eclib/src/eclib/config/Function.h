/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef eclib_config_Function_H
#define eclib_config_Function_H

#include "eclib/machine.h"

#include "eclib/config/Block.h"
#include "eclib/config/Compiler.h"
#include "eclib/config/Statement.h"

//-----------------------------------------------------------------------------

namespace eclib {
namespace config {

//-----------------------------------------------------------------------------

class Function : public Statement {

public: // methods
    
    Function( Compiler& c, Scope& scope );
    
    virtual ~Function();
    
    virtual void execute( const StringDict& in, StringDict& out );

    virtual void print( std::ostream& out );
    
private: // members

    std::string name_;
    ScopedPtr<Statement>  body_;
    
};

//-----------------------------------------------------------------------------

class FunctionCall : public Statement {

public: // methods
    
    FunctionCall( Compiler& c, Scope& scope );
    
    virtual ~FunctionCall();
    
    virtual void execute( const StringDict& in, StringDict& out );

    virtual void print( std::ostream& out );
    
private: // members

    std::string name_;
    
};

//-----------------------------------------------------------------------------

} // namespace config
} // namespace eclib

#endif
