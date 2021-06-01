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

     Name:     RRM Application

     Type:     CPP source file

     Desc:     This file contains 
               

     File:     rm_uemanager.cpp

     Sid:      qf_uemanager.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:37 2013

     Prg:     hkrishna 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=149;
/* header include files */
#include <iostream>
#include <memory>
#include <new>
#include "rm_uecontext.h"
#include "rm_uemanager.h"

#define RM_ADD_TO_UE_LIST(pUE)\
{\
   if(pUE->isCsgMember())\
   {\
      m_tlCsgActiveUEList.push_back(pUE);\
   }\
   else\
   {\
      m_tlNonCsgActiveUEList.push_back(pUE);\
   }\
   m_usTotActUe++;\
}

#define RM_DEL_ACTIVE_UE(pUE)\
{\
   if(pUE->isCsgMember())\
   {\
      m_tlCsgActiveUEList.remove(pUE);\
   }\
   else\
   {\
      m_tlNonCsgActiveUEList.remove(pUE);\
   }\
   m_usTotActUe--;\
}

#define RM_ADD_UE_TO_EXT_UE_LIST(pUE)\
{\
   m_tlExtUEList.push_back(pUE);\
   m_usTotActUe++;\
}

#define RM_DEL_FROM_EXT_UE_LIST(pUE)\
{\
   m_tlExtUEList.remove(pUE);\
   m_usTotActUe--;\
}

/* defining the static variable here */
U8 CUEManager::m_usUeEstCausePriority[RMU_MAX_EST_CAUSE];

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CUEManager::CUEManager(LrmCellConfigData & stCellCfg)
{
	m_tvUEList.resize(INITIAL_UE_POOL_SIZE);
	for(U8 bIndex=0; bIndex < INITIAL_UE_POOL_SIZE; bIndex++)
   {
		m_tlFreeUEPool.push_back(&m_tvUEList[bIndex]);
   }

	m_usMaxActiveUEs = stCellCfg.bMaxActiveUesServed;
	m_usMaxSigUEs = stCellCfg.bMaxSigUesServed;
   m_usMaxExtraUEs = stCellCfg.usMaxExtraUes;
	m_usMaxCsgUes = stCellCfg.usMaxCsgUes;
	m_usMaxNonCsgUes = stCellCfg.usMaxNonCsgUes;
   m_usTotActUe = 0;

   for(U8 bIndex=0; bIndex<(m_usMaxSigUEs + m_usMaxActiveUEs); bIndex++)
   {
      m_bUeIdx[bIndex] = TRUE;    
   }
   RLOG2(L_DEBUG," CFG MAX NUM OF CSG UEs[%d],CFG MAX NUM OF NON-CSG"
         "UEs[%d]",m_usMaxCsgUes,m_usMaxNonCsgUes);

   /* initializing the static variable here */
   /* priority for establishment causes in RmuConEstCause */
   m_usUeEstCausePriority[0] = 2; // emergency
   m_usUeEstCausePriority[1] = 1; // high priority
   m_usUeEstCausePriority[2] = 3; // mt access
   m_usUeEstCausePriority[3] = 4; // mo signaling
   m_usUeEstCausePriority[4] = 3; // mo data
   m_usUeEstCausePriority[5] = 5; // delay tolerent
   m_usUeEstCausePriority[6] = 3; // ho req
}

/* HCSG_DEV */
void CUEManager::cellRecfg(LrmCellConfigData & stCellCfg)
{
	m_usMaxCsgUes = stCellCfg.usMaxCsgUes;
	m_usMaxNonCsgUes = stCellCfg.usMaxNonCsgUes;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CUEManager::isMaxActiveUELimitReached()
{
   if(m_usTotActUe < m_usMaxActiveUEs)
   {
      return false;
   }
   else
   {
      RLOG2(L_DEBUG, "Max Active UE Limit is reached:"
            "total number of UEs:[%d],Max configured UEs:[%d]",
            m_usTotActUe,m_usMaxActiveUEs);
      return true;
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CUEManager::allowExtraUe()
{
   printStats();

   if((m_usTotActUe < (m_usMaxActiveUEs + m_usMaxExtraUEs)) &&
      ((getNumActNonCsgUEs() < m_usMaxNonCsgUes) ||
      (getNumActCsgUEs() < m_usMaxCsgUes)))
   {
      RLOG0(L_DEBUG, "Not allow extra UE");
      return false;
   }
   else
   {
      RLOG0(L_DEBUG, "Allow extra UE");
      return true;
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CUEManager::allowSigUeToActUeLst()
{
   if((getNumActNonCsgUEs() < m_usMaxNonCsgUes) ||
         (getNumActCsgUEs() < m_usMaxCsgUes))
   {
      return true;
   }
   return false;
}

void CUEManager::updateNewCRNTI(U16 usCRNTI, U16 usNewCRNTI)
{
   m_itUE = m_tmUE.find(usCRNTI);

   if( m_itUE == m_tmUE.end() )
   {
      return;
   }
   else
   {
      RLOG2(L_INFO,"Updating C-RNTI. Old[%d] New[%d]",usCRNTI,usNewCRNTI);

      m_tmUE[usNewCRNTI] = m_itUE->second;
      m_tmUE[usNewCRNTI]->setCRNTI(usNewCRNTI);
      m_tmUE[usNewCRNTI]->setRabsCRNTI(usNewCRNTI);
      m_tmUE.erase(usCRNTI);
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CUEManager::isMaxSignalingUELimitReached()
{
   if ( m_tlSigUEList.size() < m_usMaxSigUEs )
   {
      return false;
   }
   RLOG2(L_INFO,"Max Sig UE Limit Reached. "
           "Total Num of Signalling UEs:[%d],"
            "CFG Maximum signalling UEs:[%d]",
            (m_tlSigUEList.size()), m_usMaxSigUEs);

   return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CUEManager::releaseUE(CUEContext* pUE)
{
   RLOG_ARG0(L_INFO,DBG_CRNTI,pUE->getCRNTI(),"Releasing UE context.");

   if( pUE == NULL )
      return false;

   /* move the UE to free UE list */
   if(isUEInExtraUEList(pUE))
   {
      RM_DEL_FROM_EXT_UE_LIST(pUE);
   }
   else if(FALSE == pUE->isActiveUE())
   {
      /* remove the UE node from signaling UE list and 
       * insert to the m_tlFreeUEPool */
      m_tlSigUEList.remove(pUE);
   } 
   else
   {
      /* remove the UE node from Active UE List based on membership status
       * and insert into the m_tlFreeUEPool */
      RM_DEL_ACTIVE_UE(pUE);
      /*m_tlCsgActiveUEList.remove(pUE);*/
   }

   /* Erase from the UE map */
   m_tmUE.erase(pUE->getCRNTI());

   m_bUeIdx[pUE->getUeIdx()] = TRUE;
   /* reset the info of the UE */
   pUE->releaseUE();

   m_tlFreeUEPool.push_back(pUE);

   return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool findNonPreEmptableUeForEmergencyCall(CUEContext* pUE)
{
   if((pUE->getEstablishmentCause() == RMU_EST_CAUSE_EMERGENCY) ||
         (pUE->getIsEmrgBrPrst() == true))
   {
      RLOG_ARG2(L_DEBUG,DBG_CRNTI,pUE->getCRNTI(),
            "Cannot preempt UE [Est.Cause: %d EmrgcyBr: %d]",
            pUE->getEstablishmentCause(), pUE->getIsEmrgBrPrst());

      return false;
   }
   else
   {
      RLOG_ARG2(L_DEBUG,DBG_CRNTI,pUE->getCRNTI(),
            "UE selected for preemption [Est.Cause: %d EmrgcyBr: %d]",
            pUE->getEstablishmentCause(), pUE->getIsEmrgBrPrst());

      return true;
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool findNonPreEmptableUeForHighPriorityCall(CUEContext* pUE)
{
   if((pUE->getEstablishmentCause() == RMU_EST_CAUSE_EMERGENCY) ||
         (pUE->getEstablishmentCause() == RMU_EST_CAUSE_HIGH_PRIORTY_ACCESS) ||
         (pUE->getIsEmrgBrPrst() == true))
   {
      RLOG_ARG2(L_DEBUG,DBG_CRNTI,pUE->getCRNTI(),
            "Cannot preempt UE [Est.Cause: %d EmrgcyBr: %d]",
            pUE->getEstablishmentCause(), pUE->getIsEmrgBrPrst());

      return false;
   }
   else
   {
      RLOG_ARG2(L_DEBUG,DBG_CRNTI,pUE->getCRNTI(),
            "UE selected for preemption [Est.Cause: %d EmrgcyBr: %d]",
            pUE->getEstablishmentCause(), pUE->getIsEmrgBrPrst());

      return true;
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool findPreEmptableUeForEmergencyCall(CUEContext* pUE)
{
   if((pUE->isNonPreemptableUE()== true) || 
         (pUE->getEstablishmentCause() == RMU_EST_CAUSE_EMERGENCY) ||
         (pUE->getIsEmrgBrPrst() == true))
   {
      RLOG_ARG3(L_DEBUG,DBG_CRNTI,pUE->getCRNTI(),
            "Cannot preempt UE [Premptablility: %d Est.Cause: %d EmrgcyBr: %d]",
            pUE->isNonPreemptableUE(), pUE->getEstablishmentCause(),
            pUE->getIsEmrgBrPrst());

      return false;
   }
   else
   {
     RLOG_ARG3(L_DEBUG,DBG_CRNTI,pUE->getCRNTI(),
            "UE Selected for preemption [Premptablility: %d "
            "Est.Cause: %d EmrgcyBr: %d]",
            pUE->isNonPreemptableUE(), pUE->getEstablishmentCause(),
            pUE->getIsEmrgBrPrst());

      return true;
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool findPreEmptableUEForHighPriorityCall(CUEContext* pUE)
{
   if((pUE->isNonPreemptableUE()== true) ||
         (pUE->getEstablishmentCause() == RMU_EST_CAUSE_EMERGENCY) ||
         (pUE->getEstablishmentCause() == RMU_EST_CAUSE_HIGH_PRIORTY_ACCESS) ||
         (pUE->getIsEmrgBrPrst() == true))
   {
      RLOG_ARG3(L_DEBUG,DBG_CRNTI,pUE->getCRNTI(),
            "Cannot preempt UE [Premptablility: %d Est.Cause: %d EmrgcyBr: %d]",
            pUE->isNonPreemptableUE(), pUE->getEstablishmentCause(),
            pUE->getIsEmrgBrPrst());

      return false;
   } 
   else
   {
      RLOG_ARG3(L_DEBUG,DBG_CRNTI,pUE->getCRNTI(),
            "UE Selected for preemption [Premptablility: %d "
            "Est.Cause: %d EmrgcyBr: %d]",
            pUE->isNonPreemptableUE(), pUE->getEstablishmentCause(),
            pUE->getIsEmrgBrPrst());

      return true;
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool isMaxARPwithQCIPriAndNearToInactiveUe(CUEContext* pUE1, CUEContext* pUE2)
{

   if( pUE1 == NULL || pUE2 == NULL)
      return false;


   /* Priority is based on the values in - RmuConEstCause */
   /* 1. HP
    * 2. Emergency
    * 3. others */
   if(CUEManager::getUeEstCausePrio(pUE1->getEstablishmentCause()) > 
         CUEManager::getUeEstCausePrio(pUE2->getEstablishmentCause()))
   {
      return true;
   }

   /* if UE1 contains a bearer with emergency ARP and UE2 does not have
    * a bearer with emergency ARP, then UE1 is given more priority
    */
   if((true == pUE1->getIsEmrgBrPrst()) > (true == pUE2->getIsEmrgBrPrst()))
   {
      return false;
   }

   /* Highest value of ARP and highest value of Qci priority
    * is considered the candidate UE  */
   if(pUE1->getMaxArpOfUe() > pUE2->getMaxArpOfUe())
   {
      return true;
   }

   if(pUE1->getMaxArpOfUe() == pUE2->getMaxArpOfUe())
   {
      if (pUE1->getMaxArpQciPriorityOfUe() > pUE2->getMaxArpQciPriorityOfUe())
      {
         return true;
      } 
      if (pUE1->getMaxArpQciPriorityOfUe() == pUE2->getMaxArpQciPriorityOfUe())
      {
         if(pUE1->getInactivityTimerExpiryCount() > pUE2->getInactivityTimerExpiryCount())
         {
            return true;
         }
      }
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CUEContext* CUEManager::findLeastPriorityUEToRelease(U8 bConnEstablishmentCause)
{
   RLOG0(L_INFO,"Finding least priority UE to release");

   /* sort the UEs based on Max ARP priority. In a case when two UEs have 
    * the same ARP priorities then consider highest QCI priority and 
    * in a case when both the UEs have the same QCI priority also then 
    * consider the near to highest inactive timer expiry count UE 
    * as candidate UE */
   m_tlCsgActiveUEList.sort(isMaxARPwithQCIPriAndNearToInactiveUe);
   m_tlNonCsgActiveUEList.sort(isMaxARPwithQCIPriAndNearToInactiveUe);

   /* find a candidate UE from sorted list which is not an Emergency UE,
    * not a high priority UE and candidate UE should not have a single non pre-emptable bearer */

   std::list<CUEContext*>::iterator it;
   switch(bConnEstablishmentCause)
   {
      case RMU_EST_CAUSE_EMERGENCY:
         { 
            RLOG0(L_DEBUG,"Establishment cause: EMERGENCY");
            /* Here find a proper UE to get released */
            /* Check if any extra nonCSG UEs admitted, 
             * if yes pre empt extra nonCSG non GBR UE from NonCSG UE list,
             * then pre empt nonCSG GBR UE. Else check if extra CSG UEs  admitted. 
             * If yes pre empt extra CSG non GBR UE, then pre empt CSG GBR UE.
             * If there are no extra  Non CSG and CSG UEs, 
             * then preempt first nonCSG nonGBR,followed by  nonCSG GBR UEs
             * followd by CSG nonGBR and CSG GBR UE */
            if(getNumActCsgUEs() > m_usMaxCsgUes)
            {
               RLOG0(L_DEBUG,
                     "Finding CSG UE with non-GBR bearer for preemption");

               /* find a CSG UE with non-GBR bearer for preemption */
               it = find_if(m_tlCsgActiveUEList.begin(),
                     m_tlCsgActiveUEList.end(),
                     findPreEmptableUeForEmergencyCall);
               if (it == m_tlCsgActiveUEList.end())
               {
                  RLOG0(L_DEBUG,
                     "Finding CSG UE with GBR bearer for preemption");
                  /* find a CSG UE with GBR bearer for preemption */
                  it = find_if(m_tlCsgActiveUEList.begin(),
                        m_tlCsgActiveUEList.end(),
                        findNonPreEmptableUeForEmergencyCall);
                  if (it == m_tlCsgActiveUEList.end())
                  {
                     RLOG0(L_DEBUG,
                     "Finding non-CSG UE with non-GBR bearer for preemption");
                     /* find a NonCSG UE with non-GBR bearer for preemption */
                     it = find_if(m_tlNonCsgActiveUEList.begin(),
                           m_tlNonCsgActiveUEList.end(),
                           findPreEmptableUeForEmergencyCall);
                     if (it == m_tlNonCsgActiveUEList.end())
                     {
                        RLOG0(L_DEBUG, "Finding non-CSG UE with non-GBR bearer"
                              " for preemption");
                        /* find a NonCSG UE with GBR bearer for preemption */
                        it = find_if(m_tlNonCsgActiveUEList.begin(),
                              m_tlNonCsgActiveUEList.end(), 
                              findNonPreEmptableUeForEmergencyCall);
                        return (it == m_tlNonCsgActiveUEList.end()) ? NULLP : (*it);
                     }
                     else
                     {
                        RLOG_ARG0(L_DEBUG,DBG_CRNTI,(*it)->getCRNTI(),
                              "Found non-CSG premptable UE for preemption");
                        return *it;
                     }
                  }
                  else
                  {
                     RLOG_ARG0(L_DEBUG,DBG_CRNTI,(*it)->getCRNTI(),
                        "Found CSG UE with GBR bearer for preemption");
                     return *it;
                  }
               }
               else
               {
                  RLOG_ARG0(L_DEBUG,DBG_CRNTI,(*it)->getCRNTI(),
                        "Found CSG UE with non-GBR bearer for preemption");
                  return *it;
               }
            }
            else
            {
               RLOG0(L_DEBUG,
                     "Finding Non-CSG UE with non-GBR bearer for preemption");
               /* find a non-CSG UE with non-GBR bearer for preemption */
               it = find_if(m_tlNonCsgActiveUEList.begin(),
                     m_tlNonCsgActiveUEList.end(), findPreEmptableUeForEmergencyCall);
               if (it == m_tlNonCsgActiveUEList.end())
               {
                  RLOG0(L_DEBUG,
                     "Finding Non-CSG UE with GBR bearer for preemption");
                  /* find a non-CSG UE with GBR bearer for preemption */
                  it = find_if(m_tlNonCsgActiveUEList.begin(),
                        m_tlNonCsgActiveUEList.end(),
                        findNonPreEmptableUeForEmergencyCall);
                  if (it == m_tlNonCsgActiveUEList.end())
                  {
                     RLOG0(L_DEBUG,
                     "Finding CSG UE with non-GBR bearer for preemption");
                     /* find a CSG UE with non-GBR bearer for preemption */
                     it = find_if(m_tlCsgActiveUEList.begin(),
                           m_tlCsgActiveUEList.end(),
                           findPreEmptableUeForEmergencyCall);
                     if (it == m_tlCsgActiveUEList.end())
                     {
                        RLOG0(L_DEBUG,
                           "Finding CSG UE with GBR bearer for preemption");
                        /* find a CSG UE with GBR bearer for preemption */
                        it = find_if(m_tlCsgActiveUEList.begin(),
                              m_tlCsgActiveUEList.end(), 
                              findNonPreEmptableUeForEmergencyCall);
                        return (it == m_tlCsgActiveUEList.end()) ? NULLP : (*it);
                     }
                     else
                     {
                        RLOG_ARG0(L_DEBUG,DBG_CRNTI,(*it)->getCRNTI(),
                              "Found CSG pre-emptable UE for preemption");
                        return (*it);
                     }
                  }
                  else
                  {
                     RLOG_ARG0(L_DEBUG,DBG_CRNTI,(*it)->getCRNTI(),
                        "Found Non-CSG UE with GBR bearer for preemption");
                     return *it;
                  }
               }
               else
               {
                  RLOG_ARG0(L_DEBUG,DBG_CRNTI,(*it)->getCRNTI(),
                        "Found Non-CSG UE with non-GBR bearer for preemption");
                  return *it;
               }
            }
         }
         break;

      case RMU_EST_CAUSE_HIGH_PRIORTY_ACCESS:
         {
            RLOG0(L_DEBUG,"Establishment cause: HIGH_PRIORTY_ACCESS");
            /* Here find a proper UE to get released */
            /* if extra CSG UEs  admitted. If yes pre empt 
             * CSG non GBR UE . If there are no extra  Non CSG and CSG UEs, 
             * then preempt first nonCSG nonGBR,followed by  CSG Non GBR UEs */
            if(getNumActCsgUEs() > m_usMaxCsgUes)
            {
               RLOG0(L_DEBUG,
                     "Finding CSG UE with non-GBR bearer for preemption");
               it = find_if(m_tlCsgActiveUEList.begin(),
                     m_tlCsgActiveUEList.end(),
                     findPreEmptableUEForHighPriorityCall);
               if(it == m_tlCsgActiveUEList.end())
               {
                  /* find a CSG UE with GBR bearer for preemption */
                  it = find_if(m_tlCsgActiveUEList.begin(),
                        m_tlCsgActiveUEList.end(),
                        findNonPreEmptableUeForHighPriorityCall);
                  if (it == m_tlCsgActiveUEList.end())
                  {
                     RLOG0(L_DEBUG,
                        "Finding non-CSG UE with non-GBR bearer for preemption");
                     it = find_if(m_tlNonCsgActiveUEList.begin(),
                           m_tlNonCsgActiveUEList.end(), 
                           findPreEmptableUEForHighPriorityCall);
                     if(it == m_tlNonCsgActiveUEList.end())
                     {
                        it = find_if(m_tlNonCsgActiveUEList.begin(),
                              m_tlNonCsgActiveUEList.end(), 
                              findNonPreEmptableUeForHighPriorityCall);
                        return (it == m_tlNonCsgActiveUEList.end()) ? NULLP : (*it);
                     }
                     else
                     {
                        RLOG_ARG0(L_DEBUG,DBG_CRNTI,(*it)->getCRNTI(),
                              "Found non-CSG UE for pre-emption which is premptable");
                        return (*it);
                     }
                  }
                  else
                  {
                     RLOG_ARG0(L_DEBUG,DBG_CRNTI,(*it)->getCRNTI(),
                           "Found CSG UE for pre-emption which is non-premptable");
                     return (*it);
                  }
               }
               else
               {
                  RLOG_ARG0(L_DEBUG,DBG_CRNTI,(*it)->getCRNTI(),
                        "Found CSG UE for premption which is premptable ");
                  return (*it);
               }
            }
            else
            {
               RLOG0(L_DEBUG,
                     "Finding non-CSG UE with non-GBR bearer for preemption");
               it = find_if(m_tlNonCsgActiveUEList.begin(),
                     m_tlNonCsgActiveUEList.end(), 
                     findPreEmptableUEForHighPriorityCall);
               if (it == m_tlNonCsgActiveUEList.end())
               {
                  it = find_if(m_tlNonCsgActiveUEList.begin(),
                        m_tlNonCsgActiveUEList.end(), 
                        findNonPreEmptableUeForHighPriorityCall);
                  if (it == m_tlNonCsgActiveUEList.end())
                  {
                     RLOG0(L_DEBUG,
                           "Finding CSG UE with non-GBR bearer for preemption");
                     /* find a CSG UE with non-GBR bearer for preemption */
                     it = find_if(m_tlCsgActiveUEList.begin(),
                           m_tlCsgActiveUEList.end(),
                           findPreEmptableUEForHighPriorityCall);
                     if(it == m_tlCsgActiveUEList.end())
                     {
                        /* find a CSG UE with GBR bearer for preemption */
                        it = find_if(m_tlCsgActiveUEList.begin(),
                              m_tlCsgActiveUEList.end(),
                              findNonPreEmptableUeForHighPriorityCall);
                        return (it == m_tlCsgActiveUEList.end()) ? NULLP : (*it);
                     }
                     else
                     {
                        RLOG_ARG0(L_DEBUG,DBG_CRNTI,(*it)->getCRNTI(),
                              "Found CSG and pre-emptable UE for pre-emption since incoming is HP call");
                        return (*it);
                     }
                  }
                  else
                  {
                     RLOG_ARG0(L_DEBUG,DBG_CRNTI,(*it)->getCRNTI(),
                           "Found non-CSG and non-pre-emptable UE for pre-emption since incoming is HP call");
                     return (*it);
                  }
               }
               else
               {
                  RLOG_ARG0(L_DEBUG,DBG_CRNTI,(*it)->getCRNTI(),
                       "Found non-CSG and pre-emptable UE for pre-emption since incoming is HP call");
                  return (*it);
               }
            }
         }
         break;

      default:
         {
            RLOG1(L_INFO,"Reached default case for establishment cause [%d]",
                  bConnEstablishmentCause);
            it = m_tlCsgActiveUEList.end();
         }
         break;
   }

   return (NULLP);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CUEContext  *CUEManager::moveUEfromSigLstToActiveLst(CUEContext *pUE)
{
   CUEContext  *pCandidateUE = NULL;
   bool        isActiveFlagSet = TRUE;

   if (pUE->getEstablishmentCause() == RMU_EST_CAUSE_MO_SIGNALLING)
   {
      /* remove the UE node from signaling list */
      m_tlSigUEList.remove(pUE);
      /* HCSG_DEV */
      if(!pUE->isCsgMember())
      {
         if(isExtraCsgUEsAdmitted())
         {
            if(isMaxActiveUELimitReached())
            {
               /* release a CSG UE and admit non-member UE */
               pCandidateUE = findCsgUEToRelease();
               if(!pCandidateUE)
               {
                  isActiveFlagSet = FALSE;
               }
            }
         }
         else if(isMaxActiveUELimitReached())
         {
            isActiveFlagSet = FALSE;
         }
      }
      else if(pUE->isCsgMember())
      {
         if(!isMaxCsgUEsAdmitted())
         {
            if(isMaxActiveUELimitReached())
            {
               /* release a Non CSG UE and admit member UE */
               pCandidateUE = findNonCsgUEToRelease();
               if(!pCandidateUE)
               {
                  isActiveFlagSet = FALSE;
               }
            }
         }
         else if(isMaxActiveUELimitReached())
         {
            isActiveFlagSet = FALSE;
         }/*
         else
         {
            isActiveFlagSet = FALSE;
         }*/
      }
      if(isActiveFlagSet)
      {
         /* move UE to active list */
         RM_ADD_TO_UE_LIST(pUE);
         pUE->setActiveUEFlag(TRUE);
      }
      else
      {
         pCandidateUE = pUE;
      }
   }
   else
   {
      pUE->setActiveUEFlag(TRUE);
   }

   return pCandidateUE;
}
#ifdef RRM_UNUSED_FUNC
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CUEManager::moveUEfromActiveLstToSigLst(CUEContext *pUE)
{
   U32		usNumOfActiveUes = getNumActCsgUEs();

   if (pUE->isActiveUE() == true)
   {
      RLOG_ARG0(L_DEBUG,DBG_CRNTI,pUE->getCRNTI(),
            "Moving UE from Active Ue list to Signalling UE list");
      /* remove the UE node from Active UE List and insert into the 
       * m_tlSigUEList */
      RM_DEL_ACTIVE_UE(pUE);
      pUE->setActiveUEFlag(FALSE);
      m_tlSigUEList.push_back(pUE);
   }
   return true;
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool  CUEManager::isRadioResThrsldReached()
{
   if ((getNumActCsgUEs() +
            getNumActNonCsgUEs() +
            m_tlSigUEList.size() +
            m_tlExtUEList.size()) <= (m_usMaxActiveUEs + m_usMaxExtraUEs))
   {
      return false;
   }
   else
   {
      return true;
   }

}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CUEContext*  CUEManager::findUE(U16 usCRNTI)
{
   m_itUE = m_tmUE.find(usCRNTI);
   return (m_itUE == m_tmUE.end()) ?  NULL : m_itUE->second;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// @desc admitUE will always be success, before admitting make sure all conditions are met
///////////////////////////////////////////////////////////////////////////////////////////////////
CUEContext* CUEManager::admitUE(U16 usCRNTI, U8 bConnEstablishmentCause)
{
   CUEContext *pUE = NULL;

   RLOG_ARG1(L_INFO,DBG_CRNTI,usCRNTI,"Admitting UE. Establishment cause [%d]"
         ,bConnEstablishmentCause);

   pUE = findUE(usCRNTI);

   if( pUE != NULL )
   {
      if( pUE->isActiveUE() )
      {
         RM_DEL_ACTIVE_UE(pUE);
         /*m_tlCsgActiveUEList.remove(pUE);*/
      }
      else
      {
         m_tlSigUEList.remove(pUE);
      }
   }
   else
   {
      pUE = m_tlFreeUEPool.front();
      /* clok Work Error: Memory pointed by 
       'this->m_tlFreeUEPool._M_impl._M_node._M_next' is freed at this point,
       but this memory is not freed upon function exits at line(s) 284.*/
      //m_tlFreeUEPool.pop_front();
      m_tlFreeUEPool.remove(pUE);
   }

   m_tmUE[usCRNTI] = pUE;

   /* Update establishment cause and CRNTI */
   pUE->setEstablishmentCause(bConnEstablishmentCause);
   pUE->setCRNTI(usCRNTI);
   //pUE->setSetupTime();

   /* HCSG_DEV */
   /* setting membership status to TRUE by default to avoid race condition 
    * of removing this UE to admit another UE before Initial Context Setup */
   pUE->setMemberShip(TRUE);

   for(int idx = 0; idx < (m_usMaxSigUEs + m_usMaxActiveUEs); idx++)
   {
      if(m_bUeIdx[idx] == TRUE)
      {
         pUE->setUeIdx(idx);
         m_bUeIdx[idx] = FALSE;
         break;
      }   
   }

   if (RMU_EST_CAUSE_MO_SIGNALLING == bConnEstablishmentCause)
   {
      pUE->setActiveUEFlag(FALSE);
      m_tlSigUEList.push_back(pUE);
   } 
   /* HCSG_DEV start */
   else if(!isMaxActiveUELimitReached())
   {
      pUE->setActiveUEFlag(TRUE);
      /* HCSG_DEV */
      RM_ADD_TO_UE_LIST(pUE);
      /*m_tlCsgActiveUEList.push_back(pUE);*/
   }
   else
   {
      RM_ADD_UE_TO_EXT_UE_LIST(pUE);
   }
   /* HCSG_DEV end */
   return pUE;
}

/* HCSG_DEV - start */
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CUEManager::isUEInExtraUEList(CUEContext *pUE)
{
   std::list<CUEContext*>::iterator it;

   for (it = m_tlExtUEList.begin(); it != m_tlExtUEList.end(); it++)
   {
      if((*it)->getCRNTI() == pUE->getCRNTI())
      {
         return true;
      }
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CUEManager::isMaxNonCsgUEsAdmitted()
{
   if(getNumActNonCsgUEs() < m_usMaxNonCsgUes)
   {
      return false;
   }

   return true;
}

bool CUEManager::isMaxCsgUEsAdmitted()
{
   if(getNumActCsgUEs() < m_usMaxCsgUes)
   {
      return false;
   }

   return true;
}

bool CUEManager::isExtraCsgUEsAdmitted()
{
   if(getNumActCsgUEs() > m_usMaxCsgUes)
   {
      return true;
   }

   return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CUEContext* CUEManager::findCsgUEToRelease()
{
   std::list<CUEContext*>::iterator it;
   CUEContext*                 pUE;

   if(m_usMaxCsgUes == m_usMaxActiveUEs)
   {
      return NULLP;
   }
   
   /* Sort the CSG UEs based on the highest value of ARP, heighest value of 
    * QCI priority and heighest value of UE inactive timer expiry count
    */
   m_tlCsgActiveUEList.sort(isMaxARPwithQCIPriAndNearToInactiveUe);

   for(it = m_tlCsgActiveUEList.begin();
         it != m_tlCsgActiveUEList.end(); it++)
   {
      pUE = *it;
      if((pUE->isNonPreemptableUE() == false) &&
         (pUE->getEstablishmentCause() != RMU_EST_CAUSE_EMERGENCY) &&
         (pUE->getEstablishmentCause() != RMU_EST_CAUSE_HIGH_PRIORTY_ACCESS)&&
         (pUE->getIsEmrgBrPrst() != true))
      {
         break;
      }
   }

   /* NOTE: If none of the UEs satisfy the condition, the last
    * UE in the list is selected by default */
   return (it == m_tlCsgActiveUEList.end()) ? NULLP : *it;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CUEContext* CUEManager::findNonCsgUEToRelease()
{
   std::list<CUEContext*>::iterator it;
   CUEContext*                 pUE;

   /* Sort the non-CSG UEs based on the heighest value of ARP, heighest value of 
    * QCI priority and heighest value of UE inactive timer expiry count
    */
   m_tlNonCsgActiveUEList.sort(isMaxARPwithQCIPriAndNearToInactiveUe);

   for(it = m_tlNonCsgActiveUEList.begin();
         it != m_tlNonCsgActiveUEList.end(); it++)
   {
      pUE = *it;
      if((pUE->isNonPreemptableUE() == false) &&
         (pUE->getEstablishmentCause() != RMU_EST_CAUSE_EMERGENCY) &&
         (pUE->getEstablishmentCause() != RMU_EST_CAUSE_HIGH_PRIORTY_ACCESS) &&
         (pUE->getIsEmrgBrPrst() != true))
         
      {
         break;
      }
   }

   /* NOTE: If none of the UEs satisfy the condition, and end of the
    * list is reached then NULL is returned else selected UE is returned */
   return (it == m_tlNonCsgActiveUEList.end()) ? NULLP : *it;
}

void CUEManager::moveUEFrmExtLstToNonCsgLst(CUEContext *pUE)
{
   m_tlExtUEList.remove(pUE);
   m_tlNonCsgActiveUEList.push_back(pUE);
}

void CUEManager::moveUEFrmExtLstToCsgLst(CUEContext *pUE)
{
   m_tlExtUEList.remove(pUE);
   m_tlCsgActiveUEList.push_back(pUE);
}

void CUEManager::moveUEFrmNonCsgLstToCsgLst(CUEContext *pUE)
{
   m_tlNonCsgActiveUEList.remove(pUE);
   m_tlCsgActiveUEList.push_back(pUE);
}

void CUEManager::moveUEFrmCsgLstToNonCsgLst(CUEContext *pUE)
{
   m_tlCsgActiveUEList.remove(pUE);
   m_tlNonCsgActiveUEList.push_back(pUE);
}

/* HCSG_DEV - end */

void CUEManager::printStats()
{
   RLOG4(L_DEBUG, "Current CSG UEs [%d] Configured Max CSG UEs [%d] "
         "Current Non CSG UEs [%d] Configured Max Non CSG UEs [%d]",
         (getNumActCsgUEs()), m_usMaxCsgUes,
         (getNumActNonCsgUEs()), m_usMaxNonCsgUes);

   RLOG3(L_DEBUG, "Configured Max UEs [%d] Num of Active UEs [%d] Num of Extra "
         "UEs [%d]", m_usTotActUe, m_usMaxActiveUEs, m_usMaxExtraUEs);
}

/********************************************************************30**

           End of file:     qf_uemanager.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:37 2013

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
