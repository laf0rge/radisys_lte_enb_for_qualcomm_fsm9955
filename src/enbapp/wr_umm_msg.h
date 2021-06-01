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
               

     File:     wr_umm_msg.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

#ifndef __WR_UMM_MSG_H__
#define __WR_UMM_MSG_H__

#include "wr_umm.h"

#ifdef __cplusplus
extern "C" {
#endif /* end of __cplusplus */

/* The following are to be used for the field msgTyp in WrUmmIncMsg       */
#define WR_INVALID_PDU                      0
#define WR_UMM_S1_DAT_IND                   1
#define WR_UMM_X2_BMP_PDU                   2
#define WR_UMM_UU_IND_PDU                   3
#define WR_UMM_UU_CFM_PDU                   4
#define WR_UMM_RRC_CFG                      5
#define WR_UMM_SCHD_CFG                     6
#define WR_UMM_PHY_CFG                      7
#define WR_UMM_RRC_ENC_CFM                  8
#define WR_UMM_RRC_DEC_CFM                  9
#define WR_UMM_S1_CON_CFM                   10
#define WR_UMM_RAB_RLS_EVNT                 11
#define WR_UMM_DAM_CFM                      12
#define WR_UMM_DAM_UE_DEL_CFM               13
#define WR_UMM_LCL_CTXT_REL                 14
#define WR_UMM_RRC_DAT_CFM                  15
#define WR_UMM_TMREXP_REESTABLISH           16
#define WR_UMM_TMREXP_RECONFIGURE           17
#define WR_UMM_S1_REL_IND                   18
#define WR_UMM_TMREXP_INACTIVITY            19
#define WR_UMM_RRC_ERR_IND                  20
#define WR_UMM_TMR_EXPIRY                   21
#define WR_UMM_PDCP_SN_STS_UPDATE_CFM       22
#define WR_UMM_PDCP_SDU_STATUS_CFM          23
#define WR_UMM_SND_HANDOVER_CANCEL          24
#define WR_UMM_S1AP_ENC_DEC_CFM             25
#define WR_UMM_TGTTOSRC_CONT_DEC_CFM        26
#define WR_UMM_S1AP_DEC_CFM                 27
#define WR_UMM_S1_CON_PDU                   28
#define WR_UMM_INTERNAL_MSG                 29
#define WR_TMR_TGTENB_RELOCEXE_EXP          30
#define WR_UMM_CSFB_TMR_EXP                 31
#define WR_UMM_SRCTOTGT_CONT_DEC_CFM        32
#define WR_UMM_RRM_UEADMIT_RESP             33
#define WR_UMM_RRM_UERECFG_RESP             34
#define WR_UMM_RRM_UEREL_RESP               35
#ifdef TENB_AS_SECURITY
#define WR_UMM_PHY_KDF                      36
#endif
#define WR_UMM_X2AP_ERR_IND                 37
/* ccpu00126336 */
#define WR_UMM_DAM_MOD_CFM                  38
#define WR_UMM_RRM_UEHO_RESP                39
#define WR_UMM_X2_STA_IND                   40
#define WR_UMM_S1AP_ERR_IND                 41
#define WR_UMM_PDCP_DATA_RESUME_CFM         42
#define WR_UMM_S1AP_ENC_DEC_FAIL            43 
#define WR_UMM_RRM_UE_ERAB_REL_IND          44 /*RRM_ARP*/
#define WR_UMM_RRM_UERECFG_IND               45
/* sri - HOFIX - unused */
//#define WR_UMM_RAD_RES_CFG_MOD_MSG          30

/* Internal Message Type.*/
#define WR_UMM_INTMSG_CR_HO_TRANS           1
#define WR_UMM_INTMSG_MEAS_TRANS            2
#define WR_UMM_INTMSG_NEW_MEAS_TRANS        3
#define WR_UMM_INTMSG_CSFB_REDIR_TRANS      4
#define WR_UMM_INTMSG_RAD_RES_MOD_MSG       5
#define WR_UMM_INTMSG_ANR_MEAS_TRANS        6
#define WR_UMM_INTMSG_MOD_SR_CQI_RES_MSG    7
/* LTE_ADV starts */
#define WR_UMM_INTMSG_LTE_ADV_TRANS         8
/* LTE_ADV ends */
/* eCSFB - Added for ecsfb */
#define WR_UMM_INTMSG_ECSFB_TRANS           9
#define WR_UMM_INTMSG_CSFB_ECSFB_TRANS      10
#define WR_UMM_INTMSG_EVNT_ANR_MEAS_TRANS   11

/* SPS changes starts */
#define WR_UMM_INTMSG_UNSET_SR_MASK         12
/* SPS changes ends */
/*GERAN */
#define WR_UMM_INTMSG_CSFB_CCO_TRANS      13
/* Redirection to TDSCDMA */
#define WR_UMM_INTMSG_REDIR_TRANS         14

#define WR_UMM_ANR_MEAS_CREATE              1
#define WR_UMM_ANR_MEAS_DELETE              2

/* IRAT_HO */
typedef struct wrUmmIntCrIntraLteHoTranInfo
{
   WrEcgi                   ecgi;
   U8                       enbType;            /* Macro or Home eNB.*/
   U16                      tac;               /* Tracking Area code*/
   U16                      pci;               /* Physical cell ID */
   U32                      earfcnDl;         /* EARFCN DL */ 
   Bool                     isX2InterFace; /* X2 Interface to be set or not */
} WrUmmIntCrIntraLteHoTranInfo;
typedef struct wrUmmIntCrIRatUtraHoTranInfo
{
  U32                           arfcnDl;
  U16                           psc;
  U16                           rncId;
  U16                           cId;
  U16                           lac;
  WrPlmnId                      plmnId;
  U8                            rac;
  TknU32                        srvccHoInd;
} WrUmmIntCrIRatUtraHoTranInfo;

typedef struct wrUmmIntCrIRatGeranHoTranInfo
{
  Bool          psSrvcNotAvlbl;
  U8            bsic;
  U16           cId;
  U16           lac;
  U32           rssi;
  WrPlmnId      plmnId;
  U8            rac;
  U8            bandIndicator;/* band indicator for mobilityfromUtraCmd*/
  TknU32        srvccHoInd;
} WrUmmIntCrIRatGeranHoTranInfo;
typedef struct wrUmmIntCrHoTran
{
    U16                      crnti;             /* Ue Id.*/
    U32                      cellId;            /* Target Cell Id.*/
    U32                      peerId;            /* Neighbour Id.*/
    U8                       hoType;            /* Type of Handover.*/
    WrUmmMsgCause            hoCause;           /* cause choices are RadioNw*/
    Bool                    isX2InterFace;     /* X2 Interface to be set or not */
    /* CSG_DEV Start */
    U32                      csgId;             /* CSG Identity */
    U8                       csgCellAccessMode; /* CSG Access Mode */
    /* CSG_DEV Ends */
    union 
    {
       WrUmmIntCrIntraLteHoTranInfo  intraLte;
       WrUmmIntCrIRatUtraHoTranInfo  interRatUtra;
       WrUmmIntCrIRatGeranHoTranInfo  interRatGeran;
    }t;
}WrUmmIntCrHoTran;

typedef struct wrUmmDamCfgCfm
{
   U32                       lclTeId;
   U32                       transId;
   U8                        status;
} WrUmmDamCfgCfm;

/** @brief This structure used to store CSFB related information in internal
 *  * message.
 *
 *   - TknU32         csFallBackInd     CS fallback indicator
 *   - WrRegLai       regLai            Ue registered Location area identifier
 *     */
typedef struct wrUmmIntCsfbTransMsg
{
   TknU32                    csFallBackInd;
   U8                        ratPriority;
   WrRegLai                  regLai;
} WrUmmIntCsfbTransMsg;

typedef struct wrUmmIntAnrMeasMsg
{
   U8                        choice;
   WrMeasObjFreq             arfcn;
   WrUmmMeasCellTyp          cellTyp;
   WrMeasRptCfgType          rptCfgType;
   U16                       newPci;
   Bool                      rrcReCfgRqd;
} WrUmmIntAnrMeasMsg;

/* LTE_ADV starts */
typedef enum _wrUmmLteAdvProc
{
   WR_UMM_LTE_ADV_RE_PROC  = 1,
   WR_UMM_LTE_ADV_ABS_PROC,
   WR_UMM_LTE_ADV_SFR_PROC,
   WR_UMM_LTE_ADV_DSFR_PROC
} WrUmmLteAdvProc;

typedef enum  wrUmmLteAdvPeerType
{
   WR_UMM_LTE_ADV_LCL_UE   = 1,
   WR_UMM_LTE_ADV_PEER_UE,
} WrUmmLteAdvPeerType;

typedef struct wrUmmIntLteAdvMsg
{
   Bool                    isEnabled;
   WrUmmLteAdvProc         procType;
   WrUmmLteAdvPeerType     peerType;
} WrUmmIntLteAdvMsg;
/* LTE_ADV ends */

typedef struct wrUmmRabRls
{
  U8                        numrabs; 
  U8                        erabId[WR_MAX_DRBS];
  U8                        cause[WR_MAX_DRBS];/* RRM_ARP*/
  U8                        causeTyp;
} WrUmmRabRls;

typedef struct wrUmmPhyCfgCfm
{
   S16                       status;
   U32                       transId;
} WrUmmPhyCfgCfm;

#ifdef TENB_AS_SECURITY
typedef struct wrUmmKdfCfm
{
   S16                       status;
   U32                       transId;
   CtfKdfCfmInfo             *ctfKdfCfm; 
} WrUmmKdfCfm;
#endif

typedef struct wrUmmSchdCfgCfm
{
   S16                       status;
   U32                       transId;
} WrUmmSchdCfgCfm;

typedef struct wrUmmDamUeDelCfm
{
   S16                       status;
   U32                       transId;
   U16                       cellId;
   U16                       crnti;
} WrUmmDamUeDelCfm;

#define WR_UMM_RAD_RES_MOD_ULPOWER           0x01
#define WR_UMM_RAD_RES_MOD_CQI_SR            0x02
#define WR_UMM_RAD_RES_MOD_UU_IND_RCVD       0x04
#define WR_UMM_RAD_RES_MOD_RRC_DAT_CFM_RCVD  0x08
/* SPS changes starts */
#define WR_UMM_RAD_RES_UNSET_SR_MASK         0x10
/* SPS changes ends */
typedef struct wrUmmRadResMod
{   
   U16                       bitMsk;
}WrUmmRadResMod;

typedef struct wrUmmIntUeStaInd
{   
   CmLteCellId       cellId;       /*!< Cell ID */
   CmLteRnti         crnti;        /*!< UE identifier UE ID: CRNTI */
   U8                status;       /*!< Status */
}WrUmmIntUeStaInd;


typedef struct wrUmmInternalMsg
{
   U8                        intMsgType;
   union
   {
      WrUmmIntMeasTrans       *measTrans;
      WrMeasCellReCfgInfo     *measNewTrans;
      WrUmmIntCrHoTran        *hoTrans;
      WrUmmIntCsfbTransMsg    *csfbIndMsg;
      WrUmmRadResMod          radResMod;
      WrUmmIntAnrMeasMsg      *anrMeasMsg;
/* LTE_ADV starts */
      WrUmmIntLteAdvMsg       *lteAdvMsg;
/* LTE_ADV ends */
      WrUmmRedirectInfo       *redirInfo;
   }u;
}WrUmmInternalMsg;
          
typedef struct wrUmmTmrExpiry
{
   U16                       timerEvnt;
   U32                       transId;
} WrUmmTmrExpiry;

typedef struct wrUmmIncMsg
{
   U8                        msgTyp;
   U8                        refCnt;
   union
   {
      SztConCfm              *cfmPdu;
      SztDatEvntInd          *s1Pdu;
      CztEvnt                *x2Pdu;
      NhuDatIndSdus          *uuIndPdu;
      NhuDatCfmSdus          *uuCfmPdu;
      NhuCfgCfmSdus          *rrcCfm;
      WrUmmSchdCfgCfm        schdCfm;
      NhuPdcpSduStaCfm       *pdcpSduCfm;
      WrUmmPhyCfgCfm         phyCfm;
      NhuEncCfmSdus          *encCfm;
      NhuDecodeSduCfm        *decCfm;
      WrUmmRabRls            *lcrabrls;
      WrUmmDamCfgCfm         damCfm;
      WrUmmDamUeDelCfm       damUeDelCfm;
      U32                    transId;
      U32                    rrcConRelCause; 
      NhuDatCfmSdus          *rrcDatCfm;
      SztRelInd              *relInd;
      NhuErrIndSdus          *rrcErrInd;
      /* ccpu00149336 */
      SztLclErrInd           *sztErrInd;
      /* ccpu00149336 */
      WrUmmInternalMsg       *internalMsg;
      WrUmmTmrExpiry         tmrExpiry;
      SztEncDecEvnt          *sztEncCfm;
      SztEncDecEvnt          *sztDecCfm;
      CztLclErrInd           *cztLclErrInd;
      RmuUeAdmitRsp          *rrmUeAdmitResp;
      RmuUeHoRsp             *rrmUeHoResp;
      RmuUeRelRsp            *rrmUeRelResp;
      RmuUeRecfgRsp          *rrmUeRecfgResp;
      RmuUeERabRelInd        *rmuUeErabRelInd; /*RRM_ARP*/
      RmuUeRecfgInd          *rrmUeRecfgInd;
      CztStaInd              *cztStaInd;
      NhuPdcpDatResumeCfm    *pdcpDatResumeCfm;
#ifdef TENB_AS_SECURITY 
      WrUmmKdfCfm           kdfCfm; 
#endif
   } u;
} WrUmmIncMsg;

EXTERN S16 wrUmmPrcIncMsg(WrUmmCellCb *cellCb,WrUeCb *ueCb,WrUmmIncMsg *msg);
EXTERN Void wrUmmRlsIncMsg(WrUmmIncMsg **msg);
EXTERN S16 wrUmmBldInternalMsg( WrUmmInternalMsg **intMsg, U8 intMsgType,
      Void *msg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* End of __WR_UMM_MSG_H__ */

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
