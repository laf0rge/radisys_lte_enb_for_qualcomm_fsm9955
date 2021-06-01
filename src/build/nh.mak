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
#Makefile for product NH - script generated.
#-------------------------------------------------------------#
include ./rsys_fancy.mak
COLOR=$(COLOR_GREEN)

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCnhFLAGS=-UKW -UPJ

#-------------------------------------------------------------#
#File/Obj macros             
#-------------------------------------------------------------#

nhSRCS= \
        $(IN_DIR)/nh_brm.$(SRC) \
        $(VS_DIR)/nh_ccm.h \
        $(IN_DIR)/nh_cgm.$(SRC) \
        $(IN_DIR)/nh_cpm.$(SRC) \
        $(IN_DIR)/nh_db.$(SRC) \
        $(IN_DIR)/nh_3gdb.$(SRC) \
        $(IN_DIR)/nh_dbm.$(SRC)\
        $(IN_DIR)/nh_edm.$(SRC) \
        $(IN_DIR)/nh_ex_ms.$(SRC) \
        $(IN_DIR)/nh_id.$(SRC) \
        $(IN_DIR)/nh_init.$(SRC) \
        $(IN_DIR)/nh_lim.$(SRC) \
        $(IN_DIR)/nh_lmm.$(SRC) \
        $(IN_DIR)/nh_pim.$(SRC) \
        $(IN_DIR)/nh_ptli.$(SRC) \
        $(IN_DIR)/nh_ptmi.$(SRC) \
        $(IN_DIR)/nh_ptui.$(SRC) \
        $(IN_DIR)/nh_rhm.$(SRC) \
        $(IN_DIR)/nh_rtm.$(SRC) \
        $(IN_DIR)/nh_uim.$(SRC) \
        $(IN_DIR)/nh_utl.$(SRC) 

nhOBJS= \
	$(OUT_DIR)/nh_brm.$(OBJ) \
	$(OUT_DIR)/nh_ccm.$(OBJ) \
	$(OUT_DIR)/nh_cgm.$(OBJ) \
	$(OUT_DIR)/nh_cpm.$(OBJ) \
	$(OUT_DIR)/nh_db.$(OBJ) \
	$(OUT_DIR)/nh_3gdb.$(OBJ) \
	$(OUT_DIR)/nh_dbm.$(OBJ) \
	$(OUT_DIR)/nh_edm.$(OBJ) \
	$(OUT_DIR)/nh_ex_ms.$(OBJ) \
	$(OUT_DIR)/nh_id.$(OBJ) \
	$(OUT_DIR)/nh_init.$(OBJ) \
	$(OUT_DIR)/nh_lim.$(OBJ) \
	$(OUT_DIR)/nh_lmm.$(OBJ) \
	$(OUT_DIR)/nh_pim.$(OBJ) \
	$(OUT_DIR)/nh_ptli.$(OBJ) \
	$(OUT_DIR)/nh_ptmi.$(OBJ) \
	$(OUT_DIR)/nh_ptui.$(OBJ) \
	$(OUT_DIR)/nh_rhm.$(OBJ) \
	$(OUT_DIR)/nh_rtm.$(OBJ) \
	$(OUT_DIR)/nh_uim.$(OBJ) \
	$(OUT_DIR)/nh_utl.$(OBJ)
	
NHCM_INC= \
    $(VS_DIR)/nh.h $(VS_DIR)/nh.x $(VS_DIR)/nh_db.x $(VS_DIR)/nh_3gdb.x $(VS_DIR)/nh_env.h

ALL_INC=$(NHCM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
$(OUT_DIR)/nh_brm.$(OBJ): $(IN_DIR)/nh_brm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_brm.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_brm.$(SRC)
$(OUT_DIR)/nh_ccm.$(OBJ): $(IN_DIR)/nh_ccm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_ccm.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_ccm.$(SRC)
$(OUT_DIR)/nh_cgm.$(OBJ): $(IN_DIR)/nh_cgm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_cgm.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_cgm.$(SRC)
$(OUT_DIR)/nh_cpm.$(OBJ): $(IN_DIR)/nh_cpm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_cpm.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_cpm.$(SRC)
$(OUT_DIR)/nh_db.$(OBJ): $(IN_DIR)/nh_db.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_db.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_db.$(SRC)
$(OUT_DIR)/nh_3gdb.$(OBJ): $(IN_DIR)/nh_3gdb.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_3gdb.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_3gdb.$(SRC)

$(OUT_DIR)/nh_dbm.$(OBJ): $(IN_DIR)/nh_dbm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_dbm.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_dbm.$(SRC)
$(OUT_DIR)/nh_edm.$(OBJ): $(IN_DIR)/nh_edm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_edm.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_edm.$(SRC)
$(OUT_DIR)/nh_ex_ms.$(OBJ): $(IN_DIR)/nh_ex_ms.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_ex_ms.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_ex_ms.$(SRC)
$(OUT_DIR)/nh_id.$(OBJ): $(IN_DIR)/nh_id.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_id.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_id.$(SRC)
$(OUT_DIR)/nh_init.$(OBJ): $(IN_DIR)/nh_init.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_init.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_init.$(SRC)
$(OUT_DIR)/nh_lim.$(OBJ): $(IN_DIR)/nh_lim.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_lim.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_lim.$(SRC)
$(OUT_DIR)/nh_lmm.$(OBJ): $(IN_DIR)/nh_lmm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_lmm.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_lmm.$(SRC)
$(OUT_DIR)/nh_pim.$(OBJ): $(IN_DIR)/nh_pim.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_pim.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_pim.$(SRC)
$(OUT_DIR)/nh_ptli.$(OBJ): $(IN_DIR)/nh_ptli.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_ptli.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_ptli.$(SRC)
$(OUT_DIR)/nh_ptmi.$(OBJ): $(IN_DIR)/nh_ptmi.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_ptmi.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_ptmi.$(SRC)
$(OUT_DIR)/nh_ptui.$(OBJ): $(IN_DIR)/nh_ptui.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_ptui.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_ptui.$(SRC)
$(OUT_DIR)/nh_rhm.$(OBJ): $(IN_DIR)/nh_rhm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_rhm.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_rhm.$(SRC)
$(OUT_DIR)/nh_rtm.$(OBJ): $(IN_DIR)/nh_rtm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_rtm.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_rtm.$(SRC)
$(OUT_DIR)/nh_uim.$(OBJ): $(IN_DIR)/nh_uim.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_uim.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_uim.$(SRC)
$(OUT_DIR)/nh_utl.$(OBJ): $(IN_DIR)/nh_utl.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/nh_utl.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCnhFLAGS) \
	$(IN_DIR)/nh_utl.$(SRC)


#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libnh.a:$(nhOBJS)
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(AR) -cr $(OUT_DIR)/libnh.a $(nhOBJS)	

$(OUT_DIR)/nhprepro:$(nhOBJS)

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libnh.a $(nhOBJS)

