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

#include <QDir>

#include <KDebug>
#include <kpassworddialog.h>
#include <KWallet/Wallet>
#include <KCmdLineArgs>
#include <KAboutData>
#include <KApplication>

#include "../config/Configuration.h"
#include "../basket/ItemTreeBasket.h"
#include "../scene/AbstractScene.h"
#include "../data/DataManager.h"
#include "../main/MainWindow.h"

namespace Synchro
{

    template <class T>
    SynchroManager<T>::SynchroManager()
    {
    }

    template <class T>
    void SynchroManager<T>::commitGoogle()
    {
        //google( &Synchro::AbstractConnection::commitGoogle );
    }

    template <class T>
    void SynchroManager<T>::updateGoogle()
    {
        //google( &Synchro::AbstractConnection::updateGoogle );
        //load();
        //MainWindow::reloadView();
    }
/*
    template <class T>
    void SynchroManager<T>::google( void (SynchroManager::*actionPtr)(AbstractConnection &) )
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
                googleDialog( cx, dlg, wallet, actionPtr );
                return;
            }

            (this->*actionPtr)( cx );
            return;
        }

        googleDialog( cx, dlg, wallet, actionPtr );
    }

    template <class T>
    bool SynchroManager<T>::googleDialog( AbstractConnection & cx, KPasswordDialog & dlg, KWallet::Wallet * wallet, void (SynchroManager::*actionPtr)(AbstractConnection &) )
    {
        while ( dlg.exec() && !cx.authentication(dlg.username(), dlg.password()) )
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

            (this->*actionPtr)( cx );
        }
    }*/

}
