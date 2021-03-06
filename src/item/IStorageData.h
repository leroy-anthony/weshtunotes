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

#ifndef ISTORAGEDATA_H
#define ISTORAGEDATA_H

#include <QMimeData>
#include <QColor>
#include <QFont>

#include "ItemStorage.h"

namespace Item
{

    class IStorageData
    {
    public:
        virtual ~IStorageData();

        virtual void save( const QString & fileName, const QString & handleId  ) = 0;
        virtual void load( const QString & fileName ) = 0;
        virtual void load( const QMimeData * data ) = 0;

    //protected:
    //     ItemStorage m_itemStorage;

    };

}

#endif // ISTORAGEDATA_H
