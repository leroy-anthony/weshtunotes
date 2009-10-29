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

#include "AbstractScene.h"

#include <QScrollBar>

#include "../config/Configuration.h"
#include "../handle/HandleItem.h"

namespace Scene
{

    AbstractScene::~AbstractScene()
    {
    }

    AbstractScene::AbstractScene():
            m_horizontalScrollBarValueView(0),
            m_verticalScrollBarValueView(0)
    {
        setSceneRect(-100000,-100000,200000,200000);
    }

    const QString & AbstractScene::type()
    {
        return m_type;
    }

    void AbstractScene::setType( const QString & type )
    {
        m_type = type;
    }

    void AbstractScene::addData( const QMimeData * data )
    {
    }

    const QString & AbstractScene::id()
    {
        return m_id;
    }

    void AbstractScene::buildListHandleToLoad( Handle::HandleItem * h, QStringList & l )
    {
        if ( h->size() > 0 )
        {
            QList<Handle::HandleItem*> handlesChild = h->children();
            for ( int j=0 ; j<handlesChild.size() ; ++j )
            {
                buildListHandleToLoad( handlesChild[j], l );
            }
            l << h->handleId();
        }
        else
        {
            l << h->handleId();
        }
    }

    void AbstractScene::save( const QString & id, const QString & fileName )
    {
        QList<Handle::HandleItem*> handles = m_handles.keys();

        Config::Configuration settings( fileName );
        settings.clear();
        settings.sync();

        settings.beginGroup("scene");
        settings.setValue("id",id);
        settings.beginWriteArray("items");
        for (int i=0 ; i<handles.size() ; ++i)
        {
            QStringList listHandles;
            buildListHandleToLoad( handles[i], listHandles );

            settings.setArrayIndex(i);
            if ( listHandles.size() > 0 )
            {
                settings.setValue("items",listHandles);
            }
        }
        settings.endArray();
        settings.endGroup();

        settings.sync();

        for ( int i=0 ; i<handles.size() ; ++i )
        {
            handles[i]->save(fileName);
        }

        saveViewOnDisk( fileName );
    }

    void AbstractScene::storeView( CustomGraphicsView * view )
    {
        view->setInteractive(false);
        m_transformView = view->transform();
        m_horizontalScrollBarValueView = view->horizontalScrollBar()->value();
        m_verticalScrollBarValueView = view->verticalScrollBar()->value();
        view->setInteractive(true);
    }

    void AbstractScene::restoreView( CustomGraphicsView * view  )
    {
        view->setInteractive(false);
        view->setTransform(m_transformView);
        view->horizontalScrollBar()->setValue(m_horizontalScrollBarValueView);
        view->verticalScrollBar()->setValue(m_verticalScrollBarValueView);
        view->setInteractive(true);
    }

    void AbstractScene::saveViewOnDisk( const QString & fileName )
    {
        Config::Configuration settings( fileName );
        settings.beginGroup("scene");
        settings.setValue("transform",m_transformView);
        settings.setValue("hscroll",m_horizontalScrollBarValueView);
        settings.setValue("vscroll",m_verticalScrollBarValueView);
        settings.setValue("type",m_type);
        settings.endGroup();
        settings.sync();
    }

    void AbstractScene::loadViewFromDisk( const QString & fileName  )
    {
        Config::Configuration settings( fileName );
        settings.beginGroup("scene");
        m_transformView = settings.value("transform").value<QTransform>();
        m_horizontalScrollBarValueView = settings.value("hscroll").toInt();
        m_verticalScrollBarValueView   = settings.value("vscroll").toInt();
    }

    void AbstractScene::load( const QString & fileName )
    {
        Config::Configuration settings( fileName );

        QMap<QString,Handle::HandleItem*> handles;
        QMap<QString,Item::AbstractItem*> itemsToLoad;
        QMap<Handle::HandleItem*,QSize>  handlesSizes;
        QList<QStringList> listItem;

        // récupère tous les noms des notes
        settings.beginGroup("scene");
        m_id = settings.value("id","1").toString();
        int size = settings.beginReadArray("items");
        for ( int i=0 ; i<size ; ++i )
        {
            settings.setArrayIndex(i);
            listItem << settings.value("items").toStringList();
        }
        settings.endArray();
        settings.endGroup();

        // pour chaque note
        for ( int j=0 ; j<listItem.size() ; ++j )
        {
            QStringList items = listItem[j];
            Handle::HandleItem * handle = 0;
            for (int i=0 ; i<items.size() ; ++i)
            {
                settings.beginGroup(items[i]);

                int x = settings.value("x").toInt();
                int y = settings.value("y").toInt();
                handle = newHandle( x, y );
                handle->setHandleId(items[i]);
                handles[ items[i] ] = handle;

                handlesSizes[ handle ] = QSize( settings.value("width").toInt(), settings.value("height").toInt() );

                QStringList itemsToAdd = settings.value("items").toStringList();
                if ( itemsToAdd.size()>0 )
                {
                    for (int j = 0; j < itemsToAdd.size() ; ++j)
                    {
                        handle->add( handles[ itemsToAdd[j] ] );
                    }
                }
                else
                {
                    //TODO: replace me by item->load() ???
                    Item::AbstractItem * item = newItem( 0, 0 );
                    item->setItemColor( QColor(settings.value("color").value<QString>()) );
                    handle->add( item );

                    QString id = settings.value("data").toString();
                    item->setItemId(id);
                    itemsToLoad[ id ] = item;

                    QStringList namesTags = settings.value("tags").toStringList();
                    for ( int i=0 ; i<namesTags.size() ; ++i )
                    {
                        QStringList s = namesTags[i].split(":");
                        dynamic_cast<Item::NoteItem*>(item)->addTag(s[0],s[1]);
                    }

                }
                settings.endGroup();
            }
            addHandleToScene( handle );
        }

        //charge la note
        for ( int j=0 ; j<itemsToLoad.keys().size() ; ++j )
        {
            itemsToLoad[ itemsToLoad.keys()[j] ]->load( fileName );
        }

        //ajuste sa taille
        for ( int j=0 ; j<handlesSizes.keys().size() ; ++j )
        {
            handlesSizes.keys()[j]->resize( handlesSizes[ handlesSizes.keys()[j] ] );
        }

        loadViewFromDisk( fileName );
    }

    void AbstractScene::resize( QResizeEvent * event )
    {
    }

}
