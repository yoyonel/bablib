#include "Random.h"
#undef RAND_MAX
#undef random
//#include <stdlib.h>
#include <cstdlib>
#include <math.h>
BABLIB_NAMESPACE_USE

//@ a voir : sous linux stdlib.h contient une fonction random() a priori plus précise que rand() (retourne un long int au lieu d'un int), mais pas sous windows
#undef RANDOM
//#define RANDOM() random()
#define RANDOM() rand()

int Random::integer(int n) {
    unsigned long k = n * static_cast<unsigned long>(RANDOM());
    unsigned long m = 1 + static_cast<unsigned long>(RAND_MAX);
    return static_cast<int>(k / m);
    }

int Random::integer(int min, int max) {
    return min + integer(max - min);
    }

float Random::unif() {
    return static_cast<float>(static_cast<double>(RANDOM()) / static_cast<double>(RAND_MAX));
    }

float Random::unif(float min, float max) {
    return min + (max - min) * unif();
    }

double Random::unifOpen() {
    unsigned int k = static_cast<unsigned int>(RANDOM());
    unsigned int m = static_cast<unsigned int>(RAND_MAX) + 1;
    return static_cast<double>(k) / static_cast<double>(m);
    }

float Random::norm() {
    return sqrtf(-2 * logf(unif())) * cosf(2 * M_PI * unif());
    }

float Random::norm(float mu, float sigma) {
    return mu + sigma * norm();
    }


//@ pour fonctions couleurs random
/*
//QColor col = QColor::fromHsvF(Vec3::random(0.0,0.7).x,1.0f, 1.0f);
QColor col = QColor::fromHsvF(Vec3::random(0.0,0.3).x,1.0f, 1.0f);
//QColor col = QColor::fromHsvF(Vec3::random(0,1).x,Vec3::random(0.1,0.8).x, 1.0f);
Vec3 c = Vec3(col.redF(), col.greenF(), col.blueF());
*/


