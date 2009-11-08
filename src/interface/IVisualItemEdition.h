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

#ifndef IVISUALITEMEDITION_H
#define IVISUALITEMEDITION_H

#include <QString>
#include <QColor>

#include "../config/Configuration.h"

namespace Item
{

    class IVisualItemEdition
    {
    public:
        IVisualItemEdition();
        virtual ~IVisualItemEdition();

        virtual void setItemColor( const QColor & color ) = 0;
        virtual void setSymbol( const QString & icon ) = 0;

        void saveVisualItemEdition( Config::Configuration & settings );
        void loadVisualItemEdition( Config::Configuration & settings );

        const QString & symbol();
        const QColor & itemColor();

    protected:
        QString m_symbol;
        QColor m_colorItem;

    };

}

#endif // IVISUALITEMEDITION_H
