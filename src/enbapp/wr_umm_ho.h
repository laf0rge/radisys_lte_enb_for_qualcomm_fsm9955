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
               

     File:     wr_umm_ho.h

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
#ifndef __WR_UMM_HO_H__
#define __WR_UMM_HO_H__

#ifdef __cplusplus
extern "C" {
#endif

#if 0
typedef struct WrHoIntraFreqNeighCellinfo
{
   U16                 pci; /* Pci of intra frequency neighbour cell.*/
   Bool                isItBlackLstd;/* Indicates whether blacklisted or not.*/
}wrHoIntraFreqNeighCellInfo;

typedef struct WrHoInterFreqNeighCellinfo
{
   U16                 pci; /* Pci of intra frequency neighbour cell.*/
   U32                 arfcnOfEutra; /* ARFCN of EUTRA */
   Bool                isItBlackLstd; /* Indicates whether blacklisted or not.*/
}wrHoInterFreqNeighCellInfo;

typedef struct WrHoNeighInfo
{
   CmLListCp             wrIntraFreqNeighCellLst; /* list of Intra Frequency Neighbour Cell Info. */
   CmLListCp             wrInterFreqNeighCellLst; /* list of Inter Frequency Neighbour Cell Info. */
}wrHoNeighInfo;

#endif 

#define WR_HANDOVER_PREPARATION             11
#define WR_HANDOVER_PREPARATION_ACK_RCVD    12
#define WR_HANDOVER_EXECUTION               14
#define WR_HANDOVER_COMPLETION              13
#define WR_UMM_SRC_SCH_CFG_DRX_REL          0x0000000a
#define WR_UMM_SRC_DRX_DISABLED_RQST_SENT   9
#define WR_UMM_SRC_DRX_DISABLED             8

/**
    * Type of Handover.
    */
#define WR_UMM_S1_SRC_HANDOVER              1
#define WR_UMM_S1_TGT_HANDOVER              2
#define WR_UMM_X2_SRC_HANDOVER              3
/*#define WR_UMM_X2_TGT_HANDOVER              4*/
#define WR_UMM_X2_TGT_HANDOVER              15
#define WR_UMM_INTER_RAT_HANDOVER           5

/* ENB UE S1AP ID.*/
#define WR_UMM_OLD_ENB_UE_X2AP_ID           1
#define WR_UMM_NEW_ENB_UE_X2AP_ID           2

#define WR_HO_DEC_SUCCESS    0
#define WR_HO_DEC_FAILURE    1
#define WR_HO_DEC_REDIR      2

#define WR_UMM_TGT_HO_SCH_UECFG           0x0000000f

#define WR_UMM_TGT_HO_SCH_SRB1CFG         0x0000000a
#define WR_UMM_TGT_HO_SCH_SRB2CFG         0x00000009
#define WR_UMM_TGT_HO_DRX_CFG             0x00000008

#define WR_UMM_TGT_HO_PHY_CFG_SENT        0
#define WR_UMM_TGT_HO_PHY_CFG_SUCC        1
#define WR_UMM_TGT_HO_PHY_CFG_FAIL        2

#define WR_UMM_TGT_HO_RAB_SCH_CFG_SENT    0
#define WR_UMM_TGT_HO_RAB_SCH_CFG_SUCC    1
#define WR_UMM_TGT_HO_RAB_SCH_CFG_FAIL    2

#define WR_UMM_TGT_HO_TNL_CFG_SENT        0
#define WR_UMM_TGT_HO_TNL_CFG_SUCC        1
#define WR_UMM_TGT_HO_TNL_CFG_FAIL        2

#define WR_UMM_TGT_HO_PHY_CFG_DONE        1
#define WR_UMM_TGT_HO_SCH_CFG_DONE        2
#define WR_UMM_TGT_HO_TNL_CFG_DONE        3
/* ccpu00131426 - Fix */
#define WR_UMM_TGT_HO_DRX_CFG_SENT        4
#define WR_UMM_TGT_HO_DRX_CFG_DONE        5
/* ccpu0013533 : Adding new states to handle DAM MOD CFM
 * and SECURITY KEY  derivation parallelly */
#define WR_UMM_TGT_HO_PATH_SWITCH_ACK_RCVD        6
#define WR_UMM_TGT_HO_HZ_KEY_DER_DONE             7

#define WR_UMM_SRC_HO_TNL_CFG_SENT        0
#define WR_UMM_SRC_HO_TNL_CFG_SUCC        1
#define WR_UMM_SRC_HO_TNL_CFG_FAIL        2
#define WR_UMM_SRC_HO_TNL_CFG_DONE        3

#define WR_UMM_TGT_HO_RRC_CFG             1
#define WR_UMM_TGT_HO_RRC_CFG_DONE        2

#define WR_UMM_TGT_HO_SN_CFG              3
#define WR_UMM_TGT_HO_SN_CFG_DONE         4

#define WR_UMM_TGT_HO_DLDCCH_MSG_ENC      1
#define WR_UMM_TGT_HO_HOCMD_ENC           2
#define WR_UMM_TGT_HO_TGTTOSRCCONT_ENC    3
#define WR_UMM_TGT_HO_UECAP_ENC           4
#define WR_UMM_TGT_HO_RACAP_ENC           5

#define WR_UMM_SRC_HO_HOCMD_DEC           1
#define WR_UMM_SRC_HO_DLDCCH_MSG_ENC      2
#define WR_UMM_SRC_HO_HOPREP_ENC          3
#define WR_UMM_SRC_HO_UTRA_CONT_ENC       4 

#define WR_UMM_TGT_HO_CMD_ENC             1

/* SN Status Info flag */
#define WR_UMM_TGT_HO_SN_INFO_WAIT        1
#define WR_UMM_TGT_HO_SN_INFO_NOWAIT      2
#define WR_UMM_TGT_HO_SN_INFO_GOT         3

/* Inter-RAT Handover.*/
#define WR_CDMA_PERIODIC            1
#define WR_CDMA_B2                  2
#define WR_CDMA_RLS8_CSFB           2
#define WR_CDMA_eCSFB               3

/* PRACH Mask*/
#define WR_UMM_PRACH_MASK_IDX       0

#define WR_UMM_TGT_HO_CALC_NUM_TNL_CFG(_rabInfo) \
{\
   /* By default 1. For normal tunnel */\
   U8 _numTnls = 1;\
   if(_rabInfo->dlFwding == TRUE)\
   {\
      if(rabInfo->drbInfo.rbDir == WR_DIR_UL)\
      {\
         _numTnls++;\
      }\
      if(rabInfo->drbInfo.rbDir == WR_DIR_DL)\
      {\
         _numTnls++;\
      }\
      if(rabInfo->drbInfo.rbDir == WR_DIR_DL_UL)\
      {\
         _numTnls += 2;\
      }\
   }\
   _rabInfo->expectedTnlRsp += _numTnls;\
}

#define WR_UMM_SRC_HO_CALC_NUM_TNL_CFG(_rabInfo) \
{\
   /* By default 0. As normal tunnel already exist */\
   U8 _numTnls = 0;\
   if(rabInfo->rbDir == WR_DIR_UL)\
   {\
      _numTnls++;\
   }\
   if(rabInfo->rbDir == WR_DIR_DL)\
   {\
      _numTnls++;\
   }\
   if(rabInfo->rbDir == WR_DIR_DL_UL)\
   {\
      if(rabInfo->isULDatFwdReq == TRUE)\
      {\
         _numTnls += 2;\
      }\
      else\
      {\
         _numTnls += 1;\
      }\
   }\
   _rabInfo->expectedTnlRsp += _numTnls;\
}

/* CSG_DEV start */
#define WR_UMM_HO_MAX_NBR_CELLS_FOR_HO      3

typedef enum
{
   WR_UMM_HO_NBR_PRIORITY_HIGHEST = 1,
   WR_UMM_HO_NBR_PRIORITY_MORE,
   WR_UMM_HO_NBR_PRIORITY_LOW,
   WR_UMM_HO_NBR_PRIORITY_MAYBE,
   WR_UMM_HO_NBR_PRIORITY_MAX
} WrUmmHoNbrPriority;
/* CSG_DEV end */

#define WR_UMM_DECIDE_S1_OR_X2_HO_FRM_X2_AVLTY(nbrInfoTemp, hoStatus, hoType){\
   *hoStatus = WR_HO_DEC_SUCCESS;\
   if(WR_NEIGH_ENB_STATE_UP == nbrInfoTemp->t.intraLteNghInfo.isX2Connected)\
   {\
      *hoType = WR_HO_TYPE_X2;\
   }\
   else\
   {\
      *hoType = WR_HO_TYPE_INTRA_LTE_S1;\
   }\
}
/*CSG_DEV end */
/*
* S1 Based Handover
* X2 Based Handover
* Inter-RAT Handover
*/
typedef struct wrHoDecType
{
   U16  hoType;
}WrHoDecType;

/*
* Cause ... No Cell
*/

typedef struct wrHoDecFailCause
{
   U16  cause;
}WrHoDecFailCause;

/* RRC-REL10-Upgrade  start */
typedef struct wrUmmEutraHoTgtInfo
{
   Bool                    isRlfTmrConstR9Pres;
   Bool                    isMeasSubFrPatPCellR10Pres;
   Bool                    needToFillSCellToRlsLstR10;
   NhuSCellToRlsLst_r10    sourceSCellRelLst_r10;
} WrUmmEutraHoTgtInfo;
/* RRC-REL10-Upgrade End */ 
/* SPS changes starts */
typedef struct wrUmmHoSpsInfo
{
   Bool    isDlSpsPresent;
   Bool    isUlSpsPresent;
} WrUmmHoSpsInfo;
/* SPS changes ends */

/*
HANDOVER_FAIL
HANDOVER_SUCCESS
*/
typedef struct wrHoDecRslt
{
   U16 pci;     /* Physical Cell Identity */
   U8 status;   /* SUCCESS / FAILURE */
   union
   {
      WrHoDecFailCause   cause;
      WrHoDecType        typeOfHo; /* S1 Handover or X2 Handover or Inter-RAT Handover */
   }val;
}WrHoDecRslt;

EXTERN S16 wrUmmHoUpdUeHist (WrUeCb *ueCb);
EXTERN S16 wrUmmHoIsDatFwdRqrd (WrUeRAB *rbInfo, Bool *datFwdReq);
EXTERN S16 wrUtlFillMeasRptCfgMeasObjIDRmvLst(WrUeCb *ueCb,NhuEncReqSdus *dcchEncReq,
                  NhuMeasConfig *nhuMeasCfg);
EXTERN S16 wrUmmHoHdlMeasRprt (WrUeCb *ueCb, WrMeasRptCfgType event, NhuMeasResults *measRprt, U32 earfcn);
EXTERN S16 wrUmmHoCopyRadioResCfg(RmuUeRadioResCfg   *rmuUeRadioResCfg, RmuUeRadioResCfg   *txsUeRadioResCfga);
EXTERN S16 wrUmmHoFillRadioResCfg (RmuUeRadioResCfg   *ueRadioResCfg, NhuHovrPrepInfo    *nhuHovrPrepInfo);
/* ccpu00128207 */
EXTERN S16 wrUmmTgtHoGetLchIdDrbId (WrUeCb *ueCb, NhuDecodeSduCfm *decCfm,
      U32 eRABId, U8 *drbId, U8 *lchId,WrRabCfgParams *srcPdcpRlcCfgParams);
EXTERN S16 wrUmmHoTgtSndHoFail ( U32 spConnId, U32 mme_ue_s1ap_id,
                                                U32 cause_Type,U32 cause_Val);
EXTERN S16 wrUmmHoUeCapStoreCapInfo( WrUeCb *ueCb, NhuUECapInfo *ueCapInfo);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* end of __WR_UMM_HO_H__ */



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
