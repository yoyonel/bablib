#ifndef __RANDOM__
#define __RANDOM__

BABLIB_NAMESPACE_BEGIN

//@ a ajouter : couleurs random

class Random {
    public:
        static int integer(int n);              // uniform on [0,n[
        static int integer(int min, int max);   // uniform on [min,max[
        
        static float unif();                        // uniform on [0,1]
        static float unif(float min, float max);    // uniform on [min,max]
    
        static double unifOpen();   // uniform on [0,1[
        // warning: converting back to float can give the 1.0f value
    
        static float norm();                        // standard normal distribution N(0,1)
        static float norm(float mu, float sigma);   // normal distribution N(m, sigma)

        static void seedFromClock();    // uses the clock as a seed for new random numbers sequnce
    };

BABLIB_NAMESPACE_END

#endif

