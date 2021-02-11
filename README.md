# GSLI

C++ implementation of (generalized) Symmetric Level-Index Arithmetic representation
of floating point numbers, that essentially prevents overflow and underflow
in computing. Represented numbers can be as large as 
Ω = 10ˆ(10ˆ(10ˆ(10ˆ2.8e+17))), and as small as 1/Ω.

GSLI library implements basic operations including arithmetic operators (+, -, *, /), comparison operators, 
basic functions (e.g. exp, log, pow). These functions are optimized for speed and accuracy.

The SLI representation of double precision floating point number is encoded directly
in double type. Most of double precision numbers (numbers in range: 2.45e-237 - 4.07e+236) 
are represented exactly, leading to very small performance penalty and maximum possible accuracy, when 
working with numbers in this range. 

All implemented operations for small or moderately large numbers are at most 2 - 3 times slower than
corresponding operations for double type. Operations for large numbers are also reasonably fast, only
one order of magnitude slower than corresponding operations for double type.

## Code example

Simple arithmetic operations:
```cpp
        gsli_double a   = sli::exp(1.0e23);
        gsli_double b   = sli::exp(1.3e23);
        gsli_double c   = a*b;
        gsli_double d   = log_abs(c);

        std::cout << c << "\n";
        std::cout << c.get_value() << "\n";
        std::cout << d << "\n";

        output:
        {2, 14.5898}
        inf
        2.3e+23
```            

## Status

GSLI library is in release candidate stage. However tests were only
performed on Windows using Visual Studio 2015 compiler.

## Licence

This library is published under GPL licence.