The examples are made from the Ti Code Composer Studio example projects,
various blog posts and resources on the internet, plus some modifications.

`blink_interrupts.cpp` is compiled from the most basic busy-loop example from Ti
and their ULP recommendations for the interrupts.

`reading_adc10_*.cpp` are examples different modes of reading ADC10,
that are ported from the examples by Ant at [coder-tronics](https://coder-tronics.com/msp430-adc-tutorial/).
It also includes information from:
[xanthium](https://www.xanthium.in/msp430-launchpad-adc10-configuration-tutorial) short well-written tutorial,
[msp430launchpad](https://www.msp430launchpad.com/2010/09/simple-adc-example-on-launchpad.html)
somewhat broad example that also executing commands received from UART,
info on e2e ti support forum, such as
[how to configure the functions of pins for ADC10](https://e2e.ti.com/support/microcontrollers/msp-low-power-microcontrollers-group/msp430/f/msp-low-power-microcontroller-forum/1574072/msp430fr2111-how-do-i-configure-a-port-as-an-analog-input-for-the-adc).
You need to find tables for "Pin functions" in the datasheet for your controller.
(SLAS735J for MSP430G2553, SLASE78E for MSP430FR2111)
Or just search for PSEL and you will probably find the tables.

* I did not figure out how to run their "Repeat-Sequence-of-Channels" mode
in the `reading_adc10_MultipleChannelsRepeatedReads.cpp` example.
* The SLAU144K Guide (section 22.2.6.4 Repeat-Sequence-of-Channels Mode) says
only "The sequence ends after conversion of channel A0, and the next trigger signal re-starts the sequence."
So, it must mean that this "trigger signal" can be configured,
it is not the Sequence Start (ASC10SC) bit?
* How to configure that trigger? Can it come from TimerA?
(Why not just write to the SC bit in the TimerA interrup routine?
What's the advantage to have a different trigger?
It shortens the time to start the conversion?)

