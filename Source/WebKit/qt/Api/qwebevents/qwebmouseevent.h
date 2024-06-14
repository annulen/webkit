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

#pragma once

#include "qwebkitglobal.h"
#include "qwebeventbase.h"

class QWEBKIT_EXPORT QWebMouseEvent : public QWebEvent {
public:
    enum MouseButton
    {
        None = 0x00,
        Left = 0x01,
        Middle = 0x02,
        Right = 0x03,
    };
    QWebMouseEvent() = delete;

    MouseButton which() const;
    int screenX() const;
    int screenY() const;
    int clientX() const;
    int clientY() const;
    int offsetX() const;
    int offsetY() const;
    int pageX() const;
    int pageY() const;

    int x() const;
    int y() const;

    double force() const;
    bool getModifierState(QWebEvent::Modifiers);

private:
    QWebMouseEvent(WebCore::Event *e);
    friend class QWebEventListenerPrivate;
};
