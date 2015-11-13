#ifndef __KERNEL_2D__
#define __KERNEL_2D__

BABLIB_NAMESPACE_BEGIN

class Kernel2D {
    public:
        //Kernel2D(int R = -1, int* tab = NULL);   // prÃÂ©condition : <R> >= 0 et {<tab> == NULL ou <tab> est de taille (2R+1)*(2R+1)}
        Kernel2D(int R = -1);   // prÃÂ©condition : <R> >= 0
        void destroy();
        
        int radius() const { return R; }
        float& operator()(int i, int j) { return tab[N*(i+R) + (j+R)]; }    // prÃÂ©condition : tab != NULL
        bool contains(int i, int j) const { return i >= -R && i <= R && j >= -R && j <= R; }
                
        //@@ faire fonction convolution
        friend Kernel2D operator*(const Kernel2D a, const Kernel2D b);
        
        static Kernel2D constantKernel(int R, float value);
        static Kernel2D constantKernel(int R);  // valeur choisi pour que le noyau soit normalisÃÂ©
        //static Kernel2D gaussianKernel(int R);
        static Kernel2D gaussianKernel();   // noyau gaussien de rayon 1
        
        void normalize();
        void fill(float v);
        
    private:
        int R, N;
        float* tab;
    };

BABLIB_NAMESPACE_END

#endif

