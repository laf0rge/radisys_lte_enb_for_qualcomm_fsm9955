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
               

     File:     wr_umm_x2srcho.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/

static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_MODULE_ID=32;
static int RLOG_FILE_ID=124;

#include "wr.h"
#include "wr_umm.h"
#include "wr_emm.h"
#include "wr_umm_trans.h"
#include "wr_utils.h"
#include "wr_ifm_x2ap.h"
#include "wr_umm_ho.h"
#include "wr_umm_x2srcho.h"
#include "wr_ifm_rrc.h"
#include "wr_umm_x2ap_msg.h"
#include "wr_ifm_dam.h"
#include "wr_dam.h"
#include "wr_kpi.h"
#include "wr_umm_l1_msg.h"
#include "wr_umm_sch_msg.h"
#include "wr_ifm_rrm.h"
#include "wr_umm_rrc_msg.h"
#include "wr_ifm_l1.h"
#include "wr_umm_drx.h"
#include "wr_umm_s1ap_msg.h"
EXTERN S16 wrEmmBuildHoRestrnLst
(
WrUeCb                *ueCb,
CztEvnt               *hoReq,
CztHovrRestrnLst      *hoRestrnLst
);
/* UE History info - start */
EXTERN S16 wrEmmGetCellSizeType
(
WrCellId                     cellId,
U8                           *cellSizeType
);
/* UE History info - end */
PRIVATE S16 wrUmmX2SrcHoSendStartPdcpDataFwdReq(WrUmmTransCb  *transCb);
PUBLIC S16 wrPlatX2HoGetKeNBStar(WrUmmTransCb *transCb);
/**
 * @brief 
 *    This function is used to fill PLMN Identifier.
 *
 * @details
 *    Function: wrUmmX2SrcHoFillPLMNId
 *
 *    Processing steps:
 *       -# Fill PLMN Identifier
 * @param[in]
 *    pdu       : Event Pdu
 * @param[in] 
 *    plmnId    : plmnId
 * @param[in]
 *    plmnIe    : plmn IE
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoFillPLMNId
(
CztEvnt                      *pdu,
WrPlmnId                     *plmnId,
CztPLMN_Identity             *plmnIe
)
{
   U8      pLMNId[3];

   WR_GET_MEM(pdu, WR_PLMNID_IE_LEN, &(plmnIe->val));
   plmnIe->pres = PRSNT_NODEF;
   plmnIe->len  = WR_PLMNID_IE_LEN;

   pLMNId[0] =(((plmnId->mcc[1])<<4) | (plmnId->mcc[0]));
   if (plmnId->numMncDigits == 2)
   {
      pLMNId[1] =((0xf0) | (plmnId->mcc[2]));
      pLMNId[2] =(((plmnId->mnc[1])<<4) | (plmnId->mnc[0]));
   }
   else
   {
      pLMNId[1] =(((plmnId->mnc[0])<<4) | (plmnId->mcc[2]));
      pLMNId[2] =(((plmnId->mnc[2])<<4) | (plmnId->mnc[1]));
   }
   wrFillTknStrOSXL(plmnIe, WR_PLMNID_IE_LEN, pLMNId, &pdu->memCp);
   RETVALUE(ROK);
}

/* ccpu00130220 */
/**
 * @brief 
 *    This function is used to process X2AP Status Indication
 *
 * @details
 *    Function: wrUmmX2SrcPrcX2StaInd
 *
 *    Processing steps:
 *       -# If staInd is for type prepration timer expiry then
 *          -# reset UE states and release the transaction
 *       -# If staInd is for type overall timer expiry then
 *          -# send UE Context Release Request to MME
 *
 * @param[in]
 *    transCb   : Transaction Information
 *
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcPrcX2StaInd
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   CztStaInd                 *cztStaInd = msg->u.cztStaInd;
   WrUeCb                    *ueCb = transCb->ueCb;

   if(cztStaInd->status == CZT_STATUS_UE_TRELOCPREP_EXPIRY)
   {
      WR_UMM_X2_SRC_HANDLE_HO_FAIL(transCb);
      wrUmmTransComplete(transCb);
   }
   else if(cztStaInd->status == CZT_STATUS_UE_TX2RELOCOVERALL_EXPIRY)
   {
      ueCb->mobCtrlState = WR_HO_UE_RELEASE;
      if (ROK != wrUmmSndCtxtRelReq(ueCb->crnti,
               ueCb->cellId, WR_ENB_INIT_UE_REL))
      {
         RLOG0(L_ERROR, " FAiled To Send Ue Context Release Message ");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}
/* ccpu00130220 */

/**
 * @brief 
 *    This function is used to process UE Context Release Message.
 *
 * @details
 *    Function: wrUmmX2SrcHoPrcUeCntxtRls
 *
 *    Processing steps:
 *       -# Ensure that eNB received correct eNB UE S1ap Id/MME UE S1ap Id or not
 * @param[in]
 *    transCb   : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoPrcUeCntxtRls
(
WrUmmTransCb                 *transCb
)
{
   CztX2AP_PDU                       *x2apPdu = NULLP;
   CztInitiatingMsg                  *initiatingMsg = NULLP;
   CztProtIE_Field_UECntxtRls_IEs    *ie = NULLP;
   WrUeCb                            *ueCb = NULLP;
   U8                                idx = 0;
   U8                                noComp = 0;

   /**
    * Get the UE Control Block.
    */
   ueCb = transCb->ueCb;
   if (NULLP == ueCb)
   {
      RLOG0(L_ERROR, " UeCb is NULL.");
      RETVALUE(RFAILED);
   }
   x2apPdu = &transCb->msg->u.x2Pdu->pdu;
   initiatingMsg = &x2apPdu->val.initiatingMsg;
   noComp = initiatingMsg->value.u.cztUECntxtRls.protocolIEs.noComp.val;

   for (idx = 0; idx < noComp; idx++)
   {
      ie = &initiatingMsg->value.u.cztUECntxtRls.protocolIEs.member[idx];
      switch (ie->id.val)
      {
         case Cztid_Old_eNB_UE_X2AP_ID:
            {
               if (transCb->u.x2SrcHoTrans.old_eNB_UE_X2ap_Id != ie->value.u.cztid_Old_eNB_UE_X2AP_ID.val)
               {
                  RLOG0(L_ERROR, " Invalid old_eNB_UE_X2ap_Id. ");
                  RETVALUE(RFAILED);
               }
               break;
            }
         case Cztid_New_eNB_UE_X2AP_ID:
            {
               if (transCb->u.x2SrcHoTrans.new_eNB_UE_X2ap_Id != ie->value.u.cztid_New_eNB_UE_X2AP_ID.val)
               {
                  RLOG0(L_ERROR, " Invalid new_eNB_UE_X2ap_Id. ");
                  RETVALUE(RFAILED);
               }/*End of Cztid_New_eNB_UE_X2AP_ID*/
               break;
            }
         default:
            RLOG0(L_ERROR, " Invalid IE is present. ");
            RETVALUE(RFAILED);
      }/* End of Switch*/
   }/* End of For Loop.*/

   /**
    * Steps involved in local context release:                               
    *    1. Configure RRC to send out RRC release and also release resources 
    *       in MAC, RLC/PDCP once the release PDU is successfully delivered  
    *       to the UE.                                                       
    *    2. Upon receiving configuration confirm from RRC, configure the     
    *       Data application module and the scheduler to release the         
    *       resources.                                                       
    *                                                                        
    * Failure:                                                               
    *    There is provision for failure in release procedure. If RRC does    
    *    respond to release, after a timeout, the other layers are released
    */
   WR_INC_KPI_FUNC5(wrIncKpiFrHovrSuccd, WR_HO_TYPE_X2,LTE_X2_HOV,OUT_HOV,
         &(transCb->u.x2SrcHoTrans.cztCause),&(transCb->u.x2SrcHoTrans.ecgi));

   /* Successful HO Cntrs based on Intra & Inter Freq HO */
   WR_INC_KPI_FUNC2(wrIncKpiFrOutHoSuccdBsdOnFrqTyp, transCb->ueCb->freqTyp,
         transCb->ueCb->measGapUsd);

   /*Increment KPI for handover based on Drx Cycle used or not*/
   WR_INC_KPI_FUNC1(wrIncKpiFrHONoDrxOutSuccd,
         transCb->ueCb->drxCfg.isDrxEnabled);

   /* If eNodeB is connected to HeNB-GW, trigger the UE CONTEXT RELEASE 
    * REQUEST message to HeNB-GW by setting the Gw Context Release Indication
    * IE set to TRUE. We assume that eNodeB is connected to HeNB-GW if the 
    * MME-UE-S1AP-ID-2 is received from MME during UE attach
    */
   if((NULLP != ueCb->s1ConCb) && (TRUE == ueCb->s1ConCb->mmeUeS1apId2.pres))
   {
      /* Calling the common release function and during filling, we are 
       * filling the additional IE based on the HO state and HO Type.
       */
      transCb->u.x2SrcHoTrans.henbGwCtxtRelRequired = TRUE;
      wrUmmSndCtxtRelReq(ueCb->crnti, ueCb->cellId, WR_UMM_REL_CAUSE_SUCES_HO);

      /* As we are not expecting any response from HeNB-GW for this release 
       * request message, stopping the timer started 
       */
      wrStopUeTmr(ueCb, WR_TMR_UE_CTX_REL_REQ);
   }

   wrUmmPrcLclCtxtRel(ueCb,NhuRlsCauseotherEnum);
   wrUmmTransComplete(transCb);


   RETVALUE(ROK);
}/* End of wrUmmX2SrcHoPrcUeCntxtRls*/

/**
 * @brief 
 *    This function is used to Filling of Erab Not Admited List.
 *
 * @details
 *    Function: wrUmmX2SrcHoFillErabNotAdmitdLst
 *
 *    Processing steps:
 *       -# 
 * @param[in]
 *    transCb      : Transaction Information
 * @param[in]
 *    cztE_RAB_Lst : E-Rab not admitted list
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoFillErabNotAdmitdLst
(
WrUmmTransCb                 *transCb,
CztE_RAB_Lst                 *cztE_RAB_Lst
)
{
   CztProtIE_Single_Cont_E_RAB_ItemIEs  *transERabItemIE = NULLP;
   CztProtIE_Single_Cont_E_RAB_ItemIEs  *eRabItemIE = NULLP;
   U16                                  noComp = 0;
   U16                                  eRabItemIdx = 0;

   WR_ALLOC(&transCb->u.x2SrcHoTrans.cztE_RAB_Lst, sizeof(CztE_RAB_Lst));
   if(NULLP == transCb->u.x2SrcHoTrans.cztE_RAB_Lst)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /*ccpu00127635*/
   noComp = cztE_RAB_Lst->noComp.val;
   /*ccpu00127635*/

   transCb->u.x2SrcHoTrans.cztE_RAB_Lst->noComp.val = cztE_RAB_Lst->noComp.val;
   WR_ALLOC(&transCb->u.x2SrcHoTrans.cztE_RAB_Lst->member, 
         noComp * sizeof(CztProtIE_Single_Cont_E_RAB_ItemIEs));

   /* Fix for ccpu00138189 */ 
   if(NULLP == transCb->u.x2SrcHoTrans.cztE_RAB_Lst->member)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   for (eRabItemIdx =0; eRabItemIdx < noComp; eRabItemIdx++)
   {
      transERabItemIE = &transCb->u.x2SrcHoTrans.cztE_RAB_Lst->member[eRabItemIdx];
      eRabItemIE = &cztE_RAB_Lst->member[eRabItemIdx];
      transERabItemIE->value.u.cztE_RAB_Item.pres.val = PRSNT_NODEF;
      transERabItemIE->value.u.cztE_RAB_Item.e_RAB_ID.pres = PRSNT_NODEF;
      transERabItemIE->value.u.cztE_RAB_Item.e_RAB_ID.val = 
         eRabItemIE->value.u.cztE_RAB_Item.e_RAB_ID.val;
      transERabItemIE->value.u.cztE_RAB_Item.cause.choice.val = 
         eRabItemIE->value.u.cztE_RAB_Item.cause.choice.val;
      switch (transERabItemIE->value.u.cztE_RAB_Item.cause.choice.val)
      {
         case WR_CAUSE_RADIONW:
            transERabItemIE->value.u.cztE_RAB_Item.cause.val.radioNw.pres = PRSNT_NODEF;
            transERabItemIE->value.u.cztE_RAB_Item.cause.val.radioNw.val = 
               eRabItemIE->value.u.cztE_RAB_Item.cause.val.radioNw.val;
            break;
         case WR_CAUSE_TRANSPORT:
            transERabItemIE->value.u.cztE_RAB_Item.cause.val.transport.pres = PRSNT_NODEF;
            transERabItemIE->value.u.cztE_RAB_Item.cause.val.transport.val = 
               eRabItemIE->value.u.cztE_RAB_Item.cause.val.transport.val;
            break;
         case WR_CAUSE_PROTOCOL:
            transERabItemIE->value.u.cztE_RAB_Item.cause.val.protocol.pres = PRSNT_NODEF;
            transERabItemIE->value.u.cztE_RAB_Item.cause.val.protocol.val = 
               eRabItemIE->value.u.cztE_RAB_Item.cause.val.protocol.val;
            break;
         case WR_CAUSE_MISC:
            transERabItemIE->value.u.cztE_RAB_Item.cause.val.misc.pres = PRSNT_NODEF;
            transERabItemIE->value.u.cztE_RAB_Item.cause.val.misc.val = 
               eRabItemIE->value.u.cztE_RAB_Item.cause.val.misc.val;
            break;
         default:
            RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to Fill E-RAB information into transaction control
 *    block from Handover Request Ack message.
 *
 * @details
 *    Function: wrUmmX2SrcHoFillErabFrmHoAck
 *
 *    Processing steps:
 *       -# Parse E-RAB information contains in Handover Request Ack and fill
 *          transaction control block.
 * @param[in]
 *    transCb   : Transaction Information
 * @param[in]
 *    eRABLst   : E-Rab information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoFillErabFrmHoAck
(
WrUmmTransCb                 *transCb,
CztE_RABs_Admtd_Lst          *eRABLst
)
{
   WrUmmX2SrcHoTrans         *x2HoTrans = &transCb->u.x2SrcHoTrans;
   WrUeCb                    *ueCb = transCb->ueCb;
   U8                        rbIdx = 0;
   WrUmmX2SrcHoRabInfo       *fwdRbInfo;
   CztE_RABs_Admtd_Item      *eRABItem = NULLP;
   CztGTPtunnelEndpoint      *tnlInfo = NULLP;
   WrUeRAB                   *ueRbInfo = NULLP;

   x2HoTrans->numOfRbInfo = eRABLst->noComp.val;

   WR_ALLOC(&x2HoTrans->rbInfo,
         x2HoTrans->numOfRbInfo * sizeof(WrUmmX2SrcHoRabInfo));
   if(x2HoTrans->rbInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /**
    * fill the FWD tunnel information from the ERAB list
    */
   for(rbIdx = 0; rbIdx < eRABLst->noComp.val; rbIdx++)
   {
      eRABItem = &eRABLst->member[rbIdx].value.u.cztE_RABs_Admtd_Item;
      ueRbInfo = &ueCb->rbInfo.rab[eRABItem->e_RAB_ID.val-WR_UMM_ERAB_OFFSET];
      if(ueRbInfo->eRABId == eRABItem->e_RAB_ID.val)
      {
         if(ueRbInfo->isDataFwdAvl == FALSE)
         {
            break;
         }
      }
      else
      {
         WR_FREE(x2HoTrans->rbInfo,
               x2HoTrans->numOfRbInfo * sizeof(WrUmmX2SrcHoRabInfo));
         x2HoTrans->numOfRbInfo = 0;
         RETVALUE(RFAILED);
      }
      fwdRbInfo = &(x2HoTrans->rbInfo[rbIdx]);
      /**
       * Initialize the fwd information
       */
      fwdRbInfo->erabId    = ueRbInfo->eRABId;
      fwdRbInfo->rbId      = ueRbInfo->rbId;
      fwdRbInfo->rbDir     = ueRbInfo->rbDir;
      fwdRbInfo->qci       = ueRbInfo->qci;
      fwdRbInfo->dlFwding  = TRUE;

      /**
       * Copy DL Tunnel info
       */
      tnlInfo = &(eRABItem->dL_GTP_TunnelEndpoint);
      if(tnlInfo->pres.pres == PRSNT_NODEF)
      {
         /**
          * store the dl fwd tunnel id
          */
         wrX2apGetTnlId(&fwdRbInfo->datFwdDlRemTunId, &tnlInfo->gTP_TEID);
         /**
          * store the Peer IP Address
          */
         wrX2apGetTnlAddr(&fwdRbInfo->peerAddr, &tnlInfo->transportLyrAddr);
      }

      /**
       * Copy UL Tunnel info
       */
      tnlInfo = &(eRABItem->uL_GTP_TunnelEndpoint);
      if(tnlInfo->pres.pres == PRSNT_NODEF)
      {
         /**
          * store the ul fwd tunnel id
          */
         wrX2apGetTnlId(&fwdRbInfo->datFwdUlRemTunId, &tnlInfo->gTP_TEID);
         fwdRbInfo->isULDatFwdReq = TRUE;
      }
   }/* End of for() */

   RETVALUE(ROK);
} /* end of veFillErabFrmX2HoAck */

/**
 * @brief 
 *    This function is used to parse Handover Request Ack Message and fill into
 *    transaction control block.
 *
 * @details
 *    Function: wrUmmX2SrcHoParseHoReqAck
 *
 *    Processing steps:
 *       -# Parse Handover Request Ack and fill transaction control block.
 * @param[in]
 *    transCb   : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoParseHoReqAck
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2SrcHoTrans                 *x2SrcHoTrans = &transCb->u.x2SrcHoTrans;
   CztX2AP_PDU                       *x2apPdu = NULLP;
   CztSuccessfulOutcome              *successfulOutcome = NULLP;
   CztProtIE_Field_HovrRqstAckg_IEs  *hovrReqIE = NULLP;
   U8                                hovrReqIdx = 0;
   U8                                noComp;

   x2SrcHoTrans->msg = transCb->msg;
   /* ccpu00149336 */
   //transCb->msg->refCnt++;
   /* ccpu00149336 */

   x2apPdu = &transCb->msg->u.x2Pdu->pdu;
   successfulOutcome = &x2apPdu->val.successfulOutcome;
   noComp = successfulOutcome->value.u.cztHovrRqstAckg.protocolIEs.noComp.val;
   for (hovrReqIdx = 0; hovrReqIdx < noComp; hovrReqIdx++)
   {
      hovrReqIE = &successfulOutcome->value.u.cztHovrRqstAckg.protocolIEs.member[hovrReqIdx];
      switch (hovrReqIE->id.val)
      {
         case Cztid_New_eNB_UE_X2AP_ID:
         {
            x2SrcHoTrans->new_eNB_UE_X2ap_Id = hovrReqIE->value.u.cztid_New_eNB_UE_X2AP_ID.val;
            /* sri - HOFIX */
            transCb->ueCb->x2ConCb.newEnbUeX2apId = x2SrcHoTrans->new_eNB_UE_X2ap_Id;
            break;
         }
         case Cztid_E_RABs_Admtd_Lst:
         {
            if (ROK != wrUmmX2SrcHoFillErabFrmHoAck(transCb, &hovrReqIE->value.u.cztE_RABs_Admtd_Lst))
            {
               RLOG0(L_ERROR, " Failed To Fill ERAB Admited list.");
               RETVALUE(RFAILED);
            }
            break;
         }
         case Cztid_E_RABs_NotAdmtd_Lst:
         {
            if (ROK != wrUmmX2SrcHoFillErabNotAdmitdLst(transCb, &hovrReqIE->value.u.cztE_RAB_Lst))
            {
               RLOG0(L_ERROR, " Failed To Fill ERAB Admited list.");
               RETVALUE(RFAILED);
            }
            break;
         }
         case Cztid_TgeteNBtoSrc_eNBTprntCont:
         {
            /**
             * Store the Target to Src Transparent Container 
             * into Transaction Control Block.
             */
            x2SrcHoTrans->tgetToSrcTprntCont = &hovrReqIE->value.u.cztTgeteNBtoSrc_eNBTprntCont;
            break;
         }
      }/* End of Switch.*/
   }/* End of for loop.*/
   RETVALUE(ROK);
}/* End of wrUmmX2SrcHoParseHoReqAck*/

/**
 * @brief 
 *    This function is used to fill Nhu Data Req Event.
 *
 * @details
 *    Function: wrUmmX2SrcHoParseHoReqAck
 *
 *    Processing steps:
 *       -# Parse Handover Request Ack and fill transaction control block.
 * @param[in]
 *    transCb         : Transaction Information
 * @param[in]
 *    nhDatReqEvntSdu : Event Structure
 * @param[in]
 *    mBuf            : to be filled in event structure
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoFillNhuRrcConReCfgnWithMbuf
(
WrUmmTransCb                 *transCb,
NhuDatReqSdus                *nhDatReqEvntSdu,
Buffer                       *mBuf
)
{
   S16 ret = ROK;

   /**
    * Fill HDR Info
    */ 
   nhDatReqEvntSdu->hdr.cellId = transCb->u.x2SrcHoTrans.cellId;
   nhDatReqEvntSdu->hdr.ueId = transCb->u.x2SrcHoTrans.crnti;
   nhDatReqEvntSdu->hdr.transId = transCb->transId | C1_RRCCONRECFGN;
   /**
    * Populating NHU Dat Req
    */
   nhDatReqEvntSdu->sdu.ueMsg.isMsgDed = TRUE;
   nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.pdcpId = WR_PDCP_ID_SRB1;
   nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.isMsgEncoded = TRUE;
   nhDatReqEvntSdu->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.mBuf = mBuf;
   /**
    * Fill Config values
    */
   nhDatReqEvntSdu->sdu.isUeCfgPres = FALSE;
   
   RETVALUE(ret);
} /* End of veFillNhuRrcConReCfgnWithMbuf */


/**
 * @brief 
 *    This function is used to get PDCP Status
 *
 * @details
 *    Function: wrUmmX2SrcHoGetPdcpSduSta
 *
 *    Processing steps:
 *       -# Fill Header information
 *       -# Send PDCP Status Request
 *                     
 * @param[in]
 *    transCb   : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2SrcHoGetPdcpSduSta
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   NhuHdr                    nhuHdr;

   nhuHdr.cellId = ueCb->cellId;
   nhuHdr.ueId = ueCb->crnti;
   nhuHdr.transId = transCb->transId;
   nhuHdr.isUlDatFwdReq = FALSE;

   WrIfmRrcGetPdcpSduSta(nhuHdr);
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to process Handover Decode confirm from RRC.
 *
 * @details
 *    Function: wrUmmX2SrcHdlRrcHoCmdDecCfm
 *
 *    Processing steps:
 *       -# Fill Nhu Dat Event Req information.
 *       -# Send the Handover command Message to the UE.
 * @param[in]
 *    transCb         : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHdlRrcHoCmdDecCfm
(
WrUmmTransCb                 *transCb
)
{
   NhuHovrCmd                      *hovrCmd = NULLP;
   NhuHovrCmd_r8_IEshandoverCmdMsg *hoCmdMsg = NULLP;
   Buffer                          *mBuf = NULLP;
   NhuDatReqSdus                   *nhuDatReqSdu = NULLP;

   if (transCb->msg->u.decCfm->status != NHU_DECODE_SUCCESS)
   {
      RLOG0(L_ERROR, " Failed to Decode. ");
      RETVALUE(RFAILED);
   }
   hovrCmd = &transCb->msg->u.decCfm->ie.hovrCmd;
   if(hovrCmd->pres.pres)
   {
      hoCmdMsg = &hovrCmd->criticalExtns.val.c1.val.handoverCmd_r8.handoverCmdMsg;

      /**
       * convert the OSXL to Buffer
       */
      WR_ALLOCMBUF((*(wrCb.nhuSap))->pst.region, wrCb.mem.pool,&mBuf );
      if(mBuf == NULLP)
      {
         RLOG0(L_FATAL, " Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      SAddPstMsgMult((Data *)hoCmdMsg->val,
            (MsgLen)hoCmdMsg->len, (Buffer *)mBuf);
#if 0 /*130053*/     
      if(transCb->ueCb->drxCfg.isDrxEnabled != TRUE) /* Pavan */
      {
#endif         
      /**
       * Send the RRC Connection Reconfiguration Msg
       * which is already in the encoded format to RRC so that it 
       * sends out to the UE.
       */
      WR_ALLOCEVNT(&nhuDatReqSdu, sizeof(NhuDatReqSdus));
      if(NULLP == nhuDatReqSdu)
      {
            RLOG0(L_FATAL,"Memory allocation failed.");
            /* ccpu00149336 */
            WR_FREEMBUF(mBuf);
            /* ccpu00149336 */
            RETVALUE(RFAILED);
      }

      if (ROK != wrUmmX2SrcHoFillNhuRrcConReCfgnWithMbuf(transCb, nhuDatReqSdu, mBuf))
      {
         RLOG0(L_ERROR, " Failed to Fill RRC Con Reconfig. ");
         /* ccpu00149336 */
         WR_FREEMBUF(mBuf);
         WR_FREE_EVNT(nhuDatReqSdu);
         /* ccpu00149336 */
         RETVALUE(RFAILED);
      }
      if(ROK != wrIfmRrcSndReqToUe(nhuDatReqSdu))
      {
         RLOG0(L_ERROR, " Failed to send Dat Req ");
         /* ccpu00149336 */
         WR_FREEMBUF(mBuf);
         WR_FREE_EVNT(nhuDatReqSdu);
         /* ccpu00149336 */
         RETVALUE(RFAILED);
      }

      /*Increment KPI for Intra-LTE Handovers Attempted outgoing Handover to eNodeB per Handover cause*/
      transCb->ueCb->hovDir = OUT_HOV;
#ifdef WR_RSYS_KPI 
      transCb->ueCb->hov = LTE_X2_HOV;
#endif/*WR_RSYS_KPI*/

      /* Increment KPI for Out going HO */
      WR_INC_KPI_FUNC5(wrIncKpiFrHovrAttmptd, WR_HO_TYPE_X2, LTE_X2_HOV,
            OUT_HOV,&(transCb->u.x2SrcHoTrans.cztCause),&(transCb->u.x2SrcHoTrans.ecgi));
      /*Increment KPI for Handover related measurements for DRX/non-DRX Attempted*/
      WR_INC_KPI_FUNC1(wrIncKpiFrHONoDrxOutAttmptd,transCb->ueCb->drxCfg.isDrxEnabled);

      WR_INC_KPI_FUNC2(wrIncKpiFrOutHoAtmptBsdOnFrqTyp,transCb->ueCb->freqTyp,transCb->ueCb->measGapUsd);
      
/* X2_HO Changes for Falcon */      
      /* Get PDCP SN Status, No L2 Reset Timer */
      wrUmmX2SrcHoGetPdcpSduSta(transCb);
#if 0 /*130053*/      
      }

      else
      {
         transCb->u.x2SrcHoTrans.hoCmd = mBuf;
         wrUmmX2SndDrxRelToUe(transCb); 
      }
#endif      
/* X2_HO Changes for Falcon */

   } /* end of if statement */
   RETVALUE(ROK);
}/* End of wrUmmX2SrcHdlRrcHoCmdDecCfm*/

/**
 * @brief 
 *    This function is used to Decode target to source transparent container.
 *
 * @details
 *    Function: wrUmmX2SrcHoDecTgtToSrcTransprntCon
 *
 *    Processing steps:
 *       -# Fill Decode request information
 *       -# Send Decode Request to the RRC
 * @param[in]
 *    transCb         : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoDecTgtToSrcTransprntCon
(
WrUmmTransCb                 *transCb
)
{
   NhuDecodeSduReq              *nhuDecodeSduReq = NULLP;
   Buffer                       *mBuf = NULLP;

   WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, &mBuf);
   if(mBuf == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /**
    * Request RRC to decode the Target_To_Src Trasparent Container
    */
   WR_ALLOC(&nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
   if(NULLP == nhuDecodeSduReq)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/
   nhuDecodeSduReq->hdr.cellId  =   transCb->ueCb->cellId;
   nhuDecodeSduReq->hdr.ueId    =   transCb->ueCb->crnti;
   nhuDecodeSduReq->hdr.transId =   transCb->transId | WR_UMM_SRC_HO_HOCMD_DEC;
   nhuDecodeSduReq->sduType     =   NHU_MSG_HOCMD;
   /**
    * convert the OSXL to Buffer
    */
   SAddPstMsgMult((Data *)transCb->u.x2SrcHoTrans.tgetToSrcTprntCont->val,
         (MsgLen)transCb->u.x2SrcHoTrans.tgetToSrcTprntCont->len,
         (Buffer *)mBuf);
   nhuDecodeSduReq->mbuf = mBuf;

   /**
    * Decode the HO Command.This HO Command is used to send the RRC
    * Re-configuration. Further processing shall be done at the receipt
    * of NhuDecodeCfm.
    */
   wrIfmRrcSndDecodeReq(nhuDecodeSduReq);
   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to Send Tunnel Create Request.
 *
 * @details
 *    Function: wrUmmX2SrcHoSndTnlCreateReq
 *
 *    Processing steps:
 *       -# Fill Tunnel Request information
 *       -# Send Tunnel Create Request to the egtp
 * @param[in]
 *    transId         : Transaction Id
 * @param[in]
 *    ueCb            : UE Control block
 * @param[in]
 *    rabInfo         : Rab Information
 * @param[in]
 *    peerId          : peer Id
 * @param[in]
 *    tnlType         : Tunnel Type
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoSndTnlCreateReq
(
U32                          transId,
WrUeCb                       *ueCb,
WrUmmX2SrcHoRabInfo          *rabInfo,
U32                          peerId,
WrDamTnlType                 tnlType
)
{
   WrDamTnlInfo              *tnlInfo;
   CmTptAddr                 *lclAddr;
 /*  CmTptAddr                 *peerAddr; */

   WR_ALLOC_SHAREBLE_BUF(&tnlInfo, sizeof(WrDamTnlInfo));
   if (NULLP == tnlInfo)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   tnlInfo->hoSrcEnodeb = TRUE;
   tnlInfo->transId = transId;
   tnlInfo->tnlId.cellId = ueCb->cellId;
   tnlInfo->tnlId.crnti = ueCb->crnti;
   tnlInfo->tnlId.drbId = rabInfo->rbId;
   tnlInfo->tnlType = tnlType;
   tnlInfo->hoType = WR_HO_TYPE_X2;
   wrCpyCmTptAddr(&tnlInfo->dstAddr, &rabInfo->peerAddr);
   wrEmmGetDatAppAddr(&lclAddr, rabInfo->peerAddr.type);
   wrCpyCmTptAddr(&tnlInfo->srcAddr, lclAddr);
   if(tnlType == WR_TNL_DL_DATFWD)
   {
      tnlInfo->remTeid = rabInfo->datFwdDlRemTunId;
   }
   else
   {
      tnlInfo->remTeid = rabInfo->datFwdUlRemTunId;
   }
   wrUtlGetDscp(ueCb->cellId, rabInfo->qci, &(tnlInfo->dscp));

   rabInfo->tnlCfgState = WR_UMM_SRC_HO_TNL_CFG_SENT;
   wrIfmDamTnlCreatReq(transId, tnlInfo);

   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to Configure tunnels.
 *
 * @details
 *    Function: wrUmmX2SrcHoCfgTnls
 *
 *    Processing steps:
 *       -# Based on rab Direction, trigger Tunnel Request
 *       -# Trigger wrIfmDamChngTnlState
 * @param[in]
 *    transCb         : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoCfgTnls
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2SrcHoTrans         *x2SrcHoTrans = &transCb->u.x2SrcHoTrans;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmX2SrcHoRabInfo       *rabInfo = NULLP;
   U32                       transId;
   U16                       idx;
   U16                       noOfRbs;
   U32                       peerId;
   U8                        rbMode;
   WrDamTnlStInfo            *tnlInfo;

   peerId = x2SrcHoTrans->peerId;

   noOfRbs = x2SrcHoTrans->numOfRbInfo;
   for (idx = 0; idx < noOfRbs; idx++)
   {
      rabInfo = &x2SrcHoTrans->rbInfo[idx];
      /**
       * TODO : temporarily added. need to remove when DAM becomes loosely
       *        coupled
       */
      WR_UMM_SRC_HO_CALC_NUM_TNL_CFG(rabInfo);
      transId = transCb->transId | idx;

      if(rabInfo->rbDir == WR_DIR_UL || 
            rabInfo->rbDir == WR_DIR_DL_UL)
      {
         /* UL Forwarding Tunnel shall be created only if Tgt requested for UL Data Forwarding */
         if(rabInfo->isULDatFwdReq == TRUE)
         {
         RLOG_ARG2(L_INFO,DBG_CRNTI,transCb->ueCb->crnti, 
               " Sending Tunnel Create Request for rbID [%d] tnlType [%d] ",
               rabInfo->rbId, WR_TNL_UL_DATFWD);

         wrUmmX2SrcHoSndTnlCreateReq(transId, ueCb, rabInfo, peerId, WR_TNL_UL_DATFWD);
      }
      }
      if(rabInfo->rbDir == WR_DIR_DL || 
            rabInfo->rbDir == WR_DIR_DL_UL)
      {
         RLOG_ARG2(L_INFO, DBG_CRNTI,transCb->ueCb->crnti,
               " Sending Tunnel Create Request for rbID [%d] tnlType [%d] ",
               rabInfo->rbId, WR_TNL_DL_DATFWD);
         wrUmmX2SrcHoSndTnlCreateReq(transId, ueCb, rabInfo, peerId, WR_TNL_DL_DATFWD);
      }

      wrUtlGetRbMode(ueCb->cellId, rabInfo->qci, &rbMode);
      if(rbMode == WR_RLC_UM_MODE)
      {
         WR_ALLOC_SHAREBLE_BUF(&tnlInfo, sizeof(WrDamTnlStInfo));
         if(tnlInfo == NULLP)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         tnlInfo->transId = 0;
         tnlInfo->state = WR_DAM_DRB_STATE_BUFFERING;
         tnlInfo->cause = WR_DAM_HO_UE_UM_DAT_FWD;
         tnlInfo->lclTeid = ueCb->rbInfo.rab[0].locEgtpTunId;
         wrIfmDamChngTnlState(tnlInfo);
      }
   }
   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to process DAM Confirm.
 *
 * @details
 *    Function: wrUmmX2SrcHoPrcDamCfm
 *
 *    Processing steps:
 *       -# Based on Tunnel Type, set the State
 *       
 * @param[in]
 *    transCb         : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoPrcDamCfm
(
WrUmmTransCb                  *transCb
)
{
   WrUmmX2SrcHoTrans         *x2SrcHoTrans = &transCb->u.x2SrcHoTrans;
   WrUmmX2SrcHoRabInfo       *rabInfo = NULLP;
   U32                       drbId;
   U8                        status;
   U8                        idx;
   U16                       noOfRbs;
   WrUmmDamCfgCfm            *damCfm;
   WrDamTnlType              tnlType;

   damCfm = &transCb->msg->u.damCfm;
   drbId   = (damCfm->lclTeId & 0x000000ff) >> 3;
   tnlType = (damCfm->lclTeId & 0x00000007);
   status  = damCfm->status;

   noOfRbs = x2SrcHoTrans->numOfRbInfo;
   for (idx = 0; idx < noOfRbs; idx++)
   {
      rabInfo = &x2SrcHoTrans->rbInfo[idx];
      if(rabInfo->rbId == drbId)
      {
         if(status == WR_DAM_CFG_OK)
         {
            switch(tnlType)
            {
               case WR_TNL_DL_DATFWD:
               {
                  rabInfo->tnlCfgState = WR_UMM_SRC_HO_TNL_CFG_SUCC;
                  rabInfo->datFwdDlLclTunId  = damCfm->lclTeId;
                  break;
               }
               case WR_TNL_UL_DATFWD:
               {
                  rabInfo->tnlCfgState = WR_UMM_SRC_HO_TNL_CFG_SUCC;
                  rabInfo->datFwdUlLclTunId  = damCfm->lclTeId;
                  break;
               }
               case WR_TNL_NORMAL:
               {
                  break;
               }
            }
         }
         else
         {
            rabInfo->tnlCfgState = WR_UMM_SRC_HO_TNL_CFG_FAIL;
         }
         rabInfo->expectedTnlRsp--;
         RLOG_ARG1(L_INFO, DBG_CRNTI,transCb->ueCb->crnti,
              " Received Tunnel Create Confirm for rbID [%d] ",rabInfo->rbId);
         RLOG2(L_DEBUG, " tnlType [%d] pendingResponse [%d] ", tnlType, rabInfo->expectedTnlRsp);
      }
   }
   for(idx = 0; idx < noOfRbs; idx++)
   {
      rabInfo = &x2SrcHoTrans->rbInfo[idx];
      if((rabInfo->tnlCfgState == WR_UMM_SRC_HO_TNL_CFG_SENT) ||
            (rabInfo->expectedTnlRsp != 0))
      {
         RETVALUE(ROK);
      }
   }
   x2SrcHoTrans->state = WR_UMM_SRC_HO_TNL_CFG_DONE;
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to process Handover Request Ack
 *
 * @details
 *    Function: wrUmmX2SrcHoPrcHoReqAck
 *
 *    Processing steps:
 *       -# Parse Handover Request Ack
 *       -# Decode target to source transparent container
 *       -# trigger Configure tunnel request
 *       
 * @param[in]
 *    transCb         : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoPrcHoReqAck
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2SrcHoTrans         *x2HoTrans = &transCb->u.x2SrcHoTrans;

   /* TODO Stop the Preparation Timer */   
   /* TODO Start the Overall Timer */

   /**
    * Parse Handover Request Ack Message.
    */
   if (ROK != wrUmmX2SrcHoParseHoReqAck(transCb))
   {
      RLOG0(L_ERROR, " Failed to parse Handover Request Ack Message. ");
      RETVALUE(RFAILED);
   }/* End of If Condition.*/
   RLOG_ARG2(L_EVENT,DBG_CRNTI,transCb->ueCb->crnti,
         "[X2AP]:Received HANDOVER REQUEST ACKNOWLEDGE [Old-eNB-UE-X2AP-ID:%d] "
         "[New-eNB-UE-X2AP-ID:%d]", transCb->u.x2SrcHoTrans.old_eNB_UE_X2ap_Id,
         transCb->u.x2SrcHoTrans.new_eNB_UE_X2ap_Id);
  
  /* State of Handover after receving Handover Preparation Ack */
   x2HoTrans->state = WR_HANDOVER_PREPARATION_ACK_RCVD;

   if (ROK != wrUmmX2SrcHoDecTgtToSrcTransprntCon(transCb))
   {
      RLOG0(L_ERROR, " Failed to decode Tgt To Src con. ");
      RETVALUE(RFAILED);
   }
   /**
    * configure fwd tunnels
    */
   wrUmmX2SrcHoCfgTnls(transCb);
   /**
    * change state of the ue
    */
   transCb->ueCb->mobCtrlState = WR_HO_IN_PROGRESS;

   RLOG0(L_INFO, " X2 Handover state = IN PROGRESS, Cause = Received HANDOVER REQUEST ACKNOWLEDGE ");

   /* ccpu00131919 - if LocRpt with RequestType set to report on change of serving cell, then Src should not send 
    * LocRpt since its a Femto with only 1 Cell
    * Src should fill LocationReporting IE in HovrReq */

   RETVALUE(ROK);
}/* End of wrUmmX2SrcHoPrcHoReqAck*/
/**
 * @brief 
 *    This function is used to fill Sn Status Transfer Message
 *
 * @details
 *    Function: wrUmmX2SrcHoFillSnStatusTrfr
 *
 *    Processing steps:
 *       -# Fill Sn Status transfer message.
 *       
 * @param[in]
 *    transCb         : Transaction Informatiion
 * @param[in]
 *    pdu             : Event Pdu
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoFillSnStatusTrfr
(
WrUmmTransCb                 *transCb,
CztEvnt                      **pdu
)
{
   NhuPdcpSduStaCfm          *pdcpSduCfm;

   CztEvnt                          *snStatusTfrPdu = NULLP;
   CztSNStatusTfr                   *snInfo;
   CztProtIE_Field_SNStatusTfr_IEs  *ie = NULLP;
   CztProtIE_Single_Cont_E_RABs_SubjToStatusTfr_ItemIEs  *ie1 = NULLP;
   CztE_RABs_SubjToStatusTfr_Item   *snTfrItem;
   WrUeRAB                          *rbInfo = NULLP;
   U16                              pdcpSn;
   U32                              pdcpHfn = 0;
   U8                               numComp = 0;
   U8                               compCnt = 0;
   U8                               idx;
   U8                               idx1;
   U8                               rbCnt = 0;
   U8                               numBearer = 0;

   /**
    * Initialize memory control point
    */
   if(ROK != cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE, &wrCb.mem, (Ptr *)&snStatusTfrPdu))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   pdcpSduCfm = transCb->msg->u.pdcpSduCfm;

   if(pdcpSduCfm->status != 0)
   {
      RLOG0(L_ERROR,"PDCP has sent negative Status.");
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(snStatusTfrPdu->pdu), 0, sizeof(CztX2AP_PDU));
   wrFillTknU8(&(snStatusTfrPdu->pdu.choice), X2AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(snStatusTfrPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   wrFillTknU32(&(snStatusTfrPdu->pdu.val.initiatingMsg.procedureCode),
         Cztid_snStatusTfr);
   wrFillTknU32(&(snStatusTfrPdu->pdu.val.initiatingMsg.criticality),
         CztCriticalityignoreEnum);
   numComp = 3;/* Mandatory Fields.*/
   snInfo = &snStatusTfrPdu->pdu.val.initiatingMsg.value.u.cztSNStatusTfr;
   wrFillTknU8(&(snInfo->pres),PRSNT_NODEF);
   wrFillTknU16(&(snInfo->protocolIEs.noComp),numComp);

   if ((cmGetMem(&snStatusTfrPdu->memCp, (numComp * sizeof(CztProtIE_Field_SNStatusTfr_IEs)),
               (Ptr *)&snInfo->protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /**
    * Fill the Old EnB UE X2AP ID
    */
   ie = &snInfo->protocolIEs.member[compCnt++];

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_Old_eNB_UE_X2AP_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.cztid_Old_eNB_UE_X2AP_ID), transCb->u.x2SrcHoTrans.old_eNB_UE_X2ap_Id);

   /**
    * Fill the New EnB UE X2AP ID
    */
   ie = &snInfo->protocolIEs.member[compCnt++];

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_New_eNB_UE_X2AP_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.cztid_New_eNB_UE_X2AP_ID), transCb->u.x2SrcHoTrans.new_eNB_UE_X2ap_Id);

   /**
    * Fill the E-RABs list
    */
   ie = &snInfo->protocolIEs.member[compCnt];

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_E_RABs_SubjToStatusTfr_Lst);
   wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   numBearer = pdcpSduCfm->numRbId;
   wrFillTknU16(&(ie->value.u.cztE_RABs_SubjToStatusTfr_Lst.noComp), numBearer);

   if ((cmGetMem(&snStatusTfrPdu->memCp,
               (numBearer * sizeof(CztProtIE_Field_E_RABs_SubjToStatusTfr_ItemIEs)),
               (Ptr *)&ie->value.u.cztE_RABs_SubjToStatusTfr_Lst.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   rbCnt = transCb->ueCb->rbInfo.numOfRbCfgd;
   rbInfo = transCb->ueCb->rbInfo.rab;
   for(idx1 = 0; idx1 < numBearer; idx1++)
   {
      ie1 = &ie->value.u.cztE_RABs_SubjToStatusTfr_Lst.member[idx1];
      snTfrItem = &ie1->value.u.cztE_RABs_SubjToStatusTfr_Item;

      wrFillTknU8(&(ie1->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie1->id), Cztid_E_RABs_SubjToStatusTfr_Item);
      wrFillTknU32(&(ie1->criticality), CztCriticalityignoreEnum);
      wrFillTknU8(&(snTfrItem->pres), PRSNT_NODEF);

      /**
       * Fill e_RAB_ID
       */
      for(idx = 0; idx < rbCnt; idx++)
      {
         if(pdcpSduCfm->pdcpSduStaInfo[idx1].drbId == rbInfo[idx].rbId)
         {
            wrFillTknU32(&(snTfrItem->e_RAB_ID), rbInfo[idx].eRABId);
         }
      }
      /* Check the SN bit */
      if(pdcpSduCfm->pdcpSduStaInfo[idx1].snLen == NHU_PDCP_SN_SIZE_12BITS)
      {
         /**
          * Fill the UL Count Value
          */
         wrFillTknU8(&(snTfrItem->uL_COUNTvalue.pres),PRSNT_NODEF);
         pdcpSn = (pdcpSduCfm->pdcpSduStaInfo[idx1].ulCountVal & (0x0fff));
         wrFillTknU32(&(snTfrItem->uL_COUNTvalue.pDCP_SN), pdcpSn);

         pdcpHfn = ((pdcpSduCfm->pdcpSduStaInfo[idx1].ulCountVal >> 12) & 0xfffff);
         wrFillTknU32(&(snTfrItem->uL_COUNTvalue.hFN), pdcpHfn);
         snTfrItem->uL_COUNTvalue.iE_Extns.noComp.pres = NOTPRSNT;

         /**
          * Fill the DL Count Value
          */
         wrFillTknU8(&(snTfrItem->dL_COUNTvalue.pres), PRSNT_NODEF);
         pdcpSn = (pdcpSduCfm->pdcpSduStaInfo[idx1].dlCountVal & (0x0fff));
         wrFillTknU32(&(snTfrItem->dL_COUNTvalue.pDCP_SN), pdcpSn);

         pdcpHfn = ((pdcpSduCfm->pdcpSduStaInfo[idx1].dlCountVal >> 12) & 0xfffff);
         wrFillTknU32(&(snTfrItem->dL_COUNTvalue.hFN), pdcpHfn);
         snTfrItem->dL_COUNTvalue.iE_Extns.noComp.pres = NOTPRSNT;

         /* Fill receive status of UL SDU's */
         if((pdcpSduCfm->pdcpSduStaInfo[idx1].rcvStaOfUlPdcpSdus.numBits != 0) &&
               (transCb->u.x2SrcHoTrans.rbInfo[idx1].isULDatFwdReq == TRUE))
         {
            wrFillTknStr4096BSXL(&(snTfrItem->receiveStatusofULPDCPSDUs),
                  pdcpSduCfm->pdcpSduStaInfo[idx1].rcvStaOfUlPdcpSdus.numBits,
                  pdcpSduCfm->pdcpSduStaInfo[idx1].rcvStaOfUlPdcpSdus.ulBitMap,
                  &snStatusTfrPdu->memCp);
         }
      }
      else if(pdcpSduCfm->pdcpSduStaInfo[idx1].snLen == NHU_PDCP_SN_SIZE_15BITS)
      {
         CztProtExtnCont_E_RABs_SubjToStatusTfr_ItemExtIEs *extIEs;
         CztProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEs *newIE;
         /* Fill the UL Count Value */
         wrFillTknU8(&(snTfrItem->uL_COUNTvalue.pres),PRSNT_NODEF);
         wrFillTknU32(&(snTfrItem->uL_COUNTvalue.pDCP_SN), 0);
         wrFillTknU32(&(snTfrItem->uL_COUNTvalue.hFN), 0);
         snTfrItem->uL_COUNTvalue.iE_Extns.noComp.pres = NOTPRSNT;

         /* Fill the DL Count Value  */
         wrFillTknU8(&(snTfrItem->dL_COUNTvalue.pres), PRSNT_NODEF);
         wrFillTknU32(&(snTfrItem->dL_COUNTvalue.pDCP_SN), 0);
         wrFillTknU32(&(snTfrItem->dL_COUNTvalue.hFN), 0);
         snTfrItem->dL_COUNTvalue.iE_Extns.noComp.pres = NOTPRSNT;

         /* Fill Extended IEs */
         extIEs = &snTfrItem->iE_Extns;
         extIEs->noComp.pres = PRSNT_NODEF;
         extIEs->noComp.val = 3;

         if ((cmGetMem(&snStatusTfrPdu->memCp, 
                     (extIEs->noComp.val * sizeof(CztProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEs)), 
                     (Ptr *)&extIEs->member)) !=ROK)
         {
            RLOG0(L_FATAL, "Failed to allocate memory.cmGetMem Failed");
            WR_FREE_EVNT(snStatusTfrPdu);
            RETVALUE(RFAILED);
         }
         /* Fill receive status of UL SDU's */
         newIE = &extIEs->member[0];
         if((pdcpSduCfm->pdcpSduStaInfo[idx1].rcvStaOfUlPdcpSdus.numBits != 0) &&
               (transCb->u.x2SrcHoTrans.rbInfo[idx1].isULDatFwdReq == TRUE))
         {
            wrFillTknU8(&(newIE->pres),PRSNT_NODEF);
            wrFillTknU32(&(newIE->id), Cztid_ReceiveStatusOfULPDCPSDUsExtended);
            wrFillTknU32(&(newIE->criticality),CztCriticalityignoreEnum);
            wrFillTknStr16384BSXL(&(newIE->extensionValue.u.cztReceiveStatusOfULPDCPSDUsExtended),
                  pdcpSduCfm->pdcpSduStaInfo[idx1].rcvStaOfUlPdcpSdus.numBits,
                  pdcpSduCfm->pdcpSduStaInfo[idx1].rcvStaOfUlPdcpSdus.ulBitMap,
                  &snStatusTfrPdu->memCp);
         }
         else
         {
            wrFillTknU8(&(newIE->pres), NOTPRSNT);
         }

         /* Filling UL Count Value extended */
         newIE = &extIEs->member[1];
         wrFillTknU8(&(newIE->pres),PRSNT_NODEF);
         wrFillTknU32(&(newIE->id), Cztid_ULCOUNTValueExtended);
         wrFillTknU32(&(newIE->criticality),CztCriticalityignoreEnum);

         pdcpSn = (pdcpSduCfm->pdcpSduStaInfo[idx1].ulCountVal & (0x0fff));
         pdcpHfn = ((pdcpSduCfm->pdcpSduStaInfo[idx1].ulCountVal >> 15) & 0xfffff);
         wrFillTknU8(&(newIE->extensionValue.u.cztid_ULCOUNTValueExtended.pres),
               PRSNT_NODEF);
         wrFillTknU32(&(newIE->extensionValue.u.cztid_ULCOUNTValueExtended.pDCP_SNExtended), 
               pdcpSn);
         wrFillTknU32(&(newIE->extensionValue.u.cztid_ULCOUNTValueExtended.hFNMdfd), 
               pdcpHfn);

         /* Filling DL Count Value extended */
         newIE = &extIEs->member[2];
         wrFillTknU8(&(newIE->pres),PRSNT_NODEF);
         wrFillTknU32(&(newIE->id), Cztid_DLCOUNTValueExtended);
         wrFillTknU32(&(newIE->criticality),CztCriticalityignoreEnum);

         pdcpSn = (pdcpSduCfm->pdcpSduStaInfo[idx1].dlCountVal & (0x0fff));
         pdcpHfn = ((pdcpSduCfm->pdcpSduStaInfo[idx1].dlCountVal >> 15) & 0xfffff);
         wrFillTknU8(&(newIE->extensionValue.u.cztid_DLCOUNTValueExtended.pres),
               PRSNT_NODEF);
         wrFillTknU32(&(newIE->extensionValue.u.cztid_DLCOUNTValueExtended.pDCP_SNExtended), 
               pdcpSn);
         wrFillTknU32(&(newIE->extensionValue.u.cztid_DLCOUNTValueExtended.hFNMdfd), 
               pdcpHfn);
      }
   }

   *pdu = snStatusTfrPdu;

   RETVALUE(ROK);
}/* End of wrUmmX2SrcHoFillSnStatusTrfr*/

/**
 * @brief 
 *    This function is used to send Sn Status Transfer Message to the peer eNB.
 *
 * @details
 *    Function: wrUmmX2SrcHoSndSnStaTfr
 *
 *    Processing steps:
 *       -# Fill Sn Status transfer message.
 *       -# Send Sn Status Transfer Message to the peer eNB.
 *       
 * @param[in]
 *    transCb         : Transaction Informatiion
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2SrcHoSndSnStaTfr
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2SrcHoTrans         *x2HoTrans = &transCb->u.x2SrcHoTrans;
   CztEvnt                   *snStsPdu;
   if(ROK != wrUmmX2SrcHoFillSnStatusTrfr(transCb, &snStsPdu))
   {
      RLOG0(L_ERROR, " Filling of X2 SN Status failed ");
      RETVALUE(RFAILED);
   }

   /**
    * filling the peerId
    */
   snStsPdu->peerId = x2HoTrans->peerId;
   WrIfmX2apBmpReq(snStsPdu);
   RLOG_ARG2(L_EVENT,DBG_CRNTI,transCb->ueCb->crnti,"[X2AP]:Sending SN STATUS TRANSFER [Old-eNB-UE-X2AP-ID:%d][New-eNB-UE-X2AP-ID:%d]",
         transCb->u.x2SrcHoTrans.old_eNB_UE_X2ap_Id, transCb->u.x2SrcHoTrans.new_eNB_UE_X2ap_Id);
   wrUmmX2SrcHoSendStartPdcpDataFwdReq(transCb);

   RETVALUE(ROK);
}/* End of wrUmmX2SrcHoSndSnStaTfr.*/

/**
 * @brief 
 *    This function is used to Fill AS Security Information.
 *
 * @details
 *    Function: wrUmmX2SrcHoFillAsSecurInfo
 *
 *    Processing steps:
 *       -# Get KeNB Star value
 *       -# Get Next HopChaining Count value
 *              
 * @param[in]
 *    aS_SecurInform    : AS Security Info
 * @param[in]
 *    handoverReq       : Handover Req
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16  wrUmmX2SrcHoFillAsSecurInfo
(
CztAS_SecurInform            *aS_SecurInform,
CztEvnt                      *handoverReq,
WrUeCb                       *ueCb,
WrUmmX2SrcHoTrans            *x2SrcHoTrans
)
{
/* ccpu00126532 */
   /**
    * Fill the AS Security Information
    */
   wrFillTknU8(&(aS_SecurInform->pres), PRSNT_NODEF);
   wrFillTknStrBSXL(&(aS_SecurInform->key_eNodeB_star),
         256, (Data *)x2SrcHoTrans->kEnbStar, &handoverReq->memCp);

   /**
    * Fill Next Hopchaining Count.
    */
   wrFillTknU32(&(aS_SecurInform->nextHopChainingCount), ueCb->nhChainCnt);

   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to Fill ERAB To Be SetUp List
 *
 * @details
 *    Function: wrUmmX2SrcHoFillErabToBeSetupList
 *
 *    Processing steps:
 *       -# Fill E-RAB Information
 *              
 * @param[in]
 *    handoverReq          : Event structure pointer
 * @param[in]
 *    ueCb                 : UE control information
 * @param[in]
 *    e_RABs_ToBeSetup_Lst : e-RAB Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoFillErabToBeSetupList
(
CztEvnt                      *handoverReq,
WrUeCb                       *ueCb,
CztE_RABs_ToBeSetup_Lst      *e_RABs_ToBeSetup_Lst
)
{
   CztProtIE_Single_Cont_E_RABs_ToBeSetup_ItemIEs  *ie1 = NULLP;
   CztE_RAB_Lvl_QoS_Params                         *e_RAB_Lvl_QoS_Params;
   CztX2AP_PDU                                     *pdu = NULLP;
   U8                                              erabIdx = 0;
   U8                                              tunnelendPointLen = 0;
   Bool                                            datFwdReq;
   U16                                             bitRateLen = 0;
   U8                                              cnt;
   U8                                              offSet;

   pdu = &handoverReq->pdu;
   /**
    * Fill E-RABS List
    */
   wrFillTknU16(&(e_RABs_ToBeSetup_Lst->noComp),
         ueCb->rbInfo.numOfRbCfgd);
   if ((cmGetMem(handoverReq,
               (ueCb->rbInfo.numOfRbCfgd *
                sizeof(CztProtIE_Single_Cont_E_RABs_ToBeSetup_ItemIEs)),
               (Ptr *)&e_RABs_ToBeSetup_Lst->member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   for(erabIdx = 0; erabIdx < WR_MAX_DRBS; erabIdx++)
   {
      if(ueCb->rbInfo.rab[erabIdx].inUse == TRUE)
   {
      ie1 = &e_RABs_ToBeSetup_Lst->member[erabIdx];

      wrFillTknU8(&(ie1->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie1->id), Cztid_E_RABs_ToBeSetup_Item);
      wrFillTknU32(&(ie1->criticality), CztCriticalityignoreEnum);

      wrFillTknU8(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.pres),
            PRSNT_NODEF);
      /**
       * Fill the E-RAB ID
       */
      wrFillTknU32(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.e_RAB_ID),
            ueCb->rbInfo.rab[erabIdx].eRABId);
      /**
       * Fill the E-RAB QOS Parameters.
       */
      e_RAB_Lvl_QoS_Params = &ie1->value.u.cztE_RABs_ToBeSetup_Item.e_RAB_Lvl_QoS_Params;
      wrFillTknU8(&(e_RAB_Lvl_QoS_Params->pres), PRSNT_NODEF);
      /**
       * Fill the QCI value.
       */
      wrFillTknU32(&(e_RAB_Lvl_QoS_Params->qCI),
            ueCb->rbInfo.rab[erabIdx].qci);

      /* 
       * Start of Comments
       * Fill the ARP values are priority level,pre-emption capability and 
       * pre-emption vulnearability.
       * In Current release, No ARP Details.So values are filled as constant.
       */
#if 1
       /**
        * sps - TODO: CURRENT CODE FOR TESTING ONLY
        */

      wrFillTknU8(&(e_RAB_Lvl_QoS_Params->allocationAndRetentionPriority.pres),PRSNT_NODEF);
         wrFillTknU32(&(e_RAB_Lvl_QoS_Params->allocationAndRetentionPriority.priorityLvl),
                                                        ueCb->rbInfo.rab[erabIdx].priority);
         wrFillTknU32(&(e_RAB_Lvl_QoS_Params->allocationAndRetentionPriority.pre_emptionCapblty), 
                                                         ueCb->rbInfo.rab[erabIdx].preempCap);
         wrFillTknU32(&(e_RAB_Lvl_QoS_Params->allocationAndRetentionPriority.pre_emptionVulnerability), 
                                                         ueCb->rbInfo.rab[erabIdx].preempVul);
#endif
      /**
       * Filling GBR info IE
       */
      if(ueCb->rbInfo.rab[erabIdx].isGbrQosPres == TRUE)
      {
         wrFillTknU8(&(e_RAB_Lvl_QoS_Params->gbrQosInform.pres),PRSNT_NODEF);
         bitRateLen = 4;
         /*Bitrate is in kbps*/
         wrFillTknStrOSXL1(&e_RAB_Lvl_QoS_Params->gbrQosInform.e_RAB_MaxBitrateDL,
         bitRateLen,(ueCb->rbInfo.rab[erabIdx].mbr.dl),&handoverReq->memCp);
         wrFillTknStrOSXL1(&e_RAB_Lvl_QoS_Params->gbrQosInform.e_RAB_MaxBitrateUL,
         bitRateLen,(ueCb->rbInfo.rab[erabIdx].mbr.ul),&handoverReq->memCp);
         wrFillTknStrOSXL1(&e_RAB_Lvl_QoS_Params->gbrQosInform.e_RAB_GuaranteedBitrateDL,
         bitRateLen,(ueCb->rbInfo.rab[erabIdx].gbr.dl),&handoverReq->memCp);
         wrFillTknStrOSXL1(&e_RAB_Lvl_QoS_Params->gbrQosInform.e_RAB_GuaranteedBitrateUL,
         bitRateLen,(ueCb->rbInfo.rab[erabIdx].gbr.ul),&handoverReq->memCp);
      }
 
      wrUmmHoIsDatFwdRqrd(&ueCb->rbInfo.rab[erabIdx], &datFwdReq);
      if(datFwdReq == TRUE)
      {
         /**
          * Fill the DL Forwarding IE
          */
         wrFillTknU32(&(ie1->value.u.cztE_RABs_ToBeSetup_Item.dL_Fwding),
               CztDL_FwdingdL_forwardingProposedEnum);
         /**
          * set the dataFwd flag to TRUE
          */
         ueCb->rbInfo.rab[erabIdx].isDataFwdAvl = TRUE;
      }

      /**
       * Filling of UL GTP Tunnel Endpoint i.e S-GW Tunnel Endpoint Ids
       * for the delivery of the UL Packets from Target eNB to S-GW

       */

      ie1->value.u.cztE_RABs_ToBeSetup_Item.uL_GTPtunnelEndpoint.pres.pres = PRSNT_NODEF;
      /**
       * Fill the ULink GTP Tunnel
       */
      wrX2apFillTnlAddr(handoverReq,
            &ueCb->rbInfo.rab[erabIdx].sgwAddr,
            &(ie1->value.u.cztE_RABs_ToBeSetup_Item.uL_GTPtunnelEndpoint.transportLyrAddr));

      /**
       * Allocate memory for the octect string
       * sps  : TODO verify the len
       */
      //tunnelendPointLen = ueCb->rbInfo.rab[erabIdx].remEgtpTunId;
      tunnelendPointLen = 4;
      if ((cmGetMem(handoverReq,tunnelendPointLen,
                  (Ptr *)&(ie1->value.u.cztE_RABs_ToBeSetup_Item.uL_GTPtunnelEndpoint.gTP_TEID.val))) !=ROK)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      ie1->value.u.cztE_RABs_ToBeSetup_Item.uL_GTPtunnelEndpoint.gTP_TEID.pres = TRUE;
      for (cnt = 0; cnt < tunnelendPointLen; cnt++)
      {
         /**
          * compute the offset to right shift the val in U32
          * shift by 24, 16 & 8 bytes 
          */
         offSet = (U8)(tunnelendPointLen - (cnt + 1)) * 8;
         ie1->value.u.cztE_RABs_ToBeSetup_Item.uL_GTPtunnelEndpoint.gTP_TEID.val[cnt] =
                  (U8)(ueCb->rbInfo.rab[erabIdx].remEgtpTunId >> offSet);
      }
      ie1->value.u.cztE_RABs_ToBeSetup_Item.uL_GTPtunnelEndpoint.gTP_TEID.len = tunnelendPointLen;
      }
   }/* End of for(idx = 0; idx < ue_Cntxt_Info->e_RABs_ToBeSetup_Lst.noComp; idx++)*/   
   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to Fill UE Context Information
 *
 * @details
 *    Function: wrUmmX2SrcHoFillUeCntxtInfo
 *
 *    Processing steps:
 *       -# Fill UE Context Information
 *              
 * @param[in]
 *    handoverReq          : Event structure pointer
 * @param[in]
 *    ueCb                 : UE control information
 * @param[in]
 *    x2SrcHoTrans         : X2 transaction control block information
 * @param[in]
 *    ue_CntxtInform       : UE Context Information
 * @param[in]
 *    srcToTgtCntr         : Src To Tgt Transparent container info
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoFillUeCntxtInfo
(
CztEvnt                        *handoverReq,
WrUeCb                       *ueCb,
WrUmmX2SrcHoTrans            *x2SrcHoTrans,
CztUE_CntxtInform            *ue_CntxtInform,
NhuEncCfmSdus                *srcToTgtCntr
)
{
   CztUESecurCapabilities    *uESecurCapabilities;
   CztUEAggMaxBitRate        *aggrBitRate;
   U16                       bitRateLen;
   U16                       len;
   U16                       msgLen;
   CztProtExtnField_UE_CntxtInform_ExtIEs *extIE;
   U16                       numComp;

   wrFillTknU8(&(ue_CntxtInform->pres),PRSNT_NODEF);
   /**
    * Fill the rest of the Values in UE Context Information
    */
   if(ueCb->s1ConCb->mmeUeS1apId2.pres == TRUE)
   {
      wrFillTknU32(&(ue_CntxtInform->mME_UE_S1AP_ID),
            ueCb->s1ConCb->mmeUeS1apId2.val);   
   }
   else
   {
   wrFillTknU32(&(ue_CntxtInform->mME_UE_S1AP_ID),
         ueCb->s1ConCb->mme_ue_s1ap_id);   
   }

   /**
    * Fill the security capabilities of UE
    */
   uESecurCapabilities = &ue_CntxtInform->uESecurCapabilities;
   wrFillTknU8(&(uESecurCapabilities->pres), PRSNT_NODEF);
   /**
    * Fill the Encryption Algorithm suported by UE
    */
   /* ccpu00130881 */
   WR_MEM_COPY(&uESecurCapabilities->encryptionAlgorithms, &ueCb->secCfg->encrypCap, 
               sizeof(TknBStr32));
   /**
    * Fill the Integrity Algorithms which is supported by UE
    */
   WR_MEM_COPY(&uESecurCapabilities->integrityProtectionAlgorithms, &ueCb->secCfg->integCap, 
               sizeof(TknBStr32));
   /**
    * Fill As Security Information.
    */
   /* ccpu00126532 */
   if (ROK != wrUmmX2SrcHoFillAsSecurInfo(&ue_CntxtInform->aS_SecurInform,
            handoverReq, ueCb, x2SrcHoTrans))
   {
      RLOG0(L_ERROR, " Failed To Fill As Secur Info. ");
      RETVALUE(RFAILED);
   }

   /**
    * Fill the UE Aggregate Max Bit Rate
    */
   aggrBitRate = &(ue_CntxtInform->uEaggregateMaxBitRate);

   wrFillTknU8(&(aggrBitRate->pres), PRSNT_NODEF);
   bitRateLen = 4;
   wrUtlFillBitRate(&handoverReq->memCp,
         &(aggrBitRate->uEaggregateMaxBitRateDlnk), bitRateLen,
         ueCb->rbInfo.aggBr.dl);
   wrUtlFillBitRate(&handoverReq->memCp,
         &(aggrBitRate->uEaggregateMaxBitRateUlnk), bitRateLen,
         ueCb->rbInfo.aggBr.ul);
   aggrBitRate->iE_Extns.noComp.pres = NOTPRSNT; 

   /**
    * Subscriber Profile ID IE is optional.
    */
   /**
    * Fill ERAB To Be Setup List.
    */
   if (ROK != wrUmmX2SrcHoFillErabToBeSetupList(handoverReq, ueCb,
            &ue_CntxtInform->e_RABs_ToBeSetup_Lst))
   {
      RLOG0(L_ERROR, " Failed To Fill ERAB Setup List. ");
      RETVALUE(RFAILED);
   }    
   /**
    * Fill the RRC Context
    */
   SFndLenMsg(srcToTgtCntr->sdu.mBuf,(S16 *)&msgLen);
   ue_CntxtInform->rRC_Cntxt.pres = PRSNT_NODEF;
   ue_CntxtInform->rRC_Cntxt.len  = msgLen;
   WR_GET_MEM((&handoverReq->memCp), msgLen, 
         &(ue_CntxtInform->rRC_Cntxt.val));
   SCpyMsgFix(srcToTgtCntr->sdu.mBuf, 0, msgLen,
         (Data *)(ue_CntxtInform->rRC_Cntxt.val),(MsgLen *)&len);

   /* ccpu00131919 */
   /**
    * Fill the Location Reporting Information IE only if, 
    * MME sent LocRptCtrl Msg with Request set to report upon change of serving cell
    */
   if(ueCb->locReportType == TRUE)
   {
      wrFillTknU8(&(ue_CntxtInform->locationRprtngInform.pres),PRSNT_NODEF);
      wrFillTknU32(&(ue_CntxtInform->locationRprtngInform.eventTyp),
            CztEventTypchange_of_serving_cellEnum);   
      wrFillTknU32(&(ue_CntxtInform->locationRprtngInform.reportArea),
            CztReportAreaecgiEnum);      
   }
   /* Fill the Handover Restrn List MME_LDBAL*/
    if(ueCb->hoRstrctInfo != NULLP)
    {
       U8        idx = 0;
       /* Check if the serving and equivalent PLMN has to be swapped or not */
       if(!(wrUtlPlmnsEqual(&ueCb->hoRstrctInfo->srvngPlmn, &ueCb->fnlzdPLMN)))
       {
          /* The finalized PLMN supported by the Target eNodeB is not same as the 
           * serving PLMN in the handover restriction. */
          for(idx = 0; idx < ueCb->hoRstrctInfo->noOfEplmn; idx++)
          {
             if(wrUtlPlmnsEqual(&ueCb->fnlzdPLMN, &ueCb->hoRstrctInfo->ePlmn[idx]))
             {
                WR_SET_ZERO(&ueCb->hoRstrctInfo->ePlmn[idx], sizeof(WrPlmnId));
                break;
             }
          }
          /* Add the existing serving PLMN in HRL into the equivalent PLMN entry 
           * that was set to zero above. */
          ueCb->hoRstrctInfo->ePlmn[idx] = ueCb->hoRstrctInfo->srvngPlmn;
          /* Add the finalized PLMN as the serving PLMN. */
          ueCb->hoRstrctInfo->srvngPlmn = ueCb->fnlzdPLMN;
       }
        wrEmmBuildHoRestrnLst(ueCb, handoverReq, &ue_CntxtInform->handoverRestrnLst);
    }

    /* If the Management Based MDT allowed information is present, include the 
     * same in the Handover Request Message to target eNB
     */
    if(TRUE == ueCb->mgmtBasedMDTAllowed.pres)
    {
       numComp = 1;
       wrFillTknU16(&(ue_CntxtInform->iE_Extns.noComp), numComp);

       if (ROK != (cmGetMem(handoverReq, 
                 (sizeof(CztProtExtnField_UE_CntxtInform_ExtIEs) * numComp),
                 (Ptr *) &ue_CntxtInform->iE_Extns.member)))
       {
          RLOG0(L_FATAL, "Memory allocation failed.");
          RETVALUE(RFAILED);
       }
       extIE = &(ue_CntxtInform->iE_Extns.member[0]);

       wrFillTknU8(&(extIE->pres), PRSNT_NODEF);
       wrFillTknU32(&(extIE->id), Cztid_ManagementBasedMDTallowed);
       wrFillTknU32(&(extIE->criticality), CztCriticalityignoreEnum);
       wrFillTknU32(&(extIE->extensionValue.u.cztManagementBasedMDTallowed),
                      ueCb->mgmtBasedMDTAllowed.val);
    }

    /* If the subscription profile ID is present, fill the same in the 
     * handover request message during X2 handover
     */
    if(TRUE == ueCb->subscProfileId.pres)
    {
       wrFillTknU32(&(ue_CntxtInform->subscriberProfileIDforRFP), 
                     ueCb->subscProfileId.val);
    }

   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to Fill GUMMEI Information
 *
 * @details
 *    Function: wrUmmX2SrcHoFillGummei
 *
 *    Processing steps:
 *       -# Fill GUMMEI Information
 *              
 * @param[in]
 *    pdu           : Event structure pointer
 * @param[in]
 *    ueCb          : UE control information
 * @param[in]
 *    x2SrcHoTrans  : X2 transaction control block information
 * @param[in]
 *    gummei        : GUMMEI Informarion
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoFillGummei
(
CztEvnt                      *pdu,
WrUeCb                       *ueCb,
WrUmmX2SrcHoTrans            *x2SrcHoTrans,
CztGUMMEI                    *gummei
)
{
   WrGummei             *transGummei = NULLP;
   /*ccpu00128503*/
   U16                  mmeGrpId = 0;

   if(ueCb->srcGummei.pres == TRUE)
   {
      transGummei = &ueCb->srcGummei;
   }
   else
   {
      transGummei = &ueCb->gummei;
   }

   wrFillTknU8(&(gummei->pres),PRSNT_NODEF);
   /**
    * Fill the GU Group Id of of GUMMEI
    */
   wrFillTknU8(&(gummei->gU_Group_ID.pres),PRSNT_NODEF);
   /**
    * Fill the PLMN Idendity of GU Group Id
    */
   wrUmmX2SrcHoFillPLMNId(pdu, &ueCb->plmnId,
         &gummei->gU_Group_ID.pLMN_Identity);
   /**
    * Fill the MME Group ID
    */
   /*ccpu00128503*/
   mmeGrpId = CM_INET_HTON_U16(transGummei->mmeGrpId);
   wrFill2TknStr4(&(gummei->gU_Group_ID.mME_Group_ID), 2,
         (U8*)&mmeGrpId);
   /**
    * Fill the MME Code of GUMMEI
    */
   wrFill2TknStr4(&(gummei->mME_Code), 1, (U8*)&transGummei->mmec);

   gummei->gU_Group_ID.iE_Extns.noComp.pres = NOTPRSNT;

   RLOG3(L_DEBUG, " Sending MME Group Info for ueId [%d] mmeId [%ld] mmeGroupId [%d] ",
         ueCb->crnti, ueCb->mmeId, wrEmmCb.guGrpIdLst[0].mmeGrpId);

   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to Fill ECGI Information
 *
 * @details
 *    Function: wrUmmX2SrcHoFillECGI
 *
 *    Processing steps:
 *       -# Fill ECGI Information
 *              
 * @param[in]
 *    pdu           : Event structure pointer
 * @param[in]
 *    x2SrcHoTrans  : X2 transaction control block information
 * @param[in]
 *    ecgi          : ECGI Informarion
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2SrcHoFillECGI
(
CztEvnt                      *pdu,
WrUmmX2SrcHoTrans            *x2SrcHoTrans,
CztECGI                      *ecgi
)
{
   WrEcgi                    *transEcgi = NULLP;

   transEcgi = &x2SrcHoTrans->ecgi;
   wrFillTknU8(&(ecgi->pres), PRSNT_NODEF);   
   wrUmmX2SrcHoFillPLMNId(pdu, &transEcgi->plmnId, &ecgi->pLMN_Identity);
   /**
    * Fill the E-UTRAN Cell Identifier of target cell id
    */
   wrFillTknBStr32(&(ecgi->eUTRANcellIdentifier), 28, transEcgi->eutranCellId);
   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to Enable trace procedure
 *
 * @details
 *    Function: wrUmmIsTraceEnabled
 *
 *    Processing steps:
 *       -# Enable Trace Procedure
 *              
 * @param[in]
 *    transCb       : Transaction Information
 * @param[in]
 *    isEnabled     : to enable the flag
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmIsTraceEnabled
(
WrUmmTransCb                 *transCb,
U8                           *isEnabled
)
{
    *isEnabled = FALSE;
    RETVALUE(ROK);
}
/* UE History info - start */
/**
 * @brief 
 *    This function is used to Fill UE History Information
 *
 * @details
 *    Function: wrUmmX2SrcHoFillUeHistoryInform
 *
 *    Processing steps:
 *       -# Fill UE History Information
 *              
 * @param[in]
 *    handoverReq      : Event structure pointer
 * @param[in]
 *    transCb          : transaction control information
 * @param[in]
 *    ueHistoryInform  : UE History information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoFillUeHistoryInform
(
CztEvnt                      *handoverReq,
WrUmmTransCb                 *transCb,
CztUE_HistoryInform          *ueHistoryInform
)
{
   CztLastVisitedCell_Item        *lastVisitedCellInfo = NULLP;
   U16                            ueHistoryInfoIdx = 0;
   WrUeCb                         *ueCb = transCb->ueCb;
   WrUeHstryEutranInfo            *eutraCellInfo = NULLP;
   EpcTime                        currentTime;
   CztLastVisitedEUTRANCellInform *eutraInfo = NULLP;
   U32                            ueHistryCompSize;
   U32                            cellInfoIdx = 0;
   TknStrOSXL                     *utraCellInfo = NULLP;
   CztLastVisitedUTRANCellInform  *uTRAN_Cell = NULLP;
   U32                            ueTime = 0;
   WrCgi                          cgi;
   U8                             cellSizeType;

   /* Update the current cell details in the history */

   ueHistryCompSize = ueCb->ueHstryInfo.numOfCells + 1;
   if(ueHistryCompSize > WR_UMM_UE_HISTORY_MAX_CELLS)
   {
     ueHistryCompSize = WR_UMM_UE_HISTORY_MAX_CELLS;   
   }

   wrFillTknU16(&(ueHistoryInform->noComp), ueHistryCompSize);
   /* Allocate the memory for ue history component*/
   if ((cmGetMem(handoverReq,ueHistryCompSize * sizeof(CztLastVisitedCell_Item),
               (Ptr *)&ueHistoryInform->member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
 
   /* Update the current cell details in the history */

   /* Source Info */
   lastVisitedCellInfo = &ueHistoryInform->member[0];
   eutraInfo = &lastVisitedCellInfo->val.e_UTRAN_Cell;

   wrFillTknU8(&(lastVisitedCellInfo->choice),LASTVISITEDCELL_ITEM_E_UTRAN_CELL);
   wrFillTknU8(&(eutraInfo->pres),PRSNT_NODEF);
    /* Fill the global cell id */

   wrEmmGetCgi(ueCb->cellId,&cgi);
   wrFillTknU8(&(eutraInfo->global_Cell_ID.pres),PRSNT_NODEF);
   wrUmmX2SrcHoFillPLMNId(handoverReq, &cgi.plmnId, &(eutraInfo->global_Cell_ID.pLMN_Identity));
   wrFillTknBStr32(&(eutraInfo->global_Cell_ID.eUTRANcellIdentifier),
                   WR_UMM_CELL_ID_LENGTH, cgi.cellId);
   eutraInfo->global_Cell_ID.iE_Extns.noComp.pres = NOTPRSNT;

   /* Fill cell type */ 
   wrEmmGetCellSizeType(ueCb->cellId, &cellSizeType);
   wrFillTknU8(&(eutraInfo->cellTyp.pres),PRSNT_NODEF);
   wrFillTknU32(&(eutraInfo->cellTyp.cell_Size), cellSizeType);
   eutraInfo->cellTyp.iE_Extns.noComp.pres = NOTPRSNT;

   /* fill time UE stayed in cell */
   SGetEpcTime(&currentTime); //currenttime in milliseconds
   eutraInfo->time_UE_StayedInCell.val = 
      (currentTime - ueCb->ueAttachTime) / 1000;
   if(eutraInfo->time_UE_StayedInCell.val > WR_UMM_UE_MAX_STAY_TIME)
   {
      ueTime = WR_UMM_UE_MAX_STAY_TIME;
   }
   else
   {
      ueTime = eutraInfo->time_UE_StayedInCell.val;
   }
   wrFillTknU32(&(eutraInfo->time_UE_StayedInCell), ueTime);
   eutraInfo->iE_Extns.noComp.pres = NOTPRSNT;

   /* fill UE HISTORY information */
   for(ueHistoryInfoIdx = 1; ueHistoryInfoIdx < ueHistryCompSize;
         ueHistoryInfoIdx++, cellInfoIdx++)
   {
      lastVisitedCellInfo = &ueHistoryInform->member[ueHistoryInfoIdx];

      wrFillTknU8(&(lastVisitedCellInfo->choice),ueCb->ueHstryInfo.cellInfo[cellInfoIdx]
                  .ratType);
      switch (lastVisitedCellInfo->choice.val)
      {
         case LASTVISITEDCELL_ITEM_E_UTRAN_CELL:
         {
			eutraCellInfo = &(ueCb->ueHstryInfo.cellInfo[cellInfoIdx].
                            ratInfo.eutranInfo);
            eutraInfo = &lastVisitedCellInfo->val.e_UTRAN_Cell;
            wrFillTknU8(&(lastVisitedCellInfo->val.e_UTRAN_Cell.pres),PRSNT_NODEF);
            wrFillTknU8(&(lastVisitedCellInfo->val.e_UTRAN_Cell.global_Cell_ID.pres),
                  PRSNT_NODEF);
            /* Fill the PLMN ID of global cell id */
            wrUmmX2SrcHoFillPLMNId(handoverReq,&eutraCellInfo->ecgi.plmnId,
                         &(eutraInfo->global_Cell_ID.pLMN_Identity));
            /* Fill the E-UTRAN Cell Identifier of global cell id */
            wrFillTknBStr32(&(eutraInfo->global_Cell_ID.eUTRANcellIdentifier),
                            28, eutraCellInfo->ecgi.eutranCellId);
            eutraInfo->global_Cell_ID.iE_Extns.noComp.pres = NOTPRSNT;

           /* Fill cell type */ 
            wrFillTknU8(&(eutraInfo->cellTyp.pres),PRSNT_NODEF);
            wrFillTknU32(&(eutraInfo->cellTyp.cell_Size),
                         eutraCellInfo->cellSize);  
            eutraInfo->cellTyp.iE_Extns.noComp.pres = NOTPRSNT;
        /* fill time UE stayed in cell */
            wrFillTknU32(&(eutraInfo->time_UE_StayedInCell), 
                         eutraCellInfo->timeInCell);
            eutraInfo->iE_Extns.noComp.pres = NOTPRSNT;
         } 
            break;
         case LASTVISITEDCELL_ITEM_UTRAN_CELL:
         {
            utraCellInfo = &(ueCb->ueHstryInfo.cellInfo[cellInfoIdx].
                           ratInfo.utranInfo);
            uTRAN_Cell = &lastVisitedCellInfo->val.uTRAN_Cell;

            wrFillTknStrOSXL(uTRAN_Cell,utraCellInfo->len,utraCellInfo->val,
                             &handoverReq->memCp);
         }
         break;
         case LASTVISITEDCELL_ITEM_GERAN_CELL:
         {
           /* Information for GERAN is currently undefined as per 3GPP *
             * Need to update GERAN as necessary later */
         }
         break;
                    
         default:
            break;
      } /* end of switch statement */
   }/* end of For loop */
   RETVALUE(ROK);
}
/*UE History info - end*/        
/**
 * @brief 
 *    This function is used to Fill Handover Request Message.
 *
 * @details
 *    Function: wrUmmX2SrcHoFillHandoverReq
 *
 *    Processing steps:
 *       -# Fill Handover Request Information
 *              
 * @param[in]
 *    transCb   : transaction control information
 * @param[in]
 *    pdu       : Event structure pointer
 * @param[in]
 *    pci       : PCI
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2SrcHoFillHandoverReq
(
WrUmmTransCb                 *transCb,
CztEvnt                      **pdu
)
{
   CztEvnt                       *handoverReq = NULLP;
   CztHovrRqst                   *cztHovrRqst = NULLP;
   CztProtIE_Field_HovrRqst_IEs  *hovrReqIE = NULLP;
   U8                            numComp;
   U8                            compCnt = 0;
   U8                            isTraceEnabled;    
   WrUmmX2SrcHoTrans             *x2SrcHoTrans = &(transCb->u.x2SrcHoTrans);
   /* ccpu00126532 */
   NhuEncCfmSdus                 *encMsg = x2SrcHoTrans->rrcContrMsg->u.encCfm;

   if (ROK != cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE, &wrCb.mem,
            (Ptr *)&handoverReq))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/
   /* MemSet*/
   cmMemset((U8 *)&(handoverReq->pdu), 0, sizeof(CztX2AP_PDU));

   wrFillTknU8(&(handoverReq->pdu.choice), X2AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(handoverReq->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   wrFillTknU32(&(handoverReq->pdu.val.initiatingMsg.procedureCode),
         Cztid_handoverPrep);
   wrFillTknU32(&(handoverReq->pdu.val.initiatingMsg.criticality),
         CztCriticalityrejectEnum);
   numComp = 6; /* Mandatory Parameters */
   /**
    * 1. Old eNB UE X2AP ID
    * 2. Cause
    * 3. Target Cell ID
    * 4. GUMMEI
    * 5. UE Context Information
    * 6. UE History Information
    * 7. UE Membership Status
    */
   /**
    * If the Trace Activation is enabled to this UE, the source eNB shall 
    * include Trace Activation IE in Handover Request Message. 
    */
   wrUmmIsTraceEnabled(transCb, &isTraceEnabled);
   if (isTraceEnabled == TRUE)
   {
      numComp++;
   }
   /* CSG_DEV start */
   if (WR_ENB_HYBRID_ACCESS == x2SrcHoTrans->csgCellAccessMode)
   {
      numComp++;
   }
   /* CSG_DEV end */

   /* Check if SRVCC operation IE is received and it is set to TRUE */
   if(transCb->ueCb->srvccOperPossible == TRUE)
   {
      numComp++;
   }

   /**
    * TODO : to support filling of SRVCC Operation IE
    */
   cztHovrRqst = &handoverReq->pdu.val.initiatingMsg.value.u.cztHovrRqst;
   wrFillTknU8(&(cztHovrRqst->pres), PRSNT_NODEF);
   /**
    * Set no of components.
    */
   wrFillTknU16(&(cztHovrRqst->protocolIEs.noComp), numComp);
   /**
    * Allocate the Memory for the components to be added.
    */
   if (ROK != (cmGetMem(handoverReq,
               (numComp * sizeof(CztProtIE_Field_HovrRqst_IEs)),
               (Ptr *)&cztHovrRqst->protocolIEs.member)))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      WR_FREEEVNT(handoverReq);
      RETVALUE(RFAILED);
   } /* end of if statement */

   /**
    * Fill the OLD ENB UE X2AP ID
    */
   hovrReqIE = &cztHovrRqst->protocolIEs.member[compCnt++];
   /**
    * This is done intentionally to check at stack if this ID is not present
    */
   wrFillTknU8(&(hovrReqIE->pres), PRSNT_NODEF);
   wrFillTknU32(&(hovrReqIE->id), Cztid_Old_eNB_UE_X2AP_ID);
   wrFillTknU32(&(hovrReqIE->criticality), CztCriticalityrejectEnum);
   wrFillTknU32(&(hovrReqIE->value.u.cztUE_X2AP_ID),
         x2SrcHoTrans->old_eNB_UE_X2ap_Id);

   /**
    * Message shall contain cause with appropriate value
    * Use the information sent by Handover Decision module
    */
   hovrReqIE = &cztHovrRqst->protocolIEs.member[compCnt++];
   wrFillTknU8(&(hovrReqIE->pres), PRSNT_NODEF);
   wrFillTknU32(&(hovrReqIE->id), Cztid_Cause);
   wrFillTknU32(&(hovrReqIE->criticality), CztCriticalityignoreEnum);
   wrX2apFillCause(&hovrReqIE->value.u.cztCause,
         &x2SrcHoTrans->cztCause);

   /**
    * Fill the target Cell Id
    */
   hovrReqIE = &cztHovrRqst->protocolIEs.member[compCnt++];
   wrFillTknU8(&(hovrReqIE->pres), PRSNT_NODEF);
   wrFillTknU32(&(hovrReqIE->id), Cztid_TgetCell_ID);
   wrFillTknU32(&(hovrReqIE->criticality), CztCriticalityrejectEnum);
   wrUmmX2SrcHoFillECGI(handoverReq, x2SrcHoTrans, &hovrReqIE->value.u.cztECGI);


   /**
    * FILL GUMMEI
    */
   hovrReqIE = &cztHovrRqst->protocolIEs.member[compCnt++];
   wrFillTknU8(&(hovrReqIE->pres), PRSNT_NODEF);
   wrFillTknU32(&(hovrReqIE->id), Cztid_GUMMEI_ID);
   wrFillTknU32(&(hovrReqIE->criticality), CztCriticalityrejectEnum);
   wrUmmX2SrcHoFillGummei(handoverReq, transCb->ueCb, x2SrcHoTrans,
         &hovrReqIE->value.u.cztGUMMEI);

   /**
    * Fill Ue Context Information
    */
   hovrReqIE = &cztHovrRqst->protocolIEs.member[compCnt++];
   wrFillTknU8(&(hovrReqIE->pres), PRSNT_NODEF);
   wrFillTknU32(&(hovrReqIE->id), Cztid_UE_CntxtInform);
   wrFillTknU32(&(hovrReqIE->criticality), CztCriticalityrejectEnum);
   if (ROK != wrUmmX2SrcHoFillUeCntxtInfo(handoverReq, transCb->ueCb,
            x2SrcHoTrans, &hovrReqIE->value.u.cztUE_CntxtInform, encMsg))
   {
      RLOG0(L_ERROR, " Failed To Fill UE Context Info ");
      WR_FREEEVNT(handoverReq);
      RETVALUE(RFAILED);
   }

   /**
    * Filling of UE History Information
    */
   hovrReqIE = &cztHovrRqst->protocolIEs.member[compCnt++];
   wrFillTknU8(&(hovrReqIE->pres), PRSNT_NODEF);
   wrFillTknU32(&(hovrReqIE->id), Cztid_UE_HistoryInform);
   wrFillTknU32(&(hovrReqIE->criticality), CztCriticalityignoreEnum);
   /**
    * The eNB collects and stores the UE History Information for as long as
    * the UE stays in one of its cells.
    */
   if (ROK != wrUmmX2SrcHoFillUeHistoryInform(handoverReq, transCb,
            &hovrReqIE->value.u.cztUE_HistoryInform))
   {
      RLOG0(L_ERROR, " Failed To Fill UE History Inform. ");
      WR_FREEEVNT(handoverReq);
      RETVALUE(RFAILED);
   }

   /* CSG_DEV start */
   if (WR_ENB_HYBRID_ACCESS == x2SrcHoTrans->csgCellAccessMode)
   {
      hovrReqIE = &cztHovrRqst->protocolIEs.member[compCnt++];
      wrFillTknU8(&(hovrReqIE->pres), PRSNT_NODEF);
      wrFillTknU32(&(hovrReqIE->id), Cztid_CSGMembershipStatus);
      wrFillTknU32(&(hovrReqIE->criticality), CztCriticalityrejectEnum);

      if(WR_UMM_CSG_MEMBER == wrUmmCsgIsUeMember(transCb->ueCb,
               x2SrcHoTrans->csgId))
      {
         wrFillTknU32(&(hovrReqIE->value.u.cztCSGMembershipStatus), 
               CztCSGMembershipStatusmemberEnum);
      }
      else
      {
         wrFillTknU32(&(hovrReqIE->value.u.cztCSGMembershipStatus), 
               CztCSGMembershipStatusnot_memberEnum);
      }
   }
   /* CSG_DEV end */

   if(transCb->ueCb->srvccOperPossible == TRUE)
   {
      /**
       * Fill SRVCC operation possible IE , Optional 
       */
      hovrReqIE = &cztHovrRqst->protocolIEs.member[compCnt++];
      wrFillTknU8(&(hovrReqIE->pres), PRSNT_NODEF);
      wrFillTknU32(&(hovrReqIE->id), Cztid_SRVCCOperationPossible);
      wrFillTknU32(&(hovrReqIE->criticality), CztCriticalityignoreEnum);
      wrFillTknU32(&(hovrReqIE->value.u.cztSRVCCOperationPossible), 
                     CztSRVCCOperationPossiblepossibleEnum);
   }

   *pdu = handoverReq;
      
   RETVALUE(ROK);
}/* End of wrUmmX2SrcHoFillHandoverReq*/

/**
 * @brief 
 *    This function is used to prepare Handover Request Message and
 *    send to the target eNB.
 *
 * @details
 *    Function: wrUmmX2SrcHoSndHandoverReq
 *
 *    Processing steps:
 *       -# Fill Handover Request Information
 *       -# Send Handover Request to the peer eNB
 *              
 * @param[in]
 *    transCb   : transaction control information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoSndHandoverReq
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2SrcHoTrans         *x2HoTrans = &transCb->u.x2SrcHoTrans;
   CztEvnt                   *handoverReq = NULLP;
   /* ccpu00126532 */
   WrUeCb                    *ueCb = transCb->ueCb;

   /**
    * Assign the Handover State for this UE.
    */
   x2HoTrans->state = WR_HANDOVER_PREPARATION;
   /**
    *  Fill Handover Request Message and sends to the target eNB.
    */
   if (ROK != wrUmmX2SrcHoFillHandoverReq(transCb, &handoverReq))
   {
      RLOG0(L_ERROR, " Failed To Fill Handover Request Message. ");
      RETVALUE(RFAILED);
   }/* End of wrUmmX2SrcHoFillHandoverReq*/    
   /**
    * Filling peer ID.
    */
   handoverReq->peerId = x2HoTrans->peerId;
   RLOG_ARG2(L_EVENT,DBG_CRNTI,ueCb->crnti,"[X2AP]:Sending HANDOVER REQUEST [Old-eNB-UE-X2AP-ID:%d][Cause:%d]",
         transCb->u.x2SrcHoTrans.old_eNB_UE_X2ap_Id,transCb->u.x2SrcHoTrans.cztCause.causeVal);

   /* TODO : Start TRELOCprep                                             */
   if (ROK != WrIfmX2apBmpReq(handoverReq))
   {
      RLOG0(L_ERROR, " Failed To Send Handover Request Message. ");
      RETVALUE(RFAILED);
   }
   
   /*Increment KPI for Attempted outgoing inter-eNB handover preparations*/
    WR_INC_KPI_FUNC(wrIncKpiFrOutIntrEnbHoPrpAtmpt);
    /* update ueInfo */
   ueCb->x2ConCb.peerId = x2HoTrans->peerId;
   ueCb->x2ConCb.oldEnbUeX2apId = x2HoTrans->old_eNB_UE_X2ap_Id;
   RETVALUE(ROK);
   
}/* End of wrUmmX2SrcHoSndHandoverReq */
/**
 * @brief 
 *    This function is used to initialize transaction.
 *
 * @details
 *    Function: wrUmmX2SrcHoInitTrans
 *
 *    Processing steps:
 *       -# Initialize the transaction information
 *              
 * @param[in]
 *    hoTrans       : To Initialize handover Transactiion
 * @param[in]
 *    internalMsg   : Information to be filled in Handover transaction
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2SrcHoInitTrans
(
WrUmmX2SrcHoTrans            *hoTrans,
WrUmmIntCrHoTran             *internalMsg
)
{
   WrUmmIntCrIntraLteHoTranInfo *intraLteHoTransInfo = &(internalMsg->t.intraLte);
   hoTrans->crnti               = internalMsg->crnti;
   hoTrans->cellId              = internalMsg->cellId;
   hoTrans->peerId              = internalMsg->peerId;

   hoTrans->ecgi.plmnId.numMncDigits  = intraLteHoTransInfo->ecgi.plmnId.numMncDigits;
   hoTrans->ecgi.plmnId.mcc[0]    = intraLteHoTransInfo->ecgi.plmnId.mcc[0];
   hoTrans->ecgi.plmnId.mcc[1]    = intraLteHoTransInfo->ecgi.plmnId.mcc[1];
   hoTrans->ecgi.plmnId.mcc[2]    = intraLteHoTransInfo->ecgi.plmnId.mcc[2];
   if(hoTrans->ecgi.plmnId.numMncDigits == 2)
   {
      hoTrans->ecgi.plmnId.mnc[0] = intraLteHoTransInfo->ecgi.plmnId.mnc[0];
      hoTrans->ecgi.plmnId.mnc[1] = intraLteHoTransInfo->ecgi.plmnId.mnc[1];
   }
   else
   {
      hoTrans->ecgi.plmnId.mnc[0] = intraLteHoTransInfo->ecgi.plmnId.mnc[0];
      hoTrans->ecgi.plmnId.mnc[1] = intraLteHoTransInfo->ecgi.plmnId.mnc[1];
      hoTrans->ecgi.plmnId.mnc[2] = intraLteHoTransInfo->ecgi.plmnId.mnc[2];
   }
   hoTrans->ecgi.eutranCellId     = intraLteHoTransInfo->ecgi.eutranCellId;
   hoTrans->enbType               = intraLteHoTransInfo->enbType;

   /* ccpu00126532 */
   hoTrans->targetPci             = intraLteHoTransInfo->pci;
   hoTrans->targetEarfcnDl        = intraLteHoTransInfo->earfcnDl;

   hoTrans->cztCause.causeTyp     = internalMsg->hoCause.causeTyp;
   hoTrans->cztCause.causeVal     = internalMsg->hoCause.causeVal;

   /* update CSG info */
   hoTrans->csgId                 = internalMsg->csgId;
   hoTrans->csgCellAccessMode     = internalMsg->csgCellAccessMode;

   RETVALUE(ROK);
}

PRIVATE S16 wrUmmX2SrcHoSendStartPdcpDataFwdReq
(
 WrUmmTransCb                 *transCb
)
{
   WrUeCb  *ueCb = transCb->ueCb;
   NhuHdr  nhuHdr;
   WrUmmX2SrcHoTrans         *x2SrcHoTrans = NULLP;
   Bool                      ulDatFwdReq = FALSE;
   U8                        rbIdx = 0;

   nhuHdr.cellId = ueCb->cellId;
   nhuHdr.ueId = ueCb->crnti;
   nhuHdr.transId = transCb->transId | 0x2;

   x2SrcHoTrans = &transCb->u.x2SrcHoTrans;
   for(rbIdx = 0; rbIdx < x2SrcHoTrans->numOfRbInfo; rbIdx++)
   {
      if(x2SrcHoTrans->rbInfo[rbIdx].isULDatFwdReq == TRUE)
      {
         /* Even if for 1 Rab UL Forwarding is proposed set the flag to TRUE */
         ulDatFwdReq = TRUE; 
         break;
      }
   }

   nhuHdr.isUlDatFwdReq = ulDatFwdReq;
   RLOG1(L_DEBUG, " Sending PDCP Data Forward request UE (%d) ", transCb->ueCb->crnti);
   WrIfmRrcSendStartPdcpDataFwdReq(nhuHdr);
   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to handle internal message.
 *
 * @details
 *    Function: wrUmmX2SrcHoHdlIntMsg
 *
 *    Processing steps:
 *       -# Initialize the transaction information
 *              
 * @param[in]
 *    transCb   : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoHdlIntMsg
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmX2SrcHoTrans         *x2HoTrans = &transCb->u.x2SrcHoTrans;
   WrUmmInternalMsg          *internalMsg = transCb->msg->u.internalMsg;
   NhuEncReqSdus             *hoPrepEncMsg = NULLP;

   if (internalMsg == NULLP)
   {
      RLOG0(L_ERROR, " Invalid Inputs (msg is NULL). ");
      RETVALUE(RFAILED);
   }

   switch(internalMsg->intMsgType)
   {
      case WR_UMM_INTMSG_CR_HO_TRANS:
      {
         x2HoTrans->old_eNB_UE_X2ap_Id  = ueCb->cellId;
         x2HoTrans->old_eNB_UE_X2ap_Id  <<= 8;
         x2HoTrans->old_eNB_UE_X2ap_Id  |= ueCb->ueIdx;
         /*ccpu133818 : mobCtrlState is changed here  
          * inorder to prevent multiple HO requests
          * in case if both inter as well as intra 
          * meas report is received at the same time*/
         ueCb->mobCtrlState = WR_HO_INITIATED;
         /* store transId in ueCb */
         ueCb->hoTransId = transCb->transId;
         ueCb->hoType = WR_HO_TYPE_X2;

         wrUmmX2SrcHoInitTrans(x2HoTrans, internalMsg->u.hoTrans);
         /*129363*/
         WR_ALLOCEVNT(&hoPrepEncMsg, sizeof(NhuEncReqSdus));
         if(NULLP == hoPrepEncMsg)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         /**
          * Fill RRC Encode Request Header
          */
         hoPrepEncMsg->hdr.cellId = ueCb->cellId;
         hoPrepEncMsg->hdr.ueId = ueCb->crnti;
         hoPrepEncMsg->hdr.transId = transCb->transId | WR_UMM_SRC_HO_HOPREP_ENC;
         hoPrepEncMsg->sdu.msgCategory = NHU_MSG_HOPREPINFO;

         /**
          * fill transparent container
          */
         wrUmmRrcFillSrcToTgetIe(ueCb, hoPrepEncMsg);
         if(ROK != wrIfmRrcSndEncodeReq(hoPrepEncMsg))
         {
            RLOG0(L_ERROR, " Failed to send Encode Req to RRC ");
            RETVALUE(RFAILED);
         }

         /* ccpu00126532 */
         /* Generate kEnbStar for Target */
#ifdef TENB_AS_SECURITY
         wrPlatX2HoGetKeNBStar(transCb);
#endif /* TENB_AS_SECURITY */ 
         break;
      }
      default:
      {
         RLOG0(L_ERROR, " Invalid internal msgType ");
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}/* End of wrUmmX2SrcHoHdlIntMsg */
/**
 * @brief 
 *    This function is used to Handle RRC encode Confirm
 *
 * @details
 *    Function: wrUmmX2SrcHoHdlRrcEncCfm
 *
 *    Processing steps:
 *       -# Send Handover Request Message
 *              
 * @param[in]
 *    transCb   : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoHdlRrcEncCfm
(
WrUmmTransCb                 *transCb
)
{
   NhuEncCfmSdus             *encMsg;
   U16                       msgType;

   encMsg = transCb->msg->u.encCfm;
   msgType = encMsg->hdr.transId & 0x0f;
   if(encMsg->sdu.status)
   {
      RLOG1(L_ERROR, " Encoding failed for %d ", msgType);
      RETVALUE(RFAILED);
   }
   switch(msgType)
   {
      case WR_UMM_SRC_HO_HOPREP_ENC:
      {
         /* ccpu00126532 */
         transCb->u.x2SrcHoTrans.rrcContrMsg = transCb->msg;
/*ccpu00128988*/         
#ifdef TENB_AS_SECURITY
         if(transCb->u.x2SrcHoTrans.isKenbKeyGenerated == TRUE)
#endif            
         {
            if (ROK != wrUmmX2SrcHoSndHandoverReq(transCb))
            {
               RLOG0(L_ERROR, " Failed To Prepare Handover Request Message.");
               RETVALUE(RFAILED);
            }
         }
/*ccpu00128988*/         
#ifdef TENB_AS_SECURITY
         else
         {
            transCb->msg->refCnt++;
         }
         transCb->u.x2SrcHoTrans.isRrcContrPrepared = TRUE;
#endif
         break;
      }
   }

   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to handle Handover Preparation Message.
 *
 * @details
 *    Function: wrUmmX2SrcHoPrcHoPrepFailure
 *
 *    Processing steps:
 *       -# Send Handover Request Message
 *              
 * @param[in]
 *    transCb   : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2SrcHoPrcHoPrepFailure
(
WrUmmTransCb                 *transCb
)
{
   CztProtIE_Field_HovrPrepFail_IEs      *ie = NULLP;
   CztHovrPrepFail                       *hovrPrepFail = NULLP;
   U8                                    hovrPrepFailIdx = 0;
   
    /**
     * Upon reception of Handover Preparation Failure Message, eNB shall release 
     * corresponding transaction and inform to Ho decision moule about handover failure.
     */
   hovrPrepFail = &transCb->msg->u.x2Pdu->pdu.val.unsuccessfulOutcome.value.u.cztHovrPrepFail;
   ie = &hovrPrepFail->protocolIEs.member[hovrPrepFailIdx++];
   if (ie->value.u.cztUE_X2AP_ID.val != transCb->u.x2SrcHoTrans.old_eNB_UE_X2ap_Id)
   {
       RLOG0(L_ERROR, " UE X2AP ID is wrong. ");
       RETVALUE(RFAILED);
   }

   RLOG_ARG2(L_EVENT,DBG_CRNTI,transCb->ueCb->crnti,"[X2AP]:Received HANDOVER PREPARATION FAILURE [Old-eNB-UE-X2AP-ID:%d][Cause:%d]",
         transCb->u.x2SrcHoTrans.old_eNB_UE_X2ap_Id,transCb->u.x2SrcHoTrans.cztCause.causeVal);

   WR_UMM_X2_SRC_HANDLE_HO_FAIL(transCb);
   wrUmmTransComplete(transCb);
   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to Fill Handover Cancel Message.
 *
 * @details
 *    Function: wrUmmX2SrcHoFillHoCancel
 *
 *    Processing steps:
 *       -# Fill Handover Cancel Message.
 *              
 * @param[in]
 *    transCb   : Transaction Information
 * @param[in]
 *    pdu       : Event Pdu
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoFillHoCancel
(
WrUmmTransCb                 *transCb,
CztEvnt                      **pdu
)
{
   CztEvnt                         *hoCancel = NULLP;
   CztHovrCancel                   *cztHovrCancel = NULLP;
   CztProtIE_Field_HovrCancel_IEs  *hoCancelIE = NULL;
   U8                              numComp = 0;
   U8                              compIdx = 0;
   WrUmmMsgCause                   cause;

   if (ROK != cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE, &wrCb.mem, (Ptr *)&hoCancel))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/
   /* MemSet*/
   cmMemset((U8 *)&(hoCancel->pdu), 0, sizeof(CztX2AP_PDU));

   wrFillTknU8(&(hoCancel->pdu.choice), X2AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(hoCancel->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   wrFillTknU32(&(hoCancel->pdu.val.initiatingMsg.procedureCode),
         Cztid_handoverCancel);
   wrFillTknU32(&(hoCancel->pdu.val.initiatingMsg.criticality),
         CztCriticalityignoreEnum);
   /**
    * Mandatory Parameters
    */
   numComp = 2;
   /* Fix for ccpu00138194 */
   if ( WR_HANDOVER_PREPARATION_ACK_RCVD == transCb->u.x2SrcHoTrans.state)
   {
      numComp++;
   }
   cztHovrCancel = &hoCancel->pdu.val.initiatingMsg.value.u.cztHovrCancel;   
   wrFillTknU8(&(cztHovrCancel->pres), PRSNT_NODEF);
   wrFillTknU16(&(cztHovrCancel->protocolIEs.noComp),numComp);
   /**
    * Allocate the Memory.
    */
   if (ROK != (cmGetMem(hoCancel,(numComp * sizeof(CztProtIE_Field_HovrCancel_IEs)),
               (Ptr *)&cztHovrCancel->protocolIEs.member)))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   } /* end of if statement */

   /**
    * Fill the OLD ENB UE X2AP ID
    */
   hoCancelIE = &cztHovrCancel->protocolIEs.member[compIdx++];
   /* This is done intentionally to check at stack if this ID is not present */
   wrFillTknU8(&(hoCancelIE->pres), PRSNT_NODEF);
   wrFillTknU32(&(hoCancelIE->id), Cztid_Old_eNB_UE_X2AP_ID);
   wrFillTknU32(&(hoCancelIE->criticality), CztCriticalityrejectEnum);   
   /**
    * Fill eNB UE X2ap Id.
    */
   wrFillTknU32(&(hoCancelIE->value.u.cztid_Old_eNB_UE_X2AP_ID), transCb->u.x2SrcHoTrans.old_eNB_UE_X2ap_Id);

   /* Fix for ccpu00138194 */
   if (WR_HANDOVER_PREPARATION_ACK_RCVD == transCb->u.x2SrcHoTrans.state)
   {
      /**
       * Fill the OLD ENB UE X2AP ID
       */
      hoCancelIE = &cztHovrCancel->protocolIEs.member[compIdx++];
      /**
       * This is done intentionally to check at stack if this ID is not present
       */
      wrFillTknU8(&(hoCancelIE->pres), PRSNT_NODEF);
      wrFillTknU32(&(hoCancelIE->id), Cztid_New_eNB_UE_X2AP_ID);
      wrFillTknU32(&(hoCancelIE->criticality), CztCriticalityignoreEnum);   
      /**
       * Fill eNB UE X2ap Id.
       */
      wrFillTknU32(&(hoCancelIE->value.u.cztid_New_eNB_UE_X2AP_ID), transCb->u.x2SrcHoTrans.new_eNB_UE_X2ap_Id); 
   }

   /**
    * Fill the Cause IE
    */
   hoCancelIE = &cztHovrCancel->protocolIEs.member[compIdx];
   wrFillTknU8(&(hoCancelIE->pres), PRSNT_NODEF);
   wrFillTknU32(&(hoCancelIE->id), Cztid_Cause);
   wrFillTknU32(&(hoCancelIE->criticality), CztCriticalityignoreEnum);
   /**
    * Fill X2 Cause Values.
    */
   cause.causeTyp = transCb->u.x2SrcHoTrans.cztCause.causeTyp;
   cause.causeVal = transCb->u.x2SrcHoTrans.cztCause.causeVal;
   wrX2apFillCause(&hoCancelIE->value.u.cztCause, &cause);
   *pdu = hoCancel;
   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to Send Handover Cancel Message.
 *
 * @details
 *    Function: wrUmmX2SrcHoSndHoCancel
 *
 *    Processing steps:
 *       -# Fill Handover Cancel Message.
 *       -# Handover Cancel Message.
 *              
 * @param[in]
 *    transCb   : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoSndHoCancel
(
WrUmmTransCb                 *transCb
)
{
    CztEvnt                         *hoCancel = NULLP;
   
    wrUmmX2SrcHoFillHoCancel(transCb, &hoCancel);
    /* End of wrUmmX2SrcHoFillHoCancel*/

   /* Send Handover Cancel Message to the target eNB.*/
   hoCancel->peerId = 1;/* TODO*/
    if (WrIfmX2apBmpReq(hoCancel))
    {
        RLOG0(L_ERROR, " Sending of X2 HO CANCEL Failed ");
        RETVALUE(RFAILED);
    }
    else
    {
       RLOG_ARG3(L_EVENT,DBG_CRNTI,transCb->ueCb->crnti,"[X2AP]:Sending HANDOVER CANCEL [Old-eNB-UE-X2AP-ID:%d][New-eNB-UE-X2AP-ID:%d][Cause:%d]",
             transCb->u.x2SrcHoTrans.old_eNB_UE_X2ap_Id,transCb->u.x2SrcHoTrans.new_eNB_UE_X2ap_Id,transCb->u.x2SrcHoTrans.cztCause.causeVal);
    }
   
    WR_UMM_X2_SRC_HANDLE_HO_FAIL(transCb);
    /* There is no Cancel ACK received from TGT 
       in X2 HO.*/
    wrUmmTransComplete(transCb);
    RETVALUE(ROK);
}
/* ccpu00131919 */
/**
 * @brief 
 *    This function is used to Handle S1 Data Indication Msg received in HoProcessor
 *
 * @details
 *    Function: wrUmmX2SrcHoHdlS1DatInd
 *
 *    Processing steps:
 *       -# Initiating Msg : Fill LocationReport Failure Message.
 *       -# Send the LocRptFail Msg to MME
 *              
 * @param[in]
 *    transCb   : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoHdlS1DatInd
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   S1apPdu        *locRptFail;

   switch(msg->u.s1Pdu->pdu->pdu.choice.val)
   {
      case INITIATING_MSG:
      {
         /**
          * Based on procedure code , trigger corresponding initiating message processing.
          */
         switch (msg->u.s1Pdu->pdu->pdu.val.initiatingMsg.procedureCode.val)
         {
            case Sztid_LocRprtngCntrl:
            {
               /**
                * Processing of Location Reporting Control 
                */
               /* When Ho is in progress and if we receive LocRptCtrl Msg, we should send LocRptFail */
               if (ROK != wrUmmS1apBldLocReportFail(transCb->ueCb, &locRptFail))
               {
                  RLOG0(L_ERROR, " Failed to Process Location Report Message. ");
                  RETVALUE(RFAILED);
               }
               wrIfmS1apSndUeMsg(transCb->ueCb->s1ConCb->spConnId, locRptFail);

               break;
            }
            default :
               RETVALUE(RFAILED);
         }/* End of Switch*/
         break;
      }
      case SUCCESSFUL_OUTCOME:
      {
         /* To Handle in case if we receive any successful outcomes in S1-DatInd for X2 HO*/
         break;
      }
      case UN_SUCCESSFUL_OUTCOME:
      {
         /* To Handle in case if we receive any unsuccessful outcomes in S1-DatInd for X2 HO*/
         break;
      }
      default :
      {
         RLOG0(L_ERROR, " InvalidMessage. ");
         RETVALUE(RFAILED);
      }
   }/*End of switch.*/  

   RETVALUE(ROK);
}/* wrUmmX2SrcHoHdlS1DatInd */



/* X2_HO Changes for Falcon */

/**
 * @brief 
 *    This function is used to handle X2ap PDU 
 *
 * @details
 *    Function: wrUmmX2SrcHoHdlX2apPdu
 *
 *    Processing steps:
 *       -# Based on X2ap PDU type, trigger corresponding message handler.
 *                     
 * @param[in]
 *    transCb   : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoHdlX2apPdu
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;

   switch(msg->u.x2Pdu->pdu.choice.val)
   {
      case INITIATING_MSG:
      {
         /**
          * Based on procedure code , trigger corresponding 
          * initiating message processing.
          */
         switch (msg->u.x2Pdu->pdu.val.initiatingMsg.procedureCode.val)
         {
            case Cztid_uECntxtRls:
            {
               /**
                * Processing of UE Context Release Message.
                */
               if (ROK != wrUmmX2SrcHoPrcUeCntxtRls(transCb))
               {
                  RLOG0(L_ERROR, " Failed to Process UE Context Release Message. ");
                  RETVALUE(RFAILED);
               }/* End of wrUmmX2SrcHoPrcUeCntxtRls*/

   RLOG_ARG2(L_EVENT,DBG_CRNTI,transCb->ueCb->crnti,"[X2AP]:Received UE CONTEXT RELEASE [Old-eNB-UE-X2AP-ID:%d][New-eNB-UE-X2AP-ID:%d]",
                    transCb->u.x2SrcHoTrans.old_eNB_UE_X2ap_Id, 
                    transCb->u.x2SrcHoTrans.new_eNB_UE_X2ap_Id);
               break;
            }
            default :
               RETVALUE(RFAILED);
         }/* End of Switch*/
         break;
      }
      case SUCCESSFUL_OUTCOME:
      {
         /**
          * Based on procedure code , trigger corresponding successful 
          * outcome message processing.
          */
         switch (msg->u.x2Pdu->pdu.val.successfulOutcome.procedureCode.val)
         {
            case Cztid_handoverPrep:
            {
               /**
                * Process Handover Request Ack Message.
                */
               if (ROK != wrUmmX2SrcHoPrcHoReqAck(transCb))
               {
                  RLOG0(L_ERROR, " wrUmmX2SrcHoPrcHoReqAck: Failed to Process Handover Request Ack Message. ");
                  RETVALUE(RFAILED);
               }
               break;
            }
            default :
               RETVALUE(RFAILED);
         }/* End of switch*/
         break;
      }
      case UN_SUCCESSFUL_OUTCOME:
      {
         switch (msg->u.x2Pdu->pdu.val.unsuccessfulOutcome.procedureCode.val)
         {
            case Cztid_handoverPrep:
            {
               /**
                * Process Handover Request Ack Message.
                */
               if (ROK != wrUmmX2SrcHoPrcHoPrepFailure(transCb))
               {
                  RLOG0(L_ERROR, " Failed to Process Handover Request Ack Message. ");
                  RETVALUE(RFAILED);
               }
               break;
            }
            default :
               RETVALUE(RFAILED);
         }
    
         break;
      }
      default :
      {
         RLOG0(L_ERROR, " Invalid Message. ");
         RETVALUE(RFAILED);
      }
   }/*End of switch.*/  

   RETVALUE(ROK);
}/* wrUmmX2SrcHoHdlX2apPdu(WrUmmTransCb *transCb)*/

/* ccpu00126532 */
#ifdef TENB_AS_SECURITY
/**
 * @brief 
 *    This function is used to store the kEnb*
 *
 * @details
 *    Function: wrUmmX2SrcPrcKdfRsp
 *
 *    Processing steps:
 *       - Store kEnb*
 *                     
 * @param[in]
 *    transCb   : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcPrcKdfRsp
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2SrcHoTrans         *x2HoTrans = &transCb->u.x2SrcHoTrans;

   RLOG2(L_DEBUG, " Received KDF Confirm for kEnb* key generation ueId [%d] target PCI [%d] ",
         transCb->ueCb->crnti, x2HoTrans->targetPci);

   RLOG2(L_DEBUG, " target DL EARFCN [%ld] status [%d] ",
         x2HoTrans->targetEarfcnDl, transCb->msg->u.kdfCfm.status);

   if(transCb->msg->u.kdfCfm.status == WR_CFG_NOK)
   {
      RETVALUE(RFAILED);
   }

   cmMemcpy(x2HoTrans->kEnbStar,
         transCb->msg->u.kdfCfm.ctfKdfCfm->u.kEnbStarInf.secKey, 32);

   if(x2HoTrans->isRrcContrPrepared == TRUE)
   {
      if (ROK != wrUmmX2SrcHoSndHandoverReq(transCb))
      {
         RLOG0(L_ERROR, " Failed To Prepare Handover Request Message. ");
         RETVALUE(RFAILED);
      }
      wrUmmRlsIncMsg(&x2HoTrans->rrcContrMsg);
   }

   x2HoTrans->isKenbKeyGenerated = TRUE;

   RETVALUE(ROK);
}
#endif

/* X2_HO Changes for Falcon */
/* Removed HoGetPdcpSduSta from here */

/**
 * @brief 
 *    This function is used to handle the X2AP error indication
 *
 * @details
 *    Function: wrUmmX2SrcHoHdlX2ErrInd
 *
 *    Processing steps:
 *       -# Check message type
 *       -# process accordingly
 *                     
 * @param[in]
 *    transCb   : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2SrcHoHdlX2ErrInd
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   WrUeCb                    *ueCb = transCb->ueCb;

   RLOG_ARG3(L_EVENT,DBG_CRNTI,ueCb->crnti,"[X2AP]:Received ERROR INDICATION with MsgType[%d] causeType [%d] causeVal[%d]",msg->u.cztLclErrInd->msgType,msg->u.cztLclErrInd->causeType,msg->u.cztLclErrInd->causeValue);

   /* ccpu00132789 */
   switch(msg->u.cztLclErrInd->msgType)
   {
      case CZT_MSG_HO_REQ:
         {
             WR_UMM_X2_SRC_HANDLE_HO_FAIL(transCb);
             wrUmmTransComplete(transCb);
         }
         break;

      case CZT_MSG_SN_STATUS_TRFR:
         transCb->state = WR_UMM_TRANS_DELETE_UE;
         break;
   }
   RETVALUE(ROK);
}

/**
 * @brief                                                                 
 *    This function is called when any message related to a particular    
 *    X2 HO is received in the source enodeb. This is the main function   
 *    which determines the next course of action based on the message rcvd.                                                              
 *    After the receipt of the measurement report if the HO decision has been
 *    taken to perform X2 HO, this transaction shall be created.
 *
 * @details
 *    Function : wrUmmX2SrcHoProcessor
 *
 *    Processing Steps :                                                                        
 *       Steps involved in X2 HO Source side:                                   
 *          1. Build the X2 HO Request, start the X2HORelocPrepTmr              
 *             and send to target enodeb. Change the state within the transCb   
 *             to HOREQ_SENT state.                                             
 *          2. When the X2 HO Ack message is received, process the message and  
 *             stop the X2HOReqAckTmr timer.                                    
 *          3. Upon receiving, scheduler confirmation, RRC is configured to     
 *             send out RRC Reconfiguration and the lower layer configuration.  
 *          4. When RRC Reconfiguration complete is received, an ack is sent    
 *             to MME.                                                          
 *    Failure:                                                               
 *       Any faiulre causes RAB setup failure and corrective configuration   
 *       is performed to undo any changes to other layers.                   
 * @param[in]
 *    transCb   : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED                                                                        
 */
PUBLIC S16 wrUmmX2SrcHoProcessor
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;

   switch(msg->msgTyp)
   {
      case WR_UMM_INTERNAL_MSG:
      {
         wrUmmX2SrcHoHdlIntMsg(transCb);
         break;
      }
      case WR_UMM_X2_BMP_PDU:
      {
         /**
          * Handle X2AP PDU.
          */
         if (ROK != wrUmmX2SrcHoHdlX2apPdu(transCb))
         {
            RLOG0(L_ERROR, " Failed to handle X2AP PDU at the Soure eNB. ");
            RETVALUE(RFAILED);
         }/* End of wrUmmX2SrcHoHdlX2apPdu*/ 
         break;
      }
      /* Fix for ccpu00138191 */
      case WR_UMM_RRC_DEC_CFM:
      {
         /**
          * Handle NHU Handover command Decode confirm.
          */
         if(ROK != wrUmmX2SrcHdlRrcHoCmdDecCfm(transCb))
         {
             if (ROK != wrUmmX2SrcHoSndHoCancel(transCb))
             {
                 RLOG0(L_ERROR, " wrUmmX2SrcHoSndHoCancel: Failed To"
                        " send wrUmmX2SrcHoSndHoCancel.");
             }
             RETVALUE(RFAILED);
         }
         break;
      }
      case WR_UMM_PDCP_SDU_STATUS_CFM:
      {
         /**
          * Send SN Status Transfer Message.
          */
         if (ROK != wrUmmX2SrcHoSndSnStaTfr(transCb))
         {
            RLOG0(L_ERROR, " Failed to send Sn Status Transfer Message. ");
            RETVALUE(RFAILED);
         }/* End of wrUmmX2SrcHoSndSnStaTfr.*/
         break;
      }
      case WR_UMM_DAM_CFM:
      {
         wrUmmX2SrcHoPrcDamCfm(transCb);
         if(transCb->u.x2SrcHoTrans.state == WR_UMM_SRC_HO_TNL_CFG_DONE)
         {
            RLOG1(L_DEBUG, " DAM Configuration Successful for UE (%d) ",
                  transCb->ueCb->crnti);
            /* Get PDCP SN Status, No L2 Reset Timer */
            wrUmmX2SrcHoGetPdcpSduSta(transCb);
         }
         break;
      }
      case WR_UMM_SND_HANDOVER_CANCEL:
      {
         /**
          * Send Handover Cancel Message.
          */
         if (ROK != wrUmmX2SrcHoSndHoCancel(transCb))
         {
            RLOG0(L_ERROR, " Failed To send wrUmmX2SrcHoSndHoCancel. ");
            RETVALUE(RFAILED);
         }/* End of wrUmmX2SrcHoSndHoCancel*/
         break;
      }
      case WR_UMM_RRC_ENC_CFM:
      {
         /*ccpu00133818*/
         if(ROK != wrUmmX2SrcHoHdlRrcEncCfm(transCb))
         {
            WR_UMM_X2_SRC_HANDLE_HO_FAIL(transCb);
            wrUmmTransComplete(transCb);
            RETVALUE(RFAILED);
         }
         break;
      }
      /* X2_HO Changes for Falcon */
      case WR_UMM_SCHD_CFG:
      {
#if 0 /*130053*/        
         if((transCb->msg->u.schdCfm.transId & 0x0f) ==
               WR_UMM_SRC_SCH_CFG_DRX_REL)
         {
            transCb->u.s1SrcHoTrans.state = WR_UMM_SRC_DRX_DISABLED;
            if(transCb->u.x2SrcHoTrans.hoCmd != NULLP &&
                  transCb->ueCb->drxCfg.isDrxEnabled == TRUE)
            {
               NhuDatReqSdus                   *nhuDatReqSdu = NULLP;
                  
               /**
               * Send the RRC Connection Reconfiguration Msg
               * which is already in the encoded format to RRC so that it 
               * sends out to the UE.
               */
               WR_ALLOCEVNT(&nhuDatReqSdu, sizeof(NhuDatReqSdus));
               if(NULLP == nhuDatReqSdu)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  RETVALUE(RFAILED);
               }
               if (ROK != wrUmmX2SrcHoFillNhuRrcConReCfgnWithMbuf(transCb, 
                        nhuDatReqSdu, transCb->u.x2SrcHoTrans.hoCmd))
               {
                  RLOG0(L_ERROR, " Failed to Fill RRC Con Reconfig. ");
                  RETVALUE(RFAILED);
               }  
               if(ROK != wrIfmRrcSndReqToUe(nhuDatReqSdu))
               {
                  RLOG0(L_ERROR, " wrIfmRrcSndReqToUe: Failed to send Dat Req ");
                  RETVALUE(RFAILED);
               }
      
               /*Increment KPI for Intra-LTE Handovers Attempted outgoing Handover to eNodeB per Handover cause*/
#ifdef WR_RSYS_KPI 
               transCb->ueCb->hovDir = OUT_HOV;
               transCb->ueCb->hov = LTE_X2_HOV;
#endif/*WR_RSYS_KPI*/
               WR_INC_KPI_FUNC4(wrIncKpiFrHovrAttmptd,WR_HO_TYPE_X2,LTE_X2_HOV,OUT_HOV,&(transCb->u.x2SrcHoTrans.cztCause));
               /*Increment KPI for Handover related measurements for DRX/non-DRX Attempted*/
               WR_INC_KPI_FUNC1(wrIncKpiFrHONoDrxOutAttmptd,transCb->ueCb->drxCfg.isDrxEnabled);
               WR_INC_KPI_FUNC2(wrIncKpiFrOutHoAtmptBsdOnFrqTyp,transCb->ueCb->freqTyp,transCb->ueCb->measGapUsd);            

               transCb->ueCb->mobCtrlState = WR_HO_IN_PROGRESS;

               wrUmmX2SrcHoGetPdcpSduSta(transCb);
            }         
         }   
#endif         
         break;  
      }

      /* X2_HO Changes for Falcon */
      case WR_UMM_RRM_UEREL_RESP:
      {
         /* RRM is tightly coupled */
         RLOG0(L_INFO, " UE Release Response Received from RRM ");
         break;
      }
      case WR_UMM_X2AP_ERR_IND:
      {
         wrUmmX2SrcHoHdlX2ErrInd(transCb);
         break;
      }
      /* ccpu00126532 */
#ifdef TENB_AS_SECURITY
      case WR_UMM_PHY_KDF:
      {
         wrUmmX2SrcPrcKdfRsp(transCb);
         break;
      }
#endif
      /* ccpu00130220 */
      case WR_UMM_X2_STA_IND:
      {
         wrUmmX2SrcPrcX2StaInd(transCb);
         break;
      }
      /* ccpu00130220 */
      /* ccpu00131919 */
      case WR_UMM_S1_DAT_IND:
      {
         /**
          * Handle S1 DAT IND 
          */
         if (ROK != wrUmmX2SrcHoHdlS1DatInd(transCb))
         {
            RLOG0(L_ERROR, " Failed to handle S1 Dat Ind at the Soure eNB. ");
            RETVALUE(RFAILED);
         }/* End of wrUmmX2SrcHoHdlS1DatInd*/ 
      }
      default:
      {
      RLOG1(L_ERROR, " Invalid msgType [%d] ", msg->msgTyp);
      }
      break;
   } /* end of switch statement */
   RETVALUE(ROK);
}
/* HO_REEST */
/** @brief This function is called when a reestablishment message is received during
 *         X2 Source Handover transaction .
 *
 * @details
 *
 *     Function: wrUmmX2SrcHoNewTransProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  inctransCb : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmX2SrcHoNewTransProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   S1apPdu                   *s1Pdu;
   TknStrOSXL                *nasPdu = NULLP;
   WrUmmMsgCause             cause;
   S1apPdu                   *pdu;
   SztInitiatingMsg          *initMsg;
   SztDlnkNASTport           *dlNas;
   U32                       idx;
   SztProtIE_Field_DlnkNASTport_IEs *ie;

   switch(incTrans->transTyp)
   {
      case WR_UMM_RRC_REESTAB_TRANS:
      {
            if( WR_HO_INITIATED == transCb->ueCb->mobCtrlState)
            {
               /* HO preparation phase so defer the HO Transaction */
               RETVALUE(WR_UMM_TRANS_DEFER_ONGOING);
            }
            else if( WR_HO_IN_PROGRESS == transCb->ueCb->mobCtrlState)
            {
               /* Increment the HO failure counter */
#ifdef WR_RSYS_KPI
               /* Increment the INTRA LTE HO Fail Counter */
               WR_INC_KPI_FUNC4(wrIncKpiFrHovrFaild, 
                     WR_HO_TYPE_X2, WR_HO_DIR_NONE, OUT_HOV,
                     &(transCb->u.x2SrcHoTrans.cztCause));
#endif
               RLOG0(L_ERROR,"wrUmmX2SrcHoNewTransProcessor: Sending wrUmmX2SrcHoSndHoCancel.");
               if (ROK != wrUmmX2SrcHoSndHoCancel(transCb))
               {
                  RLOG0(L_ERROR,"Failed To send wrUmmX2SrcHoSndHoCancel.");
                  RETVALUE(RFAILED);
               }/* End of wrUmmX2SrcHoSndHoCancel*/
      /* Calling Dam function to change the drb state to NORMAL and delete UL/DL forwarding
       * tunnels if any */
      wrIfmDamHdlHoFailure(ueCb->cellId, ueCb->crnti);
               ueCb->hovDir = WR_HO_DIR_NONE;
            }
         }
         break;
      case WR_UMM_CSFB_REDIR_TRANS:
         {
            RLOG0(L_INFO,"Sending X2HO Cancel");
            transCb->u.x2SrcHoTrans.cztCause.causeTyp = WR_CAUSE_RADIONW;
            transCb->u.x2SrcHoTrans.cztCause.causeVal = 
                CztCauseRadioNwunspecifiedEnum;
            if (ROK != wrUmmX2SrcHoSndHoCancel(transCb))
            {
               RLOG0(L_ERROR,"Failed To send wrUmmX2SrcHoSndHoCancel.");
               RETVALUE(RFAILED);
            }/* End of wrUmmX2SrcHoSndHoCancel*/
         }
         break;
      case WR_UMM_ANR_MEAS_TRANS:
      case WR_UMM_EVENT_ANR_TRANS:
         {
            /* As the handover transaction is ongoing ANR
               transaction shouldn't be started */
            RETVALUE(WR_UMM_TRANS_FAIL);
         }
         break;
      case WR_UMM_UL_NAS_TRANS:
         {
            RETVALUE(WR_UMM_TRANS_OK);
         }
         break;
      case WR_UMM_DL_NAS_TRANS:
         {
               pdu = incTrans->msg->u.s1Pdu->pdu;
               initMsg = &pdu->pdu.val.initiatingMsg;
               dlNas   = &initMsg->value.u.sztDlnkNASTport;
            switch(transCb->ueCb->mobCtrlState)
            {
               case WR_HO_NORMAL:
                  {
                     RETVALUE(WR_UMM_TRANS_OK);
                  }
                  break;
               case WR_HO_INITIATED:
                  {
                     for(idx = 0; idx < dlNas->protocolIEs.noComp.val; idx++)
                     {
                        ie = &dlNas->protocolIEs.member[idx];
                        if((ie->id.val == Sztid_HovrRestrnLst) && 
                              (ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
                        {
                           /* Cancel the ongoing handover procedure 
                            * and proceed with DL NAS */
                           transCb->u.x2SrcHoTrans.cztCause.causeTyp = WR_CAUSE_RADIONW;
                           transCb->u.x2SrcHoTrans.cztCause.causeVal = 
                              SztCauseRadioNwhandover_cancelledEnum;
                           wrUmmX2SrcHoSndHoCancel(transCb);
                           RETVALUE(WR_UMM_TRANS_OK);
                        }
                     }
                  }
                  break;
               default:
                  break;
            }
            /* Send nas non delivery */
            cause.causeTyp = CAUSE_RADIONW;
            cause.causeVal = 
               SztCauseRadioNwx2_handover_triggeredEnum;

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
               WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, cause.causeTyp, 
                     cause.causeVal, INC_KPI_VALUE_BY_ONE);
               /* CCB Merge: KW Fix */
               if (nasPdu != NULLP)
               {
                  RLOG_ARG4(L_INFO, DBG_CRNTI,ueCb->crnti,"Sending NAS"
                        "DELIVER INDICATION MME-UE-S1AP-ID[%d] eNB-UE-S1AP-ID[%d]"
                        "Cause Type[%d] Cause Value[%d]",
                        ueCb->s1ConCb->mme_ue_s1ap_id, 
                        ueCb->s1ConCb->enb_ue_s1ap_id,
                        cause.causeTyp, cause.causeVal);

                  wrUmmS1apFillNasNonDlvInd(ueCb->s1ConCb,&s1Pdu,nasPdu,&cause);
                  wrIfmS1apSndUeMsg(ueCb->s1ConCb->spConnId,s1Pdu);
               }
               else
               {
                  RLOG_ARG4(L_ERROR, DBG_CRNTI,ueCb->crnti,"Failed to sending NAS"
                        "DELIVER INDICATION due to NULL nasPdu MME-UE-S1AP-ID[%d] eNB-UE-S1AP-ID[%d]"
                        "Cause Type[%d] Cause Value[%d]",
                        ueCb->s1ConCb->mme_ue_s1ap_id,
                        ueCb->s1ConCb->enb_ue_s1ap_id,
                        cause.causeTyp, cause.causeVal);
               }
               RETVALUE(WR_UMM_TRANS_FAIL);
         }
         break;
      default:
         {
            RLOG1(L_ERROR,"Invalid TransType[%d]", incTrans->transTyp);
         }
   }
   RETVALUE(WR_UMM_TRANS_OK);
}
/**
 * @brief 
 *    This function is used to release the transaction.
 *
 * @details
 *    Function: wrUmmX2SrcHoTransRel
 *
 *    Processing steps:
 *       -# Release the transaction.
 *                     
 * @param[in]
 *    transCb   : Transaction Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmX2SrcHoTransRel
(
WrUmmTransCb                 *transCb
)
{
    WrUmmX2SrcHoTrans        *x2SrcTrans= &transCb->u.x2SrcHoTrans; 

   /**
    * Deallocate any transaction specific information
    */
   /* ccpu00127540 - MEM_LEAK */
   if(x2SrcTrans->rbInfo != NULLP)
   {
     WR_FREE(x2SrcTrans->rbInfo, 
           transCb->u.x2SrcHoTrans.numOfRbInfo * sizeof(WrUmmX2SrcHoRabInfo));
   }
   /* ccpu00127635 - MEM_LEAK */
   if(x2SrcTrans->cztE_RAB_Lst !=NULLP)
   {
      if(x2SrcTrans->cztE_RAB_Lst->member !=NULLP)
      {
         WR_FREE(x2SrcTrans->cztE_RAB_Lst->member,
               x2SrcTrans->cztE_RAB_Lst->noComp.val * sizeof(CztProtIE_Single_Cont_E_RAB_ItemIEs));
      }
      WR_FREE(x2SrcTrans->cztE_RAB_Lst, sizeof(CztE_RAB_Lst));
   }
   /* Set UeCb hotype to NONE */
   transCb->ueCb->hoType    = WR_HO_TYPE_NONE;
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
