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

#include "LayoutScene.h"

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
#include <QGraphicsItem>

#include "settings.h"
#include "../handle/HandleItem.h"
#include "../config/Configuration.h"
#include "../handle/GraphicHandleItem.h"
#include "../item/NoteItem.h"

namespace Scene
{
    QString LayoutScene::type = "listscene";
    
    LayoutScene::LayoutScene(QWidget * parent) :
            AbstractScene(),
            m_currentGraphicsItem(0),
            m_currentAbstractItem(0),
            m_currentHandle(0),
            m_modeItem(Nothing)
    {
        m_type = LayoutScene::type;

        setBackgroundBrush(QColor(Qt::cyan).lighter(190));
        /*
          QGraphicsScene::BspTreeIndex
          QGraphicsScene::NoIndex
          */
        setItemIndexMethod(QGraphicsScene::NoIndex);

        setSceneRect(0,0,1000,200000);

        m_layout = new QGraphicsLinearLayout( Qt::Vertical, 0 );

        m_form = new QGraphicsWidget();
        m_form->setLayout(m_layout);
        m_form->setPreferredSize(1000,300);
        m_form->setFlag(QGraphicsItem::ItemIsMovable,false);
        m_form->setFlag(QGraphicsItem::ItemIsSelectable,false);
        m_form->setFlag(QGraphicsItem::ItemIsFocusable,false);

        addItem(m_form);
    }
    
    LayoutScene::~LayoutScene()
    {
    }

    void LayoutScene::resize( QResizeEvent * event )
    {
        m_form->resize( event->size() );
        setSceneRect(0,0,event->size().width(),200000);
    }
    
    void LayoutScene::addData( const QMimeData * data )
    {
        QGraphicsView * view = views()[0];
        QPointF pt = view->mapToScene( view->viewport()->width()/2-200, view->viewport()->height()/2 );

        Handle::HandleItem * handle = newHandle( pt.x(), pt.y(), Settings::widthNote() );

        Item::AbstractItem * item = newItem( pt.x(), pt.y() );

        handle->add( item );

        addHandleToScene( handle );

        item->load( data );

        static_cast<QWidget*>(handle)->resize( item->width(), item->height() );
    }

    Handle::HandleItem * LayoutScene::newHandle( int x, int y, int w )
    {
        Handle::HandleItem * handle = new Handle::HandleItem( this, x, y, w );
        connect( handle, SIGNAL(move(Handle::HandleItem*,int,int)), this, SLOT(moveItem(Handle::HandleItem*,int,int)));
        connect( handle, SIGNAL(delItem(Handle::HandleItem*)), this, SLOT(delItem(Handle::HandleItem*)));
        
        return handle;
    }
    
    Item::AbstractItem * LayoutScene::newItem( int x, int y )
    {
        Item::NoteItem * item = new Item::NoteItem();
        connect( item, SIGNAL(editItem(Item::AbstractItem*)), this, SLOT(editItem(Item::AbstractItem*)));
        
        return item;
    }
    
    Handle::HandleItem *  LayoutScene::addItems( int x, int y, const QString & dataFile )
    {
        Handle::HandleItem * handle = newHandle( x, y, Settings::widthNote() );
        
        Item::AbstractItem * item = newItem( x, y );
        //QColor(115,115,115)

        handle->add( item );

        addHandleToScene( handle );

        item->load( dataFile );

        return handle;
    }

    QGraphicsProxyWidget * LayoutScene::addHandleToScene( Handle::HandleItem * handle )
    {
        QGraphicsProxyWidget * w = m_handles[handle];
        if ( w != 0 )
        {
            m_items.remove(w);
            m_handles.remove(handle);
        }

        Handle::GraphicHandleItem * g = new Handle::GraphicHandleItem();
        g->setWidget(handle);
        //addItem(g);
        m_layout->addItem(g);

        g->setPos(handle->x(),handle->y());

        m_handles[handle] = g;
        m_items[g] = handle;

        return g;
    }

    void LayoutScene::removeGraphicsItemFromScene( Handle::HandleItem * handle )
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

    void LayoutScene::editItem( Item::AbstractItem * item )
    {
        m_currentAbstractItem = item;
        // récupère les interfaces pour les opérations et désactive/active les boutons
    }

    void LayoutScene::moveItem( Handle::HandleItem * handleItem, int x, int y )
    {/*
        m_modeItem = MoveItem;

        if ( handleItem->parentHandle() == 0 )
        {
            static_cast<QWidget*>(handleItem)->move(x,y);

            QList<QGraphicsItem *> items = collidingItems( m_handles[handleItem] );

            if ( items.size() > 1 )
            {
                int index = 0;
                if ( items[0] == m_form )
                {
                    index = 1;
                }
                m_items[static_cast<QGraphicsProxyWidget*>(items[index])]->insert( handleItem->geometry().topLeft(), handleItem->height() );
            }
            else
            {
                //Handle::HandleItem::resetInsert();
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
        }*/
    }

    void LayoutScene::delItem( Handle::HandleItem * handleItem )
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

    void LayoutScene::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
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

    void LayoutScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
    {
        if ( m_currentHandle != 0 && m_modeItem == MoveItem )
        {
            QList<QGraphicsItem *> items = collidingItems( m_currentGraphicsItem );
            if ( items.size() > 1 )
            {
                int index = 0;
                if ( items[0] == m_form )
                {
                    index = 1;
                }

                Handle::HandleItem * handleCible = m_items[static_cast<QGraphicsProxyWidget*>(items[index])];

                if ( handleCible->size() == 0 )
                {
                    QPointF pt = static_cast<QGraphicsProxyWidget*>(items[index])->pos();

                    Handle::HandleItem * handle = newHandle( pt.x(), pt.y(), Settings::widthNote() );
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
                //Handle::HandleItem::resetInsert();
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

    Item::AbstractItem * LayoutScene::currentAbstractItem()
    {
        return m_currentAbstractItem;
    }

    QGraphicsItem * LayoutScene::currentGraphicsItem()
    {
        return m_currentGraphicsItem;
    }

    Handle::HandleItem * LayoutScene::currentHandle()
    {
        return m_currentHandle;
    }

}

