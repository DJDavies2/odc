#include "LimitValues.h"

#include "oda.h"

#include "SQLIteratorSession.h"
#include "SQLSelect.h"
#include "SQLType.h"
#include "SQLIteratorOutput.h"

namespace odb {
namespace sql {

template <typename T>
SQLIteratorOutput<T>::SQLIteratorOutput(T& it):
        iterator_(it),
        count_(0)
{}

template <typename T>
SQLIteratorOutput<T>::~SQLIteratorOutput() {}

template <typename T>
void SQLIteratorOutput<T>::print(ostream& s) const
{
        s << "SQLIteratorOutput";
}

template <typename T>
void SQLIteratorOutput<T>::size(int) {}

template <typename T>
void SQLIteratorOutput<T>::reset() { count_ = 0; }

template <typename T>
void SQLIteratorOutput<T>::flush() {}

template <typename T>
bool SQLIteratorOutput<T>::output(const expression::Expressions& results)
{
        size_t nCols = results.size();
        ///ASSERT(nCols == iterator_.columns().size());
        bool missing = false;

        if (iterator_.isCachingRows())
                iterator_.cacheRow(results);
        else
                for(size_t i = 0; i < nCols; i++)
                        iterator_.data_[i] = results[i]->eval(missing /*=false*/);
        count_++;
        return true;
}

template <typename T>
void SQLIteratorOutput<T>::prepare(SQLSelect& sql) {}

template <typename T>
void SQLIteratorOutput<T>::cleanup(SQLSelect& sql) {}

template <typename T>
unsigned long long SQLIteratorOutput<T>::count() { return count_; }

} // namespace sql
} // namespace odb
