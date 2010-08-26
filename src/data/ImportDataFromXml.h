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

#ifndef IMPORTDATAFROMXML_H
#define IMPORTDATAFROMXML_H

#include <QString>
#include <QXmlStreamReader>

namespace Data
{

    class ImportDataFromXml
    {

    public:
        static void readRoot( QXmlStreamReader & xml );
        static void readBaskets( QXmlStreamReader & xml );
        static void readBasket( QXmlStreamReader & xml );
        static void readScene( QXmlStreamReader & xml );
        static void readHandle( QXmlStreamReader & xml );
        static void readItem( QXmlStreamReader & xml );

        static void importFromFile( const QString & file );

    protected:
        ImportDataFromXml();
    };

}

#endif // IMPORTDATAFROMXML_H
