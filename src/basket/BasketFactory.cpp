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
#include "BasketFactory.h"

#include <QMap>

#include <KDebug>

#include "../basket/Basket.h"
#include "../basket/TagBasket.h"
#include "../basket/AbstractBasket.h"
#include "../config/Configuration.h"
#include "../data/DataManager.h"

namespace Basket
{

    const char * BasketFactory::m_types[] = { "basket" , "tag_basket" };
    const char * BasketFactory::m_labels[] = { "Basket" , "Tag Basket" };

    BasketFactory::BasketFactory()
    {
    }

    AbstractBasket * BasketFactory::newBasket( ItemTreeBasket * itemTreeBasket,
                                               AbstractBasket * parent,
                                               const QString & id,
                                               const QMap<QString,QString> & options,
                                               const QString & type )
    {
        if ( type == QString("") )
        {
            Data::DataManager settings( Data::DataManager::configFileBasket( id ) );
            return newBasketInterne( itemTreeBasket, parent, id, options, settings.valueGroup("basket","type","basket") );
        }

        return newBasketInterne( itemTreeBasket, parent, id, options, type );
    }

    AbstractBasket * BasketFactory::newBasket( ItemTreeBasket * itemTreeBasket,
                                               const QString & id,
                                               const QMap<QString,QString> & options,
                                               const QString & type )
    {
        if ( type == QString("") )
        {
            Data::DataManager settings( Data::DataManager::configFileBasket( id ) );
            return newBasketInterne( itemTreeBasket, id, options, settings.valueGroup("basket","type","basket") );
        }

        return newBasketInterne( itemTreeBasket, id, options, type );
    }

    AbstractBasket * BasketFactory::newBasketInterne( ItemTreeBasket * itemTreeBasket,
                                                      const QString & id,
                                                      const QMap<QString,QString> & options,
                                                      const QString & type )
    {
        AbstractBasket * basket = 0;

        if ( type == m_types[ BASKET ] )
        {
            basket = new Basket( itemTreeBasket, id );
        }
        else if ( type == m_types[ TAG_BASKET ] )
        {
            basket = new TagBasket( itemTreeBasket, id, options );
        }
        else
        {
            basket = new Basket( itemTreeBasket, id );
        }

        return basket;
    }

    AbstractBasket * BasketFactory::newBasketInterne( ItemTreeBasket * itemTreeBasket,
                                                      AbstractBasket * parent,
                                                      const QString & id,
                                                      const QMap<QString,QString> & options,
                                                      const QString & type )
    {
        AbstractBasket * basket = 0;

        if ( type == m_types[ BASKET ] )
        {
            basket = new Basket( itemTreeBasket, parent, id );
        }

        if ( type == m_types[ TAG_BASKET ] )
        {
            basket = new TagBasket( itemTreeBasket, parent, id, options );
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
