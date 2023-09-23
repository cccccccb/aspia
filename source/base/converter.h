//
// Aspia Project
// Copyright (C) 2016-2023 Dmitry Chapyshev <dmitry@aspia.ru>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.
//

#ifndef BASE_CONVERTER_H
#define BASE_CONVERTER_H

#include "build/build_config.h"

#include "base/filesystem.hpp"
#include "base/optional.hpp"
#include "base/memory/byte_array.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_util.h"
#include "base/strings/unicode.h"

namespace base {

namespace internal {

template <typename T>
struct ConverterImpl
{
    // Nothing
};

template <>
struct ConverterImpl<std::string>
{
    static bool fromString(std::string str, std::string* value)
    {
        value->assign(str);
        return true;
    }

    static std::string toString(const std::string& value)
    {
        return value;
    }
};

template <>
struct ConverterImpl<std::u16string>
{
    static bool fromString(std::string str, std::u16string* value)
    {
        return utf8ToUtf16(str, value);
    }

    static std::string toString(const std::u16string& value)
    {
        return utf8FromUtf16(value);
    }
};

#if defined(OS_WIN)
template <>
struct ConverterImpl<std::wstring>
{
    static bool fromString(std::string str, std::wstring* value)
    {
        return utf8ToWide(str, value);
    }

    static std::string toString(const std::wstring& value)
    {
        return utf8FromWide(value);
    }
};
#endif // defined(OS_WIN)

template <>
struct ConverterImpl<ByteArray>
{
    static bool fromString(std::string str, ByteArray* value)
    {
        *value = fromHex(str);
        return true;
    }

    static std::string toString(const ByteArray& value)
    {
        return toHex(value);
    }
};

template <>
struct ConverterImpl<ghc::filesystem::path>
{
    static bool fromString(std::string str, ghc::filesystem::path* value)
    {
        *value = ghc::filesystem::u8path(str);
        return true;
    }

    static std::string toString(const ghc::filesystem::path& value)
    {
        return value.u8string();
    }
};

template <>
struct ConverterImpl<int64_t>
{
    static bool fromString(std::string str, int64_t* value)
    {
        return stringToInt64(str, value);
    }

    static std::string toString(int64_t value)
    {
        return numberToString(value);
    }
};

template <>
struct ConverterImpl<int32_t>
{
    static bool fromString(std::string str, int32_t* value)
    {
        return stringToInt(str, value);
    }

    static std::string toString(int32_t value)
    {
        return numberToString(value);
    }
};

template <>
struct ConverterImpl<int16_t>
{
    static bool fromString(std::string str, int16_t* value)
    {
        return stringToShort(str, value);
    }

    static std::string toString(int16_t value)
    {
        return numberToString(value);
    }
};

template <>
struct ConverterImpl<int8_t>
{
    static bool fromString(std::string str, int8_t* value)
    {
        return stringToChar(str, value);
    }

    static std::string toString(int8_t value)
    {
        return numberToString(value);
    }
};

template <>
struct ConverterImpl<unsigned long long>
{
    static bool fromString(std::string str, unsigned long long* value)
    {
        return stringToULong64(str, value);
    }

    static std::string toString(unsigned long long value)
    {
        return numberToString(value);
    }
};

template <>
struct ConverterImpl<unsigned int>
{
    static bool fromString(std::string str, unsigned int* value)
    {
        return stringToUint(str, value);
    }

    static std::string toString(unsigned int value)
    {
        return numberToString(value);
    }
};

template <>
struct ConverterImpl<unsigned long>
{
    static bool fromString(std::string str, unsigned long* value)
    {
        return stringToULong(str, value);
    }

    static std::string toString(unsigned long value)
    {
        return numberToString(value);
    }
};

template <>
struct ConverterImpl<uint16_t>
{
    static bool fromString(std::string str, uint16_t* value)
    {
        return stringToUShort(str, value);
    }

    static std::string toString(uint16_t value)
    {
        return numberToString(value);
    }
};

template <>
struct ConverterImpl<uint8_t>
{
    static bool fromString(std::string str, uint8_t* value)
    {
        return stringToUChar(str, value);
    }

    static std::string toString(uint8_t value)
    {
        return numberToString(value);
    }
};

template <>
struct ConverterImpl<bool>
{
    static bool fromString(std::string str, bool* value)
    {
        if (str == "true" || str == "1")
            *value = true;
        else if (str == "false" || str == "0")
            *value = false;
        else
            return false;

        return true;
    }

    static std::string toString(bool value)
    {
        return value ? "true" : "false";
    }
};

} // namespace internal

template <typename ValueType>
class Converter
{
public:
    static tl::optional<ValueType> get_value(std::string str)
    {
        std::string temp = trimWhitespaceASCII(str, TRIM_ALL);
        if (temp.empty())
            return tl::nullopt;

        ValueType result;
        if (!internal::ConverterImpl<ValueType>::fromString(temp, &result))
            return tl::nullopt;

        return result;
    }

    static std::string set_value(const ValueType& value)
    {
        return internal::ConverterImpl<ValueType>::toString(value);
    }
};

} // namespace base

#endif // BASE_CONVERTER_H
