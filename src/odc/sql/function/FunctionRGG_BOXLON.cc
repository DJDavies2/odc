/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "odc/sql/function/FunctionRGG_BOXLON.h"

#include "eckit/sql/expression/function/FunctionFactory.h"
#include "odc/sql/function/RggRegionCache.h"

using namespace eckit::sql::expression::function;
using namespace eckit::sql::expression;
using namespace eckit::sql;

namespace odc {
namespace sql {
namespace function {

//----------------------------------------------------------------------------------------------------------------------

/* Static self-registration */

static FunctionBuilder<FunctionRGG_BOXLON> rgg_boxlonFunctionBuilder("rgg_boxlon");

FunctionRGG_BOXLON::FunctionRGG_BOXLON(const std::string& name, const Expressions& args)
: FunctionExpression(name,args)
{}

FunctionRGG_BOXLON::FunctionRGG_BOXLON(const FunctionRGG_BOXLON& other)
: FunctionExpression(other.name_, other.args_)
{}

std::shared_ptr<SQLExpression> FunctionRGG_BOXLON::clone() const { return std::make_shared<FunctionRGG_BOXLON>(*this); }

FunctionRGG_BOXLON::~FunctionRGG_BOXLON() {}

const type::SQLType* FunctionRGG_BOXLON::type() const { return &type::SQLType::lookup("double"); }

double FunctionRGG_BOXLON::eval(bool& missing) const
{
    double lat_degrees = args_[0]->eval(missing);
    double lon_degrees = args_[1]->eval(missing);
    double resol = args_[2]->eval(missing);
    RggRegionCache p;
    double res;
    res = p.get_midlon(resol, lat_degrees, lon_degrees);

	return res;
}

//----------------------------------------------------------------------------------------------------------------------

} // namespace function
} // namespace sql
} // namespace odc
