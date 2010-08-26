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

#ifndef GRAPHICHANDLEITEM_H
#define GRAPHICHANDLEITEM_H

#include <QGraphicsProxyWidget>

namespace Handle
{
    class HandleItem;

    class GraphicHandleItem : public QGraphicsProxyWidget
    {
    public:
        GraphicHandleItem();

        virtual void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

        HandleItem * handleItem();

    };

}

#endif // GRAPHICHANDLEITEM_H
