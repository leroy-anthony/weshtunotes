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

#include "NetworkAccessManagerProxy.h"

#include <QNetworkReply>
#include <QSslError>

#include <KDebug>

#include "../main/KWeshTuNotesApp.h"
#include "../main/MainWindow.h"

namespace Synchro
{

    NetworkAccessManagerProxy::NetworkAccessManagerProxy( QObject * parent ):
            QObject(parent),
            m_networkManager(parent)
    {
        connect( &m_networkManager, SIGNAL( finished( QNetworkReply * ) ), this, SLOT(replyFinished( QNetworkReply * ) ) );
        connect( &m_networkManager, SIGNAL( sslErrors( QNetworkReply *, const QList<QSslError> & ) ), this, SLOT(sslErrors(QNetworkReply *, const QList<QSslError> &) ) );
    }

    void NetworkAccessManagerProxy::replyFinished( QNetworkReply * reply )
    {
        if ( reply->isFinished() )
        {
            m_syncEvent.quit();
        }

        m_syncEvent.exit(1);
    }

    void NetworkAccessManagerProxy::sslErrors(QNetworkReply * r, const QList<QSslError> & ssls)
    {
        for ( int i=0 ; i<ssls.size() ; ++i )
        {
            kDebug() << ssls[i].errorString();
        }
    }

    void NetworkAccessManagerProxy::waitReponse( const QNetworkRequest & request )
    {
        MainWindow::showMessage( QString("Connecting %1...").arg(request.url().toString()), 0 );

        m_syncEvent.exec(QEventLoop::ExcludeUserInputEvents);

        MainWindow::showMessage( "", 0 );
    }

    QNetworkReply * NetworkAccessManagerProxy::get( const QNetworkRequest & request )
    {
        QNetworkReply * reply = m_networkManager.get( request );

        waitReponse( request );

        return reply;
    }

    QNetworkReply * NetworkAccessManagerProxy::put( const QNetworkRequest & request, const QByteArray & data )
    {
        QNetworkReply * reply = m_networkManager.put( request, data );

        waitReponse( request );

        return reply;
    }

    QNetworkReply * NetworkAccessManagerProxy::put( const QNetworkRequest & request, QIODevice * data )
    {
        QNetworkReply * reply = m_networkManager.put( request, data );

        waitReponse( request );

        return reply;
    }

    QNetworkReply * NetworkAccessManagerProxy::post( const QNetworkRequest & request, QIODevice * data )
    {
        QNetworkReply * reply = m_networkManager.post( request, data );

        waitReponse( request );

        return reply;
    }

    QNetworkReply * NetworkAccessManagerProxy::post( const QNetworkRequest & request, const QByteArray & data )
    {
        QNetworkReply * reply = m_networkManager.post( request, data );

        waitReponse( request );

        return reply;
    }

    QNetworkReply * NetworkAccessManagerProxy::deleteResource( const QNetworkRequest & request )
    {
        QNetworkReply * reply = m_networkManager.deleteResource( request );

        waitReponse( request );

        return reply;
    }

}
