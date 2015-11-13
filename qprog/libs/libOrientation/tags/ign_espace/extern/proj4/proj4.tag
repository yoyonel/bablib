<?xml version='1.0' encoding='ISO-8859-1' standalone='yes' ?>
<tagfile>
  <compound kind="page">
    <name>index</name>
    <title>Proj4 -Cartographic Projections Library</title>
    <filename>index</filename>
    <docanchor file="/home/ndavid/codes/PROJ/proj_cvs/docs/doxygen/doxygen.dox">Download</docanchor>
    <docanchor file="index">Ressources</docanchor>
  </compound>
  <compound kind="file">
    <name>proj_ap.h</name>
    <path>/home/ndavid/codes/PROJ/proj_cvs/docs/doxygen/</path>
    <filename>proj__ap_8h</filename>
    <member kind="typedef">
      <type>PJ *</type>
      <name>projPJ</name>
      <anchorfile>proj__ap_8h.html</anchorfile>
      <anchor>be087ebc9122352389a359155ec6f050</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>jniproj.c</name>
    <path>/home/ndavid/codes/PROJ/proj_cvs/src/</path>
    <filename>jniproj_8c</filename>
  </compound>
  <compound kind="file">
    <name>pj_transform.c</name>
    <path>/home/ndavid/codes/PROJ/proj_cvs/src/</path>
    <filename>pj__transform_8c</filename>
    <includes id="projects_8h" name="projects.h" local="no" imported="no">projects.h</includes>
    <member kind="define">
      <type>#define</type>
      <name>SRS_WGS84_SEMIMAJOR</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>25d3c2b305ebaf55429fd5928398b3e2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>SRS_WGS84_ESQUARED</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>ea83f0cf0f55665900cda399f5a48e2e</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Dx_BF</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>0c9ba2b256c3c472a7ac830d89f2eb80</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Dy_BF</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>57c0e21d48773d61cf91129648f273d7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Dz_BF</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>cd831dd8e081099b3b7a6ea8922edb41</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Rx_BF</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>a3ea85d34e67ee041e157f502ddd6943</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Ry_BF</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>7407fffb5cdbd3e2f73b95fc754a82a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>Rz_BF</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>8bd0402e064a6395fe3520a52353bb96</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>M_BF</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>5982f4871d9c4f1a4bc6d8c7847ed1d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CHECK_RETURN</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>fa02ff918b3994b7340449f307cf698b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>PJ_CVSID</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>7bb701d324afd248a489e815783c493c</anchor>
      <arglist>(&quot;$Id: pj_transform.c,v 1.24 2007/12/03 15:48:20 fwarmerdam Exp $&quot;)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_compare_datums</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>8f12eb2d33852e449cb4c04effa307fa</anchor>
      <arglist>(PJ *srcdefn, PJ *dstdefn)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_geocentric_to_wgs84</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>3e03984494d39c265c5cbb065ba94dca</anchor>
      <arglist>(PJ *defn, long point_count, int point_offset, double *x, double *y, double *z)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_geocentric_from_wgs84</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>e3cc130085bfcc2a0883eb4dcb2fb394</anchor>
      <arglist>(PJ *defn, long point_count, int point_offset, double *x, double *y, double *z)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_datum_transform</name>
      <anchorfile>pj__transform_8c.html</anchorfile>
      <anchor>81d2e2af5d6736782363c37397570b45</anchor>
      <arglist>(PJ *srcdefn, PJ *dstdefn, long point_count, int point_offset, double *x, double *y, double *z)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_transform</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g1457f1f46efc2fa1e6d914cc12e4d2a6</anchor>
      <arglist>(projPJ srcdefn, projPJ dstdefn, long point_count, int point_offset, double *x, double *y, double *z)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_geodetic_to_geocentric</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g4fb5207b18c656243fee1857f6012fcd</anchor>
      <arglist>(double a, double es, long point_count, int point_offset, double *x, double *y, double *z)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_geocentric_to_geodetic</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g828de12a8c338137864cfaa61989cf1b</anchor>
      <arglist>(double a, double es, long point_count, int point_offset, double *x, double *y, double *z)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>proj_api.h</name>
    <path>/home/ndavid/codes/PROJ/proj_cvs/src/</path>
    <filename>proj__api_8h</filename>
    <class kind="struct">projUV</class>
    <member kind="define">
      <type>#define</type>
      <name>PJ_VERSION</name>
      <anchorfile>proj__api_8h.html</anchorfile>
      <anchor>00a8614c89983ac0bf31955a23922537</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RAD_TO_DEG</name>
      <anchorfile>proj__api_8h.html</anchorfile>
      <anchor>89e47af0449640d4f15191aba5ca24c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DEG_TO_RAD</name>
      <anchorfile>proj__api_8h.html</anchorfile>
      <anchor>212460e743fecb084d717bb2180c5a56</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>projXY</name>
      <anchorfile>proj__api_8h.html</anchorfile>
      <anchor>ef2739588f916e807a9085093a1943b2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>projLP</name>
      <anchorfile>proj__api_8h.html</anchorfile>
      <anchor>7869d4211b8c6f560f537273993e42cc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>void *</type>
      <name>projPJ</name>
      <anchorfile>proj__api_8h.html</anchorfile>
      <anchor>ea4dd1c4fc6b1070245e4b919e290499</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_transform</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g1457f1f46efc2fa1e6d914cc12e4d2a6</anchor>
      <arglist>(projPJ src, projPJ dst, long point_count, int point_offset, double *x, double *y, double *z)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_geocentric_to_geodetic</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g828de12a8c338137864cfaa61989cf1b</anchor>
      <arglist>(double a, double es, long point_count, int point_offset, double *x, double *y, double *z)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_geodetic_to_geocentric</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g4fb5207b18c656243fee1857f6012fcd</anchor>
      <arglist>(double a, double es, long point_count, int point_offset, double *x, double *y, double *z)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_apply_gridshift</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>ga1e0ac5144bcc78926b6dbd41f37b5bd</anchor>
      <arglist>(const char *, int, long point_count, int point_offset, double *x, double *y, double *z)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pj_deallocate_grids</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g7577b48901e8f8e7366730d3203a2e0d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pj_set_searchpath</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>gad350b99f2519d7c2f480de93073671f</anchor>
      <arglist>(int count, const char **path)</arglist>
    </member>
    <member kind="function">
      <type>projPJ</type>
      <name>pj_init</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g87a10f9442a53068526986132702dcfe</anchor>
      <arglist>(int, char **)</arglist>
    </member>
    <member kind="function">
      <type>projPJ</type>
      <name>pj_init_plus</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g4a711893e596718b9834b74ce174718d</anchor>
      <arglist>(const char *)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>pj_malloc</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g071ea4bff01372c384dad9682c93fc3a</anchor>
      <arglist>(size_t)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pj_dalloc</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>ge757cc716b4ea48812630961579c0172</anchor>
      <arglist>(void *)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>pj_strerrno</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>gd7d38d67286041e49caa6ae03413085b</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="function">
      <type>int *</type>
      <name>pj_get_errno_ref</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g7e0f473d6d29d54bc08bb3e1708e0762</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>pj_get_release</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g75caf4848c83211f8e8070318628170d</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function">
      <type>projXY</type>
      <name>pj_fwd</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g276eaf73152691fa951f6e5292065030</anchor>
      <arglist>(projLP, projPJ)</arglist>
    </member>
    <member kind="function">
      <type>projLP</type>
      <name>pj_inv</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g8dea8e93cc1d15dec875aec576c90b66</anchor>
      <arglist>(projXY, projPJ)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_compare_datums</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>gedf0414d592e264d1b45b351f7702d5c</anchor>
      <arglist>(projPJ srcdefn, projPJ dstdefn)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pj_set_finder</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g3562f23b6ded6ddb811f8b12437d1ff5</anchor>
      <arglist>(const char *(*)(const char *))</arglist>
    </member>
    <member kind="function">
      <type>projPJ</type>
      <name>pj_latlong_from_proj</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g1bf5d9d16a44c42dedb114ebf2de2307</anchor>
      <arglist>(projPJ)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_datum_transform</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g000a35ccfae92618944af04a196d51cf</anchor>
      <arglist>(projPJ src, projPJ dst, long point_count, int point_offset, double *x, double *y, double *z)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_is_latlong</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g307ad975e9a63330bc921ce83b808fc4</anchor>
      <arglist>(projPJ)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_is_geocent</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g45b374c0b6bd8ab919adecfcaf7cc86d</anchor>
      <arglist>(projPJ)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pj_pr_list</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>gb7e5b6932e81475305c6e0e358aa42d1</anchor>
      <arglist>(projPJ)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pj_free</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>g61b51c99c071a47ae41efd6165cdddc2</anchor>
      <arglist>(projPJ)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>pj_get_def</name>
      <anchorfile>group__proj__API.html</anchorfile>
      <anchor>gc40cdfe2392f30e457725648095a84d0</anchor>
      <arglist>(projPJ, int)</arglist>
    </member>
    <member kind="variable">
      <type>char const</type>
      <name>pj_release</name>
      <anchorfile>proj__api_8h.html</anchorfile>
      <anchor>1826e0f30d5237105be5a5d01f1c7215</anchor>
      <arglist>[]</arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>pj_errno</name>
      <anchorfile>proj__api_8h.html</anchorfile>
      <anchor>2477bd31455a2bf38fa11b5408642d44</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>projects.h</name>
    <path>/home/ndavid/codes/PROJ/proj_cvs/src/</path>
    <filename>projects_8h</filename>
    <includes id="proj__api_8h" name="proj_api.h" local="yes" imported="no">proj_api.h</includes>
    <class kind="struct">projUV</class>
    <class kind="struct">COMPLEX</class>
    <class kind="union">PVALUE</class>
    <class kind="struct">PJ_LIST</class>
    <class kind="struct">PJ_ELLPS</class>
    <class kind="struct">PJ_UNITS</class>
    <class kind="struct">PJ_DATUMS</class>
    <class kind="struct">VD_DATUMS</class>
    <class kind="struct">PJ_PRIME_MERIDIANS</class>
    <class kind="struct">DERIVS</class>
    <class kind="struct">FACTORS</class>
    <class kind="struct">ARG_list</class>
    <class kind="struct">PJconsts</class>
    <class kind="struct">FLP</class>
    <class kind="struct">ILP</class>
    <class kind="struct">CTABLE</class>
    <class kind="struct">QG_TABLE</class>
    <class kind="struct">_pj_gi</class>
    <class kind="struct">_qg_gi</class>
    <class kind="struct">PW_COEF</class>
    <class kind="struct">Tseries</class>
    <member kind="define">
      <type>#define</type>
      <name>C_NAMESPACE</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>274b0d5c36ea98bfadb78a6048752479</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>C_NAMESPACE_VAR</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>bd26f46b4cc329d71cfa56b390dac2fa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>NULL</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>070d2ce7b6bb7e5c05602aa8c308d0c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FALSE</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>a93f0eb578d23995850d61f7d61c55c1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TRUE</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>a8cecfc5c5c054d2875c03e77b7be15d</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MIN</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>3acffbd305ee72dcd4593c0d8af64a4f</anchor>
      <arglist>(a, b)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>fa99ec4acc4ecb2dc3c2d05da15d0e3f</anchor>
      <arglist>(a, b)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ABS</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>996f7be338ccb40d1a2a5abc1ad61759</anchor>
      <arglist>(x)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_PATH_FILENAME</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>7c5d2f95b404290e116d0ad72959c635</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>HALFPI</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>59d83d878ab677f99e48bd48ad80eef0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>FORTPI</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>f1209ece986c0719c938442edbc6a9e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PI</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>598a3330b3c21701223ee0ca14316eca</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>TWOPI</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>4912c64aec0c943b7985db6cb61ff83a</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PROJ_LIB</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>6f8fe33196ae53736f7f0ddcc5a7c394</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ID_TAG_MAX</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>529f8b78c8ba8179f9b12a24ec635713</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DIR_CHAR</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>485855876266f06fff97ab53259af368</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PJD_UNKNOWN</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>4102aaf019a02f35e176cd8cb02a60d1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PJD_3PARAM</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>0fd4bb6047d7cbefdd2ad4d3c673ae17</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PJD_7PARAM</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>ac9ee3ba4d14f4e6251b8e093ef04f90</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PJD_GRIDSHIFT</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>fbc6ef681639ead236564fb56d8b2011</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PJD_WGS84</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>81b3c6d63e25d3c860f81c7169f183aa</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PJD_ERR_GEOCENTRIC</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>f15ef99d30b2bd81cd9572409193a1e0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USE_PROJUV</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>bdbd11d729ec094690a43e0f1433ab73</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>XY</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>323358fd84497c5085b677efc3c1ab92</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>LP</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>5b11d088a6ab6484ad47eaa4398e58eb</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IS_ANAL_XL_YL</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>b7b1728225b3a409aa2a9eda9ec89ece</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IS_ANAL_XP_YP</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>e5e8f6b282f31b6faec3053babd7c4df</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IS_ANAL_HK</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>48fe1129e798aeebc375f1d5eed3844f</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>IS_ANAL_CONV</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>584f4d90ada94fb7bc4bc88f4773baa7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>MAX_TAB_ID</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>c7576eeb68a6b16837766dcdb9e88cee</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PJ_CVSID</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>c5fa8678d7924944b29cb3c505cbec51</anchor>
      <arglist>(string)</arglist>
    </member>
    <member kind="typedef">
      <type>struct ARG_list</type>
      <name>paralist</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>b1cb0255190022ba3f657f050240047d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct PJconsts</type>
      <name>PJ</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>d949da9b7d11511ebe0b74ac089774c2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>PJ *</type>
      <name>projPJ</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>be087ebc9122352389a359155ec6f050</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct _pj_gi</type>
      <name>PJ_GRIDINFO</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>edcca343683e595d3b340794e45d8325</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct _qg_gi</type>
      <name>QG_GRIDINFO</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>a081ab86a6af0c3f8e2995f60c7efe48</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>hypot</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>0cdb0b447bb2bd586b5a58bc9b61e285</anchor>
      <arglist>(double, double)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>vd_datum_set</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>34ef1739dc9d1f99752d38779d6d93ee</anchor>
      <arglist>(paralist *pl, PJ *projdef)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>dmstor</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>6d69f1d0add8bb63c930b2c013879652</anchor>
      <arglist>(const char *, char **)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>set_rtodms</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>bcb22e0e1b0ba1402d9b25bf35097fb7</anchor>
      <arglist>(int, int)</arglist>
    </member>
    <member kind="function">
      <type>char *</type>
      <name>rtodms</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>c0077f718967d44db389f3fad5217bbf</anchor>
      <arglist>(char *, double, int, int)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>adjlon</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>bdcce12c9897c81d3756f0f711cbf5cb</anchor>
      <arglist>(double)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>aacos</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>9a1b5aba507d14c0756ce144deac096e</anchor>
      <arglist>(double)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>aasin</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>d0cdfb66c15ad8931bccf32954eaaebf</anchor>
      <arglist>(double)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>asqrt</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>55ebad215545663dca8acf60f8611f0b</anchor>
      <arglist>(double)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>aatan2</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>1336df3efc52843efcf5cf1810693dfc</anchor>
      <arglist>(double, double)</arglist>
    </member>
    <member kind="function">
      <type>PVALUE</type>
      <name>pj_param</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>9e48b2a0fecfad0668b7b11876eb3975</anchor>
      <arglist>(paralist *, char *)</arglist>
    </member>
    <member kind="function">
      <type>paralist *</type>
      <name>pj_mkparam</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>173ada4107907523635c207de6cc9ae0</anchor>
      <arglist>(char *)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_ell_set</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>bcb7e4471b9a6db1bf395a1f1f52f37b</anchor>
      <arglist>(paralist *, double *, double *)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_datum_set</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>c701bf79ab032ca557a7ef505ef38252</anchor>
      <arglist>(paralist *, PJ *)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_prime_meridian_set</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>c169ee6020a538635f2b36e237c91e7f</anchor>
      <arglist>(paralist *, PJ *)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_angular_units_set</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>fde784599f8d799ee49350dd244dc275</anchor>
      <arglist>(paralist *, PJ *)</arglist>
    </member>
    <member kind="function">
      <type>double *</type>
      <name>pj_enfn</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>db2bdf7e6b8028a813493641ec1a34da</anchor>
      <arglist>(double)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>pj_mlfn</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>d9441197acb6b2a2cc621bacf15c302c</anchor>
      <arglist>(double, double, double, double *)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>pj_inv_mlfn</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>e4e6afad7480cb7ecdd012f05cfbd1ba</anchor>
      <arglist>(double, double, double *)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>pj_qsfn</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>9fced9dbfa8bd0bcaa50cdce128198d0</anchor>
      <arglist>(double, double, double)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>pj_tsfn</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>5b13a4708e464d0a087d6a881172a36d</anchor>
      <arglist>(double, double, double)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>pj_msfn</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>d9a62444d542988d7338d2634190c393</anchor>
      <arglist>(double, double, double)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>pj_phi2</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>2c56a1980b4f95a90229b4e7e0f6aa93</anchor>
      <arglist>(double, double)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>pj_qsfn_</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>e9d021402d67d20c25599f3e1c8a0020</anchor>
      <arglist>(double, PJ *)</arglist>
    </member>
    <member kind="function">
      <type>double *</type>
      <name>pj_authset</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>8837d46aab172323e0096230a1a3c450</anchor>
      <arglist>(double)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>pj_authlat</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>7f2649b0efecb8367d3e963a81452765</anchor>
      <arglist>(double, double *)</arglist>
    </member>
    <member kind="function">
      <type>COMPLEX</type>
      <name>pj_zpoly1</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>864595dbe053d24c66b09b52bf248076</anchor>
      <arglist>(COMPLEX, COMPLEX *, int)</arglist>
    </member>
    <member kind="function">
      <type>COMPLEX</type>
      <name>pj_zpolyd1</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>6ac16f3885a8d1b1ed1e645c61b7eee3</anchor>
      <arglist>(COMPLEX, COMPLEX *, int, COMPLEX *)</arglist>
    </member>
    <member kind="function">
      <type>FILE *</type>
      <name>pj_open_lib</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>90d3f3aedda63876034339a77d22d6df</anchor>
      <arglist>(char *, char *)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_deriv</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>119490b1b24f019dfeb5cf64af0f42c9</anchor>
      <arglist>(LP, double, PJ *, struct DERIVS *)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>pj_factors</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>a4fb290453b6299df4a73d466fd4db8c</anchor>
      <arglist>(LP, PJ *, double, struct FACTORS *)</arglist>
    </member>
    <member kind="function">
      <type>Tseries *</type>
      <name>mk_cheby</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>24bf88b8252562533445d7837a752bad</anchor>
      <arglist>(projUV, projUV, double, projUV *, projUV(*)(projUV), int, int, int)</arglist>
    </member>
    <member kind="function">
      <type>projUV</type>
      <name>bpseval</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>f5bca1ae168d4ba617c4fed4003f9493</anchor>
      <arglist>(projUV, Tseries *)</arglist>
    </member>
    <member kind="function">
      <type>projUV</type>
      <name>bcheval</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>a173179607d4c6e79911d150d2ca4be7</anchor>
      <arglist>(projUV, Tseries *)</arglist>
    </member>
    <member kind="function">
      <type>projUV</type>
      <name>biveval</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>f7fd0ebd0feb4794f7406af809adab4e</anchor>
      <arglist>(projUV, Tseries *)</arglist>
    </member>
    <member kind="function">
      <type>void *</type>
      <name>vector1</name>
      <anchorfile>projects_8h.html</anchorfile>
      <anchor>ad3463a49fd8ca14468899432a8cb156</anchor>
      <arglist>(int, int)</arglist>
    </member>
    <member kind="function">
      <type>void **</type>
 