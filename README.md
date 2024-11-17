# msp430_cpp

A bunch of C++ function templates, some namespaces and classes to work with `msp430.h` and `msp430g2102.h`.
There are simple templates to deal with register bit masks,
and C++ wrappers around the Ti header declarations.

Currently, it's just a bunch of tests with C++ for embedded programming,
dumped in `reg_funcs.hpp`.
It it turns out to be handy, I'll separate things into 3 parts:
general logic & register & bit field handling,
msp430 chips,
and the launchpad board.

This code profits from compile-time features, i.e. at least C++17 would be nice to have.
But the Ti compiler `cl430` currently supports only C++14.
I try to keep the code to minimum.
So, if something is not easy to express well purely in C++,
I will generate the code, most likely with Python.
I.e. the goal is to have a set of C++ idioms that cover the programming of msp430 well.
But the user of the library does not care from where came the code for these idioms:
from C++ compile-time (aka metaprogramming) or it is generated as "hardcoded" by a script.
It would be nice to have some programmable description of the chip and the board ("datasheet API")
as the input to the code-generating program.

# To compile in C++

A proper project template is a TODO.

I just modified an example C project in Ti Composer Studio Theia:
in `Debug/makefile` all `.c` files to `.cpp`.
The Ti compiler `cl430` does have an option `--c++14` for C++.
But it is used automatically for `.cpp` endings.

Build in `Debug/` by simply calling `make`.

And program the chip (MSP430G2553 in my case) with [`mspdebug`](https://github.com/dlbeer/mspdebug):

    mspdebug rf2500 "prog msp430g2xx2_1_vlo.cpp.out"
    
I run the Code Composer Studio Theia on Ubuntu Linux,
and it does not recognize the connected board.
But `mspdebug` works fine.
