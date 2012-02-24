#include "SQLExpression.h"
#include "NumberExpression.h"
#include "StrStream.h"
#include "SQLType.h"
#include "SQLOutput.h"

namespace odb {
namespace sql {
namespace expression {

SQLExpression::SQLExpression() : isBitfield_(false), hasMissingValue_(false) {}

SQLExpression::~SQLExpression() {}

bool SQLExpression::isVector() const { return false; }

Vector& SQLExpression::vector()
{
	NOTIMP;
	return *((Vector*) 0);
}

Dictionary& SQLExpression::dictionary()
{
	NOTIMP;
	return *((Dictionary*) 0);
}

SQLExpression* SQLExpression::number(double value) { return new NumberExpression(value); } 

SQLExpression* SQLExpression::simplify(bool& changed)
{
	if(isConstant() && !isNumber())
	{
		changed = true;
		bool missing = false;
		Log::info() << "SIMPLIFY " << *this << " to " << eval(missing) << endl;
		return new NumberExpression(eval(missing));
	}
	return 0;
}

void SQLExpression::output(SQLOutput& s) const 
{ 
	bool missing = false;
	double d = eval(missing); 
	s.outputReal(d, missing);
}

void SQLExpression::title(const string& t)
{
	title_ = t;
}

string SQLExpression::title() const
{
	if(title_.size())
		return title_;

	StrStream s;
	s << *this << StrStream::ends;
	return string(s);
}


const type::SQLType* SQLExpression::type() const
{
	const type::SQLType& x = type::SQLType::lookup("real");
	return &x;
}

void SQLExpression::expandStars(const std::vector<SQLTable*>&, expression::Expressions& e)
{
	e.push_back(this);
}

} // namespace expression
} // namespace sql
} // namespace odb

