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

#ifndef ABSTRACTSCENE_H_
#define ABSTRACTSCENE_H_

namespace Handle
{
    class HandleItem;
}

#include <QGraphicsScene>
#include <QList>
#include <QAction>
#include <QMimeData>

#include "../item/AbstractItem.h"
#include "../scene/CustomGraphicsView.h"

namespace Scene
{

    class AbstractScene : public QGraphicsScene
    {
        Q_OBJECT

    public:
        AbstractScene();
        virtual ~AbstractScene();

        virtual QGraphicsItem      * currentGraphicsItem() = 0;
        virtual Item::AbstractItem * currentAbstractItem() = 0;
        virtual Handle::HandleItem * currentHandle()       = 0;

        virtual void load( const QString & fileName );
        virtual void save( const QString & id, const QString & fileName );

        virtual void saveViewOnDisk( const QString & fileName );
        virtual void loadViewFromDisk( const QString & fileName );
        virtual void storeView( CustomGraphicsView * view );
        virtual void restoreView( CustomGraphicsView * view );

        virtual void addData( const QMimeData * data );

        const QString & id();

    public slots:
        virtual void delItem( Handle::HandleItem * h ) = 0;
        virtual void moveItem( Handle::HandleItem * handleItem, int x, int y ) = 0;

    protected:
        bool canInsertFromMimeData( const QMimeData *source ) const;
        void insertFromMimeData( const QMimeData *source );

        //QString m_sceneId;
        QString m_id;

    };

}

#endif // ABSTRACTSCENE_H_
