/*
 * Copyright (C) 2015 Igalia S.L
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
#include "NetworkCacheData.h"

#if ENABLE(NETWORK_CACHE)

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace WebKit {
namespace NetworkCache {

Data::Data(const uint8_t* data, size_t size)
    : m_size(size)
{
    uint8_t* copiedData = static_cast<uint8_t*>(fastMalloc(size));
    memcpy(copiedData, data, size);
    m_buffer = adoptGRef(soup_buffer_new_with_owner(copiedData, size, copiedData, fastFree));
}

Data::Data(GRefPtr<SoupBuffer>&& buffer, Backing backing)
    : m_buffer(buffer)
    , m_size(buffer ? buffer->length : 0)
    , m_isMap(m_size && backing == Backing::Map)
{
}

Data Data::empty()
{
    GRefPtr<SoupBuffer> buffer = adoptGRef(soup_buffer_new(SOUP_MEMORY_TAKE, nullptr, 0));
    return { WTF::move(buffer) };
}

const uint8_t* Data::data() const
{
    return m_buffer ? reinterpret_cast<const uint8_t*>(m_buffer->data) : nullptr;
}

bool Data::isNull() const
{
    return !m_buffer;
}

bool Data::apply(const std::function<bool (const uint8_t*, size_t)>&& applier) const
{
    if (!m_size)
        return false;

    return applier(reinterpret_cast<const uint8_t*>(m_buffer->data), m_buffer->length);
}

Data Data::subrange(size_t offset, size_t size) const
{
    if (!m_buffer)
        return { };

    GRefPtr<SoupBuffer> subBuffer = adoptGRef(soup_buffer_new_subbuffer(m_buffer.get(), offset, size));
    return { WTF::move(subBuffer) };
}

Data concatenate(const Data& a, const Data& b)
{
    if (a.isNull())
        return b;
    if (b.isNull())
        return a;

    size_t size = a.size() + b.size();
    uint8_t* data = static_cast<uint8_t*>(fastMalloc(size));
    memcpy(data, a.soupBuffer()->data, a.soupBuffer()->length);
    memcpy(data + a.soupBuffer()->length, b.soupBuffer()->data, b.soupBuffer()->length);
    GRefPtr<SoupBuffer> buffer = adoptGRef(soup_buffer_new_with_owner(data, size, data, fastFree));
    return { WTF::move(buffer) };
}

struct MapWrapper {
    ~MapWrapper()
    {
        munmap(map, size);
    }

    void* map;
    size_t size;
};

static void deleteMapWrapper(MapWrapper* wrapper)
{
    delete wrapper;
}

Data mapFile(int fd, size_t offset, size_t size)
{
    if (!size)
        return Data::empty();

    void* map = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, offset);
    if (map == MAP_FAILED)
        return { };
    return Data::adoptMap(map, size);
}

Data Data::adoptMap(void* map, size_t size)
{
    ASSERT(map);
    ASSERT(map != MAP_FAILED);
    MapWrapper* wrapper = new MapWrapper { map, size };
    GRefPtr<SoupBuffer> buffer = adoptGRef(soup_buffer_new_with_owner(map, size, wrapper, reinterpret_cast<GDestroyNotify>(deleteMapWrapper)));
    return { WTF::move(buffer), Data::Backing::Map };
}

Data mapFile(const char* path)
{
    int fd = open(path, O_RDONLY, 0);
    if (fd < 0)
        return { };
    struct stat stat;
    if (fstat(fd, &stat) < 0) {
        close(fd);
        return { };
    }
    size_t size = stat.st_size;
    auto data = mapFile(fd, 0, size);
    close(fd);

    return data;
}

SHA1::Digest computeSHA1(const Data& data)
{
    SHA1 sha1;
    data.apply([&sha1](const uint8_t* data, size_t size) {
        sha1.addBytes(data, size);
        return true;
    });
    SHA1::Digest digest;
    sha1.computeHash(digest);
    return digest;
}

bool bytesEqual(const Data& a, const Data& b)
{
    if (a.isNull() || b.isNull())
        return false;
    if (a.size() != b.size())
        return false;
    return !memcmp(a.data(), b.data(), a.size());
}

} // namespace NetworkCache
} // namespace WebKit

#endif
