Templated static constexpr classes that describe devices in the Texas Instruments MSP430 chips and Launchpad boards.
The templates are handy modern C++ wrappers with explicit description of the hardware memory maps.
They do not provide standard procedures of the devices, they just describe the memory.
Standard procedures should come in separate device libraries, preferably the Ti ones.

The templates are generated with `generate_headers.py` from an HTML document (that is generated from Markdown with Pandoc)
that aggregates the info from the User Guide (`slau208q` and `slau144k`),
the device headers like `msp430g2553.h`, and the datasheets.

The goal is to single-source as much as possible and provide comprehensive information for the programmer in
explicit C++ structures and Doxygen comments.
The information should be maintainable. Which is why it is written in standard a Markdown-HTML document and not in C++ directly.
The explicitness of C++ templates makes it easy to generate their instantiations from Markdown-HTML documentation.

* All the basic templates such as `Register`, `BitField`, `IndexedDevPack`
plus some helper functions are in the `bitlogic.hpp` header.
* `devices_templates.hpp` contains `regmaps` namespace contains templates of Texas Instruments devices
that are used in the controllers, like the [`Timer_A`](https://www.ti.com/lit/ug/slau400f/slau400f.pdf).
These are templates for purely compile-time structures,
mostly `struct`s with only `constexpr static` members.
You are not supposed to instantiate an object out of these structs.
You only _specialise the template_ with the correct memory addresses
for the actual memory-mapped devices in the micro-controller,
which are declared in `msp430*.h` headers from Ti.
  + The point of these templates is to describe the hardware as explicitly as possible
  and to be easy to generate from the HTML.
* `controllers.hpp` contains `controllers` namespace contains namespaces for MSP430 controllers, like `MSP430G2553`,
with specialisations of the templates from the `devices` namespace,
according to what is available in the corresponding controllers.
  + At some point, these should also be generated from a Markdown-HTML description.
* `controllers.hpp` also contains definitions for launchpad boards in the `launchpad_boards` namespace:
how the controller pins are wired to something on the board.
(There are no descriptions of chip packages yet.)


# Compilation in Ti Theia-based Code Composer Studio

I use the Ti `cl430` compiler.  The compiler provides only the C++14 standard.

## To compile in C++14

Once you have created a Ti Code Composer Studio project (Theia or previous generation Eclipse),
you can actually just add the `--c++14` option for the `cl430` compiler command
in the `*.out` build target of the generated `Debug/makefile`.

To set `--c++14` compilation flag in Theia:
right-click on the project in the Explorer panel,
Properties, Build, Tools, MSP430 Compiler,
add `--c++14` in the "MSP430 Compiler flags".

You can build in `Debug/` by calling `make` or `make blinky.out` etc.

Program the chip on Linux with [`mspdebug`](https://github.com/dlbeer/mspdebug):

    mspdebug rf2500 "prog ./blinky.out"

I run the Code Composer Studio Theia on Ubuntu Linux,
and it does not recognize the connected board yet.
But `mspdebug` works fine.

## Check assembly

It is worth to check the produced assembly for any unwanted runtime artifacts,
like calls to not inlined functions that write literals into memory mapped devices.

Add in `subdir_rules.mk` to the `main.obj` target:
```
"...ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430"
  --keep_asm --asm_listing ...
```

Add `-O[off01234]` for optimisation levels.
The `cl430` help message says that it defaults to 2, but I don't see it.
Without explicitly setting `-O2`, the binary comes out a couple bytes larger due to not-optimised references.


# Usage examples

The `examples/` directory is getting populated with a couple examples.

There is `blink_interrupts.cpp` with a complete simple program
that blinks the LEDs on the board and goes to sleep following TimerA interrupts.

A basic idea is to program like this:
```cpp
#include "msp430chip/controllers.hpp"

namespace board = launchpad_boards::MSP_EXP430G2;
namespace device = board::controller;

int main(void) {
  // Stop watchdog timer
  device::WatchdogTimer::Control::write(WDTPW + WDTHOLD);
  // initialize all ports to IO output
  device::Port1::p_sel::write<0x00>(); // all I/O function
  device::Port1::p_sel2::write<0x00>(); // all no advanced peripheral functions
  device::Port1::p_dir::write<0xFF>(); // all output

  ...
  device::Port1::p_dir::set<board::LED_RED, board::LED_GREEN>();
  device::Port1::p_out::toggle<board::LED_GREEN>();

  {
    using ctr = device::TimerA_0::Control;
    ctr::write<
      ctr::input_clock::set(ctr::input_clock::INPUT_CLOCK_ACLK) |
      ctr::clock_divider::set(ctr::clock_divider::DIVIDE_0) |
      ctr::mode_control::set(ctr::mode_control::MODE_CONT)
    >();
  }

  ...
}
```

And to have text editors and IDEs prompt the Doxygen comments
for all of those static classes and their members,
which contain useful information from the User Guide and datasheet etc.