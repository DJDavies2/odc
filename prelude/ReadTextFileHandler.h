/*
 * (C) Copyright 1996-2016 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */
/// @author Piotr Kuchta, December 2015

#ifndef eckit_ecml_ReadTextFileHandler_H
#define eckit_ecml_ReadTextFileHandler_H

#include <sstream>

#include "eckit/ecml/parser/Request.h"
#include "eckit/ecml/core/RequestHandler.h"

namespace eckit {

class ReadTextFileHandler : public eckit::RequestHandler {
public:
    ReadTextFileHandler(const std::string&);
    virtual eckit::Values handle(eckit::ExecutionContext&);
private:
    std::string readFile(const eckit::PathName&);
};

} // namespace eckit

#endif
