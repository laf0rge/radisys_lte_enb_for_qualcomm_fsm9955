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
               

     File:     wr_umm_ctxtrel.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=53;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_utils.h"
#include "wr_umm.h"
#include "wr_emm.h"
#include "wr_umm_trans.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_sch_msg.h"
#include "wr_umm_l1_msg.h"
#include "wr_umm_s1ap_msg.h"
#include "wr_ifm_schd.h"
#include "wr_ifm_dam.h"
#include "wr_ifm_rrc.h"
#include "wr_ifm_l1.h"
#include "wr_ifm_s1ap.h"
#include "wr_kpi.h"
#include "wr_ifm_rrm.h"
#include "wr_ifm_son.h"

#ifdef WR_TEST_CODE
EXTERN U8 wrTrgRrcConRelTO;
#endif

/* ********************************************************************** */
/* Name: wrUmmCtxtRelUpdateRelCause                                       */
/*                                                                        */
/* This function to update the release cause for rrc Connection Release   */
/* ********************************************************************** */

/* Start  ccpu00126205 - eNodeB is not sending cause value as
     "loadBalancingTAUrequired" in RRC Connection Release  */
PRIVATE S16 wrUmmCtxtRelUpdateRelCause
(
WrUmmTransCb                 *transCb
)
{
   SztInitiatingMsg                      *initMsg = NULLP;
   SztProtIE_Cont_UECntxtRlsCmmd_IEs     *protIes = NULLP;
   S1apPdu                                   *pdu = NULLP;
   SztProtIE_Field_UECntxtRlsCmmd_IEs         *ie = NULLP;
   U8                                         cnt = 0;
   
   pdu = transCb->msg->u.relInd->pdu;
   initMsg = &(pdu->pdu.val.initiatingMsg);
   protIes = &initMsg->value.u.sztUECntxtRlsCmmd.protocolIEs;
  
   /* By default rrc Release Cause is NhuRlsCauseotherEnum  */ 
   transCb->u.ctxtRelTrans.rrcConRelCause = NhuRlsCauseotherEnum;
   
   for (cnt = 0; cnt < protIes->noComp.val; cnt++)
   {
       ie = &(protIes->member[cnt]);
       switch (ie->id.val)
       {
           case  Sztid_Cause:  /* If the cause is Sztid_Cause = 2 -? cm/szt_asn.h */
           {
               wrEmmUpdateKpiCounter(&ie->value.u.sztCause);

          if (ie->value.u.sztCause.choice.val == WR_CAUSE_RADIONW )   
          {   
         if(ie->value.u.sztCause.val.radioNw.val == SztCauseRadioNwload_balancing_tau_requiredEnum )
              {
             transCb->u.ctxtRelTrans.rrcConRelCause = NhuRlsCauseloadBalancingTAUrequiredEnum;
                       RETVALUE(ROK);
              }
          }
               /*Other causes inclused Transport, NAS, Protocol and  Miscelaneous  */
          break;
           }
      default:
              break;   
       }
    }   
   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmCtxtRelSndRrcConRel                                         */
/*                                                                        */
/* This function sends release towards RRC which includes RRC Connection  */
/*    Release message to be sent to UE and release of UE from other       */
/*    layers.                                                             */
/*                                                                        */
/* ********************************************************************** */
PRIVATE S16 wrUmmCtxtRelSndRrcConRel
(
 WrUmmTransCb                *transCb
)
{
   NhuDatReqSdus             *sdu;
   U8                         accessStratumRls = 0; /* release 8 */
   WrUmmRedirectInfo         *redirInfo = NULLP;

   WR_ALLOCEVNT(&sdu, sizeof(*sdu));
   if (sdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   if(transCb->ueCb->redirectUe == TRUE)
   {
      RLOG0(L_WARNING, "Maximum UE Limit reached, redirecting UE");
      redirInfo = (WrUmmRedirectInfo*) wrUmmFillRedirectInfo(transCb);
   }
   if(transCb->ueCb->ueCap != NULLP)
   {
      accessStratumRls = transCb->ueCb->ueCap->eUTRA_Cap.accessStratumRls.val;
   }
   wrUmmFillRrcEvntHdr(&sdu->hdr, transCb->ueCb->cellId, transCb->ueCb->crnti,
                                  transCb->transId);
   if (wrUmmRrcBldConRls(transCb->ueCb, accessStratumRls, redirInfo, sdu,
            transCb->u.ctxtRelTrans.rrcConRelCause , 0) != ROK)
   {
      RETVALUE(RFAILED);
   }
   sdu->sdu.isUeCfgPres = FALSE;
   RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti, "Sending "
      "RRCConnectionRelease, Cause[%d]", NhuRlsCauseotherEnum);
   wrIfmRrcSndReqToUe(sdu);
   if(redirInfo != NULLP)
   {
      WR_FREE(redirInfo, sizeof(WrUmmRedirectInfo));
   }
   /*Start RRC Connection Release Timer*/
#if 0
#else
   wrStartTransTmr(transCb,WR_TMR_RRC_CONREL, 
         wrEmmGetRrcConRelTmrVal (transCb->ueCb->cellId));
#endif
   RETVALUE(ROK);
}
/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmCtxtRelPrcTmrExpiry                      */
/*                                                                        */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmCtxtRelPrcTmrExpiry 
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb            *ueCb = transCb->ueCb;

   /* Send L1 configuration to release ute UE                             */
   if (wrUmmL1SndUeRel(transCb->transId, ueCb->crnti, ueCb->cellId) != ROK)
   {
      /* We cannot return from here. Need to invoke other layer release   */
      RLOG0(L_ERROR, "Failure in release of UE at L1");
   }

   /* Send scheduler configuration to release the UE                      */
   if (wrUmmSchdUeRel(transCb->transId, ueCb->crnti, ueCb->cellId) != ROK)
   {
      /* We cannot return from here. Need to invoke other layer release   */
      RLOG0(L_ERROR, "Failure in release of UE at scheduler");
   }
 
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmCtxtRelSndRlsCmpl
(
WrUmmTransCb                *transCb
)
{
   WrUeCb                  *ueCb = transCb->ueCb;
   S1apPdu                 *ctxtRelPdu;
   SztRelRsp               relRsp;
   U8                      cntr1;
   U8                      cntr2;

   for(cntr1 = 0;cntr1<transCb->ueCb->rbInfo.numOfRbCfgd;cntr1++)
   {
     WR_INC_KPI_FUNC1(wrIncKpiFrERABRelAttPrQci,transCb->ueCb->rbInfo.rab[cntr1].qci);
   }
   
   wrUmmS1apFillCtxtRelCmpl(ueCb->s1ConCb, &ctxtRelPdu);
   if(ctxtRelPdu == NULLP)
   {
      RLOG0(L_ERROR, "Building of Context Release Message Failed.");
      RETVALUE(RFAILED);
   }

   RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Sending UE CONTEXT RELEASE COMPLETE"
             "[MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
             ueCb->s1ConCb->mme_ue_s1ap_id, ueCb->s1ConCb->enb_ue_s1ap_id);
   relRsp.suConnId = ueCb->s1ConCb->suConnId;
   relRsp.spConnId = ueCb->s1ConCb->spConnId;
   relRsp.pdu      = ctxtRelPdu;

   WrIfmS1apRelRsp(&relRsp);
   /* Release complete message will release the S1AP layer context
    * so need to set s1apConnState to  WR_S1AP_NOT_CONNECTED 
    * to avoid resending the S1AP layer ue context release */
   ueCb->s1ConCb->s1apConnState = WR_S1AP_NOT_CONNECTED;
   wrEmmUpdateStat(WR_EMM_CALL_STATS ,transCb->ueCb->cellId, WR_EMM_CALL_ACT, WR_EMM_DFLT_NEG_STEP,WR_EMM_RRC_MAX);
   wrEmmUpdateStat(WR_EMM_UE_CTXT_REL_STAT,ueCb->cellId,WR_EMM_UE_CTXT_REL_SUCCESS,WR_EMM_DFLT_POS_STEP,WR_EMM_UE_CTXT_REL_SUBTYP_MAX);
 
   for(cntr2 = 0;cntr2<transCb->ueCb->rbInfo.numOfRbCfgd;cntr2++)
   {
     WR_INC_KPI_FUNC1(wrIncKpiFrERABRelSuccPrQci,transCb->ueCb->rbInfo.rab[cntr2].qci);
   }
    
   /*Increment KPI for UE CONTEXT Release: Successful UE CONTEXT Release*/
   WR_INC_KPI_FUNC(wrIncKpiUeCtxRelReqSuccd);

   RETVALUE(ROK);
}


PRIVATE S16 wrUmmSendCtxtRelReq
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   SztRelReq                 s1RelReq={0};

   /* Send L1 configuration to release ute UE                             */
   if (wrUmmL1SndUeRel(transCb->transId, ueCb->crnti, ueCb->cellId) != ROK)
   {
      /* We cannot return from here. Need to invoke other layer release   */
      RLOG0(L_ERROR, "Failure in release of UE at L1");
   }

   /* Send scheduler configuration to release the UE                      */
   if (wrUmmSchdUeRel(transCb->transId, ueCb->crnti, ueCb->cellId) != ROK)
   {
      /* We cannot return from here. Need to invoke other layer release   */
      RLOG0(L_ERROR, "Failure in release of UE at scheduler");
   }
   if(wrEmmCb.mmeCont.mmes[ueCb->mmeId] != NULLP) 
   {
      if((ueCb->s1ConCb != NULLP) &&
            (ueCb->s1ConCb->s1apConnState != WR_S1AP_NOT_CONNECTED) &&
            (wrEmmCb.mmeCont.mmes[ueCb->mmeId]->state == WR_MME_UP))
      {
         /* S1AP is not deleting the UE context due to invalid suConnId.*/
         s1RelReq.connId = ueCb->s1ConCb->suConnId;
         s1RelReq.connIdType = SZT_CONN_TYPE_SU;
         s1RelReq.relType = SZT_LOCAL_REL;

         if(WrIfmS1apRelReq(&s1RelReq) != ROK)
         {
            RLOG0(L_ERROR, "Failure in S1AP Local Release");
         }
      }

   }
   RETVALUE(ROK);
}



PRIVATE Void wrUmmCtxtRelTransInit
(
WrUmmTransCb                 *transCb
)
{
   transCb->u.ctxtRelTrans.schdCfmRcvd = FALSE;
   transCb->u.ctxtRelTrans.l1CfmRcvd   = FALSE;
   transCb->u.ctxtRelTrans.damCfmRcvd  = FALSE;
   transCb->u.ctxtRelTrans.rrmRespRcvd = FALSE;
}

/**
 *
 * @details
 *
 *     Function: wrUmmCtxtRelPrcParseUeS1apIdPair 
 *
 *         Processing steps:
 *         - This function updates the ueCb with the spConnId
 *           enb_ue_s1ap_id and mme_ue_s1ap_id
 *
 * @param WrUmmTransCb         *trans
 */ 
PRIVATE Void wrUmmCtxtRelPrcParseUeS1apIdPair
(
WrUmmTransCb                 *transCb
)
{
   TknU8          *mmeS1apId = NULLP;
   SztUE_S1AP_IDs *ueS1apId = NULLP;
   WrUeCb                    *ueCb = transCb->ueCb;

   ueCb->s1ConCb->spConnId = transCb->msg->u.relInd->spConnId;
   ueCb->s1ConCb->enb_ue_s1ap_id = transCb->msg->u.relInd->spConnId;
      
   wrSzGetIE(&transCb->msg->u.relInd->pdu->pdu,Sztid_UE_S1AP_IDs,
                        (TknU8 **)&ueS1apId);
   if (ueS1apId != NULLP)
    {
       switch (ueS1apId->choice.val)
       {
          case UE_S1AP_IDS_UE_S1AP_ID_PAIR:
                mmeS1apId =  
       (TknU8*)&ueS1apId->val.uE_S1AP_ID_pair.mME_UE_S1AP_ID;
                 ueCb->s1ConCb->mme_ue_s1ap_id =
           ((SztMME_UE_S1AP_ID *)mmeS1apId)->val;
           break;
       }
     }
}

/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmUeLclCtxtRel                                                */
/*                                                                        */
/* This function is called when a context is to be released locally.      */
/*                                                        */
/*                                                        */
/* ********************************************************************** */
PRIVATE S16 wrUmmUeLclCtxtRel(WrUmmTransCb* transCb)
{ 
   wrUmmCtxtRelTransInit(transCb);
   wrStopUeTmr(transCb->ueCb, WR_TMR_UE_CTX_REL_REQ);
#ifndef WR_GRPPWR_CNTRL
   //wrStopUeTmr(transCb->ueCb, WR_TMR_RAD_RES_MOD_TMR);
#endif
#ifdef WR_DETECT_RLF
   wrStopUeTmr(transCb->ueCb, WR_TMR_DETECT_RLF);
#endif
   /* Filling the cause for local release*/
   transCb->u.ctxtRelTrans.rrcConRelCause = transCb->msg->u.rrcConRelCause; 
   /*Fix for: ccpu00125889
    * if UE is not in RRC CONNECTED state just do local context release
    * else send Connection Release request to UE */
   if (transCb->ueCb->rrcConnState != WR_UMM_RRC_CONNECTED)
   {
      wrUmmSendCtxtRelReq(transCb);
      /* Delete the UE from RRC/RLC/MAC layers */
      /* Fix for ccpu00123517 */
      if (wrIfmRrcRelUe(transCb->ueCb->cellId, transCb->ueCb->crnti) != ROK)
      {
         RLOG0(L_ERROR, "Failure in release of UE at RRC");
      }
   }
   else
   {
      /* Send RRC connection release to UE */
      wrUmmCtxtRelSndRrcConRel(transCb);
   }

   RETVALUE(ROK);
}

/** @brief: This function will process the internal message
 *          poseted for the Local Release transaction.
 *          If the Internal message contains Redirection
 *          Information then call the wrUmmCsfbRelUe().
 * */
PRIVATE S16 wrUmmRedirPrcIntMsg
(
 WrUmmTransCb     *transCb
 )
{
   NhuDatReqSdus       *sdu;
   WrUmmRedirectInfo   *redirInfo = NULLP;
   WrUeCapInfo         *ueCap = transCb->ueCb->ueCap;
   U8                   accessStratumRls;
   U32                  t320Val;

   redirInfo = transCb->msg->u.internalMsg->u.redirInfo;

   WR_ALLOCEVNT(&sdu, sizeof(*sdu));
   if (sdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   accessStratumRls = ueCap->eUTRA_Cap.accessStratumRls.val;

   wrUmmFillRrcEvntHdr(&sdu->hdr, transCb->ueCb->cellId, transCb->ueCb->crnti,
         transCb->transId);

   /* 
    *Setting transaction ID for above allocation transaction Id
    * */
   sdu->hdr.transId = transCb->transId | WR_UMM_UTRA_TDD_REDIR_REL_TRANSID;

   t320Val = 0;
   if (wrUmmRrcBldConRls(transCb->ueCb, accessStratumRls, redirInfo, sdu, 
            NhuRlsCauseotherEnum, t320Val) != ROK)
   {
      RETVALUE(RFAILED);
   }
   sdu->sdu.isUeCfgPres = FALSE;

   transCb->ueCb->rrcConnState = WR_UMM_RRC_IDLE;
   wrStartTransTmr(transCb,WR_TMR_RRC_CONREL, 
         wrEmmGetRrcConRelTmrVal (transCb->ueCb->cellId));

   RLOG0(L_WARNING,"wrUmmCsfbSndRrcConRelMsg: Sending RRC Connection Release");
   wrIfmRrcSndReqToUe(sdu);

   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmLclCtxtRelProcessor                                         */
/*                                                                        */
/* This function is called when a context is to be released locally.      */
/*    Local release includes Uu release along with other layers.          */
/*                                                                        */
/* Steps involved in local context release:                               */
/*    1. Configure RRC to send out RRC release and also release resources */
/*       in MAC, RLC/PDCP once the release PDU is successfully delivered  */
/*       to the UE.                                                       */
/*    2. Upon receiving configuration confirm from RRC, configure the     */
/*       Data application module and the scheduler to release the         */
/*       resources.                                                       */
/*                                                                        */
/* Failure:                                                               */
/*    There is provision for failure in release procedure. If RRC does    */
/*    respond to release, after a timeout, the other layers are released  */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmCtxtRelProcessor
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   WrUmmCtxtRelTrans         *ctxtRel = (&transCb->u.ctxtRelTrans);
   WrUeCb                    *ueCb = transCb->ueCb;
   switch(msg->msgTyp)
   {
      case WR_UMM_INTERNAL_MSG:
         {
            WrUmmInternalMsg       *internalMsg = msg->u.internalMsg;
            if(internalMsg->intMsgType == WR_UMM_INTMSG_REDIR_TRANS)
            {
               if(RFAILED == wrUmmRedirPrcIntMsg(transCb))
               {
                  RETVALUE(RFAILED);
               }
            }
         }
         break;
      case WR_UMM_LCL_CTXT_REL:
      case WR_UMM_S1AP_ERR_IND:
         RLOG_ARG0(L_INFO, DBG_CRNTI,ueCb->crnti,"context release because of "
            "LCL_CTXT_REL");
         RETVALUE(wrUmmUeLclCtxtRel(transCb));
         break;

      case WR_UMM_S1_DAT_IND:
         RLOG_ARG0(L_INFO, DBG_CRNTI,ueCb->crnti,"context release because of "
            "S1_DAT_IND");
         RETVALUE(wrUmmUeLclCtxtRel(transCb));
         break;

      case WR_UMM_S1_REL_IND:
      {        
         wrStopUeTmr(transCb->ueCb, WR_TMR_UE_CTX_REL_REQ);
         //wrStopUeTmr(transCb->ueCb, WR_TMR_RAD_RES_MOD_TMR);

#ifdef WR_DETECT_RLF
         wrStopUeTmr((WrUeCb *)transCb->ueCb, WR_TMR_DETECT_RLF);
#endif
         wrUmmCtxtRelTransInit(transCb);
        
         /* Fix for the CR ccpu00125615 : If Ue sends Detach Request as a part of 
         * Initial UE MSG,MME will send Ue context release command to eNB, 
         * at this point of time s1ap state will be in connecting mode and we
         * need to update the MME_UE_S1ap_id coming as a part of Ue context
         * release commnand in the ueCb */
         if(ueCb->s1ConCb->s1apConnState == WR_S1AP_CONNECTING)
         {
  
           wrUmmCtxtRelPrcParseUeS1apIdPair(transCb);
#ifdef WR_RSYS_KPI 
            if(!ueCb->s1LogicalConnEst)
            {
               WR_INC_KPI_FUNC(wrIncKpiFrS1SIGConnEstabSucc);
               ueCb->s1LogicalConnEst = TRUE;
            }
#endif/*WR_RSYS_KPI*/
         }

         RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Received UE CONTEXT RELEASE COMMAND"
                   "[MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
                   ueCb->s1ConCb->mme_ue_s1ap_id, ueCb->s1ConCb->enb_ue_s1ap_id);
         /* Fix for delayed UE COntext */
          wrUmmCtxtRelSndRlsCmpl(transCb);

        /* Update the Release Cause :  ccpu00126205  */
         wrUmmCtxtRelUpdateRelCause(transCb);

         if((transCb->ueCb->mobCtrlState == WR_HO_NORMAL) &&
           (transCb->ueCb->rrcConnState != WR_UMM_RRC_IDLE))
         {
            /* Send RRC connection release to UE */
            wrUmmCtxtRelSndRrcConRel(transCb);
         }
         else
         {
            /* ccpu00133711: T2200_HO_FIX: Need to release the UE from RRC even in case of 
             * state is UE_RELEASE. Else the context is not removed in case
             * of HO-Overall timer expires at soruce
             */
            if((transCb->ueCb->mobCtrlState == WR_HO_IN_PROGRESS) ||
               (transCb->ueCb->mobCtrlState == WR_HO_INITIATED)   ||
               (transCb->ueCb->mobCtrlState == WR_HO_UE_RELEASE))
            {
               wrIfmRrcRelUe(transCb->ueCb->cellId,transCb->ueCb->crnti);
            }
            /* send context release to Scheduler and L1 */
            wrUmmSendCtxtRelReq(transCb);
         }
         break;
      }
      case WR_UMM_RRC_DAT_CFM:
      {
#ifdef WR_TEST_CODE
         /* Return from here to verify the con rel time out action */
         if ( wrTrgRrcConRelTO == TRUE )
         {
            RLOG0(L_WARNING, "Discarding WR_UMM_RRC_DAT_CFM!!");
            RETVALUE(ROK);
         }
#endif
         /*Stop RRC Connection release  Timer*/
         wrStopTransTmr(transCb,WR_TMR_RRC_CONREL);
         /* UE should be deleted from all the other layers */
         if (transCb->msg->u.rrcDatCfm != NULLP)
         {
            if (transCb->msg->u.rrcDatCfm->datCfmStatus != NHU_ERRCAUSE_NONE)
            {
               if(wrIfmRrcRelUe(transCb->ueCb->cellId,transCb->ueCb->crnti) != ROK)
               {
                  RLOG0(L_ERROR, "Failure in release of UE at RRC");
               }
            }
         }
         if(transCb->msg->u.uuCfmPdu->hdr.transId & WR_UMM_UTRA_TDD_REDIR_REL_TRANSID)
         {
            /* Set the Release Cause as IRAT Redirection */
            transCb->errorCause = WR_UMM_IRAT_REDIR_REL_CAUSE;
            transCb->state = WR_UMM_TRANS_RELEASE_CALL;
         }
         else
         {
            wrUmmSendCtxtRelReq(transCb);
         }
         break;
      }
      case WR_UMM_DAM_UE_DEL_CFM:
      {
         /* Send UE Release Request to RRM                                */
         /* RRM is tightly coupled as of now, so the release should  */
         /* happen when the control comes back after the UeRelReq function */
         ctxtRel->rrmRespRcvd = FALSE;
         if(ROK != WrIfmRrmSendUeRelReq(transCb->ueCb->cellId,
                                        transCb->ueCb->crnti,
                                        transCb->transId))
         {
            RLOG_ARG1(L_ERROR, DBG_CRNTI,transCb->ueCb->crnti,"UE Release "
               "Request to RRM is Failed TransID:[%lu]",(transCb->transId));
         }
         /* Till the time DAM is tightly coupled, we should mark this     */
         /* when we exit from DelReq which is trigged when scheduler      */
         /* delete confirm is processed                                   */
         ctxtRel->damCfmRcvd = TRUE;
         /*Workaround as CL doesnt send DEL CFM in this platform */

         /*ctxtRel->rrmRespRcvd = TRUE;*/
         break;
      }
      case WR_UMM_RRM_UEREL_RESP:
      {
         /* Till the time RRM is tightly coupled, we should mark this     */
         /* when we exit from RelReq which is trigged UE DAM              */
         /* delete confirm is processed                                   */
         /*ctxtRel->rrmRespRcvd = FALSE;*/
         ctxtRel->rrmRespRcvd = TRUE;
         RLOG0(L_DEBUG, "UE Release Response Received from RRM");
         break;
      }
      case WR_UMM_SCHD_CFG:
      {
         ctxtRel->schdCfmRcvd = TRUE;
         /* Send DAM configuration to release the UE                      */
         /* When DAM is tightly coupled as it is now, the release should */
         /* happen when the control comes back after the DelReq function */
         ctxtRel->damCfmRcvd = FALSE;
         if (wrIfmDamUeDelReq(transCb->transId, transCb->ueCb->cellId,
                                                transCb->ueCb->crnti) != ROK)
         {
            RLOG0(L_ERROR, "Failure in release of UE at DAM");
         }
         break;
      }

      case WR_UMM_TMR_EXPIRY:
      {
         switch(transCb->msg->u.tmrExpiry.timerEvnt)
         { 
            case WR_TMR_RRC_CONREL:
               {
                  /* CELL_RESET_FIX: */
                  /* Delete the UE from RRC/RLC/MAC layers */
                  if (wrIfmRrcRelUe(transCb->ueCb->cellId, transCb->ueCb->crnti) != ROK)
                  {
                     RLOG0(L_ERROR, "Failure in release of UE at RRC");
                  }  
                  /* send context release to Scheduler and L1 */
                  wrUmmSendCtxtRelReq(transCb);
                  RETVALUE(ROK);
               }
            default:
               {
                  RLOG1(L_ERROR, "Received [%d] invalid Timer Event",
                     transCb->msg->u.tmrExpiry.timerEvnt);
                  RETVALUE(RFAILED);
               }
         }
      }
      break;

      case WR_UMM_PHY_CFG:
      {
         ctxtRel->l1CfmRcvd = TRUE;
         break;
      }
      default:
      {
         RLOG1(L_ERROR, "Unkonwn message[%d] received at local release "
            "processor", msg->msgTyp);
         break;
      }
   }
   if ((ctxtRel->l1CfmRcvd   == TRUE) &&
       (ctxtRel->damCfmRcvd  == TRUE) &&
       (ctxtRel->rrmRespRcvd == TRUE) &&
       (ctxtRel->schdCfmRcvd == TRUE))
   {
      /* Releasing call after getting the confirm we
       * are releasing the transaction */
      transCb->state = WR_UMM_TRANS_DELETE_UE;
      /*Inform about deletion of UE from eNB to SON*/
      wrIfmSonDelUeInd(transCb->ueCb->crnti, transCb->ueCb->cellId, 
         &transCb->ueCb->plmnId);
   }

   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmCtxtRelTransRel                                          */
/*                                                                        */
/* This function is local context release specific transaction release    */
/* function. There is nothing held by this transaction and so a simple    */
/* return function.                                                       */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmCtxtRelTransRel
(
WrUmmTransCb                 *transCb
)
{
   /* Stopping the RRC Connection Release timer */
   wrStopTransTmr(transCb,WR_TMR_RRC_CONREL);
   RETVALUE(ROK);
}

/** @brief This function is called when any message related to a particular
*         transaction during UE Context release transaction. 
*         During Context Release Transaction fail any other incoming transaction
*
* @details
*
*     Function: wrUmmCtxtRelNewTransProcessor 
*
*         Processing steps:
*
* @param[in]  transCb    : transaction information
* @param[in]  inctrans   : incoming transaction information
* @return S16
*    -#Success : WR_UMM_TRANS_FAIL
*/
PUBLIC S16 wrUmmCtxtRelNewTransProcessor 
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
    RLOG_ARG1(L_WARNING,DBG_CRNTI, transCb->ueCb->crnti,"Ignoring incoming Transaction Type"
          "[%d]",incTrans->transTyp);
    RETVALUE(WR_UMM_TRANS_FAIL);
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
