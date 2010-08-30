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

#include "ImportDataFromXml.h"

#include <QFile>
#include <QXmlStreamReader>
#include <KDebug>

namespace Data
{

    ImportDataFromXml::ImportDataFromXml()
    {
    }

    void ImportDataFromXml::importFromFile( const QString & filePath )
    {
        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            return;
        }

        QXmlStreamReader xml(&file);

        while (xml.readNextStartElement())
        {
            kDebug() << xml.name();

            if (xml.name() == "kweshtunotes")
            {
                readRoot(xml);
            }
            else if (xml.name() == "baskets")
            {
                readBasket(xml);
            }
            else if (xml.name() == "basket")
            {
                readBasket(xml);
            }
            else if (xml.name() == "scene")
            {
                readScene(xml);
            }
            else if (xml.name() == "handle")
            {
                readHandle(xml);
            }
            else if (xml.name() == "item")
            {
                readItem(xml);
            }
            else
            {
                xml.skipCurrentElement();
            }
        }
    }

    void ImportDataFromXml::readRoot( QXmlStreamReader & xml )
    {
        kDebug() << "root";
    }

    void ImportDataFromXml::readBaskets( QXmlStreamReader & xml )
    {
        kDebug() << "baskets";
    }

    void ImportDataFromXml::readBasket( QXmlStreamReader & xml )
    {
        kDebug() << "basket";
    }

    void ImportDataFromXml::readScene( QXmlStreamReader & xml )
    {
        kDebug() << "scene";
    }

    void ImportDataFromXml::readHandle( QXmlStreamReader & xml )
    {
        kDebug() << "handle";
    }

    void ImportDataFromXml::readItem( QXmlStreamReader & xml )
    {
        kDebug() << "item";
    }

}
