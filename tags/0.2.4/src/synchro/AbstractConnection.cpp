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

#include "AbstractConnection.h"

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
#include "../synchro/GoogleDocsConnection.h"

namespace Synchro
{

    AbstractConnection::AbstractConnection( const QString & connectionName ):
            m_networkAccessManager(0),
            m_connectionName(connectionName)
    {
        connect( &m_networkAccessManager, SIGNAL(error(const QString &)), this, SLOT(errorConnection(const QString &)) );
    }

    const QString & AbstractConnection::connectionName()
    {
        return m_connectionName;
    }

    const QString & AbstractConnection::connectionError()
    {
        return m_networkAccessManager.detailError();
    }

    void AbstractConnection::errorConnection(const QString & errorString )
    {
        emit error(errorString);
    }

}
