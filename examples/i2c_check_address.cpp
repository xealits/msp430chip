/// I2C connection - chek an address ACK and set an LED
/// based on Ti example:
/// https://dev.ti.com/tirex/explore/node?isTheia=false&node=A__ALJ6CcSCZehPN6QX8ONibw__msp430ware__IOGqZri__LATEST
/// And some bits from this extended post:
/// http://www.simplyembedded.org/tutorials/msp430-i2c-basics/
/// using USCI B module, the one with I2C support

#include "msp430chip/controllers.hpp"

// not yet
//namespace board = launchpad_boards::MSP_EXP430G2;
//namespace device = board::controller;

constexpr uint8_t slave_address = 0x77; // default address of Grove BMP280

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

    // I2C peripheral function
    P1SEL |= BIT6 + BIT7;       // Assign I2C pins to USCI_B0
    P1SEL2|= BIT6 + BIT7;       // Assign I2C pins to USCI_B0

    { // configure USCI B for I2C
        
      {
        using ctr1 = device::USCI_B0::Control1;
        ctr1::write<
          ctr1::SoftwareReset::set(1) // put in reset to configure
        >();
      }

      {
        using ctr0 = device::USCI_B0::Control0;
        ctr0::write<
          ctr0::USCIMode::set(ctr0::USCIMode::I2C)
          | ctr0::SyncMode::set(ctr0::SyncMode::SYNCHRONOUS)
          | ctr0::MasterMode::set(ctr0::MasterMode::MASTER)
        >();
      }

      /** 
      * Configure the baud rate registers for 100kHz when sourcing from SMCLK
      * where SMCLK = 1MHz
      */
      //UCB0BR0 = 10; 
      //UCB0BR1 = 0;
      {
        using bd0 = device::USCI_B0::BaudRate0::write(10);
        using bd1 = device::USCI_B0::BaudRate1::write(0);
      }

      {
        // configure slave address
        device::USCI_B0::I2CSlaveAddress::write(slave_address);
      }

      {
        // set the clock source to SMCLK, which is 1MHz, and take it out from reset
        using ctr1 = device::USCI_B0::Control1;
        ctr1::write<
          ctr1::ClockSourceSelect::set(ctr1::ClockSourceSelect::SMCLK)
          // keep SW reset - nope
          // let's follow the blog post here
          | ctr1::SoftwareReset::set(0)
        >();
      }
    }

    unsigned count_cycle{0};
    while (1) {
        __delay_cycles(500'000);  // just a reasonable delay

        //_BIS_SR(LPM0_bits + GIE); // Enter LPM0 w/ interrupt
        // does it really enter LPM3 with ADC10 core running?
        //_BIS_SR(LPM3_bits + GIE);

        // start condition
        //UCB0CTL1 |= UCTXSTT;     // I2C start condition
        /* Send the start condition */
        // and the transmitter mode
        UCB0CTL1 |= UCTR | UCTXSTT;

        // send the address byte
        // HOW?
        // the blog post just "waits for the start condition to be sent"
        // it must mean that the address byte is included in the start condition
        /* Wait for the start condition to be sent and ready to transmit interrupt */
        while ((UCB0CTL1 & UCTXSTT) && ((IFG2 & UCB0TXIFG) == 0));

        // check ACK/NACK
        bool got_ack = (device::USCI_B0::Status::read() & UCNACKIFG) > 0;

        // stop condition
        UCB0CTL1 |= UCTXSTP;     // Generate I2C stop condition

        device::Port1::p_out::write(
            (got_ack ? (1 << board::LED_RED) : 0x0)
            |
            (count_cycle & 0x1 == 0x1 ? (1 << board::LED_GREEN) : 0x0)
        );

        count_cycle++;
    }
}
