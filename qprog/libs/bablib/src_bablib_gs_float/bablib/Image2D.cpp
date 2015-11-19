#include "Image2D.h"
#include "Message.h"
#include <QImageWriter>
#include <png.h>
#include <stdlib.h>
BABLIB_NAMESPACE_USE

void AbstractImage2D::save(QString fileName) const {
    if (!loaded()) {
        Message::error("l'image ne contient pas de donnees");
        return;
        }
    
    // on deduit le format a utiliser a partir de l'extension du fichier :
    QByteArray format = fileName.section('.', -1).toLower().toLatin1();

    // on verifie d'abord que le format d'image est supporté :
    QList<QByteArray> supportedFormats = QImageWriter::supportedImageFormats();
    if (!supportedFormats.contains(format)) {
        Message::error(QString("le format '%1' n'est pas supporte").arg(QString(format)));
        QString formatsList;
        foreach (QByteArray f, supportedFormats)
            formatsList += QString(" ") + f;
        Message::info(QString("> formats supportés :") + formatsList);
        return;
        }
    
    // on écrit le fichier :
    QImage image = toQImage();
    if (!image.save(fileName, format))
        Message::error(QString("la sauvegarde de l'image '%1' a echoué").arg(fileName));
    }

namespace {
    typedef unsigned char byte;
    float int16_to_float32(const byte *data) {
        byte b0 = data[0];
        byte b1 = data[1];
        return (b0 << 8 | b1) / 65535.0f;
        }
    float int8_to_float32(const byte *data) {
        byte b0 = data[0];
        return b0 / 255.0f;
        }
    void float32_to_int16(float v, byte *data) {
        unsigned int vi = static_cast<unsigned int>(65535.0f * v);
        data[0] = (vi >> 8) & 0xFF;
        data[1] =  vi       & 0xFF;
        }
    }

float* AbstractImage2D::loadPNG16Data(QString fileName, int channels, int &w, int &h) {
    #undef ERROR
    #define ERROR(m,a) { Message::error(QString(m).arg(a)); if (file != NULL) fclose(file);  return NULL; }
    //@@ cleanup avant return..
    FILE *file = fopen(fileName.toLatin1(), "rb");
    if (file == NULL) ERROR("probleme lors du chargement du fichier '%1'", fileName);
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    //png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)user_error_ptr, user_error_fn, user_warning_fn);
    if (!png_ptr) ERROR("probleme avec un appel de libpng: <%1>", "png_create_read_struct")

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
        ERROR("probleme avec un appel de libpng: <%1>", "png_create_info_struct")
        }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
        ERROR("probleme avec un appel de libpng: <%1>", "png_create_info_struct, 2")
        }
    
    //@ set jmp?  or compile with PNG_SETJMP_NOT_SUPPORTED
    
    png_init_io(png_ptr, file);
    png_read_info(png_ptr, info_ptr);

    png_uint_32 width, height;
    int bit_depth;
    int color_type;
    png_get_IHDR(png_ptr, info_ptr,
        &width, &height,
        &bit_depth, &color_type,
        //&interlace_type, &compression_type, &filter_method);
        NULL, NULL, NULL);
    w = width;
    h = height;
    
    int inputChannels;
    switch (color_type) {
         case PNG_COLOR_TYPE_GRAY       : inputChannels = 1; break;
         case PNG_COLOR_TYPE_GRAY_ALPHA : inputChannels = 2; break;
         case PNG_COLOR_TYPE_RGB        : inputChannels = 3; break;
         case PNG_COLOR_TYPE_RGB_ALPHA  : inputChannels = 4; break;
         default: ERROR("format de couleurs non supportée : color_type=%1", color_type);
         }
    //@ possible de supporter PNG_COLOR_TYPE_PALETTE

    int nb = bit_depth == 16 ? 2 : 1;
    
    bool transfo = false;
    if (bit_depth < 8) {
        png_set_gray_1_2_4_to_8(png_ptr);
        transfo = true;
        }
    if (channels > 2 && inputChannels <= 2) {
        png_set_gray_to_rgb(png_ptr);
        inputChannels += 2;
        transfo = true;
        }
    if (channels <= 2 && inputChannels > 2) {
        png_set_rgb_to_gray(png_ptr, 1, -1.0, -1.0);  // error_action 1 ou 2
        inputChannels -= 2;
        transfo = true;
        }
    
    if (transfo)
        png_read_update_info(png_ptr, info_ptr);
    
    byte *rawdata = new byte[w * h * inputChannels * nb];
    png_bytep row_pointers[h];
    for (int j=0; j<h; j++)
        row_pointers[j] = (png_bytep)(rawdata + (h - 1 - j) * w * inputChannels * nb);   //@@ a voir : flip vertical
        //row_pointers[j] = (png_bytep)(rawdata + j * w * inputChannels * nb);
    png_read_image(png_ptr, row_pointers);
    png_read_end(png_ptr, end_info);
    
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    
    float *data = new float[w * h * channels];
    #define pix (data + j * w * channels + i * channels)
    #define pixraw(k) (rawdata + nb * (j * w * inputChannels + i * inputChannels + k))
    
    const bool alphaIn  = inputChannels % 2 == 0;
    const bool alphaOut = channels % 2 == 0;
    const int colorChannels = channels - (alphaOut ? 1 : 0);

    // la fonction de conversion 8bits/16bits --> float :
    float (*convert)(const byte *) = (nb == 1) ? &int8_to_float32 : &int16_to_float32;
    
    for (int i=0; i<w; i++) for (int j=0; j<h; j++) {
        if (alphaOut)
            pix[channels-1] = alphaIn ? convert(pixraw(inputChannels-1)) : 1.0f;
        for (int c=0; c<colorChannels; c++)
            pix[c] = convert(pixraw(c));
        }
    fclose(file);   //@ gestion erreurs
    delete[] rawdata;
    return data;
    }


void AbstractImage2D::savePNG16Data(QString fileName, int channels, int w, int h, const float *data) {
    #undef ERROR
    #define ERROR(m,a) { Message::error(QString(m).arg(a)); if (file != NULL) fclose(file); return; }
    //@@ cleanup avant return..
    FILE *file = fopen(fileName.toLatin1(), "wb");
    if (file == NULL) ERROR("probleme lors de l'écriture du fichier '%1'", fileName);
    
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    //png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, (png_voidp)user_error_ptr, user_error_fn, user_warning_fn);
    if (!png_ptr) ERROR("probleme avec un appel de libpng: <%1>", "png_create_write_struct")

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        ERROR("probleme avec un appel de libpng: <%1>", "png_create_info_struct")
        }

    //@ set jmp?  or compile with PNG_SETJMP_NOT_SUPPORTED
    
    png_init_io(png_ptr, file);
    
    const png_uint_32 width = w, height = h;
    const int bit_depth = 16;
    int color_type;
    switch (channels) {
         case 1: color_type = PNG_COLOR_TYPE_GRAY       ; break;
         case 2: color_type = PNG_COLOR_TYPE_GRAY_ALPHA ; break;
         case 3: color_type = PNG_COLOR_TYPE_RGB        ; break;
         case 4: color_type = PNG_COLOR_TYPE_RGB_ALPHA  ; break;
         default: ERROR("nombre de canaux non supporté : channels=%1", channels);
         }
       
    png_set_IHDR(png_ptr, info_ptr,
        width, height,
        bit_depth, color_type,
        PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    const int nb = 2;   // nombre d'octets = 2 pour 16 bits
    png_bytep row_pointers[h];
    byte *rawdata = new byte[w * h * channels * nb];
    for (int j=0; j<h; j++) {
        row_pointers[j] = (png_bytep)(rawdata + j * w * channels * nb);
        for (int i=0; i<w; i++)
            for (int c=0; c<channels; c++)
                float32_to_int16(data[((h - 1 - j) * w + i) * channels + c],        //@@ a voir : flip vertical
                                 rawdata + ((j * w + i) * channels + c) * nb);
        }
    
    /*
    png_set_rows(png_ptr, info_ptr, row_pointers);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    */

    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, row_pointers);

    //png_write_end(png_ptr, info_ptr);
    png_write_end(png_ptr, NULL);
    
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    fclose(file);   //@ gestion erreurs
    delete[] rawdata;
    }

