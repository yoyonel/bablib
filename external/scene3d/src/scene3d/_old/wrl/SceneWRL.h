#ifndef __SCENE_WRL__
#define __SCENE_WRL__

#include "../Scene3D.h"
#include <glide/scene.h>

class SceneWRL : public Scene3D {
    public:
        class Loader : public Scene3DLoader {
            virtual Scene3D* load(QString fileName) const { return new SceneWRL(fileName); }
            virtual QString suffix() const { return "WRL"; }
            };
    public:
        SceneWRL(QString fileName);
        ~SceneWRL();
        
        virtual void init();
        virtual void render() const;
        virtual Vec3  getCenter() const;
        virtual float getRadius() const;
    
    private:
        mutable glide::Scene scene;
    };

#endif
