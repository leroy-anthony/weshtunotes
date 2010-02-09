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

#include "glviewimage.h"

GLViewImage::GLViewImage( QWidget * parent ):
        QGLWidget(parent),
        m_zoom( 0 ),
        m_position_X( 0 ),
        m_position_Y( 0 ),
        m_abstractPresentation( (abstractPresentation*) parent )
{
    initializeGL();
}


GLViewImage::~GLViewImage()
{}


void GLViewImage::initializeGL()
{
    //Set up the rendering context, define display lists etc.:
    glClearColor(0.0, 0.0, 0.0, 0.0);


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


//     const ModelData * model = m_abstractPresentation->model();
//     if ( model != 0 )
//     {
//         int i=0;
//         while( !model->imageInfo( i ).isImage() )
//             i++;
// 
//         const ImageInfo & image = model->imageInfo( i );
//         m_idTexture = bindTexture( QPixmap(image.pathImage()), GL_TEXTURE_2D );
//         m_idList = glGenLists(1);
// 
//         glNewList(m_idList, GL_COMPILE);
//         glBindTexture(GL_TEXTURE_2D, m_idTexture);
//         glBegin(GL_QUADS);
//         glTexCoord2f(0, 0);
//         glVertex3f( 0,  0, 0 );
//         glTexCoord2f(0, 1);
//         glVertex3f( 0, image.height(), 0 );
//         glTexCoord2f(1, 1);
//         glVertex3f( image.width(), image.height(), 0 );
//         glTexCoord2f(1, 0);
//         glVertex3f( image.width(),  0, 0 );
//         glEnd();
//         glEndList();
//     }
}


void GLViewImage::loadImage( QPixmap * image, int choix )
{
	assert( image != 0 );
	
	qDebug() << choix;
	m_choix = choix;
	
	if ( !glIsList(choix) )
	{
		m_idTextures[choix] = bindTexture( *image, GL_TEXTURE_2D );

		glNewList(choix, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, m_idTextures[choix]);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f( 0,  0, 0 );
		glTexCoord2f(0, 1);
		glVertex3f( 0, image->height(), 0 );
		glTexCoord2f(1, 1);
		glVertex3f( image->width(), image->height(), 0 );
		glTexCoord2f(1, 0);
		glVertex3f( image->width(),  0, 0 );
		glEnd();
		glEndList();
	}
	
	m_position_X = (width() - image->width())/2;
	m_position_Y = (height() - image->height())/2;
	m_zoom = 0;
	mainRender( GL_RENDER );
}


void GLViewImage::reset( int i )
{
	glDeleteLists( 0, i );

	for ( int i=0 ; i<m_idTextures.size() ; i++ )
		deleteTexture( m_idTextures[i] );
}


void GLViewImage::setIdList( int i )
{
	m_idTextures.clear();
	m_idTextures.resize( i, -1 );
}

void GLViewImage::resizeGL( int w, int h )
{
    // setup viewport, projection etc.:
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    glOrtho( 0, width(), 0, height(), -5, 5);
}


void GLViewImage::paintGL()
{
    mainRender(GL_RENDER);
}


void GLViewImage::mainRender( GLenum mode )
{
    // clean the scene
    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );

    //zoom and center view
    glLoadIdentity();
    glTranslatef( -width()*m_zoom/2, -height()*m_zoom/2, 1 );
    glScalef( 1+m_zoom, 1+m_zoom, 1 );
    glTranslatef( m_position_X, m_position_Y, 0 );

	glCallList( m_choix );

    //update render
    swapBuffers();
}


void GLViewImage::zoomIn()
{
    m_zoom += 0.1;

    //update render
    mainRender(GL_RENDER);
}

void GLViewImage::zoomOut()
{
    m_zoom -= 0.1;

    //update render
    mainRender(GL_RENDER);
}

void GLViewImage::setView( int x, int y )
{
    m_position_X += int( x/(1+m_zoom) );
    m_position_Y += int( y/(1+m_zoom) );

    //update render
    mainRender(GL_RENDER);
}

void GLViewImage::setCenterView( int x, int y )
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    m_position_X += int( ((viewport[2]/2)-x)/(1+m_zoom) );
    m_position_Y += int( ((viewport[3]/2)-y)/(1+m_zoom) );

    //update render
    mainRender(GL_RENDER);
}

