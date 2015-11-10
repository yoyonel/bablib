#include "AttachedImage.h"
BABLIB_NAMESPACE_USE

/*********************************************************************************************/
// AttachedImage :

void AttachedImage::destroy() {
    detach();
    if (ownImage) image->destroy();
    delete image;
    }
void AttachedImage::attach() { image->attach(attachment); }
void AttachedImage::detach() { image->detach(attachment); }
Dim2D AttachedImage::size() const { return image->size(); }

