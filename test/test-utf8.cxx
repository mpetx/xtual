
#include <xtual.hxx>

#include <iostream>

#undef NDEBUG
#include <cassert>

void test_encode_u8_normal()
{
    char8_t buf[16];
    char8_t *i = buf;

    assert(xtual::encode_as_u8(i, buf + 16, U'a'));

    assert(buf[0] == static_cast<char8_t>(0x61));
    assert(i == buf + 1);

    i = buf;

    assert(xtual::encode_as_u8(i, buf + 16, U'я'));

    assert(buf[0] == static_cast<char8_t>(0xd1));
    assert(buf[1] == static_cast<char8_t>(0x8f));
    assert(i == buf + 2);

    i = buf;

    assert(xtual::encode_as_u8(i, buf + 16, U'아'));

    assert(buf[0] == static_cast<char8_t>(0xec));
    assert(buf[1] == static_cast<char8_t>(0x95));
    assert(buf[2] == static_cast<char8_t>(0x84));
    assert(i == buf + 3);

    i = buf;

    assert(xtual::encode_as_u8(i, buf + 16, U'𩸽'));

    assert(buf[0] == static_cast<char8_t>(0xf0));
    assert(buf[1] == static_cast<char8_t>(0xa9));
    assert(buf[2] == static_cast<char8_t>(0xb8));
    assert(buf[3] == static_cast<char8_t>(0xbd));
    assert(i == buf + 4);
}

void test_encode_b8_normal()
{
    std::byte buf[16];
    std::byte *i = buf;

    assert(xtual::encode_as_b8<std::byte>(i, buf + 16, U'a'));

    assert(buf[0] == static_cast<std::byte>(0x61));
    assert(i == buf + 1);

    i = buf;

    assert(xtual::encode_as_b8<std::byte>(i, buf + 16, U'я'));

    assert(buf[0] == static_cast<std::byte>(0xd1));
    assert(buf[1] == static_cast<std::byte>(0x8f));
    assert(i == buf + 2);

    i = buf;

    assert(xtual::encode_as_b8<std::byte>(i, buf + 16, U'아'));

    assert(buf[0] == static_cast<std::byte>(0xec));
    assert(buf[1] == static_cast<std::byte>(0x95));
    assert(buf[2] == static_cast<std::byte>(0x84));
    assert(i == buf + 3);

    i = buf;

    assert(xtual::encode_as_b8<std::byte>(i, buf + 16, U'𩸽'));

    assert(buf[0] == static_cast<std::byte>(0xf0));
    assert(buf[1] == static_cast<std::byte>(0xa9));
    assert(buf[2] == static_cast<std::byte>(0xb8));
    assert(buf[3] == static_cast<std::byte>(0xbd));
    assert(i == buf + 4);
}

void test_encode_u8_invalid()
{
    char8_t buf[16];
    char8_t *i = buf;

    assert(!xtual::encode_as_u8(i, buf + 16, U'\xd800'));
    assert(!xtual::encode_as_u8(i, buf + 16, U'\x110000'));
}

void test_encode_b8_invalid()
{
    char buf[16];
    char *i = buf;

    assert(!xtual::encode_as_b8<char>(i, buf + 16, U'\xd800'));
    assert(!xtual::encode_as_b8<char>(i, buf + 16, U'\x110000'));
}

void test_encode_u8_insufficient()
{
    char8_t buf[16];
    char8_t *i = buf;

    assert(!xtual::encode_as_u8(i, buf + 0, U'a'));
    assert(!xtual::encode_as_u8(i, buf + 1, U'á'));
    assert(!xtual::encode_as_u8(i, buf + 2, U'あ'));
    assert(!xtual::encode_as_u8(i, buf + 3, U'𩸽'));
}

void test_encode_b8_insufficient()
{
    signed char buf[16];
    signed char *i = buf;

    assert(!xtual::encode_as_b8<signed char>(i, buf + 0, U'a'));
    assert(!xtual::encode_as_b8<signed char>(i, buf + 1, U'á'));
    assert(!xtual::encode_as_b8<signed char>(i, buf + 2, U'あ'));
    assert(!xtual::encode_as_b8<signed char>(i, buf + 3, U'𩸽'));
}

int main()
{
    test_encode_u8_normal();
    test_encode_b8_normal();    

    test_encode_u8_invalid();
    test_encode_u8_invalid();

    test_encode_u8_insufficient();
    test_encode_b8_insufficient();
    
    std::cout << "OK" << std::endl;
}
