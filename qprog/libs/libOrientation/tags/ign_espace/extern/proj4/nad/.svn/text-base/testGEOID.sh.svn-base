#!/bin/bash
EXE=$1
PROJ_LIB=$2
OUT=$3
INIT_FILE=$4

#export PROJ_LIB=./../../nad
# OUT=./../../nad/proj_qg_outIGNF
# INIT_FILE=./../../nad/IGNF

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

RES="-f %.3f"
#
echo "doing tests into file ${OUT}, please wait"
#
rm -f ${OUT}

echo "***************************************************************"
echo "              TEST PROGRAMME PROJ : FRANCE                     "
echo "***************************************************************"
echo "Running ${0} using ${EXE}:"
echo "================================================================"
echo "PROJ_LIB"=$PROJ_LIB
export PROJ_DEBUG=1
#
#
#
#
echo " --------- LAMBERT 2 ETENDUE <-> LAMBERT 93 -------------------"
echo "+init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93" >>${OUT}
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93 -E $RES >>${OUT} <<EOF
 600000.0000 2600545.4523  0.0000
 135638.3592 2418760.4094  0.0000
EOF
$EXE +init=${INIT_FILE}:LAMBE +to +init=${INIT_FILE}:LAMB93 -E $RES -v >> ${OUT}.part1 <<EOF
 600000.0000 2600545.4523  0.0000
 135638.3592 2418760.4094  0.0000
EOF
head -23 ${OUT}.part1
echo " ----------------------------"
echo " coordonnées réelles:                                          "
echo " L2E   E=600000.000  N=2600545.4523  h=0.0000                  "	
echo " L93   E=652760.737  N=7033791.244   h=0.000                   "
echo " test proj :                                                   "
tail -2 ${OUT}.part1 | head -1
echo " ----------------------------"
echo " coordonnées réelles:                                          "
echo " L2E   E=135638.3592  N=2418760.4094  h=0.0000                 "	
echo " L93   E=187194.062  N=6855928.882   h=0.000                   "
echo " test proj :                                                   "
tail -1 ${OUT}.part1 | head -4
/bin/rm -f ${OUT}.part1
#
# do 'diff' with distribution results
head -3 ${OUT} | tail -2 >> ${OUT}.part1
head -3 ${OUT}.dist | tail -2 >> ${OUT}.part1.dist
echo "diff ${OUT}.part1 with ${OUT}.part1.dist"
diff -b ${OUT}.part1 ${OUT}.part1.dist
if [ $? -ne 0 ] ; then
echo  ""
echo "PROBLEMS HAVE OCCURED"
echo "LAMBERT 2 ETENDUE <-> LAMBERT 93 "
echo "test file ${OUT}.part1 saved"
echo
exit 100
else
echo "TEST OK"
echo "LAMBERT 2 ETENDUE <-> LAMBERT 93 "
echo "test file ${OUT}.part1 removed"
echo
/bin/rm -f ${OUT}.part1  ${OUT}.part1.dist
fi
#
#
#
#
echo " --------- LAMBERT 3 CARTO <-> LAMBERT 93 -------------------"
echo "+init=${INIT_FILE}:LAMB3C +to +init=${INIT_FILE}:LAMB93" >>${OUT}
$EXE +init=${INIT_FILE}:LAMB3C +to +init=${INIT_FILE}:LAMB93 -E $RES >>${OUT} <<EOF
846254.09 3113870.54 191.13
817717.80 3212387.42 732.27
EOF
$EXE +init=${INIT_FILE}:LAMB3C +to +init=${INIT_FILE}:LAMB93 -E $RES -v >>${OUT}.part2 <<EOF
846254.09 3113870.54 191.13
817717.80 3212387.42 732.27
EOF
head -23 ${OUT}.part2
echo " ----------------------------"
echo " coordonnées réelles:                                          "
echo " L3C   E=846254.09  N=313870.54  h=191.13          "	
echo " L93   E=892556.27  N=6245637.81   h=191.13                 "
echo " test proj :                                                   "
tail -2 ${OUT}.part2 | head -1
echo " ----------------------------"
echo " coordonnées réelles:                                          "
echo " L3C   E=817717.80   N=3212387.42  h=732.27 		     "	
echo " L93   E=864691.44  N=6344379.28   h=732.27                    "
echo " test proj :                                                   "
tail -1 ${OUT}.part2 | head -1
/bin/rm -f ${OUT}.part2
#
# do 'diff' with distribution results
head -6 ${OUT} | tail -2 >> ${OUT}.part2
head -6 ${OUT}.dist | tail -2 >> ${OUT}.part2.dist
echo "diff ${OUT}.part2 with ${OUT}.part2.dist"
diff -b ${OUT}.part2 ${OUT}.part2.dist
if [ $? -ne 0 ] ; then
echo  ""
echo "PROBLEMS HAVE OCCURED"
echo "LAMBERT 3 CARTO <-> LAMBERT 93 "
echo "test file ${OUT}.part2 saved"
echo
exit 100
else
echo "TEST OK"
echo "LAMBERT 3 CARTO <-> LAMBERT 93"
echo "test file ${OUT}.part2 removed"
echo
/bin/rm -f ${OUT}.part2  ${OUT}.part2.dist
fi
#
#
#
#
RES=""
echo " --------- LAMBERT 1 <-> RGF93 -------------------"
echo "+init=${INIT_FILE}:LAMB1 +to +init=${INIT_FILE}:RGF93G" >>${OUT}
$EXE +init=${INIT_FILE}:LAMB1 +to +init=${INIT_FILE}:RGF93G -E $RES >>${OUT} <<EOF
597463.81 243917.77 34.4
593560.86 250950.16 72.188
EOF
$EXE +init=${INIT_FILE}:LAMB1 +to +init=${INIT_FILE}:RGF93G -E $RES -v >>${OUT}.part3 <<EOF
597463.81 243917.77 34.4
593560.86 250950.16 72.188
EOF
head -19 ${OUT}.part3
echo " ----------------------------"
echo " coordonnées réelles:                                          "
echo "  L1   E=597463.81  N=243917.77  h=34.4                 "	
echo "  RGF93G   Lon: 02d18'04,3616''E Lat:49d53'41.4145''N H: 78.27  "
echo " test proj :                                                   "
tail -2 ${OUT}.part3 | head -1
echo " ----------------------------"
echo " coordonnées réelles:                                          "
echo "  L1   E=593560.86   N=250950.16  h=72.188                      "	
echo "  RGF93G   Lon: 2°14'48,37224''E  Lat: 49°57'28,93760''N  H:116,056  "
echo " test proj :       "
tail -1 ${OUT}.part3 | head -1
/bin/rm -f ${OUT}.part3
#
# do 'diff' with distribution results
head -9 ${OUT} | tail -2 >> ${OUT}.part3
head -9 ${OUT}.dist | tail -2 >> ${OUT}.part3.dist
echo "diff ${OUT}.part3 with ${OUT}.part3.dist"
diff -b ${OUT}.part3 ${OUT}.part3.dist
if [ $? -ne 0 ] ; then
echo  ""
echo "PROBLEMS HAVE OCCURED"
echo "LAMBERT 1 <-> RGF93 "
echo "test file ${OUT}.part3 saved"
echo
exit 100
else
echo "TEST OK"
echo "LAMBERT 1 <-> RGF93"
echo "test file ${OUT}.part3 removed"
echo
/bin/rm -f ${OUT}.part3  ${OUT}.part3.dist
fi
#
#
#
#
RES=""
echo " --------- LAMBERT 1 <-> RGF93 -------------------"
echo "+init=${INIT_FILE}:LAMB1_EGM96 +to +init=${INIT_FILE}:RGF93G" >>${OUT}
$EXE +init=${INIT_FILE}:LAMB1_EGM96 +to +init=${INIT_FILE}:RGF93G -E $RES >>${OUT} <<EOF
597463.81 243917.77 34.4
593560.86 250950.16 72.188
EOF
$EXE +init=${INIT_FILE}:LAMB1_EGM96 +to +init=${INIT_FILE}:RGF93G -E $RES -v >>${OUT}.part4 <<EOF
597463.81 243917.77 34.4
593560.86 250950.16 72.188
EOF
head -19 ${OUT}.part4
echo " ----------------------------"
echo " coordonnées réelles:                                          "
echo "  L1_IGN69   E=597463.81  N=243917.77  h=34.4                 "	
echo "  RGF93G   Lon: 02d18'04,3616''E Lat:49d53'41.4145''N H: 78.27  "
echo " test proj :                                                   "
tail -2 ${OUT}.part4 | head -1
echo " ----------------------------"
echo " coordonnées réelles:                                          "
echo "  L1_IGN69   E=593560.86   N=250950.16  h=72.188                      "	
echo "  RGF93G   Lon: 2°14'48,37224''E  Lat: 49°57'28,93760''N  H:116,056  "
echo " test proj :       "
tail -1 ${OUT}.part4 | head -1
/bin/rm -f ${OUT}.part4
#
# do 'diff' with distribution results
head -12 ${OUT} | tail -2 >> ${OUT}.part4
head -12 ${OUT}.dist | tail -2 >> ${OUT}.part4.dist
echo "diff ${OUT}.part4 with ${OUT}.part4.dist"
diff -b ${OUT}.part4 ${OUT}.part4.dist
if [ $? -ne 0 ] ; then
echo  ""
echo "PROBLEMS HAVE OCCURED"
echo "LAMBERT 1 EGM_96 <-> RGF93 "
echo "test file ${OUT}.part4 saved"
echo
exit 100
else
echo "TEST OK"
echo "LAMBERT 1 EGM_96 <-> RGF93"
echo "test file ${OUT}.part4 removed"
echo
/bin/rm -f ${OUT}.part4  ${OUT}.part4.dist
fi
# 
# 
/bin/rm -f ${OUT}
exit 0
