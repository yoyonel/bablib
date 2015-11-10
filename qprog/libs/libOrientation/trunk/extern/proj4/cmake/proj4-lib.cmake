# adds proj4 library targets
# authors: Nicolas DAVID and Bruno VALLET
# date: 23/10/2009

##############################################
### CHECK SYSTEM CONFIG                    ###
##############################################
INCLUDE (CheckIncludeFiles)
INCLUDE (CheckLibraryExists) 

CHECK_INCLUDE_FILES (dlfcn.h HAVE_DLFCN_H)
CHECK_INCLUDE_FILES (inttypes.h HAVE_INTTYPES_H)
CHECK_INCLUDE_FILES (jni.h HAVE_JNI_H)
CHECK_LIBRARY_EXISTS(m ceil "" HAVE_LIBM) 
CHECK_INCLUDE_FILES (memory.h HAVE_MEMORY_H)
CHECK_INCLUDE_FILES (stdint.h HAVE_STDINT_H)
CHECK_INCLUDE_FILES (stdlib.h HAVE_STDLIB_H)
CHECK_INCLUDE_FILES (string.h HAVE_STRING_H)
CHECK_INCLUDE_FILES (sys/stat.h HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILES (sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILES (unistd.h HAVE_UNISTD_H)
CHECK_INCLUDE_FILES("stdlib.h;stdarg.h;string.h;float.h" STDC_HEADERS)

get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
configure_file(${SELF_DIR}/libproj4/proj_config.h.in ${proj4_ROOT_DIR}/src/proj_config.h)

# define source and header files and directories
set(proj4_SRCS
PJ_aea.c 
PJ_aeqd.c 
PJ_airy.c 
PJ_aitoff.c 
PJ_august.c
PJ_bacon.c 
PJ_bipc.c 
PJ_boggs.c 
PJ_bonne.c 
PJ_cass.c 
PJ_cc.c 
PJ_cea.c 
PJ_chamb.c
PJ_collg.c 
PJ_crast.c 
PJ_denoy.c 
PJ_eck1.c 
PJ_eck2.c 
PJ_eck3.c 
PJ_eck4.c 
PJ_eck5.c
PJ_eqc.c
PJ_eqdc.c
PJ_fahey.c
PJ_fouc_s.c
PJ_gall.c
PJ_geos.c
PJ_gins8.c
PJ_glabsgm.c
PJ_gn_sinu.c
PJ_gnom.c
PJ_goode.c
PJ_hammer.c
PJ_hatano.c
PJ_imw_p.c
PJ_krovak.c
PJ_labrd.c
PJ_laea.c
PJ_lagrng.c
PJ_larr.c
PJ_lask.c
PJ_lcc.c
PJ_lcca.c
PJ_loxim.c
PJ_lsat.c
PJ_mbt_fps.c
PJ_mbtfpp.c
PJ_mbtfpq.c
PJ_merc.c
PJ_mill.c
PJ_mod_ster.c
PJ_moll.c
PJ_nell.c
PJ_nell_h.c
PJ_nocol.c
PJ_nsper.c
PJ_nzmg.c
PJ_ob_tran.c
PJ_ocea.c
PJ_oea.c
PJ_omerc.c 
PJ_ortho.c
PJ_poly.c
PJ_putp2.c
PJ_putp3.c
PJ_putp4p.c
PJ_putp5.c
PJ_putp6.c
PJ_robin.c
PJ_rpoly.c
PJ_sconics.c
PJ_somerc.c
PJ_stere.c 
PJ_sterea.c
PJ_sts.c
PJ_tcc.c
PJ_tcea.c
PJ_tmerc.c
PJ_tpeqd.c
PJ_urm5.c
PJ_urmfps.c
PJ_vandg.c
PJ_vandg2.c
PJ_vandg4.c
PJ_wag2.c
PJ_wag3.c
PJ_wag7.c
PJ_wink1.c
PJ_wink2.c 
aasincos.c
adjlon.c 
bch2bps.c 
bchgen.c
biveval.c
dmstor.c 
emess.c 
geocent.c 
geocent.h 
geoid_cvt.c 
geoid_init.c
geoid_intr.c
jniproj.c 
mk_cheby.c 
nad_cvt.c 
nad_init.c 
nad_intr.c
pj_apply_gridshift.c
pj_auth.c 
pj_datum_set.c 
pj_datums.c 
pj_deriv.c 
pj_ell_set.c 
pj_ellps.c
pj_errno.c 
pj_factors.c 
pj_fwd.c 
pj_gauss.c 
pj_geocent.c 
pj_gridinfo.c
pj_gridlist.c
pj_init.c 
pj_init.h 
pj_inv.c 
pj_latlong.c 
pj_list.c 
pj_malloc.c
pj_mlfn.c 
pj_msfn.c 
pj_open_lib.c 
pj_param.c 
pj_phi2.c 
pj_pr_list.c
pj_qsfn.c 
pj_release.c 
pj_strerrno.c 
pj_transform.c 
pj_tsfn.c 
pj_units.c
pj_utils.c 
pj_zpoly1.c 
proj_mdist.c 
proj_rouss.c 
qg_gridlist.c
qg_grindinfo.c 
rtodms.c 
vd_apply_gridshift.c 
vector1.c 
vertical_datum_set.c
)
if(WIN32)
	message(STATUS " Trying to build proj4 DLL" )
	set(proj4_SRCS ${proj4_SRCS} proj.def)
endif(WIN32)
__global_append(proj4_SRCS proj4_FULL_SRCS "${proj4_ROOT_DIR}/src/")

include_directories(${proj4_ROOT_DIR}/src)
SET(proj4_HEADERS
pj_list.h
emess.h
projects.h
nad_list.h
proj_api.h
)
__global_append(proj4_HEADERS proj4_FULL_HEADERS "${proj4_ROOT_DIR}/src/")

# set definitions and dependencies
set(proj4_DEFINITIONS "")
set(proj4_DEPENDENCIES "")

# call the macro from Extern/cmake/CommonMacros.cmake that cleanfully adds a library
__add_library (proj4 proj4_FULL_SRCS proj4_FULL_HEADERS proj4_DEFINITIONS proj4_DEPENDENCIES)

set_target_properties(proj4 PROPERTIES LINKER_LANGUAGE C)
#add_dependencies(proj4 ${proj4_ROOT_DIR}/src/proj_config.h)

