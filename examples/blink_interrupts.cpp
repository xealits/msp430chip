/// Example of using msp430chip templates in
/// and interrupt-based LED blinking on MSP_EXP430G2 board
/// with a 20-pin MSP430G2553.
/// Compiled from the most basic busy-loop example from Ti
/// plus their ULP recommendations for the interrupts.

#include "msp430chip/controllers.hpp"

namespace board = launchpad_boards::MSP_EXP430G2;
namespace device = board::controller;

namespace FsmLED {
    constexpr decltype(CCR0) CLOCK_DELAY_TICKS_TIMES[2] = {50, 100};
    unsigned int is_short_tick = 0;

    inline void led_setup(void) {
        device::TimerA_0::CaptureCompareBlock<0>::CapComReg::write(CLOCK_DELAY_TICKS_TIMES[1]);
    }

    inline void led_tick_event(void) {
        if (is_short_tick) {
            device::Port1::p_out::toggle<board::LED_RED>();
        }
        else {
            device::Port1::p_out::toggle<board::LED_GREEN>();
        }

        is_short_tick ^= 1;

        device::TimerA_0::CaptureCompareBlock<0>::CapComReg::write(CLOCK_DELAY_TICKS_TIMES[FsmLED::is_short_tick]);
        device::TimerA_0::CounterRegister::write(0);
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
  // power-efficient interrupt blinking:
  // Stop watchdog timer
  device::WatchdogTimer::Control::write(WDTPW + WDTHOLD);
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

  FsmLED::led_setup();
  {
    using capcom = device::TimerA_0::CaptureCompareBlock<0>::CapComControl;
    capcom::interrupt_enable::write(1);
  }

  //_BIS_SR(LPM0_bits + GIE); // Enter LPM0 w/ interrupt
  // let's enter LPM3
  _BIS_SR(LPM3_bits + GIE);
}
