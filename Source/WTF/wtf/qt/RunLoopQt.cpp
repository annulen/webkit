/*
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
#include "RunLoop.h"

#include <QAbstractEventDispatcher>
#include <QCoreApplication>
#include <QMetaMethod>
#include <QMetaObject>
#include <QObject>
#include <QThread>
#include <QTimerEvent>

namespace WTF {

class RunLoop::PerformWorkTimerObject : public QObject {
    Q_OBJECT
public:
    PerformWorkTimerObject(RunLoop* runLoop)
        : m_runLoop(runLoop)
    {
        int methodIndex = metaObject()->indexOfMethod("performWork()");
        m_method = metaObject()->method(methodIndex);
    }

    Q_SLOT void performWork() { m_runLoop->performWork(); }
    inline void wakeUp() { m_method.invoke(this, Qt::QueuedConnection); }

private:
    RunLoop* m_runLoop;
    QMetaMethod m_method;
};

class RunLoop::TimerObject : public QObject {
    Q_OBJECT
public:
    TimerObject(RunLoop* runLoop)
        : m_runLoop(runLoop)
    {
        m_startMethod = metaObject()->method(metaObject()->indexOfMethod("startWithLock(RunLoop::TimerBase*,int)"));
        m_stopMethod = metaObject()->method(metaObject()->indexOfMethod("stopWithLock(RunLoop::TimerBase*)"));
    }

    ~TimerObject()
    {
        LockHolder locker(m_runLoop->m_loopLock);
        TimerObjectMap::iterator it = m_runLoop->m_timerObjects.find(QThread::currentThread());
        if (it != m_runLoop->m_timerObjects.end())
            m_runLoop->m_timerObjects.remove(it);
    }

    Q_SLOT void start(RunLoop::TimerBase* timer, int milliseconds) {
        ASSERT(timer);
        timer->m_ID = startTimer(milliseconds);
        ASSERT(timer->m_ID);
        m_runLoop->m_activeTimers.add(timer->m_ID, timer);
    }

    Q_SLOT void stop(RunLoop::TimerBase* timer) {
        ASSERT(timer);
        killTimer(timer->m_ID);
        TimerMap::iterator it = m_runLoop->m_activeTimers.find(timer->m_ID);
        if (it != m_runLoop->m_activeTimers.end())
            m_runLoop->m_activeTimers.remove(it);
        timer->m_ID = 0;
    }
    Q_SLOT void startWithLock(RunLoop::TimerBase* timer, int milliseconds) {
        LockHolder locker(m_runLoop->m_loopLock);
        start(timer, milliseconds);
    }
    Q_SLOT void stopWithLock(RunLoop::TimerBase* timer) {
        LockHolder locker(m_runLoop->m_loopLock);
        stop(timer);
    }

    inline void startAsync(RunLoop::TimerBase *timer, int milliseconds) { m_startMethod.invoke(this, Qt::QueuedConnection, Q_ARG(RunLoop::TimerBase*, timer), Q_ARG(int, milliseconds)); }
    inline void stopAsync(RunLoop::TimerBase *timer) { m_stopMethod.invoke(this, Qt::QueuedConnection, Q_ARG(RunLoop::TimerBase*, timer)); }

protected:
    void timerEvent(QTimerEvent* event) override
    {
        RunLoop::TimerBase::timerFired(m_runLoop, event->timerId());
    }

private:
    RunLoop* m_runLoop;
    QMetaMethod m_startMethod;
    QMetaMethod m_stopMethod;
};

static QEventLoop* currentEventLoop;

void RunLoop::run()
{
    static bool mainEventLoopIsRunning = false;
    if (!mainEventLoopIsRunning) {
        mainEventLoopIsRunning = true;
        QCoreApplication::exec();
        mainEventLoopIsRunning = false;
    } else {
        QEventLoop eventLoop;
        QEventLoop* previousEventLoop = currentEventLoop;
        currentEventLoop = &eventLoop;

        eventLoop.exec();

        currentEventLoop = previousEventLoop;
    }
}

void RunLoop::stop()
{
    if (currentEventLoop)
        currentEventLoop->exit();
    else
        QCoreApplication::exit();
}

RunLoop::RunLoop()
{
    QEventLoop tmpLoop; // Call QEventLoop ctor for ensureEventDispatcher()
    qRegisterMetaType<RunLoop::TimerBase*>("RunLoop::TimerBase*");
    m_timer = new TimerObject(this);
    m_performWorkTimer = new PerformWorkTimerObject(this);
}

RunLoop::~RunLoop()
{
    delete m_timer;
    delete m_performWorkTimer;
}

void RunLoop::wakeUp()
{
    m_performWorkTimer->wakeUp();
}

RunLoop::CycleResult RunLoop::cycle(const String&)
{
    QCoreApplication::processEvents(QEventLoop::WaitForMoreEvents);
    return CycleResult::Continue;
}

// RunLoop::Timer

void RunLoop::TimerBase::timerFired(RunLoop* runLoop, int ID)
{
    TimerMap::iterator it = runLoop->m_activeTimers.find(ID);
    ASSERT(it != runLoop->m_activeTimers.end());
    TimerBase* timer = it->value;

    if (!timer->m_isRepeating)
        timer->stop();

    timer->fired();
}

RunLoop::TimerBase::TimerBase(RunLoop& runLoop)
    : m_runLoop(runLoop)
    , m_ID(0)
    , m_isRepeating(false)
{
}

RunLoop::TimerBase::~TimerBase()
{
    stop();
}

void RunLoop::TimerBase::start(Seconds nextFireInterval, bool repeat)
{
    stop();

    LockHolder locker(m_runLoop->m_loopLock);

    m_isActive = true;
    m_isRepeating = repeat;
    int millis = nextFireInterval.millisecondsAs<int>();
    if (m_runLoop->m_timer->thread() == QThread::currentThread())
        m_runLoop->m_timer->start(this, millis);
    else {
        QThread* thread = QThread::currentThread();
        TimerObjectMap::iterator it = m_runLoop->m_timerObjects.find(thread);
        TimerObject* timer = 0;
        if (it == m_runLoop->m_timerObjects.end()) {
            timer = new TimerObject(m_runLoop.ptr());
            timer->moveToThread(thread);
            m_runLoop->m_timerObjects.set(thread, timer);
        } else
            timer = it->value;

        ASSERT(timer);
        timer->startAsync(this, millis);
    }
}

void RunLoop::TimerBase::stop()
{
    LockHolder locker(m_runLoop->m_loopLock);
    if (!m_isActive)
        return;

    m_isActive = false;

    if (m_runLoop->m_timer->thread() == QThread::currentThread())
        m_runLoop->m_timer->stop(this);
    else {
        QThread* thread = QThread::currentThread();
        TimerObjectMap::iterator it = m_runLoop->m_timerObjects.find(thread);
        if (it != m_runLoop->m_timerObjects.end())
            it->value->stopAsync(this);;
    }
}

bool RunLoop::TimerBase::isActive() const
{
    LockHolder locker(m_runLoop->m_loopLock);
    return m_isActive;
}

#include "RunLoopQt.moc"

} // namespace WTF
