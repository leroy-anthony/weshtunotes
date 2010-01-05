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

#include "settings.h"
#include "../main/general.h"
#include "../main/general.h"

namespace Config
{

    Configuration::Configuration():
            KConfig( "kweshtunotesrc" )
    {
    }

    Configuration::Configuration( const QString & config ):
            KConfig( Settings::basketsStorePath().toLocalFile()+QDir::separator()+"baskets"+QDir::separator()+config )
    {
    }

    void Configuration::iniConfigration()
    {
        QDir dir(Settings::basketsStorePath().toLocalFile()+QDir::separator()+"data");
        if ( !dir.exists() )
        {
            dir.mkpath(Settings::basketsStorePath().toLocalFile()+QDir::separator()+"data");
        }
        QDir::addSearchPath( "data", Settings::basketsStorePath().toLocalFile()+QDir::separator()+"data" );
        QDir::addSearchPath( "icon", Settings::basketsStorePath().toLocalFile()+QDir::separator()+"data"+QDir::separator()+"icon" );

        QDir dir2( Settings::basketsStorePath().toLocalFile()+QDir::separator()+"baskets" );
        if ( !dir2.exists() )
        {
            dir2.mkpath( Settings::basketsStorePath().toLocalFile()+QDir::separator()+"baskets" );
        }
    }

    Configuration::~Configuration()
    {
    }

    QStringList Configuration::masterBaskets()
    {
        QDir dir( Settings::basketsStorePath().toLocalFile()+QDir::separator()+"baskets" );
        dir.setFilter( QDir::Dirs | QDir::NoDotAndDotDot );

        return dir.entryList();
    }

    void Configuration::saveLastBasket( const QString & name )
    {
        Configuration settings;
        settings.setValue( "", "lastBasket", name );
    }

    QString Configuration::loadLastBasket()
    {
        Configuration settings;
        return settings.valueGroup( "", "lastBasket", "" );
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
        QFileInfo file(settings.fileName());

        QStringList items = settings.values("general","items");
        for ( int i=0 ; i<items.size() ; ++i )
        {
            QString dataFileName = settings.valueGroup(items[i],"data","");
            if( dataFileName != QString("") )
            {
                QFile::remove(file.absoluteDir().path()+QDir::separator()+dataFileName+".html");
            }
        }

        QFile::remove(settings.fileName());
    }

    QString Configuration::fileName () const
    {
        return name();
    }

    void Configuration::clear( const QString & fileName )
    {
        QFile f(fileName);
        f.remove();
    }

    void Configuration::setValue( const QString & groupKey, const QString & key, QVariant value )
    {
        KConfigGroup groupConfig = group( groupKey );
        groupConfig.writeEntry( key, value );
        groupConfig.sync();
    }

    void Configuration::setValue( const QString & groupKey, const QString & subGroupKey, const QString & key, QVariant value )
    {
        KConfigGroup groupConfig = group( groupKey );
        KConfigGroup subGroup = groupConfig.group( subGroupKey );
        subGroup.writeEntry( key, value );
        subGroup.sync();
    }

    QString Configuration::valueGroup( const QString & groupKey, const QString & key, QVariant defaultValue ) const
    {
        KConfigGroup configGroup( this, groupKey );
        return configGroup.readEntry( key, defaultValue ).toString();
    }

    QString Configuration::valueSubGroup( const QString & groupKey, const QString & subGroupKey, const QString & key, QVariant defaultValue ) const
    {
        KConfigGroup configGroup( this, groupKey );
        return configGroup.group(subGroupKey).readEntry( key, defaultValue ).toString();
    }

    QStringList Configuration::values( const QString & groupKey, const QString & key ) const
    {
        KConfigGroup configGroup = group( groupKey );
        return configGroup.readEntry( key, QStringList() );
    }

    void Configuration::removeSubGroup( const QString & group, const QString & subGroup )
    {
        KConfigGroup configGroup( this, group );
        configGroup.group( subGroup ).deleteGroup();
    }

    void Configuration::removeGroup( const QString & group )
    {
        KConfigGroup configGroup( this, group );
        configGroup.deleteGroup();
    }

    QString Configuration::loadNote( const QString & fileName, const QString & nameId )
    {
        Config::Configuration settings( fileName );

        QFileInfo fileInfo(settings.fileName());
        QFile f( fileInfo.absolutePath() + QDir::separator() + nameId + ".html" );
        f.open(QFile::ReadOnly | QFile::Text);

        QTextStream stream(&f);
        QString resultat = stream.readAll();
        f.close();

        return resultat;
    }

    void Configuration::saveNote( const QString & fileName, const QString & contentNote, const QString & nameId )
    {
        QFileInfo fileInfo( fileName );
        QFile f( fileInfo.absolutePath() + QDir::separator() + nameId + ".html" );
        f.open(QFile::WriteOnly | QFile::Text);
        QTextStream stream(&f);
        stream << contentNote;
        f.close();
    }

}
