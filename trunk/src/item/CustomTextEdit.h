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

#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <ktextbrowser.h>
#include <krichtextedit.h>

#include <QMimeData>
#include <QTime>

namespace Item
{
    class NoteItem;

    class CustomTextEdit : public KTextBrowser
    {
        Q_OBJECT

    public:
        CustomTextEdit( NoteItem * noteItem );

        void addData( const QMimeData *source );
        void selectNone();

    public slots:
        void adaptSizeFromText();
        void openAnchor( const QUrl & url );

    protected:
        bool canInsertFromMimeData( const QMimeData *source ) const;
        void insertFromMimeData( const QMimeData *source );

        void mousePressEvent( QMouseEvent * event );
        void contextMenuEvent ( QContextMenuEvent * event );

        void paintEvent( QPaintEvent * event );

    private:
        NoteItem * m_noteItem;

    };

}

#endif // CUSTOMTEXTEDIT_H
