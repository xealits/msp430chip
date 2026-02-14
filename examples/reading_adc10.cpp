/// Example of reading ADC10 on MSP430G2553.
/// Single channel and single conversion.
/// Binary size: 184B .text and 214B total.

#include "msp430chip/controllers.hpp"

namespace board = launchpad_boards::MSP_EXP430G2;
namespace device = board::controller;

void ConfigureAdc(void) {
    using adc = device::ADC10;

    // first, one of examples sets to turn on the ADC core first?
    // the ON bit is in the same register as Enable Conversion (ENC)
    // and most of control bits can be modified when ENC = 0
    // so, lets turn it on and set ENC = 0
    // but let's also try to test a wrong order when the core is ON only later?
    using ctr0 = adc::Control0;
    ctr0::write<
        ctr0::enable_conversion::set(0)
        | ctr0::interrupt_enable::set(1)
        | ctr0::on_enable::set(1)
        | ctr0::sample_hold_select::set(ctr0::sample_hold_select::SH_x64)
        | ctr0::reference_select::set(ctr0::reference_select::AVCC_AVSS)
    >();

    using ctr1 = adc::Control1;
    ctr1::write<
        ctr1::input_channel::set(ctr1::input_channel::CH_3)
        | ctr1::clock_divider::set(ctr1::clock_divider::DIV_3)
        | ctr1::conversion_sequence::set(ctr1::conversion_sequence::SingleChannelSingleConversion)
        | ctr1::clock_source::set(ctr1::clock_source::ADC10OSC)
        //| ctr1::clock_source::set(ctr1::clock_source::ACLK)
    >();
    // the single-channel-single-conversion is default (bit value 0)
    // and so is the clock source
    // I just keep it here to make it explicit

    // initialize the pin
    // the datasheet SLAS735J says
    // that the ADC function on P1.3 is just set by ADC10AE.x
    // without caring for the digital port SEL setting
    //adc::AnalogEnable0::write(1 << 3);
    adc::AnalogEnable0::write<1 << 3>();
}

// set the interrupt for ADC10 conversion completion
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  	__bic_SR_register_on_exit(CPUOFF);  // Return to active mode
    // TODO check what are "bis" and "bic"
}

int main(void)
{
    // power-efficient interrupt blinking:
    // Stop watchdog timer
    device::WatchdogTimer::Control::write(WDTPW + WDTHOLD);

    // TODO: check what the calibrations do
    BCSCTL1 = CALBC1_1MHZ;			// Set range   DCOCTL = CALDCO_1MHZ;
    BCSCTL2 &= ~(DIVS_3);			// SMCLK = DCO = 1MHz

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

    ConfigureAdc();
    __enable_interrupt();  // Enable interrupts.

    __delay_cycles(1'000);  // Wait for ADC Ref to settle
    // It is not needed in the loop is it?

    unsigned count_cycle{0};
    while (1) {
        using adc = device::ADC10;
        using ctr0 = adc::Control0;

        // start conversion and go to sleep waiting for interrupt
        ctr0::set_or(
            ctr0::enable_conversion::set(1)
            | ctr0::start_conversion::set(1)
        );

        // When the ADC10 runs on ACLK with clock divider /3,
        // getting out of these sleep modes with interrupts takes noticeable time
        // the green LED visibly blinks.
        // Running on the default ADC10OSC with the /3 divider
        // the LED blinks are not visible.
        _BIS_SR(LPM0_bits + GIE); // Enter LPM0 w/ interrupt
        // does it really enter LPM3 with ADC10 core running?
        //_BIS_SR(LPM3_bits + GIE);
        unsigned int ADC_value = adc::Memory::read();

        device::Port1::p_out::write(
            (ADC_value > 512 ? (1 << board::LED_RED) : 0x0)
            |
            (count_cycle & 0x1 == 0x1 ? (1 << board::LED_GREEN) : 0x0)
        );

        count_cycle++;
    }
}
