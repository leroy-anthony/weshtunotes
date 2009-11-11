/*
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

#ifndef URLTOOLBAR_H
#define URLTOOLBAR_H

#include <QToolBar>
#include <QtGui>

//#include "../config/globalconfig.h"


/**
        @author LEROY Anthony <leroy.anthony@gmail.com>
*/
class UrlToolBar : public QToolBar
{

    QWidget * widgetLayout;
    QHBoxLayout * hboxLayout;
    QToolButton * prevToolButton;
    QToolButton * nextToolButton;
    QToolButton * upToolButton;
    QToolButton * updateToolButton;
    QComboBox   * urlComboBox;

    std::vector<QString> m_historiqueUrl;

    Q_OBJECT

public:
    UrlToolBar(QWidget *parent = 0);
    ~UrlToolBar();

    void setupWidget();
    void setUrl( const QString & text );

signals:
    void urlSend( const QString & text );

public slots:
    void activated(const QString & text);
    void up();
    void forward();
    void backward();

};

#endif
