/*
 Copyright (c) 2009 LEROY Anthony <leroy.anthony@gmail.com>

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
 */

#include <QApplication>

#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("weshTuNotes Corp.");

    if ( !QSystemTrayIcon::isSystemTrayAvailable() )
    {
        QMessageBox::critical(0, QObject::tr("Systray"), QObject::tr("I couldn't detect any system tray on this system."));
        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    //FIXME: faire mieux ...
    QDir::addSearchPath( "icon", "./data/icon" ); // windows
    QDir::addSearchPath( "icon", "/usr/share/weshtunotes/data/icon" ); // unix
    QDir::addSearchPath( "icon", "/usr/local/share/weshtunotes/data/icon" ); // unix
    
    //Démarrage de l'application
    MainWindow ui( 0, argc, argv );
    ui.show();

    return app.exec();
}