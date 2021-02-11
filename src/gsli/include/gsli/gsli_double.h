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

#include "gsli/config.h"
#include "gsli/details/gsli_constants.h"
#include "gsli/details/gsli_rep.h"

namespace sli
{

// generalized symmetric level-index arithmetic based on double
// precision floating point type; the sli representation for 
// regular numbers is given by:
//
//     val = sig_ind phi(level, index) ^ sig_lev
//
// where 0 <= level <= MAX_LEVEL; and
//     MIN_0 <= index <= MAX_0     if level = 0
//     MIN_1 <  index <= MAX_1     if level != 0
//
// and the function phi is given by:
//
//     phi(l, x) 
//          = x                                     if l = 0
//          = MAX_0 * exp(psi(l-1,x) - MIN_1)       if l > 0
//      psi(l, x)
//          = x                                     if l = 0
//          = MAX_1 * exp(psi(l - 1, x) - MIN_1)    otherwise

// double precition number in generalized symmetric level-index 
// representation
class gsli_double  
{
    public:
        // config storing representation parameters
        using config_type   = gsli_rep::config_type;

        // floating point representation tag
        struct float_rep{};

        // internal encoding tag
        struct internal_rep{};

    private:
        // data storing magnitude of index, level and sign flags; 
        // also exact double representation if level is zero
        double              m_data;

    public:
        //------------------------------------------------------
        //                  constructors
        //------------------------------------------------------
        // initialize to 0.0
        gsli_double();

	    //conversion from a floating point number
	    gsli_double(double a);

	    // construct value from sli representation, where |level| >= 1, 
        // and 0 <= |index| <= MAX_1; if level > MAX_LEVEL, then special 
        // value is constructed (0 if sign_lev = true, nan if index = nan, 
        // and inf otherwise); if |index| <= MIN_1, then |index| is set to
        // the smallest valid number 
        gsli_double(int level, double index);

        // construct from representation object
        explicit gsli_double(const gsli_rep& rep);

        // construct zero number
        static gsli_double  make_zero();

        // construct NaN number
        static gsli_double  make_nan();

        // construct +- infinity
        static gsli_double  make_infinity(bool sign);

        //------------------------------------------------------
        //        constructors from exact reresentations
        //------------------------------------------------------
        // no tests are performed; if constraints are violated,
        // then representation is invalid

	    // constructor from a floating point number, that uses
        // floating point representation; 
        // requires: MIN_0 <= |a| <= MAX_0; or a == 0.0
	    gsli_double(double a, float_rep);

        // constructor from internal representation
        gsli_double(double a, internal_rep);

	    // construct value from sli representation, where |level| >= 1, 
        // |level| <= MAX_LEVEL; and MIN_1 < index <= MAX_1;
        // or from representation of non finite numbers
        gsli_double(int level, double index, bool sign_ind);

    public:
        // convert this value to double
	    double              get_value() const;

        // return index, index sign, level, and level sign
        void                get_gsli_rep(gsli_rep& rep) const;

        // return internal representation
        const double&       get_internal_rep() const;

    public:
        //------------------------------------------------------
        //          arithmetic assignment operators
        //------------------------------------------------------

        // plus assign operator
        gsli_double&        operator+=(const gsli_double& s);
        gsli_double&        operator+=(double s);

        // subtact assign operator
        gsli_double&        operator-=(const gsli_double& s);
        gsli_double&        operator-=(double s);

        // multiply assign operator
        gsli_double&        operator*=(const gsli_double& s);
        gsli_double&        operator*=(double s);

        // divide assign operator
        gsli_double&        operator/=(const gsli_double& s);
        gsli_double&        operator/=(double s);

    private:
        // build sli representation; abs_a cannot have level 0
        // representation
        void                build_gsli(double abs_a, bool is_neg);

        // encode gsli representation into double type; 
        // require MIN_1 < index <= MAX_1; 1 <= |level| <= MAX_LEVEL; 
        // or this is a representation of non finite numbers
        static void         encode_data(double& index, bool sign_index, 
                                int level);

        // encode gsli representation into double type; 
        // require 0 <= index <= MAX_1; 1 <= |level|
        static void         encode_data_normalize(double& index, bool sign_ind, 
                                int level);

        // encode gsli representation of +- infinity into double type
        static double       encode_data_inf(bool sign);

        // encode gsli representation of nan into double type
        static double       encode_data_nan();

        // decode gsli representation
        static void         decode_data(const double& data, gsli_rep& rep);
};

};

#include "gsli/details/gsli_double.inl"