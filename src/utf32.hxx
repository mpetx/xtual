
namespace xtual
{

    template <typename charT, std::output_iterator<charT> Iter, std::sentinel_for<Iter> Sent, std::invocable<Iter &, Sent, char32_t> Writ>
    bool utf32_encode(Iter &i, Sent s, char32_t ch, Writ write)
    {
        if (!is_code_point(ch))
        {
            return false;
        }

        return write(i, s, ch);
    }

    template <std::output_iterator<char32_t> Iter, std::sentinel_for<Iter> Sent>
    bool encode_as_u32(Iter &i, Sent s, char32_t ch)
    {
        return utf32_encode<char32_t>(i, s, ch, [](Iter &i, Sent s, char32_t ch) {
            if (i == s)
            {
                return false;
            }

            *i++ = ch;

            return true;
        });
    }

    template <byte_like byteT, std::output_iterator<byteT> Iter, std::sentinel_for<Iter> Sent>
    bool encode_as_b32be(Iter &i, Sent s, char32_t ch)
    {
        return utf32_encode<byteT>(i, s, ch, [](Iter &i, Sent s, char32_t ch) {
            if (i == s)
            {
                return false;
            }

            *i++ = static_cast<byteT>(static_cast<std::byte>((ch >> 24) & 0xff));

            if (i == s)
            {
                return false;
            }

            *i++ = static_cast<byteT>(static_cast<std::byte>((ch >> 16) & 0xff));

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
    bool encode_as_b32le(Iter &i, Sent s, char32_t ch)
    {
        return utf32_encode<byteT>(i, s, ch, [](Iter &i, Sent s, char32_t ch) {
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

            if (i == s)
            {
                return false;
            }

            *i++ = static_cast<byteT>(static_cast<std::byte>((ch >> 16) & 0xff));

            if (i == s)
            {
                return false;
            }

            *i++ = static_cast<byteT>(static_cast<std::byte>((ch >> 24) & 0xff));

            return true;
        });
    }

    template <std::input_iterator Iter, std::sentinel_for<Iter> Sent, std::invocable<Iter &, Sent> Rdr>
    std::optional<char32_t> utf32_decode(Iter &i, Sent s, Rdr read)
    {
        auto opt = read(i, s);

        if (!opt.has_value())
        {
            return std::nullopt;
        }

        char32_t ch = opt.value();

        if (!is_code_point(ch))
        {
            return std::nullopt;
        }
        else
        {
            return ch;
        }
    }

    template <std::input_iterator Iter, std::sentinel_for<Iter> Sent>
    requires std::convertible_to<std::iter_value_t<Iter>, char32_t>
    std::optional<char32_t> decode_from_u32(Iter &i, Sent s)
    {
        return utf32_decode(i, s, [](Iter &i, Sent s) -> std::optional<char32_t> {
            if (i == s)
            {
                return std::nullopt;
            }

            return *i++;
        });
    }

    template <byte_like byteT, std::input_iterator Iter, std::sentinel_for<Iter> Sent>
    requires std::convertible_to<std::iter_value_t<Iter>, byteT>
    std::optional<char32_t> decode_from_b32be(Iter &i, Sent s)
    {
        return utf32_decode(i, s, [](Iter &i, Sent s) -> std::optional<char32_t> {
            if (i == s)
            {
                return std::nullopt;
            }

            byteT b1 = *i++;
            char32_t c1 = static_cast<char32_t>(static_cast<std::byte>(b1));

            if (i == s)
            {
                return std::nullopt;
            }

            byteT b2 = *i++;
            char32_t c2 = static_cast<char32_t>(static_cast<std::byte>(b2));

            if (i == s)
            {
                return std::nullopt;
            }

            byteT b3 = *i++;
            char32_t c3 = static_cast<char32_t>(static_cast<std::byte>(b3));

            if (i == s)
            {
                return std::nullopt;
            }

            byteT b4 = *i++;
            char32_t c4 = static_cast<char32_t>(static_cast<std::byte>(b4));

            return (c1 << 24) | (c2 << 16) | (c3 << 8) | c4;
        });
    }

    template <byte_like byteT, std::input_iterator Iter, std::sentinel_for<Iter> Sent>
    requires std::convertible_to<std::iter_value_t<Iter>, byteT>
    std::optional<char32_t> decode_from_b32le(Iter &i, Sent s)
    {
        return utf32_decode(i, s, [](Iter &i, Sent s) -> std::optional<char32_t> {
            if (i == s)
            {
                return std::nullopt;
            }

            byteT b1 = *i++;
            char32_t c1 = static_cast<char32_t>(static_cast<std::byte>(b1));

            if (i == s)
            {
                return std::nullopt;
            }

            byteT b2 = *i++;
            char32_t c2 = static_cast<char32_t>(static_cast<std::byte>(b2));

            if (i == s)
            {
                return std::nullopt;
            }

            byteT b3 = *i++;
            char32_t c3 = static_cast<char32_t>(static_cast<std::byte>(b3));

            if (i == s)
            {
                return std::nullopt;
            }

            byteT b4 = *i++;
            char32_t c4 = static_cast<char32_t>(static_cast<std::byte>(b4));

            return c1 | (c2 << 8) | (c3 << 16) | (c4 << 24);
        });
    }
    
}
