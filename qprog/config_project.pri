# ce fichier est a inclure dans le .pro de chaque projet
# > la variable PROJECT_PATH doit Ãªtre dÃ©finie dans le .pro pour contenir le chemin vers la racine du projet (dans le .pro : PROJECT_PATH = $$PWD)
# > la variable QMAKEFEATURES doit etre initialisee
#     - soit comme variable d'environnement
#     - soit comme 'property variable' de Qt en faisant (une seule fois) qmake -set QMAKEFEATURES <repertoire>
# > optionnel : la variable BUILD_DIR pour prÃ©ciser le rÃ©pertoire pour les fichiers intermÃ©diaires de build (sous-rÃ©pertoire de QPROG_BUILD_PATH)

isEmpty(PROJECT_PATH):\
    error("la variable PROJECT_PATH doit dÃ©finir le chemin vers la racine du projet")

# par defaut sous windows, CONFIG contient le mode "debug_and_release", donc on l'enleve :
CONFIG -= debug_and_release

#################################

#@@@@ reverifier..
win32 {
    contains(CONFIG, bablib)
        SOURCE_LIBS *= bablib scene3d glew      #@ mieux : mettre Ã§a dans qtfeatures/lib_config.pri --> seulement rÃ©p. qtfeatures qui est platform-dependent
    CONFIG *= warn_on thread console
    
    # force 'release' mode because of weird problems with debug mode on windows
    CONFIG -= debug
    CONFIG *= release
    # a voir : ne pas imposer debug/config ici..
    # --> utiliser un variable DEBUG_MODE par exemple ..
    }

#################################
# config des differents chemins #
#################################
    
    ROOT_PATH = $$PWD
    LIBS_PATH = $$ROOT_PATH/libs            # repertoire oÃ¹ sont placÃ©es les libs utilisÃ©es
    COMPILED_LIBS_PATH = $$ROOT_PATH/bin    # rÃ©pertoire contenant les version compilÃ©es des libs

    # definition of potientially customized paths :
    exists(config/paths_local.pri) {
        include(config/paths_local.pri)
        }
    else {
        include(config/paths.pri)
        }
    

##################################
# definition de fonctions utiles #
##################################

    # remplace les caracteres '\' et '/' par des underscores
    # arg1 = chaine a transformer
    defineReplace(pathToString) {
        s = $$replace(1, $$quote(/) , $$quote(_))
        s = $$replace(s, $$quote(\) , $$quote(_))
        s = $$replace(s, $$quote(:) , $$quote(_))
        return ($$s)
        }
    
    # arg1 = chaine a enlever
    # arg2 = chaine a laquelle enlever <arg1>
    defineReplace(remove) {
        return ($$replace(2, $$1, $$quote()))
        }
    
    # verifie que le path existe et essaye de le crÃ©er dans le cas contraire
    # arg1 = path a verifier
    defineTest(checkPath) {
        !exists($$1) {
            message(creation du repertoire $$1)
			 win32:dir = $$replace(1, $$quote(/), $$quote(\))
            !win32:dir = $$1
            = $$system(mkdir $$dir)
            !exists($$dir) { error(impossible de creer le repertoire $$1) }
            }
        }
    
####################################################################
# configuration des repertoires pour la compilation (obj, moc, ui) #
####################################################################
    
    # definition du repertoire dans lequel le projet doit compiler
    isEmpty(BUILD_DIR) {
        # on prend le nom du dernier repertoire qui mÃ¨ne Ã  la racine du projet :
        #ROOT_STRING = $$pathToString($$PROG_PATH)_
        #PROJ_STRING = $$pathToString($$PROJECT_PATH)
        #BUILD_DIR = $$remove($$ROOT_STRING, $$PROJ_STRING)
        BUILD_DIR = $$pathToString($$PROJECT_PATH)
        }
        
    BUILD_PATH = $$QPROG_BUILD_PATH/$$BUILD_DIR
    
    # definition des sous-repertoires de compil
    OBJECTS_DIR = $$BUILD_PATH/obj
    MOC_DIR     = $$BUILD_PATH/moc
    UI_DIR      = $$BUILD_PATH/ui
    
    # verification que les repertoires existent
    checkPath($$BUILD_PATH)
    checkPath($$OBJECTS_DIR)
    checkPath($$MOC_DIR)
    checkPath($$UI_DIR)

