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

#include "MoveHandle.h"

#include <QMouseEvent>
#include <QApplication>
#include <QObject>

#include "HandleItem.h"

namespace Handle
{
    
    MoveHandle::MoveHandle( HandleItem * parent ):
            QWidget(parent),
            m_oldCursorX(-1),
            m_oldCursorY(-1),
            m_isHover(false)
    {
        setFixedWidth(10);
        setDefaultColor(parent->defaultColor());
    }
    
    MoveHandle::~MoveHandle()
    {
    }
    
    void MoveHandle::setDefaultColor( const QColor & c )
    {
        //QColor c2(c);
        //c2.setAlpha(0.6);
        //setStyleSheet(QString("background: qlineargradient(x1:0.5, y1:0, x2:0.5, y2:1, stop:0 %1, stop:1 %2)").arg(c.name()).arg(c2.name()));
        m_defaultColor = QColor(c);
        setDefaultColor();
    }
    
    void MoveHandle::setDefaultColor()
    {
        setStyleSheet( QString("background: %1;").arg(m_defaultColor.name()) );
        setStyleSheet( QString("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 %1, stop:1 %2)")
                       .arg(m_defaultColor.lighter(150).name())
                       .arg(m_defaultColor.name()));
    }
    
    void MoveHandle::setSelectionColor()
    {
        setStyleSheet( QString("background: %1;").arg(palette().color(QPalette::Highlight).name()) );
    }
    
    void MoveHandle::mouseReleaseEvent ( QMouseEvent * event )
    {
        m_mode = Nothing;
    }
    
    void MoveHandle::mousePressEvent ( QMouseEvent * event )
    {
        m_mode = Move;
        
        m_decalageMoveX = event->pos().x();
        m_decalageMoveY = event->pos().y();
    }
    
    void MoveHandle::enterEvent( QEvent * event )
    {
        setCursor(Qt::OpenHandCursor);
    }
    
    void MoveHandle::mouseMoveEvent ( QMouseEvent * event )
    {
        if ( m_mode == Move )
        {
            HandleItem * handle = static_cast<HandleItem*>( parentWidget() );
            
            int x = parentWidget()->pos().x();
            int y = parentWidget()->pos().y();
            
            if ( handle->modeDegroupement() )
            {
                if ( m_oldCursorX != -1 )
                {
                    y -= m_oldCursorY - event->globalY();
                    x -= m_oldCursorX - event->globalX();
                }
                
                m_oldCursorY = event->globalY();
                m_oldCursorX = event->globalX();
            }
            else
            {
                m_oldCursorY = -1;
                m_oldCursorX = -1;
                
                x = event->globalX() - m_decalageMoveX;
                y = event->globalY() - m_decalageMoveY;
            }
            
            handle->moveItem( x, y );
        }
    }
    
    void MoveHandle::setHoverMode( bool isHover )
    {
        m_isHover = isHover;
        update();
    }
    
    void MoveHandle::drawHandle( QPainter & painter, HandleItem * h, int & x, int & y, QRect & r, int & decal )
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

    void MoveHandle::drawGrip( QPainter & painter, QRect & r )
    {
        int xGrips             = 2;
        int marginedHeight = (r.height() * 80 / 100); // 10% empty on top, and 10% empty on bottom, so 20% of the height should be empty of any grip, and 80% should be in the grips
        int nbGrips            = (marginedHeight - 3) / 6;
        if (nbGrips < 2)
            nbGrips = 2;
        int yGrips             = (r.height() + 1 - nbGrips * 6 - 3) / 2; // +1 to avoid rounding errors, -nbGrips*6-3 the size of the grips
        QColor darker  = palette().color(QPalette::Highlight).dark(130);
        QColor lighter = palette().color(QPalette::Highlight).light(130);

        QPen p;
        p.setWidth(1);

        for (int i = 0; i < nbGrips; ++i)
        {
            /// Dark color:
            p.setColor(darker);
            painter.setPen(p);
            // Top-left point:
            painter.drawPoint(xGrips,     yGrips);
            painter.drawPoint(xGrips + 1, yGrips);
            painter.drawPoint(xGrips,     yGrips + 1);
            // Bottom-right point:
            painter.drawPoint(xGrips + 4, yGrips + 3);
            painter.drawPoint(xGrips + 5, yGrips + 3);
            painter.drawPoint(xGrips + 4, yGrips + 4);
            /// Light color:
            p.setColor(lighter);
            // Top-left point:
            painter.drawPoint(xGrips + 1, yGrips + 1);
            // Bottom-right point:
            painter.drawPoint(xGrips + 5, yGrips + 4);
            yGrips += 6;
        }
    }

    void MoveHandle::paintEvent( QPaintEvent * event )
    {
        QRect r = event->rect();
        QPainter painter(this);
        
        QPen pen;//palette().color(QPalette::Highlight));
        pen.setStyle(Qt::NoPen);
        pen.setWidth(1);
        painter.setPen(pen);

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

        if ( m_isHover )
        {
            drawGrip( painter, r );
        }
    }
}
