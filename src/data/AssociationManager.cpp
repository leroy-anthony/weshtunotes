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

#include "AssociationManager.h"
#include "settings.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QVariant>
#include <QDebug>

#include "../config/Configuration.h"

namespace Data
{

    AssociationManager::AssociationManager()
    {
    }

    bool AssociationManager::connectDB()
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(Settings::basketsStorePath().toLocalFile()+QDir::separator()+"kweshtunotes.db");

        if (!db.open())
        {
            qDebug() << "erreur";
        }

        QSqlQuery query("SELECT count(name) FROM sqlite_master WHERE type='table' AND name='kwesh_notes'");

        while (query.next())
        {
            int tableName = query.value(0).toInt();

            if ( tableName == 0 )
            {
                if ( !query.exec("CREATE TABLE kwesh_notes (note_id TEXT, scene_id TEXT, x INT, y INT, PRIMARY KEY (note_id, scene_id) )") )
                {
                    qDebug() << query.lastError();
                }
            }
        }

        return true;
    }

    QList<QString> AssociationManager::abstractNotes( const QString & idScene )
    {
        QSqlQuery query;
        query.prepare("SELECT note_id FROM kwesh_notes WHERE scene_id=?");
        query.addBindValue(idScene);
        query.exec();

        QList<QString> list;

        while (query.next())
        {
            list <<  query.value(0).toString();
        }

        return list;
    }

    QPoint AssociationManager::positionAbstractNotes( const QString & idScene, const QString & urlItem )
    {
        QSqlQuery query;
        query.prepare("SELECT x, y FROM kwesh_notes WHERE scene_id=? AND note_id=?");
        query.addBindValue(idScene);
        query.addBindValue(urlItem);
        query.exec();

        while (query.next())
        {
            return QPoint(query.value(0).toInt(),query.value(1).toInt());
        }

        return QPoint(0,0);
    }

    bool AssociationManager::addNote( const QString & idScene, const QString & urlItem, qlonglong x, qlonglong y )
    {
        QSqlQuery query;

        query.prepare("INSERT INTO kwesh_notes ( note_id, scene_id, x, y ) VALUES ( ?, ?, ?, ? ) ");
        query.addBindValue(urlItem);
        query.addBindValue(idScene);
        query.addBindValue(x);
        query.addBindValue(y);

        if ( !query.exec() )
        {
            query.prepare("UPDATE kwesh_notes SET x = ?, y = ? WHERE note_id = ? AND scene_id = ?");
            query.addBindValue(x);
            query.addBindValue(y);
            query.addBindValue(urlItem);
            query.addBindValue(idScene);

            return query.exec();
        }

        return true;
    }

    bool AssociationManager::removeNote( const QString & idScene, const QString & urlItem )
    {
        QSqlQuery query;
        query.prepare("DELETE FROM kwesh_notes WHERE note_id = ? AND scene_id = ?");
        query.addBindValue(urlItem);
        query.addBindValue(idScene);

        Config::Configuration::deleteNoteFile(urlItem);

        return query.exec();
    }

    bool AssociationManager::removeNotes( const QString & idScene )
    {
        QList<QString> notes = abstractNotes( idScene );
        for ( int i=0 ; i<notes.size() ; ++i )
        {
            Config::Configuration::deleteNoteFile(notes[i]);
        }

        QSqlQuery query;
        query.prepare("DELETE FROM kwesh_notes WHERE scene_id = ?");
        query.addBindValue(idScene);

        return query.exec();
    }

    bool AssociationManager::removeAssociationNotes( const QString & idScene )
    {
        QSqlQuery query;
        query.prepare("DELETE FROM kwesh_notes WHERE scene_id = ?");
        query.addBindValue(idScene);

        return query.exec();
    }

}
