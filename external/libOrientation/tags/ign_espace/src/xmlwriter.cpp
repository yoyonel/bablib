#include <iostream>
#include <sstream>
#include <iomanip>

#include "noboost.hpp"

#include "modeleprojection.hpp"
#include "tinyxml.h"
#include "outil_xml.hpp"
#include "modeleprojectionconique.hpp"
#include "modeleprojectionspherique.hpp"
#include "distortionpolynomeign.hpp"
#include "distortiongrillematis.hpp"
#include "homographie2d.hpp"
#include "projectivetransfo2D.hpp"

namespace OrientationMATIS
{
	
void write_distortion_polynome(TiXmlElement * h_sensor,	shared_ptr<DistortionPolynomeIGN> dp);


bool spheric_xml_writer(const shared_ptr<ModeleProjectionSpherique> mps, const std::string & filename)
{
	return false;
}

bool conic_xml_writer(const shared_ptr<ModeleProjectionConique> mpc, const std::string & filename)
{
		// A partir de lÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ  on sait q'on a un ModeleProjectionConique mpc
//	std::cout<<"cast bien passe!"<<std::endl;

//std::cout << "Temp_OriXmlWriter fichier " << filename<<std::endl;
	TiXmlDocument doc;
	TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "");
	doc.LinkEndChild(decl);

	TiXmlElement * h_orientation = new TiXmlElement("orientation");
	doc.LinkEndChild(h_orientation);

	TiXmlElement *h_version = new TiXmlElement("version");
	h_orientation->LinkEndChild(h_version);

	h_version->LinkEndChild(new TiXmlText("1.0"));

	/** Writing auxiliary data */
	TiXmlElement* h_aux = new TiXmlElement("auxiliarydata");
	h_orientation->LinkEndChild(h_aux);

	TiXmlElement *h_image_name = new TiXmlElement("image_name");
	h_image_name->LinkEndChild(new TiXmlText("UNKNOWN"));
	h_aux->LinkEndChild(h_image_name);

	TiXmlElement * h_stereopolis = new TiXmlElement("stereopolis");
	h_stereopolis->LinkEndChild(new TiXmlText("UNKNOWN"));
	h_aux->LinkEndChild(h_stereopolis);

//	std::cout << "av date" << std::endl;
	TiXmlElement * h_image_date = new TiXmlElement("image_date");
	XmlHandler::WriteDate(h_image_date, mpc->GetAuxiliaryData().GetImageDate());
	h_aux->LinkEndChild(h_image_date);

	std::vector< Sample> VecSample = mpc->GetAuxiliaryData().GetSamples(); 
	 		
	/** Writing auxiliary data samples */
//	if (VecSample.size()) 
//	{
//		TiXmlElement * h_Samples= new TiXmlElement("samples");
//		h_aux->LinkEndChild(h_Samples);
//		for (int i = 0; i < VecSample.size(); i++) 
//		{
//			TiXmlElement * h_Sample = new TiXmlElement("sample");
//			h_Samples->LinkEndChild(h_Sample);
//
//			h_Sample->SetAttribute("systeme_geodesique",
//					VecSample[i].systeme_geodesique.c_str());
//			h_Sample->SetDoubleAttribute("x", VecSample[i].x);
//			h_Sample->SetDoubleAttribute("y", VecSample[i].y);
//			h_Sample->SetDoubleAttribute("z", VecSample[i].z);
//			h_Sample->SetDoubleAttribute("c", VecSample[i].c);
//			h_Sample->SetDoubleAttribute("l", VecSample[i].l);
//		}
//	} 
	XmlHandler::WriteSamples(h_aux,VecSample);
		
	//	std::cout << "ap date" << std::endl;

//	std::cout << "av date" << std::endl;
//	TiXmlElement * h_calibration_date = new TiXmlElement("calibration_date");
//	XmlHandler::WriteDate(h_calibration_date, mpc->GetAuxiliaryData().GetCalibrationDate());
//	h_aux->LinkEndChild(h_calibration_date);
//	std::cout << "ap date" << std::endl;

	/* Writing geometry */
	TiXmlElement * h_geometry = new TiXmlElement("geometry");
	h_orientation->LinkEndChild(h_geometry);

	TiXmlElement *h_extrinseque = new TiXmlElement("extrinseque");
	h_geometry->LinkEndChild(h_extrinseque);

	/* system geodetic  = usine a gaz*/ 
	TiXmlElement * h_systeme = new TiXmlElement("systeme");
	h_extrinseque->LinkEndChild(h_systeme);
	std::string str = mpc->GetSystemGeodesie();
//	std::cout<<"str = ori->GetSystemGeodesie();"<<str<<std::endl;
	std::string system_name;
	std::vector<std::string> SplitVec;
	
	split(SplitVec, str, is_any_of(" "));
	//	std::cout<<"SplitVec[0] "<<SplitVec[0] <<std::endl;
	if (SplitVec.size() && SplitVec[0] == "MATISRTL")	
		{
			system_name = str;
//			std::cout<<" system_name : "<<system_name<<std::endl;
		}
	else
	{
		std::vector<std::string> SplitVecNomSys;
		split(SplitVecNomSys, str, is_any_of(":"));
		std::string str1,str2;
		str1 = str; // str1 = LAMB93
		str2 = str; //str2 = LAMB93
		trim_right_if(str1, is_any_of("0123456789")); // str1 = LAMB
		trim_left_if(str2, is_any_of(str1)); // str2 = 93
		//				std::cout<<"str1=("<<str1<<")"<<"  str2=("<<str2<<")"<<std::endl;
		trim(str2); // str2 = 93 sans espace eventuelle avant et apres
		trim(str1); // str1 = LAMB sans espace eventuelle avant et apres
		//				std::cout<<"str1=("<<str1<<")"<<"  str2=("<<str2<<")"<<std::endl;
		// System check
		if (to_upper_copy(str1) != "LAMB" )
		{
			std::ostringstream tmp;
			tmp << "Lambert" << str2;
			system_name = tmp.str();
//						std::cout<<" system_name : "<<system_name<<std::endl;
		}
	}

	h_systeme->LinkEndChild(new TiXmlText(system_name.c_str()));

	/* grid alti*/
	TiXmlElement *h_grid_alti = new TiXmlElement("grid_alti");
	h_extrinseque->LinkEndChild(h_grid_alti);
	h_grid_alti->LinkEndChild(new TiXmlText("UNKNOWN"));

	/* Sommet */
	TiXmlElement * h_sommet = new TiXmlElement("sommet");
	h_extrinseque->LinkEndChild(h_sommet);
	TiXmlElement * h_easting = new TiXmlElement("easting");
	h_sommet->LinkEndChild(h_easting);
	TiXmlElement * h_northing = new TiXmlElement("northing");
	h_sommet->LinkEndChild(h_northing);
	TiXmlElement * h_altitude = new TiXmlElement("altitude");
	h_sommet->LinkEndChild(h_altitude);

	double easting = mpc->GetExtrinseque().GetSommet()[0];
	double northing = mpc->GetExtrinseque().GetSommet()[1];
	double altitude = mpc->GetExtrinseque().GetSommet()[2];

	std::ostringstream oss[3];
	oss[0] << std::setprecision(12) << easting;
	oss[1] << std::setprecision(12) << northing;
	oss[2] << std::setprecision(12) << altitude;

	h_easting->LinkEndChild(new TiXmlText(oss[0].str().c_str()));
	h_northing->LinkEndChild(new TiXmlText(oss[1].str().c_str()));
	h_altitude->LinkEndChild(new TiXmlText(oss[2].str().c_str()));

//	std::cout << "rotation" << std::endl;
	/* rotation */
	TiXmlElement * h_rotation = new TiXmlElement("rotation");
	h_extrinseque->LinkEndChild(h_rotation);

	TiXmlElement *h_Image2Ground = new TiXmlElement("Image2Ground");
	h_rotation->LinkEndChild(h_Image2Ground);
	h_Image2Ground->LinkEndChild(new TiXmlText("false"));
//	std::cout<<"hey!"<<std::endl;
	
	TiXmlElement * h_mat3d = new TiXmlElement("mat3d");
		h_rotation->LinkEndChild(h_mat3d);
	
	XmlHandler::WriteMat3D(h_mat3d, mpc->GetExtrinseque().GetRotation(0), mpc->GetExtrinseque().GetRotation(1), mpc->GetExtrinseque().GetRotation(2));
//	std::cout<<"hey aprÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ¨s!"<<std::endl;
	/* quaternion */
	TiXmlElement * h_quaternion = new TiXmlElement("quaternion");
	h_rotation->LinkEndChild(h_quaternion);
	h_quaternion->LinkEndChild(new TiXmlText("Laisse tomber le quaternion ;-)"));

	/* intrinseque */
	TiXmlElement * h_intrinseque = new TiXmlElement("intrinseque");
	h_geometry->LinkEndChild(h_intrinseque);
//	std::cout << "intrinseque" << std::endl;

	/* intrinseque  sensor*/
	TiXmlElement * h_sensor = new TiXmlElement("sensor");
	h_intrinseque->LinkEndChild(h_sensor);
	TiXmlElement *h_name = new TiXmlElement("name");
	h_sensor->LinkEndChild(h_name);
	h_name->LinkEndChild(new TiXmlText(mpc->GetAuxiliaryData().GetCameraName().c_str()));

	
	/* calibration_date */
	TiXmlElement * h_calibration_date = new TiXmlElement("calibration_date");
	XmlHandler::WriteDate(h_calibration_date, mpc->GetAuxiliaryData().GetCalibrationDate());
	h_sensor->LinkEndChild(h_calibration_date);
	
	
	/* serial_number */
	TiXmlElement * h_serial_number = new TiXmlElement("serial_number");
//	std::cout << "serial_number" << std::endl;
	h_sensor->LinkEndChild(h_serial_number);
	h_serial_number->LinkEndChild(new TiXmlText("UNKNOWN"));

	
	/* image_size */
	unsigned int nl, nc;
	mpc->GetTailleImage(nc, nl);
	TiXmlElement *h_image_size = new TiXmlElement("image_size");
//	std::cout << "image_size" << std::endl;
	h_sensor->LinkEndChild(h_image_size);
	TiXmlElement * h_width = new TiXmlElement("width");
	h_image_size->LinkEndChild(h_width);
	std::ostringstream ch_nc;
	ch_nc << nc;
	h_width->LinkEndChild(new TiXmlText(ch_nc.str().c_str()));

	TiXmlElement * h_height = new TiXmlElement("height");
	h_image_size->LinkEndChild(h_height);
	std::ostringstream ch_nl;
	ch_nl << nl;
	h_height->LinkEndChild(new TiXmlText(ch_nl.str().c_str()));

	/* ppa*/
	TiXmlElement * h_ppa = new TiXmlElement("ppa");
//	std::cout << "ppa" << std::endl;
	h_sensor->LinkEndChild(h_ppa);
	TiXmlElement * h_c = new TiXmlElement("c");
	h_ppa->LinkEndChild(h_c);
	TiXmlElement * h_l = new TiXmlElement("l");
	h_ppa->LinkEndChild(h_l);
	TiXmlElement * h_focale = new TiXmlElement("focale");
	h_ppa->LinkEndChild(h_focale);

	double c_ppa = mpc->GetcPPA();
	double l_ppa = mpc->GetlPPA();
	double f_ppa = mpc->GetFocal();

	std::ostringstream ch_c_ppa, ch_l_ppa, ch_f_ppa;
	ch_c_ppa << std::setprecision(12) << c_ppa;
	ch_l_ppa << std::setprecision(12) << l_ppa;
	ch_f_ppa << std::setprecision(12) << f_ppa;

	h_c->LinkEndChild(new TiXmlText(ch_c_ppa.str().c_str()));
	h_l->LinkEndChild(new TiXmlText(ch_l_ppa.str().c_str()));
	h_focale->LinkEndChild(new TiXmlText(ch_f_ppa.str().c_str()));

	/* distortion */
//	std::cout<<"ecriture : transfo 2D "<<std::endl;
	unsigned int nb = mpc->GetTransfo2D().GetNbTransfo2DElementaire();
//	std::cout<<"mpc->GetTransfo2D().GetNbTransfo2DElementaire() :"<<nb<<std::endl;
	for (unsigned int i = 0; i < nb; ++i)
	{
		shared_ptr<Transfo2DElementaire> te = mpc->GetTransfo2D().GetTransfo2DElementaire(i);

		if (typeid(*te) == typeid(DistortionPolynomeIGN))
		{
//			std::cout<<"avant : dynamic_cast<const DistortionPolynomeIGN*> (te); "<<std::endl;
			shared_ptr<DistortionPolynomeIGN> dp = dynamic_pointer_cast<DistortionPolynomeIGN> (te);
//			std::cout<<"dynamic_cast<const DistortionPolynomeIGN*> (te); "<<std::endl;
			//			if (!dp)
			//			{
			//				std::cout<<"problem "<<std::endl;
			//				throw std::logic_error("Exception : dynamic_cast<const DistortionPolynomeIGN*> ");
			//				
			//				//				exit(0);
			//			}
			write_distortion_polynome(h_sensor, dp);
		}
		else if (typeid(*te) == typeid(DistortionGrilleMATIS))
		{
//			std::cout<<"problem "<<std::endl;
			throw std::logic_error("DistortionGrilleMATIS non gÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ©rÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ© ! ");
			//			exit(0);
		}
		else if (typeid(*te) == typeid(Homographie2D))
		{
			shared_ptr<Homographie2D> h2d = dynamic_pointer_cast<Homographie2D> (te);
			XmlHandler::WriteHomographie2D(h_orientation,h2d); 
		}
		else if (typeid(*te) == typeid(ProjectiveTransfo2D))
		{
			shared_ptr<ProjectiveTransfo2D> h2d = dynamic_pointer_cast<ProjectiveTransfo2D> (te);
			XmlHandler::WriteProjectiveTransfo2D(h_orientation,h2d);
		}
			
		else
		{
//			std::cout<<"problem "<<std::endl;
			throw std::logic_error("type de tranformation elementaire non gÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ©rÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ© ! ");
			//			exit(0);
		}
//		std::cout<<"non problem "<<std::endl;
	}

//	std::cout<<"sort de boucle! "<<std::endl;
	doc.SaveFile(filename.c_str());
//	std::cout<<"doc.SaveFile(filename.c_str())"<<filename<<std::endl;
	return true;

}

void write_distortion_polynome(TiXmlElement * h_sensor,
		shared_ptr<DistortionPolynomeIGN> dp) 
{
	/* distortion */
	TiXmlElement * h_distortion = new TiXmlElement("distortion");
	h_sensor->LinkEndChild(h_distortion);
	TiXmlElement * h_pps = new TiXmlElement("pps");
	h_distortion->LinkEndChild(h_pps);
	TiXmlElement * h_c_pps = new TiXmlElement("c");
	TiXmlElement * h_l_pps = new TiXmlElement("l");
	h_pps->LinkEndChild(h_c_pps);
	h_pps->LinkEndChild(h_l_pps);

	TiXmlElement * h_r3 = new TiXmlElement("r3");
	TiXmlElement * h_r5 = new TiXmlElement("r5");
	TiXmlElement * h_r7 = new TiXmlElement("r7");
	h_distortion->LinkEndChild(h_r3);
	h_distortion->LinkEndChild(h_r5);
	h_distortion->LinkEndChild(h_r7);

	std::stringstream ch_c_pps;
	ch_c_pps << std::setprecision(12) << dp->GetcPPS();
	h_c_pps->LinkEndChild(new TiXmlText(ch_c_pps.str().c_str()));

	std::stringstream ch_l_pps;
	ch_l_pps << std::setprecision(12) << dp->GetlPPS();
	h_l_pps->LinkEndChild(new TiXmlText(ch_l_pps.str().c_str()));

	std::stringstream ch_r3;
	ch_r3 << std::setprecision(12) << dp->GetR3();
	h_r3->LinkEndChild(new TiXmlText(ch_r3.str().c_str()));

	std::stringstream ch_r5;
	ch_r5 << std::setprecision(12) << dp->GetR5();
	h_r5->LinkEndChild(new TiXmlText(ch_r5.str().c_str()));

	std::stringstream ch_r7;
	ch_r7 << std::setprecision(12) << dp->GetR7();
	h_r7->LinkEndChild(new TiXmlText(ch_r7.str().c_str()));
}

bool xml_writer(const shared_ptr<ModeleProjection> ori, const std::string & filename)
{
//	std::cout<<"xml_writer"<<std::endl;
	if (!ori) 
	{
		return false;
	}
	const shared_ptr<ModeleProjectionConique> mpc = dynamic_pointer_cast<ModeleProjectionConique>(ori);
	if (mpc) 
		return  conic_xml_writer(mpc, filename);
	
	const shared_ptr<ModeleProjectionSpherique> mps = dynamic_pointer_cast<ModeleProjectionSpherique>(ori);
	if (mps)
		return  spheric_xml_writer(mps, filename);
	
	return false;
}
};
