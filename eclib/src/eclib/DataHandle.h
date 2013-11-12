/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File DataHandle.h
// Baudouin Raoult - ECMWF May 96

#ifndef eclib_DataHandle_h
#define eclib_DataHandle_h

#include "eclib/Length.h"
#include "eclib/Offset.h"
#include "eclib/PathName.h"
#include "eclib/Streamable.h"
#include "eclib/TransferWatcher.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class RestartTransfer  {
	Offset from_;
public:
	RestartTransfer(const Offset& from): from_(from) {}
	const Offset& from() const { return from_; }
};

//-----------------------------------------------------------------------------

class DataHandle : public Streamable {
public:

	friend ostream& operator<<(ostream& s,const DataHandle& handle) 
		{ handle.print(s); return s;}

// -- Contructors

    DataHandle();
    DataHandle(Stream&);

// -- Destructor

    virtual ~DataHandle() {}

// -- Methods

	virtual void print(ostream& s) const = 0;

    virtual Length openForRead()              = 0; // Return estimated length
    virtual void openForWrite(const Length&)  = 0; // Receive estimated length
    virtual void openForAppend(const Length&) = 0; // Receive estimated length

    virtual long read(void*,long)        = 0;
    virtual long write(const void*,long) = 0;
    virtual void close()                 = 0;
    virtual void flush();

	virtual Length estimate()            { return 0; }
	virtual Offset position();
	virtual Offset seek(const Offset&);
	virtual void skip(size_t);

	virtual void rewind();
	virtual void restartReadFrom(const Offset&);
	virtual void restartWriteFrom(const Offset&);

	// Save into an other datahandle

	virtual Length saveInto(DataHandle&, TransferWatcher& = TransferWatcher::dummy());   

	Length saveInto(const PathName&, TransferWatcher& = TransferWatcher::dummy()); // Save into a file

    /// Save into an other datahandle
	Length appendTo(DataHandle&);   
	
    /// Save into a file
    Length appendTo(const PathName&); 

	virtual string name() const;


    /// Compare bytes
	bool compare(DataHandle&); 

	// Merge-in an other datahandle

	virtual bool compress(bool = false) { return false; }
	virtual bool merge(DataHandle*)     { return false; }
	virtual bool isEmpty() const        { return false; }


    // For remote data movers
    
    virtual bool moveable() const { return false; }
    virtual void toLocal(Stream& s) const;

    virtual DataHandle* toLocal();

    virtual void toRemote(Stream& s) const;
    virtual void cost(map<string,Length>&, bool) const;
    virtual string title() const;

// -- Overridden methods

    // From Streamble

    virtual void encode(Stream&) const;
	virtual const ReanimatorBase& reanimator() const { return reanimator_; }

// -- Class methods
    
    static  const ClassSpec&  classSpec()        { return classSpec_;}

private:

// -- Class members

    static  ClassSpec               classSpec_;
	static Reanimator<DataHandle>  reanimator_;

};

//-----------------------------------------------------------------------------

class AutoClose {
	DataHandle& handle_;
public:
	AutoClose(DataHandle& handle) : handle_(handle) {}
	~AutoClose();

};

template<>
Streamable* Reanimator<DataHandle>::ressucitate(Stream& s) const
#ifdef IBM
{ return 0;}
#else
;
#endif


//-----------------------------------------------------------------------------

} // namespace eclib

#endif
