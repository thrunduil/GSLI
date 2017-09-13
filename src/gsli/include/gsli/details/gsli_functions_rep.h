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
#include "gsli/details/gsli_rep.h"
#include "gsli/gsli_double.h"

namespace sli
{

// inverse function
gsli_double inv(const gsli_rep& v);

// exponential function, exp(x)
gsli_rep    exp(const gsli_rep& v);

// logarithm of absolute value functions, log|x|
gsli_rep    log_abs(const gsli_rep& v);

// arithmetic functions
gsli_rep    minus(const gsli_rep& v1, const gsli_rep& v2);
gsli_rep    plus(const gsli_rep& v1, const gsli_rep& v2);
gsli_rep    mult(const gsli_rep& v1, const gsli_rep& v2);
gsli_rep    div(const gsli_rep& v1, const gsli_rep& v2);

// power function |x|^y
gsli_rep    pow_abs(const gsli_rep& v1, const gsli_rep& v2);

// power function x^y, where y is integer
gsli_rep    pow_int(const gsli_rep& v1, long v2);

// comparison functions
bool        cmp_eq(const gsli_rep& v1, const gsli_rep& v2);
bool        cmp_neq(const gsli_rep& v1, const gsli_rep& v2);
bool        cmp_lt(const gsli_rep& v1, const gsli_rep& v2);
bool        cmp_gt(const gsli_rep& v1, const gsli_rep& v2);
bool        cmp_geq(const gsli_rep& v1, const gsli_rep& v2);
bool        cmp_leq(const gsli_rep& v1, const gsli_rep& v2);

// absolute value
gsli_rep    abs(const gsli_rep& v);

// unary minus
gsli_rep    uminus(const gsli_rep& v);

// inverse function
gsli_rep    inv_rep(const gsli_rep& v);

// return the closest number smaller or larger than x; if x is NaN, then
// NaN is returned; the infinite values are seen as the smallest and largest
// numbers; if the result is zero, it keeps the same sign as x
gsli_rep    nextabove(const gsli_rep& v);
gsli_rep    nextbelow(const gsli_rep& v);

// return number of distinct representations between x and y;
// the result is always a signed integer value stored in double type
double      float_distance(gsli_rep x, gsli_rep y);

};

#include "gsli/details/gsli_functions.inl"