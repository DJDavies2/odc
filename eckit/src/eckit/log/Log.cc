/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/log/Log.h"
#include "eckit/log/Channel.h"
#include "eckit/log/MultiChannel.h"
#include "eckit/log/MonitorChannel.h"
#include "eckit/log/UserChannel.h"

#include "eckit/runtime/Context.h"

#include "eckit/thread/ThreadSingleton.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

struct CreateMonitorChannel
{
    MonitorChannel* operator()() { return new MonitorChannel( MonitorChannel::NONE ); }
};

Channel& Log::monitor(char type,long mode)
{
    static ThreadSingleton<MonitorChannel,CreateMonitorChannel> x;
    MonitorChannel& y = x.instance();
    y.flags(type,mode);
    return y;
}

//-----------------------------------------------------------------------------

struct CreateMonitorStatusChannel
{
    MonitorChannel* operator()() { return new MonitorChannel( MonitorChannel::STATUS ); }
};

Channel& Log::status()
{
    static ThreadSingleton<MonitorChannel,CreateMonitorStatusChannel> x;
    return x.instance();
}

//-----------------------------------------------------------------------------

struct CreateMonitorMessageChannel
{
    MonitorChannel* operator()() { return new MonitorChannel( MonitorChannel::MESSAGE ); }
};

Channel& Log::message()
{
    static ThreadSingleton<MonitorChannel,CreateMonitorMessageChannel> x;
    return x.instance();
}

//-----------------------------------------------------------------------------

Channel& Log::info()
{
    return Context::instance().infoChannel();
}

Channel& Log::info(const CodeLocation& where)
{
    return Context::instance().infoChannel().source(where);
}

//-----------------------------------------------------------------------------

Channel& Log::error()
{
    return Context::instance().errorChannel();
}

Channel& Log::error(const CodeLocation& where)
{
    return Context::instance().infoChannel().source(where);
}

//-----------------------------------------------------------------------------

std::ostream& Log::panic()
{
    try
    {
        return Log::error();
    }
    catch(exception&)
    {
        return  std::cerr;
    }
}

std::ostream& Log::panic(const CodeLocation& where)
{
    try 
    {
        return Log::error(where);
    }
    catch(exception&)
    {
        std::cerr << "[" << where << "]";
        return  std::cerr;
    }
}

//-----------------------------------------------------------------------------

Channel& Log::warning()
{
    return Context::instance().warnChannel();
}

Channel& Log::warning(const CodeLocation& where)
{
    return Context::instance().warnChannel().source(where);
}

//-----------------------------------------------------------------------------

Channel& Log::debug(int level)
{
    static MultiChannel no_output;
    if(level >= Context::instance().debug())
        return no_output;
    else
        return Context::instance().debugChannel();
}

Channel& Log::debug(const CodeLocation& where, int level)
{
    static MultiChannel no_output;
    if(level >= Context::instance().debug())
        return no_output;
    else
        return Context::instance().debugChannel().source(where);
}

//-----------------------------------------------------------------------------

UserChannel& Log::user()
{
    static ThreadSingleton<UserChannel> x;
    return x.instance();
}

Channel& Log::userInfo()
{
    UserChannel& u = user();
    u.msgType(UserChannel::INFO);
    return u;
}

Channel& Log::userError()
{
    UserChannel& u = user();
    u.msgType(UserChannel::ERROR);
    return u;
}

Channel& Log::userWarning()
{
    UserChannel& u = user();
    u.msgType(UserChannel::WARN);
    return u;
}

void Log::notifyClient(const string& msg)
{
    UserChannel& u = user();
    UserMsg* um = u.userMsg();
    if(um) um->notifyClient(msg);
}

//-----------------------------------------------------------------------------

#ifndef _GNU_SOURCE

static void handle_strerror_r(ostream& s, int e, char es[], int hs )
{
    if( hs == 0 )
        s << " (" << es << ") " ;
    else
        s << " (errno = " << e << ") ";
}

#else // GNU implementation is not XSI compliant and returns char* instead of int

static void handle_strerror_r(ostream& s, int e, char[], char* p )
{
    if( p != 0 )
        s << " (" << p << ") " ;
    else
        s << " (errno = " << e << ") ";
}

#endif

//-----------------------------------------------------------------------------

ostream& Log::syserr(ostream& s)
{
        int e = errno;
        char estr [256];
        handle_strerror_r(s, e, estr, strerror_r( e, estr, sizeof(estr) ) );
        return s;
}

//-----------------------------------------------------------------------------

static int xindex = ios::xalloc();

int format(ostream& s)
{
        return s.iword(xindex);
}

ostream& setformat(ostream& s,int n)
{
        s.iword(xindex) = n;
        return s;
}

//-----------------------------------------------------------------------------

template class ThreadSingleton<UserChannel>;

//-----------------------------------------------------------------------------

} // namespace eckit
