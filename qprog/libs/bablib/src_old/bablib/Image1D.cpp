#include <OpenGL.h>
#include "Image1D.h"
#include "Message.h"
#include <QImageWriter>
BABLIB_NAMESPACE_USE

void AbstractImage1D::save(QString fileName) const {
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

