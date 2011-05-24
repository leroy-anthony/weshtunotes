/*
    Copyright (c) 2011 LEROY Anthony <leroy.anthony@gmail.com>

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

#ifndef PLASMASCENE_H
#define PLASMASCENE_H

#include "../scene/AbstractScene.h"

#include <KDE/Plasma/Corona>

class QEvent;
class QGraphicsSceneMouseEvent;

namespace Plasma
{
    class Containment;
}

namespace Scene
{

    class PlasmaScene : public Scene::AbstractScene
    {

    public:
        PlasmaScene();

        QGraphicsScene * scene();

        Item::AbstractItem * currentAbstractItem();
        Handle::HandleItem * currentHandle();

        void addItemToScene( Handle::GraphicHandleItem * item );
        QGraphicsProxyWidget * addHandleToScene( Handle::HandleItem * handle );
        Handle::HandleItem * newHandle( int x, int y );

        Handle::HandleItem * addData( const QMimeData * data );

        void save();
        void load( const QString & fileName );

    public slots:
        void delItem( Handle::HandleItem * h );
        void moveItem( Handle::HandleItem * handleItem, int x, int y );

    protected:
        bool eventFilter(QObject *obj, QEvent *event);
        void mousePressEvent( QGraphicsSceneMouseEvent * mouseEvent );

    private:
        Plasma::Corona m_corona;

    };

}

#endif // PLASMASCENE_H
