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

#ifndef BASKET_H_
#define BASKET_H_

#include "AbstractBasket.h"

namespace Basket
{
    class ItemTreeBasket;

    class Basket : public AbstractBasket
    {
    public:
        Basket( ItemTreeBasket * itemTreeBasket, AbstractBasket * basket, const QString & id );
        Basket( ItemTreeBasket * itemTreeBasket, const QString & id );

        void load();
        void save();
        void del();

    private:
        void initBasket();

    };

}

#endif // BASKET_H_
