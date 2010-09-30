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

        connect( &m_timer, SIGNAL(timeout()), this, SLOT(replyTimeOut()) );
    }

    int NetworkAccessManagerProxy::error()
    {
        return m_codeError;
    }

    void NetworkAccessManagerProxy::buildDetailError( int codeError )
    {
        QString detailError = "";

        switch(codeError)
        {
        case QNetworkReply::NoError:
            detailError = "No error condition. Note: When the HTTP protocol returns a redirect no error will be reported. You can check if there is a redirect with the QNetworkRequest::RedirectionTargetAttribute attribute.";
            break;
        case QNetworkReply::ConnectionRefusedError:
            detailError = "The remote server refused the connection (the server is not accepting requests)";
            break;
        case QNetworkReply::RemoteHostClosedError:
            detailError = "The remote server closed the connection prematurely, before the entire reply was received and processed";
            break;
        case QNetworkReply::HostNotFoundError:
            detailError = "The remote host name was not found (invalid hostname)";
            break;
        case QNetworkReply::TimeoutError:
            detailError = "The connection to the remote server timed out";
            break;
        case QNetworkReply::OperationCanceledError:
            detailError = "The operation was canceled via calls to abort() or close() before it was finished.";
            break;
        case QNetworkReply::SslHandshakeFailedError:
            detailError = "The SSL/TLS handshake failed and the encrypted channel could not be established. The sslErrors() signal should have been emitted.";
            break;
        case QNetworkReply::ProxyConnectionRefusedError:
            detailError = "The connection to the proxy server was refused (the proxy server is not accepting requests)";
            break;
        case QNetworkReply::ProxyConnectionClosedError:
            detailError = "The proxy server closed the connection prematurely, before the entire reply was received and processed";
            break;
        case QNetworkReply::ProxyNotFoundError:
            detailError = "The proxy host name was not found (invalid proxy hostname)";
            break;
        case QNetworkReply::ProxyTimeoutError:
            detailError = "The connection to the proxy timed out or the proxy did not reply in time to the request sent";
            break;
        case QNetworkReply::ProxyAuthenticationRequiredError:
            detailError = "The proxy requires authentication in order to honour the request but did not accept any credentials offered (if any)";
            break;
        case QNetworkReply::ContentAccessDenied:
            detailError = "The access to the remote content was denied (similar to HTTP error 401)";
            break;
        case QNetworkReply::ContentOperationNotPermittedError:
            detailError = "The operation requested on the remote content is not permitted";
            break;
        case QNetworkReply::ContentNotFoundError:
            detailError = "The remote content was not found at the server (similar to HTTP error 404)";
            break;
        case QNetworkReply::AuthenticationRequiredError:
            detailError = "The remote server requires authentication to serve the content but the credentials provided were not accepted (if any)";
            break;
        case QNetworkReply::ContentReSendError:
            detailError = "The request needed to be sent again, but this failed for example because the upload data could not be read a second time.";
            break;
        case QNetworkReply::ProtocolUnknownError:
            detailError = "The Network Access API cannot honor the request because the protocol is not known";
            break;
        case QNetworkReply::ProtocolInvalidOperationError:
            detailError = "The requested operation is invalid for this protocol";
            break;
        case QNetworkReply::UnknownNetworkError:
            detailError = "An unknown network-related error was detected";
            break;
        case QNetworkReply::UnknownProxyError:
            detailError = "An unknown proxy-related error was detected";
            break;
        case QNetworkReply::UnknownContentError:
            detailError = "An unknown error related to the remote content was detected";
            break;
        case QNetworkReply::ProtocolFailure:
            detailError = "A breakdown in protocol was detected (parsing error, invalid or unexpected responses, etc.";
            break;
        default:
            detailError = "An unknown error";
        }

        m_detailError = detailError + " : " + m_detailError+".";
    }

    const QString & NetworkAccessManagerProxy::detailError()
    {
        return m_detailError;
    }

    void NetworkAccessManagerProxy::replyTimeOut()
    {
        kDebug() << "TimeOut network.";

        m_codeError = 4;
        m_detailError = "The connection to the remote server timed out.";
        m_syncEvent.exit(1);

        emit error(m_detailError);
    }

    void NetworkAccessManagerProxy::replyFinished( QNetworkReply * reply )
    {
        m_timer.stop();

        if ( reply->error() == QNetworkReply::NoError )
        {
            m_codeError = QNetworkReply::NoError;
            m_detailError = "";
            m_syncEvent.quit();
        }
        else
        {
            m_codeError = reply->error();
            m_detailError = reply->errorString();
            buildDetailError( m_codeError );
            kDebug() << m_codeError;

            m_syncEvent.exit(1);
            emit error(m_detailError);
        }
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

        m_timer.start(10000);
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
