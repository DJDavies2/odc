/*
 * (C) Copyright 1996-2012 ECMWF.
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 * In applying this licence, ECMWF does not waive the privileges and immunities 
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#ifndef DURATION_HEADER_H
#define DURATION_HEADER_H

#include <stdint.h>

// Forward declarations

namespace utils {

  //! This class represents time durations.
  /*! 
   *  Internally, a Duration is a 64-bit signed integer.
   *  A Duration can be positive, negative or zero.
   *
   *  Durations can be compared or used to modify a DateTime.
   *  The differece between two DateTimes is a Duration.
   *
   *  Durations can be input or output (including via the stream operators
   *  <tt>&gt;&gt;</tt> and  <tt>&lt;&lt;</tt>) as retricted ISO 8601
   *  duration strings:
   *  - [-]P[dD][T[hH][mM][sS]]
   *
   *  Here, square brackets indicate optional content, and "d", "h", "m" and
   *  "s" represent  non-negative integers giving the number of days, hours
   *  minutes and seconds in the Duration. At least one of these quantities
   *  must be present. If "T" appears, then at least one of H, M or S must
   *  be present. Negative durations (which are not part of ISO 8601) are
   *  represented by an initial "-".
   *
   *  For example, the duration "1 day 12 hours and 5 minutes" could be
   *  represented as any of:
   *  - P1DT12H5M
   *  - P1DT12H300S
   *  - PT36H300S
   *  - PT2165M
   *  - PT129900S
   *
   *  On output, the first format will be used.
   */

  class Duration {

//! << and >> can be used to output and set Durations as ISO 8601 strings
  friend std::ostream& operator<<(std::ostream&, const Duration&);
  friend std::istream& operator>>(std::istream&, Duration&);

  public:
    Duration();
    Duration(const int64_t);
    Duration(const std::string &);

    //! Set the duration from an ISO 8601 duration: [-]P[dD]T[hH][mM][sS]
    void set(const std::string &);

    //! Convert a duration to an integer number of seconds.
    int64_t toSeconds () const;

    //! Convert a Duration to an ISO 8601 duration: [-]P[dD]T[hH][mM][sS]
    std::string toString() const;

    //! Comparison operators
    bool operator==(const Duration& other) const;
    bool operator!=(const Duration& other) const;
    bool operator<(const Duration& other) const;
    bool operator<=(const Duration& other) const;
    bool operator>(const Duration& other) const;
    bool operator>=(const Duration& other) const;

    //! Negate a duration
    void negate() {seconds_ = -seconds_;}

    //! Arithmetic operators
    int operator%(const Duration& other) const;

  private:
  // -- Copy allowed
  //  Example(const Example&); -- default shallow copy is OK
  //  Example& operator=(const Example&); -- default shallow copy is OK
  
  std::string eatDigits(std::istream &);


    int64_t seconds_;  // 32-bit int overflows at ~68 years

  };

} //namespace utils
#endif
