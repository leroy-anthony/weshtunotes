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

#ifndef MOVEHANDLE_H_
#define MOVEHANDLE_H_

#include "../main/general.h"

#include <QWidget>

namespace Handle
{

    class HandleItem;

    class MoveHandle : public QWidget
    {
    public:
        MoveHandle( HandleItem * parent );
        virtual ~MoveHandle();

        void setHoverMode( bool isHover );

        void setDefaultColor();
        void setSelectionColor();
        void setDefaultColor( const QColor & c );

    protected:
        void mouseReleaseEvent ( QMouseEvent * event );
        void mousePressEvent ( QMouseEvent * event );
        void mouseMoveEvent ( QMouseEvent * event );
        void enterEvent( QEvent * event );
        void leaveEvent( QEvent * event );

    private:
        QColor m_defaultColor;

        enum Mode { Nothing, Move };

        Mode m_mode;

        int m_decalageMoveX;
        int m_decalageMoveY;

        int m_oldCursorX;
        int m_oldCursorY;

    };

}

#endif /*MOVEHANDLE_H_*/
