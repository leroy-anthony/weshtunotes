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

#include "ItemTreeBasket.h"

#include <QDir>
#include <QCoreApplication>

#include "../config/Configuration.h"
#include "../config/ImageFactory.h"
#include "../scene/SceneFactory.h"
#include "../basket/Basket.h"

namespace Basket
{
    ItemTreeBasket::ItemTreeBasket( Explorer::TreeExplorer * treeExplorer, const QString & name ):
            QTreeWidgetItem( treeExplorer, QStringList(name) ),
            m_basket(0)
    {
        m_basket = new Basket( name );

        initItemTreeBasket();
    }

    ItemTreeBasket::ItemTreeBasket( ItemTreeBasket * itemTreeBasket, const QString & name ):
            QTreeWidgetItem( itemTreeBasket, QStringList(name) ),
            m_basket(0)
    {
        m_basket = new Basket( itemTreeBasket->basket(), name );

        initItemTreeBasket();
    }

    void ItemTreeBasket::initItemTreeBasket()
    {
        setIcon( 0, Config::ImageFactory::icon(Config::Image::basket) );
    }

    ItemTreeBasket::~ItemTreeBasket()
    {
    }

    AbstractBasket * ItemTreeBasket::basket()
    {
        return m_basket;
    }

}
