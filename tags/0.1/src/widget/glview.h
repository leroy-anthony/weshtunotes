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

#ifndef GLVIEW_H
#define GLVIEW_H

#define BUFSIZE 512

#include <iostream>
#include <map>
#include <vector>

#include <QGLWidget>
#include "modeldata.h"
#include "presentationthumbgl.h"

class PresentationThumbGL;

/**
	@author kahal <kahal@tamriel>
*/
class GLView : public QGLWidget
{
    Q_OBJECT
			
public:
    GLView( QWidget *parent = 0 );
    ~GLView();

    void zoom_in();
    void zoom_out();
    void set_center_view( int x, int y );

    bool clicOnSelectObject( int x, int y );
    void setMultipleSelection( int, int, int, int );
    void setSimpleSelection( int, int );
    int countSelectedObject();
    void clearSelection();

    void setDeplacement( int dx, int dy );

    void moveHorizontal( int dx );
    void moveVertical( int dy );
	
	void makeImage();
	void init();

protected:

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:

    /*1*/
    void beginSelectionRender( GLuint * selectBuf, GLint * viewport );
    /*2*/
    void multipleSelectionRender();
    /*2*/
    void simpleSelectionRender( int, int );
    /*3*/
    GLint endSelectionRender();
    /*4*/
    void processSimpleHits( GLint hits, GLuint buffer[] );
    /*4*/
    void processMultipleHits( GLint hits, GLuint buffer[] );

    //     void select_Render();
    void main_Render( GLenum mode );


    //--zoom et recentrage--//
    float m_zoom;
    int m_position_X;
    int m_position_Y;
    //----------------------//


    void drawSelection();
    bool selection;
    int x1, y1, x2, y2;

//     std::map<int,GLObject> m_map_objects;
    std::map<int,int>      m_map_objSelected;
    int m_mode_selection; // 0 click, 1 lasso
    bool m_mode_move; // 0 don'tmove
	
	PresentationThumbGL * m_presentationThumbGL;
	std::vector<GLuint> m_vect_pix;

};

#endif
