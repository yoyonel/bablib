#include "TextureSet.h"
#include "Message.h"
BABLIB_NAMESPACE_USE

int TextureSet::add(Texture* texture) {
    if (list.size() >= Texture::maxUnits())
        Message::warning(QString("nombre maximum d'unitÃÂ©s de textures (%1) atteint").arg(Texture::maxUnits()));
    list.append(texture);
    return list.size() - 1;
    }

void TextureSet::load() const {
    foreach (Texture *tex, list) tex->load();
    }

void TextureSet::activate() const {
    int n = 0;
    //foreach (Texture *tex, list) tex->activate(n++);    // associe la texture <tex> ÃÂ  l'unitÃÂ© de texture <n>
    foreach (Texture *tex, list) {
        // associe la texture <tex> ÃÂ  l'unitÃÂ© de texture <n> :
        Texture::setActiveUnit(n++);
        tex->bind();
        }
    Texture::setActiveUnit(0);
    }

void TextureSet::deactivate() const {
    /*
    int n = 0;
    foreach (Texture *tex, list) tex->deactivate(n++);  // dÃÂ©sactive l'unitÃÂ© de texture <n>
    */
    }

void TextureSet::clear() {
    // les textures ne sont pas dÃÂ©truites :
    list.clear();
    }

//@@ a voir :
//  - glAreTexturesResident
//  - glPrioritizeTextures           
