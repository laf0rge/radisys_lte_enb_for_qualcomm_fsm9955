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
     that has the corporate authority to enter U32o and perform under the 
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

     File:    qf_preEmptableERABFinder.cpp

     Sid:      qf_preEmptableERABFinder.cpp@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/11 - Mon Feb 15 12:52:03 2016

     Prg:     Ronins

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=156;

#include "rm_cmn.h"
#include "rm_utils.h"
#include "rm_qcibasedadmissioncontroller.h"
#include "rm_prbreportcollector.h"
#include "rm_preEmptableERABFinder.h"
#include "rm_prbreportcollector.h"
#include "rm_prbbasedadmissioncontroller.h"

CPreEmptableERABFinder::CPreEmptableERABFinder(LrmCellConfigData & stCellCfg)
{
  m_usMaxGBRPRBUsageLimit = stCellCfg.bMaxAvgGbrPrbUsage;
  m_bArpForEmergencyBearer = stCellCfg.bArpForEmergencyBearer;
}

#ifdef DBG_MAC_RRM_PRB_PRINT
void CPreEmptableERABFinder::displayErabMap()
{
   CPreEmptableERABFinder::RbListIterator   it;
   printf("\n[RRM] ERAB MULTI MAP ::\n");
   printf("\n=================================================================");
   printf("\nKey  |  isPreempted   |  ERAB Id   | Rqrd PRBs DL:UL]          ");
   printf("\n=================================================================\n");
   for(it = m_rabList.begin(); (it != m_rabList.end()) ; ++it)
   {
      printf("[%ld]   |    [%d]  |      [%d]   |  [%ld:%ld] \n", 
            (*it).first,
            ((*it).second)->getIsPrempted(), 
            ((*it).second)->getERABId(),
            ((*it).second)->getDlGBRPrb(),
            ((*it).second)->getUlGBRPrb());
   }
}
#endif


void CPreEmptableERABFinder::erabAdditionInd(CERABInfo* ptObjRbInfo,
                                             CUEContext* pUE)
{
   //generate key to add to ERAB List
   U32 key = generateKey(pUE->isCsgMember(),
                     ptObjRbInfo->isNonPreEmptableRab(),
                     ptObjRbInfo->getArp());

   insertToList(key, ptObjRbInfo);

   insertToQCIList((U32)ptObjRbInfo->getQci(), key, ptObjRbInfo);

   RLOG_ARG3(L_INFO,DBG_CRNTI,ptObjRbInfo->getRabCrnti(),
         "Inserting ERAB[%d] to list. QCI [%d] ARP [%d]",
         ptObjRbInfo->getERABId(),ptObjRbInfo->getQci(),ptObjRbInfo->getArp());

   return;
}


void CPreEmptableERABFinder::deleteERABInd(CERABInfo* ptObjErabInfo, 
                                           CUEContext* pUE)
{
   CPreEmptableERABFinder::RbListIterator it;
   CPreEmptableERABFinder::ErabIteratorPair itPair; 
   U32  usCrnti     = (U32)pUE->getCRNTI();
   U32  uiErabId    = ptObjErabInfo->getERABId();
   U32  uiQciVal    = ptObjErabInfo->getQci();
   U32  uiSearchKey = generateKey(pUE->isCsgMember(),
                                   ptObjErabInfo->isNonPreEmptableRab(),
                                   ptObjErabInfo->getArp());

   //Fetch the list of all ERABs bearing the same key belonging to same or
   //diferent UE
   /* delete from ERAB List */
   itPair = findERABsWithSameKey(uiSearchKey, &m_rabList);

   for(it = itPair.first;it != itPair.second; ++it)
   {
      U32 crnti = ((*it).second)->getRabCrnti();
      U32 erabId = ((*it).second)->getERABId();

      if((crnti == usCrnti) && (erabId == uiErabId))
      {
         RLOG_ARG1(L_INFO,DBG_CRNTI,usCrnti,"Removing ERAB[%d] from list",uiErabId);
         removeFromList(it);
         break;
      }
   }

   /* no need to remove from the QCI ERAB list if it is not a GBR bearer*/
   /* added for KlockWorks */
   if(isGbrBearer(uiQciVal))
   {
      /* delete from ERAB QCI List */
      itPair = findERABsWithSameKey(uiSearchKey, &m_rabQCIList[uiQciVal-1]);
      for(it = itPair.first;it != itPair.second; ++it)
      {
         U32 crnti = ((*it).second)->getRabCrnti();
         U32 erabId = ((*it).second)->getERABId();

         if((crnti == usCrnti) && (erabId == uiErabId))
         {
            RLOG_ARG2(L_INFO,DBG_CRNTI,usCrnti,"Removing ERAB[%d] from QCI[%d] list",uiErabId,uiQciVal);
            removeFromQCIList(((*it).second)->getQci(),it);
            break;
         }
      }
   }

   return;
}

void CPreEmptableERABFinder::erabModificationInd(CUEContext* pObjUE, 
                                                 CERABInfo* pObjERabInfo,
                                                 bool bIsCSGMember)
{
   U32   uiModifiedErabSearchKey;

   /* delete the RB entry from map table since Key can change because of CSG
    * membership change 
    */
   RLOG0(L_INFO,"Deleting ERAB during modification");
   deleteERABInd(pObjERabInfo, pObjUE);

   uiModifiedErabSearchKey = generateKey(bIsCSGMember,
                                         pObjERabInfo->isNonPreEmptableRab(),
                                         pObjERabInfo->getArp());

   insertToList(uiModifiedErabSearchKey, pObjERabInfo);

   insertToQCIList((U32)pObjERabInfo->getQci(),
                   uiModifiedErabSearchKey, 
                   pObjERabInfo); 

   RLOG_ARG3(L_INFO,DBG_CRNTI,pObjERabInfo->getRabCrnti(),
         "Inserting ERAB[%d] to list during modification. QCI [%d] ARP [%d]",
         pObjERabInfo->getERABId(),pObjERabInfo->getQci(),pObjERabInfo->getArp());

   return;
}

void CPreEmptableERABFinder::erabModificationInd(RmERabInfo* pstNewRbInfo,
                                                 CERABInfo* pObjOldERabInfo, 
                                                 CUEContext* pUE)
{
   U32   uiModifiedErabSearchKey;
   U32   uiModifiedErabIsNonPremptable;
   U32   uiModifiedErabArpVal; 

   /* delete the RB entry from map table since Key can change */
   RLOG0(L_INFO,"Deleting ERAB during modification");
   deleteERABInd(pObjOldERabInfo, pUE);

   uiModifiedErabIsNonPremptable = 
      (pstNewRbInfo->stErabQosInfo.stErabArpInfo.bErabPreemptVul == 0) ? 1: 0;

   uiModifiedErabArpVal = pstNewRbInfo->stErabQosInfo.stErabArpInfo.bErabPrioLvl;

   if(m_bArpForEmergencyBearer != uiModifiedErabArpVal)
   {
      uiModifiedErabSearchKey = generateKey(pUE->isCsgMember(),
            uiModifiedErabIsNonPremptable,
            uiModifiedErabArpVal);

      insertToList(uiModifiedErabSearchKey, pObjOldERabInfo);

      insertToQCIList((U32)pObjOldERabInfo->getQci(),
            uiModifiedErabSearchKey,
            pObjOldERabInfo);

      RLOG_ARG3(L_INFO,DBG_CRNTI,pObjOldERabInfo->getRabCrnti(),
            "Inserting ERAB[%d] to list during modification. QCI [%d] ARP [%d]",
            pObjOldERabInfo->getERABId(),pObjOldERabInfo->getQci(),pObjOldERabInfo->getArp());
   }
   return;
}

void CPreEmptableERABFinder::getDlUlPrbSum(CERABInfoLst & preEmptedERABs,
                                           U32 *uiDlPrbSum,
                                           U32 *uiUlPrbSum)
{
   std::list<CERABInfo*>::iterator rbListIt;

   for(rbListIt = preEmptedERABs.begin(); rbListIt != preEmptedERABs.end(); 
         rbListIt++)
   {
      (*uiDlPrbSum) += (*rbListIt)->getDlGBRPrb();
      (*uiUlPrbSum) += (*rbListIt)->getUlGBRPrb();
   }
   return;
}

bool CPreEmptableERABFinder::getErabsForPreemption(RbListIterator upperIt,
                                                   U16 usCRNTI, 
                                                   RrmErabRejected *pstRejectedErab,
                                                   U32 *uiDlPrbSum,
                                                   U32 *uiUlPrbSum,
                                                   CERABInfoLst &preEmptedERABs)
{
   bool isPrbReqSatisfied = false;
   CPreEmptableERABFinder::RbListIterator rbListIt;
   std::list<CERABInfo*> localErabsList;
   U32 uiReqDlPrb = pstRejectedErab->uiReqPrbForDl;
   U32 uiReqUlPrb = pstRejectedErab->uiReqPrbForUl;

   for(rbListIt = m_rabList.begin(); rbListIt != upperIt; rbListIt++)
   {
      CERABInfo *rbInfo = (*rbListIt).second;

      if(isPremptableErabBasedOnIncmoingErab(usCRNTI, pstRejectedErab, rbInfo))
      {
         (*uiDlPrbSum) += rbInfo->getDlGBRPrb();
         (*uiUlPrbSum) += rbInfo->getUlGBRPrb();

         RLOG2(L_DEBUG,"Required DL PRB %ld DL PRB Sum %ld",
               uiReqDlPrb,(*uiDlPrbSum));

         RLOG2(L_DEBUG,"Required UL PRB %ld UL PRB Sum %ld \n",
               uiReqUlPrb,(*uiUlPrbSum));

         RLOG_ARG1(L_INFO,DBG_CRNTI,rbInfo->getRabCrnti(),
               "Adding ERAB[%d] to pre-emption list for PRB requirement"
               , rbInfo->getERABId());

         //add to list of ERABs to be preempted
         localErabsList.push_back(rbInfo);
         rbInfo->setIsPrempted(true);
         rbInfo->setPremptedCause(RM_PREEMPTION_CAUSE_PRB);

         if((*uiDlPrbSum) >= uiReqDlPrb && (*uiUlPrbSum) >= uiReqUlPrb)
         {
            isPrbReqSatisfied = true;
            break;
         }
      }
   }

   if(isPrbReqSatisfied)
   {
      /* copy all RBs to preempted ERAB list */
      preEmptedERABs.insert(preEmptedERABs.end(), 
                            localErabsList.begin(),
                            localErabsList.end());
   }
   else
   {
      std::list<CERABInfo*>::iterator localErabsListIt;
      /* roll back and unset the preempted flag */
      for(localErabsListIt = localErabsList.begin();
            localErabsListIt != localErabsList.end();
            localErabsListIt++)
      {
         CERABInfo *rbInfo = (*localErabsListIt);
         rbInfo->setIsPrempted(false);
         rbInfo->setPremptedCause(RM_NOT_PREEMPTED);
         RLOG_ARG1(L_INFO,DBG_CRNTI,rbInfo->getRabCrnti(),
             "Reverting Pre-emption. Removing ERAB[%d] from pre-emption list"
             ,rbInfo->getERABId());

      }
   }
   return isPrbReqSatisfied;
}

CPreEmptableERABFinder::RbListIterator  
      CPreEmptableERABFinder::findSearchLimitIt(bool isCsgMember,
                                                U32  uiArpVal,
                                                ErabsMap *rabList)
{
   U32   uiKey;
   RbListIterator upperIt;

   if(m_bArpForEmergencyBearer == uiArpVal)
   {
      upperIt = rabList->end();
   }
   else if(MAX_ARP_VAL == uiArpVal)
   {
      // For ARP = 15, there shall exist no candidate ERABs for pre-emption
      upperIt = rabList->begin();
   }
   else if(isCsgMember == true)
   {
      // uiArpVal is increased by one here to avoid pre-empting a bearer having
      // same ARP as that of incoming bearer.
      uiKey = generateKey(CSG_MEMBER, VULNERABLE, (uiArpVal+1));
      upperIt = rabList->upper_bound(uiKey);
   }
   else
   {
      // uiArpVal is increased by one here to avoid pre-empting a bearer having
      // same ARP as that of incoming bearer.
      uiKey = generateKey(NON_CSG_MEMBER, VULNERABLE, (uiArpVal+1));
      upperIt = rabList->upper_bound(uiKey);
   }

   return upperIt;
}

void CPreEmptableERABFinder::findPreEmptableERABsForPRBRejection(
                                                 RrmRbcRspInfo & erabsAccRejLst,
                                                 ErabsRejMap rejectLstMap,
                                                 CERABInfoLst & preEmptedERABs,
                                                 bool isCsgMember)
{
   CPreEmptableERABFinder::RbRejListIterator  it;
   CPreEmptableERABFinder::RbListIterator   rbListIt;
   CPreEmptableERABFinder::RbListIterator   upperIt;
   U32   uiDlPrbSum = 0;
   U32   uiUlPrbSum = 0;
   U32   uiAcceptedDlPrbSum = 0;
   U32   uiAcceptedUlPrbSum = 0;
   U32   numRejERABs = erabsAccRejLst.nErabsRejected;
   U32   numAccERABs = erabsAccRejLst.nErabsAccepted;

   // get number of PRBs already preempted
   getDlUlPrbSum(preEmptedERABs, &uiDlPrbSum, &uiUlPrbSum);

   for(it = rejectLstMap.begin(); it != rejectLstMap.end(); it++)
   {
      bool isPrbReqSatisfied = false;
      U32 uiArpVal = ((*it).second)->bArpVal;
      U32 requiredDlPrb = ((*it).second)->uiReqPrbForDl;
      U32 requiredUlPrb = ((*it).second)->uiReqPrbForUl;

      if(((*it).second)->bIsModifyReq)
      {
         /*Take the diff in PRBs for erab modify*/
         ((*it).second)->uiReqPrbForDl = ((*it).second)->uiDiffPrbForDl;
         ((*it).second)->uiReqPrbForUl = ((*it).second)->uiDiffPrbForUl;
      }

      /* check PRB requirements for ERABs with only the below reject cause */
      if((*it).second->bCause != RMU_CAUSE_RADIO_RESOURCE_NOT_AVAIL &&
            (*it).second->bCause != RMU_CAUSE_CHECK_PRB_REQ)
      {
         continue;
      }

      if(false == ((*it).second)->bErabPreemptCap)
      {
         RLOG1(L_DEBUG,"ERAB [%d] incapable of preempting other bearers",
               ((*it).second)->bERabId);
         continue;
      }

      /* the DL PRB sum already preempted is suffient enough to */
      if((uiDlPrbSum >= (uiAcceptedDlPrbSum + requiredDlPrb)) && 
            (uiUlPrbSum >= (uiAcceptedUlPrbSum + requiredUlPrb)))
      {
         isPrbReqSatisfied = true;
      }
      else
      {
         upperIt = findSearchLimitIt(isCsgMember, uiArpVal, &m_rabList);

         isPrbReqSatisfied = getErabsForPreemption(upperIt, 
               erabsAccRejLst.usCRNTI,
               ((*it).second),
               &uiDlPrbSum,
               &uiUlPrbSum,
               preEmptedERABs);
      }

      if(isPrbReqSatisfied)
      {
         erabsAccRejLst.eRabAcceptedLst[numAccERABs].bERabId =
            ((*it).second)->bERabId;
         erabsAccRejLst.eRabAcceptedLst[numAccERABs].uiReqPrbForDl = requiredDlPrb;
         erabsAccRejLst.eRabAcceptedLst[numAccERABs].uiReqPrbForUl = requiredUlPrb;
         uiAcceptedDlPrbSum += requiredDlPrb; 
         uiAcceptedUlPrbSum += requiredUlPrb;
         numRejERABs--;
         numAccERABs++;
         RLOG_ARG1(L_INFO,DBG_CRNTI,erabsAccRejLst.usCRNTI,
               "[PRB]Accepting ERAB [%d] after pre-empting candidate ERABs"
               ,((*it).second)->bERabId);
      }
   }

   erabsAccRejLst.nErabsRejected = numRejERABs;
   erabsAccRejLst.nErabsAccepted = numAccERABs;

   return;
}

bool CPreEmptableERABFinder::isPremptableErabBasedOnIncmoingErab(
                                 U16 usCRNTI, 
                                 RrmErabRejected *pstRejectedErab,  
                                 CERABInfo *pobjCandidateErab)
{
   bool bIsPreEmpted = pobjCandidateErab->getIsPrempted();
   RmPremptionCause enCause = pobjCandidateErab->getPremptedCause();
   bool bIsPreemptableErab = false;
   
   /* a ERAB that was earlier preempted for QCI reasons is considered
    * here to check if PRB criteria is getting satisfied */
    if(((true == bIsPreEmpted) &&(enCause == RM_PREEMPTION_CAUSE_QCI)) ||
       (false == bIsPreEmpted))
    {
       bIsPreemptableErab = true;
    }
   
    /* same UE and same ERAB will not be pre-empted during ERAB modification */
     
    if((usCRNTI == pobjCandidateErab->getRabCrnti()) &&
       (pstRejectedErab->bERabId == pobjCandidateErab->getERABId()))
    {
       bIsPreemptableErab = false;
    }

   return bIsPreemptableErab;
}

void CPreEmptableERABFinder::findPreEmptableERABsForQCIRejection(
                                               RrmRbcRspInfo & erabsAccRejLst,
                                               ErabsRejMap rejectLstMap,
                                               CERABInfoLst & preEmptedERABs,
                                               bool isCSGMember)
{
   CPreEmptableERABFinder::RbRejListIterator  it;
   CPreEmptableERABFinder::RbListIterator     rbListIt;
   CPreEmptableERABFinder::RbListIterator     upperIt;

   for(it = rejectLstMap.begin(); it != rejectLstMap.end(); it++)
   {
      bool isPreempt = false;
      U32 uiQciVal = ((*it).second)->uiQci;
      U32 uiArpVal = ((*it).second)->bArpVal;

      if((*it).second->bCause != RMU_CAUSE_MAX_QCI_REACHED)
      {
         continue;
      }

      if(false == ((*it).second)->bErabPreemptCap)
      {
         RLOG1(L_DEBUG,"ERAB [%d] incapable of preempting other bearers",
               ((*it).second)->bERabId);
         continue;
      }

      upperIt = findSearchLimitIt(isCSGMember,
                                  uiArpVal,
                                  &m_rabQCIList[uiQciVal-1]);

      /* upper_bound returns 'node' after 'key', (OR) 
       * upper_bound returns list.end() if no 'nodes' are after 'key' */
      /* By checking if begin != upperIt, it is made sure that the node which
       * do not fall within 'key' limit is not preempted. */
      for(rbListIt = m_rabQCIList[uiQciVal-1].begin(); rbListIt != upperIt;
            rbListIt++)
      {
         if(isPremptableErabBasedOnIncmoingErab(erabsAccRejLst.usCRNTI, ((*it).second),  ((*rbListIt).second) ))
         {
            RLOG_ARG1(L_INFO,DBG_CRNTI,((*rbListIt).second)->getRabCrnti(),
                  "Adding ERAB[%d] to pre-emption list for QCI requirement"
                  , ((*rbListIt).second)->getERABId());
            preEmptedERABs.push_back((*rbListIt).second);
            ((*rbListIt).second)->setIsPrempted(true);
            ((*rbListIt).second)->setPremptedCause(RM_PREEMPTION_CAUSE_QCI);
            isPreempt = true;
            break;
         }
      }

      if(isPreempt)
      {
         (*it).second->bCause = RMU_CAUSE_CHECK_PRB_REQ;
      }
   }

   return;
}
#ifdef RRM_UNUSED_FUNC
RmuUErabAddModIe *CPreEmptableERABFinder::getErabAddModIe(
                                                 RmuErabConfigIe &stERAB,
                                                 U32 uiErabId)
{
   for (U32 bErabIndex =0; bErabIndex < stERAB.usNoErabsToAddMod; bErabIndex++)
   {
      if(uiErabId == stERAB.stErabAddModLst[bErabIndex].bErabId)
      {
         RETVALUE(&(stERAB.stErabAddModLst[bErabIndex]));
      }
   }
   RETVALUE(NULL);
}
#endif
void CPreEmptableERABFinder::reorderRejectedERABsList(RrmRbcRspInfo & erabsAccRejLst)
{
   RrmErabRejected   *eRabRejectedLst = erabsAccRejLst.eRabRejectedLst;
   RrmErabAdmitted   *eRabAcceptedLst = erabsAccRejLst.eRabAcceptedLst;
   RrmErabRejected   newERABRejLst[RMU_MAX_NUM_ERABS];

   U32   numERABsRej = erabsAccRejLst.nErabsRejected;
   U32   numERABsAcc = erabsAccRejLst.nErabsAccepted;
   U32   numERABsMoved = erabsAccRejLst.nErabsAccepted - 
                         erabsAccRejLst.nErabsRejected;
   U32   idx = 0;
   U32   idx_1 = 0;
   U32   idx_2 = numERABsAcc;

   for(; idx < (numERABsRej + numERABsMoved); idx++)
   {
      if(eRabAcceptedLst[idx_2-1].bERabId == 
            eRabRejectedLst[idx].bERabId)
      {
         eRabRejectedLst[idx].bERabId = 0;
         idx = 0;//starting the Reject List from beginning for next ERAB
         idx_2--;
      }
   }

   for(idx = 0; idx < (numERABsRej + numERABsMoved); idx++)
   {
      if(eRabRejectedLst[idx].bERabId)
      {
         newERABRejLst[idx_1] = eRabRejectedLst[idx]; 
         idx_1++;
      }
   }
   
   /* Reset old eRabRejectedLst*/
   memset(eRabRejectedLst,0,RMU_MAX_NUM_ERABS * sizeof(RrmErabRejected));

   /* copy newERABRejLst to old eRabRejectedLst*/
   memcpy(eRabRejectedLst, newERABRejLst,RMU_MAX_NUM_ERABS * sizeof(RrmErabRejected));

   return;
}

void  CPreEmptableERABFinder::createOrderedRejectedERABslist(
                                              RrmErabRejected *eRabRejectedLst,
                                              bool isCsgMember,
                                              ErabsRejMap &rejectLstMap,
                                              U32 numERABsRej)
{
   for(U32 idx = 0; idx < numERABsRej; idx++)
   {
      // Highest priority ERAB is ordered first in the reject list unlike other
      // lists. Hence (MAX_ARP_VAL - ARP)
      U32  key = generateKey(isCsgMember, 
                             !(eRabRejectedLst[idx].bIsPreemptable),
                             (MAX_ARP_VAL - eRabRejectedLst[idx].bArpVal));

      rejectLstMap.insert(std::pair<U32,RrmErabRejected*> (key,
               &eRabRejectedLst[idx])); 
   }
   return;
}

bool CPreEmptableERABFinder::findPreEmptableERABs(bool isCsgMember,
                                                RrmRbcRspInfo & erabsAccRejLst,
                                                CERABInfoLst & preEmptedERABs)
{
   CPreEmptableERABFinder::ErabsRejMap  rejectLstMap; 
   RrmErabRejected *eRabRejectedLst = erabsAccRejLst.eRabRejectedLst;
   U32  uiNumRejErabs = erabsAccRejLst.nErabsRejected;

   /* Sort the incoming Rejected list based on key so that the highest priority
    * ERABs can be admitted first */

   createOrderedRejectedERABslist(eRabRejectedLst, 
         isCsgMember,
         rejectLstMap,
         erabsAccRejLst.nErabsRejected);

   findPreEmptableERABsForQCIRejection(erabsAccRejLst,
                                       rejectLstMap,
                                       preEmptedERABs,
                                       isCsgMember);

   findPreEmptableERABsForPRBRejection(erabsAccRejLst,
                                       rejectLstMap,
                                       preEmptedERABs,
                                       isCsgMember);

   if(uiNumRejErabs != erabsAccRejLst.nErabsRejected)
   {
      reorderRejectedERABsList(erabsAccRejLst);
   }

   return true;
}

bool CPreEmptableERABFinder::findLeastPriorityPreEmptableERAB(
                                               CERABInfoLst & preEmptedERABs)
{
   CPreEmptableERABFinder::RbListIterator  rbListIt;

   for(rbListIt = m_rabList.begin(); rbListIt != m_rabList.end(); rbListIt++)
   {
      bool bIsPreEmpted = ((*rbListIt).second)->getIsPrempted();
      if(false == bIsPreEmpted)
      {
         preEmptedERABs.push_back((*rbListIt).second);
         ((*rbListIt).second)->setIsPrempted(true);
         ((*rbListIt).second)->setPremptedCause(RM_PREEMPTION_CAUSE_PRB_CONG);
         break;
      }
   }
   return (preEmptedERABs.size());
}

/********************************************************************30**

           End of file:     qf_preEmptableERABFinder.cpp@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/11 - Mon Feb 15 12:52:03 2016

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



