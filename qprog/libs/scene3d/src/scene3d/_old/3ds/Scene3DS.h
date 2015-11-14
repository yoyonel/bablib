#ifndef __SCENE_3DS__
#define __SCENE_3DS__

#include "../Scene3D.h"
#include <lib3ds/file.h>
#include <lib3ds/node.h>
//struct Lib3dsFile;
//struct Lib3dsNode;

// basé sur la version 1.3.0 de lib3ds (http://lib3ds.sourceforge.net)
class Scene3DS : public Scene3D {
    public:
        class Loader : public Scene3DLoader {
            virtual Scene3D* load(QString fileName) const { return new Scene3DS(fileName); }
            virtual QString suffix() const { return "3DS"; }
            };
    public:
        Scene3DS(QString fileName);
        ~Scene3DS();
        
        virtual void init();
        virtual void render() const;
        virtual Vec3  getCenter() const;
        virtual float getRadius() const;

    private:
        void render(Lib3dsNode *node) const;
    
    private:
        Lib3dsFile *file;
        GLuint displist;
        Vec3 center;
        float radius;
    };

#endif
