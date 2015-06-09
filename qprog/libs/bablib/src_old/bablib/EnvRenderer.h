#ifndef __ENV_RENDERER__
#define __ENV_RENDERER__

#include "Renderable.h"
#include "ProgGLSL.h"
#include "Texture.h"
#include <QString>

class EnvRenderer : public Renderable {
    public:
        EnvRenderer(QString shaderName = "[bablib]/envgrid", bool writeZ=false);
        //EnvRenderer(ProgGLSL);
        virtual ~EnvRenderer();
    
        virtual void render();
        
        ProgGLSL& shader() { return prog; }
        
    protected:
        ProgGLSL prog;
        bool writeZ;
    };

class EnvMapRenderer : public EnvRenderer {
    public:
        EnvMapRenderer(Texture *texCubeMap, float *brightness = NULL);
        //~EnvMapRenderer();
    
        //virtual void render();
    };

#endif

