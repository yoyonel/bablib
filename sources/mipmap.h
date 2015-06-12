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

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>

#include <QGLViewer/qglviewer.h>

#include <ProgGLSL.h>
#include <Texture.h>

class Viewer : public QGLViewer {
public:
        Viewer();
        ~Viewer();
        
protected :
	virtual void draw();
	virtual void init();
        virtual void keyPressEvent(QKeyEvent *);
        
// HELPERS
protected:
	void destroyAll();
	void destroyShaders();
	void destroyTextures();
	void destroyFrameBuffers();
	//
	void initShaders( bool bDestroy = false );
	void initTextures( bool bDestroy = false );
	//
	void drawQuad(float sz, float texc);
	void drawTexture( int x, int y, int w, int h, ProgGLSL& _prog, Texture& _tex );
	//
private:
	bool bFirstInit;
	
	// TEXTURES
	Texture imgTex;
	// SHADERS
	ProgGLSL prog_mipmap;
};
