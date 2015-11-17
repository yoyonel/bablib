#ifndef __MODELEPROJECTIONCONIQUE_HPP__
#define __MODELEPROJECTIONCONIQUE_HPP__

#include "modeleprojectionphysique.hpp"
#include "modeleprojectionvisitor.hpp"

namespace OrientationMATIS
{
    class ModeleProjectionConique : public ModeleProjectionPhysique
    {
    protected :
            double m_focal;
			double m_cPPA, m_lPPA;
        public :
                ModeleProjectionConique(unsigned int nc, unsigned int nl,
                                        AuxiliaryData data, Transfo2D transfo2D, std::string systemeGeodesie,shared_ptr<Euclidien> euclidien,
                                        const ModelePhysiqueExtrinseque &extr,
                                        double focal, double cPPA, double lPPA);
        ModeleProjectionConique(const ModeleProjectionConique &mod);

        double GetFocal() const { return m_focal; }
        void SetFocal(const double f) { m_focal = f; }

        double GetcPPA() const { return m_cPPA; }
        double GetlPPA() const { return m_lPPA; }
        void SetcPPA(double cPPA) { m_cPPA = cPPA; }
        void SetlPPA(double lPPA) { m_lPPA = lPPA; }

        bool Visit(shared_ptr<ModeleProjectionVisitor> &exporter) const { return exporter->Visit(this); };
        shared_ptr<ModeleProjection> Clone() const	{ return shared_ptr<ModeleProjection>( new ModeleProjectionConique(*this) ); }

        void IntrLocalToImage(double xt, double yt, double zt, double &c, double &l) const;
        void IntrImageToVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const;
    };
}

#endif //#ifndef __MODELEPROJECTIONCONIQUE_HPP__
