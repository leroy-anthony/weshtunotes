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

#include "renduthread.h"
#include "debug.h"

#include <image.hpp>
#include <exif.hpp>
#include <iostream>
#include <iomanip>
#include <cassert>

RenduThread::RenduThread( const QString & absoluteFilePath, int index, int taille, const QString & path_thumb ) :
	QThread(),
	m_absoluteFilePath(absoluteFilePath),
	m_tailleMiniature(taille),
	m_index(index),
	m_path_thumb(path_thumb)
{}


RenduThread::~RenduThread()
{
	wait();
}


void RenduThread::run()
{
	QString nb = QString::number( m_tailleMiniature );
	if ( !QFile::exists( m_path_thumb+nb ) )
		QDir( m_path_thumb ).mkdir( nb );

	QImage image;
	
	// On essaie de charger la miniature à partir des meta-données
	Exiv2::Image::AutoPtr imageExiv;
	try 
	{
		imageExiv = Exiv2::ImageFactory::open( m_absoluteFilePath.toStdString() );
		imageExiv->readMetadata();
		if ( !imageExiv->exifData().empty() )
		{
			Exiv2::DataBuf thumbnail = imageExiv->exifData().copyThumbnail();
			image.loadFromData(thumbnail.pData_, thumbnail.size_);
		}
	} 
	catch (Exiv2::Error&)
	{
	}

	// En cas d'echec on reviens à la bonne vieille méthode
	if ( image.isNull() )
		image.load( m_absoluteFilePath );

	if ( !image.isNull() )
	{
		//fixme fonction detransformation du path en nom de fichier à factoriser dans la classe imageinfo
		QString nomFichier( "/Thumb_" + ImageInfo::pathEncoder(m_absoluteFilePath) );//.replace('/',"_") + ".JPG" );
		image.scaled( QSize(300,300), Qt::KeepAspectRatio ).save( m_path_thumb + nb + nomFichier + "LARGE", "PNG", -1 );
 		image.scaled( QSize(m_tailleMiniature,m_tailleMiniature), Qt::KeepAspectRatio ).save( m_path_thumb + nb + nomFichier, "PNG", -1 );
	}

	emit fini();
}


int RenduThread::index()
{
	return m_index;
}




