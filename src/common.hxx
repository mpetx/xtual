
namespace xtual
{

    template <typename T>
    concept byte_like =
        std::same_as<T, std::byte>
        || std::same_as<T, char>
        || std::same_as<T, unsigned char>
        || std::same_as<T, signed char>
        || std::same_as<T, std::uint8_t>
        || std::same_as<T, std::int8_t>;

    constexpr bool is_surrogate(char32_t ch)
    {
        return (ch >> 11) == 0x1b;
    }

    constexpr bool is_surrogate(char16_t ch)
    {
        return (ch >> 11) == 0x1b;
    }
    
    constexpr bool is_high_surrogate(char32_t ch)
    {
        return (ch >> 10) == 0x36;
    }

    constexpr bool is_high_surrogate(char16_t ch)
    {
        return (ch >> 10) == 0x36;
    }

    constexpr bool is_low_surrogate(char32_t ch)
    {
        return (ch >> 10) == 0x37;
    }

    constexpr bool is_low_surrogate(char16_t ch)
    {
        return (ch >> 10) == 0x37;
    }
    
    constexpr bool is_code_point(char32_t ch)
    {
        return !is_surrogate(ch) && ch <= U'\x10ffff';
    }

    constexpr bool has_0_bit_tag(char8_t ch)
    {
        return (ch >> 7) == 0;
    }

    constexpr bool has_1_bit_tag(char8_t ch)
    {
        return (ch >> 6) == 0x02;
    }

    constexpr bool has_2_bit_tag(char8_t ch)
    {
        return (ch >> 5) == 0x06;
    }

    constexpr bool has_3_bit_tag(char8_t ch)
    {
        return (ch >> 4) == 0x0e;
    }

    constexpr bool has_4_bit_tag(char8_t ch)
    {
        return (ch >> 3) == 0x1e;
    }

}
