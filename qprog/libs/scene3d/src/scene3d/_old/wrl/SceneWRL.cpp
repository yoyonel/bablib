#include "SceneWRL.h"
#include <Message.h>

//using namespace glide;

SceneWRL::SceneWRL(QString fileName) : Scene3D(fileName){
    scene.load(fileName);
    //if (!scene.isLoaded()) loaded = false;
    }

SceneWRL::~SceneWRL() {
    scene.destroy();
    }

void SceneWRL::init() {
    scene.loadTextures();
    scene.mapToGPU();
    initialized = true;
    }
        
void SceneWRL::render() const {
    scene.render();
    }

Vec3  SceneWRL::getCenter() const { return scene.isLoaded() ? Vec3(scene.center()) : Vec3(0.0f); }
float SceneWRL::getRadius() const { return scene.isLoaded() ? scene.radius() : 1.0f; }

