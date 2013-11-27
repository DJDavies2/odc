/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File Monitor.h
// Baudouin Raoult - ECMWF Nov 96

#ifndef eckit_Monitor_h
#define eckit_Monitor_h

#include "eckit/eckit.h"

#include "eckit/container/MappedArray.h"
#include "eckit/memory/NonCopyable.h"
#include "eckit/runtime/TaskInfo.h"
#include "eckit/thread/AutoLock.h"
#include "eckit/thread/ThreadSingleton.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

class Monitor : private NonCopyable {

public: // types

	typedef MappedArray<TaskInfo> TaskArray;
    
public: // methods
    
    static Monitor& instance();    

    static bool active();
    static void active( bool a );
    
	void startup();
	void shutdown();
	void out(char*,char*); // called from Log

    void name(const string&);
	void kind(const string&);

	void progress(const string&,unsigned long long,unsigned long long);
	void progress(unsigned long long);
	void progress();

	char state(char);

	void message(const string&);
	string message();

	void status(const string&);
	string status();
	string statusTree();
	void stoppable(bool);
	bool stopped();

	TaskArray& tasks();
	long       self();
	void parent(long);

	void start(const string&);
	int  kill(const string& , int = 15);

	void port(int );
	int  port();

	void    host(const string& );
	string  host();

	TaskID taskID();
	void taskID(const TaskID&);

	void show(bool);

	void cancel(const string&);
	string cancelMessage();
	bool  canceled();

	void abort();
	void checkAbort();

private: // members

	unsigned long slot_;
	bool ready_;
	bool check_;

private: // methods

    /// Contructors

	Monitor();

    /// Destructor

	~Monitor();

	unsigned long hash();
    
    TaskInfo* task();

	void init();

    friend class ThreadSingleton<Monitor>;
    friend class NewAlloc0<Monitor>;
    
};

//-----------------------------------------------------------------------------

// Wrap the setting of Monitor::instance().state

class AutoState {
	char old_;
public:
	AutoState(char c): 
		old_(Monitor::instance().state(c)) {}

	~AutoState()
		{ Monitor::instance().state(old_); }

	void set(char c) 
		{ Monitor::instance().state(c); }
};

//-----------------------------------------------------------------------------

template<class T>
class AutoLockTag {
	AutoState state_;
	AutoLock<T> lock_;
public:
    AutoLockTag(T& t): state_(t.tag()), lock_(t) 
		{ state_.set(t.tag() - 'a' + 'A'); }
    AutoLockTag(T* t): state_(t->tag()), lock_(t) 
		{ state_.set(t->tag() - 'a' + 'A'); }
};

//-----------------------------------------------------------------------------

} // namespace eckit

#endif
