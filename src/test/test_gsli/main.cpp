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

#include "timer.h"
#include "test_gsli.h"
#include "test_gsli_perf.h"

#include <iostream>
//#include <vld.h>

int main(int argc, const char* argv[])
{
    using namespace sli::testing;

    (void)argc;
    (void)argv;    

    try
    {
        test_gsli().test();
        test_gsli_perf().test();
    }
    catch(std::exception& ex)
    {
        std::cout << ex.what();
    }
    catch(gsli::assert_exception& ex)
    {
        std::cout << ex.what();
    }    

    return 0;
}
