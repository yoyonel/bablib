#ifndef __TEXTURE_BUFFER__
#define __TEXTURE_BUFFER__

#include "BufferGL.h"
#include "Image1D.h"
#include "TextureData.h"

BABLIB_NAMESPACE_BEGIN

class TextureBuffer : public TextureData {
    public:
        TextureBuffer(const AbstractImage1D& source);
        //@ ajouter constructeur pour Tab1D
        virtual GLenum textureMode() const;
        virtual GLint defaultTexFormat() const;
        virtual void loadToGPU(GLint texFormat) const;
        
        void destroy();     //@ a voir : faire remonter dans Texture? + gestion objet TextureBuffer + creation facile..
    
    private:
        const AbstractImage1D& source;
        mutable BufferGL buffer;
    };

/*
// valeurs acceptées pour <texFormat> :
//-------------------------------------
// (attention : le format de <source> (CPU) doit être le même que celui spécifié pour <texFormat> (GPU)
//  car le buffer GPU est une simple copie du buffer CPU)

      ALPHA8                     ubyte         1        Y     A . . .
      ALPHA16                    ushort        1        Y     A . . .
      ALPHA16F_ARB               half          1        N     A . . .
      ALPHA32F_ARB               float         1        N     A . . .
      ALPHA8I_EXT                byte          1        N     A . . .
      ALPHA16I_EXT               short         1        N     A . . .
      ALPHA32I_EXT               int           1        N     A . . .
      ALPHA8UI_EXT               ubyte         1        N     A . . .
      ALPHA16UI_EXT              ushort        1        N     A . . .
      ALPHA32UI_EXT              uint          1        N     A . . .

      LUMINANCE8                 ubyte         1        Y     L . . .
      LUMINANCE16                ushort        1        Y     L . . .
      LUMINANCE16F_ARB           half          1        N     L . . .
      LUMINANCE32F_ARB           float         1        N     L . . .
      LUMINANCE8I_EXT            byte          1        N     L . . .
      LUMINANCE16I_EXT           short         1        N     L . . .
      LUMINANCE32I_EXT           int           1        N     L . . .
      LUMINANCE8UI_EXT           ubyte         1        N     L . . .
      LUMINANCE16UI_EXT          ushort        1        N     L . . .
      LUMINANCE32UI_EXT          uint          1        N     L . . .

      LUMINANCE8_ALPHA8          ubyte         2        Y     L A . .
      LUMINANCE16_ALPHA16        ushort        2        Y     L A . .
      LUMINANCE_ALPHA16F_ARB     half          2        N     L A . .
      LUMINANCE_ALPHA32F_ARB     float         2        N     L A . .
      LUMINANCE_ALPHA8I_EXT      byte          2        N     L A . .
      LUMINANCE_ALPHA16I_EXT     short         2        N     L A . .
      LUMINANCE_ALPHA32I_EXT     int           2        N     L A . .
      LUMINANCE_ALPHA8UI_EXT     ubyte         2        N     L A . .
      LUMINANCE_ALPHA16UI_EXT    ushort        2        N     L A . .
      LUMINANCE_ALPHA32UI_EXT    uint          2        N     L A . .

      INTENSITY8                 ubyte         1        Y     I . . .
      INTENSITY16                ushort        1        Y     I . . .
      INTENSITY16F_ARB           half          1        N     I . . .
      INTENSITY32F_ARB           float         1        N     I . . .
      INTENSITY8I_EXT            byte          1        N     I . . .
      INTENSITY16I_EXT           short         1        N     A . . .
      INTENSITY32I_EXT           int           1        N     A . . .
      INTENSITY8UI_EXT           ubyte         1        N     A . . .
      INTENSITY16UI_EXT          ushort        1        N     A . . .
      INTENSITY32UI_EXT          uint          1        N     A . . .

      RGBA8                      ubyte         4        Y     R G B A
      RGBA16                     ushort        4        Y     R G B A
      RGBA16F_ARB                half          4        N     R G B A
      RGBA32F_ARB                float         4        N     R G B A
      RGBA8I_EXT                 byte          4        N     R G B A
      RGBA16I_EXT                short         4        N     R G B A
      RGBA32I_EXT                int           4        N     R G B A
      RGBA8UI_EXT                ubyte         4        N     R G B A
      RGBA16UI_EXT               ushort        4        N     R G B A
      RGBA32UI_EXT               uint          4        N     R G B A
*/

BABLIB_NAMESPACE_END
    
#endif

