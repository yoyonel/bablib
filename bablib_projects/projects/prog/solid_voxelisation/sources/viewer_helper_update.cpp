#include "viewer_sv.h"
//
#include <Params.h>


void Viewer::updateFrameBuffers() {
    update_matrix();
    MSG_CHECK_GL;

    // - FRAME_BUFFERS: Maj des FBs et Render-Textures associÃ©es
    updateSolidVoxelisation();

    // - TIMER_QUERY
    //
    if ( PARAM(bool, timer_query.print) ) {
    }
}

void Viewer::updateSolidVoxelisation() {
    // utilisation d'une lambda fonction pour l'affichage de la scene a voxeliser
    // pour le '[&]' -> url: http://stackoverflow.com/questions/4940259/lambdas-require-capturing-this-to-call-static-member-function
    // Penser a placer le corps d'implémentation du template dictement accessible via le .h du prototype
    // => mettre en place des fichiers _impl.h comme dans la bablib
    solid_voxelisation->update( [&](ProgGLSL _prog) {
        // Draw scene occluders
        // Activate shader for rendering
        vbo->setProg( _prog );
        // RENDER
        if ( !PARAM(bool, vbo.enable_cull_face) )
            glDisable(GL_CULL_FACE);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        // MODEL_SPACE -> WORLD_SPACE
        pt_object_to_world_vbo.glMultModelView();
        vbo->render(GL_TRIANGLES, indexBuffer);
        glPopMatrix();
    });

    img_sv = solid_voxelisation->getImage();
    tex_sv = solid_voxelisation->texture();

    MSG_CHECK_GL;
}

void Viewer::update_camera_light() {
    qgl_cam_light_mf.setType( PARAM(bool, sv.use_ortho_camera) ? qglviewer::Camera::ORTHOGRAPHIC : qglviewer::Camera::PERSPECTIVE );
    qgl_cam_light_mf.computeProjectionMatrix();
    qgl_cam_light_mf.computeModelViewMatrix();
}
