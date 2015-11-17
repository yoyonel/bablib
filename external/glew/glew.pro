# ce script gÃ©nÃ©rique utilise le config.pri du rÃ©pertoire courant pour crÃ©er une lib statique
# seul le nom de la lib est Ã  redÃ©finir (dans la variable CURRENT_LIB)

#DEFINES *= GLEW_STATIC      # pour construire lib statique
##DEFINES *= GLEW_BUILD      # pour construire lib dynamique

CURRENT_LIB = glew
PROJECT_PATH = $$PWD
include($$[QPROG]/create_lib.pri)

