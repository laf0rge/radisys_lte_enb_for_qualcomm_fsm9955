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
               

     File:     wr_umm_ho.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=58;

static int RLOG_MODULE_ID=32;

#include "wr.h"        /* Common Includes */
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_cmn.h"
#include "wr_umm_msg.h"
#include "wr_umm_ho.h"
#include "wr_utils.h"
#include "wr_ifm_rrc.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_meas_fun.h"
#include <math.h>
#include "wr_kpi.h"
#include "wr_umm_drx.h"
#ifdef MME_LDBAL
#include "wr_smm_init.h"
#endif
#include "wr_ifm_dam.h"/*ccpu00138576*/
/* SPS changes starts */
EXTERN S16 wrUmmUecapEutraIeDecCfmHdl(WrUeCb *ueCb, WrUeCapInfo  *ueCap,NhuDecodeSduCfm  *nhuDecSduCfm);

/*Added for the release 9A0 extention in the UE Capability */
EXTERN S16 wrUmmUecapEutraIe9a0DecCfmHdl(WrUeCapInfo  *ueCap,NhuDecodeSduCfm   *nhuDecSduCfm);
/* SPS changes ends */
EXTERN S16 wrUmmHOFillReestReCfg(WrUeCb *ueCb, NhuDatReqSdus *recfgMsg);
#ifdef MME_LDBAL
EXTERN SmCfgCb   smCfgCb;
#endif
EXTERN S16 wrEmmValidateNghAvailability
(
WrUeCb                *ueCb,
WrHoNghInfo           *hoNghInfo,
U8                    *hoStatus,
U8                    *hoType
);
/* HO_REEST */
PRIVATE S16 wrUmmFillMeasCfg(WrUmmTransCb *transCb, NhuDatReqSdus *recfgMsg)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   NhuMeasConfig             *nhuMeasCfg;
   NhuMeasIdToAddMod         *measIdAddModLst;
   NhuMeasObjectToAddMod     *measObjIdAddModLst;
   NhuReportConfigToAddMod   *measRprtIdAddModLst;
   U32                       measIdAddModCnt;
   U32                       measRprtIdAddModCnt;
   U32                       measObjIdAddModCnt;
   U8                        idx;
   U8                        idx2;
   U8                        measIdIdx = 0;
   Bool                      measIdPres;
   U8                        measIdRemCnt = 0;
   NhuDL_DCCH_Msg           *dcchMsg;
   NhuRRCConRecfgn          *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs    *rrcConRecfgn_r8 = NULLP;
   WrUmmMeasTransInfo        *measTransCb;
   WrUmmCellCb               *cellCb;
   U16                        cellId;
   U8                         hoType;
   U8                        objIdRmvLst[WR_UMM_MAX_MEASOBJ_ID] = {0};
   /*Filling Measurement configuration */
   dcchMsg     = &(recfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   rrcConRecfgn_r8 = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   nhuMeasCfg  = &(rrcConRecfgn_r8->measConfig);
  
   if ( WR_UMM_X2_TGT_HO_TRANS == transCb->transTyp ) 
   {
     hoType = WR_HO_TYPE_X2;
   }
   else if ( WR_UMM_S1_TGT_HO_TRANS == transCb->transTyp ) 
   {
     hoType = WR_HO_TYPE_INTRA_LTE_S1;
   }
   else
   {
     ALARM("Invalid HO Trans\n");
     RETVALUE(RFAILED);
   }
   if ( WR_HO_TYPE_X2 == hoType ) 
   {
      measTransCb = &transCb->u.x2TgtHoTrans.measTransCb;
   }
   else
   {
      measTransCb = &transCb->u.s1TgtHoTrans.measTransCb;
   }
   
   cellId = transCb->ueCb->cellId;   
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId,"Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   wrMeasFillNhuMsg (&recfgMsg->memCp, nhuMeasCfg, ueCb,
                      measTransCb, cellCb->measCellCfg,
                      transCb->transId);

   /* update the measIdRemLst - remove the extra measId's that are not configured */
   measIdAddModCnt = nhuMeasCfg->measIdToAddModLst.noComp.val;
   measIdAddModLst = nhuMeasCfg->measIdToAddModLst.member;

   /* calculate the number of measId's to be removed */
   for(idx = 0; idx < ueCb->hoSrcMeasIdLstCnt; idx++)
   {
      measIdPres = FALSE;
      /* find out if the measId is configured in the system */
      for(idx2 = 0; idx2 < measIdAddModCnt; idx2++)
      {
         if(ueCb->hoSrcMeasIdLst[idx] == measIdAddModLst[idx2].measId.val)
         {
            measIdPres = TRUE;
            break;
         }
      }
      if(measIdPres == FALSE)
         measIdRemCnt++;
   }

   if(measIdRemCnt > 0)
   {
      wrFillTknU16(&nhuMeasCfg->measIdToRemoveLst.noComp, measIdRemCnt);
      WR_GET_MEM((&recfgMsg->memCp), (measIdRemCnt * sizeof(NhuMeasId)),
            (&nhuMeasCfg->measIdToRemoveLst.member));
      for(idx = 0; idx < ueCb->hoSrcMeasIdLstCnt; idx++)
      {
         measIdPres = FALSE;
         /* find out if the measId is configured in the system */
         for(idx2 = 0; idx2 < measIdAddModCnt; idx2++)
         {
            if(ueCb->hoSrcMeasIdLst[idx] == measIdAddModLst[idx2].measId.val)
            {
               measIdPres = TRUE;
               break;
            }
         }
         if(measIdPres == FALSE)
         {
            wrFillTknU32(&nhuMeasCfg->measIdToRemoveLst.member[measIdIdx++],
                  ueCb->hoSrcMeasIdLst[idx]);
         }
      }
   }
   measIdRemCnt = 0;
   measIdIdx = 0;

   /* update the measIdRemLst - remove the extra measId's that are not configured */
   measRprtIdAddModCnt = nhuMeasCfg->reportConfigToAddModLst.noComp.val;
   measRprtIdAddModLst = nhuMeasCfg->reportConfigToAddModLst.member;

   /* calculate the number of rprtId's to be removed */
   for(idx = 0; idx < ueCb->hoSrcRprtIdLstCnt; idx++)
   {
      measIdPres = FALSE;
      /* find out if the measId is configured in the system */
      for(idx2 = 0; idx2 < measRprtIdAddModCnt; idx2++)
      {
         if(ueCb->hoSrcRprtIdLst[idx] == measRprtIdAddModLst[idx2].reportConfigId.val)
         {
            measIdPres = TRUE;
            break;
         }
      }
      if(measIdPres == FALSE)
         measIdRemCnt++;
   }

   if(measIdRemCnt > 0)
   {
      wrFillTknU16(&nhuMeasCfg->reportConfigToRemoveLst.noComp, measIdRemCnt);
      WR_GET_MEM((&recfgMsg->memCp),
            (measIdRemCnt * sizeof(NhuReportConfigId)),
            (&nhuMeasCfg->reportConfigToRemoveLst.member));
      for(idx = 0; idx < ueCb->hoSrcRprtIdLstCnt; idx++)
      {
         measIdPres = FALSE;
         /* find out if the measId is configured in the system */
         for(idx2 = 0; idx2 < measRprtIdAddModCnt; idx2++)
         {
            if(ueCb->hoSrcRprtIdLst[idx] == measRprtIdAddModLst[idx2].reportConfigId.val)
            {
               measIdPres = TRUE;
               break;
            }
         }
         if(measIdPres == FALSE)
         {
            wrFillTknU32(&nhuMeasCfg->reportConfigToRemoveLst.member[measIdIdx++],
                  ueCb->hoSrcRprtIdLst[idx]);
         }
      }
   }
   /* update the measObjIdRemLst- remove the extra measObjId's that are not configured */
   measObjIdAddModCnt = nhuMeasCfg->measObjectToAddModLst.noComp.val;
   measObjIdAddModLst = nhuMeasCfg->measObjectToAddModLst.member;
  /* calculate the number of measObjId's to be removed */
   measIdRemCnt = 0;
   for(idx = 0; idx < ueCb->hoSrcMeasObjIdLstCnt; idx++)
   {
      measIdPres = FALSE;
      /* fin out if the measObjId is configured in the system */
      for(idx2 = 0; idx2 < measObjIdAddModCnt; idx2++)
      {
         if(ueCb->hoSrcMeasObjIdLst[idx] == measObjIdAddModLst[idx2].measObjectId.val)
         {
            measIdPres = TRUE;
            break;
         }
      }
      if(measIdPres == FALSE)
      {
         objIdRmvLst[measIdRemCnt] = ueCb->hoSrcMeasObjIdLst[idx];
         measIdRemCnt++;
      }
   }

   if(measIdRemCnt > 0)
   {
      wrFillTknU16(&nhuMeasCfg->measObjectToRemoveLst.noComp, measIdRemCnt);
      WR_GET_MEM((&recfgMsg->memCp), (measIdRemCnt * sizeof(NhuMeasObjectId)),
            (&nhuMeasCfg->measObjectToRemoveLst.member));

      /* calculate the number of measObjId's to be removed */
      for(idx = 0; idx < measIdRemCnt; idx++)
      {
         wrFillTknU32(&nhuMeasCfg->measObjectToRemoveLst.member[idx],
               objIdRmvLst[idx]);
      }
   }

   /* RRC-REL10-Upgrade start*/
   /* Check if isFullCfg in UMM UE control block flag is set to TRUE.
    * If yes, then fill fullConfig-r9 IE to TRUE and 
    * set isFullCfg flag to FALSE.*/
   if(ueCb->isFullCfgReqd)
   {
      rrcConRecfgn_r8->nonCriticalExtn.pres.pres = PRSNT_NODEF;
      rrcConRecfgn_r8->nonCriticalExtn.nonCriticalExtn.pres.pres = PRSNT_NODEF;
      /* When fullConfig-r9 IE is included in RRC Reconfiguration, eNodeB fills
       * required RadioResourceConfigDedicated in the same RRC Reconfiguration message.*/
      wrFillTknU32(&(rrcConRecfgn_r8->nonCriticalExtn.nonCriticalExtn.fullConfig_r9),
            NhuRRCConRecfgn_v920_IEsfullConfig_r9trueEnum);
      /* fullConfig-r9 IE is sent to UE only in first Reconfiguration message 
       * after RRC Reestablishment.*/
      /* increment - KPI_ID_LTE_RRC_CONNREESTAB_FULL_RECFG  kpi here */
      WR_INC_KPI_FUNC(wrIncKpiFullReCfgReestb);
   }
   else
   {
      rrcConRecfgn_r8->nonCriticalExtn.pres.pres = NOTPRSNT;
   }
   /* RRC-REL10-Upgrade end */

   RETVALUE(ROK);
}

PUBLIC S16 wrUmmHOFillReestReCfg( WrUeCb *ueCb, NhuDatReqSdus *recfgMsg)
{
   WrUmmTransCb  *hoTransCb = NULLP;
   if ( !ueCb->hoTransId )
   {
     RLOG0(L_ERROR, "ueCb->hoTransId = 0!!!");
     RETVALUE(RFAILED);
   }
   hoTransCb = wrUmmGetHoTransCb(ueCb);
   if ( hoTransCb == NULLP )
   {
     RLOG0(L_ERROR, "hoTransCb = NULLP!!!");
     RETVALUE(RFAILED);
   }
   wrUmmFillMeasCfg(hoTransCb, recfgMsg);
   RETVALUE(ROK);
}

PUBLIC S16 wrUmmHoIsDatFwdRqrd
(
WrUeRAB                      *rbInfo,
Bool                         *datFwdReq
)
{
   *datFwdReq = TRUE;
   RETVALUE(ROK);
}

/**
 * @brief This function fills the MeasId,RpCfgId and MeasObjID in Remove list
 *
 * @details
 *
 *     Function: wrUtlFillMeasRptCfgMeasObjIDRmvLst
 *     Purpose:  This function is used to fill measID remove list in HO Command .
 *
 *  @param[in] WrUeCb              *ueCb,
 *  @param[in] NhuEncReqSdus       *dcchEncReq
 *  @param[in] NhuMeasConfig       *nhuMeasCfg
 *
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/
PUBLIC S16 wrUtlFillMeasRptCfgMeasObjIDRmvLst
(
 WrUeCb                       *ueCb,
 NhuEncReqSdus                *dcchEncReq,
 NhuMeasConfig                *nhuMeasCfg
 )
{
   U32                       measIdAddModCnt;
   U32                       measRprtIdAddModCnt;
   U32                       measObjIdAddModCnt;
   NhuMeasIdToAddMod         *measIdAddModLst;
   NhuMeasObjectToAddMod     *measObjIdAddModLst;
   NhuReportConfigToAddMod   *measRprtIdAddModLst;
   U8                        idx;
   U8                        idx2;
   Bool                      measIdPres;
   U8                        measIdRemCnt = 0;
  /* ccpu00131426 - Fix: DRX Configuration for Hand in UE */
/* Removed DRX flag */   
   U8                        objIdRmvLst[WR_UMM_MAX_MEAS_ID] = {0};

   measIdAddModCnt = nhuMeasCfg->measIdToAddModLst.noComp.val;
   measIdAddModLst = nhuMeasCfg->measIdToAddModLst.member;
   /* calculate the number of
    * measId's to be removed */
   for(idx = 0; idx < ueCb->hoSrcMeasIdLstCnt; idx++)
   {
      measIdPres = FALSE;
      /* find out if the measId is configured in the system */
      for(idx2 = 0; idx2 < measIdAddModCnt; idx2++)
      {
         if(ueCb->hoSrcMeasIdLst[idx] == measIdAddModLst[idx2].measId.val)
         {
            measIdPres = TRUE;
            break;
         }
      }
      if(measIdPres == FALSE)
      {
         objIdRmvLst[measIdRemCnt] = ueCb->hoSrcMeasIdLst[idx];
         measIdRemCnt++;
      }
   }

   if(measIdRemCnt > 0)
   {
      wrFillTknU16(&nhuMeasCfg->measIdToRemoveLst.noComp, measIdRemCnt);
      WR_GET_MEM((&dcchEncReq->memCp), (measIdRemCnt * sizeof(NhuMeasId)),
            (&nhuMeasCfg->measIdToRemoveLst.member));
      for(idx = 0; idx < measIdRemCnt; idx++)
      {
         wrFillTknU32(&nhuMeasCfg->measIdToRemoveLst.member[idx],
               objIdRmvLst[idx]);
      }
   }

   measIdRemCnt = 0;

   /* update the measIdRemLst - remove the extra measId's that are not configured */
   measRprtIdAddModCnt = nhuMeasCfg->reportConfigToAddModLst.noComp.val;
   measRprtIdAddModLst = nhuMeasCfg->reportConfigToAddModLst.member;

   /* calculate the number of rprtId's to be removed */
   for(idx = 0; idx < ueCb->hoSrcRprtIdLstCnt; idx++)
   {
      measIdPres = FALSE;
      /* find out if the measId is configured in the system */
      for(idx2 = 0; idx2 < measRprtIdAddModCnt; idx2++)
      {
         if(ueCb->hoSrcRprtIdLst[idx] == measRprtIdAddModLst[idx2].reportConfigId.val)
         {
            measIdPres = TRUE;
            break;
         }
      }
      if(measIdPres == FALSE)
      {
         objIdRmvLst[measIdRemCnt] = ueCb->hoSrcRprtIdLst[idx];
         measIdRemCnt++;
      }
   }

   if(measIdRemCnt > 0)
   {
      wrFillTknU16(&nhuMeasCfg->reportConfigToRemoveLst.noComp, measIdRemCnt);
      WR_GET_MEM((&dcchEncReq->memCp),
            (measIdRemCnt * sizeof(NhuReportConfigId)),
            (&nhuMeasCfg->reportConfigToRemoveLst.member));
      for(idx = 0; idx < measIdRemCnt; idx++)
      {
         wrFillTknU32(&nhuMeasCfg->reportConfigToRemoveLst.member[idx],
               objIdRmvLst[idx]);
      }
   }

   measIdRemCnt = 0;

   /* update the measObjIdRemLst- remove the extra measObjId's that are not configured */
   measObjIdAddModCnt = nhuMeasCfg->measObjectToAddModLst.noComp.val;
   measObjIdAddModLst = nhuMeasCfg->measObjectToAddModLst.member;

   /* calculate the number of measObjId's to be removed */
   for(idx = 0; idx < ueCb->hoSrcMeasObjIdLstCnt; idx++)
   {
      measIdPres = FALSE;
      /* fin out if the measObjId is configured in the system */
      for(idx2 = 0; idx2 < measObjIdAddModCnt; idx2++)
      {
         if(ueCb->hoSrcMeasObjIdLst[idx] == measObjIdAddModLst[idx2].measObjectId.val)
         {
            measIdPres = TRUE;
            break;
         }
      }
      if(measIdPres == FALSE)
      {
         objIdRmvLst[measIdRemCnt] = ueCb->hoSrcMeasObjIdLst[idx];
         measIdRemCnt++;
      }
   }

   if(measIdRemCnt > 0)
   {
      wrFillTknU16(&nhuMeasCfg->measObjectToRemoveLst.noComp, measIdRemCnt);
      WR_GET_MEM((&dcchEncReq->memCp), (measIdRemCnt * sizeof(NhuMeasObjectId)),
            (&nhuMeasCfg->measObjectToRemoveLst.member));

      /* calculate the number of measObjId's to be removed */
      for(idx = 0; idx < measIdRemCnt; idx++)
      {
         wrFillTknU32(&nhuMeasCfg->measObjectToRemoveLst.member[idx],
               objIdRmvLst[idx]);
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief This function is used to fill the ASN DCCH message.
 *
 * @details
 *
 *     Function: wrUtlFillHoCmdTrnspCntrWthMobility
 *     Purpose:  This function is used to fill the ASN DCCH message.
 *
 *  @param[in]  ueCb             - Pointer the UE control block
 *  @param[out] dcchEncReq       - Pointer to the DCCH encoding parameters to be filled.
 *  @param[in]  measTransCb      - Measurement transaction CB
 *  @param[out] rrmUeHoResp      - RRM HO Response
 *  @param[in]  srcToTgtSpsInfo  - Contains if UL or DL SPS was enabled in SRC to TGT container
 *  @param[in]  pSpsCfg          - SPS configuration from transaction CB
 *  @param[in]  isUeUlSpsEnabled - If SPS is configured for the UE. This is to decide if SR 
 *                                 mask has to be enabled for the Release 9 UE.
 *
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/
PUBLIC S16 wrUtlFillHoCmdTrnspCntrWthMobility
(
WrUmmTransCb                 *transCb,
NhuEncReqSdus                *dcchEncReq,
WrUmmMeasTransInfo           *measTransCb,
/* SPS changes starts */
RmuUeHoRsp                   *rrmUeHoResp,
WrUmmHoSpsInfo               *srcToTgtSpsInfo,
WrSpsCfg                     *pSpsCfg,
Bool                         isUeUlSpsEnabled
/* SPS changes ends */
)
{
   U32                       rrc_TransId = 0;
   U16                       len = 0;
   WrUeCb                    *ueCb = transCb->ueCb;
   NhuRRCConRecfgn           *rrcConRecfgn;
   NhuRRCConRecfgn_r8_IEs    *rrcConRecfgn_r8 = NULLP;
   NhuDL_DCCH_MsgTyp         *message = NULLP;
   NhuMobilityCntrlInfo      *mobCntrlInfo;
   NhuSecConfigHO            *secConfigHO;
   NhuRACH_ConfigCmn         *rachConfigCommon = NULLP;
   NhuPRACH_Config           *prachConfig = NULLP;
   NhuPUSCH_ConfigCmn        *puschConfigCommon = NULLP;
   NhuPDSCH_ConfigCmn        *pdschCfgCmn;
   NhuPHICH_Config           *phichCfg;
   NhuPUCCH_ConfigCmn        *pucchCfgCmn;
   NhuSoundingRS_UL_ConfigCmn *srsUlCfgCmn;
   NhuUlnkPowerCntrlCmn      *ulPwrCntrlCmn;
   NhuAntennaInfoCmn         *antInfoCmn;
   NhuP_Max                  *pMax;
   NhuTDD_Config             *tddCfg;
   U16                       pci;
   WrCellCb                  *cellCb;
   NhuMeasConfig             *nhuMeasCfg;
   NhuRadioResourceConfigDedicated *rrcCfgDed = NULLP;
   NhuSecAlgorithmConfig     *secCfg;
   WrUmmEutraHoTgtInfo        *tmpEutraHoTgtInfo;
  /* ccpu00131426 - Fix: DRX Configuration for Hand in UE */
/* Removed DRX flag */   
   NhuDRX_Config        *drxCfgIe = NULLP;
   /* SPS changes starts */
   U8 bRadioResDedBitMap;
   /* SPS changes ends */
   U8 numAntPorts = 0;

   WR_GET_CELLCB(cellCb, ueCb->cellId);
   if (cellCb == NULLP)
   {
      RETVALUE(RFAILED);
   }

   pci = wrEmmGetPhyCellId(ueCb->cellId);

   dcchEncReq->sdu.msgCategory = NHU_MSG_DLDCCH;
   wrFillTknU8(&(dcchEncReq->sdu.u.dcchMsg.pres), PRSNT_NODEF);
   message = &dcchEncReq->sdu.u.dcchMsg.message;
   wrFillTknU8(&(message->choice),DL_DCCH_MSGTYP_C1);
   wrFillTknU8(&(message->val.c1.choice),C1_RRCCONRECFGN);

   rrcConRecfgn = &(message->val.c1.val.rrcConRecfgn);
   wrFillTknU8(&(rrcConRecfgn->pres),PRSNT_NODEF);
   wrFillTknU32(&(rrcConRecfgn->rrc_TrnsxnId),rrc_TransId);
   wrFillTknU8(&(rrcConRecfgn->criticalExtns.choice), CRITICALEXTNS_C1);
   wrFillTknU8(&(rrcConRecfgn->criticalExtns.val.c1.choice),
         C1_RRCCONRECFGN_R8);

   rrcConRecfgn_r8 = &(rrcConRecfgn->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   rrcCfgDed = &(rrcConRecfgn_r8->radioResourceConfigDedicated);
   wrFillTknU8(&(rrcConRecfgn_r8->pres),PRSNT_NODEF);
   /* Fill Mobility Control Information for handover.*/
   mobCntrlInfo = &(rrcConRecfgn_r8->mobilityCntrlInfo);
   wrFillTknU8(&(mobCntrlInfo->pres),PRSNT_NODEF);
   /* Fill Physical Cell Id of Target Cell*/
   wrFillTknU32(&(mobCntrlInfo->targetPhysCellId), pci);
   /* Fill Carrier Frequency of eNB*/
   wrFillTknU8(&(mobCntrlInfo->carrierFreq.pres),PRSNT_NODEF);
   wrFillTknU32(&(mobCntrlInfo->carrierFreq.dl_CarrierFreq),
         cellCb->dlCarrierFreq);
   /* Fill Carrier Bandwidth */
   wrFillTknU8(&(mobCntrlInfo->carrierBandwidth.pres),PRSNT_NODEF);
   wrFillTknU32(&(mobCntrlInfo->carrierBandwidth.dl_Bandwidth),
         cellCb->mib.dlBw);
   /* ccpu00134584 eNB broadcasting parameter "freqInfo-> ul-Bandwidth" for TDD mode.*/
#ifndef LTE_TDD
   wrFillTknU32(&(mobCntrlInfo->carrierFreq.ul_CarrierFreq),
         cellCb->sib2.ulCarrierFreq);
   wrFillTknU32(&(mobCntrlInfo->carrierBandwidth.ul_Bandwidth),
         cellCb->sib2.ulBw);
#endif
   wrFillTknU32(&(mobCntrlInfo->additionalSpectrumEmission),
         cellCb->sib2.addlSpectrEmission);
   wrFillTknU32(&(mobCntrlInfo->t304), cellCb->ueTmrConstants.t304);
   /* Fill New UE Identity.*/
   len = 16; /* Length of the Identity*/
   WR_FILL_BIT_STR_VAL(mobCntrlInfo->newUE_Identity, len, ueCb->crnti, dcchEncReq);
   /* Fill RRC Connection Reconfiguration IE.*/
   wrFillTknU8(&(mobCntrlInfo->radioResourceConfigCmn.pres),PRSNT_NODEF);
   rachConfigCommon =  &mobCntrlInfo->radioResourceConfigCmn.rach_ConfigCmn;
   wrFillTknU8(&(rachConfigCommon->pres),PRSNT_NODEF);
   wrFillTknU8(&(rachConfigCommon->preambleInfo.pres),PRSNT_NODEF);
   /* Fill No of non dedicated Preamble. 9 means that 40 preambles.*/
   wrFillTknU32(&(rachConfigCommon->preambleInfo.numberOfRA_Preambles),
                   cellCb->sib2.rachCfg.noRaPreambles);
   wrFillTknU8(&(rachConfigCommon->preambleInfo.preamblesGroupAConfig.pres),PRSNT_NODEF);
   /* Fill sizeOfRA_PreamblesGroupA IE.*/
   wrFillTknU32(&(rachConfigCommon->preambleInfo.preamblesGroupAConfig.sizeOfRA_PreamblesGroupA),
                   cellCb->sib2.rachCfg.noGrpAPreambles);
   wrFillTknU32(&(rachConfigCommon->preambleInfo.preamblesGroupAConfig.messageSizeGroupA),
                   cellCb->sib2.rachCfg.grpAMsgSize);
   wrFillTknU32(&(rachConfigCommon->preambleInfo.preamblesGroupAConfig.messagePowerOffsetGroupB),
                   cellCb->sib2.rachCfg.grpBPwrOffset);
   wrFillTknU8(&(rachConfigCommon->powerRampingParams.pres),PRSNT_NODEF);
   wrFillTknU32(&(rachConfigCommon->powerRampingParams.powerRampingStep),
                   cellCb->sib2.rachCfg.pwrRampStep);
   wrFillTknU32(&(rachConfigCommon->powerRampingParams.preambleInitReceivedTgtPower),
                   cellCb->sib2.rachCfg.prmbInitRcvdTrgtPwr);
   /* Fill ra_SupervisionInfo IE.*/
   wrFillTknU8(&(rachConfigCommon->ra_SupervisionInfo.pres),PRSNT_NODEF);
   wrFillTknU32(&(rachConfigCommon->ra_SupervisionInfo.preambleTransMax),
                   cellCb->sib2.rachCfg.maxPrmbTrans);
   wrFillTknU32(&(rachConfigCommon->ra_SupervisionInfo.ra_RspWindowSize),
                   cellCb->sib2.rachCfg.raWindowSize);
   wrFillTknU32(&(rachConfigCommon->ra_SupervisionInfo.mac_ContentionResolutionTimer),
                   cellCb->sib2.rachCfg.contResTimer);
   wrFillTknU32(&(rachConfigCommon->maxHARQ_Msg3Tx),
                   cellCb->sib2.rachCfg.maxMsg3HarqTrans);

   /* Fill NhuPRACH_Config IE of Radio Resource Config Common.*/
   prachConfig = &mobCntrlInfo->radioResourceConfigCmn.prach_Config;
   wrFillTknU8(&(prachConfig->pres),PRSNT_NODEF);
   wrFillTknU32(&(prachConfig->rootSequenceIdx), cellCb->sib2.prachCfg.rootSeqIdx);
   wrFillTknU8(&(prachConfig->prach_ConfigInfo.pres),PRSNT_NODEF);
   wrFillTknU32(&(prachConfig->prach_ConfigInfo.prach_ConfigIdx),
                   cellCb->sib2.prachCfg.prachCfgIdx);
   wrFillTknU8(&(prachConfig->prach_ConfigInfo.highSpeedFlag),
                   cellCb->sib2.prachCfg.highSpeedFlag);
   wrFillTknU32(&(prachConfig->prach_ConfigInfo.zeroCorrelationZoneConfig),
                   cellCb->sib2.prachCfg.zeroCorrZone);
   wrFillTknU32(&(prachConfig->prach_ConfigInfo.prach_FreqOffset),
                   cellCb->sib2.prachCfg.prachFreqOffset);

   /* Fill NhuPDSCH_ConfigCmn IE */
   pdschCfgCmn = &mobCntrlInfo->radioResourceConfigCmn.pdsch_ConfigCmn;
   wrFillTknU8((&pdschCfgCmn->pres), PRSNT_NODEF);
   wrFillTknS32((&pdschCfgCmn->referenceSignalPower),
                   cellCb->sib2.pdschCfg.refSigPower);
   wrFillTknU32((&pdschCfgCmn->p_b),
                   cellCb->sib2.pdschCfg.pB);

   /* Fill NhuPUSCH_ConfigCmn IE */
   puschConfigCommon = &mobCntrlInfo->radioResourceConfigCmn.pusch_ConfigCmn;
   wrFillTknU8(&(puschConfigCommon->pres),PRSNT_NODEF);
   wrFillTknU8(&(puschConfigCommon->pusch_ConfigBasic.pres),PRSNT_NODEF);
   wrFillTknU32(&(puschConfigCommon->pusch_ConfigBasic.n_SB),
                   cellCb->sib2.puschCfg.nSB);
   wrFillTknU32(&(puschConfigCommon->pusch_ConfigBasic.hoppingMode),
                   cellCb->sib2.puschCfg.hoppingMode);
   wrFillTknU32(&(puschConfigCommon->pusch_ConfigBasic.pusch_HoppingOffset),
                   cellCb->sib2.puschCfg.hoppingOffset);
   wrFillTknU8(&(puschConfigCommon->pusch_ConfigBasic.enable64QAM),
                   cellCb->sib2.puschCfg.qam64Enabled);
   wrFillTknU8(&(puschConfigCommon->ul_ReferenceSignalsPUSCH.pres),PRSNT_NODEF);
   wrFillTknU8(&(puschConfigCommon->ul_ReferenceSignalsPUSCH.groupHoppingEnabled),
                   cellCb->sib2.puschCfg.grpHoppingEnabled);
   wrFillTknU32(&(puschConfigCommon->ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH),
                   cellCb->sib2.puschCfg.grpAssignPusch);
   wrFillTknU8(&(puschConfigCommon->ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled),
                   cellCb->sib2.puschCfg.seqHoppingEnabled);
   wrFillTknU32(&(puschConfigCommon->ul_ReferenceSignalsPUSCH.cyclicShift),
                   cellCb->sib2.puschCfg.cyclicShift);

   /* Fill NhuPHICH_Config IE */
   phichCfg = &mobCntrlInfo->radioResourceConfigCmn.phich_Config;
   wrFillTknU8((&phichCfg->pres), PRSNT_NODEF);
   wrFillTknU32((&phichCfg->phich_Duration),
                   cellCb->mib.phichDur);
   wrFillTknU32((&phichCfg->phich_Resource),
                   cellCb->mib.phichGrps);

   /* Fill NhuPUCCH_ConfigCmn IE */
   pucchCfgCmn = &mobCntrlInfo->radioResourceConfigCmn.pucch_ConfigCmn;
   wrFillTknU8((&pucchCfgCmn->pres), PRSNT_NODEF);
   wrFillTknU32((&pucchCfgCmn->deltaPUCCH_Shift),
                   cellCb->sib2.pucchCfg.deltaPucchShift);
   wrFillTknU32((&pucchCfgCmn->nRB_CQI),
                   cellCb->sib2.pucchCfg.nRBCqi);
   wrFillTknU32((&pucchCfgCmn->nCS_AN),
                   cellCb->sib2.pucchCfg.nCSAN);
   wrFillTknU32((&pucchCfgCmn->n1PUCCH_AN),
                   cellCb->sib2.pucchCfg.n1PucchAn);

   /* Fill NhuSoundingRS_UL_ConfigCmn IE */
   srsUlCfgCmn = &mobCntrlInfo->radioResourceConfigCmn.soundingRS_UL_ConfigCmn;
   wrFillTknU8((&srsUlCfgCmn->choice), 0);
   wrFillTknU8((&srsUlCfgCmn->val.setup.pres), PRSNT_NODEF);
   wrFillTknU32((&srsUlCfgCmn->val.setup.srs_BandwidthConfig),
                   cellCb->sib2.srsCfg.srsBandwidthCfg);
   wrFillTknU32((&srsUlCfgCmn->val.setup.srs_SubfrmConfig),
                   cellCb->sib2.srsCfg.srsSubfrmCfg);
   wrFillTknU8((&srsUlCfgCmn->val.setup.ackNackSRS_SimultaneousTransmission),
                   cellCb->sib2.srsCfg.ackNackSrsSimulTx);
   wrFillTknU32((&srsUlCfgCmn->val.setup.srs_MaxUpPts), 0);

   /* Fill NhuUlnkPowerCntrlCmn IE */
   ulPwrCntrlCmn = &mobCntrlInfo->radioResourceConfigCmn.uplinkPowerCntrlCmn;
   wrFillTknU8((&ulPwrCntrlCmn->pres), PRSNT_NODEF);
   wrFillTknS32((&ulPwrCntrlCmn->p0_NominalPUSCH),
                   cellCb->sib2.ulPwrCtrlCfg.p0NominalPusch);
   wrFillTknU32((&ulPwrCntrlCmn->alpha),
                   cellCb->sib2.ulPwrCtrlCfg.alpha);
   wrFillTknS32((&ulPwrCntrlCmn->p0_NominalPUCCH),
                   cellCb->sib2.ulPwrCtrlCfg.p0NominlPucch);
   wrFillTknU8((&ulPwrCntrlCmn->deltaFLst_PUCCH.pres), PRSNT_NODEF);
   wrFillTknU32((&ulPwrCntrlCmn->deltaFLst_PUCCH.deltaF_PUCCH_Format1),
                   cellCb->sib2.ulPwrCtrlCfg.deltaFPucchFormat1);
   wrFillTknU32((&ulPwrCntrlCmn->deltaFLst_PUCCH.deltaF_PUCCH_Format1b),
                   cellCb->sib2.ulPwrCtrlCfg.deltaFPucchFormat1b);
   wrFillTknU32((&ulPwrCntrlCmn->deltaFLst_PUCCH.deltaF_PUCCH_Format2),
                   cellCb->sib2.ulPwrCtrlCfg.deltaFPucchFormat2);
   wrFillTknU32((&ulPwrCntrlCmn->deltaFLst_PUCCH.deltaF_PUCCH_Format2a),
                   cellCb->sib2.ulPwrCtrlCfg.deltaFPucchFormat2a);
   wrFillTknU32((&ulPwrCntrlCmn->deltaFLst_PUCCH.deltaF_PUCCH_Format2b),
                   cellCb->sib2.ulPwrCtrlCfg.deltaFPucchFormat2b);
   wrFillTknS32((&ulPwrCntrlCmn->deltaPreambleMsg3),
                   cellCb->sib2.ulPwrCtrlCfg.deltaPeambleMsg3);

   /* Fill NhuAntennaInfoCmn IE */
   antInfoCmn = &mobCntrlInfo->radioResourceConfigCmn.antennaInfoCmn;
   wrFillTknU8(&(antInfoCmn->pres), PRSNT_NODEF);
   WR_GET_NUM_ANT_PORT(cellCb,numAntPorts);
#if 0
   wrFillTknU32(&(antInfoCmn->antennaPortsCount), numAntPorts);
#else
   /* Hardcoding to test the handover failure. Need to check why numAntPorts is wrongely configured */
   wrFillTknU32(&(antInfoCmn->antennaPortsCount),NhuAntennaInfoCmnantennaPortsCountan2Enum); 
#endif

   /* Fill NhuP_Max IE */
   pMax = &mobCntrlInfo->radioResourceConfigCmn.p_Max;
   wrFillTknS32(pMax, cellCb->sib1.pMax);

   /* Fill NhuTDD_Config IE which is Mandatory.*/
   tddCfg = &mobCntrlInfo->radioResourceConfigCmn.tdd_Config;
#ifndef LTE_TDD
   tddCfg->pres.pres = NOTPRSNT;
#else
   /*ccpu00134580-ADD-To support TDD in x2 setup*/
   WR_FILL_TKN_UINT(tddCfg->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(tddCfg->subframeAssignment, cellCb->sib1.tddCfg.subframeCfg);
   WR_FILL_TKN_UINT(tddCfg->specialSubfrmPatterns, cellCb->sib1.tddCfg.specialSubframeCfg);
#endif

   /* Fill NhuUL_CyclicPrefixLength IE */
   wrFillTknU32(&(mobCntrlInfo->radioResourceConfigCmn.ul_CyclicPrefixLength),0);
/* ccpu00133532*/
   /* Fill NhuRACH_ConfigDedicated IE.*/
   if( WR_VAL_ZERO != ueCb->dedicatedPrmbl)
   {
       wrFillTknU8(&(mobCntrlInfo->rach_ConfigDedicated.pres),PRSNT_NODEF);
       wrFillTknU32(&(mobCntrlInfo->rach_ConfigDedicated.ra_PreambleIdx), ueCb->dedicatedPrmbl);
       wrFillTknU32(&(mobCntrlInfo->rach_ConfigDedicated.ra_PRACH_MaskIdx), WR_UMM_PRACH_MASK_IDX);
   }
   else
   {
       mobCntrlInfo->rach_ConfigDedicated.pres.pres = NOTPRSNT;
   }
/* ccpu00133532*/

   /* Fill security configuration */
   secConfigHO = &(rrcConRecfgn_r8->securityConfigHO);
   wrFillTknU8(&(secConfigHO->pres),PRSNT_NODEF);

   /* As per 36.331, "SecurityConfigHO" of RRCRECONFIGURATION message
    * AS security configuration filling is handled  differently for intra LTE
    * and Inter RAT to UTRAN */
   if(( WR_UMM_S1_TGT_HO_TRANS == transCb->transTyp) &&
      ((SztHovrTyputrantolteEnum == transCb->u.s1TgtHoTrans.sztHovrTyp) ||
       (SztHovrTypgerantolteEnum == transCb->u.s1TgtHoTrans.sztHovrTyp)))
   {
      NhuSecConfigHOhandoverTypinterRAT   *interRat = NULLP;
      WrUmmIRatHoTgtInfo                  *iRatHoInfo;

      wrFillTknU8(&(secConfigHO->handoverTyp.choice),HANDOVERTYP_INTERRAT);
      wrFillTknU8(&(secConfigHO->handoverTyp.val.interRAT.pres),PRSNT_NODEF);
   
      interRat = &(secConfigHO->handoverTyp.val.interRAT);

      secCfg = &interRat->securityAlgorithmConfig;
      iRatHoInfo = &transCb->u.s1TgtHoTrans.u.iRatHoInfo; 
  
      wrFillTknStrOSXL(&(interRat->nas_SecParamToEUTRA),
                        iRatHoInfo->nasSecParamToEutran.len, 
                        iRatHoInfo->nasSecParamToEutran.val,
                        &(dcchEncReq->memCp));
   }
   else
   {
      NhuSecConfigHOhandoverTypintraLTE  *intraLTE = NULLP;

      wrFillTknU8(&(secConfigHO->handoverTyp.choice),HANDOVERTYP_INTRALTE);
      wrFillTknU8(&(secConfigHO->handoverTyp.val.intraLTE.pres),PRSNT_NODEF);

      intraLTE = &(secConfigHO->handoverTyp.val.intraLTE);
      secCfg = &intraLTE->securityAlgorithmConfig;

      wrFillTknU8(&(intraLTE->keyChangeIndicator), FALSE);
      wrFillTknU32(&(intraLTE->nextHopChainingCount), ueCb->nhChainCnt);
   }
   wrFillTknU8(&(secCfg->pres),PRSNT_NODEF);

   wrFillTknU32(&(secCfg->cipheringAlgorithm), ueCb->secCfg->ciphAlgo);
      /* Configure the integrity protection algorithm */
   wrUmmRrcFillIntgAlgo(secCfg,ueCb);

   /*Filling Dedicated Radio Config IE */
   /* SPS changes starts */
   wrUmmRrcFillDedRadioCfg(rrcCfgDed,ueCb,&(dcchEncReq->memCp), isUeUlSpsEnabled);
   /* SPS changes ends */

   /* ccpu00131426 - Fix: Fill DRX configuration info */
      drxCfgIe = &rrcCfgDed->mac_MainConfig.val.explicitValue.drx_Config;
   /*DRX QCI*/
   if(ueCb->drxCfg.isDrxEnabled == TRUE && ueCb->isGBRBearerExists == NOT_IN_USE)
   {   
      wrUmmRrcFillDrxCfgIe(drxCfgIe, &ueCb->drxCfg);
   }
   else
   {   
   /* Release any DRX configuration */
      wrFillTknU8(&drxCfgIe->choice, DRX_CONFIG_RELEASE);
   }

   /*Filling Measurement configuration */
   nhuMeasCfg  = &(rrcConRecfgn_r8->measConfig);
   if(ROK != wrUmmMeasInitialUeCfg (&dcchEncReq->memCp, ueCb, measTransCb,\
                       nhuMeasCfg, transCb->transId, WR_MEAS_CONFIG_EVT_ANR))
   {
      RLOG0(L_ERROR, "MeasCfg Failed");
      RETVALUE(RFAILED);
   }

   /* Need to include meas ID remove list in case of X2 and S1 intra LTE HO
    * and shall not be included in case of inter RAT HO
    */
   if(( WR_UMM_X2_TGT_HO_TRANS == transCb->transTyp) ||
         (( WR_UMM_S1_TGT_HO_TRANS == transCb->transTyp) &&
          ( SztHovrTypintralteEnum == transCb->u.s1TgtHoTrans.sztHovrTyp)))
   {
      /* update the measIdRemLst - remove the extra measId's that are not
       * configured */
      if(ROK != wrUtlFillMeasRptCfgMeasObjIDRmvLst(ueCb, dcchEncReq, nhuMeasCfg))
      {
         RLOG0(L_ERROR, "MeasCfg Filling Remove IdLst Failed");
         RETVALUE(RFAILED);
      }
   }

   /* RRC-REL10-Upgrade Start */
   if(WR_UMM_X2_TGT_HO_TRANS == transCb->transTyp)
   {
      tmpEutraHoTgtInfo = &(transCb->u.x2TgtHoTrans.eutraTgtInfo);
   }
   else
   {
      tmpEutraHoTgtInfo = &(transCb->u.s1TgtHoTrans.u.eutraTgtInfo);
   } 

   if(NhuAccessStratumRlsrel9Enum < ueCb->ueCap->eUTRA_Cap.accessStratumRls.val )   
   {
      NhuRRCConRecfgn_v920_IEs   *tmpRel10Ies = 
                     &(rrcConRecfgn_r8->nonCriticalExtn.nonCriticalExtn);

      /* Filling NhuRRCConRecfgn_v920_IEsfullConfig_r9 IE */
      /* Check ueCb->isFullCfgReqd flag is TRUE, fill fullConfig-r9 IE with 
       * enumeration value to TRUE in RRCConnectionReconfiguration-v920-IEs.*/
      if(ueCb->isFullCfgReqd)
      {
         rrcConRecfgn_r8->nonCriticalExtn.pres.pres = PRSNT_NODEF;
         tmpRel10Ies->pres.pres = PRSNT_NODEF;
         wrFillTknU32(&(tmpRel10Ies->fullConfig_r9), NhuRRCConRecfgn_v920_IEsfullConfig_r9trueEnum);
         /* increment - KPI_ID_LTE_HO_FROMENB_FULL_RECFG KPI here */
         WR_INC_KPI_FUNC(wrIncKpiFullRecfgHo);
      }

      /* Filling NhuSCellToRlsLst_r10 IE */
      /* if needToFillSCellToRlsLstR10 is set to TRUE then, fill 
       * sCellToReleaseList-r10 IE in RRCConnectionReconfiguration-v1020-IEs
       * from the TgtHoTrans CB  */
      if(TRUE == tmpEutraHoTgtInfo->needToFillSCellToRlsLstR10)
      {
         rrcConRecfgn_r8->nonCriticalExtn.pres.pres = PRSNT_NODEF;
         tmpRel10Ies->pres.pres = PRSNT_NODEF;
         tmpRel10Ies->nonCriticalExtn.pres.pres = PRSNT_NODEF;
         wrFillTknU16(&(tmpRel10Ies->nonCriticalExtn.sCellToRlsLst_r10.noComp), 
               tmpEutraHoTgtInfo->sourceSCellRelLst_r10.noComp.val);
         WR_GET_MEM(dcchEncReq, 
               (tmpEutraHoTgtInfo->sourceSCellRelLst_r10.noComp.val * sizeof(NhuSCellIdx_r10)),
               &(tmpRel10Ies->nonCriticalExtn.sCellToRlsLst_r10.member));
         cmMemcpy((U8 *)tmpRel10Ies->nonCriticalExtn.sCellToRlsLst_r10.member,
               (U8 *)tmpEutraHoTgtInfo->sourceSCellRelLst_r10.member, 
               (tmpEutraHoTgtInfo->sourceSCellRelLst_r10.noComp.val * sizeof(NhuSCellIdx_r10)));
      }

      /* Check if isMeasSubFrPatPCellR10Pres is set to TRUE. If yes, fill 
       * MeasSubframePatternPCell-r10 IE with release configuration to NULL*/
      if(TRUE == tmpEutraHoTgtInfo->isMeasSubFrPatPCellR10Pres)
      {
         rrcConRecfgn_r8->nonCriticalExtn.pres.pres = PRSNT_NODEF;
         tmpRel10Ies->pres.pres = PRSNT_NODEF;
         rrcCfgDed->extaddgrp_2.pres.pres = PRSNT_NODEF;
         wrFillTknU8(&(rrcCfgDed->extaddgrp_2.measSubfrmPatternPCell_r10.choice),
               MEASSUBFRMPATTERNPCELL_R10_RELEASE);
      }
   }
   else
   {
      rrcConRecfgn_r8->nonCriticalExtn.pres.pres = NOTPRSNT;
   }

   /* In wrUmmRrcFillDedRadioCfg isRlfTmrConstR9Pres to release as it is common 
    * function. If Source does not sent, do not fill RlfTmrConstR9 Release */
   if(FALSE == tmpEutraHoTgtInfo->isRlfTmrConstR9Pres)
   {
      rrcCfgDed->extaddgrp_1.pres.pres = NOTPRSNT;
   }

   /* RRC-REL10-Upgrade End*/
   /* SPS changes starts */
   bRadioResDedBitMap = 0;
   if (rrmUeHoResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG)
   {
      bRadioResDedBitMap = RM_RCM_DL_SPS_CFG;
   }
   else if (srcToTgtSpsInfo->isDlSpsPresent == TRUE)
   {
      /* Since RRM has not allocated DL SPS resource for this UE and SPS was present in the Src 
       * to Tgt container so fill release SPS in the Tgt to Src container */
      bRadioResDedBitMap = RM_RCM_DL_SPS_CFG;
      rrmUeHoResp->stUeRadioResCfg.stSpsCfg.stSpsConfigDl.bIsSpsEnabled = FALSE;
      RLOG0(L_WARNING, "wrUtlFillHoCmdTrnspCntrWthMobility: DL SPS received in SRC to TGT but not enabled from RRM\n");
   }

   if (rrmUeHoResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_UL_SPS_CFG)
   {
      bRadioResDedBitMap |= RM_RCM_UL_SPS_CFG;
   }
   else if (srcToTgtSpsInfo->isUlSpsPresent == TRUE)
   {
      /* Since RRM has not allocated UL SPS resource for this UE and SPS was present in the Src 
       * to Tgt container so fill release SPS in the Tgt to Src container */
      bRadioResDedBitMap |= RM_RCM_UL_SPS_CFG;
      rrmUeHoResp->stUeRadioResCfg.stSpsCfg.stSpsConfigUl.bIsSpsEnabled = FALSE;
      RLOG0(L_WARNING, "wrUtlFillHoCmdTrnspCntrWthMobility: UL SPS received in SRC to TGT but not enabled from RRM\n");
   }

   wrUmmRrcFillSpsCfgIe(&(dcchEncReq->memCp), bRadioResDedBitMap, \
         pSpsCfg->spsRnti, &rrmUeHoResp->stUeRadioResCfg.stSpsCfg.stSpsConfigDl, \
         &rrmUeHoResp->stUeRadioResCfg.stSpsCfg.stSpsConfigUl, &rrcCfgDed->sps_Config, \
         wrEmmGetDuplexMode(ueCb->cellId));
   /* SPS changes ends */

   RETVALUE(ROK);
}/* End of wrUtlFillHoCmdTrnspCntrWthMobility*/

PRIVATE S16 wrUmmHoFillCreateHoTrans
(
WrUmmIntCrHoTran             *crHoTrans,
U8                           type,
WrUeCb                       *ueCb,
WrHoNghInfo                  *hoNghInfo
)
{
   WrUmmIntraLteHoNghInfo *intraLteHoNghInfo = &(hoNghInfo->t.intraLteNghInfo);
   WrUmmIntCrIntraLteHoTranInfo *intraLteHoTransInfo = &(crHoTrans->t.intraLte);

   WrUtraNeighCellCb *neighCellCb = NULLP;
   WrUmmGeranCellCb  *geranCellCb = NULLP;
   
    /* Filling of Dummy Message.*/
   crHoTrans->crnti    = ueCb->crnti;
   crHoTrans->cellId   = ueCb->cellId;
   crHoTrans->hoType   = type;
   crHoTrans->peerId   = hoNghInfo->peerId;
  
   crHoTrans->hoCause.causeTyp = WR_CAUSE_RADIONW;
   if( ( WR_HO_TYPE_X2 == type) || ( WR_HO_TYPE_INTRA_LTE_S1 == type) )
   {
      intraLteHoTransInfo->enbType  = intraLteHoNghInfo->enbType;

      intraLteHoTransInfo->ecgi.plmnId.numMncDigits  = hoNghInfo->plmnId.numMncDigits;
      intraLteHoTransInfo->ecgi.plmnId.mcc[0]  = hoNghInfo->plmnId.mcc[0];
      intraLteHoTransInfo->ecgi.plmnId.mcc[1]  = hoNghInfo->plmnId.mcc[1];
      intraLteHoTransInfo->ecgi.plmnId.mcc[2]  = hoNghInfo->plmnId.mcc[2];
      intraLteHoTransInfo->ecgi.plmnId.mnc[0]  = hoNghInfo->plmnId.mnc[0];
      intraLteHoTransInfo->ecgi.plmnId.mnc[1]  = hoNghInfo->plmnId.mnc[1];
   if(hoNghInfo->plmnId.numMncDigits > 2)
   {
         intraLteHoTransInfo->ecgi.plmnId.mnc[2]  = hoNghInfo->plmnId.mnc[2];
   }
      intraLteHoTransInfo->ecgi.eutranCellId   = intraLteHoNghInfo->eutranCellId;
      intraLteHoTransInfo->tac                 = intraLteHoNghInfo->tac;
   /* ccpu00126532 */
      intraLteHoTransInfo->pci                 = intraLteHoNghInfo->pci;
      intraLteHoTransInfo->earfcnDl            = intraLteHoNghInfo->earfcn;


      if(intraLteHoNghInfo->isX2Connected == WR_NEIGH_ENB_STATE_UP)
      {
         intraLteHoTransInfo->isX2InterFace = TRUE;
      }
      else
      {
         intraLteHoTransInfo->isX2InterFace = FALSE;
      }

   }
   else if ( WR_HO_TYPE_IRAT_UTRA == type )
   {
      WrUmmIntCrIRatUtraHoTranInfo *iRatUtraHoTransInfo = 
         &(crHoTrans->t.interRatUtra);

      neighCellCb = (WrUtraNeighCellCb *)hoNghInfo->neighCellCb;

      if(neighCellCb->duplexMode == WR_UTRA_MODE_FDD)
      {
         iRatUtraHoTransInfo->arfcnDl  =    neighCellCb->t.utraFdd.arfcnDl;
         iRatUtraHoTransInfo->psc      =    neighCellCb->t.utraFdd.psc;
      }
      else
      {
         iRatUtraHoTransInfo->arfcnDl  =    neighCellCb->t.utraTdd.arfcn;
         iRatUtraHoTransInfo->psc      =    neighCellCb->t.utraTdd.psc;
      }
      iRatUtraHoTransInfo->rncId    =    neighCellCb->rncId;
      iRatUtraHoTransInfo->cId      =    neighCellCb->cId;      
      iRatUtraHoTransInfo->lac      =    neighCellCb->lac;
      iRatUtraHoTransInfo->rac      =    neighCellCb->rac;

      cmMemcpy((U8 *)&iRatUtraHoTransInfo->plmnId,
            (U8 *) &(neighCellCb->plmnId[0]), sizeof(WrPlmnId));
      /* Fill SRVCC Indication */
      iRatUtraHoTransInfo->srvccHoInd  = hoNghInfo->t.iRatUtraNghInfo.srvccHoInd;
   }
   else if ( WR_HO_TYPE_IRAT_GERAN == type )
   {
      WrUmmIntCrIRatGeranHoTranInfo *iRatGeranHoTransInfo = 
         &(crHoTrans->t.interRatGeran);

      geranCellCb = (WrUmmGeranCellCb*)hoNghInfo->neighCellCb;
      iRatGeranHoTransInfo->cId   =  geranCellCb->cId;      
      iRatGeranHoTransInfo->lac   =  geranCellCb->lac;
      iRatGeranHoTransInfo->rac   =  geranCellCb->rac;
      iRatGeranHoTransInfo->bsic  =  geranCellCb->bsic;
      iRatGeranHoTransInfo->rssi  =  hoNghInfo->t.iRatGeranNghInfo.rssi;
      iRatGeranHoTransInfo->bandIndicator =  geranCellCb->freqCb->bandInd;

      cmMemcpy((U8 *)&iRatGeranHoTransInfo->plmnId,
            (U8 *) &(geranCellCb->plmnId), sizeof(WrPlmnId));
      iRatGeranHoTransInfo->srvccHoInd  = hoNghInfo->t.iRatGeranNghInfo.srvccHoInd;
      iRatGeranHoTransInfo->psSrvcNotAvlbl = hoNghInfo->t.iRatGeranNghInfo.psSrvcNotAvlbl;
   }
   /* CSG_DEV Start */
   crHoTrans->csgId = hoNghInfo->csgId;
   crHoTrans->csgCellAccessMode = hoNghInfo->csgCellAccessMode;
   /* CSG_DEV Ends */
   /*FIX for - ccpu00127185*/
   if( WR_HO_TYPE_X2 == crHoTrans->hoType )
   {
      /* X2 Handover*/
   /* CztCauseRadioNwhandover_desirable_for_radio_reasonsEnum */
   crHoTrans->hoCause.causeVal = 
       CztCauseRadioNwtime_critical_handoverEnum;
   }
   else
   {
      /* S1 Handover*/
   /*SztCauseRadioNwhandover_desirable_for_radio_reasonEnum*/
   crHoTrans->hoCause.causeVal = 
       SztCauseRadioNwtime_critical_handoverEnum;
   }

   RETVALUE(ROK);
}

PRIVATE S16 wrHoFillHoDecRslt
(
U16                          pci,
U8                           status,
U8                           hoType,
WrHoDecRslt                  *hoDecRslt
)
{
   TRC2(wrHoFillHoDecRstl)

   hoDecRslt->pci    = pci;
   hoDecRslt->status = status;
   hoDecRslt->val.typeOfHo.hoType = hoType;

   RETVALUE(ROK);
}


/**
 * @brief This function processes the A5 Event received
 *
 * @details
 *
 *     Function: wrHoPrcEutraEvnt
 *     Purpose:  This function is used to take Handover Decision.
 *
 *     Steps to be performed
 *    ======================
 *    1. 
 *       
 *     Invoked by: HO Module
 *
 *  @param[in] WrUeCb              *ueCb,
 *  @param[in] U16                 pci,
 *  @param[in] U8                  hoStatus,
 *  @param[in] U8                  hoType,
 *  @param[in] NhuMeasmentRprt     *measRprt
 *
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/
PRIVATE S16 wrHoPrcEutraEvnt
(
WrUeCb                       *ueCb,
U8                           *hoStatus,
U8                           *hoType,
NhuMeasResults               *measRprt,
U32                          earfcn,
WrHoNghInfo                  *hoNghInfo
)
{
   U16                       pci = 0;
   NhuMeasResultLstEUTRA     *eutraMeasRes;
   U32                       idx;
   /* CSG_DEV start */
   WrUmmCellCb               *cellCb;
   WrHoNghInfo               *nbrInfoTemp;
   WrHoNghInfo               nbrInfo[WR_UMM_HO_MAX_NBR_CELLS_FOR_HO];
   U8                        hoStatusLst[WR_UMM_HO_MAX_NBR_CELLS_FOR_HO];
   U8                        hoTypeLst[WR_UMM_HO_MAX_NBR_CELLS_FOR_HO];
   U8                        nbrPriority[WR_UMM_HO_NBR_PRIORITY_MAX];
   U8                        nbrInfoCnt = 0;
   U8                        lastSetPriority = WR_UMM_HO_NBR_PRIORITY_MAYBE;
   /* CSG_DEV end */
   WrHrlDecisionRet          hrlDec = WR_MEAS_MBLTY_NOT_APPL_TO_CELL;
   /*rsrq_ho*/
   S8                        rsrq = -1;

   TRC2(wrHoPrcEutraEvnt);

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId,
            "Error in getting Cell CB");
      RETVALUE(RFAILED);
   }


   eutraMeasRes = &(measRprt->measResultNeighCells.val.measResultLstEUTRA);
   /* ccpu00128905 */
   *hoStatus = WR_HO_DEC_FAILURE;

   if (NULLP == ueCb->secCfg)
   {
      RLOG0(L_WARNING, "SecCfg is NULL; Ignoring the measurement report " );
      RETVALUE(RFAILED);
   }

   for(idx = 0; (idx < eutraMeasRes->noComp.val) && 
         (nbrInfoCnt < WR_UMM_HO_MAX_NBR_CELLS_FOR_HO); idx++)
   {
      /* rsrq_ho: If UE reports RSRQ then consider RSRQ before proceeding with this PCI
       * provided reported RSRQ meet the threshold configured.*/
      if(PRSNT_NODEF == eutraMeasRes->member[idx].measResult.rsrqResult.pres)
      {
         rsrq = eutraMeasRes->member[idx].measResult.rsrqResult.val;
         if(rsrq < cellCb->measCellCfg->rsrqHoThrshld)
         {
            continue;
         }
      }
      pci = eutraMeasRes->member[idx].physCellId.val;
      nbrInfoTemp = &nbrInfo[nbrInfoCnt];
      hoStatusLst[nbrInfoCnt] = WR_HO_DEC_FAILURE;

      /* decide handover type based on availability of x2 connection */
      nbrInfoTemp->t.intraLteNghInfo.pci    = pci;
      nbrInfoTemp->t.intraLteNghInfo.earfcn = earfcn;
      RLOG2(L_INFO, "Fetching details for PCI[%d] EARFCN[%lu]", pci, earfcn);
      if(wrEmmGetIntraLteHoNghInfo(ueCb->cellId, nbrInfoTemp) == ROK)
      {
         if(ROK != wrUmmCsgValidateNbrCsgForHo(ueCb, nbrInfoTemp,
                  &hoStatusLst[nbrInfoCnt], &hoTypeLst[nbrInfoCnt],
                  &nbrPriority[nbrInfoCnt]))
         {
            RLOG3(L_ERROR, "CSG validation failed for PCI = %d "
                  "EARFCN = %lu ENBID = %lu",
                  pci, earfcn, nbrInfoTemp->t.intraLteNghInfo.eutranCellId);
            RETVALUE(RFAILED);
         }
         if(hoStatusLst[nbrInfoCnt] == WR_HO_DEC_SUCCESS)
         {
            /* Validate the neighbor for PLMN, TA, GU group ID and
             * equivalent PLMN */
            if((ueCb->hoRstrctInfo != NULLP) && (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
            {
               hrlDec = wrMbltyRestrc(WR_INTRA_SYS_MBLTY, (PTR *)nbrInfoTemp->neighCellCb, 
                     (PTR *)nbrInfoTemp->t.intraLteNghInfo.neighEnbCb, &hoTypeLst[nbrInfoCnt],
                     &ueCb->fnlzdPLMN, ueCb->hoRstrctInfo);
               if(hrlDec == WR_MEAS_MBLTY_NOT_APPL_TO_CELL)
               {
                  /* HO not applicable to this cell, so goto the next cell */
                  continue;
               }
            }
#if 0 /* Remove the entire HRL cde added in S1 flex */
            wrEmmValidateNghAvailability(ueCb, nbrInfoTemp, 
                  &hoStatusLst[nbrInfoCnt], &hoTypeLst[nbrInfoCnt]);
#endif
            RLOG1(L_DEBUG," HO Type : hoTypeLst[nbrInfoCnt]:%d",hoTypeLst[nbrInfoCnt]);
         }
         if(hoStatusLst[nbrInfoCnt] == WR_HO_DEC_FAILURE)
         {
            RLOG3(L_WARNING, " HO Decision failed for PCI = %d "
                  "EARFCN = %lu ENBID = %lu",
                  pci, earfcn, nbrInfoTemp->t.intraLteNghInfo.eutranCellId);
            /* Check for next reported PCI */
            continue;
         }
         nbrInfoCnt++;
      }
      else
      {
         RLOG2(L_ERROR, "Failed To Get Neighbor Info for PCI[%d] EARFCN[%lu]",
            pci, earfcn);
         /*Check for next measured cell if any*/
         continue;
      }
   }

   if(nbrInfoCnt)
   {
      /* prioratize neighbor selection */
      nbrInfoTemp = NULLP;
      for(idx = 0; idx < nbrInfoCnt && idx < WR_UMM_HO_MAX_NBR_CELLS_FOR_HO; idx++)
      {
         if(lastSetPriority < nbrPriority[idx])
         {
            continue;
         }

         *hoStatus       = hoStatusLst[idx];
         *hoType         = hoTypeLst[idx];
         nbrInfoTemp     = &nbrInfo[idx];
         lastSetPriority = nbrPriority[idx];

         if(WR_UMM_HO_NBR_PRIORITY_HIGHEST == nbrPriority[idx])
         {
            break;
         }
      }

      if(NULLP != nbrInfoTemp)
      {
         hoNghInfo->t.intraLteNghInfo.pci = nbrInfoTemp->t.intraLteNghInfo.pci;
         hoNghInfo->t.intraLteNghInfo.earfcn = nbrInfoTemp->t.intraLteNghInfo.earfcn;
         hoNghInfo->peerId = nbrInfoTemp->peerId;
         hoNghInfo->t.intraLteNghInfo.eutranCellId = nbrInfoTemp->t.intraLteNghInfo.eutranCellId;
         wrUtlCopyPlmnId(&nbrInfoTemp->plmnId, &hoNghInfo->plmnId);
         wrCpyCmTptAddr(&hoNghInfo->t.intraLteNghInfo.neighEnbAddr, &nbrInfoTemp->t.intraLteNghInfo.neighEnbAddr);
         hoNghInfo->t.intraLteNghInfo.isX2Connected = nbrInfoTemp->t.intraLteNghInfo.isX2Connected;
         hoNghInfo->t.intraLteNghInfo.enbType = nbrInfoTemp->t.intraLteNghInfo.enbType;
         hoNghInfo->t.intraLteNghInfo.tac = nbrInfoTemp->t.intraLteNghInfo.tac;
         hoNghInfo->t.intraLteNghInfo.neighEnbCb = nbrInfoTemp->t.intraLteNghInfo.neighEnbCb;
         hoNghInfo->neighCellCb = nbrInfoTemp->neighCellCb;
         hoNghInfo->csgId = nbrInfoTemp->csgId;
         hoNghInfo->csgCellAccessMode = nbrInfoTemp->csgCellAccessMode;

         if(nbrInfoTemp->t.intraLteNghInfo.enbType == WR_ENODEB_TYPE_HOME)
         {
            RLOG3(L_INFO,"Handover to HOME ENB PCI[%d] EARFCN[%lu] ENBID[%lu]",
                  pci, earfcn, nbrInfoTemp->t.intraLteNghInfo.eutranCellId);
         }
         else
         {
            RLOG3(L_INFO,"Handover to MACRO ENB PCI[%d] EARFCN[%lu] ENBID[%lu]",
                  pci, earfcn, nbrInfoTemp->t.intraLteNghInfo.eutranCellId);
         }
      }
      else
      {
         *hoStatus = WR_HO_DEC_FAILURE;
      }
   }
   else
   {
      *hoStatus = WR_HO_DEC_FAILURE;
   }

   RETVALUE(ROK);
}/* End of wrHoPrcEutraEvnt*/

/**
 * @brief This function Validates the SRVCC procedure for GERAN
 *
 * @details
 *
 *     Function: wrUmmValidateSrvccForGeran
 *
 *  @param[in] WrUeCb              *ueCb,
 *
 *  @return  Bool
 *           -# TRUE
 *           -# FALSE  
 **/
PRIVATE Bool wrUmmValidateSrvccForGeran
(
WrUeCb                       *ueCb
)
{
   TRC2(wrUmmValidateSrvccFoGeran)
   if(ueCb->srvccOperPossible == FALSE)
   {
      RETVALUE(FALSE);
   }

   if(TRUE == wrUmmUtilFeatureGrpInd(ueCb->cellId,ueCb->crnti,\
               WR_UMM_GERAN_SRVCC_HO_SUPPORTED))
   {
         RETVALUE(TRUE);
   }
   RETVALUE(FALSE);
}
/**
 * @brief This function Validates the SRVCC procedure for UTRA
 *
 * @details
 *
 *     Function: wrUmmValidateSrvccForUtra
 *
 *  @param[in] WrUeCb              *ueCb,
 *  @param[in] U8                  duplexMode
 *
 *  @return  Bool
 *           -# TRUE
 *           -# FALSE  
 **/
PRIVATE Bool wrUmmValidateSrvccForUtra
(
WrUeCb                       *ueCb,
U8 									duplexMode
)
{
   TRC2(wrUmmValidateSrvccForUtra)
   if(ueCb->srvccOperPossible == FALSE)
   {
      RETVALUE(FALSE);
   }

   if(duplexMode == WR_UTRA_MODE_FDD)
   {
      if(TRUE == wrUmmUtilFeatureGrpInd(ueCb->cellId,ueCb->crnti,\
               WR_UMM_UTRA_CS_SUPPORTED))
      {
         RETVALUE(TRUE);
      }
   }
   RETVALUE(FALSE);
}
/**
 * @brief This function processes the B2 UTRA Event received
 *
 * @details
 *
 *     Function: wrHoPrcB2UtraEvnt
 *     Purpose:  This function is used to take Handover Decision.
 *
 *     Steps to be performed
 *    ======================
 *    1. 
 *       
 *     Invoked by: HO Module
 *
 *  @param[in] WrUeCb              *ueCb,
 *  @param[in] U8                  hoStatus,
 *  @param[in] U8                  hoType,
 *  @param[in] NhuMeasmentRprt     *measRprt
 *  @param[in] U32                 arfcn,
 *  @param[in] WrHoNghInfo         *hoNghInfo
 *  @param[in] Bool                isQci1BearPres
 *
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 **/
PRIVATE S16 wrHoPrcB2UtraEvnt
(
WrUeCb                       *ueCb,
U8                           *hoStatus,
U8                           *hoType,
NhuMeasResults               *measRprt,
U32                          arfcn,
WrHoNghInfo                  *hoNghInfo,
Bool                         isQci1BearPres
)
{
   U16                       psc;
   NhuMeasResultLstUTRA     *utraMeasRes;
   U32                       idx;
   WrUmmIRatUtraHoNghInfo   *iRatUtraHoNghInfo = 
      &(hoNghInfo->t.iRatUtraNghInfo);
   WrHrlDecisionRet          hrlDec = WR_MEAS_MBLTY_NOT_APPL_TO_CELL;
   /*rel10: To store the PSCs of target UTRAN Cells*/
   U8                        ecno = 0;
   S16                       rscp;
   WrUmmUtraRankLst          rankLst;
   WrUtraNeighCellCb          *utraNeighCell;

   TRC2(wrHoPrcB2UtraEvnt);

   /* Initialize rankLst to WR_MAX_U8_VALUE*/
   cmMemset((U8*)&rankLst,(U8)WR_MAX_U8_VALUE,sizeof(rankLst));

   utraMeasRes = &(measRprt->measResultNeighCells.val.measResultLstUTRA);

   *hoStatus = WR_HO_DEC_FAILURE;
   for(idx = 0; idx < utraMeasRes->noComp.val; idx++)
   {
      /*rel10: */
      rscp = wrUmmGetUtraRscp(&utraMeasRes->member[idx].measResult);
      ecno = wrUmmGetUtraEcno(&utraMeasRes->member[idx].measResult);

      if( PHYSCELLID_FDD == utraMeasRes->member[idx].physCellId.choice.val)
      {
         psc = utraMeasRes->member[idx].physCellId.val.fdd.val;
      }
      else
      {
         psc = utraMeasRes->member[idx].physCellId.val.tdd.val;
      }

      iRatUtraHoNghInfo->psc      = psc;
      iRatUtraHoNghInfo->arfcn    = arfcn;

      RLOG2(L_DEBUG,"Fetching details for PSC = %d ARFCN = %lu", psc, arfcn);

      hoNghInfo->neighCellCb = (PTR)wrEmmGetNeighUtraCell( ueCb->cellId,
            iRatUtraHoNghInfo->arfcn,
            iRatUtraHoNghInfo->psc, WR_UTRA_MODE_FDD);
      if( NULLP == hoNghInfo->neighCellCb)
      {
         RLOG2(L_WARNING,"Failed To Get Neighbor Info for PSC = %d"
               "ARFCN = %lu", psc, arfcn);
         continue;
      }
      else 
      {
         /* Validate the neighbor for PLMN, equivalent PLMN, LA
          * anf forbidden RAT */
         if((ueCb->hoRstrctInfo != NULLP) && (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
         {
            hrlDec = wrMbltyRestrc(WR_INTER_SYS_UTRA_MBLTY, (PTR *)hoNghInfo->neighCellCb, 
                  NULLP, NULLP, &ueCb->fnlzdPLMN, ueCb->hoRstrctInfo);
            if(hrlDec == WR_MEAS_MBLTY_NOT_APPL_TO_CELL)
            {
               /* HO not applicable to this cell, so goto the next cell */
               RLOG2(L_WARNING,"UTRA HO not applicable to this PSC = %d"
                     "ARFCN = %lu\n", psc, arfcn);
               continue;
            }
         }

         utraNeighCell = (WrUtraNeighCellCb*)hoNghInfo->neighCellCb;

         if((isQci1BearPres == FALSE) && 
            (utraNeighCell->isPsHOCapable == FALSE))
         {
            continue;
         }
         else
         {
            *hoStatus = WR_HO_DEC_SUCCESS;
            *hoType = WR_HO_TYPE_IRAT_UTRA;
            if(ecno != INVALID_ECN0)
            {
               if(rankLst.ecno < ecno)
               {
                  rankLst.ecno = ecno;
                  rankLst.psc = psc;
               }
            }
            else
            {
               RETVALUE(ROK);
            }
         }
      }
   }
   if(WR_MAX_U16_VALUE != rankLst.psc)
   {
      hoNghInfo->neighCellCb = (PTR)wrEmmGetNeighUtraCell(ueCb->cellId,
            iRatUtraHoNghInfo->arfcn, rankLst.psc,
            WR_UTRA_MODE_FDD);
      RETVALUE(ROK);
   }
   RETVALUE(RFAILED);
}

/**
 * @brief This function gets the GERAN neigbour cell using measurement result
 *
 * @details
 *
 *     Function: wrUmmGetGeranNbhCell
 *     Purpose:  This function is to get the one valid neighbour GERAN cell from configuration,
 *               based on the measurement report received from UE
 * 
 *     Invoked by: Measurement module
 *     
 *  @param[in] WrUeCb                       *ueCb,
 *  @param[in] NhuMeasResults               *measRslt,
 *  @param[out] WrHoNghInfo                 *hoNghInfo
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/
PRIVATE S16 wrUmmGetGeranNbhCell
(
WrUeCb                       *ueCb,
NhuMeasResults               *measRslt,
WrHoNghInfo                  *hoNghInfo
)
{
   NhuMeasResultLstGERAN    *geranMeasRes = NULLP;
   NhuMeasResultGERAN       *member = NULLP;
   WrUmmGeranCellCb         *geranCell = NULLP;
   WrHrlDecisionRet          hrlDec = WR_MEAS_MBLTY_APPL_TO_CELL;
   U8                        ncc = 0x00;
   U8                        bcc = 0x00;
   U32                       idx = 0;
   
   TRC2(wrUmmHdlGeranB2MeasRpt)

   geranMeasRes = &measRslt->measResultNeighCells.val.measResultLstGERAN;
   member = geranMeasRes->member;

   if(measRslt->measResultNeighCells.choice.val != 
      MEASRESULTNEIGHCELLS_MEASRESULTLSTGERAN)
   {
      RLOG0(L_ERROR, "invalid measurement report received");
      RETVALUE(RFAILED);
   }
   for(idx = 0; ((idx < geranMeasRes->noComp.val) && 
                 (idx < LWR_MAX_NUM_NEIGH_GERAN_CELLS)); idx++)
   {
      ncc = *member[idx].physCellId.networkColourCode.val;
      bcc = *member[idx].physCellId.baseStationColourCode.val;
      /* ASN Decoder gives the bit strings in big-endian, make them little
       * endian. ncc and bcc are three bit length */
      ncc >>= 5;
      bcc >>= 5;
      geranCell = (WrUmmGeranCellCb *)wrEmmAnrGetGeranNbr(ueCb->cellId,
            member[idx].carrierFreq.arfcn.val,
            member[idx].carrierFreq.bandIndicator.val, ncc, bcc );
      /* Copy the rssi from measurement result */
      hoNghInfo->t.iRatGeranNghInfo.rssi = member[idx].measResult.rssi.val;
      if(geranCell != NULLP)
      {
         /* Verify HRL restriction */
         if((ueCb->hoRstrctInfo != NULLP) && 
            (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
         {
            hrlDec = wrMbltyRestrc(WR_INTER_SYS_GERAN_MBLTY, (PTR *)geranCell,\
                  NULLP, NULLP, NULLP, ueCb->hoRstrctInfo);
            if(hrlDec != WR_MEAS_MBLTY_APPL_TO_CELL)
            {
               RLOG0(L_INFO, "HRL: Mobility to the neighbour is restricted");
               continue;
            }
         }
         hoNghInfo->neighCellCb = (PTR)geranCell;
         RLOG1(L_INFO, "Found GERAN Neighbour cell with BSIC %d\n", geranCell->bsic);
         break;
      }
      else{
         RLOG2(L_INFO, "Not Found GERAN neighbour with ncc[%d], bcc[%d]",ncc,bcc); 
      }/* End of if geranCell */
   }/* End of for */
   if((geranCell == NULLP ) || 
      (idx == geranMeasRes->noComp.val) ||
      (idx == LWR_MAX_NUM_NEIGH_GERAN_CELLS))
   {
      RLOG0(L_ERROR, "Unable to find valid neighbour GERAN cell\n");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}/* End of wrUmmGetGeranNbhCell */


/**
 * @brief This function checks if ue is capable of voip
 *
 * @details
 *
 *     Function: wrUmmIsUeVoipCapable
 *     Purpose:  This function returns TRUE is ue is capable of Voip service
 * 
 *     
 *  @param[in] WrUeCb                       *ueCb,
 *  @return  Bool
 *           -# TRUE
 *           -# FALSE
 *
 **/
PRIVATE Bool wrUmmIsUeVoipCapable
(
WrUeCb        *ueCb
)
{
   NhuUE_EUTRA_Cap_v9a0_IEs  *ueCap_v9a0 =  NULLP;

   if(ueCb->ueCap != NULLP)
   {
      ueCap_v9a0 = &ueCb->ueCap->eUTRA_Cap_v9a0;
      if((ueCap_v9a0->nonCriticalExtn.interRAT_ParamsUTRA_v9c0.pres.pres 
          == PRSNT_NODEF) &&
         (ueCap_v9a0->nonCriticalExtn.interRAT_ParamsUTRA_v9c0.
          srvcc_FromUTRA_FDD_ToUTRA_FDD_r9.pres == PRSNT_NODEF) &&
         (ueCap_v9a0->nonCriticalExtn.interRAT_ParamsUTRA_v9c0.
          srvcc_FromUTRA_FDD_ToUTRA_FDD_r9.val == 
          NhuIRAT_ParamsUTRA_v9c0srvcc_FromUTRA_FDD_ToUTRA_FDD_r9supportedEnum))
      {
         RETVALUE(TRUE);
      }

   }
   RETVALUE(FALSE);
}/* End of wrUmmIsUeVoipCapable */
/**
 * @brief This function builds Mobility From E-UTRA Command message 
 *
 * @details
 *
 *     Function: wrHandoverDecision
 *     Purpose:  This function handles the Handover Decision received.
 * 
 *     Steps to be performed
 *     ====================
 *     1. Perform Handover Decision. To take the handover decision invoke
 *        wrHandoverDecision() function.              
 *     Invoked by: Measurement module
 *     
 *  @param[in] WrUeCb                       *ueCb,
 *  @param[in] U16                           pci,
 *  @param[in] U8                            evnt,
 *  @param[in] NhuMeasmentRprt              *measRprt,
 *  @param[in, out]WrHoDecRslt              *hoDecRslt,
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/
PRIVATE S16 wrHandoverDecision
(
WrUeCb                       *ueCb,
WrMeasRptCfgType             evnt,
NhuMeasResults               *measRslt,
U32                          arfcn,
WrHoDecRslt                  *hoDecRslt,
WrHoNghInfo                  *hoNghInfo
)
{
   U8                         status = 0;
   U8                         hoType = 0;
   Bool                       isQci1BearPres = FALSE;
   Bool                       isSrvccSupported = FALSE;

   switch(evnt)
   {
      case WR_UMM_MEAS_EVENT_A3:
      case WR_UMM_MEAS_EVENT_A5:
      {
         if (ROK != wrHoPrcEutraEvnt(ueCb, &status, &hoType, measRslt, arfcn, hoNghInfo))
         {
            RLOG0(L_ERROR, "Processing of A5 Event unsuccessful. Ignoring "
               "Measurement Report");
         }
         wrHoFillHoDecRslt(hoNghInfo->t.intraLteNghInfo.pci, status, hoType, hoDecRslt);
         break;
      }
      case WR_UMM_MEAS_EVENT_B2_UTRA:
      {
         Bool                      isVoipSupported = FALSE;

         /* Check if QCI 1 bearer is presnt */
         WR_CHK_IS_QCI_ONE_BEARER_PRES(ueCb, isQci1BearPres);

         if ( ROK != wrHoPrcB2UtraEvnt(ueCb, &status, &hoType, measRslt,arfcn,\
                  hoNghInfo, isQci1BearPres))
         {
            RLOG0(L_ERROR, "Processing of B2 Event unsuccessful. Ignoring "
                  "Measurement Report");
         }
         else
         {
            if(isQci1BearPres == TRUE)
            {
               WrUtraNeighCellCb          *utraNeighCell;
               utraNeighCell = (WrUtraNeighCellCb*)hoNghInfo->neighCellCb;

               if((utraNeighCell->isVoipCapable == TRUE) &&
                  (TRUE == wrUmmIsUeVoipCapable(ueCb)))

               {
                  isVoipSupported = TRUE;
               }
               if((isVoipSupported == FALSE) || 
                 ((isVoipSupported == TRUE) && (utraNeighCell->isPsHOCapable == FALSE)))
               {
                  isSrvccSupported = wrUmmValidateSrvccForUtra(ueCb, WR_UTRA_MODE_FDD);
                  if(isSrvccSupported == TRUE)
                  {
                     status = WR_HO_DEC_SUCCESS;
                     hoType = WR_HO_TYPE_IRAT_UTRA;
                     hoNghInfo->t.iRatUtraNghInfo.srvccHoInd.pres = PRSNT_NODEF;
                     if(utraNeighCell->isPsHOCapable == TRUE) 
                     {
                        hoNghInfo->t.iRatUtraNghInfo.srvccHoInd.val = \
                                                                      SztSRVCCHOIndpSandCSEnum;
                     }
                     else
                     {
                        hoNghInfo->t.iRatUtraNghInfo.srvccHoInd.val = \
                                                                      SztSRVCCHOIndcSonlyEnum;
                     }
                  }
                  else
                  {
                     /* In this scenario EnodeB should trigger redirection, since redirection
                      * is not supported in case of WCDMA, hence ignoring this measurement
                      * report */
                     status = WR_HO_DEC_FAILURE;
                     RLOG0(L_INFO, "SRVCC not supported by UE/CNE so ignoring measurement report");
                  }
               }
            }/*if(isQci1BearPres == TRUE)*/
            /* TODO : fill hoNghInfo */
         }
         wrHoFillHoDecRslt(hoNghInfo->t.iRatUtraNghInfo.psc, status, hoType, hoDecRslt);
         break;
      }
      case WR_UMM_MEAS_EVENT_B2_GERAN:
      {
         /* Check if QCI 1 bearer is present. if it is present, find the neighbour GERAN
          * cell. See if the neighbour GERAN cell is DTM capable. If it is not DTM capable,
          * do SRVCC HO with CS Only capable and set psSrvcNotAvlbl to 1 */

         WrUmmGeranCellCb         *nghGeranCell = NULLP;
         /* Check if QCI 1 bearer is presnt */
         WR_CHK_IS_QCI_ONE_BEARER_PRES(ueCb, isQci1BearPres);

         /* Check if UE is capabable of SRVCC HO, check for ueCapability bit 9 */
         isSrvccSupported = wrUmmValidateSrvccForGeran(ueCb);

         if(isQci1BearPres)
         {
            RLOG1(L_DEBUG,"QCI 1 bearer Present for UE crnti %d\n", ueCb->crnti);
            if(isSrvccSupported)
         {
            /* Find the neighbour GERAN Cell */
            if ( ROK != wrUmmGetGeranNbhCell(ueCb,measRslt, hoNghInfo))
            {
               status = WR_HO_DEC_FAILURE;
               RLOG0(L_ERROR, "Failed to Get the valid Geran Neighbour Cell ");
            }
            else
            {
               RLOG1(L_INFO, "UE is SRVCC capable, Triggering SRVCC Procedure for %d\n", ueCb->crnti);
               /* Check if neighbour GERAN cell is DTM capable */
               nghGeranCell = (WrUmmGeranCellCb *)hoNghInfo->neighCellCb;
                  status = WR_HO_DEC_SUCCESS;
                  hoType = WR_HO_TYPE_IRAT_GERAN;
                  hoNghInfo->t.iRatGeranNghInfo.srvccHoInd.pres = PRSNT_NODEF;
                  hoNghInfo->t.iRatGeranNghInfo.srvccHoInd.val = 
                     SztSRVCCHOIndcSonlyEnum;
               if((nghGeranCell->isDtmCapable == FALSE) || 
                  (ueCb->csfbInfo.ueDTMCap == FALSE))
               {
                  hoNghInfo->t.iRatGeranNghInfo.psSrvcNotAvlbl = TRUE;
                  RLOG0(L_INFO, "GERAN cell or UE is not DTM capable, triggering "
                        "SRVCC HO with CS ONLY and setting PS Service Not"
                        "Available to TRUE");
               }
               else
               {
                  hoNghInfo->t.iRatGeranNghInfo.psSrvcNotAvlbl = FALSE;
                  RLOG0(L_INFO, "Neighbour GERAN cell or UE is DTM capable, "
                        "hence setting PS Service Not Available to FALSE");
               }
            }
         }
         else
         {
               /* Do the re direction */
               status = WR_HO_DEC_REDIR;
               RLOG0(L_INFO, "UE/MME is not capable of SRVCC hence HO to "
                     "GERAN is not possible, triggering redirection");
            }
         }
         else
         {
            status = WR_HO_DEC_REDIR;
            RLOG0(L_INFO, "QCI 1 beare is not present, "
                  "hence HO to GERAN is not possible");
         }
         /* Fill the HO decode result */
         wrHoFillHoDecRslt(0, status, hoType, hoDecRslt);
         break;
      }
      default:
      {
         /* ignore */
         break;
      }
   } /* end of switch */

   RETVALUE(ROK);
} /* end of wrHandoverDecision */

/* This Function is used to handle Measurement Report message received from   */
/* Measurement Module. Then apply Handover decision Procedure to take the     */ 
/* decision*/
PUBLIC S16 wrUmmHoHdlMeasRprt
(
 WrUeCb                       *ueCb,
 WrMeasRptCfgType             event,
 NhuMeasResults               *measRprt,
 U32                          arfcn
 )
{
   WrUmmIntCrHoTran          *createHoTrans = NULLP;
   WrUmmIncMsg               *msg = NULLP;
   WrUmmCellCb               *cellCb = NULLP;
   WrUmmInternalMsg          *internalMsg = NULLP;
   WrHoDecRslt               hoDecRslt= {0};
   WrHoNghInfo               hoNghInfo = {0};
#ifdef WR_RSYS_KPI 
   U16                        servArfcn; /*Added for KPI*/
#endif/*WR_RSYS_KPI*/
   /* currently ignoring the second A5 report */
   if( WR_HO_NORMAL == ueCb->mobCtrlState)
   {
      wrHandoverDecision(ueCb, event, measRprt, arfcn, &hoDecRslt, &hoNghInfo);
      if (WR_HO_DEC_SUCCESS == hoDecRslt.status)
      {
         /*Reset INACTIVITY TIMER by setting dataRcvd flag to TRUE*/
         wrIfmDamSetDataRcvdFlag(ueCb->cellId, ueCb->crnti);

         WR_ALLOC(&createHoTrans, sizeof(WrUmmIntCrHoTran));
         if (NULLP == createHoTrans)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         wrUmmHoFillCreateHoTrans(createHoTrans, hoDecRslt.val.typeOfHo.hoType, ueCb, &hoNghInfo);
         if (ROK != wrUmmBldInternalMsg(&internalMsg, WR_UMM_INTMSG_CR_HO_TRANS, createHoTrans))
         {
            RLOG0(L_ERROR, "Building internal message error. handover not "
               "triggered");
            RETVALUE(RFAILED);          
         }
         WR_ALLOC(&msg,sizeof(WrUmmIncMsg));
         if (NULLP == msg)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         msg->msgTyp = WR_UMM_INTERNAL_MSG;
         msg->u.internalMsg = internalMsg;
         msg->refCnt = 1;
         /* Get UMM CELL CB.*/
         WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
         if (cellCb == NULLP)
         {
            RLOG0(L_ERROR, "Failed To Get UMM Cell Cb.");
            RETVALUE(RFAILED);
         }
      #ifdef WR_RSYS_KPI 
         /*Added for KPi*/
         servArfcn = wrEmmGetServCarrierFreq(ueCb->cellId); 
         if(servArfcn == arfcn)
         {
           ueCb->freqTyp = INTRA_FREQ;
         }
         else
         {
            if ( WR_UMM_MEAS_EVENT_A5 == event) 
            {
          ueCb->freqTyp = INTER_FREQ;
         } 
            else if ( WR_UMM_MEAS_EVENT_B2_UTRA == event)
            {
               ueCb->freqTyp = INTER_RAT;
            }
         }
          
    #endif /*WR_RSYS_KPI*/
         wrUmmPrcIncMsg(cellCb, ueCb, msg);
     
        /*FIX for ccpu00127513 (MEM_LEAK)*/
         wrUmmRlsIncMsg(&msg);

      } 
      else if (WR_HO_DEC_REDIR == hoDecRslt.status)
      {
         /* Calling redirection, as of now this is applicable only
          * in case of GERAN B2 */
         wrUmmIRatGeranRedirHdlMeasRpt(ueCb, WR_UMM_MEAS_RPTCFG_IRATB2_REDIR,
                                       measRprt);
      }
      else
      {
         RLOG0(L_ERROR, "Cannot Proceed with Handover");
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}/* End of wrUmmHoHdlMeasRprt.*/

/** removed wrUmmHoUpdUeHist
  * as part of UE History info 
  **/


/**
 * @brief This function is used to get the X2 Connection Status from ANR module. 
 *
 * @details
 *
 *     Function: wrIsX2ConExist
 *     Purpose:  This function is used to find the x2 connection exist 
 *               or not between serving cell and target cell.
 
 *     Steps to be performed
 *     =====================
 *         1. Call wrAnrGetX2ItfFlg  provided by ANR module to get X2 Connection Status.
 *         2. Fill the status value and send back to HO module.
 *       
 *     Invoked by: HO Module
 *     
 *  @param[in] U16                   pci,
 *  @param[in] U8                    status,
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/
/**
 * @brief This function processes the ECGI report 
 *
 * @details
 *
 *     Function: wrHoIsEcgiRprtd
 *     Purpose:  This function is used to find ECGI reported or not for particular pci.
 * 
 *     Steps to be performed
 *     ====================
 *        1. Check whether the PCI is present or Not.
 *        2. If pci is present, check whether cgi present or not and return corresponding value.
 *        3. otherwise, return PCI is invalid.
 *       
 *     Invoked by: HO Module
 *  @param[in] NhuMeasResults      *measResults,
 *  @param[in] U16                 pci,
 *  @param[in] U8                  *isEcgiPrsnt

 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/
/**
 * @brief This function is used to find the cell which is belong to CSG or not.
 *
 * @details
 *
 *     Function: wrHoIsServPciBlgToCsg
 *     Purpose:  This function is used to ensure that serving cell and 
 *               Neighbour pci are  belong to same CSG or Not.
 *
 *     Steps to be performed
 *     =====================
 *        1. Get the Serving cell CSG Id and compare with Neighbour Cell CSG Id.
 *        2. If it is same, the cell belong to same CSG.
 * 
 *       
 *     Invoked by: HO Module
 *
 *  @param[in] WrCellCb                    *cellcb,
 *  @param[in] NhuMeasResults              *measResults,
 *  @param[in] U8                          *isServPciBlgToCsg,
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/

/**
 * @brief This function is used to find the cell which is belong to CSG or not.
 *
 * @details
 *
 *     Function: wrHoIsPciBlgToCsg
 *     Purpose:  This function is used to ensure that cell which is belong to CSG or Not.
 *
 *     Steps to be performed
 *     =====================
 *        1. Check wether CSG member information is present or not.
 *        2. If it is present, the cell belong to CSG.
 * 
 *       
 *     Invoked by: HO Module
 *
 *  @param[in] NhuMeasResults        *measResults,
 *  @param[in] U16                   *isPciBlgToCsg,
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/
/**
 * @brief This function is used to find the X2 is prohibited  or not.
 *
 * @details
 *
 *     Function: wrHoIsX2Prohibited
 *     Purpose:  This function is used to ensure that X2 is prohibited or Not.
 *
 *     Steps to be performed
 *     =====================
 * 
 *       
 *     Invoked by: HO Module
 *  
 *  @param[in] U16                   pci,
 *  @param[in] U16                   *isProhibited,
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/
/**
 * @brief This function is used to find the Neighbour PCI belong 
 *         to same MME group or not.
 *
 * @details
 *
 *     Function: wrHoIsPciBlgToSameMMEGroup
 *     Purpose:  This function is used to ensur the Neighbour pci is belong to same mme group or not.
 *
 *     Steps to be performed
 *     =====================
 * 
 *       
 *     Invoked by: HO Module
 *  
 *  @param[in] U16                   pci,
 *  @param[in] U16                   *isProhibited,
 *  @return  S16
 *           -# ROK
 *           -# RFAILED
 *
 **/


/* HO_RLC_PDCP_CFG */
/**
 * @brief This function is used to parse and store the PDCP RLC configuration
 *        from RRC Container (Source to Target Container)
 *
 * @details
 *    Function: wrUmmHoGetPdcpRlcCfgInfo
 *
 *    Processing steps:
 *       - Parse the PDCP RLC configuration from ASN IEs and store in
 *         the transaction specific parameters
 *
 * @param[in] srcDrbInfo            : DRB config from RRC Container
 * @param[out] srcPdcpRlcCfgParams  : DRB config stored in Target Transaction
 * @return S16
 *    -#Success : ROK
 *    -#Failure : ROK
 */
PRIVATE S16 wrUmmHoGetPdcpRlcCfgInfo
(
WrUeCb                       *ueCb,
NhuDRB_ToAddMod              *srcDrbInfo,
WrRabCfgParams               *srcPdcpRlcCfgParams
)
{
   WrPdcpCfgParams           *pdcp;
   WrRlcCfgParams            *rlc;
   NhuPDCP_Config            *srcPdcp;
   NhuRLC_Config             *srcRlc;
   NhuUL_AM_RLC              *ulAm;
   NhuDL_AM_RLC              *dlAm;
   NhuUL_UM_RLC              *ulUm;
   NhuDL_UM_RLC              *dlUm;

   /* RLC Parameters */
   rlc  = &srcPdcpRlcCfgParams->rlcParams;
   srcRlc = &srcDrbInfo->rlc_Config;
   if(srcRlc->choice.val == RLC_CONFIG_AM)
   {
      srcPdcpRlcCfgParams->rlcMode = WR_RLC_AM_MODE;
      srcPdcpRlcCfgParams->rlcDir  = WR_DIR_DL_UL;

      ulAm = &(srcRlc->val.am.ul_AM_RLC);
      rlc->u.am.ul.pollRetxTmr     = ulAm->t_PollRetransmit.val;
      rlc->u.am.ul.pollPdu         = ulAm->pollPDU.val;
      rlc->u.am.ul.pollByte        = ulAm->pollByte.val;
      rlc->u.am.ul.maxRetxThrsh    = ulAm->maxRetxThreshold.val;

      dlAm = &(srcRlc->val.am.dl_AM_RLC);
      rlc->u.am.dl.reorderingTmr   = dlAm->t_Reordering.val;
      rlc->u.am.dl.statusProhTmr   = dlAm->t_StatusProhibit.val;
   }
   else if(srcRlc->choice.val == RLC_CONFIG_UM_BI_DIRECTIONAL)
   {
      srcPdcpRlcCfgParams->rlcMode = WR_RLC_UM_MODE;
      srcPdcpRlcCfgParams->rlcDir  = WR_DIR_DL_UL;

      ulUm = &(srcRlc->val.um_Bi_Directional.ul_UM_RLC);
      rlc->u.um.ul.snFldLen       = ulUm->sn_FieldLength.val;

      dlUm = &(srcRlc->val.um_Bi_Directional.dl_UM_RLC);
      rlc->u.um.dl.snFldLen       = dlUm->sn_FieldLength.val;
      rlc->u.um.dl.tReordering    = dlUm->t_Reordering.val;
   }
   else
   {
      RLOG_ARG1(L_ERROR, DBG_CRNTI,ueCb->crnti,"RLC Mode not supported[%d]",
         srcRlc->choice.val);
      RETVALUE(RFAILED);
   }

   /* PDCP Parameters */
   pdcp = &srcPdcpRlcCfgParams->pdcpParams;
   srcPdcp = &srcDrbInfo->pdcp_Config;
   pdcp->discardTimer =  srcPdcp->discardTimer.val;

   if(srcPdcpRlcCfgParams->rlcMode == WR_RLC_AM_MODE)
   {
      pdcp->statusRprtReqd   =  srcPdcp->rlc_AM.statusReportReqd.val;
   }
   else if(srcPdcpRlcCfgParams->rlcMode == WR_RLC_UM_MODE)
   {
      pdcp->pdcpSnSize   =  srcPdcp->rlc_UM.pdcp_SN_Size.val;
   }

   RETVALUE(ROK);
}


/**
 * @brief This function is used to process RRC Handover Preparation
 *        information.
 *
 * @details
 *    Function: wrUmmTgtHoGetDrbId
 *
 *    Processing steps:
 *       - Fetch the DRB ID for the matching ERAB ID from the decoded
 *         RRC container
 *
 * @param[in] ueCb       : UE Control Block
 * @param[in] decCfm     : Decoded RRC container
 * @param[in] eRABId     : ERAB Identity for which DRB ID needs to be fetched
 * @param[out] drbId     : DRB Identity
 * @param[out] lchId     : LCH Identity
 * @return S16
 *    -#Success : ROK
 *    -#Failure : ROK
 */
PUBLIC S16 wrUmmTgtHoGetLchIdDrbId
(
WrUeCb                       *ueCb,
NhuDecodeSduCfm              *decCfm,
U32                          eRABId,
U8                           *drbId,
U8                           *lchId,
WrRabCfgParams               *srcPdcpRlcCfgParams
)
{
   NhuHovrPrepInfo           *hoPrepInfo = &decCfm->ie.nhuHovrPrepInfo;
   NhuHovrPrepInfo_r8_IEs    *r8Ies;
   NhuDRB_ToAddModLst        *drbToAddModLst;
   NhuDRB_ToAddMod           *srcDrbInfo;
   NhuRadioResourceConfigDedicated *srcRadRsrcCfg;
   U8                        idx1;

   r8Ies = &hoPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8;
   srcRadRsrcCfg = &r8Ies->as_Config.sourceRadioResourceConfig;
   drbToAddModLst = &srcRadRsrcCfg->drb_ToAddModLst;

   for(idx1 = 0; idx1 < drbToAddModLst->noComp.val; idx1++)
   {
      srcDrbInfo = &drbToAddModLst->member[idx1];
      if(eRABId == srcDrbInfo->eps_BrIdentity.val)
      {
         (*drbId) = srcDrbInfo->drb_Identity.val;
         (*lchId) = srcDrbInfo->logicalChannelIdentity.val;
         wrUmmSetLch(ueCb, (*lchId));
         wrUmmSetDrb(ueCb, (*drbId));
         /* HO_RLC_PDCP_CFG */
         wrUmmHoGetPdcpRlcCfgInfo(ueCb, srcDrbInfo, srcPdcpRlcCfgParams);

         RETVALUE(ROK);
      }
   }

   RETVALUE(RFAILED);
}

/**
 * @brief This function is used to process RRC Handover Preparation
 *        information.
 *
 * @details
 *    Function: wrUmmTgtHoPrcHoPrepInfo
 *
 *    Processing steps:
 *       - Store the UE information and trigger decoding of UE capabilities.
 *
 * @param[in] ueCb       : UE Control Block
 * @param[out] ueCap      : UE Capability information
 * @param[in] hoPrepInfo : RRC decoded information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : ROK
 */
PRIVATE S16 wrUmmTgtHoPrcHoPrepInfo
(
 WrUmmTransCb                 *transCb,
WrUeCapInfo                  **ueCap,
NhuDecodeSduCfm              *decCfm
)
{
   NhuDecodeSduReq           *nhuDecodeSduReq = NULLP;
   Buffer                    *mBuf = NULLP;
   U16                       cnt;
   NhuUE_CapRAT_ContLst      *ueContLst;
   NhuHovrPrepInfo_r8_IEs    *r8Ies;
   Bool                      eutraCapPres = FALSE;
   NhuHovrPrepInfo           *hoPrepInfo = &decCfm->ie.nhuHovrPrepInfo;
   WrUeCb                    *ueCb = transCb->ueCb;
   NhuMeasConfig             *srcMeasCfg;
   /* HO_REEST */
   NhuSecAlgorithmConfig     *srcSecCfg = NULLP;
   U32                       idx;

   RLOG_ARG0(L_DEBUG, DBG_CRNTI,ueCb->crnti,"Processing RRC handover preparation"
         " information");

   /* store UE Capabilities */
   WR_ALLOCEVNT(ueCap,sizeof(WrUeCapInfo));

   if(*ueCap == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   r8Ies = &hoPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8;
   ueContLst = &r8Ies->ue_RadioAccessCapInfo;

   /* RRC-REL10-Upgrade Start*/
   if(r8Ies->nonCriticalExtn.pres.pres == PRSNT_NODEF)
   {
      /* Check the presence of ue-ConfigRelease-r9 IE in the container received 
       * from source eNodeB (i.e. intra LTE HO) and set the full configuration 
       * flag in UMM UE control block to TRUE only if the value of 
       * ue-ConfigRelease-r9 IE is greater than release 10 enum. */
      if(r8Ies->nonCriticalExtn.ue_ConfigRls_r9.pres == PRSNT_NODEF)
      {
         if(r8Ies->nonCriticalExtn.ue_ConfigRls_r9.val > 
               NhuHovrPrepInfo_v920_IEsue_ConfigRls_r9rel10Enum)
         {
            ueCb->isFullCfgReqd = TRUE;
         }
      }    
   }

   if(r8Ies->as_Config.pres.pres == PRSNT_NODEF)
   {
      WrUmmEutraHoTgtInfo       *tmpEutraHoTgtInfo;
      if(WR_UMM_X2_TGT_HO_TRANS == transCb->transTyp)
      {
         tmpEutraHoTgtInfo = &(transCb->u.x2TgtHoTrans.eutraTgtInfo);
      }
      else
      {
         tmpEutraHoTgtInfo = &(transCb->u.s1TgtHoTrans.u.eutraTgtInfo);
      }
      if(r8Ies->as_Config.sourceRadioResourceConfig.pres.pres == PRSNT_NODEF)
      {
         /* Check the presence of rlf-TimersAndConstans-r9 IE and set the flag
          * isRlfTmrConstR9Pres in TgtHoTrans CB  to TRUE. */
         if(r8Ies->as_Config.sourceRadioResourceConfig.extaddgrp_1.pres.pres == PRSNT_NODEF)
         {
            tmpEutraHoTgtInfo->isRlfTmrConstR9Pres = TRUE;
         }
         /* Check the presence of MeasSubframePatternPCell-r10 and set the flag 
          * isMeasSubFrPatPCellR10Pres in TgtHoTrans CB to TRUE. */
         if(r8Ies->as_Config.sourceRadioResourceConfig.extaddgrp_2.pres.pres == PRSNT_NODEF)
         {
            tmpEutraHoTgtInfo->isMeasSubFrPatPCellR10Pres = TRUE;
         }
         /* Check the presence of sourceSCellConfigList-r10 IE. */
         if(r8Ies->as_Config.extaddgrp_2.pres.pres == PRSNT_NODEF) 
         {
            /* cpy r8Ies->as_Config.extaddgrp_2.sourceSCellConfigLst_r10->sCellIdx_r10 
             * to sourceSCellRelLst_r10->member.val  noComp should be same in both IE's*/
            /* 1. Store sCellIndex-r10 from SCellToAddModList-r10 in TgtHoTrans CB 
             * 2. Set needToFillSCellToRlsLstR10 in TgtHoTrans CB TRUE 
             * if ue-ConfigRelease-r9 has value release 10 enum */
            tmpEutraHoTgtInfo->sourceSCellRelLst_r10.noComp.val = 
               r8Ies->as_Config.extaddgrp_2.sourceSCellConfigLst_r10.noComp.val;

            WR_ALLOC(&(tmpEutraHoTgtInfo->sourceSCellRelLst_r10.member),
                  (sizeof(NhuSCellIdx_r10) * 
                   tmpEutraHoTgtInfo->sourceSCellRelLst_r10.noComp.val));

            for(idx = 0; idx < tmpEutraHoTgtInfo->sourceSCellRelLst_r10.noComp.val; idx++)
            {
               wrFillTknU32(&(tmpEutraHoTgtInfo->sourceSCellRelLst_r10.member[idx]), 
                     r8Ies->as_Config.extaddgrp_2.sourceSCellConfigLst_r10.member[idx].sCellIdx_r10.val);
            }
            tmpEutraHoTgtInfo->needToFillSCellToRlsLstR10 = TRUE;
         }
      }
   }
   /* RRC-REL10-Upgrade end */

   for(cnt = 0; cnt < ueContLst->noComp.val; cnt++)
   {
      switch(ueContLst->member[cnt].rat_Typ.val)
      {
         case  NhuRAT_TypeutraEnum:
            {
               /* Store EUTRA UeCap Ie Octet */
               WR_GET_MEM(&(*ueCap)->memCp, ueContLst->member[cnt].ueCapRAT_Cont.len, &((*ueCap)->ueEutraOctStr.val));
               if((*ueCap)->ueEutraOctStr.val == NULLP)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  wrUmmDeAllocUeCap(*ueCap);
                  RETVALUE(RFAILED);
               }

               cmMemcpy((*ueCap)->ueEutraOctStr.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.len);
               (*ueCap)->ueEutraOctStr.pres = PRSNT_NODEF;
               (*ueCap)->ueEutraOctStr.len =
                  ueContLst->member[cnt].ueCapRAT_Cont.len;
               eutraCapPres = TRUE;
               break;
            }
         case NhuRAT_Typcdma2000_1XRTTEnum:
            {
               /* Store CDMA2000 1xRTT UeCap Ie Octet */
               WR_GET_MEM(&(*ueCap)->memCp, ueContLst->member[cnt].ueCapRAT_Cont.len, &((*ueCap)->ueCdmaOctStr.val));
               if ((*ueCap)->ueCdmaOctStr.val == NULLP)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  wrUmmDeAllocUeCap(*ueCap);
                  RETVALUE(RFAILED);
               }
               cmMemcpy((*ueCap)->ueCdmaOctStr.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.len);
               (*ueCap)->ueCdmaOctStr.pres = PRSNT_NODEF;
               (*ueCap)->ueCdmaOctStr.len =
                  ueContLst->member[cnt].ueCapRAT_Cont.len;
               break;
            }
         case NhuRAT_Typgeran_csEnum:
            {
               /* Store GERAN-CS UeCap Ie Octet */
                WR_GET_MEM(&(*ueCap)->memCp, ueContLst->member[cnt].ueCapRAT_Cont.len, 
                       &((*ueCap)->ueGeranCsOctStr.val));
               if ((*ueCap)->ueGeranCsOctStr.val == NULLP)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  wrUmmDeAllocUeCap(*ueCap);
                  RETVALUE(RFAILED);
               }
               cmMemcpy((*ueCap)->ueGeranCsOctStr.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.len);
               (*ueCap)->ueGeranCsOctStr.pres = PRSNT_NODEF;
               (*ueCap)->ueGeranCsOctStr.len =
                  ueContLst->member[cnt].ueCapRAT_Cont.len;
               break;
            }
         case NhuRAT_Typgeran_psEnum:
            {
               /* Store GERAN-PS UeCap Ie Octet */
                WR_GET_MEM(&(*ueCap)->memCp, ueContLst->member[cnt].ueCapRAT_Cont.len, 
                      &((*ueCap)->ueGeranPsOctStr.val));
               if ((*ueCap)->ueGeranPsOctStr.val == NULLP)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  wrUmmDeAllocUeCap(*ueCap);
                  RETVALUE(RFAILED);
               }
               cmMemcpy((*ueCap)->ueGeranPsOctStr.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.len);
               (*ueCap)->ueGeranPsOctStr.pres = PRSNT_NODEF;
               (*ueCap)->ueGeranPsOctStr.len =
                  ueContLst->member[cnt].ueCapRAT_Cont.len;
               break;
            }
      }
   }
   if(eutraCapPres == TRUE)
   {
      /* Request RRC to decode the UE Radio capability IE                    */
      WR_ALLOC(&nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
      if(NULLP == nhuDecodeSduReq)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }/* End of if condition.*/
      nhuDecodeSduReq->hdr.cellId  =   ueCb->cellId;
      nhuDecodeSduReq->hdr.ueId    =   ueCb->crnti;
      nhuDecodeSduReq->hdr.transId =   transCb->transId;
      nhuDecodeSduReq->sduType     =   NHU_DEC_SDUTYPE_EUTRA_UECAP_IE;
      WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, &mBuf);

      if(mBuf == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      /* convert the OSXL to Buffer */
      SAddPstMsgMult((Data *)((*ueCap)->ueEutraOctStr.val),
            (MsgLen)((*ueCap)->ueEutraOctStr.len),
            (Buffer *)mBuf);
      nhuDecodeSduReq->mbuf = mBuf;

      wrIfmRrcSndDecodeReq(nhuDecodeSduReq);
   }
   /* For interRAT HO AS-config, RRM config and As-context shall not present*/
   if(( WR_UMM_X2_TGT_HO_TRANS == transCb->transTyp) ||
         (( WR_UMM_S1_TGT_HO_TRANS == transCb->transTyp) && 
          ( SztHovrTypintralteEnum == transCb->u.s1TgtHoTrans.sztHovrTyp)))
   {
      /* HO_REEST */
      /* Add the UE CRNTI info in Handover UE List */
      ueCb->hoReestInfo.srcUeCrnti = ((r8Ies->as_Config.sourceUE_Identity.val[0] << 8) |
            (r8Ies->as_Config.sourceUE_Identity.val[1]));
      if(r8Ies->as_Ctxt.reestablishmentInfo.pres.pres == PRSNT_NODEF)
      {
         ueCb->hoReestInfo.pci        = r8Ies->as_Ctxt.reestablishmentInfo.sourcePhysCellId.val;
         /* TODO:
            ueCb->hoReestInfo.shortMacIe = ((r8Ies->as_Config.targetCellShortMAC_I.val[0] << 8) |
            (r8Ies->as_Config.targetCellShortMAC_I.val[1]));
            */
      }
      /* store the source measurement ID's */
      srcMeasCfg = &r8Ies->as_Config.sourceMeasConfig;
      for(idx = 0; idx < srcMeasCfg->measIdToAddModLst.noComp.val; idx++)
      {
         ueCb->hoSrcMeasIdLst[idx] = 
            srcMeasCfg->measIdToAddModLst.member[idx].measId.val;
      }
      ueCb->hoSrcMeasIdLstCnt = idx;

      for(idx = 0; idx < srcMeasCfg->measObjectToAddModLst.noComp.val; idx++)
      {
         ueCb->hoSrcMeasObjIdLst[idx] = 
            srcMeasCfg->measObjectToAddModLst.member[idx].measObjectId.val;
      }
      ueCb->hoSrcMeasObjIdLstCnt = idx;

      for(idx = 0; idx < srcMeasCfg->reportConfigToAddModLst.noComp.val; idx++)
      {
         ueCb->hoSrcRprtIdLst[idx] = 
            srcMeasCfg->reportConfigToAddModLst.member[idx].reportConfigId.val;
      }
      ueCb->hoSrcRprtIdLstCnt = idx;
      /* Store the source Security configuration */
      /* HO_REEST */
      srcSecCfg = &r8Ies->as_Config.sourceSecAlgorithmConfig; 
      if((srcSecCfg != NULLP) && (srcSecCfg->pres.pres == PRSNT_NODEF))
      { 
         ueCb->hoReestInfo.srcIntAlgo  = srcSecCfg->integrityProtAlgorithm.val; 
         ueCb->hoReestInfo.srcCiphAlgo = srcSecCfg->cipheringAlgorithm.val;
         /* REL-8 UE, null algo = spare7 */
         if ( ueCb->hoReestInfo.srcIntAlgo == NhuSecAlgorithmConfigintegrityProtAlgorithmspare1Enum )
         {
            ueCb->hoReestInfo.srcIntAlgo  = 0;
         }
      }
   }
   RETVALUE(ROK);
}

PUBLIC S16 wrUmmHoCopyRadioResCfg
(
RmuUeRadioResCfg   *rmuUeRadioResCfg,
RmuUeRadioResCfg   *txsUeRadioResCfg
)
{
	rmuUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stAPrdCqiRepCfg.enbld
	     = txsUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stAPrdCqiRepCfg.enbld;
	rmuUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stAPrdCqiRepCfg.enPuschReportMode
	     = txsUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stAPrdCqiRepCfg.enPuschReportMode;
	rmuUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.enCqiFormatInd
	     = txsUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.enCqiFormatInd;
	rmuUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.enPucchReportMode
	     = txsUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.enPucchReportMode;
	rmuUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.usCqiResourceIndex
	     = txsUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.usCqiResourceIndex;
	rmuUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.usCqiConfigIndex
	     = txsUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.usCqiConfigIndex;
   rmuUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.riConfigIndex.usRiConfigIndex
        = txsUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.riConfigIndex.usRiConfigIndex;
	rmuUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.bFactorK
	     = txsUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.bFactorK;
	rmuUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.bSimulAckNackCqi
	     = txsUeRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.bSimulAckNackCqi;
/* Removed DRX flag */ 
   rmuUeRadioResCfg->stMacMainCfg.stRmDrxCfg.bDrxOnDurTmr = 
      txsUeRadioResCfg->stMacMainCfg.stRmDrxCfg.bDrxOnDurTmr;
   rmuUeRadioResCfg->stMacMainCfg.stRmDrxCfg.usDrxInactvTmr = 
      txsUeRadioResCfg->stMacMainCfg.stRmDrxCfg.usDrxInactvTmr;
   rmuUeRadioResCfg->stMacMainCfg.stRmDrxCfg.bDrxRetxTmr = 
      txsUeRadioResCfg->stMacMainCfg.stRmDrxCfg.bDrxRetxTmr;
   rmuUeRadioResCfg->stMacMainCfg.stRmDrxCfg.stDrxShortDrx.usShortDrxCycle = 
      txsUeRadioResCfg->stMacMainCfg.stRmDrxCfg.stDrxShortDrx.usShortDrxCycle;
   rmuUeRadioResCfg->stMacMainCfg.stRmDrxCfg.stDrxShortDrx.bDrxShortCycleTmr = 
      txsUeRadioResCfg->stMacMainCfg.stRmDrxCfg.stDrxShortDrx.bDrxShortCycleTmr;
   rmuUeRadioResCfg->stMacMainCfg.stRmDrxCfg.stDrxLongCycleOffst.usDrxStartOffst = 
      txsUeRadioResCfg->stMacMainCfg.stRmDrxCfg.stDrxLongCycleOffst.usDrxStartOffst;
/* Removed DRX flag */ 
   RETVALUE(ROK);
}

PUBLIC S16 wrUmmHoFillRadioResCfg
(
RmuUeRadioResCfg   *ueRadioResCfg,
NhuHovrPrepInfo    *nhuHovrPrepInfo
)
{
   NhuCQI_ReportConfig *crCfg;
/* Removed DRX flag */ 
   NhuRadioResourceConfigDedicatedmac_MainConfig *macCfg;

   crCfg = &nhuHovrPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.as_Config.sourceRadioResourceConfig.physicalConfigDedicated.cqi_ReportConfig;

   if(crCfg->pres.pres == PRSNT_NODEF)
   {
       if (crCfg->cqi_ReportModeAperiodic.pres == PRSNT_NODEF)
       {
          ueRadioResCfg->stPhyCfgDed.stCqiRepCfg.stAPrdCqiRepCfg.enbld= TRUE;
          ueRadioResCfg->stPhyCfgDed.stCqiRepCfg.stAPrdCqiRepCfg.enPuschReportMode = \
             (RmuPuschReportMode)crCfg->cqi_ReportModeAperiodic.val;
       }
       if(crCfg->cqi_ReportPeriodic.choice.pres == PRSNT_NODEF &&  
         crCfg->cqi_ReportPeriodic.choice.val == CQI_REPORTPERIODIC_SETUP)
       {
          NhuCQI_ReportPeriodicsetup *setup;
          setup = &(crCfg->cqi_ReportPeriodic.val.setup);
          if (setup->cqi_FormatIndicatorPeriodic.choice.pres == PRSNT_NODEF)
          {
             ueRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.enCqiFormatInd = setup->cqi_FormatIndicatorPeriodic.choice.val;
          }
          if (setup->cqi_PUCCH_ResourceIdx.pres == PRSNT_NODEF)
          {
             ueRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.usCqiResourceIndex = setup->cqi_PUCCH_ResourceIdx.val;
          }
          if (setup->cqi_pmi_ConfigIdx.pres == PRSNT_NODEF)
          {
             ueRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.usCqiConfigIndex = setup->cqi_pmi_ConfigIdx.val;
          }
          if (setup->ri_ConfigIdx.pres == PRSNT_NODEF)
          {
             ueRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.riConfigIndex.usRiConfigIndex = setup->ri_ConfigIdx.val;
          }
          if ((setup->cqi_FormatIndicatorPeriodic.val.subbandCQI.pres.pres == PRSNT_NODEF) &&
              (setup->cqi_FormatIndicatorPeriodic.val.subbandCQI.k.pres == PRSNT_NODEF))
          {
             ueRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.bFactorK = setup->cqi_FormatIndicatorPeriodic.val.subbandCQI.k.val;
          }
          if (setup->simultaneousAckNackAndCQI.pres == PRSNT_NODEF)
          {
             ueRadioResCfg->stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.bSimulAckNackCqi = setup->simultaneousAckNackAndCQI.val;
          }

          /* Set the bitmap for bRadioResDed and PhyCfgDedRes */
          ueRadioResCfg->bRadioResDedBitMap |= RM_RCM_PHY_DED_RES_CFG;
          ueRadioResCfg->stPhyCfgDed.bPhyCfgDedResBitMap |= RM_RCM_PHY_DED_RES_CFG_CQI;
       }
   }
   
   macCfg = &nhuHovrPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.as_Config.sourceRadioResourceConfig.mac_MainConfig;
   if(macCfg->choice.pres == PRSNT_NODEF &&  
      macCfg->choice.val == MAC_MAINCONFIG_EXPLICITVALUE)
   {
       if(macCfg->val.explicitValue.drx_Config.choice.pres == PRSNT_NODEF &&  
          macCfg->val.explicitValue.drx_Config.choice.val == DRX_CONFIG_SETUP)
       {
          NhuDRX_Configsetup* setup;
          setup = &macCfg->val.explicitValue.drx_Config.val.setup;
          if (setup->pres.pres == PRSNT_NODEF)
          {
             if (setup->onDurationTimer.pres == PRSNT_NODEF)
             {
                ueRadioResCfg->stMacMainCfg.stRmDrxCfg.bDrxOnDurTmr = setup->onDurationTimer.val;
             }
             if (setup->drx_InactivityTimer.pres == PRSNT_NODEF)
             {
                ueRadioResCfg->stMacMainCfg.stRmDrxCfg.usDrxInactvTmr = setup->drx_InactivityTimer.val;
             }
             if (setup->drx_ReTxTimer.pres == PRSNT_NODEF)
             {
                ueRadioResCfg->stMacMainCfg.stRmDrxCfg.bDrxRetxTmr = setup->drx_ReTxTimer.val;
             }
             if (setup->shortDRX.pres.pres == PRSNT_NODEF)
             {
                if (setup->shortDRX.shortDRX_Cycle.pres == PRSNT_NODEF)
                {
                   ueRadioResCfg->stMacMainCfg.stRmDrxCfg.stDrxShortDrx.usShortDrxCycle = setup->shortDRX.shortDRX_Cycle.val;
                }   
                if (setup->shortDRX.drxShortCycleTimer.pres == PRSNT_NODEF)
                {
                   ueRadioResCfg->stMacMainCfg.stRmDrxCfg.stDrxShortDrx.bDrxShortCycleTmr = setup->shortDRX.drxShortCycleTimer.val;
                }   
             }   
             if (setup->longDRX_CycleStartOffset.val.sf10.pres == PRSNT_NODEF)
             {
                ueRadioResCfg->stMacMainCfg.stRmDrxCfg.stDrxLongCycleOffst.usDrxStartOffst = setup->longDRX_CycleStartOffset.val.sf10.val;
             }
          }   
       }
   }
/*Removed DRX flag*/
   RETVALUE(ROK);
}

/**
 * @brief This function is used to process RRC Decode Confirm.
 *
 * @details
 *    Function: wrUmmHoTgtPrcRrcDecCfm
 *
 *    Processing steps:
 *       - Based on decode type, trigger corresponding handler
 *
 *
 * @param[in] transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmHoTgtPrcRrcDecCfm
(
WrUmmTransCb                 *transCb,
WrUeCapInfo                  **ueCap
)
{
   U8                        subTrans;
   /* SPS changes starts */
   S16                       ret = ROK;
   /* SPS changes ends */
   NhuDecodeSduCfm           *decCfm = transCb->msg->u.decCfm;

   subTrans = decCfm->hdr.transId & 0x0f;

   if(decCfm->status)
   {
      RLOG1(L_ERROR, "Decoding Failure of[%d]", subTrans);
      RETVALUE(RFAILED);
   }

   switch(decCfm->sduType)
   {
      case NHU_MSG_HOPREPINFO:
      {
         if (ROK != wrUmmTgtHoPrcHoPrepInfo(transCb, ueCap, decCfm))
         {
            RLOG0(L_ERROR, "Failed to process handover preparation decode "
               "confirm");
            RETVALUE(RFAILED);
         }
         break;
      }
      case NHU_DEC_SDUTYPE_EUTRA_UECAP_IE:
      {
         /* SPS changes starts */
         /* The ret value WR_UMM_NHU_MSG_UECAPINFO_V9A0_DEC_SENT means the decoding
          * request for Release 9A0 extention is sent to RRC for decoding */
         if(((ret = wrUmmUecapEutraIeDecCfmHdl(transCb->ueCb, (*ueCap), decCfm)) != ROK) && \
               (ret != WR_UMM_NHU_MSG_UECAPINFO_V9A0_DEC_SENT))
         /* SPS changes ends */
         {
            RLOG1(L_ERROR, "Failed to process UE Capability IE ret = [%d]", ret);
            RETVALUE(RFAILED);
         }
         break;
      }
      /* SPS changes starts */
      case NHU_MSG_UECAPINFO_V9A0:
      {
         /* Handle release 9a extention decoding confirm */
         if (wrUmmUecapEutraIe9a0DecCfmHdl((*ueCap), decCfm) != ROK)
         {
            RLOG0(L_ERROR, "wrUmmHoTgtPrcRrcDecCfm: NHU_MSG_UECAPINFO_V9A0 decode failed\n");
            RETVALUE(RFAILED);
         }
         RLOG0(L_DEBUG, "\nNHU_MSG_UECAPINFO_V9A0 decoded while HO\n");
         break;
      }
      /* SPS changes ends */
      default:
      {
         RLOG1(L_ERROR, "Cannot handle decode for sdu type[%d]", decCfm->sduType);
         RETVALUE(RFAILED);
         break;
      }
   }
   /* SPS changes starts */
   RETVALUE(ret);
   /* SPS changes ends */
}
/**
 * @brief This function is used to send Ho Failure message from umm module
 *
 * @details
 *    Function: wrUmmHoTgtSndHoFail
 *
 *    Processing steps:
 *       - Fill HO Failure Message
 *       - Send HO Failure msg & release request to S1AP Connection
 *
 *
 * @param[in] spConnId        :  spConnId for S1AP SAP
 * @param[in] mme_ue_s1ap_id  :  MME UE S1AP ID
 * @param[in] cause Type      :  Error Cause Type
 * @param[in] cause value     :  Error Cause Value
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmHoTgtSndHoFail 
(
U32                         spConnId,
U32                         mme_ue_s1ap_id,
U32                         cause_Type,
U32                         cause_Val
)
{
   S1apPdu                     *hovrFailure = NULLP;
   SztS1AP_PDU                 *sztS1apPdu = NULLP;
   SztHovrFail                 *sztHovrFail = NULLP;
   SztProtIE_Field_HovrFailIEs *ie = NULLP;
   U16                         numComp;   
   SztRelReq                   relReq = {0};
   S16                         ret = 0;
   
   /* Filling Handover Failure Message.*/
   ret = cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem, (Ptr *)&hovrFailure);
   if (ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   cmMemset((U8 *)&hovrFailure->pdu, 0, sizeof(SztS1AP_PDU));
   /* Fill Interface Elementary Procedures for Handover Failure.*/
   sztS1apPdu = &hovrFailure->pdu;
   wrFillTknU8(&(sztS1apPdu->choice), S1AP_PDU_UNSUCCESSFULOUTCOME);
   wrFillTknU8(&(sztS1apPdu->val.unsuccessfulOutcome.pres), PRSNT_NODEF);
   wrFillTknU32(&(sztS1apPdu->val.unsuccessfulOutcome.procedureCode),Sztid_HovrResourceAllocn);
   wrFillTknU32(&(sztS1apPdu->val.unsuccessfulOutcome.criticality),SztCriticalityrejectEnum);
   sztHovrFail = &sztS1apPdu->val.unsuccessfulOutcome.value.u.sztHovrFail;
   wrFillTknU8(&(sztHovrFail->pres),PRSNT_NODEF);
   numComp = 2;
   wrFillTknU16(&(sztHovrFail->protocolIEs.noComp),numComp);
   if ((cmGetMem(&hovrFailure->memCp,(numComp * sizeof (SztProtIE_Field_HovrFailIEs)),
               (Ptr *)&sztHovrFail->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREE_EVNT(hovrFailure);
      RETVALUE(RFAILED);
   }
   ie = &sztHovrFail->protocolIEs.member[0];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID),mme_ue_s1ap_id);
   /* Fill The Cause.*/
   ie = &sztHovrFail->protocolIEs.member[1];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU32((TknU32 *)&(ie->value.u.sztCause.choice),cause_Type);
   wrFillTknU32((TknU32 *)&(ie->value.u.sztCause.val),cause_Val);
   WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, cause_Type, cause_Val, INC_KPI_VALUE_BY_ONE);

   /* Filling SztConRsp*/
   relReq.connIdType = SZT_CONN_TYPE_SP;
   relReq.relType = SZT_CXT_REL;
   relReq.connId = spConnId;
   relReq.pdu = hovrFailure;
   
   /* Sending Handover Failure Message.*/
   WrIfmS1apRelReq(&relReq);
   RETVALUE(ROK);
}
/* CSG_DEV */
/** @brief This function returns if the incoming handover is for an emergency
 *         call.
 *
 * @details
 *
 *     Function: wrUmmHoIsEmgncyCall
 *
 *         Processing steps:
 *         - check if UE security capability contains only NULL algo
 *           - if yes
 *             - return TRUE
 *           - else
 *             - return FALSE
 *
 * @param [in]  ueCb      : UE Control Block
 * @return S16
 *          -# Success : ROK
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrUmmHoIsEmgncyCall
(
WrUeSecCfg                   *secCfg
)
{
   if(!(secCfg->integCap.val[0] & 0xff &&
            secCfg->encrypCap.val[0] & 0xff))
   {
      RETVALUE(TRUE);
   }

   RETVALUE(FALSE);
}

/** @brief This function is called to store the UE capability
 *             info into the transactio control block.
 *
 * @details
 *
 *     Function: wrUmmHoUeCapStoreCapInfo
 *
 *         Processing steps:
 *         - for the number of RAT information int he message,
 *           - allocate and store the information in the initial context
 *             transaction CB
 *
 * @param[in, out] ueCb   : wrUeCb information 
 * @param[in]     ueCapInfo : UE Capability Information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmHoUeCapStoreCapInfo
(
WrUeCb                       *ueCb,
NhuUECapInfo                 *ueCapInfo
)
{
   U16                      cnt;
   NhuUE_CapRAT_ContLst     *ueContLst;
   WrUeCapInfo              *ueCap = ueCb->ueCap;

   /* If UeCap is received from MME and memory is allocated to store Ue Cap 
    * But CDMA Pdu was not present,in that case eNB sent UE Capability Enquiry message 
    * to UE. After receiving the UE Capability information need to free allocated memory first here */
    if(ueCap != NULLP)
    {
       wrUmmDeAllocUeCap(ueCap);
    }
    WR_ALLOCEVNT(&(ueCb->ueCap), sizeof(WrUeCapInfo));

    ueCap = ueCb->ueCap;
    if (ueCap == NULLP)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
       RETVALUE(RFAILED);
    }
   ueContLst = 
      &ueCapInfo->criticalExtns.val.c1.val.ueCapInfo_r8.ue_CapRAT_ContLst;
   for(cnt = 0; cnt < ueContLst->noComp.val; cnt++)
   {
      switch(ueContLst->member[cnt].rat_Typ.val)
      {
         case  NhuRAT_TypeutraEnum:
            {
               WR_GET_MEM(&ueCap->memCp, ueContLst->member[cnt].ueCapRAT_Cont.len,
                     &ueCap->ueEutraOctStr.val);

               if (ueCap->ueEutraOctStr.val == NULLP)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  wrUmmDeAllocUeCap(ueCap);
                  RETVALUE(RFAILED);
               }
               cmMemcpy(ueCap->ueEutraOctStr.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.len);
               ueCap->ueEutraOctStr.pres = TRUE;
               ueCap->ueEutraOctStr.len =
                  ueContLst->member[cnt].ueCapRAT_Cont.len;
            }
            break;
            /* eCSFB : Added case for CDMA2000_1XRTTEnum, to copy the 
             * CDMA RAT supported information by UE */
         case NhuRAT_Typcdma2000_1XRTTEnum:
            {
               /* Store CDMA2000 1xRTT UeCap Ie Octet */
               WR_GET_MEM(&ueCap->memCp, ueContLst->member[cnt].ueCapRAT_Cont.len, 
                     &ueCap->ueCdmaOctStr.val);
               if (ueCap->ueCdmaOctStr.val == NULLP)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  wrUmmDeAllocUeCap(ueCap);
                  RETVALUE(RFAILED);
               }
               cmMemcpy(ueCap->ueCdmaOctStr.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.len);
               ueCap->ueCdmaOctStr.pres = TRUE;
               ueCap->ueCdmaOctStr.len =
                  ueContLst->member[cnt].ueCapRAT_Cont.len;
            }
            break;
         case NhuRAT_Typgeran_csEnum:
            {
               /* Store GERAN-CS UeCap Ie Octet */
                WR_GET_MEM(&ueCap->memCp, ueContLst->member[cnt].ueCapRAT_Cont.len, 
                       &ueCap->ueGeranCsOctStr.val);
               if (ueCap->ueGeranCsOctStr.val == NULLP)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  wrUmmDeAllocUeCap(ueCap);
                  RETVALUE(RFAILED);
               }
               cmMemcpy(ueCap->ueGeranCsOctStr.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.len);
               ueCap->ueGeranCsOctStr.pres = TRUE;
               ueCap->ueGeranCsOctStr.len =
                  ueContLst->member[cnt].ueCapRAT_Cont.len;
               break;
            }
         case NhuRAT_Typgeran_psEnum:
            {
               /* Store GERAN-PS UeCap Ie Octet */
                WR_GET_MEM(&ueCap->memCp, ueContLst->member[cnt].ueCapRAT_Cont.len, 
                      &ueCap->ueGeranPsOctStr.val);
               if (ueCap->ueGeranPsOctStr.val == NULLP)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  wrUmmDeAllocUeCap(ueCap);
                  RETVALUE(RFAILED);
               }
               cmMemcpy(ueCap->ueGeranPsOctStr.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.len);
               ueCap->ueGeranPsOctStr.pres = TRUE;
               ueCap->ueGeranPsOctStr.len =
                  ueContLst->member[cnt].ueCapRAT_Cont.len;
               break;
            }
         case NhuRAT_TyputraEnum:
            {
               /* Store UTRAN UeCap Ie Octet */
                WR_GET_MEM(&ueCap->memCp, ueContLst->member[cnt].ueCapRAT_Cont.len,
                      &ueCap->ueUtraOctStr.val);
               if (ueCap->ueUtraOctStr.val == NULLP)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  wrUmmDeAllocUeCap(ueCap);
                  RETVALUE(RFAILED);
               }
               cmMemcpy(ueCap->ueUtraOctStr.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.val,
                     ueContLst->member[cnt].ueCapRAT_Cont.len);
               ueCap->ueUtraOctStr.pres = TRUE;
               ueCap->ueUtraOctStr.len =
                  ueContLst->member[cnt].ueCapRAT_Cont.len;
               break;
            }
      }
   }
   RETVALUE(ROK);
}
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
