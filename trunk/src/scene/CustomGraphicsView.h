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

#include "../handle/GraphicHandleItem.h"

namespace Scene
{

    class CustomGraphicsView : public QGraphicsView
    {
        Q_OBJECT

    public:
        CustomGraphicsView();

    public slots:
        void paste();
        void copy();

        void deleteItem();

        void resetZoom();
        void doubleZoom();
        void halfZoom();
        void fitInViewZoom();
        void centerZoom();

        void nextItem();
        void prevItem();

    protected:
        void wheelEvent( QWheelEvent * e );
        void mousePressEvent( QMouseEvent * event );
        void mouseReleaseEvent( QMouseEvent * event );
        void mouseMoveEvent( QMouseEvent * event );
        void resizeEvent ( QResizeEvent * event );

    private:
        void prevOrNextItem( bool next );

        double m_scale;
        bool m_move;
        bool m_selection;
        int m_indexItem;

        QGraphicsPathItem * m_selectionItem;
        QPointF m_position;

        QList<Handle::GraphicHandleItem*> m_copyHandlesItem;

    };

}

#endif // CUSTOMGRAPHICSVIEW_H
