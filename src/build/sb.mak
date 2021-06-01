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
#Makefile for product SB - script generated.
#-------------------------------------------------------------#
include ./rsys_fancy.mak
COLOR=$(COLOR_MAG)
#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCsbFLAGS=

# Product sources-------------------------------------------------------
sbSRCS= \
    $(IN_DIR)/sb_bdy1.$(SRC) \
    $(IN_DIR)/sb_bdy2.$(SRC) \
    $(IN_DIR)/sb_bdy3.$(SRC) \
    $(IN_DIR)/sb_bdy4.$(SRC) \
    $(IN_DIR)/sb_ex_ms.$(SRC) \
    $(IN_DIR)/sb_id.$(SRC) \
    $(IN_DIR)/sb_ptmi.$(SRC) \
    $(IN_DIR)/sb_ptui.$(SRC) \
    $(IN_DIR)/sb_ptli.$(SRC) \
    $(IN_DIR)/sb_mtu.$(SRC) \
    $(IN_DIR)/sb_port.$(SRC)


# Product objects-------------------------------------------------------
sbOBJS= \
    $(OUT_DIR)/sb_bdy1.$(OBJ) \
    $(OUT_DIR)/sb_bdy2.$(OBJ) \
    $(OUT_DIR)/sb_bdy3.$(OBJ) \
    $(OUT_DIR)/sb_bdy4.$(OBJ) \
    $(OUT_DIR)/sb_ex_ms.$(OBJ) \
    $(OUT_DIR)/sb_id.$(OBJ) \
    $(OUT_DIR)/sb_ptmi.$(OBJ) \
    $(OUT_DIR)/sb_ptui.$(OBJ) \
    $(OUT_DIR)/sb_ptli.$(OBJ) \
    $(OUT_DIR)/sb_mtu.$(OBJ) \
    $(OUT_DIR)/sb_port.$(OBJ) 


SBCM_INC=$(VS_DIR)/sb.h $(VS_DIR)/sb.x $(VS_DIR)/sb_mtu.h $(VS_DIR)/sb_mtu.x $(VS_DIR)/sb_port.h $(VS_DIR)/sb_port.x $(VS_DIR)/sb_err.h

ALL_INC=$(SBCM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/sb_bdy1.$(OBJ): $(IN_DIR)/sb_bdy1.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/sb_bdy1.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) \
	$(IN_DIR)/sb_bdy1.$(SRC)
$(OUT_DIR)/sb_bdy2.$(OBJ): $(IN_DIR)/sb_bdy2.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_bdy2.$(SRC) -o \
	$(OUT_DIR)/sb_bdy2.$(OBJ)
$(OUT_DIR)/sb_bdy3.$(OBJ): $(IN_DIR)/sb_bdy3.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_bdy3.$(SRC) -o \
	$(OUT_DIR)/sb_bdy3.$(OBJ)
$(OUT_DIR)/sb_bdy4.$(OBJ): $(IN_DIR)/sb_bdy4.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_bdy4.$(SRC) -o \
	$(OUT_DIR)/sb_bdy4.$(OBJ)
$(OUT_DIR)/sb_ex_ms.$(OBJ): $(IN_DIR)/sb_ex_ms.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_ex_ms.$(SRC) -o \
	$(OUT_DIR)/sb_ex_ms.$(OBJ)
$(OUT_DIR)/sb_id.$(OBJ): $(IN_DIR)/sb_id.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_id.$(SRC) -o \
	$(OUT_DIR)/sb_id.$(OBJ)
$(OUT_DIR)/sb_ptmi.$(OBJ): $(IN_DIR)/sb_ptmi.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_ptmi.$(SRC) -o \
	$(OUT_DIR)/sb_ptmi.$(OBJ)
$(OUT_DIR)/sb_ptui.$(OBJ): $(IN_DIR)/sb_ptui.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_ptui.$(SRC) -o \
	$(OUT_DIR)/sb_ptui.$(OBJ)
$(OUT_DIR)/sb_ptli.$(OBJ): $(IN_DIR)/sb_ptli.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_ptli.$(SRC) -o \
	$(OUT_DIR)/sb_ptli.$(OBJ)
$(OUT_DIR)/sb_mtu.$(OBJ): $(IN_DIR)/sb_mtu.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_mtu.$(SRC) -o \
	$(OUT_DIR)/sb_mtu.$(OBJ)
$(OUT_DIR)/sb_port.$(OBJ): $(IN_DIR)/sb_port.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  $(COPTS) $(IOPTS) $(POPTS) $(CCsbFLAGS) $(IN_DIR)/sb_port.$(SRC) -o \
	$(OUT_DIR)/sb_port.$(OBJ)

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libsb.a:$(sbOBJS)
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(AR) -cr $(OUT_DIR)/libsb.a $(sbOBJS)	

$(OUT_DIR)/sbprepro:$(sbOBJS)

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libsb.a $(sbOBJS)

