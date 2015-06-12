/****************************************************************************

Copyright (C) 2002-2007 Gilles Debunne (Gilles.Debunne@imag.fr)

This file is part of the QGLViewer library.
Version 2.2.6-3, released on August 28, 2007.

http://artis.imag.fr/Members/Gilles.Debunne/QGLViewer

libQGLViewer is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

libQGLViewer is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libQGLViewer; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*****************************************************************************/

#include "mipmap.h"
#include <stdio.h>	// # fprintf
#include <QKeyEvent>
#include <OpenGL.h>
#include <Image2D.h>
#include "helpers.h"

using namespace std;

Viewer::Viewer() : bFirstInit(true) { };

Viewer::~Viewer() {
	//@@ a faire
	destroyAll();
}
    
// Draws a spiral
void Viewer::draw()
{
	drawTexture( 0, 0, 512, 512, prog_mipmap, imgTex );
}

void Viewer::init()
{	
	if (bFirstInit) {
		// init GLEW
		OpenGL::init();
	}
	
	initTextures( bFirstInit );
	
	//initFrameBuffers();
	
	initShaders( bFirstInit );

	// Restore previous viewer state.
	restoreStateFromFile();
	
	bFirstInit = true;
}

void Viewer::keyPressEvent(QKeyEvent *e) {
	switch( e->key() ) { 
	case Qt::Key_P:
	    Params::reload();
	    updateGL();
	    break;
	case Qt::Key_L:
		Params::reload();
		init();
		updateGL();
        	break;
	default:
		QGLViewer::keyPressEvent(e);
	}
}
