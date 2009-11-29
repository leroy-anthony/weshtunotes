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

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <QDir>
#include <QSettings>
#include <KConfig>
#include <KConfigGroup>

namespace Config
{

    class Configuration : protected KConfig
    {
    public:
        Configuration();
        Configuration( const QString & config );
        ~Configuration();

        QString fileName () const;

        static void clear( const QString & fileName );

        void setValue( const QString & group, const QString & key, QVariant value );
        void setValue( const QString & group, const QString & subGroup, const QString & key, QVariant value );

        QString valueSubGroup( const QString & group, const QString & subGroup, const QString & key, QVariant defaultValue ) const;
        QString valueGroup( const QString & group, const QString & key, QVariant defaultValue ) const;
        QStringList values( const QString & group, const QString & key ) const;

        void removeSubGroup( const QString & group, const QString & subGroup );
        void removeGroup( const QString & group );



        static void iniConfigration();

        static QStringList masterBaskets();
        static void saveMasterBaskets( const QStringList & masterBaskets );
        static void saveLastBasket( const QString & name );
        static QString loadLastBasket();

        static QDir dir( const QString & dirConfig );
        static void removeConfigDir( const QString & dir );
        static QStringList subDirs( const QString & directory );

        static QString loadNote( const QString & fileName, const QString & nameId );
        static void saveNote( const QString & fileName, const QString & contentNote, const QString & nameId );

        static void deleteFile( const QString & name );

    };

}

#endif // CONFIGURATION_H_
