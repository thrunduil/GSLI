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

#include "gsli/gsli.h"

#include <vector>

namespace sli { namespace testing
{

struct test_gsli
{
    private:
        using gsli_double   = sli::gsli_double;

    public:
        void            test();

        void            test_rep();
        void            test_constants();
        void            test_nextafter(bool up);
        void            test_io();

    private:
        bool            check_rep_level(int lev, double ind, bool sign_ind);

        double          calc_prec(double exact, double calc, double tol);
        double          calc_prec(const gsli_double& res1, const gsli_double& res2);

        double          make_pow2_value(int pow, int subvalue);

        template<class Func>
        void            test_unary(int n_sample);

        template<class Func>
        void            test_binary(int n_sample);
        void            test_pow_int(int n_sample);

        template<class Func>
        void            test_binary2(int n_sample);

        void            test_mult_lev(int n_sample, int max_lev);
        void            test_plus_lev(int n_sample, int max_lev);
        void            test_log_exp_lev(int n_sample, int max_lev);
        gsli_double     simple_mult(const gsli_double& v1, const gsli_double& v2);
        gsli_double     simple_plus(const gsli_double& v1, const gsli_double& v2);

    public:
        static void     rand_binary(double& res1, double& res2);
        static void     rand_plus(double& res1, double& res2);
        static void     rand_pow_real(double& res1, double& res2);
        static void     rand_pow_int(double& res1, int& res2);
        static double   rand_num(bool with_denorm, bool with_special);
        static double   rand_special();
        static double   rand_special_base();
        static double   rand_num(int min_exp, int max_exp);
        static int      rand_int(int min, int max);
        static double   get_prob_special();
        static void     rand_exp(double& res1);
        static void     rand_unary(double& res1);
        static void     rand_mult(gsli_double& v1, gsli_double& v2, int max_lev);
        static void     rand_plus(gsli_double& v1, gsli_double& v2, int max_lev);
        static gsli_double  rand_gsli(bool with_denorm, bool with_special, int max_lev);

        static void     make_limit_numbers(std::vector<gsli_double>& limit_nubers);
        static bool     equal_nan(const gsli_double& x, const gsli_double& x2);
};

}};