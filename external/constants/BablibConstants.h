#ifndef __BABLIB_CONSTANTS__
#define __BABLIB_CONSTANTS__

struct BablibConstants {
    //static const QString default_params_file;
    // these constants are written as functions to avoid potential "static initialization fiasco"
    // (e.g. static member of other classes depending on these constants but initialized before this class)
    static const char* default_params_file();
    static const char* global_params_file();
    static const char* global_data_path();
    static const char* global_qprog_path();
    };

#endif

