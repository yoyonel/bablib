:
# Script to test some points in IGNF managed SRS.
# The expected coordinates have been calculed by
# other means.
# Real points locations are found in proj_outIGNF.dist-real
# while distribution results are found in proj_outIGNF.dist
#
EXE=$1
PROJ_LIB=$2
OUT=$3
INIT_FILE=$4

#export PROJ_LIB=./../../nad
#OUT=./../../nad/proj_outIGNF
#INIT_FILE=./../../nad/IGNF

usage()
{
    echo "Usage: ${0} <path to 'cs2cs' program>" 
    echo
    exit 1
}

if test -z "${EXE}"; then
    echo "*** ERROR: Missing path to 'cs2cs' program."
    usage
else
    if test ! -x ${EXE}; then
        echo "*** ERROR: Can not find '${EXE}' program!"
        exit 1
    fi
fi

echo "============================================"
echo "Running ${0} using ${EXE}:"
echo "============================================"



RES="-f %.3f"
#
echo "doing tests into file ${OUT}, please wait"
#
rm -f ${OUT}
echo "+init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93" >>${OUT}
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93 -E $RES >>${OUT} <<EOF
 600000.0000 2600545.4523  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93 -E $RES >>${OUT} <<EOF
 135638.3592 2418760.4094  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93 -E $RES >>${OUT} <<EOF
 998137.3947 2413822.2844  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93 -E $RES >>${OUT} <<EOF
 600000.0000 2200000.0000  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93 -E $RES >>${OUT} <<EOF
 311552.5340 1906457.4840  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93 -E $RES >>${OUT} <<EOF
 960488.4138 1910172.8812  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93 -E $RES >>${OUT} <<EOF
 600000.0000 1699510.8340  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93 -E $RES >>${OUT} <<EOF
1203792.5981 626873.17210  0.0000
EOF
echo "+init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:GEOPORTALFXX" >>${OUT}
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
 600000.0000 2600545.4523  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
 135638.3592 2418760.4094  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
 998137.3947 2413822.2844  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
 600000.0000 2200000.0000  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
 311552.5340 1906457.4840  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
 960488.4138 1910172.8812  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
 600000.0000 1699510.8340  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
1203792.5981 626873.17210  0.0000
EOF
echo "+init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:GEOPORTALFXX" >>${OUT}
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
2d20'11.4239243" 50d23'59.7718445" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
-3d57'49.4051448" 48d35'59.7121716" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
7d44'12.1439796" 48d35'59.7832558" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
2d20'11.4951975" 46d47'59.8029841" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
-1d15'48.9240599" 44d05'59.8251878" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
6d50'12.2276489" 44d06'00.0517019" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
2d20'11.7754730" 42d18'00.0824436" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:GEOPORTALFXX -E $RES >>${OUT} <<EOF
9d32'12.6680218" 41d24'00.3542556" 0.0
EOF
echo "+init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:MILLER" >>${OUT}
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:MILLER -E $RES >>${OUT} <<EOF
2d20'11.4239243" 50d23'59.7718445" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:MILLER -E $RES >>${OUT} <<EOF
-3d57'49.4051448" 48d35'59.7121716" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:MILLER -E $RES >>${OUT} <<EOF
7d44'12.1439796" 48d35'59.7832558" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:MILLER -E $RES >>${OUT} <<EOF
2d20'11.4951975" 46d47'59.8029841" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:MILLER -E $RES >>${OUT} <<EOF
-1d15'48.9240599" 44d05'59.8251878" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:MILLER -E $RES >>${OUT} <<EOF
6d50'12.2276489" 44d06'00.0517019" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:MILLER -E $RES >>${OUT} <<EOF
2d20'11.7754730" 42d18'00.0824436" 0.0
EOF
$EXE +init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:MILLER -E $RES >>${OUT} <<EOF
9d32'12.6680218" 41d24'00.3542556" 0.0
EOF
echo "+init=${INIT_FILE}:RGR92 +to +init=${INIT_FILE}:REUN47" >>${OUT}
$EXE +init=${INIT_FILE}:RGR92 +to +init=${INIT_FILE}:REUN47 -E $RES >>${OUT} <<EOF
3356123.5400 1303218.3090 5247430.6050
EOF
#
# do 'diff' with distribution results
head -9 ${OUT} | tail -8 >> ${OUT}.part1
head -9 ${OUT}.dist | tail -8 >> ${OUT}.part1.dist
echo "diff ${OUT}.part1 with ${OUT}.part1.dist"
diff -b ${OUT}.part1 ${OUT}.part1.dist
if [ $? -ne 0 ] ; then
echo  ""
echo "PROBLEMS HAVE OCCURED"
echo "+init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93"
echo "test file ${OUT}.part1 saved"
echo
exit 100
else
echo "TEST OK"
echo "+init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93"
echo "test file ${OUT}.part1 removed"
echo
/bin/rm -f ${OUT}.part1  ${OUT}.part1.dist
fi

head -18 ${OUT} | tail -8 >> ${OUT}.part2
head -18 ${OUT}.dist | tail -8 >> ${OUT}.part2.dist
echo "diff ${OUT}.part2 with ${OUT}.part2.dist"
diff -b ${OUT}.part2 ${OUT}.part2.dist
if [ $? -ne 0 ] ; then
echo  ""
echo "PROBLEMS HAVE OCCURED"
echo "+init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:GEOPORTALFXX"
echo "test file ${OUT}.part2 saved"
echo
exit 100
else
echo "TEST OK"
echo "+init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:GEOPORTALFXX"
echo "test file ${OUT}.part2 removed"
echo
/bin/rm -f ${OUT}.part2  ${OUT}.part2.dist
fi

head -27 ${OUT} | tail -8 >> ${OUT}.part3
head -27 ${OUT}.dist | tail -8 >> ${OUT}.part3.dist
echo "diff ${OUT}.part3 with ${OUT}.part3.dist"
diff -b ${OUT}.part3 ${OUT}.part3.dist
if [ $? -ne 0 ] ; then
echo  ""
echo "PROBLEMS HAVE OCCURED"
echo "+init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:GEOPORTALFXX"
echo "test file ${OUT}.part3 saved"
echo
exit 100
else
echo "TEST OK"
echo "+init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:GEOPORTALFXX"
echo "test file ${OUT}.part3 removed"
echo
/bin/rm -f ${OUT}.part3  ${OUT}.part3.dist
fi

head -36 ${OUT} | tail -8 >> ${OUT}.part4
head -36 ${OUT}.dist | tail -8 >> ${OUT}.part4.dist
echo "diff ${OUT}.part4 with ${OUT}.part4.dist"
diff -b ${OUT}.part4 ${OUT}.part4.dist
if [ $? -ne 0 ] ; then
echo  ""
echo "PROBLEMS HAVE OCCURED"
echo "+init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:MILLER"
echo "test file ${OUT}.part4 saved"
echo
exit 100
else
echo "TEST OK"
echo "+init=${INIT_FILE}:RGF93G +to +init=${INIT_FILE}:MILLER" 
echo "test file ${OUT}.part4 removed"
echo
/bin/rm -f ${OUT}.part4  ${OUT}.part4.dist
fi

head -39 ${OUT} | tail -1 >> ${OUT}.part5
head -39 ${OUT}.dist | tail -1 >> ${OUT}.part5.dist
echo "diff ${OUT}.part5 with ${OUT}.part5.dist"
diff -b ${OUT}.part5 ${OUT}.part5.dist
if [ $? -ne 0 ] ; then
echo  ""
echo "PROBLEMS HAVE OCCURED"
echo "+init=${INIT_FILE}:RGR92 +to +init=${INIT_FILE}:REUN47"
echo "test file ${OUT}.part5 saved"
echo
exit 100
else
echo "TEST OK"
echo "+init=${INIT_FILE}:RGR92 +to +init=${INIT_FILE}:REUN47"
echo "test file ${OUT}.part5 removed"
echo
/bin/rm -f ${OUT}.part5  ${OUT}.part5.dist
fi

/bin/rm -f ${OUT}
exit 0
