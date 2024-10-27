
#include <xtual.hxx>

#include <algorithm>
#include <iostream>

#undef NDEBUG
#include <cassert>

void test_encode_u32_normal()
{
    char32_t buf[4];
    char32_t *i = buf;

    assert(xtual::encode_as_u32(i, buf + 4, U'あ'));
    
    const char32_t *expect = U"あ";
    assert(std::equal(buf + 0, i, expect, expect + 1));
}

void test_encode_b32be_normal()
{
    std::byte buf[16];
    std::byte *i = buf;

    assert(xtual::encode_as_b32be<std::byte>(i, buf + 16, U'あ'));

    auto *expect = reinterpret_cast<const std::byte *>("\x00\x00\x30\x42");
    assert(std::equal(buf + 0, i, expect, expect + 4));
}

void test_encode_b32le_normal()
{
    char buf[16];
    char *i = buf;

    assert(xtual::encode_as_b32le<char>(i, buf + 16, U'ア'));

    const char *expect = "\xa2\x30\x00\x00";
    assert(std::equal(buf + 0, i, expect, expect + 4));
}

void test_encode_u32_invalid()
{
    char32_t buf[4];
    char32_t *i = buf;

    assert(!xtual::encode_as_u32(i, buf + 4, U'\xd800'));
    assert(!xtual::encode_as_u32(i, buf + 4, U'\x110000'));
}

void test_encode_b32be_invalid()
{
    std::byte buf[16];
    std::byte *i = buf;

    assert(!xtual::encode_as_b32be<std::byte>(i, buf + 16, U'\xd800'));
    assert(!xtual::encode_as_b32be<std::byte>(i, buf + 16, U'\x110000'));
}

void test_encode_b32le_invalid()
{
    std::byte buf[16];
    std::byte *i = buf;

    assert(!xtual::encode_as_b32le<std::byte>(i, buf + 16, U'\xd800'));
    assert(!xtual::encode_as_b32le<std::byte>(i, buf + 16, U'\x110000'));
}

void test_encode_u32_insufficient()
{
    char32_t buf[4];
    char32_t *i = buf;

    assert(!xtual::encode_as_u32(i, buf + 0, U'あ'));
}

void test_encode_b32be_insufficient()
{
    char buf[4];
    char *i = buf;

    assert(!xtual::encode_as_b32be<char>(i, buf + 3, U'あ'));
}

void test_encode_b32le_insufficient()
{
    char buf[4];
    char *i = buf;

    assert(!xtual::encode_as_b32le<char>(i, buf + 3, U'あ'));
}

void test_decode_u32_normal()
{
    const char32_t *buf = U"あ";
    const char32_t *i = buf;

    assert(xtual::decode_from_u32(i, buf + 1) == U'あ');
    assert(i == buf + 1);
}

void test_decode_b32be_normal()
{
    const char *buf = "\x00\x00\x30\x42";
    const char *i = buf;

    assert(xtual::decode_from_b32be<char>(i, buf + 4) == U'あ');
    assert(i == buf + 4);
}

void test_decode_b32le_normal()
{
    const char *buf = "\x42\x30\x00\x00";
    const char *i = buf;

    assert(xtual::decode_from_b32le<char>(i, buf + 4) == U'あ');
    assert(i == buf + 4);
}

void test_decode_u32_surrogate()
{
    const char32_t *buf = U"\xd800";
    const char32_t *i = buf;

    assert(!xtual::decode_from_u32(i, buf + 1).has_value());
}

void test_decode_b32be_surrogate()
{
    auto buf = reinterpret_cast<const std::byte *>("\x00\x00\xdf\x00");
    const std::byte *i = buf;

    assert(!xtual::decode_from_b32be<std::byte>(i, buf + 4).has_value());
}

void test_decode_b32le_surrogate()
{
    auto buf = reinterpret_cast<const std::byte *>("\x00\xdf\x00\x00");
    const std::byte *i = buf;

    assert(!xtual::decode_from_b32le<std::byte>(i, buf + 4).has_value());
}

void test_decode_u32_unexpected_end()
{
    const char32_t *buf = U"あ";
    const char32_t *i = buf;

    assert(!xtual::decode_from_u32(i, buf + 0).has_value());
}

void test_decode_b32be_unexpected_end()
{
    const char *buf = "\x00\x00\x30\x42";
    const char *i = buf;

    assert(!xtual::decode_from_b32be<char>(i, buf + 0).has_value());
    
    i = buf;
    assert(!xtual::decode_from_b32be<char>(i, buf + 1).has_value());
    
    i = buf;
    assert(!xtual::decode_from_b32be<char>(i, buf + 2).has_value());

    i = buf;
    assert(!xtual::decode_from_b32be<char>(i, buf + 3).has_value());
}

void test_decode_b32le_unexpected_end()
{
    const char *buf = "\x42\x30\x00\x00";
    const char *i = buf;

    assert(!xtual::decode_from_b32le<char>(i, buf + 0).has_value());

    i = buf;
    assert(!xtual::decode_from_b32le<char>(i, buf + 1).has_value());

    i = buf;
    assert(!xtual::decode_from_b32le<char>(i, buf + 2).has_value());

    i = buf;
    assert(!xtual::decode_from_b32le<char>(i, buf + 3).has_value());
}

int main()
{
    test_encode_u32_normal();
    test_encode_b32be_normal();
    test_encode_b32le_normal();

    test_encode_u32_invalid();
    test_encode_b32be_invalid();
    test_encode_b32le_invalid();

    test_encode_u32_insufficient();
    test_encode_b32be_insufficient();
    test_encode_b32le_insufficient();

    test_decode_u32_normal();
    test_decode_b32be_normal();
    test_decode_b32le_normal();

    test_decode_u32_surrogate();
    test_decode_b32be_surrogate();
    test_decode_b32le_surrogate();

    test_decode_u32_unexpected_end();
    test_decode_b32be_unexpected_end();
    test_decode_b32le_unexpected_end();
    
    std::cout << "OK" << std::endl;
}
