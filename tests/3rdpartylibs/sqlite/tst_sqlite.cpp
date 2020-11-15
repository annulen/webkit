/*
 * Copyright (C) 2020 Rajagopalan-Gangadharan <g.raju2000@gmail.com>
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

#include <QtTest/QtTest>
#include "../util.h"

#include <QTemporaryDir>
#include <qwebpage.h>
#include <qwebframe.h>

class tst_Sqlite : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void indexedDBTest();
};

void tst_Sqlite::indexedDBTest()
{
    QTemporaryDir tempDir;
    QVERIFY(tempDir.isValid());

    QString path = tempDir.path();
    QWebPage page;

    page.settings()->setOfflineStoragePath(path);
    QVERIFY(page.settings()->offlineStoragePath() == path);

    QWebSettings::setOfflineStorageDefaultQuota(1024 * 1024);
    QVERIFY(QWebSettings::offlineStorageDefaultQuota() == 1024 * 1024);

    page.settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);

    QString dbFileName = path + "Databases.db";

    if (QFile::exists(dbFileName))
        QFile::remove(dbFileName);

    QSignalSpy loadSpy(&page, SIGNAL(loadFinished(bool)));
    page.mainFrame()->load(QUrl("qrc:///test.html"));

    waitForSignal(&page, SIGNAL(loadFinished(bool)));
    QCOMPARE(loadSpy.count(), 1);

    waitForSignal(&page, SIGNAL(loadFinished(bool)), 2000);

    QVariant res = page.mainFrame()->evaluateJavaScript("res");
    QCOMPARE(res.toString(), QStringLiteral("Database Success"));

    page.mainFrame()->evaluateJavaScript("add('1', 'Name1')");
    waitForSignal(&page, SIGNAL(loadFinished(bool)), 1000);

    page.mainFrame()->evaluateJavaScript("read('1')");
    waitForSignal(&page, SIGNAL(loadFinished(bool)), 1000);

    res = page.mainFrame()->evaluateJavaScript("res");
    QCOMPARE(res.toString(), QStringLiteral("Name1"));
}

QTEST_MAIN(tst_Sqlite)
#include "tst_sqlite.moc"
