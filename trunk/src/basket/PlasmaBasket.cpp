/*
    Copyright (c) 2011 LEROY Anthony <leroy.anthony@gmail.com>

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

#include "PlasmaBasket.h"

#include "../scene/SceneFactory.h"
#include "../config/Configuration.h"
#include "../basket/BasketFactory.h"
#include "../basket/ItemTreeBasket.h"
#include "../data/AssociationManager.h"
#include "../data/DataManager.h"
#include "settings.h"

namespace Basket
{

    PlasmaBasket::PlasmaBasket( ItemTreeBasket * itemTreeBasket, AbstractBasket * basketParent, const QString & id ):
            AbstractBasket( itemTreeBasket, basketParent, id )
    {
        initBasket();
    }

    PlasmaBasket::PlasmaBasket( ItemTreeBasket * itemTreeBasket, const QString & id ):
            AbstractBasket( itemTreeBasket, id )
    {
        initBasket();
    }

    void PlasmaBasket::initBasket()
    {
        m_type = BasketFactory::type(BasketFactory::PLASMA_BASKET);

        m_contentScene = Scene::SceneFactory::newScene( m_configFile, Scene::SceneFactory::PLASMASCENE );
    }

    void PlasmaBasket::save()
    {
        AbstractBasket::save();
    }

    void PlasmaBasket::load()
    {
        AbstractBasket::load();

        delete m_contentScene;

        m_contentScene = Scene::SceneFactory::newScene( m_configFile, Scene::SceneFactory::PLASMASCENE );
        m_contentScene->load( m_configFile );;
        m_contentScene->scene()->setBackgroundBrush(QBrush(QColor(m_backgroundColor)));
    }

    void PlasmaBasket::del()
    {
        Data::DataManager::removeBasket( m_configFile );
    }

}
