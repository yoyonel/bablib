bin_PROGRAMS = 	proj cs2cs geod geoid2geoid nad2bin nad2nad proj

INCLUDES =	-DPROJ_LIB=\"$(pkgdatadir)\" @JNI_INCLUDE@

include_HEADERS = projects.h nad_list.h proj_api.h org_proj4_Projections.h

EXTRA_DIST = makefile.vc proj.def

proj_SOURCES = proj.c gen_cheb.c p_series.c
cs2cs_SOURCES = cs2cs.c gen_cheb.c p_series.c
nad2nad_SOURCES = nad2nad.c 
nad2bin_SOURCES = nad2bin.c
geod_SOURCES = geod.c geod_set.c geod_for.c geod_inv.c geodesic.h

proj_LDADD = libproj.la
cs2cs_LDADD = libproj.la
nad2nad_LDADD = libproj.la
nad2bin_LDADD = libproj.la
geod_LDADD = libproj.la

lib_LTLIBRARIES = libproj.la

libproj_la_LDFLAGS = -version-info 5:4:5

libproj_la_SOURCES = PJ_aea.c PJ_aeqd.c PJ_airy.c PJ_aitoff.c PJ_august.c \
	PJ_bacon.c PJ_bipc.c PJ_boggs.c PJ_bonne.c PJ_cass.c PJ_cc.c PJ_cea.c PJ_chamb.c \
	PJ_collg.c PJ_crast.c PJ_denoy.c PJ_eck1.c PJ_eck2.c PJ_eck3.c PJ_eck4.c PJ_eck5.c \
	PJ_eqc.c PJ_eqdc.c PJ_fahey.c PJ_fouc_s.c PJ_gall.c PJ_geos.c PJ_gins8.c \
	PJ_glabsgm.c PJ_gn_sinu.c PJ_gnom.c PJ_goode.c PJ_hammer.c PJ_hatano.c PJ_imw_p.c \
	PJ_krovak.c PJ_labrd.c PJ_laea.c PJ_lagrng.c PJ_larr.c PJ_lask.c PJ_lcc.c PJ_lcca.c \
	PJ_loxim.c PJ_lsat.c PJ_mbt_fps.c PJ_mbtfpp.c PJ_mbtfpq.c PJ_merc.c PJ_mill.c \
	PJ_mod_ster.c PJ_moll.c PJ_nell.c PJ_nell_h.c PJ_nocol.c PJ_nsper.c PJ_nzmg.c \
	PJ_ob_tran.c PJ_ocea.c PJ_oea.c PJ_omerc.c PJ_ortho.c PJ_poly.c PJ_putp2.c \
	PJ_putp3.c PJ_putp4p.c PJ_putp5.c PJ_putp6.c PJ_robin.c PJ_rpoly.c PJ_sconics.c \
	PJ_somerc.c PJ_stere.c PJ_sterea.c PJ_sts.c PJ_tcc.c PJ_tcea.c PJ_tmerc.c \
	PJ_tpeqd.c PJ_urm5.c PJ_urmfps.c PJ_vandg.c PJ_vandg2.c PJ_vandg4.c PJ_wag2.c \
	PJ_wag3.c PJ_wag7.c PJ_wink1.c PJ_wink2.c aasincos.c adjlon.c bch2bps.c bchgen.c \
	biveval.c dmstor.c emess.c emess.h geocent.c geocent.h geoid_cvt.c geoid_init.c \
	geoid_intr.c jniproj.c mk_cheby.c nad_cvt.c nad_init.c nad_intr.c \
	pj_apply_gridshift.c pj_auth.c pj_datum_set.c pj_datums.c pj_deriv.c pj_ell_set.c pj_ellps.c \
	pj_errno.c pj_factors.c pj_fwd.c pj_gauss.c pj_geocent.c pj_gridinfo.c \
	pj_gridlist.c pj_init.c pj_init.h pj_inv.c pj_latlong.c pj_list.c pj_list.h \
	pj_malloc.c pj_mlfn.c pj_msfn.c pj_open_lib.c pj_param.c pj_phi2.c pj_pr_list.c \
	pj_qsfn.c pj_release.c pj_strerrno.c pj_transform.c pj_tsfn.c pj_units.c \
	pj_utils.c pj_zpoly1.c proj_mdist.c proj_rouss.c projects.h qg_gridlist.c \
	qg_grindinfo.c rtodms.c vd_apply_gridshift.c vector1.c vertical_datum_set.c


install-exec-local:
	rm -f $(DESTDIR)$(bindir)/invproj$(EXEEXT)
	(cd $(DESTDIR)$(bindir); ln -s proj$(EXEEXT) invproj$(EXEEXT))
	rm -f $(DESTDIR)$(bindir)/invgeod$(EXEEXT)
	(cd $(DESTDIR)$(bindir); ln -s geod$(EXEEXT) invgeod$(EXEEXT))
noinst_HEADERS = pj_init.h
geoid2geoid_SOURCES = geoid2geoid.c
geoid2geoid_LDADD = libproj.la
