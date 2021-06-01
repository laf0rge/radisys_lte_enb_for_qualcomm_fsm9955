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
#                    Email: support@trillium.com
#                    Web: http://www.radisys.com 
# 
#*********************************************************************17*/
#
#
include ./rsys_fancy.mak
COLOR=$(COLOR_GREEN)
#-------------------------------------------------------------#
#Makefile for RL
#-------------------------------------------------------------#
rlPOSTPROCOPTS=-DSS_LINUX -DUSE_RLOG_DATA_TYPES 

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

# Product sources-------------------------------------------------------
rlSRCS=$(IN_DIR)/rl_rlog.$(SRC) $(IN_DIR)/rl_common.$(SRC) $(IN_DIR)/rl_platform.$(SRC) $(IN_DIR)/rl_soc.$(SRC)
rlPOSTPROCSRCS=$(IN_DIR)/rl_common.$(SRC) $(IN_DIR)/rl_postproc.cpp

# Product objects-------------------------------------------------------
rlOBJS=$(OUT_DIR)/rl_rlog.$(OBJ) $(OUT_DIR)/rl_common.$(OBJ) $(OUT_DIR)/rl_platform.$(OBJ) $(OUT_DIR)/rl_soc.$(OBJ)

ALL_INC=$(IN_DIR)/rl_common.h  $(IN_DIR)/rl_rlog.h  $(IN_DIR)/rl_interface.h \
		  $(IN_DIR)/rl_platform.h $(IN_DIR)/rl_soc.h

#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
$(OUT_DIR)/rl_rlog.$(OBJ):$(rlSRCS)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) $(COPTS) $(IOPTS) $(POPTS) $(IN_DIR)/rl_rlog.$(SRC) -rdynamic -o $(OUT_DIR)/rl_rlog.$(OBJ)
$(OUT_DIR)/rl_common.$(OBJ):$(rlSRCS)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) $(COPTS) $(IOPTS) $(POPTS) $(IN_DIR)/rl_common.$(SRC) -o $(OUT_DIR)/rl_common.$(OBJ)
$(OUT_DIR)/rl_platform.$(OBJ):$(rlSRCS)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) $(COPTS) $(IOPTS) $(POPTS) $(IN_DIR)/rl_platform.$(SRC) -o $(OUT_DIR)/rl_platform.$(OBJ)
$(OUT_DIR)/rl_soc.$(OBJ):$(rlSRCS)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(CC) $(COPTS) $(IOPTS) $(POPTS) $(IN_DIR)/rl_soc.$(SRC) -o $(OUT_DIR)/rl_soc.$(OBJ)
#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/librl:$(rlOBJS)
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(AR) -cr $(OUT_DIR)/librl.a $(rlOBJS)	
	g++ $(rlPOSTPROCOPTS) $(rlPOSTPROCSRCS) -o	$(OUT_DIR)/rlogapp

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	rm -f $(OUT_DIR)/librl.a $(rlOBJS)



