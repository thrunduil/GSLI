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

#include "gsli/details/gsli_rep.h"
#include "gsli/gsli_error.h"

//----------------------------------------------------------------------
//                        helper functions
//----------------------------------------------------------------------
namespace sli { namespace details
{

inline double_decoder::double_decoder(double* val)
    :m_value(val)
{};

inline double double_decoder::value() const
{ 
    return *m_value;
}

inline int double_decoder::get_exponent() const
{ 
    return int(m_rep->exponent) - double_binary_rep::bias; 
};

inline size_t double_decoder::get_raw_exponent() const
{ 
    return m_rep->exponent; 
};

inline size_t double_decoder::get_sign() const
{ 
    return m_rep->sign; 
};

inline void double_decoder::increase_exponent(int val)
{ 
    m_rep->exponent += val; 
};

inline double_decoder_const::double_decoder_const(const double* val)
    :m_value(val)
{};

inline int double_decoder_const::get_exponent() const
{
    return int(m_rep->exponent) - double_binary_rep::bias; 
};

inline size_t double_decoder_const::get_raw_exponent() const
{
    return m_rep->exponent; 
};

inline size_t double_decoder_const::get_sign() const
{
    return m_rep->sign; 
};

inline bool details::is_nan(double val)
{
    // do not use std::isnan; this function is very slow;
    // following implementation works for IEEE-754 machines
    return val != val;
};

inline bool details::is_inf_plus(double val)
{
    return val == std::numeric_limits<double>::infinity();
};

inline void details::get_sign_abs(const double& a, double& abs_a, bool& sign)
{
    // std::signbit is very slow; 
    // simple comparison a < 0.0 is slightly faster, but will not
    // work with -0
    double_decoder_const dec(&a);

    sign    = dec.get_sign() ? true : false;
    abs_a   = std::abs(a);
}

inline double details::signed_value(bool sign_index, double val)
{
    // avoid branch predictions
    return g_global_constants.m_double_sign_arr[sign_index] * val;
}

}};

namespace sli
{

//----------------------------------------------------------------------
//                        gsli_rep
//----------------------------------------------------------------------
inline gsli_rep::gsli_rep()
{};

inline gsli_rep::gsli_rep(double index, bool sign)
    :m_level(0), m_index(index), m_sign_index(sign)
{
    gsli_assert(test_rep() == true, "invalid gsli representation");
};

inline gsli_rep::gsli_rep(double index, bool sign, inexact)
    :m_level(0), m_index(index), m_sign_index(sign)
{
    gsli_assert(index > config_type::min_index_0/2 
              && index < config_type::max_index_0*2, "index too inaccurate");

    if (m_index < config_type::min_index_0)
        m_index     = config_type::min_index_0;
    else if (m_index > config_type::max_index_0)
        m_index     = config_type::max_index_0;

    gsli_assert(test_rep() == true, "invalid gsli representation");
};

inline gsli_rep::gsli_rep(int level, double index, bool sign_ind)
    : m_level(level), m_index(index), m_sign_index(sign_ind)
{    
    gsli_assert(test_rep() == true, "invalid gsli representation");
};

inline void gsli_rep::make_gsli_rep(int  level, double index, bool sign_ind)
{
    m_level         = level;
    m_index         = index;    
    m_sign_index    = sign_ind;    

    gsli_assert(test_rep() == true, "invalid gsli representation");
    gsli_assert(config_type::max_level, "number is not regular");
    gsli_assert(index != 0.0, "number is not regular");
};

inline void gsli_rep::make_float_rep(double index, bool sign)
{
    m_level         = 0;
    m_index         = index;
    m_sign_index    = sign;        

    gsli_assert(test_rep() == true, "invalid gsli representation");
};

inline void gsli_rep::make_inf_rep(bool sign_ind)
{
    m_level         = config_type::max_level + 1;
    m_index         = std::numeric_limits<double>::infinity();
    m_sign_index    = sign_ind;        
};

inline void gsli_rep::make_zero_rep(bool sign_ind)
{
    m_level         = -(config_type::max_level + 1);
    m_index         = 0.0;
    m_sign_index    = sign_ind;        
};

inline void gsli_rep::make_nan_rep()
{
    m_level         = config_type::max_level + 1;
    m_index         = std::numeric_limits<double>::quiet_NaN();
    m_sign_index    = false;        
}

inline gsli_rep gsli_rep::make_nan()
{
    gsli_rep ret;
    ret.make_nan_rep();
    return ret;
};

inline gsli_rep gsli_rep::make_inf(bool sign)
{
    gsli_rep ret;
    ret.make_inf_rep(sign);
    return ret;
};

inline gsli_rep gsli_rep::min()
{
    return gsli_rep(-config_type::max_level, config_type::max_index_1, false);
}

inline gsli_rep gsli_rep::max()
{
    return gsli_rep(config_type::max_level, config_type::max_index_1, false);
}

inline bool gsli_rep::can_use_float_rep(double abs_a)
{
    bool res1   = config_type::min_index_0 <= abs_a;
    bool res2   = abs_a <= config_type::max_index_0;
    return res1 && res2;
}

inline void gsli_rep::calc_level_index(double& index, int& level, int inc)
{
	while (index > config_type::max_index_1)
	{
		index       = ::log(index * config_type::max_index_1_inv);
        index       += config_type::min_index_1;
        level       += inc;
	}
};

inline int gsli_rep::level() const
{
    return m_level;
};

inline double gsli_rep::index() const
{
    return m_index;
};

inline void gsli_rep::level_sign(size_t& level, bool& sign_level) const
{
    sign_level = m_level < 0;
    level       = std::abs(m_level);
}

inline bool gsli_rep::sign_index() const
{
    return m_sign_index;
}

inline double gsli_rep::sign_index_value() const
{
    return g_global_constants.m_double_sign_arr[m_sign_index];
}

inline double gsli_rep::signed_index() const
{
    return g_global_constants.m_double_sign_arr[m_sign_index] * m_index;
};

inline bool gsli_rep::is_zero() const
{
    return m_index == 0.0;
}

inline bool gsli_rep::is_one() const
{
    return m_index == 1.0 && m_level == 0;
}

inline bool gsli_rep::is_finite() const
{
    return std::abs(m_level) <= config_type::max_level;
};

inline bool gsli_rep::is_regular() const
{
    return std::abs(m_level) <= config_type::max_level;
};

inline bool gsli_rep::is_inf() const
{
    return details::is_inf_plus(m_index);
};

inline bool gsli_rep::is_nan() const
{
    return details::is_nan(m_index);
};

inline fptype gsli_rep::fpclassify() const
{
    if (m_level == 0)
        return fptype::level_0;

    if (std::abs(m_level) <= config_type::max_level)
        return fptype::level_k;

    if (m_level < -config_type::max_level)
        return fptype::zero;

    if (details::is_nan(m_index) == true)
        return fptype::nan;
    else
        return fptype::inf;
};

inline gsli_rep gsli_rep::make_zero(bool sign)
{
    return gsli_rep(-(config_type::max_level + 1), 0.0, sign);
};

};