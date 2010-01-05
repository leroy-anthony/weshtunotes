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

#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QDragEnterEvent>

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

    protected:
        void wheelEvent( QWheelEvent * e );
        void mousePressEvent( QMouseEvent * event );
        void mouseReleaseEvent( QMouseEvent * event );
        void mouseMoveEvent( QMouseEvent * event );
        void resizeEvent ( QResizeEvent * event );

    private:
        double m_scale;
        bool m_move;
        bool m_selection;
        
        QGraphicsPathItem * m_selectionItem;
        QPointF m_position;

        QList<Handle::GraphicHandleItem*> m_copyHandlesItem;

    };

}

#endif // CUSTOMGRAPHICSVIEW_H
