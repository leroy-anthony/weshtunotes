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

#include "TreeExplorer.h"

#include <QCoreApplication>
#include <QApplication>
#include <QFileInfo>
#include <QDir>
#include <QMenu>
#include <QPair>

#include <KLineEdit>
#include <KLocalizedString>
#include <KMessageBox>

#include "../basket/NewBasketDialog.h"
#include "../basket/PropertiesBasketDialog.h"
#include "../basket/ItemTreeBasket.h"
#include "../basket/BasketFactory.h"
#include "../scene/FreeScene.h"
#include "../scene/LayoutScene.h"
#include "../config/Configuration.h"
#include "../config/ImageFactory.h"
#include "../basket/ItemTreeBasket.h"
#include "../synchro/ConnectionFactory.h"
#include "../basket/ClipperBasket.h"

namespace Explorer
{

    TreeExplorer::TreeExplorer( QWidget * parent ):
            QTreeWidget(parent),
            m_clipperBasket(0)
    {
        setHeaderLabel( i18n("My baskets") );
        setHeaderHidden( false );
        setIconSize(QSize(24,24));
        setColumnCount(3);

        setDragDropMode( QAbstractItemView::InternalMove );

        setContextMenuPolicy(Qt::CustomContextMenu);

        connect( this, SIGNAL(customContextMenuRequested( const QPoint & )), this, SLOT(showMenuContext( const QPoint & )) );


        if ( Settings::useClipper() )
        {
            Basket::ItemTreeBasket * item = new Basket::ItemTreeBasket( 0, "Clipper", 0 );
            m_clipperBasket = new Basket::ClipperBasket(item,"clipper");
            item->setBasket(m_clipperBasket);
            insertTopLevelItem(  topLevelItemCount(), item );
        }
    }

    TreeExplorer::~TreeExplorer()
    {
        if ( m_clipperBasket != 0 )
        {
            m_clipperBasket->del();
        }
    }

    void TreeExplorer::delCurrentBasket()
    {
        Basket::ItemTreeBasket * b = dynamic_cast<Basket::ItemTreeBasket*>(currentItem());
        if ( b->basket()->isImmuable() )
        {
            KMessageBox::information( 0, i18n("This basket cannot be delete."),
                                     i18n("Delete basket"));
        }
        else
        {
            int reply = KMessageBox::questionYesNo(0, i18n("Do you really want to delete this basket ?"),
                                                   i18n("Delete basket"),
                                                   KStandardGuiItem::yes(),
                                                   KStandardGuiItem::no(),
                                                   i18n("delete basket"));
            if (reply == KMessageBox::Yes)
            {
                if ( b != 0 )
                {
                    b->basket()->del();
                    delete b;

                    saveBaskets();
                }

                emit delCurrentBasketRequest();
            }
        }
    }

    Basket::ItemTreeBasket * TreeExplorer::addToCurrentBasket()
    {
        Basket::ItemTreeBasket * item = Basket::NewBasketDialog::getNewBasket( this, dynamic_cast<Basket::ItemTreeBasket*>(currentItem()) );

        setCurrentItem(item);

        emit addToCurrentBasketRequest( item );

        return item;
    }

    Basket::ItemTreeBasket * TreeExplorer::addBasketToRoot()
    {
        Basket::ItemTreeBasket * item = Basket::NewBasketDialog::getNewBasket( this, 0 );
        insertTopLevelItem(  topLevelItemCount(), item );

        setCurrentItem(item);

        emit addToCurrentBasketRequest( item );

        return item;
    }

    void TreeExplorer::showPropertiesBasket()
    {
        Basket::ItemTreeBasket * b = dynamic_cast<Basket::ItemTreeBasket*>(currentItem());
        if ( b != 0 )
        {
            Basket::PropertiesBasketDialog prop(this, b);
            prop.exec();
        }
    }

    Basket::ItemTreeBasket * TreeExplorer::rootItem()
    {
        return static_cast<Basket::ItemTreeBasket*>(QTreeWidget::invisibleRootItem());
    }

    void TreeExplorer::saveBaskets( QTreeWidgetItem * b )
    {
        QStringList childBasket;

        int childSize = b->childCount();
        for ( int i=0 ; i<childSize ; ++i )
        {
            Basket::ItemTreeBasket * child = static_cast<Basket::ItemTreeBasket*>(b->child(i));

            childBasket += child->basket()->id();

            saveBaskets( child );
        }

        if ( b != invisibleRootItem ()  )
        {
            Data::DataManager::addBasket(static_cast<Basket::ItemTreeBasket*>(b)->basket()->id(),childBasket);
        }
        else
        {
            Data::DataManager::addMasterBasket(childBasket);
        }
    }

    void TreeExplorer::saveBaskets()
    {
        Data::DataManager::removeBaskets();
        saveBaskets( invisibleRootItem() );
    }

    Basket::ItemTreeBasket * TreeExplorer::addBasket( Basket::ItemTreeBasket * parent, const QString & id )
    {
        Basket::ItemTreeBasket * b = 0;

        if ( parent == 0 )
        {
            b = new Basket::ItemTreeBasket( parent, id, 0 );
            insertTopLevelItem(  topLevelItemCount(), b );
        }
        else
        {
            b = new Basket::ItemTreeBasket( parent, id, 0 );
        }

        setCurrentIndex( indexFromItem( b, 0 ) );

        b->basket()->load();

        return b;
    }

    void TreeExplorer::loadBaskets()
    {
        QStringList masterBasket = Data::DataManager::masterBaskets();
        for (int i = 0; i < masterBasket.size(); ++i)
        {
            loadBasket( masterBasket[i] );
        }

        sortByColumn( 1, Qt::AscendingOrder );
    }

    void TreeExplorer::loadBasket( const QString & id )
    {
        loadBasket( 0, id );
    }

    void TreeExplorer::loadBasket( Basket::ItemTreeBasket * parent, const QString & id )
    {
        Basket::ItemTreeBasket * b = addBasket( parent, id );

        setCurrentItem( b );

        QStringList l = Data::DataManager::subBaskets( id );
        for ( int i=0 ; i<l.size() ; ++i )
        {
            loadBasket( b, l[i] );
        }
    }

    QTreeWidgetItem * TreeExplorer::loadFromConfigCurrentBasket()
    {
        setColumnCount(3);
        QString currentBasket = Data::DataManager::loadLastBasket();
        QList<QTreeWidgetItem*> baskets = findItems( currentBasket, Qt::MatchFixedString | Qt::MatchRecursive, 2 );

        setColumnCount(1);
        if ( baskets.size() > 0 )
        {
            expandItem(baskets[0]);
            setCurrentItem(baskets[0]);

            return baskets[0];
        }

        return 0;
    }

    void TreeExplorer::dropEvent ( QDropEvent * event )
    {
        QTreeWidget::dropEvent( event );

        saveBaskets();
    }

    void TreeExplorer::startDrag( Qt::DropActions supportedActions )
    {
        QTreeWidget::startDrag( supportedActions );
    }

    void TreeExplorer::showMenuContext( const QPoint & point )
    {
        QTreeWidgetItem * item = 0 ;
        item = this->itemAt(point);

        if ( item != 0 )
        {
            Basket::ItemTreeBasket * basketItem = static_cast<Basket::ItemTreeBasket*>(item);

            QMenu menu(this);

            QAction * addAction = new QAction(i18n("Add basket"), this);
            menu.addAction(addAction);
            connect(addAction, SIGNAL(triggered()), this, SLOT(addToCurrentBasket()));
            addAction->setIcon(Config::ImageFactory::newInstance()->icon("list-add"));

            QAction * deleteAction = new QAction(i18n("Delete"), this);
            menu.addAction(deleteAction);
            connect(deleteAction, SIGNAL(triggered()), this, SLOT(delCurrentBasket()));
            deleteAction->setIcon(Config::ImageFactory::newInstance()->icon("edit-delete"));

            menu.addSeparator();

            QList<QAction*> actions = Synchro::ConnectionFactory::menus();
            for ( int i=0 ; i < actions.size() ; i+=2 )
            {
                if (  Synchro::ConnectionFactory::synchroServiceIsOn( (Synchro::ConnectionFactory::Type) actions[i]->data().toInt() ) )
                {
                    menu.addAction(actions[i]);
                    connect(actions[i], SIGNAL(triggered()), basketItem->basket(), SLOT(update()));

                    menu.addAction(actions[i+1]);
                    connect(actions[i+1], SIGNAL(triggered()), basketItem->basket(), SLOT(commit()));
                }
            }


            menu.addSeparator();

            QAction * propertyAction = new QAction(i18n("Properties"), this);
            menu.addAction(propertyAction);
            connect(propertyAction, SIGNAL(triggered()), this, SLOT(showPropertiesBasket()));

            menu.exec(mapToGlobal(point));

        }
    }

    Basket::ClipperBasket * TreeExplorer::clipperBasket()
    {
        return m_clipperBasket;
    }

}
