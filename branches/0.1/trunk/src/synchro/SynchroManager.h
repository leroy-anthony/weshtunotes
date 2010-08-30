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

    protected:
        void connectionDialog( void (SynchroManager::*pt2Func)(const QString &), const QString & config );
        bool connectionExec( KPasswordDialog & dlg, KWallet::Wallet * wallet, void (SynchroManager::*pt2Func)(const QString &), const QString & config );

        void commitBasket(   const QString & configFileBasket );
        void updateBasket( const QString & configFileBasket );

    private:
        T m_cx;
    };

    template <class T> SynchroManager<T>::SynchroManager()
    {
    }

    template <class T> void SynchroManager<T>::commit( const QString & configFileBasket )
    {
        connectionDialog( &SynchroManager::commitBasket, configFileBasket );
    }

    template <class T> void SynchroManager<T>::update( const QString & configFileBasket )
    {
        connectionDialog( &SynchroManager::updateBasket, configFileBasket );
    }

    template <class T> void SynchroManager<T>::connectionDialog( void (SynchroManager::*actionPtr)(const QString &), const QString & config )
    {
        KPasswordDialog dlg(0, KPasswordDialog::ShowUsernameLine | KPasswordDialog::ShowKeepPassword );
        dlg.setPrompt("Enter a login and a password");
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

        Config::Configuration conf;
        QStringList logins = conf.values("GoogleAccount","login");

        T cx;

        if ( logins.size() > 0 && wallet != 0 )
        {
            dlg.setUsername(logins[0]);

            QString password;
            wallet->readPassword(logins[0],password);
            dlg.setPassword(password);

            if ( !cx.authentication(dlg.username(), dlg.password()) )
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
                conf.setValue("GoogleAccount","login",dlg.username());
                wallet->writePassword( dlg.username(), dlg.password());
            }

            (this->*actionPtr)( config );
        }
    }

    template <class T> void SynchroManager<T>::commitBasket( const QString & configFileBasket )
    {
        Data::DataManager b(configFileBasket);
        //cx->putFile(b.fileName());

        QString id = b.values("scene","id").at(0);
        Data::DataManager assoc( QString("association") + QDir::separator()  + id  );
        m_cx.saveOrUpdateFile(assoc.fileName(),"association");

        QStringList handles = assoc.values("general","items");
        for ( int i=0 ; i<handles.size() ; ++i )
        {
            Data::DataManager handle(handles[i]);
            m_cx.saveOrUpdateFile(handle.fileName(),"items");

            QStringList subHandles = handle.values("general","items");
            for ( int j=0 ; j<subHandles.size() ; ++j )
            {
                QStringList items = handle.values(subHandles[j],"data");
                for ( int k=0 ; k<items.size() ; ++k)
                {
                    QString pathItem = Settings::basketsStorePath().toLocalFile()+QDir::separator()+"items"+QDir::separator()+items[k]+".html";
                    m_cx.saveOrUpdateFile(pathItem,"items");
                }
            }
        }
    }

    template <class T>void SynchroManager<T>::updateBasket( const QString & configFileBasket )
    {
        Data::DataManager b(configFileBasket);
        //cx->putFile(b.fileName());

        QString id = b.values("scene","id").at(0);
        Data::DataManager assoc( QString("association") + QDir::separator()  + id  );

        QFile file(assoc.fileName());
        if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
        {
            QTextStream os(&file);
            os << m_cx.file(QFileInfo(assoc.fileName()).fileName(),"txt");
            file.close();
        }

        QStringList handles = assoc.values("general","items");
        for ( int i=0 ; i<handles.size() ; ++i )
        {
            Data::DataManager handle(handles[i]);
            //cx->saveOrUpdateFile(handle.fileName(),"items");

            QFile file2(handle.fileName());
            if ( file2.open(QIODevice::WriteOnly | QIODevice::Text) )
            {
                QTextStream os2(&file2);
                os2 << m_cx.file(QFileInfo(handle.fileName()).fileName(),"txt");
                file2.close();
            }

            QStringList subHandles = handle.values("general","items");
            for ( int j=0 ; j<subHandles.size() ; ++j )
            {
                QStringList items = handle.values(subHandles[j],"data");
                for ( int k=0 ; k<items.size() ; ++k)
                {
                    QString pathItem = Settings::basketsStorePath().toLocalFile()+QDir::separator()+"items"+QDir::separator()+items[k]+".html";
                    //cx->saveOrUpdateFile(pathItem,"items");

                    QFile file3(pathItem);
                    if ( file3.open(QIODevice::WriteOnly | QIODevice::Text) )
                    {
                        QTextStream os3(&file3);
                        os3 << m_cx.file(QFileInfo(pathItem).fileName(),"txt");
                        file3.close();
                    }

                }
            }
        }
    }

}



#endif // SYNCHROMANAGER_H
