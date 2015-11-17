#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>

#include "noboost.hpp"

#include "outil_xml.hpp"

namespace OrientationMATIS {
namespace XmlHandler {
bool ReadDate(const TiXmlHandle* h, Date &date) {
	//			std::cout<<" av : ReadDate \n";
	TiXmlElement *pElem = h->ToElement();

	if (pElem->FirstChildElement("day") == 0) {
		//				std::cout<<" pElem->FirstChildElement(day) == 0 \n";
		int calibration_date[6];
		for (int i = 0; i < 6; i++)
			calibration_date[i] = 0;

		if (pElem->GetText()) {
			std::string str = pElem->GetText();
			//					std::cout<<"calibration_date : "<<pElem->GetText()<<" = "<<str<<std::endl;
			std::vector<std::string> SplitVec;
			split(SplitVec, str, is_any_of("-/"));
			int taille = SplitVec.size();
			if (taille> 6)
				return false;
			int ind = 0;

			for (int i = taille - 1; i >= 0; i--) {
				calibration_date[ind] = atoi(SplitVec[i].c_str());
				//						std::cout << "calibration_date[" << ind << "]" << calibration_date[ind] << std::endl;
				ind++;
			}
		}
		date.Set(calibration_date[0], calibration_date[1], calibration_date[2],
				calibration_date[3], calibration_date[4], calibration_date[5]);

	} else {
		//				std::cout<<" dans else ! \n";
		if (!h->FirstChildElement("year").Element() || !h->FirstChildElement("month").Element() || !h->FirstChildElement("day").Element() || !h->FirstChildElement("hour").Element() || !h->FirstChildElement("minute").Element() || !h->FirstChildElement("second").Element() || !h->FirstChildElement("time_system").Element())
			return false;
		//				std::cout<<" rest dans else ! \n";
		int year = atoi(h->FirstChildElement("year").Element()->GetText());
		int month = atoi(h->FirstChildElement("month").Element()->GetText());
		int day = atoi(h->FirstChildElement("day").Element()->GetText());
		int hour = atoi(h->FirstChildElement("hour").Element()->GetText());
		int minute = atoi(h->FirstChildElement("minute").Element()->GetText());
		float second = atof(h->FirstChildElement("second").Element()->GetText());
		//				std::cout<<"  av lectur time_sys dans else ! \n";
		std::string time_system;
		if (h->FirstChildElement("time_system").Element()->GetText())
			time_system = h->FirstChildElement("time_system").Element()->GetText();
		else
			time_system = "";

		//				std::cout<<" lectur time_sys dans else ! \n";
		//				std::cout << year << "\t" << month << "\t" << day << "\t" << hour
		//				<< "\t" << minute << "\t" << std::setprecision(4) << second
		//				<< std::endl;
		date.Set(year, month, day, hour, minute, second, time_system);
		//				std::cout<<" data.set  dans else ! \n";
	}
	return true;
}

bool ReadPt3D(const TiXmlHandle* h, double &x, double &y, double &z) {
	TiXmlElement* pElem;
	TiXmlHandle h_doc(0);

	// Lecture <x>
	pElem = h->FirstChildElement("x").Element();
	if (!pElem)
		return false;
	x = atof(pElem->GetText());

	// Lecture <y>
	pElem = h->FirstChildElement("y").Element();
	if (!pElem)
		return false;
	y = atof(pElem->GetText());

	// Lecture <Z>
	pElem = h->FirstChildElement("z").Element();
	if (!pElem)
		return false;
	z = atof(pElem->GetText());
	return true;
}

bool ReadMat3D(const TiXmlHandle* h, double *l1, double *l2, double *l3) {
	TiXmlElement* pElem;
	TiXmlHandle h_doc(0);
	double x, y, z;

	// Lecture <l1>
	pElem = h->FirstChildElement("l1").Element();
	if (!pElem)
		return false;
	TiXmlHandle h_l1(pElem);
	if (!ReadPt3D(&h_l1, x, y, z))
		return false;
	l1[0] = x;
	l1[1] = y;
	l1[2] = z;

	// Lecture <l2>
	pElem = h->FirstChildElement("l2").Element();
	if (!pElem)
		return false;
	TiXmlHandle h_l2(pElem);
	if (!ReadPt3D(&h_l2, x, y, z))
		return false;
	l2[0] = x;
	l2[1] = y;
	l2[2] = z;

	// Lecture <l2>
	pElem = h->FirstChildElement("l3").Element();
	if (!pElem)
		return false;
	TiXmlHandle h_l3(pElem);
	if (!ReadPt3D(&h_l3, x, y, z))
		return false;
	l3[0] = x;
	l3[1] = y;
	l3[2] = z;
	return true;
}

void ReadSamples(const TiXmlHandle* h, std::vector <Sample> & mysamples) {
	//			std::cout<<"ReadSamples\n";
	TiXmlElement * pElem;
	pElem = h->FirstChildElement("samples").Element();
	if (pElem) {
		//				std::cout<<"samples\n";
		TiXmlHandle h_samples = TiXmlHandle(pElem);
		for (unsigned int i=0; h_samples.Child(i).ToElement() != 0; ++i) {
			//					std::cout<<"sample["<<i<<"]\n";
			Sample s;
			TiXmlElement * el = h_samples.Child(i).ToElement();

			//					if (!el->Attribute("TestType") )
			//					{
			//						s.TestType = Sample::TestUnknown;
			//						continue;
			//					}
			//					s.TestType = el->Attribute("TestType");

			if (!el->Attribute("TestType") || el->Attribute("TestType")
					== Sample::TestProjWToI) {
				//						std::cout<<"TestProjWToI\n";
				if (el->QueryDoubleAttribute("x", &s.x) != TIXML_SUCCESS
						|| el->QueryDoubleAttribute("y", &s.y) != TIXML_SUCCESS
						|| el->QueryDoubleAttribute("z", &s.z) != TIXML_SUCCESS
						|| el->QueryDoubleAttribute("c", &s.c) != TIXML_SUCCESS
						|| el->QueryDoubleAttribute("l", &s.l) != TIXML_SUCCESS
						|| !el->Attribute("systeme_geodesique")) {
					s.TestType = Sample::TestUnknown;
					mysamples.push_back(s);
					//							std::cout<<"REFUSED :TestProjWToI\n";
					continue;
				}
				s.systeme_geodesique = el->Attribute("systeme_geodesique");
				s.TestType = Sample::TestProjWToI;
				mysamples.push_back(s);
				//						std::cout<<"ACCEPTED:TestProjWToI\n";
				continue;
			}

			else if (el->Attribute("TestType") == Sample::TestTransfo2D) {
				//						std::cout<<"TestTransfo2D\n";
				if (el->QueryDoubleAttribute("ci", &s.c) != TIXML_SUCCESS
						|| el->QueryDoubleAttribute("li", &s.l)
								!= TIXML_SUCCESS || el->QueryDoubleAttribute(
						"cg", &s.x) != TIXML_SUCCESS
						|| el->QueryDoubleAttribute("lg", &s.y)
								!= TIXML_SUCCESS) {
					s.TestType = Sample::TestUnknown;
					mysamples.push_back(s);
					//							std::cout<<"REFUSED :TestTransfo2D\n";
					continue;
				}
				s.TestType = Sample::TestTransfo2D;
				//						std::cout<<"s.TestType = "<<s.TestType<<std::endl;
				mysamples.push_back(s);
				//						std::cout<<"ACCEPTED :TestTransfo2D\n";
				continue;
			}

			//d'autre test a prevoire
			else {
				//						std::cout<<"UNKNOWN \n";
				s.TestType = Sample::TestUnknown;
				mysamples.push_back(s);
				continue;
			}
		}
	}
}

//		shared_ptr<Homographie2D> ReadHomographie2D(TiXmlHandle * h_tr2delem)
//		{
//			shared_ptr<Homographie2D> tr = shared_ptr<Homographie2D>(new Homographie2D(false));
//			//	tr->SetC(1.0/scale,0,x0 );
//			//	tr->SetL(0,1.0/scale,y0 );

//		}

void ReadTransfo2D(const TiXmlHandle* h, Transfo2D & tr2D) {
	TiXmlElement * pElem;
	pElem = h->FirstChildElement("transfo2d").Element();
	if (!pElem)
		return;
	TiXmlHandle h_transfo2d= TiXmlHandle(pElem);
	for (unsigned int i=0; h_transfo2d.Child(i).ToElement() != 0; ++i) 
	{
		TiXmlElement * e_tr2delem = h_transfo2d.Child(i).ToElement();
		std::cout<<"e_tr2delem ->Attribute(\"Type\") : "
				<<std::string(e_tr2delem ->Attribute("Type"))<<std::endl;
		
		if (std::string(e_tr2delem ->Attribute("Type")) == "homographie2d") 
		{
			std::cout<< " transformation est une homographie2D!\n";

			double c[3], l[3];
			if (	   e_tr2delem->QueryDoubleAttribute("c0", &c[0]) 
							== TIXML_SUCCESS
					&& e_tr2delem->QueryDoubleAttribute("c1", &c[1])
							== TIXML_SUCCESS
					&& e_tr2delem->QueryDoubleAttribute("c2", &c[2])
							== TIXML_SUCCESS
					&& e_tr2delem->QueryDoubleAttribute("l0", &l[0])
							== TIXML_SUCCESS
					&& e_tr2delem->QueryDoubleAttribute("l1", &l[1])
							== TIXML_SUCCESS
					&& e_tr2delem->QueryDoubleAttribute("l2", &l[2])
							== TIXML_SUCCESS
					&& e_tr2delem->Attribute("isinterne") ) 
			{
				bool isintern = false;
				if (e_tr2delem->Attribute("isinterne") == "true")
					isintern = true;
				shared_ptr<Homographie2D> tr =
						shared_ptr<Homographie2D>(new Homographie2D(isintern));
				tr->SetC(c[0], c[1], c[2]);
				tr->SetL(l[0], l[1], l[2]);
				tr2D.AddTransformation2D(tr);
			}
			else 
			{
				std::string os;
				os = "Type de transformation "
						+ std::string(e_tr2delem ->Attribute("Type"))
						+ " the format is not respected ";
				throw std::logic_error(os);
			}
		} 
		
		
		//ProjectiveTransfo2D
		
		else if (std::string(e_tr2delem ->Attribute("Type")) == "projectivetransfo2D") 
			{
					std::cout<< " transformation est une projectivetransfo2D!\n";

					double a1[3]; 
					double a2[3];
					double a3[2];
					
					if (	   e_tr2delem->QueryDoubleAttribute("a11", &a1[0]) 
									== TIXML_SUCCESS
							&& e_tr2delem->QueryDoubleAttribute("a12", &a1[1])
									== TIXML_SUCCESS
							&& e_tr2delem->QueryDoubleAttribute("a13", &a1[2])
									== TIXML_SUCCESS
							&& e_tr2delem->QueryDoubleAttribute("a21", &a2[0]) 
									== TIXML_SUCCESS
							&& e_tr2delem->QueryDoubleAttribute("a22", &a2[1])
									== TIXML_SUCCESS
							&& e_tr2delem->QueryDoubleAttribute("a23", &a2[2])
									== TIXML_SUCCESS
							&& e_tr2delem->QueryDoubleAttribute("a31", &a3[0]) 
									== TIXML_SUCCESS
							&& e_tr2delem->QueryDoubleAttribute("a32", &a3[1])
									== TIXML_SUCCESS
							&& e_tr2delem->Attribute("isinterne") ) 
					{
						bool isintern = false;
						if (e_tr2delem->Attribute("isinterne") == "true")
							isintern = true;
						shared_ptr<ProjectiveTransfo2D> tr =
								shared_ptr<ProjectiveTransfo2D>(new ProjectiveTransfo2D(isintern));
						tr->SetParameters(a1,a2,a3);						
						tr2D.AddTransformation2D(tr);
					}
					else 
					{
						std::string os;
						os = "Type de transformation "
							+ std::string(e_tr2delem ->Attribute("Type"))
						+ " the format is not respected ";
						throw std::logic_error(os);
					}
		} 
		else {
					std::string os;
					os = "Type de transformation "
							+ std::string(e_tr2delem ->Attribute("Type"))
							+ " non gere! ";
					throw std::logic_error(os);
		}	
	}

}

void WriteSamples(TiXmlElement* elem, std::vector <Sample> const & mysamples) {
	//	std::cout<<"writeSamples\n";
	TiXmlElement *h_samples = new TiXmlElement("samples");
	elem->LinkEndChild(h_samples);

	for (int i = 0; i < mysamples.size(); i++) {
		//		std::cout<<"Ecriture de sample["<<i<<"]\n"; 		
		if (mysamples[i].TestType == Sample::TestUnknown)
			continue;
		//		std::cout<<"TestType not UNKNOWN\n";
		TiXmlElement * h_sample = new TiXmlElement("sample");
		h_samples->LinkEndChild(h_sample);

		//		std::cout<<"mysamples[i].TestType = "<<mysamples[i].TestType<<std::endl;
		if (mysamples[i].TestType == Sample::TestProjWToI) {
			//			std::cout<<"TestType TestProjWToI\n";
			h_sample->SetAttribute("TestType", Sample::TestProjWToI.c_str());
			h_sample->SetAttribute("systeme_geodesique",
					mysamples[i].systeme_geodesique.c_str());
			h_sample->SetDoubleAttribute("x", mysamples[i].x);
			h_sample->SetDoubleAttribute("y", mysamples[i].y);
			h_sample->SetDoubleAttribute("z", mysamples[i].z);
			h_sample->SetDoubleAttribute("c", mysamples[i].c);
			h_sample->SetDoubleAttribute("l", mysamples[i].l);
		}

		else if (mysamples[i].TestType == Sample::TestTransfo2D) {
			//			std::cout<<"TestType TestTransfo2D\n";
			h_sample->SetAttribute("TestType", Sample::TestTransfo2D.c_str());
			h_sample->SetDoubleAttribute("ci", mysamples[i].c);
			h_sample->SetDoubleAttribute("li", mysamples[i].l);
			h_sample->SetDoubleAttribute("cg", mysamples[i].x);
			h_sample->SetDoubleAttribute("lg", mysamples[i].y);
		}
	}
}

bool WriteDate(TiXmlElement * elem, const Date &date) {
	//			std::cout<<"WriteDate"<<std::endl;
	short annee;
	unsigned char mois;
	unsigned char jour;
	unsigned char heure;
	unsigned char minute;
	float seconde;
	std::string time_system;
	date.Get(annee, mois, jour, heure, minute, seconde, time_system);
	//			std::cout<<(int)annee<<","<<(int)mois<<","<<(int)jour<<","<<(int)heure<<","<<(int)minute<<","<<seconde<<","<<time_system<<std::endl;
	std::string vect_tag[7];
	vect_tag[0] = "year";
	vect_tag[1] = "month";
	vect_tag[2] = "day";
	vect_tag[3] = "hour";
	vect_tag[4] = "minute";
	vect_tag[5] = "second";
	vect_tag[6] = "time_system";
	std::ostringstream oss[7];
	oss[0] << (int) annee;
	oss[1] << (int) mois;
	oss[2] << (int) jour;
	oss[3] << (int) heure;
	oss[4] << (int) minute;
	oss[5] << std::setprecision(12)<<seconde;
	oss[6] << time_system;
	for (int i = 0; i < 7; i++) {
		//				std::cout << vect_tag[i] << ";" << oss[i].str() << std::endl;
		TiXmlElement * d = new TiXmlElement(vect_tag[i].c_str());
		elem->LinkEndChild(d);
		d->LinkEndChild(new TiXmlText( oss[i].str().c_str()));
	}
	return true;
}

bool WritePt3d(TiXmlElement * elem, double x, double y, double z) {
	TiXmlElement *h_x = new TiXmlElement("x");
	TiXmlElement *h_y = new TiXmlElement("y");
	TiXmlElement *h_z = new TiXmlElement("z");
	elem->LinkEndChild(h_x);
	elem->LinkEndChild(h_y);
	elem->LinkEndChild(h_z);

	std::ostringstream oss[3];
	oss[0] <<std::setprecision(12)<< x;
	oss[1] << std::setprecision(12)<<y;
	oss[2] << std::setprecision(12)<<z;
	h_x->LinkEndChild(new TiXmlText(oss[0].str().c_str()));
	h_y->LinkEndChild(new TiXmlText(oss[1].str().c_str()));
	h_z->LinkEndChild(new TiXmlText(oss[2].str().c_str()));
	return true;
}

bool WriteMat3D(TiXmlElement * elem, const double *l1, const double *l2,
		const double *l3) {
	TiXmlElement *h_l1 = new TiXmlElement ("l1");
	TiXmlElement *h_l2 = new TiXmlElement ("l2");
	TiXmlElement *h_l3 = new TiXmlElement ("l3");
	TiXmlElement *h_pt3d1 = new TiXmlElement ("pt3d");
	TiXmlElement *h_pt3d2 = new TiXmlElement ("pt3d");
	TiXmlElement *h_pt3d3 = new TiXmlElement ("pt3d");
	h_l1->LinkEndChild(h_pt3d1);
	h_l2->LinkEndChild(h_pt3d2);
	h_l3->LinkEndChild(h_pt3d3);

	elem->LinkEndChild(h_l1);
	elem->LinkEndChild(h_l2);
	elem->LinkEndChild(h_l3);
	WritePt3d(h_pt3d1, l1[0], l1[1], l1[2]);
	WritePt3d(h_pt3d2, l2[0], l2[1], l2[2]);
	WritePt3d(h_pt3d3, l3[0], l3[1], l3[2]);
	return true;
}

void WriteHomographie2D(TiXmlElement * e_orientation,
		const shared_ptr<Homographie2D> h2d) {
	TiXmlHandle h_orientation(e_orientation);
	TiXmlElement * e_transfo2d;
	e_transfo2d = h_orientation.FirstChildElement("transfo2d").Element();
	if (!e_transfo2d) {
		e_transfo2d = new TiXmlElement("transfo2d");
		e_orientation->LinkEndChild(e_transfo2d);
	}

	TiXmlElement * h_tr2delem= new TiXmlElement("tr2delem");
	e_transfo2d->LinkEndChild(h_tr2delem);
	std::string isintern;
	if (h2d->IsInterne())
		isintern = "true";
	else
		isintern = "false";

	h_tr2delem->SetAttribute("Type", "homographie2d");
	h_tr2delem->SetAttribute("isinterne", isintern.c_str());

	h_tr2delem->SetDoubleAttribute("c0", h2d->GetColomn()[0]);
	h_tr2delem->SetDoubleAttribute("c1", h2d->GetColomn()[1]);
	h_tr2delem->SetDoubleAttribute("c2", h2d->GetColomn()[2]);

	h_tr2delem->SetDoubleAttribute("l0", h2d->GetLine()[0]);
	h_tr2delem->SetDoubleAttribute("l1", h2d->GetLine()[1]);
	h_tr2delem->SetDoubleAttribute("l2", h2d->GetLine()[2]);
}

void WriteProjectiveTransfo2D(TiXmlElement * e_orientation,
		shared_ptr<ProjectiveTransfo2D> h2d) {
	TiXmlHandle h_orientation(e_orientation);
	TiXmlElement * e_transfo2d;
	e_transfo2d = h_orientation.FirstChildElement("transfo2d").Element();
	if (!e_transfo2d) {
		e_transfo2d = new TiXmlElement("transfo2d");
		e_orientation->LinkEndChild(e_transfo2d);
	}

	TiXmlElement * h_tr2delem= new TiXmlElement("tr2delem");
	e_transfo2d->LinkEndChild(h_tr2delem);
	std::string isintern;
	if (h2d->IsInterne())
		isintern = "true";
	else
		isintern = "false";

	h_tr2delem->SetAttribute("Type", "projectivetransfo2D");
	h_tr2delem->SetAttribute("isinterne", isintern.c_str());

	h_tr2delem->SetDoubleAttribute("a11", h2d->Geta1()[0]);
	h_tr2delem->SetDoubleAttribute("a12", h2d->Geta1()[1]);
	h_tr2delem->SetDoubleAttribute("a13", h2d->Geta1()[2]);

	h_tr2delem->SetDoubleAttribute("a21", h2d->Geta2()[0]);
	h_tr2delem->SetDoubleAttribute("a22", h2d->Geta2()[1]);
	h_tr2delem->SetDoubleAttribute("a23", h2d->Geta2()[2]);

	h_tr2delem->SetDoubleAttribute("a31", h2d->Geta3()[0]);
	h_tr2delem->SetDoubleAttribute("a32", h2d->Geta3()[1]);

}

}
;
}
;
