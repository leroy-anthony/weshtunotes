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

#ifndef ITEMTREEBASKET_H_
#define ITEMTREEBASKET_H_

#include <QTreeWidgetItem>

#include "../main/general.h"
#include "../basket/AbstractBasket.h"

namespace Explorer
{
    class TreeExplorer;
}

namespace Basket
{

    class ItemTreeBasket : public QTreeWidgetItem
    {
    public:
        ItemTreeBasket( ItemTreeBasket * itemTreeBasket,
                        const QString & id,
                        int order,
                        const QMap<QString,QString> & options = QMap<QString,QString>(),
                        const QString & type = QString("") );

        ~ItemTreeBasket();

        AbstractBasket * basket();
        void setBasket( AbstractBasket * basket );

        void setIcon( const QString & icon );

    private:
        void initItemTreeBasket();

        AbstractBasket * m_basket;

    };

}

#endif // ITEMTREEBASKET_H_
