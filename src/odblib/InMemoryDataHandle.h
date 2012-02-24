/// \file InMemoryDataHandle.h
/// Piotr Kuchta - ECMWF April 2009

#ifndef InMemoryDataHandle_H
#define InMemoryDataHandle_H

#include "PathName.h"
#include "Length.h"
#include "Offset.h"
#include "TransferWatcher.h"
#include "DataHandle.h"
#include "MemoryBlock.h"

namespace odb {

class InMemoryDataHandle : public DataHandle {
public:
    InMemoryDataHandle();
    InMemoryDataHandle(const MemoryBlock&);

    virtual ~InMemoryDataHandle();

	void buffer(const MemoryBlock&);


	bool hasSomeData() { return readIterator_ != buf_.end(); }

	/// Return estimated length.
    virtual Length openForRead()
	{
		readIterator_ = buf_.begin();
		return buf_.size();
	}

	// Receive estimated length.
    void openForWrite(const Length&) { buf_.clear(); ASSERT(buf_.size() == 0); }

	// Receive estimated length
    void openForAppend(const Length&) {}

    long read(void* p, long n)
	{
		char *dst = reinterpret_cast<char *>(p);
		long i = 0;
		for ( ; i < n && readIterator_ != buf_.end(); ++i, ++readIterator_)
			dst[i] = *readIterator_;
		return i;
	}

    long write(const void* pd, long n)
	{
		const unsigned char *p = reinterpret_cast<const unsigned char*>(pd);
		buf_.insert(buf_.end(), p, p + n);
		return n;
	}

    void close() {}

    void rewind()                {}
	Length estimate()            { return buf_.size(); }
	Offset position()            { return buf_.size(); }

	void print(ostream& s) const { /*TODO*/ }
private:
// No copy allowed
    InMemoryDataHandle(const InMemoryDataHandle&);
    InMemoryDataHandle& operator=(const InMemoryDataHandle&);

	vector<unsigned char> buf_;
	vector<unsigned char>::iterator readIterator_;

	friend ostream& operator<<(ostream& s, const InMemoryDataHandle& handle) 
		{ handle.print(s); return s;}
};

} // namespace odb 

#endif
