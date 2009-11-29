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

#include "AbstractScene.h"

#include <QScrollBar>

#include <settings.h>

#include "../config/Configuration.h"
#include "../config/VisualAspect.h"
#include "../handle/HandleItem.h"

namespace Scene
{

    AbstractScene::~AbstractScene()
    {
    }

    AbstractScene::AbstractScene(QWidget * parent):
            QGraphicsScene(parent),
            m_horizontalScrollBarValueView(0),
            m_verticalScrollBarValueView(0)
    {
        setSceneRect(-100000,-100000,200000,200000);
    }

    const QString & AbstractScene::type()
    {
        return m_type;
    }

    void AbstractScene::setType( const QString & type )
    {
        m_type = type;
    }

    void AbstractScene::addData( const QMimeData * data )
    {
    }

    const QString & AbstractScene::id()
    {
        return m_id;
    }

    void AbstractScene::buildListHandleToLoad( Handle::HandleItem * h, QStringList & l )
    {
        if ( h->size() > 0 )
        {
            QList<Handle::HandleItem*> handlesChild = h->children();
            for ( int j=0 ; j<handlesChild.size() ; ++j )
            {
                buildListHandleToLoad( handlesChild[j], l );
            }
            l << h->id();
        }
        else
        {
            l << h->id();
        }
    }

    void AbstractScene::save( const QString & id, const QString & fileName )
    {
        QList<Handle::HandleItem*> handles = m_handles.keys();

        Config::Configuration::clear( fileName );
        Config::Configuration settings( fileName );

        settings.setValue("scene","id",id);
        settings.setValue("scene","nbitems",handles.size());
        for (int i=0 ; i<handles.size() ; ++i)
        {
            QStringList listHandles;
            buildListHandleToLoad( handles[i], listHandles );

            if ( listHandles.size() > 0 )
            {
                settings.setValue("scene",QString("item%1").arg(i+1),listHandles);
            }
        }

        for ( int i=0 ; i<handles.size() ; ++i )
        {
            handles[i]->save(fileName);
        }

        saveViewOnDisk( fileName );
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
        Config::Configuration settings( fileName );

        settings.setValue("scene","transform_m11",m_transformView.m11());
        settings.setValue("scene","transform_m12",m_transformView.m12());
        settings.setValue("scene","transform_m21",m_transformView.m21());
        settings.setValue("scene","transform_m22",m_transformView.m22());
        settings.setValue("scene","transform_dx",m_transformView.dx());
        settings.setValue("scene","transform_dy",m_transformView.dy());

        settings.setValue("scene","hscroll",m_horizontalScrollBarValueView);
        settings.setValue("scene","vscroll",m_verticalScrollBarValueView);
        settings.setValue("scene","type",m_type);
    }

    void AbstractScene::loadViewFromDisk( const QString & fileName  )
    {
        Config::Configuration settings( fileName );

        m_transformView = QTransform(
            settings.valueGroup("scene","transform_m11",1).toInt(),
            settings.valueGroup("scene","transform_m12",0).toInt(),
            settings.valueGroup("scene","transform_m21",0).toInt(),
            settings.valueGroup("scene","transform_m22",1).toInt(),
            settings.valueGroup("scene","transform_dx",0).toInt(),
            settings.valueGroup("scene","transform_dy",0).toInt());

        m_horizontalScrollBarValueView = settings.valueGroup("scene","hscroll",0).toInt();
        m_verticalScrollBarValueView   = settings.valueGroup("scene","vscroll",0).toInt();
    }

    void AbstractScene::load( const QString & fileName )
    {
        Config::Configuration settings( fileName );

        QMap<QString,Handle::HandleItem*> handles;
        QMap<QString,Item::AbstractItem*> itemsToLoad;
        QList<QStringList> listItem;

        // récupère tous les noms des notes
        m_id = settings.valueGroup("scene","id","1");

        int size = settings.valueGroup("scene","nbitems",0).toInt();
        for ( int i=1 ; i<=size ; ++i )
        {
            listItem << settings.values("scene",QString("item%1").arg(i));
        }

        // pour chaque note
        for ( int j=0 ; j<listItem.size() ; ++j )
        {
            QStringList items = listItem[j];
            Handle::HandleItem * handle = 0;
            for (int i=0 ; i<items.size() ; ++i)
            {
                int x = settings.valueGroup(items[i],"x",0).toInt();
                int y = settings.valueGroup(items[i],"y",0).toInt();
                handle = newHandle( x, y, settings.valueGroup(items[i],"width", Settings::widthNote()).toInt() );
                handle->setId(items[i]);
                handles[ items[i] ] = handle;

                QStringList itemsToAdd = settings.values(items[i],"items");
                if ( itemsToAdd.size()>0 )
                {
                    for (int j = 0; j < itemsToAdd.size() ; ++j)
                    {
                        handle->add( handles[ itemsToAdd[j] ] );
                    }
                }
                else
                {
                    //TODO: replace me by item->load() ???
                    Item::AbstractItem * item = newItem( 0, 0 );
                    item->setItemColor( QColor(settings.valueGroup(items[i],"color", Settings::colorItem() )) );
                    handle->add( item );

                    QString id = settings.valueGroup(items[i],"data","");
                    item->setId(id);
                    itemsToLoad[ id ] = item;

                    QStringList namesTags = settings.values(items[i],"tags");
                    for ( int i=0 ; i<namesTags.size() ; ++i )
                    {
                        QStringList s = namesTags[i].split(":");
                        dynamic_cast<Item::NoteItem*>(item)->addTag(s[0],s[1]);
                    }
                }
            }
            addHandleToScene( handle );
        }

        //charge la note
        for ( int j=0 ; j<itemsToLoad.keys().size() ; ++j )
        {
            itemsToLoad[ itemsToLoad.keys()[j] ]->load( fileName );
        }


        loadViewFromDisk( fileName );
    }

    void AbstractScene::resize( QResizeEvent * event )
    {
    }

}
