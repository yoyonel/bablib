#ifndef __LOADER_PLY__
#define __LOADER_PLY__

#include "../TriSoup.h"

class LoaderPLY : public TriSoupLoader {
    public:
        virtual QString suffix() const { return "PLY"; }
        virtual TriSoup* loadRaw(QString fileName) const;
    };

#endif
