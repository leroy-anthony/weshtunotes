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

#include "KWeshTuNotesApp.h"

#include "MainWindow.h"
#include "../config/Configuration.h"
#include "../data/DataManager.h"

KWeshTuNotesApp::KWeshTuNotesApp():
        KUniqueApplication(),
        m_mainWindow( 0 )
{
    setQuitOnLastWindowClosed ( false );

    Data::DataManager::iniConfigration();

    m_mainWindow = new MainWindow();
    m_mainWindow->show();
}

KWeshTuNotesApp::~KWeshTuNotesApp()
{
    delete m_mainWindow;
}

int KWeshTuNotesApp::newInstance()
{
    if ( m_mainWindow != 0 )
    {
        m_mainWindow->show();
    }

    return 0;
}