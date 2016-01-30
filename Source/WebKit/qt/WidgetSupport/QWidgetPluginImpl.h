/*
 * Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
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
#ifndef QWidgetPluginImpl_h
#define QWidgetPluginImpl_h

#include "QtPluginWidgetAdapter.h"

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE

class QWidgetPluginImpl : public QtPluginWidgetAdapter {
    Q_OBJECT
public:
    QWidgetPluginImpl(QWidget *w) : m_widget(w) { }
    virtual ~QWidgetPluginImpl();
    virtual void update(const QRect &) override;
    virtual void setGeometryAndClip(const QRect&, const QRect&, bool isVisible) override;
    virtual void setVisible(bool) override;
    virtual void setStyleSheet(const QString&) override;
    virtual void setWidgetParent(QObject *) override;
    virtual QObject* handle() const override;
private:
    QWidget *m_widget;
};

#endif // QWidgetPluginImpl_h
