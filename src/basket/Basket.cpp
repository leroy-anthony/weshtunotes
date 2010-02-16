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
#include "settings.h"

#include <Nepomuk/Tag>
#include <Nepomuk/Query/Query>
#include <Nepomuk/Query/ResourceTerm>
#include <Nepomuk/Query/ComparisonTerm>
#include <Soprano/Vocabulary/NAO>
#include <Soprano/Model>
#include <Soprano/QueryResultIterator>
#include <Nepomuk/Resource>
#include <Nepomuk/Variant>
#include <Nepomuk/ResourceManager>

#include <QDebug>

namespace Basket
{

    Basket::Basket( AbstractBasket * basketParent, const QString & name ):
            AbstractBasket( basketParent, name )
    {
        initBasket();
    }

    Basket::Basket( const QString & name ):
            AbstractBasket( name )
    {
        initBasket();
    }

    void Basket::initBasket()
    {
        m_type = BasketFactory::type(BasketFactory::BASKET);

        m_contentScene = Scene::SceneFactory::newScene( m_configFilePath );
        m_nameId = m_contentScene->id();
    }

    void Basket::save()
    {
        m_contentScene->save();

        QList<AbstractBasket*> & children = childrenBasket();
        for ( int i=0 ; i<children.size() ; ++i )
        {
            children[i]->save();
        }

        AbstractBasket::save();
    }

    void Basket::load()
    {
        delete m_contentScene;

        m_contentScene = Scene::SceneFactory::newScene( m_configFilePath );
        m_contentScene->load( m_configFilePath );
        m_nameId = m_contentScene->id();

        QList<AbstractBasket*> & children = childrenBasket();
        for ( int i=0 ; i<children.size() ; ++i )
        {
            children[i]->load();
        }

        AbstractBasket::load();
    }

    void Basket::del()
    {
        QList<AbstractBasket*> & children = childrenBasket();
        for ( int i=0 ; i<children.size() ; ++i )
        {
            children[i]->del();
        }

        Config::Configuration::removeConfigDir( m_configFilePath );
    }

}
