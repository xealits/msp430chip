#pragma once
#include "device_templates.hpp"
#include "msp430g2553.h"
#include <msp430.h>

namespace TiControllers {
    namespace MSP430G2553 {
        using TimerA_0 = TiDevices::TimerA<TA0CTL, TA0CCTL0>;
        using Port1 = TiDevices::Port8bit<P1IN, P1OUT, P1DIR>;
    };
};
