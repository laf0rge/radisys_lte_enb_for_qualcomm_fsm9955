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

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=47;


static int RLOG_MODULE_ID=32;

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

#ifdef __cplusplus
extern "C" {
#endif

#define WR_UMM_ABOVE_RSRP_THRESHOLD  16
#define WR_UMM_ABOVE_RSRQ_THRESHOLD   8
#define WR_UMM_MAXCELLS_PER_REPORT    8
#define WR_UMM_ANR_DRX_RETRANS_TMR_VAL 0
#ifdef __cplusplus
}
#endif /* __cplusplus */

/**
*   @brief This function selects the next Rat Type of the ANR
* 
*       Function: wrUmmAnrSelNxtRatType
* 
*           Processing steps:
*           - Based on the current RAT type select the next RAT type.
* 
*   @param[in]   WrUeCb  : ueCb
*   @return S16  ROK
*/
PRIVATE S16 wrUmmAnrSelNxtRatType
(
  WrUeCb                       *ueCb
)
{
   WrUmmAnrMeasInfo            *ueAnrMeasInfo = &ueCb->measCfg->ueAnrMeasInfo;

   switch(ueAnrMeasInfo->currAnrCfgType)
   {
      case WR_UMM_MEAS_INTRA_FREQ:
      {
         /* Move to Inter Frequency ANR */
         ueAnrMeasInfo->currAnrCfgType = WR_UMM_MEAS_INTER_FREQ;
         ueAnrMeasInfo->currAnrInterFreqCnt = 0;
         break;
      }
      
      case WR_UMM_MEAS_INTER_FREQ:
      /*[TODO] */
      case WR_UMM_MEAS_UTRA_FDD_FREQ:
      case WR_UMM_MEAS_UTRA_TDD_FREQ:
      default :
      break;
   }
   RETVALUE(ROK);
}   

/** @brief This function is responsible for to updating new PCI List in UE CB
 *
 *  @details
 *  Function:wrUmmMeasUpdtNewPciInfo
 *      
 *  Processing steps:
 *      - Update new PCI list information
 *  
 *   @param[out] newPciNode: pointer to new PCI node.
 *   @param[in] pci: New Pci.
 *   @return S16
 *       SUCCESS : ROK
*/
PRIVATE S16 wrUmmMeasUpdtNewPciInfo
( 
  WrMeasNewPciNode            *newPciNode,
  U16                         pci
)
{
   CmLList                   *lnk;
   WrUmmMeasNewPciInfo       *newPciObj;

   TRC2(wrUmmMeasUpdtNewPciInfo)

   lnk = newPciNode->newPciLst.first;
   while(lnk != NULLP)
   {
      newPciObj = (WrUmmMeasNewPciInfo*)lnk->node;

      if(newPciObj->newPci == pci)
      {
         RETVALUE(ROK);
      }
      lnk = lnk->next;
   }
   WR_ALLOC(&newPciObj, sizeof(WrUmmMeasNewPciInfo));
   if(NULLP == newPciObj)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(NULLP);
   }
   newPciObj->newPci = pci;
   newPciObj->crnti = 0xFFFF;
   newPciObj->isConfigred = WR_UMM_NOT_CONFIGED;
   newPciObj->lnk.node = (PTR)newPciObj;
   cmLListAdd2Tail(&newPciNode->newPciLst, &newPciObj->lnk);
   RETVALUE(ROK);
}

/** @brief This function is responsible for to Find new PSC Node from UE CB
 *
 *  @details
 *  Function:wrUmmMeasGetNewPscNodeFrmArfcn
 *    
 *  Processing steps:
 *      - search in ue Cb doe New PSC node based on arfcn
 *  
 *   @param[out] measUeCb: pointer to Measurement Ue Cb.
 *   @param[in] measObjInfo: pointer to measuement Object info object.
 *   @return S16
 *       SUCCESS : ROK
*/
PRIVATE WrMeasNewPciNode* wrUmmMeasGetNewPscNodeFrmArfcn
(
WrMeasUeCb                   *measUeCb,
WrUmmMeasObjInfo             *measObjInfo
)
{
   CmLList                   *lnk;
   WrMeasNewPciNode          *newPscNode;

   TRC2(wrUmmMeasGetNewPscNodeFrmArfcn)
   lnk = measUeCb->utranNewPscLst.first;
   while(lnk != NULLP)
   {
      newPscNode = (WrMeasNewPciNode*)lnk->node;

      if(newPscNode->arfcn == measObjInfo->utraFddObj->utraMeasObj->arfcn)
      {
         RETVALUE(newPscNode);
      }
      lnk = lnk->next;
   }
   WR_ALLOC(&newPscNode, sizeof(WrMeasNewPciNode));
   if(NULLP == newPscNode)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(NULLP);
   }
   newPscNode->arfcn = measObjInfo->utraFddObj->utraMeasObj->arfcn;
   newPscNode->cellTyp = WR_UMM_MEAS_UTRA_FDD_FREQ;
   newPscNode->lnk.node = (PTR)newPscNode;
   cmLListAdd2Tail(&measUeCb->utranNewPscLst, &newPscNode->lnk);

   RETVALUE(newPscNode);
}

/** @brief This function is responsible for to Find new PCI Node from UE CB
 *
 *  @details
 *  Function:wrUmmMeasGetNewPciNodeFrmArfcn
 *    
 *  Processing steps:
 *      - search in ue Cb doe New PCI node based on arfcn
 *  
 *   @param[out] measUeCb: pointer to Measurement Ue Cb.
 *   @param[in] measObjInfo: pointer to measuement Object info object.
 *   @return S16
 *       SUCCESS : ROK
*/
PRIVATE WrMeasNewPciNode* wrUmmMeasGetNewPciNodeFrmArfcn
(
WrMeasUeCb                   *measUeCb,
WrUmmMeasObjInfo             *measObjInfo
)
{
   CmLList                   *lnk;
   WrMeasNewPciNode          *newPciNode;

   TRC2(wrUmmMeasGetNewPciNodeFrmArfcn)

   if(measObjInfo->nrType == WR_UMM_MEAS_INTRA_FREQ)
   {
      RETVALUE(&measUeCb->intraNewPci);
   }
   else
   {
      lnk = measUeCb->interNewPciLst.first;
      while(lnk != NULLP)
      {
         newPciNode = (WrMeasNewPciNode*)lnk->node;

         if(newPciNode->arfcn == measObjInfo->eutraObj->
               eutraMeasObj->carrFreq)
         {
            RETVALUE(newPciNode);
         }
         lnk = lnk->next;
      }
      WR_ALLOC(&newPciNode, sizeof(WrMeasNewPciNode));
      if(NULLP == newPciNode)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(NULLP);
      }
      newPciNode->arfcn = measObjInfo->eutraObj->eutraMeasObj->carrFreq;
      newPciNode->cellTyp = WR_UMM_MEAS_INTER_FREQ;
      newPciNode->lnk.node = (PTR)newPciNode;
      cmLListAdd2Tail(&measUeCb->interNewPciLst, &newPciNode->lnk);
      RETVALUE(newPciNode);
   }
   RETVALUE(NULLP);
}
 /** @brief This function is responsible for to create PCI List in UE CB
 *
 *  @details
 *  Function:wrUmmAnrMeasCheckForNewPci
 *    
 *  Processing steps:
 *      - Check Pci Flag
 *      - if Pci Flag is FALSE, add new PCI Node into UeCb
 *  
 *   @param[in] measObjInfo: pointer to measuement Object info object.
 *   @param[in] pciList: pointer to Pci List.
 *   @param[out] measUeCb: pointer to Measurement Ue Cb.
 *   @return S16
 *       SUCCESS : ROK
*/
PRIVATE S16 wrUmmAnrMeasCheckForNewPci 
(
WrUmmMeasObjInfo             *measObjInfo,
WrNewPciLst                  *pciList,
WrMeasUeCb                   *measUeCb  
)
{
   U8                        idCnt;
   U8                        noComp = 0;
   U8                        newPciCnt = 0; 
   WrNewPci                  *newPcis = NULLP;
   WrMeasNewPciNode          *newPciNode = NULLP;

   TRC2(wrUmmAnrMeasCheckForNewPci)
   newPciCnt = pciList->newPciCnt; 
   if(newPciCnt != 0)
   {
      noComp = pciList->numComp;
      newPcis = pciList->newPci;
      for(idCnt = 0; idCnt < noComp; idCnt++)
      {
         if(newPcis[idCnt].pciExist == TRUE)
         {
            if((measObjInfo->nrType == WR_UMM_MEAS_INTRA_FREQ) ||
                  (measObjInfo->nrType == WR_UMM_MEAS_INTER_FREQ))
            {
               /*Get the PCI List based on the Arfcn */
               newPciNode = wrUmmMeasGetNewPciNodeFrmArfcn(measUeCb, 
                     measObjInfo);
            }
            else if(measObjInfo->nrType == WR_UMM_MEAS_UTRA_FDD_FREQ) 
            {
               newPciNode = wrUmmMeasGetNewPscNodeFrmArfcn(measUeCb, 
                     measObjInfo);
            }
            else
            {
               RLOG0(L_ERROR, "Invalid Neighbour Type");

            }
            if(NULLP != newPciNode)
            {
               /*Add new PCI information into the PCI list*/
               if(ROK != wrUmmMeasUpdtNewPciInfo(newPciNode, newPcis[idCnt].pci))
               {
                  RLOG0(L_ERROR, "Adding New PCI Information Failed");
                  RETVALUE(RFAILED);
               }
            }
         }
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to create Eutra PCI List
 *
 *  @details
 *  Function:wrUmmCreatEutraPciLst
 *    
 *  Processing steps:
 *      - Copy all Pci's from meaurement report into PCI list Array
 *  
 *  @param[in] measRsultLstEutra: pointer to NhuMeasResultLstEUTRA object.
 *  @param[out] pciList: pointer to PCI list.
 *   @return S16
 *       SUCCESS : ROK
*/
PRIVATE S16 wrUmmCreatEutraPciLst
(
  NhuMeasResultLstEUTRA        *measRsultLstEutra,
  WrNewPciLst                  *pciList
)
{
   U8                        idCnt;
   U8                        pciCnt = 0;
   U8                        noComp;
   U16                       newPciLst[WR_UMM_MAXCELLS_PER_REPORT];
   TRC2(wrUmmCreatEutraPciLst)
   noComp = measRsultLstEutra->noComp.val;

   for (idCnt = 0;idCnt < noComp; idCnt++)
   {
      /* UE Can measure Neighbor cell if power is above -124dbm, below
       * -124dbm threshold, neighbor cell reported by UE is invalid */

      /*ccpu00130307 - If RSRQ and RSRQ are configured, check both RSRP and RSRQ.*/
      if((measRsultLstEutra->member[idCnt].measResult.rsrpResult.pres == PRSNT_NODEF) &&
            (measRsultLstEutra->member[idCnt].measResult.rsrqResult.pres == PRSNT_NODEF))
      {
         if(((measRsultLstEutra->member[idCnt].measResult.rsrpResult.val) >
                  WR_UMM_ABOVE_RSRP_THRESHOLD) &&
               ((measRsultLstEutra->member[idCnt].measResult.rsrqResult.val) >
                WR_UMM_ABOVE_RSRQ_THRESHOLD))
         {
            newPciLst[pciCnt++] = measRsultLstEutra->member[idCnt].physCellId.val;
         }
      }
      /*ccpu00130307 - If only RSRP is configured, check only RSRP.*/
      else if((measRsultLstEutra->member[idCnt].measResult.rsrpResult.pres == PRSNT_NODEF))
      {
         if ((measRsultLstEutra->member[idCnt].measResult.rsrpResult.val) >
               WR_UMM_ABOVE_RSRP_THRESHOLD)
         {
            newPciLst[pciCnt++] = measRsultLstEutra->member[idCnt].physCellId.val;
         }
      }
      /*ccpu00130307 - If only RSRQ is configured, check only RSRQ.*/
      else 
      {
         if((measRsultLstEutra->member[idCnt].measResult.rsrqResult.pres == PRSNT_NODEF))
         {
            if ((measRsultLstEutra->member[idCnt].measResult.rsrqResult.val) >
                  WR_UMM_ABOVE_RSRQ_THRESHOLD)
            {
               newPciLst[pciCnt++] = measRsultLstEutra->member[idCnt].physCellId.val;
            }
         }
      }
   }
   if(pciCnt != 0)
   {
      pciList->numComp = pciCnt;
      for (idCnt = 0;idCnt < pciCnt; idCnt++)
      {
         pciList->newPci[idCnt].pci = newPciLst[idCnt];
      }
      RETVALUE(ROK);
   }
   
   RETVALUE(RFAILED);
}
/** @brief This function is responsible for to create Utra PCI List
 *
 *  @details
 *  Function:wrUmmCreatUtraPciLst
 *    
 *  Processing steps:
 *      - Copy all Pci's from meaurement report into PCI list Array
 *  
 *  @param[in] measRsultLstUtra: pointer to NhuMeasResultLstUTRA object.
 *  @param[out] pciList: pointer to PCI list.
 *   @return S16
 *       SUCCESS : ROK
*/
PRIVATE S16 wrUmmCreatUtraPciLst
(
  NhuMeasResultLstUTRA        *measRsultLstUtra,
  WrNewPciLst                  *pciList
)
{
   U8                        idCnt;
   U8                        pciCnt = 0;
   U8                        noComp;
   U16                       newPciLst[WR_UMM_MAXCELLS_PER_REPORT];
   TRC2(wrUmmCreatUtraPciLst)
   noComp = measRsultLstUtra->noComp.val;

   for (idCnt = 0;idCnt < noComp; idCnt++)
   {
      if(measRsultLstUtra->member[idCnt].physCellId.choice.val == PHYSCELLID_FDD)
      {
         newPciLst[pciCnt++] = measRsultLstUtra->member[idCnt].physCellId.val.fdd.val;
      }
      else
      {
         newPciLst[pciCnt++] = measRsultLstUtra->member[idCnt].physCellId.val.tdd.val;
      }
   }
   if(pciCnt != 0)
   {
      pciList->numComp = pciCnt;
      for (idCnt = 0;idCnt < pciCnt; idCnt++)
      {
         pciList->newPci[idCnt].pci = newPciLst[idCnt];
      }
      RETVALUE(ROK);
   }
   
   RETVALUE(RFAILED);
}

/** @brief This function is responsible for to create PCI List
 *
 *  @details
 *  Function:wrUmmAnrMeasCreatePciLst
 *    
 *  Processing steps:
 *      - Copy all Pci's from meaurement report into PCI list Array
 *  
*   @param[out] pciList: pointer to PCI list.
 *   @param[in] measRprt: pointer to measurement report.
 *   @return S16
 *       SUCCESS : ROK
*/
PRIVATE S16 wrUmmAnrMeasCreatePciLst 
(
WrNewPciLst                  *pciList,
NhuMeasResults               *measRprt
)
{
   NhuMeasResultLstEUTRA     *measRsultLstEutra;
   NhuMeasResultLstUTRA      *measResultLstUTRA;

   TRC2(wrUmmAnrMeasCreatePciLst)

   /*Check is the RAT type EUTRA */
   if(measRprt->measResultNeighCells.choice.val ==
                        MEASRESULTNEIGHCELLS_MEASRESULTLSTEUTRA)
   {
      measRsultLstEutra = &measRprt->measResultNeighCells.val.measResultLstEUTRA;

      /*Based on the received RSRP and RSRQ values create the PCI List */
      if(RFAILED == wrUmmCreatEutraPciLst(measRsultLstEutra, pciList))
      {
         RLOG0(L_ERROR, "No Valid EUTRA PCI's found, Ignore Report ");
         RETVALUE(RFAILED);
      }
   }
   /*Check is the RAT type UTRA */
   else if(measRprt->measResultNeighCells.choice.val ==
                        MEASRESULTNEIGHCELLS_MEASRESULTLSTUTRA)
   {
      measResultLstUTRA = &measRprt->measResultNeighCells.val.measResultLstUTRA;
      /*Based on the received RSRP and RSRQ values create the PCI List */
      if(RFAILED == wrUmmCreatUtraPciLst(measResultLstUTRA, pciList))
      {
         RLOG0(L_ERROR, "No Valid UTRA PCI's found, Ignore Report");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for configuring Anr Measurement for
 *  Intra Frequency.
 * @details
 *
 *  Function:wrUmmAnrMeasIntraCfg
 *
 *  Processing steps:
 *    - Configure Strongest or Report CGI for Intra Frequency
 *
 *  @param[out]  anrMeasIntMsg: pointer to internal structure
 *  @param[in]  ueCb: pointer UE CB
 *  @param[in]  measTransCb: pointer to the Measurement Transaction CB.
 *  @param[in]  cellCb: pointer to the Cell control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmAnrMeasIntraCfg
(
 WrUmmIntAnrMeasMsg          *anrMeasIntMsg,
 WrUeCb                      *ueCb,
 WrUmmAnrMeasTransInfo       *measTransCb,
 WrUmmCellCb                 *cellCb
)
{
   S16                       ret;
   U8                        measId;
   U8                        measObjId;
   U8                        rptCfgId;
   WrMeasUeCb                *measUeCb;
   LwrCellMeasCfgGrp         *measCellCfg;
   WrUmmMeasRmvInfo          *rmvMeasInfo;
   WrUmmMeasAddInfo          *addMeasInfo;
   WrUmmMeasRptCfgInfo       rptCfgInfo;
   TRC2(wrUmmAnrMeasIntraCfg)

   measUeCb = ueCb->measCfg;
   measObjId = measUeCb->intraMeasObj.measObjId;

   measCellCfg = cellCb->measCellCfg;
   rmvMeasInfo = &measTransCb->measTransCfg.rmvMeasInfo; 
   addMeasInfo = &measTransCb->measTransCfg.addMeasInfo;
   measUeCb->measIdForR1 = 0;
   if(measUeCb->rmvAnrMeasId != 0)
   {
      rmvMeasInfo->measIdRmvLst[rmvMeasInfo->measIdRmvLstCnt] = measUeCb->rmvAnrMeasId;
      rmvMeasInfo->measIdRmvLstCnt++;
      measUeCb->rmvAnrMeasId = 0;
   }
   if(anrMeasIntMsg->rptCfgType == WR_UMM_MEAS_STRONG_CELL)
   {
      rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_STRONG_CELL;
      rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;
      rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
      rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

      /* Report Config for Strongest cell */
      if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_PERD_ANR, measObjId,
               &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
      {
         RLOG0(L_ERROR, "Invalid Report and Meas config Id ");
         RETVALUE(RFAILED);
      }
   }
   else if(anrMeasIntMsg->rptCfgType == WR_UMM_MEAS_REPORT_CGI)
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
            RLOG0(L_WARNING, "DRX already configured");
      }
      /*Report config for EUTRA ReportCGI*/
      if(NULLP ==  measUeCb->eutraRptCgiCfg)
      {
         ret = wrUmmMeasGetFreeRptCfgId(measUeCb, &rptCfgId);
         if(ret == WR_UMM_MEAS_INVALID_MEAS_ID)
         {
            RLOG0(L_ERROR, "Report config not available ");
            RETVALUE(ret);
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
      measId = wrUmmMeasCreatMeasIdNode(WR_UMM_MEAS_TYPE_PERD_ANR, measObjId,
            rptCfgId, measUeCb);
      if(measId != WR_UMM_MEAS_INVALID_MEAS_ID)
      {
         addMeasInfo->measIdLst[addMeasInfo->measIdCnt] = measId;
         addMeasInfo->measIdCnt++;
      }
      else
      {
         RLOG1(L_ERROR, "Invalid MeasID[%d] Received",measId);
         /* ccpu00132002 Added check for validating the ReportCgi configuration */
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

/** @brief This function schedules the Measurement gap to the scheduler.
 *
 * @details
 *
 *  Function:wrUmmAnrMeasGapSchdUeRecfgReq
 *
 *  Processing steps:
 *    - Prepare MAC message and send to scheduler for configuring the Measurement gap.
 *
 *  @param[in]  TransCb: pointer to the Transaction CB.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmAnrMeasGapSchdUeRecfgReq
(
 WrUmmTransCb                 *transCb
)
{
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   WrUeCb                    *ueCb = transCb->ueCb;
   
   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /*Fill the Configuration Request */
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   ueRecfg->ueRecfgTypes |= RGR_UE_MEASGAP_RECFG;

   /* Based on the MeasGap Status fill the gapMeasInfo of 
      cfgReq 
   */
   if ( transCb->u.anrMeasTrans.measTransCfg.measGapStatus == 
                                       WR_UMM_MEAS_GAP_INIT_SCH_CONFIG)
   {
      ueRecfg->ueMeasGapRecfg.isMesGapEnabled = TRUE;
      if (ueCb->measCfg->gapMeasInfo.gapPattern == 
                                             WR_UMM_MEAS_GAP_PERD_40MS )
      {
         ueRecfg->ueMeasGapRecfg.gapPrd =
                                              WR_UMM_MEAS_GAP_PERD_40MS_VAL;
      }
      else if ( ueCb->measCfg->gapMeasInfo.gapPattern == 
                                             WR_UMM_MEAS_GAP_PERD_80MS )
      {
         ueRecfg->ueMeasGapRecfg.gapPrd = WR_UMM_MEAS_GAP_PERD_80MS_VAL;
      }
      ueRecfg->ueMeasGapRecfg.gapOffst = 
                       ueCb->measCfg->gapMeasInfo.measGapOffset;
   }
   else if (transCb->u.anrMeasTrans.measTransCfg.measGapStatus == WR_UMM_MEAS_GAP_REL)
   {
      ueRecfg->ueMeasGapRecfg.isMesGapEnabled = FALSE;
   }
   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transCb->transId);
   RETVALUE(ROK);
}

/** @brief This function is responsible for reconfiguring the UE DRX config
  at the scheduler.
 *
 * @details
 *
 *     Function: wrUmmAnrDrxSchdUeRecfgReq
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
PRIVATE S16 wrUmmAnrDrxSchdUeRecfgReq
(
 WrUmmTransCb                 *transCb
)
{
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   U32                       transId;
   WrUmmAnrMeasTransInfo     *anrMeasTransCb = &transCb->u.anrMeasTrans;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmCellCb               *cellCb;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL.");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RETVALUE(RFAILED);
   }
   transId = transCb->transId;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = ueCb->cellId;
   ueRecfg->oldCrnti = ueCb->crnti;
   ueRecfg->newCrnti = ueCb->crnti;

   wrUmmSchdFillUeDrxCfgInfo(ueRecfg, anrMeasTransCb->drxCfgPtr);

   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}
/*Removed DRX flag*/
/** @brief This function is responsible for configuring Anr Measurement for
 *  Utran Frequency.
 * @details
 *
 *  Function:wrUmmAnrMeasUtranCfg
 *
 *  Processing steps:
 *    - Configure Strongest for ANR or Report CGI for Utran Frequency
 *
 *  @param[out]  anrMeasIntMsg: pointer to internal structure
 *  @param[in]  measUeCb: pointer measurement UE CB
 *  @param[in]  measTransCb: pointer to the Measurement Transaction CB.
 *  @param[in]  cellCb: pointer to the Cell control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmAnrMeasUtranCfg
(
 WrUmmIntAnrMeasMsg          *anrMeasIntMsg,
 WrUeCb                      *ueCb,/*rel10*/
 WrUmmAnrMeasTransInfo       *measTransCb,
 WrUmmCellCb                 *cellCb
)
{
   S16                          ret = -1;
   U8                           measId;
   U8                           measObjId = 0;
   U8                           rptCfgId = 0;
   LwrCellMeasCfgGrp            *measCellCfg;
   WrUmmMeasUtraMeasObjNode     *utraCellObj;
   WrUmmMeasObjInfo             measObjInfo;
   WrUmmMeasRmvInfo             *rmvMeasInfo;
   WrUmmMeasAddInfo             *addMeasInfo;
   WrUmmMeasRptCfgInfo          rptCfgInfo;
   WrMeasUeCb                  *measUeCb;/*rel10*/

   TRC2(wrUmmAnrMeasUtranCfg)
   measCellCfg = cellCb->measCellCfg;
   
   rmvMeasInfo = &measTransCb->measTransCfg.rmvMeasInfo; 
   addMeasInfo = &measTransCb->measTransCfg.addMeasInfo;
   /*rel10*/
   measUeCb = ueCb->measCfg;

   if(measUeCb->rmvAnrMeasId != 0)
   {
      rmvMeasInfo->measIdRmvLst[rmvMeasInfo->measIdRmvLstCnt] = measUeCb->rmvAnrMeasId;
      rmvMeasInfo->measIdRmvLstCnt++;
      measUeCb->rmvAnrMeasId = 0;
   }
   if(anrMeasIntMsg->rptCfgType == WR_UMM_MEAS_UTRA_STRONG_CELL_ANR)
   {
      /* Meas Object for Strongest cell */
      if(ROK != wrUmmMeasUpdtUtraMeasObjAndIdFromArfcn(anrMeasIntMsg->
               arfcn.t.carrierFreq, measUeCb, &measObjId, &utraCellObj))
      {
         wrUmmMeasUpdtCellUtraMeasObjFrmArfcn(anrMeasIntMsg->arfcn.t.carrierFreq,
               &cellCb->measCellCb->utraFddMeasObjLst, &utraCellObj);

         measObjId =  wrUmmMeasAddUtraMeasObjToUeCb(measUeCb, utraCellObj, 0xFFFF);
         if(measObjId != WR_UMM_MEAS_INVALID_MEAS_ID)
         {
            addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
            addMeasInfo->measObjIdCnt++;
         }
         else
         {
            RLOG1(L_ERROR, "Invalid MeasObjID[%d] received",measObjId);
            RETVALUE(RFAILED);
         }
      }
      /* Report Config for Strongest cell */
      rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_UTRA_STRONG_CELL_ANR;
      rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;
      rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
      rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

      if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_PERD_ANR, measObjId,
               &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
      {
         RLOG0(L_ERROR, "Invalid report and Meas config Id");
         RETVALUE(RFAILED);
      }
   }
   else if(anrMeasIntMsg->rptCfgType == WR_UMM_MEAS_REPORT_CGI)
   {
      /* Meas Object for report CGI*/
      if(ROK != wrUmmMeasUpdtUtraMeasObjAndIdFromArfcn(anrMeasIntMsg->
               arfcn.t.carrierFreq, measUeCb, &measObjId, &utraCellObj))
      {
         wrUmmMeasUpdtCellUtraMeasObjFrmArfcn(anrMeasIntMsg->arfcn.t.carrierFreq,
               &cellCb->measCellCb->interMeasObjLst, &utraCellObj);

         measObjId =  wrUmmMeasAddUtraMeasObjToUeCb(measUeCb, utraCellObj,
                                                    anrMeasIntMsg->newPci);
         if(measObjId != WR_UMM_MEAS_INVALID_MEAS_ID)
         {
            addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
            addMeasInfo->measObjIdCnt++;
         }
         else
         {
            RLOG1(L_ERROR, "Invalid MeasObjID[%d] received",measObjId);
            RETVALUE(RFAILED);
         }
      }
      else
      {
         wrUmmMeasFindMeasObjAndTyp (&measObjInfo, measObjId, measUeCb);
         measTransCb->measTransCfg.addMeasInfo.isCellsToBeSkip = TRUE;
         measObjInfo.utraFddObj->newPsc = anrMeasIntMsg->newPci;
         addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
         addMeasInfo->measObjIdCnt++;
      }
      /*Report config for EUTRA ReportCGI*/
      if(NULLP ==  measUeCb->utraRptCgiCfg)
      {
         if(ret == WR_UMM_MEAS_INVALID_MEAS_ID)
         {
            RLOG0(L_ERROR, "Report config not available ");
            RETVALUE(ret);
         }
         WR_ALLOC(&measUeCb->utraRptCgiCfg, sizeof(WrMeasRptCfgNode));
         if(NULLP == measUeCb->utraRptCgiCfg)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            wrUmmMeasSetFreeRptCfgId(measUeCb, rptCfgId);
            RETVALUE(RFAILED);
         }
         rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_UTRA_REPORT_CGI;
         rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;
         rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
         rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

         wrUmmMeasCreateRptCfg(rptCfgId, &rptCfgInfo, measUeCb->utraRptCgiCfg,
                              cellCb->measCellCfg,ueCb);
         measTransCb->measTransCfg.addMeasInfo.isUtraRptCgiFlg = TRUE;
      }
      else
      {
         rptCfgId = measUeCb->utraRptCgiCfg->rptCfgId;
         measTransCb->measTransCfg.addMeasInfo.isUtraRptCgiFlg = FALSE;
      }
      measUeCb->isRptCgiCfg = TRUE;

      /* Meas ID for Report CGI*/
      measId = wrUmmMeasCreatMeasIdNode(WR_UMM_MEAS_TYPE_PERD_ANR, measObjId,
                rptCfgId, measUeCb);
      if(measId != WR_UMM_MEAS_INVALID_MEAS_ID)
      {
         addMeasInfo->measIdLst[addMeasInfo->measIdCnt] = measId;
         addMeasInfo->measIdCnt++;
      }
      else
      {
         RLOG1(L_ERROR, "Invalid MeasID[%d] received",measId);
         /* ccpu00132002 Added check for validating the ReportCgi configuration */
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
 *  Function:wrUmmAnrMeasInterCfg
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
PRIVATE S16 wrUmmAnrMeasInterCfg
(
 WrUmmIntAnrMeasMsg          *anrMeasIntMsg,
 WrUeCb                      *ueCb,
 WrUmmAnrMeasTransInfo       *measTransCb,
 WrUmmCellCb                 *cellCb
)
{
   S16                          ret;
   U8                           measId;
   U8                           measObjId;
   U8                           rptCfgId;
   LwrCellMeasCfgGrp            *measCellCfg;
   WrUmmMeasEutraMeasObjNode    *eutraCellObj;
   WrUmmMeasObjInfo             measObjInfo;
   WrUmmMeasRmvInfo             *rmvMeasInfo;
   WrUmmMeasAddInfo             *addMeasInfo;
   WrUmmMeasRptCfgInfo          rptCfgInfo;
   WrMeasUeCb                   *measUeCb;
   Bool                         isGapNeed = FALSE;

   TRC2(wrUmmAnrMeasInterCfg)

   measCellCfg = cellCb->measCellCfg;
   measUeCb = ueCb->measCfg;
   rmvMeasInfo = &measTransCb->measTransCfg.rmvMeasInfo; 
   addMeasInfo = &measTransCb->measTransCfg.addMeasInfo;
   measUeCb->measIdForR1 = 0;

   if(measUeCb->rmvAnrMeasId != 0)
   {
      /* Add the Measurement ID in the remove Meas List */
      rmvMeasInfo->measIdRmvLst[rmvMeasInfo->measIdRmvLstCnt] = measUeCb->rmvAnrMeasId;
      rmvMeasInfo->measIdRmvLstCnt++;
      measUeCb->rmvAnrMeasId = 0;
   }

   /* If the report config type is Strong Cell */
   if(anrMeasIntMsg->rptCfgType == WR_UMM_MEAS_STRONG_CELL)
   {
      /* Meas Object for Strongest cell */
      if(ROK != wrUmmMeasUpdtEutraMeasObjAndIdFromArfcn(anrMeasIntMsg->
               arfcn.t.carrierFreq, measUeCb, &measObjId, &eutraCellObj))
      {
         /* Measurement object is not present in the list 
           Create New Measurement object for this carrier frequency*/
         wrUmmMeasUpdtCellEutraMeasObjFrmArfcn(anrMeasIntMsg->arfcn.t.carrierFreq,
               &cellCb->measCellCb->interMeasObjLst, &eutraCellObj);

         measObjId = wrUmmMeasAddInterMeasObjToUeCb(measUeCb, eutraCellObj, 0xFFFF);
         if(measObjId != WR_UMM_MEAS_INVALID_MEAS_ID)
         {
            addMeasInfo->isCellsToBeSkip = FALSE;
            addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
            addMeasInfo->measObjIdCnt++;
         }
         else
         {
            RLOG0(L_ERROR, "Invalid MeasObjID received");
            RETVALUE(RFAILED);
         }
      }
      /* populate the report config type */
      rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_STRONG_CELL;
      rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;
      rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
      rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

      /* Get the Report config ID and Meas ID for Strongest cell */
      if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_PERD_ANR, measObjId,
               &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
      {
         RLOG0(L_ERROR, "Invalid report and Meas config Id");
         RETVALUE(RFAILED);
      }

      /* Check Meas Gap is required for this UE */
      wrUmmChkUeNeedMeasGapForInterFreq(ueCb, anrMeasIntMsg->arfcn.t.carrierFreq, &isGapNeed);

      /*If Meas Gap is needed Configure it */
      if(TRUE == isGapNeed)
      {
         wrUmmMeasAddMeasGap(WR_UMM_MEAS_GAP_INTER_FREQ_ANR, cellCb, ueCb,
                             &measTransCb->measTransCfg.measGapStatus, 
                            ueCb->anrMeasTransId);
      }
      else
      {
        /* Release the measGap when this feature configured while processing 
         * previous frequency and not released */
        wrUmmMeasRelMeasGap(WR_UMM_MEAS_GAP_INTER_FREQ_ANR, cellCb, ueCb,
                          &measTransCb->measTransCfg.measGapStatus, 
                          ueCb->anrMeasTransId);
      }
   }
   else if(anrMeasIntMsg->rptCfgType == WR_UMM_MEAS_REPORT_CGI)
   {

      /* Release the meas Gap for this UE */ 
      wrUmmMeasRelMeasGap(WR_UMM_MEAS_GAP_INTER_FREQ_ANR, cellCb, ueCb,
                          &measTransCb->measTransCfg.measGapStatus, 
                          ueCb->anrMeasTransId);

      /* Meas Object for report CGI*/
      if(ROK != wrUmmMeasUpdtEutraMeasObjAndIdFromArfcn(anrMeasIntMsg->
               arfcn.t.carrierFreq, measUeCb, &measObjId, &eutraCellObj))
      {
         /* Measurement object is not present in the list 
           Create New Measurement object for this carrier frequency*/
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
            RLOG1(L_ERROR, "Invalid MeasObjID[%d] received",measObjId);
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

      /* Enable ANR DRX */
      measTransCb->isDrxReqForRptCgi = TRUE;

      /* check if the Report config for EUTRA ReportCGI already configured */
      if(NULLP ==  measUeCb->eutraRptCgiCfg)
      {
         /* Create the Report config ID */
         ret = wrUmmMeasGetFreeRptCfgId(measUeCb, &rptCfgId);
         if(ret == WR_UMM_MEAS_INVALID_MEAS_ID)
         {
            RLOG0(L_ERROR, "Report config not available ");
            RETVALUE(ret);
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
         /* Report config for EUTRA ReportCGI already configured */
         rptCfgId = measUeCb->eutraRptCgiCfg->rptCfgId;
         measTransCb->measTransCfg.addMeasInfo.isEutraRptCgiFlg = FALSE;
      }
      /* Report CGI config is about to send*/
      measUeCb->isRptCgiCfg = TRUE;

      /* Meas ID for Report CGI*/
      measId = wrUmmMeasCreatMeasIdNode(WR_UMM_MEAS_TYPE_PERD_ANR, measObjId,
            rptCfgId, measUeCb);
      if(measId != WR_UMM_MEAS_INVALID_MEAS_ID)
      {
         addMeasInfo->measIdLst[addMeasInfo->measIdCnt] = measId;
         addMeasInfo->measIdCnt++;
      }
      else
      {
         /* ccpu00132002 Added check for validating the ReportCgi configuration */
         RLOG1(L_ERROR, "Invalid MeasID[%d] received",measId);
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

/** @brief This function is responsible for Processing message for selected UE
 *
 * @details
 *
 *  Function:wrUmmAnrMeasPrcMsg
 *
 *  Processing steps:
 *    - Process measurement configuration
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmAnrMeasPrcMsg
(
 WrUmmTransCb                 *transCb,
 WrUmmIntAnrMeasMsg           *anrMeasIntMsg
)
{
   WrUeCb                       *ueCb = transCb->ueCb;
   WrUmmCellCb                  *cellCb;
   WrUmmAnrMeasTransInfo        *anrMeasTransCb = &transCb->u.anrMeasTrans;
   RgrCfgReqInfo                *cfgReq = NULLP;
   RgrUeRecfg                   *ueRecfg = NULLP;
   WrMeasUeCb                   *measUeCb  = ueCb->measCfg;
   U16                          longDrxTmrVal;

   TRC2(wrUmmAnrMeasPrcMsg)

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   wrUmmMeasResetTransCb(&anrMeasTransCb->measTransCfg);

   if(anrMeasIntMsg->choice == WR_UMM_ANR_MEAS_CREATE)
   {
      if(anrMeasIntMsg->cellTyp == WR_UMM_MEAS_INTRA_FREQ)
      {
         /* ccpu00132002 Added check for validating the ReportCgi configuration */
         if (RFAILED == wrUmmAnrMeasIntraCfg(anrMeasIntMsg,ueCb,anrMeasTransCb,cellCb))
         {
               RLOG0(L_ERROR, "Report CGI config is failed.");
               RETVALUE(RFAILED);
         }
      }
      else if(anrMeasIntMsg->cellTyp == WR_UMM_MEAS_INTER_FREQ)
      {
         /* ccpu00132002 Added check for validating the ReportCgi configuration */
         if( RFAILED == wrUmmAnrMeasInterCfg(anrMeasIntMsg,ueCb,anrMeasTransCb,cellCb))
         {
           RLOG0(L_ERROR, "Report CGI config is failed.");
              ueCb->anrMeasTransId = 0;
           wrUmmTransComplete(transCb);
           RETVALUE(RFAILED);
         }
      }
      else if(anrMeasIntMsg->cellTyp == WR_UMM_MEAS_UTRA_FDD_FREQ)
      {
         /* ccpu00132002 Added check for validating the ReportCgi configuration *//*rel10*/
         if( RFAILED == wrUmmAnrMeasUtranCfg(anrMeasIntMsg,ueCb,anrMeasTransCb,cellCb))
         {
            RLOG0(L_ERROR, "Report CGI config is failed.");
            RETVALUE(RFAILED);
         }
      }
   }

   /* Configuring Meas Objects and report configuration List*/
   /* DRX need to confgiure only If the UE is release 8*/
   if((anrMeasTransCb->isDrxReqForRptCgi == TRUE) &&
      (anrMeasTransCb->isDrxCfged == FALSE))
   {
      /* Fill/Update ANR related flags */
      anrMeasTransCb->isDrxCfged = TRUE;
      anrMeasTransCb->ueDrxCfg.isDrxEnabled = TRUE;
      anrMeasTransCb->isReCfgSent = TRUE;

      /* Populate DRX parameters in ANR transaction */
      anrMeasTransCb->ueDrxCfg.drxOnDurTmr =  
                                           wrEmmGetAnrDrxOnDur(ueCb->cellId);
      anrMeasTransCb->ueDrxCfg.drxLongCycleOffst.longDrxCycle = 
                                           wrEmmGetAnrDrxLongCycle(ueCb->cellId);
      /*TODO need to ensure that the drx start offset falls 
       within the long cycle offset */
      longDrxTmrVal = 
                  wrDrxLongCycle[anrMeasTransCb->ueDrxCfg.drxLongCycleOffst.longDrxCycle];

      anrMeasTransCb->ueDrxCfg.drxLongCycleOffst.drxStartOffst = 
                                (ueCb->cqiOffst - 2 + longDrxTmrVal) % longDrxTmrVal; 

      anrMeasTransCb->ueDrxCfg.drxInactvTmr = wrEmmGetAnrDrxInActivTmrVal(ueCb->cellId);
      anrMeasTransCb->ueDrxCfg.drxRetxTmr = WR_UMM_ANR_DRX_RETRANS_TMR_VAL;
      anrMeasTransCb->ueDrxCfg.drxShortDrx.pres = FALSE;
      anrMeasTransCb->drxCfgPtr = &anrMeasTransCb->ueDrxCfg;
      
      /* Prepare configure request for sending message to the scheduler */ 
      WR_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));
      if (cfgReq == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
      cfgReq->action = RGR_RECONFIG;
      cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
      ueRecfg->cellId = ueCb->cellId;
      ueRecfg->oldCrnti = ueCb->crnti;
      ueRecfg->newCrnti = ueCb->crnti;

      /* Fill DRX related parameters in the Configure message */
      wrUmmSchdFillUeDrxCfgInfo(ueRecfg, anrMeasTransCb->drxCfgPtr);

      /* Check wheather measurement gap configuration required  at the scheduer */
      if (measUeCb->gapMeasInfo.latestTxId == transCb->transId) 
      {

         /* If Gap Status is scheduler configuration then 
            fill the ueReCfg 
         */
         if (anrMeasTransCb->measTransCfg.measGapStatus ==
                                          WR_UMM_MEAS_GAP_INIT_SCH_CONFIG)
         {
            ueRecfg->ueRecfgTypes |= RGR_UE_MEASGAP_RECFG;
            ueRecfg->ueMeasGapRecfg.isMesGapEnabled = TRUE;
            /* only possibilty(this Feature configured for 40 other might have
             * configured for 80ms and now 40 is released) 
             * is configuring the 80MS */
            if ( measUeCb->gapMeasInfo.gapPattern == 
                                               WR_UMM_MEAS_GAP_PERD_80MS )
            {
               ueRecfg->ueMeasGapRecfg.gapPrd = 
                                            WR_UMM_MEAS_GAP_PERD_80MS_VAL;
            }
            ueRecfg->ueMeasGapRecfg.gapOffst = 
                               measUeCb->gapMeasInfo.measGapOffset;
         }
      }
      /* Send the message to the scheduler */
      WrIfmSchdCfgUe(cfgReq, transCb->transId);
   }
   else
   {
      /* DRX is not configured. set the flag. */
      anrMeasTransCb->ueDrxCfg.isDrxEnabled = FALSE;

      /* Check is the measurement gap is configured*/
      if( (measUeCb->gapMeasInfo.latestTxId == transCb->transId) && 
            (anrMeasTransCb->measTransCfg.measGapStatus == 
                                          WR_UMM_MEAS_GAP_INIT_SCH_CONFIG))
      {
         /*Measurement gap is configured so send the message to scheduler
           By storing the prepared RRC message
         */
         wrUmmAnrMeasGapSchdUeRecfgReq(transCb);
      }
      else
      {
         /* Measurement gap might have been configured to release. 
           In that case, update the isMeasGapCfg flag in the UE Cb to 
           Release */
         if( (measUeCb->gapMeasInfo.latestTxId == transCb->transId) && 
            (anrMeasTransCb->measTransCfg.measGapStatus == 
                                          WR_UMM_MEAS_GAP_REL))
         {
            ueCb->measCfg->gapMeasInfo.isMeasGapCfg = WR_UMM_MEAS_GAP_REL;
         }

         /*Prepare and  Send RRC Reconfiguration  message to UE */
         if(ROK != wrUmmMeasPrepareSendRrcRecfg(ueCb->cellId, 
                                          ueCb, &anrMeasTransCb->measTransCfg, 
                                          transCb->transId))
         {
            RLOG0(L_ERROR, "Deallocating RRC Reconfg Message");
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is used to select the new PCI Utran frequency for
 * UE in ANR Measurement UE selection algorithm.
 *
 * @details
 *
 *     Function: wrUmmAnrMeasUtranNewPciCfg
 *     
 *         Processing steps:
 *     1) If new PCI for Utran frequency is availble select for UE
 *
 * @param[in]  utranNewPciLst : pointer to inter New PCI Linked List.
 * @return WrUmmIntAnrMeasMsg*
 *    -#Success : WrUmmIntAnrMeasMsg
 *    -#Failure : NULLP
 */
PRIVATE WrUmmIntAnrMeasMsg* wrUmmAnrMeasUtranNewPciCfg
(
 CmLListCp                     *utranNewPciLst
)
{
   WrUmmIntAnrMeasMsg           *anrMeasIntMsg = NULLP;
   CmLList                      *utraNewPciLnk;
   CmLList                      *newPciLnk;
   WrMeasNewPciNode             *utranNewPciNode;
   WrUmmMeasNewPciInfo          *newPciObj;
   TRC2(wrUmmAnrMeasUtranNewPciCfg)

   for (utraNewPciLnk = utranNewPciLst->first;
        NULLP != utraNewPciLnk;
        utraNewPciLnk = utraNewPciLnk->next)
   {
      utranNewPciNode = (WrMeasNewPciNode*)utraNewPciLnk->node;

      newPciLnk = utranNewPciNode->newPciLst.first;
      while (NULLP != newPciLnk)
      {
         newPciObj = (WrUmmMeasNewPciInfo*)newPciLnk->node;

         if(newPciObj->isConfigred == WR_UMM_CONFIGED)
         {
            newPciLnk = newPciLnk->next;
            continue;
         }
         else
         {
            WR_ALLOC (&anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg))
            if(NULLP == anrMeasIntMsg)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(NULLP);
            }
            newPciObj->isConfigred = WR_UMM_CONFIGED;

            anrMeasIntMsg->choice = WR_UMM_ANR_MEAS_CREATE;
            anrMeasIntMsg->arfcn.choice = WR_UMM_MEAS_UTRA_FDD_FREQ;
            anrMeasIntMsg->arfcn.t.carrierFreq = utranNewPciNode->arfcn;
            anrMeasIntMsg->cellTyp = WR_UMM_MEAS_UTRA_FDD_FREQ;
            anrMeasIntMsg->rptCfgType = WR_UMM_MEAS_REPORT_CGI;
            anrMeasIntMsg->newPci = newPciObj->newPci;
            break;
         }
         newPciLnk = newPciLnk->next;
      }
   }
   RETVALUE(anrMeasIntMsg);
}

/** @brief This function is used to select the New PCI measurement configuration
 * for reportCGI.
 *
 * @details
 *
 *     Function: wrUmmAnrMeasNewPciCfg
 *
 *         Processing steps:
 *     1) start UE selection algorith based on ue capabilities
 *
 * @param[in]  ueCb : pointer to UE control block
 * @return 
 *      pointer to wrUmmIntAnrMeasMsg.
 */
PRIVATE WrUmmIntAnrMeasMsg* wrUmmAnrMeasNewPciCfg
(
  WrUeCb                      *ueCb,
  U8                          anrCfgType
)
{
   WrUmmIntAnrMeasMsg        *anrMeasTmpMsg = NULLP;
   WrMeasUeCb                *measUeCb = NULLP;

   TRC2(wrUmmAnrMeasNewPciCfg)

   measUeCb = ueCb->measCfg;

   switch(anrCfgType)
   {
       case WR_UMM_MEAS_INTRA_FREQ:
       {
          anrMeasTmpMsg = wrUmmAnrMeasIntraNewPciCfg(ueCb);
     break;
       }
       case WR_UMM_MEAS_INTER_FREQ:
       {
          anrMeasTmpMsg = wrUmmAnrMeasInterNewPciCfg(&measUeCb->interNewPciLst);
     break;
       }
       case WR_UMM_MEAS_UTRA_FDD_FREQ:
       {
          anrMeasTmpMsg = wrUmmAnrMeasUtranNewPciCfg(&measUeCb->utranNewPscLst);
     break;
       }
       default :
          break;
   }

   RETVALUE(anrMeasTmpMsg);
}

/** @brief This function is used to select the intra frequency for UE in ANR
 *  Measurement UE selection algorithm.
 *
 * @details
 *
 *     Function: wrUmmAnrMeasIntraPerdCfg
 *
 *         Processing steps:
 *     1) If Intra frequency not configured select intra frquency for UE
 *
 * @param[out]  anrMeasInfo : pointer to WrUmmAnrMeasInfo structure.
 * @return WrUmmIntAnrMeasMsg*
 *    -#Success : WrUmmIntAnrMeasMsg
 *    -#Failure : NULLP
 */
PRIVATE WrUmmIntAnrMeasMsg* wrUmmAnrMeasIntraPerdCfg
(
 WrUmmAnrMeasInfo               *anrMeasInfo
)
{
   WrUmmIntAnrMeasMsg          *anrMeasIntMsg = NULLP;

   TRC2(wrUmmAnrMeasIntraPerdCfg)

   WR_ALLOC (&anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg))
   if(NULLP == anrMeasIntMsg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(NULLP);
   }

   anrMeasIntMsg->choice = WR_UMM_ANR_MEAS_CREATE;
   anrMeasIntMsg->arfcn.choice = WR_UMM_MEAS_INTRA_FREQ;
   anrMeasIntMsg->arfcn.t.carrierFreq = anrMeasInfo->intraFreq.val;
   anrMeasIntMsg->cellTyp = WR_UMM_MEAS_INTRA_FREQ;
   anrMeasIntMsg->rptCfgType = WR_UMM_MEAS_STRONG_CELL;
   anrMeasIntMsg->newPci = 0xFFFF;
   RETVALUE(anrMeasIntMsg);
}

/** @brief This function is used to select the inter frequency for UE in ANR
 *  Measurement UE selection algorithm.
 *
 * @details
 *
 *     Function: wrUmmAnrMeasInterPerdCfg
 *
 *         Processing steps:
 *     1) If Inter frequency not configured select intra frquency for UE
 *
 * @param[in]  ueCb : pointer to Ue control block.
 * @return WrUmmIntAnrMeasMsg*
 *    -#Success : WrUmmIntAnrMeasMsg
 *    -#Failure : NULLP
 */
PRIVATE WrUmmIntAnrMeasMsg* wrUmmAnrMeasInterPerdCfg
(
 WrUeCb                      *ueCb
)
{
   WrUmmIntAnrMeasMsg          *anrMeasIntMsg = NULLP;
   U8                           targetBand;
   U8                           idCnt = 0;
   U16                          earfcn;
   Bool                         isUeSupported = FALSE;
   WrMeasUeCb                   *measUeCb = ueCb->measCfg;
   WrUmmAnrMeasInfo             *anrMeasInfo = &measUeCb->ueAnrMeasInfo;

   TRC2(wrUmmAnrMeasInterPerdCfg)

   /* The Index should start from the previous index */
   for(idCnt = anrMeasInfo->currAnrInterFreqCnt;idCnt < anrMeasInfo->interArfcnCnt;idCnt++)
   {
      /*Meas Objects for inter frequencies*/
      earfcn = anrMeasInfo->interArfcn[idCnt];
      /*Get bandclass for corresponding ARFCN*/
      wrUmmFndEUTRABand(earfcn,&targetBand);

      /*Check whether the bandclass is UE supported or not*/
      isUeSupported = wrUmmEUTRABandSupported(ueCb,targetBand, 0);

      if(isUeSupported == TRUE)
      {
         /* Store the Index value */
         anrMeasInfo->currAnrInterFreqCnt = ++idCnt; 

         /* Create Anr Meas Int Message */
         WR_ALLOC (&anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg))
         if(NULLP == anrMeasIntMsg)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(NULLP);
         }
         anrMeasIntMsg->choice = WR_UMM_ANR_MEAS_CREATE;
         anrMeasIntMsg->arfcn.choice = WR_UMM_MEAS_INTER_FREQ;
         anrMeasIntMsg->arfcn.t.carrierFreq = earfcn;
         anrMeasIntMsg->cellTyp = WR_UMM_MEAS_INTER_FREQ;
         anrMeasIntMsg->rptCfgType = WR_UMM_MEAS_STRONG_CELL;
         anrMeasIntMsg->newPci = 0xFFFF;

         RETVALUE(anrMeasIntMsg);
      }
   }

   anrMeasInfo->currAnrInterFreqCnt = idCnt;

   RETVALUE(anrMeasIntMsg);
}

/** @brief This function is used to select the utran frequency for UE in ANR
 *  Measurement UE selection algorithm.
 *
 * @details
 *
 *     Function: wrUmmAnrMeasUtraStrongCellCfg
 *
 *         Processing steps:
 *     1) If utra frequency not configured select intra frquency for UE
 *
 * @param[out]  anrMeasInfo : pointer to WrUmmAnrMeasInfo structure.
 * @return WrUmmIntAnrMeasMsg*
 *    -#Success : WrUmmIntAnrMeasMsg
 *    -#Failure : NULLP
 */
PRIVATE WrUmmIntAnrMeasMsg* wrUmmAnrMeasUtraStrongCellCfg
(
 WrUeCb                      *ueCb
)
{
   U8                           idCnt = 0;
   U8                           servingBand;
   U8                           targetBand = 0;
   U16                          servFreq;
   U16                          uarfcn;
   Bool                         isUeSupported = FALSE;
   WrMeasUeCb                   *measUeCb = ueCb->measCfg;
   WrUmmIntAnrMeasMsg           *anrMeasIntMsg = NULLP;
   WrUmmAnrMeasInfo             *anrMeasInfo = &measUeCb->ueAnrMeasInfo;

   TRC2(wrUmmAnrMeasUtraStrongCellCfg)
   servFreq = wrEmmGetServCarrierFreq(ueCb->cellId);
   /*Get serving bandclass */
   wrUmmFndEUTRABand(servFreq,&servingBand);
   
   for(;idCnt < anrMeasInfo->utranArfcnCnt;idCnt++)
   {
      /*Meas Objects for inter frequencies*/
      uarfcn = anrMeasInfo->utranArfcn[idCnt];
      
      /*Get bandclass for corresponding ARFCN*/
      wrUmmFndUTRABandFDD(uarfcn,&targetBand);

      /*Check whether the bandclass is UE supported or not*/
      isUeSupported = wrUmmUTRAFDDBandSupported(ueCb,targetBand);
      
      if(isUeSupported == TRUE)
      {
         WR_ALLOC (&anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg))
         if(NULLP == anrMeasIntMsg)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(NULLP);
         }
         anrMeasIntMsg->choice = WR_UMM_ANR_MEAS_CREATE;
         anrMeasIntMsg->arfcn.choice = WR_UMM_MEAS_UTRA_FDD_FREQ;
         anrMeasIntMsg->arfcn.t.carrierFreq = uarfcn;
         anrMeasIntMsg->cellTyp = WR_UMM_MEAS_UTRA_FDD_FREQ;
         anrMeasIntMsg->rptCfgType = WR_UMM_MEAS_UTRA_STRONG_CELL_ANR;
         anrMeasIntMsg->newPci = 0xFFFF;
         break;
      }
   }
   RETVALUE(anrMeasIntMsg);
}

/** @brief This function is used to select the measurement configuration for
 * UE in ANR Measurement UE selection algorithm.
 *
 * @details
 *
 *     Function: wrUmmCfgAnrMeasForUe
 *
 *         Processing steps:
 *     1) start UE selection algorith based on ue capabilities
 *
 * @param[in]  ueCb : pointer to UE control block
 * @param[in]  anrCfgType : ANR Configuration Type
 * @param[out] anrMeasIntMsg : pointer to WrUmmIntAnrMeasMsg structure.
 * @return
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmCfgAnrMeasForUe
(
 WrUeCb                      *ueCb,
 WrUmmIntAnrMeasMsg          **anrMeasIntMsg,
 U8                          anrCfgType
)
{
   WrUmmIntAnrMeasMsg        *anrMeasTmpMsg = NULLP;
   WrMeasUeCb                *measUeCb = ueCb->measCfg;

   TRC2(wrUmmCfgAnrMeasForUe)

   /* Mark the input parameter as NULL so that it will not be holding old 
    * pointer in case of multi-UE
    */
   *anrMeasIntMsg = NULLP;

   anrMeasTmpMsg = (WrUmmIntAnrMeasMsg*)wrUmmAnrMeasNewPciCfg(ueCb, anrCfgType);
   if(anrMeasTmpMsg != NULLP)
   {
      *anrMeasIntMsg = anrMeasTmpMsg;
      RETVALUE(ROK);
   }

   switch(anrCfgType)
   {
      case  WR_UMM_MEAS_INTRA_FREQ:
      {
         anrMeasTmpMsg = (WrUmmIntAnrMeasMsg*)wrUmmAnrMeasIntraPerdCfg(&measUeCb->ueAnrMeasInfo);
    break;
      }
      case  WR_UMM_MEAS_INTER_FREQ:
      {
         anrMeasTmpMsg = (WrUmmIntAnrMeasMsg*)wrUmmAnrMeasInterPerdCfg(ueCb);
    break;
      }
      case WR_UMM_MEAS_UTRA_FDD_FREQ:
      {
         anrMeasTmpMsg = wrUmmAnrMeasUtraStrongCellCfg(ueCb);
    break;
      }
      default:
         break;
   }

   *anrMeasIntMsg = anrMeasTmpMsg;
   RETVALUE(ROK);
}


/** @brief This function is used to selecting ANR configuration for selected UE
 *
 * @details
 *
 *     Function: wrUmmAnrSelMeasConfigForUe
 *
 *         Processing steps:
 *     1) Select the ANR config 
 *
 * @param[in]  ueCb : pointer to UE Cb
 * @return
 *    -#Success : WrUmmIntAnrMeasMsg*
 *    -#Failure : NULLP
 */
PRIVATE WrUmmIntAnrMeasMsg* wrUmmAnrSelMeasConfigForUe
(
 WrUeCb                      *ueCb
)
{
   WrUmmIntAnrMeasMsg        *anrMeasIntMsg = NULLP;
   TRC2(wrUmmAnrSelMeasConfigForUe)
   
   wrUmmCfgAnrMeasForUe(ueCb, &anrMeasIntMsg, ueCb->measCfg->ueAnrMeasInfo.currAnrCfgType);

   RETVALUE(anrMeasIntMsg);  
}

/** @brief This function is responsible for handling report CGI message for
 * WCDMA
 *
 * @details
 *
 *  Function:wrUmmAnrMeasHdlUtraRptCgi
 *
 *  Processing steps:
 *    - Process report CGI measurement report
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmAnrMeasHdlUtraRptCgi
(
  U8                           recvMeasId,
  WrUmmTransCb                 *transCb,
  WrUmmMeasObjInfo             *measObjInfo,
  NhuMeasResults               *measRprt
)
{
    WrUmmIntAnrMeasMsg         *anrMeasIntMsg = NULLP;
    WrMeasUeCb                 *measUeCb;
    WrUmmCellCb                *cellCb;
    WrUeCb                     *ueCb;
    NhuMeasResultUTRA          *utraNhuObj;

    TRC2(wrUmmAnrMeasHdlUtraRptCgi)

    ueCb = transCb->ueCb;
    measUeCb = ueCb->measCfg;

    WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
    if(NULLP == cellCb)
    {
       RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId, "Error in getting CELL CB");
       RETVALUE(RFAILED);
    }
    utraNhuObj = &measRprt->measResultNeighCells.val.measResultLstUTRA.member[0];

    measUeCb->isRptCgiCfg = FALSE;
    if(utraNhuObj->cgi_Info.pres.pres == TRUE)
    {
       wrEmmMeasReportCgiHdlr(ueCb->cellId, measObjInfo->nrType, 
           measObjInfo->utraFddObj->utraMeasObj->arfcn, measRprt,
           ueCb->ueCap->eUTRA_Cap.accessStratumRls.val);
    }
    wrUmmMeasCfgDelNewPciObj(measObjInfo, measUeCb);

    /* ccpu00132002 Fix -> Delete ReportCgi meas Id from UeCb */
    wrUmmMeasRmvMeaIdInUeCb(recvMeasId, &measUeCb->measIdLst);
    wrUmmMeasSetFreeMeasId(measUeCb, recvMeasId);

    anrMeasIntMsg = wrUmmAnrSelMeasConfigForUe(ueCb);
    if(anrMeasIntMsg != NULLP)
    {
       wrUmmAnrMeasPrcMsg(transCb, anrMeasIntMsg);
    }
    else
    {
       ueCb->measCfg->isAnrMeasCfged = WR_UMM_NOT_CONFIGED;
    }
    RETVALUE(ROK);
}

/** @brief This function initiates releasing of 
 *         Meas Id and Meas gap process
 *
 * @details
 *
 * Function:wrUmmAnrRelMeasGapMeasId
 *
 *   Processing steps:
 *        Inform MeasGap module about releasing the measurment gap.
 *        If no Meas Id and Meas Gap to be released then exit.
 *        If the measGapstatus is assigned to REL or if the measGap 
 *        doesn't give control to configure meas gap  then send message 
 *         to the UE else if the measGapStatus is assigned to SCH_CONFIG
 *         then send message to sch.
 *
 * @param[in]  transCb pointer to Transaction control block
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmAnrRelMeasGapMeasId 
( 
  WrUmmTransCb  *transCb
)
{
    WrMeasUeCb                   *measUeCb;
    WrUmmMeasRmvInfo             *rmvMeasInfo;
    WrUmmCellCb                  *cellCb;
    WrUeCb                       *ueCb;

    TRC2(wrUmmAnrRelMeasGapMeasId)

    /* Initialize the pointers declared */
    rmvMeasInfo = &transCb->u.anrMeasTrans.measTransCfg.rmvMeasInfo; 
    ueCb = transCb->ueCb;
    measUeCb = ueCb->measCfg;

    WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
    if(NULLP == cellCb)
    {
       RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId, 
                     "wrUmmAnrRelMeasGapMeasId: Error in getting CELL CB");
       RETVALUE(RFAILED);
    }

    /* Release the meas Gap for this UE */ 
    wrUmmMeasRelMeasGap(WR_UMM_MEAS_GAP_INTER_FREQ_ANR, cellCb, ueCb, 
                        &transCb->u.anrMeasTrans.measTransCfg.measGapStatus, 
                        transCb->transId);
    /* If no Measurement Id to be removed and No measuremnt gap to be released,
     * enodeb shall close ANR transaction*/
    if((measUeCb->rmvAnrMeasId == 0) &&
        (measUeCb->gapMeasInfo.latestTxId != transCb->transId)) 
    {
       /* No Meas Id & Meas Gap to be removed. */
       wrUmmTransComplete(transCb);
       RETVALUE(ROK);
    }

    /* Populate RRC Reconfiguration message with the Meas Id and 
    Meas Gap(if required) */
    if((measUeCb->gapMeasInfo.latestTxId == transCb->transId) && 
       (transCb->u.anrMeasTrans.measTransCfg.measGapStatus == WR_UMM_MEAS_GAP_INIT_SCH_CONFIG))
    {
       /* Measurement gap is configured so send the message to scheduler
       by storing the RRC message pointer */
       wrUmmAnrMeasGapSchdUeRecfgReq(transCb);
    }
    else 
    {
       if((measUeCb->gapMeasInfo.latestTxId == transCb->transId) &&  
          (transCb->u.anrMeasTrans.measTransCfg.measGapStatus ==
                                                       WR_UMM_MEAS_GAP_REL))
       {
          /* Update the meas Gap flag in the UE Cb */
          measUeCb->gapMeasInfo.isMeasGapCfg = WR_UMM_MEAS_GAP_REL;
       }
       /* Measurement gap is released so send the message to the UE 
       Initiate preparing the RRC Reconfiguration message */
       wrUmmMeasPrepareSendRrcRecfg(ueCb->cellId, 
                                    ueCb, 
                                    &transCb->u.anrMeasTrans.measTransCfg,
                                    transCb->transId);

       wrStartTransTmr(transCb,WR_TMR_RECONFIGURE,
                                      wrEmmGetRrcReCfgTmrVal(ueCb->cellId));
    }
    RETVALUE(ROK);
}

/** @brief This function is responsible for Handling measurement report
 * received from UE.
 *
 * @details
 *
 * Function:wrUmmAnrMeasHdlMeasRpt
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
PRIVATE S16 wrUmmAnrMeasHdlMeasRpt
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
   WrUmmAnrMeasTransInfo        *measTransCb;
   WrUmmMeasObjInfo             measObjInfo;
   WrUmmCellCb                  *cellCb;
   WrUeCb                       *ueCb;
   U16                          cellId;
   U8                           ueCapAnrSupport = FALSE;
   NhuDatReqSdus                *rrcReCfgMsg;
   WrUmmMeasRmvInfo             *rmvMeasInfo;
   NhuMAC_MainConfig                *macCfg;

   TRC2(wrUmmAnrMeasHdlMeasRpt)

   ueCb = transCb->ueCb;
   cellId = ueCb->cellId;
   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId, "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
   measUeCb = ueCb->measCfg;
   measTransCb = &transCb->u.anrMeasTrans;
   rmvMeasInfo = &measTransCb->measTransCfg.rmvMeasInfo; 

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
         RLOG0(L_ERROR, "report config object not found");
         RETVALUE(RFAILED);
      }
   }
   switch (rptCfgTyp)
   {
      case WR_UMM_MEAS_STRONG_CELL:
      case WR_UMM_MEAS_UTRA_STRONG_CELL_ANR:
         {
            U16              reportAmt;
            U8               rptAmtEnum;
            U32              rptInt;

            /* If UE is in CSFB_READY state i.e CSFB is going on
             * then no action is taken on report strong cells*/
            if(WR_CSFB_GSM_UE_STATE_CSFB_READY != ueCb->csfbInfo.csfbState)
            {
               RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti,"Strong Cell "
                     "measurement report received. Ignoring this report "
                     "as CSFB has been triggered for this UE");
               break;
            }

            RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti,"Received report for "
               "strongest cell");
           /* Store the measID to be removed */
           ueCb->measCfg->rmvAnrMeasId = recvMeasId;

           /* Add the received PCI information to the New PCI list only if no 
              other eNb App has initiated P-ANR stop procedure.
           */
           if(TRUE == measTransCb->closeTrns)
           {
              /* Transaction close has been initiated. So don't process the
                  incoming PCI information */ 
              break;
           }
           else 
           {
              /*Process the received new PCI List*/
              wrUmmFindNewPciFrmAnrMeasRpt(&measObjInfo,measRprt, transCb,
                                           FALSE);
           }
           measTransCb->anrPrdRptCnt++;
           rptAmtEnum = measCellCfg-> 
              measEutraPerdRptCfg[WR_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS].reportAmount;
           reportAmt = wrUmmMeasGetRptAmtFrmEnum(rptAmtEnum);

           if((measUeCb->isRptCgiCfg != TRUE) &&
                  (((measUeCb->intraNewPci.newPciLst.count != 0) || 
                    (measUeCb->interNewPciLst.count != 0)) ||
                   (measTransCb->anrPrdRptCnt >= reportAmt)))
           {
              /*If no PCI are there to process move to next frequency or RAT type */
              if((measTransCb->anrPrdRptCnt >= reportAmt) && 
                    (measUeCb->intraNewPci.newPciLst.count == 0) && 
                    (measUeCb->interNewPciLst.count == 0))
              {
                 /*ccpu00138058: Since all expected reports are received 
                  * i.e measTransCb->anrPrdRptCnt == reportAmt then no need to
                  * delete the measurement Id through RRC Re-Config message 
                  * for Intra or Inter Frequency as UE
                  * will delete this on its own */
                 wrUmmMeasRmvMeaIdInUeCb(recvMeasId, &measUeCb->measIdLst);
                 wrUmmMeasSetFreeMeasId(measUeCb, recvMeasId);
                 ueCb->measCfg->rmvAnrMeasId = 0;

                 wrUmmAnrSelNxtRatType(ueCb);
                 measTransCb->anrPrdRptCnt = 0;
                 ueCapAnrSupport = wrUmmAnrChkUeCap(ueCb, 
                                                    measUeCb->ueAnrMeasInfo.currAnrCfgType,
                                                    WR_UMM_STRONG_CELL_CFG);
              }
              else
              {
                 /* Added for the next condition */
                 ueCapAnrSupport = TRUE;
              }

              if(ueCapAnrSupport == TRUE)
              {
                 anrMeasIntMsg =(WrUmmIntAnrMeasMsg*) wrUmmAnrSelMeasConfigForUe(ueCb);
              }
              /* Process the message only if UE supports current frequency 
                 and anrMeasInternal message is created */
              if(anrMeasIntMsg != NULLP)
              {
                 wrUmmAnrMeasPrcMsg(transCb, anrMeasIntMsg);
                 WR_FREE(anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg));
                 if((1 == reportAmt) && (FALSE == measUeCb->isRptCgiCfg) && 
                   (WR_UMM_MEAS_GAP_NONE == 
                    transCb->u.anrMeasTrans.measTransCfg.measGapStatus))
                 {
                    /*Start timer WR_TMR_ANR_STRONGCELL_REPORT_R1*/
                    rptInt = wrGetRptInvlValFromEnum(measCellCfg->measEutraPerdRptCfg
                          [WR_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS].reportInterval);
                    wrStartTransTmr(transCb,WR_TMR_ANR_STRONGCELL_REPORT_R1,
                    rptInt + WR_TMR_ANR_STRONGCELL_RPRT_R1_DELTA); 
                 }
              }
              else
              {
                /* If measurement Gap is configured then release it at both 
                   the UE and scheduler and close the transaction.
                   If measurement gaps is enabled that means ANR DRX is not configured 
                 */
                 wrUmmMeasRmvMeaIdInUeCb(recvMeasId, &measUeCb->measIdLst);
                 wrUmmMeasSetFreeMeasId(measUeCb, recvMeasId);
                 measUeCb->rmvAnrMeasId = 0;
                 measTransCb->closeTrns = TRUE;
                 wrUmmAnrRelMeasGapMeasId(transCb);
              }
           }
           break;
         }
      case WR_UMM_MEAS_REPORT_CGI:
         {
            NhuMeasConfig                    *nhuMeasCfg;
            RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti,"Received report to "
                  "report cgi");

            /* 143301 */
            measUeCb->isRptCgiCfg = FALSE;

            /*Delete the PCI for which ECGI has been received from the PCI list */
            wrUmmMeasCfgDelNewPciObj(&measObjInfo, measUeCb);

            if((TRUE == measRprt->measResultNeighCells.choice.pres) &&
                  (0 != measRprt->measResultNeighCells.val.measResultLstEUTRA.noComp.val))
            {
               if(measRprt->measResultNeighCells.val.measResultLstEUTRA.member[0].
                     cgi_Info.pres.pres == TRUE)
               {
                  /* Valid CGI info has been received. Process it */
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
                  RLOG_ARG0(L_ERROR, DBG_CRNTI, ueCb->crnti,"Empty CGI report "
                        "Received");
               }
            }
            else
            {
               RLOG_ARG0(L_ERROR, DBG_CRNTI, ueCb->crnti,"Empty CGI report "
                     "Received");
            }
            /* ccpu00132002 Fix -> Delete ReportCgi meas Id from UeCb */
            wrUmmMeasRmvMeaIdInUeCb(recvMeasId, &measUeCb->measIdLst);
            wrUmmMeasSetFreeMeasId(measUeCb, recvMeasId);

            /*Any subsequent CGI report is not handled during CSFB*/
            if(WR_CSFB_GSM_UE_STATE_CSFB_READY != ueCb->csfbInfo.csfbState)
            {
               break;
            }
            RLOG_ARG2(L_DEBUG, DBG_CRNTI, ueCb->crnti,"num PCI in Intra-newPCI "
                  "list = %ld inter-newPci list = %ld",
                  measUeCb->intraNewPci.newPciLst.count,
                  measUeCb->interNewPciLst.count);
            /*Check is there more PCI to be processed */
            if((measUeCb->intraNewPci.newPciLst.count != 0) || 
                  (measUeCb->interNewPciLst.count != 0))
            {
               anrMeasIntMsg = 
                  (WrUmmIntAnrMeasMsg*) wrUmmAnrMeasNewPciCfg(ueCb, 
                        ueCb->measCfg->ueAnrMeasInfo.currAnrCfgType);
               if(anrMeasIntMsg != NULLP)
               {
                  wrUmmAnrMeasPrcMsg(transCb, anrMeasIntMsg);
                  WR_FREE(anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg));
               }
            }
            else
            {
               if(measTransCb->isDrxReqForRptCgi == TRUE)
               {
                  measTransCb->isDrxCfged = FALSE;
                  if(ROK == wrUmmMeasAllocRecfgMsg(&rrcReCfgMsg, 
                           &measTransCb->measTransCfg, 
                           ueCb, 
                           transCb->transId))
                  {
                     /* ccpu00132002 Fix -> Delete ReportCgi meas Id from UeCb */
                     wrUmmMeasGetNhuMacMainCfgFrmReCfg(rrcReCfgMsg, &macCfg);

                     /* MAC-MainConfig */
                     WR_FILL_TKN_UINT(macCfg->pres, PRSNT_NODEF);

                     WR_FILL_TKN_UINT(macCfg->timeAlignmentTimerDedicated, 7);
                     measTransCb->drxCfgPtr  = &ueCb->drxCfg;
                     wrUmmRrcFillDrxCfgIe(&macCfg->drx_Config, measTransCb->drxCfgPtr);

                     measTransCb->closeTrns = TRUE;
                     /* When eNB App is forcefully trying to stop the ANR transaction by deleting
                        PCI to be processed then it has to delete the measurement ID. TBD need to 
                        verify is it requried for P-ANR. */
                     if ((0 != measTransCb->measTransCfg.rmvMeasInfo.measIdRmvLstCnt) ||
                           (0 != measTransCb->measTransCfg.rmvMeasInfo.measObjIdRmvLstCnt) ||
                           (0 != measTransCb->measTransCfg.rmvMeasInfo.rptCfgRmvLstCnt))
                     {
                        wrUmmMeasGetNhuMeasCfgFrmReCfg(rrcReCfgMsg, &nhuMeasCfg);
                        if(ROK != wrUmmMeasFillNhuRmvIdLst(&rrcReCfgMsg->memCp, nhuMeasCfg, 
                                 &measTransCb->measTransCfg.rmvMeasInfo))
                        {
                           RLOG0(L_ERROR,"wrUmmFillNhuMsg: wrUmmMeasFillNhuRmvIdLst Failed.");
                           RETVALUE(RFAILED);
                        }
                     }
                     if(ROK == wrIfmRrcSndReqToUe(rrcReCfgMsg))
                     {
                        RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti,
                              "RRC Re-Config message sent to release DRX");
                        /* Start RRC Reconfigure timer*/
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
               }
            }
            break;
         }
      case WR_UMM_MEAS_UTRA_REPORT_CGI:
         {
            RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti,"Received ANR report "
               "to report cgi ");
            wrUmmAnrMeasHdlUtraRptCgi(recvMeasId, transCb, &measObjInfo, measRprt);
            break;
         }
      default:
         RLOG_ARG0(L_ERROR, DBG_CRNTI, ueCb->crnti,"Received invalid report type");
         break;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for Processing RRC message
 *
 * @details
 *
 *  Function:wrUmmAnrMeasPrcRrcMsg
 *
 *  Processing steps:
 *    - Process RRC Message
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
 
 PRIVATE S16 wrUmmAnrMeasPrcRrcMsg
 (
   WrUmmTransCb                 *transCb
 )
 {
    NhuUlDcchMsg                          *ulDcch = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg;
    NhuUL_DCCH_MsgTypc1                   *c1;
    NhuMeasurementReport                  *measRpt;
    NhuMeasurementReportcriticalExtnsc1   *measRptC1;
    NhuMeasResults                        *measRes;
    WrUeCb                                *ueCb = transCb->ueCb;
    U32                                   rrcTransId;
    S16                                   ret = ROK;

    c1 = &ulDcch->dcchMsg.message.val.c1;
    switch(c1->choice.val)
    {
       case C1_MEASUREMENTREPORT:
       {
          measRpt = &c1->val.measurementReport;
          measRptC1 = &measRpt->criticalExtns.val.c1;
          measRes = &measRptC1->val.measurementReport_r8.measResults;

          if((0 != ueCb->measCfg->measIdForR1) && 
                (FALSE == ueCb->measCfg->isRptCgiCfg))
          {
             /*Stop timer WR_TMR_ANR_STRONGCELL_REPORT_R1*/
             wrStopTransTmr(transCb,WR_TMR_ANR_STRONGCELL_REPORT_R1);
          }
          wrUmmAnrMeasHdlMeasRpt(transCb, measRes);
          break;
       }
       case C1_RRCCONRECFGNCOMPL:
       {
          /* Stop the RRC reconfig
           * timer */
          wrStopTransTmr(transCb,WR_TMR_RECONFIGURE);
          wrUmmMeasCfgReCfgReqCompHand(WR_UMM_MEAS_TYPE_PERD_ANR, transCb);
          rrcTransId = c1->val.rrcConRecfgnCompl.rrc_TrnsxnId.val;
          ret = wrUmmRelRrcTransId(ueCb, C1_RRCCONRECFGN, rrcTransId);
          if (ret != ROK)
          {
             RLOG0(L_ERROR, "Release of transaction ID failed");
          }

          /* Measurment Gap configuration. If the other feature requests for the 
            Meas Gap no action has to be taken.  */
          if (transCb->u.anrMeasTrans.measTransCfg.measGapStatus == 
                                                WR_UMM_MEAS_GAP_INIT_UE_CONFIG)
          {
             transCb->u.anrMeasTrans.measTransCfg.measGapStatus =
                                                    WR_UMM_MEAS_GAP_CONFIGURED;

             if (ueCb->measCfg->gapMeasInfo.latestTxId == transCb->transId) 
             {
                /* Update the measGapStatus to Configued */
                ueCb->measCfg->gapMeasInfo.isMeasGapCfg = WR_UMM_MEAS_GAP_CONFIGURED;

                /* Re-initialize the latest Tx Id */
                ueCb->measCfg->gapMeasInfo.latestTxId = 0;
          }
          }
          else if(transCb->u.anrMeasTrans.measTransCfg.measGapStatus == 
                                                            WR_UMM_MEAS_GAP_REL)
          {
             if (ueCb->measCfg->gapMeasInfo.latestTxId == transCb->transId) 
             {
                /*
                   While configuring the Meas Gap at UE, already configured Meas Gap 
                   at SCH, eNB App might have initiated the ANR close transaction which 
                   in turn sent messsage to the UE for removing the P-ANR's Measurement 
                   report and Measurement Gap(default shall be transmitted). Now in the 
                   above scenario, ANR may end up receiving two RRC configuration complete 
                   message, one for configuring other for releasing. First it will receive 
                   for configuring but since the measGapStatus changed to release it shall 
                   enter this condition. Here Gap needs to be released at scheduler only once. 
                   To avoid sending message scheduler multiple times added this condition.
                */
                if(ueCb->measCfg->gapMeasInfo.isMeasGapCfg != WR_UMM_MEAS_GAP_NONE)
                {
                   /* Send the Measurment gap configuration to the scheduler */
                   wrUmmAnrMeasGapSchdUeRecfgReq(transCb);

                   /* Release the Meas Gap configured at the UE Cb so that if 
                      required other module shall use it */
                   ueCb->measCfg->gapMeasInfo.isMeasGapCfg = WR_UMM_MEAS_GAP_NONE;
                }
                
                /* The transaction needs to be closed upon receiving the Scheduler 
                 confirmation.So changing the measGapStatus to NONE, this shall result in 
                 falling into closing block during handling scheduler confirmation. 
                 */
                if(transCb->u.anrMeasTrans.closeTrns == TRUE)
                {
                   /* With this flag the transaction shall be closed before receiving confirmations 
                      from either SCHEDULER or UE. That is one of the confirmation shall be dropped. 
                      As there is no problem in dropping with confirmation proceding as of now.*/
                   transCb->u.anrMeasTrans.measTransCfg.measGapStatus = 
                                                           WR_UMM_MEAS_GAP_NONE;
                   /* Even though the following flags are false just for 
                     ensuring this are initialised */
                   ueCb->measCfg->isRptCgiCfg = FALSE;
                   transCb->u.anrMeasTrans.isDrxReqForRptCgi = FALSE;
                }
                /* There is possiblity that eNB App initiated Close Transaction 
                   and so P-ANR released Meas Gap at UE. Till Meas Gap released at 
                   Scheduler P-ANR transaction should not be closed. So breaking 
                */
                break;
             }
          }
          else if(transCb->u.anrMeasTrans.measTransCfg.measGapStatus == 
                                                            WR_UMM_MEAS_GAP_INIT_SCH_CONFIG)
          {
             /* While configuring the UE for Meas Gap, that is ANR after configuring Scheduler
               and sent message to configure the UE, If eNB app initiated ANR close transaction 
               procedure then as a part of releasing measurment gap if Measurement module requested 
               for configuring different gap configuration then this condition hits while processing 
               the received RRC Reconfiguration complete message. Here just break.*/
             break;
          }

          if((ueCb->measCfg->isRptCgiCfg == FALSE) && 
              (transCb->u.anrMeasTrans.isDrxReqForRptCgi == TRUE))
          {
             transCb->u.anrMeasTrans.isDrxReqForRptCgi = FALSE;
             wrUmmAnrDrxSchdUeRecfgReq(transCb);
          }
          else if(transCb->u.anrMeasTrans.closeTrns == TRUE)
          {
            /* When CGI report is not configured and eNB app initiated 
               close Transaction then close the transaction.*/
            wrUmmTransComplete(transCb);
          }
          break;
       }
       default:
       {
          RLOG0(L_ERROR, "Invalid Message received");
          RETVALUE(RFAILED);
       }
    }
    RETVALUE(ROK);
 }

/** @brief This function is responsible for storing the measurement Id of type 
 *         ANR in the ANR transaction for deleting. 
 *
 * @details
 *
 *  Function:wrUmmHdlAnrInitiateMeasIdDel
 *
 *  Processing steps:
 *    - delete/remove all the measurement Id of type PANR.
 *
 *  @param[in]  newPcilist: pointer to the  pci list.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE Void wrUmmHdlAnrInitiateMeasIdDel
(
 WrUmmAnrMeasTransInfo   *anrMeasTransCb,
 WrUeCb                  *ueCb,
 WrUmmCellCb             *cellCb, 
 S16                     transId
)
{
   WrUmmMeasRmvInfo    *rmvMeasInfo = &anrMeasTransCb->measTransCfg.rmvMeasInfo;
   CmLList             *measIdLnk = ueCb->measCfg->measIdLst.first;
   WrMeasIdNode        *measIdNode;

   /* Traverse the list of measurement List in ueCb */
   while(NULLP != measIdLnk)
   {
      measIdNode = (WrMeasIdNode*)measIdLnk->node;
      /*If the measId Type is of Periodic ANR */
      if((measIdNode->measIdTyp == WR_UMM_MEAS_TYPE_PERD_ANR) &&
            (ueCb->measCfg->rmvAnrMeasId != measIdNode->measID))
      { 
         /* Update the measurement Id in the remove list. */
         rmvMeasInfo->measIdRmvLst[rmvMeasInfo->measIdRmvLstCnt] = 
                                                           measIdNode->measID;
         rmvMeasInfo->measIdRmvLstCnt++;
      }
      measIdLnk = measIdLnk->next;
   }

   if(ueCb->measCfg->rmvAnrMeasId != 0)
   {
      rmvMeasInfo->measIdRmvLst[rmvMeasInfo->measIdRmvLstCnt] = 
                                                  ueCb->measCfg->rmvAnrMeasId;
      rmvMeasInfo->measIdRmvLstCnt++;
      ueCb->measCfg->rmvAnrMeasId = 0;
   }

   RETVOID;
}

/** @brief This function is responsible for deleting all new pcis 
 *          of intra/inter/inter Rat frequencies.
 *
 * @details
 *
 *  Function:wrUmmPeriodicAnrDelNewPcis
 *
 *  Processing steps:
 *    - based on the current configuration type select the frequency type.
 *    - delete the all the  new PCIs of selected frequency.
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE Void wrUmmPeriodicAnrDelNewPcis
(
  WrUmmTransCb       *transCb
)
{
   WrMeasUeCb        *measUeCb = transCb->ueCb->measCfg;

   /* Based on the current RAT type configured by the ANR delete 
     the list of PCIs that are not configured for the CGI report */
   switch(measUeCb->ueAnrMeasInfo.currAnrCfgType)
   {
      case WR_UMM_MEAS_INTRA_FREQ: 
      {
        wrUmmDelIntraFreqNonCfgNewPciObj(&measUeCb->intraNewPci.newPciLst);
        break;
      }

      case WR_UMM_MEAS_INTER_FREQ:
      {
        wrUmmDelInterFreqNonCfgNewPciObj(&measUeCb->interNewPciLst);
        break;
      }
      case WR_UMM_MEAS_UTRA_FDD_FREQ:
      default :
      break;
   }
   RETVOID;
}

/** @brief This function is responsible for Processing internal message
 *
 * @details
 *
 *  Function:wrUmmAnrMeasPrcIntMsg
 *
 *  Processing steps:
 *    - Process measurement configaration
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmAnrMeasPrcIntMsg
(
 WrUmmTransCb                 *transCb
)
{
   WrUmmCellCb                  *cellCb;
   WrUeCb                       *ueCb;
   WrUmmIncMsg                  *msg = transCb->msg;
   WrUmmAnrMeasTransInfo        *anrMeasTransCb = &transCb->u.anrMeasTrans;
   WrUmmIntAnrMeasMsg           *anrMeasIntMsg = NULLP;
   LwrCellMeasCfgGrp            *measCellCfg;
   U32                          rptInt;

   TRC2(wrUmmAnrMeasPrcIntMsg)

   ueCb = transCb->ueCb;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId,"Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   anrMeasIntMsg = msg->u.internalMsg->u.anrMeasMsg;

   if(anrMeasIntMsg->choice == WR_UMM_ANR_MEAS_CREATE)
   {
      /* Initialise the ANR MEAS CREATE related variables*/
      wrUmmMeasResetTransCb(&transCb->u.anrMeasTrans.measTransCfg);
      anrMeasTransCb->closeTrns = FALSE;
      ueCb->measCfg->isAnrMeasCfged = WR_UMM_CONFIGED;
      ueCb->anrMeasTransId = transCb->transId;

      if(anrMeasIntMsg->cellTyp == WR_UMM_MEAS_INTRA_FREQ)
      {
         wrUmmAnrMeasIntraCfg(anrMeasIntMsg,ueCb,anrMeasTransCb,cellCb);
      }
      else if(anrMeasIntMsg->cellTyp == WR_UMM_MEAS_INTER_FREQ)
      {
         if(RFAILED == wrUmmAnrMeasInterCfg(anrMeasIntMsg,ueCb,anrMeasTransCb,cellCb))
         {
           RLOG1(L_ERROR, " Meas Inter Configuration failed for UE %d ", ueCb->crnti);
           ueCb->anrMeasTransId = 0;
           wrUmmTransComplete(transCb);
           RETVALUE(RFAILED);
         }
      }
      else if(anrMeasIntMsg->cellTyp == WR_UMM_MEAS_UTRA_FDD_FREQ)
      {
         /*rel10*/
         wrUmmAnrMeasUtranCfg(anrMeasIntMsg,ueCb,anrMeasTransCb,cellCb);
      }
      anrMeasTransCb->measTransCfg.transState = WR_UMM_ANR_MEAS_TRANS_INPROG;
   }
   else
   {

      /* Delete the PCIs */
      wrUmmPeriodicAnrDelNewPcis(transCb);

      /* Change the current RAT type */ 
      ueCb->measCfg->ueAnrMeasInfo.currAnrCfgType = WR_UMM_MEAS_NONE;

      ueCb->measCfg->isAnrMeasCfged = WR_UMM_NOT_CONFIGED;

      /* if ANR is configured for the CGI report or
         already initiated ANR close procedure 
         then return.
      */
      if((ueCb->measCfg->isRptCgiCfg == TRUE) ||
         (anrMeasTransCb->closeTrns == TRUE))
      {
         /* No need to send the message to UE. 
            So return here itself */
         RETVALUE(ROK);
      }

      /* Remove P-ANR related the measurement Id */
      wrUmmHdlAnrInitiateMeasIdDel(anrMeasTransCb, ueCb, cellCb, transCb->transId);

      /* Set the close Trns flag */
      anrMeasTransCb->closeTrns = TRUE;

      /* Release the meas Gap for this UE */
      wrUmmMeasRelMeasGap(WR_UMM_MEAS_GAP_INTER_FREQ_ANR, cellCb, ueCb,
                          &anrMeasTransCb-> measTransCfg.measGapStatus, 
                          transCb->transId);

      //TODO Raghu Need to remove this following statement. Test it.
      anrMeasTransCb->measTransCfg.transState = WR_UMM_ANR_MEAS_TRANS_DONE;
   }

   /* Check if the measurement gap needs to be configured */
   if ((ueCb->measCfg->gapMeasInfo.latestTxId == transCb->transId) && 
       (transCb->u.anrMeasTrans.measTransCfg.measGapStatus == WR_UMM_MEAS_GAP_INIT_SCH_CONFIG)
      ) 
   {
      wrUmmAnrMeasGapSchdUeRecfgReq(transCb);
   }
   else 
   {
      if((ueCb->measCfg->gapMeasInfo.latestTxId == transCb->transId) && 
         (transCb->u.anrMeasTrans.measTransCfg.measGapStatus == WR_UMM_MEAS_GAP_REL))
      {
        ueCb->measCfg->gapMeasInfo.isMeasGapCfg = WR_UMM_MEAS_GAP_REL;
      } 
      
      /*Prepare and  Send RRC Reconfiguration  message to UE */
      wrUmmMeasPrepareSendRrcRecfg(ueCb->cellId, ueCb,
                                   &transCb->u.anrMeasTrans.measTransCfg, 
                                   transCb->transId);
      if(ueCb->measCfg->measIdForR1)
      {
         measCellCfg = cellCb->measCellCfg;
         rptInt = wrGetRptInvlValFromEnum(measCellCfg->measEutraPerdRptCfg
               [WR_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS].reportInterval);
         wrStartTransTmr(transCb,WR_TMR_ANR_STRONGCELL_REPORT_R1,
                    rptInt + WR_TMR_ANR_STRONGCELL_RPRT_R1_DELTA); 
      }
   }
   RETVALUE(ROK);
}


/** @brief This function is used to select the Arfcn for UE in ANR Measurement
 * UE selection algorithm.
 *
 * @details
 *
 *     Function: wrUmmAnrStartMeasurementForUe
 *
 *         Processing steps:
 *     1) start Anr algorithm based on ue capabilities
 *
 * @param[in]   UeCb :  UE control block.
 * @return
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmAnrStartMeasurementForUe
(
  WrUeCb                      *ueCb, 
  WrUmmCellCb               *cellCb
)
{
   WrUmmIntAnrMeasMsg        *anrMeasIntMsg;
   WrUmmInternalMsg          *internalMsg;
   WrUmmIncMsg               *msg;

   TRC2(wrUmmAnrStartMeasurementForUe)

   /* Check that ueCb is valid or not */
   if(ueCb == NULLP)
   {
      RLOG0(L_ERROR,
               "NULL UeCb Received");
      RETVALUE(RFAILED);
   }

   /* Check that cellCb is valid or not */
   if (cellCb == NULLP)
   {
      RLOG1(L_ERROR, 
               "Error in getting CELL CB "
               "for cellId = %lu", (U32)ueCb->cellId);
      RETVALUE(RFAILED);
   }

   /* Initiate configuring ANR only if UE 
      - is not NULL.
      - has valid UE Capabilities.
      - ANR measurment is not configured. 
      - has no bearer with QCI 1/2/3/4. 
      - If CSFB is NOT ongoing for that UE i.e if UE 
        is in CSFB_READY state. This should be common 
        for all RATs but as of now state management is 
        only for GERAN.
      and cell cb is configured with anrmeasInfo
    */
   if((ueCb->measCfg != NULLP) && 
      (ueCb->ueCap != NULLP) && 
      (cellCb->measCellCb->anrMeasInfo != NULLP) &&
      (ueCb->measCfg->isAnrMeasCfged == WR_UMM_NOT_CONFIGED) &&
      (WR_CSFB_GSM_UE_STATE_CSFB_READY == ueCb->csfbInfo.csfbState))
   {
      /* Initialize the PCI lists */
      cmLListInit(&ueCb->measCfg->interNewPciLst);
      cmLListInit(&ueCb->measCfg->utranNewPscLst);

      /* Copy the ANR measurement Information to the Ue Cb from CellCb */
      cmMemcpy((U8*)&ueCb->measCfg->ueAnrMeasInfo, 
               (U8*)cellCb->measCellCb->anrMeasInfo, 
               sizeof(WrUmmAnrMeasInfo));

      /* Initialise the P-ANR specific flags */
      ueCb->measCfg->ueAnrMeasInfo.currAnrCfgType = WR_UMM_MEAS_INTRA_FREQ;
      ueCb->measCfg->ueAnrMeasInfo.currAnrInterFreqCnt = 0;

      /* Select the measurement configuration for UE in ANR measurement*/
      wrUmmCfgAnrMeasForUe(ueCb, &anrMeasIntMsg,WR_UMM_MEAS_INTRA_FREQ);
      if(anrMeasIntMsg != NULLP)
      {
         if (ROK != wrUmmBldInternalMsg(&internalMsg,
                                        WR_UMM_INTMSG_ANR_MEAS_TRANS, 
                                        anrMeasIntMsg))
         {
            RLOG0(L_ERROR,
                     "wrUmmAnrStartMeasurementForUe:creation of internal msg Failed\n");
            RETVALUE(RFAILED);
         }

         /* Allocate the WrUmmIncMsg */
         WR_ALLOC(&msg,sizeof(WrUmmIncMsg));
         if (NULLP == msg)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         msg->msgTyp = WR_UMM_INTERNAL_MSG;
         msg->u.internalMsg = internalMsg;
         msg->refCnt = 1;

         wrUmmPrcIncMsg(cellCb, ueCb, msg);
         wrUmmRlsIncMsg(&msg);
      }
   }
   RETVALUE(ROK);
}

/** @brief This function configures the Periodic ANR 
 *
 * @details
 *
 *     Function: wrUmmStartPeriodicAnrTrans
 *
 *         Processing steps:
 *     1) start UE selection algorith based on ue capabilities
 *     2) Check Epoch Timer and Trice Timers are running.
 *     3) If Timers are running then start ANR configuraiton for this UE.
 *
 * @param[in]  ueCb : pointer to UE control block
 * @return
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmStartPeriodicAnrTrans
(
   WrUeCb                      *ueCb
)
{
   Bool                      ueCapAnrSupport = TRUE;
   Bool                      startAnr = FALSE;
   WrUmmCellCb               *ummCellCb;

   TRC2(wrUmmStartPeriodicAnrTrans)


   /* Get the UMM Cell CB */
   WR_UMM_GET_CELLCB(ummCellCb, ueCb->cellId);
   if (ummCellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)ueCb->cellId,
               "wrUmmStartPeriodicAnrTrans:Error in getting CELLCB");
      RETVALUE(RFAILED);
   }

   /* Check the UE is capable of supporting Intra based ANR */
   if(ueCb->ueCap == NULLP) 
   {
      RLOG_ARG0(L_ERROR,DBG_CRNTI,ueCb->crnti,"UE doesn't has UE-Capablility info");
      RETVALUE(ROK);
   }
   ueCapAnrSupport = wrUmmAnrChkUeCap(ueCb, WR_UMM_MEAS_INTRA_FREQ,WR_UMM_STRONG_CELL_CFG);
   if(ueCapAnrSupport != TRUE)
   {
      RLOG_ARG0(L_WARNING, DBG_CRNTI,ueCb->crnti,"UE doesn't support ANR");
      RETVALUE(ROK);
   }

   wrEmmIsPrdAnrTmrsRunning(ueCb->cellId, &startAnr);

   if(TRUE == startAnr)
   {
         wrUmmAnrStartMeasurementForUe(ueCb, ummCellCb);
      }

   RETVALUE(ROK);
}


/** @brief This function is used to select the Arfcn for UE in ANR Measurement
 * UE selection algorithm.
 *
 * @details
 *
 *     Function: wrUmmAnrStartMeasurements
 *
 *         Processing steps:
 *     1) start UE selection algorithm based on ue capabilities
 *
 * @param[in]  cellId : cell Identifier.
 * @return
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmAnrStartMeasurements
(
 U16                         cellId
)
{
   U16                       ueCnt = 0;
   WrUeCb                    *ueCb;
   WrUmmIntAnrMeasMsg        *anrMeasIntMsg = NULLP;
   WrUmmCellCb               *cellCb;
   Bool                      ueCapAnrSupport = TRUE;

   TRC2(wrUmmAnrStartMeasurements)

   /* Get the UE CB */
   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG1(L_ERROR, 
         "wrUmmAnrStartMeasurements: Error in getting CELL CB for cellId %lu", 
         (U32)cellId);
      RETVALUE(RFAILED);
   }

   /* Process through the list of UE Connected */
   while(ueCnt < cellCb->maxUesPerCell)
   {
      anrMeasIntMsg = NULLP;
      ueCb = cellCb->ueCbs[ueCnt];

      /* Initiate configuring ANR only if UE 
        - is not NULL.
        - has no bearer with QCI 1/2/3/4. 
      */
      if((ueCb != NULLP) && 
         (ueCb->ueCap != NULLP) &&
         (ueCb->isGBRBearerExists == NOT_IN_USE) &&
         (WR_CSFB_GSM_UE_STATE_CSFB_READY == ueCb->csfbInfo.csfbState))
      {
         /* Check the UE is capable of supporting Intra based ANR */
         ueCapAnrSupport = wrUmmAnrChkUeCap(ueCb, WR_UMM_MEAS_INTRA_FREQ,WR_UMM_STRONG_CELL_CFG);
         if(TRUE == ueCapAnrSupport)
         {
            wrUmmAnrStartMeasurementForUe(ueCb, cellCb);
         }
      }
      ueCnt++;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to deleting Anr measurement
 * configration for the selected ue's.
 *
 *  @details
 * Function:wrUmmStopPeriodicAnr
 *
 *   Processing steps:
 *    - Delete ANR Measurement configuration for selected UE 
 *
 * @param[in]  ueCb : pointer to ueCb
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 **/
PUBLIC S16 wrUmmStopPeriodicAnr
(
   WrUeCb  *ueCb
)
{
   WrUmmIntAnrMeasMsg        *delAnrMeasIntMsg = NULLP;
   WrUmmInternalMsg          *internalMsg = NULLP;
   WrUmmCellCb               *cellCb = NULLP;
   WrUmmIncMsg               *msg; 

   /* Get the Cell Cb from the cell Id of ueCb */
   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,
               "wrUmmAnrDelMeasCfgForAllUe: cellCb is NULL.\n");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }


   /* Prepare the internal ANR message for posting the message to the ANR module*/
   WR_ALLOC(&delAnrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg));
   if (NULLP == delAnrMeasIntMsg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Build Internal message for the ANR */
   if (ROK != wrUmmBldInternalMsg(&internalMsg,
                                  WR_UMM_INTMSG_ANR_MEAS_TRANS, 
                                  delAnrMeasIntMsg))
   {
      RLOG0(L_ERROR, "wrUmmAnrDelMeasCfgForAllUe:creation of internal message Failed \n");
      RETVALUE(RFAILED);
   }

   /* Set the internal message for deletion */
   delAnrMeasIntMsg->choice = WR_UMM_ANR_MEAS_DELETE;

   /* Fill the Incoming message for process the ANR */
   msg->msgTyp = WR_UMM_INTERNAL_MSG;
   msg->u.internalMsg = internalMsg;
   msg->refCnt = 1;

   /* Process the Incoming message */
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   wrUmmRlsIncMsg(&msg);

   RETVALUE(ROK);
}

/** @brief This function is responsible for handling periodic measurements
 * reports.
 *
 * @details
 *
 * Function: wrUmmFindNewPciFrmAnrMeasRpt
 *
 *   Processing steps:
 *    - Process the PCI's from measurement report to ANR to find any
 new PCI's
 *      are present or not.
 * @param[in]  ueCb pointer to Ue Control block
 * @param[in]  transCb pointer to transaction control block
 * @param[in]  measRprt pointer to Measurement report
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 **/
PUBLIC S16 wrUmmFindNewPciFrmAnrMeasRpt
(
 WrUmmMeasObjInfo             *measObjInfo,
 NhuMeasResults               *measRprt,
 WrUmmTransCb                 *transCb,
 U8                           isEvntAnr
)
{
   WrUeCb                       *ueCb;
   WrUmmCellCb                  *cellCb;
   WrMeasUeCb                   *measUeCb;
   U16                          cellId;
   WrNewPciLst                  pciLst = {0};
   U8                           noComp = 0;

   TRC2(wrUmmFindNewPciFrmAnrMeasRpt)

   ueCb = transCb->ueCb;
   cellId = ueCb->cellId;
   measUeCb = ueCb->measCfg;

   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,
                     "wrUmmFindNewPciFrmAnrMeasRpt: cellCb is NULL.\n");
      RETVALUE(RFAILED);
   }
   switch(measObjInfo->nrType)
   {
      case WR_UMM_MEAS_INTRA_FREQ:
      case WR_UMM_MEAS_INTER_FREQ:
      {
         NhuMeasResultLstEUTRA     *measRsultLstEutra;

         measRsultLstEutra = &measRprt->measResultNeighCells.val.measResultLstEUTRA;
         noComp = measRsultLstEutra->noComp.val;
         pciLst.nrType = WR_EMM_ANR_EUTRAN;
         pciLst.nrInfo.earfcn = measObjInfo->eutraObj->eutraMeasObj->carrFreq;
         break;
      }
      case WR_UMM_MEAS_UTRA_FDD_FREQ:
      {
         NhuMeasResultLstUTRA     *measRsultLstUtra;

         measRsultLstUtra = &measRprt->measResultNeighCells.val.measResultLstUTRA;
         noComp = measRsultLstUtra->noComp.val;
         pciLst.nrType = WR_EMM_ANR_UTRA_FDD;
         pciLst.nrInfo.earfcn = measObjInfo->utraFddObj->utraMeasObj->arfcn;
         break;
      }
      default:
      {
         RETVALUE(RFAILED);
      }
   }
   if(noComp != 0)
   {
      if(ROK == wrUmmAnrMeasCreatePciLst (&pciLst, measRprt))
      {
         wrEmmAnrValidatePciList (cellId, pciLst.nrType, &pciLst,
               isEvntAnr, ueCb->ueIdx);

         /*Based on the received Carrier frequency, identify the PCI list. 
          * Create newPCIbject and add to the list */
         if(ROK != wrUmmAnrMeasCheckForNewPci(measObjInfo, &pciLst, measUeCb))
         {
            RLOG0(L_ERROR, "wrUmmAnrMeasCheckForNewPci failed");
            RETVALUE(RFAILED);
         }
      } 
   }
   RETVALUE(ROK);
}

/** @brief This function is to handle RRC Reconfig Timers 
 *         expiry functions.
 *
 * @details
 *
 *     Function: wrUmmAnrPrcRrcTmrExpiry 
 *
 *         Processing steps:
 *
 * @param[in]  transCb : transaction inforamation
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmAnrPrcRrcTmrExpiry 
(
  WrUmmTransCb                 *transCb
)
{
   U32                              rrcTransId;
   U32                              reportAmt;
   U8                               rptAmtEnum;
   U32                              measId;
   WrUeCb                           *ueCb = transCb->ueCb;
   WrUmmCellCb                      *cellCb;
   LwrCellMeasCfgGrp                *measCellCfg;
   WrMeasUeCb                       *measUeCb;
   WrUmmAnrMeasTransInfo            *measTransCb;
   WrUmmIntAnrMeasMsg               *anrMeasIntMsg = NULLP;

   switch(transCb->msg->u.tmrExpiry.timerEvnt)
   { 
      case WR_TMR_RECONFIGURE:
         {
            RLOG_ARG0(L_ERROR, DBG_CRNTI,ueCb->crnti,"RRC Re-Config timer expired");
            if( wrUmmGetOnGoingRrcTransId(ueCb, C1_RRCCONRECFGN, 
                     WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
                     &rrcTransId) == ROK)
            {
               RLOG_ARG1(L_ERROR, DBG_CRNTI,ueCb->crnti,"RRC Transaction [%lu]"
                  " Release On Recipt of Tmr Expiry Int Msg Type", rrcTransId);
               wrUmmRelRrcTransId(ueCb, C1_RRCCONRECFGN, rrcTransId);
            }
            else
            {
               RLOG_ARG1(L_ERROR, DBG_CRNTI,ueCb->crnti,"Invalid Tranasaction "
                  "Received[%lu]", WR_GET_TRANS_FRM_TRANSID(transCb->transId));
            }
            break;
         }
      case WR_TMR_ANR_STRONGCELL_REPORT_R1:
         {
            WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
            if(NULLP == cellCb)
            {
               RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId, "Error in getting CELL CB");
               RETVALUE(RFAILED);
            }
            measCellCfg = cellCb->measCellCfg;
            measUeCb = ueCb->measCfg;
            measTransCb = &transCb->u.anrMeasTrans;
            /*Remove INTRA measId. Add INTER measObj, measId*/
            rptAmtEnum = measCellCfg-> 
               measEutraPerdRptCfg[WR_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS].reportAmount;
            reportAmt = wrUmmMeasGetRptAmtFrmEnum(rptAmtEnum);
            measId = ueCb->measCfg->measIdForR1;
            wrUmmMeasRmvMeaIdInUeCb(measId, &measUeCb->measIdLst);
            wrUmmMeasSetFreeMeasId(measUeCb, measId);
            ueCb->measCfg->rmvAnrMeasId = measId;
            measTransCb->anrPrdRptCnt = 0;
            if(WR_UMM_MEAS_INTER_FREQ == measUeCb->ueAnrMeasInfo.currAnrCfgType)
            {
               /* If measurement Gap is configured then release it at both 
                  the UE and scheduler and close the transaction.
                  If measurement gaps is enabled that means ANR DRX is not configured 
                  */
               wrUmmMeasRmvMeaIdInUeCb(measId, &measUeCb->measIdLst);
               wrUmmMeasSetFreeMeasId(measUeCb, measId);
               measUeCb->rmvAnrMeasId = 0;
               measTransCb->closeTrns = TRUE;
               wrUmmAnrRelMeasGapMeasId(transCb);
            }
            else
            {
               wrUmmAnrSelNxtRatType(ueCb);
               if(TRUE == wrUmmAnrChkUeCap(ueCb,
                        measUeCb->ueAnrMeasInfo.currAnrCfgType,WR_UMM_STRONG_CELL_CFG))
               {
                  anrMeasIntMsg =(WrUmmIntAnrMeasMsg*) wrUmmAnrSelMeasConfigForUe(ueCb);
               }
               /* Process the message only if UE supports current frequency 
                  and anrMeasInternal message is created */
               if(anrMeasIntMsg != NULLP)
               {
                  wrUmmAnrMeasPrcMsg(transCb, anrMeasIntMsg);
                  WR_FREE(anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg));
               }
            }
            break;
         }
      default:
         {
            RLOG1(L_ERROR, "SWERROR Received [%d] invalid Timer Event",
               transCb->msg->u.tmrExpiry.timerEvnt);
            RETVALUE(RFAILED);
         } 
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for receive ANR Measurement related 
 * messages which is received from UE.
 *
 * @details
 *
 *  Function:wrUmmAnrMeasProcessor
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
PUBLIC S16 wrUmmAnrMeasProcessor
(
 WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg                  *msg       = transCb->msg;
   WrUmmIntAnrMeasMsg           *anrMeasIntMsg = NULLP;
   WrUeCb                       *ueCb = transCb->ueCb;
   WrUmmAnrMeasTransInfo        *measTransCb = &transCb->u.anrMeasTrans;
   U8                           ueCapAnrSupport = FALSE;
   U8                           rptAmtEnum;
   U32                          rptInt;
   TRC2(wrUmmAnrMeasProcessor)

   switch(msg->msgTyp)
   {
      case WR_UMM_UU_IND_PDU:
         {
            wrUmmAnrMeasPrcRrcMsg(transCb);
            break;
         }
      case WR_UMM_INTERNAL_MSG:
         {
            wrUmmAnrMeasPrcIntMsg(transCb);
            break;
         }
      case WR_UMM_SCHD_CFG:
         {
            WrUmmCellCb                      *cellCb;
            NhuDatReqSdus                    *rrcReCfgMsg;
            NhuMAC_MainConfig                *macCfg;
            NhuMeasConfig                    *nhuMeasCfg;

            WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
            if(NULLP == cellCb)
            {
                  RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId,
                     "Error in getting CELL CB");
               RETVALUE(RFAILED);
            }

            rptAmtEnum = cellCb->measCellCfg-> 
               measEutraPerdRptCfg[WR_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS].reportAmount;
            rptInt = wrGetRptInvlValFromEnum(cellCb->measCellCfg->measEutraPerdRptCfg
                  [WR_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS].reportInterval);

            /* As MeasGap is configured first for the inter Frequency so 
             * First check the Measurement Gap flags 
             */
            if((measTransCb->measTransCfg.measGapStatus == 
                                         WR_UMM_MEAS_GAP_INIT_SCH_CONFIG) &&
                /* To avoid conflicting with the DRX configuration */
                (measTransCb->isReCfgSent == FALSE))
            {
               /* Received the Measurement gap confirmation from the scheduler.*/
               RLOG0(L_DEBUG, " Received Schd Gap Setup Cfm ");
 
               /* If the latest Tx Id matches with the current TxId then 
                  this Tx  has to complete the Gap configuration. So update the 
                  measGapStatus based on the latestTxId */
               if(ueCb->measCfg->gapMeasInfo.latestTxId == transCb->transId) 
               {
                  /* Change the Gap state to UE CONFIG */
                  measTransCb->measTransCfg.measGapStatus = 
                                              WR_UMM_MEAS_GAP_INIT_UE_CONFIG;
               }
               else
               {
                  /* Change the Gap state to NONE*/
                  measTransCb->measTransCfg.measGapStatus = 
                                              WR_UMM_MEAS_GAP_NONE;
               }

               /*Prepare and  Send RRC Reconfiguration  message to UE */
               wrUmmMeasPrepareSendRrcRecfg(transCb->ueCb->cellId, 
                                            transCb->ueCb, 
                                            &measTransCb->measTransCfg, 
                                            transCb->transId);

               if(NhuReportConfigEUTRAreportAmountr1Enum == rptAmtEnum)
               {
                  wrStartTransTmr(transCb,WR_TMR_ANR_STRONGCELL_REPORT_R1,
                    rptInt + WR_TMR_ANR_STRONGCELL_RPRT_R1_DELTA); 
               }
            }
            else if ((measTransCb->measTransCfg.measGapStatus == 
                                          WR_UMM_MEAS_GAP_REL) && 
                     /* To avoid conflicting with the DRX configuration */
                     (measTransCb->isReCfgSent == FALSE))
            {
                if(transCb->u.anrMeasTrans.closeTrns == TRUE)
                {
                   /* When CGI report is not configured and eNB app initiated 
                      Transaction close procedure then close the transaction.*/
                   //Issue when close initiated while configuring Meas Gap
                   // So commenting this wrUmmTransComplete(transCb);
                }
                else
                {
                   measTransCb->measTransCfg.measGapStatus = WR_UMM_MEAS_GAP_NONE;
                }
            }
            else if((ueCb->measCfg->isRptCgiCfg == FALSE) && 
                  (measTransCb->isDrxReqForRptCgi == FALSE))   
            {
               /* Completed the current transaction so if UE supports next RAT 
                  * move to next RAT else close the transaction */

               /*reset the ANR periodic count */
               measTransCb->anrPrdRptCnt = 0;
               wrUmmMeasResetTransCb(&transCb->u.anrMeasTrans.measTransCfg);

               /*Move to next RAT */
               wrUmmAnrSelNxtRatType(ueCb);

               /* Check does UE supports the selected RAT */
               ueCapAnrSupport =  wrUmmAnrChkUeCap(ueCb, 
                                        ueCb->measCfg->ueAnrMeasInfo.currAnrCfgType,
                                        WR_UMM_STRONG_CELL_CFG);

               if((ueCapAnrSupport == TRUE) && !(ueCb->measCfg->measIdForR1))
               {
                  /* Process the selected measurement configuration for the UE */
                  anrMeasIntMsg = (WrUmmIntAnrMeasMsg*)wrUmmAnrSelMeasConfigForUe(ueCb);
               }

               if(anrMeasIntMsg != NULLP)
               {
                  /* End of DRX release procedure. 
                     So reset closeTrns flag.*/
                  transCb->u.anrMeasTrans.closeTrns = FALSE;
                  wrUmmAnrMeasPrcMsg(transCb, anrMeasIntMsg);
                  WR_FREE(anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg));
               }
               else
               {
                  ueCb->anrMeasTransId = 0;
                  wrUmmTransComplete(transCb);
               }
            }
            /* ccpu00131948 Fix */
            else if((measTransCb->isReCfgSent == TRUE) && 
                    (measTransCb->isDrxReqForRptCgi == TRUE))
            {
               /* Configured the Scheduler with the ANR DRX 
                * parameter. Send the RRC Re-config message to UE*/
               measTransCb->isReCfgSent = FALSE;
               if(ROK != wrUmmMeasAllocRecfgMsg(&rrcReCfgMsg, 
                                                &measTransCb->measTransCfg, 
                                                 ueCb, 
                                                 transCb->transId))
               {
                   RLOG0(L_ERROR,
                          "wrUmmMeasAllocRecfgMsg returned Failure");
                   RETVALUE(RFAILED);
               }
               wrUmmMeasGetNhuMacMainCfgFrmReCfg(rrcReCfgMsg, &macCfg);

               /* MAC-MainConfig */
               WR_FILL_TKN_UINT(macCfg->pres, PRSNT_NODEF);

               WR_FILL_TKN_UINT(macCfg->timeAlignmentTimerDedicated, 7);

               wrUmmRrcFillDrxCfgIe(&macCfg->drx_Config, measTransCb->drxCfgPtr);

               wrUmmMeasGetNhuMeasCfgFrmReCfg(rrcReCfgMsg, &nhuMeasCfg);
               if(ROK == wrMeasFillNhuMsg(&rrcReCfgMsg->memCp, nhuMeasCfg, ueCb, &transCb->u.anrMeasTrans.measTransCfg, 
                                         cellCb->measCellCfg, transCb->transId))
               {
                  wrIfmRrcSndReqToUe(rrcReCfgMsg);

                  /* Start RRC Reconfigure timer */
                  wrStartTransTmr(transCb,WR_TMR_RECONFIGURE, 
                        wrEmmGetRrcReCfgTmrVal(ueCb->cellId));
               /* Merge Conflit   measTransCb->rrcReCfgMsg = NULLP; */
               }
               else
               {
                  RLOG0(L_INFO, "Deallocating RRC Reconfg Message");
                  WR_FREE_EVNT(rrcReCfgMsg);
               }
            }
            else if((ueCb->measCfg->isRptCgiCfg == FALSE) && 
                  (measTransCb->isDrxReqForRptCgi == TRUE))   
            {
               /* Closing the transaction as DRX is released at
                * Scheduler and No new PCI is present. */
               RLOG1(L_INFO, "No new PCI present Closing ANR trans for UE: %d ", ueCb->crnti);
               ueCb->anrMeasTransId = 0;
               wrUmmTransComplete(transCb);
            }

            break;
         }
      case WR_UMM_RRC_DAT_CFM:
         {
            break;
         }
      case WR_UMM_TMR_EXPIRY:
         {
            wrUmmAnrPrcRrcTmrExpiry(transCb);
            if((ueCb->measCfg->isRptCgiCfg == FALSE) && 
                  (measTransCb->isDrxReqForRptCgi == TRUE))   
            {
               measTransCb->isDrxReqForRptCgi = FALSE;
               wrUmmAnrDrxSchdUeRecfgReq(transCb);
            }
            else if(WR_TMR_RECONFIGURE == msg->u.tmrExpiry.timerEvnt)
            {
               wrUmmMeasCfgReCfgReqCompHand(WR_UMM_MEAS_TYPE_PERD_ANR, transCb);
            }
            break;
         }
      default:
         {
            RLOG1(L_ERROR, "Received invalid message[%d]",msg->msgTyp);
            RETVALUE(RFAILED);
         } 
   }

   RETVALUE(ROK);
}

 /** @brief This function is responsible for deallocate transaction related
 * information after transaction complete.
 *
 * @details
 *
 *  Function:wrUmmAnrMeasTransRel
 *
 *  Processing steps:
 *    - Deallocates transaction information.
 *
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmAnrMeasTransRel
(
 WrUmmTransCb *transCb
)
{
   TRC2(wrUmmAnrMeasTransRel)

   wrUmmStopTransTimers(transCb);
   transCb->ueCb->anrMeasTransId = 0;
   /* 143302 */
   transCb->ueCb->measCfg->isAnrMeasCfged = WR_UMM_NOT_CONFIGED;
   RETVALUE(ROK);
}

/** @brief This function is responsible for to deleting Anr measurement
 * configration for All ue's.
 *
 *  @details
 * Function:wrUmmAnrDelMeasCfgForAllUe
 *
 *   Processing steps:
 *    - Delete ANR Measurement configuration for all UE present in Cell Cb
 *
 * @param[in]  cellCb  : pointre to Cell Cb
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 **/
PUBLIC S16 wrUmmAnrDelMeasCfgForAllUe
(
   U16                       cellId
)
{
   U16                       ueCnt = 0;
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;
   TRC2(wrUmmAnrDelMeasCfgForAllUe)

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,
               "wrUmmAnrDelMeasCfgForAllUe: cellCb is NULL.\n");
      RETVALUE(RFAILED);
   }
   while(ueCnt < cellCb->maxUesPerCell)
   {
      ueCb = cellCb->ueCbs[ueCnt];
      if(ueCb != NULLP)
      {
         if((ueCb->anrMeasTransId == 0) || 
               (ueCb->ueCap == NULLP))
         {
            ueCnt++;
            continue;
         }
        wrUmmStopPeriodicAnr(ueCb); 
      }
      ueCnt++;
   }
   if(cellCb->measCellCb->anrMeasInfo != NULLP)
   {
      WR_FREE(cellCb->measCellCb->anrMeasInfo, sizeof(WrUmmAnrMeasInfo));
   }
   RETVALUE(ROK);
}

/** @brief This function is used to select the new PCI intra frequency for
 * UE in ANR Measurement UE selection algorithm.
 *
 * @details
 *
 *     Function: wrUmmAnrMeasIntraNewPciCfg
 *
 *         Processing steps:
 *     1) If new PCI for Intra frequency is availble select for UE
 *
 * @param[in]  ueCb : pointer to Ue control block.
 * @return WrUmmIntAnrMeasMsg*
 *    -#Success : WrUmmIntAnrMeasMsg
 *    -#Failure : NULLP
 */
PUBLIC WrUmmIntAnrMeasMsg* wrUmmAnrMeasIntraNewPciCfg
(
  WrUeCb                      *ueCb
)
{
   WrUmmIntAnrMeasMsg           *anrMeasIntMsg = NULLP;
   CmLList                      *newPciLnk;
   WrUmmMeasNewPciInfo          *newPciObj;
   TRC2(wrUmmAnrMeasIntraNewPciCfg)

   newPciLnk = ueCb->measCfg->intraNewPci.newPciLst.first;
   while (NULLP != newPciLnk)
   {
      newPciObj = (WrUmmMeasNewPciInfo*)newPciLnk->node;

      if(newPciObj->isConfigred == WR_UMM_CONFIGED)
      {
         newPciLnk = newPciLnk->next;
         continue;
      }
      else
      {
         WR_ALLOC (&anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg))
         if(NULLP == anrMeasIntMsg)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(NULLP);
         }
         newPciObj->isConfigred = WR_UMM_CONFIGED;
         newPciObj->crnti = ueCb->crnti;

         anrMeasIntMsg->choice = WR_UMM_ANR_MEAS_CREATE;
         anrMeasIntMsg->arfcn.choice = WR_UMM_MEAS_INTRA_FREQ;
         anrMeasIntMsg->arfcn.t.carrierFreq = wrEmmGetServCarrierFreq(ueCb->cellId);
         anrMeasIntMsg->cellTyp = WR_UMM_MEAS_INTRA_FREQ;
         anrMeasIntMsg->rptCfgType = WR_UMM_MEAS_REPORT_CGI;
         anrMeasIntMsg->newPci = newPciObj->newPci;
         break;
      }
      newPciLnk = newPciLnk->next;
   }
   RETVALUE(anrMeasIntMsg);
}

/** @brief This function is used to select the new PCI inter frequency for
 * UE in ANR Measurement UE selection algorithm.
 *
 * @details
 *
 *     Function: wrUmmAnrMeasInterNewPciCfg
 *
 *         Processing steps:
 *     1) If new PCI for Inter frequency is availble select for UE
 *
 * @param[in]  interNewPciLst : pointer to inter New PCI Linked List.
 * @return WrUmmIntAnrMeasMsg*
 *    -#Success : WrUmmIntAnrMeasMsg
 *    -#Failure : NULLP
 */
PUBLIC WrUmmIntAnrMeasMsg* wrUmmAnrMeasInterNewPciCfg
(
 CmLListCp                     *interNewPciLst
)
{
   WrUmmIntAnrMeasMsg           *anrMeasIntMsg = NULLP;
   CmLList                      *interNewPciLnk;
   CmLList                      *newPciLnk;
   WrMeasNewPciNode             *interNewPciNode;
   WrUmmMeasNewPciInfo          *newPciObj;
   TRC2(wrUmmAnrMeasInterNewPciCfg)

   interNewPciLnk = interNewPciLst->first;
   while(NULLP != interNewPciLnk)
   {
      interNewPciNode = (WrMeasNewPciNode*)interNewPciLnk->node;

      newPciLnk = interNewPciNode->newPciLst.first;

      while (NULLP != newPciLnk)
      {
         newPciObj = (WrUmmMeasNewPciInfo*)newPciLnk->node;

         if(newPciObj->isConfigred == WR_UMM_CONFIGED)
         {
            newPciLnk = newPciLnk->next;
            continue;
         }
         else
         {
            WR_ALLOC (&anrMeasIntMsg, sizeof(WrUmmIntAnrMeasMsg))
            if(NULLP == anrMeasIntMsg)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(NULLP);
            }
            newPciObj->isConfigred = WR_UMM_CONFIGED;

            anrMeasIntMsg->choice = WR_UMM_ANR_MEAS_CREATE;
            anrMeasIntMsg->arfcn.choice = WR_UMM_MEAS_INTER_FREQ;
            anrMeasIntMsg->arfcn.t.carrierFreq = interNewPciNode->arfcn;
            anrMeasIntMsg->cellTyp = WR_UMM_MEAS_INTER_FREQ;
            anrMeasIntMsg->rptCfgType = WR_UMM_MEAS_REPORT_CGI;
            anrMeasIntMsg->newPci = newPciObj->newPci;
            break;
         }
         newPciLnk = newPciLnk->next;
      }
      interNewPciLnk = interNewPciLnk->next;
   }
   RETVALUE(anrMeasIntMsg);
}

/** @brief This function is responsible for Initializing Anr Measurements and 
 *   this function called after starting the trice timer to configure the list
 *   of frequency to consider for ANR measuements.
 * @details
 *
 *  Function:wrUmmAnrMeasInitCfg
 *
 *  Processing steps:
 *    - Intiallize ANR measurement object
 *
 *  @param[in]  cellId: Cell Identifier.
 *  @param[in]  anrMeasIntMsg: pointer to internal structure
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmAnrMeasInitCfg
(
 U16                         cellId,
 WrUmmAnrMeasInfo            *anrMeasCfgInfo
)
{
   WrUmmCellCb               *cellCb;
   TRC2(wrUmmAnrMeasInitCfg)

   if(NULLP == anrMeasCfgInfo)
   {
      RLOG0(L_ERROR, "ANR measurement Config is NULL");
      RETVALUE(RFAILED);
   } 

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR,"Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   if(cellCb->measCellCb->anrMeasInfo == NULLP)
   {
     cellCb->measCellCb->anrMeasInfo = anrMeasCfgInfo;
   }
   else
   {
      RLOG0(L_DEBUG, "ANR measurement info is not NULL");
      
      WR_FREE(cellCb->measCellCb->anrMeasInfo, sizeof(WrUmmAnrMeasInfo));
      cellCb->measCellCb->anrMeasInfo = anrMeasCfgInfo;
   }
   RETVALUE(ROK);
}

/** @brief This function is called when a new Transaction is created for any
 * other procedure. The new transaction is passed to this function and this
 * function determines the next course of action based on the new transaction
 *
 * @details
 *
 *     Function: wrUmmAnrMeasNewTransProcessor
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

PUBLIC S16 wrUmmAnrMeasNewTransProcessor
(
 WrUmmTransCb                *transCb,
 WrUmmTransCb                *incTrans
 )
{
   WrUeCb                    *ueCb = transCb->ueCb;
   CmLList                   *lnk = NULLP;
   WrMeasNewPciNode          *pciNode = NULLP;
   WrUmmCellCb               *cellCb;
   Bool                      drxQciFeaFlag = FALSE; 


   TRC2(wrUmmAnrMeasNewTransProcessor)

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, " cellCb is NULL. ");
      RETVALUE(RFAILED);
   }

   switch(incTrans->transTyp)
   {
      case WR_UMM_RRC_REESTAB_TRANS:
         {
               RLOG0(L_ERROR, "Reestablishment received and closing Anr "
                  "transaction");
            /* Stopping RRC Reconfiguration timer which started before 
             * fix for ccpu00147223 */
            wrStopTransTmr(transCb,WR_TMR_RECONFIGURE);
            if(ueCb->measCfg->intraNewPci.newPciLst.count != 0)
            {
               wrUmmMeasDeAllocMemForNewPciObj(&ueCb->measCfg->intraNewPci.newPciLst);
            }

              if(ueCb->measCfg->interNewPciLst.count != 0)
            {
               /* As the count is greater than 0 traverse through interNewPciLst list 
                 and release the node*/ 
               lnk = ueCb->measCfg->interNewPciLst.first;
               while(lnk != NULLP)
               {
                  pciNode = (WrMeasNewPciNode *)lnk->node;

                  wrUmmMeasDeAllocMemForNewPciObj(&pciNode->newPciLst);

                  cmLListDelFrm(&ueCb->measCfg->interNewPciLst, lnk);

                  WR_FREE(pciNode, sizeof(WrMeasNewPciNode));

                  lnk = ueCb->measCfg->interNewPciLst.first;
                  
               }
            }
            if(transCb->u.anrMeasTrans.isDrxReqForRptCgi == TRUE)
            {
               ueCb->measCfg->isRptCgiCfg = FALSE;
               wrUmmAnrDrxSchdUeRecfgReq(transCb);
               /* Merge Conflict
               if( NULLP != transCb->u.anrMeasTrans.rrcReCfgMsg)
               {
                 WR_FREE_EVNT(transCb->u.anrMeasTrans.rrcReCfgMsg);
               }*/
               /* If DRX is configured means Measurement gap
                   has been released. so Break*/
               break;
            }
            /* If Meas gap is configured then inform to the Meas 
             transaction and close the transaction */
            wrUmmMeasRelMeasGap(WR_UMM_MEAS_GAP_INTER_FREQ_ANR, cellCb, ueCb,
                               &incTrans->u.rrcReEstabTransCb.measGapStatus, 
                               transCb->transId);
            ueCb->anrMeasTransId = 0;
            wrUmmTransComplete(transCb);
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

/** @brief This function is called when a UE Capbility need to checked before
 *    configureing ANR related measurment configureation. 
 * @details
 *
 * Function: wrUmmAnrChkUeCap
 *
 * Description:
 *   This function is called when a UE Capbility need to checked before
 *   configureing ANR related measurment configureation.
 *
 * @param[in]  wrUeCb      : UE Control block 
 * @param[in]  cellFreqType: Neighbour cell Freq type (Inter-Freq or Intra-Freq) 
 * @param[in]  measAnrType :  ANR algo Type 
 * @return S16
 *  -#UE has support    : TRUE
 *  -#UE has no support : FALSE
 *  -#Failure           : FAIL
 */

PUBLIC S16 wrUmmAnrChkUeCap
(
 WrUeCb               *ueCb,
 WrUmmMeasCellTyp     cellFreqType,
 WrUmmMeasRptCfgType  measAnrType
)
{
   S16          ret = FALSE;

   TRC2(wrUmmAnrChkUeCap)

   /* FGI bit 5: Long DRX cycle, DRX command MAC control element */
   if(wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti, WR_UMM_LONG_DRX_SUPPORTED))
   {
      /* only if LongDrx feature is supported by UE,
       * then check for other capbilities */
      switch(measAnrType)
      {
         case WR_UMM_EVENT_A3_CFG:
           {
              /* FGI bit 16: Periodical measurement,reportStrongestCells:
              * Intra-frequency
              * Inter-frequency
              * Inter-RAT UTRAN FDD or UTRAN TDD */
              if( wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti,
                           WR_UMM_NON_ANR_PERD_MEAS_SUPPORTED))
              {
                 /* FGI bit 17: Intra-frequency ANR features
                 * periodical measurement, reportStrongestCells
                 * and reportCGI */
                 ret = wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti,
                           WR_UMM_ANR_PERD_INTRA_MEAS_SUPPORTED);
               }
               break;
           }
         case WR_UMM_STRONG_CELL_CFG:
         case WR_UMM_EVENT_A5_INTER_CFG:
           {
              /* FGI bit 16: Periodical measurement,reportStrongestCells:
              * Intra-frequency
              * Inter-frequency
              * Inter-RAT UTRAN FDD or UTRAN TDD */
              if(wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti,
                           WR_UMM_NON_ANR_PERD_MEAS_SUPPORTED))
              {
                 if(cellFreqType == WR_UMM_MEAS_INTRA_FREQ)
                 {
                    /* FGI bit 17: Intra-frequency ANR features
                    * periodical measurement, reportStrongestCells
                    * and reportCGI */
                    ret = wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti,
                              WR_UMM_ANR_PERD_INTRA_MEAS_SUPPORTED);

                 }
                 else if (cellFreqType == WR_UMM_MEAS_INTER_FREQ)
                 {
                    /* FGI bit 14: Event measurement reporting A4 and A5 */
                    if(wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti,
                                 WR_UMM_A4_A5_MEAS_SUPPORTED))
                    {
                       /* FGI bit 25:Inter-frequency measurements and 
                       * reporting in E-UTRA connected mode */
                       if(wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti,
                                    WR_UMM_INTER_FREQ_MEAS_SUPPORTED) )
                       {               
                          /* FGI bit 18: Inter-frequency ANR features
                          * periodical measurement, reportStrongestCells 
                          * and reportCGI */
                          ret = wrUmmUtilFeatureGrpInd(ueCb->cellId, 
                                    ueCb->crnti,
                                    WR_UMM_ANR_PERD_INTER_MEAS_SUPPORTED);

                       }
                    }
                 }
              }
              break;
           }
         default:
           {
              RLOG0(L_ERROR, "ANR Mesaurement Type is not supported or Invalid ");
              ret = FAIL;
             break;
           }
      } /* End of switch(measAnrType) */
   } /* End of Long DRX cycle support Check */
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
