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

#include <qwebpage.h>
#include <qwebframe.h>
#include <qpainter.h>
#include <qwebelement.h>

class tst_Woff2 : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void renderWoff2Font();
};

void tst_Woff2::renderWoff2Font()
{
    QWebPage page;

    page.mainFrame()->load(QUrl("qrc:/woff2.html"));
    waitForSignal(&page, SIGNAL(loadFinished(bool)));

    QWebElementCollection testDiv = page.mainFrame()->findAllElements("#test");
    QCOMPARE(testDiv.count(), 1);

    QRect imageRect(0, 0, 200, 200);

    QImage testImage(200, 200, QImage::Format_ARGB32);
    QPainter painter0(&testImage);
    painter0.fillRect(imageRect, Qt::white);
    testDiv[0].render(&painter0);
    painter0.end();

    page.mainFrame()->load(QUrl("qrc:/woff2-expected.html"));
    waitForSignal(&page, SIGNAL(loadFinished(bool)));

    QWebElementCollection expectedDiv = page.mainFrame()->findAllElements("#test");
    QCOMPARE(expectedDiv.count(), 1);

    QImage expectedImage(200, 200, QImage::Format_ARGB32);
    QPainter painter1(&expectedImage);
    painter1.fillRect(imageRect, Qt::white);
    expectedDiv[0].render(&painter1);
    painter1.end();

    QVERIFY(testImage == expectedImage);
}

QTEST_MAIN(tst_Woff2)
#include "tst_woff2.moc"
