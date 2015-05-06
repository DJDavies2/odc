/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/eckit.h"
#include "eckit/exception/Exceptions.h"
#include "odb_api/SQLCallbackSession.h"
#include "odb_api/SQLRowCallback.h"
#include "odb_api/SQLStatement.h"
#include "odb_api/SQLRowCallback.h"
#include "odb_api/SQLCallbackOutput.h"

namespace odb {
namespace sql {

SQLCallbackSession::SQLCallbackSession(SQLRowCallback& callback)
: sql_(0), 
  callback_(callback)
{}

SQLCallbackSession::~SQLCallbackSession()
{
    delete sql_;
}

SQLOutput* SQLCallbackSession::defaultOutput()
{
	return new SQLCallbackOutput(callback_);
}

void SQLCallbackSession::statement(SQLStatement *sql)
{
	ASSERT(sql);	
    sql_ = sql;
}

SQLStatement& SQLCallbackSession::statement()
{
    ASSERT(sql_);
    return *sql_;
}

} // namespace sql
} // namespace odb
