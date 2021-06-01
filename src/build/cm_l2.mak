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
#       Name:   CM - CM Compilation
#
#       Type:   Makefile
#
#       Desc:   Build the System Services.
#
#               For CM make:
#
#               1) ensure ALL the Object Files are mentioned in C_OBJ
#               2) ensure CCOPTS contain all the make optiones which are needed
#
#       File:   cm.mak
#
#       Sid:      cm_l2.mak@@/main/TeNB_QCOM_REL_BR/1 - Fri Mar 28 15:37:08 2014
#
#       Prg:    amith
#  
 
#********************************************************************21*/
#
#-----------------------------------------------------------------------
#

#-----------------------------------------------------------------------
# SHOULD BE MODIFIED
# List the object files that need to be created

C_OBJS = \
         $(OUT_DIR)/cm_bdy5.$(OBJ)\
         $(OUT_DIR)/cm_mem.$(OBJ)\
         $(OUT_DIR)/cm_mem_wl.$(OBJ)\
         $(OUT_DIR)/cm_gen.$(OBJ)\
         $(OUT_DIR)/cm_hash.$(OBJ)\
         $(OUT_DIR)/cm_math.$(OBJ)\
         $(OUT_DIR)/cm_lib.$(OBJ)\
         $(OUT_DIR)/cm_llist.$(OBJ)\
         $(OUT_DIR)/cm_lte.$(OBJ)\
         $(OUT_DIR)/cm_mblk.$(OBJ)\
         $(OUT_DIR)/cm_tkns.$(OBJ)\
         $(OUT_DIR)/ckw.$(OBJ)\
         $(OUT_DIR)/cpj.$(OBJ)\
         $(OUT_DIR)/crg.$(OBJ)\
         $(OUT_DIR)/kwu.$(OBJ)\
         $(OUT_DIR)/lkw.$(OBJ)\
         $(OUT_DIR)/lrg.$(OBJ)\
         $(OUT_DIR)/pju.$(OBJ)\
         $(OUT_DIR)/rgu.$(OBJ)\
         $(OUT_DIR)/rgr.$(OBJ)\
         $(OUT_DIR)/ctf.$(OBJ)\
         $(OUT_DIR)/tfu.$(OBJ)\
         $(OUT_DIR)/lys.$(OBJ)\
         $(OUT_DIR)/lpj.$(OBJ)\
         $(OUT_DIR)/rgm.$(OBJ)\
			$(OUT_DIR)/cm_tenb_stats.$(OBJ)\
#-----------------------------------------------------------------------
# CAN BE MODIFIED
# This section can be used to pass specific options for the layer
# Normally this section should not be used unless absolutely necessary

CCOPTS=$(COPTS) $(POPTS) $(IOPTS)
CPPOPTS=$(COPTS) $(POPTS) $(IOPTS)

#-----------------------------------------------------------------------
# DO NOT EDIT THIS SECTION
# include for the building rules
#
include $(BUILD_ROOT)/build_rule.mak

# DO NOT DELETE THIS LINE -- make depend depends on it.

