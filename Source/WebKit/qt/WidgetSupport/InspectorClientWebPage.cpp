/*
 * Copyright (C) 2007 Apple Inc.  All rights reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 * Copyright (C) 2008 Holger Hans Peter Freyther
 * Copyright (C) 2015 The Qt Company Ltd
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "InspectorClientWebPage.h"

#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>

#include <qwebelement.h>
#include <qwebframe.h>
#include <qwebframe_p.h>
#include <qwebpage_p.h>

using namespace WebKit;

InspectorClientWebPage::InspectorClientWebPage()
{
    QWebView* view = new QWebView;
    view->setPage(this);
    setParent(view);
    settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
#if !ENABLE(DEVELOPER_MODE)
    settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, false);
#endif
    connect(mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), SLOT(javaScriptWindowObjectCleared()));

    // FIXME: Find out what's going on with Settings
    settings()->setAttribute(QWebSettings::AcceleratedCompositingEnabled, false);

    // We treat "qrc:" scheme as local, but by default local content is not allowed to use
    // LocalStorage which is required for Inspector to work.
    // See https://bugs.webkit.org/show_bug.cgi?id=155265
    // Alternatively we can make "qrc:" scheme non-local like GTK port does:
    // https://bugs.webkit.org/show_bug.cgi?id=155497
    settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
}

QWebPage* InspectorClientWebPage::createWindow(QWebPage::WebWindowType)
{
    QWebView* view = new QWebView;
    QWebPage* page = new QWebPage;
    view->setPage(page);
    view->setAttribute(Qt::WA_DeleteOnClose);
    return page;
}

bool InspectorClientWebPage::event(QEvent* ev)
{
    if (ev->type() == QEvent::ContextMenu) {
        auto* contextMenuEvent = static_cast<QContextMenuEvent*>(ev);

        if (contextMenuEvent)
            m_clickPos = contextMenuEvent->pos();
    }

    return QWebPage::event(ev);
}

void InspectorClientWebPage::javaScriptWindowObjectCleared()
{
    QVariant inspectorJavaScriptWindowObjects = property("_q_inspectorJavaScriptWindowObjects");
    if (!inspectorJavaScriptWindowObjects.isValid())
        return;
    QMap<QString, QVariant> javaScriptNameObjectMap = inspectorJavaScriptWindowObjects.toMap();
    QWebFrame* frame = mainFrame();
    QMap<QString, QVariant>::const_iterator it = javaScriptNameObjectMap.constBegin();
    for ( ; it != javaScriptNameObjectMap.constEnd(); ++it) {
        QString name = it.key();
        QVariant value = it.value();
        QObject* obj = value.value<QObject*>();
        frame->addToJavaScriptWindowObject(name, obj);
    }
}

void InspectorClientWebPage::triggerAction(WebAction action, bool checked)
{
    const QWebHitTestResult hitTestResult = mainFrame()->hitTestContent(m_clickPos);

    if (hitTestResult.imageUrl().isValid() && hitTestResult.element().hasAttribute(QStringLiteral("data-url"))) {
        switch (action) {
        case OpenImageInNewWindow: {
            auto* frame = static_cast<QWebFramePrivate*>(hitTestResult.frame()->d);

            if (frame) {
                QWebPagePrivate::openNewWindow(QUrl(hitTestResult.element().attribute(QStringLiteral("data-url"))), frame->frame);
                return;
            }
        }

        case CopyImageUrlToClipboard:
            QApplication::clipboard()->setText(hitTestResult.element().attribute(QStringLiteral("data-url")));
            return;
        default:
            break;
        }
    }

    QWebPage::triggerAction(action, checked);
}
