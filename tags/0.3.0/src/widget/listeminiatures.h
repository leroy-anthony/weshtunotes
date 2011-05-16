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

#ifndef LISTEMINIATURES_H
#define LISTEMINIATURES_H

#include <QMainWindow>
#include <QApplication>
#include <QListView>
#include <QDirModel>
#include <QObject>
#include <QMenu>
#include <QMouseEvent>

#include "imageitem.h"
#include "gestionthread.h"

/**
	@class ListeMiniatures
	@brief TODO description doxygen
	@author LEROY Anthony <leroy.anthony@gmail.com>
*/
class ListeMiniatures : public QListView
{
    Q_OBJECT

private:
	QProgressBar m_progressBar;
	
    gestionThread m_gestionnaireRendu;
    int m_tailleVignette;
    ImageItem * m_vignetteItem;

    void creationNouvelleVignette( int taille, bool avecNomFichier );
    void restartGestionnaireThread();


public:
    ListeMiniatures( QWidget * parent=0 );
    ~ListeMiniatures();

	void setModelMiniatures( ModelData * model );
    void setTailleVignette( int taille );

    const ImageItem * vignetteItem() const;

//     void mouseReleaseEvent ( QMouseEvent * event );
	
	QProgressBar & progressBar();
	void setValueProgressBar( int );

protected:
	void dropEvent(QDropEvent *e);

public slots:
    void trierDate();


};

#endif
