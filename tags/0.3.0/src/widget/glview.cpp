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

#include "glview.h"

GLView::GLView( QWidget *parent ) : QGLWidget(parent)
{
    m_zoom       = 0;
    m_position_X = 0;
    m_position_Y = 0;


    // 	GLObject obj1;
    // 	obj1.addPoint(  50,  50 );
    // 	obj1.addPoint(  50, 100 );
    // 	obj1.addPoint( 100, 100 );
    // 	obj1.addPoint( 100,  50 );
    //
    // 	GLObject obj2;
    // 	obj2.addPoint( 200, 200 );
    // 	obj2.addPoint( 200, 300 );
    // 	obj2.addPoint( 300, 300 );
    // 	obj2.addPoint( 300, 200 );
    //
    // 	m_map_objects[ obj1.id() ] = obj1;
    // 	m_map_objects[ obj2.id() ] = obj2;

    m_mode_move = false;

    m_presentationThumbGL = (PresentationThumbGL*)parent;
}


GLView::~GLView()
{}

void GLView::init()
{
	initializeGL();
}

void GLView::initializeGL()
{
    // Set up the rendering context, define display lists etc.:
    glClearColor(0.0, 0.0, 0.0, 0.0);

    //glShadeModel( GL_SMOOTH );
    //glPolygonMode( GL_FRONT_AND_BACK, GL_POLYGON );
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    //initialisation of projection matrix
    glViewport( 0, 0, width(), height() );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    glOrtho( 0, width(), 0, height(), -5, 5);

    glPointSize(2.0);
    glLineWidth( 2.0 );
    glEnable(GL_LINE_STIPPLE);
    glLineStipple( 1.0, 0x0fff );
	
	
	makeImage();

}


void GLView::resizeGL(int w, int h)
{
    // setup viewport, projection etc.:
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    glOrtho( 0, width(), 0, height(), -5, 5);
}


void GLView::paintGL()
{
    //     select_Render();
    main_Render(GL_RENDER);
}


void GLView::main_Render( GLenum mode )
{
    // clean the scene
    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );

    //zoom and center view
    glLoadIdentity();
    glTranslatef( -width()*m_zoom/2 , -height()*m_zoom/2 , 1 );
    glScalef( 1+m_zoom, 1+m_zoom, 1 );
    glTranslatef( m_position_X, m_position_Y, 0 );



	for ( int i=0 ; i<m_vect_pix.size() ; i++ )
    {
		glCallList(m_vect_pix[i]);
    }


    // 	std::map<int,GLObject>::iterator it = m_map_objects.begin();
    // 	for ( it = m_map_objects.begin() ; it != m_map_objects.end() ; it++ )
    // 	{
    // 		it->second.draw( 1.0, 1.0, 1.0 );
    // 		if ( m_map_objSelected[it->second.id()] == 1 )
    // 		{
    // 			glLineWidth( 2.0 );
    // 			glEnable(GL_LINE_STIPPLE);
    // 			glLineStipple( 1.0, 0x0fff );
    // 			it->second.drawContour( 1.0, 0.0, 0.0 );
    // 		}
    // 	}


    //dessin la selection
    if (mode == GL_RENDER)
    {
        glDisable(GL_LINE_STIPPLE);
        drawSelection();
    }

    //update render
    swapBuffers();
}


void GLView::zoom_in()
{
    m_zoom += 0.5;

    //update render
    main_Render(GL_RENDER);
}

void GLView::zoom_out()
{
    m_zoom -= 0.5;

    //update render
    main_Render(GL_RENDER);
}


void GLView::set_center_view( int x, int y )
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    m_position_X += int(( (viewport[2]/2)-x)/(1+m_zoom));
    m_position_Y += int(( (viewport[3]/2)-y)/(1+m_zoom));

    //update render
    main_Render(GL_RENDER);
}

void GLView::moveHorizontal( int dx )
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    m_position_X += dx;

    //update render
    main_Render(GL_RENDER);
}

void GLView::moveVertical( int dy )
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    m_position_Y += dy;

    //update render
    main_Render(GL_RENDER);
}

void GLView::beginSelectionRender( GLuint * selectBuf, GLint * viewport )
{
    glGetIntegerv(GL_VIEWPORT, viewport);

    glSelectBuffer(BUFSIZE, selectBuf);

    glRenderMode(GL_SELECT);

    glInitNames();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
}


void GLView::multipleSelectionRender()
{
    GLuint selectBuf[512];
    GLint viewport[4];

    beginSelectionRender( selectBuf, viewport );

    int dx = x2-x1;
    int dy = y2-y1;

    int centreX = x1+(dx/2.0);
    int centreY = viewport[3]-(y1+(dy/2.0));

    if ( dx == 0 )
        dx = 1;

    if ( dy == 0 )
        dy = 1;

    gluPickMatrix( (GLdouble) centreX, (GLdouble) centreY, (GLdouble) abs(dx), (GLdouble) abs(dy), viewport );

    GLint hits = endSelectionRender();
    processMultipleHits( hits, selectBuf );
}


void GLView::simpleSelectionRender( int x, int y )
{
    GLuint selectBuf[512];
    GLint viewport[4];

    beginSelectionRender( selectBuf, viewport );

    gluPickMatrix( (GLdouble) x, (GLdouble) viewport[3]-y, (GLdouble) 1, (GLdouble) 1, viewport );

    GLint hits = endSelectionRender();
    processSimpleHits( hits, selectBuf );
}


GLint GLView::endSelectionRender()
{
    glOrtho( 0, width(), 0, height(), -5, 5 );

    main_Render(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    swapBuffers();

    return glRenderMode(GL_RENDER);
}


void GLView::processSimpleHits( GLint hits, GLuint buffer[] )
{
    m_map_objSelected.clear();
    std::map<int,int> vertexObject;

    GLuint *ptr = (GLuint *) buffer;
    for (int i = 0; i < hits; i++)
    {
        int names = *ptr;
        //  		std::cout << names  << std::endl;
        ptr++;
        // 		printf(" z1 is %g;", (float) *ptr/0x7fffffff);
        ptr++;
        // 		printf(" z2 is %g\n", (float) *ptr/0x7fffffff);
        ptr++;
        vertexObject[*ptr] += 1;
        ptr+=names;
    }

    std::map<int,int>::iterator it;
    for ( it = vertexObject.begin() ; it!=vertexObject.end(); it++ )
    {
        int id = it->first;
        if ( vertexObject[ id ] == 1 )
            m_map_objSelected[ id ] = 1;
    }
}


void GLView::processMultipleHits( GLint hits, GLuint buffer[] )
{
    m_map_objSelected.clear();
    std::map<int,int> vertexObject;

    GLuint *ptr = (GLuint *) buffer;
    for (int i = 0; i < hits; i++)
    {
        int names = *ptr;
        //  		std::cout << names  << std::endl;
        ptr++;
        // 		printf(" z1 is %g;", (float) *ptr/0x7fffffff);
        ptr++;
        // 		printf(" z2 is %g\n", (float) *ptr/0x7fffffff);
        ptr++;
        vertexObject[*ptr] += 1;
        ptr+=names;
    }

    std::map<int,int>::iterator it;
    for ( it = vertexObject.begin() ; it!=vertexObject.end(); it++ )
    {
        int id = it->first;

        if ( vertexObject[ id ] >= 5 )
            m_map_objSelected[ id ] = 1;
    }
}


void GLView::clearSelection()
{
    selection = false;
    main_Render(GL_RENDER);
}


bool GLView::clicOnSelectObject( int x, int y )
{
    GLuint selectBuf[512];
    GLint viewport[4];

    beginSelectionRender( selectBuf, viewport );
    gluPickMatrix( (GLdouble) x, (GLdouble) viewport[3]-y, (GLdouble) 1, (GLdouble) 1, viewport );
    GLuint hits = endSelectionRender();

    std::map<int,int> vertexObject;

    GLuint *ptr = (GLuint *) selectBuf;
    for (int i = 0; i < hits; i++)
    {
        int names = *ptr;
        ptr+=3;
        vertexObject[*ptr] += 1;
        ptr+=names;
    }

    std::map<int,int>::iterator it;
    for ( it = vertexObject.begin() ; it!=vertexObject.end(); it++ )
        if ( m_map_objSelected[ it->first ] == 1 )
            return true;

    return false;
}


void GLView::setDeplacement( int dx, int dy )
{
    int dx2 = dx*(1-4*m_zoom);
    int dy2 = dy*(1-4*m_zoom);
    //chaque objet de la selection est dÃ©placÃ©
    // 	std::map<int,GLObject>::iterator it = m_map_objects.begin();
    // 	for ( it = m_map_objects.begin() ; it != m_map_objects.end() ; it++ )
    // 		if ( m_map_objSelected[it->second.id()] == 1 )
    // 			m_map_objects[it->second.id()].moveTo( dx2, dy2 );

    main_Render(GL_RENDER);
}


void GLView::setMultipleSelection( int a1, int b1, int a2, int b2 )
{
    selection = true;

    x1 = a1;
    y1 = b1;
    x2 = a2;
    y2 = b2;

    multipleSelectionRender();
    main_Render(GL_RENDER);
}


void GLView::setSimpleSelection( int x, int y )
{
    simpleSelectionRender( x, y );
    main_Render(GL_RENDER);
}


void GLView::drawSelection()
{
    if ( selection )
    {
        glLoadIdentity();

        glBegin( GL_LINE_STRIP );
//         glColor3f( 1.0, 0, 0 );
        glVertex3i( x1, height()-y1, 1 );
        glVertex3i( x1, height()-y2, 1 );
        glVertex3i( x2,  height()-y2, 1 );
        glVertex3i( x2,  height()-y1, 1 );
        glVertex3i( x1,height()-y1, 1 );
        glEnd();
    }
}


int GLView::countSelectedObject()
{}


void GLView::makeImage()
{
	m_vect_pix.clear();
	
	const ModelData * model = m_presentationThumbGL->model();
	if ( model != 0 )
	{
		for ( int i=0 ; i<model->size() ; i++ )
		{
			const ImageInfo & image = model->imageInfo( i );
			if ( image.isImage() )
			{
				GLuint id = bindTexture(QPixmap(image.pathImage()),GL_TEXTURE_2D);
				GLuint list = glGenLists(1);
	
				glNewList(list, GL_COMPILE);
				glBindTexture(GL_TEXTURE_2D, id);
				glBegin(GL_QUADS);
				glTexCoord2f(0, 0);
				glVertex3f( 50+m_vect_pix.size()*100,  50, 0);
				glTexCoord2f(0, 1);
				glVertex3f( 50+m_vect_pix.size()*100, 150, 0);
				glTexCoord2f(1, 1);
				glVertex3f(150+m_vect_pix.size()*100, 150, 0);
				glTexCoord2f(1, 0);
				glVertex3f(150+m_vect_pix.size()*100,  50, 0);
				glEnd();
				glEndList();
	
				m_vect_pix.push_back( list );
			}
		}
	}
}




