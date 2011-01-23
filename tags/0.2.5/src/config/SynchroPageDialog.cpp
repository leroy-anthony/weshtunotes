#include "SynchroPageDialog.h"

#include <QDebug>

#include "../config/Configuration.h"
#include "../synchro/ConnectionFactory.h"

namespace Config
{

    SynchroPageDialog::SynchroPageDialog(QWidget *parent) :
            QWidget(parent)
    {
        setupUi( this );

        kcfg_connections->hide();

        connect(m_connections, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(connectionChanged()));
    }


    void SynchroPageDialog::connectionChanged()
    {
        kcfg_connections->clear();
        loadServices();
    }

    void SynchroPageDialog::saveServices()
    {

        loadServices();
    }

    void SynchroPageDialog::load()
    {
        QStringList checkedConnections= kcfg_connections->items();
        QStringList names = Synchro::ConnectionFactory::names();

        for (int i = 0; i < names.size(); ++i)
        {
            QListWidgetItem * item = new QListWidgetItem(names[i],m_connections);
            if ( checkedConnections.contains(names[i]) )
            {
                item->setCheckState(Qt::Checked);
            }
            else
            {
                item->setCheckState(Qt::Unchecked);
            }
        }
    }

    void SynchroPageDialog::loadServices()
    {
        for (int i = 0; i < m_connections->count(); ++i)
        {
            QListWidgetItem * item = m_connections->item(i);
            const QString service = item-> data(Qt::DisplayRole).toString();

            if ( item->checkState() == Qt::Checked )
            {
                kcfg_connections->insertItem(service);
            }
        }

        emit changed();
    }

}
