#include "FunctionEQ_BOXLON.h"
#include "EqRegionCache.h"

namespace odb {
namespace sql {
namespace expression {
namespace function {

FunctionEQ_BOXLON::FunctionEQ_BOXLON(const string& name,const expression::Expressions& args)
: FunctionExpression(name,args)
{}

FunctionEQ_BOXLON::FunctionEQ_BOXLON(const FunctionEQ_BOXLON& other)
: FunctionExpression(other.name_, other.args_)
{}

SQLExpression* FunctionEQ_BOXLON::clone() const { return new FunctionEQ_BOXLON(*this); }

FunctionEQ_BOXLON::~FunctionEQ_BOXLON() {}

double FunctionEQ_BOXLON::eval(bool& missing) const
{
    double lat_degrees = args_[0]->eval(missing);
    double lon_degrees = args_[1]->eval(missing);
    double resol = args_[2]->eval(missing);
    EqRegionCache p;
    double res;
    res = p.get_midlon(resol, lat_degrees, lon_degrees);

	return res;
}

static FunctionMaker<FunctionEQ_BOXLON> make_EQ_BOXLON("eq_boxlon",3);

} // namespace function
} // namespace expression
} // namespace sql
} // namespace odb

