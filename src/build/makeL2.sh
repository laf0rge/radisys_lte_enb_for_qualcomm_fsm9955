#!/bin/bash
# Script to compile L2 layer with Qualcomm Hexagon Code Base
# Note: This script shall be run from app/rsys/l2/build folder
#
#

# Remove the binary files 
cd ../../../../
rm -f LINUX/project/platform-fsm9900_cdp-fapi/build-target/lte-l2-1.0.0/_hexBuild/_fsm99xxCDP_1/_fsm9900_cdp/bin/*
rm -f LINUX/project/platform-fsm9900_cdp-fapi/images/mdm2_00.mbn
cd -

# Verbose flag for Radisys L2 Stack Code Base. Values: YES, NO
RSYS_VERBOSE=NO
# Verbose flag for Qualcomm Hexagon Code Base. yes, no
QCOM_VERBOSE=no
#Preprocess the stack files
PRE=PRE
POST=POST
# Compile Radisys L2 stack code base
if [ $# -eq 0 ]
then
make -f qcom_l2.mak BLD_ENV=$PRE VERBOSE=$RSYS_VERBOSE 
perl genrlogdb.pl l2 QCOM obj_l2/
make -f qcom_l2.mak BLD_ENV=$POST VERBOSE=$RSYS_VERBOSE 
#make -f qcom_l2.mak VERBOSE=$RSYS_VERBOS
else
if [ $1 == "clean" ]
then
make -f qcom_l2.mak $1
perl genfilemodid.pl rlog ../ltemac/ ../lterlc/ ../ltepdcp/ ../lteclqc/
exit 0
fi
fi

# Compile the Qualcomm Hexagon Code and link the Radisys L2 stack code base  
cd ../../../../LINUX/target/fsm9900_cdp/
if [ $# -eq 0 ]
then
make lte-l2 
else 
if [ $1 == "cleantarget" ]
then
make lte-l2_clean
else
if [ $1 == "target" ]
then
make lte-l2
else
if [ $1 == "bsp" ]
then
make 
else
if [ $1 == "bspclean" ]
then
make clean
else
if [ $1 == "platformconfig" ]
then
make platformconfig
fi
fi
fi
fi
fi
fi
cd -

