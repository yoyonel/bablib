#ifndef __SCENE_3D__
#define __SCENE_3D__

#include <Vec3.h>
#include <QString>
#include <QList>

class Scene3DLoader;

class Scene3D {
    public:
        Scene3D(QString fileName) : fileName(fileName), initialized(false) {}
        virtual ~Scene3D() {}
        virtual void init() = 0;            // à appeler avant tout appel à <render>, dans un contexte OpenGL
        virtual void render() const = 0;
        void renderUnlit()   const;
        void renderNormals() const;
           
        virtual Vec3  getCenter() const = 0;
        virtual float getRadius() const = 0;
        
        bool ready() const { return initialized; }
        QString getFileName() const { return fileName; }
        
        static Scene3D* load(QString fileName);
    
    private:
        static QList<Scene3DLoader*> loaders;
        QString fileName;
    protected:
        bool initialized;
    };

class Scene3DLoader {
    public:
        virtual ~Scene3DLoader() {}
        virtual Scene3D* load(QString fileName) const = 0;
        virtual QString suffix() const = 0;
    };

#endif
