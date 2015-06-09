#include<cstdlib>
#include<cmath>

#include "solar.hpp"

namespace OrientationMATIS
{

	// D. BOLDO : Modif du 15/02/2000 : mise a 0 du decalage heure entree - heure TU
	//#define FUSEAU ASkHeureLocaleVraie
	#define FUSEAU 0

	/*--------------------------------------------------------------------------------------*/
	void Solar::orvecteur_soleil_terre (  int annee, int mois, int jour, int heure, int minute,
									double latitude, double longitude,
									double soleil[3] )
	{
		int Year ;
		double HmsTU ;
		double JJ0h, JJ ;
		double AlphaSol, DeltaSol ;
		double TS ;
		double AzNord, Hauteur ;
		const double Pi_ = atan(1.0)*4.0 ;

		Year = annee ;
		if ( Year < 100 )
		{
			if ( Year < 50 )
			{
				Year = Year + 2000 ;
			}
			else
			{
				Year = Year + 1900 ;
			}
		}

		JJ0h = Solar::ASJourJulien ( Year, mois, jour ) ;
		// 1) Heure TU (Heure Moyenne a Grennwitch)
		HmsTU = ((double) heure)+ ((double) minute) / 60.0 ;
		HmsTU = Solar::ASTempsUniversel ( JJ0h, HmsTU, - longitude/15.0, FUSEAU ) ;

		// 2) Ascension droite et declinaison du soleil
		JJ = JJ0h + HmsTU/24.0 ;
		Solar::ASAlphaDeltaSoleil ( JJ, &AlphaSol, &DeltaSol ) ;

		// 3) Temps sideral du lieu
		TS = Solar::ASTempsSideralMoyen ( JJ0h, HmsTU, - longitude/15.0 ) ;
		TS = TS*Pi_ / 12.0 ;

		// 4) Azimut et Hauteur dans le repere local
		Solar::ASAlphaDeltaToAzHt ( TS, (latitude * Pi_/180.0), AlphaSol, DeltaSol, &AzNord, &Hauteur ) ;

		// 5) Vecteur en coordonnees cartesiennes
		soleil[2] = - sin ( Hauteur ) ;
		soleil[1] = - cos ( Hauteur ) * cos ( AzNord ) ;
		soleil[0] = - cos ( Hauteur ) * sin ( AzNord ) ;
	}

	typedef struct tagGg
	{
	  double X0, YR , C , R2 , SS ,LC0 ;
	} Gg ; 	/* Lamb_geo */

	typedef struct tagPof
	{
	  double X0, Sino, Sink, LC0, R0, YR ;
	} Pof ;	/* Geo_Lamb */

	//=====================================================================
	/*
			Lamb_geo.c
	   Calcule la longitude Alon et la latitude Alat en grades
	   d'un point de coordonnees X,Y donnees en metres
	   ( avec le chiffre indicateur de la zone Lambert pour les y)
	   dans la Projection Lambert de numero Nlamb.

	 On rappelle que la constante origine des Y vaut:
		  1200000.00 Metres pour la zone Lambert I
		  2200000.00         "           Lambert II
		  3200000.00         "           Lambert III
		  4185861.37         "           Lambert IV

	 Les 4 zones Lambert-France sont caracterisees par:
	 Lambert I  :      Alat>=53.5 Gr            (Parallele origine:55 Gr)
	 Lambert II :    50.5 Gr<=Alat<=53.5 Gr     (Parallele origine:52 Gr)
	 Lambert III: Alat<=50.5 Gr et Alon<=6.2 Gr (Parallele origine:49 Gr)
	 Lambert IV : Alat<=48 Gr et Alon>=6.4 Gr (Parallele origine:46,85 Gr)
	*/
	//=====================================================================

	void Solar::Lamb_geo (double xx,
			   double yy,
			   int N_lamb,
			   double &Alon,
			   double &Alat )

	{
	  double U, V, P, T, S, S2, A, E;
	  int Nl ;
	  Gg Eve[4] ;

	  //   Constantes des projections Lambert-France
	  Eve[0].X0= 600000;
	  Eve[1].X0= 600000;
	  Eve[2].X0= 600000;
	  Eve[3].X0= 234.358;
	  Eve[0].YR= 6657616.674 ;
	  Eve[1].YR= 8199695.768 ;
	  Eve[2].YR= 9791905.085 ;
	  Eve[3].YR= 11239161.542 ;
	  Eve[0].C= 83.721038652;
	  Eve[1].C= 87.331573464;
	  Eve[2].C= 91.479819811;
	  Eve[3].C= 94.838400858;
	  Eve[0].R2 = 2.978557976E13;
	  Eve[1].R2 = 3.5996349309E13;
	  Eve[2].R2 = 4.345321265E13;
	  Eve[3].R2 = 4.974904333E13;
	  Eve[0].SS = 1.5208119312;
	  Eve[1].SS = 1.4579372548;
	  Eve[2].SS = 1.3918255932;
	  Eve[3].SS = 1.3425358644;
	  Eve[0].LC0= 0.9919966654;
	  Eve[1].LC0= 0.9215573613;
	  Eve[2].LC0= 0.8545910977;
	  Eve[3].LC0= 0.8084757728;

	  if ( yy < 1000000. )
	  {
		yy += N_lamb * 1000000. ;
	  }

	  Nl = N_lamb - 1 ;
	  U = xx - (double) ( Eve[Nl].X0  ) ;
	  V = (double) ( Eve[Nl].YR ) - yy ;
	  Alon = ( atan( U / V ) ) * Eve[Nl].C ;
	  P =  ( U * U + V * V ) / Eve[Nl].R2 ;
	  E = exp( log(P)/Eve[Nl].SS - Eve[Nl].LC0  ) ;
	  T = ( 1 - E ) / ( 1 + E ) ;
	  S = sin( 2 * atan(T) ) ;
	  S2 = S * S ;
	  A = ( ( 1.38E-7 * S2 - 1.5707E-5 ) * S2 + 3.425046E-3 ) * S ;
	  Alat = atan( (A + T) / (1 + A * T ) ) * 127.32395447 ;

	  // Le calcul est fait en grades : en degres en sortie
	  Alon = Alon * 0.9 ;
	  Alat = Alat * 0.9 ;
	}

	//=====================================================================
	/*
				Geo_lamb.c

	   Calcule les coordonnees X ,Y en metres,dans la projection
	   Lambert de numero N_amb d'un point de longitude Alon et de
	   latitude Alat donnees en degres.
	   On rappelle que la constante origine des Y vaut:
		  1200000.00 Metres pour la zone Lambert I
		  2200000.00         "           Lambert II
		  3200000.00         "           Lambert III
		  4185861.37         "           Lambert IV
	  Les 4 zones Lambert-France sont caracterisees par:
	  Lambert I  :      Alat>=53.5 Gr            (Parallele origine:55 Gr)
	  Lambert II :    50.5 Gr<=Alat<=53.5 Gr     (Parallele origine:52 Gr)
	  Lambert III: Alat<=50.5 Gr et Alon<=6.2 Gr (Parallele origine:49 Gr)
	  Lambert IV : Alat<=48 Gr et Alon>=6.4 Gr (Parallele origine:46,85 Gr)
	*/
	//=====================================================================
	void Solar::Geo_lamb( double Alon, double Alat, int N_lamb, double &x, double &y )

	{
	double T, Lc , R , C ;
	double gAlon , gAlat ;	// en grades
	Pof Pif[4] ;
	int Nl ;
	//   Constantes des projections Lambert-France
		 Pif[0].X0=600000;
		 Pif[1].X0=600000;
		 Pif[2].X0=600000;
		 Pif[3].X0=234.358;
		 Pif[0].Sino=0.7604059656;
		 Pif[1].Sino=0.7289686274;
		 Pif[2].Sino=0.6959127966;
		 Pif[3].Sino=0.6712679322;
		 Pif[0].Sink=1.194442898E-2;
		 Pif[1].Sink=1.145061242E-2;
		 Pif[2].Sink=1.093137265E-2;
		 Pif[3].Sink=1.054425202E-2;
		 Pif[0].LC0=0.9919966654;
		 Pif[1].LC0=0.9215573613;
		 Pif[2].LC0=0.8545910977;
		 Pif[3].LC0=0.8084757728;
		 Pif[0].R0=5457616.674;
		 Pif[1].R0=5999695.768;
		 Pif[2].R0=6591905.085;
		 Pif[3].R0=7053300.173;
		 Pif[0].YR=6657616.674;
		 Pif[1].YR=8199695.768;
		 Pif[2].YR=9791905.085;
		 Pif[3].YR=11239161.542;


	   Nl = N_lamb -1 ;
	   gAlon = Alon * 200.0 / 180.0 ;
	   gAlat = Alat * 200.0 / 180.0 ;

	   T = 8.24832568E-2 * sin( gAlat * 1.5707963268E-2 ) ;

	   Lc = log( tan( 7.8539816340E-3 * ( gAlat + 100) ) )
			  - 4.12416284E-2 * log( (1+T) / (1-T) ) ;
	   // latitude croissante
	   R=Pif[Nl].R0 * exp(Pif[Nl].Sino*(Pif[Nl].LC0-Lc));
	   C=Pif[Nl].Sink * gAlon ;

	   x = Pif[Nl].X0 + R * sin(C);
	   y = Pif[Nl].YR - R * cos(C);
	/*
	 *		Formulation abandonnee :
	 *	  *y = *y - (double)(N_lamb * 1000000) ;
	 *	on adopte desormais une convention pour laquelle
	 *	Y porte les "millions" du Lambert
	 */
	}

	/* Formules astronomiques classiques - equinoxe moyen 1900 - precession et nutation
	   non prises en compte ;

	 validees par comparaison des azimut et hauteur obtenus avec les valeurs publiees
	 par le BDL (3615 BDL)

	 La precision (ERREUR MAX) est meilleure que la demi minute d'arc sur les azimuts

	 Elle reste meilleure que la demi minute pour les hauteur superieures a 14 60 degres
	 Lorsque le soleil est tres bas, le modele de refraction est sans doute trop pauvre :
	 la precision reste toutefois (a priori) meilleure que quelques (2 a 3) minutes d'arcs
	 (les deux ou trois tests effectues n'ont pas montre d'erreur superieure a la demi-minute)

		Echantillon de reference (source 3615 BDL) :
		--------------------------------------------
		Lieu du calcul : Paris (2�20'E,48�50'N)
		Annee du calcul: 1996
		Jour  du calcul: 15 de chaque mois
		Heure du calcul: 10h00 (TU)
		Azimuts comptes vers l'Est a partir du Sud en degres-minutes
		Hauteur au dessus de l'horizon en degres-minutes

		Donnees BDL :
				Mois		Azimut Est		Hauteur

				1			28�49'			15�05'
				2			33�08'			22�28'
				3			36�26'			33�02'
				4			40�06'			44�47'
				5			45�16'			53�12'
				6			50�40'			56�24'
				7			50�42'			54�08'
				8			44�05'			47�50'
				9			34�46'			38�47'
				10			27�24'			28�44'
				11			23�56'			19�21'
				12			24�59'			14�10'
		Pour les deux angles, emq = 0.25 minutes d'arc sur l'echantillon ci-dessus (compatible
		avec la simple erreur de quantification)

	 ===============================
	 NON TESTE dans l'hemisphere SUD
	 ===============================
	 */
	 /*
	  D. BOLDO
	  (15/02/2000) TESTS 3615 BDL :

	  longitude 2�20' E
	  latitude 48�50' N
	  15/10/1999 10h00 TU	27�32'			28�58'

	  longitude 1�00' E
	  latitude 48�50' N
	  15/10/1999 10h00 TU	28�58'			28�34'

	  longitude 2�20' E
	  latitude 47�00' N
	  15/10/1999 10h00 TU	28�02'			30�36'

	  longitude 2�16' E
	  latitude 49'53' N
	  13/10/1999 10h31 TU	19�11'			30�41'
	*/

	/*=========================================================================================*/


	/* Les deux Soleils programmes S1 et S2 donnent pratiquement les memes resultats
	   S2 semble legerment plus precis */
	#define _SOLEILS2_

	/* On utilise un modele de refraction simplifie : couche fine refractrice (d'environ 3km8)
	   autour de la sphere terrestre.
	   Deux parametres : _REFRAC_ = indice de refraction
						 _RATM_ = ratio entre le rayon de l'enveloppe externe de la couche et
								 le rayon terrestre
	   Ces parametres ont ete calibres pour :
			- avoir une hauteur quasi nulle au coucher du soleil (Hauteur vraie = -36'36'')
			- donner des hauteurs compatibles avec celles du BDL (echantillon de 12 dates a 10h)
	*/
	#define _REFRAC_ 	0.999689334064
	#define _RATM_  	1.000595218750

	//-----------------------------------------------------------------------------------
	void 	Solar::ASAlphaDeltaSoleil ( double JourJulien, double *AlphaSoleil, double *DeltaSoleil )
	{
		TPositionSoleil Sun ;
		double LgEclSoleil ;
		const double Pi_ = 4.0*atan(1.0) ;

		Solar::ASCalculeSoleil ( JourJulien, &Sun ) ;
		// 1) Longitude Ecliptique du soleil
		LgEclSoleil = - Sun.LambdaApp ;
		if ( LgEclSoleil < 0.0 ) LgEclSoleil = LgEclSoleil + 2.0*Pi_ ;

		// 2) Ascension Droite et declinaison soleil
		*AlphaSoleil = cos(Sun.EpsilonApp)*tan(LgEclSoleil) ;
		if ( (*AlphaSoleil > -1.0)&&(*AlphaSoleil<1.0) )
		{
			*AlphaSoleil = atan (*AlphaSoleil) ;
		}
		else
		{
			*AlphaSoleil = Pi_/2.0 -atan(1.0 / *AlphaSoleil) ;
		}
		if ( *AlphaSoleil < 0.0 )
			*AlphaSoleil = *AlphaSoleil + 2.0*Pi_ ;
		if ( std::abs(*AlphaSoleil-LgEclSoleil) > Pi_/2.0 )
		{
			*AlphaSoleil = *AlphaSoleil + Pi_ ;
			if ( *AlphaSoleil > 2.0*Pi_ )
			{
				*AlphaSoleil = *AlphaSoleil - 2.0*Pi_ ;
			}
		}
		*DeltaSoleil = -sin(Sun.EpsilonApp)*sin(LgEclSoleil) ;
		if ( (*DeltaSoleil > -0.5) && (*DeltaSoleil < 0.5) )
		{
			*DeltaSoleil = asin ( *DeltaSoleil ) ;
		}
		else
		{
			*DeltaSoleil = Pi_/2.0 -  acos ( *DeltaSoleil ) ;
		}
	}
	//-----------------------------------------------------------------------------------
	void	Solar::ASAlphaDeltaToAzHt ( double TempsSideralMoyen, double LatitudeNord,
									   double AscensionDroite, double Declinaison,
									   double *AzimutNord, double *Hauteur )
									   // tout en radians ; les azimuts par rapport au Nord et vers l'Est
	{
		double Teta ;
		const double Pi_ = 4.0*atan(1.0) ;
		double vecteur[3], av0, av1 ;

		Teta = TempsSideralMoyen + AscensionDroite ;
		if ( Teta > 2.0*Pi_ ) { Teta = Teta - 2.0*Pi_ ; }
		vecteur[0] = -cos(Declinaison)*sin(Teta) ;
		vecteur[1] = sin(Declinaison)*cos(LatitudeNord) - cos(Declinaison)*sin(LatitudeNord)*cos(Teta) ;
		vecteur[2] = sin(Declinaison)*sin(LatitudeNord) + cos(Declinaison)*cos(LatitudeNord)*cos(Teta) ;
		// Azimut
		av0 = std::abs ( vecteur[0] ) ;
		av1 = std::abs ( vecteur[1] ) ;
		if ( av0 >= av1 )
		{
			if ( av0 == 0 )
			{
				*AzimutNord = 0.0 ;
			}
			else
			{
				*AzimutNord = atan ( av0 / av1 ) ;
			}
		}
		else
		{
			*AzimutNord = Pi_/2.0 - atan ( av1 / av0 ) ;
		}
		if ( vecteur[1] < 0 ) { *AzimutNord = Pi_ - *AzimutNord ; }
		if ( vecteur[0] < 0 ) { *AzimutNord = - *AzimutNord ; }

		/* Hauteur avec correction de refraction */
		if ( vecteur[2] < 0.707 )
		{ *Hauteur = asin (vecteur[2]) ; }
		else
		{ *Hauteur = Pi_/2.0 - acos(vecteur[2]) ; }
		*Hauteur = Solar::ASCorrigeHauteur ( *Hauteur, _RATM_, _REFRAC_ ) ;
	}
	//-----------------------------------------------------------------------------------

	double Solar::ASCorrectionHoraire ( TPositionSoleil *Sun )
	// Retourne la valeur a rajouter a l'heure vraie pour obtenir l'heure moyenne
	// Equation du Temps p. 55
	{
		double ET ;

		// en Secondes
		ET = 459.74*sin((*Sun).M) + 4.80*sin(2.0*(*Sun).M) -
			591.89*sin(2.0*((*Sun).L)) + 12.74*sin(4.0*((*Sun).L)) ;

		// En Heures
		ET = ET/3600.0 ;
		return ET;
	}
	//-----------------------------------------------------------------------------------
	double Solar::ASTempsSideralMoyen (double JourJulien0Heure, double HmsTU,
									   double LongitudeOuestEnHeures )
	{
		// methode n.2 p.58 - temps sideral rapporte a l'equinoxe moyen 1900
		double T0, Sideral ;

		T0 = ASSiecleJulien ( JourJulien0Heure ) ;
		// Temps sideral greenwitch a 0 h
		Sideral = 6.64606556 + 2400.051262*T0 + 0.0000258*T0*T0 ;
		if ( std::abs((int)Sideral)>=24 ) { Sideral = Sideral - (double)(24*(int)(Sideral/24.0)) ; }
		// Prise en compte de la longitude Ouest du lieu :
		// LA LONGITUDE EST PRISE EN COMPTE SI ON DONNE L'HEURE A GREENWITCH
		Sideral = Sideral - LongitudeOuestEnHeures + HmsTU*1.0027379093 ;
		if ( std::abs((int)Sideral)>=24 )
		{
			Sideral = Sideral - (double)(24*(int)(Sideral/24.0)) ;
		}
		if ( Sideral < 0.0 )
			Sideral = Sideral + 24.0 ;
		return Sideral ;
	}
	//-----------------------------------------------------------------------------------
	double Solar::ASJourJulien ( int Annee, int Mois, int Jour )
	// Methode de A.RICHARD :
	// Mois compris entre 1 et 12
	// Jour (Quantieme) compris entre 1 et 31
	{
	/*
	Les ElapsedDays sont le tableau de la methode de A.RICHARD
	Lorsque l'annee est bissextile*, il faut enlever 1 ssi le Mois est Janv ou Fevr.
	*On compte, POUR CETTE METHODE, toute Annee divisible par 4 comme bissextile
	(cf. Formulaire Astro p.46)
		*/
		const int ElapsedDays[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 } ;
		const unsigned long SpecialDay = 4 + 32*10 + 32*16*1582 ;
		unsigned long FullDate ;
		int NJours ;
		double JJ ;

		// Nombre de jours ecoules
		NJours = ElapsedDays[Mois-1] ;
		if ( (4*(Annee/4)==Annee) && (Mois<=2) ) NJours = NJours - 1 ;

		// selection de la methode de calcul
		FullDate = Jour + 32*Mois + 32*16*Annee ;
		if ( FullDate < SpecialDay )
		{
			JJ = (double)(1721057 + (int)(365.25*(double)Annee) + NJours + Jour) + 0.5 ;
		}
		else
		{
			JJ = (double)(1721059 + (int)(365.2425*(double)Annee) + NJours + Jour ) + 0.5 ;
		}
		return JJ ;
	}
	//-----------------------------------------------------------------------------------
	#ifdef _SOLEILS1_
	void Solar::ASCalculeSoleil ( double JourJulien, TPositionSoleil* Sun )
	{
	double UU, XX, CC ;
	const double Pi_ = 4.0*atan(1.0) ;

	/* siecle julien equinoxe 1900 */
	(*Sun).T = ASSiecleJulien (JourJulien) ;
	/* Excentricite */
	(*Sun).e = 0.01675104 - 0.0000418*(*Sun).T*(*Sun).T
				- 0.000000126*(*Sun).T*(*Sun).T*(*Sun).T ;
	/* Parametres */
	(*Sun).L = 279.696668 + 36000.76892*(*Sun).T + 0.0003025*(*Sun).T*(*Sun).T ;
	(*Sun).L = (*Sun).L - (double)(360*(int)((*Sun).L/360.0)) ;
	(*Sun).M = 358.475830 + 35999.04975*(*Sun).T - 0.00015*(*Sun).T*(*Sun).T
				- 0.0000033*(*Sun).T*(*Sun).T*(*Sun).T ;
	(*Sun).M = (*Sun).M - (double)(360*(int)((*Sun).M/360.0)) ;
	/* en radians */
	(*Sun).L = (*Sun).L * Pi_ / 180.0 ;
	(*Sun).M = (*Sun).M * Pi_ / 180.0 ;

	/* Anomalie Vraie */
	UU = (*Sun).M ;
	XX = UU - (*Sun).e*sin(UU) ;
	while ( std::abs(1000000.0*((*Sun).M-XX)) > 5.0 )
	/* pour une precis. de l'ordre de la seconde d'arc */
		{
		UU = UU + ((*Sun).M-XX)/(1.0-(*Sun).e*cos(UU)) ;
		XX = UU - (*Sun).e*sin(UU) ;
		}
	(*Sun).V = 2.0 * atan ( sqrt((1.0+(*Sun).e)/(1.0-(*Sun).e))*tan(UU/2.0) ) ;
	/* Longitude Ecliptique */
	(*Sun).LambdaMoy = ( (*Sun).L + (*Sun).V - (*Sun).M ) ;

	(*Sun).LambdaApp = (*Sun).LambdaMoy ;
	/* Obliquite */
	(*Sun).EpsilonMoy = 23.4522944 - 0.0130125*(*Sun).T - 0.00000164*(*Sun).T*(*Sun).T ;
	(*Sun).EpsilonMoy = (*Sun).EpsilonMoy*Pi_ / 180.0 ;
	(*Sun).EpsilonApp = (*Sun).EpsilonMoy ;
	/* Correction des planetes */
	ASCorrectionPlanete ( Sun ) ;
	}
	#endif
	/*-----------------------------------------------------------------------------------*/
	#ifdef _SOLEILS2_
	void Solar::ASCalculeSoleil ( double JourJulien, TPositionSoleil* Sun )
	{
	double Omega, CC ;
	const double Pi_ = 4.0*atan(1.0) ;

	/* siecle julien equinoxe 1900 */
	(*Sun).T = ASSiecleJulien (JourJulien) ;
	/* Excentricite */
	(*Sun).e = 0.01675104 - 0.0000418*(*Sun).T*(*Sun).T
				- 0.000000126*(*Sun).T*(*Sun).T*(*Sun).T ;
	/* Parametres */
	(*Sun).L = 279.696668 + 36000.76892*(*Sun).T + 0.0003025*(*Sun).T*(*Sun).T ;
	(*Sun).L = (*Sun).L - (double)(360*(int)((*Sun).L/360.0)) ;
	(*Sun).M = 358.475830 + 35999.04975*(*Sun).T - 0.00015*(*Sun).T*(*Sun).T
				- 0.0000033*(*Sun).T*(*Sun).T*(*Sun).T ;
	(*Sun).M = (*Sun).M - (double)(360*(int)((*Sun).M/360.0)) ;
	/* en radians */
	(*Sun).L = (*Sun).L * Pi_ / 180.0 ;
	(*Sun).M = (*Sun).M * Pi_ / 180.0 ;

	/* Equation au centre */
	CC = (1.91946 - 0.004789*(*Sun).T - 0.000014*(*Sun).T*(*Sun).T)*sin((*Sun).M) +
		 (0.020094 - 0.00001*(*Sun).T)*sin(2.0*(*Sun).M) + 0.000293*sin(3.0*(*Sun).M) ;
	CC = CC*Pi_ / 180.0 ;

	(*Sun).LambdaMoy = (*Sun).L + CC ;
	(*Sun).V = (*Sun).M + CC ;

	Omega = 259.180 - 1934.142*(*Sun).T ;
	Omega = Omega - (double)(360*(int)(Omega/360.0)) ;
	Omega = Omega*Pi_/180.0 ;
	(*Sun).LambdaApp = (*Sun).LambdaMoy - Pi_*(0.005690 + 0.004790*sin(Omega))/180.0 ;

	(*Sun).EpsilonMoy = 23.4522944 - 0.0130125*(*Sun).T - 0.00000164*(*Sun).T*(*Sun).T ;
	(*Sun).EpsilonApp = (*Sun).EpsilonMoy + 0.002560*cos(Omega) ;
	(*Sun).EpsilonMoy = (*Sun).EpsilonMoy*Pi_ / 180.0 ;
	(*Sun).EpsilonApp = (*Sun).EpsilonApp*Pi_ / 180.0 ;

	/* Correction des planetes */
	ASCorrectionPlanete ( Sun ) ;
	}
	#endif
	/*-----------------------------------------------------------------------------------*/
	double Solar::ASDHEntreeAtmosphere ( double Hauteur, double RAtm )
	{
	double SinH ;
	double xx,aa,bb,cc,delta ;
	double teta ;

	SinH = sin (Hauteur) ;
	/*
	 pour y = Sin(hauteur)**2 et x = cos(teta)
	 On resoud en x :
		a**2.x**2 - 2a(1-y)x + 1 - y -a**2.y
	 */
	aa = RAtm*RAtm ;
	bb = -2.0*RAtm*(1.0-SinH*SinH) ;
	cc = 1.0 - SinH*SinH*(RAtm*RAtm+1.0) ;
	delta = bb*bb - 4.0*aa*cc ;
	if ( delta < 0.0 )
		{
		/* delta < 0 = pb de precision
			(le probleme a toujours une solution */
		xx = -bb/(2.0*aa) ;
		}
	else
		{
		if ( Hauteur >= 0.0 )
			{
			xx = (-bb + sqrt(delta)) / (2.0*aa) ;
			}
		else
			{
			xx = (-bb - sqrt(delta)) / (2.0*aa) ;
			}
		}

	teta = acos ( xx ) ;
	return ( teta ) ;
	}
	/*-----------------------------------------------------------------------------------*/
	double Solar::ASCorrigeHauteur ( double Hauteur, double RAtmosphere, double Refrac )
	{
	double DH, HCorrige, Result ;
	const double Pi_ = 4.0 * atan (1.0) ;

	HCorrige = Hauteur ;
	Result = HCorrige + 1 ;
	while ( std::abs(1000000.0*(HCorrige-Result))> 5.0 )
		{
		Result = HCorrige ;
		DH = ASDHEntreeAtmosphere ( HCorrige, RAtmosphere ) ;
		HCorrige = Refrac*cos(DH+Hauteur) ;
		if ( HCorrige < 0.707 )
			{ HCorrige = acos(HCorrige) ; }
		else
			{ HCorrige = Pi_/2.0 - asin(HCorrige) ; }
		HCorrige = HCorrige - DH ;
		}
	return HCorrige ;
	}
	/*-----------------------------------------------------------------------------------*/
	void Solar::ASCorrectionPlanete ( TPositionSoleil* Sun )
	{
	/* Soleil - corrections p. 74 : a rajouter a la longitude ecliptique moyenne */
	double DLg ;
	double AVenus, BVenus, CJupiter, DLune, EE ;
	const double Pi_ = 4.0 * atan (1.0) ;

	AVenus = 153.23 + 22518.7541*(*Sun).T ;
	BVenus = 216.57 + 45037.5082*(*Sun).T ;
	CJupiter = 312.69 + 32964.3577*(*Sun).T ;
	DLune = 350.74 + 445267.1142*(*Sun).T - 0.00144*(*Sun).T*(*Sun).T ;
	EE = 231.19 + 20.20*(*Sun).T ;
	DLg = 0.00134*cos(AVenus) + 0.00154*cos(BVenus) + 0.002*cos(CJupiter) +
		  0.00179*sin(DLune) + 0.00178*sin(EE) ;
	DLg = Pi_*DLg / 180.0 ;
	(*Sun).LambdaMoy = (*Sun).LambdaMoy + DLg ;
	(*Sun).LambdaApp = (*Sun).LambdaApp + DLg ;
	}
	/*-----------------------------------------------------------------------------------*/
	double 	Solar::ASTempsUniversel ( double JourJulien0h,
							   double HMS, double LongitudeOuestEnHeures, int Fuseau )
	{
	double			HmsTU ;
	double			JJ ;
	TPositionSoleil Sun ;

	HmsTU =  HMS ;
	if ( (Fuseau>-12)&&(Fuseau<12) )
		{
		/*Heure a GreenWitch*/
		HmsTU = HmsTU - (double)(Fuseau) ;
		}
	else
		{
		/* correction Horaire */
		HmsTU = HmsTU + LongitudeOuestEnHeures ; /* Heure a Greenwitch */
		if ( Fuseau == ASkHeureLocaleVraie )
			{
			JJ = JourJulien0h + HmsTU/24.0 ;
			ASCalculeSoleil ( JJ, &Sun ) ;
			HmsTU = HmsTU + ASCorrectionHoraire ( &Sun ) ;
			}
		}
	return HmsTU ;
	}
	//-----------------------------------------------------------------------------------
	double	Solar::ASSiecleJulien ( double JourJulien )
	{
		return ((JourJulien - 2415020.0)/ 36525.0) ;
	}

};
