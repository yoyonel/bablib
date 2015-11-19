#ifndef SOLID_VOXELISATION_H
#define SOLID_VOXELISATION_H

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GL/gl.h>

#include <QGLViewer/camera.h>
//
#include <OpenGL.h>
#include <Image1D.h>
#include <Image2D.h>
#include <FrameBuffer.h>
#include <ProgGLSL.h>
#include <Dim3D.h>


typedef Image1D<UInt4> Image1DUInt4;
typedef Image2D<UInt4> Image2DUInt4;


class SolidVoxelisation {

public:
    SolidVoxelisation(int w, int h, int d, const QString path_to_shader="[shaders]/solid_voxelisation");
    ~SolidVoxelisation() { destroy(); }

    int width() const   { return m_size.width();  }
    int height() const  { return m_size.height(); }
    int depth() const   { return m_size.depth();  }

    template <typename F>
    bool update(F _meth_draw);

    void setCamera(const qglviewer::Camera& _cam) { m_cam = _cam; }

    Texture texture() const { return m_tex_sv; }
    const Texture* ptrTexture() const { return &m_tex_sv; }
    FrameBuffer framebuffer() const { return m_fb_sv; }

protected:
    bool init();
    //
    bool initTextures(bool _reset = false);
    bool initFrameBuffers(bool _reset = false);
    bool initShaders(bool _reset = false);
    //
    inline void destroy() { destroyFrameBuffers(); destroyTextures(); destroyShaders(); }
    //
    void destroyTextures();
    void destroyFrameBuffers();
    void destroyShaders();

private:
    Dim3D m_size;

    qglviewer::Camera m_cam;

    FrameBuffer m_fb_sv;
    Texture m_tex_sv;
    //Image2DUInt4 m_img_sv;
    Texture m_tex_bitmask;
    ProgGLSL m_prog_sv;

    bool m_isInit;

    GLenum m_internalformat, m_filter, m_wrapmode;
    GLenum m_depth_format;

    QString m_path_to_shader;
};


template <typename F>
bool SolidVoxelisation::update(F _meth_draw) {
    // ----------
    const GLenum clearBufferBits = GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT;
    const GLboolean GL_ALL_BITS = GLboolean(~0x0);
    const GLboolean colorMask[] = {GL_ALL_BITS, GL_ALL_BITS, GL_ALL_BITS, GL_ALL_BITS};

    FrameBuffer& _framebuffer = m_fb_sv;
    qglviewer::Camera& qgl_cam_light_mf = m_cam;

    glPushAttrib( GL_ALL_ATTRIB_BITS ); {
        _framebuffer.activate();
        glViewport(0, 0, _framebuffer.width(), _framebuffer.height());

        //setOpenGLStates();

        glColorMask( colorMask[0], colorMask[1], colorMask[2], colorMask[3] );
        glClearColor( 0x0, 0x0, 0x0, 0x0 );
        glClear( clearBufferBits );

        // desactivation du test de profondeur
        glDisable( GL_DEPTH_TEST );

        // desactivation du culling face
        glDisable(GL_CULL_FACE);

        // set blending mode
        glEnable( GL_BLEND );
        glLogicOp( GL_XOR );
        glEnable( GL_COLOR_LOGIC_OP );

        glDisable(GL_LIGHTING);

        // -------

        // Set light's camera (OpenGL 1.x style)
        // Sauvegarde des matrices de transformations
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        qgl_cam_light_mf.loadProjectionMatrix();

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        //pt_world_to_light.glLoadModelView();
        qgl_cam_light_mf.loadModelViewMatrix();

        // Draw scene
        m_prog_sv.activate(); {
            _meth_draw(m_prog_sv);
        } m_prog_sv.deactivate();

        // Restore the OpenGL Matrix
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        //
        _framebuffer.deactivate();
    } glPopAttrib();

    // gather the resulting texture data
    //tex_sv.bind();
    //glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    //img_sv.readTexture(&tex_sv);

    // For debugging (mode text debugging)
    //debug_solid_voxelisation(img_sv);
    //
    //debug_solid_voxelisation_graphic_mode_full_image(img_sv, fb_sv, qgl_cam_light_mf);

    MSG_CHECK_GL;
}

#endif // SOLID_VOXELISATION_H
