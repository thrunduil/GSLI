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

#include "gsli/details/gsli_functions_rep.h"
#include "gsli/gsli_error.h"

namespace sli
{

// implement multiplication for regular numbers
struct eval_mult
{
    using config_type = gsli_double::config_type;

    static gsli_rep eval_sli_float(const gsli_rep& rep_p, const gsli_rep& rep_q);

    // assumes |lev(p)| >= |lev(q)|
    static gsli_rep eval_sli_sli(const gsli_rep& rep_p, const gsli_rep& rep_q);

    // return level 1 rep if possible or convert to level 2
    static gsli_rep normalize_level_1_2(double index, bool sign_level, bool sign_index);

    // return level 1 rep if possible or convert to level 0
    static gsli_rep normalize_level_1_0(double index, bool sign_level, bool sign_index);

    // x, y are reqular level 0 indices, but x * y overflows or underflow
    // if overflow = true, then overflow occures, otherwise we have underflow 
    static gsli_rep eval_mult_overflow(const double& x, const double& y, bool overflow);

    // x, y are reqular level 0 indices, but x / y overflows or underflows
    // if overflow = true, then overflow occures, otherwise we have underflow 
    static gsli_rep eval_div_overflow(const double& x, const double& y, bool overflow);
};

gsli_rep eval_mult::eval_mult_overflow(const double& x, const double& y, bool overflow)
{
    using config = gsli_double::config_type;

    if (overflow == true)
    {
        // x * y = inf => |x|, |y| > 1
        gsli_assert(std::abs(x) > 1.0 && std::abs(y) > 1.0, "error in eval_mult_overflow");

        // form log(|x * y|/max_index_0)
        double res  = (x * config::min_index_0) * y;

        double res_a;
        bool sign;
        
        details::get_sign_abs(res, res_a, sign);
        res_a       = std::log(res_a) + config_type::min_index_1;

        return gsli_rep(1, res_a, sign);
    }
    else
    {
        // x * y = 0 => |x|, |y| < 1
        gsli_assert(std::abs(x) < 1.0 && std::abs(y) < 1.0, "error in eval_mult_overflow");

        // form -log(|x * y|*max_index_0)
        double res  = (x * config::max_index_0) * y;

        double res_a;
        bool sign;
        
        details::get_sign_abs(res, res_a, sign);
        res_a       = -std::log(res_a) + config_type::min_index_1;

        return gsli_rep(-1, res_a, sign);
    };
};

gsli_rep eval_mult::eval_div_overflow(const double& x, const double& y, bool overflow)
{
    using config = gsli_double::config_type;

    if (overflow == true)
    {
        // x / y = inf => |x| > 1, |y| < 1
        gsli_assert(std::abs(x) > 1.0 && std::abs(y) < 1.0, "error in eval_mult_overflow");

        // form log(|x / y|/max_index_0)
        double res  = (x * config::min_index_0) / y;

        double res_a;
        bool sign;
        
        details::get_sign_abs(res, res_a, sign);
        res_a       = std::log(res_a) + config_type::min_index_1;

        return gsli_rep(1, res_a, sign);
    }
    else
    {
        // x / y = 0 => |x| < 1, |y| > 1
        gsli_assert(std::abs(x) < 1.0 && std::abs(y) > 1.0, "error in eval_mult_overflow");

        // form -log(|x * y|*max_index_0)
        double res  = (x * config::max_index_0) / y;

        double res_a;
        bool sign;
        
        details::get_sign_abs(res, res_a, sign);
        res_a       = -std::log(res_a) + config_type::min_index_1;

        return gsli_rep(-1, res_a, sign);
    };
};

gsli_rep eval_mult::normalize_level_1_0(double index, bool sign_level, bool sign_index)
{
    if (index > config_type::min_index_1)
    {
        int level   = g_global_constants.m_int_sign_arr[sign_level];
        return gsli_rep(level, index, sign_index);
    }

    index   = index - config_type::min_index_1;

    // index <= 0

    if (sign_level == true)
    {
        index   = ::exp(-index) * config_type::min_index_0;
        return gsli_rep(index, sign_index, gsli_rep::inexact());
    }
    else
    {
        index   = ::exp(index) * config_type::max_index_0;
        return gsli_rep(index, sign_index, gsli_rep::inexact());
    }
}

gsli_rep eval_mult::normalize_level_1_2(double index, bool sign_level, bool sign_index)
{
    if (index <= config_type::max_index_1)
    {
        int level   = g_global_constants.m_int_sign_arr[sign_level];
        return gsli_rep(level, index, sign_index);
    }

    int lz   = g_global_constants.m_int_sign_arr[sign_level];
    gsli_rep::calc_level_index(index, lz, lz);

    return gsli_rep(lz, index, sign_index);
};

gsli_rep eval_mult::eval_sli_sli(const gsli_rep& rep_p, const gsli_rep& rep_q)
{
    gsli_assert(rep_p.is_regular() == true && rep_q.is_regular() == true, 
              "regular numbers required");

    // assumes |lev(p)| >= |lev(q)| 

    size_t lev_p;
    size_t lev_q;
    bool sign_lev_p;
    bool sign_lev_q;

    rep_p.level_sign(lev_p, sign_lev_p);
    rep_q.level_sign(lev_q, sign_lev_q);

    // we assume, that if lev_p > 1,then after rounding p + q = p for any q; 
    // therefor if lev_p > 2, then after rounding p * q = p for any q

    if (lev_p > 2)
    {
        bool first_larger   = lev_p > lev_q 
                            || lev_p == lev_q && rep_p.index() >= rep_q.index();
        
        bool sign_ret       = rep_p.sign_index() != rep_q.sign_index();

        if (first_larger == true)
            return gsli_rep(rep_p.level(), rep_p.index(), sign_ret);
        else
            return gsli_rep(rep_q.level(), rep_q.index(), sign_ret);
    }

    double ind_p    = rep_p.index();
    double ind_q    = rep_q.index();
    bool sign_ind   = rep_p.sign_index() != rep_q.sign_index();

    if (lev_p == 1)
    {
        gsli_assert(lev_q == 1, "error in gsli plus");

        double log_max0     = config_type::log_max_index_0;

        // log(MAX_0) - MIN_1
        double log_max0_off = config_type::log_max_index_0_m;

        if (sign_lev_p != sign_lev_q)
        {
            double z    = ind_p - ind_q;

            // result is on level 1, 0, or -1
            if (z > log_max0)
            {
                z   = z - log_max0_off;
                
                // now z > MIN_1 (rounding to z <= MIN_1 not possible since 
                // eps(log_max0) >= eps(MIN_1)) 
                //  => z - log_max0 +  MIN_1 > MIN_1 + eps(MIN_1) > MIN_1
                // also z <= MAX_1
                return gsli_rep(sign_lev_p ? -1 : 1, z, sign_ind);
            }
            else if (-z > log_max0)
            {
                z   = -z - log_max0_off;

                // now z > MIN_1 (rounding to z <= MIN_1 not possible since 
                // eps(log_max0) >= eps(MIN_1)); also z <= MAX_1
                return gsli_rep(sign_lev_p ? 1 : -1, z, sign_ind);
            };

            // -log_max0 <= z <= log_max0

            if (z == log_max0)
            {
                if (sign_lev_p == false)
                    return gsli_rep(config_type::max_index_0, sign_ind);
                else
                    return gsli_rep(config_type::min_index_0, sign_ind);
            }
            else if (z == -log_max0)
            {
                if (sign_lev_p == false)
                    return gsli_rep(config_type::min_index_0, sign_ind);
                else
                    return gsli_rep(config_type::max_index_0, sign_ind);
            };

            // -log_max0 < z < log_max0
            // now it is safe to take exp; exp(z) is in (MIN_0, MAX_0) 
            // even in case of rounding errors in exp function (due to 
            // assumption on log_max0 and exp)

            if (sign_lev_p == false)
            {
                z   = ::exp(z);                               
                return gsli_rep(z, sign_ind);
            }
            else
            {
                z   = ::exp(-z);
                return gsli_rep(z, sign_ind);
            }
        }
        else
        {
            double z = ind_p + ind_q + log_max0_off;
            return normalize_level_1_2(z, sign_lev_p, sign_ind);
        }
    };

    gsli_assert(lev_p == 2, "error in gsli plus");
    gsli_assert(lev_q <= 2, "error in gsli plus");
    gsli_assert(lev_q >= 1, "error in gsli plus");

    gsli_rep log_p  = log_abs(rep_p);
    gsli_rep log_q  = log_abs(rep_q);

    gsli_rep z      = exp(plus(log_p, log_q));
    return gsli_rep(z.level(), z.index(), sign_ind);
};

#pragma warning(push)
#pragma warning(disable:4127)   // conditional expression is constant

gsli_rep eval_mult::eval_sli_float(const gsli_rep& rep_p, const gsli_rep& rep_q)
{
    gsli_assert(rep_p.is_regular() == true && rep_q.is_regular() == true, 
              "regular numbers required");

    if (rep_q.is_one() == true)
    {
        bool sign_ret   = rep_q.sign_index() != rep_p.sign_index();
        return gsli_rep(rep_p.level(), rep_p.index(), sign_ret);
    }

    size_t lev_p;
    size_t lev_q;

    bool sign_lev_p;
    bool sign_lev_q;

    rep_p.level_sign(lev_p, sign_lev_p);
    rep_q.level_sign(lev_q, sign_lev_q);

    // we assume, that if lev_p > 1,then after rounding p + q = p for any q; 
    // therefor if lev_p > 2, then after rounding p * q = p for any q

    if (lev_p > 2)
    {
        bool sign_ret   = rep_q.sign_index() != rep_p.sign_index();
        return gsli_rep(rep_p.level(), rep_p.index(), sign_ret);
    };

    gsli_assert(lev_p <= 2, "error in gsli plus");
    gsli_assert(lev_q == 0, "error in gsli plus");
    
    bool sign_ind   = rep_p.sign_index() != rep_q.sign_index();

    double ind_p    = rep_p.index();
    double ind_q    = rep_q.index();

    if (lev_p == 1)
    {
        static const double min_0   = config_type::min_index_0;
        static const double max_0   = config_type::max_index_0;
        static const double min_1   = config_type::min_index_1;

        double w        = ::log(ind_q);
        double z1       = ind_p + g_global_constants.m_double_sign_arr[sign_lev_p] * w;

        if (z1 > min_1)
        {                
            // level can increase
            int lz   = g_global_constants.m_int_sign_arr[sign_lev_p];
            gsli_rep::calc_level_index(z1, lz, lz);
            return gsli_rep(lz, z1, sign_ind);
        }

        // result is level 0 value
        double z0;

        // do not use w; this may lead to huge errors when ind_p ~ 0

        if (sign_lev_p  == false)
        {
            gsli_assert(ind_q < 1.0, "error in eval_mult");

            // max_0 >= phi(1, ind_p) * ind_q > max_0 * ind_q >= 1
            // also phi(1, ind_p) = max_0*exp(ind_p-min_1) <= max_0/ind_q
            //      => exp(ind_p-min_1) <= 1/ind_q <= max_0
            // hence exp(ind_p-min_1) will not overflow; max_0 * ind_q will
            // not overflow; and z0 cannot overlow
            z0      = (max_0 * ind_q) * ::exp(ind_p - min_1);
        }
        else
        {
            gsli_assert(ind_q > 1.0, "error in eval_mult");

            // min_0 <= ind_q/phi(1, ind_p) <= ind_q / max_0 <= 1
            // also phi(1, ind_p) = max_0*exp(ind_p-min_1) <= ind_q/min_0
            //      => exp(ind_p-min_1) <= ind_q <= max_0
            // hence exp(ind_p-min_1) will not overflow; min_0 * ind_q will
            // not overflow; and z0 cannot overlow

            z0      = (min_0 * ind_q) * ::exp(min_1 - ind_p);
        }        

        // z0 can be outside of range of numbers on level 0 due to rounding 
        // error
        return gsli_rep(z0, sign_ind, gsli_rep::inexact());
    };

    gsli_assert(lev_p == 2, "error in gsli plus");    
    gsli_assert(config_type::has_large_level_0 == true, "has_large_level_0 required");

    bool sign_ret   = rep_q.sign_index() != rep_p.sign_index();
    return gsli_rep(rep_p.level(), rep_p.index(), sign_ret);
};

#pragma warning(pop)

gsli_rep sli::mult(const gsli_rep& rep_p, const gsli_rep& rep_q)
{
    using config    = gsli_rep::config_type;

    fptype ft1  = rep_p.fpclassify();
    fptype ft2  = rep_q.fpclassify();

    if (ft1 == fptype::level_0)
    {
        if (ft2 == fptype::level_0)
        {
            double x1_val   = rep_p.signed_index();
            double x2_val   = rep_q.signed_index();

            double res      = x1_val * x2_val;         
            double res_a    = std::abs(res);

            bool v1         = res_a >= config::min_value;
            bool v2         = res_a <= config::max_value;

            // no overflow
            if (v1 && v2)
                return gsli_rep(res, gsli_rep::normalize());

            return eval_mult::eval_mult_overflow(x1_val, x2_val, v2 == false);
        }
        else if (ft2 == fptype::level_k)
        {
            return eval_mult::eval_sli_float(rep_q, rep_p);
        }
    }
    else if (ft2 == fptype::level_0 && ft1 == fptype::level_k)
    {
        return eval_mult::eval_sli_float(rep_p, rep_q);
    }
    else if (ft2 == fptype::level_k && ft1 == fptype::level_k)
    {
        // numbers are regular
	    if (rep_p.index() == rep_q.index() && rep_p.level() == -rep_q.level())
        {
            return gsli_rep(1.0, rep_p.sign_index() != rep_q.sign_index());
        };

        if (std::abs(rep_p.level()) >= std::abs(rep_q.level()))
            return eval_mult::eval_sli_sli(rep_p, rep_q);
        else
            return eval_mult::eval_sli_sli(rep_q, rep_p);
    };

    // one of number is irregular

    if (ft1 == fptype::zero)
    {
        if (ft2 == fptype::zero)
        {
            bool sign           = rep_p.sign_index() == rep_q.sign_index();
            return gsli_rep::make_zero(sign);
        }
        else if (ft2 == fptype::level_0 || ft2 == fptype::level_k)
        {
            bool sign           = rep_p.sign_index() == rep_q.sign_index();
            return gsli_rep::make_zero(sign);
        }
        
        // second argument is +-inf/nan; return nan
        return gsli_rep::make_nan();
    };

    if (ft2 == fptype::zero)
    {
        if (ft1 == fptype::level_0 || ft1 == fptype::level_k)
        {
            bool sign           = rep_p.sign_index() == rep_q.sign_index();
            return gsli_rep::make_zero(sign);
        }
        
        // second argument is +-inf/nan; return nan
        return gsli_rep::make_nan();
    };

    if (ft1 == fptype::nan || ft2 == fptype::nan)
        return gsli_rep::make_nan();

    // both are inf or regular
    bool sign           = rep_p.sign_index() != rep_q.sign_index();
    return gsli_rep::make_inf(sign);
}

gsli_rep sli::div(const gsli_rep& x1, const gsli_rep& x2)
{
    using config    = gsli_rep::config_type;

	if (x1.level() == 0 && x2.level() == 0)
    {
        double x1_val   = x1.signed_index();
        double x2_val   = x2.signed_index();
        double res      = x1_val / x2_val;
        double res_a    = std::abs(res);

        bool v1         = res_a >= config::min_value;
        bool v2         = res_a <= config::max_value;

        // no overflow
        if (v1 && v2)
            return gsli_rep(res, gsli_rep::normalize());

        return eval_mult::eval_div_overflow(x1_val, x2_val, v2 == false);
    }

	return mult(x1, inv_rep(x2));
}

};
