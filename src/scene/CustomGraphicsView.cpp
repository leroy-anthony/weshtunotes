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

#include <QMatrix>
#include <QPoint>
#include <QWheelEvent>
#include <QClipboard>
#include <QMimeData>
#include <QApplication>
#include <QGLWidget>

#include "AbstractScene.h"
#include "../main/general.h"

namespace Scene
{

    CustomGraphicsView::CustomGraphicsView():
            QGraphicsView(),
            m_scale(1.0),
            m_move(false)
    {
        QGLFormat format(QGL::SampleBuffers);
        setViewport(new QGLWidget(format));
        /*
      QGraphicsView::FullViewportUpdate
      QGraphicsView::MinimalViewportUpdate
      QGraphicsView::SmartViewportUpdate
      QGraphicsView::BoundingRectViewportUpdate
      QGraphicsView::NoViewportUpdate
      */
        setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
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
        /*
      QPainter::Antialiasing
      QPainter::TextAntialiasing
      QPainter::SmoothPixmapTransform
      QPainter::HighQualityAntialiasing
      QPainter::NonCosmeticDefaultPen
      */
        //m_view->setRenderHints(QPainter::NonCosmeticDefaultPen | QPainter::HighQualityAntialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
        setRenderHint(QPainter::Antialiasing, true);
        setRenderHint(QPainter::TextAntialiasing, true);
        setRenderHint(QPainter::HighQualityAntialiasing, false);

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);//FullViewportUpdate);
        setSceneRect(-100000,-100000,200000,200000);
        setViewportMargins(4,4,4,4);

        setAcceptDrops(true);
    }

    void CustomGraphicsView::mousePressEvent(QMouseEvent * event)
    {
        if ( scene()->itemAt( mapToScene( event->posF().x(), event->posF().y() ) ) == 0 )
        {
            m_move = true;
            m_position = event->posF();
        }
        QGraphicsView::mousePressEvent(event);
    }

    void CustomGraphicsView::mouseReleaseEvent(QMouseEvent * event)
    {
        m_move = false;
        QGraphicsView::mouseReleaseEvent(event);
    }

    void CustomGraphicsView::mouseMoveEvent(QMouseEvent * event)
    {
        if ( m_move )
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
        AbstractScene * s =  dynamic_cast<AbstractScene*>( scene() );

        const QClipboard * clipboard = QApplication::clipboard();
        const QMimeData * mimeData = clipboard->mimeData();

        s->addData( mimeData );
    }

}
