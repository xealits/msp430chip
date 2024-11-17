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

