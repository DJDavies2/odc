/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef eckit_AutoLock_h
#define eckit_AutoLock_h

#include "eckit/thread/AutoLocker.h"
#include "eckit/memory/NonCopyable.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

// The class AutoLock is used to AutoLock a mutex in a multi-threaded
// environment. AutoLocks are exception safe.

template<class T> 
class AutoLock : public AutoLocker,
                 private NonCopyable {
public:

// -- Contructors
	
    AutoLock(T& resource) : resource_(resource) 
							{ want(&resource); resource_.lock(); got(&resource);}
    AutoLock(T* resource) : resource_(*resource)
							{ want(resource); resource_.lock(); got(resource);}

// -- Destructor

    ~AutoLock() { release(&resource_); resource_.unlock(); }

private: // members
	
    T& resource_;

};

//-----------------------------------------------------------------------------

template<class T> 
class AutoSharedLock : private NonCopyable {
public:

// -- Contructors
	
    AutoSharedLock(T& resource) : resource_(resource) 
							{ resource_.lockShared(); }
    AutoSharedLock(T* resource) : resource_(*resource)
							{ resource_.lockShared(); }

// -- Destructor

    ~AutoSharedLock() { resource_.unlock(); }

private: // members
	
    T& resource_;

};

//-----------------------------------------------------------------------------

} // namespace eckit

#endif
