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

#include "DigitalBucketConnection.h"

#include <QNetworkReply>
#include <QDomDocument>
#include <QFile>

#include <kdebug.h>

namespace Synchro
{

    DigitalBucketConnection::DigitalBucketConnection ( const QString & username, const QString & password ) :
            m_username ( username ),
            m_password ( password ),
            m_address ( "https://www.digitalbucket.net/api/rest/%1.axd" )
    {
        m_networkAccessManager = new QNetworkAccessManager ( 0 );
        connect( m_networkAccessManager, SIGNAL( finished( QNetworkReply * ) ), &m_syncEvent, SLOT(quit() ) );
    }

    void DigitalBucketConnection::replyFinished ( QNetworkReply * reply )
    {
        if ( !reply->error() )
        {
	    QDomDocument doc;
	    doc.setContent(reply->readAll());
	    
	    QDomElement docElem = doc.documentElement();
	    QDomNode n = docElem.firstChild();
        }
        else
	{
	  kDebug() << "error" << reply->error();
	}

        reply->deleteLater();
    }

    void DigitalBucketConnection::setAuthentication ( QNetworkRequest & request )
    {
        request.setRawHeader ( "User-Authentication", QString ( m_username+":"+m_password ).toAscii().toBase64() );
    }

    void DigitalBucketConnection::rootFolder()
    {
        QNetworkRequest request ( m_address.arg ( "getrootfolder" ) );
        setAuthentication ( request );

        m_networkAccessManager->get ( request );
    }
    
    void DigitalBucketConnection::putFile( const QString & fileName )
    {
        QNetworkRequest request ( m_address.arg ( "putfile" ) + "?folderid=219272&filename=vv" );
        setAuthentication ( request );

	QFile * f = new QFile(fileName);
	if ( f->open( QFile::ReadOnly ) )
	{
	  m_networkAccessManager->put ( request, f );
	}
    }
    
    QByteArray DigitalBucketConnection::file( const QString & uid )
    {
	QNetworkRequest request ( m_address.arg ( "getfile" ) + "?fileid=" + uid );
	setAuthentication ( request );

	QNetworkReply * reply = m_networkAccessManager->get ( request );
	m_syncEvent.exec(QEventLoop::ExcludeUserInputEvents);

	if ( reply->isFinished() )
	{
	    return reply->readAll();
	}
    }
    
    QByteArray DigitalBucketConnection::fileByTag( const QString & tag )
    {
	QNetworkRequest request ( m_address.arg ( "getfilesbytag" ) + "?tag=" + tag );
	setAuthentication ( request );

	QNetworkReply * reply = m_networkAccessManager->get ( request );
	m_syncEvent.exec(QEventLoop::ExcludeUserInputEvents);

	if ( reply->isFinished() )
	{
	    QDomDocument doc;
	    doc.setContent(reply->readAll());
	    
	    QDomElement docElem = doc.firstChildElement("Files").firstChildElement("File").firstChildElement("FileID");
	    
	    return file( docElem.text() );
	}
	
	return "";
    }

}
