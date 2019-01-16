/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @author Piotr Kuchta
/// @date July 2010
/// @author Simon Smart
/// @date January 2019

#ifndef odc_sql_FunctionEQ_BOXLON_H
#define odc_sql_FunctionEQ_BOXLON_H

#include "eckit/sql/expression/function/FunctionExpression.h"

namespace odc {
namespace sql {
namespace function {

//----------------------------------------------------------------------------------------------------------------------

class FunctionEQ_BOXLON : public eckit::sql::expression::function::FunctionExpression {
public:
    FunctionEQ_BOXLON(const std::string&, const eckit::sql::expression::Expressions&);
	FunctionEQ_BOXLON(const FunctionEQ_BOXLON&);
	~FunctionEQ_BOXLON(); 

	std::shared_ptr<SQLExpression> clone() const;

    static int arity() { return 3; }

private:
// No copy allowed
	FunctionEQ_BOXLON& operator=(const FunctionEQ_BOXLON&);

// -- Overridden methods
	virtual const eckit::sql::type::SQLType* type() const;
	virtual double eval(bool& missing) const;

// -- Friends
	//friend std::ostream& operator<<(std::ostream& s,const FunctionEQ_BOXLON& p)
	//	{ p.print(s); return s; }

};

//----------------------------------------------------------------------------------------------------------------------

} // namespace function
} // namespace sql
} // namespace odc

#endif
