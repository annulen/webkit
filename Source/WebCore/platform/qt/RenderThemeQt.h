/*
 * This file is part of the theme implementation for form controls in WebCore.
 *
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */
#ifndef RenderThemeQt_h
#define RenderThemeQt_h

#include "RenderTheme.h"

#include <QBrush>
#include <QPalette>
#include <QSharedPointer>
#include <QString>

QT_BEGIN_NAMESPACE
class QPainter;
QT_END_NAMESPACE

namespace WebCore {

#if ENABLE(PROGRESS_ELEMENT)
class RenderProgress;
#endif
class RenderStyle;
class HTMLMediaElement;
class StylePainter;
class ScrollbarTheme;

typedef PassRefPtr<RenderTheme> (*QtThemeFactoryFunction)(Page* page);

class RenderThemeQt : public RenderTheme {

public:
    RenderThemeQt(Page*);

    static void setCustomTheme(QtThemeFactoryFunction, ScrollbarTheme* customScrollbarTheme);
    static ScrollbarTheme* customScrollbarTheme();

    String extraDefaultStyleSheet() override;

    virtual bool supportsHover(const RenderStyle&) const override;
    virtual bool supportsFocusRing(const RenderStyle&) const override;

    virtual int baselinePosition(const RenderBox&) const override;

    // A method asking if the control changes its tint when the window has focus or not.
    virtual bool controlSupportsTints(const RenderObject&) const override;

    // A general method asking if any control tinting is supported at all.
    virtual bool supportsControlTints() const override;

    virtual void adjustRepaintRect(const RenderObject&, FloatRect&) override;

    // The platform selection color.
    virtual Color platformActiveSelectionBackgroundColor() const override;
    virtual Color platformInactiveSelectionBackgroundColor() const override;
    virtual Color platformActiveSelectionForegroundColor() const override;
    virtual Color platformInactiveSelectionForegroundColor() const override;

    virtual Color platformFocusRingColor() const override;

    virtual Color systemColor(CSSValueID) const override;

    virtual int minimumMenuListSize(RenderStyle&) const override;

    virtual void adjustSliderThumbSize(RenderStyle&, Element*) const override;

#if ENABLE(DATALIST_ELEMENT)
    virtual IntSize sliderTickSize() const override;
    virtual int sliderTickOffsetFromTrackCenter() const override;
#endif

    virtual double caretBlinkInterval() const override;

    virtual bool isControlStyled(const RenderStyle&, const BorderData&, const FillLayer&, const Color&) const override;

#if 0 //ENABLE(VIDEO)
    virtual String extraMediaControlsStyleSheet();
#endif
#if ENABLE(VIDEO)
    virtual String mediaControlsStyleSheet() override;
    virtual String mediaControlsScript() override;
#endif

protected:
    virtual void updateCachedSystemFontDescription(CSSValueID systemFontID, FontCascadeDescription&) const override;

    virtual bool paintCheckbox(const RenderObject&, const PaintInfo&, const IntRect&) override;
    virtual void setCheckboxSize(RenderStyle&) const override;

    virtual bool paintRadio(const RenderObject&, const PaintInfo&, const IntRect&) override;
    virtual void setRadioSize(RenderStyle&) const override;

    virtual void setButtonSize(RenderStyle&) const override;

    virtual void adjustTextFieldStyle(StyleResolver&, RenderStyle&, Element*) const override;

    virtual bool paintTextArea(const RenderObject&, const PaintInfo&, const FloatRect&) override;
    virtual void adjustTextAreaStyle(StyleResolver&, RenderStyle&, Element*) const override;

    virtual void adjustMenuListStyle(StyleResolver&, RenderStyle&, Element*) const override;

    virtual void adjustMenuListButtonStyle(StyleResolver&, RenderStyle&, Element*) const override;

#if ENABLE(PROGRESS_ELEMENT)
    virtual void adjustProgressBarStyle(StyleResolver&, RenderStyle&, Element*) const override;
    // Returns the repeat interval of the animation for the progress bar.
    virtual double animationRepeatIntervalForProgressBar(RenderProgress*) const override;
#endif

    virtual void adjustSliderTrackStyle(StyleResolver&, RenderStyle&, Element*) const override;

    virtual void adjustSliderThumbStyle(StyleResolver&, RenderStyle&, Element*) const override;

    virtual bool paintSearchField(const RenderObject&, const PaintInfo&, const IntRect&) override;
    virtual void adjustSearchFieldStyle(StyleResolver&, RenderStyle&, Element*) const override;

    virtual void adjustSearchFieldCancelButtonStyle(StyleResolver&, RenderStyle&, Element*) const override;
    virtual bool paintSearchFieldCancelButton(const RenderBox&, const PaintInfo&, const IntRect&) override;

    virtual void adjustSearchFieldDecorationPartStyle(StyleResolver&, RenderStyle&, Element*) const override;
    virtual bool paintSearchFieldDecorationPart(const RenderObject&, const PaintInfo&, const IntRect&) override;

    virtual void adjustSearchFieldResultsDecorationPartStyle(StyleResolver&, RenderStyle&, Element*) const override;
    virtual bool paintSearchFieldResultsDecorationPart(const RenderBox&, const PaintInfo&, const IntRect&) override;

#ifndef QT_NO_SPINBOX
    virtual void adjustInnerSpinButtonStyle(StyleResolver&, RenderStyle&, Element*) const override;
#endif

#if 0 //ENABLE(VIDEO)
    virtual bool paintMediaFullscreenButton(const RenderObject&, const PaintInfo&, const IntRect&);
    virtual bool paintMediaPlayButton(const RenderObject&, const PaintInfo&, const IntRect&);
    virtual bool paintMediaMuteButton(const RenderObject&, const PaintInfo&, const IntRect&);
    virtual bool paintMediaSeekBackButton(const RenderObject&, const PaintInfo&, const IntRect&);
    virtual bool paintMediaSeekForwardButton(const RenderObject&, const PaintInfo&, const IntRect&);
    virtual bool paintMediaSliderTrack(const RenderObject&, const PaintInfo&, const IntRect&);
    virtual bool paintMediaSliderThumb(const RenderObject&, const PaintInfo&, const IntRect&);
    virtual bool paintMediaCurrentTime(const RenderObject&, const PaintInfo&, const IntRect&);
    virtual bool paintMediaVolumeSliderTrack(const RenderObject&, const PaintInfo&, const IntRect&);
    virtual bool paintMediaVolumeSliderThumb(const RenderObject&, const PaintInfo&, const IntRect&);
    virtual String formatMediaControlsCurrentTime(float currentTime, float duration) const override;
    virtual String formatMediaControlsRemainingTime(float currentTime, float duration) const override;
    virtual bool hasOwnDisabledStateHandlingFor(ControlPart) const { return true; }

    void paintMediaBackground(QPainter*, const IntRect&) const;
    double mediaControlsBaselineOpacity() const;
    QColor getMediaControlForegroundColor(const RenderObject& = 0) const;
#endif
    virtual void computeSizeBasedOnStyle(RenderStyle&) const = 0;

    virtual String fileListNameForWidth(const FileList*, const FontCascade&, int width, bool multipleFilesAllowed) const override;

    virtual QRect inflateButtonRect(const QRect& originalRect) const;
    virtual QRectF inflateButtonRect(const QRectF& originalRect) const;

    virtual void setPopupPadding(RenderStyle&) const = 0;

    virtual QSharedPointer<StylePainter> getStylePainter(const PaintInfo&) = 0;

    bool supportsFocus(ControlPart) const;

//    IntRect convertToPaintingRect(const RenderObject& inputRenderer, const RenderObject& partRenderer, IntRect partRect, const IntRect& localOffset) const;

    virtual QPalette colorPalette() const;

    Page* m_page;

    QString m_buttonFontFamily;

};

class StylePainter {
public:
    virtual ~StylePainter();

    bool isValid() const { return painter; }

    QPainter* painter;

protected:
    StylePainter(GraphicsContext&);

private:
    QBrush m_previousBrush;
    bool m_previousAntialiasing;

};

}

#endif // RenderThemeQt_h
