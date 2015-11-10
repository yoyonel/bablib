#ifndef __SYSTEMATISMEAFFINE_HPP__
#define __SYSTEMATISMEAFFINE_HPP__

#include "transfo2delementaire.hpp"
#include <math.h>
#include <string>

namespace OrientationMATIS
{
	/** Systematisme Affinite de TopAero
	 * On travaille dans un repere centre sur le PPA, en Pixel
	 * avec les X dans le sens de vol, donc generalement
	 * x -> ligne
	 * y -> colonne
	 * la formule est R=Racine(x2+y2)/100
	 * x = x + x[S6+S7(R-1)+S8(R-1)2]/100
	 * y = y + y[-S6+S7(R-1)+S8(R-1)2]/100
	 * soit
	 * colonne += (ci-_ppaCol)*(-_c0+_c1*(R-1.)+_c2*(R-1.)*(R-1.))/100
	 * ligne += (li-_ppaLig)*(_c0+_c1*(R-1.)+_c2*(R-1.)*(R-1.))/100
	 */
	class SystematismeAffineTopAero : public Transfo2DElementaire
		{
		protected:
			double _ppaCol;
			double _ppaLig;
			double _c0;
			double _c1;
			double _c2;
			
			public :
			static std::string nomTagXml;
			
			double GetS6()const;
			double GetS7()const;
			double GetS8()const;
			double GetC0()const;
			double GetL0()const;
			
			SystematismeAffineTopAero(bool isInterne);
			
			SystematismeAffineTopAero(double ppaCol,double ppaLig, double S6, double S7, double S8, bool isInterne);
						
			SystematismeAffineTopAero(const SystematismeAffineTopAero &h);
			
			shared_ptr<Transfo2DElementaire> Clone() const;
			
			void ApplyLocalToImage(double &c, double &l) const;
			
		void ApplyImageToLocal(double &c, double &l) const;
		};
} // namespace


#endif