A bunch of simple headers with templates of static constexpr classes
that describe some of Texas Instruments MSP430 chips and some Launchpad boards.
The templates are handy modern C++ wrappers with explicit interfaces to the hardware.

* `devices` namespace contains templates of Texas Instruments devices
that are used in the controllers, like the [`Timer_A`](https://www.ti.com/lit/ug/slau400f/slau400f.pdf).
These are templates for purely compile-time singleton structures,
mostly `struct`s with only `constexpr static` members.
You are not supposed to instantiate an object out of these structs.
You only _specialise the template_ with the correct memory addresses
for the actual memory-mapped devices in the micro-controller,
which are declared in `msp430*.h` headers from Ti.
* `controllers` namespace contains namespaces for MSP430 controllers, like `MSP430G2553`,
with specialisations of the templates from the `devices` namespace,
according to what is available in the corresponding controllers.
* `launchpad_boards` contains definitions for the launchpad boards:
how the controller pins are wired to something on the board.

There is also the `bitlogic` namespace with utility functions for bit field manipulations.

I use the Ti `cl430` compiler for the electronics know how,
on power efficiency and everything else.
But there are limitations on software side:
the compiler provides only C++14 standard (unfortunately not 17),
and it may not optimise C++ code as well as GCC or Clang.
It is worth to check produced assembly whether the compile-time code
does not contain unwanted runtime artifacts,
like calls to not inlined functions that write literals into memory mapped devices.

Add in `subdir_rules.mk` to the `main.obj` target:
```
"...ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430"
  --keep_asm --asm_listing ...
```

Add `-O[off01234]` for optimisation levels.
The `cl430` help message says that it defaults to 2, but I don't see it. Without explicitly setting `-O2`, the binary comes out a couple bytes larger dues to unnecessary references.

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

# Usage example

An efficient interrupt-based LED blinking:
```
#include "msp430chip/controllers.hpp"

namespace board = launchpad_boards::MSP_EXP430G2;
namespace controller = board::controller;

namespace FsmLED {
    constexpr decltype(CCR0) CLOCK_DELAY_TICKS_TIMES[2] = {50, 100};
    unsigned int is_short_tick = 0;

    inline void led_setup(void) {
        CCR0 = CLOCK_DELAY_TICKS_TIMES[1];
    }

    inline void led_tick_event(void) {
        if (is_short_tick) {
            controller::Port1::toggleOutput<board::LED1>();
        }
        else {
            controller::Port1::toggleOutput<board::LED2>();
        }

        is_short_tick ^= 1;
        CCR0 = CLOCK_DELAY_TICKS_TIMES[FsmLED::is_short_tick];
        TAR = 0;
    }
};

// set the interrupt to toggle the output pin
// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
  FsmLED::led_tick_event();
}

int main(void)
{
    /* busy loop blinking:
    volatile unsigned int i;
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    P1DIR |= 0x01;                            // Set P1.0 to output direction

    while(1)
    {
        P1OUT ^= 0x01;                        // Toggle P1.0 using exclusive-OR

        for (i=10000; i>0; i--);
    }
    */

    // power-efficient interrupt blinking:
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    controller::Port1::setDirecionOutputs<board::LED1, board::LED2>();
    controller::Port1::toggleOutput<board::LED2>();

    controller::TimerA_0::control_write<
        controller::TimerA_0::input_clock::set(controller::TimerA_0::INPUT_CLOCK_ACLK) |
        controller::TimerA_0::clock_divider::set(controller::TimerA_0::DIVIDE_0) |
        controller::TimerA_0::mode_select::set(controller::TimerA_0::MODE_CONT)
    >();

    FsmLED::led_setup();
    controller::TimerA_0::CCTL_interrupt_enable::write(1);

    // let's enter LPM3
    // if the LED state is off, the board will draw really little current
    _BIS_SR(LPM3_bits + GIE);
}
```

The current draw of the `MSP_EXP430G2` Launchpad can be measured on the board jumpers.
From [slau318g](https://www.ti.com/lit/ug/slau318g/slau318g.pdf)
"MSP-EXP430G2 LaunchPad™ Development Kit User's Guide, revised in 2016" section 5.3:
> The jumper array can also be used to measure the power consumption of the LaunchPad development kit
application. For this intention, all connections except VCC must be opened, and a multi meter can used on
the VCC Jumper to measure the current of the MSP-EXP430G2 target device and its peripherals. The
jumper J5 VCC also must be opened if the LaunchPad development kit is powered with an external power
supply over J6 Table 1 or the eZ430 interface J4.

From [slau772a](https://www.ti.com/lit/ug/slau772a/slau772a.pdf)
"MSP430G2553 LaunchPad™ Development Kit (MSP ‑‑EXP430G2ET), revised in 2020"
section "2.4 Measure MSP430 Current Draw":
> To measure the current draw of the MSP430G2553 using a multi-meter, use the 3V3 jumper on the J101
jumper isolation block. The current measured includes the target MCU and any current drawn through the
BoosterPack plug-in module headers.
>
> To measure ultra-low power, follow these steps:
> ...
