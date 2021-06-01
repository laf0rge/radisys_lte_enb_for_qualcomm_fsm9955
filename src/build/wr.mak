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
##--------------------------------------------------------------#
##Makefile for product WR
##--------------------------------------------------------------#
include ./rsys_fancy.mak
COLOR=$(COLOR_RED)
##--------------------------------------------------------------#
##User macros (to be modified)
#--------------------------------------------------------------#

RrmFlags=-DRM_INTF -DRMU_LWLC -DRM_NPLC -DLWLCSMRMMILRM -UTCSMRMMILRM -DLCSMRMMILRM -DPTSMMILRM -LRM_NPLC

SonFlags=-DNLU_LWLC -DNLU_TC -DLWLCSMSONILNL -DTCSMSONILNL -DLCSMSONILNL
COPTS_NO_WARN=-g -O3 -pipe -pedantic -W -Wno-comment -Wshadow -Wcast-qual -fno-strict-aliasing -fsigned-char --std=c99 -march=mips32r2 -mtune=mips32r2 -mips32r2

ifeq (${BLD},seum)
CCwrFLAGS=-DWR_RELAY -DEU_DAT_APP -DEGTP_U -DNX -DPX -DUZ -DEU -DKW_PDCP -UPTNXLIPJU -UPTWRLIRGR -UTFU_TDD -DTFU_WRR_2 -DLCEGUIEGT -DLCWRLICTF -DLCWRLINHU -DxLCPJUIPJU -DLCWRLIRGR -ULTE_TDD -UTA_NEW -USI_NEW -URGR_SI_SCH -DSS_MT_TMR $(RrmFlags)
else
ifeq (${BLD},picoseum)
CCwrFLAGS=-DWR_RELAY -DEU_DAT_APP -DEGTP_U -DNX -DPX -DUZ -DEU -DKW_PDCP -UPTNXLIPJU -UPTWRLIRGR -UTFU_TDD -DTFU_WRR_2 -DLCEGUIEGT -DLCWRLICTF -DLCWRLINHU -DxLCPJUIPJU -DLCWRLIRGR -ULTE_TDD -UWR_PICO -UWR_WITHOUT_CNE -DWIRESHARK_LOG -UYS_PICO -DSI_NEW -DYS_PICO -DWR_PICO  -DSS_MT_TMR $(RrmFlags)
else
ifeq (${BLDENV},lnx_e2e_ms)
CCwrFLAGS=-DWR_RELAY -DEU_DAT_APP -DEGTP_U -DNX -DPX -DUZ -DEU -DKW_PDCP -DPTNXLIPJU -DPTWRLIRGR -DTFU_WRR_2 -DLCEGUIEGT -DLCWRLICTF -DLCWRLINHU -DxLCPJUIPJU -DLCWRLIRGR  -DLCSMNHMILNH -DDG -UWR_DG_OAM -DVALIDATION_CHK -DWR_TEST_CODE -DWR_TEST_CLI -DENB_OVRLOAD -DMME_LDBAL -DSS_MT_TMR $(RrmFlags) $(SonFlags)
else
ifeq (${BLDENV},lnx_ms_pre)
CCwrFLAGS=-DWR_RELAY -DEU_DAT_APP -DEGTP_U -DNX -DPX -DUZ -DEU -DKW_PDCP -DPTNXLIPJU -DPTWRLIRGR -DTFU_WRR_2 -DLCEGUIEGT -DLCWRLICTF -DLCWRLINHU -DxLCPJUIPJU -DLCWRLIRGR -DLCSMNHMILNH -DDG -UWR_DG_OAM -DVALIDATION_CHK -DWR_TEST_CODE -DWR_TEST_CLI -DENB_OVRLOAD -DMME_LDBAL -DSS_MT_TMR $(RrmFlags) $(SonFlags)
else
ifeq (${BLDENV},lnx_ms_withoutdiag)
CCwrFLAGS=-DWR_RELAY -DEU_DAT_APP -DEGTP_U -DNX -DPX -DUZ -DEU -DKW_PDCP -DPTNXLIPJU -DPTWRLIRGR -DTFU_WRR_2 -DLCEGUIEGT -DLCWRLICTF -DLCWRLINHU -DxLCPJUIPJU -DLCWRLIRGR -DLCSMNHMILNH -DDG -UWR_DG_OAM -DVALIDATION_CHK -DWR_TEST_CODE -DWR_TEST_CLI -DENB_OVRLOAD -DMME_LDBAL -DSS_MT_TMR $(RrmFlags)
else
ifeq ($(BLDENV),lnx_e2e_bc)
CCwrFLAGS=-DWR_RELAY -DEU_DAT_APP -DEGTP_U -DNX -DPX -DUZ -DEU -DKW_PDCP -DPTNXLIPJU -DPTWRLIRGR -DDEBUGP -DTFU_WRR_2 -DLCEGUIEGT -DLCWRLICTF -DLCWRLINHU -DxLCPJUIPJU -DLCWRLIRGR -DLCSMNHMILNH -DDG -DVALIDATION_CHK -DWR_TEST_CODE -DWR_TEST_CLI -DENB_OVRLOAD -DMME_LDBAL $(RrmFlags) $(SonFlags)
else
ifeq ($(BLDENV),lnx_bc_pre)
CCwrFLAGS=-DWR_RELAY -DEU_DAT_APP -DEGTP_U -DNX -DPX -DUZ -DEU -DKW_PDCP -DPTNXLIPJU -DPTWRLIRGR -DDEBUGP -DTFU_WRR_2 -DLCEGUIEGT -DLCWRLICTF -DLCWRLINHU -DxLCPJUIPJU -DLCWRLIRGR -DLCSMNHMILNH -DDG -DVALIDATION_CHK -DWR_TEST_CODE -DWR_TEST_CLI -DENB_OVRLOAD -DMME_LDBAL $(RrmFlags) $(SonFlags)
else
ifeq ($(BLDENV),lnx_e2e_qcom)
CCwrFLAGS=-DWR_RELAY -DEU_DAT_APP -DEGTP_U -DNX -DPX -DUZ -DEU -DKW_PDCP -DPTNXLIPJU -DPTWRLIRGR -DDEBUGP -DTFU_WRR_2 -DLCEGUIEGT -DLCWRLICTF -DLCWRLINHU -DxLCPJUIPJU -DLCWRLIRGR -DLCSMNHMILNH -DDG -DVALIDATION_CHK -DWR_TEST_CODE -DWR_TEST_CLI -DENB_OVRLOAD -DMME_LDBAL $(RrmFlags) $(SonFlags)
else
ifeq ($(BLDENV),lnx_qcom_pre)
CCwrFLAGS=-DWR_RELAY -DEU_DAT_APP -DEGTP_U -DNX -DPX -DUZ -DEU -DKW_PDCP -DPTNXLIPJU -DPTWRLIRGR -DDEBUGP -DTFU_WRR_2 -DLCEGUIEGT -DLCWRLICTF -DLCWRLINHU -DxLCPJUIPJU -DLCWRLIRGR -DLCSMNHMILNH -DDG -DVALIDATION_CHK -DWR_TEST_CODE -DWR_TEST_CLI -DENB_OVRLOAD -DMME_LDBAL $(RrmFlags) $(SonFlags)
else
CCwrFLAGS=-DWR_RELAY -DEU_DAT_APP -DEGTP_U -DNX -DPX -DUZ -DEU -DKW_PDCP -DKW -UPTNXLIPJU -UPTWRLIRGR -DTFU_WRR_2 -DLCEGUIEGT -DLCWRLICTF -DLCWRLINHU -DxLCPJUIPJU -DLCWRLIRGR -DTA_NEW -DSI_NEW -DRGR_SI_SCH -UWR_SM_LOG_TO_FILE -DDG -DVALIDATION_CHK -DWR_OAM_TEST -DWR_TEST_CODE -DWR_TEST_CLI -DENB_OVRLOAD -DMME_LDBAL -DSS_MT_TMR $(RrmFlags) $(SonFlags)
endif
endif
endif
endif
endif
endif
endif
endif
endif

ifeq ($(OAM_ENABLE),OAM_ENABLE)
CCwrFLAGS+= -DWR_RSYS_OAM -DWR_RSYS_KPI -DREM_ENABLE 
endif 
ifeq ($(OAM_ENV),WR_RSYS_OAM)
CCwrFLAGS+= -DWR_RSYS_OAM
else
ifeq ($(OAM_ENV),WR_RSYS_OAM_SRC)
CCwrFLAGS+= -DWR_RSYS_OAM
endif
endif 


CCsmFLAGS=-DHI -DSB -DEG -DWR -DKW -DSZ -DYS -DRG -DPJ -DCZ 
SS_DIR=../mt

QMI_IOTS=-I$(QCOM_SYSROOT)/usr/include/ -I$(QCOM_SYSROOT)/include/

# Product sources-------------------------------------------------------
wrPerfLnxSRCS= \
    $(IN_DIR)/ \
    $(IN_DIR)/


# Product sources-------------------------------------------------------
wrPerfLnxSRCSUU= \
    $(IN_DIR)/ \
    $(IN_DIR)/

wrE2eLnxSRCS= \
    $(IN_DIR)/wr_emm_eom.$(SRC) \
    $(IN_DIR)/wr_umm_meas.$(SRC) \
    $(IN_DIR)/wr_lmm.$(SRC) \
    $(IN_DIR)/wr_emm_neighenb.$(SRC) \
    $(IN_DIR)/wr_emm_cfg.$(SRC) \
    $(IN_DIR)/wr_emm_csg.$(SRC) \
    $(IN_DIR)/wr_emm_ecsfb_cfg.$(SRC) \
    $(IN_DIR)/wr_emm_rim_app.$(SRC) \
    $(IN_DIR)/wr_emm_rim.$(SRC) \
    $(IN_DIR)/wr_umm_rrccsfb.$(SRC) \
    $(IN_DIR)/wr_umm_mobparms.$(SRC) \
    $(IN_DIR)/wr_umm_ulnas.$(SRC) \
    $(IN_DIR)/wr_umm_dlnas.$(SRC) \
    $(IN_DIR)/wr_umm_rabsetup.$(SRC) \
    $(IN_DIR)/wr_umm_ctxtrel.$(SRC) \
    $(IN_DIR)/wr_umm_rabrel.$(SRC) \
    $(IN_DIR)/wr_umm_rabmod.$(SRC) \
    $(IN_DIR)/wr_umm_lcrabrel.$(SRC) \
    $(IN_DIR)/wr_emm.$(SRC) \
    $(IN_DIR)/wr_emm_pbm.$(SRC) \
    $(IN_DIR)/wr_emm_pws.$(SRC) \
    $(IN_DIR)/wr_emm_pbm_sib.$(SRC) \
    $(IN_DIR)/wr_emm_ecsfb_utils.$(SRC) \
    $(IN_DIR)/wr_umm_ecsfb_utils.$(SRC)\
    $(IN_DIR)/wr_emm_cell.$(SRC) \
    $(IN_DIR)/wr_emm_mme.$(SRC) \
    $(IN_DIR)/wr_emm_mme_s1ap.$(SRC) \
    $(IN_DIR)/wr_emm_plmn.$(SRC) \
    $(IN_DIR)/wr_utils.$(SRC) \
    $(IN_DIR)/wr_smm_smallcell.$(SRC) \
    $(IN_DIR)/wr_smm_init.$(SRC) \
    $(IN_DIR)/wr_smm_cfg.$(SRC) \
    $(IN_DIR)/wr_smm_rrc.$(SRC) \
    $(IN_DIR)/wr_smm_rlc.$(SRC) \
    $(IN_DIR)/wr_smm_mac.$(SRC) \
    $(IN_DIR)/wr_smm_s1ap.$(SRC) \
    $(IN_DIR)/wr_smm_sctp.$(SRC) \
    $(IN_DIR)/wr_smm_tucl.$(SRC) \
    $(IN_DIR)/wr_smm_egtp.$(SRC) \
    $(IN_DIR)/wr_smm_enbapp_rsys.$(SRC) \
    $(IN_DIR)/wr_smm_rrm.$(SRC) \
    $(IN_DIR)/wr_smm_watchdog_tmr.$(SRC) \
    $(IN_DIR)/wr_smm_pdcp.$(SRC) \
    $(IN_DIR)/wr_smm_cl.$(SRC) \
    $(IN_DIR)/wr_smm_x2ap.$(SRC) \
    $(IN_DIR)/wr_plat.$(SRC) \
    $(IN_DIR)/wr_ifm_x2ap.$(SRC) \
    $(IN_DIR)/wr_ifm_egtp.$(SRC) \
    $(IN_DIR)/wr_ifm_rrc.$(SRC) \
    $(IN_DIR)/wr_ifm_pdcp.$(SRC) \
    $(IN_DIR)/wr_tenb_stats.$(SRC) \
    $(IN_DIR)/wr_ifm_schd.$(SRC) \
    $(IN_DIR)/wr_ifm_s1ap.$(SRC) \
    $(IN_DIR)/wr_ifm_l1.$(SRC) \
    $(IN_DIR)/wr_umm.$(SRC) \
    $(IN_DIR)/wr_dam.$(SRC) \
    $(IN_DIR)/wr_dam_ifm_app.$(SRC) \
    $(IN_DIR)/wr_ifm_dam.$(SRC) \
    $(IN_DIR)/wr_dam_ex_ms.$(SRC) \
    $(IN_DIR)/wr_dam_tmr.$(SRC) \
    $(IN_DIR)/wr_ifm_dam_utils.$(SRC) \
    $(IN_DIR)/wr_umm_egtp_hdl.$(SRC) \
    $(IN_DIR)/wr_umm_rrc_msg.$(SRC) \
    $(IN_DIR)/wr_umm_sch_msg.$(SRC) \
    $(IN_DIR)/wr_umm_s1ap_msg.$(SRC) \
    $(IN_DIR)/wr_umm_rrccon.$(SRC) \
    $(IN_DIR)/wr_umm_l1_msg.$(SRC) \
    $(IN_DIR)/wr_umm_l1_hdl.$(SRC) \
    $(IN_DIR)/wr_umm_pdcp_hdl.$(SRC) \
    $(IN_DIR)/wr_umm_rrc_hdl.$(SRC) \
    $(IN_DIR)/wr_umm_sch_hdl.$(SRC) \
    $(IN_DIR)/wr_umm_s1ap_hdl.$(SRC) \
    $(IN_DIR)/wr_umm_x2ap_hdl.$(SRC) \
    $(IN_DIR)/wr_tmr.$(SRC) \
    $(IN_DIR)/wr_ex_ms.$(SRC) \
    $(IN_DIR)/wr_lwr.$(SRC) \
    $(IN_DIR)/wr_umm_initcntsetup.$(SRC) \
    $(IN_DIR)/wr_umm_rrcreestab.$(SRC) \
    $(IN_DIR)/wr_umm_ctxtmod.$(SRC) \
    $(IN_DIR)/wr_emm_anr.$(SRC) \
    $(IN_DIR)/wr_emm_ecsfb_anr.$(SRC) \
    $(IN_DIR)/wr_umm_x2srcho.$(SRC) \
    $(IN_DIR)/wr_umm_x2tgtho.$(SRC) \
    $(IN_DIR)/wr_umm_s1srcho.$(SRC) \
    $(IN_DIR)/wr_umm_s1tgtho.$(SRC) \
    $(IN_DIR)/wr_umm_ho.$(SRC) \
    $(IN_DIR)/wr_umm_csfbupdtcell.$(SRC) \
    $(IN_DIR)/wr_umm_csfb.$(SRC)\
    $(IN_DIR)/wr_umm_ecsfb.$(SRC)\
    $(IN_DIR)/wr_diag.$(SRC)    \
    $(IN_DIR)/wr_smm_diag_hdl.$(SRC)\
    $(IN_DIR)/wr_umm_locrpt.$(SRC)\
    $(IN_DIR)/wr_umm_anrmeas.$(SRC)\
    $(IN_DIR)/wr_umm_eventanr.$(SRC)\
    $(IN_DIR)/wr_umm_dlcdma.$(SRC)\
    $(IN_DIR)/wr_kpi.$(SRC)\
    $(IN_DIR)/wr_umm_drx.$(SRC)\
    $(IN_DIR)/wr_nmm.$(SRC)\
    $(IN_DIR)/wr_dbg_log.$(SRC)\
    $(IN_DIR)/wr_smm_tmr.$(SRC)\
    $(IN_DIR)/wr_alarm.$(SRC)\
    $(IN_DIR)/wr_umm_lteadv.$(SRC)\
    $(IN_DIR)/wr_emm_lteadv.$(SRC)\
    $(IN_DIR)/wr_emm_s1_flex.$(SRC)\
    $(IN_DIR)/wr_emm_cnm.$(SRC)\
    $(IN_DIR)/wr_emm_cnm_nlqmi.$(SRC)\
    $(IN_DIR)/wr_smm_son.$(SRC)\
    $(IN_DIR)/wr_smm_son_utils.$(SRC)\
    $(IN_DIR)/wr_ifm_son.$(SRC)\
    $(IN_DIR)/wr_umm_perd_strong_cell_meas.$(SRC)\
    $(IN_DIR)/wr_emm_son.$(SRC)\
    $(IN_DIR)/wr_umm_csfb_geran.$(SRC)\
    $(IN_DIR)/wr_smm_qcom_oam_init.$(SRC)

ifeq ($(OAM_ENABLE),OAM_ENABLE)
wrE2eLnxSRCS1= \
     $(IN_DIR)/wr_smm_enbapp_cmnplatutils.$(SRC) \
     $(IN_DIR)/wr_smm_dyncfg.$(SRC)
else
ifeq ($(OAM_ENV),WR_RSYS_OAM)
wrE2eLnxSRCS1= \
     $(IN_DIR)/wr_smm_enbapp_cmnplatutils.$(SRC)\
     $(IN_DIR)/wr_smm_dyncfg.$(SRC)
else
ifeq ($(OAM_ENV),WR_RSYS_OAM_SRC)
wrE2eLnxSRCS1= \
     $(IN_DIR)/wr_smm_enbapp_cmnplatutils.$(SRC) \
     $(IN_DIR)/wr_smm_dyncfg.$(SRC)
else
wrE2eLnxSRCS1= \
    $(IN_DIR)/wr_smm_enbapp_utils.$(SRC)
endif
endif
endif

# Product objects-------------------------------------------------------
wrE2eLnxOBJS= \
    $(OUT_DIR)/wr_sw_version.$(OBJ) \
    $(OUT_DIR)/wr_emm_eom.$(OBJ) \
    $(OUT_DIR)/wr_umm_meas.$(OBJ) \
    $(OUT_DIR)/wr_umm_anrmeas.$(OBJ) \
    $(OUT_DIR)/wr_umm_eventanr.$(OBJ) \
    $(OUT_DIR)/wr_umm_dlcdma.$(OBJ) \
    $(OUT_DIR)/wr_lmm.$(OBJ) \
    $(OUT_DIR)/wr_emm_neighenb.$(OBJ) \
    $(OUT_DIR)/wr_emm_cfg.$(OBJ) \
    $(OUT_DIR)/wr_emm_csg.$(OBJ) \
    $(OUT_DIR)/wr_emm_ecsfb_cfg.$(OBJ) \
    $(OUT_DIR)/wr_emm_rim_app.$(OBJ) \
    $(OUT_DIR)/wr_emm_rim.$(OBJ) \
    $(OUT_DIR)/wr_utils.$(OBJ) \
    $(OUT_DIR)/wr_dam.$(OBJ) \
    $(OUT_DIR)/wr_dam_ifm_app.$(OBJ) \
    $(OUT_DIR)/wr_ifm_dam.$(OBJ) \
    $(OUT_DIR)/wr_dam_ex_ms.$(OBJ) \
    $(OUT_DIR)/wr_dam_tmr.$(OBJ) \
    $(OUT_DIR)/wr_ifm_dam_utils.$(OBJ) \
    $(OUT_DIR)/wr_umm_rrccsfb.$(OBJ) \
    $(OUT_DIR)/wr_umm_mobparms.$(OBJ) \
    $(OUT_DIR)/wr_umm_ulnas.$(OBJ) \
    $(OUT_DIR)/wr_umm_dlnas.$(OBJ) \
    $(OUT_DIR)/wr_umm_rabsetup.$(OBJ) \
    $(OUT_DIR)/wr_umm_ctxtrel.$(OBJ) \
    $(OUT_DIR)/wr_umm_rabmod.$(OBJ) \
    $(OUT_DIR)/wr_emm_cell.$(OBJ) \
    $(OUT_DIR)/wr_emm_mme.$(OBJ) \
    $(OUT_DIR)/wr_umm_sch_msg.$(OBJ) \
    $(OUT_DIR)/wr_umm_rrc_msg.$(OBJ) \
    $(OUT_DIR)/wr_umm_s1ap_msg.$(OBJ) \
    $(OUT_DIR)/wr_umm_rrccon.$(OBJ) \
    $(OUT_DIR)/wr_umm_l1_msg.$(OBJ) \
    $(OUT_DIR)/wr_emm_mme_s1ap.$(OBJ) \
    $(OUT_DIR)/wr_emm_plmn.$(OBJ) \
    $(OUT_DIR)/wr_emm.$(OBJ) \
    $(OUT_DIR)/wr_emm_pbm.$(OBJ) \
    $(OUT_DIR)/wr_emm_pws.$(OBJ) \
    $(OUT_DIR)/wr_emm_pbm_sib.$(OBJ) \
    $(OUT_DIR)/wr_emm_ecsfb_utils.$(OBJ) \
    $(OUT_DIR)/wr_umm_ecsfb_utils.$(OBJ) \
    $(OUT_DIR)/wr_smm_smallcell.$(OBJ) \
    $(OUT_DIR)/wr_smm_init_merged.$(OBJ)  	\
    $(OUT_DIR)/wr_smm_cl.$(OBJ) \
    $(OUT_DIR)/wr_smm_mac.$(OBJ) \
    $(OUT_DIR)/wr_smm_rlc.$(OBJ) \
    $(OUT_DIR)/wr_smm_pdcp.$(OBJ) \
    $(OUT_DIR)/wr_msm_common.$(OBJ) \
    $(OUT_DIR)/wr_smm_rrc.$(OBJ) \
    $(OUT_DIR)/wr_smm_s1ap.$(OBJ) \
    $(OUT_DIR)/wr_smm_x2ap.$(OBJ) \
    $(OUT_DIR)/wr_plat.$(OBJ) \
    $(OUT_DIR)/wr_smm_egtp.$(OBJ) \
    $(OUT_DIR)/wr_smm_sctp.$(OBJ) \
    $(OUT_DIR)/wr_smm_tucl.$(OBJ) \
    $(OUT_DIR)/wr_ifm_x2ap.$(OBJ) \
    $(OUT_DIR)/wr_ifm_egtp.$(OBJ) \
    $(OUT_DIR)/wr_ifm_rrc.$(OBJ) \
    $(OUT_DIR)/wr_ifm_pdcp.$(OBJ) \
    $(OUT_DIR)/wr_tenb_stats.$(OBJ) \
    $(OUT_DIR)/wr_ifm_schd.$(OBJ) \
    $(OUT_DIR)/wr_ifm_rrm.$(OBJ) \
    $(OUT_DIR)/wr_ptli.$(OBJ) \
    $(OUT_DIR)/wr_ifm_s1ap.$(OBJ) \
    $(OUT_DIR)/wr_ifm_l1.$(OBJ) \
    $(OUT_DIR)/wr_umm.$(OBJ) \
    $(OUT_DIR)/wr_umm_s1ap_hdl.$(OBJ) \
    $(OUT_DIR)/wr_umm_egtp_hdl.$(OBJ) \
    $(OUT_DIR)/wr_umm_l1_hdl.$(OBJ) \
    $(OUT_DIR)/wr_umm_pdcp_hdl.$(OBJ) \
    $(OUT_DIR)/wr_umm_rrc_hdl.$(OBJ) \
    $(OUT_DIR)/wr_umm_sch_hdl.$(OBJ) \
    $(OUT_DIR)/wr_umm_x2ap_hdl.$(OBJ) \
    $(OUT_DIR)/wr_smm_enbapp_rsys.$(OBJ) \
    $(OUT_DIR)/wr_smm_rrm.$(OBJ) \
    $(OUT_DIR)/wr_smm_watchdog_tmr.$(OBJ) \
    $(OUT_DIR)/wr_umm_x2ap_msg.$(OBJ) \
    $(OUT_DIR)/wr_tmr.$(OBJ) \
    $(OUT_DIR)/wr_ptmi.$(OBJ) \
    $(OUT_DIR)/wr_utils.$(OBJ) \
    $(OUT_DIR)/wr_ex_ms.$(OBJ) \
    $(OUT_DIR)/wr_lwr.$(OBJ) \
    $(OUT_DIR)/wr_umm_initcntsetup.$(OBJ) \
    $(OUT_DIR)/wr_umm_lcrabrel.$(OBJ) \
    $(OUT_DIR)/wr_umm_rabrel.$(OBJ) \
    $(OUT_DIR)/wr_umm_rrcreestab.$(OBJ) \
    $(OUT_DIR)/wr_umm_ctxtmod.$(OBJ) \
    $(OUT_DIR)/wr_emm_anr.$(OBJ) \
    $(OUT_DIR)/wr_emm_ecsfb_anr.$(OBJ) \
    $(OUT_DIR)/wr_umm_x2srcho.$(OBJ) \
    $(OUT_DIR)/wr_umm_x2tgtho.$(OBJ) \
    $(OUT_DIR)/wr_umm_s1srcho.$(OBJ) \
    $(OUT_DIR)/wr_umm_s1tgtho.$(OBJ) \
    $(OUT_DIR)/wr_umm_ho.$(OBJ) \
    $(OUT_DIR)/wr_umm_csfbupdtcell.$(OBJ) \
    $(OUT_DIR)/wr_umm_csfb.$(OBJ) \
    $(OUT_DIR)/wr_umm_ecsfb.$(OBJ) \
    $(OUT_DIR)/wr_umm_locrpt.$(OBJ)\
    $(OUT_DIR)/wr_diag.$(OBJ)\
    $(OUT_DIR)/wr_smm_diag_hdl.$(OBJ)\
    $(OUT_DIR)/wr_kpi.$(OBJ)\
    $(OUT_DIR)/wr_umm_drx.$(OBJ)\
    $(OUT_DIR)/wr_nmm.$(OBJ)\
    $(OUT_DIR)/wr_dbg_log.$(OBJ)\
    $(OUT_DIR)/wr_smm_tmr.$(OBJ)\
    $(OUT_DIR)/wr_alarm.$(OBJ)\
    $(OUT_DIR)/wr_umm_lteadv.$(OBJ)\
    $(OUT_DIR)/wr_emm_lteadv.$(OBJ)\
    $(OUT_DIR)/wr_emm_s1_flex.$(OBJ)\
    $(OUT_DIR)/wr_emm_cnm.$(OBJ)\
    $(OUT_DIR)/wr_emm_cnm_nlqmi.$(OBJ)\
    $(OUT_DIR)/wr_smm_son.$(OBJ)\
    $(OUT_DIR)/wr_smm_son_utils.$(OBJ)\
    $(OUT_DIR)/wr_ifm_son.$(OBJ)\
    $(OUT_DIR)/wr_umm_perd_strong_cell_meas.$(OBJ)\
    $(OUT_DIR)/wr_emm_son.$(OBJ)\
    $(OUT_DIR)/wr_umm_csfb_geran.$(OBJ)\
    $(OUT_DIR)/wr_smm_qcom_oam_init.$(OBJ)

ifeq ($(OAM_ENABLE),OAM_ENABLE)
wrE2eLnxOBJS1 =\
     $(OUT_DIR)/wr_smm_enbapp_cmnplatutils.$(OBJ)\
     $(OUT_DIR)/wr_smm_dyncfg.$(OBJ)\
     $(OUT_DIR)/wr_msm_common.$(OBJ)

else
ifeq ($(OAM_ENV),WR_RSYS_OAM)
wrE2eLnxOBJS1 =\
     $(OUT_DIR)/wr_smm_enbapp_cmnplatutils.$(OBJ)\
     $(OUT_DIR)/wr_smm_dyncfg.$(OBJ)\
     $(OUT_DIR)/wr_msm_common.$(OBJ)

else
ifeq ($(OAM_ENV),WR_RSYS_OAM_SRC)
wrE2eLnxOBJS1 =\
     $(OUT_DIR)/wr_smm_enbapp_cmnplatutils.$(OBJ)\
     $(OUT_DIR)/wr_smm_dyncfg.$(OBJ)\
     $(OUT_DIR)/wr_msm_common.$(OBJ)

else
wrE2eLnxOBJS1 =\
     $(OUT_DIR)/wr_smm_enbapp_utils.$(OBJ)
endif
endif
endif

# Product objects-------------------------------------------------------
wrE2eLnxOBJS_ms= \
    $(OUT_DIR)/wr_sw_version.$(OBJ) \
    $(OUT_DIR)/wr_emm_eom.$(OBJ) \
    $(OUT_DIR)/wr_umm_meas.$(OBJ) \
    $(OUT_DIR)/wr_umm_anrmeas.$(OBJ) \
    $(OUT_DIR)/wr_umm_eventanr.$(OBJ) \
    $(OUT_DIR)/wr_umm_dlcdma.$(OBJ) \
    $(OUT_DIR)/wr_lmm.$(OBJ) \
    $(OUT_DIR)/wr_emm_neighenb.$(OBJ) \
    $(OUT_DIR)/wr_emm_cfg.$(OBJ) \
    $(OUT_DIR)/wr_emm_csg.$(OBJ) \
    $(OUT_DIR)/wr_emm_ecsfb_cfg.$(OBJ) \
    $(OUT_DIR)/wr_emm_rim_app.$(OBJ) \
    $(OUT_DIR)/wr_emm_rim.$(OBJ) \
    $(OUT_DIR)/wr_smm_smallcell.$(OBJ) \
    $(OUT_DIR)/wr_utils.$(OBJ) \
    $(OUT_DIR)/wr_dam.$(OBJ) \
    $(OUT_DIR)/wr_dam_ifm_app.$(OBJ) \
    $(OUT_DIR)/wr_ifm_dam.$(OBJ) \
    $(OUT_DIR)/wr_dam_ex_ms.$(OBJ) \
    $(OUT_DIR)/wr_dam_tmr.$(OBJ) \
    $(OUT_DIR)/wr_ifm_dam_utils.$(OBJ) \
    $(OUT_DIR)/wr_umm_rrccsfb.$(OBJ) \
    $(OUT_DIR)/wr_umm_mobparms.$(OBJ) \
    $(OUT_DIR)/wr_umm_ulnas.$(OBJ) \
    $(OUT_DIR)/wr_umm_dlnas.$(OBJ) \
    $(OUT_DIR)/wr_umm_rabsetup.$(OBJ) \
    $(OUT_DIR)/wr_umm_ctxtrel.$(OBJ) \
    $(OUT_DIR)/wr_umm_rabmod.$(OBJ) \
    $(OUT_DIR)/wr_emm_cell.$(OBJ) \
    $(OUT_DIR)/wr_emm_mme.$(OBJ) \
    $(OUT_DIR)/wr_umm_sch_msg.$(OBJ) \
    $(OUT_DIR)/wr_umm_rrc_msg.$(OBJ) \
    $(OUT_DIR)/wr_umm_s1ap_msg.$(OBJ) \
    $(OUT_DIR)/wr_umm_rrccon.$(OBJ) \
    $(OUT_DIR)/wr_umm_l1_msg.$(OBJ) \
    $(OUT_DIR)/wr_emm_mme_s1ap.$(OBJ) \
    $(OUT_DIR)/wr_emm_plmn.$(OBJ) \
    $(OUT_DIR)/wr_emm.$(OBJ) \
    $(OUT_DIR)/wr_emm_pbm.$(OBJ) \
    $(OUT_DIR)/wr_emm_pws.$(OBJ) \
    $(OUT_DIR)/wr_emm_pbm_sib.$(OBJ) \
    $(OUT_DIR)/wr_emm_ecsfb_utils.$(OBJ) \
    $(OUT_DIR)/wr_umm_ecsfb_utils.$(OBJ) \
    $(OUT_DIR)/wr_smm_init.$(OBJ)  	\
    $(OUT_DIR)/wr_smm_cfg.$(OBJ)  	\
    $(OUT_DIR)/wr_smm_cl.$(OBJ) \
    $(OUT_DIR)/wr_smm_mac.$(OBJ) \
    $(OUT_DIR)/wr_smm_rlc.$(OBJ) \
    $(OUT_DIR)/wr_smm_pdcp.$(OBJ) \
    $(OUT_DIR)/wr_smm_rrc.$(OBJ) \
    $(OUT_DIR)/wr_smm_s1ap.$(OBJ) \
    $(OUT_DIR)/wr_smm_x2ap.$(OBJ) \
    $(OUT_DIR)/wr_plat.$(OBJ) \
    $(OUT_DIR)/wr_smm_egtp.$(OBJ) \
    $(OUT_DIR)/wr_smm_sctp.$(OBJ) \
    $(OUT_DIR)/wr_smm_tucl.$(OBJ) \
    $(OUT_DIR)/wr_ifm_x2ap.$(OBJ) \
    $(OUT_DIR)/wr_ifm_egtp.$(OBJ) \
    $(OUT_DIR)/wr_ifm_rrc.$(OBJ) \
    $(OUT_DIR)/wr_ifm_pdcp.$(OBJ) \
    $(OUT_DIR)/wr_tenb_stats.$(OBJ) \
    $(OUT_DIR)/wr_ifm_schd.$(OBJ) \
    $(OUT_DIR)/wr_ifm_rrm.$(OBJ) \
    $(OUT_DIR)/wr_ptli.$(OBJ) \
    $(OUT_DIR)/wr_ifm_s1ap.$(OBJ) \
    $(OUT_DIR)/wr_ifm_l1.$(OBJ) \
    $(OUT_DIR)/wr_umm.$(OBJ) \
    $(OUT_DIR)/wr_umm_s1ap_hdl.$(OBJ) \
    $(OUT_DIR)/wr_umm_egtp_hdl.$(OBJ) \
    $(OUT_DIR)/wr_umm_l1_hdl.$(OBJ) \
    $(OUT_DIR)/wr_umm_pdcp_hdl.$(OBJ) \
    $(OUT_DIR)/wr_umm_rrc_hdl.$(OBJ) \
    $(OUT_DIR)/wr_umm_sch_hdl.$(OBJ) \
    $(OUT_DIR)/wr_umm_x2ap_hdl.$(OBJ) \
    $(OUT_DIR)/wr_smm_enbapp_rsys.$(OBJ) \
    $(OUT_DIR)/wr_smm_rrm.$(OBJ) \
    $(OUT_DIR)/wr_smm_watchdog_tmr.$(OBJ) \
    $(OUT_DIR)/wr_umm_x2ap_msg.$(OBJ) \
    $(OUT_DIR)/wr_tmr.$(OBJ) \
    $(OUT_DIR)/wr_ptmi.$(OBJ) \
    $(OUT_DIR)/wr_utils.$(OBJ) \
    $(OUT_DIR)/wr_ex_ms.$(OBJ) \
    $(OUT_DIR)/wr_lwr.$(OBJ) \
    $(OUT_DIR)/wr_umm_initcntsetup.$(OBJ) \
    $(OUT_DIR)/wr_umm_lcrabrel.$(OBJ) \
    $(OUT_DIR)/wr_umm_rabrel.$(OBJ) \
    $(OUT_DIR)/wr_umm_rrcreestab.$(OBJ) \
    $(OUT_DIR)/wr_umm_ctxtmod.$(OBJ) \
    $(OUT_DIR)/wr_emm_anr.$(OBJ) \
    $(OUT_DIR)/wr_emm_ecsfb_anr.$(OBJ) \
    $(OUT_DIR)/wr_umm_x2srcho.$(OBJ) \
    $(OUT_DIR)/wr_umm_x2tgtho.$(OBJ) \
    $(OUT_DIR)/wr_umm_s1srcho.$(OBJ) \
    $(OUT_DIR)/wr_umm_s1tgtho.$(OBJ) \
    $(OUT_DIR)/wr_umm_ho.$(OBJ) \
    $(OUT_DIR)/wr_umm_csfbupdtcell.$(OBJ) \
    $(OUT_DIR)/wr_umm_csfb.$(OBJ) \
    $(OUT_DIR)/wr_umm_ecsfb.$(OBJ) \
    $(OUT_DIR)/wr_diag.$(OBJ) \
    $(OUT_DIR)/wr_smm_diag_hdl.$(OBJ)\
    $(OUT_DIR)/wr_umm_locrpt.$(OBJ)\
    $(OUT_DIR)/wr_kpi.$(OBJ)\
    $(OUT_DIR)/wr_umm_drx.$(OBJ)\
    $(OUT_DIR)/wr_nmm.$(OBJ)\
    $(OUT_DIR)/wr_dbg_log.$(OBJ)\
    $(OUT_DIR)/wr_smm_tmr.$(OBJ)\
    $(OUT_DIR)/wr_alarm.$(OBJ)\
    $(OUT_DIR)/wr_umm_lteadv.$(OBJ)\
    $(OUT_DIR)/wr_emm_lteadv.$(OBJ)\
    $(OUT_DIR)/wr_emm_s1_flex.$(OBJ)\
    $(OUT_DIR)/wr_emm_cnm.$(OBJ)\
    $(OUT_DIR)/wr_emm_cnm_nlqmi.$(OBJ)\
    $(OUT_DIR)/wr_smm_son.$(OBJ)\
    $(OUT_DIR)/wr_smm_son_utils.$(OBJ)\
    $(OUT_DIR)/wr_ifm_son.$(OBJ)\
    $(OUT_DIR)/wr_umm_perd_strong_cell_meas.$(OBJ)\
    $(OUT_DIR)/wr_emm_son.$(OBJ)\
    $(OUT_DIR)/wr_umm_csfb_geran.$(OBJ)\
    $(OUT_DIR)/wr_smm_qcom_oam_init.$(OBJ)

ifeq ($(OAM_ENABLE),OAM_ENABLE)
wrE2eLnxOBJS_ms1 =\
     $(OUT_DIR)/wr_smm_enbapp_cmnplatutils.$(OBJ)\
     $(OUT_DIR)/wr_smm_dyncfg.$(OBJ)\
     $(OUT_DIR)/wr_msm_common.$(OBJ)

else
ifeq ($(OAM_ENV),WR_RSYS_OAM)
wrE2eLnxOBJS_ms1 =\
     $(OUT_DIR)/wr_smm_enbapp_cmnplatutils.$(OBJ)\
     $(OUT_DIR)/wr_smm_dyncfg.$(OBJ)\
     $(OUT_DIR)/wr_msm_common.$(OBJ)

else
ifeq ($(OAM_ENV),WR_RSYS_OAM_SRC)
wrE2eLnxOBJS_ms1 =\
     $(OUT_DIR)/wr_smm_enbapp_cmnplatutils.$(OBJ)\
     $(OUT_DIR)/wr_smm_dyncfg.$(OBJ)\
     $(OUT_DIR)/wr_msm_common.$(OBJ)

else
wrE2eLnxOBJS_ms1 =\
     $(OUT_DIR)/wr_smm_enbapp_utils.$(OBJ)
endif
endif
endif

#feq (($(BLDENV),lnx_e2e_ms) || ($(BLDENV),lnx_e2e_ms_blog))
#ifeq ($(BLDENV),$(filter $(BLDENV),lnx_e2e_ms lnx_e2e_ms_blog))
SEC_DIR= ../mt/security
SECSRC=c
wrE2eLnxOBJS_ms += $(OUT_DIR)/hmac.$(OBJ) \
                   $(OUT_DIR)/hmacsha256.$(OBJ) \
                   $(OUT_DIR)/mp.$(OBJ) \
                   $(OUT_DIR)/endianness.$(OBJ) \
                   $(OUT_DIR)/sha256.$(OBJ) \
                   $(OUT_DIR)/sha2k32.$(OBJ) 
wrE2eLnxSRCS += $(SEC_DIR)/hmac.$(SECSRC) \
                $(SEC_DIR)/hmacsha256.$(SECSRC) \
                $(SEC_DIR)/mp.$(SECSRC) \
                $(SEC_DIR)/endianness.$(SECSRC) \
                $(SEC_DIR)/sha256.$(SECSRC) \
                $(SEC_DIR)/sha2k32.$(SECSRC) 
#endif

wrPerfLnxOBJS= \
    $(OUT_DIR)/ \
    $(OUT_DIR)/

wrPerfLnxOBJSUU= \
    $(OUT_DIR)/ \
    $(OUT_DIR)/
    
wrPerfCavSRCS= \
    $(IN_DIR)/ \
    $(IN_DIR)/

wrE2eCavSRCS= \
    $(IN_DIR)/ \
    $(IN_DIR)/


# Product objects-------------------------------------------------------
wrE2eCavOBJS= \
    $(OUT_DIR)/ \
    $(OUT_DIR)/

wrPerfCavOBJS= \
    $(OUT_DIR)/ \
    $(OUT_DIR)/
 
# Product sources-------------------------------------------------------
wrPerfCavSRCSUU= \
    $(IN_DIR)/ \
    $(IN_DIR)/

wrPerfCavOBJSUU=\
    $(OUT_DIR)/ \
    $(OUT_DIR)/

WRCM_INC=$(VS_DIR)/wr.h $(SEC_DIR)/hmacsha256.h

QCOM_INC += -I$(QCOM_OAM_SRC_DIR)/libs/include
QCOM_INC += -I$(QCOM_OAM_SRC_DIR)/libs/oamProtocol/include
QCOM_INC += -I$(QCOM_OAM_SRC_DIR)/libs/oamClientProtocol/include
QCOM_INC += -I$(QCOM_OAM_SRC_DIR)/libs/mgmtXml/include
QCOM_INC += -I$(QCOM_OAM_SRC_DIR)/apps/include
QCOM_INC += -I$(QCOM_SYSROOT)/usr/include/libxml2
QCOM_INC += -I$(QCOM_SYSROOT)/include/oam/
QCOM_INC += -I$(QCOM_SYSROOT)/usr/include/
QCOM_INC += -I$(QCOM_SYSROOT)/include/


ALL_INC=$(WRCM_INC) $(CM_INC) $(RL_INC)

#-------------------------------------------------------------#
#-------------------------------------------------------------#
#Compiler macros             
#-------------------------------------------------------------#

$(OUT_DIR)/wr_sw_version.$(OBJ): $(IN_DIR)/wr_sw_version.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_sw_version.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_sw_version.$(SRC)
		  
$(OUT_DIR)/wr_emm_eom.$(OBJ): $(IN_DIR)/wr_emm_eom.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_eom.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_emm_eom.$(SRC)
		  
$(OUT_DIR)/wr_umm_meas.$(OBJ): $(IN_DIR)/wr_umm_meas.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_meas.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_umm_meas.$(SRC)

$(OUT_DIR)/wr_umm_ecsfb_meas.$(OBJ): $(IN_DIR)/wr_umm_ecsfb_meas.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_ecsfb_meas.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_umm_ecsfb_meas.$(SRC)

$(OUT_DIR)/wr_umm_anrmeas.$(OBJ): $(IN_DIR)/wr_umm_anrmeas.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c -o$(OUT_DIR)/wr_umm_anrmeas.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_umm_anrmeas.$(SRC)
		  
$(OUT_DIR)/wr_umm_eventanr.$(OBJ): $(IN_DIR)/wr_umm_eventanr.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c -o$(OUT_DIR)/wr_umm_eventanr.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_umm_eventanr.$(SRC)

$(OUT_DIR)/wr_umm_dlcdma.$(OBJ): $(IN_DIR)/wr_umm_dlcdma.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c -o$(OUT_DIR)/wr_umm_dlcdma.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_umm_dlcdma.$(SRC)

$(OUT_DIR)/wr_lmm.$(OBJ): $(IN_DIR)/wr_lmm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_lmm.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_lmm.$(SRC)

$(OUT_DIR)/wr_emm_neighenb.$(OBJ): $(IN_DIR)/wr_emm_neighenb.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_neighenb.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_emm_neighenb.$(SRC)

$(OUT_DIR)/wr_emm_cfg.$(OBJ): $(IN_DIR)/wr_emm_cfg.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_cfg.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_emm_cfg.$(SRC)

$(OUT_DIR)/wr_emm_csg.$(OBJ): $(IN_DIR)/wr_emm_csg.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_csg.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_emm_csg.$(SRC)

$(OUT_DIR)/wr_emm_ecsfb_cfg.$(OBJ): $(IN_DIR)/wr_emm_ecsfb_cfg.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_ecsfb_cfg.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_emm_ecsfb_cfg.$(SRC)

$(OUT_DIR)/wr_emm_rim_app.$(OBJ): $(IN_DIR)/wr_emm_rim_app.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_rim_app.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_emm_rim_app.$(SRC)

$(OUT_DIR)/wr_emm_rim.$(OBJ): $(IN_DIR)/wr_emm_rim.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_rim.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_emm_rim.$(SRC)

$(OUT_DIR)/wr_smm_smallcell.$(OBJ): $(IN_DIR)/wr_smm_smallcell.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_smm_smallcell.$(OBJ) -I$(SS_DIR) $(QCOM_INC) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_smm_smallcell.$(SRC)

$(OUT_DIR)/wr_umm_rrccsfb.$(OBJ): $(IN_DIR)/wr_umm_rrccsfb.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_rrccsfb.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_rrccsfb.$(SRC)

$(OUT_DIR)/wr_umm_mobparms.$(OBJ): $(IN_DIR)/wr_umm_mobparms.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_mobparms.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_mobparms.$(SRC)

$(OUT_DIR)/wr_umm_ulnas.$(OBJ): $(IN_DIR)/wr_umm_ulnas.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_ulnas.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_ulnas.$(SRC)

$(OUT_DIR)/wr_umm_s1ap_msg.$(OBJ): $(IN_DIR)/wr_umm_s1ap_msg.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_s1ap_msg.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_s1ap_msg.$(SRC)

$(OUT_DIR)/wr_umm_dlnas.$(OBJ): $(IN_DIR)/wr_umm_dlnas.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_dlnas.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_dlnas.$(SRC)

$(OUT_DIR)/wr_umm_rabsetup.$(OBJ): $(IN_DIR)/wr_umm_rabsetup.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_rabsetup.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_rabsetup.$(SRC)

$(OUT_DIR)/wr_umm_ctxtrel.$(OBJ): $(IN_DIR)/wr_umm_ctxtrel.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_ctxtrel.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_ctxtrel.$(SRC)

$(OUT_DIR)/wr_umm_rabrel.$(OBJ): $(IN_DIR)/wr_umm_rabrel.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_rabrel.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_rabrel.$(SRC)

$(OUT_DIR)/wr_umm_rabmod.$(OBJ): $(IN_DIR)/wr_umm_rabmod.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_rabmod.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_rabmod.$(SRC)

$(OUT_DIR)/wr_umm_lcrabrel.$(OBJ): $(IN_DIR)/wr_umm_lcrabrel.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_lcrabrel.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_lcrabrel.$(SRC)

$(OUT_DIR)/wr_umm_rrccon.$(OBJ): $(IN_DIR)/wr_umm_rrccon.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_rrccon.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_rrccon.$(SRC)

$(OUT_DIR)/wr_emm_cell.$(OBJ): $(IN_DIR)/wr_emm_cell.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_cell.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_emm_cell.$(SRC)

$(OUT_DIR)/wr_emm_pbm.$(OBJ): $(IN_DIR)/wr_emm_pbm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_pbm.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_emm_pbm.$(SRC)

$(OUT_DIR)/wr_emm_pws.$(OBJ): $(IN_DIR)/wr_emm_pws.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_pws.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_emm_pws.$(SRC)

$(OUT_DIR)/wr_emm_mme.$(OBJ): $(IN_DIR)/wr_emm_mme.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_mme.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_emm_mme.$(SRC)

$(OUT_DIR)/wr_utils.$(OBJ): $(IN_DIR)/wr_utils.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_utils.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_utils.$(SRC)

$(OUT_DIR)/wr_emm_mme_s1ap.$(OBJ): $(IN_DIR)/wr_emm_mme_s1ap.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_mme_s1ap.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_emm_mme_s1ap.$(SRC) 

$(OUT_DIR)/wr_umm_rrc_msg.$(OBJ): $(IN_DIR)/wr_umm_rrc_msg.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_rrc_msg.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_rrc_msg.$(SRC)

$(OUT_DIR)/wr_umm_l1_msg.$(OBJ): $(IN_DIR)/wr_umm_l1_msg.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_l1_msg.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_l1_msg.$(SRC)

$(OUT_DIR)/wr_umm_sch_msg.$(OBJ): $(IN_DIR)/wr_umm_sch_msg.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_sch_msg.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_sch_msg.$(SRC)

$(OUT_DIR)/wr_emm_plmn.$(OBJ): $(IN_DIR)/wr_emm_plmn.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_plmn.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_emm_plmn.$(SRC)

$(OUT_DIR)/wr_emm_pbm_sib.$(OBJ): $(IN_DIR)/wr_emm_pbm_sib.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_pbm_sib.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_emm_pbm_sib.$(SRC)

$(OUT_DIR)/wr_emm_ecsfb_utils.$(OBJ): $(IN_DIR)/wr_emm_ecsfb_utils.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_ecsfb_utils.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_emm_ecsfb_utils.$(SRC)

$(OUT_DIR)/wr_umm_ecsfb_utils.$(OBJ): $(IN_DIR)/wr_umm_ecsfb_utils.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c -o$(OUT_DIR)/wr_umm_ecsfb_utils.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
        $(IN_DIR)/wr_umm_ecsfb_utils.$(SRC)

$(OUT_DIR)/wr_umm.$(OBJ): $(IN_DIR)/wr_umm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm.$(SRC)

$(OUT_DIR)/wr_dam_ifm_app.$(OBJ): $(IN_DIR)/wr_dam_ifm_app.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_dam_ifm_app.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_dam_ifm_app.$(SRC)

$(OUT_DIR)/wr_dam.$(OBJ): $(IN_DIR)/wr_dam.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_dam.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_dam.$(SRC)

$(OUT_DIR)/wr_nmm.$(OBJ): $(IN_DIR)/wr_nmm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_nmm.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_nmm.$(SRC)

$(OUT_DIR)/wr_dbg_log.$(OBJ): $(IN_DIR)/wr_dbg_log.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_dbg_log.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_dbg_log.$(SRC)

$(OUT_DIR)/wr_ifm_dam.$(OBJ): $(IN_DIR)/wr_ifm_dam.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_ifm_dam.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_ifm_dam.$(SRC)

$(OUT_DIR)/wr_dam_ex_ms.$(OBJ): $(IN_DIR)/wr_dam_ex_ms.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_dam_ex_ms.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_dam_ex_ms.$(SRC)

$(OUT_DIR)/wr_dam_tmr.$(OBJ): $(IN_DIR)/wr_dam_tmr.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_dam_tmr.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_dam_tmr.$(SRC)

$(OUT_DIR)/wr_ifm_dam_utils.$(OBJ): $(IN_DIR)/wr_ifm_dam_utils.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_ifm_dam_utils.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_ifm_dam_utils.$(SRC)

$(OUT_DIR)/wr_smm_init.$(OBJ): $(IN_DIR)/wr_smm_init.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_smm_init.$(OBJ) -I$(SS_DIR) $(QCOM_INC) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_smm_init.$(SRC)

$(OUT_DIR)/wr_smm_cfg.$(OBJ): $(IN_DIR)/wr_smm_cfg.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_smm_cfg.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_smm_cfg.$(SRC)

$(OUT_DIR)/wr_smm_init_merged.$(OBJ): $(IN_DIR)/wr_smm_init_merged.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_smm_init_merged.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_smm_init_merged.$(SRC)

$(OUT_DIR)/wr_smm_rrc.$(OBJ): $(IN_DIR)/wr_smm_rrc.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCSMNHMILNH -DLCLNH $(IN_DIR)/wr_smm_rrc.$(SRC) -o \
	$(OUT_DIR)/wr_smm_rrc.$(OBJ)

$(OUT_DIR)/wr_smm_rlc.$(OBJ): $(IN_DIR)/wr_smm_rlc.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCKWMILKW -DLCPJMILPJ -DLCLKW $(IN_DIR)/wr_smm_rlc.$(SRC) -o \
	$(OUT_DIR)/wr_smm_rlc.$(OBJ)

$(OUT_DIR)/wr_smm_mac.$(OBJ): $(IN_DIR)/wr_smm_mac.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCRGMILRG  -DLCLRG -DLCSMMILRG $(IN_DIR)/wr_smm_mac.$(SRC) -o $(OUT_DIR)/wr_smm_mac.$(OBJ)

$(OUT_DIR)/wr_smm_s1ap.$(OBJ): $(IN_DIR)/wr_smm_s1ap.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCSMMILSZ -DLCLSZ -DSZ_ENB $(IN_DIR)/wr_smm_s1ap.$(SRC) -o \
	$(OUT_DIR)/wr_smm_s1ap.$(OBJ)

$(OUT_DIR)/wr_smm_x2ap.$(OBJ): $(IN_DIR)/wr_smm_x2ap.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCSMMILCZ -DLCLCZ -DCZ_ENB $(IN_DIR)/wr_smm_x2ap.$(SRC) -o \
	$(OUT_DIR)/wr_smm_x2ap.$(OBJ)

$(OUT_DIR)/wr_plat.$(OBJ): $(IN_DIR)/wr_plat.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) -I$(SEC_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCSMMILCZ -DLCLCZ -DCZ_ENB $(IN_DIR)/wr_plat.$(SRC) -o \
	$(OUT_DIR)/wr_plat.$(OBJ)

$(OUT_DIR)/wr_smm_sctp.$(OBJ): $(IN_DIR)/wr_smm_sctp.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCSMSBMILSB -DLCLSB -DLSB4 -DLSB8 $(IN_DIR)/wr_smm_sctp.$(SRC) -o \
	$(OUT_DIR)/wr_smm_sctp.$(OBJ)

$(OUT_DIR)/wr_smm_tucl.$(OBJ): $(IN_DIR)/wr_smm_tucl.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCSMHIMILHI $(IN_DIR)/wr_smm_tucl.$(SRC) -o \
	$(OUT_DIR)/wr_smm_tucl.$(OBJ)

$(OUT_DIR)/wr_smm_egtp.$(OBJ): $(IN_DIR)/wr_smm_egtp.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCSMMILEG -DLCLEG $(IN_DIR)/wr_smm_egtp.$(SRC) -o \
	$(OUT_DIR)/wr_smm_egtp.$(OBJ)

$(OUT_DIR)/wr_smm_enbapp_rsys.$(OBJ): $(IN_DIR)/wr_smm_enbapp_rsys.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLWLCSMMILWR  -DLWLCSWR -DLWLCWRMILWR -DSM -UPTSMMILWR $(IN_DIR)/wr_smm_enbapp_rsys.$(SRC) -o\
	$(OUT_DIR)/wr_smm_enbapp_rsys.$(OBJ)

$(OUT_DIR)/wr_smm_watchdog_tmr.$(OBJ): $(IN_DIR)/wr_smm_watchdog_tmr.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLWLCSMMILWR  -DLWLCSWR -DLWLCWRMILWR -DSM -UPTSMMILWR $(IN_DIR)/wr_smm_watchdog_tmr.$(SRC) -o\
	$(OUT_DIR)/wr_smm_watchdog_tmr.$(OBJ)

$(OUT_DIR)/wr_smm_enbapp_utils.$(OBJ): $(IN_DIR)/wr_smm_enbapp_utils.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLWLCSMMILWR  -DLWLCSWR -DLWLCWRMILWR -DSM $(IN_DIR)/wr_smm_enbapp_utils.$(SRC) -o \
	$(OUT_DIR)/wr_smm_enbapp_utils.$(OBJ)

$(OUT_DIR)/wr_smm_enbapp_cmnplatutils.$(OBJ): $(IN_DIR)/wr_smm_enbapp_cmnplatutils.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(QCOM_INC) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLWLCSMMILWR  -DLWLCSWR -DLWLCWRMILWR -DSM $(IN_DIR)/wr_smm_enbapp_cmnplatutils.$(SRC) -o \
	$(OUT_DIR)/wr_smm_enbapp_cmnplatutils.$(OBJ)

$(OUT_DIR)/wr_msm_common.$(OBJ): $(IN_DIR)/wr_msm_common.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLWLCSMMILWR  -DLWLCSWR -DLWLCWRMILWR -DSM $(IN_DIR)/wr_msm_common.$(SRC) -o \
	$(OUT_DIR)/wr_msm_common.$(OBJ)

$(OUT_DIR)/wr_smm_dyncfg.$(OBJ): $(IN_DIR)/wr_smm_dyncfg.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLWLCSMMILWR  -DLWLCSWR -DLWLCWRMILWR -DSM $(IN_DIR)/wr_smm_dyncfg.$(SRC) -o \
	$(OUT_DIR)/wr_smm_dyncfg.$(OBJ)

$(OUT_DIR)/wr_smm_rrm.$(OBJ): $(IN_DIR)/wr_smm_rrm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -ULCSMMILWR  -ULCSWR -ULCWRMILWR  $(IN_DIR)/wr_smm_rrm.$(SRC) -o\
	$(OUT_DIR)/wr_smm_rrm.$(OBJ)
$(OUT_DIR)/wr_smm_son.$(OBJ): $(IN_DIR)/wr_smm_son.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) $(IN_DIR)/wr_smm_son.$(SRC) -o\
	$(OUT_DIR)/wr_smm_son.$(OBJ)

$(OUT_DIR)/wr_smm_son_utils.$(OBJ): $(IN_DIR)/wr_smm_son_utils.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) $(IN_DIR)/wr_smm_son_utils.$(SRC) -o\
	$(OUT_DIR)/wr_smm_son_utils.$(OBJ)

$(OUT_DIR)/wr_smm_pdcp.$(OBJ): $(IN_DIR)/wr_smm_pdcp.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCSMMILPJ -DLCPJMILPJ -DLCLPJ $(IN_DIR)/wr_smm_pdcp.$(SRC) -o \
	$(OUT_DIR)/wr_smm_pdcp.$(OBJ)

$(OUT_DIR)/wr_smm_cl.$(OBJ): $(IN_DIR)/wr_smm_cl.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/wr_smm_cl.$(SRC) -o \
	$(OUT_DIR)/wr_smm_cl.$(OBJ)

$(OUT_DIR)/wr_emm.$(OBJ): $(IN_DIR)/wr_emm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/wr_emm.$(SRC) -o \
	$(OUT_DIR)/wr_emm.$(OBJ)

$(OUT_DIR)/wr_ifm_x2ap.$(OBJ): $(IN_DIR)/wr_ifm_x2ap.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_ifm_x2ap.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_ifm_x2ap.$(SRC)

$(OUT_DIR)/wr_ifm_egtp.$(OBJ): $(IN_DIR)/wr_ifm_egtp.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_ifm_egtp.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_ifm_egtp.$(SRC)

$(OUT_DIR)/wr_ifm_rrc.$(OBJ): $(IN_DIR)/wr_ifm_rrc.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_ifm_rrc.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_ifm_rrc.$(SRC)

$(OUT_DIR)/wr_ifm_pdcp.$(OBJ): $(IN_DIR)/wr_ifm_pdcp.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_ifm_pdcp.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_ifm_pdcp.$(SRC)

$(OUT_DIR)/wr_tenb_stats.$(OBJ): $(IN_DIR)/wr_tenb_stats.$(SRC) $(ALL_INC)
	$(CC)  -o$(OUT_DIR)/wr_tenb_stats.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_tenb_stats.$(SRC)

$(OUT_DIR)/wr_ifm_schd.$(OBJ): $(IN_DIR)/wr_ifm_schd.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_ifm_schd.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_ifm_schd.$(SRC)

$(OUT_DIR)/wr_ifm_rrm.$(OBJ): $(IN_DIR)/wr_ifm_rrm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c -o$(OUT_DIR)/wr_ifm_rrm.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_ifm_rrm.$(SRC)

$(OUT_DIR)/wr_ifm_son.$(OBJ): $(IN_DIR)/wr_ifm_son.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c -o$(OUT_DIR)/wr_ifm_son.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_ifm_son.$(SRC)

$(OUT_DIR)/wr_emm_son.$(OBJ): $(IN_DIR)/wr_emm_son.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c -o$(OUT_DIR)/wr_emm_son.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_emm_son.$(SRC)

$(OUT_DIR)/wr_ptli.$(OBJ): $(IN_DIR)/wr_ptli.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c -o$(OUT_DIR)/wr_ptli.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_ptli.$(SRC)

$(OUT_DIR)/wr_ifm_s1ap.$(OBJ): $(IN_DIR)/wr_ifm_s1ap.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC) -c -o$(OUT_DIR)/wr_ifm_s1ap.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_ifm_s1ap.$(SRC)

$(OUT_DIR)/wr_ifm_l1.$(OBJ): $(IN_DIR)/wr_ifm_l1.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_ifm_l1.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_ifm_l1.$(SRC)

$(OUT_DIR)/wr_umm_egtp_hdl.$(OBJ): $(IN_DIR)/wr_umm_egtp_hdl.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_egtp_hdl.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_egtp_hdl.$(SRC)

$(OUT_DIR)/wr_umm_l1_hdl.$(OBJ): $(IN_DIR)/wr_umm_l1_hdl.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_l1_hdl.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_l1_hdl.$(SRC)

$(OUT_DIR)/wr_umm_pdcp_hdl.$(OBJ): $(IN_DIR)/wr_umm_pdcp_hdl.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_pdcp_hdl.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_pdcp_hdl.$(SRC)

$(OUT_DIR)/wr_umm_rrc_hdl.$(OBJ): $(IN_DIR)/wr_umm_rrc_hdl.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_rrc_hdl.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_rrc_hdl.$(SRC)

$(OUT_DIR)/wr_umm_s1ap_hdl.$(OBJ): $(IN_DIR)/wr_umm_s1ap_hdl.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_s1ap_hdl.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_s1ap_hdl.$(SRC)

$(OUT_DIR)/wr_umm_sch_hdl.$(OBJ): $(IN_DIR)/wr_umm_sch_hdl.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_sch_hdl.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_sch_hdl.$(SRC)

$(OUT_DIR)/wr_umm_x2ap_hdl.$(OBJ): $(IN_DIR)/wr_umm_x2ap_hdl.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_x2ap_hdl.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_x2ap_hdl.$(SRC)

$(OUT_DIR)/wr_umm_x2ap_msg.$(OBJ): $(IN_DIR)/wr_umm_x2ap_msg.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_x2ap_msg.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_x2ap_msg.$(SRC)

$(OUT_DIR)/wr_tmr.$(OBJ): $(IN_DIR)/wr_tmr.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_tmr.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_tmr.$(SRC)

#$(OUT_DIR)/wr_mi.$(OBJ): $(IN_DIR)/wr_mi.$(SRC) $(ALL_INC)
#	$(Q) $(CC)  -o$(OUT_DIR)/wr_mi.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
#	$(IN_DIR)/wr_mi.$(SRC)

$(OUT_DIR)/wr_ptmi.$(OBJ): $(IN_DIR)/wr_ptmi.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_ptmi.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCWRMILWR -DLWLCWRMILWR -DLWLCWR -DSM -DLCLWR \
	$(IN_DIR)/wr_ptmi.$(SRC)

$(OUT_DIR)/wr_ex_ms.$(OBJ): $(IN_DIR)/wr_ex_ms.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_ex_ms.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -ULCWR -DRG  -DLWLCWR  -DLWLCLWR   -DLCLWR -DSM   \
	$(IN_DIR)/wr_ex_ms.$(SRC)

$(OUT_DIR)/wr_lwr.$(OBJ): $(IN_DIR)/wr_lwr.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_lwr.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLWLCSMMILWR -DSM  -DLCLWR -DLWLCLWR -DLCSMMILWR  \
	$(IN_DIR)/wr_lwr.$(SRC)

$(OUT_DIR)/wr_umm_initcntsetup.$(OBJ): $(IN_DIR)/wr_umm_initcntsetup.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_initcntsetup.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_initcntsetup.$(SRC)

$(OUT_DIR)/wr_umm_rrcreestab.$(OBJ): $(IN_DIR)/wr_umm_rrcreestab.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_rrcreestab.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_rrcreestab.$(SRC)

$(OUT_DIR)/wr_umm_ctxtmod.$(OBJ): $(IN_DIR)/wr_umm_ctxtmod.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_ctxtmod.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_ctxtmod.$(SRC)

$(OUT_DIR)/wr_emm_anr.$(OBJ): $(IN_DIR)/wr_emm_anr.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_anr.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DDG \
	$(IN_DIR)/wr_emm_anr.$(SRC)

$(OUT_DIR)/wr_emm_ecsfb_anr.$(OBJ): $(IN_DIR)/wr_emm_ecsfb_anr.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_ecsfb_anr.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DDG \
	$(IN_DIR)/wr_emm_ecsfb_anr.$(SRC)

$(OUT_DIR)/wr_umm_x2srcho.$(OBJ): $(IN_DIR)/wr_umm_x2srcho.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_x2srcho.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_x2srcho.$(SRC)

$(OUT_DIR)/wr_umm_x2tgtho.$(OBJ): $(IN_DIR)/wr_umm_x2tgtho.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_x2tgtho.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_x2tgtho.$(SRC)

$(OUT_DIR)/wr_umm_s1tgtho.$(OBJ): $(IN_DIR)/wr_umm_s1tgtho.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_s1tgtho.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_s1tgtho.$(SRC)

$(OUT_DIR)/wr_umm_s1srcho.$(OBJ): $(IN_DIR)/wr_umm_s1srcho.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_s1srcho.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_s1srcho.$(SRC)

$(OUT_DIR)/wr_umm_ho.$(OBJ): $(IN_DIR)/wr_umm_ho.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_ho.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_ho.$(SRC)

$(OUT_DIR)/wr_umm_csfbupdtcell.$(OBJ): $(IN_DIR)/wr_umm_csfbupdtcell.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_csfbupdtcell.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_csfbupdtcell.$(SRC)

$(OUT_DIR)/wr_umm_csfb.$(OBJ): $(IN_DIR)/wr_umm_csfb.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_csfb.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_csfb.$(SRC)

$(OUT_DIR)/wr_umm_ecsfb.$(OBJ): $(IN_DIR)/wr_umm_ecsfb.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_ecsfb.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_ecsfb.$(SRC)

$(OUT_DIR)/wr_diag.$(OBJ): $(IN_DIR)/wr_diag.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_diag.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_diag.$(SRC)



$(OUT_DIR)/wr_smm_diag_hdl.$(OBJ): $(IN_DIR)/wr_smm_diag_hdl.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_smm_diag_hdl.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_smm_diag_hdl.$(SRC)

$(OUT_DIR)/wr_umm_locrpt.$(OBJ): $(IN_DIR)/wr_umm_locrpt.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_locrpt.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_locrpt.$(SRC)

$(OUT_DIR)/wr_kpi.$(OBJ): $(IN_DIR)/wr_kpi.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_kpi.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_kpi.$(SRC)

$(OUT_DIR)/wr_alarm.$(OBJ): $(IN_DIR)/wr_alarm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_alarm.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_alarm.$(SRC)

$(OUT_DIR)/wr_emm_s1_flex.$(OBJ): $(IN_DIR)/wr_emm_s1_flex.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_s1_flex.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_emm_s1_flex.$(SRC)

$(OUT_DIR)/wr_emm_cnm.$(OBJ): $(IN_DIR)/wr_emm_cnm.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_cnm.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(QMI_IOTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_emm_cnm.$(SRC)

$(OUT_DIR)/wr_emm_cnm_nlqmi.$(OBJ): $(IN_DIR)/wr_emm_cnm_nlqmi.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_cnm_nlqmi.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(QMI_IOTS)  $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_emm_cnm_nlqmi.$(SRC)

$(OUT_DIR)/wr_umm_csfb_geran.$(OBJ): $(IN_DIR)/wr_umm_csfb_geran.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_csfb_geran.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_csfb_geran.$(SRC)

$(OUT_DIR)/wr_smm_qcom_oam_init.$(OBJ): $(IN_DIR)/wr_smm_qcom_oam_init.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_smm_qcom_oam_init.$(OBJ) -I$(SS_DIR) $(QCOM_INC) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_smm_qcom_oam_init.$(SRC)

$(OUT_DIR)/wr_umm_perd_strong_cell_meas.$(OBJ): $(IN_DIR)/wr_umm_perd_strong_cell_meas.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_perd_strong_cell_meas.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_perd_strong_cell_meas.$(SRC)

$(OUT_DIR)/wr_umm_drx.$(OBJ): $(IN_DIR)/wr_umm_drx.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_drx.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_drx.$(SRC)

$(OUT_DIR)/wr_smm_tmr.$(OBJ): $(IN_DIR)/wr_smm_tmr.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -I$(SS_DIR)  $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) -DLCSMYSMILYS -DLCYSMILYS -DLCLYS $(IN_DIR)/wr_smm_tmr.$(SRC) -o \
	$(OUT_DIR)/wr_smm_tmr.$(OBJ)

$(OUT_DIR)/wr_umm_lteadv.$(OBJ): $(IN_DIR)/wr_umm_lteadv.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_umm_lteadv.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_umm_lteadv.$(SRC)

$(OUT_DIR)/wr_emm_lteadv.$(OBJ): $(IN_DIR)/wr_emm_lteadv.$(SRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q) $(CC)  -o$(OUT_DIR)/wr_emm_lteadv.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(IN_DIR)/wr_emm_lteadv.$(SRC) 



#ifeq ($(BLDENV),lnx_e2e_ms)
$(OUT_DIR)/hmac.$(OBJ): $(SEC_DIR)/hmac.$(SECSRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q)	$(CC)  -o$(OUT_DIR)/hmac.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(SEC_DIR)/hmac.$(SECSRC)
$(OUT_DIR)/hmacsha256.$(OBJ): $(SEC_DIR)/hmacsha256.$(SECSRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q)$(CC)  -o$(OUT_DIR)/hmacsha256.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(SEC_DIR)/hmacsha256.$(SECSRC)
$(OUT_DIR)/mp.$(OBJ): $(SEC_DIR)/mp.$(SECSRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q)$(CC)  -o$(OUT_DIR)/mp.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(SEC_DIR)/mp.$(SECSRC)
$(OUT_DIR)/endianness.$(OBJ): $(SEC_DIR)/endianness.$(SECSRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q)$(CC)  -o$(OUT_DIR)/endianness.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(SEC_DIR)/endianness.$(SECSRC)
$(OUT_DIR)/sha256.$(OBJ): $(SEC_DIR)/sha256.$(SECSRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q)	$(CC)  -o$(OUT_DIR)/sha256.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(SEC_DIR)/sha256.$(SECSRC)
$(OUT_DIR)/sha2k32.$(OBJ): $(SEC_DIR)/sha2k32.$(SECSRC) $(ALL_INC)
	@echo -e "Compiling $(COLOR) $< $(REVERT_COLOR)..."
	$(Q)	$(CC)  -o$(OUT_DIR)/sha2k32.$(OBJ) -I$(SS_DIR) $(COPTS) $(IOPTS) $(POPTS) $(CCwrFLAGS) \
	$(SEC_DIR)/sha2k32.$(SECSRC)
#endif

#-------------------------------------------------------------#
#Linker macros             
#-------------------------------------------------------------#
$(OUT_DIR)/libperflnxwr.a:$(wrPerfLnxOBJS)
	 $(AR) -cr $(OUT_DIR)/libperflnxwr.a $(wrPerfLnxOBJS)	

$(OUT_DIR)/libperflnxwruu.a:$(wrPerfLnxOBJSUU)
	$(AR) -cr $(OUT_DIR)/libperflnxwr.a $(wrPerfLnxOBJSUU)	

$(OUT_DIR)/libwrtenbapp_ms.a:$(wrE2eLnxOBJS_ms) $(wrE2eLnxOBJS_ms1)
	$(AR) -cr $(OUT_DIR)/libe2elnxwr_ms.a $(wrE2eLnxOBJS_ms)	$(wrE2eLnxOBJS_ms1)

ifeq (${BLDENV},lnx_e2e_ms)
$(OUT_DIR)/libe2elnxwr_ms.a:$(wrE2eLnxOBJS_ms) $(wrE2eLnxOBJS_ms1)
	$(AR) -cr $(OUT_DIR)/libe2elnxwr_ms.a $(wrE2eLnxOBJS_ms) $(wrE2eLnxOBJS_ms1)
else
ifeq (${BLDENV},lnx_e2e_ms_blog)
$(OUT_DIR)/libe2elnxwr_ms.a:$(wrE2eLnxOBJS_ms) $(wrE2eLnxOBJS_ms1)
	$(AR) -cr $(OUT_DIR)/libe2elnxwr_ms.a $(wrE2eLnxOBJS_ms) $(wrE2eLnxOBJS_ms1)
else
$(OUT_DIR)/libe2elnxwr.a:$(wrE2eLnxOBJS_ms) $(wrE2eLnxOBJS_ms1)
	$(AR) -cr $(OUT_DIR)/libe2elnxwr.a $(wrE2eLnxOBJS_ms) $(wrE2eLnxOBJS_ms1)
endif
endif

ifeq (${BLDENV},lnx_ms_pre)
$(OUT_DIR)/wrprepro:$(wrE2eLnxOBJS_ms) $(wrE2eLnxOBJS_ms1)
else
ifeq ($(BLDENV),lnx_bc_pre)
$(OUT_DIR)/wrprepro:$(wrE2eLnxOBJS_ms) $(wrE2eLnxOBJS_ms1)
else
$(OUT_DIR)/wrprepro:$(wrE2eLnxOBJS) $(wrE2eLnxOBJS1)
endif
endif


$(OUT_DIR)/libperfcavwr.a:$(wrPerfCavOBJS)
	 $(AR) -cr $(OUT_DIR)/libperfcavwr.a $(wrPerfCavOBJS)	

$(OUT_DIR)/libperfcavwruu.a:$(wrPerfCavOBJSUU)
	$(AR) -cr $(OUT_DIR)/libperfcavwr.a $(wrPerfCavOBJSUU)	

$(OUT_DIR)/libe2ecavwr.a:$(wrE2eCavOBJS)
	 $(AR) -cr $(OUT_DIR)/libe2ecavwr.a $(wrE2eCavOBJS)	

#-------------------------------------------------------------#
#Clean macros             
#-------------------------------------------------------------#
clean:
	\rm -f $(OUT_DIR)/libwr.a $(wrOBJS)
