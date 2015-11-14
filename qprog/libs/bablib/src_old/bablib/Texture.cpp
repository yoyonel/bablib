#include "Texture.h"
#include "Image2D.h"
#include "CubeMap.h"
#include "Message.h"

static int log2i(unsigned x) {
    int l = -1; // mylog2(0) will return -1
    while (x != 0u) {
        x = x >> 1u;
        ++l;
    }
    return l;
}

BABLIB_NAMESPACE_USE

Texture::Texture(const TextureData *texData, GLenum interpMode, GLenum wrapMode)
        : id(0),
          texData(texData),
          texMode(texData->textureMode()),
          interpMode(interpMode),
          wrapMode(wrapMode),
          loaded(false) {
    }

Texture::Texture(GLenum texMode, GLenum interpMode, GLenum wrapMode)
        : id(0),
          texData(NULL),
          texMode(texMode),
          interpMode(interpMode),
          wrapMode(wrapMode),
          loaded(false) {
    generateID();
    init();
    }

/***********************************************************************************************/

bool Texture::hasContent() const {
    switch (texMode) {
        case GL_TEXTURE_1D : return getWidth() > 0;
        case GL_TEXTURE_2D : return getWidth() > 0 && getHeight() > 0;
        case GL_TEXTURE_3D : return getWidth() > 0 && getHeight() > 0 && getDepth() > 0;
        };
    // cas restant : GL_TEXTURE_CUBE_MAP :
    bool defined = true;
    bind();
    for (int i=0; i<6; i++) {
        int w, h;
        glGetTexLevelParameteriv(CubeMap::targetNames[i], 0, GL_TEXTURE_WIDTH, &w);
        glGetTexLevelParameteriv(CubeMap::targetNames[i], 0, GL_TEXTURE_HEIGHT, &h);
        defined = defined && w > 0 && h > 0;
        }
    return defined;
    }

/***********************************************************************************************/

void Texture::generateID() {
    glGenTextures(1, &id);
    if (id <= 0) Message::error("probleme lors de la generation de l'identifiant de texture");
    }

void Texture::bind() const {
    glBindTexture(texMode, id);
    }

void Texture::init() const {
    bind();
    if (texMode != GL_TEXTURE_BUFFER_EXT) {
        updateInterpMode();
        updateWrapMode();
        }
    }

void Texture::destroy() {
    if (id == 0) return;
    glDeleteTextures(1, &id);
    id = 0;
    loaded = false;
    }

/***********************************************************************************************/

void Texture::load() {
    if (texData == NULL) {
        Message::error("pas de donnees associees a la texture");
        return;
        }
    load(texData->defaultTexFormat());
    }

void Texture::load(GLint internalFormat) {
    if (texData == NULL) {
        Message::error("pas de donnees associees a la texture");
        return;
        }
    if (id <= 0) generateID();
    init();
    texData->loadToGPU(internalFormat);
    loaded = true;
    }

/***********************************************************************************************/

void Texture::updateInterpMode() const {
    glTexParameteri(texMode, GL_TEXTURE_MAG_FILTER, interpMode);
    glTexParameteri(texMode, GL_TEXTURE_MIN_FILTER, interpMode);
    }

void Texture::updateWrapMode() const {
    glTexParameteri(texMode, GL_TEXTURE_WRAP_S, wrapMode);
    glTexParameteri(texMode, GL_TEXTURE_WRAP_T, wrapMode);
    glTexParameteri(texMode, GL_TEXTURE_WRAP_R, wrapMode);
    }

void Texture::setInterpMode(GLenum interpMode) {
    this->interpMode = interpMode;
    bind();
    updateInterpMode();
    }

void Texture::setWrapMode(GLenum wrapMode) {
    this->wrapMode = wrapMode;
    bind();
    updateWrapMode();
    }

void Texture::setBorderColor(const float *color) const {
    bind();
    glTexParameterfv(texMode, GL_TEXTURE_BORDER_COLOR, color);
    }

void Texture::setBorderColor(float r, float g, float b, float a) const {
    const float color[4] = { r, g, b, a };
    setBorderColor(color);
    }

/***********************************************************************************************/

void Texture::activate(GLint mode) const {
    glEnable(texMode);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode);
    bind();
    }

void Texture::deactivate() const {
    glDisable(texMode);
    }

/***********************************************************************************************/

void Texture::setActiveUnit(int unit) {
    glActiveTexture(GL_TEXTURE0_ARB + unit);
    }

int Texture::maxUnits() {
    int v;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &v);  //@ ou GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS ..
    return v;
    }

/***********************************************************************************************/
//@@ a gérer : dans les fonctions suivantes, le 7eme argument (GL_RGBA) doit prendre
//  la valeur GL_DEPTH_COMPONENT pour les depth textures 

Texture Texture::createTex1D(int w, GLint internalFormat, GLenum interpMode, GLenum wrapMode) {
    Texture tex(GL_TEXTURE_1D, interpMode, wrapMode);
    glTexImage1D(GL_TEXTURE_1D, 0, internalFormat, w, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);   // arguments 7 et 8 quelconques
    tex.loaded = true;
    return tex;
    }

Texture Texture::createTex2D(int w, int h, GLint internalFormat, GLenum interpMode, GLenum wrapMode) {
    Texture tex(GL_TEXTURE_2D, interpMode, wrapMode);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);   // arguments 7 et 8 quelconques
    tex.loaded = true;
    return tex;
    }

Texture Texture::createTex3D(int w, int h, int d, GLint internalFormat, GLenum interpMode, GLenum wrapMode) {
    Texture tex(GL_TEXTURE_3D, interpMode, wrapMode);
    glTexImage3D(GL_TEXTURE_3D, 0, internalFormat, w, h, d, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);   // arguments 7 et 8 quelconques
    tex.loaded = true;
    return tex;
    }

Texture Texture::createTex2DArray(int w, int h, int d, GLint internalFormat, GLenum interpMode, GLenum wrapMode) {
    Texture tex(GL_TEXTURE_2D_ARRAY_EXT, interpMode, wrapMode);
    glTexImage3D(GL_TEXTURE_2D_ARRAY_EXT, 0, internalFormat, w, h, d, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);   // arguments 7 et 8 quelconques
    tex.loaded = true;
    return tex;
    }

Texture Texture::createTexCube(int n, GLint internalFormat, GLenum interpMode, GLenum wrapMode) {
    Texture tex(GL_TEXTURE_CUBE_MAP, interpMode, wrapMode);
    for (int i=0; i<6; i++)
        glTexImage2D(CubeMap::targetNames[i], 0, internalFormat, n, n, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);   // arguments 7 et 8 quelconques
    tex.loaded = true;
    return tex;
    }

/***********************************************************************************************/

Texture Texture::createTex1D(QString imageFileName, GLint internalFormat, GLenum interpMode, GLenum wrapMode) {
    Image1D<Float4> image(imageFileName);
    Texture tex(&image, interpMode, wrapMode);
    tex.load(internalFormat);
    image.destroy();
    return tex;
    }

Texture Texture::createTex2D(QString imageFileName, GLint internalFormat, GLenum interpMode, GLenum wrapMode) {
    Image2D<Float4> image(imageFileName);
    Texture tex(&image, interpMode, wrapMode);
    tex.load(internalFormat);
    image.destroy();
    return tex;
    }

/***********************************************************************************************/

void Texture::copyReadBuffer(GLenum internalFormat, int w, int h, int x, int y) {
    texMode = GL_TEXTURE_2D;
    activate();
    glCopyTexImage2D(GL_TEXTURE_2D, 0, internalFormat, x, y, w, h, 0);
    deactivate();
    }

/***********************************************************************************************/

int Texture::getParam(GLenum paramName) const {
    int res;
    bind();
    glGetTexParameteriv(texMode, paramName, &res);
    return res;
    }

int Texture::getLevelParam(int target, GLenum paramName, int level) const {
    int res;
    bind();
    glGetTexLevelParameteriv(target, level, paramName, &res);
    return res;
    }

Dim2D Texture::renderSize(GLenum target, int level) const {
    int w = getLevelParam(target, GL_TEXTURE_WIDTH,  level),
        h = getLevelParam(target, GL_TEXTURE_HEIGHT, level);
    return Dim2D(w, h);
    }

/***********************************************************************************************/

void Texture::initAutoMipMapGeneration( GLenum _min_filter, GLenum _mag_filter, GLint _max_levels, GLuint _base_level, GLenum _hint_mode ) {
	// @ faire, tester les modes de texture et la possibilité de les mipmapper
	bind();
	// - 
    	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap
    	// - reglages des filtres
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _min_filter );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _mag_filter );
	// - reglages des nombres de mipmaps a generer
	m_n_mipmaps= _max_levels;
	if ( _max_levels == -1 ) {
		m_n_mipmaps = (GLint)( fmax( log2i(getWidth()), log2i(getHeight()) ) );
		Message::info( QString("# Nb MipMap to generate: ") + QString::number( m_n_mipmaps ) );
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_n_mipmaps);
	//
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, _base_level );
	//Message::info( "# _base_level: " + QString::number( _base_level ) );
	//Message::info( "# GL_TEXTURE_BASE_LEVEL: " + QString::number(getParam( GL_TEXTURE_BASE_LEVEL)) );
	//
	glHint( GL_GENERATE_MIPMAP_HINT, _hint_mode );
	// Allocate texture mipmap memory
    	generateMipMap();
}
