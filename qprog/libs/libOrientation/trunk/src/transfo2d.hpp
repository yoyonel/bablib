#ifndef __TRANSFO2D_HPP__
#define __TRANSFO2D_HPP__

#include <vector>
#include "transfo2delementaire.hpp"


namespace OrientationMATIS
{
	class Transfo2D
	{
	public :

		Transfo2D() {;}

		Transfo2D(const Transfo2D &t2d)
		{
			(*this) = t2d;
		}

		Transfo2D & operator=(const Transfo2D &t2d)
		{
            m_pileTransfo.clear();
			for (unsigned int i=0; i<t2d.m_pileTransfo.size(); ++i)
				m_pileTransfo.push_back(t2d.m_pileTransfo[i]->Clone());
			return (*this);
		}

		~Transfo2D() { ; }

		void AddTransformation2D(shared_ptr<Transfo2DElementaire> tr)
		{
			m_pileTransfo.push_back(tr);
		}

		void ApplyLocalToImage(double &c, double &l) const
		{
			std::vector< shared_ptr<Transfo2DElementaire> >::const_iterator it = m_pileTransfo.begin(), fin = m_pileTransfo.end();
			for (; it != fin; ++it)
				(*it)->ApplyLocalToImage(c,l);
		}

		void ApplyImageToLocal(double &c, double &l) const
		{
			std::vector< shared_ptr<Transfo2DElementaire> >::const_reverse_iterator it = m_pileTransfo.rbegin(), fin = m_pileTransfo.rend();
			for (; it != fin; ++it)
				(*it)->ApplyImageToLocal(c,l);
		}

		unsigned int GetNbTransfo2DElementaire() const
		{
			return m_pileTransfo.size();
		}

		const shared_ptr<Transfo2DElementaire> GetTransfo2DElementaire(unsigned int i) const
		{
			return m_pileTransfo[i];
		} 

		bool IsInterne(unsigned int i) const
		{
			return m_pileTransfo[i]->IsInterne();
		}

		void Clear()
		{
			m_pileTransfo.clear();
		}

	protected:
		std::vector< shared_ptr<Transfo2DElementaire> > m_pileTransfo;
	};
} // namespace

#endif //#ifndef __TRANSFO2D_HPP__
