/*
 * Copyright (C) 2021 Rajagopalan Gangadharan <g.raju2000@gmail.com>
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <QUuid>
#include <QThread>
#include <QDebug>
#include "qwebeventlistener.h"
#include "qwebeventlistener_p.h"

#include "ScriptExecutionContext.h"
#include "Event.h"
#include "EventListener.h"

WebEventSignal::WebEventSignal(QObject* parent)
    : QObject(parent)
{
}

WebEventSignal::~WebEventSignal()
{
}

QWebEventListenerPrivate::QWebEventListenerPrivate()
    :EventListener(CPPEventListenerType),
    uuid(QUuid::createUuid()),
    eventSignal(new WebEventSignal()),
    eventHandler(nullptr)
{
}

QWebEventListenerPrivate::QWebEventListenerPrivate(CallBackEvent userCallback)
    :EventListener(CPPEventListenerType),
    uuid(QUuid::createUuid()),
    eventSignal(nullptr),
    eventHandler(userCallback)
{
}

QWebEventListenerPrivate::~QWebEventListenerPrivate()
{
    delete eventSignal;
}

void QWebEventListenerPrivate::handleEvent(WebCore::ScriptExecutionContext*, WebCore::Event* e)
{
    if(eventSignal)
        emit eventSignal->eventSignal();
    if(eventHandler)
        eventHandler();
}

bool QWebEventListenerPrivate::operator==(const WebCore::EventListener& other)
{
    return (other.type() == CPPEventListenerType &&
        reinterpret_cast<const QWebEventListenerPrivate*>(&other)->getUuid() == uuid);
}

QUuid QWebEventListenerPrivate::getUuid() const
{
    return uuid;
}


QWebEventListener::QWebEventListener(QWebEventListenerPrivate* priv)
    :d(priv)
{
}

// Allows user to emit signal for events and attach custom call back
QWebEventListener::QWebEventListener()
    :d(new QWebEventListenerPrivate())
{
}

// Allows user to attach custom call back but doesn't emit a signal for events
QWebEventListener::QWebEventListener(CallBackEvent eventCallback)
    :d(new QWebEventListenerPrivate(eventCallback))
{
}

void QWebEventListener::setCallBack(CallBackEvent eventCallback)
{
    d->eventHandler = eventCallback;
}

CallBackEvent QWebEventListener::getCallBack()
{
    return d->eventHandler;
}

QWebEventListener::~QWebEventListener()
{
    //WebCore::EventListener takes ownership of d
}

bool QWebEventListener::operator==(const QWebEventListener& other)
{
    return (d == other.d);
}

WebEventSignal* QWebEventListener::handleEventObject()
{
    if(d->eventSignal)
        return d->eventSignal;
    return nullptr;
}

