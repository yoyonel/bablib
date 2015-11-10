#ifndef __MODELEPROJECTION_HPP__
#define __MODELEPROJECTION_HPP__

#include "noboost.hpp"
#include "../config/config_cgal.hpp"

#include "transfo2d.hpp"
#include "auxiliarydata.hpp"
#include "geodesie.hpp"
#include "euclidien.hpp"

/**
 * \brief Classe de gestion des orientations image.
 *
 * \author
 * \date
 *
 **/

namespace OrientationMATIS
{
class ModeleProjectionVisitor;

class ModeleProjection
{
protected:

	unsigned int m_width, m_height;


	AuxiliaryData m_data;
	Transfo2D m_transfo2D;

	/// Systeme geodesique interne de l'orientation (base sur proj4)
	shared_ptr<Geodesie> m_geodesie;
	shared_ptr<Euclidien> m_euclidien;

	ModeleProjection(unsigned int nc, unsigned int nl) :
		m_width(nc), m_height(nl), m_geodesie(Geodesie::CreateFromName(""))
	{
		;
	}

	ModeleProjection(unsigned int nc, unsigned int nl, AuxiliaryData data, Transfo2D transfo2D, std::string systemeGeodesie, shared_ptr<Euclidien> euclidien) :
		m_width(nc), m_height(nl), m_data(data), m_transfo2D(transfo2D), 
		m_geodesie(Geodesie::CreateFromName(systemeGeodesie)), m_euclidien(euclidien)
	{
		;
	}

	ModeleProjection(const ModeleProjection &mod) :
		m_width(mod.m_width), m_height(mod.m_height),
		 m_data(mod.m_data), m_transfo2D(mod.m_transfo2D), m_geodesie(mod.m_geodesie), 
		 m_euclidien(mod.m_euclidien)
	{
		;
	}

public:
	static void InitAllIO();
	static shared_ptr<ModeleProjection> ReadFile(const std::string &filename);
	static bool WriteFile(const shared_ptr<ModeleProjection> ori, const std::string &filename);

	virtual ~ModeleProjection()
	{
		; 
	}
	virtual bool Visit(shared_ptr<ModeleProjectionVisitor> &) const = 0;

	bool InternalCheck(double precision = 0.01) const;


	void GetTailleImage(unsigned int &nc, unsigned int & nl) const
	{
		nc = m_width;
		nl = m_height;
	}
	
	void SetTailleImage(unsigned int nc, unsigned int nl)
	{
		m_width = nc;
		m_height = nl;
	}
	
	AuxiliaryData& GetAuxiliaryData()
	{
		return m_data;
	}
	const AuxiliaryData& GetAuxiliaryData() const
	{
		return m_data;
	}
	void SetAuxiliaryData(const AuxiliaryData & data)
	{
		m_data = data;
	}

	const Transfo2D & GetTransfo2D() const
	{
		return m_transfo2D;
	}
	void ClearTransfo2D(void)
	{
		m_transfo2D.Clear();
	}
	
	
	void AddTransformation2DElementaire(shared_ptr<Transfo2DElementaire> tr)
	{
		m_transfo2D.AddTransformation2D(tr);
	}

	std::string GetSystemGeodesie() const
	{
		return m_geodesie->GetFullName();
	}

	shared_ptr<Euclidien> GetEuclidien() const
	{
		return m_euclidien;
	}

	/// Les 4 methodes virtuelles surchargees par les projections reelles
	virtual shared_ptr<ModeleProjection> Clone() const = 0;
	virtual void LocalToImage(double xl, double yl, double zl, double &c, double &l) const =0;
	virtual void ImageAndZToLocal(double c, double l, double z, double &x, double &y) const =0;
	virtual void ImageToLocalVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const =0;
	//ajout NB 03/08/2009
        //virtual bool GetSommet(double& x, double& y, double& z) const {return false;}
        virtual bool GetSommet(double& , double& , double& ) const {return false;}

	/// Les methodes de gestion de la geodesie
	bool WorldToLocal(const char *world_system_name, const double xw, const double yw, const double zw, double &xl, double &yl, double &zl) const;
	bool LocalToWorld(const double xl, const double yl, const double zl, const char *world_system_name, double &xw, double &yw, double &zw) const;

	/// La methode de projection generique
	bool WorldToImage(const char *world_system_name, const double xw, const double yw, const double zw, double &c, double &l) const;

	/**
	 * @name Calcul des emprises
	 */
	/*@{*/
private:
	/// Permet la projection des points
	void FootprintHelper(double z_min, double z_max, double c0, double l0, double c1, double l1, std::vector<double> &coord_x, std::vector<double> &coord_y);
	void FootprintHelper(const char *world_system_name, double z_min, double z_max, double c0, double l0, double c1, double l1, std::vector<double> &coord_x, std::vector<double> &coord_y);
public:
	/// Calcul de la bounding box de l'emprise en coordonnees locales
	void FootprintLocal(double z_min, double z_max, double c0, double l0, double c1, double l1, double &xmin, double &xmax, double &ymin, double &ymax);
	void FootprintLocal(double z_min, double z_max, double &xmin, double &xmax, double &ymin, double &ymax);
	/// Calcul de l'enveloppe convexe de l'emprise en coordonnees locales (uniquement disponible si lib compilee avec CGAL)
#if ORIENTATIONMATIS_USE_CGAL
	void FootprintLocal(double z_min, double z_max, double c0, double l0, double c1, double l1, footprint_type &footprint);
	void FootprintLocal(double z_min, double z_max, footprint_type &footprint);
#endif // ORIENTATIONMATIS_USE_CGAL
	/// Calcul de la bounding box de l'emprise en coordonnees generiques
	void FootprintWorld(const char *world_system_name, double z_min, double z_max, double c0, double l0, double c1, double l1, double &xmin, double &xmax, double &ymin, double &ymax);
	void FootprintWorld(const char *world_system_name, double z_min, double z_max, double &xmin, double &xmax, double &ymin, double &ymax);
	/// Calcul de l'enveloppe convexe de l'emprise en coordonnees generiques (uniquement disponible si lib compilee avec CGAL)
#if ORIENTATIONMATIS_USE_CGAL
	void FootprintWorld(const char *world_system_name, double z_min, double z_max, double c0, double l0, double c1, double l1, footprint_type &footprint);
	void FootprintWorld(const char *world_system_name, double z_min, double z_max, footprint_type &footprint);
#endif // ORIENTATIONMATIS_USE_CGAL
	/*@}*/

};

} // namespace

#endif //#ifndef __MODELEPROJECTION_HPP__
