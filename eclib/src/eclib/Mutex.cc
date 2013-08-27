/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eclib/Exceptions.h"
#include "eclib/Mutex.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

Mutex::Mutex(char tag) :
	exists_(false),
	tag_(tag)
{

//#if defined(__GNUC__) && __GNUC__ < 3
//#ifndef PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP
//#define PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP { PTHREAD_MUTEX_RECURSIVE_NP }
//#endif
//	pthread_mutexattr_t attr = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
//#else
	pthread_mutexattr_t attr;
	THRCALL(::pthread_mutexattr_init(&attr));
	THRCALL(::pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE));
//#endif

	THRCALL(pthread_mutex_init(&mutex_,&attr));

	exists_ = true;
	THRCALL(::pthread_mutexattr_destroy(&attr));
}

Mutex::~Mutex() 
{
	THRCALL(pthread_mutex_destroy(&mutex_));
}

void Mutex::lock(void) 
{

	if(!this || !exists_)
	{
		cerr << "Mutex used before being contructed" << endl;
		abort();
		return;
	}

	THRCALL(pthread_mutex_lock(&mutex_)); 
}

void Mutex::unlock(void) 
{
	if(!exists_)
	{
		cerr << "Mutex used before being contructed" << endl;
		abort();
		return;
	}
	THRCALL(pthread_mutex_unlock(&mutex_));
}

//-----------------------------------------------------------------------------

} // namespace eclib

