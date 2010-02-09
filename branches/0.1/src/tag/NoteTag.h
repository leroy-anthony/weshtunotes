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

#include <kpushbutton.h>

#include "../tag/State.h"

namespace Item
{
    class NoteItem;
}

namespace Tag
{

    class NoteTag : public KPushButton
    {
        Q_OBJECT

    public:
        NoteTag( Item::NoteItem * noteItem, const QString & name = QString("default") );
        ~NoteTag();

        void load( const QString & name );
        void save();

        Item::NoteItem * noteItem();

        void apply();

        const QString & name();
        void setName( const QString & name );

        void addState( State * state );
        void removeState( State * state );

        const QList<State*> & states();
        State * currentState();
        const QString & currentStateName();
        void setCurrentState( const QString & stateName );

        static NoteTag * newTag( const QString & name = QString("New tag") );

        void setVisibleTag( bool visible );

    public slots:
        void nextState();

    protected:
        void mouseReleaseEvent ( QMouseEvent * event );

    private:
        void loadSymbol();

        Item::NoteItem * m_noteItem;
        QList<State*> m_states;
        State * m_currentState;

        QString m_name;

        int m_index;

        bool m_visible;
        int m_sizeSymbol;
    };

}

#endif // NOTETAG_H
