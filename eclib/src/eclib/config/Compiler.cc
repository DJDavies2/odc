/*
 * © Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <ctype.h>

#include "eclib/Log.h"
#include "eclib/Tokenizer.h"
#include "eclib/StringTools.h"

#include "eclib/config/Compiler.h"

//-----------------------------------------------------------------------------

namespace eclib {
namespace config {

//-----------------------------------------------------------------------------

static bool isUnderscore( char c ) { return c == '_'; }
static bool isDot( char c )        { return c == '.'; }
static bool isSpace( char c )      { return c == ' '; }

Compiler::Compiler( istream& in ) : StreamParser(in) {}

Compiler::~Compiler() {}

static const bool with_spaces = true;

string Compiler::consumeToEOL()
{
    string s;
    char n = peek(with_spaces);
    while( n && n != '\n' )
    {
        s += next(with_spaces);
        n = peek(with_spaces);
    }
    return s;
}

void Compiler::consumeComment()
{
    consume('#');
    consumeToEOL();
}

string Compiler::parseIdentifier()
{
    string s;
    char c = peek();
    
    if( isdigit(c) )
        throw StreamParser::Error(string("Compiler::parseIdentifier -- 1st char in identifier cannot be digit"));        
    
    while( isIdentifier(c) )
    {
        s += next();
        c = peek();
    }
    
    if( s.empty() )
        throw StreamParser::Error(string("Compiler::parseIdentifier -- empty identifier"));
    
//    std::cout << "identifier [" << s << "]" << std::endl;    
    
    return s;
}

string Compiler::parseValue( bool dontBreakOnSpace )
{    
    string s;
    char c = peek();
    
    if( c == '\'' || c == '"')
    {
        char quote = c;
        consume(quote);
        c = peek(with_spaces);
        while( c != quote )
        {
            s += next(with_spaces);
            c = peek(with_spaces);
        }
        consume(quote);
        return s;
    }
    
    while( c && ( dontBreakOnSpace || !isspace(c) ) && c != '\n' && c != '#' && c != ';' && c != '}' )
    {
        s += next(with_spaces);
        c = peek(with_spaces);
    }
    
//    std::cout << "value [" << s << "]" << std::endl;    
    if( dontBreakOnSpace )
        return StringTools::trim(s);
    else
        return s;
}

StringList Compiler::parseCondition()
{
    StringList s;

    s.push_back( parseValue() );
    
    char c = peek();
    while( c == '|' )
    {
       consume("||");
       s.push_back( parseValue() );
       c = peek();
    }
    
//    std::cout << "condition [" << s << "]" << std::endl;

    return s;
}

bool Compiler::isIdentifier(char c)
{
    return ( isalnum(c) || isUnderscore(c) || isDot(c) );
}

//-----------------------------------------------------------------------------

} // namespace config
} // namespace eclib
