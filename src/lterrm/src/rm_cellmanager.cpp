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
               

     File:     rm_cellmanager.cpp

     Sid:      qf_cellmanager.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/1 - Fri Jul 19 18:22:33 2013

     Prg:     jbm, vkorde 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=141;
/* header include files */
#include "rm_cmn.h"
#include "rm_cellmanager.h"
#include "rm_application.h"
#include "rm_acb.h"
#include "rm_outinterface.h"
#include "rm_prbreportcollector.h"

Bool CCellManager::m_bIsGbrDrxDis = 0;

CCellManager::CCellManager(LrmCellConfigData   *pstCellCfgDat): 
                           m_objAcb(*pstCellCfgDat),
                           m_objUEMgr(*pstCellCfgDat), 
                           m_oAdmCtrlr(*pstCellCfgDat, this->m_objUEMgr),
                           m_objResCfgMgr(*pstCellCfgDat), 
                           m_objRBearerCtrlr(*pstCellCfgDat, this),
                           m_objPrbCongCntrl(&m_objRBearerCtrlr, 
                                 pstCellCfgDat->bMaxAvgGbrPrbUsage,
                                 pstCellCfgDat->enCellDlBandWidth,
                                 pstCellCfgDat->utNumOfPrbRpts)
{
   m_stCellCfgInfo = *pstCellCfgDat;
   m_bCellId = pstCellCfgDat->bCellId;
   m_bIsPrbReportConfigured = false;
   m_enEnbState = RM_ENB_STATE_DOWN;
   m_bIsCellCongested = false;
   if(1 < pstCellCfgDat->bNumTxAnt)
   {
       m_dftTxMode = RM_UE_TXN_MODE2; 
   }
   else
   {
       m_dftTxMode = RM_UE_TXN_MODE1; 
   }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CCellManager::deleteUe(CUEContext *pUE)
{

   RLOG_ARG0(L_INFO, DBG_CRNTI, pUE->getCRNTI(), "[RRM] UEID for UE delete.");
   

   // Release Radio Resources Configured for the UE
   m_objResCfgMgr.rmRcmRelUeRes(pUE);

   /* Release all the bearers for this UE and move them to free EARB pool
    * and update the RBAdmission
    */
   m_objRBearerCtrlr.releaseAllERABs(pUE);

   // Release the UE Object
   m_objUEMgr.releaseUE(pUE);

   return;
}
U8 CCellManager::getTxnModeForUe(RmuEutraCapInfo &stEutraCapInfo)
{
   U8    bTxnMode;

   if(1 < getNumTxAnt()) 
   {
      /* By default TM2 will be configured.
       * if the configuration itself is TM2,
       * or UE Category is 1 will take the default value TM2 */
      bTxnMode = RM_UE_TXN_MODE2;

      if(CM_LTE_UE_CAT_1 < stEutraCapInfo.uiUeCategory)
      {
         if(4 == m_stCellCfgInfo.flagTransMode)
         {
            bTxnMode = RM_UE_TXN_MODE4;
         }
         else if(3 == m_stCellCfgInfo.flagTransMode)
         {
            bTxnMode = RM_UE_TXN_MODE3;
         }
      }
   }
   else
   {
      RLOG1(L_INFO,"TransMode set to SISO as NumTxAnt set to 1, though preferred mode is %d\n",m_stCellCfgInfo.flagTransMode);
      bTxnMode = RM_UE_TXN_MODE1;
   }

   return bTxnMode;
}

S16 CCellManager::triggerStopPrbUsageReport()
{

   RLOG1(L_INFO, "Configuring the MAC to stop PRB usage report "
                     "for the cell [%d]", getCellID());
   RETVALUE(m_objRBearerCtrlr.triggerPrbReportConfig(getCellID(),
            RGM_PRB_REPORT_STOP));
}

S16 CCellManager::triggerStartPrbUsageReport()
{
  RLOG1(L_INFO, "Configuring the MAC to start PRB usage report "
                    " for the cell [%d]", getCellID());
   RETVALUE(m_objRBearerCtrlr.triggerPrbReportConfig(getCellID(), 
            RGM_PRB_REPORT_START));
}

void CCellManager::handlePrbRprt(RgmPrbRprtInd *prbRprt)
{
   /* update PRB report collector */
   CPRBReportCollector::getInstance().updateQCIsPRBReport(prbRprt);
   m_bIsCellCongested = m_objPrbCongCntrl.checkForCongestion();
}

void CCellManager::procUeAdmitReq(RmuUeAdmitReq & stUeAdmitReq)
{
   RmuUeAdmitRsp* pstUeAdmitRsp =(RmuUeAdmitRsp*)rrmAlloc(sizeof(RmuCommonMsg));
   
   if(NULLP == pstUeAdmitRsp)
   {
      RLOG0(L_FATAL, "RRM->FSM: Not able to allocate memory for UE ADMIT RSP ");
      return;
   }

   if(RM_ENB_STATE_DOWN == getEnbState())
   {
      RLOG_ARG0(L_WARNING, DBG_CRNTI, stUeAdmitReq.usCrnti,
            "RRM: eNodeB State is Down. Dont Admit UE ");
		pstUeAdmitRsp->stStatusInfo.enStatus = RMU_FAILURE;
		pstUeAdmitRsp->stStatusInfo.enCause = RMU_CAUSE_OTHER;
		rrmSendUeAdmitRsp(stUeAdmitReq, pstUeAdmitRsp);
      return;
   }

   CUEContext* pUE = m_objUEMgr.findUE(stUeAdmitReq.usCrnti);

   if (pUE != NULL)
   {
      /* Prepare the Failure Response and send to FSM */
      RLOG_ARG0(L_WARNING, DBG_CRNTI, stUeAdmitReq.usCrnti,
            "RRM: UE already exists with this CRNTI ");
      pstUeAdmitRsp->stStatusInfo.enStatus = RMU_FAILURE;
      pstUeAdmitRsp->stStatusInfo.enCause = RMU_CAUSE_UEID_ALREADY_EXISTS;
      rrmSendUeAdmitRsp(stUeAdmitReq, pstUeAdmitRsp);
      /* Free the FSM Request UE Admit Request */
      return;
   }

   /* validate UE admission based ACB state */
   if(m_objAcb.admitUE(stUeAdmitReq.enConEstabCause) == false)
   {
      RLOG_ARG0(L_WARNING, DBG_CRNTI, stUeAdmitReq.usCrnti,
            "RRM: UE admission failed due to on-going ACB");
      /* Prepare the Failure Response and send to FSM */
      pstUeAdmitRsp->stStatusInfo.enStatus = RMU_FAILURE;
      pstUeAdmitRsp->stStatusInfo.enCause = RMU_CAUSE_OTHER;
      rrmSendUeAdmitRsp(stUeAdmitReq, pstUeAdmitRsp);
      return;
   }
   pUE = m_oAdmCtrlr.handleUEAdmission(stUeAdmitReq, pstUeAdmitRsp);

   // Configure resources if establishment cause is not MO_SIGNALLING
   if( pUE != NULL) 
   {
      /* Set the default tramsis mode for UE */
      pUE->setUeTxnMode(m_dftTxMode);

      if(!(m_objUEMgr.isRadioResThrsldReached()))
      {
         m_objResCfgMgr.rmRcmCfgUeRes(pUE, pstUeAdmitRsp->stUeRadioResCfg);

         RLOG_ARG3(L_INFO, DBG_CRNTI, (pUE->getCRNTI()),
               "Resource Configuration during UE Admission: UE-Index [%d], "
               "SRCfg Idx [%d], CQICfg Idx [%d]",
               pUE->getUeIdx(), 
               pstUeAdmitRsp->stUeRadioResCfg.stPhyCfgDed.stSrCfg.usSrIdx,
               pstUeAdmitRsp->stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.usCqiConfigIndex);
      }
      else
      {
         RLOG_ARG1(L_WARNING, DBG_CRNTI, stUeAdmitReq.usCrnti,
               "Max Radio Resource threshold reached [%d] ", 
               m_objUEMgr.isRadioResThrsldReached());

         pstUeAdmitRsp->stUeRadioResCfg.bRadioResDedBitMap |= 
            RM_RCM_PHY_DED_RES_CFG;
         pstUeAdmitRsp->stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap |= 
            RM_RCM_PHY_DED_RES_CFG_ANT;
         pstUeAdmitRsp->stUeRadioResCfg.stPhyCfgDed.stAntInfo.eTxnMode = 
            (RmuTxnMode)m_dftTxMode;
      }
   }
   else
   {
      RLOG_ARG0(L_WARNING, DBG_CRNTI, stUeAdmitReq.usCrnti,
            "UE admission failed");

      /* Filling TX mode */
      if(RMU_REDIRECT == pstUeAdmitRsp->stStatusInfo.enStatus)
      {
         pstUeAdmitRsp->stUeRadioResCfg.bRadioResDedBitMap |= 
            RM_RCM_PHY_DED_RES_CFG;
         pstUeAdmitRsp->stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap |= 
            RM_RCM_PHY_DED_RES_CFG_ANT;
         pstUeAdmitRsp->stUeRadioResCfg.stPhyCfgDed.stAntInfo.eTxnMode = 
           (RmuTxnMode)m_dftTxMode;
      }
   }

   rrmSendUeAdmitRsp(stUeAdmitReq, pstUeAdmitRsp);

   m_objUEMgr.printStats();
}

void CCellManager::procUeInactInd(RmuUeInactInd & stUeInactInd)
{
   CUEContext* pUE = m_objUEMgr.findUE(stUeInactInd.uiCrnti);

   if (pUE == NULL)
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, stUeInactInd.uiCrnti,
            "RRM: UE context is not available");
      return;
   }

   if(RMU_UE_NO_DATA == stUeInactInd.uiInactEvt) 
   {
      pUE->incrementUeInactivityTimerExpiryCount();
      RLOG_ARG2(L_DEBUG, DBG_CRNTI, stUeInactInd.uiCrnti,
            "[RRM] UE Inactivity Indication: UEID: %d Count: %ld",
            (pUE->getCRNTI()), (pUE->getInactivityTimerExpiryCount()));
   }
   else
   {
      pUE->resetUeInactivityTimerExpiryCount();
      RLOG_ARG0(L_DEBUG, DBG_CRNTI, stUeInactInd.uiCrnti,
            "[RRM] Reset UE Inactivity for UE ");
   }

   return;
}


void CCellManager::modCsgMembrShp(CUEContext *pUE,
                                  RmuUeRecfgReq & stUeRecfgReq,
                                  RmuUeRecfgRsp *pstUeRecfgRsp)
{
   if(pUE->isCsgMember() != stUeRecfgReq.isMember)
   {
      if(pUE->isCsgMember())
      {
         if(!stUeRecfgReq.isMember)
         {
            RLOG_ARG0(L_INFO, DBG_CRNTI, stUeRecfgReq.usCrnti,
                "[RRM] UE Membership is changed from CSG to non-CSG member ");
            /* Do the list manipulation */
            m_objUEMgr.moveUEFrmCsgLstToNonCsgLst(pUE);
         }
      }
      else
      {
         if(stUeRecfgReq.isMember)
         {
            RLOG_ARG0(L_INFO, DBG_CRNTI, stUeRecfgReq.usCrnti,
                "[RRM] UE Membership is changed from non-CSG to CSG member ");
            /* Do the list manipulation */
            m_objUEMgr.moveUEFrmNonCsgLstToCsgLst(pUE);
         }
      }

      /* DONT CHANGE calling the below function CSG 
       * membership upidate to RBC ; 
       * make sure to call the function updateCsgMemberShip 
       * before the calling  setMemberShip*/
      updateCsgMemberShip(pUE, m_objRBearerCtrlr, stUeRecfgReq.isMember);

      pUE->setMemberShip(stUeRecfgReq.isMember);
   }

   pstUeRecfgRsp->bRecfgBitMap |= RMU_RECFG_REQ_CSG_MEMBERSHIP_MOD;
   pstUeRecfgRsp->stStatusInfo.enStatus = RMU_SUCCESS;
   pstUeRecfgRsp->stStatusInfo.enCause = RMU_CAUSE_NOT_APP;
   RLOG_ARG0(L_DEBUG, DBG_CRNTI, stUeRecfgReq.usCrnti,
                "[RRM] UE Membership change is updated successfully ");
   rrmSendUeRecfgRsp(stUeRecfgReq, pstUeRecfgRsp);
}

void CCellManager::updateCRNTI(RmuUeRecfgReq & stUeRecfgReq,
                               RmuUeRecfgRsp *pstUeRecfgRsp)
{
   m_objUEMgr.updateNewCRNTI(stUeRecfgReq.usCrnti, stUeRecfgReq.usUeNewCrnti);
   pstUeRecfgRsp->stStatusInfo.enStatus = RMU_SUCCESS;
   pstUeRecfgRsp->stStatusInfo.enCause = RMU_CAUSE_NOT_APP;
   RLOG_ARG0(L_DEBUG, DBG_CRNTI, stUeRecfgReq.usCrnti,
                "[RRM] UE ID change is successfully updated.");
   rrmSendUeRecfgRsp(stUeRecfgReq, pstUeRecfgRsp);
}

void CCellManager::handleERABReq(CUEContext *pUE, 
                                 RmuUeRecfgReq & stUeRecfgReq,
                                 RmuUeRecfgRsp *pstUeRecfgRsp)
{
   bool bSendRsp = m_objRBearerCtrlr.processErabRequest(
                                                  pUE,
                                                  stUeRecfgReq.stErabConfInfo,
                                                  pstUeRecfgRsp);

   if(bSendRsp != TRUE)
   {
      // 1. Add pending Rab Node for the Rab Request
      // 2. copy the Rab Request and Response in the into the pending Rab Node
      // store the back pointer of the UE to be released
      // 3. Process the Rab Rel Indication For the Rabs To be Deleted
      return;
   }
}

void CCellManager::handleUECapInfo(CUEContext *pUE, 
                                   RmuUeRecfgReq & stUeRecfgReq)
{
   U8 bTxnMode = getTxnModeForUe( stUeRecfgReq.stUeCapInfo.stEutraCapInfo);

   pUE->setUeCapInfo( stUeRecfgReq.stUeCapInfo);
   pUE->setUeTxnMode(bTxnMode);
   RLOG_ARG1(L_DEBUG, DBG_CRNTI, stUeRecfgReq.usCrnti,
                "[RRM] UE Tx Mode:[%d]", bTxnMode);
}
void CCellManager::handleUEReconfigReq(CUEContext *pUE, 
                                       RmuUeRecfgReq & stUeRecfgReq,
                                       RmuUeRecfgRsp *pstUeRecfgRsp)
{
   if( pUE != NULL)
   {
      if (!(m_objUEMgr.isRadioResThrsldReached()))
      {
         m_objResCfgMgr.rmRcmRecfgUeRes(pUE, stUeRecfgReq, pstUeRecfgRsp);
      }

      RLOG_ARG4(L_INFO, DBG_CRNTI, (pUE->getCRNTI()),"RRC Connection Reconfiguration: "
            "SRCfg Idx [%d], CQICfg Idx [%d], RI Idx[%d], Drx Start Offset [%d]",
            pstUeRecfgRsp->stUeRadioResCfg.stPhyCfgDed.stSrCfg.usSrIdx,
            pstUeRecfgRsp->stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.usCqiConfigIndex,
            pstUeRecfgRsp->stUeRadioResCfg.stPhyCfgDed.stCqiRepCfg.stPrdCqiRepCfg.riConfigIndex.usRiConfigIndex,
            pstUeRecfgRsp->stUeRadioResCfg.stMacMainCfg.stRmDrxCfg.stDrxLongCycleOffst.usDrxStartOffst);
   }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CCellManager::procChangeTransMode(RgmTransModeInd *stChangeTransMode)
{
   RgmTxnMode oldMode;
   CUEContext * pUE = m_objUEMgr.findUE(stChangeTransMode->usCrnti);
   if (pUE == NULL)
   {
      printf("UE corresponds to CRNTI %d is not found at RRM\n",stChangeTransMode->usCrnti);
      return;
   }
   oldMode = (RgmTxnMode)pUE->getUeTxnMode();
   if(stChangeTransMode->eMode != oldMode)
   {
      pUE->setUeTxnMode(stChangeTransMode->eMode);

      RmuUeRecfgInd *pstUeRecfgInd = (RmuUeRecfgInd *) rrmAlloc(sizeof(RmuCommonMsg));

      if(NULLP == pstUeRecfgInd)
      {
         return;

      }
      if(ROK !=  m_objResCfgMgr.rmRcmProcTxnModeInd(pUE,pstUeRecfgInd->stUeRadioResCfg))
      {
         return;
      }
      pstUeRecfgInd->bCellId = stChangeTransMode->bCellId;
      pstUeRecfgInd->usCrnti = stChangeTransMode->usCrnti;
      rrmSendUeRecfgInd(pstUeRecfgInd);
   }
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CCellManager::procUeRecfgReq(RmuUeRecfgReq & stUeRecfgReq)
{
   RmuUeRecfgRsp *pstUeRecfgRsp = (RmuUeRecfgRsp *) rrmAlloc(sizeof(RmuCommonMsg));
   
   if(NULLP == pstUeRecfgRsp)
   {
      RLOG0(L_FATAL, "RRM->FSM: Not able to allocate memory for UE RECFG RSP ");
      return;
   
   }
   CUEContext* pUE = m_objUEMgr.findUE( stUeRecfgReq.usCrnti ); 
   if (pUE == NULL) 
   {
      RLOG_ARG0(L_INFO, DBG_CRNTI, stUeRecfgReq.usCrnti,
                "[RRM] Not found the UE for this UEID");
      /* Prepare the Failure Response and send to FSM */
      pstUeRecfgRsp->stStatusInfo.enStatus = RMU_FAILURE;
      pstUeRecfgRsp->stStatusInfo.enCause = RMU_CAUSE_UEID_UNKNOWN;
      pstUeRecfgRsp->bNoErabsAccepted = 0;
      pstUeRecfgRsp->bNoErabsRejected = 0;
      rrmSendUeRecfgRsp( stUeRecfgReq, pstUeRecfgRsp);
      return;
   }

   if (stUeRecfgReq.bRecfgBitMap & RMU_RECFG_REQ_NEW_CRNTI)
   {
      RLOG_ARG0(L_DEBUG, DBG_CRNTI, stUeRecfgReq.usCrnti,
                "[RRM] Received UERECFG for UE ID change request.");
      updateCRNTI(stUeRecfgReq, pstUeRecfgRsp);
      return;
   }

   /* HCSG_DEV - start */
   if(stUeRecfgReq.bRecfgBitMap & RMU_RECFG_REQ_CSG_MEMBERSHIP_MOD)
   {
      RLOG_ARG0(L_DEBUG, DBG_CRNTI, stUeRecfgReq.usCrnti,
                "[RRM] Received UERECFG for UE CSG membership update.");
      modCsgMembrShp(pUE, stUeRecfgReq, pstUeRecfgRsp);
      return;
   }/* End of RMU_RECFG_REQ_CSG_MEMBERSHIP_MOD */

   if (false == pUE->isCsgMemberShipIdentifed())
   {
      pUE->setMemberShip(stUeRecfgReq.isMember);
      pUE->setCsgMeberShipIdentified(true);

      m_objUEMgr.printStats();

      /* perform UE admission based on the received membership information */
      if(!m_oAdmCtrlr.checkAndUpdateUEAdmission(pUE))
      {
         RLOG_ARG0(L_INFO, DBG_CRNTI, stUeRecfgReq.usCrnti,
                "[RRM] UE admission failed since MAX EXTRA UE reached and"
                 "redirection enabled ");
         pstUeRecfgRsp->stStatusInfo.enStatus = RMU_REDIRECT;
         pstUeRecfgRsp->stStatusInfo.enCause = RMU_CAUSE_MAX_EXTRA_UE_REACHED;
         rrmSendUeRecfgRsp(stUeRecfgReq, pstUeRecfgRsp);
         return;
      }
   }
   /* HCSG_DEV - end */

   if( stUeRecfgReq.bRecfgBitMap & RMU_RECFG_REQ_ERAB_CONFIG)
   {
      /* If the RAB Request is success, Rab-list is not Empty and 
       * the UE is in the Signalling list,
       * Move it to Avtive UE list
       */
      if(FALSE == pUE->isActiveUE())
      {
         CUEContext  *pCandidateUE ;
         pCandidateUE = m_objUEMgr.moveUEfromSigLstToActiveLst(pUE);
         if(pUE == pCandidateUE)
         {
            RLOG_ARG0(L_INFO, DBG_CRNTI, stUeRecfgReq.usCrnti,
                  "[RRM] Not found any pre-emptable UE.");
            RLOG_ARG0(L_INFO, DBG_CRNTI, stUeRecfgReq.usCrnti,
                  "UE admission failed since MAX EXTRA UE "
                  "reached and redirection enabled ");
            pstUeRecfgRsp->stStatusInfo.enStatus = RMU_REDIRECT;
            pstUeRecfgRsp->stStatusInfo.enCause =
               RMU_CAUSE_MAX_EXTRA_UE_REACHED;
            // Send the Ue Recfg Response
            rrmSendUeRecfgRsp(stUeRecfgReq, pstUeRecfgRsp);
            return;
         }
         else if(NULL != pCandidateUE)
         {
            RLOG_ARG1(L_INFO, DBG_CRNTI, stUeRecfgReq.usCrnti,
                  "[RRM] Found pre-emptable UE ID is:[%d]."
                  "Hence admitted this UE", (pCandidateUE->getCRNTI()));
            rrmSendUeRelInd(m_bCellId, pCandidateUE->getCRNTI(),
                  RMU_CAUSE_MAX_CSG_UE_REACHED);
            deleteUe(pCandidateUE);

         }
      }

      if(TRUE == pUE->isActiveUE())
      {
         handleERABReq(pUE, stUeRecfgReq, pstUeRecfgRsp);
      }
      else 
      {
          RLOG_ARG0(L_WARNING, DBG_CRNTI, (pUE->getCRNTI()),
            "UE not made as Active and either Redirected or Released");
      }
   }
   if( pstUeRecfgRsp->stStatusInfo.enStatus == RMU_SUCCESS)
   {
      /* Update the UE Capability Info */
      if ( stUeRecfgReq.bRecfgBitMap & RMU_RECFG_REQ_UE_CAP)
      {
         RLOG_ARG0(L_DEBUG, DBG_CRNTI, stUeRecfgReq.usCrnti,
                   "[RRM] Received UERECFG for UE capability update request.");
         handleUECapInfo(pUE, stUeRecfgReq);
      }

      handleUEReconfigReq(pUE, stUeRecfgReq, pstUeRecfgRsp);
   }

   // Send the Ue Recfg Response
   rrmSendUeRecfgRsp(stUeRecfgReq, pstUeRecfgRsp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CCellManager::procUeRelReq(RmuUeRelReq *pstUeRelReq)
{
   RmuUeRelRsp *pstUeRelRsp = (RmuUeRelRsp*) rrmAlloc(sizeof(RmuCommonMsg));

   if(NULLP == pstUeRelRsp)
   {
      RLOG0(L_FATAL, "RRM->FSM: Not able to allocate memory for UE REL RSP ");
      return;
   }

   CUEContext* pUE = m_objUEMgr.findUE(pstUeRelReq->usCrnti);

   pstUeRelRsp->stStatusInfo.enStatus = RMU_SUCCESS;
   pstUeRelRsp->stStatusInfo.enCause = RMU_CAUSE_NOT_APP;

   if (pUE == NULL)
   {
      /* Prepare the Failure Response and send to FSM */
      RLOG_ARG0(L_INFO, DBG_CRNTI, pstUeRelReq->usCrnti,
                "[RRM] Not found the UE for this UEID .seems RRM Already Released ");
      //pstUeRelRsp->stStatusInfo.enStatus = RMU_FAILURE;
      //pstUeRelRsp->stStatusInfo.enCause = RMU_CAUSE_UEID_UNKNOWN;
      rrmSendUeRelRsp(pstUeRelReq, pstUeRelRsp);
      return;
   }

   deleteUe(pUE);

   // Send the Ue Recfg Response
   rrmSendUeRelRsp(pstUeRelReq, pstUeRelRsp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CCellManager::procUeHoReq(RmuUeHoReq & stUeHoReq)
{
   bool bSendRsp = FALSE;

   RmuUeHoRsp *pstUeHoRsp = (RmuUeHoRsp *) rrmAlloc(sizeof(RmuCommonMsg));

   if(NULLP == pstUeHoRsp)
   {
      RLOG0(L_FATAL, "RRM->FSM: Not able to allocate memory for UE HO RSP ");
      return;
   }
   CUEContext* pUE = m_objUEMgr.findUE(stUeHoReq.usCrnti);

   /* Added chnages to return if enodeb state is down */
   if(RM_ENB_STATE_DOWN == getEnbState())
   {
      RLOG_ARG0(L_WARNING, DBG_CRNTI, stUeHoReq.usCrnti,"RRM: eNodeB State is Down. Dont Admit UE ");
      pstUeHoRsp->stStatusInfo.enStatus = RMU_FAILURE;
      pstUeHoRsp->stStatusInfo.enCause = RMU_CAUSE_UEID_ALREADY_EXISTS;
      rrmSendUeHoRsp(stUeHoReq, pstUeHoRsp);
      return;
   }

   pUE = m_oAdmCtrlr.handleUEHOAdmission(stUeHoReq, pstUeHoRsp);
   if (pUE == NULL)
   {
      /* Prepare the Failure Response and send to FSM */
      RLOG_ARG0(L_INFO, DBG_CRNTI, stUeHoReq.usCrnti,"RRM: UE is not admitted while hand-in.");
      pstUeHoRsp->stStatusInfo.enStatus = RMU_FAILURE;
      pstUeHoRsp->stStatusInfo.enCause = RMU_CAUSE_OUT_OF_RESOURCE;
      rrmSendUeHoRsp(stUeHoReq, pstUeHoRsp);
      return;
   }

   /* HCSG_DEV - start */
   pUE->setMemberShip(stUeHoReq.isMember);
   
   pUE->setCsgMeberShipIdentified(true);
   /* perform UE admission based on the received memebership information */
   if(!m_oAdmCtrlr.checkAndUpdateUEAdmission(pUE))
   {
      RLOG_ARG0(L_INFO, DBG_CRNTI, stUeHoReq.usCrnti,"RRM: UE is not admitted while hand-in"
            "since MAX EXTRA UE reached");
      pstUeHoRsp->stStatusInfo.enStatus = RMU_FAILURE;
      pstUeHoRsp->stStatusInfo.enCause = RMU_CAUSE_MAX_EXTRA_UE_REACHED;
   }
   /* HCSG_DEV - end */

   if (pstUeHoRsp->stStatusInfo.enStatus == RMU_SUCCESS)
   {
      bSendRsp = m_objRBearerCtrlr.processErabAdmissionInHORequest(
                                                  pUE,
                                                  stUeHoReq.stErabConfInfo, 
                                                  pstUeHoRsp);

      if (bSendRsp != TRUE)
      {
         // Send the HO Response
         rrmSendUeHoRsp(stUeHoReq, pstUeHoRsp);
         return;
      }
      // Update UE Capability
      U8 bTxnMode;
      pUE->setUeCapInfo(stUeHoReq.stUeCapInfo);
      bTxnMode = getTxnModeForUe(stUeHoReq.stUeCapInfo.stEutraCapInfo);
      pUE->setUeTxnMode(bTxnMode);

      // TODO: Next Phase::The AS Config Resources received in the HO Req need to be checked for the availabilty
      // in the Target Resource pool

      // Currently we are reconfiguring the resources and send the same in the HO Command
      if(( pUE != NULL) && !(m_objUEMgr.isRadioResThrsldReached()))
      {
         m_objResCfgMgr.rmRcmRecfgUeRes(pUE, stUeHoReq, pstUeHoRsp);
      }
   }

   // Send the HO Response
   rrmSendUeHoRsp(stUeHoReq, pstUeHoRsp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void CCellManager::reConfigureCell(LrmCellConfiguration & stCellCfgInfo)
{
   LrmCellConfigData &stCellCfgData = stCellCfgInfo.stCellCfgInfo;
   
   if(stCellCfgInfo.usReCfgType & (LRM_CELL_RECFGTYPE_ACB_MODATA |
                                   LRM_CELL_RECFGTYPE_ACB_MOSIG |
                                   LRM_CELL_RECFGTYPE_ACB_EMERGENCY))
   { 
      RLOG0(L_DEBUG, "RRM: Cell Re-configuration recvd fro ACB .");
      /*Update the ACB class for the OAM updatation ACB params */
      m_objAcb.handleCellReConfiguration(stCellCfgInfo);

      if (stCellCfgInfo.usReCfgType & LRM_CELL_RECFGTYPE_ACB_MODATA)
      {
         RLOG0(L_DEBUG, "RRM: Cell Re-configuration recvd for ACB MO-data.");
         m_stCellCfgInfo.stAcbInfo.stAcBarringMoData = stCellCfgData.stAcbInfo.stAcBarringMoData;
         m_stCellCfgInfo.stAcbInfo.bIsacBarringMoData = stCellCfgData.stAcbInfo.bIsacBarringMoData;
      }

      if (stCellCfgInfo.usReCfgType & LRM_CELL_RECFGTYPE_ACB_MOSIG)
      {
         RLOG0(L_DEBUG, "RRM: Cell Re-configuration recvd for ACB MO-Sig.");
         m_stCellCfgInfo.stAcbInfo.stAcBarringMoSig = stCellCfgData.stAcbInfo.stAcBarringMoSig;
         m_stCellCfgInfo.stAcbInfo.bIsacBarringMoSig = stCellCfgData.stAcbInfo.bIsacBarringMoSig;
      }

      if (stCellCfgInfo.usReCfgType & LRM_CELL_RECFGTYPE_ACB_EMERGENCY)
      {
         RLOG0(L_DEBUG, "RRM: Cell Re-configuration recvd for ACB Emergency.");
         m_stCellCfgInfo.stAcbInfo.bAcBarringEmergency = stCellCfgData.stAcbInfo.bAcBarringEmergency;
      }
   }

   if (stCellCfgInfo.usReCfgType & LRM_CELL_RECFGTYPE_WAIT_TIME)
   {
      RLOG0(L_DEBUG, "RRM: Cell Re-configuration recvd for wait time.");
      m_stCellCfgInfo.bWaitTime = stCellCfgData.bWaitTime;
   }

   /* HCSG DEV */
   if (stCellCfgInfo.usReCfgType & LRM_CELL_RECFGTYPE_CSG_PARAM)
   {
      RLOG0(L_DEBUG, "RRM: Cell Re-configuration recvd for CSG params.");
      m_objUEMgr.cellRecfg(stCellCfgData);
   }

   if (stCellCfgInfo.usReCfgType & LRM_CELL_RECFGTYPE_TRANS_MODE)
   {
      m_stCellCfgInfo.flagTransMode = stCellCfgData.flagTransMode;
      //m_objRcmCqiMgr.m_transModeUse = stCellCfgData.flagTransMode;
      //m_objUEMgr.cellRecfg(stCellCfgData);
   }

   /* if there is any other case need to add similar logic*/
}

void CCellManager::procUeSpsDisInd(RmuUeSpsDisInd *pstUeSpsDisInd)
{
   CUEContext* pUE = m_objUEMgr.findUE(pstUeSpsDisInd->usCrnti);

   if(pUE != NULLP)
   {
      RLOG_ARG0(L_INFO, DBG_CRNTI, pstUeSpsDisInd->usCrnti,
                "[RRM] Recvd and processig the SPS diable indication ");
      m_objResCfgMgr.rmRcmSpsDisInd(pUE);
   }     
   else
   {
      RLOG_ARG0(L_INFO, DBG_CRNTI, pstUeSpsDisInd->usCrnti,
                "[RRM] Not found the UE for this UEID on reception"
                "of SPS disable indication");
   }
}   

/* Gets the eNB State */
RmEnbState CCellManager::getEnbState()
{
   return m_enEnbState;
}

void CCellManager::setEnbState(U8 bEnbState)
{
   if(0 == bEnbState)
   {
      m_enEnbState = RM_ENB_STATE_UP;
      RLOG0(L_DEBUG, "[RRM] ENodeb state is UP");
   }
   else
   {
      m_enEnbState = RM_ENB_STATE_DOWN;
      RLOG0(L_DEBUG, "[RRM] ENodeb state is DOWN");
   }
}

/********************************************************************30**

           End of file:     qf_cellmanager.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/Br_Tenb_Mspd_T2K_Intg_MS8.0_RC/1 - Fri Jul 19 18:22:33 2013

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
