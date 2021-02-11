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

#include "gsli/gsli_double.h"
#include "gsli/gsli_functions.h"

#include <limits>
#include <iostream>

namespace sli
{

//----------------------------------------------------------------------
//                        gsli_double
//----------------------------------------------------------------------
gsli_double::gsli_double(int level, double index)    
{
    bool sign_index;

    details::get_sign_abs(index, m_data, sign_index);
    encode_data_normalize(m_data, sign_index, level);
}

void gsli_double::build_gsli(double abs_a, bool is_neg)
{
	if (abs_a == 0.0)
	{
        m_data      = details::signed_value(is_neg, 0.0);
        return;
	}

    if (abs_a <= std::numeric_limits<double>::max())
    {
        // use gsli representation
        double index        = abs_a;
        bool sign_index     = is_neg;
        int level           = 0;

        gsli_rep::calc_level_index_sign(index, level);

        if (level == 0)
        {
            m_data          = details::signed_value(sign_index, index);
            return;
        };

        m_data  = index;
        encode_data(m_data, sign_index, level);
        return;
    };

    if (details::is_nan(abs_a) == true)
    {
        m_data  = encode_data_nan();
        return;
    }
    else
    {
        // this is infinite number
        m_data  = encode_data_inf(is_neg);
        return;
    };
}

double gsli_double::encode_data_inf(bool sign)
{
    // regular values are mapped to regular double values;
    // we may use special double values to represent inf/nan

    if (sign == true)
        return -std::numeric_limits<double>::infinity();
    else
        return std::numeric_limits<double>::infinity();
}

double gsli_double::encode_data_nan()
{
    // regular values are mapped to regular double values;
    // we may use special double values to represent inf/nan

    return std::numeric_limits<double>::quiet_NaN();
}

void gsli_double::encode_data_normalize(double& index, bool sign_index, int signed_level)
{
    // rep requires, that index is not too small
    if (index <= config_type::min_index_1)
        index       = config_type::min_index_1r;    

    if (std::abs(signed_level) > config_type::max_level)
    {
        if (signed_level < 0)
            index = sign_index ? -0.0 : 0.0;
        else if (details::is_nan(index) == true)
            index = encode_data_nan();
        else 
            index = encode_data_inf(sign_index);

        return;
    };

    gsli_assert(index <= config_type::max_index_1, "invalid gsli rep");
    gsli_assert(std::abs(signed_level) >= 1, "invalid gsli rep");
    gsli_assert(std::abs(signed_level) <= config_type::max_level, "invalid gsli rep");

    return encode_data(index, sign_index, signed_level);
};

};