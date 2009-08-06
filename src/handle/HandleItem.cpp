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

#include "HandleItem.h"

#include <QFrame>
#include <QLabel>
#include <QDebug>
#include <QApplication>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QFormLayout>
#include <QGraphicsGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSizePolicy>
#include <QApplication>
#include <QSvgRenderer>

#include "../config/Configuration.h"

namespace Handle
{

    QWidget * HandleItem::m_w = 0;
    int HandleItem::m_index = 0;
    int HandleItem::m_id = 1;

    HandleItem::HandleItem( Scene::AbstractScene * parent, int x, int y ) :
            QWidget(0),
            m_scene(parent),
            m_item(0),
            m_sizeHorHandle(this),
            m_moveHandle(this),
            m_parentHandle(0),
            m_modeDegroupement(false),
            m_x(x),
            m_y(y)
    {
        setContentsMargins( 2,2,2,2 );
        m_handleId = QString("handle%1").arg(m_id);
        ++m_id;

        m_handleLayout = new QGridLayout( this );
        m_handleLayout->addWidget( &m_moveHandle, 0, 0 );
        m_handleLayout->addWidget( &m_sizeHorHandle, 0, 2 );
        m_handleLayout->setContentsMargins( 0, 0, 0, 0 );
        m_handleLayout->setSpacing( 0 );

        m_handleLayout->setSizeConstraint(QLayout::SetMaximumSize);

        m_contentLayout = new QVBoxLayout();
        m_contentLayout->setContentsMargins( 0, 0, 0, 0 );
        m_contentLayout->setSpacing( 0 );

        m_handleLayout->addLayout( m_contentLayout, 0, 1 );

        if ( m_w == 0 )
        {
            QFrame * line = new QFrame();
            line->setMinimumHeight(24);
            line->setStyleSheet("background: #F7F7C8;");
            line->setStyleSheet("border-color: black; border-width: 1px; border-style: dashed;");

            m_w = line;
        }
    }

    HandleItem::~HandleItem()
    {
    }

    void HandleItem::add( Item::AbstractItem * w )
    {
        m_contentLayout->addWidget(w);
        m_item = w;
        setDefaultColor(m_item->color());
        connect( w, SIGNAL(colorChange()), this, SLOT(setDefaultColor()) );
    }

    void HandleItem::setDefaultColor()
    {
        if ( m_item != 0 )
        {
            setDefaultColor(m_item->color());
        }
    }

    void HandleItem::setDefaultColor( const QColor & color )
    {
        m_defaultColor = QColor(color);
        setStyleSheet( QString("background: %1;").arg(m_defaultColor.name()) );
        m_moveHandle.setDefaultColor(m_defaultColor);
        m_sizeHorHandle.setDefaultColor(m_defaultColor);
    }

    const QColor & HandleItem::defaultColor()
    {
        return m_defaultColor;
    }

    void HandleItem::add( HandleItem * h )
    {
        if ( h != 0 )
        {
            static_cast<QVBoxLayout*>(m_contentLayout)->insertWidget( m_index, h );
            h->setParentHandle(this);
            m_handles.insert( m_index, h );
            setDefaultColor(h->defaultColor());
            ++m_index;
        }
    }

    void HandleItem::setParentHandle(  HandleItem *  h )
    {
        m_parentHandle = h;

        if ( h == 0 )
        {
            setContentsMargins( 2, 2, 2, 2 );
            update();
        }
        else
        {
            setContentsMargins( 0, 0, 0, 0 );
        }

    }

    HandleItem * HandleItem::parentHandle()
    {
        return m_parentHandle;
    }

    void HandleItem::remove( HandleItem * h )
    {
        m_handles.removeOne(h);
    }

    void HandleItem::moveItem( int x, int y )
    {
        m_x = x;
        m_y = y;
        emit move( this, x, y );
    }

    bool HandleItem::isRoot()
    {
        return (m_scene != 0);
    }

    Item::AbstractItem *  HandleItem::noteItem()
    {
        return m_item;
    }

    HandleItem * HandleItem::handleItemAt( int x, int y )
    {
        for (int i = 0; i < m_handles.size(); ++i)
        {
            if ( m_handles[i]->geometry().contains(mapFromParent(QPoint(x,y))) )
                return m_handles[i]->handleItemAt( x, y );
        }

        return this;
    }

    void HandleItem::insert( QWidget * w, int index )
    {
        static_cast<QVBoxLayout*>(m_contentLayout)->insertWidget( index, w );
    }

    void HandleItem::insert( QPoint  pt, int height )
    {
        int size = m_contentLayout->count();

        m_w->setVisible(true);
    //    m_w->setMaximumHeight(height);

        QPoint p =mapFromGlobal( pt );

        int choix = 0;
        int distance = 99999;
        QWidget * w = 0;
        int d = 0;
        for ( int i=0 ; i<=size ; i++ )
        {
            if ( i != size )
            {
                w = m_contentLayout->itemAt(i)->widget();
                d = abs(w->geometry().top()-p.y());
            }
            else
            {
                w = m_contentLayout->itemAt(i-1)->widget();
                d = abs(w->geometry().bottom()-p.y());
            }

            if ( d < distance )
            {
                choix = i;
                distance = d;
            }
        }

        if ( m_index != choix )
        {
            m_index = choix;
            insert( m_w, choix );
        }
    }

    void HandleItem::resetInsert()
    {
        m_index = -1;
        m_w->setVisible(false);
    }

    bool HandleItem::modeDegroupement()
    {
        return m_modeDegroupement;
    }

    void HandleItem::setModeDegroupement( bool m_mode )
    {
        m_modeDegroupement = m_mode;
    }

    int HandleItem::size()
    {
        return m_handles.size();
    }

    HandleItem * HandleItem::child()
    {
        if ( m_handles.size() == 1 )
        {
            return m_handles[0];
        }

        return 0;
    }

    const QList<HandleItem*> & HandleItem::children()
    {
        return m_handles;
    }

    void HandleItem::setHoverMode( bool isHover )
    {
        m_moveHandle.setHoverMode( isHover );
        m_sizeHorHandle.setHoverMode( isHover );
        if ( m_item != 0 )
        {
            m_item->setVisibleTag(isHover);
        }
    }

    void HandleItem::enterEvent( QEvent * event )
    {
        setHoverMode( true );
        setContentsMargins( 2,2,2,2 );
    }

    void HandleItem::leaveEvent( QEvent * event )
    {
        setHoverMode( false );
        if ( m_parentHandle != 0 )
        {
            setContentsMargins( 0, 0, 0, 0 );
        }
    }

    void HandleItem::save( const QString & fileName )
    {
        QStringList childs;
        for ( int i=0 ; i<m_handles.size() ; ++i)
        {
            childs << m_handles[i]->handleId();
        }

        Config::Configuration settings( fileName );

        settings.beginGroup(m_handleId);
        settings.setValue("x",m_x);
        settings.setValue("y",m_y);
        if ( childs.size() > 0 )
        {
            settings.setValue("items",childs);
        }
        else
        {
            m_item->save(fileName,m_handleId);
        }
        settings.setValue("color",m_defaultColor);

        settings.endGroup();
        settings.sync();

        for ( int i=0 ; i<m_handles.size() ; i++)
        {
            m_handles[i]->save(fileName);
        }
    }

    void HandleItem::load( const QString & fileName )
    {
        Config::Configuration settings( fileName );
        m_x = settings.value("x").toInt();
        m_y = settings.value("y").toInt();
        setDefaultColor( settings.value("color").value<QColor>() );
    }

    int HandleItem::x()
    {
        return m_x;
    }

    int HandleItem::y()
    {
        return m_y;
    }

    const QString & HandleItem::handleId()
    {
        return m_handleId;
    }

    void HandleItem::setHandleId( const QString & id )
    {
        m_handleId = id;
        QString idStr = QString(id).replace("handle","");
        if ( idStr.toInt() > m_id )
        {
            m_id = idStr.toInt() + 1;
        }
    }

    void HandleItem::paintEvent( QPaintEvent * event )
    {
        QRect r = event->rect();
        QPainter painter(this);
        painter.drawRect( r.x(), r.y(), r.width()-1, r.height()-1 );
    }

}
