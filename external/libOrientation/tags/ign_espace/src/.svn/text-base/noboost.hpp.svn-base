#ifndef __NOBOOST_H__
#define __NOBOOST_H__

#ifdef ORIENTATIONMATIS_USE_NOBOOST

#include <exception>
#include <stdexcept>

#include <bits/atomic_word.h>
#include <bits/concurrence.h>
#include <bits/atomicity.h>
#include <bits/stl_function.h>

#include <typeinfo>
#include <memory>

#include <tr1/boost_shared_ptr.h>

using std::tr1::shared_ptr;
using std::tr1::dynamic_pointer_cast;

#include <string>
#include <vector>

std::string extension(std::string const &nom);
std::string path(std::string const &nom);
std::string basename(std::string const &nom);

void split(std::vector<std::string> &splitVec,std::string const &str,std::string const &separateurs);

void to_upper(std::string& chaine);
std::string to_upper_copy(std::string& chaine);
void trim_right_if(std::string &str1,std::string const &isAnyOf);
void trim_left_if(std::string &str1,std::string const &isAnyOf);
void trim(std::string &str);

inline std::string is_any_of(const char *ch) { return std::string(ch); }
inline std::string is_any_of(const std::string &ch) { return ch; }

#else

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;
using boost::dynamic_pointer_cast;

#include <boost/filesystem.hpp>
using boost::filesystem::extension;

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/case_conv.hpp>

using boost::algorithm::split;
using boost::algorithm::trim;
using boost::algorithm::trim_right_if;
using boost::algorithm::trim_left_if;
using boost::to_upper;
using boost::algorithm::to_upper_copy;
using boost::is_any_of;

#endif // #ifdef ORIENTATIONMATIS_USE_NOBOOST

#endif
