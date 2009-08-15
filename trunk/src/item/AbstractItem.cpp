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

#include "AbstractItem.h"

#include "../tag/NoteTag.h"

namespace Item
{
    int AbstractItem::m_id = 0;

    AbstractItem::~AbstractItem()
    {
    }

    AbstractItem::AbstractItem( QWidget * parent ):
            QWidget(parent)
    {
        m_itemId = QString("item%1").arg(m_id);
        ++m_id;
    }

    const QList<QString> AbstractItem::operationInterfaces()
    {
        return m_operationInterfaces;
    }

    const QColor & AbstractItem::color()
    {
        return m_color;
    }

    void AbstractItem::isSelected()
    {
    }

    void AbstractItem::setItemId( const QString & id )
    {
        m_itemId = id;
        QString idStr = QString(id).replace("item","");
        if ( idStr.toInt() > m_id )
        {
            m_id = idStr.toInt() + 1;
        }
    }

    void AbstractItem::setVisibleTag( bool visible )
    {
        m_tag->setVisibleTag(visible);
    }

    void AbstractItem::adaptSize()
    {
    }

}
