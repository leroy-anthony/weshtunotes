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
            m_handleItemRoot(parent)
    {
        QVBoxLayout * l = new QVBoxLayout( this );

        l->addWidget(new QLabel(" "));
        l->setContentsMargins( 0, 0, 0, 0 );
        l->setSpacing( 0 );

        setFixedWidth(9);
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
        setSelectionColor();
        setCursor(Qt::SizeHorCursor);
    }

    void SizeHorHandle::leaveEvent( QEvent * event )
    {
        setDefaultColor();
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
            parentWidget()->resize(parentWidget()->size()+QSize(event->x(),0));
        }
    }

    void SizeHorHandle::setHoverMode( bool isHover )
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

