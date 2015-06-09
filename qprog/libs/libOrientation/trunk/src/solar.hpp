#ifndef __SOLAR_H__
#define __SOLAR_H__

namespace OrientationMATIS
{
//--------------------------------------------------------------------------
// Fichier              : solar.h
// Auteur               : Didier BOLDO a partir de orisol et astro
// Creation             : 08/02/99
//--------------------------------------------------------------------------
// Historique :
//  08/02/99			: CREATION
//  03/05/99            : Modification pour laisser la possibilite d'utiliser
//                        ImageOrientation ET orilib (probleme de link du a
//                        des fonctions de meme nom)
//  15/02/00            : Decouverte d'un BUG : le temps considere en entree
//                        etait considere en heure locale vraie et non en
//                        TU -> modification du flage Fuseau a 0
//--------------------------------------------------------------------------
// Modif D. BOLDO 14/02/2000
//#define orkMeridienDeParis    2.0+20.0/60.0+16.0/3600.0
// 2°20'16'' Est
#define orkMeridienDeParis      2.0+20.0/60.0+14.025/3600.0     //2°20'14.02500''

/*
 Constantes pour le parametre Fuseau
 */
#define ASkHeureLocaleMoyenne 	 999
#define ASkHeureLocaleVraie		1000

class Solar
{

public :
static void orvecteur_soleil_terre (	int annee, int mois, int jour, int heure, int minute,
					double latitude, double longitude, // en degres
			      		double soleil[3] ) ;

static void Lamb_geo( double /*xx*/, double /*yy*/, int /*NLamb*/,
	      double& /*Longitude*/, double& /*Latitude*/) ;
static void Geo_lamb( double /*Longitude*/, double /*Latitude*/, int /*NLamb*/,
	      double& /*xx*/, double& /*yy*/ ) ;

/*
	Routines Generales pour passage
	(AscensionDroite,Declinaison) -> Repere Local (Azimut,Hauteur)
*/
static double 	ASJourJulien ( int /*Annee*/ , int /*Mois*/, int /*Quantieme*/ ) ;
static double 	ASTempsUniversel ( double /*JourJulienZeroHeure*/,
						   double /*HeureMinuteSeconde*/,
						   double /*LongitudeOuestEnHeures*/,
						   int /*Fuseau : 	0 = TU (Greenwitch)
											-11..11 = decalage horaire/Grennwitch
											999 = Heure locale vraie */ ) ;
static double 	ASTempsSideralMoyen ( double /*JourJulienZeroHeure*/,
							  double /*TempsUniverselEnHeures*/,
							  double /*LongitudeOuestEnHeures*/) ;
static void	ASAlphaDeltaToAzHt(	double /*TempsSideralMoyenEnRadians*/,
							double /*LatitudeEnRadians*/,
							double /*AscensionDroiteEnRadians*/,
							double /*DeclinaisonEnRadins*/,
							double* /*AzimutNordEstEnRadians*/,
							double* /*HauteurEnRadians*/) ;
/*
	Calcul de Planetes
*/
static void 	ASAlphaDeltaSoleil( double /*JourJulien*/,
							double* /*AscensionDroiteSoleilEnRadians*/,
							double* /*DeclinaisonSoleilEnRadians*/);
protected :
typedef struct
{
	double T ;	/* Jour Julien en Siecles Juliens */
	double L ; /* Longitude du perihelie + Anomalie Moyenne */
	double M ; /* anomalie moyennes */
	double V ; /* anomalie vraie */
	double LambdaApp ; /* longitude geometrique apparente */
	double LambdaMoy ; /* longitude geometrique moyenne */
	double e ; /* excentricite */
	double EpsilonApp ; /* obliquite apparente */
	double EpsilonMoy ; /* obliquite moyenne */
	double Abberation ; /* distance horaire du soleil */
} TPositionSoleil ;


static void 	ASCalculeSoleil ( double /*JourJulien*/, TPositionSoleil* /*SunRecord*/ ) ;
static double 	ASCorrectionHoraire ( TPositionSoleil* /*SunRecord*/ ) ;
static void 	ASCorrectionPlanete ( TPositionSoleil* /*SunRecord*/ ) ;
static double	ASSiecleJulien ( double /*JourJulien*/ ) ;
static double 	ASCorrigeHauteur ( 	double /*Hauteur*/,
							double /*RayonAtmo*/, double /*Refraction*/ ) ;
static double 	ASDHEntreeAtmosphere ( double /*Hauteur*/, double /*RayonAtmo*/ ) ;

};
} // namespace

#endif
