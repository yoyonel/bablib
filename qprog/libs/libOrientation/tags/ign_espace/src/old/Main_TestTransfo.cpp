//#include <gdal/ogr_spatialref.h>
//#include <sstream>
//#include <stdexcept>
//#include "itkPoint.h"
//#include <fstream>
////#include "itkImage.h"
////#include <itkImageFileReader.h>
//
// 
//    
////#include "itkImageFileReader.h"
//int main2(void)
//{ 
////	typedef itk::Image <short,2> ImageType;
////	typedef itk::ImageFileReader <ImageType> readerType;
//	
//	std::ifstream fic("/media/Data/Paris12/absolut/passage4_translate.Seg3D");
//	std::ofstream fic_out("/media/Data/Paris12/absolut/passage_WGS.txt");
//	fic_out.precision(10); 
//	if (!fic.good())
//		std::cout<<"Problem de lecture de fichier! "<<std::endl;
//	typedef itk::Point <double,3> PointType;
//	std::vector <PointType> vect;
//	while(!fic.eof())	
//	{
//		PointType p,p2;
//		double x,y,z;
//		fic>>x;	fic>>y;	fic>>z;
//		p[0] = x;	p[1] = y;	p[2] = z;
//		vect.push_back(p);
//		
//		fic>>x;	fic>>y;	fic>>z;
//		p[0] = x;	p[1] = y;	p[2] = z;
//		vect.push_back(p);
//		
//		fic>>x;	fic>>y;	fic>>z;
//		p2[0] = x;	p2[1] = y;	p2[2] = z;
//				
//		fic>>x;	fic>>y;	fic>>z;
//		p[0] = x;	p[1] = y;	p[2] = z;
//		vect.push_back(p);
//		vect.push_back(p2);
//		
//		fic>>x;	fic>>y;	fic>>z;
//		fic>>x;	fic>>y;	fic>>z;
//		fic>>x;	fic>>y;	fic>>z;
//		fic>>x;	fic>>y;	fic>>z;
//	}
//
//	 
//// Creation de la transformation de coordonnees ...
//    OGRSpatialReference oSourceSRS, oTargetSRS;
//    OGRCoordinateTransformation *poCT;
//    
//
//    oSourceSRS.importFromProj4( "+init=IGNF:LAMB93" );
//    oTargetSRS.SetWellKnownGeogCS( "WGS84" );
//
//    poCT = OGRCreateCoordinateTransformation(&oSourceSRS, &oTargetSRS);
// 
//    if (poCT == NULL)
//    {
//        std::ostringstream oss;
//        oss << "File : " << __FILE__ << std::endl;
//        oss << "Function : " << __FUNCTION__ << std::endl;
//        oss << "Line : " << __LINE__ << std::endl;
//        oss << "Error : " << "Unable to create transformation !!!" << std::endl;
//        throw std::logic_error( oss.str() );
//    }
// 
//
//    for(int i = 0 ; i < vect.size() ; i++)
//    {
//    	double x,y,z;
//    	x = vect[i][0];	y = vect[i][1];	z = vect[i][2];
//    	poCT->Transform( 1, &x, &y , &z);
//    	fic_out<<x<<","<<y<<","<<z<<std::endl;
//    }
//    
//    
//
//    
//    
//    
//    //    KML << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
//    //    KML << "<kml xmlns=\"http://earth.google.com/kml/2.2\">" << std::endl;
////    <Placemark>
////              <name>Building 40</name>
////              <visibility>0</visibility>
////              <styleUrl>#transRedPoly</styleUrl>
////              <Polygon>
////                <extrude>1</extrude>
////                <altitudeMode>relativeToGround</altitudeMode>
////                <outerBoundaryIs>
////                  <LinearRing>
////                    <coordinates> -122.0848938459612,37.42257124044786,17
//          
////                      -122.0848938459612,37.42257124044786,17 </coordinates>
////                  </LinearRing>
////                </outerBoundaryIs>
////              </Polygon>
////            </Placemark>
//// 
////    // On ecrit l'en tete
////    KML << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
////    KML << "<kml xmlns=\"http://earth.google.com/kml/2.2\">" << std::endl;
////    KML << "\t<Document>" << std::endl;
////
////// pour transformer
////x = orientation.Sommet()[0];
////        y = orientation.Sommet()[1];
////        z = orientation.Sommet()[2];
////
////        poCT->Transform( 1, &x, &y , &z);
////
////// .....
////
////KML << "\t</Document>" << std::endl;
////    KML << "</kml>";
//
//}  
//
////#include <iostream>
////#include <string>
//
//#include <iostream>
//#include <string>

//
//
//using kmldom::PolygonPtr;
//using std::stringstream;
//#include <iostream>
//#include <sstream>
//#include <string>
//
//
////static PlacemarkPtr CreateMultiGeometryPlacemark() {
////  KmlFactory* factory = KmlFactory::GetFactory();
////
////  PlacemarkPtr placemark = factory->CreatePlacemark();
////  placemark->set_name("MultiGeometry");
////  placemark->set_id("pmmg");
////  MultiGeometryPtr multigeometry = factory->CreateMultiGeometry();
////  placemark->set_geometry(multigeometry);
////
////  const int num_points = 3;
////  for (int i = 0; i < num_points; ++i) {
////    multigeometry->add_geometry(Create2dPoint(-120 + i, i, 37 + i));
////  }
////
////  return placemark;
////}
//
//
//
// PointPtr Create2dPoint(int id, double longitude, double latitude) {
//  KmlFactory* factory = KmlFactory::GetFactory();
//
//  CoordinatesPtr coordinates = factory->CreateCoordinates();
//  coordinates->add_latlng(longitude, latitude);
//
//  PointPtr point = factory->CreatePoint();
//  stringstream ss;
//  ss << "pt_" << id;
//  point->set_id(ss.str());
//  point->set_coordinates(coordinates);
//  return point;
//}
//
//
//
//
//
//int mainkml() {
//  // Get the factory singleton to create KML elements.
//  KmlFactory* factory = KmlFactory::GetFactory();
//
//  // Create <coordinates>.
//  CoordinatesPtr coordinates = factory->CreateCoordinates();
//  // Create <coordinates>-122.0816695,37.42052549<coordinates>
//  coordinates->add_latlng(37.42052549,-122.0816695);;
//  
//  
//  
//  // Create <Point> and give it <coordinates>.
//  PointPtr point = factory->CreatePoint();
//  point->set_coordinates(coordinates);  // point takes ownership
//
//  // Create <Placemark> and give it a <name> and the <Point>.
//  PlacemarkPtr placemark = factory->CreatePlacemark();
//  placemark->set_name("Cool Statue");
//  placemark->set_geometry(point);  // placemark takes ownership
//  
//  MultiGeometryPtr multigeometry = factory->CreateMultiGeometry();
//    placemark->set_geometry(multigeometry);
//    const int num_points = 3;
//     for (int i = 0; i < num_points; ++i) {
//       multigeometry->add_geometry(Create2dPoint(-120 + i, i, 37 + i));
//     }
////   
////  CoordinatesPtr coordinates1 = factory->CreateCoordinates();
////    coordinates1->add_latlng(0,0.0816695);
////  PointPtr point2 = factory->CreatePoint();
////   point2->set_coordinates(coordinates1); 
////  placemark->set_geometry(point2);
////  
//  // Create <kml> and give it <Placemark>.
//  KmlPtr kml = factory->CreateKml();
//  kml->set_feature(placemark);  // kml takes ownership.
//
//  // Serialize to XML
//  std::string xml = kmldom::SerializePretty(kml);
//
//  // Print to stdout
//  std::cout << xml;
//}
#include "RoadmarkReader.hpp"
#include <iomanip>
#include <iostream>
#include "modeleprojection.hpp" 
#include "StereopolisFileReader.hpp"
#include "kml/dom.h"
#include "boost/algorithm/string/trim.hpp" 
#include "itkPoint.h"

// libkml types are in the kmldom namespace
using kmldom::CoordinatesPtr;
using kmldom::KmlPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::MultiGeometryPtr;

using kmldom::FolderPtr;
using kmldom::InnerBoundaryIsPtr;

using kmldom::LinearRingPtr;
using kmldom::LineStringPtr;

using kmldom::OuterBoundaryIsPtr;

using namespace OrientationMATIS; 
int main(void)
{
	ModeleProjection::InitAllIO();
	KmlFactory* factory = KmlFactory::GetFactory();
//	PlacemarkPtr placemark = factory->CreatePlacemark();
//	MultiGeometryPtr multigeometry = factory->CreateMultiGeometry();
//	placemark->set_geometry(multigeometry);
	
//	multigeometry->add_geometry(Create2dPoint(-120 + i, i, 37 + i));
	
//	std::string nomfic = "/media/Data/Chantier_Pari12/2008_04_24/Paris_12-080424_0669-11-00001_0000071.Seg3D";
	
//	std::string nomfic = "Z:/Stereopolis/STEREOPOLIS/DATA/PARIS12/2008_04_24/Images_debayerisees/Paris_12-080424_0669-11-00001_0000001.tiff";
	std::string nomlist = "/home/bahman/Bureau/All.txt";
	std::string rep_resultat = "/media/Data/Chantier_Pari12/2008_04_24/Results/";
	std::vector <std::string> vect; 
	Stereopolis::Reader::ReadList(nomlist,vect);
	std::vector <std::string> list_ext;
	list_ext.push_back(".Seg3D");
	
	list_ext.push_back(".T2");
	list_ext.push_back(".T3");
	list_ext.push_back(".T'1");
	list_ext.push_back(".T'2");
	list_ext.push_back(".T'P");
	list_ext.push_back(".TU");
	
	typedef itk::Point <double,4> PointType; 
	std::vector < PointType > list_tol;
	
	// longueur min et max
	// largeur min et max
	PointType Tol;
	
	// Seg3D
	Tol[0] = 1;	Tol[1] = 8;	Tol[2] = 0.40; Tol[3]= 0.80;
	list_tol.push_back(Tol);
	
	// T2
	Tol[0] = 2.7;	Tol[1] = 3.2;	Tol[2] = 0.25; Tol[3]= 0.33;
	list_tol.push_back(Tol);
	
	// T3
	Tol[0] = 1.3; /*2.5*/	Tol[1] = 3.8;	Tol[2] = 0.09; Tol[3]= 0.25;
	list_tol.push_back(Tol);
	
	// T'1
	Tol[0] = 1.30;	Tol[1] = 3.8; /*1,70*/;	Tol[2] = 0.09; Tol[3]= 0.25;
	list_tol.push_back(Tol);
	
	// T'2
	Tol[0] = 0.40;	Tol[1] = 0.55;	Tol[2] = 0.40; Tol[3]= 0.55;
	list_tol.push_back(Tol);
	
	// T'P
	Tol[0] = 0.30;	Tol[1] = 0.60;	Tol[2] = 0.08; Tol[3]= 0.15;
	list_tol.push_back(Tol);
	
	// TU
	Tol[0] = 0.40;	Tol[1] = 0.60;	Tol[2] = 0.25; Tol[3]= 0.35;
	list_tol.push_back(Tol);	
	
	
	RoadmarkReader::BandSetType bandsetTotalChantier;
	KmlPtr kml = factory->CreateKml();
	for (int i = 0 ; i < vect.size()-1 ; i++)
	{
		std::cout<<"_________________________"<<std::endl;
//		std::string nomficres = vect[i];
		boost::algorithm::trim_left_if(vect[i],boost::is_any_of("Z:/"));
		vect[i] = "/media/nasarchi/" + vect[i];	
	
		
		std::cout <<" fichier sur reseau : "<<vect[i]<<std::endl;
		std::string ficimg = boost::filesystem::basename(vect[i]);
		std::cout<<" nom fichier image : "<<ficimg<<std::endl;
		
		RoadmarkReader::BandSetType bandsetTotal;
		for (int j = 0 ; j < list_ext.size() ; j++)
		{
			RoadmarkReader::BandSetType bandset;
			std::string nomstrip = rep_resultat + ficimg + list_ext[j];
			if (!j) RoadmarkReader::ReadZebraCrossing(nomstrip,bandset,list_tol[j]);
			else RoadmarkReader::ReadDashedLine(nomstrip,bandset,list_tol[j]);
			
			std::string nomori = Stereopolis::Reader::GetOrientationName(vect[i]);
			
			RoadmarkReader::RelativeToAbsolute(bandset,nomori);
			RoadmarkReader::Lambert93ToWGS84(bandset);
			for (int k = 0 ; k < bandset.size(); k++)
				bandsetTotal.push_back(bandset[k]);
					
		}
		for (int m = 0 ; m < bandsetTotal.size(); m++)
			bandsetTotalChantier.push_back(bandsetTotal[m]);
	
	}
	PlacemarkPtr placemark = RoadmarkReader::AddBandsed(factory,bandsetTotalChantier);
				kml->set_feature(placemark);
		
	std::string xml = kmldom::SerializePretty(kml); 
	std::ofstream ficout("/home/bahman/Bureau/All5.kml");
	ficout<<xml;
	
	ficout.close();
	
	
	
	
//	std::cout << xml;
	
	
	
	//	Stereopolis::Reader::GetOrientationName(nomfic);
	
	
	
	
	
	
//	RoadmarkReader::BandSetType bandset;
////	std::cout<<"lecture fic : "<<RoadmarkReader::ReadDashedLine(nomfic,bandset)<<std::endl;;
//	RoadmarkReader::ReadZebraCrossing(nomfic,bandset);
//	std::cout<<" nomber of bands: "<<bandset.size()<<std::endl;
//	
//		
//	std::string nomori33 = "/media/nasarchi/Stereopolis/STEREOPOLIS/DATA/PARIS12/2008_04_24/Record_Stereopolis_meta_datas/PosPac/session_669/section_1/Paris_12-080424_0669-11-00001_0000071.xml";
//	RoadmarkReader::RelativeToAbsolute(bandset,nomori33);
//	RoadmarkReader::Lambert93ToWGS84(bandset);
//
//	PlacemarkPtr placemark = RoadmarkReader::AddBandsed(factory,bandset);
//	
//	  KmlPtr kml = factory->CreateKml();
//	  kml->set_feature(placemark);  // kml takes ownership.
//
//	  // Serialize to XML
//	  std::string xml = kmldom::SerializePretty(kml);
//
//	  // Print to stdout
//	  std::cout << xml;
	
	
	
//	std::cout<<"number of bandes : "<<bandset.size()<<std::endl;
//	std::cout<<" x : "<< std::setprecision(10) <<bandset[4][3][0];
//	std::cout<<" y : "<< std::setprecision(10) <<bandset[4][3][1];
//	std::cout<<" z : "<< std::setprecision(10) <<bandset[4][3][2]<<std::endl;
	
//	std::string nomori33 = "/home/bahman/work/Test_ori/bertrand/Paris_12-080428_0807-33-00001_0000607.xml";
	
//	boost::shared_ptr<ModeleProjection> ori33 = ModeleProjection::ReadFile( nomori33);
//	std::cout<<"Lecture fini! "<<std::endl;
//	const std::string nomcam = (ori33->GetAuxiliaryData()).GetCameraName() ;
//	std::cout<<"cam name : "<<nomcam<< std::endl;
	 
}
