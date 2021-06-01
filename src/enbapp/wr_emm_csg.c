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

     Desc:     This file contains the APIs to validate, update and 
               access CSG information.

     File:     wr_emm_csg.c

     Sid:      fc_emm_csg.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/7 - Mon Feb 15 12:51:22 2016

     Prg:      Sriky 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="EMM";
static int RLOG_MODULE_ID=1;
static int RLOG_FILE_ID=126;

/* header include files (.h) */
#include "wr.h"
#include "wr_cmn.h"
#include "wr_smm_smallcell.h"
#include "wr_emm.h"
#include "wr_lwr.h"
#include "wr_emm_cfg.h"
#include "wr_emm_neighenb.h"
#include "wr_emm_mme.h"
#include "wr_umm.h"
#include "wr_emm_pbm.h"
#include "wr_umm_ho.h"

/** @brief This function dervies the cell access mode.
 *
 * @details
 *
 *     Function: wrEmmCsgSetNbrAccessMode
 *
 *         Processing steps:
 *         - check if the PCI falls under the CSG PCI range and
 *           if CSG ID is present then
 *           - mark as CLOSED ACCESS
 *         - else check if the PCI falls under the small cell PCI range and
 *           if CSG ID is present then
 *           - mark as HYBRID ACCESS
 *         - else if CSG ID is not present then
 *           - mark as OPEN ACCESS
 *
 * @param [in] cellId : serving cell Cell Identity
 * @param [in] pci    : Neighbor Cell PCI
 * @param [in] isCsgIdPres : indicate if CSG id is already known
 * @param [out] accessMode : Cell Access Mode
 * @return S16
 *          -# Success : ROK 
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrEmmCsgSetNbrAccessMode
(
U16                          cellId,
U16                          nbrPci,
Bool                         isCsgIdPres,
U8                           *accessMode
)
{
   WrCellCb                  *cellCb  = NULLP;
   WrSib4Params              *sib4;
   U16                       csgMinPci;
   U16                       csgMaxPci;
   U16                       smCellMinPci;
   U16                       smCellMaxPci;
   U16                       pciRange;

   if(FALSE == isCsgIdPres)
   {
      (*accessMode) = WR_ENB_OPEN_ACCESS;
      RETVALUE(ROK);
   }

   WR_GET_CELLCB(cellCb, cellId);
   if (NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
   
   sib4 = &cellCb->sib4;
   csgMinPci = sib4->csgPhyCellIdRange.startPhyCellID;
   WR_EMM_GET_PCI_RANGE(sib4->csgPhyCellIdRange.phyCellIdRange, pciRange);
   csgMaxPci = csgMinPci + pciRange;

   smCellMinPci = cellCb->smallCellCsgPciRange.startPci;
   smCellMaxPci = smCellMinPci + cellCb->smallCellCsgPciRange.pciRange;

   switch(cellCb->csgAccessMode)
   {
      case WR_ENB_HYBRID_ACCESS:
      case WR_ENB_CLOSED_ACCESS:
      {
         if(nbrPci >= csgMinPci && nbrPci <= csgMaxPci)
         {
            (*accessMode) = WR_ENB_CLOSED_ACCESS;
         }
         else if(nbrPci >= smCellMinPci && nbrPci <= smCellMaxPci)
         {
            (*accessMode) = WR_ENB_HYBRID_ACCESS;
         }
         else
         {
            /* Though the neighbor PCI falls outside the CSG and smallcell 
             * range, if CSG ID is present then mark as Hybrid access */
            (*accessMode) = WR_ENB_HYBRID_ACCESS;
         }
      }
      break;

      case WR_ENB_OPEN_ACCESS:
      {
         if((sib4->csgCellInfoPres == TRUE) &&
               (nbrPci >= csgMinPci && nbrPci <= csgMaxPci))
         {
            (*accessMode) = WR_ENB_CLOSED_ACCESS;
         }
         else if ((cellCb->smallCellCsgPciRange.pciRangeInfoPres == TRUE) &&
               (nbrPci >= smCellMinPci && nbrPci <= smCellMaxPci))
         {
            (*accessMode) = WR_ENB_HYBRID_ACCESS;
         }
         else
         {
            /* Though the neighbor PCI falls outside the CSG and smallcell 
             * range, if CSG ID is present then mark as Hybrid access */
            (*accessMode) = WR_ENB_HYBRID_ACCESS;
         }
      }
      break;

      default:
      {
         (*accessMode) = WR_ENB_OPEN_ACCESS;
      }
      break;
   }

   RLOG2(L_INFO, "Access Mode Derived for PCI: %d = %d", nbrPci, (*accessMode));

   RETVALUE(ROK);
} /* end of wrEmmCsgSetNbrAccessMode */

/** @brief This function returns if serving cell is a CSG cell.
 *
 * @details
 *
 *     Function: wrEmmIsCsgCell
 *
 *         Processing steps:
 *         - fetch the cellCb corresponding to the cellId,
 *         - if cell access mode is not OPEN access
 *           - return TRUE,
 *         - else,
 *           - return FALSE
 *
 * @param [in]  cellId : Cell Identity
 *
 * @return Bool
 *          -# TRUE  : CSG configured
 *          -# FALSE : CSG not configured
 */
PUBLIC Bool wrEmmIsCsgCell
(
U16                          cellId
)
{
   WrCellCb                  *cellCb   = NULLP;

   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   RETVALUE((WR_ENB_OPEN_ACCESS != cellCb->csgAccessMode)? TRUE : FALSE);
} /* end of wrEmmIsCsgCell */

/** @brief This function returns the CSG ID of serving cell.
 *
 * @details
 *
 *     Function: wrEmmGetCsgIdFrmCellId
 *
 *         Processing steps:
 *         - fetch the cellCb corresponding to the cellId,
 *         - return CSG ID from Cell CB
 *
 * @param [in]  cellId : Cell Identity
 *
 * @return U32 CSG ID : CSG Identity
 */
PUBLIC U32 wrEmmGetCsgIdFrmCellId
(
U16                          cellId
)
{
   WrCellCb                  *cellCb   = NULLP;

   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   RETVALUE(cellCb->sib1.csgId.val);
} /* end of wrEmmGetCsgIdFrmCellId */

/** @brief This function returns the Access Mode of serving cell.
 *
 * @details
 *
 *     Function: wrEmmGetAccessModeFrmCellId
 *
 *         Processing steps:
 *         - fetch the cellCb corresponding to the cellId,
 *           - return access mode
 *
 * @param [in]  cellId : Cell Identity
 *
 * @return U8   accessMode : Cell Access Mode
 */
PUBLIC U8 wrEmmGetAccessModeFrmCellId
(
U16                          cellId
)
{
   WrCellCb                  *cellCb   = NULLP;

   WR_GET_CELLCB(cellCb, cellId)
   if (NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   RETVALUE(cellCb->csgAccessMode);
} /* end of wrEmmGetAccessModeFrmCellId */

/** @brief This function stores the CSG information from configuration
 *         in to cell CB.
 *
 * @details
 *
 *     Function: wrEmmCsgCopyCsgCfg
 *
 *         Processing steps:
 *         - store the cell access mode
 *         - if PCI range is present,
 *             store the PCI range
 *
 * @param [out] cellCb : Cell Control Block
 * @param [in]  cfgAdd : Cell Configuration
 * @return S16  ROK
 */
PUBLIC S16 wrEmmCsgCopyCsgCfg
(
WrCellCb                     *cellCb,
LwrAddCellCfg                *cfgAdd
)
{
   cellCb->csgAccessMode = cfgAdd->csgAccessMode;
   if(cfgAdd->smallCellCsgPciRange.pciRangeInfoPres)
   {
      cellCb->smallCellCsgPciRange.pciRangeInfoPres = TRUE;
      cellCb->smallCellCsgPciRange.startPci = 
         cfgAdd->smallCellCsgPciRange.startPci;
      cellCb->smallCellCsgPciRange.pciRange = 
         cfgAdd->smallCellCsgPciRange.pciRange;
   }
   else
   {
      cellCb->smallCellCsgPciRange.pciRangeInfoPres = FALSE;
   }
   /* HCSG_DEV Start */
   if(cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS)
   {
      cellCb->minDlResNonCsg = cfgAdd->hcsgParam.minDlResNonCsg;
      cellCb->minUlResNonCsg = cfgAdd->hcsgParam.minUlResNonCsg;
      cellCb->maxCsgUeSup    = cfgAdd->hcsgParam.maxCsgUeSup;
      cellCb->maxNonCsgUeSup = cfgAdd->hcsgParam.maxNonCsgUeSup;
   }
   /* HCSG_DEV End  */

   RETVALUE(ROK);
} /* end of wrEmmCsgCopyCsgCfg */

/** @brief This function stores the CSG information from EUTRAN neighbor add
 *         configuration into EUTRAN neighbor cell CB.
 *
 * @details
 *
 *     Function: wrEmmNbrCfgCopyCsgCfg
 *
 *         Processing steps:
 *         - store the information from Lwr configuration interface to
 *           EUTRAN neighbor cell CB.
 *
 * @param [out] eutNbrcellCb : EUTRAN neighbor Cell Control Block
 * @param [in]  nbrCellCfg   : EUTRAN neighbor add configuration
 * @return S16  ROK
 */
PUBLIC S16 wrEmmNbrCfgCopyCsgCfg
(
WrEutranNeighCellCb          *eutNbrcellCb,
LwrNeighCellAddCfg           *nbrCellCfg
)
{
   eutNbrcellCb->isCsgAccessModeResolved =
      nbrCellCfg->t.eutraCell.isCsgAccessModeResolved;

   eutNbrcellCb->csgAccessMode = nbrCellCfg->t.eutraCell.csgAccessMode;

   eutNbrcellCb->isCsgIdResolved = 
      nbrCellCfg->t.eutraCell.isCsgIdResolved;

   eutNbrcellCb->csgId = nbrCellCfg->t.eutraCell.csgId;

   RETVALUE(ROK);
} /* end of wrEmmNbrCfgCopyCsgCfg */

/** @brief This function stores the CSG information to neighbor cell CB
 *         from CGI measurement report.
 *
 * @details
 *
 *     Function: wrEmmCsgNbrUpdCsgInfoFrmExtnIe
 *
 *         Processing steps:
 *         - derive the csg id and store in neighbor cell cb
 *
 * @param [in]  csgIdIe         : CGI Identity IE
 * @param [out] eutranNrCellCb  : EUTRAN neighbor cell CB
 * @return S16
 *          -# Success : ROK 
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrEmmCsgNbrUpdCsgInfoFrmExtnIe
(
TknBStr32                    *csg_Id,
WrEutranNeighCellCb          *eutranNrCellCb
)
{
   U32                       csgIdentity;

   wrUtlGetU32FrmBStr32(&csgIdentity, csg_Id);
   eutranNrCellCb->isCsgIdResolved = WR_EMM_NBR_CSG_ID_DISCOVERED;
   eutranNrCellCb->csgId = csgIdentity;

   RLOG3(L_DEBUG, "Updated NBR CSG Information: PCI [%d] CSG ID [%d] "
         "AccessMode[%d]", eutranNrCellCb->pci, eutranNrCellCb->csgId,
         eutranNrCellCb->csgAccessMode);

   RETVALUE(ROK);
} /* end of wrEmmCsgNbrUpdCsgInfoFrmExtnIe */

/** @brief This function displays the Serving cell CSG info.
 *
 * @details
 *
 *     Function: wrUmmCsgPrntCellInfo
 *
 *         Processing steps:
 *         - fetch the cellCb corresponding to the cellId
 *           - Print the CSG info
 *
 * @param [in]  cellId  : Cell Identity
 *
 * @return S16
 *          -# Success : ROK 
 */
PUBLIC S16 wrUmmCsgPrntCellInfo
(
U16                          cellId
)
{
   WrCellCb                  *cellCb     = NULLP;
   U16                       csgMaxPci;
   U16                       pciRange;

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId,
            "Error in getting CELL CB");
   }
   else
   {
      RLOG_ARG0(L_INFO, DBG_CELLID, (U32)cellId, "Display Serving Cell CSG Info");
      RLOG_ARG4(L_INFO, DBG_CELLID, (U32)cellId, "ECGI [%d] PCI [%d] "
            "AccessMode [%d] CSG ID [%d]", cellCb->sib1.cellIdentity,
            cellCb->physCellId, cellCb->csgAccessMode, cellCb->sib1.csgId.val);
      if(TRUE == cellCb->sib4.csgCellInfoPres)
      {
         WR_EMM_GET_PCI_RANGE(cellCb->sib4.csgPhyCellIdRange.phyCellIdRange,
               pciRange);
         csgMaxPci = cellCb->sib4.csgPhyCellIdRange.startPhyCellID + pciRange;
         RLOG_ARG4(L_INFO, DBG_CELLID, (U32)cellId, "CSG PCI Start [%d] "
               "CSG PCI Max [%d] SmallCell PCI Start [%d] SmallCell "
               "PCI Max [%d]", cellCb->sib4.csgPhyCellIdRange.startPhyCellID,
               csgMaxPci, cellCb->smallCellCsgPciRange.startPci,
               (cellCb->smallCellCsgPciRange.startPci + 
                cellCb->smallCellCsgPciRange.pciRange));
      }
      else
      {
         RLOG_ARG2(L_INFO, DBG_CELLID, (U32)cellId, "CSG PCI NOT Configured. "
               "SmallCell PCI Start [%d] SmallCell PCI Max [%d]",
               cellCb->smallCellCsgPciRange.startPci,
               (cellCb->smallCellCsgPciRange.startPci + 
                cellCb->smallCellCsgPciRange.pciRange));
      }
   }

   RETVALUE(ROK);
} /* end of wrUmmCsgPrntNbrCsgInfo */

/** @brief This function displays the CSG info of all the configured neighbor
 *         nodes.
 *
 * @details
 *
 *     Function: wrUmmCsgPrntNbrCsgInfo
 *
 *         Processing steps:
 *         - fetch the neighbor list belonging to serving cell frequency
 *           - Print the CSG ID and membership status of all the nodes
 *           in UE membership status list.
 *
 * @param [in]  cellId  : Cell Identity
 *
 * @return S16
 *          -# Success : ROK 
 */
PUBLIC S16 wrUmmCsgPrntNbrCsgInfo
(
U16                          cellId
)
{
   WrCellCb                  *cellCb     = NULLP;
   WrEutranNeighCellCb       *nbrCellCb  = NULLP;
   CmLList                   *nrLnk      = NULLP;
   WrNrEutranFreqCb          *freqNode   = NULLP;

   WR_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)cellId,
            "Error in getting CELL CB");
   }
   else
   {
      freqNode = wrEmmAnrGetEutranFreqNode(cellCb->dlCarrierFreq, 
            &(cellCb->nrCb->eutranCb));
      if(freqNode != NULLP)
      {
         nrLnk = freqNode->nrWlLst.first;
         while(nrLnk != NULLP)
         {
            nbrCellCb = (WrEutranNeighCellCb *)nrLnk->node;
            RLOG_ARG0(L_INFO, DBG_CELLID, (U32)cellId,
                  "Display Neighbor Cell CSG Info");
            RLOG4(L_INFO, "ECGI [%d] PCI [%d] AccessMode [%d] CSG ID [%d]",
                  nbrCellCb->eCgi.eutranCellId, nbrCellCb->pci,
                  nbrCellCb->csgAccessMode, nbrCellCb->csgId);
            nrLnk = nrLnk->next;        
         }
      }
   }

   RETVALUE(ROK);
} /* end of wrUmmCsgPrntNbrCsgInfo */

/** @brief This function adds the given CSG ID into the membership 
 *         status list.
 *
 * @details
 *
 *     Function: wrUmmCsgAddCsgIdIntoMbrLst
 *
 *         Processing steps:
 *         - Add csgId to member list
 *
 * @param [in]  ueCb  : UE Control Block
 * @param [in]  csgId : CSG Identity
 *
 * @return S16
 *          -# Success : ROK 
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrUmmCsgAddCsgIdIntoMbrLst
(
WrUeCb                       *ueCb,
U32                          csgId,
U8                           mbrShpSts
)
{
   WrCsgMbrEntry             *csgMbrShpSts = NULLP;

   WR_ALLOC(&csgMbrShpSts, sizeof(WrCsgMbrEntry));
   if(NULLP == csgMbrShpSts)
   {
      RLOG0(L_FATAL, "Memory allocaion failed.");
      RETVALUE(RFAILED);
   }

   csgMbrShpSts->csgId     = csgId;
   csgMbrShpSts->mbrStatus = mbrShpSts;

   if(RFAILED == cmHashListInsert(&(ueCb->csgMbrLst),
            (PTR)csgMbrShpSts, (U8 *)&csgMbrShpSts->csgId, sizeof(U32)))
   {
      RLOG2(L_ERROR,"Failed to Update UE membership status CSG ID "
            "[%d] membership status [%d]",
            csgId, csgMbrShpSts->mbrStatus);
      WR_FREE(csgMbrShpSts, sizeof(WrCsgMbrEntry));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
} /* end of wrUmmCsgAddCsgIdIntoMbrLst */

/** @brief This function updates the UE membership status for the 
 *         corresponding CSG ID.
 *
 * @details
 *
 *     Function: wrUmmCsgModMbrStsForCsgId
 *
 *         Processing steps:
 *         - fetch the membership status node for the given csgId,
 *           - update the membership status
 *
 * @param [in]  ueCb  : UE Control Block
 * @param [in]  csgId : CSG Identity
 * @param [in]  mbrShpSts : UE Membership status
 *
 * @return S16
 *          -# Success : ROK 
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrUmmCsgModMbrStsForCsgId
(
WrUeCb                       *ueCb,
U32                          csgId,
U8                           mbrShpSts
)
{
   WrCsgMbrEntry             *csgMbrShpSts = NULLP;

   if(RFAILED == cmHashListFind(&(ueCb->csgMbrLst), (U8*) &csgId, sizeof(csgId),
            0, (PTR *)&csgMbrShpSts))
   {
      RLOG1(L_ERROR, "Could not find CSG membership status for csgId [%d]", 
            csgId);
      RETVALUE(RFAILED);
   }

   csgMbrShpSts->mbrStatus = mbrShpSts;

   RETVALUE(ROK);
} /* end of wrUmmCsgDelCsgIdFrmMbrLst */

/** @brief This function removes the CSG ID from membership status list.
 *
 * @details
 *
 *     Function: wrUmmCsgDelCsgIdFrmMbrLst
 *
 *         Processing steps:
 *         - fetch the membership status node for the given csgId,
 *           - return the membership status
 *
 * @param [in]  ueCb  : UE Control Block
 * @param [in]  csgId : CSG Identity
 *
 * @return S16
 *          -# Success : ROK 
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrUmmCsgDelCsgIdFrmMbrLst
(
WrUeCb                       *ueCb,
U32                          csgId
)
{
   WrCsgMbrEntry             *csgMbrShpSts = NULLP;

   if(RFAILED == cmHashListFind(&(ueCb->csgMbrLst), (U8*) &csgId, sizeof(csgId),
            0, (PTR *)&csgMbrShpSts))
   {
      RLOG1(L_ERROR, "Could not find CSG membership status for csgId [%d]", 
            csgId);
      RETVALUE(RFAILED);
   }
   cmHashListDelete(&(ueCb->csgMbrLst), (PTR)csgMbrShpSts);
   WR_FREE(csgMbrShpSts, sizeof(WrCsgMbrEntry));

   RETVALUE(ROK);
} /* end of wrUmmCsgDelCsgIdFrmMbrLst */

/** @brief This function adds the given CSG ID into the CSG black list.
 *
 * @details
 *
 *     Function: wrUmmCsgAddCsgIdIntoBlckLst
 *
 *         Processing steps:
 *         - Add csgId to CSG black list
 *
 * @param [in]  ueCb  : UE Control Block
 * @param [in]  csgId : CSG Identity
 *
 * @return S16
 *          -# Success : ROK 
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrUmmCsgAddCsgIdIntoBlckLst
(
WrUeCb                       *ueCb,
U32                          csgId,
U16                          pci
)
{
   WrCsgNbrBlLst             *nbrBlNode;

   WR_ALLOC(&nbrBlNode, sizeof(WrCsgNbrBlLst));
   if(NULLP == nbrBlNode)
   {
      RLOG0(L_FATAL, "Memory allocaion failed.");
      RETVALUE(RFAILED);
   }
   nbrBlNode->lnk.node = (PTR)nbrBlNode;
   nbrBlNode->csgId = csgId;
   nbrBlNode->pci   = pci;
   cmLListAdd2Tail(&(ueCb->csgNbrBlLst), &nbrBlNode->lnk);

   RETVALUE(ROK);
} /* end of wrUmmCsgAddCsgIdIntoBlckLst */

/** @brief This function returns the CSG membership status for a corresponding 
 *         CSG ID from UE membership status list.
 *
 * @details
 *
 *     Function: wrUmmCsgIsUeMember
 *
 *         Processing steps:
 *         - fetch the membership status node for the given csgId,
 *           - return the membership status
 *
 * @param [in]  ueCb  : UE Control Block
 * @param [in]  csgId : CSG Identity
 *
 * @return U8 mbrStatus : CSG membership status for the given CSG ID
 */
PUBLIC U8 wrUmmCsgIsUeMember
(
WrUeCb                       *ueCb,
U32                          csgId
)
{
   WrCsgMbrEntry             *csgMbrShpSts = NULLP;

   if(RFAILED == cmHashListFind(&(ueCb->csgMbrLst), (U8*) &csgId, sizeof(csgId),
            0, (PTR *)&csgMbrShpSts))
   {
      RLOG1(L_INFO, "Could not find CSG membership status for csgId [%d]", 
            csgId);
      RETVALUE(WR_UMM_CSG_MEMBERSHIP_NOT_KNOWN);
   }

   RETVALUE(csgMbrShpSts->mbrStatus);
} /* end of wrUmmCsgIsUeMember */

/** @brief This function displays the CSG IDs and the corrosponding membership
 *         status from UE CB.
 *
 * @details
 *
 *     Function: wrUmmCsgPrntUeMbrShpLst
 *
 *         Processing steps:
 *         - Print the CSG ID and membership status of all the nodes
 *           in UE membership status list.
 *
 * @param [in]  ueCb  : UE Control Block
 *
 * @return S16
 *          -# Success : ROK 
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrUmmCsgPrntUeMbrShpLst
(
WrUeCb                       *ueCb
)
{
   WrCsgMbrEntry             *prevCsgMbr = NULLP;
   WrCsgMbrEntry             *csgMbr     = NULLP;

   while(ROK == cmHashListGetNext(&(ueCb->csgMbrLst), (PTR)prevCsgMbr,
      (PTR *)&csgMbr))
   {
      RLOG2(L_INFO, "CSG ID [%d] membership status [%d]", csgMbr->csgId,
            csgMbr->mbrStatus);
      prevCsgMbr = csgMbr;
   }

   RETVALUE(ROK);
} /* end of wrUmmCsgPrntUeMbrShpLst */

/** @brief This function stores the CSG information to neighbor cell CB
 *         from CGI measurement report.
 *
 * @details
 *
 *     Function: wrUmmCsgAnrUpdtEutranCsgInfo
 *
 *         Processing steps:
 *         - if additional SI info is present in the measurement report,
 *           - store the CSG ID in EUTRAN neighbor cell CB,
 *           - derive cell access mode for the neighbor CB and store in 
 *             neighbor cell CB
 *           - store the membership status in UE CB,
 *
 * @param [in]  measResultEUTRA : EUTRAN CGI measurement report
 * @param [out] eutranNrCellCb  : EUTRAN neighbor cell CB
 * @return S16
 *          -# Success : ROK 
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrUmmCsgAnrUpdtEutranCsgInfo
(
WrUeCb                       *ueCb,
NhuMeasResults               *measRprt,
U16                          carrFreq
)
{
   WrCellCb                  *cellCb;
   WrEutranNeighCellCb       *eutranNrCellCb;
   NhuMeasResultEUTRA        *measResultEUTRA;
   NhuAdditionalSI_Info_r9   *additnlSiInfo_r9;
   WrEutranNbrCellInfo       eutraNrInfo;
   U32                       csgIdentity;
   S16                       ret;

   if(ueCb->ueCap->eUTRA_Cap.accessStratumRls.val ==
         NhuAccessStratumRlsrel8Enum)
   {
      RLOG0(L_INFO, "Ignoring Measurement Report to Update CSG Info "
            "(REL-8) UE");
      RETVALUE(ROK);
   }

   WR_GET_CELLCB(cellCb, ueCb->cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)ueCb->cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
 
   measResultEUTRA = &measRprt->measResultNeighCells.val.
                              measResultLstEUTRA.member[0];

   if(NULLP == measResultEUTRA)
   {
      RLOG0(L_WARNING, "EUTRA Measurement Result NULL.");
      RETVALUE(RFAILED);
   }

   eutraNrInfo.earfcn = carrFreq;
   eutraNrInfo.phyCellId = measResultEUTRA->physCellId.val;

   eutranNrCellCb = wrEmmAnrGetEutranNr(&cellCb->nrCb->eutranCb,
         &eutraNrInfo);
   if(NULLP != eutranNrCellCb)
   {
      /* update EUTRAN neighbor cell CB */
      if(measResultEUTRA->measResult.extaddgrp_1.pres.pres == PRSNT_NODEF)
      {
         additnlSiInfo_r9 = 
            &measResultEUTRA->measResult.extaddgrp_1.additionalSI_Info_r9;
         wrUtlGetU32FrmBSXL(&csgIdentity, &additnlSiInfo_r9->csg_Identity_r9);
         eutranNrCellCb->isCsgIdResolved = WR_EMM_NBR_CSG_ID_DISCOVERED;
         eutranNrCellCb->csgId = csgIdentity;
         if(ROK != wrEmmCsgSetNbrAccessMode(ueCb->cellId, eutranNrCellCb->pci,
                  TRUE, &eutranNrCellCb->csgAccessMode))
         {
            RLOG2(L_INFO,"Error deriving Cell Access Mode cellId [%d] pci[%d]",
                  ueCb->cellId, eutranNrCellCb->pci);
            RETVALUE(RFAILED);
         }
         eutranNrCellCb->isCsgAccessModeResolved =
            WR_EMM_CELL_ACCESS_MODE_DETERMINED;

         RLOG3(L_INFO,"Updated NBR CSG Information: PCI [%d] CSG ID [%d] "
               "AccessMode[%d]", eutranNrCellCb->pci, eutranNrCellCb->csgId,
               eutranNrCellCb->csgAccessMode);

         /* update UE membership status for this CSG ID */
         if(PRSNT_NODEF == additnlSiInfo_r9->csg_MemberStatus_r9.pres)
         {
            ret = wrUmmCsgAddCsgIdIntoMbrLst(ueCb, csgIdentity,
                  WR_UMM_CSG_MEMBER);
         }
         else
         {
            ret = wrUmmCsgAddCsgIdIntoMbrLst(ueCb, csgIdentity,
                  WR_UMM_CSG_NOT_MEMBER);
         }

         if(RFAILED == ret)
         {
            RLOG2(L_INFO,"Failed to Update UE membership status NBR CSG ID "
                  "[%d] AccessMode[%d]",
                  eutranNrCellCb->csgId, eutranNrCellCb->csgAccessMode);
            RETVALUE(RFAILED);
         }
      }
      /*Ranjith*/
      else
      {
      }
   }

   RETVALUE(ROK);
} /* end of wrUmmCsgAnrUpdtEutranCsgInfo */

/** @brief This function returns whether reportCGI should be requested for the
 *         reported PCI.
 *
 * @details
 *
 *     Function: wrEmmCsgIsReportCgiNeeded
 *
 *         Processing steps:
 *         - if inter-frequency neighbor,
 *           - return FALSE
 *         - if CSG ID is not discovered and if the reported UE is REL-9
 *           - return TRUE
 *         - if it the PCI list is for Event ANR
 *           - if CSG ID is discovered and if the reported UE is REL-9
 *             - if the membership status for that CSG ID is not known
 *               - reutrn TRUE
 *
 * @param [in]  ueIdx      : UE Index in Cell CB UE List
 * @param [in]  isEvntAnr  : If the PCI list is for Event ANR case
 * @param [in]  cellCb     : Cell Control Block
 * @param [in]  eutNbrCellCb : EUTRAN Neighbor Cell Control Block
 * @return Bool
 *          -# reportCgi Needed     : TRUE
 *          -# reportCgi Not Needed : FALSE
 */
PUBLIC Bool wrEmmCsgIsReportCgiNeeded
(
U8                           ueIdx,
U8                           isEvntAnr,
WrCellCb                     *cellCb,
WrEutranNeighCellCb          *eutNbrCellCb
)
{
   Bool                      retVal = FALSE;
   WrUeCb                    *ueCb;
   U8                        isUeMember;
   U8                        ueAccessStratRel;

   if(eutNbrCellCb->earfcnDl != cellCb->dlCarrierFreq)
   {
      RETVALUE(FALSE);
   }

   wrUmmGetUeFrmUeIdx(&ueCb, cellCb->cellId, ueIdx);
   if (ueCb == NULLP)
   {
      RLOG_ARG1(L_ERROR, DBG_CELLID,(U32)cellCb->cellId,
            "Error in getting UECB for ueIdx = %u", ueIdx);
      RETVALUE(FALSE);
   }

   ueAccessStratRel = ueCb->ueCap->eUTRA_Cap.accessStratumRls.val;

   /* common for Periodic and Event ANR */
   if((WR_EMM_NBR_CSG_ID_DISCOVERED !=
            eutNbrCellCb->isCsgIdResolved) &&
         (ueAccessStratRel > NhuAccessStratumRlsrel8Enum))
   {
      /* reportCGI is needed to get CSG ID */
      retVal = TRUE;
   }

   if((FALSE == retVal) && isEvntAnr) /* Event ANR */
   {
      if((WR_EMM_NBR_CSG_ID_DISCOVERED ==
             eutNbrCellCb->isCsgIdResolved) &&
          (ueAccessStratRel > NhuAccessStratumRlsrel8Enum) &&
          (eutNbrCellCb->csgAccessMode != WR_ENB_OPEN_ACCESS))
      {
         isUeMember = wrUmmCsgIsUeMember(ueCb, eutNbrCellCb->csgId);
         if(WR_UMM_CSG_MEMBERSHIP_NOT_KNOWN == isUeMember)
         {
            /* membership status is not known so repo;rtCGI is needed */
            retVal = TRUE;
         }
      }
   }

   RETVALUE(retVal);
} /* end of wrEmmCsgIsReportCgiNeeded */

/** @brief This function returns whether the CSG ID is blacklisted for 
 *         the UE or not.
 *
 * @details
 *
 *     Function: wrUmmCsgIsNbrBlsted
 *
 *         Processing steps:
 *         - Check the provided CSG ID with each node in csgNbrBlLst
 *           - if the CSG ID is found in the list, then
 *             - return TRUE
 *
 * @param [in]  ueCb      : UE Control Block
 * @param [in]  csgId     : CSG Identity
 * @return Bool
 *          -# CSG ID Found     : TRUE
 *          -# CSG ID Not Found : FALSE
 */
PUBLIC Bool wrUmmCsgIsNbrBlsted
(
WrUeCb                       *ueCb,
U32                          csgId,
U16                          pci
)
{
   WrCsgNbrBlLst             *nbrBlNode  = NULLP;
   CmLList                   *tmpLlist   = NULLP;

   nbrBlNode = (WrCsgNbrBlLst*)CM_LLIST_FIRST_NODE(&(ueCb->csgNbrBlLst),
         tmpLlist);
   while(NULLP != nbrBlNode)
   {
      if(csgId == nbrBlNode->csgId &&
            pci == nbrBlNode->pci)
      {
         RETVALUE(TRUE);
      }
      nbrBlNode = (WrCsgNbrBlLst*)CM_LLIST_NEXT_NODE(&(ueCb->csgNbrBlLst),
            tmpLlist);
   }

   RETVALUE(FALSE);
} /* end of wrUmmCsgIsNbrBlsted */

/** @brief This function process the neighbor CSG info and serving cell CSG info
 *         and UE membership status and decide if the neighbro is suited for
 *         handover.
 *
 * @details
 *
 *     Function: wrUmmCsgValidateNbrCsgForHo
 *
 *         Processing steps:
 *         - refer SRS section 3.2.2.7.4.1 and 3.2.2.7.4.2
 *
 * @param [in]  ueCb      : UE Control Block
 * @param [in]  nbrInfo   : Neighbor information
 * @param [out] hoStatus  : Handover decision status
 * @param [out] hoType    : Handover type
 * @return S16
 *          -# Success : ROK 
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrUmmCsgValidateNbrCsgForHo
(
WrUeCb                       *ueCb,
WrHoNghInfo                  *nbrInfo,
U8                           *hoStatus,
U8                           *hoType,
U8                           *nbrPriority
)
{
   WrUmmCellCb               *cellCb;
   U8                        mbrShpSts;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)ueCb->cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   /* if HO preparation had failed prior to this attempt */
   if((WR_ENB_OPEN_ACCESS != nbrInfo->csgCellAccessMode) && 
         (TRUE == wrUmmCsgIsNbrBlsted(ueCb, nbrInfo->csgId, nbrInfo->t.intraLteNghInfo.pci)))
   {
      RLOG2(L_DEBUG, "CSG ID [%d] PCI [%d] Blacklisted for the UE",
            nbrInfo->csgId, nbrInfo->t.intraLteNghInfo.pci);
      *hoStatus = WR_HO_DEC_FAILURE;
      RETVALUE(ROK);
   }

   /* no access restriction for emergency calls */
   if(NhuEstablishmentCauseemergencyEnum == ueCb->establishCause || 
         (ueCb->ueServTyp == WR_UMM_EMERGENCY_SERV))
   {
      WR_UMM_DECIDE_S1_OR_X2_HO_FRM_X2_AVLTY(nbrInfo, hoStatus, hoType);
      *nbrPriority = WR_UMM_HO_NBR_PRIORITY_HIGHEST;
      RETVALUE(ROK);
   }

   RLOG3(L_DEBUG, "HODBG:: processing for HO PCI: %d X2 Assoc.: %d "
         "AccessMode: %d", nbrInfo->t.intraLteNghInfo.pci,
         nbrInfo->t.intraLteNghInfo.isX2Connected,
         nbrInfo->csgCellAccessMode);

   switch (cellCb->csgAccessMode)
   {
      case WR_ENB_OPEN_ACCESS:
      {
         if(WR_ENB_OPEN_ACCESS == nbrInfo->csgCellAccessMode)
         {
            WR_UMM_DECIDE_S1_OR_X2_HO_FRM_X2_AVLTY(nbrInfo, hoStatus, hoType);
            *nbrPriority = WR_UMM_HO_NBR_PRIORITY_MORE;
         }
         else if(WR_EMM_NBR_CSG_ID_NOT_DISCOVERED ==
               nbrInfo->isCsgInfoDiscovered)
         {
            *hoStatus = WR_HO_DEC_SUCCESS;
            *hoType   = WR_HO_TYPE_INTRA_LTE_S1;
            *nbrPriority = WR_UMM_HO_NBR_PRIORITY_MAYBE;
         }
         else if(WR_ENB_CLOSED_ACCESS == nbrInfo->csgCellAccessMode)
         {
            /* open to closed - always S1 */
            mbrShpSts = wrUmmCsgIsUeMember(ueCb, nbrInfo->csgId);
            if((ueCb->ueCap->eUTRA_Cap.accessStratumRls.val < 
                     NhuAccessStratumRlsrel9Enum) || 
                  (WR_UMM_CSG_MEMBER == mbrShpSts))
            {
               *hoStatus = WR_HO_DEC_SUCCESS;
               *hoType   = WR_HO_TYPE_INTRA_LTE_S1;
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_HIGHEST;
            }
            else if(WR_UMM_CSG_MEMBERSHIP_NOT_KNOWN == mbrShpSts)
            {
               *hoStatus = WR_HO_DEC_SUCCESS;
               *hoType   = WR_HO_TYPE_INTRA_LTE_S1;
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_MAYBE;
            }
            else
            {
               *hoStatus = WR_HO_DEC_FAILURE;
            }
         }
         else
         {
            /* open to hybrid - always S1 */
            mbrShpSts = wrUmmCsgIsUeMember(ueCb, nbrInfo->csgId);
            if(WR_UMM_CSG_MEMBER == mbrShpSts)
            {
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_HIGHEST;
            }
            else if(WR_UMM_CSG_MEMBERSHIP_NOT_KNOWN == mbrShpSts)
            {
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_LOW;
            }
            else
            {
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_MAYBE;
            }
            /* Open to Hybrid Mode, always perform S1 handover */
            *hoStatus = WR_HO_DEC_SUCCESS;
            *hoType   = WR_HO_TYPE_INTRA_LTE_S1;
         }
      }
      break;

      case WR_ENB_CLOSED_ACCESS:
      {
         if(WR_ENB_OPEN_ACCESS == nbrInfo->csgCellAccessMode)
         {
            WR_UMM_DECIDE_S1_OR_X2_HO_FRM_X2_AVLTY(nbrInfo, hoStatus, hoType);
            *nbrPriority = WR_UMM_HO_NBR_PRIORITY_MORE;
         }
         else if(WR_EMM_NBR_CSG_ID_NOT_DISCOVERED == 
               nbrInfo->isCsgInfoDiscovered)
         {
            *hoStatus = WR_HO_DEC_SUCCESS;
            *hoType   = WR_HO_TYPE_INTRA_LTE_S1;
            *nbrPriority = WR_UMM_HO_NBR_PRIORITY_MAYBE;
         }
         else if(cellCb->csgId == nbrInfo->csgId)
         {
            /* when serving is closed access, connected UEs are always a member
             * hence irrespective of closed / hybrid access neighbor, X2 is
             * possible */
            WR_UMM_DECIDE_S1_OR_X2_HO_FRM_X2_AVLTY(nbrInfo, hoStatus, hoType);
            *nbrPriority = WR_UMM_HO_NBR_PRIORITY_HIGHEST;
         }
         else if(WR_ENB_CLOSED_ACCESS == nbrInfo->csgCellAccessMode)
         {
            /* if closed access neighbor and UE is a non-member */
            mbrShpSts = wrUmmCsgIsUeMember(ueCb, nbrInfo->csgId);
            if((ueCb->ueCap->eUTRA_Cap.accessStratumRls.val < 
                     NhuAccessStratumRlsrel9Enum) || 
                  (WR_UMM_CSG_MEMBER == mbrShpSts))
            {
               *hoStatus = WR_HO_DEC_SUCCESS;
               *hoType   = WR_HO_TYPE_INTRA_LTE_S1;
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_HIGHEST;
            }
            else if(WR_UMM_CSG_MEMBERSHIP_NOT_KNOWN == mbrShpSts)
            {
               *hoStatus = WR_HO_DEC_SUCCESS;
               *hoType   = WR_HO_TYPE_INTRA_LTE_S1;
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_MORE;
            }
            else
            {
               *hoStatus = WR_HO_DEC_FAILURE;
            }
         }
         else
         {
            /* neighbor and source are of different CSG ID */
            mbrShpSts = wrUmmCsgIsUeMember(ueCb, nbrInfo->csgId);
            if(WR_UMM_CSG_MEMBER == mbrShpSts)
            {
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_HIGHEST;
            }
            else if(WR_UMM_CSG_MEMBERSHIP_NOT_KNOWN == mbrShpSts)
            {
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_LOW;
            }
            else
            {
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_MAYBE;
            }
            *hoStatus = WR_HO_DEC_SUCCESS;
            *hoType   = WR_HO_TYPE_INTRA_LTE_S1;
         }
      }
      break;

      case WR_ENB_HYBRID_ACCESS:
      {
         mbrShpSts = wrUmmCsgIsUeMember(ueCb, nbrInfo->csgId);
         if(WR_ENB_OPEN_ACCESS == nbrInfo->csgCellAccessMode)
         {
            WR_UMM_DECIDE_S1_OR_X2_HO_FRM_X2_AVLTY(nbrInfo, hoStatus, hoType);
            *nbrPriority = WR_UMM_HO_NBR_PRIORITY_MORE;
         }
         else if(WR_EMM_NBR_CSG_ID_NOT_DISCOVERED == 
               nbrInfo->isCsgInfoDiscovered)
         {
            *hoStatus = WR_HO_DEC_SUCCESS;
            *hoType   = WR_HO_TYPE_INTRA_LTE_S1;
            *nbrPriority = WR_UMM_HO_NBR_PRIORITY_MAYBE;
         }
         else if((cellCb->csgId == nbrInfo->csgId) && 
               (WR_UMM_CSG_MEMBER == mbrShpSts))
         {
            /* either hybrid / closed access neighbor, if UE is a member, X2 is
             * possible */
            WR_UMM_DECIDE_S1_OR_X2_HO_FRM_X2_AVLTY(nbrInfo, hoStatus, hoType);
            *nbrPriority = WR_UMM_HO_NBR_PRIORITY_HIGHEST;
         }
         else if((WR_ENB_HYBRID_ACCESS == nbrInfo->csgCellAccessMode) && 
               (cellCb->csgId == nbrInfo->csgId))
         {
            /* if hybrid access neighbor, irrespective of UE membership, X2 is
             * possible */
            WR_UMM_DECIDE_S1_OR_X2_HO_FRM_X2_AVLTY(nbrInfo, hoStatus, hoType);
            *nbrPriority = WR_UMM_HO_NBR_PRIORITY_HIGHEST;
         }
         else if(WR_ENB_CLOSED_ACCESS == nbrInfo->csgCellAccessMode)
         {
            /* if closed access neighbor and UE is a non-member */
            if((ueCb->ueCap->eUTRA_Cap.accessStratumRls.val < 
                     NhuAccessStratumRlsrel9Enum) || 
                  (WR_UMM_CSG_MEMBER == mbrShpSts))
            {
               *hoStatus = WR_HO_DEC_SUCCESS;
               *hoType   = WR_HO_TYPE_INTRA_LTE_S1;
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_HIGHEST;
            }
            else if(WR_UMM_CSG_MEMBERSHIP_NOT_KNOWN == mbrShpSts)
            {
               *hoStatus = WR_HO_DEC_SUCCESS;
               *hoType   = WR_HO_TYPE_INTRA_LTE_S1;
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_MORE;
            }
            else
            {
               *hoStatus = WR_HO_DEC_FAILURE;
            }
         }
         else
         {
            /* neighbor and source are of different CSG ID */
            mbrShpSts = wrUmmCsgIsUeMember(ueCb, nbrInfo->csgId);
            if(WR_UMM_CSG_MEMBER == mbrShpSts)
            {
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_HIGHEST;
            }
            else if(WR_UMM_CSG_MEMBERSHIP_NOT_KNOWN == mbrShpSts)
            {
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_LOW;
            }
            else
            {
               *nbrPriority = WR_UMM_HO_NBR_PRIORITY_MAYBE;
            }
            *hoStatus = WR_HO_DEC_SUCCESS;
            *hoType   = WR_HO_TYPE_INTRA_LTE_S1;
         }
      }
      break;

      default:
      {
         RLOG1(L_WARNING, "Invalid Serving Access Mode [%d]",
               cellCb->csgAccessMode);
         *hoStatus = WR_HO_DEC_FAILURE;
      }
      break;
   }

   RLOG3(L_DEBUG, "HODBG:: processing result for HO PCI: %d hoStatus: %d "
         "hoType: %d", nbrInfo->t.intraLteNghInfo.pci,
         (*hoStatus), (*hoType));

   RETVALUE(ROK);
} /* end of wrUmmCsgValidateNbrCsgForHo */

/** @brief This function validates the CSG information present in S1AP Handover
 *         Request message.
 *
 * @details
 *
 *     Function: wrUmmCsgValidateInbndS1Ho
 *
 *         Processing steps:
 *         - if target cell is operating on CLOSED access mode
 *           - if CSG ID of source and target are not same or 
 *             if UE membership status indicates NOT_MEMBER
 *             - reject handover preparation
 *           - else,
 *             - add the CSG ID in UE member list
 *         - if target cell is operating on OPEN access mode
 *           - add the CSG ID in UE member list
 *
 * @param [in]  ueCb      : UE Control Block
 * @param [in]  nbrInfo   : Neighbor information
 * @param [out] hoStatus  : Handover decision status
 * @param [out] hoType    : Handover type
 * @return S16
 *          -# Success : ROK 
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrUmmCsgValidateInbndS1Ho
(
WrUeCb                       *ueCb,
TknU32                       csgId,
U8                           ueMbrShpSts,
U8                           isEmrgncyCall,
U8                           *causeType,
U8                           *causeVal
)
{
   WrUmmCellCb               *cellCb;
   S16                       retVal = RFAILED;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if (NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)ueCb->cellId,
            "Error in getting CELL CB");
      (*causeType) = WR_CAUSE_MISC;
      (*causeVal)  = SztCauseMiscunspecifiedEnum;
      RETVALUE(RFAILED);
   }

   switch(cellCb->csgAccessMode)
   {
      case WR_ENB_CLOSED_ACCESS:
      {
         if(csgId.pres == NOTPRSNT)
         {
            RLOG0(L_WARNING, "Mandatory IE CSG ID not present in Handover Rquest");
            (*causeType) = WR_CAUSE_RADIONW;
            (*causeVal)  = SztCauseRadioNwinvalid_CSG_IdEnum;
         }
         else if(WR_UMM_CSG_MEMBER == ueMbrShpSts)
         {
            if(csgId.val == cellCb->csgId)
            {
               /* add CSG ID to UE membership status list */
               retVal = wrUmmCsgAddCsgIdIntoMbrLst(ueCb, csgId.val,
                     WR_UMM_CSG_MEMBER);
               if(RFAILED == retVal)
               {
                  RLOG2(L_ERROR,"Failed to Update UE membership status CSG ID "
                        "[%d] membership status [%d]", csgId.val, ueMbrShpSts);
                  (*causeType) = WR_CAUSE_MISC;
                  (*causeVal)  = SztCauseMiscunspecifiedEnum;
               }
            }
            else
            {
               RLOG2(L_ERROR,"MemberShip Status Unknown and CSG ID Mismatch. "
                     "CSG ID in SIB1 [%d] CSG ID in HO REQ [%d]",
                     cellCb->csgId, csgId.val);
               (*causeType) = WR_CAUSE_RADIONW;
               (*causeVal)  = SztCauseRadioNwinvalid_CSG_IdEnum;
            }
         }
         else if(WR_UMM_CSG_NOT_MEMBER == ueMbrShpSts && isEmrgncyCall)
         {
            /* add CSG ID to UE membership status list */
            retVal = wrUmmCsgAddCsgIdIntoMbrLst(ueCb, csgId.val,
                  WR_UMM_CSG_NOT_MEMBER);
            if(RFAILED == retVal)
            {
               RLOG2(L_ERROR,"Failed to Update UE membership status CSG ID "
                     "[%d] membership status [%d]", csgId.val, ueMbrShpSts);
               (*causeType) = WR_CAUSE_MISC;
               (*causeVal)  = SztCauseMiscunspecifiedEnum;
            }
            else
            {
               RLOG3(L_ERROR, "Admitting a NON-MEMBER in CLOSED Mode for "
                     "EMERGENCY CALL CSG ID in SIB1 [%d] CSG ID in HO REQ [%d] "
                     "Member status [%d]",
                     cellCb->csgId, csgId.val, ueMbrShpSts);
            }
         }
      }
      break;

      case WR_ENB_OPEN_ACCESS:
      {
         RLOG2(L_INFO,"CSG info in OPEN Access. "
               "CSG ID in HO REQ [%d] membership status [%d]",
               csgId.val, ueMbrShpSts);
         retVal = ROK;
      }
      break;

      /* HCSG DEV start */
      case WR_ENB_HYBRID_ACCESS:
      {
         if(csgId.pres == NOTPRSNT)
         {
            RLOG0(L_ERROR,"Mandatory IE CSG ID not present in Handover Rquest");
            (*causeType) = WR_CAUSE_RADIONW;
            (*causeVal)  = SztCauseRadioNwinvalid_CSG_IdEnum;
         }
         else if(WR_UMM_CSG_MEMBER == ueMbrShpSts)
         {
            if(csgId.val == cellCb->csgId)
            {
               /* add CSG ID to UE membership status list */
               retVal = wrUmmCsgAddCsgIdIntoMbrLst(ueCb, csgId.val,
                     WR_UMM_CSG_MEMBER);
               if(RFAILED == retVal)
               {
                  RLOG2(L_ERROR,"Failed to Update UE membership status CSG ID "
                        "[%d] membership status [%d]", csgId.val, ueMbrShpSts);
                  (*causeType) = WR_CAUSE_MISC;
                  (*causeVal)  = SztCauseMiscunspecifiedEnum;
               }
            }
            else
            {
               RLOG3(L_ERROR,"CSG ID Mismatch. CSG ID in SIB1 [%d] "
                     "CSG ID in HO REQ [%d] Membership Status [%d]",
                     cellCb->csgId, csgId.val, ueMbrShpSts);
               (*causeType) = WR_CAUSE_RADIONW;
               (*causeVal)  = SztCauseRadioNwinvalid_CSG_IdEnum;
            }
         }
         else if(WR_UMM_CSG_NOT_MEMBER == ueMbrShpSts)
         {
            /* add CSG ID to UE membership status list */
            retVal = wrUmmCsgAddCsgIdIntoMbrLst(ueCb, csgId.val,
                  WR_UMM_CSG_NOT_MEMBER);
            if(RFAILED == retVal)
            {
               RLOG2(L_ERROR,"Failed to Update UE membership status CSG ID "
                     "[%d] membership status [%d]", csgId.val, ueMbrShpSts);
               (*causeType) = WR_CAUSE_MISC;
               (*causeVal)  = SztCauseMiscunspecifiedEnum;
            }
            else
            {
               if(isEmrgncyCall)
               {
                  RLOG3(L_INFO, "Admitting a NON-MEMBER in HYBRID Mode for "
                        "EMERGENCY CALL CSG ID in SIB1 [%d] CSG ID in HO REQ [%d] "
                        "Member status [%d]",
                        cellCb->csgId, csgId.val, ueMbrShpSts);
               }
               else
               {
                  RLOG3(L_INFO, "Admitting a NON-MEMBER in HYBRID Mode for "
                        "CSG ID in SIB1 [%d] CSG ID in HO REQ [%d] "
                        "Member status [%d]",
                        cellCb->csgId, csgId.val, ueMbrShpSts);
               }
            }
         }
      }
      break;
      /* HCSG DEV end */
      
      {
         RLOG1(L_WARNING,"Configured Hybrid Access Mode [%d] NOT Supported", cellCb->csgAccessMode);
         (*causeType) = WR_CAUSE_MISC;
         (*causeVal)  = SztCauseMiscunspecifiedEnum;
      }
      break;

      default:
      {
         RLOG1(L_WARNING,"Invalid Access Mode [%d]", cellCb->csgAccessMode);
         (*causeType) = WR_CAUSE_MISC;
         (*causeVal)  = SztCauseMiscunspecifiedEnum;
      }
      break;
   }

   RETVALUE(retVal);
} /* end of wrUmmCsgValidateInbndS1Ho */

/** @brief This function validates the CSG information present in X2AP handover
 *         request message.
 *
 * @details
 *
 *     Function: wrUmmCsgValidateInbndX2Ho
 *
 *         Processing steps:
 *         - if target cell is operating on CLOSED access mode
 *           - if CSG ID of source and target are not same or 
 *             if UE membership status indicates NOT_MEMBER
 *             - reject handover preparation
 *           - else,
 *             - add the CSG ID in UE member list
 *         - if target cell is operating on OPEN access mode
 *           - add the CSG ID in UE member list
 *
 * @param [in]  ueCb      : UE Control Block
 * @param [in]  nbrInfo   : Neighbor information
 * @param [out] hoStatus  : Handover decision status
 * @param [out] hoType    : Handover type
 * @return S16
 *          -# Success : ROK 
 *          -# Failure : RFAILED
 */
PUBLIC S16 wrUmmCsgValidateInbndX2Ho
(
WrUeCb                       *ueCb,
U32                          peerId,
U8                           ueMbrShpSts,
U8                           isEmrgncyCall,
U8                           *causeType,
U8                           *causeVal
)
{
   S16                       retVal = RFAILED;
   WrUmmCellCb               *cellCb;
   U32                       srcCsgId;
   U32                       enbId;
   WrNeighEnbCb              *nbrEnbCb  = NULLP;
   WrEutranNeighCellCb       *nbrCellCb = NULLP;
   CmLList                   *nrLnk     = NULLP;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if (NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)ueCb->cellId,
            "Error in getting CELL CB");
      (*causeType) = CZT_CAUSE_MISC;
      (*causeVal)  = CztCauseMiscunspecifiedEnum;
      RETVALUE(RFAILED);
   }

   WR_GET_ENBID_FROM_PEERID(enbId, peerId);
   retVal = cmHashListFind(&(wrEmmCb.neighEnbLstCp), (U8 *)&enbId,
         sizeof(enbId), 0, (PTR *)&nbrEnbCb);
   if(RFAILED == retVal)
   {
      RLOG1(L_ERROR,"Failed to fetch Neighbor ENB CB [%d]", enbId);
      (*causeType) = CZT_CAUSE_MISC;
      (*causeVal)  = CztCauseMiscunspecifiedEnum;
      RETVALUE(RFAILED);
   }

   if(WR_ENODEB_TYPE_MACRO == nbrEnbCb->enbType &&
         WR_ENB_OPEN_ACCESS != cellCb->csgAccessMode)
   {
      RLOG1(L_ERROR, "OPEN Access to CLOSED Access X2 Handover is not "
            "Allowed. Source ENB ID [%d]", enbId);
      (*causeType) = CZT_CAUSE_MISC;
      (*causeVal)  = CztCauseMiscunspecifiedEnum;
      RETVALUE(RFAILED);
   }

   CM_LLIST_FIRST_NODE((&(nbrEnbCb->srvdCellLst)), nrLnk);
   if(NULLP == nrLnk)
   {
      RLOG1(L_ERROR, "Failed to fetch Neighbor Cell CB [%d]", enbId);
      (*causeType) = CZT_CAUSE_MISC;
      (*causeVal)  = CztCauseMiscunspecifiedEnum;
      RETVALUE(RFAILED);
   }
   nbrCellCb = (WrEutranNeighCellCb *)nrLnk->node;
   srcCsgId  = nbrCellCb->csgId;

   switch(cellCb->csgAccessMode)
   {
      case WR_ENB_CLOSED_ACCESS:
      {
         if(WR_EMM_NBR_CSG_ID_NOT_DISCOVERED == nbrCellCb->isCsgIdResolved)
         {
            RLOG1(L_ERROR, "CSG ID Unknown for Neighbor Cell CB [%d]", enbId);
            (*causeType) = CZT_CAUSE_MISC;
            (*causeVal)  = CztCauseMiscunspecifiedEnum;
            retVal = RFAILED;
         }
         else if(srcCsgId == cellCb->csgId)
         {
            retVal = wrUmmCsgAddCsgIdIntoMbrLst(ueCb, cellCb->csgId,
                  WR_UMM_CSG_MEMBER);
            if(RFAILED == retVal)
            {
               RLOG2(L_ERROR,"Failed to Update UE membership status CSG ID "
                     "[%d] membership status [%d]", cellCb->csgId, ueMbrShpSts);
               (*causeType) = CZT_CAUSE_MISC;
               (*causeVal)  = CztCauseMiscunspecifiedEnum;
            }
         }
         else
         {
            RLOG2(L_ERROR,"MemberShip Status Unknown and CSG ID Mismatch. "
                  "CSG ID in SIB1 [%d] CSG ID in HO REQ [%d]",
                  cellCb->csgId, srcCsgId);
            (*causeType) = WR_CAUSE_RADIONW;
            (*causeVal)  = CztCauseRadioNwunspecifiedEnum;
            retVal = RFAILED;
         }
      }
      break;

      case WR_ENB_OPEN_ACCESS:
      {
         RLOG2(L_INFO, "Ignoring CSG info in OPEN Access. "
               "NBR CSG ID of Source [%d] membership status [%d]",
               srcCsgId, ueMbrShpSts);
         if(WR_UMM_CSG_MEMBERSHIP_NOT_KNOWN != ueMbrShpSts)
         {
            retVal = wrUmmCsgAddCsgIdIntoMbrLst(ueCb, srcCsgId, ueMbrShpSts);
            if(RFAILED == retVal)
            {
               RLOG2(L_ERROR,"Failed to Update UE membership status NBR CSG ID "
                     "[%d] membership status [%d]", srcCsgId, ueMbrShpSts);
               (*causeType) = CZT_CAUSE_MISC;
               (*causeVal)  = CztCauseMiscunspecifiedEnum;
            }
         }
      }
      break;

      /* HCSG DEV start */
      case WR_ENB_HYBRID_ACCESS:
      {
         if(WR_EMM_NBR_CSG_ID_NOT_DISCOVERED == nbrCellCb->isCsgIdResolved)
         {
            RLOG1(L_ERROR, "CSG ID Unknown for Neighbor Cell CB [%d]", enbId);
            (*causeType) = CZT_CAUSE_MISC;
            (*causeVal)  = CztCauseMiscunspecifiedEnum;
            retVal = RFAILED;
         }
         else if(srcCsgId == cellCb->csgId)
         {
            retVal = wrUmmCsgAddCsgIdIntoMbrLst(ueCb, cellCb->csgId,
                  ueMbrShpSts);
            if(RFAILED == retVal)
            {
               RLOG2(L_ERROR,"Failed to Update UE membership status CSG ID "
                     "[%d] membership status [%d]", cellCb->csgId, ueMbrShpSts);
               (*causeType) = CZT_CAUSE_MISC;
               (*causeVal)  = CztCauseMiscunspecifiedEnum;
            }
         }
         else
         {
            RLOG2(L_ERROR,"MemberShip Status Unknown and CSG ID Mismatch. "
                  "CSG ID in SIB1 [%d] CSG ID in HO REQ [%d]",
                  cellCb->csgId, srcCsgId);
            (*causeType) = WR_CAUSE_RADIONW;
            (*causeVal)  = CztCauseRadioNwunspecifiedEnum;
            retVal = RFAILED;
         }
      }
      break;
      /* HCSG DEV end */

      default:
      {
         RLOG1(L_ERROR,"Invalid Access Mode [%d]", cellCb->csgAccessMode);
         (*causeType) = CZT_CAUSE_MISC;
         (*causeVal)  = CztCauseMiscunspecifiedEnum;
         retVal = RFAILED;
      }
      break;
   }

   RETVALUE(retVal);
} /* end of wrUmmCsgValidateInbndX2Ho */

/** @brief This function prints the UE stats
 *
 * @details
 *
 *     Function: wrUmmHcsgPrintUeStats
 *
 *         Processing steps:
 *         - print the UE stats in the pre-defined format
 *
 * @param 
 * @return S16
 *          -# Success : ROK 
 *          -# Failure : RFAILED
 */
#ifdef HCSG_DBG
PUBLIC S16 wrUmmHcsgPrintUeStats
(
)
{
   U8                        idx = 0;
   U8                        cellId = 1;
   WrUmmCellCb               *cellCb = NULLP;
   char                      connType[2];
   char                      *mbrType[2] = {"MEMBER", "NOT-MEMBER"};
   WrUeCb                    *ueCb = NULL;
   Bool                      isUePrsnt = FALSE;

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {
      RETVALUE (ROK);
   }

   for (idx = 0; idx < WR_MAX_UES_PER_CELL; idx++)
   {
      if (cellCb->ueCbs[idx] != NULL)
      {
         ueCb = cellCb->ueCbs[idx];
         if(NhuEstablishmentCauseemergencyEnum != ueCb->establishCause)
         {
            connType[0] = 'N';
         }
         else
         {
            connType[0] = 'E';
         }
         if(WR_ENB_OPEN_ACCESS == cellCb->csgAccessMode)
         {
            printf("%d\t%d\t%c\t\033[1m\033[32m%s\x1B[0m \n", idx, ueCb->crnti,
                  connType[0], mbrType[0]);
         }
         else
         {
            if(WR_UMM_CSG_MEMBER == wrUmmCsgIsUeMember(ueCb, cellCb->csgId))
            {
               printf("%d\t%d\t%c\t\033[1m\033[32m%s\x1B[0m \n", idx,
                     ueCb->crnti, connType[0], mbrType[0]);
            }
            else
            {
               printf("%d\t%d\t%c\t\033[1m\033[31m%s\x1B[0m \n", idx,
                     ueCb->crnti, connType[0], mbrType[1]);
            }
         }
         isUePrsnt = TRUE;
      }
   }
   if(isUePrsnt)
      printf("\n\n\n");

   RETVALUE (ROK);
}
#endif
/********************************************************************30**

           End of file:     fc_emm_csg.c@@/main/TeNB_Main_BR/tenb_main_ccb/tenb_5.0_RIB/tenb_5.0_RIB_GA/7 - Mon Feb 15 12:51:22 2016

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
/main/0        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
