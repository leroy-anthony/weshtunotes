/*
 Copyright (c) 2009 LEROY Anthony <leroy.anthony@gmail.com>

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
 */

#include "DeleteHandle.h"

#include <QApplication>
#include <QPainter>
#include <QPen>

#include "HandleItem.h"
#include "../config/ImageFactory.h"
#include "../config/VisualAspect.h"

namespace Handle
{

    DeleteHandle::DeleteHandle( HandleItem * parent ):
            QPushButton(parent),
            m_handleItemRoot(parent),
            m_isHover(false)
    {
        setFixedWidth(Config::VisualAspect::widthHandleControl);
        setFixedHeight(Config::VisualAspect::widthHandleControl);
        setContentsMargins(0,0,0,0);
        setDefaultColor(parent->defaultColor());
        setFlat(true);
        hide();
    }

    void DeleteHandle::setDefaultColor( const QColor & c )
    {
        m_defaultColor = QColor(c);
        setDefaultColor();
    }

    void DeleteHandle::setDefaultColor()
    {
        //setStyleSheet( Config::VisualAspect::gradiantBackground( m_defaultColor ) );
    }

    void DeleteHandle::setHoverMode( bool isHover )
    {
        setVisible( isHover );
        m_isHover = isHover;
    }

    void DeleteHandle::paintEvent( QPaintEvent * event )
    {
        Q_UNUSED( event );

        QPainter painter(this);

        QPen pen;
        pen.setStyle(Qt::NoPen);
        pen.setWidth(1);
        painter.setPen(pen);

        QLinearGradient gradient(0,0,0, height());
        gradient.setColorAt(0, QColor(150,13,16).lighter(Config::VisualAspect::lighterIntensity));
        gradient.setColorAt(1, QColor(150,13,16));
        painter.setBrush(gradient);
        painter.drawRect( 0, 0, width(), height() );

        QPen pen2;
        pen2.setWidth(1);
        pen2.setColor(Qt::white);
        pen2.setCapStyle(Qt::RoundCap);

        painter.setPen(pen2);

        painter.drawLine( 2, 2, width()-2, height()-2 );
        painter.drawLine( 2, height()-2, width()-2, 2 );

    }

}
