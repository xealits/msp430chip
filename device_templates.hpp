#pragma once
#include <type_traits>

#include "bitlogic.hpp"

namespace devices {
template <volatile unsigned int& reg_CTL, volatile unsigned int& reg_CCTL>
struct TimerA {
  TimerA() = delete;

  struct Control {
    using reg_t = std::decay_t<decltype(reg_CTL)>;

    //constexpr static typename mode_control::BaseRegT MODE_STOP = 0, MODE_UPTO = 1,
    //                                                 MODE_CONT = 2, MODE_UPDN = 3;

    enum MODE {MODE_STOP = 0, MODE_UPTO = 1, MODE_CONT = 2, MODE_UPDN = 3};

    using unused11 = bitlogic::BitField<decltype(reg_CTL), reg_CTL, 11, 5>;
    using input_clock = bitlogic::BitField<decltype(reg_CTL), reg_CTL, 8, 3>;

    using clock_divider = bitlogic::BitField<decltype(reg_CTL), reg_CTL, 6, 2>;

    //using mode_control = bitlogic::BitField<decltype(reg_CTL), reg_CTL, 4, 2>;
    struct mode_control : public bitlogic::BitField<decltype(reg_CTL), reg_CTL, 4, 2> {
      // it works in clang without this using, but cl430 does not find BaseRegT and BitFieldT
      using BaseRegT = typename mode_control::BaseRegT;
      using BitFieldT = typename mode_control::BitFieldT;
      static constexpr inline BaseRegT set(MODE value) {
        //return maskValue(static_cast<BaseRegT>(value));
        return BitFieldT::set(static_cast<BaseRegT>(value));
      }
    };

    using unused3 = bitlogic::BitField<decltype(reg_CTL), reg_CTL, 3, 1>;
    using clear = bitlogic::BitField<decltype(reg_CTL), reg_CTL, 2, 1>;
    using taie = bitlogic::BitField<decltype(reg_CTL), reg_CTL, 1, 1>;
    using taifg = bitlogic::BitField<decltype(reg_CTL), reg_CTL, 0, 1>;

    constexpr static typename input_clock::BaseRegT INPUT_CLOCK_TACLK = 0,
                                                    INPUT_CLOCK_ACLK = 1,
                                                    INPUT_CLOCK_SMCLK = 2,
                                                    INPUT_CLOCK_INCLK = 3;

    constexpr static typename clock_divider::BaseRegT DIVIDE_0 = 0, DIVIDE_2 = 1,
                                                      DIVIDE_4 = 2, DIVIDE_8 = 3;

    template <reg_t new_val>
    constexpr static inline void write(void) {
      reg_CTL = new_val;
    }

    constexpr static inline void write(reg_t new_val) {
      reg_CTL = new_val;
    }

    constexpr static inline reg_t read(void) {
      return reg_CTL;
    }
  };

  using CCTL_interrupt_enable =
      bitlogic::BitField<decltype(reg_CCTL), reg_CCTL, 4, 1>;
};

template <volatile unsigned char& p_in, volatile unsigned char& p_out,
          volatile unsigned char& p_dir>
struct Port8bit {
  Port8bit() = delete;
  //
  using reg_type = std::decay_t<decltype(p_in)>;
  enum PINS {
    PIN0 = 0,
    PIN1 = 1,
    PIN2 = 2,
    PIN3 = 3,
    PIN4 = 4,
    PIN5 = 5,
    PIN6 = 6,
    PIN7 = 7
  };

  // template<reg_type directions>
  // constexpr static void setDirecionOutputs(void) {
  // p_dir = directions;
  //}
  template <unsigned... bits>
  constexpr static void setDirecionOutputs(void) {
    p_dir = bitlogic::regMask<reg_type, bits...>();
  }

  template <unsigned... bits>
  constexpr static void toggleOutput(void) {
    p_out ^= bitlogic::regMask<reg_type, bits...>();
  }

  template <unsigned... bits>
  constexpr static void setOutput(void) {
    p_out |= bitlogic::regMask<reg_type, bits...>();
  }

  constexpr static void setOutput(reg_type value) { p_out = value; }

  template <unsigned... bits>
  constexpr static void unsetOutput(void) {
    p_out &= ~bitlogic::regMask<reg_type, bits...>();
  }

  constexpr static reg_type readIn(void) { return p_in; }
};
};  // namespace devices
