#include <OpenGL.h>
#include <GL/glu.h>
#include "Message.h"
//#include <GL/glprocs.h>
BABLIB_NAMESPACE_USE

QTextStream Message::errorStream(stderr);
QTextStream Message::warningStream(stdout);
QTextStream Message::infoStream(stdout);
QTextStream Message::debugStream(stdout);

const QString Message::ERROR_PREFIX   = "- erreur : ";
const QString Message::WARNING_PREFIX = "- attention : ";
const QString Message::INFO_PREFIX    = "- info : ";
const QString Message::DEBUG_PREFIX   = "- debug : ";

void Message::error(QString message   , bool prefix) { errorStream << (prefix ? ERROR_PREFIX   : "") << message << (prefix ? "." : "") << endl; }
void Message::error(const char* message     , bool prefix) { errorStream << (prefix ? ERROR_PREFIX   : "") << message << (prefix ? "." : "") << endl; }
void Message::warning(QString message , bool prefix) { errorStream << (prefix ? WARNING_PREFIX : "") << message << (prefix ? "." : "") << endl; }
void Message::warning(const char* message   , bool prefix) { errorStream << (prefix ? WARNING_PREFIX : "") << message << (prefix ? "." : "") << endl; }
void Message::info(QString message    , bool prefix) { infoStream  << (prefix ? INFO_PREFIX    : "") << message << (prefix ? "." : "") << endl; }
void Message::info(const char* message      , bool prefix) { infoStream  << (prefix ? INFO_PREFIX    : "") << message << (prefix ? "." : "") << endl; }
void Message::debug(QString message   , bool prefix) { debugStream << (prefix ? DEBUG_PREFIX   : "") << message << (prefix ? "." : "") << endl; }
void Message::debug(const char* message     , bool prefix) { debugStream << (prefix ? DEBUG_PREFIX   : "") << message << (prefix ? "." : "") << endl; }

/*
namespace {
    bool glCheckErrors(const char* location) {
       bool ok = true;
       GLuint errnum;
       const char *errstr;
       while ((errnum = glGetError())) {
           ok = false;
           errstr = reinterpret_cast<const char *>(gluErrorString(errnum));
           Message::error(QString("GL [ %1 ] : %2 (%3)").arg(location).arg(errstr).arg(errnum));
           }
       return ok;
       }
    }
*/
bool Message::checkGLErrors(QString location) {
    bool ok = true;
    GLuint errnum;
    const char *errstr;
    while ((errnum = glGetError())) {
        ok = false;
        errstr = reinterpret_cast<const char *>(gluErrorString(errnum));
        Message::error(QString("GL [ %1 ] : %2 (%3)").arg(location).arg(errstr).arg(errnum));
        }
    return ok;
    }


