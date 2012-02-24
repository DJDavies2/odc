/// \file FunctionTHIN.h
/// (C) ECMWF July 2010

#include <utility>
#include <string>
#include <vector>

#include "SQLSession.h"
#include "SQLSelect.h"
#include "FunctionTHIN.h"
#include "Log.h"

namespace odb {
namespace sql {
namespace expression {
namespace function {

FunctionTHIN::FunctionTHIN(const string& name, const expression::Expressions& args)
: FunctionExpression(name, args),
  count_(0)
{}

FunctionTHIN::FunctionTHIN(const FunctionTHIN& other)
: FunctionExpression(other.name_, other.args_),
  count_(other.count_)
{}

FunctionTHIN::~FunctionTHIN() {}

SQLExpression* FunctionTHIN::clone() const { return new FunctionTHIN(*this); }

const odb::sql::type::SQLType* FunctionTHIN::type() const
{
    return &odb::sql::type::SQLType::lookup("integer");
}

void FunctionTHIN::output(ostream& s) const
{
    bool missing;
    s << static_cast<unsigned long>(eval(missing));
}

void FunctionTHIN::print(ostream& s) const
{
    s << "THIN()";
}

double FunctionTHIN::eval(bool& missing) const
{
    int every_nth = (int) args_[0]->eval(missing);
    if ((*count_ - 1) % every_nth == 0)
      return 1.0;
    else
      return 0.0;
}

void FunctionTHIN::prepare(SQLSelect& sql)
{
    FunctionExpression::prepare(sql);
     count_ = &sql.count_;
}

void FunctionTHIN::cleanup(SQLSelect& sql)
{
}

bool FunctionTHIN::isConstant() const
{
    return false;
}

SQLExpression* FunctionTHIN::simplify(bool&)
{
    return 0;
}

static FunctionMaker<FunctionTHIN> make_THIN("thin", 2);

} // namespace function
} // namespace expression
} // namespace sql
} // namespace odb
