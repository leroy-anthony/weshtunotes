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

#include "SizeHorHandle.h"

#include <QDebug>
#include <QMouseEvent>
#include <QLabel>
#include <QApplication>

#include "HandleItem.h"
#include "../config/VisualAspect.h"

namespace Handle
{

    SizeHorHandle::SizeHorHandle( HandleItem * parent ):
            QWidget(parent),
            m_handleItemRoot(parent),
            m_isHover(false)
    {
        setFixedWidth(Config::VisualAspect::widthHandleControl);
        setDefaultColor(parent->defaultColor());
    }

    SizeHorHandle::~SizeHorHandle()
    {
    }

    void SizeHorHandle::setDefaultColor( const QColor & c )
    {
        m_defaultColor = QColor(c);
        setDefaultColor();
    }

    void SizeHorHandle::setDefaultColor()
    {
        setStyleSheet( Config::VisualAspect::gradiantBackground( m_defaultColor ) );
    }

    void SizeHorHandle::setSelectionColor()
    {
        setStyleSheet( QString("background: %1;").arg(QApplication::palette().color(QPalette::Highlight).name()) );
    }

    void SizeHorHandle::enterEvent( QEvent * event )
    {
        Q_UNUSED( event );

        setCursor(Qt::SizeHorCursor);
    }

    void SizeHorHandle::mouseReleaseEvent ( QMouseEvent * event )
    {
        Q_UNUSED( event );

        m_mode = Nothing;
    }

    void SizeHorHandle::mousePressEvent ( QMouseEvent * event )
    {
        Q_UNUSED( event );

        if ( m_handleItemRoot->isRoot() )
        {
            m_mode = ScaleXItem;
        }
    }

    void SizeHorHandle::mouseMoveEvent ( QMouseEvent * event )
    {
        if ( m_mode == ScaleXItem )
        {
            m_handleItemRoot->resize(parentWidget()->size()+QSize(event->x(),0));
        }
    }

    void SizeHorHandle::setHoverMode( bool isHover )
    {
        m_isHover = isHover;
        update();
    }

    void SizeHorHandle::drawHandle( QPainter & painter, HandleItem * h, int & x, int & y )
    {
        if ( h->children().size() ==0 )
        {
            QLinearGradient gradient( 0, y, 0,h->height()+y);
            gradient.setColorAt( 0, h->defaultColor().lighter( Config::VisualAspect::lighterIntensity ) );
            gradient.setColorAt( 1, h->defaultColor() );
            painter.setBrush( gradient );
            painter.drawRect( x, y, width(), h->height() );
            y += h->height();
        }
        else
        {
            QList<HandleItem*> handles = h->children();
            for ( int i=0 ; i<handles.size() ; ++i )
            {
                drawHandle(painter,handles[i],x,y);
            }
        }
    }

    void SizeHorHandle::paintEvent( QPaintEvent * event )
    {
        Q_UNUSED( event );

        QPainter painter(this);

        QPen pen;
        pen.setStyle(Qt::NoPen);
        pen.setWidth(1);
        painter.setPen(pen);

        HandleItem * h = dynamic_cast<HandleItem*>(parentWidget());
        QList<HandleItem*> handles = h->children();

        if ( m_isHover )
        {
            QLinearGradient gradient(0,0,0, height());
            gradient.setColorAt(0, QApplication::palette().color(QPalette::Highlight).lighter( Config::VisualAspect::lighterIntensity ));
            gradient.setColorAt(1, QApplication::palette().color(QPalette::Highlight));

            painter.setBrush(gradient);

            painter.drawRect( 0, 0, width(), height() );
        }
        else
        {
            if ( handles.size() > 0 )
            {
                HandleItem * h = dynamic_cast<HandleItem*>(parentWidget());
                QList<HandleItem*> handles = h->children();

                int x = 0;
                int y = 0;
                for ( int i=0 ; i<handles.size() ; ++i )
                {
                    drawHandle( painter, handles[i], x, y );
                }
            }
            else
            {
                QLinearGradient gradient( 0, 0, 0, height() );
                gradient.setColorAt( 0, h->defaultColor().lighter( Config::VisualAspect::lighterIntensity ) );
                gradient.setColorAt( 1, h->defaultColor() );
                painter.setBrush( gradient );
                painter.drawRect( 0, 0, width(), height() );
            }
        }
    }
}

