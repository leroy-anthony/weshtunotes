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

#include "AssociationManager.h"
#include "settings.h"

#include <QMessageBox>
#include <QVariant>
#include <QDebug>
#include <QSet>

#include "../config/Configuration.h"
#include "../data/DataManager.h"

namespace Data
{

    AssociationManager::AssociationManager()
    {
    }

    QList<QString> AssociationManager::abstractNotes( const QString & idScene )
    {
	Data::DataManager settings( QString("association") + QDir::separator() + idScene );

	return settings.values( "general", "items" );
    }

    QPoint AssociationManager::positionAbstractNotes( const QString & idScene, const QString & urlItem )
    {
	Data::DataManager settings( QString("association") + QDir::separator() + idScene );
	return settings.pointValueGroup( idScene, urlItem, QPoint(0,0) );
    }

    bool AssociationManager::addNote( const QString & idScene, const QString & urlItem, qlonglong x, qlonglong y )
    {
	Data::DataManager settings( QString("association") + QDir::separator() + idScene );

	QStringList items = settings.values( "general", "items" );
	if ( !items.contains(urlItem) )
	{
	    items <<  urlItem;
	}
	settings.setValue( "general", "items", items );

	settings.setValue( idScene, urlItem, QPoint(x,y) );

	return true;
    }

    bool AssociationManager::removeNote( const QString & idScene, const QString & urlItem )
    {
	Data::DataManager settings( QString("association") + QDir::separator() + idScene );

	QStringList items = settings.values( "general", "items" );
	items.removeAll(urlItem);
	settings.setValue( "general", "items", items );

	settings.removeValue( idScene, urlItem );

	Data::DataManager::deleteNoteFile(urlItem);

	return true;
    }

    bool AssociationManager::removeNotes( const QString & idScene )
    {
	Data::DataManager::removeNotes( idScene );
      
	QList<QString> notes = abstractNotes( idScene );
	for ( int i=0 ; i<notes.size() ; ++i )
	{
	    Data::DataManager::deleteNoteFile(notes[i]);
	}

	return true;
    }

    bool AssociationManager::removeAssociationNotes( const QString & idScene )
    {
	Data::DataManager settings( QString("association") + QDir::separator() + idScene );
	settings.clear();

	return true;
    }

}
