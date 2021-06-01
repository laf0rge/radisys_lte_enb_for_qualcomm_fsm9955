/*******************************************************************16**

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

     Name:     RRM Application

     Type:     CPP source file

     Desc:     This file contains 

     File:     rm_prbbasedadmissioncontroller.cpp

     Sid:      qf_prbbasedadmissioncontroller.cpp@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/11 - Mon Feb 15 12:52:01 2016

     Prg:      Amit Ghosh

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=153;

#include <math.h> 
#include "rm_prbbasedadmissioncontroller.h"
#include "rm_outinterface.h"
#include "rm_prbreportcollector.h"
#include "rm_utils.h"
#include "rm_cmn.h"

#define RM_NUM_ITBS              27   /*!< Default iTBS */
#define RM_REFER_NUM_PRBS        100  /*!< Maximum number of PRBs */
#define RM_NUM_MILLI_SEC_PER_SEC 1000 /*!< Number of milliseconds in a second */
#define RM_MIN_REQ_PRB           1    /*!< Minimum PRB that can be allocated */

/* iTBs to Bits for 100 PRBs and for one code word 
 * spec:36213-section 7.1.7.2.1 */
/* @def iTBS to bits mapping for 100 PRBs */
PRIVATE U32  g_uiiTbsToBitsfor100Prbs[RM_NUM_ITBS] = {
   2792,  3624,  4584,  5736,  7224,  8760, 10296, 12216, 14112,
   15840, 17568, 19848, 22920, 25456, 28336, 30576, 32856, 36696,
   39232, 43816, 46888, 51024, 55056, 57336, 61664, 63776, 75376
} ;

S16 CPRBBasedAdmissionController::sendPrbReportCfg(U8 bCellId, U32 bAction)
{
   RgmPrbRprtCfg             *prbRprtCfg = NULLP;

   RM_ALLOC(&prbRprtCfg, sizeof(RgmPrbRprtCfg));
   if(prbRprtCfg == NULLP)
   {
      RLOG0(L_ERROR, "PRB Report Configuration Send Failure");
      RLOG1(L_ERROR, "Memory allocation failed [size: %d]", 
            sizeof(RgmPrbRprtCfg));
      RETVALUE(RFAILED);
   }

   prbRprtCfg->bCellId            = bCellId;
   prbRprtCfg->bConfigType        = (U8)bAction;
   prbRprtCfg->usPrbAvgPeriodicty = m_uiPrbReportInterval;

   rrmSendCellCfgPrbReport(prbRprtCfg);

   RETVALUE(ROK);
}

CPRBBasedAdmissionController::CPRBBasedAdmissionController(LrmCellConfigData & stCellCfg)
{
   m_bDuplexMode = stCellCfg.bDuplexMode;
   m_enConfigMode = stCellCfg.enUlDlConfig;

   if((m_bDuplexMode == RM_MODE_TDD) && 
         (m_enConfigMode >= LRM_UL_DL_CONFIG_MAX))
   {
      RLOG2(L_ERROR, "Invalid config mode:%d. Taking default value %d",
            m_enConfigMode, LRM_UL_DL_CONFIG_6);
      m_enConfigMode = LRM_UL_DL_CONFIG_6;
   }

   m_usMaxGBRPRBUsageLimit = stCellCfg.bMaxAvgGbrPrbUsage;
   m_bCellId = stCellCfg.bCellId;
   m_bNumTxAnt = stCellCfg.bNumTxAnt;
   m_bDefaultiTbs = 17;
   m_uiPrbReportInterval = stCellCfg.utPrbRptInterval;
   m_enDlBw = stCellCfg.enCellDlBandWidth; 
   
   /* configure PRB report collector */
   CPRBReportCollector::getInstance().configurePRBReportCollector(
         stCellCfg.utPrbRptInterval, stCellCfg.utNumOfPrbRpts);

   RLOG_ARG4(L_INFO, DBG_CELLID, (U32)stCellCfg.bCellId, 
         "PRB CFG: NumAnt:%d  MaxGBRPRB Limit: %d "
         "ReportInterval: %ld WindowSize: %ld", 
         m_bNumTxAnt, m_usMaxGBRPRBUsageLimit, 
         m_uiPrbReportInterval, stCellCfg.utNumOfPrbRpts);
}

CPRBBasedAdmissionController::~CPRBBasedAdmissionController()
{
   /* clear PRB report collector instance */
   CPRBReportCollector::getInstance().clearPrbUsageData();
}

U32 CPRBBasedAdmissionController::getNumPrbsRequiredForBitRate(U32 uiRcvdGbrBitsPerSec, 
                                                               bool bIsDlGbrBitRate,
                                                               U32 biTbs)
{
   U32   uiMaxNumOfbitsFor100Rbs; 
   U32   uiReqrdNumPrbsPerMilliSec;
   U32   uiRcvdGbrPerMilliSec;

   uiMaxNumOfbitsFor100Rbs = g_uiiTbsToBitsfor100Prbs[biTbs];

   uiRcvdGbrPerMilliSec = (U32)RM_ROUND(uiRcvdGbrBitsPerSec, RM_NUM_MILLI_SEC_PER_SEC);
   uiReqrdNumPrbsPerMilliSec = 
      (U32)RM_ROUND((uiRcvdGbrPerMilliSec * RM_REFER_NUM_PRBS), uiMaxNumOfbitsFor100Rbs) ;
   
   /* If it is DL data rate; consider is it SISO or MIMO */ 
   if(bIsDlGbrBitRate)
   {
      return ((U32)RM_ROUND(uiReqrdNumPrbsPerMilliSec, m_bNumTxAnt));
   }
   return (uiReqrdNumPrbsPerMilliSec);
}

U32 CPRBBasedAdmissionController::getReqPrbInPercent(U32 uiRcvdGbrBitsPerSec,
                                                    bool bIsDlGbrBitRate,
                                                    U32 biTbs)
{
   U32   uiReqrdNumPrbsPerMilliSec;
   U8    bTotalAvailRbs;
   U8    bRqrdPrbsInPercent;

   uiReqrdNumPrbsPerMilliSec = getNumPrbsRequiredForBitRate(uiRcvdGbrBitsPerSec,
                                                            bIsDlGbrBitRate,
                                                            biTbs);
   bTotalAvailRbs = getNumPrbs(m_enDlBw);

   /* The available PRBs will be  different when 
    * we compare with TDD and FDD */
   if(RM_MODE_TDD == m_bDuplexMode)
   {
      if(bIsDlGbrBitRate)
      {
         U8    bNumDlSfs = getNumDlSubFramesPerFrame(m_enConfigMode);
         bTotalAvailRbs = (bTotalAvailRbs * bNumDlSfs)/RM_NUM_SF_PER_SFN;
         RLOG2(L_DEBUG, "DL Required PRB/MS: %ld, AvailRBs: %d ",
               uiReqrdNumPrbsPerMilliSec,
               bTotalAvailRbs);
      }
      else
      {
         U8    bNumUlSfs = getNumUlSubFramesPerFrame(m_enConfigMode);
         bTotalAvailRbs = (bTotalAvailRbs * bNumUlSfs)/RM_NUM_SF_PER_SFN;
         RLOG2(L_DEBUG, "UL Required PRB/MS: %ld, AvailRBs: %d ",
               uiReqrdNumPrbsPerMilliSec,
               bTotalAvailRbs);
      }
   }

   /* convert into the percentage */
   bRqrdPrbsInPercent = (U8) RM_ROUND((uiReqrdNumPrbsPerMilliSec * 100), bTotalAvailRbs) ;
   if(!bRqrdPrbsInPercent)
   {
      return RM_MIN_REQ_PRB;
   }
   return bRqrdPrbsInPercent;
}

S16 CPRBBasedAdmissionController::triggerPrbReportConfig(U8 bCellId,
                                                         U32 bAction)
{
   RETVALUE(sendPrbReportCfg(bCellId, bAction));
}


void CPRBBasedAdmissionController::reorderAcceptList(
                                          RrmRbcRspInfo & erabRbcRspInfo)
{
   U32   uiRbIdx;
   U32   uiRbIdx2;

   for(uiRbIdx = 0; uiRbIdx < erabRbcRspInfo.nErabsAccepted; uiRbIdx++)
   {
      if(erabRbcRspInfo.eRabAcceptedLst[uiRbIdx].bERabId == 0)
      {
         for(uiRbIdx2 = uiRbIdx + 1; uiRbIdx2 < erabRbcRspInfo.nErabsAccepted && uiRbIdx2 < RMU_MAX_NUM_ERABS;
               uiRbIdx2++)
         {
            if(erabRbcRspInfo.eRabAcceptedLst[uiRbIdx2].bERabId != 0)
            {
               erabRbcRspInfo.eRabAcceptedLst[uiRbIdx].bERabId = 
                  erabRbcRspInfo.eRabAcceptedLst[uiRbIdx2].bERabId;
               erabRbcRspInfo.eRabAcceptedLst[uiRbIdx2].bERabId = 0;
               break;
            }
         }
      }
   }
}

bool CPRBBasedAdmissionController::canModifyRBsBeAdmitted(CUEContext *pUE,
                                                RmuErabConfigIe & stERAB,
                                                RrmRbcRspInfo & erabRbcRspInfo)
{
   U32               bNumErabsAdmitted;
   U32               bNumErabsRejected;
   U32               uiRbIdx;
   RrmErabAdmitted   *pAcceptLst;
   RrmErabRejected   *pRejectLst;
   U32               bCurrDlPrbUsage;
   U32               bCurrUlPrbUsage;
   U32               accDlPrbCount = 0;
   U32               accUlPrbCount = 0;

   bNumErabsAdmitted = erabRbcRspInfo.nErabsAccepted;
   bNumErabsRejected = erabRbcRspInfo.nErabsRejected;
   pAcceptLst = erabRbcRspInfo.eRabAcceptedLst;
   pRejectLst = erabRbcRspInfo.eRabRejectedLst;


   /* get current usage for DL*/
   bCurrDlPrbUsage = CPRBReportCollector::getInstance().getCurrentGbrDlPrbUsage();

   /* get current usage for UL*/
   bCurrUlPrbUsage = CPRBReportCollector::getInstance().getCurrentGbrUlPrbUsage();

   /* go through the accepted list */
   for(uiRbIdx = 0; uiRbIdx < erabRbcRspInfo.nErabsAccepted; uiRbIdx++)
   {
      U8    bErabId;
      U32   bReqPrbForDl;
      U32   bReqPrbForUl;
      U32   uiReservedPrbForDl;
      U32   uiReservedPrbForUl;      
      U8    bQci; 

      bErabId = pAcceptLst[uiRbIdx].bERabId;

      RmuUErabAddModIe *stRbAddMod = getRbAddModIe(stERAB, bErabId);
      CERABInfo* pObjOldERab = pUE->findERAB(bErabId);

      if((NULL == stRbAddMod) || (NULL == pObjOldERab))
      {
         RLOG_ARG1(L_ERROR, DBG_CRNTI, pUE->getCRNTI(),
               "ErabId %d not found", bErabId);
         continue;
      }

      RmuErabGbrInfoIe &stGbrInfo  = stRbAddMod->stErabQosInfo.stErabGbrInfo;
      bQci        = stRbAddMod->stErabQosInfo.bErabQCI;

      if(isGbrBearer(bQci))
      {
         /* get required percentage for DL */
         /* get OLD prb% and compare with the NEW prb%, if OLD > NEW, pass
          * bReqPrbForDl = 0 else pass NEW prb%*/
         bReqPrbForDl = getReqPrbInPercent(stGbrInfo.uiErabDlGbr, true,
               m_bDefaultiTbs);

         /* get required percentage for UL */
         /* get OLD prb% and compare with the NEW prb%, if OLD > NEW, pass
          * bReqPrbForUl = 0 else pass NEW prb%*/
         bReqPrbForUl = getReqPrbInPercent(stGbrInfo.uiErabUlGbr, false,
               m_bDefaultiTbs);


         /* get required percentage for DL GBR which is already reserved */
         uiReservedPrbForDl = pObjOldERab->getDlGBRPrb();

         /* get required percentage for DL GBR which is already reserved */
         uiReservedPrbForUl = pObjOldERab->getUlGBRPrb();

         if(((RM_ABS_VAL((accDlPrbCount + bCurrDlPrbUsage + bReqPrbForDl),uiReservedPrbForDl))
                   > m_usMaxGBRPRBUsageLimit) ||
            ((RM_ABS_VAL((accUlPrbCount + bCurrUlPrbUsage + bReqPrbForUl),uiReservedPrbForUl)) 
                  > m_usMaxGBRPRBUsageLimit))
         {
            /* reject the bearer */
            /* remove from accept list - reordering shall be done at the end */
            pAcceptLst[uiRbIdx].bERabId = 0;
            bNumErabsAdmitted--;
            /* add to reject list */
            pRejectLst[bNumErabsRejected].bERabId = bErabId;
            pRejectLst[bNumErabsRejected].bCause =
               RMU_CAUSE_RADIO_RESOURCE_NOT_AVAIL;
            pRejectLst[bNumErabsRejected].uiReqPrbForDl = bReqPrbForDl;
            pRejectLst[bNumErabsRejected].uiReqPrbForUl = bReqPrbForUl;
            if((bReqPrbForDl - uiReservedPrbForDl) > 0)
            {
               pRejectLst[bNumErabsRejected].uiDiffPrbForDl = 
                  bReqPrbForDl - uiReservedPrbForDl;
            }
            if((bReqPrbForUl - uiReservedPrbForUl) > 0)
            {
               pRejectLst[bNumErabsRejected].uiDiffPrbForUl =
                  bReqPrbForUl - uiReservedPrbForUl;
            }
            pRejectLst[bNumErabsRejected].bIsModifyReq = true;
            pRejectLst[bNumErabsRejected].uiQci = 
               stRbAddMod->stErabQosInfo.bErabQCI;
            /* Update ARP value */
            pRejectLst[bNumErabsRejected].bArpVal = 
               stRbAddMod->stErabQosInfo.stErabArpInfo.bErabPrioLvl;
            pRejectLst[bNumErabsRejected].bIsPreemptable = 
               stRbAddMod->stErabQosInfo.stErabArpInfo.bErabPreemptVul;
            pRejectLst[bNumErabsRejected].bErabPreemptCap = 
               stRbAddMod->stErabQosInfo.stErabArpInfo.bErabPreemptCap;
            bNumErabsRejected++;
            RLOG_ARG4(L_INFO, DBG_CRNTI, pUE->getCRNTI(),
                  "[Rejected-ERAB-PRB] DL Curr: %ld DL Req: %ld "
                  "UL Curr: %ld UL Req: %ld",
                  bCurrDlPrbUsage, bReqPrbForDl, 
                  bCurrUlPrbUsage, bReqPrbForUl);
            continue;
         }

         /* Update the required PRB for both DL and UL */
         pAcceptLst[uiRbIdx].uiReqPrbForDl = bReqPrbForDl;
         pAcceptLst[uiRbIdx].uiReqPrbForUl = bReqPrbForUl;
         accDlPrbCount = (accDlPrbCount + bReqPrbForDl) - uiReservedPrbForDl;
         accUlPrbCount = (accUlPrbCount + bReqPrbForUl) - uiReservedPrbForUl;

         RLOG_ARG3(L_INFO, DBG_CRNTI, pUE->getCRNTI(),
               "[Accepted-ERAB-PRB] DL Curr: %ld DL Req: %ld DL reserved:%ld",
               bCurrDlPrbUsage, bReqPrbForDl, uiReservedPrbForDl);
         RLOG_ARG3(L_INFO, DBG_CRNTI, pUE->getCRNTI(),
               "[Accepted-ERAB-PRB] UL Curr: %ld UL Req: %ld UL Reserved:%ld",
               bCurrUlPrbUsage, bReqPrbForUl, uiReservedPrbForUl);
      }
   }

   /* reorder accept list if any bearer was removed from the list */
   if(bNumErabsAdmitted && 
          (bNumErabsAdmitted != erabRbcRspInfo.nErabsAccepted))
   {
      reorderAcceptList(erabRbcRspInfo);
   }

   RLOG_ARG2(L_INFO, DBG_CRNTI, pUE->getCRNTI(),
         "Number of Admitted/Rejected ERABS:: [%d, %d]",
         bNumErabsAdmitted, bNumErabsRejected);
   /* update number of admitted and rejected bearers to rbc response */
   erabRbcRspInfo.nErabsAccepted = bNumErabsAdmitted;
   erabRbcRspInfo.nErabsRejected = bNumErabsRejected;

   if(!bNumErabsAdmitted)
   {
      RETVALUE(false);
   }
   RETVALUE(true);
}

bool CPRBBasedAdmissionController::canRBsBeAdmitted(
                                           RmuErabConfigIe & stERAB,
                                           RrmRbcRspInfo & erabRbcRspInfo) 
{
   U32               bNumErabsAdmitted;
   U32               bNumErabsRejected;
   U32               uiRbIdx;
   RrmErabAdmitted   *pAcceptLst;
   RrmErabRejected   *pRejectLst;
   U32               bCurrDlPrbUsage;
   U32               bCurrUlPrbUsage;
   U32               accDlPrbCount = 0;
   U32               accUlPrbCount = 0;

   bNumErabsAdmitted = erabRbcRspInfo.nErabsAccepted;
   bNumErabsRejected = erabRbcRspInfo.nErabsRejected;
   pAcceptLst = erabRbcRspInfo.eRabAcceptedLst;
   pRejectLst = erabRbcRspInfo.eRabRejectedLst;

   /* get current usage for DL*/
   bCurrDlPrbUsage = CPRBReportCollector::getInstance().getCurrentGbrDlPrbUsage();

   /* get current usage for UL*/
   bCurrUlPrbUsage = CPRBReportCollector::getInstance().getCurrentGbrUlPrbUsage();

   /* go through the accepted list */
   for(uiRbIdx = 0; uiRbIdx < erabRbcRspInfo.nErabsAccepted; uiRbIdx++)
   {
      U8    bErabId;
      U32   uiReqPrbForDl;
      U32   uiReqPrbForUl;
      U8    bQci; 

      bErabId = pAcceptLst[uiRbIdx].bERabId;
      RmuUErabAddModIe *stRbAddMod = getRbAddModIe(stERAB, bErabId);
      if(stRbAddMod == NULL)
      {
         RLOG_ARG1(L_ERROR, DBG_CRNTI, erabRbcRspInfo.usCRNTI,
               "ErabId %d not found", bErabId);
         continue;
      }
      RmuErabGbrInfoIe &stGbrInfo  = stRbAddMod->stErabQosInfo.stErabGbrInfo;
      bQci        = stRbAddMod->stErabQosInfo.bErabQCI;

      if(isGbrBearer(bQci))
      {
         /* get required percentage for DL */
         uiReqPrbForDl = getReqPrbInPercent(stGbrInfo.uiErabDlGbr, true,
               m_bDefaultiTbs);

         /* get required percentage for UL */
         uiReqPrbForUl = getReqPrbInPercent(stGbrInfo.uiErabUlGbr, false,
               m_bDefaultiTbs);

         if((accDlPrbCount + bCurrDlPrbUsage + uiReqPrbForDl) > m_usMaxGBRPRBUsageLimit ||
               (accUlPrbCount + bCurrUlPrbUsage + uiReqPrbForUl) > m_usMaxGBRPRBUsageLimit)
         {
            /* reject the bearer */
            /* remove from accept list - reordering shall be done at the end */
            pAcceptLst[uiRbIdx].bERabId = 0;
            bNumErabsAdmitted--;
            /* add to reject list */
            pRejectLst[bNumErabsRejected].bERabId = bErabId;
            pRejectLst[bNumErabsRejected].bCause =
               RMU_CAUSE_RADIO_RESOURCE_NOT_AVAIL;
            pRejectLst[bNumErabsRejected].uiReqPrbForDl = uiReqPrbForDl;
            pRejectLst[bNumErabsRejected].uiReqPrbForUl = uiReqPrbForUl;
            pRejectLst[bNumErabsRejected].uiQci = 
               stRbAddMod->stErabQosInfo.bErabQCI;
            /* Update ARP value */
            pRejectLst[bNumErabsRejected].bArpVal = 
               stRbAddMod->stErabQosInfo.stErabArpInfo.bErabPrioLvl;
            pRejectLst[bNumErabsRejected].bIsPreemptable = 
               stRbAddMod->stErabQosInfo.stErabArpInfo.bErabPreemptVul;
            pRejectLst[bNumErabsRejected].bErabPreemptCap = 
               stRbAddMod->stErabQosInfo.stErabArpInfo.bErabPreemptCap;
            bNumErabsRejected++;
            RLOG_ARG4(L_INFO, DBG_CRNTI, erabRbcRspInfo.usCRNTI,
                  "[Rejected-ERAB-PRB] DL Curr: %ld DL Req: %ld "
                  "UL Curr: %ld UL Req: %ld",
                  bCurrDlPrbUsage, uiReqPrbForDl, 
                  bCurrUlPrbUsage, uiReqPrbForUl);
            continue;
         }

         /* Update the required PRB for both DL and UL */
         pAcceptLst[uiRbIdx].uiReqPrbForDl = uiReqPrbForDl;
         pAcceptLst[uiRbIdx].uiReqPrbForUl = uiReqPrbForUl;
         accDlPrbCount += uiReqPrbForDl;
         accUlPrbCount += uiReqPrbForUl;

         RLOG_ARG4(L_INFO, DBG_CRNTI, erabRbcRspInfo.usCRNTI,
               "[Accepted-ERAB-PRB] DL Curr: %ld DL Req: %ld "
               "UL Curr: %ld UL Req: %ld",
               bCurrDlPrbUsage, uiReqPrbForDl, bCurrUlPrbUsage, uiReqPrbForUl);
      }
   }

   RLOG_ARG2(L_INFO, DBG_CRNTI, erabRbcRspInfo.usCRNTI,
         "Number of Admitted/Rejected ERABS:: [%d, %d]",
         bNumErabsAdmitted, bNumErabsRejected);

   /* reorder accept list if any bearer was removed from the list */
   if(bNumErabsAdmitted && 
          (bNumErabsAdmitted != erabRbcRspInfo.nErabsAccepted))
   {
      reorderAcceptList(erabRbcRspInfo);
   }

   /* update number of admitted and rejected bearers to rbc response */
   erabRbcRspInfo.nErabsAccepted = bNumErabsAdmitted;
   erabRbcRspInfo.nErabsRejected = bNumErabsRejected;

   if(!bNumErabsAdmitted)
   {
      RETVALUE(false);
   }
   RETVALUE(true);
}

void CPRBBasedAdmissionController::updatePrbUsageForRejErabs(
                                               RmuErabConfigIe & stERAB,
                                               RrmRbcRspInfo & erabRbcRspInfo)
{
   RrmErabRejected   *pRejectLst = erabRbcRspInfo.eRabRejectedLst;

   /* go through the rejected list to update the required PRB usage for each
    * rejected bearer*/
   for(U32 uiRbIdx = 0; uiRbIdx < erabRbcRspInfo.nErabsRejected; uiRbIdx++)
   {
      U8    bErabId = pRejectLst[uiRbIdx].bERabId;
      U32   uiReqPrbForDl;
      U32   uiReqPrbForUl;

      RmuUErabAddModIe *stRbAddMod = getRbAddModIe(stERAB, bErabId);

      if(stRbAddMod == NULL)
      {
         RLOG_ARG1(L_ERROR, DBG_CRNTI, erabRbcRspInfo.usCRNTI,
               "ErabId %d not found", bErabId);
         continue;
      }

      RmuErabGbrInfoIe &stGbrInfo  = stRbAddMod->stErabQosInfo.stErabGbrInfo;
      /* get required percentage for DL */
      uiReqPrbForDl = getReqPrbInPercent(stGbrInfo.uiErabDlGbr, true,
            m_bDefaultiTbs);

      /* get required percentage for UL */
      uiReqPrbForUl = getReqPrbInPercent(stGbrInfo.uiErabUlGbr, false,
            m_bDefaultiTbs);
   
      /* Update the required PRB for both DL and UL */
      pRejectLst[uiRbIdx].uiReqPrbForDl = uiReqPrbForDl;
      pRejectLst[uiRbIdx].uiReqPrbForUl = uiReqPrbForUl;

      /* Update ARP value */
      pRejectLst[uiRbIdx].bArpVal = 
         stRbAddMod->stErabQosInfo.stErabArpInfo.bErabPrioLvl;
   }/*end of for loop for rejected list*/

   return;
}

/********************************************************************30**

           End of file:     qf_prbbasedadmissioncontroller.cpp@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/11 - Mon Feb 15 12:52:01 2016

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
*********************************************************************91*/


