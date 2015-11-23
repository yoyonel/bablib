#ifndef __IMAGE_FUNCTIONS__
#define __IMAGE_FUNCTIONS__

//#include <Message.h>

BABLIB_NAMESPACE_BEGIN

// inline supprime les warnings GCC sur les fonctions non utilisées
// url: http://stackoverflow.com/questions/386220/how-can-i-hide-defined-but-not-used-warnings-in-gcc
//      -> http://stackoverflow.com/a/5985068
namespace {
    inline int max(int a, int b) {
        return a > b ? a : b;
        }
    inline int min(int a, int b) {
        return a < b ? a : b;
        }
    inline int clamp(int x, int xmin, int xmax) {
        return min(max(x, xmin), xmax);
        }
    inline int modulo(int x, int m) {
        //int a = x >= 0 ? x % m : m + x % m;
        // on ne peut pas utiliser l'opérateur % sur une opérande négative car le comportement dans ce cas là
        // est défini comme dépendant de l'implémentation (!!) --> cf. http://en.wikipedia.org/wiki/Modulo_operation
        return x >= 0 ? x % m : (m - (-x) % m) % m;     //deuxième modulo nécessaire car m - (-k*m) % m = m
        }
    inline int mirror(int x, int m) {
        const int a = modulo(x, 2*m);
        return (a < m) ? a : 2*m - 1 - a;
        }
    }

BABLIB_NAMESPACE_END

#endif

