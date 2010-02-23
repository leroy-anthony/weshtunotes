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

#ifndef HANDLEITEM_H_
#define HANDLEITEM_H_

#include <QPoint>

#include "../technic/GeneratorID.h"
#include "MoveHandle.h"
#include "SizeHorHandle.h"
#include "DeleteHandle.h"

class QGridLayout;
class QWidget;

namespace Scene
{
    class AbstractScene;
}

namespace Item
{
    class AbstractItem;
}

namespace Handle
{

    class HandleItem : public QWidget, public Technic::GeneratorID
    {
        Q_OBJECT

    public:
        HandleItem( Scene::AbstractScene * parent, int x, int y, int width );
        virtual ~HandleItem();

        void add( Item::AbstractItem * w );
        void add( HandleItem * h );
        void remove( HandleItem * h );

        void moveItem( int x, int y );

        void resize( const QSize & );

        enum Mode { Nothing, ScaleXItem };

        bool isRoot();

        Item::AbstractItem * noteItem();

        HandleItem * handleItemAt( int x, int y );

        void insert( QWidget * w, int index );
        void insert( QPoint  pt, int height );

        static void resetInsert();

        void setParentHandle(  HandleItem *  h );
        HandleItem * parentHandle();
        HandleItem * child();
        const QList<HandleItem*> & children();

        bool modeDegroupement();
        void setModeDegroupement( bool m_mode );

        void setHoverMode( bool isHover );

        void enterEvent( QEvent * event );
        void leaveEvent( QEvent * event );

        int size();

        const QColor & defaultColor();
        void setDefaultColor( const QColor & color );

        void save( const QString & fileName );

        int contentMarginX();
        int contentMarginY();

        int x();
        int y();

        void setFileName( const QString & fileName );
        const QString & fileName() const;

    public slots:
        void setDefaultColor();
        void questionDelItem();

    protected:
        void paintEvent( QPaintEvent * event );

    signals:
        void move( Handle::HandleItem * handleItem, int x, int y );
        void resize( Handle::HandleItem * handleItem, int x, int y );
        void delItem( Handle::HandleItem * handleItem );

    private:
        QGridLayout * m_handleLayout;
        QLayout * m_contentLayout;

        QList<Handle::HandleItem*> m_handles;

        Mode m_modeHandle;

        Scene::AbstractScene * m_scene;

        QPoint m_lastPos;

        Item::AbstractItem * m_item;

        SizeHorHandle m_sizeHorHandle;
        MoveHandle m_moveHandle;
        DeleteHandle m_deleteHandle;
        HandleItem * m_parentHandle;

        bool m_isHover;

        bool m_modeDegroupement;

        static int m_index;
        static QWidget * m_insertIndicator;

        int m_contentMarginX;
        int m_contentMarginY;

        int m_x;
        int m_y;
        int m_height;
        int m_width;

        QColor m_defaultColor;

        QString m_fileName;

    };

}

#endif /*HANDLEITEM_H_*/
