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

#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include <QString>

namespace Item
{
    class AbstractItem;

    class ItemFactory
    {
    public:
        enum Type { NOTEITEM, MAX };

        ItemFactory();

        static AbstractItem * newItem( const QString & id );
        static AbstractItem * newItem( const QString & id, ItemFactory::Type type );

        static QString typeName( ItemFactory::Type type );
        static ItemFactory::Type type( const QString & typeName );

    private:
        static AbstractItem * newItemInterne( ItemFactory::Type type, const QString & id );

    };

}

#endif // ITEMFACTORY_H
