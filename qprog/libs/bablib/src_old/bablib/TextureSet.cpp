#include "TextureSet.h"
#include "Message.h"
BABLIB_NAMESPACE_USE

int TextureSet::add(Texture* texture) {
    if (list.size() >= Texture::maxUnits())
        Message::warning(QString("nombre maximum d'unités de textures (%1) atteint").arg(Texture::maxUnits()));
    list.append(texture);
    return list.size() - 1;
    }

void TextureSet::load() const {
    foreach (Texture *tex, list) tex->load();
    }

void TextureSet::activate() const {
    int n = 0;
    //foreach (Texture *tex, list) tex->activate(n++);    // associe la texture <tex> à l'unité de texture <n>
    foreach (Texture *tex, list) {
        // associe la texture <tex> à l'unité de texture <n> :
        Texture::setActiveUnit(n++);
        tex->bind();
        }
    Texture::setActiveUnit(0);
    }

void TextureSet::deactivate() const {
    /*
    int n = 0;
    foreach (Texture *tex, list) tex->deactivate(n++);  // désactive l'unité de texture <n>
    */
    }

void TextureSet::clear() {
    // les textures ne sont pas détruites :
    list.clear();
    }

//@@ a voir :
//  - glAreTexturesResident
//  - glPrioritizeTextures           
