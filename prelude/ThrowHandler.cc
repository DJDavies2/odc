/*
 * (C) Copyright 1996-2016 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "ThrowHandler.h"

#include "eckit/ecml/parser/Request.h"
#include "eckit/ecml/core/ExecutionContext.h"
#include "eckit/ecml/core/Environment.h"

#include <vector>
#include "eckit/parser/StringTools.h"

using namespace std;

namespace eckit {

ThrowHandler::ThrowHandler(const std::string& name) : RequestHandler(name) {}

Values ThrowHandler::handle(ExecutionContext& context)
{
    string message;

    if (context.environment().lookupNoThrow("what"))
        message = StringTools::join("/", context.getValueAsList("what"));
    else
    {
        const string currentException (context.environment().lookup("current_exception", "", context));
        if (currentException.size())
            message = currentException;
    }

    throw Exception(message);
    return 0;
}

} // namespace eckit
