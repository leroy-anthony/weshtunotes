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

#ifndef BASKETFACTORY_H_
#define BASKETFACTORY_H_

#include <QStringList>

template <typename T, typename U> class QMap;

namespace Basket
{
    class AbstractBasket;

    class BasketFactory
    {
    public:
        enum Type { BASKET, TAG_BASKET, MAX };

        static AbstractBasket * newBasket( AbstractBasket * parent,
                                           const QString & fileName,
                                           const QMap<QString,QString> & options,
                                           const QString & type = QString("") );

        static AbstractBasket * newBasket( const QString & fileName,
                                           const QMap<QString,QString> & options,
                                           const QString & type = QString("") );

        static const char * type( int type );
        static const char * label( int type );

        static QStringList types();

    protected:
        BasketFactory();

    private:
        static AbstractBasket * newBasketInterne( AbstractBasket * parent,
                                                  const QString &  fileName,
                                                  const QMap<QString,QString> & options,
                                                  const QString & type );

        static AbstractBasket * newBasketInterne( const QString &  fileName,
                                                  const QMap<QString,QString> & options,
                                                  const QString & type );

        static const char * m_types[];
        static const char * m_labels[];

    };

}

#endif // BASKETFACTORY_H_
