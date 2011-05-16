/*
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
#include <limits>
#include <QGraphicsRectItem>
#include <QScrollBar>

#include "AbstractScene.h"
#include "settings.h"
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
            m_indexItem(0),
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
        setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );
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
        setOptimizationFlags(QGraphicsView::DontSavePainterState);

        setRenderHint(QPainter::SmoothPixmapTransform, true);
        setRenderHint(QPainter::Antialiasing, true);
        setRenderHint(QPainter::TextAntialiasing, true);
        setRenderHint(QPainter::HighQualityAntialiasing, true);
        setRenderHint(QPainter::NonCosmeticDefaultPen, true);

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

        setViewportMargins(5,5,5,5);

        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

        setAcceptDrops(true);

        viewport()->setFocusPolicy(Qt::NoFocus);

        setFocusPolicy(Qt::StrongFocus);
    }

    void CustomGraphicsView::setScene( QGraphicsScene * scene )
    {
        adjustLayoutSize( size().width(), size().height() );

        QGraphicsView::setScene(scene);
    }

    void CustomGraphicsView::adjustLayoutSize()
    {
        adjustLayoutSize( size().width(), size().height() );
    }

    void CustomGraphicsView::adjustLayoutSize( int width, int height )
    {
        if ( scene() != 0 )
        {
            QGraphicsWidget * form = static_cast<Scene::AbstractScene*>(scene())->form();
            if ( form != 0 )
            {
                int w = viewport()->rect().width();

                QScrollBar * vscrollBar = verticalScrollBar();
                if ( vscrollBar != 0 )
                {
                    w -= (25 - vscrollBar->width());
                }

                QRectF r = scene()->sceneRect();
                r.setRect(0,0,w,INT_MAX);
                scene()->setSceneRect(r);

                form->resize( w+8, height );
            }
        }
    }

    void CustomGraphicsView::resizeEvent( QResizeEvent * event )
    {
        adjustLayoutSize( event->size().width(), event->size().height() );

        QGraphicsView::resizeEvent(event);
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
        if ( hasZoomAbilities() )
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
        else
        {
            QGraphicsView::wheelEvent(e);
        }
    }

    void CustomGraphicsView::paste()
    {
        if ( !m_copyHandlesItem.isEmpty() )
        {
            AbstractScene * s =  static_cast<AbstractScene*>( scene() );

            QStringList filesName;
            for ( int i=0 ; i<m_copyHandlesItem.size() ; ++i )
            {
                filesName << m_copyHandlesItem[i]->handleItem()->configFile();
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

            s->loadHandles( filesName, mapToScene( viewport()->width()/2, viewport()->height()/2 ), abs(maxX-minX), abs(maxY-minY) );
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

    void CustomGraphicsView::setScale( double scale )
    {
        QMatrix m =  matrix();
        m.setMatrix( scale, 0, 0, scale, matrix().dx(), matrix().dy() );
        setMatrix( m );
    }

    void CustomGraphicsView::timerEvent( QTimerEvent * ev )
    {
        Q_UNUSED( ev );

        if( qAbs(matrix().m11()-m_scale) < qAbs((matrix().m11()*(1+m_scaleStep))-m_scale) )
        {
            killTimer(m_idTimer);

            QMatrix m =  matrix();
            m.setMatrix( m_scale, 0, 0, m_scale, matrix().dx(), matrix().dy() );
            setMatrix( m );
        }
        else
        {
            scale( 1+m_scaleStep, 1+m_scaleStep );
        }
    }

    void CustomGraphicsView::resetZoom( bool smooth )
    {
        if ( hasZoomAbilities() )
        {
            if ( !smooth && !Settings::smoothZoom() )
            {
                setScale(1);
                return;
            }

            if ( matrix().m11() == 1 )
                return;

            m_idTimer = startTimer(10);

            m_scale = 1;

            if ( matrix().m11()>1 )
            {
                m_scaleStep = -0.05;
            }
            else
            {
                m_scaleStep = 0.05;
            }
        }
    }

    void CustomGraphicsView::doubleZoom( bool smooth )
    {
        if ( hasZoomAbilities() )
        {
            if ( !smooth && !Settings::smoothZoom() )
            {
                setScale(matrix().m11()*2);
                return;
            }

            m_idTimer = startTimer(10);

            m_scale     = matrix().m11()*2;
            m_scaleStep = 0.05;
        }
    }

    void CustomGraphicsView::halfZoom( bool smooth )
    {
        if ( hasZoomAbilities() )
        {
            if ( !smooth && !Settings::smoothZoom() )
            {
                setScale(matrix().m11()*0.5);
                return;
            }

            m_idTimer = startTimer(10);

            m_scale=matrix().m11()*0.5;
            m_scaleStep = -0.05;
        }
    }

    void CustomGraphicsView::centerZoom( bool smooth )
    {
        if ( hasZoomAbilities() )
        {
            fitInViewZoom();
            resetZoom( smooth );
        }
    }

    void CustomGraphicsView::fitInViewZoom()
    {
        if ( hasZoomAbilities() )
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

            double minx = std::numeric_limits<double>::infinity();
            double miny = std::numeric_limits<double>::infinity();
            double maxx = - std::numeric_limits<double>::infinity();
            double maxy = - std::numeric_limits<double>::infinity();

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
    }

    void CustomGraphicsView::nextItem()
    {
        prevOrNextItem( true );
    }

    void CustomGraphicsView::prevItem()
    {
        prevOrNextItem( false );
    }

    void CustomGraphicsView::prevOrNextItem( bool next )
    {
        QList<QGraphicsItem*> itemList = items();

        for ( int i=0 ; i<itemList.size() ; ++i )
        {
            if ( !itemList[i]->isVisible() )
            {
                itemList.removeAt(i);
                break;
            }
        }

        if ( itemList.isEmpty() )
        {
            return;
        }

        if ( m_indexItem < 0 )
        {
            m_indexItem = itemList.size()-1;
        }

        if ( m_indexItem >= itemList.size() )
        {
            m_indexItem = 0;
        }

        QGraphicsItem * item = itemList[m_indexItem];

        if ( next )
        {
            ++m_indexItem;
        }
        else
        {
            --m_indexItem;
        }

        scene()->clearSelection();
        item->setSelected(true);
        centerZoom();
    }

    bool CustomGraphicsView::hasZoomAbilities()
    {
        AbstractScene * s = static_cast<AbstractScene*>(scene());
        return s != 0 && s->hasZoomAbilities();
    }

}
