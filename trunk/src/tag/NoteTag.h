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

#ifndef NOTETAG_H
#define NOTETAG_H

#include <QWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QLabel>

#include "../tag/State.h"

namespace Item
{
    class NoteItem;
}

namespace Tag
{

    class NoteTag : public QWidget
    {
        Q_OBJECT

    public:
        NoteTag( Item::NoteItem * noteItem = 0, const QString & name = QString("default") );
        ~NoteTag();

        void load( const QString & name );
        void save();

        Item::NoteItem * noteItem();

        void apply();

        const QString & name();
        void setName( const QString & name );

        void addState( State * state );
        const QList<State*> & states();
        State * currentState();
        const QString & currentStateName();
        void setCurrentState( const QString & stateName );

        static NoteTag * newTag();

    protected:
        void mousePressEvent( QMouseEvent * e );

    private:
        void loadSymbol();
        void loadTagsMenu();

        QMenu m_menu;

        Item::NoteItem * m_noteItem;
        QList<State*> m_states;
        State  * m_currentState;

        QString m_name;

        QHBoxLayout * m_symbolLayout;
        QLabel m_symbol;
        int m_sizeSymbol;
    };

}

#endif // NOTETAG_H
