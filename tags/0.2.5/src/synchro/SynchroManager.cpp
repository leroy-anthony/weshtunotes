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

#include "settings.h"
#include "SynchroManager.h"

#include <QDir>
#include <QLabel>

#include <kpassworddialog.h>
#include <KCmdLineArgs>
#include <KAboutData>
#include <KApplication>
#include <KMessageBox>
#include <QTextListFormat>

#include "../config/Configuration.h"
#include "../basket/ItemTreeBasket.h"
#include "../scene/AbstractScene.h"
#include "../data/DataManager.h"
#include "../main/MainWindow.h"

namespace Synchro
{

    SynchroManager::SynchroManager( AbstractConnection * connection ):
            QObject(0),
            m_cx(connection),
            m_dialog(0)
    {
        connect( m_cx, SIGNAL(error(const QString &)), this, SLOT(errorConnection(const QString &)) );
        connect( m_cx, SIGNAL(info(const QString &)), this, SLOT(infoConnection(const QString &)) );
    }

    SynchroManager::~SynchroManager()
    {
    }

    const QString & SynchroManager::connectionName()
    {
        return m_cx->connectionName();
    }

    void SynchroManager::setLogin( const QString & login )
    {
        m_login = login;
    }

    void SynchroManager::errorConnection( const QString & errorString )
    {
        if ( m_dialog != 0 )
        {
            m_dialog->enableButton(KDialog::Details,true);
            QTextCharFormat f;
            f.setForeground(QColor("red"));
            m_log.textCursor().insertText(" [ERROR] "+errorString+"\n",f);
        }
    }

    void SynchroManager::infoConnection( const QString & infoString )
    {
        if ( m_dialog != 0 )
        {
            m_dialog->enableButton(KDialog::Details,true);
            QTextCharFormat f;
            f.setForeground(QColor("black"));
            m_log.textCursor().insertText(" [INFO] "+infoString+"\n",f);
        }
    }

    void SynchroManager::commit( const QString & configFileBasket )
    {
        delete m_dialog;
        m_dialog = new KProgressDialog;
        m_dialog->setButtons( KDialog::Details | KDialog::Ok );
        m_dialog->setDetailsWidget( &m_log );
        m_dialog->setModal(true);
        m_dialog->enableButtonOk(false);
        m_dialog->setInitialSize(QSize(400,100));
        m_dialog->enableButton(KDialog::Details,false);
        m_dialog->setVisible(false);

        m_dialog->show();
        m_dialog->setLabelText(i18n("Commit basket..."));

        connectionDialog( &SynchroManager::commitBasket, configFileBasket );

        m_dialog->enableButtonOk(true);
        m_dialog->exec();
    }

    void SynchroManager::update( const QString & configFileBasket )
    {
        m_dialog = new KProgressDialog;
        m_dialog->setButtons( KDialog::Details | KDialog::Ok );
        m_dialog->setDetailsWidget( &m_log );
        m_dialog->setModal(true);
        m_dialog->enableButtonOk(false);
        m_dialog->setInitialSize(QSize(400,100));
        m_dialog->enableButton(KDialog::Details,false);
        m_dialog->setVisible(false);

        m_dialog->show();
        m_dialog->setLabelText(i18n("Update basket..."));
        m_dialog->progressBar()->setRange(0,0);

        connectionDialog( &SynchroManager::updateBasket, configFileBasket );

        m_dialog->progressBar()->setRange(0,1);
        m_dialog->progressBar()->setValue(1);
        m_dialog->enableButtonOk(true);
        m_dialog->exec();

        delete m_dialog;
        m_dialog = 0;
    }

    QStringList SynchroManager::baskets()
    {
        connectionDialog( &SynchroManager::getBaskets, "" );

        return m_ids;
    }

    void SynchroManager::connectionDialog( void (SynchroManager::*actionPtr)(const QString &), const QString & config )
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
                QStringList logins = conf.values(m_cx->connectionName(),"login");
                if ( logins.size() > 0 )
                {
                    m_login = logins[0];
                }
            }

            dlg.setUsername(m_login);

            QString password;
            wallet->readPassword(m_login,password);
            dlg.setPassword(password);

            if ( !m_cx->authentication(dlg.username(), dlg.password()) )
            {
                if ( m_cx->connectionError() == "" )
                {
                    connectionExec( dlg, wallet, actionPtr, config );
                    return;
                }

                return;
            }

            (this->*actionPtr)( config );
        }
        else
        {
            connectionExec( dlg, wallet, actionPtr, config );
        }
    }

    bool SynchroManager::connectionExec( KPasswordDialog & dlg, KWallet::Wallet * wallet, void (SynchroManager::*actionPtr)(const QString &), const QString & config )
    {
        while ( dlg.exec() &&
                !m_cx->authentication(dlg.username(), dlg.password()) &&
                (m_cx->connectionError() == "") )
        {
            dlg.showErrorMessage( "Fail authentication", KPasswordDialog::PasswordError );
        }

        if ( dlg.result() == QDialog::Accepted )
        {
            if ( dlg.keepPassword() && wallet != 0 )
            {
                Config::Configuration conf;
                conf.setValue(m_cx->connectionName(),"login",dlg.username());
                wallet->writePassword( dlg.username(), dlg.password());
            }

            (this->*actionPtr)( config );
        }

        return false;
    }

    void SynchroManager::getBaskets( const QString & idBasket )
    {
        Q_UNUSED(idBasket);

        m_ids = m_cx->content("kweshtunotes");
    }

    void SynchroManager::commitBasket( const QString & idB )
    {        
        Data::DataManager b( Data::DataManager::configFileBasket( idB ) );

        QString id = b.values("scene","id").at(0);
        Data::DataManager assoc( Data::DataManager::configFileAssoc( id ) );


        int nbStep = 3;
        QStringList handles = assoc.values("general","items");
        for ( int i=0 ; i<handles.size() ; ++i )
        {
            nbStep++;
            Data::DataManager handle(handles[i]);
            QStringList subHandles = handle.values("general","items");
            for ( int j=0 ; j<subHandles.size() ; ++j )
            {
                QStringList items = handle.values(subHandles[j],"data");
                for ( int k=0 ; k<items.size() ; ++k)
                {
                    nbStep+=2;
                    Data::DataManager data( Data::DataManager::configFileItem(items[k]) );
                    QStringList images = data.values( "data", "images" );
                    for ( int l=0 ; l<images.size() ; ++l)
                    {
                        nbStep++;
                    }
                }
            }
        }

        m_dialog->progressBar()->setRange(0,nbStep);
        nbStep = 0;

        QString idBasket = "_"+idB;
        QString idFolder = m_cx->findId( idBasket, true );
        if ( idFolder == "" )
        {
            idFolder = m_cx->createFolder( idBasket, true );
        }
        m_dialog->progressBar()->setValue( ++nbStep );

        m_cx->saveOrUpdateFile(b.fileName(),idFolder);
        m_dialog->progressBar()->setValue( ++nbStep );
        m_cx->saveOrUpdateFile(assoc.fileName(),idFolder);
        m_dialog->progressBar()->setValue( ++nbStep );


        //QStringList handles = assoc.values("general","items");
        for ( int i=0 ; i<handles.size() ; ++i )
        {
            Data::DataManager handle(handles[i]);
            m_cx->saveOrUpdateFile(handle.fileName(),idFolder);

            m_dialog->progressBar()->setValue( ++nbStep );

            QStringList subHandles = handle.values("general","items");
            for ( int j=0 ; j<subHandles.size() ; ++j )
            {
                QStringList items = handle.values(subHandles[j],"data");
                for ( int k=0 ; k<items.size() ; ++k)
                {
                    QString pathItem = Data::DataManager::itemsStorePath()+items[k]+".html";
                    m_cx->saveOrUpdateFile(pathItem,idFolder);

                    m_dialog->progressBar()->setValue( ++nbStep );

                    Data::DataManager data( Data::DataManager::configFileItem(items[k]) );
                    m_cx->saveOrUpdateFile( data.fileName(), idFolder );

                    m_dialog->progressBar()->setValue( ++nbStep );

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

                            m_cx->saveOrUpdateFile( fileTmp.fileName(), idFolder );

                            fileTmp.remove();

                            m_dialog->progressBar()->setValue( ++nbStep );

                        }
                    }
                }
            }
        }
    }

    void SynchroManager::updateBasket( const QString & idBasket )
    {
        m_ids = m_cx->content("kweshtunotes");

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
        b.setContent( m_cx->file(idBasket,"txt") );
        QString id = b.values("scene","id").at(0);

        Data::DataManager assoc( Data::DataManager::configFileAssoc( id ) );
        assoc.setContent(m_cx->file(QFileInfo(assoc.fileName()).fileName(),"txt"));

        QStringList handles = assoc.values("general","items");
        for ( int i=0 ; i<handles.size() ; ++i )
        {
            Data::DataManager handle(handles[i]);
            handle.setContent(m_cx->file(QFileInfo(handle.fileName()).fileName(),"txt"));

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
                        file3.write( m_cx->file(QFileInfo(pathItem).fileName(),"txt") );
                        file3.close();
                    }

                    QString pathDataItem = Settings::basketsStorePath().toLocalFile()+QDir::separator()+"items"+QDir::separator()+items[k];
                    QFile file4(pathDataItem);
                    if ( file4.open(QIODevice::WriteOnly) )
                    {
                        QByteArray data = m_cx->file(QFileInfo(pathDataItem).fileName(),"txt");
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
                                    fData.write( QByteArray::fromBase64( m_cx->file(images[l],"txt") ) );
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
