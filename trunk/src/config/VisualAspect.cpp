/*
 Copyright (c) 2009 LEROY Anthony <leroy.anthony@gmail.com>

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Library General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Library General Public License for more details.

 You should have received a copy of the GNU Library General Public License
 along with this library; see the file COPYING.LIB.  If not, write to
 the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 Boston, MA 02110-1301, USA.
 */

#include "VisualAspect.h"

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
