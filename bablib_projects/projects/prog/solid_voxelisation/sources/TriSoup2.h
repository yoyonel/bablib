#ifndef __TRIP_SOUP2__
#define __TRIP_SOUP2__


#include <scene3d/TriSoup.h>
#include <QString>


class Tri;
class TriSoupLoader;

///
/// \brief The TriSoup2 class
///
class TriSoup2 : public TriSoup {    
public:
    ///
    /// \brief load
    /// \param fileName
    /// \return
    ///
    static TriSoup2* load(QString fileName) { return static_cast<TriSoup2*>(TriSoup::load(fileName)); }
    
    ///
    /// \brief fitToUnitSphere
    ///
    void fitToUnitSphere();

protected:
    ///
    /// \brief freeMemory
    ///
    void freeMemory();
};

#endif
