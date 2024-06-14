/*
 * Copyright (C) 2022 Rajagopalan Gangadharan <g.raju2000@gmail.com>
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

#include "qwebkeyboardevent.h"
#include "KeyboardEvent.h"

using WebCore::KeyboardEvent;

QWebKeyboardEvent::QWebKeyboardEvent(WebCore::Event *e)
    : QWebEvent(e)
{
}

int QWebKeyboardEvent::keyCode()
{
    KeyboardEvent* event = static_cast<KeyboardEvent*>(m_event);
    return event->keyCode();   
}

int QWebKeyboardEvent::charCode()
{
    KeyboardEvent* event = static_cast<KeyboardEvent*>(m_event);
    return event->charCode();
}

QString QWebKeyboardEvent::keyIdentifier()
{
    KeyboardEvent* event = static_cast<KeyboardEvent*>(m_event);
    return QString::fromUtf8(event->keyIdentifier().utf8().data());
}

QWebKeyboardEvent::KeyLocation QWebKeyboardEvent::keyLocation()
{
    KeyboardEvent* event = static_cast<KeyboardEvent*>(m_event);
    return static_cast<KeyLocation>(event->location());
}

bool QWebKeyboardEvent::getModifierState(QWebEvent::Modifiers modifier)
{
    KeyboardEvent* event = static_cast<KeyboardEvent*>(m_event);
    return event->getModifierState(QWebEvent::modifiers[modifier]);
}
