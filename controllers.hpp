#pragma once
#include <msp430.h>

#include "device_templates.hpp"
#include "msp430g2553.h"

namespace controllers {
namespace MSP430G2553 {
#if defined(__MSP430G2553__)
using TimerA_0 = devices::TimerA<TA0CTL, TA0R, TA0CCTL0, TA0CCR0, TA0CCTL1,
                                 TA0CCR1, TA0CCTL2, TA0CCR2, TA0IV>;
using TimerA_1 = devices::TimerA<TA1CTL, TA1R, TA1CCTL0, TA1CCR0, TA1CCTL1,
                                 TA1CCR1, TA1CCTL2, TA1CCR2, TA1IV>;
using Port1 = devices::Port8bitI<P1IN, P1OUT, P1DIR, P1IFG, P1IES, P1IE, P1SEL,
                                 P1SEL2, P1REN>;
using Port2 = devices::Port8bitI<P2IN, P2OUT, P2DIR, P2IFG, P2IES, P2IE, P2SEL,
                                 P2SEL2, P2REN>;
using Port3 = devices::Port8bit<P3IN, P3OUT, P3DIR, P3SEL, P3SEL2, P3REN>;
#endif
};  // namespace MSP430G2553
};  // namespace controllers

/*! \brief Launchpad development boards
 *
 * A board defines the controller chip and the connections on the board.
 * No external devices yet.
 *
 * Use like:
 * namespace board = LaunchpadBoards::MSP_EXP430G2;
 */
namespace launchpad_boards {

/*! \brief MSP_EXP430G2
 *
 * https://www.ti.com/lit/ug/slau318g/slau318g.pdf?ts=1731831644145
 * https://dev.ti.com/tirex/explore/node?devtools=MSP-EXP430G2&node=A__ABjGDxJw12fXjxtYpSJOow__msp430_devtools__FUz-xrs__LATEST
 */
namespace MSP_EXP430G2 {
namespace controller = controllers::MSP430G2553;

//! \brief red LED
constexpr unsigned LED1 = controllers::MSP430G2553::Port1::PIN0;
//! \brief green LED
constexpr unsigned LED2 = controllers::MSP430G2553::Port1::PIN6;
};  // namespace MSP_EXP430G2
};  // namespace launchpad_boards
