/* 
 *  This file is a part of GSLI library.
 *
 *  Copyright (c) Pawe³ Kowal 2017
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#pragma once 

#include "gsli/config.h"
#include "gsli/gsli_double.h"

#include <iosfwd>

namespace sli
{

//----------------------------------------------------------------------
//                        value classification
//----------------------------------------------------------------------
// return true if this number is negative (i.e. less than 0);
// -0 and NaN are not negative
bool                is_negative(const gsli_double& v);

// return true if this number is positive (i.e. greater than 0);
// NaN is not positive
bool                is_positive(const gsli_double& v);

// return true if this number is signed (-0 is signed, NaN is not signed)
bool                is_signed(const gsli_double& v);

// return true if value is finite (i.e. neither NaN, nor an infinity)
bool                is_finite(const gsli_double& v);

// return true if value is regular (i.e. neither NaN, nor an infinity 
// nor zero)
bool                is_regular(const gsli_double& v);

// return true if this value represents 0 or -0
bool                is_zero(const gsli_double& v);

// return true if this value represents NaN
bool                is_nan(const gsli_double& v);

// return true if this value represents +Inf of -Int
bool                is_inf(const gsli_double& v);

// classify value
fptype              fpclassify(const gsli_double& v);

//----------------------------------------------------------------------
//                     unary functions
//----------------------------------------------------------------------
// unary minus
gsli_double         operator-(const gsli_double& s);

// absolute value
gsli_double         abs(const gsli_double& s);

// inverse function
gsli_double         inv(const gsli_double& p);	

// exponential function, exp(x)
gsli_double         exp(const gsli_double& s);

// logarithm of absolute value functions, log|x|
gsli_double         log_abs(const gsli_double& s);

//----------------------------------------------------------------------
//                     arithmetic functions
//----------------------------------------------------------------------
// addition
gsli_double         operator+(const gsli_double& x1, double x2);
gsli_double         operator+(double x1, const gsli_double& x2);
gsli_double         operator+(const gsli_double& x1, const gsli_double& x2);

// subtraction
gsli_double         operator-(const gsli_double& x1, double x2);
gsli_double         operator-(double x1, const gsli_double& x2);
gsli_double         operator-(const gsli_double& x1, const gsli_double& x2);

// multiplication
gsli_double         operator*(const gsli_double& x1, const gsli_double& x2);
gsli_double         operator*(double x1, const gsli_double& x2);
gsli_double         operator*(const gsli_double& x1, double x2);

// division
gsli_double         operator/(const gsli_double& x1, const gsli_double& x2);
gsli_double         operator/(double x1, const gsli_double& x2);
gsli_double         operator/(const gsli_double& x1, double x2);

// power function |x|^y
//     special cases according to IEEE 754-2008 standard for the
//     pow function:    
//
//     pow(0, +-0)     -> 1.0
//     pow(0, NaN)     -> NaN
//     pow(0, y)       -> +Inf for y < 0
//     pow(0, y)       -> +0.0 for y > 0
//     pow(x, +-0)     -> 1 for any x, even a NaN.
//     pow(1, y)       -> 1 for any y, even a NaN;
//     pow(x, -Inf)    -> +Inf for 0 < x < 1, and +0.0 for x > 1.
//     pow(x, +Inf)    -> +0.0 for 0 < x < 1, and +Inf for x > 1.
//     pow(Inf, y)     -> +0.0 for y < 0, and +Inf for y > 0.
//     pow(Inf, NaN)   -> NaN
//     pow(NaN, y)     -> NaN for y != 0
gsli_double         pow_abs(const gsli_double& x, const gsli_double& y);
gsli_double         pow_abs(const gsli_double& x, double y);
gsli_double         pow_abs(double x, const gsli_double& y);

// power function x^y, where y is integer
//      special cases according to IEEE 754-2008 standard for the
//      pow function:    
//
//      pow(+-0, y)     -> +-Inf for y < 0 and y odd
//      pow(+-0, y)     -> +Inf  for y < 0 and y even
//      pow(+-0, y)     -> +-0.0 for y > 0 and y odd
//      pow(+-0, y)     -> 0.0   for y > 0 and y even
//      pow(+1, y)      -> 1.0   for any y
//      pow(x, 0)       -> 1.0   for any x, even a NaN
//      pow(-Inf, y)    -> -0.0  for y < 0 and y odd
//      pow(-Inf, y)    -> 0.0   for y < 0 and y even
//      pow(-Inf, y)    -> -Inf  for y > 0 and y odd
//      pow(-Inf, y)    -> +Inf  for y > 0 and y even
//      pow(+Inf, y)    -> +0.0  for y < 0 and +Inf for y > 0
//      pow(NaN, y)     -> NaN   for y != 0
gsli_double         pow_int(const gsli_double& x, long y);

//----------------------------------------------------------------------
//                     relational operators
//----------------------------------------------------------------------
// equality comparison
bool                operator==(const gsli_double& p, const gsli_double& q);
bool                operator==(const gsli_double& p, double q);
bool                operator==(double p, const gsli_double& q);

// inequality comparison
bool                operator!=(const gsli_double& p, const gsli_double& q);
bool                operator!=(const gsli_double& p, double q);
bool                operator!=(double p, const gsli_double& q);

// greater than comparison
bool                operator>(const gsli_double& p, const gsli_double& q);
bool                operator>(const gsli_double& p, double q);
bool                operator>(double p, const gsli_double& q);

// less than comparison
bool                operator<(const gsli_double& p, const gsli_double& q);
bool                operator<(const gsli_double& p, double q);
bool                operator<(double p, const gsli_double& q);

// greater or equal comparison
bool                operator>=(const gsli_double& p, const gsli_double& q);
bool                operator>=(const gsli_double& p, double q);
bool                operator>=(double p, const gsli_double& q);

// less or equal comparison
bool                operator<=(const gsli_double& p, const gsli_double& q);
bool                operator<=(const gsli_double& p, double q);
bool                operator<=(double p, const gsli_double& q);

//----------------------------------------------------------------------
//                        error related
//----------------------------------------------------------------------

// return the closest number smaller or larger than x; if x is NaN, then
// NaN is returned; the infinite values are seen as the smallest and largest
// numbers; if the result is zero, it keeps the same sign as x
gsli_double         nextabove(const gsli_double& x);
gsli_double         nextbelow(const gsli_double& x);

// return number of distinct representations between x and y;
// if one of arguments is NaN, then NaN is returned;
// the result is always a signed integer value stored in double type
double              float_distance(const gsli_double& x, const gsli_double& y);

//----------------------------------------------------------------------
//                     input-output
//----------------------------------------------------------------------

// write to output stream; double values are printed according
// to current formatting of the stream os; 
// infinity is printed as (-) Inf, NaN value as NaN, level zero values
// as double values, and level k values as {k, val}
std::ostream&       operator<<(std::ostream& os, const gsli_double& v);

// read from input stream
std::istream&       operator>>(std::istream& is, gsli_double& v);

};

#include "gsli/details/gsli_functions.inl"