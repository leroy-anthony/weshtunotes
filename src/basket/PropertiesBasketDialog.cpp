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

#include "PropertiesBasketDialog.h"

#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMap>

#include <KIconButton>
#include <KComboBox>
#include <KColorCombo>

#include "settings.h"
#include "../data/DataManager.h"
#include "../basket/BasketFactory.h"
#include "../tag/TagFactory.h"
#include "../scene/AbstractScene.h"

namespace Basket
{

    PropertiesBasketDialog::PropertiesBasketDialog( Explorer::AbstractExplorer * basketExplorer, Basket::ItemTreeBasket * item ):
            KDialog(0),
            m_basketExplorer(basketExplorer),
            m_itemBasket(item)
    {
        setCaption( "Propertie's Basket" );
        setButtons( KDialog::Ok | KDialog::Cancel );
        setModal(true);

        QWidget * mainWidgetLeft  = new QWidget( this );
        QWidget * mainWidgetRight = new QWidget( this );

        QLayout * layout = new QHBoxLayout( mainWidgetLeft );

        m_iconButton = new KIconButton();
        m_iconButton->setIcon( item->basket()->icon() );
        m_iconButton->setFixedSize( 64, 64 );

        layout->addWidget(m_iconButton);
        layout->addWidget(mainWidgetRight);


        QLabel * basketNamelabel = new QLabel("Name :");
        m_basketName = new QLineEdit();
        m_basketName->setText(item->basket()->name());

        QLabel * basketBackgroundColor = new QLabel("Background color :");
        m_colorBackground = new KColorCombo();
        m_colorBackground->setColor(item->basket()->backgroundColor());

        QGridLayout * gridLayout = new QGridLayout( mainWidgetRight );
        gridLayout->addWidget( basketNamelabel,       0, 0 );
        gridLayout->addWidget( m_basketName,          0, 1 );
        gridLayout->addWidget( basketBackgroundColor, 1, 0 );
        gridLayout->addWidget( m_colorBackground,     1, 1 );

        setMainWidget( mainWidgetLeft );

    }

    void PropertiesBasketDialog::accept()
    {
        m_itemBasket->basket()->setName( m_basketName->text() );
        m_itemBasket->basket()->setIcon( m_iconButton->icon() );

        m_itemBasket->basket()->setBackgroundColor(m_colorBackground->color().name());
        m_itemBasket->basket()->scene()->setBackgroundBrush(QColor(m_colorBackground->color()));

        m_itemBasket->setIcon( m_iconButton->icon() );
        m_itemBasket->setData( 0, Qt::DisplayRole, m_basketName->text() );

        m_itemBasket->basket()->save();

        KDialog::accept();
    }

}
