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

#include "NewBasketDialog.h"

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

namespace Basket
{

    NewBasketDialog::NewBasketDialog( Explorer::AbstractExplorer * basketExplorer, Basket::ItemTreeBasket * parent ):
            KDialog(0),
            m_basketExplorer(basketExplorer),
            m_parent(parent)
    {
        setCaption( "Create Basket" );
        setButtons( KDialog::Ok | KDialog::Cancel );
        setModal(true);

        QWidget * mainWidgetLeft  = new QWidget( this );
        QWidget * mainWidgetRight = new QWidget( this );

        QLayout * layout = new QHBoxLayout( mainWidgetLeft );

        m_iconButton = new KIconButton();
        m_iconButton->setIcon( Settings::iconBasket() );
        m_iconButton->setFixedSize( 64, 64 );

        layout->addWidget(m_iconButton);
        layout->addWidget(mainWidgetRight);


        QLabel * basketNamelabel = new QLabel("Name :");
        m_basketName = new QLineEdit();

        QLabel * basketBackgroundColor = new QLabel("Background color :");
        m_colorBackground = new KColorCombo();
        m_colorBackground->setColor(Settings::colorBasket());

        QLabel * basketTypelabel = new QLabel("Type:");
        m_basketTypeCombo = new KComboBox();
        connect( m_basketTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(changeBasketType(int)) );

        QLabel * basketTaglabel = new QLabel("Tag Name :");
        m_basketTagCombo  = new KComboBox();

        QGridLayout * gridLayout = new QGridLayout( mainWidgetRight );
        gridLayout->addWidget( basketNamelabel,       0, 0 );
        gridLayout->addWidget( m_basketName,          0, 1 );
        gridLayout->addWidget( basketBackgroundColor, 1, 0 );
        gridLayout->addWidget( m_colorBackground,     1, 1 );
        gridLayout->addWidget( basketTypelabel,       2, 0 );
        gridLayout->addWidget( m_basketTypeCombo,     2, 1 );
        gridLayout->addWidget( basketTaglabel,        3, 0 );
        gridLayout->addWidget( m_basketTagCombo,      3, 1 );

        setMainWidget( mainWidgetLeft );

        for ( int i=0 ; i<BasketFactory::MAX ; ++i )
        {
            m_basketTypeCombo->addItem( BasketFactory::label(i), BasketFactory::type(i) );
        }

        QStringList tags = Tag::TagFactory::tagsNames();
        for ( int i=0 ; i<tags.size() ; ++i )
        {
            m_basketTagCombo->addItem( tags[i] );
        }

    }

    void NewBasketDialog::changeBasketType( int index )
    {
        m_basketTagCombo->setEnabled( index != 0 );
    }

    ItemTreeBasket * NewBasketDialog::addBasket()
    {
        QString name = m_basketName->text();

        QString colorBackground = m_colorBackground->color().name();

        QString type = m_basketTypeCombo->itemData(m_basketTypeCombo->currentIndex(),Qt::UserRole).toString();

        QMap<QString,QString> options;
        if ( m_basketTypeCombo->currentIndex() == 1 )
        {
            options["tagName"] = m_basketTagCombo->currentText();
        }

        ItemTreeBasket * parent = m_parent;
        if ( parent == 0 )
        {
            parent = m_basketExplorer->rootItem();
        }

        ItemTreeBasket * brother = static_cast<ItemTreeBasket*>( parent->child(parent->childCount()-1) );
        int order = 0;
        if ( brother != 0 )
        {
            order = brother->basket()->order()+1;
        }

        ItemTreeBasket * item = new ItemTreeBasket( m_parent, "", order, options, type );
        item->setIcon( m_iconButton->icon() );
        item->basket()->setName(name);
        item->basket()->setBackgroundColor(colorBackground);

        if ( m_parent == 0 )
        {
            Data::DataManager::addMasterBasket( item->basket()->id() );
        }
        else
        {
            Data::DataManager::addBasket( m_parent->basket()->id(), item->basket()->id() );
        }

        return item;
    }

    ItemTreeBasket * NewBasketDialog::getNewBasket( Explorer::AbstractExplorer * basketExplorer, Basket::ItemTreeBasket * parent )
    {
        NewBasketDialog newBasketDialog( basketExplorer, parent );

        if ( newBasketDialog.exec() == KDialog::Accepted )
        {
            return newBasketDialog.addBasket();
        }

        return 0;
    }

}
