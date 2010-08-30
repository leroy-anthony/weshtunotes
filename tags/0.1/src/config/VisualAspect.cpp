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

#include "VisualAspect.h"

#include <QColor>

namespace Config
{

    VisualAspect::VisualAspect()
    {
    }

    QString VisualAspect::gradiantBackground( const QColor & color )
    {
        return QString("border: none; background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 %1, stop:1 %2)")
                .arg(color.lighter(lighterIntensity).name())
                .arg(color.name());
    }

    int VisualAspect::lighterIntensity = 150;
    int VisualAspect::widthHandleControl = 9;

}
