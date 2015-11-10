#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>

#include "noboost.hpp"

#include "outil_xml.hpp"
#include "systematismeaffine.hpp"
#include "systematismecylindrique.hpp"

namespace OrientationMATIS {
	namespace XmlHandler 
	{
		
		TiXmlElement* AssertRoot(TiXmlDocument * doc, const std::string & rootname)
		{
			TiXmlElement* root = doc->RootElement();
			
			if(std::string(root->Value()) != rootname)
				return NULL;
			
			return root;
		}
		//		<stereopolis>
		//		<chantier>Paris_12</chantier>
		//		<date> 80428</date>
		//		<session>807</session>
		//		<section>1</section>
		//		<numero>1</numero>
		//		<position>11</position>
		//		<time_base_ms> 1 </time_base_ms>
		//		<shutter>1740</shutter>
		//		<flatfield_name>ff_test.tif</flatfield_name>
		//		</stereopolis>
		bool ReadStereopolis(const TiXmlNode* node, Stereopolis & st )
		{
			st.m_Valide = true;
			const TiXmlNode* ChantierNode  = node->FirstChild("chantier");
			const TiXmlNode* DateNode  = node->FirstChild("date");
			const TiXmlNode* SessionNode  = node->FirstChild("session");
			const TiXmlNode* SectionNode  = node->FirstChild("section");
			const TiXmlNode* NumeroNode  = node->FirstChild("numero");
			const TiXmlNode* PositionNode  = node->FirstChild("position");
			const TiXmlNode* Time_base_msNode  = node->FirstChild("time_base_ms");
			const TiXmlNode* ShutterNode  = node->FirstChild("shutter");
			const TiXmlNode* Flatfield_nameNode  = node->FirstChild("flatfield_name");
			
			
			if (ChantierNode != NULL)
				if (ChantierNode->ToElement()->GetText() != NULL) 
					st.m_Chantier= ChantierNode->ToElement()->GetText();
			
			
			if (DateNode != NULL)
				if (DateNode->ToElement()->GetText() != NULL)
					st.m_Date = DateNode->ToElement()->GetText();
			
			if (SessionNode != NULL)
				if (SessionNode->ToElement()->GetText() != NULL)
					st.m_Session = atoi(SessionNode->ToElement()->GetText());
			
			if (SectionNode != NULL)
				if (SectionNode->ToElement()->GetText() != NULL)
					st.m_Section= atoi(SectionNode->ToElement()->GetText());
			
			if (NumeroNode != NULL)
				if (NumeroNode->ToElement()->GetText() != NULL)
					st.m_Numero= atoi(NumeroNode->ToElement()->GetText());
			
			if (PositionNode!= NULL)
				if (PositionNode->ToElement()->GetText() != NULL)
					st.m_Position= atoi(PositionNode->ToElement()->GetText());
			
			if (Time_base_msNode!= NULL)
				if (Time_base_msNode->ToElement()->GetText() != NULL)
					st.m_Time_base_ms= atoi(Time_base_msNode->ToElement()->GetText());
			
			if (ShutterNode != NULL)
				if (ShutterNode->ToElement()->GetText() != NULL)
					st.m_Shutter= atoi(ShutterNode->ToElement()->GetText());
			
			if (Flatfield_nameNode != NULL)
				if (Flatfield_nameNode->ToElement()->GetText() != NULL)
					st.m_Flatfield_name= Flatfield_nameNode->ToElement()->GetText();
			
			return false;
		}
		
		
		bool ReadDate(const TiXmlNode* node, Date &date) 
		{
			const TiXmlNode* DayNode = node->FirstChild("day");
			if(DayNode == NULL)
			{
				int calibration_date[6];
				memset(calibration_date,0,6*sizeof(int));
				if(node->ToElement()->GetText()==NULL)
					return true;
				
				std::string str = node->ToElement()->GetText();
				std::vector<std::string> SplitVec;
				split(SplitVec, str, is_any_of("-/"));
				int taille = SplitVec.size();
				if (taille> 6)
					return false;
				
				int ind = 0;
				for (int i = taille - 1; i >= 0; i--,ind++) 
					calibration_date[ind] = atoi(SplitVec[i].c_str());
				
				
				date.Set(calibration_date[0], calibration_date[1], calibration_date[2],
						 calibration_date[3], (float)calibration_date[4], calibration_date[5]);
				
				return true;
			}
			
			const TiXmlNode* YearNode = node->FirstChild("year");
			const TiXmlNode* MonthNode = node->FirstChild("month");
			const TiXmlNode* HourNode = node->FirstChild("hour");
			const TiXmlNode* MinuteNode = node->FirstChild("minute");
			const TiXmlNode* SecondNode = node->FirstChild("second");
			const TiXmlNode* TimeSystemNode = node->FirstChild("time_system");
			
			
			if(YearNode==NULL || MonthNode==NULL || HourNode==NULL || MinuteNode==NULL || SecondNode==NULL || TimeSystemNode==NULL)
				return false;
			
			int year = atoi(YearNode->ToElement()->GetText());
			int month = atoi(MonthNode->ToElement()->GetText());
			int day = atoi(DayNode->ToElement()->GetText());
			int hour = atoi(HourNode->ToElement()->GetText());
			int minute = atoi(MinuteNode->ToElement()->GetText());
			float second = (float)atof(SecondNode->ToElement()->GetText());
			
			
			
			std::string time_system;
			if(TimeSystemNode->ToElement()->GetText()!=NULL)
				time_system =TimeSystemNode->ToElement()->GetText();
			
			date.Set(year, month, day, hour, minute, second, time_system);
			return true;
		}
		bool ReadPt2D(const TiXmlNode* node, double &x, double &y) 
		{
			TiXmlNode const * X_Node = node->FirstChild("x");
			TiXmlNode const * Y_Node = node->FirstChild("y");
			if(X_Node==NULL || Y_Node==NULL)
				return false;
			
			x = atof(X_Node->ToElement()->GetText());
			y = atof(Y_Node->ToElement()->GetText());
			return true;
		}
		
		bool ReadXPt3D(const TiXmlNode* node,_t_XPt3D & pt)
		{
			const TiXmlNode* Pt3DNode = node->FirstChild("pt3d");
			if(Pt3DNode ==NULL)
				return false;
			
			const TiXmlNode* XNode = Pt3DNode->FirstChild("x");
			const TiXmlNode* YNode = Pt3DNode->FirstChild("y");
			const TiXmlNode* ZNode = Pt3DNode->FirstChild("z");
			
			if(XNode==NULL || YNode==NULL || ZNode==NULL)
				return false;
			
			pt.X = atof(XNode->ToElement()->GetText());
			pt.Y = atof(YNode->ToElement()->GetText());
			pt.Z = atof(ZNode->ToElement()->GetText());
			
			return true;
			
		}
		bool ReadMat3D(const TiXmlNode* node,_t_XMat3D & mat)
		{
			const TiXmlNode* Mat3DNode = node->FirstChild("mat3d");
			if(Mat3DNode ==NULL)
				return false;
			
			const TiXmlNode* l1Node = Mat3DNode->FirstChild("l1");
			const TiXmlNode* l2Node = Mat3DNode->FirstChild("l2");
			const TiXmlNode* l3Node = Mat3DNode->FirstChild("l3");
			
			if(l1Node==NULL || l2Node==NULL || l3Node==NULL)
				return false;
			
			if(!ReadXPt3D(l1Node,mat.l1))
				return false;
			
			if(!ReadXPt3D(l2Node,mat.l2))
				return false;
			
			if(!ReadXPt3D(l3Node,mat.l3))
				return false;
			
			return true;
		}
		
		void ReadSamples(TiXmlNode* node, std::vector <Sample> & mysamples) 
		{
			TiXmlNode* SampleNode = node->FirstChild("samples");
			if(SampleNode ==NULL)
				return;
			
			const TiXmlHandle h_samples = TiXmlHandle(SampleNode);
			for (unsigned int i=0; h_samples.Child(i).ToElement() != 0; ++i) 
			{
				
				Sample s;
				TiXmlElement * el = h_samples.Child(i).ToElement();
				if (!el->Attribute("TestType") || el->Attribute("TestType") == Sample::TestProjWToI) 
				{
					if (el->QueryDoubleAttribute("x", &s.x) != TIXML_SUCCESS
						|| el->QueryDoubleAttribute("y", &s.y) != TIXML_SUCCESS
						|| el->QueryDoubleAttribute("z", &s.z) != TIXML_SUCCESS
						|| el->QueryDoubleAttribute("c", &s.c) != TIXML_SUCCESS
						|| el->QueryDoubleAttribute("l", &s.l) != TIXML_SUCCESS
						|| !el->Attribute("systeme_geodesique")) 
					{
						s.TestType = Sample::TestUnknown;
						mysamples.push_back(s);
						continue;
					}
					s.systeme_geodesique = el->Attribute("systeme_geodesique");
					s.TestType = Sample::TestProjWToI;
					mysamples.push_back(s);
					continue;
				}
				else if (el->Attribute("TestType") == Sample::TestTransfo2D) 
				{
					if (el->QueryDoubleAttribute("ci", &s.c) != TIXML_SUCCESS || 
						el->QueryDoubleAttribute("li", &s.l) != TIXML_SUCCESS || 
						el->QueryDoubleAttribute("cg", &s.x) != TIXML_SUCCESS || 
						el->QueryDoubleAttribute("lg", &s.y) != TIXML_SUCCESS) 
					{
						s.TestType = Sample::TestUnknown;
						mysamples.push_back(s);
						continue;
					}
					s.TestType = Sample::TestTransfo2D;
					mysamples.push_back(s);
					continue;
				}
				else 
				{
					s.TestType = Sample::TestUnknown;
					mysamples.push_back(s);
					continue;
				}
			}
		}
		
		bool ReadTransfo2D(const TiXmlHandle* h, Transfo2D & tr2D) 
		{
			TiXmlElement * pElem;
			pElem = h->FirstChildElement("transfo2d").Element();
			if (!pElem)
				return true;
			
			TiXmlHandle h_transfo2d= TiXmlHandle(pElem);
			
			for (unsigned int i=0; h_transfo2d.Child(i).ToElement() != 0; ++i) 
			{
				TiXmlElement * e_tr2delem = h_transfo2d.Child(i).ToElement();
				if (std::string(e_tr2delem ->Attribute("Type")) == "homographie2d") 
				{
					double c[3], l[3];
					if ( e_tr2delem->QueryDoubleAttribute("c0", &c[0]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("c1", &c[1]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("c2", &c[2]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("l0", &l[0]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("l1", &l[1]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("l2", &l[2]) == TIXML_SUCCESS &&
						e_tr2delem->Attribute("isinterne") ) 
					{
						bool isintern = false;
						if (e_tr2delem->Attribute("isinterne") == "true")
							isintern = true;
						
						shared_ptr<Homographie2D> tr =shared_ptr<Homographie2D>(new Homographie2D(isintern));
						tr->SetC(c[0], c[1], c[2]);
						tr->SetL(l[0], l[1], l[2]);
						tr2D.AddTransformation2D(tr);
						continue;
					}
					
					//std::string os = "Type de transformation " + std::string(e_tr2delem ->Attribute("Type"))+ " the format is not respected ";
					//throw std::logic_error(os);
					return false;
					
				} 
				else if (std::string(e_tr2delem ->Attribute("Type")) == "projectivetransfo2D") 
				{
					double a1[3]; 
					double a2[3];
					double a3[2];
					
					if (	e_tr2delem->QueryDoubleAttribute("a11", &a1[0]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("a12", &a1[1]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("a13", &a1[2]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("a21", &a2[0]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("a22", &a2[1]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("a23", &a2[2]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("a31", &a3[0]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("a32", &a3[1]) == TIXML_SUCCESS &&
						e_tr2delem->Attribute("isinterne") ) 
					{
						bool isintern = false;
						if (e_tr2delem->Attribute("isinterne") == "true")
							isintern = true;
						shared_ptr<ProjectiveTransfo2D> tr =
						shared_ptr<ProjectiveTransfo2D>(new ProjectiveTransfo2D(isintern));
						tr->SetParameters(a1,a2,a3);						
						tr2D.AddTransformation2D(tr);
						continue;
					}
					
					//std::string os = "Type de transformation " + std::string(e_tr2delem ->Attribute("Type")) + " the format is not respected ";
					//throw std::logic_error(os);
					
					return false;
				} 
				else if(std::string(e_tr2delem ->Attribute("Type")) == SystematismeAffineTopAero::nomTagXml)
				{
					double c[5];
					if ( e_tr2delem->QueryDoubleAttribute("C0", &c[0]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("L0", &c[1]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("S6", &c[2]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("S7", &c[3]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("S8", &c[4]) == TIXML_SUCCESS &&
						e_tr2delem->Attribute("isinterne") ) 
					{
						bool isintern = false;
						if (e_tr2delem->Attribute("isinterne") == "true")
							isintern = true;
						
						shared_ptr<SystematismeAffineTopAero> tr =shared_ptr<SystematismeAffineTopAero>(new SystematismeAffineTopAero(c[0],c[1],c[2],c[3],c[4],isintern));
						tr2D.AddTransformation2D(tr);
						continue;
					}
				}
				else if(std::string(e_tr2delem ->Attribute("Type")) == SystematismeCylindriqueTopAero::nomTagXml)
				{
					double c[4];
					if ( e_tr2delem->QueryDoubleAttribute("C0", &c[0]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("L0", &c[1]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("S1", &c[2]) == TIXML_SUCCESS &&
						e_tr2delem->QueryDoubleAttribute("S2", &c[3]) == TIXML_SUCCESS &&
						e_tr2delem->Attribute("isinterne") ) 
					{
						bool isintern = false;
						if (e_tr2delem->Attribute("isinterne") == "true")
							isintern = true;
						
						shared_ptr<SystematismeCylindriqueTopAero> tr =shared_ptr<SystematismeCylindriqueTopAero>(new SystematismeCylindriqueTopAero(c[0],c[1],c[2],c[3],isintern));
						tr2D.AddTransformation2D(tr);
						continue;
					}
				}
				else 
				{
					
					std::string os = "Type de transformation " + std::string(e_tr2delem ->Attribute("Type")) + " non gere! ";
					throw std::logic_error(os);
					return false;
				}	
			}
			
			return true;
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
		
		bool WritePt2d(TiXmlElement * elem, double x, double y) {
			TiXmlElement *h_x = new TiXmlElement("x");
			TiXmlElement *h_y = new TiXmlElement("y");
			elem->LinkEndChild(h_x);
			elem->LinkEndChild(h_y);
			
			std::ostringstream oss[2];
			oss[0] <<std::setprecision(12)<< x;
			oss[1] << std::setprecision(12)<<y;
			h_x->LinkEndChild(new TiXmlText(oss[0].str().c_str()));
			h_y->LinkEndChild(new TiXmlText(oss[1].str().c_str()));
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
		
		bool WriteStereopolis(TiXmlElement * node, const Stereopolis & st)
		{
			//		<stereopolis>
			//		<chantier>Paris_12</chantier>
			//		<date> 80428</date>
			//		<session>807</session>
			//		<section>1</section>
			//		<numero>1</numero>
			//		<position>11</position>
			//		<time_base_ms> 1 </time_base_ms>
			//		<shutter>1740</shutter>
			//		<flatfield_name>ff_test.tif</flatfield_name>
			//		</stereopolis>
			if (!st.m_Valide) return false;
			TiXmlElement* h_st = new TiXmlElement("stereopolis");
			node->LinkEndChild(h_st);
			
			TiXmlElement *h_chantier = new TiXmlElement("chantier");
			h_chantier->LinkEndChild(new TiXmlText(st.m_Chantier.c_str()));
			h_st->LinkEndChild(h_chantier);
			
			TiXmlElement *h_date= new TiXmlElement("date");
			h_date->LinkEndChild(new TiXmlText(st.m_Date.c_str()));
			h_st->LinkEndChild(h_date);
			
			std::ostringstream oss;
			TiXmlElement *h_session= new TiXmlElement("session");
			oss << st.m_Session;
			h_session->LinkEndChild(new TiXmlText(oss.str().c_str()));
			h_st->LinkEndChild(h_session);
			
			oss.str("");
			TiXmlElement *h_section= new TiXmlElement("section");
			oss << st.m_Section;
			h_section->LinkEndChild(new TiXmlText(oss.str().c_str()));
			h_st->LinkEndChild(h_section);
			
			oss.str("");
			TiXmlElement *h_numero = new TiXmlElement("numero");
			oss << st.m_Numero;
			h_numero->LinkEndChild(new TiXmlText(oss.str().c_str()));
			h_st->LinkEndChild(h_numero);
			
			oss.str("");
			TiXmlElement *h_postion = new TiXmlElement("position");
			oss << st.m_Position;
			h_postion->LinkEndChild(new TiXmlText(oss.str().c_str()));
			h_st->LinkEndChild(h_postion);
			
			oss.str("");
			TiXmlElement *h_time_base_ms= new TiXmlElement("time_base_ms");
			oss << st.m_Time_base_ms;
			h_time_base_ms->LinkEndChild(new TiXmlText(oss.str().c_str()));
			h_st->LinkEndChild(h_time_base_ms);
			
			oss.str("");
			TiXmlElement *h_shutter= new TiXmlElement("shutter");
			oss << st.m_Shutter;
			h_shutter->LinkEndChild(new TiXmlText(oss.str().c_str()));
			h_st->LinkEndChild(h_shutter);
			
			TiXmlElement *h_flatfield_name= new TiXmlElement("flatfield_name");
			h_flatfield_name->LinkEndChild(new TiXmlText(st.m_Flatfield_name.c_str()));
			h_st->LinkEndChild(h_flatfield_name);
			return true; 
		}
		
		
		bool WriteAuxiliaryData(TiXmlElement * node, const AuxiliaryData & aux)
		{
			/** Writing auxiliary data */
			
			TiXmlElement* h_aux = new TiXmlElement("auxiliarydata");
			node->LinkEndChild(h_aux);
			
			TiXmlElement *h_image_name = new TiXmlElement("image_name");
			h_image_name->LinkEndChild(new TiXmlText(aux.GetImageName().c_str()));
			h_aux->LinkEndChild(h_image_name);
			
			//TiXmlElement * h_stereopolis = new TiXmlElement("stereopolis");
			WriteStereopolis(h_aux, aux.GetStereopolis());
			//	h_stereopolis->LinkEndChild(new TiXmlText("UNKNOWN"));
			//	h_aux->LinkEndChild(h_stereopolis);
			//	WriteStereopolis(node, aux.GetStereopolis());
			
			//	std::cout << "av date" << std::endl;
			TiXmlElement * h_image_date = new TiXmlElement("image_date");
			XmlHandler::WriteDate(h_image_date, aux.GetImageDate());
			h_aux->LinkEndChild(h_image_date);
			//	std::cout << "ap date" << std::endl;
			
			/** Writing auxiliary data samples */
			std::vector< Sample> VecSample = aux.GetSamples();
			XmlHandler::WriteSamples(h_aux, VecSample);
			
			return true;
		}
		
		void WriteSystematismeAffineTopAero(TiXmlElement * elem,shared_ptr<SystematismeAffineTopAero> const &sys)
		{
			std::cout << "WriteSystematismeAffineTopAero"<<std::endl;
			TiXmlHandle h_orientation(elem);
			TiXmlElement * e_transfo2d;
			e_transfo2d = h_orientation.FirstChildElement("transfo2d").Element();
			if (!e_transfo2d) {
				e_transfo2d = new TiXmlElement("transfo2d");
				elem->LinkEndChild(e_transfo2d);
			}
			
			TiXmlElement * h_tr2delem= new TiXmlElement("tr2delem");
			e_transfo2d->LinkEndChild(h_tr2delem);
			std::string isintern;
			if (sys->IsInterne())
				isintern = "true";
			else
				isintern = "false";
			
			h_tr2delem->SetAttribute("Type", sys->nomTagXml);
			h_tr2delem->SetAttribute("isinterne", isintern.c_str());
			h_tr2delem->SetDoubleAttribute("C0", sys->GetC0());
			h_tr2delem->SetDoubleAttribute("L0", sys->GetL0());
			h_tr2delem->SetDoubleAttribute("S6", sys->GetS6());
			h_tr2delem->SetDoubleAttribute("S7", sys->GetS7());
			h_tr2delem->SetDoubleAttribute("S8", sys->GetS8());
		}
		
		void WriteSystematismeCylindriqueTopAero(TiXmlElement * elem,shared_ptr<SystematismeCylindriqueTopAero> const &sys)
		{
			std::cout << "WriteSystematismeCylindriqueTopAero"<<std::endl;
			TiXmlHandle h_orientation(elem);
			TiXmlElement * e_transfo2d;
			e_transfo2d = h_orientation.FirstChildElement("transfo2d").Element();
			if (!e_transfo2d) {
				e_transfo2d = new TiXmlElement("transfo2d");
				elem->LinkEndChild(e_transfo2d);
			}
			
			TiXmlElement * h_tr2delem= new TiXmlElement("tr2delem");
			e_transfo2d->LinkEndChild(h_tr2delem);
			std::string isintern;
			if (sys->IsInterne())
				isintern = "true";
			else
				isintern = "false";
			
			h_tr2delem->SetAttribute("Type", sys->nomTagXml);
			h_tr2delem->SetAttribute("isinterne", isintern.c_str());
			h_tr2delem->SetDoubleAttribute("C0", sys->GetC0());
			h_tr2delem->SetDoubleAttribute("L0", sys->GetL0());			
			h_tr2delem->SetDoubleAttribute("S1", sys->GetS1());
			h_tr2delem->SetDoubleAttribute("S2", sys->GetS2());
		}
		
	}
	
}

