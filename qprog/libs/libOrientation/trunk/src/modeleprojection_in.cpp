#include <map>
#include <iostream>
#include <sstream>

#include "../config/config_xerces.hpp"

#include "noboost.hpp"

#include "modeleprojection.hpp"

namespace OrientationMATIS
{

    void InitIn();
    void InitOut();

    void ModeleProjection::InitAllIO()
    {
        InitIn();
        InitOut();
    }


    typedef shared_ptr<ModeleProjection> (*PtrFonctReaderModeleProjection)(const std::string &);

    typedef std::multimap<std::string, PtrFonctReaderModeleProjection> MapReaderModeleProjection;

    static MapReaderModeleProjection & GetReaders()
    {
        static MapReaderModeleProjection readers;
        return readers;
    }

    /// Lecture de differents formats
    /// .xml : conique
    shared_ptr<ModeleProjection> xml_reader(const std::string &filename);
    shared_ptr<ModeleProjection> spheric_xml_reader(const std::string &filename);

    /// .ori
    shared_ptr<ModeleProjection> conic_ori_reader(const std::string &filename);
    shared_ptr<ModeleProjection> axono_ori_reader(const std::string &filename);
    shared_ptr<ModeleProjection> carto_ori_reader(const std::string &filename);

    /// .hdr : ArcView
    shared_ptr<ModeleProjection> carto_hdr_reader(const std::string &filename);

    /// .xml : MICMAC
    shared_ptr<ModeleProjection> carto_micmac_reader(const std::string &filename);

#if ORIENTATIONMATIS_USE_XERCES
    /// .rto : modele polynomial
    shared_ptr<ModeleProjection> rto_reader(const std::string &filename);
    /// .gri : modele grille
    shared_ptr<ModeleProjection> gri_reader(const std::string &filename);
#endif //#if ORIENTATIONMATIS_USE_XERCES

    void InitIn()
    {
        GetReaders().clear();

        GetReaders().insert(make_pair(".xml", xml_reader));
        GetReaders().insert(make_pair(".ori", conic_ori_reader));
        GetReaders().insert(make_pair(".ori", axono_ori_reader));
        GetReaders().insert(make_pair(".ori", carto_ori_reader));
        GetReaders().insert(make_pair(".hdr", carto_hdr_reader));
        GetReaders().insert(make_pair(".xml", carto_micmac_reader));
        GetReaders().insert(make_pair(".xml", spheric_xml_reader));

#if ORIENTATIONMATIS_USE_XERCES
        GetReaders().insert(make_pair("rto", rto_reader));
        GetReaders().insert(make_pair("gri", gri_reader));
#endif //#if ORIENTATIONMATIS_USE_XERCES
    }



    shared_ptr<ModeleProjection> ModeleProjection::ReadFile(const std::string &filename)
    {
        std::string ext=extension(filename);

        MapReaderModeleProjection::const_iterator it_ext = GetReaders().lower_bound(ext);
        MapReaderModeleProjection::const_iterator it = it_ext;

        // D'abord ceux qui gÃÂÃÂÃÂÃÂ¨rent ÃÂÃÂÃÂÃÂ  peu prÃÂÃÂÃÂÃÂ¨s la mÃÂÃÂÃÂÃÂªme extension
        for (it = it_ext; it != GetReaders().end(); ++it)
        {

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

}
