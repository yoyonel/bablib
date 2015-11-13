#include <iostream>
#include <sstream>
#include <iomanip>
#include <typeinfo>
#include <fstream>

#include "noboost.hpp"

#include "modeleprojection.hpp"
#include "tinyxml.h"
#include "outil_xml.hpp"
#include "modeleprojectionconique.hpp"
#include "modeleprojectionvisitor.hpp"
#include "distortionpolynomeign.hpp"
#include "distortiongrillematis.hpp"
#include "homographie2d.hpp"
#include "projectivetransfo2D.hpp"
#include "systematismeaffine.hpp"
#include "systematismecylindrique.hpp"
#include "math.h"
#include "modeleprojectioncarto.hpp"

#define round(x) (x<0?ceil((x)-0.5):floor((x)+0.5))


namespace OrientationMATIS
{

    class ORIExportVisitor : public ModeleProjectionVisitor
    {
        std::string m_filename;
    public :
            ORIExportVisitor(const std::string &filename)  : m_filename(filename)
    {
        //		std::cout << "Creation de XMLExportVisitor" << std::endl;
    }

    bool Visit(const ModeleProjectionConique *mpc)
    {
        std::setprecision(12);
        // ici o ecrit le fichier text en prenant exemple sur conicorientation.cpp de nayau_new/src/
        std::ofstream fic(m_filename.c_str());
        if (fic.good() == 0) throw std::logic_error("Impossible d'ouvrir le fichier en mode ecriture! \n");

        fic << "TEXT" << std::endl;

        int ntransfo = mpc->GetTransfo2D().GetNbTransfo2DElementaire();
        if (!ntransfo) fic << "0" << std::endl;
        else if (ntransfo > 1) throw std::logic_error("Ecriture d'orientation avec plus que 2 transfo2D n'est par gÃÂÃÂÃÂÃÂ©rÃÂÃÂÃÂÃÂ© en format ori \n");

        shared_ptr <DistortionPolynomeIGN> poly;
        bool dist_poly=false;
        if (ntransfo ==1)
        {
            shared_ptr<Transfo2DElementaire> te = mpc->GetTransfo2D().GetTransfo2DElementaire(0);
             poly = dynamic_pointer_cast<DistortionPolynomeIGN> (te);
            if (!poly )
            {
                throw std::logic_error("Seulement l'ecriture de distortion sous forme de polynome est gÃÂÃÂÃÂÃÂ©rÃÂÃÂÃÂÃÂ© ! \n");

            }
              fic << "2" << std::endl;
              dist_poly=true;
        }

        // correction attmospherique
        fic << "0" << std::endl;

        fic<<"117 110 107 110 111 119 110 0 "<<std::endl;

        short annee;
        unsigned char mois,jour, heure,minute;
        float seconde;
        std::string time_system;

        mpc->GetAuxiliaryData().GetImageDate().Get(annee, mois, jour, heure, minute, seconde, time_system);

        fic << (int) jour << " " << (int) mois << " " << (int) annee<< " ";
        fic <<(int)  heure<< " " <<(int) minute<< " " << (int) seconde << std::endl;

        fic<< (int)round(mpc->GetAuxiliaryData().GetAltiSol() *1000)<<std::endl;


        double localOriginX = 0;
        double localOriginY = 0;
        shared_ptr<Euclidien> euc = mpc->GetEuclidien();

        shared_ptr<EuclidienMATISRTL> dp = dynamic_pointer_cast<EuclidienMATISRTL> (euc);
        if (dp)
        {
            dp->GetLocalOrigin(localOriginX,localOriginY);
//            std::cout<< "localOrigin : "<<localOriginX<<" , "<<localOriginY<<std::endl;
//            std::cout<<" system geodesie : "<<mpc->GetSystemGeodesie()<<std::endl;
            fic<< std::setprecision(12)<<(int)round(localOriginX*1000)<<" ";
            fic<< std::setprecision(12)<<(int)round(localOriginY*1000)<<std::endl;
            std::string sys = mpc->GetSystemGeodesie();
            int zone = 0;
            if (sys == "+init=IGNF:LAMB1C") zone = 1;
            else if (sys == "+init=IGNF:LAMB2C") zone = 2;
            else if (sys == "+init=IGNF:LAMB3C") zone = 3;
            else if (sys == "+init=IGNF:LAMB4C") zone = 4;
            fic<<zone<<std::endl;
        } else
        {
            std::cout << "Warning: " << __FILE__ << ":" << __LINE__ <<
                    " oriwriter.cpp impossible de convertir l'ori en EuclidienMATISRTL" << std::endl;
            fic<<"0 0"<<std::endl<<"0"<<std::endl;
        }

        const double * sommet;
        sommet = mpc->GetExtrinseque().GetSommet();
        // in old ori, easting and northing are stored in mm in an int
        // this causes overflows when they are above 2.1 Mm
        double easting = sommet[0]*1000., northing = sommet[1]*1000.;
        const double billion = 1000000000.;
        if(easting > 2*billion)
        {
            double shift = floor(easting/billion)*billion;
            std::cout << "Warning: " << __FILE__ << ":" << __LINE__ <<
                    " easting too close to int limit: removing " << shift << std::endl;
            easting -= shift;
        }
        if(northing > 2*billion)
        {
            double shift = floor(northing/billion)*billion;
            std::cout << "Warning: " << __FILE__ << ":" << __LINE__ <<
                    " northing too close to int limit: removing " << shift << std::endl;
            northing -= shift;
        }
        fic<<(int)easting<<" "<<(int)northing<<" "<<(int)(sommet[2]*1000)<<std::endl;

        fic<<(int)round(mpc->GetFocal()*1000)<<std::endl;

        const double *r0,*r1,*r2;
        r0= mpc->GetExtrinseque().GetRotation(0);
        r1= mpc->GetExtrinseque().GetRotation(1);
        r2= mpc->GetExtrinseque().GetRotation(2);
        fic<<(int)round(r0[0]*billion)<<" "<<(int)round(r0[1]*billion)<<" "<<(int)round(r0[2]*billion)<<std::endl;
        fic<<(int)round(r1[0]*billion)<<" "<<(int)round(r1[1]*billion)<<" "<<(int)round(r1[2]*billion)<<std::endl;
        fic<<(int)round(r2[0]*billion)<<" "<<(int)round(r2[1]*billion)<<" "<<(int)(round(r2[2]*billion))<<std::endl;

        double sx,sy;
        mpc->GetAuxiliaryData().GetPixelSize(sx,sy);
        if(sx*sy == 0.) {sx = mpc->GetPixelSize(); sy = sx;}
        fic<<(int)round(sx * billion)<<" "<<(int)round(sy * billion)<<std::endl;

        unsigned int nc,nl;
        mpc->GetTailleImage(nc,nl);
        fic<<nc<<" "<<nl<<std::endl;

        fic<<(int)round(mpc->GetcPPA()*1000)<<" "<<(int)round(mpc->GetlPPA() *1000)<<std::endl;

        if (dist_poly)
        {
            fic<<poly->GetR3()<<" "<<poly->GetR5()<<" "<<poly->GetR7()<<std::endl;
            fic<<poly->GetcPPS()<<" "<<poly->GetlPPS()<<std::endl;
        }

       return true;
    }

    bool Visit(const ModeleProjectionCarto *mpc)
    {
        std::setprecision(20);
        std::cout<<"ecriture d'ancien format ori carto \n";

        std::ofstream fic(m_filename.c_str());
        if (fic.good() == 0) throw std::logic_error("Impossible d'ouvrir le fichier en mode ecriture! \n");

        std::string sys = mpc->GetSystemGeodesie();
        int zone = 0;
        if (sys == "+init=IGNF:LAMB1C") zone = 1;
        else if (sys == "+init=IGNF:LAMB2C") zone = 2;
        else if (sys == "+init=IGNF:LAMB3C") zone = 3;
        else if (sys == "+init=IGNF:LAMB4C") zone = 4;
        else if (sys == "+init=IGNF:LAMB93") zone = 93;

        double ZoneMultiplyer = zone;
        if (zone == 93) ZoneMultiplyer = 0;

        unsigned int nc,nl;
        mpc->GetTailleImage(nc,nl);

//        std::cout<<" test bahman \n";
//        std::cout.precision(15);
//        std::cout<<(double)mpc->GetXOrigin()  <<std::endl;
//        std::cout<<" ______________________ \n";

        fic << "CARTO" << std::endl;
        fic << 1000.0*(double)mpc->GetXOrigin()  << " " << ((mpc->GetYOrigin()-ZoneMultiplyer*1000000.0)*1000.0) << std::endl;
        fic << zone << std::endl;
        fic << nc << " " << nl << std::endl;
        fic << mpc->GetXStep()*1000.0 << " " << mpc->GetYStep()*1000.0 << std::endl;
        fic.close();

        return true;
    }



    };

    shared_ptr<ModeleProjectionVisitor> ori_exporter(const std::string &filename)
    {
        //		std::cout  << "Dans la fonction" << std::endl;
        return shared_ptr<ModeleProjectionVisitor>(new ORIExportVisitor(filename));
    }
}

