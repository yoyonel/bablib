#ifndef __KERNEL_1D__
#define __KERNEL_1D__

BABLIB_NAMESPACE_BEGIN

class Kernel1D {
    public:
        //Kernel1D(int R = -1, int* tab = NULL);   // précondition : <R> >= 0 et {<tab> == NULL ou <tab> est de taille (2R+1)*(2R+1)}
        Kernel1D(int R = -1);   // précondition : <R> >= 0 et {<tab> == NULL ou <tab> est de taille (2R+1)}
        void destroy();
        
        int radius() const { return R; }
        float& operator()(int i) { return tab[i+R]; }    // précondition : tab != NULL
        
        //@@ faire fonction convolution
        
        //static Kernel1D gaussianKernel(int R);
        static Kernel1D gaussianKernel();   // noyau gaussien de rayon 1
        
    private:
        int R, N;
        float* tab;
    };

BABLIB_NAMESPACE_END

#endif

