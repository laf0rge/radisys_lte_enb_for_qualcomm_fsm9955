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
               

     File:     wr_umm_rrccon.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=67;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_emm.h"
#include "wr_cmn.h"
#include "wr_umm.h"
#include "wr_umm_trans.h"
#include "wr_smm_smallcell.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_l1_msg.h"
#include "wr_umm_sch_msg.h"
#include "wr_umm_s1ap_msg.h"
#include "wr_ifm_rrc.h"
#include "wr_ifm_schd.h"
#include "wr_ifm_s1ap.h"
#include "wr_ifm_l1.h"
#include "wr_kpi.h"
#include "wr_umm_drx.h"
#include "wr_ifm_rrm.h"
#include "wr_ifm_son.h"
#include "wr_emm_mme.h"

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

#define WR_UMM_WAIT_TIME_VALUE   3
/*Forward Declration of function prototypes*/
PRIVATE S16 wrUmmRrcSetupHdlr(WrUmmTransCb  *transCb);
PRIVATE S16 wrUmmRrcConSetupCmpHdl(WrUmmTransCb  *transCb);
PRIVATE S16 wrUmmRrcSetupPrcPhyCfgCfm (WrUmmTransCb  *transCb);
PRIVATE S16 wrUmmRrcSetupPrcSchCfgCfm(WrUmmTransCb  *transCb);
PRIVATE S16 wrUmmRrcSetupPrcRrcCfgCfm(WrUmmTransCb  *transCb);
PRIVATE S16 wrUmmRrcSetupPrcRrcDatInd(WrUmmTransCb  *transCb);
PRIVATE S16 wrUmmRrcSetupInitUeCb (WrUeCb *ueCb);
PRIVATE S16 wrSndRrcConRej(WrUmmTransCb  *transCb, U8 internalCause);
PUBLIC S16 wrEmmGetLdBalMme( WrRrcConTransCb *rrccon);
EXTERN S16 wrEmmGetMmeForPlmnEstCause
(
U32                          establishCause,
WrRrcConTransCb              *rrccon
);
EXTERN S16 wrEmmGetTai
(
WrUeCb *ueCb,
WrTai  *tai
);
#ifdef MME_LDBAL
PUBLIC S16 wrEmmSelectMmeOnGummei(WrMmeId *mmeId, WrGummei *gummei,
                                        U32 establishmentCause);
#endif
#ifdef WR_TEST_CODE
EXTERN U8 wrTrgRrcConTO;
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @brief This function initilizes the parameters related to
 *         admitted UE.
 *
 * @details
 *
 *     Function:
 *
 *         Processing steps:
 *         - Initilizes the RRC transaction.
 *         - Initlizes all the parameters
 *            of admitted UE.
 *
 *
 * @param : WrUeCb *ueCb
 * @param
 * @return :
 *          ROK     - Status SUCCESS
 *          RFAILED - Status FAIL
 */
PRIVATE S16 wrUmmRrcSetupInitUeCb
(
WrUeCb                       *ueCb
)
{
   wrUmmInitRrcTrans(ueCb);
   ueCb->s1ConCb = NULLP;
   ueCb->ueCat                             = CM_LTE_UE_CAT_1;

   /* Reset the Radio resource configuraiotn bitmap */
   ueCb->bResCfgBitMap                     = 0;
   wrUmmInitUeCb(ueCb);

   /* Filling BSR timer values */
   ueCb->uePerBsrTmrCfg.prdBsrTmr          = wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.periodicBSR_Timer;
   ueCb->uePerBsrTmrCfg.retxBsrTmr         = wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.retxBSR_Timer;
   RETVALUE(ROK);
}/*wrUmmRrcSetupInitUeCb*/

/** @brief This function performs UE Configuration
 *         towards Physical Layer.
 *
 * @details
 *     Function: wrUmmRrcConPhyUeCfg
 *
 *         Processing steps:
 *
 *         - Fills UE configuration parameters for PHY layer.
 *         - Sends the Config parameters to interface module
 *           of Phyisical layer.
 *
 *
 * @param WrUmmTransCb  *transCb
 *
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcConPhyUeCfg
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   CtfCfgReqInfo             *cfgInfo;

   transCb->u.rrcContranscb.phyCfg = WR_UMM_CFG_ERRO;

   WR_ALLOC(&cfgInfo, sizeof(CtfCfgReqInfo));
   if (cfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   if (wrUmmPhyFillUeCfg(transCb->transId, ueCb, cfgInfo) != ROK)
   {
      RLOG0(L_ERROR, "Req preperation failed.");
      RETVALUE(ROK);
   }
   if (wrIfmPhyUeCfg(cfgInfo, transCb->transId) != ROK)
   {
      RLOG0(L_ERROR, "WrSndCtfUeCfg failed.");
      RETVALUE(RFAILED);
   }
   transCb->u.rrcContranscb.phyCfg = WR_UMM_CFG_SENT;
   RETVALUE(ROK);
}
/** @brief This function performs UE Configuration
 *         towards MAC Scheduler.
 *
 * @details
 *     Function: wrUmmRrcConSchUeCfg
 *
 *         Processing steps:
 *         - Fills the UE configuration paramters of MAC Scheduler.
 *         - Sends the UE config paramerters to interface module of
 *           MAC Scheduler.
 *
 *
 * @param WrUmmTransCb  *transCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcConSchUeCfg
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   RgrCfgReqInfo             *cfgInfo;
   U32                       transId;

   transCb->u.rrcContranscb.schUeCfg = WR_UMM_CFG_ERRO;

   WR_ALLOC(&cfgInfo, sizeof(RgrCfgReqInfo));
   if(cfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);/*Klockwork P1 warning fix*/
   }
   if (wrUmmSchFillUeCfg(transCb->transId, ueCb, cfgInfo) != ROK)
   {
      RLOG0(L_ERROR, "Req preperation failed.");
      RETVALUE(ROK);
   }

   transId = transCb->transId | WR_UMM_RRC_CON_TRANS_UE;
   cfgInfo->u.cfgInfo.u.ueCfg.csgMmbrSta = FALSE;
   if (WrIfmSchdCfgUe(cfgInfo, transId) != ROK)
   {
      RLOG0(L_ERROR, "WrIfmSchdCfgUe failed.");
      RETVALUE(RFAILED);
   }

   transCb->u.rrcContranscb.schUeCfg = WR_UMM_CFG_SENT;

   RETVALUE(ROK);
}
/** @brief This function performs building RRC CONNECTION
 *         SETUP Msg and sending towards UE.
 *
 *
 * @details
 *     Function: wrUmmRrcConSndConSetup
 *
 *         Processing steps:
 *         - Builds the RRC PDU RRC CONNECTION
 *           SETUP MSG.
 *         - Fills the SRB1 Logical channel configuration.
 *         - Sends the message torwards UE in response to
 *           MSG3 through RRC IFM module.
 *
 *
 * @param WrUmmTransCb  *transCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcConSndConSetup
(
WrUmmTransCb                 *transCb
)
{
   NhuDatRspSdus             *conSetupSdu = NULLP;
   NhuUeCfgInfo              *ueCfg = NULLP;
   WrUeCb                    *ueCb = transCb->ueCb;

   /* Build RRC Connection Setup Message */
   WR_ALLOCEVNT(&conSetupSdu, sizeof(NhuDatRspSdus));
   if(NULLP == conSetupSdu)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);/* Klockwork P1 warning fix */
   }
   conSetupSdu->sdu.m.dlCcchMsg.rbId = WR_CCCH_DL_ID;
   conSetupSdu->sdu.m.dlCcchMsg.tmpCrnti = ueCb->crnti;
   wrUmmFillRrcEvntHdr(&(conSetupSdu->hdr), ueCb->cellId, 0, transCb->transId);

   if (wrUmmRrcFillConSetup(conSetupSdu, ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmRrcFillConSetup failed");
      WR_FREEEVNT(conSetupSdu);
      RETVALUE(RFAILED);
   }

   /*-- Fill the UE configuration information --*/
   conSetupSdu->sdu.isUeCfgPres = TRUE;
   ueCfg = &(conSetupSdu->sdu.ueCfg);
   if (wrUmmRrcAddSrb1Cfg(conSetupSdu, ueCfg, ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmFillNhuUeCfgInfo Failed");
      WR_FREEEVNT(conSetupSdu);
      RETVALUE(RFAILED);
   }
   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending RRCConnectionSetup");
   /* Send RRC Connection Setup through NhuDatRsp Primitive*/
   if (RFAILED == wrIfmRrcSndCcchRspToUe(conSetupSdu))
   {
      RLOG0(L_ERROR, "wrIfmRrcSndCcchRspToUe failed");
      RETVALUE(RFAILED);
   }
   /*Start RRC Connection Setup Timer*/
#if 0
#else
   wrStartTransTmr(transCb,WR_TMR_RRC_RRCCON, wrEmmGetRrcConTmrVal (ueCb->cellId));
#endif
   RETVALUE(ROK);
}
/** @brief This function is called to handle RRC CON SETUP
 *         message received from UE.
 *
 * @details
 *     Function: wrUmmRrcSetupHdlr        
 *
 *         Processing steps:
 *         - Parses the RRC message received from UE and
 *           updates the ueCb with necessary parameters
 *           like S-TMSI etc .
 *         - Performs configuration towards physical layer.
 *         - Performs configuration towards MAC Scheduler.
 *
 *
 * @param WrUmmTransCb  *transCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcSetupHdlr
(
WrUmmTransCb                 *transCb
)
{
   NhuInitUE_Identity        *ueID = NULLP;
   WrUeCb                    *ueCb;
   U16                       cnt = 0;
   NhuDatIndSdus             *datIndSdu = NULLP;
   NhuUL_CCCH_Msg            *ccchMsg;
   NhuRRCConRqst             *rrcConRqst;
   NhuRRCConRqst_r8_IEs      *r8_IEs;
   U32                        ret = RFAILED;

   datIndSdu          = transCb->msg->u.uuIndPdu;
   ueCb               = transCb->ueCb;
   ueCb->cellId       = datIndSdu->hdr.cellId;
   ueCb->crnti        = datIndSdu->sdu.m.ulCcchMsg.tmpCrnti;
   ueCb->rrcConnState = WR_UMM_RRC_CONNECTING;
   /* ccpu00130088 */
   ueCb->ueAttachedtoMme = FALSE;

   /* Extract contention resolution id */
   ccchMsg = &(datIndSdu->sdu.m.ulCcchMsg.ccchMsg);
   rrcConRqst = &(ccchMsg->message.val.c1.val.rrcConRqst);
   /*Update the Establishment Cause*/
   r8_IEs = &(rrcConRqst->criticalExtns.val.rrcConRqst_r8);
   ueCb->establishCause = (r8_IEs->establishmentCause.val);
 #ifdef WR_RSYS_KPI 
   SGetEpcTime(&ueCb->rrcConSetupReqTm);
 #endif/*WR_RSYS_KPI*/
   /*Update Statistics for incoming RRC Connection Attempt*/
   wrEmmUpdateStat(WR_EMM_RRC_STATS,transCb->ueCb->cellId,WR_EMM_RRC_CON_ATTEMPT,1,ueCb->establishCause);

  RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti,"RRC Connection State = CONNECTING"
     " Cause = RRC setup Handler");
      
  /*Increment KPI for Attempted RRC Connection Establishments*/
  WR_INC_KPI_FUNC1(wrIncKpiFrRrcConAttmptd,ueCb->establishCause);
  
   /*Initilize the RRM Configuration Parameters*/
   /*[Modification:onsite]Removed if Check as function always return TRUE. No failure Leg exist.*/
   wrUmmRrcSetupInitUeCb(ueCb);

   ueID = &(rrcConRqst->criticalExtns.val.rrcConRqst_r8.ue_Identity);
   if (ueID->choice.val == INITUE_IDENTITY_S_TMSI)
   {
      if (ueID->val.s_TMSI.pres.pres)
      {   
         ueCb->sTMSI.pres  = TRUE;
         ueCb->sTMSI.mmec  = ueID->val.s_TMSI.mmec.val[0];
         ueCb->sTMSI.mTMSI = 0;
         for (cnt = 0; cnt < ueID->val.s_TMSI.m_TMSI.len/8; cnt++)
         {
            ueCb->sTMSI.mTMSI <<= 8;
            ueCb->sTMSI.mTMSI |= (U8)(ueID->val.s_TMSI.m_TMSI.val[cnt]);
         }
      }
   }
   if( wrEmmcheckActiveMmme() == RFAILED)
   {
      /* Increment the RRC failure due to S1AP counter */
      WR_INC_KPI_FUNC(wrIncKpiFrRrcS1apFailCause);
      /*Trigger RRC Connection Reject Message --*/
      ret = wrSndRrcConRej(transCb, WR_UMM_RRC_REJ_DUE_TO_UNSPECIFIC);
      if (ret != ROK)
      {
         RLOG0(L_ERROR, "wrSndRrcConRej failed.");
      }
      transCb->state = WR_UMM_TRANS_DELETE_UE; 
      RETVALUE(ret);
   }

   /* sending UE admit request to RRM */
   ret = wrIfmRrmUeAdmitReq(ueCb->cellId, ueCb->crnti, transCb->transId, ueCb->establishCause);


   RETVALUE(ret);
}

/** @brief This function is called to select MME either based on
 *         GUMMEI Info received from UE or based on primary PLMN.
 *
 * @details
 *     Function: wrUmmRrcConSelectMme   
 *
 *         Processing steps:
 *         - Selection of MME based on GUMMEI info received from
 *           UE.
 *         - Selection of MME based on primary PLMN Index and primary
 *           PLMN.
 *
 *
 * @param WrUmmTransCb  *transCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcConSelectMme
(
WrUmmTransCb                 *transCb
)
{
   WrRrcConTransCb           *rrccon;
   /*ccpu00128503*/
   WrMmeCb                   *mmeCb;
   U16                        idx1;
   S16                        ret = RFAILED;
#ifndef MME_LDBAL
   U32                        isGummeiOvld;
#endif


   rrccon = &transCb->u.rrcContranscb;
   /* Store the PLMN by using Selected PLMN index and store in rrccon->plmnId
    * enodeb uses rrccon->plmnId to populate TAI IE in Initial UE Message
    */
   /* Arrive at the MME based on PLMN indicated by selPlmnIdx          */
   /* Fix for CR ccpu00135294 */
   if (wrEmmCellGetSelPlmn(transCb->ueCb->cellId, rrccon->selPlmnIdx,
               &rrccon->plmnId) != ROK)
      {
      RLOG0(L_ERROR, "PLMN not found for received Selected PLMN Index");
         RETVALUE(RFAILED);
      }
   if (TRUE == transCb->ueCb->sTMSI.pres)
   {
      ret = wrEmmGetMmeFrmStmsi(transCb->ueCb->sTMSI.mmec,
            &transCb->ueCb->causeTauRequiredSet,transCb->ueCb->establishCause,
            rrccon);
      RETVALUE(ret);
   }
   
   /* For KPI IRAT.MOBILITY.LAI wrIncIratMobLai*/
   if(PRSNT_NODEF == rrccon->gummei.pres)
   {
      transCb->ueCb->mobLaiFlag = TRUE;
      transCb->ueCb->lac = rrccon->gummei.mmeGrpId;
   }

      /*ccpu00125227: Check for plmn-identity presence */
      if ((rrccon->gummei.pres == FALSE) || 
            ((TRUE == rrccon->gummei.pres) && (TRUE == rrccon->gummei.plmn_identity_pres)))
      {
         if ( wrEmmGetLdBalMme(rrccon) != ROK)
         {
            if (wrEmmGetMmeForPlmnEstCause(transCb->ueCb->establishCause, rrccon) != ROK)
            {
               RLOG1(L_ERROR, "MME Selection Failed for selected PLMN index[%d]",
                     rrccon->selPlmnIdx);
               transCb->ueCb->causeTauRequiredSet = TRUE;
               RETVALUE(RFAILED);
            }
         }
         /* CSG_DEV */
         /* Update MME Group ID and MME Code for case Registered MME IE is not
          * present in RRC Connection Setup Complete */
         /*ccpu00128503*/
         if(rrccon->gummei.pres == FALSE)
         {
            mmeCb = wrEmmCb.mmeCont.mmes[rrccon->mmeId];
            if(mmeCb != NULL)
            { 
               for(idx1 = 0; idx1 < mmeCb->numGrpIds; idx1++)
               {
                  if(mmeCb->groupIds[idx1] != 0)
                  {
                     rrccon->gummei.mmeGrpId = mmeCb->groupIds[idx1];
                     break;
                  }
               }
               for(idx1 = 0; idx1 < mmeCb->numCodes; idx1++)
               {
                  if(mmeCb->codes[idx1] != 0)
                  {
                     rrccon->gummei.mmec = mmeCb->codes[idx1];
                     break;
                  }
               }
            }
            else
            {
               RLOG1(L_ERROR, "MmeCb not found for mmeId [%d]", rrccon->mmeId);
            }
         }
         /*ccpu00128503*/
         /* CSG_DEV end */
      }
      /*When PLMN-Identity not present in Registered MME IE */
      else if((rrccon->gummei.pres == TRUE) && 
            (rrccon->gummei.plmn_identity_pres == FALSE))
      {
         /*UE is in same network, MME selection will be done on the basis of
          * MME Code recieved in RRC Conection Setup Complete Message */
      RLOG1(L_INFO, "plmn_identity is NOT PRESENT in Registered MME IE, "
         "Received MMEC[%d]", rrccon->gummei.mmec);
         WrCellCb                  *cellCb;

         WR_GET_CELLCB(cellCb,transCb->ueCb->cellId);
         if (cellCb == NULLP)
         {
            RLOG1(L_ERROR, " cellCb not found for cellId(%d)"
                  ,transCb->ueCb->cellId);
            RETVALUE(RFAILED);
         }
         
         cmMemcpy((U8 *) &rrccon->gummei.plmnId,(U8*) rrccon->plmnId, sizeof(WrPlmnId));   
#ifdef MME_LDBAL
         {
            ret =  wrEmmSelectMmeOnGummei((&rrccon->mmeId), (&rrccon->gummei),
                                          transCb->ueCb->establishCause);
            if(RFAILED == ret)
            { 
               RLOG_ARG3(L_WARNING, DBG_CRNTI, transCb->ueCb->crnti,"wrUmmRrcConSelectMme: GUMMEEI CB" 
            "not found for received Selected PLMN Index(%d) mmeGpId(%x) mmeCode(%x) \n",
            rrccon->selPlmnIdx, rrccon->gummei.mmeGrpId, rrccon->gummei.mmec);
               transCb->ueCb->causeTauRequiredSet = TRUE;
               RETVALUE(RFAILED);
            }
         }
#else      

         if(wrEmmPlmnGetMme(rrccon->plmnId,rrccon->gummei.mmeGrpId, rrccon->gummei.mmec, &rrccon->mmeId) ==ROK)
         {
            /* Check if the given GUMMEI is overloaded. If so, will be sending
             * release for the attach request if establish cause is overloaded
             * for a selected MME
             */
            isGummeiOvld = wrEmmGetGummeiOvldStatus((&rrccon->gummei), 
                                                     rrccon->mmeId);
            if((TRUE == wrEmmGetOvldStatus(rrccon->mmeId)) || 
               (TRUE == isGummeiOvld))
            {   
               if(wrGetMmeRejectStatus(transCb->ueCb->establishCause, rrccon->mmeId))
               {
                 RLOG0(L_ERROR, "Selected MME is Overloaded, Searching for "
                    "other MME in MME Pool");

               }
               else
               {
                 RLOG_ARG0(L_DEBUG, DBG_MMEID, rrccon->mmeId,"Selected MME");
                  RETVALUE(ROK);

               }
            }
            else
            {  
             RLOG_ARG0(L_DEBUG, DBG_MMEID, rrccon->mmeId,"Selected MME");
               RETVALUE(ROK);
            }
         }
         if(wrEmmMmeGetMmeByMmeGrpId(transCb->ueCb->establishCause,rrccon->gummei.mmeGrpId, &rrccon->mmeId) !=ROK)
         {
            if (wrEmmGetMmeForPlmnEstCause(transCb->ueCb->establishCause, rrccon) != ROK)
            {
                   RLOG1(L_ERROR, "MME Selection Failed for PLMN index[%d] "
                      "attach proc failed", rrccon->selPlmnIdx);
               RETVALUE(RFAILED);
            }
         }

#endif      
   }
   else
   {
      /* Control should not reach here */
      RLOG0(L_ERROR, "RRC Connection Setup Complete Message has Invalid IE");
   }

   RLOG_ARG0(L_DEBUG, DBG_MMEID, rrccon->mmeId,"Selected MME");
   RETVALUE(ROK);
}
/** @brief This function performs building and sending of
 *         first S1Ap message InitUe to the core network.
 * @details
 *     Function: wrUmmRrcConS1InitUe   
 *
 *         Processing steps:
 *         - Builds the Initial UE S1Ap Message.
 *         - Allocates SUCONID for this ue associated
 *           connection.
 *         - Delivers the PDU towards S1AP stack layer
 *           for encoding through S1AP Ifm module.
 *         - Adding of this UE [ueCb] to the seleceted MME.
 *
 *
 * @param  WrUmmTransCb  *transCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcConS1InitUe
(
WrUmmTransCb                 *transCb
)
{
   WrRrcConTransCb           *rrccon;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrS1ConCb                 *s1apConCb;
   U32                       suConId;
   SztConReq                 datEvt;
   WrMmeCb                   *mmeCb = NULLP;
   WrTai                     tai;

   rrccon = &transCb->u.rrcContranscb;
   WR_GET_S1AP_CON_ID(suConId, ueCb);
   RLOG1(L_DEBUG, "suConId[%lu]",suConId);
   WR_ALLOC(&s1apConCb, sizeof(WrS1ConCb));
   if (s1apConCb == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   s1apConCb->suConnId       = suConId;
   s1apConCb->s1apConnState  = WR_S1AP_CONNECTING;
   s1apConCb->enb_ue_s1ap_id = suConId;
   ueCb->s1ConCb             = s1apConCb;
#ifdef WR_RSYS_KPI
   ueCb->s1LogicalConnEst    = FALSE; /*Added for KPI*/
#endif/*WR_RSYs_KPI*/

   WR_SET_ZERO(&datEvt, sizeof(SztConReq));

   wrEmmGetTai(ueCb,&tai);
   rrccon->tac = tai.tac;

   /* Build Init Ue Msg */
   wrUmmS1apBldInitUePdu(transCb->ueCb, rrccon->nasPdu, rrccon->plmnId,
                             rrccon->tac, &rrccon->gummei, &rrccon->gummeiType, &datEvt.pdu);

   /* Trigger SZT Connectn. Req.TODO - get peerId here */
   datEvt.peerId.pres = PRSNT_NODEF;
   mmeCb = wrEmmCb.mmeCont.mmes[rrccon->mmeId];
   datEvt.peerId.val  = mmeCb->peerId;
   datEvt.u.suConnId  = suConId;
   RLOG2(L_DEBUG,"PeerId[%lu]\tsuConId[%lu]",datEvt.peerId.val,datEvt.u.suConnId);
   if (WrIfmS1apConReq(&datEvt) != ROK)
   {
      RLOG0(L_ERROR, "Sending INITAL UE MESSAGE Failed");
      RETVALUE(RFAILED);
   }
   
   RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[S1AP] Sending INITIAL UE MESSAGE");

   wrUmmAddUeToMme(ueCb, rrccon->mmeId);
   wrEmmUpdateStat(WR_EMM_SIG_STATS,transCb->ueCb->cellId,WR_EMM_SIG_ATTEMPT,1,WR_EMM_SIG_SUBTYP_MAX);
   
   /*Increment KPI for UE-associated logical S1-connection establishmentSuccesful UE-associated logical S1-connection establishment from eNB to MME*/
   WR_INC_KPI_FUNC(wrIncKpiFrS1SIGConnEstabAttmptd);
   
   /*Starting the timer for clearing the Context if the MME not sending
   initial context Setup request message*/
   wrStartUeTmr(ueCb,WR_TMR_INITIAL_CTX_REQ, 
                                              WR_TMR_INITIAL_CTX_REQ_TMR_VAL);
   RETVALUE(ROK);
}

/** @brief  This function copies PLMN ID from NHU to wr.
 *         
 * @details
 *     Function: wrUmmCpyNhuPlmnToLwr
 *
 *
 * @param  WrUmmTransCb  *transCb
 * @return ROK/RFAILED
 */
PRIVATE Void wrUmmCpyNhuPlmnToLwr
(
  WrPlmnId                  *enbPlmn, 
  NhuPLMN_Identity           *plmnId
)
{
   U8                         idCnt;
   TRC2(wrUmmCpyNhuPlmnToLwr)

   for(idCnt = 0;idCnt < plmnId->mcc.noComp.val; idCnt++)
   {
     enbPlmn->mcc[idCnt] = plmnId->mcc.member[idCnt].val;
   }
   enbPlmn->numMncDigits = plmnId->mnc.noComp.val;
   for(idCnt = 0;idCnt < plmnId->mnc.noComp.val; idCnt++)
   {
     enbPlmn->mnc[idCnt] = plmnId->mnc.member[idCnt].val;
   }
   
}

/** @brief This function is called to handle RRC
 *         Setup Comlpete message received from UE.
 *
 * @details
 *     Function: wrUmmRrcConSetupCmpHdl   
 *
 *         Processing steps:
 *         - Processing the RRC Setup Complete message received
 *           from UE.
 *         - Selection of MME to send the NAS PDU.
 *         - Building of S1AP InitUE message and delivering it to
 *           selected MME through S1AP stack.
 *
 *
 * @param WrUmmTransCb *transCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcConSetupCmpHdl
(
WrUmmTransCb                  *transCb
)
{
   WrUeCb                     *ueCb;
   NhuDatIndSdus              *datIndSdu = NULLP;
   NhuRegisteredMME           *regMME = NULLP;
   NhuUL_DCCH_Msg             *dcchMsg;
   NhuRRCConSetupCompl        *setupComp;
   NhuRRCConSetupCompl_r8_IEs *r8_IEs;
   WrRrcConTransCb            *rrccon;
   NhuRRCConSetupCompl_v1020_IEs *r10_IEs;

   ueCb      = transCb->ueCb;
   rrccon    = &transCb->u.rrcContranscb;
   datIndSdu = transCb->msg->u.uuIndPdu;


   
   if(ueCb->rrcConnState == WR_UMM_RRC_CONNECTED)
   {
      RLOG0(L_ERROR, "ENODEB APP: recived multiple RRC Conn Setup Complete "
         "Message");
      RETVALUE(RFAILED);
   }
    ueCb->rrcConnState = WR_UMM_RRC_CONNECTED;
    /*Stop RRC Connection Setup Timer*/
    wrStopTransTmr(transCb,WR_TMR_RRC_RRCCON);

   if(ueCb->redirectUe == TRUE)
   {
      RLOG0(L_WARNING, "Maximum UE Limit reached, redirecting UE");
      transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
      RETVALUE(ROK);
   }

   /*Added for KPI*/
 #ifdef WR_RSYS_KPI 
  SGetEpcTime(&ueCb->rrcConSetupCmplTm);
 #endif /*WR_RSYS_KPI*/
  WR_INC_KPI_FUNC3(wrUpdateRrcConnEstabTimeMeanMax,ueCb->establishCause,ueCb->rrcConSetupReqTm,
  ueCb->rrcConSetupCmplTm);

   RLOG_ARG0(L_ALWAYS, DBG_CRNTI, ueCb->crnti,"RRC Connection State = RRC_CONNECTED");


   /* Check if GUMMEI is present in the message*/
   dcchMsg   = &datIndSdu->sdu.m.ulDcchMsg.dcchMsg;
   setupComp = &dcchMsg->message.val.c1.val.rrcConSetupCompl;
   r8_IEs    = &setupComp->criticalExtns.val.c1.val.rrcConSetupCompl_r8;

   rrccon->selPlmnIdx = r8_IEs->selectedPLMN_Identity.val;
   RLOG1(L_DEBUG, " selPlmnIndexReceived(%d) recieved in Setup Complete ", rrccon->selPlmnIdx);
   rrccon->nasPdu     = &r8_IEs->dedicatedInfoNAS;
   regMME             = &r8_IEs->registeredMME;
   
   if (regMME->pres.pres == PRSNT_NODEF)
   {
      RLOG0(L_DEBUG, "Registered MME IE recieved in Setup Complete");
      rrccon->gummei.pres = TRUE;
      /*ccpu00125227: Checking for plmn-identity presence and updating the 
        corresponding flag in RRC Connection Trans CB */
      if(regMME->plmn_Identity.pres.pres == PRSNT_NODEF)
      {
         /*PLMN Identity is present, Use Selected PLMN
          *Index to get the MME*/
         wrUmmCpyNhuPlmnToLwr(&(rrccon->gummei.plmnId),&(regMME->plmn_Identity));
         rrccon->gummei.plmn_identity_pres = TRUE;
      }
      else
      {
         /*PLMN Identity is not present,use MME Code
          *Index to get the MME*/
         rrccon->gummei.plmn_identity_pres = FALSE;
      }
      WR_MEM_COPY(&rrccon->gummei.mmec, regMME->mmec.val, regMME->mmec.len/8);
      
     rrccon->gummei.mmeGrpId = regMME->mmegi.val[1];
     rrccon->gummei.mmeGrpId |= (regMME->mmegi.val[0] << 8);

      RLOG2(L_DEBUG, "From RegMMEIE,Received MME CODE[%d] MME Group Id[%d]",
         rrccon->gummei.mmec,rrccon->gummei.mmeGrpId);
   }
   else
   {
      rrccon->gummei.pres = FALSE;
      RLOG0(L_DEBUG, "Registered MME IE NOT Present in Setup Complete");
   }

   /* Check for R10 IEs */
   if(r8_IEs->nonCriticalExtn.pres.pres != NOTPRSNT)
   {
      r10_IEs = &r8_IEs->nonCriticalExtn.nonCriticalExtn;
      if(r10_IEs->pres.pres != NOTPRSNT)
      {
         /* Fill Gummei type */
         if(r10_IEs->gummei_Typ_r10.pres != NOTPRSNT)
         {
            wrFillTknU32(&(rrccon->gummeiType),
                  r10_IEs->gummei_Typ_r10.val);
         }
      }
   }
   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Received RRCConnectionSetupComplete");
   
   if (wrUmmRrcConSelectMme(transCb) != ROK)
   {
      /* No MME to serve the UE. Release the connection and go away */
      transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
      wrEmmUpdateStat(WR_EMM_RRC_STATS,transCb->ueCb->cellId,WR_EMM_RRC_CON_FAIL,1,transCb->ueCb->establishCause);
      /* Increment the RRC failure due to S1AP counter */
      WR_INC_KPI_FUNC(wrIncKpiFrRrcS1apFailCause);
      RETVALUE(ROK);
   }
   ueCb->plmnId.numMncDigits = rrccon->plmnId->numMncDigits;
   ueCb->plmnId.mcc[0] = rrccon->plmnId->mcc[0];
   ueCb->plmnId.mcc[1] = rrccon->plmnId->mcc[1];
   ueCb->plmnId.mcc[2] = rrccon->plmnId->mcc[2];
   ueCb->plmnId.mnc[0] = rrccon->plmnId->mnc[0];
   ueCb->plmnId.mnc[1] = rrccon->plmnId->mnc[1];
   if(ueCb->plmnId.numMncDigits > 2)
   {
      ueCb->plmnId.mnc[2] = rrccon->plmnId->mnc[2];
   }

   if (wrUmmRrcConS1InitUe(transCb) != ROK)
   {
      /* Increment the RRC failure due to S1AP counter */
      WR_INC_KPI_FUNC(wrIncKpiFrRrcS1apFailCause);

      /* Could not send out initial UE message to MME. Invoke local */
      /* context release and go away                                */
      transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
      wrEmmUpdateStat(WR_EMM_RRC_STATS,transCb->ueCb->cellId,WR_EMM_RRC_CON_FAIL,1,transCb->ueCb->establishCause);
      RETVALUE(ROK);
   }

   /* update the ueCb */
   wrUmmRrcFillUeSrbInfo(&ueCb->rbInfo, WR_PDCP_ID_SRB1);
   wrUmmRrcFillUeSrbInfo(&ueCb->rbInfo, WR_PDCP_ID_SRB2);

   /* update gummei information                                           */
   WR_MEM_COPY(&ueCb->gummei, &rrccon->gummei, sizeof(WrGummei));

   /* All the work is done in setting up the context. We can release */
   /* the transaction here                                           */
   wrUmmTransComplete(transCb);
   /*Infor about new UE attached in eNB to SON*/
   wrIfmSonAddUeInd(ueCb);

   wrEmmUpdateStat(WR_EMM_RRC_STATS,transCb->ueCb->cellId,WR_EMM_RRC_CON_SUCCESS,1,transCb->ueCb->establishCause);
   wrEmmUpdateStat(WR_EMM_CALL_STATS,transCb->ueCb->cellId,WR_EMM_CALL_TOT,1,WR_EMM_RRC_MAX);
   wrEmmUpdateStat(WR_EMM_CALL_STATS,transCb->ueCb->cellId,WR_EMM_CALL_ACT,1,WR_EMM_RRC_MAX);
   
   /*Increment KPI for Successful RRC Connection Establishments*/
   WR_INC_KPI_FUNC1(wrIncKpiFrRrcConSuccd,transCb->ueCb->establishCause);
   RETVALUE(ROK);
}

/** @brief This function is called to handle configuration confirm
 *         received from Physical layer.
 *
 * @details
 *
 *     Function:
 *
 *         Processing steps:
 *         - Performs the validation of Config Confim whether
 *           it is success or failure.
 *
 *
 * @param WrUmmTransCb  *transCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcSetupPrcPhyCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmPhyCfgCfm            *phyCfm = NULLP;
   WrRrcConTransCb           *rrcContranscb = NULLP;

   /*Update the RRC Trans CB state of PHY CFM */
   rrcContranscb = &transCb->u.rrcContranscb;
   /*Get the PHY CFM Message*/
   phyCfm = &transCb->msg->u.phyCfm;

   /* ccpu00132825: stop the timer if scheduler config is done*/
   if (rrcContranscb->schUeCfg == WR_UMM_CFG_SUCC)
   {
      wrStopTransTmr(transCb,WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG);
   }

   if (phyCfm->status != CTF_CFG_CFM_OK)
   {
      RLOG0(L_ERROR, "Configuration failure");
      rrcContranscb->phyCfg = WR_UMM_CFG_FAIL;
      RETVALUE(ROK);
   }
   rrcContranscb->phyCfg = WR_UMM_CFG_SUCC;
   rrcContranscb->state  = WR_UMM_RRC_CON_PHY_CFGD;

   RETVALUE(ROK);
}

/** @brief This function performs the SRB logical channel
 *         configuration towards MAC Scheduler.
 *
 * @details
 * Invoked by - wrUmmRrcSetupPrcSchCfgCfm
 *     Function:
 *
 *         Processing steps:
 *         - Builds the logical channels configuration for SRB.
 *         - Delivers the PDU towards MAC Scheduler through
 *           IFM module.
 *
 *
 * @param WrUmmTransCb  *transCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcConSchdSrbLchCfg
(
WrUmmTransCb                 *transCb
)
{
   RgrCfgReqInfo             *cfgReq;
   RgrLchCfg                 *lchCfg;
   U32                       transId;

   transCb->u.rrcContranscb.schSrbCfg = WR_UMM_CFG_ERRO;
   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   cfgReq->action            = RGR_CONFIG;
   cfgReq->u.cfgInfo.cfgType = RGR_LCH_CFG;

   lchCfg         = &cfgReq->u.cfgInfo.u.lchCfg;
   lchCfg->cellId = transCb->ueCb->cellId;
   lchCfg->crnti  = transCb->ueCb->crnti;
   lchCfg->lcId   = WR_PDCP_ID_SRB1;
   lchCfg->lcType = CM_LTE_LCH_DCCH;
   lchCfg->dlInfo.dlTrchType = CM_LTE_TRCH_DL_SCH;
   /*TODO Sriky  currently hard coded to be verified*/
   lchCfg->dlInfo.dlQos.qci = 5;

   lchCfg->dlInfo.dlQos.gbr = 1;
   lchCfg->dlInfo.dlQos.mbr = 1;
   lchCfg->dlInfo.dlQos.qci = SRB_QCI_VAL;
#ifdef LTE_L2_MEAS
   lchCfg->lcgId            = 0;
#endif

   lchCfg->ulLchQciInfo.lcId = WR_PDCP_ID_SRB1;
   lchCfg->ulLchQciInfo.qci = SRB_QCI_VAL;
   lchCfg->ulLchQciInfo.lcgId = 0;

   /* Dispatch the prepared message to scheduler                         */
   transId = transCb->transId | WR_UMM_RRC_CON_TRANS_SRB;
   transCb->u.rrcContranscb.schSrbCfg = WR_UMM_CFG_SENT;
   WrIfmSchdCfgUe(cfgReq, transId);

   RETVALUE(ROK);
}
/** @brief This function is called to handle Scheduler
 *         config confirmation.
 *
 * @details
 *     Function:  wrUmmRrcSetupPrcSchCfgCfm
 *
 *         Processing steps:
 *         - Performs the validation of Scheduler config
 *           confirm whether it is success or failure.
 *
 *
 * @param WrUmmTransCb *transCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcSetupPrcSchCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmSchdCfgCfm           *schdCfm = NULLP;
   WrRrcConTransCb           *rrcContranscb = NULLP;

   rrcContranscb = &transCb->u.rrcContranscb;
   schdCfm       = &transCb->msg->u.schdCfm;

   if ((schdCfm->transId & 0x0f) == WR_UMM_RRC_CON_TRANS_UE)
   {
      /* ccpu00132825: stop the timer if phy config is done*/
      if (rrcContranscb->phyCfg == WR_UMM_CFG_SUCC)
      {
         wrStopTransTmr(transCb,WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG);
      }
 
      if (schdCfm->status != RGR_CFG_CFM_OK)
      {
         RLOG0(L_ERROR, "UE Configuration failure");
         rrcContranscb->schUeCfg = WR_UMM_CFG_FAIL;
      }
      else
      {
         rrcContranscb->schUeCfg = WR_UMM_CFG_SUCC;
         if (wrUmmRrcConSchdSrbLchCfg(transCb) != ROK)
         {
            RLOG0(L_ERROR, "LCH Configuration failure");
         }
      }
   }
   else if ((schdCfm->transId & 0x0f) == WR_UMM_RRC_CON_TRANS_SRB)
   {
      if (schdCfm->status != RGR_CFG_CFM_OK)
      {
         RLOG0(L_ERROR, "SRB Configuration failure");
         rrcContranscb->schSrbCfg = WR_UMM_CFG_FAIL;
      }
      else
      {
         /* ccpu00128203 */
         if(wrUmmSchdUeLcgCfg(transCb->ueCb, transCb->transId,
                  WR_UMM_SRB_LCG_GRP_ID, 0,0) != ROK)
         {
            RLOG0(L_ERROR, "LCG Configuration failure");
         }
      }
   }
   else
   {
      if (schdCfm->status != RGR_CFG_CFM_OK)
      {
         RLOG0(L_ERROR, "LCG Configuration failure");
         rrcContranscb->schSrbCfg = WR_UMM_CFG_FAIL;
      }
      /* ccpu00126647 */
      else
      {
         rrcContranscb->schSrbCfg = WR_UMM_CFG_SUCC;
         rrcContranscb->state     = WR_UMM_RRC_CON_SCH_CFGD;
      }
   }
   
   if (schdCfm->status != RGR_CFG_CFM_OK)
   {
      WR_INC_KPI_FUNC(wrIncKpiFrRrcMacFailCause);     
   }
   
   RETVALUE(ROK);
}
/** @brief This function is called to handle RRC
 *         configuration confirm.
 *
 * @details
 *
 *     Function: wrUmmRrcSetupPrcRrcCfgCfm
 *
 *         Processing steps:
 *         - Performs the validation of RRC configuration
 *           confirm whether it is success or failure.
 *
 *
 *
 * @param WrUmmTransCb  *transCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcSetupPrcRrcCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   NhuCfgCfmSdus             *nhuCfgCfmSdus = NULLP;
   WrRrcConTransCb           *rrcContranscb = NULLP;

   /*Update the RRC Trans CB state of PHY CFM */
   rrcContranscb = &transCb->u.rrcContranscb;

   /* Check fro RRC Confirmation status*/
   nhuCfgCfmSdus = transCb->msg->u.rrcCfm;
   if (nhuCfgCfmSdus->sdu.isCfgSuccess != TRUE)
   {
      RLOG0(L_ERROR, "Configuration failure");
      rrcContranscb->rrcCfg = WR_UMM_CFG_FAIL;

      if ((nhuCfgCfmSdus->sdu.cfgError.errPres == TRUE) && 
          (nhuCfgCfmSdus->sdu.cfgError.errType == NHU_CFGERRTYPE_PDCP))
      {
         WR_INC_KPI_FUNC(wrIncKpiFrRrcPdcpFailCause);
      }
      if ((nhuCfgCfmSdus->sdu.cfgError.errPres == TRUE) && 
          (nhuCfgCfmSdus->sdu.cfgError.errType == NHU_CFGERRTYPE_RLC))
      {
         WR_INC_KPI_FUNC(wrIncKpiFrRrcRlcFailCause);
      }
      if ((nhuCfgCfmSdus->sdu.cfgError.errPres == TRUE) && 
          (nhuCfgCfmSdus->sdu.cfgError.errType == NHU_CFGERRTYPE_MAC))
      {
         WR_INC_KPI_FUNC(wrIncKpiFrRrcMacFailCause);
      }
      /* Increment the RRC configuration failure counter */
      WR_INC_KPI_FUNC(wrIncKpiFrRrcRrcFailCause);

      RETVALUE(ROK);
   }

   rrcContranscb->rrcCfg = WR_UMM_CFG_SUCC;
   rrcContranscb->state  = WR_UMM_RRC_CON_STK_CFGD;

   RETVALUE(ROK);
}
/** @brief This function is called to handle RRC message
 *         received from UE.
 *
 * @details
 * Invoked by -
 *     Function:
 *
 *         Processing steps:
 *         - It handles all the messages received from UE
 *           during RRC Setup transaction.
 *
 *
 * @param  WrUmmTransCb  *transCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcSetupPrcRrcDatInd
(
WrUmmTransCb                 *transCb
)
{
   NhuUL_DCCH_Msg            *ulDcchPdu = NULLP;
   U8                        messageType = 0;
   U8                        messageCat = 0;

   messageCat = transCb->msg->u.uuIndPdu->sdu.msgCategory;
   if (NHU_MSG_ULCCCH == messageCat)
   {
      if (wrUmmRrcSetupHdlr(transCb) != ROK)
      {
         RLOG0(L_ERROR, "wrUmmRrcSetupHdlr failed");
         RETVALUE(RFAILED);
      }
      RETVALUE(ROK);
   }
   else if (NHU_MSG_ULDCCH == messageCat)
   {
      ulDcchPdu = &transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg.dcchMsg;

      /*Get the message type*/
      messageType = ulDcchPdu->message.val.c1.choice.val;
      switch (messageType)
      {
         case C1_RRCCONSETUPCOMPL:
         {
#ifdef WR_TEST_CODE
            if ( wrTrgRrcConTO == TRUE )
            {
              /* Drop the ConSetupCmplt msg to verify the Time Out action */
               RLOG0(L_DEBUG, "Droping RRC ConSetupCmplt Msg");
               RETVALUE(ROK);
            }
#endif
            if (wrUmmRrcConSetupCmpHdl(transCb) != ROK)
            {
               RLOG0(L_ERROR, "wrUmmRrcConSetupCmpHdl failed");
               RETVALUE(RFAILED);
            }
            break;
         }
         default:
         {
            RLOG0(L_ERROR, "Invalid message received");
            break;
         }
      }
   }

   RETVALUE(ROK);
}
/** @brief This function performs undoing the things
 *         which are already done during RRC Setup transaction.
 *
 * @details
 * Invoked by - wrUmmRrcConPrcTransState
 *     Function:
 *
 *         Processing steps:
 *         - In Failure scenario, this function does
 *          a. DelUe to the MAC Scheduler.
 *          b. DelUe to the Physical layer.
 *          c. DelUe to RRC layer and RRC Connection Release
 *             to the UE.
 *
 *
 * @param WrUmmTransCb *transCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcConUndoCfg
(
WrUmmTransCb                 *transCb
)
{
   WrRrcConTransCb           *rrcConTrans = &transCb->u.rrcContranscb;
   WrUeCb                    *ueCb = transCb->ueCb;

   if ((rrcConTrans->schUeCfg == WR_UMM_CFG_SENT) ||
       (rrcConTrans->schUeCfg == WR_UMM_CFG_SUCC))
   {
      /* Delete the UE from scheduler */
      if (wrUmmSchdUeRel(transCb->transId, ueCb->crnti, ueCb->cellId) != ROK)
      {
         /* We cannot return from here. Need to invoke other layer release   */
         RLOG0(L_ERROR, "Failure in release of UE at SCH");
      }
   }

   if ((rrcConTrans->phyCfg == WR_UMM_CFG_SENT) ||
       (rrcConTrans->phyCfg == WR_UMM_CFG_SUCC))
   {
      /* Delete the UE from PHY layer */
      if (wrUmmL1SndUeRel(transCb->transId, ueCb->crnti, ueCb->cellId) != ROK)
      {
         /* We cannot return from here. Need to invoke other layer release   */
         RLOG0(L_ERROR, "Failure in release of UE at L1");
      }
   }

   if ((rrcConTrans->rrcCfg == WR_UMM_CFG_SENT) ||
       (rrcConTrans->rrcCfg == WR_UMM_CFG_SUCC))
   {
      /* Delete the UE from RRC layer */
      if (wrIfmRrcRelUe(ueCb->cellId, ueCb->crnti) != ROK)
      {
         RLOG0(L_ERROR, "Failure in release of UE at RRC");
      }
   }
   
   /* Send a UE Release Request to RRM */
   if (ROK != WrIfmRrmSendUeRelReq(ueCb->cellId, ueCb->crnti, transCb->transId))
   {
      RLOG_ARG1(L_INFO, DBG_CRNTI, (U32)ueCb->crnti,"Sending UE Release Request to "
         "RRM has Failed TransID:[%lu]", (transCb->transId));
   }
   RETVALUE(ROK);
}
/** @brief This function processes the RRC Setup transaction
 *         state.
 *
 * @details
 * Invoked by - wrUmmRrcConPrcTransState
 *     Function:
 *
 *         Processing steps:
 *         - If configuration confirms are failure then
 *           it tries for undoing the things then release
 *           the UE.
 *
 *
 * @param WrUmmTransCb  *transCb
 * @return ROK/RFAILED
 */

PRIVATE S16 wrUmmRrcConPrcTransState
(
WrUmmTransCb                 *transCb
)
{
   WrRrcConTransCb           *rrcConTrans = &transCb->u.rrcContranscb;

   if ((rrcConTrans->schUeCfg  == WR_UMM_CFG_ERRO) ||
       (rrcConTrans->schUeCfg  == WR_UMM_CFG_FAIL) ||
       (rrcConTrans->schSrbCfg == WR_UMM_CFG_ERRO) ||
       (rrcConTrans->schSrbCfg == WR_UMM_CFG_FAIL) ||
       (rrcConTrans->phyCfg    == WR_UMM_CFG_ERRO) ||
       (rrcConTrans->phyCfg    == WR_UMM_CFG_FAIL) ||
       (rrcConTrans->rrcCfg    == WR_UMM_CFG_ERRO))
   {
       /* UE Reject function*/
       /*127802*/
       RLOG_ARG1(L_WARNING, DBG_CRNTI, transCb->ueCb->crnti,"Send RrcConn Reject, "
          "Start Timer transCb[%x]",(unsigned int)transCb);
       wrSndRrcConRej(transCb, WR_UMM_RRC_REJ_DUE_TO_UNSPECIFIC);
       wrStopTransTmr(transCb,WR_TMR_RRC_RRCCON);
       /* ccpu00132825: stop the timer WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG before starting */
       wrStopTransTmr(transCb,WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG);
       wrStartTransTmr(transCb, WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG, WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG_VAL);
       RETVALUE(ROK);
   }

   /* If the failure is past the configuration of SCH and PHY, the error */
   /* is in RRC and is too late to send reject. Lets just clean up       */
   /* SRIKY - Split RRC configuation and message into two primitives     */
   if (rrcConTrans->rrcCfg == WR_UMM_CFG_FAIL)
   {
       /* UE Reject function*/
       /*127802*/
       RLOG_ARG1(L_WARNING, DBG_CRNTI, transCb->ueCb->crnti,"Send RrcConn Reject, "
          "Start Timer transCb[%x]",(unsigned int)transCb);
       wrSndRrcConRej(transCb, WR_UMM_RRC_REJ_DUE_TO_UNSPECIFIC);
       wrStopTransTmr(transCb,WR_TMR_RRC_RRCCON);
       wrStartTransTmr(transCb, WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG, WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG_VAL);

       RETVALUE(ROK);
   }

   RETVALUE(ROK);
}
/** @brief This function is called to handle the RRC Setup
 *         transaction.
 *
 * @details
 *
 *     Function:
 *
 *         Processing steps:
 *         - It handles RRC messages received from UE.
 *         - It handles Physical layer config confirm.
 *         - It handles MAC scheduler config confirm.
 *         - It handles RRC layer config confirm.
 *         - It handles RRC local Error Indication.
 *
 *
 * @param WrUmmTransCb *transCb
 * @return ROK/RFAILED
 */

PUBLIC S16 wrUmmRrcSetupProcessor
(
WrUmmTransCb                 *transCb
)
{
   S16                       ret;


   switch(transCb->msg->msgTyp)
   {
      case WR_UMM_UU_IND_PDU:
      {
         wrUmmRrcSetupPrcRrcDatInd(transCb);
         break;
      }
      case WR_UMM_RRC_CFG:
      {
         wrUmmRrcSetupPrcRrcCfgCfm(transCb);
         break;
      }
      case WR_UMM_SCHD_CFG:
      {
         /*Handler for SCHD Confirmation*/
         wrUmmRrcSetupPrcSchCfgCfm(transCb);

         /* All the internal configurations are successful, so perform    */
         /* configuration operation towards RRC and UE                    */
         if (transCb->u.rrcContranscb.state == WR_UMM_RRC_CON_SCH_CFGD)
         {
            if (wrUmmRrcConSndConSetup(transCb) != ROK)
            {
               RLOG0(L_ERROR, "RRC Setup Failed");
               transCb->u.rrcContranscb.rrcCfg = WR_UMM_CFG_ERRO;
            }
         }
         break;
      }
      case WR_UMM_TMR_EXPIRY:
      {
         switch(transCb->msg->u.tmrExpiry.timerEvnt)
         {
            case WR_TMR_RRC_RRCCON:
            {
                  RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,
                                             " RRC Conn Timer Expired");
               /*Increment KPI for Failed RRC Connection Establishments Due to degraded Radio conditions*/
               WR_INC_KPI_FUNC1(wrIncKpiFrRrcCnFailDuToDgrdRdio,transCb->ueCb->establishCause);
          
                 /*At this point of time we are not aware of
                 trigger for local context release transaction*/
               transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
          wrEmmUpdateStat(WR_EMM_RRC_STATS,transCb->ueCb->cellId,WR_EMM_RRC_CON_FAIL_DEGRAD,1,transCb->ueCb->establishCause);
       }
            break;

            case WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG:
            {
               /*127802*/
               RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Undo Cfg At "
                  "RRC, Release Call transnsCb[%x]", (unsigned int)transCb);
               wrUmmRrcConUndoCfg(transCb);
               transCb->state = WR_UMM_TRANS_DELETE_UE; 
            }
            break;

            default:
            {
               RLOG1(L_ERROR, "Received [%d] invalid Timer Event",
                  transCb->msg->u.tmrExpiry.timerEvnt);
               RETVALUE(RFAILED);
            }
         }
         break;
      }
      case WR_UMM_PHY_CFG:
      {
         wrUmmRrcSetupPrcPhyCfgCfm(transCb);
         break;
      }
      case WR_UMM_RRC_ERR_IND:
      {
         transCb->u.rrcContranscb.rrcCfg = WR_UMM_CFG_ERRO;
         break;
      }
      case WR_UMM_RRM_UEADMIT_RESP:
      {
         transCb->u.rrcContranscb.ueAdmitStatus = transCb->msg->u.rrmUeAdmitResp->stStatusInfo;
         if (transCb->u.rrcContranscb.ueAdmitStatus.enStatus == RMU_FAILURE)
         {
            if(transCb->u.rrcContranscb.ueAdmitStatus.enCause == RMU_CAUSE_MAX_UE_REACHED)
            {
               ret = wrSndRrcConRej(transCb, WR_UMM_RRC_REJ_DUE_TO_CONGESTION);
            }
            else
            {
               ret = wrSndRrcConRej(transCb, WR_UMM_RRC_REJ_DUE_TO_UNSPECIFIC);
            }
             if (ret != ROK)
             {
                RLOG0(L_ERROR, "wrSndRrcConRej failed.");
             }

             /*wrUmmTransComplete(transCb);*/
             RLOG0(L_ERROR, "UeCb state set to Rel Call");
             transCb->state = WR_UMM_TRANS_DELETE_UE;
             break;
         }
         else if (transCb->u.rrcContranscb.ueAdmitStatus.enStatus == RMU_REDIRECT)
         {
            transCb->ueCb->redirectUe = TRUE;
         }  
         
         /* Resources configiured by RRM can be commited @ UE Creation time even before transaction
            is complete. For the rest of transactions, the modifications are commited only after the 
            transaction is completed. */
         wrUmmCommitUeRadioResDedInfo(transCb->ueCb, &transCb->msg->u.rrmUeAdmitResp->stUeRadioResCfg);

         /* UE Cfg - PHY Layer */
         wrUmmRrcConPhyUeCfg(transCb);
         /* UE Cfg - Scheduler */
         wrUmmRrcConSchUeCfg(transCb);
         /* ccpu00132825: start the timer, if application doesnot receive scheduler and phy config 
            before it expires, revert the configuration and clear the UE context */
         wrStartTransTmr(transCb, WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG, WR_TMR_RRCCON_FAIL_RRC_UNDO_CFG_VAL);
         break;
      }
      default:
      {
         RLOG1(L_ERROR, "Received [%d] invalid message",transCb->msg->msgTyp);
         RETVALUE(RFAILED);
      }
   }

   if(transCb->state != WR_UMM_TRANS_DELETE_UE)
   {      
      wrUmmRrcConPrcTransState(transCb);
   }

   RETVALUE(ROK);
}/*wrUmmRrcSetupProcessor*/

/** @brief This function is called to handle the received RRC Re-establishment message 
 *         while RRC connection transaction is ongoing. 
 *
 * @details
 *
 *     Function: wrUmmDefRrcConnIncReestProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  inctrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDefRrcConnIncReestProcessor
(
WrUmmTransCb                *transCb, 
WrUmmTransCb                *incTrans
)
{
   /* Send reject to the received RRC Reestablishment message. 
      As a part of the Reject handler will release the UECb.*/
   incTrans->u.rrcReEstabTransCb.state = WR_UMM_REESTAB_REJECTED;
   incTrans->u.rrcReEstabTransCb.rejectCause = WR_UMM_REJ_CAUSE_RRC_CONNEC_GOING_ON;

   RETVALUE(WR_UMM_TRANS_OK);
}

/** @brief This function is called when any message related to a particular
 *         transaction during RRC connection transaction.
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmRrcConnNewTransProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  inctrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmRrcConnNewTransProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
  S16 retval = WR_UMM_TRANS_OK;
  switch(incTrans->transTyp)
  {
     case WR_UMM_RRC_REESTAB_TRANS:
     {
        retval = wrUmmDefRrcConnIncReestProcessor(transCb, incTrans);
        break;
     }
     default :
     {
        break;
     }
  }
  RETVALUE(retval);
}
/*
 *
 *      Fun:  wrGetWaitTime 
 *
 *      Desc:  Get the wait time in second.
 *
 *      Ret:   RETVOID
 *
 *      Notes: None
 *
 *      File: wr_umm_rrccon.c
 *
 */

PRIVATE Void wrGetWaitTime
(
   WrCellId              cellId, 
   U32                  *waitTime
)
{
    WrCellCb                  *cellCb;

    WR_GET_CELLCB(cellCb, cellId);
    if (cellCb == NULLP)
    {
        WR_DBGP(WR_ERROR,(WR_PRNT_BUF,"wrGetWaitTime:CellCB NULL (cellId = %d)\n", cellId));
        RETVOID;
    }

    /* Convert to second, if less than 1000 then return 1  */
    if ( cellCb->ueTmrConstants.t302 < 1000 )
    {
       *waitTime = 1;
    }
    else
    {
       *waitTime = cellCb->ueTmrConstants.t302/1000; 
    }
    RETVOID;
}



/*
 *
 *      Fun:   wrSndRrcConRej
 *
 *      Desc:  RRC Con Reject Message
 *
 *      Ret:   ROK
 *
 *      Notes: None
 *
 *      File: wr_umm_rrccon.c
 *
 */
PRIVATE S16 wrSndRrcConRej(WrUmmTransCb  *transCb, U8 internalCause)
{

   S16 ret = RFAILED;
   NhuDatRspSdus *nhuDatRspEvntSdu = NULLP;
   U32 waitTime = WR_UMM_WAIT_TIME_VALUE;

   TRC2(wrSndRrcConRej);
   
   /*Increment KPI for Failed RRC Connection Establishments Due to Non Availability of the Uu Resource*/
   WR_INC_KPI_FUNC1(wrIncKpiFrRrcCnFailDuToNoUuRsrc,transCb->ueCb->establishCause);
   
   wrEmmUpdateStat(WR_EMM_RRC_STATS,transCb->ueCb->cellId,WR_EMM_RRC_CON_FAIL,1,transCb->ueCb->establishCause);
   /*-- Allocate memory for event structure --*/
   WR_ALLOCEVNT(&nhuDatRspEvntSdu, sizeof(NhuDatRspSdus));
   if(NULLP == nhuDatRspEvntSdu)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /*-- Update RRC state to Connetion Releasing --*/
   transCb->ueCb->rrcConnState = WR_UMM_RRC_IDLE;

   RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"RRC Connection State = "
      "IDLE, Cause = RRC Connection Reject");

   /*-- Fill HDR Info --*/
   wrUmmFillRrcEvntHdr(&(nhuDatRspEvntSdu->hdr),transCb->ueCb->cellId,
                        transCb->ueCb->crnti, transCb->transId);

   /*-- Fill HDR Info --*/
   nhuDatRspEvntSdu->sdu.m.dlCcchMsg.rbId = WR_CCCH_DL_ID;
   nhuDatRspEvntSdu->sdu.m.dlCcchMsg.tmpCrnti = transCb->ueCb->crnti;

   /* Get the wait time */
   wrGetWaitTime(transCb->ueCb->cellId, &waitTime);

   /*-- Build RRC Con Reject Message --*/
   ret = wrUmmRrcFillConRej(nhuDatRspEvntSdu, waitTime);
   if (ret != ROK)
   {
      RLOG0(L_ERROR, "wrUmmRrcFillConRej Failed");
      WR_FREEEVNT(nhuDatRspEvntSdu);
      RETVALUE(RFAILED);
   }

   RLOG_ARG1(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[OTA] Sending "
      "RRCConnectionReject with Time to wait[%d]", waitTime);
   /*-- Send RRC Connection Reject through NhuDatRsp Primitive*/
   if (RFAILED ==  wrIfmRrcSndCcchRspToUe(nhuDatRspEvntSdu))
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe failed.");
      WR_FREEEVNT(nhuDatRspEvntSdu);
      RETVALUE(RFAILED);
   }

   WR_INC_KPI_FUNC1(wrIncKpiFrRrcConReject, internalCause);

   RETVALUE(ROK);
} /*-- wrSndRrcConRej --*/


/** @brief This function is local context release specific transaction release
 *         function. There is nothing held by this transaction and so a simple
 *         return function.
 *
 * @details
 *
 *     Function: wrUmmRrcConTransRel
 *
 *         Processing steps:
 *         - No Release Action Performed
 *
 * @param[in]  transCb : transaction information
 * @return  ROK
 */
PUBLIC S16 wrUmmRrcConTransRel
(
WrUmmTransCb                 *transCb
)
{
   wrStopTransTmr(transCb,WR_TMR_RRC_RRCCON);
   RETVALUE(ROK);
}
/********************************************************************30**

           End of file:    $SID$

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
$SID$        ---      Sriky         1. initial release TotaleNodeB 1.1
*********************************************************************91*/
