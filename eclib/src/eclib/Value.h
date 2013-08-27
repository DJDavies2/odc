/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File Value.h
// Manuel Fuentes - ECMWF Jun 97

#ifndef eclib_Value_h
#define eclib_Value_h

#include "eclib/Content.h"
#include "eclib/Date.h"
#include "eclib/DateTime.h"


//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class Length;
class PathName;
class JSON;

typedef vector<Value>     ValueList;
typedef map<Value,Value>  ValueMap;

class Value {
public:

// -- Contructors

    Value();

    Value(bool);
    Value(int);
    Value(long);
    Value(long long);
    Value(unsigned int);
    Value(unsigned long);
    Value(unsigned long long);
    Value(double);

    Value(const string&);
    Value(const char*);

    Value(const Length&);
    Value(const Date&);
    Value(const Time&);
    Value(const DateTime&);
    Value(const PathName&);

    Value(Stream&);
    Value(const ValueList&);

// -- Copy

	Value(const Value&);
	Value& operator=(const Value&);

// -- Destructor

	~Value();

// -- Operators

    operator short() const              { long long l;  content_->value(l); return l; }
    operator unsigned short() const     { long long l;  content_->value(l); return l; }

    operator int() const                { long long l;  content_->value(l); return l; }
    operator unsigned int() const       { long long l;  content_->value(l); return l; }

    operator long() const               { long long l;  content_->value(l); return l; }
    operator unsigned long() const      { long long l;  content_->value(l); return l; }

    operator long long() const          { long long l;  content_->value(l); return l; }
    operator unsigned long long() const { long long l;  content_->value(l); return l; }

    operator double() const             { double d;     content_->value(d); return d; }
    operator bool() const               { bool d;       content_->value(d); return d; }

    operator string() const    { string s; content_->value(s); return s; }
    operator Date() const      { Date d; content_->value(d); return d; }
    operator Time() const      { Time t; content_->value(t); return t; }
    operator DateTime() const  { DateTime d; content_->value(d); return d; }

    operator ValueList() const;
    operator ValueMap() const;

    bool operator<(const Value& v) const  { return *content_ < *(v.content_); }
    bool operator==(const Value& v) const { return *content_ == *(v.content_); }

    bool operator>(const Value& v) const  { return v < *this;     }
    bool operator!=(const Value& v) const { return !(*this == v); }

    bool operator>=(const Value& v) const  { return !(*this < v);     }
    bool operator<=(const Value& v) const  { return !(v < *this);     }


    Value operator+(const Value&) const;
    Value& operator+=(const Value&);

    Value operator-() const;

    Value operator-(const Value&) const;
    Value& operator-=(const Value&);

    Value operator*(const Value&) const;
    Value& operator*=(const Value&);

    Value operator/(const Value&) const;
    Value& operator/=(const Value&);

    Value operator%(const Value&) const;
    Value& operator%=(const Value&);

    Value operator[](const char*) const;
    Value operator[](const string&) const;
    Value operator[](const Value&) const;
    Value operator[](int) const;

    // -- Methods

    int      compare(const Value& v) const { return content_->compare(*(v.content_)); }

    bool     isNil()     const { return content_->isNil(); }
    bool     isNumber()  const { return content_->isNumber(); }
    bool     isDouble()  const { return content_->isDouble(); }
    bool     isString()  const { return content_->isString(); }
    bool     isList()    const { return content_->isList(); }
    bool     isMap()     const { return content_->isMap(); }
    bool     isDate()    const { return content_->isDate(); }
    bool     isTime()    const { return content_->isTime(); }
    bool     isDateTime()const { return content_->isDateTime(); }

    Value	 tail() const;
    Value	 head() const;

// -- Class Methods

    static Value makeList();
    static Value makeList(const Value&);
    static Value makeList(const vector<Value>&);

    static Value makeMap();
    static Value makeMap(const map<Value,Value>&);

protected:

	Value(Content*);


private:

// -- Members
	
	Content* content_;

// -- Methods

    void json(JSON& s) const        { s << *content_; }
    void print(ostream& s) const    { s << *content_; }
    void encode(Stream& s) const    { s << *content_; }

    friend JSON& operator<<(JSON& s, const Value& v) { v.json(s);  return s; }
	friend ostream& operator<<(ostream& s, const Value& v) { v.print(s);  return s; }
	friend Stream&  operator<<(Stream&  s, const Value& v) { v.encode(s); return s; }

	friend class Content;

};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif
