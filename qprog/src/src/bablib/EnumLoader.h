#ifndef __ENUM_LOADER__
#define __ENUM_LOADER__


/////////////////////////////////////////
//        exemple d'utilisation        //
/////////////////////////////////////////
// fichier "Enums.h" :
// -------------------
//     #ifndef __ENUMS__
//     #define __ENUMS__
//     #include <EnumLoader.h>
//     DEFINE_ENUM(
//         MonEnum,
//         VAL_DEF, 4,
//         VAL1, VAL2, VAL3, VAL_DEF)
//     #endif
// 
// fichier "Enums.cpp" :
// ---------------------
//     #define __ENUM_LOADER_CPP__
//     #include "Enums.h"


#include "Message.h"
#include <QString>
#include <QStringList>
#include <QInputDialog>

// cette macro sert ÃÂ  dÃÂ©finir une enum <EnumName> et une classe <EnumName>Loader qui fournit des mÃÂ©thodes
// permettant de faire la conversion entre une enum et sa reprÃÂ©sentation en chaÃÂ®ne de caractÃÂ¨res.
// ne pas oublier dans le fichier cpp ÃÂ  faire un #define __ENUM_LOADER_CPP__ pour l'implÃÂ©mentation de la classe.
//------------------------
// arguments de la macro :
//------------------------
//  - nom de l'enum
//  - valeur par dÃÂ©faut
//  - nombre de valeurs
//  - liste des enums
#define DEFINE_ENUM(EnumName, defaultValue, n, enums...) \
    enum EnumName { enums }; \
    DEF_ENUM_LOADER(EnumName, defaultValue) \
    DEF_ENUM_STRING_LIST(EnumName, n, enums)

// macro intermÃÂ©diaire :
// dÃÂ©finition de la classe <EnumName>Loader :
#define DEF_ENUM_LOADER(EnumName, defaultValue)    \
    class EnumName##Loader { \
        public: \
            static EnumName select(EnumName def = defaultValue) {   \
                bool choiceDone;    \
                QString s = QInputDialog::getItem(NULL, #EnumName, "Mode:", enumNames, def, false, &choiceDone);  \
                return choiceDone ? get(s) : def;  \
                }   \
            static EnumName get(QString enumString) {   \
                const int i = enumNames.indexOf(enumString.toLower());  \
                if (i >= 0) return static_cast<EnumName>(i);            \
                else {  \
                    BABLIB_NAMESPACE_NAME(Message)::error(QString("la valeur '%1' ne fait pas partie de l'enum '%2'").arg(enumString).arg(#EnumName));   \
                    return defaultValue;    \
                    } \
                }   \
        private:    \
            static const QStringList enumNames; \
        };

    //return (i >= 0) ? static_cast<EnumName>(i) : defaultValue;
    
// macro intermÃÂ©diaire :
// dÃÂ©finition ÃÂ©ventuelle (si on se trouve dans le cpp) de la liste de noms de l'enum :
#ifdef __ENUM_LOADER_CPP__
    #define PROCESS_STRING(a) << QString(#a).toLower()
    #define DEF_ENUM_STRING_LIST(EnumName, n, enums...) \
        const QStringList EnumName##Loader::enumNames(QStringList() PROCESS_LIST##n(PROCESS_STRING, enums));
#else
    #define DEF_ENUM_STRING_LIST(args...)
#endif


// macros pour appliquer un traitement sur une liste d'arguments :
#define PROCESS_LIST1(process, a0) process(a0)
#define PROCESS_LIST2(process, a0, a1) process(a0) process(a1)
#define PROCESS_LIST3(process, a0, args...) process(a0) PROCESS_LIST2(process, args)
#define PROCESS_LIST4(process, a0, args...) process(a0) PROCESS_LIST3(process, args)
#define PROCESS_LIST5(process, a0, args...) process(a0) PROCESS_LIST4(process, args)
#define PROCESS_LIST6(process, a0, args...) process(a0) PROCESS_LIST5(process, args)
#define PROCESS_LIST7(process, a0, args...) process(a0) PROCESS_LIST6(process, args)
#define PROCESS_LIST8(process, a0, args...) process(a0) PROCESS_LIST7(process, args)
#define PROCESS_LIST9(process, a0, args...) process(a0) PROCESS_LIST8(process, args)
#define PROCESS_LIST10(process, a0, args...) process(a0) PROCESS_LIST9(process, args)
#define PROCESS_LIST11(process, a0, args...) process(a0) PROCESS_LIST10(process, args)
#define PROCESS_LIST12(process, a0, args...) process(a0) PROCESS_LIST11(process, args)
#define PROCESS_LIST13(process, a0, args...) process(a0) PROCESS_LIST12(process, args)
#define PROCESS_LIST14(process, a0, args...) process(a0) PROCESS_LIST13(process, args)
#define PROCESS_LIST15(process, a0, args...) process(a0) PROCESS_LIST14(process, args)
#define PROCESS_LIST16(process, a0, args...) process(a0) PROCESS_LIST15(process, args)
#define PROCESS_LIST17(process, a0, args...) process(a0) PROCESS_LIST16(process, args)
#define PROCESS_LIST18(process, a0, args...) process(a0) PROCESS_LIST17(process, args)
#define PROCESS_LIST19(process, a0, args...) process(a0) PROCESS_LIST18(process, args)
#define PROCESS_LIST20(process, a0, args...) process(a0) PROCESS_LIST19(process, args)


#endif
