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
               

     File:     wr_umm_meas.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=61;


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
/* LTE_ADV starts */
#include "wr_emm.h"
#include "wr_umm_lteadv.h"
/* LTE_ADV ends */
#include "wr_umm_drx.h"

#ifdef __cplusplus
extern "C" {
#endif

#define WR_UMM_MAX_REPORT_CELLS_VAL   1 
#define WR_UMM_REPORT_INTERVEL_VAL    0 
#define WR_UMM_REPORT_AMOUNT_VAL      0 
EXTERN S16 wrUmmTrgRrcReCfgForAllUesCfg(WrUmmCellCb *cellCb, U32 utraModFreq);
EXTERN WrNrUtranFreqCb* wrEmmAnrGetFreqNode
(
U16                     arfcnDl,
WrNrUtranCb             *utranCb
);

EXTERN Bool wrUmmPopUtraCsfbLaiHrlCells
(
 WrUeCb                      *ueCb,
 WrUmmMeasUtraMeasObjNode  *utraCellObj,
 WrUmmMeasTransInfo          *measTransInfo,
 Bool                      *isNewMeasObj
 );

EXTERN Bool wrUmmChkIfPscPrsntInMeasLst
(
 WrUmmMeasUtraMeasObjNode     *ueMeasObjNode,
 U32                          psc
);

EXTERN WrGeranCellCb* wrEmmGetNeighGeranCell
(
 U16 cellId,
 U16 arfcn, 
 U16 bsic
);
EXTERN S16 wrUmmUtraTddHdlMeasRpt
(
 WrUeCb               *ueCb,
 NhuMeasResults       *measRpt,
 WrUmmRedirectInfo    **redirInfo,
 WrUmmMeasUtraMeasObjNode     *ueMeasObjNode,
 WrUmmMeasTyp          measTyp
 );

PRIVATE S16 wrUmmMeasUpdtCdmaMeasObjAndIdFromArfcn
(
U8                           bandClass,
U32                          arfcn,
WrMeasUeCb                   *measUeCb,
U8                           *measObjId,
WrUmmMeasCdmaMeasObjNode     **cellCbMeasObj
);

EXTERN Bool wrUmmChkCdmaFgi
(
 U16                         cellId,
 U16                         crnti
);
EXTERN WrUmmMeasUtraMeasObjNode* wrUmmGetUtraMeasObjNode
(
U16           arfcn,
CmLListCp     utraMeasObjLst
);
EXTERN WrMeasUeCbUtraMeasObjNode* wrUmmGetUeCbUtraMeasObj
(
U16           arfcn,
CmLListCp     utraCellLst
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @brief This function is responsible for allocate the Free Id Node
 *        in UE Control Block.
 * 
 * @details
 *
 * Function: wrUmmMeasAllocMemForId
 *
 *   Processing steps:
 *    - allocate the Free Id Node.
 * 
 * @param[in]  measIDFreeLst: pointer to Free Id List.
 * @param[in]  idVal: Value to store in Free Id Node.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasAllocMemForId
(
 CmLListCp                   *measIDFreeLst,
 U8                          idVal
)
{
   WrMeasFreeNode            *idNode;

   TRC2(wrUmmMeasAllocMemForId)
   WR_ALLOC (&idNode, sizeof(WrMeasFreeNode));
   if(NULLP != idNode)
   {
      idNode->val = idVal;
      idNode->lnk.node = (PTR)idNode;
      cmLListAdd2Tail(measIDFreeLst, &idNode->lnk);
   }
   else
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for Deallocate the Free Id Node List in
 *  UE Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasDeAllocMemForId
 *
 *   Processing steps:
 *    - Deallocate the Free Id Node List.
 * 
 * @param[in]  measIDFreeLst: pointer to Free Id List 
 * @return Void
 */

PRIVATE Void wrUmmMeasDeAllocMemForId
(
 CmLListCp                   *measIDFreeLst
)
{
   WrMeasFreeNode            *idObj;
   CmLList                   *idNode;

   TRC2(wrUmmMeasDeAllocMemForId)
   
   idNode = measIDFreeLst->first;
   while(NULLP != idNode)
   {
      idObj = (WrMeasFreeNode*)idNode->node;
      cmLListDelFrm (measIDFreeLst, idNode);
      WR_FREE(idObj, sizeof(WrMeasFreeNode));
      idNode = measIDFreeLst->first;
   }
   RETVOID;
}


 /** @brief This function is responsible for to Find new PCI Frequency Node
  *  from cellCb CB
 *
 *  @details
 *  Function:wrUmmMeasGetNewPciFreqNode
 *    
 *  Processing steps:
 *      - search New PCi frequency node
 *  
 *   @param[out] measUeCb: pointer to Measurement Ue Cb.
 *   @param[in] measObjInfo: pointer to measuement Object info object.
 *   @return S16
 *       SUCCESS : ROK
*/
PRIVATE WrMeasNewPciNode* wrUmmMeasGetNewPciFreqNode
(
 WrUmmMeasObjInfo             *measObjInfo,
 CmLListCp                    *newPciFreqNode
)
{
   CmLList                   *lnk;
   WrMeasNewPciNode          *newPciNode;

   TRC2(wrUmmMeasGetNewPciFreqNode)
   
   if(measObjInfo->nrType == WR_UMM_MEAS_INTER_FREQ)
   {
      lnk = newPciFreqNode->first;
      while(lnk != NULLP)
      {
         newPciNode = (WrMeasNewPciNode*)lnk->node;

         if(newPciNode->arfcn == measObjInfo->eutraObj->eutraMeasObj->carrFreq)
         {
            RETVALUE(newPciNode);
         }
         lnk = lnk->next;
      }
   }
   else if(measObjInfo->nrType == WR_UMM_MEAS_UTRA_FDD_FREQ)
   {
      lnk = newPciFreqNode->first;
      while(lnk != NULLP)
      {
         newPciNode = (WrMeasNewPciNode*)lnk->node;

         if(newPciNode->arfcn == measObjInfo->utraFddObj->utraMeasObj->arfcn)
         {
            RETVALUE(newPciNode);
         }
         lnk = lnk->next;
      }
   }
   RETVALUE(NULLP);
}

 /** @brief This function is responsible for to Deleting new PCI Obj in Cell CB
 *
 *  @details
 *  Function:wrUmmMeasDelNewPciFrmFreqNode
 *      
 *  Processing steps:
 *      - Delete new PCI Node from Frequency Node
 *  
 *   @param[out] newPciNode: pointer to new PCI node.
 *   @param[in] pci: New Pci.
 *   @return S16
 *       SUCCESS : ROK
*/
PRIVATE S16 wrUmmMeasDelNewPciFrmFreqNode
( 
  CmLListCp                  *newPciLLst,
  U16                         pci
)
{
   CmLList                   *lnk;
   WrUmmMeasNewPciInfo       *newPciObj;

   TRC2(wrUmmMeasDelNewPciFrmFreqNode)

   lnk = newPciLLst->first;
   while(lnk != NULLP)
   {
      newPciObj = (WrUmmMeasNewPciInfo*)lnk->node;

      if(newPciObj->newPci == pci)
      {
         cmLListDelFrm(newPciLLst, &newPciObj->lnk);
         WR_FREE(newPciObj, sizeof(WrUmmMeasNewPciInfo));
         break;
      }
      lnk = lnk->next;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to delete New Pci from measurement Object 
 * after receiving report CGI measurement report.
 *
 *  @details*
 *  Function:wrUmmMeasCfgDelNewPciObj
 *    
 *  Processing steps:
 *      - Find the measurement Object from MeasId
 *      - Update the measurement Object
 *  
 *   @param[in] measObjId: measurement Object Id.
 *   @param[in] measCellCfg: pointer to measurement cell config.
 *   @param[in] measTransCb: pointer to measurement transaction Cb.
 *   @param[in] measUeCb: pointer to measurement ue Cb.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
*/
PUBLIC S16 wrUmmMeasCfgDelNewPciObj
(
  WrUmmMeasObjInfo           *measObjInfo,
  WrMeasUeCb                 *measUeCb
)
{
   WrMeasNewPciNode         *newPciNode;
   U16                      pci;

   TRC2(wrUmmMeasCfgDelNewPciObj)

   if(measObjInfo->nrType == WR_UMM_MEAS_INTRA_FREQ)
   {
      pci = measObjInfo->eutraObj->newPci;
      wrUmmMeasDelNewPciFrmFreqNode(&measUeCb->intraNewPci.newPciLst, pci);
      measObjInfo->eutraObj->newPci = 0xFFFF;
   }
   else if(measObjInfo->nrType == WR_UMM_MEAS_INTER_FREQ)
   {
      newPciNode = wrUmmMeasGetNewPciFreqNode(measObjInfo, &measUeCb->interNewPciLst);
      if(newPciNode != NULLP)
      {
         pci = measObjInfo->eutraObj->newPci;
         wrUmmMeasDelNewPciFrmFreqNode(&newPciNode->newPciLst, pci);
         measObjInfo->eutraObj->newPci = 0xFFFF;
         if(newPciNode->newPciLst.count == 0)
         {
            cmLListDelFrm(&measUeCb->interNewPciLst, &newPciNode->lnk);
            WR_FREE(newPciNode, sizeof(WrMeasNewPciNode));
         }
      }
   }
   else if(measObjInfo->nrType == WR_UMM_MEAS_UTRA_FDD_FREQ)
   {

      newPciNode = wrUmmMeasGetNewPciFreqNode(measObjInfo, &measUeCb->utranNewPscLst);
      if(newPciNode != NULLP)
      {
         pci = measObjInfo->utraFddObj->newPsc;
         wrUmmMeasDelNewPciFrmFreqNode(&newPciNode->newPciLst, pci);
         measObjInfo->utraFddObj->newPsc = 0xFFFF;
         if(newPciNode->newPciLst.count == 0)
         {
            cmLListDelFrm(&measUeCb->utranNewPscLst, &newPciNode->lnk);
            WR_FREE(newPciNode, sizeof(WrMeasNewPciNode));
         }
      }

   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for resetting the ANR transaction
 * parameters
 *
 *  @details
 *  Function: wrUmmMeasResetTransCb
 *    
 *  Processing steps:
 *      - intialize the parameters to '0'
 *  
 *   @param[in] WrUmmMeasTransInfo : pointer to measuement trans Cb.
 *   @return S16
 *       SUCCESS : Void
*/
PUBLIC Void wrUmmMeasResetTransCb
(
WrUmmMeasTransInfo *measTransCfg
)
{
   TRC2(wrUmmMeasResetTransCb)
   measTransCfg->addMeasInfo.measIdCnt = 0;
   measTransCfg->addMeasInfo.measObjIdCnt = 0;
   measTransCfg->addMeasInfo.rptCfgIdCnt = 0;
   measTransCfg->rmvMeasInfo.measIdRmvLstCnt = 0;
   measTransCfg->rmvMeasInfo.measObjIdRmvLstCnt = 0;
   measTransCfg->rmvMeasInfo.rptCfgRmvLstCnt = 0;
   measTransCfg->addMeasInfo.isEutraRptCgiFlg = FALSE;
   RETVOID;
}
/** @brief This function is responsible for intializing the measurement Control
 *              block Parameters in UE Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasUeCbInit
 *
 *   Processing steps:
 *    - Create Free List Nodes for MeasId.
 *    - Create Free List Nodes for Meas Object Id.
 *    - Create Free List Nodes for Report config Id.
 *    - If Fails Deallocates Allocated Memory and Return Fail.
 * 
 *
 * @param[in]  cellId: Cell Identifier
 * @param[in]  ueIndx: UE Index at application 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasUeCbInit
(
 WrUeCb *ueCb
)
{
    U8                       idCnt;
    U8                       idVal = 1;
    Bool                     isAllocFail = FALSE;
    WrMeasUeCb               *measUeCb;

    TRC2(wrUmmMeasCfgInitLList)
 
    WR_ALLOC(&ueCb->measCfg,sizeof(WrMeasUeCb));
    if(NULLP == ueCb->measCfg) 
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
       RETVALUE(RFAILED);
    }
     
    measUeCb = ueCb->measCfg;

    cmMemset((U8 *)measUeCb, 0,sizeof(WrMeasUeCb));
    measUeCb->rmvAnrMeasId = 0;
    measUeCb->isAnrMeasCfged = WR_UMM_NOT_CONFIGED;
    cmLListInit(&measUeCb->measIDFreeLLst); 
    cmLListInit(&measUeCb->measObjIDFreeLLst); 
    cmLListInit(&measUeCb->measRptCfgIDFreeLLst); 
    cmLListInit(&measUeCb->measIDUsedLLst); 
    cmLListInit(&measUeCb->measObjIDUsedLLst); 
    cmLListInit(&measUeCb->measRptCfgIDUsedLLst); 
    for (idCnt = 0; idCnt < WR_UMM_MEAS_MAX_NUM_IDS; idCnt++,++idVal )
    {
      if(ROK != wrUmmMeasAllocMemForId(&measUeCb->measIDFreeLLst, idVal))
      {
         isAllocFail = TRUE;
         break;
      }
      if(ROK != wrUmmMeasAllocMemForId(&measUeCb->measObjIDFreeLLst, idVal))
      {
         isAllocFail = TRUE;
         break;
      }
      if(ROK != wrUmmMeasAllocMemForId(&measUeCb->measRptCfgIDFreeLLst, idVal))
      {
         isAllocFail = TRUE;
         break;
      }
    }
    if(isAllocFail == TRUE)
    {
       wrUmmMeasDeAllocMemForId(&measUeCb->measIDFreeLLst);  
       wrUmmMeasDeAllocMemForId(&measUeCb->measObjIDFreeLLst);  
       wrUmmMeasDeAllocMemForId(&measUeCb->measRptCfgIDFreeLLst);  
       RLOG0(L_FATAL, "Memory allocation failed for Meas Id's.");
       RETVALUE(RFAILED);
    }

/* LTE_ADV starts */
    measUeCb->crnti  = ueCb->crnti;
    measUeCb->cellId = ueCb->cellId;
/* LTE_ADV ends */

   RETVALUE(ROK);
}  
/** @brief This function is responsible for Deallocate the inter Frequency Measurement
 * Object List in UE Control Block.
 * 
 * @details
 *
 * Function: wrUmmMeasDeAllocMemForInterMeasObj
 *
 *   Processing steps:
 *    - Deallocate the inter Frequency Measurement Objects List.
 * 
 * @param[in]  interMeasObjLst: pointer to inter Freq meas Object List 
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocMemForInterMeasObj
(
 CmLListCp                   *interMeasObjLst
)
{
   WrMeasUeCbEutraMeasObjNode   *interMeasObjNode;
   CmLList                         *interMeasLnk;

   TRC2(wrUmmMeasDeAllocMemForInterMeasObj)
   
   interMeasLnk = interMeasObjLst->first;
   while(NULLP != interMeasLnk)
   {
      interMeasObjNode = (WrMeasUeCbEutraMeasObjNode*)interMeasLnk->node;
      cmLListDelFrm (interMeasObjLst, interMeasLnk);
      WR_FREE(interMeasObjNode, sizeof(WrMeasUeCbEutraMeasObjNode));
      interMeasLnk = interMeasObjLst->first;
   }
   RETVOID;
}
/** @brief This function is responsible for Deallocate the inter Frequency Measurement
 * Object List in UE Control Block.
 * 
 * @details
 *
 * Function: wrUmmMeasDeAllocMemForGeranMeasObj
 *
 *   Processing steps:
 *    - Deallocate the Geran  Measurement Objects List.
 * 
 * @param[in]  geranMeasObjLst: pointer to Geran Freq meas Object List 
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocMemForGeranMeasObj
(
 CmLListCp                   *geranMeasObjLst
)
{
   WrMeasUeCbGeranMeasObjNode   *geranMeasObjNode;
   CmLList                         *geranMeasLnk;

   TRC2(wrUmmMeasDeAllocMemForGeranMeasObj)
   
   geranMeasLnk = geranMeasObjLst->first;
   while(NULLP != geranMeasLnk)
   {
      geranMeasObjNode = (WrMeasUeCbGeranMeasObjNode*)geranMeasLnk->node;
      cmLListDelFrm (geranMeasObjLst, geranMeasLnk);
      WR_FREE(geranMeasObjNode, sizeof(WrMeasUeCbGeranMeasObjNode));
      geranMeasLnk = geranMeasObjLst->first;
   }
   RETVOID;
}
/** @brief This function is responsible for Deallocate the inter RAT Measurement
 * Object List in UE Control Block.
 * 
 * @details
 *
 * Function: wrUmmMeasDeAllocMemForUtraMeasObj
 *
 *   Processing steps:
 *    - Deallocate the inter RAT Measurement Objects List.
 * 
 * @param[in]  interRatMeasObjLst: pointer to inter Rat meas Object List 
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocMemForUtraMeasObj
(
 CmLListCp                   *utraFddMeasObjLst
)
{
   WrUmmMeasUtraMeasObjNode    *utraFddMeasNode;
   CmLList                      *utraFddMeasLnk;

   TRC2(wrUmmMeasDeAllocMemForUtraMeasObj)
   
   utraFddMeasLnk = utraFddMeasObjLst->first;
   while(NULLP != utraFddMeasLnk)
   {
      utraFddMeasNode = (WrUmmMeasUtraMeasObjNode*)utraFddMeasLnk->node;
      cmLListDelFrm (utraFddMeasObjLst, utraFddMeasLnk);
      WR_FREE(utraFddMeasNode, sizeof(WrUmmMeasUtraMeasObjNode));
      utraFddMeasLnk = utraFddMeasObjLst->first;
   }
   RETVOID;
}
/** @brief This function is responsible for Deallocate the inter RAT Measurement
 * Object List in UE Control Block.
 * 
 * @details
 *
 * Function: wrUmmMeasDeAllocMemForInterRatMeasObj
 *
 *   Processing steps:
 *    - Deallocate the inter RAT Measurement Objects List.
 * 
 * @param[in]  interRatMeasObjLst: pointer to inter Rat meas Object List 
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocMemForInterRatMeasObj
(
 CmLListCp                   *interRatMeasObjLst
)
{
   WrMeasUeCbCdmaMeasObjNode    *interRatMeasNode;
   CmLList                      *interRatMeasLnk;

   TRC2(wrUmmMeasDeAllocMemForInterRatMeasObj)
   
   interRatMeasLnk = interRatMeasObjLst->first;
   while(NULLP != interRatMeasLnk)
   {
      interRatMeasNode = (WrMeasUeCbCdmaMeasObjNode*)interRatMeasLnk->node;
      cmLListDelFrm (interRatMeasObjLst, interRatMeasLnk);
      WR_FREE(interRatMeasNode, sizeof(WrMeasUeCbCdmaMeasObjNode));
      interRatMeasLnk = interRatMeasObjLst->first;
   }
   RETVOID;
}
/** @brief This function is responsible for Deallocate the Report Config List in
 *                              UE Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasDeAllocMemForRptCfgLst
 *
 *   Processing steps:
 *    - Deallocate the report config List.
 * 
 * @param[in]  rptCfgLst: pointer to report config List 
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocMemForRptCfgLst
(
 CmLListCp                   *rptCfgLst
)
{
   WrMeasRptCfgNode          *rptCfgNode;
   CmLList                   *rptCfgLnk;

   TRC2(wrUmmMeasDeAllocMemForRptCfgLst)
   
   rptCfgLnk = rptCfgLst->first;
   while(NULLP != rptCfgLnk)
   {
      rptCfgNode = (WrMeasRptCfgNode*)rptCfgLnk->node;
      cmLListDelFrm (rptCfgLst, rptCfgLnk);
      WR_FREE(rptCfgNode, sizeof(WrMeasRptCfgNode));
      rptCfgLnk = rptCfgLst->first;
   }
   RETVOID;
}
/** @brief This function is responsible for Deallocate the measurement Id List
 *     in UE Control Block.
 * 
 * @details
 *
 * Function: wrUmmMeasDeAllocMemForMeasIdLst
 *
 *   Processing steps:
 *    - Deallocate the Measurement Id List.
 * 
 * @param[in]  measIdLst: pointer to meas Id List 
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocMemForMeasIdLst
(
 CmLListCp                   *measIdLst
)
{
   WrMeasIdNode              *measIdNode;
   CmLList                   *measIdLnk;

   TRC2(wrUmmMeasDeAllocMemForMeasIdLst)
   
   measIdLnk = measIdLst->first;
   while(NULLP != measIdLnk)
   {
      measIdNode = (WrMeasIdNode*)measIdLnk->node;
      cmLListDelFrm (measIdLst, measIdLnk);
      WR_FREE(measIdNode, sizeof(WrMeasIdNode));
      measIdLnk = measIdLst->first;
   }
   RETVOID;
}
/** @brief This function is responsible for Deallocate the New Pci Freq Node in
 *  UE Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasDeAllocMemForNewPciNodes
 *
 *   Processing steps:
 *    - Deallocate the New Pci List.
 * 
 * @param[in]  newPciLst: pointer to New Pci List 
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocMemForNewPciNodes
(
 CmLListCp                   *newPciLst
)
{
   WrMeasNewPciNode          *newPciNode;
   CmLList                   *newPciLnk;

   TRC2(wrUmmMeasDeAllocMemForNewPciNodes)
   
   newPciLnk = newPciLst->first;
   while(NULLP != newPciLnk)
   {
      newPciNode = (WrMeasNewPciNode*)newPciLnk->node;
      cmLListDelFrm (newPciLst, newPciLnk);
      WR_FREE(newPciNode, sizeof(WrMeasNewPciNode));
      newPciLnk = newPciLst->first;
   }
   RETVOID;
}
/** @brief This function is responsible for Deallocate the New Pci Obj in
 *  UE Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasDeAllocMemForNewPciObj
 *
 *   Processing steps:
 *    - Deallocate the New Pci List.
 * 
 * @param[in]  newPciLst: pointer to New Pci Object
 * @return Void
 */
PUBLIC Void wrUmmMeasDeAllocMemForNewPciObj
(
 CmLListCp                   *newPciLst
)
{
   WrUmmMeasNewPciInfo       *newPciNode;
   CmLList                   *newPciLnk;

   TRC2(wrUmmMeasDeAllocMemForNewPciObj)
   
   newPciLnk = newPciLst->first;
   while(NULLP != newPciLnk)
   {
      newPciNode = (WrUmmMeasNewPciInfo*)newPciLnk->node;
      cmLListDelFrm (newPciLst, newPciLnk);
      WR_FREE(newPciNode, sizeof(WrUmmMeasNewPciInfo));
      newPciLnk = newPciLst->first;
   }
   RETVOID;
}

/** @brief This function is responsible for deleting all new pcis of given list. 
 *
 * @details
 *
 *  Function:wrUmmDelIntraFreqNonCfgNewPci
 *
 *  Processing steps:
 *    - delete the all the  new PCIs of given list that are not configured.
 *
 *  @param[in]  newPcilist: pointer to the  pci list.
 *
 *  @return Void 
 *
 */
PUBLIC Void wrUmmDelIntraFreqNonCfgNewPciObj
(
  CmLListCp                  *newPciLLst
)
{
   CmLList                   *lnk;
   WrUmmMeasNewPciInfo       *newPciObj;

   TRC2(wrUmmAnrDelIntraFreqNonCfgNewPci)

   if(newPciLLst == NULLP)
   {
      RLOG0(L_ERROR, "wrUmmDelIntraFreqNonCfgNewPciObj:newPciFreqNode is NULL");
      RETVOID;
   }

   lnk = newPciLLst->first;
   while(lnk != NULLP)
   {
      newPciObj = (WrUmmMeasNewPciInfo*)lnk->node;

      lnk = lnk->next;

      if(newPciObj->isConfigred == WR_UMM_NOT_CONFIGED)
      {
         cmLListDelFrm(newPciLLst, &newPciObj->lnk);
         WR_FREE(newPciObj, sizeof(WrUmmMeasNewPciInfo));
      }
   }

   RETVOID;
}

/** @brief This function is responsible for deleting all new pcis 
 *          of inter frequencies.
 *
 * @details
 *
 *  Function:wrUmmDelInterFreqNonCfgNewPciObj
 *
 *  Processing steps:
 *    - delete the all the  new PCIs of inter frequency
 *
 *  @param[in]  newPciFreqNode: pointer to the  pci list.
 *
 *  @return Void 
 */
PUBLIC Void wrUmmDelInterFreqNonCfgNewPciObj
(
 CmLListCp                    *newPciFreqNode
)
{
   CmLList                   *lnk;
   WrMeasNewPciNode          *newPciNode;

   TRC2(wrUmmDelInterFreqNonCfgNewPciObj)
  
   if(newPciFreqNode == NULLP)
   {
      RLOG0(L_ERROR, "wrUmmDelInterFreqNonCfgNewPciObj:newPciFreqNode is NULL");
      RETVOID;
   }

   lnk = newPciFreqNode->first;
   while(lnk != NULLP)
   {
      newPciNode = (WrMeasNewPciNode *)lnk->node;
      lnk = lnk->next;

      wrUmmDelIntraFreqNonCfgNewPciObj(&newPciNode->newPciLst);
      if(newPciNode->newPciLst.count == 0)
      {
         cmLListDelFrm(newPciFreqNode, &newPciNode->lnk);
         WR_FREE(newPciNode, sizeof(WrMeasNewPciNode));
      }
   }
   RETVOID;
}

/** @brief This function is responsible for deintializing the measurement Control
 *              block Parameters in UE Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasUeCbDeInit
 *
 *   Processing steps:
 *    - Deallocate Free and Used List Nodes for MeasId.
 *    - Deallocate Free and Used List Nodes for Meas Object Id.
 *    - Deallocate Free and Used List Nodes for Report config Id.
 *    - Deallocate Inter and InterRat List Nodes for Meas Objects.
 *    - Deallocate meas Id, Report Config and New Pci List Nodes.
 *    - Deallocate report CGI Nodes.
 * 
 * @param[in]  cellId: Cell Identifier
 * @param[in]  ueIndx: UE Index at application 
 * @return S16
 *    SUCCESS : ROK
 *    FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasUeCbDeInit
(
WrUeCb                    *ueCb
)
{
   WrMeasUeCb                *measUeCb;
   WrMeasNewPciNode          *newPciNode;
   CmLList                   *lnk;

   TRC2(wrUmmMeasUeCbDeInit)

    measUeCb = ueCb->measCfg;
    
    if(NULLP != measUeCb)
    {
       wrUmmMeasDeAllocMemForId(&measUeCb->measIDFreeLLst);  
       wrUmmMeasDeAllocMemForId(&measUeCb->measObjIDFreeLLst);  
       wrUmmMeasDeAllocMemForId(&measUeCb->measRptCfgIDFreeLLst);  
       wrUmmMeasDeAllocMemForId(&measUeCb->measIDUsedLLst);  
       wrUmmMeasDeAllocMemForId(&measUeCb->measRptCfgIDUsedLLst);  
       wrUmmMeasDeAllocMemForId(&measUeCb->measObjIDUsedLLst);  

       if(measUeCb->eutraRptCgiCfg != NULLP)
       {
          WR_FREE(measUeCb->eutraRptCgiCfg, sizeof(WrMeasRptCfgNode));
       }
       if(measUeCb->utraRptCgiCfg != NULLP)
       {
          WR_FREE(measUeCb->utraRptCgiCfg, sizeof(WrMeasRptCfgNode));
       }
    
       wrUmmMeasDeAllocMemForInterMeasObj(&measUeCb->interMeasObjLst);  
       wrUmmMeasDeAllocMemForInterRatMeasObj(&measUeCb->interRatMeasObjLst);  
       wrUmmMeasDeAllocMemForUtraMeasObj(&measUeCb->utraFddMeasObjLst);  
       wrUmmMeasDeAllocMemForUtraMeasObj(&measUeCb->utraTddMeasObjLst);  
       wrUmmMeasDeAllocMemForGeranMeasObj(&measUeCb->geranMeasObjLst);  
       wrUmmMeasDeAllocMemForMeasIdLst(&measUeCb->measIdLst);  
       wrUmmMeasDeAllocMemForRptCfgLst(&measUeCb->rptCfgLst);  
       wrUmmMeasDeAllocMemForNewPciObj(&measUeCb->intraNewPci.newPciLst);
       for(lnk = measUeCb->interNewPciLst.first;
             NULLP != lnk;
             lnk = lnk->next)
       {
          newPciNode = (WrMeasNewPciNode*)lnk->node;
          wrUmmMeasDeAllocMemForNewPciObj(&newPciNode->newPciLst);
       }
       wrUmmMeasDeAllocMemForNewPciNodes(&measUeCb->interNewPciLst);

       WR_FREE(measUeCb, sizeof(WrMeasUeCb));
    }
   RETVALUE(ROK);
}
/** @brief This function is responsible for Deallocate the Eutra White cell List
 * in UE Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasDeAllocUtraCellList
 *
 *   Processing steps:
 *    - Deallocate the Utran Cell List.
 * 
 * @param[in]  wlCellLst: pointer to Utra Cell List 
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocUtraCellList
(
 CmLListCp                   *wlCellLst
)
{
   WrMeasNrUtraWCellNode     *wlCellNode;
   CmLList                   *wlCellLnk;

   TRC2(wrUmmMeasDeAllocUtraCellList)
   
   wlCellLnk = wlCellLst->first;
   while(NULLP != wlCellLnk)
   {
      wlCellNode = (WrMeasNrUtraWCellNode*)wlCellLnk->node;
      cmLListDelFrm (wlCellLst, wlCellLnk);
      WR_FREE(wlCellNode, sizeof(WrMeasNrUtraWCellNode));
      wlCellLnk = wlCellLst->first;
   }
   RETVOID;
}
/** @brief This function is responsible for Deallocate the Eutra White cell List
 * in UE Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasDeAllocEutraWlCellList
 *
 *   Processing steps:
 *    - Deallocate the Eutra White Cell List.
 * 
 * @param[in]  wlCellLst: pointer to Eutra White Cell List 
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocEutraWlCellList
(
 CmLListCp                   *wlCellLst
)
{
   WrMeasNrWCellNode         *wlCellNode;
   CmLList                   *wlCellLnk;

   TRC2(wrUmmMeasDeAllocEutraWlCellList)
   
   wlCellLnk = wlCellLst->first;
   while(NULLP != wlCellLnk)
   {
      wlCellNode = (WrMeasNrWCellNode*)wlCellLnk->node;
      cmLListDelFrm (wlCellLst, wlCellLnk);
      WR_FREE(wlCellNode, sizeof(WrMeasNrWCellNode));
      wlCellLnk = wlCellLst->first;
   }
   RETVOID;
}

/** @brief This function is responsible for Deallocate the Eutra Black cell List
 * in UE Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasDeAllocEutraBlCellList
 *
 *   Processing steps:
 *    - Deallocate the Eutra Black Cell List.
 * 
 * @param[in]  blCellLst: pointer to Eutra Black Cell List 
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocEutraBlCellList
(
 CmLListCp                   *blCellLst
)
{
   WrMeasNrBCellNode         *blCellNode;
   CmLList                   *blCellLnk;

   TRC2(wrUmmMeasDeAllocEutraBlCellList)
   
   blCellLnk = blCellLst->first;
   while(NULLP != blCellLnk)
   {
      blCellNode = (WrMeasNrBCellNode*)blCellLnk->node;
      cmLListDelFrm (blCellLst, blCellLnk);
      WR_FREE(blCellNode, sizeof(WrMeasNrBCellNode));
      blCellLnk = blCellLst->first;
   }
   RETVOID;
}

/** @brief This function is responsible for Deallocate the Cdma cell List
 * in UE Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasDeAllocCdmaCellList
 *
 *   Processing steps:
 *    - Deallocate the Cdma Cell List.
 * 
 * @param[in]  cdmaCellLst: pointer to Cdma Cell List 
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocCdmaCellList
(
 CmLListCp                   *cdmaCellLst
)
{
   WrMeasNrCdmaCellNode      *cellNode;
   CmLList                   *cellLnk;

   TRC2(wrUmmMeasDeAllocCdmaCellList)
   
   cellLnk = cdmaCellLst->first;
   while(NULLP != cellLnk)
   {
      cellNode = (WrMeasNrCdmaCellNode*)cellLnk->node;
      cmLListDelFrm (cdmaCellLst, cellLnk);
      WR_FREE(cellNode, sizeof(WrMeasNrCdmaCellNode));
      cellLnk = cdmaCellLst->first;
   }
   RETVOID;
}
/** @brief This function is responsible for Deallocate the Geran
 * measurement Object List in Cell Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasDeAllocGeranMeasObj
 *
 *   Processing steps:
 *    - Deallocate the UTRAN Frequency Meas Object List.
 * 
 * @param[in]  geranMeasObjLst: pointer to Geran Meas Object List.
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocGeranMeasObj
(
 CmLListCp                   *geranMeasObjLst
)
{
   WrUmmMeasGeranMeasObjNode  *geranMeasNode;
   CmLList                    *geranMeasLnk;
   
   TRC2(wrUmmMeasDeAllocGeranMeasObj)
   
   geranMeasLnk = geranMeasObjLst->first;
   while(NULLP != geranMeasLnk)
   {
      geranMeasNode = (WrUmmMeasGeranMeasObjNode*)geranMeasLnk->node;
      cmLListDelFrm (geranMeasObjLst, geranMeasLnk);
      WR_FREE(geranMeasNode, sizeof(WrUmmMeasGeranMeasObjNode));
      geranMeasLnk = geranMeasObjLst->first;
   }
   RETVOID;
}
/** @brief This function is responsible for Deallocate the UTRA Frequency
 * measurement Object List in Cell Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasDeAllocUtraMeasObj
 *
 *   Processing steps:
 *    - Deallocate the UTRAN Frequency Meas Object List.
 * 
 * @param[in]  utranMeasObjLst: pointer to Utran Frequency Meas Object List.
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocUtraMeasObj
(
 CmLListCp                   *utranMeasObjLst
)
{
   WrUmmMeasUtraMeasObjNode  *utraMeasNode;
   CmLList                    *utraMeasLnk;
   
   TRC2(wrUmmMeasDeAllocUtraMeasObj)
   
   utraMeasLnk = utranMeasObjLst->first;
   while(NULLP != utraMeasLnk)
   {
      utraMeasNode = (WrUmmMeasUtraMeasObjNode*)utraMeasLnk->node;
      wrUmmMeasDeAllocUtraCellList(&utraMeasNode->cellLst);
      cmLListDelFrm (utranMeasObjLst, utraMeasLnk);
      WR_FREE(utraMeasNode, sizeof(WrUmmMeasUtraMeasObjNode));
      utraMeasLnk = utranMeasObjLst->first;
   }
   RETVOID;
}
/** @brief This function is responsible for Deallocate the Inter Frequency
 * measurement Object List in Cell Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasDeAllocInterMeasObj
 *
 *   Processing steps:
 *    - Deallocate the Inter Frequency Meas Object List.
 * 
 * @param[in]  interMeasObjLst: pointer to Inter Frequency Meas Object List.
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocInterMeasObj
(
 CmLListCp                   *interMeasObjLst
)
{
   WrUmmMeasEutraMeasObjNode  *eutraMeasNode;
   CmLList                    *eutraMeasLnk;
   
   TRC2(wrUmmMeasDeAllocInterMeasObj)
   
   eutraMeasLnk = interMeasObjLst->first;
   while(NULLP != eutraMeasLnk)
   {
      eutraMeasNode = (WrUmmMeasEutraMeasObjNode*)eutraMeasLnk->node;
      wrUmmMeasDeAllocEutraWlCellList(&eutraMeasNode->wlCellLst);
      wrUmmMeasDeAllocEutraBlCellList(&eutraMeasNode->blCellLst);
      cmLListDelFrm (interMeasObjLst, eutraMeasLnk);
      WR_FREE(eutraMeasNode, sizeof(WrUmmMeasEutraMeasObjNode));
      eutraMeasLnk = interMeasObjLst->first;
   }
   RETVOID;
}

/** @brief This function is responsible for Deallocate the Inter RAT Frequency
 * measurement Object List in Cell Control Block.
 * 
 * @details
 *
 * Function:wrUmmMeasDeAllocCdmaMeasObj
 *
 *   Processing steps:
 *    - Deallocate the Inter RAT Frequency Meas Object List.
 * 
 * @param[in]  cdmaRatMeasObjLst: pointer to Inter RAT Frequency Meas Object List.
 * @return Void
 */
PRIVATE Void wrUmmMeasDeAllocCdmaMeasObj
(
 CmLListCp                   *cdmaRatMeasObjLst
)
{
   WrUmmMeasCdmaMeasObjNode  *cdmaMeasNode;
   CmLList                   *cdmaMeasLnk;

   TRC2(wrUmmMeasDeAllocCdmaMeasObj)
   
   cdmaMeasLnk = cdmaRatMeasObjLst->first;
   while(NULLP != cdmaMeasLnk)
   {
      cdmaMeasNode = (WrUmmMeasCdmaMeasObjNode*)cdmaMeasLnk->node;
      wrUmmMeasDeAllocCdmaCellList(&cdmaMeasNode->cdmaCellInfo);
      cmLListDelFrm (cdmaRatMeasObjLst, cdmaMeasLnk);
      WR_FREE(cdmaMeasNode, sizeof(WrUmmMeasCdmaMeasObjNode));
      cdmaMeasLnk = cdmaRatMeasObjLst->first;
   }
   RETVOID;
}

PRIVATE S16 wrUmmMeasCellReCfgParamValidate
(
 LwrCellMeasCfgGrp            *cellCfgParams, 
 LwrCellMeasCfgGrp            *measReCfg
)
{
   RETVALUE(ROK); 
}

PUBLIC S16 wrUmmMeasCellReCfgParams
(
 LwrCellMeasCfgGrp            *cellCfgParams, 
 LwrCellMeasCfgGrp            *measReCfg
)
{
   U32                       rCfgType = 0;
   U32                       mask = 1;
   S16                       ret = ROK;
  
   TRC2(wrUmmMeasCellReCfgParams)
   
   ret = wrUmmMeasCellReCfgParamValidate(cellCfgParams, measReCfg);

   if (ROK != ret)
   {
      RLOG1(L_ERROR, "Measurement Re-config validate function failed[%d]", ret);
      RETVALUE(RFAILED);
   }
   rCfgType =  measReCfg->reCfgType;
   
   while (rCfgType)
   {
      if(rCfgType & mask)
      {
         switch(mask)
         {
            case LWR_CELL_MEAS_CFG_A1EVT_RPT:
            {  
               rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->measA1EvtRptCfg, (U8*)&measReCfg->
                        measA1EvtRptCfg, sizeof(LwrMeasEvntRptCfg));
               break;
            }
            case LWR_CELL_MEAS_CFG_A2EVT_RPT:
            {
               rCfgType =  rCfgType ^ mask; 
              cmMemcpy((U8*)&cellCfgParams->measA2EvtRptCfg, (U8*)&measReCfg->
                        measA2EvtRptCfg, sizeof(LwrMeasEvntRptCfg));
               break;
            }
            case LWR_CELL_MEAS_CFG_A4EVT_RPT:
            {
               rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->measA4EvtRptCfg, (U8*)&measReCfg->
                        measA4EvtRptCfg, sizeof(LwrMeasEvntRptCfg));
               break;
            }
            case LWR_CELL_MEAS_CFG_EVT_A3:
            {
               rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->measEvtA3Cfg, (U8*)&measReCfg->
                        measEvtA3Cfg, sizeof(LwrMeasEvntA3RptCfg));
               break;
            }
            case LWR_CELL_MEAS_CFG_EVT_A5:
            {
               rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->measEvtA5Cfg, (U8*)&measReCfg->
                         measEvtA5Cfg, sizeof(LwrMeasEvntA5RptCfg));
               break;
            }
            case LWR_CELL_MEAS_CFG_EUTRA_PERD_RPT:
            {
               rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->measEutraPerdRptCfg, (U8*)&measReCfg->
                        measEutraPerdRptCfg, sizeof(LwrMeasEutraPeridRptCfg));
               break;
            }
            case LWR_CELL_MEAS_CFG_1XCDMA_EVT_B1_RPT:
            {
               rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->meas1xCDMAEventB1RptCfg, (U8*)&measReCfg->
                        meas1xCDMAEventB1RptCfg, sizeof(LwrMeasCDMAEvntB1RptCfg));
               break;
            }
            case LWR_CELL_MEAS_CFG_CDMA_HRPD_EVT_B1_RPT:
            {
               rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->measCDMAHrpdEventB1RptCfg, (U8*)&measReCfg->
                        measCDMAHrpdEventB1RptCfg, sizeof(LwrMeasCDMAEvntB1RptCfg));
               break;
            }
            case LWR_CELL_MEAS_CFG_1XCDMA_EVT_B2_RPT:
            {
               rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->meas1xCDMAEventB2RptCfg, (U8*)&measReCfg->
                        meas1xCDMAEventB2RptCfg, sizeof(LwrMeasCDMAEvntB2RptCfg));
               break;
            }
            case LWR_CELL_MEAS_CFG_CDMA_HRPD_EVT_B2_RPT:
            {
               rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->measCDMAHrpdEventB2RptCfg, (U8*)&measReCfg->
                        measCDMAHrpdEventB2RptCfg, sizeof(LwrMeasCDMAEvntB2RptCfg));
               break;
            }
            case LWR_CELL_MEAS_CFG_1XCDMA_PERIOD_RPT:
            { 
                 rCfgType =  rCfgType ^ mask; 
                cmMemcpy((U8*)&cellCfgParams->meas1xCDMAPeridRptCfg, (U8*)&measReCfg->
                         meas1xCDMAPeridRptCfg, sizeof(LwrMeasCDMAPeridRptCfg));
               break;
            }
            case LWR_CELL_MEAS_CFG_CDMA_HRPD_PERIOD_RPT:
            {
               rCfgType =  rCfgType ^ mask; 
              cmMemcpy((U8*)&cellCfgParams->measCDMAHrpdPeridRptCfg, (U8*)&measReCfg->
                       measCDMAHrpdPeridRptCfg, sizeof(LwrMeasCDMAPeridRptCfg));
               break;
            }
            case LWR_CELL_MEAS_CFG_SPD_SCL_FACT:
            {
                rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->measSpdSclFact, (U8*)&measReCfg->
                        measSpdSclFact, sizeof(LwrMeasSpdStateScaleFact));
               break;
            }
            case LWR_CELL_MEAS_CFG_MOB_STAT_PARAMS:
            {
                rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->mobStatParams, (U8*)&measReCfg->
                        mobStatParams, sizeof(LwrMeasMobilityStatParams));
               break;
            }
            case LWR_CELL_MEAS_CFG_QTY_CFG:
            {
                rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->qtyCfg, (U8*)&measReCfg->qtyCfg,
                        sizeof(LwrMeasQtyCfgParams));
               break;
            }
            case LWR_CELL_MEAS_CFG_MEAS_GAP:
            {
                rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->measGap, (U8*)&measReCfg->measGap,
                         sizeof(LwrMeasGapConfig));
               break;
            }
            case LWR_CELL_MEAS_CFG_PRE_REG_HRPD:
            {
                rCfgType =  rCfgType ^ mask; 
               cmMemcpy((U8*)&cellCfgParams->preRegHrpd, (U8*)&measReCfg->preRegHrpd,
                         sizeof(LwrMeasPreRegHrpd));
               break;
            }
            default:
            {   
               RLOG0(L_ERROR,"Measurement Re-config received unknown reCfgType");
               break;
            }
         } /* end of switch */
      }
      mask = mask << 1;
   } /* end of while */
 
   RETVALUE(ROK);
} /* end of wrUmmMeasCellReCfgParams */


/** @brief This function is responsible for Initializing Cell Measurement
 * configuration.
 * 
 * @details
 *
 * Function:wrUmmMeasFillCellCfgParams
 *
 *   Processing steps:
 *    - Initialize the cell Meas Config Object.
 * 
 * @param[in]  cellCb: pointer to cell Control block.
 * @param[in]  rcvMeasCfgParams: pointer to Received Cell Measurement config Params 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasFillCellCfgParams
(
 WrUmmCellCb                  *cellCb,
 LwrCellMeasCfgGrp            *rcvMeasCfgParams
)
{
   LwrCellMeasCfgGrp            *cellCfgParams; 
   TRC2(wrUmmMeasFillCellCfgParams)

   cellCfgParams = cellCb->measCellCfg;

   cmMemcpy((U8*)&cellCfgParams->measA1EvtRptCfg, (U8*)&rcvMeasCfgParams->
         measA1EvtRptCfg, LWR_MEAS_MAX_EVENT_A1_CFG * sizeof(LwrMeasEvntRptCfg));
   cmMemcpy((U8*)&cellCfgParams->measA2EvtRptCfg, (U8*)&rcvMeasCfgParams->
         measA2EvtRptCfg, LWR_MEAS_MAX_EVENT_A2_CFG * sizeof(LwrMeasEvntRptCfg));
   cmMemcpy((U8*)&cellCfgParams->measA4EvtRptCfg, (U8*)&rcvMeasCfgParams->
         measA4EvtRptCfg, LWR_MEAS_MAX_EVENT_A4_CFG * sizeof(LwrMeasEvntRptCfg));
   cmMemcpy((U8*)&cellCfgParams->measEvtA3Cfg, (U8*)&rcvMeasCfgParams->
         measEvtA3Cfg, LWR_MEAS_MAX_EVENT_A3_CFG * sizeof(LwrMeasEvntA3RptCfg));
   cmMemcpy((U8*)&cellCfgParams->measEvtA5Cfg, (U8*)&rcvMeasCfgParams->
         measEvtA5Cfg, LWR_MEAS_MAX_EVENT_A5_CFG * sizeof(LwrMeasEvntA5RptCfg));
   cmMemcpy((U8*)&cellCfgParams->measEutraPerdRptCfg, (U8*)&rcvMeasCfgParams->
     measEutraPerdRptCfg, LWR_MEAS_MAX_EUTRA_PERD_CFG * sizeof(LwrMeasEutraPeridRptCfg));
   cmMemcpy((U8*)&cellCfgParams->meas1xCDMAEventB1RptCfg, (U8*)&rcvMeasCfgParams->
       meas1xCDMAEventB1RptCfg, LWR_MEAS_MAX_EVENT_B1_CFG * sizeof(LwrMeasCDMAEvntB1RptCfg));
   cmMemcpy((U8*)&cellCfgParams->measCDMAHrpdEventB1RptCfg, (U8*)&rcvMeasCfgParams->
       measCDMAHrpdEventB1RptCfg, LWR_MEAS_MAX_EVENT_B1_CFG * sizeof(LwrMeasCDMAEvntB1RptCfg));
   cmMemcpy((U8*)&cellCfgParams->meas1xCDMAEventB2RptCfg, (U8*)&rcvMeasCfgParams->
       meas1xCDMAEventB2RptCfg, LWR_MEAS_MAX_1XCDMA_EVENT_B2_CFG * sizeof(LwrMeasCDMAEvntB2RptCfg));
   cmMemcpy((U8*)&cellCfgParams->measCDMAHrpdEventB2RptCfg, (U8*)&rcvMeasCfgParams->
      measCDMAHrpdEventB2RptCfg, LWR_MEAS_MAX_CDMA_HRPD_EVENT_B2_CFG * sizeof(LwrMeasCDMAEvntB2RptCfg));
   cmMemcpy((U8*)&cellCfgParams->meas1xCDMAPeridRptCfg, (U8*)&rcvMeasCfgParams->
                  meas1xCDMAPeridRptCfg, LWR_MEAS_MAX_CDMA_PERD_CFG * sizeof(LwrMeasCDMAPeridRptCfg));
   cmMemcpy((U8*)&cellCfgParams->measCDMAHrpdPeridRptCfg, (U8*)&rcvMeasCfgParams->
                  measCDMAHrpdPeridRptCfg, LWR_MEAS_MAX_CDMA_PERD_CFG * sizeof(LwrMeasCDMAPeridRptCfg));
   cmMemcpy((U8*)&cellCfgParams->measUtraPerdRptCfg, (U8*)&rcvMeasCfgParams->
                  measUtraPerdRptCfg, LWR_MEAS_MAX_UTRA_PERD_CFG * sizeof(LwrMeasUtraPeriodRptCfg));
   cmMemcpy((U8*)&cellCfgParams->measUtraEventB2RptCfg, (U8*)&rcvMeasCfgParams->
                  measUtraEventB2RptCfg, LWR_MEAS_MAX_UTRA_EVNT_B2_CFG * sizeof(LwrMeasUtraEvntB2RptCfg));

   cmMemcpy((U8*)&cellCfgParams->measGeranEventB2RptCfg, (U8*)&rcvMeasCfgParams->
                  measGeranEventB2RptCfg, LWR_MEAS_MAX_GERAN_EVNT_B2_CFG * sizeof(LwrMeasGeranEvntB2RptCfg));
   
   /* Added RLOG to print all the Geran B2 Parameters */
   RLOG1(L_INFO, "GeranB2.b2Threshold1RSRP = %d:", cellCfgParams->\
         measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR].b2Threshold1RSRP); 
   RLOG1(L_INFO, "GeranB2.b2Threshold1RSRQ = %d:", cellCfgParams->\
         measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR].b2Threshold1RSRQ); 
   RLOG1(L_INFO, "GeranB2.b2Threshold2Geran = %d:", cellCfgParams->\
         measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR].b2Threshold2Geran); 
   RLOG1(L_INFO, "GeranB2.hysteresis = %d:", cellCfgParams->\
         measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR].hysteresis); 
   RLOG1(L_INFO, "GeranB2.timeToTrigger = %d:", cellCfgParams->\
         measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR].timeToTrigger); 
   RLOG1(L_INFO, "GeranB2.maxReportCell = %d:", cellCfgParams->\
         measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR].maxReportCell); 
   RLOG1(L_INFO, "GeranB2.reportInterval = %d:", cellCfgParams->\
         measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR].reportInterval); 
   RLOG1(L_INFO, "GeranB2.reportAmount = %d:", cellCfgParams->\
         measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR].reportAmount); 
   cmMemcpy((U8*)&cellCfgParams->measGERANPeridRptCfg, (U8*)&rcvMeasCfgParams->
         measGERANPeridRptCfg ,LWR_MEAS_MAX_GERAN_PERD_CFG * sizeof(LwrMeasGERANPeridRptCfg));
   cmMemcpy((U8*)&cellCfgParams->measSpdSclFact, (U8*)&rcvMeasCfgParams->
                  measSpdSclFact, sizeof(LwrMeasSpdStateScaleFact));
   cmMemcpy((U8*)&cellCfgParams->mobStatParams, (U8*)&rcvMeasCfgParams->
                  mobStatParams, sizeof(LwrMeasMobilityStatParams));
   cmMemcpy((U8*)&cellCfgParams->qtyCfg, (U8*)&rcvMeasCfgParams->qtyCfg,
         sizeof(LwrMeasQtyCfgParams));
   cmMemcpy((U8*)&cellCfgParams->measGap, (U8*)&rcvMeasCfgParams->measGap,
         sizeof(LwrMeasGapConfig));

   cellCb->measCellCb->anrRptCfg = rcvMeasCfgParams->anrRptCfg;
   /* If any invalid vlaue configured default ANR algo will be selected. 
    * i.e."A3 Event based Intra ANR" */
   if((cellCb->measCellCb->anrRptCfg != WR_UMM_STRONG_CELL_CFG)    &&
         (cellCb->measCellCb->anrRptCfg != WR_UMM_EVENT_A3_CFG) &&
         (cellCb->measCellCb->anrRptCfg != WR_UMM_NO_ANR_CFG)   &&
         (cellCb->measCellCb->anrRptCfg != WR_UMM_EVENT_A5_INTER_CFG)) 
   {
      RLOG0(L_WARNING,"Invalid ANR Report config received and setting default A3");
      cellCb->measCellCb->anrRptCfg = WR_UMM_EVENT_A3_CFG;
   }
   cellCb->measCellCb->hoRptCfg = rcvMeasCfgParams->hoRptCfg;
   if((cellCb->measCellCb->hoRptCfg != WR_UMM_EVENT_A5_CFG) &&
         (cellCb->measCellCb->hoRptCfg != WR_UMM_EVENT_A3_CFG))
   {
      RLOG0(L_WARNING,"Invalid HO Report config received and setting default A3");
      cellCb->measCellCb->hoRptCfg = WR_UMM_EVENT_A3_CFG;
   }

   /*rsrq_ho*/
   cellCb->measCellCfg->rsrqHoThrshld = rcvMeasCfgParams->rsrqHoThrshld; 
   if( cellCb->measCellCfg->rsrqHoThrshld > WR_UMM_RSRQ_RANGE_MAX )
   {
      /* Assign Default value and proceed */
      cellCb->measCellCfg->rsrqHoThrshld = WR_UMM_RSRQ_DFLT_VAL;
      RLOG1(L_WARNING, "Invalid RSRQ range!!Assigning Dflt val[%d]", WR_UMM_RSRQ_DFLT_VAL);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for deallocate cell Measurement control
 * block.
 * 
 * @details
 *
 * Function:wrUmmMeasDeAllocCellCfgReq
 *
 *   Processing steps:
 *    - Deallocate cell measurement control block meas Objects.
 * 
 * @param[in]  cellId: Cell identifier.
 * @return 
 *     SUCCESS : ROK
 *     FAILED  : RFAILED
*/
PUBLIC S16 wrUmmMeasDeAllocCellCfgReq
(
 U16                         cellId
)
{
   WrUmmCellCb               *cellCb;
   WrUmmMeasCellCb           *measCellCb;
   TRC2(wrUmmMeasDeAllocCellCfgReq)

   WR_UMM_GET_CELLCB(cellCb, cellId)

   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }
   measCellCb = cellCb->measCellCb;
 
   WR_FREE(cellCb->measCellCfg, sizeof(LwrCellMeasCfgGrp));

   wrUmmMeasDeAllocInterMeasObj(&measCellCb->interMeasObjLst);
   wrUmmMeasDeAllocUtraMeasObj(&measCellCb->utraFddMeasObjLst);
   wrUmmMeasDeAllocUtraMeasObj(&measCellCb->utraTddMeasObjLst);
   wrUmmMeasDeAllocCdmaMeasObj(&measCellCb->cdma1xMeasObjLst);
   wrUmmMeasDeAllocCdmaMeasObj(&measCellCb->cdmaHrpdMeasObjLst);
   wrUmmMeasDeAllocGeranMeasObj(&measCellCb->geranMeasObjLst);
   WR_FREE(measCellCb, sizeof(WrUmmMeasCellCb));
   RETVALUE(ROK);
}

/** @brief This function is responsible for preparing and sending 
 *          RRC Reconfiguration message.
 * 
 * @details
 *
 * Function:wrUmmMeasPrepareSendRrcRecfg
 *
 *   Processing steps:
 *    - Validate the inputs received. 
 *    - Fill Nhu Message 
 *    - send the message.
 * 
 * @param [in] NhuDatReqSdus: pointer to the RRC Reconfig message.
 * @param [in] cellId: Cell identifier.
 * @param [in] WrMeasUeCb: Measurement UE Cb                
 * @param [in] WrUmmMeasTransInfo: Measurement transaction Info         
 * @param [in] transId : Transaction Id
 * @return 
 *     SUCCESS : ROK
 *     FAILED  : RFAILED
*/
PUBLIC S16 wrUmmMeasPrepareSendRrcRecfg
(
   U16                        cellId, 
   WrUeCb                    *ueCb,
   WrUmmMeasTransInfo        *measTransCb,
   U32                        transId
)
{
   WrUmmCellCb               *cellCb;
   NhuMeasConfig             *nhuMeasCfg;
   NhuDatReqSdus             *rrcReCfgMsg; 

   RLOG0(L_DEBUG, "Function wrUmmMeasPrepareSendRrcRecfg");
   
   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "Cell Cb is NULL ");
      RETVALUE(RFAILED);
   }

   if(wrUmmMeasAllocRecfgMsg(&rrcReCfgMsg, measTransCb, ueCb, transId) == ROK)
   {
      /* Fill the RRC Reconfiguration message */
      wrUmmMeasGetNhuMeasCfgFrmReCfg(rrcReCfgMsg, &nhuMeasCfg);

      if(ROK == wrMeasFillNhuMsg(&rrcReCfgMsg->memCp, nhuMeasCfg,
                                    ueCb, measTransCb, 
                                    cellCb->measCellCfg, 
                                    transId))
      {
         RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending "
               "RRCConnectionReconfiguration for Measurement");
         /* Send the message to the UE */
         wrIfmRrcSndReqToUe(rrcReCfgMsg); 
      }
      else 
      {
         RLOG0(L_ERROR, "wrUmmMeasPrepareSendRrcRecfg: wrMeasFillNhuMsg Failed");
         WR_FREEEVNT(rrcReCfgMsg); 
         RETVALUE(RFAILED);
      }
   }
   else 
   {
      /* Memory not allocated */
      RLOG0(L_ERROR, "wrUmmMeasPrepareSendRrcRecfg: Memory not allocated");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
/** @brief This function is responsible for get free cell index for Eutra
 * measurement Object.
 * 
 *  @details
 * 
 *  Function:wrUmmMeasGetFreeCellIdxForEutra
 *   Processing steps:
 *     - Traverse Free Cell index array and update the free cell index.
 *   
 *    @param[in]   cellLst: pointer to Eutra white Cell List.
 *    @param[out]  cellIdx: pointer to free cell index Id.
 *    @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *        
 */
PRIVATE S16 wrUmmMeasGetFreeCellIdxForEutra 
(
 Bool                        *cellLst,
 U8                          *cellIdx
)
{
   U8                        idCnt;
   Bool                      isCellIdxAvail = FALSE; 

   TRC2(wrUmmMeasGetFreeCellIdxForEutra)

   for(idCnt = 0; idCnt < WR_UMM_MEAS_MAX_CELL_IDX; idCnt++)
   {
      if(cellLst[idCnt] == FALSE)
      {
         cellLst[idCnt] = TRUE;
         *cellIdx = idCnt + 1;
         isCellIdxAvail = TRUE; 
         break;
      }
   }
   if(isCellIdxAvail == TRUE)
   {
      RETVALUE(ROK);
   }
   else
   {
      RLOG0(L_ERROR, "Cell Index not available");
      RETVALUE(RFAILED);
   }
}

/** @brief This function is responsible for filling Eutra White cell list 
 * measurement Object.
 * 
 *  @details
 * 
 *  Function:wrUmmMeasFillEutraWLCellLst
 *   Processing steps:
 *     - Fill Eutra White Cell List measurement Objects in cell Control block.
 *   
 *    @param[out]  eutraMeasObj: pointer to Eutra measurement Object.
 *    @param[in]   pci: Physical cell Id.
 *    @param[in]   cio: cell individual Offset.
 *    @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasFillEutraWLCellLst
(
 WrUmmMeasEutraMeasObjNode   *eutraMeasObj,
 U16                         pci,
 U32                         cio
)
{
   WrMeasNrWCellNode         *nrWCellObj;
   U8                        cellIdx = 0;

   TRC2(wrUmmMeasFillEutraWLCellLst)
   if(RFAILED == wrUmmMeasGetFreeCellIdxForEutra(eutraMeasObj->wlCellIdxInUse, &cellIdx))
   {
      RLOG0(L_ERROR, "Cell Index not available");
      RETVALUE(RFAILED);
   }
   WR_ALLOC(&nrWCellObj, sizeof(WrMeasNrWCellNode));
   if(NULLP == nrWCellObj)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      eutraMeasObj->wlCellIdxInUse[cellIdx - 1] = FALSE;
      RETVALUE(RFAILED);
   }
   nrWCellObj->cellIndex = cellIdx;
   nrWCellObj->pci = pci;
   nrWCellObj->cio = cio;
   nrWCellObj->lnk.node = (PTR)nrWCellObj;
   cmLListAdd2Tail(&eutraMeasObj->wlCellLst, &nrWCellObj->lnk);      
   RETVALUE(RFAILED);
}
/** @brief This function is responsible for filling Eutra Black cell list 
 * measurement Object.
 * 
 *  @details
 * 
 *  Function:wrUmmMeasFillEutraBLCellLst
 *   Processing steps:
 *     - Fill Eutra Black Cell List measurement Objects in cell Control block.
 *   
 *    @param[out]  eutraMeasObj: pointer to Eutra measurement Object.
 *    @param[in]   pci: Physical cell Id.
 *    @param[in]   cio: cell individual Offset.
 *    @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasFillEutraBLCellLst
(
 WrUmmMeasEutraMeasObjNode   *eutraMeasObj,
 U16                         startPci,
 U8                          pciRange
)
{
   U8                        cellIdx = 0;
   WrMeasNrBCellNode         *nrBCellObj;

   TRC2(wrUmmMeasFillEutraBLCellLst)
   if(RFAILED == wrUmmMeasGetFreeCellIdxForEutra(eutraMeasObj->
                   blCellIdxInUse, &cellIdx))
   {
      RLOG0(L_ERROR, "Cell Index not available");
      RETVALUE(RFAILED);
   }  
   WR_ALLOC(&nrBCellObj, sizeof(WrMeasNrBCellNode));
   if(NULLP == nrBCellObj)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      eutraMeasObj->blCellIdxInUse[cellIdx - 1] = FALSE;
      RETVALUE(RFAILED);
   }
   nrBCellObj->cellIndex = cellIdx;
   nrBCellObj->startPhyCellID = startPci;
   nrBCellObj->phyCellIdRange = pciRange;
   nrBCellObj->lnk.node = (PTR)nrBCellObj;
   cmLListAdd2Tail(&eutraMeasObj->blCellLst, &nrBCellObj->lnk);      
   RETVALUE(ROK);
}

/** @brief This function is responsible for filling Eutra measurement Object.
 * 
 *  @details
 * 
 *  Function:wrUmmMeasFillEutraMeasObj 
 *   Processing steps:
 *     - Fill Eutra frequency measurement Objects in cell Control block.
 *   
 *    @param[in]   arfcn: Carrier Frequency.
 *    @param[out]  eutraMeasObj: pointer to Eutra measurement Object.
 *    @param[in]   eutraCfgParams: pointer to Eutra configuration Parameters.
 *    @param[in]   wlCellInfo: pointer to White list Cell Information.
 *    @param[in]   blCellInfo: pointer to Black list Cell Information.
 *    @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *        
 */
PRIVATE S16 wrUmmMeasFillEutraMeasObj 
(
 U32                         arfcn,
 WrUmmMeasEutraMeasObjNode   *eutraMeasObj,
 LwrNeighEutraFreq           *eutraCfgParams,
 WrNrPerFreqWlInfo           *wlCellInfo,
 WrNrPerFreqBlInfo           *blCellInfo
)
{
   U8                        idCnt;

   TRC2(wrUmmMeasFillEutraMeasObj)
      
   eutraMeasObj->carrFreq = arfcn;
   eutraMeasObj->allowedMeasBW = eutraCfgParams->measBandwidth.val;
   eutraMeasObj->presAntPort1 = eutraCfgParams->presAntennaPort1;
   eutraMeasObj->neighCellCfg = eutraCfgParams->neighCellCfg;
   eutraMeasObj->qOffsetFreq = eutraCfgParams->offsetFreq;

   if(NULLP != wlCellInfo)
   {
      U8                        wlCellCnt;
      wlCellCnt = wlCellInfo->numWlCells;
      for(idCnt = 0; idCnt < wlCellCnt; idCnt++)
      {
         U16                   pci;
         U32                   cio;

         pci = wlCellInfo->cellInfo[idCnt].phyCellId;
         cio = wlCellInfo->cellInfo[idCnt].cio;
         wrUmmMeasFillEutraWLCellLst(eutraMeasObj, pci, cio);
      }
   }
   if(NULLP != blCellInfo)
   {
      U8                        blCellCnt;
      blCellCnt = blCellInfo->numBlInfos;
      for(idCnt = 0; idCnt < blCellCnt; idCnt++)
      {
         U16                   startPci;
         U8                    pciRange;

         startPci = blCellInfo->blInfo[idCnt].startPhyCellID;
         pciRange = blCellInfo->blInfo[idCnt].phyCellIdRange;
         wrUmmMeasFillEutraBLCellLst(eutraMeasObj, startPci,  pciRange);
      }
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for filling intra Frequency Object in
 * cell control block.
 * 
 *  @details
 *  
 *   Function:wrUmmMeasCreatIntraCellCbMeasObj
 *  
 *   Processing steps:
 *    - Fill intra frequency measurement Object in cell Control block.
 *   
 *   @param[in]  arfcn: Arfcn Value.
 *   @param[in]  cellCb: measurement cell Control block.
 *   @param[in]  measCellCfg: measurement cell configuration.
 *   @return Void
 *        
 */

PRIVATE Void wrUmmMeasCreatIntraCellCbMeasObj
(
 U16                         arfcn,
 WrUmmCellCb                 *cellCb,
 LwrNeighEutraFreq           *eutraCfgParams
)
{
   WrUmmMeasEutraMeasObjNode   *eutraMeasObj;

   TRC2(wrUmmMeasCreatIntraCellCbMeasObj)
   eutraMeasObj = &cellCb->measCellCb->intraMeasObj;
   
   wrUmmMeasFillEutraMeasObj(arfcn, eutraMeasObj, eutraCfgParams, NULLP, NULLP);
   RETVOID;
}

/** @brief This function is responsible for to create Eutra measurement object
 *  in Cell Cb. 
 *
 *  @details
 * Function: wrUmmMeasCreatEutraMeasObjInCellCb
 *
 *   Processing steps:
 *    - create new object in cell Cb
 *     
 * @param[out] interMeasObjLst: pointer to inter measurement object list         
 * @param[in]  eutraCfgParams: pointer to Eutra config Params 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasCreatEutraMeasObjInCellCb
(
 CmLListCp                   *interMeasObjLst,
 LwrNeighEutraFreq           *eutraCfgParams
)
{
   U32                         arfcn; 
   WrUmmMeasEutraMeasObjNode   *interEutraObj;

   TRC2(wrUmmMeasCreatEutraMeasObjInCellCb)

   arfcn = eutraCfgParams->earfcnDl; 
   WR_ALLOC(&interEutraObj, sizeof(WrUmmMeasEutraMeasObjNode));
   if(NULLP == interEutraObj)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   wrUmmMeasFillEutraMeasObj(arfcn, interEutraObj,eutraCfgParams, NULLP, NULLP);
   
   interEutraObj->lnk.node = (PTR)interEutraObj;
   cmLListAdd2Tail(interMeasObjLst, &interEutraObj->lnk);
   RETVALUE(ROK);
}
/** @brief This function is responsible for get free cell index for Cdma
 * measurement Object.
 * 
 *  @details
 * 
 *  Function:wrUmmMeasGetFreeCellIdxForCdma
 *   Processing steps:
 *     - Traverse Free Cell index array and update the free cell index.
 *   
 *    @param[in]   cdmaCellLst: pointer to Cdma Cell List.
 *    @param[out]  cellIdx: pointer to free cell index Id.
 *    @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *        
 */
PRIVATE S16 wrUmmMeasGetFreeCellIdxForCdma 
(
 Bool                        *cdmaCellLst,
 U8                          *cellIdx
)
{
   U8                        idCnt;
   Bool                      isCellIdxAvail = FALSE; 

   TRC2(wrUmmMeasGetFreeCellIdxForCdma)

   for(idCnt = 0; idCnt < WR_UMM_MEAS_MAX_CELL_IDX; idCnt++)
   {
      if(cdmaCellLst[idCnt] == FALSE)
      {
         cdmaCellLst[idCnt] = TRUE;
         *cellIdx = idCnt + 1;
         isCellIdxAvail = TRUE;
         break;
      }
   }
   if(isCellIdxAvail == TRUE)
   {
      RETVALUE(ROK);
   }
   else
   {
      RLOG0(L_ERROR, "Cell Index not available");
      RETVALUE(RFAILED);
   }
}

/** @brief This function is responsible for filling inter RAT Frequency Objects
 * cell information.
 *
 * @details
 *
 *  Function:wrUmmMeasFillCdmaCellsIntoMeaObj
 * 
 *  Processing steps:
 *    - Fill CDMA frequency measurement Objects cell information.
 * 
 *  @param[out]  measObjCdma2k: pointer to Cdma2000 measurement Object.
 *  @param[in]   pci: Physical cell identifier.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 * **/
 PRIVATE S16 wrUmmMeasFillCdmaCellsIntoMeaObj
 (
  WrUmmMeasCdmaMeasObjNode     *measObjCdma2k,
  U16                          pci
 )
{
   WrMeasNrCdmaCellNode     *cdma2kCellObj;
   U8                        cellIdx = 0;
   TRC2(wrUmmMeasFillCdmaCellsIntoMeaObj)

      /*Get free index from measObjCdma2k->cdmaCellIdxInUs */
   if(RFAILED == wrUmmMeasGetFreeCellIdxForCdma(measObjCdma2k->cdmaCellIdxInUse, &cellIdx))
   {
      RLOG0(L_ERROR, "Cdma Free CellId Index failed");
      RETVALUE(RFAILED);          
   }
   WR_ALLOC(&cdma2kCellObj, sizeof(WrMeasNrCdmaCellNode));
   if(NULLP == cdma2kCellObj)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      measObjCdma2k->cdmaCellIdxInUse[cellIdx] = FALSE;
      RETVALUE(RFAILED);
   }
   cdma2kCellObj->cellIndex = cellIdx;
   cdma2kCellObj->pci = pci;
   cdma2kCellObj->lnk.node = (PTR)cdma2kCellObj;
   /*Add the PCI list Info in wrUmmCellCb->measCellCb->cdma1xMeasObjLst
    * All PCI configured for a particular (band,frequency) is added in
    * cdmaCellInfo List */
   cmLListAdd2Tail(&measObjCdma2k->cdmaCellInfo, &cdma2kCellObj->lnk);      
   RETVALUE(ROK);
}
/** @brief This function is responsible for filling inter RAT Frequency Objects
 * in cell control block.
 *
 * @details
 *
 *  Function:wrUmmMeasFill1xCdmaMeasObj 
 * 
 *  Processing steps:
 *    - Fill 1x CDMA frequency measurement Objects in cell Control block.
 * 
 *  @param[out]  measObjCdma2k: pointer to Cdma2000 measurement Object.
 *  @param[in]   cdmaCfgParams: pointer to 1xCdma2000 configuration Parameters.
 *  @param[in]   cdma2kCellInfo: pointer to Cdma2000 Cell Information.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */

PRIVATE S16 wrUmmMeasFill1xCdmaMeasObj 
(
 WrUmmMeasCdmaMeasObjNode    *measObjCdma2k,
 LwrNeighCdma1xFreq          *cdmaCfgParams,
 WrCDMA2KFreqInfo            *cdma2kCellInfo
)
{
    U8                       idCnt;
    U8                       cdmaCellsCnt;

    TRC2(wrUmmMeasFill1xCdmaMeasObj)
    measObjCdma2k->cdmaType = NhuCDMA2000_Typtype1XRTTEnum;
    measObjCdma2k->bandClass = cdmaCfgParams->bandClass;
    measObjCdma2k->arfcn = cdmaCfgParams->arfcn;
    measObjCdma2k->qOffsetRangeInterRAT = cdmaCfgParams->offsetFreq;
    measObjCdma2k->searchWindowSize = cdmaCfgParams->searchRate;

    if(NULLP != cdma2kCellInfo)
    {
       cdmaCellsCnt = cdma2kCellInfo->wlData.numWlCells;
       for (idCnt = 0; idCnt < cdmaCellsCnt; idCnt++)
       {
          U16                      pci;

          pci = cdma2kCellInfo->wlData.pciInfoLst[idCnt].pnOffset;
          wrUmmMeasFillCdmaCellsIntoMeaObj(measObjCdma2k, pci);
       } 
    }
   RETVALUE(ROK);
}
/** @brief This function is responsible for filling inter RAT Frequency Objects
 * in cell control block.
 *
 * @details
 *
 *  Function:wrUmmMeasFillCdmaHrpdMeasObj 
 * 
 *  Processing steps:
 *    - Fill CDMA frequency measurement Objects in cell Control block.
 * 
 *  @param[out]  measObjCdma2k: pointer to Cdma2000 measurement Object.
 *  @param[in]   cdmaCfgParams: pointer to Cdma2000 configuration Parameters.
 *  @param[in]   cdma2kCellInfo: pointer to Cdma2000 Cell Information.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */

PRIVATE S16 wrUmmMeasFillCdmaHrpdMeasObj 
(
 WrUmmMeasCdmaMeasObjNode    *measObjCdma2k,
 LwrNeighCdmaHRPDFreq        *cdmaCfgParams,
 WrCDMA2KFreqInfo            *cdma2kCellInfo
)
{
    U8                       idCnt;
    U8                       cdmaCellsCnt;

    TRC2(wrUmmMeasFillCdmaHrpdMeasObj)
    measObjCdma2k->cdmaType = NhuCDMA2000_TyptypeHRPDEnum;
    measObjCdma2k->bandClass = cdmaCfgParams->bandClass;
    measObjCdma2k->arfcn = cdmaCfgParams->arfcn;
    measObjCdma2k->qOffsetRangeInterRAT = cdmaCfgParams->offsetFreq;
    measObjCdma2k->searchWindowSize = cdmaCfgParams->searchRate;

    if(NULLP != cdma2kCellInfo)
    {
       cdmaCellsCnt = cdma2kCellInfo->wlData.numWlCells;
       for (idCnt = 0; idCnt < cdmaCellsCnt; idCnt++)
       {
          U16                      pci;

          pci = cdma2kCellInfo->wlData.pciInfoLst[idCnt].pnOffset;
          wrUmmMeasFillCdmaCellsIntoMeaObj(measObjCdma2k, pci);
       } 
    }
   RETVALUE(ROK);
}
/** @brief This function is responsible for to update cdma measurement object
 *  from ue cb. 
 *
 *  @details
 * Function:wrUmmMeasCreat1xCdmaMeasObjInCellCb
 *
 *   Processing steps:
 *    - tranverse cell Cb list find measurement object using Arfcn.
 *    -if Object not found create new object in cell Cb
 *     
 * @param[out] interRatMeasObjLst: pointer to interRat measurement object list         
 * @param[in]  cdmaCfgParams:  poiner to cdma config params 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasCreat1xCdmaMeasObjInCellCb
(
 CmLListCp                   *interRatMeasObjLst,
 LwrNeighCdma1xFreq          *cdmaCfgParams
)
{
   WrUmmMeasCdmaMeasObjNode    *interRatObj;

   TRC2(wrUmmMeasCreat1xCdmaMeasObjInCellCb)

   WR_ALLOC(&interRatObj, sizeof(WrUmmMeasCdmaMeasObjNode));
   if(NULLP == interRatObj)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   wrUmmMeasFill1xCdmaMeasObj( interRatObj, cdmaCfgParams, NULLP);
   interRatObj->lnk.node = (PTR)interRatObj;
   cmLListAdd2Tail(interRatMeasObjLst, &interRatObj->lnk);
   RETVALUE(ROK);
}

/** @brief This function is responsible for to update Utra measurement object
 *  from ue cb. 
 *
 *  @details
 * Function:wrUmmMeasCreatUtraMeasObjInCellCb
 *
 *   Processing steps:
 *    - tranverse cell Cb list find measurement object using Arfcn.
 *    -if Object not found create new object in cell Cb
 *     
 * @param[out] utraObjLst: pointer to utra measurement object list         
 * @param[in]  utraCfgParams:  poiner to utra config params 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasCreatUtraMeasObjInCellCb
(
 CmLListCp                   *utraMeasObjLst,
 LwrNeighUtraFreq            *utraCfgParams
)
{
   WrUmmMeasUtraMeasObjNode    *utraObj;

   TRC2(wrUmmMeasCreatUtraMeasObjInCellCb)

   WR_ALLOC(&utraObj, sizeof(WrUmmMeasUtraMeasObjNode));
   if(NULLP == utraObj)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   utraObj->duplexMode = utraCfgParams->duplexMode;
   if(WR_UTRA_MODE_FDD == utraObj->duplexMode)
   {
      utraObj->arfcn = utraCfgParams->t.utraFdd.arfcnDl;
   }
   else
   {
      utraObj->arfcn = utraCfgParams->t.utraTdd.arfcn;
      utraObj->tddMode = utraCfgParams->t.utraTdd.tddMode;
      utraObj->tddBandIndicator = utraCfgParams->t.utraTdd.bandIndicator;
   }
   utraObj->qOffsetFreq = utraCfgParams->offsetFreq;

   utraObj->lnk.node = (PTR)utraObj;
   cmLListAdd2Tail(utraMeasObjLst, &utraObj->lnk);
   RETVALUE(ROK);
}

/** @brief This function is responsible for to update cdma measurement object
 *  from ue cb. 
 *
 *  @details
 * Function:wrUmmMeasCreatCdmaHrpdMeasObjInCellCb
 *
 *   Processing steps:
 *    - tranverse cell Cb list find measurement object using Arfcn.
 *    -if Object not found create new object in cell Cb
 *     
 * @param[out] interRatMeasObjLst: pointer to interRat measurement object list         
 * @param[in]  cdmaCfgParams:  poiner to cdma config params 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasCreatCdmaHrpdMeasObjInCellCb
(
 CmLListCp                   *interRatMeasObjLst,
 LwrNeighCdmaHRPDFreq        *cdmaCfgParams
)
{
   WrUmmMeasCdmaMeasObjNode    *interRatObj;

   TRC2(wrUmmMeasCreatCdmaHrpdMeasObjInCellCb)

   WR_ALLOC(&interRatObj, sizeof(WrUmmMeasCdmaMeasObjNode));
   if(NULLP == interRatObj)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   wrUmmMeasFillCdmaHrpdMeasObj(interRatObj, cdmaCfgParams, NULLP);
   interRatObj->lnk.node = (PTR)interRatObj;
   cmLListAdd2Tail(interRatMeasObjLst, &interRatObj->lnk);
   RETVALUE(ROK);
}

/* DYNAMIC_OAM and HRL interaction changes -start */

/** @brief This function is responsible for addition or modification 
 * for Eutra Measurement Objects in HRL UeCb. 
 *
 * @details
 *
 *  Function: wrUmmMeasAddModEutraMeasObjInHrlUeCb 
 * 
 *  Processing steps:
 *    - modify the Measurement object in HRL UE Cb if measObj is found
 *    - else add a new measObj in measObjLst
 * 
 * @param[out] cellCb: pointer to ue Cb         
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasAddModEutraMeasObjInHrlUeCb
(
 WrUmmCellCb                 *cellCb,
 LwrNeighEutraFreq           *eutraCfgParams
)
{
   Bool                        measObjFnd = FALSE;
   U8                          ueCnt;
   WrUeCb                      *ueCb;
   WrUmmMeasEutraMeasObjNode   *eutraMeasObj = NULLP;

   TRC2(wrUmmMeasAddModEutraMeasObjInHrlUeCb)

      for(ueCnt = 0; ueCnt < cellCb->maxUesPerCell; ueCnt++)
      {
         ueCb = cellCb->ueCbs[ueCnt];

         /* Check whether ueCb have hrlMeasCfg or not */
         if((NULLP != ueCb) && (NULLP != ueCb->hoRstrctInfo))
         {
            /* As of now,Inter meas object is only getting modified, so we will
             * check only ueCb->hrlMeasCfg.interMeasObjLst.count */

            if(ueCb->hrlMeasCfg.interMeasObjLst.count != 0)
            {
               cmLListFirst(&ueCb->hrlMeasCfg.interMeasObjLst);

               while(cmLListCrnt(&ueCb->hrlMeasCfg.interMeasObjLst))
               {
                  eutraMeasObj = (WrUmmMeasEutraMeasObjNode *) cmLListNode\
                              (cmLListCrnt(&ueCb->hrlMeasCfg.interMeasObjLst));

                  if(eutraCfgParams->earfcnDl == eutraMeasObj->carrFreq)
                  {
                     /* After fetching the eutraMeasObj,update the latest 
                      * information for modified frequency */
                     eutraMeasObj->allowedMeasBW =  eutraCfgParams->measBandwidth.val;
                     eutraMeasObj->presAntPort1 = eutraCfgParams->presAntennaPort1;
                     eutraMeasObj->neighCellCfg = eutraCfgParams->neighCellCfg;
                     eutraMeasObj->qOffsetFreq = eutraCfgParams->offsetFreq;
                     measObjFnd = TRUE;
                     break;
                  }
                  cmLListNext(&ueCb->hrlMeasCfg.interMeasObjLst);
               }

               if ( FALSE == measObjFnd )
               {
                  /* This is required to add new measObj on the addition of 
                   * new frequency */
                  WR_ALLOC(&eutraMeasObj, sizeof(WrUmmMeasEutraMeasObjNode));
                  if(eutraMeasObj == NULLP)
                  {
                     RLOG0(L_FATAL, "Memory allocation failed.");
                     RETVALUE(NULLP);
                  }

                  eutraMeasObj->carrFreq = eutraCfgParams->earfcnDl;
                  eutraMeasObj->allowedMeasBW = eutraCfgParams->measBandwidth.val;
                  eutraMeasObj->presAntPort1 = eutraCfgParams->presAntennaPort1;
                  eutraMeasObj->neighCellCfg = eutraCfgParams->neighCellCfg;
                  eutraMeasObj->qOffsetFreq = eutraCfgParams->offsetFreq;
                  WR_SET_ZERO(eutraMeasObj->wlCellIdxInUse, (WR_UMM_MEAS_MAX_CELL_IDX * sizeof(Bool)));
                  WR_SET_ZERO(eutraMeasObj->blCellIdxInUse, (WR_UMM_MEAS_MAX_CELL_IDX * sizeof(Bool)));
                  eutraMeasObj->lnk.node = (PTR)eutraMeasObj;
                  cmLListAdd2Tail(&ueCb->hrlMeasCfg.interMeasObjLst, &eutraMeasObj->lnk);
               }
            }
         }
      }
   RETVALUE(ROK);
}
/* DYNAMIC_OAM and HRL interaction changes -end */
 
/** @brief This function is responsible for allocate and Initializing memory in
 * Cell Control Block for neighbour frequency object
 * 
 * @details
 *
 * Function:wrUmmMeasAllocNrGeranObj
 *
 *   Processing steps:
 *    - Allocate Memory and Initialize for neighbor frequency Object.
 * 
 * @param[in]  cellCb: pointer to Cell CB.
 * @param[in]  numFreq: number of frequency.
 * @param[in]  cellFreqCfg: pointer to cell frequeny config Params
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasAllocNrGeranObj
(
 WrUmmCellCb                 *cellCb,
 U8                          numFreq,
 WrUmmMeasGeranMeasInfo      *cellFreqCfg
)
{
   U8                        idCnt;
   WrUmmMeasCellCb           *measCellCb;
   WrUmmMeasGeranMeasObjNode   *geranObj;
   TRC2(wrUmmMeasAllocNrGeranObj)

   measCellCb = cellCb->measCellCb;
   if(numFreq != 0)
   {
      WR_ALLOC(&geranObj, sizeof(WrUmmMeasGeranMeasObjNode));
      if(NULLP == geranObj)
      {
         RLOG0(L_FATAL, "Failed to allocate Geran memory.");
         RETVALUE(RFAILED);
      }
      geranObj->arfcnCnt = numFreq;
      for(idCnt = 0; idCnt < numFreq && idCnt < WR_UMM_MEAS_MAX_GERAN_ARFCN; idCnt++)
      {
         geranObj->arfcn[idCnt] = cellFreqCfg[idCnt].arfcn;
         geranObj->bsic[idCnt] = cellFreqCfg[idCnt].bsic;
         geranObj->nccPermitted[idCnt] = cellFreqCfg[idCnt].nccPermitted;
         geranObj->qOffset = cellFreqCfg->qOffset;
         geranObj->bandIndicator[idCnt] = cellFreqCfg[idCnt].bandIndicator;
      }/*end of for loop*/
      geranObj->lnk.node = (PTR)geranObj;
      cmLListAdd2Tail(&measCellCb->geranMeasObjLst, &geranObj->lnk);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for allocate and Initializing memory in
 * Cell Control Block for neighbour frequency object
 * 
 * @details
 *
 * Function:wrUmmMeasAllocNrFreqCfg
 *
 *   Processing steps:
 *    - Allocate Memory and Initialize for neighbor frequency Object.
 * 
 * @param[in]  cellCb: pointer to Cell CB.
 * @param[in]  numFreq: number of frequency.
 * @param[in]  cellFreqCfg: pointer to cell frequeny config Params
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasAllocNrFreqCfg
(
 WrUmmCellCb                 *cellCb,
 U8                          numFreq,
 LwrNeighFreqAddModCfg       *cellFreqCfg
)
{
   U16                       arfcn;
   U8                        idCnt;
   WrUmmMeasCellCb           *measCellCb;
   TRC2(wrUmmMeasAllocNrFreqCfg)

      if(NULLP == cellFreqCfg)
      {
         RLOG0(L_ERROR, "Neighbor Frequency info is NULL");
         RETVALUE(RFAILED);
      }
   measCellCb = cellCb->measCellCb;

   for(idCnt = 0; idCnt < numFreq; idCnt++)
   {
      switch(cellFreqCfg[idCnt].type)
      {
         case WR_NEIGH_CELL_EUTRA:

            arfcn = wrEmmGetServCarrierFreq(cellCb->cellId);
            if(arfcn == cellFreqCfg[idCnt].t.eutranFreq.earfcnDl)
            {
               wrUmmMeasCreatIntraCellCbMeasObj (arfcn, cellCb, &cellFreqCfg[idCnt].
                     t.eutranFreq);
            }
            else
            {
               if(RFAILED == wrUmmMeasCreatEutraMeasObjInCellCb( &measCellCb->
                        interMeasObjLst, &cellFreqCfg[idCnt].t.eutranFreq))
               {
                  RLOG0(L_ERROR, "Eutra Neighbor Frequency Failed");
                  RETVALUE(RFAILED);
               }
               /* DYNAMIC_OAM and HRL interaction changes -start */
               if(RFAILED == wrUmmMeasAddModEutraMeasObjInHrlUeCb(cellCb, 
                        &cellFreqCfg[idCnt].t.eutranFreq)) 
               {
                  RLOG0(L_ERROR, "Eutra Meas Obj addition in HRL UeCB failed");
                  RETVALUE(RFAILED);
               }
               /* DYNAMIC_OAM and HRL interaction changes -end */
            }
            break;

         case WR_NEIGH_CELL_UTRA_FDD:
            if(RFAILED == wrUmmMeasCreatUtraMeasObjInCellCb(&measCellCb->
                     utraFddMeasObjLst, &cellFreqCfg[idCnt].t.utranFreq))
            {
               RLOG0(L_ERROR, "Utra FDD Neighbor Frequency Failed");
               RETVALUE(RFAILED);
            }
            break;

         case WR_NEIGH_CELL_UTRA_TDD:
            if(RFAILED == wrUmmMeasCreatUtraMeasObjInCellCb(&measCellCb->
                     utraTddMeasObjLst, &cellFreqCfg[idCnt].t.utranFreq))
            {
               RLOG0(L_ERROR, "Utra TDD Neighbor Frequency Failed");
               RETVALUE(RFAILED);
            }
            break;
         case WR_NEIGH_CELL_CDMA2K_1XRTT:
            if(RFAILED == wrUmmMeasCreat1xCdmaMeasObjInCellCb(&measCellCb->
                     cdma1xMeasObjLst, &cellFreqCfg[idCnt].t.cdma1xFreq))
            {
               RLOG0(L_ERROR, "1xCdma Neighbor Frequency Failed");
               RETVALUE(RFAILED);
            }
            break;
         case WR_NEIGH_CELL_GERAN:
            {
            }
            break;
         default:
            if(RFAILED == wrUmmMeasCreatCdmaHrpdMeasObjInCellCb(&measCellCb->
                     cdmaHrpdMeasObjLst, &cellFreqCfg[idCnt].t.cdmaHrpdFreq))
            {
               RLOG0(L_ERROR, "Cdma Hrpd Neighbor Frequency Failed");
               RETVALUE(RFAILED);
            }

            break; 
      }/*end of switch*/
   }/*end of for loop*/
   RETVALUE(ROK);
}
/** @brief This function is responsible for allocate and Initializing memory in
 * Cell Control Block for Cell Measurement config.
 * 
 * @details
 *
 * Function:wrUmmMeasAllocCellCfg
 *
 *   Processing steps:
 *    - Allocate Memory and Initialize for cell Meas Config
 *    Object.
 * 
 * @param[in]  cellCb: pointer to Cell CB.
 * @param[in]  measCfgParams: pointre to measurement config Params
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasAllocCellCfg
(
 WrUmmCellCb                 *cellCb,
 LwrCellMeasCfgGrp           *measCfgParams
)
{

   TRC2(wrUmmMeasAllocCellCfg)
   
   if(NULLP == measCfgParams)
   {
      RLOG0(L_ERROR, "Meas Config params received as NULLP");
      RETVALUE(RFAILED);
   }
   if(cellCb->measCellCfg != NULLP)
   {
      if(ROK != wrUmmMeasFillCellCfgParams(cellCb, measCfgParams))
      {
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for to set Free report config Id.
 *
 *  @details
 *  Function:wrUmmMeasSetFreeRptCfgId
 *    
 *  Processing steps:
 *      - Find report config node from report config Id
 *      - delete from used list and add to free list
 *      
 *   @param[out] measUeCb: measurement Ue control block.
 *   @param[in] rptCfgId: report config Id.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/

PUBLIC S16 wrUmmMeasSetFreeRptCfgId
(
WrMeasUeCb                   *measUeCb,
U8                           rptCfgId
)
{
   WrMeasFreeNode            *delObj;
   CmLList                   *delNode;

   TRC2(wrUmmMeasSetFreeRptCfgId)
   delNode = measUeCb->measRptCfgIDUsedLLst.first;
   while(NULLP != delNode)
   {
      delObj = (WrMeasFreeNode*)delNode->node;
      if (rptCfgId == delObj->val)
      {
         cmLListDelFrm(&measUeCb->measRptCfgIDUsedLLst, &delObj->lnk);
         cmLListAdd2Tail(&measUeCb->measRptCfgIDFreeLLst, &delObj->lnk);
         break;
      }
      delNode = delNode->next;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to Get Free report config Id.
 *
 *  @details
 *  Function:wrUmmMeasGetFreeRptCfgId
 *    
 *  Processing steps:
 *      - Find report config Node from report config Id
 *      - delete from Free list and add to Used list
 *      
 *   @param[in] measUeCb: measurement Ue control block.
 *   @param[out] rptCfgId: pointer to report config Id.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/

PUBLIC S16 wrUmmMeasGetFreeRptCfgId
(
WrMeasUeCb                   *measUeCb,
U8                           *rptCfgId
)
{
   WrMeasFreeNode            *delObj;
   CmLList                   *delNode;

   TRC2(wrUmmMeasGetFreeRptCfgId)
   delNode = measUeCb->measRptCfgIDFreeLLst.first;
   if(NULLP != delNode)
   {
      delObj = (WrMeasFreeNode*)delNode->node;
      if (NULLP != delObj)
      {
         *rptCfgId = delObj->val;
      }
      cmLListDelFrm (&measUeCb->measRptCfgIDFreeLLst, &delObj->lnk);
      cmLListAdd2Tail(&measUeCb->measRptCfgIDUsedLLst, &delObj->lnk);
   }
   else
   {
      RLOG0(L_ERROR, "Report Config Node is NULL");
      RETVALUE(WR_UMM_MEAS_INVALID_MEAS_ID);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to set Free measurement Object Id.
 *
 *  @details
 *  Function:wrUmmMeasSetFreeMeasObjId
 *    
 *  Processing steps:
 *      - Find measurement Object Id node from measurement Object Id
 *      - delete from used list and add to free list
 *      
 *   @param[out] measUeCb: measurement Ue control block.
 *   @param[in] measObjId: measurement Object Id.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/

PRIVATE S16 wrUmmMeasSetFreeMeasObjId
(
WrMeasUeCb                   *measUeCb,
U8                           measObjId
)
{
   WrMeasFreeNode            *delObj = NULLP;
   CmLList                   *delNode = NULLP;

   TRC2(wrUmmMeasSetFreeMeasObjId)
   delNode = measUeCb->measObjIDUsedLLst.first;
   while(NULLP != delNode)
   {
      delObj = (WrMeasFreeNode*)delNode->node;
      if (measObjId == delObj->val)
      {
         cmLListDelFrm(&measUeCb->measObjIDUsedLLst, &delObj->lnk);
         cmLListAdd2Tail(&measUeCb->measObjIDFreeLLst, &delObj->lnk);
         break;
      }
      delNode = delNode->next;
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for to get Free measurement Object Id.
 *
 *  @details
 *  Function:wrUmmMeasGetFreeMeasObjId
 *    
 *  Processing steps:
 *      - Find measurement Object Id node from measurement Object Id
 *      - delete from Free list and add to Used list
 *      
 *   @param[in] measIdTyp  : measurement ID type - CSFB/HO/ANR
 *   @param[in] measUeCb   : measurement Ue control block.
 *   @param[out] measObjId : pointer to measurement Object Id.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/

PUBLIC S16 wrUmmMeasGetFreeMeasObjId
(
WrMeasUeCb           *measUeCb,
U8                   *measObjId
)
{
   WrMeasFreeNode            *delObj;
   CmLList                   *delNode;

   TRC2(wrUmmMeasGetFreeMeasObjId)
   delNode = measUeCb->measObjIDFreeLLst.first;
   if(NULLP != delNode)
   {
      delObj = (WrMeasFreeNode*)delNode->node;
      if (NULLP != delObj)
      {
         *measObjId = delObj->val;
      }
      cmLListDelFrm (&measUeCb->measObjIDFreeLLst, &delObj->lnk);
      cmLListAdd2Tail(&measUeCb->measObjIDUsedLLst, &delObj->lnk);
   }
   else
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, measUeCb->crnti,
            "Measurement object Id Node is NULL");
      RETVALUE(WR_UMM_MEAS_INVALID_MEAS_ID);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to set Free measurement Id.
 *
 *  @details
 *  Function:wrUmmMeasSetFreeMeasId
 *    
 *  Processing steps:
 *      - Find measurement Id node from measurement Id
 *      - delete from Free list and add to Used list
 *      
 *   @param[out] measUeCb: measurement Ue control block.
 *   @param[in] measId: measurement Id.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/

PUBLIC S16 wrUmmMeasSetFreeMeasId
(
WrMeasUeCb                   *measUeCb,
U8                           measId
)
{
   WrMeasFreeNode            *delObj;
   CmLList                   *delNode;

   TRC2(wrUmmMeasSetFreeMeasId)
   delNode = measUeCb->measIDUsedLLst.first;
   while(NULLP != delNode)
   {
      delObj = (WrMeasFreeNode*)delNode->node;
      if (measId == delObj->val)
      {
         cmLListDelFrm(&measUeCb->measIDUsedLLst, &delObj->lnk);
         cmLListAdd2Tail (&measUeCb->measIDFreeLLst, &delObj->lnk);
         break;
      }
      delNode = delNode->next;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to get Free measurement Id.
 *
 *  @details
 *  Function:wrUmmMeasGetFreeMeasId
 *    
 *  Processing steps:
 *      - Find measurement Id node from measurement Id
 *      - delete from Used list and add to Free list
 *      
 *   @param[in] measUeCb: measurement Ue control block.
 *   @param[out] measId: pointer to measurement Id.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasGetFreeMeasId
(
WrMeasUeCb            *measUeCb,
U8                    *measId
)
{
   WrMeasFreeNode            *freeObj = NULLP;
   CmLList                   *freeNode = NULLP;

   TRC2(wrUmmMeasGetFreeMeasId)

   freeNode = measUeCb->measIDFreeLLst.first;
   if(NULLP != freeNode)
   {
      freeObj = (WrMeasFreeNode*)freeNode->node;
      if(freeObj != NULLP)
      {
         *measId = freeObj->val;
      }
      cmLListDelFrm (&measUeCb->measIDFreeLLst, &freeObj->lnk);
      cmLListAdd2Tail(&measUeCb->measIDUsedLLst, &freeObj->lnk);
   }
   else
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, measUeCb->crnti,
            "Measurement ID not found");
      RETVALUE(WR_UMM_MEAS_INVALID_MEAS_ID);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for Deciding measurement gap configuration
 * requirement and provides measurment gap pattern and offset to the caller module
 * in the output structure.
 *
 *  @details
 *  Function:wrUmmMeasAddMeasGap
 *    
 *  Processing steps:
 *      - Verify if measurement gap already configured in UE and scheduler 
 *      - If not configured fill GAP flags in the output structure and 
 *        also Calcualte the measurement gap offset
 *      
 *   @param[in] reqModule: Requesting module.
 *   @param[in] cellCb: Pointre to Cell control block.
 *   @param[in] ueCb: Ue control block.
 *   @param[in] measGapStatus : measurement gap status pointer
 *   @param[in] reqModuleTransId : Transaction Id of the requested module
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/

PUBLIC Void wrUmmMeasAddMeasGap
(
 U32                          reqModule,
 WrUmmCellCb                  *cellCb,
 WrUeCb                       *ueCb,
 WrUmmMeasGapStatus           *measGapStatus,
 U32                          reqModuleTransId
)
{
   LwrMeasGapConfig          *measGapCfg;
   WrMeasUeCb                *measUeCb;
   U8                        gapPattern;  

   TRC2(wrUmmMeasAddMeasGap)

   measGapCfg = &(cellCb->measCellCfg->measGap);
   measUeCb = ueCb->measCfg;
   *measGapStatus = WR_UMM_MEAS_GAP_NONE;

   switch(reqModule)
   {
      case WR_UMM_MEAS_GAP_INTER_FREQ_HO:
         gapPattern = measGapCfg->gapPatternForInterFA; 
         break;

      case WR_UMM_MEAS_GAP_INTER_FREQ_ANR: 
         gapPattern = measGapCfg->gapPatternForANR; 
         break;

      case WR_UMM_MEAS_GAP_INTER_RAT_HO: 
         gapPattern = measGapCfg->gapPatternForInterRAT;
         break;

      case WR_UMM_MEAS_GAP_CSFB: 
         /* TODO : Presently using InterRAT configuration for CSFB
            Need to use CSFB specific configurations for the 
            same  */
         gapPattern = measGapCfg->gapPatternForInterRAT;
         break;

      case WR_UMM_MEAS_GAP_REEST:  
         {
            /* Re-establishment module shall call this function to check the
               measurement gap needs to be configured after the successful
               Re-establishment procedure. During Re-establishment procedure, 
               the features of the TeNB shall either release the measurment 
               gap(and close the transaction) or doesn't release the 
               gap(to continue after the re-establishment procedure. 
               This is used for the features that fall in the later scenario.*/
            /* Note: For the Reestablishment module the 
                     measGapModuleBitMask should not be updated */
            if ( measUeCb->gapMeasInfo.measGapModuleBitMask != 0)
            {
               /* Some of the TeNB feature is configured with the measurement gap.
                 So initiate configuring the Measurement gap at the scheduler and UE. 
                As the Gap configuration parameters are already in the UeCb,
                so just update the latest Tx Id */
               measUeCb->gapMeasInfo.latestTxId  =  reqModuleTransId;
               *measGapStatus = WR_UMM_MEAS_GAP_INIT_SCH_CONFIG;
               
            }
            else 
            {
               *measGapStatus = WR_UMM_MEAS_GAP_NONE;
            }
         } /* Break is intentional not add to fall through to default case for ROK.*/ 
      default: 
         RETVOID;
         break;
   }

   if (measUeCb->gapMeasInfo.isMeasGapCfg != WR_UMM_MEAS_GAP_NONE)
   {
      /* MeasGap state is already configured. Depending on the Gap status 
        or the gap pattern initiate configuring the meas gap at sch and UE
       */ 
      if((gapPattern < measUeCb->gapMeasInfo.gapPattern) || 
         (measUeCb->gapMeasInfo.isMeasGapCfg == WR_UMM_MEAS_GAP_REL))
      {
         /* Requested GAP patten need to be configured to SCH & UE, 
          * as configured GAP takes less priority over the requested  */
         WR_FILL_MEASGAP_FOR_CONFIG(measUeCb, gapPattern, reqModuleTransId, ueCb, (*measGapStatus)); 
      }
      /* Requested pattern is same as configured. 
       * Requested pattern has less priority over configured.
       * e.g. Configured is 40ms and requested is 80ms. 
       * Hence, for above two cases set bit mask and return */
      WR_SET_MEAS_GAP_BIT_MASK(measUeCb, gapPattern, reqModule); 
   } 
   else /* If meas gap is not configured (both UE and scheduler ) */
   {
      /* Requested GAP patten need to be configured to SCH & UE */
      WR_SET_MEAS_GAP_BIT_MASK(measUeCb, gapPattern, reqModule); 
      WR_FILL_MEASGAP_FOR_CONFIG(measUeCb, gapPattern, reqModuleTransId, ueCb, (*measGapStatus)); 
      measUeCb->gapMeasInfo.isMeasGapCfg = *measGapStatus;
   }
   RETVOID;
}

/** @brief This function is responsible for releasing the measurement gap 
 * config into Ue cb
 *
 *  @details
 *  Function:wrUmmMeasRelMeasGap
 *    
 *  Processing steps: 
 *         -Check if any other module enabled the measurement gap
 *         -Set GapRel as TRUE only if no other module configured 
 *          measurement gap
 *      
 *   @param[in] reqModule: The module requesting for gap release.
 *   @param[in] cellCb: Pointre to Cell control block.
 *   @param[in] ueCb: Ue control block.
 *   @param[in] reqModuleTransId: Requested module TranId
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/

PUBLIC Void  wrUmmMeasRelMeasGap
(
 U32                          reqModule,
 WrUmmCellCb                  *cellCb,
 WrUeCb                       *ueCb,
 WrUmmMeasGapStatus           *measGapStatus,
 U32                          reqModuleTransId
)
{
   WrMeasUeCb                *measUeCb = NULLP;
   U8                        gapPattern = 0;

   TRC2(wrUmmMeasRelMeasGap)

   measUeCb = ueCb->measCfg;

   if ((measUeCb->gapMeasInfo.isMeasGapCfg != WR_UMM_MEAS_GAP_NONE) &&
       (measUeCb->gapMeasInfo.isMeasGapCfg != WR_UMM_MEAS_GAP_REL))
   {
      /* Reset the bitmask for requested feature */
      WR_RESET_MEAS_GAP_BIT_MASK (measUeCb, gapPattern, reqModule);

      if (measUeCb->gapMeasInfo.measGapModuleBitMask == 0) 
      { 
         /* No other module is using MeasGap, hence release it */
         /* release the MeasGAP to UE & SCH, as no other features are using MeasGap*/
         *measGapStatus = WR_UMM_MEAS_GAP_REL;
         measUeCb->gapMeasInfo.isMeasGapCfg = *measGapStatus;
         measUeCb->gapMeasInfo.latestTxId    = reqModuleTransId;

      }
      else if((measUeCb->gapMeasInfo.gapPattern == WR_UMM_MEAS_GAP_PERD_40MS) && 
            ((measUeCb->gapMeasInfo.measGapModuleBitMask ^ 
              measUeCb->gapMeasInfo.measGap80msBitMask) == 0))
      {
         /* Requested release MeasGAP is configured to SCH & UE,check any other
          * features are using same pattern. If not configure SCH and UE  with 80ms */
         WR_FILL_MEASGAP_FOR_CONFIG(measUeCb, WR_UMM_MEAS_GAP_PERD_80MS, 
                                      reqModuleTransId, ueCb, (*measGapStatus)); 
      }
   }
   RETVOID;
}


/** @brief This function is responsible for Alloacte Meas Id Linked list node in
 * Ue Cb.
 * 
 *  @details
 * 
 *  Function:wrUmmMeasCreatMeasIdNode
 *   Processing steps:
 *     - Allocate MeasId node and add to Meas Id linked List.
 *   
 *    @param[in]  measObjId: Measurement Object Id.
 *    @param[in]  rptCfgId: Report Config Id.
 *    @param[in]  measTransCb: pointer to Measurement Transaction Cb.
 *    @param[in]  measUeCb: pointer to Measurement Ue Cb.
 *    @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 *        
 */
PUBLIC U8 wrUmmMeasCreatMeasIdNode
(
 WrUmmMeasTyp                measIdTyp,
 U8                          measObjId,
 U8                          rptCfgId,
 WrMeasUeCb                  *measUeCb
)
{
   U8                        measId = 0;
   S16                       ret;
   WrMeasIdNode              *measIdNode;

   TRC2(wrUmmMeasCreatMeasIdNode)

   ret = wrUmmMeasGetFreeMeasId(measUeCb, &measId);
   if(WR_UMM_MEAS_INVALID_MEAS_ID == ret)   
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, measUeCb->crnti,
            "Measurement Id is not Available");
      RETVALUE(RFAILED);
   }
   WR_ALLOC (&measIdNode, sizeof(WrMeasIdNode));
   if(NULLP == measIdNode)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      wrUmmMeasSetFreeMeasId(measUeCb, measId);
      RETVALUE(RFAILED);
   }
   measIdNode->measIdTyp = measIdTyp;
   measIdNode->measID = measId;
   measIdNode->measObjID = measObjId;
   measIdNode->rptCfgID = rptCfgId;
   measIdNode->lnk.node = (PTR)measIdNode;
   cmLListAdd2Tail(&measUeCb->measIdLst, &measIdNode->lnk);      

   RETVALUE(measId);
}

/**
*   @brief This function gives the report amount value from Enum
* 
*       Function: wrUmmMeasGetRptAmtFrmEnum
* 
*           Processing steps:
*           - return the value for Report Amount
* 
*   @param[in]   rptAmt  : Report Amount
*   @return S16  ROK
* */
PUBLIC U16 wrUmmMeasGetRptAmtFrmEnum
(
 U8        rptAmt
)
{
   U16    reportAmt;

   switch(rptAmt)
   {
      case NhuReportConfigEUTRAreportAmountr1Enum:
         reportAmt = 1;
         break;
      case NhuReportConfigEUTRAreportAmountr2Enum:
         reportAmt = 2;
         break;
      case NhuReportConfigEUTRAreportAmountr4Enum:
         reportAmt = 4;
         break;
      case NhuReportConfigEUTRAreportAmountr8Enum:
         reportAmt = 8;
         break;
      case NhuReportConfigEUTRAreportAmountr16Enum:
         reportAmt = 16;
         break;
      case NhuReportConfigEUTRAreportAmountr32Enum:
         reportAmt = 32;
         break;
      case NhuReportConfigEUTRAreportAmountr64Enum:
         reportAmt = 64;
         break;
      case NhuReportConfigEUTRAreportAmountinfinityEnum:
         reportAmt = 0xFF;
         break;
      default:
         {
            RLOG1(L_ERROR, "Report Amount[%d] is wrong", rptAmt);
            reportAmt = 0;
         }
   }
   RETVALUE(reportAmt);
}

/** @brief This function is responsible for to get report config Id from
 * report config List.
 *
 *  @details
 *  Function:wrUmmMeasFindRptCfgID
 *    
 *  Processing steps:
 *      - Traverse List and find report config Id.
 *        
 *   @param[out] rptCfgId: pointer to report config Id.
 *   @param[in] rptCfgInfo: Report config information.
 *   @param[in] rptCfgLst: Report config List.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PUBLIC S16 wrUmmMeasFindRptCfgID
(
U8                           *rptCfgId,
WrUmmMeasRptCfgInfo          *rptCfgInfo,
CmLListCp                    *rptCfgLst
)
{
   WrMeasRptCfgNode          *rptCfgObj;
   CmLList                   *rptCfgNode;
   Bool                      isRptCfgPres = FALSE;
   
   TRC2(wrUmmMeasFindRptCfgID)
   rptCfgNode = rptCfgLst->first;

   while (NULLP != rptCfgNode)
   {
      rptCfgObj = (WrMeasRptCfgNode*)rptCfgNode->node;
      
      if ((rptCfgObj->rptCfgType == rptCfgInfo->rptCfgTyp) &&
          (rptCfgObj->intRptCfgTyp == rptCfgInfo->intRptCfgType))
      {
         /*Check for threshold Value */
         isRptCfgPres = TRUE;
         *rptCfgId = rptCfgObj->rptCfgId;
         break;
      }
      rptCfgNode = rptCfgNode->next;
   }
   if(isRptCfgPres == FALSE) 
   {
      RLOG1(L_INFO, "rptcfgId of the rptCfgType:%d is not in in-use list",rptCfgInfo->rptCfgTyp);
      RETVALUE(RFAILED);
   }
   else
   {
      RETVALUE(ROK);
   }
}
/** @brief This function is responsible for to fill Utra Report CGI report
 *  config Object.
 *
 *  @details
 *  Function:wrUmmMeasFillUtraRptCgiObj
 *    
 *  Processing steps:
 *      - Fill the report config.
 *      
 *   @param[out] rptCfgUtra: pointer to report config for Utra.
 *   @param[out] perdTyp: pointer to periodic Type structure.
 *   @param[in] strngCellAnrRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasFillUtraRptCgiObj
(
NhuReportConfigInterRAT                     *rptCfgUtra,
NhuReportConfigInterRATtriggerTypperiodical *perdTyp, 
LwrMeasUtraPeriodRptCfg                     *strngCellAnrRptCfg
)
{
   TRC2(wrUmmMeasFillUtraRptCgiObj)
   wrFillTknU8(&rptCfgUtra->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(rptCfgUtra->triggerTyp.choice, TRIGGERTYP_PERIODICAL)
   wrFillTknU8(&perdTyp->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(perdTyp->purpose,
      NhuReportConfigInterRATtriggerTypperiodicalpurposereportCGIEnum)
   WR_FILL_TKN_UINT(rptCfgUtra->maxReportCells, WR_UMM_MAX_REPORT_CELLS_VAL)
   WR_FILL_TKN_UINT(rptCfgUtra->reportInterval, WR_UMM_REPORT_INTERVEL_VAL)
   WR_FILL_TKN_UINT(rptCfgUtra->reportAmount, WR_UMM_REPORT_AMOUNT_VAL)

   RETVALUE(ROK);
}

/** @brief This function is responsible for to fill interRat Report CGI report
 *  config Object.
 *
 *  @details
 *  Function:wrUmmMeasFillInterRatRptCgiObj
 *    
 *  Processing steps:
 *      - Fill the report config.
 *      
 *   @param[out] rptCfgCdm2k: pointer to report config for cdma2000.
 *   @param[out] perdTyp: pointer to periodic Type structure.
 *   @param[in] strngCellAnrRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasFillInterRatRptCgiObj
(
NhuReportConfigInterRAT                     *rptCfgCdm2k,
NhuReportConfigInterRATtriggerTypperiodical *perdTyp, 
LwrMeasCDMAPeridRptCfg                      *strngCellAnrRptCfg
)
{
   TRC2(wrUmmMeasFillInterRatRptCgiObj)
   wrFillTknU8(&rptCfgCdm2k->pres, 1);
   WR_FILL_TKN_UINT(rptCfgCdm2k->triggerTyp.choice, TRIGGERTYP_PERIODICAL)
   wrFillTknU8(&perdTyp->pres, 1);
   WR_FILL_TKN_UINT(perdTyp->purpose,
      NhuReportConfigInterRATtriggerTypperiodicalpurposereportCGIEnum)
   WR_FILL_TKN_UINT(rptCfgCdm2k->maxReportCells, 1)
   WR_FILL_TKN_UINT(rptCfgCdm2k->reportInterval, 0)
   WR_FILL_TKN_UINT(rptCfgCdm2k->reportAmount, 0)

   RETVALUE(ROK);
}

/** @brief This function is responsible for to fill Eutra Report CGI report
 *  config Object.
 *
 *  @details
 *  Function:wrUmmMeasFillEutraRptCgiObj
 *    
 *  Processing steps:
 *      - Fill the report config for EUTRA report cgi object
 *      
 *   @param[out] rptCfgEutra: pointer to report config for Eutra.
 *   @param[out] perdTyp: pointer to periodic Type structure.
 *   @param[in] perdStrngCellRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasFillEutraRptCgiObj
(
NhuReportConfigEUTRA                     *rptCfgEutra,
NhuReportConfigEUTRAtriggerTypperiodical *perdTyp, 
LwrMeasEutraPeridRptCfg                  *perdStrngCellRptCfg
)
{
   TRC2(wrUmmMeasFillEutraRptCgiObj)
   wrFillTknU8(&rptCfgEutra->pres, 1);
   WR_FILL_TKN_UINT(rptCfgEutra->triggerTyp.choice, TRIGGERTYP_PERIODICAL)
   wrFillTknU8(&perdTyp->pres, 1);
   WR_FILL_TKN_UINT(perdTyp->purpose,
      NhuReportConfigEUTRAtriggerTypperiodicalpurposereportCGIEnum)
   WR_FILL_TKN_UINT(rptCfgEutra->triggerQuantity, 0)
   WR_FILL_TKN_UINT(rptCfgEutra->reportQuantity, 1)
   WR_FILL_TKN_UINT(rptCfgEutra->maxReportCells, 1)
   WR_FILL_TKN_UINT(rptCfgEutra->reportInterval, 5)
   WR_FILL_TKN_UINT(rptCfgEutra->reportAmount, 0)
   RETVALUE(ROK);
}

/** @brief This function is responsible for to fill Utra strongest cell ANR 
 * report config Object.
 *
 *  @details
 *  Function:wrUmmMeasFillUtraStrngCellAnrRptObj
 *    
 *  Processing steps:
 *      - Fill the report config for Utra strongest cell ANR object
 *      
 *   @param[out] rptCfgUtra: pointer to report config Object.
 *   @param[out] perdTyp: pointer to periodic Type structure.
 *   @param[in] strngCellAnrRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasFillUtraStrngCellAnrRptObj
(
NhuReportConfigInterRAT                     *rptCfgUtra,
NhuReportConfigInterRATtriggerTypperiodical *perdTyp, 
LwrMeasUtraPeriodRptCfg                     *strngCellAnrRptCfg
)
{
   TRC2(wrUmmMeasFillUtraStrngCellAnrRptObj)
   wrFillTknU8(&rptCfgUtra->pres, 1);
   WR_FILL_TKN_UINT(rptCfgUtra->triggerTyp.choice, TRIGGERTYP_PERIODICAL)
   wrFillTknU8(&perdTyp->pres, 1);
   WR_FILL_TKN_UINT(perdTyp->purpose,
     NhuReportConfigInterRATtriggerTypperiodicalpurposereportStrongestCellsForSONEnum)
   WR_FILL_TKN_UINT(rptCfgUtra->maxReportCells, strngCellAnrRptCfg->maxReportCell)
   WR_FILL_TKN_UINT(rptCfgUtra->reportInterval, strngCellAnrRptCfg->reportInterval)
   WR_FILL_TKN_UINT(rptCfgUtra->reportAmount, strngCellAnrRptCfg->reportAmount)

   RETVALUE(ROK);
}

/** @brief This function is responsible for to fill Cdma strongest cell ANR 
 * report config Object.
 *
 *  @details
 *  Function:wrUmmMeasFillStrngCellAnrRptObj
 *    
 *  Processing steps:
 *      - Fill the report config for Cdma strongest cell ANR object
 *      
 *   @param[out] rptCfgCdm2k: pointer to report config Object.
 *   @param[out] perdTyp: pointer to periodic Type structure.
 *   @param[in] strngCellAnrRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasFillStrngCellAnrRptObj
(
NhuReportConfigInterRAT                     *rptCfgCdm2k,
NhuReportConfigInterRATtriggerTypperiodical *perdTyp, 
LwrMeasCDMAPeridRptCfg                      *strngCellAnrRptCfg
)
{
   TRC2(wrUmmMeasFillStrngCellAnrRptObj)
   wrFillTknU8(&rptCfgCdm2k->pres, 1);
   WR_FILL_TKN_UINT(rptCfgCdm2k->triggerTyp.choice, TRIGGERTYP_PERIODICAL)
   wrFillTknU8(&perdTyp->pres, 1);
   WR_FILL_TKN_UINT(perdTyp->purpose,
     NhuReportConfigInterRATtriggerTypperiodicalpurposereportStrongestCellsForSONEnum)
   WR_FILL_TKN_UINT(rptCfgCdm2k->maxReportCells, strngCellAnrRptCfg->maxRptCells)
   WR_FILL_TKN_UINT(rptCfgCdm2k->reportInterval, strngCellAnrRptCfg->reportInterval)
   WR_FILL_TKN_UINT(rptCfgCdm2k->reportAmount, strngCellAnrRptCfg->reportAmount)

   RETVALUE(ROK);
}

/** @brief This function is responsible for filling GERAN strongest cell 
 *         report config Object.
 *
 *  @details
 *              Function: wrUmmMeasFillGeranRptCsfb
 *    
 *              Processing steps:
 *                  - Fill the report config for GERAN strongest cell report
 *                  config
 *      
 *   @param[out] rptCfgGeran         : report config GERAN 
 *   @param[out]  perdTyp            : periodic type IE
 *   @param[in]  perdStrngCellRptCfg : Report strong cell config 
 *   
 *   @return Void
*/
PRIVATE Void wrUmmMeasFillGeranRptCsfb
(
NhuReportConfigInterRAT                         *rptCfgGeran,
NhuReportConfigInterRATtriggerTypperiodical     *perdTyp,
LwrMeasGERANPeridRptCfg                         *perdStrngCellRptCfg
)
{
   wrFillTknU8(&rptCfgGeran->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(rptCfgGeran->triggerTyp.choice, TRIGGERTYP_PERIODICAL)
      wrFillTknU8(&perdTyp->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(perdTyp->purpose,
         NhuReportConfigInterRATtriggerTypperiodicalpurposereportStrongestCellsEnum)
   WR_FILL_TKN_UINT(rptCfgGeran->maxReportCells, perdStrngCellRptCfg->maxRptCells);
   WR_FILL_TKN_UINT(rptCfgGeran->reportInterval, perdStrngCellRptCfg->reportInterval);
   WR_FILL_TKN_UINT(rptCfgGeran->reportAmount, perdStrngCellRptCfg->reportAmount);

   RETVOID;
}



/** @brief This function is responsible for to fill Utra Report
 *  config Object.
 *
 *  @details
 *  Function:wrUmmMeasFillUtraRptCsfb
 *
 *  Processing steps:
 *      - Fill the report config for UTRA report cgi object
 *
 *   @param[out] rptCfgUtra: pointer to report config for Utra.
 *   @param[out] perdTyp: pointer to periodic Type structure.
 *   @param[in] perdStrngCellRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE : RFAILED
*/
PRIVATE S16 wrUmmMeasFillUtraRptCsfb
(
NhuReportConfigInterRAT                         *rptCfgUtra,
NhuReportConfigInterRATtriggerTypperiodical     *perdTyp,
LwrMeasUtraPeriodRptCfg                         *perdStrngCellRptCfg,
WrUeCb                                          *ueCb
)
{
   TRC2(wrUmmMeasFillUtraRptCsfb)
   wrFillTknU8(&rptCfgUtra->pres, 1);
   WR_FILL_TKN_UINT(rptCfgUtra->triggerTyp.choice, TRIGGERTYP_PERIODICAL)
   wrFillTknU8(&perdTyp->pres, 1);
   WR_FILL_TKN_UINT(perdTyp->purpose,
         NhuReportConfigInterRATtriggerTypperiodicalpurposereportStrongestCellsEnum)
   WR_FILL_TKN_UINT(rptCfgUtra->maxReportCells, perdStrngCellRptCfg->maxReportCell);
   WR_FILL_TKN_UINT(rptCfgUtra->reportInterval, perdStrngCellRptCfg->reportInterval);
   WR_FILL_TKN_UINT(rptCfgUtra->reportAmount, perdStrngCellRptCfg->reportAmount);
   /*rel10*/
   /* Fill reportQuantityUTRA-FDD-r10 if FGI 114 is set*/
   if(wrUmmUtilFeatureGrpIndV1020(ueCb, WR_UMM_UTRA_MEAS_RPRT_ECN0_SUPPORTED))
   {
      wrFillTknU8(&rptCfgUtra->extaddgrp_2.pres,PRSNT_NODEF);
      wrFillTknU32(&rptCfgUtra->extaddgrp_2.reportQuantityUTRA_FDD_r10,
            NhuReportConfigInterRATextaddgrp_2reportQuantityUTRA_FDD_r10bothEnum);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for to fill Cdma Report
 *  config Object.
 *
 *  @details
 *  Function:wrUmmMeasFillCdmaRptCsfb
 *
 *  Processing steps:
 *      - Fill the report config for CDMA object
 *
 *   @param[out] rptCfgCdma: pointer to report config for Cdma.
 *   @param[out] perdTyp: pointer to periodic Type structure.
 *   @param[in] perdStrngCellRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE : RFAILED
*/
PRIVATE S16 wrUmmMeasFillCdmaRptCsfb
(
NhuReportConfigInterRAT                         *rptCfgCdma,
NhuReportConfigInterRATtriggerTypperiodical     *perdTyp,
LwrMeasCDMAPeridRptCfg                          *perdStrngCellRptCfg
)
{
   TRC2(wrUmmMeasFillCdmaRptCsfb)
   wrFillTknU8(&rptCfgCdma->pres, 1);
   WR_FILL_TKN_UINT(rptCfgCdma->triggerTyp.choice, TRIGGERTYP_PERIODICAL)
   wrFillTknU8(&perdTyp->pres, 1);
   WR_FILL_TKN_UINT(perdTyp->purpose,
         NhuReportConfigInterRATtriggerTypperiodicalpurposereportStrongestCellsEnum)
   WR_FILL_TKN_UINT(rptCfgCdma->maxReportCells, perdStrngCellRptCfg->maxRptCells);
   WR_FILL_TKN_UINT(rptCfgCdma->reportInterval, perdStrngCellRptCfg->reportInterval);
   WR_FILL_TKN_UINT(rptCfgCdma->reportAmount, perdStrngCellRptCfg->reportAmount); 

   RETVALUE(ROK);
}
/** @brief This function is responsible for to fill Eutra strongest cell
 * report config Object.
 *
 *  @details
 *  Function:wrUmmMeasFillEutraStrngCellRptObj
 *    
 *  Processing steps:
 *      - Fill the report config for Eutra strongest cell object
 *      
 *   @param[out] rptCfgEutra: pointer to report config Object.
 *   @param[out] perdTyp: pointer to periodic Type structure.
 *   @param[in] perdStrngCellRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasFillEutraStrngCellRptObj
(
NhuReportConfigEUTRA                     *rptCfgEutra,
NhuReportConfigEUTRAtriggerTypperiodical *perdTyp, 
LwrMeasEutraPeridRptCfg                  *perdStrngCellRptCfg,
Bool                                     isMeasCfgForPerdStrongCellRptCfg
)
{
   TRC2(wrUmmMeasFillEutraStrngCellRptObj)
   wrFillTknU8(&rptCfgEutra->pres, 1);
   WR_FILL_TKN_UINT(rptCfgEutra->triggerTyp.choice, TRIGGERTYP_PERIODICAL)
   wrFillTknU8(&perdTyp->pres, 1);
   WR_FILL_TKN_UINT(perdTyp->purpose,
      NhuReportConfigEUTRAtriggerTypperiodicalpurposereportStrongestCellsEnum)
   WR_FILL_TKN_UINT(rptCfgEutra->triggerQuantity, perdStrngCellRptCfg->triggerQty)
   WR_FILL_TKN_UINT(rptCfgEutra->reportQuantity, perdStrngCellRptCfg->reportQty)
   WR_FILL_TKN_UINT(rptCfgEutra->maxReportCells, perdStrngCellRptCfg->maxRptCells)
   if(isMeasCfgForPerdStrongCellRptCfg)
   {
      WR_FILL_TKN_UINT(rptCfgEutra->reportInterval, perdStrngCellRptCfg->sonCfgRptInterval)
      WR_FILL_TKN_UINT(rptCfgEutra->reportAmount, 7)/* Enum 7 means - Infinity times */
   }
   else
   {
      WR_FILL_TKN_UINT(rptCfgEutra->reportInterval, perdStrngCellRptCfg->reportInterval)
      WR_FILL_TKN_UINT(rptCfgEutra->reportAmount, perdStrngCellRptCfg->reportAmount)
   }

   RETVALUE(ROK);
}
/** @brief This function is responsible for to fill Event B2 report config
 * Object.
 *
 *  @details
 *  Function:wrUmmMeasFillEvntUtraB2RptObj
 *      
 *  Processing steps:
 *      - Fill the report config for event B2 report config object
 *      
 *   @param[out] rptCfgUtra: pointer to report config Object.
 *   @param[in] thrshldTyp: threshold type.
 *   @param[out] evntTyp: pointer to Event Type structure.
 *   @param[in] b2EvntRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasFillEvntUtraB2RptObj
(
NhuReportConfigInterRAT                *rptCfgUtra,
WrUmmMeasRptCfgInfo                    *rptCfgInfo,
NhuReportConfigInterRATtriggerTypevent *evntTyp, 
LwrMeasUtraEvntB2RptCfg                *b2EvntRptCfg,
WrUeCb                                 *ueCb /* rel10 */
)
{
   TRC2(wrUmmMeasFillEvntUtraB2RptObj)
      wrFillTknU8(&rptCfgUtra->pres, 1);
   WR_FILL_TKN_UINT(rptCfgUtra->triggerTyp.choice, TRIGGERTYP_EVENT)
      wrFillTknU8(&evntTyp->pres, 1);
   WR_FILL_TKN_UINT(evntTyp->eventId.choice, EVENTID_EVENTB2)
      wrFillTknU8(&evntTyp->eventId.val.eventB2.pres, 1);
   if( WR_UMM_MEAS_THRESHOLD_RSCP == rptCfgInfo->thrshldTyp)
   {
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold1.choice,
            THRESHOLDEUTRA_THRESHOLD_RSRP)
         WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold1.val.
               threshold_RSRP,
               b2EvntRptCfg->b2Threshold1RSRP)
         WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold2.choice,
               B2_THRESHOLD2_B2_THRESHOLD2UTRA)
         WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold2.val.
               b2_Threshold2UTRA.choice,
               THRESHOLDUTRA_UTRA_RSCP)
         if(WR_UMM_MEAS_RPTCFG_IRATB2_REDIR == rptCfgInfo->intRptCfgType)
         {
            WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold2.val.
                  b2_Threshold2UTRA.val.utra_RSCP,
                  b2EvntRptCfg->b2Threshold2UtraTddRSCP)
         }
         else
         {
            WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold2.val.
                  b2_Threshold2UTRA.val.utra_RSCP,
                  b2EvntRptCfg->b2Threshold2UtraRSCP)
         }
   } 
   else
   {
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold1.choice,
            THRESHOLDEUTRA_THRESHOLD_RSRQ)
         WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold1.val.
               threshold_RSRQ,
               b2EvntRptCfg->b2Threshold1RSRQ)
         WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold2.choice,
               B2_THRESHOLD2_B2_THRESHOLD2UTRA)
         WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold2.val.
               b2_Threshold2UTRA.choice,THRESHOLDUTRA_UTRA_ECN0)
         WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold2.val.
               b2_Threshold2UTRA.val.utra_EcN0,
               b2EvntRptCfg->b2Threshold2UtraEcNo)
   }
   
   WR_FILL_TKN_UINT(evntTyp->hysteresis, b2EvntRptCfg->hysteresis)
      WR_FILL_TKN_UINT(evntTyp->timeToTrigger, b2EvntRptCfg->timeToTrigger)
      WR_FILL_TKN_UINT(rptCfgUtra->maxReportCells, b2EvntRptCfg->maxReportCell)
      WR_FILL_TKN_UINT(rptCfgUtra->reportInterval, b2EvntRptCfg->reportInterval)
      WR_FILL_TKN_UINT(rptCfgUtra->reportAmount, b2EvntRptCfg->reportAmount)
   /*rel10*/
   /* Fill reportQuantityUTRA-FDD-r10 if FGI 114 is set*/
   if(wrUmmUtilFeatureGrpIndV1020(ueCb, WR_UMM_UTRA_MEAS_RPRT_ECN0_SUPPORTED))
   {
      wrFillTknU8(&rptCfgUtra->extaddgrp_2.pres,PRSNT_NODEF);
      wrFillTknU32(&rptCfgUtra->extaddgrp_2.reportQuantityUTRA_FDD_r10,
            NhuReportConfigInterRATextaddgrp_2reportQuantityUTRA_FDD_r10bothEnum);
   }

      RETVALUE(ROK);
}
 /** @brief This function is responsible for to fill Event B2 report config
 * Object.
 *
 *  @details
 *  Function:wrUmmMeasFillEvntGeranB2RptObj
 *      
 *  Processing steps:
 *      - Fill the report config for event B2 report config object
 *      
 *   @param[out] rptCfgGeran: pointer to report config Object.
 *   @param[in] thrshldTyp: threshold type.
 *   @param[out] evntTyp: pointer to Event Type structure.
 *   @param[in] b2EvntRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasFillEvntGeranB2RptObj
(
NhuReportConfigInterRAT                *rptCfgGeran,
WrUmmMeasRptCfgInfo                    *rptCfgInfo,
NhuReportConfigInterRATtriggerTypevent *evntTyp, 
LwrMeasGeranEvntB2RptCfg               *b2EvntRptCfg,
WrUeCb                                 *ueCb /* rel10 */
)
{
   TRC2(wrUmmMeasFillEvntGeranB2RptObj)
      wrFillTknU8(&rptCfgGeran->pres, 1);
   WR_FILL_TKN_UINT(rptCfgGeran->triggerTyp.choice, TRIGGERTYP_EVENT)
      wrFillTknU8(&evntTyp->pres, 1);
   WR_FILL_TKN_UINT(evntTyp->eventId.choice, EVENTID_EVENTB2)
      wrFillTknU8(&evntTyp->eventId.val.eventB2.pres, 1);
   if( WR_UMM_MEAS_THRESHOLD_RSSI == rptCfgInfo->thrshldTyp)
   {
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold1.choice,
            THRESHOLDEUTRA_THRESHOLD_RSRP)
         WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold1.val.
               threshold_RSRP, b2EvntRptCfg->b2Threshold1RSRP)
         WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold2.choice,
               B2_THRESHOLD2_B2_THRESHOLD2GERAN)
         WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold2.val.
               b2_Threshold2GERAN, b2EvntRptCfg->b2Threshold2Geran)
   } 
   WR_FILL_TKN_UINT(evntTyp->hysteresis, b2EvntRptCfg->hysteresis)
      WR_FILL_TKN_UINT(evntTyp->timeToTrigger, b2EvntRptCfg->timeToTrigger)
      WR_FILL_TKN_UINT(rptCfgGeran->maxReportCells, b2EvntRptCfg->maxReportCell)
      WR_FILL_TKN_UINT(rptCfgGeran->reportInterval, b2EvntRptCfg->reportInterval)
      WR_FILL_TKN_UINT(rptCfgGeran->reportAmount, b2EvntRptCfg->reportAmount)

      RETVALUE(ROK);
}
/** @brief This function is responsible for to fill Event B2 report config
 * Object.
 *
 *  @details
 *  Function:wrUmmMeasFillEvntB2RptObj
 *      
 *  Processing steps:
 *      - Fill the report config for event B2 report config object
 *      
 *   @param[out] rptCfgCdma2k: pointer to report config Object.
 *   @param[in] thrshldTyp: threshold type.
 *   @param[out] evntTyp: pointer to Event Type structure.
 *   @param[in] b2EvntRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasFillEvntB2RptObj
(
NhuReportConfigInterRAT                *rptCfgCdma2k,
WrThrshldTyp                           thrshldTyp,
NhuReportConfigInterRATtriggerTypevent *evntTyp, 
LwrMeasCDMAEvntB2RptCfg                *b2EvntRptCfg
)
{
   TRC2(wrUmmMeasFillEvntB2RptObj)
   wrFillTknU8(&rptCfgCdma2k->pres, 1);
   WR_FILL_TKN_UINT(rptCfgCdma2k->triggerTyp.choice, TRIGGERTYP_EVENT)
   wrFillTknU8(&evntTyp->pres, 1);
   WR_FILL_TKN_UINT(evntTyp->eventId.choice, EVENTID_EVENTB2)
   wrFillTknU8(&evntTyp->eventId.val.eventB2.pres, 1);
   if( WR_UMM_MEAS_THRESHOLD_RSRP == thrshldTyp)
   {
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold1.choice,
                                           THRESHOLDEUTRA_THRESHOLD_RSRP)
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold1.val.threshold_RSRP,
                                            b2EvntRptCfg->threshold1Rsrp)
   } 
   else
   {
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold1.choice,
                                           THRESHOLDEUTRA_THRESHOLD_RSRQ)
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold1.val.threshold_RSRQ,
                                             b2EvntRptCfg->threshold1Rsrq)
   }
   WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold2.choice,
                                     B2_THRESHOLD2_B2_THRESHOLD2CDMA2000)
   WR_FILL_TKN_UINT(evntTyp->eventId.val.eventB2.b2_Threshold2.val.b2_Threshold2CDMA2000,
                                                b2EvntRptCfg->threshold2)
   
   WR_FILL_TKN_UINT(evntTyp->hysteresis, b2EvntRptCfg->hysteresis)
   WR_FILL_TKN_UINT(evntTyp->timeToTrigger, b2EvntRptCfg->timeToTrigger)
   WR_FILL_TKN_UINT(rptCfgCdma2k->maxReportCells, b2EvntRptCfg->maxRptCells)
   WR_FILL_TKN_UINT(rptCfgCdma2k->reportInterval, b2EvntRptCfg->reportInterval)
   WR_FILL_TKN_UINT(rptCfgCdma2k->reportAmount, b2EvntRptCfg->reportAmount)

   RETVALUE(ROK);
}
/** @brief This function is responsible for to fill Event A5 report config
 * Object.
 *
 *  @details
 *  Function:wrUmmMeasFillEvntA5RptObj
 *      
 *  Processing steps:
 *      - Fill the report config for event A5 report config object
 *      
 *   @param[out] rptCfgEutra: pointer to Eutra report config Object.
 *   @param[in] thrshldTyp: threshold type.
 *   @param[out] evntTyp: pointer to Event Type structure.
 *   @param[in] a5EvntRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasFillEvntA5RptObj
(
NhuReportConfigEUTRA                *rptCfgEutra,
WrThrshldTyp                        thrshldTyp,
NhuReportConfigEUTRAtriggerTypevent *evntTyp, 
LwrMeasEvntA5RptCfg                 *a5EvntRptCfg
)
{
   TRC2(wrUmmMeasFillEvntA5RptObj)
   wrFillTknU8(&rptCfgEutra->pres, 1);
   WR_FILL_TKN_UINT(rptCfgEutra->triggerTyp.choice, TRIGGERTYP_EVENT)
   wrFillTknU8(&evntTyp->pres, 1);
   WR_FILL_TKN_UINT(evntTyp->eventId.choice, EVENTID_EVENTA5)
   wrFillTknU8(&evntTyp->eventId.val.eventA5.pres, 1);
   if(thrshldTyp == WR_UMM_MEAS_THRESHOLD_RSRP)
   {
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA5.a5_Threshold1.choice,
                                        THRESHOLDEUTRA_THRESHOLD_RSRP)
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA5.a5_Threshold2.choice,
                                        THRESHOLDEUTRA_THRESHOLD_RSRP)
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA5.a5_Threshold1.val.threshold_RSRP, 
                                       a5EvntRptCfg-> threshold1_RSRP)
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA5.a5_Threshold2.val.threshold_RSRP,
                                       a5EvntRptCfg-> threshold2_RSRP)
   } 
   else 
   {
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA5.a5_Threshold2.choice,
                                       THRESHOLDEUTRA_THRESHOLD_RSRQ)
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA5.a5_Threshold1.choice,
                                       THRESHOLDEUTRA_THRESHOLD_RSRQ)
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA5.a5_Threshold1.val.threshold_RSRQ,
                                      a5EvntRptCfg-> threshold1_RSRQ)
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA5.a5_Threshold2.val.threshold_RSRQ,
                                      a5EvntRptCfg-> threshold2_RSRQ)
   }
   WR_FILL_TKN_UINT(evntTyp->hysteresis, a5EvntRptCfg->hysteresis)
   WR_FILL_TKN_UINT(evntTyp->timeToTrigger, a5EvntRptCfg->timeToTrigger)
   if(thrshldTyp == WR_UMM_MEAS_THRESHOLD_RSRP)
   {
      WR_FILL_TKN_UINT(rptCfgEutra->triggerQuantity, THRESHOLDEUTRA_THRESHOLD_RSRP)
   } 
   else
   {
      WR_FILL_TKN_UINT(rptCfgEutra->triggerQuantity, THRESHOLDEUTRA_THRESHOLD_RSRQ)
   }
   WR_FILL_TKN_UINT(rptCfgEutra->reportQuantity, a5EvntRptCfg->reportQty)
   WR_FILL_TKN_UINT(rptCfgEutra->maxReportCells, a5EvntRptCfg->maxRptCells)
   WR_FILL_TKN_UINT(rptCfgEutra->reportInterval, a5EvntRptCfg->reportInterval)
   WR_FILL_TKN_UINT(rptCfgEutra->reportAmount, a5EvntRptCfg->reportAmount)

   RETVALUE(ROK);
}
/** @brief This function is responsible for to fill Event A3 report config
 * Object.
 *
 *  @details
 *  Function:wrUmmMeasFillEvntA3RptObj
 *      
 *  Processing steps:
 *      - Fill the report config for event A3 report config object
 *      
 *   @param[out] rptCfgEutra: pointer to Eutra report config Object.
 *   @param[in] thrshldTyp: threshold type.
 *   @param[out] evntTyp: pointer to Event Type structure.
 *   @param[in] a3EvntRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasFillEvntA3RptObj
(
NhuReportConfigEUTRA                *rptCfgEutra,
WrThrshldTyp                        thrshldTyp,
NhuReportConfigEUTRAtriggerTypevent *evntTyp, 
LwrMeasEvntA3RptCfg                  *a3EvntRptCfg
)
{
   TRC2(wrUmmMeasFillEvntA3RptObj)
   wrFillTknU8(&rptCfgEutra->pres, 1);
   WR_FILL_TKN_UINT(rptCfgEutra->triggerTyp.choice, TRIGGERTYP_EVENT)
   wrFillTknU8(&evntTyp->pres, 1);;
   WR_FILL_TKN_UINT(evntTyp->eventId.choice, EVENTID_EVENTA3)
   wrFillTknU8(&evntTyp->eventId.val.eventA3.pres, 1);
   wrFillTknS32(&evntTyp->eventId.val.eventA3.a3_Offset, a3EvntRptCfg->offset);
   wrFillTknU8(&evntTyp->eventId.val.eventA3.reportOnLeave, a3EvntRptCfg->reportOnLeave);
   WR_FILL_TKN_UINT(evntTyp->hysteresis , a3EvntRptCfg->hysteresis)
   WR_FILL_TKN_UINT( evntTyp->timeToTrigger , a3EvntRptCfg->timeToTrigger)
   if(thrshldTyp == WR_UMM_MEAS_THRESHOLD_RSRP)
   {
      WR_FILL_TKN_UINT(rptCfgEutra->triggerQuantity ,THRESHOLDEUTRA_THRESHOLD_RSRP)
   } 
   else
   {
      WR_FILL_TKN_UINT(rptCfgEutra->triggerQuantity , THRESHOLDEUTRA_THRESHOLD_RSRQ)
   }
   WR_FILL_TKN_UINT(rptCfgEutra->reportQuantity , a3EvntRptCfg->reportQty)
   WR_FILL_TKN_UINT(rptCfgEutra->maxReportCells , a3EvntRptCfg->maxRptCells)
   WR_FILL_TKN_UINT(rptCfgEutra->reportInterval , a3EvntRptCfg->reportInterval)
   WR_FILL_TKN_UINT(rptCfgEutra->reportAmount , a3EvntRptCfg->reportAmount)

   RETVALUE(ROK);
}

/** @brief This function is responsible for to fill Event A2 report config
 * Object.
 *
 *  @details
 *  Function:wrUmmMeasFillEvntA2RptObj
 *      
 *  Processing steps:
 *      - Fill the report config for event A2 report config object
 *      
 *   @param[out] rptCfgEutra: pointer to Eutra report config Object.
 *   @param[in] thrshldTyp: threshold type.
 *   @param[out] evntTyp: pointer to Event Type structure.
 *   @param[in] a2EvntRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasFillEvntA2RptObj
(
NhuReportConfigEUTRA                *rptCfgEutra,
WrThrshldTyp                        thrshldTyp,
NhuReportConfigEUTRAtriggerTypevent *evntTyp, 
LwrMeasEvntRptCfg                    *a2EvntRptCfg
)
{
   TRC2(wrUmmMeasFillEvntA2RptObj)
   wrFillTknU8(&rptCfgEutra->pres, 1);
   WR_FILL_TKN_UINT(rptCfgEutra->triggerTyp.choice, TRIGGERTYP_EVENT)
   wrFillTknU8(&evntTyp->pres, 1);;
   WR_FILL_TKN_UINT(evntTyp->eventId.choice, EVENTID_EVENTA2)
   wrFillTknU8(&evntTyp->eventId.val.eventA2.pres, 1);
   if(thrshldTyp == WR_UMM_MEAS_THRESHOLD_RSRP)
   {
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA2.a2_Threshold.choice,
                                        THRESHOLDEUTRA_THRESHOLD_RSRP)
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA2.a2_Threshold.val.threshold_RSRP,
                                         a2EvntRptCfg-> threshold_RSRP)
   } 
   else 
   {
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA2.a2_Threshold.choice,
                                         THRESHOLDEUTRA_THRESHOLD_RSRQ)
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA2.a2_Threshold.val.threshold_RSRQ,
                                         a2EvntRptCfg-> threshold_RSRQ)
   }
   WR_FILL_TKN_UINT(evntTyp->hysteresis , a2EvntRptCfg->hysteresis)
   WR_FILL_TKN_UINT( evntTyp->timeToTrigger , a2EvntRptCfg->timeToTrigger)
   if(thrshldTyp == WR_UMM_MEAS_THRESHOLD_RSRP)
   {
      WR_FILL_TKN_UINT(rptCfgEutra->triggerQuantity ,THRESHOLDEUTRA_THRESHOLD_RSRP)
   } 
   else
   {
      WR_FILL_TKN_UINT(rptCfgEutra->triggerQuantity , THRESHOLDEUTRA_THRESHOLD_RSRQ)
   }
   WR_FILL_TKN_UINT(rptCfgEutra->reportQuantity , a2EvntRptCfg->reportQty)
   WR_FILL_TKN_UINT(rptCfgEutra->maxReportCells , a2EvntRptCfg->maxRptCells)
   WR_FILL_TKN_UINT(rptCfgEutra->reportInterval , a2EvntRptCfg->reportInterval)
   WR_FILL_TKN_UINT(rptCfgEutra->reportAmount , a2EvntRptCfg->reportAmount)

   RETVALUE(ROK);
}
/** @brief This function is responsible for to fill Event A1 report config
 * Object.
 *
 *  @details
 *  Function:wrUmmMeasFillEvntA1RptObj
 *      
 *  Processing steps:
 *      - Fill the report config for event A1 report config object
 *      
 *   @param[out] rptCfgEutra: pointer to Eutra report config Object.
 *   @param[in] thrshldTyp: threshold type.
 *   @param[out] evntTyp: pointer to Event Type structure.
 *   @param[in] a1EvntRptCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasFillEvntA1RptObj
(
NhuReportConfigEUTRA                *rptCfgEutra,
WrThrshldTyp                        thrshldTyp,
NhuReportConfigEUTRAtriggerTypevent *evntTyp, 
LwrMeasEvntRptCfg                    *a1EvntRptCfg
)
{
   TRC2(wrUmmMeasFillEvntA1RptObj)
   wrFillTknU8(&rptCfgEutra->pres, 1);
   WR_FILL_TKN_UINT(rptCfgEutra->triggerTyp.choice , TRIGGERTYP_EVENT)
   wrFillTknU8(&evntTyp->pres, 1);
   WR_FILL_TKN_UINT(evntTyp->eventId.choice , EVENTID_EVENTA1)
   wrFillTknU8(&evntTyp->eventId.val.eventA1.pres, 1);
   if(thrshldTyp == WR_UMM_MEAS_THRESHOLD_RSRP)
   {
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA1.a1_Threshold.choice,
                                         THRESHOLDEUTRA_THRESHOLD_RSRP)
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA1.a1_Threshold.val.threshold_RSRP,
                                           a1EvntRptCfg-> threshold_RSRP)
   } 
   else 
   {
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA1.a1_Threshold.choice,
                                            THRESHOLDEUTRA_THRESHOLD_RSRQ)
      WR_FILL_TKN_UINT(evntTyp->eventId.val.eventA1.a1_Threshold.val.threshold_RSRQ,
                                            a1EvntRptCfg-> threshold_RSRQ)
   } 
   WR_FILL_TKN_UINT(evntTyp->hysteresis , a1EvntRptCfg->hysteresis)
   WR_FILL_TKN_UINT(evntTyp->timeToTrigger , a1EvntRptCfg->timeToTrigger)
   if(thrshldTyp == WR_UMM_MEAS_THRESHOLD_RSRP)
   {
      WR_FILL_TKN_UINT(rptCfgEutra->triggerQuantity , THRESHOLDEUTRA_THRESHOLD_RSRP)
   }
   else 
   {
      WR_FILL_TKN_UINT(rptCfgEutra->triggerQuantity , THRESHOLDEUTRA_THRESHOLD_RSRQ)
   }
   WR_FILL_TKN_UINT(rptCfgEutra->reportQuantity , a1EvntRptCfg->reportQty)
   WR_FILL_TKN_UINT(rptCfgEutra->maxReportCells , a1EvntRptCfg->maxRptCells)
   WR_FILL_TKN_UINT(rptCfgEutra->reportInterval , a1EvntRptCfg->reportInterval)
   WR_FILL_TKN_UINT(rptCfgEutra->reportAmount , a1EvntRptCfg->reportAmount)
   RETVALUE(ROK);
}
/** @brief This function is responsible for to create report config into Ue cb
 *
 *  @details
 *  Function:wrUmmMeasCreateRptCfg
 *    
 *  Processing steps:
 *      - create report config and store in Ue control block.
 *      
 *   @param[in] rptCfgId: report config Id.
 *   @param[in] rptCfgInfo: internal report config strucure.
 *   @param[out] rptCfg: pointer to th report config object.
 *   @param[in] measCellCfg: Pointre to the measurement Cell config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PUBLIC S16 wrUmmMeasCreateRptCfg
(
U8                                    rptCfgId,
WrUmmMeasRptCfgInfo                   *rptCfgInfo,
WrMeasRptCfgNode                      *rptCfg,
LwrCellMeasCfgGrp                     *measCellCfg,
WrUeCb                                *ueCb/*rel10*/
)
{
   NhuReportConfigEUTRA                        *rptCfgEutra;
   NhuReportConfigInterRAT                     *rptCfgUtra;
   NhuReportConfigInterRAT                     *rptCfgGeran;
   NhuReportConfigInterRAT                     *rptCfgCdma2k;
   NhuReportConfigInterRATtriggerTypevent      *utraEvntTyp; 
   NhuReportConfigEUTRAtriggerTypevent         *eutraEvntTyp; 
   NhuReportConfigInterRATtriggerTypevent      *interRatEvntTyp; 
   NhuReportConfigEUTRAtriggerTypperiodical    *eutraPerdTyp; 
   NhuReportConfigInterRATtriggerTypperiodical *interRatPerdTyp;
   NhuReportConfigInterRATtriggerTypperiodical *utraPerdTyp; 
   NhuReportConfigInterRATtriggerTypperiodical *geranPerdTyp;
         
   TRC2(wrUmmMeasCreateRptCfg)

   rptCfg->rptCfgType = rptCfgInfo->rptCfgTyp;
   rptCfg->rptCfgId = rptCfgId;
   rptCfg->intRptCfgTyp = rptCfgInfo->intRptCfgType;
   switch (rptCfgInfo->rptCfgTyp)
   {
      case WR_UMM_MEAS_EVENT_A1:
      {
         rptCfgEutra = &rptCfg->val.eventA124;
         eutraEvntTyp = &rptCfgEutra->triggerTyp.val.event; 
         
         wrUmmMeasFillEvntA1RptObj(rptCfgEutra,rptCfgInfo->thrshldTyp,eutraEvntTyp,
          &measCellCfg->measA1EvtRptCfg[WR_MEAS_EVENTA1_HO_RSRP]);

         break;
      }
      case WR_UMM_MEAS_EVENT_A2:
      {
         rptCfgEutra = &rptCfg->val.eventA124;
         eutraEvntTyp = &rptCfgEutra->triggerTyp.val.event; 
         
         wrUmmMeasFillEvntA2RptObj(rptCfgEutra, rptCfgInfo->thrshldTyp, eutraEvntTyp,
              &measCellCfg->measA2EvtRptCfg[WR_MEAS_EVENTA2_HO_RSRP]);
         break;
      }
      case WR_UMM_MEAS_EVENT_A3:
      {
         rptCfgEutra = &rptCfg->val.eventA3;
         eutraEvntTyp = &rptCfgEutra->triggerTyp.val.event; 
         if(rptCfgInfo->intRptCfgType == WR_UMM_MEAS_RPTCFG_INTRA_ANR)
         {
            wrUmmMeasFillEvntA3RptObj(rptCfgEutra, rptCfgInfo->thrshldTyp, eutraEvntTyp,
                  &measCellCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ANR]);
         }
/*ccpu00133426 - inter freq ho requirement - start*/
         else if(rptCfgInfo->intRptCfgType == WR_UMM_MEAS_RPTCFG_INTER_RSRP)
         {
            wrUmmMeasFillEvntA3RptObj(rptCfgEutra, rptCfgInfo->thrshldTyp, eutraEvntTyp,
                  &measCellCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTER_FREQ_HO_RSRP]);
         }
/*ccpu00133426 - inter freq ho requirement - end*/
         else
         {
            wrUmmMeasFillEvntA3RptObj(rptCfgEutra, rptCfgInfo->thrshldTyp, eutraEvntTyp,
                  &measCellCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_HO]);
         }
         break;
      }
      case WR_UMM_MEAS_EVENT_A5:
      {
         rptCfgEutra = &rptCfg->val.eventA5;
         eutraEvntTyp = &rptCfgEutra->triggerTyp.val.event; 
         
         if(rptCfgInfo->intRptCfgType == WR_UMM_MEAS_RPTCFG_INTER_ANR)
         {
            wrUmmMeasFillEvntA5RptObj(rptCfgEutra,rptCfgInfo->thrshldTyp, eutraEvntTyp,
            &measCellCfg->measEvtA5Cfg[WR_MEAS_EVENTA5_INTER_FREQ_ANR]);
         }
         else if(rptCfgInfo->intRptCfgType == WR_UMM_MEAS_RPTCFG_INTRA)
         {
            wrUmmMeasFillEvntA5RptObj(rptCfgEutra,rptCfgInfo->thrshldTyp, eutraEvntTyp,
            &measCellCfg->measEvtA5Cfg[WR_MEAS_EVENTA5_INTRA_FREQ_HO]);
         }
         else
         {
            wrUmmMeasFillEvntA5RptObj(rptCfgEutra,rptCfgInfo->thrshldTyp, eutraEvntTyp,
            &measCellCfg->measEvtA5Cfg[WR_MEAS_EVENTA5_INTER_FREQ_HO_RSRP]);
         }
         break;
      }
      case WR_UMM_MEAS_EVENT_B2:
      {
         rptCfgCdma2k = &rptCfg->val.eventB2;
         interRatEvntTyp = &rptCfgCdma2k->triggerTyp.val.event; 
         
         if(rptCfgInfo->cdma2kCellType == WR_UMM_MEAS_CDMA2K_1XRTT)
         {
            wrUmmMeasFillEvntB2RptObj(rptCfgCdma2k, rptCfgInfo->thrshldTyp, interRatEvntTyp,
               &measCellCfg->meas1xCDMAEventB2RptCfg[WR_MEAS_1XCDMA_EVENTB2_INTER_RAT_HO]);
         }
         else
         {
            wrUmmMeasFillEvntB2RptObj(rptCfgCdma2k, rptCfgInfo->thrshldTyp, interRatEvntTyp,
               &measCellCfg->measCDMAHrpdEventB2RptCfg[WR_MEAS_CDMA_HRPD_EVENTB2_OPT_HRPD_HO]);
         }
         break;
      }
      case WR_UMM_MEAS_EVENT_B2_GERAN:
      {
         rptCfgGeran = &rptCfg->val.b2Geran;
         utraEvntTyp = &rptCfgGeran->triggerTyp.val.event; 

         if(WR_UMM_MEAS_RPTCFG_IRATB2_REDIR == rptCfgInfo->intRptCfgType) 
         {
            wrUmmMeasFillEvntGeranB2RptObj(rptCfgGeran, rptCfgInfo, utraEvntTyp,
            &measCellCfg->measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_REDIR],ueCb);
         }
         else
         {
            wrUmmMeasFillEvntGeranB2RptObj(rptCfgGeran, rptCfgInfo, utraEvntTyp,
            &measCellCfg->measGeranEventB2RptCfg[WR_MEAS_GERAN_EVENTB2_INTER_RAT_HO],ueCb);
         }
         break;
      }
      case WR_UMM_MEAS_EVENT_B2_UTRA:
      {
         rptCfgUtra = &rptCfg->val.b2Utra;
         utraEvntTyp = &rptCfgUtra->triggerTyp.val.event; 

         if(WR_UMM_MEAS_RPTCFG_IRATB2_REDIR == rptCfgInfo->intRptCfgType) 
         {
            wrUmmMeasFillEvntUtraB2RptObj(rptCfgUtra, rptCfgInfo, utraEvntTyp,
                  &measCellCfg->measUtraEventB2RptCfg[WR_MEAS_UTRA_EVENTB2_REDIR],
                  ueCb);
         }
         else
         {
            wrUmmMeasFillEvntUtraB2RptObj(rptCfgUtra, rptCfgInfo, utraEvntTyp,
                  &measCellCfg->measUtraEventB2RptCfg[WR_MEAS_UTRA_EVENTB2_HO],
                  ueCb);
         }
         break;
      }
      case WR_UMM_MEAS_STRONG_CELL:
      {
         rptCfgEutra = &rptCfg->val.strgCell;
         eutraPerdTyp = &rptCfgEutra->triggerTyp.val.periodical; 
         
         wrUmmMeasFillEutraStrngCellRptObj(rptCfgEutra, eutraPerdTyp,
         &measCellCfg->measEutraPerdRptCfg[WR_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS],
         ueCb->isMeasCfgForPerdStrongCellRptCfg);
         break;
      }
      case WR_UMM_MEAS_STRONG_CELL_ANR:
      {
         rptCfgCdma2k = &rptCfg->val.strgCellAnr;
         interRatPerdTyp = &rptCfgCdma2k->triggerTyp.val.periodical; 
         
         if(rptCfgInfo->cdma2kCellType == WR_UMM_MEAS_CDMA2K_1XRTT)
         {
            wrUmmMeasFillStrngCellAnrRptObj(rptCfgCdma2k, interRatPerdTyp,
            &measCellCfg->meas1xCDMAPeridRptCfg[WR_MEAS_CDMA_PERIODIC_REPORT_STRNG_CELLS_ANR]);
         }
         else
         {
            wrUmmMeasFillStrngCellAnrRptObj(rptCfgCdma2k, interRatPerdTyp,
            &measCellCfg->measCDMAHrpdPeridRptCfg[WR_MEAS_CDMA_PERIODIC_REPORT_STRNG_CELLS_ANR]);
         }
         break;
      }
      case WR_UMM_MEAS_UTRA_STRONG_CELL_ANR:
      {
         rptCfgUtra = &rptCfg->val.utraStrgCellAnr;
         interRatPerdTyp = &rptCfgUtra->triggerTyp.val.periodical; 
         
         wrUmmMeasFillUtraStrngCellAnrRptObj(rptCfgUtra, interRatPerdTyp,
         &measCellCfg->measUtraPerdRptCfg[WR_MEAS_UTRA_PRDRPT_STRGCELLS_ANR]);
         break;
      }
      case WR_UMM_MEAS_REPORT_CGI:
      {
         rptCfgEutra = &rptCfg->val.strgCell;
         eutraPerdTyp = &rptCfgEutra->triggerTyp.val.periodical; 
         
         wrUmmMeasFillEutraRptCgiObj(rptCfgEutra, eutraPerdTyp,
          &measCellCfg->measEutraPerdRptCfg[WR_MEAS_EUTRA_PERIODIC_REPORT_CGI]);
         break;
      }
      case WR_UMM_MEAS_UTRA_REPORT_CGI:
      {
         rptCfgUtra = &rptCfg->val.utraRptCgi;
         utraPerdTyp = &rptCfgUtra->triggerTyp.val.periodical; 

         wrUmmMeasFillUtraRptCgiObj(rptCfgUtra, utraPerdTyp,
         &measCellCfg->measUtraPerdRptCfg[WR_MEAS_UTRA_PERIODIC_REPORT_CGI]);
         break;
      }
      /* Updated WR_UMM_MEAS_INTER_RAT_REPORT_CGI
       * as WR_UMM_MEAS_CDMA_REPORT_CGI */
      case WR_UMM_MEAS_CDMA_REPORT_CGI:
      {
         rptCfgCdma2k = &rptCfg->val.strgCellAnr;
         interRatPerdTyp = &rptCfgCdma2k->triggerTyp.val.periodical; 
         
         wrUmmMeasFillInterRatRptCgiObj(rptCfgCdma2k, interRatPerdTyp,
         &measCellCfg->meas1xCDMAPeridRptCfg[WR_MEAS_CDMA_PERIODIC_REPORT_CGI]);
         break;
      }
/* LTE_ADV_ABS starts */
      case WR_UMM_MEAS_EVENT_A3_ABS:
      {
         rptCfgEutra = &rptCfg->val.eventA3;
         eutraEvntTyp = &rptCfgEutra->triggerTyp.val.event;

         wrUmmMeasFillEvntA3RptObj(rptCfgEutra, rptCfgInfo->thrshldTyp, eutraEvntTyp,
              &measCellCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ABS]);
         break;
      }
/* LTE_ADV_ABS ends */
      /* configure the IRAT ReportStrongestCell Report type */
      case WR_UMM_MEAS_CDMA_STRONG_CELL:
      {
         rptCfgCdma2k = &rptCfg->val.strgCellAnr;
         interRatPerdTyp = &rptCfgCdma2k->triggerTyp.val.periodical; 

         if(rptCfgInfo->cdma2kCellType == WR_UMM_MEAS_CDMA2K_1XRTT)
         {
            wrUmmMeasFillCdmaRptCsfb(rptCfgCdma2k, interRatPerdTyp,
            &measCellCfg->meas1xCDMAPeridRptCfg[WR_MEAS_CDMA_PERIODIC_REPORT_STRNG_CELLS]);
         }
         else
         {
            wrUmmMeasFillCdmaRptCsfb(rptCfgCdma2k, interRatPerdTyp,
            &measCellCfg->measCDMAHrpdPeridRptCfg[WR_MEAS_CDMA_PERIODIC_REPORT_STRNG_CELLS]);
         }
         break;
      }
      case WR_UMM_MEAS_UTRA_STRONG_CELL_CSFB:
      {
         rptCfgUtra = &rptCfg->val.strgCellCsfb;
         utraPerdTyp = &rptCfgUtra->triggerTyp.val.periodical;

         wrUmmMeasFillUtraRptCsfb(rptCfgUtra, utraPerdTyp,
               &measCellCfg->measUtraPerdRptCfg[WR_MEAS_UTRA_PRDRPT_STRGCELLS_CSFB], ueCb);
         break;
      }
      case WR_UMM_MEAS_GERAN_STRONG_CELL_CSFB:
      {
         rptCfgGeran = &rptCfg->val.strgCellCsfb;
         geranPerdTyp = &rptCfgGeran->triggerTyp.val.periodical;
         wrUmmMeasFillGeranRptCsfb(rptCfgGeran,geranPerdTyp,
               &measCellCfg->measGERANPeridRptCfg[LWR_MEAS_GERAN_REPORT_STRNG_CELLS]);
         break;
      }

      default:
      break;
   }
   RETVALUE(ROK);
}
   /** @brief This function is responsible for Creating ReportConfig objects and
    * measurement Id objects.
    *
    *  @details*
    *  Function:wrUmmMeasRptAndMeasIdCfg
    *
    *  Processing steps:
    *      - create report config Object in UeCb.
    *      - create measurement Id Object in UeCb.
    *
    *   @param[in]  measIdType: measurement ID Type.
    *   @param[in]  measObjId:  measurement Object ID.
    *   @param[in]  rptCfgInfo: pointer to report config info.
    *   @param[in]  ueCb: pointer to Ue Control Block.
    *   @param[in]  measCellCfg: pointer to measurement cell config.
    *   @param[in]  addMeasTransCb: pointer to addition of measurement 
    *                          transaction  control block.
    *   @return     S16 
    *   SUCCESS:   ROK
    *   FAILED :  RFAILED
    */
PUBLIC U8 wrUmmMeasRptAndMeasIdCfg
(
   WrUmmMeasTyp                measIdTyp,
   U8                          measObjId,
   WrUmmMeasRptCfgInfo        *rptCfgInfo,
   WrUeCb                     *ueCb,/*rel10*/
   LwrCellMeasCfgGrp          *measCellCfg,
   WrUmmMeasAddInfo            *addMeasTransCb
)
{
   U8                          measId = 0;
   U8                          rptCfgId = 0;
   WrMeasUeCb                  *measUeCb = ueCb->measCfg;
   TRC2(wrUmmMeasRptAndMeasIdCfg)

      if(ROK != wrUmmMeasFindRptCfgID(&rptCfgId, rptCfgInfo, &measUeCb->rptCfgLst))
      {
         rptCfgId = wrUmmMeasCreatRptCfgObjInUeCb(rptCfgInfo, ueCb, measCellCfg);
         if(rptCfgId != WR_UMM_MEAS_INVALID_MEAS_ID)
         {
            addMeasTransCb->rptCfgLst[addMeasTransCb->rptCfgIdCnt] = rptCfgId;
            addMeasTransCb->rptCfgIdCnt++;
         }
         else
         {
            RLOG_ARG0(L_ERROR, DBG_CRNTI, measUeCb->crnti,
                  "Invalid report config Id");
            RETVALUE(RFAILED);
         }
      }

   measId = wrUmmMeasCreatMeasIdNode(measIdTyp,measObjId, rptCfgId, measUeCb);
   if(measId != WR_UMM_MEAS_INVALID_MEAS_ID)
   {
      addMeasTransCb->measIdLst[addMeasTransCb->measIdCnt] = measId;
      addMeasTransCb->measIdCnt++;
   }
   else
   {
      RLOG0(L_ERROR, "Invalid measId");
      RETVALUE(RFAILED);
   }

   if(NhuReportConfigEUTRAreportAmountr1Enum ==  measCellCfg->
         measEutraPerdRptCfg[WR_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS].reportAmount)
   {
      measUeCb->measIdForR1 = measId;      
   }
   RETVALUE(ROK);
}


/** @brief This function is responsible for to create report config in Ue Cb.
 *
 *  @details*
 *  Function:wrUmmMeasCreatRptCfgObjInUeCb
 *    
 *  Processing steps:
 *      - Get the report Config Id from free List.
 *      - Create new report config node 
 *      - update the report config node add to Ue Cb Linked List
 *  
 *   @param[in] rptCfgTyp: pointer to Report config Type structure
 *   @param[out] measUeCb: pointer to measurement ue Cb.
 *   @param[in] measCellCfg: pointer to measurement cell config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC U8 wrUmmMeasCreatRptCfgObjInUeCb
(
   WrUmmMeasRptCfgInfo       *rptCfgInfo,
   WrUeCb                    *ueCb,/*rel10*/
   LwrCellMeasCfgGrp         *measCellCfg
)
{
   WrMeasRptCfgNode          *rptCfgNode;
   WrMeasUeCb                *measUeCb = ueCb->measCfg;
   U8                        rptCfgId = 0;
   S16                       ret;
   
   TRC2(wrUmmMeasCreatRptCfgObjInUeCb)
      
   ret = wrUmmMeasGetFreeRptCfgId(measUeCb, &rptCfgId);
   if(WR_UMM_MEAS_INVALID_MEAS_ID == ret)
   {
      RLOG0(L_ERROR, "Report config Id Not available");
      RETVALUE(ret);
   }
   WR_ALLOC(&rptCfgNode, sizeof(WrMeasRptCfgNode));
   if(NULLP == rptCfgNode)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      wrUmmMeasSetFreeRptCfgId(measUeCb, rptCfgId);
      RETVALUE(RFAILED);
   }
   /*rel10*/
   wrUmmMeasCreateRptCfg(rptCfgId, rptCfgInfo, rptCfgNode, measCellCfg, ueCb);
   rptCfgNode->lnk.node = (PTR)rptCfgNode;
   cmLListAdd2Tail(&measUeCb->rptCfgLst, &rptCfgNode->lnk);      
   RETVALUE(rptCfgId);
}
 

/** @brief This function is responsible for Configuring intra frequency
 * measurement configuration in Ue control block at the time of initial context
 * setup request procedure.
 *
 *  @details*
 *  Function:wrUmmMeasCreatIntraInitalCfgMsg
 *  
 *  Processing steps:
 *      - create Intra Measurement object in Ue Control block.
 *      - configure Event A2 and Strongest cell report config
 *      - copy meas Object id, report config id and meas id into transaction
 *      control block.
 *  
 *   @param[in]  cellCb: pointer to cell Control block.
 *   @param[in]  measTransCb: pointer to measurement transaction control block.
 *   @param[in]  ueCb : Pointer to UE control block
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
   
PRIVATE S16 wrUmmMeasCreatIntraInitalCfgMsg
(
 WrUmmCellCb                 *cellCb,
 WrUmmMeasAddInfo            *addMeasInfo,
 WrUeCb                      *ueCb,
 MeasInitCause               cause
)
{
   U8                          measObjId = 0;
   WrMeasUeCbEutraMeasObjNode  *enbMeasObj;
   LwrCellMeasCfgGrp           *measCellCfg;
   WrUmmMeasCellCb             *measCellCb;
   WrUmmMeasRptCfgInfo         rptCfgInfo;
   Bool                        ueCapAnrSupport = TRUE;
   WrMeasUeCb                  *measUeCb = NULLP;

   TRC2(wrUmmMeasCreatIntraInitalCfgMsg)

   measUeCb = ueCb->measCfg;
   measCellCfg = cellCb->measCellCfg;
   measCellCb = cellCb->measCellCb;
   enbMeasObj = &measUeCb->intraMeasObj;
   

   if(WR_UMM_MEAS_INVALID_MEAS_ID != wrUmmMeasGetFreeMeasObjId(measUeCb, &measObjId))
   {
      enbMeasObj->eutraMeasObj = &measCellCb->intraMeasObj;
      enbMeasObj->measObjId = measObjId;
      enbMeasObj->newPci = 0;

      addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
      addMeasInfo->measObjIdCnt++;
   }
   else
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, measUeCb->crnti,
            "Meas Object Id not available");
      RETVALUE(RFAILED);
   }
   
   LwrMeasEutraPeridRptCfg   *eutraPerd;
   eutraPerd = &measCellCfg->measEutraPerdRptCfg[WR_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS];
   if(eutraPerd->isPerdStrongCellCfgRptEnable == TRUE)
   {
      wrUmmPerdStrongCellMeasCfg(ueCb, measObjId, measCellCfg, addMeasInfo);
   }

   /* If Event ANR algorithm is configured and No GBR bearer is configured 
      then only configure Event measurement configuration */
   if(((measCellCb->anrRptCfg == WR_UMM_EVENT_A3_CFG) || 
       (measCellCb->anrRptCfg == WR_UMM_EVENT_A5_INTER_CFG)) && 
       (ueCb->isGBRBearerExists == NOT_IN_USE) && (cause == WR_MEAS_CONFIG_EVT_ANR)) 
   {
      /* Check does the UE support intra A3 event  configuration */
      ueCapAnrSupport = wrUmmAnrChkUeCap(ueCb, WR_UMM_MEAS_INTRA_FREQ, WR_UMM_EVENT_A3_CFG);
      if(ueCapAnrSupport == TRUE)
      {
         rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_A3;
         if(!measCellCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_ANR].triggerQty)
         {
            /*RSRP*/
            rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
         }
         else
         {
            /*RSRQ*/
            rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRQ;
         }
         rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
         rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_INTRA_ANR;

         if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_EVENT_ANR, measObjId,
               &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
         {
            RLOG_ARG0(L_ERROR, DBG_CRNTI, measUeCb->crnti,
                  "Invalid report and Meas config Id");
            RETVALUE(RFAILED);
         }
      }
   }
   if(measCellCb->hoRptCfg == WR_UMM_EVENT_A3_CFG)
   {
      rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_A3;
    if(!measCellCfg->measEvtA3Cfg[WR_MEAS_EVENTA3_INTRA_FREQ_HO].triggerQty)
    {
       rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
    }
    else
    {
       /*RSRQ*/
       rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRQ;
    }
      rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
      rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;
   }
   else
   {
      rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_A5;
      if(!measCellCfg->measEvtA5Cfg[WR_MEAS_EVENTA5_INTER_FREQ_HO_RSRP].triggerQty)
      {
         rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
      }
      else
      {
         rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRQ;
      }

      rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
      rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_INTRA;
   }

   /* Report Config for Event A3/A5 */
   if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_HO, measObjId,
            &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
   {
      RLOG0(L_ERROR, "Invalid report and Meas config Id");
      RETVALUE(RFAILED);
   }
   /* Report Config for Event A2 */
   rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_A2;
   if(!measCellCfg->measA2EvtRptCfg[WR_MEAS_EVENTA2_HO_RSRP].triggerQty)
   {
      rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
   }
   else
   {
      rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRQ;
   }
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

   if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_HO, measObjId,
            &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
   {
      RLOG0(L_ERROR, "Invalid report and Meas config Id");
      RETVALUE(RFAILED);
   }
   RLOG0(L_DEBUG,"Event A2 Configured");
   RETVALUE(ROK);
}

/** @brief This function is responsible for to Add inter measurement Object
 * into uecb.
 * 
 *  @details
 * Function:wrUmmMeasAddInterMeasObjToUeCb
 *
 *   Processing steps:
 *    - Add inter measurement object into UE control block.
 *     
 * @param[in]  measUeCb: pointer to the measurement ue Cb         
 * @param[out]  cellCbMeasObj: pointer to the CellCb measurement Object. 
 * @param[in]  newPci: New PCI. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PUBLIC U8 wrUmmMeasAddInterMeasObjToUeCb
(
 WrMeasUeCb                  *measUeCb,
 WrUmmMeasEutraMeasObjNode   *cellCbMeasObj,
 U16                         newPci
)
{
   WrMeasUeCbEutraMeasObjNode     *measObjEutraNode;
   U8                              measObjId = 0;
   S16                             ret;

   TRC2(wrUmmMeasAddInterMeasObjToUeCb)
   ret =  wrUmmMeasGetFreeMeasObjId(measUeCb, &measObjId);
   if(ret == WR_UMM_MEAS_INVALID_MEAS_ID)
   {
      RLOG0(L_ERROR, "Meas Object Id not available");
      RETVALUE(ret);
   }
   WR_ALLOC(&measObjEutraNode,  sizeof(WrMeasUeCbEutraMeasObjNode));          
   if(NULLP == measObjEutraNode)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      wrUmmMeasSetFreeMeasObjId(measUeCb, measObjId);
      RETVALUE(RFAILED);
   }

   measObjEutraNode->eutraMeasObj = cellCbMeasObj;
   measObjEutraNode->measObjId = measObjId;
   measObjEutraNode->newPci = newPci;

   measObjEutraNode->lnk.node = (PTR)measObjEutraNode;
   cmLListAdd2Tail(&measUeCb->interMeasObjLst, &measObjEutraNode->lnk);      
   RETVALUE(measObjId);
}
/** @brief This function is responsible for to Add inter RAT measurement Object
 * into uecb.
 * 
 *  @details
 * Function:wrUmmMeasAddInterRatMeasObjToUeCb
 *
 *   Processing steps:
 *    - Add inter rat measurement object into UE control block.
 *         
 * @param[in]  measUeCb: pointer to the measurement ue Cb         
 * @param[out]  cellCbMeasObj: pointer to the CellCb measurement Object. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE U8 wrUmmMeasAddInterRatMeasObjToUeCb
(
 WrMeasUeCb                  *measUeCb,
 WrUmmMeasCdmaMeasObjNode    *cellCbMeasObj
)
{
   U8                           measObjId = 0; 
   S16                          ret;
   WrMeasUeCbCdmaMeasObjNode    *cdmaObj;

   TRC2(wrUmmMeasAddInterRatMeasObjToUeCb)
   ret = wrUmmMeasGetFreeMeasObjId(measUeCb, &measObjId);
   if(ret == WR_UMM_MEAS_INVALID_MEAS_ID)
   {
      RLOG0(L_ERROR, "Meas Object Id not available");
      RETVALUE(ret);
   }
   WR_ALLOC(&cdmaObj,  sizeof(WrMeasUeCbCdmaMeasObjNode));          
   if(NULLP == cdmaObj)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      wrUmmMeasSetFreeMeasObjId(measUeCb, measObjId);
      RETVALUE(RFAILED);
   }

   cdmaObj->cdmaMeasObj = cellCbMeasObj;
   cdmaObj->measObjId = measObjId;
   cdmaObj->newPci = 0;
   
   cdmaObj->lnk.node = (PTR)cdmaObj;
   cmLListAdd2Tail(&measUeCb->interRatMeasObjLst, &cdmaObj->lnk);

   RETVALUE(measObjId);
}
/** @brief This function is responsible for Allocate Reconfiguration message and
 * intializing Object.
 *
 *  @details*
 *  Function:wrUmmMeasAllocRecfgMsg
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
PUBLIC S16 wrUmmMeasAllocRecfgMsg
(
 NhuDatReqSdus             **rrcReCfgMsg,
 WrUmmMeasTransInfo        *measTransCb,
 WrUeCb                    *ueCb,
 U32                        transId
)
{
   NhuDatReqSdus             *rrcMsg;
   
   TRC2(wrUmmMeasAllocRecfgMsg)

   /* Fetch an RRC transaction id for this procedure.                     */
   if(ROK != wrUmmGetRrcTransId(ueCb, C1_RRCCONRECFGN,
         WR_GET_TRANS_FRM_TRANSID(transId),
         &measTransCb->transId))
   {
      RLOG0(L_ERROR, "RRC Transaction not available");
      RETVALUE(RFAILED);
   }
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
   rrcMsg->hdr.transId = transId;

   /* Create the reconfiguration message which is constructued excluding
    * */
   /* the actual procedure related details which will be added further
    * */
   if (wrUmmRrcCreateRrcReconfigMsg(*rrcReCfgMsg, measTransCb->transId) != ROK)
   {
      RLOG0(L_ERROR, "Intialization failed");
      WR_FREEEVNT(rrcReCfgMsg);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to Fill measurement Gap config into
 * Nhu measurement Object.
 *
 *  @details*
 *  Function:wrUmmMeasFillMeasGapCfg
 *  
 *  Processing steps:
 *      - Check the Meas Gap Flag
 *      - if Flag is TRUE, release the measurement Gap
 *      - if Flag is FLASE, get Gap offset value from Ue CB
 *      - Copy measurement Gap offset value into NHU measurement Object.
 *  
 *   @param[in]  measGapStatus: status of Measurement gap.
 *   @param[in]  measUeCb: pointer to measurement Ue control block.
 *   @param[out] nhuMeasGap: pointer to Nhu measurement Gap config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasFillMeasGapCfg
(
WrUmmMeasGapStatus           measGapStatus,
WrMeasUeCb                   *measUeCb,
NhuMeasGapConfig             *nhuMeasGap
)
{
   TRC2(wrUmmMeasFillMeasGapCfg)

   if(measGapStatus == WR_UMM_MEAS_GAP_INIT_UE_CONFIG)
   {
      WR_FILL_TKN_UINT(nhuMeasGap->choice, MEASGAPCONFIG_SETUP)
      wrFillTknU8(&nhuMeasGap->val.setup.pres, 1);
      if(measUeCb->gapMeasInfo.gapPattern == WR_UMM_MEAS_GAP_PERD_40MS)
      {
         WR_FILL_TKN_UINT(nhuMeasGap->val.setup.gapOffset.choice, GAPOFFSET_GP0)
         WR_FILL_TKN_UINT(nhuMeasGap->val.setup.gapOffset.val.gp0, measUeCb->gapMeasInfo.measGapOffset)
      }
      else
      {
         WR_FILL_TKN_UINT(nhuMeasGap->val.setup.gapOffset.choice, GAPOFFSET_GP1)
         WR_FILL_TKN_UINT(nhuMeasGap->val.setup.gapOffset.val.gp1, measUeCb->gapMeasInfo.measGapOffset)
      }
   }
   else if(measGapStatus == WR_UMM_MEAS_GAP_REL)
   {
      WR_FILL_TKN_UINT(nhuMeasGap->choice, MEASGAPCONFIG_RELEASE)
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for to Fill Quantity, Mobility State 
 * Params and Speed State Params into Nhu measurement Object.
 *
 *  @details*
 *  Function:wrUmmMeasFillQtySpdStatCfg
 *    
 *  Processing steps:
 *      - Quantity, Mobility State Params and Speed State Params into
 *      Nhu measurement Object.
 *  
 *   @param[in] memCp: pointer to memoty control pointer.
 *   @param[out] nhuMeasCfg: pointer to Nhu measurement Object.
 *   @param[in]  measCellCfg: pointer to measurement Cell Config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasFillQtySpdStatCfg
(
CmMemListCp                  *memCp,
NhuMeasConfig                *nhuMeasCfg,
LwrCellMeasCfgGrp            *measCellCfg,
NhuUE_EUTRA_Cap              *ueCap/*rel10*/
)
{
   LwrMeasQtyCfgParams          *qtyOamParams;
   LwrMeasMobilityStatParams    *mobStatParams;
   LwrMeasSpdStateScaleFact    *spdSclFac;
   LwrMeasPreRegHrpd            *preRegHrpd;
   NhuQuantityConfig           *qtyCfg;
   NhuMeasConfigspeedStatePars *spdStatePars;
   NhuMobilityStateParams      *mobStateParams;
   NhuSpeedStateScaleFactors   *timeToTrig;
   NhuPreRegistrationInfoHRPD  *nhuPreReghrpd;

   TRC2(wrUmmMeasFillQtySpdStatCfg)

   qtyOamParams = &measCellCfg->qtyCfg;
   mobStatParams = &measCellCfg->mobStatParams;
   spdSclFac = &measCellCfg->measSpdSclFact;
   preRegHrpd = &measCellCfg->preRegHrpd;
   qtyCfg = &nhuMeasCfg->quantityConfig;
   spdStatePars = &nhuMeasCfg->speedStatePars;
   mobStateParams = &spdStatePars->val.setup.mobilityStateParams;
   timeToTrig = &spdStatePars->val.setup.timeToTrigger_SF;
   nhuPreReghrpd = &nhuMeasCfg->preRegistrationInfoHRPD;

   /*S-Measure Configure*/
   WR_FILL_TKN_UINT(nhuMeasCfg->s_Measure, spdSclFac->sMeasure)


   wrFillTknU8(&qtyCfg->pres, 1);
    /* EUTRA Quantity config*/
   wrFillTknU8(&qtyCfg->quantityConfigEUTRA.pres, 1);
   WR_FILL_TKN_UINT(qtyCfg->quantityConfigEUTRA.filterCoefficientRSRP, qtyOamParams->
                                     rsrpFltrCoeff)
   WR_FILL_TKN_UINT(qtyCfg->quantityConfigEUTRA.filterCoefficientRSRQ, qtyOamParams->
                                     rsrqFltrCoeff)
  /* UTRA Quantity config*/
   wrFillTknU8(&qtyCfg->quantityConfigUTRA.pres, 1);
   WR_FILL_TKN_UINT(qtyCfg->quantityConfigUTRA.measQuantityUTRA_FDD, qtyOamParams->
                                     measQtyUtraFdd)
   WR_FILL_TKN_UINT(qtyCfg->quantityConfigUTRA.measQuantityUTRA_TDD, qtyOamParams->
                                     measQtyUtraTdd)
   WR_FILL_TKN_UINT(qtyCfg->quantityConfigUTRA.filterCoefficient, qtyOamParams->
                                     ecnoFltrCoeff)
   /*rel10*/
   /* Fill filterCoefficient2_FDD_r10 only if FGI bit 114 is set*/
   if(PRSNT_NODEF == ueCap->nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.featureGroupIndRel10_r10.pres) 
   {
      U32     fgi_rel10;
      WR_GET_U32_FRM_OSXL(fgi_rel10,ueCap->nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.featureGroupIndRel10_r10);
      if(fgi_rel10 & WR_UMM_UTRA_MEAS_RPRT_BOTH_CHK_BIT)
      {
         wrFillTknU8(&qtyCfg->extaddgrp_1.pres,PRSNT_NODEF);
         wrFillTknU8(&qtyCfg->extaddgrp_1.quantityConfigUTRA_v1020.pres,PRSNT_NODEF);
         wrFillTknU32(&qtyCfg->extaddgrp_1.quantityConfigUTRA_v1020.filterCoefficient2_FDD_r10,
               qtyOamParams->filterCoefficient2FDDr10);
      }
   }

  /* CDMA Quantity config*/
   wrFillTknU8(&qtyCfg->quantityConfigCDMA2000.pres, 1);
   WR_FILL_TKN_UINT(qtyCfg->quantityConfigCDMA2000.measQuantityCDMA2000, 
                                                  qtyOamParams->measQtyCDMA2k)

   /* GERAN Quantity config*/
   wrFillTknU8(&qtyCfg->quantityConfigGERAN.pres,PRSNT_NODEF);
   wrFillTknU32(&qtyCfg->quantityConfigGERAN.measQuantityGERAN
         ,WR_MEAS_QTY_CFG_GERAN_RSSI);
   wrFillTknU32(&qtyCfg->quantityConfigGERAN.filterCoefficient,
         qtyOamParams->fltrCoeffGERAN);

   /* Speed State Params config*/
  WR_FILL_TKN_UINT(spdStatePars->choice, 1)
  wrFillTknU8(&spdStatePars->val.setup.pres, 1);
  /*Mobility State Params */
  wrFillTknU8(&mobStateParams->pres, 1);
  WR_FILL_TKN_UINT(mobStateParams->t_Evaluation, mobStatParams->tEvalulation)
  WR_FILL_TKN_UINT(mobStateParams->t_HystNormal, mobStatParams->tHystNormal)
  WR_FILL_TKN_UINT(mobStateParams->n_CellChangeMedium, mobStatParams->nCellChangeMedium)
  WR_FILL_TKN_UINT(mobStateParams->n_CellChangeHigh, mobStatParams->nCellChangeHigh)

  /*Speed State scaling Factor Params */
  wrFillTknU8(&timeToTrig->pres, 1);
  WR_FILL_TKN_UINT(timeToTrig->sf_Medium, spdSclFac->timeToTriggerSfMedium)
  WR_FILL_TKN_UINT(timeToTrig->sf_High, spdSclFac->timeToTriggerSfHigh)
  
   RETVALUE(ROK);
}

/** @brief This function is responsible for to create internal transaction.
 * 
 *  @details
 * Function:wrUmmMeasCreateIntTrans
 * 
 *   Processing steps:
 *    - Create internal transaction
 *     
 * @param[in]  cellCb: pointer to the cellCb         
 * @param[in]  ueCb: pointer to the Ue control block. 
 * @param[in]  measIntTransTyp: Internal message transaction type. 
 * @param[in]  nrFreqTyp: Neighbour frequency Type. 
 * @param[in]  intMeasTrans: pointer to the WrUmmIntMeasTrans object. 
 * @param[in]  reCfgInfo: pointer to the WrMeasCellReCfgInfo object. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasCreateIntTrans
(
WrUmmCellCb                  *cellCb,
WrUeCb                       *ueCb,
U8                            measIntTransTyp,
WrUmmMeasNrFreqTyp            nrFreqtyp,
WrUmmIntMeasTrans            *intMeasTrans,
WrMeasCellReCfgInfo          *reCfgInfo
)
{
   WrUmmInternalMsg          *internalMsg;
   WrUmmIncMsg               *msg;

   TRC2(wrUmmMeasCreateIntTrans)
   if(nrFreqtyp == WR_UMM_MEAS_NR_NEW_FREQ)
   {
      if (ROK != wrUmmBldInternalMsg(&internalMsg, measIntTransTyp,
               reCfgInfo))
      {
         RLOG0(L_ERROR, "creation of internal message Failed");
         RETVALUE(RFAILED);
      }
   }
   else
   {
      if (ROK != wrUmmBldInternalMsg(&internalMsg, measIntTransTyp,
               intMeasTrans))
      {
         RLOG0(L_ERROR, "creation of internal message Failed");
         RETVALUE(RFAILED);
      }
   }
   WR_ALLOC(&msg,sizeof(WrUmmIncMsg));
   if (NULLP == msg)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
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

/** @brief This function is responsible to return an EUTRA measurement object
 *    if its already existing in the ueCb->hrlMeasCfg or populate a new
 *    EUTRA measurement object and store it in ueCb->hrlMeasCfg
 *
 *  @details*
 *  Function: wrUmmHrlEutraMeasObjCfg
 *    
 *  Processing steps:
 *       - Check whether the received Cell EUTRA measurement object already
 *          exists in the hrlMeasCfg, if it exists then return that measObj.
 *       - If the measObj does not exist then allocate and populate the 
 *          measObj with data that is received in the Cell EUTRA measObj.
 *       - Add the newly created measObj into the hrlMeasCfg and return the object
 *          to caller.
 */
PRIVATE WrUmmMeasEutraMeasObjNode* wrUmmHrlEutraMeasObjCfg
(
WrUmmMeasEutraMeasObjNode    *measObj,
WrUeCb            *ueCb
 )
{
   WrUmmMeasEutraMeasObjNode  *eutraMeasObj = NULLP;

   /* Check if a measurement object with this frequency already 
    * exist in the hrlMeasCfg */
   if((ueCb->hrlMeasCfg.intraMeasObj != NULLP) && 
         (measObj->carrFreq == ueCb->hrlMeasCfg.intraMeasObj->carrFreq))
   {
      eutraMeasObj = ueCb->hrlMeasCfg.intraMeasObj;
      RETVALUE(eutraMeasObj);
   }
   else
   {
      cmLListFirst(&ueCb->hrlMeasCfg.interMeasObjLst);
      while(cmLListCrnt(&ueCb->hrlMeasCfg.interMeasObjLst))
      {
         eutraMeasObj = (WrUmmMeasEutraMeasObjNode *) cmLListNode\
                        (cmLListCrnt(&ueCb->hrlMeasCfg.interMeasObjLst));
         if(measObj->carrFreq == eutraMeasObj->carrFreq)
         {
            RETVALUE(eutraMeasObj);
         }
         cmLListNext(&ueCb->hrlMeasCfg.interMeasObjLst);
      }
   }

   WR_ALLOC(&eutraMeasObj, sizeof(WrUmmMeasEutraMeasObjNode));
   if(eutraMeasObj == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(NULLP);
   }

   eutraMeasObj->carrFreq = measObj->carrFreq;
   eutraMeasObj->allowedMeasBW = measObj->allowedMeasBW;
   eutraMeasObj->presAntPort1 = measObj->presAntPort1;
   eutraMeasObj->neighCellCfg = measObj->neighCellCfg;
   eutraMeasObj->qOffsetFreq = measObj->qOffsetFreq;
   WR_SET_ZERO(eutraMeasObj->wlCellIdxInUse, (WR_UMM_MEAS_MAX_CELL_IDX * sizeof(Bool)));
   WR_SET_ZERO(eutraMeasObj->blCellIdxInUse, (WR_UMM_MEAS_MAX_CELL_IDX * sizeof(Bool)));

   if(ueCb->measCfg->intraMeasObj.eutraMeasObj->carrFreq == 
         eutraMeasObj->carrFreq)
   {
      ueCb->hrlMeasCfg.intraMeasObj = eutraMeasObj;
   }
   else
   {
      eutraMeasObj->lnk.node = (PTR)eutraMeasObj;
      cmLListAdd2Tail(&ueCb->hrlMeasCfg.interMeasObjLst, &eutraMeasObj->lnk);
   }
   RETVALUE(eutraMeasObj);
}
/** @brief This function is responsible to return an UTRA measurement object
 *    if its already existing in the ueCb->hrlMeasCfg or populate a new
 *    UTRA measurement object and store it in ueCb->hrlMeasCfg
 *
 *  @details*
 *  Function: wrUmmHrlUtraMeasObjCfg
 *    
 *  Processing steps:
 *       - Check whether the received Cell UTRA measurement object already
 *          exists in the hrlMeasCfg, if it exists then return that measObj.
 *       - If the measObj does not exist then allocate and populate the 
 *          measObj with data that is received in the Cell UTRA measObj.
 *       - Add the newly created measObj into the hrlMeasCfg and return the object
 *          to caller.
 */
PRIVATE WrUmmMeasUtraMeasObjNode* wrUmmHrlUtraMeasObjCfg
(
 WrUmmMeasUtraMeasObjNode     *measObj,
 WrUeCb                    *ueCb
 )
{
   WrUmmMeasUtraMeasObjNode   *utraMeasObj = NULLP;

   /* Check if a measurement object with this frequency already 
    * exist in the hrlMeasCfg */
   if(measObj->duplexMode == WR_UMM_MEAS_DUPLEX_MODE_FDD)
   {
      cmLListFirst(&ueCb->hrlMeasCfg.utraFddMeasObjLst);
      while(cmLListCrnt(&ueCb->hrlMeasCfg.utraFddMeasObjLst))
      {
         utraMeasObj = (WrUmmMeasUtraMeasObjNode *) cmLListNode(cmLListCrnt(&ueCb->hrlMeasCfg.utraFddMeasObjLst));
         if(measObj->arfcn == utraMeasObj->arfcn)
         {
            RETVALUE(utraMeasObj);
         }
         cmLListNext(&ueCb->hrlMeasCfg.utraFddMeasObjLst);
      }
   }
   else
   {
      cmLListFirst(&ueCb->hrlMeasCfg.utraTddMeasObjLst);
      while(cmLListCrnt(&ueCb->hrlMeasCfg.utraTddMeasObjLst))
      {
         utraMeasObj = (WrUmmMeasUtraMeasObjNode *) cmLListNode(cmLListCrnt(&ueCb->hrlMeasCfg.utraTddMeasObjLst));
         if(measObj->arfcn == utraMeasObj->arfcn)
         {
            RETVALUE(utraMeasObj);
         }
         cmLListNext(&ueCb->hrlMeasCfg.utraTddMeasObjLst);
      }
   }

   WR_ALLOC(&utraMeasObj, sizeof(WrUmmMeasUtraMeasObjNode));
   if(utraMeasObj == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(NULLP);
   }

   utraMeasObj->arfcn = measObj->arfcn;
   utraMeasObj->qOffsetFreq = measObj->qOffsetFreq;
   utraMeasObj->duplexMode = measObj->duplexMode;
   utraMeasObj->tddMode = measObj->tddMode;
   utraMeasObj->tddBandIndicator = measObj->tddBandIndicator;
   WR_SET_ZERO(utraMeasObj->cellIdxInUse, (WR_UMM_MEAS_MAX_CELL_IDX * sizeof(Bool)));

   utraMeasObj->lnk.node = (PTR) utraMeasObj;
   if(measObj->duplexMode == WR_UMM_MEAS_DUPLEX_MODE_FDD)
   {
      cmLListAdd2Tail(&ueCb->hrlMeasCfg.utraFddMeasObjLst, &utraMeasObj->lnk);
   }
   else
   {
      cmLListAdd2Tail(&ueCb->hrlMeasCfg.utraTddMeasObjLst, &utraMeasObj->lnk);
   }

   RETVALUE(utraMeasObj);
}

/** @brief This function is responsible to return an GERAN measurement object
 *    if its already existing in the ueCb->hrlMeasCfg or populate a new
 *    GERAN measurement object and store it in ueCb->hrlMeasCfg
 *
 *  @details*
 *  Function: wrUmmHrlGeranMeasObjCfg
 *    
 *  Processing steps:
 *       - Check whether the received Cell Geran measurement object already
 *          exists in the hrlMeasCfg, if it exists then return that measObj.
 *       - If the measObj does not exist then allocate and populate the 
 *          measObj with data that is received in the Cell Geran measObj.
 *       - Add the newly created measObj into the hrlMeasCfg and return the object
 *          to caller.
 */
PRIVATE WrUmmMeasGeranMeasObjNode* wrUmmHrlGeranMeasObjCfg
(
 WrUmmMeasGeranMeasObjNode     *measObj,
 WrUeCb                        *ueCb
 )
{
   U8                         idx;
   U8                         geranCellCnt = 0;
   Bool                       flag = FALSE;
   WrUmmMeasGeranMeasObjNode   *geranMeasObj = NULLP;

   /* Currently adding if condition since for GERAN only one object exist in
    * measurment cell Cb */
   if(cmLListCrnt(&ueCb->hrlMeasCfg.geranMeasObjLst))
   {
      geranMeasObj = (WrUmmMeasGeranMeasObjNode *) \
                     cmLListNode(cmLListCrnt(&ueCb->hrlMeasCfg.geranMeasObjLst));
      RETVALUE(geranMeasObj);
   }

   WR_ALLOC(&geranMeasObj, sizeof(WrUmmMeasGeranMeasObjNode));
   if(geranMeasObj == NULLP)
   {
      RLOG0(L_ERROR, "geranMeasObj is NULLP");
      RETVALUE(NULLP);
   }
   for(idx = 0; idx < measObj->arfcnCnt; idx++)
   {
      if(TRUE == wrChkGeranBandSup(ueCb->ueCap,measObj->bandIndicator[idx]))
      {
         geranMeasObj->arfcn[geranCellCnt] = measObj->arfcn[idx]; 
         geranMeasObj->bandIndicator[geranCellCnt] = measObj->bandIndicator[idx];
         geranMeasObj->bsic[geranCellCnt] = measObj->bsic[idx];
         geranMeasObj->nccPermittedMeas |= measObj->nccPermitted[idx];
         if((FALSE == flag) &&
               (WR_GERAN_PCS1900 == measObj->bandIndicator[idx]))
         {
            geranMeasObj->isBandInd1900Pres = TRUE;
            flag = TRUE ;
         }
         geranCellCnt++;
      }
   }
   geranMeasObj->arfcnCnt = geranCellCnt;
   geranMeasObj->qOffset = measObj->qOffset; 
   if(geranCellCnt != 0)
   {
      geranMeasObj->lnk.node = (PTR) geranMeasObj;
      cmLListAdd2Tail(&ueCb->hrlMeasCfg.geranMeasObjLst, &geranMeasObj->lnk);
   }
   else
   {
      WR_FREE(geranMeasObj, sizeof(WrUmmMeasGeranMeasObjNode));
   }
   RETVALUE(geranMeasObj);
}

/** @brief This function is responsible to populate a new
 *    CDMA measurement object and store it in ueCb->hrlMeasCfg
 *
 *  @details*
 *  Function: wrUmmHrlCdmaMeasObjCfg
 *    
 *  Processing steps:
 *       - Check whether the received Cell UTRA measurement object already exists in 
 *          the hrlMeasCfg, if it exists then return without creating new obj.
 *       - If the measObj does not exist then allocate and populate the 
 *          measObj with data that is received in the Cell CDMA measObj.
 *       - Add the newly created measObj into the hrlMeasCfg and return.
 */
PRIVATE S16 wrUmmHrlCdmaMeasObjCfg
(
 WrUmmMeasCdmaMeasObjNode    *measObj,
 WrUeCb                      *ueCb
 )
{
   WrUmmMeasCdmaMeasObjNode    *cdmaMeasObj = NULLP;

   /* Check if measObj with this bandclass and arfcn already exists. */
   cmLListFirst(&ueCb->hrlMeasCfg.cdmaMeasObjLst);
   while(cmLListCrnt(&ueCb->hrlMeasCfg.cdmaMeasObjLst))
   {
      cdmaMeasObj = (WrUmmMeasCdmaMeasObjNode *) cmLListNode \
                    (cmLListCrnt(&ueCb->hrlMeasCfg.cdmaMeasObjLst));
      if(cdmaMeasObj == NULLP)
      {
         RLOG0(L_ERROR, "cdmaMeasObj is NULLP");
         RETVALUE(RFAILED);
      }
      if((cdmaMeasObj->bandClass == measObj->bandClass) &&
            (cdmaMeasObj->arfcn == measObj->arfcn))
      {
         RETVALUE(ROK);
      }
      cmLListNext(&ueCb->hrlMeasCfg.cdmaMeasObjLst);
   }

   WR_ALLOC(&cdmaMeasObj, sizeof(WrUmmMeasCdmaMeasObjNode));
   if(cdmaMeasObj == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   cdmaMeasObj->cdmaType = measObj->cdmaType;
   cdmaMeasObj->bandClass = measObj->bandClass;
   cdmaMeasObj->arfcn = measObj->arfcn;
   cdmaMeasObj->qOffsetRangeInterRAT = measObj->qOffsetRangeInterRAT;
   cdmaMeasObj->searchWindowSize = measObj->searchWindowSize;

   cdmaMeasObj->lnk.node = (PTR) cdmaMeasObj;
   cmLListAdd2Tail(&ueCb->hrlMeasCfg.cdmaMeasObjLst, &cdmaMeasObj->lnk);

   RETVALUE(ROK);
}

/** @brief This function is responsible to populate a new
 *    white list cell and add it in the received EUTRA measObj
 *    of the hrlMeasCfg
 *
 *  @details*
 *  Function: wrFillUeWhtLstHrlUe
 *    
 *  Processing steps:
 *    - Allocate and populate WrMeasNrWCellNode with the data 
 *       received in the CellCb white list cell.
 *    - Add the cell to the eutraMeasObj->wlCellLst.
 */
PRIVATE S16 wrFillUeWhtLstHrlUe
(
 WrUmmMeasEutraMeasObjNode  *hrlMeasObj,
 WrMeasNrWCellNode   *nrWCellObjNode,
 U8                  *cellIdx,
 U32                 carrFreq
 )
{
   WrMeasNrWCellNode   *wlCellNode;

   /* Check if the cell already exists */
   cmLListFirst(&hrlMeasObj->wlCellLst);
   while(cmLListCrnt(&hrlMeasObj->wlCellLst))
   {
      wlCellNode = (WrMeasNrWCellNode *) cmLListNode
         (cmLListCrnt(&hrlMeasObj->wlCellLst));
      if(wlCellNode->pci == nrWCellObjNode->pci)
      {
         RETVALUE(ROK);
      }
      cmLListNext(&hrlMeasObj->wlCellLst);
   }

   WR_ALLOC(&wlCellNode, sizeof(WrMeasNrWCellNode));
   if(wlCellNode == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrUmmMeasGetFreeCellIdxForEutra(hrlMeasObj->wlCellIdxInUse, cellIdx);
   wlCellNode->cellIndex = *cellIdx;
   wlCellNode->pci = nrWCellObjNode->pci;
   wlCellNode->cio = nrWCellObjNode->cio;

   wlCellNode->lnk.node = (PTR) wlCellNode;
   cmLListAdd2Tail(&hrlMeasObj->wlCellLst, &wlCellNode->lnk);

   RETVALUE(ROK);
}

/** @brief This function is responsible to populate a new
 *    black list cell and add it in the received EUTRA measObj
 *    of the hrlMeasCfg
 *
 *  @details*
 *  Function: wrFillUeBlkLstHrlUe
 *    
 *  Processing steps:
 *    - Allocate and populate WrMeasNrBCellNode with the data 
 *       received in the CellCb white list cell.
 *    - Add the cell to the eutraMeasObj->blCellLst.
 */
PRIVATE S16 wrFillUeBlkLstHrlUe
(
 WrUmmMeasEutraMeasObjNode  *hrlMeasObj,
 WrMeasNrWCellNode   *nrWCellObjNode,
 U8                  *cellIdx,
 U32                 carrFreq
 )
{
   WrMeasNrBCellNode   *blCellNode;

   /* Check if the cell already exists */
   cmLListFirst(&hrlMeasObj->blCellLst);
   while(cmLListCrnt(&hrlMeasObj->blCellLst))
   {
      blCellNode = (WrMeasNrBCellNode *) cmLListNode
         (cmLListCrnt(&hrlMeasObj->blCellLst));
      if(blCellNode->startPhyCellID == nrWCellObjNode->pci)
      {
         RETVALUE(ROK);
      }
      cmLListNext(&hrlMeasObj->blCellLst);
   }

   WR_ALLOC(&blCellNode, sizeof(WrMeasNrBCellNode));
   if(blCellNode == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrUmmMeasGetFreeCellIdxForEutra(hrlMeasObj->blCellIdxInUse, cellIdx);
   blCellNode->cellIndex = *cellIdx;
   blCellNode->startPhyCellID = nrWCellObjNode->pci;
   blCellNode->phyCellIdRange = 0;

   blCellNode->lnk.node = (PTR) blCellNode;
   cmLListAdd2Tail(&hrlMeasObj->blCellLst, &blCellNode->lnk);

   RETVALUE(ROK);
}


/** @brief This function is responsible to populate a new
 *    black list cell based onb the Cell level black list 
 *    and add it in the received EUTRA measObj
 *    of the hrlMeasCfg
 *
 *  @details*
 *  Function: wrFillHrlUeBlkLstFromEutraBLst
 *    
 *  Processing steps:
 *    - Allocate and populate WrMeasNrBCellNode with the data 
 *       received in the CellCb black list cell.
 *    - Add the cell to the eutraMeasObj->blCellLst.
 */
PRIVATE S16 wrFillHrlUeBlkLstFromEutraBLst 
(
 WrUmmMeasEutraMeasObjNode  *hrlMeasObj,
 WrMeasNrBCellNode          *nrBCellObjNode,
 U8                         *cellIdx,
 U32                         carrFreq
 )
{
   WrMeasNrBCellNode   *blCellNode;

   /* Check if the cell already exists */
   cmLListFirst(&hrlMeasObj->blCellLst);
   while(cmLListCrnt(&hrlMeasObj->blCellLst))
   {
      blCellNode = (WrMeasNrBCellNode *) cmLListNode
         (cmLListCrnt(&hrlMeasObj->blCellLst));

      if(blCellNode->startPhyCellID == nrBCellObjNode->startPhyCellID)
      {
         RETVALUE(ROK);
      }
      cmLListNext(&hrlMeasObj->blCellLst);
   }

   WR_ALLOC(&blCellNode, sizeof(WrMeasNrBCellNode));
   if(blCellNode == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrUmmMeasGetFreeCellIdxForEutra(hrlMeasObj->blCellIdxInUse, cellIdx);
   blCellNode->cellIndex = *cellIdx;
   blCellNode->startPhyCellID = nrBCellObjNode->startPhyCellID;
   blCellNode->phyCellIdRange = nrBCellObjNode->phyCellIdRange;

   blCellNode->lnk.node = (PTR) blCellNode;
   cmLListAdd2Tail(&hrlMeasObj->blCellLst, &blCellNode->lnk);

   RETVALUE(ROK);
}
/** @brief This function is responsible to populate a new
 *    UTRA listed cell and add it in the received UTRA measObj
 *    of the hrlMeasCfg
 *
 *  @details*
 *  Function: wrFillUeUtraCellLstHrlUe
 *    
 *  Processing steps:
 *    - Allocate and populate WrMeasNrUtraCellNode with the data 
 *       received in the CellCb UTRA listed cell.
 *    - Add the cell to the utraMeasObj->cellLst.
 */
PRIVATE S16 wrFillUeUtraCellLstHrlUe
(
 WrUmmMeasUtraMeasObjNode  *hrlMeasObj,
 WrMeasNrUtraCellNode   *nrCellObjNode,
 U8                  *cellIdx,
 U32                 arfcn
 )
{
   WrMeasNrUtraCellNode   *cellNode = NULLP;

   /* Check if the cell already exists */
   cmLListFirst(&hrlMeasObj->cellLst);
   while(cmLListCrnt(&hrlMeasObj->cellLst))
   {
      cellNode = (WrMeasNrUtraCellNode *) cmLListNode
         (cmLListCrnt(&hrlMeasObj->cellLst));
      if(cellNode->psc == nrCellObjNode->psc)
      {
         RETVALUE(ROK);
      }
      cmLListNext(&hrlMeasObj->cellLst);
   }

   WR_ALLOC(&cellNode, sizeof(WrMeasNrUtraCellNode));
   if(cellNode == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrUmmMeasGetFreeCellIdxForEutra(hrlMeasObj->cellIdxInUse, cellIdx);
   cellNode->cellIndex = *cellIdx;
   cellNode->psc = nrCellObjNode->psc;

   cellNode->lnk.node = (PTR) cellNode;
   cmLListAdd2Tail(&hrlMeasObj->cellLst, &cellNode->lnk);

   RETVALUE(ROK);
}

/** This function will free the measId and measObjId
 *    from the measUeCb, if it has to be removed based 
 *    on the HRL.
 * */
PRIVATE S16 wrUmmHrlFreeMeasIdfrmUeCb
(
 U8      measObjId,
 WrMeasUeCb   *measUeCb
 )
{
   WrMeasIdNode              *measIdNode;

   cmLListFirst(&measUeCb->measIdLst);
   while(cmLListCrnt(&measUeCb->measIdLst))
   {
      measIdNode = (WrMeasIdNode *) cmLListNode \
          (cmLListCrnt(&measUeCb->measIdLst));         
      if(measIdNode->measObjID == measObjId)
      {
         wrUmmMeasSetFreeMeasId(measUeCb, measIdNode->measID);
         cmLListDelFrm(&measUeCb->measIdLst, &measIdNode->lnk);
         WR_FREE(measIdNode, sizeof(WrMeasIdNode));
      }
      cmLListNext(&measUeCb->measIdLst);
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible to populate an GERAN measurement object
 *          based on the HRL of the UE and returns TRUE if this GERAN measurement
 *          object has to be configured for th UE.
 *
 *  @details*
 *  Function: wrUmmHrlUeGeranMeasCfg
 *    
 *  Processing steps:
 *       - Populate the hrlMeasObj based on the GERAN
 *          measObj received.
 *       - Populate the GERAN Cells in the hrlMeasObj
 *          based on the HRL of the UE.
 */
PRIVATE Bool wrUmmHrlUeGeranMeasCfg
(
 WrUmmMeasGeranMeasObjNode     *geranMeasObj,
 WrUeCb                        *ueCb
 )
{
   WrGeranCellCb               *neighCellCb = NULLP;
   Bool                        flag = FALSE;
   Bool                        isBandInd1900Pres = FALSE;
   WrHrlDecisionRet           hrlDec = WR_MEAS_MBLTY_APPL_TO_CELL;
   U8                         geranCellCnt = 0;
   U8                         idx = 0;
   U8                         nccPermittedMeas = 0; 
   WrUmmMeasGeranMeasInfo     geranMeasInfo[WR_UMM_MEAS_MAX_GERAN_ARFCN];

   while(idx < geranMeasObj->arfcnCnt)
   {
      neighCellCb = (WrGeranCellCb *) wrEmmGetNeighGeranCell\
                    (ueCb->cellId, geranMeasObj->arfcn[idx], geranMeasObj->bsic[idx]);
      if(neighCellCb == NULLP)
      {
         RLOG0(L_ERROR, "Geran neighCellCb is NULLP");
         RETVALUE(RFAILED);
      }
      /* Check for the mobility restriction by calling the HRL algo. */
      hrlDec = wrMbltyRestrc(WR_INTER_SYS_GERAN_MBLTY, (PTR *)neighCellCb, \
            NULLP, NULLP, NULLP, ueCb->hoRstrctInfo);
      if(hrlDec == WR_MEAS_MBLTY_APPL_TO_CELL)
      {
         /* This cell is not forbidden for UE. So add it in listed cells. 
          * And update the listed cell info in measUeCb. */
         geranMeasInfo[geranCellCnt].arfcn = neighCellCb->freqCb->arfcn; 
         geranMeasInfo[geranCellCnt].bandIndicator = neighCellCb->freqCb->bandInd;
         nccPermittedMeas |= neighCellCb->nccPermittedMeas;
         if((FALSE == flag) &&
               (WR_GERAN_PCS1900 == neighCellCb->freqCb->bandInd))
         {
            isBandInd1900Pres = TRUE;
            flag = TRUE ;
         }
         geranCellCnt++;
      }
      idx++;
   }
   if(geranCellCnt != 0)
   {
      geranMeasObj->arfcnCnt = geranCellCnt;
      for(idx = 0; idx < geranCellCnt; idx++)
      {
         geranMeasObj->arfcn[idx] = geranMeasInfo[idx].arfcn;
         geranMeasObj->bandIndicator[idx] = geranMeasInfo[idx].bandIndicator;
      }
      geranMeasObj->nccPermittedMeas = nccPermittedMeas;
      geranMeasObj->isBandInd1900Pres = isBandInd1900Pres;
      RETVALUE(TRUE);
   }
   else
   {
      WR_FREE(geranMeasObj, sizeof(WrUmmMeasGeranMeasObjNode));
      cmLListInit(&ueCb->hrlMeasCfg.geranMeasObjLst); 
      RETVALUE(FALSE);
   }
}
/** @brief This function is responsible to populate an UTRA measurement object
 *          based on the HRL of the UE and returns TRUE if this UTRA measurement
 *          object has to be configured for th UE.
 *
 *  @details*
 *  Function: wrUmmHrlUeUtraMeasCfg
 *    
 *  Processing steps:
 *       - Populate the hrlMeasObj based on the UTRA 
 *          measObj received.
 *       - Populate the UTRA Cells in the hrlMeasObj
 *          based on the HRL of the UE.
 */
PRIVATE Bool wrUmmHrlUeUtraMeasCfg
(
 WrUmmMeasUtraMeasObjNode     *utraMeasObj,
 WrUeCb                    *ueCb
 )
{
   WrUmmMeasUtraMeasObjNode   *hrlMeasObj = NULLP;
   WrUtraNeighCellCb          *neighCellCb = NULLP;
   WrMeasNrUtraCellNode       *nrWCellObjNode = NULLP;
   WrHrlDecisionRet           hrlDec = WR_MEAS_MBLTY_APPL_TO_CELL;
   U8                         cellIdx = 0;

   /* Populate the WrHrlMeasCfg. */
   hrlMeasObj = wrUmmHrlUtraMeasObjCfg(utraMeasObj, ueCb);
   if(hrlMeasObj == NULLP)
   {
      RLOG0(L_ERROR, "hrlMeasObj is NULLP");
      RETVALUE(FALSE);
   }
   cmLListFirst(&utraMeasObj->cellLst);
   while(cmLListCrnt(&utraMeasObj->cellLst))
   {
      nrWCellObjNode = (WrMeasNrUtraCellNode *) cmLListNode\
                       (cmLListCrnt(&utraMeasObj->cellLst));

      neighCellCb = (WrUtraNeighCellCb *) wrEmmGetNeighUtraCell\
                    (ueCb->cellId, utraMeasObj->arfcn, nrWCellObjNode->psc, utraMeasObj->duplexMode);
      if(neighCellCb == NULLP)
      {
         RLOG0(L_ERROR, "neighCellCb is NULLP");
         RETVALUE(RFAILED);
      }
      /* Check for the mobility restriction by calling the HRL algo. */
      hrlDec = wrMbltyRestrc(WR_INTER_SYS_UTRA_MBLTY, (PTR *)neighCellCb, \
            NULLP, NULLP, NULLP, ueCb->hoRstrctInfo);
      if(hrlDec == WR_MEAS_MBLTY_APPL_TO_CELL)
      {
         /* This cell is not forbidden for UE. So add it in listed cells. 
          * And update the listed cell info in measUeCb. */
         wrFillUeUtraCellLstHrlUe(hrlMeasObj, nrWCellObjNode, &cellIdx, utraMeasObj->arfcn);
      }
      cmLListNext(&utraMeasObj->cellLst);
   }
   if(hrlMeasObj->cellLst.count == 0)
   {
      RETVALUE(FALSE);
   }
   else
   {
      RETVALUE(TRUE);
   }
}

/** @brief This function is responsible for determining whether the newly
 * detected EUTRA cell though ANR should be added as white listed cell or 
 * black listed cell into Nhu measurement Object.
 *
 *  @details*
 *  Function: wrUmmUpdtHrlMeasCfg
 *    
 *  Processing steps:
 *      - Populate the WrMeasEutraNrUpdtIdNode which will be used to send MeasReCfg to UE.
 *      - Update the ueCb->hrlMeasCfg by either adding this cell to white list or black list.
 *      - The updated WrMeasEutraNrUpdtIdNode will be used by the caller for populating intMeasTrans.
 */
PRIVATE WrUmmMeasEutraMeasObjNode*  wrUmmUpdtHrlMeasCfg
(
 WrUeCb                       *ueCb,
 WrMeasNrWCellNode          *nrCellNode,
 WrUmmMeasEutraMeasObjNode  *eutraMeasObj,
 WrMeasEutraNrUpdtIdNode    *nrUpdtId
 )
{
   WrHrlDecisionRet      hrlDec = WR_MEAS_MBLTY_APPL_TO_CELL;
   U8                    cellIdx = 0;
   WrEutranNeighCellCb  *neighCellCb;
   WrUmmMeasEutraMeasObjNode  *hrlMeasObj;

   /* Populate in the hrlMeasCfg */
   hrlMeasObj = wrUmmHrlEutraMeasObjCfg(eutraMeasObj, ueCb);

   neighCellCb = wrEmmFindEutranNeighCell(ueCb->cellId, \
         eutraMeasObj->carrFreq, nrCellNode->pci);
   if((neighCellCb == NULLP) || (hrlMeasObj == NULLP))
   {
      RLOG0(L_ERROR, "neighCellCb or hrlMeasObj is NULLP");
      RETVALUE(NULLP);
   }

   hrlDec = wrMbltyRestrc(WR_INTRA_SYS_MEAS, (PTR *)neighCellCb, \
         NULLP, NULLP, NULLP, ueCb->hoRstrctInfo);
   if(hrlDec == WR_MEAS_MBLTY_APPL_TO_CELL)
   {
      wrFillUeWhtLstHrlUe(hrlMeasObj, nrCellNode, &cellIdx, eutraMeasObj->carrFreq);
      if ( 0 != cellIdx )
      {
         nrUpdtId->addWCellLst[nrUpdtId->addWCellCnt++] = cellIdx;
      }
   }
   else
   {
      wrFillUeBlkLstHrlUe(hrlMeasObj, nrCellNode, &cellIdx, eutraMeasObj->carrFreq);
      if ( 0 != cellIdx )
      {
         nrUpdtId->addBCellLst[nrUpdtId->addBCellCnt++] = cellIdx;
      }
   }
   RETVALUE(hrlMeasObj);
}


/** @brief This function is responsible for freeing all the memory allocated
 *    to the white cell list within the eutraMeasObj of the ueCb->hrlMeasCfg.
 *
 *  @details*
 *  Function: wrFreeUeHrlWlLst
 *    
 *  Processing steps:
 *      - Access the cells from the white list of the received eutraMeasObj 
 *      - Free each of the accessed white listed cells.
 */
PRIVATE S16 wrFreeUeHrlWlLst
(
 WrUmmMeasEutraMeasObjNode     *eutraMeasObj
 )
{
   WrMeasNrWCellNode             *wlCellNode = NULLP;

   cmLListFirst(&eutraMeasObj->wlCellLst);
   while(cmLListCrnt(&eutraMeasObj->wlCellLst))
   {
      wlCellNode = (WrMeasNrWCellNode*) cmLListNode(cmLListCrnt(&eutraMeasObj->wlCellLst));
      cmLListDelFrm(&eutraMeasObj->wlCellLst, &wlCellNode->lnk);
      WR_FREE(wlCellNode, sizeof(WrMeasNrWCellNode));
      cmLListNext(&eutraMeasObj->wlCellLst);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for freeing all the memory allocated
 *    to the black cell list within the eutraMeasObj of the ueCb->hrlMeasCfg.
 *
 *  @details*
 *  Function: wrFreeUeHrlBlLst
 *    
 *  Processing steps:
 *      - Access the cells from the black list of the received eutraMeasObj 
 *      - Free each of the accessed black listed cells.
 */
PRIVATE S16 wrFreeUeHrlBlLst
(
 WrUmmMeasEutraMeasObjNode     *eutraMeasObj
 )
{
   WrMeasNrBCellNode          *blCellNode = NULLP;

   cmLListFirst(&eutraMeasObj->blCellLst);
   while(cmLListCrnt(&eutraMeasObj->blCellLst))
   {
      blCellNode = (WrMeasNrBCellNode *) cmLListNode(cmLListCrnt(&eutraMeasObj->blCellLst));
      cmLListDelFrm(&eutraMeasObj->blCellLst, &blCellNode->lnk);
      WR_FREE(blCellNode, sizeof(WrMeasNrBCellNode));
      cmLListNext(&eutraMeasObj->blCellLst);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for freeing all the memory allocated
 *    to the listed cell list within the utraMeasObj of the ueCb->hrlMeasCfg.
 *
 *  @details*
 *  Function: wrFreeUeHrlUtraCellLst
 *    
 *  Processing steps:
 *      - Access the cells from the listed cells of the received utraMeasObj 
 *      - Free each of the accessed listed cells.
 */
PRIVATE S16 wrFreeUeHrlUtraCellLst
(
 WrUmmMeasUtraMeasObjNode      *utraMeasObj 
 )
{
   WrMeasNrUtraCellNode       *utraCellNode = NULLP;

   cmLListFirst(&utraMeasObj->cellLst);
   while(cmLListCrnt(&utraMeasObj->cellLst))
   {
      utraCellNode = (WrMeasNrUtraCellNode *) cmLListNode(cmLListCrnt(&utraMeasObj->cellLst));
      cmLListDelFrm(&utraMeasObj->cellLst, &utraCellNode->lnk);
      WR_FREE(utraCellNode, sizeof(WrMeasNrUtraCellNode));
      cmLListNext(&utraMeasObj->cellLst);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for freeing all the memory allocated
 *    to the eutra/utra/cdma measurement objects stored in the ueCb->hrlMeasCfg.
 *    This is called when the ueCb is being deleted.
 *
 *  @details*
 *  Function: wrFreeUeHrlMeasCfg
 *    
 *  Processing steps:
 *      - Access the eutra, utra and cdma measurement objects. 
 *      - Free each of the accessed measurement objects.
 */
PUBLIC Void wrFreeUeHrlMeasCfg
(
 WrUeCb     *ueCb
 )
{
   WrUmmMeasEutraMeasObjNode     *eutraMeasObj = NULLP;
   WrUmmMeasUtraMeasObjNode      *utraMeasObj  = NULLP;
   WrUmmMeasCdmaMeasObjNode      *cdmaMeasObj = NULLP;

   eutraMeasObj = ueCb->hrlMeasCfg.intraMeasObj;
   if(eutraMeasObj != NULLP)
   {
      wrFreeUeHrlWlLst(eutraMeasObj);
      wrFreeUeHrlBlLst(eutraMeasObj);
      WR_FREE(eutraMeasObj, sizeof(WrUmmMeasEutraMeasObjNode));
   }

   cmLListFirst(&ueCb->hrlMeasCfg.interMeasObjLst);
   while(cmLListCrnt(&ueCb->hrlMeasCfg.interMeasObjLst))
   {
      eutraMeasObj = (WrUmmMeasEutraMeasObjNode *) cmLListNode \
                     (cmLListCrnt(&ueCb->hrlMeasCfg.interMeasObjLst));
      wrFreeUeHrlWlLst(eutraMeasObj);
      wrFreeUeHrlBlLst(eutraMeasObj);
      cmLListDelFrm(&ueCb->hrlMeasCfg.interMeasObjLst, &eutraMeasObj->lnk);
      cmLListNext(&ueCb->hrlMeasCfg.interMeasObjLst);
      WR_FREE(eutraMeasObj, sizeof(WrUmmMeasEutraMeasObjNode));
   }

   cmLListFirst(&ueCb->hrlMeasCfg.utraFddMeasObjLst);   
   while(cmLListCrnt(&ueCb->hrlMeasCfg.utraFddMeasObjLst))
   {
      utraMeasObj = (WrUmmMeasUtraMeasObjNode *) cmLListNode \
                    (cmLListCrnt(&ueCb->hrlMeasCfg.utraFddMeasObjLst));
      wrFreeUeHrlUtraCellLst(utraMeasObj);
      cmLListDelFrm(&ueCb->hrlMeasCfg.utraFddMeasObjLst, &utraMeasObj->lnk);
      cmLListNext(&ueCb->hrlMeasCfg.utraFddMeasObjLst);
      WR_FREE(utraMeasObj, sizeof(WrUmmMeasUtraMeasObjNode));
   }

   cmLListFirst(&ueCb->hrlMeasCfg.utraTddMeasObjLst);   
   while(cmLListCrnt(&ueCb->hrlMeasCfg.utraTddMeasObjLst))
   {
      utraMeasObj = (WrUmmMeasUtraMeasObjNode *) cmLListNode \
                    (cmLListCrnt(&ueCb->hrlMeasCfg.utraTddMeasObjLst));
      wrFreeUeHrlUtraCellLst(utraMeasObj);
      cmLListDelFrm(&ueCb->hrlMeasCfg.utraTddMeasObjLst, &utraMeasObj->lnk);
      cmLListNext(&ueCb->hrlMeasCfg.utraTddMeasObjLst);
      WR_FREE(utraMeasObj, sizeof(WrUmmMeasUtraMeasObjNode));
   }

   cmLListFirst(&ueCb->hrlMeasCfg.cdmaMeasObjLst);   
   while(cmLListCrnt(&ueCb->hrlMeasCfg.cdmaMeasObjLst))
   {
      cdmaMeasObj = (WrUmmMeasCdmaMeasObjNode *) cmLListNode \
                    (cmLListCrnt(&ueCb->hrlMeasCfg.cdmaMeasObjLst));
      cmLListDelFrm(&ueCb->hrlMeasCfg.cdmaMeasObjLst, &cdmaMeasObj->lnk);
      cmLListNext(&ueCb->hrlMeasCfg.cdmaMeasObjLst);
      WR_FREE(cdmaMeasObj, sizeof(WrUmmMeasCdmaMeasObjNode));
   }

}

/** @brief This function is responsible for deleting a listed cell
 *       from the utraMeasObj's listed cell.
 *
 *  @details*
 *  Function: wrDelUeUtraLstHrlUe
 *    
 *  Processing steps:
 *      - Access the cells from the received UTRA measObj. 
 *      - Compare the psc of the accessed cell with the psc 
 *          of the received cell.
 *      - If the psc matches then delete the cell frm the 
 *          received UTRA measObj and free its memory.
 */
PRIVATE S16 wrDelUeUtraLstHrlUe
(
 WrUmmMeasUtraMeasObjNode     *utraUeMeasObj,
 WrMeasNrUtraCellNode         *ueLstNode
 )
{
   WrMeasNrUtraCellNode         *cellNode = NULLP;

   cmLListFirst(&utraUeMeasObj->cellLst);
   while(cmLListCrnt(&utraUeMeasObj->cellLst))
   {
      cellNode = (WrMeasNrUtraCellNode *) cmLListNode(cmLListCrnt(&utraUeMeasObj->cellLst));
      if(cellNode->psc == ueLstNode->psc)
      {
         cmLListDelFrm(&utraUeMeasObj->cellLst, &cellNode->lnk);
         WR_FREE(cellNode, sizeof(WrMeasNrUtraCellNode));
         RETVALUE(ROK);
      }
      cmLListNext(&utraUeMeasObj->cellLst);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for deleting a white list 
 *       cell from the eutraMeasObj's white cell list.
 *
 *  @details*
 *  Function: wrDelUeWhtLstHrlUe
 *    
 *  Processing steps:
 *      - Access the cells from the received EUTRA measObj's white list. 
 *      - Compare the pci of the accessed cell with the pci
 *          of the received cell.
 *      - If the pci matches then delete the cell frm the 
 *          received EUTRA measObj's white list and free its memory.
 */
PRIVATE S16 wrDelUeWhtLstHrlUe
(
 WrUmmMeasEutraMeasObjNode    *eutraUeMeasObj,
 WrMeasNrWCellNode             *ueWLstNode
 )
{
   WrMeasNrWCellNode             *cellNode = NULLP;

   cmLListFirst(&eutraUeMeasObj->wlCellLst);
   while(cmLListCrnt(&eutraUeMeasObj->wlCellLst))
   {
      cellNode = (WrMeasNrWCellNode *) cmLListNode(cmLListCrnt(&eutraUeMeasObj->wlCellLst));
      if(cellNode->pci == ueWLstNode->pci)
      {
         cmLListDelFrm(&eutraUeMeasObj->wlCellLst, &cellNode->lnk);
         WR_FREE(cellNode, sizeof(WrMeasNrWCellNode));
         RETVALUE(ROK);
      }
      cmLListNext(&eutraUeMeasObj->wlCellLst);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for deleting a black list 
 *       cell from the eutraMeasObj's black cell list.
 *
 *  @details*
 *  Function: wrDelUeBlkLstHrlUe
 *    
 *  Processing steps:
 *      - Access the cells from the received EUTRA measObj's black list. 
 *      - Compare the pci of the accessed cell with the pci
 *          of the received cell.
 *      - If the pci matches then delete the cell frm the 
 *          received EUTRA measObj's black list and free its memory.
 */
PRIVATE S16 wrDelUeBlkLstHrlUe
(
 WrUmmMeasEutraMeasObjNode    *eutraUeMeasObj,
 WrMeasNrBCellNode             *ueBLstNode
 )
{
   WrMeasNrBCellNode       *cellNode = NULLP;

   cmLListFirst(&eutraUeMeasObj->blCellLst); 
   while(cmLListCrnt(&eutraUeMeasObj->blCellLst))
   {
      cellNode = (WrMeasNrBCellNode *) cmLListNode \
                 (cmLListCrnt(&eutraUeMeasObj->blCellLst));
      if(cellNode->startPhyCellID == ueBLstNode->startPhyCellID)
      {
         cmLListDelFrm (&eutraUeMeasObj->blCellLst, &cellNode->lnk);         
         WR_FREE(cellNode, sizeof(WrMeasNrBCellNode));
         RETVALUE(ROK);
      }
      cmLListNext(&eutraUeMeasObj->blCellLst);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for configuring the UE with
 *       a new UTRA measobj, if the measObj was not configured to the 
 *       UE previously based on HRL as it consisted of only forbidden cells.
 *
 *  @details*
 *  Function: wrUmmUtraDynUeMeasObjCfg
 *    
 *  Processing steps:
 *    - Update the WrMeasUtraNrUpdtIdNode received with arfcn, utraMeasObj and cells
 *       to be listed.
 *    - Access each cell in the received CellCb UTRA measObj and check it for HRL.
 *    - Populate the utraUeMeasObj with all the info.
 */
PRIVATE S16 wrUmmUtraDynUeMeasObjCfg
(
 WrUmmMeasUtraMeasObjNode     *utraCellMeasObj,
 WrUmmMeasUtraMeasObjNode     *utraUeMeasObj,
 WrUeCb                       *ueCb,
 WrMeasUtraNrUpdtIdNode       *utraNode,
 U8                           *measObjId
 )
{
   WrHrlDecisionRet hrlDec = WR_MEAS_MBLTY_APPL_TO_CELL;
   U8          cellIdx = 0;
   WrUtraNeighCellCb  *neighCellCb;
   WrMeasNrUtraCellNode         *nrWCellObjNode;
//   CmLListCp                 *utraObjLst = &(ueCb->measCfg->utraFddMeasObjLst);
//   WrUmmMeasUtraMeasObjNode  *nrCellObjNode;

   utraNode->arfcn = utraCellMeasObj->arfcn;

   utraNode->cellCbMeasObj = utraUeMeasObj;

   cmLListFirst(&utraCellMeasObj->cellLst);
   while(cmLListCrnt(&utraCellMeasObj->cellLst))
   {
      nrWCellObjNode = (WrMeasNrUtraCellNode *) cmLListNode\
                       (cmLListCrnt(&utraCellMeasObj->cellLst));
      neighCellCb = (WrUtraNeighCellCb *) wrEmmGetNeighUtraCell\
                    (ueCb->cellId, utraCellMeasObj->arfcn, nrWCellObjNode->psc,utraCellMeasObj->duplexMode);
      if(neighCellCb == NULLP)
      {
         RLOG0(L_ERROR, "neighCellCb is NULLP");
         RETVALUE(RFAILED);
      }
      /* Check for the mobility restriction by calling the HRL algo. */
      hrlDec = wrMbltyRestrc(WR_INTER_SYS_UTRA_MBLTY, (PTR *)neighCellCb, \
            NULLP, NULLP, NULLP, ueCb->hoRstrctInfo);
      if(hrlDec == WR_MEAS_MBLTY_APPL_TO_CELL)
      {
         /* This cell is not forbidden for UE. So add it in listed cells. 
          * And update the listed cell info in measUeCb. */
         wrFillUeUtraCellLstHrlUe(utraUeMeasObj, nrWCellObjNode, &cellIdx, utraCellMeasObj->arfcn);
         utraNode->addCellLst[utraNode->addCellCnt++] = cellIdx;
      }
      cmLListNext(&utraCellMeasObj->cellLst);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for validating the previously
 *    configured UTRA measCfg with the newly received HRL and reconfigure 
 *    the UE with a new measCfg based on the updated HRL.
 *
 *  @details*
 *  Function: wrUmmUtraDynUeMeasCfg
 *    
 *  Processing steps:
 *    - For each cell in the Cell UTRA measObj, check whether that cell has to be 
 *       listed or not based on the new HRL.
 *    - If in the previous UTRA measCfg the accessed cell is listed but as per the 
 *       new HRL it should not be listed, then remove the cell only from the 
 *       ueCb->hrlMeasCfg's UTRA listed cells and update the WrUmmIntMeasTrans.
 *    - If in the previous UTRA measCfg the accessed cell is not listed but as per 
 *       the new HRL it should be listed, then add the cell in the 
 *       ueCb->hrlMeasCfg's UTRA listed cells and update the WrUmmIntMeasTrans.
 */
PRIVATE WrHrlDecisionRet wrUmmUtraDynUeMeasCfg
(
 WrUmmMeasUtraMeasObjNode     *utraCellMeasObj,
 WrUmmMeasUtraMeasObjNode     *utraUeMeasObj,
 WrUeCb                       *ueCb,
 WrMeasUtraNrUpdtIdNode       *utraNode,
 WrUmmIntMeasTrans            *nrUpdtNode
 )
{
   WrHrlDecisionRet  hrlDec = WR_MEAS_MBLTY_APPL_TO_CELL;
   U8       cellIdx = 0;
   Bool     cellFnd = FALSE;
   WrUtraNeighCellCb            *utraNeighCellCb = NULLP;
   WrMeasNrUtraCellNode         *cellLstNode = NULLP;
   WrMeasNrUtraCellNode         *ueLstNode = NULLP;

   /* Compare each entry of the cell's listed cell with the UE's listed cell,
    * based on the new HRL update the wrUmmIntMeasTrans and ueCb->hrlMeasCfg. */

   utraNode->arfcn = utraCellMeasObj->arfcn;

   utraNode->cellCbMeasObj = utraUeMeasObj;

   cmLListFirst(&utraCellMeasObj->cellLst);
   while(cmLListCrnt(&utraCellMeasObj->cellLst))
   {
      cellLstNode = (WrMeasNrUtraCellNode *) cmLListNode(cmLListCrnt(&utraCellMeasObj->cellLst));
      utraNeighCellCb = (WrUtraNeighCellCb *) wrEmmGetNeighUtraCell\
                        (ueCb->cellId, utraCellMeasObj->arfcn, cellLstNode->psc, utraCellMeasObj->duplexMode);
      if(utraNeighCellCb == NULLP)
      {
         RLOG0(L_ERROR, "utraNeighCellCb is NULLP");
         RETVALUE(RFAILED);
      }
      /* Check for the mobility restriction by calling the HRL algo. */
      hrlDec = wrMbltyRestrc(WR_INTER_SYS_UTRA_MBLTY, (PTR *)utraNeighCellCb, \
            NULLP, NULLP, NULLP, ueCb->hoRstrctInfo);
      if(hrlDec == WR_MEAS_MBLTY_NOT_APPL_TO_RAT)
      {
         /* Delete this cells from ueCb->hrlMeasCfg. */
         wrDelUeUtraLstHrlUe(utraUeMeasObj, cellLstNode);
         cmLListNext(&utraCellMeasObj->cellLst);
         continue;
      }
      if(hrlDec == WR_MEAS_MBLTY_APPL_TO_CELL)
      {
         /* Cell is allowed, check whether the cell is there in UE's
          * hrlMeasCfg as listed cell. */
         cmLListFirst(&utraUeMeasObj->cellLst);
         while(cmLListCrnt(&utraUeMeasObj->cellLst))
         {
            ueLstNode = (WrMeasNrUtraCellNode *) cmLListNode(cmLListCrnt(&utraUeMeasObj->cellLst));
            if(ueLstNode->psc == cellLstNode->psc)
            {
               /* This cell is already listed, so no changes for this cell*/
               cellFnd = TRUE;
               break;
            }
            cmLListNext(&utraUeMeasObj->cellLst);            
         }
         if(cellFnd == TRUE)
         {
            cmLListNext(&utraCellMeasObj->cellLst);
            cellFnd  = FALSE;
            continue;
         }
         else
         {
            /* Cell is not listed to the UE, so add it to listed cells. */
            wrFillUeUtraCellLstHrlUe(utraUeMeasObj, cellLstNode, &cellIdx, utraCellMeasObj->arfcn);
            utraNode->addCellLst[utraNode->addCellCnt++] = cellIdx;
         }
      }
      else
      {
         /* Cell is not allowed, check whether the cell is not there
          * in UE's hrlMeasCfg */
         cmLListFirst(&utraUeMeasObj->cellLst);
         while(cmLListCrnt(&utraUeMeasObj->cellLst))
         {
            ueLstNode = (WrMeasNrUtraCellNode *) cmLListNode(cmLListCrnt(&utraUeMeasObj->cellLst));
            if(ueLstNode->psc == cellLstNode->psc)
            {
               cellFnd = TRUE;
               break;
            }
            cmLListNext(&utraUeMeasObj->cellLst);            
         }
         if(cellFnd == FALSE)
         {
            /* This cell is not listed, so no changes for this cell*/
            cmLListNext(&utraCellMeasObj->cellLst);
            continue;
         }
         else
         {
            /* Cell is listed to the UE, so delete from the listed cells. */
            cellFnd = FALSE;
            wrDelUeUtraLstHrlUe(utraUeMeasObj, ueLstNode);
            utraUeMeasObj->cellIdxInUse[ueLstNode->cellIndex - 1] = FALSE;
            utraNode->delCellLst[utraNode->delCellCnt++] = ueLstNode->cellIndex;
            if(utraUeMeasObj->cellLst.count == 0)
            {
               /* All the previously listed cells are deleted, 
                * so delete the measObj of this arfcn */
               nrUpdtNode->utraNodeDel[nrUpdtNode->numUtraDel++] = utraUeMeasObj->arfcn;
               /* Since the measObj is getting deleted, no need to send the
                * cellsToRemoveList, hence update the delCellCnt=0 for this */ 
               utraNode->delCellCnt = 0;
            }
         }
      }
      cmLListNext(&utraCellMeasObj->cellLst);
   }
   if((utraNode->addCellCnt != 0) || 
         (utraNode->delCellCnt != 0))
   {
      nrUpdtNode->numUtra++;
   }
   RETVALUE(hrlDec);
}

/** @brief This function is responsible for validating the previously
 *    configured EUTRA measCfg with the newly received HRL and reconfigure 
 *    the UE with a new measCfg based on the updated HRL.
 *
 *  @details*
 *  Function: wrUmmEutraDynUeMeasCfg
 *    
 *  Processing steps:
 *    - For each cell in the Cell EUTRA measObj, check whether that cell has to be 
 *       white listed or black listed based on the new HRL.
 *    - If in the previous EUTRA measCfg the accessed cell is white listed but as per 
 *       the new HRL it should be black listed, then remove the cell only from the 
 *       ueCb->hrlMeasCfg's EUTRA white list and add it in the ueCb->hrlMeasCfg's
 *       black list and update the WrUmmIntMeasTrans.
 *    - If in the previous EUTRA measCfg the accessed cell is black listed but as per 
 *       the new HRL it should be white listed, then remove the cell only from the 
 *       ueCb->hrlMeasCfg's EUTRA black list and add it in the ueCb->hrlMeasCfg's
 *       white list and update the WrUmmIntMeasTrans.
 */
PRIVATE S16 wrUmmEutraDynUeMeasCfg
(
 WrUmmMeasEutraMeasObjNode    *eutraCellMeasObj,
 WrUmmMeasEutraMeasObjNode    *eutraUeMeasObj,
 WrUeCb                       *ueCb,
 WrMeasEutraNrUpdtIdNode      *eutraNode,
 WrUmmIntMeasTrans            *nrUpdtNode
 )
{
   WrHrlDecisionRet  hrlDec = WR_MEAS_MBLTY_APPL_TO_CELL;
   U8       cellIdx = 0;
   Bool     cellFnd = FALSE;
   WrEutranNeighCellCb           *eutraNeighCellCb = NULLP;
   WrMeasNrWCellNode             *cellWLstNode = NULLP;
   WrMeasNrWCellNode             *ueWLstNode = NULLP;
   WrMeasNrBCellNode             *ueBLstNode = NULLP;

   /* Compare each entry of the cell's white list with the UE's white and black list,
    * based on the new HRL update the WrUmmIntMeasTrans and ueCb->hrlMeasCfg. */

   eutraNode->arfcn = eutraCellMeasObj->carrFreq;

   eutraNode->cellCbMeasObj = eutraUeMeasObj;

   cmLListFirst(&eutraCellMeasObj->wlCellLst);
   while(cmLListCrnt(&eutraCellMeasObj->wlCellLst))
   {
      cellWLstNode = (WrMeasNrWCellNode *) cmLListNode \
                     (cmLListCrnt(&eutraCellMeasObj->wlCellLst));
      eutraNeighCellCb = wrEmmFindEutranNeighCell(ueCb->cellId, \
            eutraNode->arfcn, cellWLstNode->pci);
      if(eutraNeighCellCb == NULLP)
      {
         RLOG0(L_ERROR, "eutraNeighCellCb is NULLP");
         RETVALUE(RFAILED);
      }
      /* Check for the mobility restriction by calling the HRL algo. */
      hrlDec = wrMbltyRestrc(WR_INTRA_SYS_MEAS, (PTR *)eutraNeighCellCb, \
            NULLP, NULLP, NULLP, ueCb->hoRstrctInfo);
      if(hrlDec == WR_MEAS_MBLTY_APPL_TO_CELL)
      {
         /* Cell is allowed, check whether the cell is there in UE's 
          * hrlMeasCfg as listed cell */
         cmLListFirst(&eutraUeMeasObj->wlCellLst);
         while(cmLListCrnt(&eutraUeMeasObj->wlCellLst))
         {
            ueWLstNode = (WrMeasNrWCellNode *) cmLListNode \
                         (cmLListCrnt(&eutraUeMeasObj->wlCellLst));
            if(ueWLstNode->pci == cellWLstNode->pci)
            {
               /* This cell is already white listed, so no changes for this cell. */
               cellFnd = TRUE;
               break;
            }
            cmLListNext(&eutraUeMeasObj->wlCellLst);
         }
         if(cellFnd == TRUE)
         {
            cmLListNext(&eutraCellMeasObj->wlCellLst);
            cellFnd = FALSE;
            continue;
         }
         /* Traverse through the hrlMeasCfg's blCellLst, if this cell exists then
          * delete from black list and add to white list. */
         cmLListFirst(&eutraUeMeasObj->blCellLst);
         while(cmLListCrnt(&eutraUeMeasObj->blCellLst))
         {
            ueBLstNode = (WrMeasNrBCellNode *) cmLListNode \
                         (cmLListCrnt(&eutraUeMeasObj->blCellLst));
            if(ueBLstNode->startPhyCellID == cellWLstNode->pci)
            {
               /* Delete this cell from black cell list in the hrlMeasCfg. 
                * Add in the delete black list of nrUpdtIdNode. */
               wrDelUeBlkLstHrlUe(eutraUeMeasObj, ueBLstNode);
               eutraUeMeasObj->blCellIdxInUse[ueBLstNode->cellIndex - 1] = FALSE;
               eutraNode->delBCellLst[eutraNode->delBCellCnt++] = ueBLstNode->cellIndex;
               /* Add this cell in the white list of the hrlMeasCfg. 
                * Add in the add white list of nrUpdtIdNode. */
               wrFillUeWhtLstHrlUe(eutraUeMeasObj, cellWLstNode, &cellIdx, eutraCellMeasObj->carrFreq);
               eutraNode->addWCellLst[eutraNode->addWCellCnt++] = cellIdx;
            }
            cmLListNext(&eutraUeMeasObj->blCellLst);
         }
      }
      else
      {
         /* Cell is not allowed, check whether the cell is there in UE's 
          * hrlMeasCfg as black listed cell */
         cmLListFirst(&eutraUeMeasObj->blCellLst);
         while(cmLListCrnt(&eutraUeMeasObj->blCellLst))
         {
            ueBLstNode = (WrMeasNrBCellNode *) cmLListNode \
                         (cmLListCrnt(&eutraUeMeasObj->blCellLst));
            if(ueBLstNode->startPhyCellID == cellWLstNode->pci)
            {
               /* This cell is already black listed, so no changes for this cell. */
               cellFnd = TRUE;
               break;
            }
            cmLListNext(&eutraUeMeasObj->blCellLst);
         }
         if(cellFnd == TRUE)
         {
            cmLListNext(&eutraCellMeasObj->wlCellLst);
            cellFnd = FALSE;
            continue;
         }
         /* Traverse through the hrlMeasCfg's wlCellLst, if this cell exists then
          * delete from white list and add to black list. */
         cmLListFirst(&eutraUeMeasObj->wlCellLst);
         while(cmLListCrnt(&eutraUeMeasObj->wlCellLst))
         {
            ueWLstNode = (WrMeasNrWCellNode *) cmLListNode \
                         (cmLListCrnt(&eutraUeMeasObj->wlCellLst));
            if(ueWLstNode->pci == cellWLstNode->pci)
            {
               /* Delete this cell from white cell list in the hrlMeasCfg. 
                * Add in the delete white list of nrUpdtIdNode. */
               wrDelUeWhtLstHrlUe(eutraUeMeasObj, ueWLstNode);
               eutraUeMeasObj->wlCellIdxInUse[ueWLstNode->cellIndex - 1] = FALSE;
               eutraNode->delWCellLst[eutraNode->delWCellCnt++] = ueWLstNode->cellIndex;
               /* Add this cell in the black list of the hrlMeasCfg. 
                * Add in the add black list of nrUpdtIdNode. */
               wrFillUeBlkLstHrlUe(eutraUeMeasObj, cellWLstNode, &cellIdx, eutraCellMeasObj->carrFreq);
               eutraNode->addBCellLst[eutraNode->addBCellCnt++] = cellIdx;
            }
            cmLListNext(&eutraUeMeasObj->wlCellLst);
         }
      }
      cmLListNext(&eutraCellMeasObj->wlCellLst);
   }
   if((eutraNode->addWCellCnt != 0) || (eutraNode->delWCellCnt != 0) 
         || (eutraNode->addBCellCnt != 0) || (eutraNode->delBCellCnt != 0))
   {
      nrUpdtNode->numEutra++;
   }
   RETVALUE(ROK);
}


/** @brief This function is responsible for populating a CDMA measObj
 *    in the hrlMeasCfg of the ueCb with data from the CellCb's CDMA measObj, 
 *    when a HRL is received for the first time in the DL NAS message and 
 *    previously HRL was not received in ICS or HO Req messages.
 *
 *  @details*
 *  Function: wrUmmCpyCdmaMeasObjToUe 
 *    
 *  Processing steps:
 *    - Allocate memory and populate the cdmaMeasObj.
 *    - Return the populated cdmaMeasObj to the caller.
 */
PRIVATE WrUmmMeasCdmaMeasObjNode* wrUmmCpyCdmaMeasObjToUe
(
 WrMeasUeCbCdmaMeasObjNode   *cdmaObj
 )
{
   WrUmmMeasCdmaMeasObjNode   *ueMeasObj;
   WR_ALLOC(&ueMeasObj, sizeof(WrUmmMeasCdmaMeasObjNode));
   if(ueMeasObj == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(NULLP);
   }

   ueMeasObj->cdmaType = cdmaObj->cdmaMeasObj->cdmaType;
   ueMeasObj->bandClass = cdmaObj->cdmaMeasObj->bandClass;
   ueMeasObj->arfcn = cdmaObj->cdmaMeasObj->arfcn;
   ueMeasObj->qOffsetRangeInterRAT = cdmaObj->cdmaMeasObj->qOffsetRangeInterRAT;
   ueMeasObj->searchWindowSize = cdmaObj->cdmaMeasObj->searchWindowSize;

   ueMeasObj->lnk.node = (PTR) ueMeasObj;
   RETVALUE(ueMeasObj);
}

/** @brief This function is responsible for populating a Cell List in the UE's 
 *    hrlMeasCfg's UTRA measObj with the data from received cell List
 *    when a HRL is received for the first time in the DL NAS message and 
 *    previously HRL was not received in ICS or HO Req messages.
 *
 *  @details*
 *  Function: wrUmmCpyUtraLstCells 
 *    
 *  Processing steps:
 *    - Allocate memory and populate the cell in the UE's hrlMeasCfg.
 *    - Add the created cell in the cell list of the UE's hrlMeasCfg.
 */
PRIVATE S16 wrUmmCpyUtraLstCells
(
 CmLListCp     *ueWlLst,
 CmLListCp     cellWlLst
 )
{
   WrMeasNrUtraWCellNode   *ueWlCellNode;
   WrMeasNrUtraWCellNode   *wlCellNode;

   cmLListFirst(&cellWlLst);
   while(cmLListCrnt(&cellWlLst))
   {
      wlCellNode = (WrMeasNrUtraWCellNode *) cmLListNode(cmLListCrnt(&cellWlLst));

      WR_ALLOC(&ueWlCellNode, sizeof(WrMeasNrUtraWCellNode));
      if(ueWlCellNode == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      ueWlCellNode->cellIndex = wlCellNode->cellIndex;
      ueWlCellNode->psc = wlCellNode->psc;

      ueWlCellNode->lnk.node = (PTR) ueWlCellNode;
      cmLListAdd2Tail(ueWlLst, &ueWlCellNode->lnk);

      cmLListNext(&cellWlLst);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for populating a UTRA measObj
 *    in the hrlMeasCfg of the ueCb with data from the CellCb's UTRA measObj, 
 *    when a HRL is received for the first time in the DL NAS message and 
 *    previously HRL was not received in ICS or HO Req messages.
 *
 *  @details*
 *  Function: wrUmmCpyUtraMeasObjToUe 
 *    
 *  Processing steps:
 *    - Allocate memory and populate the utraMeasObj.
 *    - Return the populated utraMeasObj to the caller.
 */
PRIVATE WrUmmMeasUtraMeasObjNode*  wrUmmCpyUtraMeasObjToUe
(
WrUmmMeasUtraMeasObjNode      *cellMeasObj
 )
{
   WrUmmMeasUtraMeasObjNode      *ueMeasObj;
   WR_ALLOC(&ueMeasObj, sizeof(WrUmmMeasUtraMeasObjNode));
   if(ueMeasObj == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(NULLP);
   }

   ueMeasObj->arfcn = cellMeasObj->arfcn;
   ueMeasObj->qOffsetFreq = cellMeasObj->qOffsetFreq;

   WR_MEM_COPY(&ueMeasObj->cellIdxInUse, &cellMeasObj->cellIdxInUse, \
         (WR_UMM_MEAS_MAX_CELL_IDX * sizeof(Bool)));
   wrUmmCpyUtraLstCells(&ueMeasObj->cellLst, cellMeasObj->cellLst);

   ueMeasObj->duplexMode = cellMeasObj->duplexMode;
   ueMeasObj->lnk.node = (PTR) ueMeasObj;

   RETVALUE(ueMeasObj);
}

/** @brief This function is responsible for populating a white cell List in 
 *     the UE's hrlMeasCfg's EUTRA measObj with the data from received cell List
 *    when a HRL is received for the first time in the DL NAS message and 
 *    previously HRL was not received in ICS or HO Req messages.
 *
 *  @details*
 *  Function: wrUmmCpyEutraWlLstCells 
 *    
 *  Processing steps:
 *    - Allocate memory and populate the white cell in the UE's hrlMeasCfg.
 *    - Add the created white cell in the white cell list of the UE's hrlMeasCfg.
 */
PRIVATE S16 wrUmmCpyEutraWlLstCells
(
 CmLListCp     *ueWlLst,
 CmLListCp     cellWlLst
 )
{
   WrMeasNrWCellNode    *ueWlCellNode;
   WrMeasNrWCellNode    *wlCellNode;

   cmLListFirst(&cellWlLst);
   while(cmLListCrnt(&cellWlLst))
   {
      wlCellNode = (WrMeasNrWCellNode *) cmLListNode(cmLListCrnt(&cellWlLst));

      WR_ALLOC(&ueWlCellNode, sizeof(WrMeasNrWCellNode));
      if(ueWlCellNode == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      ueWlCellNode->cellIndex = wlCellNode->cellIndex;
      ueWlCellNode->pci = wlCellNode->pci;
      ueWlCellNode->cio = wlCellNode->cio;

      ueWlCellNode->lnk.node = (PTR)ueWlCellNode;
      cmLListAdd2Tail(ueWlLst, &ueWlCellNode->lnk);

      cmLListNext(&cellWlLst);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for populating a black cell List in 
 *     the UE's hrlMeasCfg's EUTRA measObj with the data from received cell List
 *    when a HRL is received for the first time in the DL NAS message and 
 *    previously HRL was not received in ICS or HO Req messages.
 *
 *  @details*
 *  Function: wrUmmCpyEutraBlLstCells 
 *    
 *  Processing steps:
 *    - Allocate memory and populate the black cell in the UE's hrlMeasCfg.
 *    - Add the created black cell in the black cell list of the UE's hrlMeasCfg.
 */
PRIVATE S16 wrUmmCpyEutraBlLstCells
(
 CmLListCp     *ueBlLst,
 CmLListCp     cellBlLst
 )
{
   WrMeasNrBCellNode    *ueBlCellNode;
   WrMeasNrBCellNode    *blCellNode;

   cmLListFirst(&cellBlLst);
   while(cmLListCrnt(&cellBlLst))
   {
      blCellNode = (WrMeasNrBCellNode *) cmLListNode(cmLListCrnt(&cellBlLst));

      WR_ALLOC(&ueBlCellNode, sizeof(WrMeasNrBCellNode));
      if(ueBlCellNode == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      ueBlCellNode->cellIndex = blCellNode->cellIndex;
      ueBlCellNode->startPhyCellID = blCellNode->startPhyCellID;
      ueBlCellNode->phyCellIdRange = blCellNode->phyCellIdRange;

      ueBlCellNode->lnk.node = (PTR)ueBlCellNode;
      cmLListAdd2Tail(ueBlLst, &ueBlCellNode->lnk);

      cmLListNext(&cellBlLst);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for populating a EUTRA measObj
 *    in the hrlMeasCfg of the ueCb with data from the CellCb's EUTRA measObj, 
 *    when a HRL is received for the first time in the DL NAS message and 
 *    previously HRL was not received in ICS or HO Req messages.
 *
 *  @details*
 *  Function: wrUmmCpyEutraMeasObjToUe 
 *    
 *  Processing steps:
 *    - Allocate memory and populate the eutraMeasObj.
 *    - Return the populated eutraMeasObj to the caller.
 */
PRIVATE WrUmmMeasEutraMeasObjNode* wrUmmCpyEutraMeasObjToUe
(
 WrUmmMeasEutraMeasObjNode    *cellMeasObj
 )
{
   WrUmmMeasEutraMeasObjNode    *ueMeasObj;

   WR_ALLOC(&ueMeasObj, sizeof(WrUmmMeasEutraMeasObjNode));
   if(ueMeasObj == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(NULLP);
   }

   ueMeasObj->carrFreq = cellMeasObj->carrFreq;
   ueMeasObj->allowedMeasBW = cellMeasObj->allowedMeasBW;
   ueMeasObj->presAntPort1 = cellMeasObj->presAntPort1;
   ueMeasObj->neighCellCfg = cellMeasObj->neighCellCfg;
   ueMeasObj->qOffsetFreq = cellMeasObj->qOffsetFreq;

   WR_MEM_COPY(&ueMeasObj->wlCellIdxInUse, &cellMeasObj->wlCellIdxInUse, \
         (WR_UMM_MEAS_MAX_CELL_IDX * sizeof(Bool)));
   wrUmmCpyEutraWlLstCells(&ueMeasObj->wlCellLst, cellMeasObj->wlCellLst);

   WR_MEM_COPY(&ueMeasObj->blCellIdxInUse, &cellMeasObj->blCellIdxInUse, \
         (WR_UMM_MEAS_MAX_CELL_IDX * sizeof(Bool)));
   wrUmmCpyEutraBlLstCells(&ueMeasObj->blCellLst, cellMeasObj->blCellLst);

   ueMeasObj->lnk.node = (PTR) ueMeasObj;
   RETVALUE(ueMeasObj);
}

/** @brief This function is responsible for populating a EUTRA, UTRA and 
 *    CDMA measObj's in the hrlMeasCfg of the ueCb with data from the CellCb's 
 *    EUTRA, UTRA and CDMA measObj, when a HRL is received for the first time 
 *    in the DL NAS message and previously HRL was not received in ICS or HO Req messages.
 *
 *  @details*
 *  Function: wrUmmPopltHrlMeasCfgDlNas 
 *    
 *  Processing steps:
 *    - Populate the EUTRA intra measObj.
 *    - Populate the EUTRA inter measObj.
 *    - Populate the UTRA measObj.
 *    - Populate the CDMA measObj.
 */
PUBLIC S16  wrUmmPopltHrlMeasCfgDlNas
(
 WrUeCb     *ueCb
 )
{
   WrMeasUeCbEutraMeasObjNode  *eutraObj;
   WrUmmMeasEutraMeasObjNode  *ueEutraMeasObj;
   WrMeasUeCbUtraMeasObjNode   *utraObj;
   WrUmmMeasUtraMeasObjNode   *ueUtraMeasObj;
   WrMeasUeCbCdmaMeasObjNode   *cdmaMeasObj;
   WrUmmMeasCdmaMeasObjNode   *ueCdmaMeasObj;

   /* 1. Populate the EUTRA intra measurement object */
   eutraObj = &ueCb->measCfg->intraMeasObj;
   if((eutraObj != NULLP) && (eutraObj->eutraMeasObj != NULLP))
   {
      ueCb->hrlMeasCfg.intraMeasObj = wrUmmCpyEutraMeasObjToUe(eutraObj->eutraMeasObj);
   }
   else
   {
      /* measObjs are not yet configured to the UE, so do not process HRL now. */
      RETVALUE(RFAILED);      
   }

   /* 2. Populate the EUTRA inter measurement object */
   cmLListFirst(&ueCb->measCfg->interMeasObjLst);
   while(cmLListCrnt(&ueCb->measCfg->interMeasObjLst))
   {
      eutraObj = (WrMeasUeCbEutraMeasObjNode *) cmLListNode \
                  (cmLListCrnt(&ueCb->measCfg->interMeasObjLst));
      ueEutraMeasObj = wrUmmCpyEutraMeasObjToUe(eutraObj->eutraMeasObj);
      cmLListAdd2Tail(&ueCb->hrlMeasCfg.interMeasObjLst, &ueEutraMeasObj->lnk);
      cmLListNext(&ueCb->measCfg->interMeasObjLst);
   }

   /* 3. Populate the UTRA FDD measurement object */
   cmLListFirst(&ueCb->measCfg->utraFddMeasObjLst);
   while(cmLListCrnt(&ueCb->measCfg->utraFddMeasObjLst))
   {
      utraObj = (WrMeasUeCbUtraMeasObjNode *) cmLListNode \
                    (cmLListCrnt(&ueCb->measCfg->utraFddMeasObjLst));
      ueUtraMeasObj = wrUmmCpyUtraMeasObjToUe(utraObj->utraMeasObj);
      cmLListAdd2Tail(&ueCb->hrlMeasCfg.utraFddMeasObjLst, &ueUtraMeasObj->lnk);
      cmLListNext(&ueCb->measCfg->utraFddMeasObjLst);
   }

   /* 4. Populate the UTRA FDD measurement object */
   cmLListFirst(&ueCb->measCfg->utraTddMeasObjLst);
   while(cmLListCrnt(&ueCb->measCfg->utraTddMeasObjLst))
   {
      utraObj = (WrMeasUeCbUtraMeasObjNode *) cmLListNode \
                    (cmLListCrnt(&ueCb->measCfg->utraTddMeasObjLst));
      ueUtraMeasObj = wrUmmCpyUtraMeasObjToUe(utraObj->utraMeasObj);
      cmLListAdd2Tail(&ueCb->hrlMeasCfg.utraTddMeasObjLst, &ueUtraMeasObj->lnk);
      cmLListNext(&ueCb->measCfg->utraTddMeasObjLst);
   }

   /* 5. Populate the CDMA measurement object */
   cmLListFirst(&ueCb->measCfg->interRatMeasObjLst);
   while(cmLListCrnt(&ueCb->measCfg->interRatMeasObjLst))
   {
      cdmaMeasObj = (WrMeasUeCbCdmaMeasObjNode *) cmLListNode \
                    (cmLListCrnt(&ueCb->measCfg->interRatMeasObjLst));
      ueCdmaMeasObj = wrUmmCpyCdmaMeasObjToUe(cdmaMeasObj);
      cmLListAdd2Tail(&ueCb->hrlMeasCfg.cdmaMeasObjLst, &ueCdmaMeasObj->lnk);
      cmLListNext(&ueCb->measCfg->interRatMeasObjLst);
   }
   RETVALUE(ROK);      
}

/** @brief This function is responsible for checking whether the UTRA 
 *          FDD and TDD entries are to be updated for the UE or not.
 *          Compare the UTRA measObjs if they are already configured
 *          to the UE with the new HRL received and fill the wrUmmIntMeasTrans.
 *
 *  @details*
 *  Function: wrUmmMeasUtraRecfgAftDlNas 
 *    
 *  Processing steps:
 *    - Check for the UTRA FDD and TDD measurement objects, if any changes
 *       has to be done then add in the WrUmmIntMeasTrans and also update
 *       the ueCb->hrlMeasCfg.
 *    - Update the WrUmmIntMeasTrans with the UTRA FDD and TDD entries.
 */
PRIVATE S16 wrUmmMeasUtraRecfgAftDlNas
(
 WrUeCb     *ueCb,
 CmLListCp     cellMeasObjLst,
 CmLListCp     ueMeasObjLst,
 WrUmmIntMeasTrans          **nrUtraUpdtNode
 )
{
   U8                         ret = 0;
   U8                          measObjId = 0;
   WrUmmMeasUtraMeasObjNode   *ueUtraMeasObj = NULLP;
   WrUmmMeasUtraMeasObjNode   *cellUtraMeasObj = NULLP;
   WrUmmIntMeasTrans          *nrUpdtNode = *nrUtraUpdtNode;

   cmLListFirst(&cellMeasObjLst);
   while(cmLListCrnt(&cellMeasObjLst))
   {
      cellUtraMeasObj = (WrUmmMeasUtraMeasObjNode *) cmLListNode\
                        (cmLListCrnt(&cellMeasObjLst));
      cmLListFirst(&ueMeasObjLst);
      while(cmLListCrnt(&ueMeasObjLst))
      {
         ueUtraMeasObj = (WrUmmMeasUtraMeasObjNode *) cmLListNode\
                         (cmLListCrnt(&ueMeasObjLst));
         if(ueUtraMeasObj->arfcn == cellUtraMeasObj->arfcn)
         {
            if(ueUtraMeasObj->cellLst.count != 0)
            {
               if((ret = wrUmmUtraDynUeMeasCfg(cellUtraMeasObj, ueUtraMeasObj, ueCb, 
                           &nrUpdtNode->utraNode[nrUpdtNode->numUtra], nrUpdtNode)) == WR_MEAS_MBLTY_NOT_APPL_TO_RAT)
               {
                  /* Delete this measObj */
                  nrUpdtNode->utraNodeDel[nrUpdtNode->numUtraDel++] = ueUtraMeasObj->arfcn;
                  cmLListDelFrm(&ueMeasObjLst, &ueUtraMeasObj->lnk);
                  WR_FREE(ueUtraMeasObj, sizeof(WrUmmMeasUtraMeasObjNode));
               }
            }
            else
            {
               /* There is no UTRA measurement object configured for this 
                * arfcn to the UE, so configure a new measurement object now. */
               if((wrUmmUtraDynUeMeasObjCfg(cellUtraMeasObj, ueUtraMeasObj, ueCb, 
                     &nrUpdtNode->utraNode[nrUpdtNode->numUtra++],&measObjId))== RFAILED)
               {
                  RLOG0(L_FATAL, "wrUmmUtraDynUeMeasObjCfg: Failed to allocate memory.");
                  RETVALUE(RFAILED);
               }
               measObjId =  wrUmmMeasAddUtraMeasObjToUeCb(ueCb->measCfg, cellUtraMeasObj, 0xffff);
            }
         }
         cmLListNext(&ueMeasObjLst);
      }
      cmLListNext(&cellMeasObjLst);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for reconfiguring the UE with a 
 *    new measurement configuration if a new HRL is received in the DL NAS
 *    or if the HRL is received for the first time in the DL NAS mesaage 
 *    for a UE.
 *
 *  @details*
 *  Function: wrUmmMeasReCfgAftDlNas 
 *    
 *  Processing steps:
 *    - Check for the EUTRA intra/inter measurement objects, if any changes
 *       has to be done then add in the WrUmmIntMeasTrans and also update
 *       the ueCb->hrlMeasCfg.
 *    - Check for the UTRA and CDMA measurement objects, if any changes
 *       has to be done then add in the WrUmmIntMeasTrans and also update
 *       the ueCb->hrlMeasCfg.
 *     - Post as internal message to the measurement module by creating
 *       as internal measurement transaction and passing the populated
 *       WrUmmIntMeasTrans
 *     - Once the measReCfg is sent to the UE, free all the memory 
 *       allocated to the WrUmmIntMeasTrans.
 */
PUBLIC S16 wrUmmMeasReCfgAftDlNas
(
 WrUeCb     *ueCb
 )
{
   WrUmmCellCb                *cellCb;
   WrUmmIntMeasTrans          *nrUpdtNode;
   WrMeasEutraNrUpdtIdNode    *eutraNode = NULLP;
   WrUmmMeasEutraMeasObjNode  *cellInterMeasObj = NULLP;
   WrUmmMeasEutraMeasObjNode  *ueInterMeasObj = NULLP;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if(cellCb == NULLP)
   {
      RLOG0(L_ERROR, "cellCb is NULLP");
      RETVALUE(RFAILED);
   }
   /* WR_SZ_MEM_SDU_SIZE is 2048B, this is not relavent macro name but using it
    * rather than passing 2048 hardcoded value */
   if (ROK != cmAllocEvnt(sizeof(WrUmmIntMeasTrans), WR_SZ_MEM_SDU_SIZE, &wrCb.mem,
            (Ptr *)&nrUpdtNode))
   {    
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/ 

   /* Allocate memory for the EUTRA Node for the nos. of 
    * interFreq measObj's + 1 intraFreq measObj. */

   WR_GET_MEM(&nrUpdtNode->memCp, (1 + ueCb->hrlMeasCfg.interMeasObjLst.count) * sizeof(WrMeasEutraNrUpdtIdNode),
         (Ptr*)&eutraNode);
   if(eutraNode == NULLP)
   {
      WR_FREE_EVNT(nrUpdtNode);
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   nrUpdtNode->eutraNode = eutraNode;
   /* initially set the nrUpdtNode->numEutra to 0 */
   nrUpdtNode->numEutra = 0;

   /* 1. Check EUTRA Intra measurement object. */
   wrUmmEutraDynUeMeasCfg(&cellCb->measCellCb->intraMeasObj, 
         ueCb->hrlMeasCfg.intraMeasObj, ueCb, &eutraNode[nrUpdtNode->numEutra], nrUpdtNode);

   /* 2. Check EUTRA Inter measurement object. */
   cmLListFirst(&cellCb->measCellCb->interMeasObjLst);
   while(cmLListCrnt(&cellCb->measCellCb->interMeasObjLst))
   {
      cellInterMeasObj = (WrUmmMeasEutraMeasObjNode *) cmLListNode \
                         (cmLListCrnt(&cellCb->measCellCb->interMeasObjLst));
      cmLListFirst(&ueCb->hrlMeasCfg.interMeasObjLst);
      while(cmLListCrnt(&ueCb->hrlMeasCfg.interMeasObjLst))
      {
         ueInterMeasObj = (WrUmmMeasEutraMeasObjNode *) cmLListNode \
                          (cmLListCrnt(&ueCb->hrlMeasCfg.interMeasObjLst));
         if(ueInterMeasObj->carrFreq == cellInterMeasObj->carrFreq)
         {
            wrUmmEutraDynUeMeasCfg(cellInterMeasObj, ueInterMeasObj, ueCb, 
                  &eutraNode[nrUpdtNode->numEutra], nrUpdtNode);
         }
         cmLListNext(&ueCb->hrlMeasCfg.interMeasObjLst);
      }
      cmLListNext(&cellCb->measCellCb->interMeasObjLst);
   }

   /* 3. Check UTRA FDD measurement object */
   if((ueCb->hrlMeasCfg.utraFddMeasObjLst.count != 0)
         || (ueCb->hrlMeasCfg.utraTddMeasObjLst.count != 0))
   {
      WR_GET_MEM(&nrUpdtNode->memCp, (ueCb->hrlMeasCfg.utraFddMeasObjLst.count + 
               ueCb->hrlMeasCfg.utraTddMeasObjLst.count) * sizeof(WrMeasUtraNrUpdtIdNode),
            (Ptr*)&nrUpdtNode->utraNode);
      if(nrUpdtNode->utraNode == NULLP)
      {
         WR_FREE_EVNT(nrUpdtNode);
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      WR_GET_MEM(&nrUpdtNode->memCp,(ueCb->hrlMeasCfg.utraFddMeasObjLst.count + 
               ueCb->hrlMeasCfg.utraTddMeasObjLst.count) * sizeof(U32),
            (Ptr*)&nrUpdtNode->utraNodeDel);
      if(nrUpdtNode->utraNodeDel == NULLP)
      {
         WR_FREE_EVNT(nrUpdtNode);
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      /* initially set the nrUpdtNode->numUtra and nrUpdtNode->numUtraDel to 0 */
      nrUpdtNode->numUtra = 0;
      nrUpdtNode->numUtraDel = 0;

      wrUmmMeasUtraRecfgAftDlNas(ueCb, cellCb->measCellCb->utraFddMeasObjLst, 
            ueCb->hrlMeasCfg.utraFddMeasObjLst, &nrUpdtNode);

      wrUmmMeasUtraRecfgAftDlNas(ueCb, cellCb->measCellCb->utraTddMeasObjLst, 
            ueCb->hrlMeasCfg.utraTddMeasObjLst, &nrUpdtNode);
   }

   /* 5. Check if CDMA RAT is forbidden.
    * If its forbidden then remove the previous CDMA measCfg if any. */
   WrHrlDecisionRet  forbRat = WR_MEAS_MBLTY_APPL_TO_RAT;

   forbRat = wrChkForFrbRat(WR_FORB_RAT_CDMA, ueCb->hoRstrctInfo);
   if((ueCb->hrlMeasCfg.cdmaMeasObjLst.count != 0) &&
         (forbRat == WR_MEAS_MBLTY_NOT_APPL_TO_RAT))
   {
      WrUmmMeasCdmaMeasObjNode      *cdmaMeasObj;
      WrMeasCdmaNrDelIdNode         *cdmaDelNode;
      /* CDMA RAT is forbidden, so delete the CDMA measObj if any already
       * configured to the UE. */
      WR_GET_MEM(&nrUpdtNode->memCp, (ueCb->hrlMeasCfg.cdmaMeasObjLst.count) * sizeof(WrMeasCdmaNrDelIdNode),
            (Ptr*)&cdmaDelNode);
      if(cdmaDelNode == NULLP)
      {
         WR_FREE_EVNT(nrUpdtNode);
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      
      nrUpdtNode->cdma1xDelNode = cdmaDelNode;
      /* initially set the nrUpdtNode->numCdmaDel to 0 */
      nrUpdtNode->numCdmaDel = 0;
      
      cmLListFirst(&ueCb->hrlMeasCfg.cdmaMeasObjLst);
      while(cmLListCrnt(&ueCb->hrlMeasCfg.cdmaMeasObjLst))
      {
         cdmaMeasObj = (WrUmmMeasCdmaMeasObjNode *) cmLListNode \
                       (cmLListCrnt(&ueCb->hrlMeasCfg.cdmaMeasObjLst));
         if(cdmaMeasObj->cdmaType == NhuCDMA2000_Typtype1XRTTEnum)
         {
            cdmaDelNode[nrUpdtNode->numCdmaDel].bandClass = cdmaMeasObj->bandClass;
            cdmaDelNode[nrUpdtNode->numCdmaDel].arfcn = cdmaMeasObj->arfcn;
            nrUpdtNode->numCdmaDel++;
         }
         cmLListNext(&ueCb->hrlMeasCfg.cdmaMeasObjLst);
      }
   }

   /* Post internal message to the meaurement module */
   if((nrUpdtNode->numEutra != 0) || (nrUpdtNode->numUtra != 0)
         || (nrUpdtNode->numUtraDel != 0) || (nrUpdtNode->numCdmaDel != 0))
   {
      if(ROK !=  wrUmmMeasCreateIntTrans(cellCb, ueCb, WR_UMM_INTMSG_MEAS_TRANS, \
               WR_UMM_MEAS_NR_EXIST_FREQ, nrUpdtNode, NULLP))
      {
         WR_FREE_EVNT(nrUpdtNode);
      }
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for populating the white cells
 *       in the RRC Recfg message with measurement configuration.
 *
 *  @details*
 *  Function: wrUmmFillRRCRecfgWhtCells
 *    
 *  Processing steps:
 *    - Populate the nhuWCellsAddMod with the white
 *       cell present in the wlCellLst list.
 *    - Use the received cellIdCnt to fill the white
 *       cells in the RRC message.
 */
PRIVATE S16 wrUmmFillRRCRecfgWhtCells
(
 NhuCellsToAddMod       *nhuWCellsAddMod,
 U8                     *cellIdCnt,
 CmLListCp              wlCellLst
 )
{
   WrMeasNrWCellNode         *nrWCellObjNode;

   cmLListFirst(&wlCellLst);
   while(cmLListCrnt(&wlCellLst))
   {
      nrWCellObjNode = (WrMeasNrWCellNode *) cmLListNode(cmLListCrnt(&wlCellLst));
      wrFillTknU8(&nhuWCellsAddMod[*cellIdCnt].pres, 1);
      WR_FILL_TKN_UINT(nhuWCellsAddMod[*cellIdCnt].cellIdx, nrWCellObjNode->cellIndex)
         WR_FILL_TKN_UINT(nhuWCellsAddMod[*cellIdCnt].physCellId, nrWCellObjNode->pci)
         WR_FILL_TKN_UINT(nhuWCellsAddMod[*cellIdCnt].cellIvdualOffset, nrWCellObjNode->cio)
         (*cellIdCnt)++;
      cmLListNext(&wlCellLst);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for populating the black cells
 *       in the RRC Recfg message with measurement configuration.
 *
 *  @details*
 *  Function: wrUmmFillRRCRecfgBlkCells
 *    
 *  Processing steps:
 *    - Populate the nhuBCellsAddMod with the black
 *       cell present in the blCellLst list.
 *    - Use the received cellIdCnt to fill the black 
 *       cells in the RRC message.
 */
PRIVATE S16 wrUmmFillRRCRecfgBlkCells
(
 NhuBlackCellsToAddMod     *nhuBCellsAddMod,
 U8                     *cellIdCnt,
 CmLListCp              blCellLst
 )
{
   WrMeasNrBCellNode         *nrBCellObjNode;

   cmLListFirst(&blCellLst);
   while(cmLListCrnt(&blCellLst))
   {
      nrBCellObjNode = (WrMeasNrBCellNode *) cmLListNode(cmLListCrnt(&blCellLst));
      wrFillTknU8(&nhuBCellsAddMod[*cellIdCnt].pres, 1);
      WR_FILL_TKN_UINT(nhuBCellsAddMod[*cellIdCnt].cellIdx, nrBCellObjNode->cellIndex)
         wrFillTknU8(&nhuBCellsAddMod[*cellIdCnt].physCellIdRange.pres, 1);
      WR_FILL_TKN_UINT(nhuBCellsAddMod[*cellIdCnt].physCellIdRange.start, nrBCellObjNode->startPhyCellID)
         if(nrBCellObjNode->phyCellIdRange != 0)
         {
            nhuBCellsAddMod[*cellIdCnt].physCellIdRange.range.pres = PRSNT_NODEF;
         }
         else
         {
            nhuBCellsAddMod[*cellIdCnt].physCellIdRange.range.pres = NOTPRSNT;
         }
      nhuBCellsAddMod[*cellIdCnt].physCellIdRange.range.val = nrBCellObjNode->phyCellIdRange;
      (*cellIdCnt)++;
      cmLListNext(&blCellLst);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to Fill Eutra measurement Objects
 * into Nhu measurement Object.
 *
 *  @details*
 *  Function:wrUmmMeasFillEutraNhuAddIdLst
 *    
 *  Processing steps:
 *      - Allocate memory for white celllist and black Celllist.
 *      - copy measurement object info into Nhu measurement Object.
 *      - For report CGI config copy new pci into Nhu measurement Object.
 *  
 *   @param[in] memCp: pointer to Memory control point.
 *   @param[in] measTransCb: pointer to measurement transaction CB.
 *   @param[in] eutraMeasObj: pointer to Cell Eutra measurement Object.
 *   @param[out] nhuMeasEutra: pointer to NHU measurement Eutra object.
 *   @param[in] newPci: New Pci.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasFillEutraNhuAddIdLst
(
CmMemListCp                  *memCp,
WrUmmMeasAddInfo             *measTransCb,
WrUmmMeasEutraMeasObjNode    *eutraMeasObj,
NhuMeasObjectEUTRA           *nhuMeasEutra,
U16                          newPci,
WrUeCb                       *ueCb
)
{
   U8                        wlCellCnt;
   U8                        blCellCnt;
   U8                        cellIdCnt = 0;
   U8                        cellIdx = 0;
   Bool                      hrlCellLst = FALSE;
   CmLList                   *nrWCellLLst;
   CmLList                   *nrBCellLLst;
   NhuCellsToAddMod          *nhuWCellsAddMod = NULLP;
   NhuBlackCellsToAddMod     *nhuBCellsAddMod = NULLP;
   WrUmmMeasEutraMeasObjNode *hrlMeasObj = NULLP;

   TRC2(wrUmmMeasFillEutraNhuAddIdLst)

   wrFillTknU8(&nhuMeasEutra->pres, 1);
   WR_FILL_TKN_UINT(nhuMeasEutra->carrierFreq, eutraMeasObj->carrFreq)
   WR_FILL_TKN_UINT(nhuMeasEutra->allowedMeasBandwidth, eutraMeasObj->allowedMeasBW)
   WR_FILL_TKN_UINT(nhuMeasEutra->presenceAntennaPort1, eutraMeasObj->presAntPort1) 
   wrFillTknStrBSXL(&nhuMeasEutra->neighCellConfig, 2, &eutraMeasObj->neighCellCfg, memCp);
   WR_FILL_TKN_UINT(nhuMeasEutra->offsetFreq, eutraMeasObj->qOffsetFreq)
      
   if(measTransCb->isCellsToBeSkip != TRUE)
   {
       if((ueCb->hoRstrctInfo != NULLP))
       {
         hrlMeasObj = wrUmmHrlEutraMeasObjCfg(eutraMeasObj, ueCb);
         if(hrlMeasObj == NULLP)
         {
            RLOG0(L_ERROR, "hrlMeasObj is NULLP");
            RETVALUE(RFAILED);
         }
       }
       /* White and Black list information*/
       wlCellCnt = eutraMeasObj->wlCellLst.count;
       blCellCnt = eutraMeasObj->blCellLst.count;
         /* Traverse through the white list cells. */
         nrWCellLLst = eutraMeasObj->wlCellLst.first;

         /* Fix for updating the blCellLst in hrlBlCellLst */
         /* Traverse through the black list cells. */
         nrBCellLLst = eutraMeasObj->blCellLst.first;

         while(NULLP != nrWCellLLst)
         {
            WrMeasNrWCellNode         *nrWCellObjNode;
            nrWCellObjNode = (WrMeasNrWCellNode*)nrWCellLLst->node;
            cellIdx     = nrWCellObjNode->cellIndex;
            /* Check whether the cell needs to be added or not */
            if((ueCb->hoRstrctInfo != NULLP) && (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
            {
               WrEutranNeighCellCb   *neighCellCb;
               WrHrlDecisionRet       hrlDec = WR_MEAS_MBLTY_APPL_TO_CELL;
               /* Populate the WrHrlMeasCfg */
               hrlMeasObj = wrUmmHrlEutraMeasObjCfg(eutraMeasObj, ueCb);
               if(hrlMeasObj == NULLP)
               {
                  RLOG0(L_ERROR, "hrlMeasObj is NULLP");
                  RETVALUE(RFAILED);
               }

               neighCellCb = wrEmmFindEutranNeighCell(ueCb->cellId, \
                     eutraMeasObj->carrFreq, nrWCellObjNode->pci);
               if(neighCellCb == NULLP)
               {
                  RLOG0(L_ERROR, "neighCellCb is NULLP");
                  RETVALUE(RFAILED);
               }
               /* Check for the mobility restriction by calling the HRL algo. */
               hrlCellLst  = TRUE;
               hrlDec = wrMbltyRestrc(WR_INTRA_SYS_MEAS, (PTR *)neighCellCb, \
                     NULLP, NULLP, NULLP, ueCb->hoRstrctInfo);
               if(hrlDec == WR_MEAS_MBLTY_APPL_TO_CELL)
               {
                  /* Add this cell to ueCb->hrlMeasCfg WL list */
                  wrFillUeWhtLstHrlUe(hrlMeasObj, nrWCellObjNode, &cellIdx, eutraMeasObj->carrFreq);
               }
               else
               {
                  /* Add this cell to ueCb->hrlMeasCfg BL list */
                  wrFillUeBlkLstHrlUe(hrlMeasObj, nrWCellObjNode, &cellIdx, eutraMeasObj->carrFreq);
               }
            }
            nrWCellLLst = nrWCellLLst->next;
         }

         /* Fix for updating the blCellLst in hrlBlCellLst */
         while(NULLP != nrBCellLLst)
         {
            WrMeasNrBCellNode         *nrBCellObjNode;
            nrBCellObjNode = (WrMeasNrBCellNode*)nrBCellLLst->node;
            cellIdx     = nrBCellObjNode->cellIndex;

            /* Check whether the cell needs to be added or not */
            if((ueCb->hoRstrctInfo != NULLP) && (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
            {
               WrEutranNeighCellCb   *neighCellCb;

               /* Populate the WrHrlMeasCfg */
               hrlMeasObj = wrUmmHrlEutraMeasObjCfg(eutraMeasObj, ueCb);
               if(hrlMeasObj == NULLP)
               {
                  RLOG0(L_ERROR, "hrlMeasObj is NULLP");
                  RETVALUE(RFAILED);
               }
               neighCellCb = wrEmmFindEutranNeighCell(ueCb->cellId, \
                     eutraMeasObj->carrFreq, nrBCellObjNode->startPhyCellID);

               if(neighCellCb == NULLP)
               {
                  RLOG0(L_ERROR, "neighCellCb is NULLP");
                  RETVALUE(RFAILED);
               }

               /* Add this cell to ueCb->hrlMeasCfg BL list */
               wrFillHrlUeBlkLstFromEutraBLst(hrlMeasObj, nrBCellObjNode, &cellIdx, eutraMeasObj->carrFreq);
            }
            nrBCellLLst = nrBCellLLst->next;
         }
         /* For HRL UEs fill the white list with cells present in hrlMeasObj->wlCellLst. */
         if((hrlCellLst == TRUE) && (hrlMeasObj->wlCellLst.count != 0))
         {
            if(ROK != cmGetMem(memCp, hrlMeasObj->wlCellLst.count * sizeof(NhuCellsToAddMod), 
                     (Ptr *)&nhuWCellsAddMod))
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            nhuMeasEutra->cellsToAddModLst.member = nhuWCellsAddMod;
            WR_FILL_TKN_UINT(nhuMeasEutra->cellsToAddModLst.noComp, hrlMeasObj->wlCellLst.count)
               wrUmmFillRRCRecfgWhtCells(nhuWCellsAddMod, &cellIdCnt, hrlMeasObj->wlCellLst);
         }
         /* For non - HRL UEs fill the white list with cells present in eutraMeasObj. */
         if((0 != wlCellCnt) && (hrlCellLst == FALSE))
         {
            if(ROK != cmGetMem(memCp, wlCellCnt * sizeof(NhuCellsToAddMod), 
                     (Ptr *)&nhuWCellsAddMod))
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            nhuMeasEutra->cellsToAddModLst.member = nhuWCellsAddMod;
            WR_FILL_TKN_UINT(nhuMeasEutra->cellsToAddModLst.noComp, wlCellCnt)
               wrUmmFillRRCRecfgWhtCells(nhuWCellsAddMod, &cellIdCnt, eutraMeasObj->wlCellLst);
         }
         cellIdCnt = 0;
         /* fix for the blacklisted cell */
         if((hrlCellLst == TRUE) && ((hrlMeasObj->blCellLst.count != 0) || (blCellCnt != 0)))
         {
            if(ROK != cmGetMem(memCp, (hrlMeasObj->blCellLst.count + blCellCnt) \
                     * sizeof(NhuBlackCellsToAddMod), (Ptr *)&nhuBCellsAddMod))
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            nhuMeasEutra->blackCellsToAddModLst.member = nhuBCellsAddMod;
            WR_FILL_TKN_UINT(nhuMeasEutra->blackCellsToAddModLst.noComp,  \
                  (hrlMeasObj->blCellLst.count + blCellCnt))
               wrUmmFillRRCRecfgBlkCells(nhuBCellsAddMod, &cellIdCnt, hrlMeasObj->blCellLst);
         }
     if(0 != blCellCnt)
     {
            if(hrlCellLst == FALSE)
            {
        if(ROK != cmGetMem(memCp, blCellCnt * sizeof(NhuBlackCellsToAddMod),
                                  (Ptr *)&nhuBCellsAddMod))
        {
           RLOG0(L_FATAL, "Memory allocation failed.");
           RETVALUE(RFAILED);
        }
        nhuMeasEutra->blackCellsToAddModLst.member = nhuBCellsAddMod;
        WR_FILL_TKN_UINT(nhuMeasEutra->blackCellsToAddModLst.noComp,  blCellCnt)
            }
            if(nhuBCellsAddMod != NULLP)
            {
               wrUmmFillRRCRecfgBlkCells(nhuBCellsAddMod, &cellIdCnt, eutraMeasObj->blCellLst);
        }
     }
     nhuMeasEutra->cellForWhichToReportCGI.pres = NOTPRSNT;
   }
   else
   {
      if(newPci != 0xFFFF)
      {
         WR_FILL_TKN_UINT(nhuMeasEutra->cellForWhichToReportCGI, newPci)
      }
      else
      {
         nhuMeasEutra->cellForWhichToReportCGI.pres = NOTPRSNT;
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to Fill Utra measurement Objects
 * into Nhu measurement Object.
 *
 *  @details*
 *  Function:wrUmmMeasFillUtraNhuAddIdLst
 *
 *  Processing steps:
 *      - Allocate memory for white celllist .
 *      - copy measurement object info into Nhu measurement Object.
 *      - For report CGI config copy new psc into Nhu measurement Object.
 *
 *   @param[in] memCp: pointer to Memory control point.
 *   @param[in] measTransCb: pointer to measurement transaction CB.
 *   @param[in] utraMeasObj: pointer to Cell Eutra measurement Object.
 *   @param[out] nhuMeasUtra: pointer to NHU measurement Utra object.
 *   @param[in] newPci: New Psc.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasFillUtraNhuAddIdLst
(
CmMemListCp                  *memCp,
WrUmmMeasAddInfo             *measTransCb,
WrUmmMeasUtraMeasObjNode     *utraMeasObj,
NhuMeasObjectUTRA            *nhuMeasUtra,
 U16                          newPsc,
 WrUeCb                       *ueCb
)
{
   U8                        CellCnt;
   U8                        fddCellIdCnt = 0;
   U8                        tddCellIdCnt = 0;
   CmLList                   *nrWCellLLst;
   NhuCellsToAddModUTRA_FDD  *nhuWCellsAddModFDD;
   NhuCellsToAddModUTRA_TDD  *nhuWCellsAddModTDD;

   TRC2(wrUmmMeasFillUtraNhuAddIdLst)

   wrFillTknU8(&nhuMeasUtra->pres, 1);
   WR_FILL_TKN_UINT(nhuMeasUtra->carrierFreq, utraMeasObj->arfcn)
   WR_FILL_TKN_UINT(nhuMeasUtra->offsetFreq, utraMeasObj->qOffsetFreq)

   if(measTransCb->isCellsToBeSkip != TRUE)
   {
     /* White list information*/
     CellCnt = utraMeasObj->cellLst.count;
     if(0 != CellCnt)
     {
        if(utraMeasObj->duplexMode == WR_UMM_MEAS_DUPLEX_MODE_FDD)
        {
           WR_FILL_TKN_UINT(nhuMeasUtra->cellsToAddModLst.choice, WR_UMM_MEAS_DUPLEX_MODE_FDD)

           if(ROK != cmGetMem(memCp, CellCnt * sizeof(NhuCellsToAddModUTRA_FDD),
                      (Ptr *)&nhuWCellsAddModFDD))
           {
              RLOG0(L_FATAL, "Memory allocation failed.");
              RETVALUE(RFAILED);
           }
           nhuMeasUtra->cellsToAddModLst.val.cellsToAddModLstUTRA_FDD.member = nhuWCellsAddModFDD;
           WR_FILL_TKN_UINT(nhuMeasUtra->cellsToAddModLst.val.cellsToAddModLstUTRA_FDD.noComp, CellCnt)

           nrWCellLLst = utraMeasObj->cellLst.first;
           while(NULLP != nrWCellLLst)
           {
              WrMeasNrUtraCellNode         *nrWCellObjNode;
              nrWCellObjNode = (WrMeasNrUtraCellNode*)nrWCellLLst->node;

              wrFillTknU8(&nhuWCellsAddModFDD[fddCellIdCnt].pres, PRSNT_NODEF);
              WR_FILL_TKN_UINT(nhuWCellsAddModFDD[fddCellIdCnt].cellIdx, nrWCellObjNode->cellIndex)
              WR_FILL_TKN_UINT(nhuWCellsAddModFDD[fddCellIdCnt].physCellId, nrWCellObjNode->psc)
              nrWCellLLst = nrWCellLLst->next;
              fddCellIdCnt++;
           }
           nhuMeasUtra->cellForWhichToReportCGI.val.utra_FDD.pres = NOTPRSNT;
        }
        else 
        {
           WR_FILL_TKN_UINT(nhuMeasUtra->cellsToAddModLst.choice, WR_UMM_MEAS_DUPLEX_MODE_TDD)

           if(ROK != cmGetMem(memCp, CellCnt * sizeof(NhuCellsToAddModUTRA_TDD),
                      (Ptr *)&nhuWCellsAddModTDD))
           {
              RLOG0(L_FATAL, "Memory allocation failed.");
              RETVALUE(RFAILED);
           }
           nhuMeasUtra->cellsToAddModLst.val.cellsToAddModLstUTRA_TDD.member = nhuWCellsAddModTDD;
           WR_FILL_TKN_UINT(nhuMeasUtra->cellsToAddModLst.val.cellsToAddModLstUTRA_TDD.noComp, CellCnt)

           nrWCellLLst = utraMeasObj->cellLst.first;
           while(NULLP != nrWCellLLst)
           {
              WrMeasNrUtraCellNode         *nrWCellObjNode;
              nrWCellObjNode = (WrMeasNrUtraCellNode*)nrWCellLLst->node;

              wrFillTknU8(&nhuWCellsAddModTDD[tddCellIdCnt].pres, PRSNT_NODEF);
              WR_FILL_TKN_UINT(nhuWCellsAddModTDD[tddCellIdCnt].cellIdx, nrWCellObjNode->cellIndex)
              WR_FILL_TKN_UINT(nhuWCellsAddModTDD[tddCellIdCnt].physCellId, nrWCellObjNode->psc)
              nrWCellLLst = nrWCellLLst->next;
              tddCellIdCnt++;
           }
           nhuMeasUtra->cellForWhichToReportCGI.val.utra_TDD.pres = NOTPRSNT;
        }
     }
  }
  else
  {
    if(utraMeasObj->duplexMode == WR_UMM_MEAS_DUPLEX_MODE_FDD)
    {
       WR_FILL_TKN_UINT(nhuMeasUtra->cellForWhichToReportCGI.choice, WR_UMM_MEAS_DUPLEX_MODE_FDD)
    }
    WR_FILL_TKN_UINT(nhuMeasUtra->cellForWhichToReportCGI.val.utra_FDD, newPsc)
  }
  RETVALUE(ROK);
}

/** @brief This function is responsible for to Fill GERAN measurement Objects
 * into Nhu measurement Object.
 *
 *  @details
 *             Function: wrUmmMeasFillGeranNhuAddIdLst
 *
 *             Processing steps:
 *                - Fill starting ARFCN, bandindicator and explicit list of
 *                ARFCNs
 *                - Allocate memory for explicit list of ARFCNs
 *
 *   @param[in] memCp: pointer to Memory control point.
 *   @param[in] measTransCb: pointer to measurement transaction CB.
 *   @param[in] geranMeasObj: pointer to Geran measurement Object.
 *   @param[out] nhuMeasGeran: pointer to NHU measurement Geran object.
 *   @param[in] measCellCfg : pointer to Geran cell meas obj.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasFillGeranNhuAddIdLst
(
CmMemListCp                  *memCp,
WrUmmMeasAddInfo             *measTransCb,
WrUmmMeasGeranMeasObjNode    *geranMeasObj,
NhuMeasObjectGERAN           *nhuMeasGeran
)
{
  NhuARFCN_ValueGERAN          *member = NULLP;
  U32                          idx = 0;
  U32                          arfcnCnt = geranMeasObj->arfcnCnt;
  
  wrFillTknU8(&nhuMeasGeran->pres,PRSNT_NODEF);
  wrFillTknU8(&nhuMeasGeran->carrierFreqs.pres,PRSNT_NODEF);
  wrFillTknU32(&nhuMeasGeran->carrierFreqs.startingARFCN,geranMeasObj->arfcn[idx++]);
  arfcnCnt--;
  if(TRUE == geranMeasObj->isBandInd1900Pres)
  {
     wrFillTknU32(&nhuMeasGeran->carrierFreqs.bandIndicator,
           NhuBandIndicatorGERANpcs1900Enum);
  }
  else
  {
     wrFillTknU32(&nhuMeasGeran->carrierFreqs.bandIndicator,
           NhuBandIndicatorGERANdcs1800Enum);
  }

  wrFillTknU8(&nhuMeasGeran->carrierFreqs.followingARFCNs.choice,FOLLOWINGARFCNS_EXPLICITLSTOFARFCNS);
  /* number of components can be zero */
  wrFillTknU16(&nhuMeasGeran->carrierFreqs.followingARFCNs.val.explicitLstOfARFCNs.noComp,(arfcnCnt));
  if(arfcnCnt > 0)
  {
     if(ROK != cmGetMem(memCp, (arfcnCnt) * sizeof(NhuARFCN_ValueGERAN),
              (Ptr *)&member))
     {
        RLOG0(L_FATAL, "Memory allocation failed.");
        RETVALUE(RFAILED);
     }

     while(((WR_UMM_MAX_GERAN_ARFCN-1) > idx) && arfcnCnt)
     {
        wrFillTknU32(&member[idx-1],geranMeasObj->arfcn[idx]);
        idx++;
        arfcnCnt--;
     }
     nhuMeasGeran->carrierFreqs.followingARFCNs.val.explicitLstOfARFCNs.member = member;
  }

  /*Index is 0 for measGERANPeridRptCfg because 0 = Report strong cells*/
  wrFillTknS32(&nhuMeasGeran->offsetFreq,geranMeasObj->qOffset);

  /*Fill NCC Permitted which is ORed for all the configured cells*/
  wrFillTknStrBSXL(&nhuMeasGeran->ncc_Permitted,8,&geranMeasObj->nccPermittedMeas,memCp);

  RETVALUE(ROK);
}
/** @brief This function is responsible for to Fill CDMA measurement Objects
 * into Nhu measurement Object.
 *
 *  @details*
 *  Function:wrUmmMeasFillCdmaNhuAddIdLst
 *    
 *  Processing steps:
 *      - Allocate memory for celllist .
 *      - copy measurement object info into Nhu measurement Object.
 *      - For report CGI config copy new psc into Nhu measurement Object.
 *  
 *   @param[in] memCp: pointer to Memory control point.
 *   @param[in] measTransCb: pointer to measurement transaction CB.
 *   @param[in] cdmaMeasObj: pointer to Cell Cdma measurement Object.
 *   @param[out] nhuMeasCdma: pointer to NHU measurement Cdma object.
 *   @param[in] newPci: New Psc.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasFillCdmaNhuAddIdLst
(
 CmMemListCp                 *memCp,
 WrUmmMeasAddInfo            *measTransCb,
 WrUmmMeasCdmaMeasObjNode    *cdmaMeasObj,
NhuMeasObjectCDMA2000        *nhuMeasCdma,
 U16                         newPci,
 WrUeCb                      *ueCb
)
{
   U8                        CellCnt;
   U8                         cellIdCnt = 0;
   CmLList                   *cdmaCellLst;
   NhuCellsToAddModCDMA2000  *nhuCellsAddModCdma;

   TRC2(wrUmmMeasFillCdmaNhuAddIdLst)

      wrFillTknU8(&nhuMeasCdma->pres, 1);
      WR_FILL_TKN_UINT(nhuMeasCdma->cdma2000_Typ, cdmaMeasObj->cdmaType)
      wrFillTknU8(&nhuMeasCdma->carrierFreq.pres, 1);
      WR_FILL_TKN_UINT(nhuMeasCdma->carrierFreq.bandClass, cdmaMeasObj->bandClass)
      WR_FILL_TKN_UINT(nhuMeasCdma->carrierFreq.arfcn, cdmaMeasObj->arfcn)
      WR_FILL_TKN_UINT(nhuMeasCdma->searchWindowSize, cdmaMeasObj->searchWindowSize)
      WR_FILL_TKN_UINT(nhuMeasCdma->offsetFreq, cdmaMeasObj->qOffsetRangeInterRAT)

      if(measTransCb->isCellsToBeSkip != TRUE)
      {
         /* If this UE has HRL, then copy the CDMA measObj in ueCb->hrlMeasCfg */
         if((ueCb->hoRstrctInfo != NULLP) && (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
         {
           if((wrUmmHrlCdmaMeasObjCfg(cdmaMeasObj, ueCb))== RFAILED)
           {
              RLOG0(L_FATAL, "wrUmmMeasFillCdmaNhuAddIdLst: Failed to allocate memory.");
              RETVALUE(RFAILED);
           }
         }
         /* White list information*/
         CellCnt = cdmaMeasObj->cdmaCellInfo.count;
         if(0 != CellCnt)
         {
            /* Allocate memory for List of cells, PnOffset for a particular
             * measurement object i.e 9band, frequency) */
            if(ROK != cmGetMem(memCp, CellCnt * sizeof(NhuCellsToAddModCDMA2000),
                     (Ptr *)&nhuCellsAddModCdma))
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            nhuMeasCdma->cellsToAddModLst.member = nhuCellsAddModCdma;
            WR_FILL_TKN_UINT(nhuMeasCdma->cellsToAddModLst.noComp, CellCnt)

            cdmaCellLst = cdmaMeasObj->cdmaCellInfo.first;
            while(NULLP != cdmaCellLst)
            {
               WrMeasNrCdmaCellNode *cdmaCellNode; 
               cdmaCellNode = (WrMeasNrCdmaCellNode*)cdmaCellLst->node;

               wrFillTknU8(&nhuCellsAddModCdma[cellIdCnt].pres, TRUE);
               WR_FILL_TKN_UINT(nhuCellsAddModCdma[cellIdCnt].cellIdx, cdmaCellNode->cellIndex)
               WR_FILL_TKN_UINT(nhuCellsAddModCdma[cellIdCnt].physCellId, cdmaCellNode->pci)
               cdmaCellLst = cdmaCellLst->next;
               cellIdCnt++;
            }
            nhuMeasCdma->cellForWhichToReportCGI.pres = NOTPRSNT;
         }
      }
      else
      {
         WR_FILL_TKN_UINT(nhuMeasCdma->cellForWhichToReportCGI, newPci)
      }
   RETVALUE(ROK);
}/* End of wrUmmMeasFillCdmaNhuAddIdLst */

/** @brief This function is responsible for to get measurement Object and Type
 * from measurement object List.
 *
 *  @details
 *  Function:wrUmmMeasFindMeasObjAndTyp
 *    
 *  Processing steps:
 *      - Traverse List and find Measurement Object and Type.
 *        
 *   @param[out] nrType: pointer to the Neighbour type.
 *   @param[out] eutraMeasObj: pointer to Eutra measurement Object.
 *   @param[out] cdmaMeasObj: pointer to Cdma measurement Object.
 *   @param[in] measObjId: measurement Id.
 *   @param[in] measUeCb: pointer to measurement Ue Cb.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PUBLIC S16 wrUmmMeasFindMeasObjAndTyp
(
 WrUmmMeasObjInfo             *measObjInfo,
 U8                           measObjId,
 WrMeasUeCb                   *measUeCb
)
{
   WrMeasUeCbEutraMeasObjNode   *enbMeasObj;
   CmLList                      *measObjEutraNode;
   WrMeasUeCbCdmaMeasObjNode    *cdma2kMeasObj;
   CmLList                      *measObjCdmaNode;
   WrMeasUeCbUtraMeasObjNode    *uMeasObj;
   CmLList 		                 *measObjUtraNode;
   WrMeasUeCbGeranMeasObjNode   *geranObj;
   CmLList                      *measObjGeranNode;

   TRC2(wrUmmMeasFindMeasObjAndTyp)
   if (measUeCb->intraMeasObj.measObjId == measObjId)
   {
      measObjInfo->nrType = WR_UMM_MEAS_INTRA_FREQ;
      measObjInfo->eutraObj = &measUeCb->intraMeasObj;
      RETVALUE(ROK);
   }
   
   measObjEutraNode = measUeCb->interMeasObjLst.first;
   while (NULLP != measObjEutraNode)
   {
      enbMeasObj = (WrMeasUeCbEutraMeasObjNode*)measObjEutraNode->node;
      if (measObjId == enbMeasObj->measObjId)
      {
         measObjInfo->nrType = WR_UMM_MEAS_INTER_FREQ;
         measObjInfo->eutraObj = enbMeasObj;
         RETVALUE(ROK);
      }
      measObjEutraNode = measObjEutraNode->next;
   }
   measObjUtraNode = measUeCb->utraFddMeasObjLst.first;
   while (NULLP != measObjUtraNode)
   {
      uMeasObj = (WrMeasUeCbUtraMeasObjNode*)measObjUtraNode->node;	
      if (measObjId == uMeasObj->measObjId)
      {
         measObjInfo->nrType = WR_UMM_MEAS_UTRA_FDD_FREQ;
         measObjInfo->utraFddObj = uMeasObj;
         RETVALUE(ROK);
      }
      measObjUtraNode = measObjUtraNode->next;
   }

   measObjUtraNode = measUeCb->utraTddMeasObjLst.first;
   while (NULLP != measObjUtraNode)
   {
      uMeasObj = (WrMeasUeCbUtraMeasObjNode*)measObjUtraNode->node;	
      if (measObjId == uMeasObj->measObjId)
      {
         measObjInfo->nrType = WR_UMM_MEAS_UTRA_TDD_FREQ;
         measObjInfo->utraTddObj = uMeasObj;
         RETVALUE(ROK);
      }
      measObjUtraNode = measObjUtraNode->next;
   }

   measObjGeranNode = measUeCb->geranMeasObjLst.first;
   while (NULLP != measObjGeranNode)
   {
      geranObj = (WrMeasUeCbGeranMeasObjNode *) measObjGeranNode->node;
      if (measObjId == geranObj->measObjId)
      {
         measObjInfo->nrType = WR_UMM_MEAS_GERAN_FREQ;
         measObjInfo->geranObj = geranObj;
         RETVALUE(ROK);
      }
      measObjGeranNode = measObjGeranNode->next;
   }

   measObjCdmaNode = measUeCb->interRatMeasObjLst.first;
   while ( NULLP != measObjCdmaNode)
   {
      cdma2kMeasObj =  (WrMeasUeCbCdmaMeasObjNode*)measObjCdmaNode->node;
      if (measObjId == cdma2kMeasObj->measObjId)
      {
         if(cdma2kMeasObj->cdmaMeasObj->cdmaType == NhuCDMA2000_Typtype1XRTTEnum)
         {
            measObjInfo->nrType = WR_UMM_MEAS_1XCDMA;
         }
         else
         {
            measObjInfo->nrType = WR_UMM_MEAS_CDMA_HRPD;
         }
         measObjInfo->cdmaObj = cdma2kMeasObj;
         RETVALUE(ROK);
      }
      measObjCdmaNode = measObjCdmaNode->next;
   }
   RETVALUE(RFAILED);
}

/* @brief: This function will fetch the Utra MeasObjNode
 *  *          matching the received arfcn from the received list.
 *   * */
PUBLIC WrUmmMeasUtraMeasObjNode* wrUmmGetUtraMeasObjNode
(
  U16           arfcn,
   CmLListCp     utraMeasObjLst
    )
{
   WrUmmMeasUtraMeasObjNode      *utraMeasObjNode = NULLP;

   cmLListFirst(&utraMeasObjLst);
   while(cmLListCrnt(&utraMeasObjLst))
   {
      utraMeasObjNode = (WrUmmMeasUtraMeasObjNode *) cmLListNode(cmLListCrnt(&utraMeasObjLst));
      if(arfcn == utraMeasObjNode->arfcn)
      {
         RETVALUE(utraMeasObjNode);
      }
      cmLListNext(&utraMeasObjLst);
   }
   RETVALUE(NULLP);
}

/** @brief This function is responsible for to Fill measurement Objects
 * into Nhu measurement Object.
 *
 *  @details*
 *  Function:wrUmmMeasFillNhuAddIdLst
 *    
 *  Processing steps:
 *      - Get the Measurement Id's from measurement transaction control block.
 *      - Using Id's get the measurement objects from UE Control block.
 *      - If Object is Eutra call respective function to copy 
 *         data Nhu measurement Object.
 *  
 *   @param[in] memCp: pointer to Memory control point.
 *   @param[out] nhuMeasCfg: pointer to NHU measurement config object.
 *   @param[in] measUeCb: pointer to measurement ue control block.
 *   @param[in] measTransCb: pointer to measurement transaction CB.
 *   @param[in] measCellCfg: pointer to measurement config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasFillNhuAddIdLst
(
CmMemListCp                  *memCp,
NhuMeasConfig                *nhuMeasCfg,
WrUeCb                       *ueCb,
WrUmmMeasAddInfo             *measTransCb
)
{
   U8                           idCnt = 0;
   U8                           measObjCnt = 0;
   U8                           measObjID;
   U16                          newPci;
   U16                          newPsc;
   NhuMeasObjectToAddMod        *nhuMeasObj;

   TRC2(wrUmmMeasFillNhuAddIdLst);
   measObjCnt = measTransCb->measObjIdCnt;
   if(0 != measObjCnt)
   { 
      if(ROK != cmGetMem(memCp, measObjCnt * sizeof(NhuMeasObjectToAddMod),
               (Ptr *)&nhuMeasObj))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuMeasCfg->measObjectToAddModLst.member = nhuMeasObj; 

      WR_FILL_TKN_UINT(nhuMeasCfg->measObjectToAddModLst.noComp, measObjCnt)
   }
   /*Copy meas Objects into Nhu structure*/
   for(idCnt = 0; idCnt < measObjCnt; idCnt++)
   {
      WrUmmMeasObjInfo             measObjInfo = {0};
      measObjID = measTransCb->measObjLst[idCnt];
      if(ROK != wrUmmMeasFindMeasObjAndTyp(&measObjInfo, measObjID, ueCb->measCfg))
      {
         RLOG0(L_ERROR, "wrUmmMeasFindMeasObjAndTyp: Failed");
         RETVALUE(RFAILED);
      }

      wrFillTknU8(&nhuMeasObj[idCnt].pres, 1);
      WR_FILL_TKN_UINT(nhuMeasObj[idCnt].measObjectId, measObjID)
         if((measObjInfo.nrType == WR_UMM_MEAS_INTRA_FREQ) ||
               (measObjInfo.nrType == WR_UMM_MEAS_INTER_FREQ))
         {
            WrUmmMeasEutraMeasObjNode    *eutraMeasObj;
            NhuMeasObjectEUTRA           *nhuMeasEutra;

            eutraMeasObj = measObjInfo.eutraObj->eutraMeasObj;
            newPci = measObjInfo.eutraObj->newPci;
            nhuMeasEutra = &nhuMeasObj[idCnt].measObject.val.measObjectEUTRA;

            WR_FILL_TKN_UINT(nhuMeasObj[idCnt].measObject.choice, MEASOBJECT_MEASOBJECTEUTRA)
               if(ROK != wrUmmMeasFillEutraNhuAddIdLst(memCp, measTransCb, eutraMeasObj, nhuMeasEutra, newPci, ueCb))
               {
                  RLOG0(L_ERROR, "wrUmmMeasFillEutraNhuAddIdLst: Failed");
                  RETVALUE(RFAILED);
               }
         }
         else if((measObjInfo.nrType == WR_UMM_MEAS_UTRA_FDD_FREQ) ||
               (measObjInfo.nrType == WR_UMM_MEAS_UTRA_TDD_FREQ))
         {
            NhuMeasObjectUTRA         *nhuMeasUtra;
            WrUmmMeasUtraMeasObjNode  *utraMeasObj;

            if(measObjInfo.nrType == WR_UMM_MEAS_UTRA_FDD_FREQ)
            {
               utraMeasObj = measObjInfo.utraFddObj->utraMeasObj;
               newPsc = measObjInfo.utraFddObj->newPsc;
               /* If HRL_LAI list consist measObj for this arfcn then fetch from there. */
               if(ueCb->hrlMeasCfg.utraFddMeasObjLst.count != 0)
               {
                  utraMeasObj = (WrUmmMeasUtraMeasObjNode *) 
                     wrUmmGetUtraMeasObjNode(measObjInfo.utraFddObj->utraMeasObj->arfcn, 
                           ueCb->hrlMeasCfg.utraFddMeasObjLst);
               }
            }
            else
            {
               utraMeasObj = measObjInfo.utraTddObj->utraMeasObj;
               newPsc = measObjInfo.utraTddObj->newPsc;
               /* If HRL_LAI list consist measObj for this arfcn then fetch from there. */
               if(ueCb->hrlMeasCfg.utraTddMeasObjLst.count != 0)
               {
                  utraMeasObj = (WrUmmMeasUtraMeasObjNode *) 
                     wrUmmGetUtraMeasObjNode(measObjInfo.utraTddObj->utraMeasObj->arfcn, 
                           ueCb->hrlMeasCfg.utraTddMeasObjLst);
               }
            }
            if(utraMeasObj == NULLP)
            {
               RLOG0(L_ERROR, "utraMeasObj is NULL");
               RETVALUE(RFAILED);
            }

            nhuMeasUtra = &nhuMeasObj[idCnt].measObject.val.measObjectUTRA;
            WR_FILL_TKN_UINT(nhuMeasObj[idCnt].measObject.choice, MEASOBJECT_MEASOBJECTUTRA)
               if(ROK != wrUmmMeasFillUtraNhuAddIdLst(memCp, measTransCb, utraMeasObj, nhuMeasUtra, newPsc, ueCb))
               {
                  RLOG0(L_ERROR, "wrUmmMeasFillUtraNhuAddIdLst: Failed.");
                  RETVALUE(RFAILED);
               }
         }
         else if(measObjInfo.nrType == WR_UMM_MEAS_GERAN_FREQ)
         {
            WrUmmMeasGeranMeasObjNode   *geranMeasObj;
            NhuMeasObjectGERAN          *nhuMeasGeran;

            geranMeasObj = measObjInfo.geranObj->geranMeasObj;
            nhuMeasGeran = &nhuMeasObj[idCnt].measObject.val.measObjectGERAN;

            /* If HRL_LAI list consist measObj for this arfcn then fetch from there. */
            if(ueCb->hrlMeasCfg.geranMeasObjLst.count != 0)
            {
               geranMeasObj = wrUmmHrlGeranMeasObjCfg(geranMeasObj, ueCb);
            }
            if(geranMeasObj != NULLP)
            {
               WR_FILL_TKN_UINT(nhuMeasObj[idCnt].measObject.choice, MEASOBJECT_MEASOBJECTGERAN);
               if(ROK != wrUmmMeasFillGeranNhuAddIdLst(memCp, measTransCb, geranMeasObj,nhuMeasGeran))
               {
                  RLOG0(L_ERROR, "wrUmmMeasFillGeranNhuAddIdLst: Failed.");
                  RETVALUE(RFAILED);
               }
            }
            else
            {
               RLOG0(L_ERROR, "wrUmmMeasFillGeranNhuAddIdLst: Geran Meas Obj is NULLP.");
               RETVALUE(RFAILED);
            }
         }
         else if(measObjInfo.nrType == WR_UMM_MEAS_1XCDMA)
         {
            /* Fill the RRC Re-cfg message with the CDMA1xRTT measurment
             * configuration. Following CB is used to build this message
             * 1. measTransCb        --> It stores the measObjId, RptCfgId and MeasId
             * 2. cdma1xRTTMeasObj   --> List of UMM Measurement objects i.e
             *                            list of wrUmmMeasCdmaMeasObjNode structure
             * 2. pciLst             --> List of PCI to be filled in 
             */
            NhuMeasObjectCDMA2000     *nhuMeasCdma; 
            WrUmmMeasCdmaMeasObjNode        *cdmaMeasObj;

            cdmaMeasObj = measObjInfo.cdmaObj->cdmaMeasObj;
            newPci = measObjInfo.cdmaObj->newPci;
            nhuMeasCdma = &nhuMeasObj[idCnt].measObject.val.measObjectCDMA2000;
            WR_FILL_TKN_UINT(nhuMeasObj[idCnt].measObject.choice, MEASOBJECT_MEASOBJECTCDMA2000)

               if(ROK != wrUmmMeasFillCdmaNhuAddIdLst(memCp, measTransCb, cdmaMeasObj, nhuMeasCdma, newPci, ueCb))
               {
                  RLOG0(L_ERROR, "wrUmmMeasFillUtraNhuAddIdLst: Failed.");
                  RETVALUE(RFAILED);
               }
         }
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for to Fill remove Id list 
 * into Nhu measurement Object.
 *
 *  @details*
 *  Function:wrUmmMeasFillNhuRmvIdLst
 *    
 *  Processing steps:
 *      - copy remove list into Nhu measurement Object.
 *  
 *   @param[in] memCp: pointer to Memory control point.
 *   @param[out] nhuMeasCfg: pointer to NHU measurement config object.
 *   @param[in] measTransCb: pointer to measurement transaction CB.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasFillNhuRmvIdLst
(
CmMemListCp                  *memCp,
NhuMeasConfig                *nhuMeasCfg,
WrUmmMeasRmvInfo             *measTransCb
)
{
   U8                        idCnt;
   NhuMeasObjectId           *measIdRmvLst;
   NhuMeasId                 *measObjIdRmvLst;
   NhuReportConfigId         *rptCfgIdRmvLst;
   U8                        measIdRmvCnt;
   U8                        measObjIdRmvCnt;
   U8                        rptCfgIdRmvCnt;

   TRC2(wrUmmMeasFillNhuRmvIdLst)
   measIdRmvCnt = measTransCb->measIdRmvLstCnt;
   measObjIdRmvCnt = measTransCb->measObjIdRmvLstCnt;
   rptCfgIdRmvCnt = measTransCb->rptCfgRmvLstCnt;

   if(0 != measIdRmvCnt)
   {
      WR_FILL_TKN_UINT(nhuMeasCfg->measIdToRemoveLst.noComp, measIdRmvCnt)
      if(ROK != cmGetMem(memCp, measIdRmvCnt * sizeof(NhuMeasObjectId),
                                            (Ptr *)&measIdRmvLst))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuMeasCfg->measIdToRemoveLst.member = measIdRmvLst;
   }
   if(0 != measObjIdRmvCnt)
   {
      WR_FILL_TKN_UINT(nhuMeasCfg->measObjectToRemoveLst.noComp, measObjIdRmvCnt)
      if(ROK != cmGetMem(memCp, measObjIdRmvCnt * sizeof(NhuMeasObjectId), 
                                          (Ptr *)&measObjIdRmvLst))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuMeasCfg->measObjectToRemoveLst.member = measObjIdRmvLst;
   }
   if(0 != rptCfgIdRmvCnt)
   {
      WR_FILL_TKN_UINT(nhuMeasCfg->reportConfigToRemoveLst.noComp, rptCfgIdRmvCnt)
      if(ROK != cmGetMem(memCp, rptCfgIdRmvCnt * sizeof(NhuReportConfigId),
                                           (Ptr *)&rptCfgIdRmvLst))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuMeasCfg->reportConfigToRemoveLst.member = rptCfgIdRmvLst;
   }

      /*Copy Meas Id Remove List into Nhu structure*/
   for(idCnt = 0; idCnt < measIdRmvCnt; idCnt++)
   {
      WR_FILL_TKN_UINT(measIdRmvLst[idCnt], measTransCb->measIdRmvLst[idCnt])
   }
      /*Copy Meas Object Id Remove List into Nhu structure*/
   for(idCnt = 0; idCnt < measObjIdRmvCnt; idCnt++)
   {
      WR_FILL_TKN_UINT(measObjIdRmvLst[idCnt], measTransCb->measObjIdRmvLst[idCnt])
   }
      
      /*Copy report config Id Remove List into Nhu structure*/
   for(idCnt = 0; idCnt < rptCfgIdRmvCnt; idCnt++)
   {
      WR_FILL_TKN_UINT(rptCfgIdRmvLst[idCnt], measTransCb->rptCfgRmvLst[idCnt])
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to Fill Nhu measurement Id object.
 *
 *  @details*
 *  Function:wrUmmMeasFillMeasIdObj
 *    
 *  Processing steps:
 *      - search in meas id list using measId.
 *      - If measId object found copy into Nhu structure
 *  
 *   @param[in] measID: measurement Id.
 *   @param[out] measIdAddMod: pointer to Nhu measurement Id Object.
 *   @param[in] measIdLst: pointer to measurement Id List.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16  wrUmmMeasFillMeasIdObj
(
U8                           measID,
NhuMeasIdToAddMod            *measIdAddMod,
CmLListCp                    *measIdLst
)
{
   WrMeasIdNode              *measIdNode;
   CmLList                   *measIdLnk;

   TRC2(wrUmmMeasFillMeasIdObj)
   measIdLnk = measIdLst->first;

   while(NULLP != measIdLnk)
   {
      measIdNode = (WrMeasIdNode*)measIdLnk->node;
      if(measIdNode->measID == measID)
      {
         wrFillTknU8(&measIdAddMod->pres, 1);
         WR_FILL_TKN_UINT(measIdAddMod->measId, measIdNode->measID)
         WR_FILL_TKN_UINT(measIdAddMod->measObjectId, measIdNode->measObjID)
         WR_FILL_TKN_UINT(measIdAddMod->reportConfigId, measIdNode->rptCfgID)
         RETVALUE(ROK);
      }
      measIdLnk = measIdLnk->next;
   }

   RETVALUE(RFAILED);
}
/** @brief This function is responsible for to get report config Object from
 * report config List.
 *
 *  @details
 *  Function:wrUmmMeasFillRptCfgObj
 *    
 *  Processing steps:
 *      - Traverse List and find report config Object.
 *        
 *   @param[in] rptCfgId: pointer to report config Id.
 *   @param[in] nhuRptCfgObj: Nhu Report config Object.
 *   @param[out] rptCfgLst: Report config List.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PUBLIC S16 wrUmmMeasFillRptCfgObj
(
U8                                  rptCfgId,
NhuReportConfigToAddModreportConfig *nhuRptCfgObj,
CmLListCp                           *rptCfgLst
)
{
   WrMeasRptCfgNode          *rptCfgObj;
   CmLList                   *rptCfgNode;

   
   TRC2(wrUmmMeasFillRptCfgObj)
   rptCfgNode = rptCfgLst->first;

   while (NULLP != rptCfgNode)
   {
      rptCfgObj = (WrMeasRptCfgNode*)rptCfgNode->node;
      if(rptCfgObj->rptCfgId == rptCfgId)
      {
         if (rptCfgObj->rptCfgType == WR_UMM_MEAS_EVENT_A1)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice, REPORTCONFIG_REPORTCONFIGEUTRA)
            
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigEUTRA,(U8*)&rptCfgObj->val.
                 eventA124, sizeof(NhuReportConfigEUTRA));
            break;
         }
         if (rptCfgObj->rptCfgType == WR_UMM_MEAS_EVENT_A2)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice, REPORTCONFIG_REPORTCONFIGEUTRA)
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigEUTRA,(U8*)&rptCfgObj->val.
                  eventA124, sizeof(NhuReportConfigEUTRA));
            break;
         }
         if (rptCfgObj->rptCfgType == WR_UMM_MEAS_EVENT_A3)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice, REPORTCONFIG_REPORTCONFIGEUTRA)
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigEUTRA,(U8*)&rptCfgObj->val.
                  eventA3, sizeof(NhuReportConfigEUTRA));
            break;
         }
         if (rptCfgObj->rptCfgType == WR_UMM_MEAS_EVENT_A5)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice, REPORTCONFIG_REPORTCONFIGEUTRA)
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigEUTRA,(U8*)&rptCfgObj->val.
                     eventA5, sizeof(NhuReportConfigEUTRA));
            break;
         }
         if (rptCfgObj->rptCfgType == WR_UMM_MEAS_EVENT_B2)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice ,REPORTCONFIG_REPORTCONFIGINTERRAT)
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigInterRAT, (U8*)&rptCfgObj->val.
                   eventB2, sizeof(NhuReportConfigInterRAT));
            break;
         }
         if (rptCfgObj->rptCfgType == WR_UMM_MEAS_EVENT_B2_GERAN)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice ,REPORTCONFIG_REPORTCONFIGINTERRAT)
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigInterRAT, (U8*)&rptCfgObj->val.
                   eventB2, sizeof(NhuReportConfigInterRAT));
            break;
         }

         if ( WR_UMM_MEAS_EVENT_B2_UTRA == rptCfgObj->rptCfgType)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice ,REPORTCONFIG_REPORTCONFIGINTERRAT)
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigInterRAT, (U8*)&rptCfgObj->val.
                   eventB2, sizeof(NhuReportConfigInterRAT));
            break;
         }
         if (rptCfgObj->rptCfgType == WR_UMM_MEAS_STRONG_CELL)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice, REPORTCONFIG_REPORTCONFIGEUTRA)
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigEUTRA,(U8*)&rptCfgObj->
                  val.strgCell, sizeof(NhuReportConfigEUTRA));
            break;
         }
         if (rptCfgObj->rptCfgType == WR_UMM_MEAS_STRONG_CELL_ANR)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice, REPORTCONFIG_REPORTCONFIGINTERRAT)
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigInterRAT,(U8*)&rptCfgObj->
                  val.strgCellAnr, sizeof(NhuReportConfigInterRAT));
            break;
         }
         if (rptCfgObj->rptCfgType == WR_UMM_MEAS_UTRA_STRONG_CELL_ANR)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice, REPORTCONFIG_REPORTCONFIGINTERRAT)
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigInterRAT,(U8*)&rptCfgObj->
                  val.utraStrgCellAnr, sizeof(NhuReportConfigInterRAT));
            break;
         }
         if (rptCfgObj->rptCfgType == WR_UMM_MEAS_UTRA_STRONG_CELL_CSFB)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice, REPORTCONFIG_REPORTCONFIGINTERRAT)
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigInterRAT,(U8*)&rptCfgObj->
                  val.strgCellCsfb, sizeof(NhuReportConfigInterRAT));
            break;
         }
/* LTE_ADV_ABS starts */
         if (rptCfgObj->rptCfgType == WR_UMM_MEAS_EVENT_A3_ABS)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice, REPORTCONFIG_REPORTCONFIGEUTRA)
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigEUTRA,(U8*)&rptCfgObj->val.
                  eventA3, sizeof(NhuReportConfigEUTRA));
            break;
         }
/* LTE_ADV_ABS ends */
         if (rptCfgObj->rptCfgType == WR_UMM_MEAS_CDMA_STRONG_CELL)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice, REPORTCONFIG_REPORTCONFIGINTERRAT)
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigInterRAT,(U8*)&rptCfgObj->
                  val.interRatStrgCell, sizeof(NhuReportConfigInterRAT));
            break;
         }
         if(WR_UMM_MEAS_GERAN_STRONG_CELL_CSFB == rptCfgObj->rptCfgType)
         {
            WR_FILL_TKN_UINT(nhuRptCfgObj->choice, REPORTCONFIG_REPORTCONFIGINTERRAT);
            cmMemcpy((U8*)&nhuRptCfgObj->val.reportConfigInterRAT,
                  (U8*)&rptCfgObj->val.strgCellCsfb, 
                  sizeof(NhuReportConfigInterRAT));
            break;

         }
      } 
      rptCfgNode = rptCfgNode->next;
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for to Update Nhu measurement object
 * from reconfig message.
 * 
 *  @details
 * Function:wrUmmMeasGetNhuMeasCfgFrmReCfg
 *
 *   Processing steps:
 *    - Update Nhu measurement object
 *     
 * @param[in]  rrcReCfgMsg: pointer to reconfig Message.
 * @param[in]  nhuMeasCfg: pointer to Nhu measurement object Message.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PUBLIC Void wrUmmMeasGetNhuMeasCfgFrmReCfg
(
 NhuDatReqSdus                *rrcReCfgMsg,
 NhuMeasConfig                **nhuMeasCfg
 )
{
   NhuDL_DCCH_Msg                       *dcchMsg;
   NhuRRCConRecfgn                      *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs               *recfgIEs;

   TRC2(wrUmmMeasGetNhuMeasCfgFrmReCfg);

   dcchMsg     = &(rrcReCfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   recfgIEs    = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   *nhuMeasCfg  = &(recfgIEs->measConfig);
}

   /** @brief This function is responsible for to Update Nhu MacMain config
    * from reconfig message.
    * 
    *  @details
    * Function:wrUmmMeasGetNhuMacMainCfgFrmReCfg
    *
    *   Processing steps:
    *    - Update Nhu Mac main config
    *     
    * @param[in]  rrcReCfgMsg: pointer to reconfig Message.
    * @param[in]  nhuMeasCfg: pointer to Nhu measurement object Message.
    * @return S16
    *       SUCCESS : ROK
    *       FAILED  : RFAILED
    **/
   PUBLIC Void wrUmmMeasGetNhuMacMainCfgFrmReCfg
      (
       NhuDatReqSdus                *rrcReCfgMsg,
       NhuMAC_MainConfig            **macCfg
      )
      {
         NhuDL_DCCH_Msg                   *dcchMsg;
         NhuRRCConRecfgn                  *rrcConReCfg;
         NhuRRCConRecfgn_r8_IEs           *recfgIEs;
         NhuRadioResourceConfigDedicated  *rrcCfgDed;

         TRC2(wrUmmMeasGetNhuMacMainCfgFrmReCfg)

            dcchMsg     = &(rrcReCfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
         rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
         recfgIEs    = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
         rrcCfgDed   = &(recfgIEs->radioResourceConfigDedicated);
         *macCfg      = &(rrcCfgDed->mac_MainConfig.val.explicitValue);

         rrcCfgDed->srb_ToAddModLst.noComp.pres = NOTPRSNT;
         rrcCfgDed->drb_ToAddModLst.noComp.pres = NOTPRSNT;
         rrcCfgDed->drb_ToRlsLst.noComp.pres = NOTPRSNT;
         rrcCfgDed->sps_Config.pres.pres = NOTPRSNT;
         rrcCfgDed->physicalConfigDedicated.pres.pres = NOTPRSNT;

         wrFillTknU8(&(rrcCfgDed->pres), PRSNT_NODEF);
         wrFillTknU8(&(rrcCfgDed->mac_MainConfig.choice), 0);
      }
/** @brief This function is responsible for to Fill Nhu measurement Object.
 *
 *  @details*
 *  Function:wrMeasFillNhuMsg
 *  
 *  Processing steps:
 *      - Get the Measurement Id's from measurement transaction control block.
 *      - Using Id's get the measurement objects from UE Control block.
 *      - copy into Nhu measurement Object.
 *  
 *   @param[in]  memCp: pointer to memory control pointer.
 *   @param[out]  rrcReCfgMsg: pointer to RRC reconfig message.
 *   @param[in]  measUeCb: pointer to measurement Ue control block.
 *   @param[in]  measCfgInfo: pointer to measurement transaction control block.
 *   @param[in]  measCellCfg: pointer to measurement cell config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrMeasFillNhuMsg
(
  CmMemListCp                  *memCp,
  NhuMeasConfig                *nhuMeasCfg,
  WrUeCb                       *ueCb,
  WrUmmMeasTransInfo           *measCfgInfo,
  LwrCellMeasCfgGrp            *measCellCfg,
  U32                          transId
)
{
   U8                                   idCnt;
   U8                                   rptCfgID;
   U8                                   measID;
   U8                                   measIdCnt = 0;
   U8                                   measIdNotCfgCnt = 0;
   U8                                   rptCfgCnt = 0;
   WrMeasUeCb                           *measUeCb = ueCb->measCfg;
   NhuReportConfigToAddMod              *rptCfg;
   NhuReportConfigToAddModreportConfig  *nhuRptCfg;
   NhuMeasIdToAddMod                    *measIdAddMod;

   TRC2(wrMeasFillNhuMsg)
     
   nhuMeasCfg->pres.pres = PRSNT_NODEF;
   
   if ((0 != measCfgInfo->rmvMeasInfo.measIdRmvLstCnt) ||
       (0 != measCfgInfo->rmvMeasInfo.measObjIdRmvLstCnt) ||
       (0 != measCfgInfo->rmvMeasInfo.rptCfgRmvLstCnt))
   {
      if(ROK != wrUmmMeasFillNhuRmvIdLst(memCp, nhuMeasCfg, &measCfgInfo->rmvMeasInfo))
      {
         RLOG0(L_ERROR, "wrUmmMeasFillNhuRmvIdLst Failed.");
         RETVALUE(RFAILED);
      }
   }
   if(ROK != wrUmmMeasFillNhuAddIdLst(memCp, nhuMeasCfg, ueCb, &measCfgInfo->addMeasInfo))
   {
       RLOG0(L_ERROR, "wrUmmMeasFillNhuAddIdLst Failed.");
       RETVALUE(RFAILED);
   }

   measIdCnt = measCfgInfo->addMeasInfo.measIdCnt;
   rptCfgCnt = measCfgInfo->addMeasInfo.rptCfgIdCnt;

   if(0 != rptCfgCnt)
   {
      if(ROK != cmGetMem(memCp, rptCfgCnt * sizeof(NhuReportConfigToAddMod),
                                              (Ptr *)&rptCfg))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuMeasCfg->reportConfigToAddModLst.member = rptCfg;
      WR_FILL_TKN_UINT(nhuMeasCfg->reportConfigToAddModLst.noComp, rptCfgCnt)
   }
   if(0 != measIdCnt)
   {
      if(ROK != cmGetMem(memCp, measIdCnt * sizeof(NhuMeasIdToAddMod), (Ptr *)&measIdAddMod))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuMeasCfg->measIdToAddModLst.member = measIdAddMod;

      WR_FILL_TKN_UINT(nhuMeasCfg->measIdToAddModLst.noComp, measIdCnt)
   }
   /*Copy report config into Nhu structure*/
   for(idCnt = 0; idCnt < rptCfgCnt; idCnt++)
   {
      rptCfgID = measCfgInfo->addMeasInfo.rptCfgLst[idCnt];
      wrFillTknU8(&rptCfg[idCnt].pres, 1);
      WR_FILL_TKN_UINT(rptCfg[idCnt].reportConfigId, rptCfgID)
      nhuRptCfg = &rptCfg[idCnt].reportConfig;
      wrUmmMeasFillRptCfgObj(rptCfgID, nhuRptCfg, &measUeCb->rptCfgLst);
   }
      /*Copy Meas Id into Nhu structure*/
   for(idCnt = 0; idCnt < measIdCnt; idCnt++)
   {
      measID = measCfgInfo->addMeasInfo.measIdLst[idCnt];
      if(ROK != wrUmmMeasFillMeasIdObj(measID, &measIdAddMod[idCnt], &measUeCb->measIdLst))
      {
         RLOG0(L_WARNING, "wrUmmMeasFillMeasIdObj Failed.");
         measIdNotCfgCnt++;
      }
   }
   if(measIdNotCfgCnt != 0)
   {
      WR_FILL_TKN_UINT(nhuMeasCfg->measIdToAddModLst.noComp, (measIdCnt - measIdNotCfgCnt))
   }
   
   /* Configuring S-Measure, Quantity and Mobility Params */
   if(measUeCb->isQuantityCfged == FALSE)
   {
      wrUmmMeasFillQtySpdStatCfg(memCp, nhuMeasCfg, measCellCfg,&ueCb->ueCap->eUTRA_Cap);/*rel10*/
      measUeCb->isQuantityCfged = TRUE;
   }

   /* Configuring Meas Gap config */
   if(measUeCb->gapMeasInfo.latestTxId == transId)
   {
      /* Update the gap period and gap offset received from 
         Meas gap API 
      */
      wrUmmMeasFillMeasGapCfg(measCfgInfo->measGapStatus, measUeCb,
                                &nhuMeasCfg->measGapConfig);
   }
   
   if(measCfgInfo->addMeasInfo.isEutraRptCgiFlg == TRUE)
   {
      if(ROK != cmGetMem(memCp, sizeof(NhuReportConfigToAddMod), (Ptr *)&rptCfg))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuMeasCfg->reportConfigToAddModLst.member = rptCfg;
      if(NULLP != measUeCb->eutraRptCgiCfg)
      {
         WR_FILL_TKN_UINT(nhuMeasCfg->reportConfigToAddModLst.noComp, 1)
         rptCfgID = measUeCb->eutraRptCgiCfg->rptCfgId;
         wrFillTknU8(&rptCfg[0].pres, 1);
         WR_FILL_TKN_UINT(rptCfg[0].reportConfigId, rptCfgID)
         nhuRptCfg = &rptCfg[0].reportConfig;
         WR_FILL_TKN_UINT(nhuRptCfg->choice, REPORTCONFIG_REPORTCONFIGEUTRA)

         cmMemcpy((U8*)&nhuRptCfg->val.reportConfigEUTRA,(U8*)&measUeCb->
                 eutraRptCgiCfg->val.eutraRptCgi, sizeof(NhuReportConfigEUTRA));
      }
   }
   
   /* Configuring Report CGI for Inter RAT*/
   if(measCfgInfo->addMeasInfo.isUtraRptCgiFlg == TRUE)
   {
      if(ROK != cmGetMem(memCp, sizeof(NhuReportConfigToAddMod), (Ptr *)&rptCfg))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuMeasCfg->reportConfigToAddModLst.member = rptCfg;
      if(NULLP != measUeCb->utraRptCgiCfg)
      {
         WR_FILL_TKN_UINT(nhuMeasCfg->reportConfigToAddModLst.noComp, 1)
         rptCfgID = measUeCb->utraRptCgiCfg->rptCfgId;
         wrFillTknU8(&rptCfg[0].pres, 1);
         WR_FILL_TKN_UINT(rptCfg[0].reportConfigId, rptCfgID)
         nhuRptCfg = &rptCfg[0].reportConfig;
         WR_FILL_TKN_UINT(nhuRptCfg->choice, REPORTCONFIG_REPORTCONFIGINTERRAT)
         cmMemcpy((U8*)&nhuRptCfg->val.reportConfigInterRAT, (U8*)&measUeCb->
           utraRptCgiCfg->val.utraRptCgi, sizeof(NhuReportConfigInterRAT));
      }
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for configuring measurement
 * configuration for UE at the time of initial context setup request procedure
 * and incoming handover scenario.
 * 
 * @details
 *
 * Function:wrUmmMeasInitialUeCfg
 *
 *   Processing steps:
 *    - Verify in Ue Capabilities whether UE will support ANR intra Frequency or not.
 *    - If Ue supports intra frequency configure Event A2 and Strongest cell
 *    report configuration.
 *    - Verify in Ue Capabilities whether UE will support Inter Frequency or not.
 *    - If Ue supports inter frequency configure Strongest cell report
 *    configuration.
 *    - Configure S-Measure, Mobility and Speed state params
 *    - Fill Nhu measurement config structure
 * 
 * @param[in]  memCp: pointer to memory control pointer.
 * @param[in]  ueCb: pointer to Ue control block.
 * @param[in]  measCfgInfo: pointer to Measurement transaction control block 
 * @param[out]  rrcReCfgMsg: pointer to RRC Reconfigaration message 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasInitialUeCfg 
(
  CmMemListCp              *memCp,
  WrUeCb                   *ueCb,
  WrUmmMeasTransInfo       *measCfgInfo,
  NhuMeasConfig            *nhuMeasCfg,
  U32                       transId,
  MeasInitCause             cause
)
{
   LwrCellMeasCfgGrp         *measCellCfg = NULLP;
   WrUmmCellCb               *cellCb;
  
   TRC2(wrUmmMeasInitialUeCfg)
      
   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }
   measCellCfg = cellCb->measCellCfg; 

   if((NULLP != nhuMeasCfg) && (NULLP != measCfgInfo))
   {
      /*intra Freq Cell support by UE : Check in Ue capability*/
      wrUmmMeasCreatIntraInitalCfgMsg(cellCb, &measCfgInfo->addMeasInfo, ueCb, cause);

      /* Configuring Meas Objects and report configuration List*/
      if(ROK != wrMeasFillNhuMsg (memCp, nhuMeasCfg, ueCb, measCfgInfo, measCellCfg, transId))
      {
         RLOG0(L_ERROR, "wrMeasFillNhuMsg failed");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}
/** @brief This function is responsible for to configure measurement config after
 * receiving Event A2 measurement report.
 *
 *  @details*
 *  Function:wrUmmMeasIntraCfgA1AfrRecvA2Rpt
 *    
 *  Processing steps:
 *      - For Intra frequency Objects Configure Event A1 and Event A5 
 *  
 *   @param[in] measUeCb: pointer to measurement ue Cb.
 *   @param[in] addMeasInfo: pointer to measurement transaction Cb.
 *   @param[in] cellCb: pointer to cell control block.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasIntraCfgA1AfrRecvA2Rpt
(
 WrUeCb                      *ueCb,/*rel10*/
 WrUmmMeasAddInfo            *addMeasInfo,
 WrUmmCellCb                 *cellCb
)
{
   U8                        measObjId;
   LwrCellMeasCfgGrp         *measCellCfg;
/* LTE_ADV starts */
   WrCellCb                  *emmCellCb;
/* LTE_ADV ends */
   WrUmmMeasRptCfgInfo       rptCfgInfo;
   /*rel10*/
   WrMeasUeCb                *measUeCb = ueCb->measCfg;
   
   TRC2(wrUmmMeasIntraCfgA1AfrRecvA2Rpt)
   measObjId = measUeCb->intraMeasObj.measObjId;
   
   measCellCfg = cellCb->measCellCfg;
/* LTE_ADV starts */
   WR_GET_CELLCB(emmCellCb, measUeCb->cellId)
   if(emmCellCb == NULLP)
   {
      RLOG1(L_ERROR, " CELLCB not found for given cellId(%d) ", (U16)measUeCb->cellId);
      RETVALUE(RFAILED);         /* KW fix for LTE_ADV */
   }   
/* LTE_ADV ends */
   /* Report Config for Event A1 */
   rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_A1;
   if(!measCellCfg->measA1EvtRptCfg[WR_MEAS_EVENTA1_HO_RSRP].triggerQty)
   {
      rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
   }
   else
   {
      rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRQ;
   }
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;
   if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_HO, measObjId,
            &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
   {
      RLOG0(L_ERROR, "Invalid report and Meas config Id");
      RETVALUE(RFAILED);
   }
   RLOG_ARG0(L_INFO, DBG_CELLID,(U32)cellCb->cellId,"Received Measurement "
      "report, Event A1 Configured");


/* LTE_ADV_ABS starts */
   if((WR_LTE_ADV_PICO_ENB == emmCellCb->lteAdvCb.lteAdvCfg.enbType) &&
      (emmCellCb->lteAdvCb.lteAdvCfg.absCfg.status & RGR_ENABLE))
   {
      wrUmmIntraMeasCfgForLteAdvAddEvntA3(ueCb, addMeasInfo, 
                                      cellCb, WR_UMM_MEAS_EVENT_A3_ABS);   
   }
/* LTE_ADV_ABS ends */
   RETVALUE(ROK);
}

/** @brief This function is responsible for to update measurement object and Id
 *  from ue cb. 
 *
 *  @details
 * Function:wrUmmMeasUpdtEutraMeasObjAndIdFromArfcn
 *
 *   Processing steps:
 *    - tranverse cell Cb list find measurement object and id using Arfcn. 
 *     
 * @param[in]  arfcn: Arfcn         
 * @param[in]  measUeCb: pointer to measurement Ue control block         
 * @param[out]  measObjId: pointer to measurement object Id         
 * @param[out]  cellCbMeasObj: pointer to cell Cb measurement Object 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PUBLIC S16 wrUmmMeasUpdtEutraMeasObjAndIdFromArfcn
(
U32                          arfcn,
WrMeasUeCb                   *measUeCb,
U8                           *measObjId,
WrUmmMeasEutraMeasObjNode    **cellCbMeasObj
)
{
   CmLList                       *measObjEutraNode;
   WrUmmMeasEutraMeasObjNode     *cellCbEMeasObj;
   WrMeasUeCbEutraMeasObjNode    *ueCbEMeasObj;
   U8                            isObjPres = FALSE;

   TRC2(wrUmmMeasUpdtEutraMeasObjAndIdFromArfcn)

   if(measUeCb->intraMeasObj.eutraMeasObj == NULLP)
   {
      RETVALUE(RFAILED);
   }
   if(arfcn == measUeCb->intraMeasObj.eutraMeasObj->carrFreq)
   {
      *measObjId = measUeCb->intraMeasObj.measObjId;
      *cellCbMeasObj = measUeCb->intraMeasObj.eutraMeasObj;
      isObjPres = TRUE;
   }
   else
   {
      measObjEutraNode = measUeCb->interMeasObjLst.first;
      while (NULLP != measObjEutraNode)
      {
         ueCbEMeasObj = ((WrMeasUeCbEutraMeasObjNode*)measObjEutraNode->node);
         cellCbEMeasObj = ueCbEMeasObj->eutraMeasObj;
         if (arfcn == cellCbEMeasObj->carrFreq)
         {
            *measObjId = ueCbEMeasObj->measObjId;
            *cellCbMeasObj = cellCbEMeasObj;
            isObjPres = TRUE;
            break;
         }
         measObjEutraNode = measObjEutraNode->next;
      }
   }
   if(isObjPres == FALSE)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      RETVALUE(ROK);
   }
}
/** @brief This function is responsible for to update UTRA measurement object
 * and Id from ue cb. 
 *
 *  @details
 * Function:wrUmmMeasUpdtUtraMeasObjAndIdFromArfcn
 *
 *   Processing steps:
 *    - tranverse cell Cb list find measurement object and id using Arfcn. 
 *     
 * @param[in]  arfcn: Arfcn         
 * @param[in]  measUeCb: pointer to measurement Ue control block         
 * @param[out]  measObjId: pointer to measurement object Id         
 * @param[out]  cellCbMeasObj: pointer to cell Cb measurement Object 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PUBLIC S16 wrUmmMeasUpdtUtraMeasObjAndIdFromArfcn
(
U32                          arfcn,
WrMeasUeCb                   *measUeCb,
U8                           *measObjId,
WrUmmMeasUtraMeasObjNode     **cellCbMeasObj
)
{
   CmLList                       *measObjUtraNode;
   WrUmmMeasUtraMeasObjNode      *cellCbUMeasObj;
   WrMeasUeCbUtraMeasObjNode     *ueCbUMeasObj;

   TRC2(wrUmmMeasUpdtUtraMeasObjAndIdFromArfcn)

   measObjUtraNode = measUeCb->utraFddMeasObjLst.first;
   while (NULLP != measObjUtraNode)
   {
      ueCbUMeasObj = ((WrMeasUeCbUtraMeasObjNode*)measObjUtraNode->node);
      cellCbUMeasObj = ueCbUMeasObj->utraMeasObj;
      if (arfcn == cellCbUMeasObj->arfcn)
      {
         *measObjId = ueCbUMeasObj->measObjId;
         *cellCbMeasObj = cellCbUMeasObj;
         RETVALUE(ROK);
      }
      measObjUtraNode = measObjUtraNode->next;
   }

   measObjUtraNode = measUeCb->utraTddMeasObjLst.first;
   while (NULLP != measObjUtraNode)
   {
      ueCbUMeasObj = ((WrMeasUeCbUtraMeasObjNode*)measObjUtraNode->node);
      cellCbUMeasObj = ueCbUMeasObj->utraMeasObj;
      if (arfcn == cellCbUMeasObj->arfcn)
      {
         *measObjId = ueCbUMeasObj->measObjId;
         *cellCbMeasObj = cellCbUMeasObj;
         RETVALUE(ROK);
      }
      measObjUtraNode = measObjUtraNode->next;
   }

   RETVALUE(RFAILED);
}
/** @brief This function is responsible for to update GERAN measurement object
 * and Id from ue cb. 
 *
 *  @details
 * Function:wrUmmMeasUpdtGeranMeasObjAndIdFromArfcn
 *
 *   Processing steps:
 *    - tranverse cell Cb list find measurement object and id using Arfcn. 
 *     
 * @param[in]  measUeCb: pointer to measurement Ue control block         
 * @param[out]  measObjId: pointer to measurement object Id         
 * @param[out]  cellCbMeasObj: pointer to cell Cb measurement Object 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PUBLIC S16 wrUmmMeasUpdtGeranMeasObjAndIdFromArfcn
(
WrMeasUeCb                   *measUeCb,
U8                           *measObjId,
WrMeasUeCbGeranMeasObjNode   **cellCbMeasObj
)
{
   CmLList                       *measObjGeranNode;
   WrMeasUeCbGeranMeasObjNode     *ueCbMeasObj;

   TRC2(wrUmmMeasUpdtGeranMeasObjAndIdFromArfcn)

   measObjGeranNode = measUeCb->geranMeasObjLst.first;
   while (NULLP != measObjGeranNode)
   {
      ueCbMeasObj = ((WrMeasUeCbGeranMeasObjNode*)measObjGeranNode->node);
      if (ueCbMeasObj != NULLP)
      {
         *measObjId = ueCbMeasObj->measObjId;
         *cellCbMeasObj = ueCbMeasObj;
         RETVALUE(ROK);
      }
      measObjGeranNode = measObjGeranNode->next;
   }

   RETVALUE(RFAILED);
}
/** @brief This function is responsible for to configure measurement config after
 * receiving Event A2 measurement report.
 *
 *  @details*
 *  Function:wrUmmMeasInterCfgAfrRecvA2Rpt
 *    
 *  Processing steps:
 *      - For Inter frequency Objects Configure Event A5 
 *  
 *   @param[in] measUeCb: pointer to measurement ue Cb.
 *   @param[in] addMeasInfo: pointer to measurement transaction Cb.
 *   @param[in] cellCb: pointer to cell control block.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasInterCfgAfrRecvA2Rpt
(
 WrUeCb                      *ueCb,
 WrUmmMeasAddInfo            *addMeasInfo,
 WrUmmCellCb                 *cellCb,
 Bool                        *isGapRqd
)
{
   U8                           measObjId;
   U8                           servingBand;
   U8                           targetBand;
   U16                          servFreq;
   Bool                         isUeSupported = FALSE;
   Bool                         isGapReqd = FALSE;
   CmLList                      *interMeasObj;
   WrUmmMeasEutraMeasObjNode    *eutraCellObj;
   LwrCellMeasCfgGrp            *measCellCfg;
   WrMeasUeCb                   *measUeCb;
   WrUmmMeasCellCb              *measCellCb;
   WrUmmMeasRptCfgInfo          rptCfgInfo;
   S16                          isUeCapable = FALSE;
   S16                          isGapCfgd = FALSE;

   TRC2(wrUmmMeasInterCfgAfrRecvA2Rpt)

   measCellCb = cellCb->measCellCb;
   measUeCb = ueCb->measCfg;
   servFreq = wrEmmGetServCarrierFreq(ueCb->cellId);
   /*Get serving bandclass */
   wrUmmFndEUTRABand(servFreq,&servingBand);

   measCellCfg = cellCb->measCellCfg;
   interMeasObj = measCellCb->interMeasObjLst.first;
   /*Added for KPI*/
#ifdef WR_RSYS_KPI 
   ueCb->measGapUsd = FALSE;
#endif/*WR_RSYS_KPI*/ 
   /* Check UE capabilites, before configuring A5 ANR measurement report. */ 
   isUeCapable = wrUmmAnrChkUeCap(ueCb, WR_UMM_MEAS_INTER_FREQ , WR_UMM_EVENT_A5_INTER_CFG);
   while (NULLP != interMeasObj)
   {
      /*Meas Objects for inter frequencies*/
      eutraCellObj = (WrUmmMeasEutraMeasObjNode*)interMeasObj->node;

      /*Get bandclass for corresponding ARFCN*/
      wrUmmFndEUTRABand(eutraCellObj->carrFreq,&targetBand);

      /*Check whether the bandclass is UE supported or not*/
      isUeSupported = wrUmmEUTRABandSupported(ueCb,targetBand, 0);

      if(isUeSupported == TRUE)
      { 
         if(isGapCfgd == FALSE)
         {
            /* Check the Measurement gap is required for the 
             Inter Frequency or not */
            wrUmmMeasGapReqdInterfreq(ueCb,servingBand,targetBand, &isGapReqd);

            if(isGapReqd == TRUE)
            {
               /* Irrespective of the neighbor frequency this feature shall configure 
                meas gap with same pattern. So configuring the gap only once would be 
                sufficent */
               isGapCfgd  = TRUE;
            }
         }
         RLOG1(L_DEBUG,"Configuring Inter frequency measurements for UE Crnti [%d] \n", ueCb->crnti); 
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
         if(TRUE == wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti,
                  WR_UMM_A4_A5_MEAS_SUPPORTED))
         {
            /*ccpu00133426 - inter freq ho requirement - end*/
            rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_A5;
            rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
            rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
            rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_INTER_RSRP;
            /*ccpu00133426 - inter freq ho requirement - start*/
         }
         else
         {
            RLOG0(L_DEBUG, " BIT 14 in FGI is Zero,Hence Configuring A3 for INTER ");
            
            rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_A3;
            rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
            rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
            rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_INTER_RSRP;
         }
         /*ccpu00133426 - inter freq ho requirement - end*/
         /* Report Config for Event A3/A5*/
         if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_HO, measObjId,
             &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
         {
             RLOG0(L_ERROR, "Invalid report and Meas config Id");
             RETVALUE(RFAILED);
         }
            /* If UE is capable of Inter-Freq ANR and Configured algo is A5 evet 
             * based Inter-freq ANR, then only configure A5 ANR measurment report */
         if((ueCb->isGBRBearerExists == NOT_IN_USE) &&
            (TRUE == isUeCapable) && 
            (measCellCb->anrRptCfg == WR_UMM_EVENT_A5_INTER_CFG))
         {
               rptCfgInfo.rptCfgTyp      = WR_UMM_MEAS_EVENT_A5;
               rptCfgInfo.thrshldTyp     = WR_UMM_MEAS_THRESHOLD_RSRP;
               rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
               rptCfgInfo.intRptCfgType  = WR_UMM_MEAS_RPTCFG_INTER_ANR;

               if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_EVENT_ANR, 
                     measObjId, &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
               {
                  RLOG0(L_ERROR, " Invalid report and Meas config Id ");
                  RETVALUE(RFAILED);
               }
            }
         }
      interMeasObj = interMeasObj->next;
   }
   *isGapRqd = isGapCfgd;
   RETVALUE(ROK);
}

/** @brief This function is responsible for to get report config Type from
 * report config List.
 * 
 *  @details
 *  Function:wrUmmMeasFindRptCfgTyp
 *  
 *    Processing steps:
 *      - Traverse List and find report config Type.
 *        
 *   @param[in] rptCfgId: pointer to report config Id.
 *   @param[out] rptCfgTyp: Report config Type.
 *   @param[in] rptCfgLst: Report config List.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PUBLIC S16 wrUmmMeasFindRptCfgTyp
(
U8                           rptCfgId,
WrMeasRptCfgType             *rptCfgTyp,
CmLListCp                    *rptCfgLst
)
{
   WrMeasRptCfgNode          *rptCfgObj;
   CmLList                   *rptCfgNode;
   
   TRC2(wrUmmMeasFindRptCfgTyp)
   rptCfgNode = rptCfgLst->first;

   while (NULLP != rptCfgNode)
   {
      rptCfgObj = (WrMeasRptCfgNode*)rptCfgNode->node;
      if(rptCfgObj->rptCfgId == rptCfgId)
      {
          *rptCfgTyp = rptCfgObj->rptCfgType;
          break;
      } 
      rptCfgNode = rptCfgNode->next;
   }
   RETVALUE(ROK);
}

/** @brief: This function is responsible for internal report config type
 *          from report config list.
 * */
PRIVATE S16 wrUmmMeasFindIntRptCfgTyp
(
U8                           rptCfgId,
WrUmmMeasIntRptCfgTyp        *intRptCfgTyp,
CmLListCp                    *rptCfgLst
 )
{
   WrMeasRptCfgNode          *rptCfgObj;
   CmLList                   *rptCfgNode;
   
   TRC2(wrUmmMeasFindRptCfgTyp)
   rptCfgNode = rptCfgLst->first;
   while (NULLP != rptCfgNode)
   {
      rptCfgObj = (WrMeasRptCfgNode*)rptCfgNode->node;
      if(rptCfgObj->rptCfgId == rptCfgId)
      {
          *intRptCfgTyp = rptCfgObj->intRptCfgTyp;
          break;
      } 
      rptCfgNode = rptCfgNode->next;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to add measurement config for
 * reception of event A1 measurement report
 *
 *  @details
 *  Function:wrUmmMeasCfgForEvntA1
 *    
 *  Processing steps:
 *      - Configure Event A2
 *      - delete Event A1 and Event A5 and Event B2 
 *      
 *   @param[in] measUeCb: measurement Ue control block.
 *   @param[out] measTransCb: pointer to measurement transaction Cb.
 *   @param[in] measCellCfg: pointer to measurement cell config.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILURE :RFAILED
*/
PRIVATE S16 wrUmmMeasCfgForEvntA1
(
WrUeCb                       *ueCb,
WrUmmMeasRmvInfo             *rmvMeasInfo,
WrUmmMeasAddInfo             *addMeasInfo,
LwrCellMeasCfgGrp            *measCellCfg
)
{
   U8                        measId;
   U8                        measObjId;
   U8                        rptCfgId = 0;
   U8                        a1RptCfgId = 0;
   U8                        a5RptCfgId = 0;
   U8                        a5AnrRptCfgId = 0;
   U8                        b2RptCfgId = 0;
   U8                        b2RptCfgIdforIRat = 0;
   U8                        b2RptCfgIdforGeranRedir = 0;
   U8                        b2RptCfgIdforGeranHo = 0;
   U8                        *measIdRmvLst;
   WrMeasIdNode              *measIdNode;
   CmLList                   *measIdLnk;
/* LTE_ADV starts */
   U8                        a3RptCfgId = WR_UMM_MEAS_INVALID_MEAS_ID;
/* LTE_ADV ends */
   WrUmmMeasRptCfgInfo       rptCfgInfo;
   WrMeasUeCb                *measUeCb = ueCb->measCfg;/*rel10*/

   TRC2(wrUmmMeasCfgForEvntA1)
      measIdRmvLst = rmvMeasInfo->measIdRmvLst;

   rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_A1;
   rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

   wrUmmMeasFindRptCfgID(&a1RptCfgId, &rptCfgInfo, &measUeCb->rptCfgLst);

   rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_A5;
   rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_INTER_RSRP;

   wrUmmMeasFindRptCfgID(&a5RptCfgId, &rptCfgInfo, &measUeCb->rptCfgLst);
   /* Finding report config value for A5 event based Inter-Freq ANR */
   rptCfgInfo.rptCfgTyp      = WR_UMM_MEAS_EVENT_A5;
   rptCfgInfo.thrshldTyp     = WR_UMM_MEAS_THRESHOLD_RSRP;
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType  = WR_UMM_MEAS_RPTCFG_INTER_ANR;

   wrUmmMeasFindRptCfgID(&a5AnrRptCfgId, &rptCfgInfo, &measUeCb->rptCfgLst);

   rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_B2;
   rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CDMA2K_1XRTT;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

   wrUmmMeasFindRptCfgID(&b2RptCfgId, &rptCfgInfo, &measUeCb->rptCfgLst);
   
   rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_B2_UTRA;
   rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE; /* New type we need to add */

   wrUmmMeasFindRptCfgID(&b2RptCfgIdforIRat, &rptCfgInfo, &measUeCb->rptCfgLst);
   
   rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_B2_GERAN;
   rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSSI;  
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_IRATB2_HO;
   wrUmmMeasFindRptCfgID(&b2RptCfgIdforGeranHo, &rptCfgInfo, &measUeCb->rptCfgLst);

   rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_B2_GERAN;
   rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSSI;  
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_IRATB2_REDIR;

   wrUmmMeasFindRptCfgID(&b2RptCfgIdforGeranRedir, &rptCfgInfo, &measUeCb->rptCfgLst);
/* LTE_ADV starts */
   /* Ashish: Modified */
   rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_A3;
   rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_INTER_RSRP;

   wrUmmMeasFindRptCfgID(&a3RptCfgId,&rptCfgInfo, &measUeCb->rptCfgLst);
/* LTE_ADV ends */

   measIdLnk = measUeCb->measIdLst.first;
   while(NULLP != measIdLnk)
   {
      measIdNode = (WrMeasIdNode*)measIdLnk->node;
      measId = measIdNode->measID;
      rptCfgId = measIdNode->rptCfgID;

/* LTE_ADV starts */
      /* When UE comes back to cell center remove A3 event also */
      if((rptCfgId == a1RptCfgId)||
         (rptCfgId == b2RptCfgId)||
         (rptCfgId == a5RptCfgId)||
         (rptCfgId == a3RptCfgId)||
         (rptCfgId == b2RptCfgIdforIRat)||
         (rptCfgId == b2RptCfgIdforGeranHo) ||
         (rptCfgId == b2RptCfgIdforGeranRedir) ||
/* LTE_ADV ends */
         (rptCfgId == a5AnrRptCfgId))
      {
         measIdRmvLst[(rmvMeasInfo->measIdRmvLstCnt)++] = measId;
      }
      measIdLnk = measIdLnk->next;
   }
   /*  configure Event A2 */
   measObjId = measUeCb->intraMeasObj.measObjId;

   rptCfgId = 0;
   rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_A2;
   rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
   rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
   rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

   /* Report Config for Event A2 */
   if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_HO, measObjId,
            &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
   {
      RLOG0(L_ERROR, "Invalid report and Meas config Id");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to deleting Periodic config in
 * UeCb
 *
 *  @details
 * Function:wrUmmMeasDelPerdCfgInUeCb
 *
 *   Processing steps:
 *    - using meas Id find Object 
 *    - delete Object from ueMeasCb
 *     
 * @param[in]  measUeCb: Measurement Ue Control Block 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 **/
PUBLIC S16 wrUmmMeasDelPerdCfgInUeCb
(
 WrMeasUeCb                  *measUeCb
 )
{
   WrMeasIdNode              *measIdNode = NULLP;
   CmLList                   *measIdLnk  = NULLP;

   TRC2(wrUmmMeasDelPerdCfgInUeCb)

   measIdLnk = measUeCb->measIdLst.first;
   while(NULLP != measIdLnk)
   {
      measIdNode = (WrMeasIdNode*)measIdLnk->node;
      if ((NULLP != measIdNode) &&
            (measIdNode->rptCfgID == measUeCb->eutraRptCgiCfg->rptCfgId))
      {
         wrUmmMeasSetFreeMeasId(measUeCb,measIdNode->measID);
         cmLListDelFrm (&measUeCb->measIdLst, measIdLnk);
         WR_FREE(measIdNode, sizeof(WrMeasIdNode));
         break;
      }
      measIdLnk = measIdLnk->next;
   }
   RETVALUE(ROK);
}


/** @brief This function is responsible for to deleting Meas Objects from ue Cb 
 *
 *  @details
 * Function:wrUmmMeasRmvMeasObjInUeCb
 *
 *   Processing steps:
 *    - using  meas Object Id find Object 
 *    - delete Object from ue Cb
 *     
 * @param[in]  measObjId: measurement Object Id 
 * @param[in]  measUeCb: pointer to measurement Ue control block 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */

PRIVATE S16 wrUmmMeasRmvMeasObjInUeCb
(
U8                           measObjId,
WrMeasUeCb                   *measUeCb
)
{
   WrMeasUeCbEutraMeasObjNode  *measObj;
   WrMeasUeCbCdmaMeasObjNode   *cdmaMeasObj;
   WrMeasUeCbUtraMeasObjNode   *utraMeasObj;
   CmLList                     *measObjNode;

   TRC2(wrUmmMeasRmvMeasObjInUeCb)
      if (measUeCb->intraMeasObj.measObjId == measObjId)
      {
         measObj = &measUeCb->intraMeasObj;
         measObj->eutraMeasObj = NULLP;
         measObj->newPci = 0;
         measObj->measObjId = 0;
      }

   measObjNode = measUeCb->interMeasObjLst.first;
   while (NULLP != measObjNode)
   {
      measObj = (WrMeasUeCbEutraMeasObjNode*) measObjNode->node;
      if (measObjId == measObj->measObjId)
      {
         cmLListDelFrm (&measUeCb->interMeasObjLst, measObjNode);
         WR_FREE(measObj, sizeof(WrMeasUeCbEutraMeasObjNode));
         break;
      }
      measObjNode = measObjNode->next;
   }

   measObjNode = measUeCb->interRatMeasObjLst.first;
   while (NULLP != measObjNode)
   {
      cdmaMeasObj = (WrMeasUeCbCdmaMeasObjNode*) measObjNode->node;
      if (measObjId == cdmaMeasObj->measObjId)
      {
         cmLListDelFrm (&measUeCb->interRatMeasObjLst, measObjNode);
         WR_FREE(cdmaMeasObj, sizeof(WrMeasUeCbCdmaMeasObjNode));
         break;
      }
      measObjNode = measObjNode->next;
   }

   measObjNode = measUeCb->utraFddMeasObjLst.first;
   while(NULLP != measObjNode)
   {
      utraMeasObj = (WrMeasUeCbUtraMeasObjNode *) measObjNode->node;
      if(measObjId == utraMeasObj->measObjId)
      {
         cmLListDelFrm(&measUeCb->utraFddMeasObjLst, measObjNode);
         WR_FREE(utraMeasObj, sizeof(WrMeasUeCbUtraMeasObjNode));
         break;
      }
      measObjNode = measObjNode->next;
   }

   measObjNode = measUeCb->utraTddMeasObjLst.first;
   while(NULLP != measObjNode)
   {
      utraMeasObj = (WrMeasUeCbUtraMeasObjNode *) measObjNode->node;
      if(measObjId == utraMeasObj->measObjId)
      {
         cmLListDelFrm(&measUeCb->utraTddMeasObjLst, measObjNode);
         WR_FREE(utraMeasObj, sizeof(WrMeasUeCbUtraMeasObjNode));
         break;
      }
      measObjNode = measObjNode->next;
   }

   RETVALUE(ROK);
}


/** @brief: This function is used to remove the measObj in
 *          the measUeCb based on the measObjId provided, only if the
 *          measObjId is not present in the measObjToRmvLst of measTransInfo. 
 *          As this measObjId is not yet configured to the UE and it will not 
 *          be configured even now as the UE will be relesed with Redirection.
 *          All the measIds mapped to the measObj are also removed from the
 *          measUeCb.
 *          If the measObjId exists in measObjToRmvLst then the measObjId 
 *          will not be removed as it will be removed only after getting 
 *          Reconfiguration complete from the UE.
 * */
PRIVATE Void wrUmmRmvUtraTddCsfbMeasObj
(
 WrMeasUeCb                *measUeCb,
 WrUmmMeasTransInfo        *measTransInfo,
 U8                        measObjId
 )
{
   U8       idx = 0;
   Bool     measObjExstInRmv = FALSE;

   for(idx = 0; idx < measTransInfo->rmvMeasInfo.measObjIdRmvLstCnt; idx++)
   {
      if(measObjId == measTransInfo->rmvMeasInfo.measObjIdRmvLst[idx])
      {
         measObjExstInRmv = TRUE;
         break;
      }
   }

   if(FALSE == measObjExstInRmv)
   {
      /* When a measObjId is removed free all the measId
       * mapped to that measObj as well. */
      wrUmmHrlFreeMeasIdfrmUeCb(measObjId, measUeCb);
      wrUmmMeasRmvMeasObjInUeCb (measObjId, measUeCb);
      wrUmmMeasSetFreeMeasObjId(measUeCb, measObjId);
   }
}


/** @brief This function is responsible for to deleting report config Objects
 * from ue Cb 
 *
 *  @details
 * Function:wrUmmMeasRmvRptCfgInUeCb
 *
 *   Processing steps:
 *    - using  report config Id find Object 
 *    - delete Object from ue Cb
 *     
 * @param[in]  rptCfgId: report config Id 
 * @param[in]  rptCfgLst: pointer to report config list 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasRmvRptCfgInUeCb
(
U8                           rptCfgId,
CmLListCp                    *rptCfgLst
)
{
   CmLList                   *rptCfgNode; 
   WrMeasRptCfgNode          *rptCfgObj; 

   TRC2(wrUmmMeasRmvRptCfgInUeCb)
   rptCfgNode = rptCfgLst->first;
   while(NULLP != rptCfgNode)
   {
      rptCfgObj = (WrMeasRptCfgNode*)rptCfgNode->node;
      if (NULLP != rptCfgObj)
      {
         if(rptCfgObj->rptCfgId == rptCfgId)
         {
            cmLListDelFrm (rptCfgLst, rptCfgNode);
            WR_FREE(rptCfgObj, sizeof(WrMeasRptCfgNode));
            break;
         }
      }
      rptCfgNode = rptCfgNode->next;
   }
   RETVALUE(ROK);
}
   
/** @brief This function is responsible for to deleting Meas Id from ue Cb 
 *
 *  @details
 * Function:wrUmmMeasRmvMeaIdInUeCb
 *
 *   Processing steps:
 *    - using meas Id find Object 
 *    - delete Object from ue Cb
 *     
 * @param[in]  measId: measurement Id 
 * @param[in]  measIdLst: pointer to measurement Id list 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
*
**/
PUBLIC S16 wrUmmMeasRmvMeaIdInUeCb
(
U8                           measId,
CmLListCp                    *measIdLst
)
{
   WrMeasIdNode              *measIdNode;
   CmLList                   *measIdLnk;

   TRC2(wrUmmMeasRmvMeaIdInUeCb)
   measIdLnk = measIdLst->first;
   while(NULLP != measIdLnk)
   {
      measIdNode = (WrMeasIdNode*)measIdLnk->node;
      if (NULLP != measIdNode)
      {
         if(measIdNode->measID == measId)
         {
            cmLListDelFrm (measIdLst, measIdLnk);
            WR_FREE(measIdNode, sizeof(WrMeasIdNode));
            break;
         }
      }
      measIdLnk = measIdLnk->next;
   }
   RETVALUE(ROK);
}
/* ccpu00131966: */
/** @brief This function is responsible for reconfiguring the UE context
 *         at the scheduler.
 *
 * @details
 *
 *     Function: wrUmmMeasGapSchdUeRecfgReq
 *
 *         Processing steps:
 *         - allocate a new configuration request
 *         - set the configuration type to RGR_UE_CFG
 *         - fill the ue info and ambr values
 *         - send the request to scheduler
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmMeasGapSchdUeRecfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrMeasUeCb                *measUeCb;
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   WrUmmMeasTransInfo        *measTransCb;
   U32                       transId;

   measTransCb = &transCb->u.measTransCb;
   
   measUeCb = transCb->ueCb->measCfg;
   if ( measUeCb == NULL ) 
   {
      RETVALUE (RFAILED);
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
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   ueRecfg->ueRecfgTypes |= RGR_UE_MEASGAP_RECFG;
   if ( measTransCb->measGapStatus == WR_UMM_MEAS_GAP_INIT_SCH_CONFIG)
   {
      ueRecfg->ueMeasGapRecfg.isMesGapEnabled = TRUE;
      if ( measUeCb->gapMeasInfo.gapPattern == WR_UMM_MEAS_GAP_PERD_40MS )
      {
         ueRecfg->ueMeasGapRecfg.gapPrd = WR_UMM_MEAS_GAP_PERD_40MS_VAL;
      }
      else if ( measUeCb->gapMeasInfo.gapPattern == WR_UMM_MEAS_GAP_PERD_80MS )
      {
         ueRecfg->ueMeasGapRecfg.gapPrd = WR_UMM_MEAS_GAP_PERD_80MS_VAL;
      }
      ueRecfg->ueMeasGapRecfg.gapOffst = measUeCb->gapMeasInfo.measGapOffset;
   }
   else if ( measTransCb->measGapStatus == WR_UMM_MEAS_GAP_REL)
   {
      ueRecfg->ueMeasGapRecfg.isMesGapEnabled = FALSE;
   }
   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
} /* wrUmmMeasGapSchdUeRecfgReq */

/** @brief This function is responsible for Configuring CDMA 1xRTT or HRPD 
 * frequency measurement configuration in Ue control block at the time of 
 * CSFB or ECSFB procedure.
 *
 *  @details
 *  Function:wrUmmMeasCreatCdmaCfgMsg
 *
 *  Processing steps:
 *      - create CDMA Freq Measurement object in Ue Control block for each
 *        measurement object which is present in cell Cb.
 *      - get the bandclass for respective ARFCN.
 *      - check whether the bandclass is supported by UE or not.
 *      - determine the measurement gap.
 *   @param[in]  ueCb       : pointer to UE control block.
 *   @param[in]  cellCb     : pointer to cell Control block.
 *   @param[in]  ratType    : rat type either 1xRTT or HRPD
 *   @param[in]  addMeasInfo: pointer to measurement transaction control block.
 *   @param[in]  isGapRqd   : pointer to measurment gap flag
 *   @param[in]  measTyp    : measurment type
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasCreatCdmaCfgMsg
(
 WrUeCb                      *ueCb,
 WrUmmCellCb                 *cellCb,
 U8                          ratType,
 WrUmmMeasAddInfo            *addMeasInfo,
 Bool                        *isGapRqd,
 WrUmmMeasTyp                 measTyp
)
{
   U8                        measObjId = 0;
   LwrCellMeasCfgGrp         *measCellCfg;
   WrUmmMeasCdmaMeasObjNode  *cdmaCellObj;
   CmLList                   *cdmaMeasObj;
   U8                        servingBand;
   U16                       servFreq;
   Bool                      isUeSupported=FALSE;
   Bool                      isCfgDone=FALSE;
   WrMeasUeCb                *measUeCb;
   WrUmmMeasRptCfgInfo       rptCfgInfo;

   TRC2(wrUmmMeasCreatCdmaCfgMsg)

   *isGapRqd   = FALSE;
      measUeCb = ueCb->measCfg;

   servFreq = wrEmmGetServCarrierFreq(ueCb->cellId); 
   /*Get serving bandclass . we need serving band to verify measgap 
    * configuration required for all the supported bands*/
   wrUmmFndEUTRABand(servFreq,&servingBand);

   measCellCfg = cellCb->measCellCfg;
   if(CDMA2000_1XRTT == ratType)
   {
      cdmaMeasObj = cellCb->measCellCb->cdma1xMeasObjLst.first;
   }
   else
   {
      cdmaMeasObj = cellCb->measCellCb->cdmaHrpdMeasObjLst.first;
   }
   while (NULLP != cdmaMeasObj)
   {
      /*Meas Objects for CDMA frequencies*/
      cdmaCellObj = (WrUmmMeasCdmaMeasObjNode*)cdmaMeasObj->node;
      if(cdmaCellObj->cdmaCellInfo.count == 0)
      {
         cdmaMeasObj = cdmaMeasObj->next;
         continue;
      }

      /*Check whether the bandclass is UE supported or not*/
      isUeSupported = 
         wrUmmChk1xRttCdmaBandSupported(ueCb, cdmaCellObj->bandClass);

      if(isUeSupported == TRUE)
      {
         /* Check HRL for this UE to see if UTRA RAT is forbidden or not. */
         if((ueCb->hoRstrctInfo != NULLP) && (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
         {
            WrHrlDecisionRet forbRat = WR_MEAS_MBLTY_APPL_TO_RAT;

            forbRat = wrChkForFrbRat(WR_FORB_RAT_CDMA, ueCb->hoRstrctInfo);
            if(forbRat == WR_MEAS_MBLTY_NOT_APPL_TO_RAT)
            {
               /* CDMA is forbidden for the UE, so return failure. */
               RETVALUE(RFAILED);
            }
         }

         isCfgDone = TRUE;
         if(*isGapRqd == FALSE)
         {
            wrUmmMeasGapReqdInterRat(ueCb, servingBand,
                  cdmaCellObj->bandClass, CDMA2000_1XRTT, NULLP, isGapRqd);

         }

         /*If the measurement object is already created for particular bandClass
          * and arfcn then no need to configure the measurement object again to
          * UE
          */
         if(ROK != wrUmmMeasUpdtCdmaMeasObjAndIdFromArfcn(cdmaCellObj->bandClass, 
                            cdmaCellObj->arfcn, measUeCb, &measObjId, 
                            &cdmaCellObj))
            {
            /* this function will get the free measObjId from the free list and
             * store folloing fields in WrMeasUeCbCdmaMeasObjNode structure
             * 1. CdmaMeasObj Node
             * 2. cdmaMeasObjId
             * WrMeasUeCbCdmaMeasObjNode node is then added to
             * measUeCb->interRatMeasObjLst which will be used while filling the
             * NHU structure in the wrMeasFillNhuMsg function*/  
            measObjId =  wrUmmMeasAddInterRatMeasObjToUeCb(measUeCb, cdmaCellObj);
            if(measObjId != WR_UMM_MEAS_INVALID_MEAS_ID)
            {
               addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
               addMeasInfo->measObjIdCnt++;
            }
            else
            { 
               RLOG_ARG0(L_ERROR, DBG_CRNTI, ueCb->crnti, "Cdma Object addition in UeCb Failed");
               RETVALUE(RFAILED);
            }
         }

         /*report config for Strongest Cells*/
         if (( measTyp == WR_UMM_MEAS_TYPE_CSFB) || 
                      ( measTyp == WR_UMM_MEAS_TYPE_ECSFB))
         { 
            rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_CDMA_STRONG_CELL;
            rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;
            if(CDMA2000_1XRTT == ratType)
            {
               rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CDMA2K_1XRTT;
            }
            else
            {
               rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CDMA2K_HRPD;
            }

            rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;
         }
         else if(measTyp == WR_UMM_MEAS_TYPE_HO )
         { 
            rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_B2;
            rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;  
            if(CDMA2000_1XRTT == ratType)
            {
               rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CDMA2K_1XRTT;
            }
            else
            {
               rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CDMA2K_HRPD;
            }
            rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

         }
         if(ROK != wrUmmMeasRptAndMeasIdCfg(measTyp, measObjId,
                  &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
         {
            RLOG0(L_ERROR, "Invalid report and Meas config Id");
            RETVALUE(RFAILED);
         }
      }
      cdmaMeasObj = cdmaMeasObj->next;
   }
   if(isCfgDone == TRUE)
   {
      RETVALUE(ROK);
   }
   RETVALUE(RFAILED);
}/* End of wrUmmMeasCreatCdmaCfgMsg */

/* @brief: This function will fetch the Utra MeasObjNode
 *  *          matching the received arfcn from the received list.
 *   * */
PUBLIC WrMeasUeCbUtraMeasObjNode* wrUmmGetUeCbUtraMeasObj
(
  U16           arfcn,
   CmLListCp     utraCellLst
    )
{
   WrMeasUeCbUtraMeasObjNode      *utraMeasObjNode = NULLP;

   cmLListFirst(&utraCellLst);
   while(cmLListCrnt(&utraCellLst))
   {
      utraMeasObjNode = (WrMeasUeCbUtraMeasObjNode *) cmLListNode(cmLListCrnt(&utraCellLst));
      if(arfcn == utraMeasObjNode->utraMeasObj->arfcn)
      {
         RETVALUE(utraMeasObjNode);
      }
      cmLListNext(&utraCellLst);
   }
   RETVALUE(NULLP);
}


/* @brief: This function is used to populate the HrlLai List of the ueCb
 *  *          if UE has either registered LAI and HRL based on the LAI and
 *   *          HRL checks.
 *    * */
PUBLIC Bool wrUmmPopUtraCsfbLaiHrlCells
(
 WrUeCb                      *ueCb,
 WrUmmMeasUtraMeasObjNode  *utraCellObj,
 WrUmmMeasTransInfo          *measTransInfo,
 Bool                      *isNewMeasObj
 )
{
   WrMeasUeCbUtraMeasObjNode *measUeCb = NULLP;
   WrUmmMeasUtraMeasObjNode  *ueMeasObj = NULLP;
   WrUtraNeighCellCb         *neighCellCb = NULLP;
   Bool                       isCellAppl = FALSE;
   WrMeasNrUtraCellNode      *nrWCellObjNode = NULLP;
   U8                        cellIdx;

   /* Check if UTRA measObj for this frequency is already created ot not. 
    * If they are already created then delete that UTRA measObj and add 
    * a new measObj based on Reg. LAI and Hrl.ForbLAC */
   switch(utraCellObj->duplexMode)
   {
      case WR_UMM_MEAS_DUPLEX_MODE_FDD:
         {
            if(ueCb->measCfg->utraFddMeasObjLst.count != 0)
            {
               /* Fetch ueMeasObj from measUeCb list */
               measUeCb = (WrMeasUeCbUtraMeasObjNode *) wrUmmGetUeCbUtraMeasObj(utraCellObj->arfcn,
                     ueCb->measCfg->utraFddMeasObjLst);
               if(measUeCb != NULLP)
               {
                  /* This UTRA Freq is already configured to the UE for measurements. 
                   * So delete it. */
                  measTransInfo->rmvMeasInfo.measObjIdRmvLst\
                     [measTransInfo->rmvMeasInfo.measObjIdRmvLstCnt++] = measUeCb->measObjId;
                  *isNewMeasObj = TRUE;
               }
            }
         }
         break;
      case WR_UMM_MEAS_DUPLEX_MODE_TDD:
         {
            if(ueCb->measCfg->utraTddMeasObjLst.count != 0)
            {
               /* Fetch ueMeasObj from measUeCb list */
               measUeCb = (WrMeasUeCbUtraMeasObjNode *) wrUmmGetUeCbUtraMeasObj(utraCellObj->arfcn,
                     ueCb->measCfg->utraTddMeasObjLst);
               if(measUeCb != NULLP)
               {
                  /* This UTRA Freq is already configured to the UE for measurements. 
                   * So delete it. */
                  measTransInfo->rmvMeasInfo.measObjIdRmvLst\
                     [measTransInfo->rmvMeasInfo.measObjIdRmvLstCnt++] = measUeCb->measObjId;
                  *isNewMeasObj = TRUE;
               }
            }
         }
         break;
   }

   /* If HRL_LAI list already has cells for this frequency then clear 
    * all the entries and add new entries based on Reg.LAI and Hrl.ForbLAC. */
   ueMeasObj = (WrUmmMeasUtraMeasObjNode *) wrUmmHrlUtraMeasObjCfg(utraCellObj, ueCb);
   if(ueMeasObj != NULLP) 
   {
      /* Now add the measObj based on Reg.LAI and Hrl.ForbLAC filter. */
      cmLListFirst(&utraCellObj->cellLst);
      while(cmLListCrnt(&utraCellObj->cellLst))
      {
         nrWCellObjNode = (WrMeasNrUtraCellNode *) cmLListNode\
                          (cmLListCrnt(&utraCellObj->cellLst));
         neighCellCb = (WrUtraNeighCellCb *) wrEmmGetNeighUtraCell (ueCb->cellId,
               utraCellObj->arfcn, nrWCellObjNode->psc, utraCellObj->duplexMode);
         if(neighCellCb == NULLP)
         {
            RLOG0(L_ERROR, "utraNeighCellCb is NULLP");
            RETVALUE(FALSE);
         }
         isCellAppl = wrFindRegLaiPlmnCell(&ueCb->regLai, neighCellCb, ueCb->hoRstrctInfo);
         if(isCellAppl == TRUE)
         {
            wrFillUeUtraCellLstHrlUe(ueMeasObj, nrWCellObjNode, &cellIdx, utraCellObj->arfcn);
         }
         cmLListNext(&utraCellObj->cellLst);
      }

      if(ueMeasObj->cellLst.count == 0)
      {
         /* All cells do not match RegLaiPlmn, so do not add
          * this measObj. And delete this measObj from HRL LAI list */
         if(utraCellObj->duplexMode == WR_UMM_MEAS_DUPLEX_MODE_TDD)
         {
            cmLListFirst(&ueCb->hrlMeasCfg.utraTddMeasObjLst);
            while(cmLListCrnt(&ueCb->hrlMeasCfg.utraTddMeasObjLst))
            {
               ueMeasObj = (WrUmmMeasUtraMeasObjNode *) cmLListNode \
                           (cmLListCrnt(&ueCb->hrlMeasCfg.utraTddMeasObjLst));
               if(ueMeasObj->arfcn == utraCellObj->arfcn)
               {
                  wrFreeUeHrlUtraCellLst(ueMeasObj);
                  cmLListDelFrm(&ueCb->hrlMeasCfg.utraTddMeasObjLst, &ueMeasObj->lnk);
                  WR_FREE(ueMeasObj, sizeof(WrUmmMeasUtraMeasObjNode));
                  break;
               }
               cmLListNext(&ueCb->hrlMeasCfg.utraTddMeasObjLst);
            }
         }
         else
         {
            cmLListFirst(&ueCb->hrlMeasCfg.utraFddMeasObjLst);
            while(cmLListCrnt(&ueCb->hrlMeasCfg.utraFddMeasObjLst))
            {
               ueMeasObj = (WrUmmMeasUtraMeasObjNode *) cmLListNode \
                           (cmLListCrnt(&ueCb->hrlMeasCfg.utraFddMeasObjLst));
               if(ueMeasObj->arfcn == utraCellObj->arfcn)
               {
                  wrFreeUeHrlUtraCellLst(ueMeasObj);
                  cmLListDelFrm(&ueCb->hrlMeasCfg.utraFddMeasObjLst, &ueMeasObj->lnk);
                  WR_FREE(ueMeasObj, sizeof(WrUmmMeasUtraMeasObjNode));
               }
               cmLListNext(&ueCb->hrlMeasCfg.utraFddMeasObjLst);
            }
         }
         RETVALUE(FALSE);
      }
   }
   RETVALUE(TRUE);
}
/** @brief This function is responsible for to Add Geran measurement Object
 * into uecb.
 *
 *  @details
 * Function:wrUmmMeasAddGeranMeasObjToUeCb
 *
 *   Processing steps:
 *    - Add Geran measurement object into UE control block.
 *
 * @param[in]  measTransInfo: pointer to the measurement Transaction info
 * @param[in]  ueCb: pointer to the ue Cb
 * @param[out]  cellCbMeasObj: pointer to the CellCb measurement Object.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PUBLIC U8 wrUmmMeasAddGeranMeasObjToUeCb
(
 WrUmmMeasAddInfo            *measTransInfo,
 WrUeCb                      *ueCb,
 WrUmmMeasGeranMeasObjNode    *cellCbMeasObj,
 U16                         newPsc
)
{
   WrMeasUeCbGeranMeasObjNode    *measObjGeranNode;
   U8                            measObjId = 0;
   S16                           ret;

   TRC2(wrUmmMeasAddGeranMeasObjToUeCb)

   ret =  wrUmmMeasGetFreeMeasObjId(ueCb->measCfg, &measObjId);
   if(ret == WR_UMM_MEAS_INVALID_MEAS_ID)
   {
      RLOG0(L_ERROR, "Meas Object Id not available");
      RETVALUE(ret);
   }
   WR_ALLOC(&measObjGeranNode, sizeof(WrMeasUeCbGeranMeasObjNode));
   if(NULLP == measObjGeranNode)
   {
      wrUmmMeasSetFreeMeasObjId(ueCb->measCfg, measObjId);
      RLOG0(L_FATAL, "Failed to allocate memory.");
      RETVALUE(RFAILED);
   }
   measObjGeranNode->geranMeasObj = cellCbMeasObj;
   measObjGeranNode->measObjId = measObjId;
   measObjGeranNode->newPsc = newPsc;

   measObjGeranNode->lnk.node = (PTR)measObjGeranNode;
  
   cmLListAdd2Tail(&ueCb->measCfg->geranMeasObjLst, &measObjGeranNode->lnk);
   RETVALUE(measObjId);
}
/** @brief This function is responsible for Configuring GERAN frequency
 * measurement configuration in Ue control block at the time of  HO
 * procedure.
 *
 *  @details
 *  Function:wrUmmMeasCreatGeranCfgMsg
 *
 *  Processing steps:
 *      - create Utra Freq Measurement object in Ue Control block for each
 *        measurement object which is present in cell Cb.
 *      - get the bandclass for respective ARFCN.
 *      - check whether the bandclass is supported by UE or not.
 *      - determine the measurement gap.
 *   @param[in]  ueCb       : pointer to UE control block.
 *   @param[in]  cellCb     : pointer to cell Control block.
 *   @param[in]  measUeCb   : pointer to measurement Ue Control Block.
 *   @param[in]  measTransCb: pointer to measurement transaction control block.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasCreatGeranCfgMsg
(
 WrUeCb                      *ueCb,
 WrUmmCellCb                 *cellCb,
 WrUmmMeasTransInfo          *measTransInfo,
 Bool                        *isGapRqd
)
{
   U8                        measObjId = 0;
   LwrCellMeasCfgGrp         *measCellCfg;
   WrUmmMeasGeranMeasObjNode  *geranCellObj;
   WrMeasUeCbGeranMeasObjNode *geranUeObj;
   CmLList                   *geranMeasObj;
   WrMeasUeCb                *measUeCb;
   WrUmmMeasRptCfgInfo       rptCfgInfo;
   WrUmmMeasGeranMeasObjNode *hrlMeasObj = NULLP;
   WrUmmMeasAddInfo          *addMeasInfo = &measTransInfo->addMeasInfo;
   Bool                      isRedirSupport = FALSE;

   TRC2(wrUmmMeasCreatGeranCfgMsg)

   RLOG0(L_DEBUG, "wrUmmMeasCreatGeranCfgMsg Called");
   measUeCb = ueCb->measCfg;

   measCellCfg = cellCb->measCellCfg;
   geranMeasObj = cellCb->measCellCb->geranMeasObjLst.first;
   while (NULLP != geranMeasObj)
   {
      /*Meas Objects for UTRA frequencies*/
      geranCellObj = (WrUmmMeasGeranMeasObjNode*)geranMeasObj->node;

      if(NULLP != geranCellObj)
      {
         if(TRUE == wrUmmChkIRATBndLstNeedFrGaps(ueCb,geranCellObj->bandIndicator,
                  geranCellObj->arfcnCnt))
         {
            *isGapRqd = TRUE;
         }
      }
      else
      {
         RLOG0(L_ERROR, "geranCellObj is NULLP");
         RETVALUE(RFAILED);
      }
      /*report config for Strongest Cells*/
      /* Populate the WrHrlMeasCfg. */
      hrlMeasObj = wrUmmHrlGeranMeasObjCfg(geranCellObj, ueCb);
      if(hrlMeasObj == NULLP)
      {
         RLOG0(L_ERROR, "Geran hrlMeasObj is NULLP");
         geranMeasObj = geranMeasObj->next;
         continue;
      }
      /* For Handover, if HRL is present.
       ** Check based on HRL and  populate in HRL_LAI list. */
      if((ueCb->hoRstrctInfo != NULLP) && 
            (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
      {
         if(FALSE == wrUmmHrlUeGeranMeasCfg(hrlMeasObj, ueCb))
         {
            RLOG0(L_ERROR, "Geran hrlMeasObj is NULLP");
            geranMeasObj = geranMeasObj->next;
            continue;
         }
      }

      if(ROK != wrUmmMeasUpdtGeranMeasObjAndIdFromArfcn(measUeCb, &measObjId,
               &geranUeObj))
      {
         measObjId =  wrUmmMeasAddGeranMeasObjToUeCb(&measTransInfo->addMeasInfo,
                                     ueCb, geranCellObj, 0xffff);
         if(measObjId != WR_UMM_MEAS_INVALID_MEAS_ID)
         {
            addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
            addMeasInfo->measObjIdCnt++;
         }
         else
         { 
            RLOG0(L_ERROR, "Geran Object addition in UeCb Failed");
            RETVALUE(RFAILED);
         }
      }
      if(wrEmmGetPlmnRedirSupport(&ueCb->plmnId, &isRedirSupport, ueCb->cellId) == ROK)
      { 
         rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_B2_GERAN;
         rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSSI;  
         rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
         if(isRedirSupport == TRUE)
         {
            RLOG1(L_DEBUG,"Configuring GERAN measurements for Redirection for UE "
                                                "Crnti [%d]", ueCb->crnti);         
            rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_IRATB2_REDIR;
            if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_REDIR, measObjId,
                     &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
            {
               RLOG0(L_ERROR, "Invalid report and Meas config Id");
               RETVALUE(RFAILED);
            }
         }
         else
         {
            RLOG1(L_DEBUG,"Configuring GERAN measurements for Handover for UE "
                                                "Crnti [%d]", ueCb->crnti);         
            rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_IRATB2_HO;
            if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_HO, measObjId,
                     &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
            {
               RLOG0(L_ERROR, "Invalid report and Meas config Id");
               RETVALUE(RFAILED);
            }
         }
      }
      geranMeasObj = geranMeasObj->next;
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for Configuring utra fdd frequency
 * measurement configuration in Ue control block at the time of CSFB and HO
 * procedure.
 *
 *  @details
 *  Function:wrUmmMeasCreatUtraCfgMsg
 *
 *  Processing steps:
 *      - create Utra Freq Measurement object in Ue Control block for each
 *        measurement object which is present in cell Cb.
 *      - get the bandclass for respective ARFCN.
 *      - check whether the bandclass is supported by UE or not.
 *      - determine the measurement gap.
 *   @param[in]  ueCb       : pointer to UE control block.
 *   @param[in]  cellCb     : pointer to cell Control block.
 *   @param[in]  measUeCb   : pointer to measurement Ue Control Block.
 *   @param[in]  measTransCb: pointer to measurement transaction control block.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasCreatUtraCfgMsg
(
 WrUeCb                      *ueCb,
 WrUmmCellCb                 *cellCb,
 WrUmmMeasTransInfo          *measTransInfo,
 Bool                        *isGapRqd,
 WrUmmMeasTyp                 measTyp
)
{
   U8                        measObjId = 0;
   U8                        targetBand;
   U8                        servingBand;
   U16                       servFreq;
   LwrCellMeasCfgGrp         *measCellCfg;
   WrUmmMeasUtraMeasObjNode  *utraCellObj;
   CmLList                   *utraMeasObj;
   Bool                      isUeSupported=FALSE;
   Bool                      isGapReqd=FALSE;
   Bool                      isGapCfgd = FALSE;
   Bool                      isCfgDone=FALSE;
   Bool                      isNewMeasObj = FALSE;
   WrMeasUeCb                *measUeCb;
   WrUmmMeasRptCfgInfo       rptCfgInfo;
   WrUmmMeasAddInfo          *addMeasInfo = &measTransInfo->addMeasInfo;

   TRC2(wrUmmMeasCreatUtraCfgMsg)

   RLOG0(L_DEBUG, "wrUmmMeasCreatUtraCfgMsg Called");
   measUeCb = ueCb->measCfg;

   servFreq = wrEmmGetServCarrierFreq(ueCb->cellId);
   /*Get serving bandclass . we need serving band to verify measgap 
    * configuration required for all the supported bands*/
   wrUmmFndEUTRABand(servFreq,&servingBand);

   measCellCfg = cellCb->measCellCfg;
   utraMeasObj = cellCb->measCellCb->utraFddMeasObjLst.first;
   while (NULLP != utraMeasObj)
   {
      /*Meas Objects for UTRA frequencies*/
      utraCellObj = (WrUmmMeasUtraMeasObjNode*)utraMeasObj->node;
      if(utraCellObj->cellLst.count == 0)
      {
         utraMeasObj = utraMeasObj->next;
         continue;
      }

      if( utraCellObj->duplexMode == UTRA_FDD )
      {
         /*Get bandclass for corresponding ARFCN*/
         wrUmmFndUTRABandFDD(utraCellObj->arfcn,&targetBand);

         /*Check whether the bandclass is UE supported or not*/
         isUeSupported = wrUmmUTRAFDDBandSupported(ueCb,targetBand);
      }

      if(isUeSupported == TRUE)
      {
         RLOG0(L_DEBUG, "UTRA FDD Supported by UE");
         isCfgDone = TRUE;
         if(isGapCfgd == FALSE)
         {
            wrUmmMeasGapReqdInterRat(ueCb,servingBand,targetBand,
                  utraCellObj->duplexMode,utraCellObj->tddMode,&isGapReqd);

            if(isGapReqd == TRUE)
            {
               isGapCfgd = TRUE;
            }
         }
         /*report config for Strongest Cells*/
         if ( measTyp == WR_UMM_MEAS_TYPE_CSFB)
         { 
            if(ueCb->regLai.isLaiPres == TRUE)
            {
               /* For CSFB, if Reg. LAI is present.
                ** Check only RegLAI.PLMN and if HRL is also present check HRL.ForbLAC
                ** Populate it in HRL_LAI list. */
               if(FALSE == wrUmmPopUtraCsfbLaiHrlCells(ueCb, utraCellObj,
                                                 measTransInfo, &isNewMeasObj))
               {
                  /* All cells in this arfcn do not match Reg.LAI plmn.
                   * Do not add measObj for this arfcn. */
                  utraMeasObj = utraMeasObj->next;
                  continue;
               }
            }
            else
            {
               /* For CSFB, if Reg. LAI is not present,
                ** then proceed without measurements.
                ** Sending RFAILED will cause default redirection */
               RETVALUE(RFAILED);
            }
            rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_UTRA_STRONG_CELL_CSFB;
            rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;
            rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
            rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;
            RLOG1(L_DEBUG,"Configuring UTRA FDD measurements for CSFB for UE \
                                                Crnti [%d] \n", ueCb->crnti);         
         }
         else if(measTyp == WR_UMM_MEAS_TYPE_HO )
         { 
            /* For Handover, if HRL is present.
             ** Check based on HRL and  populate in HRL_LAI list. */
            if((ueCb->hoRstrctInfo != NULLP) && 
                   (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
            {
               if(FALSE == wrUmmHrlUeUtraMeasCfg(utraCellObj, ueCb))
               {
                  /* All cells in this arfcn are forbidden.
                   * Do not  add measObj for this arfcn. */
                  utraMeasObj = utraMeasObj->next;
                  continue;
               }
            }
            rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_B2_UTRA;
            rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSCP;  
            rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
            rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;
            RLOG1(L_DEBUG,"Configuring UTRA FDD measurements for Handover for UE "
                                               "Crnti [%d] \n", ueCb->crnti);         
         }

         if((isNewMeasObj == TRUE) || 
               (ROK != wrUmmMeasUpdtUtraMeasObjAndIdFromArfcn(utraCellObj->arfcn, 
                  measUeCb, &measObjId, &utraCellObj)))
         {
            measObjId =  wrUmmMeasAddUtraMeasObjToUeCb(measUeCb, utraCellObj, 0xffff);
            if(measObjId != WR_UMM_MEAS_INVALID_MEAS_ID)
            {
               addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
               addMeasInfo->measObjIdCnt++;
            }
            else
            { 
               RLOG0(L_ERROR, "Utra Object addition in UeCb Failed");
               RETVALUE(RFAILED);
            }
         }
         if(ROK != wrUmmMeasRptAndMeasIdCfg(measTyp, measObjId,
                  &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
         {
            RLOG0(L_ERROR, "Invalid report and Meas config Id");
            RETVALUE(RFAILED);
         }
      }
      utraMeasObj = utraMeasObj->next;
    }
   if(isCfgDone == TRUE)
   {
      *isGapRqd = isGapCfgd;
      RETVALUE(ROK);
   }
   RETVALUE(RFAILED);
}

/** @brief: This function is responsible for configuring
 *          UTRA TDD measurememnt configuration at the time 
 *          of Redirection towards TDSCDMA.
 * */
PRIVATE S16 wrUmmMeasCreatUtraTddCfgMsg
(
 WrUeCb                      *ueCb,
 WrUmmCellCb                 *cellCb,
 WrUmmTransCb                *transCb,
 Bool                        *isGapRqd,
 WrUmmMeasTyp                 measTyp
 )
{
   U8                        measObjId = 0;
   LwrCellMeasCfgGrp         *measCellCfg;
   WrUmmMeasUtraMeasObjNode  *utraCellObj;
   CmLList                   *utraMeasObj;
   WrMeasUeCb                *measUeCb;
   U8                         targetBand = 0;
   U8                        servingBand;
   U16                       servFreq;
   Bool                      isUeSupported=FALSE;
   Bool                      isGapReqd=FALSE;
   Bool                      isNewMeasObj = FALSE;
   WrUmmMeasRptCfgInfo       rptCfgInfo;
   WrUmmMeasTransInfo        *measTransInfo = NULLP;

   RLOG0(L_DEBUG, "wrUmmMeasCreatUtraTddCfgMsg called");
   if(measTyp == WR_UMM_MEAS_TYPE_CSFB)
   {
      measTransInfo = &transCb->u.csfbTrans.measTransCb;
   }
   else
   {
      measTransInfo = &transCb->u.measTransCb;
   }

   measUeCb = ueCb->measCfg;

   servFreq = wrEmmGetServCarrierFreq(ueCb->cellId);
   /*Get serving bandclass . we need serving band to verify measgap 
    * configuration required for all the supported bands*/
   wrUmmFndEUTRABand(servFreq,&servingBand);
   measCellCfg = cellCb->measCellCfg;
   utraMeasObj = cellCb->measCellCb->utraTddMeasObjLst.first;
   while (NULLP != utraMeasObj)
   {
      /*Meas Objects for UTRA TDD frequencies*/
      utraCellObj = (WrUmmMeasUtraMeasObjNode*)utraMeasObj->node;
      if(utraCellObj->cellLst.count == 0)
      {
         utraMeasObj = utraMeasObj->next;
         continue;
      }

      if(utraCellObj->duplexMode == UTRA_TDD)
      {
         targetBand = utraCellObj->tddBandIndicator;
         isUeSupported = wrUmmUtraTddBandSupported(ueCb, targetBand, 
               utraCellObj->tddMode);
         RLOG2(L_DEBUG, "UTRA TDD ARFCN = %d, targetBand = %d",
                                      utraCellObj->arfcn, targetBand); 
      }

      if(isUeSupported == TRUE)
      {
         RLOG0(L_DEBUG, "UE Supports UTRA TDD Bands ");
         if(isGapReqd == FALSE)
         {
            wrUmmMeasGapReqdInterRat(ueCb,servingBand,targetBand,
                  utraCellObj->duplexMode,utraCellObj->tddMode,&isGapReqd);
         }

         /*report config for Strongest Cells*/
         if ( measTyp == WR_UMM_MEAS_TYPE_CSFB)
         { 
            if(ueCb->regLai.isLaiPres == TRUE)
            {
               /* For CSFB, if Reg. LAI is present.
                ** Check only RegLAI.PLMN and if HRL is also present check HRL.ForbLAC
                ** Populate it in HRL_LAI list. */
               if(FALSE == wrUmmPopUtraCsfbLaiHrlCells(ueCb, utraCellObj, measTransInfo, &isNewMeasObj))
               {
                  /* All cells in this arfcn do not match Reg.LAI plmn.
                   * Do not add measObj for this arfcn. */
                  RLOG1(L_WARNING, " All cells in this arfcn [%d] do not match \
                        Reg.LAI plmn, so not adding this measObj for UE \n", utraCellObj->arfcn); 
                  utraMeasObj = utraMeasObj->next;
                  continue;
               }
            }
            else
            {
               /* For CSFB, if Reg. LAI is not present,
                ** then proceed without measurements.
                ** Sending RFAILED will cause default redirection */
               RLOG0(L_WARNING, " Meas UTRA TDD Cfg: Registered LAI is \
                     not present, so proceed without measurements ");
               RETVALUE(RFAILED);
            }

            rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_UTRA_STRONG_CELL_CSFB;
            rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;
            rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
            rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;
            RLOG1(L_DEBUG,"Configuring UTRA TDD measurements for CSFB for UE \
                                                Crnti [%d] \n", ueCb->crnti);         
         }
         else if(measTyp == WR_UMM_MEAS_TYPE_REDIR)
         { 
            /* For Redirection, if HRL is present.
             ** Check based on HRL and  populate in HRL_LAI list. */
            if((ueCb->hoRstrctInfo != NULLP) && (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
            {
               if(FALSE == wrUmmHrlUeUtraMeasCfg(utraCellObj, ueCb))
               {
                  /* All cells in this arfcn are forbidden.
                   * Do not  add measObj for this arfcn. */
                  RLOG1(L_WARNING, " All cells in this arfcn [%d] are forbidden \
                        in HRL, so not adding this measObj for UE \n", utraCellObj->arfcn); 
                  utraMeasObj = utraMeasObj->next;
                  continue;
               }
            }

            rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_EVENT_B2_UTRA;
            rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSCP;  
            rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
            rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_IRATB2_REDIR;
            RLOG1(L_DEBUG,"Configuring UTRA TDD measurements for Redirection for UE \
                                                Crnti [%d] \n", ueCb->crnti);         
         }

         if((isNewMeasObj == TRUE) || (ROK != wrUmmMeasUpdtUtraMeasObjAndIdFromArfcn(utraCellObj->arfcn, 
                  measUeCb, &measObjId, &utraCellObj)))
         {
            measObjId =  wrUmmMeasAddUtraMeasObjToUeCb(measUeCb, utraCellObj, 0xffff);
            if(measObjId != WR_UMM_MEAS_INVALID_MEAS_ID)
            {
               measTransInfo->addMeasInfo.measObjLst[measTransInfo->addMeasInfo.measObjIdCnt] = measObjId;
               measTransInfo->addMeasInfo.measObjIdCnt++;
            }
            else
            { 
               RLOG0(L_ERROR, "Utra Object addition in UeCb Failed");
               RETVALUE(RFAILED);
            }
         }
         if(ROK != wrUmmMeasRptAndMeasIdCfg(measTyp, measObjId,
                  &rptCfgInfo, ueCb, measCellCfg, &measTransInfo->addMeasInfo))
         {
            RLOG0(L_ERROR, "Invalid report and Meas config Id");
            RETVALUE(RFAILED);
         }
      }
      utraMeasObj = utraMeasObj->next;
   }

   /* 1. If number of UTRA TDD measObjs added were 0, return RFAILED.
    **    Returning RFAILED will cause default redirection for CSFB 
    **    and will not configure B2 measurements for Redirection. */
   if((ueCb->measCfg->utraTddMeasObjLst.count == 0) && 
         (ueCb->hrlMeasCfg.utraTddMeasObjLst.count == 0))      
   {
      RLOG0(L_WARNING, " Meas UTRA TDD Cfg: No UTRA TDD measurement objects added \
            due to Reg.LAI/HRL filter, so proceed without measurements ");
      RETVALUE(RFAILED);
   }

   /* 2. If number of UTRA TDD measObjs added were 1 and CSFB transaction, then populate
    **   utraTddRedirInfo and Release the UE.*/
   /* For CSFB the HRL_LAI List will always be populated (if LAI is present 
    * else we would have returned RFAIL by now), so check the HRL_LAI list count only. */
   if((ueCb->hrlMeasCfg.utraTddMeasObjLst.count == 1)
         && (measTyp == WR_UMM_MEAS_TYPE_CSFB))
   {
      RLOG0(L_WARNING, " Meas UTRA TDD Cfg: Only one measObj is available for \
           configuration to the UE due to Reg.LAI and HRL filter, \
           so proceed without measurements");
      WrUmmRedirectInfo   *redirInfo = NULLP;
      WR_ALLOC(&transCb->u.csfbTrans.redirectInfo, sizeof(WrUmmRedirectInfo));
      if (NULLP == transCb->u.csfbTrans.redirectInfo)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(NULLP);
      }

      redirInfo = transCb->u.csfbTrans.redirectInfo;

      /* Since only 1 measObj was configured, UE will be released without measurements.
       * So delete the measObj if it was configured in the ueCb->measCfg now in this API. 
       * Also delete all the measId mapped to that measObj */
      /* If we are here then the measObjId would have been filled only once and we can 
       * delete it. */
      wrUmmRmvUtraTddCsfbMeasObj(measUeCb, measTransInfo, measObjId);

      /* Populate Redir Info and Release the UE. */
      redirInfo->cellId = ueCb->cellId;
      redirInfo->ratTyp = WR_UMM_MEAS_UTRA_TDD_FREQ;

      wrUmmPopUtraRedirAndRel(ueCb, redirInfo);
      RLOG0(L_DEBUG, "wrUmmCsfbRelUe called from wrUmmMeasCreatUtraTddCfgMsg"); 
      wrUmmCsfbRelUe(transCb);
   }

   if(TRUE == isGapReqd)
   {
      *isGapRqd = TRUE;
   }
   RETVALUE(ROK);
}

/** @brief This function will check the UE's FGI bits 
 *          based on whether its for Periodic or Event
 *          measurements and will return TRUE or FALSE.
 * */
PUBLIC Bool wrUmmUtilUtraTddMeasChk
(
 WrUeCb                       *ueCb,
 WrUmmFgiType                 fgiTyp
 )
{
   Bool                          fddBndSupp = FALSE;
   Bool                          tddMeas = FALSE;

   if(TRUE == wrChkUtranFddSupp(ueCb->ueCap))
   {
      fddBndSupp = TRUE;
   }

   switch(fgiTyp)
   {
      case WR_UMM_UTRA_TDD_PERD_MEAS_SUPPORTED:
         {
            /* Check if UE supports Bit 16 */
            if(TRUE == wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti, WR_UMM_NON_ANR_PERD_MEAS_SUPPORTED))
            {
               if(fddBndSupp == TRUE)
               {
                  /* Check bit 39 */
                  if(TRUE == wrUmmUtilFeatureGrpIndRel9(ueCb->cellId, ueCb->crnti,
                           WR_UMM_UTRA_TDD_EVNT_MEAS_SUPPORTED))
                  {
                     tddMeas = TRUE;
                  }
               } 
               else 
               {
                  /* Check bit 22 */
                  if(TRUE == wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti, 
                           WR_UMM_UTRA_MEAS_SUPPORTED))
                  {
                     tddMeas = TRUE;
                  }
               }
            }
         }
         break;
      case WR_UMM_UTRA_TDD_EVNT_MEAS_SUPPORTED:
         {
            if(fddBndSupp == TRUE)
            {
               /* Check bit 39 */
               if(TRUE == wrUmmUtilFeatureGrpIndRel9(ueCb->cellId, ueCb->crnti,
                        WR_UMM_UTRA_TDD_EVNT_MEAS_SUPPORTED))
               {
                  tddMeas = TRUE;
               }
            } 
            else 
            {
               /* Check bit 22 */
               if(TRUE == wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti, 
                        WR_UMM_UTRA_MEAS_SUPPORTED))
               {
                  tddMeas = TRUE;
               }
            }
         }
         break; 
      default:
         tddMeas = FALSE;
         break;
   }
   RETVALUE(tddMeas);
}
/** @brief This function checks the UE capabilities and based on that
 *          will configure GERAN measurements for the UE.
 * */
PRIVATE S16 wrUmmMeasIratCfgGeranB2
(
 WrUeCb                       *ueCb,
 WrUmmCellCb                  *cellCb,
 WrUmmTransCb                 *transCb,
 Bool                         *isGapCfgdForIRat
 )
{
   WrUmmMeasTransInfo           *measTransCb = &transCb->u.measTransCb;
   WrHrlDecisionRet  forbRat = WR_MEAS_MBLTY_APPL_TO_RAT;

   /* Check whether GERAN RAT is forbidden or not */
   if((ueCb->ueServTyp == WR_UMM_NORMAL_SERV) && (ueCb->hoRstrctInfo != NULLP))
   {
      forbRat = wrChkForFrbRat(WR_FORB_RAT_GERAN, ueCb->hoRstrctInfo);
   }
   if(forbRat == WR_MEAS_MBLTY_NOT_APPL_TO_RAT)
   {
      RLOG0(L_ERROR, "wrUmmChkUeB2GERANCap: Handover restriction list\
            as GERAN in frdbInterRAT list");
      RETVALUE(RFAILED);
   }

   /*inter RAT HO to GERAN support by UE : Check in Ue capability*/
   if( TRUE == wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti,
                          WR_UMM_GERAN_MEAS_SUPPORTED))
   {
      wrUmmMeasCreatGeranCfgMsg(ueCb,cellCb,measTransCb,isGapCfgdForIRat);
   } 

   RETVALUE(ROK);
}
/** @brief This function checks the UE capabilities and based on that
 *          will configure UTRA FDD and UTRA TDD measurements for the 
 *          UE.
 * */
PRIVATE S16 wrUmmMeasIratCfgUtraB2
(
 WrUeCb                       *ueCb,
 WrUmmCellCb                  *cellCb,
 WrUmmTransCb                 *transCb,
 Bool                         *isGapCfgdForIRat
 )
{
   WrUmmMeasTransInfo           *measTransCb = &transCb->u.measTransCb;
   WrHrlDecisionRet  forbRat = WR_MEAS_MBLTY_APPL_TO_RAT;

   /* Check whether UTRA RAT is forbidden or not */
   if((ueCb->ueServTyp == WR_UMM_NORMAL_SERV) && (ueCb->hoRstrctInfo != NULLP))
   {
      forbRat = wrChkForFrbRat(WR_FORB_RAT_UTRA, ueCb->hoRstrctInfo);
   }
   if(forbRat == WR_MEAS_MBLTY_NOT_APPL_TO_RAT)
   {
      RLOG0(L_ERROR, "wrUmmChkUeB2UtraCap: Handover restriction list\
            as UTRA in frdbInterRAT list");
      RETVALUE(RFAILED);
   }

   /*inter RAT HO to UTRA support by UE : Check in Ue capability*/
   if(ROK == wrUmmChkUeB2UtraCap(ueCb->cellId,ueCb->crnti))
   {
      wrUmmMeasCreatUtraCfgMsg(ueCb,cellCb,measTransCb,
            isGapCfgdForIRat,WR_UMM_MEAS_TYPE_HO);
   } 

   /* UTRA TDD measurement configuration */
   if(ROK == wrUmmChkUeB2UtraTddCap(ueCb->cellId,ueCb->crnti))
   {
      wrUmmMeasCreatUtraTddCfgMsg(ueCb,cellCb, transCb,
            isGapCfgdForIRat,WR_UMM_MEAS_TYPE_REDIR);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for Configuring measurements for UE
 * with A1, A5 and B2 based on UE capabilities.
 * 
 * @details
 *
 * Function:wrUmmMeasEvntA2MeasConfig
 *
 *   Processing steps:
 *    - configure A1, A5 and B2 based on UE capabilities
 * @param[in]  recvMeasId measuremnt Identifier 
 * @param[in]  ueCb pointer to Ue Control block 
 * @param[in]  transCb pointer to transaction control block 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 **/
PRIVATE S16 wrUmmMeasEvntA2MeasConfig
(
 U32                          recvMeasId,
 WrUmmTransCb                 *transCb
)
{
   WrUeCb                       *ueCb;
   WrMeasUeCb                   *measUeCb;
   WrUmmMeasTransInfo           *measTransCb;
   WrUmmCellCb                  *cellCb;
   Bool                         isGapCfgd = FALSE;
   Bool                         isGapCfgdForIRat = FALSE;
   Bool                         isGapCfgdForUtra = FALSE;
   Bool                         isGapCfgdForGeran = FALSE;
   U16                           cellId;
/* LTE_ADV starts */
   WrCellCb                     *emmCellCb;
/* LTE_ADV ends */
   /* ccpu00133079 */
   U8                            measIdRmvCnt = 0;
   U8                            idx = 0; 
   WrUmmMeasGapStatus            measGapStatusForIntraLteHO = 
      WR_UMM_MEAS_GAP_NONE;       

   TRC2(wrUmmMeasEvntA2MeasConfig)

      ueCb = transCb->ueCb;
   cellId = ueCb->cellId;
   measUeCb = ueCb->measCfg;
   measTransCb = &transCb->u.measTransCb;

   WR_UMM_GET_CELLCB(cellCb, cellId)
      /* LTE_ADV starts */
      WR_GET_CELLCB(emmCellCb, cellId)
      if((NULLP == cellCb) || (emmCellCb == NULLP))
      {
         RLOG0(L_ERROR, "cellCb is NULL");
         RETVALUE(RFAILED);
      }
   /* LTE_ADV ends */
   /*Delete Event A2 Meas ID*/

   /* ccpu00133079: Multiple A2 report handling before
      Receiving RRC Reconfiguration complete received */
   measIdRmvCnt = measTransCb->rmvMeasInfo.measIdRmvLstCnt;
   for ( idx = 0; idx < measIdRmvCnt; idx++ )
   {
      if ( measTransCb->rmvMeasInfo.measIdRmvLst[idx] == recvMeasId )
      {
         RLOG0(L_WARNING, " MultipleA2 report- alreday recived an \
                  A2 report and processing it, hence ignoring ");
         RETVALUE(ROK);
      }
   }
   measTransCb->rmvMeasInfo.measIdRmvLst[measIdRmvCnt] = recvMeasId;
   measTransCb->rmvMeasInfo.measIdRmvLstCnt++;

/* LTE_ADV starts */
   /* Mark UE as cell edge and reconfigure schedular if ABS feature is enabled */
   ueCb->lteAdvCb.isUeCellEdge = TRUE;

/* LTE_ADV_SFR starts */ /* || */ /* LTE_ADV_DSFR starts */
   if(emmCellCb->lteAdvCb.lteAdvCfg.sfrCfg.status & RGR_ENABLE)
   {
       wrUmmHdlLteAdvRecfgForUe(cellCb, ueCb, RGR_ENABLE, 
                 WR_UMM_LTE_ADV_SFR_PROC, WR_UMM_LTE_ADV_LCL_UE);
       RLOG2(L_DEBUG, " SFR UE Recfg: UEID = %d, ISUECELLEDGE = %d ",
             ueCb->crnti, ueCb->lteAdvCb.isUeCellEdge);
   }
   else if(emmCellCb->lteAdvCb.lteAdvCfg.dsfrCfg.status & RGR_ENABLE)
   {
       wrUmmHdlLteAdvRecfgForUe(cellCb, ueCb, RGR_ENABLE, 
                 WR_UMM_LTE_ADV_DSFR_PROC, WR_UMM_LTE_ADV_LCL_UE);
       RLOG2(L_DEBUG, "DSFR UE Recfg: UEID = %d, ISUECELLEDGE = %d \n",
             ueCb->crnti, ueCb->lteAdvCb.isUeCellEdge); 
   }
/* LTE_ADV_SFR ends */ /* || */ /* LTE_ADV_DSFR ends */
/* LTE_ADV ends */

   /* Configure Event A1 */
   wrUmmMeasIntraCfgA1AfrRecvA2Rpt(ueCb, &measTransCb->addMeasInfo, cellCb);

   /*inter Freq Cell support by UE : Check in Ue capability*/
   if(TRUE == wrUmmUtilFeatureGrpInd(cellId, ueCb->crnti,
            WR_UMM_INTER_FREQ_MEAS_SUPPORTED))
   {
      wrUmmMeasInterCfgAfrRecvA2Rpt(ueCb, &measTransCb->addMeasInfo,
            cellCb, &isGapCfgd);
      if(isGapCfgd == TRUE)
      {
         wrUmmMeasAddMeasGap(WR_UMM_MEAS_GAP_INTER_FREQ_HO,cellCb, ueCb,
                  &measTransCb->measGapStatus, transCb->transId);
         measGapStatusForIntraLteHO = measTransCb->measGapStatus;
      }
   }
   /*ccpu00133426 - inter freq ho requirement - start*/
   else
   {
      RLOG0(L_ERROR, " UE doesn't support inter frequency meas ");
   }
   /*ccpu00133426 - inter freq ho requirement - end*/

   if(ROK == wrUmmMeasIratCfgUtraB2(ueCb, cellCb, transCb, &isGapCfgdForUtra))
   {
      RLOG0(L_DEBUG, " B2 measurement configuration done for UTRAN");
   }
   if(ROK == wrUmmMeasIratCfgGeranB2(ueCb, cellCb, transCb, &isGapCfgdForGeran))
   {
      RLOG0(L_DEBUG, " B2 measurement configuration done for GERAN");
   }
   if((isGapCfgdForUtra == TRUE) || (isGapCfgdForGeran == TRUE))
   {
      isGapCfgdForIRat = TRUE;
   }
   if(TRUE == isGapCfgdForIRat)
   {
      /* Calling the MeasGAP API for configuring Meas gap for Inter RAT HO */
      wrUmmMeasAddMeasGap(WR_UMM_MEAS_GAP_INTER_RAT_HO,
            cellCb, ueCb, &measTransCb->measGapStatus,
            transCb->transId); 
   }
   /* ccpu00131966 : Configure measurement gap to scheduler  */
   if ( ( measUeCb->gapMeasInfo.latestTxId == transCb->transId) && 
         ( ( WR_UMM_MEAS_GAP_INIT_SCH_CONFIG == measTransCb->measGapStatus) ||
           ( WR_UMM_MEAS_GAP_INIT_SCH_CONFIG == measGapStatusForIntraLteHO)))
   {
      measTransCb->measGapStatus = WR_UMM_MEAS_GAP_INIT_SCH_CONFIG;
      wrUmmMeasGapSchdUeRecfgReq(transCb);
   }

   else
   {
      /* Prepare and send the message to the RRC layer */
      if(ROK != wrUmmMeasPrepareSendRrcRecfg(cellId, ueCb, 
               &transCb->u.measTransCb, 
               transCb->transId))
      {
         wrUmmTransComplete(transCb);
         RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to get measurement Id object from
 * UE Cb.
 *
 *  @details*
 *  Function:wrUmmMeasGetMeasIdObj
 *
 *  Processing steps:
 *      - search in meas id list using measId.
 *      - And return the measurement Object
 *
 *   @param[in] measID: measurement Id.
 *   @param[out] measIdObj: pointer to measurement Id Node.
 *   @param[in] measIdLst: pointer to measurement Id List.
 *   @return S16
 *       SUCCESS : ROK
 *       FAILED : RFAILED
 */
PUBLIC S16 wrUmmMeasGetMeasIdObj
(
 U32                           measId,
 WrMeasIdNode                 **measIdObj,
 CmLListCp                    *measIdLst
 )     /* KW fix for LTE_ADV */
{
   WrMeasIdNode              *measIdNode = NULLP;
   CmLList                   *measIdLnk;

   TRC2(wrUmmMeasGetMeasIdObj)

   measIdLnk = measIdLst->first;
   while(NULLP != measIdLnk)
   {
      measIdNode = (WrMeasIdNode*)measIdLnk->node;
      if((measIdNode != NULLP) && (measIdNode->measID == measId))
      {
         *measIdObj = measIdNode;
         RETVALUE(OK);
      }
      measIdLnk = measIdLnk->next;
   }
   RETVALUE(RFAILED);
}

/** @brief: This function will compare the PSC received from UE
 *          with the Cell List present in the measObjNode. 
 *          If the PSC reported by UE is present in Cell Lst of
 *          measObjNode return TRUE else return FALSE.
 *
 * */
PUBLIC Bool wrUmmChkIfPscPrsntInMeasLst
(
 WrUmmMeasUtraMeasObjNode     *ueMeasObjNode,
 U32                          psc
 )
{
   WrMeasNrUtraCellNode         *cellNode = NULLP;

   cmLListFirst(&ueMeasObjNode->cellLst);
   while(cmLListCrnt(&ueMeasObjNode->cellLst))
   {
      cellNode = (WrMeasNrUtraCellNode *) cmLListNode(cmLListCrnt(&ueMeasObjNode->cellLst));
      if(cellNode->psc == psc)
      {
         RETVALUE(TRUE);
      }
      cmLListNext(&ueMeasObjNode->cellLst);
   }
   RETVALUE(FALSE);
}

/**   @brief: This function is used to handle measurement reports
 *            received for CSFB and Redirection towards UTRA TDD.
 *            Validate if PSCs received in measRpt are listed and then
 *            populate the utraTddRedirInfo.
 *            For CSFB if the current received measRpt has higher RSCP
 *            than the previously received measRpt, then store the 
 *            current measRpt PSCs and ARFCN.
 * */
PUBLIC S16 wrUmmUtraTddHdlMeasRpt
(
 WrUeCb               *ueCb,
 NhuMeasResults       *measRpt,
 WrUmmRedirectInfo    **redirInfo,
 WrUmmMeasUtraMeasObjNode     *ueMeasObjNode,
 WrUmmMeasTyp          measTyp
 )
{
   U8                      idx = 0;
   U8                      redirCnt = 0;
   S16                     prevRscp;
   TknStrOSXL              utraSI;
   NhuMeasResultLstUTRA    *utraMeasRslt = NULLP;
   NhuMeasResultUTRA       *measUtra = NULLP;
   WrUmmMeasRptInfo        measRptInfo;
   WrRedirectionUtraSupp		redirType;
   WrUmmRedirectInfo       *utraTddRedirInfo = NULLP;
   Bool                    fillAllCells = FALSE;
 
   RLOG0(L_DEBUG, "wrUmmUtraTddHdlMeasRpt Called");
   if(NULLP != (*redirInfo))
   {
      utraTddRedirInfo = (*redirInfo);
   }

   if(wrUmmMeasGetArfcnAndRptType(ueCb->ueIdx, ueCb->cellId,
            measRpt,&measRptInfo) != ROK)
   {
      RETVALUE(RFAILED);
   }

   utraMeasRslt = &measRpt->measResultNeighCells.val.measResultLstUTRA;

   if(utraMeasRslt->noComp.val != 0)
   {
      for(idx = 0; idx < utraMeasRslt->noComp.val; idx++)
      {
         measUtra = &utraMeasRslt->member[idx];

         if((measUtra->pres.pres == PRSNT_NODEF) &&
               (measUtra->physCellId.choice.val == PHYSCELLID_TDD)) 
         {
            /* Check whether the reported PSC is listed in the 
             * measObj or not. */
            if(FALSE == wrUmmChkIfPscPrsntInMeasLst(ueMeasObjNode, 
                     measUtra->physCellId.val.tdd.val))
            {
               /* UE reported a PSC which is not listed, so ignore this PSC */
               continue;
            }

            if((measUtra->measResult.utra_RSCP.val <= WR_UMM_UTRA_MIN_RSCP_VAL) || 
                  (measUtra->measResult.utra_RSCP.val > WR_UMM_UTRA_MAX_RSCP_VAL))
            {
               RLOG0(L_ERROR, "RSCP Value is Less then 6:: Ghost UTRA Cells\n");
               break;
            }

            if(NULLP == (*redirInfo))
            {
               WR_ALLOC(&utraTddRedirInfo, sizeof(WrUmmRedirectInfo));
               if (NULLP == utraTddRedirInfo)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  RETVALUE(RFAILED);
               }

               utraTddRedirInfo->cellId = ueCb->cellId;
               utraTddRedirInfo->ratTyp = WR_UMM_MEAS_UTRA_TDD_FREQ;    

               /* Fill eRedirection Support based on ueCap */
               wrChkUtranTddERedirSupp(ueCb->ueCap, &redirType);
               if (redirType == WR_REDIRECTION_REL9_SUPP)
               {
                  RLOG0(L_DEBUG, "UE Supports eRedirUtraR9Sup");
                  utraTddRedirInfo->eRedirUtraR9Sup = TRUE;
               }

               if (redirType == WR_REDIRECTION_REL10_SUPP)
               {
                  RLOG0(L_DEBUG, "UE Supports eRedirUtraR10Sup");
                  utraTddRedirInfo->val.utraTddInfo.eRedirUtraR10Sup = TRUE;
               }

               utraTddRedirInfo->val.utraTddInfo.arfcnTddLst[0] = measRptInfo.arfcn.t.carrierFreq;
               utraTddRedirInfo->val.utraTddInfo.noOfTddArfcn = 1;
            }

            if((WR_UMM_MEAS_TYPE_CSFB == measTyp) && (fillAllCells == FALSE))
            {
               /* Only the 1st PSCs RSCP is checked because the UE when sends 
                * measurement report it will send the PSC with highest RSCP 1st. */
               prevRscp = utraTddRedirInfo->val.utraTddInfo.utraCellRedirInfo[redirCnt].rscp;
               if(prevRscp <= measUtra->measResult.utra_RSCP.val)
               {
                  /* Clear PSC list of RedirInfo and set nosOfPSCs as 0. */
                  WR_SET_ZERO(utraTddRedirInfo->val.utraTddInfo.utraCellRedirInfo,
                        WR_UMM_MAX_UTRA_CELLS_WITH_SI*sizeof(WrUmmUtraTddCellRedirInfo));
                  utraTddRedirInfo->val.utraTddInfo.noOfPscs = 0;
                  utraTddRedirInfo->val.utraTddInfo.arfcnTddLst\
                     [utraTddRedirInfo->val.utraTddInfo.noOfTddArfcn - 1]= measRptInfo.arfcn.t.carrierFreq;
                  fillAllCells =TRUE;
               }
               else
               {
                  /* The already existing frequency has highest RSCP than the currently received RSCP. 
                   * Thus no need to fill this frequency PSC info or redirection. */
                  break;
               }
            }

            utraTddRedirInfo->val.utraTddInfo.utraCellRedirInfo
               [utraTddRedirInfo->val.utraTddInfo.noOfPscs].pscId =
               measUtra->physCellId.val.tdd.val;

            utraTddRedirInfo->val.utraTddInfo.utraCellRedirInfo
               [utraTddRedirInfo->val.utraTddInfo.noOfPscs].rscp =
               measUtra->measResult.utra_RSCP.val;

            utraTddRedirInfo->val.utraTddInfo.utraCellRedirInfo
               [utraTddRedirInfo->val.utraTddInfo.noOfPscs].arfcn =
               measRptInfo.arfcn.t.carrierFreq;
            RLOG3(L_DEBUG, "PSC = %d, RSCP = %d, ARCFN = %d",
                            measUtra->physCellId.val.tdd.val,
                            measUtra->measResult.utra_RSCP.val,
                            measRptInfo.arfcn.t.carrierFreq);

            if(TRUE == wrEmmGetNeighUtraTddSIInfo(measUtra->physCellId.val.tdd.val, 
                     measRptInfo.arfcn.t.carrierFreq, utraTddRedirInfo->cellId, &utraSI))
            {
               RLOG0(L_DEBUG, "UTRA SI is Present");
               utraTddRedirInfo->val.utraTddInfo.utraCellRedirInfo
                  [utraTddRedirInfo->val.utraTddInfo.noOfPscs].utraSI = utraSI;

            } /* utraSI presence check */

            utraTddRedirInfo->val.utraTddInfo.noOfPscs++;
         }
      } /* End of for */

      if(utraTddRedirInfo == NULLP)
      {
         RLOG0(L_ERROR, "MR: utraTddRedirInfo is NULLP, All PSCs are invalid ");
         /* All PSCs received are invalid */
         RETVALUE(RFAILED);
      }

      (*redirInfo) = utraTddRedirInfo;
   }
   else
   {
      RLOG0(L_ERROR, "MR: No PSCs received ");
      /* No PSCs received */
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

PRIVATE S16 wrUmmHdlGeranB2MeasRpt
(
 WrUeCb                      *ueCb,
 NhuMeasResults              *measRpt,
 WrUmmRedirectInfo           **redirInfo,
 U32                          measType
 )
{
   NhuMeasResultLstGERAN    *geranMeasRes = NULLP;
   NhuMeasResultGERAN       *member = NULLP;
   WrUmmGeranCellCb         *cellNode = NULLP;
   U8                        ncc = 0x00;
   U8                        bcc = 0x00;
   Bool                      eRedir_r9 = FALSE;
   NhuIRAT_ParamsGERAN_v920  *geranV920 = NULLP;
   U32                        idx = 0;
   U32                        idx1 = 0;
   U8                         arfcnCnt=0;
   WrHrlDecisionRet           hrlDec = WR_MEAS_MBLTY_APPL_TO_CELL;
   
   TRC2(wrUmmHdlGeranB2MeasRpt)

   geranV920 = &ueCb->ueCap->eUTRA_Cap.nonCriticalExtn.interRAT_ParamsGERAN_v920;
   if((NhuAccessStratumRlsrel9Enum <= ueCb->ueCap->eUTRA_Cap.accessStratumRls.val)
      && (PRSNT_NODEF == geranV920->pres.pres) 
      && (PRSNT_NODEF == geranV920->e_RedirectionGERAN_r9.pres))
   {
      eRedir_r9 = TRUE;
   }
   geranMeasRes = &measRpt->measResultNeighCells.val.measResultLstGERAN;
   member = geranMeasRes->member;

   if(measRpt->measResultNeighCells.choice.val != MEASRESULTNEIGHCELLS_MEASRESULTLSTGERAN)
   {
      RLOG0(L_ERROR, "invalid measurement report received");
      RETVALUE(RFAILED);
   }
   /* TODO: Take care of freeing the memory */ 
   WR_ALLOC(redirInfo, sizeof(WrUmmRedirectInfo));
   if (NULLP == *redirInfo)
   {
      RLOG0(L_FATAL, "Failed to allocate memory.");
      RETVALUE(RFAILED);
   }
   (*redirInfo)->ratTyp = WR_UMM_MEAS_GERAN_FREQ; 
   for(idx = 0; ((idx < geranMeasRes->noComp.val) && (idx < LWR_MAX_NUM_NEIGH_GERAN_CELLS)); idx++)
   {
      ncc = *member[idx].physCellId.networkColourCode.val;
      bcc = *member[idx].physCellId.baseStationColourCode.val;
      /* ASN Decoder gives the bit strings in big-endian, make them little
       * endian. ncc and bcc are three bit length */
      ncc >>= 5;
      bcc >>= 5;
      cellNode = (WrUmmGeranCellCb *)wrEmmAnrGetGeranNbr(ueCb->cellId, \
            member[idx].carrierFreq.arfcn.val,
            member[idx].carrierFreq.bandIndicator.val, ncc, bcc );
      if ( cellNode != NULLP )
      {
         /* Verify HRL restriction */
         if((ueCb->hoRstrctInfo != NULLP) && (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
         {
            hrlDec = wrMbltyRestrc(WR_INTER_SYS_GERAN_MBLTY, (PTR *)cellNode,\
                          NULLP, NULLP, NULLP, ueCb->hoRstrctInfo);
            if(hrlDec != WR_MEAS_MBLTY_APPL_TO_CELL)
            {
               RLOG0(L_INFO, "HRL: Mobility to the neighbour is restricted");
               continue;
            }
         }
         (*redirInfo)->val.geranInfo.cellInfoList[arfcnCnt].arfcn = 
            cellNode->freqCb->arfcn;
         /*Default value of redirInfo.bandIndicator = WR_GERAN_DCS1800
          * This will be filled in wrUmmFillNhuGeranRedirectInfo */
         if(WR_GERAN_PCS1900 == cellNode->freqCb->bandInd)
         {
            (*redirInfo)->val.geranInfo.bandInd = WR_GERAN_PCS1900;
         }
         (*redirInfo)->val.geranInfo.cellInfoList[arfcnCnt].bsic = \
                                                   cellNode->bsic;
         /* siPsiType 0:SI 1:PSI */
         if((eRedir_r9) && (!cellNode->siPsiType) && 
               (0 < cellNode->noOfsiPsi))/*Fill only SIs.*/
         {
            (*redirInfo)->val.geranInfo.cellInfoList[arfcnCnt].noOfsiPsi = 
               cellNode->noOfsiPsi;
            for(idx1 = 0;idx1 < cellNode->noOfsiPsi; idx1++)
            {
                (*redirInfo)->val.geranInfo.cellInfoList[arfcnCnt].siListGeran[idx1] =
                   cellNode->siOrPsi[idx1];
            }
         }
         (*redirInfo)->val.geranInfo.cellInfoCnt = ++arfcnCnt;

      }
      else
      {
         continue;
      } 
   }
   if(!arfcnCnt)
   {
      WR_FREE(*redirInfo,sizeof(WrUmmRedirectInfo));
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
} /* wrUmmHdlGeranB2MeasRpt */

/** @brief: This function is responsible for handling the 
 *          GERAN measurement report received for redirection.
 *          Based on the IRAT type call the respective RAT measurement
 *          handlers.
 * */
PUBLIC S16 wrUmmIRatGeranRedirHdlMeasRpt
(
 WrUeCb                       *ueCb,
 U8                           event,
 NhuMeasResults               *measRprt
)
{
   Bool                    vldB2MeasRpt = FALSE;
   WrUmmRedirectInfo       *redirInfo = NULLP;
   WrUmmInternalMsg        *internalMsg = NULLP;
   WrUmmIncMsg             *msg = NULLP;
   WrUmmCellCb             *cellCb = NULLP;

   RLOG0(L_DEBUG, "wrUmmIRatGeranRedirHdlMeasRpt called");
   if(WR_HO_NORMAL == ueCb->mobCtrlState)                                       
   { 
      WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
      if (cellCb == NULLP)
      {
         RLOG0(L_ERROR, "Failed To Get UMM Cell Cb.");
         RETVALUE(RFAILED);
      }

      switch(event)
      {
         case WR_UMM_MEAS_RPTCFG_IRATB2_REDIR:
            {
               if ( ROK == wrUmmHdlGeranB2MeasRpt(ueCb, measRprt, &redirInfo, WR_UMM_MEAS_TYPE_REDIR) )
               {
                  vldB2MeasRpt = TRUE;
               }
            }
            break;
         default:
            RETVALUE(RFAILED);
      }

      /* Valid B2 measRpt received.
       * Post internal message to Lcl Rls transaction and pass the 
       * Redirection information. */
      if(vldB2MeasRpt == TRUE)
      {
         RLOG0(L_DEBUG, "Calling wrUmmBldInternalMsg for GERAN Redirection");
         if (ROK != wrUmmBldInternalMsg(&internalMsg, WR_UMM_INTMSG_REDIR_TRANS, (Void *)redirInfo))
         {
            RLOG0(L_ERROR, "building internal message error.");
            RETVALUE(RFAILED);          
         }
         WR_ALLOC(&msg,sizeof(WrUmmIncMsg));
         if (NULLP == msg)
         {
            RLOG0(L_FATAL, "Failed to allocate memory.");
            RETVALUE(RFAILED);
         }
         msg->msgTyp = WR_UMM_INTERNAL_MSG;
         msg->u.internalMsg = internalMsg;
         msg->refCnt = 1;
         wrUmmPrcIncMsg(cellCb, ueCb, msg);
         wrUmmRlsIncMsg(&msg);

         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
}
/** @brief: This function is responsible for handling the 
 *          IRAT measurement report received for redirection.
 *          Based on the IRAT type call the respective RAT measurement
 *          handlers.
 *          Currently only UTRA TDD handler is being called.
 * */
PRIVATE S16 wrUmmIRatRedirHdlMeasRpt
(
 WrUeCb                       *ueCb,
 WrMeasRptCfgType             event,
 NhuMeasResults               *measRprt,
 WrUmmMeasUtraMeasObjNode     *ueMeasObjNode
 )
{
   Bool                    vldB2MeasRpt = FALSE;
   WrUmmRedirectInfo       *redirInfo = NULLP;
   WrUmmMeasRptInfo        measRptInfo;
   WrUmmInternalMsg        *internalMsg = NULLP;
   WrUmmIncMsg             *msg = NULLP;
   WrUmmCellCb             *cellCb = NULLP;

   RLOG0(L_DEBUG, "wrUmmIRatRedirHdlMeasRpt called");
   if(WR_HO_NORMAL == ueCb->mobCtrlState)                                       
   { 
      WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
      if (cellCb == NULLP)
      {
         RLOG0(L_ERROR, "Failed To Get UMM Cell Cb.");
         RETVALUE(RFAILED);
      }

      if(wrUmmMeasGetArfcnAndRptType(ueCb->ueIdx, ueCb->cellId,
               measRprt,&measRptInfo) != ROK)
      {
         RETVALUE(RFAILED);
      }

      switch(measRptInfo.cellTyp)
      {
         case WR_UMM_MEAS_UTRA_TDD_FREQ:
            {
               if(ROK == wrUmmUtraTddHdlMeasRpt(ueCb, measRprt, &redirInfo, 
                              ueMeasObjNode, WR_UMM_MEAS_TYPE_REDIR))
               {
                  vldB2MeasRpt = TRUE;
               }
            }
            break;
         default:
            RETVALUE(RFAILED);
      }

      /* Valid B2 measRpt received.
       * Post internal message to Lcl Rls transaction and pass the 
       * Redirection information. */
      if(vldB2MeasRpt == TRUE)
      {
         RLOG0(L_DEBUG, "Calling wrUmmBldInternalMsg for IRAT UTRA TDD Redirection");
         if (ROK != wrUmmBldInternalMsg(&internalMsg, WR_UMM_INTMSG_REDIR_TRANS, (Void *)redirInfo))
         {
            RLOG0(L_ERROR, "building internal message error.");
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
         wrUmmPrcIncMsg(cellCb, ueCb, msg);
         wrUmmRlsIncMsg(&msg);

         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
}

/** @brief This function is responsible for Handling measurement report
 * received from UE.
 * 
 * @details
 *
 * Function:wrUmmMeasHdlMeasRpt
 *
 *   Processing steps:
 *    - Get Measurement Id from received measurement report.
 *    - From Measaurement Id get the measurement Object Id and report config Id.
 *    - From measurement Object Id find the cell type and carrier frequency info.
 *    - From report config Id, find report config Type.
 *    - If Measurement report occurs for event A2, Configure Event A1 and Event
 *    A5.
 *    - If Measurement report occurs for event A1, Configure Event A2 and remove
 *    measurement Id's for A1 and A5.
 *     
 * @param[in]  transCb pointer to Transaction control block 
 * @param[in]  measRprt pointer to Measurement report 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasHdlMeasRpt 
(
WrUmmTransCb                 *transCb,
NhuMeasResults               *measRprt
)
{
   U8                           measId;
   U8                           measObjId;
   U8                           rptCfgId;
   U32                          recvMeasId;
   WrMeasRptCfgType             rptCfgTyp = 0;
   WrUmmMeasIntRptCfgTyp        intRptCfgTyp = WR_UMM_MEAS_RPTCFG_NONE;
   WrMeasUeCb                   *measUeCb;
   LwrCellMeasCfgGrp            *measCellCfg; 
   WrUmmMeasTransInfo              *measTransCb;
   WrUmmMeasObjInfo              measObjInfo = {0};
   WrUmmCellCb                  *cellCb;
   WrUeCb                       *ueCb;
   U16                           cellId;
   WrMeasIdNode                 *measIdObj = NULLP;
   WrUmmMeasGapStatus            measGapStatusForIntraLteHO = 
      WR_UMM_MEAS_GAP_NONE;       
   /* LTE_ADV starts */
   WrCellCb                     *emmCellCb;
   /* LTE_ADV ends */

   TRC2(wrUmmMeasHdlMeasRpt)

      ueCb = transCb->ueCb;
   if ( (ueCb == NULLP) || (ueCb->measCfg == NULLP) )
   {
      RLOG0(L_ERROR, " ueCb/ueCb->measCfg  is NULL ");
      RETVALUE(RFAILED);
   }

   cellId = ueCb->cellId;
   measUeCb = ueCb->measCfg;
   measTransCb = &transCb->u.measTransCb;

   recvMeasId = measRprt->measId.val;
   wrUmmMeasGetMeasIdObj(recvMeasId, &measIdObj, &ueCb->measCfg->measIdLst);
   
   WR_UMM_GET_CELLCB(cellCb, cellId)
      /* LTE_ADV starts */
      WR_GET_CELLCB(emmCellCb, cellId)
      if((NULLP == cellCb) || (NULLP == emmCellCb))
      {
         RLOG0(L_ERROR, "cellCb is NULL");
         RETVALUE(RFAILED);
      }
   /* LTE_ADV ends */
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
   if( ROK != wrUmmMeasFindRptCfgTyp(rptCfgId, &rptCfgTyp, &measUeCb->rptCfgLst))
   {
      RLOG0(L_ERROR, "report config object not found");
      /*Print error message*/
      RETVALUE(RFAILED);
   }
   switch (rptCfgTyp)
   {
      case WR_UMM_MEAS_EVENT_A1:
         {

            /* LTE_ADV_ABS starts */
            /* Mark UE as cell center and reconfigure schedular if ABS feature is enabled */
            WrEmmAbsEvent                        emmAbsEvent;
            ueCb->lteAdvCb.isUeCellEdge        = FALSE;
            ueCb->lteAdvCb.absInfo.isAbsUe     = FALSE;
            emmAbsEvent.eventId                = WR_EMM_ABS_NONABS_UE_EVENT;
            emmAbsEvent.u.absUeEvent.ueId      = ueCb->crnti;
            emmAbsEvent.u.absUeEvent.cellId    = ueCb->cellId;
            emmAbsEvent.u.absUeEvent.numOfCell = 0;
            wrEmmAbsProcessEvent(&emmAbsEvent);
            /* LTE_ADV_ABS ends */

            /* LTE_ADV_SFR starts */ /* || */ /* LTE_ADV_DSFR starts */
            if(emmCellCb->lteAdvCb.lteAdvCfg.sfrCfg.status & RGR_ENABLE)
            {
               wrUmmHdlLteAdvRecfgForUe(cellCb, ueCb, RGR_ENABLE,
                     WR_UMM_LTE_ADV_SFR_PROC, WR_UMM_LTE_ADV_LCL_UE);
            }
            else if(emmCellCb->lteAdvCb.lteAdvCfg.dsfrCfg.status & RGR_ENABLE)
            {
               wrUmmHdlLteAdvRecfgForUe(cellCb, ueCb, RGR_ENABLE,
                     WR_UMM_LTE_ADV_DSFR_PROC, WR_UMM_LTE_ADV_LCL_UE);
            }
            /* LTE_ADV_SFR ends */ /* || */ /* LTE_ADV_DSFR ends */

            /* Calling the MeasGAP release API for Inter freq HO */
            wrUmmMeasRelMeasGap (WR_UMM_MEAS_GAP_INTER_FREQ_HO,
                  cellCb, ueCb,&measTransCb->measGapStatus, transCb->transId);

            measGapStatusForIntraLteHO = measTransCb->measGapStatus; 

            /* Calling the MeasGAP release API for Inter RAT HO */
            wrUmmMeasRelMeasGap (WR_UMM_MEAS_GAP_INTER_RAT_HO,
                  cellCb, ueCb,&measTransCb->measGapStatus, transCb->transId);
            /* Configure Event A2 and delete Event A1, A5 and Event B2*/
            wrUmmMeasCfgForEvntA1(ueCb, &measTransCb->rmvMeasInfo,
                  &measTransCb->addMeasInfo, measCellCfg);

            /* LTE_ADV_ABS starts */
            wrUmmIntraMeasCfgForLteAdvDelEvntA3(measUeCb, &measTransCb->rmvMeasInfo, 
                  cellCb, WR_UMM_MEAS_EVENT_A3_ABS);
            /* LTE_ADV_ABS ends */

            /* If any one of the status is REL, then Release the meas gap
             * else if both are NONE then do nothing
             * if none of the above then reconfigure the meas gap to SCH & UE
             */
            if(( ueCb->measCfg->gapMeasInfo.latestTxId == transCb->transId) && 
                  (( WR_UMM_MEAS_GAP_REL == measTransCb->measGapStatus) ||
                   ( WR_UMM_MEAS_GAP_REL == measGapStatusForIntraLteHO)))        
            {
               measTransCb->measGapStatus = WR_UMM_MEAS_GAP_REL;
            }
            else if((ueCb->measCfg->gapMeasInfo.latestTxId == 
                     transCb->transId) &&
                  !(( WR_UMM_MEAS_GAP_NONE == measTransCb->measGapStatus) &&
                     ( WR_UMM_MEAS_GAP_NONE == measGapStatusForIntraLteHO)))
            {
               /* configure Meas Gap so sending messge to SCH */
               measTransCb->measGapStatus = WR_UMM_MEAS_GAP_INIT_SCH_CONFIG;
               wrUmmMeasGapSchdUeRecfgReq(transCb);
               RETVALUE(ROK);
            }
            else 
            {
               measTransCb->measGapStatus = WR_UMM_MEAS_GAP_NONE;
            }

            if(ROK != wrUmmMeasPrepareSendRrcRecfg(cellId, 
                     ueCb, measTransCb, 
                     transCb->transId))
            {
               wrUmmTransComplete(transCb);
               RETVALUE(RFAILED);
            }
               {
                  RLOG_ARG0(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Received A1 MeasurementReport ");
               }
            break;
         }
      case WR_UMM_MEAS_EVENT_A2:
         {
            /* If UE is NOT in CSFB_READY state i.e. if CSFB is going on 
             * for that UE, no action is taken on A2 measurement report*/
            if(WR_CSFB_GSM_UE_STATE_CSFB_READY != ueCb->csfbInfo.csfbState)
            {
               RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti," Received A2 "
                     "MeasurementReport. Ignoring this report "
                     "as CSFB has been triggered for this UE");
               break;
            }

            if ( ROK != wrUmmMeasEvntA2MeasConfig(recvMeasId, transCb))
            {
               wrUmmTransComplete(transCb);
               RETVALUE(RFAILED);
            }
            {
               RLOG_ARG0(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Received A2 MeasurementReport ");
            }
            break;
         }
      case WR_UMM_MEAS_EVENT_A3:
         {
            /* If UE is NOT in CSFB_READY state i.e. if CSFB is going on 
             * for that UE, no action is taken on A3 measurement report*/
            if(WR_CSFB_GSM_UE_STATE_CSFB_READY != ueCb->csfbInfo.csfbState)
            {
               RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti," Received A3 "
                     "MeasurementReport. Ignoring this report "
                     "as CSFB has been triggered for this UE");
               break;
            }
            wrUmmHoHdlMeasRprt (ueCb, rptCfgTyp, measRprt, measObjInfo.eutraObj->eutraMeasObj->carrFreq);
            wrUmmTransComplete(transCb);
            {
               RLOG_ARG0(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Received A3 MeasurementReport ");
            }
            break;
         }
      case WR_UMM_MEAS_EVENT_A5:
         {
            /* If UE is NOT in CSFB_READY state i.e. if CSFB is going on 
             * for that UE, no action is taken on A5 measurement report*/
            if(WR_CSFB_GSM_UE_STATE_CSFB_READY != ueCb->csfbInfo.csfbState)
            {
               RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti," Received A5 "
                     "MeasurementReport. Ignoring this report "
                     "as CSFB has been triggered for this UE");
               break;
            }
            wrUmmHoHdlMeasRprt (ueCb, rptCfgTyp, measRprt, measObjInfo.eutraObj->eutraMeasObj->carrFreq);
            wrUmmTransComplete(transCb);
            {
               RLOG_ARG0(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Received A5 MeasurementReport ");
            }
            break;
         }
      case WR_UMM_MEAS_EVENT_B2:
         {
            /* If UE is NOT in CSFB_READY state i.e. if CSFB is going on 
             * for that UE, no action is taken on B2 measurement report*/
            if(WR_CSFB_GSM_UE_STATE_CSFB_READY != ueCb->csfbInfo.csfbState)
            {
               RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti," Received B2 "
                     "MeasurementReport. Ignoring this report "
                     "as CSFB has been triggered for this UE");
               break;
            }
            wrUmmTransComplete(transCb);

            {
               RLOG_ARG0(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Received B2 MeasurementReport ");
            }
            break;
         }
      case WR_UMM_MEAS_EVENT_B2_UTRA:
         {
            /* If UE is NOT in CSFB_READY state i.e. if CSFB is going on 
             * for that UE, no action is taken on B2 UTRA measurement report*/
            if(WR_CSFB_GSM_UE_STATE_CSFB_READY != ueCb->csfbInfo.csfbState)
            {
               RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti,"B2 UTRA"
                     "measurement report received. Ignoring this report "
                     "as CSFB has been triggered for this UE");
               break;
            }
            if( ROK != wrUmmMeasFindIntRptCfgTyp(rptCfgId, &intRptCfgTyp, &measUeCb->rptCfgLst))
            {
               RETVALUE(RFAILED);
            }
            if(WR_UMM_MEAS_RPTCFG_IRATB2_REDIR == intRptCfgTyp)
            {
               /* If HRL is present then fetch HRL Obj */
               if(ueCb->hoRstrctInfo != NULLP)
               {
                  WrUmmMeasUtraMeasObjNode      *hrlMeasObj = NULLP;

                  hrlMeasObj = wrUmmHrlUtraMeasObjCfg\
                               (measObjInfo.utraTddObj->utraMeasObj, ueCb);
                  if(NULLP == hrlMeasObj)
                  {

                     RLOG0(L_ERROR, "HrlMeasObj Measurement is NULL");
                     RETVALUE(RFAILED);
                  }
                  wrUmmIRatRedirHdlMeasRpt(ueCb, rptCfgTyp, measRprt,
                        hrlMeasObj);
               }
               else
               {
                  wrUmmIRatRedirHdlMeasRpt(ueCb, rptCfgTyp, measRprt,
                        measObjInfo.utraTddObj->utraMeasObj);
               }
            }
            else
            {
               wrUmmHoHdlMeasRprt ( ueCb, rptCfgTyp, measRprt, 
                     measObjInfo.utraFddObj->utraMeasObj->arfcn );
            }
            wrUmmTransComplete(transCb);
            {
               RLOG_ARG0(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Received B2_UTRA MeasurementReport ");
            }
            break;
         }
      case WR_UMM_MEAS_EVENT_B2_GERAN:
         {

            RLOG_ARG0(L_DEBUG, DBG_CRNTI,ueCb->crnti,"Received B2 GERAN report for UE");
            /* If UE is NOT in CSFB_READY state i.e. if CSFB is going on 
             * for that UE, no action is taken on B2 UTRA measurement report*/
            if(WR_CSFB_GSM_UE_STATE_CSFB_READY != ueCb->csfbInfo.csfbState)
            {
               RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti,"B2 GERAN"
                     "measurement report received. Ignoring this report "
                     "as CSFB has been triggered for this UE");
               break;
            }
            if( ROK != wrUmmMeasFindIntRptCfgTyp(rptCfgId, &intRptCfgTyp, &measUeCb->rptCfgLst))
            {
               RETVALUE(RFAILED);
            }
            if(WR_UMM_MEAS_RPTCFG_IRATB2_REDIR == intRptCfgTyp)
            {
               RLOG1(L_DEBUG,"Received Geran Measurement report for redirection "
                                               "for Ue crnti %d", ueCb->crnti);
               wrUmmIRatGeranRedirHdlMeasRpt(ueCb, intRptCfgTyp, measRprt);
            }
            else
            {
               RLOG1(L_DEBUG,"Received Geran Measurement report for Handover "
                                               "for Ue crnti %d", ueCb->crnti);
               /* Here passing Arfcn as '0' because Arfcn shall present in Geran
                * measurement report and no need to pass Arfcn's explicitly*/
               wrUmmHoHdlMeasRprt(ueCb, WR_UMM_MEAS_EVENT_B2_GERAN, measRprt,0);
            }
            wrUmmTransComplete(transCb);
            break;
         }
         /* LTE_ADV_ABS starts */
      case WR_UMM_MEAS_EVENT_A3_ABS:
         {
            RLOG1(L_DEBUG, " Received A3_ABS report for UE : %d", ueCb->crnti); 
            /* If UE is NOT in CSFB_READY state i.e. if CSFB is going on 
             * for that UE, no action is taken on A3_ABS measurement report*/
            if(WR_CSFB_GSM_UE_STATE_CSFB_READY != ueCb->csfbInfo.csfbState)
            {
               RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti," Received A3_ABS"
                     "MeasurementReport. Ignoring this report "
                     "as CSFB has been triggered for this UE");
               break;
            }
            wrUmmAbsPrcHdlMeasRpt(ueCb, measRprt, measObjInfo.eutraObj->eutraMeasObj->carrFreq);
            wrUmmTransComplete(transCb);
            {
               RLOG_ARG0(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Received A3_ABS MeasurementReport ");
            }
            break;
         }
         /* LTE_ADV_ABS ends */
      default:
         RLOG_ARG0(L_ERROR, DBG_CRNTI,ueCb->crnti,"Received invalid report type");
      break;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to update measurement object
 *  from cell cb. 
 *
 *  @details
 * Function:wrUmmMeasUpdtCellEutraMeasObjFromArfcn
 *
 *   Processing steps:
 *    - tranverse cell Cb list find measurement object and id using Arfcn. 
 *     
 * @param[in]  arfcn: Arfcn         
 * @param[in]  measCellCb: pointer to measurement Ue control block         
 * @param[out]  cellCbMeasObj: pointer to cell Cb measurement Object 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasUpdtCellEutraMeasObjFromArfcn
(
U32                          arfcn,
WrUmmMeasCellCb              *measCellCb,
WrUmmMeasEutraMeasObjNode    **cellCbMeasObj
)
{
   CmLList                       *measObjEutraNode;
   WrUmmMeasEutraMeasObjNode     *cellCbEMeasObj;

   TRC2(wrUmmMeasUpdtCellEutraMeasObjFromArfcn)

   if(arfcn == measCellCb->intraMeasObj.carrFreq)
   {
       *cellCbMeasObj = &measCellCb->intraMeasObj;
   }
   else
   {
      measObjEutraNode = measCellCb->interMeasObjLst.first;
      while (NULLP != measObjEutraNode)
      {
         cellCbEMeasObj = ((WrUmmMeasEutraMeasObjNode*)measObjEutraNode->node);
         if (arfcn == cellCbEMeasObj->carrFreq)
         {
            *cellCbMeasObj = cellCbEMeasObj;
            break;
         }
      measObjEutraNode = measObjEutraNode->next;
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to update measurement object and Id
 *  from ue cb. 
 *
 *  @details
 * Function:wrUmmMeasUpdtCdmaMeasObjAndIdFromArfcn
 *
 *   Processing steps:
 *    - tranverse cell Cb list find measurement object and id using Arfcn. 
 *     
 * @param[in]  arfcn: Arfcn         
 * @param[in]  measUeCb: pointer to measurement Ue control block         
 * @param[out]  measObjId: pointer to measurement object Id         
 * @param[out]  cellCbMeasObj: pointer to cell Cb measurement Object 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasUpdtCdmaMeasObjAndIdFromArfcn
(
U8                           bandClass,
U32                          arfcn,
WrMeasUeCb                   *measUeCb,
U8                           *measObjId,
WrUmmMeasCdmaMeasObjNode     **cellCbMeasObj
)
{
   CmLList                       *measObjCdmaNode;
   WrUmmMeasCdmaMeasObjNode      *cellCbEMeasObj;
   WrMeasUeCbCdmaMeasObjNode     *ueCbEMeasObj;

   TRC2(wrUmmMeasUpdtCdmaMeasObjAndIdFromArfcn)

   measObjCdmaNode = measUeCb->interRatMeasObjLst.first;
   while (NULLP != measObjCdmaNode)
   {
      ueCbEMeasObj = ((WrMeasUeCbCdmaMeasObjNode*)measObjCdmaNode->node);
      cellCbEMeasObj = ueCbEMeasObj->cdmaMeasObj;
      if ((bandClass == cellCbEMeasObj->bandClass) &&
         (arfcn == cellCbEMeasObj->arfcn))
      {
         *measObjId = ueCbEMeasObj->measObjId;
         *cellCbMeasObj = cellCbEMeasObj;
         RETVALUE(ROK);
      }
   measObjCdmaNode = measObjCdmaNode->next;
   }
   RETVALUE(RFAILED);
}
/** @brief This function is wraper function calling wrUmmMeasCfgReCfgReqCompHand 
 *         which frees all the allocated measIds, measObjIds and report config
 *         Ids allocated for measurements of measType for that transaction 
 *
 *  @details
 * Function:wrUmmMeasSetFreeMeasIds
 *
 *   Processing steps:
 *    - call wrUmmMeasCfgReCfgReqCompHand
 *     
 * @param[in]  measTyp: measurement Type  
 * @param[in]  transCb: pointer to Transaction control block 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasSetFreeMeasIds
(
WrUmmMeasTyp                 measTyp, 
WrUmmTransCb                 *transCb
)
{
   S16  ret;
   ret = wrUmmMeasCfgReCfgReqCompHand(measTyp,transCb);
   RETVALUE(ret);
}
/** @brief This function is responsible for to deleting operation in ue Cb 
 * for error scenarios. 
 *
 *  @details
 * Function:wrUmmMeasCfgReCfgReqCompHand
 *
 *   Processing steps:
 *    - Get the Id's from trans cb 
 *    - find the object in ue Cb delete that object
 *     
 * @param[in]  measTyp: measurement Type  
 * @param[in]  transCb: pointer to Transaction control block 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasCfgReCfgReqCompHand
(
WrUmmMeasTyp                 measTyp, 
WrUmmTransCb                 *transCb
)
{
   U8                        idVal;
   U8                        measId;
   U8                        measObjId;
   U8                        rptCfgId;
   WrUeCb                    *ueCb;
   WrMeasUeCb                *measUeCb;
   WrUmmMeasRmvInfo          *rmvTransLst = NULLP;
   U8                        measIdRmvLstCnt = 0;
   U8                        measObjIdRmvLstCnt = 0;
   U8                        rptCfgRmvLstCnt = 0;

   TRC2(wrUmmMeasCfgReCfgReqCompHand)

      ueCb = transCb->ueCb;
   if(transCb->transTyp == WR_UMM_MEAS_TRANS)
   {
      rmvTransLst = &transCb->u.measTransCb.rmvMeasInfo;
   }
   else if (transCb->transTyp == WR_UMM_ANR_MEAS_TRANS)
   {
      rmvTransLst = &transCb->u.anrMeasTrans.measTransCfg.rmvMeasInfo;
   }
/* LTE_ADV starts */
   else if(transCb->transTyp == WR_UMM_LTE_ADV_TRANS) 
   {
     rmvTransLst = &transCb->u.lteAdvTransCb.u.measInfo.rmvMeasInfo;
   }
/* LTE_ADV ends */
   else if(WR_UMM_CSFB_CCO_TRANS == transCb->transTyp)
   {
     rmvTransLst = &transCb->u.csfbGeranTrans.measCfgInfo.rmvMeasInfo;
   }
   else if(WR_UMM_CSFB_REDIR_TRANS == transCb->transTyp)
   {
     rmvTransLst = &transCb->u.csfbTrans.measTransCb.rmvMeasInfo;
   }
   else
   {
      rmvTransLst = &transCb->u.eventAnrTrans.measTransCfg.rmvMeasInfo;
   }
   measUeCb = ueCb->measCfg;

   measIdRmvLstCnt = rmvTransLst->measIdRmvLstCnt;
   for(idVal = 0;idVal < measIdRmvLstCnt; idVal++)
   {
      measId = rmvTransLst->measIdRmvLst[idVal];
      wrUmmMeasRmvMeaIdInUeCb(measId, &measUeCb->measIdLst);
      wrUmmMeasSetFreeMeasId(measUeCb, measId);
   }

   measObjIdRmvLstCnt = rmvTransLst->measObjIdRmvLstCnt;
   for(idVal = 0;idVal < measObjIdRmvLstCnt; idVal++)
   {
      measObjId = rmvTransLst->measObjIdRmvLst[idVal];
      /* When a measObjId is removed free all the measId
       * mapped to that measObj as well. */
      wrUmmHrlFreeMeasIdfrmUeCb(measObjId, measUeCb);
      wrUmmMeasRmvMeasObjInUeCb (measObjId, measUeCb);
      wrUmmMeasSetFreeMeasObjId(measUeCb, measObjId);
   }

   rptCfgRmvLstCnt = rmvTransLst->rptCfgRmvLstCnt;
   for(idVal = 0;idVal < rptCfgRmvLstCnt; idVal++)
   {
      rptCfgId = rmvTransLst->rptCfgRmvLst[idVal];
      wrUmmMeasRmvRptCfgInUeCb( rptCfgId, &measUeCb->rptCfgLst);
      wrUmmMeasSetFreeRptCfgId(measUeCb, rptCfgId);
   }
   /*ccpu00132002 Fix Reseting the transCb meas Info to delete all previous measId, 
   * measObjId and RptCfgId  configuration */
   if(measTyp ==  WR_UMM_MEAS_TYPE_PERD_ANR) 
   {
      wrUmmMeasResetTransCb(&transCb->u.anrMeasTrans.measTransCfg);
   }
   if(measTyp ==  WR_UMM_MEAS_TYPE_EVENT_ANR) 
   {
      wrUmmMeasResetTransCb(&transCb->u.eventAnrTrans.measTransCfg);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for updation of cdma cells in
 * cell control block
 * 
 *  @details
 *  Function:wrUmmMeasUpdtCdmaCellsinCellCb
 *
 *   Processing steps:
 *    - check the flag, if flag is true create new node for the Pci add to list
 *    - if flag is flase delete that pci from list.
 *    - update the add or delete pci celindex into cdmaNrUpdtId structure
 *     
 * @param[in]  cdmaCellIdx: Pointre to Bool type cdma cell index         
 * @param[in, out]  cdmaCellInfo: pointer to Cdma cell Info         
 * @param[in]  neighFreqInfo: poiner to WrCDMA2KFreqInfo structure. 
 * @param[in]  cdmaNrUpdtId: poiner to cdmaNrUpdtId structure. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasUpdtCdmaCellsinCellCb
(
 Bool                        *cdmaCellIdx,
 CmLListCp                   *cdmaCellInfo,
 WrCDMA2KFreqInfo            *neighFreqInfo,
 WrMeasCdmaNrUpdtIdNode      *cdmaNrUpdtId
)
{
   U8                        idCnt;
   U8                        numPci;
   U8                        pci;
   U8                        cellIndex = 0;
   WrAnrNrUpdtType           addDelFlg;
   CmLList                   *nrCellLnk;
   WrMeasNrCdmaCellNode      *cdma2kCellNode;
   
   TRC2(wrUmmMeasUpdtCdmaCellsinCellCb)

   numPci = neighFreqInfo->wlData.numWlCells;

   if ( numPci >= WR_UMM_MEAS_MAX_CELL_IDX)
   {
      RLOG0(L_INFO,"only first 32 CDMA neighbour configured for measurement");
      numPci = WR_UMM_MEAS_MAX_CELL_IDX;
   }

   for(idCnt = 0; idCnt < numPci; idCnt++)
   {
      pci = neighFreqInfo->wlData.pciInfoLst[idCnt].pnOffset;
      addDelFlg = neighFreqInfo->wlData.pciInfoLst[idCnt].nrUpdtType;
      if(addDelFlg == WR_EMM_ANR_ADD_NR)/*Add node into List*/
      {
       if(RFAILED == wrUmmMeasGetFreeCellIdxForCdma(cdmaCellIdx, &cellIndex))
       {
            RLOG0(L_ERROR, "Cdma CellIndex not available");
          RETVALUE(RFAILED);          
       }
       WR_ALLOC(&cdma2kCellNode, sizeof(WrMeasNrCdmaCellNode));
       if(NULLP == cdma2kCellNode)
       {
          RLOG0(L_FATAL, "Memory allocation failed.");
          cdmaCellIdx[cellIndex - 1] = FALSE;
          RETVALUE(RFAILED);
       }
       cdma2kCellNode->cellIndex = cellIndex;
       cdma2kCellNode->pci = pci;
       cdma2kCellNode->lnk.node = (PTR)cdma2kCellNode;
       cmLListAdd2Tail(cdmaCellInfo, &cdma2kCellNode->lnk);      

       cdmaNrUpdtId->addCellLst[cdmaNrUpdtId->addCellCnt] = cellIndex;
       cdmaNrUpdtId->addCellCnt++;
      }
      else
      {
         nrCellLnk = cdmaCellInfo->first;
         while (NULLP != nrCellLnk)
         {
            cdma2kCellNode = (WrMeasNrCdmaCellNode*)nrCellLnk->node;
            if (pci == cdma2kCellNode->pci)
            {
               cellIndex = cdma2kCellNode->cellIndex;
               cmLListDelFrm (cdmaCellInfo, nrCellLnk);
               WR_FREE(cdma2kCellNode, sizeof(WrMeasNrCdmaCellNode));
               
               cdmaCellIdx[cellIndex - 1] = FALSE;
       
               cdmaNrUpdtId->delCellLst[cdmaNrUpdtId->delCellCnt] = cellIndex;
               cdmaNrUpdtId->delCellCnt++;
               break;
            }
         nrCellLnk = nrCellLnk->next;
         }
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for updation of Eutra white cellList in
 * cell control block
 * 
 *  @details
 *  Function:wrUmmMeasUpdtEutraWLCellinCellCb
 *
 *   Processing steps:
 *    - check the flag, if flag is true create new node for the Pci add to list
 *    - if flag is flase delete that pci from list.
 *    - update the add or delete pci celindex into nrUpdtId structure
 *     
 * @param[in]  wlCellIdxInUse: pointer to Bool type Eutra white cellList index         
 * @param[in, out]  wlCellLst: pointer to Eutra White Cell List         
 * @param[in]  wlCellInfo: poiner to WrNrPerFreqWlInfo structure. 
 * @param[in]  nrUpdtId: poiner to WrMeasEutraNrUpdtIdNode structure. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasUpdtEutraWLCellinCellCb
(
 Bool                        *wlCellIdxInUse,
 CmLListCp                   *wlCellLst,
 WrNrPerFreqWlInfo           *wlCellInfo,
 WrMeasEutraNrUpdtIdNode     *nrUpdtId
)
{
   U8                        wlCellCnt;
   U8                        idCnt;
   U16                       pci;
   U32                       cio;
   WrAnrNrUpdtType           addDelFlg;
   U8                        cellIndex; 
   CmLList                   *nrWCellLnk;
   WrMeasNrWCellNode         *nrWCellNode;
   
   TRC2(wrUmmMeasUpdtEutraWLCellinCellCb)
    
   wlCellCnt = wlCellInfo->numWlCells;

   for(idCnt = 0; idCnt < wlCellCnt; idCnt++)
   {
      pci = wlCellInfo->cellInfo[idCnt].phyCellId;
      addDelFlg = wlCellInfo->cellInfo[idCnt].nrUpdtType;
      if(addDelFlg == WR_EMM_ANR_ADD_NR)/*Add node into List*/
      {
         cio = wlCellInfo->cellInfo[idCnt].cio;
         if(RFAILED == wrUmmMeasGetFreeCellIdxForEutra( wlCellIdxInUse, &cellIndex))
         {
            RLOG0(L_ERROR, "Free Cell Index not available");
            RETVALUE(RFAILED);
         }
         WR_ALLOC(&nrWCellNode, sizeof(WrMeasNrWCellNode));
         if(NULLP == nrWCellNode)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            wlCellIdxInUse[cellIndex - 1] = FALSE;
            RETVALUE(RFAILED);
         }
         nrWCellNode->cellIndex = cellIndex;
         nrWCellNode->pci = pci;
         nrWCellNode->cio = cio;
         nrWCellNode->lnk.node = (PTR)nrWCellNode;
         cmLListAdd2Tail(wlCellLst, &nrWCellNode->lnk);
   
         nrUpdtId->addWCellLst[nrUpdtId->addWCellCnt] = cellIndex;
         nrUpdtId->addWCellCnt++;
      }
      else
      {
         nrWCellLnk = wlCellLst->first;
         while (NULLP != nrWCellLnk)
         {
            nrWCellNode = (WrMeasNrWCellNode*)nrWCellLnk->node;
            if (pci == nrWCellNode->pci)
            {
               cellIndex = nrWCellNode->cellIndex;
               cmLListDelFrm (wlCellLst, nrWCellLnk);
               WR_FREE(nrWCellNode, sizeof(WrMeasNrWCellNode));
         
               wlCellIdxInUse[cellIndex - 1] = FALSE; 
         
               nrUpdtId->delWCellLst[nrUpdtId->delWCellCnt] = cellIndex;
               nrUpdtId->delWCellCnt++;
               break;
            }
            nrWCellLnk = nrWCellLnk->next;
         }
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for updation of Utra white cellList in
 * cell control block
 * 
 *  @details
 *  Function:wrUmmMeasUpdtUtraWLCellinCellCb
 *
 *   Processing steps:
 *    - check the flag, if flag is true create new node for the Psc add to list
 *    - if flag is false delete that psc from list.
 *    - update the add or delete psc celindex into nrUpdtId structure
 *     
 * @param[in]  wlCellIdxInUse: pointer to Bool type Utra white cellList index         
 * @param[in, out]  wlCellLst: pointer to Utra White Cell List         
 * @param[in]  wlCellInfo: poiner to WrNrPerFreqWlInfo structure. 
 * @param[in]  nrUpdtId: poiner to WrMeasUtraNrUpdtIdNode structure. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasUpdtUtraWLCellinCellCb 
(
 Bool                        *cellIdxInUse,
 CmLListCp                   *wlCellLst,
 WrNrUtraPerFreqWlInfo       *wlCellInfo,
 WrMeasUtraNrUpdtIdNode      *nrUpdtId
)
{
   U8                         wlCellCnt;
   U8               idCnt;
   U16               psc;
   WrAnrNrUpdtType         addDelFlg;
   U8               cellIndex; 
   CmLList            *nrWCellLnk;
   WrMeasNrUtraWCellNode      *nrUtraWCellNode;

   TRC2(wrUmmMeasUpdtUtraWLCellinCellCb);
   
   wlCellCnt = wlCellInfo->numWlCells;

   for(idCnt = 0; idCnt < wlCellCnt; idCnt++)
   {
      psc = wlCellInfo->cellInfo[idCnt].psc;
      addDelFlg = wlCellInfo->cellInfo[idCnt].nrUpdtType;
      if(addDelFlg == WR_EMM_ANR_ADD_NR)/*Add node into List*/
      {
         if(RFAILED == wrUmmMeasGetFreeCellIdxForEutra(cellIdxInUse, &cellIndex))
         {
            RLOG0(L_ERROR, "Free Cell Index not available");
            RETVALUE(RFAILED);
         }
         WR_ALLOC(&nrUtraWCellNode, sizeof(WrMeasNrUtraWCellNode));
         if(NULLP == nrUtraWCellNode)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            cellIdxInUse[cellIndex - 1] = FALSE;
            RETVALUE(RFAILED);
         }

         nrUtraWCellNode->cellIndex = cellIndex;
         nrUtraWCellNode->psc = psc;
         nrUtraWCellNode->lnk.node = (PTR)nrUtraWCellNode;
         cmLListAdd2Tail(wlCellLst, &nrUtraWCellNode->lnk);

         nrUpdtId->addCellLst[nrUpdtId->addCellCnt] = cellIndex;
         nrUpdtId->addCellCnt++;

      }
      else
      {
         nrWCellLnk = wlCellLst->first;
         while (NULLP != nrWCellLnk)
         {
            nrUtraWCellNode = (WrMeasNrUtraWCellNode*)nrWCellLnk->node;
            if (psc == nrUtraWCellNode->psc)
            {
               cellIndex = nrUtraWCellNode->cellIndex;
               cmLListDelFrm (wlCellLst, nrWCellLnk);
               WR_FREE(nrUtraWCellNode, sizeof(WrMeasNrUtraWCellNode));

               cellIdxInUse[cellIndex - 1] = FALSE; 

               nrUpdtId->delCellLst[nrUpdtId->delCellCnt] = cellIndex;
               nrUpdtId->delCellCnt++;
               break;
            }
            nrWCellLnk = nrWCellLnk->next;
         }
      }
     
   }
   
   RETVALUE(ROK);
}
   
/** @brief This function is responsible for updation of Eutra black cellList in
 * cell control block
 * 
 *  @details
 *  Function:wrUmmMeasUpdtEutraBLCellinCellCb
 *
 *   Processing steps:
 *    - check the flag, if flag is true create new node for the Pci add to list
 *    - if flag is flase delete that pci from list.
 *    - update the add or delete pci celindex into nrUpdtId structure
 *     
 * @param[in]  blCellIdxInUse: pointer to Bool type Eutra black cellList index         
 * @param[in, out]  blCellLst: pointer to Eutra black Cell List         
 * @param[in]  blCellInfo: poiner to WrNrPerFreqBlInfo structure. 
 * @param[in]  nrUpdtId: poiner to WrMeasEutraNrUpdtIdNode structure. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasUpdtEutraBLCellinCellCb
(
 Bool                        *blCellIdxInUse,
 CmLListCp                   *blCellLst,
 WrNrPerFreqBlInfo           *blCellInfo,
 WrMeasEutraNrUpdtIdNode     *nrUpdtId
)
{
   U8                        blCellCnt;
   U8                        idCnt;
   U16                       pci;
   WrAnrNrUpdtType           addDelFlg;
   CmLList                   *nrBCellLnk;
   WrMeasNrBCellNode         *nrBCellNode;
   U8                        phyCellIdRng;
   U8                        cellIdx;
   
   TRC2(wrUmmMeasUpdtEutraBLCellinCellCb)
    
   blCellCnt = blCellInfo->numBlInfos;

   for(idCnt = 0; idCnt < blCellCnt; idCnt++)
   {
      pci = blCellInfo->blInfo[idCnt].startPhyCellID;
      addDelFlg = blCellInfo->blInfo[idCnt].nrUpdtType;
      if(addDelFlg == WR_EMM_ANR_ADD_NR)/*Add node into List*/
      {
         phyCellIdRng = blCellInfo->blInfo[idCnt].phyCellIdRange;
         
         if(RFAILED == wrUmmMeasGetFreeCellIdxForEutra(blCellIdxInUse, &cellIdx))
         {
            RLOG0(L_ERROR, "cellIndex Not availble");
            RETVALUE(RFAILED);
         }
         WR_ALLOC(&nrBCellNode, sizeof(WrMeasNrBCellNode));
         if(NULLP == nrBCellNode)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            blCellIdxInUse[cellIdx - 1] = FALSE;
            RETVALUE(RFAILED);
         }
         nrBCellNode->cellIndex = cellIdx;
         nrBCellNode->startPhyCellID = pci;
         nrBCellNode->phyCellIdRange = phyCellIdRng;
         nrBCellNode->lnk.node = (PTR)nrBCellNode;
         cmLListAdd2Tail(blCellLst, &nrBCellNode->lnk);      
         
         nrUpdtId->addBCellLst[nrUpdtId->addBCellCnt] = cellIdx;
         nrUpdtId->addBCellCnt++;

      }
      else
      {
         nrBCellLnk = blCellLst->first;
         while (NULLP != nrBCellLnk)
         {
            nrBCellNode = (WrMeasNrBCellNode*)nrBCellLnk->node;
            if (pci == nrBCellNode->startPhyCellID)
            {
               cellIdx = nrBCellNode->cellIndex;
               cmLListDelFrm (blCellLst, nrBCellLnk);
               WR_FREE(nrBCellNode, sizeof(WrMeasNrBCellNode));
               blCellIdxInUse[cellIdx - 1]  = FALSE;
               nrUpdtId->delBCellLst[nrUpdtId->delBCellCnt] = cellIdx;
               nrUpdtId->delBCellCnt++;
               break;
            }
         nrBCellLnk = nrBCellLnk->next;
         }
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to update Cdma measurement object
 * from Arfcn.
 * 
 *  @details
 * Function:wrUmmMeasGetCdmaMeasObjFromArfcn
 *
 *   Processing steps:
 *    - transverse the list and find measurement object using Afrcn
 *     
 * @param[in]  bandClass: Band Class         
 * @param[in]  arfcn: Arfcn         
 * @param[out]  measObjId: pointer to measurememnt Object Id. 
 * @param[in]  interRatMeasObjLst: poiner to inter RAT measuremnet Object List. 
 * @param[out]  cellCbMeasObj: pointer to the CellCb measurement Object. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasGetCdmaMeasObjFromArfcn
(
U8                           bandClass,
U32                          arfcn,
U8                           *measObjId,
WrMeasUeCb                   *measUeCb,
WrUmmMeasCdmaMeasObjNode     **cellCbMeasObj
)
{
   CmLList                   *measObjCdmaNode;
   WrMeasUeCbCdmaMeasObjNode *ueCbMeasObj;
   Bool                       isMeasObjPres = FALSE;
   TRC2(wrUmmMeasGetCdmaMeasObjFromArfcn)

   if(measUeCb->interRatMeasObjLst.first == NULLP)
   {
      RETVALUE(RFAILED);
   }
   measObjCdmaNode = measUeCb->interRatMeasObjLst.first;
   while (NULLP != measObjCdmaNode)
   {
      ueCbMeasObj = (WrMeasUeCbCdmaMeasObjNode*)measObjCdmaNode->node;
      if ((bandClass == ueCbMeasObj->cdmaMeasObj->bandClass) &&
          (arfcn == ueCbMeasObj->cdmaMeasObj->arfcn))
      {
         *measObjId = ueCbMeasObj->measObjId;
         *cellCbMeasObj = ueCbMeasObj->cdmaMeasObj;
         isMeasObjPres = TRUE;
         break;
      }
   measObjCdmaNode = measObjCdmaNode->next;
   }
   if(isMeasObjPres == FALSE)
   {
      RLOG0(L_ERROR, "Cdma Meas Object not found");
      RETVALUE(RFAILED);
   }
   else
   {
      RETVALUE(ROK);
   }
}
/** @brief This function is responsible for to fill cdma cell id list 
 * 
 *  @details
 * Function:wrUmmMeasFillCdmaCellIdObj
 *
 *   Processing steps:
 *    - Fill Nhu cdma cell Id List
 *     
 * @param[in]  cellIdx: cell Index         
 * @param[out] nhuCellsAddMod: pointer to the Nhu cell List addMod Object. 
 * @param[in]  cellLst: pointer to the cell List. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasFillCdmaCellIdObj
(
 U8                           cellIdx, 
 NhuCellsToAddModCDMA2000    *nhuCellsAddMod,
 CmLListCp                   *cellLst
)
{
   WrMeasNrCdmaCellNode      *nrCellNode;
   CmLList                   *nrCellLnk;

   TRC2(wrUmmMeasFillCdmaCellIdObj)
   
   nrCellLnk = cellLst->first;
   while(NULLP != nrCellLnk)
   {
      nrCellNode = (WrMeasNrCdmaCellNode*)nrCellLnk->node;
      if(nrCellNode->cellIndex == cellIdx)
      {
         wrFillTknU8(&nhuCellsAddMod->pres, 1);
         WR_FILL_TKN_UINT(nhuCellsAddMod->cellIdx, nrCellNode->cellIndex)
         WR_FILL_TKN_UINT(nhuCellsAddMod->physCellId, nrCellNode->pci)
         break;
      }
      nrCellLnk = nrCellLnk->next;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to fill cdma NHU measurement Object
 * 
 *  @details
 * Function:wrUmmMeasFillCdmaNhuMsgForNrUpdt
 *
 *   Processing steps:
 *    - Allaocate memory using memCp pointer
 *    - Fill Nhu measurement Object
 *     
 * @param[in]  memCp: pointer to memory control pointer         
 * @param[in]  cellCbMeasObj: pointer to the cellCb measurement Object. 
 * @param[in]  nhuMeasObj: poiner to nhu measurement Object. 
 * @param[in]  nrUpdtId: pointer to the neighbour update id structure. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasFillCdmaNhuMsgForNrUpdt
(
 CmMemListCp                        *memCp,
 WrUmmMeasCdmaMeasObjNode           *cellCbMeasObj,
 NhuMeasObjectToAddModmeasObject    *nhuMeasObj,
 WrMeasCdmaNrUpdtIdNode             *nrUpdtId
)
{
   U8                         idCnt;
   U8                         cellIdx;
   U8                         addCellCnt;
   U8                         delCellCnt;
   NhuMeasObjectCDMA2000      *nhuMeasCdma2k;   

   TRC2(wrUmmMeasFillCdmaNhuMsgForNrUpdt)
   
   addCellCnt = nrUpdtId->addCellCnt;
   delCellCnt = nrUpdtId->delCellCnt;
      
   nhuMeasCdma2k = &nhuMeasObj->val.measObjectCDMA2000;
   
   WR_FILL_TKN_UINT(nhuMeasObj->choice, MEASOBJECT_MEASOBJECTCDMA2000)
   wrFillTknU8(&nhuMeasCdma2k->pres, 1);

   WR_FILL_TKN_UINT(nhuMeasCdma2k->cdma2000_Typ, cellCbMeasObj->cdmaType)
   wrFillTknU8(&nhuMeasCdma2k->carrierFreq.pres, 1);
   WR_FILL_TKN_UINT(nhuMeasCdma2k->carrierFreq.bandClass, cellCbMeasObj->bandClass)
   WR_FILL_TKN_UINT(nhuMeasCdma2k->carrierFreq.arfcn, cellCbMeasObj->arfcn)
   WR_FILL_TKN_UINT(nhuMeasCdma2k->searchWindowSize, cellCbMeasObj->searchWindowSize)
   WR_FILL_TKN_UINT(nhuMeasCdma2k->offsetFreq, cellCbMeasObj->qOffsetRangeInterRAT)

   nhuMeasCdma2k->cellForWhichToReportCGI.pres = NOTPRSNT;
   /* cell list information*/
  
   if(0 != addCellCnt)
   {
      NhuCellsToAddModCDMA2000   *nhuCellsAddMod;
      WR_FILL_TKN_UINT(nhuMeasCdma2k->cellsToAddModLst.noComp, addCellCnt)
      if(ROK != cmGetMem(memCp, addCellCnt * sizeof(NhuCellsToAddModCDMA2000), 
                                             (Ptr *)&nhuCellsAddMod))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuMeasCdma2k->cellsToAddModLst.member = nhuCellsAddMod;
      for(idCnt = 0; idCnt < addCellCnt; idCnt++)
      {
         cellIdx = nrUpdtId->addCellLst[idCnt];
         wrUmmMeasFillCdmaCellIdObj(cellIdx, &nhuCellsAddMod[idCnt], &cellCbMeasObj->cdmaCellInfo);
      }
   }
   if(0 != delCellCnt)
   {
      NhuCellIdx                 *nhuRmvCellLst;

      WR_FILL_TKN_UINT(nhuMeasCdma2k->cellsToRemoveLst.noComp, delCellCnt)
      if(ROK != cmGetMem(memCp, delCellCnt * sizeof(NhuCellIdx), (Ptr *)&nhuRmvCellLst))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuMeasCdma2k->cellsToRemoveLst.member = nhuRmvCellLst;
      for(idCnt = 0; idCnt < delCellCnt; idCnt++)
      {
         WR_FILL_TKN_UINT(nhuRmvCellLst[idCnt], nrUpdtId->delCellLst[idCnt])
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to send Cdma Hrpd neighbour update
 * to all ue's.
 * 
 *  @details
 * Function:wrUmmMeasSendCdmaHrpdUpdtNrInfoToUe
 *
 *   Processing steps:
 *    - get the UeCb from Umm cell Cb
 *    - if Ue supports Cdma Hrpd send the neighbour update information to ue
 *     
 * @param[in]  memCp: pointer to the memory pointer
 * @param[in]  index: index for NHU structure
 * @param[in]  measCellCb: pointer to the cell control block
 * @param[in]  measUeCb: pointer to the measurement Ue control block
 * @param[in]  nrUpdtIdCnt: Neighbour update id count.
 * @param[in]  nrUpdtId: poiner to WrMeasCdmaNrUpdtIdNode structure.
 * @param[in]  rrcReCfgMsg: pointer to reconfig Message.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasSendCdmaHrpdUpdtNrInfoToUe
(
CmMemListCp                  *memCp,
U8                           *idxVal,
WrUmmMeasCellCb              *measCellCb,
WrMeasUeCb                   *measUeCb,
U8                           nrUpdtIdCnt,
WrMeasCdmaNrUpdtIdNode       *nrUpdtId,
NhuMeasConfig                *nhuMeasCfg
)
{
   U8                                   idCnt;
   NhuMeasObjectToAddMod               *nhuMeasObjAddMod;

   TRC2(wrUmmMeasSendCdmaHrpdUpdtNrInfoToUe)

   nhuMeasObjAddMod = nhuMeasCfg->measObjectToAddModLst.member;

   for(idCnt = 0; idCnt < nrUpdtIdCnt; idCnt++)
   {
      U8                            bandClass;
      U16                           arfcn;
      U8                            measObjId = 0;
      WrUmmMeasCdmaMeasObjNode      *cellCbMeasObj;

      bandClass = nrUpdtId[idCnt].bandClass;
      arfcn = nrUpdtId[idCnt].arfcn;
      /* Construct NHU Measurement Object List*/

      if(ROK == wrUmmMeasGetCdmaMeasObjFromArfcn(bandClass, arfcn,
                 &measObjId,measUeCb, &cellCbMeasObj))
   
      {
         NhuMeasObjectToAddModmeasObject     *nhuMeasObj;

         nhuMeasObj = &nhuMeasObjAddMod[*idxVal].measObject;
         wrFillTknU8(&nhuMeasObjAddMod[*idxVal].pres, 1);
         WR_FILL_TKN_UINT(nhuMeasObjAddMod[*idxVal].measObjectId, measObjId)
         (*idxVal)++;
         wrUmmMeasFillCdmaNhuMsgForNrUpdt(memCp, cellCbMeasObj, nhuMeasObj, &nrUpdtId[idCnt]);
      }
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for to fill Utra cell list 
 * 
 *  @details
 * Function:wrUmmMeasFillUtraCellIdObj
 * 
 *   Processing steps:
 *    - Fill Nhu Utra cell Id List
 *     
 * @param[in]  cellIdx: cell Index         
 * @param[out] nhuCellsAddMod: pointer to the Nhu cell List addMod Object. 
 * @param[in]  blCellLst: pointer to the bhite cell List. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasFillUtraCellIdObj
(
 U8                          cellIdx, 
 NhuCellsToAddModUTRA_FDD    *nhuCellsAddMod,
 CmLListCp                   *cellLst
)
{
   CmLList                   *nrCellLnk;
   WrMeasNrUtraCellNode     *nrCellNode;

   TRC2(wrUmmMeasFillUtraCellIdObj)
   
   nrCellLnk = cellLst->first;
   while(NULLP != nrCellLnk)
   {
      nrCellNode = (WrMeasNrUtraCellNode*)nrCellLnk->node;
      if(nrCellNode->cellIndex == cellIdx)
      {
         wrFillTknU8(&nhuCellsAddMod->pres, 1);
         WR_FILL_TKN_UINT(nhuCellsAddMod->cellIdx, nrCellNode->cellIndex)
         WR_FILL_TKN_UINT(nhuCellsAddMod->physCellId, nrCellNode->psc)
         break;
      }
      nrCellLnk = nrCellLnk->next;
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for to fill Utra NHU measurement Object
 * 
 *  @details
 * Function:wrUmmMeasFillUtraRrcAddModLstForNrUpdt
 *
 *   Processing steps:
 *    - Allaocate memory using memCp pointer
 *    - Fill Nhu measurement Object
 *     
 * @param[in]  memCp: pointer to memory control pointer         
 * @param[in]  cellCbMeasObj: pointer to the cellCb measurement Object. 
 * @param[in]  nhuMeasObj: poiner to nhu measurement Object. 
 * @param[in]  nrUpdtId: pointer to the neighbour update id structure. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/

PRIVATE S16 wrUmmMeasFillUtraRrcAddModLstForNrUpdt
(
 CmMemListCp                        *memCp,
 WrUmmMeasUtraMeasObjNode           *cellCbMeasObj,
 NhuMeasObjectToAddModmeasObject    *nhuMeasObj,
 WrMeasUtraNrUpdtIdNode             *nrUpdtId
)
{
   U8                       idCnt;
   U8                       cellIdx;
   U8                       addCellCnt;
   U8                       delCellCnt;
   NhuMeasObjectUTRA       *nhuUtraMeasObj;
      
   TRC2(wrUmmMeasFillUtraRrcAddModLstForNrUpdt)
   
   addCellCnt = nrUpdtId->addCellCnt;
   delCellCnt = nrUpdtId->delCellCnt;
      
   nhuUtraMeasObj = &nhuMeasObj->val.measObjectUTRA;
   
   WR_FILL_TKN_UINT(nhuMeasObj->choice, MEASOBJECT_MEASOBJECTUTRA)
   wrFillTknU8(&nhuUtraMeasObj->pres, 1);
   WR_FILL_TKN_UINT(nhuUtraMeasObj->carrierFreq, cellCbMeasObj->arfcn)
   WR_FILL_TKN_UINT(nhuUtraMeasObj->offsetFreq, cellCbMeasObj->qOffsetFreq)

   if(cellCbMeasObj->duplexMode == WR_UMM_MEAS_DUPLEX_MODE_FDD)
   {   
      nhuUtraMeasObj->cellForWhichToReportCGI.choice.pres = NOTPRSNT;
   }
   /* White and Black list information*/
  
   if(0 != delCellCnt)
   {
      NhuCellIdx               *nhuRmvCell;

      WR_FILL_TKN_UINT(nhuUtraMeasObj->cellsToRemoveLst.noComp, delCellCnt)
      if(ROK != cmGetMem(memCp, delCellCnt * sizeof(NhuCellIdx), (Ptr *)&nhuRmvCell))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuUtraMeasObj->cellsToRemoveLst.member = nhuRmvCell;
     
      for(idCnt = 0; idCnt < delCellCnt; idCnt++)
      {
         WR_FILL_TKN_UINT(nhuRmvCell[idCnt], nrUpdtId->delCellLst[idCnt])    
      }
   }
   if(0 != addCellCnt)
   {
      NhuCellsToAddModUTRA_FDD *nhuCellsAddMod;
      if(cellCbMeasObj->duplexMode == WR_UMM_MEAS_DUPLEX_MODE_FDD)
      {   
         WR_FILL_TKN_UINT(nhuUtraMeasObj->cellsToAddModLst.choice, CELLSTOADDMODLST_CELLSTOADDMODLSTUTRA_FDD)
         WR_FILL_TKN_UINT(nhuUtraMeasObj->cellsToAddModLst.val.cellsToAddModLstUTRA_FDD.noComp, addCellCnt)
         if(ROK != cmGetMem(memCp, addCellCnt * sizeof(NhuCellsToAddModUTRA_FDD), (Ptr *)&nhuCellsAddMod))
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         nhuUtraMeasObj->cellsToAddModLst.val.cellsToAddModLstUTRA_FDD.member = nhuCellsAddMod;

         for(idCnt = 0; idCnt < addCellCnt; idCnt++)
         {
           cellIdx = nrUpdtId->addCellLst[idCnt];
           wrUmmMeasFillUtraCellIdObj(cellIdx, &nhuCellsAddMod[idCnt], &cellCbMeasObj->cellLst);
         }
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for sending Utra measurement 
 *    objects that have to be deleted based on the new HRL received 
 *    for the UE.
 * 
 *  @details
 * Function: wrUmmMeasSendUtraDelNrInfoToUe
 *
 *   Processing steps:
 *    - Get the measObjId from the received UTRA arfcn.
 *    - Add the measObjId in the nhuMeasObj to remove.
 *     
**/
PRIVATE S16 wrUmmMeasSendUtraDelNrInfoToUe
(
 U8                           *idxVal,
 WrMeasUeCb                   *measUeCb,
 U8                           nrDelIdCnt,
 U32                          *nrDelId,
 NhuMeasConfig                *nhuMeasCfg
 )
{
   U8                                   idCnt;
   U8                                   objCnt = 0;
   NhuMeasObjectId                      *nhuMeasObjRmv;

   nhuMeasObjRmv = nhuMeasCfg->measObjectToRemoveLst.member;

   for(idCnt = 0; idCnt < nrDelIdCnt; idCnt++)
   {
      U16                           arfcn;
      U8                            measObjId = 0;
      WrUmmMeasUtraMeasObjNode      *cellCbMeasObj = NULLP;

      arfcn = nrDelId[idCnt];
      /* Construct NHU Measurement Object List*/
      if(ROK == wrUmmMeasUpdtUtraMeasObjAndIdFromArfcn(arfcn,
               measUeCb, &measObjId, &cellCbMeasObj))
      {
         objCnt++;
         WR_FILL_TKN_UINT(nhuMeasObjRmv[*idxVal], measObjId)
            wrUmmHrlFreeMeasIdfrmUeCb(measObjId, measUeCb);
         wrUmmMeasRmvMeasObjInUeCb (measObjId, measUeCb);
         wrUmmMeasSetFreeMeasObjId(measUeCb, measObjId);
         (*idxVal)++;
      }
   }
   if(objCnt == 0)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      RETVALUE(ROK);
   }
}

/** @brief This function is responsible for sending CDMA measurement 
 *    objects that have to be deleted based on the new HRL received 
 *    for the UE.
 * 
 *  @details
 * Function: wrUmmMeasSendCdmaDelNrInfoToUe
 *
 *   Processing steps:
 *    - Get the measObjId from the received CDMA bandclass and arfcn.
 *    - Add the measObjId in the nhuMeasObj to remove.
 *     
**/
PRIVATE S16 wrUmmMeasSendCdmaDelNrInfoToUe
(
 U8                           *idxVal,
 WrMeasUeCb                   *measUeCb,
 U8                           nrDelIdCnt,
 WrMeasCdmaNrDelIdNode        *nrDelId,
 NhuMeasConfig                *nhuMeasCfg
 )
{
   U8                                   idCnt;
   U8                                   objCnt = 0;
   NhuMeasObjectId                      *nhuMeasObjRmv;

   nhuMeasObjRmv = nhuMeasCfg->measObjectToRemoveLst.member;

   for(idCnt = 0; idCnt < nrDelIdCnt; idCnt++)
   {
      U8                            bandClass;
      U32                           arfcn;
      U8                            measObjId = 0;
      WrUmmMeasCdmaMeasObjNode      *cdmaCellObj = NULLP;

      bandClass = nrDelId[idCnt].bandClass;
      arfcn = nrDelId[idCnt].arfcn;
      /* Construct NHU Measurement Object List*/
      if(ROK != wrUmmMeasUpdtCdmaMeasObjAndIdFromArfcn(bandClass,
               arfcn, measUeCb, &measObjId, &cdmaCellObj))
      {
         objCnt++;
         WR_FILL_TKN_UINT(nhuMeasObjRmv[*idxVal], measObjId)
            (*idxVal)++;
      }
   }
   if(objCnt == 0)
   {
      RETVALUE(RFAILED);
   }
   else
   {
      RETVALUE(ROK);
   }
}

/** @brief This function is responsible for to send Utra neighbour update
 * to all ue's.
 * 
 *  @details
 * Function:wrUmmMeasSendUtraUpdtNrInfoToUe
 *
 *   Processing steps:
 *    - get the UeCb from Umm cell Cb
 *    - if Ue supports Utra send the neighbour update information to ue
 *     
 * @param[in]  memCp: pointer to the memory pointer
 * @param[in]  index: index for NHU structure
 * @param[in]  measCellCb: pointer to the cell control block
 * @param[in]  measUeCb: pointer to the measurement Ue control block
 * @param[in]  nrUpdtIdCnt: Neighbour update id count.
 * @param[in]  nrUpdtId: poiner to WrMeasUtraNrUpdtIdNode structure.
 * @param[in]  rrcReCfgMsg: pointer to reconfig Message.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasSendUtraUpdtNrInfoToUe
(
CmMemListCp                  *memCp,
U8                           *idxVal,
WrUmmMeasCellCb              *measCellCb,
WrMeasUeCb                   *measUeCb,
U8                           nrUpdtIdCnt,
WrMeasUtraNrUpdtIdNode       *nrUpdtId,
NhuMeasConfig                *nhuMeasCfg
)
{
   U8                                   idCnt;
   U8                                   objCnt = 0;
   NhuMeasObjectToAddMod               *nhuMeasObjAddMod;

   TRC2(wrUmmMeasSendUtraUpdtNrInfoToUe)

   nhuMeasObjAddMod = nhuMeasCfg->measObjectToAddModLst.member;

   for(idCnt = 0; idCnt < nrUpdtIdCnt; idCnt++)
   {
      U16                           arfcn;
      U8                            measObjId = 0;
      WrUmmMeasUtraMeasObjNode      *cellCbMeasObj = NULLP;

      arfcn = nrUpdtId[idCnt].arfcn;
      /* Construct NHU Measurement Object List*/
      if(ROK == wrUmmMeasUpdtUtraMeasObjAndIdFromArfcn(arfcn,
                        measUeCb, &measObjId, &cellCbMeasObj))
      {
         NhuMeasObjectToAddModmeasObject     *nhuMeasObj;
        
         /* For an internal message use the cellCbMeaObj coming as part 
          * of internal message. */        
         cellCbMeasObj = nrUpdtId[idCnt].cellCbMeasObj;
         objCnt++;
         nhuMeasObj = &nhuMeasObjAddMod[*idxVal].measObject;
         wrFillTknU8(&nhuMeasObjAddMod[*idxVal].pres, 1);
         WR_FILL_TKN_UINT(nhuMeasObjAddMod[*idxVal].measObjectId, measObjId)
         (*idxVal)++;
         wrUmmMeasFillUtraRrcAddModLstForNrUpdt(memCp, cellCbMeasObj, nhuMeasObj, &nrUpdtId[idCnt]);
      }
   }
   if(objCnt == 0)
   {
      RETVALUE(RFAILED);
   }
   else
   {
       RETVALUE(ROK);
   }
}

/** @brief This function is responsible for to send 1xCdma neighbour update
 * to all ue's.
 * 
 *  @details
 * Function:wrUmmMeasSend1xCdmaUpdtNrInfoToUe
 *
 *   Processing steps:
 *    - get the UeCb from Umm cell Cb
 *    - if Ue supports 1xCdma send the neighbour update information to ue
 *     
 * @param[in]  memCp: pointer to the memory pointer
 * @param[in]  index: index for NHU structure
 * @param[in]  measCellCb: pointer to the cell control block
 * @param[in]  measUeCb: pointer to the measurement Ue control block
 * @param[in]  nrUpdtIdCnt: Neighbour update id count.
 * @param[in]  nrUpdtId: poiner to WrMeasCdmaNrUpdtIdNode structure.
 * @param[in]  nhuMeasCfg: pointer to measurement Nhu Message.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasSend1xCdmaUpdtNrInfoToUe
(
CmMemListCp                  *memCp,
U8                           *idxVal,
WrUmmMeasCellCb              *measCellCb,
WrMeasUeCb                   *measUeCb,
U8                           nrUpdtIdCnt,
WrMeasCdmaNrUpdtIdNode       *nrUpdtId,
NhuMeasConfig                *nhuMeasCfg
)
{
   U8                                   idCnt;
   NhuMeasObjectToAddMod               *nhuMeasObjAddMod;

   TRC2(wrUmmMeasSend1xCdmaUpdtNrInfoToUe)

   nhuMeasObjAddMod = nhuMeasCfg->measObjectToAddModLst.member;

   for(idCnt = 0; idCnt < nrUpdtIdCnt; idCnt++)
   {
      U8                            bandClass;
      U16                           arfcn;
      U8                            measObjId = 0;
      WrUmmMeasCdmaMeasObjNode      *cellCbMeasObj;

      bandClass = nrUpdtId[idCnt].bandClass;
      arfcn = nrUpdtId[idCnt].arfcn;
      /* Construct NHU Measurement Object List*/

      if(ROK == wrUmmMeasGetCdmaMeasObjFromArfcn(bandClass, arfcn,
                 &measObjId, measUeCb,&cellCbMeasObj))
      {
         NhuMeasObjectToAddModmeasObject     *nhuMeasObj;

         nhuMeasObj = &nhuMeasObjAddMod[*idxVal].measObject;
         wrFillTknU8(&nhuMeasObjAddMod[*idxVal].pres, 1);
         WR_FILL_TKN_UINT(nhuMeasObjAddMod[*idxVal].measObjectId, measObjId)
         (*idxVal)++;
         wrUmmMeasFillCdmaNhuMsgForNrUpdt(memCp, cellCbMeasObj, nhuMeasObj, &nrUpdtId[idCnt]);
      }
   }

   RETVALUE(ROK);
}
/** @brief This function is responsible for to fill Eutra White cell list 
 * 
 *  @details
 * Function:wrUmmMeasFillEutraWLCellIdObj
 * 
 *   Processing steps:
 *    - Fill Nhu Eutra White cell Id List
 *     
 * @param[in]  cellIdx: cell Index         
 * @param[out] nhuWCellsAddMod: pointer to the Nhu white cell List addMod Object. 
 * @param[in]  wlCellLst: pointer to the white cell List. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasFillEutraWLCellIdObj
(
 U8                          cellIdx, 
 NhuCellsToAddMod            *nhuWCellsAddMod,
 CmLListCp                   *wlCellLst
)
{
   WrMeasNrWCellNode         *nrWCellNode;
   CmLList                   *nrWCellLnk;

   TRC2(wrUmmMeasFillEutraWLCellIdObj)
   
   nrWCellLnk = wlCellLst->first;
   while(NULLP != nrWCellLnk)
   {
      nrWCellNode = (WrMeasNrWCellNode*)nrWCellLnk->node;
      if(nrWCellNode->cellIndex == cellIdx)
      {
         wrFillTknU8(&nhuWCellsAddMod->pres, 1);
         WR_FILL_TKN_UINT(nhuWCellsAddMod->cellIdx, nrWCellNode->cellIndex)
         WR_FILL_TKN_UINT(nhuWCellsAddMod->physCellId, nrWCellNode->pci)
         WR_FILL_TKN_UINT(nhuWCellsAddMod->cellIvdualOffset, nrWCellNode->cio)
         break;
      }
      nrWCellLnk = nrWCellLnk->next;
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for to fill Eutra black cell list 
 * 
 *  @details
 * Function:wrUmmMeasFillEutraBLCellIdObj
 * 
 *   Processing steps:
 *    - Fill Nhu Eutra black cell Id List
 *     
 * @param[in]  cellIdx: cell Index         
 * @param[out] nhuBCellsAddMod: pointer to the Nhu black cell List addMod Object. 
 * @param[in]  blCellLst: pointer to the bhite cell List. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasFillEutraBLCellIdObj
(
 U8                          cellIdx, 
 NhuBlackCellsToAddMod       *nhuBCellsAddMod,
 CmLListCp                   *blCellLst
)
{
   CmLList                   *nrBCellLnk;
   WrMeasNrBCellNode         *nrBCellNode;

   TRC2(wrUmmMeasFillEutraBLCellIdObj)
   
   nrBCellLnk = blCellLst->first;
   while(NULLP != nrBCellLnk)
   {
      nrBCellNode = (WrMeasNrBCellNode*)nrBCellLnk->node;
      if(nrBCellNode->cellIndex == cellIdx)
      {
         wrFillTknU8(&nhuBCellsAddMod->pres, 1);
         WR_FILL_TKN_UINT(nhuBCellsAddMod->cellIdx, nrBCellNode->cellIndex)
         wrFillTknU8(&nhuBCellsAddMod->physCellIdRange.pres, 1);
         WR_FILL_TKN_UINT(nhuBCellsAddMod->physCellIdRange.start, nrBCellNode->startPhyCellID)
         WR_FILL_TKN_UINT(nhuBCellsAddMod->physCellIdRange.range, nrBCellNode->phyCellIdRange)
         if(nrBCellNode->phyCellIdRange != 0)
         {
            nhuBCellsAddMod->physCellIdRange.range.pres = PRSNT_NODEF;
         }
         else
         {
            nhuBCellsAddMod->physCellIdRange.range.pres = NOTPRSNT;
         }
         break;
      }
      nrBCellLnk = nrBCellLnk->next;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to fill Eutra NHU measurement Object
 * 
 *  @details
 * Function:wrUmmMeasFillEutraRrcAddModLstForNrUpdt
 *
 *   Processing steps:
 *    - Allaocate memory using memCp pointer
 *    - Fill Nhu measurement Object
 *     
 * @param[in]  memCp: pointer to memory control pointer         
 * @param[in]  cellCbMeasObj: pointer to the cellCb measurement Object. 
 * @param[in]  nhuMeasObj: poiner to nhu measurement Object. 
 * @param[in]  nrUpdtId: pointer to the neighbour update id structure. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasFillEutraRrcAddModLstForNrUpdt
(
 CmMemListCp                        *memCp,
 WrUmmMeasEutraMeasObjNode          *cellCbMeasObj,
 NhuMeasObjectToAddModmeasObject    *nhuMeasObj,
 WrMeasEutraNrUpdtIdNode            *nrUpdtId
)
{
   U8                       idCnt;
   U8                       cellIdx;
   U8                       addWlCellCnt;
   U8                       delWlCellCnt;
   U8                       addBlCellCnt;
   U8                       delBlCellCnt;
   NhuMeasObjectEUTRA       *nhuEutraMeasObj;
      
   TRC2(wrUmmMeasFillEutraRrcAddModLstForNrUpdt)
   
   addWlCellCnt = nrUpdtId->addWCellCnt;
   addBlCellCnt = nrUpdtId->addBCellCnt;
   delWlCellCnt = nrUpdtId->delWCellCnt;
   delBlCellCnt = nrUpdtId->delBCellCnt;
      
   nhuEutraMeasObj = &nhuMeasObj->val.measObjectEUTRA;
   
   WR_FILL_TKN_UINT(nhuMeasObj->choice, MEASOBJECT_MEASOBJECTEUTRA)
   wrFillTknU8(&nhuEutraMeasObj->pres, 1);
   WR_FILL_TKN_UINT(nhuEutraMeasObj->carrierFreq, cellCbMeasObj->carrFreq)
   WR_FILL_TKN_UINT(nhuEutraMeasObj->allowedMeasBandwidth, cellCbMeasObj->allowedMeasBW) 
   WR_FILL_TKN_UINT(nhuEutraMeasObj->presenceAntennaPort1, cellCbMeasObj->presAntPort1)
   wrFillTknStrBSXL(&nhuEutraMeasObj->neighCellConfig, 2, &cellCbMeasObj->neighCellCfg, memCp);
   WR_FILL_TKN_UINT(nhuEutraMeasObj->offsetFreq, cellCbMeasObj->qOffsetFreq)

   nhuEutraMeasObj->cellForWhichToReportCGI.pres = NOTPRSNT;
   /* White and Black list information*/
  
   if(0 != delWlCellCnt)
   {
      NhuCellIdx               *nhuRmvWLCell;

      WR_FILL_TKN_UINT(nhuEutraMeasObj->cellsToRemoveLst.noComp, delWlCellCnt)
      if(ROK != cmGetMem(memCp, delWlCellCnt * sizeof(NhuCellIdx), (Ptr *)&nhuRmvWLCell))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuEutraMeasObj->cellsToRemoveLst.member = nhuRmvWLCell;
     
      for(idCnt = 0; idCnt < delWlCellCnt; idCnt++)
      {
         WR_FILL_TKN_UINT(nhuRmvWLCell[idCnt], nrUpdtId->delWCellLst[idCnt])    
      }
   }
   if(0 != delBlCellCnt)
   {
      NhuCellIdx               *nhuRmvBLCell;
   
      WR_FILL_TKN_UINT(nhuEutraMeasObj->blackCellsToRemoveLst.noComp, delBlCellCnt)
      if(ROK != cmGetMem(memCp, delBlCellCnt * sizeof(NhuCellIdx), (Ptr *)&nhuRmvBLCell))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuEutraMeasObj->blackCellsToRemoveLst.member = nhuRmvBLCell;

      for(idCnt = 0; idCnt < delBlCellCnt; idCnt++)
      {
         WR_FILL_TKN_UINT(nhuRmvBLCell[idCnt], nrUpdtId->delBCellLst[idCnt])
      }
   }
   if(0 != addWlCellCnt)
   {
      NhuCellsToAddMod         *nhuWCellsAddMod;

      WR_FILL_TKN_UINT(nhuEutraMeasObj->cellsToAddModLst.noComp, addWlCellCnt)
      if(ROK != cmGetMem(memCp, addWlCellCnt * sizeof(NhuCellsToAddMod), (Ptr *)&nhuWCellsAddMod))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuEutraMeasObj->cellsToAddModLst.member = nhuWCellsAddMod;

      for(idCnt = 0; idCnt < addWlCellCnt; idCnt++)
      {
         cellIdx = nrUpdtId->addWCellLst[idCnt];
         wrUmmMeasFillEutraWLCellIdObj(cellIdx, &nhuWCellsAddMod[idCnt], &cellCbMeasObj->wlCellLst);
      }
   }
   if(0 != addBlCellCnt)
   {
      NhuBlackCellsToAddMod    *nhuBCellsAddMod;

      WR_FILL_TKN_UINT(nhuEutraMeasObj->blackCellsToAddModLst.noComp, addBlCellCnt)
      if(ROK != cmGetMem(memCp, addBlCellCnt * sizeof(NhuBlackCellsToAddMod), (Ptr *)&nhuBCellsAddMod))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      nhuEutraMeasObj->blackCellsToAddModLst.member = nhuBCellsAddMod;

      for(idCnt = 0; idCnt < addBlCellCnt; idCnt++)
      {
         cellIdx = nrUpdtId->addBCellLst[idCnt];
         wrUmmMeasFillEutraBLCellIdObj(cellIdx, &nhuBCellsAddMod[idCnt], &cellCbMeasObj->blCellLst);
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to send Eutra neighbour update
 * to all ue's.
 *
 *  @details
 * Function:wrUmmMeasSendEutraUpdtNrInfoToUe
 *
 *   Processing steps:
 *    - get the UeCb from Umm cell Cb
 *    - if Ue supports inter frequency Eutra send the neighbour update
 *      information to ue
 *
 * @param[in]  memCp: pointer to the memory pointer
 * @param[in]  idx: index for NHU structure
 * @param[in]  cellCb: pointer to the cell control block
 * @param[in]  measUeCb: pointer to the measurement Ue control block
 * @param[in]  nrUpdtIdCnt: Neighbour update id count.
 * @param[in]  nrUpdtId: poiner to WrMeasCdmaNrUpdtIdNode structure.
 * @param[in]  rrcReCfgMsg: pointer to reconfig Message.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 **/
PRIVATE S16 wrUmmMeasSendEutraUpdtNrInfoToUe
(
CmMemListCp                  *memCp,
U8                           *idx,
WrUmmCellCb                  *cellCb,
WrMeasUeCb                   *measUeCb,
U8                           nrUpdtIdCnt,
WrMeasEutraNrUpdtIdNode      *nrUpdtId,
NhuMeasConfig                *nhuMeasCfg
)
{
   U8                                  idCnt;
   NhuMeasObjectToAddMod               *nhuMeasObjAddMod;

   TRC2(wrUmmMeasSendEutraUpdtNrInfoToUe)

   nhuMeasObjAddMod = nhuMeasCfg->measObjectToAddModLst.member;

   for(idCnt = 0; idCnt < nrUpdtIdCnt; idCnt++)
   {
      U16                           arfcn;
      U8                            measObjId = 0;
      WrUmmMeasEutraMeasObjNode    *cellCbMeasObj = NULLP;

      arfcn = nrUpdtId[idCnt].arfcn;
      cellCbMeasObj = nrUpdtId[idCnt].cellCbMeasObj;

      /* Construct NHU Measurement Object List*/
      if(cellCbMeasObj != NULLP)
      {
         NhuMeasObjectToAddModmeasObject     *nhuMeasObj;
         /* For an internal message use the cellCbMeasObj coming as 
          * part of internal message transaction. */
         wrUmmMeasUpdtEutraMeasObjAndIdFromArfcn(arfcn, measUeCb, &measObjId,
               &cellCbMeasObj);
         cellCbMeasObj = nrUpdtId[idCnt].cellCbMeasObj;
         nhuMeasObj = &nhuMeasObjAddMod[*idx].measObject;
         wrFillTknU8(&nhuMeasObjAddMod[*idx].pres, 1);
         WR_FILL_TKN_UINT(nhuMeasObjAddMod[*idx].measObjectId, measObjId)
         wrUmmMeasFillEutraRrcAddModLstForNrUpdt(memCp, cellCbMeasObj, nhuMeasObj, &nrUpdtId[idCnt]);
         (*idx)++;
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to update Eutra measurement object
 *  from Afrcn. 
 *
 *  @details
 * Function: wrUmmMeasUpdtCellEutraMeasObjFrmArfcn
 *
 *   Processing steps:
 *    - tranverse cell Cb list find measurement object using Arfcn.
 *    -if Object not found create new object in cell Cb
 *     
 * @param[out] interMeasObjLst: pointer to inter measurement object list         
 * @param[in]  eutraCfgCnt:  Eutra config Params count 
 * @param[in]  eutraCfgParams: pointer to Eutra config Params 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PUBLIC S16 wrUmmMeasUpdtCellEutraMeasObjFrmArfcn
(
 U32                         arfcn, 
 CmLListCp                   *interMeasObjLst,
 WrUmmMeasEutraMeasObjNode   **enbMeasObj
)
{
   CmLList                     *measObjEutraNode;
   WrUmmMeasEutraMeasObjNode   *interEutraObj;

   TRC2(wrUmmMeasUpdtCellEutraMeasObjFrmArfcn)

   measObjEutraNode = interMeasObjLst->first;
   while (NULLP != measObjEutraNode)
   {
      interEutraObj = (WrUmmMeasEutraMeasObjNode*)measObjEutraNode->node;
      if (arfcn == interEutraObj->carrFreq)
      {
         *enbMeasObj = interEutraObj;
         RETVALUE(ROK);
      }
   measObjEutraNode = measObjEutraNode->next;
   }
   RETVALUE(RFAILED);
}

  /** @brief This function is responsible for to update Utra measurement object
   *  from Afrcn. 
   *
   *  @details
   * Function: wrUmmMeasUpdtCellUtraMeasObjFrmArfcn
   *
   *   Processing steps:
   *   - tranverse cell Cb list find measurement object using Arfcn.
   *   -if Object not found create new object in cell Cb
   *    
   * @param[out] interMeasObjLst: pointer to inter measurement object list         
   * @param[in]  utraCfgCnt:  utra config Params count 
   * @param[in]  utraCfgParams: pointer to utra config Params 
   * @return S16
   *      SUCCESS : ROK
   *      FAILED  : RFAILED
  **/
PUBLIC S16 wrUmmMeasUpdtCellUtraMeasObjFrmArfcn
(
   U32                          arfcn, 
   CmLListCp                   *utraMeasObjLst,
   WrUmmMeasUtraMeasObjNode      **utraMeasObj
)
{
   CmLList                       *measObjUtraNode;
   WrUmmMeasUtraMeasObjNode     *utraObj;
  
   TRC2(wrUmmMeasUpdtCellUtraMeasObjFrmArfcn);
  
   measObjUtraNode = utraMeasObjLst->first;
   while (NULLP != measObjUtraNode)
   {
      utraObj = (WrUmmMeasUtraMeasObjNode*)measObjUtraNode->node;
      if(arfcn == utraObj->arfcn)
      {
         *utraMeasObj = utraObj;
         RETVALUE(ROK);
      }
      measObjUtraNode = measObjUtraNode->next;
   }
   RETVALUE(RFAILED);
}

/** @brief This function is responsible for to deleting the neighbor cell
 * and update the white/black listed cells for hrl UEs.  
 * 
 *  @details
 * Function: wrUmmMeasDelWLEutraNghInHrlUeCb 
 *
 *   Processing steps:
 *    - Fetch the whiteLst and then compare the pci, if matches then delete the
 *    entry from whiteLst and update the nrUpdtId.
 *    - Else fetch the blackLst and then compare the pci, if matches then delete
 *    the entry from blackLst and update the nrUpdtId.
 *
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 **/
PRIVATE S16 wrUmmMeasDelWLEutraNghInHrlUeCb 
(
 U16                               pci,
 WrUmmMeasEutraMeasObjNode        *ueEutraMeasObj,
 WrMeasEutraNrUpdtIdNode          *nrUpdtId
 )
{
   CmLList                   *nrWCellLnk;
   CmLList                   *nrBCellLnk;
   U8                        cellIndex; 
   WrMeasNrWCellNode         *cellWLstNode = NULLP;
   WrMeasNrBCellNode         *cellBLstNode = NULLP;
   
   TRC2(wrUmmMeasDelWLEutraNghInHrlUeCb)

   /* Search the pci in blacklist of HrlMeasCfg */
   nrBCellLnk = ueEutraMeasObj->blCellLst.first;
   while (NULLP != nrBCellLnk)
   {
      cellBLstNode = (WrMeasNrBCellNode*)nrBCellLnk->node;
      if (pci == cellBLstNode->startPhyCellID)
      {
         cellIndex = cellBLstNode->cellIndex;
         cmLListDelFrm (&ueEutraMeasObj->blCellLst, nrBCellLnk);
         WR_FREE(cellBLstNode, sizeof(WrMeasNrBCellNode));
         ueEutraMeasObj->blCellIdxInUse[cellIndex - 1] = FALSE; 
         /* update in nrUpdtId */
         nrUpdtId->delBCellLst[nrUpdtId->delBCellCnt] = cellIndex;
         nrUpdtId->delBCellCnt++;
         RETVALUE(ROK);
      }
      nrBCellLnk = nrBCellLnk->next;
   }

   /* Search the pci in whitelist of HrlMeasCfg */
   nrWCellLnk = ueEutraMeasObj->wlCellLst.first;

   while (NULLP != nrWCellLnk)
   {
      cellWLstNode = (WrMeasNrWCellNode*)nrWCellLnk->node;

      if (pci == cellWLstNode->pci)
      {
         cellIndex = cellWLstNode->cellIndex;
         cmLListDelFrm (&ueEutraMeasObj->wlCellLst, nrWCellLnk);
         WR_FREE(cellWLstNode, sizeof(WrMeasNrWCellNode));
         ueEutraMeasObj->wlCellIdxInUse[cellIndex - 1] = FALSE; 
         /* update in nrUpdtId */
         nrUpdtId->delWCellLst[nrUpdtId->delWCellCnt] = cellIndex;
         nrUpdtId->delWCellCnt++;
         RETVALUE(ROK);
      }
      nrWCellLnk = nrWCellLnk->next;
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for to deleting the neighbor cells
 * and update the white/black listed cells for hrl UEs.  
 * 
 *  @details
 * Function: wrUmmMeasDelBLEutraNghInHrlUeCb 
 *
 *   Processing steps:
 *    - Fetch the blackLst and then compare the pci, if matches then delete
 *    the entry from blackLst and update the nrUpdtId.
 *
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 **/
PRIVATE S16 wrUmmMeasDelBLEutraNghInHrlUeCb 
(
 U16                               pci,
 WrUmmMeasEutraMeasObjNode        *ueEutraMeasObj,
 WrMeasEutraNrUpdtIdNode          *nrUpdtId
 )
{
   CmLList                   *nrBCellLnk;
   U8                        cellIndex; 
   WrMeasNrBCellNode         *cellBLstNode = NULLP;

   TRC2(wrUmmMeasDelBLEutraNghInHrlUeCb)

   nrBCellLnk = ueEutraMeasObj->blCellLst.first;

   /* Search the pci in blacklist of HrlMeasCfg */
   while (NULLP != nrBCellLnk)
   {
      cellBLstNode = (WrMeasNrBCellNode*)nrBCellLnk->node;
      if (pci == cellBLstNode->startPhyCellID)
      {
         cellIndex = cellBLstNode->cellIndex;
         cmLListDelFrm (&ueEutraMeasObj->blCellLst, nrBCellLnk);
         WR_FREE(cellBLstNode, sizeof(WrMeasNrBCellNode));
         ueEutraMeasObj->blCellIdxInUse[cellIndex - 1] = FALSE; 
         /* update in nrUpdtId */
         nrUpdtId->delBCellLst[nrUpdtId->delBCellCnt] = cellIndex;
         nrUpdtId->delBCellCnt++;
         RETVALUE(ROK);
      }
      nrBCellLnk = nrBCellLnk->next;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for fetching the eutra measObj
 * 
 *  @details
 * Function: wrUmmGetEutraMeasObjNode 
 *
 *   Processing steps:
 *      - Process the eutraLst and if carrFreq matches with arfcn then return
 *        the eutraMeasObjNode
 *
 * @param[in]  arfcn : arfcn to compare
 * @param[in]  eutraLst : eutran Meas Lst
 * @return WrUmmMeasEutraMeasObjNode* 
 *       SUCCESS : WrUmmMeasEutraMeasObjNode
 *       FAILED  : NULLP
 **/

PRIVATE WrUmmMeasEutraMeasObjNode* wrUmmGetEutraMeasObjNode
(
 U32         arfcn,
 CmLListCp   eutraLst
 )
{
   WrUmmMeasEutraMeasObjNode        *eutraMeasObjNode = NULLP;

   TRC2(wrUmmGetEutraMeasObjNode)

   cmLListFirst(&eutraLst);
   while(cmLListCrnt(&eutraLst))
   {
      eutraMeasObjNode = (WrUmmMeasEutraMeasObjNode *) cmLListNode(cmLListCrnt(&eutraLst));
      if(arfcn == eutraMeasObjNode->carrFreq)
      {
         RETVALUE(eutraMeasObjNode);
      }
      cmLListNext(&eutraLst);
   }
   RETVALUE(NULLP);
}

/** @brief This function is responsible for to comparing the newly
 * detected/added/modified neighbor cells and update the white/black cells for
 * all the HRL UEs.
 * 
 *  @details
 * Function: wrUmmMeasUpdtWCellLstinUeCb 
 *
 *   Processing steps:
 *    - get the Object from arfcn and update pci's in whiteList of 
 *      respective object
 *    - if object is not present, then create new object for hrl ueCb
 *
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 **/

PRIVATE S16 wrUmmMeasUpdtWCellLstinUeCb 
(
 WrUmmMeasEutraMeasObjNode        *eutraMeasObjNode,
 WrNrPerFreqWlInfo                *wlCellInfo,
 WrUeCb                           *ueCb,
 WrMeasEutraNrUpdtIdNode          *nrUpdtId
 )
{
   U8                          idCnt;
   U16                           pci;
   U32                       servCarrFreq;
   WrUmmCellCb               *cellCb;
   WrUmmMeasEutraMeasObjNode *enbMeasObj;
   WrMeasNrWCellNode          *nrCellNode;

   TRC2(wrUmmMeasUpdtWCellLstinUeCb)

      /* Below changes are required to update the HRL UEMeasCfg for addition of
       * new neighbors */

      WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
      if(NULLP == cellCb)
      {
         RLOG0(L_ERROR, "cellCb is NULL");
         RETVALUE(RFAILED);
      }  

   servCarrFreq = wrEmmGetServCarrierFreq(ueCb->cellId);
   if(eutraMeasObjNode->carrFreq != servCarrFreq)
   {
      if(RFAILED == wrUmmMeasUpdtCellEutraMeasObjFrmArfcn(eutraMeasObjNode->carrFreq,\
               &cellCb->measCellCb->interMeasObjLst, &enbMeasObj))
      {
         RLOG0(L_ERROR, "Eutra Object not found for Hrl UeCb");
         RETVALUE(RFAILED);
      }
   }
   else
   {
      enbMeasObj = &cellCb->measCellCb->intraMeasObj;
   }

   for(idCnt = 0; idCnt < wlCellInfo->numWlCells; idCnt++)
   {
      pci = wlCellInfo->cellInfo[idCnt].phyCellId;

      if(WR_EMM_ANR_DEL_NR ==  wlCellInfo->cellInfo[idCnt].nrUpdtType)
      {
         /* Delete from whitelist/blackList of hrlUeMeasCfg */
         if(RFAILED == wrUmmMeasDelWLEutraNghInHrlUeCb(pci, eutraMeasObjNode,
                  nrUpdtId))
         {
            RLOG0(L_ERROR, "Eutra Ngh Cell deletion failed in Hrl UeCb ");
            RETVALUE(RFAILED);
         }
      }
      else
      {
         /* This is applicable to add new neighbor cell in hrlUeMeasCfg */
         cmLListFirst(&enbMeasObj->wlCellLst);
         while(cmLListCrnt(&enbMeasObj->wlCellLst))
         {
            nrCellNode = (WrMeasNrWCellNode *) cmLListNode(cmLListCrnt(&enbMeasObj->wlCellLst));

            if(pci == nrCellNode->pci)
            {
               /* Call wrUmmUpdtHrlMeasCfg to update the wlCellIdxInUse by 1 or
                * blCellIdxInUse by 1 and insert the new neighbor into wlCellLst
                * or blCellLst after HRL decision */
               wrUmmUpdtHrlMeasCfg(ueCb, nrCellNode, eutraMeasObjNode, nrUpdtId);
               /* No need to return, as we will process the next PCI for addition*/
            }
            cmLListNext(&enbMeasObj->wlCellLst);
         }
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to comparing the newly
 * detected/added/modified neighbor cells and update the white/black cells for
 * all the HRL or non-HRL UEs.
 * 
 *  @details
 * Function: wrUmmMeasUpdtBCellLstinUeCb 
 *
 *   Processing steps:
 *    - get the Object from arfcn and update pci's in blackList of 
 *      respective object
 *    - if object is not present, then create new object for hrl ueCb
 *
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 **/
PRIVATE S16 wrUmmMeasUpdtBCellLstinUeCb 
(
 WrUmmMeasEutraMeasObjNode     *eutraMeasObjNode,
 WrNrPerFreqBlInfo             *blCellInfo,
 WrUeCb                        *ueCb,
 WrMeasEutraNrUpdtIdNode       *nrUpdtId
 )
{
   U8                          idCnt;
   U16                           pci;
   U32                       servCarrFreq;
   WrMeasNrBCellNode   *blCellNode;
   WrUmmCellCb               *cellCb;
   WrUmmMeasEutraMeasObjNode *enbMeasObj;
   U8                        cellIndex = 0;
   WrUmmMeasEutraMeasObjNode  *hrlMeasObj;
   Bool                       cellFnd = FALSE;

   TRC2(wrUmmMeasUpdtBCellLstinUeCb)

      /* Below changes are required to update the HRL UEMeasCfg for addition of
       * new neighbors */

      WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
      if(NULLP == cellCb)
      {
         RLOG0(L_ERROR, "cellCb is NULL");
         RETVALUE(RFAILED);
      }  
   servCarrFreq = wrEmmGetServCarrierFreq(ueCb->cellId);

   if(eutraMeasObjNode->carrFreq!= servCarrFreq)
   {
      if(RFAILED == wrUmmMeasUpdtCellEutraMeasObjFrmArfcn(eutraMeasObjNode->carrFreq, \
               &cellCb->measCellCb->interMeasObjLst, &enbMeasObj))
      {
         RLOG0(L_ERROR, "Eutra Object not found for Hrl UeCb");
         RETVALUE(RFAILED);
      }
   }
   else
   {
      enbMeasObj = &cellCb->measCellCb->intraMeasObj;
   }

   for(idCnt = 0; idCnt < blCellInfo->numBlInfos; idCnt++)
   {
      pci = blCellInfo->blInfo[idCnt].startPhyCellID;

      if(WR_EMM_ANR_DEL_NR ==  blCellInfo->blInfo[idCnt].nrUpdtType)
      {
         if(RFAILED == wrUmmMeasDelBLEutraNghInHrlUeCb(pci, eutraMeasObjNode, nrUpdtId))
         {
            RLOG0(L_ERROR, "Eutra Ngh cell deletion failed for Hrl UeCb");
            RETVALUE(RFAILED);
         }
      }
      else
      {
          cellFnd = FALSE;

         /* Populate in the hrlMeasCfg */
         hrlMeasObj = wrUmmHrlEutraMeasObjCfg( eutraMeasObjNode, ueCb);

         if(hrlMeasObj == NULLP)
         {
            RLOG0(L_ERROR, "hrlMeasObj is NULLP");
            RETVALUE(NULLP);
         }

         /* Check if the cell already exists in blackList of HRL */
         cmLListFirst(&hrlMeasObj->blCellLst);

         while(cmLListCrnt(&hrlMeasObj->blCellLst))
         {
            blCellNode = (WrMeasNrBCellNode *) cmLListNode
               (cmLListCrnt(&hrlMeasObj->blCellLst));

            if(blCellNode->startPhyCellID == pci)
            {
               cellFnd = TRUE;
               break;
            }
            cmLListNext(&hrlMeasObj->blCellLst);
         }

         /* Insert new entry in BlackLst of Hrl UeCb */
         if ( TRUE != cellFnd)
         {
            WR_ALLOC(&blCellNode, sizeof(WrMeasNrBCellNode));
            if(blCellNode == NULLP)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            wrUmmMeasGetFreeCellIdxForEutra(hrlMeasObj->blCellIdxInUse, &cellIndex);

            blCellNode->cellIndex = cellIndex;
            blCellNode->startPhyCellID = pci;
            blCellNode->phyCellIdRange = 0;
            blCellNode->lnk.node = (PTR) blCellNode;
            cmLListAdd2Tail(&hrlMeasObj->blCellLst, &blCellNode->lnk);

            nrUpdtId->addBCellLst[nrUpdtId->addBCellCnt++] = cellIndex;
         }
      }
   }

   RETVALUE(ROK);
}

/* DYNAMIC_OAM and HRL interaction changes -start */

/** @brief This function is responsible for to comparing the newly
 * detected/added/modified neighbor cells and update the white/black 
 * cells for all the HRL or non-HRL UEs.
 * 
 *  @details
 * Function: wrUmmMeasUpdtEutraInUeCb
 *
 *   Processing steps:
 *      - Populate the arfcn and eutraMeasObj.
 *      - Call wrUmmMeasUpdtWCellLstinUeCb, which will update the whiteListed
 *        cells for HRL UeMeasCfg.
 *      - Call wrUmmMeasUpdtBCellLstinUeCb, which will update the blackListed
 *        cells for HRL UeMeasCfg.
 *
 * @param[in]  nrUpdtNode   : pointer to the WrUmmIntMeasTrans object
 * @param[in]  intMeasTrans : pointer to the WrUmmIntMeasTrans object 
 * @param[in]  nrUpdtInfo   : Neighbour update List
 * @param[in]  ueCb         : UeCb
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 **/
PRIVATE S16 wrUmmMeasUpdtEutraInUeCb
(
 WrUmmIntMeasTrans            *nrUpdtNode,
 WrUmmIntMeasTrans            *intMeasTrans,
 WrNrUpdtInfo                 *nrUpdtInfo,
 WrUeCb                       *ueCb
 )
{
   U8                        freqCnt;
   WrNrPerFreqWlInfo         *wlCellInfo;
   WrNrPerFreqBlInfo         *blCellInfo;
   U32                       arfcn;
   WrNrPerFreqInfo           *perFreqInfo;
   WrUmmMeasEutraMeasObjNode        *eutraMeasObjNode = NULLP;
   WrMeasEutraNrUpdtIdNode     *nrUpdtId = nrUpdtNode->eutraNode;
   U8                         modifiedCellCnt = 0;

   for(freqCnt = 0; freqCnt < nrUpdtInfo->eutran.numFreq; freqCnt++)
   {
      perFreqInfo = nrUpdtInfo->eutran.freqList;
      arfcn = perFreqInfo[freqCnt].dlCarrierFreq;
      wlCellInfo = &perFreqInfo[freqCnt].wlData;
      nrUpdtId[freqCnt].arfcn = arfcn;

      if(ueCb->hoRstrctInfo != NULLP)
      {
         if((ueCb->hrlMeasCfg.intraMeasObj != NULLP) &&
            (arfcn != ueCb->hrlMeasCfg.intraMeasObj->carrFreq))
         {
            eutraMeasObjNode = (WrUmmMeasEutraMeasObjNode *) wrUmmGetEutraMeasObjNode \
                               (arfcn, ueCb->hrlMeasCfg.interMeasObjLst);
         }
         else
         {
            eutraMeasObjNode = ueCb->hrlMeasCfg.intraMeasObj;
         }
         if ( NULLP == eutraMeasObjNode )
         {
         /* If arfcn is not at all configured for the UE, then
          * eutraMeasObjNode is NULLP
          * if no meaurement object found i.e. not configured to UE yet then
          * this function returns RFAILED.
          * in RFAILED case, nrUpdtId.cellCbMeasObj will be set to NULLP so that
          * while building RRC Reconfiguration Request message if inter
          * frequency measurement objects have not configured yet to UE those
          * will be ignored with NULL check for nrUpdtId.cellCbMeasObj */
            nrUpdtId[freqCnt].cellCbMeasObj = NULLP;
            continue;
         }

         /* After fetching the eutraMeasObj process the WCellLst */
         wrUmmMeasUpdtWCellLstinUeCb(eutraMeasObjNode , wlCellInfo, ueCb,
               &nrUpdtId[freqCnt]);

         /* After fetching the eutraMeasObj process the BCellLst */
         blCellInfo = &perFreqInfo[freqCnt].blData;
         wrUmmMeasUpdtBCellLstinUeCb(eutraMeasObjNode , blCellInfo, ueCb, 
               &nrUpdtId[freqCnt]);
      }
      else
      {
         U8       measObjId;

         /* Check that particular arfcn is configured for measurements, 
          * In this case measObjId is not used further */

         /* if no meaurement object found i.e. not configured to UE yet then
          * this function returns RFAILED.
          * in RFAILED case, nrUpdtId.cellCbMeasObj will be set to NULLP so that
          * while building RRC Reconfiguration Request message if inter
          * frequency measurement objects have not configured yet to UE those
          * will be ignored with NULL check for nrUpdtId.cellCbMeasObj */
         if(ROK != wrUmmMeasUpdtEutraMeasObjAndIdFromArfcn(arfcn, ueCb->measCfg,
                  &measObjId, &eutraMeasObjNode))
         {
            nrUpdtId[freqCnt].cellCbMeasObj = NULLP;
            continue;
         }

         /* Copying the eutra - add/delete nrUpdate info */

         /* TODO: other RAT neighbors ANR detection/dynamic OAM update is not
          * enabled yet by TotaleNodeB, if enabled, need to copy other RATs 
          * new neighbor cell's info */
         if ( 0 != intMeasTrans->numEutra)
         {
            cmMemcpy((U8 *)&nrUpdtId[freqCnt], (U8 *)intMeasTrans->eutraNode,\
                  sizeof(WrMeasEutraNrUpdtIdNode));
         }
      }
      modifiedCellCnt += nrUpdtId[freqCnt].addWCellCnt + nrUpdtId[freqCnt].addBCellCnt + \
                        nrUpdtId[freqCnt].delWCellCnt + nrUpdtId[freqCnt].delBCellCnt ;

      /* Copy the nrUpdtId with eutraMeasObjNode */
      nrUpdtId[freqCnt].cellCbMeasObj = eutraMeasObjNode;
   }

   if ( 0 == modifiedCellCnt)
   {
      /* Need to get the modifiedCellCnt, if none of the HRL Cell is modified
       * for any frequency/measObject then retrun RFAILED, so that RRC Reconfig
       * msg should not trigger for this UE */
      RETVALUE(RFAILED);
   }

   nrUpdtNode->numEutra = intMeasTrans->numEutra;

   RETVALUE(ROK);
}
/* DYNAMIC_OAM and HRL interaction changes -end */

/** @brief This function is responsible for to create Eutra internal
* transction.
*
*  @details
* Function:wrUmmMeasUpdtIntTransMsg
*
*   Processing steps:
*    - Create internal transaction.
*    - process the reconfig message
*
* @param[in]  cellCb: pointer to Cell Cb
* @param[in]  nrUpdtIdNode: Neighbour update id Object
* @param[in]  nrUpdtInfo: Neighbour update List
* @return S16
*       SUCCESS : ROK
*       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasUpdtIntTransMsg
(
 WrUmmCellCb                  *cellCb,
 WrUmmIntMeasTrans            *intMeasTrans,
 WrNrUpdtInfo                 *nrUpdtInfo
 )
{
   WrUmmIntMeasTrans         *nrUpdtNode;
   WrUeCb                    *ueCb;
   U8                        ueCnt;

   TRC2(wrUmmMeasUpdtIntTransMsg)

   /* Building internal message */

   for(ueCnt = 0; ueCnt < cellCb->maxUesPerCell; ueCnt++)
   {
         ueCb = cellCb->ueCbs[ueCnt];
         if(NULLP != ueCb)
         {
            /* WR_SZ_MEM_SDU_SIZE is 2048B, this is not relavent macro name but using it
             * rather than passing 2048 hardcoded value */
            if (ROK != cmAllocEvnt(sizeof(WrUmmIntMeasTrans), WR_SZ_MEM_SDU_SIZE, &wrCb.mem,
                     (Ptr *)&nrUpdtNode))
            {    
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }/* End of if condition.*/ 

            cmMemset((U8 *)(nrUpdtNode), 0,sizeof(WrUmmIntMeasTrans));

            WR_GET_MEM(&nrUpdtNode->memCp, intMeasTrans->numEutra * sizeof(WrMeasEutraNrUpdtIdNode),
                  (Ptr*)&nrUpdtNode->eutraNode);
            if(NULLP == nrUpdtNode->eutraNode)
            {
               WR_FREE_EVNT(nrUpdtNode);
               RLOG0(L_FATAL, "Memory allocation failed.");
               continue;
            }

            /* TODO: other RAT neighbors ANR detection is not enabled yet by TotaleNodeB
             * if enabled, need to copy other RATs new neighbor cell's info */

            /* DYNAMIC_OAM and HRL interaction changes -start */
            if( ROK != wrUmmMeasUpdtEutraInUeCb (nrUpdtNode, intMeasTrans, 
                     nrUpdtInfo, ueCb))
            {
               WR_FREE_EVNT(nrUpdtNode);
               RLOG1(L_INFO, "CRNTI:[%d] Reconfiguration Request is not required\
                     to send to UE",ueCb->crnti);
               continue;
            }
            /* DYNAMIC_OAM and HRL interaction changes -end */

            /* create internal msg for those UEs who are configured with measObj */
            if(ROK != wrUmmMeasCreateIntTrans(cellCb, ueCb, WR_UMM_INTMSG_MEAS_TRANS,
                     WR_UMM_MEAS_NR_EXIST_FREQ, nrUpdtNode, NULLP))
            {
               WR_FREE_EVNT(nrUpdtNode);
            }
         }
      }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to process Eutra neighbour update. 
 *
 *  @details
 * Function:wrUmmMeasProcEutraNrUpdat
 *
 *   Processing steps:
 *    - get the Object from arfcn and update pci's in respective object
 *    - if object is not resent create new object in cell cb
 *    - send to all Ue's if Object is configured.
 *    -if it is new object send to all Ue's
 *     
 * @param[in]  cellId: cell Id         
 * @param[in]  interFreq: poiner to WrNrInterFreqInfo structure. 
 * @param[in]  nrUpdtId: poiner to WrMeasEutraNrUpdtIdNode structure. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasProcEutraNrUpdat
(
 U16                         cellId,
 WrNrInterFreqInfo           *interFreq,
 WrMeasEutraNrUpdtIdNode     *nrUpdtId
)
{
   U8                        freqCnt;
   U8                        noofFreq;
   U32                       servCarrFreq;
   WrUmmMeasCellCb           *measCellCb;
   WrNrPerFreqInfo           *perFreqInfo;
   LwrCellMeasCfgGrp         *measCellCfg;
   WrUmmCellCb               *cellCb;

   TRC2(wrUmmMeasProcEutraNrUpdat)
   
   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }
  
   servCarrFreq = wrEmmGetServCarrierFreq(cellId);
   measCellCb = cellCb->measCellCb;
   measCellCfg = cellCb->measCellCfg;
   noofFreq = interFreq->numFreq;

   for(freqCnt = 0; freqCnt < noofFreq; freqCnt++)
   {
      U32                       arfcn;
      WrNrPerFreqWlInfo         *wlCellInfo;
      WrNrPerFreqBlInfo         *blCellInfo;
      WrUmmMeasEutraMeasObjNode *enbMeasObj;

      perFreqInfo = interFreq->freqList;
      arfcn = perFreqInfo[freqCnt].dlCarrierFreq;
      wlCellInfo = &perFreqInfo[freqCnt].wlData;
      blCellInfo = &perFreqInfo[freqCnt].blData;
         
      nrUpdtId[freqCnt].arfcn = arfcn;
     
      if(arfcn != servCarrFreq)
      {
         if(RFAILED == wrUmmMeasUpdtCellEutraMeasObjFrmArfcn(arfcn, &measCellCb->
               interMeasObjLst, &enbMeasObj))
         {
            RLOG0(L_ERROR, "Eutra Object not found");
            RETVALUE(RFAILED);
         }
      }
      else
      {
         nrUpdtId[freqCnt].arfcn = servCarrFreq;
         enbMeasObj = &measCellCb->intraMeasObj;
      }
      nrUpdtId[freqCnt].cellCbMeasObj = enbMeasObj;
   
      wrUmmMeasUpdtEutraWLCellinCellCb(enbMeasObj->wlCellIdxInUse,
               &enbMeasObj->wlCellLst, wlCellInfo, &nrUpdtId[freqCnt]);
      wrUmmMeasUpdtEutraBLCellinCellCb(enbMeasObj->blCellIdxInUse,
               &enbMeasObj->blCellLst, blCellInfo, &nrUpdtId[freqCnt]);
   }
 
   RETVALUE(ROK);
}

/**This function is responsible for to process Utra neighbour update.
 *
 *  @details
 *  Function:wrUmmMeasProcUtraNrUpdat
 *
 *   Processing steps:
 *    - get the Object from arfcn and update psc's in respective object
 *    - if object is not recent create new object in cell cb
 *    - send to all Ue's if Object is configured.
 *    -if it is new object send to all Ue's
 *
 * @param[in]  cellId: cell Id
 * @param[in]  utraFreq: poiner to WrNrUtraFreqInfo structure.
 * @param[in]  nrUpdtId: poiner to WrMeasUtraNrUpdtIdNode structure.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasProcUtraNrUpdat
(
 U16                         cellId,
 WrNrUtraFreqInfo            *utraFreq,
 WrMeasUtraNrUpdtIdNode      *nrUpdtId,
 U8		         		     *utraUpdtIdx,
 WrUmmMeasCellTyp             utraType
)
{
   U8		         		freqCnt;
   U8				         noOfFreq;
   WrUmmMeasCellCb 	     	 *measCellCb;
   WrNrUtraPerFreqInfo	     *perFreqInfo;
   LwrCellMeasCfgGrp		 *measCellCfg;
   WrUmmCellCb		    	 *cellCb;
   CmLListCp             *utraMeasObjLst;

   TRC2(wrUmmMeasProcUtraNrUpdat)

      WR_UMM_GET_CELLCB(cellCb, cellId);
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }

   measCellCb = cellCb->measCellCb;
   measCellCfg = cellCb->measCellCfg;
   noOfFreq = utraFreq->numFreq;

   for(freqCnt = 0; freqCnt < noOfFreq; freqCnt++)
   {
      U32                     arfcn;
      WrNrUtraPerFreqWlInfo       *wlCellInfo;
      WrUmmMeasUtraMeasObjNode          *utraMeasObj;

      perFreqInfo = utraFreq->freqList;
      arfcn = perFreqInfo[freqCnt].dlCarrierFreq;
      wlCellInfo = &perFreqInfo[freqCnt].wlData;

      nrUpdtId[freqCnt].arfcn = arfcn;
      if(utraType == WR_UMM_MEAS_UTRA_FDD_FREQ)
      {
         utraMeasObjLst = &cellCb->measCellCb->utraFddMeasObjLst;
      }
      else
      {
         utraMeasObjLst = &cellCb->measCellCb->utraTddMeasObjLst;
      }
      if(RFAILED == wrUmmMeasUpdtCellUtraMeasObjFrmArfcn(arfcn,
               utraMeasObjLst, &utraMeasObj))
      {
         (*utraUpdtIdx) = freqCnt;
         RETVALUE(RFAILED);
      }

      wrUmmMeasUpdtUtraWLCellinCellCb(utraMeasObj->cellIdxInUse,
            &utraMeasObj->cellLst, wlCellInfo, &nrUpdtId[freqCnt + (*utraUpdtIdx)]);
   }

   (*utraUpdtIdx) = freqCnt;
   RETVALUE(ROK);
}



/**   @brief This function is responsible for to update cdma measurement object
 *  from Arfcn. 
 *
 *  @details
 * Function:wrUmmMeasUpdtCellCdmaMeasObjFrmArfcn
 *
 *   Processing steps:
 *    - tranverse cell Cb list find measurement object using Arfcn.
 *    -if Object not found create new object in cell Cb
 *     
 * @param[in]  bandClass: Band Class         
 * @param[in]  arfcn: Arfcn         
 * @param[in]  interRatMeasObjLst: pointer to interRat measurement object list         
 * @param[out] cdmaMeasObj: pointer to Cdma measurement object         
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasUpdtCellCdmaMeasObjFrmArfcn
(
 U8                          bandClass,
 U32                         arfcn, 
 CmLListCp                   *interRatMeasObjLst,
 WrUmmMeasCdmaMeasObjNode    **cdmaMeasObj
)
{
   CmLList                   *measObjCdmaNode;
   WrUmmMeasCdmaMeasObjNode  *interRatObj;

   TRC2(wrUmmMeasUpdtCellCdmaMeasObjFrmArfcn)

   measObjCdmaNode = interRatMeasObjLst->first;
   while (NULLP != measObjCdmaNode)
   {
      interRatObj = (WrUmmMeasCdmaMeasObjNode*)measObjCdmaNode->node;
      if((bandClass == interRatObj->bandClass) &&
         (arfcn == interRatObj->arfcn))
      {
         *cdmaMeasObj = interRatObj;
         RETVALUE(ROK);
      }
   measObjCdmaNode = measObjCdmaNode->next;
   }
   RETVALUE(RFAILED);
}

/** @brief This function is responsible for to get Cdma Frequency count. 
 *
 *  @details
 * Function:wrUmmMeasGetCdmaFreqCnt
 *
 *   Processing steps:
 *    - find Cdma Frequency count
 *     
 * @param[out] nrUpdtIdCnt: pointer to Nr Update Id Count         
 * @param[in] cdmaFreq: pointer to Cdma Frequency         
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
 PRIVATE S16 wrUmmMeasGetCdmaFreqCnt
(
 U8                          *nrUpdtIdCnt,
 WrCDMA2KNeighInfo           *cdmaFreq
 )
{
   U8                        numBandClass;
   U8                        bandCnt;
   U8                        numFreq;
   U8                        freqCnt;
   WrCDMA2KBandInfo          *neighBandLst;

   TRC2(wrUmmMeasGetCdmaFreqCnt)

   numBandClass = cdmaFreq->numBandClass;
   neighBandLst = cdmaFreq->neighBandLst;
   for (bandCnt = 0; bandCnt < numBandClass; bandCnt++)
   {
      numFreq  = neighBandLst[bandCnt].numFreq;
      for (freqCnt = 0; freqCnt < numFreq; freqCnt++)
      {
         *nrUpdtIdCnt = *nrUpdtIdCnt + 1;
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to process 1xCdma neighbour update. 
 *
 *  @details
 * Function:wrUmmMeasProc1xCdmaNrUpdt
 *
 *   Processing steps:
 *    - get the Object from arfcn and update pci's in respective object
 *    - send to all Ue's if Object is configured.
 *     
 * @param[in]  cellCb: cell control block         
 * @param[in]  cdmaFreq: poiner to WrCDMA2KNeighInfo structure. 
 * @param[in]  cdmaNrUpdtId: pointer to WrMeasCdmaNrUpdtIdNode object         
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasProc1xCdmaNrUpdt
(
 WrUmmCellCb                 *cellCb,
 WrCDMA2KNeighInfo           *cdmaFreq,
 WrMeasCdmaNrUpdtIdNode      *cdmaNrUpdtId 
)
{
   U8                        numBandClass;
   U8                        bandCnt;
   U8                        idCnt = 0;
   WrUmmMeasCellCb           *measCellCb;
   WrCDMA2KBandInfo          *neighBandLst;

   TRC2(wrUmmMeasProc1xCdmaNrUpdt)

   measCellCb = cellCb->measCellCb;
   numBandClass = cdmaFreq->numBandClass;
   neighBandLst = cdmaFreq->neighBandLst;

   for (bandCnt = 0; bandCnt < numBandClass; bandCnt++)
   {
      U8                        numFreq;
      U8                        freqCnt;
      U8                        bandClsVal;
      WrCDMA2KFreqInfo          *neighFreqInfo;

      bandClsVal = neighBandLst[bandCnt].bandClass;
      numFreq  = neighBandLst[bandCnt].numFreq;
      neighFreqInfo = neighBandLst[bandCnt].neighFreqInfo;
      cdmaNrUpdtId[idCnt].bandClass = bandClsVal;
      for (freqCnt = 0; freqCnt < numFreq; freqCnt++)
      {
         U32                       arfcn;
         WrUmmMeasCdmaMeasObjNode  *cdmaMeasObj;

         arfcn = neighFreqInfo[freqCnt].arfcn;
         cdmaNrUpdtId[idCnt].arfcn = arfcn;

         if(RFAILED == wrUmmMeasUpdtCellCdmaMeasObjFrmArfcn(bandClsVal, arfcn,
                   &measCellCb->cdma1xMeasObjLst, &cdmaMeasObj))
         {
            RLOG0(L_ERROR, "1xCdma Object not Found");
            RETVALUE(RFAILED);
         }
         cdmaNrUpdtId[idCnt].cellCbMeasObj = cdmaMeasObj;
         if( RFAILED == wrUmmMeasUpdtCdmaCellsinCellCb(cdmaMeasObj->cdmaCellIdxInUse,&cdmaMeasObj->
                  cdmaCellInfo, &neighFreqInfo[freqCnt], &cdmaNrUpdtId[idCnt]))
         {
            RLOG0(L_ERROR, "1xCdma cell updation failed");
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to process Cdma Hrpd neighbour 
 * update.
 *
 *  @details
 * Function:wrUmmMeasProcCdmaHrpdNrUpdt
 *
 *   Processing steps:
 *    - get the Object from arfcn and update pci's in respective object
 *    - send to all Ue's if Object is configured.
 *     
 * @param[in]  cellCb: cell control block         
 * @param[in]  cdmaFreq: poiner to WrCDMA2KNeighInfo structure. 
 * @param[in]  cdmaNrUpdtId: poiner to WrCDMA2KNeighInfo structure. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16 wrUmmMeasProcCdmaHrpdNrUpdt
(
 WrUmmCellCb                 *cellCb,
 WrCDMA2KNeighInfo           *cdmaFreq,
 WrMeasCdmaNrUpdtIdNode      *cdmaNrUpdtId 
)
{
   U8                        numBandClass;
   U8                        bandCnt;
   U8                        idCnt = 0;
   WrUmmMeasCellCb           *measCellCb;
   WrCDMA2KBandInfo          *neighBandLst;

   TRC2(wrUmmMeasProcCdmaHrpdNrUpdt)

   measCellCb = cellCb->measCellCb;
   numBandClass = cdmaFreq->numBandClass;
   neighBandLst = cdmaFreq->neighBandLst;

   for (bandCnt = 0; bandCnt < numBandClass; bandCnt++)
   {
      U8                        numFreq;
      U8                        freqCnt;
      U8                        bandClsVal;
      WrCDMA2KFreqInfo          *neighFreqInfo;

      bandClsVal = neighBandLst[bandCnt].bandClass;
      numFreq  = neighBandLst[bandCnt].numFreq;
      neighFreqInfo = neighBandLst[bandCnt].neighFreqInfo;
      cdmaNrUpdtId[idCnt].bandClass = bandClsVal;
      for (freqCnt = 0; freqCnt < numFreq; freqCnt++)
      {
         U32                       arfcn;
         WrUmmMeasCdmaMeasObjNode  *cdmaMeasObj;

         arfcn = neighFreqInfo[freqCnt].arfcn;
         cdmaNrUpdtId[idCnt].arfcn = arfcn;

         if(RFAILED == wrUmmMeasUpdtCellCdmaMeasObjFrmArfcn(bandClsVal, arfcn,
                  &measCellCb->cdmaHrpdMeasObjLst, &cdmaMeasObj))
         {
            RLOG0(L_ERROR, "Cdma Object not found");
            RETVALUE(RFAILED);
         }
         cdmaNrUpdtId[idCnt].cellCbMeasObj = cdmaMeasObj;
         if( RFAILED == wrUmmMeasUpdtCdmaCellsinCellCb(cdmaMeasObj->cdmaCellIdxInUse,&cdmaMeasObj->
                  cdmaCellInfo, &neighFreqInfo[freqCnt], &cdmaNrUpdtId[idCnt]))
         {
            RLOG0(L_ERROR, "Cdma Cells Updation failed");
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for to process Eutra neighbour update. 
 *
 *  @details
 * Function:wrUmmMeasProcNewEutraNrUpdat
 *
 *   Processing steps:
 *    - get the Object from arfcn and update pci's in respective object
 *    - if object is not resent create new object in cell cb
 *    - send to all Ue's if Object is configured.
 *    -if it is new object send to all Ue's
 *     
 * @param[in]  servArfcn: Serving cell carrier frequency         
 * @param[in]  idx: index to reconfig Strcture        
 * @param[in]  cellCb: pointer to the cell Cb         
 * @param[in]  interFreq: poiner to WrNrInterFreqInfo structure. 
 * @param[out] nrFreq: poiner to WrMeasObjFreq structure. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16  wrUmmMeasProcNewEutraNrUpdat 
(
 U32                         servArfcn,
 U8                          *idx,
 WrUmmCellCb                 *cellCb,
 WrNrInterFreqInfo           *interFreq,
 WrMeasObjFreq               *nrFreq
)
{
   U8                        freqCnt;
   U8                        noofFreq;
   WrUmmMeasCellCb           *measCellCb;
   WrNrPerFreqInfo           *perFreqInfo;

   TRC2(wrUmmMeasProcNewEutraNrUpdat)
   
   measCellCb = cellCb->measCellCb;
   noofFreq = interFreq->numFreq;
   for(freqCnt = 0; freqCnt < noofFreq; freqCnt++)
   {
      U8                        idCnt;
      U32                       arfcn;
      WrNrPerFreqWlInfo         *wlCellInfo;
      WrNrPerFreqBlInfo         *blCellInfo;
      WrUmmMeasEutraMeasObjNode *enbMeasObj;

      perFreqInfo = interFreq->freqList;
      arfcn = perFreqInfo[freqCnt].dlCarrierFreq;
      wlCellInfo = &perFreqInfo[freqCnt].wlData;
      blCellInfo = &perFreqInfo[freqCnt].blData;
      nrFreq[*idx].choice = WR_UMM_MEAS_INTER_FREQ; 
      nrFreq[(*idx)++].t.carrierFreq = arfcn;  
      
      if(arfcn != servArfcn)
      {
         if(RFAILED == wrUmmMeasUpdtCellEutraMeasObjFrmArfcn(arfcn, &measCellCb->
             interMeasObjLst, &enbMeasObj))
         {
            RLOG0(L_ERROR, "Eutra measurement object not found");
            RETVALUE(RFAILED);
         }
      }
      else
      {
         enbMeasObj = &measCellCb->intraMeasObj;
      }
      if(NULLP != wlCellInfo)
      {
         U8                        wlCellCnt;
         wlCellCnt = wlCellInfo->numWlCells;
         for(idCnt = 0; idCnt < wlCellCnt; idCnt++)
         {
            U16                   pci;
            U32                   cio;
  
            pci = wlCellInfo->cellInfo[idCnt].phyCellId;
            cio = wlCellInfo->cellInfo[idCnt].cio;
            wrUmmMeasFillEutraWLCellLst(enbMeasObj, pci, cio);
         }
      }
      if(NULLP != blCellInfo)
      {
         U8                        blCellCnt;
         blCellCnt = blCellInfo->numBlInfos;
         for(idCnt = 0; idCnt < blCellCnt; idCnt++)
         {
            U16                   startPci;
            U8                    pciRange;

            startPci = blCellInfo->blInfo[idCnt].startPhyCellID;
            pciRange = blCellInfo->blInfo[idCnt].phyCellIdRange;
            wrUmmMeasFillEutraBLCellLst(enbMeasObj, startPci, pciRange);
         }
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for to process Utra neighbour update. 
 *
 *  @details
 * Function:wrUmmMeasProcNewUtraNrUpdat
 *
 *   Processing steps:
 *    - get the Object from arfcn and update psc's in respective object
 *    - if object is not resent create new object in cell cb
 *    - send to all Ue's if Object is configured.
 *    -if it is new object send to all Ue's
 *     
 * @param[in]  servArfcn: Serving cell carrier frequency         
 * @param[in]  idx: index to reconfig Strcture        
 * @param[in]  cellCb: pointer to the cell Cb         
 * @param[in]  utraFreq: poiner to WrNrInterFreqInfo structure. 
 * @param[out] nrFreq: poiner to WrMeasObjFreq structure. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16  wrUmmMeasProcNewUtraNrUpdat 
(
 U8                          *idx,
 WrUmmCellCb                 *cellCb,
 WrNrUtraFreqInfo            *utran,
 WrMeasObjFreq               *nrFreq,
 WrUmmMeasCellTyp             utraType
)
{
   U8                       freqCnt;
   U8                    noofFreq;
   WrNrUtraPerFreqInfo   *perFreqInfo;

   TRC2(wrUmmMeasProcNewEutraNrUpdat);

   noofFreq = utran->numFreq;
   for(freqCnt = 0; freqCnt < noofFreq; freqCnt++)
   {
      U32				arfcn;
      WrNrUtraPerFreqWlInfo		*wlCellInfo;

      perFreqInfo = utran->freqList;
      arfcn = perFreqInfo[freqCnt].dlCarrierFreq;
      wlCellInfo = &perFreqInfo[freqCnt].wlData;
      nrFreq[*idx].choice = utraType; 
      nrFreq[(*idx)++].t.carrierFreq = arfcn;  
   }

   RETVALUE(ROK);
}


/** @brief This function is responsible for to process Cdma neighbour update. 
 *
 *  @details
 * Function:wrUmmMeasProcNewCdmaNrUpdt
 *
 *   Processing steps:
 *    - get the Object from arfcn and update pci's in respective object
 *    - if object is not resent create new object in cell cb
 *    - send to all Ue's if Object is configured.
 *    -if it is new object send to all Ue's
 *     
 * @param[in]  cellType: cdma Cell type        
 * @param[in]  idx: index to reconfig Strcture        
 * @param[in]  cellCb: pointer to the cell Cb         
 * @param[in]  interFreq: poiner to WrNrInterFreqInfo structure. 
 * @param[out] nrFreq: poiner to WrMeasObjFreq structure. 
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PRIVATE S16  wrUmmMeasProcNewCdmaNrUpdt 
(
 WrUmmMeasCellTyp             cellType,
 U8                          *idx,
 WrUmmCellCb                 *cellCb,
 WrCDMA2KNeighInfo           *cdmaFreq,
 WrMeasObjFreq               *nrFreq
)
{
   U8                        numBandClass;
   U8                        bandCnt;
   WrCDMA2KBandInfo          *neighBandLst;
   WrUmmMeasCellCb           *measCellCb;

   TRC2(wrUmmMeasProcNewCdmaNrUpdt)
   
   measCellCb = cellCb->measCellCb;
   /*Get Number of bandClass and neighCellLst from PBM CB i.e nrUpdtInfo */
   numBandClass = cdmaFreq->numBandClass;
   neighBandLst = cdmaFreq->neighBandLst;

   if(cellType == WR_UMM_MEAS_1XCDMA)
      nrFreq[*idx].choice = WR_UMM_MEAS_1XCDMA; 
   else
      nrFreq[*idx].choice = WR_UMM_MEAS_CDMA_HRPD;
      
   for (bandCnt = 0; bandCnt < numBandClass; bandCnt++)
   {
      U8                        numFreq;
      U8                        freqCnt;
      U8                        bandClsVal;
      WrCDMA2KFreqInfo          *neighFreqInfo;

      bandClsVal = neighBandLst[bandCnt].bandClass;
      numFreq  = neighBandLst[bandCnt].numFreq;
      neighFreqInfo = neighBandLst[bandCnt].neighFreqInfo;
      for (freqCnt = 0; freqCnt < numFreq; freqCnt++)
      {
         U32                       arfcn;
         U8                        numPcis;
         U8                        idCnt;
         WrUmmMeasCdmaMeasObjNode  *cdmaMeasObj;

         arfcn = neighFreqInfo[freqCnt].arfcn;
         nrFreq[*idx].t.measObjCDMA2000.arfcn = arfcn;  
         nrFreq[(*idx)++].t.measObjCDMA2000.bandClass = bandClsVal; 
         if(cellType == WR_UMM_MEAS_1XCDMA)
         {
            if(RFAILED == wrUmmMeasUpdtCellCdmaMeasObjFrmArfcn(bandClsVal, arfcn,
                   &measCellCb->cdma1xMeasObjLst, &cdmaMeasObj))
            {
               RLOG0(L_ERROR, "1xdma Object not found");
               RETVALUE(RFAILED);
            }
         }
         else
         {
            if(RFAILED == wrUmmMeasUpdtCellCdmaMeasObjFrmArfcn(bandClsVal, arfcn,
                   &measCellCb->cdmaHrpdMeasObjLst, &cdmaMeasObj))
            {
               RLOG0(L_ERROR, "cdma Hrpd Object not found");
               RETVALUE(RFAILED);
            }
         }
         numPcis = neighFreqInfo[freqCnt].wlData.numWlCells;
         
         for (idCnt = 0; idCnt < numPcis; idCnt++)
         {
            U16                      pci;
  
            pci = neighFreqInfo[freqCnt].wlData.pciInfoLst[idCnt].pnOffset;
            /*Add this particular PCI in the Measurement object List
             * cdmaCellInfo */
            wrUmmMeasFillCdmaCellsIntoMeaObj(cdmaMeasObj, pci);
         } 
      }
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for filling New Measurement Object
 * Information in cell control block.
 *
 * @details
 *
 *  Function:wrUmmMeasUpdtNrNewFreqCellMeasObj
 * 
 *  Processing steps:
 *    - Fill New measurement Objects in cell Control block.
 *    - Send updated information to all Ue's which is connected to the serving
 *    cell.
 *    - if new frequency measurement object found configure strongest cells.
 * 
 *  @param[in]  cellId: cell Identifier.
 *  @param[in]  nrUpdtInfo: Neighbour update List.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 **/
PRIVATE S16 wrUmmMeasUpdtNrNewFreqCellMeasObj
(
U16                          cellId,
WrNrUpdtInfo                 *nrUpdtInfo
)
{
   U8                        idx = 0;
   U8                        ueCnt;
   U32                       servCarrFreq;
   WrUmmCellCb               *cellCb;
   WrUmmMeasCellCb           *measCellCb;
   WrMeasUeCb                *measUeCb;
   WrMeasCellReCfgInfo       *reCfgInfo;
   WrMeasObjFreq             *nrFreq;
   WrUeCb                    *ueCb = NULLP;
   U8                        totCdmaHrpdObj = 0;
   U8                        tot1xCdmaObj = 0;
   WrUmmInternalMsg          *internalMsg;
   WrUmmIncMsg               *msg;


   TRC2(wrUmmMeasUpdtNrNewFreqCellMeasObj)
   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }
   servCarrFreq = wrEmmGetServCarrierFreq(cellId);

   measCellCb = cellCb->measCellCb;

   WR_ALLOC(&reCfgInfo, sizeof(WrMeasCellReCfgInfo));
   if(NULLP == reCfgInfo)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   else
   {
      reCfgInfo->action = WR_ACT_ADD; 
      nrFreq = reCfgInfo->t.addNeighFreq;
   }
   if(0 != nrUpdtInfo->eutran.numFreq)
   {
      wrUmmMeasProcNewEutraNrUpdat(servCarrFreq, &idx, cellCb, &nrUpdtInfo->eutran, nrFreq); 
   }
   if(0 != nrUpdtInfo->utran.numFreq)
   {
      wrUmmMeasProcNewUtraNrUpdat(&idx, cellCb, &nrUpdtInfo->utran, nrFreq, WR_UMM_MEAS_UTRA_FDD_FREQ); 
   }
   if(0 != nrUpdtInfo->utranTdd.numFreq)
   {
      wrUmmMeasProcNewUtraNrUpdat(&idx, cellCb, &nrUpdtInfo->utranTdd, nrFreq, WR_UMM_MEAS_UTRA_TDD_FREQ); 
   }
   if(0 != tot1xCdmaObj)
   {
      /* Update the new configured frequency in Measurement CB */
      wrUmmMeasProcNewCdmaNrUpdt(WR_UMM_MEAS_1XCDMA, &idx, cellCb, &nrUpdtInfo->cdma1xRtt, nrFreq);      
   }
   if(0 != totCdmaHrpdObj)
   {
      if(RFAILED == wrUmmMeasProcNewCdmaNrUpdt(WR_UMM_MEAS_CDMA_HRPD, &idx, cellCb,
               &nrUpdtInfo->cdmaHrpd, nrFreq))
      {
         RLOG0(L_ERROR, "cdma Hrpd updation Failed");
         WR_FREE(reCfgInfo, sizeof(WrMeasCellReCfgInfo));
         RETVALUE(RFAILED);
      }
   } 
 
   reCfgInfo->numFreq = (nrUpdtInfo->eutran.numFreq) + (nrUpdtInfo->utran.numFreq) +tot1xCdmaObj + totCdmaHrpdObj;

   if (ROK != wrUmmBldInternalMsg(&internalMsg, WR_UMM_INTMSG_NEW_MEAS_TRANS,
            reCfgInfo))
   {
      RLOG0(L_ERROR, "creation of internal message Failed");
      WR_FREE(reCfgInfo, sizeof(WrMeasCellReCfgInfo));
      RETVALUE(RFAILED);
    }
    WR_ALLOC(&msg,sizeof(WrUmmIncMsg));
    if (NULLP == msg)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
       WR_FREE(reCfgInfo, sizeof(WrMeasCellReCfgInfo));
       WR_FREE(internalMsg, sizeof(WrUmmInternalMsg));
       RETVALUE(RFAILED);
    }
    msg->msgTyp = WR_UMM_INTERNAL_MSG;
    msg->u.internalMsg = internalMsg;
    msg->refCnt = 1;

    for(ueCnt = 0; ueCnt < cellCb->maxUesPerCell; ueCnt++)
    {
      ueCb = cellCb->ueCbs[ueCnt];
      if(NULLP != ueCb && msg != NULLP)
      {
         measUeCb = ueCb->measCfg;
         if((NULLP != measUeCb) && (NULLP != measUeCb->interMeasObjLst.count))
         {
            msg->refCnt += 1;
            wrUmmPrcIncMsg(cellCb, ueCb, msg);
            wrUmmRlsIncMsg(&msg);
         }
      }
    }
    if(msg != NULLP)
    {
       wrUmmRlsIncMsg(&msg);
    }
    RETVALUE(ROK);
}

PRIVATE S16 wrProcTotal1xCdmaObj 
( 
    U8 tot1xCdmaObj, 
    WrNrUpdtInfo *nrUpdtInfo,
    WrUmmCellCb  *cellCb,
    WrUmmIntMeasTrans *nrUpdtNode
)
{
   S16   ret = ROK;

   WR_GET_MEM(&nrUpdtNode->memCp, tot1xCdmaObj * sizeof(WrMeasCdmaNrUpdtIdNode),
         (Ptr*)&nrUpdtNode->cdma1xNode);
   if(NULLP == nrUpdtNode->cdma1xNode)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   nrUpdtNode->num1xCdma = tot1xCdmaObj;
   ret = wrUmmMeasProc1xCdmaNrUpdt(cellCb, &nrUpdtInfo->cdma1xRtt, nrUpdtNode->cdma1xNode);
   RETVALUE(ret);
}


/** @brief This function is responsible for filling New Measurement Object
 * Information in cell control block.
 *
 * @details
 *
 *  Function:wrUmmMeasProcNrUpdt 
 * 
 *  Processing steps:
 *    - Fill New measurement Objects in cell Control block.
 *    - Send updated information to all Ue's which is connected to the serving
 *    cell.
 *    - if new frequency measurement object found configure strongest cells.
 * 
 *  @param[in]  cellId: cell Identifier.
 *  @param[in]  isNewFreq: TRUE: new frequency, FALSE: Existing frequency.
 *  @param[in]  nrUpdtInfo: Neighbour update List.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
*/
   
PUBLIC S16 wrUmmMeasProcNrUpdt
(
U16                          cellId,
Bool                         isNewFreq,
WrNrUpdtInfo                 *nrUpdtInfo
)
{
   WrUmmCellCb               *cellCb;
   WrUmmMeasCellCb           *measCellCb;
   U8                        tot1xCdmaObj = 0;
   U8                        totCdmaHrpdObj = 0;
   U8                        nrUpdtIdCnt = 0;
   U8                        bandIndx=0;
   U8                        utraUpdtIdx = 0;

   TRC2(wrUmmMeasProcNrUpdt)
   if(NULLP == nrUpdtInfo)
   {
      RLOG0(L_ERROR, "neighbor updation pointer is NULL");
      RETVALUE(RFAILED);
   }

   /* find total number of 1xRTT object(band, freq) stored in nrUpdtInfo */
   for(bandIndx=0; bandIndx < nrUpdtInfo->cdma1xRtt.numBandClass; bandIndx++)
   {
      tot1xCdmaObj += nrUpdtInfo->cdma1xRtt.neighBandLst[bandIndx].numFreq;
   }

   if((0 == nrUpdtInfo->eutran.numFreq) && 
      (0 == nrUpdtInfo->utran.numFreq)  &&
      (0 == nrUpdtInfo->utranTdd.numFreq) &&
      (0 == tot1xCdmaObj) && 
      (0 == totCdmaHrpdObj)) 
   {
      RETVALUE(ROK);
   }
   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }
   
   measCellCb = cellCb->measCellCb;
   /* In case of addition/configuration of new frequency isNewFreq will be TRUE
    */
   if(TRUE != isNewFreq) 
   {
      WrUmmIntMeasTrans         *nrUpdtNode;

      /* WR_SZ_MEM_SDU_SIZE is 2048B, this is not relavent macro name but using it
       * rather than passing 2048 hardcoded value */
      if (ROK != cmAllocEvnt(sizeof(WrUmmIntMeasTrans), WR_SZ_MEM_SDU_SIZE, &wrCb.mem,
               (Ptr *)&nrUpdtNode))
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }/* End of if condition.*/ 

      /* EUTRA Neighbors */
      if(0 != nrUpdtInfo->eutran.numFreq)
      {
         WR_GET_MEM(&nrUpdtNode->memCp, nrUpdtInfo->eutran.numFreq * sizeof(WrMeasEutraNrUpdtIdNode),
               (Ptr*)&nrUpdtNode->eutraNode);
         if(NULLP == nrUpdtNode->eutraNode)
         {
            WR_FREE_EVNT(nrUpdtNode);
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         nrUpdtNode->numEutra = nrUpdtInfo->eutran.numFreq;
         /* The following function should take care of HRL and non-HRL cases
          * similarly for other RATs respect NrUpdat Functions should take care
          * as of only EUTRA & HRL combination is supported  */
         wrUmmMeasProcEutraNrUpdat(cellId, &nrUpdtInfo->eutran, nrUpdtNode->eutraNode);
      }

      /* UTRA Neighbors */
      if((0 != nrUpdtInfo->utran.numFreq) || (0 != nrUpdtInfo->utranTdd.numFreq))
      {

         WR_GET_MEM(&nrUpdtNode->memCp,(nrUpdtInfo->utran.numFreq + nrUpdtInfo->utranTdd.numFreq) \
               * sizeof(WrMeasUtraNrUpdtIdNode), (Ptr*)&nrUpdtNode->utraNode);
         if(NULLP == nrUpdtNode->utraNode)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");

            WR_FREE_EVNT(nrUpdtNode);

            RETVALUE(RFAILED);
         }
         nrUpdtNode->numUtra = (nrUpdtInfo->utran.numFreq + nrUpdtInfo->utranTdd.numFreq);
         if(0 != nrUpdtInfo->utran.numFreq)
         {
            wrUmmMeasProcUtraNrUpdat(cellId, &nrUpdtInfo->utran, nrUpdtNode->utraNode, &utraUpdtIdx, WR_UMM_MEAS_UTRA_FDD_FREQ);
         }
         if(0 != nrUpdtInfo->utranTdd.numFreq)
         {
            wrUmmMeasProcUtraNrUpdat(cellId, &nrUpdtInfo->utranTdd, nrUpdtNode->utraNode, &utraUpdtIdx, WR_UMM_MEAS_UTRA_TDD_FREQ);
         }
      }
      /* CDMA 1xRTT Neighbors */
      if(0 != tot1xCdmaObj)
      {
         if(ROK != wrProcTotal1xCdmaObj(tot1xCdmaObj, nrUpdtInfo, cellCb, nrUpdtNode))
         {
            WR_FREE_EVNT(nrUpdtNode);
            RETVALUE(RFAILED);
         }
      }
      totCdmaHrpdObj = wrUmmMeasGetCdmaFreqCnt(&nrUpdtIdCnt, &nrUpdtInfo->cdmaHrpd);
      if(0 != totCdmaHrpdObj)
      {
         WR_GET_MEM(&nrUpdtNode->memCp,totCdmaHrpdObj * sizeof(WrMeasCdmaNrUpdtIdNode),
               (Ptr*)&nrUpdtNode->cdmaHrpdNode);
         if(NULLP == nrUpdtNode->cdmaHrpdNode)
         {
            WR_FREE_EVNT(nrUpdtNode);
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         nrUpdtNode->numCdmaHrpd = totCdmaHrpdObj;
         if(RFAILED == wrUmmMeasProcCdmaHrpdNrUpdt(cellCb, &nrUpdtInfo->cdmaHrpd,
                  nrUpdtNode->cdmaHrpdNode))
         {
            RLOG0(L_ERROR, "cdma Object Updation Failed");
            WR_FREE_EVNT(nrUpdtNode);
            RETVALUE(RFAILED);
         }
      }

      /* DYNAMIC_OAM and HRL interaction changes -start */
      /* Pass the nrUpdtInfo to wrUmmMeasUpdtIntTransMsg to update the HRL
       * UeCb - for newly added/modified/deleted neighbor cell information */

      /*sending Updated  Nr Info to all Ue */
      wrUmmMeasUpdtIntTransMsg(cellCb, nrUpdtNode, nrUpdtInfo);
      /* DYNAMIC_OAM and HRL interaction changes -end */
      WR_FREE_EVNT(nrUpdtNode);
   }
   else
   {
      wrUmmMeasUpdtNrNewFreqCellMeasObj(cellId, nrUpdtInfo);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for Processing RRC message
 *
 * @details
 *
 *  Function:wrUmmMeasPrcRrcMsg 
 * 
 *  Processing steps:
 *    - Process RRC Message
 *     
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasPrcRrcMsg
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

   c1 = &ulDcch->dcchMsg.message.val.c1;
   switch(c1->choice.val)
   {
      case C1_MEASUREMENTREPORT:
      {
         measRpt = &c1->val.measurementReport;
         measRptC1 = &measRpt->criticalExtns.val.c1;
         measRes = &measRptC1->val.measurementReport_r8.measResults;
         RLOG_ARG0(L_EVENT, DBG_CRNTI,transCb->ueCb->crnti,
            "[OTA] Received RRC MeasurementReport");
         wrUmmMeasHdlMeasRpt(transCb, measRes);
         break;
      }
      case C1_RRCCONRECFGNCOMPL:
      {
         RLOG_ARG0(L_INFO, DBG_CRNTI,transCb->ueCb->crnti,
            "Received C1_RRCCONRECFGNCOMPL");
         wrUmmMeasCfgReCfgReqCompHand(WR_UMM_MEAS_TYPE_HO, transCb);
         rrcTransId = c1->val.rrcConRecfgnCompl.rrc_TrnsxnId.val;
         ret = wrUmmRelRrcTransId(transCb->ueCb, C1_RRCCONRECFGN, rrcTransId);
         if (ret != ROK)
         {
            /*ccpu00127802*/
            RLOG0(L_ERROR, "Release of transaction ID failed");
         }

         if(transCb->u.measTransCb.measGapStatus == WR_UMM_MEAS_GAP_REL)
         {
            RLOG0(L_DEBUG, "Release the MeasGap in scheduler");

            /* RRC Reconfiguration complete Received for
               meas gap release. */

            /* If this Tx is the latest Tx accessing the MeasGap 
              then send message to SCH */
            if (transCb->ueCb->measCfg->gapMeasInfo.latestTxId == transCb->transId) 
            {
               wrUmmMeasGapSchdUeRecfgReq(transCb);
            }
            else 
            {
               wrUmmTransComplete(transCb);
            }
         }
         else if (transCb->u.measTransCb.measGapStatus == WR_UMM_MEAS_GAP_INIT_UE_CONFIG)
         {
            /* If this Tx is the latest Tx accessing the MeasGap 
              then Reinitialise the latest Tx Id*/
            if (transCb->ueCb->measCfg->gapMeasInfo.latestTxId == transCb->transId) 
            {
               /* Re-initialize the latest Tx Id */
               transCb->ueCb->measCfg->gapMeasInfo.latestTxId = 0;

               /* Update the measGapStatus to Configued */
               transCb->ueCb->measCfg->gapMeasInfo.isMeasGapCfg = WR_UMM_MEAS_GAP_CONFIGURED;
            }
            wrUmmTransComplete(transCb);
         }
         else
         {
            /* When GAP is not configured this will release Meas Trans */
            wrUmmTransComplete(transCb);
         }
         break;
      }
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for Processing internal message
 *
 * @details
 *
 *  Function:wrUmmMeasSendNewFreqNrUpdtInfoToUe
 * 
 *  Processing steps:
 *    - Process measurement configaration
 * 
 *  @param[in]  cellCb: pointer to the cell control block.
 *  @param[in]  ueCb: pointer to the Ue control block.
 *  @param[in]  measTransCb: pointer to the Meas trans message.
 *  @param[in]  numFreq: Number of frequency.
 *  @param[in]  nrFreq: pointer to the WrMeasObjFreq object.
 *  @param[in]  rrcReCfgMsg: pointer to the reconfig message.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasSendNewFreqNrUpdtInfoToUe
(
   WrUmmCellCb                  *cellCb,
   WrUeCb                       *ueCb,
   WrUmmTransCb                 *transCb,
   U8                           numFreq,
   WrMeasObjFreq                *nrFreq
)
{
   U8                          idCnt;
   U8                          measObjId;
   WrUmmMeasCellCb             *measCellCb;
   WrMeasUeCb                  *measUeCb;
   LwrCellMeasCfgGrp           *measCellCfg;
   WrUmmMeasRptCfgInfo         rptCfgInfo;
   WrUmmMeasTransInfo           *measTransCb = &transCb->u.measTransCb;

   TRC2(wrUmmMeasSendNewFreqNrUpdtInfoToUe)

   measCellCb = cellCb->measCellCb;
   measCellCfg = cellCb->measCellCfg;
   measUeCb = ueCb->measCfg; 

   for(idCnt = 0; idCnt < numFreq; idCnt++)
   {
      if(nrFreq[idCnt].choice == WR_UMM_MEAS_INTER_FREQ)
      {
         U32                    arfcn;

         WrUmmMeasEutraMeasObjNode   *eutraCellObj;
         arfcn = nrFreq[idCnt].t.carrierFreq;

         /*Meas Objects for interRAT frequencies*/
         if(RFAILED == wrUmmMeasUpdtCellEutraMeasObjFrmArfcn(arfcn, &measCellCb->
                  interMeasObjLst, &eutraCellObj))
         {
            RETVALUE(RFAILED);
         }

         measObjId = wrUmmMeasAddInterMeasObjToUeCb(measUeCb, eutraCellObj, 0xFFFF);
         if(measObjId != WR_UMM_MEAS_INVALID_MEAS_ID)
         {
            measTransCb->addMeasInfo.measObjLst[measTransCb->addMeasInfo.
               measObjIdCnt] = measObjId;
            measTransCb->addMeasInfo.measObjIdCnt++;
         }
         else
         { 
            RLOG0(L_ERROR, "Sending of New Freq add Failed");
            RETVALUE(RFAILED);
         }

         /*report config for Strongest Cells*/
         rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_STRONG_CELL;
         rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;
         rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
         rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

         if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_HO, measObjId,
                  &rptCfgInfo, ueCb, measCellCfg, &measTransCb->addMeasInfo))
         {
            RLOG0(L_ERROR, "Invalid report and Meas config Id");
            RETVALUE(RFAILED);
         }
      }
      else
      {
         U8                     bandClass;
         U32                    arfcn;

         WrUmmMeasCdmaMeasObjNode    *cdmaMeasObj;
         bandClass = nrFreq[idCnt].t.measObjCDMA2000.bandClass;
         arfcn = nrFreq[idCnt].t.measObjCDMA2000.arfcn;

         if(nrFreq[idCnt].choice == WR_UMM_MEAS_1XCDMA) 
         {
            /*Meas Objects for interRAT frequencies*/
            if(RFAILED == wrUmmMeasUpdtCellCdmaMeasObjFrmArfcn(bandClass, arfcn, 
                                                               &measCellCb->cdma1xMeasObjLst,
                                                               &cdmaMeasObj))
            {         
               RETVALUE(RFAILED);
            }
         }
         else
         {
            /*Meas Objects for interRAT frequencies*/
            if(RFAILED == wrUmmMeasUpdtCellCdmaMeasObjFrmArfcn(bandClass, arfcn, 
                                               &measCellCb->cdmaHrpdMeasObjLst,
                                                               &cdmaMeasObj))
            {
               RETVALUE(RFAILED);
            }
         }

         /*Meas Objects for inter frequencies*/
         measObjId = wrUmmMeasAddInterRatMeasObjToUeCb(measUeCb, cdmaMeasObj);
         if(measObjId != WR_UMM_MEAS_INVALID_MEAS_ID)
         {
            measTransCb->addMeasInfo.measObjLst[measTransCb->addMeasInfo.
               measObjIdCnt] = measObjId;
            measTransCb->addMeasInfo.measObjIdCnt++;
         }
         else
         { 
            RLOG0(L_ERROR, "Sending of New Freq add Failed");
            RETVALUE(RFAILED);
         }

         /* Report Config for Strongest cells for ANR */
         rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_STRONG_CELL_ANR;
         rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRES_TYPE_NONE;
         rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CDMA2K_1XRTT;
         rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_NONE;

         if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_HO, measObjId,
                  &rptCfgInfo, ueCb, measCellCfg, &measTransCb->addMeasInfo))
         {
            RLOG0(L_ERROR, "Invalid report and Meas config Id");
            RETVALUE(RFAILED);
         }
      }
   }

   /*Prepare and send the RRC Reconfiguration message */
   if(ROK != wrUmmMeasPrepareSendRrcRecfg(ueCb->cellId, 
                                 ueCb, measTransCb, 
                                 transCb->transId))
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for Processing internal message
 *
 * @details
 *
 *  Function:wrUmmMeasSendNewModNrUpdtInfoToUe
 * 
 *  Processing steps:
 *    - Process measurement configaration
 * 
 *  @param[in]  cellCb: pointer to the cell control block.
 *  @param[in]  ueCb: pointer to the Ue control block.
 *  @param[in]  measTransCb: pointer to the Meas trans message.
 *  @param[in]  numFreq: number of frequency.
 *  @param[in]  nrFreq: pointer to the WrMeasObjFreq object.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasSendNewModNrUpdtInfoToUe
(
   WrUmmCellCb                  *cellCb,
   WrUeCb                       *ueCb,
   WrUmmTransCb                 *transCb,
   U8                            numFreq,
   WrMeasObjFreq                *nrFreq
)
{
   WrUmmMeasCellCb             *measCellCb;
   U8                          measObjId = 0;
   U8                          idCnt;
   LwrCellMeasCfgGrp           *measCellCfg;
   WrUmmMeasAddInfo            *addMeasInfo;
   WrUmmMeasTransInfo           *measTransCb = &transCb->u.measTransCb;
   TRC2(wrUmmMeasSendNewModNrUpdtInfoToUe)

   measCellCb = cellCb->measCellCb;
   measCellCfg = cellCb->measCellCfg;
   addMeasInfo = &measTransCb->addMeasInfo;
   for(idCnt = 0; idCnt < numFreq; idCnt++)
   {
      if(nrFreq[idCnt].choice == WR_UMM_MEAS_INTER_FREQ)
      {
         U32                    arfcn;

         WrUmmMeasEutraMeasObjNode   *eutraCellObj;
         arfcn = nrFreq[idCnt].t.carrierFreq;

         /*Meas Objects for interRAT frequencies*/
         if(RFAILED == wrUmmMeasUpdtEutraMeasObjAndIdFromArfcn(arfcn, ueCb->
                  measCfg, &measObjId, &eutraCellObj))
         {
            RETVALUE(RFAILED);
         }
         addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
         addMeasInfo->measObjIdCnt++;
      }
      else
      {
         U8                     bandClass;
         U32                    arfcn;

         WrUmmMeasCdmaMeasObjNode    *cdmaMeasObj;
         bandClass = nrFreq[idCnt].t.measObjCDMA2000.bandClass;
         arfcn = nrFreq[idCnt].t.measObjCDMA2000.arfcn;

         if(nrFreq[idCnt].choice == WR_UMM_MEAS_1XCDMA) 
         {
            /*Meas Objects for interRAT frequencies*/
            if(RFAILED == wrUmmMeasUpdtCdmaMeasObjAndIdFromArfcn(bandClass, arfcn,
                     ueCb->measCfg, &measObjId, &cdmaMeasObj))
            {      
               RETVALUE(RFAILED);
            }
         }
         else
         {
            /*Meas Objects for interRAT frequencies*/
            if(RFAILED == wrUmmMeasUpdtCdmaMeasObjAndIdFromArfcn(bandClass, arfcn,
                     ueCb->measCfg, &measObjId, &cdmaMeasObj))
            {
               RETVALUE(RFAILED);
            }
         }

         /*Meas Objects for inter frequencies*/
         addMeasInfo->measObjLst[addMeasInfo->measObjIdCnt] = measObjId;
         addMeasInfo->measObjIdCnt++;
      }
   }
   measTransCb->addMeasInfo.isCellsToBeSkip = TRUE;

   /*Prepare and send the RRC Reconfiguration message */
   if(ROK != wrUmmMeasPrepareSendRrcRecfg(ueCb->cellId, 
                                 ueCb, measTransCb, 
                                 transCb->transId))
   {
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
/** @brief This function is responsible for Processing Deletion information
 *    to Ue 
 * @details
 *
 *  Function:wrUmmMeasSendDelNrUpdtInfoToUe
 * 
 *  Processing steps:
 *    - Process deletion measurement configuration information
 * 
 *  @param[in]  cellCb: pointer to the cell CB.
 *  @param[in]  ueCb: pointer to the Ue CB.
 *  @param[in]  measTransCb: pointer to the measurement Transaction CB.
 *  @param[in]  numMeasObjId: total number of measurement Object Id.
 *  @param[in]  measObjId: pointer to the measurement Object Id
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasSendDelNrUpdtInfoToUe
(
 WrUmmCellCb                 *cellCb,
 WrUeCb                      *ueCb,
 WrUmmTransCb                *transCb,
 U8                          numMeasObjId,
 U8                          *measObjId
)
{
   U8                          idCnt;
   WrUmmMeasTransInfo         *measTransCb = &transCb->u.measTransCb;

   TRC2(wrUmmMeasSendDelNrUpdtInfoToUe)

   measTransCb->rmvMeasInfo.measObjIdRmvLstCnt = numMeasObjId;
   for(idCnt = 0; idCnt < numMeasObjId; idCnt++)
   {
      measTransCb->rmvMeasInfo.measObjIdRmvLst[idCnt] = measObjId[idCnt];
   }

   /*Prepare and send the RRC Reconfiguration message */
   if(ROK != wrUmmMeasPrepareSendRrcRecfg(ueCb->cellId, 
                                          ueCb, measTransCb, 
                                          transCb->transId))
   {
     RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}
/** @brief This function is responsible for Processing internal message
 *
 * @details
 *
 *  Function:wrUmmMeasPrcIntMsg 
 * 
 *  Processing steps:
 *    - Process measurement configaration
 * 
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasPrcIntMsg
(
WrUmmTransCb                 *transCb
)
{
   WrUmmCellCb                  *cellCb;
   WrUeCb                       *ueCb;
   WrUmmIncMsg                  *msg = transCb->msg;
   U16                          cellId;
   U8                           idx = 0;
   U8                           rmvIdx = 0;
   NhuDatReqSdus                *rrcReCfgMsg;
   NhuMeasConfig                *nhuMeasCfg;
   S16                          ret = RFAILED;

   TRC2(wrUmmMeasPrcIntMsg)

   ueCb   = transCb->ueCb;
   cellId = ueCb->cellId;

   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }

   if(msg->u.internalMsg->intMsgType == WR_UMM_INTMSG_MEAS_TRANS)
   {
      U8                           totObj = 0;
      U8                           totRmvObj = 0;
      WrUmmIntMeasTrans            *measTrans;
      measTrans = msg->u.internalMsg->u.measTrans;

      if(ROK == wrUmmMeasAllocRecfgMsg(&rrcReCfgMsg, 
               &transCb->u.measTransCb, 
               transCb->ueCb, 
               transCb->transId))
      {
         wrUmmMeasGetNhuMeasCfgFrmReCfg(rrcReCfgMsg, &nhuMeasCfg);
         nhuMeasCfg->pres.pres = PRSNT_NODEF;

         totObj = measTrans->numEutra + measTrans->num1xCdma +measTrans->numCdmaHrpd + measTrans->numUtra;
         /* say a neighbor cell arfcn modified from intra to inter frequency but
          * inter frequency measurement object(s) not configured yet to this UE
          * now, measTrans->numEutra will be two. so noComp will be two but
          * for intra frequency measurement object cell list will be there for
          * the other frequency cell list will be empty */
            if(ROK != cmGetMem(&rrcReCfgMsg->memCp, totObj * sizeof(NhuMeasObjectToAddMod), 
                     (Ptr *)&nhuMeasCfg->measObjectToAddModLst.member))
            {

               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }

         if(measTrans->numEutra != 0)
         {
            ret = wrUmmMeasSendEutraUpdtNrInfoToUe(&rrcReCfgMsg->memCp, &idx, cellCb,
                  ueCb->measCfg, measTrans->numEutra, measTrans->eutraNode, nhuMeasCfg);
         }
         if(measTrans->num1xCdma != 0)
         {
            ret = wrUmmMeasSend1xCdmaUpdtNrInfoToUe(&rrcReCfgMsg->memCp, &idx, cellCb->measCellCb,
                  ueCb->measCfg, measTrans->num1xCdma, measTrans->cdma1xNode, nhuMeasCfg);
         }
         if(measTrans->numCdmaHrpd != 0)
         {
            ret = wrUmmMeasSendCdmaHrpdUpdtNrInfoToUe(&rrcReCfgMsg->memCp, &idx, cellCb->measCellCb,
                  ueCb->measCfg, measTrans->num1xCdma, measTrans->cdma1xNode, nhuMeasCfg);
         }
         if(measTrans->numUtra != 0)
         {
            ret = wrUmmMeasSendUtraUpdtNrInfoToUe(&rrcReCfgMsg->memCp, &idx, cellCb->measCellCb,
                  ueCb->measCfg, measTrans->numUtra, measTrans->utraNode, nhuMeasCfg);
         }
         /*fix for ccpu00148771*/
         if ( 0 != idx )                                                
         {
         WR_FILL_TKN_UINT(nhuMeasCfg->measObjectToAddModLst.noComp, idx);
         }

         totRmvObj = measTrans->numUtraDel + measTrans->numCdmaDel;
         if(totRmvObj != 0)
         {
            WR_FILL_TKN_UINT(nhuMeasCfg->measObjectToRemoveLst.noComp, totRmvObj)
               if(ROK != cmGetMem(&rrcReCfgMsg->memCp, totRmvObj * sizeof(NhuMeasObjectId), 
                        (Ptr *)&nhuMeasCfg->measObjectToRemoveLst.member))
               {

                  RLOG0(L_FATAL, "Memory allocation failed.");
                  RETVALUE(RFAILED);
               }
            if(measTrans->numUtraDel != 0)
            {
               ret = wrUmmMeasSendUtraDelNrInfoToUe(&rmvIdx, ueCb->measCfg, measTrans->numUtraDel, 
                     measTrans->utraNodeDel, nhuMeasCfg);
            }
            if(measTrans->numCdmaDel != 0)
            {
               ret = wrUmmMeasSendCdmaDelNrInfoToUe(&rmvIdx, ueCb->measCfg, measTrans->numCdmaDel,
                     measTrans->cdma1xDelNode, nhuMeasCfg);
            }
         }
         /* Need to check both the Idx values */
         if((idx != 0) || (rmvIdx != 0))
         {
            RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending RRCConnectionReconfiguration"
                  " for Updated EUTRA Node List");
            wrIfmRrcSndReqToUe (rrcReCfgMsg);
         }
         else
         {
            WR_FREE_EVNT(rrcReCfgMsg);   
         }
      }
   }
   else
   {
      WrMeasCellReCfgInfo       *reCfgInfo;
      reCfgInfo = msg->u.internalMsg->u.measNewTrans;
      if(reCfgInfo->numFreq <= MAX_NUM_FREQ_CFG)/* KW Fix */
      {
         if(reCfgInfo->action == WR_ACT_ADD)
         {
            ret = wrUmmMeasSendNewFreqNrUpdtInfoToUe(cellCb, ueCb, transCb, 
                  reCfgInfo->numFreq, 
                  reCfgInfo->t.addNeighFreq); 
         }
         else if(reCfgInfo->action == WR_ACT_MOD)
         {
            ret = wrUmmMeasSendNewModNrUpdtInfoToUe(cellCb, ueCb, transCb,
                  reCfgInfo->numFreq,
                  reCfgInfo->t.modNeighFreq);
         }
         else
         {
            ret = wrUmmMeasSendDelNrUpdtInfoToUe(cellCb, ueCb, transCb, 
                  reCfgInfo->numFreq, 
                  reCfgInfo->t.measObjIds); 
         }
      }
      else
      {

         RLOG0(L_ERROR, "reCfgInfo->numFreq is greater than MAX_NUM_FREQ_CFG");
         RETVALUE(RFAILED);

      }
   }

   RETVALUE(ret);
}

/** @brief This function is responsible for receive Measurement related messages
 * which is received from UE.
 *
 * @details
 *
 *  Function:wrUmmMeasProcessor 
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

PUBLIC S16 wrUmmMeasProcessor
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg       = transCb->msg;
   

   switch(msg->msgTyp)
   {
      case WR_UMM_UU_IND_PDU:
      {
         wrUmmMeasPrcRrcMsg(transCb);
         break;
      }
      case WR_UMM_INTERNAL_MSG:
      {
         if(ROK != wrUmmMeasPrcIntMsg(transCb))
         {
            wrUmmTransComplete(transCb);
         }
         break;
      }
      /*ccpu00131966  */
      case WR_UMM_SCHD_CFG:
      {
         if (transCb->u.measTransCb.measGapStatus ==
                                                   WR_UMM_MEAS_GAP_REL)
         {
            RLOG0(L_DEBUG, "Received Scd Cfm for MeasGap rel");
                  
            transCb->u.measTransCb.measGapStatus = WR_UMM_MEAS_GAP_NONE;

            if (transCb->ueCb->measCfg->gapMeasInfo.latestTxId == transCb->transId) 
            {
               transCb->ueCb->measCfg->gapMeasInfo.latestTxId = 0; 
            }
            wrUmmTransComplete(transCb);
         }
         else if (transCb->u.measTransCb.measGapStatus ==
                                             WR_UMM_MEAS_GAP_INIT_SCH_CONFIG)
         {
            RLOG0(L_DEBUG, " Received Schd Gap Setup Cfm ");
#ifdef WR_RSYS_KPI 
               /* Schd Cfm for MeasGap is received */
            transCb->ueCb->measGapUsd = TRUE;
#endif /*WR_RSYS_KPI*/

            /* Change the measGapStatus to UE Config */
            transCb->u.measTransCb.measGapStatus =
                                               WR_UMM_MEAS_GAP_INIT_UE_CONFIG;

            /*Prepare and send the RRC Reconfiguration message */
            if(ROK != wrUmmMeasPrepareSendRrcRecfg(transCb->ueCb->cellId, 
                                                   transCb->ueCb, 
                                                   &transCb->u.measTransCb, 
                                                   transCb->transId))
            {
               /* Failed to send the message to the UE. 
               So close the transaction and return failure */
               wrUmmTransComplete(transCb);
               RETVALUE(RFAILED);
            }
         }
         break;
      }
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for deallocate transaction related
 * information after transaction complete.
 *
 * @details
 *
 *  Function:wrUmmMeasTransRel 
 * 
 *  Processing steps:
 *    - Deallocates transaction information.
 * 
 *  @param[in]  transCb: pointer to the Transaction control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */

PUBLIC S16 wrUmmMeasTransRel
(
WrUmmTransCb *transCb
)
{
   TRC2(wrUmmMeasTransRel)

   RETVALUE(ROK);
}


/** @brief This function is responsible for updating the modification for Eutra
 *  cell reconfiguration. 
 *
 * @details
 *
 *  Function:wrUmmMeasModEutraMeasObjInCellCb
 * 
 *  Processing steps:
 *    - Update cell reconfiguration in cellCb.
 * 
 * @param[out] cellCb: pointer to Cell Cb         
 * @param[in]  eutraCfgParams: pointer to Eutra config Params 
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasModEutraMeasObjInCellCb
(
 WrUmmCellCb                 *cellCb,
 LwrNeighEutraFreq           *eutraCfgParams
)
{
   WrUmmMeasEutraMeasObjNode   *cellCbMeasObj = NULLP;
   U32                         arfcn; 

   TRC2(wrUmmMeasModEutraMeasObjInCellCb)

   arfcn = eutraCfgParams->earfcnDl; 
   wrUmmMeasUpdtCellEutraMeasObjFromArfcn(arfcn, cellCb->measCellCb,
         &cellCbMeasObj);
   cellCbMeasObj->allowedMeasBW = eutraCfgParams->measBandwidth.val;
   cellCbMeasObj->presAntPort1 = eutraCfgParams->presAntennaPort1;
   cellCbMeasObj->neighCellCfg = eutraCfgParams->neighCellCfg;
   cellCbMeasObj->qOffsetFreq = eutraCfgParams->offsetFreq;
   RETVALUE(ROK);
}

/** @brief This function is responsible for deletion for Cdma Measurement 
 *  Objects in Ue cb. 
 *
 * @details
 *
 *  Function:wrUmmMeasDelCdmaMeasObjInUeCb
 * 
 *  Processing steps:
 *    - delete Measurement object in UE Cb.
 * 
 * @param[out] interRatMeasObjLst: pointer to CmLListCb         
 * @param[in]  cdmaCfgParams: pointer to LwrNeighCdmaDelInfo object 
 *  @return S16
 *       SUCCESS : measObjId
 *       FAILED  : RFAILED
 */
PRIVATE U8 wrUmmMeasDelCdmaMeasObjInUeCb
(
 CmLListCp                   *interRatMeasObjLst,
 LwrNeighCdmaDelInfo         *cdmaCfgParams
)
{
   U32                       arfcn;
   U8                        bandClass;
   U8                        measObjId = 0;
   CmLList                       *measObjCdmaNode;
   WrUmmMeasCdmaMeasObjNode      *cellCbEMeasObj;
   WrMeasUeCbCdmaMeasObjNode     *ueCbEMeasObj;

   TRC2(wrUmmMeasDelCdmaMeasObjInUeCb)
   arfcn = cdmaCfgParams->arfcn;
   bandClass = cdmaCfgParams->bandClass;

   measObjCdmaNode = interRatMeasObjLst->first;
   while (NULLP != measObjCdmaNode)
   {
      ueCbEMeasObj = ((WrMeasUeCbCdmaMeasObjNode*)measObjCdmaNode->node);
      cellCbEMeasObj = ueCbEMeasObj->cdmaMeasObj;
      if ((arfcn == cellCbEMeasObj->arfcn) &&
          (bandClass == cellCbEMeasObj->bandClass))
      {
         measObjId = ueCbEMeasObj->measObjId;
         cmLListDelFrm(interRatMeasObjLst, measObjCdmaNode);
         WR_FREE(ueCbEMeasObj, sizeof(WrMeasUeCbCdmaMeasObjNode));
      }
      measObjCdmaNode = interRatMeasObjLst->first;
   }
   RETVALUE(measObjId);
}
/** @brief This function is responsible for deletion for Eutra Measurement 
 *  Objects in Ue cb. 
 *
 * @details
 *
 *  Function:wrUmmMeasDelEutraMeasObjInUeCb
 * 
 *  Processing steps:
 *    - delete Measurement object in UE Cb.
 * 
 * @param[out] interMeasObjLst: pointer to CmListCp         
 * @param[in]  arfcn: Arfcn 
 *  @return S16
 *       SUCCESS : MeasObjId
 *       FAILED  : RFAILED
 */
PRIVATE U8 wrUmmMeasDelEutraMeasObjInUeCb
(
 CmLListCp                   *interMeasObjLst,
 U16                          arfcn
)
{
   U8                        measObjId = 0;
   CmLList                       *measObjEutraNode;
   WrUmmMeasEutraMeasObjNode     *cellCbEMeasObj;
   WrMeasUeCbEutraMeasObjNode    *ueCbEMeasObj;
   
   TRC2(wrUmmMeasDelEutraMeasObjInUeCb)

   measObjEutraNode = interMeasObjLst->first;
   while (NULLP != measObjEutraNode)
   {
      ueCbEMeasObj = ((WrMeasUeCbEutraMeasObjNode*)measObjEutraNode->node);
      cellCbEMeasObj = ueCbEMeasObj->eutraMeasObj;
      if (arfcn == cellCbEMeasObj->carrFreq)
      {
         measObjId = ueCbEMeasObj->measObjId;
         cmLListDelFrm(interMeasObjLst, measObjEutraNode);
         WR_FREE(ueCbEMeasObj, sizeof(WrMeasUeCbEutraMeasObjNode));
         break;
      }
      measObjEutraNode = measObjEutraNode->next;
   }
   RETVALUE(measObjId);
}

/** @brief This function is responsible for deletion for Utra Measurement 
 *  Objects in Ue cb. 
 *
 * @details
 *
 *  Function:wrUmmMeasDelUtraMeasObjInUeCb
 * 
 *  Processing steps:
 *    - delete Measurement object in UE Cb.
 * 
 * @param[out] utraMeasObjLst: pointer to CmListCp         
 * @param[in]  arfcn: Arfcn 
 *  @return S16
 *       SUCCESS : MeasObjId
 *       FAILED  : RFAILED
 */
PRIVATE U8 wrUmmMeasDelUtraMeasObjInUeCb
(
 CmLListCp                   *utraMeasObjLst,
 U16                          arfcn
)
{
   U8                        measObjId = 0;

   CmLList                       *measObjUtraNode;
   WrUmmMeasUtraMeasObjNode      *cellCbUMeasObj;
   WrMeasUeCbUtraMeasObjNode     *ueCbUMeasObj;
   
   TRC2(wrUmmMeasDelUtraMeasObjInUeCb)

   measObjUtraNode = utraMeasObjLst->first;
   while (NULLP != measObjUtraNode)
   {
      ueCbUMeasObj = ((WrMeasUeCbUtraMeasObjNode*)measObjUtraNode->node);
      cellCbUMeasObj = ueCbUMeasObj->utraMeasObj;
      if (arfcn == cellCbUMeasObj->arfcn)
      {
         measObjId = ueCbUMeasObj->measObjId;
         cmLListDelFrm(utraMeasObjLst, measObjUtraNode);
         WR_FREE(ueCbUMeasObj, sizeof(WrMeasUeCbUtraMeasObjNode));
         break;
      }
      measObjUtraNode = measObjUtraNode->next;
   }
   RETVALUE(measObjId);
}


/** @brief This function is responsible for updating the deletion for Cdma
 *  cell reconfiguration. 
 *
 * @details
 *
 *  Function:wrUmmMeasDelCdmaMeasObjInCellCb
 * 
 *  Processing steps:
 *    - Update delete cell reconfiguration in cellCb.
 * 
 * @param[out] cellCb: pointer to Cell Cb         
 * @param[in]  cdmaCfgParams: pointer to Cdma config Params 
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasDelCdmaMeasObjInCellCb
(
 CmLListCp                   *cdmaLListCp,
 LwrNeighCdmaDelInfo         *cdmaCfgParams
)
{
   WrUmmMeasCdmaMeasObjNode      *cellCbMeasObj;
   CmLList                       *measObjCdmaNode;

   TRC2(wrUmmMeasDelCdmaMeasObjInCellCb)

   measObjCdmaNode = cdmaLListCp->first;
   while (NULLP != measObjCdmaNode)
   {
      cellCbMeasObj = ((WrUmmMeasCdmaMeasObjNode*)measObjCdmaNode->node);
      if ((cdmaCfgParams->arfcn == cellCbMeasObj->arfcn) &&
         (cdmaCfgParams->bandClass == cellCbMeasObj->bandClass))
      {
         cmLListDelFrm (cdmaLListCp, measObjCdmaNode);
         WR_FREE(cellCbMeasObj, sizeof(WrUmmMeasCdmaMeasObjNode));
         break;
      }
      measObjCdmaNode = measObjCdmaNode->next;
   }
  
   RETVALUE(ROK);
}
/** @brief This function is responsible for updating the deletion for Eutra
 *  cell reconfiguration. 
 *
 * @details
 *
 *  Function:wrUmmMeasDelEutraMeasObjInCellCb
 * 
 *  Processing steps:
 *    - Update cell reconfiguration in cellCb.
 * 
 * @param[out] cellCb: pointer to Cell Cb         
 * @param[in]  eutraCfgParams: pointer to Eutra config Params 
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasDelEutraMeasObjInCellCb
(
 WrUmmMeasCellCb              *measCellCb,
 LwrNeighEutraDelInfo         *eutraCfgParams
)
{
   WrUmmMeasEutraMeasObjNode     *cellCbMeasObj;
   CmLList                       *measObjEutraNode;

   TRC2(wrUmmMeasDelEutraMeasObjInCellCb)

   measObjEutraNode = measCellCb->interMeasObjLst.first;
   while (NULLP != measObjEutraNode)
   {
      cellCbMeasObj = ((WrUmmMeasEutraMeasObjNode*)measObjEutraNode->node);
      if (eutraCfgParams->earfcn == cellCbMeasObj->carrFreq)
      {
         cmLListDelFrm (&measCellCb->interMeasObjLst, measObjEutraNode);
         WR_FREE(cellCbMeasObj, sizeof(WrUmmMeasEutraMeasObjNode));
         break;
      }
      measObjEutraNode = measObjEutraNode->next;
   }

   RETVALUE(ROK);
}

/* DYNAMIC_OAM and HRL interaction changes -start */

/** @brief This function is responsible for deletion for Eutra Measurement 
 *  Objects in HRL UeCb. 
 *
 * @details
 *
 *  Function: wrUmmMeasDelEutraMeasObjInHrlUeCb 
 * 
 *  Processing steps:
 *    - delete Measurement object in HRL UE Cb.
 * 
 * @param[out] ueCb: pointer to ue Cb         
 * @param[in]  arfcn: arfcn value 
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasDelEutraMeasObjInHrlUeCb
(
 WrUeCb                       *ueCb,
 U16                          arfcn
)
{
   WrUmmMeasEutraMeasObjNode    *eutraMeasObj;

   TRC2(wrUmmMeasDelEutraMeasObjInHrlUeCb)

      /* As of now,Inter meas object is only getting modified, so we will
       * check only ueCb->hrlMeasCfg.interMeasObjLst.count */

      if(ueCb->hrlMeasCfg.interMeasObjLst.count != 0)
      {
         cmLListFirst(&ueCb->hrlMeasCfg.interMeasObjLst);

         while(cmLListCrnt(&ueCb->hrlMeasCfg.interMeasObjLst))
         {
            eutraMeasObj = (WrUmmMeasEutraMeasObjNode *) cmLListNode\
                           (cmLListCrnt(&ueCb->hrlMeasCfg.interMeasObjLst));
            if(arfcn == eutraMeasObj->carrFreq)
            {
               /* First of all delete the cells which are avilable in WL or BL
                * then delete the link */
               wrFreeUeHrlWlLst(eutraMeasObj);
               wrFreeUeHrlBlLst(eutraMeasObj);
               cmLListDelFrm(&ueCb->hrlMeasCfg.interMeasObjLst, &eutraMeasObj->lnk);
               WR_FREE(eutraMeasObj, sizeof(WrUmmMeasEutraMeasObjNode));
               RETVALUE(ROK);
            }
            cmLListNext(&ueCb->hrlMeasCfg.interMeasObjLst);
         }
      }
   RETVALUE(ROK);
}
/* DYNAMIC_OAM and HRL interaction changes -end */
 
/** @brief This function is responsible for updating the modification for 1xCdma
 *  cell reconfiguration. 
 *
 * @details
 *
 *  Function:wrUmmMeasMod1xCdmaMeasObjInCellCb
 * 
 *  Processing steps:
 *    - Update cell reconfiguration in cellCb.
 * 
 * @param[out] cellCb: pointer to Cell Cb         
 * @param[in]  cdmaCfgParams: pointer to 1Xcdma config Params 
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasMod1xCdmaMeasObjInCellCb
(
 WrUmmCellCb                 *cellCb,
 LwrNeighCdma1xFreq          *cdmaCfgParams
)
{
   U8                          bandClass;
   U32                         arfcn; 
   WrUmmMeasCdmaMeasObjNode    *cellCbMeasObj;

   TRC2(wrUmmMeasMod1xCdmaMeasObjInCellCb)

   bandClass = cdmaCfgParams->bandClass; 
   arfcn = cdmaCfgParams->arfcn; 
   if(RFAILED == wrUmmMeasUpdtCellCdmaMeasObjFrmArfcn(bandClass, arfcn,
            &cellCb->measCellCb->cdma1xMeasObjLst, &cellCbMeasObj))
   {
      RETVALUE(RFAILED);
   }
   cellCbMeasObj->searchWindowSize = cdmaCfgParams->searchRate;
   cellCbMeasObj->qOffsetRangeInterRAT = cdmaCfgParams->offsetFreq;
   RETVALUE(ROK);
}

/** @brief This function is responsible for updating the modification for 1xCdma
 *  cell reconfiguration. 
 *
 * @details
 *
 *  Function:wrUmmMeasModCdmaHrpdMeasObjInCellCb
 * 
 *  Processing steps:
 *    - Update cell reconfiguration in cellCb.
 * 
 * @param[out] cellCb: pointer to Cell Cb         
 * @param[in]  cdmaCfgParams: pointer to 1Xcdma config Params 
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasModCdmaHrpdMeasObjInCellCb
(
 WrUmmCellCb                 *cellCb,
 LwrNeighCdmaHRPDFreq        *cdmaCfgParams
)
{
   U8                          bandClass;
   U32                         arfcn; 
   WrUmmMeasCdmaMeasObjNode    *cellCbMeasObj;

   TRC2(wrUmmMeasModCdmaHrpdMeasObjInCellCb)

   bandClass = cdmaCfgParams->bandClass; 
   arfcn = cdmaCfgParams->arfcn; 
   if(RFAILED == wrUmmMeasUpdtCellCdmaMeasObjFrmArfcn(bandClass, arfcn,
            &cellCb->measCellCb->cdmaHrpdMeasObjLst, &cellCbMeasObj))
   {
      RETVALUE(RFAILED);
   }
   cellCbMeasObj->searchWindowSize = cdmaCfgParams->searchRate;
   cellCbMeasObj->qOffsetRangeInterRAT = cdmaCfgParams->offsetFreq;
   RETVALUE(ROK);
}

/** @brief This function is responsible for updating the deletion for Utra
 *  cell reconfiguration. 
 *
 * @details
 *
 *  Function:wrUmmMeasDelUtraMeasObjInCellCb
 * 
 *  Processing steps:
 *    - Update cell reconfiguration in cellCb.
 * 
 * @param[out] cellCb: pointer to Cell Cb         
 * @param[in]  utraCfgParams: pointer to Utra config Params 
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasDelUtraMeasObjInCellCb
(
 CmLListCp                     *measCellCb,
 LwrNeighUtraDelInfo           *utraCfgParams
)
{
   WrUmmMeasUtraMeasObjNode     *cellCbMeasObj;
   CmLList                       *measObjUtraNode;

   TRC2(wrUmmMeasDelUtraMeasObjInCellCb)

   measObjUtraNode = measCellCb->first;
   while (NULLP != measObjUtraNode)
   {
      cellCbMeasObj = ((WrUmmMeasUtraMeasObjNode*)measObjUtraNode->node);
      if (utraCfgParams->arfcn == cellCbMeasObj->arfcn)
      {
         cmLListDelFrm (measCellCb,measObjUtraNode);
         WR_FREE(cellCbMeasObj, sizeof(WrUmmMeasUtraMeasObjNode));
         break;
      }
      measObjUtraNode = measObjUtraNode->next;
   }

   RETVALUE(ROK);
}



/** @brief This function is responsible for updating the modification for Utra
 *  cell reconfiguration. 
 *
 * @details
 *
 *  Function:wrUmmMeasModUtraMeasObjInCellCb
 * 
 *  Processing steps:
 *    - Update cell reconfiguration in cellCb.
 * 
 * @param[out] cellCb: pointer to Cell Cb         
 * @param[in]   utraCfgParams: pointer to utra config Params 
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasModUtraMeasObjInCellCb
(
 WrUmmCellCb                 *cellCb,
 LwrNeighUtraFreq            *utraCfgParams
)
{
   WrUmmMeasUtraMeasObjNode    *cellCbMeasObj;

   TRC2(wrUmmMeasModUtraMeasObjInCellCb);

   if(RFAILED == wrUmmMeasUpdtCellUtraMeasObjFrmArfcn(utraCfgParams->t.utraFdd.arfcnDl,
            &cellCb->measCellCb->utraFddMeasObjLst, &cellCbMeasObj))
   {
      RETVALUE(RFAILED);
   }
  if(cellCbMeasObj->qOffsetFreq != utraCfgParams->offsetFreq)
  {
   cellCbMeasObj->qOffsetFreq= utraCfgParams->offsetFreq;
    wrUmmTrgRrcReCfgForAllUesCfg(cellCb, utraCfgParams->t.utraFdd.arfcnDl);
  }
   RETVALUE(ROK);
}

/** @brief This function is responsible for updating the Modification of 
 *  cell reconfiguration. 
 *
 * @details
 *
 *  Function:wrUmmMeasNrFreqReCfgMod
 * 
 *  Processing steps:
 *    - Update cell reconfiguration in cellCb.
 * 
 *  @param[in]  cellCb: Cell control block
 *  @param[in]  numFreq: Number of frequency
 *  @param[in]  modMeasCfg: pointer to the LwrNeighFreqAddModCfg object.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasNrFreqReCfgMod
(
 WrUmmCellCb                 *cellCb,
 U8                          numFreq,
 LwrNeighFreqAddModCfg       *modMeasCfg
)
{
   U8                          idCnt;
   U8                          ueCnt;
   WrMeasCellReCfgInfo         *reCfgInfo;
   WrMeasObjFreq               *modNrFreq; 
   WrUmmInternalMsg            *internalMsg;
   WrUmmIncMsg                 *msg;
   WrUeCb                      *ueCb;
   WrMeasUeCb                  *measUeCb;

   TRC2(wrUmmMeasNrFreqReCfgMod)

   WR_ALLOC(&reCfgInfo, sizeof(WrMeasCellReCfgInfo));
   if(NULLP == reCfgInfo)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   else
   {
      reCfgInfo->action = WR_ACT_MOD; 
      reCfgInfo->numFreq = numFreq; 
      modNrFreq = reCfgInfo->t.modNeighFreq;
   }

   for(idCnt = 0; idCnt < numFreq; idCnt++)
   {
      switch(modMeasCfg[idCnt].type)
      {
         case WR_NEIGH_CELL_EUTRA:
            {  
               if(RFAILED == wrUmmMeasModEutraMeasObjInCellCb(cellCb,
                        &modMeasCfg[idCnt].t.eutranFreq))
               {
                  RLOG0(L_ERROR, "Eutra Object modification failed");
                  RETVALUE(RFAILED);
               }
               modNrFreq[idCnt].choice = WR_UMM_MEAS_INTER_FREQ; 
               modNrFreq[idCnt].t.carrierFreq = modMeasCfg[idCnt].t.eutranFreq.earfcnDl;
               
               /* DYNAMIC_OAM and HRL interaction changes -start */
               /* Here update the Hrl UeCb for modified Freq configuration */ 
               if(RFAILED == wrUmmMeasAddModEutraMeasObjInHrlUeCb(cellCb, 
                           &modMeasCfg[idCnt].t.eutranFreq)) 
               {
                  RLOG0(L_ERROR, "Eutra Meas Obj modification in HRL UeCB failed");
                  RETVALUE(RFAILED);
               }
               /* DYNAMIC_OAM and HRL interaction changes -end */
               break;
            }
         case WR_NEIGH_CELL_CDMA2K_1XRTT:
            {
               if(RFAILED == wrUmmMeasMod1xCdmaMeasObjInCellCb(cellCb, &modMeasCfg[idCnt].
                        t.cdma1xFreq))
               {
                  RLOG0(L_ERROR, "1xCdma Modification failed");
                  RETVALUE(RFAILED);
               }
               modNrFreq[idCnt].choice = WR_UMM_MEAS_1XCDMA; 
               modNrFreq[idCnt].t.measObjCDMA2000.bandClass = modMeasCfg[idCnt].t.cdma1xFreq.bandClass;
               modNrFreq[idCnt].t.measObjCDMA2000.arfcn = modMeasCfg[idCnt].t.cdma1xFreq.arfcn;
               break;
            }
         case WR_NEIGH_CELL_CDMA2K_HRPD:
            {
               if(RFAILED == wrUmmMeasModCdmaHrpdMeasObjInCellCb(cellCb, &modMeasCfg[idCnt].
                        t.cdmaHrpdFreq))
               {
                  RLOG0(L_ERROR, "Cdma Hrpd Modification failed");
                  RETVALUE(RFAILED);
               }
               modNrFreq[idCnt].choice = WR_UMM_MEAS_CDMA_HRPD; 
               modNrFreq[idCnt].t.measObjCDMA2000.bandClass = modMeasCfg[idCnt].t.cdmaHrpdFreq.bandClass;
               modNrFreq[idCnt].t.measObjCDMA2000.arfcn = modMeasCfg[idCnt].t.cdmaHrpdFreq.arfcn;
               break;
            }
		 case WR_NEIGH_CELL_UTRA_FDD:
            {  
               if(RFAILED == wrUmmMeasModUtraMeasObjInCellCb(cellCb,
                        &modMeasCfg[idCnt].t.utranFreq))
               {
                  RLOG0(L_ERROR, "Utra Object modification failed");
                  RETVALUE(RFAILED);
               }
               modNrFreq[idCnt].choice = WR_UMM_MEAS_UTRA_FDD_FREQ; 
               modNrFreq[idCnt].t.carrierFreq = modMeasCfg[idCnt].t.utranFreq.t.utraFdd.arfcnDl;
               break;
            }
         default:
            RLOG0(L_ERROR, "Invalid Type Received");
            RETVALUE(RFAILED);
      }
   }
   /*
      rrc reconfiguration will be sent only if utra freq q-offset chnaged 
      if we want send rrc reconfiguration for particular parameter changed then 
      do it in handeling of particulr freq change  like wrUmmMeasModUtraMeasObjInCellCb
      */

   if (ROK != wrUmmBldInternalMsg(&internalMsg, WR_UMM_INTMSG_NEW_MEAS_TRANS,
            reCfgInfo))
   {
      RLOG0(L_ERROR, "creation of internal message Failed");
      WR_FREE(reCfgInfo, sizeof(WrMeasCellReCfgInfo));
      RETVALUE(RFAILED);
    }

    WR_ALLOC(&msg,sizeof(WrUmmIncMsg));
    if (NULLP == msg)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
       WR_FREE(reCfgInfo, sizeof(WrMeasCellReCfgInfo));
       WR_FREE(internalMsg, sizeof(WrUmmInternalMsg));
       RETVALUE(RFAILED);
    }

    msg->msgTyp = WR_UMM_INTERNAL_MSG;
    msg->u.internalMsg = internalMsg;
    msg->refCnt = 1;

    for(ueCnt = 0; ueCnt < cellCb->maxUesPerCell; ueCnt++)
    {
       ueCb = cellCb->ueCbs[ueCnt];
       if(NULLP != ueCb && msg != NULLP)
       {
          measUeCb = ueCb->measCfg;
          if((NULLP != measUeCb) && (NULLP != measUeCb->interMeasObjLst.count))
          {
             msg->refCnt += 1;
             wrUmmPrcIncMsg(cellCb, ueCb, msg);
             wrUmmRlsIncMsg(&msg);
          }
       }
    }
    if(msg != NULLP)
    {
       wrUmmRlsIncMsg(&msg);
    }

#if 0 /*no need to send rrc reconfiguration for all the UE's */
   /* Building internal message */
   for(ueCnt = 0; ueCnt < cellCb->maxUesPerCell; ueCnt++)
   {
      WrUeCb                 *ueCb;
      WrMeasUeCb             *measUeCb;
      ueCb = cellCb->ueCbs[ueCnt];

      if(NULLP != ueCb)
      {
      measUeCb = ueCb->measCfg;
         if((NULLP != measUeCb) && (NULLP != measUeCb->interMeasObjLst.count))
      {
         wrUmmMeasCreateIntTrans(cellCb, ueCb, WR_UMM_INTMSG_NEW_MEAS_TRANS, 
               WR_UMM_MEAS_NR_NEW_FREQ, NULLP, reCfgInfo);
      }
   }
#endif
   RETVALUE(ROK);
}

/** @brief This function is responsible for updating the deletion of 
 *  cell reconfiguration. 
 *
 * @details
 *
 *  Function:wrUmmMeasNrFreqReCfgDel
 * 
 *  Processing steps:
 *    - Update cell reconfiguration in cellCb.
 * 
 *  @param[in]  cellCb: Cell control block
 *  @param[in]  numFreq: number of frequency
 *  @param[in]  delMeasCfg: pointer to the LwrNeighFreqAddModCfg object.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasNrFreqReCfgDel
(
 WrUmmCellCb                 *cellCb,
 U8                          numFreq,
 LwrNeighFreqDelCfg          *delMeasCfg
)
{
   U8                          *totMeasObjId;
   U8                          measObjId;
   U8                          idCnt;
   U8                          ueCnt; 
   WrMeasCellReCfgInfo         *reCfgInfo;

   TRC2(wrUmmMeasNrFreqReCfgDel)

   for(ueCnt = 0; ueCnt < cellCb->maxUesPerCell; ueCnt++)
   {
      WrUeCb                 *ueCb;
      WrMeasUeCb             *measUeCb;
      ueCb = cellCb->ueCbs[ueCnt];
      if(NULLP != ueCb)
      {
         measUeCb = ueCb->measCfg;
         if((NULLP != measUeCb) && (0 != measUeCb->interMeasObjLst.count))
         {
            WR_ALLOC(&reCfgInfo, sizeof(WrMeasCellReCfgInfo));
            if(NULLP == reCfgInfo)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            else
            {
               reCfgInfo->action = WR_ACT_DEL; 
               reCfgInfo->numFreq = numFreq; 
               totMeasObjId = reCfgInfo->t.measObjIds;
            }

            for(idCnt = 0; idCnt < numFreq; idCnt++)
            {
               if(WR_NEIGH_CELL_EUTRA == delMeasCfg[idCnt].type)
               {
                  measObjId =  wrUmmMeasDelEutraMeasObjInUeCb(&ueCb->measCfg->interMeasObjLst, 
                        delMeasCfg[idCnt].t.eutranFreq.earfcn);
                  if(measObjId == 0)
                  {
                     RLOG0(L_ERROR, "Meas Object Not found");
                     WR_FREE(reCfgInfo, sizeof(WrMeasCellReCfgInfo));
                     RETVALUE(RFAILED);
                  }
                  totMeasObjId[idCnt] = measObjId;

                  /* DYNAMIC_OAM and HRL interaction changes -start */
                  if(RFAILED == wrUmmMeasDelEutraMeasObjInHrlUeCb(ueCb, 
                           delMeasCfg[idCnt].t.eutranFreq.earfcn))
                  {
                     RLOG0(L_ERROR, "Eutra Meas Obj deletion in HRL UeCB failed");
                     RETVALUE(RFAILED);
                  }
                  /* DYNAMIC_OAM and HRL interaction changes -end */
               }
               else if(WR_NEIGH_CELL_UTRA_FDD == delMeasCfg[idCnt].type)
               {
                  measObjId = wrUmmMeasDelUtraMeasObjInUeCb(&ueCb->measCfg->utraFddMeasObjLst, 
                        delMeasCfg[idCnt].t.utranFreq.arfcn);
                  if(measObjId == 0)
                  {
                     RLOG0(L_ERROR, "Meas Object Not found");
                     WR_FREE(reCfgInfo, sizeof(WrMeasCellReCfgInfo));
                     RETVALUE(RFAILED);
                  }
                  totMeasObjId[idCnt] = measObjId;
               }
               else if(WR_NEIGH_CELL_UTRA_TDD == delMeasCfg[idCnt].type)
               {
                  /* UTRA TDD MeasCfg deletion */
						{
                  measObjId = wrUmmMeasDelUtraMeasObjInUeCb(&ueCb->measCfg->utraTddMeasObjLst, 
                        delMeasCfg[idCnt].t.utranFreq.arfcn);
                  if(measObjId == 0)
                  {
                     RLOG0(L_ERROR, "Meas Object Not found");
                     WR_FREE(reCfgInfo, sizeof(WrMeasCellReCfgInfo));
                     RETVALUE(RFAILED);
                  }
                  totMeasObjId[idCnt] = measObjId;
               }
               }
               else
               {
                  measObjId =  wrUmmMeasDelCdmaMeasObjInUeCb(&ueCb->measCfg->interRatMeasObjLst, 
                        &delMeasCfg[idCnt].t.cdmaFreq);
                  totMeasObjId[idCnt] = measObjId;
               }
            }
            /* Building internal message */
            if(ROK != wrUmmMeasCreateIntTrans(cellCb, ueCb, WR_UMM_INTMSG_NEW_MEAS_TRANS, 
                     WR_UMM_MEAS_NR_NEW_FREQ, NULLP, reCfgInfo))
            {
               WR_FREE(reCfgInfo, sizeof(WrMeasCellReCfgInfo));
            }
         }
      }
   }
   for(idCnt = 0; idCnt < numFreq; idCnt++)
   {
      if(WR_NEIGH_CELL_EUTRA == delMeasCfg[idCnt].type)
      {
         if(RFAILED == wrUmmMeasDelEutraMeasObjInCellCb(cellCb->measCellCb,
                                      &delMeasCfg[idCnt].t.eutranFreq))
         {
            RLOG0(L_ERROR, "Eutra deletion failed");
            RETVALUE(RFAILED);
         }
      }
      else if(WR_NEIGH_CELL_CDMA2K_1XRTT == delMeasCfg->type)
      {
         if(RFAILED == wrUmmMeasDelCdmaMeasObjInCellCb(&cellCb->measCellCb->cdma1xMeasObjLst, 
                  &delMeasCfg->t.cdmaFreq))
         {
            RLOG0(L_ERROR, "1xCdma Deletion failed");
            RETVALUE(RFAILED);
         }
      }
      else if(WR_NEIGH_CELL_CDMA2K_HRPD == delMeasCfg->type)
      {
         if(RFAILED == wrUmmMeasDelCdmaMeasObjInCellCb(&cellCb->measCellCb->cdmaHrpdMeasObjLst,
                       &delMeasCfg->t.cdmaFreq))
         {
            RLOG0(L_ERROR, "Cdma Hrpd Deletion failed");
            RETVALUE(RFAILED);
         }
      }
      else if(WR_NEIGH_CELL_UTRA_TDD == delMeasCfg->type)
      {
      	 if(RFAILED == wrUmmMeasDelUtraMeasObjInCellCb(&cellCb->measCellCb->utraTddMeasObjLst,
                       &delMeasCfg->t.utranFreq))
         {
            RLOG0(L_ERROR, "Utra TDD Deletion failed");
            RETVALUE(RFAILED);
         }
      }
		else
      {
      	 if(RFAILED == wrUmmMeasDelUtraMeasObjInCellCb(&cellCb->measCellCb->utraFddMeasObjLst,
                       &delMeasCfg->t.utranFreq))
         {
            RLOG0(L_ERROR, "Utra Deletion failed");
            RETVALUE(RFAILED);
         }
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for updating the White and black list
 *  object
 * 
 * @details
 *
 *  Function:wrUmmMeasGetNrCellInfoFrmInterObj
 * 
 *  Processing steps:
 *    - Update white and black list object.
 * 
 *  @param[in]  cellId: Cell identifier.
 *  @param[in]  nrType: Neighbor Type.
 *  @param[in]  enbCellObj: measurement cell Object.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasGetNrCellInfoFrmInterObj
(
 WrNrPerFreqWlInfo             **wlData, 
 WrNrPerFreqBlInfo             **blData, 
 WrNrInterFreqInfo             *interFreq,
 U16                           arfcn
)
{
   U8                           numFreq;
   U8                           idCnt;
   
   TRC2(wrUmmMeasGetNrCellInfoFrmInterObj)
   numFreq = interFreq->numFreq;
   for(idCnt = 0; idCnt < numFreq; idCnt++)
   {
      if(arfcn == interFreq->freqList[idCnt].dlCarrierFreq)
      {
         *wlData = &interFreq->freqList[idCnt].wlData;
         *blData = &interFreq->freqList[idCnt].blData;
         RETVALUE(ROK);
      }
   }
   RETVALUE(RFAILED);
}
/** @brief This function is responsible for updating the cell information into
 * cell measuremnt object
 * 
 * @details
 *
 *  Function:wrUmmMeasFillCellInfoMeasCellObj
 * 
 *  Processing steps:
 *    - Update cell Information.
 * 
 *  @param[in]  cellId: Cell identifier.
 *  @param[in]  nrType: Neighbor Type.
 *  @param[in]  enbCellObj: measurement cell Object.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasFillCellInfoMeasCellObj
(
 U16                          cellId,
 WrUmmMeasCellTyp             nrType,
 WrUmmMeasEutraMeasObjNode    *enbCellObj
)
{
   U8                         idCnt;
   WrNrWlBlInfo               *nrWlBlInfo = NULLP;
   WrNrPerFreqWlInfo          *wlData = NULLP;
   WrNrPerFreqBlInfo          *blData = NULLP;

   TRC2(wrUmmMeasFillCellInfoMeasCellObj)
      if(nrType == WR_UMM_MEAS_INTRA_FREQ)
      {
         nrWlBlInfo = wrEmmAnrGetWhiteAndBlackList(cellId, WR_ANR_LST_INTRA_FREQ, TRUE);
         if(!nrWlBlInfo)
         {
            RETVALUE(RFAILED);
         }
         wlData = &nrWlBlInfo->intra.wlData;
         blData = &nrWlBlInfo->intra.blData;
      }
      else
      {
         nrWlBlInfo = wrEmmAnrGetWhiteAndBlackList(cellId, WR_ANR_LST_INTER_FREQ, TRUE);
         if(!nrWlBlInfo)
         {
            RETVALUE(RFAILED);
         }
         wrUmmMeasGetNrCellInfoFrmInterObj(&wlData, &blData, &nrWlBlInfo->inter, enbCellObj->carrFreq);
      }
   if(NULLP != wlData)
   {
      U8                        wlCellCnt;
      wlCellCnt = wlData->numWlCells;
      for(idCnt = 0; idCnt < wlCellCnt; idCnt++)
      {
         U16                   pci;
         U32                   cio;

         pci = wlData->cellInfo[idCnt].phyCellId;
         cio = wlData->cellInfo[idCnt].cio;
         wrUmmMeasFillEutraWLCellLst(enbCellObj, pci, cio);
      }
   }
   if(NULLP != blData)
   {
      U8                        blCellCnt;
      blCellCnt = blData->numBlInfos;
      for(idCnt = 0; idCnt < blCellCnt; idCnt++)
      {
         U16                   startPci;
         U8                    pciRange;

         startPci = blData->blInfo[idCnt].startPhyCellID;
         pciRange = blData->blInfo[idCnt].phyCellIdRange;
         wrUmmMeasFillEutraBLCellLst(enbCellObj, startPci,  pciRange);
      }
   }
   RETVALUE(ROK);
}
/** @brief This function is responsible for updating the cell reconfiguration
 * for intra frequency
 *
 * @details
 *
 *  Function:wrUmmMeasUpdateIntraFreqCellObj
 * 
 *  Processing steps:
 *    - Update intra cell reconfiguration in cellCb.
 * 
 *  @param[in]  cellId: Cell identifier.
 *  @param[in]  newArfcn: Arfcn.
 *  @param[in]  newPci: New Pci.
 *  @param[in]  measCellCb: measurement cell control block.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PRIVATE S16 wrUmmMeasUpdateIntraFreqCellObj
(
 U16                         cellId, 
 U16                         newArfcn, 
 U16                         newPci, 
 WrUmmMeasCellCb             *measCellCb
 )
{
   U16                         arfcn;
   U8                          idCnt;
   U8                          allowedMeasBW;
   U8                          presAntPort1;
   U8                          neighCellCfg;
   U8                          qOffsetFreq;
   WrUmmMeasEutraMeasObjNode   *intraCellObj;
   WrUmmMeasEutraMeasObjNode   *interCellObj
   
   TRC2(wrUmmMeasUpdateIntraFreqCellObj) 

   intraCellObj = &measCellCb->intraMeasObj;
   arfcn = intraCellObj->carrFreq;
   allowedMeasBW = intraCellObj->allowedMeasBW;
   presAntPort1 = intraCellObj->presAntPort1;
   neighCellCfg = intraCellObj->neighCellCfg;
   if(RFAILED == wrEmmGetOffsetFreqFrmArfcn(cellId, arfcn, &qOffsetFreq))
   {
      RETVALUE(RFAILED);
   }
 
   if(RFAILED == wrUmmMeasUpdtCellEutraMeasObjFrmArfcn(intraCellObj->carrFreq, &measCellCb->
                                            interMeasObjLst, &interCellObj))
   {
      RETVALUE(RFAILED);
   }
   
   intraCellObj->carrFreq = newArfcn;
   intraCellObj->allowedMeasBW = interCellObj->allowedMeasBW;
   intraCellObj->presAntPort1 = interCellObj->presAntPort1;
   intraCellObj->neighCellCfg = interCellObj->neighCellCfg;
   intraCellObj->qOffsetFreq = 0;
   
   interCellObj->carrFreq = arfcn;
   interCellObj->allowedMeasBW = allowedMeasBW;
   interCellObj->presAntPort1 = presAntPort1;
   interCellObj->neighCellCfg = neighCellCfg;
   interCellObj->qOffsetFreq = qOffsetFreq;

   wrUmmMeasDeAllocEutraWlCellList(&intraCellObj->wlCellLst);
   wrUmmMeasDeAllocEutraBlCellList(&intraCellObj->blCellLst);
   wrUmmMeasDeAllocEutraWlCellList(&interCellObj->wlCellLst);
   wrUmmMeasDeAllocEutraBlCellList(&interCellObj->blCellLst);

   for(idCnt = 0; idCnt < WR_UMM_MEAS_MAX_CELL_IDX; idCnt++)
   {
      intraCellObj->wlCellIdxInUse[idCnt] = FALSE;
      intraCellObj->blCellIdxInUse[idCnt] = FALSE;
      interCellObj->wlCellIdxInUse[idCnt] = FALSE;
      interCellObj->blCellIdxInUse[idCnt] = FALSE;
   }

   /* ccpu00133750: Changed the default cellindividualoffst value from 0 (-24db) 
    * to 15 (which zero DB). As this makes issue when we try handover back to
    * source
    */
   wrUmmMeasFillEutraWLCellLst(intraCellObj, newPci, LWR_OFFSET_DB_ZERO_0);

   wrUmmMeasFillCellInfoMeasCellObj(cellId,  WR_UMM_MEAS_INTRA_FREQ, intraCellObj);
   wrUmmMeasFillCellInfoMeasCellObj(cellId,  WR_UMM_MEAS_INTER_FREQ, interCellObj);

   RETVALUE(ROK);
}

/** @brief This function is responsible for verifying whether DL arfcn and pci
 * has changed or not for intra frequency
 *
 * @details
 *
 *  Function:wrUmmMeasIntraCellReCfg
 * 
 *  Processing steps:
 *    - Verify Dl arfcn and pci for intra cell reconfiguration in cellCb.
 * 
 *  @param[in]  cellId: Cell identifier.
 *  @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
 */
PUBLIC S16 wrUmmMeasIntraCellReCfg
(
 U16                         cellId
)
{
   U16                         oldArfcn;
   U16                         newArfcn;
   U16                         *oldPci;
   U16                         newPci;
   WrUmmCellCb                 *cellCb;
   CmLList                     *wlCellLnk;
   WrUmmMeasEutraMeasObjNode   *intraCellObj;

   TRC2(wrUmmMeasIntraCellReCfg)

   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }           /*KW fix for LTE_ADV */
  
   intraCellObj = &cellCb->measCellCb->intraMeasObj;
  
   newArfcn = wrEmmGetServCarrierFreq(cellId);
   newPci = wrEmmGetPhyCellId(cellId);
   oldArfcn = intraCellObj->carrFreq;
   if ( NULLP == intraCellObj->wlCellLst.first )
   {
      RLOG0(L_DEBUG, "wrUmmMeasIntraCellReCfg : wlCellLst is NULL, Intra Cell neighbors are not configured !!! ");
      RETVALUE(ROK);
   }

   wlCellLnk = intraCellObj->wlCellLst.first;
   oldPci = &(((WrMeasNrWCellNode*)(wlCellLnk->node))->pci);

   if(newArfcn != oldArfcn)
   {
      if(RFAILED == wrUmmMeasUpdateIntraFreqCellObj(cellId, newArfcn, newPci, cellCb->measCellCb))
      {
         RETVALUE(RFAILED);
      }
   }
   else
   {
      if(newPci != (*oldPci))
      {
         *oldPci = newPci;
      }
   }
      
   RETVALUE(ROK);
}
/** @brief This function is responsible for to Add inter measurement Object
 * into uecb.
 *
 *  @details
 * Function:wrUmmMeasAddUtraMeasObjToUeCb
 *
 *   Processing steps:
 *    - Add inter measurement object into UE control block.
 *
 * @param[in]  measUeCb: pointer to the measurement ue Cb
 * @param[out]  cellCbMeasObj: pointer to the CellCb measurement Object.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/
PUBLIC U8 wrUmmMeasAddUtraMeasObjToUeCb
(
 WrMeasUeCb                  *measUeCb,
 WrUmmMeasUtraMeasObjNode    *cellCbMeasObj,
 U16                         newPsc
)
{
   WrMeasUeCbUtraMeasObjNode    *measObjUtraNode;
   U8                            measObjId = 0;
   S16                           ret;

   TRC2(wrUmmMeasAddUtraMeasObjToUeCb)
      ret =  wrUmmMeasGetFreeMeasObjId(measUeCb, &measObjId);
   if(ret == WR_UMM_MEAS_INVALID_MEAS_ID)
   {
      RLOG0(L_ERROR, "Meas Object Id not available");
      RETVALUE(ret);
   }
   WR_ALLOC(&measObjUtraNode, sizeof(WrMeasUeCbUtraMeasObjNode));
   if(NULLP == measObjUtraNode)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      wrUmmMeasSetFreeMeasObjId(measUeCb, measObjId);
      RETVALUE(RFAILED);
   }
   measObjUtraNode->utraMeasObj = cellCbMeasObj;
   measObjUtraNode->measObjId = measObjId;
   measObjUtraNode->newPsc = newPsc;

   measObjUtraNode->lnk.node = (PTR)measObjUtraNode;
   if(WR_UTRA_MODE_FDD == cellCbMeasObj->duplexMode)
   {
      cmLListAdd2Tail(&measUeCb->utraFddMeasObjLst, &measObjUtraNode->lnk);
   }
   else
   {
      cmLListAdd2Tail(&measUeCb->utraTddMeasObjLst, &measObjUtraNode->lnk);
   }
   RETVALUE(measObjId);
}

/** @brief This function is responsible for configuring measurements for CSFB 
 * Module.
 *
 * @details
 *
 * Function:wrUmmMeasCsfbUtraFddCfg
 *   Triggered By: wrUmmCsfbReq
 *
 *   Processing steps:
 *    - Delete the configured measuremnent to UE.
 *    - configure CSFB related measurements
 * 
 * @param[in]  memCp:   	pointer to memoty control pointer.
 * @param[in]  nhuMeasCfg : 	pointer to Nhu Measurement object. 
 * @param[in]  crnti: 		Crnti
 * @param[in]  csfbTransCb: 	CSFB transaction
 * @return SUCCESS : ROK
 *         FAILURE : RFAILED
 */

PUBLIC S16 wrUmmMeasCsfbUtraFddCfg
(
 WrUeCb			              *ueCb,
 WrUmmTransCb                *transCb
)
{
   WrMeasUeCb                  *measUeCb;
   WrUmmCellCb                 *cellCb;
   LwrCellMeasCfgGrp           *measCellCfg;
   WrUmmMeasTransInfo          *measTransCb;
   U16			                 cellId;
   U16	                       crnti;
   Bool                         isGapReqd = FALSE;

   TRC2(wrUmmMeasCsfbUtraFddCfg)
   RLOG0(L_DEBUG, "Function wrUmmMeasCsfbUtraFddCfg");   

   measTransCb = &transCb->u.csfbTrans.measTransCb;
   cellId = ueCb->cellId;
   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }
   measCellCfg = cellCb->measCellCfg;

   crnti = ueCb->crnti;

   measUeCb = ueCb->measCfg;

   /* Measurements will be configured to UE only if FGI 16 and 22 are set*/
   /* RRC-REL10-Upgrade */ 
   if((FALSE == wrUmmUtilFeatureGrpInd(cellId, crnti, 
           WR_UMM_UTRA_MEAS_SUPPORTED)) || 
           (FALSE == wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti,
           WR_UMM_NON_ANR_PERD_MEAS_SUPPORTED)))
   {
      RLOG0(L_ERROR, "UE is not capable of doing UTRA measurements");
      RETVALUE(RFAILED);
   }

   if((NULLP != measTransCb))
   {
      /* Check HRL for this UE to see if UTRA RAT is forbidden or not. */
      if((ueCb->hoRstrctInfo != NULLP) && (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
      {
         WrHrlDecisionRet forbRat = WR_MEAS_MBLTY_APPL_TO_RAT;

         forbRat = wrChkForFrbRat(WR_FORB_RAT_UTRA, ueCb->hoRstrctInfo);
         if(forbRat == WR_MEAS_MBLTY_NOT_APPL_TO_RAT)
         {
            /* UTRA is forbidden for the UE, so return failure. */
            RLOG0(L_ERROR, "UTRA RAT is forbidden for the UE");
            RETVALUE(RFAILED);
         }
      }
      /*Utra Freq Cell support by UE : Check in Ue capability*/
      if(ROK != wrUmmMeasCreatUtraCfgMsg(ueCb,cellCb,
               measTransCb, &isGapReqd,WR_UMM_MEAS_TYPE_CSFB))
      {
         RLOG0(L_ERROR, "wrUmmMeasCreatUtraCfgMsg Returns ERROR, sending"
                                          "So Sending Default Redirectoin");
         /* If this function returns RFAIL, then proceed with
          * default redirection. Defalt Sent by CALLER*/
         RETVALUE(RFAILED);
      }

      if(isGapReqd == TRUE)
      {
         wrUmmMeasAddMeasGap(WR_UMM_MEAS_GAP_CSFB,
                             cellCb, ueCb, &measTransCb->measGapStatus,
                             transCb->transId);
      }

      if ( ( measUeCb->gapMeasInfo.latestTxId == transCb->transId) && 
            ( ( WR_UMM_MEAS_GAP_INIT_SCH_CONFIG == measTransCb->measGapStatus)))
      {
         measTransCb->measGapStatus = WR_UMM_MEAS_GAP_INIT_SCH_CONFIG;
         wrUmmMeasGapSchdUeRecfgReq(transCb);
      }

      else
      {
         /* Prepare and send the message to the RRC layer */
         if(ROK != wrUmmMeasPrepareSendRrcRecfg(cellId, ueCb, 
                  measTransCb, 
                  transCb->transId))
         {
            RETVALUE(RFAILED);
         }
      }
   }

   RETVALUE(ROK);

}


/** @brief This function is responsible for configuring measurements for CSFB 
 * Module.
 *
 * @details
 *
 * Function:wrUmmMeasCsfbUtraTddCfg
 *   Triggered By: wrUmmCsfbReq
 *
 *   Processing steps:
 *    - Delete the configured measuremnent to UE.
 *    - configure CSFB related measurements
 * 
 * @param[in]  memCp:   	pointer to memoty control pointer.
 * @param[in]  nhuMeasCfg : 	pointer to Nhu Measurement object. 
 * @param[in]  crnti: 		Crnti
 * @param[in]  csfbTransCb: 	CSFB transaction
 * @return SUCCESS : ROK
 *         FAILURE : RFAILED
 */

PUBLIC S16 wrUmmMeasCsfbUtraTddCfg
(
 WrUeCb			              *ueCb,
 WrUmmTransCb                *transCb
)
{
   WrMeasUeCb                  *measUeCb;
   WrUmmCellCb                 *cellCb;
   LwrCellMeasCfgGrp           *measCellCfg;
   WrUmmMeasTransInfo          *measTransCb;
   U16			                 cellId;
   U16	                       crnti;
   Bool                         isGapReqd = FALSE;

   TRC2(wrUmmMeasCsfbUtraTddCfg)
   RLOG0(L_DEBUG, "Function wrUmmMeasCsfbUtraTddCfg");   

   measTransCb = &transCb->u.csfbTrans.measTransCb;
   cellId = ueCb->cellId;
   WR_UMM_GET_CELLCB(cellCb, cellId)
      if(NULLP == cellCb)
      {
         RLOG0(L_ERROR, "cellCb is NULL");
         RETVALUE(RFAILED);
      }
   measCellCfg = cellCb->measCellCfg;

   crnti = ueCb->crnti;

   measUeCb = ueCb->measCfg;

   if(FALSE == wrUmmUtilFeatureGrpInd(cellId, crnti, 
            WR_UMM_UTRA_TDD_PERD_MEAS_SUPPORTED))
   {
      RLOG0(L_ERROR, "UE is not capable of doing UTRA TDD Periodic measurements");
      RETVALUE(RFAILED);
   }

   if((NULLP != measTransCb))
   {    
      /*Utra Freq Cell support by UE : Check in Ue capability*/
      if(ROK != wrUmmMeasCreatUtraTddCfgMsg(ueCb,cellCb,
               transCb, &isGapReqd,WR_UMM_MEAS_TYPE_CSFB))
      {
         RLOG0(L_ERROR, "wrUmmMeasCreatUtraTDDCfgMsg Returns ERROR, sending"
                                          "So Sending Default Redirectoin");
         /* If this function returns RFAIL, then proceed with
          * default redirection. Dflt Sent by Caller*/
         RETVALUE(RFAILED);
      }

      /* If only 1 UTRA TDD measObjs exists in HRL_LAI list  
       * then dont configure measurements as Redirection Info 
       * would have been sent to UE in wrUmmMeasCreatUtraTddCfgMsg. */
      if(ueCb->hrlMeasCfg.utraTddMeasObjLst.count == 1)
      {
         RETVALUE(ROK);
      }

      if(isGapReqd == TRUE)
      {
         wrUmmMeasAddMeasGap(WR_UMM_MEAS_GAP_CSFB,
               cellCb, ueCb, &measTransCb->measGapStatus,
               transCb->transId);
      }

      if ((measUeCb->gapMeasInfo.latestTxId == transCb->transId) && 
            ((WR_UMM_MEAS_GAP_INIT_SCH_CONFIG == measTransCb->measGapStatus)))
      {
         measTransCb->measGapStatus = WR_UMM_MEAS_GAP_INIT_SCH_CONFIG;
         wrUmmMeasGapSchdUeRecfgReq(transCb);
      }

      else
      {
         /* Prepare and send the message to the RRC layer */
         if(ROK != wrUmmMeasPrepareSendRrcRecfg(cellId, ueCb, 
                  measTransCb, 
                  transCb->transId))
         {
            RETVALUE(RFAILED);
         }
      }
   }

   RETVALUE(ROK);
}


/** @brief This function is responsible for configuring measurements for CSFB 
 * Module.
 *
 * @details
 *
 * Function:wrUmmMeasCsfbCdmaCfg
 *   Triggered By: wrUmmCsfbReq
 *
 *   Processing steps:
 *    - Delete the configured measuremnent to UE.
 *    - configure CSFB related measurements
 * 
 * @param[in]  ueCb :    pointer to UeCb
 * @param[in]  transCb: pointer to main transCb
 * @return SUCCESS : ROK
 *         FAILURE : RFAILED
 */

PUBLIC S16 wrUmmMeasCsfbCdmaCfg
(
 WrUeCb                       *ueCb,
 WrUmmTransCb                *transCb
)
{
   WrMeasUeCb                  *measUeCb;
   WrUmmCellCb                 *cellCb;
   LwrCellMeasCfgGrp           *measCellCfg = NULLP;
   WrUmmMeasTransInfo          *measTransCb = NULLP;
   U16                          cellId;
   U16                          crnti;
   Bool                         isGapReqd = FALSE;
   WrUmmMeasTyp                 measTyp;
   U8                           ratType;

   TRC2(wrUmmMeasCsfbCdmaCfg)

   cellId = ueCb->cellId;
   WR_UMM_GET_CELLCB(cellCb, cellId)
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, "cellCb is NULL");
      RETVALUE(RFAILED);
   }
   measCellCfg = cellCb->measCellCfg;

   crnti = ueCb->crnti;

   measUeCb = ueCb->measCfg;

   if(FALSE == wrUmmChkCdmaFgi(cellId, ueCb->crnti))
   {
      RLOG0(L_ERROR, "UE is not capable(FGI) of doing CDMA measurements for Rat Type=%d");
      RETVALUE(RFAILED);
   }
   /*
   if(ROK != wrUmmMeasCopyMeasIdtoRmvLst(&measUeCb->measIdLst,measTransCb))  
   {
      RLOG0(L_ERROR, "Measurement Id Node is NULL");
      RETVALUE(RFAILED);
   } 
   */
   if(NULL == transCb)
   {
      RLOG0(L_ERROR, "wrUmmMeasCsfbCdmaCfg TRANS CB  is NULL\n");
      RETVALUE(RFAILED);
   }
   if(WR_UMM_CSFB_REDIR_TRANS == transCb->transTyp)
   {
      measTransCb = &transCb->u.csfbTrans.measTransCb;
      measTyp     = WR_UMM_MEAS_TYPE_CSFB;
      ratType      = transCb->u.csfbTrans.ratPriority;
   }
   else
   {
      measTransCb = &transCb->u.ecsfbTransCb.measTransCb;
      measTyp     = WR_UMM_MEAS_TYPE_ECSFB;
      ratType      = transCb->u.ecsfbTransCb.ratPriority;
   }
   if(NULLP != measTransCb)
   {
      /*CDMA Freq Cell support by UE : Check in Ue capability*/
      if(ROK != wrUmmMeasCreatCdmaCfgMsg(ueCb, cellCb, ratType, 
                                           &measTransCb->addMeasInfo, 
                                           &isGapReqd,measTyp))
      {
         RLOG0(L_ERROR, "failed in Configuring measurement");
         RETVALUE(RFAILED);
      }

      if(isGapReqd == TRUE)
      {
         wrUmmMeasAddMeasGap(WR_UMM_MEAS_GAP_CSFB,
                             cellCb, ueCb, &measTransCb->measGapStatus,
                             transCb->transId);
      }
      if ( ( measUeCb->gapMeasInfo.latestTxId == transCb->transId) && 
            ( ( WR_UMM_MEAS_GAP_INIT_SCH_CONFIG == measTransCb->measGapStatus)))
      {
         measTransCb->measGapStatus = WR_UMM_MEAS_GAP_INIT_SCH_CONFIG;
         wrUmmMeasGapSchdUeRecfgReq(transCb);
      }

      else
      {
         /* Prepare and send the message to the RRC layer */
         if(ROK != wrUmmMeasPrepareSendRrcRecfg(cellId, ueCb, 
                  measTransCb, 
                  transCb->transId))
         {
            RETVALUE(RFAILED);
         }
      }

   }
   RETVALUE(ROK);

}
/** @brief This function is responsible for to Add inter measurement Object
 * into uecb.
 *
 *  @details
 * Function:wrUmmMeasGetArfcnAndRptType
 *
 *   Processing steps:
 *    - Add inter measurement object into UE control block.
 *
 * @param[in]  cellId  : cell Index
 * @param[in]  UeIndx   : UE Index at application
 * @param[in]  measUeCb: pointer to the measurement ue Cb
 * @param[out]  cellCbMeasObj: pointer to the CellCb measurement Object.
 * @return S16
 *       SUCCESS : ROK
 *       FAILED  : RFAILED
**/

PUBLIC S16 wrUmmMeasGetArfcnAndRptType
(
  U8                       ueIndx,
  WrCellId                 cellId,
  NhuMeasResults           *measRpt,     
  WrUmmMeasRptInfo         *measRptInfo
)
{
  U8                           measId;
  U8                           measObjId;
  U8                           rptCfgId;
  U32                          recvMeasId;
  WrMeasRptCfgType             rptCfgTyp = 0;
  WrMeasIdNode                 *measIdObj = NULLP;
  WrMeasUeCb                   *measUeCb;
  WrUmmMeasObjInfo              measObjInfo = {0};
  WrUeCb                       *ueCb;

  TRC2(wrUmmMeasGetArfcnAndRptType)

  wrUmmGetUeFrmUeIdx(&ueCb, cellId, ueIndx);
  if (ueCb == NULLP)
  {
     RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellId, "Error in getting UECB for "
           "UE INDEX [%u]", ueIndx);
     RETVALUE(RFAILED);
  }

  measUeCb = ueCb->measCfg;

  recvMeasId = measRpt->measId.val;
  wrUmmMeasGetMeasIdObj(recvMeasId, &measIdObj, &ueCb->measCfg->measIdLst);

  measId = measIdObj->measID;
  measObjId = measIdObj->measObjID;
  rptCfgId = measIdObj->rptCfgID;

  RLOG2(L_DEBUG, "measId = %d, rptCfgId = %d", measId, rptCfgId);
  /* get measurement object and Type*/   
  if(ROK !=  wrUmmMeasFindMeasObjAndTyp(&measObjInfo, measObjId, ueCb->measCfg))
  {
     RLOG0(L_ERROR, "Measurement object not found");
     RETVALUE(RFAILED);
  }

  /* get report config type */
  if( ROK != wrUmmMeasFindRptCfgTyp(rptCfgId, &rptCfgTyp,
             &measUeCb->rptCfgLst))
  {
     RLOG0(L_ERROR, "Ue report config object not found");
     RETVALUE(RFAILED);
  }

  RLOG1(L_DEBUG, "measObjInfo.nrType = %d", measObjInfo.nrType);

  if(measObjInfo.nrType == WR_UMM_MEAS_UTRA_FDD_FREQ)
  {
     measRptInfo->arfcn.choice = WR_UMM_MEAS_UTRA_FDD_FREQ;
     measRptInfo->arfcn.t.carrierFreq = measObjInfo.utraFddObj->utraMeasObj->arfcn;
     measRptInfo->cellTyp = WR_UMM_MEAS_UTRA_FDD_FREQ;
     measRptInfo->rptCfgType = rptCfgTyp;
     measRptInfo->duplexTyp = measObjInfo.utraFddObj->utraMeasObj->duplexMode;
  }

  else if(measObjInfo.nrType == WR_UMM_MEAS_UTRA_TDD_FREQ)
  {
     measRptInfo->arfcn.choice = WR_UMM_MEAS_UTRA_TDD_FREQ;
     measRptInfo->arfcn.t.carrierFreq = measObjInfo.utraTddObj->utraMeasObj->arfcn;
     measRptInfo->cellTyp = WR_UMM_MEAS_UTRA_TDD_FREQ;
     measRptInfo->rptCfgType = rptCfgTyp;
     measRptInfo->duplexTyp = measObjInfo.utraTddObj->utraMeasObj->duplexMode;
  }
  else if(measObjInfo.nrType == WR_UMM_MEAS_1XCDMA)
  {
     measRptInfo->arfcn.choice = WR_UMM_MEAS_1XCDMA;
     measRptInfo->arfcn.t.measObjCDMA2000.bandClass  = 
          measObjInfo.cdmaObj->cdmaMeasObj->bandClass;
     measRptInfo->arfcn.t.measObjCDMA2000.arfcn  = 
        measObjInfo.cdmaObj->cdmaMeasObj->arfcn;
     measRptInfo->cellTyp   = WR_UMM_MEAS_1XCDMA;
     measRptInfo->rptCfgType = rptCfgTyp;
  }
  else if(measObjInfo.nrType == WR_UMM_MEAS_CDMA_HRPD)
  {
     measRptInfo->arfcn.choice = WR_UMM_MEAS_CDMA_HRPD;
     measRptInfo->arfcn.t.measObjCDMA2000.bandClass  = 
         measObjInfo.cdmaObj->cdmaMeasObj->bandClass;
     measRptInfo->arfcn.t.measObjCDMA2000.arfcn = 
        measObjInfo.cdmaObj->cdmaMeasObj->arfcn;
     measRptInfo->cellTyp   = WR_UMM_MEAS_CDMA_HRPD;
     measRptInfo->rptCfgType = rptCfgTyp;
  }
  else if(WR_UMM_MEAS_GERAN_FREQ == measObjInfo.nrType)
  {
     measRptInfo->cellTyp = WR_UMM_MEAS_GERAN_FREQ;
     measRptInfo->rptCfgType = rptCfgTyp;
  }
  else
  {
     measRptInfo->cellTyp = WR_UMM_MEAS_INTER_FREQ;
     measRptInfo->arfcn.choice = WR_UMM_MEAS_INTER_FREQ;
     measRptInfo->arfcn.t.carrierFreq = 
        measObjInfo.eutraObj->eutraMeasObj->carrFreq;
     measRptInfo->rptCfgType = rptCfgTyp;     
  }
  RETVALUE(ROK);
}

/** @brief This function is responsible for fill Measurment trans
 *
 * @details
 *
 *     Function: wrUmmMeasFillCreateMeasTrans 
 *
 *         Processing steps:
 *         - fill RRC Meas Trans
 *
 * @param [in] UeCb, MeasObjId ueCb and measurment object id
 * @param[out] ROK : 
 * @return S16
 *      -# Success  : ROK
 *      -# Failure    : RFAILED
 */
PRIVATE S16 wrUmmMeasFillCreateMeasTrans(WrUmmIntMeasTrans *createMeasTran, WrUeCb *ueCb, 
                                                       WrMeasUeCbUtraMeasObjNode *ueCbUMeasObj)
{
  TRC2(wrUmmMeasFillCreateMeasTrans);
  createMeasTran->numUtra = 1;
  createMeasTran->utraNode->arfcn = ueCbUMeasObj->utraMeasObj->arfcn;
  createMeasTran->utraNode->cellCbMeasObj = ueCbUMeasObj->utraMeasObj;
  createMeasTran->utraNode->addCellCnt   = 0;
  createMeasTran->utraNode->delCellCnt   = 0;
  RETVALUE(ROK);
} /*end of wrUmmMeasFillCreateMeasTrans*/



/** @brief This function is responsible for fill rrc reconfig message and send to ue 
 *
 * @details
 *
 *     Function: wrUmmSendRrcReconfigToUe 
 *
 *         Processing steps:
 *         - fill RRC Reconfig message and send to UE 
 *
 * @param [in] UeCb, MeasObjId ueCb and measurment object id
 * @param[out] ROK : physical cell id      
 * @return S16
 *      -# Success  : ROK
 *      -# Failure    : RFAILED
 */
PRIVATE S16 wrUmmSendRrcReconfigToUe(WrUeCb *ueCb, WrMeasUeCbUtraMeasObjNode *ueCbUMeasObj)
{
   WrUmmIntMeasTrans         *createMeasTrans;
   WrUmmInternalMsg          *internalMsg = NULLP;
   WrUmmIncMsg               *msg = NULLP;
   WrUmmCellCb               *cellCb = NULLP;

   TRC2(wrUmmSendRrcReconfigToUe); 

   WR_ALLOC(&createMeasTrans, sizeof(WrUmmIntMeasTrans));
   if (NULLP == createMeasTrans)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   wrUmmMeasFillCreateMeasTrans(createMeasTrans, ueCb, ueCbUMeasObj);
   if (ROK != wrUmmBldInternalMsg(&internalMsg, WR_UMM_INTMSG_MEAS_TRANS, createMeasTrans))
   {
      RLOG0(L_ERROR, "building internal message error. handover not triggered");
      RETVALUE(RFAILED);          
   }
   WR_ALLOC(&msg, sizeof(*msg));
   if (msg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   msg->refCnt = 1;
   msg->msgTyp = WR_UMM_INTERNAL_MSG;
   msg->u.internalMsg = internalMsg;
   /* Get UMM CELL CB.*/
   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if (cellCb == NULLP)
   {
      RLOG0(L_ERROR, "Failed To Get UMM Cell Cb.");
      RETVALUE(RFAILED);
   }
   wrUmmPrcIncMsg(cellCb, ueCb, msg);
   RETVALUE(ROK);
}/*end of wrUmmSendRrcReconfigToUe*/


/** @brief This function is responsible for finding is any of the ue has configuread for 
 *        measurment if correspoinding freq parater is modified trigger reconfiguration to that UE
 *
 * @details
 *
 *     Function: wrUmmTrgRrcReCfgForAllUesCfg 
 *
 *         Processing steps:
 *         - scan through all the Ue's find is any of the ue is configuread freq which is part of modified   
 *         - if pci matches return 
 *
 * @param [in] neignEnb : neighbour eNodeB Control block
 * @param[out] pci : physical cell id      
 * @return S16
 *      -# Success  : ROK
 *      -# Failure    : RFAILED
 */
PUBLIC S16 wrUmmTrgRrcReCfgForAllUesCfg(WrUmmCellCb *cellCb, U32 utraModFreq)
{
   U16                         ueCnt;
   CmLList                     *measObjUtraNode;
   CmLListCp                   *utraMeasObjLst;
   WrMeasUeCbUtraMeasObjNode   *ueCbUMeasObj;
   U8                           measObjId = 0;

   TRC2(wrUmmTrgRrcReCfgForAllUesCfg);

   for(ueCnt = 0; ueCnt < cellCb->maxUesPerCell; ueCnt++)
   {
      WrUeCb                 *ueCb;
      ueCb = cellCb->ueCbs[ueCnt];
      if(NULLP != ueCb)
      {
         utraMeasObjLst = &ueCb->measCfg->utraFddMeasObjLst; 
         measObjUtraNode = utraMeasObjLst->first;
         while(NULLP != measObjUtraNode)
         {
            ueCbUMeasObj = ((WrMeasUeCbUtraMeasObjNode*)measObjUtraNode->node);
            if(ueCbUMeasObj->utraMeasObj->arfcn == utraModFreq)
            { 
               measObjId = ueCbUMeasObj->measObjId;
               wrUmmSendRrcReconfigToUe(ueCb, ueCbUMeasObj);
               break; 
            }
            measObjUtraNode = measObjUtraNode->next;
         }
      }
   }
   RETVALUE(ROK);
} /*end of wrUmmTrgRrcReCfgForAllUesCfg*/ 


/** @brief This function is called when a new Transaction is created for any
 * other procedure. The new transaction is passed to this function and this
 * function determines the next course of action based on the new transaction
 *
 * @details
 *
 *     Function: wrUmmMeasNewTransProcessor
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

PUBLIC S16 wrUmmMeasNewTransProcessor
(
 WrUmmTransCb                *transCb,
 WrUmmTransCb                *incTrans
 )

{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmCellCb                  *cellCb;
   S16                        ret = WR_UMM_TRANS_OK;


   TRC2(wrUmmMeasNewTransProcessor)

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, ueCb->cellId, "Cell Cb is NULL ");
      RETVALUE(RFAILED);
   }

   switch(incTrans->transTyp)
   {
      case WR_UMM_RRC_REESTAB_TRANS:
         {
            RLOG0(L_ERROR, "wrUmmMeasNewTransProcessor: Reestablishment received "
                  " and closing Measurement transaction");

            /* All the Measurement Id, Measurement Object won't be deleted at the UE 
             * due to re-establishment. Update the measurement gap 
             * flag in the RRC Reestablishment to configure the gap upon successful 
             * Re-establishment. */
            incTrans->u.rrcReEstabTransCb.measGapStatus = WR_UMM_MEAS_GAP_INIT_SCH_CONFIG; 

            wrUmmTransComplete(transCb);
            ret = WR_UMM_TRANS_OK;
            break;
         }

      default:
         break;
   }

   RETVALUE(ret);
}

PUBLIC Void wrUmmPerdStrongCellMeasCfg
(
WrUeCb                 *ueCb,
U8                     measObjId,
LwrCellMeasCfgGrp      *measCellCfg,
WrUmmMeasAddInfo       *addMeasInfo
)
{
//   Bool                        ueCapSupport = TRUE;
   WrUmmMeasRptCfgInfo         rptCfgInfo;

   TRC2(wrUmmPerdStrongCellMeasCfg);
/* Initiate periodic strong cell measurement configuration only if UE 
   - has valid UE Capabilities. 
   - If CSFB is NOT ongoing for that UE*/
   {
      rptCfgInfo.rptCfgTyp = WR_UMM_MEAS_STRONG_CELL;
      rptCfgInfo.thrshldTyp = WR_UMM_MEAS_THRESHOLD_RSRP;
      rptCfgInfo.cdma2kCellType = WR_UMM_MEAS_CELL_TYPE_NONE;
      rptCfgInfo.intRptCfgType = WR_UMM_MEAS_RPTCFG_INTRA;
      /* This flag is added to configure the measurement interval received from BRDCM-SON */
      ueCb->isMeasCfgForPerdStrongCellRptCfg = TRUE;
      if(ROK != wrUmmMeasRptAndMeasIdCfg(WR_UMM_MEAS_TYPE_PERD_STRONG_CELL, measObjId,
               &rptCfgInfo, ueCb, measCellCfg, addMeasInfo))
      {
         ueCb->isMeasCfgForPerdStrongCellRptCfg = FALSE;
         RLOG0(L_ERROR, "Invalid report and Meas config Id");
         printf("Invalid report and Meas config Id");
         RETVOID;
      }
   }
   cmMemset((U8*)&ueCb->perdStrongCellMeasInfo, 0 , sizeof(WrUmmMeasAddInfo));
   /* stroing measurement info into UeCb for copying into periodic transaction CB */
   /* periodic transaction CB is created after receiving the measurement report 
      for this particular periodic strong cell measurement Id*/
   ueCb->perdStrongCellMeasInfo = *addMeasInfo;
   ueCb->isMeasCfgForPerdStrongCellRptCfg = FALSE;
   RETVOID;
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
