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

#include "AbstractScene.h"

#include <limits>

#include <QScrollBar>
#include <QGraphicsWidget>

#include <KLocalizedString>

#include "settings.h"
#include "../config/Configuration.h"
#include "../config/VisualAspect.h"
#include "../handle/HandleItem.h"
#include "../main/MainWindow.h"
#include "../item/AbstractItem.h"
#include "../item/ItemFactory.h"
#include "../data/AssociationManager.h"
#include "../data/DataManager.h"

namespace Scene
{

    AbstractScene::AbstractScene( const QString & id ):
            GeneratorID(id,false),
            m_horizontalScrollBarValueView(0),
            m_verticalScrollBarValueView(0),
            m_form(0),
            m_layout(0),
            m_lastCibleHandle(0),
            m_readOnly(false)
    {
        qreal max = std::numeric_limits<qreal>::max();
        m_scene.setSceneRect( -max, -max, max, max );

        connect( &m_scene, SIGNAL(selectionChanged()), this, SLOT(showMessageStatus()) );
    }

    AbstractScene::AbstractScene():
            GeneratorID("scene"),
            m_horizontalScrollBarValueView(0),
            m_verticalScrollBarValueView(0),
            m_form(0),
            m_layout(0),
            m_lastCibleHandle(0),
            m_readOnly(false)
    {
        qreal max = std::numeric_limits<qreal>::max();
        m_scene.setSceneRect( -max, -max, max, max );

        connect( &m_scene, SIGNAL(selectionChanged()), this, SLOT(showMessageStatus()) );
    }

    AbstractScene::~AbstractScene()
    {
        m_scene.disconnect();
    }

    void AbstractScene::showMessageStatus()
    {
        int size = m_scene.selectedItems().size();
        if ( size > 0 )
        {
            MainWindow::showMessage( i18np("1 selected item", "%1 selected items", size), 0 );
        }
        else
        {
            MainWindow::showMessage( i18np("1 item","%1 items", this->handles().size()), 0 );
        }
    }

    const QString & AbstractScene::type()
    {
        return m_type;
    }

    void AbstractScene::setType( const QString & type )
    {
        m_type = type;
    }

    void AbstractScene::buildListHandleToLoad( Handle::HandleItem * h, QStringList & l )
    {
        const QList<Handle::HandleItem*> & handlesChild = h->children();
        for ( int j=0 ; j<handlesChild.size() ; ++j )
        {
            buildListHandleToLoad( handlesChild[j], l );
        }

        l << h->id();
    }

    void AbstractScene::save()
    {
        QList<Handle::HandleItem*> handles = m_handles.keys();

        // on récupère les notes présentes lors de la dernière sauvegarde

        QStringList listLastFile = Data::AssociationManager::abstractNotes( GeneratorID::id() );

        //efface l'ancien configuration du panier
        Data::DataManager::clearBasketConfig( m_directoryScene );

        Data::DataManager::setIdScene( m_directoryScene, GeneratorID::id() );

        for (int i=0 ; i<handles.size() ; ++i)
        {
            QStringList listHandles;
            buildListHandleToLoad( handles[i], listHandles );

            Data::DataManager::saveSubHandles( handles[i]->configFile(), listHandles );

            if ( listHandles.size() > 0 )
            {
                QString fileName( handles[i]->configFile() );

                Data::AssociationManager::addNote( GeneratorID::id(), fileName, handles[i]->x(), handles[i]->y() );

                listLastFile.removeOne( fileName );
            }
        }

        for ( int i=0 ; i<listLastFile.size() ; ++i )
        {
            Data::AssociationManager::removeNote( GeneratorID::id(), listLastFile[i] );
        }

        for ( int i=0 ; i<handles.size() ; ++i )
        {
            handles[i]->save();
        }

        saveViewOnDisk( m_directoryScene );
    }

    void AbstractScene::storeView( CustomGraphicsView * view )
    {
        view->setInteractive(false);
        m_transformView = view->transform();
        m_horizontalScrollBarValueView = view->horizontalScrollBar()->value();
        m_verticalScrollBarValueView = view->verticalScrollBar()->value();
        view->setInteractive(true);
    }

    void AbstractScene::restoreView( CustomGraphicsView * view  )
    {
        view->setInteractive(false);
        view->setTransform(m_transformView);
        view->horizontalScrollBar()->setValue(m_horizontalScrollBarValueView);
        view->verticalScrollBar()->setValue(m_verticalScrollBarValueView);
        view->setInteractive(true);
    }

    void AbstractScene::saveViewOnDisk( const QString & fileName )
    {
        if ( m_scene.views().size() > 0 )
        {
            storeView( (CustomGraphicsView*) m_scene.views()[0] );

            Data::DataManager::saveView( fileName, m_transformView, m_horizontalScrollBarValueView, m_verticalScrollBarValueView, m_type );
        }
    }

    void AbstractScene::loadViewFromDisk( const QString & fileName  )
    {
        //FIXME:Config::Configuration settings( fileName );

        //FIXME:m_transformView = QTransform(
        //FIXME:        settings.valueGroup("scene","transform_m11",1).toInt(),
        //FIXME:        settings.valueGroup("scene","transform_m12",0).toInt(),
        //FIXME:        settings.valueGroup("scene","transform_m21",0).toInt(),
        //FIXME:        settings.valueGroup("scene","transform_m22",1).toInt(),
        //FIXME:        settings.valueGroup("scene","transform_dx",0).toInt(),
        //FIXME:        settings.valueGroup("scene","transform_dy",0).toInt());
        QPair< QTransform, QPair<int,int> > viewConfig = Data::DataManager::loadView( fileName );
        m_transformView = viewConfig.first;

        //FIXME:m_horizontalScrollBarValueView = settings.valueGroup("scene","hscroll",0).toInt();
        //FIXME:m_verticalScrollBarValueView   = settings.valueGroup("scene","vscroll",0).toInt();
        m_horizontalScrollBarValueView = viewConfig.second.first;
        m_verticalScrollBarValueView = viewConfig.second.second;

        if ( m_scene.views().size() > 0 )
        {
            restoreView( (CustomGraphicsView*) m_scene.views()[0] );
        }
    }

    void AbstractScene::loadHandles( const QList<QString> & filesName, QPointF centerPt, int selectionWidth, int selectionHeigth, bool newHandles )
    {
        int dx = 0;
        int dy = 0;
	
        for ( int j=0 ; j<filesName.size() ; ++j )
        {
            QString fileName = filesName[j];

            QList<Item::AbstractItem*> itemsToLoad;
            QMap<QString,Handle::HandleItem*> handles;

            Data::DataManager settingsHandle( fileName );

            QStringList items = settingsHandle.values("general","items");

            Handle::HandleItem * handle = 0;

            for (int i=0 ; i<items.size() ; ++i)
            {
                QString itemId = items[i];

                QPoint point = Data::AssociationManager::positionAbstractNotes( GeneratorID::id(), fileName );
                int x = point.x();
                int y = point.y();

                if ( j == 0 && centerPt != QPointF(0,0) )
                {
                    dx = centerPt.x()-x-selectionWidth/2;
                    dy = centerPt.y()-y-selectionHeigth/2;
                }

                handle = newHandle( x+dx, y+dy );
		if ( handle != 0 )
		{
                    handle->load( itemId );

		    //for copy/paste
		    if ( newHandles )
		    {
			handle->regenerateId();
		    }

		    handles[ itemId ] = handle;

		    QStringList itemsToAdd = settingsHandle.values( itemId, "items" );
		    if ( itemsToAdd.size()>0 )
		    {                    
			for (int k = 0 ; k <itemsToAdd.size() ; ++k)
			{
			    handle->add( handles[ itemsToAdd[k] ] );
			}
		    }
		    else
		    {
                        QString id = settingsHandle.valueGroup( itemId, "data", "" );

                        Item::AbstractItem * item = Item::ItemFactory::newItem( id );
                        connect( item, SIGNAL(editItem(Item::AbstractItem*)), this, SLOT(editItem(Item::AbstractItem*)));
			handle->add( item );

                        itemsToLoad += item;

			QStringList namesTags = settingsHandle.values( itemId, "tag" );
			for ( int k=0 ; k<namesTags.size() ; ++k )
			{
			    QStringList s = namesTags[k].split(":");
                            item->addTag(s[0],s[1]);
                        }
		    }
		}
            }
            
            if ( handle != 0 )
	    {
		addHandleToScene( handle );
	    }

            //charge la note
            for ( int k=0 ; k<itemsToLoad.size() ; ++k )
            {
                itemsToLoad[k]->load();
            }
        }
    }

    void AbstractScene::load( const QString & fileName )
    {
        Data::DataManager settings( fileName );

        setId( settings.valueGroup( "scene", "id", GeneratorID::id() ) );
        m_directoryScene = fileName;

        loadViewFromDisk( fileName );
    }

    bool AbstractScene::readOnly()
    {
        return m_readOnly;
    }

    void AbstractScene::setReadOnly( bool readOnly )
    {
        m_readOnly = readOnly;
    }

    const QString & AbstractScene::directoryScene()
    {
        return m_directoryScene;
    }

    void AbstractScene::setDirectoryScene( const QString & directory )
    {
        m_directoryScene = directory;
    }

    QList<Handle::HandleItem*> AbstractScene::handles()
    {
        return m_handles.keys();
    }

    QGraphicsWidget * AbstractScene::form()
    {
        return m_form;
    }

    bool AbstractScene::hasZoomAbilities()
    {
        return true;
    }

    void AbstractScene::delItem( QGraphicsProxyWidget * item )
    {
        delItem( m_items[item] );
    }

    QGraphicsScene * AbstractScene::scene()
    {
        return &m_scene;
    }

}
