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


#include "GoogleDocsConnection.h"

#include <QNetworkReply>
#include <QDomDocument>
#include <QFile>
#include <QStringList>
#include <QSslConfiguration>
#include <QSslError>
#include <QHttp>
#include <QThread>
#include <QFileInfo>

#include <kdebug.h>

#include "../data/DataManager.h"
#include "NetworkAccessManagerProxy.h"

namespace Synchro
{

    GoogleDocsConnection::GoogleDocsConnection () :
            AbstractConnection("Google Document"),
            m_address( "/feeds/default/private/full/" ),
            m_host ( "docs.google.com" )
    {
    }

    bool GoogleDocsConnection::authentication( const QString & username, const QString & password )
    {
        QString urlConnection("https://www.google.com/accounts/ClientLogin");

        QByteArray content;
        content += "&accountType=HOSTED_OR_GOOGLE";
        content += "&Email="+username.toAscii();
        content += "&Passwd="+password.toAscii();
        content += "&service=writely";
        content += "&source=kwesh";

        QNetworkRequest request ( urlConnection );
        request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

        QNetworkReply * reply = m_networkAccessManager.post( request, content );

        QString reponse = reply->readAll();

        QStringList tokens = reponse.split("Auth=");
        if ( tokens.size() == 2 )
        {
            m_googleStringAuth = "GoogleLogin auth="+tokens[1];
            m_googleStringAuth.chop(1); // delete new line character

            return true;
        }

        return false;
    }

    void GoogleDocsConnection::setAuthentication ( QNetworkRequest & request )
    {
        request.setRawHeader ( "Host", m_host.toAscii() );
        request.setRawHeader ( "GData-Version", "3.0" );
        request.setRawHeader ( "Authorization", m_googleStringAuth.toAscii() );
    }

    QString GoogleDocsConnection::createFolder( const QString & folder, bool subDir )
    {
        QString url = "https://docs.google.com/feeds/default/private/full/folder%3A%1/contents";

        if ( !subDir )
        {
            url = "https://docs.google.com/feeds/default/private/full";
        }
        else
        {
            QString idParentDir = findId( "kweshtunotes", true );
            if ( idParentDir == "" )
            {
                idParentDir = createFolder( "kweshtunotes", false );
            }

            url = url.arg(idParentDir);
        }

        QByteArray content;
        content += "<?xml version='1.0' encoding='UTF-8'?>\r\n";
        content += "<entry xmlns=\"http://www.w3.org/2005/Atom\">\r\n";
        content += "<category scheme=\"http://schemas.google.com/g/2005#kind\" term=\"http://schemas.google.com/docs/2007#folder\"/>\r\n";
        content += "<title>";
        content += folder.toAscii();
        content += "</title>\r\n";
        content += "</entry>\r\n";

        QNetworkRequest r( url );

        setAuthentication( r );

        r.setRawHeader( "Content-Type", "application/atom+xml" );
        r.setRawHeader( "Content-Length", QString::number(content.size()).toAscii() );

        QNetworkReply * reply = m_networkAccessManager.post( r, content );

        QDomDocument result("result");
        result.setContent(reply->readAll());

        QDomNodeList nodes = result.elementsByTagName("gd:resourceId");
        if ( nodes.size() > 0 )
        {
            return nodes.at(0).toElement().text().remove("folder:");
        }

        return "";
    }

    void GoogleDocsConnection::putFile( const QString & fileName, const QString & idFolder )
    {
        QFile * f = new QFile(fileName);
        if ( f->open( QFile::ReadOnly ) )
        {            
            QFileInfo fileInfo(*f);

            QString boundary = "END_OF_PART";

            QString url = "https://docs.google.com/feeds/default/private/full/";

            if ( idFolder != QString("") )
            {
                url.append("folder%3A");
                url.append(idFolder);
                url.append("/contents");
            }

            QNetworkRequest r(url);

            QByteArray datas(QString("--" + boundary + "\r\n").toAscii());
            datas += "Content-Type: application/atom+xml\r\n\r\n";
            datas += "<?xml version='1.0' encoding='UTF-8'?>\r\n";
            datas += "<entry xmlns=\"http://www.w3.org/2005/Atom\" xmlns:docs=\"http://schemas.google.com/docs/2007\">\r\n";
            datas += "<category scheme=\"http://schemas.google.com/g/2005#kind\" term=\"http://schemas.google.com/docs/2007#document\"/>\r\n";
            datas += "<title>"+QUrl(fileInfo.fileName()).toEncoded()+"</title>\r\n";
            datas += "<docs:writersCanInvite value=\"false\" />\r\n";
            datas += "</entry>\r\n\r\n";
            datas += QString("--" + boundary + "\r\n").toAscii();
            datas += "Content-Type: text/plain\r\n\r\n";

            QString data = f->readAll();
            datas += data;

            datas += "\r\n\r\n";
            datas += QString("--" + boundary + "--").toAscii();

            setAuthentication( r );
            r.setRawHeader( "Content-Type", QString("multipart/related; boundary=" + boundary).toAscii());
            r.setRawHeader( "Content-Length", QString::number(datas.length()).toAscii());
            r.setRawHeader( "Slug", "test" );

            m_networkAccessManager.post(r,datas);
        }
    }

    void GoogleDocsConnection::saveOrUpdateFile( const QString & fileName, const QString & idFolder )
    {
        QFile * f = new QFile(fileName);
        if ( f->open( QFile::ReadOnly ) )
        {
            QFileInfo fileInfo(*f);
            QString url = "https://docs.google.com/feeds/default/media/document%3A";

            QString id = findId( fileInfo.fileName() );

            if ( id != "" )
            {
                QNetworkRequest r( url + id );

                setAuthentication( r );

                r.setRawHeader( "If-Match", "*" );
                r.setRawHeader( "Content-Type", "text/plain" );
                r.setRawHeader( "Content-Length", QString::number(f->size()).toAscii() );
                r.setRawHeader( "Slug", "test" );

                m_networkAccessManager.put(r,f);
            }
            else
            {
                putFile( fileName, idFolder );
            }
        }
    }

    QStringList GoogleDocsConnection::content( const QString & folder )
    {
        QString idFolder = findId( folder, true );
        if ( idFolder == "" )
        {
            return QStringList();
        }

        QString url = QString("https://docs.google.com/feeds/default/private/full/folder%3A%1/contents?showfolders=true").arg(idFolder);

        QNetworkRequest request ( url );

        setAuthentication ( request );

        QNetworkReply * reply = m_networkAccessManager.get ( request );

        QDomDocument result("result");
        result.setContent(reply->readAll());

        QDomNodeList nodes = result.elementsByTagName("title");

        QStringList ids;
        for ( int i=1 ; i<nodes.size() ; ++i )
        {
            QString basketName = nodes.at(i).toElement().text();
            QByteArray content = file( basketName.remove(0,1), "txt" );
            if ( content != "" )
            {
                Data::DataManager d("tmp");
                d.setContent(content);

                ids += ( basketName+":"+ d.values("basket","name")[0] );
            }
        }

        return ids;
    }

    QString GoogleDocsConnection::findId( const QString & ressourceName, bool folder )
    {
        QString url = QString("https://docs.google.com/feeds/default/private/full/?title=%1&title-exact=true").arg(ressourceName);

        if ( folder )
        {
            url.append("&showfolders=true");
        }

        QNetworkRequest request ( url );
        setAuthentication ( request );

        QNetworkReply * reply = m_networkAccessManager.get( request );

        QDomDocument result("result");
        result.setContent(reply->readAll());

        QDomNodeList nodes = result.elementsByTagName("gd:resourceId");
        if ( nodes.size() > 0 )
        {
            if ( folder )
            {
                return nodes.at(0).toElement().text().remove("folder:");
            }
            else
            {
                return nodes.at(0).toElement().text().remove("document:");
            }
        }

        return "";
    }

    QByteArray GoogleDocsConnection::file( const QString & fileName, const QString & format )
    {        
        QString url = QString("https://docs.google.com/feeds/default/private/full/?title=%1&title-exact=true").arg(fileName);

        QNetworkRequest request ( url );

        setAuthentication ( request );

        QNetworkReply * reply = m_networkAccessManager.get ( request );

        QDomDocument result("result");
        result.setContent(reply->readAll());

        QDomNodeList nodes = result.elementsByTagName("content");
        if ( nodes.size() > 0 )
        {
            QString urlEXport = nodes.at(0).toElement().attribute("src");

            QNetworkRequest request ( urlEXport+"&format="+format );

            setAuthentication ( request );

            reply = m_networkAccessManager.get ( request );

            QString content = reply->readAll();

            if ( format == "txt" )
            {
                content.remove(0,3);
            }

            return content.toLatin1();
        }

        return "";
    }
}
