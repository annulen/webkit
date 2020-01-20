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

#include "config.h"
#include "NetworkDataTaskQt.h"

#include <QNetworkRequest>
#include <QByteArray>
#include <QNetworkReply>
#include <QCoreApplication>

#include <WebCore/ResourceRequest.h>
#include <WebCore/QtMIMETypeSniffer.h>
#include <WebCore/MIMETypeRegistry.h>
#include <WebCore/HTTPParsers.h>
#include <WebCore/SharedBuffer.h>

namespace WebKit {
using namespace WebCore;

NetworkDataTaskQt::NetworkDataTaskQt(NetworkSession& session, NetworkDataTaskClient& client, const ResourceRequest& requestWithCredentials, StoredCredentialsPolicy storedCredentialsPolicy, ContentSniffingPolicy shouldContentSniff, WebCore::ContentEncodingSniffingPolicy, bool shouldClearReferrerOnHTTPSToHTTPRedirect, bool dataTaskIsForMainFrameNavigation)
    : NetworkDataTask(session, client, requestWithCredentials, storedCredentialsPolicy, shouldClearReferrerOnHTTPSToHTTPRedirect, dataTaskIsForMainFrameNavigation)
{
    auto request = requestWithCredentials;
    if (request.url().protocolIsInHTTPFamily()) {
        m_startTime = MonotonicTime::now();
        auto url = request.url();
        if (m_storedCredentialsPolicy == StoredCredentialsPolicy::Use) {
            m_user = url.user();
            m_password = url.pass();
            request.removeCredentials();
        }
    }
    createRequest(WTFMove(request));
}

NetworkDataTaskQt::~NetworkDataTaskQt()
{
}

void NetworkDataTaskQt::createRequest(ResourceRequest&& request)
{
    m_currentRequest=WTFMove(request);
    m_qRequest=m_currentRequest.toNetworkRequest(NULL);

    if (m_currentRequest.httpMethod() == "GET")
        m_method = QNetworkAccessManager::GetOperation;
    else if (m_currentRequest.httpMethod() == "HEAD")
        m_method = QNetworkAccessManager::HeadOperation;
    else if (m_currentRequest.httpMethod() == "POST")
        m_method = QNetworkAccessManager::PostOperation;
    else if (m_currentRequest.httpMethod() == "PUT")
        m_method = QNetworkAccessManager::PutOperation;
    else if (m_currentRequest.httpMethod() == "DELETE" && !m_currentRequest.httpBody()) // A delete with a body is a custom operation.
        m_method = QNetworkAccessManager::DeleteOperation;
    else
        m_method = QNetworkAccessManager::CustomOperation;
    
    m_manager = new QNetworkAccessManager();  
    QObject::connect(m_manager, &QNetworkAccessManager::finished,
        &NetworkDataTaskQt::debug);
    start();
}

void NetworkDataTaskQt::start()
{
    QNetworkReply* reply = sendNetworkRequest(m_currentRequest);
    
    QObject::connect(reply,&QIODevice::readyRead,[this,reply](){
        this->recieveMetaData(reply);
    });
    
}

void NetworkDataTaskQt::debug()
{
    qDebug()<<"Debug";
}
void NetworkDataTaskQt::recieveMetaData(QNetworkReply* reply)
{
    Vector<char> buffer(8128);
    qint64 readSize = reply->read(buffer.data(),buffer.size());
    if (readSize > 0)
    {
        buffer.shrink(readSize);
        m_client->didReceiveData(SharedBuffer::create(WTFMove(buffer)));
    }

}

QNetworkReply* NetworkDataTaskQt::sendNetworkRequest(const ResourceRequest& request)
{
    if (m_loadType == SynchronousLoad)
        m_qRequest.setAttribute(QNetworkRequest::SynchronousRequestAttribute, true);

    if (!m_manager)
        return 0;

    const QUrl url = m_qRequest.url();

    m_qRequest.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    if (m_method == QNetworkAccessManager::PostOperation
        && (!url.toLocalFile().isEmpty() || url.scheme() == QLatin1String("data")))
        m_method = QNetworkAccessManager::GetOperation;
    
    switch (m_method) {
    case QNetworkAccessManager::GetOperation:
        //clearContentHeaders();
        {
            m_qRequest.setHeader(QNetworkRequest::ContentTypeHeader, QVariant());
            m_qRequest.setHeader(QNetworkRequest::ContentLengthHeader, QVariant());
        }
        return m_manager->get(m_qRequest);
    case QNetworkAccessManager::PostOperation:
    fprintf(stderr,"\n POST \n");
    break;
    case QNetworkAccessManager::UnknownOperation:
        ASSERT_NOT_REACHED();
        return 0;
    }
    return 0;
}

void NetworkDataTaskQt::resume()
{
}

void NetworkDataTaskQt::cancel()
{
    
}

void NetworkDataTaskQt::invalidateAndCancel()
{
    cancel();   
}

NetworkDataTask::State NetworkDataTaskQt::state() const
{
    return m_state;	
}
} // namespace WebKit

