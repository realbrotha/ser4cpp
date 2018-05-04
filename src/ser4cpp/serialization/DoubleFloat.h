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
#ifndef SER4CPP_DOUBLE_FLOAT_H
#define SER4CPP_DOUBLE_FLOAT_H

#include "ser4cpp/container/SequenceTypes.h"
#include "ser4cpp/serialization/DoubleFloat.h"
#include "ser4cpp/serialization/FloatByteOrder.h"
#include "ser4cpp/util/Uncopyable.h"

#include <cstddef>
#include <cstring>
#include <limits>

namespace ser4cpp
{

class DoubleFloat : private StaticOnly
{
public:
    static_assert(sizeof(double) == 8, "Unexpected length of double float");

    static bool read_from(rseq_t& input, double& out)
    {
        if (input.length() < size) return false;

        out = read(input);
        input.advance(size);
        return true;
    }
    
    static bool write_to(wseq_t& dest, double value)
    {
        if (dest.length() < size) return false;

        write(dest, value);
        dest.advance(size);
        return true;
    }

    typedef double type_t;
    static constexpr std::size_t size = sizeof(double);
    static constexpr double max_value = std::numeric_limits<double>::max();
    static constexpr double min_value = -std::numeric_limits<double>::max();

private:
    union DoubleFloatUnion
    {
        uint8_t bytes[8];
        double value;
    };

    static double read(const uint8_t* data)
    {
        if (FloatByteOrder::order() == FloatByteOrder::Value::normal)
        {
            DoubleFloatUnion x = {{ data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7] }};
            return x.value;
        }
        else
        {
            DoubleFloatUnion x = {{ data[7], data[6], data[5], data[4], data[3], data[2], data[1], data[0] }};
            return x.value;
        }
    }

    static void write(uint8_t* dest, double value)
    {
        if (FloatByteOrder::order() == FloatByteOrder::Value::normal)
        {
            memcpy(dest, &value, size);
        }
        else
        {
            auto data = reinterpret_cast<uint8_t*>(&value);
            uint8_t bytes[8] = { data[7], data[6], data[5], data[4], data[3], data[2], data[1], data[0] };
            memcpy(dest, bytes, size);
        }
    }
};

}

#endif
