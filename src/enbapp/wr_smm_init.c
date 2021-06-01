/********************************************************************16**

            (c) Copyright 2012 by RadiSys Corporation. All rights reserved.

     This software is confidential and proprietary to RadiSys Corporation.
     No part of this software may be reproduced, stored, transmitted, 
     disclosed or used in any form or by any means other than as expressly
     provided by the written Software License Agreement between Radisys 
     and its licensee.

     Radisys warrants that for a period, as provided by the written
     Software License Agreement between Radisys and its licensee, this
     software will perform substantially to Radisys specifications as
     published at the time of shipment, exclusive of any updates or 
     upgrades, and the media used for delivery of this software will be 
     free from defects in materials and workmanship.  Radisys also warrants 
     that has the corporate authority to enter into and perform under the 
     Software License Agreement and it is the copyright owner of the software 
     as originally delivered to its licensee.

     RADISYS MAKES NO OTHER WARRANTIES, EXPRESS OR IMPLIED, INCLUDING
     WITHOUT LIMITATION WARRANTIES OF MERCHANTABILITY OR FITNESS FOR
     A PARTICULAR PURPOSE WITH REGARD TO THIS SOFTWARE, SERVICE OR ANY RELATED
     MATERIALS.

     IN NO EVENT SHALL RADISYS BE LIABLE FOR ANY INDIRECT, SPECIAL,
     CONSEQUENTIAL DAMAGES, OR PUNITIVE DAMAGES IN CONNECTION WITH OR ARISING
     OUT OF THE USE OF, OR INABILITY TO USE, THIS SOFTWARE, WHETHER BASED
     ON BREACH OF CONTRACT, TORT (INCLUDING NEGLIGENCE), PRODUCT
     LIABILITY, OR OTHERWISE, AND WHETHER OR NOT IT HAS BEEN ADVISED
     OF THE POSSIBILITY OF SUCH DAMAGE.

                       Restricted Rights Legend:

     This software and all related materials licensed hereby are
     classified as "restricted computer software" as defined in clause
     52.227-19 of the Federal Acquisition Regulation ("FAR") and were
     developed entirely at private expense for nongovernmental purposes,
     are commercial in nature and have been regularly used for
     nongovernmental purposes, and, to the extent not published and
     copyrighted, are trade secrets and confidential and are provided
     with all rights reserved under the copyright laws of the United
     States.  The government's rights to the software and related
     materials are limited and restricted as provided in clause
     52.227-19 of the FAR.

                    IMPORTANT LIMITATION(S) ON USE

     The use of this software is limited to the use set
     forth in the written Software License Agreement between Radisys and
     its Licensee. Among other things, the use of this software
     may be limited to a particular type of Designated Equipment, as 
     defined in such Software License Agreement.
     Before any installation, use or transfer of this software, please
     consult the written Software License Agreement or contact Radisys at
     the location set forth below in order to confirm that you are
     engaging in a permissible use of the software.

                    RadiSys Corporation
                    Tel: +1 (858) 882 8800
                    Fax: +1 (858) 777 3388
                    Email: support@trillium.com
                    Web: http://www.radisys.com 
 
*********************************************************************17*/

/********************************************************************20**

     Name:     EnodeB Application

     Type:     C source file

     Desc:     This file contains 
               

     File:     wr_smm_init.c

     Sid:      fc_smm_init_brcm.c@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:52:13 2014

     Prg:      Sriky 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="SMM";
static int RLOG_MODULE_ID=2;
static int RLOG_FILE_ID=117;
#include<errno.h>
#include<signal.h>
#include "wr.h"
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_smm_init.h"
#include "wr_smm_smallcell.h"
#include "cm_os.h"       /* common hashing */
#include "cm_os.x"
#include "ss_diag.h"      /* Common logging header */
#include "wr_emm_cnm.h"
/* LTE_ADV starts */
#include "wr_cmn.h"
/* LTE_ADV ends */
#include <stdlib.h>
#ifdef LTE_L2_MEAS
#include "lpj.h"
#include "lkw.h"
#include "lrg.h"
#include "wr_dam.h"
#endif/*LTE_L2_MEAS*/
#include "cm_mem.h"
#include "cm_mem.x"

#include "wr_kpi.h"
#include "wr_kpiIds.h"
#include "wr_kpi.h"
#ifdef WR_RSYS_OAM
#include "wr_msm_common.h"
extern MsmLteeNodeBparams lteeNodeBparams;
#endif
#ifdef SS_RBUF
U32 allocFreeCnt[6];
U32 regRange[4];
#endif
#ifdef WR_RSYS_OAM
EXTERN Void SmAppTst(Void);
EXTERN Void MsmConfigComplete(Void);
EXTERN Void MsmShutDownAck(Void);
PRIVATE void *wrSmmAppTstCaller(void *arg);
PRIVATE void SCreateSmmAppTst(void);
#endif
#ifdef WR_WATCHDOG
EXTERN Void wrRegWatchDog(Void);
#endif
PUBLIC S16 wrRegisterAndCreateRecords(void);
PUBLIC S16 testActvTsk(Pst *pst,  Buffer *mBuf);
PUBLIC S16 testActvInit (Ent ent, Inst inst, Region reg, Reason reason);
PUBLIC S32 ssCheckAndAddMemoryRegionMap(pthread_t  threadId,Region region);
PUBLIC S16 SPrintSRngStats(Void);
PUBLIC S16 SRegInfoShow(Region region,U32 *availmem);
EXTERN Void ssMlogInit(Void);
EXTERN Void ssMemlogInit(Void);
PUBLIC TstTkn    tkn;
EXTERN Void smStaticCfgFailAlarm(void);
PUBLIC Void wrSmHdlUnBndRrmStack(U8 event);
PUBLIC Void wrSmHdlUnBndRrcStack(U8 event);
PUBLIC Void wrSmHdlUnBndX2apStack(U8 event);
PUBLIC Void wrSmHdlUnBndEgtpStack(U8 event);
PUBLIC Void wrSmHdlUnBndS1apStack(U8 event);


#ifdef REM_ENABLE
void initNmmTask(void);
#endif

U8 wrSmStateStr[WR_SM_STATE_S1AP_STK_UNBND_DONE + 1][80] = {
    "WR_SM_STATE_INIT = 0",
    "WR_SM_STATE_TUCL_CFG_DONE",
    "WR_SM_STATE_SCTP_CFG_DONE",
    "WR_SM_STATE_S1AP_CFG_DONE",
    "WR_SM_STATE_X2AP_CFG_DONE",
    "WR_SM_STATE_EGTP_CFG_DONE",
    "WR_SM_STATE_APP_CFG_DONE",
    "WR_SM_STATE_RRC_CFG_DONE",
    "WR_SM_STATE_PDCP_CFG_DONE",
    "WR_SM_STATE_RLC_CFG_DONE",
    "WR_SM_STATE_PDCP_UL_CFG_DONE",
    "WR_SM_STATE_PDCP_DL_CFG_DONE",
    "WR_SM_STATE_RLC_UL_CFG_DONE",
    "WR_SM_STATE_RLC_DL_CFG_DONE",
    "WR_SM_STATE_MAC_CFG_DONE",
    "WR_SM_STATE_CL_CFG_DONE",
#ifdef RM_INTF
    "WR_SM_STATE_RRM_CFG_DONE",
#endif /*RM_INTF*/
    "WR_SM_STATE_SON_CFG_DONE",
    "WR_SM_STATE_S1AP_STK_BND_DONE",
    "WR_SM_STATE_EGTP_STK_BND_DONE",
    "WR_SM_STATE_X2AP_STK_BND_DONE",
    "WR_SM_STATE_RRC_STK_BND_DONE",
#ifdef RM_INTF
    "WR_SM_STATE_RRM_STK_BND_DONE",
#endif
    "WR_SM_STATE_ENB_DEBUGS_DONE",
    "WR_SM_STATE_ENB_ALRMS_DONE",
    "WR_SM_STATE_AWAIT_S1_CON",
    "WR_SM_STATE_AWAIT_CELL_UP",
    "WR_SM_STATE_DYN_X2AP_CFG_DONE",
    /* CNM_DEV */
    "WR_SM_STATE_AWAIT_SYNC",
    "WR_SM_STATE_CELL_UP",
    "WR_SM_STATE_CELL_DOWN",
#ifdef WR_RSYS_OAM 
    "WR_SM_STATE_OAM_DYN_START",
#endif
#ifdef E_TM
    "WR_SM_STATE_CL_INIT",
#endif      
    "WR_SM_STATE_ENB_LOG_DONE",
/* IPSec Support */
    "WR_SM_STATE_REBOOT_REQUIRED",
    "WR_SM_STATE_DINIT",
    "WR_SM_STATE_TUCL_SHUTDWN_DONE",
    "WR_SM_STATE_SCTP_SHUTDWN_DONE",
    "WR_SM_STATE_S1AP_SHUTDWN_DONE",
    "WR_SM_STATE_X2AP_SHUTDWN_DONE",
    "WR_SM_STATE_EGTP_SHUTDWN_DONE",
    "WR_SM_STATE_APP_SHUTDWN_DONE",
    "WR_SM_STATE_RRM_SHUTDWN_DONE",
    "WR_SM_STATE_RRC_SHUTDWN_DONE",
    "WR_SM_STATE_SON_SHUTDWN_DONE",
    "WR_SM_STATE_PDCP_DL_SHUTDWN_DONE",
    "WR_SM_STATE_PDCP_UL_SHUTDWN_DONE",
    "WR_SM_STATE_RLC_DL_SHUTDWN_DONE",
    "WR_SM_STATE_RLC_UL_SHUTDWN_DONE",
    "WR_SM_STATE_PDCP_SHUTDWN_DONE",
    "WR_SM_STATE_RLC_SHUTDWN_DONE",
    "WR_SM_STATE_MAC_SCH_SHUTDWN_DONE",
    "WR_SM_STATE_MAC_SHUTDWN_DONE",
    "WR_SM_STATE_CL_SHUTDWN_DONE",
    "WR_SM_STATE_RRM_STK_UNBND_DONE",
    "WR_SM_STATE_RRC_STK_UNBND_DONE",
    "WR_SM_STATE_X2AP_STK_UNBND_DONE",
    "WR_SM_STATE_EGTP_STK_UNBND_DONE",
    "WR_SM_STATE_S1AP_STK_UNBND_DONE"
};


sem_t smOamStartSema;
#define WR_SMM_VALIDATE_CFG_PARAM(_param, _min, _max)\
{\
   if(_min > _param || _param > _max)\
   {\
      RLOG2(L_ERROR, " Invalid parameter value %s = %d ", #_param, _param);\
      RLOG2(L_ERROR, " Param should be min=%d, max=%d ", _min, _max);\
      RETVALUE(RFAILED);\
   }\
}

PUBLIC    SmCb smCb;
PUBLIC    SmCfgCb smCfgCb;   
#ifdef LTE_L2_MEAS
PUBLIC    U32 glbMacMeasFlag;
#endif
//PUBLIC U16 wrSmMaxBufLen[SS_DIAG_MAX_CIRC_BUF];

#ifdef E_TM
PUBLIC Void wrSmHdlCellCfgEvent(U8 event);
EXTERN U8 etmCellState;
#endif

#ifdef TENB_STATS
EXTERN Void TSInfCfgCL ARGS(());
#endif

PUBLIC Void smBuildShutRestart(Void);
PRIVATE Void smBuildFullRstctrl(Void); 
PUBLIC S16 smBuildWrUnbindEg (Void);
PUBLIC S16 smBuildWrUnbindCz (Void);
PUBLIC S16 smBuildWrUnbindSz (Void);
EXTERN S16 wrEmmSndSchdReCfgReq ARGS((U32  cellId,U32  minDlResNonCsg, U32 minUlResNonCsg));
EXTERN S16 smSndCliRmuProtModCellCfg ARGS((U8 cellId, U16 maxCsgUEs, U16 maxNonCsgUEs));
/* CSG_DEV */
void flushData(int sig);
/* unused variable */
/* PRIVATE U8 wrLogEnbFirstTime = FALSE; */

#ifdef WR_TEST_CODE
/* S1 CLI variables */
U8 wrTgrMmeCfgUpdFail = FALSE;
U8 wrTgrEnbErrInd     = FALSE;
U8 wrDntRspMmeCfgUpd  = FALSE;
U8 wrTgrEnbCfgUpdRtx  = FALSE;
U8 wrTgrS1SetupRtx    = FALSE;

/* X2 CLI variables */
U8 wrTgrX2SetupFail = FALSE;
U8 wrTgX2SetupFailWoTW = FALSE;
U8 wrDntRspX2Setup = FALSE;
U8 wrTgrX2CfgUpdFail = FALSE;
U8 wrTgrX2CfgUpdFailWoTW = FALSE;
U8 wrDntRspX2CfgUpd = FALSE;
U8 wrDntRspX2Reset = FALSE;
U8 wrTrgX2ErrInd = FALSE;

U8 wrTrgNasNonDlvrInd = FALSE;

/* RRC CLI variables */
U8 wrTrgRrcConTO = FALSE;
U8 wrTrgRrcConRelTO = FALSE;
U8 wrTrgUeCapTO = FALSE;
U8 wrTrgSecTO = FALSE;
U8 wrTrgRrcReConTO = FALSE;
U8 wrTrgRrcRestTO = FALSE;  


U8        wrCmdArgs[MAXCMDLEN];
U16       wrCmdLen;
EXTERN U8 wrTgrEnbOverLoad;
#ifdef WR_CLI_TARGET_BOARD
PRIVATE S16 smCliRecvTsk ARGS((Pst *pst, Buffer *mBuf));
#endif

PUBLIC Keys  wrCliSecArgKey[] = {
   {"on",            0},
   {"off",           1},
   {"enb",           2},
   {"dis",           3},
   {"",  MATCH_FAILED }
};

/* LTE_ADV starts */
PUBLIC Keys  wrCliLteaArgKey[] = {
   {"abs",            RGR_ABS},
   {"sfr",            RGR_SFR},
   {"dsfr",           RGR_DSFR},
   {"re",             RGR_RE},
   {"",  MATCH_FAILED  }
};
/* LTE_ADV ends */

PUBLIC Keys  wrDbgLyrKey[] = {
   {"app",           1 },
   {"s1ap",          2 },
   {"sctp",          3 },
   {"tucl",          4 },
   {"x2ap",          5 },
   {"egtp",          6 },
   {"rrc",           7 },
   {"rlc",           8 },
   {"pdcp",          9 },
   {"mac",           10 },
   {"cl",            11 },
   {"cplane",        12 },
   {"all",           13 },
   {"",    MATCH_FAILED }     
};

#ifdef PHY_ERROR_LOGING
PUBLIC    SmUlAllocInfoCb smUlAllocInfoCb;   
#endif

PUBLIC Keys  wrCliMenuKey[] = {
   {"help",                      0 },
   {"enbovld",                   1 },
   {"snds1cfgupd",               2 },
   {"snds1cfgupdfail",           3 },
   {"dbgp",                      4 },
   {"s1partrst",                 5 },
   {"s1fullrst",                 6 },
   {"sndx2setupfail",            7 },
   {"dntrspx2setup",             8 },
   {"sndx2cfgupdfail",           9 },
   {"dntrspx2cfgupd",           10 },
   {"dntrspx2rst",              11 },
   {"sndx2errind",              12 },
   {"sndx2rst",                 13 },
   {"memstatus",                14 },
   {"sndrabrls",                15 },
   {"sndenberrind",             16 },
   {"sndnasnondelind",          17 },
   {"dntrsps1cfgupd",           18 },
   {"sndendcfgupdrx",           19 },
   {"snds1setuprx",             20 },
   {"empty",                    21 },
   {"empty",                    22 },
   {"drprrcconnsetupcmplt",     23 },
   {"drprrcrelcfm",             24 },
   {"drpuecapinfo",             25 },
   {"drpseccmplt",              26 },
   {"drprrcrecfgcmlpt",         27 },
   {"drprrcreestabcmplt",       28 },
   {"enbportlog",               29 },
   {"trgRrcRestTO",             30 },
   {"loadInd",                  31 },
   {"printNrInfo",              32 },
   {"macstats",                 33 },
   {"phylogging",               34 },
#ifdef E_TM 
   {"etmtest",                  35 },
#endif /* E_TM */
   {"ipchange",                 36 },
/* LTE_ADV starts */
   {"ltea",                     37 },
/* LTE_ADV ends */
   {"startlogging",             38 },
   {"stoplogging",              39 },
#ifdef SS_RBUF
   {"ringstatus",               40 },
#endif
   /* CSG_DEV */
   {"csgprntcellinfo",          41 },
   {"csgprntnbrinfo",           42 },
   /* CNM_DEV */
   {"cnmprnteutranbrinfo",      43 },
   {"csgdlulresrecfg",          44 },
   {"csgcellrecfg",             45 },
   {"",               MATCH_FAILED }
};
#endif

PUBLIC S8 *wrCfgTagLst[MAX_CFG_PARAM] = {
   "WR_TAG_CELL_ID",
   "WR_TAG_MOD_TYPE",
   "WR_TAG_DUP_MODE",
   "WR_TAG_MAX_UE_SUPPORT",
   "WR_TAG_MAX_AVG_GBRPRB_USAGE",
   "WR_TAG_MCC_0",
   "WR_TAG_MCC_1",
   "WR_TAG_MCC_2",
   "WR_TAG_MNC_0",
   "WR_TAG_MNC_1",
   "WR_TAG_MNC_2",
   "WR_TAG_TA_CODE",
   "WR_TAG_FREQ_BAND_IND",
   "WR_TAG_ENB_IP_ADDR",
   "WR_TAG_ENB_IP_ADDR_IPV6",
   "WR_TAG_PCI_SELECT_TYPE",
   "WR_TAG_PRACH_SELECT_TYPE",
   "WR_TAG_EARFCN_SELECT_TYPE",
   "WR_TAG_PCI_LIST",
   "WR_TAG_DL_EARFCN_LIST",
   "WR_TAG_UL_EARFCN_LIST",
   "WR_TAG_ROOTSEQ_IDX_LIST",
   "WR_TAG_PRACHCFG_IDX_LIST",
   "WR_TAG_ZERO_CORR_ZONE_CFG_LIST",
   "WR_TAG_PRACH_FREQ_OFFSET_LIST",
   "WR_TAG_ROOTSEQ_IDX",
   "WR_TAG_PRACHCFG_IDX",
   "WR_TAG_ZERO_CORR_ZONE_CFG",
   "WR_TAG_PRACH_FREQ_OFFSET",
   "WR_TAG_NO_OF_MME_INFO",
   "WR_TAG_MME_INFO",
   "WR_TAG_NO_OF_MME_INFO_IPV6",
   "WR_TAG_MME_INFO_IPV6",
   "WR_TAG_HI_DBG",
   "WR_TAG_SB_DBG",
   "WR_TAG_SZ_DBG",
   "WR_TAG_EG_DBG",
   "WR_TAG_WR_DBG",
   "WR_TAG_NH_DBG",
   "WR_TAG_KW_DBG",
   "WR_TAG_RG_DBG",
   "WR_TAG_YS_DBG",
   "WR_TAG_SM_DBG",
   "WR_TAG_INACTIVITY_TIMER_VAL",
   "WR_TAG_MAX_EXPIRY"
#ifdef LTE_HO_SUPPORT
      ,
   "WR_TAG_CZ_DBG",
   "WR_TAG_X2_PREP_TMR",
   "WR_TAG_X2_OVRALL_TMR",
   "WR_TAG_ENB_NAME",
   "WR_TAG_NO_OF_BRDCST_PLMN",
   "WR_TAG_PLMN_ID_LST",
   "WR_TAG_NO_OF_NGH_INFO",
   "WR_TAG_NGH_INFO_CFG",
   "WR_TAG_NO_OF_GU_GRP",
   "WR_TAG_GU_GRP_ID_CFG",
   "WR_TAG_S1_PREP_TMR",
   "WR_TAG_S1_OVRALL_TMR",
   "WR_TAG_NO_OF_NGH_CFG",
   "WR_TAG_NGH_CELL_CFG",
   "WR_TAG_NO_OF_NGH_CFG_IPV6",
   "WR_TAG_NGH_CELL_CFG_IPV6",
   "WR_TAG_MEAS_CFG_ENB",
   "WR_TAG_RRM_RNTI_STRT",
   "WR_TAG_MAX_RRM_RNTIS",
   "WR_TAG_MAC_RNTI_STRT",
   "WR_TAG_MAX_MAC_RNTIS",
   "WR_TAG_RRM_NO_OF_DED_PREMBL",
   "WR_TAG_RRM_DED_PREMBL_STRT",
   "WR_TAG_MAC_NO_OF_PREMBL",
   "WR_TAG_A1_RSRP_THRSHLD_VAL",
   "WR_TAG_A2_RSRP_THRSHLD_VAL",
   "WR_TAG_INTRA_A5_RSRP_THRSD1_VAL",
   "WR_TAG_INTRA_A5_RSRP_THRSD2_VAL",
   "WR_TAG_INTER_A5_RSRP_THRSD1_VAL",
   "WR_TAG_INTER_A5_RSRP_THRSD2_VAL",
   "WR_TAG_INTRA_HO_A3_OFFSET",
   "WR_TAG_INTRA_ANR_A3_OFFSET",
   "WR_TAG_INTER_ANR_A5_RSRP_THRSD1_VAL",  
   "WR_TAG_INTER_ANR_A5_RSRP_THRSD2_VAL",  
   "WR_TAG_ANR_REPORT_CFG_VAL",
   "WR_TAG_ANR_CELL_VALID_AGE_VAL",
   "WR_TAG_HO_REPORT_CFG_VAL",/*rsrq_ho*/
   "WR_TAG_RSRQ_THRSHLD_VAL",
   "WR_TAG_ANR_EPOC_TMR_VAL_IN_SECS",
   "WR_TAG_ANR_TRICE_INTV_COUNT",
   "WR_TAG_UTRA_B2_RSRP_THRSD1_VAL",
   "WR_TAG_UTRA_FDD_B2_RSCP_THRSD2_VAL",
   "WR_TAG_UTRA_TDD_B2_RSCP_THRSD2_VAL",
   "WR_TAG_S_MEASURE_VAL",
   "WR_TAG_INTRA_TTT_VAL",
   "WR_TAG_INTRA_HYTERISIS",
   "WR_TAG_NO_OF_NGH_ENB_CFG",
   "WR_TAG_NGH_ENB_CFG",
   "WR_TAG_ENB_TYPE",
   "WR_TAG_PCI_VAL",
   "WR_TAG_DL_NUM_UE_PER_TTI",
   "WR_TAG_UL_NUM_UE_PER_TTI",
   "WR_TAG_DL_SCHD_TYPE",
   "WR_TAG_UL_SCHD_TYPE",
   "WR_TAG_DLFS_SCHD_TYPE",
   "WR_TAG_PFS_DL_TPT_COEFFICIENT",
   "WR_TAG_PFS_DL_FAIRNESS_COEFFICIENT",
   "WR_TAG_PFS_UL_TPT_COEFFICIENT",
   "WR_TAG_PFS_UL_FAIRNESS_COEFFICIENT",
   "WR_TAG_TM_AUTO_CONFIG",
   "WR_TAG_PREFERRED_TM",
   "WR_TAG_MAX_X2_PEERS",
   "WR_TAG_X2_TIME_TO_WAIT",
   "WR_TAG_UE_HO_ATTACH_TMR",
   "WR_TAG_SCTP_SRVC_TYPE",
   "WR_TAG_OP_MODE",
   "WR_TAG_PERIOD",
   "WR_TAG_DL_EARFCN",
   "WR_TAG_UL_EARFCN",
   "WR_TAG_END_MARKER_TIMER_VAL"
#endif
#ifdef OAM_PH0_SUPPORT
      ,
   "WR_TAG_S1_RESET_TMR",
   "WR_TAG_X2_RESET_TMR"
#endif
#ifdef TENB_AS_SECURITY
      ,
   "WR_TAG_AS_PRI_INTG_ALGO_LST",
   "WR_TAG_AS_PRI_CIPH_ALGO_LST"
#endif
      ,
   "WR_TAG_DIAG_TUCL",
   "WR_TAG_DIAG_S1AP",
   "WR_TAG_DIAG_EGTP",
   "WR_TAG_DIAG_APP",
   "WR_TAG_DIAG_LTERRC",
   "WR_TAG_DIAG_LTERLC",
   "WR_TAG_DIAG_LTEPDCP",
   "WR_TAG_DIAG_LTECL",
   "WR_TAG_DIAG_LTEMAC",
   "WR_TAG_DIAG_X2AP",
   "WR_TAG_DIAG_SCTP"
#ifdef PHY_ERROR_LOGING
      ,
   "WR_TAG_UL_MCS_VAL",
   "WR_TAG_UL_RB_START_VAL",
   "WR_TAG_UL_NUM_RB_VAL",
   "WR_TAG_UL_ENABLE_TIME"
#endif
    ,
   "WR_TAG_NO_OF_UTRA_FDD_NGH_CFG",
   "WR_TAG_NGH_UTRA_FDD_CELL_CFG",
   "WR_TAG_NO_OF_UTRA_TDD_NGH_CFG",
   "WR_TAG_NGH_UTRA_TDD_CELL_CFG",
   "WR_TAG_NUM_EUTRA_FREQ",
   "WR_TAG_EUTRA_FREQ_CFG",
   "WR_TAG_NUM_UTRA_FDD_FREQ",
   "WR_TAG_UTRA_FDD_FREQ_CFG",
   "WR_TAG_NUM_UTRA_TDD_FREQ",
   "WR_TAG_UTRA_TDD_FREQ_CFG",
   "WR_TAG_NUM_CDMA_1XRTT_BAND_CLS",
   "WR_TAG_CDMA_1XRTT_BAND_CLS",
   "WR_TAG_NUM_CDMA_1XRTT_NEIGH_FREQ",
   "WR_TAG_CDMA_1XRTT_NEIGH_FREQ",
   "WR_TAG_NUM_CDMA_1XRTT_NEIGH_CELL",
   "WR_TAG_CDMA_1XRTT_NEIGH_CELL",
   "WR_TAG_CSFB_UTRA_CFG_VAL",
   "WR_TAG_CSFB_CDMA_CFG_VAL",
   "WR_TAG_ECSFB_CFG_VAL",
   "WR_TAG_DUAL_RX_SUPPORTED",
   "WR_TAG_MAX_DL_UE_PER_TTI",
   "WR_TAG_MAX_UL_UE_PER_TTI",
   "WR_TAG_DEFAULT_PAGING_CYCLE",
   "WR_TAG_DEFAULT_PCCH_CFG_NB",   
   "WR_TAG_SCTP_RTO_MIN",
   "WR_TAG_SCTP_RTO_MAX",
   "WR_TAG_SCTP_RTO_INITIAL",
   "WR_TAG_SCTP_HRTBEAT_INTERVAL",
   "WR_TAG_ABS_A3_OFFSET",
   "WR_TAG_PICO_OFFSET",
   "WR_TAG_ABS_PATTERN_TYPE",
   "WR_TAG_ABS_PATTERN",
   "WR_TAG_ABS_LOAD_PERIODICITY",
   "WR_TAG_SFR_START_RB",
   "WR_TAG_SFR_END_RB",
   "WR_TAG_SFR_POWER_LOW",
   "WR_TAG_SFR_POWER_HIGH",
   "WR_TAG_DRX_QCI_SUPPORT_ENABLED",
   "WR_TAG_DRX_ENABLED",
   "WR_TAG_DRX_INACTIVITY_TMR",
   "WR_TAG_DRX_RETX_TMR",
   "WR_TAG_DRX_LONG_CYCLE_GBR",
   "WR_TAG_DRX_LONG_CYCLE_NON_GBR",
   "WR_TAG_ANR_DRX_LONG_CYCLE",
   "WR_TAG_ANR_DRX_ON_DURATION_TMR",
   /* Fix for ccpu00132334 - To read DRX Inactivity timer for ANR*/
   "WR_TAG_ANR_DRX_INACTIVITY_TMR" 
#ifdef E_TM
      ,
   "WR_TAG_BOOT_MODE",
   "WR_TAG_ETM_ID"
#endif /* E_TM */
      ,
   "WR_TAG_BANDWIDTH",
   "WR_TAG_NO_OF_EAID_INFO",
   "WR_TAG_EAID_LIST_INFO",
   "WR_TAG_PRIMETWS_NO_BROADCAST", /* ccpu00128279 : Number of broadcast for primary ETWS. */
   "WR_TAG_PRIMETWS_REPETITION_PERIOD", /* ccpu00128279 : Repetition period for primary ETWS. */
   "WR_TAG_PWS_ETWS_CMAS_CNTRL",
   "WR_TAG_TDD_ULDL_CFG_MODE",
   "WR_TAG_TDD_SPCL_SF_CONFIG"
#ifdef WR_WATCHDOG
      ,
   "WR_TAG_WATCHDOG_SOFT_LIMIT",
   "WR_TAG_WATCHDOG_HARD_LIMIT"
#endif /* WR_WATCHDOG */
      ,
   "WR_TAG_INT_FREQ_MEAS_GAP", /* Measurement gap configuration */
   "WR_TAG_ANR_MEAS_GAP_CONFIG",   
   "WR_TAG_SRC_HO_CANCEL_TMR",
   "WR_TAG_LOG_PATH", 
   "WR_TAG_LOG_FILE",  
   "WR_TAG_LOG_MAX_FILES",
   "WR_TAG_LOG_FILESIZE_LIMIT", 
   "WR_TAG_LOG_LEVEL", 
   "WR_TAG_LOG_MASK",
   "WR_TAG_LOG_PORT",
   "WR_TAG_LOG_REMOTE_LOGGING",
   "WR_TAG_LOG_CIRBUF_SIZE",
   "WR_TAG_LOG_COREDUMP_FLAG",
   "WR_TAG_RRM_SR_PRDCTY",
   "WR_TAG_RRM_CQI_PRDCTY",
   "WR_TAG_RRM_NUM_SR_PER_TTI",
   "WR_TAG_RRM_NUM_CQI_PER_TTI",
   "WR_TAG_RRM_N1_PUCCH",
   "WR_TAG_SPS_ENABLE_FLAG",
   "WR_TAG_DL_NUM_SPS_UE_PER_TTI",
   "WR_TAG_UL_NUM_SPS_UE_PER_TTI",
   "WR_TAG_MAX_SPS_RB",
   "WR_TAG_DSCP_ENABLE",
   "WR_TAG_QCI_DSCP_MAP",
   "WR_TAG_DL_QCI_SCHD_WGT",
   "WR_TAG_UL_QCI_SCHD_WGT",
   "WR_TAG_CSG_CSG_ID",
   "WR_TAG_CSG_ACCESS_MODE",
   "WR_TAG_CSG_CSG_PCI_START",
   "WR_TAG_CSG_CSG_PCI_RANGE",
   "WR_TAG_CSG_SMCELL_PCI_START",
   "WR_TAG_CSG_SMCELL_PCI_RANGE",
   "WR_TAG_CSG_HENB_NAME",
   "WR_TAG_MAX_CSG_UES",
   "WR_TAG_MAX_NON_CSG_UES",
   "WR_TAG_MIN_DL_RSRC_FOR_NON_CSG",
   "WR_TAG_MIN_UL_RSRC_FOR_NON_CSG",
   "WR_TAG_RIM_CFG",
   "WR_TAG_NUM_GERAN_NEIGH_FREQ",  
   "WR_TAG_GERAN_NEIGH_FREQ",
   "WR_TAG_GERAN_NEIGH_CELL",
   "WR_TAG_GERAN_MEAS_CFG",
   "WR_TAG_CELL_PMAX",
   "WR_TAG_CNM_ENABLED",
   "WR_TAG_CNM_BOARD_TYPE",
   "WR_TAG_TA_TMR_CMN",
   "WR_TAG_TA_TMR_DED",
   "WR_TAG_CELL_SIZE_TYPE",/*UE History info*/
   "WR_TAG_ARP_EMER_SERV", /* ARP for Emergency Bearers */
   "WR_TAG_DCFI_ENABLE",
   "WR_TAG_CFI",
   "WR_TAG_GRP_PUCCH_PWR_CTRL_ENABLE",
   "WR_TAG_GRP_PUSCH_PWR_CTRL_ENABLE",
   "WR_TAG_LTE_PUCCH_PWR_FMT3A_ENABLE",
   "WR_TAG_LTE_PUSCH_PWR_FMT3A_ENABLE",
   /* ccpu00138740 */
   "WR_TAG_PRACH_CONFIG_IDX",
   "WR_TAG_DUAL_RX_TX_SUPPORTED", /*RRC Rel 10 Upgrade*/
   "WR_TAG_GERAN_B2_RED_RSRP_THRSD1_VAL",
   "WR_TAG_GERAN_B2_RED_THRSD2_VAL",
   "WR_TAG_GERAN_B2_HO_RSRP_THRSD1_VAL",
   "WR_TAG_GERAN_B2_HO_THRSD2_VAL"
   "WR_TAG_DUAL_RX_TX_SUPPORTED", /*RRC Rel 10 Upgrade*/
   "WR_TAG_NUM_PRB_RPTS",
   "WR_TAG_PRB_RPT_INTERVAL",
   "WR_TAG_SRS_ENABLE",
   "WR_TAG_SRS_PERIODICITY",
   "WR_TAG_SRS_CELL_BANDWIDTH",
   "WR_TAG_SRS_CELL_SUBFRAME_CFG",
   "WR_TAG_SRS_CELL_ACKNACK_SIMTX",
   "WR_TAG_SRS_CELL_MAX_UPPTS",
   "WR_TAG_SRS_UE_BANDWIDTH",
   "WR_TAG_SRS_UE_HOPPING_BW",
#ifdef RSYS_WIRESHARK
   "WR_TAG_WIRESHARK_PORT",/* to send Logs to wireshark */
   "WR_TAG_WIRESHARK_DST_IP_ADDR", /* to send Logs to wireshark */
#endif
   "WR_TAG_RSYS_TPM_TGT_RSRP",
   "WR_TAG_RSYS_TPM_ATTN_CONST_IDX",
   "WR_TAG_RSYS_TPM_MIN_RSRP",
   "WR_TAG_RSYS_TPM_MAX_RSRP",
   "WR_TAG_NUM_TX_ANTENNA"
};

/* Chinna:X2ap*/
U16 peerIdLst[WR_SMM_MAX_NBR_ENB]={1,4,5,8,12,15,17,21,24,27,32,36,38,42,44,48};

PRIVATE Void wrSmmInitSmCfgCb
(
Void
)
{
   U8                        idCnt;

   for(idCnt = 0;idCnt < WR_SMM_MAX_NBR_ENB; idCnt++)
   {
      smCfgCb.wrNghCellCfg[idCnt] = NULLP;
      smCfgCb.wrX2apPeerIdLst[idCnt].pres = FALSE;
      smCfgCb.wrX2apPeerIdLst[idCnt].val = peerIdLst[idCnt];
   }
   /* SCTP Heartbeat Param Initialization */
   smCfgCb.lwrSctpParamCfg.rtoInitial = 2;
   smCfgCb.lwrSctpParamCfg.rtoMin = 1;
   smCfgCb.lwrSctpParamCfg.rtoMax = 10;
   smCfgCb.lwrSctpParamCfg.heartBeatInterval = 50;    /* Heartbeat timer,5 seconds */
}


pthread_t tmpThrId;

SSTskId testSTaskId;

#define TESTING 1
#ifdef TESTING
#ifdef ANSI
PUBLIC S16 testActvTsk
(
Pst *pst,                   /* post */
Buffer *mBuf                /* message buffer */
)
#else
PUBLIC S16 testActvTsk(pst, mBuf)
Pst *pst;                   /* post */
Buffer *mBuf;               /* message buffer */
#endif
{
   Pst myPst;
#if 0
#else
   myPst.event     = 0;                 /* event */
   myPst.prior     = 0;                 /* priority */
   myPst.route     = 0;                 /* route */
   myPst.dstProcId = pst->srcProcId;    /* destination processor id */
   myPst.dstEnt    = pst->srcEnt;           /* source entity */
   myPst.dstInst   = pst->srcInst;          /* source instance */
   myPst.srcEnt    = ENTTST;      /* destination entity */
   myPst.srcInst   = 0;    /* destination instance */
   myPst.srcProcId = SFndProcId();      /* source processor id */
   if (SPstTsk(&myPst, mBuf) != ROK)
      RETVALUE(RFAILED);
#endif
 RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 testActvInit
(
Ent ent,                    /* entity */
Inst inst,                  /* instance */
Region reg,                 /* region */
Reason reason               /* reason */
)
#else
PUBLIC S16 testActvInit(ent, inst, reg, reason)
Ent ent;                    /* entity */
Inst inst;                  /* instance */
Region reg;                 /* region */
Reason reason;              /* reason */
#endif
{
 RETVALUE(ROK);
}



SSTskId rrc_s1ap_app_taskId;
/*
*
*       Fun:   tst
*
*       Desc:  tst entry point. First entry point.
*
*       Ret:   ROK on success, RFAILED on error
*
*       Notes: None
*
*       File:  wr_smm_init.c
*
*/
#ifdef ANSI
PUBLIC S16 tst
(
Void
)
#else
PUBLIC S16 tst(Void)
#endif
{
   /* System tasks that are needed as per the architecture */
   SSTskId egtp_dat_app_taskId;
   SSTskId sctp_taskId;
   SSTskId sm_taskId;
   SSTskId tucl_taskId;
#ifdef WR_CLI_TARGET_BOARD
   Inst dstInst = 1;
   SSTskId cli_taskId;
   Pst  cliPst;
#endif /* WR_CLI_TARGET_BOARD */
   U16 smEvent;
   /* wr002.102: Added trace macro */
   TRC2(tst)

   SSetProcId(SM_SM_PROC);
#ifndef LTE_ENB_PAL
#endif

#ifndef LTE_ENB_PAL
#ifdef SS_MULTICORE_SUPPORT
   {
      /* Two cores, each having one hardware thread */
      SCpuInfo cpuInfo = { 2, 1, {1, 1}};
      SRegCpuInfo(&cpuInfo);
   }
#endif /* SS_MULTICORE_SUPPORT */
#endif

   /*  All the System tasks(threads)  are created with same priority*/
   /* Needs discussion with Team : In order to reduce the number of 
      threads in our system, we are tieing all L3, Iu, CN side to same
      thread(system task). Essentially this will reduce number of 
      threads but impacts parallelism, since application threads involves
      IO operations. */

   SCreateSTsk(32, &rrc_s1ap_app_taskId); 
   sm_taskId = sctp_taskId = egtp_dat_app_taskId = rrc_s1ap_app_taskId;
   SCreateSTsk(32, &tucl_taskId);

   SCreateSTsk(32, &egtp_dat_app_taskId);
#ifdef SS_MULTICORE_SUPPORT
   /*Pin eGTP-U thread to Core 1.*/
#ifndef LTE_PAL_ENB
   SSetAffinity(&egtp_dat_app_taskId, SS_AFFINITY_MODE_SPECIFIC, 1, NULLP);
#endif
#endif /* SS_MULTICORE_SUPPORT */

   /* tucl_taskId = egtp_dat_app_taskId; */


   /* To Initialize SmCfg Cb params with default values */
   wrSmmInitSmCfgCb();/* To intialize wrNghCellCfg and SCTP Params */
   /* Read the parameters from the file and copy into global control block */
   if(smWrReadConfigParams() != ROK)
   {
      printf("FAILED TO READ FROM FILE wr_cfg.txt");
      RETVALUE(RFAILED);
   } /* end of if statement */

   printf("ipv4type=%d, ipv6type=%d\n", smCfgCb.enbIpAddr[WR_IPV4_IDX].type, smCfgCb.enbIpAddr[WR_IPV6_IDX].type);
   if(smCfgCb.enbIpAddr[WR_IPV4_IDX].type == CM_TPTADDR_IPV4)
     rlInitLog(smCfgCb.enbIpAddr[WR_IPV4_IDX].type);
   else if(smCfgCb.enbIpAddr[WR_IPV6_IDX].type == CM_TPTADDR_IPV6)
     rlInitLog(smCfgCb.enbIpAddr[WR_IPV6_IDX].type);
   else
   {
      printf("eNodeb IP not configured in wr_cfg. Taking IPV4 as default for socket creation \n");
      rlInitLog(CM_TPTADDR_IPV4);
   }

#ifndef WR_RSYS_OAM
#ifndef NL
   /* CSG_DEV : Validate Configration Parameters */
   if(smWrValidateConfigParams() != ROK)
   {
      RLOG0(L_FATAL, "Validation of Configuration Parameters Failed");
      flushData(SIGSEGV);
      RETVALUE(RFAILED);
   }
#endif
#endif

#ifdef WR_WATCHDOG
   wrRegWatchDog();
#endif

   /* Create RRC, RRC App and S1AP TAPA tasks and attach them 
      to a single system thread */
   if(smNhInit(rrc_s1ap_app_taskId) != ROK)
   {
      RLOG0(L_FATAL, "RRC Tapa Task initialization failed");
      RETVALUE(RFAILED);
   } /* end of if statement */

#ifdef WR_RELAY
   if(smWrInit(rrc_s1ap_app_taskId) != ROK )
   {
      RLOG0(L_FATAL, "RRC App Tapa Task initialization failed");
      RETVALUE(RFAILED);
   } /* end of if statement */
#endif /* end of WR_RELAY */
   if(smSzInit(rrc_s1ap_app_taskId) != ROK )
   {
      RLOG0(L_FATAL, "S1AP Tapa Task initialization failed");
      RETVALUE(RFAILED);
   } /* end of if statement */

   if(smRmInit(rrc_s1ap_app_taskId) != ROK )
   {
      RLOG0(L_FATAL, "RRM Tapa Task initialization failed");
      RETVALUE(RFAILED);
   } /* end of if statement */

#if 1
   if(smNlInit(rrc_s1ap_app_taskId) != ROK)
   {
      RLOG0(L_FATAL, "ESON Tapa Task initialization failed");
      RETVALUE(RFAILED);
   }
#endif

   if(smCzInit(rrc_s1ap_app_taskId) != ROK )
   {
      RLOG0(L_FATAL, "S1AP Tapa Task initialization failed");
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Create EGTP and EGTP APP TAPA tasks and attach them 
      to a single system thread */
   if(smEgInit(egtp_dat_app_taskId) != ROK)
   {
      RLOG0(L_FATAL, "EGTP Tapa Task initialization failed");
      RETVALUE(RFAILED);
   } /* end of if statement */
#ifdef EU_DAT_APP 
   if(smEuInit(egtp_dat_app_taskId) != ROK)
   {
      RLOG0(L_FATAL, "EGTP Data APP Tapa Task initialization failed");
      RETVALUE(RFAILED);
   } /* end of if statement */
#endif /* EU_DAT_APP */
   /* Create SCTP TAPA tasks and attach them to system thread */
   if(smSbInit(sctp_taskId) != ROK)
   {
      RLOG0(L_FATAL, "SCTP Tapa Task initialization failed");
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Create TUCL TAPA task and attach them to system threads */
   if(smHiInit(tucl_taskId) != ROK)
   {
      RLOG0(L_FATAL, "TUCL Tapa Task initialization failed");
      RETVALUE(RFAILED);
   } /* end of if statement */
   /* Create the SM TAPA task */
   SRegTTsk(ENTSM, (Inst) 0, TTNORM, PRIOR0, (PAIFS16)smActvInit, smActvTsk);
   SAttachTTsk(ENTSM, (Inst) 0, sm_taskId);

#ifdef WR_CLI_TARGET_BOARD
   SM_SET_ZERO(&cliPst, sizeof(Pst));
   /* Creating new trhead for cli command */
   if(SCreateSTsk(PRIOR0, &cli_taskId) != ROK)
   {
      RLOG0(L_FATAL, "Failed to Create System Task");
      RETVALUE(RFAILED);
   }
   if(SRegTTsk(ENTSM, (Inst) dstInst, TTNORM, PRIOR0, NULLP, smCliRecvTsk) != ROK)
   {
      RLOG0(L_FATAL, "Failed to Register Tapa Task");
      RETVALUE(RFAILED);
   }
   if(SAttachTTsk(ENTSM, (Inst) dstInst, cli_taskId) != ROK)
   {
      RLOG0(L_FATAL, "Failed to Attach Tapa Task");
      RETVALUE(RFAILED);
   }
   /* Self post to trigger system task */
   WR_FILL_PST(cliPst, SM_WR_PROC, ENTSM, ENTSM, EVTWRCLIRECV, WR_POOL, SM_SELECTOR_LC);
   cliPst.dstInst = dstInst; /* instance 2 */

   if(wrSmSelfPstMsg(&cliPst) != ROK)
   {
      RLOG0(L_FATAL, "Failed to do self post ");
      RETVALUE(RFAILED);
   }
#endif

   RLOG0(L_ALWAYS, "TAPA TASKS SUCCESSFULLY REGISTERED");

   /* Enable sm debugs */
   if(smCfgCb.smDbg == 1)
   {
      /* Update DbgMsk */
#ifdef DEBUGP
      smCb.init.dbgMask = 0xffffffff;
#endif
      RLOG0(L_INFO, "Enabled SM Debug Prints");
   } /* end of if statement */
   smCb.smState = WR_SM_STATE_INIT;
#ifndef LTE_ENB_PERF
//   smEvent = EVTWRTUCLCFG;
   smEvent = EVTWRSONCFG;
#else
#ifdef RM_INTF
   smEvent = EVTWRRRMCFG;
#else
   smEvent = EVTWRAPPCFG;
#endif /*RM_INTF*/
#endif

#ifdef WR_RSYS_OAM
   SCreateSmmAppTst(); 
#else
#ifdef E_TM
   if(wrSmDfltBootMode)
   {
      printf("Starting SM Process CB\n");
      smCb.smState = WR_SM_STATE_CL_INIT;
      smEvent = EVTWRCLCFG;
      smWrEtmProcSm(smEvent);
   }
   else
#endif
   {
      /* Invoke the state machine */
      printf("Starting SM Process CB\n");
      smWrProcSm(smEvent);
   }
#endif

#ifdef REM_ENABLE
   initNmmTask();
#endif

#ifdef SS_SEGV_SIG_HDLR
#ifndef SS_MULTIPLE_PROCS
   if(SRegIntrptHdlr((Ent)ENTWR, (Inst)0,wrSigSegvActvTsk) != ROK)
   {
      RLOG0(L_WARNING, "Failed to register Interrupt Hndler");
      RETVALUE(RFAILED);
   }
#endif /* SS_MULTIPLE_PROCS */
#endif /* SS_SEGV_SIG_HDLR */ 

   RETVALUE(ROK);

} /* end of tst function */
#endif
/*
*
*       Fun:  wrSmSelfPstMsg 
*
*       Desc:  This function is used by  Layer to self post 
*              a message
*
*       Ret:   None
*
*       Notes: None
*
*       File:  wr_smm_init.c
*
*/
#ifdef ANSI
PUBLIC S16 wrSmSelfPstMsg
(
Pst   *pst
)
#else
PUBLIC S16 wrSmSelfPstMsg(pst)
Pst   *pst;
#endif
{
   Buffer *mBuf;          /* message buffer */

   TRC2(wrSmSelfPstMsg);

   if(SGetMsg(DFLT_REGION, WR_POOL, &mBuf) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   } /* end of if statement */

   RETVALUE(SPstTsk(pst, mBuf));

} /* end of wrSmSelfPstMsg */
 
/**
 * @brief This function is used to send enable/disable control request to 
 * all the layers.
 *
 * @details
 *
 * Function: wrSmHdlEnbLog
 *
 * @param[in] event
 * @return Void
 */
#ifdef SS_DIAG
#ifdef ANSI
PUBLIC Void  wrSmHdlEnbLog
(
U8 event
)
#else
PUBLIC Void wrSmHdlEnbLog(event)
U8  event;
#endif /* ANSI */
{
   Pst lclPst;
   U32 logMask = 0x01;
   U8  maskIdx = 0;

   TRC2(wrSmHdlEnbLog);

   SM_SET_ZERO(&lclPst, sizeof(Pst));

   if(event == EVTWRENBLOG)
   {
       /*Send Control request to enable/disable log */
      SM_SND_DIAG_CNTRL_REQ(smCfgCb.diagInfo.tucl.diagLvl,
            smCfgCb.diagInfo.tucl.diagState, logMask, TUCL, maskIdx)

      SM_SND_DIAG_CNTRL_REQ(smCfgCb.diagInfo.sctp.diagLvl,
           smCfgCb.diagInfo.sctp.diagState, logMask, SCTP, maskIdx)

      SM_SND_DIAG_CNTRL_REQ(smCfgCb.diagInfo.s1ap.diagLvl,
           smCfgCb.diagInfo.s1ap.diagState, logMask, S1AP, maskIdx)

      SM_SND_DIAG_CNTRL_REQ(smCfgCb.diagInfo.egtp.diagLvl,
           smCfgCb.diagInfo.egtp.diagState, logMask, EGTP, maskIdx)

      SM_SND_DIAG_CNTRL_REQ(smCfgCb.diagInfo.x2ap.diagLvl,
           smCfgCb.diagInfo.x2ap.diagState, logMask, X2AP, maskIdx)

      SM_SND_DIAG_CNTRL_REQ(smCfgCb.diagInfo.app.diagLvl,
           smCfgCb.diagInfo.app.diagState, logMask , APP, maskIdx)

      SM_SND_DIAG_CNTRL_REQ(smCfgCb.diagInfo.lteRrc.diagLvl,
           smCfgCb.diagInfo.lteRrc.diagState, logMask, RRC, maskIdx)

      SM_SND_DIAG_CNTRL_REQ(smCfgCb.diagInfo.lteRlc.diagLvl,
           smCfgCb.diagInfo.lteRlc.diagState, logMask, RLC, maskIdx)
      
      SM_SND_DIAG_CNTRL_REQ(smCfgCb.diagInfo.ltePdcp.diagLvl,
           smCfgCb.diagInfo.ltePdcp.diagState, logMask, PDCP, maskIdx)

      SM_SND_DIAG_CNTRL_REQ(smCfgCb.diagInfo.lteMac.diagLvl,
           smCfgCb.diagInfo.lteMac.diagState, logMask, MAC, maskIdx)

      SM_SND_DIAG_CNTRL_REQ(smCfgCb.diagInfo.lteCl.diagLvl,
           smCfgCb.diagInfo.lteCl.diagState, logMask, CL, maskIdx)
   }
   else
   {
      RETVOID;
   } /* end of else part */

   /* Assuming after all log enable/disable Cntrl Reqs. */
   smCb.smState = WR_SM_STATE_ENB_LOG_DONE;

   WR_FILL_PST(lclPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRENBALRMS, WR_POOL, SM_SELECTOR_LC);

   if(wrSmSelfPstMsg(&lclPst) != ROK)
   {
      ALARM("Enabling log SUCCESSFULLY PERFORMED. Failure at Self Pst.\n");
   } /* end of if statement */

   RETVOID;
} /* end of wrSmHdlEnbLog */
#endif 

/*
*
*       Fun:   smWrProcSm
*
*       Desc:  This function is entry function to the  main State Machine
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/
#ifdef ANSI
PUBLIC Void smWrProcSm
(
U8 event
)
#else
PUBLIC Void smWrProcSm( event)
U8  event;
#endif /* ANSI */
{
   TRC2(smWrProcSm);

   //printf("%s: SM State [%d] : %s\n",__func__,event,wrSmStateStr[smCb.smState]);
   switch (smCb.smState)
   {
      case WR_SM_STATE_INIT:
         {
#ifndef LTE_ENB_PERF
#if 1
            wrSmHdlSonCfgEvent(event);
            break;
         }
      case WR_SM_STATE_SON_CFG_DONE:
         {
#endif

            printf("Configuring the Event %d \n",event);
            wrSmHdlTuclCfgEvent(event);
            break;
         }
      case WR_SM_STATE_TUCL_CFG_DONE:
         {
            wrSmHdlSctpCfgEvent(event);
            break;
         }
      case WR_SM_STATE_SCTP_CFG_DONE:
         {
            wrSmHdlS1apCfgEvent(event);
            break;
         }
      case WR_SM_STATE_S1AP_CFG_DONE:
         {
            wrSmHdlEgtpCfgEvent(event);
            break;
         }
      case WR_SM_STATE_EGTP_CFG_DONE:
         {
            wrSmHdlX2apCfgEvent(event);       
            break;
         }
      case WR_SM_STATE_X2AP_CFG_DONE:
         {
#endif /* LTE_ENB_PERF */
#ifdef RM_INTF
            wrSmHdlRrmCfgEvent(event);
#else
            wrSmHdlAppCfgEvent(event);
#endif /*RM_INTF*/
            break;
         }
#ifdef RM_INTF
      case WR_SM_STATE_RRM_CFG_DONE:
         {
            wrSmHdlAppCfgEvent(event);
            break;
         }
#endif /*RM_INTF*/
      case WR_SM_STATE_APP_CFG_DONE:
         {
            wrSmHdlRrcCfgEvent(event);
            break;
         }
      case WR_SM_STATE_RRC_CFG_DONE:
         {
            wrSmHdlPdcpUlCfgEvent(event);
            break;
         }
      case WR_SM_STATE_PDCP_UL_CFG_DONE:
         {
            wrSmHdlPdcpDlCfgEvent(event);
            break;
         }
      case WR_SM_STATE_PDCP_DL_CFG_DONE:
         {
            wrSmHdlRlcUlCfgEvent(event);
            break;
         }
      case WR_SM_STATE_RLC_UL_CFG_DONE:
         {
            wrSmHdlRlcDlCfgEvent(event);
            break;
         }
      case WR_SM_STATE_RLC_DL_CFG_DONE:
         {
            wrSmHdlMacCfgEvent(event);
            break;
         }
      case WR_SM_STATE_MAC_CFG_DONE:
         {
#ifndef LTE_ENB_PAL
            wrSmHdlCLCfgEvent(event);
            break;
         }
      case WR_SM_STATE_CL_CFG_DONE:
         {
#endif /* LTE_ENB_PAL */
            /* Initializing Dbg Mask to Zero */
            smCfgCb.smDbgMsk = 0;
            wrSmHdlEnbDebugs(event);
#ifdef TENB_STATS
            TSInfCfgCL();
#endif
            break;
         }
#ifdef SS_DIAG
      case WR_SM_STATE_ENB_DEBUGS_DONE:
         {
            wrSmHdlEnbLog(event);
            break;
         }
#endif
      case WR_SM_STATE_ENB_LOG_DONE:
         {
            wrSmHdlEnbAlarms(event);
            break;
         }
      case WR_SM_STATE_ENB_ALRMS_DONE:

         {
#ifndef LTE_ENB_PERF
            wrSmHdlBndS1apStack(event);
            break;
         }
      case WR_SM_STATE_S1AP_STK_BND_DONE:
         {
            wrSmHdlBndEgtpStack(event);
            break;
         }
      case WR_SM_STATE_EGTP_STK_BND_DONE:
         {
            wrSmHdlBndX2apStack(event);
            break;
         }
      case WR_SM_STATE_X2AP_STK_BND_DONE:
         {
#endif /* end of LTE_ENB_PERF */
            wrSmHdlBndRrcStack(event);
            break;
         }
      case WR_SM_STATE_RRC_STK_BND_DONE:
#ifdef RM_INTF
         {
            wrSmHdlBndRrmStack(event);
            break;
         }
      case WR_SM_STATE_RRM_STK_BND_DONE:
#endif
         {
#ifndef LTE_ENB_PERF
            wrSmHdlInitS1Setup(event);
#else
            smCb.smState = WR_SM_STATE_AWAIT_CELL_UP;
            wrSmHdlInitCellCfg(EVTWRCELLCFG);
#endif
            break;
         }
      case WR_SM_STATE_CELL_UP:
         {
#ifdef WR_RSYS_KPI
            if(kpiInfo.startCollec == FALSE)
            {
#ifdef LTE_L2_MEAS
               kpiInfo.l2CollecEna = TRUE;
               wrKpiStartKpiCollecPrc(kpiInfo.l2CollecEna); 
#endif
            }
#endif 
#ifdef RM_INTF
            wrSmSndRrmCellUpInd(event);
#endif
//        wrSmSndSonCellUpInd(event);
        /*trigger REM Scan related configuration to BRDCM-CL*/
//        SmMiLysREMScanCfgReq(smCb.cfgCb.remScanInterval,
//                smCb.cfgCb.remScanCount);

#ifdef WR_RSYS_OAM
            wrSmConfigComplete();
            break;
         }
      case WR_SM_STATE_OAM_DYN_START:
         {
#endif
            wrSmHdlInitX2Setup(event);
            break;
         }
      default:
         {
            RLOG0(L_ERROR, "smWrProcSm() Invalid  state received");
            break;
         }
   }
   RETVOID;
} /* smWrProcSm */

/*
*
*       Fun:   wrSmHdlUnBndRrmStack
*
*       Desc:  This function handles unbind of RRM stack
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmHdlUnBndRrmStack
(
U8 event
)
#else
PUBLIC Void wrSmHdlUnBndRrmStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlUnBndRrmStack);

   if(event == EVTWRRRMSTKUNBND)
   {
      /* UnBind RRM to MAC */
      smBindUnBindRmToRgmSap(AUBND);
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlUnBndRrmStack */

/*
*
*       Fun:   wrSmHdlUnBndRrcStack
*
*       Desc:  This function handles unbind of RRC stack
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmHdlUnBndRrcStack
(
U8 event
)
#else
PUBLIC Void wrSmHdlUnBndRrcStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlUnBndRrcStack);

   if(event == EVTWRRRCSTKUNBND)
   {
      /* UnBind APP to MAC */
      smBindUnBindWrToRgrSap(AUBND);
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlUnBndRrcStack */

/*
*
*       Fun:   wrSmHdlUnBndX2apStack
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmHdlUnBndX2apStack
(
U8 event
)
#else
PUBLIC Void wrSmHdlUnBndX2apStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlUnBndX2apStack);

   if(event == EVTWRX2APSTKUNBND)
   {
      /* UnBind APP to X2AP */
      smBindUnBindWrToCztSap(AUBND_DIS);
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlUnBndX2apStack */

/*
*
*       Fun:   wrSmHdlUnBndEgtpStack
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmHdlUnBndEgtpStack
(
U8 event
)
#else
PUBLIC Void wrSmHdlUnBndEgtpStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlUnBndEgtpStack);

   if(event == EVTWREGTPSTKUNBND)
   {
      /* UnBind APP to EGTP */
      smBindUnBindWrToEgtSap(AUBND_DIS);
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlUnBndEgtpStack */

/*
*
*       Fun:   wrSmHdlUnBndS1apStack
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmHdlUnBndS1apStack
(
U8 event
)
#else
PUBLIC Void wrSmHdlUnBndS1apStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlUnBndS1apStack);

   if(event == EVTWRS1APSTKUNBND)
   {
      /* UnBind APP to S1AP */
      smBindUnBindWrToSztSap(AUBND_DIS);
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlUnBndS1apStack */


#ifdef ANSI
PUBLIC Void smWrShutdownSm
(
U8 event
)
#else
PUBLIC Void smWrShutdownSm()
U8  event;
#endif /* ANSI */
{
#ifndef WR_RSYS_OAM
   U16 smevent;
#endif
   TRC2(smWrShutdownSm);

   switch (smCb.smState)
   {   
      case WR_SM_STATE_DINIT:
         {
            /* Unbind RRM stack */
            RLOG0(L_DEBUG,"Unbinding RRM stack");
            wrSmHdlUnBndRrmStack(event);
            break;
         }

      case WR_SM_STATE_RRM_STK_UNBND_DONE:
         {
            /* Unbind RRC stack */
            RLOG0(L_DEBUG,"Unbinding RRC stack");
            wrSmHdlUnBndRrcStack(event);
            break;
         }
      case WR_SM_STATE_RRC_STK_UNBND_DONE:
         {
            /* Unbind X2AP stack */
            RLOG0(L_DEBUG,"Unbinding X2AP stack");
            wrSmHdlUnBndX2apStack(event);
            break;
         }
      case WR_SM_STATE_X2AP_STK_UNBND_DONE:
         {
            /* Unbind EGTP stack */
            RLOG0(L_DEBUG,"Unbinding EGTP stack");
            wrSmHdlUnBndEgtpStack(event);
            break;
         }
      case WR_SM_STATE_EGTP_STK_UNBND_DONE:
         {
            /* Unbind S1AP stack */
            RLOG0(L_DEBUG,"Unbinding S1AP stack");
            wrSmHdlUnBndS1apStack(event);
            break;
         }
      case WR_SM_STATE_S1AP_STK_UNBND_DONE:
         {
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to RRM");
            smBuildRmShutDownCntrl();
            break;
         }
      case WR_SM_STATE_RRM_SHUTDWN_DONE:
         {
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to APP");
            smBuildWrShutDownCntrl();
            break;
         }
      case WR_SM_STATE_APP_SHUTDWN_DONE:
         {
#if 1
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to SON");
            smBuildNlShutDownCntrl();
            break;
         }
      case WR_SM_STATE_SON_SHUTDWN_DONE:
         {
#endif
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to RRC");
            smBuildNhShutDownCntrl();
            break;
         } 
      case WR_SM_STATE_RRC_SHUTDWN_DONE:
         {
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to PDCP DL");
            smBuildPjShutDownCntrl(WR_SM_PJDL_INST);
            break;
         }
      case WR_SM_STATE_PDCP_DL_SHUTDWN_DONE:
         {
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to PDCP UL");
            smBuildPjShutDownCntrl(WR_SM_PJUL_INST);
            break;
         }
      case WR_SM_STATE_PDCP_UL_SHUTDWN_DONE:
         {
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to RLC DL");
            smBuildKwShutDownCntrl((Inst)WR_SM_KWDL_INST);
            break;
         }
      case WR_SM_STATE_RLC_DL_SHUTDWN_DONE:
         {
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to RLC UL");
            smBuildKwShutDownCntrl((Inst)WR_SM_KWUL_INST);
            break;
         }
      case WR_SM_STATE_RLC_UL_SHUTDWN_DONE:
         {
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to SCH");
            smBuildRgSCHShutDownCtrl();
            break;
         }
      case WR_SM_STATE_MAC_SCH_SHUTDWN_DONE:
         {
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to MAC ");
            smBuildRgShutDownCntrl();
            break;
         }  
      case WR_SM_STATE_MAC_SHUTDWN_DONE:
         {
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to EGTP");
            smBuildWrUnbindEg();
            /* cross verify SCHEDULER release/reconfiguration */
            smBuildEgShutDownCntrl();
            break;
         }

      case  WR_SM_STATE_EGTP_SHUTDWN_DONE:
         {
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to X2AP");
            smBuildWrUnbindCz();
            smBuildCzShutDownCntrl();
            break;
         }
      case WR_SM_STATE_X2AP_SHUTDWN_DONE:
         {
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to S1AP");
            smBuildWrUnbindSz();
            smBuildSzShutDownCntrl();
            break;
         }

      case WR_SM_STATE_S1AP_SHUTDWN_DONE:
         {
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to SCTP");
            smBuildSbShutDownCntrl();
            break;
         }

      case WR_SM_STATE_SCTP_SHUTDWN_DONE:
         {
            RLOG0(L_ALWAYS,"SENDING SHUTDOWN REQUEST to TUCL");
            smBuildHiShutDownCntrl();
            break;
         }
      case  WR_SM_STATE_TUCL_SHUTDWN_DONE:
         {
#ifdef WR_RSYS_OAM
            RLOG0(L_ALWAYS, "All layers Shutdown completed");
            MsmShutDownAck();
#else
            smCb.smState = WR_SM_STATE_INIT;
//            smevent = EVTWRTUCLCFG;
            smevent = EVTWRSONCFG;
            smWrProcSm(smevent);
            RLOG0(L_DEBUG, "Reinit Started of Stack");
#endif
            break;
         }
      default:
         {
            RLOG0(L_ERROR, "Invalid state recevied");
            break;
         }

   }
   RETVOID;
}
#ifndef LTE_ENB_PERF
/*
*
*       Fun:   wrSmHdlTuclCfgEvent
*
*       Desc:  This function is handles events in Init state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/


#ifdef ANSI
PUBLIC Void wrSmHdlTuclCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlTuclCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2 (wrSmHdlTuclCfgEvent);

   if(event == EVTWRTUCLCFG)
   {
      /* Start TUCL Configuration */
      wrSmHiCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlTuclCfgEvent */
/*
*
*       Fun:   wrSmHdlSctpCfgEvent
*
*       Desc:  This function is handles events in TUCL Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/


#ifdef ANSI
PUBLIC Void wrSmHdlSctpCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlSctpCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlSctpCfgEvent);

   if(event == EVTWRSCTPCFG )
   {
      /* Start SCTP Configuration */
      wrSmSbCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlSctpCfgEvent */

/*
*
*       Fun:   wrSmHdlS1apCfgEvent
*
*       Desc:  This function is handles events in SCTP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/


#ifdef ANSI
PUBLIC Void wrSmHdlS1apCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlS1apCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlS1apCfgEvent);

   if(event == EVTWRS1APCFG )
   {
      /* Start S1AP Configuration */
      wrSmSzCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlS1apCfgEvent */

#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   wrSmHdlX2apCfgEvent
*
*       Desc:  This function is handles events in X2AP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/


#ifdef ANSI
PUBLIC Void wrSmHdlX2apCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlX2apCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlX2apCfgEvent);

   if(event == EVTWRX2APCFG )
   {
      /* Start X2AP Configuration */
      wrSmCzCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlX2apCfgEvent */
#endif

/*
*
*       Fun:   wrSmHdlEgtpCfgEvent
*
*       Desc:  This function is handles events in S1AP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/
#ifdef ANSI
PUBLIC Void wrSmHdlEgtpCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlEgtpCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlEgtpCfgEvent);

   if(event == EVTWREGTPCFG )
   {
      /* Start EGTP Configuration */
      wrSmEgCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlEgtpCfgEvent */
#endif

/*
*
*       Fun:   wrSmHdlSonCfgEvent
*
*       Desc:  This function handles events once eNB APP Config done and
*              initiate to start SON configuration
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/


#ifdef ANSI
PUBLIC Void wrSmHdlSonCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlSonCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlSonCfgEvent);

   if(event == EVTWRSONCFG )
   {
      /* Start SON Configuration */
      wrSmNlCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received for SON config", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlSonCfgEvent */

/*
*
*       Fun:   wrSmSndSonCellUpInd
*
*       Desc:  This function sends cell up indication to SON module
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/
#ifdef ANSI
PUBLIC Void wrSmSndSonCellUpInd
(
U8 event
)
#else
PUBLIC Void wrSmSndSonCellUpInd(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmSndSonCellUpInd);

   /*send cell up indication to SON module */
   smSendCellUpIndToSon();

   RETVOID;
} 


#ifdef RM_INTF
/*
*
*       Fun:   wrSmHdlRrmCfgEvent
*
*       Desc:  This function is handles events in EGTP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/


#ifdef ANSI
PUBLIC Void wrSmHdlRrmCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlRrmCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlRrmCfgEvent);

   if(event == EVTWRRRMCFG )
   {
      /* Start Application Configuration */
      wrSmRmCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlRrmCfgEvent */
#endif /*RM_INTF*/

/*
*
*       Fun:   wrSmHdlAppCfgEvent
*
*       Desc:  This function is handles events in EGTP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmHdlAppCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlAppCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlAppCfgEvent);

   if(event == EVTWRAPPCFG )
   {
      /* Start Application Configuration */
      wrSmWrCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlAppCfgEvent */

/*
*
*       Fun:   wrSmHdlRrcCfgEvent
*
*       Desc:  This function is handles events in EGTP App Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmHdlRrcCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlRrcCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlRrcCfgEvent);

   if(event == EVTWRRRCCFG )
   {
      /* Start RRC Configuration */
      wrSmNhCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlRrcCfgEvent */

#ifdef PHY_ERROR_LOGING
/*
*       Fun:   wrSmSendUlAllocCfgReq
*
*       Desc:  This function is sends events for UL Alloc Configuration 
*
*       Ret:   None
*
*       Notes: <NONE>
*
*/
#ifdef ANSI
PUBLIC Void wrSmSendUlAllocCfgReq
(
SmPhyCmd phyCmd
)
#else
PUBLIC Void wrSmSendUlAllocCfgReq(phyCmd)
SmPhyCmd phyCmd;
#endif /* ANSI */
{
   TRC2(wrSmSendUlAllocCfgReq);

   /* Start Sch Alloc Configuration */
   wrSmRgSchUlAllocCfg(phyCmd);

   RETVOID;
} /* end of wrSmSendUlAllocCfgReq*/
#endif /* PHY_ERROR_LOGING */
/*
*
*       Fun:   wrSmHdlPdcpUlCfgEvent
*
*       Desc:  This function is handles events in RRC APP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_enb_init.c
*
*/


#ifdef ANSI
PUBLIC Void wrSmHdlPdcpUlCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlPdcpUlCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlPdcpCfgEvent);

   if(event == EVTWRPDCPCFG )
   {
      /* Start PDCP Configuration */
      wrSmPjUlCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlPdcpCfgEvent */

/*
*
*       Fun:   wrSmHdlPdcpDlCfgEvent
*
*       Desc:  This function is handles events in RRC APP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_enb_init.c
*
*/


#ifdef ANSI
PUBLIC Void wrSmHdlPdcpDlCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlPdcpDlCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlPdcpDlCfgEvent);

   if(event == EVTWRPDCPCFG )
   {
      /* Start PDCP Configuration */
      wrSmPjDlCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlPdcpCfgEvent */

/*
*
*       Fun:   wrSmHdlRlcUlCfgEvent
*
*       Desc:  This function is handles events in PDCP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_enb_init.c
*
*/


#ifdef ANSI
PUBLIC Void wrSmHdlRlcUlCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlRlcUlCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlRlcUlCfgEvent);

   if(event == EVTWRRLCCFG )
   {
      /* Start RLC Configuration */
      wrSmKwUlCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlRlcCfgEvent */

/*
*
*       Fun:   wrSmHdlRlcDlCfgEvent
*
*       Desc:  This function is handles events in PDCP Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_enb_init.c
*
*/


#ifdef ANSI
PUBLIC Void wrSmHdlRlcDlCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlRlcDlCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlRlcDlCfgEvent);

   if(event == EVTWRRLCCFG )
   {
      /* Start RLC Configuration */
      wrSmKwDlCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlRlcCfgEvent */

/*
*
*       Fun:   wrSmHdlMacCfgEvent
*
*       Desc:  This function is handles events in RLC Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmHdlMacCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlMacCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlMacCfgEvent);

   if(event == EVTWRMACCFG )
   {
      /* Start LTE MAC Configuration */
      wrSmRgCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlMacCfgEvent */

#ifndef LTE_ENB_PAL
/*
*
*       Fun:   wrSmHdlCLCfgEvent
*
*       Desc:  This function is handles events in MAC Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmHdlCLCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlCLCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlCLCfgEvent);

   if(event == EVTWRCLCFG )
   {
      /* Start LTE Convergence Layer Configuration */
      wrSmYsCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlCLCfgEvent */
#endif

#ifndef LTE_ENB_PERF
/*
*
*       Fun:   wrSmHdlBndS1apStack
*
*       Desc:  This function is handles events in CL Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/


#ifdef ANSI
PUBLIC Void wrSmHdlBndS1apStack
(
U8 event
)
#else
PUBLIC Void wrSmHdlBndS1apStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlBndS1apStack);

   if(event == EVTWRS1APSTKBND)
   {
      /*Bind the SCTP lower SAP with TUCL */
      smBindUnBindSbToLSap(SB_HI_SPID,ABND_ENA);
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlCLCfgEvent */

/*
*
*       Fun:   wrSmHdlBndEgtpStack
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmHdlBndEgtpStack
(
U8 event
)
#else
PUBLIC Void wrSmHdlBndEgtpStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlBndEgtpStack);

   if(event == EVTWREGTPSTKBND)
   {
      /*Bind the EGTP lower SAP with */
      smBindUnBindEgToHitSap(EG_HI_SPID,ABND_ENA);
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlBndEgtpStack */

#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   wrSmHdlBndX2apStack
*
*       Desc:  This function is handles events in Egtp Stack Bind  done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/


#ifdef ANSI
PUBLIC Void wrSmHdlBndX2apStack
(
U8 event
)
#else
PUBLIC Void wrSmHdlBndX2apStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlBndX2apStack);

   if(event == EVTWRX2APSTKBND)
   {
      /*Bind the X2AP lower SAP with SCTP */
      smBindUnBindCzToLSap(ABND_ENA);
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlCLCfgEvent */
#endif /* LTE_HO_SUPPORT */
#endif /* LTE_ENB_PERF */

/*
*
*       Fun:   wrSmHdlBndRrmStack
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmHdlBndRrmStack
(
U8 event
)
#else
PUBLIC Void wrSmHdlBndRrmStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlBndRrmStack);

   if(event == EVTWRRRMSTKBND)
   {
      /*Bind the RRM with MAC */
      smBindUnBindRmToRgmSap(ABND);
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlBndRgmStack */


/*
*
*       Fun:   wrSmSndRrmCellUpInd
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_enb_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmSndRrmCellUpInd
(
U8 event
)
#else
PUBLIC Void wrSmSndRrmCellUpInd(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlBndRrcStack);

   /*Bind the MAC lower SAP with CL */
   smSendCellUpIndToRrm();

   RETVOID;
} /* end of wrSmHdlBndRgmStack */




/*
*
*       Fun:   wrSmHdlBndRrcStack
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmHdlBndRrcStack
(
U8 event
)
#else
PUBLIC Void wrSmHdlBndRrcStack(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlBndRrcStack);

   if(event == EVTWRRRCSTKBND)
   {
      /*Bind the MAC lower SAP with CL */
      smBindUnBindRgToTfuSap(ABND);
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlBndEgtpStack */

/*
*
*       Fun:   wrSmHdlEnbAlarms
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void  wrSmHdlEnbAlarms
(
U8 event
)
#else
PUBLIC Void wrSmHdlEnbAlarms(event)
U8  event;
#endif /* ANSI */
{
   Pst  wrPst;

   TRC2(wrSmHdlEnbAlarms);
   SM_SET_ZERO(&wrPst, sizeof(Pst));

   if(event == EVTWRENBALRMS)
   {
      /* Send Control request to enable alarms in reqd. layers */
#ifndef LTE_ENB_PERF
      smBuildSzGenCntrl();

#ifdef LTE_HO_SUPPORT
      smBuildCzGenCntrl();
#endif
#endif
      smBuildNhGenCntrl();
      smBuildWrGenCntrl();
      smBuildEgGenCntrl();

      smCb.smState = WR_SM_STATE_ENB_ALRMS_DONE;
      /* Trigger to next state */
#ifndef LTE_ENB_PERF
      WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRS1APSTKBND, WR_POOL, SM_SELECTOR_LC);
#else
      WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRRRCSTKBND, WR_POOL, SM_SELECTOR_LC);
#endif

      if(wrSmSelfPstMsg(&wrPst) != ROK)
      {
         RLOG0(L_ERROR, "ENABLING OF ALARMS IS SUCCESSFULLY PERFORMED. Failure at Self Pst.");
      }
      else
      {
         RLOG0(L_INFO, "ENABLING OF ALARMS IS SUCCESSFULLY PERFORMED. ");
      } /* end of else part */

   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */
   smBuildNhGenCntrl();
   RETVOID;
} /* end of wrSmHdlEnbAlarms */

/*
 *
 *       Fun:   wrSmHdlEnbDebugs
 *
 *       Desc:  This function is handles events in done state
 *
 *       Ret:   None
 *
 *       Notes: <NONE>
 *
 *       File:  wr_smm_init.c
 *
 */

#ifdef ANSI
PUBLIC Void wrSmHdlEnbDebugs
(
U8 event
)
#else
PUBLIC Void wrSmHdlEnbDebugs(event)
U8  event;
#endif /* ANSI */
{
   Pst lclPst;

   TRC2(wrSmHdlEnbDebugs);

   SM_SET_ZERO(&lclPst, sizeof(Pst));

   if(event == EVTWRENBDEBUGS)
   {
#ifndef LTE_ENB_PERF
      /* Send Control request to enable debugs */
      if((smCfgCb.hiDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_HI_DBG_MASK))
      {
         RLOG0(L_INFO, "Enable HI Debug Prints");
         smBuildHiDbgCntrl();
         RETVOID;
      } /* end of if statement */
      /* Send Control request to enable debugs */
      if((smCfgCb.sbDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_SB_DBG_MASK))
      {
         RLOG0(L_INFO, "Enable SB Debug Prints");
         smBuildSbDbgCntrl();
         RETVOID;
      } /* end of if statement */
      /* Send Control request to enable debugs */
      if((smCfgCb.szDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_SZ_DBG_MASK))
      {
         RLOG0(L_INFO, "Enable SZ Debug Prints");
         smBuildSzDbgCntrl();
         RETVOID;
      } /* end of if statement */
      /* Send Control request to enable debugs */
      if((smCfgCb.egDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_EG_DBG_MASK))
      {
         RLOG0(L_INFO, "Enable EG Debug Prints");
         smBuildEgDbgCntrl();
         RETVOID;
      } /* end of if statement */

#ifdef LTE_HO_SUPPORT
      /* Send Control request to enable debugs */
      if((smCfgCb.czDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_CZ_DBG_MASK))
      {
         RLOG0(L_INFO, "Enable CZ Debug Prints");
         smBuildCzDbgCntrl();
         RETVOID;
      } /* end of if statement */
#endif
#endif
      /* Send Control request to enable debugs */
      if((smCfgCb.wrDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_WR_DBG_MASK))
      {
         RLOG0(L_INFO, "Enable WR Debug Prints");
         smBuildWrDbgCntrl();
         RETVOID;
      } /* end of if statement */
      /* Send Control request to enable debugs */
      if((smCfgCb.nhDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_NH_DBG_MASK))
      {
         RLOG0(L_INFO, "Enable NH Debug Prints");
         smBuildNhDbgCntrl();
         RETVOID;
      } /* end of if statement */
      /* Send Control request to enable debugs */
      if((smCfgCb.kwDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_KW_DBG_MASK))
      {
         RLOG0(L_INFO, "Enable KW  Debug Prints");
         smBuildKwDbgCntrl();
         RETVOID;
      } /* end of if statement */
      /* Send Control request to enable debugs */
      if((smCfgCb.rgDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_RG_DBG_MASK))
      {
         RLOG0(L_INFO, "Enable RG  Debug Prints");
         smBuildRgDbgCntrl();
         RLOG0(L_INFO, "Enable Schd  Debug Prints");
         smBuildSchdDbgCntrl();
         RETVOID;
      } /* end of if statement */
#ifdef YS_PICO
      /* Send Control request to enable debugs */
      if((smCfgCb.ysDbg == 1) && !(smCfgCb.smDbgMsk & WR_SM_YS_DBG_MASK))
      {
         RLOG0(L_ERROR, "Enable YS  Debug Prints");
         smBuildYsDbgCntrl();
         RETVOID;
      } /* end of if statement */
#endif /* YS_PICO */ 
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
      RETVOID;
   } /* end of else part */


   /* Assuming after all Debug Cntrl Reqs. */
   smCb.smState = WR_SM_STATE_ENB_LOG_DONE;
   WR_FILL_PST(lclPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRENBALRMS, WR_POOL, SM_SELECTOR_LC);
   if(wrSmSelfPstMsg(&lclPst) != ROK)
   {
      RLOG0(L_WARNING, "Enabling Debug Prints SUCCESSFULLY PERFORMED. "
         "Failure at Self Pst.");
   } /* end of if statement */

   RETVOID;
} /* end of wrSmHdlEnbDebugs */

/*
*
*       Fun:   wrSmHdlInitS1Setup
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void  wrSmHdlInitS1Setup
(
U8 event
)
#else
PUBLIC Void wrSmHdlInitS1Setup(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlInitS1Setup);

   if(event == EVTWRINITS1SETUP)
   {
      /* Send Control request to APP to indicate binding is complete*/
      smBuildWrS1InitCntrl();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlInitS1Setup*/

/*
*
*       Fun:   wrSmHdlInitCellCfg
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void  wrSmHdlInitCellCfg
(
U8 event
)
#else
PUBLIC Void wrSmHdlInitCellCfg(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlInitCellCfg);

   if(event == EVTWRCELLCFG)
   {
      /* Send Control request to APP to indicate binding is complete*/
      smBuildWrCellCntrl();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlInitCellCfg */

#ifdef LTE_HO_SUPPORT
/*
*
*       Fun:   wrSmHdlInitX2Setup
*
*       Desc:  This function is handles events in done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void  wrSmHdlInitX2Setup
(
U8 event
)
#else
PUBLIC Void wrSmHdlInitX2Setup(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlInitX2Setup);

   if(event == EVTWRINITX2SETUP)
   {
      /* Send Control request */
      smBuildWrX2InitCntrl();
   }
#ifdef WR_RSYS_OAM
   else if(event == EVTWRSTARTDYNUPDATE)
   {
     MsmConfigComplete();
   }
#endif
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlInitX2Setup*/
#endif

/*
*
*       Fun:  wrSmmSetBandwidth 
*
*       Desc:  Function which changes the bandwidth prameter depending on the 
*              bandwidth configured. Currently function supports 5MHZ and 10MHZ
*              configuration only
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  wr_smm_init.c
*
*/

PUBLIC Void wrSmmSetBandwidth
(
U32    freqBandwidth
)
{

   switch(freqBandwidth)
   {
      case WR_FREQ_BANDWIDTH_5MHZ:
      {
         wrSmDfltCellDlBW        = CTF_BW_RB_25;      /* 25 RBS */ 
         wrSmDfltCellUlBW        = CTF_BW_RB_25;      /* 25 RBS */ 
         wrSmDfltMaxUlBwPerUe    = 25;
         wrSmDfltMaxDlBwPerUe    = 25;
         wrSmDfltMaxDlRetxBw     = 25;
         wrSmDfltPuschSubBndStrt = 2;
         wrSmDfltPuschNoSubbnds  = 4;
         wrSmDfltPuschSubBandSz  = 5;
         /*To solve UE attach issue raised in defect ccpu00139234*/
 		   wrSmDfltZeroCorrelZoneCfg = 0x0A;        
         /*ccpu00130314:  Fix for 5MHZ RACH issue. for 5MHZ range for FreqOffset is 0-19*/ 
         wrSmDfltPrachFreqOffset   = 0x00;
          
	     wrSmNumUesGrpPwrCtrlFmt3  = 12; 
	     wrSmNumUesGrpPwrCtrlFmt3A = 25; 
      }
      break;
      /* By default the configuration is made for 10 Mhz, still overwriting the 
       * values*/
      case WR_FREQ_BANDWIDTH_10MHZ:
      {
         wrSmDfltCellDlBW        = CTF_BW_RB_50;      /* 50 RBS */ 
         wrSmDfltCellUlBW        = CTF_BW_RB_50;      /* 50 RBS */ 
         wrSmDfltMaxUlBwPerUe    = 50;
         wrSmDfltMaxDlBwPerUe    = 50;
         wrSmDfltMaxDlRetxBw     = 50;
         wrSmDfltPuschSubBndStrt = 1;
         /* ccpu00135759 */
         wrSmDfltPuschNoSubbnds  = 15;
         wrSmDfltPuschSubBandSz  = 3;
        /* Note :- wrSmDfltZeroCorrelZoneCfg not required for 10 Mhz as 
                   default configuration is 10 Mhz.
        */ 
	     wrSmNumUesGrpPwrCtrlFmt3  = 13; 
	     wrSmNumUesGrpPwrCtrlFmt3A = 27; 
      }
      break;
      case WR_FREQ_BANDWIDTH_15MHZ:
      {
         wrSmDfltCellDlBW        = CTF_BW_RB_75;      /* 75 RBS */ 
         wrSmDfltCellUlBW        = CTF_BW_RB_75;      /* 75 RBS */ 
         wrSmDfltMaxUlBwPerUe    = 75;
         wrSmDfltMaxDlBwPerUe    = 75;
         wrSmDfltMaxDlRetxBw     = 75;
         wrSmDfltPuschSubBndStrt = 1;
         //wrSmDfltPuschNoSubbnds  = 8;
         wrSmDfltPuschNoSubbnds  = 12;
         wrSmDfltPuschSubBandSz  = 6;
	     wrSmNumUesGrpPwrCtrlFmt3  = 13; 
	     wrSmNumUesGrpPwrCtrlFmt3A = 27; 
      }
      break;
      case WR_FREQ_BANDWIDTH_20MHZ:
      {
         wrSmDfltCellDlBW        = CTF_BW_RB_100;      /* 50 RBS */ 
         wrSmDfltCellUlBW        = CTF_BW_RB_100;      /* 50 RBS */ 
         wrSmDfltMaxUlBwPerUe    = 100;
         wrSmDfltMaxDlBwPerUe    = 100;
         wrSmDfltMaxDlRetxBw     = 100;
         wrSmDfltPuschSubBndStrt = 2;
         wrSmDfltPuschNoSubbnds  = 16;
         wrSmDfltPuschSubBandSz  = 6;
	     wrSmNumUesGrpPwrCtrlFmt3  = 14; 
	     wrSmNumUesGrpPwrCtrlFmt3A = 28; 
      }
      break;
   }

   RETVOID;
}
/*
*
*       Fun:   wrCfgGetTagNum
*
*       Desc:  Retrive the Tag number corresponds to the tag string.
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  wr_smm_init.c
*
*/
#ifdef ANSI
PUBLIC Void smWrCfgGetTagNum
(
U8* tag,
U16* tagNum
)
#else
PUBLIC Void smWrCfgGetTagNum(tag, tagNum)
U8* tag;
U16* tagNum;
#endif
{
   S16 loopCnt = 0;

   TRC2(smWrCfgGetTagNum);

   for (loopCnt = 0; loopCnt < (MAX_CFG_PARAM); loopCnt++)
   {
      if(wrCfgTagLst[loopCnt] == NULLP)
      {
         break;
      }
      if (0 == strcmp((S8 *)tag,wrCfgTagLst[loopCnt]))
      {
         *tagNum = loopCnt;
         break;
      }
   }/* End of for */
   RETVOID;
}/* End of wrCfgGetTagNum()*/

/*
*
*       Fun:   wrCfgValdtInput
*
*       Desc:  This function validates the format of the input and 
*              throws an error in case it's wrong.
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  wr_smm_init.c
*
*/
#ifdef ANSI
PUBLIC Void wrCfgValdtInput
(
U8* input,
Bool* status,
U8* noOfElem
)
#else
PUBLIC Void wrCfgValdtInput(input, status, noOfElem)
U8* input;
Bool* status;
U8* noOfElem;
#endif
{
   U16 strLen = 0;
   S16 loopCnt = 0;
   U8 lCnt = 0;
   U8 rCnt = 0;
   U8 dupCnt = 0;   
   U8 numOfComa = 0;
   TRC2(wrCfgValdtInput);
   strLen = strlen((S8 *)input);
   while (loopCnt < strLen)
   {
      if ('{' == input[loopCnt])
      {
        lCnt++;
      }
      else if ('}' == input[loopCnt])
      {
        rCnt++;
        /* Calculating the number of inner structures and arrays */
        if ((lCnt - rCnt) >= 2)
        {
           dupCnt++;
        }
      }
      else if (' ' == input[loopCnt])
      {
         RLOG0(L_ERROR, " Please check the "
           "Configuration format.There should not be any space in the configuration values.");
         *status = FALSE;
         break;
      }
      else if ((',' == input[loopCnt]) && (',' == input[loopCnt + 1]))      
      {
         RLOG0(L_ERROR, " Please check the "
         "Configuration format.Parameter value is  missing between"
         " two commo's.");
         *status = FALSE;
         break;
      }
      else if (('}' == input[loopCnt]) && ('{' == input[loopCnt + 1]))      
      {
         RLOG0(L_ERROR, " Please check the Configuration format.commo is missing.");
         *status = FALSE;
         break;
      }
      else if (',' == input[loopCnt])
      {
         numOfComa++;
      }
      loopCnt++;
   }/* End of while()*/
   if (FALSE != *status)
   {
      if (lCnt != rCnt)
      {
         RLOG0(L_ERROR, " Please check the"
         " Configuration format. '{' or '}' missed out or remove the space if presents");
         *status = FALSE;
      }
      else
      {
         /* Condition to check whether the input is a simple array of system
            defined data types or a single user defined structure.*/

         if (lCnt == 1)
         {
            /* Calculating the number of elements in the array or structure using the 
               number of commas */
            *noOfElem = numOfComa + 1;
            *status = TRUE;
         }
         else
         {
            /* Calculating the number of structures if the input is array 
             * of structures. deduction of 1 is for the outer most braces and 
             * deduction of dupCnt is for inner structures if any */
            *noOfElem = (lCnt - 1) - dupCnt;
            *status = TRUE;
         }
      }
   }/* End of if ()*/
   RETVOID;
}/* End of wrCfgValdtInput() */

/*
*
*       Fun:   wrCfgGetParamVal
*
*       Desc:  This function parses the validated input and makes the
*              the list of parameters and returns the list and the number of parameters.
*
*       Ret:   Void
*
*       Notes: None
*
*       File:  wr_smm_init.c
*
*/
#ifdef ANSI
PUBLIC Void wrCfgGetParamVal
(
U8* tagVal,
U8 paramLst[][50],
U16* noOfParams
)
#else
PUBLIC Void wrCfgGetParamVal(tagVal, paramLst[][50], noOfParams)
U8* tagVal;
U8 paramLst[][50];
U16* noOfParams;
#endif
{
   U16 strLen = 0;
   S16 loopCnt = 0;
   U8 loopCnt1 = 0;
   U8 param[50];

   TRC2(wrCfgGetParamVal);

   strLen = strlen((S8 *)tagVal);
   while (loopCnt < strLen)
   {
      if (('{' != tagVal[loopCnt]) && ('}' != tagVal[loopCnt]) && 
         (',' != tagVal[loopCnt]))   
      {
         loopCnt1 = 0;
         while((',' != tagVal[loopCnt]) && ('}' != tagVal[loopCnt]))
         {
            param[loopCnt1++] = tagVal[loopCnt];
            loopCnt++;
         }
         param[loopCnt1] = '\0'; 
         /* Increment the param count for this parameter */
         strcpy((S8 *)paramLst[*noOfParams], (S8 *)param);
         (*noOfParams)++;
      }
      else
      {
         /* skipping the charecter */
         loopCnt += 1;
      }
   }/* End of while()*/
   RETVOID;
}/* End of wrCfgGetParamVal() */

#ifdef WR_TEST_CODE

/*
*
*       Fun:   wrIBPrintMenu
*
*       Desc:  Print Menu
*
*       Ret:   None.
*
*       Notes: None
*
*       File:  wr_smm_init.c
*
*/
#ifdef ANSI
PRIVATE Void wrIBPrintMenu
(
Void
)
#else
PRIVATE Void wrIBPrintMenu()
#endif
{
   Txt pBuf[512];

   TRC2(sbIBPrintMenu);

   sprintf(pBuf, "\n\t\t *** List of available commands are: ***\n");
   SPrint(pBuf);
   sprintf(pBuf, "1.help       ----- Display help\n");
   SPrint(pBuf);
   sprintf(pBuf, "2.overload   ----- Move ENB into overloaded state\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|Command             | Action|Input1   |Input2  | Description                            |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|snds1cfgupd         | NA    | NA      |NA      | send s1 config update                  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|snds1cfgupdfail     | on    | NA      |NA      | enable sending s1 config update fail   |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable sending s1 config update fail  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|s1partrst           | NA    | NA      |NA      | send s1ap partial reset                |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|s1fullrst           | NA    | NA      |NA      | send s1ap full reset                   |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|sndx2setupfail      | on    | 0       |NA      | enable sending x2 setup fail without   |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | time to wait                           |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | on    | 1       |NA      | enable sending x2 setup fail with time |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | to wait                                |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable sending x2 setup fail          |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|dntrspx2setup       | on    | NA      |NA      | enable discarding x2setup request      |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable discarding x2setup request     |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|sndx2cfgupdfail     | on    | 0       |NA      | enable sending x2 config update fail   |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | without time to wait                   |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | on    | 1       |NA      | enable sending x2 config updatefail    |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | with time to wait                      |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable sending x2 config update fail  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|--------------------------------------  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|dntrspx2cfgupd      | on    | NA      |NA      | enable discarding x2 config update     |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable discarding x2 config update    |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|--------------------------------------  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|dntrspx2rst         | on    | NA      |NA      | enable discarding x2 reset             |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable discarding x2 reset            |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|--------------------------------------  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|sndx2errind         | on    | NA      |NA      | enable sending x2 error indication     |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable sending x2 error indication    |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|--------------------------------------  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|sndx2rst            | NA    | NA      |NA      | send x2 reset request                  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|--------------------------------------  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|sndrabrls           | NA    | NA      |NA      | initiate rab release procedure         |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|--------------------------------------  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|sndenberrind        | on    | NA      |NA      | enable sending enodeB error indication |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable sending enodeB error indication|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|sndnasnondelind     | on    | NA      |NA      | enable sending NAS non delivery        |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | indication                             |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable sending NAS non delivery       |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | indication                             |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|dntrsps1cfgupd      | on    | NA      |NA      | enable discarding s1 config update     |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | message                                |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable discarding s1 config update    |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | message                                |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|sndendcfgupdrx      | on    | NA      |NA      | enable sending config update response  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable sending config update response |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|snds1setuprx        | on    | NA      |NA      | enable sending s1 setup response       |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable sending s1 setup response      |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|drprrcconnsetupcmplt| on    | NA      |NA      | enable discarding rrc connection setup |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | complete                               |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable discarding rrc connection setup|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | complete                               |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|drprrcrelcfm        | on    | NA      |NA      | enable discarding rrc release confirm  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable discarding rrc release confirm |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|drpuecapinfo        | on    | NA      |NA      | enable discarding ue capability info   |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable discarding ue capability info  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|drpseccmplt         | on    | NA      |NA      | enable discarding security mode        |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | complete                               |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable discarding security mode       |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | complete                               |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|drprrcrecfgcmlpt    | on    | NA      |NA      | enable discarding rrc reconfig complete|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable discarding rrc reconfig        |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | complete                               |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|drprrcreestabcmplt  | on    | NA      |NA      | enable discarding rrc restablishment   |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | complete                               |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable discarding rrc restablishment  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | complete                               |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|dbgp                | on    | layer   |1       | send debug enable in the layer with    |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       | name    |        | debug level as 1                       |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | on    | layer   |2       | send debug enable in the layer with    |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       | name    |        | debug level as 2                       |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | on    | layer   |3       | send debug enable in the layer with    |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       | name    |        | debug level as 3                       |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | on    | layer   |4       | send debug enable in the layer with    |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       | name    |        | debug level as 4                       |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | layer   |NA      | send debug disable in the layer        |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       | name    |        |                                        |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|macstats            | on    | NA      |NA      | enable MAC CQI & HARQ ACK/NACK stats   |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable  MAC CQI & HARQ ACK/NACK stats |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
#ifdef PHY_ERROR_LOGING
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|phylogging          | on    | NA      |NA      | enable PHY logging                     |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | NA      |NA      | disable PHY logging                    |\n");
   SPrint(pBuf);
/* LTE_ADV starts */
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|ltea                | on    | abs     |NA      | enable Almost Blanking Subframe (ABS)  |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | Feature                                |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | on    | re      |NA      | enable Range Extension(RE) Feature     |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | on    | sfr     |NA      | enable Soft Frequency Reuse(SFR)       |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | Feature                                |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | on    | dsfr    |NA      | enable Dynamic Soft Frequency Reuse    |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | (DSFR) Feature                         |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | abs     |NA      | disable Almost Blanking Subframe       |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | (ABS) Feature                          |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | re      |NA      | disable Range Extension(RE) Feature    |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | sfr     |NA      | disable Soft Frequency Reuse(SFR)      |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | Feature                                |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    | off   | dsfr    |NA      | disable Dynamic Soft Frequency Reuse   |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|                    |       |         |        | (DSFR) Feature                         |\n");
   SPrint(pBuf);
/* LTE_ADV ends */
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
    /* ccpu00125303:Added debug prints for cpu load indication */
   SPrint(pBuf);
#endif
   sprintf(pBuf, "\t|loadInd             | on    | CPU %%   |NA      | Trigger CPU load indication            |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|ipchange            | on    | w.x.y.z  |NA      | Trigger IP change                      |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);

   /* CNM_DEV start */
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|cnmprnteutranbrinfo | on    | 1       | NA      | Print all EUTRAN neighbor info        |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   /* CNM_DEV end */

   /* CSG_DEV start */
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|csgdlulresrecfg| on    | 1    | mindl/ulres| Modify the minimum dl res info for nonCSG |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|csgcellrecfg| on    | 1     |  csgUes | nonCsgUes| Modify the max Csg and Non Csg  Ues Info |\n");
   SPrint(pBuf);
   sprintf(pBuf, "\t|--------------------|-------|---------|--------|----------------------------------------|\n");
   SPrint(pBuf);
   /* CSG_DEV end */

   RETVOID;

} /* wrIBPrintMenu */


/*
 *
 *       Fun:   wrISStrtoul
 *
 *       Desc:  wrapper for strtoul 
 *
 *       Ret:    ascii string to long value 
 *
 *       Notes: None
 *
 *       File:  
 *        
 */
#ifdef ANSI
PRIVATE U32 wrISStrtoul
(     
 CONSTANT S8 *str
)          
#else
PRIVATE U32 wrISStrtoul(str)
CONSTANT S8 *str;
#endif   
{
      TRC2(wrISStrtoul)
         RETVALUE(strtoul(str, NULLP, NULLP));
                 
} /* end of wrISStrtoul */


/*
 *
 *      Fun:   wrIBParseMenu
 *
 *      Desc:  This function parses the menu option selected
 *
 *      Ret:   RETVOID,
 *
 *      Notes: None.
 *
 *      File:  wr_smm_init.c
 *
 */
#ifdef ANSI
PRIVATE Void wrIBParseMenu
(
TstTkn     *token             /* Command Name */
)
#else
PRIVATE Void wrIBParseMenu(token)
TstTkn     *token;            /* Command Name */
#endif
{
   Txt  pBuf[100];
   S32  cmd;
   S32  arg1 = NULLP;
   S32  arg2;
   S32  arg3;
   S32  arg4;
   U32  memAvail;
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
   U32 memIndx;
#endif

   TRC2(wrIBParseMenu)

   cmd = osMatchKeyword(wrCliMenuKey, (S8 *)token->tok.val);
   if (token->nmbArgs >= 1)
   {
      arg1 = osMatchKeyword(wrCliSecArgKey ,(S8*)token->args[0].val);

      if(! (arg1 == WR_ON || arg1 == WR_OFF))
      {
         sprintf(pBuf, "CLI Error: ** Second Option is invalid **\n");
         SPrint(pBuf);
         RETVOID;
      }     
   }

   switch (cmd)
   {
      case  0: /* help */ 
         {
            wrIBPrintMenu();
         }
         break;

      case  1:  /* enbovld */ 
         {
            if (token->nmbArgs != 2 )
            {
               sprintf(pBuf, "CLI Error: ** For endovld Third Option is \
                     invalid **\n");
               SPrint(pBuf);
               break;
            }
            arg2 =  wrISStrtoul((S8*)token->args[1].val);
            if (arg1 == WR_ON)
            {
               switch(arg2)
               {
                  case 1:
                     {
                        smSndWrCellReCfgSib2(0x08);
                        ALARM("Triggered eNB Overload with CRITICAL\n");
                     }
                     break;
                  case 2:
                     {
                        smSndWrCellReCfgSib2(0x04);
                        ALARM("Triggered eNB Overload with MAJOR\n");
                     }
                     break;
                  case 3:
                     {
                        smSndWrCellReCfgSib2(0x02);
                        ALARM("Triggered eNB Overload with MINOR\n");
                     }
                     break;

               }
            }
            else if(arg1 == WR_OFF)
            {
               smSndWrCellReCfgSib2(0x01);
               ALARM("Triggered eNB Overload STOP\n");
            }
         }
         break;

      case  2:  /* snds1cfgupd */
         {
            /* Modify TAC and cell id */
            ALARM("Trigger S1 & X2 Cfg Upd \n");
            smSndWrModCellCfg();
         }
         break;

      case  3:  /* snds1cfgupdfail */
         {
            if (arg1 == WR_ON)
            {
               ALARM("Enable MME Cfg Upd fail \n");
               wrTgrMmeCfgUpdFail = TRUE;
            }
            else 
            {
               ALARM("Disable MME Cfg Upd fail \n");
               wrTgrMmeCfgUpdFail = FALSE;
            }
         }
         break;

      case  4: /* dbgp */ 
         {      
         }
         break;

      case  5: /* s1partrst */ 
         {
            ALARM("Trigger Partial S1-Reset \n");
            smBuildWrPartRstCntrl();
         }
         break;
      case  6:  /* s1fullrst */
         {
            ALARM("Trigger Full S1-Reset \n");
            smBuildWrFullRstCntrl();
         }
         break; 
      case  7:  /*  sndx2setupfail */
         {
            if (token->nmbArgs != 2 )
            {
               sprintf(pBuf, " Error : **  Argument Missing ** sndx2setupfail\
                     on/off with_timetowait\n");
               SPrint(pBuf);
               break;
            }
            arg2 =  wrISStrtoul((S8*)token->args[1].val); 

            if(arg2 == 1)
            {
               if (arg1 == WR_ON )
               {
                  wrTgrX2SetupFail = TRUE;
               }
               else
               {
                  wrTgrX2SetupFail =  FALSE;
               }
            }
            else
            {
               if (arg1 == WR_ON )
               {
                  wrTgX2SetupFailWoTW = TRUE;
               }
               else 
               {
                  wrTgX2SetupFailWoTW = FALSE;
               }
            }
         }
         break; 
      case  8:  /* dntrspx2setup */
         {
            if (arg1 == WR_ON )
            {
               wrDntRspX2Setup = TRUE;
            }
            else 
            {
               wrDntRspX2Setup = FALSE;
            }     
            ALARM("Enable/Disable X2 Setup retransmission:\
                  wrDntRspX2Setup = %d\n", wrDntRspX2Setup);
         }
         break; 
      case  9:  /* sndx2cfgupdfail */
         {
            if (token->nmbArgs != 2 )
            {
               sprintf(pBuf, " Error : **  Argument Missing ** sndx2cfgupdfail\
                     on/off with_timetowait\n");
               SPrint(pBuf);
               break;
            }
            arg2 =  wrISStrtoul((S8*)token->args[1].val); 

            if(arg2 == 1)
            {
               if (arg1 == WR_ON )
               {
                  wrTgrX2CfgUpdFail = TRUE;
               }
               else 
               {
                  wrTgrX2CfgUpdFail = FALSE;
               }       
            }
            else
            {
               if (arg1 == WR_ON )
               {
                  wrTgrX2CfgUpdFailWoTW = TRUE;
               }
               else 
               {
                  wrTgrX2CfgUpdFailWoTW = FALSE;
               }
            }      
            ALARM("Enable/Disable X2 CfgUpd Fail :\
                  wrTgrX2CfgUpdFail = %d\n", wrTgrX2CfgUpdFail);
         }
         break;
      case 10:  /* dntrspx2cfgupd */
         {
            if (arg1 == WR_ON )
            {
               wrDntRspX2CfgUpd = TRUE;
            }
            else 
            {
               wrDntRspX2CfgUpd = FALSE;
            }      
            ALARM("Enable/Disable X2 CfgUpd Retransmission: \
                  wrDntRspX2CfgUpd = %d\n", wrDntRspX2CfgUpd);
         }
         break;
      case 11: /* dntrspx2rst */ 
         {
            if (arg1 == WR_ON )
            {
               wrDntRspX2Reset = TRUE;
            }
            else 
            {
               wrDntRspX2Reset = FALSE;
            }      

            ALARM("Enable/Disable X2 Reset Retransmission:\
                  wrDntRspX2Reset = %d\n", wrDntRspX2Reset);
         }
         break;
      case 12: /* sndx2errind */
         {
            if (arg1 == WR_ON )
            {
               wrTrgX2ErrInd = TRUE;
            }
            else 
            {
               wrTrgX2ErrInd = FALSE;
            }      
         }
         break;
      case 13: /* sndx2rst */
         {
            ALARM("Tiggering X2-Reset \n");
            /* Trigger X2 Reset To all Neighbor Enb's */
            smBuildNghRstCntrl();
         }
         break;
      case  14: /* memstatus */
         {
#if defined(SS_MULTICORE_SUPPORT) && defined(SS_M_PROTO_REGION)
            for(memIndx = 0; memIndx < 4/* (SS_MAX_REGS)*/; memIndx++)
            {
               SRegInfoShow(memIndx, &memAvail);
               memAvail = 0;
            }
#else
            SRegInfoShow(DFLT_REGION, &memAvail);
//            brcm_print_buffer_status(1); 
#endif  
            /* Statistics print */
            wrUmmEnbStats();
         }
         break;
      case 15: /* sndrabrls*/
         {
            smBuildWrErabRelIndCntrl();
         }
         break;
      case 16: /* sndenberrind */
         {
            if (arg1 == WR_ON )
            {
               wrTgrEnbErrInd = TRUE;
            }
            else 
            {
               wrTgrEnbErrInd = FALSE;
            }      
         }
         break;
      case 17: /* sndnasnondelind */
         {
            if (arg1 == WR_ON )
            {
               wrTrgNasNonDlvrInd = TRUE;
            }
            else 
            {
               wrTrgNasNonDlvrInd = FALSE;
            }      
         }
         break;
      case 18: /* dntrsps1cfgupd */
         {
            if (arg1 == WR_ON )
            {
               wrDntRspMmeCfgUpd = TRUE;
            }
            else 
            {
               wrDntRspMmeCfgUpd = FALSE;
            }      
         }
         break;
      case 19: /* sndendcfgupdrx */
         {
            if (arg1 == WR_ON )
            {
               wrTgrEnbCfgUpdRtx = TRUE;
            }
            else 
            {
               wrTgrEnbCfgUpdRtx = FALSE;
            }      
         }
         break;
      case 20: /* snds1setuprx */
         {
            if (arg1 == WR_ON )
            {
               wrTgrS1SetupRtx = TRUE;
            }
            else 
            {
               wrTgrS1SetupRtx = FALSE;
            }      
         }
         break;
      case 23: /* drprrcconnsetupcmplt */
         {
            if (arg1 == WR_ON )
            {
               wrTrgRrcConTO = TRUE;
            }
            else 
            {
               wrTrgRrcConTO = FALSE;
            }      
         }
         break;
      case 24: /* drprrcrelcfm */
         {
            if (arg1 == WR_ON )
            {
               wrTrgRrcConRelTO = TRUE;
            }
            else 
            {
               wrTrgRrcConRelTO = FALSE;
            }      
         }
         break;
      case 25: /* drpuecapinfo  */
         {
            if (arg1 == WR_ON )
            {
               wrTrgUeCapTO = TRUE;
            }
            else 
            {
               wrTrgUeCapTO = FALSE;
            }      
         }
         break;
      case 26: /* drpseccmplt */
         {
            if (arg1 == WR_ON )
            {
               wrTrgSecTO = TRUE;
            }
            else 
            {
               wrTrgSecTO = FALSE;
            }      
         }
         break;
      case 27: /* drprrcrecfgcmlpt */
         {
            if (arg1 == WR_ON )
            {
               wrTrgRrcReConTO = TRUE;
            }
            else 
            {
               wrTrgRrcReConTO = FALSE;
            }      
         }
         break;
      case 28: /* drprrcreestabcmplt */
         {
            if (arg1 == WR_ON )
            {
               wrTrgRrcRestTO = TRUE;
            }
            else 
            {
               wrTrgRrcRestTO = FALSE;
            }      
         }
         break;
       case 29: /* enbportlog */
         {
         }
         break;
      case 30: /* trgRrcRestTO */
         {
            wrTrgRrcRestTO = (wrTrgRrcRestTO)?FALSE:TRUE;
         }
         break;
      case 31: /* loadInd */
         {
            ALARM("Tiggering Load Indication\n");

            if (token->nmbArgs != 2 )
            {
               sprintf(pBuf, " Error : ** Token Missing **\n");
               SPrint(pBuf);
               break;
            }
            /* Choice, at present only 1 (CPU load) is supported */
            arg1 =  wrISStrtoul((S8*)token->args[0].val);

            /* Value */
            arg2 =  wrISStrtoul((S8*)token->args[1].val);

            /* Modify CPU Load using Enodeb reconfiguration */
#ifdef RM_INTF
            if(smSendCpuLoadToRrm((U8)arg2) != ROK)
            {
               ALARM("Failed to send EnbSta indication to RRM\n");
            }
#endif

            if (smSndLoadInd(WRSM_ENB_CPU_LOAD, arg2) != ROK)
            {
               ALARM("Load indication failed\n");
            }

         }
         break;
      case 32: /* printNrInfo */
         {
            ALARM("Tiggering ANR to print Neighbour cell Information\n");
            smBuildAnrPrintCntrl();
         }
         break;
#ifdef PHY_ERROR_LOGING
      case 33: /* macstats */
         {
            ALARM("Tiggering Mac Stats\n");

            if(arg1 == WR_ON)
            {
               /* send data the updated data to MAC Sch */
               wrSmSendUlAllocCfgReq(PHY_CMD_START_TEST);
            }
            else
            {
               /* send data the updated data to MAC Sch */
               wrSmSendUlAllocCfgReq(PHY_CMD_STOP_TEST);   
            }
         }
         break;

      case 34: /* phylogging */
         {
            ALARM("Tiggering PHY logging\n");

            if(arg1 == WR_ON)
            {
               /* send data the updated data to MAC Sch */
               wrSmSendUlAllocCfgReq(PHY_CMD_ENABLE_LOG);
            }
            else
            {
               /* send data the updated data to MAC Sch */
               wrSmSendUlAllocCfgReq(PHY_CMD_DISABLE_LOG);   
            }
         }
         break;
#endif /* PHY_ERROR_LOGING */
#ifdef E_TM
      case 35: /* etmtest */
         {
            if ( arg1 == WR_ON ) 
            {
               ALARM("Command Enetered: etmtest on \n");
               if ( token->nmbArgs != 2 ) 
               {
                  ALARM("Wrong Usage of the command!!!\n");
                  ALARM("Usage: etmtest on/off < test mode >\n");
                  break;
               }
               arg2 =  wrISStrtoul((S8*)token->args[1].val);
               if ( arg2 < 1 || arg2 > 6 ) 
               {
                  ALARM("Error(Val = %ld): Invalid Test Model. Valid TM: 1 to 6 \n", arg2); 
                  break;
               }
               if ( etmCellState == ETM_CELL_STATE_IDLE )
               {
                  ALARM("Cell Cfg Has not been done!!\n");
                  break; 
               }
               else if ( etmCellState == ETM_CELL_STATE_UP )
               {
                  ALARM("Already test is sarted, try 'etmtest off', before starting new test\n");
                  break;
               }
               /* Send ETM Init */ 
               wrSmDfltEtmId = arg2;
               RLOG1(L_ERROR,"Entered Test Model = %ld", wrSmDfltEtmId);
               wrSmYsEtmCfg();
            }
            else if ( arg1 == WR_OFF ) 
            {
               ALARM("Command Enetered: etmtest off \n");
               if ( etmCellState == ETM_CELL_STATE_IDLE || etmCellState == ETM_CELL_STATE_CFG ) 
               {
                  ALARM("Test has not been started!!\n");
                  break;
               }
               /* Send ETM_STOP request  */
               smBuildYsEtmCntrl(ADISIMM);
            }
         }
         break;
#endif /* E_TM */
     case 36:
             if(WR_ON == arg1)
             {
                 CmInetAddrInfo addrInfo;
                 CmInetAddrInfo *res = NULLP;
                 S16 ret;

                 if(token->nmbArgs!=2)
                 {
                   ALARM("Usage:ipchange on <ip address in format of w.x.y.z>!!\n");
                    break;
                 } 
                 ret = cmInetGetAddrInfo((S8 *)token->args[1].val, NULLP,&addrInfo,&res);
                 if(ret != ROK)
                 {
                      if(res)
                      {
                         cmInetFreeAddrInfo(res);
                      }
                      ALARM("Validation of IP address failed:[%s]",(S8 *)token->args[1].val);
                      break;
                 }

                if (res && res->ai_family == AF_INET)
                {
                   wrUpdateIpAddr(((U8*)token->args[1].val),&(smCfgCb.enbIpAddr[WR_IPV4_IDX]));
                   wrCpyCmTptAddr(&smCfgCb.sctpIpAddr[WR_IPV4_IDX], &smCfgCb.enbIpAddr[WR_IPV4_IDX]);
                }
                else if (res && res->ai_family == AF_INET6)
                {
                   wrUpdateIpAddr(((U8*)token->args[1].val),&(smCfgCb.enbIpAddr[WR_IPV6_IDX]));
                   wrCpyCmTptAddr(&smCfgCb.sctpIpAddr[WR_IPV6_IDX], &smCfgCb.enbIpAddr[WR_IPV6_IDX]);
                }
                else
                {
                      cmInetFreeAddrInfo(res);/*klockwork fix*/
                      ALARM("Unknown error:[%s]",(S8 *)token->args[1].val);
                      break;
                }

                 /* First release all cals,call full reset, and bring down Tx Power */ 
                 /* get confirmation and then call shutdown */
                 /*execv("./ipchange.sh",((S8*)token->args[1].val));*/
                 smCb.reCfg = TRUE;
                 smBuildShutRestart(); 
             } 
             break;        
/* LTE_ADV starts */
      case  37:  /* lteadv */
         {
            /* Modify LTEA */
            arg2 = osMatchKeyword(wrCliLteaArgKey ,(S8*)token->args[1].val);
            
            switch(arg2)
            {            
               case RGR_ABS:
               case RGR_SFR:
               case RGR_DSFR:
               case RGR_RE: 
               {
                  RgrFeatureStatus status = ((arg1 == WR_ON) ?  RGR_ENABLE : RGR_DISABLE);
                  WrEmmlteAdvFeatureActDeact configInfo = {(RgrFeature)arg2, status};         
                  smSndWrLteAdvancedCellReCfg(&configInfo);
                  sprintf(pBuf, "Application Configuration Initiated **\n");
                  SPrint(pBuf);
                  break;
               }
               default:
               {
                  sprintf(pBuf, "CLI Error: ** Second Option is invalid **\n");
                  SPrint(pBuf);
                  RETVOID;
               }
            }
         }
         break;
       case 38: /*startlogging */
        {
#ifdef L2_LOGGING_ENABLED
   ALARM("Starting Dumping Logs in Buffer\n");
   rbMlogStartLogging(0);
#else
   ALARM("Please Enable L2_LOGGING_ENABLED FLAG in make file to dump the logs.\n");
#endif
    }
   break;
       case 39: /*stoplogging */
        {
#ifdef L2_LOGGING_ENABLED
   ALARM("Dumping the Logs in /root/L2_log.txt\n");
   rbMlogRead(0);
#else
   ALARM("Please Enable L2_LOGGING_ENABLED FLAG in make file to dump the logs.\n");
#endif
    }
   break;
#ifdef SS_RBUF
        case 40: /* ringstatus */
        SPrintSRngStats();
        break;
#endif
      /* CSG_DEV start */
      case 41: /* csgprntcellinfo */
      {
         if (token->nmbArgs != 2 )
         {
            sprintf(pBuf, " Error : ** Token Missing [Enter Cell ID]**\n");
            SPrint(pBuf);
            break;
         }
         arg2 =  wrISStrtoul((S8*)token->args[1].val);

         wrUmmCsgPrntCellInfo(arg2);
      }
      break;

      case 42: /* csgprntnbrinfo */
      {
         if (token->nmbArgs != 2 )
         {
            sprintf(pBuf, " Error : ** Token Missing [Enter Cell ID]**\n");
            SPrint(pBuf);
            break;
         }
         arg2 =  wrISStrtoul((S8*)token->args[1].val);

         wrUmmCsgPrntNbrCsgInfo(arg2);
      }
      break;
      /* CSG_DEV end */
      /* CNM_DEV start */
      case 43: /* cnmprnteutranbrinfo */
      {
         if (token->nmbArgs != 2 )
         {
            sprintf(pBuf, " Error : ** Token Missing [Enter Cell ID]**\n");
            SPrint(pBuf);
            break;
         }
         arg2 =  wrISStrtoul((S8*)token->args[1].val);

         wrEmmCnmPrntEutraNbrInfo(arg2);
      }
      break;
      /* CNM_DEV end */
      /* CSG_DEV start */
      case 44: /* csgdlulresrecfg */
      {
         if (token->nmbArgs != 4 )
         {
            sprintf(pBuf, " Error : ** Token Missing [Enter dl res parameter ]**\n");
            SPrint(pBuf);
            break;
         }
         arg2 =  wrISStrtoul((S8*)token->args[1].val);
         arg3 =  wrISStrtoul((S8*)token->args[2].val);
         arg4 =  wrISStrtoul((S8*)token->args[3].val);

         wrEmmSndSchdReCfgReq(arg2, arg3, arg4);
      }
      break;
      case 45: /* csgcellrecfg */
      {
         if (token->nmbArgs != 4)
         {
            sprintf(pBuf, " Error : ** Token Missing [Enter all parameter ]\n"
                  "example csgmaxuerecfg on 1 12 12**\n");
            SPrint(pBuf);
            break;
         }
         arg2 =  wrISStrtoul((S8*)token->args[1].val);
         arg3 =  wrISStrtoul((S8*)token->args[2].val);
         arg4 =  wrISStrtoul((S8*)token->args[3].val);

         smSndCliRmuProtModCellCfg(arg2, arg3, arg4);
      }
      break;
      /* CSG_DEV end */
               default:
         sprintf(pBuf, " Unknown CLI Command : Type 'help' For Menu  \n");
         SPrint(pBuf);
         break;
   }

   RETVOID;
} /* end of sbIBParseMenu */ 



/*
*
*       Fun:   wrIBLineToTkn
*
*       Desc:  Convert Line format to Token Format
*
*       Ret:   Number of arguments on success
*              RFAILED on error
*
*       Notes:
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PRIVATE S16 wrIBLineToTkn
(
S8           *line,
TstTkn       *token
)
#else
PRIVATE S16 wrIBLineToTkn(line, token)
S8           *line;
TstTkn       *token;
#endif
{
   S8 *lPtr;
   S8 word[20];
   S32 i;

   TRC2(wrIBLineToTkn)

   lPtr = osGetWord(line, word);

   if (osStrlen(word) >= sizeof(token->tok.val)) /* make sure there's room */
      RETVALUE(-1);

   osStrcpy((S8*)token->tok.val, word);

   token->tok.pres = TRUE;
   token->tok.len = osStrlen((S8 *)token->tok.val);
   token->nmbArgs = 0;

   for (i = 0; i < MAXARGS; i++)
   {
      lPtr = osGetWord(lPtr, word);

      if (osStrlen(word) >= sizeof(token->tok.val)) /* make sure there's room */
         RETVALUE(-1);

      if (*word == '\0' || *word == '#') /* end of line or comment */
      {
         token->args[i].pres = FALSE;
         break;
      }

      osStrcpy((S8 *)token->args[i].val, word);
      token->args[i].pres = TRUE;
      token->args[i].len = osStrlen((S8*)token->args[i].val);
      token->nmbArgs++;
   }

   RETVALUE(token->nmbArgs);

} /* end of wrIBLineToTkn */   
#endif /* WR_TEST_CODE */

#ifdef WR_CLI_TARGET_BOARD
/* Function to receive CLI command from CLI client */
/*
 *
 *      Fun:    smCliRecvTsk
 *
 *      Desc:   Receive socket event from CLI client
 *
 *      Ret:    ROK - ok
 *
 *      Notes:  None
 *
        File:
 *
 */

#ifdef ANSI
PRIVATE S16 smCliRecvTsk
(
 Pst    *post,               /* post structure */
 Buffer *mBuf                /* message buffer */
 )
#else
PRIVATE S16 smCliRecvTsk (post, mBuf)
   Pst    *post;               /* post structure */
   Buffer *mBuf;               /* message buffer */
#endif
{
   TRC3(smCliRecvTsk);
   U32 addr_len;
   S16 bytes_read;
   U8 recv_data[50];
   U32 idx = 0;
   struct sockaddr_in server_addr;
   struct sockaddr_in6 server_addr6;
   static S32 sock;
   int domain;
   struct sockaddr *server;

   /* Free mBuf */
   WR_FREEMBUF(mBuf);

   if(smCfgCb.enbIpAddr[WR_IPV4_IDX].type == CM_TPTADDR_IPV4)
   {
      server_addr.sin_family = AF_INET;
      server_addr.sin_port = htons(WR_CLI_READ_PORT_NUM);
      server_addr.sin_addr.s_addr = INADDR_ANY;
      bzero(&(server_addr.sin_zero),8);
      domain = AF_INET;
      server = (struct sockaddr *)&server_addr; 
      addr_len = sizeof(struct sockaddr_in);

   }
   else if(smCfgCb.enbIpAddr[WR_IPV6_IDX].type == CM_TPTADDR_IPV6)
   {
      server_addr6.sin6_family = AF_INET6;
      server_addr6.sin6_port = htons(WR_CLI_READ_PORT_NUM);
      server_addr6.sin6_addr = in6addr_any;
      bzero(&(server_addr6.sin6_zero),8);
      domain = AF_INET6;
      server = (struct sockaddr *)&server_addr6; 
      addr_len = sizeof(struct sockaddr_in6);
   }

   if ((sock = socket(domain, SOCK_DGRAM, 0)) == -1)
   {
      RLOG0(L_FATAL, "Error creating socket");
       perror("Socket");
       exit(1);
   }

   if (bind(sock, server, addr_len) == -1)
   {
      perror("Bind");
      exit(1);
   }

   RLOG1(L_INFO, "UDP Server for CLI Commands waiting for client on port %d", WR_CLI_READ_PORT_NUM);

   memset((char*)recv_data, 0, sizeof(recv_data));

   while (1)
   {
      bytes_read = recvfrom(sock,recv_data,50, MSG_WAITALL,
            server, &addr_len);


      if(bytes_read > 0)
      {
         /*ccpu00127802*/
         RLOG1(L_INFO, "Received CLI command=%s,Buf Size=%d", recv_data, strlen(recv_data));
         idx = 0;
         while(1)
         {
           rdConQ(recv_data[idx]);
           if ( recv_data[idx++] == '\0' )
           {
              break;
           }
         }
         memset((char*)recv_data, 0, sizeof(recv_data));
      }
      else
      {
         perror("Received data less than 1");
      }
  }
}
#endif /* WR_CLI_TARGET_BOARD */

#ifdef WR_TEST_CODE
/*
*
*       Fun:   smWrProcCliMsg
*
*       Desc: Used to parse the command line token
*       received from CLI thread. 
*
*       Ret:   Void
*
*       File:  wr_smm_init.c
*
*/

PUBLIC Void smWrProcCliMsg(Void)
{
   wrIBParseMenu(&tkn);
   RETVOID;
}
#endif

/*
*
*       Fun:   rdConQ
*
*       Desc:  
*
*       Ret:   ROK, RFAILED
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC S16 rdConQ
(
Data data
)
#else
PUBLIC S16 rdConQ(data)
Data data;
#endif
{
   Pst       cliPst;

   TRC2(rdConQ)


   if ((data == 0x03) ||
       (data == 27))
   {
      /*ccpu00127802*/
      printf ("wrong key hit ");     
   }
   switch (data)
   {
      case 'q' :
      case 'Q' :
         exit(0);
         break;
#ifdef SS_LIGHT_MEM_LEAK_STS
      case 'z' :
      case 'Z' :
         cmStartStopLeakLog();
         break;
#endif
#ifndef LTE_ENB_PAL
      case 'd':
      case 'D':
        printf("\n************* Printing the LOGS on User Request ***************\n"); 
        /*ssMlogPrint();
        ssMemlogWrite(); */
      break;
#endif
#ifdef WR_TEST_CODE
      default :
         if (wrCmdLen >= MAXCMDLEN - 1)
         {
            wrCmdLen = 0;
            printf ("wrong key hit");      
            RETVALUE(ROK);
         }
         
         wrCmdArgs[wrCmdLen++] = data;

         if ((data == '\n') || (data == '\0'))
         {
            wrIBLineToTkn((S8*)wrCmdArgs, &tkn);

            WR_FILL_PST(cliPst, SM_WR_PROC, ENTSM, ENTWR, EVTWRCLIRECV, WR_POOL, SM_SELECTOR_LC);

            if(wrSmSelfPstMsg(&cliPst) != ROK)
            {
               printf ("Failed to do self post");
               RETVALUE(RFAILED);
            }

//            wrIBParseMenu(&tkn);
            wrCmdLen = 0;
            cmMemset((U8 *)wrCmdArgs, 0, MAXCMDLEN);
         }
#else
         /*ccpu00127802*/
         printf ("****************************************************************");
         printf ("CLI is disabled as 'WR_TEST_CODE' compile time flag is not enbaled");
         printf ("****************************************************************");
#endif
        break;
   }
   RETVALUE(ROK);
}

#ifdef LTE_L2_MEAS
void wrSendL2MeasTrigger()
{
   Pst               pst;          /* post structure  */
   LrgSchMeasReqInfo    measInfo;      /* Meas Req Info */
   static int transId;

   if(ROK == wrDamGetActiveUeCnt(0) || (WR_CELL_STATE_UP != wrEmmCb.cellCb[0]->cellState))
   {
      if(ROK == wrDamGetActiveUeCnt(0))	
      {      
         /*ccpu00127802*/
         RLOG0(L_WARNING, "No Active UE in the System");
      }
      else
      {
         RLOG0(L_WARNING, "Cell is NOT UP");
      }
      /*Send Zero Statistics to PM*/
      glbMacMeasFlag = 0; 
      RETVOID;
   }

   if(glbMacMeasFlag > 0 )
   {
      /*ccpu00127802*/
      RLOG1(L_WARNING, "Already Request in Progress with transId = [%d]", glbMacMeasFlag);
      RETVOID;
   }

   SM_SET_ZERO(&pst, sizeof(Pst));

   /*Fill Pst Structure*/
   pst.selector  = 0; /*Currently hardCoded to 1 to accelerate debugging*/
   pst.srcEnt    = ENTSM; 
   pst.dstEnt    = wrCb.rgrSap[0]->pst.dstEnt;
   pst.dstProcId = wrCb.rgrSap[0]->pst.dstProcId;
   pst.srcProcId = wrCb.rgrSap[0]->pst.srcProcId;
   pst.dstInst = wrCb.rgrSap[0]->pst.dstInst;
   pst.srcInst = wrCb.rgrSap[0]->pst.srcInst;

   pst.event = EVTLRGSCHL2MEASREQ;

   /*Fill response Values.*/
   measInfo.hdr.response.selector = 0;
   measInfo.hdr.response.prior = 0;
   measInfo.hdr.response.route = 0;
   measInfo.hdr.response.mem.region = 0;
   measInfo.hdr.response.mem.pool = 0;  
   measInfo.hdr.transId = transId++;
   glbMacMeasFlag = measInfo.hdr.transId;

   /*ccpu00127802*/
   RLOG1(L_INFO, "Sending Measurement Request with TransID=[%d]", measInfo.hdr.transId);
   /*Fill desired Measurement Info*/
   measInfo.cellId = 1;
   measInfo.measType = LRG_L2MEAS_AVG_PRB_DL
      |LRG_L2MEAS_AVG_PRB_UL
      |LRG_L2MEAS_AVG_PRB_PER_QCI_DL
      |LRG_L2MEAS_AVG_PRB_PER_QCI_UL
      |LRG_L2MEAS_RA_PREAMBLE
      |LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL
      |LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL ;


   measInfo.timePrd = 6000;	/*changed 6000 from 3000 mjkim 2012-08-28*/
#if 0
#else
   measInfo.avgPrbQciUl.numQci=1;
   measInfo.avgPrbQciUl.qci[0]=5;
#endif

#if 0        
#else
   measInfo.avgPrbQciDl.numQci=1;
   measInfo.avgPrbQciDl.qci[0]=5;
#endif

#if 0
#else
   measInfo.nmbActvUeQciUl.sampPrd = 255;
   measInfo.nmbActvUeQciUl.numQci = 1;
   measInfo.nmbActvUeQciUl.qci[0] = 5;
#endif

#if 0
#else
   measInfo.nmbActvUeQciDl.sampPrd = 255;
   measInfo.nmbActvUeQciDl.numQci = 1;
   measInfo.nmbActvUeQciDl.qci[0] = 5;
#endif

   SmMiLrgSchL2MeasStartReq(&pst,&measInfo);

   RETVOID;
}  

/*
*
*       Fun:   wrSendL2MeasStartReq 
*
*       Desc:  This function is used to start L2 Meas 
*              at lower layers (PDCP/RLC/MAC) 
*
*       Ret:   S16 ret
*              
*
*       Notes:
*
*       File:  wr_smm_init.c
*
*/
PUBLIC S16 wrSendL2MeasStartReq(Void)
{
   U16          measType;
   S16         ret = ROK;

   TRC2(wrSendL2MeasStartReq)

   /*bulding and sending L2 Meas req for sending to PDCP layer  */
    measType = LPJ_L2MEAS_UL_LOSS | LPJ_L2CPU_PERCORE_STATS | LPJ_L2MEM_PERPOOL_STATS;
    RLOG1(L_DEBUG, "Sending Start Req to PDCP with measType:%d", measType); 
             /*  LPJ_L2MEAS_DL_DELAY| 
              LPJ_L2MEAS_DL_DISC |
	           LPJ_L2MEAS_DLDP_BIT_RATE |
              LPJ_L2MEAS_ULDP_BIT_RATE|LPJ_L2MEAS_DLCP_BIT_RATE|
              LPJ_L2MEAS_ULCP_BIT_RATE;*/
   /*building and sending L2 meas Req*/
   smSndPjL2MeasStartReq(measType);

   /*Sending L2 Meas Req to RLC layer*/
   measType =  LKW_L2MEAS_DL_DISC ;
   smSndKwL2MeasStartReq(measType);

   measType =  LKW_L2MEAS_UU_LOSS ; 
   smSndKwL2MeasStartReq(measType);

   measType =  LKW_L2MEAS_DL_DELAY; 
   smSndKwL2MeasStartReq(measType);

   /*filling Meas type*/
   measType = LKW_L2MEAS_DL_IP;
   /*building and sending L2 meas Req for sending to RLC Layer*/
   smSndKwL2MeasStartReq(measType);

    /*filling Meas type*/
   measType = LKW_L2MEAS_UL_IP;
   /*building and sending L2 meas Req for sending to RLC Layer*/
   smSndKwL2MeasStartReq(measType);
  
#if 1
   measType = LRG_L2MEAS_AVG_PRB_PER_QCI_DL|
              LRG_L2MEAS_AVG_PRB_DL|
              LRG_L2MEAS_AVG_PRB_UL|
              LRG_L2MEAS_RA_PREAMBLE |
              LRG_L2MEAS_AVG_PRB_PER_QCI_UL |
              LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL |
              LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL |
              LRG_L2MEAS_TB_TRANS_DL_COUNT |
              LRG_L2MEAS_TB_TRANS_UL_COUNT |
              LRG_L2MEAS_TB_TRANS_DL_FAULTY_COUNT |
              LRG_L2MEAS_TB_TRANS_UL_FAULTY_COUNT;
#else
  measType = LRG_L2MEAS_RA_PREAMBLE;
#endif
   /*buidling and sending L2 meas req for sending to Mac Layer*/
   smSndRgSchL2MeasStartReq(measType);
   
   RETVALUE(ret);
}/*wrSendL2MeasStartReq*/
/*
*
*       Fun:   wrSndL2MeasStopReq 
*
*       Desc:  This function is used to Stop L2 Meas 
*              at lower layers (PDCP/RLC/MAC) 
*
*       Ret:   S16 ret
*              
*
*       Notes:
*
*       File:  wr_smm_init.c
*
*/
PUBLIC S16  wrSndL2MeasStopReq(Void)
{
  U16 measType;

  TRC2(wrSndL2MeasStopReq)
 
/* Send L2 Meas Stop req to PDCP layer  */
  measType = LPJ_L2MEAS_UL_LOSS | LPJ_L2CPU_PERCORE_STATS | LPJ_L2MEM_PERPOOL_STATS; 
  RLOG1(L_DEBUG, "Sending Stop Req to PDCP with measType:%d", measType); 
  //measType = LPJ_L2MEAS_UL_LOSS;
       /*     LPJ_L2MEAS_DL_DELAY |
              LPJ_L2MEAS_DL_DISC;*/ 
  smSndPjL2MeasStopReq(measType);
  (kpiInfo.numOfL2MeasStopReq)++;

  measType = LKW_L2MEAS_DL_DISC;
  smSndKwL2MeasStopReq(measType);
  (kpiInfo.numOfL2MeasStopReq)++;

  measType = LKW_L2MEAS_UU_LOSS;
  smSndKwL2MeasStopReq(measType);
  (kpiInfo.numOfL2MeasStopReq)++;

  measType = LKW_L2MEAS_DL_IP;
  smSndKwL2MeasStopReq(measType);
  (kpiInfo.numOfL2MeasStopReq)++;

  measType = LKW_L2MEAS_UL_IP;
  smSndKwL2MeasStopReq(measType);
  (kpiInfo.numOfL2MeasStopReq)++; 

  measType = LKW_L2MEAS_DL_DELAY ;
  smSndKwL2MeasStopReq(measType);
  (kpiInfo.numOfL2MeasStopReq)++;

#if 1
  measType = LRG_L2MEAS_AVG_PRB_PER_QCI_DL|
             LRG_L2MEAS_AVG_PRB_DL|
             LRG_L2MEAS_AVG_PRB_UL|
             LRG_L2MEAS_RA_PREAMBLE|
        LRG_L2MEAS_AVG_PRB_PER_QCI_UL |
             LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL |
             LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL|
             LRG_L2MEAS_TB_TRANS_DL_COUNT |
             LRG_L2MEAS_TB_TRANS_UL_COUNT |
             LRG_L2MEAS_TB_TRANS_DL_FAULTY_COUNT |
             LRG_L2MEAS_TB_TRANS_UL_FAULTY_COUNT;
#else
  measType = LRG_L2MEAS_RA_PREAMBLE;
#endif
  smSndRgSchL2MeasStopReq(measType);
  (kpiInfo.numOfL2MeasStopReq)++;

  RETVALUE(ROK);
}/*wrSndL2MeasStopReq*/
/*
*
*       Fun:   wrSndL2MeasSendReq
*
*       Desc:  This function is used to request L2 Meas
*              to lower layers (PDCP/RLC/MAC)
*
*       Ret:   S16 ret
*
*
*       Notes:
*
*       File:  wr_smm_init.c
*
*/
PUBLIC S16 wrSndL2MeasSendReq(Void)
{
  U16 measType;

  TRC2(wrSndL2MeasSendReq)

/* This part sends L2 meas req to PDCP lyaer */
  measType = LPJ_L2MEAS_UL_LOSS | LPJ_L2CPU_PERCORE_STATS | LPJ_L2MEM_PERPOOL_STATS; 
  RLOG1(L_DEBUG, "Sending Send Req measType:%d", measType); 
  //measType = LPJ_L2MEAS_UL_LOSS;
           /* LPJ_L2MEAS_DL_DELAY | 
             LPJ_L2MEAS_UL_LOSS; */
  
  smSndPjL2MeasSendReq(measType);

  measType = LKW_L2MEAS_DL_DISC;
            /*LKW_L2MEAS_UU_LOSS;*/
  smSndKwL2MeasSendReq(measType);

  measType = LKW_L2MEAS_UU_LOSS;
  smSndKwL2MeasSendReq(measType);

  measType = LKW_L2MEAS_DL_IP;
  smSndKwL2MeasSendReq(measType);

  measType = LKW_L2MEAS_UL_IP;
  smSndKwL2MeasSendReq(measType);

  measType = LKW_L2MEAS_DL_DELAY ;
  smSndKwL2MeasSendReq(measType);

#if 1
  measType = LRG_L2MEAS_AVG_PRB_PER_QCI_DL|
             LRG_L2MEAS_AVG_PRB_DL|
             LRG_L2MEAS_AVG_PRB_UL|
             LRG_L2MEAS_RA_PREAMBLE|
             LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_DL |
             LRG_L2MEAS_NMB_ACTV_UE_PER_QCI_UL |
             LRG_L2MEAS_AVG_PRB_PER_QCI_UL|
             LRG_L2MEAS_TB_TRANS_DL_COUNT |
             LRG_L2MEAS_TB_TRANS_UL_COUNT |
             LRG_L2MEAS_TB_TRANS_DL_FAULTY_COUNT |
             LRG_L2MEAS_TB_TRANS_UL_FAULTY_COUNT;
#else
  measType = LRG_L2MEAS_RA_PREAMBLE; 
#endif
  smSndRgSchL2MeasSendReq(measType);

   RETVALUE(ROK);
}/*wrSndL2MeasSendReq*/
 
#endif/*LTE_L2_MEAS*/ 

/*     
*     Fun:   wrMemCpuUtilMeasStartReq
*
*     This function is used to start memory and CPU utilization 
*              measurement information 
*
*       Ret:   S16 ret
*              
*
*       Notes:
*
*       File:  wr_smm_init_merged.c
*
*/
PUBLIC S16 wrMemCpuUtilMeasStartReq(Void)
{
   S16         ret = ROK;

   TRC2(wrCpuUtilMeasStartReq)

   /* Enable the measurement mask for the CPU and MEMORY utilization */
   smCb.memCpuMeasCb.memUtilizationMask = TRUE;
   smCb.memCpuMeasCb.cpuUtilizationMask = TRUE;
   cmInitTimers(&(smCb.memCpuMeasCb.cpuMemUtilTmr), 1);

   ret = wrStartTmr((PTR)NULLP,WR_TMR_MEM_CPU_UTIL_INFO,CM_MEM_CPU_UITL_INFO_TMR_VAL);

   RETVALUE(ret);
}/*wrSendL2MeasStartReq*/


/*
 *
*       Fun:   wrMemCpuL3MeasStopReq 
*
*       Desc:  This function is used to stop memory and CPU utilization 
*              measurement information 
*
*       Ret:   S16 ret
*              
*
*       Notes:
*
*       File:  wr_smm_init.c
*
*/
PUBLIC S16  wrMemCpuL3MeasStopReq(Void)
{
   S16 ret = ROK;
   TRC2(wrMemCpuL3MeasStopReq)

   /* Disable the measurement mask for the CPU and MEMORY utilization */
   wrStopTmr((PTR)NULLP,WR_TMR_MEM_CPU_UTIL_INFO);
   /* Enable the measurement mask for the CPU and MEMORY utilization */
   smCb.memCpuMeasCb.memUtilizationMask = FALSE;
   smCb.memCpuMeasCb.cpuUtilizationMask = FALSE;
//   cmClearMemUtilizationCounter(&smCb.memCpuMeasCb.memInfo);
//   cmClearCpuUtilizationCounter(&smCb.memCpuMeasCb.cpuInfo);
   RETVALUE(ret);
}/*wrSndL2MeasStopReq*/

/*
*
*       Fun:   smSendKpiMemCpuInfo
*
*       Desc:  This function is used to send cpu and memory  
*              KPI information to OAM
*
*       Ret:   S16 ret
*
*
*       Notes:
*
*       File:  wr_smm_init.c
*
*/
PUBLIC Void smSendKpiMemCpuInfo()
{
   U32  kpiId = INVALID_KPI;
   CmLteMemInfo *mInfo;
#ifdef WR_RSYS_OAM
   U16 ssiMemIdx = 0;
   U16 sharedMemIdx = 0;
   U16 kpiIdAvgCpu = KPI_ID_LTE_L3CPUSTATS_CORE0_AVG;
   U16 kpiIdMaxCpu = KPI_ID_LTE_L3CPUSTATS_CORE0_MAX;
#endif
   U8 idxCore = 0;

   if(smCb.memCpuMeasCb.memUtilizationMask == TRUE)
   {
      U8   idxReg;
      U8   idxPool;
      U8   numReg;
      mInfo = &(smCb.memCpuMeasCb.memCfmInfo);
      numReg = mInfo->numRegions;
      RLOG1(L_DEBUG, "Number of Regions in CPUH :%d", numReg); 
      for(idxReg=0; idxReg< numReg; idxReg++)
      {
         if(mInfo->regInfo[idxReg].regionType == 0)
         {
            if (idxReg == 0)
               kpiId = KPI_ID_LTE_MEMINFO_CPUH_SSI_REG0_POOL0_SIZE;
            else if (idxReg == 1)
               kpiId = KPI_ID_LTE_MEMINFO_CPUH_SSI_REG1_POOL0_SIZE;
            else if (idxReg == 2)
               kpiId = KPI_ID_LTE_MEMINFO_CPUH_SSI_REG2_POOL0_SIZE;
            else if (idxReg == 3)
               kpiId = KPI_ID_LTE_MEMINFO_CPUH_SSI_REG3_POOL0_SIZE;
            for(idxPool=0; idxPool<mInfo->regInfo[idxReg].numPools; idxPool++)
            {
               WR_INC_KPI((kpiId + ssiMemIdx++),KPI_VALUE_TYPE_INTEGER,
                     mInfo->regInfo[idxReg].poolInfo[idxPool].poolSize);
               WR_INC_KPI((kpiId + ssiMemIdx++),KPI_VALUE_TYPE_INTEGER,
                     mInfo->regInfo[idxReg].poolInfo[idxPool].totAvailable);
               WR_INC_KPI((kpiId + ssiMemIdx++),KPI_VALUE_TYPE_INTEGER,
                     mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed);
               WR_INC_KPI((kpiId + ssiMemIdx++),KPI_VALUE_TYPE_INTEGER,
                     mInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed);
            }
         }
         else if(mInfo->regInfo[idxReg].regionType == 1)
         {
            kpiId = KPI_ID_LTE_MEMINFO_CPUH_SHARED_REG_POOL0_SIZE;
            for(idxPool=0; idxPool<mInfo->regInfo[idxReg].numPools; idxPool++)
            {
               WR_INC_KPI((kpiId + sharedMemIdx++),KPI_VALUE_TYPE_INTEGER,
                     mInfo->regInfo[idxReg].poolInfo[idxPool].poolSize);
               WR_INC_KPI((kpiId + sharedMemIdx++),KPI_VALUE_TYPE_INTEGER,
                     mInfo->regInfo[idxReg].poolInfo[idxPool].totAvailable);
               WR_INC_KPI((kpiId + sharedMemIdx++),KPI_VALUE_TYPE_INTEGER,
                     mInfo->regInfo[idxReg].poolInfo[idxPool].crntUsed);
               WR_INC_KPI((kpiId + sharedMemIdx++),KPI_VALUE_TYPE_INTEGER,
                     mInfo->regInfo[idxReg].poolInfo[idxPool].maxUsed);
            }
         }
      }
   }
   if(smCb.memCpuMeasCb.cpuUtilizationMask == TRUE)
   {
      for(idxCore=0; idxCore < smCb.memCpuMeasCb.cpuCfmInfo.numCores; idxCore++)
      {
            WR_INC_KPI((kpiIdAvgCpu + idxCore),KPI_VALUE_TYPE_INTEGER,
                  smCb.memCpuMeasCb.cpuCfmInfo.cpuUtil[idxCore].avgCpuUtil);
            WR_INC_KPI((kpiIdMaxCpu + idxCore),KPI_VALUE_TYPE_INTEGER,
                  smCb.memCpuMeasCb.cpuCfmInfo.cpuUtil[idxCore].maxCpuUtil);
      }
   }

   /*Increments number of confirm recevied from PDCP layer*/
   
   RLOG0(L_DEBUG,"Received Confirmation");
#ifdef LTE_L2_MEAS
   //if(kpiInfo.numOfTrggrL2MeasReq == kpiInfo.numOfRcvdL2MeasCfm)
   if( kpiInfo.measConfigRecv == WR_MEAS_CFG_DONE )
   {
#ifdef WR_RSYS_OAM
      KpisInfoSentNotification();
#endif
   }
#endif

}



/*
*
*       Fun:   wrMemCpuL3MeasSendReq
*
*       Desc:  This function is used to stop memory and CPU utilization 
*              measurement information 
*
*       Ret:   S16 ret
*
*
*       Notes:
*
*       File:  wr_smm_init.c
*
*/
PUBLIC S16  wrMemCpuL3MeasSendReq(Void)
{
   S16 ret = ROK;

   TRC2(wrMemCpuUtilMeasSendReq)

   wrStopTmr((PTR)NULLP,WR_TMR_MEM_CPU_UTIL_INFO);
   if(ret == ROK)
   {

      if(smCb.memCpuMeasCb.memUtilizationMask == TRUE)
      {
         /* Update Memory Info */
         cmFillMemUtilizationMeas(&(smCb.memCpuMeasCb.memCfmInfo),
                 &(smCb.memCpuMeasCb.memInfo));
      }
      if(smCb.memCpuMeasCb.cpuUtilizationMask == TRUE)
      {
         cmFillCpuUtilizationMeas(&(smCb.memCpuMeasCb.cpuCfmInfo),
               &(smCb.memCpuMeasCb.cpuInfo));
      }
        smSendKpiMemCpuInfo();
        cmClearMemUtilizationCounter(&smCb.memCpuMeasCb.memInfo);
        cmClearCpuUtilizationCounter(&smCb.memCpuMeasCb.cpuInfo);
   }

   ret = wrStartTmr((PTR)NULLP,WR_TMR_MEM_CPU_UTIL_INFO,CM_MEM_CPU_UITL_INFO_TMR_VAL);

   RETVALUE(ret);
}/*wrSndL2MeasSendReq*/

/*
*
*       Fun:   wrMemCpuUtilTmrExp
*
*       Desc:  This function is used to call SOC specific API to get  
*              CPU and Memory information 
*
*       Ret:   S16 ret
*
*
*       Notes:
*
*       File:  wr_smm_init.c
*
*/
PUBLIC Void  wrMemCpuUtilTmrExp(Void)
{
   if(smCb.memCpuMeasCb.memUtilizationMask == TRUE)
   {
        cmUpdateSsiMemInfo(&(smCb.memCpuMeasCb.memInfo));
//      UpdateSocMemInfo(CM_L3_MEM_UTIL_AREAIDX, &(smCb.memCpuMeasCb.memInfo));
   }
   if(smCb.memCpuMeasCb.cpuUtilizationMask == TRUE)
   {
       UpdateSocCpuInfo(&(smCb.memCpuMeasCb.cpuInfo),CM_L3_CPU_UTIL);
   }

   wrStartTmr((PTR)NULLP,WR_TMR_MEM_CPU_UTIL_INFO,CM_MEM_CPU_UITL_INFO_TMR_VAL);
   RETVOID;
}

#ifdef WR_RSYS_OAM
/*
 *
 *       Fun:  SCreateSmmAppTst
 *
 *       Desc: Create SmmApp Thread 
*
*       Ret:   ROK, RFAILED
*
*       File:  wr_enb_init.c
*
*/

#ifdef ANSI
PRIVATE void SCreateSmmAppTst()
#else
PRIVATE void SCreateSmmAppTst()
#endif
{
  pthread_t      tskId_smmApp;
  if ((pthread_create(&tskId_smmApp, NULLP, wrSmmAppTstCaller, NULLP)) != 0)
  {
     RLOG0(L_ERROR,"*********\noam pthread_create failed \n**********");
     RETVOID;
  }

}/*End SCreateSmmAppTst*/

/*
*
*       Fun:  wrSmmAppTstCaller
*
*       Desc: Create SmmApp Thread 
*
*       Ret:   ROK, RFAILED
*
*       File:  wr_enb_init.c
*
*/

#ifdef ANSI
PRIVATE void *wrSmmAppTstCaller
(
Void *arg
)
#else
PRIVATE void *wrSmmAppTstCaller(arg)
Void *arg;
#endif

{
   printf("Calling OAM to Init the Cell UP Procedure\n");
   SmAppTst();
   RETVALUE((Void *)NULLP);
}/*End wrSmmAppTstCaller*/
#endif
#ifdef E_TM
/*
*
*       Fun:   smWrEtmProcSm
*
*       Desc:  This function is entry function to the  main State Machine for ETM
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/
#ifdef ANSI
PUBLIC Void smWrEtmProcSm
(
U8 event
)
#else
PUBLIC Void smWrEtmProcSm(event)
U8  event;
#endif /* ANSI */
{
   TRC2(smWrEtmProcSm);

   switch (smCb.smState)
   {
      case WR_SM_STATE_CL_INIT:
         {
            wrSmHdlCLCfgEvent(event);
            break;
         }
      case WR_SM_STATE_CL_CFG_DONE:
         {
            wrSmHdlCellCfgEvent(event);
            break;
         }
      default:
         {
            RLOG0(L_ERROR, "smWrEtmProcSm() Invalid state received");
            break;
         }
   }
} /* smWrEtmProcSm */   
/*
*
*       Fun:   wrSmHdlCellCfgEvent
*
*       Desc:  This function is handles events in MAC Config done state
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

#ifdef ANSI
PUBLIC Void wrSmHdlCellCfgEvent
(
U8 event
)
#else
PUBLIC Void wrSmHdlCellCfgEvent(event)
U8  event;
#endif /* ANSI */
{
   TRC2(wrSmHdlCellCfgEvent);

   if(event == EVTWRCELLCFG )
   {
      /* Start LTE Convergence Layer Configuration */
      wrSmYsCellCfg();
   }
   else
   {
      RLOG1(L_ERROR, "Invalid event [%d] received", event);
   } /* end of else part */

   RETVOID;
} /* end of wrSmHdlCellCfgEvent */

#endif   
/*
*
*       Fun:   smBuildShutdown
*
*       Desc:  
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

PUBLIC Void smBuildShutdown()
{
   Pst wrPst;
   TRC2(smBuildShutdown);
   WR_DBGP(DBGMASK_SM, (WR_PRNT_BUF,"\n Message to initiate \
            shutdown \n"));
   if(smCb.smState == WR_SM_STATE_CELL_UP)
   {
      smCb.smState = WR_SM_STATE_REBOOT_REQUIRED;
      smBuildFullRstctrl();
   }
   else
   {
      smCb.cellState = FALSE; /* We reached here means cell has been brought down successfully*/
      smCb.smState = WR_SM_STATE_DINIT;
      /*smCb.smState = WR_SM_STATE_RRM_SHUTDWN_DONE;*/
      WR_FILL_PST(wrPst, SM_WR_PROC, ENTSM, ENTWR,EVTWRRRMSHUT, WR_POOL, SM_SELECTOR_LWLC);
      if(wrSmSelfPstMsg(&wrPst) != ROK)
      {
      #ifdef DEBUGP
       WR_DBGP(DBGMASK_SM, (SM_PRNT_BUF, " Failure at Self Pst. %d\n", smCb.smState));
      #endif
      }
   }
   RETVOID;
}   


/*
*
*       Fun:   smBuildShutRestart
*
*       Desc:  
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

PUBLIC Void smBuildShutRestart()

{
   TRC2(smBuildShutRestart);
   smCb.smState = WR_SM_STATE_REBOOT_REQUIRED;
   /* Stop the RLOG restriction for limiting the number of LOgs per 10ms
    * All logs here onwards will be logged
    */
   rlStopLogCountLimit();
   smBuildFullRstctrl();
   RLOG0(L_INFO, "Message to initiate shutdown");
   RETVOID;
}   


/*
*
*       Fun:   smBuildFullRstctrl
*
*       Desc:  This function Builds and send control request for full reset
*
*       Ret:   None
*
*       Notes: <NONE>
*
*       File:  wr_smm_init.c
*
*/

PRIVATE Void smBuildFullRstctrl(Void)
{
   LwrMngmt *cntrl = NULLP;
   Pst      pst;

    TRC2(smBuildFullRstctrl);
   /* wr002.102: Corrected function name in trace macro */

   WR_ALLOC(&cntrl, sizeof(LwrMngmt));

   if(cntrl == NULLP)
   {
    printf("Calling smBuildFullRstctrl 2\n");
      RETVOID;
   }

   SM_SET_ZERO(cntrl, sizeof(LwrMngmt));
   SM_SET_ZERO(&pst, sizeof(Pst));

  /* cntrl.t.cntrl.u.sapId       =  0; */ /* Needed ? */

   /* Fill Header */
   cntrl->hdr.msgType             = TCNTRL;
   cntrl->hdr.entId.ent           = ENTWR;
   cntrl->hdr.entId.inst          = SM_INST_ZERO;
   cntrl->hdr.elmId.elmnt         = STWRMMERSETCTRL;
   cntrl->hdr.response.selector   = SM_SELECTOR_LWLC;

   /* Fill Pst */
   pst.selector  = SM_SELECTOR_LWLC;
   pst.srcEnt    = ENTSM;
   pst.dstEnt    = ENTWR;
   pst.dstProcId = SM_WR_PROC;
   pst.srcProcId = SM_SM_PROC;
   pst.event     = EVTLWRCNTRLREQ;
   RLOG0(L_DEBUG, "Cntrl Req to wr sent for STWRMMERSETCTRL ");
   /* Send the request to the APP */
   SmMiLwrCntrlReq(&pst, cntrl);
   RETVOID;
   
} 

#ifdef LTE_PAL_ENB
/*
*
*       Fun:   Initialize CL task.
*
*       Desc:  Invoked to create LTE CL TAPA task.
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  ve_se_tst.c
*
*/
#ifdef ANSI
PUBLIC S16 smYsInit
(
SSTskId    sysTskId
)
#else
PUBLIC S16 smYsInit(sysTskId)
SSTskId    sysTskId;
#endif
{

   TRC2(smYsInit); 

   /* Register CL TAPA Task */
   if (SRegTTsk((Ent)ENTYS, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            ysActvInit, (ActvTsk)ysActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

#ifndef CNE_UE_SIM_TEST
   if (SRegTTsk((Ent)ENTTF, (Inst)0, (Ttype)TTNORM, (Prior)PRIOR0,
            NULLP, (ActvTsk)ysActvTsk) != ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   /* Attach PHY STUB TAPA Task */
   if (SAttachTTsk((Ent)ENTTF, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */
#endif

   /* Attach CL TAPA Task */
   if (SAttachTTsk((Ent)ENTYS, (Inst)0, sysTskId)!= ROK) 
   {
      RETVALUE(RFAILED);
   } /* end of if statement */

   RLOG1(L_INFO, "CL Tapa Task successfully registered and attached to %d",sysTskId);

   RETVALUE(ROK);
} /* end of smYsInit */

#endif


 
/********************************************************************30**

           End of file:     fc_smm_init_brcm.c@@/main/TeNB_Main_BR/7 - Mon Aug 11 16:52:13 2014

*********************************************************************31*/


/********************************************************************40**
  
        Notes:
  
*********************************************************************41*/

/********************************************************************50**

*********************************************************************51*/

   
/********************************************************************60**
  
        Revision history:
  
*********************************************************************61*/
  
/********************************************************************90**
 
     ver       pat    init                  description
------------ -------- ---- ----------------------------------------------
SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
