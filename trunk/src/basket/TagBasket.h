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

#ifndef TAGBASKET_H_
#define TAGBASKET_H_

#include "AbstractBasket.h"

namespace Basket
{
    class AbstractBasket;

    class TagBasket : public AbstractBasket
    {
    public:
        TagBasket( ItemTreeBasket * itemTreeBasket, AbstractBasket * basket, const QString & id, const QMap<QString,QString> & options );
        TagBasket( ItemTreeBasket * itemTreeBasket, const QString & id, const QMap<QString,QString> & options );

        void load();
        void save();
        void del();

    private:
        void initTagBasket( const QMap<QString,QString> & options );

        QString m_tagName;

    };

}

#endif // TAGBASKET_H_
