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

#include "ConfigDialog.h"

#include <settings.h>

#include <QWidget>

#include <KLocalizedString>
#include <KConfigDialog>

#include "../config/GeneralPageDialog.h"
#include "../config/AppareancePageDialog.h"
#include "../config/AnimationPageDialog.h"
#include "../config/SynchroPageDialog.h"

namespace Config
{

    ConfigDialog::ConfigDialog( QWidget * parent )
    {
        if (KConfigDialog::showDialog("settings"))
        {
            return;
        }

        KConfigDialog *dialog = new KConfigDialog(parent, "settings", Settings::self());
        dialog->resize(650,400);
        dialog->setFaceType(KPageDialog::List);
        dialog->addPage(new Config::GeneralPageDialog(dialog), i18n("General"), "system-run" );
        dialog->addPage(new Config::AppareancePageDialog(dialog), i18n("Appareance note"), "preferences-desktop-color" );
        dialog->addPage(new Config::AnimationPageDialog(dialog), i18n("Animation"), "preferences-desktop-launch-feedback" );

        Config::SynchroPageDialog * p = new Config::SynchroPageDialog(dialog);
        connect( p, SIGNAL(changed()), dialog, SLOT(settingsChangedSlot()) );
        dialog->addPage(p, i18n("Synchronisation"), "preferences-desktop-launch-feedback" );
        p->load();

        dialog->show();

        connect( dialog, SIGNAL(settingsChanged(const QString&)), this, SLOT(updateConfiguration()) );
    }

    void ConfigDialog::updateConfiguration()
    {
        Settings::self()->writeConfig();
    }

}
