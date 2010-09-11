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

#include "DataManager.h"

#include <QPair>
#include <QFileInfo>
#include <QCoreApplication>
#include <QTemporaryFile>

#include "settings.h"
#include "../main/general.h"
#include "../config/Configuration.h"

namespace Data
{
      
    DataManager::DataManager():
            KConfig( "kweshtunotesrc" )
    {
    }

    DataManager::DataManager( const QString & config ):
            KConfig( Settings::basketsStorePath().toLocalFile()+QDir::separator()+config )
    {
    }

    void DataManager::iniConfigration()
    {
        QDir dir(datasStorePath());
        if ( !dir.exists() )
        {
            dir.mkpath(datasStorePath());
        }

        QDir dir2( basketsStorePath() );
        if ( !dir2.exists() )
        {
            dir2.mkpath( basketsStorePath() );
        }

        QDir dir3( itemsStorePath() );
        if ( !dir3.exists() )
        {
            dir3.mkpath( itemsStorePath() );
        }

        QDir dir4( associationStorePath() );
        if ( !dir4.exists() )
        {
            dir4.mkpath( associationStorePath() );
        }
    }

    void DataManager::reparseData()
    {
        reparseConfiguration();
    }

    void DataManager::setContent( const QByteArray & data )
    {
        QFile file(fileName());
        if ( file.open(QIODevice::WriteOnly) )
        {
            file.write(data);
            file.close();

            reparseConfiguration();
        }
    }

    void DataManager::clear()
    {
        clear(name());
    }

    void DataManager::clearBasketConfig( const QString & directoryScene )
    {
        //efface l'ancien configuration du panier
        Data::DataManager::clear( directoryScene );
    }

    void DataManager::setIdScene( const QString & directoryScene, const QString & id )
    {
        Data::DataManager settings( directoryScene );
        settings.setValue("scene", "id", id );
    }

    void DataManager::saveSubHandles( const QString & configFileHandle, const QStringList & listHandles )
    {
        Data::DataManager settingsHandle( configFileHandle );
        settingsHandle.setValue( "general", "items", listHandles );
    }

    void DataManager::saveView( const QString & fileName, const QTransform & transformView, int hscroll, int vscroll, const QString & type )
    {
        Data::DataManager settings( fileName );

        settings.setValue("scene","transform_m11",transformView.m11());
        settings.setValue("scene","transform_m12",transformView.m12());
        settings.setValue("scene","transform_m21",transformView.m21());
        settings.setValue("scene","transform_m22",transformView.m22());
        settings.setValue("scene","transform_dx",transformView.dx());
        settings.setValue("scene","transform_dy",transformView.dy());

        settings.setValue("scene","hscroll",hscroll);
        settings.setValue("scene","vscroll",vscroll);
        settings.setValue("scene","type",type);
    }

    QPair< QTransform,QPair<int, int> >  DataManager::loadView( const QString & fileName )
    {
        DataManager settings( fileName );

        QPair<QTransform,QPair<int, int> > resultat;

        resultat.first = QTransform(
                settings.valueGroup("scene","transform_m11",1).toInt(),
                settings.valueGroup("scene","transform_m12",0).toInt(),
                settings.valueGroup("scene","transform_m21",0).toInt(),
                settings.valueGroup("scene","transform_m22",1).toInt(),
                settings.valueGroup("scene","transform_dx",0).toInt(),
                settings.valueGroup("scene","transform_dy",0).toInt());

        QPair<int, int> scroll;
        scroll.first = settings.valueGroup("scene","hscroll",0).toInt();
        scroll.second = settings.valueGroup("scene","vscroll",0).toInt();

        resultat.second = scroll;

        return resultat;
    }

    QString DataManager::basketsStorePath()
    {
        return Settings::basketsStorePath().toLocalFile()+QDir::separator()+"baskets"+QDir::separator();
    }

    QString DataManager::itemsStorePath()
    {
        return Settings::basketsStorePath().toLocalFile()+QDir::separator()+"items"+QDir::separator();
    }

    QString DataManager::datasStorePath()
    {
        return Settings::basketsStorePath().toLocalFile()+QDir::separator()+"data"+QDir::separator();
    }

    QString DataManager::associationStorePath()
    {
        return Settings::basketsStorePath().toLocalFile()+QDir::separator()+"association"+QDir::separator();
    }

    QStringList DataManager::masterBaskets()
    {
        DataManager settings("baskets/baskets");

        return settings.values("general","baskets");
    }

    void DataManager::addMasterBasket( const QString & id )
    {
        DataManager settings("baskets/baskets");

        QStringList baskets = settings.values("general","baskets");
        baskets += id;
        settings.setValue("general","baskets",baskets);
    }

    void DataManager::addMasterBasket( const QStringList & ids )
    {
        if ( !ids.isEmpty() )
        {
            DataManager settings("baskets/baskets");
            settings.setValue("general","baskets",ids);
        }
    }

    void DataManager::addBasket( const QString & idParent, const QStringList & ids )
    {
        if ( !ids.isEmpty() )
        {
            DataManager settings("baskets/baskets");
            settings.setValue(idParent,"baskets",ids);
        }
    }

    void DataManager::addBasket( const QString & idParent, const QString & id )
    {
        DataManager settings("baskets/baskets");

        QStringList baskets = settings.values(id,"baskets");
        baskets += id;
        settings.setValue(idParent,"baskets",baskets);
    }

    void DataManager::saveLastBasket( const QString & name )
    {
        DataManager settings;
        settings.setValue( "config", "lastBasket", name );
    }

    QString DataManager::loadLastBasket()
    {
        DataManager settings;
        return settings.valueGroup( "config", "lastBasket", "" );
    }

    QStringList DataManager::subBaskets( const QString & directory )
    {
        DataManager settings("baskets/baskets");

        return settings.values(directory,"baskets");
    }

    QStringList DataManager::subDirs( const QString & directory )
    {
        QDir dir = Data::DataManager::dir( directory );
        dir.setFilter( QDir::Dirs | QDir::NoDotAndDotDot );

        return dir.entryList();
    }

    QDir DataManager::dir( const QString & dirConfig )
    {
        DataManager s( dirConfig );
        QFileInfo fileInfo(s.fileName());
        QDir dir = fileInfo.absoluteDir();

        return dir;
    }

    void DataManager::removeBaskets()
    {
        DataManager settings("baskets/baskets");
        QFile::remove(settings.fileName());
    }

    void DataManager::removeBasket( const QString & directory )
    {
        DataManager settings(directory);
        QFile::remove(settings.fileName());
    }

    void DataManager::removeConfigDir( const QString & directory )
    {
        QDir dir = DataManager::dir( directory );
        dir.setFilter(QDir::Files);

        QStringList files = dir.entryList();
        for ( int i=0 ; i<files.size() ; ++i )
        {
            dir.remove( files[i] );
        }

        dir.rmdir(dir.absolutePath());
    }

    void DataManager::deleteNoteFile( const QString & name )
    {
        DataManager settings(name);
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
    
    void DataManager::removeNotes( const QString & idScene )
    {
	Data::DataManager settings( QString("../association") + QDir::separator() + idScene );
	QFile::remove(settings.fileName());
    }

    QString DataManager::fileName() const
    {
        return name();
    }

    void DataManager::clear( const QString & fileName )
    {
        QFile f(Settings::basketsStorePath().toLocalFile()+QDir::separator()+fileName);
        f.remove();
    }

    void DataManager::setValue( const QString & groupKey, const QString & key, QVariant value )
    {
        KConfigGroup groupConfig = group( groupKey );
        groupConfig.writeEntry( key, value );
        groupConfig.sync();
    }

    void DataManager::setValue( const QString & groupKey, const QString & subGroupKey, const QString & key, QVariant value )
    {
        KConfigGroup groupConfig = group( groupKey );
        KConfigGroup subGroup = groupConfig.group( subGroupKey );
        subGroup.writeEntry( key, value );
        subGroup.sync();
    }

    QString DataManager::valueGroup( const QString & groupKey, const QString & key, QVariant defaultValue ) const
    {
        KConfigGroup configGroup( this, groupKey );
        return configGroup.readEntry( key, defaultValue ).toString();
    }
    
    QPoint DataManager::pointValueGroup( const QString & groupKey, const QString & key, QVariant defaultValue ) const
    {
        KConfigGroup configGroup( this, groupKey );
        return configGroup.readEntry( key, defaultValue ).toPoint();
    }

    void DataManager::removeValue( const QString & groupKey, const QString & key )
    {
        KConfigGroup configGroup( this, groupKey );
        return configGroup.deleteEntry( key );
    }

    QString DataManager::valueSubGroup( const QString & groupKey, const QString & subGroupKey, const QString & key, QVariant defaultValue ) const
    {
        KConfigGroup configGroup( this, groupKey );
        return configGroup.group(subGroupKey).readEntry( key, defaultValue ).toString();
    }

    QStringList DataManager::values( const QString & groupKey, const QString & key ) const
    {
        KConfigGroup configGroup = group( groupKey );
        return configGroup.readEntry( key, QStringList() );
    }

    void DataManager::removeSubGroup( const QString & group, const QString & subGroup )
    {
        KConfigGroup configGroup( this, group );
        configGroup.group(subGroup).deleteGroup();
	configGroup.sync();
    }

    void DataManager::removeGroup( const QString & group )
    {
        KConfigGroup configGroup( this, group );
        configGroup.deleteGroup();
	configGroup.sync();
    }

    QString DataManager::loadNote( const QString & nameId )
    {
        QFile f( itemsStorePath() + nameId + ".html" );
        f.open(QFile::ReadOnly | QFile::Text);

        QTextStream stream(&f);
        QString resultat = stream.readAll();
        f.close();

        return resultat;
    }

    void DataManager::saveNote( const QString & contentNote, const QString & nameId )
    {
        QFile f( itemsStorePath() + nameId + ".html" );
        f.open(QFile::WriteOnly | QFile::Text);

        QTextStream stream(&f);
        stream << contentNote;
        f.close();	
    }

    QString DataManager::itemPath( const QString & id )
    {
        return Data::DataManager::itemsStorePath()+id+".html";
    }

    QString DataManager::configFileBasket( const QString & id )
    {
        return QString("baskets/") + id;
    }

    QString DataManager::configFileItem( const QString & id )
    {
        return QString("items/") + id;
    }

    QString DataManager::configFileAssoc( const QString & id )
    {
        return QString("association/") + id;
    }

}
