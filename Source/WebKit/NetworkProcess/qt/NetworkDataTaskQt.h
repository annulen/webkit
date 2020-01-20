/*
 * Copyright (C) 2019 Rajagopalan <g.raju2000@gmail.com>
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

#pragma once

#include "config.h"
#include "NetworkDataTask.h"

#include <WebCore/ResourceRequest.h>
#include <QObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <WebCore/QtMIMETypeSniffer.h>

QT_BEGIN_NAMESPACE
class QNetworkRequest;
class QNetworkReply;
QT_END_NAMESPACE

namespace WebKit {

class NetworkDataTaskQt final : public NetworkDataTask {
public:
    static Ref<NetworkDataTask> create(NetworkSession& session, NetworkDataTaskClient& client, const WebCore::ResourceRequest& request, WebCore::StoredCredentialsPolicy storedCredentialsPolicy, WebCore::ContentSniffingPolicy shouldContentSniff, WebCore::ContentEncodingSniffingPolicy shouldContentEncodingSniff, bool shouldClearReferrerOnHTTPSToHTTPRedirect, bool dataTaskIsForMainFrameNavigation)
    {
        return adoptRef(*new NetworkDataTaskQt(session, client, request, storedCredentialsPolicy, shouldContentSniff, shouldContentEncodingSniff, shouldClearReferrerOnHTTPSToHTTPRedirect, dataTaskIsForMainFrameNavigation));
    }
    enum LoadType {
        AsynchronousLoad,
        SynchronousLoad
    };
    static void debug();
    ~NetworkDataTaskQt();

private:
    NetworkDataTaskQt(NetworkSession&, NetworkDataTaskClient&, const WebCore::ResourceRequest&, WebCore::StoredCredentialsPolicy, WebCore::ContentSniffingPolicy, WebCore::ContentEncodingSniffingPolicy, bool shouldClearReferrerOnHTTPSToHTTPRedirect, bool dataTaskIsForMainFrameNavigation);

    void createRequest(WebCore::ResourceRequest&&);
    void start();

    QNetworkReply* sendNetworkRequest(const WebCore::ResourceRequest& request);
    void cancel() override;
    void resume() override;
    void invalidateAndCancel() override;
    NetworkDataTask::State state() const override;

    void recieveMetaData(QNetworkReply*);

    State m_state{ State::Suspended };

    WebCore::ResourceRequest m_currentRequest;
    QNetworkRequest m_qRequest;
    QNetworkAccessManager::Operation m_method;
    QNetworkAccessManager* m_manager;
    MonotonicTime m_startTime;
    LoadType m_loadType;

    QUrl m_redirectionTargetUrl;
    QString m_encoding;
};

} // namespace WebKit
