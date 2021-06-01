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
#-------------------------------------------------------------#
#Makefile for product CZ - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#
include ./rsys_fancy.mak
COLOR=$(COLOR_YELLOW)

#product specific options should be identified.
CCczFLAGS=-DLCLCZ -DLCSCT -DCZ -DSM 

# Product sources-------------------------------------------------------
czSRCS= \
    $(IN_DIR)/cz_db.$(SRC) \
    $(IN_DIR)/cz_lim.$(SRC) \
    $(IN_DIR)/cz_nmm.$(SRC) \
    $(IN_DIR)/cz_ex_ms.$(SRC) \
    $(IN_DIR)/cz_id.$(SRC) \
    $(IN_DIR)/cz_lmm.$(SRC) \
    $(IN_DIR)/cz_ptli.$(SRC) \
    $(IN_DIR)/cz_ptmi.$(SRC) \
    $(IN_DIR)/cz_ptui.$(SRC) \
    $(IN_DIR)/cz_tmr.$(SRC) \
    $(IN_DIR)/cz_uim.$(SRC) \
    $(IN_DIR)/cz_utl.$(SRC) \
    $(IN_DIR)/cz_tpm.$(SRC) \
    $(IN_DIR)/cz_bmp.$(SRC) \
    $(IN_DIR)/cz_edm.$(SRC) \
    $(IN_DIR)/cz_gpm.$(SRC) \
    $(IN_DIR)/cz_ldm.$(SRC) \


# Product objects-------------------------------------------------------
czOBJS= \
    $(OUT_DIR)/cz_db.$(OBJ) \
    $(OUT_DIR)/cz_lim.$(OBJ) \
    $(OUT_DIR)/cz_nmm.$(OBJ) \
    $(OUT_DIR)/cz_ex_ms.$(OBJ) \
    $(OUT_DIR)/cz_id.$(OBJ) \
    $(OUT_DIR)/cz_lmm.$(OBJ) \
    $(OUT_DIR)/cz_ptli.$(OBJ) \
    $(OUT_DIR)/cz_ptmi.$(OBJ) \
    $(OUT_DIR)/cz_ptui.$(OBJ) \
    $(OUT_DIR)/cz_tmr.$(OBJ) \
    $(OUT_DIR)/cz_uim.$(OBJ) \
    $(OUT_DIR)/cz_utl.$(OBJ) \
    $(OUT_DIR)/cz_tpm.$(OBJ) \
    $(OUT_DIR)/cz_bmp.$(OBJ) \
    $(OUT_DIR)/cz_edm.$(OBJ) \
    $(OUT_DIR)/cz_gpm.$(OBJ) \
    $(OUT_DIR)/cz_ldm.$(OBJ) 


CZCM_INC=$(VS_DIR)/cz.h $(VS_DIR)/cz.x $(VS_DIR)/cz_err.h

ALL_INC=$(CZCM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/cz_db.$(OBJ): $(IN_DIR)/cz_db.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/cz_db.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) \
	$(IN_DIR)/cz_db.$(SRC)
$(OUT_DIR)/cz_lim.$(OBJ): $(IN_DIR)/cz_lim.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_lim.$(SRC) -o \
	$(OUT_DIR)/cz_lim.$(OBJ)
$(OUT_DIR)/cz_nmm.$(OBJ): $(IN_DIR)/cz_nmm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_nmm.$(SRC) -o \
	$(OUT_DIR)/cz_nmm.$(OBJ)
$(OUT_DIR)/cz_ex_ms.$(OBJ): $(IN_DIR)/cz_ex_ms.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_ex_ms.$(SRC) -o \
	$(OUT_DIR)/cz_ex_ms.$(OBJ)
$(OUT_DIR)/cz_id.$(OBJ): $(IN_DIR)/cz_id.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_id.$(SRC) -o \
	$(OUT_DIR)/cz_id.$(OBJ)
$(OUT_DIR)/cz_lmm.$(OBJ): $(IN_DIR)/cz_lmm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_lmm.$(SRC) -o \
	$(OUT_DIR)/cz_lmm.$(OBJ)
$(OUT_DIR)/cz_ptli.$(OBJ): $(IN_DIR)/cz_ptli.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_ptli.$(SRC) -o \
	$(OUT_DIR)/cz_ptli.$(OBJ)
$(OUT_DIR)/cz_ptmi.$(OBJ): $(IN_DIR)/cz_ptmi.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_ptmi.$(SRC) -o \
	$(OUT_DIR)/cz_ptmi.$(OBJ)
$(OUT_DIR)/cz_ptui.$(OBJ): $(IN_DIR)/cz_ptui.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_ptui.$(SRC) -o \
	$(OUT_DIR)/cz_ptui.$(OBJ)
$(OUT_DIR)/cz_tmr.$(OBJ): $(IN_DIR)/cz_tmr.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_tmr.$(SRC) -o \
	$(OUT_DIR)/cz_tmr.$(OBJ)
$(OUT_DIR)/cz_uim.$(OBJ): $(IN_DIR)/cz_uim.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_uim.$(SRC) -o \
	$(OUT_DIR)/cz_uim.$(OBJ)
$(OUT_DIR)/cz_utl.$(OBJ): $(IN_DIR)/cz_utl.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_utl.$(SRC) -o \
	$(OUT_DIR)/cz_utl.$(OBJ)
$(OUT_DIR)/cz_tpm.$(OBJ): $(IN_DIR)/cz_tpm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_tpm.$(SRC) -o \
	$(OUT_DIR)/cz_tpm.$(OBJ)
$(OUT_DIR)/cz_bmp.$(OBJ): $(IN_DIR)/cz_bmp.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_bmp.$(SRC) -o \
	$(OUT_DIR)/cz_bmp.$(OBJ)
$(OUT_DIR)/cz_edm.$(OBJ): $(IN_DIR)/cz_edm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_edm.$(SRC) -o \
	$(OUT_DIR)/cz_edm.$(OBJ)
$(OUT_DIR)/cz_gpm.$(OBJ): $(IN_DIR)/cz_gpm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_gpm.$(SRC) -o \
	$(OUT_DIR)/cz_gpm.$(OBJ)
$(OUT_DIR)/cz_ldm.$(OBJ): $(IN_DIR)/cz_ldm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCczFLAGS) $(IN_DIR)/cz_ldm.$(SRC) -o \
	$(OUT_DIR)/cz_ldm.$(OBJ)

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libcz.a:$(czOBJS)
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(AR) -cr $(OUT_DIR)/libcz.a $(czOBJS)	

$(OUT_DIR)/czprepro:$(czOBJS)

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libcz.a $(czOBJS)

