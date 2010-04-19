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

#include "Basket.h"

#include "../scene/SceneFactory.h"
#include "../config/Configuration.h"
#include "../basket/BasketFactory.h"
#include "../basket/ItemTreeBasket.h"
#include "../data/AssociationManager.h"
#include "settings.h"

namespace Basket
{

    Basket::Basket( ItemTreeBasket * itemTreeBasket, AbstractBasket * basketParent, const QString & name ):
            AbstractBasket( itemTreeBasket, basketParent, name )
    {
        initBasket();
    }

    Basket::Basket( ItemTreeBasket * itemTreeBasket, const QString & name ):
            AbstractBasket( itemTreeBasket, name )
    {
        initBasket();
    }

    void Basket::initBasket()
    {
        m_type = BasketFactory::type(BasketFactory::BASKET);

        m_contentScene = Scene::SceneFactory::newScene( m_configFilePath );
    }

    void Basket::save()
    {
        m_contentScene->save();

        AbstractBasket::save();
    }

    void Basket::load()
    {
        AbstractBasket::load();

        delete m_contentScene;

        m_contentScene = Scene::SceneFactory::newScene( m_configFilePath );
        m_contentScene->load( m_configFilePath );

        Config::Configuration settings( m_configFilePath );

        m_contentScene->loadHandles( Data::AssociationManager::abstractNotes( m_contentScene->id() ) );
    }

    void Basket::del()
    {
        QList<AbstractBasket*> & children = childrenBasket();
        for ( int i=0 ; i<children.size() ; ++i )
        {
            children[i]->del();
        }

        Data::AssociationManager::removeNotes( m_contentScene->id() );
        Config::Configuration::removeConfigDir( m_configFilePath );
    }

}
