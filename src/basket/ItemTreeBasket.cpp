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

namespace Basket
{
    int ItemTreeBasket::m_id = 0;

    ItemTreeBasket::ItemTreeBasket( Explorer::TreeExplorer * treeExplorer, const QString & name ):
            QTreeWidgetItem( treeExplorer, QStringList(name) ),
            m_directory( name ),
            m_name( name ),
            m_configFilePath( name + QDir::separator() + name ),
            m_contentScene(0)
    {
        initItemTreeBasket();
    }

    ItemTreeBasket::ItemTreeBasket( Basket::ItemTreeBasket * itemTreeBasket, const QString & name ):
            QTreeWidgetItem( itemTreeBasket, QStringList(name) ),
            m_name( name ),
            m_contentScene(0)
    {
        m_directory = itemTreeBasket->directory() + QDir::separator() + name;
        m_configFilePath = m_directory + QDir::separator() + name;

        initItemTreeBasket();
    }

    void ItemTreeBasket::initItemTreeBasket()
    {
        m_basketId = QString("basket%1").arg(m_id);
        ++m_id;

        setIcon( 0, Config::ImageFactory::icon(Config::Image::basket) );
    }

    ItemTreeBasket::~ItemTreeBasket()
    {
        delete m_contentScene;
    }

    const QString & ItemTreeBasket::name()
    {
        return m_name;
    }

    const QString & ItemTreeBasket::directory()
    {
        return m_directory;
    }

    const QString & ItemTreeBasket::configFilePath()
    {
        return m_configFilePath;
    }

    Scene::AbstractScene * ItemTreeBasket::scene()
    {
        return m_contentScene;
    }

    void ItemTreeBasket::setScene( Scene::AbstractScene * scene )
    {
        m_contentScene = scene;
    }

    void ItemTreeBasket::save()
    {
        m_contentScene->save( m_basketId, m_configFilePath );
        int childsSize = childCount();
        for ( int i=0 ; i<childsSize ; ++i )
        {
            dynamic_cast<ItemTreeBasket*>(child(i))->save();
        }
    }

    void ItemTreeBasket::load()
    {
        m_contentScene = Scene::SceneFactory::newScene( m_configFilePath );

        m_basketId = m_contentScene->id();

        setText(1,m_basketId);
        int childsSize = childCount();
        for ( int i=0 ; i<childsSize ; ++i )
        {
            dynamic_cast<ItemTreeBasket*>(child(i))->load();
        }
    }

    void ItemTreeBasket::load( const QString & type )
    {
        m_contentScene = Scene::SceneFactory::newScene( m_configFilePath, type );

        m_basketId = m_contentScene->id();

        setText(1,m_basketId);
        int childsSize = childCount();
        for ( int i=0 ; i<childsSize ; ++i )
        {
            dynamic_cast<ItemTreeBasket*>(child(i))->load();
        }
    }

    void ItemTreeBasket::del()
    {
        int childSize = childCount();
        for ( int i=0 ; i<childSize ; ++i )
        {
            dynamic_cast<ItemTreeBasket*>(child(i))->del();
        }

        Config::Configuration::removeConfigDir( m_configFilePath );
    }

    void ItemTreeBasket::setBasketId( const QString & id )
    {
        m_basketId = id;
        QString idStr = QString(id).replace("basket","");
        if ( idStr.toInt() > m_id )
        {
            m_id = idStr.toInt() + 1;
        }
    }

    const QString & ItemTreeBasket::basketId()
    {
        return m_basketId;
    }

}
