/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/types/Seconds.h"
#include "eckit/utils/Timer.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

Timer::Timer( bool output ):
    name_(),
    stopped_(true),
    outputAtExit_(output),
    out_( Log::info() )
{
    this->start();
}

Timer::Timer(const string& name, std::ostream& o ):
    name_(name),
    stopped_(true),
    outputAtExit_(true),
    out_(o)
{
    this->start();
}

Timer::~Timer()
{
    stop();

    double  s   = elapsed();
    double  cpu = elapsed_cpu();

    out_ << name_ << ": "
         << Seconds(s) << " elapsed, "
         << Seconds(cpu) << " cpu"
         << std::endl;

}


void Timer::start()
{
    if( !running() )
    {
        ::gettimeofday(&timeStart_,0);
        timeStop_ = timeStart_;

        cpuStart_ = ::clock();
        cpuStop_  = cpuStart_;

        stopped_ = false;
    }
}


void Timer::stop()
{
    if( !running() ) return;
    takeTime();
    stopped_ = true;
}


double Timer::elapsed()
{
    if( running() ) takeTime();

    timeval diff = timeStop_ - timeStart_;
    return (double)diff.tv_sec + ((double)diff.tv_usec / 1000000.);
}

double Timer::elapsed_cpu()
{
    if( running() ) takeTime();

    return double(cpuStop_ - cpuStart_) / CLOCKS_PER_SEC;
}

void Timer::takeTime()
{
    cpuStop_ = ::clock();
    ::gettimeofday(&timeStop_,0);
}

//-----------------------------------------------------------------------------

timeval operator-(const timeval& a,const timeval& b)
{
    timeval diff;

    diff.tv_sec  = a.tv_sec  - b.tv_sec;
    diff.tv_usec = a.tv_usec - b.tv_usec;

    if (diff.tv_usec < 0)
    {
          diff.tv_sec--;
          diff.tv_usec += 1000000;
    }

    return diff;
}

//-----------------------------------------------------------------------------

} // namespace eckit
