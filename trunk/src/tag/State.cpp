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
#include "../tag/NoteTag.h"

namespace Tag
{

    State::State( NoteTag * tag ):
            m_tag(tag),
            m_item(tag->noteItem()),
            m_name("default"),
            m_symbol("icon:triangle.svg"),
            m_bold(true),
            m_italic(true),
            m_alignment(Qt::AlignLeft),
            m_underLine(true),
            m_colorFont(QColor(150,150,150)),
            m_font("DejaVu Sans"),
            m_weight(16),
            m_colorItem(QColor(255,0,0))
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

    bool State::bold()
    {
        return m_bold;
    }

    void State::setItalic( bool italic )
    {
        m_italic = italic;
        if ( m_item != 0 )
        {
            m_item->setItalic( italic );
        }
    }

    bool State::italic()
    {
        return m_italic;
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

    void State::setTextColor( const QColor & color )
    {
        m_colorFont = color;
        if ( m_item != 0 )
        {
            m_item->setTextColor( color );
        }
    }

    const QColor & State::textColor()
    {
        return m_colorFont;
    }

    void State::setFontFamily( const QFont & font )
    {
        m_font = font.family();
        if ( m_item != 0 )
        {
            m_item->setFontFamily( font );
        }
    }

    const QString & State::fontFamily()
    {
        return m_font;
    }

    void State::setFontPointSize( int weight )
    {
        m_weight = weight;
        if ( m_item != 0 )
        {
            m_item->setFontPointSize( weight );
        }
    }

    int State::fontPointSize()
    {
        return m_weight;
    }

    void State::setItem( Item::NoteItem * item )
    {
        m_item = item;
    }

    const QString & State::symbol()
    {
        return m_symbol;
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

    void State::setItemColor( const QColor & color )
    {
        m_colorItem = QColor(color);
        if ( m_item != 0 )
        {
            m_item->setItemColor( color );
        }
    }

    const QColor & State::itemColor()
    {
        return m_colorItem;
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
        m_symbol = settings.value( "symbol" ).toString();
        m_bold = settings.value( "bold" ).toBool();
        m_italic = settings.value( "italic" ).toBool();
        m_alignment = settings.value( "alignment" ).toInt();
        m_underLine = settings.value( "underline" ).toBool();
        m_colorFont = settings.value( "colorFont" ).value<QColor>();
        m_font = settings.value( "font" ).toString();
        m_weight = settings.value( "weight" ).toInt();

        m_colorItem = settings.value( "colorItem" ).value<QColor>();
    }

    void State::save()
    {
        //Config::Configuration settings( m_tag->name()+":"+m_name );
        Config::Configuration settings( "tags" );

        settings.beginGroup( m_tag->name() );
        settings.beginGroup( m_name );

        settings.setValue( "name", m_name );
        settings.setValue( "symbol", m_symbol );
        settings.setValue( "bold", m_bold );
        settings.setValue( "italic", m_italic );
        settings.setValue( "alignment", m_alignment );
        settings.setValue( "underline", m_underLine );
        settings.setValue( "colorFont", m_colorFont );
        settings.setValue( "font", m_font );
        settings.setValue( "weight", m_weight );

        settings.setValue( "colorItem", m_colorItem );

        settings.endGroup();
        settings.endGroup();

        settings.sync();
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
        setItemColor( m_colorItem );
    }

    State * State::newState( NoteTag * tag )
    {
        State * state = new State(tag);
        state->setName( "new state" );

        return state;
    }

}
