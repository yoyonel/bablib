#ifndef __SPHERE_PICKER__
#define __SPHERE_PICKER__

#include "Renderable.h"
#include "Vec3.h"
#include "ProgGLSL.h"

class SpherePicker : public Renderable {
    public:
        SpherePicker(Vec3 center, float radius);    // an OpenGL context must be active
        ~SpherePicker();
        
        void setPoint(Vec3 p) { point = p.normalized(); }
        Vec3 getPoint() const { return point; }
        
        void pick(Vec3 origRay, Vec3 dirRay);
        
        virtual void render();
        
        bool isActive() const { return active; }
        void activate()   { active = true; }
        void deactivate() { active = false; }
        void toggle() { active = !active; }
        
    private:
        Vec3 center;
        float radius;
        Vec3 point;
        ProgGLSL visu;
        bool active;
    };

#endif

