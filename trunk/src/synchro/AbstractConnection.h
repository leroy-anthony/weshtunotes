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

#ifndef ABSTRACTCONNECTION_H
#define ABSTRACTCONNECTION_H

#include <kpassworddialog.h>
#include <KWallet/Wallet>

#include "../technic/GeneratorID.h"

namespace Synchro
{
    class GoogleDocsConnection;

    class AbstractConnection
    {
    public:
        AbstractConnection( const QString & connectionName );

        virtual bool authentication( const QString & username, const QString & password ) = 0;

        virtual void saveOrUpdateFile( const QString & fileName, const QString & folder = QString("") ) = 0;
        virtual QByteArray file( const QString & fileName, const QString & format ) = 0;

        const QString & connectionName();

    private:
        QString m_connectionName;

    };
}

#endif // ABSTRACTCONNECTION_H
