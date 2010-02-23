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

#ifndef LAYOUTSCENE_H_
#define LAYOUTSCENE_H_

#include "AbstractScene.h"

#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QHash>
#include <QToolBar>
#include <QGraphicsLinearLayout>

namespace Scene
{

    class LayoutScene : public Scene::AbstractScene
    {
        Q_OBJECT

    public:
        LayoutScene( QWidget * parent  = 0 );
        ~LayoutScene();

        enum Mode { Nothing, MoveItem, ScaleXItem, ScaleYItem, ScaleXYItem };

        QGraphicsItem * currentGraphicsItem();
        Item::AbstractItem * currentAbstractItem();
        Handle::HandleItem * currentHandle();

        void addData( const QMimeData * data );
        Handle::HandleItem * addItems( int x, int y, const QString & dataFile  );
        void delItem( Handle::HandleItem * h );

        void save( const QString & id, const QString & fileName );

        static QString type;

    protected:
        void mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent );
        void mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent);
        void removeGraphicsItemFromScene( Handle::HandleItem * handle );
        QGraphicsProxyWidget * addHandleToScene( Handle::HandleItem * handle );
        Handle::HandleItem * newHandle( int x, int y, int w );
        Item::AbstractItem * newItem( int x, int y );



    private:
        QGraphicsItem * m_currentGraphicsItem;
        Item::AbstractItem * m_currentAbstractItem;
        Handle::HandleItem * m_currentHandle;
        QPointF m_mouseLocalPositionItem;

        QHash<QGraphicsProxyWidget * ,Handle::HandleItem*> m_items;
        QToolBar * m_textToolBar;
        Mode m_modeItem;

        QGraphicsWidget * m_form;
        QGraphicsLinearLayout * m_layout;

    private slots:
        void editItem( Item::AbstractItem * item );

    public slots:
        void moveItem( Handle::HandleItem * handleItem, int x, int y );
        void resize( QResizeEvent * event );

    };

}

#endif /*LAYOUTSCENE_H_*/
