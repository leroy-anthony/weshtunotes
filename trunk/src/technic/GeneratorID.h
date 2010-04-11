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

#ifndef GENERATORID_H
#define GENERATORID_H

#include <QString>
#include <QMap>

namespace Technic
{
    class GeneratorID
    {
    public:
        GeneratorID( const QString & name );

        const QString & id() const;
        void setId( const QString & id );
        void regenerateId();

    private:
        QString m_name;
        QString m_nameId;

    private:
        void nextCacheId( const QString & m_name );
        long cacheId( const QString & m_name );
        static QMap<QString,long> m_cacheId;
    };
}

#endif // GENERATORID_H
