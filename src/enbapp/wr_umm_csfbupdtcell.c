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
               

     File:     wr_umm_csfbupdtcell.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=51;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_umm.h"
#include "wr_utils.h"
#include "wr_emm.h"
#include "wr_umm_trans.h"
#include "wr_umm_csfb.h"
#include "wr_umm_meas_fun.h"
#include "wr_umm_csfbupdtcell.h"

EXTERN S16 wrUmmChkOneUtraNeigh
(
 WrUeCb           *ueCb,
 U8               ratPriority
);
EXTERN WrUmmMeasUtraMeasObjNode* wrUmmGetUtraMeasObjNode
(
U16           arfcn,
CmLListCp     utraMeasObjLst
);
EXTERN Bool wrUmmChkIfPscPrsntInMeasLst
(
 WrUmmMeasUtraMeasObjNode     *ueMeasObjNode,
 U32                          psc
 );
PUBLIC S16 wrUmmCsfbCdmaHdlMeasRpt
(
 WrUmmTransCb                       *ummTransCb,
 NhuMeasResults                     *measRpt,
 WrUmmMeasRptInfo                   *measRptInfo
);

PUBLIC S16 wrUmmChkPciRcvdForListedCell
(
U8                         measId,
U32                        pci,
WrUeCb                     *ueCb
);

/** @brief   
This function will be called when any message receives from UE 
 *     to enodeb which is belongs to CSFB.   
 * @details
 *
 *     Function: wrUmmChkOneUtraNeigh
 *
 *         Processing steps:
 *            - Get the Number of Neighbours Configured by eNodeB for that Band 
 *              Chekc if That band supported by UE 
 *                - if only One Neighbour then choose UE for redirection without UE meas.
 *                - else eNB shall perform Measurements for redirection 
 *           
 * @param [in] wrUeCb 
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */ 
PUBLIC S16 wrUmmChkOneUtraNeigh
(
 WrUeCb           *ueCb,
 U8               ratPriority
)
{
   WrUmmCellCb               *cellCb;
   Bool                      isUeSupported = FALSE;
   U8                        cnt = 0;
   U8                        targetBand;
   WrUmmMeasUtraMeasObjNode  *utraCellObj;
   CmLList                   *utraMeasObj;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   if(NULLP == cellCb) 
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId,
                                   "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

	if (ratPriority == UTRA_FDD)
	{
	   utraMeasObj = cellCb->measCellCb->utraFddMeasObjLst.first;
	}
	else
	{
	   utraMeasObj = cellCb->measCellCb->utraTddMeasObjLst.first;
	}

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
         wrUmmFndUTRABandFDD(utraCellObj->arfcn, &targetBand);

         /*Check whether the bandclass is UE supported or not*/
         isUeSupported = wrUmmUTRAFDDBandSupported(ueCb,targetBand);
         if(isUeSupported == TRUE)
         {
            /* If band is supported by UE, increment the count here */
            cnt++;
            if(cnt > 1)
            {
               /* cnt is greater than 1 only in two cases - 
                * 1. Either more than 1 band supported by UE is configured.
                * 2. Or more than 1 Neighbor is configured 
                * Hence, eNb can choose this UE for redirection with meas */
               RLOG0(L_INFO, " More than one Neighbour is Configured");
               RETVALUE(ROK);
            }
         }
      }
		else
		{

         /*Check whether the bandclass is UE supported or not*/			
			targetBand = utraCellObj->tddBandIndicator;
         isUeSupported = wrUmmUtraTddBandSupported(ueCb, targetBand, utraCellObj->tddMode);
         if(isUeSupported == TRUE)
         {
            /* If band is supported by UE, increment the count here */
            cnt++;
            if(cnt > 1)
            {
               /* cnt is greater than 1 only in two cases - 
                * 1. Either more than 1 band supported by UE is configured.
                * 2. Or more than 1 Neighbor is configured 
                * Hence, eNb can choose this UE for redirection with meas */
               RLOG0(L_INFO, " More than one Neighbour is Configured");
               RETVALUE(ROK);
            }
         }
      }
		utraMeasObj = utraMeasObj->next;
   }

   RLOG0(L_INFO, "Only one Neighbour is configured for that band supported by UE");
   RETVALUE (RFAILED);
}/*wrUmmChkOneUtraNeigh */

/** @brief This function finds out RSCP  value of a UTRA Cell   
 * @details
 *
 *     Function: wrUmmCsfbGetUtraRscp
 *
 *
 * @param [in]  utraMeasResult
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PRIVATE S16 wrUmmCsfbGetUtraRscp
(
   NhuMeasResultUTRAmeasResult *utraMeasResult
)
{
   S32 rscp = WR_UMM_MIN_VAL_SHORT;

   if(utraMeasResult->utra_RSCP.pres != NOTPRSNT)
   {
      rscp = utraMeasResult->utra_RSCP.val;
   }
   RETVALUE(rscp);

}/*wrUmmCsfbGetUtraRscp*/

/** @brief This function finds out ECNO  value of a UTRA Cell   
 * @details
 *
 *     Function: wrUmmCsfbGetUtraEcno
 *
 *
 * @param [in]  utraMeasResult
 * @return U8
 */
PRIVATE U8 wrUmmCsfbGetUtraEcno
(
   NhuMeasResultUTRAmeasResult *utraMeasResult
)
{
   U8 ecno = INVALID_ECN0;

   if(utraMeasResult->utra_EcN0.pres != NOTPRSNT)
   {
      ecno = utraMeasResult->utra_EcN0.val;
   }
   RETVALUE(ecno);

}/*wrUmmCsfbGetUtraEcno*/

/** @brief This function gets PSC id of a UTRA Cell   
 * @details
 *
 *     Function: wrUmmCsfbGetUtraPscId
 *
 *
 * @param [in]  phyCell
 * @return U32
 */
PRIVATE U32 wrUmmCsfbGetUtraPscId
(
  NhuMeasResultUTRAphysCellId *phyCell
)
{
   U32 pscId = 0;
   if(phyCell->choice.val == PHYSCELLID_FDD)
   {
      pscId = phyCell->val.fdd.val;
   }
   else 
   {
      pscId = phyCell->val.tdd.val;
   }
   RETVALUE(pscId);

}/*wrUmmCsfbGetUtraPscId*/


/** @brief This function selects the WCDMA candidate arfcn and cells for 
  *         CS fallback   
 *         from measurement  report, function will be called after
 *         getting measurement report for CSFB Module
 * @details
 *
 *     Function: wrUmmCsfbHdlMeasRpt
 *
 *         Processing steps:
 *         1. check if UE context's PSC list for CS fallback have any element
 *             if no then allocate memory to store WCDMA PSCs and goto step 6.
 *             if yes then 
 *         2. Read PSC from the list of PSCs from measurement report.
 *         3. Check if PLMN ID and LAC of PSC read in above step, is same 
 *             as used currently.
 *         4. If yes ,check if power of this PSC is greater than 
 *             candidate PSC stored in UE Context,
 *                -if yes,delete existing content of PSC list  
 *                   in UE Control block and copy the PSC (measurement report) 
 *                   to ueCb and goto step 6.
 *                - if no, ignore this measurement report and return from 
 *                   function.
 *         5. If PLMN ID and LAC doesn't match. goto step 2.    
 *         6. Read the next PSC(until PSCs are there in the list ) from  
 *             measurement report.
 *         7. Check if PLMN ID and LAC of PSC read in above step, is same 
 *             as used currently,
 *             -If yes, copy/append the PSC to UE control blcok's PSC list.
 *             -If no, delete the PSC from measrement report and goto step 6.
 *
 * @param [in] ueCb
 * @param [in] measRpt 
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbHdlMeasRpt
(
 WrUmmTransCb                       *ummTransCb,
 NhuMeasResults                     *measRpt
)
{
   U8                   cnt               = 0;
   U8                   redirCnt          = 0;
   U8                   numPscsTransCb    = 0;
   S16                  rscp;
   S16                  prevRscp;
   U8                   ecno              = INVALID_ECN0;
   U8                   prevEcno          = INVALID_ECN0;
   U32                  pscId             = 0;
   Bool                 flag              = TRUE;
   WrUeCb               *ueCb             = NULLP;
   WrUmmMeasRptInfo     measRptInfo;
   WrUtraNeighCellCb    *utranCellCb      = NULLP;
   U8                   duplexType;
   WrUmmMeasCellTyp     cellType;
   WrUmmCsfbTransCb     *transCb;
   U8                   numPscs; 
   Bool                 plmnSupported     = FALSE;
   U8                   idx = 0;
   S16                  ret = ROK;

   RLOG0(L_DEBUG, "Function wrUmmCsfbHdlMeasRpt");

   if(measRpt->pres.pres == NOTPRSNT)
   {
      RLOG0(L_ERROR, "measurement report is not present");
      RETVALUE(RFAILED);
   }
   /* ccpu00143123: this means empty measurement report, this check should 
    * return ROK, not RFAILED */
   if( measRpt->measResultNeighCells.choice.pres == NOTPRSNT)
   {
      RLOG0(L_WARNING, "measurement report is empty");
      RETVALUE(ROK);
   }
   ueCb = ummTransCb->ueCb;
   transCb = &ummTransCb->u.csfbTrans;

   if(wrUmmMeasGetArfcnAndRptType(ueCb->ueIdx, ueCb->cellId,
            measRpt,&measRptInfo) != ROK)
   {
      RETVALUE(RFAILED);
   }

   if(WR_UMM_MEAS_UTRA_FDD_FREQ == measRptInfo.cellTyp)
   {
       RLOG0(L_INFO, "MR Received for UTRA FDD");
      WrUmmMeasUtraMeasObjNode      *utraMeasObjNode = NULLP;

      numPscs = measRpt->measResultNeighCells.val.measResultLstUTRA.noComp.val;
      cellType = measRptInfo.cellTyp;
      duplexType  = measRptInfo.duplexTyp;

      /* For CSFB to UTRA FDD measurements would be done only if Reg LAI was received.
       * All the measObjs based on the Reg LAI filtering will be stored in the 
       * ueCb->hrlMeasCfg. So fetch the measObjNode from ueCb->hrlMeasCfg. */
      utraMeasObjNode = (WrUmmMeasUtraMeasObjNode *) wrUmmGetUtraMeasObjNode\
                        (measRptInfo.arfcn.t.carrierFreq, ueCb->hrlMeasCfg.utraFddMeasObjLst);
      if(NULLP == utraMeasObjNode)
      {
         RLOG0(L_ERROR, "UTRA FDD Measurement utraMeasObjNode is Not Configured");
         RETVALUE(RFAILED);
      }

      while(cnt < numPscs/*numPscs--*/)
      {
         if(measRpt->measResultNeighCells.val.measResultLstUTRA.member[cnt].physCellId.choice.val ==
               PHYSCELLID_FDD)
         {
            pscId = measRpt->measResultNeighCells.val.
               measResultLstUTRA.member[cnt].physCellId.val.fdd.val;
            if(FALSE == wrUmmChkIfPscPrsntInMeasLst(utraMeasObjNode, pscId))
            {
               /* UE has reported PSC which is not listed, so ignore thise PSC. */
               cnt++;
               continue;
            }
         }
         /*Get PLMN id of Neighbour cell*/
         utranCellCb = wrEmmGetNeighUtraCell(ueCb->cellId,
               measRptInfo.arfcn.t.carrierFreq, pscId, WR_UTRA_MODE_FDD);
         if(utranCellCb == NULLP)
         {
            cnt++;
            continue;
         }

         if (NULLP == transCb->redirectInfo)
         {
            WR_ALLOC(&transCb->redirectInfo, sizeof(WrUmmRedirectInfo));
            if (NULLP == transCb->redirectInfo)
            {
               RLOG0(L_FATAL, "Memory allocation failed. ");
               RETVALUE(NULLP);
            }
            flag = FALSE;
            wrFillTknU8(&(transCb->redirectInfo->val.utraInfo.pres),PRSNT_NODEF);
            transCb->redirectInfo->val.utraInfo.arfcn = measRptInfo.arfcn.
               t.carrierFreq;
            transCb->redirectInfo->cellId = ueCb->cellId;
            transCb->redirectInfo->ratTyp = WR_UMM_MEAS_UTRA_FDD_FREQ;
            transCb->redirectInfo->val.utraInfo.duplexTyp = duplexType;				
         }

         if((transCb->csFallBackInd.val == 0)/*0:csfb without high priority
                                               1 : with high priority*/
               && (ueCb->regLai.isLaiPres == TRUE))
         {
            for(idx=0;idx<utranCellCb->numPlmn;idx++)
            {
               if(wrUtlPlmnsEqual(&utranCellCb->plmnId[idx],
                        &ueCb->regLai.plmnId))
               {
                  plmnSupported = TRUE;
                  break;
               }
            }
         }
         else
         {
            plmnSupported = TRUE;
            ueCb->regLai.lac = utranCellCb->lac;
         }

         if(plmnSupported && ueCb->regLai.lac == utranCellCb->lac &&
               measRpt->measResultNeighCells.val.
               measResultLstUTRA.member[cnt].measResult.pres.pres != NOTPRSNT)
         {

            rscp = wrUmmCsfbGetUtraRscp(&measRpt->measResultNeighCells.val.
                  measResultLstUTRA.member[cnt].measResult);
            ecno = wrUmmCsfbGetUtraEcno(&measRpt->measResultNeighCells.val.
                  measResultLstUTRA.member[cnt].measResult);
            if(((rscp <= WR_UMM_UTRA_MIN_RSCP_VAL) ||
                  (rscp > WR_UMM_UTRA_MAX_RSCP_VAL)) && ( INVALID_ECN0 == ecno))
            {
               RLOG0(L_WARNING, "RSCP Value is Less then 6:: Ghost UTRA Cells\n");
               break;
            }
            pscId = wrUmmCsfbGetUtraPscId(&measRpt->measResultNeighCells.val.
                  measResultLstUTRA.member[cnt].physCellId);
            prevRscp = 
               transCb->redirectInfo->val.utraInfo.utraCellRedirInfo[redirCnt].rscp;
            prevEcno = 
               transCb->redirectInfo->val.utraInfo.utraCellRedirInfo[redirCnt].ecno;

            if(INVALID_ECN0 == ecno)  
            {
               if((prevRscp > rscp) && flag)
               {
                  break;
               }
               else if(((prevRscp < rscp) || (prevRscp == rscp)) && flag )
               {
                  /*delete PSC list of transCb*/
                  WR_SET_ZERO(transCb->redirectInfo, sizeof(WrUmmRedirectInfo));
                  wrFillTknU8(&(transCb->redirectInfo->val.utraInfo.
                           pres),PRSNT_NODEF);
                  transCb->redirectInfo->val.utraInfo.arfcn = 
                     measRptInfo.arfcn.t.carrierFreq;
                  transCb->redirectInfo->cellId = ueCb->cellId;
                  transCb->redirectInfo->ratTyp = WR_UMM_MEAS_UTRA_FDD_FREQ;
                  transCb->redirectInfo->val.utraInfo.duplexTyp = duplexType;
                  flag = FALSE;
               }
            }
            else
            {
               /* flag will be TRUE for second MR */
               if ( flag )
               {
                  if( prevEcno > ecno )
                  {
                     break;
                  }
                  else if( prevEcno <= ecno )
                  {
                     /*delete PSC list of transCb*/
                     WR_SET_ZERO(transCb->redirectInfo, sizeof(WrUmmRedirectInfo));
                     wrFillTknU8(&(transCb->redirectInfo->val.utraInfo.
                              pres),PRSNT_NODEF);
                     transCb->redirectInfo->val.utraInfo.arfcn = 
                        measRptInfo.arfcn.t.carrierFreq;
                     transCb->redirectInfo->cellId = ueCb->cellId;
                     transCb->redirectInfo->ratTyp = WR_UMM_MEAS_UTRA_FDD_FREQ;
                     transCb->redirectInfo->val.utraInfo.duplexTyp = duplexType;
                     flag = FALSE;
                  }
               } /* if ( flag ) */
               /* Update the ecno in utraCellRedirInfo array
                * Assumption: enco received in the 
                * MR will be in descending order. i.e. Best cell will
                * be on top of the list
                * LOGIC: Check the first entry of previous 
                * MR and if it is less than or equal to the first entry of
                * received MR then overwrite it. second cells/entry
                * onwards add to the list without comparing it 
                * i.e:  numPscsTransCb != 0
                */

               if ( (prevEcno <= ecno) || ( numPscsTransCb != 0) ) 
               {
                  transCb->redirectInfo->val.utraInfo.utraCellRedirInfo[numPscsTransCb]
                     .ecno = ecno;
               }

            } 
            transCb->redirectInfo->val.utraInfo.utraCellRedirInfo[numPscsTransCb]
               .pscId = pscId;
            transCb->redirectInfo->val.utraInfo.utraCellRedirInfo[numPscsTransCb]
               .rscp = rscp;
            numPscsTransCb++;
         }/*end of outer if*/
         cnt++;
      }/*end of while loop*/

      if(numPscsTransCb > 0)
      {
         RLOG0(L_INFO, "UTRA FDD No Of PSCs are more then ZERO");
         transCb->redirectInfo->val.utraInfo.noOfPscs = numPscsTransCb;
      }
   } 

   if(WR_UMM_MEAS_UTRA_TDD_FREQ == measRptInfo.cellTyp)
   {
      RLOG0(L_INFO, "MR Received for UTRA TDD");
      WrUmmMeasUtraMeasObjNode      *utraMeasObjNode = NULLP;

      /* For CSFB to UTRA TDD measurements would be done only if Reg LAI was received.
       *        * All the measObjs based on the Reg LAI filtering will be stored in the
       *               * ueCb->hrlMeasCfg. So fetch the measObjNode from ueCb->hrlMeasCfg. */
      utraMeasObjNode = (WrUmmMeasUtraMeasObjNode *) wrUmmGetUtraMeasObjNode\
                        (measRptInfo.arfcn.t.carrierFreq, ueCb->hrlMeasCfg.utraTddMeasObjLst);
      if(NULLP == utraMeasObjNode)
      {
         RLOG0(L_ERROR, "UTRA TDD measurement utraMeasObjNode is Not Configured");
         RETVALUE(RFAILED);
      }

      wrUmmUtraTddHdlMeasRpt(ummTransCb->ueCb, measRpt, &transCb->redirectInfo, 
            utraMeasObjNode, WR_UMM_MEAS_TYPE_CSFB);
   }

   if(WR_UMM_MEAS_1XCDMA == measRptInfo.cellTyp)
   {
      /* Store the received measurment report details in the
       * csfbTransCb->cdmaPilotList table */ 
      ret = wrUmmCsfbCdmaHdlMeasRpt(ummTransCb, measRpt, &measRptInfo);
      if(ROK != ret)
      {
         RLOG0(L_ERROR, "measurement report handling failed");
         /*Only this instnce of measurement report failed, so ignore this report
          * and wait for next one */
         RETVALUE(RFAILED);
      }
   }
   if(WR_UMM_MEAS_GERAN_FREQ == measRptInfo.cellTyp)
   {
      ret = wrUmmCsfbGeranHdlMeasRpt(ummTransCb, measRpt);
      RETVALUE(ret);
   }
   RETVALUE(ROK);
}/*End of wrUmmCsfbHdlMeasRpt*/


/** @brief This function handles the CDMA measurment reports receied for Known
 * or unknown 1xRTT Cell
 * @details
 *
 *     Function: wrUmmChkPciRcvdForListedCell
 *
 *         Processing steps:
 *         1.Process the recived CDMA measurement report for Known or unknown 
 *           PCI configured for the UE
 *         2. Get the Measurement Object configured for CDMA
 *         3. if its unknown PCI then retirn Fail else ROK
 *          
 *
 * @param [in]  measID
 * @param [in]  PCI/PnOffset 
 * @param [out] UeCb
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmChkPciRcvdForListedCell
(
 U8                         measId, 
 U32                        pci,
 WrUeCb                     *ueCb
)
{
   U8                           measObjId;
   WrMeasIdNode                 *measIdObj = NULLP;
   WrUmmMeasObjInfo             measObjInfo;
   WrUmmMeasCdmaMeasObjNode     *cdmaMeasObj;
   CmLList                      *lnk = NULLP;
   WrMeasNrCdmaCellNode         *cellNode;

   RLOG0(L_DEBUG, "wrUmmChkPciRcvdForListedCell Function ");
   /*Get MeasIdObj from ueCb->measCfg->measIdLst */
   if(RFAILED == wrUmmMeasGetMeasIdObj(measId, &measIdObj, 
            &ueCb->measCfg->measIdLst))
   {
      RLOG0(L_ERROR, "CDMA MeasId Object not found from UE Cb");
      RETVALUE(RFAILED);
   }

   measId = measIdObj->measID;
   measObjId = measIdObj->measObjID;

   /* get measurement object and Type*/
   if(ROK !=  wrUmmMeasFindMeasObjAndTyp(&measObjInfo,measObjId,ueCb->measCfg))
   {
      RLOG0(L_ERROR, "CDMA Measurement object not found");
      RETVALUE(RFAILED);
   }

   if(WR_UMM_MEAS_1XCDMA != measObjInfo.nrType)
   {
      RLOG0(L_ERROR, "CDMA Measurement NR TYPE not found");
      RETVALUE(RFAILED);
   }
   cdmaMeasObj = measObjInfo.cdmaObj->cdmaMeasObj;
   lnk         = cdmaMeasObj->cdmaCellInfo.first; 
   while(lnk != NULLP)
   {
      cellNode = (WrMeasNrCdmaCellNode *)lnk->node;
      if(cellNode->pci == pci)
      {
         RETVALUE(ROK);
      }
      lnk = lnk->next;
   }
   RETVALUE(RFAILED);
}/* End of wrUmmChkPciRcvdForListedCell */

/** @brief This function handles the CDMA measurment reports and store the best
 * Pilot
 * @details
 *
 *     Function: wrUmmCsfbCdmaHdlMeasRpt
 *
 *         Processing steps:
 *         1.Process the recived CDMA measurement report from UE
 *           store the best measurement received from UE in corresponding
 *           cdmaPilotList pointer. 
 *         2.For CSFB store the information in appropritae memory
 *          
 *
 * @param [in]  ummTransCb
 * @param [in]  measRpt 
 * @param [out] measRptInfo 
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmCsfbCdmaHdlMeasRpt
(
 WrUmmTransCb                       *ummTransCb,
 NhuMeasResults                     *measRpt,
 WrUmmMeasRptInfo                   *measRptInfo
)
{
   U8                      cnt;
   U8                      numPnOffset;
   WrCdmaRttPilotLst       *cdmaRttPilotLst;
   NhuMeasResultCDMA2000   *cdmaMeasRpt = NULLP;

   RLOG0(L_DEBUG,"Function wrUmmCsfbCdmaHdlMeasRpt MR Received for 1xRTT Cell");
   /* Get the number of Cells reported in Measurment Report. If nocomp is zero
    * then ignore this report, else do following
    * 1. Allocate memory for WrCdmaRttPilotLst and store 
    * -------------------------------------------------------
    * |bandClass|arfcn| PnOffset|PilotStrength| PilotPnPhse|
    * -------------------------------------------------------
    *  for each reported PnOffset. 
    */
    numPnOffset = measRpt->measResultNeighCells
         .val.measResultsCDMA2000.measResultLstCDMA2000.noComp.val;

    cnt = 0;
    while(cnt < numPnOffset) 
    {
       cdmaMeasRpt = &measRpt->measResultNeighCells.val.
          measResultsCDMA2000.measResultLstCDMA2000.
          member[cnt];
       /* PilotPNPhase is used in either SRVCC handover or enhanced
        * 1xRTT CS fallback procedure to CDMA2000 1xRTT
        * so In case for eCSFB if Pilot not received Ignore the 
        * Reported Measurment Value  */
       if(PRSNT_NODEF != cdmaMeasRpt->measResult.pilotPnPhase.pres)
       {
          RLOG0(L_WARNING, "PilotPnPhase is not Pressent for this MR");
          cnt++;
          continue;
       }
       if(ROK != wrUmmChkPciRcvdForListedCell(measRpt->measId.val, 
                cdmaMeasRpt->physCellId.val, ummTransCb->ueCb))
       {
          RLOG0(L_WARNING, "MeasRpt contains PCI for non listed CDMA Cells"
                "Ignore the UnKNOW PCIs for CDMA");
          /*TODO Handle ANR */
          cnt++;
          continue;
       }

       if(NULLP == ummTransCb->u.csfbTrans.cdmaRttPilotLst)
       {
          if(WR_UMM_MEAS_1XCDMA == measRptInfo->arfcn.choice)
          {

             WR_ALLOC(&cdmaRttPilotLst, sizeof(WrCdmaRttPilotLst));
             if(NULLP == cdmaRttPilotLst)
             {
                RLOG0(L_FATAL, "Memory allocation failed.");
                RETVALUE(RFAILED);
             }
             cdmaRttPilotLst->bandClass     = measRptInfo->arfcn.t.
                measObjCDMA2000.bandClass;
             cdmaRttPilotLst->arfcn         = measRptInfo->arfcn.t.
                measObjCDMA2000.arfcn;
             cdmaRttPilotLst->pnOffset      = cdmaMeasRpt->physCellId.val;
                cdmaRttPilotLst->pilotPnPhase  = cdmaMeasRpt->measResult.
                   pilotPnPhase.val;
             cdmaRttPilotLst->pilotStrength = cdmaMeasRpt->measResult.
                pilotStrength.val;
             /* Details of reported Pnoffset will be stored in
              * csfbTransCb->cdmaRttPilotLst[]
              */
             ummTransCb->u.csfbTrans.cdmaRttPilotLst = cdmaRttPilotLst;
          }
          else
          {
             RLOG0(L_INFO, "Measurement Report Rx for HRPD Cells");
             /*HRPD Measurement report is not handled. */
             cnt++;
             continue;
          }

       }
       else
       {
          /*Check if the latest measurment report strength is better than
           * the available pilot list strength in the table. For CSFB at
           * index 0 always the best pilot willl be stored
           */
          if(ummTransCb->u.csfbTrans.cdmaRttPilotLst->pilotStrength >
                cdmaMeasRpt->measResult.pilotStrength.val)
          {
             ummTransCb->u.csfbTrans.cdmaRttPilotLst->bandClass = measRptInfo->arfcn.t.
                measObjCDMA2000.bandClass;
             ummTransCb->u.csfbTrans.cdmaRttPilotLst->arfcn  = measRptInfo->arfcn.t.
                measObjCDMA2000.arfcn;
             ummTransCb->u.csfbTrans.cdmaRttPilotLst->pnOffset = cdmaMeasRpt->physCellId.val;
             if(PRSNT_NODEF == cdmaMeasRpt->measResult.pilotPnPhase.pres)
             {
                ummTransCb->u.csfbTrans.cdmaRttPilotLst->pilotPnPhase = cdmaMeasRpt->measResult.
                   pilotPnPhase.val;
             }
             ummTransCb->u.csfbTrans.cdmaRttPilotLst->pilotStrength = cdmaMeasRpt->measResult.
                pilotStrength.val;
          } 
       }
       cnt++;
    }
    RETVALUE(ROK);
}/* End of wrUmmCsfbCdmaHdlMeasRpt */


/** @brief This function handles the CDMA measurment reports and store the best
 * Pilot in the table in asending order
 * @details
 *
 *     Function: wrUmmEcsfbCdmaHdlMeasRpt
 *
 *         Processing steps:
 *         1.Process the recived CDMA measurement report from UE
 *           store the measurement received from UE in corresponding
 *           cdmaPilotList table in asending order. 
 *         2.For ECSFB store the information in appropritae cdmaPilotLst
 *          
 *
 * @param [in]  ummTransCb
 * @param [in]  measRpt 
 * @param [out] measRptInfo 
 * @return S16
    -# Success : ROK
    -# Failure : RFAILED 
 */
PUBLIC S16 wrUmmEcsfbCdmaHdlMeasRpt
(
 WrUmmTransCb                       *ummTransCb,
 NhuMeasResults                     *measRpt,
 WrUmmMeasRptInfo                   *measRptInfo
)
{
   U8                      cnt;
   U8                      idx, idx1;        
   U8                      numPnOffset;
   WrCdmaRttPilotLst       *cdmaRttPilotLst;
   NhuMeasResultCDMA2000   *cdmaMeasRpt;;
   WrUmmEcsfbTransCb       *ecsfbTransCb = &ummTransCb->u.ecsfbTransCb;

   /* Get the number of Cells reported in Measurment Report. If nocomp is zero
    * then ignore this report, else do following
    * 1. Allocate memory for WrCdmaRttPilotLst and store 
    * -------------------------------------------------------
    * |bandClass|arfcn| PnOffset|PilotStrength| PilotPnPhse|
    * -------------------------------------------------------
    *  for each reported PnOffset. 
    */
    numPnOffset = measRpt->measResultNeighCells
         .val.measResultsCDMA2000.measResultLstCDMA2000.noComp.val;

    cdmaRttPilotLst = NULLP;
    cnt = 0;
    while((cnt < numPnOffset) &&
       (ecsfbTransCb->numOfPilotLst < WR_UMM_MAX_CDMA_MEAS_RPT))
    {
       cdmaMeasRpt = &measRpt->measResultNeighCells.val.
          measResultsCDMA2000.measResultLstCDMA2000.
          member[cnt];
       /* PilotPNPhase is used in either SRVCC handover or enhanced
        * 1xRTT CS fallback procedure to CDMA2000 1xRTT
        * so In case for eCSFB if Pilot not received Ignore the 
        * Reported Measurment Value  */
       if(PRSNT_NODEF != cdmaMeasRpt->measResult.pilotPnPhase.pres)
       {
          RLOG0(L_INFO, "PilotPnPhase is not Pressent for this"
                                             "Measurement Report ");
          cnt++;
          continue;
       }
       if(ROK != wrUmmChkPciRcvdForListedCell(measRpt->measId.val, 
                              cdmaMeasRpt->physCellId.val, ummTransCb->ueCb))
       {
          RLOG0(L_WARNING, "MeasRpt contains PCI for non listed CDMA Cells"
                                           "Ignore the UnKNOW PCIs for CDMA");
          /*TODO Handle ANR */
          cnt++;
          continue;
       }

       if(WR_UMM_MEAS_1XCDMA == measRptInfo->arfcn.choice)
       {
          WR_ALLOC(&cdmaRttPilotLst, sizeof(WrCdmaRttPilotLst));
          if(NULLP == cdmaRttPilotLst)
          {
             RLOG0(L_FATAL, "Memory allocation failed.");
             RETVALUE(RFAILED);
          }
          cdmaRttPilotLst->bandClass     = measRptInfo->arfcn.t.
             measObjCDMA2000.bandClass;
          cdmaRttPilotLst->arfcn         = measRptInfo->arfcn.t.
             measObjCDMA2000.arfcn;
          cdmaRttPilotLst->pnOffset      = cdmaMeasRpt->physCellId.val;
             cdmaRttPilotLst->pilotPnPhase  = cdmaMeasRpt->measResult.
                pilotPnPhase.val;
          cdmaRttPilotLst->pilotStrength = cdmaMeasRpt->measResult.
             pilotStrength.val;
          /* Details of reported Pnoffset will be stored in
           * csfbTransCb->cdmaRttPilotLst[]
           */
          for(idx=0; idx < ecsfbTransCb->numOfPilotLst; idx++)
          {
             if(cdmaRttPilotLst->pilotStrength < 
                   ecsfbTransCb->cdmaRttPilotLst[idx]->pilotStrength)
             {
                for(idx1 = ecsfbTransCb->numOfPilotLst; idx1 > idx; idx1--)
                {
                   ecsfbTransCb->cdmaRttPilotLst[idx1] = 
                      ecsfbTransCb->cdmaRttPilotLst[idx1 -1];
                }
                break;
             }
          }
          ecsfbTransCb->cdmaRttPilotLst[idx] = cdmaRttPilotLst;
          ecsfbTransCb->numOfPilotLst++;
       }
       else
       {
          RLOG0(L_INFO, "Measurement Report Received for HRPD Cell");
          /*HRPD Measurement report is not handled. */
          cnt++;
          continue;
       }
       cnt++;
    }
    RETVALUE(ROK);
}/* End of wrUmmEcsfbCdmaHdlMeasRpt */

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
