/*
    Copyright (c) 2011 LEROY Anthony <leroy.anthony@gmail.com>

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

#include "ItemFactory.h"


#include "../data/DataManager.h"
#include "../item/NoteItem.h"

#include <QDebug>

namespace Item
{

    ItemFactory::ItemFactory()
    {
    }

    AbstractItem * ItemFactory::newItem( const QString & id )
    {
        QString filename = Data::DataManager::configFileItem(id);
        Data::DataManager settings( filename );

        return newItemInterne( type(settings.valueGroup("general", "type", typeName(NOTEITEM))), id );
    }

    AbstractItem * ItemFactory::newItem( const QString & id, ItemFactory::Type type )
    {
        return newItemInterne( type, id );
    }

    ItemFactory::Type ItemFactory::type( const QString & typeName )
    {
        if ( typeName == "noteitem" )
        {
            return NOTEITEM;
        }

        return NOTEITEM;
    }

    QString ItemFactory::typeName( ItemFactory::Type type )
    {
        if ( type == NOTEITEM )
        {
            return "noteitem";
        }

        return "noteitem";
    }

    AbstractItem * ItemFactory::newItemInterne( ItemFactory::Type type, const QString & id )
    {
        AbstractItem * item = 0;

        if ( type == NOTEITEM )
        {
            item = new NoteItem();
        }

        item->setId( id );

        return item;
    }

}
