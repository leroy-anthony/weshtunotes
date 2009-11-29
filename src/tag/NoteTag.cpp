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

#include "NoteTag.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QStylePainter>
#include <QStyleOptionButton>
#include <QtSvg>
#include <QWidgetAction>

#include "../item/NoteItem.h"
#include "../main/general.h"
#include "../tag/TagFactory.h"
#include "../config/ImageFactory.h"
#include "../config/VisualAspect.h"

namespace Tag
{

    NoteTag::NoteTag( Item::NoteItem * noteItem, const QString & name ):
            KPushButton(noteItem),
            m_noteItem(noteItem),
            m_currentState(0),
            m_name(name),
            m_visible(false),
            m_sizeSymbol(32),
            m_index(0)
    {
        setFlat(true);
        setContentsMargins( 2, 2, 2, 2 );
        load(name);

        setStyleSheet(QString("background-color: %1").arg(Qt::transparent));
    }

    NoteTag::~NoteTag()
    {
        for ( int i=0 ; i<m_states.size() ; ++i )
        {
            delete m_states[i];
        }
    }

    void NoteTag::apply()
    {
        if ( m_currentState != 0 )
        {
            m_currentState->apply();
        }
    }

    Item::NoteItem * NoteTag::noteItem()
    {
        return m_noteItem;
    }

    const QString & NoteTag::name()
    {
        return m_name;
    }

    void NoteTag::setName( const QString & name )
    {
        if ( m_name != "default" )
        {
            Config::Configuration settings("tags");
            settings.removeGroup( m_name );
        }
        m_name = name;
    }

    void NoteTag::addState( State * state )
    {
        m_states << state;
    }

    void NoteTag::removeState( State * state )
    {
        m_states.removeAll( state );
    }

    const QList<State*> & NoteTag::states()
    {
        return m_states;
    }

    State * NoteTag::currentState()
    {
        return m_currentState;
    }

    const QString & NoteTag::currentStateName()
    {
        return m_currentState->name();
    }

    void NoteTag::setCurrentState( const QString & stateName )
    {
        for ( int i=0 ; i<m_states.size() ; ++i )
        {
            if ( m_states[i]->name() == stateName )
            {
                m_currentState = m_states[i];
                loadSymbol();
            }
        }
    }

    void NoteTag::loadSymbol()
    {        
        if ( m_name != "default" && m_currentState != 0 )
        {
            QString symbol = m_currentState->symbol();
            if ( symbol != "" )
            {
                setIconSize(QSize(m_sizeSymbol,m_sizeSymbol));
                setIcon( Config::ImageFactory::newInstance()->icon(m_currentState->symbol()) );
                m_visible = true;
            }
            else
            {
                setFixedSize( 0, 0 );
            }
        }
    }

    void NoteTag::load( const QString & name )
    {
        Config::Configuration settings("tags");

        m_name = name;

        QStringList states = settings.values( m_name,"states" );
        for ( int i=0 ; i<states.size() ; ++i )
        {
            State * s = new State(this);
            s->load( settings, m_name, states[i] );
            m_states << s;
        }

        if ( states.size() > 0 )
        {
            m_currentState = m_states[0];
            m_index = 0;
        }

        loadSymbol();
    }

    void NoteTag::save()
    {
        Config::Configuration settings( "tags" );

        QStringList namesStates;
        for ( int i=0 ; i<m_states.size() ; ++i )
        {
            namesStates << m_states[i]->name();
        }

        settings.setValue( m_name, "states", namesStates );
    }

    NoteTag * NoteTag::newTag( const QString & name )
    {
        NoteTag * tag = new NoteTag( 0, name );
        State * state = State::newState( tag );
        tag->addState( state );
        tag->setCurrentState( state->name() );

        tag->save();
        state->save();

        return tag;
    }

    void NoteTag::setVisibleTag( bool visible )
    {
        m_visible = visible;
        setVisible( visible );
    }

    void NoteTag::mouseReleaseEvent ( QMouseEvent * event )
    {
        m_index++;
        if ( m_states.size() == m_index )
        {
            m_index=0;
        }

        if ( m_states.size() > m_index )
        {
            setCurrentState( m_states[m_index]->name() );
            m_noteItem->tagApply();
        }

        QPushButton::mouseReleaseEvent(event);
    }

}
