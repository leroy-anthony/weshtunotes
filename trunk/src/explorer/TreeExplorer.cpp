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
#include <QFileInfo>
#include <QDir>
#include <QInputDialog>

#include "../scene/FreeScene.h"
#include "../basket/ItemTreeBasket.h"
#include "../config/Configuration.h"

namespace Explorer
{

    TreeExplorer::TreeExplorer( QWidget * parent ):
            QTreeWidget(parent)
    {
    }

    TreeExplorer::~TreeExplorer()
    {
    }

    void TreeExplorer::delCurrentBasket()
    {
        Basket::ItemTreeBasket * b = dynamic_cast<Basket::ItemTreeBasket*>(currentItem());
        if ( b != 0 )
        {
            b->del();
            delete b;
        }
    }

    Basket::ItemTreeBasket * TreeExplorer::addToCurrentBasket()
    {
        bool ok;
        QString name = QInputDialog::getText(this, tr("Basket name"),
                                             tr("Basket name:"), QLineEdit::Normal,
                                             "My basket", &ok);

        if ( ok && !name.isEmpty() )
        {
            return addBasket( dynamic_cast<Basket::ItemTreeBasket*>(currentItem()), name );
        }

        return 0;
    }

    Basket::ItemTreeBasket * TreeExplorer::addBasketToRoot()
    {
        bool ok;
        QString name = QInputDialog::getText(this, tr("Basket name"),
                                             tr("Basket name:"), QLineEdit::Normal,
                                             "My basket", &ok);

        if ( ok && !name.isEmpty() )
        {
            return addBasket( 0, name );
        }

        return 0;
    }

    Basket::ItemTreeBasket * TreeExplorer::addBasket( Basket::ItemTreeBasket * parent, const QString & name )
    {
        Basket::ItemTreeBasket * b = 0;
        
        QList<QTreeWidgetItem*> items = findItems( name, Qt::MatchCaseSensitive | Qt::MatchRecursive );
        if ( parent != 0 )
        {
            if (  items.size()>0 && items[0]->parent() == parent )
            {
                return 0;
            }
            b = new Basket::ItemTreeBasket( parent, name );
        }
        else
        {
            if (  items.size()>0 && items[0]->parent() == 0 )
            {
                return 0;
            }
            b = new Basket::ItemTreeBasket( this, name );
            insertTopLevelItem(  topLevelItemCount(), b );
        }

        b->setScene( new Scene::FreeScene( 0 ) );

        return b;
    }

    void TreeExplorer::loadBaskets()
    {
        Config::Configuration settings;
        QStringList masterBasket = settings.masterBaskets();
        for (int i = 0; i < masterBasket.size(); ++i)
        {
            loadBasket( masterBasket[i] );
        }

    }

    void TreeExplorer::saveBaskets()
    {
        QStringList masterBasket;
        int masterBasketCount = topLevelItemCount();
        for ( int i=0 ; i<masterBasketCount ; ++i )
        {
            QTreeWidgetItem * item = topLevelItem( i );
            Basket::ItemTreeBasket * b = dynamic_cast<Basket::ItemTreeBasket*>(item);
            b->save();
            masterBasket << b->name();
        }

        Config::Configuration::saveMasterBaskets( masterBasket );
    }

    void TreeExplorer::loadBasket( const QString & name )
    {
        loadBasket( 0, name );
    }

    void TreeExplorer::loadBasket( Basket::ItemTreeBasket * parent, const QString & name )
    {
        Basket::ItemTreeBasket * b = addBasket( parent, name );
        b->load();
        setCurrentItem( b );

        QStringList l = Config::Configuration::subDirs( b->directory() );
        for ( int i=0 ; i<l.size() ; ++i )
        {
            loadBasket( b, l[i] );
        }
    }

}
