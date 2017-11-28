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

#include "gsli/details/gsli_functions_rep.h"
#include "gsli/gsli_functions.h"
#include "gsli/gsli_error.h"

namespace sli
{

//----------------------------------------------------------------------
//                  unary functions
//----------------------------------------------------------------------
gsli_double sli::inv(const gsli_rep& rep)
{
	if (rep.level() == 0)
		return gsli_double(1.0 / rep.signed_index());

    if (std::abs(rep.level()) <= gsli_rep::config_type::max_level)
	    return gsli_double(-rep.level(), rep.index(), rep.sign_index());

	if (rep.is_zero() == true)
		return gsli_double(1.0 / rep.signed_index());

    if (rep.is_inf())
        return rep.sign_index() ? -gsli_double::make_zero() : gsli_double::make_zero();
    else
        return gsli_double::make_nan();
}

gsli_rep sli::inv_rep(const gsli_rep& rep)
{
    fptype fp   = rep.fpclassify();

    switch(fp)
    {
        case fptype::level_0:
        {
		    return gsli_rep(1.0 / rep.index(), rep.sign_index());
        }
        case fptype::level_k:
        {
	        return gsli_rep(-rep.level(), rep.index(), rep.sign_index());
        }
        case fptype::zero:
        {
            return gsli_rep::make_inf(rep.sign_index());
        }
        case fptype::inf:
        {
            return gsli_rep::make_zero(rep.sign_index());
        }
        case fptype::nan:
        default:
        {
            return gsli_rep::make_nan();
        }
    };
}

};