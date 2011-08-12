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
#include "../basket/AbstractBasket.h"
#include "../synchro/ConnectionFactory.h"
#include "../synchro/SynchroManager.h"
#include "../explorer/AbstractExplorer.h"
#include "../explorer/ItemTreeBasket.h"
#include "../tag/TagFactory.h"

namespace Basket
{

    NewBasketDialog::NewBasketDialog( Explorer::AbstractExplorer * basketExplorer, Explorer::ItemTreeBasket * parent ):
            KDialog(0),
            m_basketExplorer(basketExplorer),
            m_parent(parent)
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

        for (int i=0 ; i<Synchro::ConnectionFactory::MAX ; ++i)
        {
            Synchro::ConnectionFactory::Type type = Synchro::ConnectionFactory::Type(i);
            Synchro::AbstractConnection * cx = Synchro::ConnectionFactory::newConnection(type);
            m_managerConnections << new Synchro::SynchroManager(cx);
            m_typeAccountRemote->addItem( cx->connectionName() );
        }

        for ( int i=0 ; i<BasketFactory::MAX ; ++i )
        {
            QString type = BasketFactory::type(i);
            if ( type == "tag_basket")
            {
                if ( Settings::useNepomuk() )
                {
                    m_basketTypeCombo->addItem( BasketFactory::label(i), type );
                }
            }
            else
            {
                m_basketTypeCombo->addItem( BasketFactory::label(i), type );
            }
        }

        m_basketTagCombo->setEnabled( false );
        QStringList tags = Tag::TagFactory::tagsNames();
        for ( int i=0 ; i<tags.size() ; ++i )
        {
            m_basketTagCombo->addItem( tags[i] );
        }

        connect( m_basketTypeCombo,    SIGNAL(currentIndexChanged(int)),    this, SLOT(changeBasketType(int)) );
        connect( m_getBasketsRemote,   SIGNAL(released()),                  this, SLOT(getBasketsRemote()) );
        connect( m_basketsTab,         SIGNAL(currentChanged(int)),         this, SLOT(changeTab(int)) );
        connect( m_basketName,         SIGNAL(textChanged(const QString&)), this, SLOT(valid(const QString&)) );
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
        int index = m_typeAccountRemote->currentIndex();
        QStringList ids = m_managerConnections[index]->baskets();
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
        m_basketTagCombo->setEnabled( index == 1 );
    }

    int NewBasketDialog::order()
    {
        Explorer::ItemTreeBasket * parent = m_parent;
        if ( parent == 0 )
        {
            parent = m_basketExplorer->rootItem();
        }

        Explorer::ItemTreeBasket * brother = static_cast<Explorer::ItemTreeBasket*>( parent->child(parent->childCount()-1) );
        int order = 0;
        if ( brother != 0 )
        {
            order = brother->basket()->order()+1;
        }

        return order;
    }

    Explorer::ItemTreeBasket * NewBasketDialog::addRemoteBasket()
    {
        QString id = m_basketsRemote->itemData(m_basketsRemote->currentIndex()).toString();

        int index = m_typeAccountRemote->currentIndex();
        m_managerConnections[index]->update(id);

        QMap<QString,QString> options;

        Explorer::ItemTreeBasket * item = new Explorer::ItemTreeBasket( m_parent, id, order(), options, "" );

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

    Explorer::ItemTreeBasket * NewBasketDialog::addBasket()
    {
        QString name = m_basketName->text();

        QString colorBackground = m_colorBackground->color().name();

        QString type = m_basketTypeCombo->itemData(m_basketTypeCombo->currentIndex(),Qt::UserRole).toString();

        QMap<QString,QString> options;
        if ( m_basketTypeCombo->currentIndex() == 1 )
        {
            options["tagName"] = m_basketTagCombo->currentText();
        }

        Explorer::ItemTreeBasket * item = new Explorer::ItemTreeBasket( m_parent, "", order(), options, type );
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

    Explorer::ItemTreeBasket * NewBasketDialog::getNewBasket( Explorer::AbstractExplorer * basketExplorer, Explorer::ItemTreeBasket * parent )
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
