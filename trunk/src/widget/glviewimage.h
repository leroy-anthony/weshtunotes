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

#ifndef GLVIEWIMAGE_H
#define GLVIEWIMAGE_H

#include <QWidget>
#include <QGLWidget>
#include <QPixmap>
#include "abstractpresentation.h"

/**
	@author kahal <kahal@tamriel>
*/
class GLViewImage : public QGLWidget
{

    Q_OBJECT

private:
	//--zoom et recentrage--//
	float m_zoom;
	int m_position_X;
	int m_position_Y;
	//----------------------//
	
	GLuint m_idTexture;
	GLuint m_idList;
	
	std::vector<GLuint> m_vect_idList;
	std::vector<GLuint> m_idTextures;
	int m_choix;
	
	abstractPresentation * m_abstractPresentation;
	
	void mainRender( GLenum mode );


protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();


public:
	GLViewImage( QWidget *parent );
    ~GLViewImage();
	
	void zoomIn();
	void zoomOut();
	void setCenterView( int x, int y );
	void setView( int x, int y );
	
	int x() { return m_position_X; };
	int y() { return m_position_Y; };
	
	int m_xSave;
	int m_ySave;
	void saveState() { m_xSave = m_position_X; m_ySave = m_position_Y; };

	void loadImage( QPixmap *, int );
	void setIdList( int );
	void reset( int );
};

#endif
