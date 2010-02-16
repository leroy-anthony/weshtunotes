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

#include "BasketFactory.h"

#include "../basket/Basket.h"
#include "../basket/TagBasket.h"
#include "../basket/AbstractBasket.h"
#include "../config/Configuration.h"

#include <QDebug>

namespace Basket
{

    const char * BasketFactory::m_types[] = { "basket" , "tag_basket" };
    const char * BasketFactory::m_labels[] = { "Basket" , "Tag Basket" };

    BasketFactory::BasketFactory()
    {
    }

    AbstractBasket * BasketFactory::newBasket( AbstractBasket * parent,
                                               const QString & fileName,
                                               const QMap<QString,QString> & options,
                                               const QString & type )
    {
        if ( type == QString("") )
        {
            Config::Configuration settings( parent->directory() + QDir::separator() + fileName );
            return newBasketInterne( parent, fileName, options, settings.valueGroup("basket","type","basket") );
        }

        return newBasketInterne( parent, fileName, options, type );
    }

    AbstractBasket * BasketFactory::newBasket( const QString & fileName,
                                               const QMap<QString,QString> & options,
                                               const QString & type )
    {
        if ( type == QString("") )
        {
            Config::Configuration settings( fileName + QDir::separator() + fileName );
            return newBasketInterne( fileName, options, settings.valueGroup("basket","type","basket") );
        }

        return newBasketInterne( fileName, options, type );
    }

    AbstractBasket * BasketFactory::newBasketInterne( const QString & fileName,
                                                      const QMap<QString,QString> & options,
                                                      const QString & type )
    {
        AbstractBasket * basket = 0;

        if ( type == m_types[ BASKET ] )
        {
            basket = new Basket( fileName );
        }
        else if ( type == m_types[ TAG_BASKET ] )
        {
            basket = new TagBasket( fileName, options );
        }
        else
        {
            basket = new Basket( fileName );
        }

        return basket;
    }

    AbstractBasket * BasketFactory::newBasketInterne( AbstractBasket * parent,
                                                      const QString & fileName,
                                                      const QMap<QString,QString> & options,
                                                      const QString & type )
    {
        AbstractBasket * basket = 0;

        if ( type == m_types[ BASKET ] )
        {
            basket = new Basket( parent, fileName );
        }

        if ( type == m_types[ TAG_BASKET ] )
        {
            basket = new TagBasket( parent, fileName, options );
        }

        return basket;
    }

    QStringList BasketFactory::types()
    {
        QStringList types;

        for ( int i=0 ; i<MAX ; ++i )
        {
            types << QString(m_types[i]);
        }

        return types;
    }

    const char * BasketFactory::type( int type )
    {
        return m_types[ type ];
    }

    const char * BasketFactory::label( int type )
    {
        return m_labels[ type ];
    }

}
