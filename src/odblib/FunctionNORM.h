/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File FunctionNORM.h
// ECMWF July 2010

#ifndef FunctionNORM_H
#define FunctionNORM_H

#include "odblib/FunctionExpression.h"

namespace odb {
namespace sql {
namespace expression {
namespace function {

class FunctionNORM : public FunctionExpression {
public:
	FunctionNORM(const string&,const expression::Expressions&);
	FunctionNORM(const FunctionNORM&);
	~FunctionNORM();

	SQLExpression* clone() const;
private:
// No copy allowed
	FunctionNORM& operator=(const FunctionNORM&);

	double value_;

// -- Overridden methods
	virtual void prepare(SQLSelect&);
	virtual void cleanup(SQLSelect&);
	virtual void partialResult();
	virtual double eval(bool& missing) const;

	bool isAggregate() const { return true; }
	bool resultNULL_;
// -- Friends
	//friend ostream& operator<<(ostream& s,const FunctionNORM& p)
	//	{ p.print(s); return s; }
};

} // namespace function
} // namespace expression 
} // namespace sql
} // namespace odb

#endif
