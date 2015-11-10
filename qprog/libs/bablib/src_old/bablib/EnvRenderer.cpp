#include "EnvRenderer.h"
#include "ViewportGL.h"

/**************************************************************************************/
// <EnvRenderer> implementation:

EnvRenderer::EnvRenderer(QString shaderName, bool writeZ) : writeZ(writeZ) {
    prog.loadDir(shaderName);
    }

EnvRenderer::~EnvRenderer() {
    prog.destroy();
    }
    
void EnvRenderer::render() {
    if (!prog.isLinked()) return;
    GLboolean old_depth_mask;
    if (!writeZ) {
        glGetBooleanv(GL_DEPTH_WRITEMASK, &old_depth_mask);
        glDepthMask(false);
        }
    prog.activate();
    ViewportGL view;
    prog.setUniformIVec2("res", view.w, view.h, false);      //@ opti possible : ne faire que lorsque la taille du viewport change
    glBegin(GL_QUADS);
        glVertex2f(0,0);
        glVertex2f(1,0);
        glVertex2f(1,1);
        glVertex2f(0,1);
    glEnd();
    prog.deactivate();
    if (!writeZ) glDepthMask(old_depth_mask);
    }

/**************************************************************************************/
// <EnvMapRenderer> implementation:

EnvMapRenderer::EnvMapRenderer(Texture *texCubeMap, float *brightness) : EnvRenderer("[bablib]/envmap", false) {
    prog.addTexture("cubeMap", texCubeMap, true);
    if (brightness != NULL) prog.addUniform("brightness", brightness, true);
    }

/*
EnvMapRenderer::~EnvMapRenderer() {
    prog.destroy();
    }
*/
    
/*
void EnvMapRenderer::render() {
    if (!prog.isLinked()) return;
    prog.activate();
    glBegin(GL_QUADS);
        glVertex2f(0,0);
        glVertex2f(1,0);
        glVertex2f(1,1);
        glVertex2f(0,1);
    glEnd();
    prog.deactivate();
    }
*/

