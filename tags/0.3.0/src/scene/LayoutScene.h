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

#ifndef LAYOUTSCENE_H_
#define LAYOUTSCENE_H_

#include "FreeScene.h"

#include <QGraphicsProxyWidget>
#include <QHash>
#include <QToolBar>
#include <QGraphicsLinearLayout>

namespace Scene
{

    class LayoutScene : public FreeScene
    {
        Q_OBJECT

    public:
        LayoutScene();
        LayoutScene( const QString & id );

        ~LayoutScene();

        Handle::GraphicHandleItem * addHandleToScene( Handle::HandleItem * handle );
        void delItem( Handle::HandleItem * h );

        virtual bool hasZoomAbilities();

    public slots:
        void moveItem( Handle::HandleItem * handleItem, int x, int y );

    protected:
        void mouseReleaseEvent( QGraphicsSceneMouseEvent * mouseEvent);

    };

}

#endif /*LAYOUTSCENE_H_*/
