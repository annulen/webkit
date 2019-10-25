/*
    Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "WebFrameNetworkingContext.h"

#include "WebFrame.h"
#include "WebPage.h"

#include <QObject>
#include <QVariant>
#include <WebCore/FrameLoader.h>

using namespace WebCore;

namespace WebKit {

WebFrameNetworkingContext::WebFrameNetworkingContext(WebFrame* frame)
    : FrameNetworkingContext(frame->coreFrame())
    , m_mimeSniffingEnabled(true)
{
}

Ref<WebFrameNetworkingContext> WebFrameNetworkingContext::create(WebFrame* frame)
{
    return adoptRef(*new WebFrameNetworkingContext(frame));
}

void WebFrameNetworkingContext::ensurePrivateBrowsingSession(WebsiteDataStoreParameters&&)
{
}

void WebFrameNetworkingContext::ensureWebsiteDataStoreSession(WebsiteDataStoreParameters&&)
{
}

WebFrameLoaderClient* WebFrameNetworkingContext::webFrameLoaderClient() const
{
    if (!frame())
        return nullptr;

    return toWebFrameLoaderClient(frame()->loader().client());
}

QNetworkAccessManager* WebFrameNetworkingContext::networkAccessManager() const
{
    // QTFIXME: This is a leftover of old process model
    // QtMM player may call networkAccessManager() in WebProcess
    // so we cannot just have ASSERT here.
    qWarning("QtWebKit bug: WebFrameNetworkingContext::networkAccessManager() is called");
    return nullptr;
}

}
