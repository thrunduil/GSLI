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

#pragma once

#include "test_gsli_perf.h"
#include "test_gsli.h"

#include "timer.h"
#include "rand.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace sli { namespace testing
{

struct eq_func
{
    static const char* name()
    {
        return "eq";
    }

    static bool eval_double(double x, double y)   
    { 
        return x == y; 
    };

    static bool eval_gsli(const gsli_double& x, const gsli_double& y)   
    { 
        return x == y; 
    };
};

struct neq_func
{
    static const char* name()
    {
        return "neq";
    }

    static bool eval_double(double x, double y)   
    { 
        return x != y; 
    };

    static bool eval_gsli(const gsli_double& x, const gsli_double& y)   
    { 
        return x != y; 
    };

};

struct leq_func
{
    static const char* name()
    {
        return "leq";
    }

    static bool eval_double(double x, double y)   
    { 
        return x <= y; 
    };

    static bool eval_gsli(const gsli_double& x, const gsli_double& y)   
    { 
        return x <= y; 
    };
};

struct geq_func
{
    static const char* name()
    {
        return "geq";
    }

    static bool eval_double(double x, double y)   
    { 
        return x >= y; 
    };

    static bool eval_gsli(const gsli_double& x, const gsli_double& y)   
    { 
        return x >= y; 
    };
};

struct lt_func
{
    static const char* name()
    {
        return "lt";
    }

    static bool eval_double(double x, double y)   
    { 
        return x < y; 
    };

    static bool eval_gsli(const gsli_double& x, const gsli_double& y)   
    { 
        return x < y; 
    };
};

struct gt_func
{
    static const char* name()
    {
        return "gt";
    }

    static bool eval_double(double x, double y)   
    { 
        return x > y; 
    };

    static bool eval_gsli(const gsli_double& x, const gsli_double& y)   
    { 
        return x > y; 
    };
};

void test_gsli_perf::test_cons_small(int max)
{
    using sli   = sli::gsli_double;
    double res  = 0.0;

    tic();

    for (int i = 1; i < max; ++i)
    {
        double val  = (double)i;

        sli x       = sli(val);
        double x_v  = x.get_value();

        res         += ::abs(val - x_v);
    };

    double t        = toc();

    std::cout << "\n";
    std::cout << "level 0 cons/get_value: " << t << " " << res << "\n";   
};

void test_gsli_perf::test_cons_large(int n_vec, int n_rep)
{
    using sli   = sli::gsli_double;

    std::vector<double> vec_double;
    rand_double_all(vec_double, n_vec);

    double res  = 0.0;

    tic();

    for (int j = 0; j < n_rep; ++j)
    {
        for (int i = 0; i < n_vec; ++i)
        {
            double val  = vec_double[i];

            sli x       = sli(val);
            double x_v  = x.get_value();

            double err  = ::abs(val - x_v);;
            if (val != 0.0)
                err     = err / std::abs(val);

            res         += err;

        };
    };

    res             = res / n_rep / n_vec;
    double t        = toc();

    std::cout << "\n";
    std::cout << "cons/get_value: " << t << " " << res << "\n";   
};

void test_gsli_perf::test_inv_small(int n_vec, int n_rep)
{
    double t1           = 0.0;
    double t2           = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_double_1;
        rand_double_all_0(vec_double_1, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_double_1.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = 1.0/ptr_1[j];
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_double_1;
        rand_gsli_all_0(vec_double_1, n_vec);

        const gsli_double * ptr_1 = vec_double_1.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = inv(ptr_1[j]);
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing inv small" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::test_log_small(int n_vec, int n_rep)
{
    double t1           = 0.0;
    double t2           = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_double_1;
        rand_double_all_0(vec_double_1, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_double_1.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = std::log(std::abs(ptr_1[j]));
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_double_1;
        rand_gsli_all_0(vec_double_1, n_vec);

        const gsli_double * ptr_1 = vec_double_1.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = log_abs(ptr_1[j]);
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing log small" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::test_exp_small(int n_vec, int n_rep)
{
    double t1           = 0.0;
    double t2           = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_double_1;
        rand_double_exp(vec_double_1, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_double_1.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = std::exp(ptr_1[j]);
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_double_1;
        rand_gsli_exp(vec_double_1, n_vec);

        const gsli_double * ptr_1 = vec_double_1.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = exp(ptr_1[j]);
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing exp small" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::test_add_small(int n_vec, int n_rep)
{
    double t1               = 0.0;
    double t2               = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_double_1, vec_double_2;
        rand_double_add(vec_double_1, n_vec);
        rand_double_add(vec_double_2, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_double_1.data();
        const double* ptr_2  = vec_double_2.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = ptr_1[j] + ptr_2[j];
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_double_1, vec_double_2;
        rand_gsli_add(vec_double_1, n_vec);
        rand_gsli_add(vec_double_2, n_vec);

        const gsli_double * ptr_1 = vec_double_1.data();
        const gsli_double* ptr_2  = vec_double_2.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = ptr_1[j] + ptr_2[j];
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing plus small" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::test_sub_small(int n_vec, int n_rep)
{
    double t1               = 0.0;
    double t2               = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_double_1, vec_double_2;
        rand_double_add(vec_double_1, n_vec);
        rand_double_add(vec_double_2, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_double_1.data();
        const double* ptr_2  = vec_double_2.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = ptr_1[j] - ptr_2[j];
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_double_1, vec_double_2;
        rand_gsli_add(vec_double_1, n_vec);
        rand_gsli_add(vec_double_2, n_vec);

        const gsli_double * ptr_1 = vec_double_1.data();
        const gsli_double* ptr_2  = vec_double_2.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = ptr_1[j] - ptr_2[j];
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing minus small" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::test_mult_small(int n_vec, int n_rep)
{
    double t1               = 0.0;
    double t2               = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_double_1, vec_double_2;
        rand_double_mult(vec_double_1, n_vec);
        rand_double_mult(vec_double_2, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_double_1.data();
        const double* ptr_2  = vec_double_2.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = ptr_1[j] * ptr_2[j];
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_double_1, vec_double_2;
        rand_gsli_mult(vec_double_1, n_vec);
        rand_gsli_mult(vec_double_2, n_vec);

        const gsli_double * ptr_1 = vec_double_1.data();
        const gsli_double* ptr_2  = vec_double_2.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = ptr_1[j] * ptr_2[j];
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing mult small" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::test_div_small(int n_vec, int n_rep)
{
    double t1               = 0.0;
    double t2               = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_double_1, vec_double_2;
        rand_double_mult(vec_double_1, n_vec);
        rand_double_mult(vec_double_2, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_double_1.data();
        const double* ptr_2  = vec_double_2.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = ptr_1[j] / ptr_2[j];
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_double_1, vec_double_2;
        rand_gsli_mult(vec_double_1, n_vec);
        rand_gsli_mult(vec_double_2, n_vec);

        const gsli_double * ptr_1 = vec_double_1.data();
        const gsli_double* ptr_2  = vec_double_2.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = ptr_1[j] / ptr_2[j];
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing div small" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::test_pow_small(int n_vec, int n_rep)
{
    double t1               = 0.0;
    double t2               = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_double_1, vec_double_2;
        rand_double_pow1(vec_double_1, n_vec);
        rand_double_pow2(vec_double_2, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_double_1.data();
        const double* ptr_2  = vec_double_2.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = std::pow(std::abs(ptr_1[j]), ptr_2[j]);
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_double_1, vec_double_2;
        rand_gsli_pow1(vec_double_1, n_vec);
        rand_gsli_pow2(vec_double_2, n_vec);

        const gsli_double * ptr_1 = vec_double_1.data();
        const gsli_double* ptr_2  = vec_double_2.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = pow_abs(ptr_1[j], ptr_2[j]);
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing pow small" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::test_pow_int_small(int n_vec, int n_rep)
{
    double t1               = 0.0;
    double t2               = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_1;
        std::vector<int> vec_2;
        rand_double_pow1(vec_1, n_vec);
        rand_double_pow_int2(vec_2, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_1.data();
        const int* ptr_2     = vec_2.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = std::pow(std::abs(ptr_1[j]), ptr_2[j]);
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_1;
        std::vector<int> vec_2;
        rand_gsli_pow1(vec_1, n_vec);
        rand_double_pow_int2(vec_2, n_vec);

        const gsli_double * ptr_1 = vec_1.data();
        const int* ptr_2          = vec_2.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = pow_int(ptr_1[j], ptr_2[j]);
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing pow_int small" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

template<class Func>
void test_gsli_perf::test_cmp_small(int n_vec, int n_rep)
{
    double t1           = 0.0;
    double t2           = 0.0;

    std::vector<int>    res_1;
    std::vector<int>    res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_double_1, vec_double_2;
        rand_double_all_0(vec_double_1, n_vec);
        rand_double_all_0(vec_double_2, n_vec);

        const double * ptr_1 = vec_double_1.data();
        const double* ptr_2  = vec_double_2.data();
        int* ptr_3           = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = Func::eval_double(ptr_1[j], ptr_2[j]);
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_double_1, vec_double_2;
        rand_gsli_all_0(vec_double_1, n_vec);
        rand_gsli_all_0(vec_double_2, n_vec);

        const gsli_double * ptr_1 = vec_double_1.data();
        const gsli_double* ptr_2  = vec_double_2.data();
        int* ptr_3                = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = Func::eval_gsli(ptr_1[j],ptr_2[j]);
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing " << Func::name() << " small" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::rand_double_add(std::vector<double>& vec, int n_elem)
{
    using config        = gsli_double::config_type;
    int min_exp         = -int(config::max_index_exp_0) + int(config::precision + 5);
    int max_exp         = int(config::max_index_exp_0) - int(config::precision + 5);

    for (int i = 0; i < n_elem; ++i)
    {
        double val      = test_gsli::rand_num(min_exp, max_exp);
        vec.push_back(val);
    };
};

void test_gsli_perf::rand_gsli_add(std::vector<gsli_double>& vec, int n_elem)
{
    using config        = gsli_double::config_type;
    int min_exp         = -int(config::max_index_exp_0) + int(config::precision + 5);
    int max_exp         = int(config::max_index_exp_0) - int(config::precision + 5);

    for (int i = 0; i < n_elem; ++i)
    {
        double val      = test_gsli::rand_num(min_exp, max_exp);
        vec.push_back(gsli_double(val));
    };
};

void test_gsli_perf::rand_double_all_0(std::vector<double>& vec, int n_elem)
{
    using config        = gsli_double::config_type;
    int min_exp         = -int(config::max_index_exp_0) + 20;
    int max_exp         = int(config::max_index_exp_0) - 20;

    for (int i = 0; i < n_elem; ++i)
    {
        double val      = test_gsli::rand_num(min_exp, max_exp);
        vec.push_back(val);
    };
};

void test_gsli_perf::rand_double_all(std::vector<double>& vec, int n_elem)
{
    using config        = gsli_double::config_type;
    int min_exp         = int(config::min_exp) + 1;
    int max_exp         = int(config::max_exp) - 1;

    for (int i = 0; i < n_elem; ++i)
    {
        double val      = test_gsli::rand_num(min_exp, max_exp);
        vec.push_back(val);
    };
};

void test_gsli_perf::rand_gsli_all_0(std::vector<gsli_double>& vec, int n_elem)
{
    using config        = gsli_double::config_type;
    int min_exp         = -int(config::max_index_exp_0) + 20;
    int max_exp         = int(config::max_index_exp_0) - 20;

    for (int i = 0; i < n_elem; ++i)
    {
        double val      = test_gsli::rand_num(min_exp, max_exp);
        vec.push_back(gsli_double(val));
    };
};

void test_gsli_perf::rand_gsli_all(std::vector<gsli_double>& vec, int n_elem, int max_lev)
{
    for (int i = 0; i < n_elem; ++i)
    {
        gsli_double val = test_gsli::rand_gsli(false, false, max_lev);
        vec.push_back(val);
    };
};

void test_gsli_perf::rand_double_exp(std::vector<double>& vec, int n_elem)
{
    using config        = gsli_double::config_type;
    // allowed range is -340.3353 to 340.3353
    int min_exp         = -8;
    int max_exp         = 8;

    for (int i = 0; i < n_elem; ++i)
    {
        double val      = test_gsli::rand_num(min_exp, max_exp);
        vec.push_back(val);
    };
};

void test_gsli_perf::rand_gsli_exp(std::vector<gsli_double>& vec, int n_elem)
{
    using config        = gsli_double::config_type;
    // allowed range is -340.3353 to 340.3353
    int min_exp         = -8;
    int max_exp         = 8;

    for (int i = 0; i < n_elem; ++i)
    {
        double val      = test_gsli::rand_num(min_exp, max_exp);
        vec.push_back(gsli_double(val));
    };
};

void test_gsli_perf::rand_gsli_mult(std::vector<gsli_double>& vec, int n_elem)
{
    using config        = gsli_double::config_type;
    int min_exp         = -int(config::max_index_exp_0/2) + 5;
    int max_exp         = int(config::max_index_exp_0/2) - 5;

    for (int i = 0; i < n_elem; ++i)
    {
        double val      = test_gsli::rand_num(min_exp, max_exp);
        vec.push_back(gsli_double(val));
    };
};

void test_gsli_perf::rand_double_mult(std::vector<double>& vec, int n_elem)
{
    using config        = gsli_double::config_type;
    int min_exp         = -int(config::max_index_exp_0/2) + 5;
    int max_exp         = int(config::max_index_exp_0/2) - 5;

    for (int i = 0; i < n_elem; ++i)
    {
        double val      = test_gsli::rand_num(min_exp, max_exp);
        vec.push_back(val);
    };
};

void test_gsli_perf::rand_double_pow2(std::vector<double>& vec, int n_elem)
{
    using config        = gsli_double::config_type;

    int min_exp         = -int(config::max_index_exp_0) + 5;
    int max_exp         = int(std::trunc(std::log2(config::sqrt_log_max_index_0)));

    for (int i = 0; i < n_elem; ++i)
    {
        double val      = test_gsli::rand_num(min_exp, max_exp);
        vec.push_back(val);
    };
};

void test_gsli_perf::rand_double_pow_int2(std::vector<int>& vec, int n_elem)
{
    using config        = gsli_double::config_type;

    int min_val         = 0;
    int max_val         = config::sqrt_log_max_index_0_int;

    for (int i = 0; i < n_elem; ++i)
    {
        int val         = test_gsli::rand_int(min_val, max_val);
        vec.push_back(val);
    };
};

void test_gsli_perf::rand_gsli_pow2(std::vector<gsli_double>& vec, int n_elem)
{
    using config        = gsli_double::config_type;

    int min_exp         = -int(config::max_index_exp_0) + 5;
    int max_exp         = int(std::trunc(std::log2(config::sqrt_log_max_index_0)));

    for (int i = 0; i < n_elem; ++i)
    {
        double val      = test_gsli::rand_num(min_exp, max_exp);
        vec.push_back(val);
    };
};

void test_gsli_perf::rand_double_pow1(std::vector<double>& vec, int n_elem)
{
    using config        = gsli_double::config_type;

    int min_exp         = int(std::trunc(std::log2(config::exp_sqrt_log_max_index_0_inv))) + 1;
    int max_exp         = int(std::trunc(std::log2(config::exp_sqrt_log_max_index_0))) - 1;

    for (int i = 0; i < n_elem; ++i)
    {
        double val      = test_gsli::rand_num(min_exp, max_exp);
        vec.push_back(val);
    };
};

void test_gsli_perf::rand_gsli_pow1(std::vector<gsli_double>& vec, int n_elem)
{
    using config        = gsli_double::config_type;

    int min_exp         = int(std::trunc(std::log2(config::exp_sqrt_log_max_index_0_inv))) + 1;
    int max_exp         = int(std::trunc(std::log2(config::exp_sqrt_log_max_index_0))) - 1;

    for (int i = 0; i < n_elem; ++i)
    {
        double val      = test_gsli::rand_num(min_exp, max_exp);
        vec.push_back(val);
    };
};


void test_gsli_perf::test_add_large(int n_vec, int n_rep, int max_lev)
{
    double t1               = 0.0;
    double t2               = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_double_1, vec_double_2;
        rand_double_all(vec_double_1, n_vec);
        rand_double_all(vec_double_2, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_double_1.data();
        const double* ptr_2  = vec_double_2.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = ptr_1[j] + ptr_2[j];
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_double_1, vec_double_2;
        rand_gsli_all(vec_double_1, n_vec, max_lev);
        rand_gsli_all(vec_double_2, n_vec, max_lev);

        const gsli_double * ptr_1 = vec_double_1.data();
        const gsli_double* ptr_2  = vec_double_2.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = ptr_1[j] + ptr_2[j];
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing plus large" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::test_mult_large(int n_vec, int n_rep, int max_lev)
{
    double t1               = 0.0;
    double t2               = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_double_1, vec_double_2;
        rand_double_all(vec_double_1, n_vec);
        rand_double_all(vec_double_2, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_double_1.data();
        const double* ptr_2  = vec_double_2.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = ptr_1[j] * ptr_2[j];
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_double_1, vec_double_2;
        rand_gsli_all(vec_double_1, n_vec, max_lev);
        rand_gsli_all(vec_double_2, n_vec, max_lev);

        const gsli_double * ptr_1 = vec_double_1.data();
        const gsli_double* ptr_2  = vec_double_2.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = ptr_1[j] * ptr_2[j];
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing mult large" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::test_pow_large(int n_vec, int n_rep, int max_lev)
{
    double t1               = 0.0;
    double t2               = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_double_1, vec_double_2;
        rand_double_all(vec_double_1, n_vec);
        rand_double_all(vec_double_2, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_double_1.data();
        const double* ptr_2  = vec_double_2.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = std::pow(std::abs(ptr_1[j]), ptr_2[j]);
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_double_1, vec_double_2;
        rand_gsli_all(vec_double_1, n_vec, max_lev);
        rand_gsli_all(vec_double_2, n_vec, max_lev);

        const gsli_double * ptr_1 = vec_double_1.data();
        const gsli_double* ptr_2  = vec_double_2.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = pow_abs(ptr_1[j], ptr_2[j]);
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing pow large" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::test_log_large(int n_vec, int n_rep, int max_lev)
{
    double t1               = 0.0;
    double t2               = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_1;
        rand_double_all(vec_1, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_1.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = std::log(std::abs(ptr_1[j]));
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_1;
        rand_gsli_all(vec_1, n_vec, max_lev);

        const gsli_double * ptr_1 = vec_1.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = log_abs(ptr_1[j]);
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing log large" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::test_exp_large(int n_vec, int n_rep, int max_lev)
{
    double t1               = 0.0;
    double t2               = 0.0;

    std::vector<double>      res_1;
    std::vector<gsli_double> res_2;

    res_1.resize(n_vec);
    res_2.resize(n_vec);

    {
        std::vector<double> vec_1;
        rand_double_all(vec_1, n_vec);

        // use raw pointers instead of vector;
        // VS generates strange code, when vectors are used

        const double * ptr_1 = vec_1.data();
        double* ptr_3        = res_1.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = std::exp(std::abs(ptr_1[j]));
        };

        t1          = toc();
    };

    {
        std::vector<gsli_double> vec_1;
        rand_gsli_all(vec_1, n_vec, max_lev);

        const gsli_double * ptr_1 = vec_1.data();
        gsli_double* ptr_3        = res_2.data();

        tic();

        for (int i = 0; i < n_rep; ++i)
        {        
            for (int j = 0; j < n_vec; ++j)
                ptr_3[j]    = exp(ptr_1[j]);
        };

        t2          = toc();
    };

    std::cout << "\n";
    std::cout << "testing exp large" << "\n";
    std::cout << "time double: " << t1 << "; time gsli: " << t2 << "; ratio: " << t2 / t1 << "\n";
    std::cout << "res double: " << res_1[0] << "; res gsli: " << res_2[0] << "\n";
};

void test_gsli_perf::test()
{    
    {
        int n_rep       = 500000000;
        test_cons_small(n_rep);        
    } 
    {
        int n_vec       = 1000000;
        int n_rep       = 10;

        test_cons_large(n_vec, n_rep);
        test_add_large(n_vec, n_rep, 2);
        test_mult_large(n_vec, n_rep, 3);
        test_exp_large(n_vec, n_rep, GSLI_MAX_LEVEL_DOUBLE);
        test_log_large(n_vec, n_rep, GSLI_MAX_LEVEL_DOUBLE);
        test_pow_large(n_vec, n_rep, GSLI_MAX_LEVEL_DOUBLE);
    };    

    {
        int n_vec       = 30000000;
        int n_rep_add   = 1;
        
        test_inv_small(n_vec, n_rep_add);
        test_log_small(n_vec, n_rep_add);
        test_exp_small(n_vec, n_rep_add);
        
        test_add_small(n_vec, n_rep_add);
        test_sub_small(n_vec, n_rep_add);
        test_mult_small(n_vec, n_rep_add);
        test_div_small(n_vec, n_rep_add);

        test_cmp_small<eq_func>(n_vec, n_rep_add);
        test_cmp_small<neq_func>(n_vec, n_rep_add);
        test_cmp_small<leq_func>(n_vec, n_rep_add);
        test_cmp_small<geq_func>(n_vec, n_rep_add);
        test_cmp_small<lt_func>(n_vec, n_rep_add);
        test_cmp_small<gt_func>(n_vec, n_rep_add);
    };
    {
        int n_vec       = 1000000;
        int n_rep_add   = 10;
        test_pow_small(n_vec, n_rep_add);
        test_pow_int_small(n_vec, n_rep_add);
    }
};

}};