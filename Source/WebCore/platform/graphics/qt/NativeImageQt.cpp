/*
 * Copyright (C) 2006 Dirk Mueller <mueller@kde.org>
 * Copyright (C) 2006 Zack Rusin <zack@kde.org>
 * Copyright (C) 2006 Simon Hausmann <hausmann@kde.org>
 * Copyright (C) 2009 Torch Mobile Inc. http://www.torchmobile.com/
 * Copyright (C) 2010 Sencha, Inc.
 *
 * All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "NativeImage.h"

#include "AffineTransform.h"
#include "BitmapImage.h"
#include "FloatRect.h"
#include "GraphicsContext.h"
#include "ImageObserver.h"
#include "ShadowBlur.h"
#include "StillImageQt.h"
#include "Timer.h"

#include <QPainter>
#include <QPaintEngine>
#include <QPixmap>
#include <QPixmapCache>

#include <private/qhexstring_p.h>

namespace WebCore {

IntSize nativeImageSize(const NativeImagePtr& image)
{
    return image.size();// image ? IntSize(image->size()) : IntSize();
}

bool nativeImageHasAlpha(const NativeImagePtr& image)
{
    return image.hasAlphaChannel();//  !image || image->hasAlphaChannel();
}

Color nativeImageSinglePixelSolidColor(const NativeImagePtr& image)
{
    if (image.width() != 1 || image.height() != 1)
        return Color();

    return QColor::fromRgba(image.pixel(0, 0));
}

const QImage* prescaleImageIfRequired(QPainter* painter, const QImage* image, QImage* prescaledImage, const QRectF& destRect, QRectF* srcRect)
{
    // The quality of down scaling at 0.5x and below in QPainter is not very good
    // due to using bilinear sampling, so for high quality scaling we need to
    // perform scaling ourselves.
    ASSERT(image);
    ASSERT(painter);
    if (!(painter->renderHints() & QPainter::SmoothPixmapTransform))
        return image;

    if (painter->paintEngine()->type() != QPaintEngine::Raster)
        return image;

    QTransform transform = painter->combinedTransform();

    // Prescaling transforms that does more than scale or translate is not supported.
    if (transform.type() > QTransform::TxScale)
        return image;

    QRectF transformedDst = transform.mapRect(destRect);
    // Only prescale if downscaling to 0.5x or less
    if (transformedDst.width() * 2 > srcRect->width() && transformedDst.height() * 2 > srcRect->height())
        return image;

    // This may not work right with subpixel positions, but that can not currently happen.
    QRect pixelSrc = srcRect->toRect();
    QSize scaledSize = transformedDst.size().toSize();

    QString key = QStringLiteral("qtwebkit_prescaled_")
        % HexString<qint64>(image->cacheKey())
        % HexString<int>(pixelSrc.x()) % HexString<int>(pixelSrc.y())
        % HexString<int>(pixelSrc.width()) % HexString<int>(pixelSrc.height())
        % HexString<int>(scaledSize.width()) % HexString<int>(scaledSize.height());

    QPixmap buffer;

    if (!QPixmapCache::find(key, &buffer)) {
        if (pixelSrc != image->rect())
            buffer = QPixmap::fromImage(image->copy(pixelSrc).scaled(scaledSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        else
            buffer = QPixmap::fromImage(image->scaled(scaledSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        QPixmapCache::insert(key, buffer);
    }

    *srcRect = QRectF(QPointF(), buffer.size());
    *prescaledImage = buffer.toImage();

    return prescaledImage;
}

void drawNativeImage(const NativeImagePtr& image, GraphicsContext& ctxt, const FloatRect& destRect, const FloatRect& srcRect, const IntSize& srcSize, const ImagePaintingOptions& options)
{
    // QTFIXME: Handle imageSize? See e.g. NativeImageDirect2D
    ctxt.platformContext()->setRenderHint(QPainter::SmoothPixmapTransform, true);

    QRectF normalizedDst = destRect.normalized();
    QRectF normalizedSrc = srcRect.normalized();

    QImage prescaledImage;
    const NativeImagePtr& maybePrescaledImage = *prescaleImageIfRequired(ctxt.platformContext(), &image, &prescaledImage, normalizedDst, &normalizedSrc);

    CompositeOperator previousOperator = ctxt.compositeOperation();
    BlendMode previousBlendMode = ctxt.blendModeOperation();
    ctxt.setCompositeOperation(!image.hasAlphaChannel() && options.compositeOperator() == CompositeSourceOver && options.blendMode() == BlendMode::Normal ? CompositeCopy : options.compositeOperator(), options.blendMode());

    if (ctxt.hasShadow() && ctxt.mustUseShadowBlur()) {
        ShadowBlur shadow(ctxt.state());
        shadow.drawShadowLayer(ctxt.getCTM(), ctxt.clipBounds(), normalizedDst,
            [normalizedSrc, normalizedDst, &image](GraphicsContext& shadowContext)
            {
                QPainter* shadowPainter = shadowContext.platformContext();
                shadowPainter->drawImage(normalizedDst, image, normalizedSrc);
            },
            [&ctxt](ImageBuffer& layerImage, const FloatPoint& layerOrigin, const FloatSize& layerSize)
            {
                ctxt.drawImageBuffer(layerImage, FloatRect(roundedIntPoint(layerOrigin), layerSize), FloatRect(FloatPoint(), layerSize), ctxt.compositeOperation());
            });
    }

    ctxt.platformContext()->drawImage(normalizedDst, maybePrescaledImage, normalizedSrc);

    ctxt.setCompositeOperation(previousOperator, previousBlendMode);
}

void clearNativeImageSubimages(const NativeImagePtr&)
{
}

} // namespace WebCore
