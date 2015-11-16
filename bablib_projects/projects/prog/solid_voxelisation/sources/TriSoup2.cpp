#include "TriSoup2.h"
#include <scene3d/Tri.h>
#include <Message.h>

void TriSoup2::fitToUnitSphere() {
    for(int i=0; i<nv; i++) {
        vertex[i] = (vertex[i] - this->getCenter()) / this->getRadius();
    }
}

void TriSoup2::freeMemory() {
  	delete[] vertex;
  	delete[] normal;
  	delete[] tri;
}
