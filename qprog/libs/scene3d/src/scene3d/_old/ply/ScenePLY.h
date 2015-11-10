#ifndef __SCENE_PLY__
#define __SCENE_PLY__

#include "../Scene3D.h"
#include <glide/glasset.h>

class ScenePLY : public Scene3D {
    public:
        class Loader : public Scene3DLoader {
            virtual Scene3D* load(QString fileName) const { return new ScenePLY(fileName); }
            virtual QString suffix() const { return "PLY"; }
            };
    public:
        ScenePLY(QString fileName);
        ~ScenePLY();
        
        virtual void init();
        virtual void render() const;
        virtual Vec3  getCenter() const;
        virtual float getRadius() const;
    
    private:
        class Object;
        Object *object;
        glide::GLAsset *geometry;
    };

#endif
