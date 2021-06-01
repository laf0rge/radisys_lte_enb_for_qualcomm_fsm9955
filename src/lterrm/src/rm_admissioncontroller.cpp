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
               

     File:     rm_admissioncontroller.cpp

     Sid:      qf_admissioncontroller.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:32 2013

     Prg:     hkrishna 

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=139;

/* header include files */
#include "rm_application.h"
#include "rm_admissioncontroller.h"
#include "rm_cellmanager.h"
#include "rm_outinterface.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CAdmissionController::CAdmissionController(LrmCellConfigData& stCellCfg, CUEManager& oUEMgr) :  m_rObjUEMgr(oUEMgr)
{
   m_bWaitTime = stCellCfg.bWaitTime;
   m_bCellId = stCellCfg.bCellId;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CAdmissionController::admitMoSignalingCall(RmuStatusInfo& stStatusInfo)
{
   if(m_rObjUEMgr.isMaxSignalingUELimitReached() == false)
      return true;

   /*  Update the response structure with status: RRM_WR_CFG_REDIRECT
    *  and cause: RRM_WR_CAUSE_MAX_CAPACITY_REACHED */
   stStatusInfo.enStatus = RMU_REDIRECT;
   stStatusInfo.enCause = RMU_CAUSE_MAX_UE_REACHED;
   RLOG1(L_INFO,"Ue is suggested to Redirect Status [%d]",stStatusInfo.enStatus);

   return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CAdmissionController::admitMoSignalAsMoDataCall(RmuStatusInfo& stStatusInfo)
{
   return admitMoDataAndMtAccessCall(stStatusInfo);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CAdmissionController::admitMoDataAndMtAccessCall(RmuStatusInfo& stStatusInfo)
{
   if(m_rObjUEMgr.isMaxActiveUELimitReached() == false)
      return true;

   /*  Update the response structure with status: RRM_WR_CFG_REDIRECT
    *  and cause: RRM_WR_CAUSE_MAX_CAPACITY_REACHED */
   stStatusInfo.enStatus = RMU_REDIRECT;
   stStatusInfo.enCause = RMU_CAUSE_MAX_UE_REACHED;

   RLOG1(L_INFO,"Ue is suggested to Redirect Status [%d]",stStatusInfo.enStatus);
   return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CAdmissionController::admitEmergencyAndHighPriorityCall(RmuUeAdmitReq& stUEAdmReq, RmuStatusInfo& stStatusInfo)
{
   if(m_rObjUEMgr.isMaxActiveUELimitReached() == false)
      return true;

   /* Release a lowest priority UE */
   CUEContext* pCandidateUE=m_rObjUEMgr.findLeastPriorityUEToRelease(stUEAdmReq.enConEstabCause);

   /* Prepare and send the UE Release Indication message to the Candidate UE */
   if(pCandidateUE)
   {
      RLOG_ARG2(L_DEBUG, DBG_CRNTI, (stUEAdmReq.usCrnti), 
             "Pre-empted the UE:[%d]. Since the incoming UE admited with casue:[%d]", 
              (pCandidateUE->getCRNTI()),
             stUEAdmReq.enConEstabCause);
      rrmSendUeRelInd(m_bCellId, pCandidateUE->getCRNTI(), RMU_CAUSE_OTHER);
      /* Delete the UE CB and RBs and resources in RRM */
      g_rObjRRMApp.procDelUe(m_bCellId, pCandidateUE);

      stStatusInfo.enStatus = RMU_SUCCESS;
      stStatusInfo.enCause = RMU_CAUSE_NOT_APP;
      return true;
   }
   RLOG_ARG1(L_INFO, DBG_CRNTI, (stUEAdmReq.usCrnti), 
             "Not found the Pre-emptable UE. Hence not admiting"
             "the incoming UE with Cause:[%d]", 
             stUEAdmReq.enConEstabCause);

   /*  Update the response structure with status: RRM_WR_CFG_FAILURE
    *  and cause: RRM_WR_CAUSE_MAX_CAPACITY_REACHED */
   stStatusInfo.enStatus = RMU_FAILURE;
   stStatusInfo.enCause = RMU_CAUSE_MAX_UE_REACHED;

   return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CAdmissionController::admitExtraUE(RmuStatusInfo& stStatusInfo)
{
   if(m_rObjUEMgr.allowExtraUe() == false)
   {
      RLOG0(L_DEBUG, "Allowed the extra UE");
      return true;
   }

   /*  Update the response structure with status: RRM_WR_CFG_REDIRECT
    *  and cause: RRM_WR_CAUSE_MAX_CAPACITY_REACHED */
   stStatusInfo.enStatus = RMU_REDIRECT;
   stStatusInfo.enCause = RMU_CAUSE_MAX_UE_REACHED;
   RLOG0(L_DEBUG, "Not allowed the extra UE");

   return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CUEContext* CAdmissionController::handleUEAdmission(RmuUeAdmitReq& stUEAdmReq, RmuUeAdmitRsp* pstUeAdmitRsp)
{
   CUEContext* pUE=NULL;
   bool bAdmitUE = false;

   RLOG_ARG1(L_DEBUG, DBG_CRNTI, (stUEAdmReq.usCrnti), 
             "Received UE Admit Req with casue:[%d]", 
             stUEAdmReq.enConEstabCause);
   switch(stUEAdmReq.enConEstabCause)
   {
      case RMU_EST_CAUSE_EMERGENCY:
      case RMU_EST_CAUSE_HIGH_PRIORTY_ACCESS:
         bAdmitUE = admitEmergencyAndHighPriorityCall(stUEAdmReq, pstUeAdmitRsp->stStatusInfo);
         /* If admission failed then return failure.
          * If OAM configured waitTime is present, 
          * then use the OAM configured value. otherwise 
          * use default wait time */
         if ((bAdmitUE == FALSE ) && 
               (pstUeAdmitRsp->stStatusInfo.enStatus == RMU_FAILURE))
         {
            pstUeAdmitRsp->bWaitTime = m_bWaitTime ? 
               m_bWaitTime : RM_DEFAULT_WAIT_TIME_IN_SEC;
            RLOG_ARG1(L_INFO, DBG_CRNTI, (stUEAdmReq.usCrnti), 
                      "UE is not Admited and wait time is :[%d]", 
                       (pstUeAdmitRsp->bWaitTime));
            return pUE;
         }
         break;

      case RMU_EST_CAUSE_MO_SIGNALLING:
         bAdmitUE = admitMoSignalingCall(pstUeAdmitRsp->stStatusInfo);
         break;

      case RMU_EST_CAUSE_MT_ACCESS:
      case RMU_EST_CAUSE_MO_DATA:
      case RMU_EST_CAUSE_DELAY_TOLERANT:
         bAdmitUE = admitMoDataAndMtAccessCall(pstUeAdmitRsp->stStatusInfo);
         break;

      default:
         pstUeAdmitRsp->stStatusInfo.enStatus = RMU_FAILURE;
         pstUeAdmitRsp->stStatusInfo.enCause = RMU_CAUSE_OTHER;
         if ((bAdmitUE == FALSE ) && 
               (pstUeAdmitRsp->stStatusInfo.enStatus == RMU_FAILURE))
         {
            pstUeAdmitRsp->bWaitTime = m_bWaitTime ? 
               m_bWaitTime : RM_DEFAULT_WAIT_TIME_IN_SEC;
            RLOG_ARG1(L_INFO, DBG_CRNTI, (stUEAdmReq.usCrnti), 
                      "UE is not Admited and wait time is :[%d]", 
                       (pstUeAdmitRsp->bWaitTime));
            return pUE;
         }
         break;
   }

   /* HCSG_DEV */
   /* extra UE admit logic is only applicable for non-signalling */
   if(bAdmitUE || (stUEAdmReq.enConEstabCause != RMU_EST_CAUSE_MO_SIGNALLING &&
            admitExtraUE(pstUeAdmitRsp->stStatusInfo)))
   {
      pUE = m_rObjUEMgr.admitUE(stUEAdmReq.usCrnti, stUEAdmReq.enConEstabCause);
      RLOG_ARG1(L_INFO, DBG_CRNTI, (stUEAdmReq.usCrnti), 
                "UE is Successfully Admited  with cause :[%d]", 
                (stUEAdmReq.enConEstabCause));
      pstUeAdmitRsp->stStatusInfo.enStatus = RMU_SUCCESS;
      pstUeAdmitRsp->stStatusInfo.enCause = RMU_CAUSE_NOT_APP;
   }

   /* If OAM configured waitTime is present, then use the OAM configured value. otherwise use default wait time */
   if (pstUeAdmitRsp->stStatusInfo.enStatus == RMU_FAILURE)
   {
      pstUeAdmitRsp->bWaitTime = m_bWaitTime ? 
         m_bWaitTime : RM_DEFAULT_WAIT_TIME_IN_SEC;
      RLOG_ARG1(L_INFO, DBG_CRNTI, (stUEAdmReq.usCrnti), 
                "UE is not Admited and wait time is :[%d]", 
                (pstUeAdmitRsp->bWaitTime));
   }

   return pUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
CUEContext* CAdmissionController::handleUEHOAdmission(RmuUeHoReq & stUEHoReq, RmuUeHoRsp *pstUeHoRsp)
{
   CUEContext* pUE = NULL;
   /* HCSG DEV start */
   CUEContext* pRelUE = NULL;

   if(m_rObjUEMgr.isMaxActiveUELimitReached())
   {
      RLOG_ARG0(L_DEBUG, DBG_CRNTI, (stUEHoReq.usCrnti), "Max Active UE Limit is reached ");
      if(stUEHoReq.isEmrgncyBrersPsnt)
      {
         /* Release a lowest priority UE */
         pRelUE= m_rObjUEMgr.findLeastPriorityUEToRelease(RMU_EST_CAUSE_EMERGENCY);

         /* release the UE */
         if(pRelUE)
         {
            RLOG_ARG1(L_DEBUG, DBG_CRNTI, (stUEHoReq.usCrnti), 
                      "Pre-empted the UE:[%d] since Hand-in UE has emergency bearer",
                      (pRelUE->getCRNTI()));
            rrmSendUeRelInd(m_bCellId, pRelUE->getCRNTI(), RMU_CAUSE_OTHER);
            /* Delete the UE CB and RBs and resources in RRM */
            g_rObjRRMApp.procDelUe(m_bCellId, pRelUE);

            pUE = m_rObjUEMgr.admitUE(stUEHoReq.usCrnti, RMU_EST_CAUSE_EMERGENCY);

            pstUeHoRsp->stStatusInfo.enStatus = RMU_SUCCESS;
            pstUeHoRsp->stStatusInfo.enCause = RMU_CAUSE_NOT_APP;
         }
         else
         {
            RLOG_ARG0(L_INFO, DBG_CRNTI, (stUEHoReq.usCrnti), 
                      "Not found a Pre-emptable UE for Hand-in UE even has UE emergency bearer");
            pstUeHoRsp->stStatusInfo.enStatus = RMU_FAILURE;
            pstUeHoRsp->stStatusInfo.enCause = RMU_CAUSE_MAX_UE_REACHED;
         }
      }
      else if(admitExtraUE(pstUeHoRsp->stStatusInfo))
      {
         pUE = m_rObjUEMgr.admitUE(stUEHoReq.usCrnti, RMU_EST_CAUSE_HO_REQ);
         pstUeHoRsp->stStatusInfo.enStatus = RMU_SUCCESS;
         pstUeHoRsp->stStatusInfo.enCause = RMU_CAUSE_NOT_APP;
         RLOG_ARG0(L_DEBUG, DBG_CRNTI, (stUEHoReq.usCrnti), 
                  "Admited the Hand-in UE");
      }
      else
      {
         RLOG_ARG0(L_INFO, DBG_CRNTI, (stUEHoReq.usCrnti), 
                  "Not admited the Hand-in UE");
         pstUeHoRsp->stStatusInfo.enStatus = RMU_FAILURE;
         pstUeHoRsp->stStatusInfo.enCause = RMU_CAUSE_MAX_UE_REACHED;
      }
   }
   else
   {
      if(stUEHoReq.isEmrgncyBrersPsnt)
      {
         pUE = m_rObjUEMgr.admitUE(stUEHoReq.usCrnti, RMU_EST_CAUSE_EMERGENCY);
         RLOG_ARG0(L_DEBUG, DBG_CRNTI, (stUEHoReq.usCrnti), 
                  "Admited the Hand-in UE has Emergency bearer");
      }
      else
      {
         pUE = m_rObjUEMgr.admitUE(stUEHoReq.usCrnti, RMU_EST_CAUSE_HO_REQ);
         RLOG_ARG0(L_DEBUG, DBG_CRNTI, (stUEHoReq.usCrnti), 
                  "Admited the Hand-in UE ");
      }
      pstUeHoRsp->stStatusInfo.enStatus = RMU_SUCCESS;
      pstUeHoRsp->stStatusInfo.enCause = RMU_CAUSE_NOT_APP;
   }
   /* HCSG DEV end */

   return pUE;
}

/* HCSG_DEV - start */
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CAdmissionController::checkAndUpdateUEAdmission(CUEContext* pUE)
{
   bool        retVal = true;

   /* Update UE admission based on the received membership status */
   if(m_rObjUEMgr.isUEInExtraUEList(pUE))
   {
      RLOG_ARG0(L_DEBUG, DBG_CRNTI, (pUE->getCRNTI()), "UE is in EXTRA UE List ");
      if((RMU_EST_CAUSE_EMERGENCY == pUE->getEstablishmentCause()) ||
         (RMU_EST_CAUSE_HIGH_PRIORTY_ACCESS == pUE->getEstablishmentCause()))
      {
         if(pUE->isCsgMember())
         {
            /* remove from extra UE list to CSG active UE list */
            RLOG_ARG0(L_DEBUG, DBG_CRNTI, (pUE->getCRNTI()), 
                  "Move the UE from Extra UE list to CSG UE list ");
            m_rObjUEMgr.moveUEFrmExtLstToCsgLst(pUE);
         }
         else
         {
            /* remove from extra UE list to Non CSG active UE list */
            RLOG_ARG0(L_DEBUG, DBG_CRNTI, (pUE->getCRNTI()), 
                  "Move the UE from Extra UE list to non-CSG UE list ");
            m_rObjUEMgr.moveUEFrmExtLstToNonCsgLst(pUE);
         }
         pUE->setActiveUEFlag(TRUE);
      }
      /* if non member and max csg count is not reaached, then
       * continue serving the UE */
      else if(!pUE->isCsgMember() && 
            (!(m_rObjUEMgr.isMaxNonCsgUEsAdmitted())))
      {
         /* release a CSG UE and admit non-member UE */
         CUEContext  *pCandidateUE = m_rObjUEMgr.findCsgUEToRelease();
         if(pCandidateUE)
         {
            RLOG_ARG1(L_DEBUG, DBG_CRNTI, (pUE->getCRNTI()), 
                  "Non-CSG UE and Max non-CSG UEs are not reached."
                  "Hence Pre-empted CSG UE:[%d]",(pCandidateUE->getCRNTI()));
            /* NOTE: If none of the UEs satisfy the condition, the last
             * UE in the list is selected by default */
            rrmSendUeRelInd(m_bCellId, pCandidateUE->getCRNTI(),
                  RMU_CAUSE_MAX_CSG_UE_REACHED);
           /* Delete the UE CB and RBs and resources in RRM */
            g_rObjRRMApp.procDelUe(m_bCellId, pCandidateUE);
            RLOG_ARG0(L_DEBUG, DBG_CRNTI, (pUE->getCRNTI()), 
                  "Move the UE from Extra UE list to non-CSG UE list ");
            /* remove from extra UE list to non CSG active UE list */
            m_rObjUEMgr.moveUEFrmExtLstToNonCsgLst(pUE);
            pUE->setActiveUEFlag(TRUE);
         }
         else
         {
            retVal = false;
         }
      }
      else if(pUE->isCsgMember() &&
            (!(m_rObjUEMgr.isMaxCsgUEsAdmitted())))
      {
         /* release a non-CSG UE and admit member UE */
         CUEContext  *pCandidateUE = m_rObjUEMgr.findNonCsgUEToRelease();
         if(pCandidateUE)
         {
            RLOG_ARG1(L_DEBUG, DBG_CRNTI, (pUE->getCRNTI()), 
                  "CSG UE and Max CSG UEs are not reached."
                  "Hence Pre-empted non-CSG UE:[%d]",(pCandidateUE->getCRNTI()));
            /* NOTE: If none of the UEs satisfy the condition, the last
             * UE in the list is selected by default */
            rrmSendUeRelInd(m_bCellId, pCandidateUE->getCRNTI(),
                  RMU_CAUSE_MAX_NON_CSG_UE_REACHED);
           /* Delete the UE CB and RBs and resources in RRM */
            g_rObjRRMApp.procDelUe(m_bCellId, pCandidateUE);
            RLOG_ARG0(L_DEBUG, DBG_CRNTI, (pUE->getCRNTI()), 
                  "Move the UE from Extra UE list to CSG UE list ");
            /* remove from extra UE list to CSG active UE list */
            m_rObjUEMgr.moveUEFrmExtLstToCsgLst(pUE);
            pUE->setActiveUEFlag(TRUE);
         }
         else
         {
            retVal = false;
         }
      }
      else
      {
         retVal = false;
      }
   }
   else if(!pUE->isCsgMember() && 
         (RMU_EST_CAUSE_MO_SIGNALLING != pUE->getEstablishmentCause()))
   {
      /* if non member and max non CSG UEs count is not reached, then
       * continue serving the UE */
      RLOG_ARG0(L_DEBUG, DBG_CRNTI, (pUE->getCRNTI()), 
                  "Move the UE from CSG UE list to non-CSG UE list ");
      m_rObjUEMgr.moveUEFrmCsgLstToNonCsgLst(pUE);
   }
   /* by default UE is in CSG List, so the condition is not handled 
    * explicitly */

    RLOG_ARG1(L_INFO, DBG_CRNTI, (pUE->getCRNTI()),
                "[RRM]  UE admission status :[%d]",
                retVal);
   return retVal;
}
/* HCSG_DEV - end */

/********************************************************************30**

           End of file:     qf_admissioncontroller.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:32 2013

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
