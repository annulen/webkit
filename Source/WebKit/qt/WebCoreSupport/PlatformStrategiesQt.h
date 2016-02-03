/*
 * Copyright (C) 2010, 2012 Apple Inc. All rights reserved.
 * Copyright (C) 2010 INdT - Instituto Nokia de Tecnologia
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
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

#ifndef PlatformStrategiesQt_h
#define PlatformStrategiesQt_h

#include <CookiesStrategy.h>
#include <LoaderStrategy.h>
#include <PlatformStrategies.h>
#include <PluginStrategy.h>

namespace WebCore {
class PasteboardStrategy;
}

class PlatformStrategiesQt : public WebCore::PlatformStrategies, private WebCore::CookiesStrategy, private WebCore::LoaderStrategy {
public:
    static void initialize();

private:
    PlatformStrategiesQt();

    // WebCore::PlatformStrategies
    virtual WebCore::CookiesStrategy* createCookiesStrategy();
    virtual WebCore::LoaderStrategy* createLoaderStrategy();
    virtual WebCore::PasteboardStrategy* createPasteboardStrategy();
    virtual WebCore::PluginStrategy* createPluginStrategy();
    
    // WebCore::CookiesStrategy
    virtual String cookiesForDOM(const WebCore::NetworkStorageSession&, const WebCore::URL& firstParty, const WebCore::URL&);
    virtual void setCookiesFromDOM(const WebCore::NetworkStorageSession&, const WebCore::URL& firstParty, const WebCore::URL&, const String&);
    virtual bool cookiesEnabled(const WebCore::NetworkStorageSession&, const WebCore::URL& firstParty, const WebCore::URL&);
    virtual String cookieRequestHeaderFieldValue(const WebCore::NetworkStorageSession&, const WebCore::URL& firstParty, const WebCore::URL&);
    virtual bool getRawCookies(const WebCore::NetworkStorageSession&, const WebCore::URL& firstParty, const WebCore::URL&, Vector<WebCore::Cookie>&);
    virtual void deleteCookie(const WebCore::NetworkStorageSession&, const WebCore::URL&, const String&);

#if 0
    // WebCore::PluginStrategy
    virtual void refreshPlugins();
    virtual void getPluginInfo(const WebCore::Page*, Vector<WebCore::PluginInfo>&);
#endif

// FIXME: Check if we still need to hook into visited links logic
// See https://bugs.webkit.org/show_bug.cgi?id=20952
#if 0
    // WebCore::VisitedLinkStrategy
    virtual bool isLinkVisited(WebCore::Page*, WebCore::LinkHash, const WebCore::URL& baseURL, const WTF::AtomicString& attributeURL);
    virtual void addVisitedLink(WebCore::Page*, WebCore::LinkHash);
#endif

    // LoaderStrategy interface
public:
    RefPtr<WebCore::SubresourceLoader> loadResource(WebCore::Frame *, WebCore::CachedResource *, const WebCore::ResourceRequest &, const WebCore::ResourceLoaderOptions &) override;
    void loadResourceSynchronously(WebCore::NetworkingContext *, unsigned long identifier, const WebCore::ResourceRequest &, WebCore::StoredCredentials, WebCore::ClientCredentialPolicy, WebCore::ResourceError &, WebCore::ResourceResponse &, Vector<char> &data) override;
    void remove(WebCore::ResourceLoader *) override;
    void setDefersLoading(WebCore::ResourceLoader *, bool) override;
    void crossOriginRedirectReceived(WebCore::ResourceLoader *, const WebCore::URL &redirectURL) override;
    void servePendingRequests(WebCore::ResourceLoadPriority minimumPriority) override;
    void suspendPendingRequests() override;
    void resumePendingRequests() override;
    void createPingHandle(WebCore::NetworkingContext *, WebCore::ResourceRequest &, bool shouldUseCredentialStorage) override;

    // PlatformStrategies interface
private:
    WebCore::BlobRegistry *createBlobRegistry() override;
};

#endif // PlatformStrategiesQt_h
