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

struct test_gsli_perf
{
    private:
        using gsli_double   = sli::gsli_double;

    public:
        void            test();

        void            test_cons_small(int max);
        void            test_cons_large(int n_vec, int n_rep);
        void            test_inv_small(int n_vec, int n_rep);
        void            test_log_small(int n_vec, int n_rep);
        void            test_exp_small(int n_vec, int n_rep);

        void            test_add_small(int n_vec, int n_rep);
        void            test_sub_small(int n_vec, int n_rep);
        void            test_mult_small(int n_vec, int n_rep);
        void            test_div_small(int n_vec, int n_rep);
        void            test_pow_small(int n_vec, int n_rep);
        void            test_pow_int_small(int n_vec, int n_rep);

        template<class Func>
        void            test_cmp_small(int n_vec, int n_rep);

        void            test_add_large(int n_vec, int n_rep, int max_lev);
        void            test_mult_large(int n_vec, int n_rep, int max_lev);
        void            test_log_large(int n_vec, int n_rep, int max_lev);
        void            test_exp_large(int n_vec, int n_rep, int max_lev);
        void            test_pow_large(int n_vec, int n_rep, int max_lev);

    private:
        void            rand_double_add(std::vector<double>& vec, int n_elem);
        void            rand_double_mult(std::vector<double>& vec, int n_elem);
        void            rand_double_all_0(std::vector<double>& vec, int n_elem);
        void            rand_double_all(std::vector<double>& vec, int n_elem);
        void            rand_double_exp(std::vector<double>& vec, int n_elem);
        void            rand_double_pow1(std::vector<double>& vec, int n_elem);
        void            rand_double_pow2(std::vector<double>& vec, int n_elem);
        void            rand_double_pow_int2(std::vector<int>& vec, int n_elem);
        void            rand_gsli_add(std::vector<gsli_double>& vec, int n_elem);
        void            rand_gsli_mult(std::vector<gsli_double>& vec, int n_elem);
        void            rand_gsli_all_0(std::vector<gsli_double>& vec, int n_elem);
        void            rand_gsli_all(std::vector<gsli_double>& vec, int n_elem, int max_lev);
        void            rand_gsli_exp(std::vector<gsli_double>& vec, int n_elem);
        void            rand_gsli_pow1(std::vector<gsli_double>& vec, int n_elem);
        void            rand_gsli_pow2(std::vector<gsli_double>& vec, int n_elem);
};

}};