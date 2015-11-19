#include "Scene3D.h"
#include "TriSoup.h"
#include "TriSoupRenderer.h"
#include <Message.h>

/**************************************************************************************************/
// partie à modifier à chaque ajout de nouveau loader :

//#include "wrl/SceneWRL.h"
#include "3ds/Scene3DS.h"
#include "off/LoaderOFF.h"
#include "obj/LoaderOBJ.h"
#include "ply/LoaderPLY.h"

// interpretation of a TriSoup as a Scene3D :
class SceneTriSoup : public Scene3D {
    public:
        SceneTriSoup(QString fileName, TriSoup *ts, bool own=true) : Scene3D(fileName), ts(ts), own(own), renderer(NULL) {}
        virtual ~SceneTriSoup() {
            if (renderer != NULL) delete renderer;
            if (own) delete ts;
            }
        virtual void init() {
            if (renderer == NULL)
                renderer = new TriSoupRenderer(ts);
            }
        virtual void render() const { renderer->render(); }
        
        virtual Vec3  getCenter() const { return ts->getCenter(); }
        virtual float getRadius() const { return ts->getRadius(); }
        
    private:
        TriSoup *ts;
        const bool own;   // true iff <ts> belongs to this object
        TriSoupRenderer *renderer;
    };

// generic loader for TriSoup as a Scene3D :
class SceneTriSoupLoader : public Scene3DLoader {
    public:
        SceneTriSoupLoader(TriSoupLoader *loader) : loader(loader) {}
        virtual ~SceneTriSoupLoader()                   { delete loader; }
        virtual Scene3D* load(QString fileName) const   { return new SceneTriSoup(fileName, loader->load(fileName)); }
        virtual QString suffix() const                  { return loader->suffix(); }
    private:
        TriSoupLoader *loader;
    };

QList<Scene3DLoader*> Scene3D::loaders = QList<Scene3DLoader*>()
    << new Scene3DS::Loader()
    //<< new SceneWRL::Loader()
    << new SceneTriSoupLoader(new LoaderOFF())
    << new SceneTriSoupLoader(new LoaderOBJ())
    << new SceneTriSoupLoader(new LoaderPLY());    //@ could use the TriSoup::loaders list
    //<< new SceneOFF::Loader()
    //<< new ScenePLY::Loader()
    //<< new SceneOBJ::Loader();

/**************************************************************************************************/

Scene3D* Scene3D::load(QString fileName) {
    foreach(Scene3DLoader *loader, loaders) {
        if (fileName.endsWith("." + loader->suffix(), Qt::CaseInsensitive))
            return loader->load(fileName);
        }
    Message::error(QString("format de fichier non supporté : %1").arg(fileName));
    return NULL;
    }

void Scene3D::renderUnlit()   const { render(); }
void Scene3D::renderNormals() const { render(); }

