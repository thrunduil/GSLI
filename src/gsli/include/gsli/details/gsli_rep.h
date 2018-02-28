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
#include "gsli/details/gsli_constants.h"

#include <stdint.h>

namespace sli { namespace details
{

//----------------------------------------------------------------------
//                        helper functions
//----------------------------------------------------------------------

// binary representation of IEEE 758 double number
struct double_binary_rep
{
    using base_type = uint64_t;
       
    static const int bias   = 1023;

    uint64_t    fraction    : 52;
    uint64_t    exponent    : 11;
    uint64_t    sign        : 1; 
};

// decode IEEE 754 double value; denormal numbers are not
// supported
union double_decoder
{
    public:
        static const int bias   = double_binary_rep::bias;

    private:
        double*             m_value;
        double_binary_rep*  m_rep;

    public:
        // decode a value val (val cannot be denormal)
        double_decoder(double* val);

        // return double value
        double  value() const;

        // get exponent 
        int     get_exponent() const;

        // get exponent (bias correction not performed)
        size_t  get_raw_exponent() const;

        // get sign bit
        size_t  get_sign() const;

        // increase exponent by val (i.e. multiply by 2^val);
        // no overflow checks are performed
        void    increase_exponent(int val);
};

// decode pointers to IEEE 754 double value; denormal numbers are not
// supported;
union double_decoder_const
{
    private:
        const double*       m_value;
        double_binary_rep*  m_rep;

    public:
        // decode a value val (val cannot be denormal)
        double_decoder_const(const double* val);

        // get exponent 
        int     get_exponent() const;

        // get exponent (bias correction not performed)
        size_t  get_raw_exponent() const;

        // get sign bit
        size_t  get_sign() const;
};

// decompose number a into absilute value and sign
void            get_sign_abs(const double& a, double& abs_a, bool& sign);

// return true if value is NaN
bool            is_nan(double val);

// return true if value is +INF
bool            is_inf_plus(double val);

// return sign_index? -val : val
double          signed_value(bool sign_index, double val);

}};

namespace sli
{

//----------------------------------------------------------------------
//                        gsli_rep
//----------------------------------------------------------------------

// class storing gsli representation; regular numbers are represented as:
//
//     val = sig_ind phi(level, index) ^ sig_lev
//
// where 0 <= level <= MAX_LEVEL; and
//     MIN_0 <= index <= MAX_0     if level = 0
//     MIN_1 <  index <= MAX_1     if level != 0
//
// special numbers:
//     0       - represented as level 0 number
//     +- INF  - encoded on level MAX_LEVEL + 1
//     NaN     - encoded on level MAX_LEVEL + 1
class gsli_rep
{
    public:
        // config storing representation parameters
        using config_type   = config<double, GSLI_MAX_LEVEL_DOUBLE>;

    public:
        // tag used as a request for normalization
        struct normalize{};

        // tag used to indicate, that required constraints can be
        // violated due to numerical inaccuracy
        struct inexact{};

    private:
        // level
        int             m_level;

        // positive index
        double          m_index;

        // index sign
        bool            m_sign_index;

        friend class gsli_double;

    public:
        // construct uninitialized object
        gsli_rep();

        //------------------------------------------------------
        //              exact reresentation
        //------------------------------------------------------
        // no tests are performed; if constraints are violated,
        // then representation is invalid

        // construct level 0 representation;
        // requires: MIN_0 <= index <= MAX_0; or index = 0
        gsli_rep(double index, bool sign_ind);

        // construct level k representation of a regular number, i.e.
        //     MIN_0 <= index <= MAX_0     if level = 0
        //     MIN_1 <  index <= MAX_1     if level != 0
        //     0 <= |level| <= MAX_LEVEL
        // or from a valid representation of special numbers
        gsli_rep(int level, double index, bool sign_ind);

        // inplace construction of level 0 representation;
        // requires: MIN_0 <= index <= MAX_0, or index = 0
        void            make_float_rep(double index, bool sign);

        // inplace construction of level k regular number, i.e.
        //     MIN_0 <= index <= MAX_0     if level = 0
        //     MIN_1 <  index <= MAX_1     if level != 0
        //     0 <= |level| <= MAX_LEVEL
        void            make_gsli_rep(int level, double index, bool sign_ind);

        // inplace construction INF representation
        void            make_inf_rep(bool sign_ind);

        // inplace construction NAN representation
        void            make_nan_rep();

        // inplace construction zero representation
        void            make_zero_rep(bool sign_ind);

        // build gsli representation of zero (potencially signed)
        static gsli_rep make_zero(bool sign);

        // build gsli representation of NaN value
        static gsli_rep make_nan();

        // build gsli representation of +- infinity
        static gsli_rep make_inf(bool sign);

        // return the smallest positive number
        static gsli_rep min();

        // return the largest positive finite number
        static gsli_rep max();

        //------------------------------------------------------
        //              inexact reresentation
        //------------------------------------------------------

        // construct level 0 representation;
        // requires: index is finite; forces MIN_0 <= index <= MAX_0;
        gsli_rep(double index, bool sign_ind, inexact);

        // construct level k representation;
        // requires: index is finite; forces MIN_0 <= index <= MAX_0;

        // construct level k representation of a regular number, i.e.
        //     MIN_0 <= index <= MAX_0     if level = 0
        //     MIN_1 <  index <= MAX_1     if level != 0
        //     0 <= |level| <= MAX_LEVEL
        // or from a valid representation of special numbers; 
        // index is satisfy constraints
        gsli_rep(int level, double index, bool sign_ind, inexact);

        // construct from signed value; representation level is unknown;
        // number need not be regular
        gsli_rep(double signed_index, normalize);

    public:
        //------------------------------------------------------
        //              representation parameters
        //------------------------------------------------------

        // return level
        int             level() const;

        // return (positiv index)
        double          index() const;

        // return true if index is signed
        bool            sign_index() const;

        // return index() * sign_index_value()
        double          signed_index() const;

        // return -1.0 if sign_index() == true and 1.0 otherwise
        double          sign_index_value() const;

        // get absolute value of level and its sign
        void            level_sign(size_t& level, bool& sign_level) const;

    public:
        //------------------------------------------------------
        //              value classification
        //------------------------------------------------------

        // return true if this value represents 0 or -0
        bool            is_zero() const;

        // return true if this object represents 1 or -1
        bool            is_one() const;

        // return true if value is finite (i.e. neither NaN, nor an infinity)
        bool            is_finite() const;

        // return true if value is regular (i.e. neither NaN, nor an infinity 
        // nor zero)
        bool            is_regular() const;

        // return true if this value represents +Inf of -Int
        bool            is_inf() const;

        // return true if this value represents NaN
        bool            is_nan() const;

        // classify value
        fptype          fpclassify() const;

    public:
        //------------------------------------------------------
        //              number conversions
        //------------------------------------------------------

        // convert to floating point value
        double          get_value() const;

        // return true if positive floating point number
        // can be stores in floating point representation
        static bool     can_use_float_rep(double abs_a);

        // calculate level and index of a positive regular number x;
        // if level must be changed, then output argument lev is increased
        // by inc; requires: MIN_1 < x < INF;
        static void     calc_level_index(double& x, int& lev, int inc);

        // calculate level and index of a positive regular number x;
        // it is assumed, that level of x is not zero
        static void     calc_level_index_sign(double& x, int& lev);

    public:
        //------------------------------------------------------
        //              other functions
        //------------------------------------------------------

        // return true if representation is valid
        bool            test_rep() const;
};

};

#include "gsli/details/gsli_rep.inl"