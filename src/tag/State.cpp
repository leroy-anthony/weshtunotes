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
            Config::Configuration settings( "tags" );
            settings.beginGroup( m_tag->name() );
            settings.remove( m_name );
            settings.remove( "states" );
            settings.sync();
        }

        m_name = name;
    }

    void State::load()
    {
        Config::Configuration settings("tags");
        settings.beginGroup(m_tag->name());
        settings.beginGroup(m_name);
        load( settings );
        settings.endGroup();
        settings.endGroup();
    }

    void State::load( Config::Configuration & settings )
    {
        m_name = settings.value( "name" ).toString();

        loadTextEdition( settings );
        loadVisualItemEdition( settings );
    }

    void State::save()
    {
        Config::Configuration settings( "tags" );

        settings.beginGroup( m_tag->name() );
        settings.beginGroup( m_name );

        settings.setValue( "name", m_name );

        saveTextEdition( settings );
        saveVisualItemEdition( settings );

        settings.endGroup();
        settings.endGroup();

        settings.sync();
    }

    void State::apply()
    {
        qDebug() << "refe";

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

        return state;
    }

}
