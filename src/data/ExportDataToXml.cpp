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

#include "ExportDataToXml.h"

#include <QXmlStreamWriter>
#include <QFile>
#include <QDateTime>
#include <QTextStream>

#include "../explorer/ItemTreeBasket.h"
#include "../basket/AbstractBasket.h"
#include "../config/Configuration.h"
#include "../scene/AbstractScene.h"
#include "../handle/HandleItem.h"
#include "../item/AbstractItem.h"
#include "../data/DataManager.h"

namespace Data
{

    ExportDataToXml::ExportDataToXml()
    {
    }

    void ExportDataToXml::exploreBaskets( Explorer::ItemTreeBasket * itemBasket, QXmlStreamWriter & xmlWriter )
    {
        for ( int i=0 ; itemBasket != 0 && i<itemBasket->childCount() ; ++i )
        {
            Explorer::ItemTreeBasket * b = static_cast<Explorer::ItemTreeBasket*>(itemBasket->child(i));
            exportBasket( b, xmlWriter );
            exploreBaskets( b, xmlWriter );
        }
    }

    void ExportDataToXml::exportHandle( Item::AbstractItem * abstractItem, QXmlStreamWriter & xmlWriter )
    {
        xmlWriter.writeStartElement("item");
        xmlWriter.writeAttribute("id", abstractItem->id());
        exportConfigFile( Data::DataManager::itemPath( abstractItem->id() ), xmlWriter );
        xmlWriter.writeEndElement(); // item
    }

    void ExportDataToXml::exportHandle( Handle::HandleItem * handle, QXmlStreamWriter & xmlWriter )
    {
        Item::AbstractItem * item = handle->abstractItem();

        xmlWriter.writeStartElement("handle");
        xmlWriter.writeAttribute("id", handle->id());
        xmlWriter.writeAttribute("height", QString::number(handle->height(),10));
        xmlWriter.writeAttribute("width", QString::number(handle->width(),10));

        if ( item != 0 )
        {
            xmlWriter.writeAttribute("color", handle->defaultColor().name());

            exportHandle( item, xmlWriter );
        }
        else
        {
             QList<Handle::HandleItem*> handles = handle->children();
             for ( int i=0 ; i<handles.size() ; ++i )
             {
                 exportHandle( handles[i], xmlWriter );
             }

             exportConfigFile( Data::DataManager::basketsStorePath()+handle->configFile(), xmlWriter );
        }

        xmlWriter.writeEndElement(); // handle
    }

    void ExportDataToXml::exportScene( Scene::AbstractScene * scene, QXmlStreamWriter & xmlWriter )
    {
        xmlWriter.writeStartElement("scene");
        xmlWriter.writeAttribute("id", scene->id());
        xmlWriter.writeAttribute("type", scene->type());

        QList<Handle::HandleItem*> handles = scene->handles();
        for ( int i=0 ; i<handles.size() ; ++i )
        {
            exportHandle( handles[i], xmlWriter );
        }

        xmlWriter.writeEndElement(); // scene
    }

    void ExportDataToXml::exportBasket( Explorer::ItemTreeBasket * itemBasket, QXmlStreamWriter & xmlWriter )
    {
        Basket::AbstractBasket * basket = itemBasket->basket();

        xmlWriter.writeStartElement("basket");
        xmlWriter.writeAttribute("id", basket->id());
        xmlWriter.writeAttribute("type", basket->type());
        xmlWriter.writeAttribute("icon", basket->icon());
        xmlWriter.writeAttribute("order", QString::number(basket->order(),10));

        exportScene( basket->scene(), xmlWriter );
        //exportConfigFile( Data::DataManager::basketsStorePath()+basket->configFilePath(), xmlWriter );

        xmlWriter.writeEndElement(); // basket
    }

    void ExportDataToXml::exportConfigFile( const QString & pathFile, QXmlStreamWriter & xmlWriter )
    {
        xmlWriter.writeStartElement("configfile");

        QFile configBasket(pathFile);
        if (configBasket.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&configBasket);
            QString text;
            while (!in.atEnd())
            {
                text += in.readLine()+"\n";
            }

            xmlWriter.writeCDATA(text);
        }

        xmlWriter.writeEndElement(); // config file
    }

    void ExportDataToXml::exportToFile( Explorer::ItemTreeBasket * root, const QString & filePath )
    {
        QFile file(filePath);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return;
        }

        QXmlStreamWriter xmlWriter(&file);

        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        xmlWriter.writeStartElement("kweshtunotes");
        xmlWriter.writeAttribute("version", "0.0");
        xmlWriter.writeAttribute("date", QDateTime::currentDateTime().toString());
        xmlWriter.writeStartElement("baskets");

        exploreBaskets( root, xmlWriter );

        xmlWriter.writeEndElement(); // baskets
        xmlWriter.writeEndElement(); // kweshtunotes

        xmlWriter.writeEndDocument();

    }

}
