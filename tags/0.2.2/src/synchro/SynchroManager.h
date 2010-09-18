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

#include <QDir>

#include <kpassworddialog.h>
#include <KWallet/Wallet>
#include <KCmdLineArgs>
#include <KAboutData>
#include <KApplication>

#include "../technic/GeneratorID.h"
#include "../synchro/AbstractConnection.h"
#include "../config/Configuration.h"
#include "../basket/ItemTreeBasket.h"
#include "../scene/AbstractScene.h"
#include "../data/DataManager.h"

namespace Synchro
{

    template <class T> class SynchroManager
    {
    public:
        SynchroManager();

        void commit( const QString & configFileBasket );
        void update( const QString & configFileBasket );

        QStringList baskets();

        void setLogin( const QString & login );

        const QString & connectionName();

    protected:
        void connectionDialog( void (SynchroManager::*pt2Func)(const QString &), const QString & config );
        bool connectionExec( KPasswordDialog & dlg, KWallet::Wallet * wallet, void (SynchroManager::*pt2Func)(const QString &), const QString & config );

        void commitBasket( const QString & idBasket );
        void updateBasket( const QString & idBasket );
        void getBaskets( const QString & idBasket );

    private:
        T m_cx;

        QStringList m_ids;

        QString m_login;

    };

    template <class T> SynchroManager<T>::SynchroManager()
    {
    }

    template <class T> const QString & SynchroManager<T>::connectionName()
    {
        return m_cx.connectionName();
    }

    template <class T> void SynchroManager<T>::setLogin( const QString & login )
    {
        m_login = login;
    }

    template <class T> void SynchroManager<T>::commit( const QString & configFileBasket )
    {
        connectionDialog( &SynchroManager::commitBasket, configFileBasket );
    }

    template <class T> void SynchroManager<T>::update( const QString & configFileBasket )
    {
        connectionDialog( &SynchroManager::updateBasket, configFileBasket );
    }

    template <class T> QStringList SynchroManager<T>::baskets()
    {
        connectionDialog( &SynchroManager::getBaskets, "" );

        return m_ids;
    }

    template <class T> void SynchroManager<T>::connectionDialog( void (SynchroManager::*actionPtr)(const QString &), const QString & config )
    {
        KPasswordDialog dlg(0, KPasswordDialog::ShowUsernameLine | KPasswordDialog::ShowKeepPassword );
        dlg.setPrompt(i18n("Enter a login and a password"));
        dlg.setKeepPassword(true);

        QString name = KCmdLineArgs::aboutData()->programName();
        KWallet::Wallet * wallet = KWallet::Wallet::openWallet(KWallet::Wallet::NetworkWallet(), 0, KWallet::Wallet::Synchronous);
        bool walletOk = wallet != 0 &&
                        (wallet->hasFolder(name) || wallet->createFolder(name)) &&
                        wallet->setFolder(name);

        if ( !walletOk )
        {
            wallet = 0;
        }

        if ( wallet != 0 )
        {
            if ( m_login == "" )
            {
                Config::Configuration conf;
                QStringList logins = conf.values(m_cx.connectionName(),"login");
                if ( logins.size() > 0 )
                {
                    m_login = logins[0];
                }
            }

            dlg.setUsername(m_login);

            QString password;
            wallet->readPassword(m_login,password);
            dlg.setPassword(password);

            if ( !m_cx.authentication(dlg.username(), dlg.password()) )
            {
                connectionExec( dlg, wallet, actionPtr, config );
                return;
            }

            (this->*actionPtr)( config );
            return;
        }

        connectionExec( dlg, wallet, actionPtr, config );
    }

    template <class T> bool SynchroManager<T>::connectionExec( KPasswordDialog & dlg, KWallet::Wallet * wallet, void (SynchroManager::*actionPtr)(const QString &), const QString & config )
    {
        while ( dlg.exec() && !m_cx.authentication(dlg.username(), dlg.password()) )
        {
            dlg.showErrorMessage( "Echec de connexion", KPasswordDialog::PasswordError );
        }

        if ( dlg.result() == QDialog::Accepted )
        {
            if ( dlg.keepPassword() && wallet != 0 )
            {
                Config::Configuration conf;
                conf.setValue(m_cx.connectionName(),"login",dlg.username());
                wallet->writePassword( dlg.username(), dlg.password());
            }

            (this->*actionPtr)( config );
        }
    }

    template <class T> void SynchroManager<T>::getBaskets( const QString & idBasket )
    {
        m_ids = m_cx.content("kweshtunotes");
    }

    template <class T> void SynchroManager<T>::commitBasket( const QString & idB )
    {
        QString idBasket = "_"+idB;

        QString idFolder = m_cx.findId( idBasket, true );
        if ( idFolder == "" )
        {
            idFolder = m_cx.createFolder( idBasket, true );
        }

        Data::DataManager b( Data::DataManager::configFileBasket( idB ) );
        m_cx.saveOrUpdateFile(b.fileName(),idFolder);

        QString id = b.values("scene","id").at(0);
        Data::DataManager assoc( Data::DataManager::configFileAssoc( id ) );
        m_cx.saveOrUpdateFile(assoc.fileName(),idFolder);

        QStringList handles = assoc.values("general","items");
        for ( int i=0 ; i<handles.size() ; ++i )
        {
            Data::DataManager handle(handles[i]);
            m_cx.saveOrUpdateFile(handle.fileName(),idFolder);

            QStringList subHandles = handle.values("general","items");
            for ( int j=0 ; j<subHandles.size() ; ++j )
            {
                QStringList items = handle.values(subHandles[j],"data");
                for ( int k=0 ; k<items.size() ; ++k)
                {
                    QString pathItem = Data::DataManager::itemsStorePath()+items[k]+".html";
                    m_cx.saveOrUpdateFile(pathItem,idFolder);

                    Data::DataManager data( Data::DataManager::configFileItem(items[k]) );
                    m_cx.saveOrUpdateFile( data.fileName(), idFolder );

                    QStringList images = data.values( "data", "images" );
                    for ( int l=0 ; l<images.size() ; ++l)
                    {
                        QFile f(Data::DataManager::datasStorePath()+images[l]);
                        if ( f.open( QFile::ReadOnly ) )
                        {
                            QByteArray data = f.readAll().toBase64();

                            QFile fileTmp( Data::DataManager::basketsStorePath()+images[l] );
                            fileTmp.open(QFile::WriteOnly);
                            QTextStream stream(&fileTmp);
                            stream << data;
                            fileTmp.close();

                            m_cx.saveOrUpdateFile( fileTmp.fileName(), idFolder );

                            fileTmp.remove();
                        }
                    }
                }
            }
        }
    }

    template <class T> void SynchroManager<T>::updateBasket( const QString & idBasket )
    {
        m_ids = m_cx.content("kweshtunotes");

        QStringList ids;
        for ( int i=0 ; i<m_ids.size() ; ++i )
        {
            ids += m_ids[i].split(":")[0];
        }

        if ( !ids.contains(idBasket) )
        {
            return;
        }

        Data::DataManager b( Data::DataManager::configFileBasket( idBasket ) );
        b.setContent( m_cx.file(idBasket,"txt") );
        QString id = b.values("scene","id").at(0);

        Data::DataManager assoc( Data::DataManager::configFileAssoc( id ) );
        assoc.setContent(m_cx.file(QFileInfo(assoc.fileName()).fileName(),"txt"));

        QStringList handles = assoc.values("general","items");
        for ( int i=0 ; i<handles.size() ; ++i )
        {
            Data::DataManager handle(handles[i]);
            handle.setContent(m_cx.file(QFileInfo(handle.fileName()).fileName(),"txt"));

            QStringList subHandles = handle.values("general","items");
            for ( int j=0 ; j<subHandles.size() ; ++j )
            {
                QStringList items = handle.values(subHandles[j],"data");
                for ( int k=0 ; k<items.size() ; ++k)
                {
                    QString pathItem = Settings::basketsStorePath().toLocalFile()+QDir::separator()+"items"+QDir::separator()+items[k]+".html";
                    QFile file3(pathItem);
                    if ( file3.open(QIODevice::WriteOnly | QIODevice::Text) )
                    {
                        file3.write( m_cx.file(QFileInfo(pathItem).fileName(),"txt") );
                        file3.close();
                    }

                    QString pathDataItem = Settings::basketsStorePath().toLocalFile()+QDir::separator()+"items"+QDir::separator()+items[k];
                    QFile file4(pathDataItem);
                    if ( file4.open(QIODevice::WriteOnly) )
                    {
                        QByteArray data = m_cx.file(QFileInfo(pathDataItem).fileName(),"txt");
                        if ( data != "" )
                        {
                            file4.write(data);
                            file4.close();

                            Data::DataManager data( Data::DataManager::configFileItem(items[k]) );
                            QStringList images = data.values( "data", "images" );
                            for ( int l=0 ; l<images.size() ; ++l)
                            {
                                QFile fData(Data::DataManager::datasStorePath()+images[l]);
                                if ( fData.open(QIODevice::WriteOnly) )
                                {
                                    fData.write( QByteArray::fromBase64( m_cx.file(images[l],"txt") ) );
                                    fData.close();
                                }
                            }
                        }
                        else
                        {
                            file4.close();
                        }
                    }
                }
            }
        }
    }

}



#endif // SYNCHROMANAGER_H
