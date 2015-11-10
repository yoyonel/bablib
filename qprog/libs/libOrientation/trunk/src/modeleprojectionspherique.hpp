#ifndef __MODELEPROJECTIONSPHERIQUE_HPP__
#define __MODELEPROJECTIONSPHERIQUE_HPP__

#include "modeleprojectionphysique.hpp"
#include "modeleprojectionvisitor.hpp"

namespace OrientationMATIS
{
    //    class ModeleProjectionSpherique  : public ModeleProjectionPhysique
    //    {
    //    protected :
    //            double m_cPas, m_lPas;
    //    double m_cPPA, m_lPPA;
    //
    //        public :
    //                ModeleProjectionSpherique(unsigned int nc, unsigned int nl,
    //                                          AuxiliaryData data, Transfo2D transfo2D, std::string systemeGeodesie,shared_ptr<Euclidien> euclidien,
    //                                          const ModelePhysiqueExtrinseque &extr,
    //                                          double cPas, double lPas, double cPPA, double lPPA);
    //        ModeleProjectionSpherique(const ModeleProjectionSpherique &mod);
    //        bool Visit(shared_ptr<ModeleProjectionVisitor> &exporter) const { return exporter->Visit(this); };
    //
    //        shared_ptr<ModeleProjection> Clone() const	{ return shared_ptr<ModeleProjection>( new ModeleProjectionSpherique(*this) );	}
    //
    //        double GetcPas() const { return m_cPas; }
    //        double GetlPas() const { return m_lPas; }
    //
    //        void IntrLocalToImage(double xt, double yt, double zt, double &c, double &l) const;
    //        void IntrImageToVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const;
    //    };

    class ModeleProjectionSpherique : public ModeleProjectionPhysique
    {
    public:
        ModeleProjectionSpherique(unsigned int nc, unsigned int nl,
                                  AuxiliaryData data,
                                  Transfo2D transfo2D,
                                  std::string systemeGeodesie,shared_ptr<Euclidien> euclidien,
                                  const ModelePhysiqueExtrinseque &extr,
                                  double lambdaMin, double lambdaMax,
                                  double phiMin, double phiMax,
                                  double cPPA, double lPPA);

        ModeleProjectionSpherique(double cPPA, double lPPA,
                                  double width, double height,
                                  double lambdaMin, double lambdaMax,
                                  double phiMin, double phiMax);

        ModeleProjectionSpherique(const ModeleProjectionSpherique &mod);

        bool Visit(shared_ptr<ModeleProjectionVisitor> &exporter) const { return exporter->Visit(this); };

        virtual shared_ptr<ModeleProjection> Clone() const { return shared_ptr<ModeleProjection>( new ModeleProjectionSpherique(*this) ); }

        void IntrLocalToImage(double x, double y, double z, double &c, double &l) const;
        void IntrImageToVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const;

        void Dump( std::ostream &o );

        //inline const double LambdaMin() const { return m_lambdaMin; }
        inline double LambdaMin() const { return m_lambdaMin; }
        inline void LambdaMin( double lm ) { m_lambdaMin = lm; }
        //inline const double LambdaMax() const { return m_lambdaMax; }
        inline double LambdaMax() const { return m_lambdaMax; }
        inline void LambdaMax( double lM ) { m_lambdaMax = lM; }
        //inline const double PhiMin() const { return m_phiMin; }
        inline double PhiMin() const { return m_phiMin; }
        inline void PhiMin( double lm ) { m_phiMin = lm; }
        //inline const double PhiMax() const { return m_phiMax; }
        inline double PhiMax() const { return m_phiMax; }
        inline void PhiMax( double lM ) { m_phiMax = lM; }

    protected:
        double m_lambdaMin, m_lambdaMax;
        double m_phiMin, m_phiMax;
        double m_cPPA, m_lPPA;
    };
}

#endif //#ifndef __MODELEPROJECTIONSPHERIQUE_HPP__
