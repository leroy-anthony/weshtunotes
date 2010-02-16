/*
 Copyright (c) 2010 LEROY Anthony <leroy.anthony@gmail.com>

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

#ifndef NEWBASKETDIALOG_H
#define NEWBASKETDIALOG_H

#include <KDialog>
#include <KIconButton>
#include <KComboBox>

#include "../basket/ItemTreeBasket.h"
#include "../explorer/AbstractExplorer.h"

namespace Basket
{

    class NewBasketDialog : public KDialog
    {
        Q_OBJECT

    public:
        NewBasketDialog( Explorer::AbstractExplorer * basketExplorer, ItemTreeBasket * parent );

        static ItemTreeBasket * getNewBasket( Explorer::AbstractExplorer * basketExplorer, Basket::ItemTreeBasket * parent );

    public slots:
        ItemTreeBasket * addBasket();

    private slots:
        void changeBasketType( int index );

    private:
        Explorer::AbstractExplorer * m_basketExplorer;

        QLineEdit      * m_basketName;
        ItemTreeBasket * m_parent;
        KIconButton    * m_iconButton;
        KComboBox      * m_basketTypeCombo;
        KComboBox      * m_basketTagCombo;

    };

}

#endif // NEWBASKETDIALOG_H
