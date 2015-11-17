# ce script générique utilise le config.pri du répertoire courant pour créer une lib statique
# seul le nom de la lib est à redéfinir (dans la variable CURRENT_LIB)

#DEFINES *= GLEW_STATIC      # pour construire lib statique
##DEFINES *= GLEW_BUILD      # pour construire lib dynamique

CURRENT_LIB = glew
PROJECT_PATH = $$PWD
include($$[QPROG]/create_lib.pri)

