
#pragma once

#include <cstdint>
#include "bitlogic.hpp"

namespace regmaps {
using bitlogic::BitField;
using bitlogic::Register;


template<volatile unsigned int& CapComControl_t,
         volatile unsigned int& CapComReg_t>
struct CaptureCompareBlockParams {};

template<typename CaptureCompareBlockParams_t>
struct CaptureCompareBlockTemplate;

template<volatile unsigned int& CapComControl_t,
         volatile unsigned int& CapComReg_t>
struct CaptureCompareBlockTemplate<CaptureCompareBlockParams<CapComControl_t, CapComReg_t>> {
  CaptureCompareBlockTemplate() = delete;

  struct CapComControl : public Register<decltype(CapComControl_t), CapComControl_t> {
    using interrupt_flag = BitField<decltype(CapComControl_t), CapComControl_t, 0, 1>;
    using capture_overflow = BitField<decltype(CapComControl_t), CapComControl_t, 1, 1>;
    using out_x = BitField<decltype(CapComControl_t), CapComControl_t, 2, 1>;
    using capcom_input = BitField<decltype(CapComControl_t), CapComControl_t, 3, 1>;
    using interrupt_enable = BitField<decltype(CapComControl_t), CapComControl_t, 4, 1>;
    using output_mode = BitField<decltype(CapComControl_t), CapComControl_t, 5, 3>;
    using capture_or_compare = BitField<decltype(CapComControl_t), CapComControl_t, 8, 1>;
    using dummy_0 = BitField<decltype(CapComControl_t), CapComControl_t, 9, 1>;
    using sync_capcom_input = BitField<decltype(CapComControl_t), CapComControl_t, 10, 1>;
    using async_sync_capture = BitField<decltype(CapComControl_t), CapComControl_t, 11, 1>;
    using input_select = BitField<decltype(CapComControl_t), CapComControl_t, 12, 2>;
    using capture_mode = BitField<decltype(CapComControl_t), CapComControl_t, 14, 2>;
  };

  struct CapComReg : public Register<decltype(CapComReg_t), CapComReg_t> {};
};

template<volatile unsigned int& Control_t,
         volatile unsigned int& TimerReg_t,
         volatile unsigned int& InterruptVector_t,
         typename CaptureCompareBlock_t>
struct TimerA {
  TimerA() = delete;

  struct Control : public Register<decltype(Control_t), Control_t> {
    using taifg = BitField<decltype(Control_t), Control_t, 0, 1>;
    using taie = BitField<decltype(Control_t), Control_t, 1, 1>;
    using clear = BitField<decltype(Control_t), Control_t, 2, 1>;
    using unused3 = BitField<decltype(Control_t), Control_t, 3, 1>;
    using mode_control = BitField<decltype(Control_t), Control_t, 4, 2>;
    constexpr static typename mode_control::OPT
      MODE_STOP{0},
      MODE_UPTO{1},
      MODE_CONT{2},
      MODE_UPDN{3};
    using clock_divider = BitField<decltype(Control_t), Control_t, 6, 2>;
    constexpr static typename clock_divider::OPT
      DIVIDE_0{0},
      DIVIDE_2{1},
      DIVIDE_4{2},
      DIVIDE_8{3};
    using input_clock = BitField<decltype(Control_t), Control_t, 8, 3>;
    constexpr static typename input_clock::OPT
      INPUT_CLOCK_TACLK{0},
      INPUT_CLOCK_ACLK{1},
      INPUT_CLOCK_SMCLK{2},
      INPUT_CLOCK_INCLK{3};
    using unused11 = BitField<decltype(Control_t), Control_t, 11, 5>;
  };

  struct TimerReg : public Register<decltype(TimerReg_t), TimerReg_t> {};

  struct InterruptVector : public Register<decltype(InterruptVector_t), InterruptVector_t> {
    using contents = BitField<decltype(InterruptVector_t), InterruptVector_t, 1, 4>;
  };

  template<unsigned dev_i>
  using CaptureCompareBlock = typename CaptureCompareBlock_t::template DevByIndex<dev_i>;
};

template<volatile unsigned char& p_in_t,
         volatile unsigned char& p_out_t,
         volatile unsigned char& p_dir_t,
         volatile unsigned char& p_sel_t,
         volatile unsigned char& p_sel2_t,
         volatile unsigned char& p_en_t>
struct PortIO8bit {
  PortIO8bit() = delete;

  struct p_in : public Register<decltype(p_in_t), p_in_t> {};

  struct p_out : public Register<decltype(p_out_t), p_out_t> {};

  struct p_dir : public Register<decltype(p_dir_t), p_dir_t> {};

  struct p_sel : public Register<decltype(p_sel_t), p_sel_t> {};

  struct p_sel2 : public Register<decltype(p_sel2_t), p_sel2_t> {};

  struct p_en : public Register<decltype(p_en_t), p_en_t> {};
};

template<volatile unsigned char& p_in_t,
         volatile unsigned char& p_out_t,
         volatile unsigned char& p_dir_t,
         volatile unsigned char& p_sel_t,
         volatile unsigned char& p_sel2_t,
         volatile unsigned char& p_en_t,
         volatile unsigned char& p_ifg_t,
         volatile unsigned char& p_ies_t,
         volatile unsigned char& p_ie_t>
struct PortIO8bitI {
  PortIO8bitI() = delete;

  struct p_in : public Register<decltype(p_in_t), p_in_t> {};

  struct p_out : public Register<decltype(p_out_t), p_out_t> {};

  struct p_dir : public Register<decltype(p_dir_t), p_dir_t> {};

  struct p_sel : public Register<decltype(p_sel_t), p_sel_t> {};

  struct p_sel2 : public Register<decltype(p_sel2_t), p_sel2_t> {};

  struct p_en : public Register<decltype(p_en_t), p_en_t> {};

  struct p_ifg : public Register<decltype(p_ifg_t), p_ifg_t> {};

  struct p_ies : public Register<decltype(p_ies_t), p_ies_t> {};

  struct p_ie : public Register<decltype(p_ie_t), p_ie_t> {};
};

};

