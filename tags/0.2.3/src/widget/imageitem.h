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

#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <map>
		

#include <QAbstractItemDelegate>
#include <QFontMetrics>
#include <QModelIndex>
#include <QSize>
#include <QVariant>
#include <QPainter>
#include <QTime>


#include <QtSvg>


#include "imageinfo.h"
#include "modeldata.h"



/**
	@class ImageItem
	@brief TODO description doxygen
	@author LEROY Anthony <leroy.anthony@gmail.com>
 */

class ImageItem : public QAbstractItemDelegate
{
	Q_OBJECT

public:
	ImageItem(QObject *parent = 0, int taille = -1, bool avecNomFichier = true, bool uniform = true );

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index ) const;

	int getVignetteTaille() const;


private:

	QSvgWidget m_svgIcons;
	
	QPen m_penBlack;
	QPen m_penRed;
	
	QFontMetrics m_fontMetrics;
	
	int m_vignetteTaille;
	int m_decal;

	int m_nomFichier_nbCharMaxParLigne;
	bool m_avecNomFichier;
	bool m_uniform;

	int tailleNomFichierY( const QString & nomFichier, int taille ) const;
	int tailleNomFichierX( int taille ) const;
};

#endif
