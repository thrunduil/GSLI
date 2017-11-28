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

#include "gsli/details/gsli_rep.inl"
#include "gsli/gsli_double.h"
#include "gsli/gsli_error.h"

namespace sli
{

//----------------------------------------------------------------------
//                        gsli_rep
//----------------------------------------------------------------------
gsli_rep::gsli_rep(double signed_index, normalize)
{
    double abs_a;
    bool sign_index;

    details::get_sign_abs(signed_index, abs_a, sign_index);

    if (can_use_float_rep(abs_a) == true)
    {
        m_index         = abs_a;
        m_sign_index    = sign_index;
        m_level         = 0;
    }
    else if (abs_a == 0.0)
    {
        make_zero_rep(sign_index);
    }
    else if (abs_a <= std::numeric_limits<double>::max())
    {
        m_level         = 0;
        m_index         = abs_a;
        m_sign_index    = sign_index;

        calc_level_index_sign(m_index, m_level);
    }
    else if (details::is_nan(abs_a) == true)
    {
        make_nan_rep();
    }
    else
    {
        make_inf_rep(sign_index);
    }

    gsli_assert(test_rep() == true, "invalid gsli representation");
};

gsli_rep::gsli_rep(int level, double index, bool sign_ind, inexact)
{
    m_index         = index;
    m_sign_index    = sign_ind;
    m_level         = level;

    if (level == 0)
    {
        gsli_assert(index > config_type::min_index_0/2 
              && index < config_type::max_index_0*2, "index too inaccurate");

        if (m_index < config_type::min_index_0)
            m_index     = config_type::min_index_0;
        else if (m_index > config_type::max_index_0)
            m_index     = config_type::max_index_0;
    }
    else if (std::abs(level) <= config_type::max_level)
    {
        gsli_assert(index > config_type::min_index_1/2 
              && index < config_type::max_index_1*2, "index too inaccurate");

        if (m_index <= config_type::min_index_1)
            m_index     = config_type::min_index_1r;
        else if (m_index > config_type::max_index_1)
            m_index     = config_type::max_index_1;
    }

    gsli_assert(test_rep() == true, "invalid gsli representation");
};

double gsli_rep::get_value() const
{
    if (this->level() == 0)
        return this->signed_index();    

    double res      = this->index();

    size_t level;
    bool sign_level;

    this->level_sign(level, sign_level);

    if (level == config_type::max_level + 1)
    {
        if (sign_level == true)
            return this->sign_index_value() * 0.0;	

        if (details::is_nan(res) == true)
            return std::numeric_limits<double>::quiet_NaN();

        if (this->sign_index() == true)
            return -std::numeric_limits<double>::infinity();
        else
            return std::numeric_limits<double>::infinity();
    };	    

    static constexpr double off = config_type::min_index_1;

	for (size_t i = 2; i <= level; i++)
		res     = ::exp(res - off) * config_type::max_index_1;

    // last level is treated differently in order to avoid overflows
    // for denormal numbers
    if (sign_level == true)
        res     = ::exp(off - res) * config_type::min_index_0;
    else
        res     = ::exp(res - off) * config_type::max_index_0;

    res         = this->sign_index_value() * res;	
    return res;
};

bool gsli_rep::test_rep() const
{
    if (this->level() == 0)
    {
        if (can_use_float_rep(this->index()) == false)
            return false;

        return true;
    };

    size_t level;
    bool sign_lev;

    level_sign(level, sign_lev);

    if (level == config_type::max_level + 1)
    {
        //this is inf or nan

        if (sign_lev == true)
        {
            if (this->index() == 0.0)
                return true;
            else
                return false;
        }

        // this is inf
        if (details::is_inf_plus(this->index()) == true)
            return true;

        // this is nan
        if (details::is_nan(this->index()) == true)
            return true;
        else
            return false;
    };

    if (this->index() <= config_type::min_index_1)
        return false;

    if (this->index() > config_type::max_index_1)
        return false;

    if (level > config_type::max_level + 1)
        return false;

    return true;
};

void gsli_rep::calc_level_index_sign(double& index, int& level)
{
    int inc         = 0;

	if (index < 1.0)
    {
        index       = -::log(index * config_type::max_index_0);
        inc         = -1;
        --level;        
	}
    else
    {
        index       = ::log(index * config_type::min_index_0);
        inc         = 1;
        ++level;
    };

    index           += config_type::min_index_1;

    if (index == config_type::min_index_1)
    {
        // this value should be rounded to max or min on level 0
        if (level > 0)
            index   = config_type::max_index_0;
        else
            index   = config_type::min_index_0;

        level       = 0;
        return;
    };

    calc_level_index(index, level, inc);
};

};