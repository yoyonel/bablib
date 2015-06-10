# ce script fait les configurations necessaires pour utiliser la lib definie dans CURRENT_LIB
# > si la variable SOURCE_LIBS contient le nom de la lib (courante), elle est incluse sous
#   forme de sources au lieu de faire appel � la lib statique pr�compil�e
# > le repertoire racine de la lib doit �tre LIBS_PATH/CURRENT_LIB
#   il doit contenir un fichier config.pri d�finissant les variables LIB_HEADERS, LIB_SOURCES et LIB_INCLUDEPATH
# > autres variables qui doivent etre pr�c�demment d�finies :
#   - LIBS_PATH : racine des libs
#   - COMPILED_LIBS_PATH : lieu des libs statiques compil�es

isEmpty(CURRENT_LIB):error("la variable CURRENT_LIB doit etre definie dans le .prf (et doit contenir le nom de la lib a configurer)")
isEmpty(COMPILED_LIBS_PATH):error("la variable COMPILED_LIBS_PATH n'est pas d�finie")

# following variables must be erased before being written by lib's config.pri :
LIB_HEADERS =
LIB_SOURCES =
LIB_INCLUDEPATH =

#CURRENT_LIB_PATH = $$LIBS_PATH/$$CURRENT_LIB
include($$LIBS_PATH/$$CURRENT_LIB/config.pri)
INCLUDEPATH *= $$LIB_INCLUDEPATH

isEmpty(ONLY_LIBS_HEADERS) {
    # mode inclusion directe des sources :
    contains(SOURCE_LIBS, $$CURRENT_LIB) {
        HEADERS *= $$LIB_HEADERS
        SOURCES *= $$LIB_SOURCES
        }
    
    # mode utilisation de la lib statique pr�compil�e :
    !contains(SOURCE_LIBS, $$CURRENT_LIB) {
        LIBS            *= -L$$COMPILED_LIBS_PATH -l$${CURRENT_LIB}
        POST_TARGETDEPS *= $$COMPILED_LIBS_PATH/lib$${CURRENT_LIB}.a
        }
    
    ## 'dynamic' sources that should be recompiled for each project (e.g. if they depend on defines varying among projects)
    #HEADERS *= $$LIB_HEADERS_DYN
    #SOURCES *= $$LIB_SOURCES_DYN
    }

