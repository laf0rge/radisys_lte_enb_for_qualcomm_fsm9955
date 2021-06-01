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
               

     File:     wr_umm_ctxtmod.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=52;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_utils.h"
#include "wr_umm.h"
#include "wr_umm_trans.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_sch_msg.h"
#include "wr_ifm_schd.h"
#include "wr_ifm_rrc.h"
#include "wr_umm_s1ap_msg.h"
#include "wr_kpi.h"
#include "wr_kpiIds.h"
#include "wr_ifm_rrm.h"
/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmCtxtModPrcSchdCfm                                           */
/*                                                                        */
/* This function :                                                        */
/*                This Function Handles SCH CFM message                   */
/*                and sends respective CTXTMOD based on SUCCESS           */
/*                or Failure                                              */
/* Steps involved :                                                       */
/*                                                                        */
/*                                                                        */
/* Return:                                                                */
/*    ROK: if Success                                                     */
/*    RFAILED: if any of the Case failed                                  */
/* ********************************************************************** */
PRIVATE S16 wrUmmCtxtModPrcSchdCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmCtxtModTrans         *ctxtMod = &(transCb->u.ctxtModTrans);
   U8                        idx;
    
   idx = transCb->msg->u.schdCfm.transId & 0x0f;
   ctxtMod->schdUeCfgCfmRcvd = TRUE;
   if (idx == WR_CTXT_MOD_UE_TRANS_ID)
   {
      /* This is in response to UE reconfiguration for AMBR              */
      if(transCb->msg->u.schdCfm.status != ROK)
      {
         ctxtMod->schdUeCfgCfmRcvd = FALSE;
         /* RADIO NW CAUSE                                                */
         /*  TODO : verify this value */
         ctxtMod->cause.causeTyp = 0;
         ctxtMod->cause.causeVal = 
            SztCauseRadioNwradio_resources_not_availableEnum;
         ctxtMod->status = WR_CTXT_MOD_SEND_FAIL;
       }
         ctxtMod->status = WR_CTXT_MOD_SEND_SUCC;
   }
   RETVALUE(ROK);
}


/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmCtxtModSchdUeReCfgReq                                       */
/*                                                                        */
/* This function                                                          */
/*      Send Filled Reconfiguration Request message to SCH                */
/*                                                                        */
/* Steps involved :                                                       */
/*                                                                        */
/*                                                                        */
/* Return:                                                                */
/*    ROK: if Success                                                     */
/*    RFAILED: if any of the Case failed                                  */
/* ********************************************************************** */

PRIVATE S16 wrUmmCtxtModSchdUeReCfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrUmmCtxtModTrans         *ctxtMod;
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   U32                       transId;

   /* CSG_DEV start */
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmCellCb               *cellCb = NULLP;

   ctxtMod = &(transCb->u.ctxtModTrans);
   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if(NULLP == cellCb)
   {
      transCb->state = WR_UMM_TRANS_RELEASE_CALL;
      RLOG_ARG0(L_ERROR, DBG_CELLID, ueCb->cellId, "Cell Cb is NULL ");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Set last 4 bits as 1 for UE reconfiguration */
   transId = transCb->transId | WR_CTXT_MOD_UE_TRANS_ID;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;
   if(ctxtMod->ambrCfg)
   {
      ueRecfg->ueRecfgTypes = RGR_UE_QOS_RECFG;
      ueRecfg->ueQosRecfg.ambrPres = TRUE;
      if ((ctxtMod->dlAmbr != 0) || (ctxtMod->ulAmbr != 0))
      {
         ueRecfg->ueQosRecfg.dlAmbr   = ctxtMod->dlAmbr / 8;
         ueRecfg->ueQosRecfg.ueBr     = ctxtMod->ulAmbr / 8;
      }
      else
      {
         ueRecfg->ueQosRecfg.dlAmbr   = ctxtMod->dlAmbr;
         ueRecfg->ueQosRecfg.ueBr     = ctxtMod->ulAmbr;
      }
   }
   if((cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS) && 
      (ctxtMod->csgMembrSta.pres == PRSNT_NODEF))
   {
      ueRecfg->ueRecfgTypes |=  RGR_UE_CSG_PARAM_RECFG;
      if(ctxtMod->csgMembrSta.val == WR_UMM_CSG_MEMBER)
      {
         ueRecfg->csgMmbrSta = TRUE;
      }
      else
      {
         ueRecfg->csgMmbrSta = FALSE;
      }
   }

   /* Dispatch the prepared message to scheduler.                         */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}


/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmCtxtModUpdateCfg                                            */
/*                                                                        */
/* This function :                                                        */
/*               Configure the SCH and lower layers for AMBR value        */
/*               and also update the ueCb with modified values mentioned  */
/*                                                                        */
/*                                                                        */
/* Return:                                                                */
/*    ROK: if Success                                                     */
/*    RFAILED: if any of the Case failed                                  */
/* ********************************************************************** */

PRIVATE S16 wrUmmCtxtModUpdateCfg
(
WrUmmTransCb            *transCb
)
{
      wrUmmCtxtModSchdUeReCfgReq(transCb);
   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmCtxtModPrcS1Msg                                             */
/*                                                                        */
/* This function will Process ue context modification is received         */
/*    from MME.                                                           */
/*                                                                        */
/* Steps involved :                                                       */
/*    1. Check if HO is in Progress if then send UECtxtModFail            */
/*    2. check for other IEs and if not send UeCtxtModFail                */
/* Return:                                                                */
/*    ROK: if Success                                                     */
/*    RFAILED: if any of the Case failed                                  */
/* ********************************************************************** */

PRIVATE S16 wrUmmCtxtModPrcS1Msg
(
WrUmmTransCb            *transCb
)
{
   WrUmmIncMsg                                 *msg = transCb->msg;
   WrUmmCtxtModTrans                           *ctxtMod;
   S1apPdu                                     *pdu;
   SztUECntxtModificationRqst                  *ctxtModReq;
   SztProtIE_Field_UECntxtModificationRqstIEs  *ies;
   SztSecurKey                                 *securKey;
   SztSubscriberProfileIDforRFP                *sbscrPfID;
   SztUEAggMaxBitrate                          *ueAggMaxBr;
   SztCSFallbackIndicator                      *csFlbkInd;
   SztUESecurCapabilities                      *ueSecCap;
   SztCSGMembershipStatus                      *csgMmbrSta;
   SztLAI                                      *lai;
   U16                                         ieIdx;
   WrUeCb                                      *ueCb = transCb->ueCb;

   ctxtMod = &(transCb->u.ctxtModTrans);
   pdu = msg->u.s1Pdu->pdu;
   ctxtModReq = &(pdu->pdu.val.initiatingMsg.value.u.sztUECntxtModificationRqst); 

   RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Received UE CONTEXT MODIFICATION"
             "REQUEST [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
             ueCb->s1ConCb->mme_ue_s1ap_id, ueCb->s1ConCb->enb_ue_s1ap_id);

   ctxtMod->ambrCfg = FALSE;
   /* Check if PS-HO is in Execution Phase if so Send UECtxtModFail */
   if(ueCb->mobCtrlState == WR_HO_IN_PROGRESS)
   {
      RLOG0(L_WARNING, "wrUmmCtxtModPrcS1Msg: PS-HO is in Execution Phase"
            "Sending UE ContextModification to MME");
      ctxtMod->status = WR_CTXT_MOD_SEND_FAIL;
      ctxtMod->cause.causeTyp = WR_CAUSE_RADIONW;
      if(WR_HO_TYPE_INTRA_LTE_S1 == ueCb->hoType) 
      {
         ctxtMod->cause.causeVal = 
            SztCauseRadioNws1_intra_system_handover_triggeredEnum;
      }
      else if( WR_HO_TYPE_IRAT_UTRA == ueCb->hoType) 
      {
         ctxtMod->cause.causeVal = 
            SztCauseRadioNws1_inter_system_handover_triggeredEnum;
      }

      else if(WR_HO_TYPE_X2 == ueCb->hoType)
      {
         ctxtMod->cause.causeVal = 
            SztCauseRadioNwx2_handover_triggeredEnum;
      }
      /*CSFB Attempt KPI*/
      WR_INC_KPI(KPI_ID_LTE_CSFB_ATT_SUM,
            KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   
      /*CSFB Failure KPI*/
      WR_INC_KPI(KPI_ID_LTE_CSFB_FAIL_SUM,
            KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
   
      RETVALUE(RFAILED);
   }

   for(ieIdx = 0; ieIdx < ctxtModReq->protocolIEs.noComp.val; ieIdx++)
   {
      ies = &(ctxtModReq->protocolIEs.member[ieIdx]);
      switch(ies->id.val)
      {
         case Sztid_SecurKey:
            {
               securKey = &(ies->value.u.sztSecurKey);
               if(ctxtMod->csFallBackInd.pres)
               {
                  /* NAS CAUSE */
                  ctxtMod->cause.causeTyp = WR_CAUSE_PROTOCOL;
                  ctxtMod->cause.causeVal = SztCauseProtsemantic_errorEnum;
                  ctxtMod->status = WR_CTXT_MOD_SEND_FAIL;
                  /* Increment the eCSFB Attempted and Failure KPI */
                  /*CSFB Attempt KPI*/
                  WR_INC_KPI(KPI_ID_LTE_CSFB_ATT_SUM,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  /*CSFB Failure KPI*/
                  WR_INC_KPI(KPI_ID_LTE_CSFB_FAIL_SUM,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  RETVALUE(RFAILED);
               }
               /* TODO : need to address */
               ctxtMod->secCfg = TRUE;
            }
            break;
         case Sztid_SubscriberProfileIDforRFP:
            {
               sbscrPfID = &(ies->value.u.sztSubscriberProfileIDforRFP);
               wrFillTknU32(&(ueCb->subscProfileId), sbscrPfID->val);
            }
            break;
         case Sztid_uEaggregateMaxBitrate:
            {
               /* AMBR and CSFB can come together?
                * as per 36413
                * handling AMBR: eNodeB should act and then sends UCM response 
                * handling CSFB: eNodeB should send response and then act 
                * implicitly both can not come together */

               ueAggMaxBr = &(ies->value.u.sztUEAggMaxBitrate);
               wrGetBitRate(&(ueAggMaxBr->uEaggregateMaxBitRateDL),
                     &(ctxtMod->dlAmbr));
               wrGetBitRate(&(ueAggMaxBr->uEaggregateMaxBitRateUL),
                     &(ctxtMod->ulAmbr));
               /* Check if the AMBR value received for both UL and DL are zero
                * if so, send UE Context Modification Failure to MME
                */
               if((0 == ctxtMod->dlAmbr) && (0 == ctxtMod->ulAmbr))
               {
                  RLOG2(L_ERROR,
                      "Invalid AMBR value received for UE idx [%d],CRNTI [%d]",
                         transCb->ueCb->ueIdx, transCb->ueCb->crnti);
                  ctxtMod->cause.causeTyp = WR_CAUSE_PROTOCOL;
                  ctxtMod->cause.causeVal = SztCauseProtsemantic_errorEnum;
                  ctxtMod->status = WR_CTXT_MOD_SEND_FAIL;
                  RETVALUE(RFAILED);
               }
               ctxtMod->ambrCfg = TRUE;
            }
            break;
         case Sztid_UESecurCapabilities:
            {
               ueSecCap = &(ies->value.u.sztUESecurCapabilities);
               /* TODO : need to address */
               if(ctxtMod->csFallBackInd.pres)
               {
                  /* NAS CAUSE */
                  ctxtMod->cause.causeTyp = WR_CAUSE_PROTOCOL; 
                  ctxtMod->cause.causeVal = SztCauseProtsemantic_errorEnum;
                  ctxtMod->status = WR_CTXT_MOD_SEND_FAIL;
                  /* Increment the eCSFB Attempted and Failure KPI */
                  /*CSFB Attempt KPI*/
                  WR_INC_KPI(KPI_ID_LTE_CSFB_ATT_SUM,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  /*CSFB failure KPI*/
                  WR_INC_KPI(KPI_ID_LTE_CSFB_FAIL_SUM,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  RETVALUE(RFAILED);
               }
               ctxtMod->updSecCap = TRUE;
            }
            break;
         case Sztid_CSFallbackIndicator:
            {
               csFlbkInd = &(ies->value.u.sztCSFallbackIndicator);
               if((ctxtMod->secCfg == TRUE) ||
                     (ctxtMod->updSecCap == TRUE))
               {
                  /* PROTOCOL CAUSE */
                  ctxtMod->cause.causeTyp = WR_CAUSE_PROTOCOL; 
                  ctxtMod->cause.causeVal = SztCauseProtsemantic_errorEnum;
                  ctxtMod->status = WR_CTXT_MOD_SEND_FAIL;
                  /* Increment the eCSFB Attempted and Failure KPI */
                  /*CSFB Attempt KPI*/
                  WR_INC_KPI(KPI_ID_LTE_CSFB_ATT_SUM,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);

                  /*CSFB failure KPI*/
                  WR_INC_KPI(KPI_ID_LTE_CSFB_FAIL_SUM,
                        KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                  RETVALUE(RFAILED);
               }
               if(csFlbkInd->pres == PRSNT_NODEF)
               {
                  wrFillTknU32(&(ctxtMod->csFallBackInd), csFlbkInd->val);
                  if(csFlbkInd->val == SztCSFallbackIndicatorcs_fallback_high_priorityEnum)
                  {
                     ueCb->ueServTyp = WR_UMM_EMERGENCY_SERV;
                  }
               }
            }
            break;
         case Sztid_CSGMembershipStatus:
            {
               csgMmbrSta = &(ies->value.u.sztCSGMembershipStatus);
               if(SztCSGMembershipStatusmemberEnum == csgMmbrSta->val)
               {
                  wrFillTknU32(&(ctxtMod->csgMembrSta), WR_UMM_CSG_MEMBER);
               }
               else
               {
                  wrFillTknU32(&(ctxtMod->csgMembrSta), WR_UMM_CSG_NOT_MEMBER);
               }
            }
            break;
         case Sztid_RegisteredLAI:
            {
               lai = &(ies->value.u.sztLAI);
               wrUtlGetPlmnId(&(ueCb->regLai.plmnId), &(lai->pLMNidentity));
               WR_GET_U32_FRM_OSXL(ueCb->regLai.lac,lai->lAC);
			
	            ueCb->regLai.isLaiPres = TRUE;
               if(NULLP != ueCb->hoRstrctInfo)
               {
                  if(FALSE == wrValidateRegLai(ueCb->hoRstrctInfo,ueCb->regLai))
                  {
                     ctxtMod->status = WR_CTXT_MOD_SEND_FAIL;
                     ctxtMod->cause.causeTyp = WR_CAUSE_RADIONW;
                     ctxtMod->cause.causeVal = 
                        SztCauseRadioNwho_target_not_allowedEnum;
                     RLOG0(L_ERROR,"Registered LAI is invalid");
                     /*Increment CSFB Attempt Counter*/
                     WR_INC_KPI(KPI_ID_LTE_CSFB_ATT_SUM,KPI_VALUE_TYPE_INTEGER,
                           INC_KPI_VALUE_BY_ONE);
                     /*CSFB Failure KPI*/
                     WR_INC_KPI(KPI_ID_LTE_CSFB_FAIL_SUM,
                           KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
                     RETVALUE(RFAILED);
                  }
               }
            }
            break;
      }
   }

    /* TODO : need to address */
   if(ctxtMod->secCfg || ctxtMod->updSecCap)
   {
      /* as TotaleNodeB is not handling these two IEs, sending
       * UE Context Modification Failure to MME */
      ctxtMod->cause.causeTyp = WR_CAUSE_MISC;
      ctxtMod->cause.causeVal = SztCauseMiscunspecifiedEnum;
      ctxtMod->status = WR_CTXT_MOD_SEND_FAIL;
      RETVALUE(RFAILED);
   }

   /* Check if UE's serving PLMN's CSFB RAT priority is Forbidden*/
   if((NULLP != ueCb->hoRstrctInfo) && 
         (PRSNT_NODEF == ctxtMod->csFallBackInd.pres) &&
         (PRSNT_NODEF == ueCb->hoRstrctInfo->frbdnInterRATs.pres))
   {
      if(wrChkAllRatsForbidden(ueCb))
      {
         ctxtMod->status = WR_CTXT_MOD_SEND_FAIL;
         ctxtMod->cause.causeTyp = WR_CAUSE_RADIONW;
         ctxtMod->cause.causeVal = 
            SztCauseRadioNwho_target_not_allowedEnum;
         RLOG0(L_ERROR,"Chosen CSFB RAT is Forbidden for UE");
         /*Increment CSFB Attempt Counter*/
         WR_INC_KPI(KPI_ID_LTE_CSFB_ATT_SUM,KPI_VALUE_TYPE_INTEGER,
               INC_KPI_VALUE_BY_ONE);
         /*CSFB Failure KPI*/
         WR_INC_KPI(KPI_ID_LTE_CSFB_FAIL_SUM,
               KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
         RETVALUE(RFAILED);
      }
   }

   ctxtMod->status = WR_CTXT_MOD_SEND_SUCC;
   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmCtxtModSndCtxtModRsp                                        */
/*                                                                        */
/* This function sends wrUmmCtxtModSndCtxtModRsp to MME                   */
/*                                                                        */
/*                                                                        */
/* Steps involved :                                                       */
/*     a. sends wrUmmCtxtModSndCtxtModRsp to MME                          */
/*     b. sets transcb state to WR_UMM_TRANS_DONE                         */
/* Return:                                                                */
/*                                                                        */
/*    ROK: if Success                                                     */
/*    RFAILED: if any of the Case failed                                  */
/* ********************************************************************** */
PUBLIC S16 wrUmmCtxtModSndCtxtModRsp
(
WrUmmTransCb            *transCb
)
{
   WrUmmCtxtModTrans         *ctxtMod = &(transCb->u.ctxtModTrans);
   WrUeCb                    *ueCb = transCb->ueCb;
   S1apPdu                   *rspPdu;

   /* to avoid the linking error as the following fucntion have no 
    * definition */
   if (ctxtMod->status == WR_CTXT_MOD_SEND_SUCC)
   {
      RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Sending UE CONTEXT MODIFICATION"
                "RESPONSE [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
         ueCb->s1ConCb->mme_ue_s1ap_id, ueCb->s1ConCb->enb_ue_s1ap_id);
      wrUmmS1apUeCntxtModRsp(ueCb->s1ConCb, &rspPdu);
   }
   else
   {
      RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Sending UE CONTEXT MODIFICATION"
                "FAILURE [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
                ueCb->s1ConCb->mme_ue_s1ap_id, ueCb->s1ConCb->enb_ue_s1ap_id);
      wrPrintS1apCauseInfo(ctxtMod->cause.causeTyp,ctxtMod->cause.causeVal);
      wrUmmS1apUeCntxtModFailure(ueCb->s1ConCb, &rspPdu, &ctxtMod->cause);
   }

   wrIfmS1apSndUeMsg(ueCb->s1ConCb->spConnId, rspPdu);
   transCb->state = WR_UMM_TRANS_DONE;

   RETVALUE(ROK);
}

/** @brief This function is responsible for sending UE Reconfiguration message
 *         to RRM if CSG membership status changes during UE context modification
 *
 * @details
 *
 *     Function: wrUmmPrcRabSetup
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmCtxtModRrmCsgParam
(
WrUmmTransCb                 *transCb
)
{
   RmuUeRecfgReq             *ueRecfgReq = NULLP;
   S16                       retVal = ROK;
   /* HCSG_DEV */
   WrUmmCellCb               *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId);
   if(NULLP == cellCb)
   {
      transCb->state = WR_UMM_TRANS_RELEASE_CALL;
      RLOG_ARG0(L_ERROR, DBG_CELLID, transCb->ueCb->cellId, "Cell Cb is NULL ");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&ueRecfgReq, sizeof(RmuCommonMsg));
   if (ueRecfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Failed to allocate memory for RrmWrUeRecfgReqInfo.");
      RETVALUE(RFAILED);
   }

   ueRecfgReq->bCellId = transCb->ueCb->cellId;
   ueRecfgReq->usCrnti = transCb->ueCb->crnti;
   /* HCSG_DEV - start */
   if((cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS) && 
      (transCb->u.ctxtModTrans.csgMembrSta.pres == PRSNT_NODEF))
   {
      ueRecfgReq->bRecfgBitMap |=  RMU_RECFG_REQ_CSG_MEMBERSHIP_MOD;
      if(WR_UMM_CSG_MEMBER == transCb->u.ctxtModTrans.csgMembrSta.val)
      {
         ueRecfgReq->isMember = TRUE;
      }
      else
      {
         ueRecfgReq->isMember = FALSE;
      }
   }
   /* HCSG_DEV - end */
   retVal = wrIfmRrmSndUeRecfgReq(ueRecfgReq);
   RETVALUE(retVal);
}

/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmCtxtModProcessor                                            */
/*                                                                        */
/* This function is called when a ue context modification is received     */
/*    from MME.                                                           */
/*                                                                        */
/* Steps involved in local context release:                               */
/*    1. If AMBR values needs to be updated, then configure the Schedular */
/*       with appropriate values.                                         */
/*                                                                        */
/* Failure:                                                               */
/*    There is provision for failure in release procedure. If there are   */
/*    security and cs fallback indicator present in the mesage OR         */
/*    if the ambr configuration at the schedular failes, then the         */
/*    context modification failure message is sent to MME.                */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmCtxtModProcessor
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   WrUmmCtxtModTrans         *ctxtMod = &(transCb->u.ctxtModTrans);
   /* CSG_DEV start */
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmCellCb               *cellCb = NULLP;
   /* CSG_DEV end */

   /*if the status is already WR_CTXT_MOD_SEND_FAIL then send UCM failure 
    * from here and close the transaction */
   if(WR_CTXT_MOD_SEND_FAIL == ctxtMod->status)
   {
      wrUmmCtxtModSndCtxtModRsp(transCb);
      RETVALUE(RFAILED);
   }
   ctxtMod->status = WR_CTXT_MOD_WAIT;

   switch(msg->msgTyp)
   {
      case WR_UMM_S1_DAT_IND:
      {
         WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
         if(NULLP == cellCb)
         {
            transCb->state = WR_UMM_TRANS_RELEASE_CALL;
            RLOG_ARG0(L_ERROR, DBG_CELLID, ueCb->cellId, "Cell Cb is NULL ");
            RETVALUE(RFAILED);
         }
         /* process the incoming S1AP Message                             */
         if((ROK != wrUmmCtxtModPrcS1Msg(transCb)))
         {
            /*Sending UE CONTEXT MODIFICATION FAILURE*/
            wrUmmCtxtModSndCtxtModRsp(transCb);
            RETVALUE(RFAILED);
         }
         else if((PRSNT_NODEF == ctxtMod->csFallBackInd.pres))
         {
            /*Sending UE CONTEXT MODIFICATION RESPONSE*/
            wrUmmCtxtModSndCtxtModRsp(transCb);
         }
         else if((cellCb->csgAccessMode != WR_ENB_HYBRID_ACCESS) &&
               (PRSNT_NODEF == ctxtMod->csgMembrSta.pres))
         {
            /*Sending UE CONTEXT MODIFICATION RESPONSE*/
            wrUmmCtxtModSndCtxtModRsp(transCb);
         }
         if(PRSNT_NODEF == ctxtMod->csFallBackInd.pres)
         {
            /* perform the CSFB transaction creation
             * along with CSFB if any other valid IEs are sent by MME 
             * (valid IEs: security Key and UE Security Capability IEs are
             * invalid as per 36413 but discussed about other IEs)
             * those IEs will not be handled e.g AMBR,CSG */
            if(RFAILED == wrUmmCreatCsfbTrans(transCb))
            {
               RLOG0(L_ERROR, "Creation of CSFB transaction FAILED");
            }
         }
         else if(ctxtMod->csgMembrSta.pres == PRSNT_NODEF)
         {
            /*CSG_DEV:Check for the CSG Member state for UE here and 
              initiate the UE context release procedure
              */
            if (cellCb->csgAccessMode == WR_ENB_CLOSED_ACCESS)
            {
               if (ctxtMod->csgMembrSta.val == WR_UMM_CSG_NOT_MEMBER)
               {
                  /* Initiate the UE context release procedure here */
                  transCb->errorCause = WR_UMM_CTXT_REL_CSG_SUBSCRIPTION_EXP;
                  transCb->state = WR_UMM_TRANS_RELEASE_CALL;
               }/* if WR_UMM_CSG_NOT_MEMBER*/   
            }/* if WR_ENB_CSG_CLOSED_ACCESS */
            else if (cellCb->csgAccessMode == WR_ENB_OPEN_ACCESS)
            {
               RLOG1(L_WARNING,"Ignoring UE Subscription info in OPEN Access "
                     "MemberStatus [%d]", ctxtMod->csgMembrSta.val);
            }
            else if(cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS)
            {
               RLOG0(L_INFO,"Processing UE Context modification sending Scheduler Reconfiguration"
                     "and RRM UE Reconfiguration");
               wrUmmCtxtModUpdateCfg(transCb);
               wrUmmCsgModMbrStsForCsgId(ueCb, cellCb->csgId, 
                     (U8)ctxtMod->csgMembrSta.val);
               wrUmmCtxtModRrmCsgParam(transCb);

            }/* WR_ENB_CSG_HYBRID_ACCESS */
            RETVALUE(ROK);
         }
         else
         {
            /* perform the necessary updations for IEs
             * AMBR, Security Key and UE Security Capabilities */
            wrUmmCtxtModUpdateCfg(transCb);
         }
         break;
      }
      case WR_UMM_SCHD_CFG:
      {
         wrUmmCtxtModPrcSchdCfm(transCb);
         wrUmmCtxtModSndCtxtModRsp(transCb);
         break;
      }
      default:
      {
         RLOG1(L_ERROR, "Unkonwn message [%d] received at context modif "
            "processor", msg->msgTyp);
         break;
      }
   }
   
   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmCtxtModTransRel                                             */
/*                                                                        */
/* This function is local context release specific transaction release    */
/* function. There is nothing held by this transaction and so a simple    */
/* return function.                                                       */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmCtxtModTransRel
(
WrUmmTransCb                 *transCb
)
{
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
