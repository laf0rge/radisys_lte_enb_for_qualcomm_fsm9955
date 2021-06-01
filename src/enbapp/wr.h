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

     Type:     C include file

     Desc:     This file contains 
               

     File:     wr.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_H__
#define __WR_H__

#ifdef __cplusplus
extern "C" {
#endif

#if TENB_MAX_UE_SUPPORTED > 96
#error "Invalid value of TENB_MAX_UE_SUPPORTED"
#endif

/* header include files (.h) */
#include "envopt.h"        /* Environment options */
#include "envdep.h"        /* Environment dependent */
#include "envind.h"        /* Environment independent */

#include "gen.h"           /* General */
#include "ssi.h"           /* System services */
#include "cm5.h"           /* Common timer defines */
#include "cm_tkns.h"       /* Common tokens defines */
#include "cm_mblk.h"       /* Common memory allocation library defines */
#include "cm_llist.h"      /* Common link list  defines  */
#include "cm_hash.h"       /* Common hashlist  defines */
#include "cm_lte.h"        /* Common LTE defines */
#include "cm_inet.h"
#include "cm_tpt.h"
#include "szt_asn.h"       /* S1AP RRM control Interface */
#include "nhu_3gasn.h" /* 3G RRC ASN control Interface */

/* header/extern include files (.x) */
#include "gen.x"           /* General */
#include "ssi.x"           /* System services */

#include "cm5.x"           /* Common timer library */
#include "cm_tkns.x"       /* Common tokens */
#include "cm_mblk.x"       /* Common memory allocation */
#include "cm_llist.x"      /* Common link list */
#include "cm_hash.x"       /* Common hashlist */
#include "cm_lte.x"       /*  Common UMTS includes */
#include "cm_lib.x"
#include "cm_inet.x"
#include "cm_tpt.x"

/* header include files related to lower layer interfaces */
#include "nhu.h"           /* NHU interface defines */
#include "pju.h"           /* PDCP data interface defines */
#ifdef RM_INTF
#include "rmu.h"           /* RRM interface defines */
#endif /*RM_INTF*/
#include "nlu.h"           /* SON interface defines */
#include "cpj.h"           /* PDCP conrol interface defines */
#include "rgr.h"           /* MAC RRM control Interface */
#include "ctf.h"           /* PHY RRM control Interface */
#include "szt.h"           /* S1AP RRM control Interface */
#include "sct.h"           /* S1AP RRM control Interface */

#include "czt.h"           /* X2AP Upper Interface */
#include "sct.h"           /* S1AP RRM control Interface */
#include "lrg.h"           /* MAC RRM control Interface */
#include "egt.h"           /* DATA app, GTP interface   */
#ifdef SZTV3
#include "szt_3gasn.h"
#endif

#include "nhu.x"           /* NHU Interface includes */
#include "pju.x"           /* PJU Interface includes */
#include "cm_lte.x"
#include "cm_lte_rrm.x"
#include "rmu.x"           /* RRM Interface includes */
#include "nlu.x"           /* SON Interface include */
#include "cpj.x"           /* CPJ Interface includes */
#include "rgr.x"           /* MAC RRM control Interface */
#include "ctf.x"           /* PHY RRM control Interface */
#include "szt_asn.x"       /* S1AP RRM control Interface */
#ifndef SZ_ENC_DEC
#include "szt_pk.x"
#include "szt_unpk.x"
#endif /* SZ_ENC_DEC */
#ifdef SZTV3
#include "szt_3gasn.x"
#endif
#include "szt.x"           /* S1AP RRM control Interface */
#include "nhu_asn.x"       /* RRC ASN Interface */

#include "nhu_3gasn.x"       /* 3G RRC ASN Interface */

#include "czt.x"           /* X2AP Upper Interface */
#include "sct.x"           /* X2AP RRM control Interface */
#include "lrg.x"           /* MAC RRM control Interface */
#include "egt.x"           /* DATA app, GTP interface   */
#include "wr_lwr.h"
#include "ss_diag.h"

//#include "rm_wr.h"         /* RRM-FSM interface */
#if (defined(WR_DBG_CIRLOG) || defined(SS_SEGV_SIG_HDLR))
#include "wr_dbg_log.h"
#endif /* WR_DBG_LOGS */
#ifdef RLOG_REDIRECT 
#define WR_REDIRECT_RLOGS 1
#endif /* WR_DBG_LOGS */

#include "rl_interface.h"

#ifdef RSYS_WIRESHARK
#include <arpa/inet.h>
#endif

/* macros(#defines) */
#define WR_SRC_PROC_ID                 1
#define WR_DST_PROC_ID                 0

#define WR_NUM_TQENTRY                 100
#define WR_MAX_SAPS                    10
#define WR_MEM_REGION                  0
#define WR_MEM_POOL                    0


#define WR_RLC_UM_DFLT_SN_LEN          10

#define WRLAYERNAME                    "EnodeB APP"

#define WR_CALC_WAIT_TIME(_delay) (_delay/((1000/SS_TICKS_SEC) * wrCb.timeRes))

#define WR_NUM_PJU_SAPS  2
#define WR_NUM_EGT_SAPS  1
#define WR_NUM_CZT_SAPS  1
#define WR_NUM_SZT_SAPS  1
#define WR_NUM_CTF_SAPS  1
#define WR_NUM_RGR_SAPS  1
#define WR_NUM_NHU_SAPS  1



typedef enum wrTmr 
{
   WR_TMR_NHU_SAP_BND=1,
   WR_TMR_RGR_SAP_BND,
   WR_TMR_CTF_SAP_BND,
   WR_TMR_SZT_SAP_BND,
   WR_TMR_CZT_SAP_BND,
   WR_TMR_EGT_SAP_BND,
   WR_TMR_PJU_SAP_BND,
   WR_TMR_INACTIVITY, 
   WR_TMR_S1_RELOC_TMR,
   WR_TMR_S1_OVRL_TMR,
   WR_TMR_UE_HO_ATTACH_TMR,
   WR_TMR_MME_SETUP_RSP,
   WR_TMR_MME_SETUP_WAIT,
   WR_TMR_REESTABLISH,
   WR_TMR_RECONFIGURE,
   WR_TMR_ENB_SETUP_RSP,
   WR_TMR_ENB_SETUP_WAIT,
   WR_TMR_ENB_CFG_RSP,
   WR_TMR_ENB_CFG_WAIT,
   WR_TMR_OVERLD_SAMPLE,
   WR_TMR_OVERLD_STATUS,
#ifdef DG
   WR_TMR_ANR_INTRA_LTE,
   WR_TMR_ANR_INTER_RAT,
   WR_TMR_ANR_DFLT_PERIOD,
   WR_TMR_ANR_1XRTT_DFLT_PERIOD,
   WR_TMR_ANR_HRPD_DFLT_PERIOD,
#endif
   WR_TMR_X2_RST_RSP,
   WR_TMR_ENB_CONFIG_UPDATE_WAIT,
   WR_TMR_RST_RSP,
   WR_TMR_S1_ENB_CFG_RSP,
   WR_TMR_S1_ENB_CFG_WAIT,
   WR_TMR_RRC_RRCCON,
   WR_TMR_RRC_CONREL,
   WR_TMR_RRC_CAPENQ,
   WR_TMR_RRC_SECUCMD,
   WR_TMR_CSFB_CFG,
   WR_TMR_ECSFB_MEAS_RPT,
   WR_TMR_ECSFB_DATCFM,
   WR_TMR_TGTENB_RELOCEXE,

   WR_TMR_RAD_RES_MOD_TMR,
   WR_TMR_INITIAL_CTX_REQ,
   WR_TMR_END_MARKER_TMR ,
   WR_TMR_DETECT_RLF,
   WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG,
   WR_TMR_UE_CTX_REL_REQ,
   WR_TMR_ENB_CFG_TRANSFER_WAIT,

   WR_TMR_ANR_MEAS_EPOCH_TMR,
   WR_TMR_ANR_MEAS_TRICE_TMR,
   WR_TMR_ANR_CELL_MAX_AGE_TMR,
   WR_TMR_TGT_HOPREP,
   /* ecsfb - Added as part of ecsfb phase */
   WR_TMR_RRC_DAT_CFM,
   WR_TMR_TGT_PATH_SWITCH,
   WR_TMR_RRC_REL_GUARD,
   /* PH04_CMAS */
#ifdef WR_RSYS_KPI
   WR_TMR_KPI_PREDFND_INTRVL_TMR,/*Added for KPI*/
#endif /*WR_RSYS_KPI*/
#ifdef RM_INTF
   WR_TMR_RMU_SAP_BND,/*RRM->FSM sck changes*/
#endif /*RM_INTF*/
   /* PH04_CMAS */
   WR_TMR_REPETITION_PERIOD,
   /* ccpu00126817 */
   WR_TMR_TGT_SNSTATUS_TMR,
   WR_TMR_ECSFB_SIB8_TMR,
   WR_TMR_ABS_LOAD_INFO_TMR,
   /*SON interface bind timer*/
   WR_TMR_NLU_SAP_BND,
   /* Added 3 timers for eCSFB */
   WR_TMR_UL_HO_PREP_WAIT_TMR,
   WR_TMR_DL_CDMA_RECV_HO_WAIT_TMR,
   WR_TMR_ECSFB_HO_EXEC_TMR,
   WR_TMR_SRC_HO_CANCEL,
   /* RIM timers */
   WR_TMR_RIM_OVERALL,
   WR_TMR_RIM_TRIR,
   WR_TMR_RIM_TRIAE,
   WR_TMR_CSFB_GSM_T304_EXP_WAIT,
   WR_TMR_CSFB_GSM_MEAS_WAIT,
   WR_TMR_PCI_MOD_TMR,
   WR_TMR_ANR_STRONGCELL_REPORT_R1,
#ifdef WR_RSYS_KPI
   WR_TMR_MEM_CPU_UTIL_INFO,
#endif /* WR_RSYS_KPI*/
   WR_TMR_GUARD_UE_CTX_REL
} enWrTimer;

/* PH04_CMAS */
#ifdef WR_RSYS_KPI
#define WR_TMR_KPI_PREDFND_INTRVL_TMR_VAL 600 
#endif /*WR_RSYS_KPI*/

#define WR_TMR_RRCCON_VAL              1000
#ifdef WR_ACC
#define WR_TMR_RRC_RECONFIG_VAL        10000
#define WR_TMR_ANR_STRONGCELL_RPRT_R1_DELTA 1150 
#else /* WR_ACC */
#define WR_TMR_RRC_RECONFIG_VAL        1000
#define WR_TMR_ANR_STRONGCELL_RPRT_R1_DELTA 30 /* 15ms of procedure delay + 15ms of processing delay*/
#endif /* WR_ACC */
#define WR_TMR_CONREL_VAL              1000
#define WR_TMR_CAPENQ_VAL               1000
#define WR_TMR_SECUCMD_VAL             10000
#define WR_TMR_REESTABLISH_VAL         1000
#define WR_TMR_RAD_RES_MOD_TMR_VAL     600000
#define WR_TMR_INITIAL_CTX_REQ_TMR_VAL 10000
#define WR_TMR_T304_GERAN_DELAY   100/*70ms of processing delay + 30 ms of procedure delay*/
#define WR_TMR_GERAN_MEAS_RPT_WAIT_DELAY 30/*PROCEDURE DELAY*/
/*Increased guard timer to 2 secs*/
#define WR_TMR_RRC_REL_GUARD_VAL       2000 

#define WR_TMR_ECSFB_CFG_VAL            3000
#define WR_TMR_S1_RELOC_VAL            20000
#define WR_TMR_CSFB_CFG_VAL            3000
#define WR_TMR_TGT_HOPREP_VAL          1000
#define WR_TMR_DETECT_RLF_VAL        5000
#define WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG_VAL    500
#define WR_TMR_ECSFB_DATCFM_CFG_VAL    1000
   /*ccpu00129789*/
#define WR_SCHD_DL_CQI_RPT             5

/* Fix for ccpu00126648.Increased the UE_CTX_REL
 * and the PATH_SWITCH_VAL values to 10 and 5 sec respectively */
#define WR_TMR_UE_CTX_REL_VAL          10000
#define WR_TMR_TGT_PATH_SWITCH_VAL     5000
/* ccpu00126817 */
#define WR_TMR_TGT_SNSTATUS_TMR_VAL    5000
/* LTE_ADV_ABS starts */
#define WR_TMR_ABS_LOAD_INFO_TMR_VAL   5000
/* LTE_ADV_ABS ends */

#define WR_MAX_TMR_PER_UE              1
#define WR_APP_TQSIZE                  64
/* Currently we have macro for Dat Cfm timer value, later it will be derived from
 * RLC Max ReTx value */
#define WR_TMR_RRC_DAT_CFM_VAL         2000
#define WR_TMR_UL_HO_PREP_WAIT_TMR_VAL       1000
#define WR_TMR_DL_CDMA_RECV_HO_WAIT_TMR_VAL  4000
#define WR_TMR_ECSFB_HO_EXEC_TMR_VAL         4000
#define WR_TMR_GUARD_UE_CTX_REL_TMR_VAL      8000 
#define WR_CPU_MON_PERIOD                    500 /* 5 sec */ 

#define WR_DFLT_EGTP_PORT              2152

#define WR_INIT_STATE                  0

#define WR_SI_CNT                      4

#define WR_MAX_TARGET_PREP             3
#define WR_MAX_X2AP_CON                10

#define WR_BYTE_LEN                    8
#define WR_TRAC_LEN                    16

#define WR_MEM_PDU_SIZE                4084

#define WR_MAX_SRBS                    2

#define WR_RGR_SRB_CFG_LCH             0x1
#define WR_RGR_SRB_CFG_LCG             0x2
#define WR_RGR_DRB_CFG_LCH             0x4
#define WR_RGR_DRB_CFG_LCG             0x8

#define WR_S1_SETUP_DONE               1
#define WR_S1_CONNECTING               2
#define WR_S1_CONNECTED                3
#define WR_MAX_S1AP_CON                100

/* Encode Request TransIds */
#define WR_ENC_TRANSID_HO_REQ          11
#define WR_ENC_TRANSID_HO_CMD          12
#define WR_DEC_TRANSID_HO_CMD          13
#define WR_ENC_TRANSID_HO_DL_DCCH_MSG  14

/*#define SZ_MAX_MSG_ID                  63*/
#define SZ_MEM_SDU_SIZE                2048
#define WR_CZ_MAX_MSG_ID               25
#define WR_SZ_MEM_SDU_SIZE             2048
#define WR_CZ_MEM_SDU_SIZE             2048

#ifndef WR_DBG_CIRLOG
#define WR_PRNT_BUF                    wrCb.init.prntBuf
#else
/* Currently passing index as zero, when multiple system tasks are used then 
 * retrieve the corresponding circular buffer for that system task.
 */
#define WR_PRNT_BUF                    wrCb.init.prntCirBuf, MAX_LOG_BUF_SIZE 
#endif /* WR_DBG_CIRLOG */
#define DBGMASK_SM                     1
#define WR_EVEBASE                     0
#define WR_ERRVE                       (WR_EVEBASE + 0)  /* reserved */
#define WR_EVE001                      (WR_ERRVE + 1) /* ve_sm_hi_exms.c: 241 */

#define WR_PRSNT      1
#define WR_NOTPRSNT   0

/*129311 : Scheduler default k value for Wideband Cqi report should be non zero*/
#define WR_DFLT_K_VAL_FOR_SCH 1

/* ccpu00133012:The UL Carrier Frequency is derived for DL Carrier Frequency. The
   difference is sepc defined gievn in 36.101. But this specific requirement is to 
   cater the KT requirement.
*/

#define WR_NO_DFLT_UL_CARRIER_FREQ 0

/** @brief Enum holds the value of the intermediate States for cell reset
*/
typedef enum WrCellResetStates
{
   WR_INIT_STATE_NONE = 0,
   WR_S1_RESET_DONE   = 1,
   WR_UE_RELEASE_DONE = 2,
   WR_X2_RESET_DONE   = 4,
   WR_ALL_RESET_DONE  = 7
}WrCellResetStates;

#define   WR_S1_SETUP_DONE_PAR_RESET    8
#define   WR_PAR_RESET_DONE   10 /* Logic is both UE Release and S1 Setup should be done
                                    WR_UE_RELEASE_DONE | 
                                    WR_S1_SETUP_DONE_PAR_RESET = WR_PAR_RESET_DONE */
#define   WR_X2_SETUP_DONE    16
/*************************************************************************
* Debug print                                                            *
*************************************************************************/
#define WR_ERROR 1

#ifdef WR_DBG_CIRLOG 
#define WR_DBGP(_msgClass, _arg) \
{\
   DBGP_CIRLOG(&(wrCb.init), WRLAYERNAME": ", _msgClass, _arg); \
}
#else /* WR_DBG_CIRLOG */

#ifdef DEBUGP
#if (WR_SM_LOG_TO_FILE && DEBUGP)
#define WR_DBGP(_msgClass, _args)                               \
   do                                                             \
   {                                                              \
      if(smCb.init.dbgMask)                                       \
      {                                                           \
         if((smCb.dbgFp == NULLP) ||                              \
               (smCb.nmbDbgLines == smCb.cfgDbgLines))            \
         {                                                        \
            wrSmCreateDbgFile();                                  \
         }                                                        \
         sprintf(SM_PRNT_BUF, "[%s 0x%x:%x] %s:%d ",              \
               SMLAYERNAME,smCb.init.ent, smCb.init.inst,         \
               __FILE__, __LINE__);                               \
         fwrite((SM_PRNT_BUF),cmStrlen((U8*)SM_PRNT_BUF), 1,      \
               (smCb.dbgFp));                                     \
         sprintf _args;                                           \
         fwrite((SM_PRNT_BUF),                                    \
               cmStrlen((U8*)SM_PRNT_BUF), 1,                     \
               (smCb.dbgFp));                                     \
         smCb.nmbDbgLines++;                                      \
      }                                                           \
   }                                                              \
   while(0)

#else /* (WR_SM_LOG_TO_FILE && DEBUGP) */
#define WR_DBGP(_msgClass, _arg) \
   {\
      DBGP(&wrCb.init, WRLAYERNAME": ", _msgClass, _arg); \
   }
#endif /* (WR_SM_LOG_TO_FILE && DEBUGP) */
#else
#define WR_DBGP(_msgClass, _arg) 
#endif
#endif /* WR_DBG_CIRLOG */

/** @brief This structure contains information about the Application's lower
 * SAP.
 - Pst        pst          Post structure
 - SuId       suId         Service user ID
 - SpId       spId         Service provider ID
 - State      sapState     SAP State (Bound/Unbound)
 - Mem        mem          Region/Pool
 - CmTimer    timer        Timer for Bind
 - U8         bndRetryCnt  Number of Bind Retrys
 - U8         maxBndRetry  Max number of Bind Retrys
 - TmrCfg     bndTmr       Bind Timer configuration
 */
typedef struct wrLiSapCb
{
   Pst                       pst;
   SuId                      suId;
   SpId                      spId;
   State                     sapState;
   Mem                       mem;
   CmTimer                   timer;
   U8                        bndRetryCnt;
   U8                        maxBndRetry;
   TmrCfg                    bndTmr;    
} WrLiSapCb;

/** @brief This structure is the Control block for the application intialized at
 * the time of wrActvInit.
 - Mem         mem                  Memory Region and Pool information
 - CmTqCp      tqCp                 Timing Queue Control point
 - CmTqType    tq[WR_NUM_TQENTRY]   Timing Queue for Application
 - TskInit     init                 Init structure
 - Header      lmHdr                Layer Manager Header
 - UConnId     s1apConId            Last used S1AP connection id
 - WrLiSapCb   **nhuSap             RRC SAP
 - WrLiSapCb   **ctfSap             CTF SAP
 - WrLiSapCb   **rgrSap             RGR SAP
 - WrLiSapCb   **sztSap             SZT SAP
 - WrLiSapCb   **cztSap             CZT SAP
 - WrLiSapCb   **egtSap             EGT SAP
 - WrLiSapCb   **pjuSap             PJU SAP
 - WrLiSapCb   **rmuSap             RMU SAP
 - WrLiSapCb   **nluSap             NLU SAP
 */
typedef struct wrCb
{
   Mem                       mem; /* memory pool info */
   CmTqCp                    tqCp;           /* Timing Queue Control point    */
   CmTqType                  tq[WR_NUM_TQENTRY]; /* Timing Queue for a resln. */
   TskInit                   init;
   Header                    lmHdr;
   UConnId                   s1apConId; /* Last used connection id; counter */
   Pst                      wrDamPst;
   WrLiSapCb                 **nhuSap;
   WrLiSapCb                 **ctfSap;
   WrLiSapCb                 **rgrSap;
   WrLiSapCb                 **sztSap;
   WrLiSapCb                 **cztSap;
#ifdef RM_INTF
   WrLiSapCb                 **rmuSap; /*RRM->FSM sck changes*/
#endif /*RM_INTF*/
   WrLiSapCb                 **nluSap; /* SON SAP*/
   WrCellResetStates         resetCmfState;
   WrCellResetStates         parResetCmfState;
#if (VE_SM_LOG_TO_FILE && DEBUGP)
   FILE*                    dbgFp;  /*!< Debug file pointer */
   Txt                      filePath[LVE_MAX_FILE_PATH]; /*!< store log files */
   U32                      nmbDbgLines; /*!< Number of lines per debug file */
   U32                      cfgDbgLines; /*!< Cfgd number of lines/Dbg file */
#endif
   Ticks                    timeRes;       /* Time resolution */
   Bool                     enblSIAndPagngLog; /* To enable SI and paging wireshark logging*/
} WrCb;

typedef struct wrUmmTimer
{
   void *cb;
   CmLList tmrLnk;
   CmTimer tmr;
}WrUmmTimer;

/** @brief This structre is control block for ipv4 and ipv6 type socket addess */
typedef struct wrSockAddr
{
  U16 len; /*!< Length of Ipv4 or Ipv6 socket address */
  U8 type; /*!< Ipv4 or Ipv6 socket type */
  union 
  {
     CmInet4SockAddr addr;  /*!< IPv4 socket address */
     CmInet6SockAddr addr6; /*!< Ipv6 socket address */
  }u;
} WrSockAddr;


/** @breif Enum defines number of antenna ports configured **/
typedef enum smLteAntennaPortsCount
{
   SM_LWR_AP_CNT_1,
   SM_LWR_AP_CNT_2,
   SM_LWR_AP_CNT_4
}SmLteAntennaPortsCount;

EXTERN WrCb                 wrCb;
/*ccpu00129785*/
EXTERN Bool                 memAlarm;

EXTERN S16  wrActvTmr (Ent ent, Inst inst);
EXTERN S16  wrRegInitTmr(Void);
EXTERN S16  wrStartTmr(PTR cb, S16 tmrEvnt, U32 delay);
EXTERN Void wrStopTmr(PTR cb, S16 event);
EXTERN Bool wrIsTmrRunning(CmTimer *tmr, S16 event);

EXTERN Void wrSendLmAlarm(U16 category, U16 event, U16 cause);
EXTERN Void wrShutdown(Void);
EXTERN S16 wrGenerateCfm (Pst *pst, CmStatus  *cfmSts, U8 prim, LwrMngmt *cfm);

EXTERN Void wrPlatNonrtTickHdl(Void);
EXTERN Void PrintRLCStats(Void);
EXTERN Void wrPlatRgrTtiHndl(U32 tticnt);
EXTERN Void ResetRLCStats(Void);
EXTERN Void ResetPDCPStats(Void);
EXTERN Void PrintPDCPStats(Void);

EXTERN const char * wrGetSwVersion(Void);
EXTERN const char * wrGetPfVersion(Void);

#ifdef RSYS_WIRESHARK
EXTERN S16 wrInitWireShark(Void); 
EXTERN S16 wrHandleWireSharkMsg(Buffer *mBuf);
#endif

#ifdef SS_DIAG
EXTERN S16 wrDiagPrntIpAddr(U32 ipAddr, U32 diagLvl);
/* Note: Any changes to these enums should reflect to */
/** @details Enums for special argument
 *
*/
typedef enum
{
   WR_DIAG_CELLID,
   WR_DIAG_CRNTI,
   WR_DIAG_MMEID,
   WR_DIAG_UEID,
   WR_DIAG_PEERID,
   WR_DIAG_ENBID,
   WR_DIAG_NA
} WrDiagSplArg;


/** 
 * @details Macro definition for WR level 0 logs
*/
#define WR_DIAG_LVL0(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)                                                         \
{                                                                                                                                               \
   if(wrCb.init.logMask & SS_DIAG_LVL0)                                                                                                         \
   {                                                                                                                                            \
      ssDiagFix(_tknId, _splArgEnum, ENTWR, wrCb.init.inst, SS_DIAG_LVL0, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                                                                                                            \
}

/** 
 * @details Macro definition for WR level 1 logs
*/
#define WR_DIAG_LVL1(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)                                                         \
{                                                                                                                                               \
   if(wrCb.init.logMask & SS_DIAG_LVL1)                                                                                                         \
   {                                                                                                                                            \
      ssDiagFix(_tknId, _splArgEnum, ENTWR, wrCb.init.inst, SS_DIAG_LVL1, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                                                                                                            \
}

/** 
 * @details Macro definition for WR level 2 logs
*/
#define WR_DIAG_LVL2(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)                                                         \
{                                                                                                                                               \
   if(wrCb.init.logMask & SS_DIAG_LVL2)                                                                                                         \
   {                                                                                                                                            \
      ssDiagFix(_tknId, _splArgEnum, ENTWR, wrCb.init.inst, SS_DIAG_LVL2, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                                                                                                            \
}

/** 
 * @details Macro definition for WR  level 3 logs 
*/
#define WR_DIAG_LVL3(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)                                                         \
{                                                                                                                                               \
   if(wrCb.init.logMask & SS_DIAG_LVL3)                                                                                                         \
   {                                                                                                                                            \
      ssDiagFix(_tknId, _splArgEnum, ENTWR, wrCb.init.inst, SS_DIAG_LVL3, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                                                                                                            \
}

/** 
 * @details Macro definition for WR  level 4 logs
*/
#define WR_DIAG_LVL4(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)                                                         \
{                                                                                                                                               \
   if(wrCb.init.logMask & SS_DIAG_LVL4)                                                                                                         \
   {                                                                                                                                            \
      ssDiagFix(_tknId, _splArgEnum, ENTWR, wrCb.init.inst, SS_DIAG_LVL4, SS_DIAG_MSG_TYPE_FIXED, _splArg, _arg1, _arg2, _arg3, _arg4, _string);\
   }                                                                                                                                            \
}

/** 
 * @details Macro definition for TeNB APP Varible length logs
*/
#define WR_DIAG_VAR(_tknId, _splArgEnum, _splArg, _string, _stringPtr)                                                         \
{                                                                                                                              \
   if(wrCb.init.logMask & SS_DIAG_VAR)                                                                                         \
   {                                                                                                                           \
      ssDiagFix(_tknId, _splArgEnum, ENTWR, wrCb.init.inst, SS_DIAG_VAR, SS_DIAG_MSG_TYPE_FIXED, _splArg, 0, 0, 0, 0, _string);\
      ssDiagVarible(ENTWR, wrCb.init.inst, SS_DIAG_VAR, SS_DIAG_MSG_TYPE_VAR, _tknId, _splArg, (S8*)_stringPtr);                    \
   }                                                                                                                           \
}
#else
#define WR_DIAG_LVL0(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)
#define WR_DIAG_LVL1(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)
#define WR_DIAG_LVL2(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)
#define WR_DIAG_LVL3(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)
#define WR_DIAG_LVL4(_tknId, _splArgEnum, _splArg, _string, _arg1, _arg2, _arg3, _arg4)
#define WR_DIAG_VAR(_tknId, _splArgEnum, _splArg, _string, _stringPtr)
#endif

/* TODO - This needs to be improved to use LWR to send ARM to LM */
#define ALARM printf


#ifdef RSYS_WIRESHARK
#if 1  /* Imp: Do not delete this */
#define WR_MAC_LTE_START_STRING        "mac-lte"
#define WR_MAC_LTE_RNTI_TAG            0x02
#define WR_MAC_LTE_UEID_TAG            0x03
#define WR_MAC_LTE_SUBFRAME_TAG        0x04
#define WR_MAC_LTE_PREDFINED_DATA_TAG  0x05
#define WR_MAC_LTE_RETX_TAG            0x06
#define WR_MAC_LTE_CRC_STATUS_TAG      0x07
#define WR_MAC_LTE_PAYLOAD_TAG         0x01

int                     g_sockfd, g_sockfd1;
struct sockaddr_in      g_serv_addr, g_serv_addr1;
#endif
#endif

/* adding for logs */

typedef struct rnwCause
{
   U32  logLevel;
   U8   cause[128];
}RnwCause;

typedef struct tportCause
{
   U32  logLevel;
   U8   cause[128];
}TportCause;

typedef struct nasCause
{
   U32  logLevel;
   U8   cause[128];
}NasCause;

typedef struct protCause
{
   U32  logLevel;
   U8   cause[128];
}ProtCause;

typedef struct MiscCause
{
   U32  logLevel;
   U8   cause[128];
}MiscCause;

typedef struct staticAlarms
{
   U32 logLevel;
   U8  alarmType[128];
}StaticAlarms;

typedef struct dynAlarms
{
   U32 logLevel;
   U8  alarmType[128];
}DynamicAlarms;

typedef struct s1Alarm
{
   U32  logLevel;
   U8   s1AlarmType[128];
}S1Alarm;

typedef struct x2Alarm
{
   U32  logLevel;
   U8   x2AlarmType[128];
}X2Alarm;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR__H__ */


/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
