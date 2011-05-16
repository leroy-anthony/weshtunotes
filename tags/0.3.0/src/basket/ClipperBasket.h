/*
    Copyright (c) 2011 LEROY Anthony <leroy.anthony@gmail.com>

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



#ifndef CLIPPERBASKET_H
#define CLIPPERBASKET_H

#include "../basket/AbstractBasket.h"

class QClipboard;

namespace Basket
{

    class ClipperBasket : public Basket::AbstractBasket
    {
        Q_OBJECT

    public:
        ClipperBasket( ItemTreeBasket * itemTreeBasket, const QString & id );

        void save();
        void load();
        void del();

    public slots:
        void clipboardDataChanged();

    private:
        QClipboard * m_clipboard;

        int m_maxItem;

    };

}

#endif // CLIPPERBASKET_H
