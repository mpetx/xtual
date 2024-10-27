
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

    constexpr bool is_ascii(char8_t ch)
    {
        return (ch >> 7) == 0;
    }

    constexpr bool is_2_sequence_indicator(char8_t ch)
    {
        return (ch >> 5) == 0x06;
    }

    constexpr bool is_3_sequence_indicator(char8_t ch)
    {
        return (ch >> 4) == 0x0e;
    }

    constexpr bool is_4_sequence_indicator(char8_t ch)
    {
        return (ch >> 3) == 0x1e;
    }

    constexpr bool is_sequence_constituent(char8_t ch)
    {
        return (ch >> 6) == 0x02;
    }

    template <std::input_iterator Iter, std::sentinel_for<Iter> Sent, std::invocable<Iter &, Sent> Rdr>
    std::optional<std::pair<char8_t, char8_t>> read_2_units(Iter &i, Sent s, Rdr read)
    {
        auto opt1 = read(i, s);

        if (!opt1.has_value())
        {
            return std::nullopt;
        }

        char8_t c1 = opt1.value();

        auto opt2 = read(i, s);

        if (!opt2.has_value())
        {
            return std::nullopt;
        }

        char8_t c2 = opt2.value();

        return std::pair { c1, c2 };
    }

    template <std::input_iterator Iter, std::sentinel_for<Iter> Sent, std::invocable<Iter &, Sent> Rdr>
    std::optional<std::tuple<char8_t, char8_t, char8_t>> read_3_units(Iter &i, Sent s, Rdr read)
    {
        auto opt1 = read(i, s);

        if (!opt1.has_value())
        {
            return std::nullopt;
        }

        char8_t c1 = opt1.value();

        auto opt2 = read(i, s);

        if (!opt2.has_value())
        {
            return std::nullopt;
        }

        char8_t c2 = opt2.value();

        auto opt3 = read(i, s);

        if (!opt3.has_value())
        {
            return std::nullopt;
        }

        char8_t c3 = opt3.value();
        
        return std::tuple { c1, c2, c3 };
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

        if (is_ascii(w1))
        {
            return static_cast<char32_t>(w1);
        }
        else if (is_2_sequence_indicator(w1))
        {
            auto opt2 = read(i, s);

            if (!opt2.has_value())
            {
                return std::nullopt;
            }

            char8_t w2 = opt2.value();

            if (!is_sequence_constituent(w2))
            {
                return std::nullopt;
            }

            char32_t ch= ((static_cast<char32_t>(w1) & U'\x1f') << 6)
                | (static_cast<char32_t>(w2) & U'\x3f');

            if ((ch & ~U'\x7f') == 0 || !is_code_point(ch))
            {
                return std::nullopt;
            }

            return ch;
        }
        else if (is_3_sequence_indicator(w1))
        {
            auto opt_rest = read_2_units(i, s, read);

            if (!opt_rest.has_value())
            {
                return std::nullopt;
            }

            auto [ w2, w3 ] = opt_rest.value();

            if (!is_sequence_constituent(w2) || !is_sequence_constituent(w3))
            {
                return std::nullopt;
            }

            char32_t ch = ((static_cast<char32_t>(w1) & U'\x0f') << 12)
                | ((static_cast<char32_t>(w2) & U'\x3f') << 6)
                | (static_cast<char32_t>(w3) & U'\x3f');

            if ((ch & ~U'\x7ff') == 0 || !is_code_point(ch))
            {
                return std::nullopt;
            }

            return ch;
        }
        else if (is_4_sequence_indicator(w1))
        {
            auto opt_rest = read_3_units(i, s, read);

            if (!opt_rest.has_value())
            {
                return std::nullopt;
            }

            auto [ w2, w3, w4 ] = opt_rest.value();

            if (!is_sequence_constituent(w2)
                || !is_sequence_constituent(w3)
                || !is_sequence_constituent(w4))
            {
                return std::nullopt;
            }
            
            char32_t ch = ((static_cast<char32_t>(w1) & U'\x07') << 18)
                | ((static_cast<char32_t>(w2) & U'\x3f') << 12)
                | ((static_cast<char32_t>(w3) & U'\x3f') << 6)
                | (static_cast<char32_t>(w4) & U'\x3f');

            if ((ch & ~U'\xffff') == 0 || !is_code_point(ch))
            {
                return std::nullopt;
            }

            return ch;
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
