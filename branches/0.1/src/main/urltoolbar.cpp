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

#include "urltoolbar.h"
#include "debug.h"

UrlToolBar::UrlToolBar(QWidget *parent)
 : QToolBar(parent)
{

	setupWidget();

	addWidget( widgetLayout );

	connect( urlComboBox, SIGNAL(activated(const QString & )), this, SLOT(activated(const QString & )) );
}


UrlToolBar::~UrlToolBar()
{
}

void UrlToolBar::setupWidget()
{
    widgetLayout = new QWidget(this);
    widgetLayout->setObjectName(QString::fromUtf8("widgetLayout"));

    hboxLayout = new QHBoxLayout(widgetLayout);
    hboxLayout->setSpacing(3);
    hboxLayout->setMargin(1);
    hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));

    prevToolButton = new QToolButton(widgetLayout);
	GlobalConfig::setupToolBarButton(prevToolButton);
    prevToolButton->setObjectName(QString::fromUtf8("prevToolButton"));
	prevToolButton->setIcon( GlobalConfig::baseIcon("back.png") );
    hboxLayout->addWidget(prevToolButton);
	connect( prevToolButton, SIGNAL(clicked()), this, SLOT(backward()) );

    nextToolButton = new QToolButton(widgetLayout);
	GlobalConfig::setupToolBarButton(nextToolButton);
    nextToolButton->setObjectName(QString::fromUtf8("nextToolButton"));
	nextToolButton->setIcon( GlobalConfig::baseIcon("forward.png") );
    hboxLayout->addWidget(nextToolButton);
	connect( nextToolButton, SIGNAL(clicked()), this, SLOT(forward()) );

    upToolButton = new QToolButton(widgetLayout);
	GlobalConfig::setupToolBarButton(upToolButton);
    upToolButton->setObjectName(QString::fromUtf8("upToolButton"));
	upToolButton->setIcon( GlobalConfig::baseIcon("up.png") );
    hboxLayout->addWidget(upToolButton);
	connect( upToolButton, SIGNAL(clicked()), this, SLOT(up()) );

    updateToolButton = new QToolButton(widgetLayout);
	GlobalConfig::setupToolBarButton(updateToolButton);
    updateToolButton->setObjectName(QString::fromUtf8("updateToolButton"));
	updateToolButton->setIcon( GlobalConfig::baseIcon("reload.png") );    hboxLayout->addWidget(updateToolButton);

	urlComboBox = new QComboBox(widgetLayout);
	urlComboBox->setEditable( true );
	urlComboBox->setObjectName(QString::fromUtf8("urlComboBox"));
	urlComboBox->setSizePolicy( QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed) );
	hboxLayout->addWidget(urlComboBox);

}


void UrlToolBar::activated(const QString & text)
{
	emit urlSend(text);
}


void UrlToolBar::setUrl( const QString & text )
{
	urlComboBox->setEditText( text );
	int index = urlComboBox->findText( text );
	if ( index != -1 )
		urlComboBox->removeItem( index );
	urlComboBox->addItem( text );
}


void UrlToolBar::up()
{
	QString url = urlComboBox->currentText();
	activated( url+"/.." );
}

void UrlToolBar::forward()
{
	int forward = urlComboBox->currentIndex();
	if ( (forward+1) >= 0 )
		activated(  urlComboBox->itemText(forward+1)  );
}

void UrlToolBar::backward()
{
	int back = urlComboBox->currentIndex();
//	if ( (back-1) >= 0 )
		activated(  urlComboBox->itemText(urlComboBox->count()-2)  );
}




