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

#include "AddTag.h"

#include <QDebug>
#include <QPainter>

#include <KAction>

#include "../tag/TagFactory.h"
#include "../tag/NoteTag.h"
#include "../item/NoteItem.h"
#include "../config/VisualAspect.h"

namespace Tag
{

    AddTag::AddTag( Item::AbstractItem * noteItem ):
            QPushButton( noteItem ),
            m_abstractItem( noteItem )
    {
        setFixedSize(16,13);
        setContentsMargins(0,0,0,0);
        hide();

        setStyleSheet(QString("background-color: %1").arg(Qt::transparent));
    }

    void AddTag::mousePressEvent( QMouseEvent * e )
    {
        Q_UNUSED( e );

        if ( TagFactory::tagsNames().size() > 0 )
        {
            loadTagsMenu();
            m_menu.popup(QCursor::pos());
        }
        else
        {
            Tag::TagFactory::newTagFactory()->show();
        }
    }

    void AddTag::loadTagsMenu()
    {
        m_menu.clear();
        QStringList tags = TagFactory::tagsNames();
        for ( int i=0 ; i<tags.size() ; ++i )
        {
            KAction * action = new KAction(0);
            action->setData(tags[i]);
            action->setText(tags[i]);
            action->setCheckable(true);
            action->setChecked( m_abstractItem->containTag(tags[i]) );
            m_menu.addAction(action);
        }
        if ( m_abstractItem != 0 )
        {
            connect(&m_menu, SIGNAL(triggered(QAction*)), m_abstractItem, SLOT(tagApply(QAction*)));
        }
    }

    void AddTag::enterEvent( QEvent * event )
    {
        Q_UNUSED( event );

        setCursor(Qt::PointingHandCursor);
    }

    void AddTag::paintEvent( QPaintEvent * event )
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

        float h = height()-2;
        float dx = 2;

        float h1 = h/3.0 + 1;
        float h2 = 2.0*h/3.0 + 1;
        float h3 = h;

        float w1 = h/3.0 + 1;
        float w2 = 2.0*h/3.0 + 1;
        float w3 = h;

        QPainterPath roundRectPath;
        roundRectPath.moveTo( dx, h1 );
        roundRectPath.lineTo( w1, h1 );
        roundRectPath.lineTo( w1, dx );
        roundRectPath.lineTo( w2, dx );
        roundRectPath.lineTo( w2, h1 );
        roundRectPath.lineTo( w3, h1 );
        roundRectPath.lineTo( w3, h2 );
        roundRectPath.lineTo( w2, h2 );
        roundRectPath.lineTo( w2, h3 );
        roundRectPath.lineTo( w1, h3 );
        roundRectPath.lineTo( w1, h2 );
        roundRectPath.lineTo( dx, h2 );
        roundRectPath.closeSubpath();

        painter.drawPath(roundRectPath);
    }

}
