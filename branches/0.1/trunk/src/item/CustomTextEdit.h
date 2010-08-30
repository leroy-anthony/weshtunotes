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

#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <KTextBrowser>

class QMimeData;

namespace Item
{
    class NoteItem;

    class CustomTextEdit : public KTextBrowser
    {
        Q_OBJECT

    public:
        CustomTextEdit( NoteItem * noteItem );

        void addData( const QMimeData * source );
        void selectNone();

    public slots:
        void adaptSizeFromText();
        void openAnchor( const QUrl & url );

    protected:
        bool canInsertFromMimeData( const QMimeData * source ) const;
        void insertFromMimeData( const QMimeData * source );

        void contextMenuEvent ( QContextMenuEvent * event );

    private:
        NoteItem * m_noteItem;

    };

}

#endif // CUSTOMTEXTEDIT_H
