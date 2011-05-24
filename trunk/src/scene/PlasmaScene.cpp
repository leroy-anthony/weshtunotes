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

#include "PlasmaScene.h"

#include <KDE/Plasma/Applet>
#include <KDE/Plasma/Containment>
#include <KDE/Plasma/Corona>
#include <Plasma/Service>
#include <Plasma/DataEngine>
#include <KDE/KServiceTypeTrader>

#include <QGraphicsSceneMouseEvent>
#include <QListWidgetItem>

#include "../scene/NewAppletDialog.h"
#include "../config/ImageFactory.h"

namespace Scene
{

    PlasmaScene::PlasmaScene():
        Scene::AbstractScene()
    {
        m_corona.setBackgroundBrush(QColor(Qt::cyan).lighter(190));

        m_type = "plasmascene";

        m_corona.initializeLayout();

        QList<Plasma::Containment*> cs = m_corona.containments();
        for ( int i=0 ; i<cs.size() ; ++i )
        {
            KConfigGroup cg = cs[i]->config();
            cs[i]->restore(cg);
        }

        m_corona.installEventFilter(this);
    }

    bool PlasmaScene::eventFilter(QObject *obj, QEvent *event)
    {
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            QGraphicsSceneMouseEvent * mouseButtonPress = static_cast<QGraphicsSceneMouseEvent *>(event);
            mousePressEvent(mouseButtonPress);
        }

        // standard event processing
        return QObject::eventFilter(obj, event);
    }

    void PlasmaScene::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
    {
        if ( mouseEvent->button() == Qt::RightButton )
        {
            NewAppletDialog dialog;

            KService::List offers = KServiceTypeTrader::self()->query("Plasma/Applet", 0);
            foreach (const KPluginInfo & info, KPluginInfo::fromServices(offers))
            {
                if (info.property("NoDisplay").toBool()) {
                    continue;
                }

                QString name = info.pluginName();

                QListWidgetItem * item = new QListWidgetItem();
                item->setIcon(Config::ImageFactory::newInstance()->icon(info.icon()));
                item->setText(info.pluginName());
                item->setToolTip(info.comment());

                dialog.listWidget()->addItem(item);
            }

            if ( dialog.exec() == KDialog::Accepted )
            {
                if ( dialog.listWidget()->currentItem() != 0 )
                {
                    QList<Plasma::Containment*> cs = m_corona.containments();
                    for ( int i=0 ; i<cs.size() ; ++i )
                    {
                        cs[i]->addApplet( dialog.listWidget()->currentItem()->text() );
                    }
                }
            }
        }
    }

    void PlasmaScene::load( const QString & fileName )
    {
    }

    void PlasmaScene::save()
    {
    }

    Item::AbstractItem * PlasmaScene::currentAbstractItem()
    {
        return 0;
    }

    Handle::HandleItem * PlasmaScene::currentHandle()
    {
        return 0;
    }

    void PlasmaScene::addItemToScene( Handle::GraphicHandleItem * item )
    {
    }

    QGraphicsProxyWidget * PlasmaScene::addHandleToScene( Handle::HandleItem * handle )
    {
        return 0;
    }

    Handle::HandleItem * PlasmaScene::newHandle( int x, int y )
    {
        return 0;
    }

    Handle::HandleItem * PlasmaScene::addData( const QMimeData * data )
    {
        return 0;
    }

    void PlasmaScene::delItem( Handle::HandleItem * h )
    {
    }

    void PlasmaScene::moveItem( Handle::HandleItem * handleItem, int x, int y )
    {
    }

    QGraphicsScene * PlasmaScene::scene()
    {
        return &m_corona;
    }

}
