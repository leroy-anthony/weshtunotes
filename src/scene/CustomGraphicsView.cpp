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

#include "CustomGraphicsView.h"

#include <algorithm>
#include <limits>

#include <QMatrix>
#include <QPoint>
#include <QWheelEvent>
#include <QClipboard>
#include <QMimeData>
#include <QApplication>
#include <QGLWidget>
#include <QGraphicsItem>

#include "AbstractScene.h"
#include "../main/general.h"
#include "../main/MainWindow.h"
#include "../handle/HandleItem.h"

namespace Scene
{

    CustomGraphicsView::CustomGraphicsView():
            QGraphicsView(),
            m_scale(1.0),
            m_move(false),
            m_selection(false),
            m_selectionItem(0)
    {
        if ( QGLFormat::hasOpenGL() )
        {
            setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
        }
        /*
      QGraphicsView::FullViewportUpdate
      QGraphicsView::MinimalViewportUpdate
      QGraphicsView::SmartViewportUpdate
      QGraphicsView::BoundingRectViewportUpdate
      QGraphicsView::NoViewportUpdate
      */
        setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
        /*
      QGraphicsView::CacheNone
      QGraphicsView::CacheBackground
      */
        setCacheMode( QGraphicsView::CacheNone );
        /*
      QGraphicsView::DontAdjustForAntialiasing
      QGraphicsView::DontSavePainterState
      QGraphicsView::DontClipPainter
      */
        //m_view->setOptimizationFlags( QGraphicsView::DontClipPainter );

        setRenderHint(QPainter::SmoothPixmapTransform, true);
        setRenderHint(QPainter::Antialiasing, false);
        setRenderHint(QPainter::TextAntialiasing, false);
        setRenderHint(QPainter::HighQualityAntialiasing, true);
        setRenderHint(QPainter::NonCosmeticDefaultPen, false);

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);//FullViewportUpdate);
        setViewportMargins(4,4,4,4);

        setAcceptDrops(true);
    }

    void CustomGraphicsView::mousePressEvent(QMouseEvent * event)
    {
        if ( scene()->itemAt( mapToScene( event->posF().x(), event->posF().y() ) ) == 0 )
        {
            if ( event->modifiers() == Qt::ControlModifier && event->button() == Qt::LeftButton )
            {
                m_selection = true;

                if ( m_selectionItem == 0 )
                {
                    m_selectionItem = scene()->addPath( QPainterPath() );

                    QColor colorSelection = QApplication::palette().color(QPalette::Highlight);
                    m_selectionItem->setPen(QPen(colorSelection, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
                    colorSelection.setAlpha(100);
                    m_selectionItem->setBrush(colorSelection);
                    m_selectionItem->setZValue(100);
                }

                if ( m_selectionItem->scene() != scene() )
                {
                    m_selectionItem->scene()->removeItem( m_selectionItem );
                    scene()->addItem( m_selectionItem );
                }

                m_selectionItem->setPath( QPainterPath() );
                m_selectionItem->show();
            }
            else
            {
                m_move = true;
            }

            m_position = event->posF();
        }
        QGraphicsView::mousePressEvent(event);
    }

    void CustomGraphicsView::mouseReleaseEvent(QMouseEvent * event)
    {
        m_move = false;
        m_selection = false;

        if ( m_selectionItem != 0 )
        {
            m_selectionItem->hide();
        }

        QGraphicsView::mouseReleaseEvent(event);
    }

    void CustomGraphicsView::mouseMoveEvent(QMouseEvent * event)
    {
        if ( m_selection )
        {
            QPainterPath selectionPath;

            int x = std::min(m_position.x(),event->posF().x());
            int y = std::min(m_position.y(),event->posF().y());
            int h = abs(event->posF().y() - m_position.y());
            int w = abs(event->posF().x() - m_position.x());

            QPolygonF r = mapToScene( x, y, w, h );
            if ( r.size() > 0 )
            {
                selectionPath.addPolygon( r );
                selectionPath.closeSubpath();

                m_selectionItem->setPath( selectionPath );
                scene()->setSelectionArea( selectionPath );
            }
        }
        else if ( m_move )
        {
            double dx = (event->posF().x() - m_position.x()) / matrix().m11();
            double dy = (event->posF().y() - m_position.y()) / matrix().m22();

            setInteractive(false);
            translate(dx, dy);
            setInteractive(true);

            m_position = event->posF();
        }
        else
        {
            QGraphicsView::mouseMoveEvent(event);
        }
    }

    void CustomGraphicsView::wheelEvent(QWheelEvent * e)
    {
        int numSteps = ( e->delta() / 8 ) / 15;
        double f = 1.2;

        QMatrix mat = matrix();
        QPoint mousePosition = e->pos();

        mat.translate((width() / 2) - mousePosition.x(), (height() / 2) - mousePosition.y());

        if ( numSteps > 0 )
        {
            mat.scale( numSteps * f, numSteps * f );
            m_scale = numSteps * f;
        }
        else
        {
            mat.scale( -1 / ( numSteps * f ), -1 / ( numSteps * f ) );
            m_scale = -1 / (numSteps * f);
        }

        mat.translate(mousePosition.x() - (width() / 2), mousePosition.y() - (height() / 2));
        setMatrix(mat);
    }

    void CustomGraphicsView::paste()
    {
        if ( !m_copyHandlesItem.isEmpty() )
        {
            AbstractScene * s =  static_cast<AbstractScene*>( scene() );

            QStringList filesName;
            for ( int i=0 ; i<m_copyHandlesItem.size() ; ++i )
            {
                filesName << m_copyHandlesItem[i]->handleItem()->fileName();
            }

            double minX = std::numeric_limits<double>::max();
            double maxX = std::numeric_limits<double>::min();
            double minY = std::numeric_limits<double>::max();
            double maxY = std::numeric_limits<double>::min();
            for ( int i=0 ; i<m_copyHandlesItem.size() ; ++i )
            {
                if ( m_copyHandlesItem[i]->x() < minX )
                {
                    minX = m_copyHandlesItem[i]->x();
                }
                if ( (m_copyHandlesItem[i]->x() + m_copyHandlesItem[i]->boundingRect().width()) > maxX )
                {
                    maxX = m_copyHandlesItem[i]->x() + m_copyHandlesItem[i]->boundingRect().width();
                }
                if ( m_copyHandlesItem[i]->rect().y() < minY )
                {
                    minY = m_copyHandlesItem[i]->rect().y();
                }
                if ( (m_copyHandlesItem[i]->rect().y() + m_copyHandlesItem[i]->rect().height()) > maxY )
                {
                    maxY = m_copyHandlesItem[i]->rect().y() + m_copyHandlesItem[i]->rect().height();
                }
            }

            s->loadHandles( filesName,  mapToScene( viewport()->width()/2, viewport()->height()/2 ), abs(maxX-minX), abs(maxY-minY) );
        }
        else
        {
            AbstractScene * s =  static_cast<AbstractScene*>( scene() );

            const QClipboard * clipboard = QApplication::clipboard();
            const QMimeData * mimeData = clipboard->mimeData();

            s->addData( mimeData );
        }
    }

    void CustomGraphicsView::copy()
    {
        AbstractScene * s =  static_cast<AbstractScene*>( scene() );
        s->save();

        QList<QGraphicsItem*> items = s->selectedItems();
        m_copyHandlesItem.clear();

        for ( int i=0 ; i<items.size() ; ++i )
        {
            m_copyHandlesItem << static_cast<Handle::GraphicHandleItem*>(items[i]);
        }
    }

    void CustomGraphicsView::deleteItem()
    {
        AbstractScene * s =  static_cast<AbstractScene*>( scene() );

        QList<QGraphicsItem*> items = s->selectedItems();
        for ( int i=0 ; i<items.size() ; ++i )
        {
            s->delItem( static_cast<Handle::GraphicHandleItem*>(items[i])->handleItem() );
        }
    }

    void CustomGraphicsView::resetZoom()
    {
        int dx = matrix().dx();
        int dy = matrix().dy();
        resetMatrix();
        translate(dx,dy);
    }

    void CustomGraphicsView::doubleZoom()
    {
        scale(2.0,2.0);
    }

    void CustomGraphicsView::halfZoom()
    {
        scale(0.5,0.5);
    }

    void CustomGraphicsView::fitInViewZoom()
    {
        QList<QGraphicsItem*> itemList = scene()->selectedItems();

        if ( itemList.isEmpty() )
        {
            itemList = items();
        }

        if ( itemList.isEmpty() )
        {
            return;
        }

        double minx = 9999;
        double miny = 9999;
        double maxx = -9999;
        double maxy = -9999;
        for ( int i=0 ; i<itemList.size() ; ++i )
        {
            QGraphicsItem * item = itemList[i];
            if ( item->isVisible() )
            {
                if ( minx > item->x() )
                {
                    minx = item->x();
                }

                if ( miny > item->y() )
                {
                    miny= item->y();
                }
                if ( maxx < item->x() + item->boundingRect().width() )
                {
                    maxx = item->x() + item->boundingRect().width();
                }

                if ( maxy < item->y() + item->boundingRect().height() )
                {
                    maxy= item->y() + item->boundingRect().height();
                }
            }
        }
        fitInView(minx,miny,maxx-minx,maxy-miny,Qt::KeepAspectRatio);
    }

    void CustomGraphicsView::resizeEvent ( QResizeEvent * event )
    {
        QGraphicsScene * s = scene();
        if ( s != 0 )
        {
            dynamic_cast<AbstractScene*>( s )->resize( event );
        }

        QGraphicsView::resizeEvent( event );
    }

}
