#include "SQLIndex.h"
#include "SQLColumn.h"
#include "SQLTable.h"
#include "Timer.h"

namespace odb {
namespace sql {

struct _data {
	unsigned long offset_;
	unsigned long pool_;
};


SQLIndex::SQLIndex(SQLColumn& owner):
	owner_(owner),
	value_(0)
{} 

SQLIndex::~SQLIndex() {}

void SQLIndex::update()
{
#if 0
	PathName path  = owner_.indexPath();


	Log::info() << "Loading index " << path << endl;

	owner_.rewind();
    bool create = !path.exists();


	if(create)
	{
		BerkeleyDB db(path,true);

		_data d;
		Timer timer("Create index");

		unsigned long long rows = owner_.noRows();

		for(unsigned long long i = 0; i < rows; i++)
		{
			bool missing = false;
			d.offset_    = owner_.offset();
			d.pool_      = owner_.currentPool();
			double value = owner_.next(missing);

			db.add(&value,sizeof(value),&d,sizeof(d));

		}
	}

	auto_ptr<BerkeleyDB> db(new BerkeleyDB(path,false));
	db_ = db;
#endif
}

void SQLIndex::rewind(double* value)
{
#if 0
	value_ = value;
	db_->rewind();
//	cout << "SQLIndex::rewind " << *value << endl;
#endif
}

bool SQLIndex::next(unsigned long& pool,unsigned long& offset)
{
#if 0
	_data d;
	size_t size = sizeof(d);

//	cout << "SQLIndex::next " << value_ << endl;

//	for(;;)
	{
		if(db_->get(value_,sizeof(*value_),&d,size))
		{
//			if(pool_ < 0 || (pool_ == d.pool_ && d.offset_ >= offset1_ && d.offset_ <= offset2_))
			{
//				cout << "SQLIndex::next " << d.pool_ << " " << d.offset_ << endl;
				pool   = d.pool_;
				offset = d.offset_;
				return true;
			}
		}
		else
			return false;
	}

#endif
return false;
}

#if 0
void SQLIndex::restrict(unsigned long pool,unsigned long offset,unsigned long length)
{
	pool_    = pool;
	offset1_ = offset;
	offset2_ = offset + length - 1;
	cout << "Restrict " << pool_ << " " << offset1_ << " " << offset2_ << endl;
}
#endif

} // namespace sql
} // namespace odb
