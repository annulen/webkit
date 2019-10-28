/*
 * Copyright (C) 2010 Apple Inc. All rights reserved.
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
#include "ShareableBitmap.h"

#include "NativeImageQt.h"
#include <QImage>
#include <QPainter>
#include <QtGlobal>
#include <WebCore/BitmapImage.h>
#include <WebCore/GraphicsContext.h>

using namespace WebCore;

namespace WebKit {

QImage ShareableBitmap::createQImage()
{
    ref(); // Balanced by deref in releaseSharedMemoryData
    return QImage(reinterpret_cast<uchar*>(data()), m_size.width(), m_size.height(), m_size.width() * 4,
                  NativeImageQt::defaultFormatForOpaqueImages(),//FIXME
                  releaseSharedMemoryData, this);
}

void ShareableBitmap::releaseSharedMemoryData(void* typelessBitmap)
{
    static_cast<ShareableBitmap*>(typelessBitmap)->deref(); // Balanced by ref in createQImage.
}

RefPtr<Image> ShareableBitmap::createImage()
{
    //Should NativeImagePtr be QPixmap or QImage which is better - FIXME
    QImage image = createQImage();
    return BitmapImage::create(WTFMove(image));
}

std::unique_ptr<GraphicsContext> ShareableBitmap::createGraphicsContext()
{
    // QTFIXME: Shallow QImage leaks here, but it should not be destroyed earlier than QPainter
    QImage* image = new QImage(createQImage());
    QPainter* painter = new QPainter(image);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    auto context = std::make_unique<GraphicsContext>(painter);
    context->takeOwnershipOfPlatformContext();
    return context;
}

void ShareableBitmap::paint(GraphicsContext& context, const IntPoint& dstPoint, const IntRect& srcRect)
{
    QImage image = createQImage();
    QPainter* painter = context.platformContext();
    painter->drawImage(dstPoint, image, QRect(srcRect));
}

void ShareableBitmap::paint(GraphicsContext& context, float scaleFactor, const IntPoint& dstPoint, const IntRect& srcRect)
{
    if (qFuzzyCompare(scaleFactor, 1)) {
        paint(context, dstPoint, srcRect);
        return;
    }

    QImage image = createQImage();
    QPainter* painter = context.platformContext();

    painter->save();
    painter->scale(scaleFactor, scaleFactor);
    painter->drawImage(dstPoint, image, QRect(srcRect));
    painter->restore();
}

Checked<unsigned, RecordOverflow> ShareableBitmap::calculateBytesPerRow(WebCore::IntSize size, const ShareableBitmap::Configuration& config)
{
    unsigned bytes = calculateBytesPerPixel(config)*size.width();
    return bytes;
}

unsigned ShareableBitmap::calculateBytesPerPixel(const Configuration&)
{
    return 4;	
}

}
// namespace WebKit
