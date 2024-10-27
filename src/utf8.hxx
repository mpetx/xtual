
namespace xtual
{

    template <typename charT, std::output_iterator<charT> Iter, std::sentinel_for<Iter> Sent, std::invocable<Iter &, Sent, char8_t> Writ>
    bool utf8_encode(Iter &i, Sent s, char32_t ch, Writ write)
    {
        if (!is_code_point(ch))
        {
            return false;
        }

        if ((ch & ~static_cast<char32_t>(0x7f)) == 0)
        {
            return write(i, s, static_cast<char8_t>(ch));
        }
        else if ((ch & ~static_cast<char32_t>(0x07'ff)) == 0)
        {
            char8_t c1 = static_cast<char8_t>(0xc0);
            char8_t c2 = static_cast<char8_t>(0x80);

            c1 |= static_cast<char8_t>((ch >> 6) & 0x1f);
            c2 |= static_cast<char8_t>(ch & 0x3f);

            return write(i, s, c1) && write(i, s, c2);
        }
        else if ((ch & ~static_cast<char32_t>(0xff'ff)) == 0)
        {
            char8_t c1 = static_cast<char8_t>(0xe0);
            char8_t c2 = static_cast<char8_t>(0x80);
            char8_t c3 = static_cast<char8_t>(0x80);

            c1 |= static_cast<char8_t>((ch >> 12) & 0x0f);
            c2 |= static_cast<char8_t>((ch >> 6) & 0x3f);
            c3 |= static_cast<char8_t>(ch & 0x3f);

            return write(i, s, c1)
                && write(i, s, c2)
                && write(i, s, c3);
        }
        else
        {
            char8_t c1 = static_cast<char8_t>(0xf0);
            char8_t c2 = static_cast<char8_t>(0x80);
            char8_t c3 = static_cast<char8_t>(0x80);
            char8_t c4 = static_cast<char8_t>(0x80);

            c1 |= static_cast<char8_t>((ch >> 18) & 0x07);
            c2 |= static_cast<char8_t>((ch >> 12) & 0x3f);
            c3 |= static_cast<char8_t>((ch >> 6) & 0x3f);
            c4 |= static_cast<char8_t>(ch & 0x3f);

            return write(i, s, c1)
                && write(i, s, c2)
                && write(i, s, c3)
                && write(i, s, c4);
        }
    }
    
    template <std::output_iterator<char8_t> Iter, std::sentinel_for<Iter> Sent>
    bool encode_as_u8(Iter &i, Sent s, char32_t ch)
    {
        return utf8_encode<char8_t>(i, s, ch, [](Iter &i, Sent s, char8_t ch) {
            if (i == s)
            {
                return false;
            }

            *i++ = ch;

            return true;
        });
    }

    template <byte_like byteT, std::output_iterator<byteT> Iter, std::sentinel_for<Iter> Sent>
    bool encode_as_b8(Iter &i, Sent s, char32_t ch)
    {
        return utf8_encode<byteT>(i, s, ch, [](Iter &i, Sent s, char8_t ch) {
            if (i == s)
            {
                return false;
            }

            *i++ = static_cast<byteT>(static_cast<std::byte>(ch));

            return true;
        });
    }

    template <typename charT, std::input_iterator Iter, std::sentinel_for<Iter> Sent, std::invocable<Iter &, Sent> Rdr>
    requires std::convertible_to<std::iter_value_t<Iter>, charT>
    std::optional<char32_t> utf8_decode(Iter &i, Sent s, Rdr read)
    {
        auto opt1 = read(i, s);

        if (!opt1.has_value())
        {
            return std::nullopt;
        }

        char8_t w1 = opt1.value();

        if (has_0_bit_tag(w1))
        {
            return static_cast<char32_t>(w1);
        }
        else if (has_2_bit_tag(w1))
        {
            auto opt2 = read(i, s);

            if (!opt2.has_value())
            {
                return std::nullopt;
            }

            char8_t w2 = opt2.value();

            if (!has_1_bit_tag(w2))
            {
                return std::nullopt;
            }

            return ((static_cast<char32_t>(w1) & U'\x1f') << 6)
                | (static_cast<char32_t>(w2) & U'\x3f');
        }
        else if (has_3_bit_tag(w1))
        {
            auto opt2 = read(i, s);

            if (!opt2.has_value())
            {
                return std::nullopt;
            }

            char8_t w2 = opt2.value();

            if (!has_1_bit_tag(w2))
            {
                return std::nullopt;
            }

            auto opt3 = read(i, s);

            if (!opt3.has_value())
            {
                return std::nullopt;
            }

            char8_t w3 = opt3.value();

            if (!has_1_bit_tag(w3))
            {
                return std::nullopt;
            }

            return ((static_cast<char32_t>(w1) & U'\x0f') << 12)
                | ((static_cast<char32_t>(w2) & U'\x3f') << 6)
                | (static_cast<char32_t>(w3) & U'\x3f');
        }
        else if (has_4_bit_tag(w1))
        {
            auto opt2 = read(i, s);

            if (!opt2.has_value())
            {
                return std::nullopt;
            }

            char8_t w2 = opt2.value();

            if (!has_1_bit_tag(w2))
            {
                return std::nullopt;
            }

            auto opt3 = read(i, s);

            if (!opt3.has_value())
            {
                return std::nullopt;
            }

            char8_t w3 = opt3.value();

            if (!has_1_bit_tag(w3))
            {
                return std::nullopt;
            }

            auto opt4 = read(i, s);

            if (!opt4.has_value())
            {
                return std::nullopt;
            }

            char8_t w4 = opt4.value();

            if (!has_1_bit_tag(w4))
            {
                return std::nullopt;
            }
            
            return ((static_cast<char32_t>(w1) & U'\x07') << 18)
                | ((static_cast<char32_t>(w2) & U'\x3f') << 12)
                | ((static_cast<char32_t>(w3) & U'\x3f') << 6)
                | (static_cast<char32_t>(w4) & U'\x3f');
        }
        else
        {
            return std::nullopt;
        }
    }

    template <std::input_iterator Iter, std::sentinel_for<Iter> Sent>
    requires std::convertible_to<std::iter_value_t<Iter>, char8_t>
    std::optional<char32_t> decode_from_u8(Iter &i, Sent s)
    {
        return utf8_decode<char8_t>(i, s, [](Iter &i, Sent s) -> std::optional<char8_t> {
            if (i == s)
            {
                return std::nullopt;
            }

            return *i++;
        });
    }

    template <byte_like byteT, std::input_iterator Iter, std::sentinel_for<Iter> Sent>
    requires std::convertible_to<std::iter_value_t<Iter>, byteT>
    std::optional<char32_t> decode_from_b8(Iter &i, Sent s)
    {
        return utf8_decode<byteT>(i, s, [](Iter &i, Sent s) -> std::optional<char8_t> {
            if (i == s)
            {
                return std::nullopt;
            }

            return static_cast<char8_t>(static_cast<std::byte>(*i++));
        });
    }
    
}
