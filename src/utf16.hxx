
namespace xtual
{

    template <typename charT, std::output_iterator<charT> Iter, std::sentinel_for<Iter> Sent, std::invocable<Iter &, Sent, char16_t> Writ>
    bool utf16_encode(Iter &i, Sent s, char32_t ch, Writ write)
    {
        if (!is_code_point(ch))
        {
            return false;
        }
        
        if ((ch & ~U'\xffff') == 0)
        {
            return write(i, s, static_cast<char16_t>(ch));
        }
        else
        {
            char32_t u = ch - U'\x10000';
            
            char16_t w1 = u'\xd800';
            char16_t w2 = u'\xdc00';

            w1 |= static_cast<char16_t>((u >> 10) & U'\x3ff');
            w2 |= static_cast<char16_t>(u & U'\x3ff');

            return write(i, s, w1) && write(i, s, w2);
        }
    }

    template <std::output_iterator<char16_t> Iter, std::sentinel_for<Iter> Sent>
    bool encode_as_u16(Iter &i, Sent s, char32_t ch)
    {
        return utf16_encode<char16_t>(i, s, ch, [](Iter &i, Sent s, char16_t ch) {
            if (i == s)
            {
                return false;
            }

            *i++ = ch;

            return true;
        });
    }

    template <byte_like byteT, std::output_iterator<byteT> Iter, std::sentinel_for<Iter> Sent>
    bool encode_as_b16be(Iter &i, Sent s, char32_t ch)
    {
        return utf16_encode<byteT>(i, s, ch, [](Iter &i, Sent s, char16_t ch) {
            if (i == s)
            {
                return false;
            }

            *i++ = static_cast<byteT>(static_cast<std::byte>((ch >> 8) & 0xff));
            
            if (i == s)
            {
                return false;
            }

            *i++ = static_cast<byteT>(static_cast<std::byte>(ch & 0xff));

            return true;
        });
    }

    template <byte_like byteT, std::output_iterator<byteT> Iter, std::sentinel_for<Iter> Sent>
    bool encode_as_b16le(Iter &i, Sent s, char32_t ch)
    {
        return utf16_encode<byteT>(i, s, ch, [](Iter &i, Sent s, char16_t ch) {
            if (i == s)
            {
                return false;
            }

            *i++ = static_cast<byteT>(static_cast<std::byte>(ch & 0xff));
            
            if (i == s)
            {
                return false;
            }

            *i++ = static_cast<byteT>(static_cast<std::byte>((ch >> 8) & 0xff));

            return true;
        });
    }

    template <typename charT, std::input_iterator Iter, std::sentinel_for<Iter> Sent, std::invocable<Iter &, Sent> Rdr>
    requires std::convertible_to<std::iter_value_t<Iter>, charT>
    std::optional<char32_t> utf16_decode(Iter &i, Sent s, Rdr read)
    {
        auto opt1 = read(i, s);

        if (!opt1.has_value())
        {
            return std::nullopt;
        }

        char16_t w1 = opt1.value();

        if (!is_surrogate(w1))
        {
            return static_cast<char32_t>(w1);
        }
        
        if (!is_high_surrogate(w1))
        {
            return std::nullopt;
        }

        auto opt2 = read(i, s);

        if (!opt2.has_value())
        {
            return std::nullopt;
        }

        char16_t w2 = opt2.value();

        if (!is_low_surrogate(w2))
        {
            return std::nullopt;
        }

        char32_t u = ((static_cast<char32_t>(w1) & U'\x3ff') << 10)
            | (static_cast<char32_t>(w2) & U'\x3ff');

        return u + U'\x10000';
    }

    template <std::input_iterator Iter, std::sentinel_for<Iter> Sent>
    requires std::convertible_to<std::iter_value_t<Iter>, char16_t>
    std::optional<char32_t> decode_from_u16(Iter &i, Sent s)
    {
        return utf16_decode<char16_t>(i, s, [](Iter &i, Sent s) -> std::optional<char16_t> {
            if (i == s)
            {
                return std::nullopt;
            }

            return *i++;
        });
    }

    template <byte_like byteT, std::input_iterator Iter, std::sentinel_for<Iter> Sent>
    requires std::convertible_to<std::iter_value_t<Iter>, byteT>
    std::optional<char32_t> decode_from_b16be(Iter &i, Sent s)
    {
        return utf16_decode<byteT>(i, s, [](Iter &i, Sent s) -> std::optional<char16_t> {
            if (i == s)
            {
                return std::nullopt;
            }

            byteT b1 = *i++;
            char16_t c1 = static_cast<char16_t>(static_cast<std::byte>(b1));

            if (i == s)
            {
                return std::nullopt;
            }

            byteT b2 = *i++;
            char16_t c2 = static_cast<char16_t>(static_cast<std::byte>(b2));

            return (c1 << 8) | c2;
        });
    }

    template <byte_like byteT, std::input_iterator Iter, std::sentinel_for<Iter> Sent>
    requires std::convertible_to<std::iter_value_t<Iter>, byteT>
    std::optional<char32_t> decode_from_b16le(Iter &i, Sent s)
    {
        return utf16_decode<byteT>(i, s, [](Iter &i, Sent s) -> std::optional<char16_t> {
            if (i == s)
            {
                return std::nullopt;
            }

            byteT b1 = *i++;
            char16_t c1 = static_cast<char16_t>(static_cast<std::byte>(b1));

            if (i == s)
            {
                return std::nullopt;
            }

            byteT b2 = *i++;
            char16_t c2 = static_cast<char16_t>(static_cast<std::byte>(b2));

            return c1 | (c2 << 8);
        });
    }
    
}
