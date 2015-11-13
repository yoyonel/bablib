/*
#amiens cathedrale
RGFG93: 
Lon: 02d18'04,3616''E Lat:49d53'41.4145''N H: 78.27
RGF L93:
E     649750.13         N: 6977570.07      h:34.4
NTF Lambert1:
E      597463.81        N: 243917.77       h:34.4

#Golfe morbihan (saint armel)
RGFG93:
lon: 2d44'52.2471''O   Lat:47d34'36.8062''N H:60.05
RGF L93:
E     268287.44          N:6735332.29       h:11.74
NTF Lambert2:
E     217761.56          N: 298736.18       h:11.74

#Golfe morbihan (Crach)
RGFG93:
lon: 2d57'43.8854''O  Lat:47d37'09.1204''N   H:71
RGF L93:
E     252 573.00         N:6741213.04       h:22.44
NTF Lambert2:
E     201987.9           N: 304493.7         h:22.44 

#Marseille (croix dome eglise)
RGFG93:
lon: 5d22'16.7784''E  Lat:43d17'02.0040''N  H:240.09
RGF L93:
E      892556.27         N:6245637.81      h:191.13
Lambert 3:
E      846254.09         N:  1138870.54    h:191.13

#Montmirail (suzette la crepe..)
RGFG93:
lon: 5d03'36.2010''E  Lat:44d10'44.7108''N  H:782.25
RGF L93:
E       864691.44        N:6344379.28      h:732.27
NTF Lambert3
E      817717.80       N:212387.42     h:732.27
*/
#include <iostream>
#include <vector>

#define PROJ_DEBUG 1
//#include <proj_api.h>
#include <projects.h>


int main(int argc, char **argv)
{
//on declare les projections ÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ  utiliser
	projPJ Lambert93;

	std::cout.precision(9);

//initialise les projections
	if(!(Lambert93=pj_init_plus("+init=IGNF:LAMB93")) )
	{
		std::cout << "erreur initialisation lambert 93" <<std::endl;
		return -93;
	}
	else
	{
		std::cout << "initialisation lambert 93 OK" << std::endl;
		std::cout << pj_get_def(Lambert93,0) << std::endl;
	}

        projPJ RGF93G;

        std::cout.precision(9);

//initialise les projections
        if(!(RGF93G=pj_init_plus("+init=IGNF:RGF93G")) )
        {
                std::cout << "erreur initialisation lambert 93" <<std::endl;
                return -92;
        }
        else
        {
                std::cout << "initialisation RGFG 93 OK" << std::endl;
                std::cout << pj_get_def(RGF93G,0) << std::endl;
        }
	

        //site SGN: 75056N A Immeuble : Terrasse superieure : Cheminee : Plaquette-cible collee
        //2ÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ° 21' 48,9146'' E        48ÃÂÃÂÃÂÃÂÃÂÃÂÃÂÃÂ° 49' 55,4329'' N  161,92
        //653 282,54     6 859 300,38   118.1

        char* lat_c="2d21'48.9146E";
        char* long_c="48d49'55.4329N";

        double lat=dmstor(lat_c,&lat_c);
        double longitude=dmstor(long_c, &long_c);
        double height=161.92;

        pj_transform(RGF93G,Lambert93,1,0,&lat,&longitude,&height);
        std::cout<<" compute : "<<std::endl;
        std::cout<<" E="<<lat<<" \t N="<<longitude<<" alt="<<height<<std::endl;

        double true_east=653282.54;
        double true_north=6859300.38;
        double true_alt=118.1;
        std::cout<<" true :"<<std::endl;
        std::cout<<" E="<<true_east<<" \t N="<<true_north<<" alt="<<true_alt<<std::endl;
	return 0;
}
