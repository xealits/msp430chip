#pragma once
#include "bitlogic.hpp"

namespace TiDevices {
    template<volatile unsigned int& reg_CCTL>
    struct TimerA {
        using CCTL_interrupt_enable = BitLogic::BitField<decltype(reg_CCTL), reg_CCTL, 4, 1>;
    };
};
