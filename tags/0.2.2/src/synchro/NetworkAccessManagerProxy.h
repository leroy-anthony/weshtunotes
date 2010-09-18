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

#ifndef NETWORKACCESSMANAGERPROXY_H
#define NETWORKACCESSMANAGERPROXY_H

#include <QEventLoop>

#include <KIO/AccessManager>

class QNetworkReply;

namespace Synchro
{

    class NetworkAccessManagerProxy : public QObject
    {
        Q_OBJECT

    public:
        NetworkAccessManagerProxy(QObject *parent = 0);

        QNetworkReply *	deleteResource( const QNetworkRequest & request );
        QNetworkReply * get(  const QNetworkRequest & );
        QNetworkReply *	post( const QNetworkRequest & request, QIODevice * data );
        QNetworkReply *	post( const QNetworkRequest & request, const QByteArray & data );
        QNetworkReply *	put(  const QNetworkRequest & request, QIODevice * data );
        QNetworkReply *	put(  const QNetworkRequest & request, const QByteArray & data );

    public slots:
        void replyFinished( QNetworkReply * );
        void sslErrors( QNetworkReply *, const QList<QSslError> &);

    private:
        void waitReponse( const QNetworkRequest & request );

        KIO::AccessManager m_networkManager;
        QEventLoop m_syncEvent;

    };

}

#endif // NETWORKACCESSMANAGERPROXY_H
