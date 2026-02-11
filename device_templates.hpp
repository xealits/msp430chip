
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

template<>
struct PortIO8bit {
  PortIO8bit() = delete;


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
         volatile unsigned int& Control2_t,
         volatile unsigned int& Control3_t>
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

  struct Control3 : public Register<decltype(Control3_t), Control3_t> {
    struct Busy : public BitField<decltype(Control3_t), Control3_t, 0, 1> {};
    struct FlashKeyViolationFlag : public BitField<decltype(Control3_t), Control3_t, 1, 1> {};
    struct FlashAccessViolationFlag : public BitField<decltype(Control3_t), Control3_t, 2, 1> {};
    /// for segment write
    struct WaitFlag : public BitField<decltype(Control3_t), Control3_t, 3, 1> {};
    /// locked = read only
    struct LockBit : public BitField<decltype(Control3_t), Control3_t, 4, 1> {};
    struct FlashEmergencyExit : public BitField<decltype(Control3_t), Control3_t, 5, 1> {};
    /// locked = read only
    struct LockBitSegmentA : public BitField<decltype(Control3_t), Control3_t, 6, 1> {};
    /// last program or erase failed
    struct Fail : public BitField<decltype(Control3_t), Control3_t, 7, 1> {};
    /// Always reads as 096h (0x96 = 150). Must be written as 0A5h (0xA5 = 165) or a PUC is generated.
    struct ControlPassword : public BitField<decltype(Control3_t), Control3_t, 8, 8> {};
  };
};

template<volatile unsigned int& Control1_t,
         volatile unsigned int& Control2_t,
         volatile unsigned int& Control3_t,
         volatile unsigned int& Control4_t>
struct FlashMemoryModule4 {
  FlashMemoryModule4() = delete;

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

  struct Control3 : public Register<decltype(Control3_t), Control3_t> {
    struct Busy : public BitField<decltype(Control3_t), Control3_t, 0, 1> {};
    struct FlashKeyViolationFlag : public BitField<decltype(Control3_t), Control3_t, 1, 1> {};
    struct FlashAccessViolationFlag : public BitField<decltype(Control3_t), Control3_t, 2, 1> {};
    /// for segment write
    struct WaitFlag : public BitField<decltype(Control3_t), Control3_t, 3, 1> {};
    /// locked = read only
    struct LockBit : public BitField<decltype(Control3_t), Control3_t, 4, 1> {};
    struct FlashEmergencyExit : public BitField<decltype(Control3_t), Control3_t, 5, 1> {};
    /// locked = read only
    struct LockBitSegmentA : public BitField<decltype(Control3_t), Control3_t, 6, 1> {};
    /// last program or erase failed
    struct Fail : public BitField<decltype(Control3_t), Control3_t, 7, 1> {};
    /// Always reads as 096h (0x96 = 150). Must be written as 0A5h (0xA5 = 165) or a PUC is generated.
    struct ControlPassword : public BitField<decltype(Control3_t), Control3_t, 8, 8> {};
  };

  struct Control4 : public Register<decltype(Control4_t), Control4_t> {
    /// This bit is set by hardware and can only be cleared by software.
    ///   If DVCC changed significantly during programming,
    ///   this bit is set to indicate an invalid result.
    struct VoltageChangedDuringProgramError : public BitField<decltype(Control4_t), Control4_t, 0, 1> {};
    struct Reserved1 : public BitField<decltype(Control4_t), Control4_t, 1, 3> {};
    struct EnableMarginal0ReadMode : public BitField<decltype(Control4_t), Control4_t, 4, 1> {};
    struct EnableMarginal1ReadMode : public BitField<decltype(Control4_t), Control4_t, 5, 1> {};
    struct Reserved6 : public BitField<decltype(Control4_t), Control4_t, 6, 1> {};
    /// If set, the information memory cannot be erased in segment erase mode and cannot be written to.
    struct LockInformationMemory : public BitField<decltype(Control4_t), Control4_t, 7, 1> {};
    /// Always reads as 096h (0x96 = 150). Must be written as 0A5h (0xA5 = 165) or a PUC is generated.
    struct ControlPassword : public BitField<decltype(Control4_t), Control4_t, 8, 8> {};
  };
};

template<volatile unsigned char& Control0_t,
         volatile unsigned char& Control1_t,
         volatile unsigned char& BaudRate0_t,
         volatile unsigned char& BaudRate1_t,
         volatile unsigned char& ModulationControl_t,
         volatile unsigned char& Status_t,
         volatile unsigned char& RXBuffer_t,
         volatile unsigned char& TXBuffer_t,
         volatile unsigned char& AutoBaudRateControl_t,
         volatile unsigned char& IrDATransmitControl_t,
         volatile unsigned char& IrDAReceiveControl_t>
struct USCI_A {
  USCI_A() = delete;

  struct Control0 : public Register<decltype(Control0_t), Control0_t> {
    struct SyncMode : public BitField<decltype(Control0_t), Control0_t, 0, 1> {
      constexpr static typename SyncMode::OPT
        UART_MODE{0},
        SPI_MODE{1};
    };
    /// Selects async mode when SyncMode = 0.
    ///   And sync mode when SyncMode = 1.
    struct USCIMode : public BitField<decltype(Control0_t), Control0_t, 1, 2> {
      constexpr static typename USCIMode::OPT
        UART_MODE_or_3pin_SPI{0},
        IleLine_Multiprocessor_or_4pin_SPI_UCxSTE_active_HIGH{1},
        AddressBit_Multiprocessor_or_4pin_SPI_UCxSTE_active_LOW{2},
        AutomaticBaudRate_or_I2C_MODE{3};
    };
    struct StopBits_or_MasterMode : public BitField<decltype(Control0_t), Control0_t, 3, 1> {
      constexpr static typename StopBits_or_MasterMode::OPT
        ONE_STOP_BIT_or_SLAVE_MODE{0},
        TWO_STOP_BITS_or_MASTER_MODE{1};
    };
    /// UC7BIT
    ///   Selects 7-bit or 8-bit character length.
    struct CharacterLength : public BitField<decltype(Control0_t), Control0_t, 4, 1> {
      constexpr static typename CharacterLength::OPT
        CHAR_8bit{0},
        CHAR_7bit{1};
    };
    /// UCMSB
    ///   Controls the direction of the receive and transmit shift register.
    struct MSBFirst : public BitField<decltype(Control0_t), Control0_t, 5, 1> {
      constexpr static typename MSBFirst::OPT
        LSB_FIRST{0},
        MSB_FIRST{1};
    };
    /// UCPAR in Async (UART, Sync = 0)
    ///   selects parity or is not used when parity is disabled by the last bitfield Parity UCPEN.
    ///   UCCKPL in Sync (SPI, Sync = 1)
    ///   selects clock polarity.
    struct Parity_or_Polarity : public BitField<decltype(Control0_t), Control0_t, 6, 1> {
      constexpr static typename Parity_or_Polarity::OPT
        OddParity_or_InactiveStateIsLow{0},
        EvenParity_or_InactiveStateIsHigh{1};
    };
    /// UCPEN in Async (UART, Sync = 0) enables parity: 
    ///   parity bit is generated (UCAxTXD) and expected (UCAxRXD).
    ///   In address-bit multiprocessor mode, the address bit is included in the parity calculation.
    ///   UCCKPL in Sync (SPI, Sync = 1) selects clock phase:
    ///   data is changed of the first UCLK edge and captured on the following edge,
    ///   or is captured on the first edge and changed on the following.
    struct ParityEnable_or_ClockPhaseSelect : public BitField<decltype(Control0_t), Control0_t, 7, 1> {
      constexpr static typename ParityEnable_or_ClockPhaseSelect::OPT
        ParityDisable_or_DataChangeCapture{0},
        ParityEnable_or_DataCaptureChange{1};
    };
  };

  struct Control1 : public Register<decltype(Control1_t), Control1_t> {
    /// Valid in both modes.
    ///   When enabled (=1), the USCI logic is held in reset state.
    ///   When disabled (=0), USCI released for operation.
    struct SoftwareReset : public BitField<decltype(Control1_t), Control1_t, 0, 1> {};
    /// UCTXBRK
    ///   Transmits a break with the next write to the transmit buffer.
    ///   In UART mode with automatic baud-rate detection,
    ///   055h (0x55) must be written into
    ///   UCAxTXBUF to generate the required break/synch fields.
    ///   Otherwise, 0x0 must be written into the transmit buffer.
    struct TransmitBreak : public BitField<decltype(Control1_t), Control1_t, 1, 1> {
      constexpr static typename TransmitBreak::OPT
        NEXT_FRAME_IS_NOT_A_BREAK{0},
        NEXT_FRAME_IS_A_BREAK{1} /** Next frame transmitted is a break or a break/synch. */;
    };
    /// UCTXADDR
    ///   Transmits to be transmitted is marked as address,
    ///   depending on the selected multiprocessor mode.

    /// Value options.
    /// NEXT_FRAME_IS_DATA 
    /// NEXT_FRAME_IS_ADDRESS
    struct TransmitAddress : public BitField<decltype(Control1_t), Control1_t, 2, 1> {};
    /// UCDORM
    ///   Puts USCI into sleep mode.

    /// Value options.
    /// NOT_DORMANT
    /// All received characters set the flag UCRXIFG.

    /// DORMANT
    /// Only characters that are preceded by an idle-line or with address bit set UCRXIFG.
    ///   In UART mode with automatic baud-rate detection, only the combination of a break and synch fields sets UCRXIFG.
    struct Dormant : public BitField<decltype(Control1_t), Control1_t, 3, 1> {};
    /// UCBRKIE
    ///   Receive break character interrupt enable - set or no UCRXIFG.

    /// Value options.
    /// RX_BREAK_DONOT_SET_FLAG
    /// Received break characters do not set the flag UCRXIFG.

    /// RX_BREAK_SET_FLAG
    /// Received break characters set the flag UCRXIFG.
    struct RXBreakCharInterruptEnable : public BitField<decltype(Control1_t), Control1_t, 4, 1> {};
    /// UCRXEIE
    ///   Receive erroneous-character interrupt enable - set or no UCRXIFG.

    /// Value options.
    /// ERRCHARS_REJECTED_N_UCRXIFG_NOT_SET 
    /// ERRCHARS_RECEIVED_N_UCRXIFG_SET
    struct RXErroneousCharInterruptEnable : public BitField<decltype(Control1_t), Control1_t, 5, 1> {};
    /// UCSSELx
    ///   USCI clock source select. Select the BRCLK source clock.

    /// Value options.
    /// UCAxCLK External USCI clock 
    /// ACLK 
    /// SMCLK 
    /// SMCLK
    struct ClockSource : public BitField<decltype(Control1_t), Control1_t, 6, 2> {};
  };

  struct BaudRate0 : public Register<decltype(BaudRate0_t), BaudRate0_t> {};

  struct BaudRate1 : public Register<decltype(BaudRate1_t), BaudRate1_t> {};

  struct ModulationControl : public Register<decltype(ModulationControl_t), ModulationControl_t> {
    /// UCOS16
    struct OversamplingEnable : public BitField<decltype(ModulationControl_t), ModulationControl_t, 0, 1> {};
    /// UCBRSx
    ///   These bits determine the modulation pattern for BITCLK. Table 36-2 in the slau208q User Guide.
    struct SecondModulationStageSelect : public BitField<decltype(ModulationControl_t), ModulationControl_t, 1, 3> {};
    /// UCBRFx
    ///   These bits determine the modulation pattern for BITCLK16 when Oversampling is enabled (UCOS16).
    ///   Ignored when Oversampling is disabled.
    ///   Table 36-2 in the slau208q User Guide.
    struct FirstModulationStageSelect : public BitField<decltype(ModulationControl_t), ModulationControl_t, 4, 4> {};
  };

  struct Status : public Register<decltype(Status_t), Status_t> {
    /// UCBUSY Common to both modes.
    ///   Indicated if a transmit or receive operation is in progress.
    struct Busy : public BitField<decltype(Status_t), Status_t, 0, 1> {};
    /// UCADDR/UCIDLE Only UART.
    ///   UCADDR: address received in address-bit multiprocessor mode.
    ///   Cleared when UCAxRXBUF is read.
    ///   UCIDLE: idle line detected in idle-line multiprocessor mode.
    ///   Cleared when UCAxRXBUF is read.
    struct AddrIdle : public BitField<decltype(Status_t), Status_t, 1, 1> {
      constexpr static typename AddrIdle::OPT
        DATA_or_NOIDLE{0},
        ADDRESS_OR_IDLE_DETECTED{1};
    };
    /// UCRXERR Only UART.
    ///   Indicated a character was received with error(s).
    ///   When UCRXERR = 1, one or more error flags, UCFE UCPE UCOE, is also set.
    ///   Cleared when UCAxRXBUF is read.
    struct ErrorFlag : public BitField<decltype(Status_t), Status_t, 2, 1> {
      constexpr static typename ErrorFlag::OPT
        NO_ERRORS{0},
        ERROR_DETECTED{1};
    };
    /// UCBRK Only UART.
    ///   Cleared when UCAxRXBUF is read.
    struct BreakDetectedFlag : public BitField<decltype(Status_t), Status_t, 3, 1> {
      constexpr static typename BreakDetectedFlag::OPT
        NO_BREAK_CONDITIONS{0},
        BREAK_CONDITION_OCCURED{1};
    };
    /// UCPE Only UART.
    ///   Cleared when UCAxRXBUF is read.
    struct ErrorFlagParity : public BitField<decltype(Status_t), Status_t, 4, 1> {
      constexpr static typename ErrorFlagParity::OPT
        NO_PARITY_ERROR{0},
        WITH_PARITY_ERROR{1};
    };
    /// UCOE Both modes.
    ///   The bit is set when a character is transfered into UCAxRXBUF before the previous character was read.
    ///   Cleared when UCAxRXBUF is read, and must not be cleared by software.
    ///   Otherwise, it does not function correctly.
    struct ErrorFlagOverrun : public BitField<decltype(Status_t), Status_t, 5, 1> {
      constexpr static typename ErrorFlagOverrun::OPT
        NO_OVERRUN_ERROR{0},
        OVERRUN_OCCURED{1};
    };
    /// UCFE Both modes.
    ///   Framing error flag.
    ///   Cleared when UCAxRXBUF is read.
    struct ErrorFlagFraming : public BitField<decltype(Status_t), Status_t, 6, 1> {
      constexpr static typename ErrorFlagFraming::OPT
        NO_FRAMING_ERROR{0},
        WITH_LOW_STOP_BIT{1};
    };
    /// UCLISTEN Both modes.
    ///   Selects loopback mode.
    ///   When enabled (=1) the UCAxTXD is internally fed back to the receiver.
    struct ListenEnable : public BitField<decltype(Status_t), Status_t, 7, 1> {};
  };

  struct RXBuffer : public Register<decltype(RXBuffer_t), RXBuffer_t> {};

  struct TXBuffer : public Register<decltype(TXBuffer_t), TXBuffer_t> {};

  struct AutoBaudRateControl : public Register<decltype(AutoBaudRateControl_t), AutoBaudRateControl_t> {
    /// UCABDEN
    ///   Automatic baud-rate detect enable.
    ///   When enabled (=1) the length of break and sync field is measured and baud-rate settings are changed accordingly.
    struct Enable : public BitField<decltype(AutoBaudRateControl_t), AutoBaudRateControl_t, 0, 1> {};
    struct Reserved1 : public BitField<decltype(AutoBaudRateControl_t), AutoBaudRateControl_t, 1, 1> {};
    /// UCBTOE
    ///   Break time out error.
    ///   When set (=1), the length of break field exceeded 22 bit times.
    struct TimeoutBreak : public BitField<decltype(AutoBaudRateControl_t), AutoBaudRateControl_t, 2, 1> {};
    /// UCSTOE
    ///   Synch field time out error.
    ///   When set (=1), the length of synch field exceeded measurable time.
    struct TimeoutSynch : public BitField<decltype(AutoBaudRateControl_t), AutoBaudRateControl_t, 3, 1> {};
    /// UCDELIMx
    ///   Break and synch delimiter length.
    struct BreakSynchDelimiterLength : public BitField<decltype(AutoBaudRateControl_t), AutoBaudRateControl_t, 4, 2> {
      constexpr static typename BreakSynchDelimiterLength::OPT
        DELIM_1_BIT{0},
        DELIM_2_BIT{1},
        DELIM_3_BIT{2},
        DELIM_4_BIT{3};
    };
    struct Reserved6 : public BitField<decltype(AutoBaudRateControl_t), AutoBaudRateControl_t, 6, 2> {};
  };

  struct IrDATransmitControl : public Register<decltype(IrDATransmitControl_t), IrDATransmitControl_t> {
    struct EncoderDecoderEnable : public BitField<decltype(IrDATransmitControl_t), IrDATransmitControl_t, 0, 1> {};
    struct TransmitPulseClockSelect : public BitField<decltype(IrDATransmitControl_t), IrDATransmitControl_t, 1, 1> {
      constexpr static typename TransmitPulseClockSelect::OPT
        BRCLK{0},
        BITCLK{1} /** When Oversampling is ON (UCOS16 = 1). Otherwise, BRCLK. */;
    };
    /// UCIRTXPLx
    ///   The minimum pulse length for receive is given by:
    ///   t_pulse = (UCIRTXPLx + 1) / (2 \* f_irtxclk)
    struct TransmitLength : public BitField<decltype(IrDATransmitControl_t), IrDATransmitControl_t, 2, 6> {};
  };

  struct IrDAReceiveControl : public Register<decltype(IrDAReceiveControl_t), IrDAReceiveControl_t> {
    struct FilterEnable : public BitField<decltype(IrDAReceiveControl_t), IrDAReceiveControl_t, 0, 1> {};
    struct Polarity : public BitField<decltype(IrDAReceiveControl_t), IrDAReceiveControl_t, 1, 1> {
      constexpr static typename Polarity::OPT
        HIGH_PULSE_ON_LIGHT{0},
        LOW_PULSE_ON_LIGHT{1};
    };
    /// UCIRRXFLx
    ///   The minimum pulse length for receive is given by:
    ///   t_min = (UCIRRXFLx + 4) / (2 \* f_brclk)
    struct FilterLength : public BitField<decltype(IrDAReceiveControl_t), IrDAReceiveControl_t, 2, 6> {};
  };
};

template<volatile unsigned char& Control0_t,
         volatile unsigned char& Control1_t,
         volatile unsigned char& BaudRate0_t,
         volatile unsigned char& BaudRate1_t,
         volatile unsigned char& I2CInterruptEnable_t,
         volatile unsigned char& Status_t,
         volatile unsigned char& RXBuffer_t,
         volatile unsigned char& TXBuffer_t,
         volatile unsigned int& I2COwnAddress_t,
         volatile unsigned int& I2CSlaveAddress_t>
struct USCI_B {
  USCI_B() = delete;

  struct Control0 : public Register<decltype(Control0_t), Control0_t> {
    /// UCSYNC
    /// Not sure what this one means here.
    /// The msp430g2553.h header says it is Async=UART and Sync=SPI,
    /// i.e. the same as for USCI A. Which cannot be true.
    /// One should SPI and another one is I2C.
    /// I guess, SyncMode=ASYNCHRONOUS and USCIMode=I2C selects I2C?
    struct SyncMode : public BitField<decltype(Control0_t), Control0_t, 0, 1> {
      constexpr static typename SyncMode::OPT
        ASYNCHRONOUS{0},
        SYNCHRONOUS{1};
    };
    /// UCMODE0 and UCMODE1
    struct USCIMode : public BitField<decltype(Control0_t), Control0_t, 1, 2> {
      constexpr static typename USCIMode::OPT
        SYNC_3pin_SPI{0},
        SYNC_4pin_SPI_UCxSTE_active_HIGH{1} /** Slave enabled when UCxSTE = 1. */,
        SYNC_4pin_SPI_UCxSTE_active_LOW{2} /** Slave enabled when UCxSTE = 0. */,
        I2C_MODE{3};
    };
    /// UCMST
    /// In I2C mode: when a master loses arbitration in a multi-master environment (UCMM = 1),
    /// the UCMST bit is automatically cleared and the module acts as slave.
    struct MasterMode : public BitField<decltype(Control0_t), Control0_t, 3, 1> {
      constexpr static typename MasterMode::OPT
        SLAVE_MODE{0},
        MASTER_MODE{1};
    };
    /// UC7BIT
    /// In SPI mode: selects 7-bit or 8-bit character length.
    /// In I2C: reserved, alwasy reads as 0.
    struct CharacterLength : public BitField<decltype(Control0_t), Control0_t, 4, 1> {
      constexpr static typename CharacterLength::OPT
        CHAR_8bit{0},
        CHAR_7bit{1};
    };
    /// In SPI mode: UCMSB, controls the direction of the receive and transmit shift register.
    /// In I2C mode: UCMM, selects single-master or multi-master environment.
    struct MSBFirst_or_MultiMasterSelect : public BitField<decltype(Control0_t), Control0_t, 5, 1> {
      constexpr static typename MSBFirst_or_MultiMasterSelect::OPT
        LSB_FIRST_or_SINGLE_MASTER{0} /** I2C single-master: there is no other master in the system. The address compare unit is disabled. */,
        MSB_FIRST_or_MULTI_MASTER{1};
    };
    /// In SPI (Sync) mode: UCCKPL selects clock polarity.
    /// In I2C mode: address slave with 7-bit or 10-bit address.
    struct Polarity_or_SlaveAddressingMode : public BitField<decltype(Control0_t), Control0_t, 6, 1> {
      constexpr static typename Polarity_or_SlaveAddressingMode::OPT
        InactiveStateIsLow_or_Address7bit{0},
        InactiveStateIsHigh_or_Address10bit{1};
    };
    /// In SPI (Sync) mode: UCCKPL selects clock phase:
    /// data is changed of the first UCLK edge and captured on the following edge,
    /// or is captured on the first edge and changed on the following.
    /// In I2C mode: UCA10 selects own address is 7-bit or 10-bit address.
    struct ClockPhaseSelect_or_OwnAddressingMode : public BitField<decltype(Control0_t), Control0_t, 7, 1> {
      constexpr static typename ClockPhaseSelect_or_OwnAddressingMode::OPT
        DataChangeCapture_or_OwnAddress7bit{0},
        DataCaptureChange_or_OwnAddress10bit{1};
    };
  };

  struct Control1 : public Register<decltype(Control1_t), Control1_t> {
    /// Valid in both modes.
    ///   When enabled (=1), the USCI logic is held in reset state.
    ///   When disabled (=0), USCI released for operation.
    struct SoftwareReset : public BitField<decltype(Control1_t), Control1_t, 0, 1> {};
    /// UCTXSTT Only I2C mode.
    ///   Transmit START condition in master mode, ignored in slave mode.
    ///   In master receiver mode, a repeated START condition is preceded by a NACK.
    ///   UCTXSTT is automatically cleared after START comdition
    ///   and address information is transmitted.
    struct TransmitStartCondition : public BitField<decltype(Control1_t), Control1_t, 1, 1> {
      constexpr static typename TransmitStartCondition::OPT
        DO_NOT_GENERATE_START_CONDITION{0},
        GENERATE_START_CONDITION{1};
    };
    /// UCTXSTP Only in I2C mode.
    ///   Transmit STOP condition in master mode. Ignored in slave mode.
    ///   In master receiver mode, the STOP condition is preceded by a NACK.
    ///   UCTXSTP is automatically cleared after STOP is generated.
    struct TransmitStopCondition : public BitField<decltype(Control1_t), Control1_t, 2, 1> {
      constexpr static typename TransmitStopCondition::OPT
        NO_STOP_GENERATED{0},
        GENERATE_STOP{1};
    };
    /// UCTXNACK Only I2C mode.
    ///   Transmit a NACK. UCTXNACK is automatically cleared after a NACK is transmitted.
    struct TransmitNACK : public BitField<decltype(Control1_t), Control1_t, 3, 1> {
      constexpr static typename TransmitNACK::OPT
        ACKNOWLEDGE_NORMALLY{0},
        GENERATE_NACK{1};
    };
    /// UCTR Only in I2C.
    struct TransmitterOrReceiver : public BitField<decltype(Control1_t), Control1_t, 4, 1> {
      constexpr static typename TransmitterOrReceiver::OPT
        RECEIVER{0},
        TRANSMITTER{1};
    };
    /// Always reads 0.
    struct Reserved : public BitField<decltype(Control1_t), Control1_t, 5, 1> {};
    /// UCSSELx Valid in both SPI and I2C modes.
    ///   USCI clock source select.
    ///   In SPI mode: select the BRCLK source clock in master mode,
    ///   UCxCLK is always used in slave mode.
    ///   In I2C mode: select BRCLK source clock.
    struct ClockSource : public BitField<decltype(Control1_t), Control1_t, 6, 2> {
      constexpr static typename ClockSource::OPT
        Reserved_or_UCLKI{0},
        ACLK{1},
        SMCLK{3};
    };
  };

  struct BaudRate0 : public Register<decltype(BaudRate0_t), BaudRate0_t> {};

  struct BaudRate1 : public Register<decltype(BaudRate1_t), BaudRate1_t> {};

  struct I2CInterruptEnable : public Register<decltype(I2CInterruptEnable_t), I2CInterruptEnable_t> {
    /// UCRXIE
    struct ReceiveIE : public BitField<decltype(I2CInterruptEnable_t), I2CInterruptEnable_t, 0, 1> {};
    /// UCTXIE
    struct TransmitIE : public BitField<decltype(I2CInterruptEnable_t), I2CInterruptEnable_t, 1, 1> {};
    /// UCSTTIE
    struct StartConditionIE : public BitField<decltype(I2CInterruptEnable_t), I2CInterruptEnable_t, 2, 1> {};
    /// UCSTPIE
    struct StopConditionIE : public BitField<decltype(I2CInterruptEnable_t), I2CInterruptEnable_t, 3, 1> {};
    /// UCALIE
    struct ArbitrationLostIE : public BitField<decltype(I2CInterruptEnable_t), I2CInterruptEnable_t, 4, 1> {};
    /// UCNACKIE
    struct NotAcknowledgeIE : public BitField<decltype(I2CInterruptEnable_t), I2CInterruptEnable_t, 5, 1> {};
    /// UCNACKIE
    struct Reserved : public BitField<decltype(I2CInterruptEnable_t), I2CInterruptEnable_t, 6, 2> {};
  };

  struct Status : public Register<decltype(Status_t), Status_t> {
    /// UCBUSY only SPI.
    ///   Indicated if a transmit or receive operation is in progress.
    struct Busy : public BitField<decltype(Status_t), Status_t, 0, 1> {};
    struct Reserved1 : public BitField<decltype(Status_t), Status_t, 1, 3> {};
    /// UCBBUSY only in I2C.
    struct I2CBusBusy : public BitField<decltype(Status_t), Status_t, 4, 1> {
      constexpr static typename I2CBusBusy::OPT
        BUS_INACTIVE{0},
        BUS_BUSY{1};
    };
    /// UCOE overrun flag in SPI.
    ///   The bit is set when a character is transfered into UCxRXBUF before the previous character was read.
    ///   Cleared when UCxRXBUF is read, and must not be cleared by software.
    ///   Otherwise, it does not function correctly.
    ///   UCGC general call address received in I2C.
    ///   It is cleared automatically when a START condition is received.
    struct ErrorFlagOverrun_or_GeneralCallAddressReceived : public BitField<decltype(Status_t), Status_t, 5, 1> {
      constexpr static typename ErrorFlagOverrun_or_GeneralCallAddressReceived::OPT
        NO_OVERRUN_ERROR_or_NO_GENERAL_CALL_ADDRESS_RECEIVED{0},
        OVERRUN_OCCURED_or_GENERAL_CALL_ADDRESS_RECEIVED{1};
    };
    /// UCFE framing error flag in SPI.
    ///   This bit indicates a bus conflict in 4-wire master mode.
    ///   UCFE is not used in 3-wire master or any slave mode.
    ///   UCSCLLOW in I2C: SCL is held low.
    struct ErrorFlagFraming_or_SCLIsLow : public BitField<decltype(Status_t), Status_t, 6, 1> {
      constexpr static typename ErrorFlagFraming_or_SCLIsLow::OPT
        NO_ERROR_or_SCL_NOT_LOW{0},
        BUS_CONFLICT_OCCURED_or_SCL_IS_LOW{1};
    };
    /// UCLISTEN only SPI mode.
    ///   Selects loopback mode.
    ///   When enabled (=1) the transmitter output is internally fed back to the receiver.
    struct ListenEnable : public BitField<decltype(Status_t), Status_t, 7, 1> {};
  };

  struct RXBuffer : public Register<decltype(RXBuffer_t), RXBuffer_t> {};

  struct TXBuffer : public Register<decltype(TXBuffer_t), TXBuffer_t> {};

  struct I2COwnAddress : public Register<decltype(I2COwnAddress_t), I2COwnAddress_t> {
    /// I2COAx I2C own address, the local address of
    ///   the USCI_Bx I2C controller.
    ///   The address is right justified.
    ///   In 7-bit addressing mode, bit 6 is the MSB and bits 9-7 are ignored.
    ///   In 10-bit addressing mode, bit 9 is MSB.
    struct OwnAddress : public BitField<decltype(I2COwnAddress_t), I2COwnAddress_t, 0, 10> {};
    struct Reserved : public BitField<decltype(I2COwnAddress_t), I2COwnAddress_t, 10, 5> {};
    /// UCGCEN
    struct GeneralCallResponseEnable : public BitField<decltype(I2COwnAddress_t), I2COwnAddress_t, 15, 1> {
      constexpr static typename GeneralCallResponseEnable::OPT
        DO_NOT_RESPOND_TO_GENERAL_CALL{0},
        RESPOND_TO_GENERAL_CALL{1};
    };
  };

  struct I2CSlaveAddress : public Register<decltype(I2CSlaveAddress_t), I2CSlaveAddress_t> {
    /// I2CSAx I2C slave address of the external device to be addressed by
    ///   the USCI_Bx module. It is only used in master mode.
    ///   The address is right justified.
    ///   In 7-bit slave addressing mode, bit 6 is the MSB and bits 9-7 are ignored.
    ///   In 10-bit slave addressing mode, bit 9 is MSB.
    struct SlaveAddress : public BitField<decltype(I2CSlaveAddress_t), I2CSlaveAddress_t, 0, 10> {};
    struct Reserved : public BitField<decltype(I2CSlaveAddress_t), I2CSlaveAddress_t, 10, 6> {};
  };
};

template<volatile unsigned int& Control_t>
struct WatchdogTimer {
  WatchdogTimer() = delete;

  struct Control : public Register<decltype(Control_t), Control_t> {
    /// WDTIS Select the watchdog timer interval
    ///   to set WDTIFG flag or generate a PUC.
    ///   The User Guide says it is a 3-bit field.
    ///   But the  header says the third bit is WDTSSEL - source clock select?
    ///   But the User Guide says the clock select field WDTSSEL is at 5-6 bits.
    ///   I go with the Guide.
    struct TimerIntervalSelect : public BitField<decltype(Control_t), Control_t, 0, 3> {
      constexpr static typename TimerIntervalSelect::OPT
        CLOCK_DIV_2e31{0} /** 18h:12m:16s at 32.768 kHz */,
        CLOCK_DIV_2e27{1} /** 01h:08m:16s at 32.768 kHz */,
        CLOCK_DIV_2e23{2} /** 00h:04m:16s at 32.768 kHz */,
        CLOCK_DIV_2e19{3} /** 00h:00m:16s at 32.768 kHz */,
        CLOCK_DIV_2e15{4} /** 1s at 32.768 kHz */,
        CLOCK_DIV_2e13{5} /** 250ms at 32.768 kHz */,
        CLOCK_DIV_2e9{6} /** 15.625ms at 32.768 kHz */,
        CLOCK_DIV_2e6{7} /** 1.95ms at 32.768 kHz */;
    };
    /// WDTCNTCL clears the count value to 0.
    struct CounterClear : public BitField<decltype(Control_t), Control_t, 3, 1> {};
    /// WDTTMSEL
    struct ModeSelect : public BitField<decltype(Control_t), Control_t, 4, 1> {
      constexpr static typename ModeSelect::OPT
        WATCHDOG{0},
        INTERVAL_TIMER{1};
    };
    /// WDTSSEL
    struct ClockSource : public BitField<decltype(Control_t), Control_t, 5, 2> {
      constexpr static typename ClockSource::OPT
        SMCLK{0},
        ACLK{1},
        VLOCLK{2},
        X_CLK{3} /** VLOCLK in devices that do not use X_CLK */;
    };
    /// WDTHOLD Stops the watchdog timer.
    /// WDTHOLD = 1 when WDT is not in use conserves power.
    struct TimerHold : public BitField<decltype(Control_t), Control_t, 7, 1> {
      constexpr static typename TimerHold::OPT
        NOT_STOPPED{0},
        STOPPED{1};
    };
  };
};

template<volatile unsigned char& DCO_16MHZ_t,
         volatile unsigned char& BC1_16MHZ_t,
         volatile unsigned char& DCO_12MHZ_t,
         volatile unsigned char& BC1_12MHZ_t,
         volatile unsigned char& DCO_8MHZ_t,
         volatile unsigned char& BC1_8MHZ_t,
         volatile unsigned char& DCO_1MHZ_t,
         volatile unsigned char& BC1_1MHZ_t,
         volatile unsigned int& TLVChecksum_t,
         volatile unsigned char& TLVDCO30Tag_t,
         volatile unsigned char& TLVDCO30Len_t,
         volatile unsigned char& TLVADC10Tag_t,
         volatile unsigned char& TLVADC10Len_t>
struct CalibrationData {
  CalibrationData() = delete;

  struct DCO_16MHZ : public Register<decltype(DCO_16MHZ_t), DCO_16MHZ_t> {};

  struct BC1_16MHZ : public Register<decltype(BC1_16MHZ_t), BC1_16MHZ_t> {};

  struct DCO_12MHZ : public Register<decltype(DCO_12MHZ_t), DCO_12MHZ_t> {};

  struct BC1_12MHZ : public Register<decltype(BC1_12MHZ_t), BC1_12MHZ_t> {};

  struct DCO_8MHZ : public Register<decltype(DCO_8MHZ_t), DCO_8MHZ_t> {};

  struct BC1_8MHZ : public Register<decltype(BC1_8MHZ_t), BC1_8MHZ_t> {};

  struct DCO_1MHZ : public Register<decltype(DCO_1MHZ_t), DCO_1MHZ_t> {};

  struct BC1_1MHZ : public Register<decltype(BC1_1MHZ_t), BC1_1MHZ_t> {};

  struct TLVChecksum : public Register<decltype(TLVChecksum_t), TLVChecksum_t> {};

  struct TLVDCO30Tag : public Register<decltype(TLVDCO30Tag_t), TLVDCO30Tag_t> {};

  struct TLVDCO30Len : public Register<decltype(TLVDCO30Len_t), TLVDCO30Len_t> {};

  struct TLVADC10Tag : public Register<decltype(TLVADC10Tag_t), TLVADC10Tag_t> {};

  struct TLVADC10Len : public Register<decltype(TLVADC10Len_t), TLVADC10Len_t> {};
};

};

