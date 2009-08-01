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

#include "FreeScene.h"

#include <QMouseEvent>
#include <QGraphicsWidget>
#include <QLabel>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QGraphicsWidget>
#include <QGraphicsProxyWidget>
#include <QGraphicsGridLayout>
#include <QHBoxLayout>
#include <QGraphicsLinearLayout>
#include <QLabel>
#include <QGraphicsLayout>
#include <QLabel>
#include <QAction>
#include <QApplication>
#include <QGraphicsSvgItem>

#include "../handle/HandleItem.h"
#include "../config/Configuration.h"

namespace Scene
{

    FreeScene::FreeScene(QWidget * parent) :
            AbstractScene(),
            m_currentGraphicsItem(0),
            m_currentAbstractItem(0),
            m_currentHandle(0),
            m_modeItem(Nothing)
    {
    }

    FreeScene::~FreeScene()
    {
    }

    Handle::HandleItem * FreeScene::newHandle( int x, int y )
    {
        Handle::HandleItem * handle = new Handle::HandleItem( this, x, y );
        connect( handle, SIGNAL(move(Handle::HandleItem*,int,int)), this, SLOT(moveItem(Handle::HandleItem*,int,int)));

        return handle;
    }

    Item::AbstractItem * FreeScene::newItem( int x, int y, const QColor & color )
    {
        Item::NoteItem * item = new Item::NoteItem();
        //       item->setItemColor( color );
        connect( item, SIGNAL(editItem(Item::AbstractItem*)), this, SLOT(editItem(Item::AbstractItem*)));

        return item;
    }

    Handle::HandleItem *  FreeScene::addItems( int x, int y, const QString & dataFile )
    {
        Handle::HandleItem * handle = newHandle( x, y );
        handle->handleId();
        Item::AbstractItem * item = newItem( x, y, QColor(115,115,115) );
        handle->add( item );

        addHandleToScene( handle );

        item->load( dataFile );

        return handle;
    }

    QGraphicsProxyWidget * FreeScene::addHandleToScene( Handle::HandleItem * handle )
    {
        QGraphicsProxyWidget * w = m_handles[handle];
        if ( w != 0 )
        {
            m_items.remove(w);
            m_handles.remove(handle);
        }

        w = addWidget(handle);
        w->setPos(handle->x(),handle->y());

        m_handles[handle] = w;
        m_items[w] = handle;

        //w->setOpacity(0.9);

        return w;
    }

    void FreeScene::removeGraphicsItemFromScene( Handle::HandleItem * handle )
    {
        QGraphicsProxyWidget * g = m_handles[ handle ];
        if ( g != 0 )
        {
            removeItem(g);
            g->setWidget(0);
            m_handles.remove( handle );
            m_items.remove( g );
            delete g;
        }
    }

    void FreeScene::editItem( Item::AbstractItem * item )
    {
        m_currentAbstractItem = item;
        // récupère les interfaces pour les opérations et désactive/active les boutons
    }

    void FreeScene::moveItem( Handle::HandleItem * handleItem, int x, int y )
    {
        m_modeItem = MoveItem;

        if ( handleItem->parent() == 0 )
        {
            static_cast<QWidget*>(handleItem)->move(x,y);

            QList<QGraphicsItem *> items = collidingItems( m_handles[handleItem] );
            if ( items.size() > 0 )
            {
                m_items[static_cast<QGraphicsProxyWidget*>(items[0])]->insert( handleItem->geometry().topLeft(), handleItem->height() );
            }
            else
            {
                Handle::HandleItem::resetInsert();
            }
        }
        else
        {    
            Handle::HandleItem * parentItem = handleItem->parentHandle();
            parentItem->remove( handleItem );
            handleItem->setParent(0);
            handleItem->setModeDegroupement(true);
            m_currentGraphicsItem = addHandleToScene( handleItem );
            static_cast<QWidget*>(handleItem)->move(x,y);
        }
    }

    void FreeScene::delItem( Handle::HandleItem * handleItem )
    {
        //TODO: supprimer les noteItem
        Handle::HandleItem * parentItem = handleItem->parentHandle();
        if ( parentItem != 0 )
        {
            parentItem->remove( handleItem );
        }
        else
        {
            removeGraphicsItemFromScene( handleItem );
        }
        handleItem->setParent(0);
    }

    void FreeScene::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
    {
        if ( mouseEvent->button() == Qt::LeftButton )
        {
            m_currentGraphicsItem = itemAt( mouseEvent->scenePos().x(), mouseEvent->scenePos().y() );
            Handle::HandleItem * h = m_items[ static_cast<QGraphicsProxyWidget*>(m_currentGraphicsItem) ];
            if ( h != 0 )
            {
                m_currentHandle = h->handleItemAt( mouseEvent->scenePos().x(), mouseEvent->scenePos().y() );
            }
        }
        else
        {
            addItems( mouseEvent->scenePos().x(), mouseEvent->scenePos().y(), "templatePardefaut" );
        }

        QGraphicsScene::mousePressEvent ( mouseEvent );
    }

    void FreeScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
    {
        if ( m_currentHandle != 0 && m_modeItem == MoveItem )
        {
            QList<QGraphicsItem *> items = collidingItems( m_currentGraphicsItem );
            if ( items.size() > 0 )
            {
                Handle::HandleItem * handleCible = m_items[static_cast<QGraphicsProxyWidget*>(items[0])];
                if ( handleCible->size() == 0 )
                {
                    QPointF pt = static_cast<QGraphicsProxyWidget*>(items[0])->pos();

                    Handle::HandleItem * handle = newHandle( pt.x(), pt.y() );
                    handle->add( handleCible );
                    handle->add( m_currentHandle );
                    addHandleToScene( handle );

                    removeGraphicsItemFromScene( handleCible );
                }
                else
                {
                    handleCible->add( m_currentHandle );
                }

                removeGraphicsItemFromScene( m_currentHandle );
                Handle::HandleItem::resetInsert();
            }
            else
            {
                Handle::HandleItem * parentHandle = m_currentHandle->parentHandle();
                if (  parentHandle != 0 && m_currentHandle->modeDegroupement() )
                {
                    if ( parentHandle->children().size() == 1 )
                    {
                        Handle::HandleItem * child = parentHandle->child();
                        parentHandle->remove( child );
                        child->setParent(0);
                        child->setParentHandle(0);

                        QGraphicsProxyWidget * g = addHandleToScene( child );
                        g->setPos(parentHandle->pos());
                        removeGraphicsItemFromScene( parentHandle );
                    }
                    m_currentHandle->setParentHandle(0);
                }
            }

            m_currentHandle->setModeDegroupement(false);
            m_currentHandle->setHoverMode( false );
            if ( m_currentHandle->noteItem() != 0 )
            {
                m_currentHandle->noteItem()->isSelected();
            }
        }

        m_modeItem = Nothing;
        QGraphicsScene::mouseReleaseEvent ( mouseEvent );
    }

    Item::AbstractItem * FreeScene::currentAbstractItem()
    {
        return m_currentAbstractItem;
    }

    QGraphicsItem * FreeScene::currentGraphicsItem()
    {
        return m_currentGraphicsItem;
    }

    Handle::HandleItem * FreeScene::currentHandle()
    {
        return m_currentHandle;
    }

    void FreeScene::buildListHandleToLoad( Handle::HandleItem * h, QStringList & l )
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

    void FreeScene::save( const QString & fileName )
    {
        QList<Handle::HandleItem*> handles = m_handles.keys();

        Config::Configuration settings( fileName );
        settings.clear();
        settings.sync();

        settings.beginGroup("scene");
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

    }

    void FreeScene::load( const QString &  fileName )
    {
        Config::Configuration settings( fileName );

        QMap<QString,Handle::HandleItem*> handles;
        QMap<QString,Item::AbstractItem*> itemsToLoad;
        QList<QStringList> listItem;

        settings.beginGroup("scene");
        int size = settings.beginReadArray("items");
        for ( int i=0 ; i<size ; ++i )
        {
            settings.setArrayIndex(i);
            listItem << settings.value("items").toStringList();
        }
        settings.endArray();
        settings.endGroup();

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
                    Item::AbstractItem * item = newItem( 0, 0, settings.value("color").value<QColor>() );
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

        for ( int j=0 ; j<itemsToLoad.keys().size() ; ++j )
        {
            itemsToLoad[ itemsToLoad.keys()[j] ]->load( fileName );
        }
    }
}

