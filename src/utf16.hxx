
namespace xtual
{

    template <typename charT, std::output_iterator<charT> Iter, std::sentinel_for<Iter> Sent, std::invocable<Iter &, Sent, char16_t> Writ>
    bool utf16_encode(Iter &i, Sent s, char32_t ch, Writ write)
    {
        if (!is_code_point(ch))
        {
            return false;
        }
        
        if ((ch & ~static_cast<char32_t>(0xff'ff)) == 0)
        {
            return write(i, s, static_cast<char16_t>(ch));
        }
        else
        {
            char32_t u = ch - 0x01'00'00;
            
            char16_t w1 = static_cast<char16_t>(0xd800);
            char16_t w2 = static_cast<char16_t>(0xdc00);

            w1 |= static_cast<char16_t>((u >> 10) & 0x03'ff);
            w2 |= static_cast<char16_t>(u & 0x03'ff);

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
    
}
