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
            m_oldCursorY(-1)
    {
        setFixedWidth(9);
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
        if ( isHover )
        {
            setSelectionColor();
        }
        else
        {
            setDefaultColor();
        }
    }

}
