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

#include "HandleItem.h"

namespace Handle
{

    SizeHorHandle::SizeHorHandle( HandleItem * parent ):
            QWidget(parent),
            m_handleItemRoot(parent),
            m_isHover(false)
    {

        setFixedWidth(10);
        setDefaultColor(parent->defaultColor());
    }

    SizeHorHandle::~SizeHorHandle()
    {
    }

    void SizeHorHandle::setDefaultColor( const QColor & c )
    {
        setStyleSheet( QString("background: %1;").arg(c.name()) );
        m_defaultColor = QColor(c);
    }

    void SizeHorHandle::setDefaultColor()
    {
        setStyleSheet( QString("background: %1;").arg(m_defaultColor.name()) );
    }

    void SizeHorHandle::setSelectionColor()
    {
        setStyleSheet( QString("background: %1;").arg(palette().color(QPalette::Highlight).name()) );
    }

    void SizeHorHandle::enterEvent( QEvent * event )
    {
        setCursor(Qt::SizeHorCursor);
    }

    void SizeHorHandle::mouseReleaseEvent ( QMouseEvent * event )
    {
        m_mode = Nothing;
    }

    void SizeHorHandle::mousePressEvent ( QMouseEvent * event )
    {
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

    void SizeHorHandle::drawHandle( QPainter & painter, HandleItem * h, int & x, int & y, QRect & r, int & decal  )
    {
        if ( h->children().size() ==0 )
        {
            QLinearGradient gradient( 0, y-decal, 0,h->height()+y-decal);
            gradient.setColorAt( 0, h->defaultColor().lighter(150) );
            gradient.setColorAt( 1, h->defaultColor() );
            painter.setBrush( gradient );
            painter.drawRect( x, y, r.width(), h->height()-decal );
            y += h->height()-decal;
            decal=0;
        }
        else
        {
            QList<HandleItem*> handles = h->children();
            for ( int i=0 ; i<handles.size() ; ++i )
            {
                drawHandle(painter,handles[i],x,y,r,decal);
            }
        }
    }

    void SizeHorHandle::paintEvent( QPaintEvent * event )
    {
        QRect r = event->rect();
        QPainter painter(this);

        QPen pen;
        pen.setStyle(Qt::NoPen);
        painter.setPen(pen);

        HandleItem * h = dynamic_cast<HandleItem*>(parentWidget());
        QList<HandleItem*> handles = h->children();

        if ( m_isHover )
        {
            QLinearGradient gradient(0,0,0, height());
            gradient.setColorAt(0, palette().color(QPalette::Highlight).lighter(150));
            gradient.setColorAt(1, palette().color(QPalette::Highlight));

            painter.setBrush(gradient);

            painter.drawRect( r.x(), r.y(), r.width(), r.height() );
        }
        else
        {
            if ( handles.size() > 0 )
            {
                HandleItem * h = dynamic_cast<HandleItem*>(parentWidget());
                QList<HandleItem*> handles = h->children();
                int x = r.x();
                int y = r.y();

                int decal = 0;
                if ( y != 0 )
                {
                    decal = h->height() - r.height() - (h->parentHandle() == 0 ? h->contentMarginX()+h->contentMarginY() : 0);
                }
                for ( int i=0 ; i<handles.size() ; ++i )
                {
                    drawHandle( painter, handles[i], x, y, r, decal );
                }
            }
            else
            {
                QLinearGradient gradient( 0, 0, 0, r.height() + (h->height() - r.height()) );
                gradient.setColorAt( 0, h->defaultColor().lighter(150) );
                gradient.setColorAt( 1, h->defaultColor() );
                painter.setBrush( gradient );
                painter.drawRect( r.x(), r.y(), r.width(), r.height() );
            }
        }
    }
}

