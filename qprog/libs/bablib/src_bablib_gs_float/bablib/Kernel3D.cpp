#include "Kernel3D.h"
#include <iostream>     // pour NULL
BABLIB_NAMESPACE_USE

Kernel3D::Kernel3D(int R) : R(R), tab(NULL) {
    N = 2 * R + 1;
    if (tab == NULL && R >= 0) tab = new float[N * N * N];
    }

void Kernel3D::destroy() {
    if (tab == NULL) return;
    delete tab;
    tab = NULL;
    }
        
/*
Kernel3D Kernel3D::gaussianKernel() {
    Kernel3D kernel(1);
    kernel(-1,-1,-1) = 1; kernel( 0,-1,-1) = 2; kernel( 1,-1,-1) = 1;
    kernel(-1, 0,-1) = 2; kernel( 0, 0,-1) = 4; kernel( 1, 0,-1) = 2;
    kernel(-1, 1,-1) = 1; kernel( 0, 1,-1) = 2; kernel( 1, 1,-1) = 1;

    kernel(-1,-1, 0) = 1; kernel( 0,-1, 0) = 2; kernel( 1,-1, 0) = 1;
    kernel(-1, 0, 0) = 2; kernel( 0, 0, 0) = 4; kernel( 1, 0, 0) = 2;
    kernel(-1, 1, 0) = 1; kernel( 0, 1, 0) = 2; kernel( 1, 1, 0) = 1;

    kernel(-1,-1, 1) = 1; kernel( 0,-1, 1) = 2; kernel( 1,-1, 1) = 1;
    kernel(-1, 0, 1) = 2; kernel( 0, 0, 1) = 4; kernel( 1, 0, 1) = 2;
    kernel(-1, 1, 1) = 1; kernel( 0, 1, 1) = 2; kernel( 1, 1, 1) = 1;

    return kernel;
    }
*/
