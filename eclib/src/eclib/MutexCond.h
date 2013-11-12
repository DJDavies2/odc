/*
 * (C) Copyright 1996-2012 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File MutexCond.h
// Baudouin Raoult - ECMWF Jun 96

#ifndef eclib_MutexCond_h
#define eclib_MutexCond_h

#include <pthread.h>

#include "eclib/NonCopyable.h"

//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

// A mutex and a condition variable
// for Producer/Consumer architectures

class MutexCond : private NonCopyable {
public:

// -- Contructors

	MutexCond(char tag = ' ');

// -- Destructor

	~MutexCond();

// -- Methods

	void lock();
	void unlock();
	void wait();
	void signal();
	void broadcast();
	bool wait(int);
	char tag() const { return tag_; }

private:

// -- Members

	pthread_mutex_t mutex_;
	pthread_cond_t  cond_;
	char tag_;
	bool inited_;

};


//-----------------------------------------------------------------------------

} // namespace eclib

#endif
