#ifndef __ENUM_HELPER__
#define __ENUM_HELPER__

#include "MacroTools.h"
#include "Message.h"
#include <QString>
#include <QStringList>
#include <QInputDialog>

// intermediate macros for ENUM_HELPER
#define ENUM_HELPER__EXPAND_COMMA_LIST(r,data,t) t,
#define ENUM_HELPER__EXPAND_ADD(r,data,t) << t
#define ENUM_HELPER__EXPAND_ADD_QSTRING(r,data,t)   << BOOST_PP_STRINGIZE(t)
#define ENUM_HELPER__EXPAND_CASE(r,data,t)          case t : return QString(BOOST_PP_STRINGIZE(t));
#define ENUM_HELPER__EXPAND_TEST_VALUE(r,data,t)    if (data == QString(BOOST_PP_STRINGIZE(t))) return t;

// macro to define an enum with an associated helper class allowing easy conversions between names and values
// example : ENUM_HELPER(SomeEnum, (ABC) (DEF) (GHI))
//    > defines :
//        enum SomeEnum { ABC, DEF, GHI };
//        struct SomeEnumHelper { ... useful static methods ... };

#define ENUM_HELPER(enum_name, enum_seq)                                                                        \
    enum enum_name { BOOST_PP_SEQ_FOR_EACH(ENUM_HELPER__EXPAND_COMMA_LIST, ~, enum_seq) };                      \
    struct enum_name##Helper {                                                                                  \
        static QString convE2S(enum_name t) {                                                                   \
            switch (t) { BOOST_PP_SEQ_FOR_EACH(ENUM_HELPER__EXPAND_CASE, ~, enum_seq) };                        \
            MSG_ERROR("value %1 does not belong to enum %2", (t)(#enum_name))                                   \
            return "<undefined enum value>";                                                                    \
            }                                                                                                   \
                                                                                                                \
        static enum_name convS2E(QString s) {                                                                   \
            BOOST_PP_SEQ_FOR_EACH(ENUM_HELPER__EXPAND_TEST_VALUE, s, enum_seq)                                  \
            MSG_ERROR("value %1 does not belong to enum %2", (s)(#enum_name))                                   \
            return BOOST_PP_SEQ_ELEM(0, enum_seq);                                                              \
            }                                                                                                   \
                                                                                                                \
        static enum_name select(enum_name def, QString text = "Select a value:") {                              \
            const QStringList names = namesList();                                                              \
            bool choiceDone;                                                                                    \
            QString s = QInputDialog::getItem(NULL, #enum_name, text, names, def, false, &choiceDone);          \
            return choiceDone ? convS2E(s) : def;                                                               \
            }                                                                                                   \
                                                                                                                \
        static QStringList namesList() {                                                                        \
            return QStringList() BOOST_PP_SEQ_FOR_EACH(ENUM_HELPER__EXPAND_ADD_QSTRING, ~, enum_seq);           \
            }                                                                                                   \
                                                                                                                \
        static QList<enum_name> valuesList() {                                                                  \
            return QList<enum_name>() BOOST_PP_SEQ_FOR_EACH(ENUM_HELPER__EXPAND_ADD, ~, enum_seq);              \
            }                                                                                                   \
        };
    
//@ todo:
// - allow association of arbitrary integer values with enums
// - select : option for lower case names

#endif

