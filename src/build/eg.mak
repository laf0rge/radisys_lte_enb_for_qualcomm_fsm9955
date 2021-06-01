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


#-------------------------------------------------------------#
#Makefile for product EG - script generated.
#-------------------------------------------------------------#

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCegFLAGS=-DEGTP_U -DLCHIT -DLCEGLIHIT -DEU

#-------------------------------------------------------------#
#File/Obj macros             
#-------------------------------------------------------------#

egSRCS= \
        $(IN_DIR)/eg_cpm.$(SRC) \
        $(IN_DIR)/eg_edm.$(SRC) \
        $(IN_DIR)/eg_ex_ms.$(SRC) \
        $(IN_DIR)/eg_id.$(SRC) \
        $(IN_DIR)/eg_tunn.$(SRC) \
        $(IN_DIR)/eg_li.$(SRC) \
        $(IN_DIR)/eg_ui.$(SRC) \
        $(IN_DIR)/eg_tpt.$(SRC) \
        $(IN_DIR)/eg_mi.$(SRC) \
        $(IN_DIR)/eg_tmr.$(SRC) \
        $(IN_DIR)/eg_ptli.$(SRC) \
        $(IN_DIR)/eg_ptmi.$(SRC) \
        $(IN_DIR)/eg_util.$(SRC) \
        $(IN_DIR)/eg_ptui.$(SRC) \
		$(IN_DIR)/eg_socklayer.$(SRC)

egOBJS= \
	$(OUT_DIR)/eg_cpm.$(OBJ) \
	$(OUT_DIR)/eg_edm.$(OBJ) \
	$(OUT_DIR)/eg_ex_ms.$(OBJ) \
	$(OUT_DIR)/eg_id.$(OBJ) \
	$(OUT_DIR)/eg_tunn.$(OBJ) \
	$(OUT_DIR)/eg_li.$(OBJ) \
	$(OUT_DIR)/eg_ui.$(OBJ) \
	$(OUT_DIR)/eg_tpt.$(OBJ) \
	$(OUT_DIR)/eg_mi.$(OBJ) \
	$(OUT_DIR)/eg_tmr.$(OBJ) \
	$(OUT_DIR)/eg_ptli.$(OBJ) \
	$(OUT_DIR)/eg_ptmi.$(OBJ) \
	$(OUT_DIR)/eg_util.$(OBJ) \
	$(OUT_DIR)/eg_ptui.$(OBJ) \
	$(OUT_DIR)/eg_socklayer.$(OBJ) 
	
EGCM_INC= \
    $(VS_DIR)/eg.h $(VS_DIR)/eg.x $(VS_DIR)/eg_util.h $(VS_DIR)/eg_util.x

ALL_INC=$(EGCM_INC) $(CM_INC)
   
#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/eg_cpm.$(OBJ): $(IN_DIR)/eg_cpm.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_cpm.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_cpm.$(SRC)
$(OUT_DIR)/eg_edm.$(OBJ): $(IN_DIR)/eg_edm.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_edm.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_edm.$(SRC)
$(OUT_DIR)/eg_ex_ms.$(OBJ): $(IN_DIR)/eg_ex_ms.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_ex_ms.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_ex_ms.$(SRC)
$(OUT_DIR)/eg_id.$(OBJ): $(IN_DIR)/eg_id.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_id.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_id.$(SRC)
$(OUT_DIR)/eg_tunn.$(OBJ): $(IN_DIR)/eg_tunn.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_tunn.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_tunn.$(SRC)
$(OUT_DIR)/eg_li.$(OBJ): $(IN_DIR)/eg_li.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_li.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_li.$(SRC)
$(OUT_DIR)/eg_ui.$(OBJ): $(IN_DIR)/eg_ui.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_ui.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_ui.$(SRC)
$(OUT_DIR)/eg_tpt.$(OBJ): $(IN_DIR)/eg_tpt.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_tpt.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_tpt.$(SRC)
$(OUT_DIR)/eg_mi.$(OBJ): $(IN_DIR)/eg_mi.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_mi.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_mi.$(SRC)
$(OUT_DIR)/eg_tmr.$(OBJ): $(IN_DIR)/eg_tmr.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_tmr.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_tmr.$(SRC)
$(OUT_DIR)/eg_ptli.$(OBJ): $(IN_DIR)/eg_ptli.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_ptli.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_ptli.$(SRC)
$(OUT_DIR)/eg_ptmi.$(OBJ): $(IN_DIR)/eg_ptmi.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_ptmi.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_ptmi.$(SRC)
$(OUT_DIR)/eg_util.$(OBJ): $(IN_DIR)/eg_util.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_util.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_util.$(SRC)
$(OUT_DIR)/eg_ptui.$(OBJ): $(IN_DIR)/eg_ptui.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_ptui.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_ptui.$(SRC)
	
$(OUT_DIR)/eg_socklayer.$(OBJ): $(IN_DIR)/eg_socklayer.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/eg_socklayer.$(OBJ) $(COPTS) $(IOPTS) $(POPTS) $(CCegFLAGS) \
	$(IN_DIR)/eg_socklayer.$(SRC)

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libeg.a:$(egOBJS)
	$(AR) -cr $(OUT_DIR)/libeg.a $(egOBJS)	

$(OUT_DIR)/egprepro:$(egOBJS)

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libeg.a $(egOBJS)

