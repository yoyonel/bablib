#ifndef __SCENE_OBJ__
#define __SCENE_OBJ__

#include "../Scene3D.h"
#include <Vec3.h>
#include <glide/glasset.h>
class Face;

class SceneOBJ : public Scene3D {
    public:
        class Loader : public Scene3DLoader {
            virtual Scene3D* load(QString fileName) const { return new SceneOBJ(fileName); }
            virtual QString suffix() const { return "OBJ"; }
            };
    public:
        SceneOBJ(QString fileName);
        ~SceneOBJ();
        
        virtual void init();
        virtual void render() const;
        virtual Vec3  getCenter() const;
        virtual float getRadius() const;
    
    private:
        int nv;     // nombre de vertices
        int nf;     // nombre de faces
        Vec3 *vertex;
        Vec3 *normal;
        Face *face;
        Vec3 center;
        float radius;
        glide::GLAsset *geometry;
        //@@@@@@@
        GLuint displist;
    };

#endif
