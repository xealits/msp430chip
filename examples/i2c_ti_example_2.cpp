#include <msp430.h>

#include "msp430chip/controllers.hpp"

namespace board = launchpad_boards::MSP_EXP430G2;
namespace device = board::controller;

unsigned char TXData;
unsigned char TXByteCtr;

unsigned count_cycle{0};

void blink_code(unsigned code, unsigned bit_len) {
  unsigned min_len = (bit_len < sizeof(code) * 8 ? bit_len : sizeof(code) * 8);
  for (unsigned bit_i = 0; bit_i < min_len; bit_i++) {
    __delay_cycles(500'000);
    device::Port1::p_out::write(0x00);

    if (code & (1 << bit_i) > 0) {
      __delay_cycles(1'000'000);
    }
    else {
      __delay_cycles(100'000);
    }
    device::Port1::p_out::write((1 << board::LED_RED));
  }
}

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  // red led on launchpad board
  P1OUT &= ~BIT0;                           // P1.0 = 0
  P1DIR |= BIT0;                            // P1.0 output

  P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
  UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
  UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
  UCB0BR1 = 0;
  UCB0I2CSA = 0x48;                         // Slave Address is 048h
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation

  IE2 |= UCB0TXIE;                          // Enable TX interrupt

  TXData = 0x00;                            // Holds TX data

  device::Port1::p_out::write(0x00);
  __delay_cycles(500'000);  // just a delay
  device::Port1::p_out::write((1 << board::LED_RED));
  // why tf this does not light up the led when the __bis is called in the loop?

  blink_code(0b1111, 4);

  while (1)
  {
    TXByteCtr = 1;                          // Load TX byte counter

    blink_code(0b0, 1);

    while (UCB0CTL1 & UCTXSTP)             // Ensure stop condition got sent
    {
      blink_code(0b0, 1);
      // this flag never gets cleared
    }

    UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition

    //device::Port1::p_out::write(
    //    (count_cycle & 0x1 == 0x1 ? (1 << board::LED_RED) : 0x0)
    //);

    blink_code(0b0, 1);

    // it never comes out from this interrupt?
    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
                                            // Remain in LPM0 until all data
                                            // is TX'd
    TXData++;                               // Increment data byte

    // it does come out from the interrupt on the first iteration
    // and gets stuck on the second

    blink_code(0b0, 10);

    __delay_cycles(500'000);  // just a delay

      //device::Port1::p_out::write(
      //    //(got_ack ? (1 << board::LED_RED) : 0x0)
      //    //(check_stp ? (1 << board::LED_RED) : 0x0)
      //    //|
      //    // green led is on pin 6 - which is used for I2C now
      //    //(count_cycle & 0x1 == 0x1 ? (1 << board::LED_GREEN) : 0x0)
      //    (count_cycle & 0x1 == 0x1 ? (1 << board::LED_RED) : 0x0)
      //);

      count_cycle++;

  }
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

  blink_code(0b11, 2);

  if (TXByteCtr)                            // Check TX byte counter
  {
    UCB0TXBUF = TXData;                     // Load TX buffer
    TXByteCtr--;                            // Decrement TX byte counter

    blink_code(0b01, 2);

    // check NACK?
    if (UCB0STAT & UCNACKIFG) {
      blink_code(0b0001, 4);
    }

    else {
      blink_code(0b0111, 4);
    }

    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
    // also clear NACK:
    UCB0STAT &= ~UCNACKIFG;

    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
  }

  else
  {
    // this branch is never triggered
    blink_code(0b010101, 6);

    UCB0CTL1 |= UCTXSTP;                    // I2C stop condition
    IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
    __bic_SR_register_on_exit(CPUOFF);      // Exit LPM0
  }
}

