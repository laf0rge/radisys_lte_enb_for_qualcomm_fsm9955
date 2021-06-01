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
               

     File:     rm_application.cpp

     Sid:      qf_application.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:32 2013

     Prg:     jbm, vkorde 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=136;

/* header include files */
#include <iostream>
#include <memory>
#include <new>
using namespace std;

#include "rm_application.h"
#include "rm_cmn.h"
#include "rm_acb.h"



RmTddUlSfInfo CRmRcmResMgr::m_astRmTddUlSfInfo[LRM_UL_DL_CONFIG_MAX] = 
   { 
      {6,{2,3,4,7,8,9}},
      {4,{2,3,7,8}},
      {2,{2,7}},
      {3,{2,3,4}},
      {2,{2,3}},
      {1,{2}},
      {5,{2,3,4,7,8}}
   };

CRrmApplication CRrmApplication::m_oRrmApplication;
CRrmApplication & g_rObjRRMApp(CRrmApplication::getInstance());

CRrmApplication::CRrmApplication() : m_rObjMMEMgr (CMMEManger::getInstance())
{
   m_ftnPtrPrcFsmMsg[EVTRMUUEADMITREQ] = &CRrmApplication::handleUeAdmitReq;
   m_ftnPtrPrcFsmMsg[EVTRMUUEHOREQ] = &CRrmApplication::handleHoReq;
   m_ftnPtrPrcFsmMsg[EVTRMUUERELREQ] = &CRrmApplication::handleUeRelReq;
   m_ftnPtrPrcFsmMsg[EVTRMUUERECFGREQ] = &CRrmApplication::handleUeRecfgReq;
   m_ftnPtrPrcFsmMsg[EVTRMUOVLDSTRTIND] = &CRrmApplication::handleMmeOverloadStart;
   m_ftnPtrPrcFsmMsg[EVTRMUOVLDSTPIND] = &CRrmApplication::handleMmeOverloadStop;
   m_ftnPtrPrcFsmMsg[EVTRMUFSMSTAIND] = &CRrmApplication::handleFsmStatusInd;

   m_ftnPtrPrcFsmMsg[EVTRMUBASEEVT] = NULLP;
   m_ftnPtrPrcFsmMsg[EVTRMUCELLRECFGIND] = NULLP;
   m_ftnPtrPrcFsmMsg[EVTRMUUEADMITRSP] = NULLP;
   m_ftnPtrPrcFsmMsg[EVTRMUUEHORSP] = NULLP;
   m_ftnPtrPrcFsmMsg[EVTRMUUERELRSP] = NULLP;
   m_ftnPtrPrcFsmMsg[EVTRMUUERELIND] = NULLP;
   m_ftnPtrPrcFsmMsg[EVTRMUUERECFGRSP] = NULLP;
   m_ftnPtrPrcFsmMsg[EVTRMUBNDREQ] = NULLP;
   m_ftnPtrPrcFsmMsg[EVTRMUBNDCFM] = NULLP;
   m_ftnPtrPrcFsmMsg[EVTRMUUESPSDISIND] = &CRrmApplication::handleUeSpsDisInd;
   m_ftnPtrPrcFsmMsg[EVTRMUUEINACTIND] = &CRrmApplication::handleUeInactInd;/*RRM_SP3*/
   m_ftnPtrPrcFsmMsg[EVTRMUUERECFGIND] = NULLP;
   

   m_ftnPtrPrcSmMsg[EVTLRMBASEEVT] = NULLP;
   m_ftnPtrPrcSmMsg[EVTLRMCFGREQ] = &CRrmApplication::handleRmProtoCfgReq;
   m_ftnPtrPrcSmMsg[EVTLRMCFGCFM] = NULLP;
   m_ftnPtrPrcSmMsg[EVTLRMCNTRLREQ] = NULLP;
   m_ftnPtrPrcSmMsg[EVTLRMCNTRLCFM] = NULLP;
   m_ftnPtrPrcSmMsg[EVTLRMSTAIND] = NULLP;
   m_ftnPtrPrcSmMsg[EVTLRMENBSTAIND] = &CRrmApplication::handleSmEnbStaInd;
   
   m_ftnPtrPrcMacMsg[EVTRGMBASEEVT   ] = NULLP;
   m_ftnPtrPrcMacMsg[EVTRGMBNDREQ    ] = NULLP;
   m_ftnPtrPrcMacMsg[EVTRGMUBNDREQ   ] = NULLP;
   m_ftnPtrPrcMacMsg[EVTRGMBNDCFM    ] = NULLP;
   m_ftnPtrPrcMacMsg[EVTRGMCFGPRBRPRT] = NULLP;
   m_ftnPtrPrcMacMsg[EVTRGMPRBRPRTIND] = &CRrmApplication::handlePrbRprtInd;
   m_ftnPtrPrcMacMsg[EVTRGMTRANSMODEIND] = &CRrmApplication::handleTransModeChangeInd;

   /* By default Maximum cells support is One */
   m_bMaxNumCellSupp = 1;
   m_pCellMgr = NULL;
   
}

CRrmApplication & CRrmApplication::getInstance()
{
	return m_oRrmApplication;
}


void CRrmApplication::handlePrbRprtInd(U8* pBuff)
{
   RgmPrbRprtInd *pstPrbRprtInd = (RgmPrbRprtInd*)pBuff;

   if(pstPrbRprtInd == NULL)
   {
         RLOG0(L_ERROR,"Pointer to PRB Ind is NULL");
      return;
   }
	m_pCellMgr = getCellCtxt(pstPrbRprtInd->bCellId);

   if(m_pCellMgr)
   {
#ifdef RRM_UNUSED_FUNC
      m_pCellMgr->m_objAcb.handlePrbUsageInd(pstPrbRprtInd->bAvgPrbUlUsage,
            pstPrbRprtInd->bAvgPrbDlUsage);
#endif
      m_pCellMgr->handlePrbRprt(pstPrbRprtInd);
   }
}

void CRrmApplication::handleTransModeChangeInd(U8* pBuff)
{
   RgmTransModeInd *pstTransModeInd = (RgmTransModeInd*)pBuff;
   if(pstTransModeInd == NULL)
   {
      printf("\n Pointer to TransMode indication is NULL\n\n");
      return;
   }
	m_pCellMgr = getCellCtxt(pstTransModeInd->bCellId);

   if(m_pCellMgr)
   {
      m_pCellMgr->procChangeTransMode(pstTransModeInd);
   } /*if m_pCellMgr*/
}

CCellManager* CRrmApplication::getCellCtxt(U8 bCellId)
{
	m_itCell =  m_tmCellList.find(bCellId);

	if( m_itCell == m_tmCellList.end())
	{
      RLOG1(L_ERROR,"cell Id [%d] with cell context not found", bCellId);
		return NULL;
	}
	return (CCellManager*)(m_itCell->second);
}

void CRrmApplication::handleUeAdmitReq(U8* pBuff)
{
	printCellCtxts();

	RmuUeAdmitReq *pstUeAdmitReq = (RmuUeAdmitReq*) pBuff;
	m_pCellMgr = getCellCtxt(pstUeAdmitReq->bCellId);

	if( m_pCellMgr )
	{
		m_pCellMgr->procUeAdmitReq(*pstUeAdmitReq);
	}
}

void CRrmApplication::handleUeRecfgReq(U8* pBuff)
{
	RmuUeRecfgReq *pstUeRecfgReq = (RmuUeRecfgReq *) pBuff;

	m_pCellMgr = getCellCtxt(pstUeRecfgReq->bCellId);

	if( m_pCellMgr )
		m_pCellMgr->procUeRecfgReq( *pstUeRecfgReq );
}

void CRrmApplication::handleUeRelReq(U8* pBuff)
{
	RmuUeRelReq *pstUeRelReq = (RmuUeRelReq *) pBuff;

	m_pCellMgr = getCellCtxt(pstUeRelReq->bCellId);

	if( m_pCellMgr )
	{
		m_pCellMgr->procUeRelReq((RmuUeRelReq *) pBuff);
	}
}

void CRrmApplication::handleHoReq(U8* pBuff)
{
	RmuUeHoReq *pstUeHoReq = (RmuUeHoReq *) pBuff;

	m_pCellMgr = getCellCtxt(pstUeHoReq->bCellId);

	if( m_pCellMgr )
	{
		m_pCellMgr->procUeHoReq(*pstUeHoReq);
	}
}

void CRrmApplication::printCellCtxts()
{

   for(m_itCell = m_tmCellList.begin(); m_itCell != m_tmCellList.end(); m_itCell++)    
   {
        RLOG1(L_DEBUG,"Cell ID:%d ", m_itCell->second->getCellID());
   }   
}

void CRrmApplication::handleMmeOverloadStart(U8* pBuff)
{
	RmuMmeOvldStartInd* pMmeOvldStartInd = (RmuMmeOvldStartInd*) pBuff;
	updateMMEOverloadStatus(m_rObjMMEMgr.handleMMEOvldStart(*pMmeOvldStartInd));
}

void CRrmApplication::handleMmeOverloadStop(U8* pBuff)
{
	RmuMmeOvldStopInd* pMmeOvldStopInd = (RmuMmeOvldStopInd*) pBuff;
	updateMMEOverloadStatus(m_rObjMMEMgr.handleMmeOverloadStop(*pMmeOvldStopInd));
}

void CRrmApplication::updateMMEOverloadStatus(RmuMmeOvldStatus enMmeOvldStatus)
{
	for (m_itCell = m_tmCellList.begin(); m_itCell != m_tmCellList.end(); m_itCell++)
	{
		m_itCell->second->m_objAcb.updateLeastMmeOverloadStatus(enMmeOvldStatus);
	}
}

void CRrmApplication::procMmeStatus(RmuMmeStatusInd & stMmeStatusInd)
{
   // MME status is received UP from FSM
   if( stMmeStatusInd.enMmeStatus == RMU_MME_STATUS_UP)
   {
  	   updateMMEOverloadStatus(m_rObjMMEMgr.upMME(stMmeStatusInd.usMmeId));
   }

   if ( stMmeStatusInd.enMmeStatus == RMU_MME_STATUS_DOWN )
   {
	   updateMMEOverloadStatus(m_rObjMMEMgr.downMME(stMmeStatusInd.usMmeId));
   }

}

void CRrmApplication::procEnodebStatus(RmuEnbStatusInd & stEnbStatusInd)
{
   m_pCellMgr = getCellCtxt(stEnbStatusInd.bCellId);
   if(m_pCellMgr)
   {
      m_pCellMgr->setEnbState(stEnbStatusInd.enEnbStatus);
   }
}

void CRrmApplication::handleFsmStatusInd(U8* pBuff)
{
   RmuFsmStatusInd* pstFsmStatusInd = (RmuFsmStatusInd *)pBuff;

   if(pstFsmStatusInd == NULL)
   {
        RLOG0(L_ERROR,"Ptr to Fsm Status Ind is NULL");
      return;
   }
   else if(pstFsmStatusInd->enStatusType == RMU_FSM_STATUS_MME)
   {
      procMmeStatus(pstFsmStatusInd->u.stMmeStatusInd);
   } 
   else if(pstFsmStatusInd->enStatusType == RMU_FSM_STATUS_ENB) 
   {
      procEnodebStatus(pstFsmStatusInd->u.stEnbStatusInd);
   }
   else
   {
        RLOG0(L_WARNING,"Invalid Status Type Received From MME");
   }
}


LrmCause CRrmApplication::handleRmProtoCfgReq(U8* pBuff)
{
	LrmCause 	enCause = LRM_CAUSE_NOT_APP;

	RmProtoCfg  *pstProtoCfg = (RmProtoCfg*)pBuff;

	switch(pstProtoCfg->bCfgType)
	{
		case LRM_CELL_CONFIG:
			enCause = handleCellCfgReq(&pstProtoCfg->u.stCellCfg);
			break;
		case LRM_ENB_CONFIG:
			enCause = handleEnbCfgReq(&pstProtoCfg->u.stEnbCfg);
			break;
		default:
			enCause = LRM_CAUSE_INVALID_CFG_TYPE;
         RLOG1(L_ERROR,"LRM protocal configuration rcvd with invalid type:[%d]",
               pstProtoCfg->bCfgType);
	}
	return 	enCause;
}

LrmCause CRrmApplication::procMmeCfg(LrmMmeCfg & stMmeCfg)
{
	LrmCause enCause = LRM_CAUSE_NOT_APP;

	switch (stMmeCfg.enMmeAddModOrDel)
	{
		case LRM_CFG_ADD:
			enCause = m_rObjMMEMgr.addMME(stMmeCfg);
			break;
		case LRM_CFG_DEL:
			enCause = m_rObjMMEMgr.deleteMME(stMmeCfg);
			break;
      default:
         RLOG1(L_ERROR,"Received MME configuration with invalid type:[%d]",
               stMmeCfg.enMmeAddModOrDel);
         enCause = LRM_CAUSE_INVALID_CFG_TYPE;
	}

	return enCause;
}

LrmCause CRrmApplication::handleEnbCfgReq(LrmEnbConfiguration* pstRmEnbCfgData)
{
	LrmCause enCause = LRM_CAUSE_NOT_APP;

	switch (pstRmEnbCfgData->bCfgType)
	{
		case RRM_ENB_CFG_SYS_CFG:
			m_bMaxNumCellSupp = pstRmEnbCfgData->u.bMaxNumCells;
			break;
		case RRM_ENB_CFG_MME_CFG:
			enCause = procMmeCfg(pstRmEnbCfgData->u.stMmeCfg);
			break;
		default :
			enCause = LRM_CAUSE_INVALID_CFG_TYPE;
         RLOG1(L_ERROR,"Received EnodeB level configuration"
               "with invalid type:[%d]", pstRmEnbCfgData->bCfgType);
			break;
	}

	return enCause;
}

LrmCause rmAppVldtRadioResCfg(LrmCellConfigData* stCellCfg)
{
   U16             usMaxNumUes = stCellCfg->bMaxActiveUesServed;
   U16             usTotalCqiRes;
   U16             usTotalSrRes;
   U16             usN2Pucch;
   U8              bNumSrPerTti = stCellCfg->usNumSrResPerTti;
   U8              bNumCqiPerTti = stCellCfg->usNumCqiResPerTti;
#ifdef LTE_TDD
   U8              bNumUlSf = CRmRcmResMgr::getNumUlSf(stCellCfg->enUlDlConfig);
#else
   U8              bNumUlSf = RM_NUM_SF_PER_SFN;
#endif
   U8              abSrPrdcty[LRM_SR_PRDCTY_MAX] = {5,10,20,40,80,2,1};
#ifdef LTE_TDD
   U8              abCqiPrdcty[LRM_CQI_PRDCTY_MAX] = {1,5,10,20,40,80,160};
#else
   U8              abCqiPrdcty[LRM_CQI_PRDCTY_MAX] = {2,5,10,20,40,80,160,32,64,128};
#endif
   S16             retVal = LRM_CAUSE_NOT_APP;
   U16             usTotalCqiRequired;

   /* Validate the SR Resources over the periodicity configured */

   bNumSrPerTti = stCellCfg->usNumSrResPerTti;
   usTotalSrRes = bNumSrPerTti * bNumUlSf * abSrPrdcty[stCellCfg->enSrPrdcty] / RM_NUM_SF_PER_SFN;
                                     
   if(stCellCfg->stSpsCellCfg.bIsSpsEnabled)
   {
#ifdef LTE_TDD   

      /* The 5ms SR periodicity is supported for UL-DL configuation 1 only */
      if(( LRM_UL_DL_CONFIG_2 <= stCellCfg->enUlDlConfig ) && 
            ( LRM_SR_PRDCTY_5 == stCellCfg->enSrPrdcty))
      {
         RLOG1(L_ERROR,"RRM Configuraiton failed : 5ms SR Periodicity"
           " Not Supported for UL-DL Config - %d",stCellCfg->enUlDlConfig);

         return LRM_CAUSE_INVALID_CFG_INFO;       
      }


      switch(stCellCfg->enUlDlConfig)
      {   
         case LRM_UL_DL_CONFIG_1:
         case LRM_UL_DL_CONFIG_2:
          break;
         default:
          {
             RLOG1(L_ERROR,"Configuraiton failed : TDD Config [%d] Not Supported",
                    stCellCfg->enUlDlConfig);
             return LRM_CAUSE_INVALID_CFG_INFO;
          }   
      }
#endif      

   }   

   if(usTotalSrRes < usMaxNumUes)
   {
		RLOG0(L_ERROR,"RRM Configuraiton failed : Not Enough SR resources are available");

		RLOG2(L_ERROR,"Total SR Configured = [%d][i.e Sr_Prdcty * Sr_Per_Tti],"
                    "Number SR Required[%d]",usTotalSrRes,usMaxNumUes);

      RLOG2(L_ERROR,"NUM_SR_PER_TTI [%d],SR_PRDCTY [%d]", 
				bNumSrPerTti,	abSrPrdcty[stCellCfg->enSrPrdcty]);

      return LRM_CAUSE_INVALID_CFG_INFO;       
   }   

   /* Validate the CQI Resources over the periodicity configured */
   /* the below 2 is indicating the two resources for CQI and RI */
    
   usTotalCqiRes = bNumCqiPerTti * bNumUlSf * abCqiPrdcty[stCellCfg->enCqiPrdcty] / RM_NUM_SF_PER_SFN;
   usN2Pucch = stCellCfg->bNRbCqi * RM_NUM_CQI_RES_PER_RB;
   usTotalCqiRequired = (usMaxNumUes * RM_RCM_NUM_N2PUCCH_PER_UE);
   
   if((usN2Pucch < bNumCqiPerTti))
   {
   	RLOG2(L_ERROR,"N2RB configured is Less[%d] to accomodate [%d] CQI_PER_TTI"
                     ,stCellCfg->bNRbCqi,bNumCqiPerTti);
      retVal = LRM_CAUSE_INVALID_CFG_INFO;
   }
   if(usTotalCqiRes < usTotalCqiRequired)
   {
      RLOG3(L_ERROR,"Total CQI Configured(Cqi_Prdcty * Cqi_Per_Tti[%d]),"
                    "Number CQI Required[%d],NUM_CQI_PER_TTI [%d]", 
		usTotalCqiRes, usTotalCqiRequired,  bNumCqiPerTti);
      retVal = LRM_CAUSE_INVALID_CFG_INFO;
   }
   if(LRM_CAUSE_NOT_APP != retVal)
   {
      RLOG0(L_ERROR,"Configuraiton failed : Not Enough CQI resources are available");
      return LRM_CAUSE_INVALID_CFG_INFO; 
   }

   return LRM_CAUSE_NOT_APP;
}

LrmCause CRrmApplication::handleCellCfgReq(LrmCellConfiguration* stCellCfgData)
{
	LrmCause	enCause = LRM_CAUSE_NOT_APP;
   CCellManager *objCellManager;

	switch (stCellCfgData->enCellAddModDel)
	{
		case LRM_PROTOCFG_ADD_CELL:
		{
         /* Validate the Radio Resources configured */
         enCause = rmAppVldtRadioResCfg(&stCellCfgData->stCellCfgInfo);
        
         if(LRM_CAUSE_NOT_APP == enCause)
         {
            if( m_tmCellList.find(stCellCfgData->stCellCfgInfo.bCellId) == 
                  m_tmCellList.end() )
            {
               m_tmCellList[stCellCfgData->stCellCfgInfo.bCellId] = 
                            new CCellManager(&stCellCfgData->stCellCfgInfo);

               if(NULLP == m_tmCellList[stCellCfgData->stCellCfgInfo.bCellId])
               {
                    RLOG0(L_ERROR,"Memory Alloc Failed");
                  enCause = LRM_CAUSE_OTHER;

               }
               objCellManager = m_tmCellList[stCellCfgData->stCellCfgInfo.bCellId];

               objCellManager->rmInitRadioRes();

               CCellManager::setGbrDrxDisFlag(stCellCfgData->stCellCfgInfo.stRmCellDrxCfg.bIsGbrDrxDis);
            }
            else
            {
                 RLOG1(L_ERROR,"cell Id [%d] already exist", 
                             stCellCfgData->stCellCfgInfo.bCellId);
               enCause = LRM_CAUSE_CELL_ALREADY_EXISTS;
            }
         }
         else
         {
              RLOG0(L_FATAL,"Cell Configuration Failed in RRM");
              RLOG0(L_ERROR,"RRM cell is not UP");
         }
			break;
		}
		case LRM_PROTOCFG_MOD_CELL:
		{
			if( m_tmCellList.find(stCellCfgData->stCellCfgInfo.bCellId) != m_tmCellList.end() )
			{
				m_tmCellList[stCellCfgData->stCellCfgInfo.bCellId]->reConfigureCell(*stCellCfgData);
			}
			else
			{
				// Send Failure status with proper enCause value as Unknown_cellID
            RLOG1(L_ERROR,"Cell modification is received for unkown cell id:%d",
                  stCellCfgData->stCellCfgInfo.bCellId);
            enCause = LRM_CAUSE_CELLID_UNKNOWN;
			}
			break;
		}
		case LRM_PROTOCFG_DEL_CELL:
		{
			if( m_tmCellList.find(stCellCfgData->stCellCfgInfo.bCellId) != m_tmCellList.end() )
			{
				m_pCellMgr = m_tmCellList[stCellCfgData->stCellCfgInfo.bCellId];
				m_tmCellList.erase(stCellCfgData->stCellCfgInfo.bCellId);
				delete m_pCellMgr;
			}
			else
			{
				// Send Failure status with proper enCause value as Unknown_cellID
            RLOG1(L_ERROR,"Cell deletation is received for unkown cell id:%d",
                  stCellCfgData->stCellCfgInfo.bCellId);
				enCause = LRM_CAUSE_CELLID_UNKNOWN;
			}
			break;
		}
		default:
		{
			// Send Failure status with proper enCause value as Unknown_action_on_cell
         RLOG2(L_ERROR,"Unknown operation[%d] on cell with cell id:%d",
                  stCellCfgData->enCellAddModDel, stCellCfgData->stCellCfgInfo.bCellId);
			enCause = LRM_CAUSE_INVALID_CFG_TYPE;
			break;
		}
	}

	return enCause;
}

LrmCause CRrmApplication::handleSmEnbStaInd (U8* pBuff)
{
   LrmCause		 enCause = LRM_CAUSE_NOT_APP;
   RmMngmt      *pstMngmt = (RmMngmt*)pBuff;
   LrmEnbStaInd *pstEnbStaInd = &(pstMngmt->t.usta.u.stEnbStaInfo);

   switch(pstEnbStaInd->enStaType)
   {
      case LRM_ENBSTATYPE_CPULOAD:
         {
            for ( m_itCell=m_tmCellList.begin() ; m_itCell != m_tmCellList.end(); m_itCell++ )
               m_itCell->second->m_objAcb.handleCpuLoadInd(pstEnbStaInd->u.bCpuLoad);
         }
         break;
      case LRM_ENBSTATYPE_CELLUP:
         // Currently the PRB report Configuration is done during the cellup.
         //going forward may triggerred from other events also
         m_pCellMgr = getCellCtxt(pstEnbStaInd->u.bCellId);

         if( m_pCellMgr )
         {
            m_pCellMgr->setEnbState(RM_ENB_STATE_UP);
            /* triggering to the MAC to report PRB usage Indication to RRM */
            if(ROK != m_pCellMgr->triggerStartPrbUsageReport())
            {
               RLOG0(L_ERROR,"Sending PRB Report configuration to SCH is failed\n\n");
            }
         }
         break;
      default:
         RLOG1(L_ERROR,"RRM received Unknown operation[%d] during "
               "Enb status indication ", pstEnbStaInd->enStaType);
         enCause = LRM_CAUSE_INVALID_CFG_TYPE;
         break;
   }
   return enCause;
}

void CRrmApplication::handleUeSpsDisInd(U8* pBuff)
{
	RmuUeSpsDisInd *pstUeSpsDisInd = (RmuUeSpsDisInd *) pBuff;

	m_pCellMgr = getCellCtxt(pstUeSpsDisInd->cellId);

	if( m_pCellMgr )
   {
		m_pCellMgr->procUeSpsDisInd( pstUeSpsDisInd );
   } 
}
/*RRM_SP3*/
void CRrmApplication::handleUeInactInd(U8* pBuff)
{
   RmuUeInactInd   *ueInactInd = (RmuUeInactInd *)pBuff;

   m_pCellMgr = getCellCtxt(ueInactInd->uiCellId);

   if( m_pCellMgr )
   {
      m_pCellMgr->procUeInactInd(*ueInactInd);
   }
}

/********************************************************************30**

           End of file:     qf_application.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:32 2013

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
