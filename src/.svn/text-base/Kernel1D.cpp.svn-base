#include "Kernel1D.h"
#include <iostream>     // pour NULL
BABLIB_NAMESPACE_USE

Kernel1D::Kernel1D(int R) : R(R), tab(NULL) {
    N = 2 * R + 1;
    if (tab == NULL && R >= 0) tab = new float[N];
    }

void Kernel1D::destroy() {
    if (tab == NULL) return;
    delete tab;
    tab = NULL;
    }
        
Kernel1D Kernel1D::gaussianKernel() {
    Kernel1D kernel(1);
    kernel(-1) = 1;
    kernel( 0) = 2;
    kernel( 1) = 1;
    return kernel;
    }

