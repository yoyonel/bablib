# ce script g�n�rique utilise le config.pri du r�pertoire courant pour cr�er une lib statique
# seul le nom de la lib est � red�finir (dans la variable CURRENT_LIB)

CURRENT_LIB  = bablib
PROJECT_PATH = $$PWD
include($$[QPROG]/create_lib.pri)

