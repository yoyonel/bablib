# ce script gÃÂ©nÃÂ©rique utilise le config.pri du rÃÂ©pertoire courant pour crÃÂ©er une lib statique
# seul le nom de la lib est ÃÂ  redÃÂ©finir (dans la variable CURRENT_LIB)

CURRENT_LIB = lib3ds
PROJECT_PATH = $$PWD
include($$[QPROG]/create_lib.pri)

