/// Wake up on a ComparatorA interrupt
/// I test it with a Launchpad, jumper wires and a potentiometer.
/// The connection is flimsy and it basically blinks the red LED chaotically.
/// Sometimes, when the connection is better, it is stable.

#include "msp430chip/controllers.hpp"

namespace board = launchpad_boards::MSP_EXP430G2;
namespace device = board::controller;

// the comparator interrupt vector
#pragma vector=COMPARATORA_VECTOR
__interrupt void COMPA_ISR (void)
{
    device::Port1::p_out::toggle<board::LED_RED>();
}

int main(void)
{
    // Stop watchdog timer
    device::WatchdogTimer::Control::write(WDTPW + WDTHOLD);

    BCSCTL1 = CALBC1_1MHZ;  // Set range   DCOCTL = CALDCO_1MHZ;
    BCSCTL2 &= ~(DIVS_3);   // SMCLK = DCO = 1MHz

    // initialize all ports to IO output - unused too
    device::Port1::p_sel::write<0x00>(); // all I/O function
    device::Port1::p_sel2::write<0x00>(); // all no advanced peripheral functions
    device::Port1::p_dir::write<0xFF>(); // all output

    { // unused ports
        // set not connected pins to primary peripheral mode
        // (a way this works, setting them to I/O hangs the chip)
        device::Port2::p_sel::write<0xF0 - 0x30>(); // 6 pins 0-5 are on the package
        device::Port2::p_sel2::write<0x00>();
        device::Port2::p_dir::write<0x3F>();

        device::Port3::p_sel::write<0xFF>();
        device::Port3::p_sel2::write<0x00>();
    }

    { // configure comparator A

      {
        using ctr1 = device::ComparatorA::Control1;
        ctr1::write<
          //ctr1::InternalReference::set(ctr1::InternalReference::REF_0p5Vcc)
          ctr1::InternalReference::set(ctr1::InternalReference::REF_0p5Vcc)
          //ctr1::InternalReference::set(ctr1::InternalReference::OFF)
          | ctr1::InternalReferenceEnable::set(1)
          | ctr1::InterruptEnable::set(1)
          | ctr1::Enable::set(1)
          | ctr1::ExchangeInputs::set(1) // to use the other bits in ctr2
        >();
      }

      {
        using ctr2 = device::ComparatorA::Control2;
        ctr2::write<
          // now, set CAEX exchange bit and select the positive terminal with the other 2 bits here
          // CA3:
          ctr2::TerminalMultiplexer::set(0b11 << 1)
          // enable the RC filter, to make it a bit more stable
          // (it did not really help with the breadboard wires
          | ctr2::EnableOutputFilter::set(1)
        >();
      }

      // still not sure whether Port Disable is needed or no
      //using capd = device::ComparatorA::PortDisable;
      //capd::write<
      //  1 << 3 // the pin I randomly use
      //>();
    }

    __enable_interrupt();  // Enable interrupts.
    _BIS_SR(LPM3_bits + GIE);
}
