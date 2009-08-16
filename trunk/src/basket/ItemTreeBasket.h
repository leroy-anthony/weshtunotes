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

#ifndef ITEMTREEBASKET_H_
#define ITEMTREEBASKET_H_

#include <QTreeWidgetItem>

#include "../main/general.h"

#include "../explorer/TreeExplorer.h"
#include "../scene/AbstractScene.h"

namespace Basket
{

    class ItemTreeBasket : public QTreeWidgetItem
    {
    public:
        ItemTreeBasket( Explorer::TreeExplorer * treeExplorer, const QString & name );
        ItemTreeBasket( Basket::ItemTreeBasket * itemTreeBasket, const QString & name );
        ~ItemTreeBasket();

        void load();
        void save();

        const QString & name();
        const QString & folder();
        const QString & directory();

        Scene::AbstractScene * scene();
        void setScene( Scene::AbstractScene * scene);

        void del();

         void setBasketId( const QString & id );
         const QString & basketId();

    private:
        void initItemTreeBasket();

        QString m_directory;
        QString m_folder;
        QString m_name;
        Scene::AbstractScene * m_contentScene;

        QString m_basketId;
        static int m_id;

    };

}

#endif // ITEMTREEBASKET_H_
