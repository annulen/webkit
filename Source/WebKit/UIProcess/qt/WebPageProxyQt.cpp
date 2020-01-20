/*
 * Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2010 Apple Inc. All rights reserved.
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
#include "WebPageProxy.h"

#include "PageClient.h"
#include "UserAgentQt.h"
#include "WebPageMessages.h"
#include "WebProcessProxy.h"
#include "WebsiteDataStore.h"
#include <WebCore/Editor.h>
#include <WebCore/NotImplemented.h>

#if HAVE(QTQUICK)
#include "QtNetworkReplyData.h"
#include "QtPageClient.h"
#include "qquicknetworkreply_p.h"
#endif

using namespace WebCore;

namespace WebKit {

void WebPageProxy::platformInitialize()
{
}

String WebPageProxy::standardUserAgent(const String& applicationNameForUserAgent)
{
    return UserAgentQt::standardUserAgent(applicationNameForUserAgent,2,11);
}

void WebPageProxy::saveRecentSearches(const String&, const Vector<WebCore::RecentSearch>&)
{
    notImplemented();
}

void WebPageProxy::loadRecentSearches(const String&, CompletionHandler<void(Vector<WebCore::RecentSearch>&&)>&&)
{
    notImplemented();
}

void WebPageProxy::authenticationRequiredRequest(const String& hostname, const String& realm, const String& prefilledUsername, String& username, String& password)
{
    //WIP
}

void WebPageProxy::proxyAuthenticationRequiredRequest(const String& hostname, uint16_t port, const String& prefilledUsername, String& username, String& password)
{
    //WIP
}

void WebPageProxy::certificateVerificationRequest(const String& hostname, bool& ignoreErrors)
{
    //WIP
}

#if PLUGIN_ARCHITECTURE(X11)
void WebPageProxy::createPluginContainer(uint64_t& windowID)
{
    notImplemented();
}

void WebPageProxy::windowedPluginGeometryDidChange(const WebCore::IntRect& frameRect, const WebCore::IntRect& clipRect, uint64_t windowID)
{
    notImplemented();
}

void WebPageProxy::windowedPluginVisibilityDidChange(bool isVisible, uint64_t windowID)
{
    notImplemented();
}
#endif

void WebPageProxy::changeSelectedIndex(int32_t selectedIndex)
{
    //WIP
}

void WebPageProxy::closePopupMenu()
{
    //WIP
}

void WebPageProxy::willSetInputMethodState()
{
    //WIP
}

void WebPageProxy::updateEditorState(const EditorState&)
{
    //WIP
}

} // namespace WebKit
