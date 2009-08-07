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

#include "GraphicHandleItem.h"

#include "../main/general.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPen>

namespace Handle
{

    GraphicHandleItem::GraphicHandleItem():
            QGraphicsProxyWidget()
    {
        setWindowFrameMargins(0,0,2,2);
    }

    void GraphicHandleItem::paint(QPainter *painter,
                                  const QStyleOptionGraphicsItem *option,
                                  QWidget *w)
    {
        QRectF r = option->rect;
        r.setX(r.x()+2);
        r.setY(r.y()+2);

        painter->save();
        painter->setOpacity(0.5);
        painter->setBrush(Qt::gray);
        painter->setPen(Qt::lightGray);
        painter->drawRect(r);
        painter->restore();

        QGraphicsProxyWidget::paint(painter,option,w);
    }

}
