A bunch of simple headers with templates of static constexpr classes
that describe some of Texas Instruments MSP430 chips and some Launchpad boards.
The templates should serve as handy modern C++ wrappers
with explicit interfaces to the hardware.

* `devices` namespace contains templates of Texas Instruments devices
that are used in the controllers, like the [`Timer_A`](https://www.ti.com/lit/ug/slau400f/slau400f.pdf).
These are templates for purely compile-time singleton structures,
mostly `struct`s with only `constexpr static` members.
You are not supposed to instantiate an object out of these structs.
You only _specialise the template_ with the correct memory addresses
for the actual devices in the micro-controller,
which are declared in `msp430*.h` headers from Ti.
* `controllers` namespace contains namespaces for MSP430 controllers, like `MSP430G2553`,
with specialisations of the devices in the controllers.
* `launchpad_boards` contains definitions for the launchpad boards:
how the controller pins are wired to something on the board.

There is also the `bitlogic` namespace with utility functions for bit field manipulations.

I use the Ti `cl430` compiler for the domain know how,
on power efficiency and everything else.
But there are limitations on software side:
the compiler provides only C++14 standard (unfortunately not 17),
and it may not optimise C++ code as well as GCC or Clang.
It is worth to check produced assembly whether the compile-time code
does not produce unwanted runtime artifacts, like calls to not inlined functions.

# To compile in C++14

Once you have created a Ti Code Composer Studio project (Theia or previous generation Eclipse),
you can actually just add the `--c++14` option for the `cl430` compiler command
in the `*.out` build target of the generated `Debug/makefile`.

To set `--c++14` compilation flag in Theia:
right-click on the project in the Explorer panel,
Properties, Build, Tools, MSP430 Compiler,
add `--c++14` in the "MSP430 Compiler flags".

You can build in `Debug/` by simply calling `make`.

Program the chip on Linux with [`mspdebug`](https://github.com/dlbeer/mspdebug):

    sudo mspdebug rf2500 "blinky.out"

I run the Code Composer Studio Theia on Ubuntu Linux,
and it does not recognize the connected board yet.
But `mspdebug` works fine.
