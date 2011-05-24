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

#include "listeminiatures.h"
#include "debug.h"

/**
 * @brief Constructeur
 * @param QWidget * parent
 */
ListeMiniatures::ListeMiniatures( QWidget * parent ):
		QListView( parent ),
		m_tailleVignette(64),
		m_vignetteItem(0)
{
	creationNouvelleVignette( m_tailleVignette, true );


	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setDragEnabled(true);
	setAcceptDrops(true);
	//setDropIndicatorShown(true);


	setViewMode( QListView::IconMode );
	setSpacing( 5 );
	//     setGridSize( QSize(m_tailleVignette*2,m_tailleVignette*2) );
	setUniformItemSizes ( false );


	m_progressBar.setFixedHeight( 20 );


	m_gestionnaireRendu.setListView( this );
}


/**
 * @brief Destructeur
 */
ListeMiniatures::~ListeMiniatures()
{
	if ( m_vignetteItem != 0)
		delete m_vignetteItem;
}



/**
 * @brief Trier les images par leur date de création
 */
void ListeMiniatures::trierDate()
{
	dynamic_cast<ModelData*>(model())->trierDate();
}

// void ListeMiniatures::mouseReleaseEvent ( QMouseEvent * event )
// {
// 	if ( event->button() == 1 )
// 	{
// 		if( model() != 0 )
// 			if ( selectionModel() != 0 )
// 				if ( selectionModel()->selectedIndexes().size() > 0 )
// 				{
// 					((ModelData*)model())->setImageSelected( selectionModel()->selectedIndexes().first().row() );
// 					clearSelection();
// 				}
// 	}
// 	else if ( event->button() == 2 )
// 	{
// 		setCurrentIndex(  indexAt(event->pos()) );
// 	}
//
// 	QObject::eventFilter(this, event);
// }

/**
 * @brief Définit le modèle de données
 * @param model 
 */
void ListeMiniatures::setModelMiniatures( ModelData * model )
{
	qDebug() << model;
	m_tailleVignette = model->tailleImage();
	setModel( model );

	restartGestionnaireThread();
}


/**
 * @brief Redémarre le gestionnaire de threads
 */
void ListeMiniatures::restartGestionnaireThread()
{
	m_progressBar.setVisible( true );
	m_progressBar.setValue( 0 );
	m_progressBar.setRange( 0, ((ModelData*)model())->size() );

	//---
	//        BIDOUILLE ?
	//---
	if ( m_gestionnaireRendu.nombreThread() != 0 )
	{
		// Il y a encore des threads actifs...

		// on vide la liste de threads à créer
		m_gestionnaireRendu.reset();

		// on patiente que les derniers threads finissent...
		while ( m_gestionnaireRendu.nombreThread() > 0 )
		{
			// dis au reste du monde de ne pas l'attendre
			QApplication::processEvents( /*QEventLoop::ExcludeUserInputEvents*/ );
		}
	}

	m_gestionnaireRendu.start();
}


/**
 * @brief Création d'une nouvelle type de vignette
 * @param taille taille de la nouvelle vignette
 * @param avecNomFichier affichage du nom de la vignette
 */
void ListeMiniatures::creationNouvelleVignette( int taille, bool avecNomFichier )
{
	ImageItem * vignetteItemOld = m_vignetteItem;
	m_vignetteItem = new ImageItem( this, taille, avecNomFichier, true  );


	this->setItemDelegate( m_vignetteItem );

	if ( vignetteItemOld != 0 )
	{
		delete vignetteItemOld;
		vignetteItemOld = 0;
	}
}


/**
 * @brief Redéfinition de la taille des vignettes
 * @param taille 
 */
void ListeMiniatures::setTailleVignette( int taille )
{
	ModelData * m = (ModelData*) model();
	m->updateTaille( taille );

	m_tailleVignette = taille;
	creationNouvelleVignette( m_tailleVignette, true );

	reset();
	restartGestionnaireThread();
}



/**
 * @brief Renvoie la vignette courante
 * @return const ImageItem *
 */
const ImageItem * ListeMiniatures::vignetteItem() const
{
	return m_vignetteItem;
}


/**
 * @brief Renvoie la barre de progression
 * @return QProgressBar *
 */
QProgressBar & ListeMiniatures::progressBar()
{
	return m_progressBar;
}


/**
 * @brief Définit la valeur de la barre de progression
 * @param value Valeur de la barre de progression
 */
void ListeMiniatures::setValueProgressBar( int value )
{
	m_progressBar.setValue( dynamic_cast<ModelData*>(model())->size() - value );
	if (m_progressBar.value() == m_progressBar.maximum() )
		m_progressBar.setVisible( false );
}

void ListeMiniatures::dropEvent(QDropEvent *e)
{
	QAbstractItemView::dropEvent(e);
	//QListView::dropEvent(e);

}


