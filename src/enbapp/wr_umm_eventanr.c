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
               

     File:     wr_umm_anrmeas.c

     Sid:      fc_umm_eventanr.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/T2K_MergeTemp_BR/1 - Mon Sep  2 17:30:52 2013

     Prg:      Sriky 

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";

static int RLOG_FILE_ID=57;

static int RLOG_MODULE_ID=32;

#ifdef __cplusplus
extern "C" {
#endif

#include "wr.h"
#include "wr_utils.h"
#include "wr_umm.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_trans.h"
#include "wr_umm_sch_msg.h"
#include "wr_umm_l1_msg.h"
#include "wr_ifm_schd.h"
#include "wr_ifm_rrc.h"
#include "nhu_asn.x"
#include "wr_lwr.h"
#include "wr_umm_meas_fun.h"
#include "wr_umm_drx.h"

#define WR_UMM_MAXCELLS_PER_REPORT    8
#define WR_UMM_ANR_DRX_RETRANS_TMR_VAL 1

/*Function prototype*/
PRIVATE S16 wrUmmEventAnrHdlMeasRpt(WrUmmTransCb *transCb,NhuMeasResults *measRprt);

#ifdef __cplusplus
extern "C" }
#endif


/** @brief This function is responsible for configuring Anr Measurement for
 *  Intra Frequency.
 * @details
 *
 *  Function:wrUmmEventAnrIntraCfg
 *
 *  Processing steps:
 *    - Configure Strongest or Report CGI for Intra Frequency
 *
 *  @param[out] anrMeasIntMsg : pointer to internal structure
 *  @param[in]  measUeCb      : pointer to measurement UE CB
 *  @param[in]  measTransCb   : pointer to the Measurement Transaction CB.
 *  @param[in]  cellCb        : pointer to the Cell control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmEventAnrIntraCfg
(
 WrUmmIntAnrMeasMsg          *anrMeasIntMsg,
 WrUeCb                      *ueCb,
 WrUmmEventAnrTransInfo      *measTransCb,
 WrUmmCellCb                 *cellCb
 )
{
   U8                        measId;
   U8                        measObjId;
   U8                        rptCfgId;
   WrMeasUeCb                *measUeCb;
   LwrCellMeasCfgGrp         *measCellCfg;
   WrUmmMeasRmvInfo          *rmvMeasInfo;
   WrUmmMeasAddInfo          *addMeasInfo;
   WrUmmMeasRptCfgInfo       rptCfgInfo;
   TRC2(wrUmmEventAnrIntraCfg)

      measUeCb = ueCb->measCfg;
   measObjId = measUeCb->intraMeasObj.measObjId;

   measCellCfg = cellCb->measCellCfg;
   rmvMeasInfo = &measTransCb->measTransCfg.rmvMeasInfo; 
   addMeasInfo = &measTransCb->measTransCfg.addMeasInfo;
   if(measUeCb->rmvAnrMeasId != 0)
   {
      rmvMeasInfo->measIdRmvLst[rmvMeasInfo->measIdRmvLstCnt] = measUeCb->rmvAnrMeasId;
      rmvMeasInfo->measIdRmvLstCnt++;
      measUeCb->rmvAnrMeasId = 0;
   }
   if(anrMeasIntMsg->rptCfgType == WR_UMM_MEAS_REPORT_CGI)
   {
      measUeCb->intraMeasObj.newPci = anrMeasIntMsg->newPci;
      addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
      addMeasInfo->measObjIdCnt++;
      measTransCb->measTransCfg.addMeasInfo.isCellsToBeSkip = TRUE;
      if(measTransCb->isDrxCfged == FALSE)
      {
         measTransCb->isDrxReqForRptCgi = TRUE;
      }
      else
      {
         RLOG2(L_ALWAYS, "EMM: %d: already configured: %d",
               __LINE__, measTransCb->isDrxReqForRptCgi);
         RLOG0(L_WARNING, "DRX already configured");
      }
      /*Report config for EUTRA ReportCGI*/
      if(NULLP ==  measUeCb->eutraRptCgiCfg)
      {
         if(RFAILED == wrUmmMeasGetFreeRptCfgId(measUeCb, &rptCfgId))
         {
            RLOG0(L_ERROR, "Report config not available");
            RETVALUE(RFAILED);
         }
         WR_ALLOC(&measUeCb->eutraRptCgiCfg, sizeof(WrMeasRptCfgNode));
         if(NULLP == measUeCb->eutraRptCgiCfg)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            wrUmmMeasSetFreeRptCfgId(measUeCb, rptCfgId);
            RETVALUE(RFAILED);
         }
         rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_REPORT_CGI;
         rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;
         rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
         rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

         wrUmmMeasCreateRptCfg(rptCfgId, &rptCfgInfo, measUeCb->eutraRptCgiCfg,
               cellCb->measCellCfg,ueCb);
         measTransCb->measTransCfg.addMeasInfo.isEutraRptCgiFlg = TRUE;
      }
      else
      {
         rptCfgId = measUeCb->eutraRptCgiCfg->rptCfgId;
         measTransCb->measTransCfg.addMeasInfo.isEutraRptCgiFlg = FALSE;
      }
      measUeCb->isRptCgiCfg = TRUE;
      /* Meas ID for Report CGI*/
      measId = wrUmmMeasCreatMeasIdNode(WR_UMM_MEAS_TYPE_EVENT_ANR, measObjId,
            rptCfgId, measUeCb);
      /* ccpu00132002 Added check for ReportCgi meas Id */
      if(measId != WR_UMM_MEAS_INVALID_MEAS_ID)
      {
         addMeasInfo->measIdLst[addMeasInfo->measIdCnt] = measId;
         addMeasInfo->measIdCnt++;
      }
      else
      {
         RLOG0(L_ERROR, "Invalid MeasID received");
         if(measTransCb->measTransCfg.addMeasInfo.isEutraRptCgiFlg == TRUE)
         {
            measTransCb->measTransCfg.addMeasInfo.isEutraRptCgiFlg = FALSE;
            WR_FREE(measUeCb->eutraRptCgiCfg, sizeof(WrMeasRptCfgNode));
            wrUmmMeasSetFreeRptCfgId(measUeCb, rptCfgId);
         }
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}


/** @brief This function is responsible for configuring Anr Measurement for
 *  Inter Frequency.
 * @details
 *
 *  Function:wrUmmEventAnrInterCfg
 *
 *  Processing steps:
 *    - Configure Strongest or Report CGI for Inter Frequency
 *
 *  @param[out]  anrMeasIntMsg: pointer to internal structure
 *  @param[in]  measUeCb: pointer measurement UE CB
 *  @param[in]  measTransCb: pointer to the Measurement Transaction CB.
 *  @param[in]  cellCb: pointer to the Cell control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmEventAnrInterCfg
(
 WrUmmIntAnrMeasMsg          *anrMeasIntMsg,
 WrUeCb                      *ueCb,/*rel10*/
 WrUmmEventAnrTransInfo      *measTransCb,
 WrUmmCellCb                 *cellCb
 )
{
   U8                           measId;
   U8                           measObjId;
   U8                           rptCfgId;
   LwrCellMeasCfgGrp            *measCellCfg;
   WrUmmMeasEutraMeasObjNode    *eutraCellObj;
   WrUmmMeasObjInfo             measObjInfo;
   WrUmmMeasRmvInfo             *rmvMeasInfo;
   WrUmmMeasAddInfo             *addMeasInfo;
   WrUmmMeasRptCfgInfo          rptCfgInfo;
   WrMeasUeCb                   *measUeCb;/*rel10*/

   TRC2(wrUmmEventAnrInterCfg)
      measCellCfg = cellCb->measCellCfg;
   measUeCb = ueCb->measCfg;/*rel10*/
   rmvMeasInfo = &measTransCb->measTransCfg.rmvMeasInfo; 
   addMeasInfo = &measTransCb->measTransCfg.addMeasInfo;

   if(measUeCb->rmvAnrMeasId != 0)
   {
      rmvMeasInfo->measIdRmvLst[rmvMeasInfo->measIdRmvLstCnt] = measUeCb->rmvAnrMeasId;
      rmvMeasInfo->measIdRmvLstCnt++;
      measUeCb->rmvAnrMeasId = 0;
   }
   if(anrMeasIntMsg->rptCfgType == WR_UMM_MEAS_REPORT_CGI)
   {
      /* Meas Object for report CGI*/
      if(ROK != wrUmmMeasUpdtEutraMeasObjAndIdFromArfcn(anrMeasIntMsg->
               arfcn.t.carrierFreq, measUeCb, &measObjId, &eutraCellObj))
      {
         wrUmmMeasUpdtCellEutraMeasObjFrmArfcn(anrMeasIntMsg->arfcn.t.carrierFreq,
               &cellCb->measCellCb->interMeasObjLst, &eutraCellObj);

         measObjId = wrUmmMeasAddInterMeasObjToUeCb(measUeCb, eutraCellObj, 
               anrMeasIntMsg->newPci);
         if(measObjId != WR_UMM_MEAS_INVALID_MEAS_ID)
         {
            addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
            addMeasInfo->measObjIdCnt++;
         }
         else
         {
            RLOG0(L_ERROR, "measurement ID not available");
            RETVALUE(RFAILED);
         }
      }
      else
      {
         wrUmmMeasFindMeasObjAndTyp (&measObjInfo, measObjId, measUeCb);
         measTransCb->measTransCfg.addMeasInfo.isCellsToBeSkip = TRUE;
         measObjInfo.eutraObj->newPci = anrMeasIntMsg->newPci;
         addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
         addMeasInfo->measObjIdCnt++;
      }
      /*Report config for EUTRA ReportCGI*/
      if(NULLP ==  measUeCb->eutraRptCgiCfg)
      {
         if(RFAILED == wrUmmMeasGetFreeRptCfgId(measUeCb, &rptCfgId))
         {
            RLOG0(L_ERROR, "Report config not available");
            RETVALUE(RFAILED);
         }
         WR_ALLOC(&measUeCb->eutraRptCgiCfg, sizeof(WrMeasRptCfgNode));
         if(NULLP == measUeCb->eutraRptCgiCfg)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            wrUmmMeasSetFreeRptCfgId(measUeCb, rptCfgId);
            RETVALUE(RFAILED);
         }
         rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_REPORT_CGI;
         rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;
         rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
         rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

         wrUmmMeasCreateRptCfg(rptCfgId, &rptCfgInfo, measUeCb->eutraRptCgiCfg,
               cellCb->measCellCfg,ueCb);
         measTransCb->measTransCfg.addMeasInfo.isEutraRptCgiFlg = TRUE;
      }
      else
      {
         rptCfgId = measUeCb->eutraRptCgiCfg->rptCfgId;
         measTransCb->measTransCfg.addMeasInfo.isEutraRptCgiFlg = FALSE;
      }
      measUeCb->isRptCgiCfg = TRUE;
      measTransCb->isDrxReqForRptCgi = TRUE;
      RLOG2(L_ALWAYS, "EMM: %d: setting TRUE: %d",
            __LINE__, measTransCb->isDrxReqForRptCgi);

      /* Meas ID for Report CGI*/
      measId = wrUmmMeasCreatMeasIdNode(WR_UMM_MEAS_TYPE_EVENT_ANR, measObjId,
            rptCfgId, measUeCb);
      /* ccpu00132002 Added check for ReportCgi meas Id */
      if(measId != WR_UMM_MEAS_INVALID_MEAS_ID)
      {
         addMeasInfo->measIdLst[addMeasInfo->measIdCnt] = measId;
         addMeasInfo->measIdCnt++;
      }
      else
      {
         RLOG0(L_ERROR, "Invalid MeasID received");
         if(measTransCb->measTransCfg.addMeasInfo.isEutraRptCgiFlg == TRUE)
         {
            measTransCb->measTransCfg.addMeasInfo.isEutraRptCgiFlg = FALSE;
            WR_FREE(measUeCb->eutraRptCgiCfg, sizeof(WrMeasRptCfgNode));
            wrUmmMeasSetFreeRptCfgId(measUeCb, rptCfgId);
         }
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

PRIVATE Void wrUmmEvntAnrRmvAllMeasId
(
   WrUmmMeasTransInfo         *measTransCb,
   WrUeCb                     *ueCb
)
{
   CmLList                      *measIdLnk;
   WrMeasIdNode                 *measIdNode;
   U8                            measIdRmvCnt = 0;
   U8                            idx = 0;

   /* As the measIdRmvLstCnt may increase based on the below transversing, 
      to avoid unnecessary iterations initiatlised measIdRmvCnt outside of the 
      loop */
   measIdRmvCnt = measTransCb->rmvMeasInfo.measIdRmvLstCnt;

   /* Remove the measurement Ids from meas Id list based on event ANR meas type */
   measIdLnk = ueCb->measCfg->measIdLst.first;
   while(NULLP != measIdLnk)
   {
      measIdNode = (WrMeasIdNode*)measIdLnk->node;

      if((measIdNode != NULLP) &&
         (measIdNode->measIdTyp == WR_UMM_MEAS_TYPE_EVENT_ANR))
      {
         /* To avoid the duplication of the measurement Id in the remove list
            traverse though the list added so far.*/
         for ( idx = 0; idx < measIdRmvCnt; idx++ )
         {
            if ( measTransCb->rmvMeasInfo.measIdRmvLst[idx] == measIdNode->measID)
            {
               break;
            }
         }

         if(idx >= measIdRmvCnt)
         {
            /* This is new measurement Id */
            measTransCb->rmvMeasInfo.measIdRmvLst[measTransCb->rmvMeasInfo.measIdRmvLstCnt] = measIdNode->measID;
            RLOG2(L_INFO, "Deleting MeasID=%d\t MeasIdRmvCnt=%d",
                  measTransCb->rmvMeasInfo.measIdRmvLst[measTransCb->rmvMeasInfo.measIdRmvLstCnt],
                  (measTransCb->rmvMeasInfo.measIdRmvLstCnt + 1));
            measTransCb->rmvMeasInfo.measIdRmvLstCnt = measTransCb->rmvMeasInfo.measIdRmvLstCnt + 1;
         }
      }
      measIdLnk = measIdLnk->next;
  }
  RETVOID;
}

/** @brief This function is responsible for reconfiguring the UE DRX config
  at the scheduler.
 *
 * @details
 *
 *     Function: wrUmmEventAnrDrxSchdUeRecfgReq
 *
 *         Processing steps:
 *         - allocate a new configuration request
 *         - set the configuration type to RGR_UE_CFG
 *         - fill the ue info and DRX Reconfig info
 *         - send the request to scheduler
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILE
 */
PRIVATE S16 wrUmmEventAnrDrxSchdUeRecfgReq
(
 WrUmmTransCb                 *transCb
 )
{
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   U32                       transId;
   WrUmmEventAnrTransInfo    *eventAnrTransCb = &transCb->u.eventAnrTrans;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmCellCb               *cellCb;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   transId = transCb->transId;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = ueCb->cellId;
   ueRecfg->oldCrnti = ueCb->crnti;
   ueRecfg->newCrnti = ueCb->crnti;

   wrUmmSchdFillUeDrxCfgInfo(ueRecfg, eventAnrTransCb->drxCfgPtr);

   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}

/** @brief This function is responsible for Processing message for selected UE
 *
 * @details
 *
 *  Function:wrUmmEventAnrPrcMsg
 *
 *  Processing steps:
 *    - Process measurement configuration
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @param[in]  anrMeasIntMsg: pointer to the Anr Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmEventAnrPrcMsg
(
 WrUmmTransCb                 *transCb,
 WrUmmIntAnrMeasMsg           *anrMeasIntMsg
 )
{
   WrUeCb                       *ueCb = transCb->ueCb;
   WrUmmCellCb                  *cellCb;
   WrUmmEventAnrTransInfo       *eventAnrTransCb = &transCb->u.eventAnrTrans;
   U16                          longDrxTmrVal;
   TRC2(wrUmmEventAnrPrcMsg)

      WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
      if(NULLP == cellCb)
      {
         RLOG0(L_ERROR, "cellCb is NULL");
         RETVALUE(RFAILED);
      }

   wrUmmMeasResetTransCb(&eventAnrTransCb->measTransCfg);

   if(anrMeasIntMsg->choice == WR_UMM_ANR_MEAS_CREATE)
   {
      if(anrMeasIntMsg->cellTyp == WR_UMM_MEAS_INTRA_FREQ)
      {
         /* ccpu00132002 Added check for validating the ReportCgi configuration */
         if(RFAILED == wrUmmEventAnrIntraCfg(anrMeasIntMsg,ueCb,eventAnrTransCb,cellCb))
         {
            RLOG0(L_ERROR,
                  "wrUmmEventAnrPrcMsg: Report CGI config is failed for Intra freq");
            RETVALUE(RFAILED);
         }
      }
      else if(anrMeasIntMsg->cellTyp == WR_UMM_MEAS_INTER_FREQ)
      {
         /* ccpu00132002 Added check for validating the ReportCgi configuration */
         if(RFAILED == wrUmmEventAnrInterCfg(anrMeasIntMsg,ueCb,eventAnrTransCb,cellCb))/*rel10*/
         {
            RLOG0(L_ERROR,
                  "wrUmmEventAnrPrcMsg: Report CGI config is failed for Inter freq");
            RETVALUE(RFAILED);
         }
      }
   }
   /* Configuring Meas Objects and report configuration List*/
   if((eventAnrTransCb->isDrxReqForRptCgi == TRUE) &&
         (eventAnrTransCb->isDrxCfged == FALSE))
   {
      /* Copy the old/Default DRX configuration in UeCb 
       * Send the DRX configuration first to Scheduler */
      eventAnrTransCb->isDrxCfged = TRUE;
      eventAnrTransCb->ueDrxCfg.isDrxEnabled = TRUE;
      eventAnrTransCb->ueDrxCfg.drxOnDurTmr = wrEmmGetAnrDrxOnDur(ueCb->cellId);
      eventAnrTransCb->ueDrxCfg.drxLongCycleOffst.longDrxCycle = wrEmmGetAnrDrxLongCycle(ueCb->cellId);
      /*TODO need to ensure that the drx start offset falls 
        within the long cycle offset */
      longDrxTmrVal = 
         wrDrxLongCycle[eventAnrTransCb->ueDrxCfg.drxLongCycleOffst.longDrxCycle];
      eventAnrTransCb->ueDrxCfg.drxLongCycleOffst.drxStartOffst = 
         (ueCb->cqiOffst - 2 + longDrxTmrVal) % longDrxTmrVal; 

      eventAnrTransCb->ueDrxCfg.drxInactvTmr = wrEmmGetAnrDrxInActivTmrVal(ueCb->cellId);
      eventAnrTransCb->ueDrxCfg.drxRetxTmr = WR_UMM_ANR_DRX_RETRANS_TMR_VAL;
      eventAnrTransCb->ueDrxCfg.drxShortDrx.pres = FALSE;


      /*Store the RRC Re-Config message, once scheduler Confirm comes
       * then send Re-config message to UE */
      eventAnrTransCb->drxCfgPtr = &eventAnrTransCb->ueDrxCfg;
      eventAnrTransCb->isReCfgSent = TRUE;
      wrUmmEventAnrDrxSchdUeRecfgReq(transCb);
   }
   else
   {
      /*Prepare and  Send RRC Reconfiguration  message to UE */
      if(ROK == wrUmmMeasPrepareSendRrcRecfg(ueCb->cellId,
               ueCb, &eventAnrTransCb->measTransCfg,
               transCb->transId))
      {
         /* Start RRC Reconfigure timer */
         wrStartTransTmr(transCb,WR_TMR_RECONFIGURE, 
               wrEmmGetRrcReCfgTmrVal(ueCb->cellId));
      }
      else
      {
         RLOG0(L_ERROR,
               "wrUmmAnrMeasPrcMsg: Deallocating RRC Reconfg Message");
         /* ccpu00132002 Added check for validation */
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for Handling measurement report
 * received from UE.
 *
 * @details
 *
 * Function:wrUmmEventAnrHdlMeasRpt
 *
 *   Processing steps:
 *    - Get Measurement Id from received measurement report.
 *    - From Measaurement Id get the measurement Object Id and report config Id.
 *    - From measurement Object Id find the cell type and carrier frequency info.
 *    - From report config Id, find report config Type.
 *    - If Measurement report for strongest cells, give list of PCI,s
 *     which is present in measurement report to ANR Module to find New Pci's.
 *    - If Measurement report for Report CGI, send report to EMM and update Ue 
 *    Cb Measurement Object and send to Ue.
 *
 * @param[in]  transCb pointer to Transaction control block
 * @param[in]  measRprt pointer to Measurement report
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmEventAnrHdlMeasRpt
(
 WrUmmTransCb                 *transCb,
 NhuMeasResults               *measRprt
 )
{
   U8                           measId;
   U8                           measObjId;
   U8                           rptCfgId;
   U32                          recvMeasId;
   WrMeasRptCfgType             rptCfgTyp;
   WrMeasIdNode                 *measIdObj = NULLP;
   WrUmmIntAnrMeasMsg           *anrMeasIntMsg = NULLP;
   WrMeasUeCb                   *measUeCb;
   LwrCellMeasCfgGrp            *measCellCfg;
   WrUmmEventAnrTransInfo       *eventAnrTransCb;
   WrUmmMeasObjInfo             measObjInfo;
   WrUmmCellCb                  *cellCb;
   WrUeCb                       *ueCb;
   U16                          cellId;

   TRC2(wrUmmEventAnrHdlMeasRpt)

      ueCb = transCb->ueCb;
   cellId = ueCb->cellId;
   WR_UMM_GET_CELLCB(cellCb, cellId)
      if(NULLP == cellCb)
      {
         RLOG0(L_ERROR, "cellCb is NULL");
         RETVALUE(RFAILED);
      }
   measUeCb = ueCb->measCfg;
   eventAnrTransCb = &transCb->u.eventAnrTrans;

   /* Assign Event ANR TransId to the event ANR transID of ueCb */
   transCb->ueCb->eventAnrTransId = transCb->transId;

   recvMeasId = measRprt->measId.val;
   if(RFAILED == wrUmmMeasGetMeasIdObj(recvMeasId, &measIdObj, &ueCb->measCfg->measIdLst))
   {
      RLOG0(L_ERROR, "MeasId Object not found from UE Cb");
      RETVALUE(RFAILED);
   }

   measId = measIdObj->measID;
   measObjId = measIdObj->measObjID;
   rptCfgId = measIdObj->rptCfgID;

   measCellCfg = cellCb->measCellCfg;
   /* get measurement object and Type*/
   if(ROK !=  wrUmmMeasFindMeasObjAndTyp(&measObjInfo,measObjId,ueCb->measCfg))
   {
      RLOG0(L_ERROR, "Measurement object not found");
      /*Print error message*/
      RETVALUE(RFAILED);
   }
   /* get report config object and Type*/
   if((NULLP != measUeCb->eutraRptCgiCfg) &&
         (rptCfgId == measUeCb->eutraRptCgiCfg->rptCfgId))
   {
      rptCfgTyp = WR_UMM_MEAS_REPORT_CGI;
   }
   else if((NULLP != measUeCb->utraRptCgiCfg) &&
         (rptCfgId == measUeCb->utraRptCgiCfg->rptCfgId))
   {
      rptCfgTyp = WR_UMM_MEAS_UTRA_REPORT_CGI;
   }
   else
   {
      if( ROK != wrUmmMeasFindRptCfgTyp(rptCfgId, &rptCfgTyp, &measUeCb->rptCfgLst))
      {
         RLOG0(L_ERROR, "Report config object not found");
         RETVALUE(RFAILED);
      }
   }
   switch (rptCfgTyp)
   {
      case WR_UMM_MEAS_EVENT_A3:
      case WR_UMM_MEAS_EVENT_A5:
         {
            /* If UE is NOT in CSFB_READY state i.e. if CSFB is going on 
             * for that UE, no action is taken on A3/A5 measurement reports*/
            if(WR_CSFB_GSM_UE_STATE_CSFB_READY != ueCb->csfbInfo.csfbState)
            {
               RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti,"A3/A5 "
                     "measurement report received. Ignoring this report "
                     "as CSFB has been triggered for this UE");
               break;
            }
            RLOG_ARG0(L_DEBUG,DBG_CRNTI,ueCb->crnti, "Received ANR Event A3 report");

            /* Add the received PCI information to the New PCI list only if no 
               other eNb App has initiated Event ANR stop procedure.
               */
            if(TRUE == eventAnrTransCb->closeTrns)
            {
               break;
            }
            if(ROK != wrUmmFindNewPciFrmAnrMeasRpt(&measObjInfo, 
                     measRprt, transCb, TRUE))
            {
               RLOG0(L_ERROR, "wrUmmFindNewPciFrmAnrMeasRpt Failed");
               RETVALUE(RFAILED);
            }
            /* If Inter/Intra new PCI count is zero and ReportCGI 
             * configuration is not ongoing */
            if ((measUeCb->isRptCgiCfg != TRUE) && 
                  ((measUeCb->intraNewPci.newPciLst.count != 0))) 
            {
               /* Configure Report CGI config to UE */
               anrMeasIntMsg = wrUmmAnrMeasIntraNewPciCfg (ueCb);
            }
            else if ((measUeCb->isRptCgiCfg != TRUE) &&
                  (measUeCb->interNewPciLst.count != 0))
            {
               /* Configure Report CGI config to UE */
               anrMeasIntMsg =
                  wrUmmAnrMeasInterNewPciCfg (&measUeCb->interNewPciLst);

            }
            if(anrMeasIntMsg != NULLP)
            {
               if(ROK != wrUmmEventAnrPrcMsg(transCb, anrMeasIntMsg))
               {
                  RLOG0(L_ERROR, "wrUmmEventAnrPrcMsg Failed");
                  WR_FREE(anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg));
                  RETVALUE(RFAILED);
               }
               WR_FREE(anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg));
            }
            else
            {
               if((measUeCb->isRptCgiCfg == FALSE)  &&
                     (eventAnrTransCb->isDrxReqForRptCgi != TRUE))
               {
                  RLOG2(L_ALWAYS,"EMM: %d: %d: Closing Transaction",
                        __LINE__, eventAnrTransCb->isDrxReqForRptCgi);
                  wrUmmTransComplete(transCb);
               }
            }
            break;
         }
      case WR_UMM_MEAS_REPORT_CGI:
         {
            RLOG_ARG0(L_INFO,DBG_CRNTI,ueCb->crnti, "Received report "
                  "to report cgi");

            measUeCb->isRptCgiCfg = FALSE;

            /*ccpu00134769 -To avoid crash */
            if((TRUE == measRprt->measResultNeighCells.choice.pres) &&
                  (0 != measRprt->measResultNeighCells.val.measResultLstEUTRA.noComp.val))
            {
               if(measRprt->measResultNeighCells.val.measResultLstEUTRA.member[0].
                     cgi_Info.pres.pres == TRUE)
               {
                  /*This function will add new PCI in NRT list and 
                   * send the RRC Re-config message to all connected UEs and
                   * send the SIB4 also */
                  wrEmmMeasReportCgiHdlr(ueCb->cellId, measObjInfo.nrType, 
                        measObjInfo.eutraObj->eutraMeasObj->carrFreq, measRprt,
                        ueCb->ueCap->eUTRA_Cap.accessStratumRls.val);
                  /* CSG_DEV start */
                  wrUmmCsgAnrUpdtEutranCsgInfo(ueCb, measRprt,
                        measObjInfo.eutraObj->eutraMeasObj->carrFreq);
                  /* CSG_DEV end */

               }
               else
               {
                  RLOG_ARG0(L_INFO,DBG_CRNTI,ueCb->crnti, "Empty CGI report"
                        "Received");
               }
            }
            else 
            {
               RLOG_ARG0(L_INFO, DBG_CRNTI,ueCb->crnti,"CGI report with"
                     "zero neghbour cell Received");
            }

            /*Delete the PCI object from UeCb. */
            wrUmmMeasCfgDelNewPciObj(&measObjInfo, measUeCb);

            /* ccpu00132002 Fix -> Delete Cgi meas Id from UeCb */
            wrUmmMeasRmvMeaIdInUeCb(recvMeasId, &measUeCb->measIdLst);
            wrUmmMeasSetFreeMeasId(measUeCb, recvMeasId);
            /*Any subsequent CGI report is not handled during CSFB*/
            if(WR_CSFB_GSM_UE_STATE_CSFB_READY != ueCb->csfbInfo.csfbState)
            {
               break;
            }

            RLOG_ARG2(L_DEBUG, DBG_CRNTI, ueCb->crnti,
                  "num PCI in Intra-newPCI list = %ld "
                  "inter-newPci list = %ld",
                  measUeCb->intraNewPci.newPciLst.count,
                  measUeCb->interNewPciLst.count);
            if (measUeCb->intraNewPci.newPciLst.count != 0)
            {
               anrMeasIntMsg = wrUmmAnrMeasIntraNewPciCfg (ueCb);
            }
            else if (measUeCb->interNewPciLst.count != 0)
            {
               anrMeasIntMsg =
                  wrUmmAnrMeasInterNewPciCfg (&measUeCb->interNewPciLst);
            }

            if(anrMeasIntMsg != NULLP)
            {
               if(ROK != wrUmmEventAnrPrcMsg(transCb, anrMeasIntMsg))
               {
                  RLOG0(L_ERROR, "wrUmmEventAnrPrcMsg Failed");
                  WR_FREE(anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg));
                  RETVALUE(RFAILED);
               }
               WR_FREE(anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg));
            }
            else
            {
               RLOG1(L_ALWAYS, "DRX cfg = %d", 
                     eventAnrTransCb->isDrxReqForRptCgi);
               if(eventAnrTransCb->isDrxReqForRptCgi == TRUE)
               {
                  NhuDatReqSdus                *rrcReCfgMsg;
                  NhuMAC_MainConfig            *macCfg;
                  NhuMeasConfig                    *nhuMeasCfg;

                  /* If some other module has initiated event ANR close procedure 
                     while event ANR configured for the RPT CGI, then explicitly
                     measurement Id of EVENT ANR meas type  needs to be removed */
                  if(TRUE == eventAnrTransCb->closeTrns)
                  {
                     wrUmmEvntAnrRmvAllMeasId(&eventAnrTransCb->measTransCfg, ueCb);
                  }
                  /* No of new PCI count is zero, so release the DRX first
                   * from UE and once Re-config complete receives from UE
                   * then delete the DRX config from Scheduler also */
                  eventAnrTransCb->isDrxCfged = FALSE;

                  /*ccpu00128883*/
                  if(ROK == wrUmmMeasAllocRecfgMsg(&rrcReCfgMsg, 
                           &eventAnrTransCb->measTransCfg,
                           ueCb,
                           transCb->transId))
                  {
                     /* ccpu00132002 Fix -> Delete ReportCgi meas Id from UeCb */
                     wrUmmMeasGetNhuMacMainCfgFrmReCfg(rrcReCfgMsg, &macCfg);

                     /* MAC-MainConfig  */
                     WR_FILL_TKN_UINT(macCfg->pres, PRSNT_NODEF);

                     WR_FILL_TKN_UINT(macCfg->timeAlignmentTimerDedicated, 7);
                     eventAnrTransCb->drxCfgPtr = &ueCb->drxCfg;
                     wrUmmRrcFillDrxCfgIe(&macCfg->drx_Config, eventAnrTransCb->drxCfgPtr);

                     /* When eNB App is forcefully trying to stop the ANR transaction by deleting
                        PCI to be processed then it has to delete the measurement ID. TBD need to 
                        verify is it requried for P-ANR. */
                     if ((0 != eventAnrTransCb->measTransCfg.rmvMeasInfo.measIdRmvLstCnt) ||
                           (0 != eventAnrTransCb->measTransCfg.rmvMeasInfo.measObjIdRmvLstCnt) ||
                           (0 != eventAnrTransCb->measTransCfg.rmvMeasInfo.rptCfgRmvLstCnt))
                     {
                        wrUmmMeasGetNhuMeasCfgFrmReCfg(rrcReCfgMsg, &nhuMeasCfg);
                        if(ROK != wrUmmMeasFillNhuRmvIdLst(&rrcReCfgMsg->memCp, nhuMeasCfg, 
                                 &eventAnrTransCb->measTransCfg.rmvMeasInfo))
                        {
                           RLOG0(L_ERROR,"wrUmmFillNhuMsg: wrUmmMeasFillNhuRmvIdLst Failed.");
                           RETVALUE(RFAILED);
                        }
                     }

                     if(ROK == wrIfmRrcSndReqToUe(rrcReCfgMsg))
                     {
                        RLOG_ARG0(L_INFO,DBG_CRNTI,ueCb->crnti,
                              "RRC Re-Config message sent to release DRX");
                        /* Start RRC Reconfigure timer */
                        wrStartTransTmr(transCb,WR_TMR_RECONFIGURE, 
                              wrEmmGetRrcReCfgTmrVal(ueCb->cellId));
                     }
                     else
                     {
                        RLOG0(L_ERROR, "Deallocating RRC Reconfg Message");
                        WR_FREE_EVNT(rrcReCfgMsg);
                        RETVALUE(RFAILED);
                     }
                  } 
                  else
                  {
                     RLOG_ARG0(L_ERROR, DBG_CRNTI, ueCb->crnti,
                           "Failed to send DRX configuration ");
                     RLOG0(L_ALWAYS, "Failed to send DRX configuration");
                  }
               }
            }
            break;
         }
      default:
         RLOG_ARG0(L_ERROR,DBG_CRNTI,ueCb->crnti,
               "Received invalid report type");
         break;
   }
   RETVALUE(ROK);
}


/** @brief This function is responsible for Processing RRC message
 *
 * @details
 *
 *  Function:wrUmmEventAnrPrcRrcMsg
 *
 *  Processing steps:
 *    - Process RRC Message
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */

PRIVATE S16 wrUmmEventAnrPrcRrcMsg
(
 WrUmmTransCb                 *transCb
 )
{
   NhuUlDcchMsg                          *ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
   NhuUL_DCCH_MsgTypc1                   *c1;
   NhuMeasurementReport                  *measRpt;
   NhuMeasurementReportcriticalExtnsc1   *measRptC1;
   NhuMeasResults                        *measRes;
   U32                                   rrcTransId;
   S16                                   ret = ROK;
   WrMeasUeCb                            *measUeCb = NULLP;

   c1 = &ulDcch->dcchMsg.message.val.c1;
   measUeCb = transCb->ueCb->measCfg;
   switch(c1->choice.val)
   {
      case C1_MEASUREMENTREPORT:
         {
            measRpt = &c1->val.measurementReport;
            measRptC1 = &measRpt->criticalExtns.val.c1;
            measRes = &measRptC1->val.measurementReport_r8.measResults;

            wrUmmEventAnrHdlMeasRpt(transCb, measRes);
            break;
         }
      case C1_RRCCONRECFGNCOMPL:
         {
            RLOG_ARG0(L_INFO,DBG_CRNTI,transCb->ueCb->crnti,
                  "Received Reconfig Complete message");
            /* Stop the RRC reconfig timer */
            wrStopTransTmr(transCb,WR_TMR_RECONFIGURE);
            wrUmmMeasCfgReCfgReqCompHand(WR_UMM_MEAS_TYPE_EVENT_ANR, transCb);
            rrcTransId = c1->val.rrcConRecfgnCompl.rrc_TrnsxnId.val;
            ret = wrUmmRelRrcTransId(transCb->ueCb, C1_RRCCONRECFGN, rrcTransId);
            if (ret != ROK)
            {
               RLOG0(L_ERROR, "Release of transaction ID failed");
            }
            RLOG2(L_ALWAYS, "rptcgicfg = %d drx cfg = %d",
                  measUeCb->isRptCgiCfg,
                  transCb->u.eventAnrTrans.isDrxReqForRptCgi);
            if((measUeCb->isRptCgiCfg == FALSE) &&
                  (transCb->u.eventAnrTrans.isDrxReqForRptCgi == TRUE))
            {
               wrUmmEventAnrDrxSchdUeRecfgReq(transCb);
            }
            else if(measUeCb->isRptCgiCfg == TRUE)
            {
               break;
            }
            else if(transCb->u.eventAnrTrans.closeTrns == TRUE)
            {
               wrUmmTransComplete(transCb);
            }
            break;
         }
      default:
         {
            RLOG0(L_ERROR, "Unknown msg Type recived");
            RETVALUE(RFAILED);
         }
   }
   RETVALUE(ROK);
}

/** @brief This function creates teh internal message for event ANR.
 *
 * @details
 *
 *  Function:wrUmmEvntAnrMeasCreatInternalMsg
 *
 *  Processing steps:
 *      - Allocate memory for the internal message. 
 *      - Populate the internal message structure.
 *
 *  @param[in]  reCfgReq : Flag for transmission of configuring A3 ANR event
 *  @param[in]  choice   : Check do you need to create/delete the event transction.
 *  @param[out]  intMsg  : Pointer to internal message.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *
 */
PRIVATE S16  wrUmmEvntAnrMeasCreatInternalMsg
(
 WrUmmIntAnrMeasMsg          **intMsg,
 U8                          choice,
 Bool                        rrcRecfgReqd
)
{
   WrUmmIntAnrMeasMsg          *anrMeasIntMsg = NULLP;

   TRC2(wrUmmEvntAnrMeasCreatInternalMsg);

   WR_ALLOC (&anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg));
   if(NULLP == anrMeasIntMsg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   anrMeasIntMsg->choice = choice;
   anrMeasIntMsg->arfcn.choice = WR_UMM_MEAS_INTRA_FREQ;
   anrMeasIntMsg->cellTyp = WR_UMM_MEAS_INTRA_FREQ;
   anrMeasIntMsg->rptCfgType = WR_UMM_MEAS_EVENT_A3_FOR_ANR;
   anrMeasIntMsg->rrcReCfgRqd = rrcRecfgReqd;
   anrMeasIntMsg->newPci = 0xFFFF;

   *intMsg = anrMeasIntMsg;

   RETVALUE(ROK);
}


/** @brief This function is responsible for preparing and send Measurement 
 *        configuration IE to the UE.
 *
 * @details
 *
 *  Function:wrUmmEvntAnrPrepareSndMeasCfgUe
 *
 *  Processing steps:
 *    - Prepare and send Measurement configuration IE. 
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *
 */
PRIVATE S16 wrUmmEvntAnrPrepareSndMeasCfgUe
(
  WrUmmTransCb    *transCb
)
{
   NhuDatReqSdus               *recfgMsg = NULLP;
   WrUeCb                      *ueCb = transCb->ueCb;
   NhuDL_DCCH_Msg              *dcchMsg = NULLP;
   NhuRRCConRecfgn             *rrcConReCfg = NULLP;
   NhuRRCConRecfgn_r8_IEs      *recfgIEs = NULLP;
   WrUmmCellCb                 *cellCb = NULLP;
   WrUmmMeasTransInfo          *measTransCb = &transCb->u.eventAnrTrans.measTransCfg;
   U8                         transId = 0;

   /* Get the cellCb from the ueCb */
   WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,
                  "wrUmmEvntAnrPrepareSndMeasCfgUe: cellCb is NULL.");
      RETVALUE(RFAILED);
   }

   /* Fetch an RRC transaction id for this procedure.                     */
   if(wrUmmGetRrcTransId(ueCb, C1_RRCCONRECFGN, 
                         WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
                         &transId)!= ROK)
   {
      RLOG1(L_ERROR, 
               "[%d] Max RRC Transaction already Reached", ueCb->crnti);
      RETVALUE(RFAILED);
   }

   /* Prepare and send RRC Reconfiguration message.*/

   /* Allocate memory for the message */
   WR_ALLOCEVNT(&recfgMsg, sizeof(NhuDatReqSdus));
   if (recfgMsg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Fill the header of the RRC Reconfiguration message */
   recfgMsg->hdr.cellId  = ueCb->cellId;
   recfgMsg->hdr.ueId    = ueCb->crnti;
   recfgMsg->hdr.transId = transCb->transId | C1_RRCCONRECFGN;

   /* Initialize RRC Reconfig message */
   wrUmmRrcCreateRrcReconfigMsg(recfgMsg, transId);

   dcchMsg     = &(recfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   recfgIEs    = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);

   /* Fill Measurement Configuration IE                                   */
   wrMeasFillNhuMsg(&recfgMsg->memCp, 
                    &(recfgIEs->measConfig), 
                    ueCb, 
                    measTransCb, 
                    cellCb->measCellCfg,
                    transCb->transId);
   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending RRC reconfiguration"
         "for Event ANR measurement config ");
   /* Send the prepared message to the UE */
   if (RFAILED == wrIfmRrcSndReqToUe(recfgMsg))
   {
      RLOG0(L_ERROR,
               "wrUmmEvntAnrPrepareSndMeasCfgUe: wrIfmRrcSndReqToUe failed.");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }

   /* Start RRC Reconfigure timer */
   wrStartTransTmr(transCb, 
                   WR_TMR_RECONFIGURE, 
                   wrEmmGetRrcReCfgTmrVal(ueCb->cellId));
   
   RETVALUE(ROK);
}

/** @brief This function is responsible for receive event  ANR Measurement
 * related messages which is received from UE.
 *
 * @details
 *
 *  Function:wrUmmCreatEvntAnr
 *
 *  Processing steps:
 *    - Depending on the received message parameters prepare and send 
 *       RRC Reconfiguration with Intra A3 ANR events to the UE.
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *
 */
PRIVATE S16 wrUmmCreatEvntAnr
(
  WrUmmTransCb    *transCb
)
{
   U8                          measObjId = 0;
   WrUmmCellCb                 *cellCb = NULLP;
   WrMeasUeCb                  *measUeCb = transCb->ueCb->measCfg;
   WrUmmMeasCellCb             *measCellCb = NULLP;
   WrMeasUeCbEutraMeasObjNode  *enbMeasObj = NULLP;
   WrUmmMeasTransInfo          *measTransCb = &transCb->u.eventAnrTrans.measTransCfg;
   WrUmmMeasAddInfo            *addMeasInfo = &measTransCb->addMeasInfo;
   WrUmmMeasRptCfgInfo       rptCfgInfo;
   Bool                         isUeSupported = FALSE;
   WrUmmMeasEutraMeasObjNode    *eutraCellObj = NULLP;
   S16                          isUeCapable = FALSE;
   CmLList                      *interMeasObj = NULLP;
   U8                           targetBand; 
   LwrCellMeasCfgGrp            *measCellCfg = NULLP;
   U8                           rptCfgId = 0;

   /* Assign Event ANR TransId to the event ANR transID of ueCb */
   transCb->ueCb->eventAnrTransId = transCb->transId;

   if(transCb->msg->u.internalMsg->u.anrMeasMsg->rrcReCfgRqd == FALSE)
   {
      /* No need to send the RRC Reconfiguration message for configuring the A3 event. */
      RETVALUE(ROK);
   }

   /* Get the cellCb from the ueCb */
   WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,
                     "wrUmmCreatEvntAnr: cellCb is NULL");
      RETVALUE(RFAILED);
   }

   measCellCb = cellCb->measCellCb;

   /* Check does measurement object is already created */
   if(measUeCb->intraMeasObj.eutraMeasObj == NULLP)
   {
      /* Measurment object hasnot been created so create a new measurement object */
      if(WR_UMM_MEAS_INVALID_MEAS_ID != wrUmmMeasGetFreeMeasObjId(measUeCb, &measObjId))
      {
         enbMeasObj = &measUeCb->intraMeasObj;
         enbMeasObj->eutraMeasObj = &measCellCb->intraMeasObj;
         enbMeasObj->measObjId = measObjId;
         enbMeasObj->newPci = 0;

         addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
         addMeasInfo->measObjIdCnt++;
      }
      else
      {
         RLOG0(L_ERROR,
                  "wrUmmCreatEvntAnr: Meas Object Id not available\n");
         RETVALUE(RFAILED);
      }
   }
   else
   {
      /* Measurement object is already created */
      measObjId = measUeCb->intraMeasObj.measObjId;
   }

   /* Fill Intra A3 ANR Events in the measurment configuration in hte 
      ueCb and measTransCb based on the measObject Id */ 

   /* Populate the report configuration Information */
   rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_A3;
   rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_INTRA_ANR;

   /* Map Measurement Id, measurement object Id  and report configuration Id */
   if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_EVENT_ANR, measObjId,
                                      &rptCfgInfo, transCb->ueCb, 
                                      cellCb->measCellCfg, 
                                      &measTransCb->addMeasInfo))
   {
      RLOG0(L_ERROR,
               "wrUmmCreatEvntAnr: Invalid report and Meas config Id");
      RETVALUE(RFAILED);
   }
   /*Fix for ccpu00141304 -start*/
   rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_A1;
   rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

   if(ROK == (wrUmmMeasFindRptCfgID(&rptCfgId, &rptCfgInfo, &measUeCb->rptCfgLst))&&
      (measCellCb->anrRptCfg == WR_UMM_EVENT_A5_INTER_CFG))
   {
      measCellCfg = cellCb->measCellCfg;
      interMeasObj = measCellCb->interMeasObjLst.first;
      /* Check UE capabilites, before configuring A5 ANR measurement report. */ 
      isUeCapable = wrUmmAnrChkUeCap(transCb->ueCb, WR_UMM_MEAS_INTER_FREQ , 
                                               WR_UMM_EVENT_A5_INTER_CFG);
      while (NULLP != interMeasObj)
      {
         /*Meas Objects for inter frequencies*/
         eutraCellObj = (WrUmmMeasEutraMeasObjNode*)interMeasObj->node;

         /*Get bandclass for corresponding ARFCN*/
         wrUmmFndEUTRABand(eutraCellObj->carrFreq,&targetBand);

        /*Check whether the bandclass is UE supported or not*/
        isUeSupported = wrUmmEUTRABandSupported(transCb->ueCb,targetBand, 0);

        if(isUeSupported == TRUE)
        {
           /* Meas Object for Inter Frequency */
           if(ROK != wrUmmMeasUpdtEutraMeasObjAndIdFromArfcn(eutraCellObj->
                  carrFreq, measUeCb, &measObjId, &eutraCellObj))
           {
              measObjId = wrUmmMeasAddInterMeasObjToUeCb(measUeCb, eutraCellObj, 0XFFFF);
              if(measObjId != WR_UMM_MEAS_INVALID_MEAS_ID)
              {
                 addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
                 addMeasInfo->measObjIdCnt++;
              }
              else
              { 
                 RLOG0(L_ERROR, "INter Object addition in UeCb Failed");
                 RETVALUE(RFAILED);
              }
           } 
        }
        rptCfgInfo.rptCfgTyp      = WR_UMM_MEAS_EVENT_A5;
        rptCfgInfo.thrshldTyp     = WR_UMM_MEAS_THRESHOLD_RSRP;
        rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
        rptCfgInfo.intRptCfgType  = WR_UMM_MEAS_RPTCFG_INTER_ANR;
        if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_EVENT_ANR, 
             measObjId, &rptCfgInfo, transCb->ueCb, measCellCfg, addMeasInfo))
        {
           RLOG0(L_ERROR, " Invalid report and Meas config Id ");
           RETVALUE(RFAILED);
        }
        interMeasObj = interMeasObj->next;
     }   
  }
  /*Fix for ccpu00141304 -End*/ 

   /* Prepare and send RRC Reconfiguration message */
   wrUmmEvntAnrPrepareSndMeasCfgUe(transCb);

   RETVALUE(ROK);
}


/** @brief This function is responsible for closing ANR Event.
 *
 * @details
 *
 *  Function:wrUmmCloseEvntAnr
 *
 *  Processing steps:
 *    - Depending on the received message parameters prepare and send 
 *       RRC Reconfiguration with Intra A3 ANR events to the UE.
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *
 */
PRIVATE S16 wrUmmCloseEvntAnr
(
   WrUmmTransCb                 *transCb
)
{
   WrUeCb                       *ueCb;
   WrUmmCellCb                  *cellCb;

   ueCb = transCb->ueCb;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,
               "wrUmmCloseEvntAnr: cellCb is NULL");
      RETVALUE(RFAILED);
   }


   /* Delete the PCI of Intra Cells that needs to be processed */
   wrUmmDelIntraFreqNonCfgNewPciObj(&ueCb->measCfg->intraNewPci.newPciLst);

   /* Delete the PCI of Inter Cells that needs to be processed */
   wrUmmDelInterFreqNonCfgNewPciObj(&ueCb->measCfg->interNewPciLst);

  if(ueCb->measCfg->isRptCgiCfg == FALSE)
  {
     /* Fill remove meas Id list of measTransCfg */
     wrUmmEvntAnrRmvAllMeasId(&transCb->u.eventAnrTrans.measTransCfg, ueCb);

     /* CGI is not configured. */
     wrUmmEvntAnrPrepareSndMeasCfgUe(transCb);
  }

  /* set the flag for closing the transaction */
  transCb->u.eventAnrTrans.closeTrns = TRUE;

  RETVALUE(ROK);
}

/** @brief This function is responsible for processing Internal message
 * received by event ANR transaction.
 *
 * @details
 *
 *  Function:wrUmmEventAnrPrcIntMsg
 *
 *  Processing steps:
 *    - Depending on the type of the internal message received 
 *       Either start the event ANR transaction or release the ANR 
 *       transaction.
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *
 */
PRIVATE S16 wrUmmEventAnrPrcIntMsg
(
  WrUmmTransCb    *transCb
)
{
   WrUmmIncMsg         *msg = transCb->msg;
  WrUmmIntAnrMeasMsg   *anrMeasIntMsg = NULLP;

   anrMeasIntMsg = msg->u.internalMsg->u.anrMeasMsg;

   switch(anrMeasIntMsg->choice)
   {
      case WR_UMM_ANR_MEAS_CREATE:
         RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,
               "Starting Event ANR");
         /* Create Event ANR Transaction */
         wrUmmCreatEvntAnr(transCb); 
         break;
      case  WR_UMM_ANR_MEAS_DELETE:
         RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,
               "Stopping Event ANR");
         /* Close Event ANR Transaction */
         wrUmmCloseEvntAnr(transCb);
         break;

      default:
         break;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for starting Event ANR 
 * transaction. 
 *
 * @details
 *
 *  Function:wrUmmStartEventAnrTrans
 *
 *  Processing steps:
 *      - Start event ANR transaction 
 *
 *  @param[in]  reCfgReq : Flag for transmission of configuring A3 ANR event
 *  @param[in]  ueCb     : Pointer to the UE control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *
 */
PUBLIC S16 wrUmmStartEventAnrTrans
(
   Bool reCfgReq, 
   WrUeCb   *ueCb
)
{
   WrUmmInternalMsg          *internalMsg = NULLP;
   WrUmmIntAnrMeasMsg        *intMsg = NULLP;
   WrUmmIncMsg               *msg = NULLP;
   WrUmmCellCb               *cellCb = NULLP;
   S16                       ret = RFAILED;

    /* Check UE is supports Event ANR measurements */
    ret = wrUmmAnrChkUeCap(ueCb, WR_UMM_MEAS_INTRA_FREQ, WR_UMM_EVENT_A3_CFG);
    if(ret == FALSE)
    {
       RETVALUE(ROK);
    }

   /* Create the internal mesage */
   wrUmmEvntAnrMeasCreatInternalMsg(&intMsg, WR_UMM_ANR_MEAS_CREATE, reCfgReq);

   /* Build the internal message with EVENT ANR MEAS TRANS */
   if (ROK != wrUmmBldInternalMsg(&internalMsg,
                                  WR_UMM_INTMSG_EVNT_ANR_MEAS_TRANS, 
                                  intMsg))
   {
      RLOG0(L_ERROR,
               "wrUmmStartEventAnrTrans:creation of internal msg Failed");
      RETVALUE(RFAILED);
   }

   /* Allocate and fill wrUmmIncMsg*/
   WR_ALLOC(&msg,sizeof(WrUmmIncMsg));
   if (NULLP == msg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   msg->msgTyp = WR_UMM_INTERNAL_MSG;
   msg->u.internalMsg = internalMsg;
   msg->refCnt = 1;

   /* Get the cellCb from the ueCb */
   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,
                     "wrUmmStartEventAnrTrans: cellCb is NULL");
      RETVALUE(RFAILED);
   }

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);
   RETVALUE(ROK);
}

/** @brief This function is responsible for starting Event ANR 
 * transaction. 
 *
 * @details
 *
 *  Function:wrUmmStopEventAnrTrans
 *
 *  Processing steps:
 *      - Stop event ANR transaction 
 *
 *  @param[in]  reCfgReq : Flag for transmission of configuring A3 ANR event
 *  @param[in]  ueCb     : Pointer to the UE control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *
 */
PUBLIC S16 wrUmmStopEventAnrTrans
(
   WrUeCb   *ueCb
)
{
   WrUmmInternalMsg          *internalMsg = NULLP;
   WrUmmIntAnrMeasMsg        *intMsg = NULLP;
   WrUmmIncMsg               *msg = NULLP;
   WrUmmCellCb               *cellCb = NULLP;

   /* Create the internal mesage */
   wrUmmEvntAnrMeasCreatInternalMsg(&intMsg, WR_UMM_ANR_MEAS_DELETE, FALSE);

   /* Build the internal message with EVENT ANR MEAS TRANS */
   if (ROK != wrUmmBldInternalMsg(&internalMsg,
                                  WR_UMM_INTMSG_EVNT_ANR_MEAS_TRANS, 
                                  intMsg))
   {
      RLOG0(L_ERROR,
               "wrUmmStopEventAnrTrans:creation of internal msg Failed");
      RETVALUE(RFAILED);
   }

   /* Allocate and fill wrUmmIncMsg*/
   WR_ALLOC(&msg,sizeof(WrUmmIncMsg));
   if (NULLP == msg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   msg->msgTyp = WR_UMM_INTERNAL_MSG;
   msg->u.internalMsg = internalMsg;
   msg->refCnt = 1;

   /* Get the cellCb from the ueCb */
   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,
                     "wrUmmStopEventAnrTrans: cellCb is NULL.\n");
      RETVALUE(RFAILED);
   }

   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);
   RETVALUE(ROK);
}

/** @brief This function is responsible for receive event  ANR Measurement
 * related messages which is received from UE.
 *
 * @details
 *
 *  Function:wrUmmEventAnrProcessor
 *
 *  Processing steps:
 *    - Check the received message is measurement report or reconfig complete
 *    message.
 *    - calls respective API's
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *
 */
PUBLIC S16 wrUmmEventAnrProcessor
(
 WrUmmTransCb                 *transCb
 )
{
   WrUmmIncMsg               *msg  = transCb->msg;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmEventAnrTransInfo    *eventAnrTransCb = &transCb->u.eventAnrTrans;
   TRC2(wrUmmEventAnrProcessor);

   switch(msg->msgTyp)
   {
      case WR_UMM_UU_IND_PDU:
         {
            /* Function to handle measurement report and RRC Re-Config
             * Complete */
            wrUmmEventAnrPrcRrcMsg(transCb);
            break;
         }
      case WR_UMM_INTERNAL_MSG:
         {
            wrUmmEventAnrPrcIntMsg(transCb);
            break;
         }
      case WR_UMM_SCHD_CFG:
         {
            WrUmmCellCb                      *cellCb;
            NhuDatReqSdus                    *rrcReCfgMsg;
            NhuMAC_MainConfig                *macCfg;
            NhuMeasConfig                    *nhuMeasCfg;

            WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
            if(NULLP == cellCb)
            {
               RLOG0(L_ERROR, "cellCb is NULL");
               RETVALUE(RFAILED);
            }
            RLOG2(L_DEBUG, "wrUmmEventAnrProcessor: "
                  "sched Cfm received isRptCgiCfg = %d, isDrxReqForRptCgi : %d",
                  ueCb->measCfg->isRptCgiCfg, eventAnrTransCb->isDrxReqForRptCgi);

            if((ueCb->measCfg->isRptCgiCfg == FALSE) &&
                  (eventAnrTransCb->isDrxReqForRptCgi == TRUE))
            {
               /* Closing the transaction as DRX is released at
                * Scheduler and No new PCI is present. */
               RLOG_ARG0(L_DEBUG,DBG_CRNTI,ueCb->crnti,
                     "No new PCI present");
               if(eventAnrTransCb->closeTrns == TRUE)
               {
                  RLOG_ARG0(L_DEBUG,DBG_CRNTI,ueCb->crnti,
                        "Closing Event ANR trans");
                  wrUmmTransComplete(transCb);
               }
            }
            else
            {
               /* ccpu00131948 Fix */
               if(eventAnrTransCb->isReCfgSent == TRUE)
               {
                  eventAnrTransCb->isReCfgSent = FALSE;
                  /* Sending RRC Re-Config message to UE */
                  if(ROK != wrUmmMeasAllocRecfgMsg(&rrcReCfgMsg, 
                           &eventAnrTransCb->measTransCfg, 
                           ueCb, 
                           transCb->transId))
                  {
                     RLOG0(L_ERROR,
                           "wrUmmMeasAllocRecfgMsg failed.");
                     RETVALUE(RFAILED);
                  }
                  wrUmmMeasGetNhuMacMainCfgFrmReCfg(rrcReCfgMsg, &macCfg);

                  /* MAC-MainConfig  */
                  WR_FILL_TKN_UINT(macCfg->pres, PRSNT_NODEF);

                  WR_FILL_TKN_UINT(macCfg->timeAlignmentTimerDedicated, 7);
                  wrUmmRrcFillDrxCfgIe(&macCfg->drx_Config, eventAnrTransCb->drxCfgPtr);

                  wrUmmMeasGetNhuMeasCfgFrmReCfg(rrcReCfgMsg, &nhuMeasCfg);
                  if(ROK == wrMeasFillNhuMsg(&rrcReCfgMsg->memCp, 
                           nhuMeasCfg, 
                           ueCb,
                           &eventAnrTransCb->measTransCfg,
                           cellCb->measCellCfg, 
                           transCb->transId))
                  {
                     wrIfmRrcSndReqToUe(rrcReCfgMsg);
                     RLOG_ARG0(L_DEBUG,DBG_CRNTI,ueCb->crnti,
                           "Re-Config msg sent ");
                     /* Start RRC Reconfigure timer */
                     wrStartTransTmr(transCb,WR_TMR_RECONFIGURE, 
                           wrEmmGetRrcReCfgTmrVal(ueCb->cellId));
                     /* eventAnrTransCb->rrcReCfgMsg = NULLP; */
                  }
                  else
                  {
                     RLOG0(L_ERROR, "Deallocating RRC Reconfg Message");
                     WR_FREE_EVNT(rrcReCfgMsg);
                  }
               }
            }

            break;
         }
      case WR_UMM_TMR_EXPIRY:
         {
            wrUmmAnrPrcRrcTmrExpiry(transCb);
            /* if Timer expires when DRX is configured at Schduler
             * and UE doesn't sends Reconfig complete */
            if((ueCb->measCfg->isRptCgiCfg == FALSE) &&
                  (eventAnrTransCb->isDrxReqForRptCgi == TRUE))
            {
               RLOG_ARG0(L_INFO,DBG_CRNTI,ueCb->crnti,
                     "Deleting the DRX configuration at Scheduler");
               /* Delete the DRX configuration done at Scheduler */
               wrUmmEventAnrDrxSchdUeRecfgReq(transCb);
            }
            /* ccpu00132002 Fix -> Delete ReportCgi meas Id from UeCb */
            break;
         }
   }

   RETVALUE(ROK);
}

/** @brief This function is called when a new Transaction is created for any
 * other procedure. The new transaction is passed to this function and this
 * function determines the next course of action based on the new
 transaction
 *
 * @details
 *
 *     Function: wrUmmEventAnrNewTransProcessor
 *
 *         Description:
 *         This function is called when a new Transaction is
 *         created for any other procedure. The new transaction is passed to
 *         this function and this function determines the next course of action
 *         based on the new transaction
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *  -#Success : ROK
 *  -#Failure : RFAILED
 */

PUBLIC S16 wrUmmEventAnrNewTransProcessor
(
 WrUmmTransCb                *transCb,
 WrUmmTransCb                *incTrans
 )
{
   WrUeCb                    *ueCb = transCb->ueCb;
   Bool                      drxQciFeaFlag = FALSE;
   CmLList                   *lnk = NULLP;
   WrMeasNewPciNode          *freqNode = NULLP;

   TRC2(wrUmmEventAnrNewTransProcessor)
      switch(incTrans->transTyp)
      {
         case WR_UMM_RRC_REESTAB_TRANS:
            {
               RLOG0(L_INFO, "Reestablishment received and closing event "
                     "Anr transaction");
               /* Check & delete if any Intra PCI are present in the list */
               if(ueCb->measCfg->intraNewPci.newPciLst.count != 0)
               {
                  wrUmmMeasDeAllocMemForNewPciObj(&ueCb->measCfg->intraNewPci.newPciLst);
               }
               /* Check & delete if any Inter PCI are present in the list */
               if(ueCb->measCfg->interNewPciLst.count != 0)
               {
                  /* As the count is greater than 0 traverse through interNewPciLst list 
                     and release all PCI nodes*/ 
                  lnk = ueCb->measCfg->interNewPciLst.first;
                  while(lnk != NULLP)
                  {
                     /* select Inter-frequency list */
                     freqNode = (WrMeasNewPciNode *)lnk->node;
                     /* Delete all PCIs in selected Inter-frequency list */
                     wrUmmMeasDeAllocMemForNewPciObj(&freqNode->newPciLst);
                     /* Delete Inter-frequency node from the list */
                     cmLListDelFrm(&ueCb->measCfg->interNewPciLst, lnk);
                     /* free the memory for the node allocated */
                     WR_FREE(freqNode, sizeof(WrMeasNewPciNode));
                     lnk = ueCb->measCfg->interNewPciLst.first;
                  }
               }

               if(transCb->u.eventAnrTrans.isDrxReqForRptCgi == TRUE)
               {
                  ueCb->measCfg->isRptCgiCfg = FALSE;
                  wrUmmEventAnrDrxSchdUeRecfgReq(transCb);
                  /*
                     if (NULLP != transCb->u.eventAnrTrans.rrcReCfgMsg)
                     {
                     WR_FREE_EVNT(transCb->u.eventAnrTrans.rrcReCfgMsg);
                     }
                     */
               }
               else
               {
                  wrUmmTransComplete(transCb);
               }
               break;
            }
            /*DRX QCI*/
         case WR_UMM_ERAB_SETUP_TRANS:
            {
               if(wrGetDrxQCIFeatureFlag(transCb->ueCb->cellId,
                        &drxQciFeaFlag)!= ROK)
               {
                  RETVALUE(WR_UMM_TRANS_FAIL);   
               }
               if(drxQciFeaFlag == TRUE)
               {
                  if(wrUmmCheckGBRBearer(incTrans->msg) == ROK)
                  {
                     if(transCb->numDefMsgCount < WR_UMM_DEF_LIST)
                     {
                        WR_UMM_CPY_DEFR_MSG_TO_TRANSCB(transCb, incTrans->msg);
                        wrUmmStopAnr(transCb->ueCb);
                     } 
                     else
                     {
                        ALARM("Maximum number of Derefred Message Execeding"
                              "the Specified Limit at  Defered processor\n");
                     }
                     RETVALUE(WR_UMM_TRANS_FAIL);
                  }
               }
               break;
            }
      }

   RETVALUE(WR_UMM_TRANS_OK);
}

/** @brief This function is responsible for deallocate transaction related
 * information after transaction complete.
 *
 * @details
 *
 *  Function:wrUmmEventAnrTransRel
 *
 *  Processing steps:
 *    - Deallocates transaction information.
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmEventAnrTransRel
(
 WrUmmTransCb *transCb
 )
{
   TRC2(wrUmmEventAnrTransRel)
   wrUmmStopTransTimers(transCb);
      transCb->ueCb->eventAnrTransId = 0;
   RETVALUE(ROK);
}

/********************************************************************30**

     End of file:     fc_umm_eventanr.c@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/T2K_MergeTemp_BR/1 - Mon Sep  2 17:30:52 2013

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
SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
