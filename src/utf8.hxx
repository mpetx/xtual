
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
    
}
