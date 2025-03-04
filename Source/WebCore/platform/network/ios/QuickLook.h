/*
 * Copyright (C) 2009 Apple Inc. All rights reserved.
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

#ifndef QuickLook_h
#define QuickLook_h

#if USE(QUICK_LOOK)

#import "QuickLookHandleClient.h"
#import "ResourceRequest.h"
#import <objc/runtime.h>
#import <wtf/RefPtr.h>

OBJC_CLASS NSData;
OBJC_CLASS NSDictionary;
OBJC_CLASS NSFileHandle;
OBJC_CLASS NSSet;
OBJC_CLASS NSString;
OBJC_CLASS NSURL;
OBJC_CLASS NSURLConnection;
OBJC_CLASS NSURLRequest;
OBJC_CLASS NSURLResponse;
OBJC_CLASS QLPreviewConverter;

#if USE(CFNETWORK)
typedef struct _CFURLResponse* CFURLResponseRef;
typedef struct _CFURLConnection* CFURLConnectionRef;
#endif

namespace WebCore {

class QuickLookHandleClient;
class ResourceHandle;
class ResourceLoader;
class ResourceResponse;
class SynchronousResourceHandleCFURLConnectionDelegate;

WEBCORE_EXPORT NSSet *QLPreviewGetSupportedMIMETypesSet();

// Used for setting the permissions on the saved QL content
NSDictionary *QLFileAttributes();
NSDictionary *QLDirectoryAttributes();

WEBCORE_EXPORT void addQLPreviewConverterWithFileForURL(NSURL *, id converter, NSString *fileName);
WEBCORE_EXPORT NSString *qlPreviewConverterFileNameForURL(NSURL *);
WEBCORE_EXPORT NSString *qlPreviewConverterUTIForURL(NSURL *);
WEBCORE_EXPORT void removeQLPreviewConverterForURL(NSURL *);

WEBCORE_EXPORT RetainPtr<NSURLRequest> registerQLPreviewConverterIfNeeded(NSURL *, NSString *mimeType, NSData *);

const URL safeQLURLForDocumentURLAndResourceURL(const URL& documentURL, const String& resourceURL);

const char* QLPreviewProtocol();

WEBCORE_EXPORT NSString *createTemporaryFileForQuickLook(NSString *fileName);

class QuickLookHandle {
    WTF_MAKE_NONCOPYABLE(QuickLookHandle);
public:
    WEBCORE_EXPORT static bool shouldCreateForMIMEType(const String&);

    static std::unique_ptr<QuickLookHandle> create(ResourceHandle*, NSURLConnection *, NSURLResponse *, id delegate);
#if USE(CFNETWORK)
    static std::unique_ptr<QuickLookHandle> create(ResourceHandle*, SynchronousResourceHandleCFURLConnectionDelegate*, CFURLResponseRef);
#endif
    // FIXME: Use of ResourceLoader here is a platform violation.
    WEBCORE_EXPORT static std::unique_ptr<QuickLookHandle> create(ResourceLoader&, const ResourceResponse&);

    WEBCORE_EXPORT ~QuickLookHandle();

    WEBCORE_EXPORT bool didReceiveDataArray(CFArrayRef);
    WEBCORE_EXPORT bool didReceiveData(CFDataRef);
    WEBCORE_EXPORT bool didFinishLoading();
    WEBCORE_EXPORT void didFail();

    WEBCORE_EXPORT NSURLResponse *nsResponse();
#if USE(CFNETWORK)
    CFURLResponseRef cfResponse();
#endif

    void setClient(PassRefPtr<QuickLookHandleClient> client) { m_client = client; }

    WEBCORE_EXPORT String previewFileName() const;
    WEBCORE_EXPORT String previewUTI() const;
    NSURL *firstRequestURL() const { return m_firstRequestURL.get(); }
    WEBCORE_EXPORT NSURL *previewRequestURL() const;
    QLPreviewConverter *converter() const { return m_converter.get(); }

private:
    QuickLookHandle(NSURL *, NSURLConnection *, NSURLResponse *, id delegate);

    RetainPtr<NSURL> m_firstRequestURL;
    RetainPtr<QLPreviewConverter> m_converter;
    RetainPtr<id> m_delegate;
    bool m_finishedLoadingDataIntoConverter;
    RetainPtr<NSFileHandle *> m_quicklookFileHandle;
    RetainPtr<NSURLResponse> m_nsResponse;
    RefPtr<QuickLookHandleClient> m_client;
};

} // namespace WebCore

#endif // USE(QUICK_LOOK)

#endif // QuickLook_h
