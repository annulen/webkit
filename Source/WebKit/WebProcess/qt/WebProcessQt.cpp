/*
 * Copyright (C) 2010, 2013 Apple Inc. All rights reserved.
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
#include "WebProcess.h"

#include "InjectedBundle.h"
#include "QtBuiltinBundle.h"
#include "SeccompFiltersWebProcessQt.h"
#include "WKBundleAPICast.h"
#include "WebProcessCreationParameters.h"

#include <QCoreApplication>
#include <wtf/FileSystem.h>
#include <WebCore/MemoryCache.h>
#include <WebCore/RuntimeEnabledFeatures.h>
#include <wtf/RAMSize.h>

#if defined(Q_OS_MACOS)
#include <dispatch/dispatch.h>
#include <mach/host_info.h>
#include <mach/mach.h>
#include <mach/mach_error.h>
#elif !defined(Q_OS_WIN)
#include <unistd.h>
#endif

using namespace WebCore;

namespace WebKit {

void WebProcess::platformSetCacheModel(CacheModel cacheModel)
{
    // FIXME: Implement hybrid in-memory- and disk-caching as e.g. the Mac port does.
}

#if defined(Q_OS_MACOS)
static void parentProcessDiedCallback(void*)
{
    QCoreApplication::quit();
}
#endif

void WebProcess::platformInitializeWebProcess(WebProcessCreationParameters& parameters)
{
    // QTFIXME: leftover of old process model
#if 0
#if ENABLE(SECCOMP_FILTERS)
    {
        WebKit::SeccompFiltersWebProcessQt seccompFilters(parameters);
        seccompFilters.initialize();
    }
#endif
    m_networkAccessManager = new QtNetworkAccessManager(this);

    if (!parameters.cookieStorageDirectory.isEmpty()) {
        WebCore::SharedCookieJarQt* jar = WebCore::SharedCookieJarQt::create(parameters.cookieStorageDirectory);
        m_networkAccessManager->setCookieJar(jar);
        // Do not let QNetworkAccessManager delete the jar.
        jar->setParent(0);
    }

    if (!parameters.diskCacheDirectory.isEmpty()) {
        QNetworkDiskCache* diskCache = new QNetworkDiskCache();
        diskCache->setCacheDirectory(parameters.diskCacheDirectory);
        // The m_networkAccessManager takes ownership of the diskCache object upon the following call.
        m_networkAccessManager->setCache(diskCache);
    }

#if defined(Q_OS_MACOS)
    pid_t ppid = getppid();
    dispatch_queue_t queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    dispatch_source_t source = dispatch_source_create(DISPATCH_SOURCE_TYPE_PROC, ppid, DISPATCH_PROC_EXIT, queue);
    if (source) {
        dispatch_source_set_event_handler_f(source, parentProcessDiedCallback);
        dispatch_resume(source);
    }
#endif

    // We'll only install the Qt builtin bundle if we don't have one given by the UI process.
    // Currently only WTR provides its own bundle.
    if (parameters.injectedBundlePath.isEmpty()) {
        m_injectedBundle = InjectedBundle::create(parameters, transformHandlesToObjects(parameters.initializationUserData.object()).get());
        QtBuiltinBundle::shared().initialize(toAPI(m_injectedBundle.get()));
    }
#endif
}

void WebProcess::platformSetWebsiteDataStoreParameters(WebProcessDataStoreParameters&&)
{

}

void WebProcess::platformTerminate()
{
    // QTFIXME: leftover of old process model
#if 0
    delete m_networkAccessManager;
    m_networkAccessManager = 0;
    WebCore::SharedCookieJarQt::shared()->destroy();
#endif
}

} // namespace WebKit
