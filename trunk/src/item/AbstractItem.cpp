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

#include "settings.h"
#include "../tag/NoteTag.h"
#include "../config/VisualAspect.h"

namespace Item
{

    AbstractItem::~AbstractItem()
    {
    }

    AbstractItem::AbstractItem( QWidget * parent ):
            QWidget(parent),
            GeneratorID("item")
    {
        m_color = Settings::colorItem();
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

    void AbstractItem::setVisibleAddTag( bool visible )
    {
        m_addTag->setVisible(visible);
    }

    void AbstractItem::adaptSize()
    {
    }

    void AbstractItem::setItemColor( const QColor & color )
    {
        setStyleSheet( Config::VisualAspect::gradiantBackground( color ) );

        m_color = QColor(color);
        emit colorChange();
    }

    const QColor & AbstractItem::itemColor()
    {
        return m_color;
    }

    void AbstractItem::keyPressEvent ( QKeyEvent * keyEvent )
    {
        QWidget::keyPressEvent(keyEvent);
    }

    bool AbstractItem::containTag( const QString & tagName )
    {
        for ( int i=0 ; i<m_tags.size() ; ++i )
        {
            if ( m_tags[i]->name() == tagName )
            {
                return true;
            }
        }

        return false;
    }

     const QList<Tag::NoteTag*> & AbstractItem::tags()
     {
         return m_tags;
     }

     void AbstractItem::insertData( const QMimeData * data )
     {
     }

}
