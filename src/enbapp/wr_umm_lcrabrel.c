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
               

     File:     wr_umm_lcrabrel.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=60;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_umm_msg.h"
#include "wr_umm.h"
#include "wr_umm_lcrabrel.h"
#include "wr_emm.h"
#include "wr_umm_trans.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_sch_msg.h"
#include "wr_ifm_schd.h"
#include "wr_ifm_dam.h"
#include "wr_umm_s1ap_msg.h"
#include "wr_ifm_rrm.h"
#include "wr_ifm_rrc.h"
#include "wr_umm_drx.h"

#ifdef WR_TEST_CODE
EXTERN U8 wrTrgRrcReConTO;
#endif
/** @brief This function is called to handle RRC configuration
 *         confirm. 
 *
 * @details
 * Invoked by - wrUmmLcRabRelProcessor
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
PRIVATE S16 wrUmmLcRabRelProcRrcCfgCfm
(
WrUmmTransCb            *transCb
)
{
   WrUmmLcRabRelTransCb     *rabTrans = &transCb->u.lcRabRelTransCb;
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
PRIVATE S16 wrUmmLcRabRelSchdUeRecfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrUmmLcRabRelTransCb      *rabRel = &transCb->u.lcRabRelTransCb;
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
   transId = transCb->transId | 0x0f;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   if(rabRel->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
   {
      wrUmmSchdFillUeRadioResCfgInfo(ueRecfg,
            (rabRel->rrmUeRecfgRspMsg->u.rrmUeRecfgResp), transCb->ueCb,FALSE);
   }

   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}

/** @brief This function is called to handle logical channel
 *         confirmation from MAC Scheduler. 
 *
 * @details
 * Invoked by - wrUmmLcRabRelProcessor
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
PRIVATE S16 wrUmmLcRabRelSchdLchCfgReq
(
WrUmmTransCb                 *transCb,
U8                           idx
)
{
   WrUmmLcRabRelTransCb      *rabRel = &transCb->u.lcRabRelTransCb;
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
   lchdelCfg->u.lchDel.lcId   = rabRel->rabs[idx].lchId;
   rabRel->rabs[idx].schdCfgState = WR_RAB_REL_SCHD_CFG_SENT;


   /* Dispatch the prepared message to scheduler                    */
   WrIfmSchdCfgUe(cfgReq, transId);

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

PRIVATE S16 wrUmmLcRabRelCfgSchd
(
WrUmmTransCb                 *transCb
)
{
   WrUmmLcRabRelTransCb      *rabRel = &transCb->u.lcRabRelTransCb;
   U32                       idx;

   /* Configure the scheduler for each  RAB delete */
   for (idx = 0; idx < rabRel->numRabs; idx++)
   {
      wrUmmLcRabRelSchdLchCfgReq(transCb, idx);
   }
   
   if(rabRel->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
   {
      wrUmmLcRabRelSchdUeRecfgReq(transCb);
   }

   RETVALUE(ROK);
}

/*Removed DRX flag*/
/** @brief This function is responsible for reconfiguring the UE context
 *         at the scheduler.
 *
 * @details
 *
 *     Function: wrUmmRabLcRelDrxReCfgSchdUeRecfgReq
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
PRIVATE S16 wrUmmRabLcRelDrxReCfgSchdUeRecfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrUmmLcRabRelTransCb      *lcRabRelTransCb = &transCb->u.lcRabRelTransCb;
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   RmuUeRecfgRsp *rrmUeRecfgResp = lcRabRelTransCb->rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
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
   lcRabRelTransCb->state = WR_UMM_RAB_REL_SCHD_CFG_DRX;
   wrUmmSchdFillUeDrxCfgInfo(ueRecfg, (WrDrxCfg *)&rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.stRmDrxCfg);
   
   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
} 
/*Removed DRX flag*/



/** @brief This function parses all ERABs to be deleted from the RAB
 *         RLS EVENT and updates into the tranaction control
 *         block. 
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - It parses RAB Release Event and updates 
 *           all the info into the transaction control block.
 *
 *
 *
 * @param WrUmmTransCb *transCb 
 * @return ROK/RFAILED 
 */ 

PRIVATE S16 wrUmmParseLcRabRelMsg
(
WrUmmTransCb                 *transCb
)
{
   WrUmmRabRls               *lcrabrel = transCb->msg->u.lcrabrls;
   WrUmmLcRabRelTransCb      *lcRabRelTrns = &transCb->u.lcRabRelTransCb;
   WrUeCb                    *ueCb = transCb->ueCb;
   U8                        idx;
   U8                        idx1 = 0;

   for(idx = 0;(idx < WR_MAX_DRBS && idx1 < lcrabrel->numrabs);idx++)
   {
      if(ueCb->rbInfo.rab[idx].eRABId == lcrabrel->erabId[idx1])
      {
         lcRabRelTrns->rabs[idx1].erabId = lcrabrel->erabId[idx1];
         lcRabRelTrns->cause[idx1]       = lcrabrel->cause[idx1];/*RRM_ARP*/
         lcRabRelTrns->causeTyp         = lcrabrel->causeTyp;
         lcRabRelTrns->rabs[idx1].schdCfgState = WR_RAB_REL_SCHD_CFG_IDLE;
         idx1++;
         lcRabRelTrns->numRabs++;
         continue;
      }/*End of IF condition*/   
   }/*End of For Loop*/   
   /*ccpu00124868 Fix: If E-Rab Id received in RaB Release doesn't match with
    * any existing created E-Rab then return RFAILED */
   if(0 == idx1)
   {
      RLOG0(L_ERROR, "E-Rab ID in ERab Release event doesn't match with any "
         "existing E-Rab Id");
     RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
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

PRIVATE S16 wrUmmFindLch
(
WrUeCb                       *ueCb,
WrUmmLcRabRelInfo           *rabrel
)
{
   U32             idx =((rabrel->erabId) - WR_UMM_ERAB_OFFSET);

   if ((idx >= WR_MAX_DRBS) || ((idx < 0)))
   {   
      RETVALUE(RFAILED);
   }   
   rabrel->lchId = ueCb->rbInfo.rab[idx].lchId;
   
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

PRIVATE S16 wrUmmBldUeCfgforReCfgReq
(
NhuDatReqSdus                   *event,
NhuRadioResourceConfigDedicated *radioRes,
WrUmmTransCb                    *transCb
)
{

   WrUmmLcRabRelTransCb            *rabrel = &transCb->u.lcRabRelTransCb;
   U8                              idx;
   NhuUeCfgInfo                    *ueCfg;
   NhuRbRlsCfgList                 *rbCfgList;
   U8                              rrcIdx = 0;
   WrUmmLcRabRelInfo               *rabInfo;
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
   WR_FILL_TKN_UINT(radioRes->drb_ToRlsLst.noComp,rabrel->numRabs);

   /* Allocate memory for each Rab */
   WR_GET_MEM(event, (sizeof(NhuDRB_Identity) * rabrel->numRabs),\
         &(radioRes->drb_ToRlsLst.member));

   for (idx = 0; idx < rabrel->numRabs; idx++)
   {
      rabInfo = &rabrel->rabs[idx];
      drbIe   = radioRes->drb_ToRlsLst.member + idx;
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
   }/*End of idx for loop*/

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
PRIVATE S16 wrUmmLcRabRelPrepRrcMsgAndCfg
(
WrUmmTransCb                 *transCb
)
{
   WrUmmLcRabRelTransCb            *rabRelTrans = &transCb->u.lcRabRelTransCb;
   NhuDatReqSdus                   *recfgMsg;
   WrUeCb                          *ueCb = transCb->ueCb;
   NhuDL_DCCH_Msg                  *dcchMsg;
   NhuRRCConRecfgn                 *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs          *recfgIEs;
   NhuRadioResourceConfigDedicated *rrcCfgDed;

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
                      &rabRelTrans->transId))
   {
      RLOG0(L_ERROR, "RRC Transaction Not available");
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
   if(ROK != wrUmmBldUeCfgforReCfgReq(recfgMsg,rrcCfgDed,transCb))
   {
     RETVALUE(RFAILED);
   }

   if(rabRelTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
   {
      wrUmmRrcFillUePhyCfgDedInfo(rrcCfgDed,
                       (rabRelTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp));
   }

   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending" 
         " RRCConnectionReconfiguration for ERab Release");
   /*Send message towards UE*/
   if (ROK != wrIfmRrcSndReqToUe(recfgMsg))
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe : failed.");
      RETVALUE(RFAILED);
   }

   /* Start RRC Reconfiguration timer */
   wrStartTransTmr(transCb,WR_TMR_RECONFIGURE, wrEmmGetRrcReCfgTmrVal(ueCb->cellId));
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

PRIVATE S16 wrUmmFindDrb
(
WrUeCb                       *ueCb,
WrUmmLcRabRelInfo           *rabrel
)
{

   U32             idx =((rabrel->erabId) - WR_UMM_ERAB_OFFSET);
   
   if ((idx >= WR_MAX_DRBS) || ((idx < 0)))
   {   
      RETVALUE(RFAILED);
   }   
   rabrel->drbId = ueCb->rbInfo.rab[idx].rbId;
   
   RETVALUE(ROK);
}
/** @brief This function processes the Rab Release
 *         transaction to determine the LCHID and DRBID
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
 * @param transCb: Transaction Control Block
 * @return ROK
 */ 

PRIVATE S16 wrUmmPrcLcRabRel
(
WrUmmTransCb                 *transCb
)
{
   WrUmmLcRabRelTransCb      *rabTrans = &transCb->u.lcRabRelTransCb;
   U32                       idx;

   S16                           retVal = ROK;
   RmuUeRecfgReq                 *ueRecfgReq;
   RmuErabRelIe                  *eRabRelPtr;
   
   WR_ALLOC(&ueRecfgReq, sizeof(RmuCommonMsg));
   if (ueRecfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   eRabRelPtr = ueRecfgReq->stErabConfInfo.stErabRelLst;
   ueRecfgReq->stErabConfInfo.usNoErabsToAddMod = 0;
   ueRecfgReq->stErabConfInfo.bNoErabsToRel = 0;

   /* Need to determine LCH Id and RB Id for each RAB to be released         */
   for (idx = 0; idx < rabTrans->numRabs; idx++)
   {
      if (wrUmmFindLch(transCb->ueCb,&rabTrans->rabs[idx]) != ROK)
      {
         /*ccpu00127802*/
         RLOG0(L_ERROR, "Failed to find LCH for a RAB");
         break;
      }
      if (wrUmmFindDrb(transCb->ueCb,&rabTrans->rabs[idx]) != ROK)
      {
         /*ccpu00127802*/
         RLOG0(L_ERROR, "Failed to find DRB for a RAB");
         break;
      }

      eRabRelPtr->bErabId                        = rabTrans->rabs[idx].erabId;
      ueRecfgReq->stErabConfInfo.bNoErabsToRel++;
      /* to support multiple eRabs */
      eRabRelPtr++;
   }

   if(ueRecfgReq->stErabConfInfo.bNoErabsToRel)
   {
      ueRecfgReq->bCellId = transCb->ueCb->cellId;
      ueRecfgReq->usCrnti = transCb->ueCb->crnti;
      ueRecfgReq->uiTransId = transCb->transId;
      ueRecfgReq->bRecfgBitMap = RMU_RECFG_REQ_ERAB_CONFIG;
      ueRecfgReq->stErabConfInfo.enErabAction = RMU_DEL_ERAB;/*RRM_ARP*/

      retVal = wrIfmRrmSndUeRecfgReq(ueRecfgReq);
   }
   else
   {
         WR_FREE(ueRecfgReq, sizeof(RmuCommonMsg));
#if 1 /*changes for LC*/
         /* Perform configuration towards stack and also dispatch UE   */
         /* reconfiguration message on Uu interface                    */
         if(ROK != wrUmmLcRabRelPrepRrcMsgAndCfg(transCb))
         {
            /* TODO : HARI */
            RLOG0(L_ERROR, "Failed to find RRC Trans ID");
            RETVALUE(RFAILED);
         }


         /* Configure scheduler for the RABs and AMBR received            */
         wrUmmLcRabRelCfgSchd(transCb);
#endif
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
PRIVATE S16 wrUmmLcRabRelPrcSchdCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmLcRabRelTransCb      *rabRelTrans = &transCb->u.lcRabRelTransCb;
   U8                        idx;
    
   idx = transCb->msg->u.schdCfm.transId & 0x0f;
   if (idx > 15)
   {
      /*ccpu00127802*/
      RLOG_ARG0(L_ERROR, DBG_TRNSID, transCb->transId,"invalid transaction id "
         "for RAB Release");
      RETVALUE(ROK);
   }
   if (idx >= WR_MAX_DRBS)
   {
      /*ccpu00127802*/
      RLOG_ARG0(L_ERROR, DBG_TRNSID, transCb->transId,"invalid transaction id "
         "for RAB Release");
      RETVALUE(ROK);
   }
   if (idx > rabRelTrans->numRabs)
   {
      /*ccpu00127802*/
      RLOG_ARG0(L_ERROR, DBG_TRNSID, transCb->transId,"invalid transaction id "
         "for RAB Release");
      RETVALUE(ROK);
   }
   if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
   {
      rabRelTrans->rabs[idx].schdCfgState = WR_RAB_REL_SCHD_CFM_SUCC;
   }
   else
   {
      rabRelTrans->rabs[idx].schdCfgState = WR_RAB_REL_SCHD_CFM_FAIL;
   }
   for (idx = 0; idx < rabRelTrans->numRabs; idx++)
   {
      if (rabRelTrans->rabs[idx].schdCfgState ==  WR_RAB_REL_SCHD_CFG_SENT)
      {
         RETVALUE(ROK);
      }
   }
   rabRelTrans->state = WR_UMM_RAB_REL_SCHD_CFG_DONE;
   RETVALUE(ROK);
}

PRIVATE S16 wrProcUmmTimerExp(WrUmmTransCb  *transCb)
{
  if( transCb == NULLP)
  {
        RETVALUE(RFAILED);  
  }             

  switch (transCb->msg->u.tmrExpiry.timerEvnt )
  {
    case WR_TMR_RECONFIGURE:
    {
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

/** @brief This function is called when any message related to a particular
 *       Local RAB RELEASE Event is received. This is the main function that 
 *       determines next course of action based on the message received.
 *
 * @details
 *
 *     Function: 
 *
 *         Processing steps:
 *         - RRM is invoked based on ERAB RELEASE IND with list of  ERAB IDs 
 *         - Scheduler is configured with the delete
 *           RBs. 
 *         - Upon receiving, scheduler confirmation, RRC is configured to 
 *           send out RRC Reconfiguration and the lower layer configuration.
 *         - When RRC Reconfiguration complete is received, RAB RELEASE
 *           INDICATION is sent to MME.  
 *
 *
 * @param WrUmmTransCb  *transCb  
 * @return ROK 
 */ 
PUBLIC S16 wrUmmLcRabRelProcessor
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   U8                        idx;
   WrS1ConCb                 *s1apConCb;
   SztDatEvntReq             erabrlsInd;
   WrUmmRabRlsLst            *rabLst;
   U8                        numrab;
   U8                        rabOff;
   WrDamTnlId                *damTnlId = NULL;


   switch(msg->msgTyp)
   {
      case WR_UMM_RAB_RLS_EVNT:
      {
         /*ccpu00124868 Fix: Storing received message before processing*/
         /* Increment the reference and store the message in the transCb  */
         transCb->msg->refCnt++;
         transCb->u.lcRabRelTransCb.msg = transCb->msg;

         /* Parse the received RAB Release message and udpate transCb       */
         if (wrUmmParseLcRabRelMsg(transCb) != ROK)
         {
            wrUmmTransComplete(transCb);
            RETVALUE(ROK);
         }


         /* For now, RRM invocation is being ignored. Proceed with sched  */
         if (wrUmmPrcLcRabRel(transCb) != ROK)
         {
            /* TODO: HARI */
            RLOG_ARG0(L_ERROR, DBG_CRNTI,transCb->ueCb->crnti,
               "Failed to prepare RRC Reconfig message");
            /* Sending UE context Release Request to MME */
            transCb->errorCause = WR_UMM_CTXT_REL_SW_ERROR;
            transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
            RETVALUE(ROK);
         }
         /* Sending out ERAB RELEASE INDICATION to MME first so that the context
          * at Core Network is released before eNB deletes tunnels. This avoids
          * EGTP error indication being sent to SGW during DL data on this
          * bearer.  This issue was observed on INTEL.*/ 
            WR_SET_ZERO(&erabrlsInd, sizeof(SztDatEvntReq));
            S16   ret;
            s1apConCb = transCb->ueCb->s1ConCb;
            numrab = transCb->u.lcRabRelTransCb.numRabs;

            WR_ALLOC(&rabLst, numrab * sizeof(WrUmmRabRlsLst));
            if (NULLP == rabLst)
            {
               RLOG0(L_FATAL, "Failed to allocate memory for rabLst.");
               RETVALUE(RFAILED);
            } 
            for (idx = 0; idx < numrab; idx++)
            {
               rabLst[idx].erabId = transCb->u.lcRabRelTransCb.
                  rabs[idx].erabId;

               rabLst[idx].cause.causeTyp = transCb->u.lcRabRelTransCb.causeTyp; 
               rabLst[idx].cause.causeVal = transCb->u.lcRabRelTransCb.
                  cause[idx];/*RRM_ARP*/
               wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_RELEASE_ENB,WR_EMM_DFLT_POS_STEP,WR_EMM_ERAB_FAIL_RADIORESOURCESNOTAVAILABLE);
            }   
            /*  Send RAB Release Indication over S1 interface to MME        */
            ret =  wrUmmS1apBldERABRelInd(s1apConCb,&erabrlsInd.pdu,numrab,rabLst); /* kw fix */
            if (ret != ROK)
            {
               /*ccpu00127802*/
               RLOG0(L_ERROR, "ERAB Release Ind MSG building  failed");
            }

            /*Freeing the local structure*/
            WR_FREE(rabLst, numrab * sizeof(WrUmmRabRlsLst));
            erabrlsInd.spConnId = s1apConCb->spConnId;
            ret = WrIfmS1apDatReq(&erabrlsInd);
            if (ret != ROK)
            {
               /*ccpu00127802*/
               RLOG0(L_ERROR, "Sending ERAB Release Response failed");
            }
            wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_RELEASE_ENB,numrab,WR_EMM_ERAB_SUCCESS);

#if 0  /*Changes for LC*/
         /* Perform configuration towards stack and also dispatch UE   */
         /* reconfiguration message on Uu interface                    */
         wrUmmLcRabRelPrepRrcMsgAndCfg(transCb);


         /* Configure scheduler for the RABs and AMBR received            */
         wrUmmLcRabRelCfgSchd(transCb);
#endif 
         break;
      }
      case WR_UMM_RRC_CFG:
      {
         wrUmmLcRabRelProcRrcCfgCfm(transCb);
      break;
      }
      case WR_UMM_SCHD_CFG:

         if(transCb->u.lcRabRelTransCb.state == WR_UMM_RAB_REL_SCHD_CFG_DRX)
         {
#if 0 /*original*/
            S16            ret;
            WR_SET_ZERO(&erabrlsInd, sizeof(SztDatEvntReq));
            s1apConCb = transCb->ueCb->s1ConCb;
            numrab = transCb->u.lcRabRelTransCb.numRabs;

            WR_ALLOC(&rabLst, numrab * sizeof(WrUmmRabRlsLst));
            if (NULLP == rabLst)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            } 
            for (idx = 0; idx < numrab; idx++)
            {
               rabLst[idx].erabId = transCb->u.lcRabRelTransCb.
                  rabs[idx].erabId;

               rabLst[idx].cause.causeTyp = transCb->u.lcRabRelTransCb.causeTyp; 
               rabLst[idx].cause.causeVal = transCb->u.lcRabRelTransCb.
                  cause[idx];/*RRM_ARP*/
               wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_RELEASE_ENB,WR_EMM_DFLT_POS_STEP,WR_EMM_ERAB_FAIL_RADIORESOURCESNOTAVAILABLE);
            }   
            /*  Send RAB Release Indication over S1 interface to MME        */
            ret =  wrUmmS1apBldERABRelInd(s1apConCb,&erabrlsInd.pdu,numrab,rabLst); /* kw fix */
            if (ret != ROK)
            {
               /*ccpu00127802*/
               RLOG0(L_ERROR, "ERAB Release Ind MSG building  failed");
            }

            /*Freeing the local structure*/
            WR_FREE(rabLst, numrab * sizeof(WrUmmRabRlsLst));
            erabrlsInd.spConnId = s1apConCb->spConnId;
            ret = WrIfmS1apDatReq(&erabrlsInd);
            if (ret != ROK)
            {
               /*ccpu00127802*/
               RLOG0(L_ERROR, "Sending ERAB Release Response failed");
            }
            wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_RELEASE_ENB,numrab,WR_EMM_ERAB_SUCCESS);
#endif /*0*/
            for (idx = 0; idx < transCb->u.lcRabRelTransCb.numRabs; idx++)
            {
               /*Removing the Rabs from the UeCb*/
               if (wrUmmRelLch(transCb->ueCb, 
                        transCb->u.lcRabRelTransCb.rabs[idx].lchId) != ROK)
               {
                  /*ccpu00127802*/
                  RLOG0(L_ERROR, "Failed to release LCH for a RAB");
                  break;
               }
               if (wrUmmRelDrb(transCb->ueCb,
                        transCb->u.lcRabRelTransCb.rabs[idx].drbId) != ROK)
               {
                  /*ccpu00127802*/
                  RLOG0(L_ERROR, "Failed to release DRB for a RAB");
                  break;
               }
               transCb->ueCb->rbInfo.numOfRbCfgd--;
               rabOff = transCb->u.lcRabRelTransCb.rabs[idx].erabId - 
                  WR_UMM_ERAB_OFFSET;
               transCb->ueCb->rbInfo.rab[rabOff].inUse = FALSE;

            }
            /*DRX QCI*/                 
            if((transCb->ueCb->isGBRBearerExists == RAB_RELEASED))
            {
               transCb->ueCb->isGBRBearerExists = NOT_IN_USE;
               RLOG0(L_INFO,"Last GBR Bearer is Released");
               if(wrUmmStartAnr(transCb->ueCb,TRUE) != ROK)
               {
                  RLOG0(L_ERROR,"Failed to Start the ANR");
               }
            }
            if(transCb->u.lcRabRelTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
            {
               wrUmmCommitUeRadioResDedInfo(transCb->ueCb, 
                     &(transCb->u.lcRabRelTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg));
            }
            /*Release the transaction CB as the transaction is complete*/
            wrUmmTransComplete(transCb);
         }
         else
         {
            U8                  transId = 0;
            //WrDamTnlId          damTnlId;
       
            /* We have to wait to receive all the configuration confirms     */
            wrUmmLcRabRelPrcSchdCfgCfm(transCb);
            if (transCb->u.lcRabRelTransCb.state == WR_UMM_RAB_REL_SCHD_CFG_DONE)
            {
               for (idx = 0; idx < transCb->u.lcRabRelTransCb.numRabs; idx++)
               {
                  /* This is converted to pointer as part of DAM Separation Changes */
                  WR_ALLOC_SHAREBLE_BUF(&damTnlId,sizeof(WrDamTnlId));
                  if (NULLP == damTnlId)
                  {
                     RLOG0(L_FATAL, "Memory allocation failed.");
                     RETVALUE(RFAILED);
                  }
                  damTnlId->cellId = transCb->ueCb->cellId;
                  damTnlId->crnti  = transCb->ueCb->crnti;
                  /* Fix for ccpu00123253 */
                  damTnlId->tnlType = WR_TNL_NORMAL;
                  damTnlId->drbId  = transCb->u.lcRabRelTransCb.rabs[idx].drbId;
                  transId = transId | idx;
                  //if (ROK != wrDamTnlDelReq(transId,&damTnlId))
                  if (ROK != wrIfmDamTnlDelReq(transId,damTnlId))
                  {
                     /*ccpu00127802*/
                     RLOG0(L_ERROR, "wrDamTnlDelReq is failed");
                  }
               }   
            }
         }
         break;
      case WR_UMM_RRM_UERECFG_RESP: 
      {
         transCb->u.lcRabRelTransCb.rrmUeRecfgRspMsg = transCb->msg;
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
#if 1   /*Changes for LC*/
         /* Perform configuration towards stack and also dispatch UE   */
         /* reconfiguration message on Uu interface                    */
         if (wrUmmLcRabRelPrepRrcMsgAndCfg(transCb) != ROK)
         {
            /* TODO : HARI */
            RLOG_ARG0(L_ERROR, DBG_CRNTI, transCb->ueCb->crnti,"Failed to "
               "prepare RRC Reconfig message");
            /* Sending UE context Release Request to MME */
            transCb->errorCause = WR_UMM_CTXT_REL_SW_ERROR;
            transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
            RETVALUE(ROK);
         }

         /* Configure scheduler for the RABs and AMBR received            */
         wrUmmLcRabRelCfgSchd(transCb);
#endif
         break;
      }
      case WR_UMM_TMR_EXPIRY:
      {
        wrProcUmmTimerExp(transCb);    
        break;
      }
      case WR_UMM_UU_IND_PDU:
      {
         NhuUlDcchMsg *ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
         NhuUL_DCCH_MsgTypc1 *c1 = &ulDcch->dcchMsg.message.val.c1;
/*Removed DRX flag*/
         RmuUeRecfgRsp *rrmUeRecfgResp = transCb->u.lcRabRelTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
         U8             rrcTransId = c1->val.rrcConRecfgnCompl.rrc_TrnsxnId.val;
         S16            ret;
#ifdef WR_TEST_CODE
         if ( wrTrgRrcReConTO == TRUE )
         {
            RLOG0(L_WARNING, "Dropping RRCCONRECFGNCOMPL!!");
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
            wrUmmRabLcRelDrxReCfgSchdUeRecfgReq(transCb); 
         }   
         else
         {
            for (idx = 0; idx < transCb->u.lcRabRelTransCb.numRabs; idx++)
            {
               /*Removing the Rabs from the UeCb*/
               if (wrUmmRelLch(transCb->ueCb, 
                        transCb->u.lcRabRelTransCb.rabs[idx].lchId) != ROK)
               {
                  /*ccpu00127802*/
                  RLOG0(L_ERROR, "Failed to release LCH for a RAB");
                  break;
               }
               if (wrUmmRelDrb(transCb->ueCb,
                        transCb->u.lcRabRelTransCb.rabs[idx].drbId) != ROK)
               {
                  /*ccpu00127802*/
                  RLOG0(L_ERROR, "Failed to release DRB for a RAB");
                  break;
               }
               transCb->ueCb->rbInfo.numOfRbCfgd--;
               rabOff = transCb->u.lcRabRelTransCb.rabs[idx].erabId - 
                  WR_UMM_ERAB_OFFSET;
               transCb->ueCb->rbInfo.rab[rabOff].inUse = FALSE;

            }
            /*DRX QCI*/                 
            if((transCb->ueCb->isGBRBearerExists == RAB_RELEASED))
            {
               transCb->ueCb->isGBRBearerExists = NOT_IN_USE;
               RLOG0(L_INFO,"Last GBR Bearer is Released");
               if(wrUmmStartAnr(transCb->ueCb,TRUE) != ROK)
               {
                  RLOG0(L_ERROR,"Failed to Start the ANR");
               }
            }
            if(transCb->u.lcRabRelTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
            {
               wrUmmCommitUeRadioResDedInfo(transCb->ueCb, 
                     &(transCb->u.lcRabRelTransCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg));
            }
            /*Release the transaction CB as the transaction is complete*/
            wrUmmTransComplete(transCb);
         }
       break;
      }
   }/*End of Switch*/
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
PUBLIC S16 wrUmmLcRabRelTransRel
(
WrUmmTransCb                 *transCb
)
{
   WrUmmLcRabRelTransCb        *rabRel = &transCb->u.lcRabRelTransCb;

   wrUmmRlsIncMsg((WrUmmIncMsg **)&rabRel->msg);
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
