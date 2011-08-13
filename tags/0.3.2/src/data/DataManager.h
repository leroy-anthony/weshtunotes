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

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QString>
#include <QTransform>
#include <QPair>
#include <QDir>

#include <KConfig>

#include "../synchro/AbstractConnection.h"

namespace Data
{

    class DataManager : protected KConfig
    {
    public:
        DataManager();
        DataManager( const QString & config );

        void reparseData();

        void setContent( const QByteArray & data );

        static void clearBasketConfig( const QString & directoryScene );
        static void setIdScene( const QString & directoryScene, const QString & id );
        static void saveSubHandles( const QString & configFileHandle, const QStringList & listHandles );
        static void saveView( const QString & fileName, const QTransform & transformView, int hscroll, int vscroll, const QString & type );
        static QPair< QTransform,QPair<int, int> > loadView( const QString & fileName );


        //-----------------------------//

        static void clear( const QString & fileName );
        void clear();

        void setValue( const QString & group, const QString & key, QVariant value );
        void setValue( const QString & group, const QString & subGroup, const QString & key, QVariant value );

        QString valueSubGroup( const QString & group, const QString & subGroup, const QString & key, QVariant defaultValue ) const;
        QString valueGroup( const QString & group, const QString & key, QVariant defaultValue ) const;
	QPoint pointValueGroup( const QString & groupKey, const QString & key, QVariant defaultValue ) const;
        QStringList values( const QString & group, const QString & key ) const;

        void removeValue( const QString & groupKey, const QString & key );
        void removeSubGroup( const QString & group, const QString & subGroup );
        void removeGroup( const QString & group );

        static void iniConfigration();

        static QStringList masterBaskets();
        static void addBasket( const QString & idParent, const QString & id );
        static void addBasket( const QString & idParent, const QStringList & ids );
        static void addMasterBasket( const QString & id );
        static void addMasterBasket( const QStringList & ids );
        static QStringList subBaskets( const QString & directory );
        static void saveMasterBaskets( const QStringList & masterBaskets );
        static void saveLastBasket( const QString & name );
        static QString loadLastBasket();

        static QString loadNote( const QString & nameId );
        static void saveNote( const QString & contentNote, const QString & nameId );

        static void removeBaskets();
        static void removeBasket( const QString & directory );
	static void removeNotes( const QString & idScene );
	
        static void deleteNoteFile( const QString & name );

        static QString itemsStorePath();
        static QString datasStorePath();
        static QString associationStorePath();
        static QString basketsStorePath();

        static QString itemPath( const QString & id );


        static QString configFileAssoc( const QString & id );
        static QString configFileBasket( const QString & id );
        static QString configFileItem( const QString & id );

        QString fileName () const;


    protected:
        static QDir dir( const QString & dirConfig );
        static void removeConfigDir( const QString & dir );
        static QStringList subDirs( const QString & directory );


    };

}

#endif // DATAMANAGER_H
