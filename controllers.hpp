#pragma once
#include "device_templates.hpp"
#include <msp430.h>

namespace TiControllers {
    namespace MSP430G2553 {
        using TimerA_0 = TiDevices::TimerA<TA0CCTL0>;
    };
};
