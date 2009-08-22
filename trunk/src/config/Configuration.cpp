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

#include "Configuration.h"

#include <QFileInfo>
#include <QCoreApplication>

#include "../main/general.h"

namespace Config
{

    int Constant::defaultHandleWidth = 400;
    QString Constant::main = "main";
    QString Constant::lastBasket = "lastBasket";
    QString Constant::dirBasketKey = "dirBasket";
    QString Constant::dirDataKey = "data";
    QString Constant::home = QDir::homePath()+QDir::separator()+".weshTuNotes";
    QString Constant::homeData = QDir::homePath()+QDir::separator()+".weshTuNotes"+QDir::separator()+"data";
    QString Constant::homeBaskets = QDir::homePath()+QDir::separator()+".weshTuNotes"+QDir::separator()+"baskets";
    QString Constant::dirBasket = Configuration().value(Constant::dirBasketKey,Constant::homeBaskets).toString();

    Configuration::Configuration():
            QSettings( QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), Constant::main )
    {
    }

    Configuration::Configuration( const QString & config ):
            QSettings( Constant::dirBasket+QDir::separator()+config, QSettings::IniFormat )
    {
    }

    void Configuration::iniConfigration()
    {
        QDir::addSearchPath( "icon", "./data/icon" ); // windows
        QDir::addSearchPath( "icon", "/usr/share/weshtunotes/data/icon" ); // unix
        QDir::addSearchPath( "icon", "/usr/local/share/weshtunotes/data/icon" ); // unix

        QDir dir(Constant::homeData);
        if ( !dir.exists() )
        {
            dir.mkpath(Constant::homeData);
        }
        QDir::addSearchPath( "data", Constant::homeData );
    }

    Configuration::~Configuration()
    {
    }

    QStringList Configuration::masterBaskets()
    {
        QDir dir(Constant::dirBasket);
        dir.setFilter( QDir::Dirs | QDir::NoDotAndDotDot );

        return dir.entryList();
    }

    void Configuration::saveLastBasket( const QString & name )
    {
        Configuration settings;
        settings.setValue( Constant::lastBasket, name );
        settings.sync();
    }

    QString Configuration::loadLastBasket()
    {
        Configuration settings;
        return settings.value( Constant::lastBasket ).toString();
    }

    QStringList Configuration::subDirs( const QString & directory )
    {
        QDir dir = Config::Configuration::dir( directory );
        dir.setFilter( QDir::Dirs | QDir::NoDotAndDotDot );

        return dir.entryList();
    }

    QDir Configuration::dir( const QString & dirConfig )
    {
        Configuration s( dirConfig );
        QFileInfo fileInfo(s.fileName());
        QDir dir = fileInfo.absoluteDir();

        return dir;
    }

    void Configuration::removeConfigDir( const QString & directory )
    {
        QDir dir = Config::Configuration::dir( directory );
        dir.setFilter(QDir::Files);

        QStringList files = dir.entryList();
        for ( int i=0 ; i<files.size() ; ++i )
        {
            dir.remove( files[i] );
        }

        dir.rmdir(dir.absolutePath());
    }

    void Configuration::deleteFile( const QString & name )
    {
        Configuration settings(name);
        QFile::remove(settings.fileName());
    }
}
