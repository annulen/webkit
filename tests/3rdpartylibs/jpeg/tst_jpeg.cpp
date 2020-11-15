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

#include <qpainter.h>
#include <qwebpage.h>
#include <qwebelement.h>
#include <qwebframe.h>

class tst_Libjpeg : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void decodeAndCompare();
};
void tst_Libjpeg::decodeAndCompare()
{
    QString html(QStringLiteral("<img src='qrc:///image.jpg'>"));

    QWebPage page;
    QSignalSpy loadSpy(&page, SIGNAL(loadFinished(bool)));
    page.mainFrame()->setHtml(html);

    waitForSignal(&page, SIGNAL(loadFinished(bool)));
    QCOMPARE(loadSpy.count(), 1);

    QWebElementCollection imgs = page.mainFrame()->findAllElements("img");
    QCOMPARE(imgs.count(), 1);

    QImage resource(":/image.jpg");
    QRect imageRect(0, 0, resource.width(), resource.height());

    QImage testImage(resource.width(), resource.height(), QImage::Format_ARGB32);
    QPainter painter0(&testImage);
    painter0.fillRect(imageRect, Qt::white);
    QPixmap pix = QPixmap::fromImage(resource);
    painter0.drawPixmap(0, 0, pix);
    painter0.end();

    QImage renderedImage(resource.width(), resource.height(), QImage::Format_ARGB32);
    QPainter painter1(&renderedImage);
    painter1.fillRect(imageRect, Qt::white);
    imgs[0].render(&painter1);
    painter1.end();

    QVERIFY(renderedImage == testImage);
}

QTEST_MAIN(tst_Libjpeg)
#include "tst_jpeg.moc"
