#pragma once
#include <type_traits>

#include "bitlogic.hpp"

namespace devices {
using bitlogic::BitField;
using bitlogic::Register;

template <volatile unsigned& reg_CTL, volatile unsigned& reg_TAR,
          volatile unsigned& reg_CCTL0, volatile unsigned& reg_CCR0,
          volatile unsigned& reg_CCTL1, volatile unsigned& reg_CCR1,
          volatile unsigned& reg_CCTL2, volatile unsigned& reg_CCR2,
          volatile unsigned& reg_TAIV>
// the manual lists 5 CCTLs: 0-4. So, there may be 5 in some devices?
// I probably should have a variadic template here
struct TimerA {
  TimerA() = delete;

  struct Control : public Register<decltype(reg_CTL), reg_CTL> {
    using taifg = BitField<decltype(reg_CTL), reg_CTL, 0, 1>;
    using taie = BitField<decltype(reg_CTL), reg_CTL, 1, 1>;
    using clear = BitField<decltype(reg_CTL), reg_CTL, 2, 1>;
    using unused3 = BitField<decltype(reg_CTL), reg_CTL, 3, 1>;

    using mode_control = BitField<decltype(reg_CTL), reg_CTL, 4, 2>;
    constexpr static typename mode_control::OPT MODE_STOP{0}, MODE_UPTO{1},
        MODE_CONT{2}, MODE_UPDN{3};

    using clock_divider = BitField<decltype(reg_CTL), reg_CTL, 6, 2>;
    constexpr static typename clock_divider::OPT DIVIDE_0{0}, DIVIDE_2{1},
        DIVIDE_4{2}, DIVIDE_8{3};

    using input_clock = BitField<decltype(reg_CTL), reg_CTL, 8, 3>;
    constexpr static typename input_clock::OPT INPUT_CLOCK_TACLK{0},
        INPUT_CLOCK_ACLK{1}, INPUT_CLOCK_SMCLK{2}, INPUT_CLOCK_INCLK{3};

    using unused11 = BitField<decltype(reg_CTL), reg_CTL, 11, 5>;
  };

  struct TimerReg : public Register<decltype(reg_TAR), reg_TAR> {};

  template <typename RegCCTL_t, RegCCTL_t& reg_CCTL>
  struct CapComControl : public Register<RegCCTL_t, reg_CCTL> {
    using interrupt_flag = BitField<RegCCTL_t, reg_CCTL, 0, 1>;
    using capture_overflow = BitField<RegCCTL_t, reg_CCTL, 1, 1>;
    using out_x = BitField<RegCCTL_t, reg_CCTL, 2, 1>;
    using capcom_input = BitField<RegCCTL_t, reg_CCTL, 3, 1>;

    using interrupt_enable = BitField<RegCCTL_t, reg_CCTL, 4, 1>;
    using output_mode = BitField<RegCCTL_t, reg_CCTL, 5, 3>;

    using capture_or_compare = BitField<RegCCTL_t, reg_CCTL, 8, 1>;
    using dummy_0 = BitField<RegCCTL_t, reg_CCTL, 9, 1>;
    using sync_capcom_input = BitField<RegCCTL_t, reg_CCTL, 10, 1>;
    using async_sync_capture = BitField<RegCCTL_t, reg_CCTL, 11, 1>;

    using input_select = BitField<RegCCTL_t, reg_CCTL, 12, 2>;
    using capture_mode = BitField<RegCCTL_t, reg_CCTL, 14, 2>;
  };

  using CapComControl0 = CapComControl<decltype(reg_CCTL0), reg_CCTL0>;
  using CapComReg0 = Register<decltype(reg_CCR0), reg_CCR0>;

  using CapComControl1 = CapComControl<decltype(reg_CCTL1), reg_CCTL1>;
  using CapComReg1 = Register<decltype(reg_CCR1), reg_CCR1>;

  using CapComControl2 = CapComControl<decltype(reg_CCTL2), reg_CCTL2>;
  using CapComReg2 = Register<decltype(reg_CCR2), reg_CCR2>;

  // using CCTL_interrupt_enable =
  //     bitlogic::BitField<decltype(reg_CCTL0), reg_CCTL0, 4, 1>;

  struct InterruptVector : public Register<decltype(reg_TAIV), reg_TAIV> {
    using contents = BitField<decltype(reg_TAIV), reg_TAIV, 1, 4>;
  };
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
