#include "StereopolisFileReader.hpp"

std::string Stereopolis::Reader::GetOrientationName(std::string ImageFileName)
{
	std::string repimg = boost::filesystem::path(ImageFileName).branch_path().string();;
	std::string repdate = boost::filesystem::path(repimg).branch_path().string();;
	std::string ficimg = boost::filesystem::basename(ImageFileName);
//	std::string ext = boost::filesystem::extension(nomfic);
	
	std::cout<<"repimg : "<<repimg<<"  repdate : "<<repdate<<"  ficimg : "<<ficimg<<std::endl;
	std::string session,section;
	GetInfo(ficimg,session,section);
	std::cout<<"session : "<<session<<std::endl;
	std::cout<<"section : "<<section<<std::endl;
	std::string repori = repdate + "/Record_Stereopolis_meta_datas/PosPac/session_"+session + "/section_"+section;
	std::string nomori = repori + "/" + ficimg + ".xml";
	std::cout<<" nomori : "<<nomori<<std::endl;
	return nomori;
	
	
}

int Stereopolis::Reader::GetInfo(std::string ImageFileName,std::string &session,
		std::string &section)
{
	std::string ficimg = boost::filesystem::basename(ImageFileName);
	std::vector<std::string> SplitVec; 
	boost::algorithm::split( SplitVec, ficimg,  boost::is_any_of("-_") );
//	for ( int i = 0 ; i < SplitVec.size(); i++)
//			std::cout<< "SplitVec["<<i<<"] : "<<SplitVec[i]<<std::endl; 
		 
	
	boost::trim_left_if(SplitVec[3],boost::is_any_of("0"));
	boost::trim_left_if(SplitVec[5],boost::is_any_of("0"));
	
	
	session = SplitVec[3];
	section = SplitVec[5];
	
//	for ( int i = 0 ; i < SplitVec.size(); i++)
//			std::cout<< "SplitVec["<<i<<"] : "<<SplitVec[i]<<std::endl; 
		
	
}

int  Stereopolis::Reader::ReadList(std::string ListFileName,std::vector< std::string> & vect)
{
	std::ifstream fic(ListFileName.c_str());
	if (!fic) return 0 ; 
	while (!fic.eof())
	{
		std::string tmp; 
		fic>> tmp;
		vect.push_back(tmp);
	}
	return 1;
}
