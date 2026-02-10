#pragma once
#include <msp430.h>

#include "device_templates.hpp"
#include "msp430chip/bitlogic.hpp"
#include "msp430g2553.h"

namespace controllers {
using bitlogic::DevPack;
using bitlogic::Register;
using bitlogic::BitField;
using regmaps::CaptureCompareBlockTemplate;
using regmaps::CaptureCompareBlockParams;

#if defined(__MSP430G2553__)
namespace MSP430G2553 {

    using TimerA_0 = regmaps::TimerA<
        TA0CTL, TA0R, TA0IV,
        DevPack<CaptureCompareBlockTemplate
            , CaptureCompareBlockParams<TA0CCTL0, TA0CCR0>
            , CaptureCompareBlockParams<TA0CCTL1, TA0CCR1>
            , CaptureCompareBlockParams<TA0CCTL2, TA0CCR2>
        >>;

    using TimerA_1 = regmaps::TimerA<
        TA1CTL, TA1R, TA1IV,
        DevPack<CaptureCompareBlockTemplate
            , CaptureCompareBlockParams<TA1CCTL0, TA1CCR0>
            , CaptureCompareBlockParams<TA1CCTL1, TA1CCR1>
            , CaptureCompareBlockParams<TA1CCTL2, TA1CCR2>
        >>;

    using Port1 = regmaps::PortIO8bitI<P1IN, P1OUT, P1DIR,
                                    P1SEL, P1SEL2, P1REN,
                                    P1IFG, P1IES, P1IE>;
    using Port2 = regmaps::PortIO8bitI<P2IN, P2OUT, P2DIR,
                                    P2SEL, P2SEL2, P2REN,
                                    P2IFG, P2IES, P2IE>;
    using Port3 = regmaps::PortIO8bit<P3IN, P3OUT, P3DIR,
                                    P3SEL, P3SEL2, P3REN>;

    using FlashMemoryModule = regmaps::FlashMemoryModule<FCTL1, FCTL2, FCTL3>;

    /// Special Function Registers
    namespace SFRs {
        struct InterruptEnable1 : public Register<decltype(IE1), IE1> {
            using Watchdog = BitField<decltype(IE1), IE1, 0, 1>;
            using OscFault = BitField<decltype(IE1), IE1, 1, 1>;
            using NMI = BitField<decltype(IE1), IE1, 4, 1>;
            using FlashAccessViolation = BitField<decltype(IE1), IE1, 5, 1>;
        };

        struct InterruptFlag1 : public Register<decltype(IFG1), IFG1> {
            using Watchdog = BitField<decltype(IFG1), IFG1, 0, 1>;
            using OscFault = BitField<decltype(IFG1), IFG1, 1, 1>;
            using PowerOn = BitField<decltype(IFG1), IFG1, 2, 1>;
            using Reset = BitField<decltype(IFG1), IFG1, 3, 1>;
            using NMI = BitField<decltype(IFG1), IFG1, 4, 1>;
        };

        struct InterruptEnable2 : public Register<decltype(IE2), IE2> {
            using UCA0RX = BitField<decltype(IE2), IE2, 0, 1>;
            using UCA0TX = BitField<decltype(IE2), IE2, 1, 1>;
            using UCB0RX = BitField<decltype(IE2), IE2, 2, 1>;
            using UCB0TX = BitField<decltype(IE2), IE2, 3, 1>;
        };

        struct InterruptFlag2 : public Register<decltype(IFG2), IFG2> {
            using UCA0RX = BitField<decltype(IFG2), IFG2, 0, 1>;
            using UCA0TX = BitField<decltype(IFG2), IFG2, 1, 1>;
            using UCB0RX = BitField<decltype(IFG2), IFG2, 2, 1>;
            using UCB0TX = BitField<decltype(IFG2), IFG2, 3, 1>;
        };
    };

};  // namespace MSP430G2553
#endif
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
