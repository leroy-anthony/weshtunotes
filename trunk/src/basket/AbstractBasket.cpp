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

#include "AbstractBasket.h"

#include <QDir>

#include "../config/Configuration.h"

namespace Basket
{
    AbstractBasket::AbstractBasket( const QString & name ):
            GeneratorID("basket"),
            m_name( name ),
            m_contentScene(0)
    {
        m_directory = name;
        m_configFilePath = m_directory + QDir::separator() + name;
    }

    AbstractBasket::AbstractBasket( AbstractBasket * basketParent, const QString & name ):
            GeneratorID("basket"),
            m_name( name ),
            m_contentScene(0)
    {
        basketParent->addChild( this );
        m_directory = basketParent->directory() + QDir::separator() + name;
        m_configFilePath = m_directory + QDir::separator() + name;
    }

    void AbstractBasket::addChild( AbstractBasket * basket )
    {
        m_childsBasket.append( basket );
    }

    QList<AbstractBasket*> & AbstractBasket::childsBasket()
    {
        return m_childsBasket;
    }

    const QString & AbstractBasket::name()
    {
        return m_name;
    }

    const QString & AbstractBasket::directory()
    {
        return m_directory;
    }

    const QString & AbstractBasket::configFilePath()
    {
        return m_configFilePath;
    }

    Scene::AbstractScene * AbstractBasket::scene()
    {
        return m_contentScene;
    }

    void AbstractBasket::setScene( Scene::AbstractScene * scene )
    {
        m_contentScene = scene;
    }

    const QString & AbstractBasket::type()
    {
        return m_type;
    }

    void AbstractBasket::save()
    {
        Config::Configuration settings( m_configFilePath );
        settings.setValue("basket","type",m_type);
    }

}
