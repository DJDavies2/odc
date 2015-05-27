/*
 * (C) Copyright 1996-2013 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "ListHandler.h"

#include "eckit/parser/Request.h"
#include "eckit/utils/ExecutionContext.h"

using namespace std;
using namespace eckit;

ListHandler::ListHandler(const string& name) : RequestHandler(name) {}

Values ListHandler::handle(const Request request)
{
    NOTIMP;
    return Values();
}

Values ListHandler::handle(const Request request, ExecutionContext& context)
{
    request->showGraph("ListHandler::handle: request", false);

    Values r (Cell::clone(request->valueOrDefault("values", new Cell("_list", "", 0, 0))));

    r->showGraph("value of values", false);

    context.stack().push(r);
    return r;
}

