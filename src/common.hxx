
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
    
}
