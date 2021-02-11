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

#include "gsli/gsli_functions.h"
#include "gsli/gsli_error.h"
#include "gsli/details/gsli_functions_rep.h"

#include <iostream>

namespace sli
{


gsli_double details::inv_general(const gsli_double& v)
{
    gsli_rep rep;
    v.get_gsli_rep(rep);

    return inv(rep);
};

gsli_double details::exp_general(const gsli_double& v)
{
    gsli_rep rep;
    v.get_gsli_rep(rep);

    rep =  exp(rep);
    return gsli_double(rep);
};

gsli_double details::log_general(const gsli_double& v)
{
    gsli_rep rep;
    v.get_gsli_rep(rep);

    rep = log_abs(rep);
    return gsli_double(rep);
};

//
gsli_double details::plus_general(const gsli_double& x1, const gsli_double& x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2;
 
    x1.get_gsli_rep(rep_1);
    x2.get_gsli_rep(rep_2);

    gsli_rep ret = plus(rep_1, rep_2);
    return gsli_double(ret);
};

gsli_double details::plus_general(const gsli_double& x1, double x2)
{
    gsli_rep rep_1;    
    x1.get_gsli_rep(rep_1);

    gsli_rep rep_2(x2, gsli_rep::normalize());

    gsli_rep ret = plus(rep_1, rep_2);
    return gsli_double(ret);
};

gsli_double details::plus_general(double x1, const gsli_double& x2)
{
    gsli_rep rep_2;
    x2.get_gsli_rep(rep_2);

    gsli_rep rep_1(x1, gsli_rep::normalize());

    gsli_rep ret = plus(rep_1, rep_2);
    return gsli_double(ret);
};

//
gsli_double details::mult_general(const gsli_double& x1, const gsli_double& x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2;
 
    x1.get_gsli_rep(rep_1);
    x2.get_gsli_rep(rep_2);

    gsli_rep res = mult(rep_1, rep_2);
    return gsli_double(res);
};

gsli_double details::mult_general(double x1, const gsli_double& x2)
{
    gsli_rep rep_1(x1, gsli_rep::normalize());
    gsli_rep rep_2;
 
    x2.get_gsli_rep(rep_2);

    gsli_rep res = mult(rep_1, rep_2);
    return gsli_double(res);
};

gsli_double details::mult_general(const gsli_double& x1, double x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2(x2, gsli_rep::normalize());
 
    x1.get_gsli_rep(rep_1);

    gsli_rep res = mult(rep_1, rep_2);
    return gsli_double(res);
};

//
gsli_double details::div_general(const gsli_double& x1, const gsli_double& x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2;
 
    x1.get_gsli_rep(rep_1);
    x2.get_gsli_rep(rep_2);

    gsli_rep res = div(rep_1, rep_2);
    return gsli_double(res);
};

gsli_double details::div_general(double x1, const gsli_double& x2)
{
    gsli_rep rep_1(x1, gsli_rep::normalize());;
    gsli_rep rep_2;
 
    x2.get_gsli_rep(rep_2);

    gsli_rep res = div(rep_1, rep_2);
    return gsli_double(res);
};

gsli_double details::div_general(const gsli_double& x1, double x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2(x2, gsli_rep::normalize());
 
    x1.get_gsli_rep(rep_1);

    gsli_rep res = div(rep_1, rep_2);
    return gsli_double(res);
};

//
gsli_double details::pow_abs_general(const gsli_double& x1, const gsli_double& x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2;
 
    x1.get_gsli_rep(rep_1);
    x2.get_gsli_rep(rep_2);

    gsli_rep res = pow_abs(rep_1, rep_2);
    return gsli_double(res);
}

gsli_double details::pow_abs_general(const gsli_double& x1, double x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2(x2, gsli_rep::normalize());
 
    x1.get_gsli_rep(rep_1);

    gsli_rep res = pow_abs(rep_1, rep_2);
    return gsli_double(res);
}

gsli_double details::pow_abs_general(double x1, const gsli_double& x2)
{
    gsli_rep rep_1(x1, gsli_rep::normalize());
    gsli_rep rep_2;
 
    x2.get_gsli_rep(rep_2);

    gsli_rep res = pow_abs(rep_1, rep_2);
    return gsli_double(res);
}

//
gsli_double details::pow_int_general(const gsli_double& x1, long x2)
{
    gsli_rep rep_1;
 
    x1.get_gsli_rep(rep_1);

    gsli_rep res = pow_int(rep_1, x2);
    return gsli_double(res);
};

//
bool details::cmp_gt_general(const gsli_double& x1, const gsli_double& x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2;
 
    x1.get_gsli_rep(rep_1);
    x2.get_gsli_rep(rep_2);

    return cmp_gt(rep_1, rep_2);
};

bool details::cmp_gt_general(const gsli_double& x1, double x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2(x2, gsli_rep::normalize());
 
    x1.get_gsli_rep(rep_1);

    return cmp_gt(rep_1, rep_2);
};

bool details::cmp_gt_general(double x1, const gsli_double& x2)
{
    gsli_rep rep_1(x1, gsli_rep::normalize());;
    gsli_rep rep_2;
 
    x2.get_gsli_rep(rep_2);

    return cmp_gt(rep_1, rep_2);
};

bool details::cmp_lt_general(const gsli_double& x1, const gsli_double& x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2;
 
    x1.get_gsli_rep(rep_1);
    x2.get_gsli_rep(rep_2);

    return cmp_lt(rep_1, rep_2);
};

bool details::cmp_lt_general(const gsli_double& x1, double x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2(x2, gsli_rep::normalize());
 
    x1.get_gsli_rep(rep_1);

    return cmp_lt(rep_1, rep_2);
};

bool details::cmp_lt_general(double x1, const gsli_double& x2)
{
    gsli_rep rep_1(x1, gsli_rep::normalize());;
    gsli_rep rep_2;
 
    x2.get_gsli_rep(rep_2);

    return cmp_lt(rep_1, rep_2);
};

bool details::cmp_geq_general(const gsli_double& x1, const gsli_double& x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2;
 
    x1.get_gsli_rep(rep_1);
    x2.get_gsli_rep(rep_2);

    return cmp_geq(rep_1, rep_2);
};

bool details::cmp_geq_general(const gsli_double& x1, double x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2(x2, gsli_rep::normalize());
 
    x1.get_gsli_rep(rep_1);

    return cmp_geq(rep_1, rep_2);
};

bool details::cmp_geq_general(double x1, const gsli_double& x2)
{
    gsli_rep rep_1(x1, gsli_rep::normalize());;
    gsli_rep rep_2;
 
    x2.get_gsli_rep(rep_2);

    return cmp_geq(rep_1, rep_2);
};

bool details::cmp_leq_general(const gsli_double& x1, const gsli_double& x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2;
 
    x1.get_gsli_rep(rep_1);
    x2.get_gsli_rep(rep_2);

    return cmp_leq(rep_1, rep_2);
};

bool details::cmp_leq_general(const gsli_double& x1, double x2)
{
    gsli_rep rep_1;
    gsli_rep rep_2(x2, gsli_rep::normalize());
 
    x1.get_gsli_rep(rep_1);

    return cmp_leq(rep_1, rep_2);
};

bool details::cmp_leq_general(double x1, const gsli_double& x2)
{
    gsli_rep rep_1(x1, gsli_rep::normalize());;
    gsli_rep rep_2;
 
    x2.get_gsli_rep(rep_2);

    return cmp_leq(rep_1, rep_2);
};

//----------------------------------------------------------------------
//                        error related
//----------------------------------------------------------------------
gsli_double sli::nextabove(const gsli_double& x)
{
    using config = gsli_double::config_type;

    gsli_rep rep;
    x.get_gsli_rep(rep);

    return gsli_double(nextabove(rep));
};

gsli_double sli::nextbelow(const gsli_double& x)
{
    using config = gsli_double::config_type;

    gsli_rep rep;
    x.get_gsli_rep(rep);

    return gsli_double(nextbelow(rep));
};

double sli::float_distance(const gsli_double& x, const gsli_double& y)
{
    using config = gsli_double::config_type;

    gsli_rep rep_x, rep_y;
    x.get_gsli_rep(rep_x);
    y.get_gsli_rep(rep_y);

    return float_distance(rep_x, rep_y);
};

//----------------------------------------------------------------------
//                     input-output
//----------------------------------------------------------------------
static bool read_doube_helper(std::istream &is, double &r)
{
    using config    = gsli_double::config_type;

    char c      = 0;
    bool neg    = false;

    while (is)
    {
        is.get(c);

        if (c != ' ' && c != '\t'  && c != '\n')
            break;
    }

    if (is.eof())
        return false;

    if (c == '-') 
        neg = true; 
    else 
        is.putback(c);

    if (is)
    {
        is.get(c);
        if (is && (c == 'i' || c == 'I'))
        {
            is.get(c);
            
            if (is && (c == 'n' || c == 'N'))
            {
                is.get(c);

                if ((c == 'f') ||(c == 'F'))
                {
                    r = (neg) ? -config::inf : config::inf;
                    return true;
                }
                else 
                    return false;
            }
            else return false;
        }
        if (is && (c == 'n' || c == 'N'))
        {
            is.get(c);
            if (is && (c == 'a' || c == 'A'))
            {
                is.get(c);
                if ((c == 'n' || c == 'N') && !neg)
                {
                    r = config::nan;
                    return true;
                }
                else 
                    return false;
            }
            else
                return false;
        }

        if (c == '-')
            return false; // double -
    }
    else
    {
        return false;
    }

    is.putback(c);
    is >> r;

    if (is.fail() || is.bad())
        return false;
    
    if (neg) 
        r = -r;

    return true;
};

std::istream& sli::operator>>(std::istream& is, gsli_double& v)
{    
    char c  = 0;

    // consume whitespaces
    while (is)
    {
        is.get(c);

        if (c != ' ' && c != '\t'  && c != '\n')
            break;
    }

    if (is.good() == false)
    {
        v = gsli_double::make_nan();
        return is;
    }

    if (c != '{')
    {
        // double value or inf/nan
        is.putback(c);
        double val;

        bool suc    = read_doube_helper(is, val);

        if (suc == true)
            v   = gsli_double(val);
        else
            v   = gsli_double::make_nan();

        return is;
    }

    // level value {lev, val}
    int lev     = 0;
    double val  = 0.0;
    char sep    = ' ';
    char fin    = ' ';

    is >> lev;
    is >> sep;
    is >> val;
    is >> fin;

    if (sep != ',' || fin != '}')
    {
        v   = gsli_double::make_nan();
        return is;
    }

    v   = gsli_double(lev, val);
    return is;
}

std::ostream& sli::operator<<(std::ostream& os, const gsli_double& v)
{
    gsli_rep rep;
    v.get_gsli_rep(rep);

    if (rep.level() == 0)
    {
        os << rep.signed_index();
        return os;
    }

    using config    = gsli_double::config_type;

    if (rep.level() == config::max_level + 1)
    {
        if (rep.is_nan() == true)
        {
            os << "NaN";
        }
        else if (rep.is_inf() == true)
        {
            if (rep.sign_index() == true)
                os << "-Inf";
            else
                os << "Inf";
        };

        return os;
    }
    else if (rep.level() == -config::max_level - 1)
    {
        // this is zero
        os << rep.signed_index();
        return os;
    }
    else
    {
        os << "{" << rep.level() << ", " << rep.signed_index() << "}";
        return os;
    };
};

};
