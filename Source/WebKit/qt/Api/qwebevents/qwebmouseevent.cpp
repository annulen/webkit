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

#include "qwebmouseevent.h"
#include "MouseEvent.h"

using WebCore::MouseEvent;

QWebMouseEvent::QWebMouseEvent(WebCore::Event *e)
    : QWebEvent(e)
{
}

QWebMouseEvent::MouseButton QWebMouseEvent::which() const
{
    MouseEvent* event = static_cast<MouseEvent*>(m_event);
    return static_cast<QWebMouseEvent::MouseButton>(event->which());
}

int QWebMouseEvent::screenX() const
{
    MouseEvent* event = static_cast<MouseEvent*>(m_event);
    return event->screenX();
}

int QWebMouseEvent::screenY() const
{
    MouseEvent* event = static_cast<MouseEvent*>(m_event);
    return event->screenY();
}

int QWebMouseEvent::clientX() const
{
    MouseEvent* event = static_cast<MouseEvent*>(m_event);
    return event->clientX();
}

int QWebMouseEvent::clientY() const
{
    MouseEvent* event = static_cast<MouseEvent*>(m_event);
    return event->clientY();
}

int QWebMouseEvent::offsetX() const
{
    MouseEvent* event = static_cast<MouseEvent*>(m_event);
    return event->offsetX();
}

int QWebMouseEvent::offsetY() const
{
    MouseEvent* event = static_cast<MouseEvent*>(m_event);
    return event->offsetY();
}

int QWebMouseEvent::x() const
{
    MouseEvent* event = static_cast<MouseEvent*>(m_event);
    return event->x();
}

int QWebMouseEvent::y() const
{
    MouseEvent* event = static_cast<MouseEvent*>(m_event);
    return event->y();
}

double QWebMouseEvent::force() const
{
    // Webkit specific method to get force of force touch trackpads.
    MouseEvent* event = static_cast<MouseEvent*>(m_event);
    return event->force();
}

bool QWebMouseEvent::getModifierState(QWebEvent::Modifiers modifier)
{
    MouseEvent* event = static_cast<MouseEvent*>(m_event);
    if (QWebEvent::modifiers[modifier] == "Control")
        return event->ctrlKey();
    if (QWebEvent::modifiers[modifier] == "Shift")
        return event->shiftKey();
    if (QWebEvent::modifiers[modifier] == "Alt")
        return event->altKey();
    if (QWebEvent::modifiers[modifier] == "Meta")
        return event->metaKey();
    return false;
}
