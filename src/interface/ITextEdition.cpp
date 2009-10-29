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

#include "ITextEdition.h"

namespace Item
{
    ITextEdition::~ITextEdition()
    {
    }

    void ITextEdition::saveTextEdition( Config::Configuration & settings )
    {
        settings.setValue( "bold", m_bold );
        settings.setValue( "italic", m_italic );
        settings.setValue( "strikeOut", m_strikeOut );
        settings.setValue( "alignment", m_alignment );
        settings.setValue( "underline", m_underLine );
        settings.setValue( "colorFont", m_colorFont );
        settings.setValue( "font", m_font );
        settings.setValue( "weight", m_weight );
    }

    void ITextEdition::loadTextEdition( Config::Configuration & settings )
    {
        m_bold = settings.value( "bold" ).toBool();
        m_italic = settings.value( "italic" ).toBool();
        m_strikeOut = settings.value( "strikeOut" ).toBool();
        m_alignment = settings.value( "alignment" ).toInt();
        m_underLine = settings.value( "underline" ).toBool();
        m_colorFont = settings.value( "colorFont" ).value<QColor>();
        m_font = settings.value( "font" ).toString();
        m_weight = settings.value( "weight" ).toInt();
    }

    bool ITextEdition::bold()
    {
        return m_bold;
    }

    bool ITextEdition::italic()
    {
        return m_italic;
    }

    int ITextEdition::alignment()
    {
        return m_alignment;
    }

    bool ITextEdition::underline()
    {
        return m_underLine;
    }

    bool ITextEdition::fontStrikeOut()
    {
        return m_strikeOut;
    }

    const QColor & ITextEdition::textColor()
    {
        return m_colorFont;
    }

    const QString & ITextEdition::fontFamily()
    {
        return m_font;
    }

    int ITextEdition::fontPointSize()
    {
        return m_weight;
    }

}

