/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <pthread.h>
#include <signal.h>

#include "eckit/compat/StrStream.h"
#include "eckit/config/Resource.h"
#include "eckit/filesystem/PathName.h"
#include "eckit/runtime/Context.h"
#include "eckit/runtime/Monitor.h"
#include "eckit/runtime/TaskInfo.h"
#include "eckit/thread/AutoLock.h"
#include "eckit/thread/AutoLock.h"

#include "eckit/os/BackTrace.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

static bool active_ = false;

static Monitor::TaskArray* mapArray = 0;
static pthread_once_t once = PTHREAD_ONCE_INIT;

static void taskarray_init(void)
{
    string  monitor = Resource<string>("monitorPath","~/etc/monitor");
	long    size    = Resource<long>("monitorSize",1000);
    mapArray = new Monitor::TaskArray(monitor,size);
}

Monitor::TaskArray& Monitor::tasks()
{
    pthread_once(&once, taskarray_init);
	return *mapArray;
}

//-----------------------------------------------------------------------------

template class ThreadSingleton<Monitor>;

Monitor& Monitor::instance()
{
    static ThreadSingleton<Monitor> monitor;
    Monitor& m = monitor.instance();
	if(!m.ready_) m.init();
	return m;
}

//-----------------------------------------------------------------------------

Monitor::Monitor():
	ready_(false),
	check_(false)
{ 
}

void Monitor::init()
{
	if(ready_) return;

    if(!active_) {  ready_ = true;  return; }  // this is for processes that don't use Monitor

	if(check_) return; // safely ignore double initialization
	check_ = true;

	TaskArray& a = tasks();

	AutoLock<TaskArray> lock(a);

	int slot = hash() % a.size();

	bool found = false;

	for(bool check = false; !check && !found ; check = true)
	{
		for( size_t i = 0 ; i < a.size() && !found ; ++i )
		{
			slot_ = (slot+i) % a.size();
			if(!a[slot_].busy(check))
			{
				new(&a[slot_]) TaskInfo();
				found = true;
			}
		}
	}

    if(!found) 
    {
        cout << "No free monitor slots" << endl;
        cerr << "No free monitor slots" << endl;
    }

	// No free monitor slots

	PANIC(!found);

	check_ = false;
	ready_ = true;
}

Monitor::~Monitor() 
{ 
	if(ready_ && active_) {
		TaskArray& a = tasks();
		AutoLock<TaskArray> lock(a);
		a[slot_].TaskInfo::~TaskInfo();
    }
}

//-----------------------------------------------------------------------------

bool Monitor::active()
{
    return active_;
}

void Monitor::active( bool a ) 
{
    active_ = a;
}

void Monitor::startup()
{
    if(!ready_) init();
}

void Monitor::shutdown()
{
	if(!active_) return;

	TaskArray& a = tasks();
	AutoLock<TaskArray> lock(a);

	pid_t pid = getpid();

	for( size_t i = 0; i < a.size() ; ++i )
		if(a[i].pid() == pid)
			a[i].TaskInfo::~TaskInfo();

	// should unmap
}

unsigned long Monitor::hash()
{
	return (((unsigned long)pthread_self() << 16) | (unsigned long)getpid());
}

TaskInfo* Monitor::task()
{
	if( !active_ || !ready_ )
	{
		static TaskInfo info;
		return &info;
	}

    TaskArray& a = tasks();
	return &a[slot_];
}


long Monitor::self()
{
	if(!active_) return 0;
	if(!ready_)  return -1;
	return slot_;	
}

void Monitor::out(char* from,char *to)
{
    if(!ready_) return;    
    task()->out(from,to);
}

void Monitor::name(const string& s) 
{
    if(!ready_) return;    
	task()->name(s);
}

void Monitor::kind(const string& s) 
{
    if(!ready_) return;    
	task()->kind(s);
}

void Monitor::progress(const string& name, unsigned long long min,unsigned long long max)
{
    if(!ready_) return;    
	task()->progressName(name);
	task()->start(min,max);
}

void Monitor::progress(unsigned long long value)
{
    if(!ready_) return;    
	task()->progress(value);
}

void Monitor::progress()
{
    if(!ready_) return;    
	task()->done();
}

char Monitor::state(char c)
{
	char x = task()->state();
	task()->state(c);
	return x;
}

void Monitor::status(const string& msg)
{
	if(!ready_) return;
	task()->status(msg);
}

string Monitor::status()
{
	if(!ready_) return string();
	return task()->status();
}

void Monitor::message(const string& msg)
{
	if(!ready_) return;
	task()->message(msg);
}

string Monitor::message()
{
	if(!ready_) return string();
	return task()->message();
}

void Monitor::stoppable(bool b)
{
	task()->stoppable(b);
}

bool Monitor::stopped()
{
	return task()->stopped();
}

void Monitor::abort()
{
	task()->abort();
}

void Monitor::checkAbort()
{
	task()->checkAbort();
}

void Monitor::parent(long p)
{
	task()->parent(p);
}


string Monitor::statusTree()
{
    StrStream os;
    os << status();
    int n = self();

	TaskArray& p = tasks();

	for( size_t j = 0 ; j < p.size(); ++j )
		if((p[j].parent() == n) && p[j].show() && p[j].busy(true))
		{
            os << "|" << p[j].status();
		}

    os << StrStream::ends;

    return string(os);
}

void Monitor::start(const string& app)
{
	if(!active_) return;

	TaskArray& p = tasks();

	for( size_t j = 0 ; j < p.size(); ++j )
		if(p[j].busy(true) && app == p[j].application() && 
			p[j].depth() == 0)
		{
			Log::warning() << app << " is already running with a pid of " 
				<< p[j].pid() << endl;
			Log::warning() << "Please stop it first" << endl;
			return;
		}

	PathName cmd = Resource<PathName>("startCmd","~/admin/starter");

	string s = string(cmd) + " " + app;

	Log::info() << "Executing shell command: " << s << endl;

	SYSCALL(::system(s.c_str()));

}

void Monitor::port(int p)
{
	task()->port(p);
}

int Monitor::port() 
{
	return task()->port();
}

void Monitor::host(const string& h)
{
	task()->host(h);
}

string Monitor::host() 
{
	return task()->host();
}

void Monitor::taskID(const TaskID& id)
{
	task()->taskID(id);
}

TaskID Monitor::taskID() 
{
	return task()->taskID();
}

void Monitor::show(bool on)
{
	task()->show(on);
}

int Monitor::kill(const string& name, int sig)
{

	if(!active_) return 0;

    Monitor::TaskArray& info = tasks();
	pid_t      me   = ::getpid();
	int        n    = 0;

	// Name. Look for Unix process ID
	for( size_t i = 0; i < info.size(); ++i )
		if(info[i].busy(true) && info[i].application() == name)
		{
			pid_t pid = info[i].pid();
			if(pid == me)
				Log::info() << pid << ": Suicide avoided ;-)" << endl;
			else
			{
				if(::kill(pid,sig))
					Log::info() << Log::syserr << endl; 
				else
				{
					Log::info() << pid << ": Killed" << endl;
					n++;
				}
			}
		}

	return n;
}

//-----------------------------------------------------------------------------

} // namespace eckit

