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

#ifndef ABSTRACTITEM_H_
#define ABSTRACTITEM_H_

#include <QWidget>
#include <QString>
#include <QList>

#include "IStorageData.h"

#include "../tag/AddTag.h"

namespace Tag
{
    class NoteTag;
}

namespace Item
{

    class AbstractItem :  public QWidget
    {
        Q_OBJECT

    public:
        virtual ~AbstractItem();

        const QList<QString> operationInterfaces();
        const QColor & color();

        void setItemId( const QString & id );

        void setVisibleAddTag( bool visible );

        virtual void adaptSize();

        virtual void save( const QString & fileName, const QString & handleId  ) = 0;
        virtual void load( const QString & fileName ) = 0;
        virtual void load( const QMimeData * data ) = 0;

        void setItemColor( const QColor & color );
        const QColor & itemColor();

    signals:
        void editItem( Item::AbstractItem * item );
        void sizeChange();
        void colorChange();

    public slots:
        virtual void isSelected();

    protected:
        AbstractItem( QWidget * parent );

        QList<QString> m_operationInterfaces;
        QColor m_color;

        QString m_itemId;
        static int m_id;

        Tag::AddTag * m_addTag;

    };

}

#endif // ABSTRACTITEM_H_
