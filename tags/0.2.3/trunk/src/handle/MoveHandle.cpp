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

#include "MoveHandle.h"

#include <QMouseEvent>
#include <QApplication>
#include <QObject>
#include <QRadialGradient>
#include <QPainter>

#include "HandleItem.h"
#include "../config/VisualAspect.h"

namespace Handle
{
    
    MoveHandle::MoveHandle( HandleItem * parent ):
            QWidget(parent),
            m_oldCursorX(-1),
            m_oldCursorY(-1),
            m_isHover(false)
    {
        setContentsMargins(0,0,0,0);
        setFixedWidth(Config::VisualAspect::widthHandleControl);
        setDefaultColor(parent->defaultColor());
    }
    
    MoveHandle::~MoveHandle()
    {
    }
    
    void MoveHandle::setDefaultColor( const QColor & c )
    {
        m_defaultColor = QColor(c);
        setDefaultColor();
    }
    
    void MoveHandle::setDefaultColor()
    {
        setStyleSheet( Config::VisualAspect::gradiantBackground( m_defaultColor ) );
    }
    
    void MoveHandle::setSelectionColor()
    {
        setStyleSheet( QString("background: %1;").arg(QApplication::palette().color(QPalette::Highlight).name()) );
    }
    
    void MoveHandle::mouseReleaseEvent ( QMouseEvent * event )
    {
        Q_UNUSED( event );
        m_mode = Nothing;

        QWidget::mouseReleaseEvent(event);
    }
    
    void MoveHandle::mousePressEvent ( QMouseEvent * event )
    {
        m_mode = Move;
        
        m_decalageMoveX = event->pos().x();
        m_decalageMoveY = event->pos().y();
    }
    
    void MoveHandle::enterEvent( QEvent * event )
    {
        Q_UNUSED( event );
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
    
    void MoveHandle::drawHandle( QPainter & painter, HandleItem * h, int & x, int & y )
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
            const QList<HandleItem*> & handles = h->children();
            for ( int i=0 ; i<handles.size() ; ++i )
            {
                drawHandle(painter,handles[i],x,y);
            }
        }
    }

    void MoveHandle::drawGrip( QPainter & painter )
    {
        int xGrips             = 2;
        int marginedHeight = (height() * 80 / 100); // 10% empty on top, and 10% empty on bottom, so 20% of the height should be empty of any grip, and 80% should be in the grips
        int nbGrips            = (marginedHeight - 3) / 6;
        if (nbGrips < 2)
            nbGrips = 2;
        int yGrips             = (height() + 1 - nbGrips * 6 - 3) / 2; // +1 to avoid rounding errors, -nbGrips*6-3 the size of the grips
        QColor darker  = QApplication::palette().color(QPalette::Highlight).dark(130);
        QColor lighter = QApplication::palette().color(QPalette::Highlight).light(130);

        painter.setPen(Qt::NoPen);

        for (int i = 0; i < nbGrips; ++i)
        {
            QRadialGradient radialGrad(QPointF(xGrips, yGrips), 2);
            radialGrad.setColorAt(0, darker);
            radialGrad.setColorAt(1, lighter);
            painter.setBrush(radialGrad);

            painter.drawEllipse( xGrips, yGrips, 2, 2 );

            radialGrad = QRadialGradient(QPointF(xGrips + 3, yGrips + 3), 2);
            radialGrad.setColorAt(0, darker);
            radialGrad.setColorAt(1, lighter);
            painter.setBrush(radialGrad);

            painter.drawEllipse( xGrips + 3, yGrips + 3, 2, 2 );

            yGrips += 6;
        }
    }

    void MoveHandle::paintEvent( QPaintEvent * event )
    {
        QPainter painter(this);
        painter.setPen(Qt::NoPen);

        if ( m_isHover )
        {
            QLinearGradient gradient(0,0,0, height());
            gradient.setColorAt(0, QApplication::palette().color(QPalette::Highlight).lighter(Config::VisualAspect::lighterIntensity));
            gradient.setColorAt(1, QApplication::palette().color(QPalette::Highlight));
            painter.setBrush( gradient );

            painter.drawRect( 0, 0, width(), height() );
        }
        else
        {
            HandleItem * h = dynamic_cast<HandleItem*>(parentWidget());
            const QList<HandleItem*> & handles = h->children();

            int x = 0;
            int y = 0;
            for ( int i=0 ; i<handles.size() ; ++i )
            {
                drawHandle( painter, handles[i], x, y );
            }
        }

        if ( m_isHover )
        {
            drawGrip( painter );
        }
    }
}
