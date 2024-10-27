
#include <xtual.hxx>

#include <iostream>

#undef NDEBUG
#include <cassert>

void test_encode_u16_normal()
{
    char16_t buf[4];
    char16_t *i = buf;

    assert(xtual::encode_as_u16(i, buf + 4, U'阿'));

    assert(buf[0] == u'阿');
    assert(i == buf + 1);

    i = buf;

    assert(xtual::encode_as_u16(i, buf + 4, U'𩸽'));

    assert(buf[0] == u'\xd867');
    assert(buf[1] == u'\xde3d');
    assert(i == buf + 2);
}

void test_encode_b16be_normal()
{
    std::byte buf[16];
    std::byte *i = buf;

    assert(xtual::encode_as_b16be<std::byte>(i, buf + 16, U'阿'));

    assert(buf[0] == static_cast<std::byte>(0x96));
    assert(buf[1] == static_cast<std::byte>(0x3f));
    assert(i == buf + 2);

    i = buf;

    assert(xtual::encode_as_b16be<std::byte>(i, buf + 16, U'𩸽'));

    assert(buf[0] == static_cast<std::byte>(0xd8));
    assert(buf[1] == static_cast<std::byte>(0x67));
    assert(buf[2] == static_cast<std::byte>(0xde));
    assert(buf[3] == static_cast<std::byte>(0x3d));
    assert(i == buf + 4);
}

void test_encode_b16le_normal()
{
    std::uint8_t buf[16];
    std::uint8_t *i = buf;

    assert(xtual::encode_as_b16le<std::uint8_t>(i, buf + 16, U'阿'));

    assert(buf[0] == static_cast<std::uint8_t>(0x3f));
    assert(buf[1] == static_cast<std::uint8_t>(0x96));
    assert(i == buf + 2);

    i = buf;

    assert(xtual::encode_as_b16le<std::uint8_t>(i, buf + 16, U'𩸽'));

    assert(buf[0] == static_cast<std::uint8_t>(0x67));
    assert(buf[1] == static_cast<std::uint8_t>(0xd8));
    assert(buf[2] == static_cast<std::uint8_t>(0x3d));
    assert(buf[3] == static_cast<std::uint8_t>(0xde));
    assert(i == buf + 4);
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
    char16_t buf[] = { u'阿', u'\xd867', u'\xde3d' };
    char16_t *i = buf;

    auto opt1 = xtual::decode_from_u16(i, buf + 3);

    assert(opt1.has_value() && opt1.value() == U'阿');
    assert(i == buf + 1);
    
    auto opt2 = xtual::decode_from_u16(i, buf + 3);

    assert(opt2.has_value() && opt2.value() == U'𩸽');
    assert(i == buf + 3);
}

void test_decode_b16be_normal()
{
    std::uint8_t buf[] = { 0x96, 0x3f, 0xd8, 0x67, 0xde, 0x3d };
    std::uint8_t *i = buf;

    auto opt1 = xtual::decode_from_b16be<char>(i, buf + 6);

    assert(opt1.has_value() && opt1.value() == U'阿');
    assert(i == buf + 2);
    
    auto opt2 = xtual::decode_from_b16be<char>(i, buf + 6);

    assert(opt2.has_value() && opt2.value() == U'𩸽');
    assert(i == buf + 6);
}

void test_decode_b16le_normal()
{
    std::uint8_t buf[] = {  0x3f, 0x96, 0x67, 0xd8, 0x3d, 0xde };
    std::uint8_t *i = buf;

    auto opt1 = xtual::decode_from_b16le<char>(i, buf + 6);

    assert(opt1.has_value() && opt1.value() == U'阿');
    assert(i == buf + 2);
    
    auto opt2 = xtual::decode_from_b16le<char>(i, buf + 6);

    assert(opt2.has_value() && opt2.value() == U'𩸽');
    assert(i == buf + 6);
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
    
    std::cout << "OK" << std::endl;
}
