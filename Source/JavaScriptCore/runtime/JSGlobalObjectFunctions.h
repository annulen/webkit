/*
 *  Copyright (C) 1999-2000 Harri Porten (porten@kde.org)
 *  Copyright (C) 2003, 2006, 2007, 2008, 2009, 2010 Apple Inc. All rights reserved.
 *  Copyright (C) 2007 Cameron Zwarich (cwzwarich@uwaterloo.ca)
 *  Copyright (C) 2007 Maks Orlovich
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#pragma once

#include "JSCJSValue.h"
#include <unicode/uchar.h>

namespace JSC {

class ArgList;
class CallFrame;
class JSObject;
using ExecState = CallFrame;

// FIXME: These functions should really be in JSGlobalObject.cpp, but putting them there
// is a 0.5% reduction.

extern const ASCIILiteral ObjectProtoCalledOnNullOrUndefinedError;

EncodedJSValue JSC_HOST_CALL globalFuncEval(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncParseInt(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncParseFloat(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncDecodeURI(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncDecodeURIComponent(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncEncodeURI(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncEncodeURIComponent(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncEscape(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncUnescape(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncThrowTypeError(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncThrowTypeErrorArgumentsCalleeAndCaller(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncMakeTypeError(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncProtoGetter(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncProtoSetter(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncHostPromiseRejectionTracker(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncBuiltinLog(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncBuiltinDescribe(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncImportModule(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncPropertyIsEnumerable(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncOwnKeys(JSGlobalObject*, CallFrame*);
EncodedJSValue JSC_HOST_CALL globalFuncDateTimeFormat(JSGlobalObject*, CallFrame*);

double jsToNumber(StringView);

} // namespace JSC
