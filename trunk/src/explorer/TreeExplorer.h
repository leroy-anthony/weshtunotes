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

#ifndef TREEEXPLORER_H_
#define TREEEXPLORER_H_

#include <QTreeWidget>

#include "../explorer/AbstractExplorer.h"
#include "../explorer/ItemTreeBasket.h"

namespace Basket
{
    class ClipperBasket;
}

namespace Explorer
{

    class TreeExplorer : public QTreeWidget, public AbstractExplorer
    {
        Q_OBJECT

    public:
        TreeExplorer( QWidget * parent );
        ~TreeExplorer();

        QTreeWidgetItem * loadFromConfigCurrentBasket();

        ItemTreeBasket * rootItem();
        Basket::ClipperBasket * clipperBasket();

        void saveBaskets( QTreeWidgetItem * b );

    public slots:
        void saveBaskets();
        void delCurrentBasket();
        ItemTreeBasket * addBasketToRoot();
        ItemTreeBasket * addToCurrentBasket();
        void showPropertiesBasket();

        void loadBaskets();
        void loadBasket( const QString & name );

        void showMenuContext( const QPoint & );

    signals:
        void delCurrentBasketRequest();
        void addToCurrentBasketRequest( Explorer::ItemTreeBasket * );
    
    protected:
        void dropEvent ( QDropEvent * event );
        void startDrag( Qt::DropActions supportedActions );
        
    private:
        ItemTreeBasket * addBasket( ItemTreeBasket * parent, const QString & name );
        void loadBasket( ItemTreeBasket * parent, const QString & name );
        Basket::ClipperBasket * m_clipperBasket;

    };

}

#endif // TREEEXPLORER_H_
