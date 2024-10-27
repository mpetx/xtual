
#include <xtual.hxx>

#include <iostream>

#undef NDEBUG
#include <cassert>

void test_encode_u32_normal()
{
    char32_t buf[4];
    char32_t *i = buf;

    assert(xtual::encode_as_u32(i, buf + 4, U'あ'));

    assert(buf[0] == U'あ');
    assert(i == buf + 1);
}

void test_encode_b32be_normal()
{
    std::byte buf[16];
    std::byte *i = buf;

    assert(xtual::encode_as_b32be<std::byte>(i, buf + 16, U'あ'));

    assert(buf[0] == static_cast<std::byte>(0x00));
    assert(buf[1] == static_cast<std::byte>(0x00));
    assert(buf[2] == static_cast<std::byte>(0x30));
    assert(buf[3] == static_cast<std::byte>(0x42));
    assert(i == buf + 4);
}

void test_encode_b32le_normal()
{
    char buf[16];
    char *i = buf;

    assert(xtual::encode_as_b32le<char>(i, buf + 16, U'ア'));

    assert(buf[0] == static_cast<char>(0xa2));
    assert(buf[1] == static_cast<char>(0x30));
    assert(buf[2] == static_cast<char>(0x00));
    assert(buf[3] == static_cast<char>(0x00));
    assert(i == buf + 4);
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
    
    std::cout << "OK" << std::endl;
}
