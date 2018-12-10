/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef odblib_ColumnType_H
#define odblib_ColumnType_H

namespace odc {

enum ColumnType
{
	IGNORE   = 0,
	INTEGER  = 1,
	REAL     = 2,
	STRING   = 3,
	BITFIELD = 4,
	DOUBLE   = 5
};

} // namespace odc 

#endif // odblib_ColumnType_H
