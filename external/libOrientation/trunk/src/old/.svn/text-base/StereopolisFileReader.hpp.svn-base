#ifndef STEREOPOLISFILEREADER_HPP_
#define STEREOPOLISFILEREADER_HPP_
#include "boost/filesystem.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/algorithm/string/split.hpp"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>


namespace Stereopolis
{
// This function get the umage file name and 
//give the corresponding orientation file name
class Reader
{
public:
	static std::string GetOrientationName(std::string ImageFileName);
//private:
	static int GetInfo(std::string ImageFileName,std::string &session,
			std::string &section);
	static int ReadList(std::string ListFileName,std::vector< std::string> & vect);
	
};
}

#endif /*STEREOPOLISFILEREADER_HPP_*/
