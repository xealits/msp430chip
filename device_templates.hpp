
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
    struct interrupt_flag : public BitField<decltype(CapComControl_t), CapComControl_t, 0, 1> {};
    struct capture_overflow : public BitField<decltype(CapComControl_t), CapComControl_t, 1, 1> {};
    struct out_x : public BitField<decltype(CapComControl_t), CapComControl_t, 2, 1> {};
    struct capcom_input : public BitField<decltype(CapComControl_t), CapComControl_t, 3, 1> {};
    struct interrupt_enable : public BitField<decltype(CapComControl_t), CapComControl_t, 4, 1> {};
    struct output_mode : public BitField<decltype(CapComControl_t), CapComControl_t, 5, 3> {};
    struct capture_or_compare : public BitField<decltype(CapComControl_t), CapComControl_t, 8, 1> {};
    struct dummy_0 : public BitField<decltype(CapComControl_t), CapComControl_t, 9, 1> {};
    struct sync_capcom_input : public BitField<decltype(CapComControl_t), CapComControl_t, 10, 1> {};
    struct async_sync_capture : public BitField<decltype(CapComControl_t), CapComControl_t, 11, 1> {};
    struct input_select : public BitField<decltype(CapComControl_t), CapComControl_t, 12, 2> {};
    struct capture_mode : public BitField<decltype(CapComControl_t), CapComControl_t, 14, 2> {};
  };

  struct CapComReg : public Register<decltype(CapComReg_t), CapComReg_t> {};
};

template<volatile unsigned int& Control_t,
         volatile unsigned int& CounterRegister_t,
         volatile unsigned int& InterruptVectorWord_t,
         typename CaptureCompareBlock_t>
struct TimerA {
  TimerA() = delete;

  struct Control : public Register<decltype(Control_t), Control_t> {
    struct taifg : public BitField<decltype(Control_t), Control_t, 0, 1> {};
    struct taie : public BitField<decltype(Control_t), Control_t, 1, 1> {};
    struct clear : public BitField<decltype(Control_t), Control_t, 2, 1> {};
    struct unused3 : public BitField<decltype(Control_t), Control_t, 3, 1> {};
    struct mode_control : public BitField<decltype(Control_t), Control_t, 4, 2> {
      constexpr static typename mode_control::OPT
        MODE_STOP{0},
        MODE_UPTO{1},
        MODE_CONT{2},
        MODE_UPDN{3};
    };
    struct clock_divider : public BitField<decltype(Control_t), Control_t, 6, 2> {
      constexpr static typename clock_divider::OPT
        DIVIDE_0{0},
        DIVIDE_2{1},
        DIVIDE_4{2},
        DIVIDE_8{3};
    };
    struct input_clock : public BitField<decltype(Control_t), Control_t, 8, 3> {
      constexpr static typename input_clock::OPT
        INPUT_CLOCK_TACLK{0},
        INPUT_CLOCK_ACLK{1},
        INPUT_CLOCK_SMCLK{2},
        INPUT_CLOCK_INCLK{3};
    };
    struct unused11 : public BitField<decltype(Control_t), Control_t, 11, 5> {};
  };

  struct CounterRegister : public Register<decltype(CounterRegister_t), CounterRegister_t> {};

  struct InterruptVectorWord : public Register<decltype(InterruptVectorWord_t), InterruptVectorWord_t> {
    struct contents : public BitField<decltype(InterruptVectorWord_t), InterruptVectorWord_t, 1, 4> {
      constexpr static typename contents::OPT
        NO_INTERRUPTS{0},
        CCR1_IFG{1},
        CCR2_IFG{2},
        TA_IFG{5};
    };
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

template<volatile unsigned char& DataTransferControl0_t,
         volatile unsigned char& DataTransferControl1_t,
         volatile unsigned char& AnalogEnable0_t,
         volatile unsigned int& Control0_t,
         volatile unsigned int& Control1_t>
struct ADC10 {
  ADC10() = delete;

  struct DataTransferControl0 : public Register<decltype(DataTransferControl0_t), DataTransferControl0_t> {
    struct fetch : public BitField<decltype(DataTransferControl0_t), DataTransferControl0_t, 0, 1> {};
    struct block_one : public BitField<decltype(DataTransferControl0_t), DataTransferControl0_t, 1, 1> {};
    struct continuous_transfer : public BitField<decltype(DataTransferControl0_t), DataTransferControl0_t, 2, 1> {};
    struct two_block_mode : public BitField<decltype(DataTransferControl0_t), DataTransferControl0_t, 3, 1> {};
  };

  struct DataTransferControl1 : public Register<decltype(DataTransferControl1_t), DataTransferControl1_t> {
    struct fetch : public BitField<decltype(DataTransferControl1_t), DataTransferControl1_t, 0, 1> {};
    struct block_one : public BitField<decltype(DataTransferControl1_t), DataTransferControl1_t, 1, 1> {};
    struct continuous_transfer : public BitField<decltype(DataTransferControl1_t), DataTransferControl1_t, 2, 1> {};
    struct two_block_mode : public BitField<decltype(DataTransferControl1_t), DataTransferControl1_t, 3, 1> {};
  };

  struct AnalogEnable0 : public Register<decltype(AnalogEnable0_t), AnalogEnable0_t> {};

  struct Control0 : public Register<decltype(Control0_t), Control0_t> {
    struct start_conversion : public BitField<decltype(Control0_t), Control0_t, 0, 1> {};
    struct enable_conversion : public BitField<decltype(Control0_t), Control0_t, 1, 1> {};
    struct interrupt_flag : public BitField<decltype(Control0_t), Control0_t, 2, 1> {};
    struct interrupt_enable : public BitField<decltype(Control0_t), Control0_t, 3, 1> {};
    struct on_enable : public BitField<decltype(Control0_t), Control0_t, 4, 1> {};
    struct ref_on : public BitField<decltype(Control0_t), Control0_t, 5, 1> {};
    struct ref_setting : public BitField<decltype(Control0_t), Control0_t, 6, 1> {};
    struct multiple_sample_conversion : public BitField<decltype(Control0_t), Control0_t, 7, 1> {};
    struct ref_burst_mode : public BitField<decltype(Control0_t), Control0_t, 8, 1> {};
    struct enable_output_of_ref : public BitField<decltype(Control0_t), Control0_t, 9, 1> {};
    struct sampling_rate : public BitField<decltype(Control0_t), Control0_t, 10, 1> {};
    struct sample_hold_select : public BitField<decltype(Control0_t), Control0_t, 11, 2> {
      constexpr static typename sample_hold_select::OPT
        SH_x4{0} /** 4 x ADC10 clocks */,
        SH_x8{1} /** 8 x ADC10 clocks */,
        SH_x16{2} /** 16 x ADC10 clocks */,
        SH_x64{3} /** 64 x ADC10 clocks */;
    };
    struct reference_select : public BitField<decltype(Control0_t), Control0_t, 13, 3> {
      constexpr static typename reference_select::OPT
        REF_0{0} /** VR+ = AVCC and VR- = AVSS */,
        REF_1{1} /** VR+ = VREF+ and VR- = AVSS */,
        REF_2{2} /** VR+ = VEREF+ and VR- = AVSS */,
        REF_3{3} /** VR+ = VEREF+ and VR- = AVSS */,
        REF_4{4} /** VR+ = AVCC and VR- = VREF-/VEREF- */,
        REF_5{5} /** VR+ = VREF+ and VR- = VREF-/VEREF- */,
        REF_6{6} /** VR+ = VEREF+ and VR- = VREF-/VEREF- */,
        REF_7{7} /** VR+ = VEREF+ and VR- = VREF-/VEREF- */;
    };
  };

  struct Control1 : public Register<decltype(Control1_t), Control1_t> {
    struct busy : public BitField<decltype(Control1_t), Control1_t, 0, 1> {};
    struct conversion_sequence : public BitField<decltype(Control1_t), Control1_t, 1, 2> {
      constexpr static typename conversion_sequence::OPT
        SingleChannelSingleConversion{0},
        SequenceOfChannels{1},
        RepeatSingleChannel{2},
        RepeatSequenceOfChannels{3};
    };
    struct clock_source : public BitField<decltype(Control1_t), Control1_t, 3, 2> {
      constexpr static typename clock_source::OPT
        ADC10OSC{0},
        ACLK{1},
        MCLK{2},
        SMCLK{3};
    };
    struct clock_divider : public BitField<decltype(Control1_t), Control1_t, 5, 3> {
      constexpr static typename clock_divider::OPT
        DIV_0{0},
        DIV_1{1},
        DIV_2{2},
        DIV_3{3},
        DIV_4{4},
        DIV_5{5},
        DIV_6{6},
        DIV_7{7};
    };
    struct invert_sample_hold : public BitField<decltype(Control1_t), Control1_t, 8, 1> {};
    /// 0 = binary, 1 = 2's complement
    struct data_format : public BitField<decltype(Control1_t), Control1_t, 9, 1> {};
    struct sample_hold_source : public BitField<decltype(Control1_t), Control1_t, 10, 2> {
      constexpr static typename sample_hold_source::OPT
        ADC10OSC{0},
        TA3_OUT1{1},
        TA3_OUT0{2},
        TA3_OUT2{3};
    };
    struct input_channel : public BitField<decltype(Control1_t), Control1_t, 12, 4> {
      constexpr static typename input_channel::OPT
        CH_0{0},
        CH_1{1},
        CH_2{2},
        CH_3{3},
        CH_4{4},
        CH_5{5},
        CH_6{6},
        CH_7{7},
        CH_8{8},
        CH_9{9},
        CH_10{10},
        CH_11{11},
        CH_12{12},
        CH_13{13},
        CH_14{14},
        CH_15{15};
    };
  };
};

template<volatile unsigned char& DCOClockFrequencyControl_t,
         volatile unsigned char& Control1_t,
         volatile unsigned char& Control2_t,
         volatile unsigned char& Control3_t>
struct BasicClockModule {
  BasicClockModule() = delete;

  struct DCOClockFrequencyControl : public Register<decltype(DCOClockFrequencyControl_t), DCOClockFrequencyControl_t> {
    struct Modulation : public BitField<decltype(DCOClockFrequencyControl_t), DCOClockFrequencyControl_t, 0, 5> {};
    struct DCOSelect : public BitField<decltype(DCOClockFrequencyControl_t), DCOClockFrequencyControl_t, 5, 3> {};
  };

  struct Control1 : public Register<decltype(Control1_t), Control1_t> {
    struct Range : public BitField<decltype(Control1_t), Control1_t, 0, 4> {};
    struct ACLKDivider : public BitField<decltype(Control1_t), Control1_t, 4, 2> {
      constexpr static typename ACLKDivider::OPT
        DIV_x1{0},
        DIV_x2{1},
        DIV_x4{2},
        DIV_x8{3};
    };
    struct XTSelect : public BitField<decltype(Control1_t), Control1_t, 6, 1> {
      constexpr static typename XTSelect::OPT
        LOW_FREQUENCY{0},
        HIGH_FREQUENCY{1};
    };
    /// Enable XT2CLK
    struct XT2Off : public BitField<decltype(Control1_t), Control1_t, 7, 1> {};
  };

  struct Control2 : public Register<decltype(Control2_t), Control2_t> {
    struct SMCLKDivider : public BitField<decltype(Control2_t), Control2_t, 0, 2> {
      constexpr static typename SMCLKDivider::OPT
        DIV_x1{0},
        DIV_x2{1},
        DIV_x4{2},
        DIV_x8{3};
    };
    struct SMCLKSource : public BitField<decltype(Control2_t), Control2_t, 2, 1> {
      constexpr static typename SMCLKSource::OPT
        DCOCLK{0},
        XT2CLK_LFXTCLK{1};
    };
    struct MCLKDivider : public BitField<decltype(Control2_t), Control2_t, 3, 2> {
      constexpr static typename MCLKDivider::OPT
        DIV_x1{0},
        DIV_x2{1},
        DIV_x4{2},
        DIV_x8{3};
    };
    struct MCLKSource : public BitField<decltype(Control2_t), Control2_t, 5, 2> {
      constexpr static typename MCLKSource::OPT
        DCOCLK{1},
        XT2CLK_LFXTCLK{2},
        LFXTCLK{3};
    };
  };

  struct Control3 : public Register<decltype(Control3_t), Control3_t> {
    /// Low/high frequency oscillator fault flag
    struct LFXT_OscillatorFaultFlag : public BitField<decltype(Control3_t), Control3_t, 0, 1> {};
    /// High frequency oscillator 2 fault flag
    struct XT_OscillatorFaultFlag : public BitField<decltype(Control3_t), Control3_t, 1, 1> {};
    struct XIN_XOUT_Cap : public BitField<decltype(Control3_t), Control3_t, 2, 2> {
      constexpr static typename XIN_XOUT_Cap::OPT
        XCAP_0pF{0},
        XCAP_6pF{1},
        XCAP_10pF{2},
        XCAP_12pF5{3};
    };
    /// (XTS = 0)
    struct LFXT1_Mode : public BitField<decltype(Control3_t), Control3_t, 4, 2> {
      constexpr static typename LFXT1_Mode::OPT
        NORMAL_OPERATION{0},
        Reserved{1},
        VLO{2},
        DIGITAL_INPUT_SIGNAL{3};
    };
    struct XT2_Mode : public BitField<decltype(Control3_t), Control3_t, 6, 2> {
      constexpr static typename XT2_Mode::OPT
        MODE_0p4_1_MHz{0},
        MODE_1_4_MHz{1},
        MODE_2_16_MHz{2},
        DIGITAL_INPUT_SIGNAL{3};
    };
  };
};

template<volatile unsigned char& Control1_t,
         volatile unsigned char& Control2_t,
         volatile unsigned char& PortDisable_t>
struct ComparatorA {
  ComparatorA() = delete;

  struct Control1 : public Register<decltype(Control1_t), Control1_t> {
    struct InterruptFlag : public BitField<decltype(Control1_t), Control1_t, 0, 1> {};
    struct InterruptEnable : public BitField<decltype(Control1_t), Control1_t, 1, 1> {};
    struct EdgeSelect : public BitField<decltype(Control1_t), Control1_t, 2, 1> {
      constexpr static typename EdgeSelect::OPT
        RISING{0},
        FALLING{1};
    };
    struct Enable : public BitField<decltype(Control1_t), Control1_t, 3, 1> {};
    struct InternalReference : public BitField<decltype(Control1_t), Control1_t, 4, 2> {
      constexpr static typename InternalReference::OPT
        OFF{0},
        REF_0p25Vcc{1},
        REF_0p5Vcc{2},
        REF_Vt{3};
    };
    struct InternalReferenceEnable : public BitField<decltype(Control1_t), Control1_t, 6, 1> {};
    struct ExchangeInputs : public BitField<decltype(Control1_t), Control1_t, 7, 1> {};
  };

  struct Control2 : public Register<decltype(Control2_t), Control2_t> {
    struct ComparatorAOutput : public BitField<decltype(Control2_t), Control2_t, 0, 1> {};
    struct EnableOutputFilter : public BitField<decltype(Control2_t), Control2_t, 1, 1> {};
    struct TerminalMultiplexer : public BitField<decltype(Control2_t), Control2_t, 2, 5> {};
    struct ShortTerminals : public BitField<decltype(Control2_t), Control2_t, 7, 1> {};
  };

  struct PortDisable : public Register<decltype(PortDisable_t), PortDisable_t> {
    struct InputBuffersOfPortRegister : public BitField<decltype(PortDisable_t), PortDisable_t, 0, 8> {};
  };
};

template<volatile unsigned int& Control1_t,
         volatile unsigned int& Control2_t>
struct FlashMemoryModule {
  FlashMemoryModule() = delete;

  struct Control1 : public Register<decltype(Control1_t), Control1_t> {
    struct Reserved0 : public BitField<decltype(Control1_t), Control1_t, 0, 1> {};
    struct Erase_MassErase : public BitField<decltype(Control1_t), Control1_t, 1, 2> {
      constexpr static typename Erase_MassErase::OPT
        NO_ERASE{0},
        SEGMENT_ERASE{1},
        BANK_ERASE{2} /** erase one bank */,
        MASS_ERASE{3} /** erase all flash memory banks */;
    };
    struct Reserved3 : public BitField<decltype(Control1_t), Control1_t, 3, 2> {};
    /// If this bit is set, the program time is shortened. The programming quality has to be checked by marginal read modes.
    struct SmartWrite : public BitField<decltype(Control1_t), Control1_t, 5, 1> {};
    struct Write_BlockWrite : public BitField<decltype(Control1_t), Control1_t, 6, 2> {
      constexpr static typename Write_BlockWrite::OPT
        Reserved{0},
        BYTE_OR_WORD_WRITE{1},
        LONG_WORD_WRITE{2},
        LONG_WORD_BLOCK_WRITE{3};
    };
    /// Always reads as 096h (0x96 = 150). Must be written as 0A5h (0xA5 = 165) or a PUC is generated.
    struct ControlPassword : public BitField<decltype(Control1_t), Control1_t, 8, 8> {};
  };

  struct Control2 : public Register<decltype(Control2_t), Control2_t> {
    /// Divide Flash clock by 1 to 64 using these (FN0-FN5) bits as:
    /// 32\*FN5 + 16\*FN4 + 8\*FN3 + 4\*FN2 + 2\*FN1 + FN0 + 1.
    /// So, it is just the field + 1.
    struct DivideClock : public BitField<decltype(Control2_t), Control2_t, 0, 6> {};
    struct ClockSelect : public BitField<decltype(Control2_t), Control2_t, 6, 2> {
      constexpr static typename ClockSelect::OPT
        ACLK{0},
        MCLK{1},
        SMCLK{3};
    };
    /// Always reads as 096h (0x96 = 150). Must be written as 0A5h (0xA5 = 165) or a PUC is generated.
    struct ControlPassword : public BitField<decltype(Control2_t), Control2_t, 8, 8> {};
  };
};

};

