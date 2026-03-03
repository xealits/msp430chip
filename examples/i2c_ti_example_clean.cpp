// Trying to repeat the following Ti example,
// but MSP430G2553 gets stuck on sending.
// It does process the TX interrupt once, i.e. writes to TX BUF.
// But it never receives the second interrupt, never sends the STOP condition,
// and never exits the LPM0 sleep.
// If I make it exit the sleep at the end of the first interrupt
// and add the stop condition, USC B never clears the stop condition flag.
// It is based on the Ti example of the USCI peripheral:
// https://dev.ti.com/tirex/explore/node?isTheia=false&node=A__AOXtWQGRbQWMQsrS1Xx.3g__msp430ware__IOGqZri__LATEST

//******************************************************************************
//  MSP430G2xx3 Demo - USCI_B0 I2C Master TX single bytes to MSP430 Slave
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  transmits to the slave. This is the master code. It continuously
//  transmits 00h, 01h, ..., 0ffh and demonstrates how to implement an I2C
//  master transmitter sending a single byte using the USCI_B0 TX interrupt.
//  ACLK = n/a, MCLK = SMCLK = BRCLK = default DCO = ~1.2MHz
//
//  *** to be used with "msp430g2xx3_uscib0_i2c_07.c" ***
//
//                                /|\  /|\
//               MSP430G2xx3      10k  10k     MSP430G2xx3
//                   slave         |    |        master
//             -----------------   |    |  -----------------
//           -|XIN  P1.7/UCB0SDA|<-|---+->|P1.7/UCB0SDA  XIN|-
//            |                 |  |      |                 |
//           -|XOUT             |  |      |             XOUT|-
//            |     P1.6/UCB0SCL|<-+----->|P1.6/UCB0SCL     |
//            |                 |         |                 |
//
//  D. Dang
//  Texas Instruments Inc.
//  February 2011
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************
#include <msp430.h>

unsigned char TXData = 0x00;
unsigned char TXByteCtr;

// 0x48 - Ti example
// 0x77 - my Grive BMP280 board
constexpr unsigned slave_address = 0x48;

volatile bool got_nack = false;
volatile bool got_error = false;

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
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  // blink red led on Launchpad
  P1OUT &= ~BIT0;                           // P1.0 = 0
  P1DIR |= BIT0;                            // P1.0 output

  P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0

  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = slave_address;                // Slave Address is 048h
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation

  IE2 |= UCB0TXIE;                          // Enable TX interrupt
  UCB0I2CIE |= UCNACKIE;

  // somehow, it works without enabling the RX interrupt
  if ((IE2 & UCB0RXIE) > 0) while (1) {
    blink_sos();
  }

  TXData = 0x00;                            // Holds TX data

  blink_code(0b1111, 4);

  while (1)
  {
    got_nack = false;
    got_error = false;
    TXByteCtr = 1;                          // Load TX byte counter

    //blink_code(0b100001111, 8);

    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
                                            // Remain in LPM0 until all data
                                            // is TX'd
    TXData++;                               // Increment data byte

    // if got error - clear the STOP request yourself
    if (got_error) {
      UCB0CTL1 &= ~UCTXSTP;
    }

    while (UCB0CTL1 & UCTXSTP)              // Ensure stop condition got sent
    {
      // it enters and blinks here once
      //blink_code(0b11, 2);
      blink_sos();
    }

    if (got_nack) {
      blink_code(0b1100, 4);
    }

    else {
      blink_code(0b00, 2);
    }

    if (got_error) {
      blink_code(0b1010, 4);
    }
  }
}

// the receive interrupt vector handles the change flags, such as NACK
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{

  // the NACK interrupt (enabled by the NACKIE in the setup)
  // triggers the RX ISR

  // on NACK, the flags change like this:
  bool nack_flag_is_on = (UCB0STAT & UCNACKIFG) > 0;
  bool start_condition_is_cleared = (UCB0CTL1 & UCTXSTT) == 0;
  bool tx_buf_ready = (IFG2 & UCB0TXIFG) > 0;
  bool nack_full_condition = start_condition_is_cleared && nack_flag_is_on && !tx_buf_ready;

  // the NACK interrupt when the address is wrong
  if (!nack_full_condition) while (1) blink_code(0b1, 1);

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
  // if nothing is wrong on the bus,
  // the SCL must be held down until STP condition is requested
  //if (!(UCB0STAT & UCSCLLOW > 0)) {
  //  got_error = true;
  //}
  // nope, that one does not work
  // Bus Busy does seem to be set
  if ((UCB0STAT & UCBBUSY) == 0) {
    got_error = true;
  }

  //while (1) blink_sos();

  // clear the flag
  UCB0STAT &= ~UCNACKIFG;

  UCB0CTL1 |= UCTXSTP;  // I2C stop condition

  // let's also clear the flag
  IFG2 &= ~UCB0RXIFG;

  __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
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
  if (TXByteCtr)                            // Check TX byte counter
  {
    UCB0TXBUF = TXData;                     // Load TX buffer
    TXByteCtr--;                            // Decrement TX byte counter

    // blink after writing to the buffer
    //blink_code(0b011010, 6);

    //// check whether there is a NACK at this point
    //if (UCB0STAT & (UCNACKIFG) )
    //{
    //  while (1) blink_code(0x0, 1);
    //}
  }

  // this branch is never triggered - there is never a second interrupt
  // and the msp430g2553 never leaves the LPM0 sleep state
  // red LED remains ON
  else
  {
    //while (1) blink_code(0x01, 2);
    got_nack = false;

    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
  }

  //// here, if everything is OK, the SCL must be low
  //// - somehow, no, it is not SCL low even when things work
  ////if ((UCB0STAT & UCSCLLOW) == 0) {
  ////  got_error = true;
  ////}
  //// bus busy? - yeah, this one seems to work in the good case
  //// but it does not catch the bad case
  //while (1) {
  //  blink_code(0b10, 2);

  //  // yeah, the error case never hits it
  //  // so USCI did release the bus, but its status register does not show it
  //  if ((UCB0STAT & UCBBUSY) == 0) {
  //    got_error = true;

  //    while (1) blink_sos();

  //    // and it needs to exit
  //    //UCB0CTL1 &= ~UCTXSTP; // clear stop condition?
  //    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
  //    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
  //  }
  //}
}

