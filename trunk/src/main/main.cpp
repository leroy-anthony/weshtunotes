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

#include <iostream>

#include <KUniqueApplication>
#include <KCmdLineArgs>
#include <KAboutData>
#include <KSystemTrayIcon>
#include <KMessageBox>

#include <QTextBrowser>

#include "KWeshTuNotesApp.h"

#include "version.h" // CMake generated include

int main(int argc, char *argv[])
{
    KAboutData aboutData("kweshtunotes",
                         "kweshtunotes",
                         ki18n("(K) Wesh ! Tu Notes ?"),
                         VERSION_INFO,
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

    KUniqueApplication::addCmdLineOptions();

    /*if (!KWeshTuNotesApp::start())
    {
        std::cerr << "KWeshTuNotes is already running!\n" << std::endl;
        return 0;
    }*/

    KWeshTuNotesApp app;

    return app.exec();
}
