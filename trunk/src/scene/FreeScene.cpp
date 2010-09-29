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
#include <QPropertyAnimation>
#include <QAnimationGroup>

#include "settings.h"
#include "../handle/HandleItem.h"
#include "../config/Configuration.h"
#include "../handle/GraphicHandleItem.h"
#include "../animation/AnimationManager.h"

namespace Scene
{
    QString FreeScene::type = "freescene";

    FreeScene::FreeScene(QWidget * parent) :
            AbstractScene(parent,FreeScene::type),
            m_currentAbstractItem(0),
            m_currentHandle(0),
            m_modeItem(Nothing)
    {
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
        if ( m_currentAbstractItem == 0 )
        {
            QGraphicsView * view = views()[0];

            QPointF pt = view->mapToScene( view->viewport()->width()/2, view->viewport()->height()/2 );
            int x = (rand() % 30 + 1)*(rand() % 2 == 0 ? -1 : 1 );
            int y = (rand() % 30 + 1)*(rand() % 2 == 0 ? -1 : 1 );
            pt += QPointF(x,y);

            Handle::HandleItem * handle = newHandle( pt.x(), pt.y(), Settings::widthNote() );

            Item::AbstractItem * item = newItem( pt.x(), pt.y() );

            handle->add( item );

            addHandleToScene( handle );

            item->load( data );
        }
        else
        {
            m_currentAbstractItem->insertData( data );
        }
    }

    Handle::HandleItem * FreeScene::newHandle( int x, int y, int w  )
    {
        Handle::HandleItem * handle = new Handle::HandleItem( this, x, y, w );
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
    
    Handle::HandleItem *  FreeScene::addItems( int x, int y )
    {
        Handle::HandleItem * handle = newHandle( x, y, Settings::widthNote() );
        
        Item::AbstractItem * item = newItem( x, y );

        handle->add( item );

        Handle::GraphicHandleItem  * g = addHandleToScene( handle );

        QMimeData data;
        data.setHtml(Settings::textItem());
        item->insertData( &data );

        Animation::AnimationManager::startCreate( g );

        return handle;
    }

    void FreeScene::addItemToScene( Handle::GraphicHandleItem * g )
    {
        addItem(g);

        Handle::HandleItem * handle = (Handle::HandleItem *) g->widget();

        g->setPos( handle->x(), handle->y() );
        g->setZValue( 1 );

        m_handles[handle] = g;
        m_items[g] = handle;
    }

    Handle::GraphicHandleItem * FreeScene::addHandleToScene( Handle::HandleItem * handle )
    {
        QGraphicsProxyWidget * w = m_handles[handle];
        if ( w != 0 )
        {
            m_items.remove(w);
            m_handles.remove(handle);
        }

        Handle::GraphicHandleItem * g = new Handle::GraphicHandleItem();
        g->setFlag(QGraphicsItem::ItemIsSelectable,true);
        g->setFlag(QGraphicsItem::ItemIsFocusable,true);
        g->setWidget(handle);

        addItem(g);

        g->setPos( handle->x(), handle->y() );
        g->setZValue( 1 );

        m_handles[handle] = g;
        m_items[g] = handle;

        return g;
    }

    void FreeScene::editItem( Item::AbstractItem * item )
    {
        m_currentAbstractItem = item;
    }

    void FreeScene::moveItem( Handle::HandleItem * handleItem, int x, int y )
    {
        m_modeItem = MoveItem;

        if ( handleItem->parent() == 0 )
        {
            static_cast<QWidget*>(handleItem)->move(x,y);

            if ( m_lastCibleHandle != 0 )
            {
                m_lastCibleHandle->resetInsert();
            }

            QList<QGraphicsItem*> items = collidingItems( m_handles[handleItem] );
            if ( items.size() > 0 )
            {
                for ( int i=0 ; i<items.size() ; ++i )
                {
                    if ( items[i]->isVisible() )
                    {
                        m_lastCibleHandle = m_items[static_cast<QGraphicsProxyWidget*>(items[i])];
                        m_lastCibleHandle->insert( handleItem->geometry().topLeft(), handleItem->height() );
                        break;
                    }
                }
            }
        }
        else
        {
            Handle::HandleItem * parentItem = handleItem->parentHandle();

            parentItem->remove( handleItem );
            handleItem->setParent(0);
            handleItem->setModeDegroupement(true);
            addHandleToScene( handleItem )->setSelected(true);

            static_cast<QWidget*>(handleItem)->move(x,y);
        }
    }

    void FreeScene::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
    {
        m_currentAbstractItem = 0;
        Handle::GraphicHandleItem * currentGraphicsItem = static_cast<Handle::GraphicHandleItem*>(itemAt( mouseEvent->scenePos().x(), mouseEvent->scenePos().y() ));

        if ( mouseEvent->button() == Qt::LeftButton )
        {
            Handle::HandleItem * h = m_items[ static_cast<QGraphicsProxyWidget*>(currentGraphicsItem) ];
            if ( h != 0 )
            {
                if ( mouseEvent->modifiers() != Qt::ControlModifier )
                {
                    clearSelection();
                }

                currentGraphicsItem->setSelected(true);
                currentGraphicsItem->setFocus();

                m_currentHandle = h->handleItemAt( mouseEvent->scenePos().x(), mouseEvent->scenePos().y() );
                currentGraphicsItem->setZValue( 100 );
            }
        }
        else if ( currentGraphicsItem == 0 )
        {
            if ( !m_readOnly )
            {
                addItems( mouseEvent->scenePos().x(), mouseEvent->scenePos().y() );
            }
        }

        QGraphicsScene::mousePressEvent ( mouseEvent );
    }

    void FreeScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
    {
        if ( m_currentHandle != 0 && m_modeItem == MoveItem )
        {
            delUselessHandleGroup( m_currentHandle  );

            QList<QGraphicsItem *> items = collidingItems( m_handles[ m_currentHandle ] );
            if ( items.size() > 0 )
            {
                if ( items[0]->isVisible() )
                {
                    Handle::HandleItem * handleCible = m_items[static_cast<QGraphicsProxyWidget*>(items[0])];
                    if ( handleCible->size() == 0 )
                    {
                        QPointF pt = static_cast<QGraphicsProxyWidget*>(items[0])->pos();

                        Handle::HandleItem * handle = newHandle( pt.x(), pt.y(), Settings::widthNote() );
                        handle->setIndexInsert( handleCible->indexInsert() );
                        handle->add( handleCible );
                        handle->add( m_currentHandle );
                        addHandleToScene( handle );

                        removeGraphicsItemFromScene( handleCible, false );
                    }
                    else
                    {
                        handleCible->add( m_currentHandle );
                    }

                    if ( m_lastCibleHandle )
                    {
                        m_lastCibleHandle->resetInsert();
                    }

                    removeGraphicsItemFromScene( m_currentHandle, false );
                }
            }

            m_currentHandle->setModeDegroupement(false);
            m_currentHandle->setHoverMode( false );

            if ( m_currentHandle->abstractItem() != 0 )
            {
                m_currentHandle->abstractItem()->isSelected();
                m_currentHandle->abstractItem()->setFocus();
            }
        }

        m_modeItem = Nothing;

        QGraphicsScene::mouseReleaseEvent ( mouseEvent );
    }

    Item::AbstractItem * FreeScene::currentAbstractItem()
    {
        return m_currentAbstractItem;
    }

    Handle::HandleItem * FreeScene::currentHandle()
    {
        return m_currentHandle;
    }

    void FreeScene::removeGraphicsItemFromScene( Handle::HandleItem * handle, bool animated )
    {
        QGraphicsProxyWidget * g = m_handles[ handle ];
        if ( g != 0 )
        {
            if ( animated )
            {
                QAbstractAnimation * animation = Animation::AnimationManager::startDelete( g );
                connect(animation, SIGNAL(finished()), this, SLOT(deleteGraphicsItemFromSceneAnimated()));
            }
            else
            {
                deleteGraphicsItemFromScene( g );
            }

            m_handles.remove( handle );
        }
    }

    void FreeScene::deleteGraphicsItemFromScene( QGraphicsProxyWidget * g )
    {
        g->clearFocus();

        removeItem(g);

        m_items.remove(g);

        g->setWidget(0);

        //FIXME: Qt bug, pb focus, bug 6544, mark as fixed but not.
        //g->deleteLater();
    }

    void FreeScene::deleteGraphicsItemFromSceneAnimated()
    {
        QAnimationGroup * animationGroup = static_cast<QAnimationGroup*>(sender());
        if ( animationGroup != 0 && animationGroup->animationCount() > 0 )
        {
            QPropertyAnimation * animation = static_cast<QPropertyAnimation*>(animationGroup->animationAt(0));
            deleteGraphicsItemFromScene( static_cast<QGraphicsProxyWidget*>(animation->targetObject()) );

            animationGroup->deleteLater();
        }
    }

    void FreeScene::delUselessHandleGroup( Handle::HandleItem * currentHandle  )
    {
        Handle::HandleItem * parentHandle = currentHandle->parentHandle();
        if (  parentHandle != 0 && currentHandle->modeDegroupement() )
        {
            if ( parentHandle->children().size() == 1 )
            {
                Handle::HandleItem * child = parentHandle->child();
                parentHandle->remove( child );
                child->setParent(0);
                child->setParentHandle(0);
                child->clearFocus();
                parentHandle->add( child->abstractItem() );

                //FIXME: Qt bug, pb focus, bug 6544, mark as fixed but not.
                //child->deleteLater();
            }

            currentHandle->setParentHandle(0);
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
            removeGraphicsItemFromScene( handleItem, true );
        }

        handleItem->setParent(0);
    }

}

