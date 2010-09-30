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
#include <KLocalizedString>

#include "settings.h"
#include "../data/DataManager.h"
#include "../basket/BasketFactory.h"
#include "../tag/TagFactory.h"

namespace Basket
{

    NewBasketDialog::NewBasketDialog( Explorer::AbstractExplorer * basketExplorer, Basket::ItemTreeBasket * parent ):
            KDialog(0),
            m_basketExplorer(basketExplorer),
            m_parent(parent),
            m_managerCx(new Synchro::GoogleDocsConnection())
    {

        setCaption( i18n("Create Basket") );
        setButtons( KDialog::Ok | KDialog::Cancel );
        setModal( true );

        button( KDialog::Ok )->setEnabled(false);

        m_basketsTab = new KTabWidget();
        setupUi( m_basketsTab );
        setMainWidget( m_basketsTab );

        m_iconButton->setIcon( Settings::iconBasket() );
        m_iconButton->setFixedSize( 64, 64 );
        m_colorBackground->setColor(Settings::colorBasket());

        m_typeAccountRemote->addItem( m_managerCx.connectionName() );

        connect( m_basketTypeCombo,    SIGNAL(currentIndexChanged(int)),    this, SLOT(changeBasketType(int)) );
        connect( m_getBasketsRemote,   SIGNAL(released()),                  this, SLOT(getBasketsRemote()) );
        connect( m_basketsTab,         SIGNAL(currentChanged(int)),         this, SLOT(changeTab(int)) );
        connect( m_basketName,         SIGNAL(textChanged(const QString&)), this, SLOT(valid(const QString&)) );

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

    void NewBasketDialog::changeTab( int index )
    {
        button( KDialog::Ok )->setEnabled(false);

        if ( index == 0 && !m_basketName->text().isEmpty() )
        {
            button( KDialog::Ok )->setEnabled(true);
        }

        if ( index == 1 && !m_basketsRemote->currentText().isEmpty() )
        {
            button( KDialog::Ok )->setEnabled(true);
        }
    }

    void NewBasketDialog::valid( const QString & text )
    {
        button( KDialog::Ok )->setEnabled( !text.isEmpty() );
    }

    void NewBasketDialog::enableGetBasketButton( const QString & text )
    {
        m_getBasketsRemote->setEnabled(!text.isEmpty());
    }

    void NewBasketDialog::getBasketsRemote()
    {
        QStringList ids = m_managerCx.baskets();
        m_basketsRemote->clear();
        for ( int i=0 ; i<ids.size() ; ++i )
        {
            int index = ids[i].indexOf(":");
            m_basketsRemote->addItem(ids[i].midRef(index+1).toString(),ids[i].midRef(0,index).toString());
        }

        button( KDialog::Ok )->setEnabled(!ids.isEmpty());
    }

    void NewBasketDialog::changeBasketType( int index )
    {
        m_basketTagCombo->setEnabled( index != 0 );
    }

    int NewBasketDialog::order()
    {
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

        return order;
    }

    ItemTreeBasket * NewBasketDialog::addRemoteBasket()
    {
        QString id = m_basketsRemote->itemData(m_basketsRemote->currentIndex()).toString();

        m_managerCx.update(id);

        QMap<QString,QString> options;

        ItemTreeBasket * item = new ItemTreeBasket( m_parent, id, order(), options, "" );

        item->basket()->load();

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

        ItemTreeBasket * item = new ItemTreeBasket( m_parent, "", order(), options, type );
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

        item->basket()->save();

        return item;
    }

    int NewBasketDialog::currentIndexTab()
    {
        return m_basketsTab->currentIndex();
    }

    ItemTreeBasket * NewBasketDialog::getNewBasket( Explorer::AbstractExplorer * basketExplorer, Basket::ItemTreeBasket * parent )
    {
        NewBasketDialog newBasketDialog( basketExplorer, parent );

        if ( newBasketDialog.exec() == KDialog::Accepted )
        {
            if ( newBasketDialog.currentIndexTab() == 0 )
            {
                return newBasketDialog.addBasket();
            }
            else
            {
                return newBasketDialog.addRemoteBasket();
            }
        }

        return 0;
    }

}
