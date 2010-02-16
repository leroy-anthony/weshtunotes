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

#ifndef TREEEXPLORER_H_
#define TREEEXPLORER_H_

#include <QTreeWidget>

#include "../explorer/AbstractExplorer.h"

namespace Basket
{
    class ItemTreeBasket;
}

namespace Explorer
{

    class TreeExplorer : public QTreeWidget, public AbstractExplorer
    {
        Q_OBJECT

    public:
        TreeExplorer( QWidget * parent );
        virtual ~TreeExplorer();

        QTreeWidgetItem * loadFromConfigCurrentBasket();

        bool findBasket( Basket::ItemTreeBasket * parent, const QString & name );

    public slots:
        void delCurrentBasket();
        Basket::ItemTreeBasket * addBasketToRoot();
        Basket::ItemTreeBasket * addToCurrentBasket();

        void loadBaskets();
        void loadBasket( const QString & name );
        void saveBaskets();

    private:
        Basket::ItemTreeBasket * addBasket( Basket::ItemTreeBasket * parent, const QString & name );
        void loadBasket( Basket::ItemTreeBasket * parent, const QString & name );

    };

}

#endif // TREEEXPLORER_H_
