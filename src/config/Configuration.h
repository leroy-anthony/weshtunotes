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

namespace Config
{

    class Constant
    {
    public:
        static QString main;
        static QString root;
    };

    class Configuration : public QSettings
    {
    public:
        Configuration();
        Configuration( const QString & config );
        ~Configuration();

        QStringList masterBaskets();
        static void saveMasterBaskets( const QStringList & masterBaskets );
        static void saveCurrentBasket( const QString & name );
        static QString loadCurrentBasket();

        static QDir dir( const QString & dirConfig );
        static void removeConfigDir( const QString & dir );
        static QStringList subDirs( const QString & directory );

        static void deleteFile( const QString & name );

    };

}

#endif // CONFIGURATION_H_
