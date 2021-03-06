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

#ifndef NOTETAG_H
#define NOTETAG_H

#include <QList>

#include <KPushButton>

#include "../tag/State.h"

class QMouseEvent;

namespace Nepomuk
{
    class Tag;
}

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

        const Nepomuk::Tag * nepomukTag();

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

        Nepomuk::Tag * m_nepomukTag;

    };

}

#endif // NOTETAG_H
