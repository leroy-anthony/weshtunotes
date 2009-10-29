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

#include "AddTag.h"

#include "../tag/TagFactory.h"
#include "../item/NoteItem.h"

namespace Tag
{

    AddTag::AddTag( Item::NoteItem * noteItem ):
            QPushButton(),
            m_noteItem( noteItem )
    {
        setIcon(Config::ImageFactory::icon(Config::Image::addBasket));
        setIconSize(QSize(12,12));
        setFixedWidth(12);
        setFixedHeight(12);
        setContentsMargins(0,0,0,0);
        setFlat(true);
        hide();
    }

    void AddTag::mousePressEvent( QMouseEvent * e )
    {
        Q_UNUSED( e );

        if ( TagFactory::tagsNames().size() > 0 )
        {
            loadTagsMenu();
            m_menu.popup(QCursor::pos());
        }
        else
        {
            Tag::TagFactory::newTagFactory()->show();
        }
    }

    void AddTag::loadTagsMenu()
    {
        m_menu.clear();
        QStringList tags = TagFactory::tagsNames();
        for ( int i=0 ; i<tags.size() ; ++i )
        {
            QAction * action = new QAction(0);
            action->setText(tags[i]);
            action->setCheckable(true);
            action->setChecked( m_noteItem->containTag(tags[i]) );
            m_menu.addAction(action);
        }
        if ( m_noteItem != 0 )
        {
            connect(&m_menu, SIGNAL(triggered(QAction*)), m_noteItem, SLOT(tagApply(QAction*)));
        }
    }

}
