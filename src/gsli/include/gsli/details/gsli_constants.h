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

#pragma once 

#include "gsli/config.h"
#include "gsli/details/gsli_constants_impl.h"

#include <limits>

namespace sli
{

// maximum level used in gsli double representation
#define GSLI_MAX_LEVEL_DOUBLE 4

// calculate gsli representation constants for a floating point
// type T with Max_level levels
template<class T, int Max_level>
struct config
{
    static_assert(Max_level >= 1, "invalid Max_level");

    // value of log(2)
    static constexpr double log_2       = 0.69314718055994530941723212145818;

    //----------------------------------------------------------
    //          floating type constants
    //----------------------------------------------------------

    // maximum representable number
    static constexpr double max_value   = std::numeric_limits<T>::max();

    // minimum representable (normal) number
    static constexpr double min_value   = std::numeric_limits<T>::min();

    // maximum exponent for double type
    static const int max_exp            = std::numeric_limits<T>::max_exponent - 1;

    // minimum exponent for double type (for normal numbers)
    static const int min_exp            = std::numeric_limits<T>::min_exponent - 1;

    // mantissa width in bits
    static const int precision          = std::numeric_limits<T>::digits - 1;

    // machine epsilon
    static constexpr double eps         = std::numeric_limits<T>::epsilon();

    // infinite number
    static constexpr double inf         = std::numeric_limits<T>::infinity();

    // NaN number
    static constexpr double nan         = std::numeric_limits<T>::quiet_NaN();

    //----------------------------------------------------------
    //          representation constants
    //----------------------------------------------------------

    // maximum level allowed
    static const int max_level          = Max_level;

    // exponent of the smallest index, that can be represented 
    // on level 1-k; should be close to zero, higher number will 
    // waste bits, since small numbers on levels 1-k are accurate up 
    // to 1eps (as results of log(x), x > 1)
    static const int min_index_exp_1    = 0;

    // exponent of the largest index, that can be represented
    // on level 1-k; should be set to the lowest value, that guaranties,
    // that addition on level 2 is trivial
    static const int    max_index_exp_1 = details::eval_max_exp_1<precision>::value;

    // exponent of largest index, that can be represented on level 0;
    // should be set to maximum number, that guaranties, that all 
    // encoded numbers are represented by finite (normal) numbers, but
    // cannot be higher than |min_exp|/2 (otherwise multiplication on
    // level 0 will be inaccurate)
    static const int    max_index_exp_0 = -min_exp 
                                        - (min_index_exp_1 + max_index_exp_1 + 1) * max_level;

    // maximum index that can be represented on level 0
    static constexpr double max_index_0 = details::eval_2_pow_k(max_index_exp_0);

    // minimum index that can be represented on level 0
    static constexpr double min_index_0 = 1.0 / max_index_0;

    // maximum index that can be represented on levels 1-k
    static constexpr double max_index_1 = details::eval_2_pow_k(max_index_exp_1);

    // inverse of max_index_1
    static constexpr double max_index_1_inv = 1.0 / max_index_1;

    // lower bound on numbers can be represented on levels 1-k
    static constexpr double min_index_1 = 1.0 / details::eval_2_pow_k(min_index_exp_1);

    // minimum index that can be represented on levels 1-k
    static constexpr double min_index_1r = (1.0 + eps) / details::eval_2_pow_k(min_index_exp_1);

    // if true, then log[max_index_0] < eps(max_index_0)/4
    //  = 2^(max_index_exp_0 - precision - 2); sufficient condition is:
    // log(2) * max_index_exp_0 < 2^(max_index_exp_0 - precision - 2)
    static const bool has_large_level_0 = log_2 * max_index_exp_0 
                                        < details::eval_2_pow_k(max_index_exp_0 - precision - 2);

    // square root maximum index that can be represented on level 0
    // upper bound is good enough
    static constexpr double max_index_0_sqrt    = details::eval_2_pow_k(max_index_exp_0/2);

    // use exponent decode table during decoding level
    static const bool use_decode_table  = true;

    //----------------------------------------------------------
    //                  tests of parameters
    //----------------------------------------------------------
    // addition is trivial on level 2 if exp(-eps(2^max_index_exp_1)) <= eps/4;
    // i.e. when 2^(max_index_exp_1 - precision - 2) >= log(2) * precision
    static_assert(details::test_max_exp_1<precision, max_index_exp_1>::value == true, 
            "max_index_exp_1 too small, addition is not trivial on level 2");

    // check if multiplication of level 2 value and level 0 value is trivial
    static_assert(has_large_level_0 == true, "warning: max_index_exp_0 too small");
    
    // equal to log(max_index_0)
    static double   log_max_index_0;

    // equal to log(max_index_1)
    static double   log_max_index_1;

    // equal to log(max_index_0 / max_index_1)
    static double   log_max0_div_max1;

    // equal to log(max_index_0 / max_index_1) + min_index_1
    static double   log_max0_div_max1_p;

    // equal to log(max_index_0) - min_index_1
    static double   log_max_index_0_m;

    // equal to log(max_index_1) - min_index_1
    static double   log_max_index_1_m;

    // equal to 4 * min_index_0 / eps
    static double   four_min_0_div_eps;

    // equal to sqrt(log(max_index_0))
    static double   sqrt_log_max_index_0;

    // equal to floor(sqrt(log(max_index_0)))
    static int      sqrt_log_max_index_0_int;

    // equal to exp(sqrt(log(max_index_0)))
    static double   exp_sqrt_log_max_index_0;

    // equal to exp(-sqrt(log(max_index_0)))
    static double   exp_sqrt_log_max_index_0_inv;
};

template<class T, int Max_level>
double config<T, Max_level>::log_max_index_0 = std::log(max_index_0);

template<class T, int Max_level>
double config<T, Max_level>::log_max_index_0_m = std::log(max_index_0) - min_index_1;

template<class T, int Max_level>
double config<T, Max_level>::log_max_index_1 = std::log(max_index_1);

template<class T, int Max_level>
double config<T, Max_level>::log_max0_div_max1 = std::log(max_index_0 / max_index_1);

template<class T, int Max_level>
double config<T, Max_level>::log_max0_div_max1_p = log_max0_div_max1 + min_index_1;

template<class T, int Max_level>
double config<T, Max_level>::log_max_index_1_m = std::log(max_index_1) - min_index_1;

template<class T, int Max_level>
double config<T, Max_level>::four_min_0_div_eps = 4.0 * min_index_0 / eps;

template<class T, int Max_level>
double config<T, Max_level>::sqrt_log_max_index_0 = std::sqrt(log_max_index_0);

template<class T, int Max_level>
int config<T, Max_level>::sqrt_log_max_index_0_int = (int)std::floor(sqrt_log_max_index_0);

template<class T, int Max_level>
double config<T, Max_level>::exp_sqrt_log_max_index_0 = std::exp(sqrt_log_max_index_0);

template<class T, int Max_level>
double config<T, Max_level>::exp_sqrt_log_max_index_0_inv = std::exp(-sqrt_log_max_index_0);

// precomputed config for double type with 5 levels
struct config_double_precomputed
{
    //----------------------------------------------------------
    //          floating type constants
    //----------------------------------------------------------

    static constexpr double max_value   = 1.7976931348623157e+308;
    static constexpr double min_value   = 2.2250738585072014e-308;
    static const int max_exp            = 1023;
    static const int min_exp            = -1022;
    static const int precision          = 52;
    static constexpr double eps         = std::numeric_limits<double>::epsilon();
    static constexpr double inf         = std::numeric_limits<double>::infinity();
    static constexpr double nan         = std::numeric_limits<double>::quiet_NaN();

    //----------------------------------------------------------
    //          representation constants
    //----------------------------------------------------------
    static const int max_level          = GSLI_MAX_LEVEL_DOUBLE;
    static const int min_index_exp_1    = 0;
    static const int    max_index_exp_1 = 58;

    static const int    max_index_exp_0 = 786;

    static constexpr double max_index_0     = 4.0698330278807704309569407902351e+236;
    static constexpr double min_index_0     = 2.4571032598866017563732286466098e-237;
    static constexpr double max_index_1     = 2.88230376151711744e+17;
    static constexpr double max_index_1_inv = 1.0 / max_index_1;
    static constexpr double min_index_1     = 1.0;
    static constexpr double min_index_1r    = 1.0 * (1.0 + eps);

    static const bool has_large_level_0     = true;
    static const bool use_decode_table      = true;

    //----------------------------------------------------------
    //                  tests of parameters
    //----------------------------------------------------------
    using config_type = config<double, GSLI_MAX_LEVEL_DOUBLE>;

    static_assert(max_value == config_type::max_value, "error");
    static_assert(min_value == config_type::min_value, "error");
    static_assert(max_exp == config_type::max_exp, "error");
    static_assert(min_exp == config_type::min_exp, "error");
    static_assert(precision == config_type::precision, "error");
    static_assert(eps == config_type::eps, "error");
    static_assert(inf == config_type::inf, "error");

    static_assert(max_level == config_type::max_level, "error");
    static_assert(min_index_exp_1 == config_type::min_index_exp_1, "error");
    static_assert(max_index_exp_1 == config_type::max_index_exp_1, "error");

    static_assert(max_index_exp_0 == config_type::max_index_exp_0, "error");
    static_assert(max_index_0 == config_type::max_index_0, "error");
    static_assert(min_index_0 == config_type::min_index_0, "error");
    static_assert(max_index_1 == config_type::max_index_1, "error");
    static_assert(max_index_1_inv == config_type::max_index_1_inv, "error");
    static_assert(min_index_1 == config_type::min_index_1, "error");
    static_assert(min_index_1r == config_type::min_index_1r, "error");
    static_assert(has_large_level_0 == config_type::has_large_level_0, "error");
    static_assert(use_decode_table == config_type::use_decode_table, "error");    
};

// classify value
enum class fptype : int
{
    level_0,        // regular level 0 value
    level_k,        // regular level k value
    zero,           // zero value
    inf,            // infinity 
    nan,            // not-a-number
};

// store for global constants used internally;
// this class should be constructed directly
class global_constants
{
    public:
        using config_type   = config<double, GSLI_MAX_LEVEL_DOUBLE>;

    private:
        static const bool use_decode_table  = config_type::use_decode_table;    
        static const int m_exp_map_size_all = config_type::max_exp - config_type::min_exp + 1 + 2;
        static const int m_exp_map_size     = use_decode_table ? m_exp_map_size_all : 1;

    public:
        // array {1, -1}
        int     m_int_sign_arr[2];

        // array {1.0, -1.0}
        double  m_double_sign_arr[2];

        // array storing encoding parameters used by gsli_double::encode_data
        double  m_encode_data[(2*config_type::max_level + 2 + 1) * 2];

        // array storing decoding parameters used by gsli_double::decode_data
        double  m_decode_data[2*config_type::max_level + 2 + 1];

        // exponent -> level map used during decoding
        int     m_decode_exp[m_exp_map_size];

    private:
        void    initialize();
        void    init_pow2_pplevel(double* data);
        void    init_pow2_pmlevel(double* data);
        void    init_pow2_mplevel(double* data);
        void    init_pow2_mmlevel(double* data);
        void    initialize_pow2_pos_decode(double* data);
        void    initialize_pow2_neg_decode(double* data);
        void    initialize_decode_exp(int* data);

        void    destroy();

        friend struct global_constants_initializer;
};

extern global_constants g_global_constants;

// initializer of global_constants
struct global_constants_initializer 
{
    global_constants_initializer();
    ~global_constants_initializer();
} 

// static initializer for every translation unit
static global_constants_initializer;

};