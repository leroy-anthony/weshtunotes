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

#include "TreeExplorer.h"

#include <QCoreApplication>
#include <QApplication>
#include <QFileInfo>
#include <QDir>

#include <KLineEdit>
#include <KLocalizedString>

#include "../basket/NewBasketDialog.h"
#include "../basket/ItemTreeBasket.h"
#include "../basket/BasketFactory.h"
#include "../scene/FreeScene.h"
#include "../scene/LayoutScene.h"
#include "../config/Configuration.h"

namespace Explorer
{

    TreeExplorer::TreeExplorer( QWidget * parent ):
            QTreeWidget(parent)
    {
        setHeaderLabel( tr("Paniers") );
        setHeaderHidden( false );
        setIconSize(QSize(24,24));
        setColumnCount(3);
    }

    TreeExplorer::~TreeExplorer()
    {
    }

    void TreeExplorer::delCurrentBasket()
    {
        Basket::ItemTreeBasket * b = dynamic_cast<Basket::ItemTreeBasket*>(currentItem());
        if ( b != 0 )
        {
            b->basket()->del();
            delete b;
        }
    }

    Basket::ItemTreeBasket * TreeExplorer::addToCurrentBasket()
    {
        return Basket::NewBasketDialog::getNewBasket( this, dynamic_cast<Basket::ItemTreeBasket*>(currentItem()) );
    }

    Basket::ItemTreeBasket * TreeExplorer::addBasketToRoot()
    {
        Basket::ItemTreeBasket * b = Basket::NewBasketDialog::getNewBasket( this, 0 );
        insertTopLevelItem(  topLevelItemCount(), b );

        return b;
    }

    bool TreeExplorer::findBasket( Basket::ItemTreeBasket * parent, const QString & name )
    {
        QList<QTreeWidgetItem*> items = findItems( name, Qt::MatchCaseSensitive | Qt::MatchRecursive );

        if (  items.size()>0 && items[0]->parent() == parent )
        {
            return true;
        }

        return false;
    }

    Basket::ItemTreeBasket * TreeExplorer::rootItem()
    {
        return static_cast<Basket::ItemTreeBasket*>(QTreeWidget::invisibleRootItem());
    }

    Basket::ItemTreeBasket * TreeExplorer::addBasket( Basket::ItemTreeBasket * parent, const QString & name )
    {
        Basket::ItemTreeBasket * b = 0;

        if ( !findBasket( parent, name ) )
        {
            if ( parent == 0 )
            {
                b = new Basket::ItemTreeBasket( parent, name, 0 );
                insertTopLevelItem(  topLevelItemCount(), b );
            }
            else
            {
                b = new Basket::ItemTreeBasket( parent, name, 0 );
            }
        }
        else
        {
            return 0;
        }

        setCurrentIndex( indexFromItem( b, 0 ) );

        b->basket()->load();

        return b;
    }

    void TreeExplorer::loadBaskets()
    {
        QStringList masterBasket = Config::Configuration::masterBaskets();
        for (int i = 0; i < masterBasket.size(); ++i)
        {
            loadBasket( masterBasket[i] );
        }

        sortByColumn( 1, Qt::AscendingOrder );
    }

    void TreeExplorer::saveBaskets()
    {
        QStringList masterBasket;
        int masterBasketCount = topLevelItemCount();
        for ( int i=0 ; i<masterBasketCount ; ++i )
        {
            QTreeWidgetItem * item = topLevelItem( i );
            Basket::ItemTreeBasket * b = dynamic_cast<Basket::ItemTreeBasket*>(item);
            b->basket()->save();
        }
    }

    void TreeExplorer::loadBasket( const QString & name )
    {
        loadBasket( 0, name );
    }

    void TreeExplorer::loadBasket( Basket::ItemTreeBasket * parent, const QString & name )
    {
        Basket::ItemTreeBasket * b = addBasket( parent, name );

        setCurrentItem( b );

        QStringList l = Config::Configuration::subDirs( b->basket()->configFilePath() );
        for ( int i=0 ; i<l.size() ; ++i )
        {
            loadBasket( b, l[i] );
        }
    }

    QTreeWidgetItem * TreeExplorer::loadFromConfigCurrentBasket()
    {
        setColumnCount(2);
        QString currentBasket = Config::Configuration::loadLastBasket();
        QList<QTreeWidgetItem*> baskets = findItems( currentBasket, Qt::MatchFixedString | Qt::MatchRecursive, 1 );
        setColumnCount(1);
        if ( baskets.size() > 0 )
        {
            expandItem(baskets[0]);
            setCurrentItem(baskets[0]);

            return baskets[0];
        }

        return 0;
    }


}
