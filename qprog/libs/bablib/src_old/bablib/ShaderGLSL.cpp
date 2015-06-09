#include "ShaderGLSL.h"
#include "Message.h"
#include <QFile>
#include <QTextStream>
BABLIB_NAMESPACE_USE

ShaderGLSL::ShaderGLSL(QString source, Type type, bool loadFromFile, QString name) : fileName(loadFromFile ? source : name) {
    QString sourceCode;
    if (loadFromFile) {
        QFile file(source);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            Message::error(QString("impossible d'ouvrir le fichier '%1'").arg(source));
            return;
            }
    
        sourceCode = QTextStream(&file).readAll();
        file.close();
        }
    else
        sourceCode = source;
    
    QByteArray array = sourceCode.toLatin1();
    const char *src = array.constData();
    
    // chargement du source :
    GLenum typeEnum = (type == VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
    id = glCreateShader(typeEnum);
    glShaderSource(id, 1, &src, NULL);
    }

void ShaderGLSL::destroy() { glDeleteShader(id); }

void ShaderGLSL::compile() {
    glCompileShader(id);
    
    int ok;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (ok == GL_FALSE)
        Message::error(QString("- erreur de compilation du shader '%1' :\n%2").arg(fileName).arg(getInfoLog()), false);
    }

QString ShaderGLSL::getInfoLog() const {
    int logLength;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
    char *infoLog = new char[logLength];
    glGetShaderInfoLog(id, logLength, NULL, infoLog);
    QString res(infoLog);
    delete infoLog;
    return res;
    }

