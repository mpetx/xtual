
#include <xtual.hxx>

#include <iostream>

#undef NDEBUG
#include <cassert>

void test_is_surrogate()
{
    assert(!xtual::is_surrogate(U'अ'));
    assert(xtual::is_surrogate(U'\xd800'));
    assert(xtual::is_surrogate(U'\xd8ff'));
    assert(xtual::is_surrogate(U'\xdb00'));
    assert(xtual::is_surrogate(U'\xdbff'));
    assert(xtual::is_surrogate(U'\xdc00'));
    assert(xtual::is_surrogate(U'\xdcff'));
    assert(xtual::is_surrogate(U'\xdf00'));
    assert(xtual::is_surrogate(U'\xdfff'));
    assert(!xtual::is_surrogate(U'\x1d800'));
    assert(!xtual::is_surrogate(U'\x1dfff'));
}

void test_is_high_surrogate()
{
    assert(!xtual::is_high_surrogate(U'अ'));
    assert(xtual::is_high_surrogate(U'\xd800'));
    assert(xtual::is_high_surrogate(U'\xd8ff'));
    assert(xtual::is_high_surrogate(U'\xdb00'));
    assert(xtual::is_high_surrogate(U'\xdbff'));
    assert(!xtual::is_high_surrogate(U'\xdc00'));
    assert(!xtual::is_high_surrogate(U'\xdcff'));
    assert(!xtual::is_high_surrogate(U'\xdf00'));
    assert(!xtual::is_high_surrogate(U'\xdfff'));
    assert(!xtual::is_high_surrogate(U'\x1d800'));
    assert(!xtual::is_high_surrogate(U'\x1dfff'));
}

void test_is_low_surrogate()
{
    assert(!xtual::is_low_surrogate(U'अ'));
    assert(!xtual::is_low_surrogate(U'\xd800'));
    assert(!xtual::is_low_surrogate(U'\xd8ff'));
    assert(!xtual::is_low_surrogate(U'\xdb00'));
    assert(!xtual::is_low_surrogate(U'\xdbff'));
    assert(xtual::is_low_surrogate(U'\xdc00'));
    assert(xtual::is_low_surrogate(U'\xdcff'));
    assert(xtual::is_low_surrogate(U'\xdf00'));
    assert(xtual::is_low_surrogate(U'\xdfff'));
    assert(!xtual::is_low_surrogate(U'\x1d800'));
    assert(!xtual::is_low_surrogate(U'\x1dfff'));
}

void test_is_code_point()
{
    assert(xtual::is_code_point(U'अ'));
    assert(!xtual::is_code_point(U'\xd800'));
    assert(!xtual::is_code_point(U'\xd8ff'));
    assert(!xtual::is_code_point(U'\xdb00'));
    assert(!xtual::is_code_point(U'\xdbff'));
    assert(!xtual::is_code_point(U'\xdc00'));
    assert(!xtual::is_code_point(U'\xdcff'));
    assert(!xtual::is_code_point(U'\xdf00'));
    assert(!xtual::is_code_point(U'\xdfff'));
    assert(xtual::is_code_point(U'\x1d800'));
    assert(xtual::is_code_point(U'\x1dfff'));
    assert(xtual::is_code_point(U'\x10ffff'));
    assert(!xtual::is_code_point(U'\x110000'));
}

int main()
{
    test_is_surrogate();
    test_is_high_surrogate();
    test_is_low_surrogate();
    test_is_code_point();
    
    std::cout << "OK" << std::endl;
}
