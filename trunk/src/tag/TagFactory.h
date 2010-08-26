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

#ifndef TAGFACTORY_H
#define TAGFACTORY_H

#include "tag_factory.h"

#include <QMap>

#include "kdialog.h"

namespace Tag
{
    class State;
    class NoteTag;

    class TagFactory : public KDialog, public Ui::TagFactory
    {
        Q_OBJECT

    public:
        static TagFactory * newTagFactory( QWidget * parent = 0 );
        static QStringList tagsNames();

        void loadTags();

    protected:
        TagFactory( QWidget * parent = 0 );

    public slots:
        void ok();

        void newTag();
        void newState();
        void loadTagOrState( QTreeWidgetItem * item , int column );
        
        void del();

        void changeNameTagOrState();
        void withColorBackground( int state );
        void withIcon( int state );
        void changeItemColor( const QColor & color );
        void changeState();
        void changeFontState( const QFont & font );
        void changeTextColor( const QColor & color );
        void changeSizeFontState( int index );
        void selectIcon( const QString & icon );

    private:
        static TagFactory * m_instance;

        State * m_currentState;
        NoteTag * m_currentTag;

        QTreeWidgetItem * m_currentItemState;
        QTreeWidgetItem * m_currentItemTag;

        void loadTag( NoteTag * noteTag );
        void loadState( State * state );

        QMap<QTreeWidgetItem*,NoteTag*> m_itemToTag;
        QMap<QTreeWidgetItem*,State*> m_itemToState;

    };

}

#endif // TAGFACTORY_H
