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

#ifndef ASSOCIATIONMANAGER_H
#define ASSOCIATIONMANAGER_H

#include "../item/AbstractItem.h"

namespace Data
{

    class AssociationManager
    {
    protected:
        AssociationManager();

    public:
        static bool connectDB();

        static QList<QString> abstractNotes( const QString & idScene );
        static QPoint positionAbstractNotes( const QString & idScene, const QString & urlItem );

        static bool addNote( const QString & idScene, const QString & urlItem, qlonglong x, qlonglong y );
        static bool removeNote( const QString & idScene, const QString & urlItem );
        static bool removeNotes( const QString & idScene );
        static bool removeAssociationNotes( const QString & idScene );

    };

}

#endif // ASSOCIATIONMANAGER_H