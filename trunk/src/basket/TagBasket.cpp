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

#include "TagBasket.h"

#include <QFile>

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

#include "settings.h"
#include "../scene/AbstractScene.h"
#include "../scene/SceneFactory.h"
#include "../config/Configuration.h"
#include "../basket/AbstractBasket.h"
#include "../basket/BasketFactory.h"
#include "../basket/ItemTreeBasket.h"

namespace Basket
{
    
    TagBasket::TagBasket( ItemTreeBasket * itemTreeBasket,
                          AbstractBasket * basket,
                          const QString & name,
                          const QMap<QString,QString> & options ):
            AbstractBasket( itemTreeBasket, basket, name ),
            m_tagName( "" )
    {
        initTagBasket( options );
    }
    
    TagBasket::TagBasket( ItemTreeBasket * itemTreeBasket,
                          const QString & name,
                          const QMap<QString,QString> & options ):
            AbstractBasket( itemTreeBasket, name ),
            m_tagName( "" )
    {
        initTagBasket( options );
    }

    void TagBasket::initTagBasket( const QMap<QString,QString> & options )
    {
        m_type = BasketFactory::type(BasketFactory::TAG_BASKET);

        m_contentScene = Scene::SceneFactory::newScene( m_configFilePath );

        if ( options.contains("tagName") )
        {
            m_tagName = options["tagName"];
        }
    }
    
    void TagBasket::save()
    {
        Config::Configuration settings( m_configFilePath );
        settings.setValue( "basket", "tag_name", m_tagName );

        AbstractBasket::save();
    }
    
    void TagBasket::load()
    {
        AbstractBasket::load();

        delete m_contentScene;
        m_contentScene = Scene::SceneFactory::newScene( m_configFilePath );

        Config::Configuration settings( m_configFilePath );
        m_tagName = settings.valueGroup("basket","tag_name","T");
        
        Nepomuk::Tag tag( "KWeshTuNotes::" + m_tagName );
        
        // term matching the tag
        Nepomuk::Query::ResourceTerm tagTerm( tag );
        
        // term matching tagged resource
        Nepomuk::Query::ComparisonTerm term( Soprano::Vocabulary::NAO::hasTag(),
                                             tagTerm,
                                             Nepomuk::Query::ComparisonTerm::Equal );
        
        // build the query
        Nepomuk::Query::Query query( term );
        
        Soprano::QueryResultIterator it = Nepomuk::ResourceManager::instance()->mainModel()->executeQuery( query.toSparqlQuery(), Soprano::Query::QueryLanguageSparql );
        while( it.next() )
        {
            QUrl url = Nepomuk::Resource( it.binding( "r" ).uri() ).property( QUrl("http://www.semanticdesktop.org/ontologies/2007/01/19/nie#url") ).toUrl();
            
            QList<QString> liste;
            liste << url.toLocalFile().replace(Settings::basketsStorePath().toLocalFile()+QDir::separator()+"baskets"+QDir::separator(),"");

            if ( QFile::exists( url.toLocalFile() ) )
            {
                m_contentScene->loadHandles( liste, QPointF(0,0), 0, 0 );
            }
            else
            {
                //retire la ressource de nepomuk
                Nepomuk::Resource resource( url );
                resource.remove();
            }
        }
        
        m_nameId = m_contentScene->id();
        
        QList<AbstractBasket*> & children = childrenBasket();
        for ( int i=0 ; i<children.size() ; ++i )
        {
            children[i]->load();
        }

        m_itemTreeBasket->setData( 0, Qt::DisplayRole, QString(m_name+" (TAG: "+m_tagName+")") );

    }
    
    void TagBasket::del()
    {
        Config::Configuration::removeConfigDir( m_configFilePath );
    }
    
}
