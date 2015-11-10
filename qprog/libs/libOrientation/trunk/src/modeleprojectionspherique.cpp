#include "modeleprojectionspherique.hpp"

namespace OrientationMATIS
{
    //    ModeleProjectionSpherique::ModeleProjectionSpherique(unsigned int nc, unsigned int nl,
    //                                                         AuxiliaryData data,
    //                                                         Transfo2D transfo2D,
    //                                                         std::string systemeGeodesie,
    //                                                         shared_ptr<Euclidien> euclidien,
    //                                                         const ModelePhysiqueExtrinseque &extr,
    //                                                         double cPas, double lPas, double cPPA, double lPPA) :
    //    ModeleProjectionPhysique(nc, nl, data, transfo2D, systemeGeodesie, euclidien, extr),
    //    m_cPas(cPas), m_lPas(lPas), m_cPPA(cPPA), m_lPPA(lPPA)
    //    {;}
    //
    //    ModeleProjectionSpherique::ModeleProjectionSpherique(const ModeleProjectionSpherique &mod) :
    //            ModeleProjectionPhysique(mod),
    //            m_cPas(mod.m_cPas), m_lPas(mod.m_lPas), m_cPPA(mod.m_cPPA), m_lPPA(mod.m_lPPA)
    //    {;}
    //
    //    void ModeleProjectionSpherique::IntrLocalToImage(double xt, double yt, double zt, double &c, double &l) const
    //    {
    //        double norme = sqrt(xt*xt + yt*yt + zt*zt);
    //        double theta = acos ( zt / norme);
    //        double phi = atan2( yt , xt);
    //        c = phi / m_cPas;
    //        l = theta / m_lPas;
    //        c += m_cPPA;
    //        l += m_lPPA;
    //    }
    //
    //    void ModeleProjectionSpherique::IntrImageToVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const
    //    {
    //        x0=y0=z0= 0.;
    //        double theta = (c-m_cPPA) * m_cPas;
    //        double phi = (l-m_lPPA) * m_lPas;
    //        x1 = sin(theta) * cos(phi);
    //        y1 = sin(theta) * sin(phi);
    //        z1 = cos(theta);
    //    }

    ModeleProjectionSpherique::ModeleProjectionSpherique(unsigned int nc, unsigned int nl,
                                                         AuxiliaryData data,
                                                         Transfo2D transfo2D,
                                                         std::string systemeGeodesie,
                                                         shared_ptr<Euclidien> euclidien,
                                                         const ModelePhysiqueExtrinseque &extr,
                                                         double lambdaMin, double lambdaMax,
                                                         double phiMin, double phiMax,
                                                         double cPPA, double lPPA) :
    ModeleProjectionPhysique(nc, nl, data, transfo2D, systemeGeodesie, euclidien, extr),
        m_lambdaMin(lambdaMin), m_lambdaMax(lambdaMax),
        m_phiMin(phiMin), m_phiMax(phiMax),
        m_cPPA(cPPA), m_lPPA(lPPA)
    {;}

    ModeleProjectionSpherique::ModeleProjectionSpherique(const ModeleProjectionSpherique &mod) :
        ModeleProjectionPhysique(mod),
        m_lambdaMin(mod.m_lambdaMin), m_lambdaMax(mod.m_lambdaMax),
        m_phiMin(mod.m_phiMin), m_phiMax(mod.m_phiMax),
        m_cPPA(mod.m_cPPA), m_lPPA(mod.m_lPPA)
    {;}

    void ModeleProjectionSpherique::IntrLocalToImage(double xt, double yt, double zt, double &c, double &l) const
    {
        double norme = sqrt(xt*xt + yt*yt + zt*zt);

        double lambda = m_lambdaMax - atan2(yt, xt);
        double phi = m_phiMax - acos(zt / norme);
        c = m_width * lambda / (m_lambdaMax - m_lambdaMin );
        l = m_lPPA - m_height * phi / (m_phiMax - m_phiMin );
    }

    void ModeleProjectionSpherique::IntrImageToVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const
    {
        x0 = y0 = z0 = 0.;

        double lambda = m_lambdaMin + (m_lambdaMax - m_lambdaMin) * c / m_width;
        double phi = m_phiMax + (m_phiMin - m_phiMax) * l / m_height;
        x1 = cos(phi) * cos(-lambda);
        y1 = cos(phi) * sin(-lambda);
        z1 = sin(phi);
    }
}
