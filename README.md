# xtual

C++20のためのUnicodeデコーダ・エンコーダ

## 例

```c++
#include <xtual.hxx>

#include <cstddef>
#include <iostream>
#include <memory>

void encode_example()
{
    auto buf = std::make_unique<std::byte[]>(1024);
    auto i = buf.get();
    auto s = buf.get() + 1024;

    for (auto ch : U"𠮷野家で𩸽")
    {
        xtual::encode_as_b16be<std::byte>(i, s, ch);
    }

    std::cout << std::hex << std::setfill('0') << std::uppercase;

    for (auto j = buf.get(); j < i; ++j)
    {
        std::cout << std::setw(2) << static_cast<int>(*j) << ' ';
    }

    std::cout << std::dec << std::setfill(' ') << std::nouppercase << std::endl;
}

// Output:
//   D8 42 DF B7 91 CE 5B B6 30 67 D8 67 DE 3D 00 00
```

```c++
#include <xtual.hxx>

#include <cstring>
#include <iomanip>
#include <iostream>

void decode_example()
{
    const char *buf = "\xF0\xB0\xBB\x9E\xF0\xB0\xBB\x9E\xE9\xBA\xBA";
    auto i = buf;
    auto s = buf + std::strlen(buf);

    std::cout << std::hex << std::setfill('0') << std::uppercase << std::right;

    while (i != s)
    {
        auto opt = xtual::decode_from_b8<char>(i, s);

        std::cout << "U+" << std::setw(4) << static_cast<long>(opt.value()) << ' ';
    }

    std::cout << std::dec << std::setfill(' ') << std::nouppercase << std::endl;
}

// Output:
//   U+30EDE U+30EDE U+9EBA
```

## 概要

`xtual.hxx`は以下の関数テンプレートを定義します。

- `encode_as_u32`
- `encode_as_b32be`
- `encode_as_b32le`
- `encode_as_u16`
- `encode_as_b16be`
- `encode_as_b16le`
- `encode_as_u8`
- `encode_as_b8`
- `decode_from_u32`
- `decode_from_b32be`
- `decode_from_b32le`
- `decode_from_u16`
- `decode_from_b16be`
- `decode_from_b16le`
- `decode_from_u8`
- `decode_from_b8`

`u32`や`b16be`などは符号化方式を指定する接尾辞です。

| 接尾辞 | 符号化方式 |
|:-|:-|
| `u32` | `char32_t`の列 |
| `b32be` | UTF-32BEで符号化されたバイト列 |
| `b32le` | UTF-32LEで符号化されたバイト列 |
| `u16` | `char16_t`の列 |
| `b16be` | UTF-16BEで符号化されたバイト列 |
| `b16le` | UTF-16LEで符号化されたバイト列 |
| `u8` | `char8_t`の列 |
| `b8` | UTF-8で符号化されたバイト列 |

バイトとして扱うことができるのは以下のいずれかです。

- `std::byte` (`cstddef`)
- `char`
- `unsigned char`
- `signed char`
- `std::uint8_t` (`cstdint`)
- `std::int8_t` (`cstdint`)

### エンコード

`encode_as_X`は符号点を符号単位列あるいはバイト列にエンコードするための関数テンプレートです。イテレータと番兵、符号点を引数にとり、成否を表す真偽値を返します。

```c++
template <std::output_iterator<char32_t> Iter, std::sentinel_for<Iter> Sent>
bool xtual::encode_as_uX(Iter &i, Sent s, char32_t ch);

template <xtual::byte_like byteT, std::output_iterator<byteT> Iter, std::sentiniel_for<Iter> Sent>
bool xtual::encode_as_bX(Iter &i, Sent s, char32_t ch);
```

```c++
std::byte buf[64];
std::byte *i = buf;

xtual::encode_as_b16be<std::byte>(i, buf + 64, U'\x3106C');

auto expect = reinterpret_cast<const std::byte *>("\xD8\x84\xDC\x6C");
assert(std::equal(buf + 0, i, expect, expect + 4));
```

エンコードが失敗するのは次のような場合です。

- 与えられた符号点が有効な符号点でない場合 (サロゲートや`U+10FFFF`より大きい値など)
- エンコード結果をすべて出力する前にイテレータの終端に到達した場合

### デコード

`decode_from_X`は符号単位列あるいはバイト列をデコードして符号点を得るための関数テンプレートです。イテレータと番兵を引数にとり、成功時には`char32_t`を返します。

```c++
template <std::input_iterator Iter, std::sentinel_for<Iter> Sent>
std::optional<char32_t> xtual::decode_from_uX(Iter &i, Sent s);

template <xtual::byte_like byteT, std::input_iterator Iter, std::sentinel_for<Iter> Sent>
std::optional<char32_t> xtual::decode_from_bX(Iter &i, Sent s);
```

```c++
const char8_t *buf = u8"भारत";
const char8_t *i = buf;

assert(xtual::decode_from_u8(i, buf + 12) == U'भ');
assert(xtual::decode_from_u8(i, buf + 12) == U'ा');
assert(xtual::decode_from_u8(i, buf + 12) == U'र');
assert(xtual::decode_from_u8(i, buf + 12) == U'त');
assert(i == buf + 12);
```

デコードが失敗するのは次のような場合です。

- デコードした結果が不正な符号点である場合 (サロゲートや`U+10FFFF`より大きい値など)
- デコードの途中でイテレータの終端に到達した場合
- UTF-16において、不正なサロゲートが発見された場合
- UTF-8において、符号点が最小のバイト数で表現されていない場合
- UTF-8において、バイト列が不正な形式をとっている場合
