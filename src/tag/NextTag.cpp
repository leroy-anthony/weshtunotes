/*
 Copyright (c) 2010 LEROY Anthony <leroy.anthony@gmail.com>

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

#include "NextTag.h"

#include <QDebug>

#include "../tag/NoteTag.h"
#include "../item/NoteItem.h"

namespace Tag
{

    NextTag::NextTag( Item::AbstractItem * noteItem ):
            QPushButton( noteItem ),
            m_abstractItem( noteItem )
    {
        setIcon(Config::ImageFactory::newInstance()->icon("go-next.png"));
        setIconSize(QSize(12,12));
        setContentsMargins(0,0,0,0);
        hide();

        setStyleSheet(QString("background-color: %1").arg(Qt::transparent));
    }

    void NextTag::mousePressEvent( QMouseEvent * e )
    {
        Q_UNUSED( e );

        NoteTag * tag = m_abstractItem->tag();
        if ( tag != 0 )
        {
            tag->nextState();
        }

        QPushButton::mousePressEvent(e);
    }

    void NextTag::paintEvent( QPaintEvent * event )
    {
        QPushButton::paintEvent(event);
    }

}
