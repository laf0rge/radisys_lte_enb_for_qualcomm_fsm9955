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
               

     File:     wr_umm_dlnas.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=55;
static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_utils.h"
#include "wr_umm.h"
#include "wr_umm_trans.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_s1ap_msg.h"
#include "wr_ifm_rrc.h"
#include "wr_ifm_s1ap.h"
#include "wr_kpi.h"

#ifdef WR_TEST_CODE
EXTERN U8 wrTrgNasNonDlvrInd; 
#endif
PRIVATE S16 wrUmmDlNasSndNonDel
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   S1apPdu                   *s1Pdu;
   TknStrOSXL                *nasPdu = transCb->u.dlNasTrans.nasPdu;
   WrUmmMsgCause             cause;

   cause.causeTyp = CAUSE_RADIONW;
   cause.causeVal = SztCauseRadioNwradio_connection_with_ue_lostEnum;
   WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, cause.causeTyp, cause.causeVal, INC_KPI_VALUE_BY_ONE);
   /* Fix for CR ccpu00125862 */
   if(NULL != ueCb->s1ConCb)
   {
      if( ueCb->s1ConCb->s1apConnState == WR_S1AP_CONNECTED)
      {
         wrUmmS1apFillNasNonDlvInd(ueCb->s1ConCb, &s1Pdu, nasPdu, &cause);
         wrIfmS1apSndUeMsg(ueCb->s1ConCb->spConnId, s1Pdu);
      }
      RLOG_ARG2(L_EVENT,DBG_CRNTI,ueCb->crnti,"[S1AP]:Sending NAS NON DELIVERY"
            " INDICATION [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu] ",
            ueCb->s1ConCb->mme_ue_s1ap_id, ueCb->s1ConCb->enb_ue_s1ap_id);
      wrPrintS1apCauseInfo(cause.causeTyp, cause.causeVal);
   }
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmDlNasSndUuNas
(
WrUmmTransCb                 *transCb
)
{
   NhuDatReqSdus             *dlInfTfr;
   S1apPdu                   *pdu;
   SztInitiatingMsg          *initMsg;
   SztDlnkNASTport           *dlNas;
   SztNAS_PDU                *nasPdu = NULLP;
   U32                       idx;
   SztProtIE_Field_DlnkNASTport_IEs *ie;
   WrUeCb           *ueCb;
   TknU32                    *subScProfId;
   U8                         hrlRet = ROK;
   SztHovrRestrnLst           *sztHovrRestrnLst = NULLP;


   /* Init the DL NAS transaction */
   transCb->u.dlNasTrans.msg    = NULLP;
   transCb->u.dlNasTrans.nasPdu = NULLP;

   /* Get the instance of local copy of UECB*/
   ueCb = transCb->ueCb;

   if (transCb->msg->msgTyp == WR_UMM_S1_CON_CFM)
   {
      /* Update the ENB and MME logical S1AP Ids into the UE CB */
      TknU8                  *mmeIdTkn = NULLP; 
      TknU8                  *enbIdTkn = NULLP; 

      wrSzGetIE(&transCb->msg->u.cfmPdu->pdu->pdu, 
            Sztid_MME_UE_S1AP_ID, &mmeIdTkn);
      wrSzGetIE(&transCb->msg->u.cfmPdu->pdu->pdu, 
            Sztid_eNB_UE_S1AP_ID, &enbIdTkn);
      if (enbIdTkn == NULLP || mmeIdTkn == NULLP)
      {
         RLOG0(L_ERROR, "wrSzGetIE failed");
         RETVALUE(RFAILED);
      }
      RLOG2(L_INFO, "Received the CONFIRM MMEID[%lu] and ENBID[%lu]",
            ((TknU32 *)mmeIdTkn)->val, ((TknU32 *)enbIdTkn)->val);

      /* Update S1AP Con Cb */
      ueCb->s1ConCb->mme_ue_s1ap_id = ((TknU32 *)mmeIdTkn)->val;
      ueCb->s1ConCb->enb_ue_s1ap_id = ((TknU32 *)enbIdTkn)->val;
      ueCb->s1ConCb->spConnId = transCb->msg->u.cfmPdu->spConnId;
      ueCb->s1ConCb->s1apConnState = WR_S1AP_CONNECTED;

      /*Update the S1AP PDU*/ 
      pdu = transCb->msg->u.cfmPdu->pdu;
#ifdef WR_RSYS_KPI 
      if(!ueCb->s1LogicalConnEst)
      {
         WR_INC_KPI_FUNC(wrIncKpiFrS1SIGConnEstabSucc);
         ueCb->s1LogicalConnEst = TRUE;
      }
#endif/*WR_RSYS_KPI*/

   }
   else
   {
      pdu = transCb->msg->u.s1Pdu->pdu; 
   }

   /* Get the Handover Restriction List IE if its present and store the value 
    * in ueCb and process it.
    */
   wrSzGetIE(&pdu->pdu, 
         Sztid_HovrRestrnLst, (TknU8 **)&sztHovrRestrnLst);
   if(NULLP != sztHovrRestrnLst )
   {
      if(sztHovrRestrnLst->pres.pres)
      {
         if(transCb->ueCb->hoRstrctInfo == NULLP)
         {
            /* This is the first HRL, so populate ueCb->hrlMeasCfg 
             * with measUeCb for EUTRA and UTRA. */
            hrlRet = wrUmmPopltHrlMeasCfgDlNas(transCb->ueCb);
         }
         wrUmmCpyHoRestrnLst(transCb->ueCb,sztHovrRestrnLst);            
         if(ROK == hrlRet)
         {
            RLOG0(L_INFO, "wrUmmDlNasSndUuNas: Rcvd DL NAS, Reconfiguring the UE with Meas.");
            wrUmmMeasReCfgAftDlNas(transCb->ueCb);
         }
      }
   }

   /* Get the subscription profile ID IE if its present and store the value 
    * in ueCb
    */
   wrSzGetIE(&pdu->pdu, Sztid_SubscriberProfileIDforRFP, 
         (TknU8 **)(&subScProfId));
   if(NULLP != subScProfId)
   {
      ueCb->subscProfileId.pres = TRUE;
      ueCb->subscProfileId.val = subScProfId->val;
   }

   initMsg = &pdu->pdu.val.initiatingMsg;
   dlNas   = &initMsg->value.u.sztDlnkNASTport;

   ie = dlNas->protocolIEs.member;
   for(idx = 0; idx < dlNas->protocolIEs.noComp.val; idx++)
   {
      if (ie->id.val == Sztid_NAS_PDU)
      {
         nasPdu = &ie->value.u.sztNAS_PDU;
         break;
      }
      ie = ie + 1;
   }

   if (nasPdu == NULLP)
   {
      RLOG0(L_WARNING, "Missing NAS PDU in DL NAS TRANSFER");
      RETVALUE(ROK);
   }

   /* Build RRC message and send it out to the UE                         */
   /* TODO: Choose bearer based on whether security is complete           */
   WR_ALLOCEVNT(&dlInfTfr, sizeof(NhuDatReqSdus));
   dlInfTfr->hdr.cellId         = transCb->ueCb->cellId;
   dlInfTfr->hdr.ueId           = transCb->ueCb->crnti;
   dlInfTfr->hdr.transId        = transCb->transId;
   /* DL NAS should be sent in SRB2 , if SRB2 exists */
   /*Work around:  sending DL nas to SRB 1 since BRDCM security engine caulcte wrong 
     MAC-I for SRB2  and UE is doing re-estabslishment.*/
   if (transCb->ueCb->ueAttachedtoMme != TRUE)
   {
      wrUmmRrcFillDlInfTfr(dlInfTfr, nasPdu, WR_PDCP_ID_SRB1);
   }
   else
   {
      wrUmmRrcFillDlInfTfr(dlInfTfr, nasPdu, WR_PDCP_ID_SRB2);
   }

   RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti, "[S1AP]:Received DOWNLINK NAS TRANSPORT"
         "[MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
         transCb->ueCb->s1ConCb->mme_ue_s1ap_id,
         transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
   /* Call the function to dispatch the message over to RRC               */
   RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti, "[OTA] Sending "
         "DLInformationTransfer including DL NAS TRANSFER");
   if (wrIfmRrcSndReqToUe(dlInfTfr) != ROK)
   { 
      /*127802*/
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe in DL NAS TRANSFER failed");
      wrUmmTransComplete(transCb);
      RETVALUE(ROK);
   }

   /* Take a reference to NAS PDU in the DL NAS transaction CB so that    */
   /* NAS NON-DELIVERY may be filled using it                             */
   transCb->u.dlNasTrans.nasPdu = nasPdu;
   transCb->u.dlNasTrans.msg    = transCb->msg;
   transCb->msg->refCnt++;
   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmDlNasProcessor                                              */
/*                                                                        */
/* This function is called when a DL NAS TRANSPORT S1AP message is        */
/*    received from MME. The included NAS pdu is then transmitted to      */
/*    UE in DL information transfer RRC message.                          */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmDlNasProcessor
(
WrUmmTransCb                 *transCb
)
{
   switch(transCb->msg->msgTyp)
   {
      case WR_UMM_S1_DAT_IND:
      case WR_UMM_S1_CON_CFM:
      {
         wrUmmDlNasSndUuNas(transCb);
         break;
      }
      case WR_UMM_RRC_DAT_CFM:
      {
#ifdef WR_TEST_CODE
/* Added to test Nas Non delivery Ind message delivery 
   to the MME */
         if ( wrTrgNasNonDlvrInd == TRUE )
         {
            RLOG0(L_WARNING, "Dropping DL NAS Message");
            wrUmmDlNasSndNonDel(transCb);
            RETVALUE(ROK);
         }
#endif
         if (transCb->msg->u.rrcDatCfm->datCfmStatus != NHU_ERRCAUSE_NONE)
         {
            RLOG_ARG1(L_INFO, DBG_CRNTI,transCb->ueCb->crnti,"DL NAS RLC "
               "TRANSFER failed causeValue[%d] and sending NAS Non Delivery "
               "Indication to MME", transCb->msg->u.rrcDatCfm->datCfmStatus);
            wrUmmDlNasSndNonDel(transCb);
         }
         wrUmmTransComplete(transCb);
         break;
      }
      default:
      {
         RLOG1(L_ERROR,"Unexpected message [%d] received",transCb->msg->msgTyp);
         RETVALUE(ROK);
      }
   }

   RETVALUE(ROK);
}

/** @brief This function is called to process any new incoming message 
 *         when DL NAS transport transaction is on going and to determine
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmDlNasNewTransProcessor
 *
 *         Processing steps:
 *         - if context release transaction is created when DL NAS transaction
 *           is on going;
 *            - store the incoming message and release the transaction
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  inctrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDlNasNewTransProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
   S16 ret = WR_UMM_TRANS_OK;
#if 0/*arjun*/
   switch(incTrans->transTyp)
   {
      case WR_UMM_CTXT_REL_TRANS:
      {
         RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,
               "Deferring incoming UE Context Release transaction");
         /* storing the message and fail (release) the incoming transaction */
         WR_UMM_CPY_DEFR_MSG_TO_TRANSCB(transCb, incTrans->msg);
         ret = WR_UMM_TRANS_FAIL;
         break;
      }

      default:
      {
         RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,
               "Ignoring incoming transaction type [%d]", incTrans->transTyp);
         break;
      } 
   } 
#endif  
   RETVALUE(ret);
}

/** @brief This function is DL NAS transaction specific release function. 
 *
 * @details
 *
 *     Function: wrUmmDlNasTransRel
 *
 *         Processing steps:
 *
 * @param[in] transCb: Transaction Cb
 * @return S16
 *        -# Success : ROK
 */
PUBLIC S16 wrUmmDlNasTransRel
(
WrUmmTransCb                 *transCb
)
{
   if (transCb->u.dlNasTrans.msg != NULLP)
   {
     wrUmmRlsIncMsg(&transCb->u.dlNasTrans.msg);
   }
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
