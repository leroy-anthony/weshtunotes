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

#include <iostream>

#include <KApplication>
#include <KCmdLineArgs>
#include <KAboutData>
#include <KSystemTrayIcon>
#include <kmessagebox.h>

#include "KWeshTuNotesApp.h"
#include "../config/ImageFactory.h"

int main(int argc, char *argv[])
{
    KAboutData aboutData("kweshtunotes",
                         "kweshtunotes",
                         ki18n("(K) Wesh ! Tu Notes ?"),
                         "0.0",
                         ki18n("(K)WeshTuNotes is a note-taking application, just like BasKet Note Pads."),
                         KAboutData::License_GPL_V3,
                         ki18n("(c) 2009"),
                         ki18n("(K)WeshTuNotes allows you to write notes, copy/paste images and files to better organize them."),
                         "http://code.google.com/p/weshtunotes/",
                         "leroy.anthony@gmail.com");

    aboutData.addAuthor( ki18n("Anthony Leroy"),
                         ki18n("dev"),
                         "leroy.anthony@gmail.com",
                         "http://code.google.com/p/weshtunotes/" );

    aboutData.setProgramIconName("basket");

    KCmdLineArgs::init( argc, argv, &aboutData );
   // KUniqueApplication::addCmdLineOptions();

   /* if (!KWeshTuNotesApp::start())
    {
        std::cerr << "KWeshTuNotes is already running!\n" << std::endl;
        return 0;
    }*/

    KWeshTuNotesApp app;

    return app.exec();
}
