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

#ifndef APPAREANCEPAGEDIALOG_H
#define APPAREANCEPAGEDIALOG_H

#include <QWidget>

#include <AppareanceSettings.h>

namespace Config
{

    class AppareancePageDialog : public QWidget, public Ui::AppareanceSettings
    {
        Q_OBJECT

    public:
        AppareancePageDialog( QWidget * parent );

    public slots:
        void iconBasketChanged( const QString & icon );

    };

}

#endif // APPAREANCEPAGEDIALOG_H