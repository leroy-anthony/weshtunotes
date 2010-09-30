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

#include "AnimationManager.h"

#include <QPropertyAnimation>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsWidget>
#include <QParallelAnimationGroup>

#include <settings.h>

namespace Animation
{

    QGraphicsView * AnimationManager::m_view = 0;

    void AnimationManager::initAnimation( QGraphicsView * view )
    {
        AnimationManager a(view);
    }

    AnimationManager::AnimationManager( QGraphicsView * view )
    {
        m_view = view;
    }

    void AnimationManager::startLoad( const QList<QGraphicsItem*> & graphicsWidgets )
    {
        for ( int i=0 ; i<graphicsWidgets.size() ; ++i )
        {
            startLoad( static_cast<QGraphicsWidget*>(graphicsWidgets[i]) );
        }
    }

    void AnimationManager::startLoad( QGraphicsWidget * graphicsWidget )
    {
        QPropertyAnimation * animation = new QPropertyAnimation( graphicsWidget, "pos" );

        animation->setDuration( Settings::loadDuration() );

        QPointF centerScene = m_view->mapToScene( m_view->viewport()->width()/2, m_view->viewport()->height()/2 );
        centerScene.setX( centerScene.x() - graphicsWidget->size().width()/2 );
        animation->setStartValue( centerScene );

        animation->setEndValue( QPointF(graphicsWidget->pos()) );
        animation->setEasingCurve( QEasingCurve::OutExpo );

        animation->start();
    }

    QAbstractAnimation * AnimationManager::startCreate( QGraphicsWidget * graphicsWidget )
    {
        QPropertyAnimation * animation1 = new QPropertyAnimation( graphicsWidget, "scale" );
        animation1->setDuration( Settings::createDuration() );
        animation1->setStartValue( 0 );
        animation1->setEndValue( 1 );
        animation1->setEasingCurve( QEasingCurve::OutElastic );

        QPropertyAnimation * animation2 = new QPropertyAnimation( graphicsWidget, "opacity" );
        animation2->setDuration( Settings::createDuration() );
        animation2->setStartValue( 0 );
        animation2->setEndValue( 1 );
        animation2->setEasingCurve( QEasingCurve::OutCubic );

        QParallelAnimationGroup * group = new QParallelAnimationGroup;
        group->addAnimation(animation1);
        group->addAnimation(animation2);
        group->start();

        return group;
    }

    QAbstractAnimation * AnimationManager::startDelete( QGraphicsWidget * graphicsWidget )
    {
        QPropertyAnimation * animation1 = new QPropertyAnimation( graphicsWidget, "opacity" );
        animation1->setDuration( Settings::deleteDuration() );
        animation1->setStartValue( 1 );
        animation1->setEndValue( 0 );
        animation1->setEasingCurve( QEasingCurve::Linear );

        QPropertyAnimation * animation2 = new QPropertyAnimation( graphicsWidget, "scale" );
        animation2->setDuration( Settings::deleteDuration() );
        animation2->setStartValue( 1 );
        animation2->setEndValue( 0 );
        animation2->setEasingCurve( QEasingCurve::Linear );

        QPropertyAnimation * animation3 = new QPropertyAnimation( graphicsWidget, "pos" );
        animation3->setDuration( Settings::deleteDuration() );
        animation3->setStartValue( graphicsWidget->pos() );
        animation3->setEndValue( graphicsWidget->pos() + QPointF(graphicsWidget->size().width()/2,graphicsWidget->size().height()/2) );
        animation3->setEasingCurve( QEasingCurve::Linear );

        QParallelAnimationGroup * group = new QParallelAnimationGroup;
        group->addAnimation(animation1);
        group->addAnimation(animation2);
        group->addAnimation(animation3);
        group->start();

        return group;
    }

}
