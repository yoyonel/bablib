#ifndef __IMAGE_FUNCTIONS__
#define __IMAGE_FUNCTIONS__

//#include <Message.h>

BABLIB_NAMESPACE_BEGIN

namespace {
	/*
    int max(int a, int b) {
        return a > b ? a : b;
        }
    int min(int a, int b) {
        return a < b ? a : b;
        }
	
    int clamp(int x, int xmin, int xmax) {
        return min(max(x, xmin), xmax);
        }
	*/	
    int modulo(int x, int m) {
        //int a = x >= 0 ? x % m : m + x % m;
        // on ne peut pas utiliser l'opérateur % sur une opérande négative car le comportement dans ce cas là
        // est défini comme dépendant de l'implémentation (!!) --> cf. http://en.wikipedia.org/wiki/Modulo_operation
        return x >= 0 ? x % m : (m - (-x) % m) % m;     //deuxième modulo nécessaire car m - (-k*m) % m = m
        }
    int mirror(int x, int m) {
        const int a = modulo(x, 2*m);
        return (a < m) ? a : 2*m - 1 - a;
        }
    }

BABLIB_NAMESPACE_END

#endif

