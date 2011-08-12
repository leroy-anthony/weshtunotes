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

#ifndef IVISUALITEMEDITION_H
#define IVISUALITEMEDITION_H

#include <QString>
#include <QColor>

#include "../data/DataManager.h"

namespace Item
{

    class IVisualItemEdition
    {
    public:
        IVisualItemEdition();
        virtual ~IVisualItemEdition();

        virtual void setItemColor( const QColor & color ) = 0;
        virtual void setSymbol( const QString & icon ) = 0;

        void saveVisualItemEdition( Data::DataManager & settings, const QString & tagName, const QString & state  );
        void loadVisualItemEdition( Data::DataManager & settings, const QString & tagName, const QString & state  );

        const QString & symbol();
        const QColor & itemColor();

    protected:
        QString m_symbol;
        QColor m_colorItem;

    };

}

#endif // IVISUALITEMEDITION_H
