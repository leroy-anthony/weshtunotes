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

#include "ItemTreeBasket.h"

#include <QDir>
#include <QCoreApplication>

#include "../config/Configuration.h"
#include "../config/ImageFactory.h"
#include "../scene/SceneFactory.h"
#include "../basket/Basket.h"
#include "../basket/BasketFactory.h"
#include "../explorer/TreeExplorer.h"

namespace Explorer
{

    ItemTreeBasket::ItemTreeBasket( ItemTreeBasket * itemTreeBasket,
                                    const QString & id,
                                    int order,
                                    const QMap<QString,QString> & options,
                                    const QString & type ):
        QTreeWidgetItem( itemTreeBasket, QStringList(id) ),
        m_basket(0)
    {
        if ( itemTreeBasket != 0 )
        {
            m_basket = Basket::BasketFactory::newBasket( this, itemTreeBasket->basket(), id, options, type );
        }
        else
        {
            m_basket = Basket::BasketFactory::newBasket( this, id, options, type );
        }

        m_basket->setOrder( order );

        initItemTreeBasket();
    }

    void ItemTreeBasket::initItemTreeBasket()
    {
        QTreeWidgetItem::setIcon( 0, Config::ImageFactory::newInstance()->icon( m_basket->icon() ) );
    }

    void ItemTreeBasket::setIcon( const QString & icon )
    {
        if ( m_basket != 0 )
        {
            m_basket->setIcon( icon );
        }

        QTreeWidgetItem::setIcon( 0, KIcon(icon) );
    }

    ItemTreeBasket::~ItemTreeBasket()
    {
        delete m_basket;
    }

    void ItemTreeBasket::setBasket( Basket::AbstractBasket * basket )
    {
        delete m_basket;
        m_basket = basket;
    }

    Basket::AbstractBasket * ItemTreeBasket::basket()
    {
        return m_basket;
    }

}
