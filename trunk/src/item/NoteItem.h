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

#ifndef NOTEITEM_H_
#define NOTEITEM_H_

#include "CustomTextEdit.h"
#include "AbstractItem.h"
#include "../interface/ITextEdition.h"
#include "../interface/IVisualItemEdition.h"

class QHBoxLayout;

namespace Tag
{
    class NoteTag;
}

namespace Item
{

    class NoteItem : public Item::AbstractItem, public ITextEdition//, public IVisualItemEdition
    {
        Q_OBJECT

    public:
        NoteItem( QWidget * parent  = 0 );
        ~NoteItem();

        void load( const QMimeData * data );
        void load( const QString & fileName );
        void save( const QString & fileName, const QString & handleId  );

        void addTag( const QString & tagName, const QString & tagState );
        void tagApply();

        void removeNoteFromNepomuk();
        void addNoteToNepomuk();

        void adaptSize();

        void insertData( const QMimeData * data );

        Qt::Alignment alignment();
        
    public slots:
        void setBold( bool checked );
        void setAlignment ( Qt::Alignment a );
        void setItalic ( bool italic );
        void setFontFamily( const QFont & font );
        void setFontUnderline ( bool underline );
        void setFontStrikeOut ( bool strikeOut );
        void setFontPointSize ( int weight );
        void setTextBackgroundColor ( const QColor & c );
        void setTextColor( const QColor & c );

        void tagApply( QAction * action );

    protected:
        void paintEvent( QPaintEvent * event );

    private:
        QHBoxLayout * m_horizontalLayout;
        CustomTextEdit * m_plainTextEdit;

        QString m_fileName;

    private slots:
        void edit();

    };

}

#endif // NOTEITEM_H_
