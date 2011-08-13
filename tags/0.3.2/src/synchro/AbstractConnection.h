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

#include "../technic/GeneratorID.h"
#include "../synchro/NetworkAccessManagerProxy.h"

namespace Synchro
{
    class GoogleDocsConnection;

    class AbstractConnection : public QObject
    {
        Q_OBJECT

    public:
        AbstractConnection( const QString & connectionName );

        virtual bool authentication( const QString & username, const QString & password ) = 0;

        virtual void saveOrUpdateFile( const QString & fileName, const QString & folder = QString("") ) = 0;
        virtual QByteArray file( const QString & fileName, const QString & format ) = 0;
        virtual QString findId( const QString & ressourceName, bool folder = false ) = 0;
        virtual QString createFolder( const QString & folder, bool subDir ) = 0;
        virtual QStringList content( const QString & folder ) = 0;

        const QString & connectionName();

        const QString & connectionError();

    signals:
        void error( const QString & errorString );
        void info( const QString & infoString );

    private slots:
        void errorConnection(const QString & errorString );
        void infoConnection(const QString & infoString );

    protected:
        NetworkAccessManagerProxy m_networkAccessManager;

    private:
        QString m_connectionName;

    };
}

#endif // ABSTRACTCONNECTION_H
