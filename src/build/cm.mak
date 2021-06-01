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
#Makefile for product cm - script generated.
#Only the $(CCcmFLAGS) may be modified.
#-------------------------------------------------------------#
include ./rsys_fancy.mak
COLOR=$(COLOR_GREEN)

#-------------------------------------------------------------#
#User macros (to be modified)
#-------------------------------------------------------------#

CCrmFLAGS=-DLRM_LWLC -DRMU_LWLC -URMU_NPLC -DRGM_LWLC -DLWLCSMRMMILRM -DLCSMRMMILRM -DSM -DRM_INTF
CCnlFlags=-DLWLCSMSONILNL -DNLU_LWLC -DLCSMSONILNL -DNLU_TC -DTCSMSONILNL -DNL
ifeq (${BLDENV}, lnx_e2e_ms)
CCcmFLAGS=-DSS_MT_TMR -DSS_FLOAT  -DRGR_V1 -DLSZV1 $(CCrmFLAGS)  #<---Insert cm specific defines here
else
ifeq (${BLDENV},lnx_ms_withoutdiag)
CCcmFLAGS=-DSS_MT_TMR -DSS_FLOAT  -DRGR_V1 -DLSZV1 $(CCrmFLAGS)  #<---Insert cm specific defines here
else
CCcmFLAGS=-DSS_MT_TMR -DSS_FLOAT  -DRGR_V1 -DLSZV1 -DVE_SB_SCTP $(CCrmFLAGS)#<---Insert cm specific defines here
endif
endif

ifeq (${BLDENV}, lnx_e2e_ms)
CCsmFLAGS=-DHI -DSB -DEG -DWR -DKW -DKW_PDCP -DPJ -DSZ -DYS -DRG -DNH -UWR_SM_LOG_TO_FILE -DRGR_SI_SCH -UTA_NEW -DSI_NEW -DCZ $(CCrmFLAGS) $(CCnlFlags)
else
ifeq (${BLDENV},lnx_ms_withoutdiag)
CCsmFLAGS=-DHI -DSB -DEG -DWR -DKW -DKW_PDCP -DPJ -DSZ -DYS -DRG -DNH -UWR_SM_LOG_TO_FILE -DRGR_SI_SCH -UTA_NEW -DSI_NEW -DCZ $(CCrmFLAGS) $(CCnlFlags)
else
CCsmFLAGS=-DHI -DSB -DEG -DWR -DKW -DKW_PDCP -DPJ -DSZ -DYS -DRG -DNH -UVE_SM_LOG_TO_FILE -DRGR_SI_SCH -DTA_NEW -DSI_NEW -DCZ $(CCrmFLAGS) $(CCnlFlags)
endif
endif
#-------------------------------------------------------------#
#File/Obj macros             
#-------------------------------------------------------------#

cmOBJS= \
	$(OUT_DIR)/ckw.$(OBJ)\
	$(OUT_DIR)/cm_bdy5.$(OBJ)\
	$(OUT_DIR)/sm_bdy1.$(OBJ)\
	$(OUT_DIR)/sm_ex_ms.$(OBJ)\
	$(OUT_DIR)/cm_mem.$(OBJ)\
	$(OUT_DIR)/cm_dns.$(OBJ)\
	$(OUT_DIR)/cm_gen.$(OBJ)\
	$(OUT_DIR)/cm_hash.$(OBJ)\
	$(OUT_DIR)/cm_math.$(OBJ)\
	$(OUT_DIR)/cm_inet.$(OBJ)\
	$(OUT_DIR)/cm_lib.$(OBJ)\
	$(OUT_DIR)/cm_llist.$(OBJ)\
	$(OUT_DIR)/cm_lte.$(OBJ)\
	$(OUT_DIR)/cm_mblk.$(OBJ)\
	$(OUT_DIR)/cm_os.$(OBJ)\
	$(OUT_DIR)/cm_pasn1.$(OBJ)\
	$(OUT_DIR)/cm_pasn2.$(OBJ)\
	$(OUT_DIR)/cm_pasn3.$(OBJ)\
	$(OUT_DIR)/cm_pasn4.$(OBJ)\
	$(OUT_DIR)/cm_tkns.$(OBJ)\
	$(OUT_DIR)/cm_tpt.$(OBJ)\
	$(OUT_DIR)/cpj.$(OBJ)\
	$(OUT_DIR)/crg.$(OBJ)\
	$(OUT_DIR)/kwu.$(OBJ)\
	$(OUT_DIR)/lkw.$(OBJ)\
	$(OUT_DIR)/lnh.$(OBJ)\
	$(OUT_DIR)/lrg.$(OBJ)\
	$(OUT_DIR)/leg.$(OBJ)\
	$(OUT_DIR)/egt.$(OBJ)\
	$(OUT_DIR)/lhi.$(OBJ)\
	$(OUT_DIR)/hit.$(OBJ)\
	$(OUT_DIR)/lsb.$(OBJ)\
	$(OUT_DIR)/sct.$(OBJ)\
	$(OUT_DIR)/lsz.$(OBJ)\
	$(OUT_DIR)/szt.$(OBJ)\
	$(OUT_DIR)/szt_pk.$(OBJ)\
	$(OUT_DIR)/szt_3gpk.$(OBJ)\
	$(OUT_DIR)/szt_unpk.$(OBJ)\
	$(OUT_DIR)/szt_3gunpk.$(OBJ)\
	$(OUT_DIR)/nhu.$(OBJ)\
	$(OUT_DIR)/nhu_pk.$(OBJ)\
	$(OUT_DIR)/nhu_unpk.$(OBJ)\
	$(OUT_DIR)/nhu_3gpk.$(OBJ)\
	$(OUT_DIR)/nhu_3gunpk.$(OBJ)\
	$(OUT_DIR)/rgr.$(OBJ)\
	$(OUT_DIR)/pju.$(OBJ)\
	$(OUT_DIR)/ctf.$(OBJ)\
	$(OUT_DIR)/cm_tenb_stats.$(OBJ)\
	$(OUT_DIR)/lys.$(OBJ)\
	$(OUT_DIR)/lpj.$(OBJ)\
    $(OUT_DIR)/lcz.$(OBJ)\
    $(OUT_DIR)/czt.$(OBJ)\
    $(OUT_DIR)/czt_pk.$(OBJ)\
    $(OUT_DIR)/czt_unpk.$(OBJ)\
    $(OUT_DIR)/rmu_pk.$(OBJ)\
    $(OUT_DIR)/rmu_unpk.$(OBJ)\
    $(OUT_DIR)/rmu.$(OBJ)\
    $(OUT_DIR)/rgm.$(OBJ)\
    $(OUT_DIR)/lrm.$(OBJ)\
    $(OUT_DIR)/nlu_pk.$(OBJ)\
    $(OUT_DIR)/nlu_unpk.$(OBJ)\
    $(OUT_DIR)/lnl.$(OBJ)

EG_DIR=../egtpu
SZ_DIR=../s1ap
CZ_DIR=../x2ap

EGTOPTS=-DEG -DLCEGT -DLCEGUIEGT -DEGTP_U
LEGOPTS=-DLCSMMILEG -DLCLEG -DSM -DEG -DLCEGMILEG -DLCSMEGMILEG -DEGTP_U
LHIOPTS=-DLCHIMILHI -DSM -DLCLHI
HITOPTS=-DLCHIT -DLCSBLIHIT -DLCHIUIHIT -DHI_REL_1_4 -DLCEGLIHIT 
SCTOPTS=-DLCSCT 
LRGOPTS= -DLCSMMILRG -DLCLRG -DSM -DRG -DLCRGMILRG
LSBOPTS=-DLCLSB -DSM -DSB -DLSB4  -DLSB8 -DHI_REL_1_4
LSZOPTS=-DLCLSZ -DLCSCT -DSM -DSZ -DLCSMSZMILSZ  -DLCSZMILSZ -DSZ_ENB
SZTOPTS=-DSZ -DUZ -DLCUZLISZT -DLWLCUZLISZT -DLCSZUISZT -DLCSZT -DPTSZUISZT
LCZOPTS=-DLCLCZ -DLCSCT -DSM -DCZ -DLCSMCZMILCZ  -DLCCZMILCZ 
CZTOPTS=-DCZ -DRS -DLCRSLICZT -DLWLCRSLICZT -DLCCZUICZT -DLCCZT -DPTCZUICZT
RGROPTS=-DRG -DLCWRLIRGR -DLCRGR -DLWLCWRLIRGR -DLCRGUIRGR
CTFOPTS=-DLCCTF -DWR -DLCWRLICTF -DLCYSUICTF
LNHOPTS=-DNH -DSM -DLCLNH -DLCSMMILNH -DLCSMNHMILNH -DLCNHMILNH
NHUOPTS=-DLCNHU -DWR -DNX -DLCWRLINHU -DLCNXLINHU -DLCNHUINHU
LWROPTS=-DWR -DSM -DLCLWR -ULCSMMILWR -ULCWRMILWR -DWR_RELAY -DEU_DAT_APP
LPJOPTS=-DPJ -DSM -DLCLPJ -DLCSMMILPJ -DLCPJMILPJ -DKW_PDCP
LKWOPTS=-DKW -DSM -DLCLKW -DLCSMMILKW -DLCKWMILKW -DKW_PDCP
PJUVEOPTS=-DPJ -DLCPJU -DWR -DNX -DNH -DLCWRLIPJU -DLCPJUIPJU -DLCNHLIPJU
CKWOPTS=-DLCNHLICKW -DLCCKW -DLCKWUICKW -DKW -DNH
CPJOPTS=-DPJ -DLCCPJ -DNH -DLCNHLICPJ -DLCPJUICPJ
CRGOPTS= -DLCNHLICRG -DLCCRG -DLCRGUICRG -DRG
KWUOPTS=-DKW -DNH -DLCKWUIKWU -DLCKWU -DLCNHLIKWU -DCCPU_OPT
RGUOPTS= -DRG -DKW -DLCKWLIRGU -DLCRGU -DLCRGUIRGU -DCCPU_OPT
TFUOPTS= -DTFU_VER_2 -DLCTFU 
LYSOPTS=-DYS -DLCLYS -DLCSMMILYS -DLCYSMILYS

LYSOPTS=-DYS -DLCLYS -DLCSMMILYS -DLCYSMILYS
EGT_INC= \
    $(EG_DIR)/eg.h $(EG_DIR)/eg_util.h $(EG_DIR)/eg_util.x
	
LSZ_INC= \
    $(SZ_DIR)/sz.h $(SZ_DIR)/sz_err.h 
LCZ_INC= \
    $(CZ_DIR)/cz.h $(CZ_DIR)/cz_err.h 
	
EGALL_INC=$(EGT_INC) $(CM_INC)

#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#
$(OUT_DIR)/ckw.$(OBJ): $(IN_DIR)/ckw.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c -o $(OUT_DIR)/ckw.o $(COPTS) $(IOPTS) $(POPTS) $(CKWOPTS) $(CCcmFLAGS) $(IN_DIR)/ckw.c
$(OUT_DIR)/cm_bdy5.$(OBJ): $(IN_DIR)/cm_bdy5.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_bdy5.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_bdy5.c
$(OUT_DIR)/sm_bdy1.$(OBJ): $(IN_DIR)/sm_bdy1.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/sm_bdy1.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(CCsmFLAGS) $(IN_DIR)/sm_bdy1.c
$(OUT_DIR)/sm_ex_ms.$(OBJ): $(IN_DIR)/sm_ex_ms.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/sm_ex_ms.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(CCsmFLAGS) $(IN_DIR)/sm_ex_ms.c
$(OUT_DIR)/cm_mem.$(OBJ): $(IN_DIR)/cm_mem.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_mem.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_mem.c
$(OUT_DIR)/cm_dns.$(OBJ): $(IN_DIR)/cm_dns.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_dns.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_dns.c
$(OUT_DIR)/cm_gen.$(OBJ): $(IN_DIR)/cm_gen.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_gen.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_gen.c
$(OUT_DIR)/cm_hash.$(OBJ): $(IN_DIR)/cm_hash.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_hash.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_hash.c
$(OUT_DIR)/cm_math.$(OBJ): $(IN_DIR)/cm_math.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_math.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_math.c
$(OUT_DIR)/cm_inet.$(OBJ): $(IN_DIR)/cm_inet.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_inet.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_inet.c
$(OUT_DIR)/cm_lib.$(OBJ): $(IN_DIR)/cm_lib.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_lib.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_lib.c
$(OUT_DIR)/cm_llist.$(OBJ): $(IN_DIR)/cm_llist.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_llist.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_llist.c
$(OUT_DIR)/cm_lte.$(OBJ): $(IN_DIR)/cm_lte.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_lte.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_lte.c
$(OUT_DIR)/cm_mblk.$(OBJ): $(IN_DIR)/cm_mblk.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_mblk.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_mblk.c
$(OUT_DIR)/cm_os.$(OBJ): $(IN_DIR)/cm_os.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_os.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_os.c
$(OUT_DIR)/cm_pasn1.$(OBJ): $(IN_DIR)/cm_pasn1.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_pasn1.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_pasn1.c
$(OUT_DIR)/cm_pasn2.$(OBJ): $(IN_DIR)/cm_pasn2.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_pasn2.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_pasn2.c
$(OUT_DIR)/cm_pasn3.$(OBJ): $(IN_DIR)/cm_pasn3.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_pasn3.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_pasn3.c
$(OUT_DIR)/cm_pasn4.$(OBJ): $(IN_DIR)/cm_pasn4.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_pasn4.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_pasn4.c
$(OUT_DIR)/cm_tkns.$(OBJ): $(IN_DIR)/cm_tkns.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_tkns.o $(COPTS) $(IOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_tkns.c
$(OUT_DIR)/cm_tpt.$(OBJ): $(IN_DIR)/cm_tpt.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_tpt.o $(COPTS) $(IOPTS) $(SCTOPTS) $(POPTS) $(CCcmFLAGS) $(IN_DIR)/cm_tpt.c
$(OUT_DIR)/cpj.$(OBJ): $(IN_DIR)/cpj.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cpj.o $(COPTS) $(IOPTS) $(POPTS) $(CPJOPTS) $(CCcmFLAGS) $(IN_DIR)/cpj.c
$(OUT_DIR)/crg.$(OBJ): $(IN_DIR)/crg.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/crg.o $(COPTS) $(IOPTS) $(POPTS) $(CRGOPTS) $(CCcmFLAGS) $(IN_DIR)/crg.c
$(OUT_DIR)/kwu.$(OBJ): $(IN_DIR)/kwu.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/kwu.o $(COPTS) $(IOPTS) $(POPTS) $(KWUOPTS) $(CCcmFLAGS) $(IN_DIR)/kwu.c
$(OUT_DIR)/lkw.$(OBJ): $(IN_DIR)/lkw.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/lkw.o $(COPTS) $(IOPTS) $(POPTS) $(LKWOPTS) $(CCcmFLAGS) $(IN_DIR)/lkw.c
$(OUT_DIR)/lnh.$(OBJ): $(IN_DIR)/lnh.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/lnh.o $(COPTS) $(IOPTS) $(POPTS) $(LNHOPTS) $(CCcmFLAGS) $(IN_DIR)/lnh.c
$(OUT_DIR)/lrg.$(OBJ): $(IN_DIR)/lrg.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/lrg.o $(COPTS) $(IOPTS) $(POPTS) $(LRGOPTS) $(CCcmFLAGS) $(IN_DIR)/lrg.c
$(OUT_DIR)/nhu.$(OBJ): $(IN_DIR)/nhu.c $(CM_INC) 
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/nhu.o $(COPTS) $(IOPTS) -DNX $(POPTS) $(NHUOPTS) $(CCcmFLAGS) $(IN_DIR)/nhu.c
$(OUT_DIR)/nhu_pk.$(OBJ): $(IN_DIR)/nhu_pk.c $(CM_INC) 
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/nhu_pk.o $(COPTS) $(IOPTS) -DNX $(POPTS) $(NHUOPTS) $(CCcmFLAGS) $(IN_DIR)/nhu_pk.c
$(OUT_DIR)/nhu_unpk.$(OBJ): $(IN_DIR)/nhu_unpk.c $(CM_INC) 
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/nhu_unpk.o $(COPTS) $(IOPTS) -DNX $(POPTS) $(NHUOPTS) $(CCcmFLAGS) $(IN_DIR)/nhu_unpk.c
$(OUT_DIR)/nhu_3gpk.$(OBJ): $(IN_DIR)/nhu_3gpk.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/nhu_3gpk.o $(COPTS) $(IOPTS) -DNX $(POPTS) $(NHUOPTS) $(CCcmFLAGS) $(IN_DIR)/nhu_3gpk.c
$(OUT_DIR)/nhu_3gunpk.$(OBJ): $(IN_DIR)/nhu_3gunpk.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/nhu_3gunpk.o $(COPTS) $(IOPTS) -DNX $(POPTS) $(NHUOPTS) $(CCcmFLAGS) $(IN_DIR)/nhu_3gunpk.c

$(OUT_DIR)/pju.$(OBJ): $(IN_DIR)/pju.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/pju.o $(COPTS) $(IOPTS) $(POPTS) $(PJUVEOPTS) $(CCcmFLAGS) $(IN_DIR)/pju.c
$(OUT_DIR)/rgu.$(OBJ): $(IN_DIR)/rgu.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/rgu.o $(COPTS) $(IOPTS) $(POPTS) $(RGUOPTS) $(CCcmFLAGS) $(IN_DIR)/rgu.c
$(OUT_DIR)/rgr.$(OBJ): $(IN_DIR)/rgr.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/rgr.o $(COPTS) $(IOPTS) $(POPTS) $(RGROPTS) $(CCcmFLAGS) $(IN_DIR)/rgr.c
$(OUT_DIR)/ctf.$(OBJ): $(IN_DIR)/ctf.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/ctf.o $(COPTS) $(IOPTS) $(POPTS) $(CTFOPTS) $(CCcmFLAGS) $(IN_DIR)/ctf.c
$(OUT_DIR)/cm_tenb_stats.$(OBJ): $(IN_DIR)/cm_tenb_stats.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/cm_tenb_stats.o $(COPTS) $(IOPTS) $(POPTS) $(CTFOPTS) $(CCcmFLAGS) $(IN_DIR)/cm_tenb_stats.c
$(OUT_DIR)/lpj.$(OBJ): $(IN_DIR)/lpj.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/lpj.o $(COPTS) $(IOPTS) $(POPTS) $(LPJOPTS) $(CCcmFLAGS) $(IN_DIR)/lpj.c
$(OUT_DIR)/lys.$(OBJ): $(IN_DIR)/lys.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/lys.o $(COPTS) $(IOPTS) $(POPTS) $(LYSOPTS) $(CCcmFLAGS) $(IN_DIR)/lys.c
$(OUT_DIR)/leg.$(OBJ): $(IN_DIR)/leg.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/leg.o $(COPTS) $(IOPTS) $(POPTS) $(LEGOPTS) $(CCcmFLAGS) $(IN_DIR)/leg.c
$(OUT_DIR)/egt.$(OBJ): $(IN_DIR)/egt.c $(EGALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/egt.o $(COPTS) $(IOPTS) $(POPTS) $(EGTOPTS) $(CCcmFLAGS) $(IN_DIR)/egt.c
$(OUT_DIR)/lhi.$(OBJ): $(IN_DIR)/lhi.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/lhi.o $(COPTS) $(IOPTS) $(POPTS) $(LHIOPTS) $(CCcmFLAGS) $(IN_DIR)/lhi.c
$(OUT_DIR)/hit.$(OBJ): $(IN_DIR)/hit.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/hit.o $(COPTS) $(IOPTS) $(POPTS) $(HITOPTS) $(CCcmFLAGS) $(IN_DIR)/hit.c
$(OUT_DIR)/lsb.$(OBJ): $(IN_DIR)/lsb.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/lsb.o $(COPTS) $(IOPTS) $(POPTS) $(LSBOPTS) $(CCcmFLAGS) $(IN_DIR)/lsb.c
$(OUT_DIR)/sct.$(OBJ): $(IN_DIR)/sct.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/sct.o $(COPTS) $(IOPTS) $(POPTS) $(SCTOPTS) $(CCcmFLAGS) $(IN_DIR)/sct.c
$(OUT_DIR)/lsz.$(OBJ): $(IN_DIR)/lsz.c $(CM_INC) $(LSZ_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/lsz.o $(COPTS) $(IOPTS) $(POPTS) $(LSZOPTS) $(CCcmFLAGS) $(IN_DIR)/lsz.c
$(OUT_DIR)/szt.$(OBJ): $(IN_DIR)/szt.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/szt.o $(COPTS) $(IOPTS) $(POPTS) $(SZTOPTS) $(CCcmFLAGS) $(IN_DIR)/szt.c
$(OUT_DIR)/szt_pk.$(OBJ): $(IN_DIR)/szt_pk.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/szt_pk.o $(COPTS) $(IOPTS) $(POPTS) $(SZTOPTS) $(CCcmFLAGS) $(IN_DIR)/szt_pk.c
$(OUT_DIR)/szt_3gpk.$(OBJ): $(IN_DIR)/szt_3gpk.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/szt_3gpk.o $(COPTS) $(IOPTS) $(POPTS) $(SZTOPTS) $(CCcmFLAGS) $(IN_DIR)/szt_3gpk.c
$(OUT_DIR)/szt_unpk.$(OBJ): $(IN_DIR)/szt_unpk.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/szt_unpk.o $(COPTS) $(IOPTS) $(POPTS) $(SZTOPTS) $(CCcmFLAGS) $(IN_DIR)/szt_unpk.c
$(OUT_DIR)/szt_3gunpk.$(OBJ): $(IN_DIR)/szt_3gunpk.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/szt_3gunpk.o $(COPTS) $(IOPTS) $(POPTS) $(SZTOPTS) $(CCcmFLAGS) $(IN_DIR)/szt_3gunpk.c

$(OUT_DIR)/lcz.$(OBJ): $(IN_DIR)/lcz.c $(CM_INC) $(LCZ_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/lcz.o $(COPTS) $(IOPTS) $(POPTS) $(LCZOPTS) $(CCcmFLAGS) $(IN_DIR)/lcz.c
$(OUT_DIR)/czt.$(OBJ): $(IN_DIR)/czt.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/czt.o $(COPTS) $(IOPTS) $(POPTS) $(CZTOPTS) $(CCcmFLAGS) $(IN_DIR)/czt.c
$(OUT_DIR)/czt_pk.$(OBJ): $(IN_DIR)/czt_pk.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/czt_pk.o $(COPTS) $(IOPTS) $(POPTS) $(CZTOPTS) $(CCcmFLAGS) $(IN_DIR)/czt_pk.c
$(OUT_DIR)/czt_unpk.$(OBJ): $(IN_DIR)/czt_unpk.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/czt_unpk.o $(COPTS) $(IOPTS) $(POPTS) $(CZTOPTS) $(CCcmFLAGS) $(IN_DIR)/czt_unpk.c
$(OUT_DIR)/rmu_pk.$(OBJ): $(IN_DIR)/rmu_pk.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/rmu_pk.o $(COPTS) $(IOPTS) $(POPTS) $(CZTOPTS) $(CCcmFLAGS) $(IN_DIR)/rmu_pk.c
$(OUT_DIR)/rmu_unpk.$(OBJ): $(IN_DIR)/rmu_unpk.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/rmu_unpk.o $(COPTS) $(IOPTS) $(POPTS) $(CZTOPTS) $(CCcmFLAGS) $(IN_DIR)/rmu_unpk.c

$(OUT_DIR)/rmu.$(OBJ): $(IN_DIR)/rmu.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/rmu.o $(COPTS) $(IOPTS) $(POPTS) $(CZTOPTS) $(CCcmFLAGS) $(IN_DIR)/rmu.c
$(OUT_DIR)/rgm.$(OBJ): $(IN_DIR)/rgm.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/rgm.o $(COPTS) $(IOPTS) $(POPTS) $(CZTOPTS) $(CCcmFLAGS) $(IN_DIR)/rgm.c
$(OUT_DIR)/lrm.$(OBJ): $(IN_DIR)/lrm.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/lrm.o $(COPTS) $(IOPTS) $(POPTS) $(CZTOPTS) $(CCcmFLAGS) $(IN_DIR)/lrm.c

$(OUT_DIR)/lnl.$(OBJ): $(IN_DIR)/lnl.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/lnl.o $(COPTS) $(IOPTS) $(POPTS) $(CZTOPTS) $(CCcmFLAGS) $(IN_DIR)/lnl.c
$(OUT_DIR)/nlu_pk.$(OBJ): $(IN_DIR)/nlu_pk.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/nlu_pk.o $(COPTS) $(IOPTS) $(POPTS) $(CZTOPTS) $(CCcmFLAGS) $(IN_DIR)/nlu_pk.c
$(OUT_DIR)/nlu_unpk.$(OBJ): $(IN_DIR)/nlu_unpk.c $(CM_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c  -o $(OUT_DIR)/nlu_unpk.o $(COPTS) $(IOPTS) $(POPTS) $(CZTOPTS) $(CCcmFLAGS) $(IN_DIR)/nlu_unpk.c

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libcm.a:$(cmOBJS)
	@echo -e "Creating Archive $(COLOR) $@ $(REVERT_COLOR)"
	$(AR) -cr $(OUT_DIR)/libcm.a $(cmOBJS)	
#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libcm.a $(cmOBJS)


