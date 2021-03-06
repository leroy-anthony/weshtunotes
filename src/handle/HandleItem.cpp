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

#include "HandleItem.h"

#include <QFrame>
#include <QLabel>
#include <QDebug>
#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QFormLayout>
#include <QGraphicsGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSizePolicy>
#include <QApplication>
#include <QAction>
#include <QMenu>

#include <KDebug>
#include <KLocalizedString>
#include <KMessageBox>

#include "../config/Configuration.h"
#include "../config/VisualAspect.h"
#include "../item/AbstractItem.h"
#include "../scene/AbstractScene.h"
#include "../data/DataManager.h"
#include "../main/KWeshTuNotesApp.h"
#include "../main/MainWindow.h"
#include "MoveHandle.h"
#include "SizeHorHandle.h"
#include "DeleteHandle.h"
#include "PinHandle.h"
#include "settings.h"

namespace Handle
{

    HandleItem::HandleItem( Scene::AbstractScene * parent, int x, int y ) :
            QWidget(0),
            GeneratorID("handle"),
            m_scene(parent),
            m_item(0),
            m_parentHandle(0),
            m_isHover(false),
            m_isPin(false),
            m_modeDegroupement(false),
            m_index(-1),
            m_insertIndicator(0),
            m_contentMarginX(1),
            m_contentMarginY(1),
            m_x(x),
            m_y(y)
    {
        QWidget::resize(Settings::widthNote(),Settings::widthNote());

        setContentsMargins( m_contentMarginX, m_contentMarginY, m_contentMarginX, m_contentMarginY );

        m_sizeHorHandle = new SizeHorHandle(this);
        m_moveHandle = new MoveHandle(this);
        m_deleteHandle = new DeleteHandle(this);
        m_pinHandle = new PinHandle(this);

        m_handleLayout = new QGridLayout( this );
        m_handleLayout->setContentsMargins( 0, 0, 0, 0 );
        m_handleLayout->setSpacing( 0 );
        m_handleLayout->setSizeConstraint(QLayout::SetMaximumSize);

        m_handleLayout->addWidget( m_moveHandle, 0, 0 );

        m_contentLayout = new QVBoxLayout();
        m_contentLayout->setContentsMargins( 0, 0, 0, 0 );
        m_contentLayout->setSpacing( 0 );
        m_handleLayout->addLayout( m_contentLayout, 0, 1 );

        QVBoxLayout * h = new QVBoxLayout();
        h->addWidget( m_deleteHandle );
        h->addWidget( m_pinHandle );
        h->addWidget( m_sizeHorHandle );
        m_handleLayout->addLayout( h, 0, 2 );

        connect( m_deleteHandle, SIGNAL(pressed()), this, SLOT(questionDelItem()) );

        m_insertIndicator = new QFrame();
        m_insertIndicator->setMinimumHeight(24);
        m_insertIndicator->setStyleSheet( Config::VisualAspect::gradiantBackground( QApplication::palette().color(QPalette::Highlight) ) );
        m_insertIndicator->setVisible(false);
    }

    HandleItem::~HandleItem()
    {
    }

    void HandleItem::add( Item::AbstractItem * w )
    {
        m_contentLayout->addWidget(w);
        m_item = w;
	m_item->setHandle( this );
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
        setStyleSheet( Config::VisualAspect::gradiantBackground( m_defaultColor ) );
        m_moveHandle->setDefaultColor(m_defaultColor);
        m_sizeHorHandle->setDefaultColor(m_defaultColor);
        m_deleteHandle->setDefaultColor(m_defaultColor);
        m_pinHandle->setDefaultColor(m_defaultColor);
    }

    void HandleItem::add( HandleItem * h )
    {
        if ( h != 0 )
        {
            h->setHoverMode( false );
            h->setParentHandle( this );

            if ( m_index >= 0 && m_index<=m_handles.size() )
            {
                m_handles.insert( m_index, h );
                static_cast<QVBoxLayout*>(m_contentLayout)->insertWidget( m_index, h );
            }
            else
            {
                m_handles << h;
                static_cast<QVBoxLayout*>(m_contentLayout)->insertWidget( m_contentLayout->count(), h );
            }

            setDefaultColor(h->defaultColor());

            m_insertIndicator->setVisible(false);
        }
    }

    void HandleItem::setParentHandle(  HandleItem *  h )
    {
        m_parentHandle = h;

        if ( h == 0 )
        {
            setContentsMargins( m_contentMarginX, m_contentMarginY, m_contentMarginX, m_contentMarginY );
            update();
        }
        else
        {
            setContentsMargins( 0, 0, 0, 0 );
        }
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

    void HandleItem::resize( const QSize & size )
    {
        if ( m_item != 0 )
        {
            m_item->adaptSize();
        }
        else
        {
            for ( int i=0 ; i<m_handles.size() ; ++i )
            {
                m_handles[i]->resize(size);
            }
        }

        QWidget::resize( size );
    }

    HandleItem * HandleItem::handleItemAt( int x, int y )
    {
        QPoint pt = mapFromParent(QPoint(x,y));
        for (int i = 0; i < m_handles.size(); ++i)
        {
            if ( m_handles[i]->geometry().contains(pt) )
            {
                return m_handles[i]->handleItemAt( pt.x(), pt.y() );
            }
        }

        return this;
    }

    void HandleItem::insert( QPoint  pt, int height )
    {
        Q_UNUSED(height);

        int size = m_contentLayout->count();
        if ( m_index >=0 && m_index < size )
        {
            m_contentLayout->removeWidget(m_contentLayout->itemAt(m_index)->widget());
            size = m_contentLayout->count();
        }

        QPoint p = mapFromGlobal( pt );

        int choix = 0;
        int distance = 99999;
        QWidget * w = 0;
        int d = 0;
        for ( int i=0 ; i<=size && size != 0 ; ++i )
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

        m_index = choix;

        static_cast<QVBoxLayout*>(m_contentLayout)->insertWidget( choix, m_insertIndicator );

        m_insertIndicator->setVisible(true);
    }

    void HandleItem::resetInsert()
    {
        m_index = -1;
        m_insertIndicator->setVisible(false);
    }

    void HandleItem::setModeDegroupement( bool m_mode )
    {
        m_modeDegroupement = m_mode;
    }

    HandleItem * HandleItem::child()
    {
        if ( m_handles.size() == 1 )
        {
            return m_handles[0];
        }

        return 0;
    }

    void HandleItem::setHoverMode( bool isHover )
    {
        m_isHover = isHover;
        m_moveHandle->setHoverMode( isHover );
        m_sizeHorHandle->setHoverMode( isHover );

        if ( m_parentHandle == 0 ) // on ne peut pas effacer/punaiser les handles qui dans dans des handles
        {
            m_deleteHandle->setHoverMode( isHover );
            m_pinHandle->setHoverMode( isHover );
        }

        if ( m_item != 0 )
        {
            m_item->setVisibleAddTag(isHover);
        }
    }

    void HandleItem::enterEvent( QEvent * event )
    {
        Q_UNUSED( event );

        setHoverMode( true && !m_isPin );
        setContentsMargins( m_contentMarginX, m_contentMarginY, m_contentMarginX, m_contentMarginY );
    }

    void HandleItem::leaveEvent( QEvent * event )
    {
        Q_UNUSED(event);

        setHoverMode( false );
        if ( m_parentHandle != 0 )
        {
            setContentsMargins( 0, 0, 0, 0 );
        }
    }
    
    void HandleItem::load( const QString & id )
    {
        Data::DataManager settings( Data::DataManager::configFileItem( id ) );

        int width = settings.valueGroup( id, "width", Settings::widthNote()).toInt();
        QWidget::resize(width, width);

        setPin( settings.valueGroup( id, "pin", false) != "false" );

        setId(id);
    }

    void HandleItem::load()
    {
        load( GeneratorID::id() );
    }

    void HandleItem::save()
    {

        QStringList children;
        for ( int i=0 ; i<m_handles.size() ; ++i)
        {
            children << m_handles[i]->id();
        }

        QString configFileHandle = configFile();

        Data::DataManager settings( configFileHandle );

        settings.setValue(GeneratorID::id(),"height",height());
        settings.setValue(GeneratorID::id(),"width",width());
        settings.setValue(GeneratorID::id(),"pin",m_isPin);

        if ( children.size() > 0 )
        {
            settings.setValue(GeneratorID::id(),"items",children);
        }
        else
        {
            m_item->save(configFileHandle,GeneratorID::id());
        }

        settings.setValue(GeneratorID::id(),"color",m_defaultColor.name());

        for ( int i=0 ; i<m_handles.size() ; i++)
        {
            m_handles[i]->save();
        }
    }

    void HandleItem::paintEvent( QPaintEvent * event )
    {
        QPainter painter(this);

        QLinearGradient gradient( 0, 0, 0, height());
        gradient.setColorAt( 0, m_defaultColor.lighter( Config::VisualAspect::lighterIntensity ) );
        gradient.setColorAt( 1, m_defaultColor );
        painter.setBrush( gradient);

        if ( m_parentHandle == 0 || m_isHover )
        {
            QPen p(QApplication::palette().color(QPalette::Highlight));
            p.setWidth(3);
            p.setJoinStyle(Qt::MiterJoin);
            painter.setPen(p);
        }

        painter.drawRect( event->rect().x(), event->rect().y(), event->rect().width(), event->rect().height() );
    }

    void HandleItem::questionDelItem()
    {
        int reply = KMessageBox::questionYesNo(0, i18n("Do you really want to delete this note ?"),
                                               i18n("Delete note"),
                                               KStandardGuiItem::yes(),
                                               KStandardGuiItem::no(),
                                               i18n("delete note"));
        if (reply == KMessageBox::Yes)
        {
            emit delItem( this );
        }
    }

    int HandleItem::x()
    {
        return m_x;
    }

    int HandleItem::y()
    {
        return m_y;
    }

    int HandleItem::contentMarginX()
    {
        return m_contentMarginX;
    }

    int HandleItem::contentMarginY()
    {
        return m_contentMarginY;
    }

    const QColor & HandleItem::defaultColor()
    {
        return m_defaultColor;
    }

    HandleItem * HandleItem::parentHandle()
    {
        return m_parentHandle;
    }

    bool HandleItem::isRoot()
    {
        return (m_scene != 0);
    }

    Item::AbstractItem * HandleItem::abstractItem()
    {
        return m_item;
    }

    bool HandleItem::modeDegroupement()
    {
        return m_modeDegroupement;
    }

    int HandleItem::nbItems()
    {
        return m_handles.size();
    }

    const QList<HandleItem*> & HandleItem::children()
    {
        return m_handles;
    }

    QString HandleItem::configFile() const
    {
        if ( m_parentHandle != 0 )
        {
            return m_parentHandle->configFile();
        }

        return Data::DataManager::configFileItem( GeneratorID::id() );
    }

    int HandleItem::indexInsert()
    {
        return m_index;
    }

    void HandleItem::setIndexInsert( int i )
    {
        m_index = i;
    }

    void HandleItem::setPin( bool pin )
    {
        m_isPin = pin;
        m_moveHandle->setDisabled(pin);
        m_sizeHorHandle->setDisabled(pin);

        if ( m_item != 0 )
        {
            m_item->setPin(pin);
        }
        else
        {
            for ( int i=0 ; i<m_handles.size() ; ++i )
            {
                m_handles[i]->setPin(pin);
            }
        }
    }

    bool HandleItem::isPin()
    {
        return m_isPin;
    }

    void HandleItem::setLock()
    {
        if ( m_parentHandle != 0 )
        {
            m_parentHandle->setPin(!m_isPin);
        }
        else
        {
            setPin(!m_isPin);
        }
    }

    void HandleItem::contextMenuEvent ( QContextMenuEvent * event )
    {
        QMenu * menu = new QMenu(KWeshTuNotesApp::mainWindow());

        if ( m_isPin )
        {
            QAction * a = new QAction("Unlock",this);
            menu->addAction(a);
            connect(a, SIGNAL(triggered(bool)), this, SLOT(setLock()));
        }
        else
        {
            QAction * a = new QAction("Lock",this);
            menu->addAction(a);
            connect(a, SIGNAL(triggered(bool)), this, SLOT(setLock()));
        }

        menu->exec(QCursor::pos());
    }

}
