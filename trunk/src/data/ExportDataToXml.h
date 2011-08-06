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

#ifndef EXPORTDATATOXML_H
#define EXPORTDATATOXML_H

#include <QString>
#include <QXmlStreamWriter>

namespace Explorer
{
    class ItemTreeBasket;
}

namespace Scene
{
    class AbstractScene;
}

namespace Handle
{
    class HandleItem;
}

namespace Item
{
    class AbstractItem;
}

namespace Data
{

    class ExportDataToXml
    {
    public:

        static void exportHandle( Item::AbstractItem * abstractItem, QXmlStreamWriter & xmlWriter );
        static void exportHandle( Handle::HandleItem * handleItem, QXmlStreamWriter & xmlWriter );
        static void exportConfigFile( const QString & pathFile, QXmlStreamWriter & xmlWriter );
        static void exportScene( Scene::AbstractScene * scene, QXmlStreamWriter & xmlWriter );
        static void exploreBaskets( Explorer::ItemTreeBasket * basket, QXmlStreamWriter & xmlWriter );
        static void exportBasket( Explorer::ItemTreeBasket * basket, QXmlStreamWriter & xmlWriter );
        static void exportToFile( Explorer::ItemTreeBasket * root, const QString & file);

    protected:
        ExportDataToXml();

    };

}

#endif // EXPORTDATATOXML_H
