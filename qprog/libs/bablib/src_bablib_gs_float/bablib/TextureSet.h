#ifndef __TEXTURE_SET__
#define __TEXTURE_SET__

#include "Texture.h"
#include <QList>

BABLIB_NAMESPACE_BEGIN

// cette classe repr�sente un ensemble de textures destin�es � �tre activ�es en m�me temps
// la destruction des textures n'est pas prise en charge par la classe
class TextureSet {
    public:
        TextureSet() {}
        int add(Texture* texture);      // retourne le num�ro de texture
        void load() const;              // charge les textures de la liste en m�moire vid�o
        void activate() const;          // active les textures
        void deactivate() const;        // desactive les textures
        void clear();                   // supprime toutes les textures de la liste
    private:
        QList<Texture*> list;
    };

BABLIB_NAMESPACE_END

#endif

