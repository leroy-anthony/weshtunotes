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

#include "NewBasketForm.h"
#include "../basket/ItemTreeBasket.h"
#include "../explorer/AbstractExplorer.h"
#include "../synchro/SynchroManager.h"
#include "../synchro/GoogleDocsConnection.h"

class KComboBox;
class KIconButton;
class KColorCombo;
class KTabWidget;

namespace Basket
{

    class NewBasketDialog : public KDialog, public Ui::NewBasketDialog
    {
        Q_OBJECT

    public:
        NewBasketDialog( Explorer::AbstractExplorer * basketExplorer, ItemTreeBasket * parent );

        static ItemTreeBasket * getNewBasket( Explorer::AbstractExplorer * basketExplorer, Basket::ItemTreeBasket * parent );

        int currentIndexTab();

    public slots:
        ItemTreeBasket * addBasket();
        ItemTreeBasket * addRemoteBasket();
        void changeTab( int index );
        void valid( const QString & text );

    private slots:
        void changeBasketType( int index );
        void getBasketsRemote();
        void enableGetBasketButton( const QString & text );

    private:
        int order();

        Explorer::AbstractExplorer * m_basketExplorer;
        ItemTreeBasket * m_parent;

        KTabWidget * m_basketsTab;

        QList<Synchro::SynchroManager*> m_managerConnections;
    };

}

#endif // NEWBASKETDIALOG_H
