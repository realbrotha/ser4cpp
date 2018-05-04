/*
 * Copyright (c) 2018, Automatak LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 * following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 * disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "catch.hpp"

#include "ser4cpp/container/Buffer.h"
#include "ser4cpp/serialization/DoubleFloat.h"
#include "ser4cpp/serialization/FloatByteOrder.h"
#include "ser4cpp/serialization/SingleFloat.h"
#include "ser4cpp/util/HexConversions.h"

using namespace ser4cpp;
using namespace std;

template <class T>
bool TestFloatParsing(std::string expected_hex, typename T::type_t value)
{
    Buffer buffer(T::size);

    auto dest = buffer.as_wslice();
    if (!T::write_to(dest, value)) return false;

    if(dest.is_not_empty()) return false;

    const auto hex = HexConversions::to_hex(buffer.as_rslice());

    if (expected_hex != hex) return false;

    typename T::type_t read_value;

    auto input = buffer.as_rslice();

    return T::read_from(input, read_value) && input.is_empty() && (value == read_value);
}

#define SUITE(name) "FloatSerializationTestSuite - " name

TEST_CASE(SUITE("Float memory byte order is IEEE 754"))
{
    REQUIRE(ser4cpp::FloatByteOrder::order() != FloatByteOrder::Value::unsupported);
}

TEST_CASE(SUITE("DoubleFloatSerialization"))
{
    REQUIRE(TestFloatParsing<ser4cpp::DoubleFloat>("0A 52 84 2F C7 2B A2 C0", -2.3258890344E3));
    REQUIRE(TestFloatParsing<ser4cpp::DoubleFloat>("00 00 00 00 64 89 67 41", 12340000.0));
    REQUIRE(TestFloatParsing<ser4cpp::DoubleFloat>("00 00 00 00 00 00 34 C0", -20.0));
    REQUIRE(TestFloatParsing<ser4cpp::DoubleFloat>("8F 81 9C 95 2D F9 64 BB", -13.879E-23));
    REQUIRE(TestFloatParsing<ser4cpp::DoubleFloat>("00 00 00 00 00 00 59 40", 100.0));
}

TEST_CASE(SUITE("SingleFloatSerialization"))
{
    REQUIRE(TestFloatParsing<ser4cpp::SingleFloat>("20 4B 3C 4B", 12340000.0f));
    REQUIRE(TestFloatParsing<ser4cpp::SingleFloat>("6D C9 27 9B", -13.879E-23f));
    REQUIRE(TestFloatParsing<ser4cpp::SingleFloat>("00 00 A0 C1", -20.0));
}
