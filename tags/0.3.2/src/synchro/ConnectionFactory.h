/*
*/

#ifndef CONNECTIONFACTORY_H
#define CONNECTIONFACTORY_H

#include <QList>
#include <QStringList>

class QAction;

namespace Synchro
{
    class AbstractConnection;

    class ConnectionFactory
    {
    public:
        enum Type { GOOGLE, MAX };

        ConnectionFactory();

        static AbstractConnection * newConnection( ConnectionFactory::Type type );

        static QList<QAction*> menus();

        static QStringList names();

        static bool synchroServiceIsOn( Type typeConnection );

        static QString connectionName( ConnectionFactory::Type type );


    };

}

#endif // CONNECTIONFACTORY_H
