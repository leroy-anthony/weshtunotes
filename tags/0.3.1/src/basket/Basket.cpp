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

#include "Basket.h"

#include "../scene/SceneFactory.h"
#include "../config/Configuration.h"
#include "../basket/BasketFactory.h"
#include "../basket/ItemTreeBasket.h"
#include "../data/AssociationManager.h"
#include "../data/DataManager.h"
#include "settings.h"

namespace Basket
{

    Basket::Basket( ItemTreeBasket * itemTreeBasket, AbstractBasket * basketParent, const QString & id ):
            AbstractBasket( itemTreeBasket, basketParent, id )
    {
        initBasket();
    }

    Basket::Basket( ItemTreeBasket * itemTreeBasket, const QString & id ):
            AbstractBasket( itemTreeBasket, id )
    {
        initBasket();
    }

    void Basket::initBasket()
    {
        m_type = BasketFactory::type(BasketFactory::BASKET);

        m_contentScene = Scene::SceneFactory::newScene( m_configFile );
    }

    void Basket::save()
    {
        m_contentScene->save();// clear and save !!

        AbstractBasket::save();
    }

    void Basket::load()
    {
        AbstractBasket::load();

        delete m_contentScene;

        m_contentScene = Scene::SceneFactory::newScene( m_configFile );
        m_contentScene->load( m_configFile );;
        m_contentScene->loadHandles( Data::AssociationManager::abstractNotes( m_contentScene->id() ) );
        m_contentScene->scene()->setBackgroundBrush(QBrush(QColor(m_backgroundColor)));
    }

    void Basket::del()
    {
        QList<AbstractBasket*> & children = childrenBasket();
        for ( int i=0 ; i<children.size() ; ++i )
        {
            children[i]->del();
        }

        Data::AssociationManager::removeNotes( m_contentScene->id() );
        Data::DataManager::removeBasket( m_configFile );
    }

}
