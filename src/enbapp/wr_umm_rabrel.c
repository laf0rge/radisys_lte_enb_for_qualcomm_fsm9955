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
               

     File:     wr_umm_rabrel.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=63;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_umm_msg.h"
#include "wr_umm.h"
#include "wr_umm_rabrel.h"
#include "wr_emm.h"
#include "wr_umm_trans.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_sch_msg.h"
#include "wr_umm_s1ap_msg.h"
#include "wr_ifm_schd.h"
#include "wr_ifm_dam.h"
#include "wr_ifm_rrm.h"
#include "wr_ifm_rrc.h"
#include "wr_kpi.h"
#include "wr_umm_drx.h"

#ifdef WR_TEST_CODE
EXTERN U8 wrTrgRrcReConTO;
#endif

EXTERN S16 wrMeasFillNhuMsg
(
 CmMemListCp                  *memCp,
 NhuMeasConfig                *nhuMeasCfg,
 WrUeCb                       *ueCb,
 WrUmmMeasTransInfo              *measTransCb,
 LwrCellMeasCfgGrp            *measCellCfg
);

/** @brief This function is called to handle RRC configuration
 *         confirm. 
 *
 * @details
 * Invoked by - wrUmmRabRelProcessor
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It validates the confirmation from RRC whether it is
 *           success or failure.
 *
 *
 *
 * @param WrUmmTransCb  *transCb 
 * @return ROK/RFAILED 
 */ 
PRIVATE S16 wrUmmRabRelProcRrcCfgCfm
(
WrUmmTransCb            *transCb
)
{
   WrUmmRabRelTransCb       *rabTrans = &transCb->u.rabRelTransCb;
   NhuCfgCfmSdus            *rrcCfm;

   rrcCfm = transCb->msg->u.rrcCfm;
   if(rrcCfm->sdu.isCfgSuccess == FALSE)
   {
      rabTrans->isDrbCfgDone = FALSE;
      /* logging error */
      RLOG0(L_ERROR, "DRB Config failed");
      RETVALUE(RFAILED);
   }
   else
   {
      rabTrans->isDrbCfgDone = TRUE;
   }
   RETVALUE(ROK);
}

/* SPS changes starts */
/** @brief This function is responsible for filling the UL SPS LC for SCHD.
 *         With our current implementation we will never encounter this case
 *         since with the release of SPS ERAB, SPS will be released. 
 *
 * @details
 *
 *     Function: wrUmmRabRelSchdUlSpsLchCfgReq
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

PUBLIC Void wrUmmRabRelSchdUlSpsLchCfgReq
(
 RmuUeRecfgRsp           *rrmUeRecfgResp,
 U32                     numErabs,
 Void                    *pErabList,
 RgrUeSpsCfg             *ueSpsCfg
 )
{
   U32 count;
   WrUmmRabRelRabInfo  *pErabInfo = (WrUmmRabRelRabInfo *)pErabList;

   ueSpsCfg->ulSpsCfg.lcCnt = 0;
   for (count = 0; count < numErabs; count++)
   {
      /* Here Application should take stuffs from the RRM accepted list but currently 
       * there is no RBC support at both APP and RRM. Thus the expectation is, RRM has accepted 
       * all the ERABs in the request and thus have provided ERAB in the same order as sent to it. */
      /* Secondly, since the ERABs is being deleted, so all the UL LC for the ERABs are maked as non-SPS.
       * This decision is independent of the fact whether RRM is keeping/modifying/deleting the configuration.
       * The keeping/modifying is for future enhancement when more than one ERAB is configured as SPS and
       * one of then is deleted as a result RRM has modified the SPS configuration.*/
      /* Thus commenting the statements for filling the LCs
      ueSpsCfg->ulSpsCfg.spsLcInfo[ueSpsCfg->ulSpsCfg.lcCnt].isSpsEnabled = FALSE;
      ueSpsCfg->ulSpsCfg.spsLcInfo[ueSpsCfg->ulSpsCfg.lcCnt].lcId = (pErabInfo+count)->lchId;
      ueSpsCfg->ulSpsCfg.lcCnt++; */
      (pErabInfo+count)->isUlSpsEnabled = FALSE;
   }
}
/* SPS changes ends */

/** @brief This function is responsible for reconfiguring the UE context
 *         at the scheduler.
 *
 * @details
 *
 *     Function: wrUmmRabRelSchdUeRecfgReq
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
PRIVATE S16 wrUmmRabRelSchdUeRecfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabRelTransCb        *rabRel = &transCb->u.rabRelTransCb;
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
   transId = transCb->transId | WR_RAB_REL_UE_TRANS_ID;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   if ((rabRel->dlAmbr != 0) || (rabRel->ulAmbr != 0))
   {
      ueRecfg->ueRecfgTypes = RGR_UE_QOS_RECFG;
      ueRecfg->ueQosRecfg.ambrPres = TRUE;
      ueRecfg->ueQosRecfg.dlAmbr   = rabRel->dlAmbr / 8;
      ueRecfg->ueQosRecfg.ueBr     = rabRel->ulAmbr / 8;
   }
   
   if(rabRel->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
   {
      wrUmmSchdFillUeRadioResCfgInfo(ueRecfg, 
            (rabRel->rrmUeRecfgRspMsg->u.rrmUeRecfgResp), transCb->ueCb, FALSE);
   }
   /* SPS changes starts */
   wrUmmFillSpsSchdReCfgUe(transCb->transTyp, transCb->ueCb, rabRel->rrmUeRecfgRspMsg->u.rrmUeRecfgResp, \
         ueRecfg, rabRel->numRabs, (Void *)(rabRel->rabs), &transCb->spsCfgTransCb.spsCfg, \
         &transCb->spsCfgTransCb.isUeDlSpsEnabled, &transCb->spsCfgTransCb.isUeUlSpsEnabled);
   /* SPS changes ends */

   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}




/** @brief This function is called to handle logical channel
 *         confirmation from MAC Scheduler. 
 *
 * @details
 * Invoked by - wrUmmRabRelProcessor
 *     Function: 
 *
 *         Processing steps:
 *         - It validates the config confirmation from Scheduler
 *           whether it is success or failure for each logical
 *           cahnnel.  
 *
 *
 * @param WrUmmTransCb *transCb 
 * @param U8           idx Logical channel Id
 * @return ROK/RFAILED
 */ 


PRIVATE S16 wrUmmRabRelSchdLchCfgReq
(
WrUmmTransCb                 *transCb,
U8                           idx
)
{
   WrUmmRabRelTransCb        *rabRel = &transCb->u.rabRelTransCb;
   RgrCfgReqInfo             *cfgReq;
   U32                       transId;
   RgrDel                    *lchdelCfg;
   U32                       rbIdx;
   U8                        lcgId;

   rbIdx = rabRel->rabs[idx].erabId - WR_UMM_ERAB_OFFSET;
   lcgId = transCb->ueCb->rbInfo.rab[rbIdx].lcgId;

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
   lchdelCfg->u.lchDel.lcId   = rabRel->rabs[idx].lchId;
   lchdelCfg->u.lchDel.lcgId  = lcgId;

   /* SPS changes starts */
   /* The SPS configuration for the LC has to be removed as the ERAB is being deleted */
   rabRel->rabs[idx].isDlSpsEnabled = FALSE;
   /* SPS changes ends */

   /* Dispatch the prepared message to scheduler                    */
   WrIfmSchdCfgUe(cfgReq, transId);

   if (WR_UMM_IS_GBR_BEARER(rabRel->rabs[idx].qci))
   {
      /* Converting kbps to kBps, because Schedular expect in terms of byte */
      transCb->ueCb->rbInfo.rab[rbIdx].gbr.ul /= 8;
      transCb->ueCb->rbInfo.rab[rbIdx].mbr.ul /= 8;
      transCb->ueCb->mappedLcgInfo[lcgId].effUlGbr -= transCb->ueCb->rbInfo.rab[rbIdx].gbr.ul;
      transCb->ueCb->mappedLcgInfo[lcgId].effUlMbr -= transCb->ueCb->rbInfo.rab[rbIdx].mbr.ul;

      wrUmmSchdUeLcgReCfg(transCb->ueCb, transCb->transId, lcgId, \
            transCb->ueCb->mappedLcgInfo[lcgId].effUlGbr,transCb->ueCb->mappedLcgInfo[lcgId].effUlMbr);
      rabRel->cfgStateMsk |= WR_UMM_RAB_REL_LCG_CFG_SENT;
      rabRel->lcgCfgStatus[lcgId] = WR_UMM_RAB_REL_LCG_CFG_SENT;
   }

   RETVALUE(ROK);
}
/** @brief This function configures each logical channel
 *         to be deleted to the MAC Scheduler. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It configures the MAC Scheduler with a list of logical
 *           channels to be deleted. 
 *
 *
 * @param WrUmmTransCb *transCb 
 * @return ROK 
 */ 

PRIVATE S16 wrUmmRabRelCfgSchd
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabRelTransCb        *rabRel = &transCb->u.rabRelTransCb;
   U32                       idx;

/* SPS changes starts */
   /* The initialisation from the ueCb is required for the following case:
    * 1) If the SPS was already enabled for the UE, thus RRM doesnot allocate SPS
    *    - The SR mask for all the LCs should be set for rel 9 UE based on this.
    * 2) If the SPS was neither enabled previously nor it is enabled by RRM
    *    - Donot enable SR Mask for any of the LCs based on this. */
   transCb->spsCfgTransCb.isUeUlSpsEnabled = transCb->ueCb->spsCfg.spsCfgUl.bIsSpsEnabled;
   transCb->spsCfgTransCb.isUeDlSpsEnabled = transCb->ueCb->spsCfg.spsCfgDl.bIsSpsEnabled;
/* SPS changes ends */
   /* Configure the scheduler with the updated AMBR values                */
   if ((rabRel->dlAmbr != 0) || (rabRel->ulAmbr != 0) ||
         (rabRel->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap))
   {
      wrUmmRabRelSchdUeRecfgReq(transCb);
      rabRel->cfgStateMsk |= WR_UMM_RAB_REL_UE_CFG_SENT;
   }

   /* Configure the scheduler for each  RAB delete              */
   for (idx = 0; idx < rabRel->numRabs; idx++)
   {
      if(rabRel->rabs[idx].rabfailed != TRUE)
      {  
         wrUmmRabRelSchdLchCfgReq(transCb, idx);
         rabRel->cfgStateMsk |= WR_UMM_RAB_REL_LCH_CFG_SENT;
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
 *     Function: wrUmmRabRelDrxRelSchdUeRecfgReq
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
PRIVATE S16 wrUmmRabRelDrxRelSchdUeRecfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabRelTransCb        *rabRel = &transCb->u.rabRelTransCb;
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   RmuUeRecfgRsp *rrmUeRecfgResp = rabRel->rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
   U32                       transId;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   transId = transCb->transId | 0x0f;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   /* Mark the transaction that UE config has been sent */
   rabRel->state = WR_UMM_RAB_REL_SCH_CFG_DRX;
   wrUmmSchdFillUeDrxCfgInfo(ueRecfg, (WrDrxCfg *)&rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.stRmDrxCfg);
   
   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
} 
/*Removed DRX flag*/

/** @brief This function processes the ERAB ID list from the
 *         ERAB RELEASE msg and updates into the transaction CB.  
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It parses the RAB ID list from the S1Ap message to 
 *           be deleted and updates the same into the transaction
 *           Control block.  
 *
 *
 * @param WrUmmRabRelTransCb *rabTrans RAB Release Transaction CB 
 * @param SztE_RABLst        *rabLst   S1AP RAB Lst IE
 * @param WrUeCb             *ueCb     UE Control Block
 * @return ROK/RFAILED 
 */ 

PRIVATE S16 wrUmmPrcRabRelLst
(
WrUmmRabRelTransCb           *rabTrans,
SztE_RABLst                  *rabLst,
WrUeCb                       *ueCb
)
{
   U32                          numRabs;
   U8                           idx;
   U32                          eRabId;
   U32                          indx = 0;
   U8                           eRabIdx = 0;
   U8                           rabOff;
   SztE_RABItem                 *rabItem;
   S32                          ret = RFAILED;

   numRabs = rabLst->noComp.val;
   /*Updating the TransCb*/
   WR_ALLOC(&rabTrans->rabs, numRabs * sizeof(WrUmmRabRelRabInfo));
   if (rabTrans->rabs == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ret);
   }

   idx = indx =  0;

   /*
     To handle the bearers received with eRabId 
     invalid range. 
   */
   for(idx = 0; idx < numRabs; idx++)
   {
     eRabId = rabLst->member[idx].value.u.sztE_RABItem.e_RAB_ID.val;
     rabItem  = &rabLst->member[idx].value.u.sztE_RABItem;
     rabOff   = rabItem->e_RAB_ID.val - WR_UMM_ERAB_OFFSET;

     if((TRUE == rabTrans->isUeAggrMaxBitRateRcvd) &&
         (0 == rabTrans->dlAmbr) && (0 == rabTrans->ulAmbr))
     {
        rabTrans->rabs[indx].erabId       = eRabId;
        rabTrans->rabs[indx].relCause.causeTyp = WR_CAUSE_PROTOCOL; 
        rabTrans->rabs[indx].relCause.causeVal = SztCauseProtsemantic_errorEnum;  
        rabTrans->rabs[indx].rabfailed    = TRUE;
        indx++;
        continue;
     }
     if(rabLst->member[idx].value.u.sztE_RABItem.pres.pres != NOTPRSNT)
     {
       rabTrans->rabs[indx].erabId       = eRabId;
       rabTrans->rabs[indx].cause        = 
                   rabLst->member[idx].value.u.sztE_RABItem.cause.choice.val;
       rabTrans->rabs[indx].schdCfgState = WR_RAB_REL_SCHD_CFG_IDLE;
       rabTrans->rabs[indx].rabfailed = FALSE;
       /*Increment KPI for Number of E-RABs attempted to releas*/
       WR_INC_KPI_FUNC1(wrIncKpiFrERABRelAttPrQci,ueCb->rbInfo.rab[rabOff].qci);
       rabTrans->rabs[idx].qci  = ueCb->rbInfo.rab[rabOff].qci; /*Added for KPI*/   
       eRabIdx = (idx + 1);

       /* Handling the RAB Release with multiple Bearers with same bearer ID*/
       while(eRabIdx < numRabs)
       {
          if(eRabId == rabLst->member[eRabIdx].value.u.sztE_RABItem.e_RAB_ID.val)
          { 
            rabLst->member[eRabIdx].value.u.sztE_RABItem.pres.pres = NOTPRSNT;
          }
          eRabIdx++;
       }
       ret = ROK;

       /*
         To handle the bearers received with eRabId 
         invalid range. 
        */
       if(eRabId < WR_UMM_ERAB_OFFSET || eRabId > WR_UMM_MAX_ERABID)
       {
         /*Error_Cause: Filling proper cause value*/ 
         rabTrans->rabs[indx].relCause.causeTyp = CAUSE_RADIONW; 
         rabTrans->rabs[indx].relCause.causeVal = SztCauseRadioNwunknown_E_RAB_IDEnum;  
         rabTrans->rabs[indx].rabfailed    = TRUE;
         indx++;
         continue;
       }
       
       indx++;
     }
   }
   rabTrans->numRabs            = indx;
   RETVALUE(ret);
}
/** @brief This function parses all the IEs from ERAB RELEASE
 *         messages and updates into the tranaction control
 *         block. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It parses ERAB Release msg from MME and updates 
 *           all the IEs into the transaction control block.
 *
 *
 *
 * @param WrUmmTransCb *transCb 
 * @return ROK/RFAILED 
 */ 

PRIVATE S16 wrUmmParseRabRelMsg
(
WrUmmTransCb                 *transCb
)
{
   S1apPdu                   *pdu = transCb->msg->u.s1Pdu->pdu;
   SztInitiatingMsg          *initMsg = &pdu->pdu.val.initiatingMsg;
   WrUmmRabRelTransCb        *rabrelTrans = &transCb->u.rabRelTransCb;
   U32                       idx;
   SztE_RABRlsCmmd           *rabrel = &initMsg->value.u.sztE_RABRlsCmmd;
   SztProtIE_Field_E_RABRlsCmmdIEs   *ie; 
   S32                        ret = ROK;

   for(idx = 0;idx < rabrel->protocolIEs.noComp.val; idx++)
   {
      ie = &rabrel->protocolIEs.member[idx];
      switch(ie->id.val)
      {
         case Sztid_uEaggregateMaxBitrate:
         {
            SztUEAggMaxBitrate *ambr = &ie->value.u.sztUEAggMaxBitrate;
            wrGetBitRate(&ambr->uEaggregateMaxBitRateDL, &rabrelTrans->dlAmbr);
            wrGetBitRate(&ambr->uEaggregateMaxBitRateUL, &rabrelTrans->ulAmbr);
            rabrelTrans->isUeAggrMaxBitRateRcvd = TRUE;
            break;
         }
         case Sztid_E_RABToBeRlsdLst:
         {  
       
            ret = wrUmmPrcRabRelLst(rabrelTrans, 
                                &ie->value.u.sztE_RABLst,transCb->ueCb);
            break;
         }
         case Sztid_NAS_PDU: 
         {   
            rabrelTrans->nasPdu       = &ie->value.u.sztNAS_PDU;
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
 * @param WrUeCb             *ueCb  
 * @param WrUmmRabRelRabInfo *rabrel
 * @return ROK
 */ 

PRIVATE S16 wrUmmRabRelFindLch
(
WrUeCb                       *ueCb,
WrUmmRabRelRabInfo           *rabrel
)
{
   U32             idx =((rabrel->erabId) - WR_UMM_ERAB_OFFSET);
  
   if((idx >=  WR_MAX_DRBS) || (idx < 0)) 
   {
      RETVALUE(RFAILED);
   }
   
   if (ueCb->rbInfo.rab[idx].inUse != FALSE)
   {
      rabrel->lchId = ueCb->rbInfo.rab[idx].lchId;
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
 * @param WrUmmRabRelRabInfo *rabrel
 * @return ROK 
 */ 

PRIVATE S16 wrUmmRabRelFindDrb
(
WrUeCb                       *ueCb,
WrUmmRabRelRabInfo           *rabrel
)
{

   U32             idx =((rabrel->erabId) - WR_UMM_ERAB_OFFSET);
    
   if((idx >=  WR_MAX_DRBS) || (idx < 0)) 
   {
      RETVALUE(RFAILED);
   }
   
   if (ueCb->rbInfo.rab[idx].inUse != FALSE)
   {
     rabrel->drbId = ueCb->rbInfo.rab[idx].rbId;
   }
   else
   {
     RETVALUE(RFAILED);
   }
   
   RETVALUE(ROK);
}

/* ccpu00130441 and ccpu00130443 */
/** @brief This function is responsible for handling handover by filling 
 *  ERAB release failure for each RABs.
 *
 * @details
 *
 *     Function: wrUmmHOHandleRabRelease
 *
 *         Processing steps:
 *         - for the number of RABs that where successfully parsed/admitted, 
 *           - fill the failure cause values and set rabFailed as TRUE 
 *
 * @param[in]  transCb : transaction information
 * @param[in]  hoType : Handover type
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmHOHandleRabRelease
(
WrUmmTransCb              *transCb,
WrHoType                  hoType
)
{
   WrUmmRabRelTransCb        *rabTransCbRel = &transCb->u.rabRelTransCb;
   U32                       idx           = 0;
   
   for (idx = 0; idx < rabTransCbRel->numRabs; idx++)
   {
      rabTransCbRel->rabs[idx].rabfailed = TRUE;
      rabTransCbRel->rabs[idx].relCause.causeTyp  = CAUSE_RADIONW;
      
      if( WR_HO_TYPE_INTRA_LTE_S1 == hoType) 
      {
         rabTransCbRel->rabs[idx].relCause.causeVal  = 
            SztCauseRadioNws1_intra_system_handover_triggeredEnum;
      }
      else if( WR_HO_TYPE_IRAT_UTRA == hoType)
      {
         rabTransCbRel->rabs[idx].relCause.causeVal  = 
            SztCauseRadioNws1_inter_system_handover_triggeredEnum;
      }
      else if( WR_HO_TYPE_X2 == hoType)
      {
         rabTransCbRel->rabs[idx].relCause.causeVal  = 
            SztCauseRadioNwx2_handover_triggeredEnum;
      }
   }
   
   RETVALUE(ROK);
}

/** @brief This function processes the Rab Release
 *         tranaction to determine the LCHID and DRBID
 *         corresponding to ERABID from the existing
 *         Rab control block. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It determines the LCHID and DRBID to be 
 *           deleted corresponding to the ERABID from
 *           the Rab control block. 
 *
 *
 * @param WrUmmTransCb *transCb 
 * @return ROK
 */ 

PRIVATE S16 wrUmmPrcRabRel
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabRelTransCb        *rabTransCbRel = &transCb->u.rabRelTransCb;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmRabRelRabInfo        *rabRelInfo;
   U8                        freeIdx;
   U8                        idx;
   S16                       retVal = ROK;
   RmuUeRecfgReq            *ueRecfgReq;
   RmuErabRelIe             *eRabRelPtr;
   
   WR_ALLOC(&ueRecfgReq, sizeof(RmuCommonMsg));
   if (ueRecfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   eRabRelPtr = ueRecfgReq->stErabConfInfo.stErabRelLst;
   ueRecfgReq->stErabConfInfo.usNoErabsToAddMod = 0;
   ueRecfgReq->stErabConfInfo.bNoErabsToRel = 0;

/*Fix For ccpu00123304*/
   rabTransCbRel->numRabsFound = 0;
   RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[S1AP] Received E-RAB RELEASE "
      "COMMAND [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
      transCb->ueCb->s1ConCb->mme_ue_s1ap_id,
      transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
   /* Need to determine LCH Id and RB Id for each RAB to be released         */
   for (idx = 0; idx < rabTransCbRel->numRabs; idx++)
   {
      /*stop procesing the bearers whose validations already failed*/
      if(rabTransCbRel->rabs[idx].rabfailed == TRUE)
      {
         continue;
      }

      rabRelInfo = &(rabTransCbRel->rabs[idx]);
      freeIdx = (rabRelInfo->erabId) - WR_UMM_ERAB_OFFSET; 
      if((freeIdx >= WR_MAX_DRBS) ||
         (ueCb->rbInfo.rab[freeIdx].inUse == FALSE))
      {
      /*ccpu00127802*/
         RLOG0(L_WARNING, "Unknown RAB ID found for E-RAB Release");
         rabTransCbRel->rabs[idx].rabfailed = TRUE;
         /*TODO Update with Proper Cause Values */
         /*Error_Cause: Filling proper cause value*/ 
         rabTransCbRel->rabs[idx].relCause.causeTyp = CAUSE_RADIONW; 
         rabTransCbRel->rabs[idx].relCause.causeVal = SztCauseRadioNwunknown_E_RAB_IDEnum;  
         rabTransCbRel->rabs[idx].rabfailed = TRUE;
         continue;
      }
      if (wrUmmRabRelFindLch(transCb->ueCb, &rabTransCbRel->rabs[idx]) != ROK)
      {
        /*ccpu00127802*/
        RLOG0(L_WARNING, "Failed to find LCH for a RAB");
        rabRelInfo->relCause.causeTyp = CAUSE_RADIONW;
        rabRelInfo->relCause.causeVal = 
                          SztCauseRadioNwradio_resources_not_availableEnum;
        rabRelInfo->rabfailed = TRUE;
        continue;
      }
      if (wrUmmRabRelFindDrb(transCb->ueCb, &rabTransCbRel->rabs[idx]) != ROK)
      {
        /*ccpu00127802*/
        RLOG0(L_WARNING, "Failed to find DRB for a RAB");
        rabRelInfo->relCause.causeTyp = CAUSE_RADIONW;
        rabRelInfo->relCause.causeVal = 
                          SztCauseRadioNwradio_resources_not_availableEnum;
        rabRelInfo->rabfailed = TRUE;
        continue;
      }
      rabTransCbRel->numRabsFound++;

      eRabRelPtr->bErabId = rabTransCbRel->rabs[idx].erabId;
      eRabRelPtr++;
      ueRecfgReq->stErabConfInfo.bNoErabsToRel++;
      
   }

   if(ueRecfgReq->stErabConfInfo.bNoErabsToRel)
   {
      ueRecfgReq->bCellId = transCb->ueCb->cellId;
      ueRecfgReq->usCrnti = transCb->ueCb->crnti;
      ueRecfgReq->uiTransId = transCb->transId;
      ueRecfgReq->bRecfgBitMap = RMU_RECFG_REQ_ERAB_CONFIG;
      ueRecfgReq->stErabConfInfo.enErabAction = RMU_DEL_ERAB;
      retVal = wrIfmRrmSndUeRecfgReq(ueRecfgReq);
   }
   else
   {
      WR_FREE(ueRecfgReq, sizeof(RmuCommonMsg));
   }

   RETVALUE(retVal);
}
/** @brief This function is called to handle MAC Scheduler
 *         configuration confirm. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It validates the configuration confirm for each 
 *           logical channel whether it is success/Failure. 
 *
 *
 * @param WrUmmTransCb   *transCb 
 * @return ROK
 */ 

PRIVATE S16 wrUmmRabRelPrcSchdCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabRelTransCb        *rabRelTrans = &transCb->u.rabRelTransCb;
   WrUmmSchdCfgCfm           *schdCfm = &(transCb->msg->u.schdCfm);
   U8                        idx;
   U8                        lcgId;
    
   idx = schdCfm->transId & 0x0f;
   if (!(rabRelTrans->cfgStateMsk & WR_UMM_RAB_REL_LCG_CFG_SENT))
   {
      rabRelTrans->cfgStateMsk |= WR_UMM_RAB_REL_LCG_CFG_RECVD;
   }
   if (!(rabRelTrans->cfgStateMsk & WR_UMM_RAB_REL_UE_CFG_SENT))
   {
      rabRelTrans->cfgStateMsk |= WR_UMM_RAB_REL_UE_CFG_RECVD;
   }
   if (!(rabRelTrans->cfgStateMsk & WR_UMM_RAB_REL_LCH_CFG_SENT))
   {
      rabRelTrans->cfgStateMsk |= WR_UMM_RAB_REL_LCH_CFG_RECVD;
   }
   
   if (idx == WR_RAB_REL_UE_TRANS_ID)
   {
      /*ccpu00127802*/
      RLOG0(L_DEBUG, "RAB-REL: UE-recfg schd-cfm rcvd");
      /* This is in response to UE reconfiguration for AMBR */
      rabRelTrans->cfgStateMsk |= WR_UMM_RAB_REL_UE_CFG_RECVD;
   }
   else if (idx == WR_UMM_TRANS_ID_LCG0_CFG)
   {
      rabRelTrans->lcgCfgStatus[0] = WR_UMM_RAB_REL_LCG_CFG_RECVD;
      for (lcgId=0; lcgId < WR_UMM_MAX_LCG_GRP_ID; lcgId++)
      {
         if (rabRelTrans->lcgCfgStatus[lcgId] == WR_UMM_RAB_REL_LCG_CFG_SENT)
         {
            RETVALUE(ROK);
         }
      }
      rabRelTrans->cfgStateMsk |= WR_UMM_RAB_REL_LCG_CFG_RECVD;
   }
   else if (idx == WR_UMM_TRANS_ID_LCG1_CFG)
   {
      rabRelTrans->lcgCfgStatus[1] = WR_UMM_RAB_REL_LCG_CFG_RECVD;
      for (lcgId=0; lcgId < WR_UMM_MAX_LCG_GRP_ID; lcgId++)
      {
         if (rabRelTrans->lcgCfgStatus[lcgId] == WR_UMM_RAB_REL_LCG_CFG_SENT)
         {
            RETVALUE(ROK);
         }
      }
      rabRelTrans->cfgStateMsk |= WR_UMM_RAB_REL_LCG_CFG_RECVD;
   }
   else if (idx == WR_UMM_TRANS_ID_LCG2_CFG)
   {
      rabRelTrans->lcgCfgStatus[2] = WR_UMM_RAB_REL_LCG_CFG_RECVD;
      for (lcgId=0; lcgId < WR_UMM_MAX_LCG_GRP_ID; lcgId++)
      {
         if (rabRelTrans->lcgCfgStatus[lcgId] == WR_UMM_RAB_REL_LCG_CFG_SENT)
         {
            RETVALUE(ROK);
         }
      }
      rabRelTrans->cfgStateMsk |= WR_UMM_RAB_REL_LCG_CFG_RECVD;
   }
   else if (idx == WR_UMM_TRANS_ID_LCG3_CFG)
   {
      rabRelTrans->lcgCfgStatus[3] = WR_UMM_RAB_REL_LCG_CFG_RECVD;
      for (lcgId=0; lcgId < WR_UMM_MAX_LCG_GRP_ID; lcgId++)
      {
         if (rabRelTrans->lcgCfgStatus[lcgId] == WR_UMM_RAB_REL_LCG_CFG_SENT)
         {
            RETVALUE(ROK);
         }
      }
      rabRelTrans->cfgStateMsk |= WR_UMM_RAB_REL_LCG_CFG_RECVD;
   }

   else if (idx < rabRelTrans->numRabs)
   {
      if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
      {
         rabRelTrans->rabs[idx].schdCfgState = WR_RAB_REL_SCHD_CFM_SUCC;
         rabRelTrans->rabs[idx].rabfailed = FALSE;
      }
      else
      {
         /*Error_Cause: Filling proper cause value when scheduler sends failure for
          * particular E-Rab delete
          */
         rabRelTrans->rabs[idx].relCause.causeTyp = CAUSE_RADIONW;
         rabRelTrans->rabs[idx].relCause.causeVal = SztCauseRadioNwradio_resources_not_availableEnum;
         rabRelTrans->rabs[idx].schdCfgState = WR_RAB_REL_SCHD_CFM_FAIL;
         rabRelTrans->rabs[idx].rabfailed = TRUE;
      }
      for (idx = 0; idx < rabRelTrans->numRabs; idx++)
      {
         if (rabRelTrans->rabs[idx].schdCfgState ==  WR_RAB_REL_SCHD_CFG_SENT)
         {
            RETVALUE(ROK);
         }
      }
      rabRelTrans->cfgStateMsk |= WR_UMM_RAB_REL_LCH_CFG_RECVD;
   }
   else
   {
      /*ccpu00127802*/
      RLOG_ARG0(L_WARNING, DBG_TRNSID, transCb->transId,"invalid transaction id"
         " for RAB Release");
      RETVALUE(ROK);
   }

   if ((rabRelTrans->cfgStateMsk & WR_UMM_RAB_REL_CFG_RECVD) == WR_UMM_RAB_REL_CFG_RECVD)
   {
      rabRelTrans->state = WR_UMM_RAB_REL_SCHD_CFG_DONE;
   }

   RETVALUE(ROK);
}
/** @brief This function constructs the UE Reconfiguration
 *         message towards RRC layer. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It builds the information for Ue configuration
 *           parameters towards RRC layer. 
 *
 *
 * @param  NhuDatReqSdus                   *event
 * @param  NhuRadioResourceConfigDedicated *radioRes
 * @param  WrUmmTransCb                    *transCb
 * @return ROK 
 */ 

PRIVATE S16 wrUmmBldUeCfgforReCfg
(
NhuDatReqSdus                   *event, 
NhuRadioResourceConfigDedicated *radioRes,
WrUmmTransCb                    *transCb
)
{

   WrUmmRabRelTransCb              *rabrel = &transCb->u.rabRelTransCb;
   U8                              idx;
   NhuUeCfgInfo                    *ueCfg;
   NhuRbRlsCfgList                 *rbCfgList;
   U8                              rrcIdx = 0;
   WrUmmRabRelRabInfo              *rabInfo;
   NhuDRB_Identity                 *drbIe;

   /* ================================================== 
    *     * Fill UE Configuration
    * ================================================== */
    event->sdu.isUeCfgPres = TRUE;
    ueCfg = &(event->sdu.ueCfg);
    ueCfg->ueCfgBitMsk = NHU_EXP_CFG_RB_CFG;
    ueCfg->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_RLS;
    rbCfgList = &(ueCfg->rbCfgAllList.rbToRlsCfgList);
    rbCfgList->numOfRbCfgInfo = rabrel->numRabs;

    WR_GET_MEM(event, (sizeof(NhuRbId) * rabrel->numRabs),\
         &(rbCfgList->rbsToRel));

   /* ================================================== 
    *     * Fill RADIO RESOURCE Configuration
    * ================================================== */
   WR_FILL_TKN_UINT(radioRes->pres, PRSNT_NODEF);

   /* Allocate memory for each Rab */
   WR_GET_MEM(event, (sizeof(NhuDRB_Identity) * rabrel->numRabs),\
         &(radioRes->drb_ToRlsLst.member));

   for (idx = 0; idx < rabrel->numRabs; idx++)
   {
     if(rabrel->rabs[idx].rabfailed != TRUE)
     { 
      rabInfo = &rabrel->rabs[idx];
      drbIe   = radioRes->drb_ToRlsLst.member + rrcIdx;
      /* We send information of only those RABs that are successful       */
      /* Filling DRB Identity to be released */
      wrFillTknU32(drbIe,rabInfo->drbId);

      /* ================================================== 
      *                 * Fill UE Configuration
      * ================================================== */
      rbCfgList->rbsToRel[rrcIdx].rbId = 
                       rabInfo->drbId;
      rbCfgList->rbsToRel[rrcIdx].rbTyp = CM_LTE_DRB;
      /* updating RRC Index */
      rrcIdx++;
     }
   }/*End of idx for loop*/
   WR_FILL_TKN_UINT(radioRes->drb_ToRlsLst.noComp,rrcIdx);

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

PRIVATE S16 wrUmmRabRelPrepRrcMsgAndCfg
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabRelTransCb              *rabRelTrans = &transCb->u.rabRelTransCb;
   NhuDatReqSdus                   *recfgMsg;
   WrUeCb                          *ueCb = transCb->ueCb;
   NhuDL_DCCH_Msg                  *dcchMsg;
   NhuRRCConRecfgn                 *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs          *recfgIEs;
   NhuRadioResourceConfigDedicated *rrcCfgDed;
   
   /* SPS changes starts */
   NhuDRB_ToAddMod                 *drbIe = NULLP;
   U32 idx = 0, rbIdx = 0, tcbIdx = 0, rabCount = 0;
   Bool isReleasedRab = FALSE;
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
                      &rabRelTrans->transId))
   {
      RLOG0(L_ERROR, "RRC Transaction Not avialble");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }
   /* Create the reconfiguration message which is constructued excluding  */
   /* the actual procedure related details which will be added further    */
   if (wrUmmRrcCreateRrcReconfigMsg(recfgMsg, rabRelTrans->transId) != ROK)
   {
      RETVALUE(RFAILED);
   }

   /* Prepare the radio resources to exclude all the IEs present in the   */
   /* the IE. Those IEs are filled subsequently                           */
   dcchMsg     = &(recfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   recfgIEs    = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   rrcCfgDed   = &(recfgIEs->radioResourceConfigDedicated);

   wrFillTknU8(&recfgIEs->radioResourceConfigDedicated.pres,PRSNT_NODEF);
   /*Fill UE Configuration*/
   if(ROK != wrUmmBldUeCfgforReCfg(recfgMsg,rrcCfgDed,transCb))
   {
      RETVALUE(RFAILED); 
   }   
   if (ROK != wrUmmRrcDedNASInfo(recfgMsg,recfgIEs,rabRelTrans->nasPdu))
   {
      RETVALUE(RFAILED);
   }   

   if(rabRelTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
   {
      /* SPS changes starts */
      wrUmmRrcFillUeRadioResDedInfo(&recfgMsg->memCp, ueCb, &transCb->spsCfgTransCb.spsCfg, rrcCfgDed,
            (rabRelTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp));

      /* Unset SR Mask if UL SPS is released for the UE by RRM */
      if ((transCb->ueCb->ueCap->eUTRA_Cap.accessStratumRls.val >= NhuAccessStratumRlsrel9Enum) && \
            (transCb->ueCb->isSrMaskEnabled == TRUE) && \
          (rabRelTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.\
                             bRadioResDedBitMap & RM_RCM_UL_SPS_CFG) && \
          (rabRelTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.\
                             stSpsCfg.stSpsConfigUl.bIsSpsEnabled == FALSE))
      {
         rabCount = transCb->ueCb->rbInfo.numOfRbCfgd - transCb->u.rabRelTransCb.numRabs;
         if (rabCount > 0)
         {
            WR_GET_MEM(recfgMsg, (sizeof(NhuDRB_ToAddMod) * rabCount), 
                  &(rrcCfgDed->drb_ToAddModLst.member));
            if (rrcCfgDed->drb_ToAddModLst.member == NULLP)
            {
               WR_FREEEVNT(recfgMsg);
               RETVALUE(RFAILED);
            }

            for (idx = 0; (idx < WR_MAX_DRBS) && (rbIdx < rabCount); idx++)
            {
               isReleasedRab = FALSE;
               for (tcbIdx = 0; tcbIdx < transCb->u.rabRelTransCb.numRabs; tcbIdx++)
               {
                  if (transCb->ueCb->rbInfo.rab[idx].eRABId == transCb->u.rabRelTransCb.rabs[tcbIdx].erabId)
                  {
                     isReleasedRab = TRUE;
                     break;
                  }
               }
               if (isReleasedRab == FALSE)
               {
                  drbIe   = rrcCfgDed->drb_ToAddModLst.member + rbIdx;

                  wrUmmRrcReconfigFillModDrb(transCb->ueCb, transCb->spsCfgTransCb.isUeUlSpsEnabled, drbIe, \
                        (U8)transCb->ueCb->rbInfo.rab[idx].eRABId,
                        transCb->ueCb->rbInfo.rab[idx].rbId ,transCb->ueCb->rbInfo.rab[idx].lchId ,
                        (U8)transCb->ueCb->rbInfo.rab[idx].qci);
                  rbIdx++;
               }
            }
            wrFillTknU16(&rrcCfgDed->drb_ToAddModLst.noComp, rbIdx);
         }
         /* SPS changes ends */
      }
   }    
   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending"
         " RRCConnectionReconfiguration for ERAB Release");
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

/** @brief This function is responsible for filling and sending the RAB Release 
 *         Response message to S1AP.
 *
 * @details
 *
 *     Function: wrUmmRabRelSndS1apRsp
 *
 *         Processing steps:
 *         - invoke wrUmmS1apBldERABRelRsp to fill the response message
 *         - send the message to S1AP
 *         - set the transaction as complete
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmRabRelSndS1apRsp
(
WrUmmTransCb              *transCb
)
{
   WrUmmRabRelTransCb              *rabRelTrans = &transCb->u.rabRelTransCb;
   /*WrUeCb                          *ueCb = transCb->ueCb;
   S1apPdu                         *s1apPdu; */
   U8                              idx;
   WrS1ConCb                       *s1apConCb;
   SztDatEvntReq                   erabRsp;
   WrUmmRabRlsLst                  *rabLst;
   U8                              numrab;
   S16                             ret;
 
   WR_SET_ZERO(&erabRsp, sizeof(SztDatEvntReq));
   s1apConCb = transCb->ueCb->s1ConCb;
   numrab = rabRelTrans->numRabs;
         
   WR_ALLOC(&rabLst, numrab * sizeof(WrUmmRabRlsLst));
   
   if(rabLst == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < numrab; idx++)
   {
      if(rabRelTrans->rabs[idx].rabfailed == FALSE)
      {
         rabLst[idx].rabFailed = FALSE;

      }
      else
      {
         /*Error_Cause:Filling proper cause value */
         rabLst[idx].rabFailed = TRUE;
         rabLst[idx].cause.causeTyp = rabRelTrans->rabs[idx].relCause.causeTyp;
         rabLst[idx].cause.causeVal = rabRelTrans->rabs[idx].relCause.causeVal;
      }  
         rabLst[idx].qci = rabRelTrans->rabs[idx].qci;/*Added for KPI*/
         rabLst[idx].erabId    = rabRelTrans->rabs[idx].erabId;
   }
   /*  Send RAB Release Response over S1 interface to MME        */
   ret =  wrUmmS1apBldERABRelRsp(s1apConCb,&erabRsp.pdu,numrab,rabLst);
   if (ret != ROK)
   {
      /*ccpu00127802*/
      RLOG0(L_ERROR, "ERAB Release MSG building  failed");
   }
   /*Freeing the local structure*/
   WR_FREE(rabLst, numrab * sizeof(WrUmmRabRlsLst));
   erabRsp.spConnId = s1apConCb->spConnId;

   ret = WrIfmS1apDatReq(&erabRsp);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "Sending E-RAB RELEASE RESPONSE Failed");
   }
   else
   {
      RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[S1AP] Sending E-RAB RELEASE "
            "RESPONSE [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
            transCb->ueCb->s1ConCb->mme_ue_s1ap_id,
            transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
   }
   RETVALUE(ROK);
}



PRIVATE Void wrUmmRabRelPrcUpdateDatAppTunnel
(
 WrUmmTransCb                 *transCb
)
{
    U8                  transId = 0;
    U8                  idx;
    WrDamTnlId          *damTnlId; 
    
    if(transCb == NULLP)
    {   
       RETVOID;
    }     

    if (transCb->u.rabRelTransCb.state == WR_UMM_RAB_REL_SCHD_CFG_DONE)
    {
       for (idx = 0; idx < transCb->u.rabRelTransCb.numRabs; idx++)
       {
          if(transCb->u.rabRelTransCb.rabs[idx].rabfailed != TRUE)
          {
             WR_ALLOC_SHAREBLE_BUF(&damTnlId,sizeof(WrDamTnlId));
             if (NULLP == damTnlId)
             {
                RLOG0(L_FATAL, "Memory allocation failed.");
                RETVOID;
             }
             /*Update the Data App Tunnel Information*/
             damTnlId->cellId = transCb->ueCb->cellId;
             damTnlId->crnti  = transCb->ueCb->crnti;
             damTnlId->drbId  = transCb->u.rabRelTransCb.rabs[idx].drbId;
             damTnlId->tnlType  = WR_TNL_NORMAL;
             transId = transId | idx;
             /* EGTPU Release*/
             if (ROK != wrIfmDamTnlDelReq(transId,damTnlId))
             {
                RLOG0(L_ERROR, " wrIfmDamTnlDelReq is failed ");
             }
          }
       }   
    }
    RETVOID;
}    

PRIVATE S16 wrUmmRabRelPrcUmmTimerExp
(
 WrUmmTransCb  *transCb
 )
{
   if(transCb == NULLP)
      RETVALUE(RFAILED);


   switch (transCb->msg->u.tmrExpiry.timerEvnt )
   {
      case WR_TMR_RECONFIGURE: 
         {
            if (transCb->u.rabRelTransCb.rrmUeRecfgRspMsg != NULLP) 
            {
               wrUmmRlsIncMsg(&transCb->u.rabRelTransCb.rrmUeRecfgRspMsg);
            }
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

        RETVALUE(ROK);
}

/** @brief This function is calculates and commits UE the GBR after RAB release
 *
 * @details
 *
 *     Function: wrUmmCommitUeRabRelGbr
 *
 *         Processing steps:
 *         - Calculates GBR for the UE
 *         - Commit in the ueCb
 *
 *
 * @param U32                  transRbInx - RB index in WrUmmRabRelTransCb
 * @param WrUmmRabRelTransCb   *rabRel    - RB release transaction block
 * @param WrUeCb               *ueCb      - UE control block
 * @return Void 
 */
PRIVATE Void wrUmmCommitUeRabRelGbr
(
 U8                  transRbIdx,
 WrUeCb              *ueCb,
 WrUmmRabRelTransCb  *rabRel
)
{
   if (WR_UMM_IS_GBR_BEARER(rabRel->rabs[transRbIdx].qci))
   {
      U32  rbIdx;
      U8   lcgId;

      rbIdx = rabRel->rabs[transRbIdx].erabId - WR_UMM_ERAB_OFFSET;
      lcgId = ueCb->rbInfo.rab[rbIdx].lcgId;
   }
}


/** @brief This function is called when any message related to a particular
 *       E-RAB RELEASE is received. This is the main function that determines 
 *       next course of action based on the message received.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - RRM is invoked based on the information reaceived in ERAB RELEASE.
 *         - Once RRM allows deletion, scheduler is configured with the delete
 *           RBs. 
 *         - Upon receiving, scheduler confirmation, RRC is configured to 
 *           send out RRC Reconfiguration and the lower layer configuration.
 *         - When RRC Reconfiguration complete is received, an ack is sent
 *           to MME.  
 *
 *
 * @param WrUmmTransCb  *transCb  
 * @return ROK 
 */ 

PUBLIC S16 wrUmmRabRelProcessor
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   /*Removed DRX flag*/
   WrUmmRabRelTransCb        *rabRel =  &(transCb->u.rabRelTransCb);
   U8                        idx;
   WrS1ConCb                 *s1apConCb;
   SztDatEvntReq             erabRsp;
   WrUmmRabRlsLst            *rabLst;
   U8                        numrab;
   U8                        rabOff;

   switch(msg->msgTyp)
   {
      case WR_UMM_S1_DAT_IND:
      {
         /* Parse the received RAB Release message and udpate transCb       */
         if (wrUmmParseRabRelMsg(transCb) != ROK)
         {
            wrUmmRabRelSndS1apRsp(transCb);
            wrUmmTransComplete(transCb);
            RETVALUE(ROK);
         }

         /* ccpu00130441 and ccpu00130443: Update the failure cause value for each RABS, if Handover is in progress */
         if((transCb->ueCb->mobCtrlState == WR_HO_INITIATED) || (transCb->ueCb->mobCtrlState == WR_HO_IN_PROGRESS))
         {
            wrUmmHOHandleRabRelease(transCb,transCb->ueCb->hoType);
         }
         
         /* Increment the reference and store the message in the transCb  */
         transCb->msg->refCnt++;
         transCb->u.rabRelTransCb.msg = transCb->msg;

         transCb->u.rabRelTransCb.rrmUeRecfgRspMsg = NULLP;

         /* For now, RRM invocation is being ignored. Proceed with sched  */
         wrUmmPrcRabRel(transCb);
       
         if (transCb->u.rabRelTransCb.numRabsFound == 0)
         {
            /* We just need to send a response and complete the trans     */
            if(RFAILED == wrUmmRabRelSndS1apRsp(transCb))
            {
               RLOG0(L_ERROR, "wrUmmRabRelSndS1apRsp failed.");
               RETVALUE(RFAILED);
            }

            wrUmmTransComplete(transCb);
            RETVALUE(ROK);
         }

         /*Received ERAB - Release Request From MME.
         Update the Statistics to reflect ERAB Release Request Initiated by Enodeb */
         wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,
                         WR_EMM_ERAB_RELEASE_MME,WR_EMM_DFLT_POS_STEP,WR_EMM_ERAB_ATTEMPT);

#if 0 /*Changes for LC*/
         /* Perform configuration towards stack and also dispatch UE   */
         /* reconfiguration message on Uu interface                    */
         wrUmmRabRelPrepRrcMsgAndCfg(transCb);

         /* Configure scheduler for the RABs and AMBR received            */
         wrUmmRabRelCfgSchd(transCb);
#endif
         break;
      }
      case WR_UMM_SCHD_CFG:
      {
         if(rabRel->state == WR_UMM_RAB_REL_SCH_CFG_DRX)
         {
            S16            ret;

            WR_SET_ZERO(&erabRsp, sizeof(SztDatEvntReq));
            s1apConCb = transCb->ueCb->s1ConCb;
            numrab = transCb->u.rabRelTransCb.numRabs;

            WR_ALLOC(&rabLst, numrab * sizeof(WrUmmRabRlsLst));
            if(rabLst == NULLP)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }

            for (idx = 0; idx < numrab; idx++)
            {
               if(transCb->u.rabRelTransCb.rabs[idx].rabfailed != FALSE)
               {
                  rabLst[idx].rabFailed = TRUE;
                  rabLst[idx].cause.causeTyp = transCb->u.rabRelTransCb.rabs[idx].relCause.causeTyp;
                  rabLst[idx].cause.causeVal = transCb->u.rabRelTransCb.rabs[idx].relCause.causeVal;
               }
               else
               {
                  rabLst[idx].rabFailed = FALSE;
               }
               rabLst[idx].erabId   = transCb->u.rabRelTransCb.
                  rabs[idx].erabId;
               rabLst[idx].qci = transCb->u.rabRelTransCb.rabs[idx].qci;/*Added for KPi*/
            }   

            /*  Send RAB Release Response over S1 interface to MME        */
            ret =  wrUmmS1apBldERABRelRsp(s1apConCb,&erabRsp.pdu,numrab,rabLst);
            if (ret != ROK)
            {
               /*ccpu00127802*/
               RLOG0(L_ERROR, "ERAB Release MSG building  failed");
            }

            /*Sent ERAB - Release Response to MME. Update the Statistics to reflect sucessfull ERAB Release Initiated by
              Enodeb */
            wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,
                  WR_EMM_ERAB_RELEASE_MME,WR_EMM_DFLT_POS_STEP,WR_EMM_ERAB_SUCCESS);

            /*Freeing the local structure*/
            WR_FREE(rabLst, numrab * sizeof(WrUmmRabRlsLst));
            erabRsp.spConnId = s1apConCb->spConnId;

            ret = WrIfmS1apDatReq(&erabRsp);
            if (ret != ROK)
            {
               /*ccpu00127802*/
               RLOG0(L_ERROR, "Sending ERAB Release Response failed");
            }
            for (idx = 0; idx < rabRel->numRabs; idx++)
            {
               /*Removing the Rabs from the UeCb*/
               if (wrUmmRelLch(transCb->ueCb, 
                        transCb->u.rabRelTransCb.rabs[idx].lchId) != ROK)
               {
                  /*ccpu00127802*/
                  RLOG0(L_ERROR, "Failed to release LCH for a RAB");
                  break;
               }
               wrUmmCommitUeRabRelGbr(idx, transCb->ueCb, rabRel);

               if (wrUmmRelDrb(transCb->ueCb,
                        transCb->u.rabRelTransCb.rabs[idx].drbId) != ROK)
               {
                  /*ccpu00127802*/
                  RLOG0(L_ERROR, "Failed to release DRB for a RAB");
                  break;
               }
               transCb->ueCb->rbInfo.numOfRbCfgd --;
               rabOff = transCb->u.rabRelTransCb.rabs[idx].erabId - 
                  WR_UMM_ERAB_OFFSET;
               transCb->ueCb->rbInfo.rab[rabOff].inUse = FALSE;

               RLOG_ARG2(L_INFO, DBG_CRNTI,transCb->ueCb->crnti,"RAB Released,"
                  " rabId[%d], lclTunelId[%d]",
                     transCb->u.rabRelTransCb.rabs[idx].erabId, 
                  transCb->u.rabRelTransCb.rabs[idx].drbId);
            }

            /*DRX QCI*/                 
            if((transCb->ueCb->isGBRBearerExists == RAB_RELEASED))
            {
               transCb->ueCb->isGBRBearerExists = NOT_IN_USE;
               RLOG_ARG0(L_INFO,DBG_CRNTI,transCb->ueCb->crnti,"Last GBR Bearer is Released");
               if(wrUmmStartAnr(transCb->ueCb,TRUE) != ROK)
               {
                  RLOG0(L_ERROR,"Failed to Start the ANR");
               }
            }

            if(transCb->u.rabRelTransCb.rrmUeRecfgRspMsg->
                  u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
            {
               wrUmmCommitUeRadioResDedInfo(transCb->ueCb, 
                     &(transCb->u.rabRelTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg));
            }
            /* SPS changes starts */
            if(transCb->u.rabRelTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp->\
                  stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_UL_SPS_CFG)
            {
               transCb->ueCb->spsCfg.spsRnti = transCb->spsCfgTransCb.spsCfg.spsRnti;
               transCb->ueCb->spsCfg.spsCfgUl = transCb->spsCfgTransCb.spsCfg.spsCfgUl;
            }
            if(transCb->u.rabRelTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp->\
                  stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG)
            {
               transCb->ueCb->spsCfg.spsRnti = transCb->spsCfgTransCb.spsCfg.spsRnti;
               transCb->ueCb->spsCfg.spsCfgDl = transCb->spsCfgTransCb.spsCfg.spsCfgDl;
            }
            /* SPS changes ends */

            wrUmmRlsIncMsg(&transCb->u.rabRelTransCb.rrmUeRecfgRspMsg);

            /*Release the transaction CB as the transaction is complete*/
            wrUmmTransComplete(transCb);
         }
         else
         {
            /* We have to wait to receive all the configuration confirms     */
            wrUmmRabRelPrcSchdCfgCfm(transCb);

            /*Update the Data App Tunnel Information*/
            wrUmmRabRelPrcUpdateDatAppTunnel(transCb);
            /*TODO Add the Else Part 
             * Jsut remove the COntext for E_RAB for RElease case */
         }
         break;
      }
      case WR_UMM_RRC_CFG:
      {
         wrUmmRabRelProcRrcCfgCfm(transCb);
         break;
      }
      case WR_UMM_RRM_UERECFG_RESP: 
      {
         transCb->u.rabRelTransCb.rrmUeRecfgRspMsg = transCb->msg;
         transCb->msg->refCnt++;
         /*DRX QCI*/
         if( (transCb->ueCb->isGBRBearerExists == RAB_ESTABLISHED)&&
           (transCb->msg->u.rrmUeRecfgResp->isGBRBearerExists == FALSE))
         {
            transCb->ueCb->isGBRBearerExists = RAB_RELEASED;   
         }

         if(transCb->msg->u.rrmUeRecfgResp->bRecfgBitMap &
               RMU_RECFG_RSP_ERAB_CONFIG)
         {
            /* TBD: Rab Info update is to be added once it is 
             *      RBC is supported in RRM 
             */
         }

         /*LC changes moved from above*/
         /* Perform configuration towards stack and also dispatch UE   */
         /* reconfiguration message on Uu interface                    */
         if(ROK != wrUmmRabRelPrepRrcMsgAndCfg(transCb))
         {
            /* TODO : HARI */
            RLOG_ARG0(L_INFO, DBG_CRNTI,transCb->ueCb->crnti,"Failed to prepare RRC "
               "Reconfig message");

            transCb->errorCause = WR_UMM_CTXT_REL_SW_ERROR;
            transCb->state = WR_UMM_TRANS_RELEASE_CALL; 

            RETVALUE(ROK);
         }

         /* Configure scheduler for the RABs and AMBR received            */
         wrUmmRabRelCfgSchd(transCb);

         break;
      }
      case WR_UMM_TMR_EXPIRY:
      {
        wrUmmRabRelPrcUmmTimerExp(transCb);  
        break;
      }
      case WR_UMM_UU_IND_PDU:
      {
         NhuUlDcchMsg *ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
         NhuUL_DCCH_MsgTypc1 *c1 = &ulDcch->dcchMsg.message.val.c1;
   /*Removed DRX flag*/
         RmuUeRecfgRsp *rrmUeRecfgResp = transCb->u.rabRelTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
         U8             rrcTransId = c1->val.rrcConRecfgnCompl.rrc_TrnsxnId.val;
         S16            ret;

         RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,
               "[OTA] Received RRCConnectionReconfigurationComplete");
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

          /* configuration of the EGTP Tunnels */
            if((rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & 
                   RM_RCM_MAC_MAIN_CFG) &&
               (rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap & 
                   RM_RCM_MAC_MAIN_CFG_DRX))
            {
               RLOG0(L_DEBUG, "Configuring the DRX to SCHD");
               wrUmmRabRelDrxRelSchdUeRecfgReq(transCb); 
            }   
            else
            {
               WR_SET_ZERO(&erabRsp, sizeof(SztDatEvntReq));
               s1apConCb = transCb->ueCb->s1ConCb;
               numrab = transCb->u.rabRelTransCb.numRabs;

               WR_ALLOC(&rabLst, numrab * sizeof(WrUmmRabRlsLst));
               if(rabLst == NULLP)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  RETVALUE(RFAILED);
               }

               for (idx = 0; idx < numrab; idx++)
               {
                  if(transCb->u.rabRelTransCb.rabs[idx].rabfailed != FALSE)
                  {
                     rabLst[idx].rabFailed = TRUE;
                     rabLst[idx].cause.causeTyp = transCb->u.rabRelTransCb.rabs[idx].relCause.causeTyp;
                     rabLst[idx].cause.causeVal = transCb->u.rabRelTransCb.rabs[idx].relCause.causeVal;
                  }
                  else
                  {
                     rabLst[idx].rabFailed = FALSE;
                  }
                  rabLst[idx].erabId   = transCb->u.rabRelTransCb.
                     rabs[idx].erabId;
                  rabLst[idx].qci = transCb->u.rabRelTransCb.rabs[idx].qci;/*Added for KPi*/
               }   

               /*  Send RAB Release Response over S1 interface to MME        */
               ret =  wrUmmS1apBldERABRelRsp(s1apConCb,&erabRsp.pdu,numrab,rabLst);
               if (ret != ROK)
               {
                  /*ccpu00127802*/
                  RLOG0(L_ERROR, "ERAB Release MSG building  failed");
               }

               /*Sent ERAB - Release Response to MME. Update the Statistics to reflect sucessfull ERAB Release Initiated by
                 Enodeb */
               wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,
                     WR_EMM_ERAB_RELEASE_MME,WR_EMM_DFLT_POS_STEP,WR_EMM_ERAB_SUCCESS);

               /*Freeing the local structure*/
               WR_FREE(rabLst, numrab * sizeof(WrUmmRabRlsLst));
               erabRsp.spConnId = s1apConCb->spConnId;

               ret = WrIfmS1apDatReq(&erabRsp);
               if (ret != ROK)
               {
                  /*ccpu00127802*/
                  RLOG0(L_ERROR, "Sending ERAB Release Response failed");
               }
               RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,
                     "[S1AP] Sending E-RAB RELEASE RESPONSE "
                     "[MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
                     transCb->ueCb->s1ConCb->mme_ue_s1ap_id,
                     transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
               for (idx = 0; idx < transCb->u.rabRelTransCb.numRabs; idx++)
               {
                  /*Removing the Rabs from the UeCb*/
                  if (wrUmmRelLch(transCb->ueCb, 
                           transCb->u.rabRelTransCb.rabs[idx].lchId) != ROK)
                  {
                     /*ccpu00127802*/
                     RLOG0(L_ERROR, "Failed to release LCH for a RAB");
                     break;
                  }
                  wrUmmCommitUeRabRelGbr(idx, transCb->ueCb, rabRel);
                  
                  if (wrUmmRelDrb(transCb->ueCb,
                           transCb->u.rabRelTransCb.rabs[idx].drbId) != ROK)
                  {
                     /*ccpu00127802*/
                     RLOG0(L_ERROR, "Failed to release DRB for a RAB");
                     break;
                  }
                  transCb->ueCb->rbInfo.numOfRbCfgd --;
                  rabOff = transCb->u.rabRelTransCb.rabs[idx].erabId - 
                     WR_UMM_ERAB_OFFSET;
                  transCb->ueCb->rbInfo.rab[rabOff].inUse = FALSE;
                  transCb->ueCb->rbInfo.rab[rabOff].isEmergencyBearer = FALSE;
                  transCb->ueCb->ueServTyp = WR_UMM_NORMAL_SERV;
                  
                  RLOG_ARG2(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,
                     "RAB Released, rabId[%d] lclTunelId[%d]",
                        transCb->u.rabRelTransCb.rabs[idx].erabId, 
                     transCb->u.rabRelTransCb.rabs[idx].drbId);
               }

               /* Update the UEs service type based on existing bearer */
               wrUmmUpdtUeServType(transCb->ueCb);

               if(transCb->u.rabRelTransCb.rrmUeRecfgRspMsg->
                     u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
               {
                  wrUmmCommitUeRadioResDedInfo(transCb->ueCb, 
                        &(transCb->u.rabRelTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg));
               }
               /*DRX QCI*/                 
               if((transCb->ueCb->isGBRBearerExists == RAB_RELEASED))
               {
                  transCb->ueCb->isGBRBearerExists = NOT_IN_USE;
                  RLOG0(L_ERROR,"Last GBR Bearer is Released");
                  if(wrUmmStartAnr(transCb->ueCb,TRUE) != ROK)
                  {
                     RLOG0(L_ERROR,"Failed to Start the ANR");
                  }
               }
               /* SPS changes starts */
               if(transCb->u.rabRelTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp->\
                     stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_UL_SPS_CFG)
               {
                  transCb->ueCb->spsCfg.spsRnti = transCb->spsCfgTransCb.spsCfg.spsRnti;
                  transCb->ueCb->spsCfg.spsCfgUl = transCb->spsCfgTransCb.spsCfg.spsCfgUl;
               }
               if(transCb->u.rabRelTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp->\
                     stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG)
               {
                  transCb->ueCb->spsCfg.spsRnti = transCb->spsCfgTransCb.spsCfg.spsRnti;
                  transCb->ueCb->spsCfg.spsCfgDl = transCb->spsCfgTransCb.spsCfg.spsCfgDl;
               }
               /* SPS changes ends */

               wrUmmRlsIncMsg(&transCb->u.rabRelTransCb.rrmUeRecfgRspMsg);

               /*Release the transaction CB as the transaction is complete*/
               wrUmmTransComplete(transCb);
            }
       break;
    }/*End of Case*/
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

PUBLIC S16 wrUmmRabRelTransRel
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabRelTransCb        *rabRel = &transCb->u.rabRelTransCb;

   WR_FREE(rabRel->rabs, rabRel->numRabs * sizeof(*rabRel->rabs));
   wrUmmRlsIncMsg((WrUmmIncMsg **)&rabRel->msg);
   RETVALUE(ROK);
}

/** @brief This function is called when any message related to a RRC Re-Est
 *         transaction during E-RAB Release transaction. 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmDefRabRelIncReestProcessor 
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDefRabRelIncReestProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
   WrUmmRabRelTransCb        *rabRel = &transCb->u.rabRelTransCb;
 
   if (rabRel->state == WR_UMM_RAB_REL_SCHD_CFG_DONE)
   {
      NhuUlCcchMsg               *ulCcchMsg;
      NhuRRCConReestbRqst        *reestab;
      NhuRRCConReestbRqst_r8_IEs *r8_IE;
      U8                        idx;

      ulCcchMsg = &(incTrans->msg->u.uuIndPdu->sdu.m.ulCcchMsg);
      reestab   = &(ulCcchMsg->ccchMsg.message.val.c1.val.rrcConReestbRqst);
      r8_IE     = &(reestab->criticalExtns.val.rrcConReestbRqst_r8);

      /* Stop the RRC Re-Cfg Timer coz of ReCfg failed cause*/
      wrStopTransTmr((WrUmmTransCb *)transCb,WR_TMR_RECONFIGURE);

      if (r8_IE->reestablishmentCause.val == NhuReestbCausereconfigurationFailEnum)
      {
         if (transCb->u.rabRelTransCb.rrmUeRecfgRspMsg != NULLP) 
         {
            wrUmmRlsIncMsg(&transCb->u.rabRelTransCb.rrmUeRecfgRspMsg);
         }

         /* At this point, we return ERAB Release Response with failed list to MME.*/
         /* Continue with the Re-establsihment transaction                       */
         
         for (idx = 0; idx < rabRel->numRabs; idx++)
         {
             if(rabRel->rabs[idx].rabfailed == FALSE)
             { 
               rabRel->rabs[idx].rabfailed = TRUE; 
               rabRel->rabs[idx].relCause.causeTyp = WR_CAUSE_RADIONW;
               rabRel->rabs[idx].relCause.causeVal =  
                      SztCauseRadioNwfailure_in_radio_interface_procedureEnum;
               rabRel->rabs[idx].qci =  transCb->u.rabRelTransCb.rabs[idx].qci;/*Added for KPI*/  
             }
         }   
         /*  Send RAB Release Response over S1 interface to MME  */
         wrUmmRabRelSndS1apRsp(transCb);
         /*Release the transaction CB as the transaction is complete*/
         wrUmmTransComplete(transCb);
     }
     else
     {
         /*TODO Actions due to other cause*/
         RLOG0(L_DEBUG, "Re_establishmenat with other cause during RAB release"
            " received at  Defered processor");
     }
   }
   else
   {
     /*TODO Actions due to Radio Link failure*/
      RLOG0(L_INFO, "Radio Link failure during RAB release received at  "
         "Defered processor");
   }
   RETVALUE(ROK);
}

/** @brief This function is called when any message related to ue context release
 *         transaction during Rab Release transaction. 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmDefRabRelIncCtxtRelProcessor 
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDefRabRelIncCtxtRelProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
  wrUmmStopTransTimers(transCb);
  RETVALUE(ROK);
}



/** @brief This function is called when any message related to a particular
 *         transaction during E-RAB Release transaction. 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmRabRelNewTransProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  inctrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmRabRelNewTransProcessor 
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
              wrUmmDefRabRelIncReestProcessor(transCb,incTrans);
              break;
           } 

        case WR_UMM_CTXT_REL_TRANS:
           {
              wrUmmDefRabRelIncCtxtRelProcessor(transCb,incTrans);
              transCb->state = WR_UMM_TRANS_DONE;
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
        case WR_UMM_RAB_REL_TRANS:
           {
              RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Deferring "
                    "incoming transaction %d ",incTrans->transTyp);
              WR_UMM_CPY_DEFR_MSG_TO_TRANSCB(transCb, incTrans->msg);
              ret = WR_UMM_TRANS_FAIL;
              break;
           }
        default:
           {
              RLOG1(L_ERROR, "Unkonwn transaction [%d] received at  processor",
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
