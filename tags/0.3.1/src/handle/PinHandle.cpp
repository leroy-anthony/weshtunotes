/*
    Copyright (c) 2009 LEROY Anthony <leroy.anthony@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "PinHandle.h"

#include <QApplication>
#include <QPainter>
#include <QPen>

#include "HandleItem.h"
#include "../config/ImageFactory.h"
#include "../config/VisualAspect.h"

namespace Handle
{

    PinHandle::PinHandle( HandleItem * parent ):
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

    void PinHandle::setDefaultColor( const QColor & c )
    {
        m_defaultColor = QColor(c);
        setDefaultColor();
    }

    void PinHandle::setDefaultColor()
    {
        //setStyleSheet( Config::VisualAspect::gradiantBackground( m_defaultColor ) );
    }

    void PinHandle::setHoverMode( bool isHover )
    {
        setVisible( isHover );
        m_isHover = isHover;
    }

    void PinHandle::paintEvent( QPaintEvent * event )
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

        painter.setPen(Qt::white);
        painter.setBrush(QBrush(Qt::white));
        painter.drawEllipse( QPointF(width()/2.0, height()/2.0), width()/2-2, height()/2-2 );

    }

    void PinHandle::mouseReleaseEvent ( QMouseEvent * event )
    {
        Q_UNUSED( event );

        m_handleItemRoot->setHoverMode(false);
        //m_handleItemRoot->setDisabled(true);
        m_handleItemRoot->setPin(true);
    }

}
