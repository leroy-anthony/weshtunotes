/*
    Copyright (c) 2011 LEROY Anthony <leroy.anthony@gmail.com>

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

#include "ClipperBasket.h"
#include "../config/ImageFactory.h"
#include "../basket/ItemTreeBasket.h"
#include "../basket/BasketFactory.h"
#include "../scene/AbstractScene.h"
#include "../scene/SceneFactory.h"
#include "../data/DataManager.h"
#include "../data/AssociationManager.h"
#include "../handle/HandleItem.h"
#include "../scene/CustomGraphicsView.h"
#include "settings.h"

#include <QApplication>
#include <QClipboard>
#include <QMap>
#include <QGraphicsLayout>

namespace Basket
{

    ClipperBasket::ClipperBasket( ItemTreeBasket * itemTreeBasket, const QString & id ):
        AbstractBasket( itemTreeBasket, id ),
        m_maxItem(Settings::maxItem())
    {
        m_name = "Clipper";
        m_icon = "klipper.png";
        m_backgroundColor = "darkCyan";
        m_type = BasketFactory::type(BasketFactory::BASKET);
        m_order = -1;
        m_immuable = true;

        itemTreeBasket->setIcon(m_icon);

        m_clipboard = QApplication::clipboard();
        connect( m_clipboard, SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));

        m_contentScene = Scene::SceneFactory::newScene( m_configFile, Scene::SceneFactory::LAYOUTSCENE, "clipper" );

        save();
    }

    void ClipperBasket::save()
    {
        m_contentScene->save();
        AbstractBasket::save();
    }

    void ClipperBasket::load()
    {
        AbstractBasket::load();

        delete m_contentScene;

        m_contentScene = Scene::SceneFactory::newScene( m_configFile, Scene::SceneFactory::LAYOUTSCENE, "clipper" );
        m_contentScene->load( m_configFile );;
        m_contentScene->loadHandles( Data::AssociationManager::abstractNotes( m_contentScene->id() ) );
        m_contentScene->scene()->setBackgroundBrush(QBrush(QColor(m_backgroundColor)));
    }

    void ClipperBasket::del()
    {
        Data::AssociationManager::removeNotes( m_contentScene->id() );
        Data::DataManager::removeBasket( m_configFile );
    }

    void ClipperBasket::clipboardDataChanged()
    {
        const QMimeData * mimeData = m_clipboard->mimeData();

        Handle::HandleItem * handle = m_contentScene->addData( mimeData );
        if ( handle != 0 )
        {
            handle->setLock();
        }

        QGraphicsWidget * f = m_contentScene->form();
        if ( f->layout()->count() > Settings::maxItem() )
        {
            QGraphicsProxyWidget * item = static_cast<QGraphicsProxyWidget*>(f->layout()->itemAt(0));
            m_contentScene->delItem(item);
        }

        QList<QGraphicsView*> viewList = m_contentScene->scene()->views();
        if ( viewList.size() > 0 )
        {
            QGraphicsView * view = viewList[0];
            static_cast<Scene::CustomGraphicsView*>(view)->adjustLayoutSize();
        }

        save();
    }

}
