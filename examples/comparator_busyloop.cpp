/// Wake up on a ComparatorA interrupt
/// based on Ti example:
/// https://dev.ti.com/tirex/explore/node?isTheia=false&node=A__AHzWBP8bZ4AHl41.8b1Ouw__msp430ware__IOGqZri__LATEST

#include "msp430chip/controllers.hpp"

namespace board = launchpad_boards::MSP_EXP430G2;
namespace device = board::controller;

// not yet
//// the comparator interrupt vector
//#pragma vector=COMPARATORA_VECTOR
//__interrupt void COMPA_ISR (void)
//{
//  	__bic_SR_register_on_exit(CPUOFF);  // Return to active mode
//}

int main(void)
{
    // Stop watchdog timer
    device::WatchdogTimer::Control::write(WDTPW + WDTHOLD);

    BCSCTL1 = CALBC1_1MHZ;  // Set range   DCOCTL = CALDCO_1MHZ;
    BCSCTL2 &= ~(DIVS_3);   // SMCLK = DCO = 1MHz

//    // initialize all ports to IO output - unused too
//    device::Port1::p_sel::write<0x00>(); // all I/O function
//    device::Port1::p_sel2::write<0x00>(); // all no advanced peripheral functions
//    device::Port1::p_dir::write<0xFF>(); // all output
//
//    { // unused ports
//        // set not connected pins to primary peripheral mode
//        // (a way this works, setting them to I/O hangs the chip)
//        device::Port2::p_sel::write<0xF0 - 0x30>(); // 6 pins 0-5 are on the package
//        device::Port2::p_sel2::write<0x00>();
//        device::Port2::p_dir::write<0x3F>();
//
//        device::Port3::p_sel::write<0xFF>();
//        device::Port3::p_sel2::write<0x00>();
//    }

    // from the example:
    P1DIR |= (1 << board::LED_RED) | (1 << board::LED_GREEN);

    { // configure comparator A

      {
        using ctr1 = device::ComparatorA::Control1;
        ctr1::write<
          //ctr1::InternalReference::set(ctr1::InternalReference::REF_0p5Vcc)
          ctr1::InternalReference::set(ctr1::InternalReference::REF_0p25Vcc)
          //ctr1::InternalReference::set(ctr1::InternalReference::OFF)
          | ctr1::InternalReferenceEnable::set(1)
          | ctr1::Enable::set(1)
          | ctr1::ExchangeInputs::set(1) // to use the other bits in ctr2
        >();
      }

      {
        using ctr2 = device::ComparatorA::Control2;
        ctr2::write<
          //ctr2::TerminalMultiplexer::set(1 << (5 - 1))
          // terminal multiplexer is weird,
          // that's why the header comments are so weird
          // this selects the last bit in the field P2CA4
          // and together with the 0th bit it makes it 10b = CA1 input
          // and it looks like it automatically enables the comparator function on P1.1?
          // no need for CAPD? CAPD is instructed by the g2553 datasheet "Pin FUnctions"
          // also
          // from the slau144k guide, it looks like 1 bit is just not used in this field?
          // select CA1:
          //ctr2::TerminalMultiplexer::set(1 << (5 - 1))
          // select CA2:
          //ctr2::TerminalMultiplexer::set(1 << (5 - 1) | 0b1)
          // now, set CAEX exchange bit and select the positive terminal with the other 2 bits here
          // CA3:
          ctr2::TerminalMultiplexer::set(0b11 << 1)
        >();
      }

      // the g2553 datasheet "Pin Functions" tables say
      // that you need to set CAPD PortDisable bits to 1
      // to get the P1 port pins into the ComparatorA function
      // but the Guide talks how CAPD disables _bufering_ on the pins
      // not sure what buffering means here
      // and the example does not do anything with the pins
      // let's try
      //using capd = device::ComparatorA::PortDisable;
      //capd::write<
      //  1 << 3 // the pin I randomly use
      //>();
    }

    // not yet
    //__enable_interrupt();  // Enable interrupts.

    unsigned count_cycle{0};
    while (1) {
        __delay_cycles(500'000);  // just a 1s delay to see things

        //_BIS_SR(LPM0_bits + GIE); // Enter LPM0 w/ interrupt
        // does it really enter LPM3 with ADC10 core running?
        //_BIS_SR(LPM3_bits + GIE);

        using ctr2 = device::ComparatorA::Control2;
        bool comp_on = ctr2::ComparatorAOutput::read() > 0;

        device::Port1::p_out::write(
            (comp_on ? (1 << board::LED_RED) : 0x0)
            |
            (count_cycle & 0x1 == 0x1 ? (1 << board::LED_GREEN) : 0x0)
        );

        count_cycle++;
    }
}
