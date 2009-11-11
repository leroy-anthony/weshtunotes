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
        static QString lastBasket;
        static QString home;
        static QString dirBasket;
        static QString homeData;
        static QString homeBaskets;
        static QString dirBasketKey;
        static QString dirDataKey;
        static int defaultHandleWidth;
    };

    class Configuration : protected QSettings
    {
    public:
        Configuration();
        Configuration( const QString & config );
        ~Configuration();

        QString fileName () const;

        void clear();

        void beginGroup( const QString & prefix );
        void endGroup();

        void setValue( const QString & key, const QVariant & value );
        QVariant value ( const QString & key, const QVariant & defaultValue = QVariant() ) const;

        void remove ( const QString & key );

        QStringList childGroups() const;

        int beginReadArray ( const QString & prefix );
        void beginWriteArray ( const QString & prefix );

        void setArrayIndex( int i );
        void endArray();

        void sync();


        static void iniConfigration();

        QStringList masterBaskets();
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
