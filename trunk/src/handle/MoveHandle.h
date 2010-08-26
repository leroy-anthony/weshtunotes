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
        void paintEvent( QPaintEvent * event );

    private:
        void drawHandle( QPainter & painter, HandleItem * h, int & x, int & y );
        void drawGrip( QPainter & painter );

        QColor m_defaultColor;

        enum Mode { Nothing, Move };

        Mode m_mode;

        int m_decalageMoveX;
        int m_decalageMoveY;

        int m_oldCursorX;
        int m_oldCursorY;

        bool m_isHover;

    };

}

#endif /*MOVEHANDLE_H_*/
