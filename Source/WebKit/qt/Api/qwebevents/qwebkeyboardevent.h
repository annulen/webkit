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

class QWEBKIT_EXPORT QWebKeyboardEvent : public QWebEvent {
public:
    enum KeyLocation
    {
        Standard = 0x00, // Key doesn't have left/right
        Left = 0x01, // Keys like ctrl which have left
        Right = 0x02, // Keys like ctrl which have right
        Numpad = 0x03, // Keys like numpad keys
    };

    QWebKeyboardEvent() = delete;

    int keyCode();
    int charCode();

    QString keyIdentifier();
    QWebKeyboardEvent::KeyLocation keyLocation();
    bool getModifierState(QWebEvent::Modifiers);
private:
    QWebKeyboardEvent(WebCore::Event *e);
    friend class QWebEventListenerPrivate;
};
 