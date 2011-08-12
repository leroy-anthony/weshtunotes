/*
*/

#include "ConnectionFactory.h"

#include <QAction>

#include <KLocalizedString>

#include "../synchro/GoogleDocsConnection.h"
#include "../config/Configuration.h"

namespace Synchro
{

    ConnectionFactory::ConnectionFactory()
    {
    }


    QList<QAction*> ConnectionFactory::menus()
    {
        QList<QAction*> actions;

        QAction * a = new QAction(i18n("Load from google"),0);
        a->setData(GOOGLE);

        actions << a;

        a = new QAction(i18n("Save on google"),0);
        a->setData(GOOGLE);

        actions << a;

        return actions;
    }

    AbstractConnection * ConnectionFactory::newConnection( ConnectionFactory::Type type )
    {
        if ( type == GOOGLE )
        {
            return new GoogleDocsConnection();
        }

        return 0;
    }

    QString ConnectionFactory::connectionName( ConnectionFactory::Type type )
    {
        if ( type == GOOGLE )
        {
            return "Google Document";
        }

        return 0;
    }

    QStringList ConnectionFactory::names()
    {
        QStringList names;

        for ( int i=0 ; i<MAX ; ++i )
        {
            AbstractConnection * cx = newConnection( ConnectionFactory::Type(i) );
            names << cx->connectionName();
            delete cx;
        }

        return names;
    }

    bool ConnectionFactory::synchroServiceIsOn( Type typeConnection )
    {
        Config::Configuration settings;
        QStringList connections = settings.values( "synchro", "connections" );

        return connections.contains( connectionName(typeConnection) );
    }

}
