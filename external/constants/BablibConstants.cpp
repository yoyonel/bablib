#include "BablibConstants.h"

#define STRINGIZE_I(x)# x
#define STRINGIZE(x) STRINGIZE_I(x)
//#define STRINGCAT_I(x,y) x y
//#define STRINGCAT(x,y) STRINGCAT_I(x,y)

/*
#ifdef BABLIB_DEFAULT_PARAMS_FILE
    const char* BablibConstants::default_params_file() { return STRINGIZE(BABLIB_DEFAULT_PARAMS_FILE); }
#else
    const char* BablibConstants::default_params_file() { return "default.par"; }
#endif
*/
const char* BablibConstants::default_params_file() { return STRINGIZE(BABLIB_DEFAULT_PARAMS_FILE); }

//const char* BablibConstants::global_params_file = "bablib.par";
//const char* BablibConstants::global_params_file = STRINGCAT(STRINGIZE(BABLIB_GLOBAL_DATA_PATH), "/bablib.par");
//const char* BablibConstants::global_params_file() { return STRINGIZE(BABLIB_GLOBAL_DATA_PATH) "/bablib.par"; }
const char* BablibConstants::global_params_file() { return STRINGIZE(BABLIB_GLOBAL_PARAMS_FILE); }
const char* BablibConstants::global_data_path() { return STRINGIZE(BABLIB_GLOBAL_DATA_PATH); }
const char* BablibConstants::global_qprog_path() { return STRINGIZE(BABLIB_GLOBAL_QPROG_PATH); }

