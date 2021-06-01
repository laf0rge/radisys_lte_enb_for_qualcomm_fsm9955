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
               

     File:     wr_umm.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/



#ifndef __WR_UMM_H__
#define __WR_UMM_H__

#include "wr.h"
#include "wr_cmn.h"
#include "wr_emm.h"
#include "wr_umm_meas.h"
#include "wr_umm_anrmeas.h"
#include "wr_umm_csfb.h"
#include "wr_umm_csfb_geran.h" 
#include "wr_lwr.h"
#include "wr_smm_smallcell.h"
#include "wr_utils.h"
#include "wr_ifm_s1ap.h"
#include "wr_umm_perd_strong_cell_meas.h"
#ifdef __cplusplus
extern "C" {
#endif

/* MACROS */
#define WR_EGU_DFLT_MAXTPDUSZ          5000

/* length of IP address in bits */
#define WR_UMM_IPV4_LEN                     32
#define WR_UMM_IPV6_LEN                     128

/* added to Handle defered list */
#define WR_UMM_DEF_LIST                10
/* update UECb DefList */
#define WR_UMM_CPY_DEFR_MSG_TO_TRANSCB(_transCb, _msg)      \
   {\
   _msg->refCnt++;                              \
   _transCb->defList[_transCb->numDefMsgCount] = _msg;   \
   _transCb->numDefMsgCount++;\
   }

/* state of the tunnel */
#define WR_EU_INITIAL                  0
#define WR_EU_CONNECTED                1

#define WR_EU_START_DATBUF             2

/* indication of which packets to forward */
#define WR_EU_NO_FWD                   3
#define WR_EU_FWD_NEW_PKTS             4
#define WR_EU_FWD_OLD_PKTS             5

/* to indicate the reason for changing state of a tunnel */
#define WR_EU_END_MARKER_RECVD         6
#define WR_EU_UE_FOUND_AT_CELL         7

/* to indicate which tunnels to delete on reception of END MARKER */
#define WR_EU_DEL_TUNNELS_NONE         1
#define WR_EU_DEL_FWD_TUNNELS          2
#define WR_EU_DEL_ALL_TUNNELS          3

/* 8 - max DRB for UE
 * 3 - 1 - actual tunnel
 *     2 - forwarding tunnel to receive DL pkts 
 *     3 - forwarding tunnel to receive UL pkts 
 */
#define WR_MAX_UE_TUNNELS              (8 * 3) 

#define WR_UMM_STOP_OLD_TRANSACTION    1 
#define WR_UMM_RESUME_DEF_TRANSACTION  2

/* lower layer configuration status */
#define WR_LOW_LYR_CFG_NOT_STRTD       0
#define WR_LOW_LYR_CFG_IN_PRGS         1
#define WR_LOW_LYR_CFG_COMPL           2
#define WR_LOW_LYR_CFG_REM             3

/* Number of packets that can be buffered during buffering state */
#define WR_EU_PKT_LIMIT                (1024 * 1024)

#define WR_EGTP_TUN_CREATE             1
#define WR_EGTP_TUN_DEL                2
#define WR_EGTP_FWD_TUN_CREATE         3

#define WR_EG_UTIL_MEM_SIZE            1024

#define WR_S1AP_NOT_CONNECTED          0
#define WR_S1AP_CONNECTING             1
#define WR_S1AP_CONNECTED              2

#define WR_UMM_RRC_IDLE                0
#define WR_UMM_RRC_CONNECTING          1
#define WR_UMM_RRC_CONNECTED           2
#define WR_UMM_RRC_CON_RELEASED        3

#define WR_S1_SETUP_DONE               1
#define WR_S1_CONNECTING               2
#define WR_S1_CONNECTED                3

/* Security related info */
#define WR_INTG_KEY_LEN                16
#define WR_CIPH_KEY_LEN                16
#define WR_STRING_S_LEN                7

#define WR_CP_CIPH_KEY                 0
#define WR_UP_CIPH_KEY                 1
#define WR_INTG_KEY                    2

   /*Feature Group Indicator Macros */
#define WR_UMM_UTRA_PS_CHK_BIT           (1 << (32-8))
#define WR_UMM_GERAN_SRVCC_HO_CHK_BIT    (1 << (32-9))
#define WR_UMM_1XRTT_CHK_BIT             (1 << (32-11))
#define WR_UMM_HRPD_CHK_BIT              (1 << (32-12))
#define WR_UMM_INTER_HO_CHK_BIT          (1 << (32-13))
#define WR_UMM_A4_A5_MEAS_CHK_BIT        (1 << (32-14))
#define WR_UMM_NON_ANR_PERD_MEAS_CHK_BIT (1 << (32-16))
#define WR_UMM_ANR_INTRA_PERD_MEAS_CHK_BIT (1 << (32-17))
#define WR_UMM_ANR_INTER_PERD_MEAS_CHK_BIT (1 << (32-18))
#define WR_UMM_ANR_INTERRAT_PERD_MEAS_CHK_BIT (1 << (32-19))
#define WR_UMM_UTRA_MEAS_CHK_BIT         (1 << (32-22))
#define WR_UMM_1XRTT_MEAS_CHK_BIT        (1 << (32-24))
#define WR_UMM_INTER_FREQ_MEAS_CHK_BIT   (1 << (32-25))
#define WR_UMM_HRPD_MEAS_CHK_BIT         (1 << (32-26))
#define WR_UMM_UTRA_CS_CHK_BIT           (1 << (32-27))
#define WR_UMM_LONG_DRX_CHK_BIT          (1 << (32-5))    
#define WR_UMM_SHORT_DRX_CHK_BIT         (1 << (32-4))
#define WR_UMM_CCO_CHK_BIT               (1 << (32-10))
#define WR_UMM_GERAN_MEAS_CHK_BIT        (1 << (32-23))
#define WR_UMM_DCI_FORMAT_3A_CHK_BIT     (1 << (32-1))   
/*rel10*/
/* FGI bit 114 */
#define WR_UMM_UTRA_MEAS_RPRT_BOTH_CHK_BIT (1 << (32-14))

   /* Rel.9 Feature Group Indicator Macros */
#define WR_UMM_UTRA_TDD_MEAS_CHK_BIT      (1 << (32-7))
/*MAX RBs per UE for transaction CB*/
#define WR_MAX_ERAB_PER_UE             8  
#define WR_UMM_SINGLE_RAB_CFG          1
/* RB Direction */
#define WR_DIR_UL                      1
#define WR_DIR_DL                      2
#define WR_DIR_DL_UL                   3

/* SRB QCI value */
#define SRB_QCI_VAL                    5 

/*UCI params Periodicities */
#define WR_UMM_DLCQI_PERIODICITY           80
#define WR_UMM_SR_PERIODICITY              40

#define WR_UMM_SR_CONFIG_IDX               35
#define WR_UMM_CQI_PMI_CONFIG_IDX          77 /*REEMA - changing it 77 to
                                                change offset to avoid
                                                collision with SR for the
                                                same UE*/
#define WR_MAX_RAT_TYPES                   6 /* Max number of RATs */
/** @brief This structure hold the information on Number of
    RAT types and list of RAT types to be included in UE capability
 - U8          noComp                number of RATs               
                                   
 - U8          ratType               list of RAT type 
*/

/*UE History info - start*/
#define WR_UMM_CELL_ID_LENGTH           28
#define WR_UMM_S1AP_ENC_UE_HISTORY      0
/*UE History info - end*/
typedef struct wrUmmRatLst
{
   U8                        noComp;
   U32                       ratType[WR_MAX_RAT_TYPES]; 
} WrUmmRatLst;
/* Macros for UE Radio Resources Configured */
#define WR_UE_RADIO_RES_CFG_DRX           1
#define WR_UE_RADIO_RES_CFG_SR            1<<1
#define WR_UE_RADIO_RES_CFG_CQI           1<<2
#define WR_UE_RADIO_RES_CFG_SRS           1<<3
   
/* Macro to support MAX QCI */
#define WR_MAX_QCI_SUPPORTED 9



/*rel10*/
#define WR_UMM_MIN_VAL_SHORT  0x8000
#define WR_UMM_UTRA_MIN_RSCP_VAL  6
#define WR_UMM_UTRA_MAX_RSCP_VAL  91

#define WR_UMM_IS_GBR_BEARER(qci) ((qci) >=1 && (qci) <= 4) /* More checks to be added for SPS feature */

/* UL Sounding RS specific paramters */
/* SRS Periodicity in milli seconds, refer spec 36.213 */
#define SRS_PERIODICITY_20MS 20
#define SRS_PERIODICITY_40MS 40
#define SRS_PERIODICITY_80MS 80
#define SRS_PERIODICITY_160MS 160

/* SRS Configuration Index starting range for each of the SRS Periodicity, 
 *  * refer spec 36.213 
 *   */
#define SRSCFG_INDX_START_20MS 26

/* SRS Config index increment step for each UE for 20ms SRS Periodicity */
#define SRSCFG_INDX_INCREMENT_20MS 5

#define WR_CHK_IS_QCI_ONE_BEARER_PRES(_ueCb, _isQci1BearPres)\
{\
   U32     idx;\
   U8      rbIdx = 0;\
   for(idx = 0; (idx < WR_MAX_DRBS) && (rbIdx < _ueCb->rbInfo.numOfRbCfgd); idx++)\
   {\
      if(_ueCb->rbInfo.rab[idx].inUse == TRUE)\
      {\
         if(_ueCb->rbInfo.rab[idx].qci == 1)\
         {\
            isQci1BearPres = TRUE;\
            RLOG1(L_INFO, "QCI 1 bearer is found for UE %d", _ueCb->crnti);\
            break;\
         }\
         rbIdx++;\
      }\
   }\
}

   
typedef struct wrUeCb WrUeCb;

/** @brief This structure hold the information of a RAB within UE
 - U8          inUse                 This parameter specifies if the RAB is 
                                     in Use or not 
 - U8          rabId                 This parameter specifies the RAB Identity
 - U8          rbType                This parameter specifies type of the RAB
                                      (SRB0/SRB1/SRB2/DRB)
 - U8          rbDir                 This parameter specifies the direction
                                     (UL/DL/BI-DIR) 
 - U8          rbMode                This parameter specifies the Mode
                                      (AM/UM/TM)
 - U8          rbId                  This parameter specifies the Radio 
                                      Bearer Identity
 - U32         eRABId                This parameter specifies the eRAB 
                                      Identity 
 - U8          lchId                 This parameter specifies the logical 
                                      channel Identity
 - U8          pdcpId                This parameter specifies the PDCP Identity
 - U32         qci                   This parameter specifies the Qos Class 
                                      Identifier
 - WrBitRate   mbr                   This parameter specifies the Maximum 
                                      Bit Rate
 - WrBitRate   gbr                   This parameter specifies the Guaranteed 
                                      Bit rate 
 - U32         remEgtpTunId          This parameter specifies the Remote Egtp 
                                     Tunnel Identity  
 - U32         locEgtpTunId          This parameter specifies the Local Egtp 
                                     Tunnel Identity      
 - CmTptAddr   sgwAddr               This parameter specifies the SGW IP 
                                     Address
 - Bool        isDataFwdAvl          This parameter specifies if Data Forwarding 
                                     is Available
 - SztGTP_TEID remUlDataFwdTeId      This parameter specifies the Remote UL 
                                     data forwarding Tunnel Id
 - SztGTP_TEID remDlDataFwdTeId      This parameter specifies the Remote DL 
                                     data forwarding Tunnel Id 
 - CmTptAddr   datFwdPeerAddr        This parameter specifies the Data Forwarding 
                                     Peer Address
 - U32         datFwdUlLocEgtpTunId  This parameter specifies the Data Fowrarding 
                                     UL local Egtp Tunnel Id 
 - U32         datFwdDlLocEgtpTunId  This parameter specifies the Data Forwarding 
                                     DL local Egtp Tunnel Id
 */
typedef struct wrUeRAB
{
   U8                        inUse; 
   U8                        rabId;
   U8                        rbType;
   U8                        rbDir;
   U8                        rbMode;
   U8                        rbId; 
   U32                       eRABId;
   U8                        lchId;
   U8                        pdcpId; 
   U32                       qci;
   Bool                      isGbrQosPres;
   WrBitRate                 mbr; 
   WrBitRate                 gbr;
   U8                        lcgId;
   U32                       priority;
   U32                       preempCap;
   U32                       preempVul;
   U32                       remEgtpTunId; 
   U32                       locEgtpTunId; 
   CmTptAddr                 sgwAddr; 
   Bool                      isDataFwdAvl;
   /* SPS changes starts */
   Bool                      isDlSpsEnabled;
   Bool                      isUlSpsEnabled;
   /* SPS changes ends */
   Bool                      isEmergencyBearer;
   /*RRM_ARP*/
   U8                        rejectCause;/*To be filled during ERAB Reject*/
} WrUeRAB;

/* maximum number of SRBs allowed per UE */
#define WR_MAX_SRBS          2
/* maximum number of DRBs per UE is 8  LCH Id range [3..10] */
#define WR_MAX_DRBS                  8
#define WR_MAX_DRBS_ALLOWED          8

/** @brief This structure provides the RLC information of SRB
 - U32  tPollRetransmit  This parameter specifies the duration of Poll 
                         Retransmit timer
 - U32  pollPdu          This parameter specifies the value to trigger 
                         poll after reaching this value.
 - U32  pollByte         This parameter specifies the value of number of
                         bytes after which poll is triggered.
 - U32  maxRetxThreshold This parameter specifies the maximum number of
                         retransmissions allowed.
 - U32  tReordering      This parameter specifies the duration of the
                         Re-ordering timer.
 - U32  tStatusProhibit  This parameter specifies the timer value to
                         prohibit transmission of STATUS pdu.
 */
typedef struct wrSrbRlcCfg
{
   U32                  tPollRetransmit;
   U32                  pollPdu;
   U32                  pollByte;
   U32                  maxRetxThreshold;
   U32                  tReorderingDl;
   U32                  tReorderingUl;
   U32                  tStatusProhibit;
}WrSrbRlcCfg;

/** @brief This structure holds the UE Radio Bearer Information
 - U8           rgrCfgMask       This parameter specifies the Scheduler 
                                 configuration mask
 - U8           numOfSrbCfgd     This parameter specifies the number of 
                                 SRBs configured
 - WrUeRAB      srb[WR_MAX_SRBS] This parameter specifies the RAB information
                                 of the SRB.
 - U8           numOfRbCfgd      This paraemter specifies the number of
                                 DRBs configured.
 - WrUeRAB      rab[WR_MAX_DRBS] This parameter specifies the RAB information
                                 of the DRB.
 - WrBitRate    aggBr            This parameter specifies the aggregate Bit 
                                 rate.
 - WrSrbRlcCfg  srbRlcCfg        This parameter specifies the SRB RLC
                                 configuration
 - WrLchChnlCfg lchChnlCfg       This parameter specifies the logical channel
                                 configuration
 */
typedef struct wrUeRBInfo
{
   U8                        rgrCfgMask;
   U8                        numOfSrbCfgd;
   WrUeRAB                   srb[WR_MAX_SRBS];
   U8                        numOfRbCfgd;
   WrUeRAB                   rab[WR_MAX_DRBS];
   WrBitRate                 aggBr;
   WrSrbRlcCfg               srbRlcCfg;
   WrLchChnlCfg              lchChnlCfg;

} WrUeRBInfo;

/* UE history info - start */
#define WR_UMM_UE_MAX_STAY_TIME             4095
/* Maximum time UE stayed in Cell Enhanced Granularity */
#define WR_UMM_UE_MAX_STAY_TIME_ENH_GRAN    40950
#define WR_UMM_UE_HISTORY_MAX_CELLS         16
typedef struct wrUeHstryEutranInfo
{
   WrEcgi                    ecgi;
   U32                       cellSize;
   U32                       timeInCell;
   /* Time UE Stayed in Cell Enhanced Granularity */
   U32                       timeInCellEnhGranularity;
} WrUeHstryEutranInfo;
   
typedef struct wrUeHstryCellInfo
{
   U8                        ratType;
   union
   {
      WrUeHstryEutranInfo    eutranInfo;
      TknStrOSXL             utranInfo;
      /* No info for GERAN as per 3GPP.
       * Need to update GERAN as necessary later */
   } ratInfo;
} WrUeHstryCellInfo;

typedef struct wrUeHstryInfo
{
   U8                         numOfCells;
   WrUeHstryCellInfo          cellInfo[WR_UMM_UE_HISTORY_MAX_CELLS];
} WrUeHstryInfo;
/* UE history info - end */

/* HO States . mobCtrlStates */
typedef enum 
{
   WR_HO_NORMAL = 0,    /* Before initiation of HO.*/
   WR_HO_INITIATED,     /* After initiation of HO.*/
   WR_HO_IN_PROGRESS,   /* Handover is in Progress.*/
   WR_HO_UE_RELEASE,    /* Release the UE Context Once UE has been Handovered Successfully or timer expiry.*/
   WR_HO_MAX_STATES     /* Maximum of Handover state.*/
} WrHoState;


typedef enum 
{
   WR_EVNT_HO_DECISION,         /* Event of Handover Decision.*/
   WR_EVNT_HO_PREP_FAILURE,     /* Event of Handover Preparation failure.*/
   WR_EVNT_HO_PREP_TMR_EXPIRY,  /* Event of Handover Preparation timer expiry.*/
   WR_EVNT_HO_ACK_RCVD,         /* Event of Handover ack received.*/
   WR_EVNT_HO_UE_CNTXT_RLS,     /* Event of Handover UE context release.*/
   WR_EVNT_HO_OVERALL_TMR_EXPIRY, /* Event of Handover overall timer expiry.*/
   WR_HO_SRC_MAX_EVENTS         /* Maximum num of events at Src eNB.*/
}WrSrcFsmEvnts;

typedef enum 
{
   WR_EVNT_HO_PREP_SUCCESS,          /* Event of Handover Preparation success.*/
   WR_EVNT_HO_ADM_FAILURE,           /* Event of Handover Admission Failure.*/
   WR_EVNT_HO_RECONFIG_CMPL_RCVD,    /* Event of Handover Reconfiguration complete.*/
   WR_EVNT_HO_PATH_SWTH_FAILED,      /* Event of Handover path switch request fail.*/
   WR_EVNT_HO_UE_ATTACH_TMR_EXPIRY,  /* Event of Handover UE Attach timer expiry.*/
   WR_EVNT_HO_CANCEL_RCVD,           /* Event of Handover Cancel received.*/
   WR_HO_TGT_MAX_EVENTS              /* Maximum num of events at target eNB.*/
}WrTgtFsmEvnts;
  
/* Handover Type.*/
typedef enum
{
   WR_HO_TYPE_NONE,
   WR_HO_TYPE_INTRA_LTE_S1,  
   WR_HO_TYPE_X2,
   WR_HO_TYPE_IRAT_UTRA,
   WR_HO_TYPE_IRAT_GERAN
} WrHoType;
/*Added for KPI*/
typedef enum  wrHov
{
INTRA_LTE_HOV,
LTE_TO_UTRAN_HOV,
LTE_TO_GERAN_HOV,
UTRAN_TO_LTE_HOV,
GERAN_TO_LTE_HOV,
LTE_X2_HOV
}WrHo;
/*Added for KPI*/
typedef enum  wrHovFreqTyp
{
INTRA_FREQ=1,
INTER_FREQ,
INTER_RAT
}WrHovFreqTyp;

/*Hov dir*/
#define INC_HOV            1
#define OUT_HOV            2
/* HO_REEST */
/* Handover Direction */
typedef enum
{
   WR_HO_DIR_NONE,
   WR_HO_DIR_IN,
   WR_HO_DIR_OUT
} WrHoDir;
/* Feature Group Indicator */
typedef enum
{
   WR_UMM_UTRA_PS_SUPPORTED,
   WR_UMM_1xRTT_SUPPORTED,
   WR_UMM_HRPD_SUPPORTED,
   WR_UMM_CCO_SUPPORTED,
   WR_UMM_INTER_FREQ_HO_SUPPORTED,
   WR_UMM_A4_A5_MEAS_SUPPORTED,
   WR_UMM_NON_ANR_PERD_MEAS_SUPPORTED,
   WR_UMM_ANR_PERD_INTRA_MEAS_SUPPORTED,
   WR_UMM_ANR_PERD_INTER_MEAS_SUPPORTED,
   WR_UMM_ANR_PERD_INTERAT_MEAS_SUPPORTED,
   WR_UMM_GERAN_MEAS_SUPPORTED,
   WR_UMM_UTRA_MEAS_SUPPORTED,
   WR_UMM_1xRTT_MEAS_SUPPORTED,
   WR_UMM_HRPD_MEAS_SUPPORTED,
   WR_UMM_INTER_FREQ_MEAS_SUPPORTED,
   WR_UMM_UTRA_CS_SUPPORTED,
   WR_UMM_LONG_DRX_SUPPORTED,      
   WR_UMM_SHORT_DRX_SUPPORTED,
   WR_UMM_UTRA_MEAS_RPRT_ECN0_SUPPORTED,
   WR_UMM_UTRA_TDD_PERD_MEAS_SUPPORTED,
   WR_UMM_UTRA_TDD_EVNT_MEAS_SUPPORTED,
   WR_UMM_GERAN_SRVCC_HO_SUPPORTED,
   WR_UMM_DCI_FORMAT_3A_SUPPORTED   
} WrUmmFgiType;

/* QCI values*/
typedef enum
{
   WR_UMM_QCI_1,
   WR_UMM_QCI_2,
   WR_UMM_QCI_3,
   WR_UMM_QCI_4,
   WR_UMM_QCI_5,
   WR_UMM_QCI_6,
   WR_UMM_QCI_7,
   WR_UMM_QCI_8,
   WR_UMM_QCI_9
}  WrUmmQci;
/** @brief This structure provides the Uplink HARQ Info
 - U8       ulMaxHarqTx     This parameter specifies the UL Maximum HARQ 
                            transmission
 - U8       deltaHarqOffset This parameter specifies the Delta HARQ
                            offset
 */
typedef struct wrUlHarqInfo
{
   U8       ulMaxHarqTx;
   U8       deltaHarqOffset;
}WrUlHarqInfo;

/** @brief This structure provides the LCG Info
 - U32      effGbr     This parameter specifies the UL GBR for LCG
 */
typedef struct wrLcgInfo
{
   U32       effUlGbr; /* Total LCG UL GBR for LCG. */
   U32       effUlMbr; /* Total LCG UL MBR for LCG. */
}WrLcgInfo;

/** @brief This structure provide the Scheduling Request Configuration
 - U8       srCfgIdx        This parameter specifies the SR configuration
                            index
 - U16      srPUCCHResIdx   This parameter specifies the PUCCH Index
 */
typedef struct wrSrCfg
{
   U16                 srCfgIdx;
   U16                 srPUCCHResIdx;   
   CmRrmDsrTransMax    enDsrTransMax;
} WrSrCfg;

/** @brief This structure provides the QOS configuratiot
 - Bool ambrPres    This parameter specifies the Aggregate Maximum Bit Rate
 - U32  dlAmbr      This parameter specifies the DL AMBR
 - U32  ueBr        This parameter specifies the UE Bit Rate
 */
typedef struct wrQosCfg
{
   Bool ambrPres;
   U32  dlAmbr;
   U32  ueBr;
} WrQosCfg;

/** @brief This structure holds the configuration for ACK/NACK repeatation.
 - Bool    isAckNackEnabled      TRUE if repeatation is enabled
 - U16     pucchAckNackRep       PUCCH resource for HARQ feedback
 - U32     ackNackRepFactor      Number of repeatation
 */
typedef struct wrUeAckNackRepCfg
{
   Bool                 isAckNackEnabled;
   U16                  pucchAckNackRep;
   U32                  ackNackRepFactor; 
}WrUeAckNackRepCfg;

/** @brief This structure contains the UE Capabilities
 -  U8      pwrClass          Power class of the UE
 -  Bool    intraSfFeqHop     TRUE if intra subframe frequency hopping is supported
 -  Bool    resAloocType1     TRUE if resource allocation Type 1 is supported
 -  Bool    simCqiAckNack     TRUE if CQI and HARQ feedback can be transmitted 
                              simultaneously by the UE
 -  Bool    txAntSel          TRUE if transmit antenna selection is enabled
 */
typedef struct wrUeCapCfg
{
    U8                       pwrClass;
    Bool                     intraSfFeqHop;
    Bool                     resAloocType1;
    Bool                     simCqiAckNack;
    Bool                     txAntSel;
} WrUeCapCfg;

/** @brief This structure contains the UL Harq Configuration
 - U8    maxUlHqTx     This parameter specifies the UL HQ Transmission        
 - U8    deltaHqOffst  This parameter specifies the HARQ delta offset       
 */
typedef struct wrUeUlHqCfg
{
   U8                        maxUlHqTx;           
   U8                        deltaHqOffst;        
} WrUeUlHqCfg;

/** @brief This structure contains the BSR Tmr Configuration
 - U8    prdBsrTmr   This parameter specifies the periodic timer value
 - U8    retxBsrTmr  This parameter specifies the BSR retx cfg
 */
typedef struct wrUePerBsrTmrCfg
{
   U16       prdBsrTmr;
   U16       retxBsrTmr;
} WrUePerBsrTmrCfg;

/** @brief This structure contains the configuration for measurement gaps for a
 * UE.
 - Bool  isMesGapEnabled      TRUE if measurement gap is enabled
 - U8    gapPrd               Gap period 40/80 ms
 - U8    gapOffst             Offset within the gap period
 */
typedef struct wrUeMeasGapCfg
{
   Bool                      isMesGapEnabled;
   U8                        gapPrd;
   U8                        gapOffst; 
} WrUeMeasGapCfg;

/** @brief This structure contains the Time Alignment timer configuration for the UE
 - Bool     pres     TRUE if the value of taTmr is valid
 - U16      taTmr    Value of the Time Alignment timer
 */
typedef struct wrTaTmrCfg
{
   Bool                      pres;
   U16                       taTmr; 
}WrTaTmrCfg;

/** @brief This structure contains the configuration for PDSCH dedicated
 * configuration.

 - U32      p_a      Pa parameter as per 36.213
 */
typedef struct wrPdschDedCfg
{
   U32                       p_a;
}WrPdschDedCfg;


/** @brief This structure contains the configuration of PUCCH for a UE
 - U8       pres                 TRUE if the values are valid
 - U32      repeatFactor         Repeation factor for ACK/NACK
 - U16      n1pucchAn            PUCCH resource for ACK/NACK Repeation
 - U32      tddAckNackFdbkMode   HARQ feedback mode for TDD 
                                 [Bundling/Multiplexing]
 */
typedef struct wrPucchDedCfg
{
   U8                        pres;
   U32                       repeatFactor;
   U16                       n1pucchAn;
   U32                       tddAckNackFdbkMode;
}WrPucchDedCfg;

/** @brief This structure contains the UL power control configuration for a UE.
 - Bool            pres       If TRUE values are valid
 - CmLteRnti       tpcRnti    Tx Power command RNTI assigned to the UE
 - U8              idx        Index of the UE in the TPC command
 */
typedef struct wrUeUlPwrCfg 
{
   Bool            pres;           
   CmLteRnti       tpcRnti;         
   U8              idx;
   U8              isFmt3a;
} WrUeUlPwrCfg;

/** @brief This structure contains the configuration for multiplexing of the UCI
 * information on the PUSCH.
 - U8          ackIdx         ACK's Beta offset value
 - U8          riIdx          Rank Index Beta offset value
 - U8          cqiIdx         CQI Beta offset value
 */
typedef struct wrPuschDedCfg
{
   U8                        ackIdx;
   U8                        riIdx;
   U8                        cqiIdx;
   WrUeUlPwrCfg              puschPwrCfg;
}WrPuschDedCfg;   

/** @brief This structure contains the UL power control configuration for the
 * UE.
 - S8          p0UePusch           UE specific value for PUSCH power control
 - U32         deltaMcsEnabled     Corresponds to Ks value in 36.213
 - Bool        accumlationEnabled  TRUE if accumlation is enabled
 - S8          p0UePucch           UE specific value of PUCCH power control
 - U8          pSrsOff             P(srs) Offset is a UE specific 4 bit value
 - U32         filterCoeff         Filter coeffcient
 - U8          trgCqi              Target CQI 
 */
typedef struct wrUlPwrDedCtrl
{
   S8          p0UePusch;
   U32         deltaMcsEnabled;
   Bool        accumlationEnabled;
   S8          p0UePucch;
   U8          pSrsOff;
   U32         filterCoeff;  
   U8          trgCqi;
}WrUlPwrDedCtrl;



/** @brief This structure contains the Transmission mode information for  a UE
  -   U32       txmode                Transmission mode
  -   U8        codebookpres          TRUE if codebook restrictions present
  -   U8        codebktype            Codebook type
  -   union CodeBkSubsetRestS
  -   {
  -    TknStrBSXL   n2TxAntenna_tm3  Codebook restriction for 2 TX mode 3
  -    TknStrBSXL   n4TxAntenna_tm3  Codebook restriction for 4 TX mode 3 
  -    TknStrBSXL   n2TxAntenna_tm4  Codebook restriction for 2 TX mode 4
  -    TknStrBSXL   n4TxAntenna_tm4  Codebook restriction for 4 TX mode 4
  -    TknStrBSXL   n2TxAntenna_tm5  Codebook restriction for 2 TX mode 5
  -    TknStrBSXL   n4TxAntenna_tm5  Codebook restriction for 4 TX mode 5
  -    TknStrBSXL   n2TxAntenna_tm6  Codebook restriction for 2 TX mode 6
  -    TknStrBSXL   n4TxAntenna_tm6  Codebook restriction for 4 TX mode 6
  -   }CodeBkSubsetRest;  
  -   U32       ueTxAntSel            TX antenna selection
 */
typedef struct wrAntennaInfo
{
      U32       txmode;
      U8        codebookpres;
      U8        codebktype;
      union CodeBkSubsetRestS
      {
       TknStrBSXL   n2TxAntenna_tm3;  
       TknStrBSXL   n4TxAntenna_tm3;  
       TknStrBSXL   n2TxAntenna_tm4;  
       TknStrBSXL   n4TxAntenna_tm4;  
       TknStrBSXL   n2TxAntenna_tm5;  
       TknStrBSXL   n4TxAntenna_tm5;  
       TknStrBSXL   n2TxAntenna_tm6;  
       TknStrBSXL   n4TxAntenna_tm6;  
      }CodeBkSubsetRest;  
      U32       ueTxAntSel;

}WrAntennaInfo;

/** @brief This structure contains the Scheduling Request configuration for a UE
    - U16       srPucchRsrcIdx   PUCCH resource index for SR
    - U16       srCfgIdx         SR configuraiton index: determines the periodicity 
                                 and offset
    - U32       dsrTransMax      Max SR transmission as per 36.321
 */
typedef struct wrSchReqCfg
{
      U16       srPucchRsrcIdx;
      U16       srCfgIdx;
      U32       dsrTransMax;
}WrSchReqCfg;

/** @brief This structure the Sounding reference signal's configuration for a
 * UE.
    - U8       pres           If TRUE values are valid
    - U32      srsBw          SRS Bandwidth (UE specific)
    - U32      srsHoppingBw   SRS hopping Bandwidth
    - U8       freqDomPos     Frequency domain i.e. nRRC
    - Bool     duration       Duration (Single or infinite)
    - U16      srsCfgIdx      SRS configuraition index : determines the SRS 
                              periodicity and offset
    - U8       transComb      Transmission comb (0, 1) K(tc)
    - U32      cyclicshift    Cyclic shift parameter
 */
typedef struct wrSrsUlDedCfg
{
   U8                        pres;
   U32                       srsBw;
   U32                       srsHoppingBw;
   U8                        freqDomPos;
   Bool                      duration;
   U16                       srsCfgIdx;
   U8                        transComb;
   U32                       cyclicshift;

}WrSrsUlDedCfg;


/** @brief This structure contains the information for X2 connection CB
 - U32          oldEnbUeX2apId      UE's EnodeB ID
 - U32          newEnbUeX2apId      New UE's Enodeb ID
 - U32          peerId              Peer ID
 - WrUeCb       *ueCb               Pointer to UeCb
 - U8           causeType           Cause Type
 - U8           causeVal            Cause Value
 */
typedef struct wrX2ConCb
{
   U32                          oldEnbUeX2apId;
   U32                          newEnbUeX2apId;
   U32                          peerId;
   /* Holds the cause-of-operation information for a procedure */
   U8                           causeType;
   U8                           causeVal;
} WrX2ConCb;

/** @brief This structure is the list of candidate cell Cbs
 - CmLList    lnk          Link in the linked list
 - WrX2ConCb  *x2ConCb     Connection Control block
 */
typedef struct wrCandCellDb
{
   CmLList                   lnk;
   WrX2ConCb                 *x2ConCb;
} WrCandCellDb;

/** @brief This structure RNTI node in the free list of RNTIs
 - CmLList    lnk    Link to the free list
 - U8         rnti   RNTI
 */
typedef struct wrRntiNode
{
   CmLList                   lnk;
   /* SPS changes starts */
   /* Changing rnti to U16 for future enhancement */
   U16                       rnti;
   /* SPS changes ends */
} WrRntinode;

/* ********************************************************************* */
/*                                                                       */
/* rntiStart          - The first RNTI in the range of RNTIs managed by  */
/*                      APP for HO.                                      */
/* maxRrmRntis        - Number RNTIs in the range of RNTIs managed.      */
/* freeRntiLst        - Linked list of available RNTIs                   */
/* inUseRntiLst       - Linked list of in user RNTIs                     */
/* ********************************************************************* */
/** @brief This structure DB for RNTI management i.e. a free list
 - U16          rntiStart     Start of RNTI
 - U16          maxRrmRntis   Maximum number of RNTIs
 - CmLListCp    freeRntiLst   List of Free RNTIs
 - CmLListCp    inUseRntiLst  List of In use RNTIs
 */
typedef struct wrRntiDb
{
   U16                       rntiStart;
   U16                       maxRrmRntis;
   CmLListCp                 freeRntiLst;
   CmLListCp                 inUseRntiLst;
} WrRntiDb;

/** @brief This structure a node in the preamble's free list implementation
 - CmLList    lnk       Link in to the linked list
 - U8         prmblId   Preamble ID
 */
typedef struct wrPrmblNode
{
   CmLList                   lnk;
   U8                        prmblId;
} WrPrmblnode;

/** @brief This structure is the preamble DB i.e. maintains the free list for
 * the Preambles.
 - U16         prmblStart     Start of the preamble
 - U16         maxPrmbls      Number of preambles
 - CmLListCp   freePrmblLst   Free list of preambles
 - CmLListCp   inUsePrmblLst  List of preambles that are being used
 */
typedef struct wrPrmblDb
{
   U16                       prmblStart;
   U16                       maxPrmbls;
   CmLListCp                 freePrmblLst;
   CmLListCp                 inUsePrmblLst;
} WrPrmblDb;

/** @brief This structure carries the information for SN status for a particular
 * RAB.
- U8                 rbId              RAB Id
- NhuHoPdcpSduInfo   nhuHoPdcpSduInfo  NHU's PDCP SN information IE
*/
typedef struct wrSnSts
{
  U8                         rbId;
  NhuHoPdcpSduInfo           nhuHoPdcpSduInfo;
} WrSnSts;

/** @brief This structure contains the list of RAB's SN status information
- U8         numOfRb       Number of RABs
- WrSnSts    wrSnSts[11]   SN status information
*/
typedef struct wrSnStsInfo
{
  U8                         numOfRb;
  WrSnSts                    wrSnSts[11];
} WrSnStsInfo;

/** @brief This structure contains the S1 ConnectionCB
 - UConnId     suConnId          SuConnId
 - UConnId     spConnId          SpConnId
 - U32         mme_ue_s1ap_id    MME's UE S1AP ID
 - U32         enb_ue_s1ap_id    EnodeB's UE S1AP ID
 - Bool        s1apConnState     S1AP Connection state 
 - U32         nmbERAB           Number of RABs 
 - WrUeCb      *ueCb             Pointer to the UeCb
 - SztCause    cause             Cause field
 */
typedef struct wrS1ConCb
{
   UConnId                   suConnId;
   UConnId                   spConnId;
   U32                       mme_ue_s1ap_id;
   U32                       enb_ue_s1ap_id;
   U8                        s1apConnState;
   U32                       nmbERAB;
   WrUeCb                    *ueCb;
   SztCause                  cause;
   TknU32                    mmeUeS1apId2;
} WrS1ConCb;

/* HO_REEST */
/** @brief This structure the Reestablishment information for a
 * UE.
 WrCrnti                    srcUeCrnti; 
 U16                        pci; 
 TODO
 later we need to have ShartMAC IE
 */

typedef struct wrUeReestInfo
{
   NhuCrnti                   srcUeCrnti; /* serving cell ue id */
   U16                        pci;
   Bool                       isTooLateHO;
   U32                        srcIntAlgo;
   U32                        srcCiphAlgo;
   /* TODO: need to have ShartMAC IE */
}WrUeReestInfo;

typedef struct wrUeCsfbInfo
{
   Bool              dualRx;
   wrCsfbRatType     ratType;
   U8                redirType;
   Bool              nwDTMCap; /*This will be set to true if Target GERAN cell supports DTM*/
   Bool              ueDTMCap; /*This will be set to true if UE  supports DTM*/
   U8                csfbState;
}WrUeCsfbInfo;

typedef NhuCrnti             WrCrnti;

#define WR_UMM_MAX_TRANSID               256
#define WR_UMM_MAX_LCHS                  11
#define WR_UMM_MAX_DRBS                  32
#define WR_NUM_RRC_TRANS                 4
#define WR_NUM_DLDCCH_MSGS               10
#define WR_INVALID_TRANS_ID              0x0100
#define WR_UMM_ERAB_OFFSET               5
/*Fix For ccpu00123304*/
#define WR_UMM_MAX_ERABID                15
#define WR_UMM_QCI_NOT_SUPPORTED         1

/* ccpu00128203 */
#define WR_UMM_SRB_LCG_GRP_ID            0
#define WR_UMM_LCG_GRP_ID_1              1
#define WR_UMM_LCG_GRP_ID_2              2
#define WR_UMM_LCG_GRP_ID_3              3
#define WR_UMM_MAX_DRB_LCG_GRP_ID        3
#define WR_UMM_MAX_LCG_GRP_ID            4 /* 3 DRB LCG and + 1 SRB LCG ID */
#define WR_UMM_TRANS_ID_LCG3_CFG          0x0000000e
#define WR_UMM_TRANS_ID_LCG2_CFG          0x0000000d
#define WR_UMM_TRANS_ID_LCG1_CFG          0x0000000c
#define WR_UMM_TRANS_ID_LCG0_CFG          0x0000000b

/* Funciton Prototypes */

EXTERN S16 wrUmmGetRrcTransId(WrUeCb *ueCb, U8 dlDcchMsg, U8 transId, U8 *rrcTransId);
EXTERN S16 wrUmmFreeRrcTransId(WrUeCb *ueCb, U8 transId); 
EXTERN S16 wrUmmGetTransId(WrUeCb *ueCb, U8 dlDcchMsg, U8 rrcTransId, 
                                                                 U8 *transId);
EXTERN S16 wrUmmRelRrcTransId(WrUeCb *ueCb, U8 dlDcchMsg, U8 rrcTransId);
EXTERN Bool wrUmmUtraTddBandSupported(WrUeCb    *ueCb, U8   band, LwrUtraTddMode    tddMode);
EXTERN Bool wrUmmUtilFeatureGrpIndRel9(U16   cellId, U16    crnti, WrUmmFgiType    isSupported);
EXTERN S16 wrUmmChkUeB2UtraTddCap(U16  cellId, U16  crnti);
EXTERN Bool wrChkUtranTddSupp(WrUeCapInfo *ueCap);
EXTERN Bool wrChkAllRatsForbidden(WrUeCb *ueCb);
EXTERN Bool wrChkSingleRatForbidden(WrUeCb *ueCb, wrCsfbRatType *ratPriority);
/* RLC_DL_MAX_RETX fix */
EXTERN S16 wrUmmPdcpDatCfm(CmLtePdcpId pdcpId);

EXTERN S16 wrUmmInitUeCb(WrUeCb *ueCb);
EXTERN S16 wrUmmConfigurePeriodicANR(WrUeCb *ueCb);
EXTERN Void wrUmmUpdtUeServType(WrUeCb *ueCb);
EXTERN  WrUmmRedirectInfo* wrUmmFillDfltUtraTddRedirectInfo
(
   WrUeCb                    *ueCb, 
   Bool 								laiCellsTobeSent
);

EXTERN Bool wrChkCdma2000_1xRTTSupp
(
 WrUeCapInfo *ueCap
);
EXTERN Bool wrChkCdma2000_HrpdSupp
(
 WrUeCapInfo *ueCap
);
#if 0
/** @brief This structure contains the Location Area Identity
 - WrPlmnId  plmnId  PLMN Identity
 - U16       lac     Location Area Code
 */
typedef struct wrRegLai
{
   WrPlmnId                  plmnId;
   U16                       lac;
} WrRegLai;
#endif
/* Security Information */

/** @brief This structure contains the UE's security information
 - U16   ciphAlgo                    Specifies the ciphering algorithm
 - U16   intgAlgo                    Specifies the integrity algorithm
 - U8    cpCiphKey[WR_CIPH_KEY_LEN]  Specifies the Control Plance Ciphering key
 - U8    upCiphKey[WR_CIPH_KEY_LEN]  Specifies the User Plane Ciphering key 
 - U8    intgKey[WR_INTG_KEY_LEN]    Specifies the Integrity Key
 - U8    kENB[WR_KENB_KEY_LEN]       Specifies the Kenb key
 */
typedef struct wrUeSecCfg
{
   TknBStr32                 encrypCap;
   TknBStr32                 integCap;
   U16                       ciphAlgo; 
   U16                       intgAlgo; 
   U8                        cpCiphKey[WR_CIPH_KEY_LEN]; 
   U8                        upCiphKey[WR_CIPH_KEY_LEN]; 
   U8                        intgKey[WR_INTG_KEY_LEN]; 
   U8                        kENB[WR_KENB_KEY_LEN]; 
} WrUeSecCfg;

/* LTE_ADV_ABS starts */
typedef struct _wrAbsUeInfo
{
   CmLListCp                 cellListCp;
   Bool                      isAbsUe;
} WrAbsUeInfo;
/* LTE_ADV_ABS ends */
/* LTE_ADV starts */
typedef struct _wrLteAdvUeCb
{
   Bool                      isUeCellEdge;
/* LTE_ADV_ABS starts */
   WrAbsUeInfo               absInfo;
/* LTE_ADV_ABS ends */
} WrLteAdvUeCb;
/* LTE_ADV ends */

/* CSG_DEV start */
typedef enum
{
   WR_UMM_CSG_MEMBER = 1,
   WR_UMM_CSG_NOT_MEMBER,
   WR_UMM_CSG_MEMBERSHIP_NOT_KNOWN
} WrUmmCsgMmbrStatus;

typedef enum wrUmmCsfbSupport
{
   WR_UMM_CSFB_WITHOUT_MEAS,
   WR_UMM_CSFB_WITH_MEAS
}WrUmmCsfbSupport;

typedef enum wrUmmEcsfbSupport
{
   WR_UMM_ECSFB_NOT_SUPPORTED,
   WR_UMM_ECSFB_WITHOUT_MEAS,
   WR_UMM_ECSFB_WITH_MEAS
}WrUmmEcsfbSupport;

#define WR_UMM_MAX_CSG_MBR_ENTRY           256
#define WR_UMM_MAX_CSG_MBR_LST_OFFSET      0


/** @brief This structure contains the UE's CSG membership status for a CSG ID
 - CmHashListEnt  hashListEnt          Hash List link
 - U32            csgId                CSG Identity
 - U8             mbrStatus            UE Membership status for this CSG ID
 */
typedef struct wrCsgMbrEntry
{
   CmHashListEnt  hashListEnt;  
   U32            csgId;
   U8             mbrStatus;
} WrCsgMbrEntry;

#define WR_GET_CSG_ID(_cellId, _csgId)\
{\
   WrUmmCellCb  *_cellCb = NULLP;\
   WR_UMM_GET_CELLCB(_cellCb, _cellId);\
   if (NULLP == _cellCb)\
   {\
      RLOG1(L_ERROR, "Failed to get cell CB [%d]", _cellId);\
   }\
   else\
   {\
      _csgId = _cellCb->csgId;\
   }\
}

/** @brief This structure contains the black listed neighbor CSG IDs for this UE.
 *         Handover will not be performed towards these neighbors.
 - U32   csgId                    CSG Identity
 */
typedef struct wrCsgNbrBlLst
{
   CmLList lnk;
   U32 csgId;
   U16 pci;
} WrCsgNbrBlLst;
/* CSG_DEV end */
/*rel10*/
typedef struct wrUmmUtraRankLst
{
   S16   ecno;
   U16   psc;
}WrUmmUtraRankLst;

/*DRX QCI*/
/** @brief This enumerator is used for finding the state of GBR bearer  
 *  
 */
typedef enum wrGBRBearerState
{
   NOT_IN_USE,
   RAB_IN_PROGRESS,
   RAB_ESTABLISHED,
   RAB_RELEASED 
}WrGBRBearerState;


/* @brief This enumerator is used for identifying the UE's service type
 *
 * */
typedef enum wrUeServiceTyp
{
   WR_UMM_NORMAL_SERV = 0,
   WR_UMM_EMERGENCY_SERV
}WrUeServiceTyp;

typedef struct wrHrlMeasCfg
{
   WrUmmMeasEutraMeasObjNode    *intraMeasObj;
   CmLListCp                    interMeasObjLst;
   CmLListCp                    utraFddMeasObjLst;
   CmLListCp                    utraTddMeasObjLst;
   CmLListCp                    cdmaMeasObjLst;
   CmLListCp                    geranMeasObjLst;
}WrHrlMeasCfg;

/** @brief This structure 
  - CmLListCp     transLst    List of transaction for the UE.
  - CmLList       mmeLnk      Link into the list per MME 
  - U8            ueCat       UE category
  - U8            lastTransId Last transaction Identity used
  - U8            transIdInUse[WR_UMM_MAX_TRANSID] - Transactions which are
                     allocated are set to TRUE to avoid duplicate TransIds;
  - U8            avlTransId - which is used in present transaction allocated
                     in the previous Transaction;
  - U16           rrcTrans[WR_NUM_DLDCCH_MSGS][WR_NUM_RRC_TRANS] 
                              RRC transaction
  - U8            lchs[WR_UMM_MAX_LCHS] Array of logical channels
  - U8            drbs[WR_UMM_MAX_DRBS] Array of Data Radio Bearers
  - WrCrnti       crnti      CRNTI of the UE
  - WrCellId      cellId     Cell identity to which UE belongs to.
  - WrMmeId       mmeId      MME Identity of the MME that it is connected to
  - Bool          rrcConnState    RRC Connection State
  - U32           establishCause  Establishment cause of the call
  - WrPlmnId      plmnId          Selected PLMN by the UE
  - WrUeRBInfo    rbInfo          Radio Bearer information.
  - U16           pmiCfgIdx       PMI configuration index
  - WrUeSecCfg    *secCfg         Security configuration
  - WrUeCapInfo   *ueCap          UE capabilities
  - WrS1ConCb     *s1ConCb        S1ap control block
  - SuId          sapId           SAP Id
  - S1apPdu       *pdu            S1AP Pdu
  - WrSTmsi       sTMSI           STMSI of the UE
  - WrGummei      gummei          Globally Unique MME Identity
  - U8            numOfTunnelCreReq Number of Tunnels
  - U8            lowLyrCfgSts      Status of lower layer configuration
  - WrHoType      hoType            Type of handover i.e. S1 or X2
  - WrHoState     mobCtrlState      Mobility control state
  - WrUeHstryInfo ueHstryInfo       UE history info 
  - CmLListCp     candCellList      Candidate cell list
  - WrX2ConCb     *x2ConCb          X2 AP connection CB
  - NhuEncCfmSdus *trnspContainer   Transparent container
  - U16           dedicatedPrmbl    Dedicated preamble allocated to the UE
  - U32           src_mme_ue_s1ap_id Source MME's S1-AP id
  - WrUmmTimer     ueHoAttachTmr      UE's HO timer
  - WrUmmTimer     s1OvrallTmr        OverAll timer
  - WrUmmTimer     rrcReConfigTmr     RRC reconfiguration timer
  - WrUmmTimer     rrcReEstabTmr      RRC reestablishment timer
  - WrUmmTimer     inactivity         Inactivity timer
  - void          *trcStructure      Trace structure
  - WrHoRstrctInfo *hoRstrctInfo     HO restriction information
  - TknU32         subscProfileId    Subscription profile ID
  - TknU32         csFallBackInd     CS fallback indicator
  - WrRegLai       regLai            Location area identifier
  - S8             cqiOffst          CQI offset
  - WrUeUlPwrCfg   uePuschPwr        UL power control configuration for PUSCH
  - WrUeUlPwrCfg   uePucchPwr        UL power control configration for PUCCH
  - WrPdschDedCfg  pdschDedCfg       PDSCH configuration
  - WrPucchDedCfg  pucchDedCfg       PUCCH configuration
  - WrPuschDedCfg  puschDedCfg       PUSCH configuration
  - WrUlPwrDedCtrl ulPwrDedCtrl      UL power control configuration
  - WrTpcPdcchCfg  tpcPdcchCfgPucch  TPC configuration for PUCCH
  - WrTpcPdcchCfg  tpcPdcchCfgPusch  TPC configuration for PUSCH
  - WrSrsUlDedCfg  srsUlDedCfg       SRS configuration
  - WrCqiRptCfg    cqiRptCfg         CQI reporting configuration
  - WrAntennaInfo  antennaInfo       Antenna configuration
  - WrSchReqCfg    schReqCfg         Scheduler configuration
  - WrPdcpCfg      pdcpCfg           PDCP configuration
  - WrUlHarqInfo   harqinfo          UL HARQ configuration
  - WrSrCfg        srCfg             Scheduling Request configuration
  - WrQosCfg       qosCfg            QOS configuration
  - WrTaTmrCfg     taTmrCfg          Time aligment timer configuration
  - WrDrxCfg       drxCfg            DRX configuration
  - WrUeAckNackRepCfg ueAckNackRepCfg ACK NACK Repeation configuration
  - WrUeMeasGapCfg ueMeasGapCfg      Measurement gap configuration
  - WrUeCapCfg     ueCapCfg          UE capabilities configuration
  - WrUeUlHqCfg    ueUlHqCfg         UE UL HARQ configuration
  - U32            dlcchTransId      TransId on DCCH
  - U16            numDefMsgCnt      number of Defered Message Count
  - wrUmmIncMsg    defList[WR_UMM_DEF_LIST]; incomming deflist contains 
                                     wrUmmIncMsg Max 10 assigned
  - TknU32         mgmtBasedMDTAllowed MGMT based MDT allowed value rcvd
                                       from MME/neighbor eNB
  -  Bool            srvccOpPossible;Flag to indicate if SRVCC operation is possible or not
 */
struct wrUeCb
{
   CmLListCp                 transLst;
   CmLListCp                 tmrLst;
   CmLList                   mmeLnk;
   U8                        ueCat;
   U8                        ueCatV1020; /* RRC-REL10-Upgrade */
   U8                        lastTransId;
   U8                        transIdInUse[WR_UMM_MAX_TRANSID];
   U8                        avlTransId;
   U16                       rrcTrans[WR_NUM_DLDCCH_MSGS][WR_NUM_RRC_TRANS];
   U8                        lchs[WR_UMM_MAX_LCHS];
   U8                        drbs[WR_UMM_MAX_DRBS];
   U8                        ueIdx;
   WrCrnti                   crnti;
   WrCrnti                   oldCrnti;
   WrCellId                  cellId;
   WrMmeId                   mmeId;
   Bool                      rrcConnState;
   /*ccpu00130088 */
   Bool                      ueAttachedtoMme;
   U32                       establishCause; 
   WrPlmnId                  plmnId;
   WrUeRBInfo                rbInfo; /* RAB Info (Both SRB & Data RBs) */
   U16                       pmiCfgIdx; /* Cqi related info */
   WrUeSecCfg                *secCfg; /* Security related info */
#ifdef TENB_AS_SECURITY
   WrUeSecCfg                *hzSecCfg; /* Security related info */
#endif
   WrS1ConCb                 *s1ConCb;
   WrMeasUeCb                *measCfg;/*Measurement Configuration*/
   SuId                      sapId;
   S1apPdu                   *pdu;
   Bool                      redirectUe;
   WrSTmsi                   sTMSI;
   WrGummei                  gummei;                  
   WrGummei                  srcGummei;/* will be filled and used in HO when
                                        * connected via F-GW */
   U8                        numOfTunnelCreReq;
   U8                        lowLyrCfgSts; /* indicates whether the configura-
                                              tion of lower layers is complete
                                              or in progress */
   WrHoType                  hoType;       /* indicates the type of HO -
                                              (1 - S1 or 2 - X2) */
   WrHoState                 mobCtrlState; /* indicates the UE HO state */
   WrUeHstryInfo             ueHstryInfo;   /* UE history info */ 
   CmLListCp                 candCellList; /* linked List for WrCandCellDb */
   /* sri - HOFIX */
   WrX2ConCb                 x2ConCb;     /* X2 Connection control Block*/
   NhuEncCfmSdus             *trnspContainer; /* Container which contains 
                                              encoded data.*/
   U16                       dedicatedPrmbl; /* Dedicated preamble allocated for
                                              the UE during the successful
                                              admission of the UE*/
   U32                       hoTransId;
   U32                       anrMeasTransId;
   U32                       perdStrongCellMeasTransId;
   U32                       eventAnrTransId;
   U32                       rrcCsfbTransId;
   U32                       src_mme_ue_s1ap_id; /* source MMEUES1AP Id received
                                                    through X2 HO REQ. This will
                                                    be used to fill the same in 
                                                    the PATH SWITCH Request */
   WrUmmTimer                ueHoAttachTmr; /* Timer which is used by UE During
                                           synchronization.*/
   WrUmmTimer                s1TgtEnbRelocTmr;
   WrUmmTimer                s1OvrallTmr;/* Timer which is used by eNB.*/
   WrUmmTimer                rrcReConfigTmr;/* Timer which is used by eNB.*/
   WrUmmTimer                rrcReEstabTmr;/* Timer which is used by eNB.*/
   void                      *trcStructure; /*!< trace info */
   WrHoRstrctInfo            *hoRstrctInfo; /*!< Handover Restriction info */
   U8                        nhChainCnt;
   TknU32                    subscProfileId;
   U32                       csfbRedirTransId;
   WrRegLai                  regLai;
   S8                        cqiOffst;
   WrUeUlPwrCfg              uePuschPwr;
   WrUeUlPwrCfg              uePucchPwr;
   WrPdschDedCfg             pdschDedCfg;
   WrPucchDedCfg             pucchDedCfg;
   WrPuschDedCfg             puschDedCfg;
   WrUlPwrDedCtrl            ulPwrDedCtrl;
   WrSrsUlDedCfg             srsUlDedCfg;
   WrCqiRptCfg               cqiRptCfg;
   WrAntennaInfo             antennaInfo;
   WrSchReqCfg               schReqCfg;
   WrPdcpCfg                 pdcpCfg;
   WrUlHarqInfo              harqinfo;
   WrSrCfg                   srCfg; 
   WrQosCfg                  qosCfg;
   WrTaTmrCfg                taTmrCfg;
   WrDrxCfg                  drxCfg;
   WrUeAckNackRepCfg         ueAckNackRepCfg;
   WrUeMeasGapCfg            ueMeasGapCfg;
   WrUeCapCfg                ueCapCfg;
   WrUeUlHqCfg               ueUlHqCfg;
   U32                       dlcchTransId;
   WrUeCapInfo               *ueCap; /* Ue Capability Info */
   WrUmmTimer                radResModTmr;
   WrUmmTimer                intialCntRspTmr;
   TknStrOSXL                *rrcContr;
   U8                        locReportType;  /* indicates the type of Location Report, 
                                             TRUE - Indicates, need to report upon 
                                                    UE changes cell
                                             FALSE - No report is required */
   WrUmmTimer                cqiRprtTmr;
   WrUmmTimer                ueCtxRelTmr;
/* LTE_ADV starts */
   WrLteAdvUeCb              lteAdvCb;
/* LTE_ADV ends */
   WrUePerBsrTmrCfg          uePerBsrTmrCfg;   
   U32                       numTtiTicks;
/* UE history info - start */
   EpcTime  ueAttachTime; /*timestamp(ms)when MME receives ATTACH Complete from ENB*/
/* UE history info - end */
#ifdef WR_RSYS_KPI 
   EpcTime  rrcConSetupReqTm; /*timestamp(ms)when ENB recevies RRC con Req from UE(KPI)*/
   EpcTime  rrcConSetupCmplTm; /*timestamp(ms) when ENB recevies RRC cmpl from UE(KPI)*/
   SztCause  hovSztCause;   /*Added for KPI hoTrigerred cause*/
   CztCause  hovCztCause;   /*Added for KPI*/
   U8        hov;        /*Added for KPI*/
   Bool      measGapUsd; /*Added for KPI*/
   U8        freqTyp;  /*Added for KPI : INTRA_FREQ or INTER_FREQ*/
   Bool      s1LogicalConnEst;
#endif/*WR_RSYS_KPI*/
   U8        hovDir;     
   U8                        hoSrcMeasIdLstCnt;
   U32                       hoSrcMeasIdLst[32];
   U8                        hoSrcRprtIdLstCnt;
   U32                       hoSrcRprtIdLst[32];
   U8                        hoSrcMeasObjIdLstCnt;
   U32                       hoSrcMeasObjIdLst[32];

   /* HO_REEST */
   WrUeReestInfo             hoReestInfo;
   WrUeCsfbInfo              csfbInfo;
   /* eCSFB - Added for ecsfb */
   TknU32                    eCsfbRandNum;
   U8                        causeTauRequiredSet;
   U8                        bResCfgBitMap;
   /* CSG_DEV */
   CmHashListCp              csgMbrLst; /*Stores WrCsgMbrEntry */
   CmLListCp                 csgNbrBlLst; /* Black-listed CSG neighbors */
   /*DRX QCI*/
   WrGBRBearerState          isGBRBearerExists;
   /* SPS changes starts */
   WrSpsCfg                  spsCfg;
   Bool                      isSrMaskEnabled;
   /* SPS changes ends */
   TknU32                    mgmtBasedMDTAllowed;
   Bool                      mobLaiFlag; /*This flag is set to TRUE if UE sends
                                           registered MME IE*/
   U16                       lac; /*for IRAT.MOBILITY.LAI KPI store 
                                   MMEGI from registered MME IE*/
   WrUeServiceTyp            ueServTyp;
   WrPlmnId                  fnlzdPLMN;
   WrHrlMeasCfg              hrlMeasCfg;       /*!< Measurement configurations based on HRL. */
   Bool                      isFullCfgReqd;   /* This flag indicates does  perform Full Reconfiguration requried */ /* RRC-REL10-Upgrade */
   WrLcgInfo                 mappedLcgInfo[WR_UMM_MAX_LCG_GRP_ID];  /*!< Lcg related Information */
   Bool                      srvccOperPossible; /*!< Flag to indicate if SRVCC operation is possible or not */
   Bool                      isMeasCfgForPerdStrongCellRptCfg; /*!< flag is added to configure the measurement interval received from BRDCM-SON */
   WrUmmMeasAddInfo          perdStrongCellMeasInfo;
   /* Allocated Index of UL SRS CfgIndex value for this UE */
    U8                        idxSrsCfgIdx;    
    Bool                      tobeReleased;
};


/** @brief This structure contains the MME control information
 - WrMmeId      mmeId     MME Identity
 - U16          mmeCfgd   
 - CmLListCp    ueLst     List of UEs associated with this MME
 */
typedef struct wrUmmMmeCb
{
   WrMmeId                   mmeId;
   U16                       mmeCfgd;
   CmLListCp                 ueLst;
} WrUmmMmeCb;

#define  WR_UMM_INCLUDE_FIELD(pField, len, data)\
{\
      pField->fldIncl = TRUE;\
      pField->fldSize = len;\
      pField->fldData = data;\
}
/* Used to store parameter into below format for encoding purpose */
typedef struct wrMobParamsFields
{
  Bool  fldIncl;
  U8    fldSize;
  U32   fldData;
}WrCdma2000Field;

#define CDMA2000_MOB_PARAMS_MAX_FIELDS 65

typedef struct mobParams
{
   WrCdma2000Field   fields[CDMA2000_MOB_PARAMS_MAX_FIELDS];
   TknPres           pzHystEnabled;    /* Packet zone hysteris enabled */
   TknPres           pzHystInfoIncl;   /* Pkt zone Hys Info Included indicator */
   TknPres           pzHystListLen;    /* Pkt zone Hys list length */
   TknPres           pzHystActTimer;   /* Pkt zone Hys Activation timer */
   TknPres           pzHystTimerMul;   /* Pkt zone Hys timer multiplier */
   TknPres           pzHystTimerExp;   /* Pkt zone hys timer exponent */
   U8                fpcFchInitSetptRC3;
   U8                fpcFchInitSetptRC4;
   U8                fpcFchInitSetptRC5;
   U8                fpcFchInitSetptRC11;
   U8                fpcFchInitSetptRC12;
}WrMobParams;

/* Conversion from OAM params to APPLICATION format */
typedef struct wrCdma2000Pdu
{  
  U8 pduType;
  union
  {
    WrMobParams  mobParams;
  }msgInfo;

}WrCdma2000Pdu;

/* SPS changes starts */
#define WR_RNTI_GAP  10
#define WR_DELTA_SPS_RNTI 5
/* SPS changes ends */

/*This structure is used for KPI IRAT.MOBILITY.LAI*/
typedef struct wrLaiLst
{
   Bool       pres;
   U16        lac;
   WrPlmnId   plmnId;
}WrLaiLst;

/* Each WrUeCell contains all the UEs that are establishing or have       */
/* established RRC connections to the cell. The identity for each UE is   */
/* RNTI allocated to the UE.                                              */
/** @brief This structure contains the Cell Information
 - WrCellId   cellId                      cell Identity
 - WrPrmblDb  prmblDb                     Preamble DB.
 - WrRntiDb   rntiDb                      RNTI DB.
 - WrUeCb     *ueCbs[WR_MAX_UES_PER_CELL] List of Ue control blocks
 */
typedef struct wrUmmCell
{
   WrCellId                  cellId;
   WrCellState               cellState;
   Bool                      isFree;
   LwrCsfbSupport            measFlagForCsfbUtra; /* Enable/Disable Measuremnet for CSFB UTRA*/
   LwrCsfbSupport            measFlagForCsfbCdma; /* Enable/Disable Measuremnet for CSFB CDMA*/
   LwrEcsfbSupport           measFlagForEcsfb;    /* with/without UE measurement for eCSFB */
   U16                       noUeCntxToRel;
   WrPrmblDb                 prmblDb;
   WrRntiDb                  rntiDb;
   U16                       maxUesPerCell;        /* Dheeraj: Maximum number of UEs per Cell*/
   WrUeCb                    *ueCbs[WR_MAX_UES_PER_CELL];
   WrUmmMeasCellCb           *measCellCb;
   LwrCellMeasCfgGrp         *measCellCfg;
   WrRabCfgParams            wrRabCfgParams[WR_MAX_QCI + WR_QCI_IDX];
   WrCqiRptCfg               cqiRptCfg;
   LwrMacMainConifg          macMainCfg;
   LwrDrbPdcpInfo            pdcpInfo; 
   WrSchReqCfg               schReqCfg;
   WrPuschDedCfg             puschDedicated;
   LwrPdschCfgDedicated      pdschCfgDedicated;
   LwrUeUlPwrCfg             uePucchPwr;
   LwrQosCfg                 qosCfg; 
   TknStrOSXL                mobParam1xOct;
   U8                        lastDelUeIdx; /* ccpu00145343: Keep track of last
                                              deleted UE Index, do not allocate it
                                              for next UE */
   /* SPS changes starts */
   WrRntiDb                  spsRntiDb;
   /* SPS changes ends */
   /* CSG_DEV */
   U32                   csgId;
   U8                    csgAccessMode;
   LwrCsfbGeranCfg           csfbToGeranCfg;
   WrLaiLst                  lstLai[LWR_MAX_NUM_MBLTY_LAI];
   U8                    rabArpForEmerServ;
   CmTimer               ueCtxRelGuardTmr;
} WrUmmCellCb;

/* The UEs in the enodeb are arranged per cell. Each UE is identified by  */
/* the cell id and its CRNTI. The cell id here refers to identity that is */
/* given to the cell for administration which can be any unsigned integer */
/* other than 0. This cell identity is shared across UMM and EMM modules  */
/** @brief This structure contains the UE database with the list of cells and mmes.
-  WrUmmCellCb  cells[WR_MAX_CELLS] Specifies the cell information
-  WrUmmMmeCb   mmes[WR_MAX_MMES]   Specifies the MME information.
 */
typedef struct wrUeDb
{
   WrUmmCellCb               cells[WR_MAX_CELLS];
   WrUmmMmeCb                mmes[WR_MAX_MMES];
} WrUeDb;

#define WR_UMM_GET_CELLCB(_cellCb, _cellId){\
   U32 _idx;\
   _cellCb = NULLP;\
   for(_idx = 0; _idx < WR_MAX_CELLS; _idx++) {\
      if (wrUeDb.cells[_idx].cellId == _cellId && \
         wrUeDb.cells[_idx].isFree == TRUE) {\
         _cellCb = &wrUeDb.cells[_idx];\
      }\
   }\
}                                                        

EXTERN WrUeDb                wrUeDb;

/* Comment in absence of caller of this macro
#define WR_INS_UECB_CELLCB(_ueCb ,_cellId,_crnti)\
{\
   U8 _idx = 0; \
   for(_idx = 0; _idx < WR_MAX_UES_PER_CELL; _idx++)\
   {\
      if((wrUeDb.cells[_cellId]).ueCbs[_idx] == NULLP)\
      {\
         (wrUeDb.cells[_cellId]).ueCbs[_idx] = ueCb;\
         (wrUeDb.cells[_cellId]).ueCbs[_idx]->crnti = _crnti;\
         break;\
      }\
   }\
}
*/

EXTERN Void wrUmmInit(Void);

EXTERN PUBLIC U8 wrUmmGetLcgIdForQci
(
 U32   cellId, 
 U8    qci
);
#ifdef WR_UMM_WITH_RRM
/** @brief This structure 
 - U32                       srPeriodicity  This parameter specifies the
                                           SR periodicity
 - U16                       srRes[MAX_SR_RES] This parameter specifies the
                                            SR Resources.
 */
typedef struct WrRrmSrRes
{
   U32                       srPeriodicity;
   U16                       srRes[];
   
} WrRrmSrRes;

/** @brief This structure defines the SRS frequency resource
 - U8     ffi     nRRC i.e. starting frequency for SRS
 */
typedef struct wrRrmSrsRes
{
   U8                        ffi;
} WrRrmSrsRes;

/** @brief This structure 
 - U32  cqiPeriodicity Specifies the CQI periodicity
 - U16  cqiResources   Specifies the CQI resource information
 */
typedef struct wrRrmCqiRes
{
   U32                       cqiPeriodicity;
   U16                       cqiResources;
} WrRrmCqiRes;

/** @brief This structure 
 - WrRrmSrRes    srRes  Specifies the Service Request Resource information
 - WrRrmSrsRes   srsRes Specifies the Sounding Reference Signal 
                       Resource information
 - WrRrmCqiRes   cqiRes Specifies the CQI resource information
 */
typedef struct wrRrmCell
{
   WrRrmSrRes                srRes;
   WrRrmSrsRes               srsRes;
   WrRrmCqiRes               cqiRes;
} WrRrmCell;

/** @brief This structure contains the Radio Resource Control information
 - WrRrmCell  cells[WR_MAX_CELLS] Specifies the cell information of RRM.
 */
typedef struct wrRrmCb
{
   WrRrmCell                 cells[WR_MAX_CELLS];
} WrRrmCb;
EXTERN WrRrmCb               wrRrmCb;
#endif

/* The allowed values for event parameter of function wrUmmSndCtxtRelReq */
#define WR_UMM_CTXT_REL_INACTIVITY               0
#define WR_UMM_CTXT_REL_SW_ERROR                 1
#define WR_ENB_INIT_UE_REL                       2
#define WR_UMM_CTXT_CSFB_REL                     3
#define WR_UMM_HIGH_PRIOR_UE_ADM                 4
#define WR_UMM_CTXT_RLF_REL                      5
#define WR_UMM_REL_CAUSE_SUCES_HO                6
/* ccpu00126817 */
#define WR_UMM_REL_CAUSE_HO_SN_NOTREVD           7
/* ccpu00126336 */
#define WR_UMM_REL_CAUSE_UL_TNL_MOD_FAIL         8
/*ccpu00130088: Reject Causes for handling the Re-establishment procedure when there is  
  initial and RRC connection transaction are ongoing */
#define WR_UMM_REJ_CAUSE_RRC_CONNEC_GOING_ON     9
#define WR_UMM_REJ_CAUSE_INIT_CTXT_GOING_ON      10
/* CSG_DEV */
#define WR_UMM_CTXT_REL_CSG_SUBSCRIPTION_EXP     11
#define WR_UMM_CTXT_REL_S1_U_ERROR       12
/* UE connection lost */
#define WR_UMM_UE_LOST                           13
#define WR_UMM_SEMANTIC_ERROR                    14
#define WR_UMM_IRAT_REDIR_REL_CAUSE              15

EXTERN U8  wrUmmPrioIdforQci[WR_MAX_QCI_SUPPORTED];


EXTERN S16 wrUmmSndCtxtRelReq(U16 crnti, U16 cellId, U8 event);

/* Internal utilities across different files of UMM module                */
EXTERN S16  wrUmmGetLch(WrUeCb *ueCb, U8 *lch);
EXTERN S16  wrUmmRelLch(WrUeCb *ueCb, U8 lch);
EXTERN S16  wrUmmGetDrb(WrUeCb *ueCb, U8 *drb);
/* ccpu00128207 */
EXTERN S16  wrUmmSetDrb(WrUeCb *ueCb, U8 drb);
EXTERN S16  wrUmmSetLch(WrUeCb *ueCb, U8 lch);
EXTERN S16  wrUmmRelDrb(WrUeCb *ueCb, U8 drb);
EXTERN S16  wrUmmGetDrbIdFromErabId(WrUeCb *ueCb, U8 erabId, U8 *drbId);

EXTERN S16  wrUmmRelUeCb(WrUeCb *ueCb);
EXTERN Void wrUmmEnbStats (Void);

EXTERN Void wrUmmInitRrcTrans(WrUeCb *ueCb);
EXTERN S16  wrUmmPrcLclCtxtRel(WrUeCb *ueCb, U32 relCause);
EXTERN S16  wrUmmAddUeToMme(WrUeCb *ueCb, WrMmeId mmeId);
EXTERN S16  wrUmmGetUe(WrUeCb **ueCb,WrCellId cellId,U16 crnti);
EXTERN S16  wrUmmGetReestUe(WrUeCb **ueCb,WrCellId cellId,U16 crnti);
EXTERN S16 wrUmmGetUeFrmUeIdx(WrUeCb **ueCb, WrCellId cellId, U8 idx);
EXTERN S16  wrUmmGetUeCbforRabRel(WrUeCb **ueCb,WrCellId cellId);
/* HO_REEST */
EXTERN S16  wrUmmStoreUeInHoLst (WrCellId cellId, U16 sourceCrnti,U8 ueIdx, U16 sourcePhyCellId);
EXTERN S16 wrUmmGetUeFrmHoLst (WrUeCb **ueCb, WrCellId cellId,U16 crnti, U16 phyCellId);
EXTERN S16 wrHoHdlMeasRprt (WrUeCb *ueCb, NhuMeasResults *measRprt);
EXTERN S16 wrEmmUpdateStat(U8 moduleType, WrCellId wrCellId, Action updType, S8 incStep, U32 subType);
EXTERN S16 wrEmmUpdateKpiCounter(SztCause *s1cause);
EXTERN S16 wrUmmMeasGapReqdInterRat
(
 WrUeCb                       *ueCb,
 U8                           servingBand,
 U8                           targetBand,
 U8                           ratType,
 LwrUtraTddMode               tddMode,
 Bool                         *result
 );
EXTERN S16 wrUmmMeasGapReqdInterfreq
(
 WrUeCb                       *ueCb,
 U8                           servingBand,
 U8                           targetBand,
 Bool                         *result
 );
EXTERN S16 wrUmmAnrChkUeCap
(
 WrUeCb               *ueCb,
 WrUmmMeasCellTyp     cellFreqType,
 WrUmmMeasRptCfgType  measAnrType
);

EXTERN S16 wrUmmStartAnr
(
   WrUeCb               *ueCb, 
   Bool                 recfgReq
);

EXTERN S16 wrUmmStopAnr
(
   WrUeCb               *ueCb
);

EXTERN Bool wrUmmRedirectionUTRA
(
 WrCellId                     cellId,
 U16                          crnti
);
EXTERN S16 wrUmmFndEUTRABand
(
 U32        arfcn,
 U8         *result
);
EXTERN S16 wrUmmFndUTRABandFDD
(
 U32        arfcn,
 U8         *result
);
EXTERN S16 wrUmmFndUTRABandTDD_128
(
 U32        arfcn,
 U8         *result
);
EXTERN S16 wrUmmFndUTRABandTDD_384
(
 U32        arfcn,
 U8         *result
);
EXTERN S16 wrUmmFndUTRABandTDD_768
(
 U32        arfcn,
 U8         *result
);
EXTERN  Bool wrUmmUTRAFDDBandSupported
(
 WrUeCb     *ueCb,
 U8            band
);
EXTERN Bool wrUmmUTRATDD128BandSupported
(
 WrUeCb     *ueCb,
 U8            band
);
EXTERN Bool wrUmmUTRATDD384BandSupported
(
 WrUeCb     *ueCb,
 U8            band
);
EXTERN  Bool wrUmmUTRATDD768BandSupported
(
 WrUeCb     *ueCb,
 U8            band
);
EXTERN Bool wrUmmChk1xRttCdmaBandSupported
(
  WrUeCb                      *ueCb,
  U8                           band
);

EXTERN Bool wrUmmChkCdmaHrpdBandSupported
(
  WrUeCb                      *ueCb,
  U8                           band
);

EXTERN Bool wrUmmEUTRABandSupported
(
  WrUeCb                      *ueCb,
  U8                          band,
  U8                          duplexTyp
);

EXTERN Bool wrUmmUtilFeatureGrpInd
(
 U16                         cellId,
 U16                         crnti,
 WrUmmFgiType                isSupported
);
EXTERN Bool wrUmmUtilFeatureGrpIndV1020
(
 WrUeCb                      *ueCb,
 WrUmmFgiType                isSupported
); 

EXTERN Void wrUmmCommitUeRadioResDedInfo
(
   WrUeCb                        *ueCb,
   RmuUeRadioResCfg              *stUeRadioResCfg
);
EXTERN Void wrUmmCommitUePhyCfgDedResInfo
(
   WrUeCb                   *ueCb,
   RmuUeDedPhyCfg         *stPhyCfgDed
);
EXTERN Void wrUmmCommitUePhyCfgDedHoResInfo
(
   WrUeCb                *ueCb,
   RmuUeHoRsp            *ueHoResp
);
EXTERN S16 wrUmmCfgCellMeasTmrs
(
  U16                         cellId,
  U32                         csfbTransTmrVal
);

EXTERN WrUmmRedirectInfo* wrUmmFillUtraFddRedirectInfo
(
   WrUeCb                    *ueCb,
	Bool								laiCellsTobeSent
);

EXTERN WrUmmRedirectInfo* wrUmmFillEutraRedirectInfo
(
   WrUeCb                    *ueCb
);
EXTERN S16 wrUmmChkUeNeedMeasGapForInterFreq
(
   WrUeCb                     *ueCb, 
   U32                        targetArfcn, 
   Bool                       *output
);

/* ccpu00126532 */
#ifdef TENB_AS_SECURITY
EXTERN S16 wrUmmSecExtractSecKeys1 (WrUeCb *ueCb, U32 transId,
      U8 keyDerType);
EXTERN S16 wrUmmSecExtractSecKeys (WrUeSecCfg *secCfg, TknStrBSXL *secKey,
      Bool *asKeyGenerated, U32 transId);
#else
EXTERN S16 wrUmmSecExtractSecKeys (WrUeSecCfg *secCfg, TknStrBSXL *secKey);
#endif

#define WR_GET_CQIPMI_CFGINDX(cqiPmiCfgIdx, ueCb)\
{\
   if(ueCb->ueIdx < WR_NUM_SR_CQI_RES)\
   {\
      cqiPmiCfgIdx = WR_UMM_CQI_PMI_CONFIG_IDX + ((ueCb->cqiDrxOffset) % WR_UMM_DLCQI_PERIODICITY);\
   }\
   else\
   {\
      cqiPmiCfgIdx = WR_INVALID_SR_INDEX;\
   }\
}

EXTERN Void wrUmmCqiRptCfg
(
    WrCqiRptCfg                  *cqiRpt
);
EXTERN Void wrUmmMacMainCfg 
(
  LwrMacMainConifg     *macMainCfg  
);
EXTERN Void wrUmmPdcpCfg 
(
 LwrDrbPdcpInfo   *pdcpInfo
);
EXTERN Void wrUmmPuschDedCfg 
(
 LwrPuschDedCfg    *puschDedicated
);
EXTERN Void wrUmmSchReqCfg 
(
 LwrSchReqCfg     *schReqCfg
);
EXTERN Void wrUmmPucchPwrCfg 
(
 LwrUeUlPwrCfg       *pucchPwrCfg
);
EXTERN Void wrUmmPdschPwrCfg 
(
LwrPdschCfgDedicated  *pdschCfgDedicated
);
EXTERN Void wrUmmQosCfg 
(
 LwrQosCfg             *qosCfg
);
EXTERN Void wrUmmCsfbGeranCfg
(
 LwrCsfbGeranCfg       *csfbToGeranCfg
);
EXTERN S16 wrUmmCfgCellMeas
(
    U16                        cellId,
   LwrCellMeasCfgGrp            *measCfgGrp
);
EXTERN S16 wrUmmCfgRabParams
(
    U16                        cellId,
   LwrRabPolicyCfgGrp         *rabPolicyCfgGrp
);
EXTERN S16  wrUmmReCfgCellMeas
(
    U16                        cellId,
   LwrCellMeasCfgGrp            *measReCfgGrp
);
EXTERN S16 wrUmmReCfgRabParams
(
    U16                        cellId,
   LwrRabPolicyCfgGrp         *rabPolicyReCfgGrp
);
EXTERN S16 wrUmmPdcpDatCfm
(
   CmLtePdcpId                  pdcpId
);
EXTERN S16 wrUmmPrcX2ErrInd(CztLclErrInd   *cztLclErrInd);
EXTERN Void wrUmmDeAllocUeCap(WrUeCapInfo *ueCap);
/* Comment in absence of caller of this macro
#define WR_GET_RI_CFGINDX(riCfgIdx, ueCb)\
{\
   riCfgIdx = wrSmDfltRiConfigIdx + (ueCb->ueIdx % WR_MAX_UES_PER_CELL); \
}
*/
   /* Changed the logic to fix the CQI or RI overlapping with SR from T3K */
#define WR_GET_SR_CFGINDX(srCfgIdx, ueCb)\
{\
   if(ueCb->ueIdx < (WR_MAX_SR_CQI_CONFIGURED_UES/2))\
   {\
      srCfgIdx = WR_UMM_SR_CONFIG_IDX + ((ueCb->ueIdx * 2 + 2) % WR_MAX_SR_CQI_CONFIGURED_UES); \
   }\
   else if (ueCb->ueIdx < WR_MAX_SR_CQI_CONFIGURED_UES)\
   {\
      srCfgIdx = WR_UMM_SR_CONFIG_IDX + (WR_MAX_SR_CQI_CONFIGURED_UES - ((ueCb->ueIdx - (WR_MAX_SR_CQI_CONFIGURED_UES / 2 ))*2+1)) % WR_MAX_SR_CQI_CONFIGURED_UES;\
   }\
   else\
   {\
      srCfgIdx = WR_INVALID_SR_INDEX;\
   }\
}
/*ccpu00139148 - Changed the below macros as tpcRnti should be same
 * for the group and index should be unique */

#define WR_GET_PUCCH_FMT3_TPC_RNTI_AND_IDX(pucchFmt3TpcRnti, pucchFmt3Idx, ueCb, cellCb)\
{\
  pucchFmt3TpcRnti = cellCb->schdCfg.pwrCfg.pucchPwrFmt3Rnti + (ueCb->ueIdx/wrSmNumUesGrpPwrCtrlFmt3);  \
  pucchFmt3Idx = (ueCb->ueIdx % wrSmNumUesGrpPwrCtrlFmt3) + 1; \
}
#define WR_GET_PUCCH_FMT3A_TPC_RNTI_AND_IDX(pucchFmt3ATpcRnti, pucchFmt3AIdx, ueCb, cellCb)\
{\
   pucchFmt3ATpcRnti = cellCb->schdCfg.pwrCfg.pucchPwrFmt3aRnti + (ueCb->ueIdx/wrSmNumUesGrpPwrCtrlFmt3A); \
   pucchFmt3AIdx = (ueCb->ueIdx % wrSmNumUesGrpPwrCtrlFmt3A ) + 1; \
}

#define WR_GET_PUSCH_FMT3_TPC_RNTI_AND_IDX(puschFmt3TpcRnti, puschFmt3Idx, ueCb, cellCb)\
{\
   puschFmt3TpcRnti =  cellCb->schdCfg.pwrCfg.puschPwrFmt3Rnti + (ueCb->ueIdx/wrSmNumUesGrpPwrCtrlFmt3); \
   puschFmt3Idx = (ueCb->ueIdx % wrSmNumUesGrpPwrCtrlFmt3) + 1; \
}

#define WR_GET_PUSCH_FMT3A_TPC_RNTI_AND_IDX(puschFmt3ATpcRnti, puschFmt3AIdx, ueCb, cellCb)\
{\
   puschFmt3ATpcRnti =  cellCb->schdCfg.pwrCfg.puschPwrFmt3aRnti + (ueCb->ueIdx/wrSmNumUesGrpPwrCtrlFmt3A); \
   puschFmt3AIdx = (ueCb->ueIdx % wrSmNumUesGrpPwrCtrlFmt3A) + 1; \
}

#define WR_GET_NUM_ANT_PORT(cellCb,numAntPorts)\
   switch(cellCb->numAntPorts)\
{\
   case 1:                          \
      numAntPorts = NhuAntennaInfoCmnantennaPortsCountan1Enum; \
      break;\
   case 2:                          \
      numAntPorts = NhuAntennaInfoCmnantennaPortsCountan2Enum; \
      break;\
   case 4:                          \
      numAntPorts = NhuAntennaInfoCmnantennaPortsCountan4Enum; \
      break;\
   default:                         \
      numAntPorts = NhuAntennaInfoCmnantennaPortsCountan2Enum; \
}

EXTERN S16  wrStartUeTmr(WrUeCb *cb, S16 tmrEvnt, U32 delay);
EXTERN Void wrStopUeTmr(WrUeCb *cb, S16 event);
EXTERN S16 wrUmmPrcErabRelIndctrl(void);
EXTERN S16 wrUmmGetOldUe(WrUeCb  **ueCb,WrCellId  cellId,U16  oldCrnti);
EXTERN S16 WrIfmX2apRelReq(CztRlsReq *relReq);
EXTERN S16 wrUmmMeasUeCbInit(WrUeCb *ueCb);
EXTERN S16 wrUmmPrcX2BmpInd(CztEvnt  *cztInd);
EXTERN S16 wrUmmPrcX2BmpCfm(CztEvnt  *cztCfm);
EXTERN S16 wrUmmPrcPdcpSduStaCfm(NhuPdcpSduStaCfm   *pdcpSduStaCfm);
EXTERN S16 wrUmmS1apFillCtxtRelReq(WrUeCb *ueCb, S1apPdu **pdu, WrUmmMsgCause *cause);
EXTERN S16 wrUmmX2TmrPrcStaInd(CztStaInd *cztStaInd);
EXTERN S16 wrEmmInitUeCb(WrUeCb *ueCb);
EXTERN Void wrUmmGetRand32(WrUeCb  *ueCb);

EXTERN S16 wrUmmEncCdmaPdu( WrUmmCellCb *cellCb, LwrCdma2k1xMobParams *mobParams);
#ifdef RGR_CQI_REPT
EXTERN S16 wrUmmSchdStaInd (RgrStaIndInfo *staInd);
#endif /*RGR_CQI_REPT */

EXTERN S16 wrUmmGetExistingTransId(U8 transTyp, CmLListCp *transLst, U32 *transId);

/* CSG_DEV start */
EXTERN S16 wrUmmCsgAnrUpdtEutranCsgInfo (WrUeCb *ueCb,
      NhuMeasResults *measRprt, U16 carrFreq);
EXTERN U8 wrUmmCsgIsUeMember (WrUeCb *ueCb, U32 csgId);
EXTERN S16 wrUmmCsgValidateNbrCsgForHo (WrUeCb *ueCb, WrHoNghInfo *nbrInfo,
      U8 *hoStatus, U8 *hoType, U8 *nbrPriority);
EXTERN S16 wrUmmCsgAddCsgIdIntoMbrLst (WrUeCb *ueCb, U32 csgId, U8 mbrShpSts);
EXTERN S16 wrUmmCsgModMbrStsForCsgId (WrUeCb *ueCb, U32 csgId, U8 mbrShpSts);
EXTERN S16 wrUmmCsgDelCsgIdFrmMbrLst (WrUeCb *ueCb, U32 csgId);
EXTERN S16 wrUmmCsgAddCsgIdIntoBlckLst (WrUeCb *ueCb, U32 csgId, U16 pci);
EXTERN S16 wrUmmCsgValidateInbndS1Ho (WrUeCb *ueCb, TknU32 csgId, U8 ueMbrShpSts,
      U8 isEmrgncyCall, U8 *causeType, U8 *causeVal);
EXTERN S16 wrUmmCsgValidateInbndX2Ho (WrUeCb *ueCb, U32 csgId, U8 ueMbrShpSts,
      U8 isEmrgncyCall, U8 *causeType, U8 *causeVal);
EXTERN S16 wrUmmCsgPrntUeMbrShpLst (WrUeCb *ueCb);
EXTERN Bool wrUmmCsgIsNbrBlsted (WrUeCb *ueCb, U32 csgId, U16 pci);
EXTERN S16 wrUmmHoIsEmgncyCall (WrUeSecCfg *secCfg);

/* CSG_DEV ends */

EXTERN S16 wrUmmCpyHoRestrnLst(WrUeCb  *ueCb, SztHovrRestrnLst *hoRestrnLst);
EXTERN S16 wrUmmCreateIntCsfb(WrUmmCellCb *cellCb, WrUeCb *ueCb, U8 intMsgType);
EXTERN S16 wrUmmChkUeB2UtraCap
(
 U16                         cellId, 
 U16                         crnti
);

EXTERN S16 wrGetCsdomainPLMNNeighFreqs
(
WrUeCb                    *ueCb,
WrPlmnId                  csdomainPlmn,
WrUmmRedirectInfo         **redirInfo
);
EXTERN Bool wrChkGeranSupp
(
 WrUeCapInfo *ueCap
);
EXTERN Bool wrValidateRegLai
(
 WrHoRstrctInfo *hoRstrctInfo,
 WrRegLai        lai
 );
EXTERN Bool wrChkForbiddenRAT
(  
 WrUeCb *ueCb
); 
EXTERN Bool wrChkGeranBandSup
(  
 WrUeCapInfo *ueCap,      
 U8    bandInd
);
EXTERN Bool wrChkCsdomainPlmnNeigh
(  
  WrPlmnId csdomainPlmn,    
   WrCellId cellId
);    
EXTERN Void wrFillRatLstRatType
(
 WrUeCapInfo   *ueCap,
 WrUmmRatLst   *ratLst,
 Bool          *handInUeCdma
);
EXTERN S16 wrUmmCsfbSndRedirAftReest
(
 WrUeCb      *ueCb
);
EXTERN Bool wrFindNghCellLaiPres
(
   WrUeCb          *ueCb,
   WrPlmnId        *plmnId,
   U16             lac,
   U16 				ratSelAlgo
);
EXTERN Bool wrUtlValNghCellforHrl
(
 WrUeCb *ueCb, 
 WrPlmnId *plmnId, 
 U16 lac
);

EXTERN Void wrFreeUeHrlMeasCfg
(
 WrUeCb  *ueCb
);

EXTERN S16 wrUmmPopltHrlMeasCfgDlNas
(
WrUeCb   *ueCb
);

EXTERN S16 wrUmmMeasReCfgAftDlNas
(
WrUeCb   *ueCb
);
EXTERN Bool wrUtlGetDedTaTmrVal 
(
U8    taTimerDedicated,
U16   *taTmr         
);
EXTERN Void wrUtlUpdateUeCatV1020
(
 WrUeCb *ueCb, 
 WrUeCapInfo *ueCap
); 
/* RRC-REL10-Upgrade */
EXTERN Bool wrUmmUtilUtraTddMeasChk
(
 WrUeCb                       *ueCb,
 WrUmmFgiType                 fgiTyp
 );
EXTERN Void wrUmmFilleRedirUtraSup
(
WrUeCapInfo         *ueCap,
WrUmmRedirectInfo   *redirInfo 
 );
EXTERN S16 wrUmmPopUtraRedirAndRel
(
 WrUeCb              *ueCb,
 WrUmmRedirectInfo   *redirInfo
 );

EXTERN S16 wrUmmChkOneUtraNeigh
(
 WrUeCb           *ueCb,
 U8               ratPriority
);
EXTERN S16 wrEmmChkCsfbToUtraMeasSup
(
WrCellId                    cellId, 
U8                          *csfbUtraMeasSup
);
/*RRM_ARP*/
EXTERN S16 wrUmmProcRrmUeRabRelInd
(
 RmuUeERabRelInd *relIndinfo
);

EXTERN S16 wrUmmProcRmUeRecfgInd
(
RmuUeRecfgInd         *ueRecfgIndPtr
);

EXTERN Void wrUmmPerdStrongCellMeasCfg
(
WrUeCb                 *ueCb,
U8                     measObjId,
LwrCellMeasCfgGrp      *measCellCfg,
WrUmmMeasAddInfo       *addMeasInfo
);

EXTERN Void wrUmmUeCtxRelGrdTmrExp
(
WrUmmCellCb *cellCb
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __WR_UMM_H__ */



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
