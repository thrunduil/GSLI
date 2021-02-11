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

#include "gsli/gsli_double.h"
#include "gsli/gsli_error.h"

namespace sli
{

//----------------------------------------------------------------------
//                        gsli_double
//----------------------------------------------------------------------
inline gsli_double::gsli_double()
    : m_data(0.0)
{}

inline gsli_double::gsli_double(double a, float_rep)
    : m_data(a)
{    
    gsli_assert(gsli_rep::can_use_float_rep(::abs(a)) == true || a == 0.0,
                "invalid float rep");
};

inline gsli_double::gsli_double(double a, internal_rep)
    : m_data(a)
{};

inline gsli_double::gsli_double(double a)
    : m_data(a)
{
    double abs_a;
    bool sign;

    details::get_sign_abs(a, abs_a, sign);

	if (gsli_rep::can_use_float_rep(abs_a) == true)
        return;

    build_gsli(abs_a, sign);
};

inline gsli_double::gsli_double(int level, double index, bool sign_ind)
    :m_data(index)
{
    encode_data(m_data, sign_ind, level);
}

inline gsli_double::gsli_double(const gsli_rep& rep)    
{
    if (rep.level() != 0)
    {
        *this = gsli_double(rep.level(), rep.index(), rep.sign_index());
    }
    else
    {
        m_data  = rep.signed_index();

        gsli_assert(gsli_rep::can_use_float_rep(rep.index()) == true,
                  "invalid gsli rep");
    };
};

inline double gsli_double::get_value() const
{
    if (gsli_rep::can_use_float_rep(std::abs(m_data)) == true)
        return m_data;

    gsli_rep rep;
    decode_data(m_data, rep);
    return rep.get_value();
}

inline void gsli_double::get_gsli_rep(gsli_rep& rep) const
{
    decode_data(m_data, rep);
};

inline const double& gsli_double::get_internal_rep() const
{ 
    return m_data; 
};

inline gsli_double gsli_double::make_zero()
{
    return gsli_double(0.0, internal_rep());
};

inline gsli_double gsli_double::make_nan()
{
    return gsli_double(encode_data_nan(), internal_rep());
};

inline gsli_double gsli_double::make_infinity(bool sign)
{
    return gsli_double(encode_data_inf(sign), internal_rep());
};

__forceinline 
void gsli_double::encode_data(double& index, bool sign_index, int signed_level)
{
    gsli_assert(std::abs(signed_level) == config_type::max_level + 1
                || index <= config_type::max_index_1, "invalid gsli rep");
    gsli_assert(std::abs(signed_level) == config_type::max_level + 1
                || index >  config_type::min_index_1, "invalid gsli rep");
    gsli_assert(std::abs(signed_level) >= 1, "invalid gsli rep");
    gsli_assert(std::abs(signed_level) <= config_type::max_level + 1, "invalid gsli rep");

    // level 0 indices are in range 2^-MAX_0 <= ind0 <= 2^MAX_0

    // level +k index ind = sig * 2^pow, in range 2^-MIN_1 < ind <= 2^MAX_1
    // 1 <= sig < 2, is encoded as ind_e = sig * 2^level(+k,pow), where
    //
    //      level(+k, pow) = M0 - MAX_1 + k * (1 + MIN_1 + MAX_1) + pow
    //
    // thus, ind_e = ind * 2^off_k
    //      off_k   = M0 - MAX_1 + k * (MIN_1 + MAX_1 + 1)
    //
    // resulting index ind_e satisfies 2^MIN_k < ind_e <= 2^MAX_k, with
    //
    //  MIN_k = M0 + (k-1) * (MIN_1 + MAX_1 + 1) + 1
    //  MAX_k = M0 + k     * (MIN_1 + MAX_1 + 1)

    // level -k index ind = sig * 2^pow, in range 2^-MIN_1 < ind <= 2^MAX_1
    // 1 <= sig < 2, is encoded as ind_e = sig * 2^level(-k,pow), where
    //
    //      level(-k, pow) = -(M0 - MIN_1 + k * (1 + MIN_1 + MAX_1)) + pow
    //
    // thus, ind_e = ind * 2^-off_k
    //      off_k   = M0 - MIN_1 + k * (MIN_1 + MAX_1 + 1)
    //
    // resulting index ind_e satisfies 2^MIN_k < ind_e <= 2^MAX_k, with
    //    
    //  MIN_k = -M0 - k     * (MIN_1 + MAX_1 + 1)
    //  MIN_k = -M0 - (k-1) * (MIN_1 + MAX_1 + 1) - 1

    // these sets of numbers are different for different levels; 
    //  ... (-M0-2*D, -M0-D-1] (-M0-D, -M0-1] [-M0, M0]
    //      (M0 + 1, M0 + D] (M0 + D + 1, M0 + 2*D] ...
    // D = MIN_1 + MAX_1 + 1

    // therefore this encoding is a bijection; 

    //if sign_level = false, then form +-1.0 * index * g_pow2_plevel_data[level]
    // otherwise form +-1.0 * index * g_pow2_mlevel_data[level]
    // if statement is too slow

    int pos         = int(sign_index)* (2 * config_type::max_level + 2 + 1)
                    + config_type::max_level + 1 + signed_level;

    index           = index * g_global_constants.m_encode_data[pos];

    gsli_assert(std::abs(signed_level) == config_type::max_level + 1
                || ::abs(index) >= config_type::min_value
                    && ::abs(index) <= config_type::max_value, "invalid encoding");

    gsli_assert(std::abs(signed_level) == config_type::max_level + 1 
                || ::abs(index) > config_type::max_index_0
                || ::abs(index) < config_type::min_index_0, "invalid encoding");

    return;
}

#pragma warning(push)
#pragma warning(disable : 4127) //conditional expression is constant

__forceinline
void gsli_double::decode_data(const double& data, gsli_rep& rep)
{
    static const bool use_decode_table  = config_type::use_decode_table;

    details::double_decoder_const dec(&data);
    int exp_bias    = int(dec.get_raw_exponent());

    double index;
    bool sign_index;

    details::get_sign_abs(data, index, sign_index);

    int lev;

    if (use_decode_table == true)
    {
        lev         = g_global_constants.m_decode_exp[exp_bias];
    }
    else
    {
        if (gsli_rep::can_use_float_rep(index) == true)
        {
            rep.make_float_rep(index, sign_index);
            return;
        }    

        // original index : ind = d * 2^k, where 1 <= |d| < 2
        // encoded index  : enc = d * 2^(k + off)
        // condition 2^-MIN_1 < |d|*2^k <= 2^MAX_1 
        //      => -MIN_1 < k < MAX_1 
        //          or k = -MIN_1 and d > 1.0
        //          or k = MAX_1 and d == 1.0

        static const int bias           = details::double_decoder::bias;
        static const int level_mult     = 1 + config_type::max_index_exp_1 
                                        + config_type::min_index_exp_1;
        static const int level_base     = config_type::max_index_exp_0 
                                        - config_type::max_index_exp_1; 

        static const int lev_base_pbias = level_base + bias;
        static const int lev_base_mbias = level_base - bias;

        gsli_assert(config_type::min_index_exp_1 == 0, 
                    "decoder works only for min_index_exp_1 == 0");

        // For exp >= 0:
        // exp - MAX_0 + M1_MAX = lev * (M1_MAX + 1) + k; 
        //  lev > 0, 0 <= k <= M1_MAX;

        // For exp < 0:
        // e2 = exp + MAX_0 = lev * (1 + M1_MAX) + k;  
        //  lev < 0, 0 <= k <= M1_MAX, thus e2 < 0:
        // and:
        // -exp - MAX_0 + M1_MAX = -lev * (1 + M1_MAX) + (M1_MAX - k);
        //   lev < 0; 0 <= M1_MAX - k <= M1_MAX, -exp - MAX_0 + M1_MAX > 0,
        //      thus -lev       = (-exp - MAX_0 + M1_MAX)/(1 + M1_MAX)
        //           M1_MAX - k = (-exp - MAX_0 + M1_MAX) % (1 + M1_MAX)

        if (exp_bias >= bias)
        {
            int exp         = exp_bias - lev_base_pbias;
            lev             = exp / level_mult;
        }
        else
        {
            int exp         = -exp_bias - lev_base_mbias;
            lev             = exp / level_mult;
            lev             = -lev;
        }

        // inf and nan are already decoded properly
    };

    // index = index * 2^off
    int pos                 = config_type::max_level + 1 + lev;
    rep.m_index             = index * g_global_constants.m_decode_data[pos];
    rep.m_sign_index        = sign_index;
    rep.m_level             = lev;
};

#pragma warning(pop)

};