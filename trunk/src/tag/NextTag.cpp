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

#include "NextTag.h"

#include <QDebug>
#include <QPainter>
#include <QApplication>

#include "../tag/NoteTag.h"
#include "../item/NoteItem.h"
#include "../config/VisualAspect.h"

namespace Tag
{

    NextTag::NextTag( Item::AbstractItem * noteItem ):
            QPushButton( noteItem ),
            m_abstractItem( noteItem )
    {
        setFixedSize(13,13);
        setContentsMargins(0,0,0,0);
        hide();

        setStyleSheet(QString("background-color: %1").arg(Qt::transparent));
    }

    void NextTag::enterEvent( QEvent * event )
    {
        Q_UNUSED( event );

        setCursor(Qt::PointingHandCursor);
    }

    void NextTag::mousePressEvent( QMouseEvent * e )
    {
        Q_UNUSED( e );

        NoteTag * tag = m_abstractItem->tag();
        if ( tag != 0 )
        {
            tag->nextState();
        }

        QPushButton::mousePressEvent(e);
    }

    void NextTag::paintEvent( QPaintEvent * event )
    {
        Q_UNUSED(event);

        QPainter painter(this);

        QPen p(QApplication::palette().color(QPalette::Highlight));
        p.setWidth(1);
        painter.setPen(p);

        QLinearGradient gradient(0,0,0, height());
        gradient.setColorAt(0, QApplication::palette().color(QPalette::Highlight).lighter(Config::VisualAspect::lighterIntensity));
        gradient.setColorAt(1, QApplication::palette().color(QPalette::Highlight));
        painter.setBrush( gradient );

        float dx = 1;

        float h1 = (height()-1)/3.0 + 0.5;
        float h2 = 2.0*(height()-1)/3.0 + 0.5;
        float h3 = height()-1;

        float w1 = (height()-1)/3.0 + dx ;
        float w3 = height()-1;

        QPainterPath roundRectPath;
        roundRectPath.moveTo(dx, h1);
        roundRectPath.lineTo(w1, h1);
        roundRectPath.lineTo(w1,  1);
        roundRectPath.lineTo(w3, height()/2.0);
        roundRectPath.lineTo(w1, h3);
        roundRectPath.lineTo(w1, h2);
        roundRectPath.lineTo(dx, h2);
        roundRectPath.closeSubpath();

        painter.drawPath(roundRectPath);
    }

}
