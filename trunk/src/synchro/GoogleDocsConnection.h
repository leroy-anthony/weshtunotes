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

#ifndef GOOGLEDOCSCONNECTION_H
#define GOOGLEDOCSCONNECTION_H

#include <QString>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QObject>
#include <QHttp>
#include <QSslError>

#include "../synchro/AbstractConnection.h"

class QNetworkAccessManager;

namespace Synchro
{

    class GoogleDocsConnection : public QObject, public AbstractConnection
    {
        Q_OBJECT

    public:
        GoogleDocsConnection();
        bool authentication( const QString & username, const QString & password );

        void saveOrUpdateFile( const QString & fileName, const QString & folder = QString("") );
        QByteArray file( const QString & fileName, const QString & format );


    public slots:
        void replyFinished( QNetworkReply * );
        void sslErrors(QNetworkReply *, const QList<QSslError> &);


    private:
        void setAuthentication( QNetworkRequest & request );

        void putFile( const QString & fileName, const QString & folder = QString("") );
        QString findId( const QString & ressourceName, bool folder = false );


        QNetworkAccessManager * m_networkAccessManager;

        QEventLoop m_syncEvent;

        QString m_username;
        QString m_password;
        QString m_address;
        QString m_host;

        QString m_googleStringAuth;
    };

}

#endif // GOOGLEDOCSCONNECTION_H
