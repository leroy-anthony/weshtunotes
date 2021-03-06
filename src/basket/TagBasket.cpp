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
#include "../explorer/ItemTreeBasket.h"
#include "../data/AssociationManager.h"
#include "../data/DataManager.h"

namespace Basket
{
    
    TagBasket::TagBasket( Explorer::ItemTreeBasket * itemTreeBasket,
                          AbstractBasket * basket,
                          const QString & id,
                          const QMap<QString,QString> & options ):
    AbstractBasket( itemTreeBasket, basket, id ),
    m_tagName( "" )
    {
        initTagBasket( options );
    }
    
    TagBasket::TagBasket( Explorer::ItemTreeBasket * itemTreeBasket,
                          const QString & id,
                          const QMap<QString,QString> & options ):
    AbstractBasket( itemTreeBasket, id ),
    m_tagName( "" )
    {
        initTagBasket( options );
    }

    void TagBasket::initTagBasket( const QMap<QString,QString> & options )
    {
        m_type = BasketFactory::type(BasketFactory::TAG_BASKET);

        m_contentScene = Scene::SceneFactory::newScene( m_configFile );

        if ( options.contains("tagName") )
        {
            m_tagName = options["tagName"];
        }
    }
    
    void TagBasket::save()
    {
        m_contentScene->save(); // clear and save !!

        Data::DataManager settings( m_configFile );
        settings.setValue( "basket", "tag_name", m_tagName );
        
        AbstractBasket::save();
    }
    
    void TagBasket::load()
    {
        AbstractBasket::load();

        delete m_contentScene;
        m_contentScene = Scene::SceneFactory::newScene( m_configFile );
        m_contentScene->load( m_configFile );
        m_contentScene->setReadOnly(true);
	
        Data::DataManager settings( m_configFile );

        m_tagName = settings.valueGroup("basket","tag_name","error");
        
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
            liste << url.toLocalFile().replace(Settings::basketsStorePath().toLocalFile()+QDir::separator(),"");

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

        m_itemTreeBasket->setData( 0, Qt::DisplayRole, QString(m_name+" (TAG: "+m_tagName+")") );
    }
    
    void TagBasket::del()
    {
        Data::DataManager::removeBasket( m_configFile );
        Data::AssociationManager::removeAssociationNotes( m_contentScene->id() );
    }
    
}
