/* 
 *  This file is a part of Symbolic library.
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

#include "gsli/details/gsli_constants.h"
#include "gsli/details/gsli_rep.h"

#include <iostream>

#pragma warning(push)
#pragma warning(disable: 4127)  // conditional expression is constant

namespace sli
{

void global_constants::initialize()
{
    m_int_sign_arr[0]       = 1;
    m_int_sign_arr[1]       = -1;

    m_double_sign_arr[0]    = 1.0;
    m_double_sign_arr[1]    = -1.0;

    init_pow2_pplevel(m_encode_data);
    init_pow2_pmlevel(m_encode_data);
    init_pow2_mplevel(m_encode_data);
    init_pow2_mmlevel(m_encode_data);

    initialize_pow2_pos_decode(m_decode_data);
    initialize_pow2_neg_decode(m_decode_data);

    if (use_decode_table == true)
        initialize_decode_exp(m_decode_exp);
};

void global_constants::init_pow2_pplevel(double* data0)
{
    using config                = config_type;
    static const int max_lev    = config::max_level;

    static const int level_mult     = config::max_index_exp_1 
                                    + config::min_index_exp_1 + 1;
    static const int level_base_1   = config::max_index_exp_0 
                                    - config::max_index_exp_1; 
    static const int level_base_2   = config::max_index_exp_0 
                                    - config::min_index_exp_1; 

    int pos_0       = max_lev + 1;
    double* data    = data0 + pos_0;
    data[0]         = 1.0;

    for (int i = 1; i <= max_lev; ++i)
    {
        int exp     = level_base_1 + i * level_mult;
        data[i]     = std::ldexp(1.0,  exp);
    }

    data[max_lev+1] = 1.0;
};

void global_constants::init_pow2_mplevel(double* data0)
{
    using config                = config_type;
    static const int max_lev    = config::max_level;

    static const int level_mult     = config::max_index_exp_1 
                                    + config::min_index_exp_1 + 1;
    static const int level_base_1   = config::max_index_exp_0 
                                    - config::max_index_exp_1; 
    static const int level_base_2   = config::max_index_exp_0 
                                    - config::min_index_exp_1; 

    int pos_0       = ((max_lev + 1) * 2 + 1) + (max_lev + 1);
    double* data    = data0 + pos_0;
    data[0]         = -1.0;

    for (int i = 1; i <= max_lev; ++i)
    {
        int exp     = level_base_1 + i * level_mult;
        data[i]     = std::ldexp(-1.0,  exp);
    }

    data[max_lev+1] = -1.0;
};

void global_constants::init_pow2_pmlevel(double* data0)
{
    using config                = config_type;
    static const int max_lev    = config::max_level;

    static const int level_mult     = config::max_index_exp_1 
                                    + config::min_index_exp_1 + 1;
    static const int level_base_1   = config::max_index_exp_0 
                                    - config::max_index_exp_1; 
    static const int level_base_2   = config::max_index_exp_0 
                                    - config::min_index_exp_1; 

    int pos_0       = max_lev + 1;
    double* data    = data0 + pos_0;

    for (int i = 1; i <= max_lev; ++i)
    {
        int exp     = level_base_2 + i * level_mult;
        data[-i]    = std::ldexp(1.0,  -exp);
    }

    data[-max_lev-1]= 1.0;
};

void global_constants::init_pow2_mmlevel(double* data0)
{
    using config                = config_type;
    static const int max_lev    = config::max_level;

    static const int level_mult     = config::max_index_exp_1 
                                    + config::min_index_exp_1 + 1;
    static const int level_base_1   = config::max_index_exp_0 
                                    - config::max_index_exp_1; 
    static const int level_base_2   = config::max_index_exp_0 
                                    - config::min_index_exp_1; 

    int pos_0       = ((max_lev + 1) * 2 + 1) + (max_lev + 1);
    double* data    = data0 + pos_0;

    for (int i = 1; i <= max_lev; ++i)
    {
        int exp     = level_base_2 + i * level_mult;
        data[-i]    = std::ldexp(-1.0,  -exp);
    }

    data[-max_lev-1]= -1.0;
};

void global_constants::initialize_pow2_pos_decode(double* pow_ptr0)
{
    using config                = config_type;
    static const int max_lev    = config::max_level;
    static const int level_mult = 1 + config::max_index_exp_1 
                                + config::min_index_exp_1;
    static const int level_base = config::max_index_exp_0 
                                - config::max_index_exp_1;     

    int pos_0                   = config_type::max_level + 1;
    double* pow_ptr             = pow_ptr0 + pos_0;

    pow_ptr[0]                  = 1.0;

    for (int i = 1; i <= max_lev; ++i)
    {
        int off                 = -(i * level_mult + level_base);
        pow_ptr[i]              = std::ldexp(1.0, off);
    };

    pow_ptr[max_lev + 1]        = 1.0;
};

void global_constants::initialize_pow2_neg_decode(double* pow_ptr0)
{
    using config                = config_type;
    static const int max_lev    = config::max_level;
    static const int level_mult = 1 + config::max_index_exp_1 
                                + config::min_index_exp_1;
    static const int level_base = config::max_index_exp_0 
                                - config::max_index_exp_1;     

    int pos_0                   = config_type::max_level + 1;
    double* pow_ptr             = pow_ptr0 + pos_0;

    for (int i = 1; i <= max_lev; ++i)
    {
        int off                 = i * level_mult + config::max_index_exp_0;
        pow_ptr[-i]             = std::ldexp(1.0, off);
    };

    pow_ptr[-max_lev - 1]       = 1.0;
};

void global_constants::initialize_decode_exp(int* data)
{
    using config                = config_type;
    static const int bias       = details::double_decoder::bias;
    static const int max_level  = config_type::max_level;

    // level 0
    static const int min_exp_0  = config::max_index_exp_0;
    static const int max_exp_0  = config::max_index_exp_0;
    static const int min_exp_1  = config::min_index_exp_1;
    static const int max_exp_1  = config::max_index_exp_1;

    static const int base_pos   = max_exp_0 - max_exp_1;
    static const int base_neg   = max_exp_0 - min_exp_1;
    static const int inc_level  = 1 + min_exp_1 + max_exp_1;

    for (int i = -min_exp_0; i <= max_exp_0; ++i)
        data[bias + i]     = 0;

    // positive levels
    for (int i = 1; i <= max_level; ++i)
    {
        // level(+k, pow) = M0 - MAX_1 + k * (1 + MIN_1 + MAX_1) + pow

        for (int pow = -min_exp_1; pow <= max_exp_1; ++pow)
        {
            int enc_pow     = bias + base_pos + i * inc_level + pow;
            data[enc_pow]   = i;
        }
    };

    // inf level
    {
        int enc_pow     = bias + base_pos + (max_level + 1) * inc_level + 0;
        data[enc_pow]   = max_level + 1;
        data[enc_pow+1] = max_level + 1;
    }

    // negative levels
    for (int i = 1; i <= max_level; ++i)
    {
        // level(-k, pow) = -(M0 - MIN_1 + k * (1 + MIN_1 + MAX_1)) + pow

        for (int pow = -min_exp_1; pow <= max_exp_1; ++pow)
        {
            int enc_pow     = bias - (base_neg + i * inc_level) + pow;
            data[enc_pow]   = -i;
        }
    };

    // -inf level
    {
        int enc_pow     = bias - (base_neg + (max_level+1) * inc_level) + max_exp_1;
        data[enc_pow]   = -(max_level + 1);
    }
};

void global_constants::destroy()
{};

global_constants g_global_constants;

// nifty counter
static int g_counter = 0;

global_constants_initializer::global_constants_initializer()
{
    if (g_counter == 0)
        g_global_constants.initialize();

    ++g_counter;
};

global_constants_initializer::~global_constants_initializer()
{
    --g_counter;

    if (g_counter == 0)   
        g_global_constants.destroy();
}

};

#pragma warning(pop)