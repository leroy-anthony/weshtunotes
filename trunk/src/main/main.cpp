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

#include <KApplication>
#include <KCmdLineArgs>
#include <KAboutData>
#include <kuniqueapplication.h>
#include <kmessagebox.h>

#include "main_window.h"
#include "../config/Configuration.h"

int main(int argc, char *argv[])
{
    KAboutData aboutData("KWeshTuNotes",
                         0,
                         ki18n("KWeshTuNotes"),
                         "0.0",
                         ki18n("Prise de note"),
                         KAboutData::License_GPL,
                         ki18n("(c) 2009"),
                         ki18n("Some text..."),
                         "http://code.google.com/p/weshtunotes/",
                         "leroy.anthony@gmail.com");

    KCmdLineArgs::init( argc, argv, &aboutData );

    KApplication app;
    app.setOrganizationName("weshTuNotes Corp.");

    if ( !KSystemTrayIcon::isSystemTrayAvailable() )
    {
        KMessageBox::error(0, QObject::tr("Systray"), QObject::tr("I couldn't detect any system tray on this system."));
        return 1;
    }

   // KApplication::setQuitOnLastWindowClosed(false);

    Config::Configuration::iniConfigration();

    //Démarrage de l'application
    MainWindow * main = new MainWindow( 0, argc, argv );
    main->show();

   //

    return app.exec();
}
