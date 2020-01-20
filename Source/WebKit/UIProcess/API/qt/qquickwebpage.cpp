/*
 * Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"
#include "qquickwebpage_p.h"

#include "CoordinatedGraphicsScene.h"
#include "QtWebPageSGNode.h"
#include "qquickwebpage_p_p.h"
#include "qquickwebview_p.h"
#include "qquickwebview_p_p.h"
#include "qwebkittest_p.h"
#include <QQuickWindow>
#include <WKPage.h>
#include <WebCore/IntRect.h>
#include "DrawingAreaProxyCoordinatedGraphics.h"

using namespace WebKit;

QQuickWebPage::QQuickWebPage(QQuickWebView* viewportItem)
    : QQuickPaintedItem(viewportItem->contentItem())
    , d(new QQuickWebPagePrivate(this, viewportItem))
{
    setFlag(ItemHasContents);
    setClip(true);

    // We do the transform from the top left so the viewport can assume the position 0, 0
    // is always where rendering starts.
    setTransformOrigin(TopLeft);
}

QQuickWebPage::~QQuickWebPage()
{
    delete d;
}

QQuickWebPagePrivate::QQuickWebPagePrivate(QQuickWebPage* q, QQuickWebView* viewportItem)
    : q(q)
    , viewportItem(viewportItem)
    , paintingIsInitialized(false)
    , contentsScale(1)
{
}

void QQuickWebPagePrivate::paint(QPainter* painter, const WebCore::Color& backgroundColor, bool drawsBackground)
{
#if USE(COORDINATED_GRAPHICS)
    if (CoordinatedGraphicsScene* scene = QQuickWebViewPrivate::get(viewportItem)->coordinatedGraphicsScene())
        scene->paintToGraphicsContext(painter, backgroundColor, drawsBackground);
#endif
}
void QQuickWebPage::paint(QPainter* painter)
{
    QQuickWebViewPrivate* webViewPrivate = QQuickWebViewPrivate::get(d->viewportItem);
    WebPageProxy* pageProxy = webViewPrivate->webPageProxy.get();
    auto drawingArea = static_cast<DrawingAreaProxyCoordinatedGraphics*>(pageProxy->drawingArea());
    if(!drawingArea)
    return;

    WebCore::IntRect updateArea(this->boundingRect());
    WebCore::Region unpainted;

    drawingArea->paint(painter,updateArea,unpainted);
}

void QQuickWebPage::setContentsSize(const QSizeF& size)
{
    if (size.isEmpty() || d->contentsSize == size)
        return;

    d->contentsSize = size;
    d->updateSize();
    emit d->viewportItem->experimental()->test()->contentsSizeChanged();
}

const QSizeF& QQuickWebPage::contentsSize() const
{
    return d->contentsSize;
}

void QQuickWebPage::setContentsScale(qreal scale)
{
    ASSERT(scale > 0);
    d->contentsScale = scale;
    d->updateSize();
    emit d->viewportItem->experimental()->test()->contentsScaleChanged();
}

qreal QQuickWebPage::contentsScale() const
{
    ASSERT(d->contentsScale > 0);
    return d->contentsScale;
}

QTransform QQuickWebPage::transformFromItem() const
{
    return transformToItem().inverted();
}

QTransform QQuickWebPage::transformToItem() const
{
    qreal xPos = x();
    qreal yPos = y();

    if (d->viewportItem->experimental()->flickableViewportEnabled()) {
        // Flickable moves its contentItem so we need to take that position into
        // account, as well as the potential displacement of the page on the
        // contentItem because of additional QML items.
        xPos += d->viewportItem->contentItem()->x();
        yPos += d->viewportItem->contentItem()->y();
    }

    return QTransform(d->contentsScale, 0, 0, 0, d->contentsScale, 0, xPos, yPos, 1);
}

void QQuickWebPagePrivate::updateSize()
{
    QSizeF scaledSize = contentsSize * contentsScale;

    q->setSize(scaledSize);

    if (viewportItem->experimental()->flickableViewportEnabled()) {
        // Make sure that the content is sized to the page if the user did not
        // add other flickable items. If that is not the case, the user needs to
        // disable the default content item size property on the WebView and
        // bind the contentWidth and contentHeight accordingly, in accordance
        // accordance with normal Flickable behaviour.
        if (viewportItem->experimental()->useDefaultContentItemSize()) {
            viewportItem->setContentWidth(scaledSize.width());
            viewportItem->setContentHeight(scaledSize.height());
        }
    }
}

QQuickWebPagePrivate::~QQuickWebPagePrivate()
{
}

#include "moc_qquickwebpage_p.cpp"
