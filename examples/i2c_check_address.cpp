/// I2C connection - chek an address ACK and set an LED
/// based on Ti example:
/// https://dev.ti.com/tirex/explore/node?isTheia=false&node=A__ALJ6CcSCZehPN6QX8ONibw__msp430ware__IOGqZri__LATEST
/// And some bits from this extended post:
/// http://www.simplyembedded.org/tutorials/msp430-i2c-basics/
/// using USCI B module, the one with I2C support

#include "msp430chip/controllers.hpp"

namespace board = launchpad_boards::MSP_EXP430G2;
namespace device = board::controller;

// 0x77 default address of Grove BMP280
// 0x48 some sensor from Ti example
constexpr uint8_t slave_address = 0x77;

// set true on NACK interrupt
volatile bool got_nack = false;
bool latch_rx_interrupt = false;

// just to show the order in which the two interrupts happen
volatile unsigned interrupt_counter = 0;

void blink_code(unsigned code, unsigned bit_len) {
  unsigned min_len = (bit_len < sizeof(code) * 8 ? bit_len : sizeof(code) * 8);

  for (unsigned bit_i = 0; bit_i < min_len; bit_i++) {
    P1OUT &= ~BIT0; // P1.0 = 0
    __delay_cycles(200'000);

    P1OUT |= BIT0; // P1.0 = 1
    if ((code & (1 << (min_len - 1 - bit_i))) > 0) {
      __delay_cycles(1'000'000);
    }
    else {
      __delay_cycles(100'000);
    }
  }
}

void blink_sos(void) {
  blink_code(0b000, 3);
  blink_code(0b111, 3);
}

int main(void)
{
    // Stop watchdog timer
    device::WatchdogTimer::Control::write(WDTPW + WDTHOLD);

    BCSCTL1 = CALBC1_1MHZ;  // Set range   DCOCTL = CALDCO_1MHZ;
    BCSCTL2 &= ~(DIVS_3);   // SMCLK = DCO = 1MHz

  // blink red led on Launchpad
  P1OUT &= ~BIT0;                           // P1.0 = 0
  P1DIR |= BIT0;                            // P1.0 output

  P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0


    /*
    // initialize all ports to IO output - unused too
    // pins 6 and 7 are set to USCI_B
    // the rest are I/O with direction output
    device::Port1::p_sel::write<(1 << 6) | (1 << 7)>();
    device::Port1::p_sel2::write<(1 << 6) | (1 << 7)>();
    device::Port1::p_dir::write<0xFF>(); // all output

    //// I2C peripheral function
    //P1SEL |= BIT6 + BIT7;       // Assign I2C pins to USCI_B
    //P1SEL2|= BIT6 + BIT7;       // Assign I2C pins to USCI_B

    { // unused ports
        // set not connected pins to primary peripheral mode
        // (a way this works, setting them to I/O hangs the chip)
        device::Port2::p_sel::write<0xF0 - 0x30>(); // 6 pins 0-5 are on the package
        device::Port2::p_sel2::write<0x00>();
        device::Port2::p_dir::write<0x3F>();

        device::Port3::p_sel::write<0xFF>();
        device::Port3::p_sel2::write<0x00>();
    }

    { // configure USCI B for I2C
        
      {
        using ctr1 = device::USCI_B::Control1;
        ctr1::write<
          ctr1::SoftwareReset::set(1) // put in reset to configure
        >();
      }

      {
        using ctr0 = device::USCI_B::Control0;
        ctr0::write<
          ctr0::USCIMode::set(ctr0::USCIMode::I2C)
          | ctr0::SyncMode::set(ctr0::SyncMode::SYNCHRONOUS)
          | ctr0::MasterMode::set(ctr0::MasterMode::MASTER)
        >();
      }

      //
      //Configure the baud rate registers for 100kHz when sourcing from SMCLK
      //where SMCLK = 1MHz
      //
      //UCB0BR0 = 10; 
      //UCB0BR1 = 0;
      {
        device::USCI_B::BaudRate0::write(10);
        device::USCI_B::BaudRate1::write(0);
      }

      {
        // configure slave address
        device::USCI_B::I2CSlaveAddress::write(slave_address);
      }

      {
        // set the clock source to SMCLK, which is 1MHz, and take it out from reset
        using ctr1 = device::USCI_B::Control1;
        ctr1::write<
          ctr1::ClockSource::set(ctr1::ClockSource::SMCLK)
          // keep SW reset - nope
          // let's follow the blog post here
          | ctr1::SoftwareReset::set(0)
        >();
      }
    }
    */

  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = slave_address;                // Slave Address is 048h
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation

    // enable TX interrupt
    IE2 |= UCB0TXIE;

    // enable the NACK interrupt
    //UCB0I2CIE |= UCNACKIE;
    //UCB0I2CIE |= 0b1000;

    blink_code(0b1100, 4);

    unsigned count_cycle{0};
    while (1) {
        interrupt_counter = 0;
        //__delay_cycles(500'000);  // just a reasonable delay

    //blink_code(0b1100, 4);

        //_BIS_SR(LPM0_bits + GIE); // Enter LPM0 w/ interrupt
        // does it really enter LPM3 with ADC10 core running?
        //_BIS_SR(LPM3_bits + GIE);

        //if (count_cycle == 1 && (IFG2 & UCB0TXIFG) > 0)
        //if (count_cycle == 1)
        ////while (1)
        //{
        //  //blink_code(0b0, 1);

        //  // status is all clear here
        //  //blink_code(UCB0STAT, 4);

        //  // there is no reset
        //  if (UCB0CTL1 & UCSWRST > 0) blink_code(0b0, 1);
        //  // everything is still set
        //  else if ((UCB0I2CSA & 0x7f) != slave_address) blink_code(0b0, 1);
        //  // the interrupt is still set
        //  else if ((IE2 & UCB0TXIE) == 0) blink_code(0b0, 1);
        //  else blink_code(0b1, 1);
        //}

    //// let's try to re-emnable the TX interrupt
    //IE2 &= ~UCB0TXIE;
    //IE2 |= UCB0TXIE;
    //// no, it still hangs

    //// let's try SW reset:
    //UCB0CTL1 |= UCSWRST;   // Enable SW reset
    //// it resets everything -- the clock and data lines are down
    //UCB0CTL1 &= ~UCSWRST;  // Clear SW reset, resume operation

  //      // let's redo the full config:
  //P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  //P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  //UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  //UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  //UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  //UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
  //UCB0BR1 = 0;
  //UCB0I2CSA = slave_address;                // Slave Address is 048h
  //UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  //  // enable TX interrupt
  //  IE2 |= UCB0TXIE;
  //  // enable the NACK interrupt
  //  UCB0I2CIE |= UCNACKIE;
  //  //UCB0I2CIE |= 0b1000;

  //  // it still hangs!!!

        // start condition
        //UCB0CTL1 |= UCTXSTT;     // I2C start condition
        /* Send the start condition */
        // and the transmitter mode
        UCB0CTL1 |= UCTR | UCTXSTT;

    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts

    // with the good address, it never comes out from the sleep here
    // although, I do see how the address byte is sent the second time
    // -- is that because I currently do not load the TX buffer?
        //if (count_cycle == 1) while (1) {
        //  blink_code(0b0, 1);
        //}

  // do we ever get out from the sleep?
  // looks like no?
  // yes, it does get out - it is the problem of reset vs power cycle with the sleep mode
  //while (1) blink_sos();

        // maybe let's write a byte?
        //UCB0TXBUF = 0x10;
        // it hangs here too

        //// send the address byte
        //// HOW?
        //// the blog post just "waits for the start condition to be sent"
        //// it must mean that the address byte is included in the start condition
        ///* Wait for the start condition to be sent and ready to transmit interrupt */
        // the NACK interrupt must be handled before it passes this wait
        while ((UCB0CTL1 & UCTXSTT) && ((IFG2 & UCB0TXIFG) == 0)) {
          blink_sos();
        }

        //// maybe let's write a byte?
        //UCB0TXBUF = 0x10;

        //__delay_cycles(100'000);  // just a delay

        //// stop condition
        //UCB0CTL1 |= UCTXSTP;     // Generate I2C stop condition

    /*
    while (1) blink_code(0b1, 1);

        // check ACK/NACK
        bool got_nack = (device::USCI_B::Status::read() & UCNACKIFG) > 0;
        bool got_ifg = (IFG2 & UCB0TXIFG) > 0;

    while (UCB0CTL1 & UCTXSTP)              // Ensure stop condition got sent
    {
      blink_code(0b1, 1);
      // it seems there is always at least one of these
    }
    */

        //device::Port1::p_out::write(
        //    ((got_nack || got_ifg) ? (1 << board::LED_RED) : 0x0)
        //    |
        //    // green led is on pin 6 - which is used for I2C now
        //    //(count_cycle & 0x1 == 0x1 ? (1 << board::LED_GREEN) : 0x0)
        //    (count_cycle & 0x1 == 0x1 ? (1 << board::LED_RED) : 0x0)
        //);

        // the ISRs have set the got_nack correctly:
        if (got_nack) {
          blink_code(0b10, 2);
        }

        else {
          blink_code(0b001, 3);

          // test that the RX interrupt never fires in the ACK case:
          if (latch_rx_interrupt) {
            while (1) blink_code(0b0, 1);
          }
        }

        // check and clear the interrupt counter:
        // ACK case, 1 interrupt: only TX (buf ready) and no RX interrupts
        // NACK case, 4 interrutps: TX (buf ready), RX (NACK)
        if (!(interrupt_counter == 1 || interrupt_counter == 2)) {
          while (1) {
            blink_sos();
            blink_code(0b001, 3);
          }
        }

        count_cycle++;
    }
}

// the receive interrupt vector handles the change flags, such as NACK
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
  // it seems that when it is the ACK case, the RX interrupt never fires
  // let's latch and test it:
  latch_rx_interrupt = true;

  // the NACK interrupt (enabled by the NACKIE in the setup)
  // triggers the RX ISR
  // let's confirm that it is indeed the second interrupt
  // after the TX buf ready:

  // at this NACK, the flags change like this:
  bool nack_flag_is_on = (UCB0STAT & UCNACKIFG) > 0;
  bool start_condition_is_cleared = (UCB0CTL1 & UCTXSTT) == 0;
  bool tx_buf_ready = (IFG2 & UCB0TXIFG) > 0;
  bool nack_full_condition = start_condition_is_cleared && nack_flag_is_on && !tx_buf_ready;

  if (interrupt_counter == 0) {
    // this never happens - the first interrupt is TX buf ready
    while (1) blink_code(0b0, 1);
  }

  if (interrupt_counter == 1)
  {
    // the NACK interrupt when the address is wrong
    if (!nack_full_condition) while (1) blink_code(0b1, 1);
    //if (!(start_condition_is_cleared && !nack_full_condition)) while (1) blink_code(0b0, 1);

    // just to confirm that the stop condition is cleared here:
    bool stop_condition_cleared = (UCB0CTL1 & UCTXSTP) == 0;
    if (!stop_condition_cleared) while (1) blink_code(0b1, 1);
    // the TX ISR requests the stop condition and exits
    // the MSP430 goes to process the RX ISR for the NACK interrupt
    // and the USCI B has already done the stop condition
    // so, it is already cleared, but I am not sure whether this is reliable
    // could the MSP430 clock be much faster than the I2C SCL,
    // so that the RX ISR is processed before the stop condition is done?

    got_nack = true;
    // clear the flag or this ISR will rerun non-stop
    // after the interrupt 3 (i.e. interrupt 4, 5, etc will be NACK interrutps):
    UCB0STAT &= ~UCNACKIFG;
  }

  else
  {
    while (1) {
      blink_code(0b01, 2);
      blink_sos();
    }
  }

  // finally:
  interrupt_counter++;
  // let's also clear the flag
  IFG2 &= ~UCB0RXIFG;

  // not sure why it is needed to exit with CPUOFF
  // I thought the TX ISR already got the MSP430 out from LPM
  // but, it hangs without this
  __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0

  // and, in principle, no need to do anything
  // the TX ISR has requested the stop condition and exited the LPM0

  /*
  if (!first_tx_interrupt)
  {
    bool start_condition_is_cleared = (UCB0CTL1 & UCTXSTT) == 0;
    bool nack_flag_is_on = (UCB0STAT & UCNACKIFG) > 0;
    bool tx_buf_ready = (IFG2 & UCB0TXIFG) > 0;

    // now, in the RX interrupt vector
    // STT gets cleared for the bad address here
    // for the good address, this ISR does not run
    // -- the ISR must be triggered by the NACK for the bad address
    //if (start_condition_is_cleared) while (1) blink_code(0b010, 3);

    // the bad address does not hit this:
    if (tx_buf_ready) while (1) blink_code(0b01, 3);

    // the bad address does hit this loop:
    //if (nack_flag_is_on) while (1) blink_code(0b001, 3);

    // so, the conditions must be as in the diagram:
    bool nack_full_condition = start_condition_is_cleared && nack_flag_is_on && !tx_buf_ready;
    if (nack_full_condition) while (1) blink_code(0b011, 3);

    // interesting that with the wrong address
    // I do not hit this loop
    // i.e. there is no interrupt after writing to TXBUF?
  }

  else {
    while (1) blink_code(0b111, 3);
  }
  */

}

//------------------------------------------------------------------------------
// The USCIAB0TX_ISR is structured such that it can be used to transmit any
// number of bytes by pre-loading TXByteCtr with the byte count.
//------------------------------------------------------------------------------
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCIAB0TX_VECTOR))) USCIAB0TX_ISR (void)
#else
#error Compiler not supported!
#endif
{
  // the TX ISR is triggered by the TX buffer ready interrupt
  // after the Start Condition is out
  // i.e. it is the first interrupt in the transaction
  // let's confirm it:
  //if (interrupt_counter == 1) while (1) {
  //  blink_code(0b001, 3);
  //  // yeah, on ACK, TX gets triggered the second time
  //}

  if (interrupt_counter > 1)
  {
    while(1) blink_code(0b1, 1);
  }

  // the Start Condition includes the address byte
  // so, the only thing left to do here is to end the transaction
  // with the stop condition:

  // it did not get NACK yet
  got_nack = false;

  if (interrupt_counter == 0) {
    //// apparently, if I do not write to the buffer on ACK, it will hang?
    //// on the next iteration of the main loop, after the Start Condition
    //// MSP430 won't get an interrupt into this ISR and will remain in the LPM
    //// maybe there is a way to reset it with the UCSWRST
    UCB0TXBUF = 0xD0; // the ID register address in BMP280
    //// no, this does not help
    /// and I do not see the data! no control byte!!

    // with this line, I got the control byte - and no extra ring on the ACK bit
    //while(1) blink_code(0b1, 1);
    // when I reset, only the clock goes up.......
  }

  else {
    // it never gets here and both data and clock are on the ground
    UCB0CTL1 |= UCTXSTP;  // I2C stop condition
    IFG2 &= ~UCB0TXIFG;   // Clear USCI_B0 TX int flag

    // and exit LPM0
    __bic_SR_register_on_exit(CPUOFF);

    // test if we get here
    //while(1) blink_code(0b01, 2);
    // yep, now it gets gere
  }

    //UCB0CTL1 |= UCTXSTP;  // I2C stop condition
    //IFG2 &= ~UCB0TXIFG;   // Clear USCI_B0 TX int flag

    //// and exit LPM0
    //__bic_SR_register_on_exit(CPUOFF);
    // OK, this stop condition must not be done together with the data buffer!
    // and the extra ring on ACK happens on the stop condition


  //if (interrupt_counter == 0) {
  //  if ((IFG2 & UCB0TXIFG) > 0)
  //    while(1) blink_code(0b01, 2);
  //}

  //  while(1) blink_code(0b0, 1);

  interrupt_counter++;

  /*
  // blink to show that it handles the first interrupt
  // the scope triggers at the interrupt - before this blinking pattern
  //blink_code(0b011010, 6);

  // clear the LED pin
  // in case it does not wake up from the sleep,
  // it is the only place where LED is cleared
  //P1OUT &= ~BIT0; // P1.0 = 0

  // do we ever hit interrupt? - yes, whether the address is right or wrong
  //while (1) blink_sos();

  //// it never hits this when the address is wrong
  //// when should I check for the NACK????
  //if (UCB0STAT & (UCNACKIFG) ) {
  //  while (1) blink_sos();
  //}

  // User Gauid slau144k says
  // "as soon as slave acknowledges the address the UCTXSTT bit is cleared"
  //bool start_condition_cleared = (UCB0CTL1 & UCTXSTT) == 0;
  // let's just wait for the start condition to clear
  // now the correct address never clears it???
  // and the wrong address does not clear it either
  // so the User Guide is wrong here?
  // UCTXSTT just stays ON
  // -- it stays on for the good address (no matter NACK interrupt)
  // it is off for the bad address, when the NACK interrupt is enabled or no!!
  // then it does hit the nack bit condition -- the sos blinking

  //while ((UCB0CTL1 & UCTXSTT) > 0) {
  //  blink_code(0b01, 2);
  //}

  // let's check the interrupt flag
  // this flag says: ready to send some data
  // according to the user guide
  // it is set after the start condition is generated
  // -- here, yes, this is the flag that's on now
  // both for a wrong address and the right one
  //while ((IFG2 & UCB0TXIFG) > 0) {
  //  blink_code(0b1, 1);
  //}

  //if (!start_condition_cleared) {
  //  //while (1) blink_sos();
  //  // it does hit this when the address is wrong
  //  // and also when the address is right...
  //  bool nack_flag_is_on = (UCB0STAT & UCNACKIFG) > 0;
  //}

  // I do see the correct signals on the scope:
  // clock and data line with the 9th bit NACK
  // after that, for the wrong address the MSP430 just holds the clock like down
  // -- naturally, it never gets to the stop condition etc.
  // So, why on earth it gives me nothing here?
  // Why no NACK flag? - try to enable the NACK interrupt
  // no, still no nack flag....
  //
  // and somehow it works now
  // the only difference -- I "reset" it by power-cycling instead of reset command
  // and no - it stopped working
  //
  // if I add that loop on the STT - the following starts working for the wrong address
  // does the order of reading registers matter?
  */

  /*
  bool start_condition_is_cleared = (UCB0CTL1 & UCTXSTT) == 0;
  //// so, with NACK IE on or off, for both addresses
  //// it stops here - STT is never cleared here
  //if (!start_condition_is_cleared) {
  //  while (1) blink_code(0b01, 2);
  //}

  bool tx_buf_ready = (IFG2 & UCB0TXIFG) > 0;
  if (!tx_buf_ready) {
    while (1) blink_code(0b0, 1);
  }

  bool nack_flag_is_on = (UCB0STAT & UCNACKIFG) > 0;

  //now check NACK:
  //if (start_condition_is_cleared && nack_flag_is_on)
  // this one is never set on the start condition now: for both addresses and NACKIE = on
  // although I do see the ACK and NACK on the scope
  if (nack_flag_is_on)
  {
    while (1) blink_sos();
  }

  //else {
  //  // it just always goes to this branch
  //  // for the correct and wrong addess
  //  while (1) blink_code(0b0, 1);
  //}
  */

  /*
  if (!first_tx_interrupt)
  {
    bool start_condition_is_cleared = (UCB0CTL1 & UCTXSTT) == 0;
    bool nack_flag_is_on = (UCB0STAT & UCNACKIFG) > 0;
    bool tx_buf_ready = (IFG2 & UCB0TXIFG) > 0;

    // the good address hits this line -- i.e. there is a second interrupt
    // but the bad address does not -- the NACK interrupt is not generated
    // although I do have it enabled
    // UCB0I2CIE |= UCNACKIE;
    if (start_condition_is_cleared) while (1) blink_code(0b000, 3);

    //if (nack_flag_is_on) while (1) blink_code(0b001, 3);
    //if (tx_buf_ready) while (1) blink_code(0b01, 3);

    // interesting that with the wrong address
    // I do not hit this loop
    // i.e. there is no interrupt after writing to TXBUF?
  }

  //while (1) blink_code(0b001, 3);

  if (first_tx_interrupt)                     // Check TX byte counter
  {
    first_tx_interrupt = false;
    UCB0TXBUF = 0b10101100;                   // Load TX buffer
    //TXByteCtr--;                            // Decrement TX byte counter

    // blink after writing to the buffer
    //blink_code(0b011010, 6);

  // now it does hit this loop even for the good address
  // -- i.e. the second interrupt either does not happen or does not pull it from this ISR?
  // and STT is still not cleared here?
  //bool start_condition_is_cleared = (UCB0CTL1 & UCTXSTT) == 0;
  //if (!start_condition_is_cleared) {
  //  while (1) blink_code(0b00, 2);
  //}
  // so, let's let it go
  // the idea is that the second interrupt always happens -- for the bad address too

    // check whether there is a NACK at this point
    // if the address is wrong, I still do not get this triggered
    // so, when should I check the NACK bit?
    if (UCB0STAT & (UCNACKIFG) )
    {
      while (1) blink_code(0b0, 1);
      //while (1) blink_sos();
    }

    //while (1) blink_code(0b1100, 4); // without this, it blinks 0001 for some reason?
    // I commented it out and it still blinks 1100
    // then rebuilt-reuploaded -- it blinks 0001

    // it seems I do not get here, because the second trigger gets launched
    //while (1) blink_code(0b1111, 4);
    //while (1) blink_code(0b1, 1);
    // now finally it blinks steady 1
    // something is messed up, either in the compiler or in the flashing
    // the flash did not refresh this line of code or whatever?
    //while (1) blink_code(0b1111, 4); // and now it blinks 0001 again
    //while (1) blink_code(0b1110, 4); // and now it blinks steady 0 -- because it was 0x !!! not b
  }

  // this branch is never triggered - there is never a second interrupt
  // and the msp430g2553 never leaves the LPM0 sleep state
  // red LED remains ON
  else
  {
    //while (1) blink_code(0b01, 2);

  // do we ever hit the second interrupt?
  // looks like no? - it is an issue of the reset VS power-cycle
  // reset does not get it out of the sleep state or whatever
  // bottom line: it does get here, I just need to power cycle to see the new code work correctly
  //while (1) {
  //  blink_sos();
  //  blink_code(0b00, 2);
  //}

    first_tx_interrupt = true;

    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag

    // done with the I2C transaction - exit
    // but I do not see this one - how???
    //blink_code(0b010101, 6);

    // but I do see this one:
  //while (1) {
  //  blink_sos();
  //  blink_code(0b001, 3);
  //}

    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
  }
*/
}


