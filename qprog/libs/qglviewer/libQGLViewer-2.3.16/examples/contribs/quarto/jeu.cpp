/****************************************************************************

 Copyright (C) 2002-2011 Gilles Debunne. All rights reserved.

 This file is part of the QGLViewer library version 2.3.16.

 http://www.libqglviewer.com - contact@libqglviewer.com

 This file may be used under the terms of the GNU General Public License 
 versions 2.0 or 3.0 as published by the Free Software Foundation and
 appearing in the LICENSE file included in the packaging of this file.
 In addition, as a special exception, Gilles Debunne gives you certain 
 additional rights, described in the file GPL_EXCEPTION in this package.

 libQGLViewer uses dual licensing. Commercial/proprietary software must
 purchase a libQGLViewer Commercial License.

 This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/

#include "jeu.h"

bool Jeu::needDrawing(int i)
{ return tab[i].empty; }

void Jeu::placePiece(int select, Piece *p)
{
<<<<<<< HEAD:qprog/libs/qglviewer/libQGLViewer-2.3.16/examples/contribs/quarto/jeu.cpp
  // on verifie que la piece n'ai pas �t� d�ja plac�e autre part
||||||| parent of 57cec52... local change (ign)
  // on verifie que la piece n'ai pas ÃÂÃÂ©tÃÂÃÂ© dÃÂÃÂ©ja placÃÂÃÂ©e autre part
=======
  // on verifie que la piece n'ai pas été déja placée autre part
>>>>>>> 57cec52... local change (ign):qprog/libs/qglviewer/libqglviewer/examples/contribs/quarto/jeu.cpp
  for(int i=0; i<16; i++)
    if(tab[i].piece==p) {
<<<<<<< HEAD:qprog/libs/qglviewer/libQGLViewer-2.3.16/examples/contribs/quarto/jeu.cpp
      if(i==select) return; // la piece a d�ja �t� plac�e au bon endroit
      // si oui, on l'enl�ve
||||||| parent of 57cec52... local change (ign)
      if(i==select) return; // la piece a dÃÂÃÂ©ja ÃÂÃÂ©tÃÂÃÂ© placÃÂÃÂ©e au bon endroit
      // si oui, on l'enlÃÂÃÂ¨ve
=======
      if(i==select) return; // la piece a déja été placée au bon endroit
      // si oui, on l'enlève
>>>>>>> 57cec52... local change (ign):qprog/libs/qglviewer/libqglviewer/examples/contribs/quarto/jeu.cpp
      tab[i].empty=true;
      tab[i].piece=NULL;
    }
  // on place la piece
  tab[select].empty=false;
  tab[select].piece=p;
}

<<<<<<< HEAD:qprog/libs/qglviewer/libQGLViewer-2.3.16/examples/contribs/quarto/jeu.cpp
// Vrai ssi les 4 pieces ont unbe caract�ristique commune
||||||| parent of 57cec52... local change (ign)
// Vrai ssi les 4 pieces ont unbe caractÃÂÃÂ©ristique commune
=======
// Vrai ssi les 4 pieces ont unbe caractéristique commune
>>>>>>> 57cec52... local change (ign):qprog/libs/qglviewer/libqglviewer/examples/contribs/quarto/jeu.cpp
bool Jeu::caracCommune(Piece* pieces[4])
{
  for (int i=0; i<4; ++i)
    if (!pieces[i])
      return false;

  // sinon on teste les caracteristiques communes
  // la taille
  if (pieces[0]->getTaille()==pieces[1]->getTaille() &&
      pieces[0]->getTaille()==pieces[2]->getTaille() &&
      pieces[0]->getTaille()==pieces[3]->getTaille()) return true;
  // la couleur
  if (pieces[0]->getCouleur()==pieces[1]->getCouleur() &&
      pieces[0]->getCouleur()==pieces[2]->getCouleur() &&
      pieces[0]->getCouleur()==pieces[3]->getCouleur()) return true;
  // la forme
  if (pieces[0]->getForme()==pieces[1]->getForme() &&
      pieces[0]->getForme()==pieces[2]->getForme() &&
      pieces[0]->getForme()==pieces[3]->getForme()) return true;
  // le trou
  if (pieces[0]->getTrou()==pieces[1]->getTrou() &&
      pieces[0]->getTrou()==pieces[2]->getTrou() &&
      pieces[0]->getTrou()==pieces[3]->getTrou()) return true;

  return false;
}

<<<<<<< HEAD:qprog/libs/qglviewer/libQGLViewer-2.3.16/examples/contribs/quarto/jeu.cpp
// Analyse si un alignement a �t� effectu�
||||||| parent of 57cec52... local change (ign)
// Analyse si un alignement a ÃÂÃÂ©tÃÂÃÂ© effectuÃÂÃÂ©
=======
// Analyse si un alignement a été effectué
>>>>>>> 57cec52... local change (ign):qprog/libs/qglviewer/libqglviewer/examples/contribs/quarto/jeu.cpp
bool Jeu::analyze()
{
  Piece* pieces[4];

  // on teste les lignes //
  for(int j=0; j<4; j++) {
    for (int i=0; i<4; ++i)
      pieces[i] = tab[j*4+i].piece;
    if (caracCommune(pieces))
      return true;
  }

  // on teste les colonnes //
  for(int j=0; j<4; j++) {
    for (int i=0; i<4; ++i)
      pieces[i] = tab[4*i+j].piece;
    if (caracCommune(pieces))
      return true;
  }

  // on teste les diagonales //
  for (int i=0; i<4; ++i)
    pieces[i] = tab[5*i].piece;
  if (caracCommune(pieces))
    return true;

  for (int i=0; i<4; ++i)
    pieces[i] = tab[3*(i+1)].piece;
  if (caracCommune(pieces))
    return true;

  return false;
}
