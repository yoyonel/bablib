#ifndef PJ_INIT_H
#define PJ_INIT_H


#include <projects.h>

#ifdef __cplusplus
extern "C" {
#endif
 paralist * get_opt(paralist **start, FILE *fid, char *name, paralist *next);
 paralist * get_defaults(paralist **start, paralist *next, char *name);
 paralist * get_init(paralist **start, paralist *next, char *name);
#ifdef __cplusplus
}
#endif

#endif /* end of init header */

