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

#ifndef FREESCENE_H_
#define FREESCENE_H_

#include <QHash>

#include "AbstractScene.h"

class QGraphicsProxyWidget;
class QToolBar;
class QGraphicsLinearLayout;

namespace Handle
{
    class GraphicHandleItem;
}

namespace Scene
{

    class FreeScene : public Scene::AbstractScene
    {
        Q_OBJECT

    public:
        FreeScene();
        FreeScene( const QString & id );

        enum Mode { Nothing, MoveItem, ScaleXItem, ScaleYItem, ScaleXYItem };

        Item::AbstractItem * currentAbstractItem();
        Handle::HandleItem * currentHandle();

        void addItemToScene( Handle::GraphicHandleItem * item );
        Handle::HandleItem * addData( const QMimeData * data );
        Handle::HandleItem * addItems( int x, int y );

        void save( const QString & id, const QString & fileName );


    public slots:
        void moveItem( Handle::HandleItem * handleItem, int x, int y );
        void delItem( Handle::HandleItem * h );


    protected:
        bool eventFilter(QObject *obj, QEvent *event);
        void mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent );
        void mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent);

        virtual void removeGraphicsItemFromScene( Handle::HandleItem * handle, bool animated );
        Handle::GraphicHandleItem * addHandleToScene( Handle::HandleItem * handle );
        Handle::HandleItem * newHandle( int x, int y );
        Item::AbstractItem * newItem();
        void delUselessHandleGroup( Handle::HandleItem * currentHandle  );
        void deleteGraphicsItemFromScene( QGraphicsProxyWidget * g );

        Mode m_modeItem;
        Handle::HandleItem * m_currentHandle;
        Item::AbstractItem * m_currentAbstractItem;


    private:
        QPointF m_mouseLocalPositionItem;
        QToolBar * m_textToolBar;


    private slots:
        void editItem( Item::AbstractItem * item );
        void deleteGraphicsItemFromSceneAnimated();

    };

}

#endif /*FREESCENE_H_*/
