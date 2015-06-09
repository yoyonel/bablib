#ifndef __LOADER_OFF__
#define __LOADER_OFF__

#include "../TriSoup.h"

class LoaderOFF : public TriSoupLoader {
    virtual QString suffix() const { return "OFF"; }
    virtual TriSoup* loadRaw(QString fileName) const;
    
    public:
        // extra functionnality:
        void save(const TriSoup *ts, QString fileName) const;   // saves the trisoup in *binary* OFF format
    };

#endif
