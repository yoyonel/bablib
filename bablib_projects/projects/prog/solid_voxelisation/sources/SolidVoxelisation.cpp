#include "SolidVoxelisation.h"
#include "helper_preprocess.h"
//
#include <Params.h>
#include <Message.h>


SolidVoxelisation::SolidVoxelisation(int w, int h, int d, const QString _path_to_shader) {
    m_size = Dim3D(w, h, d);
    //
    m_path_to_shader = _path_to_shader;
    //
    m_internalformat = GL_RGBA32UI; // => 128 bits
    m_filter = GL_NEAREST;
    m_wrapmode = GL_CLAMP_TO_EDGE;
    m_depth_format = GL_DEPTH_COMPONENT32;
    //
    m_isInit = init();
}

bool SolidVoxelisation::init() {
    bool isInit = true;

    isInit &= initTextures();
    isInit &= initShaders();
    isInit &= initFrameBuffers();

    return isInit;
}

bool SolidVoxelisation::initTextures(bool _reset) {
    if ( _reset ) {
        destroyTextures();
    }

    //
    m_tex_sv = Texture::createTex2DInteger(
                m_size.width(),
                m_size.height(),
                m_internalformat,
                m_filter,
                m_wrapmode
                );

    const int resZ = std::min(128, m_size.depth());

    Image1DUInt4 img(resZ, 0);
    GLuint* lookup = (GLuint*)(img.ptrData());
    memset(lookup, 0U, sizeof(GLuint)*resZ);
    for( int i = 1; i < resZ; i++ ) {
        if ( i < 32 ) {
            lookup[ 4 * i + 3 ] = ( 1U << std::min( 32, i ) ) - 1;
        } else if( i == 32 ) {
            lookup[ 4 * i + 3 ] = 0xFFFFFFFF;
        } else if ( i < 64 ) {
            lookup[ 4 * i + 3 ] = 0xFFFFFFFF;
            lookup[ 4 * i + 2 ] = ( 1U << std::min( 32, ( i - 32 ) ) ) - 1;
        } else if( i == 64 ) {
            lookup[ 4 * i + 3 ] = 0xFFFFFFFF;
            lookup[ 4 * i + 2 ] = 0xFFFFFFFF;
        } else if ( i < 96 ) {
            lookup[ 4 * i + 3 ] = 0xFFFFFFFF;
            lookup[ 4 * i + 2 ] = 0xFFFFFFFF;
            lookup[ 4 * i + 1 ] = ( 1U << std::min( 32, ( i - 64 ) ) ) - 1;
        } else if( i == 96 ) {
            lookup[ 4 * i + 3 ] = 0xFFFFFFFF;
            lookup[ 4 * i + 2 ] = 0xFFFFFFFF;
            lookup[ 4 * i + 1 ] = 0xFFFFFFFF;
        } else {
            lookup[ 4 * i + 3 ] = 0xFFFFFFFF;
            lookup[ 4 * i + 2 ] = 0xFFFFFFFF;
            lookup[ 4 * i + 1 ] = 0xFFFFFFFF;
            lookup[ 4 * i + 0 ] = ( 1U << std::min( 32, ( i - 96 ) ) ) - 1;
        }
        /**
        std::cout << "bits field: sample " << i << " -> " << \
                     std::bitset<32>(lookup[ 4 * i + 0 ]) << \
                     std::bitset<32>(lookup[ 4 * i + 1 ]) << \
                     std::bitset<32>(lookup[ 4 * i + 2 ]) << \
                     std::bitset<32>(lookup[ 4 * i + 3 ]) << \
                     std::endl;
        /**/
    }
    m_tex_bitmask = Texture(&img, GL_NEAREST, GL_CLAMP_TO_EDGE);
    m_tex_bitmask.load(GL_RGBA32UI);
    img.destroy();

    //m_img_sv = Image2DUInt4(tex_sv.getWidth(), tex_sv.getHeight());

    MSG_CHECK_GL;

    return true;
}

bool SolidVoxelisation::initFrameBuffers( bool _reset )
{
    if ( _reset ) {
        destroyFrameBuffers();
    }

    m_fb_sv = FrameBuffer::create_Tex2D_Z(&m_tex_sv, m_depth_format);
    //
    m_fb_sv.checkCompleteness(true);

    MSG_CHECK_GL;

    return true;
}

bool SolidVoxelisation::initShaders( bool _reset )
{
    if ( _reset ) {
        destroyShaders();
    }

    LOADDIRSHADER( m_path_to_shader, m_prog_sv );
    m_prog_sv.addTexture("bitmask", &m_tex_bitmask);

    MSG_CHECK_GL;

    return true;
}

void SolidVoxelisation::destroyTextures() {
    m_tex_bitmask.destroy();
    m_tex_sv.destroy();
}

void SolidVoxelisation::destroyFrameBuffers() {
    m_fb_sv.destroy();
}

void SolidVoxelisation::destroyShaders() {
    m_prog_sv.destroy();
}
