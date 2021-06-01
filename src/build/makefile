#/********************************************************************16**
#
#            (c) Copyright 2012 by RadiSys Corporation. All rights reserved.
#
#     This software is confidential and proprietary to RadiSys Corporation.
#     No part of this software may be reproduced, stored, transmitted, 
#     disclosed or used in any form or by any means other than as expressly
#     provided by the written Software License Agreement between Radisys 
#     and its licensee.
#
#     Radisys warrants that for a period, as provided by the written
#     Software License Agreement between Radisys and its licensee, this
#     software will perform substantially to Radisys specifications as
#     published at the time of shipment, exclusive of any updates or 
#     upgrades, and the media used for delivery of this software will be 
#     free from defects in materials and workmanship.  Radisys also warrants 
#     that has the corporate authority to enter into and perform under the 
#     Software License Agreement and it is the copyright owner of the software 
#     as originally delivered to its licensee.
#
#     RADISYS MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
#     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
#     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
#     MATERIALS.
#
#     IN NO EVENT SHALL RADISYS BE LIABLE FOR ANY INDIRECT, SPECIAL,
#     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
#     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
#     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
#     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
#     OF THE POSSIBILITY OF SUCH DAMAGE.
#
#                       Restricted Rights Legend:
#
#     This software and all related materials licensed hereby are
#     classified as "restricted computer software" as defined in clause
#     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
#     developed entirely at private expense for nongovernmental purposes,
#     are commercial in nature and have been regularly used for
#     nongovernmental purposes, and, to the extent not published and
#     copyrighted, are trade secrets and confidential and are provided
#     with all rights reserved under the copyright laws of the United
#     States.  The government's rights to the software and related
#     materials are limited and restricted as provided in clause
#     52.227-19 of the FAR.
#
#                    IMPORTANT LIMITATION(S) ON USE
#
#     The use of this software is limited to the use set
#     forth in the written Software License Agreement between Radisys and
#     its Licensee. Among other things, the use of this software
#     may be limited to a particular type of Designated Equipment, as 
#     defined in such Software License Agreement.
#     Before any installation, use or transfer of this software, please
#     consult the written Software License Agreement or contact Radisys at
#     the location set forth below in order to confirm that you are
#     engaging in a permissible use of the software.
#
#                    RadiSys Corporation
#                    Tel: +1 (858) 882 8800
#                    Fax: +1 (858) 777 3388
#                    Email: supporttrillium.com
#                    Web: http://www.radisys.com 
# 
#*********************************************************************17*/
#********************************************************************20**
#
#       Name:   LTE eNodeB Sample Application
#
#       Type:   make file
#
#       Desc:   Compile, assemble and link product software for
#               various configurations. Further comments are
#               embedded in the file. 
#
#               This file supports a variety of environments and
#               build targets. The default build target will be the
#               portable target for the Linu 2.4.x with GNU C(gcc)
#
#       Env:    Linux 2.4.x with GNU C (gcc)
#
#               BUILD TARGETS:
#
#               clean            remove all object files
#   
#       File:  stack.mak
#
#
#       Prg:   an
#
#********************************************************************21*/
#=======================================================================

CPUINFO=`cat /proc/cpuinfo | grep processor| wc -l`
ifeq ($(CPUINFO), 1)
   NUM_PARALLEL=1
else
   NUM_PARALLEL=$(shell echo $(CPUINFO) - 1 |bc)
endif

ifeq ($(FAST), 1)
   NUM_PARALLEL=$(shell echo $(CPUINFO) + 2 |bc)
endif

PARALLEL_COMP=-j $(NUM_PARALLEL) 

Q=
S=
# Check for verbose build
ifneq ($(V),1)
    Q:=@
    S:=-s
endif
export Q
export S

#-----------------------------------------------------------------------
# macro for output file name and makefile name
#

lnxperf:
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak acc BLDENV=lnx_perf

lnxe2e:
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak acc BLDENV=lnx_pre
	@perl genrlogdb.pl l3 obj/
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak acc BLDENV=lnx_e2e

lin382:
	@echo "This is deprecated. You must be trying to build enodeB WITHOUT OAM"
	@echo "Please do \"make t2k_int\""
	@echo "For more options do \"make help\""

lin382_oam:
	@echo "This is deprecated. You must be trying to build enodeB WITH OAM"
	@echo "Please do \"make t2k_enb\""
	@echo "For more options do \"make help\""

#========================Qcom  specific=================================

qcom_enb:
	mkdir -p ./libs/
	mkdir -p ./rsys ./rsys/bin/ ./rsys/config  ./rsys/scripts
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak acc BLDENV=lnx_e2e_ms VENDOR=RSYS RSYS_RRM=YES RSYS_SON=YES RSYS_REMLIB=YES RSYS_REMAPP=YES CREATE_ENB=YES APP_WITH_OAM=YES $(PARALLEL_COMP)
	cp -f ../lteremapp/obj/hbs2-iu/hbs2-4/arm-v7a-linux-gnueabi/rem  rsys/bin/
	cp -f ../lteremapp/libs/arm-v7a-linux-gnueabi/libssi.a ../../libs/
	cp -f ../lteremapp/src/librem.a ../../libs/
	cp -f ./obj/enodeb ./rsys/bin/
	cp -f ./wr_cfg.txt ./rsys/config/
	cp -f ./start_eNB_qcom.sh ./rsys/scripts/start_eNB.sh
	cp -f ./runEnb.sh ./rsys/scripts/
	cp -f ./genL2Log.sh ./rsys/scripts/
	mkdir -p ../../bin/
	cp -rf ./rsys ../../bin/
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak  strip
	mkdir -p ../../libs/
	cp -f ./libs/*.a ../../libs

qcom_enb_blog:
	mkdir -p ./libs/
	mkdir -p ./rsys ./rsys/bin/ ./rsys/config  ./rsys/scripts
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak acc BLDENV=lnx_ms_pre VENDOR=RSYS RSYS_RRM=YES RSYS_SON=YES RSYS_REMLIB=YES RSYS_REMAPP=YES CREATE_ENB=YES APP_WITH_OAM=YES $(PARALLEL_COMP)
	perl genrlogdb.pl l3 QCOM obj/
	grep ";RRM;" l3.logdb >l3.logdb.rrm
	grep ";SON;" l3.logdb >l3.logdb.son
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak acc BLDENV=lnx_e2e_ms_blog VENDOR=RSYS RSYS_RRM=YES RSYS_SON=YES RSYS_REMLIB=YES RSYS_REMAPP=YES CREATE_ENB=YES APP_WITH_OAM=YES $(PARALLEL_COMP)
	cp -f ../lteremapp/obj/hbs2-iu/hbs2-4/arm-v7a-linux-gnueabi/rem  rsys/bin/
	cp -f ../lteremapp/libs/arm-v7a-linux-gnueabi/libssi.a ./libs/
	cp -f ../lteremapp/src/librem.a ./libs/
	cp -f ./obj/enodeb ./rsys/bin/
	cp -f ./obj/rlogapp ./rsys/bin/
	cp -f ./l3.logdb ./rsys/bin/
	cp -f ./wr_cfg.txt ./rsys/config/
	cp -f ./start_eNB_qcom.sh ./rsys/scripts/start_eNB.sh
	cp -f ./runEnb.sh ./rsys/scripts/
	cp -f ./genL2Log.sh ./rsys/scripts/
	mkdir -p ../../bin/
	cp -rf ./rsys ../../bin/
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak  strip
	mkdir -p ../../libs/
	cp -f ./libs/*.a ../../libs

qcom_int_blog:
	mkdir -p ./libs/
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak acc BLDENV=lnx_ms_pre RSYS_RRM=YES RSYS_SON=YES CREATE_ENB=YES $(PARALLEL_COMP)
	perl genrlogdb.pl l3 QCOM obj/
	grep ";RRM;" l3.logdb >l3.logdb.rrm
	grep ";SON;" l3.logdb >l3.logdb.son
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak acc BLDENV=lnx_e2e_ms_blog RSYS_RRM=YES RSYS_SON=YES CREATE_ENB=YES $(PARALLEL_COMP)
	mkdir -p ./rsys_withoutoam ./rsys_withoutoam/bin ./rsys_withoutoam/l1 ./rsys_withoutoam/scripts ./rsys_withoutoam/config 
	cp -f ./obj/enodeb ./rsys_withoutoam/bin
	cp -f ./obj/rlogapp ./rsys_withoutoam/bin/
	cp -f ./l3.logdb ./rsys_withoutoam/bin/
	cp -f ./wr_cfg.txt ./rsys_withoutoam/config/wr_cfg.txt
	cp -f ./wr_cfg.txt ./rsys_withoutoam/bin/wr_cfg.txt
	cp -f ./genL2Log.sh ./rsys_withoutoam/
	cp -f ./runEnbWithoutOam.sh ./rsys_withoutoam/
	mkdir -p ../../bin/
	cp -rf ./rsys_withoutoam ../../bin
	mkdir -p ../../libs/
	cp -f ./libs/*.a ../../libs
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak  strip_wooam

qcom_int:
	mkdir -p ./libs/
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak acc BLDENV=lnx_e2e_ms RSYS_RRM=YES RSYS_SON=YES CREATE_ENB=YES $(PARALLEL_COMP)
	mkdir -p ./rsys_withoutoam ./rsys_withoutoam/bin ./rsys_withoutoam/l1 ./rsys_withoutoam/scripts ./rsys_withoutoam/config 
	cp -f ./obj/enodeb ./rsys_withoutoam/bin
	cp -f ./wr_cfg.txt ./rsys_withoutoam/config/wr_cfg.txt
	cp -f ./wr_cfg.txt ./rsys_withoutoam/bin/wr_cfg.txt
	cp -f ./genL2Log.sh ./rsys_withoutoam/
	cp -f ./runEnbWithoutOam.sh ./rsys_withoutoam/
	mkdir -p ../../bin/
	cp -rf ./rsys_withoutoam ../../bin
	mkdir -p ../../libs/
	cp -f ./libs/*.a ../../libs
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak  strip_wooam

qcom_oam:
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak acc BLDENV=lnx_e2e_ms OAM_ENABLE=OAM_ENABLE VENDOR=RSYS $(PARALLEL_COMP)
	rm -rf ./rsys
	echo . | sh ./TeNBCP_build qcom

qcom_oam_clean:
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak VENDOR=RSYS clean_oam
	rm -rf ./rsys

qcom_rrm:
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak acc BLDENV=lnx_e2e_ms RSYS_RRM=YES RSYS_SON=YES $(PARALLEL_COMP)

qcom_rrm_clean:
	$(Q) $(MAKE) $(S) -f rm.mak OUT_DIR='$(RRM_LIBS_DIR)' OBJ='o' clean

qcom_remapp:
	mkdir -p ../lteremapp/libs/
	mkdir -p ../lteremapp/libs/arm-v7a-linux-gnueabi/
	if [ -e ../../libs/libssi.a ];then cp -f ../../libs/libssi.a ../lteremapp/libs/arm-v7a-linux-gnueabi/libssi.a;fi
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak acc BLDENV=lnx_e2e_ms RSYS_REMAPP=YES $(PARALLEL_COMP)
	mkdir -p ../../libs/
	mkdir -p ../../bin/
	mkdir -p ../../bin/rsys/
	mkdir -p ../../bin/rsys/bin/
	cp -f ../lteremapp/libs/arm-v7a-linux-gnueabi/libssi.a ../../libs/
	cp -f ../lteremapp/obj/hbs2-iu/hbs2-4/arm-v7a-linux-gnueabi/rem  ../../bin/rsys/bin/
	cp -f ../lteremapp/src/librem.a  ../../libs/

qcom_remapp_clean:
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak clean_remapp
	rm -f ./libs/libssi.a ./libs/librem.a ./libs/liboamcl.a ./rsys/bin/rem

qcom_remlib:
	mkdir -p ./libs/
	mkdir -p ../lteremapp/libs/
	mkdir -p ../lteremapp/libs/arm-v7a-linux-gnueabi/
	if [ -e ../../libs/libssi.a ];then cp -f ../../libs/libssi.a ../lteremapp/libs/arm-v7a-linux-gnueabi/libssi.a;fi
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak acc BLDENV=lnx_e2e_ms RSYS_OAMCLLIB=YES RSYS_REMLIB=YES $(PARALLEL_COMP)
	mkdir -p ../../libs/
	cp -f ../lteremapp/libs/arm-v7a-linux-gnueabi/libssi.a ../../libs/
	cp -f ../lteremapp/src/librem.a ../../libs/

qcom_remlib_clean:
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak clean_remlib 
	rm -f ./libs/libssi.a ./libs/liboamcl.a ./libs/librem.a

#	make -f enodeb_qcom.mak clean
#	rm -f ./libs/* 
qcom_clean:
	$(Q) $(MAKE) $(S) -f enodeb_qcom.mak clean
	$(Q) $(MAKE) $(S) qcom_rrm_clean
	$(Q) $(MAKE) $(S) qcom_remlib_clean
	rm -rf rsys_withoutoam
	rm -rf rsys
	rm -f ./libs/*.a
	perl genfilemodid.pl rlog ../enbapp/ ../lterrc/ ../lterrm/src/

qcom_clean_all:
	$(Q) $(MAKE) $(S) qcom_remapp_clean
	$(Q) $(MAKE) $(S) qcom_oam_clean
	$(Q) $(MAKE) $(S) qcom_clean

clean_all:
	$(Q) $(MAKE) $(S) qcom_clean_all

clean:
	$(Q) $(MAKE) $(S) qcom_clean

tenb_textlog:
	$(Q) $(MAKE) $(S) qcom_remlib 
	$(Q) $(MAKE) $(S) qcom_int 

tenb:
	$(Q) $(MAKE) $(S) qcom_remlib 
	$(Q) $(MAKE) $(S) qcom_int_blog 

tenb_oam_textlog:
	$(Q) $(MAKE) $(S) qcom_oam 
	$(Q) $(MAKE) $(S) qcom_remlib 
	$(Q) $(MAKE) $(S) qcom_remapp 
	$(Q) $(MAKE) $(S) qcom_enb 

tenb_oam:
	$(Q) $(MAKE) $(S) qcom_oam
	$(Q) $(MAKE) $(S) qcom_remlib
	$(Q) $(MAKE) $(S) qcom_remapp
	$(Q) $(MAKE) $(S) qcom_enb_blog


help:
	@echo "***********************************************************"
	@echo "TARGET            DESCRIPTION                              "
	@echo "-----------------------------------------------------------"
	@echo "tenb               Builds TenB without RADISYS OAM                      "
	@echo "tenb_oam           Builds TenB with RADISYS OAM, REM and RRM            "
	@echo "clean              Cleanup enodeb build                                 "
	@echo "clean_all          Cleanup everytihng                                   "
	@echo "qcom_oam           Builds Only RADISYS OAM                              "
	@echo "qcom_oam_clean     Cleans Only the RADISYS OAM build                        "
	@echo "qcom_rrm           Builds Only RADISYS RRM                                   "
	@echo "qcom_rrm_clean     Cleans Only RRM build                                    "
	@echo "qcom_remapp        Builds Only RADISYS REMAPP                                "
	@echo "qcom_remapp_clean  Cleans Only RADISYS REMAPP                                "
	@echo "qcom_remlib        Builds Only RADISYS REM LIBRARY                           "
	@echo "qcom_remlib_clean  Cleans Only RADISYS REM LIBRARY                           "
	@echo "****************************************************************************"

lnxe2e_oam_01:
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak acc BLDENV=lnx_pre OAM_ENV=WR_RSYS_OAM_SRC
	@perl Tokenize_Pre.pl
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak acc BLDENV=lnx_e2e OAM_ENV=WR_RSYS_OAM_SRC

mse2e:
	$(Q) $(MAKE) $(S) -f enodeb_uarm.mak acc BLDENV=lnx_ms_pre
	@perl genrlogdb.pl l3 obj/
	grep ";RRM;" l3.logdb >l3.logdb.rrm
	grep ";SON;" l3.logdb >l3.logdb.son
	$(Q) $(MAKE) $(S) -f enodeb_uarm.mak acc BLDENV=lnx_e2e_ms

#lnxe2e_oam_01:
#	@make -f enodeb_pal_lnx.mak acc BLDENV=lnx_pre OAM_ENV=WR_RSYS_OAM_SRC
#	@perl Tokenize_Pre.pl
#	@make -f enodeb_pal_lnx.mak acc BLDENV=lnx_e2e OAM_ENV=WR_RSYS_OAM_SRC

mse2e_oam_01:
	$(Q) $(MAKE) $(S) -f enodeb_uarm.mak acc BLDENV=lnx_ms_pre OAM_ENV=WR_RSYS_OAM_SRC
	@perl Tokenize_Pre.pl
	$(Q) $(MAKE) $(S) -f enodeb_uarm.mak acc BLDENV=lnx_e2e_ms OAM_ENV=WR_RSYS_OAM_SRC

lnxe2e_oam:
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak acc BLDENV=lnx_pre OAM_ENV=WR_RSYS_OAM
	@perl Tokenize_Pre.pl
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak acc BLDENV=lnx_e2e OAM_ENV=WR_RSYS_OAM

mse2e_oam:
	$(Q) $(MAKE) $(S) -f enodeb_uarm.mak acc BLDENV=lnx_ms_pre OAM_ENV=WR_RSYS_OAM
	@perl Tokenize_Pre.pl
	$(Q) $(MAKE) $(S) -f enodeb_uarm.mak acc BLDENV=lnx_e2e_ms OAM_ENV=WR_RSYS_OAM

lnx_withoutdiag:
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak acc BLDENV=lnx_withoutdiag

mse2e_withoutdiag:
	$(Q) $(MAKE) $(S) -f enodeb_uarm.mak acc BLDENV=lnx_ms_withoutdiag

lnx_withoutdiag_oam:
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak acc BLDENV=lnx_withoutdiag OAM_ENV=WR_RSYS_OAM

mse2e_withoutdiag_oam:
	$(Q) $(MAKE) $(S) -f enodeb_uarm.mak acc BLDENV=lnx_ms_withoutdiag OAM_ENV=WR_RSYS_OAM

ms4gmxe2e:
	$(Q) $(MAKE) $(S) -f enodeb_larm.mak acc BLDENV=4gmx_e2e

ms4gmxe2elib:
	$(Q) $(MAKE) $(S) -f enodeb_larm.mak acc BLDENV=4gmx_e2e_lib


lnx_lib:
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak acc BLDENV=lnx_lib

lnxclean:
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak clean

cleanstack:
	$(Q) $(MAKE) $(S) -f enodeb_larm.mak cleanstack

msclean:
	$(Q) $(MAKE) $(S) -f enodeb_uarm.mak clean
	perl genfilemodid.pl l3 ../src/ ../../lterrc ../../ltemac

lnxkworks:
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak acc BLDENV=lnx_kworks

lnxclean_oam:
	cd ../../tenb_commonplatform/software/products/fap/hbs2/hbs2-iu/;	$(MAKE) $(S) clean PRODUCT=generic HARDWARE=generic BUILD=i686-linux;

msclean_oam:
	cd ../../tenb_commonplatform/software/products/fap/hbs2/hbs2-iu/;	$(MAKE) $(S) clean PRODUCT=hbs2-iu HARDWARE=hbs2-4 BUILD=arm-v7a-linux-gnueabi;

lnxsplit:
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak acc BLDENV=lnx_split
	mkdir -p ../../../bin/rsys_withoutoam/
	cp -f obj/enodeb ../../../bin/rsys_withoutoam/
	cp -f wr_cfg_pal.txt ../../../bin/rsys_withoutoam/
	cp -f ys_cfg.txt ../../../bin/rsys_withoutoam/

lnxsplit_oam:
	$(Q) $(MAKE) $(S) -f enodeb_pal_lnx.mak acc BLDENV=lnx_split OAM_ENV=WR_RSYS_OAM
	echo . | . ./TeNBCP_build linux 
	cp -rf rsys ../../../bin/
	cp -f obj/librm.a ../../../libs/

lnxclean_all:
	$(Q) $(MAKE) $(S) lnxclean
	$(Q) $(MAKE) $(S) lnxclean_oam
	rm -rf rsys
	rm -rf lib/*

# DO NOT DELETE THIS LINE -- make depend depends on it.

