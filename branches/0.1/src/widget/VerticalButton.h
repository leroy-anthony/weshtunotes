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

#ifndef VERTICALBUTTON_H
#define VERTICALBUTTON_H

#include <QPushButton>
#include <QRect>
#include <QPaintEvent>
#include <QStylePainter>
#include <QStyleOption>
#include <QStyle>
#include <QPainter>
#include <iostream>

namespace Widget
{

/**
	@class VerticalButton
	@brief TODO description doxygen
	@author LEROY Anthony <leroy.anthony@gmail.com>
*/
class VerticalButton : public QPushButton
{
    Q_OBJECT

private:
    bool eventFilter(QObject *obj, QEvent *event);
	
	int pointeurSouris;
	int clickSouris;
	int red;
	int green;
	int blue;
	int h;
	int s;
	float v;
	float dv;
	QColor couleurSelection;
	
	int xBoundingRect;
	int yBoundingRect;
	
	QPen pen;

	static int _staticId;
	int _id;

public:
    VerticalButton(QWidget *parent = 0);
    ~VerticalButton();

    void paintEvent( QPaintEvent * event );
	void deSelectionner();
	void selectionner();
	void setText( const QString & text );
};

}

#endif
