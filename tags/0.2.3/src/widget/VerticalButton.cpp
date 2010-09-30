/*
    (K)WeshTaVue

  Copyright (c) 2006 LEROY Anthony <leroy.anthony@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "VerticalButton.h"
//#include "debug.h"

namespace Widget
{

    int VerticalButton::_staticId = 0;

    VerticalButton::VerticalButton(QWidget * parent) : QPushButton(parent)
    {
        setMouseTracking ( true );
        installEventFilter(this);

        pointeurSouris = 0;
        clickSouris = 0;

        QStyleOptionButton option;
        option.initFrom(this);
        couleurSelection = option.palette.highlight().color();

        red = 255;
        green = 255;
        blue = 255;
        h = 255;
        s = 255;

        v= 255.0;
        dv = couleurSelection.value()/400.0;

        xBoundingRect = 0;
        yBoundingRect = 0;

        _id = _staticId;
        _staticId++;
    }


    VerticalButton::~VerticalButton()
    {}

    bool VerticalButton::eventFilter(QObject *obj, QEvent *event)
    {
        if ( event->type() == QEvent::Leave )
        {
            pointeurSouris = 0;
            v= 255.0;
        }
        else if ( event->type() == QEvent::Enter )
        {
            pointeurSouris = 1;
        }
        else if ( event->type() == QEvent::MouseButtonPress )
        {
            clickSouris = 1;
        }
        else if ( event->type() == QEvent::MouseButtonRelease )
        {
            //clickSouris = 0;
        }

        return QObject::eventFilter(obj, event);
    }

    void VerticalButton::setText( const QString & text )
    {
        QFont newFont = font();
        newFont.setPixelSize(12);
        setFont(newFont);

        QFontMetrics fontMetrics(newFont);
        xBoundingRect = fontMetrics.boundingRect(text).width();
        yBoundingRect = fontMetrics.boundingRect(text).height();

        ((QPushButton*)this)->setText(text);
    }

    void VerticalButton::deSelectionner()
    {
        clickSouris = 0;
        update();
    }

    void VerticalButton::selectionner()
    {
        clickSouris = 1;
        update();
    }

    void VerticalButton::paintEvent( QPaintEvent * event )
    {
        const QRect r = event->rect();

        QPainter painter(this);

        pen.setColor( QColor( 120, 120, 120 ) );
        pen.setWidth(0);
        painter.setPen( pen );

        if ( clickSouris == 1 )
        {
            painter.setBrush( couleurSelection );
        }
        else if ( pointeurSouris == 1 )
        {
            QColor c(couleurSelection);
            c.setHsv( couleurSelection.hue(), couleurSelection.saturation(), v );
            painter.setBrush( c );

            if ( v-30>couleurSelection.value() )
            {
                v -= dv;
                update();
            }
        }

        if ( _id == 0 )
        {
            painter.drawRect( r.x(), r.y(), r.width()-1, r.height()-1 );
        }
        else if ( _id == _staticId )
        {
            painter.drawRect( r.x(), r.y()-1, r.width()-1, r.height() );
        }
        else
        {
            painter.drawRect( r.x(), r.y()-1, r.width()-1, r.height()-1 );
        }

        pen.setColor( QColor(0, 0, 0) );
        painter.setPen( pen );

        painter.translate ( r.width()/2+yBoundingRect/2, r.height()/2+xBoundingRect/2 );
        painter.rotate( -90.0 );

        if ( isDown () )
            painter.drawText( r.x()+1, r.y()-2, text());
        else
            painter.drawText( r.x(), r.y()-3, text());
    }

}
