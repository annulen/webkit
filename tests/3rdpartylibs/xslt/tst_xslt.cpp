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

class tst_Libxslt : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void applyXSLTtoXML();
};

void tst_Libxslt::applyXSLTtoXML()
{
    QWebPage page;

    QSignalSpy loadSpy(&page, SIGNAL(loadFinished(bool)));
    page.mainFrame()->load(QUrl("qrc:///test.html"));

    waitForSignal(&page, SIGNAL(loadFinished(bool)));
    QCOMPARE(loadSpy.count(), 1);

    QVariant res = page.mainFrame()->evaluateJavaScript("document.getElementById('child1').innerText");
    QCOMPARE(res.toString(), QStringLiteral("Child1"));

    res = page.mainFrame()->evaluateJavaScript("document.getElementById('child1').style.backgroundColor");
    QCOMPARE(res.toString(), QStringLiteral("red"));

    res = page.mainFrame()->evaluateJavaScript("document.getElementById('child2').innerText");
    QCOMPARE(res.toString(), QStringLiteral("Child2"));

    res = page.mainFrame()->evaluateJavaScript("document.getElementById('child2').style.backgroundColor");
    QCOMPARE(res.toString(), QStringLiteral("blue"));
}

QTEST_MAIN(tst_Libxslt)
#include "tst_xslt.moc"
