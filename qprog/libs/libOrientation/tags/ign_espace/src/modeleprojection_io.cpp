#include <map>
#include <iostream>
#include <sstream>

#include "noboost.hpp"

#include "modeleprojection.hpp"

namespace OrientationMATIS 
{
typedef shared_ptr<ModeleProjection> (*PtrFonctReaderModeleProjection)(
		const std::string &);
typedef bool (*PtrFonctWriterModeleProjection)(
		const shared_ptr<ModeleProjection>, const std::string &);

typedef std::multimap<std::string, PtrFonctReaderModeleProjection>
		MapReaderModeleProjection;
typedef std::multimap<std::string, PtrFonctWriterModeleProjection>
		MapWriterModeleProjection;

static MapReaderModeleProjection & GetReaders() {
	static MapReaderModeleProjection readers;
	return readers;
}

static MapWriterModeleProjection & GetWriters() {
	static MapWriterModeleProjection writers;
	return writers;
}

/// Lecture de differents formats
/// .xml : conique
/// TODO : ajouter spherique
shared_ptr<ModeleProjection> xml_reader(const std::string &filename);

/// .ori
shared_ptr<ModeleProjection>
		conic_ori_reader(const std::string &filename);
shared_ptr<ModeleProjection>
		axono_ori_reader(const std::string &filename);
shared_ptr<ModeleProjection>
		carto_ori_reader(const std::string &filename);

/// .hdr : ArcView 
shared_ptr<ModeleProjection>
		carto_hdr_reader(const std::string &filename);

/// .xml : MICMAC
shared_ptr<ModeleProjection> carto_micmac_reader(
		const std::string &filename);

#if ORIENTATIONMATIS_USE_XERCES
/// .rto : modele polynomial
shared_ptr<ModeleProjection> rto_reader(const std::string &filename);
/// .gri : modele grille
shared_ptr<ModeleProjection> gri_reader(const std::string &nom);
#endif //#if ORIENTATIONMATIS_USE_XERCES
/// Ecriture de differents formats 
/// .xml
bool xml_writer(const shared_ptr<ModeleProjection>ori,
		const std::string & filename);

void ModeleProjection::InitAllIO() {
	{
		GetReaders().clear();

		GetReaders().insert(make_pair(".xml", xml_reader));
		GetReaders().insert(make_pair(".ori", conic_ori_reader));
		GetReaders().insert(make_pair(".ori", axono_ori_reader));
		GetReaders().insert(make_pair(".ori", carto_ori_reader));
		GetReaders().insert(make_pair(".hdr", carto_hdr_reader));
		GetReaders().insert(make_pair(".xml", carto_micmac_reader));

#if ORIENTATIONMATIS_USE_XERCES
		GetReaders().insert(make_pair("rto", rto_reader));
		GetReaders().insert(make_pair("gri", gri_reader));
#endif //#if ORIENTATIONMATIS_USE_XERCES
	}
	{
		GetWriters().clear();
		GetWriters().insert(make_pair(".xml", xml_writer) );
	}
}

shared_ptr<ModeleProjection> ModeleProjection::ReadFile(
		const std::string &filename) {
	std::string ext=extension(filename);

	MapReaderModeleProjection::const_iterator it_ext = GetReaders().lower_bound(ext);
	MapReaderModeleProjection::const_iterator it = it_ext;

	// D'abord ceux qui gèrent à peu près la même extension
	for (it = it_ext; it != GetReaders().end(); ++it) {

		shared_ptr<ModeleProjection> base = (*it).second(filename);
		if (base != shared_ptr<ModeleProjection>())
		return base;
	}
	// Puis tous les autres
	for (it = GetReaders().begin(); it != it_ext; ++it)
	{

		shared_ptr<ModeleProjection> base = (*it).second(filename);
		if (base != shared_ptr<ModeleProjection>())
		return base;
	}

	return shared_ptr<ModeleProjection>();
}

bool ModeleProjection::WriteFile(const shared_ptr<ModeleProjection> ori,
const std::string &filename)
{
	//	std::cout<<"ModeleProjection::WriteFile"<<std::endl;
	std::string ext=extension(filename);

	MapWriterModeleProjection::const_iterator it_ext = GetWriters().lower_bound(ext);
	MapWriterModeleProjection::const_iterator it = it_ext;

	// D'abord ceux qui gèrent à peu près la même extension
	for (it = it_ext; it != GetWriters().end(); ++it)
	{
		if ((*it).second(ori, filename))
		return true;
	}
	// Puis tous les autres
	for (it = GetWriters().begin(); it != it_ext; ++it)
	{
		if ((*it).second(ori, filename))
		return true;
	}
	return false;
}
};
