/*
*  Morfa programming language.
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

#include <limits>

namespace sli { namespace details
{

// evaluate x * x
constexpr double square(double x)
{
    return x * x;
}

// evaluate 2^pow, pow >= 0
constexpr double eval_2_pow_k(size_t pow)
{
    return pow == 0 ? 1.0
                    : pow == 1 ? 2.0
                               : square(eval_2_pow_k(pow/2)) * eval_2_pow_k(pow % 2);
};

// test if maximum exponent on level 1 (Exp_1) is large enough, to guarantee,
// that addition on level 2 is trivial
template<int Prec, int Exp_1>
struct test_max_exp_1
{
    // value of log(2)
    static constexpr double log_2 = 0.69314718055994530941723212145818;

    // value = true, if Exp_1 is large enough
    static const bool value     = eval_2_pow_k(Exp_1 - Prec) > log_2*(Prec+2);

    // test_next = true, if Exp_1 + 1 is large enough
    static const bool test_next = eval_2_pow_k(Exp_1 - Prec + 1) > log_2*(Prec+2);

    // next_best = true, if next exponent satisfies contraint
    static const bool next_best = value == false && test_next == true;
};

// evaluate value = Cond ? Val + 1 : eval_max_exp_1<Prec, Val + 1>::value,
template<bool Cond, int Prec, int Val>
struct select_max_exp_1;

template<int Prec, int Val>
struct select_max_exp_1<true, Prec, Val>
{
    static const int value = Val + 1;
};

template<int Prec, int Val>
struct eval_max_exp_1;

template<int Prec, int Val>
struct select_max_exp_1<false, Prec, Val>
{
    static const int value = eval_max_exp_1<Prec, Val + 1>::value;
};

// find smallest exponent of the largest index on level 1, that guaranties,
// that addition on level 2 is trivial;
// we are looking for the smallest index px = 2^Val of a level 1 number x = 
// phi(1,px), such that for any number y, 0 <= y < x: y/x < eps/4; 
// then x + z ~ x for any z, such that |z| < |x|; also x + z ~ z for any z 
// satisying |x| < |z|; y/x < eps/4 => exp(-eps(px)) < 2^(-Prec-2);
// we have eps(px) = 2^(Val - Prec), thus eps(px) > log(2) * (Prec+2) and
// 2^(Val - Prec) > log(2) * (Prec+2);
// then x + x = z, where z = phi(1, px + log[2]); since log[2] < eps(px)/4,
// px + log[2] ~ px and x + x ~ x.
// (condition y/x < eps/4 guarantees, that (x+prev(x))/2 < x +- y < (z+next(x))/2
// and x +- y must be rounded to x) 
template<int Prec, int Val = Prec>
struct eval_max_exp_1
{
    // check if next exponent satisfies constraints
    static const bool valid = test_max_exp_1<Prec, Val>::next_best;

    // evaluate value = valid ? Val + 1 : eval_max_exp_1<Prec, Val + 1>::value,
    // i.e. return Val + 1 if Val + 1 satisfies constraint or continue searching
    static const int value  = select_max_exp_1<valid, Prec, Val>::value;
};

}};

