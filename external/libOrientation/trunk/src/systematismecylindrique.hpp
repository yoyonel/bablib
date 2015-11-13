#ifndef __SYSTEMATISMECYLINDRIQUE_HPP__
#define __SYSTEMATISMECYLINDRIQUE_HPP__

#include "transfo2delementaire.hpp"
#include <string>

namespace OrientationMATIS
{
	/** Systematisme Affinite de TopAero
	 * On travaille dans un repere centre sur le PPA, en Pixel
	 * avec les X dans le sens de vol, donc generalement
	 * x -> ligne
	 * y -> colonne
	 * la formule est 
	 * x = x + S1*y
	 * y = y + S2*y
	 * soit
	 * colonne += (colonne - _ppaCol)*_c1
	 * ligne += (colonne - _ppaCol)*_c0
	 */
	class SystematismeCylindriqueTopAero : public Transfo2DElementaire
		{
		protected:
			double _ppaCol;
			double _ppaLig;
			double _c0;
			double _c1;
			
		public:
			
			static std::string nomTagXml;
			
			double GetS1()const;
			double GetS2()const;
			double GetC0()const;
			double GetL0()const;
			SystematismeCylindriqueTopAero(bool isInterne);
			
			SystematismeCylindriqueTopAero(double ppaCol,double ppaLig, double S1, double S2, bool isInterne);
			
			SystematismeCylindriqueTopAero(const SystematismeCylindriqueTopAero &h);
			
			shared_ptr<Transfo2DElementaire> Clone() const;
			
			void ApplyLocalToImage(double &c, double &l) const;
			
			void ApplyImageToLocal(double &c, double &l) const;
		};
} // namespace


#endif