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
    ITextEdition::ITextEdition():
            m_bold(false),
            m_italic(false),
            m_alignment(0),
            m_underLine(false),
            m_colorFont(Qt::black),
            m_font("Arial"),
            m_weight(10),
            m_strikeOut(false)
    {
    }

    ITextEdition::~ITextEdition()
    {
    }

    void ITextEdition::saveTextEdition( Config::Configuration & settings, const QString & group, const QString & subGroup )
    {
        settings.setValue( group,subGroup,"bold", m_bold );
        settings.setValue( group,subGroup,"italic", m_italic );
        settings.setValue( group,subGroup,"strikeOut", m_strikeOut );
        settings.setValue( group,subGroup,"alignment", (int) m_alignment );
        settings.setValue( group,subGroup,"underline", m_underLine );
        settings.setValue( group,subGroup,"colorFont", m_colorFont.name() );
        settings.setValue( group,subGroup,"font", m_font );
        settings.setValue( group,subGroup,"weight", m_weight );
    }

    void ITextEdition::loadTextEdition( Config::Configuration & settings, const QString & group, const QString & subGroup )
    {
        m_bold = settings.valueSubGroup( group,subGroup,"bold", QVariant(false) ) == QString("true");
        m_italic = settings.valueSubGroup( group,subGroup,"italic", QVariant(false) ) == QString("true");
        m_strikeOut = settings.valueSubGroup( group,subGroup,"strikeOut", QVariant(false) ) == QString("true");
        //FIXME: m_alignment = settings.valueSubGroup( group,subGroup,"alignment", QVariant(1) ).toInt();
        m_underLine = settings.valueSubGroup( group,subGroup,"underline", QVariant(false) ) == QString("true");
        m_colorFont = QColor( settings.valueSubGroup( group,subGroup,"colorFont", "#000000" ) );
        m_font = settings.valueSubGroup( group,subGroup,"font","Arial" );
        m_weight = settings.valueSubGroup( group,subGroup,"weight", QVariant(1) ).toInt();
    }

    bool ITextEdition::bold()
    {
        return m_bold;
    }

    bool ITextEdition::italic()
    {
        return m_italic;
    }

    Qt::Alignment ITextEdition::alignment()
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

