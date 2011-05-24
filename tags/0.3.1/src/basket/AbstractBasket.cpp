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

#include "AbstractBasket.h"

#include <KDebug>
#include <kpassworddialog.h>
#include <KWallet/Wallet>
#include <KCmdLineArgs>
#include <KAboutData>
#include <KApplication>

#include "../config/Configuration.h"
#include "../basket/ItemTreeBasket.h"
#include "../scene/AbstractScene.h"
#include "../data/DataManager.h"
#include "../main/MainWindow.h"
#include "../synchro/GoogleDocsConnection.h"
#include "../synchro/AbstractConnection.h"
#include "../synchro/SynchroManager.h"
#include "../synchro/ConnectionFactory.h"

namespace Basket
{
    AbstractBasket::AbstractBasket( ItemTreeBasket * itemTreeBasket, const QString & id ):
            GeneratorID("basket"),
            m_itemTreeBasket(itemTreeBasket),
            m_contentScene(0),
            m_order(0),
            m_name( "<unkown>" ),
            m_immuable(false)
    {
        initConfigFile(id);
    }

    AbstractBasket::AbstractBasket( ItemTreeBasket * itemTreeBasket, AbstractBasket * basketParent, const QString & id ):
            GeneratorID("basket"),
            m_itemTreeBasket(itemTreeBasket),
            m_contentScene(0),
            m_order(0),
            m_name( "<unkown>" )
    {
        basketParent->addChild( this );

        initConfigFile(id);
    }

    void AbstractBasket::initConfigFile( const QString & id )
    {
        if ( id != "" )
        {
            m_configFile = Data::DataManager::configFileBasket( id );
            setId(id);
        }
        else
        {
            m_configFile = Data::DataManager::configFileBasket( GeneratorID::id() );
        }
    }

    AbstractBasket::~AbstractBasket()
    {
    }

    void AbstractBasket::addChild( AbstractBasket * basket )
    {
        m_childrenBasket.append( basket );
    }

    QList<AbstractBasket*> & AbstractBasket::childrenBasket()
    {
        return m_childrenBasket;
    }

    const QString & AbstractBasket::name()
    {
        return m_name;
    }

    void AbstractBasket::setName( const QString & name )
    {
        m_name = name;
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

    const QString & AbstractBasket::icon()
    {
        return m_icon;
    }

    void AbstractBasket::setIcon( const QString & icon )
    {
        m_icon = icon;
    }

    void AbstractBasket::save()
    {
        Data::DataManager settings( m_configFile );
        settings.setValue( "basket", "type",  m_type );
        settings.setValue( "basket", "icon",  m_icon );
        settings.setValue( "basket", "order", m_order );
        settings.setValue( "basket", "id",    GeneratorID::id() );
        settings.setValue( "basket", "name",  m_name );
        settings.setValue( "basket", "color", m_backgroundColor );

    }

    void AbstractBasket::load()
    {
        Data::DataManager settings( m_configFile );

        if ( m_type.isEmpty() )
        {
            m_type = settings.valueGroup( "basket", "type", "basket" );
        }

        m_icon = settings.valueGroup( "basket", "icon", "folder" );
        m_name = settings.valueGroup( "basket", "name", "<unknow>" );
        m_order = settings.valueGroup( "basket", "order", 0 ).toInt();
        m_backgroundColor = settings.valueGroup( "basket", "color", Settings::colorBasket().name() );

        setId( settings.valueGroup( "basket", "id", GeneratorID::id() ) );

        m_itemTreeBasket->setIcon( m_icon );
        m_itemTreeBasket->setData( 1, Qt::DisplayRole, m_order );
        m_itemTreeBasket->setData( 0, Qt::DisplayRole, m_name );
    }

    int AbstractBasket::order()
    {
        return m_order;
    }

    void AbstractBasket::setOrder( int order )
    {
        m_order = order;
    }

    const QString & AbstractBasket::configFile()
    {
        return m_configFile;
    }

    void AbstractBasket::commit()
    {
        QAction * a = static_cast<QAction*>(sender());

        Synchro::ConnectionFactory::Type type = (Synchro::ConnectionFactory::Type) a->data().toInt();
        Synchro::AbstractConnection * cx = Synchro::ConnectionFactory::newConnection( type );

        if ( cx != 0 )
        {
            Synchro::SynchroManager sync(cx);
            sync.commit( GeneratorID::id() );
        }
    }

    void AbstractBasket::update()
    {
        QAction * a = static_cast<QAction*>(sender());

        Synchro::ConnectionFactory::Type type = (Synchro::ConnectionFactory::Type) a->data().toInt();
        Synchro::AbstractConnection * cx = Synchro::ConnectionFactory::newConnection( type );

        if ( cx != 0 )
        {
            Synchro::SynchroManager sync(cx);
            sync.update( GeneratorID::id() );
        }

        load();
        MainWindow::reloadView();
    }

    const QString & AbstractBasket::backgroundColor()
    {
        return m_backgroundColor;
    }

    void AbstractBasket::setBackgroundColor( const QString & color )
    {
        m_backgroundColor = color;
    }

    bool AbstractBasket::isImmuable()
    {
        return m_immuable;
    }

}
