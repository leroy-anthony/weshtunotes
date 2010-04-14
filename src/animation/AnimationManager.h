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

#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <QList>

class QGraphicsItem;
class QGraphicsWidget;
class QGraphicsView;
class QAbstractAnimation;

namespace Animation
{

    class AnimationManager
    {
    public:
        static void initAnimation( QGraphicsView * view );

        static void startLoad( QGraphicsWidget * graphicsWidget );
        static void startLoad( const QList<QGraphicsItem*> & graphicsWidgets );

        static QAbstractAnimation * startCreate( QGraphicsWidget * graphicsWidget );
        static QAbstractAnimation * startDelete( QGraphicsWidget * graphicsWidget );

    protected:
        AnimationManager( QGraphicsView * view );

    private:
        static QGraphicsView * m_view;

    };

}

#endif // ANIMATIONMANAGER_H
