/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @file UserChannel.h
/// @author Tiago Quintino

#ifndef eckit_log_UserChannel_h
#define eckit_log_UserChannel_h

#include "eckit/log/Channel.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

class UserMsg {
public:
	virtual void infoMsg(const string&)      = 0;
	virtual void warningMsg(const string&)   = 0;
	virtual void errorMsg(const string&)     = 0;
	virtual void notifyClient(const string&) = 0;
};

//-----------------------------------------------------------------------------

class UserBuffer;

class UserChannel : public Channel {
public: // types
    
    enum MsgType { NONE, INFO, ERROR, WARN };
    
public: // methods
    
    /// Constructor
    UserChannel();
    
    /// Destructor
    ~UserChannel();
    
    /// type for next message
    void msgType( MsgType t );
    
    void userMsg( UserMsg* );
    UserMsg* userMsg() const;

protected:

    UserBuffer* buffer_;    
};

//-----------------------------------------------------------------------------

} // namespace eckit

#endif
