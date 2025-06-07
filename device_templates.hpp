#pragma once
#include "bitlogic.hpp"
#include <type_traits>

namespace TiDevices {
    template<
        volatile unsigned int& reg_CTL,
        volatile unsigned int& reg_CCTL>
    struct TimerA {
        using reg_CTL_type = std::decay_t<decltype(reg_CTL)>;
        using input_clock = BitLogic::BitField<decltype(reg_CTL), reg_CTL, 8, 3>;
        constexpr static typename input_clock::BaseRegT
            INPUT_CLOCK_TACLK = 0,
            INPUT_CLOCK_ACLK  = 1,
            INPUT_CLOCK_SMCLK = 2,
            INPUT_CLOCK_INCLK = 3;

        using clock_divider = BitLogic::BitField<decltype(reg_CTL), reg_CTL, 6, 2>;
        constexpr static typename clock_divider::BaseRegT
            DIVIDE_0 = 0,
            DIVIDE_2 = 1,
            DIVIDE_4 = 2,
            DIVIDE_8 = 3;

        using mode_select = BitLogic::BitField<decltype(reg_CTL), reg_CTL, 4, 2>;
        constexpr static typename mode_select::BaseRegT
            MODE_STOP = 0,
            MODE_UPTO = 1,
            MODE_CONT = 2,
            MODE_UPDN = 3;

        template<reg_CTL_type new_val>
        constexpr static inline void control_write(void) {
            reg_CTL = new_val;
        }

        using CCTL_interrupt_enable = BitLogic::BitField<decltype(reg_CCTL), reg_CCTL, 4, 1>;
    };

    template<
        volatile unsigned char& p_in,
        volatile unsigned char& p_out,
        volatile unsigned char& p_dir
        >
    struct Port8bit {
        //
        using reg_type = std::decay_t<decltype(p_in)>;

        //template<reg_type directions>
        //constexpr static void setDirecionOutputs(void) {
            //p_dir = directions;
        //}
        template<unsigned... bits>
        constexpr static void setDirecionOutputs(void) {
            p_dir = BitLogic::regMask<reg_type, bits...>();
        }

        template<unsigned... bits>
        constexpr static void toggleOutput(void) {
            p_out ^= BitLogic::regMask<reg_type, bits...>();
        }

        template<unsigned... bits>
        constexpr static void setOutput(void) {
            p_out |= BitLogic::regMask<reg_type, bits...>();
        }

        constexpr static void setOutput(reg_type value) {
            p_out = value;
        }

        template<unsigned... bits>
        constexpr static void unsetOutput(void) {
            p_out &= ~BitLogic::regMask<reg_type, bits...>();
        }

        constexpr static reg_type readIn(void) {
            return p_in;
        }
    };
};
