#include "RoadmarkReader.hpp"
using namespace OrientationMATIS;

int Test_specif(RoadmarkReader::BandType strip, RoadmarkReader::TolType tol)
{
// 	EuclideanDistanceTo
	double larg1 = strip[0].EuclideanDistanceTo(strip[2]);
	double larg2 = strip[1].EuclideanDistanceTo(strip[3]);
	double long1 = strip[0].EuclideanDistanceTo(strip[1]);
	double long2 = strip[2].EuclideanDistanceTo(strip[3]);
	if (std::abs(long1) < tol[0] || std::abs(long1) > tol[1] ) return 0;
	if (std::abs(long2) < tol[0] || std::abs(long2) > tol[1] ) return 0;
	if (std::abs(larg1) < tol[2] || std::abs(larg1) > tol[3] ) return 0;
	if (std::abs(larg2) < tol[2] || std::abs(larg2) > tol[3] ) return 0;
	return 1;
}






int RoadmarkReader::ReadDashedLine(std::string Nomfic, BandSetType & Bandset, TolType tol) {
	std::cout<<"nomfic : "<<Nomfic.c_str()<<std::endl;
	std::ifstream fic(Nomfic.c_str());
	fic.precision(10);
	if (!fic.good())
		return 0;
	int n;
	fic>>n; // number of lines
	std::cout<<"number of lines: "<<n<<std::endl;
	int temp;
	for (int i = 0; i < n; i++) {
		int nb_strip;
		fic>>nb_strip;
		for (int j = 0; j < nb_strip; j++) {
			fic >> temp;
			BandType mystrip;
			for (int k = 0; k < 4; k++) {
				PointType mypoint;
				double x, y, z;
				fic>>x>>y>>z;
				mypoint[0] = x;
				mypoint[1] = y;
				mypoint[2] = z;
				mystrip.push_back(mypoint);
			}
			if (Test_specif(mystrip, tol))	Bandset.push_back(mystrip);
		}
	}
	fic.close();
	return 1;
}


int RoadmarkReader::ReadZebraCrossing(std::string Nomfic, BandSetType & Bandset , TolType tol)
{
	std::cout<<"nomfic : "<<Nomfic.c_str()<<std::endl;
	std::ifstream fic(Nomfic.c_str());
	fic.precision(10);
	if (!fic.good())
		return 0;
	int counter = 0 ; 
	while (!fic.eof() )
	{
		counter++;
	
//		
		double temp,y,z;
		fic>>temp;
		std::cout<<"temp : "<<temp<<std::endl;
		if (fic.eof()) break;
		
		fic>>y;	
		if (fic.eof()) break;
		
		fic>>z;
		if (fic.eof()) break;
		
		std::cout<<"y,z "<<y<<" , "<<z<<std::endl;
		PointType pt;
		BandType mystrip;
		pt[0] = temp;	pt[1] = y;	pt[2] = z;
		// PointC1P1
		mystrip.push_back(pt);
		
		// Point C1P2,C2P1,C2P2
		for ( int m = 0 ; m < 3 ; m++)
		{
			if (fic.eof() /*|| counter > 9*/) return 1;
			std::cout<<" counter : "<<counter<<std::endl;
			fic>>temp;	fic>>y;	fic>>z;
			pt[0] = temp;	pt[1] = y;	pt[2] = z;
			mystrip.push_back(pt);			
		}
		for ( int m = 0 ; m < 4 ; m++)
			{
				//if (fic.eof() /*|| counter > 8*/) return 1;;
				//std::cout<<" counter : "<<counter<<std::endl;
				fic>>temp;	fic>>y;	fic>>z;
				//pt[0] = temp;	pt[1] = y;	pt[2] = z;
				//mystrip.push_back(pt);			
			}
		if (Test_specif(mystrip, tol))	Bandset.push_back(mystrip);
	}
	
	fic.close();
	return 1;	
}
	
	


int RoadmarkReader::Lambert93ToWGS84(BandSetType & Bandset) {
	// Creation de la transformation de coordonnees ...
	OGRSpatialReference oSourceSRS, oTargetSRS;
	OGRCoordinateTransformation *poCT;
	oSourceSRS.importFromProj4("+init=IGNF:LAMB93");
	oTargetSRS.SetWellKnownGeogCS("WGS84");
	poCT = OGRCreateCoordinateTransformation(&oSourceSRS, &oTargetSRS);

	for (int i = 0; i < Bandset.size(); i++)
		for (int j = 0; j < 4; j++)
			poCT->Transform( 1, &(Bandset[i][j][0]), &(Bandset[i][j][1]),
					&(Bandset[i][j][2]));
}

int RoadmarkReader::RelativeToAbsolute(BandSetType & Bandset, std::string nomori) {
	boost::shared_ptr<ModeleProjection> ori = ModeleProjection::ReadFile(nomori);
	const boost::shared_ptr<ModeleProjectionPhysique> mpp =
			boost::dynamic_pointer_cast<ModeleProjectionPhysique>(ori);
	if (!mpp) {return 0;} // A partir de là on sait q'on a un ModeleProjectionPhysique mpp
	const boost::shared_ptr<ModelePhysiqueExtrinseque> mpp_ext = mpp->GetExtrinseque();
	if (!mpp_ext ) return 0;
	//typedef itk::Matrix <double,3,3> MatrixType;
	typedef vnl_matrix_fixed<double,3,3> MatrixType;
	MatrixType R, Rt;

	for ( int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			R(i,j) = mpp_ext->GetRotation(i)[j];
		}
		std::cout<<R[i][0]<<" , "<<R[i][1]<<" , "<<R[i][2]<<std::endl;
	}
	vnl_matrix_fixed<double,3,1> T,Xrel,Xabs;
	T(0,0)= mpp_ext->GetSommet()[0];
	T(1,0)= mpp_ext->GetSommet()[1];
	T(2,0)= mpp_ext->GetSommet()[2];
	std::cout<<" avant transformation ! "<<std::endl;
	for (int i = 0; i < Bandset.size(); i++)
	for (int j = 0; j < 4; j++)
	{
		Xrel(0,0) = Bandset[i][j][0];
		Xrel(1,0)= Bandset[i][j][1];
		Xrel(2,0) = Bandset[i][j][2];
		Rt = R.transpose();
		std::cout<<" avant : Xabs = Rt * Xrel +T "<<std::endl;
		Xabs = Rt * Xrel +T;
		std::cout<<" Xabs = Rt * Xrel +T "<<std::endl;
		Bandset[i][j][0] = Xabs(0,0);
		Bandset[i][j][1] = Xabs(1,0);
		Bandset[i][j][2] = Xabs(2,0);
	}
	std::cout<<"fin de absolute"<<std::endl;

}
using kmldom::MultiGeometryPtr;
using kmldom::FolderPtr;
using kmldom::InnerBoundaryIsPtr;

using kmldom::LinearRingPtr;
using kmldom::LineStringPtr;

using kmldom::OuterBoundaryIsPtr;

using kmldom::KmlFactory;
using kmldom::PolygonPtr;
using kmldom::PlacemarkPtr;
using kmldom::CoordinatesPtr;
PlacemarkPtr RoadmarkReader::AddBandsed(KmlFactory* factory,
		BandSetType & Bandset) {
	CoordinatesPtr coordinates = factory->CreateCoordinates();
	PlacemarkPtr placemark = factory->CreatePlacemark();
	placemark->set_name("MultiGeometry");
	placemark->set_id("pmmg");
	MultiGeometryPtr multigeometry = factory->CreateMultiGeometry();
	placemark->set_geometry(multigeometry);

	for (int i = 0; i < Bandset.size(); i++) {
		CoordinatesPtr coordinates = factory->CreateCoordinates();
		coordinates->add_latlngalt(Bandset[i][0][1], Bandset[i][0][0], 0);
		coordinates->add_latlngalt(Bandset[i][1][1], Bandset[i][1][0], 0);
		coordinates->add_latlngalt(Bandset[i][3][1], Bandset[i][3][0], 0);
		coordinates->add_latlngalt(Bandset[i][2][1], Bandset[i][2][0], 0);
		coordinates->add_latlngalt(Bandset[i][0][1], Bandset[i][0][0], 0);

		LinearRingPtr linearring = factory->CreateLinearRing();
		linearring->set_coordinates(coordinates);

		OuterBoundaryIsPtr outerboundaryis = factory->CreateOuterBoundaryIs();
		outerboundaryis->set_linearring(linearring);

		PolygonPtr polygon = factory->CreatePolygon();
		polygon->set_tessellate(false);
		polygon->set_altitudemode(kmldom::ALTITUDEMODE_RELATIVETOGROUND);
		polygon->set_outerboundaryis(outerboundaryis);

		multigeometry->add_geometry(polygon);
	}
	return placemark;

}

