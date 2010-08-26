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

#include "gestionthread.h"
#include "listeminiatures.h"
#include "debug.h"
#include <QSortFilterProxyModel>

gestionThread::gestionThread():
		QObject(),
		m_listViewImage(0),
		m_nombreThread(0),
		m_nombreThreadMaxi(4),
		m_tailleMiniature(64),
		m_path_thumb(GlobalConfig::baseCacheDir()),
		m_nombreMiniatures(0)
{}


gestionThread::~gestionThread()
{
	m_listViewImage = 0;
}


void gestionThread::setListView( ListeMiniatures * listViewImage )
{
	m_listViewImage = listViewImage;
}


void gestionThread::reset()
{
	m_listeImage.clear();
}


void gestionThread::start()
{
	if ( m_listViewImage != NULL )
	{
		m_model = dynamic_cast<ModelData*>(m_listViewImage->model());

		if ( m_model != NULL )
		{
			//fixme j'aime pas ça... ca dépend pas du modèle mais du média, mainWindow
			//indique à view où mettre ses miniatures d'après les dires de média
			m_path_thumb = m_model->baseThumb();
			m_tailleMiniature = m_model->tailleImage();
			m_listeImage = m_model->listeImages();

			m_nombreMiniatures = m_listeImage.size();

			while ( (!m_listeImage.empty()) && (m_nombreThread<m_nombreThreadMaxi) )
			{
				creerNouveauThread();
			}
		}
		else
		{
			std::cout << "Erreur model est NULL" << std::endl;
		}
	}
	else
	{
		std::cout << "Erreur m_listViewImage est NULL" << std::endl;
	}
}


void gestionThread::creerNouveauThread()
{
	int index = m_nombreMiniatures - m_listeImage.size();

	QString tailleMiniature;
	tailleMiniature.setNum(  m_tailleMiniature );

	//on cherche s'il ne manque une miniature. Si on la trouve, on la crée...
	bool newThread = false;
	while ( !m_listeImage.empty() && (!newThread))
	{
		const ImageInfo & imageInfo = m_listeImage.front();
		if ( imageInfo.isImage() )
		{
			QString nomFichier( "/Thumb_" + ImageInfo::pathEncoder(imageInfo.absoluteFilePath()) );

			if ( !QFile::exists(m_path_thumb + tailleMiniature + nomFichier) )
			{
				RenduThread * t = new RenduThread( imageInfo.absoluteFilePath(), index, m_tailleMiniature, m_path_thumb );
				connect( t, SIGNAL(fini()), this, SLOT(detruireThread()) );
				t->start();
				++m_nombreThread;
				newThread = true;
			}
		}

		m_listeImage.erase(m_listeImage.begin());
		m_listViewImage->setValueProgressBar( m_listeImage.size() );
	}
}


void gestionThread::detruireThread()
{
	RenduThread * rt = dynamic_cast<RenduThread*>(sender());
	
	/*! Une miniature a été créée, donc on met à jour le widget m_listViewImage */
	//m_listViewImage->update();
	dynamic_cast<ModelData*>(m_listViewImage->model())->updateMiniature(rt->index());

	/*! On d"truit ce thread car il a termin" son travail et on met à jour le compteur (m_nombreThread). */
	delete( rt );
	--m_nombreThread;

	/*! Si la liste d'images n'est pas vide, on crée un nouveau thread. */
	if (  !m_listeImage.empty() )
		creerNouveauThread();
}


int gestionThread::nombreThread()
{
	return m_nombreThread;
}


const QString & gestionThread::pathThumb() const
{
	return m_path_thumb;
}

void gestionThread::setPathThumb( QString path )
{
	m_path_thumb = path;
}

