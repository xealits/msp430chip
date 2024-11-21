#pragma once

#include "machine/_stdint.h"
#include "msp430g2102.h"
#include <msp430.h>

template<typename Type>
using base_type = typename std::remove_reference<Type>::type;

namespace BitLogic {
    /** \brief Make a bit mask in an unsigned of type RegT
     * 
     * Use ~regMask<>() to make a mask of zeros.
     */
    template<typename RegT, RegT mask, unsigned bit_n>
    inline constexpr RegT regMaskUnfold() {
        static_assert((bit_n) < sizeof(RegT) * 8,
                "The requested bit does not fit in the register");
        return mask | (0x1 << bit_n);
    }

    template<typename RegT, RegT mask, unsigned bit_n, unsigned... rest_bit_ns>
    inline constexpr RegT regMaskUnfold() {
        static_assert((bit_n) < sizeof(RegT) * 8,
                "The requested bit does not fit in the register");
        return regMaskUnfold<RegT, mask | (0x1 << bit_n), rest_bit_ns...>();
    }

    template<typename RegT, unsigned bit_n, unsigned... rest_bit_ns>
    inline constexpr base_type<RegT>
    regMask() {
        return regMaskUnfold<base_type<RegT>, 0x0, bit_n, rest_bit_ns...>();
    }

    enum LOGIC_LEVELS {HIGH, LOW};

    // TODO: check the bit shifting edge cases
    template<unsigned bit_n, typename RegT> // RegT should have max number of bits
    constexpr void setBit(RegT& reg, LOGIC_LEVELS level) {
        static_assert((bit_n) < sizeof(RegT) * 8,
                "The requested bit does not fit in the register");

        if (level == LOGIC_LEVELS::LOW) reg &= ~RegT(0x1 << bit_n);
        else                            reg |=  (0x1 << bit_n);
    }

    /** \brief Return a mask of N low bits set
     */
    template<unsigned n_bits, typename RegT>
    constexpr inline RegT nBitsMask() {
        static_assert(n_bits > 0,
                "Cannot have bit field of 0 bits");
        static_assert((n_bits) <= sizeof(RegT) * 8,
                "The requested bit field width does not fit in the register");

        // to handle the edge case of overflow: n_bits == sizeof(RegT) * 8
        // I cannot just n_bits_mask = (1 << n_bits) - 1
        // also, I don't want to rely on if constexpr of C++17
        // hence:
        RegT n_bits_mask = 1 << (n_bits-1);
        if (n_bits > 1) n_bits_mask |= ((RegT) 1 << (n_bits-1)) - 1;

        return n_bits_mask;
    }

    template<unsigned offset, unsigned n_bits, typename RegT>
    constexpr inline RegT getRegField(const RegT& a_reg) {
        static_assert((offset+n_bits) <= sizeof(RegT) * 8,
                "The requested bit field does not fit in the register");
        // TODO: assert RegT is not a reference thing?
        const RegT n_bits_mask = nBitsMask<n_bits, RegT>();
        RegT reg_val = a_reg;
        return (reg_val >> offset) & n_bits_mask;
    }

    template<unsigned offset, unsigned n_bits, typename RegT>
    constexpr inline void setRegField(RegT& a_reg, RegT new_val) {
        static_assert((offset+n_bits) <= sizeof(RegT) * 8,
                "The requested bit field does not fit in the register");
        // TODO: assert RegT is not a reference thing?

        const RegT n_bits_mask = nBitsMask<n_bits, RegT>();

        // make sure the input is in the mask
        new_val &= n_bits_mask;

        // clear the field in the reg
        RegT reg_val = a_reg;
        reg_val &= ~(n_bits_mask << offset);
        // add the input into the field
        reg_val |= new_val << offset;
        // write the new value to the HW memory:
        a_reg = reg_val;
    }

    template<unsigned offset, const unsigned n_bits, typename RegT>
    constexpr inline RegT maskRegField(RegT new_val) {
        static_assert((offset+n_bits) <= sizeof(RegT) * 8,
                "The requested bit field does not fit in the register");
        // TODO: assert RegT is not a reference thing?

        const RegT n_bits_mask = nBitsMask<n_bits, RegT>();

        // make sure the input is in the mask
        new_val &= n_bits_mask;

        // clear the field in the reg
        RegT reg_val = 0x0; //a_reg;
        reg_val &= ~(n_bits_mask << offset);
        
        // add the input into the field
        reg_val |= (new_val << offset);
        // return the new value
        // instead of setting it to some ref
        return reg_val;
    }

    /** struct is just a handy way to bundle a couple functions that act on the same refreference
     *  I could just use namespace, but I could not template it.
     *
     *  But it looks like Ti compiler cl430 does not inline this stuff well.
     */
    template<typename RegT, RegT& t_reg, unsigned t_offset, unsigned t_n_bits>
    struct BitFieldSignature {
        //static const char* s_name;
        //static const BitFieldInfo<t_offset, t_n_bits> info;
        constexpr static inline RegT get(void) {return getRegField<t_offset, t_n_bits>(t_reg);}
        //static inline void set(RegT val) {return setRegField<t_offset, t_n_bits>(t_reg, val);}
        // clang compiles it, but Ti compiler cl430 cannot:
        // "../msp430_cpp/reg_funcs.hpp", line 122: error #306: no instance of function template "BitLogic::setRegField" matches the argument list
        //    argument types are: (volatile unsigned int, volatile unsigned int)
        //  detected during instantiation of "void BitLogic::BitFieldSignature<RegT, t_reg, t_offset, t_n_bits>::set(RegT) [with RegT=base_type<volatile unsigned int>, t_reg=TA0CCTL0, t_offset=4U, t_n_bits=1U]" at line 142 of "../msp430g2xx2_1_vlo.cpp"
        // like this should be easier:
        constexpr static inline void set(RegT val) {t_reg = maskRegField<t_offset, t_n_bits>(val);}

        constexpr static inline RegT mask(RegT val) {return maskRegField<t_offset, t_n_bits>(val);}
        //static BitFieldInfo info(void) {return std::make_tuple(t_offset, t_n_bits);};
        //static BitFieldInfo info(void) {return {t_offset, t_n_bits, s_name};};
    };

    template<typename RegT, RegT& t_reg, unsigned field_offset, unsigned field_width>
    constexpr static inline RegT get(void) {return getRegField<field_offset, field_width>(t_reg);}

    template<typename RegT, RegT& t_reg, unsigned field_offset, unsigned field_width>
    constexpr static inline void set(RegT val) {t_reg = maskRegField<field_offset, field_width>(val);}

    template<typename RegT, RegT& t_reg, unsigned field_offset, unsigned field_width>
    constexpr static inline RegT mask(RegT val) {return maskRegField<field_offset, field_width>(val);}
};
//using RegType = uint16_t;

namespace IOPort1Ctrl {
    using BitLogic::LOGIC_LEVELS;

    constexpr unsigned NUMBER_OF_PINS = 8;
    // not sure if it is worth to name pins
    // it just links the board namespace to this one, the chip namespace
    enum PINS {PIN0=0, PIN1=1, PIN2=2, PIN3=3, PIN4=4, PIN5=5, PIN6=6, PIN7=7};

    //decltype(P1IN)&   input         = P1IN;                               /* Port 1 Input */
    //decltype(P1OUT)&  output        = P1OUT;                              /* Port 1 Output */
    //decltype(P1DIR)&  direction_reg = P1DIR;                              /* Port 1 Direction */
    // it looks like cl430 does not optimise references
    // let's just C it:
    #define input         P1IN    /* Port 1 Input */
    #define output        P1OUT   /* Port 1 Output */
    #define direction_reg P1DIR   /* Port 1 Direction */

    // they use extern volatile unsigned char -- it is not a pointer.
    // a symbol itself is probably mapped as needed?
    // use a reference then
    // there are more registers:
    //decltype(P1IFG)    = P1IFG;                              /* Port 1 Interrupt Flag */
    //decltype(P1IES)    = P1IES;                              /* Port 1 Interrupt Edge Select */
    //decltype(P1IE)     = P1IE;                               /* Port 1 Interrupt Enable */
    //decltype(P1SEL)    = P1SEL;                              /* Port 1 Selection */
    //decltype(P1SEL2)   = P1SEL2;                             /* Port 1 Selection 2 */
    //decltype(P1REN)    = P1REN;                              /* Port 1 Resistor Enable */

    // and there are 2 IO ports
    // what's the best wat to instantiate them at compile time?
    // Ti defines the availability like this:
    // #define __MSP430_HAS_PORT1_R__                /* Definition to show that Module is available */
    // #define __MSP430_HAS_PORT2_R__                /* Definition to show that Module is available */

    // #define __MSP430_HAS_P1SEL__                  /* Define for DriverLib */
    // #define __MSP430_HAS_P1SEL2__                 /* Define for DriverLib */
    // #define __MSP430_HAS_P2SEL__                  /* Define for DriverLib */
    // #define __MSP430_HAS_P2SEL2__                 /* Define for DriverLib */

    //using IO_DIR = LOGIC_LEVELS;
    //using IO_IN  = LOGIC_LEVELS::LOW;
    //using IO_OUT = LOGIC_LEVELS::HIGH;
    enum IO_DIR {IO_IN = LOGIC_LEVELS::LOW, IO_OUT = LOGIC_LEVELS::HIGH};

    template<std::remove_reference<decltype(direction_reg)>::type pin_n>
    void setPinDirection(IO_DIR dir) {
      static_assert(pin_n < NUMBER_OF_PINS, "pin_n >= NUMBER_OF_PINS!"); // TODO: template N pins
      setBit<pin_n>(direction_reg, static_cast<LOGIC_LEVELS>(dir));
    }

    template<unsigned... bit_ns>
    constexpr inline void setDirecionOutputs(void) {
      //direction_reg = regMask<std::remove_reference<decltype(direction_reg)>::type, 0x0, bit_ns...>();
      //direction_reg = regMaskUnfold<base_type<decltype(direction_reg)>, 0x0, bit_ns...>();
      direction_reg = BitLogic::regMask<decltype(direction_reg), bit_ns...>();
    }

    // the output on the pins
    template<unsigned... bit_ns>
    constexpr inline void toggleOutput(void) {
      output ^= BitLogic::regMask<decltype(output), bit_ns...>();
    }

    template<unsigned... bit_ns>
    constexpr inline void setOutput(void) {
      output |= BitLogic::regMask<decltype(output), bit_ns...>();
    }

    template<unsigned... bit_ns>
    constexpr inline void unsetOutput(void) {
      output &= ~ BitLogic::regMask<decltype(output), bit_ns...>();
    }
};

// not sure what the numbers mean? Why Timer0 and why A3?
// the ag_11-timer-A.pdf talks only about Timer_A
// with 5 CCRs from 0 to 4
///************************************************************
//* Timer0_A3
//************************************************************/
//#define __MSP430_HAS_TA3__                    /* Definition to show that Module is available */
//
// maybe 3 means how many CC blocks there are?
// and Timer0 means just the number of the timer
// SFR_16BIT(TA0IV);                             /* Timer0_A3 Interrupt Vector Word */
// SFR_16BIT(TA0CTL);                            /* Timer0_A3 Control */
// SFR_16BIT(TA0CCTL0);                          /* Timer0_A3 Capture/Compare Control 0 */
// SFR_16BIT(TA0CCTL1);                          /* Timer0_A3 Capture/Compare Control 1 */
// SFR_16BIT(TA0CCTL2);                          /* Timer0_A3 Capture/Compare Control 2 */
// SFR_16BIT(TA0R);                              /* Timer0_A3 Counter Register */
// SFR_16BIT(TA0CCR0);                           /* Timer0_A3 Capture/Compare 0 */
// SFR_16BIT(TA0CCR1);                           /* Timer0_A3 Capture/Compare 1 */
// SFR_16BIT(TA0CCR2);                           /* Timer0_A3 Capture/Compare 2 */

//#define MC_0                   (0*0x10u)      /* Timer A mode control: 0 - Stop */
//#define MC_1                   (1*0x10u)      /* Timer A mode control: 1 - Up to CCR0 */
//#define MC_2                   (2*0x10u)      /* Timer A mode control: 2 - Continous up */
//#define MC_3                   (3*0x10u)      /* Timer A mode control: 3 - Up/Down */
//#define ID_0                   (0*0x40u)      /* Timer A input divider: 0 - /1 */
//#define ID_1                   (1*0x40u)      /* Timer A input divider: 1 - /2 */
//#define ID_2                   (2*0x40u)      /* Timer A input divider: 2 - /4 */
//#define ID_3                   (3*0x40u)      /* Timer A input divider: 3 - /8 */
//#define TASSEL_0               (0*0x100u)     /* Timer A clock source select: 0 - TACLK */
//#define TASSEL_1               (1*0x100u)     /* Timer A clock source select: 1 - ACLK  */
//#define TASSEL_2               (2*0x100u)     /* Timer A clock source select: 2 - SMCLK */
//#define TASSEL_3               (3*0x100u)     /* Timer A clock source select: 3 - INCLK */

namespace Timer0_A3 {
    // I use very few regs now:
    //CCTL0 = CCIE;                             // CCR0 interrupt enabled
    //CCR0 = CLOCK_DELAY_TICKS;
    //is_short_tick ^= 1; // i.e. next tick will be short
    ////TACTL = TASSEL_2 + MC_2;                  // SMCLK, contmode
    ////TACTL = TASSEL_1 + MC_2;                  // ACLK, contmode
    //TACTL = TASSEL_1 + MC_2 + ID_0;             // ACLK, contmode, divided: ID_0 means no divider
    //
    /* and this code becomes:
     * Timer_A::cc0_interrupt_enable.set(1);
     * Timer_A::ccr0 = CLOCK_DELAY_TICKS;
     * 
     * Timer_A::input_clock_select.set(Timer_A::INPUT_CLOCK_ACLK);
     * Timer_A::clock_divider.set(Timer_A::DIVIDE_0);
     * Timer_A::mode_control.set(Timer_A::MODE_CONT);
     * Not sure if it is better. I.e. there should be a way to combine the fields?
     * ok, then like this:
     * Timer_A::timer_control =
     *     Timer_A::input_clock_select.mask(Timer_A::INPUT_CLOCK_ACLK) |
     *     Timer_A::clock_divider.mask(Timer_A::DIVIDE_0) |
     *     Timer_A::mode_control.mask(Timer_A::MODE_CONT);
    */

    //
    //decltype(TACTL)& timer_control  = TACTL; // so, this is just a more readable shortcut then?
    //#define timer_control  TACTL // so, this is just a more readable shortcut then?
    // with a define inside a namespace, you won't be able to assign to it
    // i.e. it Timer0_A3::timer_control will turn into Timer0_A3::TACTL which does not exist
    namespace timer_control {
        constexpr inline void set(decltype(TACTL) new_val) {
            TACTL = new_val;
        }

        template<decltype(TACTL) new_val>
        constexpr inline void set(void) {
            TACTL = new_val;
        }

        constexpr inline decltype(TACTL) get(void) {
            return TACTL;
        }
    };

    //BitLogic::BitFieldSignature<decltype(TACTL),
    //    //timer_control, // you cannot use a reference as a non-type template paremeter... C++ gotchas suck
    //    TACTL,
    //    8,
    //    3> input_clock_select;
    namespace input_clock_select {
        constexpr inline decltype(TACTL) get(void) {return BitLogic::get<decltype(TACTL), TACTL, 8, 3>();}
        constexpr inline void set(decltype(TACTL) val) {BitLogic::set<decltype(TACTL), TACTL, 8, 3>(val);}
        //constexpr inline decltype(TACTL) mask(decltype(TACTL) val) {return BitLogic::mask<decltype(TACTL), TACTL, 8, 3>(val);}
        using BitLogic::mask;
        //template decltype(TACTL) mask<decltype(TACTL), TACTL, 8, 3>(decltype(TACTL) val);
        constexpr inline decltype(TACTL) mask(decltype(TACTL) val) {return mask<decltype(TACTL), TACTL, 8, 3>(val);}
        //it does not get inlined!

        template<decltype(TACTL) val>
        constexpr static inline decltype(TACTL) mask(void) {
            return BitLogic::maskRegField<8, 3>(val);
        }
    };

    //using BitLogic::mask;
    //constexpr inline decltype(TACTL) input_clock_select_mask(decltype(TACTL) val) {return mask<decltype(TACTL), TACTL, 8, 3>(val);}
    //constexpr static inline RegT mask(RegT val) {return maskRegField<field_offset, field_width>(val);}
    using BitLogic::maskRegField;
    constexpr static inline decltype(TACTL) input_clock_select_mask(decltype(TACTL) val) {return maskRegField<8, 3>(val);}
    // still no inline
    // let's try BitLogic::maskRegField directly in main?
    // the point is that I want to hide the bit offset and width behind a meaningful name

    constexpr unsigned INPUT_CLOCK_TACLK = 0;
    constexpr unsigned INPUT_CLOCK_ACLK  = 1;
    constexpr unsigned INPUT_CLOCK_SMCLK = 2;
    constexpr unsigned INPUT_CLOCK_INCLK = 3;

    //BitLogic::BitFieldSignature<decltype(TACTL), TACTL, 6, 2> clock_divider;
    namespace clock_divider {
        template<decltype(TACTL) val>
        constexpr static inline decltype(TACTL) mask(void) {
            return BitLogic::maskRegField<6, 2>(val);
        }
    };

    constexpr unsigned DIVIDE_0 = 0;
    constexpr unsigned DIVIDE_2 = 1;
    constexpr unsigned DIVIDE_4 = 2;
    constexpr unsigned DIVIDE_8 = 3;

    //BitLogic::BitFieldSignature<decltype(TACTL), TACTL, 4, 2> mode_control;
    namespace mode_control {
        template<decltype(TACTL) val>
        constexpr static inline decltype(TACTL) mask(void) {
            return BitLogic::maskRegField<4, 2>(val);
        }
    };

    constexpr unsigned MODE_STOP = 0;
    constexpr unsigned MODE_UPTO = 1;
    constexpr unsigned MODE_CONT = 2;
    constexpr unsigned MODE_UPDN = 3;

    BitLogic::BitFieldSignature<decltype(TACTL), TACTL, 2, 1> clear;
    BitLogic::BitFieldSignature<decltype(TACTL), TACTL, 1, 1> overflow_interrupt_enable;
    BitLogic::BitFieldSignature<decltype(TACTL), TACTL, 0, 1> overflow_flag;

    decltype(CCTL0)& ccr0_control   = CCTL0;
    //BitLogic::BitFieldSignature<base_type<decltype(CCTL0)>, CCTL0, 4, 1> cc0_interrupt_enable;
    namespace cc0_interrupt_enable {
        template<decltype(CCTL0) new_val>
        constexpr inline void set(void) {CCTL0 = BitLogic::maskRegField<4, 1>(new_val);}
    };

    /* with the namespaces above we are back to below 500B program size
     * from 630B bloat of the poor not-inlined compile-time code

     Programming...
     Writing  488 bytes at fc00 [section: .text]...
     Writing   16 bytes at fdec [section: .cinit]...
    */

    //decltype(CCR0)&  ccr0           = CCR0;
    namespace ccr0 {
        inline void set(decltype(CCR0) val) {CCR0 = val;} // the compiler does not inline even this
        inline decltype(CCR0) get(void) {return CCR0;}
    };
};

namespace LaunchpadBoards {
    // use with something like:
    // namespace Board = LaunchpadBoards::MSP_EXP430G2;

    namespace MSP_EXP430G2 {
        // my board says "MSP-EXP430G2"
        // https://www.ti.com/lit/ug/slau318g/slau318g.pdf?ts=1731831644145
        // https://dev.ti.com/tirex/explore/node?devtools=MSP-EXP430G2&node=A__ABjGDxJw12fXjxtYpSJOow__msp430_devtools__FUz-xrs__LATEST

        /// \brief red led
        constexpr unsigned LED1 = IOPort1Ctrl::PIN0;
        /// \brief green led
        constexpr unsigned LED2 = IOPort1Ctrl::PIN6;
    };
};
