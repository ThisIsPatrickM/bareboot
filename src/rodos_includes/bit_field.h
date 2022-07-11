#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace RODOS {
// NOLINTBEGIN
template <typename integer_t>
struct Integer {
public:
    using value_t = integer_t;
    value_t value = 0;

    inline void write(const value_t value) { this->value = value; }

    constexpr value_t read() const { return this->value; }
};

template <typename value_t, value_t offset_, uint8_t length_ = 1>
struct SubValue {
    static_assert(offset_ + length_ <= sizeof(value_t) * 8);
    static constexpr uint8_t length = length_;
    static constexpr uint8_t offset = offset_;
    static constexpr value_t bitMask = static_cast<value_t>(((1ul << length) - 1) << offset);
    static constexpr bool isSubValue { true };

    const value_t shiftedValue;

    constexpr SubValue(value_t value)
        : shiftedValue(static_cast<value_t>(value << offset) & bitMask) {};
    constexpr SubValue(std::byte value)
        : SubValue { std::to_integer<value_t>(value) } {};

    template <value_t otherOffset, uint8_t otherLength>
    constexpr value_t operator|(const SubValue<value_t, otherOffset, otherLength>& otherValue)
    {
        return shiftedValue | otherValue.shiftedValue;
    }

    constexpr value_t operator|(value_t value) { return shiftedValue | value; }

    constexpr friend value_t operator|(value_t lhs, SubValue<value_t, offset, length> rhs)
    {
        return rhs | lhs;
    }

    constexpr operator value_t() { return shiftedValue; }
};

template <typename MemoryCell>
struct BitField : public MemoryCell {
public:
    using MemoryCell::read;
    using value_t = typename MemoryCell::value_t;
    template <value_t offset_, uint8_t length_ = 1>
    using SubValue = SubValue<value_t, offset_, length_>;

    constexpr void setBits(const value_t setMask) { write(MemoryCell::read() | setMask); }

    constexpr void clearBits(const value_t clearMask)
    {
        write(MemoryCell::read() & (value_t)(~clearMask));
    }

    constexpr value_t readBits(const value_t readMask) const
    {
        return MemoryCell::read() & readMask;
    }

    constexpr void clearAndSetBits(const value_t clearMask, const value_t setMask)
    {
        MemoryCell::write((MemoryCell::read() & (value_t)(~clearMask)) | setMask);
    }

    /**
     * @brief does a read-modify-write sequence on the register, modifying the given subvalues and
     * leaving all other bits unmodified
     */
    template <class... subValues>
    constexpr void set(subValues... args)
    {
        constexpr value_t clearMask = (subValues::bitMask | ...);
        value_t setMask = (args.shiftedValue | ...);
        clearAndSetBits(clearMask, setMask);
    }

    /**
     * @brief writes arg into the MemoryCell and overwrites any previous value
     */
    constexpr void write(value_t arg) { MemoryCell::write(arg); }

    /**
     * @brief writes all of the given subvalues into the MemoryCell and sets all other bits to 0
     */
    template <
        class... subValues,
        typename = typename std::enable_if_t<(subValues::isSubValue && ...), void>>
    constexpr void write(subValues... args)
    {
        MemoryCell::write((args.shiftedValue | ...));
    }

    template <class subValue>
    constexpr value_t read() const
    {
        return static_cast<value_t>((MemoryCell::read() & subValue::bitMask) >> subValue::offset);
    }

    template <typename OtherMemoryCell>
    constexpr operator BitField<OtherMemoryCell>()
    {
        return { MemoryCell::read() };
    }
};

using UInt8BitField = BitField<Integer<uint8_t>>;
using UInt16BitField = BitField<Integer<uint16_t>>;
using UInt32BitField = BitField<Integer<uint32_t>>;

template <uint8_t offset_, uint8_t length_ = 1>
using UInt8SubValue = BitField<Integer<uint8_t>>::SubValue<offset_, length_>;
template <uint16_t offset_, uint8_t length_ = 1>
using UInt16SubValue = BitField<Integer<uint16_t>>::SubValue<offset_, length_>;
template <uint32_t offset_, uint8_t length_ = 1>
using UInt32SubValue = BitField<Integer<uint32_t>>::SubValue<offset_, length_>;
// NOLINTEND
}
