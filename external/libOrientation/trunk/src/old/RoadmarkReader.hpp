#ifndef __ROADMARKREADER_HPP__
#define __ROADMARKREADER_HPP__
#include "itkPoint.h"
#include "modeleprojection.hpp" 
#include <fstream>
#include <gdal/ogr_spatialref.h>
#include "itkMatrix.h"
#include "modeleprojectionphysique.hpp"
#include "kml/dom.h"
using kmldom::MultiGeometryPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;

namespace RoadmarkReader
{
typedef itk::Point <double , 3> PointType;
typedef std::vector <PointType> BandType;
typedef std::vector < BandType > BandSetType;
typedef itk::Point <double,4> TolType; 


int ReadDashedLine(std::string Nomfic, BandSetType & Bandset, TolType tol );
int ReadZebraCrossing(std::string Nomfic, BandSetType & Bandset, TolType tol); 
int Lambert93ToWGS84(BandSetType & Bandset);
int RelativeToAbsolute(BandSetType & Bandset,std::string nomori);

PlacemarkPtr AddBandsed(KmlFactory* factory,BandSetType & Bandset);
}






#endif
