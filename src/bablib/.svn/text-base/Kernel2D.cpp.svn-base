#include "Kernel2D.h"
#include <iostream>     // pour NULL
BABLIB_NAMESPACE_USE

Kernel2D::Kernel2D(int R) : R(R), tab(NULL) {
    N = 2 * R + 1;
    if (R >= 0) tab = new float[N * N];
    }

void Kernel2D::destroy() {
    if (tab == NULL) return;
    delete tab;
    tab = NULL;
    }
/*
Kernel2D operator*(const Kernel2D a, const Kernel2D b) {
    const int r = a.radius() + b.radius();
    Kernel2D res = constantKernel(R, 0);
    //@@ attention : inversion d'un des deux kernels ?
    }
*/
void Kernel2D::fill(float v) {
    for (int c=0; c<N*N; c++) tab[c] = v;
    }

void Kernel2D::normalize() {
    float s = 0;
    for (int c=0; c<N*N; c++) s += tab[c];
    if (s != 0)
        for (int c=0; c<N*N; c++) tab[c] /= s;
    }

Kernel2D Kernel2D::constantKernel(int R, float value) {
    Kernel2D kernel(R);
    kernel.fill(value);
    return kernel;
    }

Kernel2D Kernel2D::constantKernel(int R) {
    return constantKernel(R, 1.0f / (float(R) * float(R)));
    }

Kernel2D Kernel2D::gaussianKernel() {
    Kernel2D kernel(1);
    kernel(-1,-1) = 1; kernel( 0,-1) = 2; kernel( 1,-1) = 1;
    kernel(-1, 0) = 2; kernel( 0, 0) = 4; kernel( 1, 0) = 2;
    kernel(-1, 1) = 1; kernel( 0, 1) = 2; kernel( 1, 1) = 1;
    //@@ a voir : normaliser ?
    return kernel;
    }


