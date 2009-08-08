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

    private:    
        enum Mode { Nothing, ScaleXItem };
        Mode m_mode;

        HandleItem * m_handleItemRoot;

        QColor m_defaultColor;

    };

}

#endif // SIZEHORHANDLE_H_