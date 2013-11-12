/*
 * (C) Copyright 1996-2012 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File JSON.cc
// Baudouin Raoult - (c) ECMWF Aug 11

#include "eclib/JSON.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

JSON::JSON(ostream& out):
    out_(out) ,
    null_(true)
{
    sep_.push_back("");
    state_.push_back(true);
}

JSON::~JSON()
{
    if(null_) out_ << "null";
}

void JSON::sep() 
{
    null_ = false;
    out_ << sep_.back();
    if(indict() && sep_.back() != ":")
        sep_.back() = ":";
    else
        sep_.back() = ",";
}

static ostream& encode(ostream& s,const char *p) {
    s << '"';
    while(*p)
    {
        switch(*p) {

        case '\\':
            s << "\\\\";
            break;

        case '\n':
            s << "\\n";
            break;

        case '\t':
            s << "\\t";
            break;

        case '\b':
            s << "\\b";
            break;

        case '\f':
            s << "\\f";
            break;

        case '\r':
            s << "\\r";
            break;

        case '"':
            s << "\\\"";
            break;

        default:
            s << *p;
            break;
        }
        p++;
    }

    s << '"';
    return s;
}


JSON& JSON::startObject() 
{
    null_ = false;
    sep();
    sep_.push_back("");
    state_.push_back(true);
    out_ << "{";
    return *this;
}

JSON& JSON::null()
{
    null_ = false;
    sep();
    out_ << "null";
    return *this;
}

JSON& JSON::startList() 
{
    null_ = false;
    sep();
    sep_.push_back("");
    state_.push_back(false);
    out_ << "[";
    return *this;
}

JSON& JSON::endObject() 
{
    sep_.pop_back();
    state_.pop_back();
    out_ << "}";
    return *this;
}

JSON& JSON::endList() 
{
    sep_.pop_back();
    state_.pop_back();
    out_ << "]";
    return *this;
}

JSON& JSON::operator<<(const char* s)
{
    null_ = false;
    sep();
    encode(out_, s);
    return *this;
}

JSON& JSON::operator<<(const string& s)
{
    null_ = false;
    sep();
    encode(out_, s.c_str());
    return *this;
}

JSON& JSON::operator<<(int n)
{
    null_ = false;
    sep();
    out_ << n;
    return *this;
}

JSON& JSON::operator<<(char n)
{
    null_ = false;
    sep();
    out_ << '"' << n << '"';
    return *this;
}

JSON& JSON::operator<<(bool n)
{
    null_ = false;
    sep();
    out_ << (n ? "true": "false");
    return *this;
}

JSON& JSON::operator<<(long n)
{
    null_ = false;
    sep();
    out_ << n;
    return *this;
}

JSON& JSON::operator<<(unsigned long n)
{
    null_ = false;
    sep();
    out_ << n;
    return *this;
}

JSON& JSON::operator<<(long long n)
{
    null_ = false;
    sep();
    out_ << n;
    return *this;
}

JSON& JSON::operator<<(unsigned long long n)
{
    null_ = false;
    sep();
    out_ << n;
    return *this;
}

JSON& JSON::operator<<(double n)
{
    null_ = false;
    sep();
    out_ << n;
    return *this;
}

//-----------------------------------------------------------------------------

} // namespace eclib
