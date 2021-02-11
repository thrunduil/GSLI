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

#include "test_gsli.h"

#include "timer.h"
#include "rand.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <iomanip>

namespace sli { namespace testing
{

using namespace sli;

double eps(double val)
{
    static const double inf = std::numeric_limits<double>::infinity();
    return std::nextafter(val, inf) - val;
};

gsli_double eps(const gsli_double& v)
{
    gsli_rep rep;
    v.get_gsli_rep(rep);

    gsli_rep rep_next   = nextabove(rep);
    gsli_rep dif        = minus(rep_next, rep);
    
    return gsli_double(dif);
};

struct func_exp
{
    static std::string name()
    { 
        return "exp"; 
    };
    
    static void rand(double& v1)
    { 
        return test_gsli::rand_exp(v1); 
    };

    static double eval_double(double v1)
    {
        return std::exp(v1);
    }

    static gsli_double eval_gsli(const gsli_double& v1)
    {
        return exp(v1);
    }

    static double eval_prec(double v1, double res)
    {
        (void)v1;
        return eps(res);
    }
};

struct func_log
{
    static std::string name()
    { 
        return "log"; 
    };
    
    static void rand(double& v1)
    { 
        return test_gsli::rand_unary(v1); 
    };

    static double eval_double(double v1)
    {
        return std::log(std::abs(v1));
    }

    static gsli_double eval_gsli(const gsli_double& v1)
    {
        return log_abs(v1);
    }

    static double eval_prec(double v1, double res)
    {
        (void)v1;
        return eps(res);
    }
};

struct func_abs
{
    static std::string name()
    { 
        return "abs"; 
    };
    
    static void rand(double& v1)
    { 
        return test_gsli::rand_unary(v1); 
    };

    static double eval_double(double v1)
    {
        return std::abs(v1);
    }

    static gsli_double eval_gsli(const gsli_double& v1)
    {
        return abs(v1);
    }

    static double eval_prec(double v1, double res)
    {
        (void)v1;
        return eps(res);
    }
};

struct func_inv
{
    static std::string name()
    { 
        return "inv"; 
    };
    
    static void rand(double& v1)
    { 
        return test_gsli::rand_unary(v1); 
    };

    static double eval_double(double v1)
    {
        return 1.0/v1;
    }

    static gsli_double eval_gsli(const gsli_double& v1)
    {
        return inv(v1);
    }

    static double eval_prec(double v1, double res)
    {
        (void)v1;
        return eps(res);
    }
};

struct func_uminus
{
    static std::string name()
    { 
        return "uminus"; 
    };
    
    static void rand(double& v1)
    { 
        return test_gsli::rand_unary(v1); 
    };

    static double eval_double(double v1)
    {
        return -v1;
    }

    static gsli_double eval_gsli(const gsli_double& v1)
    {
        return -v1;
    }

    static double eval_prec(double v1, double res)
    {
        (void)v1;
        return eps(res);
    }
};

struct func_cmp_eq
{
    static std::string name()
    { 
        return "eq"; 
    };
    
    static void rand(double& v1, double& v2)
    { 
        return test_gsli::rand_binary(v1, v2); 
    };

    static bool eval_double(double v1, double v2)
    {
        return v1 == v2;
    }

    static bool eval_gsli(const gsli_double& v1, const gsli_double& v2)
    {
        return v1 == v2;
    }
    static bool eval_mixed(const gsli_double& v1, double v2)
    {
        return v1 == v2;
    }
    static bool eval_mixed(double v1, const gsli_double& v2)
    {
        return v1 == v2;
    }
};

struct func_cmp_neq
{
    static std::string name()
    { 
        return "neq"; 
    };
    
    static void rand(double& v1, double& v2)
    { 
        return test_gsli::rand_binary(v1, v2); 
    };

    static bool eval_double(double v1, double v2)
    {
        return v1 != v2;
    }

    static bool eval_gsli(const gsli_double& v1, const gsli_double& v2)
    {
        return v1 != v2;
    }
    static bool eval_mixed(const gsli_double& v1, double v2)
    {
        return v1 != v2;
    }
    static bool eval_mixed(double v1, const gsli_double& v2)
    {
        return v1 != v2;
    }
};

struct func_cmp_leq
{
    static std::string name()
    { 
        return "leq"; 
    };
    
    static void rand(double& v1, double& v2)
    { 
        return test_gsli::rand_binary(v1, v2); 
    };

    static bool eval_double(double v1, double v2)
    {
        return v1 <= v2;
    }

    static bool eval_gsli(const gsli_double& v1, const gsli_double& v2)
    {
        return v1 <= v2;
    }
    static bool eval_mixed(const gsli_double& v1, double v2)
    {
        return v1 <= v2;
    }
    static bool eval_mixed(double v1, const gsli_double& v2)
    {
        return v1 <= v2;
    }
};

struct func_cmp_geq
{
    static std::string name()
    { 
        return "geq"; 
    };
    
    static void rand(double& v1, double& v2)
    { 
        return test_gsli::rand_binary(v1, v2); 
    };

    static bool eval_double(double v1, double v2)
    {
        return v1 >= v2;
    }

    static bool eval_gsli(const gsli_double& v1, const gsli_double& v2)
    {
        return v1 >= v2;
    }
    static bool eval_mixed(const gsli_double& v1, double v2)
    {
        return v1 >= v2;
    }
    static bool eval_mixed(double v1, const gsli_double& v2)
    {
        return v1 >= v2;
    }
};

struct func_cmp_lt
{
    static std::string name()
    { 
        return "lt"; 
    };
    
    static void rand(double& v1, double& v2)
    { 
        return test_gsli::rand_binary(v1, v2); 
    };

    static bool eval_double(double v1, double v2)
    {
        return v1 < v2;
    }

    static bool eval_gsli(const gsli_double& v1, const gsli_double& v2)
    {
        return v1 < v2;
    }
    static bool eval_mixed(const gsli_double& v1, double v2)
    {
        return v1 < v2;
    }
    static bool eval_mixed(double v1, const gsli_double& v2)
    {
        return v1 < v2;
    }
};

struct func_cmp_gt
{
    static std::string name()
    { 
        return "gt"; 
    };
    
    static void rand(double& v1, double& v2)
    { 
        return test_gsli::rand_binary(v1, v2); 
    };

    static bool eval_double(double v1, double v2)
    {
        return v1 > v2;
    }

    static bool eval_gsli(const gsli_double& v1, const gsli_double& v2)
    {
        return v1 > v2;
    }
    static bool eval_mixed(const gsli_double& v1, double v2)
    {
        return v1 > v2;
    }
    static bool eval_mixed(double v1, const gsli_double& v2)
    {
        return v1 > v2;
    }
};

struct func_div
{
    static std::string name()
    { 
        return "div"; 
    };
    
    static void rand(double& v1, double& v2)
    { 
        return test_gsli::rand_binary(v1, v2); 
    };

    static double eval_double(double v1, double v2)
    {
        return v1/v2;
    }

    static gsli_double eval_gsli(const gsli_double& v1, const gsli_double& v2)
    {
        return v1/v2;
    }
    static gsli_double eval_mixed(const gsli_double& v1, double v2)
    {
        return v1/v2;
    }
    static gsli_double eval_mixed(double v1, const gsli_double& v2)
    {
        return v1/v2;
    }

    static double eval_prec(double v1, double v2, double res)
    {
        (void)v1;
        (void)v2;
        return eps(res);
    }
};

struct func_mult
{
    static std::string name()
    { 
        return "mult"; 
    };
    
    static void rand(double& v1, double& v2)
    { 
        return test_gsli::rand_binary(v1, v2); 
    };

    static double eval_double(double v1, double v2)
    {
        return v1*v2;
    }

    static gsli_double eval_gsli(const gsli_double& v1, const gsli_double& v2)
    {
        return v1*v2;
    }
    static gsli_double eval_mixed(const gsli_double& v1, double v2)
    {
        return v1*v2;
    }
    static gsli_double eval_mixed(double v1, const gsli_double& v2)
    {
        return v1*v2;
    }

    static double eval_prec(double v1, double v2, double res)
    {
        (void)v1;
        (void)v2;
        return eps(res);
    }
};

struct func_pow_real
{
    static std::string name()
    { 
        return "pow_real"; 
    };
    
    static void rand(double& v1, double& v2)
    { 
        return test_gsli::rand_pow_real(v1, v2); 
    };

    static double eval_double(double v1, double v2)
    {
        return std::pow(std::abs(v1),v2);
    }

    static gsli_double eval_gsli(const gsli_double& v1, const gsli_double& v2)
    {
        return pow_abs(v1, v2);
    }
    static gsli_double eval_mixed(const gsli_double& v1, double v2)
    {
        return pow_abs(v1, v2);
    }
    static gsli_double eval_mixed(double v1, const gsli_double& v2)
    {
        return pow_abs(v1, v2);
    }

    static double eval_prec(double v1, double v2, double res)
    {
        (void)v1;
        (void)v2;
        return eps(res);
    }
};

struct func_plus
{
    static std::string name()
    { 
        return "plus"; 
    };
    
    static void rand(double& v1, double& v2)
    { 
        return test_gsli::rand_plus(v1, v2); 
    };

    static double eval_double(double v1, double v2)
    {
        return v1+v2;
    }

    static gsli_double eval_gsli(const gsli_double& v1, const gsli_double& v2)
    {
        return v1+v2;
    }

    static gsli_double eval_mixed(const gsli_double& v1, double v2)
    {
        return v1+v2;
    }
    static gsli_double eval_mixed(double v1, const gsli_double& v2)
    {
        return v1+v2;
    }

    static double eval_prec(double v1, double v2, double res)
    {
        if (res == 0.0)
            return 0.0;

        double err  = std::abs(v1 / res) * eps(v1) + std::abs(v2/res)*eps(v2);
        return err;
    }
};

struct func_minus
{
    static std::string name()
    { 
        return "minus"; 
    };
    
    static void rand(double& v1, double& v2)
    { 
        return test_gsli::rand_plus(v1, v2); 
    };

    static double eval_double(double v1, double v2)
    {
        return v1-v2;
    }

    static gsli_double eval_gsli(const gsli_double& v1, const gsli_double& v2)
    {
        return v1-v2;
    }
    static gsli_double eval_mixed(const gsli_double& v1, double v2)
    {
        return v1-v2;
    }
    static gsli_double eval_mixed(double v1, const gsli_double& v2)
    {
        return v1-v2;
    }

    static double eval_prec(double v1, double v2, double res)
    {
        if (res == 0.0)
            return 0.0;

        double err  = std::abs(v1 / res) * eps(v1) + std::abs(v2/res)*eps(v2);
        return err;
    }
};

template<class Func>
void test_gsli::test_unary(int n_rep)
{
    using sli           = sli::gsli_double;

    std::cout << "\n" << "testing " << Func::name() << ":" << "\n";

    double prec_mean    = 0.0;
    double prec_max     = 0.0;
    bool failed         = false;

    size_t stop_index   = (size_t)-1;

    for (int i = 0; i < n_rep; ++i)
    {
        double v1;   
        Func::rand(v1);

        if (i == (int)stop_index)
        {
            std::cout << v1 << "\n";
        };

        double res1     = Func::eval_double(v1);
        double prec_fun = Func::eval_prec(v1, res1);

        sli x1          = sli(v1);
        sli res         = Func::eval_gsli(x1);
        
        double res2     = res.get_value();
        double prec     = calc_prec(res1, res2, prec_fun);

        prec_mean       += prec;
        prec_max        = std::max(prec_max, prec);

        if (prec > 200)
        {
            failed      = true;

            std::cout   << i << " " << v1 << " " << prec;
            std::cout << "\n";
        };
    };

    prec_mean           = prec_mean / n_rep;

    if (failed == true)
        std::cout << "FAILED";
    else
        std::cout << "OK";

    std::cout << "; mean error: " << prec_mean << "; max error: " << prec_max << "\n";
};

template<class Func>
void test_gsli::test_binary(int n_rep)
{
    using sli           = sli::gsli_double;

    std::cout << "\n" << "testing " << Func::name() << ":" << "\n";

    double prec_mean    = 0.0;
    double prec_max     = 0.0;
    bool failed         = false;

    size_t stop_index   = (size_t)-1;

    for (int i = 0; i < n_rep; ++i)
    {
        double v1, v2;   
        Func::rand(v1, v2);

        if (i == (int)stop_index)
        {
            std::cout << v1 << " " << v2 << "\n";
        };

        double res1     = Func::eval_double(v1, v2);
        double prec_fun = Func::eval_prec(v1, v2, res1);

        sli x1          = sli(v1);
        sli x2          = sli(v2);

        sli res         = Func::eval_gsli(x1, x2);
        sli res_d1      = Func::eval_mixed(x1, v2);
        sli res_d2      = Func::eval_mixed(v1, x2);
        
        double res2     = res.get_value();
        double prec     = calc_prec(res1, res2, prec_fun);

        prec_mean       += prec;
        prec_max        = std::max(prec_max, prec); 

        bool eq         = equal_nan(res, res_d1) && equal_nan(res, res_d2);

        if (prec > 1000 || eq == false)
        {
            failed      = true;

            std::cout   << i << " " << v1 << " " << v2 << " " 
                        << prec;
            std::cout << "\n";
        };
    };

    prec_mean           = prec_mean / n_rep;

    if (failed == true)
        std::cout << "FAILED";
    else
        std::cout << "OK";

    std::cout << "; mean error: " << prec_mean << "; max error: " << prec_max << "\n";
};

void test_gsli::test_pow_int(int n_rep)
{
    using sli           = sli::gsli_double;

    std::cout << "\n" << "testing " << "pow_int" << ":" << "\n";

    double prec_mean    = 0.0;
    double prec_max     = 0.0;
    bool failed         = false;

    size_t stop_index   = (size_t)-1;

    for (int i = 0; i < n_rep; ++i)
    {
        double v1;   
        int v2;
        rand_pow_int(v1, v2);

        if (i == (int)stop_index)
        {
            std::cout << v1 << " " << v2 << "\n";
        };

        double res1     = std::pow(v1, v2);
        double prec_fun = func_mult::eval_prec(v1, v2, res1);

        sli x1          = sli(v1);
        sli res         = pow_int(x1, v2);
        
        double res2     = res.get_value();
        double prec     = calc_prec(res1, res2, prec_fun);

        prec_mean       += prec;
        prec_max        = std::max(prec_max, prec); 

        if (prec > 1000)
        {
            failed      = true;

            std::cout   << i << " " << v1 << " " << v2 << " " 
                        << prec;
            std::cout << "\n";
        };
    };

    prec_mean           = prec_mean / n_rep;

    if (failed == true)
        std::cout << "FAILED";
    else
        std::cout << "OK";

    std::cout << "; mean error: " << prec_mean << "; max error: " << prec_max << "\n";
};

template<class Func>
void test_gsli::test_binary2(int n_rep)
{
    using sli           = sli::gsli_double;

    std::cout << "\n" << "testing " << Func::name() << ":" << "\n";

    double prec_mean    = 0.0;
    bool failed         = false;

    size_t stop_index   = (size_t)-1;

    for (int i = 0; i < n_rep; ++i)
    {
        double v1, v2;   
        Func::rand(v1, v2);

        auto res1       = Func::eval_double(v1, v2);

        if (i == (int)stop_index)
        {
            std::cout << v1 << " " << v2 << "\n";
        };

        sli x1          = sli(v1);
        sli x2          = sli(v2);

        auto res2       = Func::eval_gsli(x1, x2);
        auto res_d1     = Func::eval_mixed(v1, x2);
        auto res_d2     = Func::eval_mixed(x1, v2);

        double prec     = res1 == res2 ? 0.0 : 1.0;
        prec            += res1 == res_d1 ? 0.0 : 1.0;
        prec            += res1 == res_d2 ? 0.0 : 1.0;

        prec_mean       += prec;

        if (prec > 0.0)
        {
            failed      = true;

            std::cout   << i << " " << v1 << " " << v2 << " " 
                        << prec;
            std::cout << "\n";
        };
    };

    prec_mean           = prec_mean / n_rep;

    if (failed == true)
        std::cout << "FAILED";
    else
        std::cout << "OK";

    std::cout << "; mean error: " << prec_mean << "\n";
};

gsli_double test_gsli::simple_mult(const gsli_double& v1, const gsli_double& v2)
{
    gsli_rep r1, r2;
    v1.get_gsli_rep(r1);
    v2.get_gsli_rep(r2);

    if (r1.level() == 0 && r2.level() == 0)
    {
        return v1 * v2;
    }

    size_t lev_1, lev_2;
    bool sign_lev_1, sign_lev_2;

    r1.level_sign(lev_1, sign_lev_1);
    r2.level_sign(lev_2, sign_lev_2);

    if (lev_1 == 1 && lev_2 == 1)
    {
        if (sign_lev_1 != sign_lev_2)
        {
            double res      = (sign_lev_1? -1.0 : 1.0) * r1.index() 
                            + (sign_lev_2? -1.0 : 1.0) * r2.index();
            double res_sig  = r1.sign_index_value() * r2.sign_index_value();
            return exp(gsli_double(res)) * res_sig;
        };
    };

    gsli_double sum = log_abs(v1) + log_abs(v2);

    gsli_double res;

    if (is_inf(sum) == true && is_positive(sum) == true)
        res         = sum;
    else
        res = exp(sum);

    if (is_negative(v1) != is_negative(v2))
        res = - res;

    return res;
};

gsli_double test_gsli::simple_plus(const gsli_double& v1, const gsli_double& v2)
{
    gsli_double a1      = abs(v1);
    gsli_double a2      = abs(v2);
    gsli_double max     = (a1 >= a2) ? a1 : a2;

    //v1 + v2 = max * (v1/max + v2/max)

    if (is_nan(v1) == true || is_nan(v2) == true)
        return gsli_double::make_nan();

    if (is_inf(max) == true)
    {
        gsli_double sum = 0;

        if (is_inf(v1) == true)
            sum         += is_positive(v1) ? 1.0 : -1.0;

        if (is_inf(v2) == true)
            sum         += is_positive(v2) ? 1.0 : -1.0;

        return max * sum;
    }
    else if (is_zero(max) == true)
    {
        return v1.get_value() + v2.get_value();
    }
    else
    {        
        double sum1     = (v1/max).get_value();
        double sum2     = (v2/max).get_value();
        double sum      = sum1 + sum2;
        return max * sum;
    };
};

void test_gsli::test_mult_lev(int n_rep, int max_lev)
{
    using sli           = sli::gsli_double;

    std::cout << "\n" << "testing mult lev > 1" << "\n";

    double prec_mean    = 0.0;
    double prec_max     = 0.0;
    bool failed         = false;

    size_t stop_index   = (size_t)-1;

    for (int i = 0; i < n_rep; ++i)
    {
        gsli_double v1, v2;   
        rand_mult(v1, v2, max_lev);

        if (i == (int)stop_index)
        {
            std::cout << v1 << " " << v2 << "\n";
        };

        gsli_double res2    = simple_mult(v1, v2);


        if (i == (int)stop_index)
        {
            std::cout << res2 << "\n";
        };

        gsli_double res1    = v1 * v2;                
        double prec         = calc_prec(res2, res1);

        prec_mean           += prec;
        prec_max            = std::max(prec_max, prec); 

        if (prec > 5000)
        {
            failed      = true;

            std::cout   << i << " " << v1 << " " << v2 << "\n";
            std::cout   << res1 << " " << res2 << " " << prec;
            std::cout << "\n";
        };
    };

    prec_mean           = prec_mean / n_rep;

    if (failed == true)
        std::cout << "FAILED";
    else
        std::cout << "OK";

    std::cout << "; mean error: " << prec_mean << "; max error: " << prec_max << "\n";
};

void test_gsli::test_plus_lev(int n_rep, int max_lev)
{
    using sli           = sli::gsli_double;

    std::cout << "\n" << "testing plus lev > 1" << "\n";

    double prec_mean    = 0.0;
    double prec_max     = 0.0;
    bool failed         = false;

    size_t stop_index   = (size_t)-1;

    for (int i = 0; i < n_rep; ++i)
    {
        gsli_double v1, v2;   
        rand_plus(v1, v2, max_lev);

        if (i == (int)stop_index)
        {
            std::cout << v1 << " " << v2 << "\n";
        };

        gsli_double res2    = simple_plus(v1, v2);

        if (i == (int)stop_index)
        {
            std::cout << res2 << "\n";
        };

        gsli_double res1    = v1 + v2;                
        double prec         = calc_prec(res2, res1);

        prec_mean           += prec;
        prec_max            = std::max(prec_max, prec); 

        if (prec > 5000)
        {
            failed      = true;

            std::cout   << i << " " << v1 << " " << v2 << "\n";
            std::cout   << res1 << " " << res2 << " " << prec;
            std::cout << "\n";
        };
    };

    prec_mean           = prec_mean / n_rep;

    if (failed == true)
        std::cout << "FAILED";
    else
        std::cout << "OK";

    std::cout << "; mean error: " << prec_mean << "; max error: " << prec_max << "\n";
};

void test_gsli::test_log_exp_lev(int n_rep, int max_lev)
{
    using sli           = sli::gsli_double;

    std::cout << "\n" << "testing log/exp > 1" << "\n";

    double prec_mean    = 0.0;
    double prec_max     = 0.0;
    bool failed         = false;

    size_t stop_index   = (size_t)-1;

    for (int i = 0; i < n_rep; ++i)
    {
        gsli_double v1  = rand_gsli(false, true, max_lev);

        if (i == (int)stop_index)
        {
            std::cout << v1 << "\n";
        };

        gsli_double ex, lg;
        gsli_double v2;

        if (abs(v1) > 1)
        {
            ex          = exp(v1);
            lg          = log_abs(ex);
            v2          = lg;
        }
        else
        {
            lg          = log_abs(v1);
            ex          = exp(lg);            
            v2          = ex;

            if (v1 < 0.0)
                v2      = -v2;
        };

        if (i == (int)stop_index)
        {
            std::cout << ex << " " << lg << "\n";
        };
              
        double prec         = calc_prec(v1, v2);

        prec_mean           += prec;
        prec_max            = std::max(prec_max, prec); 

        if (prec > 5000)
        {
            failed      = true;

            std::cout   << i << " " << v1 << " " << v2 << "\n";
            std::cout   << ex << " " << lg << " " << prec;
            std::cout << "\n";
        };
    };

    prec_mean           = prec_mean / n_rep;

    if (failed == true)
        std::cout << "FAILED";
    else
        std::cout << "OK";

    std::cout << "; mean error: " << prec_mean << "; max error: " << prec_max << "\n";
};

bool test_gsli::check_rep_level(int lev, double ind, bool sign_ind)
{
    using sli = sli::gsli_double;

    sli x       = sli(lev, ind, sign_ind);
    gsli_rep rep;
    x.get_gsli_rep(rep);

    int level_r     = rep.level();
    double index_r  = rep.index();
    bool sign_ind_r = rep.sign_index();

    bool ok     = true;

    if (level_r != lev)
        ok          = false;
    if (index_r != ind)
        ok          = false;
    if (sign_ind_r != sign_ind)
        ok          =  false;

    return ok;
};

#pragma warning(push)
#pragma warning(disable : 4127) //conditional expression is constant

void test_gsli::test_constants()
{
    using config_type = gsli_double::config_type;

    if (std::ldexp(1.0, config_type::max_index_exp_0) != config_type::max_index_0)
        throw std::runtime_error("invalid constant max_index_0");

    if (std::ldexp(1.0, -config_type::max_index_exp_0) != config_type::min_index_0)
        throw std::runtime_error("invalid constant min_index_0");

    if (std::ldexp(1.0, config_type::max_index_exp_1) != config_type::max_index_1)
        throw std::runtime_error("invalid constant max_index_1");

    if (std::ldexp(1.0, -(int)config_type::min_index_exp_1) != config_type::min_index_1)
        throw std::runtime_error("invalid constant min_index_1");

    int max_exp_rep = config_type::max_index_exp_0 + config_type::max_level 
        + (config_type::max_index_exp_1 + config_type::min_index_exp_1)*config_type::max_level;

    if (max_exp_rep > config_type::max_exp)
        throw std::runtime_error("invalid constants; exponent too big");

    if (-(int)max_exp_rep < config_type::min_exp)
        throw std::runtime_error("invalid constants; exponent too big");

    if (-(int)max_exp_rep != config_type::min_exp)
    {
        std::cout << "warning: " "max_index_exp_0" << " can be increased" << "\n";
    }

    double eps  = std::numeric_limits<double>::epsilon();

    if (config_type::min_index_0 > eps)
        throw std::runtime_error("max_index_exp_0 too small, precision is lost");

    if (config_type::min_index_1 > 1)
        throw std::runtime_error("min_index_exp_1 too small, precision is lost");

    double inf  = std::numeric_limits<double>::infinity();
    double dif  = std::nextafter(config_type::max_index_1, inf) - config_type::max_index_1;

    if (std::exp(-dif) > eps)
        throw std::runtime_error("max_index_1 too small; addition is not trivial on level 2");

    {
        double log0 = std::log(config_type::max_index_0);
        double log1 = std::log(config_type::max_index_1);

        double dif0 = log0 - config_type::log_max_index_0;
        double dif1 = log1 - config_type::log_max_index_1;

        double eps0 = std::nextafter(log0, inf) - log0;
        double eps1 = std::nextafter(log1, inf) - log1;

        double err0 = std::abs(dif0) / eps0;
        double err1 = std::abs(dif1) / eps1;

        if (err0 >= 1.0)
            throw std::runtime_error("inaccurate log_max_index_0");

        if (err1 >= 1.0)
            throw std::runtime_error("inaccurate log_max_index_1");

        double exp0 = std::exp(log0 - testing::eps(log0));
        double exp1 = std::exp(log1 - testing::eps(log1));

        double eps2 = std::nextafter(exp0, inf) - exp0;
        double eps3 = std::nextafter(exp1, inf) - exp1;

        double dif2 = exp0 - config_type::max_index_0;
        double dif3 = exp1 - config_type::max_index_1;

        double err2 = dif2 / eps2;
        double err3 = dif3 / eps3;

        // it is required, that exp(log_max_index_0 - eps) <= max_index_0
        // and exp(log_max_index_1 - eps) <= max_index_1

        if (err2 > 0.0)
            throw std::runtime_error("inaccurate exp(log_max_index_0)");

        if (err3 >  0.0)
            throw std::runtime_error("inaccurate exp(log_max_index_1)");
    };

    {
        gsli_double val_inf = gsli_double::make_infinity(false);

        gsli_rep rep;
        val_inf.get_gsli_rep(rep);

        if (rep.is_inf() == false)
            throw std::runtime_error("invalid encoding of INF, possibly invalid GSLI_INF_INDEX");
    }

    {
        gsli_double val_nan = gsli_double::make_nan();

        gsli_rep rep;
        val_nan.get_gsli_rep(rep);

        if (rep.is_nan() == false)
            throw std::runtime_error("invalid encoding of NAN");
    }
    {
        gsli_double val_nan = -gsli_double::make_nan();

        gsli_rep rep;
        val_nan.get_gsli_rep(rep);

        if (rep.is_nan() == false)
            throw std::runtime_error("invalid encoding of -NAN");
    }
    {
        gsli_double val_zero = gsli_double::make_zero();

        gsli_rep rep;
        val_zero.get_gsli_rep(rep);

        if (rep.is_zero() == false)
            throw std::runtime_error("invalid encoding of 0");
    }
    {
        gsli_double val_zero = -gsli_double::make_zero();

        gsli_rep rep;
        val_zero.get_gsli_rep(rep);

        if (rep.is_zero() == false || rep.sign_index() == false)
            throw std::runtime_error("invalid encoding of -0");
    }
};

#pragma warning(pop)

void test_gsli::test_nextafter(bool up)
{
    std::cout << "\n";
    std::cout << "testing " << (up ? "nextabove" : "nextbelow") << "\n";

    std::vector<gsli_double> limit_nubers;
    make_limit_numbers(limit_nubers);

    int first, dir;
    int size    = int(limit_nubers.size());

    if (up == true)
    {
        first   = 0;
        dir     = +1;
    }
    else
    {
        first   = int(limit_nubers.size()) - 1;
        dir     = -1;
    }

    bool error  = false;

    for (int i = first, count = 0; count < size; i += dir, ++count)
    {
        gsli_double num     = limit_nubers[i];
        gsli_double next    = up ? nextabove(num) : nextbelow(num);

        double dist         = float_distance(num, next);

        if (next < num)
            dist            = -dist;

        bool new_error      = false;
        if (up == true && dist != 1 || up == false && dist != -1)
        {
            if (is_nan(num) && is_nan(dist))
            {
                new_error   = false;
            }
            else if (dist == 0 && is_inf(num))
            {
                if (is_positive(num) == true && up == true)
                    new_error   = false;
                else if (is_positive(num) == false && up == false)
                    new_error   = false;
                else
                    new_error   = true;
            }
            else
            {
                new_error   = true; 
            };            
        }

        error   |= new_error;

        if (new_error == true)
            std::cout   << num << " -> " << next << ": " << dist << "\n";
    }

    std::cout << (error ? "FAILED" : "OK") << "\n";
};

void test_gsli::make_limit_numbers(std::vector<gsli_double>& vec)
{
    using config    = gsli_double::config_type;    

    int max_level   = config::max_level;

    vec.push_back(gsli_double::make_infinity(true));

    for (int i = max_level; i > 0; --i)
    {
        gsli_double v1(i, config::max_index_1, true);
        gsli_double v2(i, config::min_index_1r, true);

        vec.push_back(v1);
        vec.push_back(v2);
    };

    vec.push_back(gsli_double(-config::max_index_0));
    vec.push_back(gsli_double(-config::min_index_0));

    for (int i = 1; i <= max_level; ++i)
    {
        gsli_double v1(-i, config::min_index_1r, true);
        gsli_double v2(-i, config::max_index_1, true);

        vec.push_back(v1);
        vec.push_back(v2);
    };

    vec.push_back(-gsli_double::make_zero());
    vec.push_back(gsli_double::make_zero());

    for (int i = max_level; i >= 1; --i)
    {
        gsli_double v1(-i, config::max_index_1, false);
        gsli_double v2(-i, config::min_index_1r, false);

        vec.push_back(v1);
        vec.push_back(v2);        
    };

    vec.push_back(gsli_double(config::min_index_0));
    vec.push_back(gsli_double(config::max_index_0));

    for (int i = 1; i <= max_level; ++i)
    {
        gsli_double v1(i, config::min_index_1r, false);
        gsli_double v2(i, config::max_index_1, false);

        vec.push_back(v1);
        vec.push_back(v2);        
    };

    vec.push_back(gsli_double::make_infinity(false));
    vec.push_back(gsli_double::make_nan());
};

void test_gsli::test_io()
{
    std::cout << "\n";
    std::cout << "testing " << "io" << "\n";

    std::vector<gsli_double> limit_nubers;
    make_limit_numbers(limit_nubers);

    bool error  = false;

    for (gsli_double elem : limit_nubers)
    {
        std::stringstream os;
        os << std::setprecision(17);

        os << elem << " " << elem;

        os.seekg(std::ios_base::beg);
        
        gsli_double v1;
        os >> v1;

        if (v1 != elem && is_nan(elem) == false)
        {
            error   = true;
            std::cout << elem << " " << v1 << "\n";
        };

        gsli_double v2;
        os >> v2;

        if (v2 != elem && is_nan(elem) == false)
        {
            error   = true;
            std::cout << elem << " " << v2 << "\n";
        };
    }

    std::cout << (error ? "FAILED" : "OK") << "\n";
}

double test_gsli::make_pow2_value(int pow, int subvalue)
{
    double val = std::ldexp(1.0, pow);

    static const double E1  = 1.0 + std::numeric_limits<double>::epsilon();
    static const double E2  = 1.5;

    switch(subvalue)
    {
        case 0:     return val;
        case 1:     return val * E1;
        case 2:     return val / E1;
        case 3:     return val * E2;
        case 4:     return val / E2;
        default:    return val;
    };
};

void test_gsli::test_rep()
{   
    using config_type   = gsli_double::config_type;
    using sli           = sli::gsli_double;

    bool ok     = true;

    double E            = 1.0 + std::numeric_limits<double>::epsilon();

    for (size_t lev = 1; lev < config_type::max_level; ++lev)
    for(size_t sign_lev = 0; sign_lev <= 1; ++sign_lev)
    for(size_t sign_ind = 0; sign_ind <= 1; ++sign_ind)
    {
        bool sign_lev2  = sign_lev ? true : false;
        bool sign_ind2  = sign_ind ? true : false;

        int sig_lev = sign_lev2 ? -1 : 1;

        ok          &= check_rep_level(sig_lev, config_type::min_index_1r, sign_ind2);
        ok          &= check_rep_level(sig_lev, config_type::min_index_1r * E, sign_ind2);
        ok          &= check_rep_level(sig_lev, config_type::min_index_1r * 1.5, sign_ind2);
        ok          &= check_rep_level(sig_lev, config_type::min_index_1r * 2.0, sign_ind2);
        ok          &= check_rep_level(sig_lev, config_type::max_index_1, sign_ind2);
        ok          &= check_rep_level(sig_lev, config_type::max_index_1 / E, sign_ind2);
        ok          &= check_rep_level(sig_lev, config_type::max_index_1 / 1.5, sign_ind2);
        ok          &= check_rep_level(sig_lev, config_type::max_index_1 / 2.0, sign_ind2);
    }

    if (ok == false)
        std::cout << "gsli rep error" << "\n";

    int min_pow     = -1074;
    int max_pow     = 1023;

    double sum_prec = 0.0;
    int num_prec    = 0;

    for(int i = min_pow; i <= max_pow; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            double val  = make_pow2_value(i, j);
            sli x       = sli(val);

            double x_d  = x.get_internal_rep();
            double x_v  = x.get_value();
            double prec = (x_v - val)/val;
            prec        = ::abs(prec);

            sum_prec    += prec;
            num_prec    += 1;

            if (prec < 1e-13)
                continue;

            std::cout   << i << " ";
            std::cout   <<  std::scientific << std::setw(11) << std::setprecision(5) << val << " : " 
                        <<  std::scientific << std::setw(11) << std::setprecision(5) << x_d << "; " 
                        <<  std::scientific << std::setw(11) << std::setprecision(5) << x_v << "; "
                        <<  std::scientific << std::setw(11) << std::setprecision(5) << prec
                        << "\n";
        };
    };

    std::cout << "\n";
    std::cout << "mean error: " << sum_prec/num_prec << "\n";
    std::cout << "\n";
};

double test_gsli::rand_num(bool with_denorm, bool with_special)
{
    const int min_pow   = with_denorm ? (-1074 + 1) : (-1022 + 1);
    const int max_pow   = 1023;

    double prob_special = get_prob_special();

    if (with_special == true)
    {
        if (genrand_real1() < prob_special)
            return rand_special();
    };

    return rand_num(min_pow, max_pow);
};

double test_gsli::rand_special()
{
    double base = rand_special_base();

    unsigned ir         = genrand_int32();
    ir                  = ir % 5;

    static const double E1  = 1.0 + std::numeric_limits<double>::epsilon();
    static const double E2  = 1.5;

    switch (ir)
    {
        case 0:     return base;
        case 1:     return base * E1;
        case 2:     return base / E1;
        case 3:     return base * E2;
        case 4:     return base / E2;
        default:    return base;
    }
}

double test_gsli::rand_special_base()
{
    using config_type   = gsli_double::config_type;
    unsigned ir         = genrand_int32();
    ir                  = ir % 19;

    switch(ir)
    {
        case 0:     return 0.0;
        case 1:     return -0.0;
        
        case 2:     return 1.0;
        case 3:     return -1.0;
        
        case 4:     return 2.0;
        case 5:     return -2.0;
        
        case 6:     return 0.5;
        case 7:     return -0.5;

        case 8:     return -std::numeric_limits<double>::infinity();
        case 9:     return std::numeric_limits<double>::infinity();
        case 10:    return std::numeric_limits<double>::quiet_NaN();

        case 11:    return config_type::max_index_0;
        case 12:    return -config_type::max_index_0;
        case 13:    return config_type::min_index_0;
        case 14:    return -config_type::min_index_0;

        case 15:    return config_type::max_index_1;
        case 16:    return -config_type::max_index_1;
        case 17:    return config_type::min_index_1;
        case 18:    return -config_type::min_index_1;

        default:
            return 0.0;
    }
};

double test_gsli::rand_num(int min_pow, int max_pow)
{
    const int num_pow    = max_pow - min_pow + 1;

    // rand power
    unsigned ir     = genrand_int32();
    int pow         = min_pow + (ir % num_pow);

    // number from [0.5, 1];
    double sig      = ::abs(genrand_real1() - 0.5) + 0.5;

    double sign     = 1.0;
    if (genrand_real1() < 0.5)
        sign        = -1.0;

    double val      = sign * std::ldexp(sig, pow);
    return val;
};

int test_gsli::rand_int(int min, int max)
{
    const int num_val   = max - min + 1;

    // rand power
    unsigned ir     = genrand_int32();
    int val         = min + (ir % num_val);

    if (genrand_real1() < 0.5)
        val         = -val;

    return val;
};

bool test_gsli::equal_nan(const gsli_double& x1, const gsli_double& x2)
{
    if (x1 == x2)
        return true;

    if (is_nan(x1) == true && is_nan(x2) == true)
        return true;

    return false;
}

double test_gsli::calc_prec(double exact, double calc, double prec)
{
    if (std::isfinite(exact) == true && std::isfinite(calc) == true)
    {
        if (exact - calc == 0.0)
            return 0.0;

        //|exact - calc| <= eps(exact) + prec
        return std::abs((exact - calc))/(eps(exact) + prec);
    }

    if (exact == calc)
        return 0.0;

    bool nan1   = is_nan(exact);
    bool nan2   = is_nan(calc);
    if (nan1 == true && nan2 == true)
        return 0.0;

    return 1.0e100;
};

double test_gsli::calc_prec(const gsli_double& res1, const gsli_double& res2)
{
    if (is_finite(res1) == true && is_finite(res2) == true)
    {
        if (res1 - res2 == 0.0)
            return 0.0;

        return float_distance(res1, res2);
    }

    if (res1 == res2)
        return 0.0;

    bool nan1   = is_nan(res1);
    bool nan2   = is_nan(res2);
    if (nan1 == true && nan2 == true)
        return 0.0;

    return 1.0e100;
};

void test_gsli::rand_binary(double& res1, double& res2)
{
    res1    = rand_num(false, true);
    res2    = rand_num(false, true);
};

void test_gsli::rand_mult(gsli_double& v1, gsli_double& v2, int max_lev)
{
    v1      = rand_gsli(false, true, max_lev);
    v2      = rand_gsli(false, true, max_lev);
}

void test_gsli::rand_plus(gsli_double& v1, gsli_double& v2, int max_lev)
{
    v1      = rand_gsli(false, true, max_lev);
    v2      = rand_gsli(false, true, max_lev);
}

gsli_double test_gsli::rand_gsli(bool with_denorm, bool with_special, int max_lev)
{
    using config_type   = gsli_double::config_type;

    const int num_lev   = max_lev + 1;

    // rand power
    unsigned ir     = genrand_int32();
    int lev         = 0 + (ir % num_lev);

    if (lev == 0)
    {
        double prob_spec= 0.02;

        if (genrand_real1() < prob_spec)
        {
            int num_cases   = 6;
            unsigned r      = genrand_int32();
            int ver         = 0 + (r % num_cases);

            double eps      = config_type::eps;
            double ind;

            switch(ver)
            {
                case 0:     ind = config_type::min_index_0; break;
                case 1:     ind = config_type::max_index_0; break;
                case 2:     ind = config_type::min_index_0*(1+eps); break;
                case 3:     ind = config_type::max_index_0*(1-eps); break;
                case 4:     ind = config_type::min_index_0*(1+2*eps); break;
                case 5:     ind = config_type::max_index_0*(1-2*eps); break;
                default:    ind = config_type::max_index_0; break;
            }

            return ind;
        };

        return rand_num(with_denorm, with_special);
    }

    double ind;
    double prob_spec= 0.02;

    if (genrand_real1() < prob_spec)
    {
        int num_cases   = 6;
        unsigned r  = genrand_int32();
        int ver     = 0 + (r % num_cases);

        double eps  = config_type::eps;

        switch(ver)
        {
            case 0:     ind = config_type::min_index_1r; break;
            case 1:     ind = config_type::max_index_1; break;
            case 2:     ind = config_type::min_index_1r*(1+eps); break;
            case 3:     ind = config_type::max_index_1*(1-eps); break;
            case 4:     ind = config_type::min_index_1r*(1+2*eps); break;
            case 5:     ind = config_type::max_index_1*(1-2*eps); break;
            default:    ind = config_type::max_index_1; break;
        }
    }
    else
    {
        int min_exp = config_type::min_index_exp_1;
        int max_exp = config_type::max_index_exp_1;
        ind         = rand_num(-min_exp, max_exp);
        ind         = std::abs(ind);
    };

    bool sign_lev   = (genrand_real1() < 0.5);
    bool sign_ind   = (genrand_real1() < 0.5);

    if (ind <= config_type::min_index_1)
        ind     = config_type::min_index_1r;

    return gsli_double(sign_lev ? -lev : lev, ind, sign_ind);
};

void test_gsli::rand_unary(double& res1)
{
    res1    = rand_num(true, true);
};

void test_gsli::rand_exp(double& res1)
{
    double prob_special = get_prob_special();
    if (genrand_real1() < prob_special)
    {
        res1 = rand_special();
        return;
    };

    int min_exp = -53;
    int max_exp = 10;
    res1 = rand_num(min_exp, max_exp);
};

void test_gsli::rand_pow_real(double& res1, double& res2)
{
    double prob_special = get_prob_special();

    double m;
    if (genrand_real1() < prob_special)
        m       = rand_special();
    else if (genrand_real1() < 1e-4)
        m       = rand_num(false, true);
    else
        m       = rand_num(-10, 10);

    res1        = rand_num(false, true);
    res2        = res1 * m;

    if (genrand_real1() < 0.5)
        std::swap(res1, res2);
};

void test_gsli::rand_pow_int(double& res1, int& res2)
{
    res1        = rand_num(false, true);

    unsigned r  = genrand_int32();
    int max_pow = 100;
    res2        = r %(max_pow * 2 + 1);
    res2        = res2 - max_pow;
};

void test_gsli::rand_plus(double& res1, double& res2)
{
    double prob_special = get_prob_special();

    double m;
    if (genrand_real1() < prob_special)
        m = rand_special();
    else if (genrand_real1() < 1e-4)
        m       = rand_num(false, true);
    else
        m       = rand_num(-52, 52);

    res1        = rand_num(false, true);
    res2        = res1 * m;

    if (genrand_real1() < 0.5)
        std::swap(res1, res2);
};

double test_gsli::get_prob_special()
{
    return 0.05;
}

void test_gsli::test()
{   
    {
        gsli_double a   = sli::exp(1.0e23);
        gsli_double b   = sli::exp(1.3e23);
        gsli_double c   = a*b;
        gsli_double d   = log_abs(c);

        std::cout << c << "\n";
        std::cout << c.get_value() << "\n";
        std::cout << d << "\n";
    }
    int n_rep_func  = 10000000;

    //using config_type = config<float, 3>;
    using config_type = gsli_double::config_type;

    std::cout << "representation parameters" << "\n";
    std::cout << "max_value: "        << config_type::max_value << "\n";
    std::cout << "min_value: "        << config_type::min_value << "\n";
    std::cout << "max_exp: "          << config_type::max_exp << "\n";
    std::cout << "max_exp: "          << config_type::min_exp << "\n";
    std::cout << "precision: "        << config_type::precision << "\n";
    std::cout << "eps: "              << config_type::eps << "\n";
    std::cout << "inf: "              << config_type::inf << "\n";
    std::cout << "nan: "              << config_type::nan << "\n";

    std::cout << "max_level: "        << config_type::max_level << "\n";
    std::cout << "min_index_exp_1: "  << config_type::min_index_exp_1 << "\n";
    std::cout << "max_index_exp_1: "  << config_type::max_index_exp_1 << "\n";
    std::cout << "max_index_exp_0: "  << config_type::max_index_exp_0 << "\n";
    std::cout << "max_index_0: "      << config_type::max_index_0 << "\n";
    std::cout << "min_index_0: "      << config_type::min_index_0 << "\n";
    std::cout << "max_index_1: "      << config_type::max_index_1 << "\n";
    std::cout << "min_index_1: "      << config_type::min_index_1 << "\n";
    std::cout << "min_index_1r: "     << config_type::min_index_1r << "\n";
    std::cout << "has_large_level_0: " << config_type::has_large_level_0 << "\n";
    std::cout << "use_decode_table: " << config_type::use_decode_table << "\n";
   
    test_constants();
    test_rep();
    test_nextafter(true);
    test_nextafter(false);
    test_io();

    test_unary<func_abs>(n_rep_func);
    test_unary<func_inv>(n_rep_func);
    test_unary<func_uminus>(n_rep_func);
    test_unary<func_exp>(n_rep_func);
    test_unary<func_log>(n_rep_func);

    test_binary2<func_cmp_eq>(n_rep_func);
    test_binary2<func_cmp_neq>(n_rep_func);
    test_binary2<func_cmp_lt>(n_rep_func);
    test_binary2<func_cmp_gt>(n_rep_func);
    test_binary2<func_cmp_leq>(n_rep_func);
    test_binary2<func_cmp_geq>(n_rep_func);

    test_binary<func_plus>(n_rep_func);
    test_binary<func_minus>(n_rep_func);
    test_binary<func_div>(n_rep_func);   
    test_binary<func_mult>(n_rep_func);

    test_binary<func_pow_real>(n_rep_func);
    test_pow_int(n_rep_func);    

    test_mult_lev(n_rep_func, 3);
    test_plus_lev(n_rep_func, 2);
    test_log_exp_lev(n_rep_func, 3);
};

}};