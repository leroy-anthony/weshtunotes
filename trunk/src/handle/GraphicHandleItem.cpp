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

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPen>
#include <QApplication>
#include <QPolygon>

#include "../main/general.h"
#include "../scene/AbstractScene.h"

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

        painter->setBrush(QApplication::palette().color(QPalette::Highlight));
        painter->setPen(QApplication::palette().color(QPalette::Highlight));

        Scene::AbstractScene * s = static_cast<Scene::AbstractScene*>(scene());

        if ( option->state & QStyle::State_Selected )
        {
            int decal = 5;

            QPolygon triangle1;
            triangle1 << option->rect.topLeft() - QPoint(decal,decal) << option->rect.topLeft() - QPoint(0,decal) << option->rect.topLeft() - QPoint(decal,0);

            QPolygon triangle2;
            triangle2 << option->rect.topRight() + QPoint(0,-decal) << option->rect.topRight() + QPoint(decal,-decal) << option->rect.topRight() + QPoint(decal,0);

            QPolygon triangle3;
            triangle3 << option->rect.bottomLeft() - QPoint(decal,-decal) << option->rect.bottomLeft() + QPoint(0,decal) << option->rect.bottomLeft() - QPoint(decal,0);

            QPolygon triangle4;
            triangle4 << option->rect.bottomRight() + QPoint(decal,decal) << option->rect.bottomRight() + QPoint(0,decal) << option->rect.bottomRight() + QPoint(decal,0);

            painter->drawPolygon(triangle1);
            painter->drawPolygon(triangle2);
            painter->drawPolygon(triangle3);
            painter->drawPolygon(triangle4);
        }

        painter->setOpacity(0.3);
        painter->drawRect(r);
        painter->restore();

        QGraphicsProxyWidget::paint(painter,option,w);
    }

}
