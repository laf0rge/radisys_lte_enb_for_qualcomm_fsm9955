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
               

     File:     wr_ifm_son.c

     Sid:      fc_ifm_son.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:16:53 2014

     Prg:      subhamay 

*********************************************************************21*/



/* header include files (.h) */
static const char* RLOG_MODULE_NAME="IFM";
static int RLOG_FILE_ID=350;
static int RLOG_MODULE_ID=4;


#include "wr.h"
#include "wr_cmn.h"
#include "wr_umm.h"
#include "wr_ifm_son.h"
#include "wr_emm.h"
#include "wr_emm_pbm.h"
#include "wr_emm_cfg.h"
#include "wr_umm_trans.h"
#include "cm_lte.x"
#include "nlu.x"

/* @brief This function is used to process the bind confirm from the SON 
 *
 * @details
 *
 *     Function: WrLiNluBndCfm
 *    
 *     This API is used to process bind confirm received from SON for 
 *     binding NLU interface between eNB App and SON module
 *
 * @param[in] Pst  *pst
 * @param[in] SuId  suId
 * @param[in] SpId  spId
 * @param[in] U8    status
 * 
 * @return  S16
 *        -# Success : ROK
 */
#ifdef ANSI
PUBLIC S16 WrLiNluBndCfm
(
Pst    *pst,    /* post structure */
SuId    suId,   /* service user Id */
SpId    spId,   /* service provider Id*/
U8      status  /* bind request status */
)
#else
PUBLIC S16 WrLiNluBndCfm (pst, suId, status)
Pst    *pst;    /* post structure */
SuId    suId;   /* service user Id */
SpId    spId;   /* service provider Id*/
U8      status; /* bind request status */
#endif
{
   TRC2(WrLiNluBndCfm);
   
   /*Send alarm */
   if(CM_BND_OK == status)
   {
      wrCb.nluSap[0]->sapState = LWR_SAP_BOUND;
      wrStopTmr((PTR)wrCb.nluSap[0],WR_TMR_NLU_SAP_BND);
      RLOG0(L_INFO, "[ALARM] Sending NLU_SAP_BND_OK to OAM");
      wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_OK, LWR_CAUSE_NLU_SAP_BOUND);
   }
   else
   {
      wrCb.nluSap[0]->sapState = LWR_SAP_UNBOUND;
      wrStopTmr((PTR)wrCb.nluSap[0],WR_TMR_NLU_SAP_BND);
      RLOG0(L_FATAL, "[ALARM] Sending NLU_SAP_BND_FAIL to OAM");
      wrSendLmAlarm(LCM_CATEGORY_INTERFACE, LCM_EVENT_BND_FAIL, LCM_CAUSE_UNKNOWN);
   }
 
   RETVALUE(ROK);
}

/* @brief This function is used to process neighbor deletion
 *  which are reported as part of measurement ANR
 *
 * @details
 *
 *     Function: wrIfmSonDelAnrNeighborReq
 *
 *     This API is used to process deletion of  
 *     neighbor(ANR measurement report)received from Application
 *     to SON Server using the  NLU interface between eNB App and SON module
 *
 * @param[in] NONE
 *
 * @return  S16
 *        -# Success : ROK
 */

#ifdef ANSI
PUBLIC S16 wrIfmSonDelAnrNeighborReq
(
U32                 cellId,
WrEutranNeighCellCb *neighborCellCb
)
#else
PUBLIC S16 wrIfmSonDelAnrNeighborReq(cellId, neighborCellCb)
U32                  cellId;
WrEutranNeighCellCb *neighborCellCb;
#endif
{
   Pst                       *pst = &wrCb.nluSap[0]->pst;
   SuId                      suId = wrCb.nluSap[0]->suId;
   S16                       ret = RFAILED;
   NluNeighborReport         *nghRep = NULLP;
   NluNghCellDel             *nghCellDel = NULLP;


   TRC2(wrIfmSonDelAnrNeighborReq);

   WR_ALLOC(&nghRep, sizeof(NluNeighborReport));
   if(NULLP == nghRep)
   {
      RLOG0(L_ERROR, "IFM: Failed to allocate memory for Neighbor Req");
      RETVALUE(RFAILED);
   }
   nghRep->msgType = NLU_ENB_ANR_NGH_RPT;
   nghRep->servCellId = cellId;
   nghRep->numNghEnb = 1;
   nghRep->nghEnbCellCfg[0].nghEnbId = neighborCellCb->enbId;
   nghRep->nghEnbCellCfg[0].nghCellParam.cfgType = NLU_NGH_DEL;
   nghCellDel = &(nghRep->nghEnbCellCfg[0].nghCellParam.u.nghCellDel);
   nghCellDel->numNgh = 1;
   cmMemcpy((U8 *)&(nghCellDel->ecgi[0].plmn), (
            U8 *)&(neighborCellCb->eCgi.plmnId), sizeof(NluPlmnId));
   ret = WrLiNluNeighborReq(pst, suId, nghRep);
   RETVALUE(ROK);
}

/* @brief This function is used to process New neighbor addition
 *  which are reported as part of measurement ANR
 *
 * @details
 *
 *     Function: wrIfmSonAddAnrNeighborReq
 *
 *     This API is used to process addition of new 
 *     neighbor(ANR measurement report)received from Application
 *     to SON Server using the  NLU interface between eNB App and SON module
 *
 * @param[in] NONE
 *
 * @return  S16
 *        -# Success : ROK
 */

#ifdef ANSI
PUBLIC S16 wrIfmSonAddAnrNeighborReq
(
U32                 cellId,
WrEutranNeighCellCb *neighborCellCb
)
#else
PUBLIC S16 wrIfmSonAddAnrNeighborReq(cellId, neighborCellCb)
U32                  cellId;
WrEutranNeighCellCb *neighborCellCb;
#endif
{
   Pst                       *pst = &wrCb.nluSap[0]->pst;
   SuId                      suId = wrCb.nluSap[0]->suId;
   S16                       ret = RFAILED;
   NluNeighborReport         *nghRep = NULLP;
   NluNghCellAdd             *nghCellAdd = NULLP;


   TRC2(wrIfmSonAddAnrNeighborReq);

   WR_ALLOC(&nghRep, sizeof(NluNeighborReport));
   if(NULLP == nghRep)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   nghRep->msgType = NLU_ENB_ANR_NGH_RPT;
   nghRep->servCellId = cellId;
   nghRep->numNghEnb = 1;
   nghRep->nghEnbCellCfg[0].nghEnbId = neighborCellCb->enbId;
   nghRep->nghEnbCellCfg[0].nghCellParam.cfgType = NLU_NGH_ADD;
   nghCellAdd = &(nghRep->nghEnbCellCfg[0].nghCellParam.u.nghCellAdd);
   nghCellAdd->numNgh = 1;
   nghCellAdd->nghCellAddInfo[0].nghCellInfo.pci = neighborCellCb->pci;
   nghCellAdd->nghCellAddInfo[0].nghCellInfo.dlEarfcn = neighborCellCb->earfcnDl;
   nghCellAdd->nghCellAddInfo[0].nghCellInfo.ulEarfcn = neighborCellCb->earfcnUl;
   nghCellAdd->nghCellAddInfo[0].nghCellInfo.ecgi.cellId = neighborCellCb->eCgi.eutranCellId;
   cmMemcpy((U8 *)&(nghCellAdd->nghCellAddInfo[0].nghCellInfo.ecgi.plmn), (
            U8 *)&(neighborCellCb->eCgi.plmnId), sizeof(NluPlmnId));
   nghCellAdd->nghCellAddInfo[0].numNghNgh = 0;
   
   ret = WrLiNluNeighborReq(pst, suId, nghRep);
   RETVALUE(ROK);
}

#ifdef ANSI
PUBLIC S16 wrIfmSonSendNeighborReq
(
NluNeighborReport            *nghRpt 
)
#else
PUBLIC S16 wrIfmSonSendNeighborReq(nghRpt)
NluNeighborReport            *nghRpt; 
#endif
{
   Pst                       *pst = &wrCb.nluSap[0]->pst;
   SuId                      suId = wrCb.nluSap[0]->suId;
   S16                       ret = RFAILED;

   TRC2(wrIfmSonSendNeighborReq);
   ret = WrLiNluNeighborReq(pst, suId, nghRpt);
   RETVALUE(ret);
}

/* @brief This function is used to send PCI modification Indication confirm toSON 
 *
 * @details
 *
 *     Function: wrIfmSonPciModIndCfm
 *
 *     This API is used to send PCI modification indication
 *     confirm to SON after receiving successfull confirmation from CL
 *
 * @param[in] cellCb   : cell control block
 *
 * @return  S16
 *        -# Success : ROK
 */
#ifdef ANSI
PUBLIC S16 wrIfmSonPciModIndCfm
(
WrCellCb                  *cellCb
)
#else
PUBLIC S16 wrIfmSonPciModIndCfm (cellCb)
WrCellCb                  *cellCb;
#endif
{
   Pst                       *pst = &wrCb.nluSap[0]->pst;
   SuId                      suId = wrCb.nluSap[0]->suId;
   S16                       ret  = RFAILED;
   NluPciModIndCfm           *pciMod = NULLP;

   TRC2(wrIfmSonPciModIndCfm);

   WR_ALLOC(&pciMod, sizeof(NluPciModIndCfm));
   if(NULLP == pciMod)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   pciMod->pci = cellCb->physCellId;
   pciMod->ecgi.cellId = cellCb->cellId;
   ret = WrLiNluPciModIndCfm(pst, suId, pciMod);
   RETVALUE(ret);
}

#ifdef RGR_CQI_REPT

/* @brief This function is used to send UE CQI report toSON
 *
 * @details
 *
 *     This API is used to send UE CQI report indication to SON
 *
 * @param[in] ueCb   : UE control block
 *
 * @return  S16
 *        -# Success : ROK
 *        -# failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 wrIfmSonUeCqiReportInd
(
RgrStaIndInfo   *statInd,
WrUeCb          *ueCb
)
#else
PUBLIC S16 wrIfmSonUeCqiReportInd(statInd, cellCb)
RgrStaIndInfo   *statInd;
WrUeCb             *ueCb;
#endif
{
   Pst             *pst = &wrCb.nluSap[0]->pst;
   SuId             suId = wrCb.nluSap[0]->suId;
   S16              ret  = RFAILED;
   NluUeCqiRpt      *cqiRpt = NULLP;

   WR_ALLOC(&cqiRpt, sizeof(NluUeCqiRpt));
   if(NULLP == cqiRpt)
   {
      RLOG0(L_ERROR, "Failed to allocate memory for UE CQI RPT Ind");
      RETVALUE(RFAILED);
   }
   cmMemcpy((U8 *)cqiRpt, (U8 *)statInd, sizeof(NluUeCqiRpt));
   ret = WrLiNluUeCqiRptInd(pst, suId, cqiRpt);
   RETVALUE(ret);
}

#endif
/* @brief This function is used to send UE Measurement report toSON
 *
 * @details
 *
 *     This API is used to send UE Measurement report indication to SON
 *
 * @param[in] ueCb      : UE control block
 * @param[in] measRpt   : UE measurement report
 *
 * @return  S16
 *        -# Success : ROK
 *        -# failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 wrIfmSonUeMeasReportInd
(
WrUeCb           *ueCb,
NhuMeasResults   *measRpt
)
#else
PUBLIC S16 wrIfmSonUeMeasReportInd(ueCb, measRpt)
WrUeCb           *ueCb;
NhuMeasResults   *measRpt;
#endif
{
   Pst             *pst = &wrCb.nluSap[0]->pst;
   SuId             suId = wrCb.nluSap[0]->suId;
   S16              ret  = RFAILED;
   NhuMeasResultLstEUTRA        *measRsultLstEutra;
   U32              count;
   U32              idCnt;
   NluUeMeasRpt     *nluMeasRpt;
   WR_ALLOC(&nluMeasRpt, sizeof(NluUeMeasRpt));
   if(NULLP == nluMeasRpt)
   {
      RLOG0(L_ERROR, "Failed to allocate memory for UE MEASUREMENT RPT Ind");
      RETVALUE(RFAILED);
   }
   nluMeasRpt->ecgi.cellId = (U32)ueCb->cellId;
   cmMemcpy((U8 *)&nluMeasRpt->ecgi.plmn, (U8 *)&ueCb->plmnId, sizeof(WrPlmnId));
   nluMeasRpt->crnti = ueCb->crnti;
   nluMeasRpt->servCellRsrp = measRpt->measResultPCell.rsrpResult.val;
   nluMeasRpt->servCellRsrq = measRpt->measResultPCell.rsrqResult.val;
   /*store the neighbor information*/
   measRsultLstEutra = &measRpt->measResultNeighCells.val.measResultLstEUTRA;
   nluMeasRpt->noNghCell = measRsultLstEutra->noComp.val;
   for(count = 0; count <  nluMeasRpt->noNghCell; count ++)
   {
      nluMeasRpt->nghCellMeasRpt[count].pci = measRsultLstEutra->
                                 member[count].physCellId.val;
      /*DL-EARFCN of neighbor not required as of now*/
      nluMeasRpt->nghCellMeasRpt[count].nghCellRsrp = measRsultLstEutra->
                                 member[count].measResult.rsrpResult.val;
      nluMeasRpt->nghCellMeasRpt[count].nghCellRsrq =  measRsultLstEutra->
                                 member[count].measResult.rsrqResult.val;
      for(idCnt = 0; idCnt <  measRsultLstEutra->member[count].cgi_Info.
                    cellGlobalId.plmn_Identity.mcc.noComp.val; idCnt++)
      {
          nluMeasRpt->nghCellMeasRpt[count].ecgi.plmn.mcc[idCnt] = 
                       measRsultLstEutra->member[count].cgi_Info.
                    cellGlobalId.plmn_Identity.mcc.member[idCnt].val;
      }
      nluMeasRpt->nghCellMeasRpt[count].ecgi.plmn.numMncDigits =  
           measRsultLstEutra->member[count].cgi_Info.
                    cellGlobalId.plmn_Identity.mnc.noComp.val;
      for(idCnt = 0; idCnt <  measRsultLstEutra->member[count].cgi_Info.
                    cellGlobalId.plmn_Identity.mnc.noComp.val; idCnt++)
      {
          nluMeasRpt->nghCellMeasRpt[count].ecgi.plmn.mnc[idCnt] =
                       measRsultLstEutra->member[count].cgi_Info.
                    cellGlobalId.plmn_Identity.mnc.member[idCnt].val;
      }
      wrUtlGetU32FrmBSXL(&nluMeasRpt->nghCellMeasRpt[count].ecgi.cellId, &measRsultLstEutra->member[count].cgi_Info.
                    cellGlobalId.cellIdentity);
   }
   ret = WrLiNluUeMeasRptInd(pst, suId, nluMeasRpt);
   RETVALUE(ret);
}

/* @brief This function is used to send Delete UE indication to SON
 *
 * @details
 *
 *     This API is used to send delete UE indication to SON
 *     whenever any UE detached from eNB
 *
 * @param[in] crnti   : UE Identity
 * @param[in] cellId  : cell identity
 * @param[in] plmnId  : plmn Identity
 *
 * @return  S16
 *        -# Success : ROK
 *        -# failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 wrIfmSonDelUeInd
(
U16             crnti,   
U16             cellId,
WrPlmnId        *plmnId
)
#else
PUBLIC S16 wrIfmSonDelUeInd(crnti, cellId, plmnId)
U16             crnti;  
U16             cellId;
WrPlmnId        *plmnId;
#endif
{
   Pst             *pst = &wrCb.nluSap[0]->pst;
   SuId             suId = wrCb.nluSap[0]->suId;
   S16              ret  = RFAILED;
   NluUeInfo        *ueInfo = NULLP;
   WR_ALLOC(&ueInfo, sizeof(NluUeInfo));
   if(NULLP == ueInfo)
   {
      RLOG0(L_ERROR, "Failed to allocate memory for UE Dlete Ind");
      RETVALUE(RFAILED);
   }
   ueInfo->event = NLU_UE_DELETED;
   ueInfo->crnti = crnti;
   ueInfo->ecgi.cellId = cellId;
   cmMemcpy((U8 *)&ueInfo->ecgi.plmn, (U8 *)plmnId, sizeof(WrPlmnId));
   ret = WrLiNluUeInd(pst, suId, ueInfo);
   RETVALUE(ret);
}

/* @brief This function is used to send ADD UE indication to SON
 *
 * @details
 *
 *     This API is used to send Add UE indication to SON
 *     whenever any UE attached with eNB
 *
 * @param[in] ueCb   : UE control block
 *
 * @return  S16
 *        -# Success : ROK
 *        -# failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 wrIfmSonAddUeInd
(
WrUeCb          *ueCb
)
#else
PUBLIC S16 wrIfmSonAddUeInd(ueCb)
WrUeCb          *ueCb;
#endif
{
   Pst             *pst = &wrCb.nluSap[0]->pst;
   SuId             suId = wrCb.nluSap[0]->suId;
   S16              ret  = RFAILED;
   NluUeInfo        *ueInfo = NULLP;
   WR_ALLOC(&ueInfo, sizeof(NluUeInfo));
   if(NULLP == ueInfo)
   {
      RLOG0(L_ERROR, "Failed to allocate memory for UE Dlete Ind");
      RETVALUE(RFAILED);
   }
   ueInfo->event = NLU_UE_ADDED;
   ueInfo->crnti = ueCb->crnti;
   ueInfo->ecgi.cellId = ueCb->cellId;
   cmMemcpy((U8 *)&ueInfo->ecgi.plmn, (U8 *)&ueCb->plmnId, sizeof(WrPlmnId));
   ret = WrLiNluUeInd(pst, suId, ueInfo);
   RETVALUE(ret);
}

/* @brief This function is used to send UE CRNTI change indication to SON
 *
 * @details
 *
 *     This API is used to send UE CRNTI change indication to SON
 *     UE CRNTI may change during RRC Re-Establishment
 *
 * @param[in] ueCb   : UE control block
 *
 * @return  S16
 *        -# Success : ROK
 *        -# failure : RFAILED
 */
#ifdef ANSI
PUBLIC S16 wrIfmSonModUeInd
(
WrUeCb          *ueCb
)
#else
PUBLIC S16 wrIfmSonModUeInd(ueCb)
WrUeCb          *ueCb;
#endif
{
   Pst             *pst = &wrCb.nluSap[0]->pst;
   SuId             suId = wrCb.nluSap[0]->suId;
   S16              ret  = RFAILED;
   NluUeInfo        *ueInfo = NULLP;
   WR_ALLOC(&ueInfo, sizeof(NluUeInfo));
   if(NULLP == ueInfo)
   {
      RLOG0(L_ERROR, "Failed to allocate memory for UE Dlete Ind");
      RETVALUE(RFAILED);
   }
   ueInfo->event = NLU_UE_CRNTI_CHANGED;
   ueInfo->crnti = ueCb->crnti;
   ueInfo->oldCrnti = ueCb->oldCrnti;
   ueInfo->ecgi.cellId = ueCb->cellId;
   cmMemcpy((U8 *)&ueInfo->ecgi.plmn, (U8 *)&ueCb->plmnId, sizeof(WrPlmnId));
   ret = WrLiNluUeInd(pst, suId, ueInfo);
   RETVALUE(ret);
}


/* Periodic REM scan for TPM */
/* @brief This function is used to cell search response to SON 
 *
 * @details
 *
 *     Function: WrIfmSonPerCellSrchRsp
 *
 *     This API is used to send cell search response for continuous Freq Sync
 *     scan after receiving cell search response from L1.
 *
 * @param[in] cellCb   : cell control block
 *
 * @return  S16
 *        -# Success : ROK
 */
#ifdef ANSI
PUBLIC S16 WrIfmSonPerCellSrchRsp
(
CtfPeriodicRemCellSearchRsp *cellSrchInfo
)
#else
PUBLIC S16 WrIfmSonPerCellSrchRsp (cellSrchInfo)
CtfPeriodicRemCellSearchRsp *cellSrchInfo;
#endif
{
   Pst                          *pst = &wrCb.nluSap[0]->pst;
   SuId                         suId = wrCb.nluSap[0]->suId;
   S16                          ret  = RFAILED;
   NluPeriodicRemCellSearchRsp  *cellSrchRsp = NULLP;

   TRC2(WrIfmSonPerCellSrchRsp);

   RLOG0(L_INFO,"WrIfmSonPerCellSrchRsp: Building Message to SON");
   WR_ALLOC(&cellSrchRsp, sizeof(NluPeriodicRemCellSearchRsp));
   if(NULLP == cellSrchRsp)
   {
      RLOG0(L_ERROR, "Failed to allocate memory for Periodic Cell Search Response");
      RETVALUE(RFAILED);
   }

   cmMemcpy((U8 *)cellSrchRsp, (U8 *)cellSrchInfo,
         sizeof(NluPeriodicRemCellSearchRsp));

   ret = WrLiNluPerCellSrchRsp(pst, suId, cellSrchRsp);

   WR_FREE(cellSrchInfo, sizeof(CtfPeriodicRemCellSearchRsp));
   RETVALUE(ret);
}
/* Periodic REM scan for TPM End */

/********************************************************************30**

           End of file:     fc_ifm_son.c@@/main/TeNB_61750_TDD_EA2_4.0.0_SON/1 - Mon Dec 29 17:16:53 2014

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
       ---                       1. initial release TotaleNodeB 1.1
*********************************************************************91*/

