#include <OpenGL.h>
#include <GL/glu.h>
#include "Message.h"
//#include <GL/glprocs.h>
BABLIB_NAMESPACE_USE

void Message::init() {
    static bool done = false;
    if (done) return;
    done = true;
    Message::errorStream   = new QTextStream(stderr);
    Message::warningStream = new QTextStream(stdout);
    Message::infoStream    = new QTextStream(stdout);
    Message::debugStream   = new QTextStream(stdout);
    }
        
QTextStream* Message::errorStream;
QTextStream* Message::warningStream;
QTextStream* Message::infoStream;
QTextStream* Message::debugStream;
/*
QTextStream Message::errorStream(stderr);
QTextStream Message::warningStream(stdout);
QTextStream Message::infoStream(stdout);
QTextStream Message::debugStream(stdout);
*/

//const QString Message::ERROR_PREFIX   = "- erreur : ";
//const QString Message::WARNING_PREFIX = "- attention : ";
//const QString Message::INFO_PREFIX    = "- info : ";
//const QString Message::DEBUG_PREFIX   = "- debug : ";
#define ERROR_PREFIX   "- erreur : "
#define WARNING_PREFIX "- attention : "
#define INFO_PREFIX    "- info : "
#define DEBUG_PREFIX   "- debug : "

void Message::error(QString         message, bool prefix) { init(); *errorStream << (prefix ? ERROR_PREFIX   : "") << message << (prefix ? "." : "") << endl; }
void Message::error(const char*     message, bool prefix) { init(); *errorStream << (prefix ? ERROR_PREFIX   : "") << message << (prefix ? "." : "") << endl; }
void Message::warning(QString       message, bool prefix) { init(); *errorStream << (prefix ? WARNING_PREFIX : "") << message << (prefix ? "." : "") << endl; }
void Message::warning(const char*   message, bool prefix) { init(); *errorStream << (prefix ? WARNING_PREFIX : "") << message << (prefix ? "." : "") << endl; }
void Message::info(QString          message, bool prefix) { init(); *infoStream  << (prefix ? INFO_PREFIX    : "") << message << (prefix ? "." : "") << endl; }
void Message::info(const char*      message, bool prefix) { init(); *infoStream  << (prefix ? INFO_PREFIX    : "") << message << (prefix ? "." : "") << endl; }
void Message::debug(QString         message, bool prefix) { init(); *debugStream << (prefix ? DEBUG_PREFIX   : "") << message << (prefix ? "." : "") << endl; }
void Message::debug(const char*     message, bool prefix) { init(); *debugStream << (prefix ? DEBUG_PREFIX   : "") << message << (prefix ? "." : "") << endl; }

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


