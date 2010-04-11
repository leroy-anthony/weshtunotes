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

    void AbstractScene::addItemToScene( Handle::GraphicHandleItem * g )
    {
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

    void AbstractScene::save()
    {
        QList<Handle::HandleItem*> handles = m_handles.keys();

        Config::Configuration delSettings( m_fileName );
        QStringList listFile = delSettings.values("scene","items");
        for ( int i=0 ; i<listFile.size() ; ++i )
        {
            Config::Configuration::deleteFile(listFile[i]);
        }
        listFile.clear();

        Config::Configuration::clear( m_fileName );

        Config::Configuration settings( m_fileName );
        settings.setValue("scene","id",m_id);
        settings.setValue("scene","nbitems",handles.size());
        for (int i=0 ; i<handles.size() ; ++i)
        {
            QStringList listHandles;
            buildListHandleToLoad( handles[i], listHandles );

            if ( listHandles.size() > 0 )
            {
                Config::Configuration settingsHandle( m_fileName+"_"+handles[i]->id() );
                settingsHandle.setValue("general","items",listHandles);
                listFile << m_fileName+"_"+handles[i]->id();
            }
        }

        settings.setValue("scene","items",listFile);

        for ( int i=0 ; i<handles.size() ; ++i )
        {
            handles[i]->save(m_fileName+"_"+handles[i]->id());
        }

        saveViewOnDisk( m_fileName );
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

    void AbstractScene::loadHandles( const QList<QString> & filesName, QPointF centerPt, int selectionWidth, int selectionHeigth )
    {
        int dx = 0;
        int dy = 0;

        for ( int j=0 ; j<filesName.size() ; ++j )
        {
            QString fileName = filesName[j];

            QMap<QString,Item::AbstractItem*> itemsToLoad;
            QMap<QString,Handle::HandleItem*> handles;

            Config::Configuration settingsHandle( fileName );

            QStringList items = settingsHandle.values("general","items");

            Handle::HandleItem * handle = 0;

            for (int i=0 ; i<items.size() ; ++i)
            {
                QString itemId = items[i];

                int x = settingsHandle.valueGroup( itemId, "x", 0 ).toInt();
                int y = settingsHandle.valueGroup( itemId, "y", 0 ).toInt();

                if ( j == 0 && centerPt != QPointF(0,0) )
                {
                    dx = centerPt.x()-x-selectionWidth/2;
                    dy = centerPt.y()-y-selectionHeigth/2;
                }

                handle = newHandle( x+dx, y+dy, settingsHandle.valueGroup( itemId, "width", Settings::widthNote()).toInt() );
                handle->setId( itemId );
                handle->regenerateId();

                handles[ itemId ] = handle;
                handle->setFileName( fileName );

                QStringList itemsToAdd = settingsHandle.values( itemId, "items" );
                if ( itemsToAdd.size()>0 )
                {
                    for (int k = 0; k < itemsToAdd.size() ; ++k)
                    {
                        handle->add( handles[ itemsToAdd[k] ] );
                    }
                }
                else
                {
                    //TODO: replace me by item->load() ???
                    Item::AbstractItem * item = newItem( 0, 0 );
                    item->setItemColor( QColor(settingsHandle.valueGroup(items[i],"color", Settings::colorItem() )) );
                    handle->add( item );

                    QString id = settingsHandle.valueGroup( itemId,"data","");
                    item->setId( id );
                    itemsToLoad[ id ] = item;

                    QStringList namesTags = settingsHandle.values( itemId, "tags" );
                    for ( int k=0 ; k<namesTags.size() ; ++k )
                    {
                        QStringList s = namesTags[k].split(":");
                        dynamic_cast<Item::NoteItem*>(item)->addTag(s[0],s[1]);
                    }
                }
            }

            addHandleToScene( handle );

            //charge la note
            for ( int k=0 ; k<itemsToLoad.keys().size() ; ++k )
            {
                Item::AbstractItem * item = itemsToLoad[ itemsToLoad.keys()[k] ];
                item->load( fileName );
            }
        }
    }

    void AbstractScene::load( const QString & fileName )
    {
        Config::Configuration settings( fileName );

        m_id = settings.valueGroup("scene","id","1");
        m_fileName = fileName;

        loadHandles( settings.values("scene","items") );

        loadViewFromDisk( fileName );
    }

    void AbstractScene::resize( QResizeEvent * event )
    {
    }

}