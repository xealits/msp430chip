#pragma once
#include <type_traits>

template <typename Type>
using base_type = typename std::remove_reference<Type>::type;

namespace bitlogic {
// C++14 does not allow auto typename: error "auto" is not allowed here
// template<auto*const reg_addr, unsigned offset, unsigned width, typename
// RegType=std::decay_t<decltype(reg_addr)>> struct BitField
template <typename RegType, RegType& reg, unsigned offset, unsigned width>
struct BitField {
  // Ti compiler says "std has no member is_pointer_v"
  // static_assert(!std::is_pointer_v<RegType>, "RegType cannot be a pointer, it
  // is a reg or a reference");
  static_assert(!std::is_pointer<RegType>::value,
                "RegType cannot be a pointer, it is a reg or a reference");
  using BaseRegT = std::decay_t<RegType>;

  static constexpr unsigned reg_size = sizeof(RegType) * 8;
  static_assert(offset < reg_size, "bit field offset must fit within register");
  static_assert(width > 0, "bit field width must be > 0");
  static_assert(width <= (reg_size - offset),
                "bit field width must fit from offset to the reg end");

  static constexpr BaseRegT mask =
      width < reg_size ? (0b1 << width) - 1 : ~BaseRegT{0};

  static constexpr inline BaseRegT maskValue(BaseRegT value) {
    return (value & mask) << offset;
  }

  static constexpr inline BaseRegT set(BaseRegT value) {
    return maskValue(value);
  }

  static constexpr RegType read(void) { return (reg >> offset) & mask; }
  static constexpr void write(BaseRegT field_val) {
    // auto new_value = reg | ((field_val & mask) << offset);
    // reg = new_value;
    // reg |= (field_val & mask) << offset;
    BaseRegT new_val = (field_val & mask) << offset;
    reg = new_val;
  }
};

/** \brief Make a bit mask in an unsigned of type RegT
 *
 * Use ~regMask<>() to make a mask of zeros.
 */
template <typename RegT, RegT mask, unsigned bit_n>
inline constexpr RegT regMaskUnfold() {
  static_assert((bit_n) < sizeof(RegT) * 8,
                "The requested bit does not fit in the register");
  return mask | (0x1 << bit_n);
}

template <typename RegT, RegT mask, unsigned bit_n, unsigned... rest_bit_ns>
inline constexpr RegT regMaskUnfold() {
  static_assert((bit_n) < sizeof(RegT) * 8,
                "The requested bit does not fit in the register");
  return regMaskUnfold<RegT, mask | (0x1 << bit_n), rest_bit_ns...>();
}

template <typename RegT, unsigned bit_n, unsigned... rest_bit_ns>
inline constexpr base_type<RegT> regMask() {
  return regMaskUnfold<base_type<RegT>, 0x0, bit_n, rest_bit_ns...>();
}

enum LOGIC_LEVELS { HIGH, LOW };

// TODO: check the bit shifting edge cases
template <unsigned bit_n, typename RegT>  // RegT should have max number of bits
constexpr void setBit(RegT& reg, LOGIC_LEVELS level) {
  static_assert((bit_n) < sizeof(RegT) * 8,
                "The requested bit does not fit in the register");

  if (level == LOGIC_LEVELS::LOW)
    reg &= ~RegT(0x1 << bit_n);
  else
    reg |= (0x1 << bit_n);
}

/** \brief Return a mask of N low bits set
 */
template <unsigned n_bits, typename RegT>
constexpr inline RegT nBitsMask() {
  static_assert(n_bits > 0, "Cannot have bit field of 0 bits");
  static_assert((n_bits) <= sizeof(RegT) * 8,
                "The requested bit field width does not fit in the register");

  // to handle the edge case of overflow: n_bits == sizeof(RegT) * 8
  // I cannot just n_bits_mask = (1 << n_bits) - 1
  // also, I don't want to rely on if constexpr of C++17
  // hence:
  RegT n_bits_mask = 1 << (n_bits - 1);
  if (n_bits > 1) n_bits_mask |= ((RegT)1 << (n_bits - 1)) - 1;

  return n_bits_mask;
}

template <unsigned offset, unsigned n_bits, typename RegT>
constexpr inline RegT getRegField(const RegT& a_reg) {
  static_assert((offset + n_bits) <= sizeof(RegT) * 8,
                "The requested bit field does not fit in the register");
  // TODO: assert RegT is not a reference thing?
  const RegT n_bits_mask = nBitsMask<n_bits, RegT>();
  RegT reg_val = a_reg;
  return (reg_val >> offset) & n_bits_mask;
}

template <unsigned offset, unsigned n_bits, typename RegT>
constexpr inline void setRegField(RegT& a_reg, RegT new_val) {
  static_assert((offset + n_bits) <= sizeof(RegT) * 8,
                "The requested bit field does not fit in the register");
  // TODO: assert RegT is not a reference thing?

  const RegT n_bits_mask = nBitsMask<n_bits, RegT>();

  // make sure the input is in the mask
  new_val &= n_bits_mask;

  // clear the field in the reg value
  RegT reg_val = a_reg;
  reg_val &= ~(n_bits_mask << offset);
  // add the input into the field
  reg_val |= new_val << offset;
  // write the new value to the HW memory:
  a_reg = reg_val;
}

template <unsigned offset, const unsigned n_bits, typename RegT>
constexpr inline RegT maskRegField(RegT new_val) {
  static_assert((offset + n_bits) <= sizeof(RegT) * 8,
                "The requested bit field does not fit in the register");
  // TODO: assert RegT is not a reference thing?

  const RegT n_bits_mask = nBitsMask<n_bits, RegT>();

  // make sure the input is in the mask
  new_val &= n_bits_mask;

  // clear the field in the reg
  RegT reg_val = 0x0;  // a_reg;
  // reg_val &= ~(n_bits_mask << offset);

  // add the input into the field
  reg_val |= (new_val << offset);
  // return the new value
  // instead of setting it to some ref
  return reg_val;
}

/** struct is just a handy way to bundle a couple functions that act on the same
 * refreference I could just use namespace, but I could not template it.
 *
 *  But it looks like Ti compiler cl430 does not inline this stuff well.
 */
template <typename RegT, RegT& t_reg, unsigned t_offset, unsigned t_n_bits>
struct BitFieldSignature {
  // static const char* s_name;
  // static const BitFieldInfo<t_offset, t_n_bits> info;
  constexpr static inline RegT get(void) {
    return getRegField<t_offset, t_n_bits>(t_reg);
  }
  // static inline void set(RegT val) {return setRegField<t_offset,
  // t_n_bits>(t_reg, val);}
  //  clang compiles it, but Ti compiler cl430 cannot:
  //  "../msp430_cpp/reg_funcs.hpp", line 122: error #306: no instance of
  //  function template "BitLogic::setRegField" matches the argument list
  //     argument types are: (volatile unsigned int, volatile unsigned int)
  //   detected during instantiation of "void BitLogic::BitFieldSignature<RegT,
  //   t_reg, t_offset, t_n_bits>::set(RegT) [with RegT=base_type<volatile
  //   unsigned int>, t_reg=TA0CCTL0, t_offset=4U, t_n_bits=1U]" at line 142 of
  //   "../msp430g2xx2_1_vlo.cpp"
  //  like this should be easier:
  constexpr static inline void set(RegT val) {
    t_reg = maskRegField<t_offset, t_n_bits>(val);
  }

  constexpr static inline RegT mask(RegT val) {
    return maskRegField<t_offset, t_n_bits>(val);
  }
  // static BitFieldInfo info(void) {return std::make_tuple(t_offset,
  // t_n_bits);}; static BitFieldInfo info(void) {return {t_offset, t_n_bits,
  // s_name};};
};

template <typename RegT, RegT& t_reg, unsigned field_offset,
          unsigned field_width>
constexpr static inline RegT get(void) {
  return getRegField<field_offset, field_width>(t_reg);
}

template <typename RegT, RegT& t_reg, unsigned field_offset,
          unsigned field_width>
constexpr static inline void set(RegT val) {
  t_reg = maskRegField<field_offset, field_width>(val);
}

template <typename RegT, RegT& t_reg, unsigned field_offset,
          unsigned field_width>
constexpr static inline RegT mask(RegT val) {
  return maskRegField<field_offset, field_width>(val);
}
};  // namespace bitlogic
