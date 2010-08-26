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

#ifndef NEWBASKETDIALOG_H
#define NEWBASKETDIALOG_H

#include <KDialog>

#include "../basket/ItemTreeBasket.h"
#include "../explorer/AbstractExplorer.h"

class KComboBox;
class KIconButton;
class KColorCombo;

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
        KColorCombo    * m_colorBackground;

    };

}

#endif // NEWBASKETDIALOG_H
