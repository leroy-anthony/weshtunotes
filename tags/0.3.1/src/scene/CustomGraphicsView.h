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

#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QTimer>
#include <QGraphicsScene>

#include "../handle/GraphicHandleItem.h"

namespace Scene
{
    class AbstractScene;

    class CustomGraphicsView : public QGraphicsView
    {
        Q_OBJECT

    public:
        CustomGraphicsView();

        void setScene( AbstractScene * scene );

        bool hasZoomAbilities();

        void adjustLayoutSize();
        void adjustLayoutSize( int width, int height );

    public slots:
        void paste();
        void copy();

        void deleteItem();

        void resetZoom( bool smooth = false );
        void doubleZoom( bool smooth = false );
        void halfZoom( bool smooth = false );
        void fitInViewZoom();
        void centerZoom( bool smooth = false );

        void nextItem();
        void prevItem();

    protected:
        void wheelEvent( QWheelEvent * e );
        void mousePressEvent( QMouseEvent * event );
        void mouseReleaseEvent( QMouseEvent * event );
        void mouseMoveEvent( QMouseEvent * event );

        void timerEvent( QTimerEvent * ev );

        void resizeEvent( QResizeEvent * event );

    private:
        void prevOrNextItem( bool next );
        void setScale( double scale );

        double m_scale;
        double m_scaleStep;

        double m_x;
        double m_y;

        bool m_move;
        bool m_selection;
        int m_indexItem;

        QTimer m_timer;
        int m_idTimer;

        QGraphicsPathItem * m_selectionItem;
        QPointF m_position;

        QList<Handle::GraphicHandleItem*> m_copyHandlesItem;

        AbstractScene * m_scene;

    };

}

#endif // CUSTOMGRAPHICSVIEW_H
