/*
*  This file is a part of GSLI library.
*
*  Copyright (c) Pawe³ Kowal 2017 - 2021
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

#include "gsli/details/gsli_functions_rep.h"
#include "gsli/gsli_functions.h"
#include "gsli/gsli_error.h"

#include <algorithm>
#include <cmath>

namespace sli { namespace details
{

//----------------------------------------------------------------------
//                        utils
//----------------------------------------------------------------------

// return true if a number x is level 0 number
inline bool is_level_0(const double& x)
{
    return gsli_rep::can_use_float_rep(std::abs(x));
};

// if x passes this test, then inv(x) be level 0 number
inline bool result_inv_level_0(const double& x)
{
    //inverse of any level 0 number is a level 0 number
    return is_level_0(x);
}

// if x passes this test, then log(x) be level 0 number
inline bool result_log_level_0(const double& x)
{
    //log of any level 0 number is a level 0 number
    return is_level_0(x);
}

// if x passes this test, then exp(x) be level 0 number
inline bool result_exp_level_0(const double& x)
{
    // we require |x| < log_max_0
    // when |x| > min_index_0, then x is also level 0 number
    // when |x| <= min_index_0, then x is level -k number (k>0)
    //      but exp(x) ~ 1 and 

    using config                    = gsli_double::config_type;
    static const double log_max     = config::log_max_index_0;

    double abs_x    = std::abs(x);
    bool res1       = abs_x < log_max;

    return res1;
}

// if x, y passes this test, then +-|x| +- |y| must be level 0 number
inline bool result_add_level_0(const double& x, const double& y)
{
    using config    = gsli_double::config_type;

    static constexpr double min = config::min_index_0/config::eps;
    static constexpr double max = config::max_index_0/2.0;

    double abs_x    = std::abs(x);
    double abs_y    = std::abs(y);

    // we have: |x| + |y| <= 2.0 * max(|x|, |y|)
    // and      |x| - |y| >= max(eps(x)/2, eps(y)), for |x| > |y|

    // threfore min_index_0/eps <= |x|, |y| <= max_index_1/2 is sufficient
    // max(eps(x)/2, eps(y)) >= eps(y) >= eps*(min_index_0/eps) = min_index_0
    // then also x and y are level 0 numbers

    bool res1       = min <= abs_x;
    bool res2       = min <= abs_y;
    bool res3       = abs_x <= max;
    bool res4       = abs_y <= max;

    return res1 && res2 && res3 && res4;
}

// if x, y passes this test, then |x| * |y| must be level 0 number
inline bool result_mult_level_0(const double& x, const double& y)
{
    using config    = gsli_double::config_type;

    static constexpr double max = config::max_index_0_sqrt;
    static constexpr double min = 1.0 / max;

    double abs_x    = std::abs(x);
    double abs_y    = std::abs(y);

    // condition sqrt(min_index_0) <= |x|, |y| <= sqrt(max_index_0)
    // is sufficient for x*y to be a level 0 number
    // then also |x|, |y| are level 0 numbers

    bool res1       = min <= abs_x;
    bool res2       = min <= abs_y;
    bool res3       = abs_x <= max;
    bool res4       = abs_y <= max;

    return res1 && res2 && res3 && res4;
}

// if x, y passes this test, then |x| / |y| must be level 0 number
inline bool result_div_level_0(const double& x, const double& y)
{
    //we may use mult test
    return result_mult_level_0(x, y);
}

// if x, y passes this test, then pow_abs(x, y) must be level 0 number
inline bool result_pow_abs_level_0(const double& x, const double& y)
{
    using config                            = gsli_double::config_type;
    static const double sqrt_log_max        = config::sqrt_log_max_index_0;
    static const double exp_sqrt_log_max    = config::exp_sqrt_log_max_index_0;
    static const double exp_sqrt_log_max_inv= config::exp_sqrt_log_max_index_0_inv;

    // pow_abs(x, y) = exp(log(|x|) * y)
    // hence we require (as for exp) |log(|x|)| * |y| < log(max_index_0) (1)

    // condition:
    //      |y|         < sqrt_log_max
    //      |log(|x|)|  < sqrt_log_max
    // is sufficient for (1) to hold, thus we require
    //      |x|         < exp(sqrt_log_max)
    //      |x|         > exp(-sqrt_log_max)
    //  and |y|         >= min_index_0
    //  then x, y are also level 0 numbers

    double abs_x    = std::abs(x);
    double abs_y    = std::abs(y);    

    bool res1       = abs_y < sqrt_log_max;
    bool res2       = abs_y >= config::min_index_0;
    bool res3       = abs_x < exp_sqrt_log_max;
    bool res4       = abs_x > exp_sqrt_log_max_inv;

    return res1 && res2 && res3 && res4;
}

// if x, y passes this test, then pow_int(x, y) must be level 0 number
inline bool result_pow_int_level_0(const double& x, long y)
{
    using config                            = gsli_double::config_type;
    static const long sqrt_log_max          = config::sqrt_log_max_index_0_int;
    static const double exp_sqrt_log_max    = config::exp_sqrt_log_max_index_0;
    static const double exp_sqrt_log_max_inv= config::exp_sqrt_log_max_index_0_inv;

    // pow_abs(x, y) = exp(log(|x|) * y)
    // hence we require (as for exp) |log(|x|)| * |y| < log(max_index_0) (1)

    // condition:
    //      |y|         <= sqrt_log_max
    //      |log(|x|)|  < sqrt_log_max
    // is sufficient for (1) to hold, thus we require
    //      |x|         < exp(sqrt_log_max)
    //      |x|         > exp(-sqrt_log_max)
    //  and |y|         >= 0
    //  then x is also level 0 number

    double abs_x    = std::abs(x);
    long abs_y      = std::abs(y);    

    bool res1       = abs_y <= sqrt_log_max;
    bool res3       = abs_x < exp_sqrt_log_max;
    bool res4       = abs_x > exp_sqrt_log_max_inv;

    return res1 && res3 && res4;
}

// if x, y passes this test; then comparison can be done based
// on internal representation
inline bool can_cmp_level0(const double& x, const double& y)
{
    using config    = gsli_double::config_type;

    // it is enough to check if |x|, |y| > min_index_0
    // then x, y need not be level 0 numbers, but in this
    // case enc(x) op enc(y) == x op y; where enc(x) is result
    // of encoding and op is ==, !=, <, >, <=, or >=

    return config::min_index_0 <= std::abs(x)
            && config::min_index_0 <= std::abs(y);
}

gsli_double plus_general(const gsli_double& x1, const gsli_double& x2);
gsli_double plus_general(const gsli_double& x1, double x2);
gsli_double plus_general(double x1, const gsli_double& x2);

gsli_double mult_general(const gsli_double& x1, const gsli_double& x2);
gsli_double mult_general(const gsli_double& x1, double x2);
gsli_double mult_general(double x1, const gsli_double& x2);

gsli_double div_general(const gsli_double& x1, const gsli_double& x2);
gsli_double div_general(const gsli_double& x1, double x2);
gsli_double div_general(double x1, const gsli_double& x2);

gsli_double pow_abs_general(const gsli_double& x1, const gsli_double& x2);
gsli_double pow_abs_general(const gsli_double& x1, double x2);
gsli_double pow_abs_general(double x1, const gsli_double& x2);

gsli_double pow_int_general(const gsli_double& x1, long x2);

gsli_double inv_general(const gsli_double& x2);
gsli_double log_general(const gsli_double& x2);
gsli_double exp_general(const gsli_double& x2);

bool        cmp_leq_general(const gsli_double& x1, const gsli_double& x2);
bool        cmp_leq_general(const gsli_double& x1, double x2);
bool        cmp_leq_general(double x1, const gsli_double& x2);

bool        cmp_geq_general(const gsli_double& x1, const gsli_double& x2);
bool        cmp_geq_general(const gsli_double& x1, double x2);
bool        cmp_geq_general(double x1, const gsli_double& x2);

bool        cmp_lt_general(const gsli_double& x1, const gsli_double& x2);
bool        cmp_lt_general(const gsli_double& x1, double x2);
bool        cmp_lt_general(double x1, const gsli_double& x2);

bool        cmp_gt_general(const gsli_double& x1, const gsli_double& x2);
bool        cmp_gt_general(const gsli_double& x1, double x2);
bool        cmp_gt_general(double x1, const gsli_double& x2);

}};

namespace sli
{

//----------------------------------------------------------------------
//                        gsli_double
//----------------------------------------------------------------------
inline bool sli::is_zero(const gsli_double& val)
{
    return val.get_internal_rep() == 0.0;
};

inline bool sli::is_negative(const gsli_double& val)
{
    return val.get_internal_rep() < 0.0;
};

inline bool sli::is_positive(const gsli_double& val)
{
    return val.get_internal_rep() > 0.0;
};

inline bool sli::is_signed(const gsli_double& val)
{
    details::double_decoder_const dec(&val.get_internal_rep());
    return dec.get_sign() ? true : false;
};

inline bool sli::is_nan(const gsli_double& val)
{
    return details::is_nan(val.get_internal_rep());
};

inline bool sli::is_inf(const gsli_double& val)
{
    return std::abs(val.get_internal_rep()) == std::numeric_limits<double>::infinity();
};

inline bool sli::is_finite(const gsli_double& val)
{
    return std::abs(val.get_internal_rep()) < std::numeric_limits<double>::infinity();
};

inline bool sli::is_regular(const gsli_double& val)
{
    return std::abs(val.get_internal_rep()) < std::numeric_limits<double>::infinity()
            && val.get_internal_rep() != 0.0;
};

inline bool sli::is_int(const gsli_double& val)
{
    int v_int   = (int)val.get_internal_rep();
    double v_d  = (double)v_int;

    return v_d == val.get_internal_rep();
};

inline fptype sli::fpclassify(const gsli_double& v)
{
    gsli_rep rep;
    v.get_gsli_rep(rep);
    return rep.fpclassify();
}

inline gsli_double sli::operator-(const gsli_double& s)
{
	return gsli_double(-s.get_internal_rep(), gsli_double::internal_rep());
}

inline gsli_double sli::abs(const gsli_double& s)
{
	return gsli_double(::abs(s.get_internal_rep()), gsli_double::internal_rep());
}

inline gsli_double sli::inv(const gsli_double& v)
{
    if (details::result_inv_level_0(v.get_internal_rep()) == true)
        return gsli_double(1.0/v.get_internal_rep(), gsli_double::float_rep());
    else
        return details::inv_general(v);
};

inline gsli_double sli::exp(const gsli_double& v)
{
    if (details::result_exp_level_0(v.get_internal_rep()) == true)
        return gsli_double(std::exp(v.get_internal_rep()), gsli_double::float_rep());
    else
        return details::exp_general(v);
};

inline gsli_double sli::log_abs(const gsli_double& v)
{
    if (details::result_log_level_0(v.get_internal_rep()) == true)
        return gsli_double(std::log(std::abs(v.get_internal_rep())), gsli_double::float_rep());
    else
        return details::log_general(v);
};

inline bool sli::operator==(const gsli_double& x1, const gsli_double& x2)
{
    return x1.get_internal_rep() == x2.get_internal_rep();
};

inline bool sli::operator==(const gsli_double& x1, double x2)
{
    if (details::is_level_0(x2) == true)
        return x1.get_internal_rep() == x2;
    else
        return x1.get_internal_rep() == gsli_double(x2).get_internal_rep();
};

inline bool sli::operator==(double x1, const gsli_double& x2)
{
    if (details::is_level_0(x1) == true)
        return x1 == x2.get_internal_rep();
    else
        return gsli_double(x1).get_internal_rep() == x2.get_internal_rep();
};

inline bool sli::operator!=(const gsli_double& x1, const gsli_double& x2)
{
    return x1.get_internal_rep() != x2.get_internal_rep();
};

inline bool sli::operator!=(const gsli_double& x1, double x2)
{
    if (details::is_level_0(x2) == true)
        return x1.get_internal_rep() != x2;
    else
        return x1.get_internal_rep() != gsli_double(x2).get_internal_rep();
};

inline bool sli::operator!=(double x1, const gsli_double& x2)
{
    if (details::is_level_0(x1) == true)
        return x1 != x2.get_internal_rep();
    else
        return gsli_double(x1).get_internal_rep() != x2.get_internal_rep();
};

inline bool sli::operator>(const gsli_double& x1, const gsli_double& x2)
{
    bool res    = details::can_cmp_level0(x1.get_internal_rep(), x2.get_internal_rep());

    if (res) 
        return x1.get_internal_rep() > x2.get_internal_rep();
    else
        return details::cmp_gt_general(x1, x2);
};

inline bool sli::operator>(const gsli_double& x1, double x2)
{
    if (details::is_level_0(x2) == true)
        return x1.get_internal_rep() > x2;
    else
        return details::cmp_gt_general(x1, x2);
};

inline bool sli::operator>(double x1, const gsli_double& x2)
{
    if (details::is_level_0(x1) == true)
        return x1 > x2.get_internal_rep();
    else
        return details::cmp_gt_general(x1, x2);
};

inline bool sli::operator<(const gsli_double& x1, const gsli_double& x2)
{
    bool res    = details::can_cmp_level0(x1.get_internal_rep(), x2.get_internal_rep());

    if (res) 
        return x1.get_internal_rep() < x2.get_internal_rep();
    else
        return details::cmp_lt_general(x1, x2);
};

inline bool sli::operator<(const gsli_double& x1, double x2)
{
    if (details::is_level_0(x2) == true)
        return x1.get_internal_rep() < x2;
    else
        return details::cmp_lt_general(x1, x2);
};

inline bool sli::operator<(double x1, const gsli_double& x2)
{
    if (details::is_level_0(x1) == true)
        return x1 < x2.get_internal_rep();
    else
        return details::cmp_lt_general(x1, x2);
};

inline bool sli::operator>=(const gsli_double& x1, const gsli_double& x2)
{
    bool res    = details::can_cmp_level0(x1.get_internal_rep(), x2.get_internal_rep());

    if (res) 
        return x1.get_internal_rep() >= x2.get_internal_rep();
    else
        return details::cmp_geq_general(x1, x2);
};

inline bool sli::operator>=(const gsli_double& x1, double x2)
{
    if (details::is_level_0(x2) == true)
        return x1.get_internal_rep() >= x2;
    else
        return details::cmp_geq_general(x1, x2);
};

inline bool sli::operator>=(double x1, const gsli_double& x2)
{
    if (details::is_level_0(x1) == true)
        return x1 >= x2.get_internal_rep();
    else
        return details::cmp_geq_general(x1, x2);
};

inline bool sli::operator<=(const gsli_double& x1, const gsli_double& x2)
{
    bool res    = details::can_cmp_level0(x1.get_internal_rep(), x2.get_internal_rep());

    if (res) 
        return x1.get_internal_rep() <= x2.get_internal_rep();
    else
        return details::cmp_leq_general(x1, x2);
};

inline bool sli::operator<=(const gsli_double& x1, double x2)
{
    if (details::is_level_0(x2) == true)
        return x1.get_internal_rep() <= x2;
    else
        return details::cmp_leq_general(x1, x2);
};

inline bool sli::operator<=(double x1, const gsli_double& x2)
{
    if (details::is_level_0(x1) == true)
        return x1 <= x2.get_internal_rep();
    else
        return details::cmp_leq_general(x1, x2);
};

inline gsli_double sli::operator+(const gsli_double& x1, const gsli_double& x2)
{
    if (details::result_add_level_0(x1.get_internal_rep(), x2.get_internal_rep()) == true)
        return gsli_double(x1.get_internal_rep() + x2.get_internal_rep(), gsli_double::float_rep());
    else
        return details::plus_general(x1, x2);
};

inline gsli_double sli::operator+(const gsli_double& x1, double x2)
{
    if (details::result_add_level_0(x1.get_internal_rep(), x2) == true)
        return gsli_double(x1.get_internal_rep() + x2, gsli_double::float_rep());
    else
        return details::plus_general(x1, x2);
}

inline gsli_double sli::operator+(double x1, const gsli_double& x2)
{
    if (details::result_add_level_0(x1, x2.get_internal_rep()) == true)
        return gsli_double(x1 + x2.get_internal_rep(), gsli_double::float_rep());
    else
        return details::plus_general(x1, x2);
};

//
inline gsli_double sli::operator-(const gsli_double& x1, double x2)
{
    if (details::result_add_level_0(x1.get_internal_rep(), x2) == true)
        return gsli_double(x1.get_internal_rep() - x2, gsli_double::float_rep());
    else
        return details::plus_general(x1, -x2);
};

inline gsli_double sli::operator-(double x1, const gsli_double& x2)
{
    if (details::result_add_level_0(x1, x2.get_internal_rep()) == true)
        return gsli_double(x1 - x2.get_internal_rep(), gsli_double::float_rep());
    else
        return details::plus_general(x1, -x2);
};

inline gsli_double sli::operator-(const gsli_double& x1, const gsli_double& x2)
{
    if (details::result_add_level_0(x1.get_internal_rep(), x2.get_internal_rep()) == true)
        return gsli_double(x1.get_internal_rep() - x2.get_internal_rep(), gsli_double::float_rep());
    else
        return details::plus_general(x1, -x2);
};

inline gsli_double sli::operator*(const gsli_double& x1, const gsli_double& x2)
{
    if (details::result_mult_level_0(x1.get_internal_rep(), x2.get_internal_rep()) == true)
        return gsli_double(x1.get_internal_rep() * x2.get_internal_rep(), gsli_double::float_rep());
    else
        return details::mult_general(x1, x2);
};

inline gsli_double sli::operator*(const gsli_double& x1, double x2)
{
    if (details::result_mult_level_0(x1.get_internal_rep(), x2) == true)
        return gsli_double(x1.get_internal_rep() * x2, gsli_double::float_rep());
    else
        return details::mult_general(x1, x2);
};

inline gsli_double sli::operator*(double x1, const gsli_double& x2)
{
    if (details::result_mult_level_0(x1, x2.get_internal_rep()) == true)
        return gsli_double(x1 * x2.get_internal_rep(), gsli_double::float_rep());
    else
        return details::mult_general(x1, x2);
};

inline gsli_double sli::operator/(const gsli_double& x1, const gsli_double& x2)
{
    if (details::result_div_level_0(x1.get_internal_rep(), x2.get_internal_rep()) == true)
        return gsli_double(x1.get_internal_rep() / x2.get_internal_rep(), gsli_double::float_rep());
    else
        return details::div_general(x1, x2);
};

inline gsli_double sli::operator/(double x1, const gsli_double& x2)
{
    if (details::result_div_level_0(x1, x2.get_internal_rep()) == true)
        return gsli_double(x1 / x2.get_internal_rep(), gsli_double::float_rep());
    else
        return details::div_general(x1, x2);
};

inline gsli_double sli::operator/(const gsli_double& x1, double x2)
{
    if (details::result_div_level_0(x1.get_internal_rep(), x2) == true)
        return gsli_double(x1.get_internal_rep() / x2, gsli_double::float_rep());
    else
        return details::div_general(x1, x2);
};

//
inline gsli_double sli::pow_abs(const gsli_double& x1, const gsli_double& x2)
{
    if (details::result_pow_abs_level_0(x1.get_internal_rep(), x2.get_internal_rep()) == true)
    {
        return gsli_double(std::pow(std::abs(x1.get_internal_rep()), x2.get_internal_rep()), 
                           gsli_double::float_rep());
    }
    else
    {
        return details::pow_abs_general(x1, x2);
    }
};

inline gsli_double sli::pow_abs(double x1, const gsli_double& x2)
{
    if (details::result_pow_abs_level_0(x1, x2.get_internal_rep()) == true)
    {
        return gsli_double(std::pow(std::abs(x1), x2.get_internal_rep()), 
                           gsli_double::float_rep());
    }
    else
    {
        return details::pow_abs_general(x1, x2);
    }
};

inline gsli_double sli::pow_abs(const gsli_double& x1, double x2)
{
    if (details::result_pow_abs_level_0(x1.get_internal_rep(), x2) == true)
    {
        return gsli_double(std::pow(std::abs(x1.get_internal_rep()), x2), 
                           gsli_double::float_rep());
    }
    else
    {
        return details::pow_abs_general(x1, x2);
    }
};

//
inline gsli_double sli::pow_int(const gsli_double& x1, long x2)
{
    if (details::result_pow_int_level_0(x1.get_internal_rep(), x2) == true)
    {
        return gsli_double(std::pow(x1.get_internal_rep(), x2), gsli_double::float_rep());
    }
    else
    {
        return details::pow_int_general(x1, x2);
    }
};

//----------------------------------------------------------------------
//                  gsli_double assign function
//----------------------------------------------------------------------
inline gsli_double& gsli_double::operator+=(const gsli_double& s)
{
    *this = *this + s;
    return *this;
};

inline gsli_double& gsli_double::operator+=(double s)
{
    *this = *this + s;
    return *this;
};

inline gsli_double& gsli_double::operator-=(const gsli_double& s)
{
    *this = *this - s;
    return *this;
};

inline gsli_double& gsli_double::operator-=(double s)
{
    *this = *this - s;
    return *this;
};

inline gsli_double& gsli_double::operator*=(const gsli_double& s)
{
    *this = *this * s;
    return *this;
};

inline gsli_double& gsli_double::operator*=(double s)
{
    *this = *this * s;
    return *this;
};

inline gsli_double& gsli_double::operator/=(const gsli_double& s)
{
    *this = *this / s;
    return *this;
};

inline gsli_double& gsli_double::operator/=(double s)
{
    *this = *this / s;
    return *this;
};

//----------------------------------------------------------------------
//                        gsli_rep
//----------------------------------------------------------------------

inline gsli_rep sli::abs(const gsli_rep& v)
{
    return gsli_rep(v.level(), v.index(), false);
};

inline gsli_rep sli::uminus(const gsli_rep& v)
{
    return gsli_rep(v.level(), v.index(), !v.sign_index());
};

};