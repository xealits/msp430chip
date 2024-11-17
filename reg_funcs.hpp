#pragma once

/** \brief Make a bit mask in an unsigned of type RegT
 * 
 * Use ~regMask<>() to make a mask of zeros.
 */
template<typename RegT, RegT mask, unsigned bit_n>
constexpr RegT regMaskUnfold() {
	static_assert((bit_n) < sizeof(RegT) * 8,
            "The requested bit does not fit in the register");
    return mask | (0x1 << bit_n);
}

template<typename RegT, RegT mask, unsigned bit_n, unsigned... rest_bit_ns>
constexpr RegT regMaskUnfold() {
	static_assert((bit_n) < sizeof(RegT) * 8,
            "The requested bit does not fit in the register");
    return regMaskUnfold<RegT, mask | (0x1 << bit_n), rest_bit_ns...>();
}

template<typename Type>
using base_type = typename std::remove_reference<Type>::type;

template<typename RegT, unsigned bit_n, unsigned... rest_bit_ns>
constexpr base_type<RegT>
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
    RegT n_bits_mask = nBitsMask<n_bits, RegT>();
    return (a_reg >> offset) & n_bits_mask;
}

template<unsigned offset, unsigned n_bits, typename RegT>
constexpr inline void setRegField(RegT& a_reg, RegT new_val) {
    static_assert((offset+n_bits) <= sizeof(RegT) * 8,
            "The requested bit field does not fit in the register");

    RegT n_bits_mask = nBitsMask<n_bits, RegT>();

    // make sure the input is in the mask
    new_val &= n_bits_mask;

    // clear the field in the reg
    a_reg &= ~(n_bits_mask << offset);
    // add the input into the field
    a_reg |= new_val << offset;
}

template<typename RegT, RegT& t_reg, unsigned t_offset, unsigned t_n_bits>
struct BitFieldSignature {
	//static const char* s_name;
	//static const BitFieldInfo<t_offset, t_n_bits> info;
	static RegT get(void) {return getRegField<t_offset, t_n_bits>(t_reg);}
	static void set(RegT val) {return setRegField<t_offset, t_n_bits>(t_reg, val);}
	//static BitFieldInfo info(void) {return std::make_tuple(t_offset, t_n_bits);};
	//static BitFieldInfo info(void) {return {t_offset, t_n_bits, s_name};};
};

using RegType = uint16_t;

namespace IOPort1Ctrl {
  constexpr unsigned NUMBER_OF_PINS = 8;
  // not sure if it is worth to name pins
  // it just links the board namespace to this one, the chip namespace
  enum PINS {PIN0=0, PIN1=1, PIN2=2, PIN3=3, PIN4=4, PIN5=5, PIN6=6, PIN7=7};

  decltype(P1IN)&   input         = P1IN;                               /* Port 1 Input */
  decltype(P1OUT)&  output        = P1OUT;                              /* Port 1 Output */
  decltype(P1DIR)&  direction_reg = P1DIR;                              /* Port 1 Direction */
  // they use extern volatile unsigned char -- it is not a pointer.
  // a symbol itself is probably mapped as needed?
  // use a reference then
  //decltype(P1IFG)    = P1IFG;                              /* Port 1 Interrupt Flag */
  //decltype(P1IES)    = P1IES;                              /* Port 1 Interrupt Edge Select */
  //decltype(P1IE)     = P1IE;                               /* Port 1 Interrupt Enable */
  //decltype(P1SEL)    = P1SEL;                              /* Port 1 Selection */
  //decltype(P1SEL2)   = P1SEL2;                             /* Port 1 Selection 2 */
  //decltype(P1REN)    = P1REN;                              /* Port 1 Resistor Enable */

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
  constexpr void setDirectionMask(void) {
    //direction_reg = regMask<std::remove_reference<decltype(direction_reg)>::type, 0x0, bit_ns...>();
    //direction_reg = regMaskUnfold<base_type<decltype(direction_reg)>, 0x0, bit_ns...>();
    direction_reg = regMask<decltype(direction_reg), bit_ns...>();
  }

  // the output on the pins
  template<unsigned... bit_ns>
  constexpr void toggleOutput(void) {
    output ^= regMask<decltype(output), bit_ns...>();
  }

  template<unsigned... bit_ns>
  constexpr void setOutput(void) {
    output |= regMask<decltype(output), bit_ns...>();
  }

  template<unsigned... bit_ns>
  constexpr void unsetOutput(void) {
    output &= ~regMask<decltype(output), bit_ns...>();
  }
};

namespace BoardPins {
    /// \brief red led
	constexpr unsigned LED1 = IOPort1Ctrl::PIN0;
    /// \brief green led
	constexpr unsigned LED2 = IOPort1Ctrl::PIN6;
};
