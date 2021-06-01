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
               

     File:     rm_radiobearercontroller.cpp

     Sid:      qf_radiobearercontroller.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:35 2013

     Prg:     hkrishna 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=158;
/* header include files */
#include "rm_radiobearercontroller.h"
#include "rm_utils.h"
#include "rm_uemanager.h"
#include "rm_outinterface.h"
#include "rmu.h"

void updateCsgMemberShip(CUEContext* pUE, CRBearerController &ObjRbc, bool bIsCSGMember)
{
   std::list<CERABInfo*>::iterator   it;

	for(it = pUE->m_tlERABList.begin(); it != pUE->m_tlERABList.end() ; ++it)
   {
      CERABInfo  *ptrObjErab = (*it);
      if(isGbrBearer(ptrObjErab->getQci()))
      {
         ObjRbc.m_objPreEmptableRabsFinder.erabModificationInd(pUE, 
                                                               ptrObjErab,
                                                               bIsCSGMember);
      }
   }
}

CRBearerController::CRBearerController(LrmCellConfigData & stCellCfg, 
                                     CCellManager *pstObjCellMngr):
                                     m_objRbAdmissionController(stCellCfg, 
                                                                pstObjCellMngr),
                                     m_objPreEmptableRabsFinder(stCellCfg)
{
   m_usBroadBandUlBwUsage = 0;
   m_usBroadBandDlBwUsage = 0;
   m_bArpForEmergencyBearer = stCellCfg.bArpForEmergencyBearer;

   m_usMaxUlBroadbandBw = stCellCfg.usMaxUlBroadbandBw;
   m_usMaxDlBroadbandBw = stCellCfg.usMaxDlBroadbandBw;
   m_bCellId = stCellCfg.bCellId;

   m_tvRabList.resize((INITIAL_UE_POOL_SIZE * RMU_MAX_NUM_ERABS));
   for(U32 uiIndex=0; uiIndex < (INITIAL_UE_POOL_SIZE * RMU_MAX_NUM_ERABS); uiIndex++)
   {
      m_tlFreeRabPool.push_back(&m_tvRabList[uiIndex]);
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#if RM_ERAB_BHBW_SUPPORT /* Consider Next Release */
inline bool CRBearerController::validateUlBwUsage(S32 usUlGbr)
{
	if((m_usBroadBandUlBwUsage + usUlGbr) <= m_usMaxUlBroadbandBw)
	{
		return true;
	}
	else
	{
	   return false;
   }
}
inline bool CRBearerController::validateDlBwUsage(S32 usDlGbr)
{
	if(m_usBroadBandDlBwUsage + usDlGbr <= m_usMaxDlBroadbandBw)
	{
	  return true;
	}
	return false;
}
inline void CRBearerController::updateDlBwUsage(S32 usDlGbr)
{
	m_usBroadBandDlBwUsage += usDlGbr ;
}
inline void CRBearerController::updateUlBwUsage(S32 usUlGbr)
{
	m_usBroadBandUlBwUsage += usUlGbr ;
}
#endif /* end of RM_ERAB_BHBW_SUPPORT */

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#if RM_ERAB_BHBW_SUPPORT /* Considering Next Release */
bool CRBearerController::validateAndUpdateBackHaulBandWidth(
                                   CERABInfo *pObjExistedERab, 
                                   RmuUErabAddModIe &stERabAddMod)
{
	U32 			uiExistedUlGbr, uiExistedDlGbr;
	S32				sDeltaUlGbr = 0;
	S32				sDeltaDlGbr = 0;
	bool 			bIsGbrBearerOld = false, bIsGbrBearerNew = false;

	bIsGbrBearerOld = isGbrBearer(pObjExistedERab->getQci());
	bIsGbrBearerNew = isGbrBearer(stERabAddMod.stErabQosInfo.bErabQCI);
	if (bIsGbrBearerOld)
	{
		uiExistedUlGbr = pObjExistedERab->getUlGBR();
		uiExistedDlGbr = pObjExistedERab->getDlGBR();
	} else {
		uiExistedUlGbr = 0;
		uiExistedDlGbr = 0;
	}

	sDeltaUlGbr = ((stERabAddMod.stErabQosInfo.stErabGbrInfo.uiErabUlGbr)- uiExistedUlGbr );
	sDeltaDlGbr = ((stERabAddMod.stErabQosInfo.stErabGbrInfo.uiErabDlGbr)- uiExistedDlGbr );

	/* convert the Bits per sec to Mb/sec */
	S32 sDeltaUlGbrInMbps;// = convertToMbpswithRoundOff(sDeltaUlGbr);
	S32 sDeltaDlGbrInMbps;// =  convertToMbpswithRoundOff(sDeltaDlGbr);

	/* old:non-GBR, new:non-GBR bearers , admit the new non-GBR bearer always */
	if ((!bIsGbrBearerOld) && (!bIsGbrBearerNew) )
	{
		return true;
	} else if (bIsGbrBearerOld && (!bIsGbrBearerNew)) { /* Old:GBR, new: non-GBR ,
														   then allow the new non-GBR bearer */
		/* add the negative values sDeltaUlGbrInMbps , sDeltaDlGbrInMbps causes decresing the BW usage */
		updateUlBwUsage(sDeltaUlGbrInMbps);
		updateDlBwUsage(sDeltaDlGbrInMbps);
		return true;
	} else if ((!bIsGbrBearerOld) && bIsGbrBearerNew) { /* old:non-GBR, new:GBR bearer type */
		if((validateUlBwUsage(sDeltaUlGbrInMbps)) &&
		   (validateDlBwUsage(sDeltaDlGbrInMbps)))
		{
			/* add the sDeltaUlGbrInMbps, sDeltaDlGbrInMbps , similar to ERAB setup */
			updateUlBwUsage(sDeltaUlGbrInMbps);
			updateDlBwUsage(sDeltaDlGbrInMbps);
			return true;
		}
	} else { /* old:GBR, new: GBR bearer type */
		if((validateUlBwUsage(sDeltaUlGbrInMbps)) &&
		   (validateDlBwUsage(sDeltaDlGbrInMbps)))
		{
			/* add the sDeltaUlGbrInMbps, sDeltaDlGbrInMbps , similar to ERAB setup */
			updateUlBwUsage(sDeltaUlGbrInMbps);
			updateDlBwUsage(sDeltaDlGbrInMbps);
			return true;
		}
	}
	return false;
}
#endif /* end of RM_ERAB_BHBW_SUPPORT */

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CERABInfo* CRBearerController::addErabToUe(CUEContext *pUE, 
                                     RmuUErabAddModIe &stERabAddMod)
{
   CERABInfo *ptrObjERab = m_tlFreeRabPool.front();
   m_tlFreeRabPool.remove(ptrObjERab);
   
   /* Update the RAB params and CRNTI of UE into ERAB info */
   ptrObjERab->updateERABInfo(stERabAddMod, pUE->getCRNTI());

   /* Set SPS parameters (flag for SPS enabling + erab ID), 
    * if QCI 1 bearer is admitted */
   if(RM_QCI_ONE == stERabAddMod.stErabQosInfo.bErabQCI)
   {
      pUE->setSpsParam(stERabAddMod.bErabId);
   }

   ptrObjERab->setIsPrempted(false);                                          
   ptrObjERab->setPremptedCause(RM_NOT_PREEMPTED);

   /* update is emergency bearer present */
   if(m_bArpForEmergencyBearer == ptrObjERab->getArp())
   {
      pUE->setIsEmrgBrPrst(true);
   }

   /* Add the bearer to the UE */
   pUE->addERAB(ptrObjERab);
   return ptrObjERab;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRBearerController::releaseAllERABs(CUEContext* pUE)
{
   CERABInfo *ptrObjERab;

   while((ptrObjERab = pUE->removeFirstERabFrmUeRabList()))
   {
      
      /* update removal of the ERAB to RB admision object for to decrement the
       * number of QCI-x bearers  */
      if(isGbrBearer(ptrObjERab->getQci()))
      {
         if(m_bArpForEmergencyBearer != ptrObjERab->getArp())
         {
            /* remove the E-RAB from Pre-emptable ERAB Map table */
            m_objPreEmptableRabsFinder.deleteERABInd(ptrObjERab, pUE);
         }
       
         m_objRbAdmissionController.rbReleaseInd((U32) ptrObjERab->getQci());
      }

      /* Add the ERAB to free RAB list */
      m_tlFreeRabPool.push_back(ptrObjERab);
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void rrmSendPreEmptedUesERabRelInd(U8 bCellId, 
                        CERABInfoLst &objErabsRelListPerUe)
{
   std::list<CERABInfo*>::iterator  it = objErabsRelListPerUe.begin();
   RmuUeERabRelInd*  pstUeERabRelInd;
   RmuS1apCause      enRelCause;

   pstUeERabRelInd = (RmuUeERabRelInd*)rrmAlloc(sizeof(RmuCommonMsg));

   if(NULLP == pstUeERabRelInd)
   {
      return;
   }
   
   pstUeERabRelInd->bCellId   = bCellId;
   pstUeERabRelInd->usCrnti = (*it)->getRabCrnti();
   pstUeERabRelInd->stErabRelInfo.bNoErabsToRel = objErabsRelListPerUe.size();

   for(U32 uiIndex=0; it != (objErabsRelListPerUe.end()) && (uiIndex <
         RMU_MAX_NUM_ERABS) ; ++it, ++uiIndex)
   {
      pstUeERabRelInd->stErabRelInfo.stErabRelLst[uiIndex].bErabId = (*it)->getERABId();
      if(RM_PREEMPTION_CAUSE_PRB_CONG == (*it)->getPremptedCause())
      {
         enRelCause = RMU_CAUSE_REDUCE_LOAD_IN_SERV_CELL;
      }
      else
      {
         enRelCause = RMU_CAUSE_RADIO_RESOURCE_NOT_AVAIL;
      }
      pstUeERabRelInd->stErabRelInfo.stErabRelLst[uiIndex].enErabRelCause = enRelCause;
   }

   /* Send the UE ERAB Release Indication message to the
    * FSM */
   rrmSendUeERabRelInd(pstUeERabRelInd);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CRBearerController::triggerERabRelIndPerUe(CERABInfoLst &objPreEmptedERABsList)

{
   CERABInfoLst    objErabsRelListPerUe;
   
   while(!objPreEmptedERABsList.empty())
   {
      std::list<CERABInfo*>::iterator it = objPreEmptedERABsList.begin();
      U16 usCrnti = (*it)->getRabCrnti();

      /* clear the prempted ERAB per UE list */
      objErabsRelListPerUe.clear();

      for (; it != objPreEmptedERABsList.end(); )
      {   
         if(usCrnti == (*it)->getRabCrnti())
         {
            objErabsRelListPerUe.push_back((*it));
            it = objPreEmptedERABsList.erase(it);
         }
         else
         {
            it++;
         }
      }
      rrmSendPreEmptedUesERabRelInd(m_bCellId, 
                          objErabsRelListPerUe); 
   }
}

bool CRBearerController::processErabSetupRequest(CUEContext *pUE, 
                                                 RmuErabConfigIe & stERAB,
                                                 RmuUeRecfgRsp *pstRabRspInfo)
{
   bool  bRetVal;

   if((stERAB.usNoErabsToAddMod + pUE->getNumRabsPresentInUE()) > 
         RMU_MAX_NUM_ERABS)
   {
      pstRabRspInfo->stStatusInfo.enStatus = RMU_FAILURE;
      pstRabRspInfo->stStatusInfo.enCause  = RMU_CAUSE_OTHER;
      RETVALUE(false);
   }

    processIncomingERabsforSetup(pUE,
                                 stERAB, 
                                 pstRabRspInfo->stErabAcceptLst,
                                 pstRabRspInfo->stErabRejectLst,
                                 &pstRabRspInfo->bNoErabsAccepted,
                                 &pstRabRspInfo->bNoErabsRejected);
 
   if(pstRabRspInfo->bNoErabsAccepted)
   {
      pstRabRspInfo->stStatusInfo.enStatus = RMU_SUCCESS;
      pstRabRspInfo->stStatusInfo.enCause  = RMU_CAUSE_NOT_APP;
      bRetVal = true;
   }
   else
   {
      pstRabRspInfo->stStatusInfo.enStatus = RMU_FAILURE;
      pstRabRspInfo->stStatusInfo.enCause  = RMU_CAUSE_OUT_OF_RESOURCE;
      bRetVal = false;
   }

   RETVALUE(bRetVal);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRBearerController::processErabModifyRequest(CUEContext *pUE, 
                                                  RmuErabConfigIe & stERAB, 
                                                  RmuUeRecfgRsp *pstRabRspInfo)
{
	CERABInfo 		*pobjERab = NULL;
   Bool           isSpsErabMdfy = false;
   Bool           isQciOneBrrExist = false;
   U8             qciOneErabId = 0; 
   RrmRbcRspInfo  stErabRbaRspInfo; 
   bool           bIsRabsPreEmpted;
   bool           bRetVal;

   memset(&stErabRbaRspInfo, 0, sizeof(RrmRbcRspInfo));

   if((stERAB.usNoErabsToAddMod + pUE->getNumRabsPresentInUE()) > 
         RMU_MAX_NUM_ERABS)
   {
      pstRabRspInfo->stStatusInfo.enStatus = RMU_FAILURE;
      pstRabRspInfo->stStatusInfo.enCause  = RMU_CAUSE_OTHER;
      return false;
   }
   
   /* Update the CRNTI of the UE */
   stErabRbaRspInfo.usCRNTI = pUE->getCRNTI();

   /* check the requested beareres with Radio Bearere Admison (QCI, PRB based)*/
   m_objRbAdmissionController.canModifyRBsBeAdmitted(pUE,
                                                     stERAB, 
                                                     stErabRbaRspInfo);

   /* Can find a pre-emptable bearer then trigger the pre-emption of the ERAB
    * if the incoming ERAB setup req contains at least one erab rejected 
    */
   if(stErabRbaRspInfo.nErabsRejected)
   {
      CERABInfoLst    objPreEmptedERABsList;

      bIsRabsPreEmpted = m_objPreEmptableRabsFinder.findPreEmptableERABs(
                                                      pUE->isCsgMember(),
                                                      stErabRbaRspInfo,
                                                      objPreEmptedERABsList);
      /* trigger the pre-emption of ERABs */
      if(bIsRabsPreEmpted)
      {
         triggerERabRelIndPerUe(objPreEmptedERABsList);
      }
   }

   /* Fill E-Rabs accepted list */
   for (U32 bRabIndex = 0; bRabIndex < stErabRbaRspInfo.nErabsAccepted; bRabIndex++)
   {
      pstRabRspInfo->stErabAcceptLst[bRabIndex].bErabId = 
         stErabRbaRspInfo.eRabAcceptedLst[bRabIndex].bERabId;

		pobjERab = pUE->findERAB(stErabRbaRspInfo.eRabAcceptedLst[bRabIndex].bERabId);
      if(NULL == pobjERab)
      {
         RLOG0(L_FATAL," For accepted RB ID we should have always bearer");
         continue;
      }

      for (U32 bErabIndex =0; bErabIndex < stERAB.usNoErabsToAddMod; bErabIndex++)
      {
         if( stErabRbaRspInfo.eRabAcceptedLst[bRabIndex].bERabId ==
              stERAB.stErabAddModLst[bErabIndex].bErabId )
         {
            RmuUErabAddModIe &stERabAddMod = stERAB.stErabAddModLst[bErabIndex];

            /* update the modified GBR ERab to the Premptable MultiMap table */
            if(isGbrBearer(pobjERab->getQci()))
            {
               /* check the requested beareres with Radio Bearere Admison (QCI, PRB based)*/
               m_objPreEmptableRabsFinder.erabModificationInd(&stERabAddMod, pobjERab, pUE);

               /* Change in QCI only do the update to RB->QCI Mangager */
               if (pobjERab->getQci() != stERabAddMod.stErabQosInfo.bErabQCI)
               {
                  m_objRbAdmissionController.rbAdditionInd(stERabAddMod.stErabQosInfo.bErabQCI);
                  m_objRbAdmissionController.rbReleaseInd(pobjERab->getQci());
               }
            }
            
            /* Update the DL/UL PRBs required in this ERAB object */
            pobjERab->updateReqDlPrb(stErabRbaRspInfo.eRabAcceptedLst[bRabIndex].uiReqPrbForDl);
            pobjERab->updateReqUlPrb(stErabRbaRspInfo.eRabAcceptedLst[bRabIndex].uiReqPrbForUl);
            
            /* existed ERAB QCI is cahnged from 1 to different QCI(2,3,4) */
            if((stERabAddMod.bErabId == pUE->getSpsErabId()) && 
               (stERabAddMod.stErabQosInfo.bErabQCI != RM_QCI_ONE))
            {   
               isSpsErabMdfy = true;
               pUE->relSpsParam();
            }   
            /* first time QCI-1 ERAB-setup */
            else if((stERabAddMod.bErabId != pUE->getSpsErabId()) &&
                  (stERabAddMod.stErabQosInfo.bErabQCI == RM_QCI_ONE))
            {
               if(!isQciOneBrrExist)
               {   
                  qciOneErabId = stERabAddMod.bErabId;
                  isQciOneBrrExist = true;
               }
            }

            /* update the modified bearer info to the Rab of the UE */
            pobjERab->modifyERAB(stERabAddMod);

            /* update emergency bearer presence flag */
            pUE->updateEmrgncyBrPrsnt(pUE, m_bArpForEmergencyBearer);

            break;
         } /* end of if to match erabid */
      } /* end of for to traverse incoming erab info list*/
   } /* end of for to traverse the Accepted bearers by PRB and QCI admission */

   /* Fill E-Rabs rejected list */
   for (U32 bRabIndex = 0; 
         ((bRabIndex < stErabRbaRspInfo.nErabsRejected) && 
          (bRabIndex < RMU_MAX_NUM_ERABS));
         bRabIndex++)
   {
      pstRabRspInfo->stErabRejectLst[bRabIndex].bErabId = 
         stErabRbaRspInfo.eRabRejectedLst[bRabIndex].bERabId;

      pstRabRspInfo->stErabRejectLst[bRabIndex].enCause = 
         (RmuS1apCause) stErabRbaRspInfo.eRabRejectedLst[bRabIndex].bCause;
   }

   pstRabRspInfo->bNoErabsAccepted = stErabRbaRspInfo.nErabsAccepted;
   pstRabRspInfo->bNoErabsRejected = stErabRbaRspInfo.nErabsRejected;
   
   if(pstRabRspInfo->bNoErabsAccepted)
   {
      pstRabRspInfo->stStatusInfo.enStatus = RMU_SUCCESS;
      pstRabRspInfo->stStatusInfo.enCause  = RMU_CAUSE_NOT_APP;
      bRetVal = true;
   }
   else
   {
      pstRabRspInfo->stStatusInfo.enStatus = RMU_FAILURE;
      pstRabRspInfo->stStatusInfo.enCause  = RMU_CAUSE_OUT_OF_RESOURCE;
      bRetVal = false;
   }

   if((isSpsErabMdfy == false) && (pUE->getSpsErabId() == 0))
   {
      if(isQciOneBrrExist == true)
      {   
         /* In case if some other GBR bearer QCI has been modified
            to QCI 1 then enable SPS if not enabled earlier for this UE*/
         pUE->setSpsParam(qciOneErabId);
      }
   }   
   RETVALUE(bRetVal);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRBearerController::processErabReleaseRequest(CUEContext *pUE,
                                                   RmuErabConfigIe & stERAB,
                                                   RmuUeRecfgRsp *pstRabRspInfo)
{
   RmuErabAdmitted    *pAcceptLst = pstRabRspInfo->stErabAcceptLst;
   rmuErabRejected    *pRejectLst = pstRabRspInfo->stErabRejectLst;
   U32                numErabsReleased = 0;
   U32                numErabsFailedToRel = 0;
 
   if((stERAB.bNoErabsToRel + pUE->getNumRabsPresentInUE()) > 
         RMU_MAX_NUM_ERABS)
   {
      pstRabRspInfo->stStatusInfo.enStatus = RMU_FAILURE;
      pstRabRspInfo->stStatusInfo.enCause  = RMU_CAUSE_OTHER;
      return false;
   }

   pstRabRspInfo->isGBRBearerExists = false;

	for (U32 bRabIndex = 0; bRabIndex < stERAB.bNoErabsToRel; ++bRabIndex)
	{
      U8                 qci;
      U8                 erabId;

      erabId = stERAB.stErabRelLst[bRabIndex].bErabId;
      CERABInfo *ptrObjERab = pUE->findERAB(erabId);
      if(NULLP == ptrObjERab)
      {
         pRejectLst[numErabsFailedToRel].bErabId = erabId;
         pRejectLst[numErabsFailedToRel].enCause = RMU_CAUSE_UNKNOWN_ERAB_ID;
         numErabsFailedToRel++;
         continue;
      }

      qci = ptrObjERab->getQci();

      /* update the removal of the E-RAB to RB admison 
       * about GBR bearer release */
      if(isGbrBearer(qci))
      {
         m_objRbAdmissionController.rbReleaseInd(qci);
        
         if(m_bArpForEmergencyBearer != ptrObjERab->getArp())
         {
            /* remove the E-RAB from Pre-emptable ERAB Map table */
            m_objPreEmptableRabsFinder.deleteERABInd(ptrObjERab, pUE);
         }
      }
 
      /* remove the ERAB from UE ERAB list */
	   pUE->releaseERAB(ptrObjERab);

      /* update emergency bearer presence flag */
      pUE->updateEmrgncyBrPrsnt(pUE, m_bArpForEmergencyBearer);

      /* Add the ERAB to free RAB list */
      m_tlFreeRabPool.push_back(ptrObjERab);

      pAcceptLst[numErabsReleased].bErabId = erabId;

      /* If released bearer is the SPS enabled ERAB then release SPS
       * and mark SPS enabling as FALSE in RAB response message */
      if(erabId == pUE->getSpsErabId())
      {
         pAcceptLst[numErabsReleased].bIsDlSpsEnabled = false;
         pAcceptLst[numErabsReleased].bIsUlSpsEnabled = false;
         pUE->relSpsParam();
      }
      numErabsReleased++;
	}

   /* update number of admitted and rejected bearers to reconfig response */
   pstRabRspInfo->bNoErabsAccepted = numErabsReleased;
   pstRabRspInfo->bNoErabsRejected = numErabsFailedToRel;

   return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRBearerController::processErabRequest(CUEContext *pUE,
                                            RmuErabConfigIe & stERAB, 
                                            RmuUeRecfgRsp *pstUeRecfgRsp)
{
   bool fRet = true;

   if(pUE == NULL || pstUeRecfgRsp == NULL)
      return false;

   if (stERAB.bIsUeAmbrPres)
   {
      pUE->setUlUeAmbr(stERAB.stUeAmbrInfo.uiAggUEMaxBitRateUL);
      pUE->setDlUeAmbr(stERAB.stUeAmbrInfo.uiAggUEMaxBitRateDL);
   }

   switch (stERAB.enErabAction)
   {
      case RMU_ADD_ERAB:
      {
         fRet = processErabSetupRequest(pUE, stERAB, pstUeRecfgRsp);
         break;
      }

      case RMU_MOD_ERAB:
      {
         fRet = processErabModifyRequest(pUE, stERAB, pstUeRecfgRsp);
         break;
      }

      case RMU_DEL_ERAB:
      {
         fRet = processErabReleaseRequest(pUE, stERAB, pstUeRecfgRsp);
         break;
      }

      default:
         RLOG1(L_WARNING,"Unknown ERab Action:%d", stERAB.enErabAction);
   }

   /* Update the QoS related parameters required for other features 
    * like Emergency CAC, DRX 
    */
   if(fRet)
   {
      pUE->updateHighPriorityQosParams();
      pstUeRecfgRsp->isGBRBearerExists = pUE->isGbrQciExisForAnr();
      pstUeRecfgRsp->bRecfgBitMap |=  RMU_RECFG_RSP_ERAB_CONFIG;
      pstUeRecfgRsp->stStatusInfo.enStatus = RMU_SUCCESS;
      pstUeRecfgRsp->stStatusInfo.enCause = RMU_CAUSE_NOT_APP;
      return true;
   }
   else
   {
      pstUeRecfgRsp->bRecfgBitMap |=  RMU_RECFG_RSP_ERAB_CONFIG;
      return false;
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CRBearerController::processErabAdmissionInHORequest(
                                                  CUEContext *pUE,
                                                  RmuErabConfigIe & stERAB,
                                                  RmuUeHoRsp *pstHoRspInfo)
{
   bool  bRetVal;

   if(stERAB.usNoErabsToAddMod > RMU_MAX_NUM_ERABS)
   {
      pstHoRspInfo->stStatusInfo.enStatus = RMU_FAILURE;
      pstHoRspInfo->stStatusInfo.enCause  = RMU_CAUSE_OTHER;
      return false;
   }

   if (stERAB.bIsUeAmbrPres)
   {
      pUE->setUlUeAmbr(stERAB.stUeAmbrInfo.uiAggUEMaxBitRateUL);
      pUE->setDlUeAmbr(stERAB.stUeAmbrInfo.uiAggUEMaxBitRateDL);
   }

   processIncomingERabsforSetup(pUE,
         stERAB, 
         pstHoRspInfo->stErabAdmitLst,
         pstHoRspInfo->stErabRejectLst,
         &pstHoRspInfo->bNoErabsAdmitted,
         &pstHoRspInfo->bNoErabsRejected);

   if(pstHoRspInfo->bNoErabsAdmitted)
   {
      /* Update High priority iQoS params used for UE admission */
      pUE->updateHighPriorityQosParams();
      pstHoRspInfo->isGBRBearerExists = pUE->isGbrQciExisForAnr();
      pstHoRspInfo->stStatusInfo.enStatus = RMU_SUCCESS;
      pstHoRspInfo->stStatusInfo.enCause  = RMU_CAUSE_NOT_APP;
      bRetVal = true;
   }
   else
   {
      pstHoRspInfo->stStatusInfo.enStatus = RMU_FAILURE;
      pstHoRspInfo->stStatusInfo.enCause  = RMU_CAUSE_OUT_OF_RESOURCE;
      bRetVal = false;
   }

   return bRetVal;
}

void CRBearerController::processIncomingERabsforSetup(CUEContext *pUE,
                                  RmuErabConfigIe & stERAB,
                                  RmuErabAdmitted *pstErabAdmitLst,
                                  RmuErabRejected *stErabRejectLst,
                                  U8  *bNoErabsAdmitted,
                                  U8  *bNoErabsRejected)
{
   RrmRbcRspInfo   stErabRbaRspInfo; 
   bool            bIsRabsPreEmpted;

   memset(&stErabRbaRspInfo, 0, sizeof(RrmRbcRspInfo));

   /* Update the CRNTI of the UE */
   stErabRbaRspInfo.usCRNTI = pUE->getCRNTI();

   /* check the requested bearers with Radio Bearere Admison (QCI, PRB based)*/
   m_objRbAdmissionController.canRBsBeAdmitted(pUE, 
                                               stERAB,
                                               stErabRbaRspInfo);

   /* Find a pre-emptable bearer, then trigger the pre-emption of the ERAB
    * if the incoming ERAB setup req contains at least one erab rejected 
    */
   if(stErabRbaRspInfo.nErabsRejected)
   {
      CERABInfoLst    objPreEmptedERABsList;

      bIsRabsPreEmpted = m_objPreEmptableRabsFinder.findPreEmptableERABs(
                                                      pUE->isCsgMember(),
                                                      stErabRbaRspInfo, 
                                                      objPreEmptedERABsList);
      /* trigger the pre-emption of ERABs */
      if(bIsRabsPreEmpted)
      {
         triggerERabRelIndPerUe(objPreEmptedERABsList);
      }
   }

   /* Fill E-Rabs accepted list */
   for (U32 bRabIndex = 0; bRabIndex < stErabRbaRspInfo.nErabsAccepted; bRabIndex++)
   {
      pstErabAdmitLst[bRabIndex].bErabId =
         stErabRbaRspInfo.eRabAcceptedLst[bRabIndex].bERabId;

      for (U32 bErabIndex =0; bErabIndex < stERAB.usNoErabsToAddMod; bErabIndex++)
      {
         RmuUErabAddModIe &stERabAddMod = stERAB.stErabAddModLst[bErabIndex];

         if(stErabRbaRspInfo.eRabAcceptedLst[bRabIndex].bERabId == 
               stERabAddMod.bErabId )
         {
            /* Add ERAB to the UE's ERabs list */
            CERABInfo *ptrErabObj = addErabToUe(pUE, stERabAddMod);

            ptrErabObj->updateReqDlPrb(
                  stErabRbaRspInfo.eRabAcceptedLst[bRabIndex].uiReqPrbForDl);
            ptrErabObj->updateReqUlPrb(
                  stErabRbaRspInfo.eRabAcceptedLst[bRabIndex].uiReqPrbForUl);

            /* Set SPS parameters (flag for SPS enabling + erab ID),
             * if QCI 1 bearer is admitted */
             if(stERabAddMod.stErabQosInfo.bErabQCI == RM_QCI_ONE)
             {
                pUE->setSpsParam(stERabAddMod.bErabId);
             }

            /* Add the new GBR ERab to the Pre-emptable MultiMap table. 
             * Emergency ARP bearer is not inserted into Pre-emptable MultiMap
             * table*/
            if(isGbrBearer(ptrErabObj->getQci()))
            {
               if(m_bArpForEmergencyBearer != ptrErabObj->getArp())
               {
                  m_objPreEmptableRabsFinder.erabAdditionInd(ptrErabObj, pUE);
               }
               /* Update the number of QCI X bearers admitted */
               m_objRbAdmissionController.rbAdditionInd(ptrErabObj->getQci());
            }
            break;
         } /* end of if to match erabid */
      } /* end of for to traverse incoming erab info list*/
   } /* end of for to traverse the Accepted bearers by PRB and QCI admission */

   /* Fill E-Rabs rejected list */
   for (U32 bRabIndex = 0; ((bRabIndex < stErabRbaRspInfo.nErabsRejected) && 
                            (bRabIndex < RMU_MAX_NUM_ERABS));
                            bRabIndex++)
   {
      stErabRejectLst[bRabIndex].bErabId = 
         stErabRbaRspInfo.eRabRejectedLst[bRabIndex].bERabId;

      stErabRejectLst[bRabIndex].enCause = 
         (RmuS1apCause) RMU_CAUSE_RADIO_RESOURCE_NOT_AVAIL;
   }

   *bNoErabsAdmitted = stErabRbaRspInfo.nErabsAccepted;
   *bNoErabsRejected = stErabRbaRspInfo.nErabsRejected;

   RLOG_ARG2(L_DEBUG, DBG_CRNTI, (pUE->getCRNTI()), 
             "RBC:No Bearers Accpeted:%d, Rejected:%d", 
             stErabRbaRspInfo.nErabsAccepted,
             stErabRbaRspInfo.nErabsRejected);
}

void CRBearerController::preemptErabDueToCongestion()
{
   CERABInfoLst   objPreEmptedERABsList;

   bool bIsRabsPreEmpted = 
      m_objPreEmptableRabsFinder.findLeastPriorityPreEmptableERAB(
                                                    objPreEmptedERABsList);

   /* trigger the pre-emption of ERABs */
   if(bIsRabsPreEmpted)
   {
      triggerERabRelIndPerUe(objPreEmptedERABsList);
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
S16 CRBearerController::triggerPrbReportConfig(U8 cellId, U32 action)
{
   RETVALUE(m_objRbAdmissionController.triggerPrbReportConfig(cellId, action));
}

/********************************************************************30**

           End of file:     qf_radiobearercontroller.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:35 2013

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
