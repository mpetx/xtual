
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
    
}
