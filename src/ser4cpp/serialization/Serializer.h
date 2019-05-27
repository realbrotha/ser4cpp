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
#ifndef SER4CPP_SERIALIZER_H
#define SER4CPP_SERIALIZER_H

#include "ser4cpp/container/SequenceTypes.h"

#include <cstdint>

namespace ser4cpp
{

template<class T> class Serializer
{
public:
    typedef bool (*ReadFunc)(rseq_t& buffer, T& output);
    typedef bool (*WriteFunc)(const T& value, wseq_t& buffer);

    Serializer() : size(0), pReadFunc(nullptr), pWriteFunc(nullptr) {}

    Serializer(uint32_t size, ReadFunc pReadFunc, WriteFunc pWriteFunc)
        : _size(size), pReadFunc(pReadFunc), pWriteFunc(pWriteFunc)
    {
    }

    /**
     * @return The size (in bytes) required for every call to read/write
     */
    uint32_t size() const
    {
        return _size;
    }

    /**
     * reads the value and advances the read buffer
     */
    bool read(rseq_t& buffer, T& output) const
    {
        return (*pReadFunc)(buffer, output);
    }

    /**
     * writes the value and advances the write buffer
     */
    bool write(const T& value, wseq_t& buffer) const
    {
        return (*pWriteFunc)(value, buffer);
    }

private:
    uint32_t _size;
    ReadFunc pReadFunc;
    WriteFunc pWriteFunc;
};

}

#endif
