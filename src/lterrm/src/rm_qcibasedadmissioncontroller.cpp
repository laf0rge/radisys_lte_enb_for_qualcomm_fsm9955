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

     File:     rm_qcibasedadmissioncontroller.cpp

     Sid:      qf_qcibasedadmissioncontroller.cpp@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/10 - Mon Feb 15 12:52:04 2016

     Prg:      Amit Ghosh

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=157;
#include "rm_qcibasedadmissioncontroller.h"
#include "rm_cmn.h"
#include "rm_utils.h"

CQCIBasedAdmissionController::CQCIBasedAdmissionController(
                                       LrmCellConfigData &stCellCfg) 
{
   U8    numDlUEsPerTTI, numUlUEsPerTTI;
 
   m_bDuplexMode = stCellCfg.bDuplexMode;   
   m_bNumDlNewUesPerTti = stCellCfg.bNumDlNewUesPerTti;   
   m_bNumUlNewUesPerTti = stCellCfg.bNumUlNewUesPerTti;   
   
   /* NOTE: we are observed that reserving the TTIs with a configured value is 
    * not much useful incase 1 UE/TTI configuration will give the problem. Hence
    * we are reserving the 10% of QCI bearers for the HARQ re-transmission.
    */
   //m_bNumDlReTxUesPerTti = stCellCfg.bNumDlReTxUesPerTti;   
   //m_bNumUlReTxUesPerTti = stCellCfg.bNumUlReTxUesPerTti;   
   m_bNumDlReTxUesPerTti = 0; 
   m_bNumUlReTxUesPerTti = 0; 
   m_bRbcQciTmngReq[0] = stCellCfg.bRbcQci1TmngReq;
   m_bRbcQciTmngReq[1] = stCellCfg.bRbcQci2TmngReq;
   m_bRbcQciTmngReq[2] = stCellCfg.bRbcQci3TmngReq;
   m_bRbcQciTmngReq[3] = stCellCfg.bRbcQci4TmngReq;
   m_enConfigMode = stCellCfg.enUlDlConfig; 
   m_bRbcQci4TmngReqEnable = false;

   if((m_bDuplexMode == RM_MODE_TDD) && 
         (m_enConfigMode >= LRM_UL_DL_CONFIG_MAX))
   {
      RLOG1(L_ERROR,"Config mode [%d] is not in range",m_enConfigMode);
      return;
   }
  
   /* Updating the MaxNumber of QCI-X beares allowed 
    * as per timing requiremetnt
    */ 
   
   /* actual NumOfUEsPerTTI is differncec between the configured 
    * numberOfUEsPerTTI - reserved numOfUEsPerTTI for retransmission 
    */
   numDlUEsPerTTI = (m_bNumDlNewUesPerTti - m_bNumDlReTxUesPerTti);

   if(!numDlUEsPerTTI)
   {
      numDlUEsPerTTI = m_bNumDlNewUesPerTti;
   }

   numUlUEsPerTTI = (m_bNumUlNewUesPerTti - m_bNumUlReTxUesPerTti);

   if(!numUlUEsPerTTI)
   {
      numUlUEsPerTTI = m_bNumUlNewUesPerTti;
   }

   if(RM_MODE_TDD == m_bDuplexMode)
   {
      U32 totalNumDlSfs, totalNumUlSfs;

      for(U32 idx = 0; idx < RM_MAX_NUM_GBR; idx++)
      {
         /* Update the max QCI-1 allowed bearers */
         getTotalNumberOfDlUlSubframes(m_enConfigMode,
                                       m_bRbcQciTmngReq[idx],
                                       &totalNumDlSfs,
                                       &totalNumUlSfs);
         
         /* max no of QCI x bearers allowed is minimum of DL and UL subframes
          * which we can schedule for the given QCI x timing requiremnt 
          */
         m_MaxNumOfQciBearersAllowed[idx] = 
            RM_MIN((numDlUEsPerTTI * totalNumDlSfs), 
                   (numUlUEsPerTTI * totalNumUlSfs));
       
         /* Reserving 10% of QCI bearers b/wor PRBs  for HARQ re-transmission */
         m_MaxNumOfQciBearersAllowed[idx] = (m_MaxNumOfQciBearersAllowed[idx] - 
                                    RM_ROUND((m_MaxNumOfQciBearersAllowed[idx]*10), 100));

      } /* end of for to traverse all GBRs */
   }
   else if (RM_MODE_FDD == m_bDuplexMode)
   {
      for(U32 idx = 0; idx < RM_MAX_NUM_GBR; idx++)
      {
         m_MaxNumOfQciBearersAllowed[idx] = 
            m_bRbcQciTmngReq[idx] * numDlUEsPerTTI;
         
         /* Reserving 10% of QCI bearers b/wor PRBs  for HARQ re-transmission */
         m_MaxNumOfQciBearersAllowed[idx] = (m_MaxNumOfQciBearersAllowed[idx] - 
                                    RM_ROUND((m_MaxNumOfQciBearersAllowed[idx]*10), 100));
      }
   }

   m_MaxNumOfQciBearersAllowed[1] = stCellCfg.bMaxActiveUesServed + 5;
   m_MaxNumOfQciBearersAllowed[2] = stCellCfg.bMaxActiveUesServed + 5;
   m_MaxNumOfQciBearersAllowed[3] = stCellCfg.bMaxActiveUesServed + 5;


   /* initializing number of current admitted bearers */
   cmMemset((U8*)&m_numOfQciBearersAdmitted, 0,
         sizeof(m_numOfQciBearersAdmitted));

   RLOG1(L_INFO,"MAX QCI-1 bearers:%d",m_MaxNumOfQciBearersAllowed[0]);
}

bool CQCIBasedAdmissionController::canRBsBeAdmitted(
                                                RmuErabConfigIe & stERAB, 
                                                RrmRbcRspInfo & erabRbcRspInfo)
{
   bool                           bRetVal = false;
   U32                            bQci;
   U8                             numErabsAdmitted = 0;
   U8                             numErabsRejected = 0;
   U16                            numOfQciBearersAdmitted[RM_MAX_NUM_GBR];
   RrmErabAdmitted                *pAcceptLst = erabRbcRspInfo.eRabAcceptedLst;
   RrmErabRejected                *pRejectLst = erabRbcRspInfo.eRabRejectedLst;

   RLOG0(L_INFO,"Checking admission of ERAB during ERAB setup");

   //Taking a back-up of m_numOfQciBearersAdmitted for restoration
   memcpy(numOfQciBearersAdmitted, m_numOfQciBearersAdmitted, RM_MAX_NUM_GBR * 
          sizeof(U16));

   for (U32 bRabIndex = 0; bRabIndex < stERAB.usNoErabsToAddMod; bRabIndex++)
   {
      RmuUErabAddModIe &stERabAddMod = stERAB.stErabAddModLst[bRabIndex];

      bQci = stERabAddMod.stErabQosInfo.bErabQCI;

      if(!isValidQci(bQci))
      {
         RLOG2(L_INFO,"Invalid QCI [%d] ERAB[%d] for admission",
               bQci,stERabAddMod.bErabId);

         /* add bearer to rejected list */
         pRejectLst[numErabsRejected].bERabId = stERabAddMod.bErabId;
         pRejectLst[numErabsRejected].bArpVal = 
            stERabAddMod.stErabQosInfo.stErabArpInfo.bErabPrioLvl;
         pRejectLst[numErabsRejected].bCause = 
            RMU_CAUSE_INVALID_QOS_COMB;
         numErabsRejected++;
         continue;
      }

      /* All non-GBR bearers are admitted always */
      if(isGbrBearer(bQci))
      {
         switch(bQci)
         {
            case RM_QCI_ONE:
            {
               bRetVal = canQCI1RBBeAdmitted(stERabAddMod.stErabQosInfo);
               break;
            }
            case RM_QCI_TWO:
            {
               bRetVal = canQCI2RBBeAdmitted(stERabAddMod.stErabQosInfo);
               break;
            }
            case RM_QCI_THREE:
            {
               bRetVal = canQCI3RBBeAdmitted(stERabAddMod.stErabQosInfo);
               break;
            }
            case RM_QCI_FOUR:
            {
               bRetVal = canQCI4RBBeAdmitted(stERabAddMod.stErabQosInfo);
               break;
            }
         }

         if(bRetVal)
         {
            /* add bearer to admitted list */
            pAcceptLst[numErabsAdmitted++].bERabId = stERabAddMod.bErabId;
            erabRbcRspInfo.isGBRBearerExists = true;
         }
         else
         {
            /* add bearer to rejected list */
            pRejectLst[numErabsRejected].bERabId = stERabAddMod.bErabId;
            pRejectLst[numErabsRejected].bArpVal = 
               stERabAddMod.stErabQosInfo.stErabArpInfo.bErabPrioLvl;
            pRejectLst[numErabsRejected].bCause = 
               RMU_CAUSE_MAX_QCI_REACHED;
            pRejectLst[numErabsRejected].uiQci = (U32) bQci; 
            /* Update ARP value */
            pRejectLst[numErabsRejected].bArpVal = 
               stERabAddMod.stErabQosInfo.stErabArpInfo.bErabPrioLvl;
            pRejectLst[numErabsRejected].bIsPreemptable = 
               stERabAddMod.stErabQosInfo.stErabArpInfo.bErabPreemptVul;
            pRejectLst[numErabsRejected].bErabPreemptCap = 
               stERabAddMod.stErabQosInfo.stErabArpInfo.bErabPreemptCap;
            numErabsRejected++;
            continue;
         }
      }
      else
      {
         /* add bearer to admitted list */
         pAcceptLst[numErabsAdmitted++].bERabId = stERabAddMod.bErabId;
         RLOG1(L_INFO, "Allowing Non-GBR ERAB [%d]",stERabAddMod.bErabId);
      }
   } /* end for to traverse the Rab setup list */
   
   //Restoring m_numOfQciBearersAdmitted
   memcpy(m_numOfQciBearersAdmitted, numOfQciBearersAdmitted, RM_MAX_NUM_GBR * 
          sizeof(U16));

   /* update number of admitted and rejected bearers to reconfig response */
   erabRbcRspInfo.nErabsAccepted = numErabsAdmitted;
   erabRbcRspInfo.nErabsRejected = numErabsRejected;

   RETVALUE(numErabsAdmitted != 0);
}

bool CQCIBasedAdmissionController::canQCI1RBBeAdmitted(
                                              RmuUErabQosInfoIe &stRabQos)
{
   /* index is QCI num - 1 */
   if(m_MaxNumOfQciBearersAllowed[RM_QCI_ONE-1] > 
         m_numOfQciBearersAdmitted[RM_QCI_ONE-1])
   {
      RLOG1(L_DEBUG,"QCI %d bearer is admitted", stRabQos.bErabQCI);
      m_numOfQciBearersAdmitted[RM_QCI_ONE-1]++;
      RETVALUE(true);
   }
   RLOG1(L_DEBUG,"QCI %d bearer is rejected", stRabQos.bErabQCI);
   RLOG2(L_DEBUG,"Since NoOf[%ld], maxNoOf[%ld] QCI-1 bearers", 
         m_numOfQciBearersAdmitted[RM_QCI_ONE-1],
         m_MaxNumOfQciBearersAllowed[RM_QCI_ONE-1]);
   RETVALUE(false);
}

bool CQCIBasedAdmissionController::canQCI2RBBeAdmitted(
                                              RmuUErabQosInfoIe &stRabQos)
{
   /* index is QCI num - 2 */
   if(m_MaxNumOfQciBearersAllowed[RM_QCI_TWO-1] > 
         m_numOfQciBearersAdmitted[RM_QCI_TWO-1])
   {
      RLOG1(L_DEBUG,"QCI %d bearer is admitted", stRabQos.bErabQCI);
      m_numOfQciBearersAdmitted[RM_QCI_TWO-1]++;
      RETVALUE(true);
   }
   RLOG1(L_DEBUG,"QCI %d bearer is rejected", stRabQos.bErabQCI);
   RLOG2(L_DEBUG,"Since NoOf[%ld], maxNoOf[%ld] QCI-2 bearers", 
         m_numOfQciBearersAdmitted[RM_QCI_TWO-1],
         m_MaxNumOfQciBearersAllowed[RM_QCI_TWO-1]);
   RETVALUE(false);
}

bool CQCIBasedAdmissionController::canQCI3RBBeAdmitted(
                                              RmuUErabQosInfoIe &stRabQos)
{
   /* index is QCI num - 3 */
   if(m_MaxNumOfQciBearersAllowed[RM_QCI_THREE-1] > 
         m_numOfQciBearersAdmitted[RM_QCI_THREE-1])
   {
      RLOG1(L_DEBUG,"QCI %d bearer is admitted", stRabQos.bErabQCI);
      m_numOfQciBearersAdmitted[RM_QCI_THREE-1]++;
      RETVALUE(true);
   }
   RLOG1(L_DEBUG,"QCI %d bearer is rejected", stRabQos.bErabQCI);
   RLOG2(L_DEBUG,"Since NoOf[%ld], maxNoOf[%ld] QCI-3 bearers", 
         m_numOfQciBearersAdmitted[RM_QCI_THREE-1],
         m_MaxNumOfQciBearersAllowed[RM_QCI_THREE-1]);
   RETVALUE(false);
}

bool CQCIBasedAdmissionController::canQCI4RBBeAdmitted(
                                              RmuUErabQosInfoIe &stRabQos)
{
   /* index is QCI num - 4 */
   if(m_MaxNumOfQciBearersAllowed[RM_QCI_FOUR-1] > 
         m_numOfQciBearersAdmitted[RM_QCI_FOUR-1])
   {
      RLOG1(L_DEBUG,"QCI %d bearer is admitted", stRabQos.bErabQCI);
      m_numOfQciBearersAdmitted[RM_QCI_FOUR-1]++;
      RETVALUE(true);
   }
   RLOG1(L_DEBUG,"QCI %d bearer is rejected", stRabQos.bErabQCI);
   RLOG2(L_DEBUG,"Since NoOf[%ld], maxNoOf[%ld] QCI-4 bearers", 
         m_numOfQciBearersAdmitted[RM_QCI_FOUR-1],
         m_MaxNumOfQciBearersAllowed[RM_QCI_FOUR-1]);
   RETVALUE(false);
}

void CQCIBasedAdmissionController::rbReleaseInd(U32 uiQci)
{
   /* decrement number of bearers admitted for the particular QCI */
   m_numOfQciBearersAdmitted[uiQci-1]--;
}

void CQCIBasedAdmissionController::rbAdditionInd(U32 qci)
{
   m_numOfQciBearersAdmitted[qci-1]++;
}
bool CQCIBasedAdmissionController::canModifyRBsBeAdmitted(CUEContext *pUE,
                                                    RmuErabConfigIe & stERAB, 
                                                 RrmRbcRspInfo & erabRbcRspInfo)
{
   bool   bRetVal = false;
   U8    numErabsAdmitted = 0;
   U8    numErabsRejected = 0;
   U16   numOfQciBearersAdmitted[RM_MAX_NUM_GBR];
   RrmErabAdmitted  *pAcceptLst = erabRbcRspInfo.eRabAcceptedLst;
   RrmErabRejected  *pRejectLst = erabRbcRspInfo.eRabRejectedLst;

   RLOG0(L_INFO,"Checking admission of ERAB during ERAB modification");

   //Taking a back-up of m_numOfQciBearersAdmitted for restoration
   memcpy(numOfQciBearersAdmitted, m_numOfQciBearersAdmitted, RM_MAX_NUM_GBR * 
          sizeof(U16));

   for (U32 bRabIndex = 0; bRabIndex < stERAB.usNoErabsToAddMod; bRabIndex++)
   {
      RmuUErabAddModIe &stERabAddMod = stERAB.stErabAddModLst[bRabIndex];
      U8 bQci = stERabAddMod.stErabQosInfo.bErabQCI;
      U8 bErabId = stERabAddMod.bErabId;
      CERABInfo* pObjOldERab = pUE->findERAB(bErabId);
     
      if(NULL == pObjOldERab)
      {
         RLOG_ARG1(L_INFO,DBG_CRNTI,pUE->getCRNTI(),
               "Invalid ERAB [%d] for admission", bErabId);

         /* add bearer to rejected list */
         pRejectLst[numErabsRejected].bERabId = bErabId;
         pRejectLst[numErabsRejected].bArpVal = 
            stERabAddMod.stErabQosInfo.stErabArpInfo.bErabPrioLvl;
         pRejectLst[numErabsRejected].bCause = RMU_CAUSE_UNKNOWN_ERAB_ID;
         numErabsRejected++;
         continue;
      }
      
     
      if(!isValidQci(bQci))
      {
         RLOG_ARG1(L_INFO,DBG_CRNTI,pUE->getCRNTI(),
            "Invalid QCI for admission [%d]", bQci);

         /* add bearer to rejected list */
         pRejectLst[numErabsRejected].bERabId = stERabAddMod.bErabId;
         pRejectLst[numErabsRejected].bArpVal = 
            stERabAddMod.stErabQosInfo.stErabArpInfo.bErabPrioLvl;
         pRejectLst[numErabsRejected].bCause = 
            RMU_CAUSE_INVALID_QOS_COMB;
         numErabsRejected++;
         continue;
      }
     
      /* No change in the QCI */
      if(bQci == pObjOldERab->getQci())
      {
         /* add bearer to accept list for modication */
         pAcceptLst[numErabsAdmitted++].bERabId = stERabAddMod.bErabId;
         continue;
      }

      /* GBR to non-GBR or non-GBR to GBR modification is not allowed */
      if((isGbrBearer(pObjOldERab->getQci()) and (!isGbrBearer(bQci))) ||
         ((!isGbrBearer(pObjOldERab->getQci())) and (isGbrBearer(bQci))))
      {
         RLOG3(L_DEBUG,"Non-GBR to GBR or vice versa is not allowed for ERAB"
               "moficiation ERAB ID [%d], oldQci:%d, newQci:%d", 
               bErabId, pObjOldERab->getQci(), bQci);

         pRejectLst[numErabsRejected].bERabId = bErabId;
         pRejectLst[numErabsRejected].bArpVal = 
            stERabAddMod.stErabQosInfo.stErabArpInfo.bErabPrioLvl;
         pRejectLst[numErabsRejected].bCause = 
            RMU_CAUSE_INVALID_QOS_COMB;
         numErabsRejected++;
         continue;
      }
      
      /* All non-GBR bearers modification is accepted always */
      if(isGbrBearer(bQci))
      {
         switch(bQci)
         {
            case RM_QCI_ONE:
            {
               bRetVal = canQCI1RBBeAdmitted(stERabAddMod.stErabQosInfo);
               break;
            }
            case RM_QCI_TWO:
            {
               bRetVal = canQCI2RBBeAdmitted(stERabAddMod.stErabQosInfo);
               break;
            }
            case RM_QCI_THREE:
            {
               bRetVal = canQCI3RBBeAdmitted(stERabAddMod.stErabQosInfo);
               break;
            }
            case RM_QCI_FOUR:
            {
               bRetVal = canQCI4RBBeAdmitted(stERabAddMod.stErabQosInfo);
               break;
            }
         }

         if(bRetVal)
         {
            /* decrease the previous QCI count */
            rbReleaseInd(pObjOldERab->getQci());

            /* add bearer to admitted list */
            pAcceptLst[numErabsAdmitted++].bERabId = stERabAddMod.bErabId;
            erabRbcRspInfo.isGBRBearerExists = true;
         }
         else
         {
            /* add bearer to rejected list */
            pRejectLst[numErabsRejected].bERabId = stERabAddMod.bErabId;
            pRejectLst[numErabsRejected].bArpVal = 
               stERabAddMod.stErabQosInfo.stErabArpInfo.bErabPrioLvl;
            pRejectLst[numErabsRejected].bCause = 
               RMU_CAUSE_MAX_QCI_REACHED;
            pRejectLst[numErabsRejected].uiQci = (U32) bQci; 
            /* Update ARP value */
            pRejectLst[numErabsRejected].bArpVal = 
               stERabAddMod.stErabQosInfo.stErabArpInfo.bErabPrioLvl;
            pRejectLst[numErabsRejected].bIsPreemptable = 
               stERabAddMod.stErabQosInfo.stErabArpInfo.bErabPreemptVul;
            pRejectLst[numErabsRejected].bErabPreemptCap = 
               stERabAddMod.stErabQosInfo.stErabArpInfo.bErabPreemptCap;
            numErabsRejected++;
            continue;
         }
      }
      else
      {
         /* add bearer to admitted list */
         pAcceptLst[numErabsAdmitted++].bERabId = stERabAddMod.bErabId;
         RLOG_ARG1(L_INFO,DBG_CRNTI,pUE->getCRNTI(),
               "Allowing Non-GBR ERAB [%d]",stERabAddMod.bErabId);
      }
   } /* end for to traverse the Rab setup list */

   //Restoring m_numOfQciBearersAdmitted
   memcpy(m_numOfQciBearersAdmitted, numOfQciBearersAdmitted, RM_MAX_NUM_GBR * 
          sizeof(U16));

   /* update number of admitted and rejected bearers to reconfig response */
   erabRbcRspInfo.nErabsAccepted = numErabsAdmitted;
   erabRbcRspInfo.nErabsRejected = numErabsRejected;

   RETVALUE(numErabsAdmitted != 0);
}

/********************************************************************30**

           End of file:     qf_qcibasedadmissioncontroller.cpp@@/main/tenb_5.0_RIB/tenb_5.0_RIB_GA/10 - Mon Feb 15 12:52:04 2016

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


