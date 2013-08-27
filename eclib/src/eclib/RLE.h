/*
 * (C) Copyright 1996-2012 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File RLE.h
// Baudouin Raoult - ECMWF Jun 96

#ifndef eclib_RLE_h
#define eclib_RLE_h


//-----------------------------------------------------------------------------

namespace eclib {

//-----------------------------------------------------------------------------

class Stream;

template <class InputIterator, class OutputIterator>
long long RLEencode2(InputIterator first, InputIterator last,
	OutputIterator result,long long maxloop);

template <class InputIterator, class OutputIterator>
void RLEdecode2(InputIterator first, InputIterator last,
	OutputIterator result);

template <class InputIterator>
void RLEprint(ostream&,InputIterator first, InputIterator last);


template<class InputIterator,class OutputIterator>
bool DIFFencode(InputIterator first,InputIterator last,OutputIterator result);

template<class InputIterator,class OutputIterator>
void DIFFdecode(InputIterator first,InputIterator last,OutputIterator result);




//==========================================================================

template<class InputIterator>
Stream& RLEwrite(Stream&,InputIterator,InputIterator,long long);

template<class OutputIterator,class T>
Stream& RLEread(Stream&,OutputIterator,T*);

template<class InputIterator>
Stream& RLEDIFFwrite(Stream&,InputIterator,InputIterator,long long);

template<class OutputIterator,class T>
Stream& RLEDIFFread(Stream&,OutputIterator,T*);


//-----------------------------------------------------------------------------

} // namespace eclib

#include "eclib/RLE.cc"

#endif
