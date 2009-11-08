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

#include "DeleteHandle.h"

#include "HandleItem.h"
#include "../config/ImageFactory.h"
#include "../config/VisualAspect.h"

namespace Handle
{

    DeleteHandle::DeleteHandle( HandleItem * parent ):
            QPushButton(parent),
            m_handleItemRoot(parent)
    {
        setIcon(Config::ImageFactory::icon(Config::Image::exitAction));
        setIconSize(QSize(Config::VisualAspect::widthHandleControl,Config::VisualAspect::widthHandleControl));
        setFixedWidth(Config::VisualAspect::widthHandleControl);
        setFixedHeight(Config::VisualAspect::widthHandleControl);
        setContentsMargins(0,0,0,0);
        setDefaultColor(parent->defaultColor());
        setFlat(true);
        hide();
    }

    void DeleteHandle::setDefaultColor( const QColor & c )
    {
        m_defaultColor = QColor(c);
        setDefaultColor();
    }

    void DeleteHandle::setDefaultColor()
    {
        setStyleSheet( Config::VisualAspect::gradiantBackground( m_defaultColor ) );
    }

    void DeleteHandle::setHoverMode( bool isHover )
    {
        setVisible( isHover );
    }

}
