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

#ifndef GESTIONTHREAD_H
#define GESTIONTHREAD_H

#include <QObject>
#include <QListView>

#include <vector>
#include <iostream>

#include "renduthread.h"
#include "globalconfig.h"



class ListeMiniatures;

/*! \class QObject qt class */
class QObject;


/*! \class gestionThread
 *  \brief Gestion des threads utilisÃ©s pour le rendu des images miniatures.
 *  \author Anthony Leroy
 *  \version 0.1
 *  \date    2006
 *
 *  La classe gestionThread gÃ¨re les threads utilisÃ©s pour le rendu des images miniatures.
 *  Le nombre maximum de threads est crÃ©Ã©, dÃ¨s qu'un thread a fini sa tache, un nouveau est
 *  crÃ©Ã© et ainsi de suite... Le nombre de thread est dÃ©fini de facon statique par la variable
 *  m_nombreThreadMaxi.
 */
class gestionThread : public QObject
{
    Q_OBJECT

private:
    ListeMiniatures * m_listViewImage; /*!< Pointeur sur le widget QListView dont dÃ©pend m_model. */
    std::vector<ImageInfo> m_listeImage;  /*!< Liste des urls des images dont on doit crÃ©er des miniatures de taille m_tailleMiniature. */
    ModelData * m_model;   /*!< Pointeur sur le model utilisÃ© par m_listViewImage. */

    int m_nombreThread;     /*!< Nombre de threads utilisÃ©s. */
    int m_nombreThreadMaxi; /*!< Nombre de threads maximum utilisables. */
    int m_tailleMiniature;  /*!< Taille des miniatures Ã  crÃ©er. */

    QString m_path_thumb;


    int m_nombreMiniatures;

    /*! \fn void gestionThread::creerNouveauThread()
    *  \brief CrÃ©e un nouveau thread dans lequel les miniatures sont calculÃ©es.
    *  \return void
    */
    void creerNouveauThread();

public:
    /*! \fn gestionThread::gestionThread()
    *  \brief Constructeur
    */
    gestionThread();


    /*! \fn gestionThread::~gestionThread()
    *  \brief Destructeur
    */
    ~gestionThread();


    /*! \fn void gestionThread::reset()
    *  \brief Vide la liste de miniatures Ã  crÃ©er.
    *  \return void
    */
    void reset();


    /*! \fn void gestionThread::setListView( QListView * listViewImage )
    *  \brief DÃ©finit l'objet QlistView dont la classe dÃ©pend.
    *  \return void
    */
    void setListView( ListeMiniatures * listViewImage );


    /*! \fn void gestionThread::nombreThread()
    *  \brief Renvoie le nombre de threads en cours d'utilisation.
    *  \return int
    */
    int nombreThread();


    /*! \fn void gestionThread::start()
    *  \brief DÃ©marre le gestionnaire et commence Ã  gÃ©nÃ©rer les threads en fonction des urls contenues dans la liste m_listeImage
    *  \return void
    */
    void start();

    const QString & pathThumb() const;
    void setPathThumb( QString path );


private slots:
    /*! \fn void gestionThread::detruireThread( RenduThread * renduThread )
    *  \brief Lorsqu'un thread de type RenduThread a fini sa tache, il appelle cette fonction
    *  \param RenduThread : un objet RenduThread
    *  \return void
    */
    void detruireThread();

};

#endif
