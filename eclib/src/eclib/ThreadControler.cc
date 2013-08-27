/*
 * (C) Copyright 1996-2012 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */


#include <signal.h>

#include "eclib/Application.h"
#include "eclib/AutoLock.h"
#include "eclib/Log.h"
#include "eclib/MemoryPool.h"
#include "eclib/Monitor.h"
#include "eclib/Thread.h"
#include "eclib/ThreadControler.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------


ThreadControler::ThreadControler(Thread* proc,bool detached,size_t stack):
    thread_(0),
    proc_(proc),
    running_(false),
    stack_(stack),
    detached_(detached)
{
    //cout << "ThreadControler::ThreadControler(" << this << ")" << " " << hex << pthread_self() <<endl;
}

ThreadControler::~ThreadControler()
{
    //cout << "ThreadControler::~ThreadControler(" << this << ")" << " " <<hex <<  pthread_self() <<endl;
    AutoLock<MutexCond> lock(cond_);

    if(running_)
    {
        // The Thread will delete itself
        // so there is no need for:
        // delete proc_;
    }
    else
    {
        //Log::warning() << "Deleting Thread in ThreadControler::~ThreadControler()" << " " << hex << pthread_self() <<endl;
        delete proc_;
        proc_ = 0;
    }
}

//------------------------------------------------------

void ThreadControler::execute()
{
    // Make a copy, because "this" will deasapear
    Thread* proc = proc_;
    proc_ = 0;

    //cout << "ThreadControler::execute(" << this << ")" <<  " " << hex << pthread_self() << endl;
    //=================
    // Make sure the logs are created...

    Log::init();
    Monitor::startup();
    Monitor::parent(Context::instance().self());

    //============


    { // Signal that we are running

        AutoLock<MutexCond> lock(cond_);
        running_ = true;
        cond_.signal();

        // NOTE: "this" is not valid any more after this point

    }

    //=============

    // We don't want to recieve reconfigure events

    sigset_t set,old_set;

    sigemptyset(&set);

    sigaddset(&set, SIGHUP);
    sigaddset(&set, SIGCHLD);
    sigaddset(&set, SIGPIPE);

    THRCALL(pthread_sigmask(SIG_BLOCK, &set, &old_set));

    //=============

    ASSERT(proc);

    try {
        proc->run();
    }
    catch(exception& e){
        Log::error() << "** " << e.what() << " Caught in "
                     << Here() <<  endl;
        Log::error() << "** Exception is terminates thread "
                     << pthread_self() << endl;
    }

    if(proc->autodel_)
    {
        delete proc;
    }
}

void* ThreadControler::startThread (void* data)
{
    //cout << "ThreadControler::startThread(" << data << ")" << " " << hex << pthread_self() <<endl;
    reinterpret_cast<ThreadControler*>(data)->execute(); // static_cast or dynamic_cast ??
    return 0;
}

void ThreadControler::start()
{
    //cout << "ThreadControler::start(" << this << ")" << " " << hex << pthread_self() <<endl;
    ASSERT(thread_ == 0);

    pthread_attr_t attr;
    pthread_attr_init(&attr);

    if(stack_)
    {
        THRCALL(pthread_attr_setstacksize(&attr, stack_));
    }
    if(detached_)
        THRCALL(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED));
    else
        THRCALL(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE));

    AutoLock<MutexCond> lock(cond_);

    THRCALL(pthread_create(&thread_,&attr,startThread,this));

    pthread_attr_destroy(&attr);

    while(!running_)
        cond_.wait();
}

void ThreadControler::kill()
{
    pthread_cancel(thread_);
    //pthread_kill(thread_,sig);
}

void ThreadControler::stop()
{
    proc_->stop();
}

void ThreadControler::wait()
{
    ASSERT(!detached_);
    // if(running_)
    THRCALL(pthread_join(thread_,0));
}

bool ThreadControler::active()
{
    if(thread_ != 0)
    {
        // Try see if it exists

        int policy;
        sched_param param;

        int n = pthread_getschedparam(thread_, &policy, &param);

        // The thread does not exist
        if(n != 0)
            thread_ = 0;

    }
    return thread_ != 0;
}

//-----------------------------------------------------------------------------

} // namespace eclib

