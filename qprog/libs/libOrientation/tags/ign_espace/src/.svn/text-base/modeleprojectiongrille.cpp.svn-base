#include "modeleprojectiongrille.hpp"

namespace OrientationMATIS
{

	ModeleProjectionGrille::ModeleProjectionGrille(unsigned int nc, unsigned int nl) : ModeleProjection(nc, nl)
	{
		Image2Obj_Taille = std::complex<int>(0,0);
		Image2Obj_TailleZ = 0;
		Obj2Image_Taille = std::complex<int>(0,0);
		Obj2Image_TailleZ = 0;
	}

	ModeleProjectionGrille::ModeleProjectionGrille(const ModeleProjectionGrille &modgri) : ModeleProjection(modgri)
	{
		Image2Obj		=modgri.Image2Obj;
		Obj2Image		=modgri.Obj2Image;
		Image2Obj_Value		=modgri.Image2Obj_Value;
		Obj2Image_Value		=modgri.Obj2Image_Value;
		Image2Obj_ULC		=modgri.Image2Obj_ULC;
		Image2Obj_Pas		=modgri.Image2Obj_Pas;
		Image2Obj_Taille	=modgri.Image2Obj_Taille;
		Image2Obj_TailleZ	=modgri.Image2Obj_TailleZ;
		Obj2Image_ULC		=modgri.Obj2Image_ULC;
		Obj2Image_Pas		=modgri.Obj2Image_Pas;
		Obj2Image_Taille	=modgri.Obj2Image_Taille;
		Obj2Image_TailleZ	=modgri.Obj2Image_TailleZ;
		PrecisionRetour		=modgri.PrecisionRetour;
	}

	ModeleProjectionGrille::~ModeleProjectionGrille()
	{}

	shared_ptr<ModeleProjection> ModeleProjectionGrille::Clone() const
	{
		return shared_ptr<ModeleProjectionGrille>( new ModeleProjectionGrille(*this) );
	}

	void ModeleProjectionGrille::LocalToImage(double x, double y, double z, double &c, double &l) const
	{
		// On verifie que la grille est disponible
		if (Obj2Image.size()==0)
		{
			return;
		}
		// Position dans la grille
		std::complex<double> position_grille((x-Obj2Image_ULC.real())/Obj2Image_Pas.real(),
							  (Obj2Image_ULC.imag()-y)/Obj2Image_Pas.imag());
		// Recherche des deux niveaux les plus proches
		int l1=-1;
		double d1=0.;
		int l2=-1;
		double d2=0.;
		for(size_t id=0;id<Obj2Image_Value.size();++id)
		{
			double d=z-Obj2Image_Value[id];
			if (l1==-1)
			{
				l1 = (int) id;
				d1 = d;
			}
			else if (std::abs(d)<std::abs(d1))
			{
				l2 = l1;
				d2 = -d1;
				l1 = (int) id;
				d1 = d;
			}
			else if ((l2==-1)||(std::abs(d)<std::abs(d2)))
			{
				l2 = (int) id;
				d2 = -d;
			}
		}

		std::complex<double> P1 = interpolation(position_grille,Obj2Image[l1],Obj2Image_Taille,Obj2Image_TailleZ);
		std::complex<double> P2 = interpolation(position_grille,Obj2Image[l2],Obj2Image_Taille,Obj2Image_TailleZ);

		c = (P1.real()*d2+P2.real()*d1)/(d1+d2);
		l = (P1.imag()*d2+P2.imag()*d1)/(d1+d2);

		double Xretour,Yretour;
		ImageAndZToLocal(c,l,z,Xretour,Yretour);
		double Err = sqrt((x-Xretour)*(x-Xretour)+(y-Yretour)*(y-Yretour));
		int NbEtape = 0;
		if (Err<=PrecisionRetour)
			return;
		while((Err>PrecisionRetour)&&(NbEtape<10))
		{
			++NbEtape;
			double X1,Y1,X2,Y2;
			ImageAndZToLocal(c+1,l,z,X1,Y1);
			ImageAndZToLocal(c,l+1,z,X2,Y2);
			double dX1,dY1,dX2,dY2;
			dX1 = X1-Xretour;dY1 = Y1-Yretour;
			dX2 = X2-Xretour;dY2 = Y2-Yretour;
			double N = sqrt(dX1*dX1+dY1*dY1);
			if (N==0.)
				return;
			double dc = (dX1*(Xretour-x)+dY1*(Yretour-y))/N/N;
			double dl = (dX2*(Xretour-x)+dY2*(Yretour-y))/N/N;
			c=c-dc;
			l=l-dl;
			ImageAndZToLocal(c,l,z,Xretour,Yretour);
			Err = sqrt((x-Xretour)*(x-Xretour)+(y-Yretour)*(y-Yretour));
		}
		m_transfo2D.ApplyLocalToImage(c,l);
	}

	void ModeleProjectionGrille::ImageAndZToLocal(double c, double l, double z, double &x, double &y) const
	{
		
		m_transfo2D.ApplyImageToLocal(c,l);
		// Position dans la grille
		std::complex<double> position_grille((c-Image2Obj_ULC.real())/Image2Obj_Pas.real(),
							  (Image2Obj_ULC.imag()-l)/Image2Obj_Pas.imag());
		// Recherche des deux niveaux les plus proches
		int l1=-1;
		double d1=0.;
		int l2=-1;
		double d2=0.;
		for(size_t id=0;id<Image2Obj_Value.size();++id)
		{
			double d=z-Image2Obj_Value[id];
			if (l1==-1)
			{
				l1 = (int) id;
				d1 = d;
			}
			else if (std::abs(d)<std::abs(d1))
			{
				l2 = l1;
				d2 = -d1;
				l1 = (int) id;
				d1 = d;
			}
			else if ((l2==-1)||(std::abs(d)<std::abs(d2)))
			{
				l2 = (int) id;
				d2 = -d;
			}
		}

		std::complex<double> P1 = interpolation(position_grille,Image2Obj[l1],Image2Obj_Taille,Image2Obj_TailleZ);
		std::complex<double> P2 = interpolation(position_grille,Image2Obj[l2],Image2Obj_Taille,Image2Obj_TailleZ);

		x = (P1.real()*d2+P2.real()*d1)/(d1+d2);
		y = (P1.imag()*d2+P2.imag()*d1)/(d1+d2);
	}

	void ModeleProjectionGrille::ImageToLocalVec(double c, double l, double &x0, double &y0, double &z0, double &x1, double &y1, double &z1) const
	{
		z0 = 1;
		z1 = 0;
		ImageAndZToLocal(c, l, z0, x0, y0);
		ImageAndZToLocal(c, l, z1, x1, y1);
	}

	std::complex<double> ModeleProjectionGrille::interpolation(std::complex<double> position_grille,std::vector<double> const &grille,std::complex<int> Taille, int TZ) const
	{
		if ((Taille.real()<2)||(Taille.imag()<2)||(TZ<2))
			return std::complex<double>(0.,0.);
		int col = (int)floor(position_grille.real());
		int lig = (int)floor(position_grille.imag());
		if (col<0) col=0;
		if (lig<0) lig=0;
		if (col>(Taille.real()-2)) col = Taille.real()-2;
		if (lig>(Taille.imag()-2)) lig = Taille.imag()-2;
		double dcol = position_grille.real()-col;
		double dlig = position_grille.imag()-lig;
		// Les quatres Points autour de cette position
		std::complex<double> A(grille[(lig*Taille.real()+col)*TZ],
					   grille[(lig*Taille.real()+col)*TZ+1]);
		std::complex<double> B(grille[(lig*Taille.real()+col+1)*TZ],
					   grille[(lig*Taille.real()+col+1)*TZ+1]);
		std::complex<double> C(grille[((lig+1)*Taille.real()+col)*TZ],
					   grille[((lig+1)*Taille.real()+col)*TZ+1]);
		std::complex<double> D(grille[((lig+1)*Taille.real()+col+1)*TZ],
					   grille[((lig+1)*Taille.real()+col+1)*TZ+1]);
		// Interpolation bi lineaire
		std::complex<double> Pt((A.real()*(1-dcol)+B.real()*dcol)*(1-dlig)+(C.real()*(1-dcol)+D.real()*dcol)*dlig,
					 (A.imag()*(1-dcol)+B.imag()*dcol)*(1-dlig)+(C.imag()*(1-dcol)+D.imag()*dcol)*dlig);
		return Pt;
	}
};
