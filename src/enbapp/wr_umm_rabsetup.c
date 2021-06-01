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
               

     File:     wr_umm_rabsetup.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=64;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_utils.h"
#include "wr_smm_smallcell.h"
#include "wr_emm.h"
#include "wr_umm_msg.h"
#include "wr_umm.h"
#include "wr_umm_rabsetup.h"
#include "wr_umm_trans.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_sch_msg.h"
#include "wr_ifm_schd.h"
#include "wr_ifm_dam.h"
#include "wr_ifm_rrc.h"
#include "wr_ifm_s1ap.h"
#include "wr_ifm_rrm.h"
#include "wr_kpi.h"
#include "wr_umm_drx.h"
#include "wr_emm_mme.h"

#ifdef WR_TEST_CODE
EXTERN U8 wrTrgRrcReConTO;
#endif
#define WR_RAB_SETUP_UE_TRANS_ID      0x0000000f

EXTERN S16 wrS1apFillCause(SztCause *ie,WrUmmMsgCause *cause);
EXTERN S16 wrEmmErabParseStat(WrEmmErabStat updType, 
                              WrEmmErabSubType subType, 
                              WrUmmRabSetupTrans * rabSetup, 
                              WrCellId cellId );
PUBLIC S16 wrUmmDefRabSetupIncCtxtRelProcessor(WrUmmTransCb *transCb, WrUmmTransCb *incTrans);
PUBLIC S16 wrUmmDefRabSetupIncReestProcessor(WrUmmTransCb *transCb, WrUmmTransCb *incTrans);
PUBLIC S16 wrUmmBldUeCfgforRvrt(NhuCfgReqSdus *ueCfg, WrUmmTransCb *transCb);
PUBLIC S16 wrUmmDefRabSetupIncHoProcessor(WrUmmTransCb *transCb, WrUmmTransCb *incTrans);

/* SPS changes starts */
/** @brief This function is responsible for filling the UL SPS LC for SCHD.
 *
 * @details
 *
 *     Function: wrUmmRabSetupSchdUlSpsLchCfgReq
 *
 *         Processing steps:
 *         - Check if SPS ERAB
 *         - Fill LC Id
 *
 * @param[in]  rrmUeRecfgResp : RRM Recfg Resp
 *             numErabs       : Number of ERABs from transCb
 *             pErabList      : Pointer to ERAB list from transCb
 *             ueSpsCfg       : UE SPS Cfg to be filled for SCHD
 * @return Void
 */

PUBLIC Void wrUmmRabSetupSchdUlSpsLchCfgReq
(
 RmuUeRecfgRsp           *rrmUeRecfgResp,
 U32                     numErabs,
 Void                    *pErabList,
 RgrUeSpsCfg             *ueSpsCfg
)
{
   U32 count;
   WrUmmRabSetupRabInfo  *pErabInfo = (WrUmmRabSetupRabInfo *)pErabList;

   ueSpsCfg->ulSpsCfg.lcCnt = 0;
   for (count = 0; count < numErabs; count++)
   {
      if(((pErabInfo+count)->rabAllowed == TRUE) && (rrmUeRecfgResp->stErabAcceptLst[count].bIsUlSpsEnabled == TRUE))
      {
         /* Here Application should take stuffs from the RRM accepted list but currently 
          * there is no RBC support at both APP and RRM. Thus the expectation is, RRM has accepted 
          * all the ERABs in the request and thus have provided ERAB in the same order as sent to it. */
         ueSpsCfg->ulSpsCfg.spsLcInfo[ueSpsCfg->ulSpsCfg.lcCnt].isSpsEnabled = TRUE;
         ueSpsCfg->ulSpsCfg.spsLcInfo[ueSpsCfg->ulSpsCfg.lcCnt].lcId = (pErabInfo+count)->lchId;
         ueSpsCfg->ulSpsCfg.lcCnt++;
         (pErabInfo+count)->isUlSpsEnabled = TRUE;
      }
      else
      {
         (pErabInfo+count)->isUlSpsEnabled = FALSE;
      }
   }
}
/* SPS changes ends */
/*RRM_ARP*/
/** @brief This function is responsible for updating RAB allowed status in
 *         transcb for every ERAB
 *
 * @details
 *
 *     Function: wrUmmUpdateRabSetupTrans 
 *
 *         Processing steps:
 *              - Update rabSetup with rabAllowed status received from RRM for
 *              every RAB 
 *
 * @param[out] rabSetup   : ERAB setup transaction pointer
 * @param[in]  ueRecfgRsp : RRM ue config rsp pointer
 * @param[in]  ueCb       : UE Control block pointer
 * @return Void
 */
PRIVATE Void wrUmmUpdateRabSetupTrans
(
WrUmmRabSetupTrans        *rabSetup,
RmuUeRecfgRsp             *ueRecfgRsp,
WrUeCb                       *ueCb

)
{
   U32       idx,idx1;
   U8        flag = FALSE;
   U8        isGbrExist = FALSE;
   U32       gbrFailCnt = 0;

   rabSetup->numRabsAllowed = ueRecfgRsp->bNoErabsAccepted;

   for(idx = 0; idx < rabSetup->numRabs; idx++)
   {
      for(idx1 = 0; (FALSE == flag) && (idx1 < ueRecfgRsp->bNoErabsAccepted); idx1++)
      {
         if(rabSetup->rabs[idx].erabId == ueRecfgRsp->stErabAcceptLst[idx1].bErabId)
         {
            rabSetup->rabs[idx].rabAllowed = TRUE;
            flag = TRUE;
            if((FALSE == isGbrExist) && 
                  (rabSetup->rabs[idx].qci <= (WR_EMM_ERAB_QCI_4 -QCI_OFFSET)))
            {
               isGbrExist = TRUE;
            }
         }
      }
      
      for(idx1 = 0; (FALSE == flag) && (idx1 < ueRecfgRsp->bNoErabsRejected) ; idx1++)
      {
         if(rabSetup->rabs[idx].erabId == ueRecfgRsp->stErabRejectLst[idx1].bErabId)
         {
            wrUmmRelDrb(ueCb,rabSetup->rabs[idx].drbId);
            rabSetup->rabs[idx].rabAllowed = FALSE;
            rabSetup->rabs[idx].cause.causeTyp = WR_CAUSE_RADIONW;
            rabSetup->rabs[idx].cause.causeVal = ueRecfgRsp->stErabRejectLst[idx1].enCause;
            if(rabSetup->rabs[idx].qci <= (WR_EMM_ERAB_QCI_4 -QCI_OFFSET))
            {
               gbrFailCnt++;
            }
         }
      }
      flag = FALSE;
   }

   /* ANR is not re-started if 
    * 1. Atleast one GBR ERAB is accepted 
    * 2. All the incoming GBR ERABs failed but there exists GBR bearer already
    *    in ue context. */
   if((TRUE == isGbrExist) || 
         ((RAB_ESTABLISHED == ueCb->isGBRBearerExists) && 
         (gbrFailCnt == rabSetup->numRabs)))
   {
      rabSetup->startAnr = FALSE;
   }
   else
   {
      rabSetup->startAnr = TRUE;
   }

   RETVOID;
}
/** @brief This function is responsible for reconfiguring the UE context
 *         at the scheduler.
 *
 * @details
 *
 *     Function: wrUmmRabSetupSchdUeRecfgReq
 *
 *         Processing steps:
 *         - allocate a new configuration request
 *         - set the configuration type to RGR_UE_CFG
 *         - fill the ue info and ambr values
 *         - set the configuration state to WR_UMM_RAB_SETUP_CFG_SENT
 *         - send the request to scheduler
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmRabSetupSchdUeRecfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabSetupTrans        *rabSetup = &transCb->u.rabSetupTrans;
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   U32                       transId;
   /* SPS changes starts */
   U32                       count  = 0;
   /* SPS changes ends */
   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Set last 4 bits as 1 for UE reconfiguration */
   transId = transCb->transId | WR_RAB_SETUP_UE_TRANS_ID;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   if ((rabSetup->dlAmbr != 0) || (rabSetup->ulAmbr != 0))
   {
      ueRecfg->ueRecfgTypes = RGR_UE_QOS_RECFG;
      ueRecfg->ueQosRecfg.ambrPres = TRUE;
      ueRecfg->ueQosRecfg.dlAmbr   = rabSetup->dlAmbr / 8;
      ueRecfg->ueQosRecfg.ueBr     = rabSetup->ulAmbr / 8;
   }
   
   if(rabSetup->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
   {
      wrUmmSchdFillUeRadioResCfgInfo(ueRecfg, 
            (rabSetup->rrmUeRecfgRspMsg->u.rrmUeRecfgResp), transCb->ueCb,FALSE);
   }
   /* SPS changes starts */
   /* The initialisation from the ueCb is required for the following case:
    * 1) If the SPS was already enabled for the UE, thus RRM doesnot allocate SPS
    *    - The SR mask for all the LCs should be set for rel 9 UE based on this.
    * 2) If the SPS was neither enabled previously nor it is enabled by RRM
    *    - Donot enable SR Mask for any of the LCs based on this. */
   for (count = 0; count < rabSetup->numRabs; count++)
   {
      rabSetup->rabs[count].isUlSpsEnabled = FALSE;
   }
   wrUmmFillSpsSchdReCfgUe(transCb->transTyp, transCb->ueCb, rabSetup->rrmUeRecfgRspMsg->u.rrmUeRecfgResp, \
         ueRecfg, rabSetup->numRabs, (Void *)(rabSetup->rabs), &transCb->spsCfgTransCb.spsCfg, \
         &transCb->spsCfgTransCb.isUeDlSpsEnabled, &transCb->spsCfgTransCb.isUeUlSpsEnabled);
   /* SPS changes ends */

   /* Mark the transaction that UE config has been sent */
   rabSetup->schdUeCfgState = WR_UMM_RAB_SETUP_CFG_SENT;
   
   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}

/** @brief This function is responsible for preparing and sending the logical
 *         channel configuration for DRBs to scheduler.
 *
 * @details
 *
 *     Function: wrUmmRabSetupSchdLchCfgReq
 *
 *         Processing steps:
 *         - check if the particular ERAB is admitted
 *         - allocate a new configuration request
 *         - set the configuration type to RGR_LCH_CFG
 *         - fill the ue info and logical channel info
 *         - send the request to scheduler
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmRabSetupSchdLchCfgReq
(
WrUmmTransCb                 *transCb,
U8                           idx
)
{
   WrUmmRabSetupTrans        *rabSetup = &transCb->u.rabSetupTrans;
   RgrCfgReqInfo             *cfgReq;
   RgrLchCfg                 *lchCfg;
   U32                       transId;
   U8                        lcgId;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Set last 4 bits as 1 for UE reconfiguration */
   transId = transCb->transId | idx;
   lchCfg  = &cfgReq->u.cfgInfo.u.lchCfg;
   cfgReq->action = RGR_CONFIG;
   cfgReq->u.cfgInfo.cfgType = RGR_LCH_CFG;
   lchCfg->cellId = transCb->ueCb->cellId;
   lchCfg->crnti  = transCb->ueCb->crnti;
   lchCfg->lcId   = rabSetup->rabs[idx].lchId;
   lchCfg->lcType = CM_LTE_LCH_DTCH;
   lchCfg->dlInfo.dlTrchType = CM_LTE_TRCH_DL_SCH;
   lchCfg->dlInfo.dlQos.qci = rabSetup->rabs[idx].qci;
   lchCfg->dlInfo.dlQos.gbr = rabSetup->rabs[idx].dlGbr;
   lchCfg->dlInfo.dlQos.mbr = rabSetup->rabs[idx].dlMbr;

   lchCfg->ulLchQciInfo.lcId = rabSetup->rabs[idx].lchId;
   lchCfg->ulLchQciInfo.qci = rabSetup->rabs[idx].qci;
   /* Fetching the lcgId from the table, initilized at wrSmDfltPrioInfoTbl */
   lcgId = wrUmmGetLcgIdForQci(transCb->ueCb->cellId, 
                                       rabSetup->rabs[idx].qci);

   /* SPS changes starts */
   if ((rabSetup->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG) && \
      (rabSetup->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stErabAcceptLst[idx].bIsDlSpsEnabled == TRUE))
   {
      /* Since there is no RBC and both RRM and APP need an enhancement. So, here there can be two approaches 
       * 1) Either assume that the ERAB sent to RRM is being received in the same order in the accepted list
       * 2) Or using a loop compare ERAB ids and find out if bIsSpsEnabled == TRUE 
       * Approach one is assumed as of now to avoid unnecessary loop. Thus accessed directly with idx */
      lchCfg->dlInfo.dlSpsCfg.isSpsEnabled = TRUE;
      rabSetup->rabs[idx].isDlSpsEnabled = TRUE;
      /* Forcing the LCG ID to 1 since SPS is enabled */
      lcgId = WR_UMM_LCG_GRP_ID_1;  
   }
   else
   {
      lchCfg->dlInfo.dlSpsCfg.isSpsEnabled = FALSE;
      rabSetup->rabs[idx].isDlSpsEnabled = FALSE;
   }
   /* SPS changes ends */

#ifdef LTE_L2_MEAS
   lchCfg->lcgId = wrUmmGetLcgIdForQci(transCb->ueCb->cellId, 
                                       rabSetup->rabs[idx].qci);
                                       
#endif
   lchCfg->ulLchQciInfo.lcgId =  lcgId;
   rabSetup->rabs[idx].schdCfgState = WR_UMM_RAB_SETUP_CFG_SENT;

   WrIfmSchdCfgUe(cfgReq, transId);
   if(WR_UMM_IS_GBR_BEARER(rabSetup->rabs[idx].qci))
   {
      /* Updating the effUlGbr and effUlMbr values */
      transCb->ueCb->mappedLcgInfo[lcgId].effUlGbr += rabSetup->rabs[idx].ulGbr;
      transCb->ueCb->mappedLcgInfo[lcgId].effUlMbr += rabSetup->rabs[idx].ulMbr;
      /* Sending the updated values to MAC */
      wrUmmSchdUeLcgReCfg(transCb->ueCb, transCb->transId, lcgId, \
            (transCb->ueCb->mappedLcgInfo[lcgId].effUlGbr), \
            (transCb->ueCb->mappedLcgInfo[lcgId].effUlMbr));
      rabSetup->cfgStateMsk |= WR_UMM_RAB_SETUP_LCG_CFG_SENT;
      rabSetup->lcgCfgStatus[lcgId] = WR_UMM_RAB_SETUP_LCG_CFG_SENT;
      rabSetup->expectedLcgCfgStatus++;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for sending the UE reconfiguration 
 *         request and logical channel configuration request for all the 
 *         admitted bearers to scheduler.
 *
 * @details
 *
 *     Function: wrUmmRabSetupCfgSchd
 *
 *         Processing steps:
 *         - if the ambr values are changed in the RAB Setup request, then
 *           send the UE reconfiguration request
 *         - for all the ERABs that are to be setup, send the logical channel
 *           configuration request
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmRabSetupCfgSchd
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabSetupTrans        *rabSetup = &transCb->u.rabSetupTrans;
   U32                       idx;

   /* Converting kbps to kBps, because Schedular expect in terms of byte */
   for (idx = 0; idx < rabSetup->numRabs; idx++)
   {
      rabSetup->rabs[idx].ulGbr /= 8;
      rabSetup->rabs[idx].dlGbr /= 8;
      rabSetup->rabs[idx].ulMbr /= 8;
      rabSetup->rabs[idx].dlMbr /= 8; 
   }

   /* SPS changes starts */
   /* The initialisation from the ueCb is required for the following case:
    * 1) If the SPS was already enabled for the UE, thus RRM doesnot allocate SPS
    *    - The SR mask for all the LCs should be set for rel 9 UE based on this.
    * 2) If the SPS was neither enabled previously nor it is enabled by RRM
    *    - Donot enable SR Mask for any of the LCs based on this. */
   transCb->spsCfgTransCb.isUeUlSpsEnabled = transCb->ueCb->spsCfg.spsCfgUl.bIsSpsEnabled;
   transCb->spsCfgTransCb.isUeDlSpsEnabled = transCb->ueCb->spsCfg.spsCfgDl.bIsSpsEnabled;
   /* SPS changes ends */
   if ((rabSetup->dlAmbr != 0) || (rabSetup->ulAmbr != 0) ||
         (rabSetup->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap))
   {
      wrUmmRabSetupSchdUeRecfgReq(transCb);
      rabSetup->cfgStateMsk |= WR_UMM_RAB_SETUP_UE_CFG_SENT;
   }

   /* Make the expected LCG config status to 0*/
   rabSetup->expectedLcgCfgStatus = 0;

   /* Configure the scheduler for each new RAB being created.             */
   for (idx = 0; idx < rabSetup->numRabs; idx++)
   {
      if(TRUE == rabSetup->rabs[idx].rabAllowed)
      {
         wrUmmRabSetupSchdLchCfgReq(transCb, idx);
         rabSetup->cfgStateMsk |= WR_UMM_RAB_SETUP_LCH_CFG_SENT;
      }
   }

   RETVALUE(ROK);
}

   /*Removed DRX flag*/
/** @brief This function is responsible for reconfiguring the UE context
 *         at the scheduler.
 *
 * @details
 *
 *     Function: wrUmmRabSetupDrxRelSchdUeRecfgReq
 *
 *         Processing steps:
 *         - allocate a new configuration request
 *         - set the configuration type to RGR_UE_CFG
 *         - fill the ue info and DRX Release info
 *         - set the configuration state to WR_UMM_RAB_SETUP_SCH_CFG_DRX
 *         - send the request to scheduler
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmRabSetupDrxRelSchdUeRecfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabSetupTrans        *rabSetup = &transCb->u.rabSetupTrans;
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   RmuUeRecfgRsp *rrmUeRecfgResp = rabSetup->rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
   U32                       transId;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   transId = transCb->transId | WR_RAB_SETUP_UE_TRANS_ID;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   /* Mark the transaction that UE config has been sent */
   rabSetup->state = WR_UMM_RAB_SETUP_SCH_CFG_DRX;
   wrUmmSchdFillUeDrxCfgInfo(ueRecfg, (WrDrxCfg *)&rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.stRmDrxCfg);
   
   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
} 
/*Removed DRX flag*/

/** @brief This function is responsible for traversing the RABs to be setup
 *         list in the RAB Setup Request message and find the duplicate E-Rab
 *         ID is the RabLst.
 *   ccpu00127706: Modified this function as a part of this CR fix.
 *
 * @details
 *
 *     Function: wrUmmChkRabIdInRabSetupLst
 *
 *         Processing steps:
 *         - for the number of ERAB information in the message, 
 *           - Find the duplicate E-Rab ID is lst
 *
 * @param[in, out]  rabSetup : RAB Setup transaction information
 * @param[in]  rabLst   : RAB Setup list present in the RAB Setup message
 * @return  -#Success ROK
 *          -#Failure RFAILED
 */

PRIVATE S16 wrUmmChkRabIdInRabSetupLst
(
SztE_RABToBeSetupLstBrSUReq  *rabLst,
WrUmmRabSetupTrans           *rabSetup,
U8                            idx,
U8                           *indx
)
{
  U32                          eRabId;
  U32                          numRabs;
  U8                           dupFlag = FALSE;
  U8                           eRabIdx = 0;
  SztE_RABToBeSetupItemBrSUReq *rabInfo;

  numRabs = rabLst->noComp.val;
  eRabId = rabLst->member[idx].value.u.sztE_RABToBeSetupItemBrSUReq.e_RAB_ID.val;
  eRabIdx = (idx + 1);
  
  while(eRabIdx < numRabs)
  {
    rabInfo = &rabLst->member[eRabIdx].value.u.sztE_RABToBeSetupItemBrSUReq;
    if(eRabId == rabInfo->e_RAB_ID.val)
    { 
       /* As ERAB-ID valid range is from 0 to 15 taken 16 to keep track */
      rabInfo->pres.pres = NOTPRSNT;
      dupFlag = TRUE;
    }
    eRabIdx++;
  }
  if(dupFlag)
  {
    dupFlag = FALSE;
    rabLst->member[idx].value.u.sztE_RABToBeSetupItemBrSUReq.pres.pres = NOTPRSNT;
    rabSetup->rabs[*indx].rabAllowed     = FALSE;
    rabSetup->rabs[*indx].cause.causeTyp = CAUSE_RADIONW;
    rabSetup->rabs[*indx].cause.causeVal = SztCauseRadioNwmultiple_E_RAB_ID_instancesEnum;
    (*indx)++;
    RETVALUE(RFAILED);
  } 
  RETVALUE(ROK);
 
}/* End of wrUmmChkRabIdInRabSetupLst */ 

/** @brief This function is responsible for traversing the RABs to be setup
 *         list in the RAB Setup Request message and populating the internal
 *         data structure.
 *
 * @details
 *
 *     Function: wrUmmPrcRabSetupLst
 *
 *         Processing steps:
 *         - for the number of ERAB information in the message, 
 *           - copy the QoS and tunnel information to the internal data 
 *             structure.
 *
 * @param[in, out]  rabSetup : RAB Setup transaction information
 * @param[in]  rabLst   : RAB Setup list present in the RAB Setup message
 * @return  -#Success ROK
 *          -#Failure RFAILED
 */
PRIVATE S16 wrUmmPrcRabSetupLst
(
WrUmmRabSetupTrans           *rabSetup,
SztE_RABToBeSetupLstBrSUReq  *rabLst
)
{
   U32                          numRabs;
   SztE_RABToBeSetupItemBrSUReq *rabInfo;
   U8                            indx = 0;
   U8                            idx = 0;
   U8                            ret = RFAILED;

   numRabs = rabLst->noComp.val;
   
   WR_ALLOC(&rabSetup->rabs, numRabs * sizeof(WrUmmRabSetupRabInfo));
   if (rabSetup->rabs == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   rabSetup->curCfgRes              = WR_UMM_RAB_SETUP_CFG_IDLE;

   for(indx = 0;indx < numRabs; indx++)
   {
     rabInfo = &rabLst->member[indx].value.u.sztE_RABToBeSetupItemBrSUReq;

     rabSetup->rabs[idx].erabId  = rabInfo->e_RAB_ID.val;

     /* Check if we have received the AMBR value and both of them are set to
      * zero. If so, set all the RAB allowed to FALSE and return failure
      */
     if((TRUE == rabSetup->isUeAggrMaxBitRateRcvd) && 
        (0 == rabSetup->dlAmbr) && (0 == rabSetup->ulAmbr))
     {
        RLOG1(L_ERROR, "Received Invalid AMBR value for RAB ID [%d]",
              rabSetup->rabs[idx].erabId);
        rabSetup->rabs[idx].cause.causeTyp  = WR_CAUSE_PROTOCOL;
        rabSetup->rabs[idx].cause.causeVal  = SztCauseProtsemantic_errorEnum;
        rabSetup->rabs[idx].rabAllowed      = FALSE;
        idx++;
        continue;
     }

      /*Increment KPI for Number of E-RAB Attempted to setup*/
      WR_INC_KPI_FUNC1(wrIncKpiFrERABEstabAttPrQci,rabInfo->e_RABlevelQoSParams.qCI.val);

     if((rabInfo->pres.pres == NOTPRSNT) ||
        (wrUmmChkRabIdInRabSetupLst(rabLst,rabSetup, indx, &idx) != ROK))
     {
        continue;
     }

     if(rabSetup->rabs[idx].erabId < WR_UMM_ERAB_OFFSET || 
        rabSetup->rabs[idx].erabId > WR_UMM_MAX_ERABID)
     {
         /* Error_Cause: Filling the proper cause value in E-Rab setup failure
          * */
         rabSetup->rabs[idx].cause.causeTyp  = CAUSE_RADIONW;
         rabSetup->rabs[idx].cause.causeVal  = SztCauseRadioNwunknown_E_RAB_IDEnum;
         rabSetup->rabs[idx].rabAllowed      = FALSE;
         idx++;
         continue;
     }

     if (rabInfo->e_RABlevelQoSParams.gbrQosInform.pres.pres == PRSNT_NODEF)
     {
        SztGBR_QosInform    *gbrIe;

        gbrIe = &rabInfo->e_RABlevelQoSParams.gbrQosInform;
        wrGetBitRate(&gbrIe->e_RAB_MaxBitrateDL, &rabSetup->rabs[idx].dlMbr);
        wrGetBitRate(&gbrIe->e_RAB_MaxBitrateUL, &rabSetup->rabs[idx].ulMbr);
        wrGetBitRate(&gbrIe->e_RAB_GuaranteedBitrateDL, 
                                &rabSetup->rabs[idx].dlGbr);
        wrGetBitRate(&gbrIe->e_RAB_GuaranteedBitrateUL, 
             &rabSetup->rabs[idx].ulGbr);
     }
     else if(rabInfo->e_RABlevelQoSParams.qCI.val <= 4)
     {
        rabSetup->rabs[idx].rabAllowed     = FALSE;
        rabSetup->rabs[idx].cause.causeTyp = CAUSE_RADIONW;
        rabSetup->rabs[idx].cause.causeVal = SztCauseRadioNwinvalid_qos_combinationEnum;
        idx++;
        continue;
     }
     else
     {
        rabSetup->rabs[idx].ulGbr = 0;
        rabSetup->rabs[idx].dlGbr = 0;
        rabSetup->rabs[idx].ulMbr = 0;
        rabSetup->rabs[idx].dlMbr = 0;
     }
     if (rabInfo->e_RABlevelQoSParams.allocationRetentionPriority.pres.pres == PRSNT_NODEF)
     {
        SztAllocnAndRetentionPriority    *arpIe;

        arpIe = &rabInfo->e_RABlevelQoSParams.allocationRetentionPriority;

        rabSetup->rabs[idx].arpInfo.priorityLvl = 
           (arpIe->priorityLvl.pres == TRUE)?arpIe->priorityLvl.val:0;
        rabSetup->rabs[idx].arpInfo.preemptCapability =
           (arpIe->pre_emptionCapblty.pres == TRUE)?
           arpIe->pre_emptionCapblty.val:0;
        rabSetup->rabs[idx].arpInfo.preemptVulnerability =
           (arpIe->pre_emptionVulnerability.pres == TRUE)?
           arpIe->pre_emptionVulnerability.val:0;
      }

       if (wrUtlGetTportAddr(&rabInfo->transportLyrAddr, 
             &rabSetup->rabs[idx].sgwAddr) != ROK)
       {
          rabSetup->rabs[idx].rabAllowed     = FALSE;
          rabSetup->rabs[idx].cause.causeTyp = CAUSE_TRANSPORT;
          rabSetup->rabs[idx].cause.causeVal = SztCauseTportunspecifiedEnum;
          idx++;
          continue;
       }
       if(CM_TPTADDR_IPV4 == rabSetup->rabs[idx].sgwAddr.type)
       {
          rabSetup->rabs[idx].sgwAddr.u.ipv4TptAddr.port = WR_DFLT_EGTP_PORT;
       }
       else if(CM_TPTADDR_IPV6 == rabSetup->rabs[idx].sgwAddr.type)
       {
          rabSetup->rabs[idx].sgwAddr.u.ipv6TptAddr.port = WR_DFLT_EGTP_PORT;
       }

       rabSetup->rabs[idx].qci     = rabInfo->e_RABlevelQoSParams.qCI.val;
       rabSetup->rabs[idx].remTeid = wrUtlGetTeid(&rabInfo->gTP_TEID);
       rabSetup->rabs[idx].nasPdu       = &rabInfo->nAS_PDU;
       rabSetup->rabs[idx].schdCfgState = WR_UMM_RAB_SETUP_CFG_IDLE;
       rabSetup->rabs[idx].rabAllowed   = TRUE;
       rabSetup->rabs[idx].tnlCfgState  = WR_UMM_RAB_SETUP_CFG_IDLE;
       ret = ROK;
      
       RLOG1(L_INFO, "Serving GW Type [%d]",
          rabSetup->rabs[idx].sgwAddr.type);
 #ifdef WR_RSYS_KPI  
   SGetEpcTime(&(rabSetup->rabs[idx].earbSetupReqTm));  
 #endif/*WR_RSYS_KPI*/
       /* increment the rabSetup index */
       idx++;
   }
   /* update the numRabs */
   rabSetup->numRabs = idx;

   RETVALUE(ret);
}

/** @brief This function is responsible for parsing the ERAB Setup Request
 *         message and store the information into the internal data structure.
 *
 * @details
 *
 *     Function: wrUmmParseRabSetupMsg
 *
 *         Processing steps:
 *         - for the number of IEs present in the message
 *           - parse the IE and store the information into the internal data
 *             structure
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmParseRabSetupMsg
(
WrUmmTransCb                 *transCb
)
{
   S1apPdu                   *pdu = transCb->msg->u.s1Pdu->pdu;
   SztInitiatingMsg          *initMsg = &pdu->pdu.val.initiatingMsg;
   SztE_RABSetupRqst         *rabSetupIE = &initMsg->value.u.sztE_RABSetupRqst;
   WrUmmRabSetupTrans        *rabSetup = &transCb->u.rabSetupTrans;
   U32                       idx;
   SztProtIE_Field_E_RABSetupRqstIEs *ie;
   S32                       ret = ROK;
   
   rabSetup->dlAmbr             = 0;
   rabSetup->ulAmbr             = 0;
   rabSetup->numRabs            = 0;
   rabSetup->numRabsAllowed     = 0;
   for(idx = 0;idx < rabSetupIE->protocolIEs.noComp.val; idx++)
   {
      ie = &rabSetupIE->protocolIEs.member[idx];
      switch(ie->id.val)
      {
         case Sztid_uEaggregateMaxBitrate:
         {
            SztUEAggMaxBitrate *ambr = &ie->value.u.sztUEAggMaxBitrate;
            wrGetBitRate(&ambr->uEaggregateMaxBitRateDL, &rabSetup->dlAmbr);
            wrGetBitRate(&ambr->uEaggregateMaxBitRateUL, &rabSetup->ulAmbr);
            /* Set value in transaction CB to take further action */
            rabSetup->isUeAggrMaxBitRateRcvd = TRUE;
            break;
         }
         case Sztid_E_RABToBeSetupLstBrSUReq:
         {
            ret = wrUmmPrcRabSetupLst(rabSetup, 
                                    &ie->value.u.sztE_RABToBeSetupLstBrSUReq);
            break;
         }

      }
   }

   RETVALUE(ret);
}
/* ccpu00130441 and ccpu00130443 */
/** @brief This function is responsible for handling handover by filling 
 *  ERAB setup failure for each RABs.
 *
 * @details
 *
 *     Function: wrUmmHOHandleRabSetup
 *
 *         Processing steps:
 *         - for the number of RABs that where successfully parsed/admitted, 
 *           - fill the failure cause values and set rabAllowed as FALSE 
 *
 * @param[in]  transCb : transaction information
 * @param[in]  hoType : Handover type
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmHOHandleRabSetup
(
WrUmmTransCb              *transCb,
WrHoType                  hoType
)
{
   WrUmmRabSetupTrans        *rabSetupTrans = &transCb->u.rabSetupTrans;
   U32                       idx           = 0;
   
   for (idx = 0; idx < rabSetupTrans->numRabs; idx++)
   {
      rabSetupTrans->rabs[idx].rabAllowed = FALSE;
      rabSetupTrans->rabs[idx].cause.causeTyp  = CAUSE_RADIONW;

      if( WR_HO_TYPE_INTRA_LTE_S1 == hoType)  
      {
         rabSetupTrans->rabs[idx].cause.causeVal  = 
            SztCauseRadioNws1_intra_system_handover_triggeredEnum;
      }
      else if ( WR_HO_TYPE_IRAT_UTRA == hoType) 
      {
         rabSetupTrans->rabs[idx].cause.causeVal  = 
            SztCauseRadioNws1_inter_system_handover_triggeredEnum;
      }
      else if( WR_HO_TYPE_X2 == hoType)
      {
         rabSetupTrans->rabs[idx].cause.causeVal  = 
            SztCauseRadioNwx2_handover_triggeredEnum;
      }
   }
   
   RETVALUE(ROK);
}


/** @brief This function is responsible for fetching a new LCH and DRB id for
 *         the incoming ERABs.
 *
 * @details
 *
 *     Function: wrUmmPrcRabSetup
 *
 *         Processing steps:
 *         - for the number of RABs that where successfully parsed/admitted, 
 *           - if the QCI is supportted
 *             - dont assign LCH and DRB id
 *           - else, get a free LCH and DRB id
 *           - set the RAB state to WR_UMM_RAB_SETUP_ALLOWED,
 *           - set the scheduler state of this RAB to WR_UMM_RAB_SETUP_CFG_SENT
 *           - increment the number of RABs that are admitted
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmPrcRabSetup
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabSetupTrans        *rabSetupTrans = &transCb->u.rabSetupTrans;
   WrUmmCellCb               *ummCellCb     = NULLP;
   U32                       idx           = 0;
   U16                       cellId         = transCb->ueCb->cellId;
   U8                        freeIdx        = 0;
   S16                       retVal = ROK;
   RmuUeRecfgReq            *ueRecfgReq;
   RmuUErabAddModIe         *eRabAddPtr;
   
   WR_ALLOC(&ueRecfgReq, sizeof(RmuCommonMsg));
   if (ueRecfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   eRabAddPtr = ueRecfgReq->stErabConfInfo.stErabAddModLst;
   ueRecfgReq->stErabConfInfo.usNoErabsToAddMod = 0;
   ueRecfgReq->stErabConfInfo.bNoErabsToRel = 0;
   ueRecfgReq->stErabConfInfo.enErabAction = RMU_ADD_ERAB;

   WR_UMM_GET_CELLCB(ummCellCb, cellId);
   if (NULLP == ummCellCb)
   {
      
      RLOG0(L_ERROR, "Failed to get cell CB");
      WR_FREE(ueRecfgReq, sizeof(RmuCommonMsg));
      RETVALUE(RFAILED);
   }   
   RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti, "[S1AP]:Received E-RAB SETUP "
      "REQUEST [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
      transCb->ueCb->s1ConCb->mme_ue_s1ap_id,
      transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
   /* Need to determine LCH Id and RB Id for each RAB to be setup         */
   for (idx = 0; idx < rabSetupTrans->numRabs; idx++)
   {
      /* ccpu00125866: to handle the bearers that failed while
       *  parsing the ERAB List */
      if(rabSetupTrans->rabs[idx].rabAllowed == FALSE)
      {
        continue;
      }
     
      /* Fix for ccpu00128218 */
      if((transCb->ueCb->rbInfo.numOfRbCfgd + rabSetupTrans->numRabsAllowed) >= WR_MAX_DRBS_ALLOWED)
      {
          rabSetupTrans->rabs[idx].cause.causeTyp  = CAUSE_RADIONW;
          rabSetupTrans->rabs[idx].cause.causeVal  = SztCauseRadioNwradio_resources_not_availableEnum;
          rabSetupTrans->rabs[idx].rabAllowed      = FALSE;
          continue;
      }

      /* Fix for ccpu00124991:As MME is sending the out of range ERAB ID, 
       * eNB will stop processing this error. */ 
      freeIdx = rabSetupTrans->rabs[idx].erabId  - WR_UMM_ERAB_OFFSET;
      if((freeIdx >= WR_MAX_DRBS) || 
                      (transCb->ueCb->rbInfo.rab[freeIdx].inUse == TRUE))
      {
         /* Duplication of the E_RAB setup */
         /* TODO Add the Proper Cause Value */
         /* Error_Cause: Filling the proper cause value in E-Rab setup failure
          * */
         rabSetupTrans->rabs[idx].cause.causeTyp  = CAUSE_RADIONW;
         rabSetupTrans->rabs[idx].cause.causeVal  = (freeIdx >= WR_MAX_DRBS)?SztCauseRadioNwunknown_E_RAB_IDEnum:
                                                    SztCauseRadioNwmultiple_E_RAB_ID_instancesEnum;
         rabSetupTrans->rabs[idx].rabAllowed      = FALSE;
         continue;
      }

      if ((rabSetupTrans->rabs[idx].qci > WR_MAX_QCI_SUPPORTED) || (ummCellCb->wrRabCfgParams[rabSetupTrans->rabs[idx].qci].qciSupported == FALSE))
      {
         /* Error_Cause: Filling the proper cause value in E-Rab setup failure
          * */
         rabSetupTrans->rabs[idx].cause.causeTyp  = CAUSE_RADIONW;
         /* ccpu00131036: Changing the cause value */
         rabSetupTrans->rabs[idx].cause.causeVal  = SztCauseRadioNwnot_supported_QCI_valueEnum;
         rabSetupTrans->rabs[idx].rabAllowed      = FALSE;
         continue;
      }

      if (wrUmmGetLch(transCb->ueCb, &rabSetupTrans->rabs[idx].lchId) != ROK)
      {
         /*ccpu00127802*/
         RLOG0(L_ERROR, "Failed to allocate LCH for a RAB");

         /* Error_Cause: Filling the proper cause value in E-Rab setup failure
          * */
         rabSetupTrans->rabs[idx].cause.causeTyp  = CAUSE_RADIONW;
         rabSetupTrans->rabs[idx].cause.causeVal  = SztCauseRadioNwradio_resources_not_availableEnum;
         rabSetupTrans->rabs[idx].rabAllowed      = FALSE;
         continue;
      }
      if (wrUmmGetDrb(transCb->ueCb, &rabSetupTrans->rabs[idx].drbId) != ROK)
      {
         /*ccpu00127802*/
         RLOG0(L_ERROR, "Failed to allocate DRB for a RAB");

         /* Error_Cause: Filling the proper cause value in E-Rab setup failure
          * */
         wrUmmRelLch(transCb->ueCb, rabSetupTrans->rabs[idx].lchId);
         rabSetupTrans->rabs[idx].cause.causeTyp = CAUSE_RADIONW;
         rabSetupTrans->rabs[idx].cause.causeVal  = SztCauseRadioNwradio_resources_not_availableEnum;
         rabSetupTrans->rabs[idx].rabAllowed = FALSE;
         continue;
      }
      rabSetupTrans->rabs[idx].schdCfgState = WR_UMM_RAB_SETUP_CFG_SENT;
      rabSetupTrans->numRabsAllowed++;


     // eRabAddPtr->isERabToAdd                   = TRUE;
      eRabAddPtr->bErabId                        = rabSetupTrans->rabs[idx].erabId;
      
      eRabAddPtr->stErabQosInfo.bErabQCI           = rabSetupTrans->rabs[idx].qci;
         
      eRabAddPtr->stErabQosInfo.stErabGbrInfo.uiErabDlMbr = rabSetupTrans->rabs[idx].dlMbr;
      eRabAddPtr->stErabQosInfo.stErabGbrInfo.uiErabUlMbr = rabSetupTrans->rabs[idx].ulMbr;
      eRabAddPtr->stErabQosInfo.stErabGbrInfo.uiErabDlGbr = rabSetupTrans->rabs[idx].dlGbr;
      eRabAddPtr->stErabQosInfo.stErabGbrInfo.uiErabUlGbr = rabSetupTrans->rabs[idx].ulGbr;
      
      eRabAddPtr->stErabQosInfo.stErabArpInfo.bErabPrioLvl  =
               (U8)rabSetupTrans->rabs[idx].arpInfo.priorityLvl;
      eRabAddPtr->stErabQosInfo.stErabArpInfo.bErabPreemptCap =
               (Bool)rabSetupTrans->rabs[idx].arpInfo.preemptCapability;
      eRabAddPtr->stErabQosInfo.stErabArpInfo.bErabPreemptVul =
               (Bool)rabSetupTrans->rabs[idx].arpInfo.preemptVulnerability;
      
      ueRecfgReq->stErabConfInfo.usNoErabsToAddMod++;

      /*ccpu00127802*/
      RLOG3(L_DEBUG, "DEDICATED BEARER INFO: qci[%d] rabId[%d] dlMbr[%ld]",
         eRabAddPtr->stErabQosInfo.bErabQCI, eRabAddPtr->bErabId,
         eRabAddPtr->stErabQosInfo.stErabGbrInfo.uiErabDlMbr);

      RLOG3(L_DEBUG,"dlGbr=%ld, ulGbr=%ld ulMbr[%ld]",
         eRabAddPtr->stErabQosInfo.stErabGbrInfo.uiErabDlGbr,
         eRabAddPtr->stErabQosInfo.stErabGbrInfo.uiErabUlGbr,
         eRabAddPtr->stErabQosInfo.stErabGbrInfo.uiErabUlMbr);

      /* RRM fix to handle multiple E-Rab Ids in Rab setup */
      eRabAddPtr++;
   }

   if (ueRecfgReq->stErabConfInfo.usNoErabsToAddMod)
   {
      ueRecfgReq->bCellId = transCb->ueCb->cellId;
      ueRecfgReq->usCrnti = transCb->ueCb->crnti;
      ueRecfgReq->uiTransId = transCb->transId;
      ueRecfgReq->bRecfgBitMap |= RMU_RECFG_REQ_ERAB_CONFIG;

      retVal = wrIfmRrmSndUeRecfgReq(ueRecfgReq);
   }
   else
   {
      WR_FREE(ueRecfgReq, sizeof(RmuCommonMsg));
   }
   
   RETVALUE(retVal);
}

/** @brief This function is responsible for processing the response(s) of UE 
 *         and logical channel configuration of the scheduler for this UE.
 *
 * @details
 *
 *     Function: wrUmmRabSetupPrcSchdCfgCfm
 *
 *         Processing steps:
 *         - fetch the sub-transaction id from the transId,
 *         - if id is WR_RAB_SETUP_UE_TRANS_ID,
 *           - if the scheduler configuration is successful,
 *             - set scheduler flag for UE configuration to 
 *               WR_UMM_RAB_SETUP_CFG_SUCC
 *             - else, set scheduler flag for UE configuration to 
 *               WR_UMM_RAB_SETUP_CFG_FAIL
 *         - else, validate the id with the number of RABs to be setup
 *           - if the scheduler configuration is successful,
 *             - set the scheduler configuration state for that RAB to 
 *               WR_UMM_RAB_SETUP_CFG_SUCC
 *           - else, set the scheduler configuration state for that RAB to 
 *             WR_UMM_RAB_SETUP_CFG_FAIL
 *         - after receiving scheduler response for all the LCH configurations
 *           and UE configuration, set the configuration state to 
 *           WR_UMM_RAB_SETUP_SCHD_CFG_DONE
 *         - if either UE configuration or any 1 of the LCH configurations 
 *           failed, then
 *           - set the configuration status to WR_UMM_RAB_SETUP_CFG_FAIL,
 *         - else, set the configuration status to WR_UMM_RAB_SETUP_CFG_SUCC
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmRabSetupPrcSchdCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabSetupTrans        *rabSetup = &transCb->u.rabSetupTrans;
   WrUmmSchdCfgCfm           *schdCfm = &(transCb->msg->u.schdCfm);
   U8                        idx;
   U8                        lcgId;
    
   idx = schdCfm->transId & 0x0f;

   if (!(rabSetup->cfgStateMsk & WR_UMM_RAB_SETUP_LCG_CFG_SENT))
   {
      rabSetup->cfgStateMsk |= WR_UMM_RAB_SETUP_LCG_CFG_RECVD;
   }
   if (!(rabSetup->cfgStateMsk & WR_UMM_RAB_SETUP_UE_CFG_SENT))
   {
      rabSetup->cfgStateMsk |= WR_UMM_RAB_SETUP_UE_CFG_RECVD;
   }
   if (!(rabSetup->cfgStateMsk & WR_UMM_RAB_SETUP_LCH_CFG_SENT))
   {
      rabSetup->cfgStateMsk |= WR_UMM_RAB_SETUP_LCH_CFG_RECVD;
   }

   if (idx == WR_RAB_SETUP_UE_TRANS_ID)
   {
      rabSetup->cfgStateMsk |= WR_UMM_RAB_SETUP_UE_CFG_RECVD;
   }
   else if (idx == WR_UMM_TRANS_ID_LCG0_CFG)
   {
      rabSetup->lcgCfgStatus[0] = WR_UMM_RAB_SETUP_LCG_CFG_RECVD;
      rabSetup->expectedLcgCfgStatus--;
      for (lcgId=0; lcgId < WR_UMM_MAX_LCG_GRP_ID; lcgId++)
      {
         if (rabSetup->lcgCfgStatus[lcgId] == WR_UMM_RAB_SETUP_LCG_CFG_SENT)
         {
            RETVALUE(ROK);
         }
      }
   }
   else if (idx == WR_UMM_TRANS_ID_LCG1_CFG)
   {
      rabSetup->lcgCfgStatus[1] = WR_UMM_RAB_SETUP_LCG_CFG_RECVD;
      rabSetup->expectedLcgCfgStatus--;
      for (lcgId=0; lcgId < WR_UMM_MAX_LCG_GRP_ID; lcgId++)
      {
         if (rabSetup->lcgCfgStatus[lcgId] == WR_UMM_RAB_SETUP_LCG_CFG_SENT)
         {
            RETVALUE(ROK);
         }
      }
   }
   else if (idx == WR_UMM_TRANS_ID_LCG2_CFG)
   {
      rabSetup->lcgCfgStatus[2] = WR_UMM_RAB_SETUP_LCG_CFG_RECVD;
      rabSetup->expectedLcgCfgStatus--;
      for (lcgId=0; lcgId < WR_UMM_MAX_LCG_GRP_ID; lcgId++)
      {
         if (rabSetup->lcgCfgStatus[lcgId] == WR_UMM_RAB_SETUP_LCG_CFG_SENT)
         {
            RETVALUE(ROK);
         }
      }
   }
   else if (idx == WR_UMM_TRANS_ID_LCG3_CFG)
   {
      rabSetup->lcgCfgStatus[3] = WR_UMM_RAB_SETUP_LCG_CFG_RECVD;
      rabSetup->expectedLcgCfgStatus--;
      for (lcgId=0; lcgId < WR_UMM_MAX_LCG_GRP_ID; lcgId++)
      {
         if (rabSetup->lcgCfgStatus[lcgId] == WR_UMM_RAB_SETUP_LCG_CFG_SENT)
         {
            RETVALUE(ROK);
         }
      }
   }
   else if (idx < rabSetup->numRabs)
   {
      if (schdCfm->status == RGR_CFG_CFM_OK)
      {
         rabSetup->rabs[idx].schdCfgState = WR_UMM_RAB_SETUP_CFG_SUCC;
      }
      else
      {
         /* Error_Cause: Filling the proper cause value in E-Rab setup failure
          * */
         rabSetup->rabs[idx].cause.causeTyp  = CAUSE_RADIONW;
         rabSetup->rabs[idx].cause.causeVal  = SztCauseRadioNwradio_resources_not_availableEnum;
         rabSetup->rabs[idx].schdCfgState    = WR_UMM_RAB_SETUP_CFG_FAIL;
         WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, rabSetup->rabs[idx].cause.causeTyp, rabSetup->rabs[idx].cause.causeVal, INC_KPI_VALUE_BY_ONE);
         rabSetup->rabs[idx].rabAllowed      = FALSE;
      }
      for (idx = 0; idx < rabSetup->numRabs; idx++)
      {
         if ((rabSetup->rabs[idx].schdCfgState == WR_UMM_RAB_SETUP_CFG_SENT) &&
              (TRUE == rabSetup->rabs[idx].rabAllowed))
         {
            RETVALUE(ROK);
         }
      }
      rabSetup->cfgStateMsk |= WR_UMM_RAB_SETUP_LCH_CFG_RECVD;
   }
   else
   {
      /*ccpu00127802*/
      RLOG_ARG0(L_ERROR, DBG_TRNSID, transCb->transId,"invalid transaction "
            "for RAB Setup");
      RETVALUE(ROK);
   }

   /* once we get all the confirmation for all LCGs setting the bitmask */
   if (0 == rabSetup->expectedLcgCfgStatus)
   {
      rabSetup->cfgStateMsk |= WR_UMM_RAB_SETUP_LCG_CFG_RECVD;
   }

   if ((schdCfm->status == RGR_CFG_CFM_OK) && \
         (rabSetup->schdUeCfgState != WR_UMM_RAB_SETUP_CFG_FAIL))
   {
      rabSetup->schdUeCfgState = WR_UMM_RAB_SETUP_CFG_SUCC;
   }
   else
   {
      rabSetup->schdUeCfgState = WR_UMM_RAB_SETUP_CFG_FAIL;
   }

   if ((rabSetup->cfgStateMsk & WR_UMM_RAB_SETUP_CFG_RECVD) == WR_UMM_RAB_SETUP_CFG_RECVD)
   {
      rabSetup->state = WR_UMM_RAB_SETUP_SCHD_CFG_DONE;
      if (rabSetup->schdUeCfgState == WR_UMM_RAB_SETUP_CFG_FAIL)
      {
         rabSetup->curCfgRes = WR_UMM_RAB_SETUP_CFG_FAIL;
      }
      else
      {
         rabSetup->curCfgRes = WR_UMM_RAB_SETUP_CFG_SUCC;
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for getting the DRB configuration
 *         filled.
 *
 * @details
 *
 *     Function: wrUmmRabSetupRrcReconfigAddDrbCfg
 *
 *         Processing steps:
 *         - call wrUmmRrcAddDrbCfg, with the information fromt the rab data 
 *           structure, to fill the RRC configuration data structure.
 *
 * @param[in, out] drbCfg  : RRC data structure to be filled
 * @param[in]     rabInfo : RAB for which the IE is to be filled
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmRabSetupRrcReconfigAddDrbCfg
(
U16                          cellId,
NhuRbCfgInfo                 *drbCfg,
WrUmmRabSetupRabInfo         *rabInfo
)
{
   /* SPS changes starts */
   if (wrUmmRrcAddDrbCfg(cellId, drbCfg, rabInfo->drbId, rabInfo->lchId, 
                                   rabInfo->qci, rabInfo->isUlSpsEnabled) != ROK)
   /* SPS changes ends */
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for filling the RRC structure for
 *         configuring the lower layers.
 *
 * @details
 *
 *     Function: wrUmmRabSetupFillRrcCfg
 *
 *         Processing steps:
 *         - for the number of RABs to be setup,
 *           - if the RAB is admitted, then
 *             - invoke wrUmmRabSetupRrcReconfigAddDrbCfg to fill the
 *               RRC configuration structure
 *
 * @param[in] transCb : transaction information 
 * @param[in, out]     evnt    : RRC configuration
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmRabSetupFillRrcCfg
(
WrUmmTransCb                 *transCb,
NhuDatReqSdus                *evnt
)
{
   WrUmmRabSetupTrans        *rabSetup = &transCb->u.rabSetupTrans;
   NhuRbCfgInfo              *drbCfg;
   WrUmmRabSetupRabInfo      *rabInfo;
   NhuUeCfgInfo              *ueCfg;
   NhuRbCfgList              *rbCfgList;
   U32                       idx;
   U32                       numRabs = 0;
   U16                       cellId = transCb->ueCb->cellId;

   evnt->sdu.isUeCfgPres = TRUE;
   ueCfg = &(evnt->sdu.ueCfg);
   ueCfg->ueCfgBitMsk = NHU_EXP_CFG_RB_CFG;
   ueCfg->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_ADD;
   rbCfgList = &(ueCfg->rbCfgAllList.rbToAddCfgList);
   WR_GET_MEM(evnt, (sizeof(NhuRbCfgInfo) * rabSetup->numRabs), 
                                              &(rbCfgList->rbCfgInfoList));
   for (idx = 0; idx < rabSetup->numRabs; idx++)
   {
      rabInfo = &rabSetup->rabs[idx];
      drbCfg  = rbCfgList->rbCfgInfoList + numRabs;

      /* We send information of only those RABs that are successful       */
      if (rabInfo->rabAllowed == TRUE)
      {
         wrUmmRabSetupRrcReconfigAddDrbCfg(cellId, drbCfg, rabInfo);
         numRabs++;
      }
   }
   rbCfgList->numOfRbCfgInfo = numRabs;

   RETVALUE(ROK);
}

/** @brief This function is responsible for getting the RRC ASN DRB IE filled.
 *
 * @details
 *
 *     Function: wrUmmRabSetupRrcReconfigAddDrb
 *
 *         Processing steps:
 *         - invoke wrUmmRrcReconfigFillDrb, to fill the RRC ASN DRB IE.
 *
 * @param[in, out] drbIe   : RRC ASN DRB IE to be filled
 * @param[in]     rabInfo : RAB information that is to be used for filling
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmRabSetupRrcReconfigAddDrb
(
 /* SPS changes starts */
 WrUeCb                       *ueCb,
 WrUmmTransCb                 *transCb,
 /* SPS changes ends */
 NhuDRB_ToAddMod              *drbIe,
 WrUmmRabSetupRabInfo         *rabInfo
)
{
   /* SPS changes starts */
   if (wrUmmRrcReconfigFillDrb(ueCb, transCb->spsCfgTransCb.isUeUlSpsEnabled, \
            drbIe, rabInfo->erabId, rabInfo->drbId, rabInfo->lchId, rabInfo->qci, \
            rabInfo->isDlSpsEnabled, rabInfo->isUlSpsEnabled) != ROK)
   /* SPS changes ends */
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for preparing the sending the RRC
 *         reconfiguration message. The lower layer configurations are also 
 *         filled here.
 *
 * @details
 *
 *     Function: wrUmmRabSetupPrepRrcMsgAndCfg
 *
 *         Processing steps:
 *         - allocate a new RRC data request event,
 *         - fill the RRC interface header
 *         - get a new free tranaction for the reconfiguration message
 *         - invoke wrUmmRrcCreateRrcReconfigMsg, to create a new 
 *           reconfiguration message
 *         - invoke wrUmmRrcPrepRadioResourceDed, to prepare the Radio
 *           Resource Dedicated IE
 *         - for every RAB that is admitted,
 *           - invoke wrUmmRabSetupRrcReconfigAddDrb, to fill the DRB 
 *             information to the ASN IE
 *         - for every RAB that is admitted,
 *           - copy the dedicated NAS information to the ASN IE
 *         - invoke wrUmmRabSetupFillRrcCfg to fill the lower layer 
 *           configuration for the DRB(s)
 *         - send the event to RRC
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmRabSetupPrepRrcMsgAndCfg
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabSetupTrans                  *rabSetup = &transCb->u.rabSetupTrans;
   NhuDatReqSdus                       *recfgMsg;
   WrUeCb                              *ueCb = transCb->ueCb;
   WrUmmRabSetupRabInfo                *rabInfo;
   NhuDL_DCCH_Msg                      *dcchMsg;
   NhuRRCConRecfgn                     *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs              *recfgIEs;
   NhuRadioResourceConfigDedicated     *rrcCfgDed;
   NhuDRB_ToAddMod                     *drbIe;
   U8                                 numRabs = 0;
   U8                                 nasPduCount = 0;
   U32                                 idx;
   NhuRRCConRecfgn_r8_IEsdedicatedInfoNASLst *dedNasLst;
   /* SPS changes starts */
   U32                                 rabCount = 0;
   RmuUeRecfgRsp                       *rrmUeRecfgResp = rabSetup->rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
   /* SPS changes ends */

   WR_ALLOCEVNT(&recfgMsg, sizeof(NhuDatReqSdus));
   if (recfgMsg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   recfgMsg->hdr.cellId  = ueCb->cellId;
   recfgMsg->hdr.ueId    = ueCb->crnti;
   recfgMsg->hdr.transId = transCb->transId;

   /* Fetch an RRC transaction id for this procedure.                     */
   if (ROK != wrUmmGetRrcTransId(ueCb, C1_RRCCONRECFGN, 
                      WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
                      &rabSetup->transId))
   {
      RLOG0(L_ERROR, "RRC Transaction Not avialble");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }

   /* Create the reconfiguration message which is constructued excluding  */
   /* the actual procedure related details which will be added further    */
   if (wrUmmRrcCreateRrcReconfigMsg(recfgMsg, rabSetup->transId) != ROK)
   {
      RETVALUE(RFAILED);
   }
   
   /* Prepare the radio resources to exclude all the IEs present in the   */
   /* the IE. Those IEs are filled subsequently                           */
   dcchMsg     = &(recfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   recfgIEs    = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   rrcCfgDed   = &(recfgIEs->radioResourceConfigDedicated);
   wrUmmRrcPrepRadioResourceDed(rrcCfgDed);

   /* SPS changes starts */
   rabCount = rabSetup->numRabs;
   if ((rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_UL_SPS_CFG) && \
      (ueCb->spsCfg.spsCfgUl.bIsSpsEnabled != rrmUeRecfgResp->stUeRadioResCfg.\
       stSpsCfg.stSpsConfigUl.bIsSpsEnabled) && (ueCb->isSrMaskEnabled == TRUE) && \
      (ueCb->ueCap->eUTRA_Cap.accessStratumRls.val >= NhuAccessStratumRlsrel9Enum))
   {
      /* Set SR Mask for all the logical channel */
      rabCount += ueCb->rbInfo.numOfRbCfgd;
   }

   /* Allocate memory to include each RAB into the message towards the UE */
   WR_GET_MEM(recfgMsg, (sizeof(NhuDRB_ToAddMod) * rabCount), 
                          &(rrcCfgDed->drb_ToAddModLst.member));
   /* SPS changes ends */
   if (rrcCfgDed->drb_ToAddModLst.member == NULLP)
   {
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < rabSetup->numRabs; idx++)
   {
      rabInfo = &rabSetup->rabs[idx];

      /* We send information of only those RABs that are successful       */
      if (rabInfo->rabAllowed == TRUE)
      {
         drbIe   = rrcCfgDed->drb_ToAddModLst.member + numRabs;
         /* SPS changes starts */
         wrUmmRabSetupRrcReconfigAddDrb(ueCb, transCb, drbIe, rabInfo);
         /* SPS changes ends */
         /* UL PBR changes */
         wrUmmRrcReconfigFillulPbr(drbIe, rabInfo->ulGbr, rabInfo->qci, 
                    rabInfo->isUlSpsEnabled); 
         numRabs++;
         if(rabInfo->nasPdu != NULLP)
         {
            nasPduCount++;
         }
      }
   }
   /* SPS changes starts */
   if (rabCount > rabSetup->numRabs)
   {  
      for (idx = 0; (idx < WR_MAX_DRBS) && (numRabs < rabCount); idx++)
      {
         if(ueCb->rbInfo.rab[idx].inUse == TRUE)
         {
            RLOG3(L_DEBUG, "wrUmmRrcFillUeRadioResDedInfo: rbInfo info present:: idx [%u] erabId [%u] rbId [%u]\n",
                  idx, ueCb->rbInfo.rab[idx].eRABId, ueCb->rbInfo.rab[idx].rbId);
            drbIe   = rrcCfgDed->drb_ToAddModLst.member + numRabs;

            wrUmmRrcReconfigFillModDrb(ueCb, rrmUeRecfgResp->stUeRadioResCfg.stSpsCfg.stSpsConfigUl.bIsSpsEnabled, \
                  drbIe, (U8)ueCb->rbInfo.rab[idx].eRABId, ueCb->rbInfo.rab[idx].rbId ,ueCb->rbInfo.rab[idx].lchId, \
                  (U8)ueCb->rbInfo.rab[idx].qci);

            /* UL PBR changes gbr.ul is in kbps so gbr.ul/8 is done here*/
            wrUmmRrcReconfigFillulPbr(drbIe, ueCb->rbInfo.rab[idx].gbr.ul/8,
                  ueCb->rbInfo.rab[idx].qci, rrmUeRecfgResp->stUeRadioResCfg.stSpsCfg.stSpsConfigUl.bIsSpsEnabled); 
            numRabs++;
         }
      }
   }
   /* SPS changes ends */

   wrFillTknU16(&rrcCfgDed->drb_ToAddModLst.noComp,numRabs);

   /* Include NAS PDUs into the reconfiguration message                   */
   dedNasLst = &(recfgIEs->dedicatedInfoNASLst);
   if(nasPduCount)
   {
     WR_GET_MEM(recfgMsg, (sizeof(NhuDedicatedInfoNAS) * nasPduCount),
                          &(dedNasLst->member));
     if (dedNasLst->member == NULLP)
     {
        RETVALUE(RFAILED);
     }
     numRabs = 0;
     for (idx = 0; idx < rabSetup->numRabs; idx++)
     {
        rabInfo = &rabSetup->rabs[idx];

        /* We send information of only those RABs that are successful       */
        if ((rabInfo->rabAllowed == TRUE) && (rabInfo->nasPdu != NULLP))
        {
           wrFillTknStrOSXL(&(dedNasLst->member[numRabs]), rabInfo->nasPdu->len, 
                          rabInfo->nasPdu->val, &(recfgMsg->memCp));
           numRabs++;
        }
     }
     wrFillTknU16(&dedNasLst->noComp, numRabs);
   }
   else
   {
     dedNasLst->noComp.pres = NOTPRSNT;
   }

   /* Build the configuration towards RRC                                 */
   wrUmmRabSetupFillRrcCfg(transCb, recfgMsg);

   /* SPS changes starts */
   if(rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
   {
      wrUmmRrcFillUeRadioResDedInfo(&recfgMsg->memCp, ueCb, &transCb->spsCfgTransCb.spsCfg, \
            rrcCfgDed, rrmUeRecfgResp);
      /* SPS changes ends */
   }
   
   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,
         "[OTA] Sending RRCConnectionReconfiguration for ERAB Setup");
   if (wrIfmRrcSndReqToUe(recfgMsg) != ROK)
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe failed");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }
   /* Start RRC Reconfiguration Timer */
   wrStartTransTmr(transCb,WR_TMR_RECONFIGURE, wrEmmGetRrcReCfgTmrVal(ueCb->cellId));

   RETVALUE(ROK);
}

/** @brief This function is responsible for filling and sending the tunnel 
 *         creation request to DAM.
 *
 * @details
 *
 *     Function: wrUmmRabSetupCfgTnls
 *
 *         Processing steps:
 *         - for every admitted RAB,
 *           - fill the tunnel information, 
 *             Source address, Local tunnel id
 *             Destination address, Remote tunnel id
 *             DSCP,
 *           - set the tunnel configuration state for this RAB to 
 *             WR_UMM_RAB_SETUP_CFG_SENT
 *           - send the tunnel create requeset to DAM 
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmRabSetupCfgTnls
(
WrUmmTransCb     *transCb
)
{
   WrUmmRabSetupTrans          *rabSetup = &transCb->u.rabSetupTrans;
   WrUeCb                      *ueCb = transCb->ueCb;
   WrUmmRabSetupRabInfo        *rabInfo;
   U32                         transId;
   U16                         idx;
   WrDamTnlInfo                *tnlInfo;

   for (idx = 0; idx < rabSetup->numRabs; idx++)
   {
      rabInfo = &rabSetup->rabs[idx];
      if (rabInfo->rabAllowed == TRUE)
      {
         WR_ALLOC_SHAREBLE_BUF(&tnlInfo, sizeof(WrDamTnlInfo));
         if (NULLP == tnlInfo)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }   
         tnlInfo->hoSrcEnodeb = FALSE;
         transId = transCb->transId | idx;
         tnlInfo->transId = transId;
         tnlInfo->tnlId.cellId = ueCb->cellId;
         tnlInfo->tnlId.crnti  = ueCb->crnti;
         tnlInfo->tnlId.drbId  = rabInfo->drbId;
         /* Cache Opt Changes */
         tnlInfo->tnlId.tnlPriority = wrUmmPrioIdforQci[rabInfo->qci-1];
         tnlInfo->tnlType      = WR_TNL_NORMAL;
         tnlInfo->hoType       = WR_HO_TYPE_NONE; 
         wrCpyCmTptAddr(&tnlInfo->dstAddr, &(rabInfo->sgwAddr));
         if(rabInfo->sgwAddr.type == wrEmmCb.datAppAddr[WR_IPV4_IDX].type)
         {
            wrCpyCmTptAddr(&tnlInfo->srcAddr, &(wrEmmCb.datAppAddr[WR_IPV4_IDX]));
         }
         else if(rabInfo->sgwAddr.type == wrEmmCb.datAppAddr[WR_IPV6_IDX].type)
         {
            wrCpyCmTptAddr(&tnlInfo->srcAddr, &(wrEmmCb.datAppAddr[WR_IPV6_IDX]));
         }
         tnlInfo->remTeid = rabInfo->remTeid;
         wrUtlGetDscp(ueCb->cellId, rabInfo->qci, &(tnlInfo->dscp));
         rabInfo->tnlCfgState = WR_UMM_RAB_SETUP_CFG_SENT;
         wrIfmDamTnlCreatReq(transId, tnlInfo);
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for processing the responses for
 *         tunnel creation.
 *
 * @details
 *
 *     Function: wrUmmRabSetupPrcTnlMgmtCfm
 *
 *         Processing steps:
 *         - set the tunnel configuration flag for the particular DRB to
 *           WR_UMM_RAB_SETUP_CFG_SUCC 
 *         - copy the tunnel id to the local data structure
 *         - if any 1 of the tunnel creations failed,
 *           - set the configuration state to WR_UMM_RAB_SETUP_CFG_FAIL
 *         - if reponse for all the admitted RABs are successful, then,
 *           - set the RAB transaction state to WR_UMM_RAB_SETUP_DAM_CFG_DONE
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmRabSetupPrcTnlMgmtCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabSetupTrans        *rabSetup = &transCb->u.rabSetupTrans;
   WrUmmRabSetupRabInfo      *rabInfo;
   U8                        status;
   U8                        idx;

   idx     = transCb->msg->u.damCfm.transId & 0x0000000f;
   rabInfo = &rabSetup->rabs[idx];
   status  = transCb->msg->u.damCfm.status;
   if (status == WR_DAM_CFG_OK)
   {
      rabInfo->tnlCfgState = WR_UMM_RAB_SETUP_CFG_SUCC;
      rabInfo->lclTeid  = transCb->msg->u.damCfm.lclTeId;
   }
   else
   {
      rabInfo->tnlCfgState = WR_UMM_RAB_SETUP_CFG_FAIL;
      rabInfo->cause.causeTyp = CAUSE_TRANSPORT;
      rabInfo->cause.causeVal  = SztCauseTporttransport_resource_unavailableEnum;
      rabInfo->rabAllowed  = FALSE;
   }

   for(idx = 0; idx < rabSetup->numRabs; idx++)
   {
      rabInfo = &rabSetup->rabs[idx];
      /* Due to coupling modified such that it will either in IDLE state or in SENT state for allowed bearers*/
      if((rabInfo->rabAllowed == TRUE) && (rabInfo->tnlCfgState <= WR_UMM_RAB_SETUP_CFG_SENT))
      {
         RETVALUE(ROK);
      }
   }
   rabSetup->state = WR_UMM_RAB_SETUP_DAM_CFG_DONE;
   for (idx = 0; idx < rabSetup->numRabs; idx++)
   {
      rabInfo = &rabSetup->rabs[idx];
      if (rabInfo->tnlCfgState == WR_UMM_RAB_SETUP_CFG_FAIL)
      {
         /* Error_Cause: Filling the proper cause value in E-Rab setup failure
          * */
         rabSetup->rabs[idx].cause.causeTyp = CAUSE_TRANSPORT;
         rabSetup->rabs[idx].cause.causeVal  = SztCauseTporttransport_resource_unavailableEnum;
         rabSetup->curCfgRes = WR_UMM_RAB_SETUP_CFG_FAIL;
         rabSetup->rabs[idx].rabAllowed  = FALSE;
         RETVALUE(ROK);
      }
   }
   rabSetup->curCfgRes = WR_UMM_RAB_SETUP_CFG_SUCC;

   RETVALUE(ROK);
}

/** @brief This function is responsible for handling the RRC lower layer 
 *         configuration status.
 *
 * @details
 *
 *     Function: wrUmmRabSetupPrcRrcCfgCfm
 *
 *         Processing steps:
 *         - if the configuration status is FALSE,
 *           - set the configuration state to WR_UMM_RAB_SETUP_CFG_FAIL
 *         - else, set the configuration state to WR_UMM_RAB_SETUP_CFG_SUCC
 *         - set the RAB transaction state to WR_UMM_RAB_SETUP_STACK_CFG_DONE
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmRabSetupPrcRrcCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabSetupTrans        *rabSetup = &transCb->u.rabSetupTrans;
   NhuCfgCfmSdus             *rrcCfm;

   rrcCfm     = transCb->msg->u.rrcCfm;
   if (rrcCfm->sdu.isCfgSuccess == FALSE)
   {
      RLOG0(L_ERROR, "DRB Config failed");
      rabSetup->curCfgRes = WR_UMM_RAB_SETUP_CFG_FAIL;
   }
   else
   {
      rabSetup->curCfgRes = WR_UMM_RAB_SETUP_CFG_SUCC;
   }
   rabSetup->state = WR_UMM_RAB_SETUP_STACK_CFG_DONE;

   RETVALUE(ROK);
}

/** @brief This function is responsible for copying the DRB information to 
 *         the UE Control Block.
 *
 * @details
 *
 *     Function: wrUmmRabSetupCommitUeInfo
 *
 *         Processing steps:
 *         - copy the DRB information for those DRBs that are suceccessfully
 *           established.
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmRabSetupCommitUeInfo
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabSetupTrans        *rabSetup = &transCb->u.rabSetupTrans;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmRabSetupRabInfo      *rab;
   WrUeRAB                   *rabInfo;
   U8                        freeIdx;
   U8                        idx;
   WrUmmCellCb               *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
      if(cellCb == NULLP)
      {
         RLOG0(L_ERROR, " CellCb is NULL. ");
         RETVALUE(RFAILED);
      } 

   for(idx = 0; idx < rabSetup->numRabs; idx++)
   {
      rab = &(rabSetup->rabs[idx]);
      if (rab->rabAllowed == TRUE)
      {
         freeIdx = rab->erabId - WR_UMM_ERAB_OFFSET; 
         rabInfo = &(ueCb->rbInfo.rab[freeIdx]);
         rabInfo->inUse    = TRUE;
         rabInfo->eRABId   = rab->erabId;
         rabInfo->rbId     = rab->drbId;
         wrUtlGetRbMode(ueCb->cellId, rab->qci, &(rabInfo->rbMode));
         wrUtlGetRbDir(ueCb->cellId, rab->qci, &(rabInfo->rbDir));
         rabInfo->lchId    = rab->lchId;
         rabInfo->qci      = rab->qci;
         /* Fetching the lcgId from the table, initilized at wrSmDfltPrioInfoTbl */
         rabInfo->lcgId    = wrUmmGetLcgIdForQci(ueCb->cellId, rab->qci);

         /*Convert kBps to kbps. This is required during X2 HO*/
         rabInfo->mbr.dl   = rab->dlMbr * 8;
         rabInfo->mbr.ul   = rab->ulMbr * 8;
         if(rab->dlGbr != 0 || rab->ulGbr != 0)
         {
            rabInfo->isGbrQosPres = TRUE;
         }
         rabInfo->gbr.dl   = rab->dlGbr * 8;
         rabInfo->gbr.ul   = rab->ulGbr * 8;

         rabInfo->priority = rab->arpInfo.priorityLvl; 
         rabInfo->preempCap = rab->arpInfo.preemptCapability; 
         rabInfo->preempVul = rab->arpInfo.preemptVulnerability; 
         if(rabInfo->priority == cellCb->rabArpForEmerServ)
         {
            rabInfo->isEmergencyBearer = TRUE;
            ueCb->ueServTyp = WR_UMM_EMERGENCY_SERV; 
         }
         ueCb->rbInfo.numOfRbCfgd++;
         rabInfo->locEgtpTunId  = rab->lclTeid;
         rabInfo->remEgtpTunId  = rab->remTeid;
         /* SPS changes starts */
         rabInfo->isDlSpsEnabled  = rab->isDlSpsEnabled;
         rabInfo->isUlSpsEnabled  = rab->isUlSpsEnabled;
         /* updating the LCGID to 1 if SPS is enabled */
         if(TRUE == rab->isUlSpsEnabled)
         {
            /* Forcing the LCG ID to 1 since SPS is enabled */
            rabInfo->lcgId = WR_UMM_LCG_GRP_ID_1;  
         }
         /* SPS changes ends */
         wrCpyCmTptAddr(&rabInfo->sgwAddr, &(rab->sgwAddr));

         RLOG_ARG3(L_INFO, DBG_CRNTI,ueCb->crnti,"RAB Activated, RabId[%d] "
               "Local Tunel Id[%d] Remote Tunel Id[%d]", rabInfo->eRABId,
               rabInfo->locEgtpTunId, rabInfo->remEgtpTunId);
      }
   }

   if (rabSetup->dlAmbr != 0)
   {
      ueCb->rbInfo.aggBr.dl = rabSetup->dlAmbr;
   }

   if (rabSetup->ulAmbr != 0)
   {
      ueCb->rbInfo.aggBr.ul = rabSetup->ulAmbr;
   }

   if(rabSetup->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
   {
      wrUmmCommitUeRadioResDedInfo(ueCb, 
            &(rabSetup->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg));
   }
   /* SPS changes starts */
   if(rabSetup->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_UL_SPS_CFG)
   {
      ueCb->spsCfg.spsRnti = transCb->spsCfgTransCb.spsCfg.spsRnti;
      ueCb->spsCfg.spsCfgUl = transCb->spsCfgTransCb.spsCfg.spsCfgUl;
   }
   if(rabSetup->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG)
   {
      ueCb->spsCfg.spsRnti = transCb->spsCfgTransCb.spsCfg.spsRnti;
      ueCb->spsCfg.spsCfgDl = transCb->spsCfgTransCb.spsCfg.spsCfgDl;
   }
   /* SPS changes ends */

   RETVALUE(ROK);
}

/** @brief This function is responsible for filling the S1AP RAB Setup Response
 *         message.
 *
 * @details
 *
 *     Function: wrFillS1apRabSetupRsp
 *
 *         Processing steps:
 *         - fill the RAB Setup Response message with the following ies,
 *           UE ENB S1AP Id
 *           UE MME S1AP Id
 *           E-RAB Setup List
 *           E-RAB Failed List (if any RAB could not be setup)
 *
 * @param[in] ueCb     : UE Control Block
 * @param[in] rabSetup : RAB Transaction specific RAB information
 * @param[in, out] pdu  : S1AP PDU to be filled
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrFillS1apRabSetupRsp
(
WrUeCb                       *ueCb,
WrUmmRabSetupTrans           *rabSetup,
S1apPdu                      **pdu
)
{
   WrUmmRabSetupRabInfo      *rab;
   S1apPdu                   *szERABRspPdu = NULLP;
   SztSuccessfulOutcome      *succOut;
   SztE_RABSetupResp         *rabSetupRsp;
   S16                       ret = ROK;
   WrS1ConCb                 *s1apCon;
   U16                       idx;
   U16                       crntIe = 0; 
   U8                       numRabsFailed = 0;
   U32                       ieIdx;
   U8                       numRabsAllowed = 0;
   SztProtIE_Field_E_RABSetupRespIEs             *ie = NULLP;
   SztProtIE_SingleCont_E_RABSetupItemBrSUResIEs *rspMember = NULLP;
   U8                        offSet;
   U8                        cnt = 0;
   U8                        noComp = 0;
   WrMmeCb                   *mmeCb;

   RLOG0(L_DEBUG, "Filling E-RAB Setup Response");
   for (idx = 0; idx < rabSetup->numRabs; idx++)
   {
      if(rabSetup->rabs[idx].rabAllowed == TRUE)
      {
         numRabsAllowed++;
         /*Increment KPI for Number of E-RABs successfully established*/
         WR_INC_KPI_FUNC1(wrIncKpiFrERABEstabSuccPrQci,rabSetup->rabs[idx].qci);
      }
   }

   numRabsFailed = (rabSetup->numRabs) - numRabsAllowed;
   if(numRabsFailed)
   {
      noComp = 4;
   }
   else
   {
      noComp = 3;
   }

   s1apCon = ueCb->s1ConCb;
   if (cmAllocEvnt(sizeof(S1apPdu), SZ_MEM_SDU_SIZE, &wrCb.mem,
                                           (Ptr *)&szERABRspPdu) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(szERABRspPdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   succOut = &(szERABRspPdu->pdu.val.successfulOutcome);
   wrFillTknU8(&(succOut->pres), PRSNT_NODEF);
   wrFillTknU32(&(succOut->procedureCode), Sztid_E_RABSetup);
   wrFillTknU32(&(succOut->criticality), SztCriticalityrejectEnum);
   rabSetupRsp = &(succOut->value.u.sztE_RABSetupResp);
   wrFillTknU8(&(rabSetupRsp->pres),PRSNT_NODEF);

/*TODO Modify the Number of COmponent */

   /*List of IEs Filled
    * 1. UE ENB S1AP Id
    * 2. UE MME S1AP Id
    * 3. E-RAB Setup List
    * */
   if (cmGetMem(szERABRspPdu, (noComp * sizeof(SztProtIE_Field_E_RABSetupRespIEs)),
                   (Ptr *)&rabSetupRsp->protocolIEs.member) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Filling MME ID */
   ie = &rabSetupRsp->protocolIEs.member[crntIe++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);   
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);   
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1apCon->mme_ue_s1ap_id);  

   /* Filling ENB ID */
   ie = &rabSetupRsp->protocolIEs.member[crntIe++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum); 
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1apCon->enb_ue_s1ap_id);


   if (numRabsAllowed > 0)
   {
         /* ERAB Setup List  */
      ie = &rabSetupRsp->protocolIEs.member[crntIe++];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
      wrFillTknU32(&(ie->id), Sztid_E_RABSetupLstBrSURes);   
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum); 
      if ((cmGetMem(szERABRspPdu,
       (numRabsAllowed * sizeof(SztProtIE_SingleCont_E_RABSetupItemBrSUResIEs)),
              (Ptr *)&ie->value.u.sztE_RABSetupLstBrSURes.member)) != ROK)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      /* Locate ERAB to be setup List IE  */
      ieIdx = 0;
      for (idx = 0; idx < rabSetup->numRabs; idx++)
      {
         SztE_RABSetupItemBrSURes *rabIE;

         if (rabSetup->rabs[idx].rabAllowed == FALSE)
         {
            continue;
         }
         rspMember = &(ie->value.u.sztE_RABSetupLstBrSURes.member[ieIdx++]);
         wrFillTknU8(&(rspMember->pres), PRSNT_NODEF);  
         wrFillTknU32(&(rspMember->id), Sztid_E_RABSetupItemBrSURes);   
         wrFillTknU32(&(rspMember->criticality), SztCriticalityignoreEnum); 
         rab = &(rabSetup->rabs[idx]);
         rabIE = &(rspMember->value.u.sztE_RABSetupItemBrSURes);
         wrFillTknU8(&(rabIE->pres), PRSNT_NODEF); 
         wrFillTknU32(&rabIE->e_RAB_ID, rab->erabId);

         /* Retrieve the enb ip address */
         /* We know that the eNB and MME are connected, based on the same 
          * find the enb ip address type.
          */
         mmeCb = WR_EMM_MME_GET_MME(ueCb->mmeId);
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (mmeCb == NULLP)
         {
            RLOG0(L_ERROR,"wrFillS1apRabSetupRsp: mmeCb is NULL");
            RETVALUE(RFAILED);
         }
#endif

         if (wrEmmCb.datAppAddr[WR_IPV4_IDX].type == mmeCb->mmeAddr[0].type)
         {
            rabIE->transportLyrAddr.pres = PRSNT_NODEF;
            rabIE->transportLyrAddr.len = (U16)(WR_IPV4_ADDR_LEN * WR_BYTE_LEN);
            WR_GET_MEM(szERABRspPdu, WR_IPV4_ADDR_LEN, &rabIE->transportLyrAddr.val);
            for (cnt = 0; cnt < WR_IPV4_ADDR_LEN; cnt++)
            {
               offSet =(U8)((WR_IPV4_ADDR_LEN - (cnt + 1)) * WR_BYTE_LEN);
               rabIE->transportLyrAddr.val[cnt] =
                  (U8)(wrEmmCb.datAppAddr[WR_IPV4_IDX].u.ipv4TptAddr.address >> offSet);
            }
         }   
         else if (wrEmmCb.datAppAddr[WR_IPV6_IDX].type == mmeCb->mmeAddr[0].type)
         {
            rabIE->transportLyrAddr.pres = PRSNT_NODEF;
            rabIE->transportLyrAddr.len = (U16)(WR_IPV6_ADDR_LEN * WR_BYTE_LEN);
            WR_GET_MEM(szERABRspPdu, WR_IPV6_ADDR_LEN, &rabIE->transportLyrAddr.val);
            for (cnt = 0; cnt < WR_IPV6_ADDR_LEN; cnt++)
            {
               rabIE->transportLyrAddr.val[cnt] =
                  (U8)(wrEmmCb.datAppAddr[WR_IPV6_IDX].u.ipv6TptAddr.ipv6NetAddr[cnt]);
            }
         }

         wrFillTknStrOSXL1(&rabIE->gTP_TEID, 4, rab->lclTeid, &(szERABRspPdu->memCp));
         rabIE->iE_Extns.noComp.pres = NOTPRSNT;
         if (ieIdx >= numRabsAllowed)
         {
            break;
         }
    #ifdef WR_RSYS_KPI 
       SGetEpcTime(&(rabSetup->rabs[idx].erabEstabTm));
    #endif/*WR_RSYS_KPI*/
       WR_INC_KPI_FUNC3(wrUpdateErabEstabTimeMeanMax,rabSetup->rabs[idx].qci,rabSetup->rabs[idx].earbSetupReqTm,
        rabSetup->rabs[idx].erabEstabTm);   
      }
      wrFillTknU16(&(ie->value.u.sztE_RABSetupLstBrSURes.noComp), ieIdx);
      

   }

   if (numRabsFailed != 0)
   {
      SztE_RABLst                  *sztE_RABLst;
      SztProtIE_Field_E_RABItemIEs *failedIes;

      ie = &rabSetupRsp->protocolIEs.member[crntIe++];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
      wrFillTknU32(&(ie->id), Sztid_E_RABFailedToSetupLstBrSURes);   
      wrFillTknU32(&(ie->criticality),SztCriticalityignoreEnum); 
      sztE_RABLst = &(ie->value.u.sztE_RABLst);
      if (cmGetMem(szERABRspPdu,
                   (numRabsFailed * sizeof(SztProtIE_SingleCont_E_RABItemIEs)),
                   (Ptr *)&(sztE_RABLst->member)) != ROK)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         cmFreeMem(szERABRspPdu);
         RETVALUE(RFAILED);
      }

      ieIdx = 0;
      for(idx = 0; idx < rabSetup->numRabs; idx++)
      {
         rab = &(rabSetup->rabs[idx]);
         if (rab->rabAllowed == FALSE)
         {
            failedIes = &sztE_RABLst->member[ieIdx++];
            wrFillTknU8(&(failedIes->pres), PRSNT_NODEF);
            wrFillTknU32(&(failedIes->id), 
                                        Sztid_E_RABItem);
            wrFillTknU32(&(failedIes->criticality), SztCriticalityignoreEnum);
            wrFillTknU8(&(failedIes->value.u.sztE_RABItem.pres), PRSNT_NODEF);
            wrFillTknU32(&(failedIes->value.u.sztE_RABItem.e_RAB_ID),
                  rab->erabId);
            /*Error_Cause: Fill the proper cause value here*/
            RLOG3(L_DEBUG, "Failed to Setup: ERABID[%d]\t Cause Type[%ld]\t "
                  "Cause Value[%ld]", rab->erabId, rab->cause.causeTyp, 
                  rab->cause.causeVal);
            wrS1apFillCause(&(failedIes->value.u.sztE_RABItem.cause), &(rab->cause));
            failedIes->value.u.sztE_RABItem.iE_Extns.noComp.pres = NOTPRSNT;
            /*Increment KPI for Number of E-RABs failed to setup*/
            WR_INC_KPI_FUNC1(wrIncKpiFrERABFailPrCause, &(rab->cause));
            WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, rab->cause.causeTyp,
                  rab->cause.causeVal, INC_KPI_VALUE_BY_ONE);
            if (ieIdx >= numRabsFailed)
            {
               break;
            }
         }
      }
      wrFillTknU16(&(sztE_RABLst->noComp), ieIdx);
   }

   wrFillTknU16(&(rabSetupRsp->protocolIEs.noComp), crntIe);

   *pdu = szERABRspPdu;
   RETVALUE(ret);
}

/** @brief This function is responsible for filling and sending the RAB Setup 
 *         Response message to S1AP.
 *
 * @details
 *
 *     Function: wrUmmRabSetupSndS1apRsp
 *
 *         Processing steps:
 *         - invoke wrFillS1apRabSetupRsp to fill the response message
 *         - send the message to S1AP
 *         - set the transaction as complete
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmRabSetupSndS1apRsp
(
WrUmmTransCb              *transCb
)
{
   WrUmmRabSetupTrans        *rabSetup = &transCb->u.rabSetupTrans;
   WrUeCb                    *ueCb = transCb->ueCb;
   S1apPdu                   *s1apPdu = NULLP;

   wrFillS1apRabSetupRsp(ueCb, rabSetup, &s1apPdu);
   

   if (ROK != wrIfmS1apSndUeMsg(ueCb->s1ConCb->spConnId, s1apPdu))
   {
      RLOG0(L_ERROR, "Sending E-RAB SETUP RESPONSE Failed");
   }
   else
   {
      RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP] Sending E-RAB SETUP RESPONSE"
            "[MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]", ueCb->s1ConCb->mme_ue_s1ap_id,
            ueCb->s1ConCb->enb_ue_s1ap_id);
   }

   RETVALUE(ROK);
}


PRIVATE Void wrUmmTransRollBackHdlMsg
(
 WrUmmTransCb                 *transCb    
)
{
   WrUmmIncMsg               *msg = transCb->msg;  

   switch(msg->msgTyp)
   {
     case WR_UMM_SCHD_CFG:
     {
       WrUmmSchdCfgCfm           *schdCfm = &(transCb->msg->u.schdCfm);
       transCb->u.rabSetupTrans.schdUndoCfgCnt++;
       if (schdCfm->status != RGR_CFG_CFM_OK)
       {
          /*ccpu00127802*/
          RLOG1(L_ERROR,"ALARM SCHD Confirmation failed[%d] for RollBack state",
             schdCfm->status);
          transCb->u.rabSetupTrans.schdUndoCfgCnt--;
       }
      break;
     }
     case WR_UMM_DAM_CFM:
     {
       U32 status  = transCb->msg->u.damCfm.status;
       /*DAM Delete is always returns success*/
       if (status != WR_DAM_CFG_OK)
       {
          /*ccpu00127802*/
          RLOG1(L_ERROR,"ALARM DAM Confirmation failed[%d] for RollBack state",
             status);
       }
       break;
     }
     case WR_UMM_RRC_CFG:
     {
       NhuCfgCfmSdus             *rrcCfm;

       rrcCfm     = transCb->msg->u.rrcCfm;
       transCb->u.rabSetupTrans.rrcUndostate = TRUE;
       if (rrcCfm->sdu.isCfgSuccess == FALSE)
       {
         RLOG0(L_ERROR, "RRC Config failed");
         transCb->u.rabSetupTrans.rrcUndostate = FALSE;
       }
       break;
     }
   }  
}

/** @brief This function is called when any message related to a particular
 *         E-RAB SETUP is received. This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmRabSetupProcessor
 *
 *         Processing steps:
 *         - RRM is invoked based on the information reaceived in ERAB SETUP
 *         - Once RRM allows creation, scheduler is configured with the new
 *           RBs.
 *         - Upon receiving, scheduler confirmation, RRC is configured to
 *           send out RRC Reconfiguration and the lower layer configuration.
 *         - When RRC Reconfiguration complete is received, an ack is sent
 *           to MME.
 *         - Failure:
 *              Any faiulre causes RAB setup failure and corrective 
 *              configuration is performed to undo any changes to other
 *              layers.
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmRabSetupProcessor
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   WrUmmRabSetupTrans        *rabSetup = &(transCb->u.rabSetupTrans);
   Bool                      drxQciFeaFlag = FALSE;
   
   
  if(transCb->state == WR_UMM_TRANS_INUSE)
  {

   switch(msg->msgTyp)
   {
      case WR_UMM_S1_DAT_IND:
      {
         /* Parse the received RAB SETUP message and udpate transCb       */
         if (wrUmmParseRabSetupMsg(transCb) != ROK)
         {
            wrUmmRabSetupSndS1apRsp(transCb);
            wrUmmTransComplete(transCb);
            RETVALUE(ROK);
         }
        
         /* ccpu00130441 and ccpu00130443: Update the failure cause value for each RABS, if Handover is in progress */
         if((transCb->ueCb->mobCtrlState == WR_HO_INITIATED) || (transCb->ueCb->mobCtrlState == WR_HO_IN_PROGRESS))
         {
            wrUmmHOHandleRabSetup(transCb,transCb->ueCb->hoType);
         }

         /* Increment the reference and store the message in the transCb  */
         transCb->msg->refCnt++;
         rabSetup->msg = transCb->msg;

         transCb->u.rabSetupTrans.rrmUeRecfgRspMsg = NULLP;

         wrUmmPrcRabSetup(transCb);

         /*Update Statistics for ERAB Setup Attempt*/
         wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_DEDICATED,
               rabSetup->numRabs,WR_EMM_ERAB_ATTEMPT);

         /*Parse ERAB Setup Request and send Statistics update on QCI Basis*/
         wrEmmErabParseStat(WR_EMM_ERAB_DEDICATED,WR_EMM_ERAB_ATTEMPT,rabSetup,transCb->ueCb->cellId);

         if (rabSetup->numRabsAllowed == 0)
         {
            /* We just need to send a response and complete the trans     */
            wrUmmRabSetupSndS1apRsp(transCb);
            wrUmmTransComplete(transCb);
            RETVALUE(ROK);
         }

         /*Changes for LC interface*/
         break;
      }
      case WR_UMM_SCHD_CFG:
      {
         if(rabSetup->state == WR_UMM_RAB_SETUP_SCH_CFG_DRX)
         {
            /* committing the information to ueCb */
            wrUmmRabSetupCommitUeInfo(transCb);
            wrUmmRlsIncMsg(&transCb->u.rabSetupTrans.rrmUeRecfgRspMsg);
            /* send ERAB setup response to MME */
            wrUmmRabSetupSndS1apRsp(transCb);
            wrUmmTransComplete(transCb);
            wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_DEDICATED,
                           transCb->u.rabSetupTrans.numRabs,WR_EMM_ERAB_SUCCESS);
         }   
         else
         {       
            /* We have to wait to receive all the configuration confirms     */
            wrUmmRabSetupPrcSchdCfgCfm(transCb);
            if( (rabSetup->state == WR_UMM_RAB_SETUP_SCHD_CFG_DONE) &&
                  (rabSetup->curCfgRes == WR_UMM_RAB_SETUP_CFG_SUCC) )
            {
               wrUmmRabSetupCfgTnls(transCb);
               RETVALUE(ROK);
            }
         }
         break;
      }
      case WR_UMM_DAM_CFM:
      {
         /* process the tunnel create confirm */
         wrUmmRabSetupPrcTnlMgmtCfm(transCb);
         if ((rabSetup->state == WR_UMM_RAB_SETUP_DAM_CFG_DONE)&&
             (rabSetup->curCfgRes == WR_UMM_RAB_SETUP_CFG_SUCC))
         {

            /* Perform configuration towards stack and also dispatch UE   */
            /* reconfiguration message on Uu interface                    */
            if(ROK != wrUmmRabSetupPrepRrcMsgAndCfg(transCb))
            {
               /* TODO : HARI */
              RLOG1(L_DEBUG, "Failed to prepare RRC Reconfig message for UE : %d ",
                                                       transCb->ueCb->crnti);
               rabSetup->curCfgRes = WR_UMM_RAB_SETUP_CFG_FAIL;
            }
         }
         break;
      }
      case WR_UMM_RRC_CFG:
      {
         wrUmmRabSetupPrcRrcCfgCfm(transCb);
         break;
      }
      case WR_UMM_RRM_UERECFG_RESP: 
      {
         transCb->u.rabSetupTrans.rrmUeRecfgRspMsg = transCb->msg;
         transCb->msg->refCnt++;

         RmuUeRecfgRsp *ueRecfgRsp = transCb->msg->u.rrmUeRecfgResp;
         /*DRX QCI*/
         if((transCb->ueCb->isGBRBearerExists == NOT_IN_USE) && 
           (ueRecfgRsp->isGBRBearerExists == TRUE))
         {
            transCb->ueCb->isGBRBearerExists = RAB_ESTABLISHED;
         }
         
         if(ueRecfgRsp->bRecfgBitMap &
               RMU_RECFG_RSP_ERAB_CONFIG)
         {

            /*RRM_ARP: Update transCb->u.rabSetupTrans with
             * transCb->msg->u.rrmUeRecfgResp ERAB data*/
            wrUmmUpdateRabSetupTrans(rabSetup,ueRecfgRsp,transCb->ueCb); 

            if(ROK == wrGetDrxQCIFeatureFlag(transCb->ueCb->cellId,&drxQciFeaFlag))
            {
               if((TRUE == drxQciFeaFlag) && (TRUE == rabSetup->startAnr))
               {
                  wrUmmStartAnr(transCb->ueCb,TRUE);
               }
            }
            else
            {
               /*Generally control does not reach here. This is the case
                * if no cells are found*/
               RETVALUE(RFAILED);
            }

            /*RRM_ARP: If all ERABs requested are rejected by RRM then send ERAB
             * setup response and close the transaction*/
            if(ueRecfgRsp->bNoErabsRejected == rabSetup->numRabs)
            {
               wrUmmRabSetupSndS1apRsp(transCb);
               wrUmmTransComplete(transCb);
               RETVALUE(ROK);
            }
         }
         /*LC changes-moved from above*/

         /* We have some RABs to be setup. We first configures schduler   */
         /* Configure scheduler for the RABs and AMBR received            */
         wrUmmRabSetupCfgSchd(transCb);

         break;
      }
      case WR_UMM_TMR_EXPIRY:
      {
        switch (transCb->msg->u.tmrExpiry.timerEvnt )
        {
           case WR_TMR_RECONFIGURE: 
           {
              if (transCb->u.rabSetupTrans.rrmUeRecfgRspMsg != NULLP)
              {
                 wrUmmRlsIncMsg(&transCb->u.rabSetupTrans.rrmUeRecfgRspMsg);
              }


             /*Failed to Setup Dediaced ERAB : Re-Configuration Timer Expiry*/
             wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_DEDICATED,
                             transCb->u.rabSetupTrans.numRabsAllowed,WR_EMM_ERAB_FAIL_RADIOCONNECTIONWITHUELOST);

             transCb->errorCause = WR_UMM_CTXT_REL_SW_ERROR;
             transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
           }
           break;
           default:
           {
               RLOG1(L_ERROR, "Received [%d] invalid Timer Event",
                  transCb->msg->u.tmrExpiry.timerEvnt);
               RETVALUE(RFAILED);
           } 
        }
        break;
      }
      case WR_UMM_UU_IND_PDU:
      {
         NhuUlDcchMsg *ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
         NhuUL_DCCH_MsgTypc1 *c1 = &ulDcch->dcchMsg.message.val.c1;
         U8           rrcTransId = c1->val.rrcConRecfgnCompl.rrc_TrnsxnId.val;
/*Removed DRX flag*/
         RmuUeRecfgRsp *rrmUeRecfgResp = transCb->u.rabSetupTrans.rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
         S16          ret;
         /* We received confirmation from UE for the reconfiguration      */
         RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,
               "[OTA] Received RRCConnectionReconfigurationComplete");
         /* We release the RRC reconfiguration message transaction id     */
#ifdef WR_TEST_CODE
         if ( wrTrgRrcReConTO == TRUE )
         {
           RLOG0(L_DEBUG, "Dropping RRCCONRECFGNCOMPL!!");
           RETVALUE(ROK);
         }
#endif
         /* Stop the RRC Reconfiguration Timer */
         wrStopTransTmr(transCb,WR_TMR_RECONFIGURE);
         ret = wrUmmRelRrcTransId(transCb->ueCb, C1_RRCCONRECFGN, rrcTransId);
         if (ret != ROK)
         {
            /*ccpu00127802*/
            RLOG0(L_ERROR, "Release of transaction ID failed");
         }
         /* configuration of the EGTP Tunnels */
         if(rabSetup->state == WR_UMM_RAB_SETUP_STACK_CFG_DONE)
         {
            if((rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & 
                   RM_RCM_MAC_MAIN_CFG) &&
               (rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap & 
                   RM_RCM_MAC_MAIN_CFG_DRX))
            {
               wrUmmRabSetupDrxRelSchdUeRecfgReq(transCb); 
            }   
            else
            {
               /* committing the information to ueCb */
               wrUmmRabSetupCommitUeInfo(transCb);
               wrUmmRlsIncMsg(&transCb->u.rabSetupTrans.rrmUeRecfgRspMsg);
               /* send ERAB setup response to MME */
               wrUmmRabSetupSndS1apRsp(transCb);
               wrUmmTransComplete(transCb);
               wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_DEDICATED,
                              transCb->u.rabSetupTrans.numRabs,WR_EMM_ERAB_SUCCESS);
            }
         }  
         break;
      }
   }

   if (rabSetup->curCfgRes == WR_UMM_RAB_SETUP_CFG_FAIL)
   {
      /* This is an internal error that should not have happened. At this */
      /* point in the call, it is assumed to be better to release the     */
      /* call by sending S1AP CONTEXT RELEASE REQUEST to MME              */
      if (transCb->u.rabSetupTrans.rrmUeRecfgRspMsg != NULLP)
      {
         wrUmmRlsIncMsg(&transCb->u.rabSetupTrans.rrmUeRecfgRspMsg);
      }

      transCb->errorCause = WR_UMM_CTXT_REL_SW_ERROR;
      transCb->state = WR_UMM_TRANS_RELEASE_CALL; 

      /*Failed to Setup Dediaced ERAB : Re-Configuration Timer Expiry*/
      wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_DEDICATED,
                      transCb->u.rabSetupTrans.numRabsAllowed,WR_EMM_ERAB_FAIL_RADIORESOURCESNOTAVAILABLE);

   }
  }
  else if (transCb->state == WR_UMM_TRANS_STATE_ROLLBACK)
  {
      wrUmmTransRollBackHdlMsg(transCb);

    if ((transCb->u.rabSetupTrans.schdUndoCfgCnt ==
         transCb->u.rabSetupTrans.numRabs)&&
       (transCb->u.rabSetupTrans.rrcUndostate))
    {
       if (transCb->u.rabSetupTrans.rrmUeRecfgRspMsg != NULLP)
       {
          wrUmmRlsIncMsg(&transCb->u.rabSetupTrans.rrmUeRecfgRspMsg);
       }

       wrUmmTransComplete(transCb);
    } 
  }
  else
  {
      /*ccpu00127802*/
      RLOG0(L_ERROR, "Invalid Transaction state in wrUmmRabSetupProcessor");
  }

   RETVALUE(ROK);
}

/** @brief This function is  UL NAS transaction specific release function.
 *
 * @details
 *
 *     Function: wrUmmRabSetupTransRel
 *
 *         Processing steps:
 *         - free the rab information
 *         - free the stored RAB Setup Request message
 *
 * @param[in]  transCb : transaction inforamation
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmRabSetupTransRel
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabSetupTrans        *rabSetup = &transCb->u.rabSetupTrans;

   WR_FREE(rabSetup->rabs, rabSetup->numRabs * sizeof(*rabSetup->rabs));
   wrUmmRlsIncMsg((WrUmmIncMsg **)&rabSetup->msg);
   RETVALUE(ROK);
}
/** @brief This function constructs the UE Reconfiguration
 *         message towards RRC layer. 
 *
 * @details
 *
 *     Function: wrUmmBldUeCfgforRvrt
 *
 *         Processing steps:
 *         - It builds the information for Ue configuration
 *           parameters towards RRC layer. 
 *
 *
 * @param  NhuCfgReqSdus                   *event
 * @param  NhuRadioResourceConfigDedicated *radioRes
 * @param  WrUmmTransCb                    *transCb
 * @return ROK 
 */ 

PUBLIC S16 wrUmmBldUeCfgforRvrt
(
NhuCfgReqSdus                   *ueCfg, 
WrUmmTransCb                    *transCb
)
{

   WrUmmRabSetupTrans              *rabSetup = &(transCb->u.rabSetupTrans);
   U8                              idx;
   NhuRbRlsCfgList                 *rbCfgList;
   U8                              rrcIdx = 0;
   WrUmmRabSetupRabInfo            *rabInfo;

   /* ================================================== 
    *     * Fill UE Configuration
    * ================================================== */
    ueCfg->cfgInfo.ueCfgBitMsk = NHU_EXP_CFG_RB_CFG;
    ueCfg->cfgInfo.rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_RLS;
    rbCfgList = &(ueCfg->cfgInfo.rbCfgAllList.rbToRlsCfgList);
    rbCfgList->numOfRbCfgInfo = rabSetup->numRabs;

    WR_GET_MEM(ueCfg, (sizeof(NhuRbId) * rabSetup->numRabs),\
         &(rbCfgList->rbsToRel));
   for (idx = 0; idx < rabSetup->numRabs; idx++)
   {
      rabInfo = &(rabSetup->rabs[idx]);
      /* ================================================== 
      *                 * Fill UE Configuration
      * ================================================== */
      rbCfgList->rbsToRel[rrcIdx].rbId = 
                       rabInfo->drbId;
          /*ccpu00127802*/
      RLOG1(L_DEBUG, "DRBId [%x]",rabInfo->drbId);
      rbCfgList->rbsToRel[rrcIdx].rbTyp = CM_LTE_DRB;
      /* updating RRC Index */
      rrcIdx++;
   }/*End of idx for loop*/

   RETVALUE(ROK);
}

/** @brief This function prepares RRC messages to be revert back
 *         UE configuration towards RRC. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It prepares the RRC message RRC Reconfiguration Req
 *           and  delivers it to RRC through IFM module.
 *
 *
 * @param WrUmmTransCb  *transCb 
 * @return ROK/RFAILED 
 */ 

PRIVATE S16 wrUmmSetupRvrtRrcMsgAndCfg
(
WrUmmTransCb                 *transCb
)
{
   NhuCfgReqSdus                   *recfgMsg;
   WrUeCb                          *ueCb = transCb->ueCb;

   WR_ALLOCEVNT(&recfgMsg, sizeof(NhuCfgReqSdus));
   if (recfgMsg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   recfgMsg->hdr.cellId  = ueCb->cellId;
   recfgMsg->hdr.ueId    = ueCb->crnti;
   recfgMsg->hdr.transId = transCb->transId;

   /*Fill UE Configuration for lower layer revert*/
   if(ROK != wrUmmBldUeCfgforRvrt(recfgMsg,transCb))
   {
     RETVALUE(RFAILED); 
   }   
   /*Send message towards UE*/
   if (ROK != wrIfmRrcSndCfgReq(recfgMsg))
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe : failed");
      RETVALUE(RFAILED);
   } 
   RETVALUE(ROK);
}



/** @brief This function is called to handle logical channel
 *         confirmation from MAC Scheduler. 
 *
 * @details
 * Invoked by - wrUmmRvrtSchdLchCfgReq 
 *     Function: 
 *
 *         Processing steps:
 *         - It will revert back the LCH configuration during 
 *           Rab Setup because of re-establsihment.
 *           
 *           
 *
 *
 * @param WrUmmTransCb *transCb 
 * @param U8           idx Logical channel Id
 * @return ROK/RFAILED
 */ 


PRIVATE S16 wrUmmRvrtSchdLchCfgReq
(
WrUmmTransCb                 *transCb,
U8                           idx
)
{
   WrUmmRabSetupTrans        *rabSetup = &(transCb->u.rabSetupTrans);
   RgrCfgReqInfo             *cfgReq;
   U32                       transId;
   RgrDel                    *lchdelCfg;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Set last 4 bits as 1 for UE reconfiguration */
   transId   = transCb->transId | idx;
   lchdelCfg = &cfgReq->u.delInfo;

   cfgReq->action = RGR_DELETE;
   cfgReq->u.cfgInfo.cfgType = RGR_LCH_CFG;
   lchdelCfg->u.lchDel.cellId = transCb->ueCb->cellId;
   lchdelCfg->u.lchDel.crnti  = transCb->ueCb->crnti;
   lchdelCfg->u.lchDel.lcId   = rabSetup->rabs[idx].lchId;


   /* Dispatch the prepared message to scheduler                    */
   WrIfmSchdCfgUe(cfgReq, transId);

   RETVALUE(ROK);
}

/** @brief This function is called when any message related to a RRC Re-establsiment
 *         transaction during E-RAB SETUP transaction. 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmDefRabSetupIncReestProcessor 
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDefRabSetupIncReestProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
   WrUmmRabSetupTrans        *rabSetup = &(transCb->u.rabSetupTrans);
 
   if (rabSetup->curCfgRes == WR_UMM_RAB_SETUP_CFG_SUCC)
   {
      NhuUlCcchMsg               *ulCcchMsg;
      NhuRRCConReestbRqst        *reestab;
      NhuRRCConReestbRqst_r8_IEs *r8_IE;
      U8                         idx;
      U8                         transId = 0;
      /* This is converted to pointer as part of DAM Separation Changes */
      WrDamTnlId                 *damTnlId;
 
      ulCcchMsg = &(incTrans->msg->u.uuIndPdu->sdu.m.ulCcchMsg);
      reestab   = &(ulCcchMsg->ccchMsg.message.val.c1.val.rrcConReestbRqst);
      r8_IE     = &(reestab->criticalExtns.val.rrcConReestbRqst_r8);
      /* Stop the RRC Re-Cfg Timer coz of ReCfg failed cause*/
      wrStopTransTmr(transCb,WR_TMR_RECONFIGURE);

      if ((r8_IE->reestablishmentCause.val == NhuReestbCausereconfigurationFailEnum)
        || (r8_IE->reestablishmentCause.val == NhuReestbCauseotherFailEnum))
      {
 
         if (transCb->u.rabSetupTrans.rrmUeRecfgRspMsg != NULLP)
         {
            wrUmmRlsIncMsg(&transCb->u.rabSetupTrans.rrmUeRecfgRspMsg);
         }

         /*Revert Back RRC Configuration during Rab Setup*/
         wrUmmSetupRvrtRrcMsgAndCfg(transCb); 
      
         /* Revert back  the scheduler configuration  */
         for (idx = 0; idx < rabSetup->numRabs; idx++)
         {
            wrUmmRvrtSchdLchCfgReq(transCb, idx);
         }
         /*Revert back the GTPU tunnels created during RAB Setup*/ 
         for (idx = 0; idx < rabSetup->numRabs; idx++)
         {
            /*Update the Data App Tunnel Information*/
            WR_ALLOC_SHAREBLE_BUF(&damTnlId,sizeof(WrDamTnlId));
            if (NULLP == damTnlId)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            damTnlId->cellId = transCb->ueCb->cellId;
            damTnlId->crnti  = transCb->ueCb->crnti;
            damTnlId->drbId  = rabSetup->rabs[idx].drbId;
            transId = transId | idx;
            /* EGTPU Release*/
            //if (ROK != wrDamTnlDelReq(transId,&damTnlId))
            if (ROK != wrIfmDamTnlDelReq(transId,damTnlId))
            {
               /*ccpu00127802*/
               RLOG0(L_ERROR, "wrDamTnlDelReq is failed");
            }
         }
         /* At this point, we return ERAB Setup Response with failed list to MME.*/
         /* Continue with the Re-establsihment transaction                       */
         /*Overide the numRabsAllowed to zero to send failed List*/
         rabSetup->numRabsAllowed = 0; 
         for(idx = 0;idx < rabSetup->numRabs;idx++)
         {
            if(rabSetup->rabs[idx].rabAllowed  == TRUE)
            {
               rabSetup->rabs[idx].rabAllowed   = FALSE;
               rabSetup->rabs[idx].cause.causeTyp = WR_CAUSE_RADIONW;
               rabSetup->rabs[idx].cause.causeVal =
                       SztCauseRadioNwfailure_in_radio_interface_procedureEnum;
            }
         }
         /* send ERAB setup response to MME */
         wrUmmRabSetupSndS1apRsp(transCb);
      }
      /*Update the transaction state to handle confirms */
      transCb->state = WR_UMM_TRANS_STATE_ROLLBACK;
      /*TODO Transaction is complete till we handle confirms with old CRNTI*/
      wrUmmTransComplete(transCb); 
   }
   else
   {
     /*TODO Actions due to Radio Link failure*/
     RLOG0(L_ERROR, "Radio Link failure during RAB Setup received at "
        "Defered processor");
   }
   RETVALUE(ROK);
}

/* ccpu00130441 and ccpu00130443 */
/** @brief This function is called when any message related to Handover
 *         transaction during Rab Setup transaction. 
 *         This function will send ERAB Setup Response
 *         with failed bearer list for all bearers with proper cause value.
 *
 * @details
 *
 *     Function: wrUmmDefRabSetupIncHoProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDefRabSetupIncHoProcessor
(
 WrUmmTransCb                 *transCb,
 WrUmmTransCb                 *incTrans
 )
{
   /* fix ccpu00148237 */
   U8  hoType=incTrans->msg->u.internalMsg->u.hoTrans->hoType;
   wrUmmHOHandleRabSetup(transCb,hoType);

   /* Send the ERAB Setup Response with failed ERAB list */
   wrUmmRabSetupSndS1apRsp(transCb);

   wrUmmTransComplete(transCb);

   RETVALUE(ROK);
}

/** @brief This function is called when any message related to ue context release
 *         transaction during Rab Setup transaction. 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmDefRabSetupIncCtxtRelProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDefRabSetupIncCtxtRelProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
  wrUmmStopTransTimers(transCb);
  RETVALUE(ROK);
}


/** @brief This function is called when any message related to a particular
 *         transaction during E-RAB SETUP transaction. 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmRabSetupNewTransProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  inctransCb : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmRabSetupNewTransProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
     S16 ret = WR_UMM_TRANS_OK;
     switch(incTrans->transTyp)
     {
       case WR_UMM_RRC_REESTAB_TRANS: 
       {
          wrUmmDefRabSetupIncReestProcessor(transCb,incTrans);
          break;
       } 

       case WR_UMM_CTXT_REL_TRANS:
       {
          wrUmmDefRabSetupIncCtxtRelProcessor(transCb,incTrans);
          transCb->state = WR_UMM_TRANS_DONE;
          break;
       }
       /* ccpu00130441 and ccpu00130443 */
       case WR_UMM_S1_SRC_HO_TRANS:
       case WR_UMM_X2_SRC_HO_TRANS:
       {
          wrUmmDefRabSetupIncHoProcessor(transCb,incTrans);
          break;
       }
       case WR_UMM_UE_CTXT_MOD_TRANS:
       {
          /*As of now CTXT MOD FAIL is being sent for all CTXT MOD but it needs 
           * to be done only if CSFB Indicator is present*/
          WrUmmCtxtModTrans         *ctxtMod = &(incTrans->u.ctxtModTrans);
          ctxtMod->cause.causeTyp = CAUSE_RADIONW;
          ctxtMod->cause.causeVal =
             SztCauseRadioNwinteraction_with_other_procedureEnum;
          ctxtMod->status = WR_CTXT_MOD_SEND_FAIL;
          break;
       }
        case WR_UMM_ERAB_SETUP_TRANS:
       {
          RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Deferring "
                "incoming transaction %d ",incTrans->transTyp);
          WR_UMM_CPY_DEFR_MSG_TO_TRANSCB(transCb, incTrans->msg);
          ret = WR_UMM_TRANS_FAIL;
          break;
       }
       default:
       {
         RLOG1(L_INFO, "Ignoring incoming Transaction Type [%d] received at "
               "Rab Setup processor",incTrans->transTyp);
         break;
       } 
     }   
     RETVALUE(ret);
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
