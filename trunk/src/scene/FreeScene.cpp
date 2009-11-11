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
#include <QColor>
#include <QApplication>
#include <QGraphicsSvgItem>
#include <QGraphicsView>
#include <QScrollBar>

#include "../handle/HandleItem.h"
#include "../config/Configuration.h"

#include "../handle/GraphicHandleItem.h"

namespace Scene
{
    QString FreeScene::type = "freescene";

    FreeScene::FreeScene(QWidget * parent) :
            AbstractScene(parent),
            m_currentGraphicsItem(0),
            m_currentAbstractItem(0),
            m_currentHandle(0),
            m_modeItem(Nothing)
    {
        m_type = FreeScene::type;

        setBackgroundBrush(QColor(Qt::cyan).lighter(190));
        /*
          QGraphicsScene::BspTreeIndex
          QGraphicsScene::NoIndex
          */
        setItemIndexMethod(QGraphicsScene::NoIndex);
    }
    
    FreeScene::~FreeScene()
    {
    }
    
    void FreeScene::addData( const QMimeData * data )
    {
        QGraphicsView * view = views()[0];
        QPointF pt = view->mapToScene( view->viewport()->width()/2-200, view->viewport()->height()/2 );

        Handle::HandleItem * handle = newHandle( pt.x(), pt.y() );

        Item::AbstractItem * item = newItem( pt.x(), pt.y() );

        handle->add( item );

        addHandleToScene( handle );

        item->load( data );

        static_cast<QWidget*>(handle)->resize( item->width(), item->height() );
    }

    Handle::HandleItem * FreeScene::newHandle( int x, int y )
    {
        Handle::HandleItem * handle = new Handle::HandleItem( this, x, y );
        connect( handle, SIGNAL(move(Handle::HandleItem*,int,int)), this, SLOT(moveItem(Handle::HandleItem*,int,int)));
        connect( handle, SIGNAL(delItem(Handle::HandleItem*)), this, SLOT(delItem(Handle::HandleItem*)));
        
        return handle;
    }
    
    Item::AbstractItem * FreeScene::newItem( int x, int y )
    {
        Item::NoteItem * item = new Item::NoteItem();
        connect( item, SIGNAL(editItem(Item::AbstractItem*)), this, SLOT(editItem(Item::AbstractItem*)));
        
        return item;
    }
    
    Handle::HandleItem *  FreeScene::addItems( int x, int y, const QString & dataFile )
    {
        Handle::HandleItem * handle = newHandle( x, y );
        
        Item::AbstractItem * item = newItem( x, y );

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

        Handle::GraphicHandleItem * g = new Handle::GraphicHandleItem();
        g->setWidget(handle);
        addItem(g);

        g->setPos(handle->x(),handle->y());

        m_handles[handle] = g;
        m_items[g] = handle;

        return g;
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

}

