/*
 * Copyright (C) 2013 University of Szeged
 * Copyright (C) 2013 Renata Hodovan <reni@inf.u-szeged.hu>
 * Copyright (C) 2010 Apple Inc. All rights reserved.
 * Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies)
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

#include "AuxiliaryProcessMain.h"
#include "WebKit2Initialize.h"
#include <QDebug>
#include <QGuiApplication>
#include <QList>
#include <QString>
#include <QStringList>
#include <QUrl>
#include <errno.h>
#include <wtf/RunLoop.h>

#ifndef NDEBUG
#if !OS(WINDOWS)
#include <unistd.h>
#endif
#endif

#if USE(MACH_PORTS)
#include <servers/bootstrap.h>

extern "C" kern_return_t bootstrap_look_up2(mach_port_t, const name_t, mach_port_t*, pid_t, uint64_t);
#endif

#if ENABLE(SUID_SANDBOX_LINUX)
#include "SandboxEnvironmentLinux.h"
#include <sys/wait.h>
#endif

using namespace WebCore;

namespace WebKit {
#ifndef NDEBUG
#if OS(WINDOWS)
static void sleep(unsigned seconds)
{
    ::Sleep(seconds * 1000);
}
#endif
#endif

#if ENABLE(SUID_SANDBOX_LINUX)
pid_t chrootMe()
{
    // Get the file descriptor of the socketpair.
    char* sandboxSocketDescriptorString = getenv(SANDBOX_DESCRIPTOR);
    if (!sandboxSocketDescriptorString)
        return -1;

    char* firstInvalidCharacter;
    long int sandboxSocketDescriptor = strtol(sandboxSocketDescriptorString, &firstInvalidCharacter, 10);
    if (*firstInvalidCharacter != '\0') {
        fprintf(stderr, "The socket descriptor of sandbox is not valid.\n");
        return -1;
    }

    // Get the PID of the setuid helper.
    char* sandboxHelperPIDString = getenv(SANDBOX_HELPER_PID);
    pid_t sandboxHelperPID = -1;

    // If no PID is available, the default of -1 will do.
    if (sandboxHelperPIDString) {
        errno = 0;
        sandboxHelperPID = strtol(sandboxHelperPIDString, &firstInvalidCharacter, 10);
        if (*firstInvalidCharacter != '\0') {
            fprintf(stderr, "The PID of sandbox is not valid.\n");
            return -1;
        }
    }

    // Send the chrootMe message to the helper.
    char sandboxMeMessage = MSG_CHROOTME;
    ssize_t numberOfCharacters = write(sandboxSocketDescriptor, &sandboxMeMessage, 1);
    if (numberOfCharacters != 1) {
        fprintf(stderr, "ChrootMe msg failed to write: %s.\n", strerror(errno));
        return -1;
    }

    // Read the acknowledgement message from the helper.
    numberOfCharacters = read(sandboxSocketDescriptor, &sandboxMeMessage, 1);
    if (numberOfCharacters != 1 || sandboxMeMessage != MSG_CHROOTED) {
        fprintf(stderr, "Couldn't read the confirmation message: %s.\n", strerror(errno));
        return -1;
    }
    close(sandboxSocketDescriptor);

    // Wait for the helper process.
    int expectedPID = waitpid(sandboxHelperPID, 0, 0);
    if (expectedPID != -1 && (sandboxHelperPID == -1 || expectedPID == sandboxHelperPID))
        return expectedPID;
    fprintf(stderr, "Couldn't wait for the helper process: %s\n", strerror(errno));
    return -1;
}
#endif

class WebProcessMain final : public AuxiliaryProcessMainBase {
public:
    bool platformInitialize() override
    {
        return true;
    }
};

Q_DECL_EXPORT int WebProcessMainQt(QGuiApplication* app,int argc,char** argv)
{
#if ENABLE(SUID_SANDBOX_LINUX)
    pid_t helper = chrootMe();
    if (helper == -1) {
        fprintf(stderr, "Asking for chroot failed.\n");
        return -1;
    }
#endif
    if (app->arguments().size() <= 1) {
        qDebug() << "Error: wrong number of arguments.";
        return 1;
    }

#if USE(MACH_PORTS)
    QString serviceName = app->arguments().value(1);

    // Get the server port.
    mach_port_t identifier;
    kern_return_t kr = bootstrap_look_up2(bootstrap_port, serviceName.toUtf8().data(), &identifier, 0, 0);
    if (kr) {
        printf("bootstrap_look_up2 result: %x", kr);
        return 2;
    }
#else
    bool wasNumber = false;
    qulonglong id = app->arguments().at(1).toULongLong(&wasNumber, 10);
    if (!wasNumber) {
        qDebug() << "Error: connection identifier wrong.";
        return 1;
    }
    IPC::Connection::Identifier identifier;
#if OS(WINDOWS)
    // Convert to HANDLE
    identifier = reinterpret_cast<IPC::Connection::Identifier>(id);
#else
    // Convert to int
    identifier = static_cast<IPC::Connection::Identifier>(id);
#endif
#endif

    return AuxiliaryProcessMain<WebProcess, WebProcessMain>(argc, argv);
}

}
