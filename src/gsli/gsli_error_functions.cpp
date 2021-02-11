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

#include "gsli/gsli_functions.h"
#include "gsli/gsli_error.h"
#include "gsli/details/gsli_functions_rep.h"

#include <boost/math/special_functions/next.hpp>

namespace sli
{

namespace bm = boost::math;

//----------------------------------------------------------------------
//                        error related
//----------------------------------------------------------------------

static gsli_rep eval_next_lev_k(const gsli_rep& rep, bool up)
{
    using config    = gsli_rep::config_type;

    bool index_up   = true;
    bool level_up   = true;

    if (rep.sign_index() == true)
    {
        index_up    = !index_up;
        level_up    = !level_up;
    }

    if (rep.level() < 0)
    {
        index_up    = !index_up;
    }

    if (up == false)
    {
        index_up    = !index_up;
        level_up    = !level_up;
    };

    double dir      = index_up ? config::inf : -config::inf;
    double ind_new  = std::nextafter(rep.index(), dir);

    if (ind_new <= config::max_index_1 && ind_new > config::min_index_1)
        return gsli_rep(rep.level(), ind_new, rep.sign_index());

    if (level_up == true)
    {
        int new_lev = rep.level() + 1;

        if (new_lev > config::max_level)
        {
            return gsli_rep::make_inf(rep.sign_index());
        }
        else if (new_lev == 0)
        {
            return gsli_rep(config::min_index_0, rep.sign_index());
        }
        else
        {
            bool sign_lev   = (new_lev < 0) ? true : false;
            if (sign_lev == false)
                return gsli_rep(new_lev, config::min_index_1r, rep.sign_index());           
            else
                return gsli_rep(new_lev, config::max_index_1, rep.sign_index());
        }
    }
    else
    {
        int new_lev = rep.level() - 1;

        if (new_lev < -config::max_level)
        {
            return gsli_rep::make_zero(rep.sign_index());
        }
        else if (new_lev == 0)
        {
            return gsli_rep(config::max_index_0, rep.sign_index());
        }
        else
        {
            bool sign_lev   = (new_lev < 0) ? true : false;

            if (sign_lev == false)
                return gsli_rep(new_lev, config::max_index_1, rep.sign_index());
            else
                return gsli_rep(new_lev, config::min_index_1r, rep.sign_index());
        }
    };
}

gsli_rep sli::nextabove(const gsli_rep& rep)
{
    using config = gsli_double::config_type;

    fptype ft   = rep.fpclassify();

    switch(ft)
    {
        case fptype::nan:
            return gsli_rep::make_nan();

        case fptype::inf:
        {
            if (rep.sign_index() == false)
                return gsli_rep::make_inf(false);
            else
                return gsli_rep(config::max_level, config::max_index_1, true);
        }

        case fptype::zero:
            return gsli_rep(-config::max_level, config::max_index_1, false);

        case fptype::level_0:
        {            
            if (rep.sign_index() == false)
            {        
                double ind_new  = std::nextafter(rep.index(), config::inf);

                if (ind_new > config::max_index_0)
                    return gsli_rep(1, config::min_index_1r, false);
                else 
                    return gsli_rep(ind_new, false);
            }
            else
            {
                double ind_new  = std::nextafter(rep.index(), -config::inf);

                if (ind_new < config::min_index_0)
                    return gsli_rep(-1, config::min_index_1r, true);
                else 
                    return gsli_rep(ind_new, true);
            }
        }

        case fptype::level_k:
        {
            return eval_next_lev_k(rep, true);
        }

        default:
        {
            gsli_assert(false, "unknown case");
            throw;
        }
    };
};

gsli_rep sli::nextbelow(const gsli_rep& rep)
{
    using config = gsli_double::config_type;

    fptype ft   = rep.fpclassify();

    switch(ft)
    {
        case fptype::nan:
            return gsli_rep::make_nan();

        case fptype::inf:
        {
            if (rep.sign_index() == true)
                return gsli_rep::make_inf(true);
            else
                return gsli_rep(config::max_level, config::max_index_1, false);
        }

        case fptype::zero:
            return gsli_rep(-config::max_level, config::max_index_1, true);

        case fptype::level_0:
        {            
            if (rep.sign_index() == false)
            {        
                double ind_new  = std::nextafter(rep.index(), -config::inf);

                if (ind_new < config::min_index_0)
                    return gsli_rep(-1, config::min_index_1r, false);
                else 
                    return gsli_rep(ind_new, false);
            }
            else
            {
                double ind_new  = std::nextafter(rep.index(), config::inf);

                if (ind_new > config::max_index_0)
                    return gsli_rep(1, config::min_index_1r, true);
                else 
                    return gsli_rep(ind_new, true);
            }
        }

        case fptype::level_k:
        {
            return eval_next_lev_k(rep, false);
        }

        default:
        {
            gsli_assert(false, "unknown case");
            throw;
        }
    };
};

double sli::float_distance(gsli_rep x, gsli_rep y)
{
    using config_type = gsli_double::config_type;

    double inc  = 0;

    // process nan and zero
    if (x.is_nan() == true || y.is_nan() == true)
        return config_type::nan;

    if (x.is_zero() == true)
    {
        if (y.is_zero() == true)
            return 0.0;

        x   = gsli_rep::min();
        y   = abs(y);
        inc += 1.0;
    }
    else if (y.is_zero() == true)
    {
        y   = abs(x);
        x   = gsli_rep::min();
        inc += 1.0;
    };

    if (x.sign_index() != y.sign_index())
    {
        double d1   = float_distance(gsli_rep::min(), abs(x)) + 1;
        double d2   = float_distance(gsli_rep::min(), abs(y)) + 1;

        return d1 + d2 + inc;
    }

    //signs are the same; x, y are regular or inf
    
    if (x.sign_index() == true)
    {
        x   = abs(x);
        y   = abs(y);
    }

    // process inf
    if (x.is_inf() == true)
    {
        if (y.is_inf() == true)
            return inc;

        x   = y;
        y   = gsli_rep::max();
        inc += 1;
    }
    else if (y.is_inf() == true)
    {
        y   = gsli_rep::max();
        inc += 1;
    };

    //x and y are positive regular
    if (cmp_lt(y, x) == true)
        std::swap(x,y);

    if (cmp_eq(x, y) == true)
        return inc;

    //x and y are positive regular and x < y

    int lev_x       = x.level();
    int lev_y       = y.level();

    double ind_x    = x.index();
    double ind_y    = y.index();

    if (lev_x == lev_y)
        return std::abs(bm::float_distance(ind_x, ind_y)) + inc;

    // distance between smallest and largest number on level 0
    static const double distance_level_0    
                        = std::abs(bm::float_distance(config_type::min_index_0, 
                            config_type::max_index_0));

    // distance between smallest and largest number on level 1
    static const double distance_level_1
                        = std::abs(bm::float_distance(config_type::min_index_1r, 
                            config_type::max_index_1));

    if (lev_x < 0 && lev_y > 0)
    {
        // largest number on negative level
        gsli_rep m1     = gsli_rep(-1, config_type::min_index_1r, false);

        // smallest number on positive level
        gsli_rep m2     = gsli_rep(1, config_type::min_index_1r, false);

        double d1       = float_distance(x, m1);
        double d2       = distance_level_0;
        double d3       = float_distance(m2, y);

        return d1 + 1 + d2 + 1 + d3 + inc;
    }

    if (lev_x < 0 && lev_y == 0)
    {
        // largest number on negative level
        gsli_rep m1     = gsli_rep(-1, config_type::min_index_1r, false);

        double d1       = float_distance(x, m1);
        double d2       = std::abs(bm::float_distance(config_type::min_index_0, ind_y));

        return d1 + 1 + d2 + inc;
    };
    
    if (lev_x == 0 && lev_y > 0)
    {
        // smallest number on positive level
        gsli_rep m2     = gsli_rep(1, config_type::min_index_1r, false);

        double d2       = std::abs(bm::float_distance(ind_x, config_type::max_index_0));
        double d3       = float_distance(m2, y);

        return d2 + 1 + d3 + inc;
    };

    // levels of x and y are both positive or both negative
    // and different

    int num_full_levels = lev_y - lev_x - 1;

    if (lev_x < 0)
    {
        double d1   = std::abs(bm::float_distance(ind_x, config_type::min_index_1r));
        double d2   = num_full_levels * distance_level_1;
        double d3   = std::abs(bm::float_distance(ind_y, config_type::max_index_1));

        return d1 + d2 + num_full_levels + 1 + d3 + inc;
    }
    else
    {
        double d1   = std::abs(bm::float_distance(ind_x, config_type::max_index_1));
        double d2   = num_full_levels * distance_level_1;
        double d3   = std::abs(bm::float_distance(config_type::min_index_1r, ind_y));

        return d1 + d2 + num_full_levels + 1 + d3 + inc; 
    }
};

};
