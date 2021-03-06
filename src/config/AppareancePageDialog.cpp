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

#include "AppareancePageDialog.h"

#include <KConfigDialog>

#include "settings.h"

namespace Config
{

    AppareancePageDialog::AppareancePageDialog( QWidget * parent ):
            QWidget( parent )
    {
        setupUi( this );

        kcfg_iconBasket->setVisible(false);
        m_iconBasketButton->setIcon(Settings::iconBasket());

        kcfg_colorItem->setVisible(false);
        m_colorItem->setColor(Settings::colorItem());

        kcfg_colorBasket->setVisible(false);
        m_colorBasket->setColor(Settings::colorBasket());

        connect( m_iconBasketButton, SIGNAL(iconChanged(const QString &)), this, SLOT(iconBasketChanged(const QString &)) );
        connect( m_colorItem, SIGNAL(highlighted(const QColor &)), this, SLOT(colorItemChanged(const QColor &)) );
        connect( m_colorBasket, SIGNAL(highlighted(const QColor &)), this, SLOT(colorBasketChanged(const QColor &)) );
    }

    void AppareancePageDialog::iconBasketChanged( const QString & icon )
    {
        kcfg_iconBasket->setText(icon);
    }

    void AppareancePageDialog::colorItemChanged( const QColor & color )
    {
        kcfg_colorItem->setText( color.name() );
    }

    void AppareancePageDialog::colorBasketChanged( const QColor & color )
    {
        kcfg_colorBasket->setText( color.name() );
    }

}
