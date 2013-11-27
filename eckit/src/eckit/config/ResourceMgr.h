/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File ResourceMgr.h
// Baudouin Raoult - ECMWF May 96

#ifndef eckit_ResourceMgr_h
#define eckit_ResourceMgr_h

#include <set>

#include "eckit/memory/NonCopyable.h"
#include "eckit/filesystem/PathName.h"
#include "eckit/types/Types.h"

//-----------------------------------------------------------------------------

namespace eckit {

//-----------------------------------------------------------------------------

class LocalPathName;
class Configurable;

namespace config { class Script; }

/// ResourceMgr is a singleton class
class ResourceMgr : private NonCopyable {

public: // methods

    /// destructor for singleton
	~ResourceMgr();

    /// @returns the singleton instance of this class
    static ResourceMgr& instance();
    
	void reset();
    bool lookUp( Configurable*, const string&, const StringDict* args, string&);
	void set(const string&,const string&);

    void appendConfig( istream& in );
    void appendConfig( const PathName& );
    
protected: // methods
    
    void readConfigFiles();
    
private: // methods

    /// private contructor for singleton
	ResourceMgr();

private: // members

	bool inited_;
    
    config::Script* script_;

    std::set<PathName> parsed_;
};


//-----------------------------------------------------------------------------

} // namespace eckit

#endif
