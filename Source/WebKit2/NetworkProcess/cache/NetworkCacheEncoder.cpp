/*
 * Copyright (C) 2010, 2014 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "NetworkCacheEncoder.h"

#if ENABLE(NETWORK_CACHE)

namespace WebKit {
namespace NetworkCache {

Encoder::Encoder()
    : m_checksum(0)
{
}

Encoder::~Encoder()
{
}

uint8_t* Encoder::grow(size_t size)
{
    size_t newPosition = m_buffer.size();
    m_buffer.grow(m_buffer.size() + size);
    return m_buffer.data() + newPosition;
}

void Encoder::updateChecksumForData(unsigned& checksum, const uint8_t* data, size_t size)
{
    // FIXME: hashMemory should not require alignment.
    size_t hashSize = size - size % 2;
    unsigned hash = StringHasher::hashMemory(data, hashSize) ^ Encoder::Salt<uint8_t*>::value;
    checksum = WTF::pairIntHash(checksum, hash);
}

void Encoder::encodeFixedLengthData(const uint8_t* data, size_t size)
{
    updateChecksumForData(m_checksum, data, size);

    uint8_t* buffer = grow(size);
    memcpy(buffer, data, size);
}

template<typename Type>
void Encoder::encodeNumber(Type value)
{
    Encoder::updateChecksumForNumber(m_checksum, value);

    uint8_t* buffer = grow(sizeof(Type));
    memcpy(buffer, &value, sizeof(Type));
}

void Encoder::encode(bool value)
{
    encodeNumber(value);
}

void Encoder::encode(uint8_t value)
{
    encodeNumber(value);
}

void Encoder::encode(uint16_t value)
{
    encodeNumber(value);
}

void Encoder::encode(uint32_t value)
{
    encodeNumber(value);
}

void Encoder::encode(uint64_t value)
{
    encodeNumber(value);
}

void Encoder::encode(int32_t value)
{
    encodeNumber(value);
}

void Encoder::encode(int64_t value)
{
    encodeNumber(value);
}

void Encoder::encode(float value)
{
    encodeNumber(value);
}

void Encoder::encode(double value)
{
    encodeNumber(value);
}

void Encoder::encodeChecksum()
{
    encodeNumber(m_checksum);
}

}
}

#endif
