/// I2C connection - chek an address ACK and set an LED
/// based on Ti example:
/// https://dev.ti.com/tirex/explore/node?isTheia=false&node=A__ALJ6CcSCZehPN6QX8ONibw__msp430ware__IOGqZri__LATEST
/// And some bits from this extended post:
/// http://www.simplyembedded.org/tutorials/msp430-i2c-basics/
/// using USCI B module, the one with I2C support

#include "msp430chip/controllers.hpp"

namespace board = launchpad_boards::MSP_EXP430G2;
namespace device = board::controller;

// a bit array to save which addresses reply ACK
constexpr unsigned n_i2c_addresses = 1 << 7;
static_assert(n_i2c_addresses == 128, "1 << 7 == 128");
uint8_t address_acks[n_i2c_addresses / 8];

// set true on NACK interrupt
volatile bool got_nack = false;
// keep track of the two TX interrupts
bool sent_tx_byte = false;

//// 0x77 default address of Grove BMP280
//// 0x48 some sensor from the Ti example
//constexpr uint8_t slave_addresses[] = {0x77, 0x48};

//// count the TX interrupts
//volatile unsigned interrupt_counter = 0;
//// and that the RX interrupts don't happen in the ACK case
//bool latch_rx_interrupt = false;

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

  /*
  // blink red led on Launchpad
  P1OUT &= ~BIT0;                           // P1.0 = 0
  P1DIR |= BIT0;                            // P1.0 output

  // Configs from the Ti example:
  P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0

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
  UCB0I2CIE |= UCNACKIE;
  */

  // initialize all ports to IO output - unused too
  // pins 6 and 7 are set to USCI_B
  // the rest are I/O with direction output
  device::Port1::p_sel::write<(1 << 6) | (1 << 7)>();
  device::Port1::p_sel2::write<(1 << 6) | (1 << 7)>();
  device::Port1::p_dir::write<0xFF>(); // all output

  { // unused ports
    // the configuration of anused ports adds some 16B to the size of the binary
    // wrt the Ti config above
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
    {
      device::USCI_B::BaudRate0::write(12);
      device::USCI_B::BaudRate1::write(0);
    }

    {
      // configure slave address
      device::USCI_B::I2CSlaveAddress::write(0x00);
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

    // enable the NACK interrupt
    //UCB0I2CIE |= UCNACKIE;
    {
      using ie = device::USCI_B::I2CInterruptEnable;
      ie::write<
        ie::NotAcknowledgeIE::set(1)
        >();
    }
  }

  // enable TX interrupt
  IE2 |= UCB0TXIE;

  unsigned count_iterations = 0;
  while (1) {
    sent_tx_byte = false;

    // set the address
    uint16_t cur_i2c_address = count_iterations & 0x7f;
    device::USCI_B::I2CSlaveAddress::write(cur_i2c_address);

    // on address 0x00, let's clear the array of the ack scan
    if (cur_i2c_address == 0x00) {
      for (uint8_t& byte: address_acks) {
        byte = 0x00;
      }
    }

    // Send the start condition and the transmitter mode
    UCB0CTL1 |= UCTR | UCTXSTT;
    // and go to sleep until all USCI B interrupts are processed
    __bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts

    // Wait for the start condition to be sent and ready to transmit interrupt
    // the NACK interrupt must be handled before it passes this wait
    while ((UCB0CTL1 & UCTXSTT) && ((IFG2 & UCB0TXIFG) == 0)) {
      blink_sos();
    }

    // the ISRs have set the got_nack correctly:
    if (!got_nack) {
      // save the found ack in address_acks
      uint8_t n_byte = cur_i2c_address / 8;
      uint8_t n_bit = cur_i2c_address % 8;

      uint8_t& ack_byte = address_acks[n_byte];
      ack_byte |= 1 << n_bit;
    }

    // on the last address 0x7F, let's blink out the found addresses
    if (cur_i2c_address == 0x7F) {
      uint8_t n_byte = 0;
      for (const uint8_t& byte: address_acks) {

        for (uint8_t n_bit = 0; n_bit < 8; n_bit++) {
          bool got_ack = (byte & (1 << n_bit)) > 0;

          if (got_ack) {
            uint8_t i2c_addr = n_byte * 8 + n_bit;
            blink_code(i2c_addr, 8);
            __delay_cycles(1'000'000);
          }
        }

        n_byte ++;
      }
    }

    count_iterations++;
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
  // clear the flag or this ISR will rerun non-stop
  // (i.e. interrupts 3, 4, 5, etc will be NACK interrutps):
  UCB0STAT &= ~UCNACKIFG;

  // and request stop from USCI
  // otherwise, the SCL will be held to the ground - the bust constantly busy
  UCB0CTL1 |= UCTXSTP;  // I2C stop condition
  // you can see on the scope how long it takes
  // to process this RX interrupt after the TX one
  // and issue the stop after the address byte got NACK

  // let's also clear the flag
  IFG2 &= ~UCB0RXIFG;

  // not sure why it is needed to exit with CPUOFF
  // I thought the TX ISR already got the MSP430 out from LPM
  // but, it hangs without this
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
  // the TX ISR is triggered by the TX buffer ready interrupt
  // after the Start Condition is out
  // and, if the address byte gets ACK, the USCI B writes the control byte
  // and it triggers the second TX interrupt - ready for the next byte
  // i.e. TX interrupt can be 1st and 2nd in the transaction

  // it did not get NACK yet
  got_nack = false;

  if (!sent_tx_byte) {
    // apparently, if I do not write to the buffer on ACK, it will hang?
    // on the next iteration of the main loop, after the Start Condition
    // MSP430 won't get an interrupt into this ISR and will remain in the LPM
    // maybe there is a way to reset it with the UCSWRST
    UCB0TXBUF = 0xD0; // the ID register address in BMP280
    sent_tx_byte = true;
  }

  else {
    // after sending the control byte,
    // TX interrupt is triggered again - TX buffer is ready
    // send the stop condition here
    // and finish the transaction
    UCB0CTL1 |= UCTXSTP;  // I2C stop condition
    IFG2 &= ~UCB0TXIFG;   // Clear USCI_B0 TX int flag

    // and exit LPM0
    __bic_SR_register_on_exit(CPUOFF);
  }
}

