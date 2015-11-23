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


//class ProgGLSL;

typedef Image2D<UInt4> Image2DUInt4;
//typedef void(*unary_void_meth)(ProgGLSL*);


///
/// \brief The SolidVoxelisation class
///
class SolidVoxelisation {

public:
    ///
    /// \brief SolidVoxelisation
    /// \param w
    /// \param h
    /// \param d
    /// \param path_to_shader
    ///
    SolidVoxelisation(int w, int h, int d, const QString path_to_shader="[shaders]/solid_voxelisation");

    ///
    ~SolidVoxelisation() { destroy(); }

    ///
    /// \brief width
    /// \return
    ///
    int width() const   { return m_size.width();  }
    ///
    /// \brief height
    /// \return
    ///
    int height() const  { return m_size.height(); }
    ///
    /// \brief depth
    /// \return
    ///
    int depth() const   { return m_size.depth();  }

    ///
    ///
    /// \brief update
    /// \param _meth_draw, F = [](ProgGLSL) -> void
    /// \return bool
    template <typename F>
    bool update(F _meth_draw);

    ///
    /// \brief setCamera
    /// \param _cam
    ///
    void setCamera(const qglviewer::Camera& _cam) { m_cam = _cam; }

    const qglviewer::Camera& camera() const { return m_cam; }

    ///
    /// \brief texture
    /// \return
    ///
    Texture texture() const { return m_tex_sv; }

    explicit operator Texture() const { return m_tex_sv; }

    ///
    /// \brief ptrTexture
    /// \return
    ///
    const Texture* ptrTexture() const { return &m_tex_sv; }

    ///
    /// \brief framebuffer
    /// \return
    ///
    FrameBuffer framebuffer() const { return m_fb_sv; }

    ///
    /// \brief getImage
    /// \return
    ///
    Image2DUInt4 getImage() const;

protected:
    ///
    /// \brief init
    /// \return
    ///
    bool init();

    ///
    /// \brief initTextures
    /// \param _reset
    /// \return
    ///
    bool initTextures(bool _reset = false);

    ///
    /// \brief initFrameBuffers
    /// \param _reset
    /// \return
    ///
    bool initFrameBuffers(bool _reset = false);

    ///
    /// \brief initShaders
    /// \param _reset
    /// \return
    ///
    bool initShaders(bool _reset = false);

    ///
    /// \brief destroy
    ///
    inline void destroy() { destroyFrameBuffers(); destroyTextures(); destroyShaders(); }

    ///
    /// \brief destroyTextures
    ///
    void destroyTextures();

    ///
    /// \brief destroyFrameBuffers
    ///
    void destroyFrameBuffers();

    ///
    /// \brief destroyShaders
    ///
    void destroyShaders();

private:
    ///
    /// \brief m_size
    ///
    Dim3D m_size;

    ///
    /// \brief m_cam
    ///
    qglviewer::Camera m_cam;

    ///
    /// \brief m_fb_sv
    ///
    FrameBuffer m_fb_sv;

    ///
    /// \brief m_tex_sv
    ///
    Texture m_tex_sv;

    ///
    /// \brief m_tex_bitmask
    ///
    Texture m_tex_bitmask;

    ///
    /// \brief m_prog_sv
    ///
    ProgGLSL m_prog_sv;

    ///
    /// \brief m_isInit
    ///
    bool m_isInit;

    ///
    /// \brief m_internalformat
    ///
    GLenum m_internalformat;
    ///
    /// \brief m_filter
    ///
    GLenum m_filter;
    ///
    /// \brief m_wrapmode
    ///
    GLenum m_wrapmode;
    ///
    /// \brief m_depth_format
    ///
    GLenum m_depth_format;

    ///
    /// \brief m_path_to_shader
    ///
    QString m_path_to_shader;
};

#include "SolidVoxelisation_impl.h"

#endif // SOLID_VOXELISATION_H
