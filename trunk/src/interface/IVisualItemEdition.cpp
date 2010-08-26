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

#include "IVisualItemEdition.h"

#include "settings.h"
#include "../config/VisualAspect.h"
#include "../data/DataManager.h"

namespace Item
{
    IVisualItemEdition::IVisualItemEdition():
            m_colorItem( Settings::colorItem() )
    {
    }

    IVisualItemEdition::~IVisualItemEdition()
    {
    }

    void IVisualItemEdition::saveVisualItemEdition( Data::DataManager & settings, const QString & tagName, const QString & state )
    {
        settings.setValue( tagName, state, "symbol", m_symbol );
        settings.setValue( tagName, state, "colorItem", m_colorItem.name() );
    }

    void IVisualItemEdition::loadVisualItemEdition( Data::DataManager & settings, const QString & tagName, const QString & state  )
    {
        m_symbol = settings.valueSubGroup( tagName, state, "symbol", "" );
        m_colorItem = QColor( settings.valueSubGroup( tagName, state, "colorItem", Settings::colorItem() ) );
    }

    const QString & IVisualItemEdition::symbol()
    {
        return m_symbol;
    }

    const QColor & IVisualItemEdition::itemColor()
    {
        return m_colorItem;
    }

}


