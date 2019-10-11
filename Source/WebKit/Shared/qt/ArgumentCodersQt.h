/*
    Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies)

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

#ifndef ArgumentCodersQt_h
#define ArgumentCodersQt_h

#include "Decoder.h"
#include "Encoder.h"
#include "DragData.h"

namespace IPC {

void encode(Encoder&, const WebCore::DragData&);
bool decode(Decoder&, WebCore::DragData&);

template<> struct Coder<WebCore::DragData> {
    static void encode(Encoder&, const WebCore::DragData&);
    static bool decode(Decoder&, WebCore::DragData&);
};

}

#endif // ArgumentCodersQt_h
