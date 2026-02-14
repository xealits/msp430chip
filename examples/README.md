The examples are made from the Ti Code Composer Studio example projects,
various blog posts and resources on the internet, plus some modifications.

`blink_interrupts.cpp` is compiled from the most basic busy-loop example from Ti
and their ULP recommendations for the interrupts.

`reading_adc10.cpp` is the simplest mode of reading ADC10: single channel and
single conversion.  It is based on:
[coder-tronics](https://coder-tronics.com/msp430-adc-tutorial/) good brief set of 3 examples,
[xanthium](https://www.xanthium.in/msp430-launchpad-adc10-configuration-tutorial) short well-written tutorial,
[msp430launchpad](https://www.msp430launchpad.com/2010/09/simple-adc-example-on-launchpad.html)
somewhat broad example that also executing commands received from UART,
info on e2e ti support forum, such as
[how to configure the functions of pins for ADC10](https://e2e.ti.com/support/microcontrollers/msp-low-power-microcontrollers-group/msp430/f/msp-low-power-microcontroller-forum/1574072/msp430fr2111-how-do-i-configure-a-port-as-an-analog-input-for-the-adc).
You need to find tables for "Pin functions" in the datasheet for your controller.
(SLAS735J for MSP430G2553, SLASE78E for MSP430FR2111)
Or just search for PSEL and you will probably find the tables.
