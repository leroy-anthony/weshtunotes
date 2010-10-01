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

#ifndef SYNCHROMANAGER_H
#define SYNCHROMANAGER_H

#include "settings.h"

#include <KWallet/Wallet>

#include "../synchro/AbstractConnection.h"

namespace Synchro
{

    class SynchroManager : public QObject
    {
        Q_OBJECT

    public:
        SynchroManager( AbstractConnection * connection );

        void commit( const QString & configFileBasket );
        void update( const QString & configFileBasket );

        QStringList baskets();

        void setLogin( const QString & login );

        const QString & connectionName();

    public slots:
        void errorConnection( const QString & errorString );

    protected:
        void connectionDialog( void (SynchroManager::*pt2Func)(const QString &), const QString & config );
        bool connectionExec( KPasswordDialog & dlg, KWallet::Wallet * wallet, void (SynchroManager::*pt2Func)(const QString &), const QString & config );

        void commitBasket( const QString & idBasket );
        void updateBasket( const QString & idBasket );
        void getBaskets( const QString & idBasket );

    private:
        AbstractConnection * m_cx;

        QStringList m_ids;

        QString m_login;

    };

}



#endif // SYNCHROMANAGER_H
