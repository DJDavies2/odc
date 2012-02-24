#include "FunctionEQ.h"
#include "ColumnExpression.h"
#include "SQLType.h"
#include "Log.h"
#include "StringTool.h"

namespace odb {
namespace sql {
namespace expression {
namespace function {

using namespace odb::sql::type;

FunctionEQ::FunctionEQ(const FunctionEQ& other)
: FunctionExpression(other.name_, other.args_),
  tmp_(other.tmp_)
{}

FunctionEQ::FunctionEQ(const string& name, const expression::Expressions& args)
: FunctionExpression(name, args),
  tmp_(0)
{}

SQLExpression* FunctionEQ::clone() const { return new FunctionEQ(*this); }

FunctionEQ::~FunctionEQ() {}

void FunctionEQ::trimStringInDouble(char* &p, size_t& len)
{
	len = 0;
	for(; len < sizeof(double) && isprint(p[len]); ++len)
		;
	for(; len > 0 && isspace(p[len - 1]); --len)
		;
	size_t plen = len;
	for (char *pp = p; isspace(*p) && p < pp + plen;)
	{
		++p;
		--len;
	}
}

bool FunctionEQ::equal(const SQLExpression& l, const SQLExpression& r, bool& missing)
{
	if (l.type()->getKind() == SQLType::stringType)
	{
		double v1 = l.eval(missing);
		double v2 = r.eval(missing);
		if (missing)
			return false;

		char *p1 = reinterpret_cast<char*>(&v1);
		char *p2 = reinterpret_cast<char*>(&v2);
		
		size_t len1 = sizeof(double);
		size_t len2 = sizeof(double);

		trimStringInDouble(p1, len1);
		trimStringInDouble(p2, len2);

		if (len1 != len2)
			return false;

		return 0 == strncmp(p1, p2, len1);
	}

	return l.eval(missing) == r.eval(missing);
}

double FunctionEQ::eval(bool& missing) const
{
	return equal(*args_[0], *args_[1], missing);
}

SQLExpression* FunctionEQ::simplify(bool& changed) 
{
	SQLExpression* x = FunctionExpression::simplify(changed);
	if(x) return x;

	ColumnExpression* a = dynamic_cast<ColumnExpression*>(args_[0]);
	ColumnExpression* b = dynamic_cast<ColumnExpression*>(args_[1]);

	if(a && b) {
		args_[0] = 0;
		args_[1] = 0;
		return FunctionFactory::build("join",a,b);
	}

	//
	if(args_[0]->isConstant() && !args_[1]->isConstant())
		swap(args_[0],args_[1]);

	return 0;
}

bool FunctionEQ::useIndex()
{
	return args_[0]->indexed() && args_[1]->isConstant();
}

SQLIndex* FunctionEQ::getIndex(double*)
{
	bool missing = false;
	tmp_ = args_[1]->eval(missing);
	return args_[0]->getIndex(&tmp_);
}

static FunctionMaker<FunctionEQ> make_EQ("=",2);

} // namespace function
} // namespace expression
} // namespace sql
} // namespace odb

