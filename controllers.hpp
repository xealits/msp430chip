#pragma once
#include <msp430.h>

#include "device_templates.hpp"
#include "msp430g2553.h"

namespace controllers {
namespace MSP430G2553 {
#if defined(__MSP430G2553__)

using regmaps::CaptureCompareBlockTemplate;
using regmaps::CaptureCompareBlockParams;

using TimerA_0 = regmaps::TimerA<
    TA0CTL, TA0R, TA0IV,
    DevPack<CaptureCompareBlockTemplate
        , CaptureCompareBlockParams<TA0CCTL0, TA0CCR0>
        , CaptureCompareBlockParams<TA0CCTL1, TA0CCR1>
        , CaptureCompareBlockParams<TA0CCTL2, TA0CCR2>
    >>;

using Port1 = regmaps::PortIO8bitI<P1IN, P1OUT, P1DIR,
                                   P1SEL, P1SEL2, P1REN,
                                   P1IFG, P1IES, P1IE>;
//using Port2 = devices::Port8bitI<P2IN, P2OUT, P2DIR, P2IFG, P2IES, P2IE, P2SEL,
//                                 P2SEL2, P2REN>;
//using Port3 = devices::Port8bit<P3IN, P3OUT, P3DIR, P3SEL, P3SEL2, P3REN>;

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
// TODO: figure out how to do it with explicit types and compile-time checks
constexpr unsigned LED1 = controllers::MSP430G2553::Port1::p_out::check_bit<0>();
constexpr unsigned LED_RED = LED1;
//! \brief green LED
constexpr unsigned LED2 = controllers::MSP430G2553::Port1::p_out::check_bit<6>();
constexpr unsigned LED_GREEN = LED2;
};  // namespace MSP_EXP430G2
};  // namespace launchpad_boards
