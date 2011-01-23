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

#include "imageitem.h"
#include "debug.h"

ImageItem::ImageItem(QObject *parent, int taille, bool avecNomFichier, bool uniform ):
		QAbstractItemDelegate(parent),
		m_vignetteTaille(taille+10),
		m_decal(5),
		m_nomFichier_nbCharMaxParLigne(0),
		m_avecNomFichier(avecNomFichier),
		m_uniform(uniform),
		m_penBlack(QColor( 0, 0, 0 )),
		m_penRed(QColor( 255, 0, 0 )),
		m_fontMetrics(QFont())
{
	m_svgIcons.load(QString("/home/kahal/Programmation/C++/imgka/kweshtavue/Icons/dessin2.svg"));
}


int ImageItem::tailleNomFichierY( const QString & nomFichier, int nbCharMaxParLigne ) const
{
	return ( (float)nomFichier.size() / (float)nbCharMaxParLigne + 1 )*m_fontMetrics.height();
}

int ImageItem::tailleNomFichierX( int taille ) const
{
	return taille / (float) m_fontMetrics.width( 'M' ) + 3;
}

void ImageItem::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QVariant v =  index.model()->data(index, 0);
	QString str = qVariantValue<QString>( v );

	QVariant v2 =  index.model()->data(index, 1);
	QString nomFichier = qVariantValue<QString>( v2 );

	int nbCharMaxParLigne = tailleNomFichierX( option.rect.width() );

	int nomFichier_espacementY = tailleNomFichierY( nomFichier, nbCharMaxParLigne );


	int hauteurVignette = option.rect.height()-m_decal-nomFichier_espacementY;
	int largeurVignette = option.rect.width()-m_decal;

	QRect rectSVG( option.rect.x(), option.rect.y(), option.rect.width(), hauteurVignette );
	QRect rectForegroundSVG( option.rect.x(), option.rect.y(), largeurVignette, hauteurVignette );
	QRect rectBackgroundSVG( option.rect.x()+m_decal, option.rect.y()+m_decal, largeurVignette, hauteurVignette );


	QSvgRenderer * svgIcons = m_svgIcons.renderer();

	if ( str == "REPERTOIRE" )
	{
		if ( option.state & QStyle::State_Selected )
			svgIcons->render( painter, QString("folderSelection"), rectSVG );

		svgIcons->render( painter, QString("folder"), rectSVG );
	}
	else
	{
		int indexSelection = dynamic_cast<const ModelData*>(index.model())->imageSelected();
		
		if ( index.row() == indexSelection )
			str = str + "LARGE";

		QImage image(str);

// 		const QImage & image = ((ModelData*)index.model())->imageAt(index.row());
		
		//si l'image est null on choisit l'image de chargement
// 		if ( image.isNull() )
// 			if ( !m_uniform )
// 				image = m_iconFailed.scaled( 5, 5 );
// 			else
// 				image = m_iconFailed;
		
		if ( option.state & QStyle::State_Selected )
		{
			svgIcons->render( painter, QString("cadreBackgroundSelection"), rectBackgroundSVG );
			svgIcons->render( painter, QString("cadreForegroundSelection"), rectForegroundSVG );
		}
		else
		{
			svgIcons->render( painter, QString("cadreBackground"), rectBackgroundSVG );
			svgIcons->render( painter, QString("cadreForeground"), rectForegroundSVG );
		}

		QSize imageSize = image.size();
		
// 		int y = 0;
// 		if ( imageSize.rheight() < m_vignetteTaille )
// 			y = (m_vignetteTaille - imageSize.rheight())/2;
// 
// 		int x = 0;
// 		if ( imageSize.rwidth() < m_vignetteTaille )
// 			x = (m_vignetteTaille - imageSize.rwidth())/2;

		int y = 0;
		if ( imageSize.rheight() < (option.rect.height() - m_decal ))
			y = (hauteurVignette - imageSize.rheight())/2;

		int x = 0;
		if ( imageSize.rwidth() < (option.rect.width() - m_decal ) )
			x = (largeurVignette - imageSize.rwidth())/2;

		QRect r( option.rect.x()+x, option.rect.y()+y, imageSize.rwidth(), imageSize.rheight() );
		painter->drawImage( r, image );
	}
	
	if (m_avecNomFichier)
	{
		QRect rt( option.rect.x(), option.rect.y() + hauteurVignette + m_decal, option.rect.width(), nomFichier_espacementY );
		//QRect rt( rectX, rectY + option.rect.height() - nomFichier_espacementY + m_decal, m_vignetteTaille, nomFichier_espacementY );
		QTextOption text( Qt::AlignHCenter );
		text.setWrapMode( QTextOption::WrapAnywhere );
		painter->drawText ( rt, nomFichier, text );
	}
}

QSize ImageItem::sizeHint(const QStyleOptionViewItem &  option , const QModelIndex & index ) const
{
	const ImageInfo & img = dynamic_cast<const ModelData*>(index.model())->imageInfo(index.row());
	int nomFichier_espacementY = tailleNomFichierY( img.name(), tailleNomFichierX( m_vignetteTaille + m_decal ) );

	if ( img.isImage() )
	{
		int indexSelection = dynamic_cast<const ModelData*>(index.model())->imageSelected();
		if ( index.row() == indexSelection )
			return QSize(350,350);
	}

	if ( !m_uniform )
	{

		if ( !img.isImage() )
			return QSize(m_vignetteTaille, m_vignetteTaille+nomFichier_espacementY );

		QSize tailleImage( img.width(), img.height() );
		tailleImage.scale(m_vignetteTaille, m_vignetteTaille, Qt::KeepAspectRatio);

		int nomFichier_espacementY2 = tailleNomFichierY( img.name(), tailleNomFichierX( tailleImage.width() ) + m_decal );
		tailleImage.setHeight( tailleImage.height() + nomFichier_espacementY2+ m_decal );

		return tailleImage;
	}

	return QSize( m_vignetteTaille + m_decal, m_vignetteTaille + m_decal + nomFichier_espacementY );
}

int ImageItem::getVignetteTaille() const
{
	return m_vignetteTaille + m_decal;
}


