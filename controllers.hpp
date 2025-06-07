#pragma once
#include "device_templates.hpp"
#include "msp430g2553.h"
#include <msp430.h>

namespace controllers {
    namespace MSP430G2553 {
        using TimerA_0 = devices::TimerA<TA0CTL, TA0CCTL0>;
        using Port1 = devices::Port8bit<P1IN, P1OUT, P1DIR>;
    };
};

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
};
};
