# adds PROJ4 testing
# authors: Nicolas DAVID and Bruno VALLET
# date: 23/10/2009

if(UNIX)
	if(BUILD_CS2CS)
		add_test(testIGNF ${PROJ4_ROOT_DIR}/nad/testIGNF.sh ${PROJECT_BINARY_DIR}/bin/cs2cs ${PROJ4_ROOT_DIR}/nad
			${PROJ4_ROOT_DIR}/nad/proj_outIGNF ${PROJ4_ROOT_DIR}/nad/IGNF)
		add_test(testGEOID ${PROJ4_ROOT_DIR}/nad/testGEOID.sh ${PROJECT_BINARY_DIR}/bin/cs2cs ${PROJ4_ROOT_DIR}/nad
			${PROJ4_ROOT_DIR}/nad/proj_qg_outIGNF ${PROJ4_ROOT_DIR}/nad/IGNF)
	else(BUILD_CS2CS)
		message("warning: PROJ4 tests require building cs2cs")
	endif(BUILD_CS2CS)
else(UNIX)
	message("warning: PROJ4 tests require a UNIX system")
endif(UNIX)