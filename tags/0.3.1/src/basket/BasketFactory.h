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

#ifndef BASKETFACTORY_H_
#define BASKETFACTORY_H_

#include <QStringList>

template <typename T, typename U> class QMap;

namespace Basket
{
    class AbstractBasket;
    class ItemTreeBasket;

    class BasketFactory
    {
    public:
        enum Type { BASKET, TAG_BASKET, PLASMA_BASKET, MAX };

        static AbstractBasket * newBasket( ItemTreeBasket * itemTreeBasket,
                                           AbstractBasket * parent,
                                           const QString & id,
                                           const QMap<QString,QString> & options,
                                           const QString & type = QString("") );

        static AbstractBasket * newBasket( ItemTreeBasket * itemTreeBasket,
                                           const QString & id,
                                           const QMap<QString,QString> & options,
                                           const QString & type = QString("") );

        static const char * type( int type );
        static const char * label( int type );

        static QStringList types();

    protected:
        BasketFactory();

    private:
        static AbstractBasket * newBasketInterne( ItemTreeBasket * itemTreeBasket,
                                                  AbstractBasket * parent,
                                                  const QString &  id,
                                                  const QMap<QString,QString> & options,
                                                  const QString & type );

        static AbstractBasket * newBasketInterne( ItemTreeBasket * itemTreeBasket,
                                                  const QString &  id,
                                                  const QMap<QString,QString> & options,
                                                  const QString & type );

        static const char * m_types[];
        static const char * m_labels[];

    };

}

#endif // BASKETFACTORY_H_
