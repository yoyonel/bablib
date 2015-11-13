#ifndef __TEXTURE_SET__
#define __TEXTURE_SET__

#include "Texture.h"
#include <QList>

BABLIB_NAMESPACE_BEGIN

// cette classe reprÃÂ©sente un ensemble de textures destinÃÂ©es ÃÂ  ÃÂªtre activÃÂ©es en mÃÂªme temps
// la destruction des textures n'est pas prise en charge par la classe
class TextureSet {
    public:
        TextureSet() {}
        int add(Texture* texture);      // retourne le numÃÂ©ro de texture
        void load() const;              // charge les textures de la liste en mÃÂ©moire vidÃÂ©o
        void activate() const;          // active les textures
        void deactivate() const;        // desactive les textures
        void clear();                   // supprime toutes les textures de la liste
    private:
        QList<Texture*> list;
    };

BABLIB_NAMESPACE_END

#endif

