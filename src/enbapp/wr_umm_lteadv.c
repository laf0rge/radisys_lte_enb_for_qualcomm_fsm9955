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
     Name:     Lte Advanced Module

     Type:     C source file

     Desc:     Data structures  

     File:     wr_umm_lteadv.c

     Sid:      fc_umm_lteadv.c@@/main/TeNB_Main_BR/4 - Mon Aug 11 16:52:51 2014

     Prg:      

*********************************************************************21*/

#ifdef __cplusplus
extern "C" {
#endif

static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_MODULE_ID=32;
static int RLOG_FILE_ID=121;

/* LTE_ADV starts */

#include "wr.h"
#include "wr_utils.h"
#include "wr_umm.h"
#include "wr_emm.h"
#include "wr_cmn.h"
#include "wr_umm_msg.h"
#include "wr_umm_lteadv.h"
#include "wr_umm_trans.h"
#include "wr_umm_meas_fun.h"
#include "wr_umm_rrc_msg.h"
#include "wr_ifm_rrc.h"
#include "wr_ifm_schd.h"

/** @brief This function triggers schedular reconfig for 
 *         all ues currently connected to enb
 *        
 * @details
 *
 * Function: wrUmmHdlLteaRecfgForAllUe
 *
 *   Processing steps:
 *   -
 * 
 * @param[in]  cellCb: pointer to cell control block.
 * @param[in]  status: Indicates whether feature is enabled or disabled.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmHdlLteaRecfgForAllUe
(
WrCellCb                     *cellCb,
RgrFeatureStatus             status,
RgrFeature                   feature
)
{
   U16                       ueCnt         = 0;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *ummCellCb;

   TRC2(wrUmmHdlLteaRecfgForAllUe)

   WR_UMM_GET_CELLCB(ummCellCb, cellCb->cellId);
   if(ummCellCb == NULLP)        
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", (U16)cellCb->cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                
   while(ueCnt < ummCellCb->maxUesPerCell)
   {
      ueCb = ummCellCb->ueCbs[ueCnt++];
      if(ueCb == NULLP)
      {
         continue;
      }

      switch (feature)
      {
         case RGR_ABS:
         {
            if((WR_LTE_ADV_PICO_ENB == cellCb->lteAdvCb.lteAdvCfg.enbType) && 
                                    (TRUE == ueCb->lteAdvCb.isUeCellEdge))
            {
               /* Configure UE for ABS measurement report */
               wrUmmHdlLteAdvRecfgForUe(ummCellCb, ueCb, status, 
                     WR_UMM_LTE_ADV_ABS_PROC,WR_UMM_LTE_ADV_PEER_UE);
               if (status == RGR_DISABLE)
               {
                  ueCb->lteAdvCb.absInfo.isAbsUe = FALSE;
               }
            }
            break;
         }
         case RGR_SFR:
         {
            wrUmmHdlLteAdvRecfgForUe(ummCellCb, ueCb, status, 
                           WR_UMM_LTE_ADV_SFR_PROC, WR_UMM_LTE_ADV_LCL_UE);
            break;
         }
         case RGR_RE:
         {
            if((WR_LTE_ADV_PICO_ENB == cellCb->lteAdvCb.lteAdvCfg.enbType)||
               ((WR_LTE_ADV_MACRO_ENB == cellCb->lteAdvCb.lteAdvCfg.enbType) && 
                (TRUE == ueCb->lteAdvCb.isUeCellEdge)))
            {
               wrUmmHdlLteAdvRecfgForUe(ummCellCb, ueCb, status, 
                             WR_UMM_LTE_ADV_RE_PROC, WR_UMM_LTE_ADV_LCL_UE);
            }
            break;
         } 
         default:
         {
            RLOG0(L_ERROR, " Request for unsupported lte adv feature rcvd ");
         }
      }
   }

   RETVALUE(ROK);
}

/** @brief This function triggers appropriate handler depending 
 *         modified feature
 *        
 * @details
 *
 * Function: wrUmmHdlLteAdvRecfg
 *
 *   Processing steps:
 *   -
 * 
 * @param[in]  cellCb: pointer to cell control block.
 * @param[in]  feature: Feature which is modified.
 * @param[in]  status: Indicates whether feature is enabled or disabled.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16  wrUmmHdlLteAdvRecfg
(
U32                          cellId,
RgrFeature                   feature,
RgrFeatureStatus             status 
)
{
   WrCellCb                  *cellCb;
   TRC2(wrUmmHdlLteAdvRecfg)

   WR_GET_CELLCB(cellCb, cellId);
   if(cellCb == NULLP)        
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", (U16)cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                    
   switch(feature)
   {
      case RGR_RE:
      {
         RLOG1(L_DEBUG, " Request range extention feature modification rcvd. status: %d ", status);
         /* Reconfigure the UEs measurement configuration */
         wrUmmHdlLteaRecfgForAllUe(cellCb, status, feature);
         break;
      }
      case RGR_ABS:
      {
         RLOG1(L_DEBUG, " Request for ABS feature modification rcvd. status: %d ", status);
         /* Reconfigure schedular to configure UE as ABS/Non-ABS UE */
         wrUmmHdlLteaRecfgForAllUe(cellCb, status, feature);
         break;
      }
     case RGR_SFR:
     {
         RLOG1(L_DEBUG, " Request for SFR feature modification rcvd. status: %d ", status);
         /* Reconfigure schedular to configure UE as CE/CC */
         wrUmmHdlLteaRecfgForAllUe(cellCb, status, feature);
         break;
      }
      default:
      {
         RLOG0(L_ERROR, " Request for unsupported lte adv feature rcvd ");
      }
   }
   RETVALUE(ROK);
}
/** @brief This function triggers schedular  reconfig for 
 *         a UE by posting internal message
 *        
 * @details
 *
 * Function: wrUmmHdlLteAdvRecfgForUe
 *
 *   Processing steps:
 *   -
 * 
 * @param[in]  cellCb: pointer to cell control block.
 * @param[in]  ueCb: pointer to ue control block.
 * @param[in]  status: Indicates whether feature is enabled or disabled.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmHdlLteAdvRecfgForUe
(
WrUmmCellCb               *cellCb,
WrUeCb                    *ueCb,
RgrFeatureStatus          status,
WrUmmLteAdvProc           procType,
WrUmmLteAdvPeerType       peerType
)
{
   WrUmmIntLteAdvMsg *lteAdvIntMsg = NULLP;
   WrUmmInternalMsg  *internalMsg  = NULLP;
   WrUmmIncMsg       *msg          = NULLP;    

   TRC2(wrUmmHdlLteAdvRecfgForUe)

   /* Constuct the internal message and post it*/
   WR_ALLOC(&lteAdvIntMsg,sizeof(WrUmmIntLteAdvMsg));
   if (NULLP == lteAdvIntMsg)
   {
      RLOG0(L_FATAL, "Memory allocation failed. ");
      RETVALUE(NULLP);
   }

   if (ROK != wrUmmBldInternalMsg(&internalMsg,
             WR_UMM_INTMSG_LTE_ADV_TRANS, lteAdvIntMsg))
   {
      RLOG0(L_ERROR, " creation of internal message Failed ");
      WR_FREE(lteAdvIntMsg,sizeof(WrUmmIntLteAdvMsg));
      RETVALUE(RFAILED);
   }

   lteAdvIntMsg->procType  = procType;
   lteAdvIntMsg->peerType  = peerType;
   lteAdvIntMsg->isEnabled = (Bool)((status == RGR_ENABLE)? TRUE:FALSE); /* KW fix for LTE_ADV */

   /* ueCb->lteAdvCb.absInfo.isAbsUe = (Bool)((status == RGR_ENABLE)? TRUE:FALSE); */

   WR_ALLOC(&msg,sizeof(WrUmmIncMsg));
   if (NULLP == msg)
   {
      RLOG0(L_FATAL, " Memory allocation failed. ");
      WR_FREE(lteAdvIntMsg,sizeof(WrUmmIntLteAdvMsg));
      WR_FREE(internalMsg, sizeof(WrUmmInternalMsg));
      RETVALUE(RFAILED);
   }

   msg->msgTyp = WR_UMM_INTERNAL_MSG;
   msg->u.internalMsg = internalMsg;
   msg->refCnt = 1;

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);
   RETVALUE(ROK);
}

/** @brief This function is used to send ue reconfiguratin request to scheduler 
 *
 * @details
 *
 *     Function:wrUmmRrcReEstabSchdUeRecfgReq
 *
 *         Processing steps:
 *          - Fill UE Reconfiguration structure for Scheduler 
 *          - Send Reconfiguration request using WrIfmSchdCfgUe 
 *
 * @param[in] transCb: Transaction CB 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PRIVATE S16 wrUmmLteAdvSchdUeRecfgReq
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
      RETVALUE(RFAILED);
   }

   /* Set last 4 bits as 1 for UE reconfiguration */
   transId                       = transCb->transId | 0x0000000f;
   ueRecfg                       = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action                = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId               = transCb->ueCb->cellId;
   ueRecfg->oldCrnti             = transCb->ueCb->crnti;
   ueRecfg->newCrnti             = transCb->ueCb->crnti;
   ueRecfg->ueRecfgTypes         = RGR_UE_LTEA_RECFG;  /*KW fix for LTE_ADV */

   if(WR_UMM_LTE_ADV_SFR_PROC == transCb->u.lteAdvTransCb.procType)
   {
      ueRecfg->ueLteAdvCfg.pres |= RGR_SFR;
      ueRecfg->ueLteAdvCfg.isUeCellEdge = transCb->ueCb->lteAdvCb.isUeCellEdge;
   }
/* LTE_ADV_ABS starts */
   if(WR_UMM_LTE_ADV_ABS_PROC == transCb->u.lteAdvTransCb.procType)
   {
      ueRecfg->ueLteAdvCfg.pres |= RGR_ABS;
      ueRecfg->ueLteAdvCfg.isAbsUe      = transCb->msg->u.internalMsg->u.lteAdvMsg->isEnabled;
   }
/* LTE_ADV_ABS ends */

   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}
/** @brief This function is responsible for setting A3 measurement for UE
 * 
 * @details
 *
 * Function:wrUmmIntraMeasCfgForLteAdvAddEvntA3
 *
 *   Processing steps:
 *    -
 *     
 * @param[in]  measUeCb: pointer to ue meas control block 
 * @param[out]  addMeasInfo: pointer to  meas add info
 * @param[in]  cellCb: pointer to umm cell control block
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmIntraMeasCfgForLteAdvAddEvntA3
(
WrUeCb                      *ueCb,
WrUmmMeasAddInfo            *addMeasInfo,
WrUmmCellCb                 *cellCb,
WrMeasRptCfgType            rptCfgTyp  
)
{
   U8                        measId;
   U8                        measObjId;
   U8                        rptCfgId;
   LwrCellMeasCfgGrp         *measCellCfg;
   WrUmmMeasRptCfgInfo       rptCfgInfo;
   /*rel10*/
   WrMeasUeCb                *measUeCb;

   TRC2(wrUmmIntraMeasCfgForLteAdvAddEvntA3)

   measUeCb = ueCb->measCfg;
   measObjId = measUeCb->intraMeasObj.measObjId;
   measCellCfg = cellCb->measCellCfg;

   /* Report Config for Event A3 */
   rptCfgInfo.rptCfgTyp = rptCfgTyp;
   rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

   if(ROK !=  wrUmmMeasFindRptCfgID(&rptCfgId, &rptCfgInfo, &measUeCb->rptCfgLst))
   {
      rptCfgId = wrUmmMeasCreatRptCfgObjInUeCb(&rptCfgInfo, ueCb, measCellCfg);
      addMeasInfo->rptCfgLst[addMeasInfo->rptCfgIdCnt] = rptCfgId;
      addMeasInfo->rptCfgIdCnt++;
   }
   /* Meas ID for Event A3 */
   if(WR_UMM_MEAS_EVENT_A3 == rptCfgTyp)
   {
      measId = wrUmmMeasCreatMeasIdNode(WR_UMM_MEAS_TYPE_HO, measObjId, rptCfgId, measUeCb);
   }
/* LTE_ADV_ABS starts */
   else
   {
      measId = wrUmmMeasCreatMeasIdNode(WR_UMM_MEAS_TYPE_ABS, measObjId, rptCfgId, measUeCb);
   }
/* LTE_ADV_ABS ends */
   addMeasInfo->measIdLst[addMeasInfo->measIdCnt] = measId;
   addMeasInfo->measIdCnt++;
   RETVALUE(ROK);
} /* wrUmmIntraMeasCfgForLteAdvAddEvntA3 */
/** @brief This function is responsible for remiving A3 measurement for UE
 * 
 * @details                  
 *
 * Function:wrUmmIntraMeasCfgForLteAdvAddEvntA3
 *
 *   Processing steps:
 *    -
 *     
 * @param[in]  measUeCb: pointer to ue meas control block 
 * @param[out]  rmvMeasInfo: pointer to  meas remove info
 * @param[in]  cellCb: pointer to umm cell control block
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmIntraMeasCfgForLteAdvDelEvntA3
(
WrMeasUeCb                   *measUeCb,
WrUmmMeasRmvInfo             *rmvMeasInfo,
WrUmmCellCb                  *cellCb,
WrMeasRptCfgType             rptCfgTyp
)
{
   U8                        measId;
   U8                        a3RptCfgId;
   U8                        rptCfgId;
   U8                        *measIdRmvLst;
   WrMeasIdNode              *measIdNode;
   CmLList                   *measIdLnk;
   WrUmmMeasRptCfgInfo       rptCfgInfo;

   TRC2(wrUmmIntraMeasCfgForLteAdvDelEvntA3)
   measIdRmvLst = rmvMeasInfo->measIdRmvLst;
   rptCfgInfo.rptCfgTyp = rptCfgTyp;
   rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

 
   wrUmmMeasFindRptCfgID(&a3RptCfgId, &rptCfgInfo, &measUeCb->rptCfgLst);

   measIdLnk = measUeCb->measIdLst.first;
   while(NULLP != measIdLnk)
   {
      measIdNode = (WrMeasIdNode*)measIdLnk->node;
      measId     = measIdNode->measID;
      rptCfgId   = measIdNode->rptCfgID;
      
      /* When UE comes back to cell center remove A3 event also */
      if(rptCfgId == a3RptCfgId)
      {
         measIdRmvLst[(rmvMeasInfo->measIdRmvLstCnt)++] = measId;
      }
      measIdLnk = measIdLnk->next;
   }
   RETVALUE(ROK);
} /* wrUmmIntraMeasCfgForLteAdvDelEvntA3 */
/** @brief This function is responsible for modifying cell specific offset
 * 
 * @details                  
 *
 * Function:wrUmmIntraMeasCfgForRangeExtModCellSpfcOffSet
 *
 *   Processing steps:
 *    -
 *     
 * @param[in]  measUeCb: pointer to ue meas control block 
 * @param[out] addMeasInfo: pointer to  meas add info
 * @param[in]  cellCb: pointer to umm cell control block
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmIntraMeasCfgForRangeExtModCellSpfcOffSet
(
 WrMeasUeCb                  *measUeCb,
 WrUmmMeasAddInfo            *addMeasInfo,
 WrUmmCellCb                 *cellCb
)
{
   U8                        measObjId;
   TRC2(wrUmmIntraMeasCfgForRangeExtModCellSpfcOffSet)

   measObjId = measUeCb->intraMeasObj.measObjId;
   addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt++] = measObjId;
   RETVALUE(ROK);
} /* wrUmmIntraMeasCfgForRangeExtModCellSpfcOffSet */

/** @brief This function is responsible for Allocate Reconfiguration message and
 * intializing Object.
 *
 *  @details*
 *  Function:wrUmmLteAdvAllocRecfgMsg
 *  
 *  Processing steps:
 *      - create Reconfig message
 *      - initialize header and fetch transaction Id.
 *  
 *   @param[out]  rrcReCfgMsg: pointer to RRC reconfig message.
 *   @param[in]  transCb: pointer to transaction control block.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmLteAdvAllocRecfgMsg
(
 NhuDatReqSdus               **rrcReCfgMsg,
 WrUmmTransCb                *transCb
)
{
   NhuDatReqSdus             *rrcMsg;
   WrUeCb                    *ueCb;
   WrUmmMeasTransInfo        *measInfo;
   
   TRC2(wrUmmLteAdvAllocRecfgMsg)

   measInfo = &transCb->u.lteAdvTransCb.u.measInfo;
   ueCb     = transCb->ueCb;
   /* Allocating the RRC Reconfiguration Event                            */
   WR_ALLOCEVNT(&(rrcMsg), sizeof(NhuDatReqSdus));
   if(NULLP == rrcMsg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);/* Klockwork P1 warning fix */
   }
   *rrcReCfgMsg = rrcMsg;

   rrcMsg->hdr.cellId  = ueCb->cellId;
   rrcMsg->hdr.ueId    = ueCb->crnti;
   rrcMsg->hdr.transId = transCb->transId;

   /* Fetch an RRC transaction id for this procedure.                     */
   wrUmmGetRrcTransId(ueCb, C1_RRCCONRECFGN,
         WR_GET_TRANS_FRM_TRANSID(transCb->transId),
         &measInfo->transId);
   /* Create the reconfiguration message which is constructued excluding
    * the actual procedure related details which will be added further
    * */
   if (wrUmmRrcCreateRrcReconfigMsg(*rrcReCfgMsg, measInfo->transId) != ROK)
   {
      RLOG0(L_ERROR, " Intialization failed ");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

#ifdef TFU_UPGRADE
/** @brief This function is responsible for building RRC
 *  reconfig message.
 * 
 *  @details
 * Function:wrUmmLteAdvSfrBuildReCfgMsg
 *
 * @param[in]  rrcReCfgMsg: pointer to reconfig Message.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE Void wrUmmLteAdvSfrBuildReCfgMsg
(
 NhuDatReqSdus                *rrcReCfgMsg,
 WrUmmTransCb                 *transCb
)
{
   NhuDL_DCCH_Msg                       *dcchMsg;
   NhuRRCConRecfgn                      *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs               *recfgIEs;
   NhuRadioResourceConfigDedicated      *radResCfgDed;
   NhuPhysicalConfigDedicated           *phyCfgDed;
   NhuPDSCH_ConfigDedicated             *pdSchCfgDed;
   WrCellId                             cellId;
   WrCellCb                             *cellCb;
   RgrSfrConfig                         *sfrCfg;

   TRC2(wrUmmLteAdvSfrBuildReCfgMsg);

   cellId        = transCb->ueCb->cellId;
   dcchMsg       = &(rrcReCfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg   = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   recfgIEs      = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);

   WR_GET_CELLCB(cellCb, cellId)
   if(cellCb == NULLP)        
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", (U16)cellId);
       RETVOID;          
   }

   radResCfgDed  = &(recfgIEs->radioResourceConfigDedicated);
   wrFillTknU8(&radResCfgDed->pres, PRSNT_NODEF);

   phyCfgDed = &(radResCfgDed->physicalConfigDedicated);
   wrFillTknU8(&phyCfgDed->pres, PRSNT_NODEF);

   pdSchCfgDed = &(phyCfgDed->pdsch_ConfigDedicated);
   wrFillTknU8(&pdSchCfgDed->pres, PRSNT_NODEF);

   sfrCfg = &cellCb->lteAdvCb.lteAdvCfg.sfrCfg;

   if(transCb->ueCb->lteAdvCb.isUeCellEdge)
   {   
      wrFillTknU32(&pdSchCfgDed->p_a, sfrCfg->pwrThreshold.pHigh);
   }
   else
   {   
      wrFillTknU32(&pdSchCfgDed->p_a, sfrCfg->pwrThreshold.pLow);
   }
   RETVOID;
}

/** @brief This function is responsible for Allocate Reconfiguration message and
 * intializing Object.
 *
 *  @details*
 *  Function:wrUmmLteAdvSfrPrepRrcRecfgMsg
 *  
 *  Processing steps:
 *      - create Reconfig message
 *      - initialize header and fetch transaction Id.
 *  
 *   @param[out]  rrcReCfgMsg: pointer to RRC reconfig message.
 *   @param[in]  transCb: pointer to transaction control block.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmLteAdvSfrPrepRrcRecfgMsg
(
 NhuDatReqSdus               **rrcReCfgMsg,
 WrUmmTransCb                *transCb
)
{
   NhuDatReqSdus             *rrcMsg;
   WrUeCb                    *ueCb;
   U8                       rrcTransId;
   
   TRC2(wrUmmLteAdvSfrPrepRrcRecfgMsg)

   ueCb     = transCb->ueCb;
   /* Allocating the RRC Reconfiguration Event                            */
   WR_ALLOCEVNT(&(rrcMsg), sizeof(NhuDatReqSdus));
   if(NULLP == rrcMsg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   } 
   *rrcReCfgMsg = rrcMsg;

   rrcMsg->hdr.cellId  = ueCb->cellId;
   rrcMsg->hdr.ueId    = ueCb->crnti;
   rrcMsg->hdr.transId = transCb->transId;

   /* Fetch an RRC transaction id for this procedure.                     */
   wrUmmGetRrcTransId(ueCb, C1_RRCCONRECFGN,
         WR_GET_TRANS_FRM_TRANSID(transCb->transId),
         &rrcTransId);
   /* Create the reconfiguration message which is constructued excluding
    * the actual procedure related details which will be added further
    * */
   if (wrUmmRrcCreateRrcReconfigMsg(*rrcReCfgMsg, rrcTransId) != ROK)
   {
      RLOG0(L_ERROR, " Intialization failed ");
      RETVALUE(RFAILED);
   }

   wrUmmLteAdvSfrBuildReCfgMsg(*rrcReCfgMsg, transCb);

   RETVALUE(ROK);
}


/** @brief This function is responsible for Processing RRC message
 *
 * @details
 *
 *  Function:wrUmmSfrPrcRrcMsg 
 * 
 *  Processing steps:
 *    - Process RRC Message
 *     
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmSfrPrcRrcMsg
(
WrUmmTransCb           *transCb
)
{
   NhuUlDcchMsg        *ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
   NhuUL_DCCH_MsgTypc1 *c1     = &ulDcch->dcchMsg.message.val.c1;
   S16                 ret;      

   TRC2(wrUmmSfrPrcRrcMsg)

   if(C1_RRCCONRECFGNCOMPL == c1->choice.val)
   {
      ret = wrUmmRelRrcTransId(transCb->ueCb, C1_RRCCONRECFGN, 
                            c1->val.rrcConRecfgnCompl.rrc_TrnsxnId.val);
      if (ret != ROK)     
      {                   
        RLOG0(L_ERROR, " Release of transaction ID failed ");
      }
   }   
   else
   {
      RLOG1(L_ERROR, " Invalid message received: %d ", c1->choice.val);
   }

   wrUmmTransComplete(transCb);

   RETVALUE(ROK);
}
#endif

/** @brief This function is responsible for Processing RRC message
 *
 * @details
 *
 *  Function:wrUmmRangeExtPrcRrcMsg 
 * 
 *  Processing steps:
 *    - Process RRC Message
 *     
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmRangeExtPrcRrcMsg
(
WrUmmTransCb           *transCb
)
{
   NhuUlDcchMsg        *ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
   NhuUL_DCCH_MsgTypc1 *c1     = &ulDcch->dcchMsg.message.val.c1;
   S16                 ret;      

   TRC2(wrUmmRangeExtPrcRrcMsg)

   if(C1_RRCCONRECFGNCOMPL == c1->choice.val)
   {
      wrUmmMeasCfgReCfgReqCompHand(WR_UMM_MEAS_TYPE_HO, transCb);
      ret = wrUmmRelRrcTransId(transCb->ueCb, C1_RRCCONRECFGN, 
                            c1->val.rrcConRecfgnCompl.rrc_TrnsxnId.val);
      if (ret != ROK)     
      {                   
        RLOG0(L_ERROR, " Release of transaction ID failed ");
      }
      wrUmmTransComplete(transCb);
   }   
   else
   {
      RLOG1(L_ERROR, " Invalid message received: %d ", c1->choice.val);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for Processing RRC message
 *
 * @details
 *
 *  Function:wrUmmAbsPrcRrcMsg 
 * 
 *  Processing steps:
 *    - Process RRC Message
 *     
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmAbsPrcRrcMsg
(
WrUmmTransCb           *transCb
)
{
   NhuUlDcchMsg        *ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
   NhuUL_DCCH_MsgTypc1 *c1     = &ulDcch->dcchMsg.message.val.c1;
   S16                 ret;      

   TRC2(wrUmmAbsPrcRrcMsg)

   if(C1_RRCCONRECFGNCOMPL == c1->choice.val)
   {
      wrUmmMeasCfgReCfgReqCompHand(WR_UMM_MEAS_TYPE_HO, transCb);
      ret = wrUmmRelRrcTransId(transCb->ueCb, C1_RRCCONRECFGN, 
                            c1->val.rrcConRecfgnCompl.rrc_TrnsxnId.val);
      if (ret != ROK)     
      {                   
        RLOG0(L_ERROR, " Release of transaction ID failed ");
      }
      wrUmmTransComplete(transCb);
   }   
   else
   {
      RLOG1(L_ERROR, " Invalid message received: %d ", c1->choice.val);
   }

   RETVALUE(ROK);
}
/** @brief This function is responsible for Processing internal message
 *
 * @details
 *
 *  Function:wrUmmRangeExtPrcIntMsg 
 * 
 *  Processing steps:
 *    - Process measurement configaration
 * 
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmRangeExtPrcIntMsg
(
WrUmmTransCb           *transCb
)
{
   NhuMeasConfig            *nhuMeasCfg   = NULLP;
   WrCellCb                 *emmCellCb     = NULLP;
   WrUmmCellCb              *ummCellCb    = NULLP;
   WrUeCb                   *ueCb         = transCb->ueCb;
   WrMeasUeCb               *measUeCb     = ueCb->measCfg;
   WrUmmMeasTransInfo       *measInfo     = &transCb->u.lteAdvTransCb.u.measInfo;
   NhuDatReqSdus            *rrcReCfgMsg  = NULLP;
   S16                      ret           = RFAILED;

   TRC2(wrUmmRangeExtPrcIntMsg)
   WR_GET_CELLCB(emmCellCb, ueCb->cellId)
   if(emmCellCb == NULLP)        
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", (U16)ueCb->cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                  
   WR_UMM_GET_CELLCB(ummCellCb, ueCb->cellId)
   if(ummCellCb == NULLP)        
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", (U16)ueCb->cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                  

   switch(emmCellCb->lteAdvCb.lteAdvCfg.enbType)
   {
      case WR_LTE_ADV_PICO_ENB:
      case WR_LTE_ADV_MACRO_ENB:
      {
         ret = wrUmmIntraMeasCfgForRangeExtModCellSpfcOffSet(measUeCb,
                                         &measInfo->addMeasInfo, ummCellCb);
         break;
      }
   }

   if(ret == ROK)
   {
      wrUmmLteAdvAllocRecfgMsg(&rrcReCfgMsg, transCb);
      wrUmmMeasGetNhuMeasCfgFrmReCfg(rrcReCfgMsg, &nhuMeasCfg);
      nhuMeasCfg->pres.pres = PRSNT_NODEF;
      wrMeasFillNhuMsg(&rrcReCfgMsg->memCp, nhuMeasCfg, ueCb,
                                    measInfo, ummCellCb->measCellCfg, transCb->transId);
      RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending " 
            "RRCConnectionReconfiguration for LTE ADV Measurement Configuration");
      wrIfmRrcSndReqToUe (rrcReCfgMsg);
   }
   else
   {
      RLOG0(L_ERROR, " Range extention meas config failed ");
      wrUmmTransComplete(transCb);
   }
   RETVALUE(ROK);
} /* wrUmmRangeExtPrcIntMsg */

/* LTE_ADV_ABS starts */
/** @brief This function is responsible for Processing internal message
 *
 * @details
 *
 *  Function: wrUmmAbsCfgUeMeas
 * 
 *  Processing steps:
 *    - Process measurement configaration
 * 
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmAbsCfgUeMeas
(
WrUmmTransCb           *transCb
)
{
   NhuMeasConfig            *nhuMeasCfg   = NULLP;
   WrCellCb                 *emmCellCb     = NULLP;
   WrUmmCellCb              *ummCellCb    = NULLP;
   WrUeCb                   *ueCb         = transCb->ueCb;
   WrUmmIntLteAdvMsg        *lteAdvMsg    = transCb->msg->u.internalMsg->u.lteAdvMsg;
   WrUmmMeasTransInfo       *measInfo     = &transCb->u.lteAdvTransCb.u.measInfo;
   NhuDatReqSdus            *rrcReCfgMsg  = NULLP;
   S16                      ret           = RFAILED;

   TRC2(wrUmmAbsCfgUeMeas)
   WR_GET_CELLCB(emmCellCb, ueCb->cellId)
   if(emmCellCb == NULLP)        
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", (U16)ueCb->cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                   
   WR_UMM_GET_CELLCB(ummCellCb, ueCb->cellId)
   if(ummCellCb == NULLP)        
   {    
       RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", (U16)ueCb->cellId);
       RETVALUE(RFAILED);        /* KW fix for LTE_ADV */   
   }                   
   switch(emmCellCb->lteAdvCb.lteAdvCfg.enbType)
   {
      case WR_LTE_ADV_PICO_ENB:
      {
         if(TRUE == lteAdvMsg->isEnabled)
         {
            ret = wrUmmIntraMeasCfgForLteAdvAddEvntA3(ueCb,
                       &measInfo->addMeasInfo, ummCellCb, WR_UMM_MEAS_EVENT_A3_ABS);
         }
         else
         {
            ret = wrUmmIntraMeasCfgForLteAdvDelEvntA3(ueCb->measCfg,
                       &measInfo->rmvMeasInfo, ummCellCb, WR_UMM_MEAS_EVENT_A3_ABS);
         }
         break;
      }
      case WR_LTE_ADV_MACRO_ENB:
      {
         /* Nothing to do */
         break;
      }
   }

   if(ret == ROK)
   {
      wrUmmLteAdvAllocRecfgMsg(&rrcReCfgMsg, transCb);
      wrUmmMeasGetNhuMeasCfgFrmReCfg(rrcReCfgMsg, &nhuMeasCfg);
      nhuMeasCfg->pres.pres = PRSNT_NODEF;
      wrMeasFillNhuMsg(&rrcReCfgMsg->memCp, nhuMeasCfg, ueCb,
                                    measInfo, ummCellCb->measCellCfg, transCb->transId);
      wrIfmRrcSndReqToUe (rrcReCfgMsg);
   }
   else
   {
      RLOG0(L_ERROR, " ABS meas config failed ");
      wrUmmTransComplete(transCb);
   }
   RETVALUE(ROK);
} /* wrUmmAbsCfgUeMeas */

/** @brief This function is responsible for Processing internal message
 *
 * @details
 *
 *  Function:wrUmmAbsPrcIntMsg 
 * 
 *  Processing steps:
 *    - 
 * 
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmAbsPrcIntMsg
(
WrUmmTransCb           *transCb
)
{
   WrUmmIncMsg         *msg   = transCb->msg;

   TRC2(wrUmmAbsPrcIntMsg)

   if(WR_UMM_LTE_ADV_PEER_UE == msg->u.internalMsg->u.lteAdvMsg->peerType)
   {
      wrUmmAbsCfgUeMeas(transCb);
   }
   else
   {
      wrUmmLteAdvSchdUeRecfgReq(transCb);
      RLOG0(L_DEBUG, " Scheduler config sent ");
   }

   RETVALUE(ROK);
}

/* LTE_ADV_ABS ends */


/** @brief This function is responsible for receive Lte Advanced related messages.
 *
 * @details
 *
 *  Function:wrUmmLteAdvProcessor
 * 
 *  Processing steps:
 *    - Check the received message is internal or from UE.
 *    - calls respective API's
 * 
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *
 */
PUBLIC S16 wrUmmLteAdvProcessor
( 
WrUmmTransCb   *transCb
) 
{
   WrUmmIncMsg               *msg   = transCb->msg;
#ifdef TFU_UPGRADE         
   NhuDatReqSdus             *rrcReCfgMsg  = NULLP;
#endif
   TRC2(wrUmmLteAdvProcessor)
   
   RLOG3(L_DEBUG, " msgTyp: %d procType: %d transId: 0x%lx ", 
         msg->msgTyp,transCb->u.lteAdvTransCb.procType, transCb->transId);
   switch(msg->msgTyp)
   {
      case WR_UMM_UU_IND_PDU:
      {
         if(WR_UMM_LTE_ADV_RE_PROC == transCb->u.lteAdvTransCb.procType)
         {
            wrUmmRangeExtPrcRrcMsg(transCb);
         }
/* LTE_ADV_ABS starts */
         else if(WR_UMM_LTE_ADV_ABS_PROC == transCb->u.lteAdvTransCb.procType)
         {
            wrUmmAbsPrcRrcMsg(transCb);
         }
#ifdef TFU_UPGRADE         
         else if(WR_UMM_LTE_ADV_SFR_PROC == transCb->u.lteAdvTransCb.procType)
         {
            wrUmmSfrPrcRrcMsg(transCb);
         }   
#endif         
/* LTE_ADV_ABS ends */
         break;
      }
      case WR_UMM_RRC_DAT_CFM:
      {
         if(msg->u.rrcDatCfm->datCfmStatus != NHU_ERRCAUSE_NONE)
         {
            RLOG1(L_ERROR, " data confirmation failure: %d ", msg->u.rrcDatCfm->datCfmStatus);
            wrUmmTransComplete(transCb);
         }
         break;
      }
      case WR_UMM_INTERNAL_MSG:
      {
         /* Update procedure type for further use */
         transCb->u.lteAdvTransCb.procType = msg->u.internalMsg->u.lteAdvMsg->procType;

         if(WR_UMM_LTE_ADV_RE_PROC == transCb->u.lteAdvTransCb.procType)
         {
            wrUmmRangeExtPrcIntMsg(transCb);
         }
/* LTE_ADV_ABS starts */
         else if(WR_UMM_LTE_ADV_ABS_PROC == transCb->u.lteAdvTransCb.procType)
         {
            wrUmmAbsPrcIntMsg(transCb);
         }
/* LTE_ADV_ABS ends */
         else if(WR_UMM_LTE_ADV_SFR_PROC == transCb->u.lteAdvTransCb.procType)
         {
            wrUmmLteAdvSchdUeRecfgReq(transCb);
            RLOG0(L_DEBUG, " Scheduler config sent ");
         }
         else
         {
            wrUmmTransComplete(transCb);
         }
         break;
      }
      case WR_UMM_SCHD_CFG:
      {
            RLOG0(L_DEBUG, " Scheduler config cfm received ");

         /* Sending RRC reconfiguration message to UE for
          * PA reconfiguration in dedicated pdsch config IE. 
          * For CE UE Phigh value is sent as PA and for CC
          * UE plow value is sent as PA.*/
#ifdef TFU_UPGRADE         
         if(WR_UMM_LTE_ADV_SFR_PROC == transCb->u.lteAdvTransCb.procType)
         {
            wrUmmLteAdvSfrPrepRrcRecfgMsg(&rrcReCfgMsg, transCb);
            RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[OTA] Sending "
                  "RRCConnectionReconfiguration for LTE ADV SFR Processing");
            wrIfmRrcSndReqToUe (rrcReCfgMsg); 
         }   
#endif         
         break;
      }
      default:
      {
         RLOG1(L_ERROR, " Unknown message received:%d ", msg->msgTyp);
      }
   }

   RETVALUE(ROK);
}

/* LTE_ADV_ABS starts */
/** @brief This function is responsible for handling ABS measurement report
 *
 * @details
 *
 *  Function: wrUmmAbsPrcHdlMeasRpt
 * 
 *  Processing steps:
 *    - Mark UE as ABS UE
 *    - Post the event to EMM module
 * 
 *  @param[in]  ueCb : UE Control Block
 *  @param[in]  measRprt : Measurement report from UE
 *  @param[in]  earfcn : DL EARFCN
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmAbsPrcHdlMeasRpt
(
WrUeCb                       *ueCb,
NhuMeasResults               *measRprt,
U32                          earfcn
)
{
   U8                        idx        = 0;
   U8                        nghCnt     = 0;
   WrHoNghInfo               nghInfo;
   NhuMeasResultLstEUTRA     *eutraMeasRes;
   WrEmmAbsEvent             emmAbsEvent;

   TRC2(wrUmmAbsPrcHdlMeasRpt)

   ueCb->lteAdvCb.absInfo.isAbsUe = TRUE;
   eutraMeasRes = &(measRprt->measResultNeighCells.val.measResultLstEUTRA);

   for(idx = 0; idx < eutraMeasRes->noComp.val; idx++)
   {
      nghInfo.t.intraLteNghInfo.pci    = (U16)eutraMeasRes->member[idx].physCellId.val; /* KW fix for LTE_ADV */
      nghInfo.t.intraLteNghInfo.earfcn = earfcn;
      if(wrEmmGetIntraLteHoNghInfo(ueCb->cellId, &nghInfo) == ROK)
      {
         emmAbsEvent.u.absUeEvent.cellInfo[nghCnt].cellId.cId = nghInfo.t.intraLteNghInfo.eutranCellId;
         emmAbsEvent.u.absUeEvent.cellInfo[nghCnt].cellId.plmnId = nghInfo.plmnId; 
         WR_GET_ENBID_FROM_PEERID(emmAbsEvent.u.absUeEvent.cellInfo[nghCnt].enbId.val, 
                                                                   nghInfo.peerId);
         nghCnt++;
      }
   }

   if(nghCnt)
   {
      emmAbsEvent.eventId = WR_EMM_ABS_ABS_UE_EVENT;
      emmAbsEvent.u.absUeEvent.ueId = ueCb->crnti;
      emmAbsEvent.u.absUeEvent.cellId = ueCb->cellId;
      emmAbsEvent.u.absUeEvent.numOfCell = nghCnt;
      wrEmmAbsProcessEvent(&emmAbsEvent);
   }

   RETVALUE(ROK); 
} 

/* LTE_ADV_ABS ends */


/** @brief This function is responsible for deallocate transaction related
 * information after transaction complete.
 *
 * @details
 *
 *  Function:wrUmmLteAdvTransRel
 * 
 *  Processing steps:
 *    - Deallocates transaction information.
 * 
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmLteAdvTransRel
(
WrUmmTransCb                 *transCb
)
{
   TRC2(wrUmmLteAdvTransRel);
   RETVALUE(ROK);
}
/* LTE_ADV ends */

#ifdef __cplusplus
}
#endif /* __cplusplus */

