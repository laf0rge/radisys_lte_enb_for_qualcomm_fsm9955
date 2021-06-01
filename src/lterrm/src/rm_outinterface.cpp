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
               

     File:     rm_outinterface.cpp

     Sid:      qf_outinterface.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:34 2013

     Prg:     vkorde, jbm 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="RRM";
static int RLOG_MODULE_ID=128;
static int RLOG_FILE_ID=152;

/* header include files */
#include "rm_application.h"
#include "rm_outinterface.h"

// static CRmApplication  *g_pObjRrmApp = CRmApplication::getRmAppInstance();

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void rrmSendAcbParamsToFsm(RmuAcbParamInd & stAcb, U8    bCellId)
{
	RmuCellRecfgInd   *pstCellRecfgInd = (RmuCellRecfgInd*)rrmAlloc(sizeof(RmuCommonMsg));//YSM
   
  if(NULLP == pstCellRecfgInd)
  {
     RLOG0(L_FATAL, "RRM->FSM: Not able to allocate memory for CELL RECFG IND");
     return;
  }

	pstCellRecfgInd->stAcbParamInd = stAcb;
	pstCellRecfgInd->enCellRecfgType = RMU_CELL_RECFG_ACB_IND;
	pstCellRecfgInd->bCellId = bCellId;

	/* Send the Cell Reconfiguration Indication message to the FSM */
	rrmSendCellRecfgInd(pstCellRecfgInd);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void rrmSendUeAdmitRsp(RmuUeAdmitReq & stUeAdmitReq, RmuUeAdmitRsp *pstUeAdmitRsp)
{
	pstUeAdmitRsp->bCellId = stUeAdmitReq.bCellId;
	pstUeAdmitRsp->uiTransId = stUeAdmitReq.uiTransId;
	pstUeAdmitRsp->usCrnti = stUeAdmitReq.usCrnti;

   RLOG4(L_EVENT, "RRM->FSM: Sending the UE Admit Response"
             "CELLID:[%d] CRNTI:[%d] STATUS:[%d] CAUSE:[%d]",
              pstUeAdmitRsp->bCellId,
              pstUeAdmitRsp->usCrnti,
              pstUeAdmitRsp->stStatusInfo.enStatus,
              pstUeAdmitRsp->stStatusInfo.enCause );
	RmUiRmuUeAdmitRsp(&g_stRmCb.rmuSap.pst, g_stRmCb.rmuSap.suId, pstUeAdmitRsp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void rrmSendUeRecfgRsp(RmuUeRecfgReq & stUeRecfgReq, RmuUeRecfgRsp *pstUeRecfgRsp)
{

	pstUeRecfgRsp->bCellId = stUeRecfgReq.bCellId;
	pstUeRecfgRsp->uiTransId = stUeRecfgReq.uiTransId;
	pstUeRecfgRsp->usCrnti = stUeRecfgReq.usCrnti;

   RLOG4(L_EVENT, "RRM->FSM: Sending the UE RCFG Response"
             "CELLID:[%d] CRNTI:[%d] STATUS:[%d] CAUSE:[%d]",
              pstUeRecfgRsp->bCellId,
              pstUeRecfgRsp->usCrnti,
              pstUeRecfgRsp->stStatusInfo.enStatus,
              pstUeRecfgRsp->stStatusInfo.enCause );
	RmUiRmuUeRecfgRsp(&g_stRmCb.rmuSap.pst, g_stRmCb.rmuSap.suId, pstUeRecfgRsp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void rrmSendUeRelRsp(RmuUeRelReq *pstUeRelReq, RmuUeRelRsp *pstUeRelRsp)
{
	pstUeRelRsp->bCellId = pstUeRelReq->bCellId;
	pstUeRelRsp->uiTransId = pstUeRelReq->uiTransId;
	pstUeRelRsp->usCrnti = pstUeRelReq->usCrnti;

   RLOG4(L_EVENT, "RRM->FSM: Sending the UE Release Response"
             "CELLID:[%d] CRNTI:[%d] STATUS:[%d] CAUSE:[%d]",
              pstUeRelRsp->bCellId,
              pstUeRelRsp->usCrnti,
              pstUeRelRsp->stStatusInfo.enStatus,
              pstUeRelRsp->stStatusInfo.enCause );
	RmUiRmuUeRelRsp(&g_stRmCb.rmuSap.pst, g_stRmCb.rmuSap.suId, pstUeRelRsp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void rrmSendUeHoRsp(RmuUeHoReq & stUeHoReq, RmuUeHoRsp *pstUeHoRsp)
{
	pstUeHoRsp->bCellId = stUeHoReq.bCellId;
	pstUeHoRsp->uiTransId = stUeHoReq.uiTransId;
	pstUeHoRsp->usCrnti = stUeHoReq.usCrnti;

   RLOG4(L_EVENT, "RRM->FSM: Sending the UE HO Response"
             "CELLID:[%d] CRNTI:[%d] STATUS:[%d] CAUSE:[%d]",
         pstUeHoRsp->bCellId,
         pstUeHoRsp->usCrnti,
         pstUeHoRsp->stStatusInfo.enStatus,
         pstUeHoRsp->stStatusInfo.enCause );
	RmUiRmuUeHoRsp(&g_stRmCb.rmuSap.pst, g_stRmCb.rmuSap.suId, pstUeHoRsp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void rrmSendCellCfgPrbReport(RgmPrbRprtCfg   *pstPrbRprt)
{
   RLOG2(L_EVENT, "RRM->SCH: Send cell configuration for PRB report "
         "[CELL ID:PRB report start/stop]-[%d:%d]",
         pstPrbRprt->bCellId,
         pstPrbRprt->bConfigType);
	RmLiRgmCfgPrbRprt(&g_stRmCb.rgmSap.pst, g_stRmCb.rgmSap.suId, pstPrbRprt);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void rrmSendCellRecfgInd(RmuCellRecfgInd *pstCellRecfgInd)
{
   RLOG2(L_EVENT, "RRM->FSM: [CELL ID:RECFG TYPE]-[%d:%d] CELL RECFG Indication sent",
         pstCellRecfgInd->bCellId,
         pstCellRecfgInd->enCellRecfgType);
	RmUiRmuCellRecfgInd(&g_stRmCb.rmuSap.pst, g_stRmCb.rmuSap.suId, pstCellRecfgInd);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void rrmSendUeRelInd(RmuUeRelInd *pstUeRelInd)
{
   RLOG2(L_EVENT, "RRM->FSM: CELL:UE [%d:%d] UE Release Indication sent",
         pstUeRelInd->bCellId,
         pstUeRelInd->usCrnti);
	RmUiRmuUeRelInd(&g_stRmCb.rmuSap.pst, g_stRmCb.rmuSap.suId, pstUeRelInd);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void rrmSendUeRelInd(U8 bCellId, U16 usCrnti, RmuRrcCause enRelCause)
{
   RmuUeRelInd* pstUeRelInd = (RmuUeRelInd*)rrmAlloc(sizeof(RmuCommonMsg));//YSM
  
  if(NULLP == pstUeRelInd)
  {
     RLOG0(L_FATAL, "RRM->FSM: Not able to allocate memory for UE REL IND ");
     return;
  }

   pstUeRelInd->enRelCause  = enRelCause;
   pstUeRelInd->usCrnti	    = usCrnti;
   pstUeRelInd->bCellId 	 = bCellId;

   RLOG2(L_EVENT, "RRM->FSM: CELL:UE [%d:%d] UE Release Indication sent",
         pstUeRelInd->bCellId,
         pstUeRelInd->usCrnti);
   /* Send the UE Release Indication message to the FSM */
   rrmSendUeRelInd(pstUeRelInd);
}

void rrmSendUeRecfgInd(RmuUeRecfgInd *pstUeRecfgInd)
{
   RmUiRmuUeRecfgInd(&g_stRmCb.rmuSap.pst, g_stRmCb.rmuSap.suId, pstUeRecfgInd);
}

void rrmSendUeERabRelInd( RmuUeERabRelInd *pstUeERabRelInd)
{
   RLOG3(L_EVENT,
         "RRM->FSM: CELL:UE:numberOfRABs [%d:%d:%d] ERAB Release Indication sent",
         pstUeERabRelInd->bCellId,
         pstUeERabRelInd->usCrnti,
         pstUeERabRelInd->stErabRelInfo.bNoErabsToRel);
   
   RLOG0(L_DEBUG, "RRM->FSM: List of RABS for Release are:");
   for (U32 indx = 0; indx < pstUeERabRelInd->stErabRelInfo.bNoErabsToRel; indx++)
   {
      RLOG2(L_DEBUG, "RAB ID [%d], CAUSE:[%d]", 
            pstUeERabRelInd->stErabRelInfo.stErabRelLst[indx].bErabId,
            pstUeERabRelInd->stErabRelInfo.stErabRelLst[indx].enErabRelCause);
   }

   RmUiRmuUeERabRelInd(&g_stRmCb.rmuSap.pst, g_stRmCb.rmuSap.suId,
   pstUeERabRelInd);
}
/********************************************************************30**

           End of file:     qf_outinterface.cpp@@/main/Br_Tenb_RTLinux_Intg/Br_Tenb_Mspd_T2K_Intg/1 - Wed Apr  3 15:41:34 2013

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
