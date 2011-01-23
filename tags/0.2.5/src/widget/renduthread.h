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

#ifndef RENDUTHREAD_H
#define RENDUTHREAD_H

#include <QObject>
#include <QFileInfo>
#include <QImage>
#include <QString>
#include <QThread>
#include <QDir>
#include <QImageReader>

#include <iostream>

#include "modeldata.h"



/*! \class QThread qt class */


/**
	@class RenduThread
	@brief TODO description doxygen
	@author LEROY Anthony <leroy.anthony@gmail.com>
*/
class RenduThread : public QThread
{
	Q_OBJECT

private:

	QString m_absoluteFilePath;
	int m_tailleMiniature;
	int m_index;

	const QString & m_path_thumb;


public:
	RenduThread( const QString &, int, int, const QString & );
	~RenduThread();

	void run();
	int index();


signals:
	void fini();

};

#endif
