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

#include <QDebug>

namespace Basket
{

    Basket::Basket( AbstractBasket * basketParent, const QString & name ):
            AbstractBasket( basketParent, name )
    {
        m_type = "basket";
    }

    Basket::Basket( const QString & name ):
            AbstractBasket( name )
    {
        m_type = "basket";
    }

    void Basket::save()
    {
        m_contentScene->save( m_nameId, m_configFilePath );
        QList<AbstractBasket*> children = childrenBasket();
        for ( int i=0 ; i<children.size() ; ++i )
        {
            children[i]->save();
        }

        AbstractBasket::save();
    }

    void Basket::load()
    {
        m_contentScene = Scene::SceneFactory::newScene( m_configFilePath );

        m_nameId = m_contentScene->id();

        QList<AbstractBasket*> children = childrenBasket();
        for ( int i=0 ; i<children.size() ; ++i )
        {
            children[i]->load();
        }
    }

    void Basket::del()
    {
        QList<AbstractBasket*> children = childrenBasket();
        for ( int i=0 ; i<children.size() ; ++i )
        {
            children[i]->del();
        }

        Config::Configuration::removeConfigDir( m_configFilePath );
    }

}
