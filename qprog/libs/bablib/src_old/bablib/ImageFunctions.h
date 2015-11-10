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
        // on ne peut pas utiliser l'op�rateur % sur une op�rande n�gative car le comportement dans ce cas l�
        // est d�fini comme d�pendant de l'impl�mentation (!!) --> cf. http://en.wikipedia.org/wiki/Modulo_operation
        return x >= 0 ? x % m : (m - (-x) % m) % m;     //deuxi�me modulo n�cessaire car m - (-k*m) % m = m
        }
    int mirror(int x, int m) {
        const int a = modulo(x, 2*m);
        return (a < m) ? a : 2*m - 1 - a;
        }
    }

BABLIB_NAMESPACE_END

#endif

