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

namespace Tag
{

    NoteTag::NoteTag( Item::NoteItem * noteItem, const QString & name ):
            QLabel(noteItem),
            m_noteItem(noteItem),
            m_currentState(0),
            m_name(name),
            m_visible(false),
            m_sizeSymbol(32)
    {
        setContentsMargins( 2, 2, 2, 2 );
        load(name);
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
            settings.remove( m_name );
        }
        m_name = name;
    }

    void NoteTag::addState( State * state )
    {
        m_states << state;
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
                QPixmap pix;
                Config::ImageFactory::pixmap(m_currentState->symbol(),pix);
                setPixmap(pix.scaled(m_sizeSymbol,m_sizeSymbol));
                m_visible = true;
            }
        }
    }

    void NoteTag::load( const QString & name )
    {
        Config::Configuration settings("tags");

        m_name = name;

        settings.beginGroup(m_name);

        QStringList states = settings.value( "states" ).toStringList();
        for ( int i=0 ; i<states.size() ; ++i )
        {
            State * s = new State(this);
            settings.beginGroup(states[i]);
            s->load( settings );
            settings.endGroup();
            m_states << s;
        }

        if ( states.size() > 0 )
        {
            m_currentState = m_states[0];
        }

        loadSymbol();
    }

    void NoteTag::save()
    {
        Config::Configuration settings( "tags" );

        settings.beginGroup(m_name);

        QStringList namesStates;
        for ( int i=0 ; i<m_states.size() ; ++i )
        {
            namesStates << m_states[i]->name();
        }

        settings.setValue( "states", namesStates );

        settings.endGroup();

        settings.sync();
    }

    NoteTag * NoteTag::newTag()
    {
        NoteTag * tag = new NoteTag(0,"new tag");
        State * state = State::newState(tag);
        tag->addState( state );
        tag->setCurrentState( state->name() );

        tag->save();
        state->save();

        return tag;
    }

    void NoteTag::setVisibleTag( bool visible )
    {
        m_visible = visible;
    }

    void NoteTag::paintEvent( QPaintEvent * event )
    {
        if ( m_visible )
        {
            QLabel::paintEvent( event );
        }
    }

}