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

#ifndef TAGFACTORY_H
#define TAGFACTORY_H

#include "tag_factory.h"

#include <QMap>
#include <QDialog>
#include <QMenu>

namespace Tag
{
    class State;
    class NoteTag;

    class TagFactory : public QDialog, public Ui::TagFactory
    {
        Q_OBJECT

    public:
        static TagFactory * newTagFactory( QWidget * parent = 0 );
        void loadTags();

        static void showMenuTag();
        static QStringList tagsNames();

    protected:
        TagFactory( QWidget * parent = 0 );

    public slots:
        void ok();
        void quit();

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

        void tagApply( QAction * action );

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
