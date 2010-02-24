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

#include "IStorageData.h"
#include "../technic/GeneratorID.h"

class QString;
class QMimeData;

namespace Tag
{
    class NextTag;
    class AddTag;
    class NoteTag;
}

namespace Item
{

    class AbstractItem :  public QWidget, public Technic::GeneratorID
    {
        Q_OBJECT

    public:
        virtual ~AbstractItem();

        const QColor & color();

        void setVisibleAddTag( bool visible );

        virtual void adaptSize();

        virtual void save( const QString & fileName, const QString & handleId  ) = 0;
        virtual void load( const QString & fileName ) = 0;
        virtual void load( const QMimeData * data ) = 0;

        void setItemColor( const QColor & color );
        const QColor & itemColor();

        bool containTag( const QString & tagName );
        Tag::NoteTag * tag();

        virtual void insertData( const QMimeData * data );

        bool selected();

    signals:
        void editItem( Item::AbstractItem * item );
        void sizeChange();
        void colorChange();

    public slots:
        virtual void isSelected();
        virtual void tagApply( QAction * action ) = 0;

    protected:
        AbstractItem( QWidget * parent );

        void keyPressEvent ( QKeyEvent * keyEvent );

        QColor m_color;

        Tag::AddTag  * m_addTag;
        Tag::NextTag * m_nextTag;
        Tag::NoteTag * m_tag;

        bool m_selected;

    };

}

#endif // ABSTRACTITEM_H_
