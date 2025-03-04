/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
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

#ifndef DFABytecodeInterpreter_h
#define DFABytecodeInterpreter_h

#if ENABLE(CONTENT_EXTENSIONS)

#include "ContentExtensionRule.h"
#include "ContentExtensionsDebugging.h"
#include "DFABytecode.h"
#include <wtf/DataLog.h>
#include <wtf/HashSet.h>
#include <wtf/Vector.h>

namespace WebCore {
    
namespace ContentExtensions {

class DFABytecodeInterpreter {
public:
    DFABytecodeInterpreter(const DFABytecode* bytecode, unsigned bytecodeLength, Vector<bool>& pagesUsed)
        : m_bytecode(bytecode)
        , m_bytecodeLength(bytecodeLength)
        , m_pagesUsed(pagesUsed)
    {
    }
    ~DFABytecodeInterpreter()
    {
#if CONTENT_EXTENSIONS_MEMORY_REPORTING
        size_t total = 0;
        for (bool& b : m_pagesUsed)
            total += b;
        dataLogF("Pages used: %zu / %zu\n", total, m_pagesUsed.size());
#endif
    }
    
    typedef HashSet<uint64_t, DefaultHash<uint64_t>::Hash, WTF::UnsignedWithZeroKeyHashTraits<uint64_t>> Actions;
    
    Actions interpret(const CString&, uint16_t flags);
    Actions actionsFromDFARoot();

private:
    void interpretAppendAction(unsigned& programCounter, Actions&);
    void interpretTestFlagsAndAppendAction(unsigned& programCounter, uint16_t flags, Actions&);
    const DFABytecode* m_bytecode;
    const unsigned m_bytecodeLength;
    Vector<bool>& m_pagesUsed;
};

} // namespace ContentExtensions
    
} // namespace WebCore

#endif // ENABLE(CONTENT_EXTENSIONS)

#endif // DFABytecodeInterpreter_h
