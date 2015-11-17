//#include "internalorientation.hpp"
//using namespace MatisOrientation;
#include "modeleprojection.hpp"

#include <iostream>
#include "boost/algorithm/string/split.hpp"
#include <boost/algorithm/string/trim.hpp>
#include "boost/algorithm/string/case_conv.hpp"
 #include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include<cmath>
#include<iomanip>

using namespace OrientationMATIS;
 
int main2(int argc, char ** argv)
{
//	InternalOrientation::InitAllIO();
//
	//boost::shared_ptr<InternalOrientation> io = InternalOrientation::ReadFile( "Sample_conic.xml" );
//	std::cout.precision(15);
//	std::cout << io->GetcPPA() << std::endl;
//
//	InternalOrientation::WriteFile( io , "Copie_Sample_conic.xml" );
//	
////	boost::shared_ptr<InternalOrientation> ori_copy;
//	std::cout<<"io->GetlPPA() : "<<io->GetlPPA()<<std::endl;
//	
//	InternalOrientation ori_copy = *io;
//	std::cout<<"ori_copy->GetlPPA() : "<<ori_copy.GetlPPA()<<std::endl;
////	*(ori_copy) = *(io);
//	
//	ori_copy.SetlPPA(0);
//	std::cout<<"ori_copy->GetlPPA() : "<<ori_copy.GetlPPA()<<std::endl;
//	std::cout<<"io->GetlPPA() : "<<io->GetlPPA()<<std::endl;
//	std::cout << io->GetGeodeticSystemName() << std::endl;
	
	try
	{
		std::cout<<" creeation ModeleProjection"<<std::endl;
			const std::string nom = "/home/bahman/work/Test_ori/olivier_ecrit.xml";
//			std::string ns = "Lamber93";
//			std::string ns_copi = ns;
////			boost::algorithm::to_upper(ns);
////			std::cout<<"ns = "<<ns<<std::endl;
//			 
//			boost::algorithm::trim_right_if(ns,boost::is_any_of("0123456789"));
//			std::cout<<"ns_copi  : "<<ns_copi << "ns : "<<ns<<std::endl;
//			boost::algorithm::trim_left_if(ns_copi,boost::is_any_of(ns));
//			std::cout<<"ns_copi  : "<<ns_copi <<std::endl;
			
			
			
			
			ModeleProjection::InitAllIO();
			boost::shared_ptr<ModeleProjection> io = ModeleProjection::ReadFile( nom );	
			std::cout<<" systemeGeodesie : "<<io->GetSystemGeodesie()<<std::endl;
			
//			std::vector<std::string> SplitVec;
//			std::string str = io->GetSystemGeodesie();
//			boost::algorithm::split(SplitVec, str, boost::is_any_of(": "));
//			for ( int i = 0 ; i < SplitVec.size(); i++)
//			{
//				std::cout<<SplitVec[i]<<std::endl;
//			}
//			
			
			
			//			double x0,y0,z0, x1, y1, z1;
//			double c0 = 100;
//			double l0 = 200;
//			double c,l;
//			double x = 657721.99; 
//			double y = 6860584.61;
//			double z = 99.96-(126.249-82.45);
//			io->LocalToImage(x, y, z,c, l);
		//	io->ImageToLocalVec(c0,l0, x0,y0,z0, x1, y1, z1);
		//	std::cout<<"(C0,l0) = "<<c0<<","<<l0<<std::endl;
		//	std::cout<<"(x,y,z) = "<<x1<<","<<y1<<","<<z1<<std::endl;
//			std::cout<<"(C,l) = "<<c<<","<<l<<std::endl;
//			std::cout<<" creeation ModeleProjection fini !"<<std::endl;
			if (!ModeleProjection::WriteFile(io,"/home/bahman/work/Test_ori/olivier_ecrit_ecrit.xml")) 
				std::cout<<"probleme d'ecriture! "<<std::endl;
			
			std::cout<<"fin de program ! "<<std::endl;
			

		
	}
	catch (const std::logic_error& e)
			{
				std::cout<<"Erreur!"<<std::endl;
				throw e;
			}
	return 0;
}  

int main1(void)
{
	ModeleProjection::InitAllIO();
		
//	std::string nomori34 = "/home/bahman/work/Test_ori/Geodesie/Pantheon-080506_0588-34-00001_0000263.xml";
//	std::string nomori33 = "/home/bahman/work/Test_ori/Geodesie/Pantheon-080506_0588-33-00001_0000263.xml";
	std::string nomori33 = "/home/bahman/work/Test_ori/bertrand/Paris_12-080428_0807-33-00001_0000607.xml";
	std::string nomori34 = "/home/bahman/work/Test_ori/bertrand/Paris_12-080428_0807-34-00001_0000607.xml";
	
	boost::shared_ptr<ModeleProjection> ori34 = ModeleProjection::ReadFile( nomori34);
	boost::shared_ptr<ModeleProjection> ori33 = ModeleProjection::ReadFile( nomori33);
	std::cout<<"cam name h: "<<ori33->GetAuxiliaryData().GetCameraName()<<std::endl;
	double x,y,c,l;
	x = 1811;
	y = 852;
	double x0,y0,z0,x1,y1,z1;
	ori34->ImageToLocalVec(x,y,x0,y0,z0,x1,y1,z1);
	std::cout<<"x0,y0,z0"<<x0<<","<<y0<<","<<z0<<std::endl;
	std::cout<<"x1,y1,z1"<<x1<<","<<y1<<","<<z1<<std::endl;
	 
	double norme = sqrt((x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) + (z1-z0)*(z1-z0));
	double xg,yg,zg;
	xg = x0 + 1000 * (x1-x0)/norme;
	yg = y0 + 1000 * (y1-y0)/norme;
	zg = z0 + 1000 * (z1-z0)/norme;
	
	ori33->LocalToImage(xg,yg,zg,c,l);
	std::cout<<"c,l : "<<c<<" , "<<l<<std::endl;
	
	
	
//	double x,y,z;
//	x = 648237.27;
//	y = 6862256.29;
//	z = 350.9;
//	double c,l;
//	ori->LocalToImage(x,y,z,c,l);
//	std::cout<<"c,l : "<<c<<" , "<<l<<std::endl;
	
	

}

int main (int argc, char **argv)
{
	ModeleProjection::InitAllIO();
	boost::shared_ptr<ModeleProjection> ori = ModeleProjection::ReadFile("Sample_conic.xml");
	ModeleProjection::WriteFile(ori, "Sample_conic_write.xml");
	return 0;
	for (int i=1;i<argc; ++i)
	{
		boost::shared_ptr<ModeleProjection> ori = ModeleProjection::ReadFile(argv[i]);
		if (!(ori->InternalCheck()))
			return -i;
	}
	return 0;	
}

