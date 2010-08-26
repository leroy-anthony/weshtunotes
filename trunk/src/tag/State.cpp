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

#include "State.h"

#include "../main/general.h"
#include "../config/Configuration.h"
#include "../config/VisualAspect.h"
#include "../tag/NoteTag.h"

namespace Tag
{

    State::State( NoteTag * tag ):
            m_tag(tag),
            m_item(tag->noteItem()),
            m_name("default")
    {
    }

    State::~State()
    {
    }

    void State::setBold( bool checked )
    {
        m_bold = checked;
        if ( m_item != 0 )
        {
            m_item->setBold( checked );
        }
    }

    void State::setItalic( bool italic )
    {
        m_italic = italic;
        if ( m_item != 0 )
        {
            m_item->setItalic( italic );
        }
    }

    void State::setAlignment( Qt::Alignment alig )
    {
        m_item->setAlignment( alig );
    }

    Qt::Alignment State::alignment()
    {
        return m_item->alignment();
    }

    void State::setFontUnderline( bool underLine )
    {
        m_underLine = underLine;
        if ( m_item != 0 )
        {
            m_item->setFontUnderline( underLine );
        }
    }

    void State::setFontStrikeOut ( bool strikeOut )
    {
        m_strikeOut = strikeOut;
        if ( m_item != 0 )
        {
            m_item->setFontStrikeOut( strikeOut );
        }
    }

    void State::setTextColor( const QColor & color )
    {
        m_colorFont = color;
        if ( m_item != 0 )
        {
            m_item->setTextColor( color );
        }
    }

    void State::setFontFamily( const QFont & font )
    {
        m_font = font.family();
        if ( m_item != 0 )
        {
            m_item->setFontFamily( font );
        }
    }

    void State::setFontPointSize( int weight )
    {
        m_weight = weight;
        if ( m_item != 0 )
        {
            m_item->setFontPointSize( weight );
        }
    }

    void State::setItem( Item::NoteItem * item )
    {
        m_item = item;
    }

    void State::setItemColor( const QColor & color )
    {
        m_colorItem = QColor(color);
        if ( m_item != 0 )
        {
            m_item->setItemColor( color );
        }
    }

    void State::setSymbol( const QString & icon )
    {
        m_symbol = icon;
    }

    const QString & State::name()
    {
        return m_name;
    }

    void State::setName( const QString & name )
    {
        if ( m_name != "default" )
        {
            Data::DataManager settings( "tags" );
            settings.removeSubGroup( m_tag->name(), m_name );
        }

        m_name = name;
    }

    void State::load()
    {
        Data::DataManager settings( "tags" );
        load( settings, m_tag->name(), m_name );
    }

    void State::load( Data::DataManager & settings, const QString & tagName, const QString & state  )
    {
        m_name = settings.valueSubGroup( tagName, state, "name", "" );

        loadTextEdition( settings, tagName, state );
        loadVisualItemEdition( settings, tagName, state );
    }

    void State::save()
    {
        Data::DataManager settings( "tags" );

        settings.setValue( m_tag->name(),m_name,"name", m_name );

        saveTextEdition( settings, m_tag->name(), m_name );
        saveVisualItemEdition( settings, m_tag->name(), m_name );
    }

    void State::apply()
    {
        setBold( m_bold );
        setItalic( m_italic );
        setAlignment( Qt::AlignLeft );
        setFontUnderline( m_underLine );
        setTextColor( m_colorFont );
        setFontFamily( m_font );
        setFontPointSize( m_weight );
        setFontStrikeOut( m_strikeOut );
        setItemColor( m_colorItem );
    }

    State * State::newState( NoteTag * tag )
    {
        State * state = new State(tag);
        state->setName( "new state" );

        QStringList names;
        for ( int i=0 ; i<tag->states().size() ; ++i )
        {
            names += tag->states()[i]->name();
        }

        int index = 1;
        while ( names.contains(state->name()) )
        {
            ++index;
            state->setName( QString("new state (%1)").arg(index) );
        }

        return state;
    }

    NoteTag * State::tag()
    {
        return m_tag;
    }

}
