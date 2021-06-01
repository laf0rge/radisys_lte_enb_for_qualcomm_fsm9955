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
##-------------------------------------------------------------#
#Makefile for product SZ - script generated.
#-------------------------------------------------------------#
include ./rsys_fancy.mak
COLOR=$(COLOR_MAG)
#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCszFLAGS=-DLCLSZ -DLCSCT -DSZ -DSM -US1AP_REL851 -DLSZV1 -DS1AP_REL9A6

# Product sources-------------------------------------------------------
szSRCS= \
    $(IN_DIR)/sz_db.$(SRC) \
    $(IN_DIR)/sz_li.$(SRC) \
    $(IN_DIR)/sz_mgmt.$(SRC) \
    $(IN_DIR)/sz_mi.$(SRC) \
    $(IN_DIR)/sz_msgfn.$(SRC) \
    $(IN_DIR)/sz_ex_ms.$(SRC) \
    $(IN_DIR)/sz_ndb.$(SRC) \
    $(IN_DIR)/sz_id.$(SRC) \
    $(IN_DIR)/sz_sm.$(SRC) \
    $(IN_DIR)/sz_smlienb.$(SRC) \
    $(IN_DIR)/sz_smuienb.$(SRC) \
    $(IN_DIR)/sz_ptli.$(SRC) \
    $(IN_DIR)/sz_ptmi.$(SRC) \
    $(IN_DIR)/sz_ptui.$(SRC) \
    $(IN_DIR)/sz_tmr.$(SRC) \
    $(IN_DIR)/sz_ui.$(SRC) \
    $(IN_DIR)/sz_utils.$(SRC) \
    $(IN_DIR)/sz_3gdb.$(SRC) 


# Product objects-------------------------------------------------------
szOBJS= \
    $(OUT_DIR)/sz_db.$(OBJ) \
    $(OUT_DIR)/sz_li.$(OBJ) \
    $(OUT_DIR)/sz_mgmt.$(OBJ) \
    $(OUT_DIR)/sz_mi.$(OBJ) \
    $(OUT_DIR)/sz_msgfn.$(OBJ) \
    $(OUT_DIR)/sz_ex_ms.$(OBJ) \
    $(OUT_DIR)/sz_ndb.$(OBJ) \
    $(OUT_DIR)/sz_id.$(OBJ) \
    $(OUT_DIR)/sz_sm.$(OBJ) \
    $(OUT_DIR)/sz_smlienb.$(OBJ) \
    $(OUT_DIR)/sz_smuienb.$(OBJ) \
    $(OUT_DIR)/sz_ptli.$(OBJ) \
    $(OUT_DIR)/sz_ptmi.$(OBJ) \
    $(OUT_DIR)/sz_ptui.$(OBJ) \
    $(OUT_DIR)/sz_tmr.$(OBJ) \
    $(OUT_DIR)/sz_ui.$(OBJ) \
    $(OUT_DIR)/sz_utils.$(OBJ) \
    $(OUT_DIR)/sz_3gdb.$(OBJ) 


SZCM_INC=$(VS_DIR)/sz.h $(VS_DIR)/sz.x $(VS_DIR)/sz_err.h

ALL_INC=$(SZCM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/sz_db.$(OBJ): $(IN_DIR)/sz_db.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/sz_db.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) \
	$(IN_DIR)/sz_db.$(SRC)

$(OUT_DIR)/sz_3gdb.$(OBJ): $(IN_DIR)/sz_3gdb.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/sz_3gdb.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) \
	$(IN_DIR)/sz_3gdb.$(SRC)

$(OUT_DIR)/sz_li.$(OBJ): $(IN_DIR)/sz_li.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_li.$(SRC) -o \
	$(OUT_DIR)/sz_li.$(OBJ)
$(OUT_DIR)/sz_mgmt.$(OBJ): $(IN_DIR)/sz_mgmt.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_mgmt.$(SRC) -o \
	$(OUT_DIR)/sz_mgmt.$(OBJ)
$(OUT_DIR)/sz_mi.$(OBJ): $(IN_DIR)/sz_mi.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_mi.$(SRC) -o \
	$(OUT_DIR)/sz_mi.$(OBJ)
$(OUT_DIR)/sz_msgfn.$(OBJ): $(IN_DIR)/sz_msgfn.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_msgfn.$(SRC) -o \
	$(OUT_DIR)/sz_msgfn.$(OBJ)
$(OUT_DIR)/sz_ex_ms.$(OBJ): $(IN_DIR)/sz_ex_ms.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_ex_ms.$(SRC) -o \
	$(OUT_DIR)/sz_ex_ms.$(OBJ)
$(OUT_DIR)/sz_ndb.$(OBJ): $(IN_DIR)/sz_ndb.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_ndb.$(SRC) -o \
	$(OUT_DIR)/sz_ndb.$(OBJ)
$(OUT_DIR)/sz_id.$(OBJ): $(IN_DIR)/sz_id.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_id.$(SRC) -o \
	$(OUT_DIR)/sz_id.$(OBJ)
$(OUT_DIR)/sz_sm.$(OBJ): $(IN_DIR)/sz_sm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_sm.$(SRC) -o \
	$(OUT_DIR)/sz_sm.$(OBJ)
$(OUT_DIR)/sz_smlienb.$(OBJ): $(IN_DIR)/sz_smlienb.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_smlienb.$(SRC) -o \
	$(OUT_DIR)/sz_smlienb.$(OBJ)
$(OUT_DIR)/sz_smuienb.$(OBJ): $(IN_DIR)/sz_smuienb.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_smuienb.$(SRC) -o \
	$(OUT_DIR)/sz_smuienb.$(OBJ)
$(OUT_DIR)/sz_ptli.$(OBJ): $(IN_DIR)/sz_ptli.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_ptli.$(SRC) -o \
	$(OUT_DIR)/sz_ptli.$(OBJ)
$(OUT_DIR)/sz_ptmi.$(OBJ): $(IN_DIR)/sz_ptmi.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_ptmi.$(SRC) -o \
	$(OUT_DIR)/sz_ptmi.$(OBJ)
$(OUT_DIR)/sz_ptui.$(OBJ): $(IN_DIR)/sz_ptui.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_ptui.$(SRC) -o \
	$(OUT_DIR)/sz_ptui.$(OBJ)
$(OUT_DIR)/sz_tmr.$(OBJ): $(IN_DIR)/sz_tmr.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_tmr.$(SRC) -o \
	$(OUT_DIR)/sz_tmr.$(OBJ)
$(OUT_DIR)/sz_ui.$(OBJ): $(IN_DIR)/sz_ui.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_ui.$(SRC) -o \
	$(OUT_DIR)/sz_ui.$(OBJ)
$(OUT_DIR)/sz_utils.$(OBJ): $(IN_DIR)/sz_utils.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCszFLAGS) $(IN_DIR)/sz_utils.$(SRC) -o \
	$(OUT_DIR)/sz_utils.$(OBJ)

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libsz.a:$(szOBJS)
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(AR) -cr $(OUT_DIR)/libsz.a $(szOBJS)	

$(OUT_DIR)/szprepro:$(szOBJS)

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libsz.a $(szOBJS)

