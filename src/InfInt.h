/*
 * InfInt - Arbitrary-Precision Integer Arithmetic Library
 * Copyright (C) 2013 Sercan Tutar
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 *
 * USAGE:
 *   It is pretty straight forward to use the library. Just create an instance of
 *   InfInt class and start using it.
 *
 *   Useful methods:
 *      intSqrt:        integer square root operation
 *      digitAt:        returns digit at index
 *      numberOfDigits: returns number of digits
 *      size:           returns size in bytes
 *      toString:       converts it to a string
 *
 *   There are also conversion methods which allow conversion to primitive types:
 *   toInt, toLong, toLongLong, toUnsignedInt, toUnsignedLong, toUnsignedLongLong.
 *
 *   You may define INFINT_USE_EXCEPTIONS and library methods will start raising
 *   InfIntException in case of error instead of writing error messages using
 *   std::cerr.
 *
 *   See ReadMe.txt for more info.
 *
 *
 * No overflows, happy programmers!
 *
 */

#ifndef INFINT_H_
#define INFINT_H_

#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <climits>

//#include <limits.h>
//#include <stdlib.h>

#ifdef _WIN32
#define LONG_LONG_MIN LLONG_MIN
#define LONG_LONG_MAX LLONG_MAX
#define ULONG_LONG_MAX ULLONG_MAX
#endif


typedef int ELEM_TYPE;
typedef long long PRODUCT_TYPE;
static const ELEM_TYPE BASE = 1000000000;
static const ELEM_TYPE UPPER_BOUND = 999999999;
static const ELEM_TYPE DIGIT_COUNT = 9;
static const int powersOfTen[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000 };


inline static div_t my_div(int num, int denom)
{
    div_t result;
    result.quot = num / denom;
    result.rem = num - denom * result.quot;
    return result;
}

inline static ldiv_t my_ldiv(long num, long denom)
{
    ldiv_t result;
    result.quot = num / denom;
    result.rem = num - denom * result.quot;
    return result;
}

inline static lldiv_t my_lldiv(long long num, long long denom)
{
    lldiv_t result;
    result.quot = num / denom;
    result.rem = num - denom * result.quot;
    return result;
}

class InfInt
{
    friend std::ostream& operator<<(std::ostream &s, const InfInt &n);
    friend std::istream& operator>>(std::istream &s, InfInt &val);

public:
    /* constructors */
    InfInt();
    InfInt(const char* c);
    InfInt(const std::string& s);
    InfInt(int l);
    InfInt(long l);
    InfInt(long long l);
    InfInt(unsigned int l);
    InfInt(unsigned long l);
    InfInt(unsigned long long l);
    InfInt(const InfInt& l);

    /* assignment operators */
    const InfInt& operator=(const char* c);
    const InfInt& operator=(const std::string& s);
    const InfInt& operator=(int l);
    const InfInt& operator=(long l);
    const InfInt& operator=(long long l);
    const InfInt& operator=(unsigned int l);
    const InfInt& operator=(unsigned long l);
    const InfInt& operator=(unsigned long long l);
    const InfInt& operator=(const InfInt& l);

    /* unary increment/decrement operators */
    const InfInt& operator++();
    const InfInt& operator--();
    InfInt operator++(int);
    InfInt operator--(int);

    /* operational assignments */
    const InfInt& operator+=(const InfInt& rhs);
    const InfInt& operator-=(const InfInt& rhs);
    const InfInt& operator*=(const InfInt& rhs);
    const InfInt& operator/=(const InfInt& rhs); // throw
    const InfInt& operator%=(const InfInt& rhs); // throw
    const InfInt& operator*=(ELEM_TYPE rhs);

    /* operations */
    InfInt operator-() const;
    InfInt operator+(const InfInt& rhs) const;
    InfInt operator-(const InfInt& rhs) const;
    InfInt operator*(const InfInt& rhs) const;
    InfInt operator/(const InfInt& rhs) const; // throw
    InfInt operator%(const InfInt& rhs) const; // throw
    InfInt operator*(ELEM_TYPE rhs) const;

    /* relational operations */
    bool operator==(const InfInt& rhs) const;
    bool operator!=(const InfInt& rhs) const;
    bool operator<(const InfInt& rhs) const;
    bool operator<=(const InfInt& rhs) const;
    bool operator>(const InfInt& rhs) const;
    bool operator>=(const InfInt& rhs) const;

    /* integer square root */
    InfInt intSqrt() const; // throw

    /* digit operations */
    char digitAt(size_t i) const; // throw
    size_t numberOfDigits() const;

    /* size in bytes */
    size_t size() const;

    /* string conversion */
    std::string toString() const;

    /* conversion to primitive types */
    int toInt() const; // throw
    long toLong() const; // throw
    long long toLongLong() const; // throw
    unsigned int toUnsignedInt() const; // throw
    unsigned long toUnsignedLong() const; // throw
    unsigned long long toUnsignedLongLong() const; // throw

private:
    static ELEM_TYPE dInR(const InfInt& R, const InfInt& D);
    static void multiplyByDigit(ELEM_TYPE factor, std::vector<ELEM_TYPE>& val);

    void correct(bool justCheckLeadingZeros = false, bool hasValidSign = false);
    void fromString(const std::string& s);
    void optimizeSqrtSearchBounds(InfInt& lo, InfInt& hi) const;
    void truncateToBase();
    bool equalizeSigns();
    void removeLeadingZeros();

    std::vector<ELEM_TYPE> val; // number with base FACTOR
    bool pos; // true if number is positive
};

#endif