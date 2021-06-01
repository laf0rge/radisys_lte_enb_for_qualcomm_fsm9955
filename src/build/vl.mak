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
#    52.227-19 of the Federal Acquisition Regulation ("FAR") and were
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
#                    Email: support@trillium.com
#                    Web: http://www.radisys.com 
# 
#*********************************************************************17*/
#
#********************************************************************20**
#
#       Name:   VL - Qcom CL Compilation
#
#       Type:   Makefile
#
#       Desc:   Build the Qcom CL. 
#
#               For CL make:
#
#               1) ensure ALL the Object Files are mentioned in C_OBJ
#               2) ensure CCOPTS contain all the make optiones which are needed
#
#       File:   vl.mak
#
#       Sid:      vl.mak@@/main/TeNB_QCOM_REL_BR/2 - Fri Jun 27 17:28:14 2014
#
#       Prg:    amith
#
#  
#********************************************************************21*/
#
#-----------------------------------------------------------------------
#

#-----------------------------------------------------------------------
# SHOULD BE MODIFIED
# List the object files that need to be created

C_OBJS=  \
    $(OUT_DIR)/vl_init.$(OBJ) \
    $(OUT_DIR)/vl_utl.$(OBJ) \
    $(OUT_DIR)/vl_mi.$(OBJ) \
    $(OUT_DIR)/vl_ui.$(OBJ) \
    $(OUT_DIR)/vl_db.$(OBJ) \
    $(OUT_DIR)/vl_ptmi.$(OBJ) \
    $(OUT_DIR)/vl_ptui.$(OBJ) \
    $(OUT_DIR)/vl_stub_mi.$(OBJ) \
    $(OUT_DIR)/vl_tmr.$(OBJ)\
    $(OUT_DIR)/vl_stub.$(OBJ)\
    $(OUT_DIR)/vl_ex_ms.$(OBJ)

CPP_OBJS= \
    $(OUT_DIR)/vl_sm.$(OBJ)\
    $(OUT_DIR)/vl_dl.$(OBJ)\
    $(OUT_DIR)/vl_ul.$(OBJ)\
    $(OUT_DIR)/vl_li.$(OBJ)\
    $(OUT_DIR)/vl_fapi_mem.$(OBJ)\
    $(OUT_DIR)/vl_ifm_li.$(OBJ)

#-----------------------------------------------------------------------
# CAN BE MODIFIED
# This section can be used to pass specific options for the layer
# Normally this section should not be used unless absolutely necessary

ROOT=$(BUILD_ROOT)/../../../
include $(BUILD_ROOT)/../../../targets/fsm99xxCDP_1/make.target.vars

#QCOM_SPECIFIC_DEFS= -DCPU_SPEED=$(CPU_SPEED) 
QCOM_SPECIFIC_DEFS=  \
                    -DCPU_THREADS=$(CPU_THREADS) 

CCOPTS=$(COPTS) $(POPTS) $(QCOM_SPECIFIC_DEFS) $(IOPTS)
CPPOPTS=$(COPTS) $(POPTS) $(QCOM_SPECIFIC_DEFS)  $(IOPTS)


#-----------------------------------------------------------------------
# DO NOT EDIT THIS SECTION
# include for the building rules
#
ifeq ($(BLD_ENV),HEX)
include $(BUILD_ROOT)/build_rule.mak
else 
ifeq ($(BLD_ENV),PRE)
include $(BUILD_ROOT)/build_rule_pre.mak
else 
ifeq ($(BLD_ENV),POST)
include $(BUILD_ROOT)/build_rule_post.mak
else
include $(BUILD_ROOT)/build_rule.mak
endif
endif
endif

# DO NOT DELETE THIS LINE -- make depend depends on it.

