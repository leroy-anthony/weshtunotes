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
        QDir::addSearchPath( "data", "/home/kahal/Programmation/C++/weshtunotes/data" );
        QDir::addSearchPath( "icon", "/home/kahal/Programmation/C++/weshtunotes/data/icon" );

        QDir dir(Constant::homeData);
        if ( !dir.exists() )
        {
            dir.mkpath(Constant::homeData);
        }
        QDir::addSearchPath( "data", Constant::homeData );
        QDir::addSearchPath( "icon", Constant::homeData+QDir::separator()+"icon" );

        QDir dir2(Constant::homeBaskets);
        if ( !dir2.exists() )
        {
            dir2.mkpath(Constant::homeBaskets);
        }
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
        qDebug() << settings.fileName();
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

    QString Configuration::fileName () const
    {
        return QSettings::fileName();
    }

    void Configuration::clear()
    {
        QSettings::clear();
    }

    void Configuration::beginGroup( const QString & prefix )
    {
        QSettings::beginGroup( prefix );
    }

    void Configuration::endGroup()
    {
        QSettings::endGroup();
    }

    void Configuration::setValue( const QString & key, const QVariant & value )
    {
        QSettings::setValue( key, value );
    }

    QVariant Configuration::value( const QString & key, const QVariant & defaultValue ) const
    {
        return QSettings::value( key, defaultValue );
    }

    void Configuration::remove( const QString & key )
    {
        QSettings::remove( key );
    }

    QStringList Configuration::childGroups() const
    {
        return QSettings::childGroups();
    }

    int Configuration::beginReadArray( const QString & prefix )
    {
        return QSettings::beginReadArray( prefix );
    }

    void Configuration::beginWriteArray( const QString & prefix )
    {
        QSettings::beginWriteArray( prefix );
    }

    void Configuration::setArrayIndex( int i )
    {
        QSettings::setArrayIndex( i );
    }

    void Configuration::endArray()
    {
        QSettings::endArray();
    }

    void Configuration::sync()
    {
        QSettings::sync();
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
