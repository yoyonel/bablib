#include <sstream>
#include <fstream>
#include <iostream>

#include "noboost.hpp"

#include <tinyxml.h>

#include "outil_xml.hpp"
#include "modeleprojectionconique.hpp"
#include "modeleprojectioncarto.hpp"
#include "distortionpolynomeign.hpp"
#include "distortiongrillematis.hpp"
#include "geodesieproj4.hpp"
#include "systematismeaffine.hpp"
#include "systematismecylindrique.hpp"
#include "modeleprojectionspherique.hpp"

namespace OrientationMATIS
{
typedef struct
{
	//Image Size
	int imNs,imNl;

	//PPA
	double ppa_x, ppa_y, focale;

	double pps_x,pps_y;

	double r1,r3,r5,r7;

	double pixel_size;
}ConicCalibrationIntrinsic_t;

//****************************FC
typedef struct
{
        //Image Size
        int imNs,imNl;

        //PPA
        double lambdaMin, lambdaMax;
        double phiMin, phiMax;
        double cPPA, lPPA;

}SphericCalibrationIntrinsic_t;
//****************************\FC


typedef struct
{
	shared_ptr<Euclidien> euclidien;
	std::string nomsysy_out;

	double sommet[3];
	double rotation[3][3];

	//Distorsion et autre transfo
	Transfo2D t2D;
}ConicCalibrationExtrinsic_t;

typedef struct
{
	AuxiliaryData   aux;
	ConicCalibrationIntrinsic_t intrinseque;
	ConicCalibrationExtrinsic_t extrinseque;
}ConicCalibrationData_t;

//***********************************FC
typedef struct
{
        AuxiliaryData   aux;
        SphericCalibrationIntrinsic_t intrinseque;
        ConicCalibrationExtrinsic_t extrinseque;
}SphericCalibrationData_t;
//*************************\FC



bool LoadAuxilaryData_FromNode(TiXmlNode* node,ConicCalibrationData_t  & aux)
{
	bool verbose = false;
	TiXmlNode* temp = node->FirstChild("image_date");
	if(temp ==NULL)
		return false;

	Date img_date;
	if (!XmlHandler::ReadDate(temp, img_date))
		return false;

	aux.aux.SetImageDate(img_date);

	TiXmlNode* ImageNameNode = node->FirstChild("image_name");
	if(ImageNameNode != NULL)
	{
		if(ImageNameNode->ToElement()->GetText()!=NULL)
			aux.aux.SetImageName(ImageNameNode->ToElement()->GetText());
	}
	if (verbose)
		std::cout<<"Lecture stereopolis ! \n";
	
	TiXmlNode* StereopolisNode = node->FirstChild("stereopolis");
	if (StereopolisNode != NULL) 
	{
		Stereopolis st;
		XmlHandler::ReadStereopolis(StereopolisNode, st);
		aux.aux.SetStereopolis(st);
	}
	
	

	std::vector<Sample> mysamples;
	XmlHandler::ReadSamples(node, mysamples);

	aux.aux.AddSamples(mysamples);

	return true;
}

//********************************************************************************FC
//copie de LoadAuxilaryData_FromNode(TiXmlNode* node,ConicCalibrationData_t  & aux)
//  -> pas trÃÂÃÂÃÂÃÂ¨s propre, ÃÂÃÂÃÂÃÂ¿il faudrait faire dÃÂÃÂÃÂÃÂ©river ConicCalibrationData_t et SphericCalibrationData_t d'une mÃÂÃÂÃÂÃÂªme classe mÃÂÃÂÃÂÃÂ¨re?
// non modifiÃÂÃÂÃÂÃÂ©
bool LoadAuxilaryData_FromNode(TiXmlNode* node,SphericCalibrationData_t  & aux)
{
        bool verbose = false;
        TiXmlNode* temp = node->FirstChild("image_date");
        if(temp ==NULL)
                return false;

        Date img_date;
        if (!XmlHandler::ReadDate(temp, img_date))
                return false;

        aux.aux.SetImageDate(img_date);

        TiXmlNode* ImageNameNode = node->FirstChild("image_name");
        if(ImageNameNode != NULL)
        {
                if(ImageNameNode->ToElement()->GetText()!=NULL)
                        aux.aux.SetImageName(ImageNameNode->ToElement()->GetText());
        }
        if (verbose)
                std::cout<<"Lecture stereopolis ! \n";

        TiXmlNode* StereopolisNode = node->FirstChild("stereopolis");
        if (StereopolisNode != NULL)
        {
                Stereopolis st;
                XmlHandler::ReadStereopolis(StereopolisNode, st);
                aux.aux.SetStereopolis(st);
        }



        std::vector<Sample> mysamples;
        XmlHandler::ReadSamples(node, mysamples);

        aux.aux.AddSamples(mysamples);

        return true;
}
//***************************************************************\FC

//********************************
//	Geometry Intrinseque
//********************************
bool LoadSensor_FromNode(TiXmlNode* node,ConicCalibrationData_t   & ptr)
{
	//Calibration Date
	TiXmlNode* CalibrationNode = node->FirstChild("calibration_date");
	if(CalibrationNode ==NULL)
	{
		std::cout << "Pas d info sur la calibration"<<std::endl;
		//return false;
	}

	Date calibration_date;
	if (XmlHandler::ReadDate(CalibrationNode, calibration_date))
		ptr.aux.SetCalibrationDate(calibration_date);
	else
	{
		std::cout << "Pas de date de calibration" << std::endl;
		//return false;
	}
	
	// Lecture <image_size> <width> & <height>
	TiXmlNode* ImageSizeNode = node->FirstChild("image_size");
	if(ImageSizeNode ==NULL)
	{
		std::cout << "ERREUR : Pas de champ Image Size"<<std::endl;
		return false;
	}

	TiXmlNode* ImageSizeWidthNode = ImageSizeNode->FirstChild("width");
	TiXmlNode* ImageSizeHeigthNode = ImageSizeNode->FirstChild("height");

	if(ImageSizeHeigthNode ==NULL || ImageSizeWidthNode ==NULL)
	{
		std::cout << "ERREUR : Pas de champ Image Size"<<std::endl;
		return false;
	}
	
	ptr.intrinseque.imNs = atoi(ImageSizeWidthNode->ToElement()->GetText());
	ptr.intrinseque.imNl = atoi(ImageSizeHeigthNode->ToElement()->GetText());
	
	
	//Lecture <sensor_size> <width> & <height>
	TiXmlNode* SensorSizeNode = node->FirstChild("sensor_size");
	if(SensorSizeNode ==NULL)
		ptr.aux.SetSensorSize(ptr.intrinseque.imNs, ptr.intrinseque.imNl);
	else
	{
		TiXmlNode* SensorSizeWidthNode = SensorSizeNode->FirstChild("width");
		TiXmlNode* SensorSizeHeigthNode = SensorSizeNode->FirstChild("height");

		if (SensorSizeHeigthNode ==NULL || SensorSizeWidthNode ==NULL)
		{
			std::cout << "ERREUR : Pas de width et height dans le sensor_size"<<std::endl;
			return false;
		}
		ptr.aux.SetSensorSize(atoi(SensorSizeWidthNode->ToElement()->GetText()), 
							  atoi(SensorSizeHeigthNode->ToElement()->GetText()));
	}

	// Lecture <sensor> <name>
	TiXmlNode* NameNode = node->FirstChild("name");
	if(NameNode ==NULL)
	{
		std::cout << "ERREUR : pas de champ name"<<std::endl;
		//return false;
	}
	else
		if(NameNode->ToElement()->GetText()!=NULL)
			ptr.aux.SetCameraName(NameNode->ToElement()->GetText());


	// Lecture <sensor><ppa> <c> <l> <focale>
	TiXmlNode* PPANode = node->FirstChild("ppa");
	if(PPANode ==NULL)
	{
		std::cout << "ERREUR : pas de ppa"<<std::endl;
		return false;
	}

	TiXmlNode* PPA_X_Node = PPANode->FirstChild("c");
	TiXmlNode* PPA_Y_Node = PPANode->FirstChild("l");
	TiXmlNode* PPA_F_Node = PPANode->FirstChild("focale");
	if(PPA_X_Node==NULL || PPA_Y_Node==NULL || PPA_F_Node==NULL)
	{
		std::cout << "ERREUR : pas de ppa"<<std::endl;
		return false;
	}

	ptr.intrinseque.ppa_x = atof(PPA_X_Node->ToElement()->GetText());
	ptr.intrinseque.ppa_y = atof(PPA_Y_Node->ToElement()->GetText());
	ptr.intrinseque.focale = atof(PPA_F_Node->ToElement()->GetText());


	//lecture <sensor><transfo2d>
	TiXmlNode* TransfoNode = node->FirstChild("transfo2d");
	if (TransfoNode != NULL)
	{
		TiXmlNode* TransfoElemNode = TransfoNode->FirstChild("tr2delem");
		while(TransfoElemNode)
		{
			std::cout << "Chargement d'une transfo"<<std::endl;
			TiXmlElement* TransfoElemElement = TransfoElemNode->ToElement();
			if (TransfoElemElement == NULL)
			{
				std::cout << "Erreur dans la recherche des attributs"<<std::endl;
				return false;
			}
			// test
			const TiXmlAttribute* attribut = TransfoElemElement->FirstAttribute();
			while(attribut)
			{
				std::cout << "attribut : "<<attribut->Name()<<" "<<attribut->Value()<<std::endl;
				attribut=attribut->Next();
			}
			std::string strType = TransfoElemElement->Attribute("Type");
			if (!strType.size())
			{
				std::cout << "Pas de type sur cette transfo"<<std::endl;
				return false;
			}
			if (strType==SystematismeCylindriqueTopAero::nomTagXml)
			{
				std::cout << "SystematismeCylindriqueTopAero"<<std::endl;
				std::string strInterne = TransfoElemElement->Attribute("isinterne");
				bool isInterne = true;
				if (strInterne.size())
				{
					if (strInterne != std::string("true"))
						isInterne = false;
				}
				double c0=0.,l0=0.,s1=0.,s2=0.;
				TransfoElemElement->QueryDoubleAttribute("C0",&c0);
				TransfoElemElement->QueryDoubleAttribute("L0",&l0);
				TransfoElemElement->QueryDoubleAttribute("S1",&s1);
				TransfoElemElement->QueryDoubleAttribute("S2",&s2);
				SystematismeCylindriqueTopAero * sys = new SystematismeCylindriqueTopAero(c0,l0,s1,s2,isInterne);
				shared_ptr<Transfo2DElementaire> tr = shared_ptr<Transfo2DElementaire> (sys);
				ptr.extrinseque.t2D.AddTransformation2D(tr);
			}
			else if (strType==SystematismeAffineTopAero::nomTagXml)
			{
				std::cout << "SystematismeAffineTopAero"<<std::endl;
				std::string strInterne = TransfoElemElement->Attribute("isinterne");
				bool isInterne = true;
				if (strInterne.size())
				{
					if (strInterne != std::string("true"))
						isInterne = false;
				}
				double c0=0.,l0=0.,s6=0.,s7=0.,s8=0.;
				TransfoElemElement->QueryDoubleAttribute("C0",&c0);
				TransfoElemElement->QueryDoubleAttribute("L0",&l0);
				TransfoElemElement->QueryDoubleAttribute("S6",&s6);
				TransfoElemElement->QueryDoubleAttribute("S7",&s7);
				TransfoElemElement->QueryDoubleAttribute("S8",&s8);
				SystematismeAffineTopAero * sys = new SystematismeAffineTopAero(c0,l0,s6,s7,s8,isInterne);
				shared_ptr<Transfo2DElementaire> tr = shared_ptr<Transfo2DElementaire> (sys);
				ptr.extrinseque.t2D.AddTransformation2D(tr);
				
			}
			else
			{
				std::cout << "Type de transfo non gere : "<<strType<<std::endl;
				return false;
			}
			TransfoElemNode = TransfoElemNode->NextSiblingElement("tr2delem");
		}
	}
	
	//lecture <sensor> <distorsion>
	TiXmlNode* DistoNode = node->FirstChild("distortion");
	if(DistoNode !=NULL)
	{
		//PPS
		TiXmlNode* DistoPPSNode = DistoNode->FirstChild("pps");
		if(DistoPPSNode ==NULL)
		{
			std::cout << "Pas de PPS dans la distortion"<<std::endl;
			return false;
		}

		TiXmlNode* PPS_X_Node = DistoPPSNode->FirstChild("c");
		TiXmlNode* PPS_Y_Node = DistoPPSNode->FirstChild("l");
		if(PPS_X_Node==NULL || PPS_Y_Node==NULL)
		{
			std::cout << "Pas de PPS dans la distortion"<<std::endl;
			return false;
		}
		ptr.intrinseque.pps_x = atof(PPS_X_Node->ToElement()->GetText());
		ptr.intrinseque.pps_y = atof(PPS_Y_Node->ToElement()->GetText());

		//Polynome
		TiXmlNode* Disto_R1_Node = DistoNode->FirstChild("r1");
		TiXmlNode* Disto_R3_Node = DistoNode->FirstChild("r3");
		TiXmlNode* Disto_R5_Node = DistoNode->FirstChild("r5");
		TiXmlNode* Disto_R7_Node = DistoNode->FirstChild("r7");

		if(Disto_R3_Node==NULL || Disto_R5_Node==NULL || Disto_R7_Node==NULL)
		{
			std::cout << "Pas de Coef pour le polynome de distorsion"<<std::endl;
			return false;
		}
		ptr.intrinseque.r1 = 0;
		if(Disto_R1_Node!=NULL)
			ptr.intrinseque.r1= atof(Disto_R1_Node->ToElement()->GetText());
		ptr.intrinseque.r3 = atof(Disto_R3_Node->ToElement()->GetText());
		ptr.intrinseque.r5 = atof(Disto_R5_Node->ToElement()->GetText());
		ptr.intrinseque.r7 = atof(Disto_R7_Node->ToElement()->GetText());
		DistortionPolynomeIGN * polyIGN = 
			new DistortionPolynomeIGN(ptr.intrinseque.pps_x, ptr.intrinseque.pps_y, 
								  ptr.intrinseque.r1, ptr.intrinseque.r3,
								  ptr.intrinseque.r5, ptr.intrinseque.r7, true);
		//Fixer la limite de validite du polynome de distorsion
		//polyIGN->SetR2max(ptr.intrinseque.imNs, ptr.intrinseque.imNl);
		//modif par bahman 4 juin 2010
		unsigned int sensorNX, sensorNY;
		ptr.aux.GetSensorSize(sensorNX, sensorNY);
		polyIGN->SetR2max(sensorNX, sensorNY);
		shared_ptr<Transfo2DElementaire> tr = shared_ptr<Transfo2DElementaire> (polyIGN);
		ptr.extrinseque.t2D.AddTransformation2D(tr);
	}
	
	//Noeud optionnel mais fort utile...
	TiXmlNode* PixelSizeNode = node->FirstChild("pixel_size");
	if(PixelSizeNode == NULL)
		return true;
	
	ptr.intrinseque.pixel_size = atof(PixelSizeNode->ToElement()->GetText());

	return true;
}

//*****************************************************************************FC
//modifiÃÂÃÂÃÂÃÂ©
bool LoadSpherique_FromNode(TiXmlNode* node,SphericCalibrationData_t   & ptr)
{

        // Lecture <image_size> <width> & <height>
        TiXmlNode* ImageSizeNode = node->FirstChild("image_size");
        if(ImageSizeNode ==NULL)
        {
                std::cout << "ERREUR : Pas de champ Image Size"<<std::endl;
                return false;
        }

        TiXmlNode* ImageSizeWidthNode = ImageSizeNode->FirstChild("width");
        TiXmlNode* ImageSizeHeigthNode = ImageSizeNode->FirstChild("height");

        if(ImageSizeHeigthNode ==NULL || ImageSizeWidthNode ==NULL)
        {
                std::cout << "ERREUR : Pas de champ Image Size"<<std::endl;
                return false;
        }

        ptr.intrinseque.imNs = atoi(ImageSizeWidthNode->ToElement()->GetText());
        ptr.intrinseque.imNl = atoi(ImageSizeHeigthNode->ToElement()->GetText());


        // Lecture <spherique><ppa> <c> <l>
        TiXmlNode* PPANode = node->FirstChild("ppa");
        if(PPANode ==NULL)
        {
                std::cout << "ERREUR : pas de ppa"<<std::endl;
                return false;
        }

        TiXmlNode* PPA_X_Node = PPANode->FirstChild("c");
        TiXmlNode* PPA_Y_Node = PPANode->FirstChild("l");
        if(PPA_X_Node==NULL || PPA_Y_Node==NULL)
        {
                std::cout << "ERREUR : pas de ppa"<<std::endl;
                return false;
        }

        ptr.intrinseque.cPPA = atof(PPA_X_Node->ToElement()->GetText());
        ptr.intrinseque.lPPA = atof(PPA_Y_Node->ToElement()->GetText());


        // Lecture <spherique><frame> <lambda_min> <lambda_max> <phi_min> <phi_max>
        TiXmlNode* FrameNode = node->FirstChild("frame");
        if(FrameNode ==NULL)
        {
                std::cout << "ERREUR : pas de frame"<<std::endl;
                return false;
        }

        TiXmlNode* lambda_min_Node = FrameNode->FirstChild("lambda_min");
        TiXmlNode* lambda_max_Node = FrameNode->FirstChild("lambda_max");
        TiXmlNode* phi_min_Node = FrameNode->FirstChild("phi_min");
        TiXmlNode* phi_max_Node = FrameNode->FirstChild("phi_max");
        if(lambda_min_Node==NULL || lambda_max_Node==NULL || phi_min_Node==NULL || phi_max_Node==NULL)
        {
                std::cout << "ERREUR : pas de frame"<<std::endl;
                return false;
        }

        ptr.intrinseque.lambdaMin = atof(lambda_min_Node->ToElement()->GetText());
        ptr.intrinseque.lambdaMax = atof(lambda_max_Node->ToElement()->GetText());
        ptr.intrinseque.phiMin = atof(phi_min_Node->ToElement()->GetText());
        ptr.intrinseque.phiMax = atof(phi_max_Node->ToElement()->GetText());



        return true;
}
//***************************************************\FC


bool LoadIntrinseque_FromNode(TiXmlNode* node,ConicCalibrationData_t   & ptr)
{
	TiXmlNode* SensorNode = node->FirstChild("sensor");
	if(SensorNode ==NULL)
		return false;

	return LoadSensor_FromNode(SensorNode,ptr);
}

//*******************************************************************************FC
// non modifiÃÂÃÂÃÂÃÂ©
bool LoadIntrinseque_FromNode(TiXmlNode* node,SphericCalibrationData_t   & ptr)
{
        TiXmlNode* SpheriqueNode = node->FirstChild("spherique");
        if(SpheriqueNode ==NULL)
                return false;

        return LoadSpherique_FromNode(SpheriqueNode,ptr);
}
//******************************************\FC


//********************************
//	Geometry Extrinseque
//********************************
bool LoadNewSystemEuclidien_FromNode(TiXmlNode* node,ConicCalibrationData_t   & ptr)
{	
	std::string typeEuclidien(node->ToElement()->Attribute("type"));
	if (typeEuclidien == "TOPAERO")
	{
		TiXmlNode const * Geographique_Node = node->FirstChild("geographique");
		TiXmlNode const * Cartographique_Node = node->FirstChild("cartographique");
		TiXmlNode const * Origine_Node = node->FirstChild("origine");
		if ((Geographique_Node==NULL)||(Cartographique_Node==NULL)||(Origine_Node==NULL))
			return false;
		std::string sysGeographique = Geographique_Node->ToElement()->GetText();
		std::string sysCartographique = Cartographique_Node->ToElement()->GetText();
		std::cout << "sys geographique : "<<sysGeographique<<std::endl;
		std::cout << "sys cartographique : "<<sysCartographique<<std::endl;
		
		double origin[2];
		bool ok = XmlHandler::ReadPt2D(Origine_Node,origin[0], origin[1]);
		if (!ok)
			return false;
		std::cout << "origine : "<< origin[0] << " "<<origin[1]<<std::endl;
		ptr.extrinseque.euclidien.reset(new EuclidienTOPAERO(sysCartographique,sysGeographique,origin));
	}
	else if(typeEuclidien == "MATISRTL")
	{
		TiXmlNode* X_Node = node->FirstChild("x");
		TiXmlNode* Y_Node = node->FirstChild("y");
		if(X_Node==NULL || Y_Node==NULL)
			return false;
		
		double X = atof(X_Node->ToElement()->GetText());
		double Y = atof(Y_Node->ToElement()->GetText());

		ptr.extrinseque.euclidien.reset(new EuclidienMATISRTL(X, Y));
	}
	else
	{
		std::cout << "typeEuclidien : " << typeEuclidien << " non gere" << std::endl;
		return false;
	}
	return true;
}	

//*********************************************************************FC
// non modifiÃÂÃÂÃÂÃÂ©
bool LoadNewSystemEuclidien_FromNode(TiXmlNode* node,SphericCalibrationData_t   & ptr)
{
        std::string typeEuclidien(node->ToElement()->Attribute("type"));
        if (typeEuclidien == "TOPAERO")
        {
                TiXmlNode const * Geographique_Node = node->FirstChild("geographique");
                TiXmlNode const * Cartographique_Node = node->FirstChild("cartographique");
                TiXmlNode const * Origine_Node = node->FirstChild("origine");
                if ((Geographique_Node==NULL)||(Cartographique_Node==NULL)||(Origine_Node==NULL))
                        return false;
                std::string sysGeographique = Geographique_Node->ToElement()->GetText();
                std::string sysCartographique = Cartographique_Node->ToElement()->GetText();
                std::cout << "sys geographique : "<<sysGeographique<<std::endl;
                std::cout << "sys cartographique : "<<sysCartographique<<std::endl;

                double origin[2];
                bool ok = XmlHandler::ReadPt2D(Origine_Node,origin[0], origin[1]);
                if (!ok)
                        return false;
                std::cout << "origine : "<< origin[0] << " "<<origin[1]<<std::endl;
                ptr.extrinseque.euclidien.reset(new EuclidienTOPAERO(sysCartographique,sysGeographique,origin));
        }
        else if(typeEuclidien == "MATISRTL")
        {
                TiXmlNode* X_Node = node->FirstChild("x");
                TiXmlNode* Y_Node = node->FirstChild("y");
                if(X_Node==NULL || Y_Node==NULL)
                        return false;

                double X = atof(X_Node->ToElement()->GetText());
                double Y = atof(Y_Node->ToElement()->GetText());

                ptr.extrinseque.euclidien.reset(new EuclidienMATISRTL(X, Y));
        }
        else
        {
                std::cout << "typeEuclidien : " << typeEuclidien << " non gere" << std::endl;
                return false;
        }
        return true;
}
//**************************************************\FC

bool LoadOldSystem_FromNode(TiXmlNode* node,ConicCalibrationData_t   & ptr)
{
	std::string nomsystem;
	if(node->ToElement()->GetText())
		nomsystem = node->ToElement()->GetText();
	
	if (nomsystem[0] == '+')
	{
		ptr.extrinseque.nomsysy_out = nomsystem;
		return true;
	}
	
	std::vector<std::string> SplitVecNomSys;
	split(SplitVecNomSys, nomsystem, is_any_of(" "));
	if (SplitVecNomSys.size()==0)
		return false;

	if (to_upper_copy(SplitVecNomSys[0]) == "MATISRTL")
	{
		double X = atof(SplitVecNomSys[1].c_str());
		double Y = atof(SplitVecNomSys[2].c_str());

		ptr.extrinseque.euclidien.reset(new EuclidienMATISRTL(X, Y));

		ptr.extrinseque.nomsysy_out = std::string("");
		for (size_t k = 3; k < SplitVecNomSys.size(); ++k)
			ptr.extrinseque.nomsysy_out += SplitVecNomSys[k] + std::string(" ");
		
		return true;
	}
 
	//Orientation stereopolis "LambertX" ou "Lamber X"
	std::string str1, str2;
	str1 = nomsystem; // str1 = Lambert93
	str2 = nomsystem; //str2 = Lamber93

	trim_right_if(str1, is_any_of("0123456789")); // str1 = Lambert
	trim_left_if(str2, is_any_of(str1)); // str2 = 93

	trim(str2); // str2 = 93 sans espace eventuelle avant et apres
	trim(str1);

	if(to_upper_copy(str1) == "LAMBERT2E" || to_upper_copy(str1) == "LAMB2E")
	{
		str2 = "2E";
		str1 = "LAMBERT";
	}

	// System check
	if (to_upper_copy(str1) != "LAMBERT" && to_upper_copy(str1) != "LAMB")
		return false;

	ptr.extrinseque.nomsysy_out = Geodesie::NomSysteme(to_upper_copy(str2));

	return true;
	
}

//*************************************************************FC
// non modifiÃÂÃÂÃÂÃÂ©
bool LoadOldSystem_FromNode(TiXmlNode* node,SphericCalibrationData_t   & ptr)
{
        std::string nomsystem;
        if(node->ToElement()->GetText())
                nomsystem = node->ToElement()->GetText();

        if (nomsystem[0] == '+')
        {
                ptr.extrinseque.nomsysy_out = nomsystem;
                return true;
        }

        std::vector<std::string> SplitVecNomSys;
        split(SplitVecNomSys, nomsystem, is_any_of(" "));
        if (SplitVecNomSys.size()==0)
                return false;

        if (to_upper_copy(SplitVecNomSys[0]) == "MATISRTL")
        {
                double X = atof(SplitVecNomSys[1].c_str());
                double Y = atof(SplitVecNomSys[2].c_str());

                ptr.extrinseque.euclidien.reset(new EuclidienMATISRTL(X, Y));

                ptr.extrinseque.nomsysy_out = std::string("");
                for (size_t k = 3; k < SplitVecNomSys.size(); ++k)
                        ptr.extrinseque.nomsysy_out += SplitVecNomSys[k] + std::string(" ");

                return true;
        }

        //Orientation stereopolis "LambertX" ou "Lamber X"
        std::string str1, str2;
        str1 = nomsystem; // str1 = Lambert93
        str2 = nomsystem; //str2 = Lamber93

        trim_right_if(str1, is_any_of("0123456789")); // str1 = Lambert
        trim_left_if(str2, is_any_of(str1)); // str2 = 93

        trim(str2); // str2 = 93 sans espace eventuelle avant et apres
        trim(str1);

        if(to_upper_copy(str1) == "LAMBERT2E" || to_upper_copy(str1) == "LAMB2E")
        {
                str2 = "2E";
                str1 = "LAMBERT";
        }

        // System check
        if (to_upper_copy(str1) != "LAMBERT" && to_upper_copy(str1) != "LAMB")
                return false;

        ptr.extrinseque.nomsysy_out = Geodesie::NomSysteme(to_upper_copy(str2));

        return true;

}
//*************************************************************\FC

bool LoadSystem_FromNode(TiXmlNode* node,ConicCalibrationData_t   & ptr)
{
	//Chargement des sytÃÂÃÂÃÂÃÂ¨mes gÃÂÃÂÃÂÃÂ©odÃÂÃÂÃÂÃÂ©siques. 2 versions cohÃÂÃÂÃÂÃÂ©xistent
	TiXmlNode* GeodesyNode = node->FirstChild("geodesique");
	if(GeodesyNode != NULL)
	{	
		if (GeodesyNode->ToElement()->GetText())
			ptr.extrinseque.nomsysy_out = GeodesyNode->ToElement()->GetText();

		//Nouveau cas
		TiXmlNode* SystemeEuclidienNode = node->FirstChild("euclidien");
		if(SystemeEuclidienNode != NULL)
		{	
			if(!LoadNewSystemEuclidien_FromNode(SystemeEuclidienNode,ptr))
				return false;
		}
		return true;
	}
	
	//Ancien cas
	if(!LoadOldSystem_FromNode(node,ptr))
		return false;

	return true;
}

//****************************************************************************FC
// non modifiÃÂÃÂÃÂÃÂ©
bool LoadSystem_FromNode(TiXmlNode* node,SphericCalibrationData_t   & ptr)
{
        //Chargement des sytÃÂÃÂÃÂÃÂ¨mes gÃÂÃÂÃÂÃÂ©odÃÂÃÂÃÂÃÂ©siques. 2 versions cohÃÂÃÂÃÂÃÂ©xistent
        TiXmlNode* GeodesyNode = node->FirstChild("geodesique");
        if(GeodesyNode != NULL)
        {
                if (GeodesyNode->ToElement()->GetText())
                        ptr.extrinseque.nomsysy_out = GeodesyNode->ToElement()->GetText();

                //Nouveau cas
                TiXmlNode* SystemeEuclidienNode = node->FirstChild("euclidien");
                if(SystemeEuclidienNode != NULL)
                {
                        if(!LoadNewSystemEuclidien_FromNode(SystemeEuclidienNode,ptr))
                                return false;
                }
                return true;
        }

        //Ancien cas
        if(!LoadOldSystem_FromNode(node,ptr))
                return false;

        return true;
}
//**********************************************\FC


bool LoadExtrinsequeRotation_FromNode(TiXmlNode* node,ConicCalibrationData_t   & ptr)
{
	double *iVect = ptr.extrinseque.rotation[0];
	double *jVect = ptr.extrinseque.rotation[1];
	double *kVect = ptr.extrinseque.rotation[2];

	TiXmlNode* Image2groundNode = node->FirstChild("Image2Ground");
	if(Image2groundNode ==NULL)
		return false;

	bool Isimage2Ground = false;
	if (Image2groundNode->ToElement()->GetText() == std::string("true"))
		Isimage2Ground = true;

	XmlHandler::_t_XMat3D mat;
	if(!XmlHandler::ReadMat3D(node,mat))
		return false;

	// rotation est le suivant
	if (Isimage2Ground)
	{
		iVect[0] = mat.l1.X;
		iVect[1] = mat.l2.X;
		iVect[2] = mat.l3.X;

		jVect[0] = mat.l1.Y;
		jVect[1] = mat.l2.Y;
		jVect[2] = mat.l3.Y;

		kVect[0] = mat.l1.Z;
		kVect[1] = mat.l2.Z;
		kVect[2] = mat.l3.Z;
	}
	else
	{
		iVect[0] = mat.l1.X;
		iVect[1] = mat.l1.Y;
		iVect[2] = mat.l1.Z;

		jVect[0] = mat.l2.X;
		jVect[1] = mat.l2.Y;
		jVect[2] = mat.l2.Z;

		kVect[0] = mat.l3.X;
		kVect[1] = mat.l3.Y;
		kVect[2] = mat.l3.Z;
	}
	return true;
}

//*****************************************************FC
bool LoadExtrinsequeRotation_FromNode(TiXmlNode* node,SphericCalibrationData_t   & ptr)
{
        double *iVect = ptr.extrinseque.rotation[0];
        double *jVect = ptr.extrinseque.rotation[1];
        double *kVect = ptr.extrinseque.rotation[2];

        TiXmlNode* Image2groundNode = node->FirstChild("Image2Ground");
        if(Image2groundNode ==NULL)
                return false;

        bool Isimage2Ground = false;
        if (Image2groundNode->ToElement()->GetText() == std::string("true"))
                Isimage2Ground = true;

        XmlHandler::_t_XMat3D mat;
        if(!XmlHandler::ReadMat3D(node,mat))
                return false;

        // rotation est le suivant
        if (Isimage2Ground)
        {
                iVect[0] = mat.l1.X;
                iVect[1] = mat.l2.X;
                iVect[2] = mat.l3.X;

                jVect[0] = mat.l1.Y;
                jVect[1] = mat.l2.Y;
                jVect[2] = mat.l3.Y;

                kVect[0] = mat.l1.Z;
                kVect[1] = mat.l2.Z;
                kVect[2] = mat.l3.Z;
        }
        else
        {
                iVect[0] = mat.l1.X;
                iVect[1] = mat.l1.Y;
                iVect[2] = mat.l1.Z;

                jVect[0] = mat.l2.X;
                jVect[1] = mat.l2.Y;
                jVect[2] = mat.l2.Z;

                kVect[0] = mat.l3.X;
                kVect[1] = mat.l3.Y;
                kVect[2] = mat.l3.Z;
        }
        return true;
}
//*****************************************************\FC

bool LoadExtrinseque_FromNode(TiXmlNode* node,ConicCalibrationData_t   & ptr)
{
	//<System>
	TiXmlNode* SystemeNode = node->FirstChild("systeme");
	if(SystemeNode ==NULL)
		return false;

	if(!LoadSystem_FromNode(SystemeNode,ptr))
		return false;

	//<Sommet>
	TiXmlNode* SommetNode = node->FirstChild("sommet");
	if(SommetNode ==NULL)
		return false;

	TiXmlNode* EastingNode = SommetNode->FirstChild("easting");
	TiXmlNode* NorthingNode = SommetNode->FirstChild("northing");
	TiXmlNode* AltitudeNode = SommetNode->FirstChild("altitude");

	if(EastingNode == NULL || NorthingNode == NULL || AltitudeNode == NULL)
		return false;

	ptr.extrinseque.sommet[0] = atof(EastingNode->ToElement()->GetText());
	ptr.extrinseque.sommet[1] = atof(NorthingNode->ToElement()->GetText());
	ptr.extrinseque.sommet[2] = atof(AltitudeNode->ToElement()->GetText());

	// Lecture <rotation>
	TiXmlNode* RotationNode = node->FirstChild("rotation");
	if(RotationNode ==NULL)
		return false;

	if(!LoadExtrinsequeRotation_FromNode(RotationNode,ptr))
		return false;

	return true;
}

//*******************************************************************************FC
// non modifiÃÂÃÂÃÂÃÂ©
bool LoadExtrinseque_FromNode(TiXmlNode* node,SphericCalibrationData_t   & ptr)
{
        //<System>
        TiXmlNode* SystemeNode = node->FirstChild("systeme");
        if(SystemeNode ==NULL)
                return false;

        if(!LoadSystem_FromNode(SystemeNode,ptr))
                return false;

        //<Sommet>
        TiXmlNode* SommetNode = node->FirstChild("sommet");
        if(SommetNode ==NULL)
                return false;

        TiXmlNode* EastingNode = SommetNode->FirstChild("easting");
        TiXmlNode* NorthingNode = SommetNode->FirstChild("northing");
        TiXmlNode* AltitudeNode = SommetNode->FirstChild("altitude");

        if(EastingNode == NULL || NorthingNode == NULL || AltitudeNode == NULL)
                return false;

        ptr.extrinseque.sommet[0] = atof(EastingNode->ToElement()->GetText());
        ptr.extrinseque.sommet[1] = atof(NorthingNode->ToElement()->GetText());
        ptr.extrinseque.sommet[2] = atof(AltitudeNode->ToElement()->GetText());

        // Lecture <rotation>
        TiXmlNode* RotationNode = node->FirstChild("rotation");
        if(RotationNode ==NULL)
                return false;

        if(!LoadExtrinsequeRotation_FromNode(RotationNode,ptr))
                return false;

        return true;
}
//***************************************************FC

//********************************
//	Geometry GÃÂÃÂÃÂÃÂ©nrale
//********************************
bool LoadGeometry_FromNode(TiXmlNode* node,ConicCalibrationData_t  & ptr)
{
	// Lecture <intrinseque>
	TiXmlNode* IntrinsequeNode = node->FirstChild("intrinseque");
	if(IntrinsequeNode ==NULL)
		return false;

	if(!LoadIntrinseque_FromNode(IntrinsequeNode,ptr ))
		return false;

	TiXmlNode* ExtrinsequeNode = node->FirstChild("extrinseque");
	if(ExtrinsequeNode ==NULL)
		return false;

	if(!LoadExtrinseque_FromNode(ExtrinsequeNode,ptr))
		return false;

	return true;
}

//************************************************************FC
// non modifiÃÂÃÂÃÂÃÂ©
bool LoadGeometry_FromNode(TiXmlNode* node,SphericCalibrationData_t  & ptr)
{
        // Lecture <intrinseque>
        TiXmlNode* IntrinsequeNode = node->FirstChild("intrinseque");
        if(IntrinsequeNode ==NULL)
                return false;

        if(!LoadIntrinseque_FromNode(IntrinsequeNode,ptr ))
                return false;

        TiXmlNode* ExtrinsequeNode = node->FirstChild("extrinseque");
        if(ExtrinsequeNode ==NULL)
                return false;

        if(!LoadExtrinseque_FromNode(ExtrinsequeNode,ptr))
                return false;

        return true;
}
//*******************************\FC

void InitConicData(ConicCalibrationData_t &data)
{
	data.intrinseque.imNl =0;
	data.intrinseque.imNs =0;

	data.intrinseque.focale =0;
	data.intrinseque.ppa_x =0;
	data.intrinseque.ppa_y =0;

	data.intrinseque.pps_x =0;
	data.intrinseque.pps_y =0;

	data.intrinseque.r1 =0;
	data.intrinseque.r3 =0;
	data.intrinseque.r5 =0;
	data.intrinseque.r7 =0;

	data.intrinseque.pixel_size =0;

	memset(data.extrinseque.sommet,0,3*sizeof(double));
	memset(data.extrinseque.rotation,0,9*sizeof(double));
}

//**********************************************FC
void InitSphericData(SphericCalibrationData_t &data)
{
        data.intrinseque.imNl =0;
        data.intrinseque.imNs =0;

        data.intrinseque.lambdaMin =0;
        data.intrinseque.lambdaMax =0;

        data.intrinseque.phiMin =0;
        data.intrinseque.phiMax =0;

        data.intrinseque.cPPA =0;
        data.intrinseque.lPPA =0;

        memset(data.extrinseque.sommet,0,3*sizeof(double));
        memset(data.extrinseque.rotation,0,9*sizeof(double));
}
//*********************************************\FC

shared_ptr<ModeleProjection> xml_reader(const std::string &filename)
{
	
	bool verbose = false;

	if (verbose)
		std::cout << " conic_xml_reader " << "\n";
	
	TiXmlDocument doc(filename.c_str());
	if (!doc.LoadFile())
	{ 
		if (verbose)
			std::cout << "problrinseque.rotation,0,9*sizeof(double));
}
//*********************************************\FC

shared_ptr<ModeleProjection> xml_reader(const std::string &filename)
{
	
	bool verbose = false;

	if (verbose)
		std::cout << " conic_xml_reader " << "\n";
	
	TiXmlDocument doc(filename.c_str());
	if (!doc.LoadFile())
	{ 
		if (verbose)
			std::cout << "probl