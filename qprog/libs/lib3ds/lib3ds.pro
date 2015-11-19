# ce script gÃ©nÃ©rique utilise le config.pri du rÃ©pertoire courant pour crÃ©er une lib statique
# seul le nom de la lib est Ã  redÃ©finir (dans la variable CURRENT_LIB)

CURRENT_LIB = lib3ds
PROJECT_PATH = $$PWD
include($$[QPROG]/create_lib.pri)

