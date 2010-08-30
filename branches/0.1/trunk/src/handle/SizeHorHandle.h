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

#ifndef SIZEHORHANDLE_H_
#define SIZEHORHANDLE_H_

#include <QWidget>

namespace Handle
{

    class HandleItem;

    class SizeHorHandle : public QWidget
    {
        Q_OBJECT

    public:
        SizeHorHandle( HandleItem * parent );
        virtual ~SizeHorHandle();

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

        enum Mode { Nothing, ScaleXItem };
        Mode m_mode;

        HandleItem * m_handleItemRoot;

        QColor m_defaultColor;

        bool m_isHover;

    };

}

#endif // SIZEHORHANDLE_H_
