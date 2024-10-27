
#include <xtual.hxx>

#include <algorithm>
#include <iostream>

#undef NDEBUG
#include <cassert>

void test_encode_u16_normal()
{
    char16_t buf[4];
    char16_t *i = buf;

    assert(xtual::encode_as_u16(i, buf + 4, U'阿'));

    const char16_t *expect1 = u"阿";
    assert(std::equal(buf + 0, i, expect1, expect1 + 1));

    i = buf;

    assert(xtual::encode_as_u16(i, buf + 4, U'𩸽'));

    const char16_t *expect2 = u"\xd867\xde3d";
    assert(std::equal(buf + 0, i, expect2, expect2 + 2));
}

void test_encode_b16be_normal()
{
    std::byte buf[16];
    std::byte *i = buf;

    assert(xtual::encode_as_b16be<std::byte>(i, buf + 16, U'阿'));

    auto expect1 = reinterpret_cast<const std::byte *>("\x96\x3f");
    assert(std::equal(buf + 0, i, expect1, expect1 + 2));

    i = buf;

    assert(xtual::encode_as_b16be<std::byte>(i, buf + 16, U'𩸽'));

    auto expect2 = reinterpret_cast<const std::byte *>("\xd8\x67\xde\x3d");
    assert(std::equal(buf + 0, i, expect2, expect2 + 4));
}

void test_encode_b16le_normal()
{
    unsigned char buf[16];
    unsigned char *i = buf;

    assert(xtual::encode_as_b16le<unsigned char>(i, buf + 16, U'阿'));

    auto expect1 = reinterpret_cast<const unsigned char *>("\x3f\x96");
    assert(std::equal(buf + 0, i, expect1, expect1 + 2));

    i = buf;

    assert(xtual::encode_as_b16le<std::uint8_t>(i, buf + 16, U'𩸽'));

    auto expect2 = reinterpret_cast<const unsigned char *>("\x67\xd8\x3d\xde");
    assert(std::equal(buf + 0, i, expect2, expect2 + 4));
}

void test_encode_u16_invalid()
{
    char16_t buf[4];
    char16_t *i = buf;

    assert(!xtual::encode_as_u16(i, buf + 16, U'\xd800'));
    assert(!xtual::encode_as_u16(i, buf + 16, U'\x110000'));
}

void test_encode_b16be_invalid()
{
    std::byte buf[16];
    std::byte *i = buf;

    assert(!xtual::encode_as_b16be<std::byte>(i, buf + 16, U'\xdc00'));
    assert(!xtual::encode_as_b16be<std::byte>(i, buf + 16, U'\x110000'));
}

void test_encode_b16le_invalid()
{
    std::byte buf[16];
    std::byte *i = buf;

    assert(!xtual::encode_as_b16le<std::byte>(i, buf + 16, U'\xdfff'));
    assert(!xtual::encode_as_b16le<std::byte>(i, buf + 16, U'\x110000'));
}

void test_encode_u16_insufficient()
{
    char16_t buf[4];
    char16_t *i = buf;

    assert(!xtual::encode_as_u16(i, buf + 0, U'阿'));
    assert(!xtual::encode_as_u16(i, buf + 1, U'𩸽'));
}

void test_encode_b16be_insufficient()
{
    char buf[16];
    char *i = buf;

    assert(!xtual::encode_as_b16be<char>(i, buf + 1, U'阿'));
    assert(!xtual::encode_as_b16be<char>(i, buf + 3, U'𩸽'));
}

void test_encode_b16le_insufficient()
{
    char buf[16];
    char *i = buf;

    assert(!xtual::encode_as_b16le<char>(i, buf + 1, U'阿'));
    assert(!xtual::encode_as_b16le<char>(i, buf + 3, U'𩸽'));
}

void test_decode_u16_normal()
{
    const char16_t *buf = u"阿\xd867\xde3d";
    const char16_t *i = buf;

    assert(xtual::decode_from_u16(i, buf + 3) == U'阿');
    assert(i == buf + 1);

    assert(xtual::decode_from_u16(i, buf + 3) == U'𩸽');
    assert(i == buf + 3);
}

void test_decode_b16be_normal()
{
    auto buf = reinterpret_cast<const signed char *>("\x96\x3f\xd8\x67\xde\x3d");
    const signed char *i = buf;

    assert(xtual::decode_from_b16be<signed char>(i, buf + 6) == U'阿');
    assert(i == buf + 2);

    assert(xtual::decode_from_b16be<signed char>(i, buf + 6) == U'𩸽');
    assert(i == buf + 6);
}

void test_decode_b16le_normal()
{
    auto buf = reinterpret_cast<const std::byte *>("\x3f\x96\x67\xd8\x3d\xde");
    const std::byte *i = buf;

    assert(xtual::decode_from_b16le<std::byte>(i, buf + 6) == U'阿');
    assert(i == buf + 2);

    assert(xtual::decode_from_b16le<std::byte>(i, buf + 6) == U'𩸽');
    assert(i == buf + 6);
}

void test_decode_u16_lone_surrogate()
{
    const char16_t *buf = u"\xd800あ";
    assert(!xtual::decode_from_u16(buf, buf + 2).has_value());

    buf = u"\xdf00あ";
    assert(!xtual::decode_from_u16(buf, buf + 2).has_value());
}

void test_decode_b16be_lone_surrogate()
{
    const char *buf = "\xd8\x00\x30\x42";
    assert(!xtual::decode_from_b16be<char>(buf, buf + 4).has_value());

    buf = "\xdf\x00\x30\x42";
    assert(!xtual::decode_from_b16be<char>(buf, buf + 4).has_value());
}

void test_decode_b16le_lone_surrogate()
{
    const char *buf = "\x00\xd8\x42\x30";
    assert(!xtual::decode_from_b16le<char>(buf, buf + 4).has_value());

    buf = "\x00\xdf\x42\x30";
    assert(!xtual::decode_from_b16le<char>(buf, buf + 4).has_value());
}

void test_decode_u16_unexpected_end()
{
    const char16_t *buf = u"あ";
    assert(!xtual::decode_from_u16(buf, buf).has_value());

    buf = u"\xd800";
    assert(!xtual::decode_from_u16(buf, buf + 1).has_value());
}

void test_decode_b16be_unexpected_end()
{
    const char *buf = "\x30\x42";
    assert(!xtual::decode_from_b16be<char>(buf, buf + 1).has_value());

    buf = "\xd8\x00";
    assert(!xtual::decode_from_b16be<char>(buf, buf + 2).has_value());

    buf = "\xd8\x00\xdc";
    assert(!xtual::decode_from_b16be<char>(buf, buf + 3).has_value());
}

void test_decode_b16le_unexpected_end()
{
    const char *buf = "\x42\x30";
    assert(!xtual::decode_from_b16le<char>(buf, buf + 1).has_value());

    buf = "\x00\xd8";
    assert(!xtual::decode_from_b16le<char>(buf, buf + 2).has_value());

    buf = "\x00\xd8\x00\xdc";
    assert(!xtual::decode_from_b16le<char>(buf, buf + 3).has_value());
}

int main()
{
    test_encode_u16_normal();
    test_encode_b16be_normal();
    test_encode_b16le_normal();

    test_encode_u16_invalid();
    test_encode_b16be_invalid();
    test_encode_b16le_invalid();

    test_encode_u16_insufficient();
    test_encode_b16be_insufficient();
    test_encode_b16le_insufficient();

    test_decode_u16_normal();
    test_decode_b16be_normal();
    test_decode_b16le_normal();

    test_decode_u16_lone_surrogate();
    test_decode_b16be_lone_surrogate();
    test_decode_b16le_lone_surrogate();

    test_decode_u16_unexpected_end();
    test_decode_b16be_unexpected_end();
    test_decode_b16le_unexpected_end();
    
    std::cout << "OK" << std::endl;
}
