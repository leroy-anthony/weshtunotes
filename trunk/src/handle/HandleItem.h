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

#ifndef HANDLEITEM_H_
#define HANDLEITEM_H_

#include <QSize>
#include <QColor>
#include <QWidget>

#include "../technic/GeneratorID.h"

class QGridLayout;
class QWidget;
class QPoint;
class QPaintEvent;
class QLayout;

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
    class PinHandle;
    class SizeHorHandle;
    class MoveHandle;
    class DeleteHandle;


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

        Item::AbstractItem * abstractItem();

        HandleItem * handleItemAt( int x, int y );

        void insert( QPoint pt, int height );

        void resetInsert();

        void setParentHandle(  HandleItem *  h );
        HandleItem * parentHandle();
        HandleItem * child();
        const QList<HandleItem*> & children();

        bool modeDegroupement();
        void setModeDegroupement( bool m_mode );

        void setHoverMode( bool isHover );
        bool isPin();

        void enterEvent( QEvent * event );
        void leaveEvent( QEvent * event );

        int size();

        const QColor & defaultColor();
        void setDefaultColor( const QColor & color );

        void save();

        int contentMarginX();
        int contentMarginY();

        int x();
        int y();

        int indexInsert();
        void setIndexInsert( int i );

        QString configFile() const;

    public slots:
        void setDefaultColor();
        void questionDelItem();
        void setPin( bool pin );
        void setLock();
        void contextMenuEvent( QContextMenuEvent * event );

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

        SizeHorHandle * m_sizeHorHandle;
        MoveHandle    * m_moveHandle;
        DeleteHandle  * m_deleteHandle;
        PinHandle     * m_pinHandle;
        HandleItem    * m_parentHandle;

        bool m_isHover;
        bool m_isPin;

        bool m_modeDegroupement;

        int m_index;
        QWidget * m_insertIndicator;

        int m_contentMarginX;
        int m_contentMarginY;

        int m_x;
        int m_y;
        int m_height;
        int m_width;

        QColor m_defaultColor;

    };

}

#endif /*HANDLEITEM_H_*/
