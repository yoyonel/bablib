#ifndef __TRANSFO2DELEMENTAIRE_HPP__
#define __TRANSFO2DELEMENTAIRE_HPP__

#include "noboost.hpp"

namespace OrientationMATIS
{
	class Transfo2DElementaire
	{
		Transfo2DElementaire();
	protected :
		bool m_isInterne;

		Transfo2DElementaire(const Transfo2DElementaire &o){ m_isInterne = o.m_isInterne;}

	public :
		Transfo2DElementaire(bool isInterne) : m_isInterne(isInterne){;}
		virtual ~Transfo2DElementaire() {;}

		virtual shared_ptr<Transfo2DElementaire> Clone() const =0;
		virtual void ApplyLocalToImage(double &c, double &l) const =0;
		virtual void ApplyImageToLocal(double &c, double &l) const =0;
		
		bool IsInterne() const { return m_isInterne; }
		void IsInterne(bool isInterne) { m_isInterne = isInterne; }
	};
} // namespace

#endif //#ifndef __TRANSFO2DELEMENTAIRE_HPP__
