# ce script générique utilise le config.pri du répertoire courant pour créer une lib statique
# seul le nom de la lib est à redéfinir (dans la variable CURRENT_LIB)

CURRENT_LIB = lib3ds
PROJECT_PATH = $$PWD
include($$[QPROG]/create_lib.pri)

