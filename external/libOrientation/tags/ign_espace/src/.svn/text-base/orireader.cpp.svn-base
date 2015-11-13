#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>

#include "noboost.hpp"

#include "modeleprojectionconique.hpp"
#include "modeleprojectionaxono.hpp"
#include "modeleprojectioncarto.hpp"
#include "distortionpolynomeign.hpp"
#include "distortiongrillematis.hpp"

namespace OrientationMATIS
{
	shared_ptr<ModeleProjection> axono_ori_reader(const std::string &filename)
	{
	  std::ifstream fic(filename.c_str());
	  if (!fic.good()) 
	  	return shared_ptr<ModeleProjection>();

	  std::string typFic;
	  fic >> typFic;
	  if ( typFic != std::string("AXONO")  )
	  	return shared_ptr<ModeleProjection>();
	 

	  std::string systemeGeodesie;
	  // position centre repere local coordonnees
	  double localOrigin[2];
	  fic >> localOrigin[0] >> localOrigin[1];
	  // numero de la zome lambert
	  int zoneLambert;
	  fic >> zoneLambert;
	  localOrigin[0] /= 1000.0 ;
	  localOrigin[1] /= 1000.0 ;
	  
	  if (localOrigin[1] > 1000000.0) 
	  {
		  localOrigin[1] -= zoneLambert*1000000.0;
	  }
	  
	  std::ostringstream nomSysteme;
	  nomSysteme.precision(10);
	  nomSysteme << "MATISRTL " << localOrigin[0] << " " << localOrigin[1] << " " << Geodesie::NomSysteme(zoneLambert);

	  // taille de l'image
	  unsigned int nc, nl;
	  fic >> nc >> nl;
	  
	  double sommet[3], rotation[3][3];
	  // position 3D d'un point de l'image
	  fic >> sommet[0] >> sommet[1]>> sommet[2];
	  // Axe secondaire colonne image
	  fic >> rotation[0][0] >> rotation[0][1]>> rotation[0][2];
	  // Axe secondaire ligne image
	  fic >> rotation[1][0] >> rotation[1][1]>> rotation[1][2];
	  // Axe Principal
	  fic >> rotation[2][0] >> rotation[2][1]>> rotation[2][2];
	  for (unsigned int i=0; i<3;++i)
	  {
	  	sommet[i] /= 1000.0;
	  	for (unsigned int j=0;j<3;++j)
	  		rotation[i][j] /= 1000000000.0;
	  }
 	  ModelePhysiqueExtrinseque extr(sommet, rotation);

	  // image du point defini en 3D precedemment
	  double cPPA, lPPA;
	  fic >> cPPA >> lPPA;
	  cPPA /= 1000.0 ;
	  lPPA /= 1000.0 ;

	  // Echelle
	  double cPas, lPas;
	  fic >> cPas;	 
	  cPas /= 1000.0;
	  lPas = cPas;
	  
	  fic.close();
	  
	  AuxiliaryData data;
	  Transfo2D transfo2D;
	  return shared_ptr<ModeleProjection> (new ModeleProjectionAxono( nc, nl,
								data, transfo2D, systemeGeodesie,
								extr, 
								cPas, lPas, cPPA, lPPA) );
	}

	shared_ptr<ModeleProjection> carto_hdr_reader(const std::string &filename)
	{
		/* Exemple 1 
		byteorder I
		nbits 16
		xdim 10.000000
		ydim 10.000000
		ncols 700
		nrows 670
		nbands 1
		Datum AGD66
		UTM Zone 55
		mapunits METERS
		ulxmap 383805.000000
		ulymap 5402695.000000 
		*/
		/* Exemple 2
		nrows 1441
		ncols 1441
		nbands 1
		nbits 16
		layout bsq
		byteorder I
		ulxmap -9024309
		ulymap 9024309
		xdim 12533.7625
		ydim 12533.7625
		*/ 
		std::ifstream fic(filename.c_str());
		if (!fic.good()) 
			return shared_ptr<ModeleProjection>();

		unsigned int nc,nl;
		double pas_x, pas_y;
		double position_origine_x,position_origine_y;
		std::string systemeGeodesie;
		
		unsigned short good = 0;
		
		while(fic.good())
		{
			std::string s;
			fic >> s;
			if(!fic.good()) 
		  		break;
			to_upper(s);
		  	
//			if ( s == "MAPUNITS") { fic >> s; }
//			else if( s == "METERS") { fic >> s; }
//			else 
			if( s == "ULXMAP") { fic >> position_origine_x ; good |= 1;}
			else if( s == "ULYMAP") { fic >> position_origine_y; good |=2; }
			else if( s == "XDIM") { fic >> pas_x; good |= 4;}
			else if( s == "YDIM") { fic >> pas_y; good |= 8;}
			else if( s == "NROWS") { fic >> nc; good |= 16;}
			else if( s == "NCOLS") { fic >> nl; good |= 32;}
			else if( s == "PROJECTION") 
			{ 
				std::string systeme;
				fic >> systeme;
				std::cout << "Fichier HDR, projection lue : " << systeme << std::endl;			
					
				if (systeme == "LAMBERT1")
					systemeGeodesie = "+init=IGNF:LAMB1C";
				else if(systeme == "LAMBERT2")
					systemeGeodesie = "+init=IGNF:LAMB2C";
				else if(systeme == "LAMBERT3")
					systemeGeodesie = "+init=IGNF:LAMB3C";
				else if(systeme == "LAMBERT4")
					systemeGeodesie = "+init=IGNF:LAMB4C";
				else if(systeme == "LAMBERT93")
					systemeGeodesie = "+init=IGNF:LAMB93";
				else
					std::cout << "Projection inconnue : non convertie en projection PROJ4" << std::endl;
			}

			getline(fic, s);
		}
		
		if (good != 63) 
			return shared_ptr<ModeleProjection>();

		position_origine_x += 0.5*pas_x;
		position_origine_y -= 0.5*pas_y;
		
		AuxiliaryData data;
		Transfo2D transfo2D;
	  
		return shared_ptr<ModeleProjection> (new ModeleProjectionCarto(nc, nl, data, 
				transfo2D, systemeGeodesie, 
				position_origine_x, position_origine_y, pas_x, pas_y) );
	}

	shared_ptr<ModeleProjection> carto_ori_reader(const std::string &filename)
	{
		std::ifstream fic(filename.c_str());
		if (!fic.good()) 
			return shared_ptr<ModeleProjection>();

		// Verification du type du fichier
		std::string s;
		fic >> s;
		if ( s != std::string("CARTO") ) 
			return shared_ptr<ModeleProjection>();
		
		
		double position_origine_x, position_origine_y;
		fic >> position_origine_x >> position_origine_y;
		position_origine_x /= 1000.0;
		position_origine_y /= 1000.0;

		int zoneLambert;
		fic >> zoneLambert;
		if (position_origine_x > 1000000.0) 
			position_origine_x -= zoneLambert*1000000.0;

		std::string systemeGeodesie = Geodesie::NomSysteme(zoneLambert);
		unsigned int nc, nl;
		fic >> nc >> nl;
		
		double pas_x, pas_y;
		fic >> pas_x >> pas_y;
		pas_x /= 1000.0;
		pas_y /= 1000.0;

		fic.close();
	  
		AuxiliaryData data;
		Transfo2D transfo2D;

		return shared_ptr<ModeleProjection> (new ModeleProjectionCarto(nc, nl, data, 
				transfo2D, systemeGeodesie, 
				position_origine_x, position_origine_y, pas_x, pas_y) );
	}
	
	shared_ptr<ModeleProjection> conic_ori_reader(const std::string &filename)
	{
	//	std::cout << " conic_ori_reader " << "\n";
		std::ifstream fic(filename.c_str());
		if (!fic.good())
			return shared_ptr<ModeleProjection>();
	//	std::cout << "Fichier est bon" << std::endl;
		char typFic[5];
		fic >> typFic;
	//	std::cout << "typfic =" << typFic <<")"<< std::endl;
		if (::strcmp(typFic, "TEXT") != 0)
			return shared_ptr<ModeleProjection>();
	//	std::cout << "Fichier TEXT" << std::endl;
		int distor;
		fic >> distor;
		if ((distor != 0) && (distor != 1) && (distor != 2))
			return shared_ptr<ModeleProjection>();

		int refraction;
		fic >> refraction;

		double temp;
		char chambre[9];
		bool chambre_is_valid = true;
		fic >> temp;
		chambre[0] = (char) temp;
		chambre_is_valid = chambre_is_valid && isalnum(chambre[0]);
		fic >> temp;
		chambre[1] = (char) temp;
		chambre_is_valid = chambre_is_valid && isalnum(chambre[1]);
		fic >> temp;
		chambre[2] = (char) temp;
		chambre_is_valid = chambre_is_valid && isalnum(chambre[2]);
		fic >> temp;
		chambre[3] = (char) temp;
		chambre_is_valid = chambre_is_valid && isalnum(chambre[3]);
		fic >> temp;
		chambre[4] = (char) temp;
		chambre_is_valid = chambre_is_valid && isalnum(chambre[4]);
		fic >> temp;
		chambre[5] = (char) temp;
		chambre_is_valid = chambre_is_valid && isalnum(chambre[5]);
		fic >> temp;
		chambre[6] = (char) temp;
		chambre_is_valid = chambre_is_valid && isalnum(chambre[6]);
		fic >> temp;
		chambre[7] = (char) temp;
		chambre_is_valid = chambre_is_valid && isalnum(chambre[7]);
		chambre[8] = 0;

		int jour, mois, annee, heure, minute, seconde;
		fic >> jour >> mois >> annee >> heure >> minute >> seconde;

		double altiSol;
		fic >> altiSol;
		altiSol /= 1000.0;

		double localOrigin[2];
		fic >> localOrigin[0] >> localOrigin[1];
		localOrigin[0] /= 1000.0;
		localOrigin[1] /= 1000.0;

		int zoneLambert;
		fic >> zoneLambert;
		if ((zoneLambert >= 1) && (zoneLambert <= 4))
			if (localOrigin[1] > 1000000.0)
				localOrigin[1] -= zoneLambert * 1000000.0;

		double sommet[3];
		fic >> sommet[0] >> sommet[1] >> sommet[2];
		sommet[0] /= 1000.0;
		sommet[1] /= 1000.0;
		sommet[2] /= 1000.0;

		double focal;
		fic >> focal;
		focal /= 1000.0;

		double rotation[3][3];
		double *iVect = rotation[0];
		fic >> iVect[0] >> iVect[1] >> iVect[2];
		iVect[0] /= 1000000000.0;
		iVect[1] /= 1000000000.0;
		iVect[2] /= 1000000000.0;

		double *jVect = rotation[1];
		fic >> jVect[0] >> jVect[1] >> jVect[2];
		jVect[0] /= 1000000000.0;
		jVect[1] /= 1000000000.0;
		jVect[2] /= 1000000000.0;

		double *kVect = rotation[2];
		fic >> kVect[0] >> kVect[1] >> kVect[2];
		kVect[0] /= 1000000000.0;
		kVect[1] /= 1000000000.0;
		kVect[2] /= 1000000000.0;

		double pixSize[2];
		fic >> pixSize[0] >> pixSize[1];
		pixSize[0] /= 1000000000.0;
		pixSize[1] /= 1000000000.0;

		unsigned int imNs, imNl;
		fic >> imNs >> imNl;

		double PPxy[2];
		fic >> PPxy[0] >> PPxy[1];
		PPxy[0] /= 1000.0;
		PPxy[1] /= 1000.0;

		ModelePhysiqueExtrinseque extr(sommet, rotation);
		
		//Bahman 19 sep 2008
		// AuxiliaryData
		AuxiliaryData aux;
		if ((mois != 0) && (jour != 0))
			aux.SetImageDate(annee, mois, jour, heure, minute, seconde);
		if (chambre_is_valid && (std::string(chambre) != "UNKNOWN"))
			aux.SetCameraName(chambre);
		
		//SystemeGeodesie
		std::ostringstream nomSysteme;
		if (zoneLambert != 0)
		{
			nomSysteme.precision(10);
			nomSysteme << "MATISRTL " << localOrigin[0] << " " << localOrigin[1] << " " << Geodesie::NomSysteme(zoneLambert);
		}
		
	//	if (chambre_is_valid && (std::string(chambre) != "UNKNOWN"))
	//		res->GetAuxiliaryData().SetCameraName(chambre);
	//	if ((mois != 0) && (jour != 0))
	//		res->GetAuxiliaryData().SetImageDate(annee, mois, jour, heure, minute, seconde);

	//	std::ostringstream nomSysteme;
	//	nomSysteme.precision(10);
	//	nomSysteme << "MATISRTL " << localOrigin[0] << " " << localOrigin[1] << " " << InternalOrientation::NomSysteme(zoneLambert);
	//	res->SetGeodeticSystemName(nomSysteme.str());
	//	//std::cout << "Lecture distortion" << std::endl;
		Transfo2D t2D;
		
		if (distor == 1)
		{

			int gTaille;
			fic >> gTaille;
			long gns, gnl, gstep;
			fic >> gns >> gnl >> gstep;
			if (gTaille > gns * gnl)
				return shared_ptr<ModeleProjection>();
			Grille2D * mGround2Photo = new Grille2D(gns, gnl, gstep);
			if (mGround2Photo == 00)
				return shared_ptr<ModeleProjection>();

			long ii;
			double *theDx = mGround2Photo->GetDx();
			double *theDy = mGround2Photo->GetDy();
			for (ii = 0; ii < gTaille; ii++)
			{
				fic >> theDx[ii] >> theDy[ii];
				theDx[ii] /= 1000000.0;
				theDy[ii] /= 1000000.0;
			}

			fic >> gns >> gnl >> gstep;
			if (gTaille > gns * gnl)
				return shared_ptr<ModeleProjection>();

			shared_ptr<Transfo2DElementaire> tr = shared_ptr<Transfo2DElementaire>(new DistortionGrilleMATIS(mGround2Photo, 00, true));
			t2D.AddTransformation2D(tr);
		}
		else if (distor == 2)
		{
			double xPPS, yPPS, cr3, cr5, cr7;
			fic >> cr3 >> cr5 >> cr7;
			fic >> xPPS >> yPPS;
			shared_ptr<Transfo2DElementaire> tr = shared_ptr<Transfo2DElementaire> (new DistortionPolynomeIGN(xPPS, yPPS, cr3, cr5, cr7, true));
			t2D.AddTransformation2D(tr);
		}
	//	std::cout << " fin de conic_ori_reader!" << std::endl;
		
	//	shared_ptr<ModeleProjection> mp = shared_ptr<ModeleProjection>( 
	//				new ModeleProjectionPhysique(imNs, imNl, extr, intr) );
	//	shared_ptr<InternalOrientation> res = shared_ptr<InternalOrientation >( new InternalOrientation( mp ) );

		shared_ptr<ModeleProjection> mp = shared_ptr<ModeleProjection>( 
			new ModeleProjectionConique(imNs, imNl, 
											aux, t2D, nomSysteme.str(),
											extr, 
											focal, PPxy[0], PPxy[1]) );


		return mp;
	}
};
