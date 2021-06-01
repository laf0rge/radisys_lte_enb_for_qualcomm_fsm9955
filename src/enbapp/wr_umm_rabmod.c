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
               

     File:     wr_umm_rabmod.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=62;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_umm_msg.h"
#include "wr_umm.h"
#include "wr_umm_rabmod.h"
#include "wr_emm.h"
#include "wr_umm_trans.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_s1ap_msg.h"
#include "wr_umm_sch_msg.h"
#include "wr_ifm_schd.h"
#include "wr_ifm_rrm.h"
#include "wr_kpi.h"
#include "wr_utils.h"
#include "wr_ifm_s1ap.h"
#include "wr_ifm_rrc.h"
#include "wr_umm_drx.h"

#define WR_RAB_MODIFY_UE_TRANS_ID 0x0000000f

#ifdef WR_TEST_CODE
EXTERN U8 wrTrgRrcReConTO;
#endif
/** @brief This function performs the Logical channels
 *         reconfiguration to the MAC Scheduler with old 
 *         parameters. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It does the logical channels reconfiguration
 *           with old QOS parameters. 
 *         - It fetches the message to the MAC Scheduler 
 *           through IFM module.
 *
 *
 * @param WrUmmTransCb  *transCb Transaction CB
 * @param U8            idx      LCHID Index
 * @return ROK/RFAILED 
 */ 
PRIVATE S16 wrUmmRvrtSchdLchRecfgReq 
(
WrUmmTransCb                 *transCb,
U8                           idx
)
{
   WrUmmRabModifyTransCb     *rabMod = &transCb->u.rabModifyTransCb;
   RgrCfgReqInfo             *cfgReq;
   RgrLchRecfg               *lchRecfg;
   U32                       transId;
   WrUeRAB                   *rabInfo;
   U8                        freeIdx;
  

   freeIdx = (rabMod->rabs[idx].erabId) - WR_UMM_ERAB_OFFSET; 
   rabInfo = &(transCb->ueCb->rbInfo.rab[freeIdx]);

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Set last 4 bits as 1 for UE reconfiguration */
   transId = transCb->transId | idx;
   lchRecfg = &cfgReq->u.recfgInfo.u.lchRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_LCH_CFG;

   lchRecfg->cellId            = transCb->ueCb->cellId; 
   lchRecfg->crnti             = transCb->ueCb->crnti;
   lchRecfg->lcId              = rabMod->rabs[idx].lchId; 
   lchRecfg->dlRecfg.dlQos.qci = rabInfo->qci; 
   lchRecfg->dlRecfg.dlQos.gbr = rabInfo->gbr.dl;  
   lchRecfg->dlRecfg.dlQos.mbr = rabInfo->gbr.ul;
   lchRecfg->ulLchQciInfo.lcId = rabMod->rabs[idx].lchId;
   lchRecfg->ulLchQciInfo.qci = rabInfo->qci;
   lchRecfg->ulLchQciInfo.lcgId = rabInfo->lcgId;
   /* TODO DlSpsRecfg*/

   rabMod->rabs[idx].schdCfgState =  WR_RAB_MODIFY_SCHD_CFG_SENT;
   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}

/** @brief This function performs the Logical channel group
 *         reconfiguration to the MAC Scheduler with old 
 *         parameters. 
 *
 * @details
 *
 *     Function: wrUmmRvrtSchdLcgRecfgReq 
 *
 *         Processing steps:
 *         - It does the logical channel group reconfiguration
 *           with old QOS parameters. 
 *         - Revert back the effective UL/DL GBR
 *
 * @param WrUmmRabModifyRabInfo *incomingRabInfo Incoming RAB request
 * @param WrUmmTransCb          *transCb Transaction CB
 * @return ROK/RFAILED 
 **/
PRIVATE S16 wrUmmRvrtSchdLcgRecfgReq
(
WrUmmRabModifyRabInfo  *incomingRabInfo,
WrUmmTransCb           *transCb
)
{
   U32   prevUlGbr = 0;
   U32   prevUlMbr = 0;
   U32   prevQci;
   U32   newQci;
   U32   prevLcgId;
   U32   newLcgId;
   WrUeRAB   *existingRabInfo;

   existingRabInfo = &transCb->ueCb->rbInfo.rab[incomingRabInfo->erabId - 
                     WR_UMM_ERAB_OFFSET];

   prevQci = existingRabInfo->qci;
   newQci = incomingRabInfo->qci;

   prevLcgId = existingRabInfo->lcgId;
   newLcgId = wrUmmGetLcgIdForQci(transCb->ueCb->cellId, newQci);

   /* KWorks: Boundary checking*/
   if((prevLcgId >= WR_UMM_MAX_LCG_GRP_ID) || (newLcgId >= WR_UMM_MAX_LCG_GRP_ID))
   {
      RLOG0(L_ERROR,"LCG ID is exceeding MAX GRP ID");
      RETVALUE(RFAILED);
   }

   if (WR_UMM_IS_GBR_BEARER(prevQci))
   {
      prevUlGbr = existingRabInfo->gbr.ul;
      prevUlMbr = existingRabInfo->mbr.ul;
   }

   if(prevLcgId == newLcgId)
   {
      /* Revert the effective UL and DL GBR values*/
      if (WR_UMM_IS_GBR_BEARER(newQci))
      {
         transCb->ueCb->mappedLcgInfo[newLcgId].effUlGbr -= 
            WR_ABS_VAL(incomingRabInfo->ulGbr, prevUlGbr);
         transCb->ueCb->mappedLcgInfo[newLcgId].effUlMbr -= 
            WR_ABS_VAL(incomingRabInfo->ulMbr, prevUlMbr);

         wrUmmSchdUeLcgReCfg(transCb->ueCb, transCb->transId, newLcgId, \
               (transCb->ueCb->mappedLcgInfo[newLcgId].effUlGbr), \
               (transCb->ueCb->mappedLcgInfo[newLcgId].effUlMbr));

         transCb->u.rabModifyTransCb.expectedLcgCfgStatus++;
      }
   }
   else
   {
      /* removeing the gbr and mbr from the corresponding old lcg association */
      if (WR_UMM_IS_GBR_BEARER(prevQci))
      {
         /* if the previous LCG ID 0, we will not be send the config to schd */
         if(0 != prevLcgId)
         {
            /* Revert the effective UL and DL GBR values*/
            transCb->ueCb->mappedLcgInfo[prevLcgId].effUlGbr += prevUlGbr;
            transCb->ueCb->mappedLcgInfo[prevLcgId].effUlMbr += prevUlMbr;

            wrUmmSchdUeLcgReCfg(transCb->ueCb, transCb->transId, prevLcgId, \
                  (transCb->ueCb->mappedLcgInfo[prevLcgId].effUlGbr), \
                  (transCb->ueCb->mappedLcgInfo[prevLcgId].effUlMbr));

            transCb->u.rabModifyTransCb.expectedLcgCfgStatus++;
         }
      }
      if (WR_UMM_IS_GBR_BEARER(newQci))
      {
         /* Revert the effective UL and DL GBR values*/
         transCb->ueCb->mappedLcgInfo[newLcgId].effUlGbr -= incomingRabInfo->ulGbr;
         transCb->ueCb->mappedLcgInfo[newLcgId].effUlMbr -= incomingRabInfo->ulMbr;

         /* adding the gbr and mbr from the corresponding new lcg association */
         wrUmmSchdUeLcgReCfg(transCb->ueCb, transCb->transId, newLcgId, \
               (transCb->ueCb->mappedLcgInfo[newLcgId].effUlGbr), \
               (transCb->ueCb->mappedLcgInfo[newLcgId].effUlMbr));

         transCb->u.rabModifyTransCb.expectedLcgCfgStatus++;
      }
   }
   RETVALUE(ROK);
}

/** @brief This function reverts RRM config
 *
 * @details
 *
 *     Function: wrUmmRvrtRrmConfig 
 *
 *         Processing steps:
 *         - Revert back ERAB config
 *
 * @param WrUmmTransCb          *transCb Transaction CB
 * @param U32                    erabId Incoming RAB Id
 * @return ROK/RFAILED
 **/
PRIVATE S16 wrUmmRvrtRrmConfig
(
 WrUmmTransCb *transCb,
 U32           erabId
)
{
   WrUeRAB                      *existingRabInfo;
   RmuUeRecfgReq                *ueRecfgReq;
   RmuUErabAddModIe             *eRabModPtr;

   existingRabInfo = &transCb->ueCb->rbInfo.rab[erabId - WR_UMM_ERAB_OFFSET];

   WR_ALLOC(&ueRecfgReq, sizeof(RmuCommonMsg));
   if (ueRecfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   eRabModPtr = ueRecfgReq->stErabConfInfo.stErabAddModLst;
   ueRecfgReq->stErabConfInfo.usNoErabsToAddMod = 0;
   ueRecfgReq->stErabConfInfo.bNoErabsToRel = 0;
   ueRecfgReq->stErabConfInfo.enErabAction = RMU_MOD_ERAB;

   eRabModPtr->bErabId                        = existingRabInfo->eRABId;

   eRabModPtr->stErabQosInfo.bErabQCI           = existingRabInfo->qci;

   eRabModPtr->stErabQosInfo.stErabGbrInfo.uiErabDlMbr = existingRabInfo->mbr.dl;
   eRabModPtr->stErabQosInfo.stErabGbrInfo.uiErabUlMbr = existingRabInfo->mbr.ul;
   eRabModPtr->stErabQosInfo.stErabGbrInfo.uiErabDlGbr = existingRabInfo->gbr.dl;
   eRabModPtr->stErabQosInfo.stErabGbrInfo.uiErabUlGbr = existingRabInfo->gbr.ul;

   eRabModPtr->stErabQosInfo.stErabArpInfo.bErabPrioLvl =
      (U8)existingRabInfo->priority;
   eRabModPtr->stErabQosInfo.stErabArpInfo.bErabPreemptCap =
      (Bool)existingRabInfo->preempCap;
   eRabModPtr->stErabQosInfo.stErabArpInfo.bErabPreemptVul  =
      (Bool)existingRabInfo->preempVul;

   ueRecfgReq->stErabConfInfo.usNoErabsToAddMod++;
   ueRecfgReq->bCellId = transCb->ueCb->cellId;
   ueRecfgReq->usCrnti = transCb->ueCb->crnti;
   ueRecfgReq->uiTransId = transCb->transId;
   ueRecfgReq->bRecfgBitMap |= RMU_RECFG_REQ_ERAB_CONFIG;

   wrIfmRrmSndUeRecfgReq(ueRecfgReq);

   RETVALUE(ROK);
}

/** @brief This function handles SCHD CFM failure and triggers SCHD recfg for
 *         failed ERABs and reverts their cfg.
 *
 * @details
 *
 *     Function: wrUmmRabModHdlSchdCfmFail 
 *
 *         Processing steps:
 *         - Wait untill all ERABs are processed by SCHD
 *         - Revert LCG/LCH for failed ERABs only
 *
 * @param WrUmmTransCb  *transCb Transaction CB
 * @return ROK/RFAILED 
 **/
PRIVATE S16 wrUmmRabModHdlSchdCfmFail
(
   WrUmmTransCb *transCb
)
{
   WrUmmRabModifyTransCb     *rabTrans = &transCb->u.rabModifyTransCb;
   U32                       numRabs = rabTrans->numRabs;
   U32                       idx;
   U32                       failedRabCnt = 0;
   U32                       succRabCnt = 0;

   /*Check if all the requested Rabs failed due to Schd Cfm*/
   for(idx = 0; idx < numRabs; idx++)
   {
      if((FALSE == rabTrans->rabs[idx].rabAllowed) &&
            (WR_RAB_MODIFY_SCHD_CFM_FAIL == rabTrans->rabs[idx].schdCfgState))
      {
         /*Counting RABs failed due to SCHD CFG*/
         failedRabCnt++;
      }
      else if((TRUE == rabTrans->rabs[idx].rabAllowed) && 
            (WR_RAB_MODIFY_SCHD_CFM_SUCC == rabTrans->rabs[idx].schdCfgState))
      {
         /*Count the succesfully processed RABs*/
         succRabCnt++;
      }
   }

   /* Return if all RABs are yet to be processed at SCHD*/
   if(numRabs != (failedRabCnt + succRabCnt))
   {
      RETVALUE(ROK);
   }

   if(failedRabCnt)
   {
      /*All RABs have been processed by SCHD. Now revert SCHD cfg for failed RABs*/
      for (idx = 0; idx < numRabs; idx++)
      {
         if(rabTrans->cfgStateMsk & WR_UMM_RAB_MODIFY_LCH_CFG_SENT)
         {
            if((FALSE == rabTrans->rabs[idx].rabAllowed) &&
              (WR_RAB_MODIFY_SCHD_CFM_FAIL == rabTrans->rabs[idx].schdCfgState))
            {
               /* Revert LCH configuration*/
               wrUmmRvrtSchdLchRecfgReq(transCb,idx);

               rabTrans->rabs[idx].schdRvrtCfgCfmCnt++;

               /* Revert LCG configuration*/
               wrUmmRvrtSchdLcgRecfgReq(&rabTrans->rabs[idx],transCb);

               /*Change state to reverted so that upon receivng CFG CFM no
                * special handling shall be done.*/
               rabTrans->rabs[idx].schdCfgState = 
                  WR_RAB_MODIFY_SCHD_CFM_FAIL_RVRT;

               /*Revert RRM configuration.*/
               wrUmmRvrtRrmConfig(transCb,rabTrans->rabs[idx].erabId);

               RLOG_ARG1(L_INFO,DBG_CRNTI,transCb->ueCb->crnti,
                     "Reverting SCHD and RRM CFG for ERAB Id [%d]",rabTrans->rabs[idx].erabId);
            }

         }
      }/*end of FOR*/

      /*This is used to handle RRM response in processor*/
      rabTrans->rrmConfigRvrt = TRUE;

      if(numRabs == failedRabCnt)
      {
         rabTrans->allRabsFailed = TRUE;
      }
      
   }
   RETVALUE(ROK);
}
/** @brief This function commits the information into the
 *         UE control block after successfully configured
 *         at UE from tranasaction CB. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It copies the transient data from the transaction
 *           CB into UECB after receiving RECFG COMPLETE
 *           message from UE. 
 *
 *
 * @param WrUmmTransCb *transCb  
 * @return ROK 
 */ 
PRIVATE S16 wrUmmRabModifyCommitUeInfo
(
WrUmmTransCb              *transCb
)
{
   WrUmmRabModifyTransCb     *rabTrans = &transCb->u.rabModifyTransCb;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmRabModifyRabInfo     *rab;
   WrUeRAB                   *rabInfo;
   U8                        freeIdx;
   U8                        idx;
   WrUmmCellCb               *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
      if(cellCb == NULLP)
      {
         RETVALUE(RFAILED);
      }

   for(idx = 0; idx < rabTrans->numRabs; idx++)
   {
      if(rabTrans->rabs[idx].rabAllowed) 
      {   
         rab = &(rabTrans->rabs[idx]);
         if(rab->schdCfgState == WR_RAB_MODIFY_SCHD_CFM_SUCC)
         {
            freeIdx = rab->erabId - WR_UMM_ERAB_OFFSET; 
            rabInfo = &(ueCb->rbInfo.rab[freeIdx]);

            wrUtlGetRbMode(ueCb->cellId, rab->qci, &(rabInfo->rbMode));
            /*Convert back to kbps from kBps. This is requried in case of X2 HO*/
            rabInfo->mbr.dl   = rab->dlMbr * 8;
            rabInfo->mbr.ul   = rab->ulMbr * 8;
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
            else
            {
               rabInfo->isEmergencyBearer = FALSE; 
               /* Update the UEs service type based on existing bearer */
               wrUmmUpdtUeServType(transCb->ueCb);
            }
            /* SPS changes starts */
            rabInfo->isDlSpsEnabled  = rab->isDlSpsEnabled;
            rabInfo->isUlSpsEnabled  = rab->isUlSpsEnabled;
            /* SPS changes ends */
            wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_MODIFY,
                  WR_EMM_DFLT_POS_STEP,WR_EMM_ERAB_SUCCESS);
         }
      }  
   }

   if (rabTrans->dlAmbr != 0)
   {
      ueCb->rbInfo.aggBr.dl = rabTrans->dlAmbr;
   }

   if (rabTrans->ulAmbr != 0)
   {
      ueCb->rbInfo.aggBr.ul = rabTrans->ulAmbr;
   }
   /* SPS changes starts */
   /* Commit SPS configuration in the UE control block */
   if(rabTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_UL_SPS_CFG)
   {
      ueCb->spsCfg.spsRnti = transCb->spsCfgTransCb.spsCfg.spsRnti;
      ueCb->spsCfg.spsCfgUl = transCb->spsCfgTransCb.spsCfg.spsCfgUl;
   }
   if(rabTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG)
   {
      ueCb->spsCfg.spsRnti = transCb->spsCfgTransCb.spsCfg.spsRnti;
      ueCb->spsCfg.spsCfgDl = transCb->spsCfgTransCb.spsCfg.spsCfgDl;
   }
   /* SPS changes ends */

   RETVALUE(ROK);
}

/** @brief This function is called to handle configuration
 *         confirm from RRC layer. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It validates the confirmation from RRC whether it
 *           is success or failure. 
 *
 *
 * @param  WrUmmTransCb *transCb
 * @return ROK/RFAILED 
 */ 

PRIVATE S16 wrUmmRabModifyProcRrcCfgCfm
(
WrUmmTransCb            *transCb
)
{
   WrUmmRabModifyTransCb    *rabTrans = &transCb->u.rabModifyTransCb;
   NhuCfgCfmSdus            *rrcCfm;
   //U32                      cfgCfmTrId;

   rrcCfm = transCb->msg->u.rrcCfm;
  // cfgCfmTrId = rrcCfm->hdr.transId;
   if(rrcCfm->sdu.isCfgSuccess == FALSE)
   {
      rabTrans->isDrbCfgDone = FALSE;
      /* logging error */
      RLOG3(L_ERROR, "DRB Config failed with config error type[%d] "
         "error cause[%d] pres[%d]", rrcCfm->sdu.cfgError.errType,
         rrcCfm->sdu.cfgError.errCause, rrcCfm->sdu.cfgError.errPres);
      
      RETVALUE(RFAILED);
   }
   else
   {
      rabTrans->isDrbCfgDone = TRUE;
   }
   RETVALUE(ROK);
}
/** @brief This function performs the UE Reconfiguration
 *         to the MAC Scheduler. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It does the UE reconfiguration to the MAC 
 *           Scheduler with new QOS parameters from MME.
 *         - It delivers the UE RECFG REQ to SCHD through
 *           IFM module. 
 *
 *
 * @param WrUmmTransCb *transCb 
 * @return ROK 
 */ 
PRIVATE S16 wrUmmRabModifySchdUeRecfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabModifyTransCb     *rabMod = &transCb->u.rabModifyTransCb;
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   U32                       transId;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Set last 4 bits as 1 for UE reconfiguration */
   transId = transCb->transId | WR_RAB_MODIFY_UE_TRANS_ID;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   if ((rabMod->dlAmbr != 0) || (rabMod->ulAmbr != 0))
   {
      ueRecfg->ueRecfgTypes = RGR_UE_QOS_RECFG;
      ueRecfg->ueQosRecfg.ambrPres = TRUE;
      ueRecfg->ueQosRecfg.dlAmbr   = rabMod->dlAmbr / 8;
      ueRecfg->ueQosRecfg.ueBr     = rabMod->ulAmbr / 8;
   }


   if (rabMod->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
   {
      wrUmmSchdFillUeRadioResCfgInfo(ueRecfg, 
            (rabMod->rrmUeRecfgRspMsg->u.rrmUeRecfgResp), transCb->ueCb,FALSE);
   }
   /* SPS changes starts */
   /* The initialisation from the ueCb is required for the following case:
    * 1) If the SPS was already enabled for the UE, thus RRM doesnot allocate SPS
    *    - The SR mask for all the LCs should be set for rel 9 UE based on this.
    * 2) If the SPS was neither enabled previously nor it is enabled by RRM
    *    - Donot enable SR Mask for any of the LCs based on this. */
   wrUmmFillSpsSchdReCfgUe(transCb->transTyp, transCb->ueCb, rabMod->rrmUeRecfgRspMsg->u.rrmUeRecfgResp, \
         ueRecfg, rabMod->numRabs, (Void *)(rabMod->rabs), &transCb->spsCfgTransCb.spsCfg, \
         &transCb->spsCfgTransCb.isUeDlSpsEnabled, &transCb->spsCfgTransCb.isUeUlSpsEnabled);
   /* Storing the existing configuration */
   /* SPS changes ends */
   
   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}
/** @brief This function performs the Logical channels
 *         reconfiguration to the MAC Scheduler. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It does the logical channels reconfiguration
 *           with new QOS parameters received from RRM. 
 *         - It fetches the message to the MAC Scheduler 
 *           through IFM module.
 *
 *
 * @param WrUmmTransCb  *transCb Transaction CB
 * @param U8            idx      LCHID Index
 * @return ROK/RFAILED 
 */ 
PRIVATE S16 wrUmmRabModifySchdLchRecfgReq
(
WrUmmTransCb                 *transCb,
U8                           idx
)
{
   WrUmmRabModifyTransCb     *rabMod = &transCb->u.rabModifyTransCb;
   RgrCfgReqInfo             *cfgReq;
   RgrLchRecfg               *lchRecfg;
   U32                       transId;
   /* SPS changes starts */
   WrUeRAB                   *rabInfo = NULLP;
   U8                        rabIdx = 0;
   /* SPS changes ends */
   U32                       prevUlGbr;
   U32                       prevUlMbr;
   U8                        prevLcgId;
   U8                        newLcgId;
   U8                        prevQci;
   U8                        newQci;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Set last 4 bits as 1 for UE reconfiguration */
   transId = transCb->transId | idx;
   lchRecfg = &cfgReq->u.recfgInfo.u.lchRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_LCH_CFG;

   lchRecfg->cellId            = transCb->ueCb->cellId; 
   lchRecfg->crnti             = transCb->ueCb->crnti;
   lchRecfg->lcId              = rabMod->rabs[idx].lchId; 
   lchRecfg->dlRecfg.dlQos.qci = rabMod->rabs[idx].qci; 
   lchRecfg->dlRecfg.dlQos.gbr = rabMod->rabs[idx].dlGbr;  
   lchRecfg->dlRecfg.dlQos.mbr = rabMod->rabs[idx].dlMbr;
#ifdef LTE_L2_MEAS
   lchRecfg->lcgId = wrUmmGetLcgIdForQci(transCb->ueCb->cellId, rabMod->rabs[idx].qci);
#endif
   lchRecfg->ulLchQciInfo.lcId = rabMod->rabs[idx].lchId;
   lchRecfg->ulLchQciInfo.qci = rabMod->rabs[idx].qci; 
   lchRecfg->ulLchQciInfo.lcgId = lchRecfg->lcgId;

   /* SPS changes starts */
   if (rabMod->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG)
   {
      /* Modification in SPS configuration */
      if(rabMod->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stErabAcceptLst[idx].bIsDlSpsEnabled == TRUE)
      {
         /* Enable SPS Configuration */
         /* Since there is no RBC and both RRM and APP need an enhancement. So, here there can be two approaches 
          * 1) Either assume that the ERAB sent to RRM is being received in the same order in the accepted list
          * 2) Or using a loop compare ERAB ids and find out if bIsSpsEnabled == TRUE 
          * Approach one is assumed as of now to avoid unnecessary loop. Thus accessed directly with idx */
         lchRecfg->dlRecfg.dlSpsRecfg.isSpsEnabled = TRUE;
         rabMod->rabs[idx].isDlSpsEnabled = TRUE;
         RLOG2(L_DEBUG, "wrUmmRabModifySchdLchRecfgReq: LcId=[%u] changed to SPS LC. isDlSpsEnabled=[%u]", \
               rabMod->rabs[idx].lchId, rabMod->rabs[idx].isDlSpsEnabled);
      }
      else
      {
         /* Disable SPS configuration for this LC */
         lchRecfg->dlRecfg.dlSpsRecfg.isSpsEnabled = FALSE;
         rabMod->rabs[idx].isDlSpsEnabled = FALSE;
         RLOG2(L_DEBUG, "wrUmmRabModifySchdLchRecfgReq: LcId=[%u] changed to non-SPS LC. isDlSpsEnabled=[%u]", \
               rabMod->rabs[idx].lchId, rabMod->rabs[idx].isDlSpsEnabled);
      }
   }
   else
   {
      /* This means that the SPS configuration is unmodified i.e. if
       * the SPS  was configured, mark isSpsEnabled==TRUE but if the
       * SPS was not configured, mark isSpsEnabled==FALSE, which ever
       * was present in the rabInfo->isDlSpsEnabled */
      rabIdx = (U8)(rabMod->rabs[idx].erabId - (U8)WR_UMM_ERAB_OFFSET);
      rabInfo = &(transCb->ueCb->rbInfo.rab[rabIdx]);
      lchRecfg->dlRecfg.dlSpsRecfg.isSpsEnabled = rabInfo->isDlSpsEnabled;
      rabMod->rabs[idx].isDlSpsEnabled = rabInfo->isDlSpsEnabled;
      RLOG2(L_DEBUG, "wrUmmRabModifySchdLchRecfgReq: SPS unmodified for lcId=[%u] isDlSpsEnabled=[%u]", \
            rabMod->rabs[idx].lchId, rabMod->rabs[idx].isDlSpsEnabled);
   }
   /* SPS changes ends */

   /*00127049 : Configuring Scheduler for RAB's (QoS) state updation*/
    rabMod->rabs[idx].schdCfgState = WR_RAB_MODIFY_SCHD_CFG_SENT;

   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);

   prevQci = transCb->ueCb->rbInfo.rab[rabMod->rabs[idx].erabId - WR_UMM_ERAB_OFFSET].qci;
   newQci = rabMod->rabs[idx].qci;
   prevLcgId = transCb->ueCb->rbInfo.rab[rabMod->rabs[idx].erabId - WR_UMM_ERAB_OFFSET].lcgId;
   newLcgId = wrUmmGetLcgIdForQci(transCb->ueCb->cellId, newQci);
   prevUlGbr = 0;
   prevUlMbr = 0;
   if (WR_UMM_IS_GBR_BEARER(prevQci))
   {
      prevUlGbr = transCb->ueCb->rbInfo.rab[rabMod->rabs[idx].erabId - WR_UMM_ERAB_OFFSET].gbr.ul;
      prevUlMbr = transCb->ueCb->rbInfo.rab[rabMod->rabs[idx].erabId - WR_UMM_ERAB_OFFSET].mbr.ul;
   }

   if(prevLcgId == newLcgId)
   {
      /* LCG association has not changed. Just update GBR,MBR  */
      if (WR_UMM_IS_GBR_BEARER(newQci))
      {
         transCb->ueCb->mappedLcgInfo[newLcgId].effUlGbr += rabMod->rabs[idx].ulGbr - prevUlGbr;
         transCb->ueCb->mappedLcgInfo[newLcgId].effUlMbr += rabMod->rabs[idx].ulMbr - prevUlMbr;
         wrUmmSchdUeLcgReCfg(transCb->ueCb, transCb->transId, newLcgId, \
               (transCb->ueCb->mappedLcgInfo[newLcgId].effUlGbr), \
               (transCb->ueCb->mappedLcgInfo[newLcgId].effUlMbr));
         rabMod->cfgStateMsk |= WR_UMM_RAB_MODIFY_LCG_CFG_SENT;
         rabMod->lcgCfgStatus[newLcgId] = WR_UMM_RAB_MODIFY_LCG_CFG_SENT;
         rabMod->expectedLcgCfgStatus++;
      }
   }
   else
   {
      /* removeing the gbr and mbr from the corresponding old lcg association */
      if (WR_UMM_IS_GBR_BEARER(prevQci))
      {
         /* of the previous LCG ID 0, we will not be send the config to schd */
         if(0 != prevLcgId)
         {
            transCb->ueCb->mappedLcgInfo[prevLcgId].effUlGbr -= prevUlGbr;
            transCb->ueCb->mappedLcgInfo[prevLcgId].effUlMbr -= prevUlMbr;
            wrUmmSchdUeLcgReCfg(transCb->ueCb, transCb->transId, prevLcgId, \
                  (transCb->ueCb->mappedLcgInfo[prevLcgId].effUlGbr), \
                  (transCb->ueCb->mappedLcgInfo[prevLcgId].effUlMbr));
            rabMod->lcgCfgStatus[prevLcgId] = WR_UMM_RAB_MODIFY_LCG_CFG_SENT;
            rabMod->cfgStateMsk |= WR_UMM_RAB_MODIFY_LCG_CFG_SENT;
            rabMod->expectedLcgCfgStatus++;
         }
      }
      if (WR_UMM_IS_GBR_BEARER(newQci))
      {
         transCb->ueCb->mappedLcgInfo[newLcgId].effUlGbr += rabMod->rabs[idx].ulGbr;
         transCb->ueCb->mappedLcgInfo[newLcgId].effUlMbr += rabMod->rabs[idx].ulMbr;
         /* adding the gbr and mbr from the corresponding new lcg association */
         wrUmmSchdUeLcgReCfg(transCb->ueCb, transCb->transId, newLcgId, \
               (transCb->ueCb->mappedLcgInfo[newLcgId].effUlGbr), \
               (transCb->ueCb->mappedLcgInfo[newLcgId].effUlMbr));
         rabMod->lcgCfgStatus[newLcgId] = WR_UMM_RAB_MODIFY_LCG_CFG_SENT;
         rabMod->cfgStateMsk |= WR_UMM_RAB_MODIFY_LCG_CFG_SENT;
         rabMod->expectedLcgCfgStatus++;
      }
   }
   RETVALUE(ROK);
}
/** @brief This function handles the UE and LCHs reconfiguration
 *         to the MAC Scheduler. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It sends UE Reconfiguration with new QOS parameters
 *           to the MAC scheduler.
 *         - It sends logical channels Reconfiguration with new
 *           QOS parameters to the MAC scheduler.  
 *
 *
 * @param WrUmmTransCb  *transCb 
 * @return ROK 
 */ 
PRIVATE S16 wrUmmRabModifyCfgSchd
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabModifyTransCb        *rabMod = &transCb->u.rabModifyTransCb;
   U32                          idx;
   /* SPS changes starts */
   U8                           rabIdx          = 0;
   U32                          count           = 0;
   RmuUeRecfgRsp                *rrmUeRecfgResp = rabMod->rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
   RmuUeSpsCfg                  *stRmuSpsCfg    = &rrmUeRecfgResp->stUeRadioResCfg.stSpsCfg;
   /* SPS changes ends */

   /*Convert to kBps from kbps*/
   for (idx = 0; idx < rabMod->numRabs; idx++)
   {
      rabMod->rabs[idx].ulMbr /= 8; 
      rabMod->rabs[idx].dlMbr /= 8; 
      rabMod->rabs[idx].ulGbr /= 8; 
      rabMod->rabs[idx].dlGbr /= 8; 
   }

   /* SPS changes starts */
   /* The initialisation from the ueCb is required for the following case:
    * 1) If the SPS was already enabled for the UE, thus RRM doesnot allocate SPS
    *    - The SR mask for all the LCs should be set for rel 9 UE based on this.
    * 2) If the SPS was neither enabled previously nor it is enabled by RRM
    *    - Donot enable SR Mask for any of the LCs based on this. */
   for (count = 0; count < rabMod->numRabs; count++)
   {
      if ((rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_UL_SPS_CFG) && \
            (transCb->ueCb->spsCfg.spsCfgUl.bIsSpsEnabled != stRmuSpsCfg->stSpsConfigUl.bIsSpsEnabled) && \
            (stRmuSpsCfg->stSpsConfigUl.bIsSpsEnabled == FALSE))
      {
         /* Handles the case when SPS bearer is modified to non-SPS bearer */
         rabMod->rabs[count].isUlSpsEnabled = FALSE;
      }
      else
      {
         rabIdx = (U8)(rabMod->rabs[count].erabId - (U8)WR_UMM_ERAB_OFFSET); 
         rabMod->rabs[count].isUlSpsEnabled = transCb->ueCb->rbInfo.rab[rabIdx].isUlSpsEnabled;
      }
      RLOG3(L_DEBUG, "wrUmmRabModifyCfgSchd: count=[%u] lcId[%u] isUlSpsEnabled=[%u]\n", \
            count, rabMod->rabs[count].lchId, rabMod->rabs[count].isUlSpsEnabled);
   }
   
   transCb->spsCfgTransCb.isUeUlSpsEnabled = transCb->ueCb->spsCfg.spsCfgUl.bIsSpsEnabled;
   transCb->spsCfgTransCb.isUeDlSpsEnabled = transCb->ueCb->spsCfg.spsCfgDl.bIsSpsEnabled;
   /* SPS changes ends */
   /* Configure the scheduler with the updated AMBR values                */
   if ((rabMod->dlAmbr != 0) || (rabMod->ulAmbr != 0) ||
      (rabMod->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap))
   {
      wrUmmRabModifySchdUeRecfgReq(transCb);
      rabMod->cfgStateMsk |= WR_UMM_RAB_MODIFY_UE_CFG_SENT;
   }
   /* Make the expected LCG config status to 0*/
   rabMod->expectedLcgCfgStatus = 0;

   /* Configure the scheduler for each new RAB being modified.             */
   for (idx = 0; idx < rabMod->numRabs; idx++)
   {
     /* Rabs which are successfull only then configure SCH*/ 
     if(rabMod->rabs[idx].rabAllowed) 
     {   
        wrUmmRabModifySchdLchRecfgReq(transCb,idx);
        rabMod->cfgStateMsk |= WR_UMM_RAB_MODIFY_LCH_CFG_SENT;
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
 *     Function: wrUmmRabModDrxReCfgSchdUeRecfgReq
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
PRIVATE S16 wrUmmRabModDrxReCfgSchdUeRecfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabModifyTransCb     *rabMod = &transCb->u.rabModifyTransCb;
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   RmuUeRecfgRsp             *rrmUeRecfgResp = rabMod->rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
   U32                       transId;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   transId = transCb->transId | WR_RAB_MODIFY_UE_TRANS_ID;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   /* Mark the transaction that UE config has been sent */
   rabMod->state = WR_UMM_RAB_MODIFY_SCHD_DRX_CFG_DONE;
   wrUmmSchdFillUeDrxCfgInfo(ueRecfg, (WrDrxCfg *)&rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.stRmDrxCfg);
   
   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
} 
/*Removed DRX flag*/


/** @brief This function check the duplicate of the bearer ID
 *             received in the ERAB mod message.
 *
 * @details
 *
 *     Function: wrUmmChkRabIdInRabModLst
 *
 *         Processing steps:
 *              This function check the duplicate of the bearer ID
 *               received in the ERAB mod message.
 *
 *
 * @param WrUmmTransCb  *transCb 
 * @return ROK 
 */ 
PRIVATE S16 wrUmmChkRabIdInRabModLst
(
SztE_RABToBeMdfdLstBrModReq  *rabLst,
WrUmmRabModifyTransCb        *rabTrans,
U8                          indx,
U8                          *idx
)
{
  U32                          eRabId;
  U32                          numRabs;
  U8                           dupFlag = FALSE;
  U8                           eRabIdx = 0;
  SztE_RABToBeMdfdItemBrModReq *rabInfo;

  numRabs = rabLst->noComp.val;
  eRabId = rabLst->member[indx].value.u.sztE_RABToBeMdfdItemBrModReq.e_RAB_ID.val;
  eRabIdx = (indx + 1);
  while(eRabIdx < numRabs)
  {
    rabInfo = &rabLst->member[eRabIdx].value.u.sztE_RABToBeMdfdItemBrModReq;
    if(eRabId == rabInfo->e_RAB_ID.val)
    { 
       rabInfo->pres.pres = NOTPRSNT;
       dupFlag = TRUE;
    }
    eRabIdx++;
  }
  if(dupFlag)
  {
    rabLst->member[indx].value.u.sztE_RABToBeMdfdItemBrModReq.pres.pres = NOTPRSNT;
    rabTrans->rabs[*idx].erabId  = eRabId;
    rabTrans->rabs[*idx].rabAllowed     = FALSE;
    rabTrans->rabs[*idx].cause.causeTyp = CAUSE_RADIONW;
    rabTrans->rabs[*idx].cause.causeVal = SztCauseRadioNwmultiple_E_RAB_ID_instancesEnum;
    (*idx)++;
    RETVALUE(RFAILED);
  } 
  RETVALUE(ROK);
}


/** @brief This function parses the ERABs to be modified
 *         list from the message from MME. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It parses the ERABs list to be modified along with the
 *           all the QOS parameters,NAS PDU for each RAB and updates
 *           the information into the trans CB. 
 *
 *
 * @param WrUmmRabModifyTransCb       *rabTrans 
 * @param SztE_RABToBeMdfdLstBrModReq *rabLst
 * @param WrUeCb                      *ueCb
 * @return ROK/RFAILED 
 */ 
PRIVATE S16 wrUmmPrcRabModifyLst
(
WrUmmRabModifyTransCb           *rabTrans,
SztE_RABToBeMdfdLstBrModReq     *rabLst,
WrUeCb                          *ueCb
)
{
   U32                          numRabs;
   U32                          idx = 0;
   U32                          indx = 0;
   SztE_RABToBeMdfdItemBrModReq *rabInfo;
   S16                          ret = RFAILED;

   numRabs = rabLst->noComp.val;
   WR_ALLOC(&rabTrans->rabs, numRabs * sizeof(WrUmmRabModifyRabInfo));
   if (rabTrans->rabs == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ret);
   }
   for(indx = 0;indx < numRabs; indx++)
   {
      rabInfo = &rabLst->member[indx].value.u.sztE_RABToBeMdfdItemBrModReq;

      rabTrans->rabs[idx].erabId = rabInfo->e_RAB_ID.val;

      if((TRUE == rabTrans->isUeAggrMaxBitRateRcvd) && 
         (0 == rabTrans->dlAmbr) && (0 == rabTrans->ulAmbr))
      {
         rabTrans->rabs[idx].rabAllowed     = FALSE;
         rabTrans->rabs[idx].cause.causeTyp = WR_CAUSE_PROTOCOL;
         rabTrans->rabs[idx].cause.causeVal  = SztCauseProtsemantic_errorEnum;
         idx++;
         continue;
      }
      /*Increment KPI for E-RAB modification Number of E-RABs attempted to 
       modify the QoS parameter*/
      WR_INC_KPI_FUNC1(wrIncKpiFrERABModQoSAttPrQci,rabInfo->e_RABLvlQoSParams.
                                                            qCI.val);      
      if((rabInfo->pres.pres == NOTPRSNT) || 
         (wrUmmChkRabIdInRabModLst(rabLst,rabTrans, indx,(U8*)&idx) != ROK))
      {
         continue;
      }

      /* check that received ERAB ID falls in the out of range */
      if((rabInfo->e_RAB_ID.val < WR_UMM_ERAB_OFFSET) || 
         (rabInfo->e_RAB_ID.val > WR_UMM_MAX_ERABID))
      {
         /*Error_Cause: Filling proper cause values when no e_RAB_ID is present
          * for Rab-Modification
          */
         rabTrans->rabs[idx].rabAllowed     = FALSE;
         rabTrans->rabs[idx].cause.causeTyp = CAUSE_RADIONW;
         rabTrans->rabs[idx].cause.causeVal  = SztCauseRadioNwunknown_E_RAB_IDEnum;
         idx++;
         continue;
      }

      /* check the GBR bearer has Qos parameters or not*/
      if ((rabInfo->e_RABLvlQoSParams.gbrQosInform.pres.pres == PRSNT_NODEF) && (rabInfo->e_RABLvlQoSParams.qCI.val <= 4))
      {
         SztGBR_QosInform    *gbrIe;

         gbrIe = &rabInfo->e_RABLvlQoSParams.gbrQosInform;
         wrGetBitRate(&gbrIe->e_RAB_MaxBitrateDL, &rabTrans->rabs[idx].dlMbr);
         wrGetBitRate(&gbrIe->e_RAB_MaxBitrateUL, &rabTrans->rabs[idx].ulMbr);
         wrGetBitRate(&gbrIe->e_RAB_GuaranteedBitrateDL, 
                  &rabTrans->rabs[idx].dlGbr);
         wrGetBitRate(&gbrIe->e_RAB_GuaranteedBitrateUL, 
                &rabTrans->rabs[idx].ulGbr);
      }
      else if(rabInfo->e_RABLvlQoSParams.qCI.val <= 4)
      {
         /* Fix for ccpu00125875: Support for handling GBR bearer with out 
         * QOS info */
         rabTrans->rabs[idx].rabAllowed     = FALSE;
         rabTrans->rabs[idx].cause.causeTyp = CAUSE_RADIONW;
         rabTrans->rabs[idx].cause.causeVal = SztCauseRadioNwinvalid_qos_combinationEnum;
         idx++; 
         continue;
      }
      else
      {
         rabTrans->rabs[idx].ulGbr = 0;
         rabTrans->rabs[idx].dlGbr = 0;
         rabTrans->rabs[idx].ulMbr = 0;
         rabTrans->rabs[idx].dlMbr = 0;
      }

      if (rabInfo->e_RABLvlQoSParams.allocationRetentionPriority.pres.pres == PRSNT_NODEF)
      {
         SztAllocnAndRetentionPriority    *arpIe;

         arpIe = &rabInfo->e_RABLvlQoSParams.allocationRetentionPriority;

         rabTrans->rabs[idx].arpInfo.priorityLvl = 
               (arpIe->priorityLvl.pres == TRUE)?arpIe->priorityLvl.val:0;
         rabTrans->rabs[idx].arpInfo.preemptCapability =
               (arpIe->pre_emptionCapblty.pres == TRUE)?
               arpIe->pre_emptionCapblty.val:0;
         rabTrans->rabs[idx].arpInfo.preemptVulnerability =
              (arpIe->pre_emptionVulnerability.pres == TRUE)?
               arpIe->pre_emptionVulnerability.val:0;
      }
      else
      {
         rabTrans->rabs[idx].arpInfo.priorityLvl = 0;
         rabTrans->rabs[idx].arpInfo.preemptCapability = 0;
         rabTrans->rabs[idx].arpInfo.preemptVulnerability = 0;
      }
      ret = ROK;
      rabTrans->rabs[idx].nasPdu       = &rabInfo->nAS_PDU;
      rabTrans->rabs[idx].schdCfgState = WR_RAB_MODIFY_SCHD_CFG_IDLE;
      rabTrans->rabs[idx].rabAllowed   = TRUE;
      rabTrans->rabs[idx].qci    = rabInfo->e_RABLvlQoSParams.qCI.val;
      idx++;
   }
   rabTrans->numRabs = idx;

   RETVALUE(ret);
}
/** @brief This function parses all the IEs from the
 *         S1Ap message ERAB Modify from MME. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It parses all the information elements from 
 *           S1Ap message ERAB Modify received
 *           from MME.
 *         - It stores new parameters from the received message into
 *           the transaction  control block.
 *
 *
 * @param WrUmmTransCb *transCb 
 * @return ROK 
 */ 
PRIVATE S16 wrUmmParseRabModifyMsg
(
WrUmmTransCb                 *transCb
)
{
   S1apPdu                   *pdu = transCb->msg->u.s1Pdu->pdu;
   SztInitiatingMsg          *initMsg = &pdu->pdu.val.initiatingMsg;
   SztE_RABMdfyRqst          *rabmod = &initMsg->value.u.sztE_RABMdfyRqst;
   WrUmmRabModifyTransCb     *rabTrans = &transCb->u.rabModifyTransCb;
   U32                       idx;
   SztProtIE_Field_E_RABMdfyRqstIEs *ie;
   S32                       ret = ROK;
   
   rabTrans->dlAmbr             = 0;
   rabTrans->ulAmbr             = 0;
   rabTrans->numRabs            = 0;

   RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[S1AP] Received E-RAB MODIFY "
      "REQUEST [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
      transCb->ueCb->s1ConCb->mme_ue_s1ap_id,
      transCb->ueCb->s1ConCb->enb_ue_s1ap_id);


   for(idx = 0;idx < rabmod->protocolIEs.noComp.val; idx++)
   {
      ie = &rabmod->protocolIEs.member[idx];
      switch(ie->id.val)
      {
         case Sztid_uEaggregateMaxBitrate:
         {
            SztUEAggMaxBitrate *ambr = &ie->value.u.sztUEAggMaxBitrate;
            wrGetBitRate(&ambr->uEaggregateMaxBitRateDL, &rabTrans->dlAmbr);
            wrGetBitRate(&ambr->uEaggregateMaxBitRateUL, &rabTrans->ulAmbr);
            rabTrans->isUeAggrMaxBitRateRcvd = TRUE;
            break;
         }
         case Sztid_E_RABToBeMdfdLstBrModReq:
         {
            ret = wrUmmPrcRabModifyLst(rabTrans, 
                                   &ie->value.u.sztE_RABToBeMdfdLstBrModReq,
                                   transCb->ueCb);
            break;
         }
      }
   }
   RETVALUE(ret);
}
/** @brief This function finds the Logical channel ID
 *         from UE control block. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It determines the logical channel ID 
 *           from ERABID as index and updates into
 *           the transaction control block. 
 *
 *
 * @param WrUeCb                *ueCb  
 * @param WrUmmRabModifyRabInfo *rabmod
 * @return ROK
 */ 

PRIVATE S16 wrUmmRabModFindLch
(
WrUeCb                       *ueCb,
WrUmmRabModifyRabInfo        *rabmod
)
{
   U32             idx =((rabmod->erabId) - WR_UMM_ERAB_OFFSET);
   
   if ((idx >= WR_MAX_DRBS) || (idx < 0))
   {   
      RETVALUE(RFAILED);
   }
   
   if (ueCb->rbInfo.rab[idx].inUse != FALSE)
   {
      rabmod->lchId = ueCb->rbInfo.rab[idx].lchId;
   }
   else
   {
      RETVALUE(RFAILED);
   }
      
   RETVALUE(ROK);
}
/** @brief This function finds the DRB ID 
 *         from UE control block.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It determines the DRB ID 
 *           from ERABID as index and updates into
 *           the transaction control block.
 *
 *
 * @param WrUeCb             *ueCb 
 * @param WrUmmRabModifyRabInfo *rabmod
 * @return ROK 
 */ 
PRIVATE S16 wrUmmRabModFindDrb
(
WrUeCb                       *ueCb,
WrUmmRabModifyRabInfo        *rabmod
)
{

   U32             idx =((rabmod->erabId) - WR_UMM_ERAB_OFFSET);
   
   if ((idx >= WR_MAX_DRBS) || (idx < 0))
   {   
      RETVALUE(RFAILED);
   }
   
   if (ueCb->rbInfo.rab[idx].inUse != FALSE)
   {
      rabmod->drbId = ueCb->rbInfo.rab[idx].rbId;
   }
   else
   {
      RETVALUE(RFAILED);
   }
   
   RETVALUE(ROK);
}
/** @brief This function processes the ERAB modify 
 *         to determine LCHID and DRBID correponding
 *         to ERABId.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It determines the LCHID and DRBID based on
 *           ERABID as index to the existing RAB info
 *           control block. 
 *
 *
 * @param WrUmmTransCb *transCb 
 * @return ROK 
 */ 
PRIVATE S16 wrUmmPrcRabModify
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabModifyTransCb        *rabModTransCb = &transCb->u.rabModifyTransCb;
   U32                          idx;
   U32                          rabOff;
   WrUmmCellCb                  *ummCellCb = NULLP;
   U16                          cellId = transCb->ueCb->cellId;
   WrUmmRabModifyRabInfo        *rabModInfo;
   S16                           retVal = ROK;
   RmuUeRecfgReq                *ueRecfgReq;
   RmuUErabAddModIe             *eRabModPtr;

   WR_ALLOC(&ueRecfgReq, sizeof(RmuCommonMsg));
   if (ueRecfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   eRabModPtr = ueRecfgReq->stErabConfInfo.stErabAddModLst;
   ueRecfgReq->stErabConfInfo.usNoErabsToAddMod = 0;
   ueRecfgReq->stErabConfInfo.bNoErabsToRel = 0;
   ueRecfgReq->stErabConfInfo.enErabAction = RMU_MOD_ERAB;

   /*Fix For ccpu00123304*/  
   rabModTransCb->numRabsFound = 0;
   WR_UMM_GET_CELLCB(ummCellCb, cellId);
   if (ummCellCb == NULLP)
   {
      RLOG0(L_ERROR, "Getting cell CB failed");
      WR_FREE(ueRecfgReq, sizeof(RmuCommonMsg));
      RETVALUE(RFAILED);
   }
   /* Need to determine LCH Id and RB Id for each RAB to be modify */
   for (idx = 0; idx < rabModTransCb->numRabs; idx++)
   {
      if (rabModTransCb->rabs[idx].rabAllowed == TRUE) 
      {
         if (ummCellCb->wrRabCfgParams[rabModTransCb->rabs[idx].qci].qciSupported == FALSE)
         {
            /* Error_Cause: Filling the proper cause value in E-Rab Modify failure
             * */
            rabModTransCb->rabs[idx].cause.causeTyp = CAUSE_RADIONW;
            rabModTransCb->rabs[idx].cause.causeVal  = SztCauseRadioNwnot_supported_QCI_valueEnum;
            rabModTransCb->rabs[idx].rabAllowed = FALSE;
            continue;
         } 
         rabModInfo = &(rabModTransCb->rabs[idx]);

         rabOff = (rabModInfo->erabId) - WR_UMM_ERAB_OFFSET;
         
         
          if (rabOff < 0)
         {
            RLOG0(L_ERROR, "Invalid rabOff");
            WR_FREE(ueRecfgReq, sizeof(RmuCommonMsg));
            RETVALUE(RFAILED);
         }   

         if((rabOff >= WR_MAX_DRBS) ||
              (transCb->ueCb->rbInfo.rab[rabOff].inUse == FALSE))
         {
            /*Error_Cause: Filling proper cause values when no e_RAB_ID is present
             * for Rab-Modification
             */
            rabModTransCb->rabs[idx].cause.causeTyp = CAUSE_RADIONW;
            rabModTransCb->rabs[idx].cause.causeVal  = SztCauseRadioNwunknown_E_RAB_IDEnum;
            rabModTransCb->rabs[idx].rabAllowed = FALSE;
            continue;
         }   
         /* As per the 23401-960 spec under section 5.4.2.1, 
          * modifying the bearers from GBR to Non GBR
          * and vice versa is not supported 
          * (Non GBR  && GBR ) || (GBR && Non GBR)
          * 1 - 4 : GBR bearer
          * 5 - 9 : Non GBR bearer
         */
         /* Fix for ccpu00126072 */
         if (((transCb->ueCb->rbInfo.rab[rabOff].qci > 4) && 
                                (rabModInfo->qci <= 4)) ||
            ((transCb->ueCb->rbInfo.rab[rabOff].qci <= 4) && 
                                (rabModInfo->qci > 4)))
         {
            rabModTransCb->rabs[idx].rabAllowed     = FALSE;
            rabModTransCb->rabs[idx].cause.causeTyp = CAUSE_RADIONW;
            rabModTransCb->rabs[idx].cause.causeVal = 
                         SztCauseRadioNwinvalid_qos_combinationEnum;
            continue;
         }    

         if( wrUmmRabModFindLch(transCb->ueCb,&rabModTransCb->rabs[idx]) != ROK)
         {
            /*ccpu00127802*/
            RLOG0(L_ERROR, "Failed to find LCH for a RAB");
            rabModTransCb->rabs[idx].rabAllowed     = FALSE;
            rabModTransCb->rabs[idx].cause.causeTyp = CAUSE_RADIONW;
            rabModTransCb->rabs[idx].cause.causeVal = 
                          SztCauseRadioNwradio_resources_not_availableEnum;
            continue;
         }
         if (wrUmmRabModFindDrb(transCb->ueCb,&rabModTransCb->rabs[idx]) != ROK)
         {
            /*ccpu00127802*/
            RLOG0(L_ERROR, "Failed to find DRB for a RAB");
            rabModTransCb->rabs[idx].rabAllowed     = FALSE;
            rabModTransCb->rabs[idx].cause.causeTyp = CAUSE_RADIONW;
            rabModTransCb->rabs[idx].cause.causeVal = 
                           SztCauseRadioNwradio_resources_not_availableEnum;
            continue;
         }

         /* RRC-REL10-Upgrade */
         /* If the QCI value is changed for the given RAB, set the flag in the
          * transaction to TRUE so that discard timer value will be filled 
          * during RRC Recofiguration
          */
         if(transCb->ueCb->rbInfo.rab[rabOff].qci != rabModInfo->qci)
         {
            rabModInfo->isQciChanged = TRUE;
         }

         rabModTransCb->numRabsFound++;

         eRabModPtr->bErabId                        = rabModInfo->erabId;

         eRabModPtr->stErabQosInfo.bErabQCI           = rabModInfo->qci;

         eRabModPtr->stErabQosInfo.stErabGbrInfo.uiErabDlMbr = rabModInfo->dlMbr;
         eRabModPtr->stErabQosInfo.stErabGbrInfo.uiErabUlMbr = rabModInfo->ulMbr;
         eRabModPtr->stErabQosInfo.stErabGbrInfo.uiErabDlGbr = rabModInfo->dlGbr;
         eRabModPtr->stErabQosInfo.stErabGbrInfo.uiErabUlGbr = rabModInfo->ulGbr;

         eRabModPtr->stErabQosInfo.stErabArpInfo.bErabPrioLvl =
            (U8)rabModInfo->arpInfo.priorityLvl;
         eRabModPtr->stErabQosInfo.stErabArpInfo.bErabPreemptCap =
            (Bool)rabModInfo->arpInfo.preemptCapability;
         eRabModPtr->stErabQosInfo.stErabArpInfo.bErabPreemptVul  =
            (Bool)rabModInfo->arpInfo.preemptVulnerability;

    eRabModPtr++;

         ueRecfgReq->stErabConfInfo.usNoErabsToAddMod++;
      }
   }

   if(ueRecfgReq->stErabConfInfo.usNoErabsToAddMod)
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

/** @brief This function handles the configuration
 *         confirm from MAC Scheduler. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It validates the configuration confirm
 *           from MAC Sceduler whether it is success
 *           or failure.     
 *
 *
 * @param WrUmmTransCb *transCb 
 * @return ROK 
 */ 
PRIVATE S16 wrUmmRvrtPrcSchdCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabModifyTransCb     *rabTrans = &transCb->u.rabModifyTransCb;
   U8                        idx;
    
   idx = transCb->transId & 0x0f;
   if (idx > 15)
   {
      /*ccpu00127802*/
      RLOG_ARG0(L_INFO, DBG_TRNSID, transCb->transId,"Invalid transaction id "
         "for RAB Modify");
      RETVALUE(ROK);
   }

   if (idx == WR_RAB_MODIFY_UE_TRANS_ID)
   {
      /* This is in response to UE reconfiguration for AMBR              */
      RETVALUE(ROK);
   }
   if (idx > rabTrans->numRabs)
   {
      /*ccpu00127802*/
      RLOG_ARG0(L_INFO, DBG_TRNSID, transCb->transId,"Invalid transaction id "
         "for RAB Modify");
      RETVALUE(ROK);
   }
   if (transCb->msg->u.schdCfm.status != RGR_CFG_CFM_OK)
   {
      /*Error_Cause: Filling proper cause value when Scheduler sends failure
       * response
       */
      rabTrans->rabs[idx].rabAllowed     = FALSE;
      rabTrans->rabs[idx].cause.causeTyp = CAUSE_RADIONW;
      rabTrans->rabs[idx].cause.causeVal = SztCauseRadioNwunspecifiedEnum;
      rabTrans->rabs[idx].schdCfgState =  WR_RAB_MODIFY_SCHD_CFM_FAIL;
   }
   else
   {
      rabTrans->rabs[idx].schdCfgState =  WR_RAB_MODIFY_SCHD_CFM_SUCC;
   }
   rabTrans->schdUndoCfgState = FALSE;
   for (idx = 0; idx < rabTrans->numRabs; idx++)
   {
      if (rabTrans->rabs[idx].schdCfgState ==  WR_RAB_MODIFY_SCHD_CFG_SENT)
      {
         RETVALUE(ROK);
      }
   }
   rabTrans->schdUndoCfgState = TRUE;
   RETVALUE(ROK);
}

/** @brief This function handles the configuration
 *         confirm from MAC Scheduler. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It validates the configuration confirm
 *           from MAC Sceduler whether it is success
 *           or failure.     
 *
 *
 * @param WrUmmTransCb *transCb 
 * @return ROK 
 */ 
PRIVATE S16 wrUmmRabModifyPrcSchdCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabModifyTransCb     *rabTrans = &transCb->u.rabModifyTransCb;
   WrUmmSchdCfgCfm           *schdCfm = &(transCb->msg->u.schdCfm);
   U8                        lcgId;
   U8                        idx;

   idx = schdCfm->transId & 0x0f;

   if (idx == WR_RAB_MODIFY_UE_TRANS_ID)
   {
      /*ccpu00127802*/
      RLOG0(L_DEBUG, "RAB-REL: UE-recfg schd-cfm rcvd");
      /* This is in response to UE reconfiguration for AMBR              */
      rabTrans->cfgStateMsk |= WR_UMM_RAB_MODIFY_UE_CFG_RECVD;
   }
   else if (idx == WR_UMM_TRANS_ID_LCG0_CFG)
   {
      rabTrans->lcgCfgStatus[0] = WR_UMM_RAB_MODIFY_LCG_CFG_RECVD;
      rabTrans->expectedLcgCfgStatus--;
      for (lcgId=0; lcgId < WR_UMM_MAX_LCG_GRP_ID; lcgId++)
      {
         if (rabTrans->lcgCfgStatus[lcgId] == WR_UMM_RAB_MODIFY_LCG_CFG_SENT)
         {
            RETVALUE(ROK);
         }
      }
   }
   else if (idx == WR_UMM_TRANS_ID_LCG1_CFG)
   {
      rabTrans->lcgCfgStatus[1] = WR_UMM_RAB_MODIFY_LCG_CFG_RECVD;
      rabTrans->expectedLcgCfgStatus--;
      for (lcgId=0; lcgId < WR_UMM_MAX_LCG_GRP_ID; lcgId++)
      {
         if (rabTrans->lcgCfgStatus[lcgId] == WR_UMM_RAB_MODIFY_LCG_CFG_SENT)
         {
            RETVALUE(ROK);
         }
      }
   }
   else if (idx == WR_UMM_TRANS_ID_LCG2_CFG)
   {
      rabTrans->lcgCfgStatus[2] = WR_UMM_RAB_MODIFY_LCG_CFG_RECVD;
      rabTrans->expectedLcgCfgStatus--;
      for (lcgId=0; lcgId < WR_UMM_MAX_LCG_GRP_ID; lcgId++)
      {
         if (rabTrans->lcgCfgStatus[lcgId] == WR_UMM_RAB_MODIFY_LCG_CFG_SENT)
         {
            RETVALUE(ROK);
         }
      }
   }
   else if (idx == WR_UMM_TRANS_ID_LCG3_CFG)
   {
      rabTrans->lcgCfgStatus[3] = WR_UMM_RAB_MODIFY_LCG_CFG_RECVD;
      rabTrans->expectedLcgCfgStatus--;
      for (lcgId=0; lcgId < WR_UMM_MAX_LCG_GRP_ID; lcgId++)
      {
         if (rabTrans->lcgCfgStatus[lcgId] == WR_UMM_RAB_MODIFY_LCG_CFG_SENT)
         {
            RETVALUE(ROK);
         }
      }
   }
   else if (idx < rabTrans->numRabs)
   {
      /* For SCHD CFG reverted bearers, no subsequent handling*/
      if(WR_RAB_MODIFY_SCHD_CFM_FAIL_RVRT == rabTrans->rabs[idx].schdCfgState)
      {
         if(0 != rabTrans->rabs[idx].schdRvrtCfgCfmCnt)
         {
            rabTrans->rabs[idx].schdRvrtCfgCfmCnt--;
         }
         if(0 == rabTrans->rabs[idx].schdRvrtCfgCfmCnt)
         {
            rabTrans->state = WR_UMM_RAB_MODIFY_SCHD_CFG_RVRT_DONE;   
            if(FALSE == rabTrans->allRabsFailed)
            {
               rabTrans->state = WR_UMM_RAB_MODIFY_SCHD_CFG_DONE;
            }
         }
         RETVALUE(ROK);
      }
      if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
      {
         rabTrans->rabs[idx].schdCfgState = WR_RAB_MODIFY_SCHD_CFM_SUCC;
      }
      else
      {
         /*127050 :Filling the proper cause values*/  
         rabTrans->rabs[idx].cause.causeTyp  = CAUSE_RADIONW; /*129993*/
         rabTrans->rabs[idx].cause.causeVal  = SztCauseRadioNwunspecifiedEnum;
         rabTrans->rabs[idx].rabAllowed      = FALSE;
         rabTrans->rabs[idx].schdCfgState = WR_RAB_MODIFY_SCHD_CFM_FAIL;

         /*ccpu00127802*/
         RLOG0(L_WARNING, "LCH ReCfg is falied for RAB Modify");
         RETVALUE(ROK);
      }
      for (idx = 0; idx < rabTrans->numRabs; idx++)
      {
         if ((rabTrans->rabs[idx].schdCfgState ==  WR_RAB_MODIFY_SCHD_CFG_SENT)
               && (rabTrans->rabs[idx].rabAllowed))
         {
            RETVALUE(ROK);
         }
      }
      rabTrans->cfgStateMsk |= WR_UMM_RAB_MODIFY_LCH_CFG_RECVD;
   }
   else
   {
      RLOG_ARG0(L_WARNING, DBG_TRNSID, transCb->transId,"Invalid transaction id "
            "for RAB Modify");
      RETVALUE(ROK);
   }

   /* once we get all the confirmation for all LCGs setting the bitmask */
   if (0 == rabTrans->expectedLcgCfgStatus)
   {
      rabTrans->cfgStateMsk |= WR_UMM_RAB_MODIFY_LCG_CFG_RECVD;
   }

   if ((rabTrans->cfgStateMsk & WR_UMM_RAB_MODIFY_CFG_RECVD) == WR_UMM_RAB_MODIFY_CFG_RECVD)
   {
      rabTrans->state = WR_UMM_RAB_MODIFY_SCHD_CFG_DONE;
   }
   RETVALUE(ROK);
}
/** @brief This function fills the DRB reconfiguration
 *         parameters.        
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It prepares RRC reconfiguration message
 *           for each DRB to be reconfigured. 
 *
 *
 * @param NhuRbCfgInfo           *drbCfg 
 * @param WrUmmRabModifyRabInfo  *rabInfo 
 * @return ROK/RFAILED 
 */ 
PRIVATE S16 wrUmmRabModifyRrcReconfigModDrbCfg
(
 U16                         cellId,
NhuRbCfgInfo                 *drbCfg,
WrUmmRabModifyRabInfo        *rabInfo
)
{
   if (wrUmmRrcModDrbCfg(cellId, drbCfg, rabInfo->drbId, rabInfo->lchId, 
            rabInfo->qci, rabInfo->isUlSpsEnabled) != ROK)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
/** @brief This function performs reconfiguration of RRC
 *         layer and RRC Recfg message to the UE. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - 
 *
 *
 * @param WrUmmTransCb  *transCb  
 * @param NhuDatReqSdus *evnt
 * @return ROK 
 */ 
PRIVATE S16 wrUmmRabModifyFillRrcCfg
(
WrUmmTransCb                 *transCb,
NhuDatReqSdus                *evnt
)
{
   WrUmmRabModifyTransCb     *rabTrans = &transCb->u.rabModifyTransCb;
   NhuRbCfgInfo              *drbCfg;
   WrUmmRabModifyRabInfo     *rabInfo;
   NhuUeCfgInfo              *ueCfg;
   NhuRbCfgList              *rbCfgList;
   U32                       idx;
   U32                       numDrbs = 0;
   U8                        rbIdx   = 0;
   U16                       cellId = transCb->ueCb->cellId;

   evnt->sdu.isUeCfgPres = TRUE;
   ueCfg = &(evnt->sdu.ueCfg);
   ueCfg->ueCfgBitMsk = NHU_EXP_CFG_RB_CFG;
   ueCfg->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_MODIFY;
   rbCfgList = &(ueCfg->rbCfgAllList.rbToModCfgList);
   WR_GET_MEM(evnt, (sizeof(NhuRbCfgInfo) * rabTrans->numRabs), 
                                              &(rbCfgList->rbCfgInfoList));
   for (idx = 0; idx < rabTrans->numRabs; idx++)
   {
      if(rabTrans->rabs[idx].rabAllowed)
      {
         rabInfo = &rabTrans->rabs[idx];
         drbCfg  = rbCfgList->rbCfgInfoList + rbIdx;
         rbIdx++;

         /* We send information of only those RABs that are successful       */
         if ((rabInfo->rabAllowed == TRUE) &&
            (rabInfo->schdCfgState == WR_RAB_MODIFY_SCHD_CFM_SUCC))
         {
            wrUmmRabModifyRrcReconfigModDrbCfg(cellId, drbCfg, rabInfo);
            numDrbs++;
         }
      }   
   }
   rbCfgList->numOfRbCfgInfo = numDrbs;
   RETVALUE(ROK);
}
/** @brief This function fills DRB information for 
 *         reconfiguration to RRC layer. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It prepares RRC reconfigution message
 *           with all the information required to
 *           reconfigure DRB based on new QCI. 
 *
 *
 * @param NhuDRB_ToAddMod          *drbIe 
 * @param WrUmmRabModifyRabInfo    *rabInfo
 * @return ROK/RFAILED 
 */ 
PRIVATE S16 wrUmmRabModifyRrcReconfigModDrb
(
 /* SPS changes starts */
 WrUmmTransCb                *transCb,
 /* SPS changes ends */
NhuDRB_ToAddMod              *drbIe,
WrUmmRabModifyRabInfo        *rabInfo
)
{
   U8                           erabId = rabInfo->erabId;
   U8                           drbId = rabInfo->drbId;
   U8                           lchId = rabInfo->lchId;
   U8                           qci = rabInfo->qci;
   WrRabCfgParams              *params;
   WrPdcpCfgParams             *pdcp; 
   WrRlcCfgParams              *rlc; 
   WrLchCfgParams              *lch;
   NhuLogChannelConfigul_SpecificParams *chCfgParams = NULLP;
   WrUmmCellCb                          *ummCellCb = NULLP;
 /* SPS changes starts */
   WrUeCb                      *ueCb = transCb->ueCb;

   WR_UMM_GET_CELLCB(ummCellCb, ueCb->cellId);
 /* SPS changes ends */

   if(ummCellCb == NULLP)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }

   params = &ummCellCb->wrRabCfgParams[qci];
   pdcp   = &(params->pdcpParams);
   rlc    = &(params->rlcParams);
   lch    = &(params->lchParams);
   wrFillTknU8(&drbIe->pres, PRSNT_NODEF);
   wrFillTknU32(&drbIe->eps_BrIdentity, erabId);
   wrFillTknU32(&drbIe->drb_Identity, drbId);


   /* Fill RLC related params */
   if (params->rlcMode == WR_RLC_AM_MODE)
   {
      NhuRLC_Configam        *rlcAm = &drbIe->rlc_Config.val.am;
      wrFillTknU8(&drbIe->rlc_Config.choice, RLC_CONFIG_AM);
      wrFillTknU8(&rlcAm->pres, PRSNT_NODEF);
      wrFillTknU8(&rlcAm->ul_AM_RLC.pres, PRSNT_NODEF);
      wrFillTknU32(&rlcAm->ul_AM_RLC.t_PollRetransmit, 
                                                   rlc->u.am.ul.pollRetxTmr);
      wrFillTknU32(&rlcAm->ul_AM_RLC.pollPDU, rlc->u.am.ul.pollPdu);
      wrFillTknU32(&rlcAm->ul_AM_RLC.pollByte, rlc->u.am.ul.pollByte);
      wrFillTknU32(&rlcAm->ul_AM_RLC.maxRetxThreshold, 
                                                  rlc->u.am.ul.maxRetxThrsh);
      wrFillTknU8(&rlcAm->dl_AM_RLC.pres, PRSNT_NODEF);
      if (rabInfo->isDlSpsEnabled == TRUE)
      {
         wrFillTknU32(&rlcAm->dl_AM_RLC.t_Reordering, NhuT_Reorderingms20Enum);
      }
      else
      {
         wrFillTknU32(&rlcAm->dl_AM_RLC.t_Reordering, rlc->u.am.dl.reorderingTmr);
      }
      wrFillTknU32(&rlcAm->dl_AM_RLC.t_StatusProhibit, rlc->u.am.dl.statusProhTmr);
      RLOG2(L_DEBUG, "RLC AM mode is being configured statusProh[%ld] "
         "reordering[%ld]", rlcAm->dl_AM_RLC.t_StatusProhibit.val,
         rlcAm->dl_AM_RLC.t_Reordering.val);
   }
   else
   {
      NhuRLC_Configum_Bi_Directional *rlcUmBiDir = &drbIe->rlc_Config.
         val.um_Bi_Directional;

      wrFillTknU8(&drbIe->rlc_Config.choice, RLC_CONFIG_UM_BI_DIRECTIONAL);
      wrFillTknU8(&rlcUmBiDir->pres, PRSNT_NODEF);
      wrFillTknU8(&rlcUmBiDir->ul_UM_RLC.pres, PRSNT_NODEF);
      wrFillTknU32(&rlcUmBiDir->ul_UM_RLC.sn_FieldLength, rlc->u.um.ul.snFldLen);
      wrFillTknU8(&rlcUmBiDir->dl_UM_RLC.pres, PRSNT_NODEF);
      wrFillTknU32(&rlcUmBiDir->dl_UM_RLC.sn_FieldLength, rlc->u.um.dl.snFldLen );
      if (rabInfo->isDlSpsEnabled == TRUE)
      {
         wrFillTknU32(&rlcUmBiDir->dl_UM_RLC.t_Reordering, NhuT_Reorderingms20Enum);
      }
      else
      {
         wrFillTknU32(&rlcUmBiDir->dl_UM_RLC.t_Reordering, rlc->u.um.dl.tReordering);
      }
      RLOG0(L_DEBUG, "RLC UM mode is being configured");
   }

   /* TODO: THe following portion of the code needs correction            */
   wrFillTknU32(&drbIe->logicalChannelIdentity, lchId);
   wrFillTknU8(&drbIe->logicalChannelConfig.pres, PRSNT_NODEF);
   chCfgParams = &(drbIe->logicalChannelConfig.ul_SpecificParams);
   wrFillTknU8(&chCfgParams->pres, PRSNT_NODEF);
   wrFillTknU32(&chCfgParams->priority, lch->priority);
   wrFillTknU32(&chCfgParams->bucketSizeDuration, lch->bucketSizeDur);

   /* SPS changes starts */
   RLOG4(L_DEBUG, "LCID [%u], UE SR MASK[%u], transCb->spsCfgTransCb.isUeUlSpsEnabled [%u], UE Rel [%u]\n", \
         lchId, ueCb->isSrMaskEnabled, transCb->spsCfgTransCb.isUeUlSpsEnabled, \
         ueCb->ueCap->eUTRA_Cap.accessStratumRls.val);
   if ((ueCb->isSrMaskEnabled == TRUE) && (transCb->spsCfgTransCb.isUeUlSpsEnabled == TRUE) && \
         (ueCb->ueCap->eUTRA_Cap.accessStratumRls.val >= NhuAccessStratumRlsrel9Enum))
   {
      RLOG1(L_DEBUG, "Setting Logical Channel SR mask Logical Channel Id [%u]\n", lchId);
      wrFillTknU8(&drbIe->logicalChannelConfig.extaddgrp_1.pres, PRSNT_NODEF);
      wrFillTknU32(&drbIe->logicalChannelConfig.extaddgrp_1.logicalChannelSR_Mask_r9, \
            NhuLogChannelConfigextaddgrp_1logicalChannelSR_Mask_r9setupEnum);
   }
   if (rabInfo->isUlSpsEnabled == TRUE)
   {
      RLOG1(L_DEBUG, "Setting LCG [1] and Bitrate [Infinity] for the Logical Channel Id [%u]\n", lchId);
      wrFillTknU32(&chCfgParams->prioritisedBitRate, \
            NhuLogChannelConfigul_SpecificParamsprioritisedBitRateinfinityEnum);
      wrFillTknU32(&chCfgParams->logicalChannelGroup, 1);
   }
   else
   {
      RLOG2(L_DEBUG, "Setting LCG [%u] for the Logical Channel Id [%u]\n", lchId, lch->lchGrp);
      wrFillTknU32(&chCfgParams->prioritisedBitRate, lch->bitRate);
      wrFillTknU32(&chCfgParams->logicalChannelGroup, lch->lchGrp);
   }
   /* SPS changes ends */  

   /* RRC-REL10-Upgrade */
   /* Fill the discard timer value in PDCP config incase if the QCI value is 
    * changed from previous value. All other values are made NOTPRESENT. 
    */

   /* fix for ccpu00148395 
    * discardTimer would be included only if  QCI is modified and 
    * UE release is >= Rel10 
    */
   if((rabInfo->isQciChanged) 
         && (NhuAccessStratumRlsrel10Enum <= 
            ueCb->ueCap->eUTRA_Cap.accessStratumRls.val))
   {
      NhuPDCP_Config      *rrcPdcpCfg = &drbIe->pdcp_Config;
      wrFillTknU8(&(rrcPdcpCfg->pres), PRSNT_NODEF);
      wrFillTknU32(&(rrcPdcpCfg->discardTimer), pdcp->discardTimer);
      if (!pdcp->hdrCompress)
      {
         wrFillTknU8(&drbIe->pdcp_Config.headerCompression.choice,
               HEADERCOMPRESSION_NOTUSED);
      }
      else
      { 
         RLOG0(L_INFO, "Header compression is turned on");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}
/** @brief This function prepares RRC messages to be delivered
 *         to the UE and UE Recoifguration towards RRC. 
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

PRIVATE S16 wrUmmRabModifyPrepRrcMsgAndCfg
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabModifyTransCb           *rabTrans = &transCb->u.rabModifyTransCb;
   NhuDatReqSdus                   *recfgMsg;
   WrUeCb                          *ueCb = transCb->ueCb;
   WrUmmRabModifyRabInfo           *rabInfo;
   NhuDL_DCCH_Msg                  *dcchMsg;
   NhuRRCConRecfgn                 *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs          *recfgIEs;
   NhuRadioResourceConfigDedicated *rrcCfgDed;
   NhuDRB_ToAddMod                 *drbIe;
   U32                             numDrbs = 0;
   U8                              nasPduCount = 0;
   U32                             idx     = 0;
   U32                             nasIdx  = 0;
   NhuRRCConRecfgn_r8_IEsdedicatedInfoNASLst   *dedNasLst;
   /* SPS changes starts */
   U32                             rabCount = 0;
   U32                             drbModMask = 0;
   RmuUeRecfgRsp                   *rrmUeRecfgResp = rabTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
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
   if(ROK != wrUmmGetRrcTransId(ueCb, C1_RRCCONRECFGN, 
                      WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
                      &rabTrans->transId))
   {
      RLOG0(L_ERROR, "RRC Transaction Not avialble");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }

   /* Create the reconfiguration message which is constructued excluding  */
   /* the actual procedure related details which will be added further    */
   if (wrUmmRrcCreateRrcReconfigMsg(recfgMsg, rabTrans->transId) != ROK)
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
   rabCount = rabTrans->numRabs;
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
   
   /* Fill the Rab related details */
   for (idx = 0; idx < rabTrans->numRabs; idx++)
   {
      if(rabTrans->rabs[idx].rabAllowed)
      {   
         rabInfo = &rabTrans->rabs[idx];
    
         /* We send information of only those RABs that are successful       */
         if (rabInfo->schdCfgState == WR_RAB_MODIFY_SCHD_CFM_SUCC)
         {
            drbModMask |= 1<<(rabInfo->drbId - 1);
            drbIe   = rrcCfgDed->drb_ToAddModLst.member + numDrbs;
            /* SPS changes starts */
            wrUmmRabModifyRrcReconfigModDrb(transCb, drbIe, rabInfo);
            /* SPS changes ends */
            /* UL PBR changes */
            wrUmmRrcReconfigFillulPbr(drbIe, rabInfo->ulGbr, rabInfo->qci, rabInfo->isUlSpsEnabled); 
            numDrbs++;
            if(rabInfo->nasPdu != NULLP)
            {
               nasPduCount++;    
            }
         }
      }   
   }

   /* SPS changes starts */
   if (rabCount > rabTrans->numRabs)
   {  
      for (idx = 0; (idx < WR_MAX_DRBS) && (numDrbs < rabCount); idx++)
      {
         if((ueCb->rbInfo.rab[idx].inUse == TRUE) && (!(drbModMask & (1<<(ueCb->rbInfo.rab[idx].rbId-1)))))
         {
            RLOG4(L_DEBUG, "wrUmmRrcFillUeRadioResDedInfo: rbInfo info present:: idx [%u] erabId [%u] rbId [%u] lcId [%u]\n",
                  idx, ueCb->rbInfo.rab[idx].eRABId, ueCb->rbInfo.rab[idx].rbId, ueCb->rbInfo.rab[idx].lchId);
            drbIe   = rrcCfgDed->drb_ToAddModLst.member + numDrbs;

            wrUmmRrcReconfigFillModDrb(ueCb, rrmUeRecfgResp->stUeRadioResCfg.stSpsCfg.stSpsConfigUl.bIsSpsEnabled, \
                  drbIe, (U8)ueCb->rbInfo.rab[idx].eRABId,
                  ueCb->rbInfo.rab[idx].rbId ,ueCb->rbInfo.rab[idx].lchId ,
                  (U8)ueCb->rbInfo.rab[idx].qci);
            /* UL PBR changes gbr.ul is in kbps so gbr.ul/8 is done here*/
            wrUmmRrcReconfigFillulPbr(drbIe, ueCb->rbInfo.rab[idx].gbr.ul/8,
                  ueCb->rbInfo.rab[idx].qci, rrmUeRecfgResp->stUeRadioResCfg.stSpsCfg.stSpsConfigUl.bIsSpsEnabled);
            numDrbs++;
         }
      }
   }
   /* SPS changes ends */

   wrFillTknU16(&rrcCfgDed->drb_ToAddModLst.noComp,numDrbs);

   /* fill the Rab related NAS PDUs if any*/
   dedNasLst = &(recfgIEs->dedicatedInfoNASLst);
   if(nasPduCount)
   {
     /* Allocate memory to include NAS PDUs into the message towards the UE */
     WR_GET_MEM(recfgMsg, (sizeof(NhuDedicatedInfoNAS) * nasPduCount),
                          &(dedNasLst->member));
     if (dedNasLst->member == NULLP)
     {
        RETVALUE(RFAILED);
     }
     for (idx = 0; idx < rabTrans->numRabs; idx++)
     {
       if(rabTrans->rabs[idx].rabAllowed)
       {   
         rabInfo = &rabTrans->rabs[idx];

         /* We send information of only those RABs that are successful       */
         if ( (rabInfo->schdCfgState == WR_RAB_MODIFY_SCHD_CFM_SUCC) && 
        (rabInfo->nasPdu != NULLP))
         {
            wrFillTknStrOSXL(&(dedNasLst->member[nasIdx]), rabInfo->nasPdu->len, 
                  rabInfo->nasPdu->val, &(recfgMsg->memCp));
            nasIdx++;
         }
       }   
     }
     wrFillTknU16(&dedNasLst->noComp, nasIdx);
   }
   else
   {
     dedNasLst->noComp.pres = NOTPRSNT;
   }
   /* Build the configuration towards RRC   */
   wrUmmRabModifyFillRrcCfg(transCb, recfgMsg);

      /* SPS changes starts */
   if(rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
   {
      wrUmmRrcFillUeRadioResDedInfo(&recfgMsg->memCp, ueCb, &transCb->spsCfgTransCb.spsCfg, \
            rrcCfgDed, rrmUeRecfgResp);
      /* SPS changes ends */
   }
   
   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending "
         "RRCConnectionReconfiguration for ERAB Modification");
   /*Send message towards UE*/
   if (ROK != wrIfmRrcSndReqToUe(recfgMsg))
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe : failed");
      RETVALUE(RFAILED);
   }   

   /* Start RRC Reconfiguration timer */
   wrStartTransTmr(transCb,WR_TMR_RECONFIGURE, wrEmmGetRrcReCfgTmrVal(ueCb->cellId));
   RETVALUE(ROK);
}

/** @brief This function is responsible for filling and sending the RAB Modify 
 *         Response message to S1AP.
 *
 * @details
 *
 *     Function: wrUmmRabModSndS1apRsp
 *
 *         Processing steps:
 *         - invoke wrUmmS1apBldERABModRsp to fill the response message
 *         - send the message to S1AP
 *         - set the transaction as complete
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmRabModSndS1apRsp
(
WrUmmTransCb              *transCb
)
{
   WrUmmRabModifyTransCb     *rabTrans = &transCb->u.rabModifyTransCb;
   SztDatEvntReq             erabModRsp;
   U8                        numRabs;
   WrUmmRabRlsLst            *rabLst;
   U8                        idx;
   S16                       ret;



   numRabs = rabTrans->numRabs;
   WR_ALLOC(&rabLst, numRabs * sizeof(WrUmmRabRlsLst));
   if (rabLst == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   for (idx = 0;idx < numRabs;idx++)
   {
      if(FALSE == rabTrans->rabs[idx].rabAllowed)
      {
         rabLst[idx].rabFailed = TRUE;
         /*Error_Cause:Filling proper cause value*/
         rabLst[idx].cause.causeTyp = rabTrans->rabs[idx].cause.causeTyp; 
         rabLst[idx].cause.causeVal = rabTrans->rabs[idx].cause.causeVal; 
      }
      else
      {
         rabLst[idx].rabFailed = FALSE;
      }

      rabLst[idx].erabId = rabTrans->rabs[idx].erabId; 
   }
   WR_SET_ZERO(&erabModRsp, sizeof(SztDatEvntReq));
   /* Send RAB Modify Response over S1 interface to MME        */
   ret = wrUmmS1apBldERABModRsp(transCb->ueCb->s1ConCb,
         &erabModRsp.pdu,idx,rabLst);
   if (ret != ROK)
   {
      /*ccpu00127802*/
      RLOG0(L_ERROR, "ERAB Modify RSP MSG building  failed");
   }
   
   /*Freeing the local structure*/
   WR_FREE(rabLst, numRabs * sizeof(WrUmmRabRlsLst));
   erabModRsp.spConnId = transCb->ueCb->s1ConCb->spConnId;
   ret = WrIfmS1apDatReq(&erabModRsp);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Sending ERAB MODIFY RESPONSE Failed");
   }
   else
   {
      RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[S1AP] Sending E-RAB MODIFY "
            "RESPONSE [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
            transCb->ueCb->s1ConCb->mme_ue_s1ap_id,
            transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
   }
   RETVALUE(ROK);
}

/* ccpu00130441 and ccpu00130443 */
/** @brief This function is responsible for handling handover by filling 
 *  ERAB modify failure for each RABs.
 *
 * @details
 *
 *     Function: wrUmmHOHandleRabModify
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
PRIVATE S16 wrUmmHOHandleRabModify
(
WrUmmTransCb              *transCb,
WrHoType                  hoType
)
{
   WrUmmRabModifyTransCb     *rabModifyTransCb = &transCb->u.rabModifyTransCb;
   U32                       idx           = 0;
   
   for (idx = 0; idx < rabModifyTransCb->numRabs; idx++)
   {
      rabModifyTransCb->rabs[idx].rabAllowed = FALSE;
      rabModifyTransCb->rabs[idx].cause.causeTyp  = CAUSE_RADIONW;

      if( WR_HO_TYPE_INTRA_LTE_S1 == hoType)  
      {
         rabModifyTransCb->rabs[idx].cause.causeVal  = 
            SztCauseRadioNws1_intra_system_handover_triggeredEnum;
      }
      else if( WR_HO_TYPE_IRAT_UTRA == hoType)
      {
         rabModifyTransCb->rabs[idx].cause.causeVal  = 
            SztCauseRadioNws1_inter_system_handover_triggeredEnum;
      }
      else if( WR_HO_TYPE_X2 == hoType)
      {
         rabModifyTransCb->rabs[idx].cause.causeVal  = 
            SztCauseRadioNwx2_handover_triggeredEnum;
      }
   }
   
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmRabModifyProcessorForS1DatInd
(
    WrUmmTransCb                 *transCb
)
{
   if(transCb ==  NULLP)
   {
     RETVALUE(RFAILED);
   }  

   /* Parse the received RAB MODIFY message and udpate transCb       */
   if (wrUmmParseRabModifyMsg(transCb) != ROK)
   {
      wrUmmRabModSndS1apRsp(transCb);
      wrUmmTransComplete(transCb);
      RETVALUE(ROK);
   }
   /*ccpu00130441 and ccpu00130443: Update the failure cause value for each RABS, if Handover is in progress */
   if((transCb->ueCb->mobCtrlState == WR_HO_INITIATED) || (transCb->ueCb->mobCtrlState == WR_HO_IN_PROGRESS))
   {
            wrUmmHOHandleRabModify(transCb,transCb->ueCb->hoType);
   }
   
   /*Update the Statistics for ERAB-Modify Attempted*/
   wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_MODIFY,
                         WR_EMM_DFLT_POS_STEP,WR_EMM_ERAB_ATTEMPT);

   /* Increment the reference and store the message in the transCb  */
   transCb->msg->refCnt++;
   transCb->u.rabModifyTransCb.msg = transCb->msg;

   transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg = NULLP;

   wrUmmPrcRabModify(transCb);
 
   if (transCb->u.rabModifyTransCb.numRabsFound == 0)
   {
      /* We just need to send a response and complete the trans     */
      wrUmmRabModSndS1apRsp(transCb);
      wrUmmTransComplete(transCb);
      RETVALUE(ROK);
   }

   RETVALUE(ROK);
}

/* SPS changes starts */
/** @brief This function is responsible for filling the UL SPS LC for SCHD.
 *
 * @details
 *
 *     Function: wrUmmRabModifySchdUlSpsLchCfgReq
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

PUBLIC Void wrUmmRabModifySchdUlSpsLchCfgReq
(
 RmuUeRecfgRsp           *rrmUeRecfgResp,
 U32                     numErabs,
 Void                    *pErabList,
 RgrUeSpsCfg             *ueSpsCfg,
 WrUeCb                  *ueCb
)
{
   U32                    count      = 0;
   U8                     rabIdx     = 0;
   WrUeRAB                *rabInfo   = NULLP;
   WrUmmRabModifyRabInfo  *pErabInfo = (WrUmmRabModifyRabInfo *)pErabList;

   ueSpsCfg->ulSpsCfg.lcCnt = 0;
   for (count = 0; count < numErabs; count++)
   {
      if ((pErabInfo+count)->rabAllowed == TRUE)
      {
         rabIdx = (U8)((pErabInfo+count)->erabId - (U8)WR_UMM_ERAB_OFFSET); 
         rabInfo = &(ueCb->rbInfo.rab[rabIdx]);
         if (rabInfo->isUlSpsEnabled != rrmUeRecfgResp->stErabAcceptLst[count].bIsUlSpsEnabled)
         {
            if(rrmUeRecfgResp->stErabAcceptLst[count].bIsUlSpsEnabled == TRUE)
            {
               /* Here Application should take stuffs from the RRM accepted list but currently 
                * there is no RBC support at both APP and RRM. Thus the expectation is, RRM has accepted 
                * all the ERABs in the request and thus have provided ERAB in the same order as sent to it. */
               ueSpsCfg->ulSpsCfg.spsLcInfo[ueSpsCfg->ulSpsCfg.lcCnt].isSpsEnabled = TRUE;
               (pErabInfo+count)->isUlSpsEnabled = TRUE;
            }
            else
            {
               ueSpsCfg->ulSpsCfg.spsLcInfo[ueSpsCfg->ulSpsCfg.lcCnt].isSpsEnabled = FALSE;
               (pErabInfo+count)->isUlSpsEnabled = FALSE;
            }
            RLOG3(L_DEBUG, "wrUmmRabModifySchdUlSpsLchCfgReq: Modifying UL LC, count=[%u] lcId=[%u] isUlSpsEnabled [%u]", \
                     count, (pErabInfo+count)->lchId, (pErabInfo+count)->isUlSpsEnabled);
            ueSpsCfg->ulSpsCfg.spsLcInfo[ueSpsCfg->ulSpsCfg.lcCnt].lcId = (pErabInfo+count)->lchId;
            ueSpsCfg->ulSpsCfg.lcCnt++;
         }
      }
      else
      {
         RLOG3(L_DEBUG, \
             "wrUmmRabModifySchdUlSpsLchCfgReq: Not Allowed RAB recived from RRM. count=[%u] lcId=[%u] isUlSpsEnabled [%u]", \
             count, (pErabInfo+count)->lchId, ((pErabInfo+count)->isUlSpsEnabled));
         /* Initialise the value to FALSE to avoid any mismatch in SPS configuration */
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
 *     Function: wrUmmUpdateRabModTrans 
 *
 *         Processing steps:
 *              - Update rabMod with rabAllowed status received from RRM for
 *              every RAB 
 *
 * @param[out] rabMod   : ERAB setup transaction pointer
 * @param[in]  ueRecfgRsp : RRM ue config rsp pointer
 * @return Void
 */
PRIVATE Void wrUmmUpdateRabModTrans
(
WrUmmRabModifyTransCb     *rabMod,
RmuUeRecfgRsp             *ueRecfgRsp
)
{
   U32       idx,idx1;
   U8        flag = FALSE;

   for(idx = 0; idx < rabMod->numRabs; idx++)
   {
      for(idx1 = 0; (FALSE == flag) && (idx1 < ueRecfgRsp->bNoErabsAccepted); idx1++)
      {
         if(rabMod->rabs[idx].erabId == ueRecfgRsp->stErabAcceptLst[idx1].bErabId)
         {
            rabMod->rabs[idx].rabAllowed = TRUE;
            flag = TRUE;
         }
      }
      
      for(idx1 = 0; (FALSE == flag) && (idx1 < ueRecfgRsp->bNoErabsRejected) ; idx1++)
      {
         if(rabMod->rabs[idx].erabId == ueRecfgRsp->stErabRejectLst[idx1].bErabId)
         {
            rabMod->rabs[idx].rabAllowed = FALSE;
            rabMod->rabs[idx].cause.causeTyp = WR_CAUSE_RADIONW;
            rabMod->rabs[idx].cause.causeVal = ueRecfgRsp->stErabRejectLst[idx1].enCause;
         }
      }
      flag = FALSE;
   }

   RETVOID;
}




/** @brief This function is called when any message related to a particular
 *       E-RAB MODIFY is received. This is the main function that determines 
 *       next course of action based on the message received.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - RRM is invoked based on the information reaceived in ERAB MODIFY.
 *         - Once RRM allows modification, scheduler is configured with the 
 *           reconfiguration of RBs. 
 *         - Upon receiving, scheduler confirmation, RRC is configured to 
 *           send out RRC Reconfiguration and the lower layer configuration.
 *         - When RRC Reconfiguration complete is received, a response is sent
 *           to MME.  
 *
 *
 * @param WrUmmTransCb  *transCb  
 * @return ROK 
 */ 
PUBLIC S16 wrUmmRabModifyProcessor
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   SztDatEvntReq             erabModRsp;
   U8                        numRabs;
   WrUmmRabRlsLst            *rabLst;
/*Removed DRX flag*/ 
   WrUmmRabModifyTransCb     *rabModifyTransCb = &transCb->u.rabModifyTransCb;

   if(transCb->state == WR_UMM_TRANS_INUSE)
   {

   switch(msg->msgTyp)
   {
      case WR_UMM_S1_DAT_IND:
      {
         wrUmmRabModifyProcessorForS1DatInd(transCb);
         /*LC change moving it to RrmRecfgCfm */
         break;
      }
      case WR_UMM_SCHD_CFG:
         if(rabModifyTransCb->state == WR_UMM_RAB_MODIFY_SCHD_DRX_CFG_DONE)
         {
            S16            ret;
            U8             idx;
            U8             succIdx = 0;
            /* committing the information to ueCb */
            wrUmmRabModifyCommitUeInfo(transCb);

            numRabs = transCb->u.rabModifyTransCb.numRabs;

            WR_ALLOC(&rabLst, numRabs * sizeof(WrUmmRabRlsLst));
            if (rabLst == NULLP)
            { 
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            } 
            for (idx = 0;idx < numRabs;idx++)
            {
               if(transCb->u.rabModifyTransCb.rabs[idx].rabAllowed)
               {
                  rabLst[succIdx].rabFailed = FALSE;
               }
               else
               {
                  rabLst[succIdx].rabFailed = TRUE;
                  rabLst[succIdx].cause.causeTyp = transCb->u.rabModifyTransCb.rabs[idx].cause.causeTyp; 

                  /*Update the statistics for ERAB-Modify Attempted*/
                  wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_MODIFY,
                        WR_EMM_DFLT_POS_STEP,WR_EMM_ERAB_FAIL_FAILUREINTHERADIOINTERFACEPROCEDURE);
                  /* Need to Enhace this Feature for multiple cause Values */
                  rabLst[succIdx].cause.causeVal = 
                     transCb->u.rabModifyTransCb.rabs[idx].cause.causeVal; 
               }
               /* FIX for ccpu00135460 */
               /* Filling QCI to make RAB MOD KPIs count proper */
               rabLst[succIdx].qci = transCb->u.rabModifyTransCb.rabs[idx].qci;
               rabLst[succIdx++].erabId = transCb->u.rabModifyTransCb.rabs[idx].erabId; 
            }

            if(transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg->
                  u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
            {
               wrUmmCommitUeRadioResDedInfo(transCb->ueCb, 
                     &(transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg));
            }

            wrUmmRlsIncMsg(&transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg);

            WR_SET_ZERO(&erabModRsp, sizeof(SztDatEvntReq));
            /* Send RAB Modify Response over S1 interface to MME        */
            ret = wrUmmS1apBldERABModRsp(transCb->ueCb->s1ConCb,
                  &erabModRsp.pdu,succIdx,rabLst);
            if (ret != ROK)
            {
               /*ccpu00127802*/
               RLOG0(L_ERROR, "ERAB Modify RSP MSG building  failed");
            }

            /*Freeing the local structure*/
            WR_FREE(rabLst, numRabs * sizeof(WrUmmRabRlsLst));
            erabModRsp.spConnId = transCb->ueCb->s1ConCb->spConnId;
            ret = WrIfmS1apDatReq(&erabModRsp);
            if (ret != ROK)
            {
               /*ccpu00127802*/
               RLOG0(L_ERROR, "Sending ERAB Modify Response failed");
            }

            /* Release the transaction CB as the transaction is complete*/
            wrUmmTransComplete(transCb);
         }
         else
         {
            /* We have to wait to receive all the configuration confirms     */
            wrUmmRabModifyPrcSchdCfgCfm(transCb);

            /* Revert SCHD CFG for failed RABs*/
            wrUmmRabModHdlSchdCfmFail(transCb);

            if (transCb->u.rabModifyTransCb.state == WR_UMM_RAB_MODIFY_SCHD_CFG_DONE)
            {
               /* Perform configuration towards stack and also dispatch UE   */
               /* reconfiguration message on Uu interface                    */
               if(ROK != wrUmmRabModifyPrepRrcMsgAndCfg(transCb))
               {
                  RLOG_ARG0(L_ERROR, DBG_CRNTI, transCb->ueCb->crnti,
                     "Failed to prepare RRC Reconfig message");

                  transCb->errorCause = WR_UMM_CTXT_REL_SW_ERROR;
                  transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
                  RETVALUE(ROK);
               }
            }
            else if((WR_UMM_RAB_MODIFY_SCHD_CFG_RVRT_DONE == 
                  transCb->u.rabModifyTransCb.state) && 
                  (TRUE == transCb->u.rabModifyTransCb.allRabsFailed))
            {
               /* Send the S1AP response and close the transaction if all
                * incoming ERABs were reverted*/
               wrUmmRabModSndS1apRsp(transCb);
               wrUmmTransComplete(transCb);
               RETVALUE(ROK);
            }
         }
         break;
      case WR_UMM_TMR_EXPIRY:
      {
        switch (transCb->msg->u.tmrExpiry.timerEvnt )
        {
           case WR_TMR_RECONFIGURE: 
           {
             if (transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg != NULLP)
             {
                wrUmmRlsIncMsg(&transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg);
             }


             /*Update the Statistics for ERAB-Modify Attempted*/
             wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_MODIFY,
                             WR_EMM_DFLT_POS_STEP,WR_EMM_ERAB_FAIL_RADIOCONNECTIONWITHUELOST);
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
         NhuUlDcchMsg   *ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
         NhuUL_DCCH_MsgTypc1 *c1 = &ulDcch->dcchMsg.message.val.c1;
               /*Removed DRX flag*/ 
         RmuUeRecfgRsp *rrmUeRecfgResp = transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
         U8             rrcTransId = c1->val.rrcConRecfgnCompl.rrc_TrnsxnId.val;
         S16            ret;
         U8             idx;
         U8             succIdx = 0;

         RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[OTA] Received "
               "RRCConnectionReconfigurationComplete");
#ifdef WR_TEST_CODE
         if ( wrTrgRrcReConTO == TRUE )
         {
           RLOG0(L_DEBUG, "Dropping RRCCONRECFGNCOMPL!!");
           RETVALUE(ROK);
         }
#endif
         /* Stop the RRC Reconfiguration Timer */
         wrStopTransTmr(transCb,WR_TMR_RECONFIGURE);

         /* We received confirmation from UE for the reconfiguration      */
         /* We release the RRC reconfiguration message transaction id     */
         ret = wrUmmRelRrcTransId(transCb->ueCb, C1_RRCCONRECFGN, rrcTransId);
         if (ret != ROK)
         {
            /*ccpu00127802*/
            RLOG0(L_ERROR, "Release of transaction ID failed");
         }

            if((rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & 
                   RM_RCM_MAC_MAIN_CFG) &&
               (rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap & 
                   RM_RCM_MAC_MAIN_CFG_DRX))
            {
               wrUmmRabModDrxReCfgSchdUeRecfgReq(transCb); 
            }   
            else
            {
               /* committing the information to ueCb */
               wrUmmRabModifyCommitUeInfo(transCb);

               numRabs = transCb->u.rabModifyTransCb.numRabs;

               WR_ALLOC(&rabLst, numRabs * sizeof(WrUmmRabRlsLst));
               if (rabLst == NULLP)
               { 
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  RETVALUE(RFAILED);
               } 
               for (idx = 0;idx < numRabs;idx++)
               {
                  if(transCb->u.rabModifyTransCb.rabs[idx].rabAllowed)
                  {
                     rabLst[succIdx].rabFailed = FALSE;
                  }
                  else
                  {
                     rabLst[succIdx].rabFailed = TRUE;
                     rabLst[succIdx].cause.causeTyp = transCb->u.rabModifyTransCb.rabs[idx].cause.causeTyp; 

                     /*Update the statistics for ERAB-Modify Attempted*/
                     wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_MODIFY,
                           WR_EMM_DFLT_POS_STEP,WR_EMM_ERAB_FAIL_FAILUREINTHERADIOINTERFACEPROCEDURE);
                     /* Need to Enhace this Feature for multiple cause Values */
                     rabLst[succIdx].cause.causeVal = 
                        transCb->u.rabModifyTransCb.rabs[idx].cause.causeVal; 
                  }   
                  /* FIX for ccpu00135460 */
                  /* Filling QCI to make RAB MOD KPIs count proper */
                  rabLst[succIdx].qci = transCb->u.rabModifyTransCb.rabs[idx].qci;
                  rabLst[succIdx++].erabId = transCb->u.rabModifyTransCb.rabs[idx].erabId; 
               }

               if(transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg->
                     u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
               {
                  wrUmmCommitUeRadioResDedInfo(transCb->ueCb, 
                        &(transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg));
               }

               wrUmmRlsIncMsg(&transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg);

               WR_SET_ZERO(&erabModRsp, sizeof(SztDatEvntReq));
               /* Send RAB Modify Response over S1 interface to MME        */
               ret = wrUmmS1apBldERABModRsp(transCb->ueCb->s1ConCb,
                     &erabModRsp.pdu,succIdx,rabLst);
               if (ret != ROK)
               {
                  /*ccpu00127802*/
                  RLOG0(L_ERROR, "ERAB Modify RSP MSG building  failed");
               }

               /*Freeing the local structure*/
               WR_FREE(rabLst, numRabs * sizeof(WrUmmRabRlsLst));
               erabModRsp.spConnId = transCb->ueCb->s1ConCb->spConnId;
               ret = WrIfmS1apDatReq(&erabModRsp);
               if (ret != ROK)
               {
                  /*ccpu00127802*/
                  RLOG0(L_ERROR, "Sending ERAB Modify Response failed");
               }

               RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,
                     "[S1AP] Sending E-RAB MODIFY RESPONSE "
                     "[MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
                     transCb->ueCb->s1ConCb->mme_ue_s1ap_id,
                     transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
               /* Release the transaction CB as the transaction is complete*/
               wrUmmTransComplete(transCb);
            }
         break;
      }
      case WR_UMM_RRC_CFG:
      {
         wrUmmRabModifyProcRrcCfgCfm(transCb);
      }
      break;
      case WR_UMM_RRM_UERECFG_RESP: 
      {
         if(FALSE == transCb->u.rabModifyTransCb.rrmConfigRvrt)
         {
            transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg = transCb->msg;
            transCb->msg->refCnt++;
            RmuUeRecfgRsp *ueRecfgRsp = transCb->msg->u.rrmUeRecfgResp;
            if(transCb->msg->u.rrmUeRecfgResp->bRecfgBitMap &
                  RMU_RECFG_REQ_ERAB_CONFIG)
            {
               /* RRM_ARP: update transCb->u.rabModifyTransCb with
                * transCb->msg->u.rrmUeRecfgResp 
                */
               wrUmmUpdateRabModTrans(rabModifyTransCb,ueRecfgRsp);
               /*RRM_ARP: If all ERABs requested are rejected by RRM then send ERAB
                * modify response and close the transaction*/
               if(ueRecfgRsp->bNoErabsRejected == rabModifyTransCb->numRabsFound)
               {
                  wrUmmRabModSndS1apRsp(transCb);
                  wrUmmTransComplete(transCb);
                  RETVALUE(ROK);
               }
            }
            /* Configure scheduler for the RABs and AMBR received            */
            wrUmmRabModifyCfgSchd(transCb);
         }
         break;
      }
   }
   }
   else if (transCb->state == WR_UMM_TRANS_STATE_ROLLBACK)
   {
      switch(msg->msgTyp)
      {
         case WR_UMM_SCHD_CFG:
         {
            wrUmmRvrtPrcSchdCfgCfm(transCb);
            break;
         }
      }
      if (transCb->u.rabModifyTransCb.schdUndoCfgState)
      {
         if (transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg != NULLP)
         {
            wrUmmRlsIncMsg(&transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg);
         }

         wrUmmTransComplete(transCb);
      } 
  }
  else
  {
      /*ccpu00127802*/
      RLOG0(L_ERROR, "Invalid Transaction state in wrUmmRabModifyProcessor");
  }

   RETVALUE(ROK);
}
/** @brief This function performs the releases of resources
 *         which are transiently created during processing
 *         of this transaction.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It does cleaup of resources created during the 
 *           processing of this transaction. 
 *
 *
 * @param WrUmmTransCb *transCb 
 * @return ROK 
 */ 
PUBLIC S16 wrUmmRabModifyTransRel
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabModifyTransCb      *rabMod = &transCb->u.rabModifyTransCb;

   WR_FREE(rabMod->rabs, rabMod->numRabs * sizeof(*rabMod->rabs));
   wrUmmRlsIncMsg((WrUmmIncMsg **)&rabMod->msg);
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

PRIVATE S16 wrUmmModRvrtRrcMsgAndCfg
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabModifyTransCb           *rabMod = &transCb->u.rabModifyTransCb;
   NhuCfgReqSdus                   *recfgMsg;
   WrUeCb                          *ueCb = transCb->ueCb;
   U32                             idx;
   NhuRbCfgInfo                    *drbCfg;
   WrUmmRabModifyRabInfo           *rab;
   WrUeRAB                         *rabInfo;
   NhuRbCfgList                    *rbCfgList;
   U32                             numDrbs = 0;
   U8                              freeIdx   = 0;
 

   WR_ALLOCEVNT(&recfgMsg, sizeof(NhuCfgReqSdus));
   if (recfgMsg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   recfgMsg->hdr.cellId  = ueCb->cellId;
   recfgMsg->hdr.ueId    = ueCb->crnti;
   recfgMsg->hdr.transId = transCb->transId;

   recfgMsg->cfgInfo.ueCfgBitMsk = NHU_EXP_CFG_RB_CFG;
   recfgMsg->cfgInfo.rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_MODIFY;
   rbCfgList = &(recfgMsg->cfgInfo.rbCfgAllList.rbToModCfgList);
   WR_GET_MEM(recfgMsg, (sizeof(NhuRbCfgInfo) * rabMod->numRabs), 
                                              &(rbCfgList->rbCfgInfoList));
   for (idx = 0; idx < rabMod->numRabs; idx++)
   {
      freeIdx = (rabMod->rabs[idx].erabId) - WR_UMM_ERAB_OFFSET; 
      rabInfo = &(ueCb->rbInfo.rab[freeIdx]);
      rab     = &rabMod->rabs[idx];

      rab->qci      = rabInfo->qci;
      drbCfg  = rbCfgList->rbCfgInfoList + idx;

      wrUmmRabModifyRrcReconfigModDrbCfg(ueCb->cellId, drbCfg, rab);
      numDrbs++;
   }
   rbCfgList->numOfRbCfgInfo = numDrbs;

   /*Send message towards UE*/
   if (ROK != wrIfmRrcSndCfgReq(recfgMsg))
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe : failed.");
      RETVALUE(RFAILED);
   } 
   RETVALUE(ROK);
}


/** @brief This function performs the UE Reconfiguration
 *         to the MAC Scheduler with old values. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It does the UE reconfiguration to the MAC 
 *           Scheduler with old QOS parameters .
 *         - It delivers the UE RECFG REQ to SCHD through
 *           IFM module. 
 *
 *
 * @param WrUmmTransCb *transCb 
 * @return ROK 
 */ 
PRIVATE S16 wrUmmRvrtSchdUeRecfgReq
(
WrUmmTransCb                 *transCb
)
{
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   U32                       transId;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Set last 4 bits as 1 for UE reconfiguration */
   transId = transCb->transId | WR_RAB_MODIFY_UE_TRANS_ID;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;
   ueRecfg->ueRecfgTypes = RGR_UE_QOS_RECFG;
   ueRecfg->ueQosRecfg.ambrPres = TRUE;
   ueRecfg->ueQosRecfg.dlAmbr   = (transCb->ueCb->rbInfo.aggBr.dl) / 8;
   ueRecfg->ueQosRecfg.ueBr     = (transCb->ueCb->rbInfo.aggBr.ul) / 8;

   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}

/** @brief This function is called when any message related to a RRC Re-establsiment
 *         transaction during E-RAB Modify transaction. 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmDefRabModIncReestProcessor 
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDefRabModIncReestProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
   WrUmmRabModifyTransCb     *rabMod = &(transCb->u.rabModifyTransCb);

   if (rabMod->state == WR_UMM_RAB_MODIFY_SCHD_CFG_DONE)
   {
      NhuUlCcchMsg               *ulCcchMsg;
      NhuRRCConReestbRqst        *reestab;
      NhuRRCConReestbRqst_r8_IEs *r8_IE;
      U8                         idx;

      ulCcchMsg = &(incTrans->msg->u.uuIndPdu->sdu.m.ulCcchMsg);
      reestab   = &(ulCcchMsg->ccchMsg.message.val.c1.val.rrcConReestbRqst);
      r8_IE     = &(reestab->criticalExtns.val.rrcConReestbRqst_r8);

      /* Stop the RRC Re-Cfg Timer coz of ReCfg failed cause*/
      wrStopTransTmr(transCb,WR_TMR_RECONFIGURE);
    
      if ((r8_IE->reestablishmentCause.val == NhuReestbCausereconfigurationFailEnum)
        || (r8_IE->reestablishmentCause.val == NhuReestbCauseotherFailEnum))
      {

         if (transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg != NULLP)
         {
            wrUmmRlsIncMsg(&transCb->u.rabModifyTransCb.rrmUeRecfgRspMsg);
         }

        /*Revert Back RRC Configuration during Rab Setup*/
        wrUmmModRvrtRrcMsgAndCfg(transCb); 
 
        /* Revert Configuration the scheduler with the old AMBR values   */
        if ((rabMod->dlAmbr != 0) || (rabMod->ulAmbr != 0))
        {
           wrUmmRvrtSchdUeRecfgReq(transCb);
        } 
        /* Revert Configuration scheduler for each new RAB being modified. */
        for (idx = 0; idx < rabMod->numRabs; idx++)
        {
              wrUmmRvrtSchdLchRecfgReq(transCb,idx);
        }
        /* At this point, we return ERAB Modify Response with failed list to MME.*/
         /* Continue with the Re-establsihment transaction                       */

         for (idx = 0;idx < rabMod->numRabs;idx++)
         {
             if(rabMod->rabs[idx].rabAllowed == TRUE)
             {
                rabMod->rabs[idx].rabAllowed = FALSE;
                rabMod->rabs[idx].cause.causeTyp = WR_CAUSE_RADIONW; 
                rabMod->rabs[idx].cause.causeVal =
                       SztCauseRadioNwfailure_in_radio_interface_procedureEnum;
             }
         }
         /* Send RAB Modify Response over S1 interface to MME        */
         wrUmmRabModSndS1apRsp(transCb);
      }
      transCb->state = WR_UMM_TRANS_STATE_ROLLBACK;
      /*TODO Transaction is complete till we handle confirms with old CRNTI*/
      wrUmmTransComplete(transCb); 
   }
   else
   {
     /*TODO Actions due to Radio Link failure*/
     RLOG0(L_DEBUG, "Radio Link failure during RAB Setup received at Defered "
        "processor");
   }
   RETVALUE(ROK);
}

/** @brief This function is called when any message related to Handover 
 *         transaction during Rab Modification transaction. 
 *         This function will send ERAB Modification Response
 *         with failed bearer list for all bearers with proper cause value.
 *
 * @details
 *
 *     Function: wrUmmDefRabModIncHoProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDefRabModIncHoProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
   wrUmmHOHandleRabModify(transCb,transCb->ueCb->hoType);

   /* Send the ERAB modify response with failed Bearer list */
   wrUmmRabModSndS1apRsp(transCb);

   wrUmmTransComplete(transCb);
   RETVALUE(ROK);
}

/** @brief This function is called when any message related to ue context release
 *         transaction during Rab Modification transaction. 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmDefRabModIncCtxtRelProcessor 
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDefRabModIncCtxtRelProcessor 
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
  wrUmmStopTransTimers(transCb);
  RETVALUE(ROK);
}



/** @brief This function is called when any message related to a particular
 *         transaction during E-RAB Modify transaction. 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmRabModNewTransProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  inctrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmRabModNewTransProcessor 
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
              wrUmmDefRabModIncReestProcessor(transCb,incTrans);
              break;
           } 

        case WR_UMM_CTXT_REL_TRANS:
           {
              wrUmmDefRabModIncCtxtRelProcessor(transCb,incTrans);
              transCb->state = WR_UMM_TRANS_DONE;
              break;
           }
           /* ccpu00130441 and ccpu00130443 */
        case WR_UMM_S1_SRC_HO_TRANS:
        case WR_UMM_X2_SRC_HO_TRANS:
           {
              wrUmmDefRabModIncHoProcessor(transCb,incTrans);
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
        case WR_UMM_ERAB_MOD_TRANS:
           {
              RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Deferring "
                         "incoming transaction %d ",incTrans->transTyp);
              WR_UMM_CPY_DEFR_MSG_TO_TRANSCB(transCb, incTrans->msg);
              ret = WR_UMM_TRANS_FAIL;
              break;
           }
        default:
           {
              RLOG1(L_ERROR, "Transaction %d received at RAB Modify processor",
                    incTrans->transTyp);
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
