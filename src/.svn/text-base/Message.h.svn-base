#ifndef __MESSAGE__
#define __MESSAGE__

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "MacroTools.h"
#include <QTextStream>
#include <QString>

BABLIB_NAMESPACE_BEGIN

class Message {
    public:
        static void error(QString   message, bool prefix = true);
        static void error(const char*     message, bool prefix = true);
        static void warning(QString message, bool prefix = true);
        static void warning(const char*   message, bool prefix = true);
        static void info(QString    message, bool prefix = false);
        static void info(const char*      message, bool prefix = false);
        static void debug(QString   message, bool prefix = true);
        static void debug(const char*     message, bool prefix = true);
        
        static bool checkGLErrors(QString location = QString::null);
        
    private:
        static QTextStream
            errorStream,
            warningStream,
            infoStream,
            debugStream;
        static const QString
            ERROR_PREFIX,
            WARNING_PREFIX,
            INFO_PREFIX,
            DEBUG_PREFIX;
    };


namespace {
    template<class Object>
    QString convToQS(const Object &obj) { return QString("%1").arg(obj); }
    
    #define CONV_TO_QS_SPECIALIZE(Type) \
        template<> QString convToQS<Type>(const Type &obj) { return obj.toQString(); }
        
    CONV_TO_QS_SPECIALIZE(Vec2)
    CONV_TO_QS_SPECIALIZE(Vec3)
    CONV_TO_QS_SPECIALIZE(Vec4)
    
    //template<> QString convToQS<Vec3>(const Vec3 &obj) { return obj.toQString(); }
    }

#define MSG_DEBUG_VAR(v) Message::debug(QString(#v " = %1").arg(convToQS(v)))
#define MSG_DEBUG_POINT Message::debug(QString("file %1 line %2").arg(__FILE__).arg(__LINE__));

#define MSG_CHECK_GL Message::checkGLErrors(QString("file %1 line %2").arg(__FILE__).arg(__LINE__));
#define MSG_CHECK_GL_EXIT if (!Message::checkGLErrors(QString("file %1 line %2").arg(__FILE__).arg(__LINE__))) exit(1);

////////////////////

#define MSG_FUNC_TOARG(r,data,t)    .arg(convToQS(t))
#define MSG_FUNC_EXPAND_ARGS(seq)   BOOST_PP_SEQ_FOR_EACH(MSG_FUNC_TOARG, ~, seq)

// macros to simplify insertion of arguments in printed strings :
//  > ex: MSG_INFO("a=%1, b=%2", a+b=%3, (a)(b)(a+b))
#define MSG_ERROR(  message, params_seq)   Message::error(QString(message)   MSG_FUNC_EXPAND_ARGS(params_seq));
#define MSG_DEBUG(  message, params_seq)   Message::debug(QString(message)   MSG_FUNC_EXPAND_ARGS(params_seq));
#define MSG_INFO(   message, params_seq)   Message::info(QString(message)    MSG_FUNC_EXPAND_ARGS(params_seq));
#define MSG_WARNING(message, params_seq)   Message::warning(QString(message) MSG_FUNC_EXPAND_ARGS(params_seq));


BABLIB_NAMESPACE_END

#endif

