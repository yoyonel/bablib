#ifndef __KERNEL_3D__
#define __KERNEL_3D__

BABLIB_NAMESPACE_BEGIN

class Kernel3D {
    public:
        //Kernel3D(int R = -1, int* tab = NULL);   // précondition : <R> >= 0 et {<tab> == NULL ou <tab> est de taille (2R+1)*(2R+1)*(2R+1)}
        Kernel3D(int R = -1);   // précondition : <R> >= 0 et {<tab> == NULL ou <tab> est de taille (2R+1)*(2R+1)*(2R+1)}
        void destroy();
        
        int radius() const { return R; }
        float& operator()(int i, int j, int k) { return tab[N*(N*(i+R) + (j+R)) + (k+R)]; }    // précondition : tab != NULL
        
        //@@ faire fonction convolution
        
        //static Kernel3D gaussianKernel(int R);
        //static Kernel3D gaussianKernel();   // noyau gaussien de rayon 1
        
    private:
        int R, N;
        float* tab;
    };

BABLIB_NAMESPACE_END

#endif

