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

class tst_ICU : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void encodeDecodeText();
    void jsIntlAPI();
};

void tst_ICU::encodeDecodeText()
{
    QWebPage page;

    QSignalSpy loadSpy(&page, SIGNAL(loadFinished(bool)));
    page.mainFrame()->load(QUrl("qrc:///test.html"));

    waitForSignal(&page, SIGNAL(loadFinished(bool)));
    QCOMPARE(loadSpy.count(), 1);

    QVariant res = page.mainFrame()->evaluateJavaScript("document.body.innerText");
    QCOMPARE(res.toString(), QStringLiteral("Привет мир!"));
}

void tst_ICU::jsIntlAPI()
{
    QWebPage page;
    QVariant res;

    //Test: DateTime[en-US,en-GB]
    page.mainFrame()->evaluateJavaScript("const date = new Date(Date.UTC(2012, 11, 20, 3, 0, 0));");
    res = page.mainFrame()->evaluateJavaScript("new Intl.DateTimeFormat('en-US').format(date)");
    QCOMPARE(res.toString(), QStringLiteral("12/20/2012"));
    res = page.mainFrame()->evaluateJavaScript("new Intl.DateTimeFormat('en-GB').format(date)");
    QCOMPARE(res.toString(), QStringLiteral("20/12/2012"));

    //Test: Collator
    page.mainFrame()->evaluateJavaScript(
        "function letterSort(lang, letters) {"
        "letters.sort(new Intl.Collator(lang).compare);"
        "return letters;}");
    res = page.mainFrame()->evaluateJavaScript("letterSort('de', [ 'a', 'z', 'ä' ]).toString()");
    QCOMPARE(res.toString(), QStringLiteral("a,ä,z"));
    res = page.mainFrame()->evaluateJavaScript("letterSort('sv', [ 'a', 'z', 'ä' ]).toString()");
    QCOMPARE(res.toString(), QStringLiteral("a,z,ä"));

    //Test: Currency[euro,yen]
    page.mainFrame()->evaluateJavaScript("const number = 123456.789;");
    res = page.mainFrame()->evaluateJavaScript("new Intl.NumberFormat('de-DE', { style: 'currency', currency: 'EUR' }).format(number)");
    QEXPECT_FAIL("", "Latest Webkit Required", Continue);
    QCOMPARE(res.toString(), QStringLiteral("123.456,79 €"));
    res = page.mainFrame()->evaluateJavaScript("new Intl.NumberFormat('ja-JP', { style: 'currency', currency: 'JPY' }).format(number)");
    QEXPECT_FAIL("", "Latest Webkit Required", Continue);
    QCOMPARE(res.toString(), QStringLiteral("￥123,457"));

    //Test: Text Translation of script names
    page.mainFrame()->evaluateJavaScript("const regionNamesInEnglish = new Intl.DisplayNames(['en'], { type: 'region' });");
    page.mainFrame()->evaluateJavaScript("const regionNamesInTraditionalChinese = new Intl.DisplayNames(['zh-Hant'], { type: 'region' });");
    res = page.mainFrame()->evaluateJavaScript("regionNamesInEnglish.of('US')");
    QEXPECT_FAIL("", "Latest Webkit Required", Continue);
    QCOMPARE(res.toString(), QStringLiteral("United States"));
    res = page.mainFrame()->evaluateJavaScript("regionNamesInTraditionalChinese.of('US')");
    QEXPECT_FAIL("", "Latest Webkit Required", Continue);
    QCOMPARE(res.toString(), QStringLiteral("美國"));
}

QTEST_MAIN(tst_ICU)
#include "tst_icu.moc"
