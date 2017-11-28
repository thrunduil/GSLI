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

#include "gsli/details/gsli_functions_rep.h"
#include "gsli/gsli_error.h"

namespace sli
{

struct eval_plus
{
    using config_type = gsli_double::config_type;

    // p and q are reqular
    static gsli_rep eval_sli_float(const gsli_rep& rep_p, const gsli_rep& rep_q);

    // assumes |p| >= |q| and p != q; p and q are reqular
    static gsli_rep eval_sli_sli(const gsli_rep& rep_p, const gsli_rep& rep_q);
};

gsli_rep eval_plus::eval_sli_float(const gsli_rep& rep_p, const gsli_rep& rep_q)
{
	if (rep_p.level() < 0)
    {
        if (rep_p.level() <= -2)
        {
            // lev_p <= -2 and |ind_p| < |ind_q|
            // then |p/q| = phi(2, ind_p)^-1/ind_q <= MAX_0/phi(2, ind_p) 
            //            < MAX_0/phi(1, MAX_1) < eps/4, by definition of MAX_1
            // thus p + q ~ p

            return rep_q;
        }

        // addition is trivial when |p/q| < eps/4, i.e. 4*MIN_0/eps*exp[ind_p-1] < q
        // sufficient condition is q > 4/M0/eps

        if (rep_q.index() > config_type::four_min_0_div_eps)
            return rep_q;

        // this is a small number; if conversion to double underflows,
        // then result is accurate; resulting number is regular, since
        // inf, nan, and 0 are not possible

		return gsli_rep(rep_p.get_value() + rep_q.signed_index(), gsli_rep::normalize());
    };

    int lev_p       = rep_p.level();
    int lev_q       = rep_q.level();
    (void)lev_q;

    // we assume, that if level of p > 1, then after rounding p + q ~ p for any q 
    if (lev_p > 1)
        return rep_p;

    gsli_assert(lev_p == 1, "error in gsli plus");
    gsli_assert(lev_q == 0, "error in gsli plus");

    double ind_p    = rep_p.index();
    double ind_q    = rep_q.index();

    // ind_q < ind_p

	if (rep_p.sign_index() == rep_q.sign_index())
    {        
        // exp may overflow, but then w = 0 and result is accurate
        double w    = (ind_q * config_type::min_index_0)
                    * std::exp(config_type::min_index_1 - ind_p);
        double z    = ind_p + std::log(1.0 + w);

        // result cannot be on level 2, since log(2)/MAX_1 < eps/4
        return gsli_rep(1, z, rep_p.sign_index(), gsli_rep::inexact());
    }
    else
    {
        // exp may overflow, but then w = 0 and result is accurate
        double w    = (ind_q * config_type::min_index_0)
                    * std::exp(config_type::min_index_1 - ind_p);
        w           = 1.0 - w;
        double z    = ind_p + std::log(w);

        // result has level 1 or level 0

        if (z > config_type::min_index_1)
            return gsli_rep(1, z, rep_p.sign_index());

        // result is level 0 number
        // convert level 1 number to level 0 number
        // exp overflow not possible
        z               = (config_type::max_index_0 * w)
                        * std::exp(ind_p - config_type::min_index_1);

        // z can be outside level 0 bounds due to rounding errors of exp
        return gsli_rep(z, rep_p.sign_index(), gsli_rep::inexact());
    };
};

gsli_rep eval_plus::eval_sli_sli(const gsli_rep& rep_p, const gsli_rep& rep_q)
{
    // assumes |p| > |q| or p == q

    // we assume, that if level of p > 1, then after rounding p + q ~ p for any q 
    
    bool sign_lev_p;
    bool sign_lev_q;
    size_t lev_p;
    size_t lev_q;

    rep_p.level_sign(lev_p, sign_lev_p);
    rep_q.level_sign(lev_q, sign_lev_q);

    if (lev_p > 1)
        return rep_p;

    if (lev_q > 1)
    {
        // lev_q <= -2, lev_p = -1, 0, 1; 
        // then |q/p| < phi(1, MAX_1-eps)/phi(1, MAX_1) < eps/4, by definition of MAX_1
        // thus p + q ~ p

        gsli_assert(rep_q.level() < 0, "error in eval_plus");
        return rep_p;
    }

    gsli_assert(lev_p == 1, "error in gsli plus");
    gsli_assert(lev_q == 1, "error in gsli plus");

    if (sign_lev_p != sign_lev_q)
    {
        gsli_assert(sign_lev_p == false && sign_lev_q == true, 
                    "error in gsli plus");

        // p has level 1 and q has level -1; addition is trivial if |q/p| < eps/4

        using config                    = gsli_double::config_type;

        // addition is trivial if |q/p| < 1/MAX_0^2 <= eps/4
        static const bool is_trivial    = 1.0/(config::max_index_0 * config::max_index_0)
                                        < config::eps / 4;

        // current configuration implies, that every large-small case is trivial
        static_assert(is_trivial == true, "large-small case in eval_plus not implemented");
        
        return rep_p;
    };

    if (sign_lev_p == false)
    {
        //large-large case
        double ind_p    = rep_p.index();
        double ind_q    = rep_q.index();            

        gsli_assert(ind_q <= ind_p, "error in plus");

	    if (rep_p.sign_index() == rep_q.sign_index())
        {
            double z    = ind_p + std::log(1 + std::exp(ind_q - ind_p));

            // result cannot be level 2, since log(2)/MAX_1 < eps/4
            gsli_assert(z <= config_type::max_index_1, 
                  "unexpected level 2 index in eval_plus");

            return gsli_rep(1, z, rep_p.sign_index());
        }
        else
        {
            double w    = 1 - std::exp(ind_q - ind_p);
            double z    = ind_p + std::log(w);

            // result has level 1 or level 0

            if (z > config_type::min_index_1)
                return gsli_rep(1, z, rep_p.sign_index());

            // result is level 0 number
            // convert level 1 number to level 0 number
            // exp overflow not possible
            z               = (config_type::max_index_0 * w)
                            * std::exp(ind_p - config_type::min_index_1);

            // z can be outside level 0 bounds due to rounding errors of exp
            return gsli_rep(z, rep_p.sign_index(), gsli_rep::inexact());
        };
    }
    else
    {
        // small-small case
        double ind_p    = rep_p.index();
        double ind_q    = rep_q.index();

        gsli_assert(ind_p <= ind_q, "error in plus");

	    if (rep_p.sign_index() != rep_q.sign_index())
        {
            double z    = ind_p - std::log(1 - std::exp(ind_p - ind_q));

            // result cannot be level -2, since we assumed, that all
            // additions on level 2 are trivial
            gsli_assert(z <= config_type::max_index_1, 
                  "unexpected level 2 index in eval_plus");

            return gsli_rep(-1, z, rep_p.sign_index());
        }
        else
        {
            double w    = 1 + std::exp(ind_p - ind_q);
            double z    = ind_p - std::log(w);

            // result has level -1 or level 0

            if (z > config_type::min_index_1)
                return gsli_rep(-1, z, rep_p.sign_index());

            // result is level 0 number

            // convert level -1 number to level 0
            // exp overflow not possible
            z               = (config_type::min_index_0 * w) 
                            * std::exp(config_type::min_index_1 - ind_p);

            // z can be outside level 0 bounds due to rounding errors of exp
            return gsli_rep(z, rep_p.sign_index(), gsli_rep::inexact());
        };
    }
};

gsli_rep sli::plus(const gsli_rep& rep_p, const gsli_rep& rep_q)
{
    fptype ft1  = rep_p.fpclassify();
    fptype ft2  = rep_q.fpclassify();

    if (ft1 == fptype::level_0)
    {
        if (ft2 == fptype::level_0)
        {
            // result may be zero
			return gsli_rep(rep_p.signed_index() + rep_q.signed_index(), 
                           gsli_rep::normalize());
        }
        else if (ft2 == fptype::level_k)
        {
            return eval_plus::eval_sli_float(rep_q, rep_p);
        }
    }
    else if (ft1 == fptype::level_k)
    {
        if (ft2 == fptype::level_0)
        {
            return eval_plus::eval_sli_float(rep_p, rep_q);
        }
        else if (ft2 == fptype::level_k)
        {
            // numbers are regular
            gsli_rep abs_p      = abs(rep_p);
            gsli_rep abs_q      = abs(rep_q);

            if (cmp_gt(abs_p, abs_q) == true)
                return eval_plus::eval_sli_sli(rep_p, rep_q);

            if (cmp_neq(abs_p, abs_q) == true)
                return eval_plus::eval_sli_sli(rep_q, rep_p);

            if (rep_p.sign_index() != rep_q.sign_index())
                return gsli_rep::make_zero(false);
            else
                return eval_plus::eval_sli_sli(rep_q, rep_p);
        };
    };

    // one of number is irregular

    if (ft1 == fptype::zero)
    {
        if (ft2 == fptype::zero)
        {
            bool sign_eq    = rep_p.sign_index() == rep_q.sign_index();
            return gsli_rep::make_zero(sign_eq ? rep_p.sign_index() : false);
        }

        return rep_q;
    };

    if (ft2 == fptype::zero)
        return rep_p;

    if (ft1 == fptype::nan || ft2 == fptype::nan)
        return gsli_rep::make_nan();

    if (ft1 == fptype::inf)
    {
        if (ft2 == fptype::inf)
        {
            // both are inf
            bool sign_eq    = rep_p.sign_index() == rep_q.sign_index();
            bool sign       = rep_p.sign_index();

            if (sign_eq)
                return gsli_rep::make_inf(sign);

            return gsli_rep::make_nan();
        }
        else
        {
            return rep_p;
        }
    }

    // first is regular, second is inf
    return rep_q;
}

gsli_rep sli::minus(const gsli_rep& x1, const gsli_rep& x2)
{
    return plus(x1, uminus(x2));
}

};