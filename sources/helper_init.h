#ifndef __HELPER_VIEWER_INIT_H__
#define __HELPER_VIEWER_INIT_H__

#include <Params.h>
#include <QString>
#include <QDebug>

void Viewer::initShaders( bool bDestroy ) 
{
	if ( bDestroy ) {
		destroyShaders();
	}

	LOADDIRSHADER( PARAM(QString, shaderDir.prog_mipmap), prog_mipmap );
	prog_mipmap.addTexture("tex_src", &imgTex);
	/**/
	
	CHECK_GL_ERRORS("initShaders");
}

void Viewer::initTextures( bool bDestroy )
{
	if ( bDestroy ) {
		destroyTextures();
		}
		
	// -- load texture file
	QString qsFileNameTexture( PARAM(QString, texture.FileName) );
	qDebug() << "# Load texture: " << qsFileNameTexture;
	// - TEXTURE 2D
	Image2D<Float4> imgData( qsFileNameTexture ); 	
	//
    	imgTex = Texture( &imgData, PARAM(GLenum, texture.interp_mode), PARAM(GLenum, texture.wrap) );
    	imgTex.load(); // charge en VRAM
    	imgData.destroy(); // plus besoin de la version RAM
    	// Border Color
    	imgTex.bind();
    	imgTex.setBorderColor( PARAM(Float4, texture.border_color) );
    	// init de la generation automatique de mipmap
    	imgTex.initAutoMipMapGeneration( PARAM(GLenum, texture.mipmap.min_filter), PARAM(GLenum, texture.mipmap.mag_filter) );
    	//
	qDebug() << "# Size texture: " << imgTex.getWidth() << "x" << imgTex.getHeight();
	
	//
	CHECK_GL_ERRORS("initTextures");
}	
#endif
