#ifndef __MODELEPROJECTIONGRILLE_HPP__
#define	__MODELEPROJECTIONGRILLE_HPP__

#include <complex>
#include "modeleprojection.hpp"
#include "modeleprojectionvisitor.hpp"

namespace OrientationMATIS
{
class ModeleProjectionGrille : public ModeleProjection
{
	public :
		std::complex<double> interpolation(std::complex<double> position_grille,
							std::vector<double> const &grille,
							std::complex<int> Taille, int TailleZ) const;

		std::vector< std::vector<double> > Image2Obj;
		std::vector< std::vector<double> > Obj2Image;
		std::vector<double> Image2Obj_Value;
		std::vector<double> Obj2Image_Value;
		std::complex<double> Image2Obj_ULC;
		std::complex<double> Image2Obj_Pas;
		std::complex<int> Image2Obj_Taille;
		int Image2Obj_TailleZ;
		std::complex<double> Obj2Image_ULC;
		std::complex<double> Obj2Image_Pas;
		std::complex<int> Obj2Image_Taille;
		int Obj2Image_TailleZ;

		double PrecisionRetour;

	public :
		ModeleProjectionGrille(unsigned int nc, unsigned int nl);
		ModeleProjectionGrille(const ModeleProjectionGrille &modgri);
		~ModeleProjectionGrille();
		bool Visit(shared_ptr<ModeleProjectionVisitor> &exporter) const { return exporter->Visit(this); };
		shared_ptr<ModeleProjection> Clone() const;
		void LocalToImage(double x, double y, double z, double &c, double &l) const;
		void ImageAndZToLocal(double c, double l, double z, double &x, double &y) const;
		void ImageToLocalVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const;
};
} // namespace

#endif
