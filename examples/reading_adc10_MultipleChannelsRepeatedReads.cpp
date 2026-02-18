/// Example of reading ADC10 on MSP430G2553.
/// Repeat sequence of channels.
/// This example does not work.
/// I do not see how to run this Reoeated Sequence of Channels mode.
/// How it is different from just Sequence of Channels in the slau144k Guide?
/// Just sequence "can be triggered by the ADC10SC bit" - StartConversion bit.
/// And for the repeated sequence: "the next trigger signal re-starts the sequence".
/// So, I guess, the repeated sequence trigger signal can come from different things?
/// As it is now (with the SC and adc_readings[n_adc_sequences][n_channels]),
/// adc_readings[0][0] is just random. DT does not pick up the right data.

#include "msp430chip/controllers.hpp"

namespace board = launchpad_boards::MSP_EXP430G2;
namespace device = board::controller;

// I am not sure in which order it scans.
// From the Guide, it seems that it scans the channels (from INCHx to A0)
// and then repeats it N times.
//

constexpr unsigned n_channels = 4; // from channel 3 down to 0, right?
constexpr unsigned n_adc_sequences = 10;
unsigned int adc_readings[n_adc_sequences][n_channels] = {0};
constexpr auto n_adc_reeadings = sizeof(adc_readings) / sizeof(adc_readings[0][0]);

void ConfigureAdc(void) {
    using adc = device::ADC10;

    using ctr0 = adc::Control0;
    ctr0::write<
        ctr0::enable_conversion::set(0)
        | ctr0::interrupt_enable::set(1)
        | ctr0::on_enable::set(1)
        | ctr0::sample_hold_select::set(ctr0::sample_hold_select::SH_x64)
        | ctr0::reference_select::set(ctr0::reference_select::AVCC_AVSS)
        // multiple conversions (do not forget this one - otherwise it hangs):
        | ctr0::multiple_sample_conversion::set(1)
    >();

    using ctr1 = adc::Control1;
    ctr1::write<
        ctr1::input_channel::set(ctr1::input_channel::CH_3)
        | ctr1::clock_divider::set(ctr1::clock_divider::DIV_3)
        // single channel multiple conversions:
        | ctr1::conversion_sequence::set(ctr1::conversion_sequence::RepeatSequenceOfChannels)
        | ctr1::clock_source::set(ctr1::clock_source::ADC10OSC)
        //| ctr1::clock_source::set(ctr1::clock_source::ACLK)
    >();

    // number of reads -- in this mode it will be number of channels
    adc::DataTransferControl1::write<n_adc_reeadings>();

    // enable three channels
    // The sequence begins with the Control1 input_channel (INCHx)
    // and continues down to A0. But only these three are enabled.
    adc::AnalogEnable0::write<
        (1 << 3) | (1 << 2) | (1 << 1)
    >();
}

// set the interrupt for ADC10 conversion completion
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR (void)
{
  	__bic_SR_register_on_exit(CPUOFF);  // Return to active mode
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

    ConfigureAdc();
    __enable_interrupt();  // Enable interrupts.

    __delay_cycles(1'000);  // Wait for ADC Ref to settle

    unsigned count_cycle{0};
    while (1) {
        using adc = device::ADC10;
        using ctr0 = adc::Control0;

        __delay_cycles(500'000);  // just a 1s delay to see things

        // start conversion and go to sleep waiting for interrupt
        adc::DataTransferStartAddress::write_addr(&adc_readings[0][0]);
        ctr0::set_or(
            ctr0::enable_conversion::set(1)
            | ctr0::start_conversion::set(1)
        );

        _BIS_SR(LPM0_bits + GIE); // Enter LPM0 w/ interrupt
        // does it really enter LPM3 with ADC10 core running?
        //_BIS_SR(LPM3_bits + GIE);

        // I do not need to read the memory register
        // Data Transfer copies the data into the memory
        //unsigned int ADC_value = adc::Memory::read();

        unsigned adc_sums[n_channels] = {0};
        for (unsigned seq_i = 0; seq_i < n_adc_sequences; seq_i++) {
            for (unsigned chan_i = 0; chan_i < n_channels; chan_i++) {
                adc_sums[chan_i] += adc_readings[seq_i][chan_i];
            }
        }

        // remark #1530-D: (ULP 5.1) Detected divide operation(s). Recommend moving them to RAM during run time or not using as these are processing/power intensive
        unsigned adc_average[3] = {
          adc_sums[0] / n_adc_sequences
          , adc_sums[1] / n_adc_sequences
          , adc_sums[2] / n_adc_sequences
        };
        unsigned adc_average_all = (
            adc_average[0] + adc_average[1] + adc_average[2]) / 3;

        unsigned adc_average_min = adc_average[0] < adc_average[1] ? adc_average[0] : adc_average[1] ;
        adc_average_min = adc_average[2] < adc_average_min ? adc_average[2] : adc_average_min;

        device::Port1::p_out::write(
            //(adc_average_all < 800 ? (1 << board::LED_RED) : 0x0)
            //(adc_average_min < 512 ? (1 << board::LED_RED) : 0x0)
            (adc_readings[0][0] < 512 ? (1 << board::LED_RED) : 0x0)
            // somehow, it just randomly goes on and off
            |
            (count_cycle & 0x1 == 0x1 ? (1 << board::LED_GREEN) : 0x0)
            //(adc_average_min < 512 ? (1 << board::LED_GREEN) : 0x0)
        );

        count_cycle++;
    }
}
