
#include <xtual.hxx>

#include <algorithm>
#include <iostream>

#undef NDEBUG
#include <cassert>

void test_encode_u8_normal()
{
    char8_t buf[16];
    char8_t *i = buf;

    assert(xtual::encode_as_u8(i, buf + 16, U'a'));

    const char8_t *expect1 = u8"a";
    assert(std::equal(buf + 0, i, expect1, expect1 + 1));

    i = buf;

    assert(xtual::encode_as_u8(i, buf + 16, U'я'));

    const char8_t *expect2 = u8"я";
    assert(std::equal(buf + 0, i, expect2, expect2 + 2));

    i = buf;

    assert(xtual::encode_as_u8(i, buf + 16, U'아'));

    const char8_t *expect3 = u8"아";
    assert(std::equal(buf + 0, i, expect3, expect3 + 3));

    i = buf;

    assert(xtual::encode_as_u8(i, buf + 16, U'𩸽'));

    const char8_t *expect4 = u8"𩸽";
    assert(std::equal(buf + 0, i, expect4, expect4 + 4));
}

void test_encode_b8_normal()
{
    char buf[16];
    char *i = buf;

    assert(xtual::encode_as_b8<char>(i, buf + 16, U'a'));

    const char *expect1 = "a";
    assert(std::equal(buf + 0, i, expect1, expect1 + 1));

    i = buf;

    assert(xtual::encode_as_b8<char>(i, buf + 16, U'я'));

    const char *expect2 = "я";
    assert(std::equal(buf + 0, i, expect2, expect2 + 2));

    i = buf;

    assert(xtual::encode_as_b8<char>(i, buf + 16, U'아'));

    const char *expect3 = "아";
    assert(std::equal(buf + 0, i, expect3, expect3 + 3));

    i = buf;

    assert(xtual::encode_as_b8<char>(i, buf + 16, U'𩸽'));

    const char *expect4 = "𩸽";
    assert(std::equal(buf + 0, i, expect4, expect4 + 4));
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
