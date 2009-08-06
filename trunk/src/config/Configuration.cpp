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
#include <QSettings>
#include <QCoreApplication>

#include "../main/general.h"

namespace Config
{

    QString Constant::main = "main";
    QString Constant::root = "root";

    Configuration::Configuration():
            QSettings( QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), Constant::main )
    {
    }

    Configuration::Configuration( const QString & config ):
            QSettings( QSettings::IniFormat, QSettings::UserScope, QCoreApplication::organizationName(), config )
    {
    }


    Configuration::~Configuration()
    {
    }

    QStringList Configuration::masterBaskets()
    {
        return value( Constant::root ).toStringList();
    }

    void Configuration::saveMasterBaskets( const QStringList & masterBaskets )
    {
        Configuration settings;
        settings.clear();
        settings.setValue( "root", masterBaskets );
        settings.sync();
    }

    QStringList Configuration::subDirs( const QString & directory )
    {
        QDir dir = Config::Configuration::dir( directory );
        dir.setFilter( QDir::Dirs | QDir::NoDotAndDotDot );

        return dir.entryList();
    }

    QDir Configuration::dir( const QString & dirConfig )
    {
        QSettings s( QCoreApplication::organizationName(), dirConfig );
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
