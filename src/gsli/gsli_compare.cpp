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

#include "gsli/details/gsli_functions_rep.h"
#include "gsli/gsli_error.h"

namespace sli
{

bool sli::cmp_eq(const gsli_rep& v1, const gsli_rep& v2)
{
    if (v1.level() != v2.level())
        return false;

    if (v1.sign_index() != v2.sign_index())
        return false;

    if (v1.index() != v2.index())
        return false;

    return true;
};

bool sli::cmp_neq(const gsli_rep& v1, const gsli_rep& v2)
{
    return !cmp_eq(v1, v2);
};

bool sli::cmp_gt(const gsli_rep& rep_p, const gsli_rep& rep_q)
{
	if (rep_p.level() == 0 && rep_q.level() == 0)
		return rep_p.signed_index() > rep_q.signed_index();

    if (rep_p.is_regular() == false || rep_q.is_regular() == false)
    {
        if (rep_p.is_nan() == true || rep_q.is_nan() == true)
            return false;

        if (rep_p.is_zero() == true)
        {
            if (rep_q.is_zero() == true)
                return false;
            else
                return rep_q.sign_index() == true;
        };

        if (rep_q.is_zero() == true)
            return rep_p.sign_index() == false;
    };

    // check signs
    if (rep_p.sign_index() == true && rep_q.sign_index() == false)
        return false;

    if (rep_p.sign_index() == false && rep_q.sign_index() == true)
        return true;

    int lev_1               = rep_p.level();
    int lev_2               = rep_q.level();

    bool sign_lev_1         = lev_1 < 0;
    bool sign_lev_2         = lev_2 < 0;

	if (lev_1 == lev_2)
    {
        if (sign_lev_1 != sign_lev_2)
        {
            if (sign_lev_1 == false)
                return rep_p.sign_index() == false;
            else
                return rep_p.sign_index() == true;
        };

        if (sign_lev_2 == true)
        {
            if (rep_p.sign_index() == true)
                return rep_p.index() > rep_q.index();
            else
                return rep_p.index() < rep_q.index();
        }
        else
        {
            if (rep_p.sign_index() == true)
                return rep_p.index() < rep_q.index();
            else
                return rep_p.index() > rep_q.index();
        };
    }
	else
    {
        if (rep_p.sign_index() == true)
		    return lev_1 < lev_2;
        else
            return lev_1 > lev_2;
    }
}

bool sli::cmp_lt(const gsli_rep& rep_p, const gsli_rep& rep_q)
{
	if (rep_p.level() == 0 && rep_q.level() == 0)
		return rep_p.signed_index() < rep_q.signed_index();

    if (rep_p.is_regular() == false || rep_q.is_regular() == false)
    {
        if (rep_p.is_nan() == true || rep_q.is_nan() == true)
            return false;

        if (rep_p.is_zero() == true)
        {
            if (rep_q.is_zero() == true)
                return false;
            else
                return rep_q.sign_index() == false;
        };

        if (rep_q.is_zero() == true)
            return rep_p.sign_index() == true;
    };

    // check signs
    if (rep_p.sign_index() == true && rep_q.sign_index() == false)
        return true;

    if (rep_p.sign_index() == false && rep_q.sign_index() == true)
        return false;

    int lev_1               = rep_p.level();
    int lev_2               = rep_q.level();

    bool sign_lev_1         = lev_1 < 0;
    bool sign_lev_2         = lev_2 < 0;

	if (lev_1 == lev_2)
    {
        if (sign_lev_1 != sign_lev_2)
        {
            if (sign_lev_1 == false)
                return rep_p.sign_index() == true;
            else
                return rep_p.sign_index() == false;
        };

        if (sign_lev_2 == true)
        {
            if (rep_p.sign_index() == true)
                return rep_p.index() < rep_q.index();
            else
                return rep_p.index() > rep_q.index();
        }
        else
        {
            if (rep_p.sign_index() == true)
                return rep_p.index() > rep_q.index();
            else
                return rep_p.index() < rep_q.index();
        };
    }
	else
    {
        if (rep_p.sign_index() == true)
		    return lev_1 > lev_2;
        else
            return lev_1 < lev_2;
    }
}

bool sli::cmp_leq(const gsli_rep& rep_p, const gsli_rep& rep_q)
{
	if (rep_p.level() == 0 && rep_q.level() == 0)
		return rep_p.signed_index() <= rep_q.signed_index();

    if (rep_p.is_regular() == false || rep_q.is_regular() == false)
    {
        if (rep_p.is_nan() == true || rep_q.is_nan() == true)
            return false;

        if (rep_p.is_zero() == true)
        {
            if (rep_q.is_zero() == true)
                return true;
            else
                return rep_q.sign_index() == false;
        };

        if (rep_q.is_zero() == true)
            return rep_p.sign_index() == true;
    };

    // check signs
    if (rep_p.sign_index() == true && rep_q.sign_index() == false)
        return true;

    if (rep_p.sign_index() == false && rep_q.sign_index() == true)
        return false;

    int lev_1               = rep_p.level();
    int lev_2               = rep_q.level();

    bool sign_lev_1         = lev_1 < 0;
    bool sign_lev_2         = lev_2 < 0;

	if (lev_1 == lev_2)
    {
        if (sign_lev_1 != sign_lev_2)
        {
            if (sign_lev_1 == false)
                return rep_p.sign_index() == true;
            else
                return rep_p.sign_index() == false;
        };

        if (sign_lev_2 == true)
        {
            if (rep_p.sign_index() == true)
                return rep_p.index() <= rep_q.index();
            else
                return rep_p.index() >= rep_q.index();
        }
        else
        {
            if (rep_p.sign_index() == true)
                return rep_p.index() >= rep_q.index();
            else
                return rep_p.index() <= rep_q.index();
        };
    }
	else
    {
        if (rep_p.sign_index() == true)
		    return lev_1 >= lev_2;
        else
            return lev_1 <= lev_2;
    }
}

bool sli::cmp_geq(const gsli_rep& rep_p, const gsli_rep& rep_q)
{
	if (rep_p.level() == 0 && rep_q.level() == 0)
		return rep_p.signed_index() >= rep_q.signed_index();

    if (rep_p.is_regular() == false || rep_q.is_regular() == false)
    {
        if (rep_p.is_nan() == true || rep_q.is_nan() == true)
            return false;

        if (rep_p.is_zero() == true)
        {
            if (rep_q.is_zero() == true)
                return true;
            else
                return rep_q.sign_index() == true;
        };

        if (rep_q.is_zero() == true)
            return rep_p.sign_index() == false;
    };

    // check signs
    if (rep_p.sign_index() == true && rep_q.sign_index() == false)
        return false;

    if (rep_p.sign_index() == false && rep_q.sign_index() == true)
        return true;

    int lev_1               = rep_p.level();
    int lev_2               = rep_q.level();

    bool sign_lev_1         = lev_1 < 0;
    bool sign_lev_2         = lev_2 < 0;

	if (lev_1 == lev_2)
    {
        if (sign_lev_1 != sign_lev_2)
        {
            if (sign_lev_1 == false)
                return rep_p.sign_index() == false;
            else
                return rep_p.sign_index() == true;
        };

        if (sign_lev_2 == true)
        {
            if (rep_p.sign_index() == true)
                return rep_p.index() >= rep_q.index();
            else
                return rep_p.index() <= rep_q.index();
        }
        else
        {
            if (rep_p.sign_index() == true)
                return rep_p.index() <= rep_q.index();
            else
                return rep_p.index() >= rep_q.index();
        };
    }
	else
    {
        if (rep_p.sign_index() == true)
		    return lev_1 <= lev_2;
        else
            return lev_1 >= lev_2;
    }
}

};