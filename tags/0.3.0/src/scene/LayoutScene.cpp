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
#include <QGraphicsLinearLayout>

#include "settings.h"
#include "../handle/HandleItem.h"
#include "../config/Configuration.h"
#include "../handle/GraphicHandleItem.h"
#include "../item/NoteItem.h"

namespace Scene
{
    LayoutScene::LayoutScene( const QString & id ) :
            FreeScene(id)
    {
        setSceneRect(0,0,1600,INT_MAX);

        m_type = "layoutscene";

        m_layout = new QGraphicsLinearLayout( Qt::Vertical, 0 );

        m_form = new QGraphicsWidget();
        m_form->setLayout(m_layout);
        m_form->setFlag(QGraphicsItem::ItemIsMovable,false);
        m_form->setFlag(QGraphicsItem::ItemIsSelectable,false);
        m_form->setFlag(QGraphicsItem::ItemIsFocusable,false);

        m_readOnly = true;

        addItem(m_form);
    }

    LayoutScene::LayoutScene() :
            FreeScene()
    {
        setSceneRect(0,0,1600,INT_MAX);

        m_type = "layoutscene";

        m_layout = new QGraphicsLinearLayout( Qt::Vertical, 0 );

        m_form = new QGraphicsWidget();
        m_form->setLayout(m_layout);
        m_form->setFlag(QGraphicsItem::ItemIsMovable,false);
        m_form->setFlag(QGraphicsItem::ItemIsSelectable,false);
        m_form->setFlag(QGraphicsItem::ItemIsFocusable,false);

        m_readOnly = true;

        addItem(m_form);
    }
    
    LayoutScene::~LayoutScene()
    {
    }

    Handle::GraphicHandleItem * LayoutScene::addHandleToScene( Handle::HandleItem * handle )
    {
        QGraphicsProxyWidget * w = m_handles[handle];
        if ( w != 0 )
        {
            m_layout->removeItem(w);
        }

        Handle::GraphicHandleItem * g = FreeScene::addHandleToScene( handle );

        m_layout->addItem(g);



        return g;
    }

    void LayoutScene::moveItem( Handle::HandleItem * handleItem, int x, int y )
    {
        FreeScene::moveItem( handleItem, x, y );

        QGraphicsProxyWidget * g = m_handles.value( handleItem );
        m_layout->removeItem(g);
    }

    void LayoutScene::mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent)
    {       
        if ( FreeScene::currentHandle() != 0 && m_modeItem == MoveItem )
        {
            delUselessHandleGroup( FreeScene::currentHandle()  );

            QList<QGraphicsItem*> items = collidingItems( m_handles.value( FreeScene::currentHandle() ) );
            if ( items.size() > 1 )
            {
                for ( int i=0 ; i<items.size() ; ++i )
                {
                    Handle::HandleItem * handleCible = m_items[static_cast<QGraphicsProxyWidget*>(items[i])];
                    if ( items[i]->isVisible() && handleCible != 0 && !handleCible->isPin() )
                    {
                        if ( handleCible->nbItems() == 0 )
                        {
                            QPointF pt = static_cast<QGraphicsProxyWidget*>(items[i])->pos();

                            Handle::HandleItem * handle = newHandle( pt.x(), pt.y(), Settings::widthNote() );

                            handle->setIndexInsert( handleCible->indexInsert() );

                            handle->add( handleCible );
                            handle->add( FreeScene::currentHandle() );
                            addHandleToScene( handle );

                            handle->setIndexInsert(-1);

                            removeGraphicsItemFromScene( handleCible, false );
                        }
                        else
                        {
                            handleCible->add( FreeScene::currentHandle() );
                        }

                        if ( m_lastCibleHandle )
                        {
                            m_lastCibleHandle->resetInsert();
                        }

                        removeGraphicsItemFromScene( FreeScene::currentHandle(), false );
                        break;
                    }
                }
            }
            else
            {
                m_layout->addItem( m_handles.value(FreeScene::currentHandle()) );
            }

            FreeScene::currentHandle()->setModeDegroupement(false);
            FreeScene::currentHandle()->setHoverMode( false );

            if ( FreeScene::currentHandle()->abstractItem() != 0 )
            {
                FreeScene::currentHandle()->abstractItem()->isSelected();
                FreeScene::currentHandle()->abstractItem()->setFocus();
            }
        }
    }

    void LayoutScene::delItem( Handle::HandleItem * handleItem )
    {
        QGraphicsProxyWidget * g = m_handles[ handleItem ];
        if ( g != 0 )
        {
            m_layout->removeItem(g);
        }

        FreeScene::delItem(handleItem);
    }

    bool LayoutScene::hasZoomAbilities()
    {
        return false;
    }

}

