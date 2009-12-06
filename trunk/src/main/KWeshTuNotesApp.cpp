/*
  Copyright (c) 2009 LEROY Anthony <leroy.anthony@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "KWeshTuNotesApp.h"

#include "main_window.h"
#include "../config/Configuration.h"

KWeshTuNotesApp::KWeshTuNotesApp():
        KApplication(),
        m_mainWindow( 0 )
{
    setQuitOnLastWindowClosed ( false );

    Config::Configuration::iniConfigration();

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
