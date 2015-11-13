#ifndef __MODELEPROJECTIONPHYSIQUE_HPP__
#define __MODELEPROJECTIONPHYSIQUE_HPP__


#include <cmath>
//#include <boost/shared_ptr.hpp>

#include "modeleprojection.hpp"

namespace OrientationMATIS
{
    class ModelePhysiqueExtrinseque
    {
    protected :
            double m_sommet[3];
			double m_rotation[3][3];
        public :
        ModelePhysiqueExtrinseque(const ModelePhysiqueExtrinseque &mpe);
        ModelePhysiqueExtrinseque(const double sommet[3], const double rotation[3][3]);
        ~ModelePhysiqueExtrinseque();

        ModelePhysiqueExtrinseque & operator=(const ModelePhysiqueExtrinseque &mpe);

        const double * GetSommet() const { return m_sommet; }
        void SetSommet(const double sommet[3])
        {
            m_sommet[0] = sommet[0];
            m_sommet[1] = sommet[1];
            m_sommet[2] = sommet[2];
        }
        
        void SetSommet(double x, double y, double z)
        {
        	m_sommet[0] = x;
        	m_sommet[1] = y;
        	m_sommet[2] = z;
        }
        
        const double * GetRotation(unsigned int line) const { return m_rotation[line]; }
        void SetRotation(int line , const double matrixline[3])
        {
            m_rotation[line][0] = matrixline[0];
            m_rotation[line][1] = matrixline[1];
            m_rotation[line][2] = matrixline[2];
        }
        void LocalToImage(double xt, double yt, double zt, double &xi, double &yi, double &zi) const;
        void ImageToLocal(double xi, double yi, double zi, double &xt, double &yt, double &zt) const;
        double GetDistance(double xt, double yt, double zt) const;
    };

    class ModeleProjectionPhysique : public ModeleProjection
    {
    protected :
            ModelePhysiqueExtrinseque m_extr;
			double m_PixelSize;

        public :
                ModeleProjectionPhysique(unsigned int nc, unsigned int nl,
                                         AuxiliaryData data, Transfo2D transfo2D, std::string systemeGeodesie, shared_ptr<Euclidien> euclidien,
                                         const ModelePhysiqueExtrinseque& extr) :
                ModeleProjection(nc, nl,data,transfo2D,systemeGeodesie,euclidien),
                m_extr( extr ),m_PixelSize(0.0)
        {;}

		void SetPixelSize(double val){m_PixelSize= val;}
                //const double  GetPixelSize() const {return m_PixelSize;}
                double  GetPixelSize() const {return m_PixelSize;}

        ModeleProjectionPhysique(const ModeleProjectionPhysique &mpp) : ModeleProjection(mpp), m_extr(mpp.m_extr) {;}

        void ImageAndZToLocal(double c, double l, double z, double &x, double &y) const;
        void LocalToImage(double xt, double yt, double zt, double &c, double &l) const;
        void ImageToLocalVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const;

        bool IsInfrontOfImage(double x, double y, double z);
        virtual void IntrLocalToImage(double xt, double yt, double zt, double &c, double &l) const = 0;
        virtual void IntrImageToVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const =0;

        const ModelePhysiqueExtrinseque& GetExtrinseque() const { return m_extr; }
        ModelePhysiqueExtrinseque& GetExtrinseque() { return m_extr; }

		void SetExtrinseque(const ModelePhysiqueExtrinseque& extr) { m_extr =extr; }
    };

    //class ModeleProjectionGrilleMPD : public ModeleProjectionPhysique


} // namespace

#endif
