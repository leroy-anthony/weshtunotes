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

#ifndef NEXTTAG_H
#define NEXTTAG_H

#include <QPushButton>

namespace Item
{
    class AbstractItem;
}

namespace Tag
{

    class NextTag : public QPushButton
    {
        Q_OBJECT

    public:
        NextTag( Item::AbstractItem * noteItem );

    protected:
        void enterEvent( QEvent * event );
        void mousePressEvent( QMouseEvent * e );
        void paintEvent( QPaintEvent * event );

    private:
        Item::AbstractItem * m_abstractItem;

    };

}

#endif // NEXTTAG_H
