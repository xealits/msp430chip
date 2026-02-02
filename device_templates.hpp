#pragma once
#include <array>
#include <type_traits>

#include "bitlogic.hpp"
#include "msp430g2553.h"

// Type-indexed device type
//template<unsigned dev_n, typename DevT>
//struct IndexedDev : public DevT { };

// TypesPack
template <unsigned dev_i, template <typename> class DevTemplate, typename... RegRefTs>
struct IndexedDevPack;

template <unsigned dev_i, template <typename> class DevTemplate, typename FirstRef, typename... RestRefs>
struct IndexedDevPack<dev_i, DevTemplate, FirstRef, RestRefs...> {
    static_assert(dev_i > 0, "Index out of bounds");
    struct Dev : public IndexedDevPack<dev_i, DevTemplate, RestRefs...>::Dev {};

};

template <template <typename> class DevTemplate, typename FirstRef, typename... RestRefs>
struct IndexedDevPack<0, DevTemplate, FirstRef, RestRefs...> {
    struct Dev : public DevTemplate<FirstRef> {};
};

template <template <typename> class DevTemplate, typename... RegRefTs>
struct DevPack {
    template<unsigned dev_i>
    struct Dev : public IndexedDevPack<dev_i, DevTemplate, RegRefTs...>::Dev {};
};

namespace devices {
using bitlogic::BitField;
using bitlogic::Register;

// TODO g2553 devices and registers from the header msp430g2553.h
// here or in controllers.hpp
// status register? with the named bits, like C, Z, etc
// LPM bits -- no need for any Cpp wrapping?
// special function registers -- interrupt enables and flags
// Interrupt vectors -- also nothing to do with them
// ADC10 device
// Basic Clock Module
// Comparator A
// Flash Memory
// finish DIO
// finish Timer A
// USCI - I2C, SPI and UART modes
// Watchdog Timer
// Clock and other calibration data info

template<typename RegCCTL_t, RegCCTL_t& RegCCTL, typename RegCCR_t, RegCCR_t& RegCCR>
struct CaptureCompareBlockRegs {};

template<typename CaptureCompareBlockRegs_t>
struct CaptureCompareBlockTemplate;

template<typename RegCCTL_t, RegCCTL_t& RegCCTL, typename RegCCR_t, RegCCR_t& RegCCR>
struct CaptureCompareBlockTemplate<CaptureCompareBlockRegs<RegCCTL_t, RegCCTL, RegCCR_t, RegCCR>> {

  struct CapComControl : public Register<RegCCTL_t, RegCCTL> {
    using interrupt_flag = BitField<RegCCTL_t, RegCCTL, 0, 1>;
    using capture_overflow = BitField<RegCCTL_t, RegCCTL, 1, 1>;
    using out_x = BitField<RegCCTL_t, RegCCTL, 2, 1>;
    using capcom_input = BitField<RegCCTL_t, RegCCTL, 3, 1>;

    using interrupt_enable = BitField<RegCCTL_t, RegCCTL, 4, 1>;
    using output_mode = BitField<RegCCTL_t, RegCCTL, 5, 3>;

    using capture_or_compare = BitField<RegCCTL_t, RegCCTL, 8, 1>;
    using dummy_0 = BitField<RegCCTL_t, RegCCTL, 9, 1>;
    using sync_capcom_input = BitField<RegCCTL_t, RegCCTL, 10, 1>;
    using async_sync_capture = BitField<RegCCTL_t, RegCCTL, 11, 1>;

    using input_select = BitField<RegCCTL_t, RegCCTL, 12, 2>;
    using capture_mode = BitField<RegCCTL_t, RegCCTL, 14, 2>;
  };

  struct CapComReg : public Register<RegCCR_t, RegCCR> {};
};


/// \brief TimerA template with variable number of Capture/Compare blocks
template <volatile unsigned& reg_CTL, volatile unsigned& reg_TAR,
          typename CaptureCompareBlocks_t,
          volatile unsigned& reg_TAIV
          //, unsigned n_ccrx = 0
          >
// the manual lists 5 CCTLs: 0-4. So, there may be 5 in some devices?
// I probably should have a variadic template here
struct TimerA2 {
  TimerA2() = delete;

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

  using ccblocks = CaptureCompareBlocks_t;

  struct InterruptVector : public Register<decltype(reg_TAIV), reg_TAIV> {
    using contents = BitField<decltype(reg_TAIV), reg_TAIV, 1, 4>;
  };
};

/// \brief TimerA template
/// TimerA has a variable number (2-3) of Capture/Compare blocks.
/// Each block is operated by a TACCTLx Control register
/// and a TACCRs setting register.

template <volatile unsigned& reg_CTL, volatile unsigned& reg_TAR,
          volatile unsigned& reg_CCTL0, volatile unsigned& reg_CCR0,
          volatile unsigned& reg_CCTL1, volatile unsigned& reg_CCR1,
          volatile unsigned& reg_CCTL2, volatile unsigned& reg_CCR2,
          volatile unsigned& reg_TAIV
          //, unsigned n_ccrx = 0
          >
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
  //constexpr 

  //struct CapComBlock {
    //Register2<decltype(TA0CCTL0)> cctl;
    //Register2<decltype(TA0CCR0)> cr;
  //};

  // cannot define it outside the namespace
  //static std::array<CapComBlock, n_ccrx> capcoms;
  //using capcoms_t = decltype(capcoms);

  // using CCTL_interrupt_enable =
  //     bitlogic::BitField<decltype(reg_CCTL0), reg_CCTL0, 4, 1>;

  struct InterruptVector : public Register<decltype(reg_TAIV), reg_TAIV> {
    using contents = BitField<decltype(reg_TAIV), reg_TAIV, 1, 4>;
  };
};

//! \brief basic digital IO port
template <volatile unsigned char& p_in, volatile unsigned char& p_out,
          volatile unsigned char& p_dir, volatile unsigned char& p_sel,
          volatile unsigned char& p_sel2, volatile unsigned char& p_en>
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

//! \brief digital IO port with external interrupt
template <volatile unsigned char& p_in, volatile unsigned char& p_out,
          volatile unsigned char& p_dir, volatile unsigned char& p_ifg,
          volatile unsigned char& p_ies, volatile unsigned char& p_ie,
          volatile unsigned char& p_sel, volatile unsigned char& p_sel2,
          volatile unsigned char& p_en>
struct Port8bitI : public Port8bit<p_in, p_out, p_dir, p_sel, p_sel2, p_en> {
  Port8bitI() = delete;
};
};  // namespace devices
