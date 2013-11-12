/*
 * © Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eclib/Buffer.h"
#include "eclib/DataHandle.h"
#include "eclib/Length.h"

#include "eclib/config/Script.h"
#include "eclib/config/Block.h"

//-----------------------------------------------------------------------------

namespace eclib {
namespace config {

//-----------------------------------------------------------------------------

Script::Script()
{
}

Script::Script( Compiler& c )
{
    Block* blk = new Block(c, new Scope() );
    
    ASSERT(c.peek() == 0);
    
    blocks_.push_back(blk);
}

Script::~Script()
{
    clear();
}

void Script::execute(const StringDict &in, StringDict &out)
{
    for( BlockStore::iterator i = blocks_.begin(); i != blocks_.end(); ++i )
        (*i)->execute(in,out);
}

void Script::print(ostream &out)
{
    for( BlockStore::iterator i = blocks_.begin(); i != blocks_.end(); ++i )
        (*i)->print(out);
}

bool Script::readFile(const PathName& path)
{       
    if( path.exists() )
    {
        ifstream in;
        in.open ( path.asString().c_str() );
        if (!in)
            throw CantOpenFile( path.asString() );

//        std::cout << "reading config file [" << path.asString() << "]" << std::endl;
        
        Compiler c(in);
        
        Block* blk = new Block(c, new Scope() );
        
        ASSERT(c.peek() == 0);
        
        blocks_.push_back(blk);
        
        return true;
    }
    return false;
}

void Script::readStream(istream &in)
{
    Compiler c(in);
    
    Block* blk = new Block(c, new Scope() );
    
    ASSERT(c.peek() == 0);
    
    blocks_.push_back(blk);
}

void Script::clear()
{
    for( BlockStore::iterator i = blocks_.begin(); i != blocks_.end(); ++i )
        delete (*i);
}

//-----------------------------------------------------------------------------

} // namespace config
} // namespace eclib
