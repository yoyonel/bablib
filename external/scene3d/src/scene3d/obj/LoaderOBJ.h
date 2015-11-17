#ifndef __LOADER_OBJ__
#define __LOADER_OBJ__

#include "../TriSoup.h"

class LoaderOBJ : public TriSoupLoader {
    public:
        virtual QString suffix() const { return "OBJ"; }
        virtual TriSoup* loadRaw(QString fileName) const;
    };

#endif
