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

#ifndef CHOOSETAG_H_
#define CHOOSETAG_H_

#include <QWidget>

namespace Tag
{

    class ChooseTag : public QWidget
    {
        Q_OBJECT

    public:
        ChooseTag();
        virtual ~ChooseTag();

    };

}

#endif // CHOOSETAG_H_
