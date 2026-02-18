/// Example of reading ADC10 on MSP430G2553.
/// Multiple channels and multiple conversions.
/// Not ready yet.

#include "msp430chip/controllers.hpp"

namespace board = launchpad_boards::MSP_EXP430G2;
namespace device = board::controller;

unsigned int adc_readings[10] = {0};
constexpr auto n_adc_reeadings = sizeof(adc_readings) / sizeof(adc_readings[0]);

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
        | ctr1::conversion_sequence::set(ctr1::conversion_sequence::RepeatSingleChannel)
        | ctr1::clock_source::set(ctr1::clock_source::ADC10OSC)
        //| ctr1::clock_source::set(ctr1::clock_source::ACLK)
    >();

    // conversion address:
    adc::DataTransferControl1::write<n_adc_reeadings>();

    // indeed, as in the coder-tronics example, you cannot set the address once
    // in the setup code -- it has to be set right before StartConversion
    // if no, the conversion hangs
    //adc::DataTransferStartAddress::write(&adc_readings[0]);
    //adc::DataTransferStartAddress::write_addr<decltype(adc_readings[0]), &adc_readings[0]>();
    //adc::DataTransferStartAddress::write_addr<decltype(adc_readings[0])>(&adc_readings[0]);
    // somehow, it does not instantiate those function templates?
    //adc::DataTransferStartAddress::write_addr(&adc_readings[0]);

    adc::AnalogEnable0::write<1 << 3>();
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

        //__delay_cycles(1'000'000);  // just a 1s delay to see things

        // start conversion and go to sleep waiting for interrupt
        adc::DataTransferStartAddress::write_addr(&adc_readings[0]);
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

        unsigned adc_sum = 0;
        // I wonder if c430 will unroll this loop
        for (unsigned read_i = 0; read_i < n_adc_reeadings; read_i++) {
            adc_sum += adc_readings[read_i];
        }
        unsigned adc_average = adc_sum / n_adc_reeadings;

        device::Port1::p_out::write(
            (adc_average > 512 ? (1 << board::LED_RED) : 0x0)
            |
            (count_cycle & 0x1 == 0x1 ? (1 << board::LED_GREEN) : 0x0)
        );

        count_cycle++;
    }
}
