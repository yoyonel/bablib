#include "noboost.hpp"
#include <iostream>

#ifdef ORIENTATIONMATIS_USE_NOBOOST

const char dirSepChar='/';

std::string extension(std::string const &nom)
{
	int placePoint = -1;
	for(int l=nom.size()-1;(l>=0)&&(placePoint==-1);--l)
	{
		if (nom[l]=='.')
		{
			placePoint = l;
		}
	}
	std::string ext = std::string("");
	if (placePoint!=-1)
	{
		ext.assign(nom.begin()+placePoint,nom.end());
	}
	//std::cout << "NO BOOST : "<<nom<<" extension : "<<ext<<std::endl;
	return ext;
}

std::string path(std::string const &nom)
{
	int placeSlash = -1;
	for(int l=nom.size()-1;(l>=0)&&(placeSlash==-1);--l)
	{
		if (nom[l]==dirSepChar)
		{
			placeSlash = l;
		}
	}
	std::string path = std::string("");
	if (placeSlash!=-1)
	{
		path.assign(nom.begin(),nom.begin()+placeSlash);
	}
	//std::cout << "NO BOOST : "<<nom<<" path : "<<path<<std::endl;
	return path;
}

std::string basename(std::string const &nom)
{
	int placeSlash = -1;
	for(int l=nom.size()-1;(l>=0)&&(placeSlash==-1);--l)
	{
		if (nom[l]==dirSepChar)
		{
			placeSlash = l;
		}
	}
	std::string name = std::string("");
	if (placeSlash!=-1)
	{
		 name.assign(nom.begin()+placeSlash+1,nom.end());
	}
	int placePoint = -1;
	for(int l=name.size()-1;(l>=0)&&(placePoint==-1);--l)
	{
		if (name[l]=='.')
		{
			placePoint = l;
		}
	}
	std::string basename;
	if (placePoint == -1)
		basename = name;
	else
		basename.assign(name.begin(),name.begin()+placePoint);
	//std::cout << "NO BOOST : "<<nom<<" basename : "<<basename<<std::endl;
	return basename;
}


void split(std::vector<std::string> &splitVec,std::string const &str,std::string const &separateurs)
{
	splitVec.clear();
	int debut=0;
	int pos=0;
	while(pos<str.size())
	{
		bool isSeparateur=false;
		for(size_t i=0;(i<separateurs.size())&&(!isSeparateur);++i)
			isSeparateur = (str[pos] == separateurs[i]);
		if (isSeparateur)
		{
			if (pos>debut)
			{
				std::string newBlock(str.begin()+debut,str.begin()+pos);
				splitVec.push_back(newBlock);
				debut=pos+1;
			}
		}
		++pos;
	}
	if (pos>debut)
	{
		std::string newBlock(str.begin()+debut,str.begin()+pos);
		splitVec.push_back(newBlock);
	}
	/*
	std::cout << "NO BOOST : split : "<<str<<std::endl;
	for(size_t i=0;i<splitVec.size();++i)
	{
		std::cout << "=== : "<<splitVec[i]<<std::endl;
	}
	 */
}

void to_upper(std::string& chaine)
{
	//std::cout << "NO BOOST : to_upper : "<<chaine<<std::endl;
	std::transform(chaine.begin(), chaine.end(), chaine.begin(),(int(*)(int)) std::toupper);
	//std::cout << "=== "<<chaine<<std::endl;
}

std::string to_upper_copy(std::string& chaine)
{
	std::string copie(chaine);
	//std::cout << "NO BOOST : to_upper : "<<copie<<std::endl;
	to_upper(copie);
	//std::cout << "=== "<<copie<<std::endl;
	return copie;
}

void trim_if(std::string &str1,std::string const &isAnyOf)
{
	//std::cout << "NO BOOST : trim_if : "<<str1<<std::endl;
	for(size_t pos=0;pos<str1.size();++pos)
	{
		bool isSeparateur=false;
		for(size_t i=0;(i<isAnyOf.size())&&(!isSeparateur);++i)
			isSeparateur = (str1[pos] == isAnyOf[i]);
		if (isSeparateur)
		{
			str1.erase(str1.begin()+pos);
			--pos;
		}
	}
	//std::cout << "=== "<<str1<<std::endl;
}

void trim_right_if(std::string &str1,std::string const &isAnyOf)
{
	trim_if(str1,isAnyOf);
}

void trim_left_if(std::string &str1,std::string const &isAnyOf)
{
	trim_if(str1,isAnyOf);
}

void trim(std::string &str)
{
	trim_if(str,std::string(" "));
}

#endif
