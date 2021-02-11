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

#include "gsli/details/gsli_functions_rep.h"
#include "gsli/gsli_functions.h"
#include "gsli/gsli_error.h"

namespace sli
{

#pragma warning(push)
#pragma warning(disable:4127)   // conditional expression is constant

gsli_rep sli::exp(const gsli_rep& rep)
{
    using config_type = gsli_double::config_type;

    fptype ft1  = rep.fpclassify();

    switch(ft1)
    {
        case fptype::zero:
            return gsli_rep(1.0, false);

        case fptype::nan:
            return gsli_rep::make_nan();

        case fptype::inf:
        {
            if (rep.sign_index() == true)
                return gsli_rep::make_zero(false);
            else
                return gsli_rep::make_inf(false);
        }
    }

	if (rep.level() == 0)
    {
        double index        = rep.index();
        bool sign_index     = rep.sign_index();

        double log_max_0    = config_type::log_max_index_0;
        double log_max0_off = config_type::log_max_index_0_m;

        // exp(index) < MAX_0
        if (index < log_max_0)
        {
            // it is assumed, that exp satisfy:
            // exp(x) < MAX_0 for x < log(MAX_0)

            // result is level 0 number
            double ind_sign = details::signed_value(sign_index, index);

            return gsli_rep(::exp(ind_sign), false);
        };

        // result should be level 0 but can be level 1 (due to rounding problems)
        if (index == log_max_0)
        {
            if (sign_index == true)
                return gsli_rep(config_type::min_index_0, false);
            else
                return gsli_rep(config_type::max_index_0, false);
        };

        // result is level 1 gsli (or higher)

        double index_z      = index - log_max0_off;
        int lev_z           = 1;

        gsli_rep::calc_level_index(index_z, lev_z, 1);
        lev_z               = g_global_constants.m_int_sign_arr[sign_index] * lev_z;

        return gsli_rep(lev_z, index_z, false);
    };

	if (rep.level() < 0)
    {
        // this is a sli representation of a number very close to zero
		return gsli_rep(1.0, false);
    };

    // sig_z * phi(level_z, index_z) ^ sig_lev_z = exp[sign_index * |v|]
    //      => sig_z = 1, sig_lev_z = sign_index =>
    // M0 * exp[psi(level_z - 1, index_z)-off] = exp[|v|] =>
    //  psi(level_z - 1, index_z) = |v| - log[M0] + off

    if (rep.level() >= config_type::max_level)
        return gsli_rep::make_inf(false);

    gsli_assert(config_type::has_large_level_0 == true, "has_large_level_0 required");

    // in this case log[MAX_0] - off < log[MAX_0] < eps(MAX_0)/4 < eps(|v|)/4; thus
    // psi(level_z - 1, index_z) = |v|
    // M1 * exp[psi(level_z - 2, index_z)-1] = M0 * exp(psi(level_v - 1, index_v) - 1)
    // psi(level_z - 2, index_z) = log[M0/M1] + psi(level_v - 1, index_v)
        
    if (rep.level() >= 2)
    {
        // in this case psi(level_v - 1, index_v) > MAX_0 and again 
        // log[M0/M1] < eps(psi(level_v - 1, index_v)) / 4 => 
        // log[M0/M1] + psi(level_v - 1, index_v) ~ psi(level_v - 1, index_v), thus
        // psi(level_z - 2, index_z) = psi(level_v - 1, index_v)

        int new_lev = rep.level() + 1;
        new_lev     = g_global_constants.m_int_sign_arr[rep.sign_index()] * new_lev;

	    return gsli_rep(new_lev, rep.index(), false);
    }
    else
    {
        // now level_1 = 1 and:
        // psi(level_z - 2, index_z) = log[M0/M1] + index_v => level_z >= 2

        int new_lev     = rep.level() + 1;
        double z        = rep.index() + config_type::log_max0_div_max1;

        gsli_rep::calc_level_index(z, new_lev, 1);

        new_lev         = g_global_constants.m_int_sign_arr[rep.sign_index()] * new_lev;

	    return gsli_rep(new_lev, z, false);
    };
}

gsli_rep sli::log_abs(const gsli_rep& rep)
{
    using config_type = gsli_double::config_type;

    fptype ft1  = rep.fpclassify();

    switch(ft1)
    {
        case fptype::level_0:
        {
            double res  = ::log(rep.index());
            return gsli_rep(res, gsli_rep::normalize());
        }

        case fptype::zero:
            return gsli_rep::make_inf(true);

        case fptype::inf:
            return gsli_rep::make_inf(false);

        case fptype::nan:
            return gsli_rep::make_nan();
    }

    size_t lev;
    bool sign_lev;

    rep.level_sign(lev, sign_lev);

    if (lev == 1)
    {                
        // log(MAX_0) - MIN_1
        double log_max0_off = config_type::log_max_index_0_m;

        double res      = log_max0_off + rep.index();

        // res > min_0
        // if has_large_level_0, then log_max0_off <= log[max_0] 
        //      < eps(max_0)/4 , and log_max0_off + max_0 ~ max_0,
        // thus res <= max_0

        gsli_assert(config_type::has_large_level_0 == true, "trivial mult20 required");

        return gsli_rep(res, sign_lev);
    };

    // log[phi(lev, ind) ^ sig] = sig * (log[M0] - off + psi(lev-1, ind))
    // where lev >= 2    

    gsli_assert(config_type::has_large_level_0 == true, "has_large_level_0 required");

    // in this case log[M0] - off < eps(MAX_0)/4 < eps(psi(lev-1, ind))/4; thus
    // log[M0] - off + psi(lev-1, ind) ~ psi(lev-1, ind) and
    // phi(lev_z, ind_z) ~ psi(lev-1, ind)

    // M0*exp[psi(lev_z-1, ind_z)-1] = M1*exp[psi(lev-2, ind)-1]
    // psi(lev_z-1, ind_z) = psi(lev-2, ind) - log[M0/M1]

    if (lev >= 3)
    {
        // in this case log[M0/M1] < eps(M0)/4 < eps(psi(lev-2, ind)) / 4 and
        // psi(lev-2, ind) - log[M0/M1] ~ psi(lev-2, ind), thus
        // psi(lev_z-1, ind_z) ~ psi(lev-2, ind)
        return gsli_rep(int(lev) - 1, rep.index(), sign_lev);
    };

    if (rep.index() > config_type::log_max0_div_max1_p)
    {
        double z    = rep.index() - config_type::log_max0_div_max1;
        return gsli_rep(int(lev) - 1, z, sign_lev);
    };

    // result has level 0
    // z = M1*exp[ind-1] + log(M0) - 1
    double z        = std::exp(rep.index() - config_type::min_index_1);
    z               = config_type::max_index_1 * z;
    z               = z + config_type::log_max_index_0_m;
    
    return gsli_rep(z, sign_lev, gsli_rep::inexact());
}

#pragma warning(pop)

// both arguments must be regular
static gsli_rep pow_abs_regular(const gsli_rep& rep_p, const gsli_rep& rep_q)
{
    gsli_rep out    = mult(log_abs(rep_p), rep_q);
    out             = exp(out);
    return out;
};

gsli_rep sli::pow_abs(const gsli_rep& rep_p, const gsli_rep& rep_q)
{
    using config_type = gsli_double::config_type;

    // IEEE 754-2008 standards for the pow function:    
    // pow(0, +-0)          -> 1.0
    // pow(0, y)            -> plus infinity for y negative
    // pow(0, y)            -> plus zero for y positive
    // pow(0, NaN)          -> NaN
    // pow(x, +-0)          -> 1 for any x, even a NaN.
    // pow(1, y)            -> 1 for any y, even a NaN;
    // pow(x, -Inf)         -> plus infinity for 0 < |x| < 1, and plus zero for |x| > 1.
    // pow(x, +Inf)         -> plus zero for 0 < |x| < 1, and plus infinity for |x| > 1.
    // pow(Inf, y)          -> plus zero for y negative, and plus infinity for y positive.
    // pow(inf, NaN)        -> NaN

    fptype ft1  = rep_p.fpclassify();
    fptype ft2  = rep_q.fpclassify();

    if (ft1 == fptype::level_0)
    {
        if (rep_p.index() == 1.0)
            return gsli_rep(1.0, false);

        if (ft2 == fptype::level_0)
        {
		    double res      = std::pow(rep_p.index(), rep_q.signed_index());

            // not overflow or underflow
		    if (res >= config_type::min_value && res <= config_type::max_value)
			    return gsli_rep(res, gsli_rep::normalize());

            // overflow occured; use general version
            return pow_abs_regular(rep_p, rep_q);
        }
        else if (ft2 == fptype::level_k)
        {
            return pow_abs_regular(rep_p, rep_q);
        }
    }
    else if ((ft2 == fptype::level_0 || ft2 == fptype::level_k) 
             && ft1 == fptype::level_k)
    {
        return pow_abs_regular(rep_p, rep_q);
    };

    // one of number is irregular

    if (ft1 == fptype::zero)
    {
        if (ft2 == fptype::zero)
        {
            return gsli_rep(1.0, false);
        }
        else if (ft2 == fptype::level_0 || ft2 == fptype::level_k 
                 || ft2 == fptype::inf)
        {
            if (rep_q.sign_index() == false)
                return gsli_rep::make_zero(false);
            else
                return gsli_rep::make_inf(false);
        }

        // second argument is nan
        return gsli_rep::make_nan();
    };

    if (ft2 == fptype::zero)
        return gsli_rep(1.0, false);

    if (ft1 == fptype::nan || ft2 == fptype::nan)
        return gsli_rep::make_nan();

    if (ft1 == fptype::inf)
    {
        if (rep_q.sign_index() == false)
            return gsli_rep::make_inf(false);
        else
            return gsli_rep::make_zero(false);
    };

    gsli_assert(ft2 == fptype::inf, "error in pow_abs");

    // rep_p is regular
    bool p_less_1   = rep_p.level() < 0 || rep_p.level() == 0 && rep_p.index() < 1.0;

    if (p_less_1 == true)
    {
        if (rep_q.sign_index() == false)
            return gsli_rep::make_zero(false);
        else
            return gsli_rep::make_inf(false);
    }
    else
    {
        if (rep_q.sign_index() == true)
            return gsli_rep::make_zero(false);
        else
            return gsli_rep::make_inf(false);
    }
}

// first argument must be regular;  y != 0
static gsli_rep pow_int_regular(const gsli_rep& rep_p, long y)
{
    gsli_rep rep_q  = gsli_rep(double(y), gsli_rep::normalize());
    gsli_rep out    = mult(log_abs(rep_p), rep_q);
    out             = exp(out);

    bool sign       = rep_p.sign_index() == true && (y % 2) != 0;

    if (sign == true)
        return uminus(out);
    else
        return out;
};

gsli_rep sli::pow_int(const gsli_rep& rep_p, long y)
{
    using config_type = gsli_double::config_type;

    // special cases according to IEEE 754-2008 standard for the
    // pow function:    
    // 
    // pow(+-0, y)     -> +-Inf for y < 0 and y odd
    // pow(+-0, y)     -> +Inf  for y < 0 and y even
    // pow(+-0, y)     -> +-0.0 for y > 0 and y odd
    // pow(+-0, y)     -> 0.0   for y > 0 and y even
    // pow(+1, y)      -> 1.0   for any y
    // pow(x, 0)       -> 1.0   for any x, even a NaN
    // pow(-Inf, y)    -> -0.0  for y < 0 and y odd
    // pow(-Inf, y)    -> 0.0   for y < 0 and y even
    // pow(-Inf, y)    -> -Inf  for y > 0 and y odd
    // pow(-Inf, y)    -> +Inf  for y > 0 and y even
    // pow(+Inf, y)    -> +0.0  for y < 0 and +Inf for y > 0
    // pow(NaN, y)     -> NaN   for y != 0

    if (y == 0)
        return gsli_rep(1.0, false);

    fptype ft1  = rep_p.fpclassify();

    if (ft1 == fptype::level_0)
    {
		double res      = std::pow(rep_p.signed_index(), y);

        // no overflow or underflow
		if (res >= config_type::min_value && res <= config_type::max_value)
			return gsli_rep(res, gsli_rep::normalize());

        // overflow occured; use general version
        return pow_int_regular(rep_p, y);
    }
    else if (ft1 == fptype::level_k)
    {
        return pow_int_regular(rep_p, y);
    };

    // one of number is irregular

    if (ft1 == fptype::zero)
    {
        if (y < 0)
        {
            if ((y % 2) != 0)
                return gsli_rep::make_inf(rep_p.sign_index());
            else
                return gsli_rep::make_inf(false);
        }
        else
        {
            if ((y % 2) != 0)
                return gsli_rep::make_zero(rep_p.sign_index());
            else
                return gsli_rep::make_zero(false);
        };
    };

    if (ft1 == fptype::nan)
        return gsli_rep::make_nan();

    //ft1 == fptype::inf
    if (rep_p.sign_index() == true)
    {
        if (y < 0)
        {
            if ((y % 2) != 0)
                return gsli_rep::make_zero(true);
            else
                return gsli_rep::make_zero(false);
        }
        else
        {
            if ((y % 2) != 0)
                return gsli_rep::make_inf(true);
            else
                return gsli_rep::make_inf(false);
        };
    }
    else
    {
        if (y < 0)
            return gsli_rep::make_zero(false);
        else
            return gsli_rep::make_inf(false);
    };
}

};