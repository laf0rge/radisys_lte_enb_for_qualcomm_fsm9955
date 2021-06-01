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
               

     File:     wr_umm_x2tgtho.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=81;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_umm_trans.h"
#include "wr_utils.h"
#include "wr_ifm_x2ap.h"
#include "wr_ifm_s1ap.h"
#include "wr_umm_x2tgtho.h"
#include "wr_umm_ho.h"
#include "wr_umm_rrc_msg.h"
#include "wr_ifm_rrc.h"
#include "wr_emm_mme.h"
#include "wr_ifm_schd.h"
#include "wr_smm_smallcell.h"
#include "wr_dam.h"
#include "wr_kpi.h"
#include "wr_umm_drx.h"
#include "wr_umm_x2ap_msg.h"
#include "wr_umm_sch_msg.h"
#include "wr_umm_l1_msg.h"
#include "wr_ifm_l1.h"
#include "wr_ifm_son.h"
#include "wr_umm_s1srcho.h"
#include "wr_ifm_rrm.h"
#include "wr_emm_neighenb.h" /*UE History info*/

EXTERN S16 wrUtlFillX2Cause(U8  causeChoice,U8  causeValue,CztCause  *cause);
EXTERN S16 wrEmmMmeSztFillPLMNId(S1apPdu  *pdu,WrPlmnId  *plmnId,
                                                SztPLMNidentity  *plmnIe);
EXTERN S16 wrEmmMmeSztFillTAC(U16 tac,S1apPdu  *pdu,SztTAC  *tAC);
EXTERN S16 wrUmmSecGetSecCap(SztUESecurCapabilities   *ueSecCap,WrUeSecCfg   *secCfg);
EXTERN S16 wrUmmHoFillRadioResCfg (RmuUeRadioResCfg   *ueRadioResCfg, NhuHovrPrepInfo    *nhuHovrPrepInfo);
EXTERN S16 wrUmmHoCopyRadioResCfg (RmuUeRadioResCfg   *rmuUeRadioResCfg, RmuUeRadioResCfg   *txsUeRadioResCfg);
EXTERN S16 wrIfmRrmHoAdmitReq (RmuUeHoReq *rrmWrUeHoAdmitReq);
PRIVATE S16 wrPdcpSnStsUpd (WrUmmTransCb *transCb);
PUBLIC S16 wrUmmSecExtractX2HOSecKeys3(WrUmmTransCb *transCb,TknStrBSXL *secKey,Bool *asKeyGenerated,U8  kdfType);
#ifdef TENB_AS_SECURITY
EXTERN S16 wrPlatSecExtractSecKeys( WrUeSecCfg  *secCfg, TknStrBSXL *secKey, Bool *asKeyGenerated, U32 transId);
EXTERN S16 wrPlatS1SecExtractSecKeys (WrUeSecCfg *secCfg, Bool *asKeyGenerated,
      U32 transId);
EXTERN S16 wrPlatSecExtractSecKeys1(WrUeCb *ueCb,U32 transId,U8 keyDerType);
#endif
EXTERN S16 wrEmmGetTai
(
WrUeCb *ueCb,
WrTai  *tai
);
EXTERN S16 wrEmmGetTai
(
WrUeCb *ueCb,
WrTai  *tai
);
/**
 * @brief 
 *    This function is used to initialize Ue control block.
 *
 * @details
 *    Function: wrUmmX2TgtHoInitUeCb
 *
 *    Processing steps:
 *       -# initialize UE Control block.
 * @param[in] 
 *    ueCb    : UE Information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC U16 wrUmmX2TgtHoInitUeCb 
(
WrUeCb                       *ueCb
)
{
   wrUmmInitRrcTrans(ueCb);
   ueCb->ueCat                             = CM_LTE_UE_CAT_4;
   ueCb->establishCause                    = RMU_EST_CAUSE_HO_REQ;

   wrUmmInitUeCb(ueCb);

   /* Initialize Relocation Timer */
   cmInitTimers(&ueCb->s1TgtEnbRelocTmr.tmr,1);
   RETVALUE(ROK);
}/*wrUmmX2TgtHoInitUeCb*/

/**
 * @brief 
 *    This function is used to handle Handover Cancel Message.
 *
 * @details
 *    Function: wrUmmX2PrcHoCancel
 *
 *    Processing steps:
 *       -# Release the UE context Locally.
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 */

PRIVATE S16 wrUmmX2PrcHoCancel
(
 WrUmmTransCb                *transCb
)
{
   /* Need To be handled.*/
   RLOG_ARG2(L_EVENT,DBG_CRNTI, transCb->ueCb->crnti,"[X2AP]:Received HANDOVER CANCEL "
   "[Old-eNB-UE-X2AP-ID:%d][New-eNB-UE-X2AP-ID:%d]",
         transCb->u.x2TgtHoTrans.old_eNB_UE_X2ap_Id,
         transCb->u.x2TgtHoTrans.new_eNB_UE_X2ap_Id);
   /*FIX for ccpu00127073*/
  transCb->state = WR_UMM_TRANS_RELEASE_CALL;

   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to handle Error Indication Message.
 *
 * @details
 *    Function: wrUmmX2PrcHoErrInd
 *
 *    Processing steps:
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 */

PRIVATE S16 wrUmmX2PrcHoErrInd
(
 WrUmmTransCb                *transCb
)
{

   RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[X2AP]:Received ERROR INDICATION "
   "[Old-eNB-UE-X2AP-ID:%d][New-eNB-UE-X2AP-ID:%d]",
      transCb->u.x2TgtHoTrans.old_eNB_UE_X2ap_Id,
      transCb->u.x2TgtHoTrans.new_eNB_UE_X2ap_Id);
   /* Fix for ccpu00138192 */
   wrUmmTransComplete(transCb);
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to Fill Handover Request Ack message.
 *
 * @details
 *    Function: wrUmmX2TgtHoFillHoReqAck
 *
 *    Processing steps:
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @param[in]
 *    pdu        : Event Pdu
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoFillHoReqAck
(
WrUmmTransCb                 *transCb,
CztEvnt                      **pdu
)
{
   WrUmmX2TgtHoTrans                           *x2HoTrans;
   CztEvnt                                     *hovrRqstAckgPdu;
   U16                                         compcnt = 0;
   U8                                          noOfBearer;
   U8                                          rbIdx;
   S16                                         len;
   U8                                          cnt;
   U8                                          ret;
   U8                                          offSet;
   U8                                          numComp = 0;
   CztProtIE_Field_HovrRqstAckg_IEs            *ie = NULLP;
   CztProtIE_Single_Cont_E_RABs_Admtd_ItemIEs  *ie1 = NULLP;
   CztProtIE_Single_Cont_E_RAB_ItemIEs         *notAdmtdItem = NULLP;
   WrUmmX2TgtHoRabInfo                         *rbInfo = NULLP;
   CztSuccessfulOutcome                        *successfulOutcome = NULLP;
   U8                                          noOfFailedRbs = 0;
   U8                                          rbIdx1 = 0;
   CmTptAddr                                   *lclAddr;
   U8                                          rbMode;

   x2HoTrans = &transCb->u.x2TgtHoTrans;

   if (ROK != cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE,&wrCb.mem, (Ptr *)&hovrRqstAckgPdu))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/
   /* MemSet*/
   cmMemset((U8 *)&(hovrRqstAckgPdu->pdu), 0, sizeof(CztX2AP_PDU));
   successfulOutcome = &hovrRqstAckgPdu->pdu.val.successfulOutcome;
   wrFillTknU8(&(hovrRqstAckgPdu->pdu.choice), X2AP_PDU_SUCCESSFULOUTCOME);
   wrFillTknU8(&(successfulOutcome->pres), PRSNT_NODEF);
   wrFillTknU32(&(successfulOutcome->procedureCode), Cztid_handoverPrep);
   wrFillTknU32(&(successfulOutcome->criticality), CztCriticalityrejectEnum);
   /** 
    * eNB knows that how many bearers are going to support and not support at 
    * the time of Call admission Control.
    */
   noOfFailedRbs = x2HoTrans->erabsToRejectLst.numrabs; /*RRM_ARP*/
   if (noOfFailedRbs == 0)
   {
      numComp = 4;
   }
   else
   {
      numComp = 5;
   }/* End of else.*/
   wrFillTknU8(&(successfulOutcome->value.u.cztHovrRqstAckg.pres), PRSNT_NODEF);
   wrFillTknU16(&(successfulOutcome->value.u.cztHovrRqstAckg.protocolIEs.noComp), numComp);

   if ((cmGetMem(hovrRqstAckgPdu, (numComp * sizeof(CztProtIE_Field_HovrRqstAckg_IEs)),
               (Ptr *)&successfulOutcome->value.u.cztHovrRqstAckg.protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/
   /**
    * Fill the Old ENB UE X2AP ID
    */
   ie = &successfulOutcome->value.u.cztHovrRqstAckg.protocolIEs.member[compcnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_Old_eNB_UE_X2AP_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   wrFillTknU32(&(ie->value.u.cztid_Old_eNB_UE_X2AP_ID), x2HoTrans->old_eNB_UE_X2ap_Id);
   /**
    * Fill the New ENB UE X2AP ID
    */
   /**
    * This ID generated by Target eNB.
    * TODO*/
   ie = &successfulOutcome->value.u.cztHovrRqstAckg.protocolIEs.member[compcnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_New_eNB_UE_X2AP_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   wrFillTknU32(&(ie->value.u.cztid_New_eNB_UE_X2AP_ID), x2HoTrans->new_eNB_UE_X2ap_Id);
   /**
    * Fill the E-RABs Admitted list
    */
   ie = &successfulOutcome->value.u.cztHovrRqstAckg.protocolIEs.member[compcnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_E_RABs_Admtd_Lst);
   wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum); 
   wrFillTknU16(&(ie->value.u.cztE_RABs_Admtd_Lst.noComp), x2HoTrans->noOfAdmitdBearers);
   /**
    * No Of Admitted Bearers.
    */
   noOfBearer = x2HoTrans->noOfAdmitdBearers;

   if ((cmGetMem(hovrRqstAckgPdu, 
               (noOfBearer * sizeof(CztProtIE_Single_Cont_E_RABs_Admtd_ItemIEs)),
               (Ptr *)&ie->value.u.cztE_RABs_Admtd_Lst.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if Condition.*/
   rbInfo = x2HoTrans->rbInfo;
   for(rbIdx = 0; (rbIdx < x2HoTrans->numOfRbCfgd) && (rbIdx1 < noOfBearer); rbIdx++)
   {
      if (rbInfo[rbIdx].rabAllowed == TRUE)
      {
         ie1 = &ie->value.u.cztE_RABs_Admtd_Lst.member[rbIdx1];

         /* Determining if we need to wait for MME Status Transfer message */
         /* check if there is atleast 1 AM bearer */
         wrUtlGetRbMode(transCb->ueCb->cellId, rbInfo[rbIdx].drbInfo.qci,
               &rbMode);
         /* need not set for every AM bearer. single flag would suffice */
         if((x2HoTrans->snStatusInfoFlag == WR_UMM_TGT_HO_SN_INFO_NOWAIT) &&
               (rbMode == WR_RLC_AM_MODE))
         {
            x2HoTrans->snStatusInfoFlag = WR_UMM_TGT_HO_SN_INFO_WAIT;
         }
         
      wrFillTknU8(&(ie1->pres),PRSNT_NODEF);
      wrFillTknU32(&(ie1->id), Cztid_E_RABs_Admtd_Item);
      wrFillTknU32(&(ie1->criticality),CztCriticalityignoreEnum);
      wrFillTknU8(&(ie1->value.u.cztE_RABs_Admtd_Item.pres), PRSNT_NODEF);

      wrFillTknU32(&(ie1->value.u.cztE_RABs_Admtd_Item.e_RAB_ID),
            (rbInfo[rbIdx].drbInfo.eRABId));
      if(rbInfo[rbIdx].dlFwding == TRUE)
      {
         /* Retrieve the enb ip address. */
         /* We know that the eNB is connected with the X2 eNB neighbor using 
          * ip address, we use this ip address for retrieving. 
          */
         wrEmmGetDatAppAddr(&lclAddr, x2HoTrans->peerAddr.type);
         if ((WR_DIR_UL == (rbInfo[rbIdx].drbInfo.rbDir)) ||
               (WR_DIR_DL_UL == (rbInfo[rbIdx].drbInfo.rbDir))) 
         {
            wrFillTknU8(&(ie1->value.u.cztE_RABs_Admtd_Item.uL_GTP_TunnelEndpoint.pres), PRSNT_NODEF);
            len = 4;
            ie1->value.u.cztE_RABs_Admtd_Item.uL_GTP_TunnelEndpoint.gTP_TEID.pres = PRSNT_NODEF;
            ie1->value.u.cztE_RABs_Admtd_Item.uL_GTP_TunnelEndpoint.gTP_TEID.len = len;
            WR_GET_MEM(hovrRqstAckgPdu, len,&(ie1->value.u.cztE_RABs_Admtd_Item.uL_GTP_TunnelEndpoint.gTP_TEID.val));
            for (cnt = 0; cnt < len; cnt++)
            {
               /** compute the offset to right shift the val in U32
                * shift by 24, 16 & 8 bytes 
                */
               offSet = (U8)(len - (cnt + 1)) * 8;
               ie1->value.u.cztE_RABs_Admtd_Item.uL_GTP_TunnelEndpoint.gTP_TEID.val[cnt] =
                  (U8)(rbInfo[rbIdx].datFwdUlLocEgtpTunId >> offSet);
            }
            wrX2apFillTnlAddr(hovrRqstAckgPdu, lclAddr, 
                  &ie1->value.u.cztE_RABs_Admtd_Item.uL_GTP_TunnelEndpoint.transportLyrAddr);
         }
         if ((WR_DIR_DL == (rbInfo[rbIdx].drbInfo.rbDir)) ||
               (WR_DIR_DL_UL == (rbInfo[rbIdx].drbInfo.rbDir)))
         {
            wrFillTknU8(&(ie1->value.u.cztE_RABs_Admtd_Item.dL_GTP_TunnelEndpoint.pres),PRSNT_NODEF);
            len = 4;
            ie1->value.u.cztE_RABs_Admtd_Item.dL_GTP_TunnelEndpoint.gTP_TEID.pres = PRSNT_NODEF;
            ie1->value.u.cztE_RABs_Admtd_Item.dL_GTP_TunnelEndpoint.gTP_TEID.len = len;
            WR_GET_MEM(hovrRqstAckgPdu, len,&(ie1->value.u.cztE_RABs_Admtd_Item.dL_GTP_TunnelEndpoint.gTP_TEID.val));

            for (cnt = 0; cnt < len; cnt++)
            {
               /**
                * compute the offset to right shift the val in U32
                * shift by 24, 16 & 8 bytes
                */
               offSet = (U8)(len - (cnt + 1)) * 8;
               ie1->value.u.cztE_RABs_Admtd_Item.dL_GTP_TunnelEndpoint.gTP_TEID.val[cnt] =
                  (U8)(rbInfo[rbIdx].datFwdDlLocEgtpTunId >> offSet);
            }
            wrX2apFillTnlAddr(hovrRqstAckgPdu, lclAddr, 
                  &ie1->value.u.cztE_RABs_Admtd_Item.dL_GTP_TunnelEndpoint.transportLyrAddr);
         }
      }
      ie1->value.u.cztE_RABs_Admtd_Item.iE_Extns.noComp.pres = NOTPRSNT;
         rbIdx1++;
   }
   }

   if (noOfFailedRbs != 0)
   {
      /**
       * Fill E-RABs Not Admitted List
       */
      ie = &successfulOutcome->value.u.cztHovrRqstAckg.protocolIEs.member[compcnt++];

      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Cztid_E_RABs_NotAdmtd_Lst);
      wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
      wrFillTknU16(&(ie->value.u.cztE_RAB_Lst.noComp),noOfFailedRbs);
      /**
       * Allocate the Memory
       */
      if ((cmGetMem(hovrRqstAckgPdu, 
               (noOfFailedRbs * sizeof(CztProtIE_Single_Cont_E_RAB_ItemIEs)),
               (Ptr *)&ie->value.u.cztE_RAB_Lst.member)) !=ROK)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }/* End of if Condition.*/

      WrUmmRabRls  *rabRls = &x2HoTrans->erabsToRejectLst;

      for(rbIdx = 0; rbIdx < noOfFailedRbs; rbIdx++) /*RRM_ARP*/
      {
            notAdmtdItem = &ie->value.u.cztE_RAB_Lst.member[rbIdx];
            wrFillTknU8(&(notAdmtdItem->pres),PRSNT_NODEF);
            wrFillTknU32(&(notAdmtdItem->id), Cztid_E_RAB_Item);
            wrFillTknU32(&(notAdmtdItem->criticality),CztCriticalityignoreEnum);
            wrFillTknU8(&(notAdmtdItem->value.u.cztE_RAB_Item.pres), PRSNT_NODEF);
            wrFillTknU32(&(notAdmtdItem->value.u.cztE_RAB_Item.e_RAB_ID),
               (rabRls->erabId[rbIdx]));
           wrFillTknU8(&(notAdmtdItem->value.u.cztE_RAB_Item.cause.choice), 
                 rabRls->causeTyp);
           wrFillTknU32(&(notAdmtdItem->value.u.cztE_RAB_Item.cause.val.radioNw),
                  rabRls->cause[rbIdx]);
      }/*End of for*/
   }

   
   /**
    * Fill target to Source Transparant container
    */
   ie = &successfulOutcome->value.u.cztHovrRqstAckg.protocolIEs.member[compcnt];

   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_TgeteNBtoSrc_eNBTprntCont);
   wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
   /**
    * Find the length of the Encoded TrgtToSrcTC
    */
   ret = SFndLenMsg(transCb->msg->u.encCfm->sdu.mBuf, (S16 *)&len);
   if((len == 0) || (ret != ROK))
   {
      RLOG1(L_ERROR, "In correct buffer len[%d]", len);
      RETVALUE(RFAILED);
   }
   ie->value.u.cztTgeteNBtoSrc_eNBTprntCont.pres = PRSNT_NODEF;
   ie->value.u.cztTgeteNBtoSrc_eNBTprntCont.len = len;
   cnt = 0;
   /**
    * Allocate memory for the flat buffer
    */
   WR_GET_MEM(hovrRqstAckgPdu, len, &(ie->value.u.cztTgeteNBtoSrc_eNBTprntCont.val));
   SCpyMsgFix(transCb->msg->u.encCfm->sdu.mBuf,0, len, 
         (Data *)ie->value.u.cztTgeteNBtoSrc_eNBTprntCont.val,
         (S16 *)&cnt);
   *pdu = hovrRqstAckgPdu;
   RETVALUE(ROK);
}/*End of wrUmmX2TgtHoFillHoReqAck*/

/**
 * @brief 
 *    This function is used to Send Handover Request Ack.
 *
 * @details
 *    Function: wrUmmX2TgtHoSndHoReqAck
 *
 *    Processing steps:
 *       -# Fill Handover Request Ack message.
 *       -# Send Handover Request Ack to the Peer eNB.
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoSndHoReqAck
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &(transCb->u.x2TgtHoTrans);
   CztEvnt                   *hoAckPdu = NULLP;
   WrUmmCellCb               *cellCb = NULLP;  /*Cell CB required for KPI Increment*/
   /*FIX for ccpu00127073*/
   /*
    * New ue x2ap Id is now generated in prcHoReq Msg instead of here
    * */

   if (ROK != wrUmmX2TgtHoFillHoReqAck(transCb, &hoAckPdu))
   {
      RLOG0(L_ERROR, "Failed to fill Handover Ack Message");
      RETVALUE(RFAILED);
   }/* End of wrUmmX2TgtHoFillHoReqAck*/

   transCb->ueCb->mobCtrlState = WR_HO_IN_PROGRESS;
   transCb->ueCb->ueAttachedtoMme = TRUE; /* ccpu00130088 */
   /**
    * Filling peer ID
    */
   hoAckPdu->peerId = x2TgtHoTrans->peerId;
   /**
    * Start the Relocation Timer.
    */
   wrStartUeTmr(transCb->ueCb, WR_TMR_TGTENB_RELOCEXE, WR_TMR_S1_RELOC_VAL);
   if (ROK != WrIfmX2apBmpRsp(hoAckPdu))
   {
      RLOG0(L_ERROR, "Sending Response Failed");
      RETVALUE(RFAILED);
   }
   else
   {
      RLOG_ARG2(L_EVENT,DBG_CRNTI, transCb->ueCb->crnti,"[X2AP]:Sending HANDOVER REQUEST ACKNOWLEDGE "
      "[Old-eNB-UE-X2AP-ID:%d][New-eNB-UE-X2AP-ID:%d]",
            transCb->u.x2TgtHoTrans.old_eNB_UE_X2ap_Id,
            transCb->u.x2TgtHoTrans.new_eNB_UE_X2ap_Id);
   }
   wrStopTransTmr(transCb, WR_TMR_TGT_HOPREP); 
   /* CSG_DEV */
#ifdef WR_RSYS_KPI
   /* Updating KPI Counter for attempted HO to CSG or Hybrid Cell */
   WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId);
   if(cellCb == NULLP)
   {
      RLOG1(L_ERROR, " Error in getting CELL CB for cellId = %lu ",
            (U32)transCb->ueCb->cellId);
      RETVALUE(RFAILED);
   }

   if ((cellCb->csgAccessMode == WR_ENB_CLOSED_ACCESS)||
                 (cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS))
   {
      RLOG0(L_DEBUG,"Updating the KPI Counter for Succeeded Inbound Mobility");
      WR_INC_KPI_FUNC(wrIncKpiFrInboundCsgMobilityAttmptd);
   }
#endif
   /**
    * Change the Handover State
    */
   x2TgtHoTrans->state = WR_HANDOVER_EXECUTION;
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoSndHoReqAck.*/

/**
 * @brief 
 *    This function is used to fill crnti into appropriate transaction control
 *    block.
 *
 * @details
 *    Function: wrUmmHoUtlFillCrntiIntoTrans
 *
 *    Processing steps:
 *       -# Fill Crnti value.       
 *
 *  @param [in]  transCb    : transaction information.
 *  @param [in]  crnti      : crnti value.
 *
 *  @return S16
 *   -# Success : ROK
 *   -# Failure : RFAILED
 */
/**
 * @brief 
 *    This function is used to get Logical Channel ID.
 *
 * @details
 *    Function: wrUmmX2TgtHoGetLchDrbIds
 *
 *    Processing steps:
 *       -# Get the Logical Channel Id
 *       -# Get the DRB Id
 *
 *  @param [in]  transCb    : transaction information.
 *  @return S16
 *   -# Success : ROK
 *   -# Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoGetLchDrbIds
(
 WrUmmTransCb                *transCb
)
{
   U8                       bearerIdx = 0;
   U8                       numOfRbCfgd = 0;
   U8                       lchId;
   U8                       drbId;
   WrUmmX2TgtHoRabInfo      *rbInfo;

   numOfRbCfgd = transCb->u.x2TgtHoTrans.numOfRbCfgd;
   /**
    * Need To Reserve LCH ID and RB ID for every bearer.
    */
   for  (bearerIdx = 0; bearerIdx < numOfRbCfgd; bearerIdx++)
   {
      rbInfo = &(transCb->u.x2TgtHoTrans.rbInfo[bearerIdx]);

      /* ccpu00128207 */
      if (ROK != wrUmmTgtHoGetLchIdDrbId(transCb->ueCb, transCb->msg->u.decCfm,
                        rbInfo->drbInfo.eRABId, &drbId, &lchId,
                        &rbInfo->srcPdcpRlcCfgParams))
      {
         RLOG0(L_ERROR, "Failed To Get Drb ID");
         wrUmmRelLch(transCb->ueCb, lchId);
         RETVALUE(RFAILED);         
      }
      rbInfo->drbInfo.lchId = lchId;
      rbInfo->drbInfo.rbId  = drbId;
      rbInfo->rabAllowed    = TRUE;
   }
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoGetLchDrbIds*/

/* SPS changes starts */
/** @brief This function is responsible for filling the UL SPS LC for SCHD.
 *
 * @details
 *
 *     Function: wrUmmX2TgtHoSchdUlSpsLchCfgReq
 *
 *         Processing steps:
 *         - Check if SPS ERAB
 *         - Fill LC Id
 *
 * @param[in]  rrmUeRecfgResp : RRM Recfg Resp
 *             numErabs       : Number of ERABs from transCb
 *             pErabList      : Pointer to ERAB list from transCb
 *             ueSpsCfg       : UE SPS Cfg to be filled for SCHD
 * @return Void
 */

PUBLIC Void wrUmmX2TgtHoSchdUlSpsLchCfgReq
(
 RmuUeRecfgRsp        *rrmUeRecfgResp,
 U32                  numErabs,
 Void                 *pErabList,
 RgrUeSpsCfg          *ueSpsCfg
)
{
   U32 count;
   WrUmmX2TgtHoRabInfo  *pErabInfo = (WrUmmX2TgtHoRabInfo *)pErabList;

   ueSpsCfg->ulSpsCfg.lcCnt = 0;
   for (count = 0; count < numErabs; count++)
   {
      if((rrmUeRecfgResp->stErabAcceptLst[count].bIsUlSpsEnabled == TRUE))
      {
         /* Here Application should take stuffs from the RRM accepted list but currently 
          * there is no RBC support at both APP and RRM. Thus the expectation is, RRM has accepted 
          * all the ERABs in the request and thus have provided ERAB in the same order as sent to it. */
         ueSpsCfg->ulSpsCfg.spsLcInfo[ueSpsCfg->ulSpsCfg.lcCnt].isSpsEnabled = TRUE;
         ueSpsCfg->ulSpsCfg.spsLcInfo[ueSpsCfg->ulSpsCfg.lcCnt].lcId = (pErabInfo+count)->drbInfo.lchId;
         ueSpsCfg->ulSpsCfg.lcCnt++;
         (pErabInfo+count)->isUlSpsEnabled = TRUE;
      }
      else
      {
         (pErabInfo+count)->isUlSpsEnabled = FALSE;
      }
   }
}
/* SPS changes ends */

/**
 * @brief 
 *    This function is used to configure the UE.
 *
 * @details
 *    Function: wrUmmX2TgtHoSchdUeCfgReq
 *
 *    Processing steps:
 *       -# Fill UE Config
 *       -# Send the UE Configuration Request to scheduler.
 *
 *  @param [in]  transCb    : transaction information.
 *  @return S16
 *   -# Success : ROK
 *   -# Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoSchdUeCfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   RgrCfgReqInfo             *cfgReq = NULLP;
   RgrUeCfg                  *ueCfg = NULLP;
   U32                       transId = NULLP;
   U16                       prmbl;

   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &(transCb->u.x2TgtHoTrans);
   /* SPS changes starts */
   U32                       count         = 0;
   /* SPS changes ends */
   U8                        mode = 0;
   WrUmmCellCb               *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)ueCb->cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /*ccpu00129789*/
#ifdef RGR_CQI_REPT
   cfgReq->u.cfgInfo.u.ueCfg.ueCqiReptCfg.numColltdCqiRept = WR_SCHD_DL_CQI_RPT;
   if(ueCb->cqiRptCfg.fmtInd !=  WR_UE_CQI_FMT_WB)
   {
      /*ccpu00148940 - num of Collated Rept is reduced to 2 in case of 
       *Sub-band periodic CQI reporting to avoid RLF.As only Wide Band Cqi reporting 
       *are indicated in the CQI report from MAC to Application */
      cfgReq->u.cfgInfo.u.ueCfg.ueCqiReptCfg.numColltdCqiRept = 2;
   }
#endif
   if(ROK != wrUmmSchFillUeCfg(transCb->transId, transCb->ueCb, cfgReq))
   {
      RLOG0(L_ERROR, "Req preperation failed");
      RETVALUE(ROK);
   }

   ueCfg  = &(cfgReq->u.cfgInfo.u.ueCfg);
   /**
    * Fill dedicated preamble info
    */
   wrUtlGetDedPreamble(&prmbl, ueCb->cellId);
/*ccpu00133532 removed for*/
   if(prmbl != 0)
   {
      wrFillTknU8(&ueCfg->dedPreambleId, prmbl);
   }
   else
   {
      ueCfg->dedPreambleId.pres = PRSNT_DEF;
   }
   ueCb->dedicatedPrmbl = prmbl;

   if((x2TgtHoTrans->ambr.dl != 0) || (x2TgtHoTrans->ambr.dl != 0))
   {
      ueCfg->ueQosCfg.ambrPres = TRUE;
      ueCfg->ueQosCfg.dlAmbr = x2TgtHoTrans->ambr.dl / 8;
      ueCfg->ueQosCfg.ueBr = x2TgtHoTrans->ambr.ul / 8;
   }

   ueCfg->txMode.pres = PRSNT_NODEF;
   WR_GET_RGR_TM(ueCb->antennaInfo.txmode,mode);
   ueCfg->txMode.txModeEnum = mode;

   ueCfg->ueCodeBookRstCfg.pres = PRSNT_NODEF;
   ueCfg->ueCodeBookRstCfg.pmiBitMap[0] = wrPmiBitMap0;
   ueCfg->ueCodeBookRstCfg.pmiBitMap[1] = wrPmiBitMap1;
   /* SPS changes starts */
   /* The initialisation from the ueCb is required for the following case:
    * 1) If the SPS was already enabled for the UE, thus RRM doesnot allocate SPS
    *    - The SR mask for all the LCs should be set for rel 9 UE based on this.
    * 2) If the SPS was neither enabled previously nor it is enabled by RRM
    *    - Donot enable SR Mask for any of the LCs based on this. */
   for (count = 0; count < x2TgtHoTrans->numOfRbCfgd; count++)
   {
      x2TgtHoTrans->rbInfo[count].isUlSpsEnabled = FALSE;
   }
   transCb->spsCfgTransCb.isUeDlSpsEnabled = ueCb->spsCfg.spsCfgDl.bIsSpsEnabled;
   transCb->spsCfgTransCb.isUeUlSpsEnabled = ueCb->spsCfg.spsCfgUl.bIsSpsEnabled;
   wrUmmFillSpsSchdCfgUe(transCb->transTyp, transCb->ueCb, \
         x2TgtHoTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp, \
         ueCfg, x2TgtHoTrans->numOfRbCfgd, (Void *)(x2TgtHoTrans->rbInfo), \
         x2TgtHoTrans->ueCap->eUTRA_Cap.accessStratumRls.val,  &transCb->spsCfgTransCb.spsCfg,
         &transCb->spsCfgTransCb.isUeDlSpsEnabled, &transCb->spsCfgTransCb.isUeUlSpsEnabled);
   /* SPS changes ends */

   transId = transCb->transId | WR_UMM_TGT_HO_SCH_UECFG;

   if ((cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS) &&
         (WR_UMM_CSG_NOT_MEMBER == x2TgtHoTrans->ueMbrShpSts))
   {
      ueCfg->csgMmbrSta = FALSE;
   }
   else
   {
      /* membership true for closed and open access */
      ueCfg->csgMmbrSta = TRUE;
   }

   /**
    * Dispatch the prepared message to scheduler. There is no need for
    * another funtion for LCH separately than for UE
    */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoSchdUeCfgReq*/

/**
 * @brief 
 *    This function is used to configure SRB1 for the newly admitted UE.
 *
 * @details
 *    Function: wrUmmX2TgtHoSchdSrbLchCfgReq
 *
 *    Processing steps:
 *       -# Fill SRB Config
 *       -# Send the SRB Configuration Request to scheduler.
 *
 *  @param [in]  transCb    : transaction information.
 *  @param [in]  srbId      : srb Id
 *  @return S16
 *   -# Success : ROK
 *   -# Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoSchdSrbLchCfgReq
(
 WrUmmTransCb                *transCb,
 U8                          srbId
)
{
   RgrCfgReqInfo             *cfgReq = NULLP;
   RgrLchCfg                 *lchCfg = NULLP;
   U32                       transId = NULLP;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   if(srbId == WR_PDCP_ID_SRB1)
   {
      transId = transCb->transId | WR_UMM_TGT_HO_SCH_SRB1CFG;
   }
   else
   {
      transId = transCb->transId | WR_UMM_TGT_HO_SCH_SRB2CFG;
   }
      
   /**
    * Set last 4 bits as 1 for UE reconfiguration
    */
   lchCfg  = &cfgReq->u.cfgInfo.u.lchCfg;
   cfgReq->action = RGR_CONFIG;
   cfgReq->u.cfgInfo.cfgType = RGR_LCH_CFG;
   lchCfg->cellId = transCb->ueCb->cellId;
   lchCfg->crnti  = transCb->ueCb->crnti;
   lchCfg->lcId   = srbId;
   lchCfg->lcType = CM_LTE_LCH_DCCH;
   lchCfg->dlInfo.dlQos.qci = 5;

   lchCfg->dlInfo.dlQos.gbr = 1;
   lchCfg->dlInfo.dlQos.mbr = 1;
   lchCfg->dlInfo.dlQos.qci = SRB_QCI_VAL;
#ifdef LTE_L2_MEAS
   lchCfg->lcgId            = 0;
#endif

   lchCfg->ulLchQciInfo.lcId = srbId;
   lchCfg->ulLchQciInfo.qci = SRB_QCI_VAL;
   lchCfg->ulLchQciInfo.lcgId = 0;

   /**
    * Dispatch the prepared message to scheduler
    */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}/* End wrUmmX2TgtHoSchdSrbLchCfgReq*/

/**
 * @brief 
 *    This function is used to configure DRBs for the newly admitted UE.
 *
 * @details
 *    Function: wrUmmX2TgtHoSchdLchCfgReq
 *
 *    Processing steps:
 *       -# Fill Logical Channel Config
 *       -# Send the DRB Configuration Request to scheduler.
 *
 *  @param [in]  transCb    : transaction information.
 *  @param [in]  idx        : index value
 *  @return S16
 *   -# Success : ROK
 *   -# Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoSchdLchCfgReq
(
 WrUmmTransCb                *transCb,
 U8                          idx
)
{
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &transCb->u.x2TgtHoTrans;
   WrUeRAB                   *drbInfo;
   RgrCfgReqInfo             *cfgReq;
   RgrLchCfg                 *lchCfg;
   U32                       transId;
   WrRabCfgParams                       *params;
   WrLchCfgParams                       *lch;
   WrUmmCellCb                          *ummCellCb = NULLP;


   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /**
    * Set last 4 bits as 1 for UE reconfiguration
    */
   transId = transCb->transId | idx;
   lchCfg  = &cfgReq->u.cfgInfo.u.lchCfg;
   cfgReq->action = RGR_CONFIG;
   cfgReq->u.cfgInfo.cfgType = RGR_LCH_CFG;
   lchCfg->cellId = transCb->ueCb->cellId;
   lchCfg->crnti  = transCb->ueCb->crnti;
   drbInfo = &(x2TgtHoTrans->rbInfo[idx].drbInfo);
   lchCfg->lcId   = drbInfo->lchId;
   lchCfg->lcType = CM_LTE_LCH_DTCH;
   lchCfg->dlInfo.dlTrchType = CM_LTE_TRCH_DL_SCH;
   lchCfg->dlInfo.dlQos.qci = drbInfo->qci;
   lchCfg->dlInfo.dlQos.gbr = drbInfo->gbr.dl;
   lchCfg->dlInfo.dlQos.mbr = drbInfo->mbr.dl;
   lchCfg->ulLchQciInfo.lcId = drbInfo->lchId;
   lchCfg->ulLchQciInfo.qci = drbInfo->qci;

   /* SPS changes starts */
   if ((x2TgtHoTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG) && \
      (x2TgtHoTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stErabAcceptLst[idx].bIsDlSpsEnabled == TRUE))
   {
      /* Since there is no RBC and both RRM and APP need an enhancement. So, here there can be two approaches 
       * 1) Either assume that the ERAB sent to RRM is being received in the same order in the accepted list
       * 2) Or using a loop compare ERAB ids and find out if bIsSpsEnabled == TRUE 
       * Approach one is assumed as of now to avoid unnecessary loop. Thus accessed directly with idx */
      lchCfg->dlInfo.dlSpsCfg.isSpsEnabled = TRUE;
      x2TgtHoTrans->rbInfo[idx].isDlSpsEnabled = TRUE;
   }
   else
   {
      lchCfg->dlInfo.dlSpsCfg.isSpsEnabled = FALSE;
      x2TgtHoTrans->rbInfo[idx].isDlSpsEnabled = FALSE;
   }
   /* SPS changes ends */
   WR_UMM_GET_CELLCB(ummCellCb, transCb->ueCb->cellId);
   if(ummCellCb == NULLP)
   {
      RLOG0(L_ERROR, " CellCb is NULL. ");
      RETVALUE(RFAILED);
   }
   params = &ummCellCb->wrRabCfgParams[drbInfo->qci];
   lch           = &(params->lchParams);
   lchCfg->ulLchQciInfo.lcgId = lch->lchGrp;
#ifdef LTE_L2_MEAS
      lchCfg->lcgId = lch->lchGrp;
#endif

   /**
    * Dispatch the prepared message to scheduler
    */
   WrIfmSchdCfgUe(cfgReq, transId); 
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoSchdLchCfgReq*/

/**
 * @brief 
 *    This function is used to configure the scheduler
 *
 * @details
 *    Function: wrUmmX2TgtHoCfgSchd
 *
 *    Processing steps:
 *       -# Get the Logical Channel Id
 *       -# Configure the scheduler for Aggregate Max Bit Rate
 *       -# Configure scheduler for SRB1
 *       -# Configure scheduler for SRB2
 *       -# Configure scheduler for DRBs
 *       -# Configure Logical Channel Group
 *
 *  @param [in]  transCb    : transaction information.
 *  @return S16
 *   -# Success : ROK
 *   -# Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoCfgSchd
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2TgtHoRabInfo       *rbInfo;
   U8                        bearerIdx = 0;
   U8                        noOfBearers = 0;
   U32                       ulGbr[WR_UMM_MAX_LCG_GRP_ID] = {0};
   U32                       ulMbr[WR_UMM_MAX_LCG_GRP_ID] = {0};

   /*Convert kbps to kBps as scheduler expects in bytes*/
   noOfBearers = transCb->u.x2TgtHoTrans.numOfRbCfgd;
   for (bearerIdx = 0; bearerIdx < noOfBearers; bearerIdx++)
   {
      rbInfo = &(transCb->u.x2TgtHoTrans.rbInfo[bearerIdx]); 
      rbInfo->drbInfo.gbr.dl /= 8;
      rbInfo->drbInfo.gbr.ul /= 8;
      rbInfo->drbInfo.mbr.dl /= 8;
      rbInfo->drbInfo.mbr.ul /= 8;
   }

   /**
    * configure the Scheduler with Aggregate Max BitRate.
    */
   if (ROK != wrUmmX2TgtHoSchdUeCfgReq(transCb))
   {
      RLOG0(L_ERROR, "Failed To configure Scheduler.");
       RETVALUE(RFAILED);
   }

   /**
    * Configure the scheduler for SRB 1
    */
   wrUmmX2TgtHoSchdSrbLchCfgReq(transCb, WR_PDCP_ID_SRB1);

   /**
    * Configure the scheduler for SRB 2
    */
   wrUmmX2TgtHoSchdSrbLchCfgReq(transCb, WR_PDCP_ID_SRB2);

   /**
    * Configure the scheduler for each new RAB being created.
    */
   for (bearerIdx = 0; bearerIdx < noOfBearers; bearerIdx++)
   {
      rbInfo = &(transCb->u.x2TgtHoTrans.rbInfo[bearerIdx]);
      if(rbInfo->rabAllowed)
      {
         wrUmmX2TgtHoSchdLchCfgReq(transCb, bearerIdx);
         rbInfo->schdCfgState =  WR_UMM_TGT_HO_RAB_SCH_CFG_SENT;
         if (WR_UMM_IS_GBR_BEARER(rbInfo->drbInfo.qci))
         {
            U8 lcgId;
            lcgId = wrUmmGetLcgIdForQci(transCb->ueCb->cellId, rbInfo->drbInfo.qci);
            ulGbr[lcgId] += (rbInfo->drbInfo.gbr.ul >> 3);
            ulMbr[lcgId] += (rbInfo->drbInfo.mbr.ul >> 3);
         }
      }
   }

   /* ccpu00128203 */
   /* Configure all the logical channel groups at the same time. 
    * when a bearer of a different QCI is established, only UE needs to be 
    * configured */
   wrUmmSchdUeLcgCfg(transCb->ueCb, transCb->transId, WR_UMM_SRB_LCG_GRP_ID, 0, 0);
   wrUmmSchdUeLcgCfg(transCb->ueCb, transCb->transId, WR_UMM_LCG_GRP_ID_1, ulGbr[1], ulMbr[1]);
   wrUmmSchdUeLcgCfg(transCb->ueCb, transCb->transId, WR_UMM_LCG_GRP_ID_2, ulGbr[2], ulMbr[2]);
   wrUmmSchdUeLcgCfg(transCb->ueCb, transCb->transId, WR_UMM_LCG_GRP_ID_3, 0, 0);
   transCb->u.x2TgtHoTrans.expctLcgCfgCfm = WR_UMM_MAX_LCG_GRP_ID;

   RETVALUE(ROK);
}

/** @brief This function performs UE Configuration 
 *         towards Physical Layer. 
 *
 * @details
 *
 *     Function: wrUmmX2TgtHoPhyUeCfg
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
PRIVATE S16 wrUmmX2TgtHoPhyUeCfg
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   CtfCfgReqInfo             *cfgInfo;
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &transCb->u.x2TgtHoTrans;


   WR_ALLOC(&cfgInfo, sizeof(CtfCfgReqInfo));
   if (cfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }
   if (wrUmmPhyFillUeCfg(transCb->transId, ueCb, cfgInfo) != ROK)
   {
      RLOG0(L_ERROR, "Req preperation failed");
      RETVALUE(ROK);
   }
   if (wrIfmPhyUeCfg(cfgInfo, transCb->transId) != ROK)
   {
      RLOG0(L_ERROR, "WrSndCtfUeCfg failed");
      RETVALUE(RFAILED);
   }
   x2TgtHoTrans->phyCfg = WR_UMM_TGT_HO_PHY_CFG_SENT;
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to allocate resorces to newly admitted UE.
 *
 * @details
 *    Function: wrUtlHoRscAlloc
 *
 *    Processing steps:
 *       -# Init UE Control Block
 *       -# Update SRB1 related information into UeCb
 *       -# Update SRB2 related information into UeCb
 *       -# Configure PHY Layer.
 *       -# Trigger scheduler Configuration
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUtlHoRscAlloc
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrS1ConCb                 *s1ConCb;
   U32                       suConId;
   /* ccpu00131426 - Fix */
   RmuUeHoRsp                *hoAdmitRspInfo = NULLP;

   WR_GET_S1AP_CON_ID(suConId, ueCb);
   WR_ALLOC(&s1ConCb, sizeof(WrS1ConCb));
   if (NULLP == s1ConCb) 
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of the if condition */
   s1ConCb->suConnId       = suConId;
   s1ConCb->s1apConnState  = WR_S1AP_CONNECTING;
   s1ConCb->enb_ue_s1ap_id = suConId;
   ueCb->s1ConCb           = s1ConCb;
   /**
    * Init UE Control block
    */
   /*ccpu00129650*/
  /* wrUmmX2TgtHoInitUeCb(ueCb);*/

   /* ccpu00131426 - Fix */
   /* Commit the Radio Resource Config Parameters */
    hoAdmitRspInfo = transCb->u.x2TgtHoTrans.rrmUeRecfgRspMsg->u.rrmUeHoResp;
    wrUmmCommitUeRadioResDedInfo(transCb->ueCb,&(hoAdmitRspInfo->stUeRadioResCfg));

   /**
    * Update SRB1 related information in the UeCb.
    */
   wrUmmRrcFillUeSrbInfo(&ueCb->rbInfo, WR_PDCP_ID_SRB1);
   /**
    * Update SRB2 related information in the UeCb.
    */
   wrUmmRrcFillUeSrbInfo(&ueCb->rbInfo, WR_PDCP_ID_SRB2);
   /**
    * configure phy
    */
   wrUmmX2TgtHoPhyUeCfg(transCb);

   /**
    * Configure Scheduler
    */
   if (ROK != wrUmmX2TgtHoCfgSchd(transCb))
   {
      RLOG0(L_ERROR, "Scheduler configuration failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/* End of wrUtlHoRscAlloc*/

/**
 * @brief 
 *    This function is used to Fill Ue Context Release message.
 *
 * @details
 *    Function: wrUmmX2TgtHoFillX2UeCntxtRls
 *
 *    Processing steps:
 *       -# Fill UE Context Release Message. *
 *
 * @param[in] 
 *    transCb    : transaction information
 * @param[in]
 *    pdu        : Event Pdu
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoFillX2UeCntxtRls
(
 WrUmmTransCb                *transCb,
 CztEvnt                     **pdu
 )
{
   CztEvnt    *ueCntxtRlsPdu = NULLP;
   S16        numComp = 0;
   S16        ret = 0;
   CztProtIE_Field_UECntxtRls_IEs *ie = NULLP;

   /**
    * Initialize memory control point
    */
   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE, &wrCb.mem, (Ptr *)&ueCntxtRlsPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   } /* end of if statement */
   cmMemset((U8 *)&(ueCntxtRlsPdu->pdu), 0, sizeof(CztX2AP_PDU));
   wrFillTknU8(&(ueCntxtRlsPdu->pdu.choice), X2AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(ueCntxtRlsPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   wrFillTknU32(&(ueCntxtRlsPdu->pdu.val.initiatingMsg.procedureCode), Cztid_uECntxtRls);
   wrFillTknU32(&(ueCntxtRlsPdu->pdu.val.initiatingMsg.criticality),
         CztCriticalityignoreEnum);
   /**
    * Fill the requested IEs
    */
   numComp = 2;
   wrFillTknU8(&(ueCntxtRlsPdu->pdu.val.initiatingMsg.value.u.cztUECntxtRls.pres),
         PRSNT_NODEF);
   wrFillTknU16(&(ueCntxtRlsPdu->pdu.val.initiatingMsg.value.u.cztUECntxtRls.
            protocolIEs.noComp),numComp);
   if ((cmGetMem(ueCntxtRlsPdu, (numComp * sizeof(CztProtIE_Field_UECntxtRls_IEs)),
               (Ptr *)&ueCntxtRlsPdu->pdu.val.initiatingMsg.value.u.cztUECntxtRls.protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /**
    * Fill the Old EnB UE X2AP ID
    */
   ie = &ueCntxtRlsPdu->pdu.val.initiatingMsg.value.u.cztUECntxtRls.protocolIEs.member[0];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_Old_eNB_UE_X2AP_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.cztid_Old_eNB_UE_X2AP_ID), transCb->u.x2TgtHoTrans.old_eNB_UE_X2ap_Id);

   /**
    * Fill the New EnB UE X2AP ID
    */
   ie = &ueCntxtRlsPdu->pdu.val.initiatingMsg.value.u.cztUECntxtRls.protocolIEs.member[1];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Cztid_New_eNB_UE_X2AP_ID);
   wrFillTknU32(&(ie->criticality), CztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.cztid_New_eNB_UE_X2AP_ID), transCb->u.x2TgtHoTrans.new_eNB_UE_X2ap_Id);

   *pdu = ueCntxtRlsPdu;
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoFillX2UeCntxtRls*/

/**
 * @brief 
 *    This function is used to UE Context Release Message to the peer eNB.
 *
 * @details
 *    Function: wrUmmX2TgtHoSndUeCtxtRls
 *
 *    Processing steps:
 *       -# Fill UE Context Release Message.
 *       -# Send UE context Release Message.       
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoSndUeCtxtRls
(
WrUmmTransCb                 *transCb
)
{
   CztEvnt                  *ue_Cntxt_Release = NULLP;
   /**
    * Fill UE Context Release Message.
    */
   if (ROK != wrUmmX2TgtHoFillX2UeCntxtRls(transCb, &ue_Cntxt_Release))
   {
      RLOG0(L_ERROR, "Failed to fill UE Context Release Message.");
      RETVALUE(RFAILED);
   }/* End of wrUmmX2TgtHoFillX2UeCntxtRls*/

   /**
    * Filling peer ID.
    */
   ue_Cntxt_Release->peerId = transCb->u.x2TgtHoTrans.peerId;

   RLOG_ARG2(L_EVENT,DBG_CRNTI, transCb->ueCb->crnti,"[X2AP]:Sending UE CONTEXT RELEASE"
   "[Old-eNB-UE-X2AP-ID:%d][New-eNB-UE-X2AP-ID:%d]",
      transCb->u.x2TgtHoTrans.old_eNB_UE_X2ap_Id,
      transCb->u.x2TgtHoTrans.new_eNB_UE_X2ap_Id);
   /* Send Ue Context Release Message to source eNB.*/
   WrIfmX2apBmpReq(ue_Cntxt_Release);
   
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoSndUeCtxtRls*/

/**
 * @brief 
 *    This function is used to fill Cause.
 *
 * @details
 *    Function: wrUtlFillX2Cause
 *
 *    Processing steps:
 *       -# Fill the Cause value       
 *
 * @param[in] 
 *    transCause    : Cause information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUtlFillX2Cause
(
U8              causeChoice,
U8              causeValue,
CztCause       *cause
)
{
    /**
     * Fill CztCause Value.
     */   
   wrFillTknU8(&(cause->choice), causeChoice);
    switch (causeChoice)
    {
        case WR_CAUSE_RADIONW:
                        cause->val.radioNw.pres = PRSNT_NODEF;
         wrFillTknU32(&(cause->val.radioNw), causeValue);
         break;
      case WR_CAUSE_TRANSPORT:
         cause->val.transport.pres =  PRSNT_NODEF;
         wrFillTknU32(&(cause->val.transport), causeValue);
         break;
            /*ccpu00128672*/
         /*The Cause Values for X2AP is different from S1AP in case of PROTOCOL
          * & MISC, so replacing with CZT_CAUSE in those 2 Cause Scenarios*/
      case CZT_CAUSE_PROTOCOL:
         cause->val.protocol.pres= PRSNT_NODEF;
         wrFillTknU32(&(cause->val.protocol), causeValue);
         break;
      case CZT_CAUSE_MISC:
              cause->val.misc.pres = PRSNT_NODEF;
         wrFillTknU32(&(cause->val.misc), causeValue);
         break;
      default :
         RLOG0(L_ERROR, "Invalid Cause");
         RETVALUE(RFAILED);
    }/* End of Switch(choice)*/
    RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to Fill Handover Preparation Failure Message.
 *
 * @details
 *    Function: wrUmmX2TgtHoFillHovrPrepFailure
 *
 *    Processing steps:
 *       -# Fill Handover Preparation Failure Message.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @param[in]
 *    hovrReqAckFailpdu : Hanover Request Failure message PDU.
 * @param[in]
 *    causeChoice : choice of cause
 * @param[in]
 *    causeValue  : value of cause
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoFillHovrPrepFailure
(
WrUmmTransCb    *transCb, 
CztEvnt         **hovrReqAckFailpdu     
)
{
    CztEvnt                           *pdu;
   CztProtIE_Field_HovrPrepFail_IEs  *ie = NULLP;
   U8                                ret;
   U8                                numComp = 0;
   

   ret = cmAllocEvnt(sizeof(CztEvnt), WR_CZ_MEM_SDU_SIZE, &wrCb.mem, (Ptr *)&pdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   } /* end of if statement */
    cmMemset((U8 *)&(pdu->pdu), 0, sizeof(CztX2AP_PDU));
   wrFillTknU8(&(pdu->pdu.choice), X2AP_PDU_UNSUCCESSFULOUTCOME);
   wrFillTknU8(&(pdu->pdu.val.unsuccessfulOutcome.pres), PRSNT_NODEF);
   wrFillTknU32(&(pdu->pdu.val.unsuccessfulOutcome.procedureCode), Cztid_handoverPrep);
   wrFillTknU32(&(pdu->pdu.val.unsuccessfulOutcome.criticality),CztCriticalityrejectEnum);
   /**
    * Fill the requested IEs
    */
   numComp = 2;
   wrFillTknU8(&(pdu->pdu.val.unsuccessfulOutcome.value.u.cztHovrPrepFail.pres),PRSNT_NODEF);
   wrFillTknU16(&(pdu->pdu.val.unsuccessfulOutcome.value.u.cztHovrPrepFail.protocolIEs.noComp),numComp);
   
   if ((cmGetMem(pdu, (numComp * sizeof(CztProtIE_Field_HovrPrepFail_IEs)),
    (Ptr *)&pdu->pdu.val.unsuccessfulOutcome.value.u.cztHovrPrepFail.protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /**
    * Fill the Old EnB UE X2AP ID
    */
    ie = &pdu->pdu.val.unsuccessfulOutcome.value.u.cztHovrPrepFail.protocolIEs.member[0];
    wrFillTknU8(&(ie->pres), PRSNT_NODEF);
    wrFillTknU32(&(ie->id), Cztid_Old_eNB_UE_X2AP_ID);
    wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);
    wrFillTknU32(&(ie->value.u.cztUE_X2AP_ID), transCb->u.x2TgtHoTrans.old_eNB_UE_X2ap_Id);

   /**
    * Fill CztCause
    */
    ie = &pdu->pdu.val.unsuccessfulOutcome.value.u.cztHovrPrepFail.protocolIEs.member[1];
    wrFillTknU8(&(ie->pres), PRSNT_NODEF);
    wrFillTknU32(&(ie->id), Cztid_Cause);
    wrFillTknU32(&(ie->criticality), CztCriticalityignoreEnum);    
    if (ROK != wrUtlFillX2Cause(
          transCb->u.x2TgtHoTrans.hoPrepFailCause.causeTyp, 
          transCb->u.x2TgtHoTrans.hoPrepFailCause.causeVal,
          &ie->value.u.cztCause))
    {
      RLOG0(L_ERROR, "Failed To Fill X2 Cause");
      RETVALUE(RFAILED);
    }
   *hovrReqAckFailpdu = pdu;
    RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to Fill and send Handover Preparation Failure Message to the peer eNB.
 *
 * @details
 *    Function: wrUmmX2TgtHoSndPrepFailure
 *
 *    Processing steps:
 *       -# Fill Handover Preparation Failure Message.
 *       -# Send Handover Preparation Failure Message to the peer eNB.
 * @param[in] 
 *    transCb    : transaction information
 * @param[in]
 *    causeChoice : choice of cause
 * @param[in]
 *    causeValue  : value of cause
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoSndPrepFailure
(
 WrUmmTransCb *transCb    
)
{
    CztEvnt                  *hovrReqAckFailpdu;
    
    /**
     * Filling of Handover Request Ack Failure.
     */
   if (ROK != wrUmmX2TgtHoFillHovrPrepFailure(transCb, &hovrReqAckFailpdu))  
   {
      RLOG0(L_ERROR, "Failed to fill Handover Request Ack Message.");
       RETVALUE(RFAILED);
   }
   hovrReqAckFailpdu->peerId = transCb->u.x2TgtHoTrans.peerId;
   /**
    * Send Handover Request Ack message.
    */
   WrIfmX2apBmpRsp(hovrReqAckFailpdu);
   RLOG_ARG2(L_EVENT,DBG_CRNTI, transCb->ueCb->crnti,"[X2AP]:Sending HANDOVER PREPARATION FAILURE"
   "[Old-eNB-UE-X2AP-ID:%d][New-eNB-UE-X2AP-ID:%d]",
         transCb->u.x2TgtHoTrans.old_eNB_UE_X2ap_Id,
         transCb->u.x2TgtHoTrans.new_eNB_UE_X2ap_Id);

    RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to Fill UE Security Information.
 *
 * @details
 *    Function: wrUmmX2TgtHoFillUeSecCfg
 *
 *    Processing steps:
 *       -# Fill UE Security Configuration
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @param[in]
 *    ue_SecurCapabilities : to fill UE Security Capabilities.
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoFillUeSecCfg
(
 WrUmmTransCb                *transCb,
 CztUESecurCapabilities      *ue_SecurCapabilities
 )
{
    WrUmmX2TgtHoTrans        *x2TgtHoTrans = NULLP;

    x2TgtHoTrans = &transCb->u.x2TgtHoTrans;
    /**
     * Allocate the Memory.
     */
    WR_ALLOC(&x2TgtHoTrans->secCfg, sizeof(WrUeSecCfg));
    if (x2TgtHoTrans->secCfg == NULLP)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
       RETVALUE(RFAILED);
    }/* End of x2TgtHoTrans.secCfg == NULLP*/

#ifdef TENB_AS_SECURITY
#if  (defined SS_RBUF) || (defined LTE_QCOM)
    WR_ALLOC(&x2TgtHoTrans->hzSecCfg, sizeof(WrUeSecCfg));
    if (x2TgtHoTrans->hzSecCfg == NULLP)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
       RETVALUE(RFAILED);
    }/* End of x2TgtHoTrans.secCfg == NULLP*/
    /* store the UE capability info */
#endif
#endif
   wrUmmSecGetSecCap((SztUESecurCapabilities*)ue_SecurCapabilities, x2TgtHoTrans->secCfg);
   /* CSG_DEV */
   x2TgtHoTrans->isEmrgncyCall = wrUmmHoIsEmgncyCall(x2TgtHoTrans->secCfg);
    

#ifdef TENB_AS_SECURITY
#if  (defined SS_RBUF) || (defined LTE_QCOM)
   wrUmmSecGetSecCap((SztUESecurCapabilities*)ue_SecurCapabilities, x2TgtHoTrans->hzSecCfg);
   WR_MEM_COPY(&x2TgtHoTrans->hzSecCfg->encrypCap, &ue_SecurCapabilities->integrityProtectionAlgorithms, 
               sizeof(TknBStr32));
   WR_MEM_COPY(&x2TgtHoTrans->hzSecCfg->integCap, &ue_SecurCapabilities->encryptionAlgorithms, 
               sizeof(TknBStr32));
#endif
#endif
    RETVALUE(ROK);
}/* End of wrUmmX2TgtHoFillUeSecCfg*/

/*UE History info - start*/
/**
 * @brief 
 *    This function is used to Fill UE History Information List.
 *
 * @details
 *    Function: wrUmmX2TgtHoStoreUeHistoryInform
 *
 *    Processing steps:
 *       -# Store UE Handover Restriction List.
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @param[in]
 *    ueHistoryInform : to store UE History Information.
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoStoreUeHistoryInform
(
 WrUmmTransCb                *transCb,
CztUE_HistoryInform          *historyInfo
 )
{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUeHstryEutranInfo       *eutraCellInfo;
   CztLastVisitedCell_Item   *cellItem;
   U8                        cellIdx = 0;
   TknStrOSXL                *utraCellInfo = NULLP;

   /* Update lastVisitedCell List.*/
   cellItem = &historyInfo->member[cellIdx];

   ueCb->ueHstryInfo.numOfCells = historyInfo->noComp.val;

   /* Allocate the Memory.*/ 
   for(cellIdx = 0; cellIdx < historyInfo->noComp.val; cellIdx++)
   {
      ueCb->ueHstryInfo.cellInfo[cellIdx].ratType =
         cellItem[cellIdx].choice.val;
      switch (cellItem[cellIdx].choice.val)
      {
         case LASTVISITEDCELL_ITEM_E_UTRAN_CELL:
         {
            eutraCellInfo = &(ueCb->ueHstryInfo.cellInfo[cellIdx].
                            ratInfo.eutranInfo);

            /* parse ECGI ID */
            wrUtlGetPlmnId(&eutraCellInfo->ecgi.plmnId,&cellItem[cellIdx].
                           val.e_UTRAN_Cell.global_Cell_ID.pLMN_Identity);
            wrUtlGetU32FrmBStr32(&eutraCellInfo->ecgi.eutranCellId,
                           &cellItem[cellIdx].val.e_UTRAN_Cell.global_Cell_ID.
                           eUTRANcellIdentifier);
            /* Receiving cell type */
            eutraCellInfo->cellSize = cellItem[cellIdx].
               val.e_UTRAN_Cell.cellTyp.cell_Size.val;
            /* Receiving time UE stayed in cell */
            eutraCellInfo->timeInCell = cellItem
               [cellIdx].val.e_UTRAN_Cell.time_UE_StayedInCell.val;
         }
         break;

         case LASTVISITEDCELL_ITEM_UTRAN_CELL:
         {
            utraCellInfo = &(ueCb->ueHstryInfo.cellInfo[cellIdx].
                           ratInfo.utranInfo);
            utraCellInfo->pres = TRUE;
            utraCellInfo->len = cellItem[cellIdx].val.uTRAN_Cell.len ;
            WR_ALLOC(&utraCellInfo->val,utraCellInfo->len * sizeof(U8));
            if(utraCellInfo->val == NULLP)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }
            cmMemcpy(utraCellInfo->val,
                 cellItem[cellIdx].val.uTRAN_Cell.val,utraCellInfo->len);
         }
         break;
         case LASTVISITEDCELL_ITEM_GERAN_CELL:
         {
           /* Information for GERAN is currently undefined as per 3GPP *
             * Need to update GERAN as necessary later */
         }
         break;
      }
   }
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoStoreUeHistoryInform.*/
/*UE History info - end*/

/**
 * @brief 
 *    This function is used to Fill UE Rab Information.
 *
 * @details
 *    Function: wrUmmX2TgtHoFillUeRabInform
 *
 *    Processing steps:
 *       -# Fill UE Rab Information.
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @param[in]
 *    ue_CntxtInform : to fill UE Context Information.
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoFillUeRabInform
(
WrUmmTransCb                 *transCb,
CztUE_CntxtInform            *ue_CntxtInform
)
{
   WrUmmX2TgtHoTrans             *x2TgtHoTrans = NULLP;
   CztE_RABs_ToBeSetup_Lst       *e_RABS_ToBeSetupLst = NULLP;
   CztE_RABs_ToBeSetup_Item      *erabItem;
   CztGBR_QosInform              *gbrQosInform;
   CztE_RAB_Lvl_QoS_Params       *qosParams;
   CztAllocnAndRetentionPriority *allocRetPrior;
   CztGTPtunnelEndpoint          *uL_GTPtunnelEndpoint = NULLP;
   CztProtIE_Single_Cont_E_RABs_ToBeSetup_ItemIEs  *ies;
   /*CztGTP_TEI                    *remTeIdStr = NULLP; */
   U8                            erabIdx = 0;
   U32                           dlBitRate = 0;
   U32                           ulBitRate = 0;
   WrUmmX2TgtHoRabInfo           *rbInfo;
   U8                             shiftBits = 0;
   U16                            indx = 0;
   U32                            addrMask = 0;
   U16                            cellId = transCb->ueCb->cellId;
   /* HO_RLC_PDCP_CFG */
   WrUmmCellCb                                     *cellCb;

   WR_UMM_GET_CELLCB(cellCb, cellId);
   if (cellCb == NULLP)
   {        
     /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)transCb->ueCb->cellId,
            "Error in getting CELL CB");
       /* Klockwork P1 warning fix */
     RETVALUE(RFAILED);
   }

   x2TgtHoTrans = &transCb->u.x2TgtHoTrans;
   /* Update eRABs To Be Setup List.*/
   e_RABS_ToBeSetupLst = &ue_CntxtInform->e_RABs_ToBeSetup_Lst;

   /* Allocate Memory.*/
   WR_ALLOC(&x2TgtHoTrans->rbInfo,
       (e_RABS_ToBeSetupLst->noComp.val * sizeof(WrUmmX2TgtHoRabInfo)));
   if (NULLP == x2TgtHoTrans->rbInfo)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   rbInfo = x2TgtHoTrans->rbInfo;

   /* Update UE Aggregate Max Bit rate.*/
   wrGetBitRate(&ue_CntxtInform->uEaggregateMaxBitRate.uEaggregateMaxBitRateDlnk, &dlBitRate);
   x2TgtHoTrans->ambr.dl = dlBitRate;
   wrGetBitRate(&ue_CntxtInform->uEaggregateMaxBitRate.uEaggregateMaxBitRateUlnk, &ulBitRate);
   x2TgtHoTrans->ambr.ul = ulBitRate;

   for (erabIdx = 0; erabIdx < e_RABS_ToBeSetupLst->noComp.val; erabIdx++)
   {
      ies = &e_RABS_ToBeSetupLst->member[erabIdx];
      erabItem = &ies->value.u.cztE_RABs_ToBeSetup_Item;
      rbInfo[erabIdx].drbInfo.eRABId  = erabItem->e_RAB_ID.val;

      qosParams = &(erabItem->e_RAB_Lvl_QoS_Params);
      rbInfo[erabIdx].drbInfo.qci = qosParams->qCI.val;

      wrUtlGetRbMode(cellId, rbInfo[erabIdx].drbInfo.qci,
            &(rbInfo[erabIdx].drbInfo.rbMode));
      wrUtlGetRbDir(cellId, rbInfo[erabIdx].drbInfo.qci,
            &(rbInfo[erabIdx].drbInfo.rbDir));

      gbrQosInform = &(qosParams->gbrQosInform);
      if( gbrQosInform->pres.pres == TRUE)
      {
         WR_GET_U32_FRM_OSXL(dlBitRate, gbrQosInform->e_RAB_MaxBitrateDL);
         rbInfo[erabIdx].drbInfo.mbr.dl = dlBitRate;
         WR_GET_U32_FRM_OSXL(ulBitRate, gbrQosInform->e_RAB_MaxBitrateUL);
         rbInfo[erabIdx].drbInfo.mbr.ul = ulBitRate;
         WR_GET_U32_FRM_OSXL(dlBitRate , gbrQosInform->e_RAB_GuaranteedBitrateDL);
         rbInfo[erabIdx].drbInfo.gbr.dl = dlBitRate;
         WR_GET_U32_FRM_OSXL(ulBitRate, gbrQosInform->e_RAB_GuaranteedBitrateUL);
         rbInfo[erabIdx].drbInfo.gbr.ul = ulBitRate;
      }

      /* ARP values to be filled */
      allocRetPrior = &qosParams->allocationAndRetentionPriority;
      if(allocRetPrior->pres.pres == PRSNT_NODEF)
      {
         rbInfo[erabIdx].drbInfo.priority = allocRetPrior->priorityLvl.val;
         rbInfo[erabIdx].drbInfo.preempCap = allocRetPrior->pre_emptionCapblty.val;
         rbInfo[erabIdx].drbInfo.preempVul = allocRetPrior->pre_emptionVulnerability.val;

         if(rbInfo[erabIdx].drbInfo.priority == cellCb->rabArpForEmerServ)
         {
            /* Its an emergency bearer */
            rbInfo[erabIdx].drbInfo.isEmergencyBearer = TRUE;
            transCb->ueCb->ueServTyp = WR_UMM_EMERGENCY_SERV;
         }
      }

      if(erabItem->dL_Fwding.pres)
      {
         rbInfo[erabIdx].dlFwding = TRUE;
      }

      /* Fill tunnel information */
      uL_GTPtunnelEndpoint = &erabItem->uL_GTPtunnelEndpoint; 
      /* Store the remote tunnel information */
      /* Retrieve the remote S-GW's EGTP-U tunnel Id */
      shiftBits = uL_GTPtunnelEndpoint->gTP_TEID.len;
      addrMask = 0xFF000000;
      for(indx = 0;indx < uL_GTPtunnelEndpoint->gTP_TEID.len;indx++)
      {
         shiftBits--;
         rbInfo[erabIdx].drbInfo.remEgtpTunId |=
            ((U32)(uL_GTPtunnelEndpoint->gTP_TEID.val[indx] << (8*shiftBits)) & addrMask);
         addrMask = addrMask>>8;
      }
      /* store the SGW IP Address */
      switch(uL_GTPtunnelEndpoint->transportLyrAddr.len)
      {
         case 32:
         {
            rbInfo[erabIdx].drbInfo.sgwAddr.type = CM_TPTADDR_IPV4;
            rbInfo[erabIdx].drbInfo.sgwAddr.u.ipv4TptAddr.port = WR_DFLT_EGTP_PORT;

            /* copy 4bytes into the U32 */
            shiftBits = uL_GTPtunnelEndpoint->transportLyrAddr.len / 8;
            addrMask = 0xFF000000;
            for(indx = 0; indx < 4; indx++)
            {
               shiftBits--;
               rbInfo[erabIdx].drbInfo.sgwAddr.u.ipv4TptAddr.address |=
                  ((U32)(uL_GTPtunnelEndpoint->transportLyrAddr.val[indx] 
                     << (8 * shiftBits)) & addrMask);
               addrMask = addrMask >> 8;
            }
            break;
         }
         case 128:
         {
            rbInfo[erabIdx].drbInfo.sgwAddr.type = CM_TPTADDR_IPV6;
            rbInfo[erabIdx].drbInfo.sgwAddr.u.ipv6TptAddr.port = WR_DFLT_EGTP_PORT;
            for (indx = 0; indx < 16; indx++)
            {
               rbInfo[erabIdx].drbInfo.sgwAddr.u.ipv6TptAddr.ipv6NetAddr[indx]=
                  uL_GTPtunnelEndpoint->transportLyrAddr.val[indx];
            }
            break;
         }
         default:
         {
            /* This means both IPv4 and IPv6 addresses are present. We are */
            /* yet to support this option                                  */
            x2TgtHoTrans->hoPrepFailCause.causeTyp = WR_CAUSE_PROTOCOL;
            x2TgtHoTrans->hoPrepFailCause.causeVal =
               CztCauseProtmessage_not_compatible_with_receiver_stateEnum; 
            RETVALUE(RFAILED);  
         }
      }
      /* HO_RLC_PDCP_CFG */
      /* create a copy of PDCP RLC configurations */
      cmMemcpy((U8 *)&(rbInfo[erabIdx].srcPdcpRlcCfgParams),
         (CONSTANT U8 *)&cellCb->wrRabCfgParams[rbInfo[erabIdx].drbInfo.qci],
          sizeof(WrRabCfgParams));
   }
   /* Updating No of DRBs are configured for the UE.*/
   x2TgtHoTrans->numOfRbCfgd = erabIdx;

   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoFillUeRabInform*/

/**
 * @brief 
 *    This function is used to Fill UE Context Information
 *
 * @details
 *    Function: wrUmmX2TgtHoFillUeCntxtInform
 *
 *    Processing steps:
 *       -# Fill UE Context Information
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @param[in]
 *    ue_CntxtInform : to fill UE Context Information.
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoFillUeCntxtInform
(
WrUmmTransCb                 *transCb,
CztUE_CntxtInform            *ue_CntxtInform
)
{
   WrUmmX2TgtHoTrans             *x2TgtHoTrans = NULLP;
#ifndef TENB_AS_SECURITY
   U16                           len = 0;
   U16                           keNBStrIdx = 0;
#endif
   NhuDecodeSduReq               *nhuDecodeSduReq = NULLP;
   Buffer                        *mBuf = NULLP;
   Bool                          servPlmnSup = TRUE;
   CztProtExtnField_UE_CntxtInform_ExtIEs *extIE;
   U32                            idx;

   x2TgtHoTrans = &transCb->u.x2TgtHoTrans;
   /**
    * Parsing the UE context Information
    */
   x2TgtHoTrans->mme_ue_s1ap_id = ue_CntxtInform->mME_UE_S1AP_ID.val;

   if (ROK != wrUmmX2TgtHoFillUeSecCfg(transCb, &ue_CntxtInform->uESecurCapabilities))
   {
      RLOG0(L_ERROR, "Failed to Fill UE Security Capabilities.");
      RETVALUE(RFAILED);
   }/* End of wrUmmX2TgtHoFillUeSecCfg*/

   /**
    * Filling of KeNB Star Values.
    */
   /* ccpu00126532 */
#ifdef TENB_AS_SECURITY
#if  !(defined SS_RBUF) && !(defined LTE_QCOM)
   wrPlatSecExtractSecKeys(x2TgtHoTrans->secCfg,
         &(ue_CntxtInform->aS_SecurInform.key_eNodeB_star),
         &x2TgtHoTrans->asKeyGenerated, transCb->transId);
#else
   wrUmmSecExtractX2HOSecKeys3(transCb, &(ue_CntxtInform->aS_SecurInform.key_eNodeB_star),&x2TgtHoTrans->asKeyGenerated,CTF_KDF_TYPE2);
   wrUmmSecExtractX2HOSecKeys3(transCb, &(ue_CntxtInform->aS_SecurInform.key_eNodeB_star),&x2TgtHoTrans->asKeyGenerated,CTF_KDF_TYPE4);
   wrUmmSecExtractX2HOSecKeys3(transCb, &(ue_CntxtInform->aS_SecurInform.key_eNodeB_star),&x2TgtHoTrans->asKeyGenerated,CTF_KDF_TYPE3);
#endif
#else
   len = ue_CntxtInform->aS_SecurInform.key_eNodeB_star.len / 8;

   for (keNBStrIdx = 0; keNBStrIdx < len; keNBStrIdx++)
   {
      x2TgtHoTrans->secCfg->kENB[keNBStrIdx] = 
         ue_CntxtInform->aS_SecurInform.key_eNodeB_star.val[keNBStrIdx];
   }
#endif

   /**
    * Fill Next Hop Chaining Count
    */
   x2TgtHoTrans->nextHopChainingCount = ue_CntxtInform->aS_SecurInform.nextHopChainingCount.val;
   /**
    * FIll UE RAB Information.
    */
   if(ROK != wrUmmX2TgtHoFillUeRabInform(transCb, ue_CntxtInform))
   {
      RLOG0(L_ERROR, "Failed to parse UE RAB Information");
      RETVALUE(RFAILED);
   }/* End of wrUmmX2TgtHoFillUeRabInform*/

   if(ue_CntxtInform->handoverRestrnLst.pres.pres == PRSNT_NODEF)
   {
      /*typecasting CztHovrRestrnLst to SztHovrRestrnLst as
       * both are same and can use same utility function*/
      wrUmmCpyHoRestrnLst(transCb->ueCb,
            (SztHovrRestrnLst*)&ue_CntxtInform->handoverRestrnLst);
      if(transCb->ueCb->ueServTyp == WR_UMM_NORMAL_SERV)
      {
         /* Check whether the serving PLMN of the HRL is supported by 
          * the target or not. */
         servPlmnSup = wrEmmChkServPlmnHoUe(transCb->ueCb->cellId, 
               transCb->ueCb->hoRstrctInfo->srvngPlmn);
      }
   }
   else
   {
      /* If HRL is not received, then consider the PLMN received in the GUMMEI
       * as the serving PLMN of the UE. */
      if(transCb->ueCb->ueServTyp == WR_UMM_NORMAL_SERV)
      {
         /* Check whether the serving PLMN of the GUMMEI is supported by 
          * the target or not. */
         servPlmnSup = wrEmmChkServPlmnHoUe(transCb->ueCb->cellId, 
               x2TgtHoTrans->gummei.plmnId);
      }
   }
   if(servPlmnSup == FALSE)
   {
      /* Serving PLMN is not supported by the eNodeB */
      x2TgtHoTrans->hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW;
      x2TgtHoTrans->hoPrepFailCause.causeVal = 
         CztCauseRadioNwho_target_not_allowedEnum;

      RETVALUE(RFAILED);
   }

   /* ccpu00131919 */
   if(ue_CntxtInform->locationRprtngInform.pres.val == PRSNT_NODEF)
   {
      if(ue_CntxtInform->locationRprtngInform.eventTyp.val == CztEventTypchange_of_serving_cellEnum)
      {
         /* Set the LocRpt flag to TRUE */
         transCb->ueCb->locReportType = TRUE;
      }
   }

   if(ue_CntxtInform->iE_Extns.noComp.pres == PRSNT_NODEF)
   {
      for(idx = 0; idx < ue_CntxtInform->iE_Extns.noComp.val; idx++)
      {
         extIE = &(ue_CntxtInform->iE_Extns.member[idx]);
         switch(extIE->id.val)
         {
            case Cztid_ManagementBasedMDTallowed:
               transCb->ueCb->mgmtBasedMDTAllowed.pres = TRUE;
               transCb->ueCb->mgmtBasedMDTAllowed.val = 
                  extIE->extensionValue.u.cztManagementBasedMDTallowed.val;
               break;

            default:
               break;
         }
      }
   }

   /* Extract and store the subscription profile ID information in
    * ueCb
    */
   if(PRSNT_NODEF == ue_CntxtInform->subscriberProfileIDforRFP.pres)
   {
      transCb->ueCb->subscProfileId.pres = TRUE;
      transCb->ueCb->subscProfileId.val = 
              ue_CntxtInform->subscriberProfileIDforRFP.val;
   }

   /* fill rrc container */
   /* Request RRC to decode the SourceeNB_To_TgteNB Trasparent Container*/
   WR_ALLOC(&nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
   if(NULLP == nhuDecodeSduReq)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/
   nhuDecodeSduReq->hdr.cellId  =   transCb->ueCb->cellId;
   nhuDecodeSduReq->hdr.ueId    =   transCb->ueCb->crnti;
   nhuDecodeSduReq->hdr.transId =   transCb->transId;
   nhuDecodeSduReq->sduType     =   NHU_MSG_HOPREPINFO;
   WR_ALLOCMBUF(wrCb.mem.region, wrCb.mem.pool, &mBuf);
   if(NULLP == mBuf)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/
   
   /* convert the OSXL to Buffer */
   SAddPstMsgMult((Data *)ue_CntxtInform->rRC_Cntxt.val,
         (MsgLen)ue_CntxtInform->rRC_Cntxt.len,
         (Buffer *)mBuf);
   nhuDecodeSduReq->mbuf = mBuf;
   /* Decode the HO Preparation Message.
    */
   if(ROK != wrIfmRrcSndDecodeReq(nhuDecodeSduReq))
   {
      RLOG0(L_ERROR, "Failed to send the RRC message");
       /* Fix for ccpu00125004 */
      WR_FREE(nhuDecodeSduReq, sizeof(NhuDecodeSduReq));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/*End of wrUmmX2TgtHoFillUeCntxtInform*/

/**
 * @brief 
 *    This function is used to fill Cause.
 *
 * @details
 *    Function: wrUmmX2TgtHoFillX2Cause
 *
 *    Processing steps:
 *       -# Fill the Cause value       
 * @param[in]
 *    transCb       : transaction information
 * @param[in] 
 *    transCause    : Cause information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoFillX2Cause
(
 WrUmmTransCb                *transCb,
 CztCause                    *cause
)
{
    /**
     * Filling Cause Values into Transaction Control Block.
     */
    WR_ALLOC(&transCb->u.x2TgtHoTrans.cztCause, sizeof(CztCause));
    if (NULLP == transCb->u.x2TgtHoTrans.cztCause)
    {
       RLOG0(L_FATAL, "Memory allocation failed.");
       RETVALUE(RFAILED);
    }/*End of transCb->u.x2TgtHoTrans.cztCause.*/
    switch (cause->choice.val)
    {
        case WR_CAUSE_RADIONW:
            /**
             * Fill Radio Network Cause value
             */
            transCb->u.x2TgtHoTrans.cztCause->val.radioNw.val = cause->val.radioNw.val;
            break;
        case WR_CAUSE_TRANSPORT:
            /**
             * Fill Transport Cause value
             */
            transCb->u.x2TgtHoTrans.cztCause->val.transport.val = cause->val.transport.val;
            break;

            /*ccpu00128672*/
         /*The Cause Values for X2AP is different from S1AP in case of PROTOCOL
          * & MISC, so replacing with CZT_CAUSE in those 2 Cause Scenarios*/
        case CZT_CAUSE_PROTOCOL:
            /**
             * Fill Protocol Cause value
             */
            transCb->u.x2TgtHoTrans.cztCause->val.protocol.val = cause->val.protocol.val;
            break;
        case CZT_CAUSE_MISC:
            /**
             * Fill MISC Cause Value
             */
            transCb->u.x2TgtHoTrans.cztCause->val.misc.val = cause->val.misc.val;
            break;
        default:
   {
            RLOG0(L_ERROR, "Invalid Cause");
            transCb->u.x2TgtHoTrans.hoPrepFailCause.causeTyp = WR_CAUSE_PROTOCOL;
            transCb->u.x2TgtHoTrans.hoPrepFailCause.causeVal = 
          CztCauseProtsemantic_errorEnum;               
            RETVALUE(RFAILED);
   }
    }/* End of switch*/
    RETVALUE(ROK);
}/* End of wrUmmX2TgtHoFillX2Cause */


/**
 * @brief 
 *    This function is used to fill PLMN Identifier.
 *
 * @details
 *    Function: wrUmmX2TgtHoFillPlmn_IdInToTrans
 *
 *    Processing steps:
 *       -# Fill PLMN Identifier
 * @param[in]
 *    transPlmnId       : transPlmn Id to fill
 * @param[in] 
 *    plmn_Id           : set the PLMN Id
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
/* FIX for ccpu00135294 */
PRIVATE S16 wrUmmX2TgtHoFillPlmn_IdInToTrans
(
WrPlmnId                     *plmnId,
CztPLMN_Identity             *plmnIe
)
{
   plmnId->mcc[0] = (plmnIe->val[0] & 0x0F);
   plmnId->mcc[1] = ((plmnIe->val[0] >> 4) & 0x0F);
   plmnId->mcc[2] = plmnIe->val[1] & 0x0F;
   if ((plmnIe->val[1] & 0xf0) == 0xf0)
   {
      plmnId->numMncDigits = 2;
      plmnId->mnc[0] = (plmnIe->val[2] & 0x0F);
      plmnId->mnc[1] = (plmnIe->val[2] & 0xF0) >> 4;
   }
   else
   {
      plmnId->numMncDigits = 3;    
      plmnId->mnc[0] = (plmnIe->val[1] & 0xf0) >> 4;
      plmnId->mnc[1] = (plmnIe->val[2] & 0x0F);
      plmnId->mnc[2] = (plmnIe->val[2] & 0xf0) >> 4;
   }
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to fill PLMN Identifier.
 *
 * @details
 *    Function: wrUmmX2TgtHoFillPlmn_IdInToTrans
 *
 *    Processing steps:
 *       -# Fill PLMN Identifier
 * @param[in]
 *    transPlmnId       : transPlmn Id to fill
 * @param[in] 
 *    plmn_Id           : set the PLMN Id
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoFillTgtCellId
(
WrUmmTransCb                 *transCb,
CztECGI                      *eCGI
)
{
   U32                       cellId;
   
   /* Fill Target Cell Identifier.*/
   if (ROK != wrUtlGetU32FrmBStr32(&cellId,&eCGI->eUTRANcellIdentifier))/*UE History info*/
   {
      RLOG0(L_ERROR, "Failed To Fill EUTRAN Cell Id Into Trans");
      RETVALUE(RFAILED);
   }
   transCb->u.x2TgtHoTrans.cellId = cellId;
   /* Fill PLMN ID.*/
   if (ROK != wrUmmX2TgtHoFillPlmn_IdInToTrans(&transCb->u.x2TgtHoTrans.plmnId,
      &eCGI->pLMN_Identity))
   {
      RLOG0(L_ERROR, "Failed To Fill PLMN ID Into Trans");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/*End of wrUmmX2TgtHoFillTgtCellId.*/

/**
 * @brief 
 *    This function is used to fill PLMN Identifier.
 *
 * @details
 *    Function: wrUmmX2TgtHoFillPlmn_IdInToTrans
 *
 *    Processing steps:
 *       -# Fill PLMN Identifier
 * @param[in]
 *    transPlmnId       : transPlmn Id to fill
 * @param[in] 
 *    plmn_Id           : set the PLMN Id
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoFillGUMMEI
(
WrUmmTransCb                 *transCb,
CztGUMMEI                    *cztGUMMEI
)
{
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &(transCb->u.x2TgtHoTrans);

   /**
    * Fill GUMMEI Value.
    */
   if(cztGUMMEI->pres.pres == PRSNT_NODEF)
   {
      x2TgtHoTrans->gummei.pres = TRUE;

      /**
       * parse MME PLMN ID
       */
      wrEmmMmeParsePlmnIe(&cztGUMMEI->gU_Group_ID.pLMN_Identity, 
                   &x2TgtHoTrans->gummei.plmnId);

      /**
       * MME Group Id 16 bit len
       */
      x2TgtHoTrans->gummei.mmeGrpId = ((cztGUMMEI->gU_Group_ID.mME_Group_ID.val[1]) |
                                       (cztGUMMEI->gU_Group_ID.mME_Group_ID.val[0]) << 8);

      /**
       * MME Code 8 bit len
       */
      WR_MEM_COPY(&x2TgtHoTrans->gummei.mmec, cztGUMMEI->mME_Code.val,\
                   cztGUMMEI->mME_Code.len);

      /* inter MME HO */

      if(ROK != wrGetMmeIdfromPlmnId(&x2TgtHoTrans->gummei, &x2TgtHoTrans->mmeId, transCb->ueCb->establishCause))
      {
         RLOG2(L_ERROR,"Failed to retrieve the MME details for"
               "MME with GrpId[%d] and code[%d]",
               x2TgtHoTrans->gummei.mmeGrpId,x2TgtHoTrans->gummei.mmec);
         x2TgtHoTrans->errCause = CztCauseRadioNwunknown_MME_CodeEnum;
         RETVALUE(RFAILED);
      }
      /* intr MME HO */
      RLOG_ARG3(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"MME information for "
         "mmec [%d] mmeId [%lu] mmeGrpId [%d]",x2TgtHoTrans->gummei.mmec,
         x2TgtHoTrans->mmeId, x2TgtHoTrans->gummei.mmeGrpId);
   }
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoFillGUMMEI()*/

/**
 * @brief 
 *    This function is used to fill EnbUeX2apId.
 *
 * @details
 *    Function: wrUmmX2TgtHoFillEnbUeX2apId
 *
 *    Processing steps:
 *       -# Fill Enb UE S1ap ID
 * @param[in]
 *    transCb           : transaction Information
 * @param[in] 
 *    eNB_Ue_X2ap_Id    : eNB UE S1ap Id
 * @param[in]
 *    ueIdType          : UE ID type
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoFillEnbUeX2apId
(
WrUmmTransCb                 *transCb,
U32                          eNB_Ue_X2ap_Id,
U8                           ueIdType
)
{
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &(transCb->u.x2TgtHoTrans);

    /**
     * Filling Of eNB UE X2AP ID.
     */
   if (ueIdType == WR_UMM_OLD_ENB_UE_X2AP_ID)
   {
      x2TgtHoTrans->old_eNB_UE_X2ap_Id = eNB_Ue_X2ap_Id;
   }
   else if (ueIdType == WR_UMM_NEW_ENB_UE_X2AP_ID)
   {
      x2TgtHoTrans->new_eNB_UE_X2ap_Id = eNB_Ue_X2ap_Id;
   }
   
    RETVALUE(ROK);
}/* End of wrUmmX2TgtHoFillEnbUeX2apId.*/


/**
 * @brief 
 *    This function is used to Parse all IEs of Handover Request Message and 
 *    stores into transaction control block.
 *
 * @details
 *    Function: wrUmmX2TgtHoParseHoReq
 *
 *    Processing steps:
 *       -# Parse Handover Request message and stores all IEs into transaction
 *          control block for future use.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoParseHoReq
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2TgtHoTrans             *x2TgtHoTrans = &(transCb->u.x2TgtHoTrans);
   CztUE_CntxtInform             *ue_CntxtInform = NULLP;
   CztUE_HistoryInform           *ueHistoryInform = NULLP;
   CztX2AP_PDU                   *x2apPdu = NULLP;
   CztInitiatingMsg              *initiatingMsg = NULLP;
   CztProtIE_Field_HovrRqst_IEs  *ie = NULLP;
   CztCause                      *cztCause = NULLP;
   U8                            hoReqIesidx = 0;
   U8                            noComp;
   U32                           peerId;
   WrUmmCellCb                   *cellCb = NULLP;

   /**
    * Getting the incoming HO Reqest message PDU
    */
   x2apPdu = &transCb->msg->u.x2Pdu->pdu;
   initiatingMsg = &x2apPdu->val.initiatingMsg;

   /**
    * parse peerId and fetch ip address
    */
   peerId = transCb->msg->u.x2Pdu->peerId;
   if(ROK != wrEmmGetNeighIpFrmPeerId(peerId, transCb->ueCb->cellId,
                         &x2TgtHoTrans->peerAddr))
   {
      RLOG1(L_ERROR, "Invalid PeerId [%ld]", peerId);
      transCb->u.x2TgtHoTrans.hoPrepFailCause.causeTyp = WR_CAUSE_PROTOCOL;
      transCb->u.x2TgtHoTrans.hoPrepFailCause.causeVal =
         CztCauseProtmessage_not_compatible_with_receiver_stateEnum;
      RETVALUE(RFAILED);
   }

   WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)transCb->ueCb->cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   if (cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS)
   {
      x2TgtHoTrans->ueMbrShpSts = WR_UMM_CSG_MEMBERSHIP_NOT_KNOWN;
   }
   else
   {
      /* membership true for closed and open access */
      x2TgtHoTrans->ueMbrShpSts = WR_UMM_CSG_MEMBER;
   }

   noComp = initiatingMsg->value.u.cztHovrRqst.protocolIEs.noComp.val;
   for (hoReqIesidx = 0; hoReqIesidx < noComp; hoReqIesidx++)
   {
      ie = &initiatingMsg->value.u.cztHovrRqst.protocolIEs.member[hoReqIesidx];
      switch (ie->id.val)
      {
         case Cztid_Old_eNB_UE_X2AP_ID:
         {
            /**
             * Fill eNB UE X2AP ID
             */
            wrUmmX2TgtHoFillEnbUeX2apId(transCb, 
                     ie->value.u.cztUE_X2AP_ID.val,
                     WR_UMM_OLD_ENB_UE_X2AP_ID);
            break;
         }
         case Cztid_Cause:
         {
            cztCause = &ie->value.u.cztCause;
            /**
             * Fill Cause
             */
            if (ROK != wrUmmX2TgtHoFillX2Cause(transCb, cztCause))
            {
               RLOG0(L_ERROR, "Failed to Fill X2 Cause");
               RETVALUE(RFAILED);
            }/* End of wrUmmX2TgtHoFillX2Cause */
            break;
         }
         case Cztid_TgetCell_ID:
         {
            /**
             * Fill Target Cell ID.
             */
            if (ROK != wrUmmX2TgtHoFillTgtCellId(transCb, &ie->value.u.cztECGI))
            {
               RLOG0(L_ERROR, "Failed to Fill Target Cell ID");
               RETVALUE(RFAILED);
            }/* End of wrUmmX2TgtHoFillTgtCellId*/
            break;
         }
         case Cztid_GUMMEI_ID:
         {
            /**
             * Fill GUMMEI_ID Value.
             */
            if (ROK != wrUmmX2TgtHoFillGUMMEI(transCb,&ie->value.u.cztGUMMEI))
            {
               RLOG0(L_ERROR, "Failed to Fill GUMMEI ID");
               RETVALUE(RFAILED);
            }/* End of wrUmmX2TgtHoFillGUMMEI()*/
            break;
         }
         case Cztid_UE_CntxtInform:
         {
            /**
             * Parse UE context information.
             */
            ue_CntxtInform = &ie->value.u.cztUE_CntxtInform;
            if (ROK != wrUmmX2TgtHoFillUeCntxtInform(transCb, ue_CntxtInform))
            {
               RLOG0(L_ERROR, "Failed to Parse UE Context Information.");
               RETVALUE(RFAILED);
            }/* End of if Condition wrUmmX2TgtHoFillUeCntxtInform.*/
            break;
         }
         case Cztid_UE_HistoryInform:
         {
            /**
             * Parse UE History Information
             */
            ueHistoryInform = &ie->value.u.cztUE_HistoryInform;
            if (ROK != wrUmmX2TgtHoStoreUeHistoryInform(transCb,ueHistoryInform))
            {
               RLOG0(L_ERROR, "Failed to Parse UE History Information.");/*UE History info*/
               RETVALUE(RFAILED);
            }/* End of wrUmmX2TgtHoStoreUeHistoryInform*/
            break;
         }
         case Cztid_TraceActvn:
            break;
         /* CSG_DEV start */
         case Cztid_CSGMembershipStatus:
         {
            if(CztCSGMembershipStatusmemberEnum == 
                  ie->value.u.cztCSGMembershipStatus.val)
            {
               x2TgtHoTrans->ueMbrShpSts = WR_UMM_CSG_MEMBER;
            }
            else
            {
               x2TgtHoTrans->ueMbrShpSts = WR_UMM_CSG_NOT_MEMBER;
            }
         }
         break;
         
         case Cztid_SRVCCOperationPossible:
         {
            if((ie->value.u.cztSRVCCOperationPossible.pres == TRUE) &&
               (CztSRVCCOperationPossiblepossibleEnum == 
                  ie->value.u.cztSRVCCOperationPossible.val))
            {
               transCb->ueCb->srvccOperPossible = TRUE;
            }
         }
         break;
         /* CSG_DEV end */

         default :
            break;
      }/* End of switch ie->id.val */
   }/* End of for loop */

   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoParseHoReq*/

/**
 * @brief 
 *    This function is used to fill Tunnel Details from Path Switch Ack message.
 *
 * @details
 *    Function: wrUmmUpdUlTunPthSwtAck
 *
 *    Processing steps:
 *       -# Fill Tunnel information from path switch Ack.
 *
 * @param[in] 
 *    rbInfo       : rab information
 * @param[in]
 *    s1ap_Pdu     : Event PDU
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmUpdUlTunPthSwtAck
(
WrUmmTransCb                 *transCb,
SztS1AP_PDU                  *s1ap_Pdu
)
{
   U8                        erabId = 0;
   U8                        rbId = 0;
   U8                        shiftBits = 0;
   U16                       ipAddrIdx = 0;
   U32                       addrMsk = 0;
   SztE_RABToBeSwedULLst     *eRABLst = NULLP;
   SztE_RABToBeSwedULItem    *eRABItem = NULLP;
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &(transCb->u.x2TgtHoTrans);
   WrUmmX2TgtHoRabInfo       *rbInfo;
   WrDamTnlModInfo           *tnlModInfo = NULLP;
   U16                       indx;

   rbInfo = x2TgtHoTrans->rbInfo;

    /**
     * fetch the ERAB Admitted list from the Path Switch Ack PDU
     */
    if(ROK != wrSzGetIE(s1ap_Pdu, Sztid_E_RABToBeSwedULLst, (TknU8 **) &eRABLst))
    {
        RLOG0(L_ERROR, "Failed to veSzGetIE.");
        RETVALUE(RFAILED);
    }
    if (eRABLst == NULLP)
    {
       /**
        * NO change in UL tunnel information
        */
       RETVALUE(ROK);
    }
    /* ccpu00126336 */
    /* Update the number of response expected from DAM */
    rbInfo->expectedTnlRsp = eRABLst->noComp.val;
    for(erabId = 0; erabId < eRABLst->noComp.val; erabId++)
    {
      eRABItem = &eRABLst->member[erabId].value.u.sztE_RABToBeSwedULItem;
      for (rbId = 0; rbId < x2TgtHoTrans->numOfRbCfgd; rbId++)
      {
         if(rbInfo[rbId].drbInfo.eRABId == eRABItem->e_RAB_ID.val)
         {
            /**
             * Copy UL Tunnel info
             */
            if(eRABItem->gTP_TEID.pres == PRSNT_NODEF)
            {
               shiftBits = eRABItem->gTP_TEID.len;
               addrMsk = 0xFF000000;
               for(indx = 0;indx < eRABItem->gTP_TEID.len;indx++)
               {
                  shiftBits--;
                  rbInfo[rbId].newRemTnlId |=
                     ((U32)(eRABItem->gTP_TEID.val[indx] << (8*shiftBits)) & addrMsk);
                  addrMsk = addrMsk>>8;
               }

               /**
                * retrieve Serv. GW Address later will be used for tunnel creation
                */
               switch(eRABItem->transportLyrAddr.len)
               {
                  case 32:
                     rbInfo[rbId].newSgwAddr.type = CM_TPTADDR_IPV4;
                     rbInfo[rbId].newSgwAddr.u.ipv4TptAddr.port = WR_DFLT_EGTP_PORT;
                     /**
                      * copy 4bytes into the U32
                      */
                     shiftBits = eRABItem->transportLyrAddr.len / 8;
                     addrMsk = 0xFF000000;
                     for(ipAddrIdx = 0; ipAddrIdx < 4; ipAddrIdx++)
                     {
                        shiftBits--;
                        rbInfo[rbId].newSgwAddr.u.ipv4TptAddr.address |=
                           ((U32)(eRABItem->transportLyrAddr.val[ipAddrIdx] << (8 * shiftBits)) & addrMsk);
                        addrMsk = addrMsk >> 8;
                     }
                     break;
                  case 128:
                     rbInfo[rbId].newSgwAddr.type = CM_TPTADDR_IPV6;
                     rbInfo[rbId].newSgwAddr.u.ipv6TptAddr.port = WR_DFLT_EGTP_PORT;
                     for (ipAddrIdx = 0; ipAddrIdx < 16; ipAddrIdx++)
                     {
                        rbInfo[rbId].newSgwAddr.u.ipv6TptAddr.ipv6NetAddr[ipAddrIdx] =
                           eRABItem->transportLyrAddr.val[ipAddrIdx];
                     }
                     break;
                  default:
                     /**
                      * would be in case if len = 160 bits which means support for both 
                      * ipv4 and ipv6
                      */
                     break;
               }
            }

            /* update DAM with new UL tunnel info */
            WR_ALLOC_SHAREBLE_BUF(&tnlModInfo, sizeof(WrDamTnlModInfo));
            if(tnlModInfo == NULLP )
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }

            tnlModInfo->transId = transCb->transId;
            tnlModInfo->tnlId.cellId = transCb->ueCb->cellId;
            tnlModInfo->tnlId.crnti = transCb->ueCb->crnti;
            tnlModInfo->tnlId.drbId = rbInfo[rbId].drbInfo.rbId;
            tnlModInfo->tnlId.tnlType = WR_TNL_NORMAL;
            tnlModInfo->remTeid = rbInfo[rbId].newRemTnlId;
            /*Added for passing dscp value in tunnel modification request*/
            wrUtlGetDscp(transCb->ueCb->cellId, rbInfo[rbId].drbInfo.qci, &(tnlModInfo->dscp));

            wrCpyCmTptAddr(&tnlModInfo->newDstAddr,
                  &(rbInfo[rbId].newSgwAddr));

            wrIfmDamTnlModReq(transCb->transId, tnlModInfo);
         }
      }
   }
   RETVALUE(ROK);
}/* End of wrUmmUpdUlTunPthSwtAck*/


/**
 * @brief 
 *    This function is used to prepare Handover command.
 *
 * @details
 *    Function: wrUmmX2TgtHoPrepHovrCmd
 *
 *    Processing steps:
 *       -# Fill Handover Command Message.
 *       -# Trigger Encode Procedure to encode Handover Command Message.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoPrepHovrCmd
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb  = transCb->ueCb;
   NhuEncReqSdus             *hoCmdEncReq = NULLP;
   NhuEncCfmSdus             *encCfm = transCb->msg->u.encCfm;
   MsgLen                    cnt = 0;
   S16                       retVal = ROK;
   MsgLen                    len = 0;
   NhuHovrCmdcriticalExtnsc1 *c1;

   /**
    * trigger NhuEncodeReq to encode Tgt to Src Transparent Container
    */
   /*129363*/
   WR_ALLOCEVNT(&hoCmdEncReq, sizeof(NhuEncReqSdus));
   if(NULLP == hoCmdEncReq)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /**
    * Fill Handover Cmd Header
    */
   hoCmdEncReq->hdr.cellId = ueCb->cellId;
   hoCmdEncReq->hdr.ueId = ueCb->crnti;
   hoCmdEncReq->hdr.transId = transCb->transId | WR_UMM_TGT_HO_HOCMD_ENC;


   hoCmdEncReq->sdu.msgCategory = NHU_MSG_HOCMD;
   WR_FILL_TKN_UINT(hoCmdEncReq->sdu.u.hovrCmd.pres,PRSNT_NODEF);
   WR_FILL_TKN_UINT(hoCmdEncReq->sdu.u.hovrCmd.criticalExtns.choice,
         CRITICALEXTNS_C1);
   c1 = &hoCmdEncReq->sdu.u.hovrCmd.criticalExtns.val.c1;
   WR_FILL_TKN_UINT(c1->choice,C1_HANDOVERCMD_R8);
   WR_FILL_TKN_UINT(c1->val.handoverCmd_r8.pres,PRSNT_NODEF);
   /**
    * Find the length of the received IP packet
    */
   retVal = SFndLenMsg(encCfm->sdu.mBuf, &len);
   if((len == 0) || (retVal != ROK))
   {
      RLOG1(L_ERROR, "Incorrect buffer len [%d]", len);
      RETVALUE(RFAILED);
   }
   c1->val.handoverCmd_r8.handoverCmdMsg.pres = 1;
   c1->val.handoverCmd_r8.handoverCmdMsg.len = len;
   /**
    * Allocate memory for the flat buffer
    */
   cmGetMem(&(hoCmdEncReq->memCp), len,
   (Ptr *)&(c1->val.handoverCmd_r8.handoverCmdMsg.val)); 
   if(NULLP == c1->val.handoverCmd_r8.handoverCmdMsg.val)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   SCpyMsgFix(encCfm->sdu.mBuf,0,len,
         (Data *)c1->val.handoverCmd_r8.handoverCmdMsg.val,
         &cnt);
   WR_FILL_TKN_UINT(c1->val.handoverCmd_r8.nonCriticalExtn.pres,NOTPRSNT);
   /**
    * Send Encode Request Message.
    */
   if(ROK != wrIfmRrcSndEncodeReq(hoCmdEncReq))
   {
      RLOG0(L_ERROR, "Failed to send Encode Req to RRC");
      /* CR Fix for ccpu00125004 , 04/12/12 */
      WR_FREE(hoCmdEncReq, sizeof(NhuEncReqSdus));
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to prepare RRC Connection Reconfiguration.
 *
 * @details
 *    Function: wrUmmX2TgtHoPrepRrcDlDcchMsg
 *
 *    Processing steps:
 *       -# Fill RRC Connection Reconfiguration with mobility Info
 *       -# Trigger Encode function.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoPrepRrcDlDcchMsg
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &(transCb->u.x2TgtHoTrans);
   WrUeCb                    *ueCb  = transCb->ueCb;
   NhuEncReqSdus             *dcchEncReq = NULLP;

   /**
    * trigger NhuEncodeReq to encode Tgt to Src Transparent Container
    */
   /*129363*/
   WR_ALLOCEVNT(&dcchEncReq, sizeof(NhuEncReqSdus));
   if(NULLP == dcchEncReq)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /**
    * Fill Handover Cmd Header 
    */
   dcchEncReq->hdr.cellId = ueCb->cellId;
   dcchEncReq->hdr.ueId = ueCb->crnti;
   dcchEncReq->hdr.transId = transCb->transId | WR_UMM_TGT_HO_DLDCCH_MSG_ENC;

   /**
    * fill the NhuHovrPrepInfo info inside hoPrepEncReq
    */
   /* SPS changes starts */
   if(ROK != wrUtlFillHoCmdTrnspCntrWthMobility(transCb, dcchEncReq, 
             &x2TgtHoTrans->measTransCb, x2TgtHoTrans->rrmUeRecfgRspMsg->u.rrmUeHoResp, 
             &x2TgtHoTrans->srcToTgtSpsInfo, &transCb->spsCfgTransCb.spsCfg, \
             transCb->spsCfgTransCb.isUeUlSpsEnabled))
   /* SPS changes ends */
   {
      RLOG0(L_ERROR, "Filling of RRC Msg Failed");
      RETVALUE(RFAILED);
   }/* End of veUtlFillHoCmdTrnspCntrWthMobility*/

   if(ROK != wrIfmRrcSndEncodeReq(dcchEncReq))
   {
      RLOG0(L_ERROR, "Failed to send Encode Req to RRC");
      /* CR Fix for ccpu00125004 , 04/12/12 */
      WR_FREE(dcchEncReq, sizeof(NhuEncReqSdus));
      RETVALUE(RFAILED);
   }/* End of VeLiNhuEncodeReq*/

   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoPrepRrcDlDcchMsg */

/**
 * @brief 
 *    This function is used to Fill Path Switch Request.
 *
 * @details
 *    Function: wrUmmX2TgtHoFillPathSwitchReq
 *
 *    Processing steps:
 *       -# Fill Path switch Request Message.
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @param[in]
 *    pdu        : Event Pdu.
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoFillPathSwitchReq
(
WrUmmTransCb                 *transCb,
S1apPdu                      **pdu
)
{
   WrUmmX2TgtHoTrans                           *x2TgtHoTrans;
   WrS1ConCb                                   *s1ConCb;
   WrUmmX2TgtHoRabInfo                         *rbInfo;
   CmTptAddr                                   *lclAddr;
   S1apPdu                                     *s1ap_PDU = NULLP;
   SztProtIE_Field_PathSwRqstIEs               *ie = NULLP;
   SztProtIE_SingleCont_E_RABToBeSwedDLItemIEs *ie1 = NULLP;
   U16                                         numComp = 0;
   U16                                         len;
   U8                                          erabIdx = 0;
   U8                                          compCnt = 0;
   SztTportLyrAddr                             *trprtAddr;
   U8                                          cnt;
   U8                                          offset;
   /* CSG_DEV start */
   WrUmmCellCb                                 *cellCb;
   /* CSG_DEV end */
   WrCgi                                       eCgi;
   WrMmeCb                                     *mmeCb = NULLP;
   U8                                          mbrShpSts;

   x2TgtHoTrans = &transCb->u.x2TgtHoTrans;
   s1ConCb = transCb->ueCb->s1ConCb;

   /* CSG_DEV start */
   WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId);
   if (NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)transCb->ueCb->cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }
   /* CSG_DEV end */

   if (ROK != cmAllocEvnt(sizeof(S1apPdu),WR_SZ_MEM_SDU_SIZE,&wrCb.mem,(Ptr *)&s1ap_PDU))
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of If Condition.*/
   /* MemSet*/
   cmMemset((U8 *)&(s1ap_PDU->pdu), 0, sizeof(SztS1AP_PDU));
   /**
    * Fill the Interface Elementory Procedures
    */
   wrFillTknU8(&(s1ap_PDU->pdu.choice),S1AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(s1ap_PDU->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   wrFillTknU32(&(s1ap_PDU->pdu.val.initiatingMsg.procedureCode), Sztid_PathSwRqst);
   wrFillTknU32(&(s1ap_PDU->pdu.val.initiatingMsg.criticality), SztCriticalityrejectEnum);
   /**
    * Indicates no of Manadatory fields are present.
    */
   numComp = 7;

   if(WR_ENB_OPEN_ACCESS != cellCb->csgAccessMode)
   {
      numComp++; /* CSG ID */
   }
   if(WR_ENB_HYBRID_ACCESS == cellCb->csgAccessMode)
   {
      numComp += 2; /* access mode and membership status */
   }

   wrFillTknU8(&(s1ap_PDU->pdu.val.initiatingMsg.value.u.sztPathSwRqst.pres), PRSNT_NODEF);
   wrFillTknU16(&(s1ap_PDU->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.noComp), numComp);
   if ((cmGetMem(&s1ap_PDU->memCp,(numComp * sizeof(SztProtIE_Field_PathSwRqstIEs)),
               (Ptr *)&s1ap_PDU->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   ie = &s1ap_PDU->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.member[compCnt++];
   /**
    * Fill the eNB UE S1AP ID
    */
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1ConCb->enb_ue_s1ap_id);
   /**
    * E-RABs To be Switched in DL List
    */
   ie = &s1ap_PDU->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_E_RABToBeSwedDLLst);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   /**
    * Fill the no of bearers are present in the List.
    */
   numComp = x2TgtHoTrans->noOfAdmitdBearers;
   wrFillTknU16(&(ie->value.u.sztE_RABToBeSwedDLLst.noComp), numComp);

   if ((cmGetMem(&s1ap_PDU->memCp,(numComp * sizeof(SztProtIE_SingleCont_E_RABToBeSwedDLItemIEs)),
               (Ptr *)&ie->value.u.sztE_RABToBeSwedDLLst.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /**
    * Fill all the members of E_RABToBeSwedList.
    */
   for(erabIdx = 0; erabIdx < x2TgtHoTrans->numOfRbCfgd; erabIdx++)
   {
      ie1 = &ie->value.u.sztE_RABToBeSwedDLLst.member[erabIdx];
      rbInfo = &x2TgtHoTrans->rbInfo[erabIdx];

      wrFillTknU8(&(ie1->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie1->id), Sztid_E_RABToBeSwedDLItem);
      wrFillTknU32(&(ie1->criticality), SztCriticalityrejectEnum);
      /**
       * Fill the E-RAB_ID
       */
      wrFillTknU8(&(ie1->value.u.sztE_RABToBeSwedDLItem.pres), PRSNT_NODEF);
      wrFillTknU32(&(ie1->value.u.sztE_RABToBeSwedDLItem.e_RAB_ID),
            rbInfo->drbInfo.eRABId);
      /* PathSwitchRequest is Sent towards MME. Based on IP Type of MME, filling the IP of enodeB*/
      mmeCb = wrEmmCb.mmeCont.mmes[transCb->ueCb->mmeId];
      wrEmmGetDatAppAddr(&lclAddr, mmeCb->mmeAddr[0].type);      
      if (lclAddr->type == CM_TPTADDR_IPV4)
      {
         trprtAddr = &ie1->value.u.sztE_RABToBeSwedDLItem.transportLyrAddr;
         len = 4;
         trprtAddr->pres = PRSNT_NODEF;
         trprtAddr->len = (U16)(len * 8);
         WR_GET_MEM(&s1ap_PDU->memCp, len,
               &trprtAddr->val);
         for (cnt = 0; cnt < len; cnt++)
         {
            offset =(U8)((len - (cnt + 1)) * 8);
            trprtAddr->val[cnt] = 
               (U8)(lclAddr->u.ipv4TptAddr.address >> offset);
         }
       }
       else if (lclAddr->type == CM_TPTADDR_IPV6)
       {
         trprtAddr = &ie1->value.u.sztE_RABToBeSwedDLItem.transportLyrAddr;
         len = 16;
         trprtAddr->pres = PRSNT_NODEF;
         trprtAddr->len = (U16)(len * 8);
         WR_GET_MEM(&s1ap_PDU->memCp, len,
               &trprtAddr->val);
         for (cnt = 0; cnt < len; cnt++)
         {
            trprtAddr->val[cnt] = (U8)(lclAddr->u.ipv6TptAddr.ipv6NetAddr[cnt]);
         }

      }
      else
      {
           RLOG1(L_ERROR,"Invalid IP Type : lclAddr->type=%d",lclAddr->type);
      }
      len = 4;
      ie1->value.u.sztE_RABToBeSwedDLItem.gTP_TEID.len = len;
      ie1->value.u.sztE_RABToBeSwedDLItem.gTP_TEID.pres = PRSNT_NODEF;
      WR_GET_MEM(&s1ap_PDU->memCp, len,&(ie1->value.u.sztE_RABToBeSwedDLItem.gTP_TEID.val));
      for (cnt = 0; cnt < len; cnt++)
      {
         /**
          * compute the offset to right shift the val in U32 shift by 24,16 & 8 bytes
          */
         offset = (U8)(len - (cnt + 1)) * 8;
         ie1->value.u.sztE_RABToBeSwedDLItem.gTP_TEID.val[cnt] =
            (U8)(rbInfo->drbInfo.locEgtpTunId >> offset);
      }
      ie1->value.u.sztE_RABToBeSwedDLItem.iE_Extns.noComp.pres = NOTPRSNT;
   }
   ie = &s1ap_PDU->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
      member[compCnt++];
   /**
    * Fill the MME-UE-S1AP-ID
    */
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_SrcMME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), x2TgtHoTrans->mme_ue_s1ap_id);

   ie = &s1ap_PDU->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
      member[compCnt++];
   /**
    * Fill the EUTRAN CGI
    */
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_EUTRAN_CGI);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU8(&(ie->value.u.sztEUTRAN_CGI.pres), PRSNT_NODEF);
   wrEmmMmeSztFillPLMNId(s1ap_PDU, &(x2TgtHoTrans->plmnId),
         &(ie->value.u.sztEUTRAN_CGI.pLMNidentity));

   /* ccpu00136516 */
   wrEmmGetCgi(transCb->ueCb->cellId, &eCgi);
   wrFillTknBStr32(&(ie->value.u.sztEUTRAN_CGI.cell_ID), 28,
            eCgi.cellId);
   
   ie->value.u.sztEUTRAN_CGI.iE_Extns.noComp.pres = NOTPRSNT;
   /**
    * Fill the TAI which contains PLMN and TAC
    */
   ie = &s1ap_PDU->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
      member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_TAI);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU8(&(ie->value.u.sztTAI.pres), PRSNT_NODEF);
   wrEmmMmeSztFillPLMNId(s1ap_PDU, &x2TgtHoTrans->plmnId, 
         &(ie->value.u.sztEUTRAN_CGI.pLMNidentity));
   wrEmmMmeSztFillTAC(x2TgtHoTrans->tac, NULLP, &(ie->value.u.sztTAI.tAC));
   /**
    * Fill UE Security capabilities.
    */
   ie = &s1ap_PDU->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
      member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_UESecurCapabilities);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU8(&(ie->value.u.sztUESecurCapabilities.pres), PRSNT_NODEF);

   /* ccpu00131644 */
   WR_MEM_COPY(&ie->value.u.sztUESecurCapabilities.encryptionAlgorithms, 
         &x2TgtHoTrans->secCfg->encrypCap, sizeof(TknBStr32));
   WR_MEM_COPY(&ie->value.u.sztUESecurCapabilities.integrityProtectionAlgorithms,
         &x2TgtHoTrans->secCfg->integCap, sizeof(TknBStr32));

   ie->value.u.sztUESecurCapabilities.iE_Extns.noComp.pres = NOTPRSNT;

   if(WR_ENB_OPEN_ACCESS != cellCb->csgAccessMode)
   {
      ie = &s1ap_PDU->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
         member[compCnt++];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_CSG_Id);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      wrFillTknBStr32(&(ie->value.u.sztCSG_Id), WR_S1AP_CSG_ID_LEN,
            cellCb->csgId);
   }

   if(WR_ENB_HYBRID_ACCESS == cellCb->csgAccessMode)
   {
      ie = &s1ap_PDU->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
         member[compCnt++];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_CellAccessMode);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      wrFillTknU32(&(ie->value.u.sztCellAccessMode),
            SztCellAccessModehybridEnum);
   }

   /**
    * Fill GUMMEI information
    */
   ie = &s1ap_PDU->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
      member[compCnt++];
   SztGUMMEI *gummei;
   gummei = &ie->value.u.sztGUMMEI;
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_SrcMME_GUMMEI);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU8(&(gummei->pres),PRSNT_NODEF);

   wrUmmS1SrcHoFillPLMNId(s1ap_PDU, &x2TgtHoTrans->gummei.plmnId,
         &gummei->pLMN_Identity);

   wrFill2TknStr4(&(gummei->mME_Group_ID), 2,
         (U8*)&x2TgtHoTrans->gummei.mmeGrpId);

   wrFill2TknStr4(&(gummei->mME_Code), 1, (U8*)&x2TgtHoTrans->gummei.mmec);
   gummei->iE_Extns.noComp.pres = NOTPRSNT;

   if(WR_ENB_HYBRID_ACCESS == cellCb->csgAccessMode)
   {
      mbrShpSts = wrUmmCsgIsUeMember(transCb->ueCb, cellCb->csgId);
      ie = &s1ap_PDU->pdu.val.initiatingMsg.value.u.sztPathSwRqst.protocolIEs.
         member[compCnt];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_CSGMembershipStatus);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      if(WR_UMM_CSG_MEMBER == mbrShpSts)
      {
         wrFillTknU32(&(ie->value.u.sztCSGMembershipStatus),
               SztCSGMembershipStatusmemberEnum);
      }
      else
      {
         wrFillTknU32(&(ie->value.u.sztCSGMembershipStatus),
               SztCSGMembershipStatusnot_memberEnum);
      }
   }

   *pdu = s1ap_PDU;
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoFillPathSwitchReq.*/

/**
 * @brief 
 *    This function is used to prepare Path Switch Request.
 *
 * @details
 *    Function: wrUmmPrepPathSwitchReq
 *
 *    Processing steps:
 *       -# Fill Path switch Request Message.
 *       -# Send Path Switch Request Message to the MME.
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmPrepPathSwitchReq
(
WrUmmTransCb                 *transCb
)
{
   S1apPdu                   *pathSwitchReq = NULLP;
   WrS1ConCb                 *s1ConCb = transCb->ueCb->s1ConCb;
   SztConReq                 datEvt;
   WrUmmX2TgtHoTrans         *x2TgtHoTrans;
   WrMmeCb                   *mmeCb = NULLP;

   x2TgtHoTrans = &transCb->u.x2TgtHoTrans;

   mmeCb = wrEmmCb.mmeCont.mmes[x2TgtHoTrans->mmeId];
   if(mmeCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_MMEID, x2TgtHoTrans->mmeId,"MME CB Not Found");
      RETVALUE(RFAILED);
   }

   if (ROK != wrUmmX2TgtHoFillPathSwitchReq(transCb, &pathSwitchReq))
   {
      RLOG0(L_ERROR, "Building of Path Switch Request Failed");
      RETVALUE(RFAILED);
   }/* End of wrUmmSndPathSwitchReq.*/

   WR_SET_ZERO(&datEvt, sizeof(SztConReq));

   /* Trigger SZT Connectn. Req.TODO - get peerId here */
   datEvt.peerId.pres = PRSNT_NODEF;
   datEvt.peerId.val  = mmeCb->peerId;
   datEvt.u.suConnId  = s1ConCb->suConnId;
   datEvt.pdu = pathSwitchReq;

   RLOG_ARG1(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[S1AP]:Sending PATH SWITCH REQUEST"
         "[SRC-MME-UE-S1AP-ID:%lu]",transCb->u.x2TgtHoTrans.mme_ue_s1ap_id);
   if (WrIfmS1apConReq(&datEvt) != ROK)
   {
      RLOG0(L_ERROR, "Failure in sending the SZT CON REQ");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/* End of wrUmmPrepPathSwitchReq.*/

/**
 * @brief 
 *    This function is used to send Path Switch Request and handle the UE
 *    arrival.
 *
 * @details
 *    Function: wrUmmX2TgtHoSndPathSwitchReq
 *
 *    Processing steps:
 *       - Send Path Switch Request to MME
 *       - Start Path Switch Request timer
 *       - Start END Marker timer
 *
 * @param[in] 
 *    transCb    : transaction information
 *
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmX2TgtHoSndPathSwitchReq
(
WrUmmTransCb                 *transCb
)
{

#ifdef WR_DETECT_RLF
   if(transCb->ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_CQI)
   {
      wrStartUeTmr((WrUeCb *)transCb->ueCb, WR_TMR_DETECT_RLF, WR_TMR_DETECT_RLF_VAL);
   }
#endif
   if (ROK != wrUmmPrepPathSwitchReq(transCb))
   {
      RLOG0(L_ERROR, "Failed to Prepare Path Switch Request Message.");
      RETVALUE(RFAILED);
   }/* End of the wrUmmPrepPathSwitchReq*/ 

   /* ccpu00145982 */ 
   /* Stop the Relocation Timer */

   wrStopUeTmr(transCb->ueCb, WR_TMR_TGTENB_RELOCEXE);

   /* start Path Switch Timer */
   wrStartTransTmr(transCb, WR_TMR_TGT_PATH_SWITCH, WR_TMR_TGT_PATH_SWITCH_VAL);
   RLOG0(L_DEBUG, "Inactivity req has been sent to dam");
   wrIfmDamStartInactivityTimer(transCb->ueCb->cellId,transCb->ueCb->crnti);/*suhas*/
   /*ccpu00136428 
    * Changing the tunnel state and sending the buffered packets to PDCP
    * is done only after receiving the End Marker Packet 
    * or on expiry of the End Marker Timer */
   /* ccpu00129042
    * SndUeCtxtRel after receiving Path_Switch_Ack from MME
    * */

   RETVALUE(ROK);
}
/* ccpu00131426 - Fix */
/** @brief This function is responsible for reconfiguring the UE context
 *         at the scheduler.
 *
 * @details
 *
 *     Function: wrUmmX2HoDrxReCfgSchdUeRecfgReq
 *
 *         Processing steps:
 *         - allocate a new configuration request
 *         - set the configuration type to RGR_UE_CFG
 *         - fill the ue info and DRX Release info
 *         - send the request to scheduler
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2HoDrxReCfgSchdUeRecfgReq 
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2TgtHoTrans *x2TgtHoTrans = &transCb->u.x2TgtHoTrans;
   RgrCfgReqInfo     *cfgReq;
   RgrUeRecfg        *ueRecfg;
   RmuUeRecfgRsp     *rrmUeRecfgResp = x2TgtHoTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
   U32                transId;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   transId = transCb->transId | WR_UMM_TGT_HO_DRX_CFG;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   /* Mark the transaction that UE config has been sent */
   x2TgtHoTrans->state = WR_UMM_TGT_HO_DRX_CFG_SENT;
   wrUmmSchdFillUeDrxCfgInfo(ueRecfg, (WrDrxCfg *)&rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.stRmDrxCfg);
   
   /* Dispatch the prepared message to scheduler. */ 
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
} /* wrUmmX2HoDrxReCfgSchdUeRecfgReq */ 



/**
 * @brief 
 *    This function is used to process RRC Connection Reconfiguration Complete.
 *
 * @details
 *    Function: wrUmmX2TgtHoHdlHoCompletion
 *
 *    Processing steps:
 *       -# Set the Handover State
 *       -# Send Path Switch Request Message to the MME.
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoHdlHoCompletion
(
WrUmmTransCb                 *transCb
)
{
    WrUmmX2TgtHoTrans        *x2TgtHoTrans = &transCb->u.x2TgtHoTrans;

   /**
    * Handling of RRC Connection Reconfiguration Complete.
    * Upon Reception of RRC Connection Reconfiguration Complete Message 
    * from the UE(Handover Confirmation.), the target eNB shall prepare 
    * Path Switch Request Message and send it to the MME.
    */
    /**
    * Change the Handover State.
    */
   x2TgtHoTrans->state = WR_HANDOVER_COMPLETION;
   transCb->ueCb->rrcConnState = WR_UMM_RRC_CONNECTED;
/*ccpu00129789*/
#ifdef WR_DETECT_RLF
      if(transCb->ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_CQI)
      {
         wrStartUeTmr(transCb->ueCb, WR_TMR_DETECT_RLF, WR_TMR_DETECT_RLF_VAL);
      }
#endif
   /* ccpu00131426 - Fix : Send Scheduler DRX configuration */
   RmuUeRecfgRsp *rrmUeRecfgResp = x2TgtHoTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
   if((rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & 
          RM_RCM_MAC_MAIN_CFG) &&
      (rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap & 
          RM_RCM_MAC_MAIN_CFG_DRX))
   {
     wrUmmX2HoDrxReCfgSchdUeRecfgReq(transCb);
   }
   SGetEpcTime(&transCb->ueCb->ueAttachTime);/*UE History info*/
   RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Sending Path Switch "
      "Request to MME");

   if(x2TgtHoTrans->snStatusInfoFlag == WR_UMM_TGT_HO_SN_INFO_NOWAIT || 
         x2TgtHoTrans->snStatusInfoFlag == WR_UMM_TGT_HO_SN_INFO_GOT)
   {
      /* Send PDCP SN Status to PDCP through RRC */
      if(x2TgtHoTrans->snStatusInfoFlag == WR_UMM_TGT_HO_SN_INFO_GOT)
      {
          if (ROK != wrPdcpSnStsUpd(transCb))
          {
              RETVALUE(RFAILED);
          }
         
      }

      wrUmmX2TgtHoSndPathSwitchReq(transCb);
   }

   /* RRC-REL10-Upgrade */
   transCb->ueCb->isFullCfgReqd = FALSE;

   x2TgtHoTrans->isUeAttached = TRUE;

   /*Increment KPI for Intra-LTE Handovers Successful incoming Handover from eNodeB per Handover cause*/
   if ( FALSE == transCb->u.x2TgtHoTrans.isRrcReEstRcvd )
   {
      WR_INC_KPI_FUNC5(wrIncKpiFrHovrSuccd, WR_HO_TYPE_X2, LTE_X2_HOV,INC_HOV,
            &(transCb->u.x2TgtHoTrans.cztCause), NULLP);
   }
   else /* TRUE == transCb->u.s1TgtHoTrans.isRrcReEstRcvd */
   {
      /* ccpu00141451 - Increment the HO success counter
       * After RRC-Reestablishemnt */
      WR_INC_KPI_FUNC(wrIncKpiIncHoSuccAftrReEst);
      transCb->u.x2TgtHoTrans.isRrcReEstRcvd = FALSE;
   }

   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoHandleRrcConReConfgComplete*/

#define WR_CALC_COUNT(_snLen, _sn, _txHfn, _count)         \
{                                                          \
   _count = _sn;                                           \
   _txHfn = _txHfn << _snLen;                               \
   _count |= _txHfn;                                        \
}

/**
 * @brief 
 *    This function is used to Fill Erab Subject To Status Transfer List.
 *
 * @details
 *    Function: wrUmmX2TgtHoFillErabsSubjToStatusTfrLst
 *
 *    Processing steps:
 *       -# Fill E-RAB status transfer list.
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @param[in]
 *    eRABsSubjToStatusTfr : E-RAB Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoFillErabsSubjToStatusTfrLst
(
WrUmmTransCb                   *transCb,
CztE_RABs_SubjToStatusTfr_Lst  *eRABsSubjToStatusTfr
)
{
   WrUmmX2TgtHoTrans                                *x2TgtHoTrans;
   WrUeCb                                           *ueCb;
   U8                                               loopCnt;
   U8                                               idx;
   U16                                              seqNumber;
   U32                                              hfn;   
   WrUmmSnSts                                       *snSts;
   NhuHoPdcpSduInfo                                 *sduInfo;
   CztE_RABs_SubjToStatusTfr_Item                   *itemInfo;
   Czt_ValueE_RABs_SubjToStatusTfr_ItemIEsCls       *itemIe;
   CztProtExtnField_E_RABs_SubjToStatusTfr_ItemExtIEs *extIEs;

   x2TgtHoTrans = &transCb->u.x2TgtHoTrans;
   ueCb = transCb->ueCb;

   /**
    * Allocate the Memory.
    */
   WR_ALLOC(&x2TgtHoTrans->snStsInfo, sizeof(WrUmmSnStsInfo));
   if (NULLP == x2TgtHoTrans->snStsInfo)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of Allocate Memory*/
   cmMemset((U8 *)x2TgtHoTrans->snStsInfo, 0, (sizeof(WrUmmSnStsInfo)));
   /**
    * Filling No of Radio Bearers.
    */
   x2TgtHoTrans->snStsInfo->numOfRb = eRABsSubjToStatusTfr->noComp.val;
   for (loopCnt = 0; loopCnt < eRABsSubjToStatusTfr->noComp.val;loopCnt++)
   {
      snSts = &x2TgtHoTrans->snStsInfo->wrSnSts[loopCnt];
      itemIe = &eRABsSubjToStatusTfr->member[loopCnt].value;
      itemInfo = &itemIe->u.cztE_RABs_SubjToStatusTfr_Item;

      if(ROK != wrUmmGetDrbIdFromErabId(ueCb, itemInfo->e_RAB_ID.val,
                                              &snSts->rbId))
      {
         RETVALUE(RFAILED);
      }

      sduInfo = &snSts->nhuHoPdcpSduInfo;
      sduInfo->isHoPres = 0x00;
      if (TRUE == itemInfo->dL_COUNTvalue.pres.pres)
      {
         /**
          * updating the bitmap to indicate the presence of DL COUNT
          */
         sduInfo->isHoPres |= NHU_HO_DL;
         /* RRC-REL10-Upgrade start */
         if(transCb->ueCb->isFullCfgReqd)
         {
            /* we will send full configuration to Rel 10 UE in the HO command .Hence reseting to zero*/
            seqNumber = 0;
            hfn = 0;
         }
         else
         { 
         seqNumber = itemInfo->dL_COUNTvalue.pDCP_SN.val;
         hfn = itemInfo->dL_COUNTvalue.hFN.val;
         }
         sduInfo->snLen = NHU_PDCP_SN_SIZE_12BITS;
         WR_CALC_COUNT(NHU_PDCP_SN_SIZE_12BITS, seqNumber, hfn,
                                               sduInfo->dlCountVal);
         RLOG3(L_DEBUG, "DL pdcp sn[%d] pdcp hfn[%ld] count[%ld]", seqNumber,
            itemInfo->dL_COUNTvalue.hFN.val, sduInfo->dlCountVal);
      }/* End of if condition.*/
      if (TRUE == itemInfo->uL_COUNTvalue.pres.pres)
      {
         /**
          * updating the bitmap to indicate the presence of UL COUNT
          */
         sduInfo->isHoPres |= NHU_HO_UL;
         /* RRC-REL10-Upgrade start */
         if(transCb->ueCb->isFullCfgReqd)
         {
            /* we will send full configuration to Rel 10 UE in the HO command .Hence reseting to zero*/
            seqNumber = 0;
            hfn = 0;
         }
         else
         { 
         seqNumber = itemInfo->uL_COUNTvalue.pDCP_SN.val;
         hfn = itemInfo->uL_COUNTvalue.hFN.val;
         }
         sduInfo->snLen = NHU_PDCP_SN_SIZE_12BITS;
         WR_CALC_COUNT(NHU_PDCP_SN_SIZE_12BITS, seqNumber, hfn,
                                               sduInfo->ulCountVal);
         
         RLOG3(L_DEBUG, "UL pdcp sn[%d] pdcp hfn[%ld] count[%ld]", seqNumber,
            itemInfo->uL_COUNTvalue.hFN.val, sduInfo->ulCountVal);
      }/* End of if condition.*/        
      /* Check for the availability of Receive Status of UL PDCP SDUs */
      if(TRUE == itemInfo->receiveStatusofULPDCPSDUs.pres)
      {
         /* Retreive the UL Bitmap and numOfBits 
          * Send it to PDCP */
         wrRetrvBitMapfrm4096BSXL(&(itemInfo->receiveStatusofULPDCPSDUs),
               &snSts->nhuHoPdcpSduInfo.numBits, &snSts->nhuHoPdcpSduInfo.ulBitMap);
      }
      if (itemInfo->iE_Extns.noComp.pres == TRUE)
      {
         for (idx = 0; idx < itemInfo->iE_Extns.noComp.val; idx++)
         {
            extIEs = &itemInfo->iE_Extns.member[idx];
            if (extIEs->id.val == Cztid_ULCOUNTValueExtended)
            {
               if (extIEs->extensionValue.u.cztid_ULCOUNTValueExtended.pres.pres)
               {
                  /**
                   * updating the bitmap to indicate the presence of UL COUNT
                   */
                  sduInfo->isHoPres |= NHU_HO_UL;
                  /* RRC-REL10-Upgrade start */
                  if(transCb->ueCb->isFullCfgReqd)
                  {
                     /* we will send full configuration to Rel 10 UE in the HO command .Hence reseting to zero*/
                     seqNumber = 0;
                     hfn = 0;
                  }
                  else
                  { 
                     seqNumber = extIEs->extensionValue.u.cztid_ULCOUNTValueExtended.pDCP_SNExtended.val;
                     hfn = extIEs->extensionValue.u.cztid_ULCOUNTValueExtended.hFNMdfd.val;
                  }

                  sduInfo->snLen = NHU_PDCP_SN_SIZE_15BITS;
                  WR_CALC_COUNT(NHU_PDCP_SN_SIZE_15BITS, seqNumber, hfn,
                        sduInfo->ulCountVal);

                  RLOG3(L_DEBUG, "UL pdcp Ext sn[%d] pdcp hfn[%ld] count[%ld]", seqNumber,
                        extIEs->extensionValue.u.cztid_ULCOUNTValueExtended.hFNMdfd.val,
                        sduInfo->ulCountVal);
               }
            }
            if (extIEs->id.val == Cztid_DLCOUNTValueExtended)
            {
               if (extIEs->extensionValue.u.cztid_DLCOUNTValueExtended.pres.pres)
               {
                  /**
                   * updating the bitmap to indicate the presence of DL COUNT
                   */
                  sduInfo->isHoPres |= NHU_HO_DL;
                  /* RRC-REL10-Upgrade start */
                  if(transCb->ueCb->isFullCfgReqd)
                  {
                     /* we will send full configuration to Rel 10 UE in the HO command .Hence reseting to zero*/
                     seqNumber = 0;
                     hfn = 0;
                  }
                  else
                  { 
                     seqNumber = extIEs->extensionValue.u.cztid_DLCOUNTValueExtended.pDCP_SNExtended.val;
                     hfn = extIEs->extensionValue.u.cztid_DLCOUNTValueExtended.hFNMdfd.val;
                  }
                  sduInfo->snLen = NHU_PDCP_SN_SIZE_15BITS;
                  WR_CALC_COUNT(NHU_PDCP_SN_SIZE_15BITS, seqNumber, hfn,
                        sduInfo->dlCountVal);
                  RLOG3(L_DEBUG, "DL pdcp Ext sn[%d] pdcp hfn[%ld] count[%ld]", seqNumber,
                        extIEs->extensionValue.u.cztid_DLCOUNTValueExtended.hFNMdfd.val,
                        sduInfo->dlCountVal);
               }
            }
            if (extIEs->id.val == Cztid_ReceiveStatusOfULPDCPSDUsExtended)
            {
               /* Retreive the UL Bitmap and numOfBits 
                * Send it to PDCP */
               wrRetrvBitMapfrm16384BSXL(&(extIEs->extensionValue.u.cztReceiveStatusOfULPDCPSDUsExtended),
                     &snSts->nhuHoPdcpSduInfo.numBits, &snSts->nhuHoPdcpSduInfo.ulBitMap);
            }
         }
      }
   }/* End of for loop.*/
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoFillErabsSubjToStatusTfrLst*/

/**
 * @brief 
 *    This function is used to Parse Sn Status Transfer Message received from peer eNB.
 *
 * @details
 *    Function: wrUmmX2TgtHoParseSnStatusTfr
 *
 *    Processing steps:
 *       -# Parse Sn Status Transfer Message.
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoParseSnStatusTfr
(
WrUmmTransCb                 *transCb
)
{
   CztX2AP_PDU                    *x2apPdu = NULLP;
   TknU8                          *ie;
   CztE_RABs_SubjToStatusTfr_Lst  *snLst;

   /**
    * Getting the incoming Sn Status Transfer Message PDU
    */
   x2apPdu = &transCb->msg->u.x2Pdu->pdu;

   wrUtlX2GetCztIE(x2apPdu, Cztid_E_RABs_SubjToStatusTfr_Lst, (void **)&ie);
   if(ie == NULLP)
   {
      RLOG0(L_ERROR, "Old UE X2ID not found");
      RETVALUE(RFAILED);
   }
   snLst = (CztE_RABs_SubjToStatusTfr_Lst*)ie;
   /**
    * Fill E-Rab Subject to transfer list
    */
   if (ROK != wrUmmX2TgtHoFillErabsSubjToStatusTfrLst(transCb, snLst))
   {
      RLOG0(L_ERROR, "Parsing of E_RABs SN Status Info Failed");
      RETVALUE(RFAILED);
   }/* End of wrUmmX2TgtHoFillErabsSubjToStatusTfrLst*/
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoParseSnStatusTfr.*/

/**
 * @brief 
 *    This function is used to handle Sn Status Transfer Message received from peer eNB.
 *
 * @details
 *    Function: wrUmmX2TgtHoPrcSnStatusTfr
 *
 *    Processing steps:
 *       -# Parse Sn Status Transfer Message.
 *       -# Configure PDCP to update SN Status Information.
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrPdcpSnStsUpd
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   NhuCfgReqSdus             *nhuCfgReqSdus = NULLP;
   NhuUeCfgInfo              *nhuUeCfgInfo = NULLP;
   NhuRbCfgList              *rbCfgList = NULLP;
   NhuRbCfgInfo              *nhuRbCfgInfo = NULLP;
   NhuPdcpCfg                *pdcpCfg = NULLP;
   U8                        numOfRb = 0;
   U8                        loopCnt = 0;

   /*
    * Allocate memory for event structure.
    */
   WR_ALLOCEVNT(&nhuCfgReqSdus, sizeof(NhuCfgReqSdus));
   if(NULLP == nhuCfgReqSdus)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /**
    * Fill Event Handler Information.
    */
   wrUmmFillRrcEvntHdr(&(nhuCfgReqSdus->hdr), ueCb->cellId, ueCb->crnti,
         transCb->transId | WR_UMM_TGT_HO_SN_CFG);
   /*
    * Fill SN status information in the ueCfg whic will be conveyed to PDCP by RRC .
    */
   nhuUeCfgInfo = &nhuCfgReqSdus->cfgInfo;
   nhuUeCfgInfo->ueCfgBitMsk = NHU_EXP_CFG_RB_CFG;
   nhuUeCfgInfo->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_MODIFY;
   rbCfgList = &(nhuUeCfgInfo->rbCfgAllList.rbToModCfgList);
   numOfRb = transCb->u.x2TgtHoTrans.snStsInfo->numOfRb;
   rbCfgList->numOfRbCfgInfo = numOfRb;
   WR_GET_MEM(nhuCfgReqSdus, (sizeof(NhuRbCfgInfo) * (numOfRb)),\
         &(rbCfgList->rbCfgInfoList));
   for (loopCnt = 0; loopCnt < numOfRb; loopCnt++)
   {
      nhuRbCfgInfo = &rbCfgList->rbCfgInfoList[loopCnt];
      /**
       * Initialize and update the bitmask
       */
      nhuRbCfgInfo->rbCfgBitMsk = 0x00 | NHU_RB_CFG_PDCP_INFO;

      nhuRbCfgInfo->rbId.rbId = transCb->u.x2TgtHoTrans.snStsInfo->wrSnSts[loopCnt].rbId;
      /*
       * As the data forwarding only happens for DRB and the SN transfer also comes
       * only for the DRBs assign the bearer type to CM_LTE_DRB .
       */
      nhuRbCfgInfo->rbId.rbTyp = CM_LTE_DRB;

      /**
       * Logical channel Mapping
       */
      nhuRbCfgInfo->lcId.lcId = transCb->u.x2TgtHoTrans.snStsInfo->wrSnSts[loopCnt].rbId;
      /*
       * As the data forwarding only happens for DRB and the SN transfer also comes
       * only for the DRBs assign the Logical Channel Type to CM_LTE_LCH_DTCH.
       */
      nhuRbCfgInfo->lcId.logChType = CM_LTE_LCH_DTCH;

      pdcpCfg = &nhuRbCfgInfo->pdcpCfg;
      /**
       * Initialize and update the bitmask
       */
      pdcpCfg->pdcpCfgBitMsk = 0x00 | NHU_PDCP_SDU_STA_INFO;
      pdcpCfg->hoPdcpSduStaInfo = transCb->u.x2TgtHoTrans.snStsInfo->wrSnSts[loopCnt].nhuHoPdcpSduInfo; 
      /**
       * initializing the members which are not used
       */
      pdcpCfg->intgReqd = FALSE;
      pdcpCfg->cfmReqd = FALSE;
   }/* End of for () */

   /**
    * Send the UE configuration to RRC
    */    
   if (ROK != wrIfmRrcSndCfgReq(nhuCfgReqSdus))
   {
      RLOG0(L_ERROR, "Failed to send RRC Config Req.");
      /* CR Fix for ccpu00125004 , 04/12/12 */
      WR_FREEEVNT(nhuCfgReqSdus);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to handle Sn Status Transfer Message received from peer eNB.
 *
 * @details
 *    Function: wrUmmX2TgtHoPrcSnStatusTfr
 *
 *    Processing steps:
 *       -# Parse Sn Status Transfer Message.
 *       -# Configure PDCP to update SN Status Information.
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoPrcSnStatusTfr
(
 WrUmmTransCb                *transCb
)
{
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &(transCb->u.x2TgtHoTrans);

    /**
     * Parse the X2 Sn Status Transfer Message.
     */
   if (ROK != wrUmmX2TgtHoParseSnStatusTfr(transCb))
   {
      RLOG0(L_ERROR, "Failed to Parse Sn Status Transfer Message.");
      RETVALUE(RFAILED);
   }/* End of wrUmmX2TgtHoParseSnStatusTfr*/

   RLOG_ARG2(L_EVENT,DBG_CRNTI, transCb->ueCb->crnti,"[X2AP]:Received SN STATUS TRANSFER "
   "[Old-eNB-UE-X2AP-ID:%d][New-eNB-UE-X2AP-ID:%d]",
         transCb->u.x2TgtHoTrans.old_eNB_UE_X2ap_Id, 
         transCb->u.x2TgtHoTrans.new_eNB_UE_X2ap_Id);

   x2TgtHoTrans->snStatusInfoFlag = WR_UMM_TGT_HO_SN_INFO_GOT;
   if(x2TgtHoTrans->isUeAttached == TRUE)
   {
      /* Send PDCP SN Status to PDCP through RRC */
      if (ROK != wrPdcpSnStsUpd(transCb))
      {
         RLOG0(L_ERROR, "Failed to send wrPdcpSnStsUpd.");
         RETVALUE(RFAILED);
      }

      wrUmmX2TgtHoSndPathSwitchReq(transCb);
   }
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoPrcSnStatusTfr*/

/**
 * @brief 
 *    This function is used to handle Handover Request Message received from peer eNB.
 *
 * @details
 *    Function: wrUmmX2TgtHoPrcHoReq
 *
 *    Processing steps:
 *       -# Parse Handover Request message and stores all IEs into transaction
 *          control block for future use.
 *          -# If fails to parse, It shall prepare Handover Preparation failure
 *             message and send to peer eNB.
 *       -# Apply Call Admission control Algorithm.
 *          -# If fails, It shall prepare Handover Preparation failure message
 *             and send to peer eNB.
 *       -# If eNB admits the new UE, trigger resource allocation procedure.
 *       -# If Resource Allocation is Failed, It shall prepare Handover
 *          Preparation failure message and send to peer eNB.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 */
EXTERN U32 hovrAttempted;
PRIVATE S16 wrUmmX2TgtHoPrcHoReq
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &(transCb->u.x2TgtHoTrans);
   WrTai                     tai;
   U8                        causeChoice;
   U8                        causeValue;
   /* CSG_DEV start */
   WrUmmCellCb               *cellCb;
   /* CSG_DEV end */

   x2TgtHoTrans->peerId = transCb->msg->u.x2Pdu->peerId;
   /**
    * Upon reception of Handover Request Message from the source eNB,
    * Target eNB shall parse all IEs and stores into transaction control 
    * block to use in future and set the state as HANDOVER_PREPARATION.
    */
   /**
    * Fill UE Id.
    */ 
   x2TgtHoTrans->crnti = transCb->ueCb->crnti;
   x2TgtHoTrans->state = WR_HANDOVER_PREPARATION;
   wrEmmGetTai(transCb->ueCb, &tai);
   /* get plmn id */
   cmMemcpy((U8 *)&x2TgtHoTrans->plmnId,(U8 *)&tai.plmnId, sizeof(WrPlmnId));
   /* get tac id */
   x2TgtHoTrans->tac = tai.tac;

   x2TgtHoTrans->isUeAttached = FALSE;
   x2TgtHoTrans->snStatusInfoFlag = WR_UMM_TGT_HO_SN_INFO_NOWAIT;


   /**
    * Parse the X2 HO Request Message.
    */
   if (ROK != wrUmmX2TgtHoParseHoReq(transCb))
   {
      /**
       * If eNB is failed to parse Handover Request Message, eNB shall send
       * Handover Preparation failure Message to the Source eNB.
       */
      RLOG0(L_ERROR, "Handover Request Msg Parsing failed.");
      if(x2TgtHoTrans->errCause == CztCauseRadioNwunknown_MME_CodeEnum)
      {
         x2TgtHoTrans->hoPrepFailCause.causeVal = 
                        CztCauseRadioNwunknown_MME_CodeEnum;
      }
      if(ROK != wrUmmX2TgtHoSndPrepFailure(transCb))
      {
              RLOG0(L_ERROR,"Failed To Send Handover Failure");
      }
      transCb->state = WR_UMM_TRANS_RELEASE_CALL;
      RETVALUE(RFAILED);
   }/* End of wrUmmX2TgtHoParseHoReq*/

   WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId);
   if (NULLP == cellCb)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)transCb->ueCb->cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   /* CSG_DEV start */
   if(RFAILED == wrUmmCsgValidateInbndX2Ho(transCb->ueCb,
            transCb->msg->u.x2Pdu->peerId,
            transCb->u.x2TgtHoTrans.ueMbrShpSts,
            transCb->u.x2TgtHoTrans.isEmrgncyCall,
            &causeChoice, &causeValue))
   {
      x2TgtHoTrans->errCause = causeChoice;
      x2TgtHoTrans->hoPrepFailCause.causeTyp = causeChoice; 
      x2TgtHoTrans->hoPrepFailCause.causeVal = causeValue; 
      wrUmmX2TgtHoSndPrepFailure(transCb);
      transCb->state = WR_UMM_TRANS_DELETE_UE;
      RETVALUE(RFAILED);
   }
   /* CSG_DEV end */
   
   /*Increment KPI for Intra-LTE Handovers Attempted incomming Handover from eNodeB per Handover cause*/
   transCb->ueCb->hovDir = INC_HOV;
#ifdef WR_RSYS_KPI 
   transCb->ueCb->hov = LTE_X2_HOV;
   /* We have common function for incoming and out going HO, KPI count will be
    * diffentiated by direction */
   WR_INC_KPI_FUNC5(wrIncKpiFrHovrAttmptd, WR_HO_TYPE_X2, LTE_X2_HOV,INC_HOV,
         &(transCb->u.x2TgtHoTrans.cztCause), NULLP);
   /* CSG_DEV */
   /* Updating KPI Counter for attempted HO to CSG or Hybrid Cell */
   if ((cellCb->csgAccessMode == WR_ENB_CLOSED_ACCESS)||
                 (cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS))
   {
      RLOG0(L_INFO,"Updating the KPI Counter for Attempted Inbound Mobility");
      WR_INC_KPI_FUNC(wrIncKpiFrInboundCsgMobilityAttmptd);
   }
   /* CSG_DEV end */
 
#endif/*WR_RSYS_KPI*/
   hovrAttempted++;

   RLOG_ARG2(L_EVENT,DBG_CRNTI, transCb->ueCb->crnti,"[X2AP]:Received HANDOVER REQUEST "
   "[Old-eNB-UE-X2AP-ID:%d][Cause:%d]",
         transCb->u.x2TgtHoTrans.old_eNB_UE_X2ap_Id,
         transCb->u.x2TgtHoTrans.cztCause->val.radioNw.val);
   /* update MME data base */
   wrUmmAddUeToMme(transCb->ueCb, x2TgtHoTrans->mmeId);

   RETVALUE(ROK);
}/*End of wrUmmX2TgtHoPrcHoReq*/

/**
 * @brief 
 *    This function is used to parse Path Switch Request Ack Message received
 *    from peer eNB.
 *
 * @details
 *    Function: wrUmmS1ParsePathSwitchReqAck
 *
 *    Processing steps:
 *       -# Parse Path Switch Request Ack message.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmS1ParsePathSwitchReqAck
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                             *ueCb = transCb->ueCb;
   SztS1AP_PDU                        *s1ap_pdu = NULLP;
   SztSuccessfulOutcome               *successfulOutcomeMsg = NULLP;
   SztProtIE_Field_PathSwRqstAckgIEs  *ie = NULLP;
   U8                                 noComp = 0;
   U8                                 pthSwthRqstAckIeIdx = 0;
   U32                                dlBitRate = 0;
   U32                                ulBitRate = 0;

   ueCb->s1ConCb->spConnId = transCb->msg->u.cfmPdu->spConnId;

   ueCb->s1ConCb->s1apConnState = WR_S1AP_CONNECTED;

   s1ap_pdu = &transCb->msg->u.cfmPdu->pdu->pdu;
   successfulOutcomeMsg = &s1ap_pdu->val.successfulOutcome;
   noComp = successfulOutcomeMsg->value.u.sztPathSwRqstAckg.protocolIEs.noComp.val;
   for (pthSwthRqstAckIeIdx = 0; pthSwthRqstAckIeIdx < noComp; pthSwthRqstAckIeIdx++)
   {
      ie = &successfulOutcomeMsg->value.u.sztPathSwRqstAckg.protocolIEs.member[pthSwthRqstAckIeIdx];
      switch (ie->id.val)
      {
         case Sztid_eNB_UE_S1AP_ID:
         {
            ueCb->s1ConCb->enb_ue_s1ap_id = ie->value.u.sztENB_UE_S1AP_ID.val;
            break;
         }
         case Sztid_MME_UE_S1AP_ID:
         {
            ueCb->s1ConCb->mme_ue_s1ap_id = ie->value.u.sztid_MME_UE_S1AP_ID.val;
            break;
         }
         case Sztid_uEaggregateMaxBitrate:
         {
            if (ie->value.u.sztUEAggMaxBitrate.pres.val == PRSNT_NODEF)
            {
               /**
                * ENB shall replace previously provided UE Aggregate Max Bit Rate
                * by the received UE Aggregate Max Bit Rate in the UE Context.
                */
               WR_GET_U32_FRM_OSXL(dlBitRate,ie->value.u.sztUEAggMaxBitrate.uEaggregateMaxBitRateDL);
               transCb->u.x2TgtHoTrans.ambr.dl = dlBitRate;
               WR_GET_U32_FRM_OSXL(ulBitRate,ie->value.u.sztUEAggMaxBitrate.uEaggregateMaxBitRateUL);
               transCb->u.x2TgtHoTrans.ambr.ul = ulBitRate;                        

               if((0 == dlBitRate) && (0 == ulBitRate))
               {
                  RLOG2(L_ERROR,
                      "Invalid AMBR value [dl = %ld, ul = %ld] received"
                       "sending context release", dlBitRate, ulBitRate);
                  transCb->state = WR_UMM_TRANS_RELEASE_CALL;
                  transCb->errorCause = WR_UMM_SEMANTIC_ERROR;
                  RETVALUE(RFAILED);
               }
            }/* End of if condition.*/
            else
            {
               /**
                * eNB shall use previous UE Aggregate Max Bit Rate.
                */
            }/*End of Else part*/
            break;
         }
         case Sztid_E_RABToBeSwedULLst:
         {
            if (ROK != wrUmmUpdUlTunPthSwtAck(transCb, s1ap_pdu))
            {
               RLOG0(L_ERROR, "Failed to Fill E_RABToBeSwedULLst.");
               RETVALUE(RFAILED);
            }/**/
            break;
         }
         case Sztid_E_RABToBeRlsdLst:
         {
            if (ie->value.u.sztE_RABLst.noComp.val == 0)
            {
               /**
                * No Need to Delete Any Bearer.
                */
            }/*End Of ie->value.u.sztE_RABLst.noComp == 0*/
            else
            {
               /**
                * Need to delete the bearer.
                */
            }/* End of else part.*/
            break;
         }
         case Sztid_SecurCntxt:
         {
            /** 
             * The eNB shall use this security Context for the next X2 
             * handover or Intra eNB Handover.
             */
            transCb->u.x2TgtHoTrans.new_nextHopChainingCount = 
               ie->value.u.sztSecurCntxt.nextHopChainingCount.val;
            /**
             * Fill Security Key.
             */
            /* ccpu00126532 */
            cmMemcpy(transCb->u.x2TgtHoTrans.secCfg->kENB,
                  ie->value.u.sztSecurCntxt.nextHopParameter.val,
                  WR_KENB_KEY_LEN);
#ifdef TENB_AS_SECURITY
#if  (defined SS_RBUF) || (defined LTE_QCOM)
            cmMemcpy(transCb->u.x2TgtHoTrans.hzSecCfg->kENB,
                  ie->value.u.sztSecurCntxt.nextHopParameter.val,
                  WR_KENB_KEY_LEN);
#endif
#endif
            transCb->ueCb->secCfg = transCb->u.x2TgtHoTrans.secCfg;
#ifdef TENB_AS_SECURITY
#if  !(defined SS_RBUF) && !(defined LTE_QCOM)
            wrPlatSecExtractSecKeys1(transCb->ueCb,
                  transCb->transId, CTF_KDF_TYPE2);
#else
   TknStrBSXL tmpSecKey;
   wrUmmSecExtractX2HOSecKeys3(transCb, &(tmpSecKey),&(transCb->u.x2TgtHoTrans.asKeyGenerated),CTF_KDF_TYPE4);
   tmpSecKey.len = WR_KENB_KEY_LEN;
   WR_ALLOC(&(tmpSecKey.val),WR_KENB_KEY_LEN); 
   if (NULLP == tmpSecKey.val)
   {
      RLOG0(L_ERROR, "Error in getting Allocating Memory SecKey");
      RETVALUE(RFAILED);
   }
   cmMemcpy(tmpSecKey.val,transCb->u.x2TgtHoTrans.hzSecCfg->kENB,WR_KENB_KEY_LEN);
   wrUmmSecExtractX2HOSecKeys3(transCb, &(tmpSecKey),&(transCb->u.x2TgtHoTrans.asKeyGenerated),CTF_KDF_TYPE3);
   WR_FREE((tmpSecKey.val),WR_KENB_KEY_LEN);
   transCb->ueCb->hzSecCfg = transCb->u.x2TgtHoTrans.hzSecCfg;
   transCb->u.x2TgtHoTrans.state = WR_UMM_TGT_HO_HZ_KEY_DER_DONE;
#endif
#endif
            break;
         }
         case Sztid_MME_UE_S1AP_ID_2:
         {
            wrFillTknU32(&transCb->u.x2TgtHoTrans.mmeUeS1apId2,
                 ie->value.u.sztid_MME_UE_S1AP_ID_2.val);
            RLOG_ARG1(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti, "MME UE S1AP "
               "ID 2 :id [%ld]", transCb->u.x2TgtHoTrans.mmeUeS1apId2.val);
            wrFillTknU32(&ueCb->s1ConCb->mmeUeS1apId2,
                  transCb->u.x2TgtHoTrans.mmeUeS1apId2.val);
            break;
         }
         /* CSG Member Status IE */
         case Sztid_CSGMembershipStatus:
         {
            if (ie->value.u.sztCSGMembershipStatus.pres != NOTPRSNT)
            {
               transCb->u.x2TgtHoTrans.ueMbrShpSts =
                  ie->value.u.sztCSGMembershipStatus.val;
               RLOG_ARG1(L_DEBUG, DBG_CRNTI, ueCb->crnti, 
                     "Received UE membership status in Path Switch "
                     "MemberStatus [%d]", transCb->u.x2TgtHoTrans.ueMbrShpSts);
            }
         }
      }/* End of Switch.*/
   }/* End of for loop.*/ 
   RLOG_ARG2(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"Received X2 PATH SWITCH "
      "REQUEST ACKNOWLEDGE eNB-UE-S1AP-ID[%d] MME-UE-S1AP-ID[%lu]",
      ueCb->s1ConCb->enb_ue_s1ap_id, ueCb->s1ConCb->mme_ue_s1ap_id);
   RETVALUE(ROK);
}/* End of wrUmmS1ParsePathSwitchReqAckMsg.*/

/**
 * @brief 
 *    This function is used to handle Path Switch Request Ack Message received
 *    from peer eNB.
 *
 * @details
 *    Function: wrUmmX2TgtHoPrcPathSwitchReqAck
 *
 *    Processing steps:
 *       -# Parse Path Switch Request Ack message.
 *       -# Send UE context Release Message.
 *       -# Release the transaction.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoPrcPathSwitchReqAck
(
 WrUmmTransCb                *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUeCapInfo                   *ueCap = ueCb->ueCap;
   WrUmmX2TgtHoTrans             *x2TgtHoTrans = &transCb->u.x2TgtHoTrans;
   WrUmmRatLst                ratLst;
   WrUmmCellCb               *cellCb;

   WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId);
   /* Klockwork P1 warning fix */
   if (cellCb == NULLP)
   {        
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)transCb->ueCb->cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }


   /**
    * Processing of Pathe Switch Request Ack Messsage.
    */
   if (ROK != wrUmmS1ParsePathSwitchReqAck(transCb))
   {
       RLOG0(L_ERROR, "Failed to parse Path Switch Request Ack Message.");
       RETVALUE(RFAILED);
   }/* End of wrUmmS1ParsePathSwitchReqAck*/

   RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[S1AP]:Received PATH SWITCH "
             "REQUEST ACK [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
             transCb->ueCb->s1ConCb->mme_ue_s1ap_id,
             transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
   /* ccpu00135331*/
   transCb->u.x2TgtHoTrans.state = WR_UMM_TGT_HO_PATH_SWITCH_ACK_RCVD; 

   /* update next hop chaining count information from mme */
   ueCb->nhChainCnt = transCb->u.x2TgtHoTrans.new_nextHopChainingCount;

   /* stop Path Switch Timer */
   wrStopTransTmr(transCb, WR_TMR_TGT_PATH_SWITCH);
   /*
    * Trigger UE Context Release towards Src eNb after receiving 
    * PATH SWITCH REQ ACK from MME
    * ccpu00126780 FIX
    */ 
   transCb->ueCb->ueAttachedtoMme = TRUE; /* ccpu00131019 HOFIX_KOREA */ 
   wrUmmX2TgtHoSndUeCtxtRls(transCb);
   /* CSG membership status */
   if (cellCb->csgAccessMode == WR_ENB_CLOSED_ACCESS)
   {
      if (transCb->u.x2TgtHoTrans.ueMbrShpSts == WR_UMM_CSG_NOT_MEMBER)
      {
         /* Initiate the UE context release procedure here */
         transCb->errorCause = WR_UMM_CTXT_REL_CSG_SUBSCRIPTION_EXP;
         transCb->state = WR_UMM_TRANS_RELEASE_CALL;
      }/* if WR_UMM_CSG_NOT_MEMBER*/   
      else
      {
         wrUmmCsgAddCsgIdIntoMbrLst(ueCb, cellCb->csgId,WR_UMM_CSG_MEMBER);
      }
   }/* if WR_ENB_CSG_CLOSED_ACCESS */
   else if (cellCb->csgAccessMode == WR_ENB_OPEN_ACCESS)
   {
      /* to avoid any EPC issues of receiving membership status in path switch
       * req ack when cell is in open access mode */
      transCb->u.x2TgtHoTrans.ueMbrShpSts = WR_UMM_CSG_MEMBER;
      RLOG_ARG1(L_DEBUG, DBG_CRNTI, ueCb->crnti, 
            "Setting default membership status to UE in OPEN Access after "
            "Path Switch Procedure. MemberStatus [%d]",
            transCb->u.x2TgtHoTrans.ueMbrShpSts);
   }
   else
   {
      if (transCb->u.x2TgtHoTrans.ueMbrShpSts == WR_UMM_CSG_NOT_MEMBER)
      {
         wrUmmCsgAddCsgIdIntoMbrLst(ueCb, cellCb->csgId,WR_UMM_CSG_NOT_MEMBER);
      }
      else
      {
         wrUmmCsgAddCsgIdIntoMbrLst(ueCb, cellCb->csgId,WR_UMM_CSG_MEMBER);
      }
   }/* WR_ENB_CSG_HYBRID_ACCESS */

   /* Check for the UE Capability Info  when the supported band list IE in 
    * the UE Radio Capability IE contains IRAT bands and specific IRAT  octet 
    * string is not present in the Source to Target Container which is updated in
    * the ueCb, then sen UECAP ENQ to UE with required IRATs type */
   if(PRSNT_NODEF == ueCap->eUTRA_Cap.interRAT_Params.pres.pres)
   {
      wrFillRatLstRatType(ueCb->ueCap,&ratLst,&x2TgtHoTrans->handInUeCdma);
      if(x2TgtHoTrans->handInUeCdma == TRUE)
      {
         if(ROK != wrUmmUeCapSndUeCapEnq(transCb, &ratLst))
         {
            RLOG0(L_ERROR,"Failed to Send UECAP Enq.");
            RETVALUE(RFAILED);
         }
      }
   }
  
#ifndef TENB_AS_SECURITY
   /* ccpu00135331*/
   x2TgtHoTrans->state = WR_UMM_TGT_HO_HZ_KEY_DER_DONE;
   /*DRX QCI*/                 
   if((ueCb->isGBRBearerExists == NOT_IN_USE))
   {
      RLOG0(L_INFO,
        "GBR bearer is not exists in Initail Context setup");
      if(wrUmmStartAnr(transCb->ueCb,FALSE) != ROK)
      {
         RLOG0(L_ERROR,"Failed to Start the ANR");
      }
   }           
   /* ccpu00135331*/
#ifndef SS_RBUF
   if( ( 0 == x2TgtHoTrans->rbInfo->expectedTnlRsp ) &&
       ( FALSE == x2TgtHoTrans->handInUeCdma ))
   {
      wrUmmTransComplete(transCb);
   }
#endif
#endif
#ifndef T2K3K_SPECIFIC_CHANGES
   /**
    * release the transaction
    */
   /* ccpu00135331*/
   if((0 == x2TgtHoTrans->rbInfo->expectedTnlRsp ) &&
      (FALSE == x2TgtHoTrans->handInUeCdma))
   {
      wrUmmTransComplete(transCb);
   }
#endif
   /*Inform about new UE to SON*/
   wrIfmSonAddUeInd(ueCb);

   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoPrcPathSwitchReqAck*/

/**
 * @brief 
 *    This function is used to fill SRB configuration information.
 *
 * @details
 *    Function: wrUmmX2TgtHoRrcCfgAddSrbCfg
 *
 *    Processing steps:
 *       -# Trigger AddSrbCfg
 *
 * @param[in] 
 *    cellId    : cellId
 * @param[in]
 *    srbCfg    : SRB Configuration Information
 * @param[in]
 *    isSrb1    : SRB Id
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoRrcCfgAddSrbCfg
(
U16                          cellId,
NhuRbCfgInfo                 *srbCfg,
U8                           isSrb1
)
{
   if (wrUmmRrcAddSrbCfg(cellId, srbCfg, isSrb1) != ROK)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to fill DRB configuration information.
 *
 * @details
 *    Function: wrUmmX2TgtHoRrcReconfigAddDrbCfg
 *
 *    Processing steps:
 *       -# Trigger AddDrbCfg
 *
 * @param[in] 
 *    cellId    : cellId
 * @param[in]
 *    drbCfg    : DRB Configuration Information
 * @param[in]
 *    rabInfo   : rab Information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoRrcReconfigAddDrbCfg
(
 U16                         cellId,
NhuRbCfgInfo                 *drbCfg,
WrUeRAB                      *rabInfo
)
{
   /* SPS changes starts */
   if (wrUmmRrcAddDrbCfg(cellId, drbCfg, rabInfo->rbId, rabInfo->lchId,
            rabInfo->qci, rabInfo->isUlSpsEnabled) != ROK)
   /* SPS changes ends */
   {
      RETVALUE(RFAILED);              
   }
   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to fill RRC Configuration request.
 *
 * @details
 *    Function: wrUmmX2TgtHoFillRrcCfg
 *
 *    Processing steps:
 *       -# Fill RRC Configuration Request
 *
 * @param[in] 
 *    transCb    : transaction information
 * @param[in]
 *    evnt       : Event SDUs
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoFillRrcCfg
(
WrUmmTransCb                 *transCb,
NhuCfgReqSdus                *evnt
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   NhuRbCfgInfo              *drbCfg;
   WrUmmX2TgtHoRabInfo       *rabInfo;
   NhuUeCfgInfo              *ueCfg;
   NhuRbCfgList              *rbCfgList;
   U32                       idx;
   U32                       numDrbs = 0;
   U16                       noOfRbConfgrd;

   ueCfg = &(evnt->cfgInfo);

   /**
    * Fill up the UE configuration.
    */
   ueCfg->ueCfgBitMsk |= NHU_EXP_CFG_UE_CFG;
   ueCfg->ueInfo.newCrnti = ueCb->crnti;

   ueCfg->ueCfgBitMsk |= NHU_EXP_CFG_TRCH_CFG;
   wrUmmRrcFillTrchCfg(&(ueCfg->trChCfg),ueCb);

   ueCfg->ueCfgBitMsk |= NHU_EXP_CFG_RB_CFG;
   /**
    * Fill RB info NHU_EXP_CFG_RB_CFG
    */
   ueCfg->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_ADD;
   rbCfgList = &(ueCfg->rbCfgAllList.rbToAddCfgList);
   noOfRbConfgrd = transCb->u.x2TgtHoTrans.noOfAdmitdBearers;
   /**
    * allocating memory for numErabs + SRB1 and SRB2 
    */
   WR_GET_MEM(evnt, (sizeof(NhuRbCfgInfo) * noOfRbConfgrd + 2), 
                                               &(rbCfgList->rbCfgInfoList));

   /**
    * filling SRB1 configuration
    */
   wrUmmX2TgtHoRrcCfgAddSrbCfg(ueCb->cellId,
                        &rbCfgList->rbCfgInfoList[0], TRUE);

   /**
    * filling SRB2 configuration
    */
   wrUmmX2TgtHoRrcCfgAddSrbCfg(ueCb->cellId, 
                      &rbCfgList->rbCfgInfoList[1], FALSE);

   /**
    * filling other ERAB configuration
    */
   for (idx = 0; idx < transCb->u.x2TgtHoTrans.numOfRbCfgd; idx++)
   {
      rabInfo = &(transCb->u.x2TgtHoTrans.rbInfo[idx]);
      drbCfg  = &rbCfgList->rbCfgInfoList[numDrbs + 2];

      /**
       * We send information of only those RABs that are successful
       */
      if ((rabInfo->rabAllowed == TRUE) &&
          (rabInfo->schdCfgState == WR_UMM_TGT_HO_RAB_SCH_CFG_SUCC))
      {
         wrUmmX2TgtHoRrcReconfigAddDrbCfg(ueCb->cellId, drbCfg, &rabInfo->drbInfo);
         numDrbs++;
      }
   }
   rbCfgList->numOfRbCfgInfo = numDrbs + 2;
   rbCfgList->isHO = TRUE;

   /* ccpu00126532 */
   /* Configure AS Security */
#ifdef TENB_AS_SECURITY
   ueCfg->ueCfgBitMsk |= NHU_EXP_CFG_SEC_CFG;
   wrUmmRrcFillSecCfg(&(ueCfg->secCfg), transCb->u.x2TgtHoTrans.secCfg);
#endif

   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to configure RRC
 *
 * @details
 *    Function: wrUmmX2TgtHoRrcCfg
 *
 *    Processing steps:
 *       -# Fill Configure request
 *       -# Send configuration request to RRC.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoRrcCfg
(
WrUmmTransCb                 *transCb
)
{
   NhuCfgReqSdus             *rrcCfg;
   WrUeCb                    *ueCb   = transCb->ueCb;

   WR_ALLOCEVNT(&rrcCfg, sizeof(NhuCfgReqSdus));
   if (rrcCfg == NULLP)
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   rrcCfg->hdr.cellId  = ueCb->cellId;
   //rrcCfg->hdr.ueId    = ueCb->crnti;
   rrcCfg->hdr.ueId    = 0;
   rrcCfg->hdr.transId = transCb->transId | WR_UMM_TGT_HO_RRC_CFG;

   /* Build the configuration towards RRC                                 */
   wrUmmX2TgtHoFillRrcCfg(transCb, rrcCfg);
   if (RFAILED == wrIfmRrcSndCfgReq(rrcCfg))
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe failed.");
      WR_FREEEVNT(rrcCfg);
      RETVALUE(RFAILED);
   }
   
   RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"Sending RRC layer "
      "reconfiguration request");

   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to send tunnel configuration request.
 *
 * @details
 *    Function: wrUmmX2TgtHoSndTnlCreateReq
 *
 *    Processing steps:
 *       -# Fill configuration information.
 *       -# Send tunnel configuration request to egtp.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @param[in]
 *    ueCb       : UE Information
 * @param[in]
 *    rabInfo    : Rab Information
 * @param[in]
 *    peerAddr   : peer Information
 * @param[in]
 *    tnlType    : tunnel type
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoSndTnlCreateReq
(
U32                          transId,
WrUeCb                       *ueCb,
WrUmmX2TgtHoRabInfo          *rabInfo,
CmTptAddr                    *peerAddr,
WrDamTnlType                 tnlType
)
{
   WrDamTnlInfo              *tnlInfo;
   CmTptAddr                 *lclAddr;

   CmTptAddr    dstIpAddr1234;
   S8           dummyIpv4[] = "1.2.3.4";
   S8           dummyIpv6[] = "::1";
   S8           str[46];

   WR_ALLOC_SHAREBLE_BUF(&tnlInfo, sizeof(WrDamTnlInfo));
   if (NULLP == tnlInfo)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   tnlInfo->hoSrcEnodeb = FALSE;
   tnlInfo->transId = transId;
   tnlInfo->tnlId.cellId = ueCb->cellId;
   tnlInfo->tnlId.crnti = ueCb->crnti;
   tnlInfo->tnlId.drbId = rabInfo->drbInfo.rbId;
   tnlInfo->tnlType = tnlType;
   tnlInfo->hoType = WR_HO_TYPE_X2;
   /* Retreive the enb ip address. */
   /* We know that the enb and X2 enb neighbor is connected with ip 
    * address. We use this ip address type to retrieve it.
    */
   wrEmmGetDatAppAddr(&lclAddr, peerAddr->type);
   if(tnlType == WR_TNL_NORMAL)
   {
      wrCpyCmTptAddr(&tnlInfo->dstAddr, peerAddr);
   }
   else
   {
      /* RRC-REL10-Upgrade start */
      /* DAM UE control block needs to be updated with full configuration flag
       * that being sent in wrIfmDamTnlCreatReq */
      if(ueCb->isFullCfgReqd)
      {
         tnlInfo->isFullCfg = TRUE;
      }
      /* RRC-REL10-Upgrade end */
      if(lclAddr->type == CM_TPTADDR_IPV4)
      {
         wrUpdateIpAddr((U8 *)dummyIpv4,&dstIpAddr1234);
         dstIpAddr1234.u.ipv4TptAddr.port = WR_DFLT_EGTP_PORT;
         wrCpyCmTptAddr(&tnlInfo->dstAddr, &(dstIpAddr1234));
         cmInetNtop(tnlInfo->dstAddr.type,&tnlInfo->dstAddr.u.ipv4TptAddr.
          address,str,MAX_IPADDR_LEN);
      }
      else if(lclAddr->type == CM_TPTADDR_IPV6)
      {
         wrUpdateIpAddr((U8 *)dummyIpv6,&dstIpAddr1234);
         dstIpAddr1234.u.ipv6TptAddr.port = WR_DFLT_EGTP_PORT;
         wrCpyCmTptAddr(&tnlInfo->dstAddr, &(dstIpAddr1234));
         cmInetNtop(tnlInfo->dstAddr.type,&tnlInfo->dstAddr.u.ipv6TptAddr.
          ipv6NetAddr,str,MAX_IPADDR_LEN);

      }
      RLOG_STR(L_DEBUG,"PKT_FWD_TNL IP[%s]",str);
   }
   wrCpyCmTptAddr(&tnlInfo->srcAddr, lclAddr);
   tnlInfo->remTeid = rabInfo->drbInfo.remEgtpTunId;
   wrUtlGetDscp(ueCb->cellId, rabInfo->drbInfo.qci, &(tnlInfo->dscp));

   rabInfo->tnlCfgState = WR_UMM_TGT_HO_TNL_CFG_SENT;
   wrIfmDamTnlCreatReq(transId, tnlInfo);

   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to configure tunnels.
 *
 * @details
 *    Function: wrUmmX2TgtHoCfgTnls
 *
 *    Processing steps:
 *       -# Fill configuration information.
 *       -# Send tunnel configuration request.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoCfgTnls
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &transCb->u.x2TgtHoTrans;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmX2TgtHoRabInfo       *rabInfo = NULLP;
   U32                       transId;
   U16                       idx;
   U16                       noOfRbs;

   noOfRbs = x2TgtHoTrans->numOfRbCfgd;
   for (idx = 0; idx < noOfRbs; idx++)
   {
      rabInfo = &x2TgtHoTrans->rbInfo[idx];
      /* TODO : temporarily added. need to remove when DAM becomes loosely
       *        coupled
       */
      if (rabInfo->rabAllowed == TRUE)
      {
         WR_UMM_TGT_HO_CALC_NUM_TNL_CFG(rabInfo);
         transId = transCb->transId | idx;
         RLOG_ARG3(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Sending Tunnel "
            "Create Request rbID [%d] tnlType [%d] expectedResp [%d]",
            rabInfo->drbInfo.rbId, WR_TNL_NORMAL, rabInfo->expectedTnlRsp);
         wrUmmX2TgtHoSndTnlCreateReq(transId, ueCb, rabInfo, &rabInfo->drbInfo.sgwAddr,
               WR_TNL_NORMAL);
         if(rabInfo->dlFwding == TRUE)
         {
            if(rabInfo->drbInfo.rbDir == WR_DIR_UL || 
               rabInfo->drbInfo.rbDir == WR_DIR_DL_UL)
            {
               RLOG_ARG2(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Sending Tunnel "
                  "Create Request rbID [%d] tnlType [%d] ", rabInfo->drbInfo.rbId,
                  WR_TNL_UL_DATFWD);
               wrUmmX2TgtHoSndTnlCreateReq(transId, ueCb, rabInfo, &x2TgtHoTrans->peerAddr,
                     WR_TNL_UL_DATFWD);
            }
            if(rabInfo->drbInfo.rbDir == WR_DIR_DL || 
               rabInfo->drbInfo.rbDir == WR_DIR_DL_UL)
            {
               RLOG_ARG2(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Sending Tunnel "
                  "Create Request rbID [%d] tnlType [%d] ", rabInfo->drbInfo.rbId,
                  WR_TNL_DL_DATFWD);
               wrUmmX2TgtHoSndTnlCreateReq(transId, ueCb, rabInfo, &x2TgtHoTrans->peerAddr,
                     WR_TNL_DL_DATFWD);
            }
         }
      }
   }

   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to process Dam Confirm.
 *
 * @details
 *    Function: wrUmmX2TgtHoPrcDamCfm
 *
 *    Processing steps:
 *       -# process Dam Confirm.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoPrcDamCfm
(
WrUmmTransCb                  *transCb
)
{
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &transCb->u.x2TgtHoTrans;
   WrUmmX2TgtHoRabInfo       *rabInfo = NULLP;
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

   noOfRbs = x2TgtHoTrans->numOfRbCfgd;
   for (idx = 0; idx < noOfRbs; idx++)
   {
      rabInfo = &x2TgtHoTrans->rbInfo[idx];
      if(rabInfo->drbInfo.rbId == drbId)
      {
         if(status == WR_DAM_CFG_OK)
         {
            switch(tnlType)
            {
               case WR_TNL_NORMAL:
               {
                  rabInfo->tnlCfgState = WR_UMM_TGT_HO_TNL_CFG_SUCC;
                  rabInfo->drbInfo.locEgtpTunId  = damCfm->lclTeId;
                  break;
               }
               case WR_TNL_DL_DATFWD:
               {
                  rabInfo->tnlCfgState = WR_UMM_TGT_HO_TNL_CFG_SUCC;
                  rabInfo->datFwdDlLocEgtpTunId  = damCfm->lclTeId;
                  break;
               }
               case WR_TNL_UL_DATFWD:
               {
                  rabInfo->tnlCfgState = WR_UMM_TGT_HO_TNL_CFG_SUCC;
                  rabInfo->datFwdUlLocEgtpTunId  = damCfm->lclTeId;
                  break;
               }
            }
         }
         else
         {
            rabInfo->tnlCfgState = WR_UMM_TGT_HO_TNL_CFG_FAIL;
         }
         rabInfo->expectedTnlRsp--;
         RLOG_ARG3(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"Received Tunnel "
            "Create Confirm rbID [%d] tnlType [%d] pendingResponse [%d]",
            rabInfo->drbInfo.rbId, tnlType, rabInfo->expectedTnlRsp);
      }
   }

   for(idx = 0; idx <noOfRbs; idx++)
   {
      rabInfo = &x2TgtHoTrans->rbInfo[idx];
      /* Here, we are adding check for only those RABs which are set as
       * rabAllowed. So no need to verify for those bearer which are not
       * allowed by eNB */
      /* Fix for ccpu00138196 */
      if (rabInfo->rabAllowed == TRUE)
      {
         if((rabInfo->tnlCfgState == WR_UMM_TGT_HO_TNL_CFG_SENT) ||
               (rabInfo->expectedTnlRsp != 0))
         {
            RETVALUE(ROK);
         }
      }
   }
   x2TgtHoTrans->state = WR_UMM_TGT_HO_TNL_CFG_DONE;
   RETVALUE(ROK);
}

/* ccpu00126336 */
/**
 * @brief 
 *    This function is used to process Dam Mod Confirm.
 *
 * @details
 *    Function: wrUmmX2TgtHoPrcDamModCfm
 *
 *    Processing steps:
 *       -# process Dam modify Confirm.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoPrcDamModCfm
(
WrUmmTransCb                  *transCb
)
{
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &transCb->u.x2TgtHoTrans;
   WrUmmX2TgtHoRabInfo       *rabInfo = NULLP;
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

   noOfRbs = x2TgtHoTrans->numOfRbCfgd;
   for (idx = 0; idx < noOfRbs; idx++)
   {
      rabInfo = &x2TgtHoTrans->rbInfo[idx];

      if(rabInfo == NULLP)
      {
         RLOG0(L_ERROR, "Failed To get rabInfo.");
         RETVALUE(RFAILED);
      }

      if(rabInfo->drbInfo.rbId == drbId)
      {
         rabInfo->expectedTnlRsp--;
         if(status == WR_DAM_CFG_OK)
         {
            RLOG_ARG3(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"Tunnel "
               "Modification Successful rbID [%d] tnlType [%d] "
               "pendingResponse [%d]", rabInfo->drbInfo.rbId, tnlType,
               rabInfo->expectedTnlRsp);
         }
         else
         {
            RLOG_ARG3(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Tunnel "
               "Modification FAILED rbID [%d] tnlType [%d] pendingResponse [%d]",
               rabInfo->drbInfo.rbId, tnlType, rabInfo->expectedTnlRsp);
            /* Release the UE as the UL packets will not reach Core Network
             * properly */
            transCb->errorCause = WR_UMM_REL_CAUSE_UL_TNL_MOD_FAIL;
            transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
            RETVALUE(ROK);
                  
         }
      }
   }

   if(rabInfo == NULLP)
   {
       RLOG0(L_ERROR, "Failed To get rabInfo.");
       RETVALUE(RFAILED);
   }

   if(rabInfo->expectedTnlRsp == 0)
   {
      /**
       * release the transaction
       */
   /* ccpu00135331*/
      if(( x2TgtHoTrans->state == WR_UMM_TGT_HO_HZ_KEY_DER_DONE ) 
        &&( FALSE == x2TgtHoTrans->handInUeCdma ))
      {
         wrUmmTransComplete(transCb);
      }
   }

   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to parse path switch request Failure message.
 *
 * @details
 *    Function: wrUmmX2TgtHoParsePathSwitchReqFail
 *
 *    Processing steps:
 *       -# Parse Path Switch Request Failure Message.       
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoParsePathSwitchReqFail
(
 WrUmmTransCb                *transCb
)
{
   WrUeCb                             *ueCb = transCb->ueCb;
   SztS1AP_PDU                        *s1ap_pdu = NULLP;
   SztUnsuccessfulOutcome             *unSuccessfulOutcomeMsg = NULLP;
   SztProtIE_Field_PathSwRqstFailIEs  *ie = NULLP;
   U8                                 noComp = 0;
   U8                                 pthSwthRqstFailIeIdx = 0;

   ueCb->s1ConCb->spConnId = transCb->msg->u.cfmPdu->spConnId;

   s1ap_pdu = &transCb->msg->u.cfmPdu->pdu->pdu;
   unSuccessfulOutcomeMsg = &s1ap_pdu->val.unsuccessfulOutcome;
   noComp = unSuccessfulOutcomeMsg->value.u.sztPathSwRqstFail.protocolIEs.noComp.val;
   for (pthSwthRqstFailIeIdx = 0; pthSwthRqstFailIeIdx < noComp; pthSwthRqstFailIeIdx++)
   {
      ie = &unSuccessfulOutcomeMsg->value.u.sztPathSwRqstFail.protocolIEs.member[pthSwthRqstFailIeIdx];
      switch(ie->id.val)
      {
         case Sztid_MME_UE_S1AP_ID:
            {
               if (ie->value.u.sztMME_UE_S1AP_ID.val != 
                     ueCb->s1ConCb->mme_ue_s1ap_id)
               {
                  /*ccpu00127802*/
                  RLOG0(L_ERROR, "Invalid MME UE S1AP ID");
                  RETVALUE(RFAILED);
               }
               break;
            }
         case Sztid_eNB_UE_S1AP_ID:
            {
               ueCb->s1ConCb->enb_ue_s1ap_id =
                  ie->value.u.sztENB_UE_S1AP_ID.val;
               break;
            }
            /*ccpu00129156*/
          case Sztid_Cause:            
            {
                wrEmmUpdateKpiCounter(&ie->value.u.sztCause);

               /* No need to fill the Cause since the UE will be released */
               break;
            }
         default:
            RETVALUE(RFAILED);
      }
   }
   RETVALUE(ROK);
}
/**
 * @brief 
 *    This function is used to process path switch request Failure message.
 *
 * @details
 *    Function: wrUmmX2TgtHoPrcPathSwitchReqFail
 *
 *    Processing steps:
 *       -# Parse Path Switch Request Failure Message.
 *       -# Release Transaction Complete.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoPrcPathSwitchReqFail
(
WrUmmTransCb                 *transCb
)
{
   /**
    * Processing of Pathe Switch Request Failure Messsage.
    */
   if (ROK != wrUmmX2TgtHoParsePathSwitchReqFail(transCb))
   {
       RLOG0(L_ERROR, "Failed to parse Path Switch Request Fail Message.");
       RETVALUE(RFAILED);
   }/* End of wrUmmX2TgtHoParsePathSwitchReqFail */

   RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[S1AP]:Received PATH SWITCH "
             "REQUEST FAILURE [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
             transCb->ueCb->s1ConCb->mme_ue_s1ap_id,
             transCb->ueCb->s1ConCb->enb_ue_s1ap_id);
   /**
    * MME explicitly trigger Detach Procedure.So no need to send RRC
    * Connection Release message to the UE
    */
   /**
    * release the transaction
    */
   wrUmmTransComplete(transCb);
   /*ccpu00129156*/
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is called when any message related to S1 interface is 
 *    received. This is the main function that determines next course of action 
 *    based on the message received.
 *
 * @details
 *    Function: wrUmmX2TgtHoHdlS1apPdu
 *
 *    Processing steps:
 *       -# Based on PDU type, trigger corresponding message handler
 *
 *          -# If PDU type is Initiating Message,
 *             -# Trigger Initiating message handler.
 *          -# If PDU type is Successful Message,
 *             -# Trigger Successful message handler.
 *          -# If PDU type is Successful Message,
 *             -# Trigger Successful message handler.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
EXTERN U32 hoSucessHopCount;
PRIVATE S16 wrUmmX2TgtHoHdlS1apPdu
(
 WrUmmTransCb                *transCb
)
{
    WrUmmIncMsg               *msg = transCb->msg;

    switch(msg->u.cfmPdu->pdu->pdu.choice.val)
    {
        case INITIATING_MSG:
            break;
        case SUCCESSFUL_OUTCOME:
        {
            switch (msg->u.cfmPdu->pdu->pdu.val.successfulOutcome.procedureCode.val)
            {
                case Sztid_PathSwRqst:
                {
                   RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Received"
                      " Path Switch Request Acknowledgement");
                    /**
                     * Process Path Switch Request Ack Message.
                     */
                    if (ROK != wrUmmX2TgtHoPrcPathSwitchReqAck(transCb))
                    {
                        RLOG0(L_ERROR, "Failed to Process Pathe Switch Request "
                           "Ack Message.");
                        RETVALUE(RFAILED);
                    }/* End of wrUmmX2TgtHoPrcPathSwitchReqAck*/
                    hoSucessHopCount++;
                    /*ccpu00129850 - Removed calling of wrUmmX2TgtHoSndBufferedNasMsg */
                    break;
                }
                default :
                    RLOG0(L_ERROR, "Invalid Procedure Code.");
                    RETVALUE(RFAILED);
            }/* End of switch */

            break;
        }
        case UN_SUCCESSFUL_OUTCOME:
        {
           if(msg->u.cfmPdu->pdu->pdu.val.unsuccessfulOutcome.procedureCode.val == Sztid_PathSwRqst)
           {
              RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Received Path"
                 "Switch Request Failure");
                   /*
                    * Process Path Switch Request Failure Message.
                    */
                   if (ROK != wrUmmX2TgtHoPrcPathSwitchReqFail(transCb))
              { 
                 RLOG0(L_ERROR, "Invalid Procedure Code.");
                      RETVALUE(RFAILED);
              }
           }
           else
           {
              RLOG0(L_ERROR, "Invalid Procedure Code.");
              RETVALUE(RFAILED);
           }
           break;
        }
        default :
            RLOG0(L_ERROR, "Invalid Message.");
            RETVALUE(RFAILED);
    }/*End of switch.*/
    RETVALUE(ROK);
}/* End of wrUmmX2TgtHoHdlS1apPdu*/

/**
 * @brief 
 *    This function is used to handle X2ap PDU received from peer eNB.
 *
 * @details
 *    Function: wrUmmX2TgtHoHdlX2apPdu
 *
 *    Processing steps:
 *       -# Based on PDU type, trigger corresponding message handler
 *
 *          -# If PDU type is Initiating Message,
 *             -# Trigger Initiating message handler.
 *          -# If PDU type is Successful Message,
 *             -# Trigger Successful message handler.
 *          -# If PDU type is Successful Message,
 *             -# Trigger Successful message handler.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 */

PRIVATE S16 wrUmmX2TgtHoHdlX2apPdu
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg              *msg = transCb->msg;
   switch(msg->u.x2Pdu->pdu.choice.val)
   {
      case INITIATING_MSG:
      {
         switch (msg->u.x2Pdu->pdu.val.initiatingMsg.procedureCode.val)
         {
            case Cztid_handoverPrep:
            {
               RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,
                  "Processing handover Request");
               /**
                * Process the X2AP HO Request
                */
               if (ROK != wrUmmX2TgtHoPrcHoReq(transCb))
               {
                  RLOG0(L_ERROR, "Failed to Process Handover Request Message.");
                  transCb->state = WR_UMM_TRANS_DELETE_UE; 
                  RETVALUE(RFAILED);
               }/* End of wrUmmX2TgtHoPrcHoReq*/
               break;
            } /* end of Cztid_handoverPrep */
            case Cztid_snStatusTfr:
            {
               RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,
                  "Processing SN Status");
               /**
                * Process the X2AP SN Status Transfer Request
                */
               if (ROK != wrUmmX2TgtHoPrcSnStatusTfr(transCb))
               {
                  RLOG0(L_ERROR,"Failed to Process Sn Status Transfer Message");
                  RETVALUE(RFAILED);
               }/* End of wrUmmX2TgtHoPrcSnStatusTfr*/
               break;
            }/*End of case Cztid_snStatusTfr*/
            case Cztid_handoverCancel:
            {
               /**
                * Process the X2AP Handover Cancel Message.
                */
               if (ROK != wrUmmX2PrcHoCancel(transCb))
               {
                  RLOG0(L_ERROR, "Failed to Process HO Cancel Message.");
                  RETVALUE(RFAILED);
               }/* End of wrUmmX2PrcHoCancel*/
               break;
            }/*End of case Cztid_handoverCancel*/   
            case Cztid_errorInd:
            {
               /**
                * Process the X2AP Error Indication
                */
               if (ROK != wrUmmX2PrcHoErrInd(transCb))
               {
                  RLOG0(L_ERROR,"Failed to Process HO Error Indication Message");
                  RETVALUE(RFAILED);
               }/* End of wrUmmX2PrcHoErrInd*/
               break;
            }/*End of case Cztid_errorInd*/
            default:
            {
               RLOG0(L_ERROR, "Invalid Procedure Code.");
               RETVALUE(RFAILED);
            }
         }/* End of switch */
         break;
      } /* end of case INITIATING */
/* Fix for ccpu00138191 */
      default :
         RLOG0(L_ERROR, "Invalid Message.");
         RETVALUE(RFAILED);
   }/*End of switch.*/
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoHdlX2apPdu*/

/**
 * @brief 
 *    This function is used to process RRC Encode Confirm.
 *
 * @details
 *    Function: wrUmmX2TgtHoPrcRrcEncCfm
 *
 *    Processing steps:
 *       -# Based on Encode type, trigger corresponding message handler
 *
 *          -# If Encode type is DLDCCH_MSG_ENC,
 *             -# Prepare Handover Command message.
 *          -# If Encode Type is HOCMD_ENC,
 *             -# Send Handover Request ack message.          
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 */

PRIVATE S16 wrUmmX2TgtHoPrcRrcEncCfm
(
WrUmmTransCb                 *transCb
)
{
   NhuEncCfmSdus             *encCfm = transCb->msg->u.encCfm;
   U8                        subTrans;

   subTrans = encCfm->hdr.transId & 0x0f;

   if(encCfm->sdu.status)
   {
      RLOG1(L_ERROR, "Encoding Failure of [%d]", subTrans);
      transCb->u.x2TgtHoTrans.hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW;
      transCb->u.x2TgtHoTrans.hoPrepFailCause.causeVal = 
    CztCauseRadioNwunspecifiedEnum; 
      RETVALUE(RFAILED);
   }

   switch(subTrans)
   {
      case WR_UMM_TGT_HO_DLDCCH_MSG_ENC:
      {
         if (ROK != wrUmmX2TgtHoPrepHovrCmd(transCb))
         {
            RLOG0(L_ERROR, "Failed to Prepare HO Command");
            RETVALUE(RFAILED);
         }
         break;
      }

      case WR_UMM_TGT_HO_HOCMD_ENC:
      {
         /**
          * Prepare Handover Request Ack and send it to the Source eNB.
          */
         if (ROK != wrUmmX2TgtHoSndHoReqAck(transCb))
         {
            RLOG0(L_ERROR, "Failed to Prepare Handover Req Ack Message..");
            RETVALUE(RFAILED);
         }
         }
         break;
      case WR_UMM_TGT_HO_UECAP_ENC:
         {
            wrUmmUeCapHdlUeCapEncCfm(transCb->ueCb, 
                  encCfm, transCb->transId | WR_UMM_TGT_HO_RACAP_ENC);
            RETVALUE(ROK);
         }
         break;

      case WR_UMM_TGT_HO_RACAP_ENC:
         {
            if(ROK == wrUmmUeCapHdlUeRaCapEncCfm(transCb->ueCb, encCfm))
            {
               if (PRSNT_NODEF == transCb->ueCb->ueCap->ueEutraOctStr.pres)
               {
                  wrUmmRrcDecReqHdl(transCb, &transCb->ueCb->ueCap->ueEutraOctStr,
                        NHU_DEC_SDUTYPE_EUTRA_UECAP_IE);
                  RETVALUE(ROK);
               }
            }
         }
         break;

      default:
         {
            RLOG0(L_ERROR,"Default Switch Case");
      }
   }

   RETVALUE(ROK);
}

/** @brief This function is called to handle configuration confirm
 *         received from Physical layer. 
 *
 * @details
 *
 *     Function: wrUmmX2TgtHoPrcPhyCfgCfm
 *
 *         Processing steps:
 *         - Performs the validation of Config Confim whether 
 *           it is success or failure. 
 *
 *
 * @param WrUmmTransCb  *transCb 
 * @return ROK/RFAILED 
 */
PRIVATE S16 wrUmmX2TgtHoPrcPhyCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmPhyCfgCfm            *phyCfm = NULLP;
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &transCb->u.x2TgtHoTrans;

   /*Get the PHY CFM Message*/
   phyCfm = &transCb->msg->u.phyCfm;

   if (phyCfm->status != CTF_CFG_CFM_OK)
   {
      RLOG0(L_ERROR, "Configuration failure");
      x2TgtHoTrans->phyCfg = WR_UMM_TGT_HO_PHY_CFG_FAIL;
      /* Fix for ccpu00138199 */
      RETVALUE(RFAILED);
   }
   x2TgtHoTrans->phyCfg = WR_UMM_TGT_HO_PHY_CFG_SUCC;
   x2TgtHoTrans->state = WR_UMM_TGT_HO_PHY_CFG_DONE;

   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to handle scheduler confirm.
 *
 * @details
 *    Function: wrUmmX2TgtHoPrcSchCfm
 *
 *    Processing steps:
 *       -# Process scheduler confirm and set the states accordingly.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failed  : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoPrcSchCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2TgtHoTrans         *x2HoTrans = &(transCb->u.x2TgtHoTrans);
   U8                        idx;
    
   idx = transCb->msg->u.schdCfm.transId & 0x0f;
   if (idx > 15)
   {
      /*ccpu00127802*/
      RLOG_ARG0(L_DEBUG, DBG_TRNSID, transCb->transId,"Invalid transaction id"
         "for RAB Setup");
      RETVALUE(ROK);
   }
/*ccpu00131426 - Fix: verify if sch Cfm received for DRX */
   if ( idx == WR_UMM_TGT_HO_DRX_CFG )
   {
      if ( x2HoTrans->state == WR_UMM_TGT_HO_DRX_CFG_SENT )
      {
         if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
         {
            RLOG0(L_DEBUG, "=======DRX Cfg IS SuccessFull=====");
            x2HoTrans->state = WR_UMM_TGT_HO_DRX_CFG_DONE; 
            RETVALUE(ROK);
         }
         else
         {
            RLOG0(L_ERROR, "=======DRX Cfg IS FAILED=====");
            RETVALUE(RFAILED);
         }
      }
   }
   if (idx == WR_UMM_TGT_HO_SCH_UECFG)
   {
      if (transCb->msg->u.schdCfm.status != RGR_CFG_CFM_OK)
      {
         /*ccpu00127802*/
         RLOG0(L_ERROR, "UE Config Failed");
         RETVALUE(RFAILED);
      }
      /**
       * This is in response to UE reconfiguration for AMBR.
       */
      x2HoTrans->schdUeCfgCfmRcvd = TRUE;

      /**
       * if all the lch cfm's are setup prior to reception of ue cfg cfm, set 
       * state to success.
       */
      if((x2HoTrans->schdLchCfgCmpl == TRUE) &&
            (x2HoTrans->schdSrb1LchCfgCmpl == TRUE) &&
            (x2HoTrans->schdSrb2LchCfgCmpl == TRUE) &&
            (x2HoTrans->lcgCfgCmpl == TRUE))
      {
         x2HoTrans->state = WR_UMM_TGT_HO_SCH_CFG_DONE;
      }
      RETVALUE(ROK);
   }

   /**
    * check if the confirm is for SRB 1
    */
   if(idx == WR_UMM_TGT_HO_SCH_SRB1CFG)
   {
      if (transCb->msg->u.schdCfm.status != RGR_CFG_CFM_OK)
      {
         /*ccpu00127802*/
         RLOG0(L_ERROR, "SRB1 Config Failed");
         RETVALUE(RFAILED);
      }
      x2HoTrans->schdSrb1LchCfgCmpl = TRUE;

      /**
       * if all the lch cfm's are setup prior to reception of ue cfg cfm, 
       * set state to success
       */
      if((x2HoTrans->schdLchCfgCmpl == TRUE) && 
            (x2HoTrans->schdUeCfgCfmRcvd == TRUE) &&
            (x2HoTrans->schdSrb2LchCfgCmpl == TRUE) &&
            (x2HoTrans->lcgCfgCmpl == TRUE))
      {
         x2HoTrans->state = WR_UMM_TGT_HO_SCH_CFG_DONE;
      }
      RETVALUE(ROK);
   }

   /**
    * check if the confirm is for SRB 2
    */
   if(idx == WR_UMM_TGT_HO_SCH_SRB2CFG)
   {
      if (transCb->msg->u.schdCfm.status != RGR_CFG_CFM_OK)
      {
         /*ccpu00127802*/
         RLOG0(L_ERROR, "SRB2 Config Failed");
         RETVALUE(RFAILED);
      }
      x2HoTrans->schdSrb2LchCfgCmpl = TRUE;

      /**
       * if all the lch cfm's are setup prior to reception of ue cfg cfm, 
       * set state to success.
       */
      if((x2HoTrans->schdLchCfgCmpl == TRUE) && 
            (x2HoTrans->schdUeCfgCfmRcvd == TRUE) &&
            (x2HoTrans->schdSrb1LchCfgCmpl == TRUE) &&
            (x2HoTrans->lcgCfgCmpl == TRUE))
      {
         x2HoTrans->state = WR_UMM_TGT_HO_SCH_CFG_DONE;
      }
      RETVALUE(ROK);
   }

   /* ccpu00128203 */
   if((idx == WR_UMM_TRANS_ID_LCG0_CFG)||(idx == WR_UMM_TRANS_ID_LCG1_CFG)\
         ||(idx == WR_UMM_TRANS_ID_LCG2_CFG)||(idx == WR_UMM_TRANS_ID_LCG3_CFG))
   {
      if (transCb->msg->u.schdCfm.status != RGR_CFG_CFM_OK)
      {
         /*ccpu00127802*/
         RLOG0(L_ERROR, "LCH Group Config Failed");
         RETVALUE(RFAILED);
      }
      x2HoTrans->numlcgCfgCfm++;
      if(x2HoTrans->numlcgCfgCfm == x2HoTrans->expctLcgCfgCfm)
      {
         x2HoTrans->lcgCfgCmpl = TRUE;

         /**
          * if all the lch cfm's are setup prior to reception of ue cfg cfm, 
          * set state to success.
          */
         if((x2HoTrans->schdLchCfgCmpl == TRUE) && 
               (x2HoTrans->schdUeCfgCfmRcvd == TRUE) &&
               (x2HoTrans->schdSrb1LchCfgCmpl == TRUE) &&
               (x2HoTrans->schdSrb2LchCfgCmpl == TRUE))
         {
            x2HoTrans->state = WR_UMM_TGT_HO_SCH_CFG_DONE;
         }
      }
      RETVALUE(ROK);
   }

   if (idx > x2HoTrans->numOfRbCfgd)
   {
      /*ccpu00127802*/
      RLOG_ARG0(L_DEBUG, DBG_TRNSID, transCb->transId,"Invalid transaction id"
         " for RAB Setup");
      RETVALUE(ROK);
   }
   if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
   {
      x2HoTrans->rbInfo[idx].schdCfgState = WR_UMM_TGT_HO_RAB_SCH_CFG_SUCC;
      x2HoTrans->noOfAdmitdBearers++;
   }
   else
   {
      x2HoTrans->rbInfo[idx].schdCfgState = WR_UMM_TGT_HO_RAB_SCH_CFG_FAIL;
      /* Fix for ccpu00138196 */
      x2HoTrans->rbInfo[idx].rabAllowed = FALSE;
      RLOG1(L_ERROR, "SCH Config for DRB [%d] failed",
         x2HoTrans->rbInfo[idx].drbInfo.rbId);
   }
   for (idx = 0; idx < x2HoTrans->numOfRbCfgd; idx++)
   {
      if ((x2HoTrans->rbInfo[idx].schdCfgState == WR_UMM_TGT_HO_RAB_SCH_CFG_SENT)
            && (TRUE == x2HoTrans->rbInfo[idx].rabAllowed))
      {
         RETVALUE(ROK);
      }
   }
   /**
    * only when all the lch cfm's are success
    */
   x2HoTrans->schdLchCfgCmpl = TRUE;

   /**
    * only when UE Config is done, we are setting the state to SUCCESS
    */
   if((x2HoTrans->schdUeCfgCfmRcvd == TRUE) &&
         (x2HoTrans->schdSrb1LchCfgCmpl == TRUE) &&
         (x2HoTrans->schdSrb2LchCfgCmpl == TRUE) &&
         (x2HoTrans->lcgCfgCmpl == TRUE))
   {
      x2HoTrans->state = WR_UMM_TGT_HO_SCH_CFG_DONE;
   }

   RETVALUE (ROK);
}

/**
 * @brief 
 *    This function is used to handle RRC Config confirm.
 *
 * @details
 *    Function: wrUmmX2TgtHoPrcRrcCfgCfm
 *
 *    Processing steps:
 *       -# Process RRC Config confirm and set the states accordingly.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failed  : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoPrcRrcCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   NhuCfgCfmSdus             *rrcCfm;
   U8                        subTrans;

   rrcCfm   = msg->u.rrcCfm;
   subTrans = rrcCfm->hdr.transId & 0x0f;
   switch(subTrans)
   {
      case WR_UMM_TGT_HO_RRC_CFG:
      {
         if(rrcCfm->sdu.isCfgSuccess == FALSE)
         {
            RLOG0(L_ERROR, "DRB Config failed");
            RETVALUE(RFAILED);
         }
         else
         {
            transCb->u.x2TgtHoTrans.state   = WR_UMM_TGT_HO_RRC_CFG_DONE;
         }
         break;
      }

      case WR_UMM_TGT_HO_SN_CFG:
      {
         if(rrcCfm->sdu.isCfgSuccess == FALSE)
         {
            RLOG0(L_ERROR, "SN Config failed");
            /* TODO : update state */
         }
         else
         {
            RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,
               "SN Status has been updated");
            transCb->u.x2TgtHoTrans.state   = WR_UMM_TGT_HO_SN_CFG_DONE;
         }
         break;
      }
   }
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is used to commit UE Information.
 *
 * @details
 *    Function: wrUmmX2TgtHoCommitUeInfo
 *
 *    Processing steps:
 *       -# Update UE information.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failed  : RFAILED
 */

PRIVATE S16 wrUmmX2TgtHoCommitUeInfo
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2TgtHoTrans         *x2HoTrans = &(transCb->u.x2TgtHoTrans);
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmX2TgtHoRabInfo       *transRbInfo;
   WrUeRAB                   *ueRbInfo;
   U8                        idx;
   U8                        freeIdx;
   WrUmmCellCb               *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   /**
    * update bearer information
    */
   for(idx = 0; idx < x2HoTrans->numOfRbCfgd; idx++)
   {
      if(x2HoTrans->rbInfo[idx].rabAllowed)
      {
         transRbInfo = &x2HoTrans->rbInfo[idx];
         freeIdx = transRbInfo->drbInfo.eRABId - WR_UMM_ERAB_OFFSET;
         ueRbInfo = &ueCb->rbInfo.rab[freeIdx];

         ueRbInfo->inUse    = TRUE;
         ueRbInfo->eRABId   = transRbInfo->drbInfo.eRABId;
         ueRbInfo->rbId     = transRbInfo->drbInfo.rbId;
         wrUtlGetRbMode(ueCb->cellId, transRbInfo->drbInfo.qci, &(ueRbInfo->rbMode));
         wrUtlGetRbDir(ueCb->cellId, transRbInfo->drbInfo.qci, &(ueRbInfo->rbDir));
         ueRbInfo->lchId    = transRbInfo->drbInfo.lchId;
         ueRbInfo->qci      = transRbInfo->drbInfo.qci;
         if (WR_UMM_IS_GBR_BEARER(transRbInfo->drbInfo.qci))
         {
            /* Update new GBR value */
            ueCb->mappedLcgInfo[ueRbInfo->lcgId].effUlGbr += ueRbInfo->gbr.ul; 
            ueCb->mappedLcgInfo[ueRbInfo->lcgId].effUlMbr += ueRbInfo->mbr.ul; 
         }
         /*Converting back to kbps from kBps*/
         ueRbInfo->mbr.dl   = transRbInfo->drbInfo.mbr.dl * 8;
         ueRbInfo->mbr.ul   = transRbInfo->drbInfo.mbr.ul * 8;
         ueRbInfo->gbr.dl   = transRbInfo->drbInfo.gbr.dl * 8;
         ueRbInfo->gbr.ul   = transRbInfo->drbInfo.gbr.ul * 8;
         ueRbInfo->priority = transRbInfo->drbInfo.priority;
         ueRbInfo->preempCap = transRbInfo->drbInfo.preempCap;
         ueRbInfo->preempVul = transRbInfo->drbInfo.preempVul;
         ueRbInfo->isEmergencyBearer = transRbInfo->drbInfo.isEmergencyBearer;
         if(TRUE == ueRbInfo->isEmergencyBearer)
         {
            ueCb->ueServTyp = WR_UMM_EMERGENCY_SERV; 
         }
         /* SPS changes starts */
         ueRbInfo->isDlSpsEnabled = transRbInfo->isDlSpsEnabled;
         ueRbInfo->isUlSpsEnabled = transRbInfo->isUlSpsEnabled;
         /* SPS changes ends */
         if(((ueRbInfo->gbr.ul != 0) || (ueRbInfo->gbr.dl != 0)) && (ueRbInfo->qci <= 4))
            ueRbInfo->isGbrQosPres = TRUE;

         ueRbInfo->lcgId = wrUmmGetLcgIdForQci(ueCb->cellId,transRbInfo->drbInfo.qci);

        
         ueRbInfo->locEgtpTunId = transRbInfo->drbInfo.locEgtpTunId;
         ueRbInfo->remEgtpTunId = transRbInfo->drbInfo.remEgtpTunId;
         wrCpyCmTptAddr(&ueRbInfo->sgwAddr, &transRbInfo->drbInfo.sgwAddr);
      }
   }
   ueCb->rbInfo.numOfRbCfgd = x2HoTrans->noOfAdmitdBearers;

   ueCb->rbInfo.aggBr.dl = x2HoTrans->ambr.dl;
   ueCb->rbInfo.aggBr.ul = x2HoTrans->ambr.ul;

   /**
    * update security information
    */
   ueCb->secCfg = x2HoTrans->secCfg;
   /* ccpu00126532 */
#ifdef TENB_AS_SECURITY
   if(ueCb->hzSecCfg)
   {
      WR_FREE(ueCb->hzSecCfg, sizeof(WrUeSecCfg));
   }
   ueCb->hzSecCfg = x2HoTrans->hzSecCfg;
#endif

   /* update next hop chaining count information from source */
   ueCb->nhChainCnt = x2HoTrans->nextHopChainingCount;

   /* update ue capability information                                    */
   if(x2HoTrans->ueCap != NULLP)
   {
      ueCb->ueCap = x2HoTrans->ueCap;
      ueCb->ueCat = ueCb->ueCap->eUTRA_Cap.ue_Category.val;
      /* RRC-REL10-Upgrade */
      wrUtlUpdateUeCatV1020(ueCb, ueCb->ueCap);
      x2HoTrans->ueCap = NULLP;
   }

   /**
    * update gummei information
    */
   WR_MEM_COPY(&ueCb->gummei, &x2HoTrans->gummei, sizeof(WrGummei));
   WR_MEM_COPY(&ueCb->srcGummei, &x2HoTrans->gummei, sizeof(WrGummei));

   /**
    * update mmeId
    */
   ueCb->mmeId = x2HoTrans->mmeId;

   /**
    * update plmnId
    */
   wrUtlCopyPlmnId(&x2HoTrans->plmnId, &ueCb->plmnId);

   /**
    * Update Transmission Mode to TM3 in case MIMO is suppoerted
    */
   if(wrEmmCb.numTxAntenna > 1)
   {
      ueCb->antennaInfo.txmode = ueCb->antennaInfo.txmode;
   }
   /* update mme id */
   ueCb->s1ConCb->mme_ue_s1ap_id = x2HoTrans->mme_ue_s1ap_id;
   if(x2HoTrans->mmeUeS1apId2.pres == TRUE)
   {
      wrFillTknU32(&ueCb->s1ConCb->mmeUeS1apId2, x2HoTrans->mmeUeS1apId2.val);
   }

   /* update x2 ue info */
   ueCb->x2ConCb.peerId = x2HoTrans->peerId;
   ueCb->x2ConCb.newEnbUeX2apId = x2HoTrans->new_eNB_UE_X2ap_Id;
   ueCb->x2ConCb.oldEnbUeX2apId = x2HoTrans->old_eNB_UE_X2ap_Id;

   RETVALUE(RFAILED);
}
/**
 * @brief 
 *    This function is used to process Uu PDU.
 *
 * @details
 *    Function: wrUmmX2TgtHoPrcUuPdu
 *
 *    Processing steps:
 *       -# Based on message type, trigger corresponding message handler.
 *       -# if message type is RRCCONRECFGNCOMPL, process RRC Connection
 *          Reconfiguration Complete.       
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmX2TgtHoPrcUuPdu
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   NhuDatIndSdus             *uuIndPdu;
   NhuUL_DCCH_MsgTyp         *ulDcchMsg;
   NhuUL_DCCH_MsgTypc1       *rrcMsg;
   NhuUECapInfo              *ueCapInfo;
   WrUeCb                    *ueCb = transCb->ueCb;
   U32                       rrcTransId = 0;
  
   uuIndPdu = msg->u.uuIndPdu;
   ulDcchMsg = &(uuIndPdu->sdu.m.ulDcchMsg.dcchMsg.message);
   rrcMsg = &(ulDcchMsg->val.c1);
   switch (rrcMsg->choice.val)
   {
      case C1_RRCCONRECFGNCOMPL:
      {
         RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,
            "Received RRC Reconfiguration Complete");
         if (ROK != wrUmmX2TgtHoHdlHoCompletion(transCb))
         {
            RLOG0(L_ERROR, "Handling of RRC Reconfiguration Complete failed");
            RETVALUE(RFAILED);
         }/* End of wrUmmX2TgtHoRrcConReConfgComplete.*/
      }
      break;
      case C1_UECAPINFO:
      {
         if((ueCb->ueCap != NULLP) && ( NOTPRSNT != ueCb->ueCap->eUTRA_Cap.pres.pres)&& (NOTPRSNT != ueCb->ueCap->eUTRA_Cap.accessStratumRls.pres))
         {   
            RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[OTA] Received UECapabilityInformation"
                  " ue-Category [%d] accessStratumRelease [%u]", ueCb->ueCat, ueCb->ueCap->eUTRA_Cap.accessStratumRls.val);
         }
         else
         {
            RLOG_ARG1(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[OTA] Received UECapabilityInformation"
                  " ue-Category [%d] ", ueCb->ueCat);
         }
         ueCapInfo = &(rrcMsg->val.ueCapInfo);
         rrcTransId = ueCapInfo->rrc_TrnsxnId.val;
         if (wrUmmRelRrcTransId(transCb->ueCb, C1_UECAPENQUIRY,
                  rrcTransId) != ROK)
         {
            RLOG0(L_ERROR,"Release of transaction ID failed");
         }
         /*Stop Radio Capability Enquiry  Timer*/
         wrStopTransTmr(transCb, WR_TMR_RRC_CAPENQ);
         /* eCSFB:- Set the transCb state, as unable to store
          * ueCapInfo due to mem allocationfailue*/
         if(ROK != wrUmmHoUeCapStoreCapInfo(transCb->ueCb , ueCapInfo))
         {
            RETVALUE(RFAILED);
         }
         wrUmmSndUeCapEncReq(ueCb, 
               transCb->transId | WR_UMM_TGT_HO_UECAP_ENC, ueCapInfo);
      }
      break;
      default:
      {
         RLOG1(L_ERROR, "unknown msg recved msgId [%d]", rrcMsg->choice.val);
         RETVALUE(RFAILED);
         break;
      }
   }
   RETVALUE(ROK);
}
/* ccpu00126532 */
#ifdef TENB_AS_SECURITY
PRIVATE S16 wrUmmX2TgtHoHdlKdfResp
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmIncMsg               *msg = transCb->msg;
   WrUmmX2TgtHoTrans         *x2HoTrans = &(transCb->u.x2TgtHoTrans);
   WrUmmKdfCfm               *kdfCfm;
   CtfAskeyCfmInfo           *kEnbInf;
   WrUmmCellCb               *cellCb;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if (NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId,"Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   kdfCfm = &(msg->u.kdfCfm);

   RLOG0(L_DEBUG, "****************************");
   RLOG0(L_DEBUG, "AS KEY GENERATION SUCESS");
   RLOG0(L_DEBUG, "****************************");
   if(kdfCfm->ctfKdfCfm->keyDerType == CTF_KDF_TYPE1)
   {
      kEnbInf = &(kdfCfm->ctfKdfCfm->u.kEnbInf);
      if(x2HoTrans->asKeyGenerated == FALSE)
      {
         /*Store initial AS security keys*/
         cmMemcpy(x2HoTrans->secCfg->cpCiphKey, kEnbInf->cpCiphKey,
               WR_CIPH_KEY_LEN);
         cmMemcpy(x2HoTrans->secCfg->upCiphKey, kEnbInf->upCiphKey,
               WR_CIPH_KEY_LEN);
         cmMemcpy(x2HoTrans->secCfg->intgKey, kEnbInf->intKey,
               WR_INTG_KEY_LEN);
         /*ccpu00129252*/
         if(transCb->u.x2TgtHoTrans.state == WR_UMM_TGT_HO_SCH_CFG_DONE)
         {
            RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,
               "DBG :: RRC Configuration with SecKeys");
            if (ROK != wrUmmX2TgtHoRrcCfg(transCb))
            {
               RLOG0(L_ERROR, "Failed to Configure RRC.");
               RETVALUE(RFAILED);
            }
         }
         x2HoTrans->asKeyGenerated = TRUE;      
      }
      else
      {
         /*store horizontally derived AS security keys*/
         cmMemcpy(x2HoTrans->hzSecCfg->cpCiphKey, kEnbInf->cpCiphKey,
               WR_CIPH_KEY_LEN);
         cmMemcpy(x2HoTrans->hzSecCfg->upCiphKey, kEnbInf->upCiphKey,
               WR_CIPH_KEY_LEN);
         cmMemcpy(x2HoTrans->hzSecCfg->intgKey, kEnbInf->intKey,
               WR_INTG_KEY_LEN);

         /*DRX QCI*/                 
         if((ueCb->isGBRBearerExists == NOT_IN_USE))
         {
            RLOG0(L_INFO,
              "GBR bearer is not exists After Hand-in");
            if(wrUmmStartAnr(transCb->ueCb,FALSE) != ROK)
            {
               RLOG0(L_ERROR,"Failed to Start the ANR");
            }
         }           
         /* ccpu00135331*/
         if ( x2HoTrans->state == WR_UMM_TGT_HO_PATH_SWITCH_ACK_RCVD )
         {
            x2HoTrans->state = WR_UMM_TGT_HO_HZ_KEY_DER_DONE;
         }
         if( ( FALSE == x2HoTrans->handInUeCdma ) && 
             ( 0 == x2HoTrans->rbInfo->expectedTnlRsp ))
         {
            wrUmmTransComplete(transCb);
         }
      }
   }
   else if(kdfCfm->ctfKdfCfm->keyDerType == CTF_KDF_TYPE2)
   {
      WR_ALLOC(&(x2HoTrans->hzSecCfg), sizeof(WrUeSecCfg));
      if (x2HoTrans->hzSecCfg == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      /* Copy the security capabilities and the selected algorithm */
      x2HoTrans->hzSecCfg->ciphAlgo = x2HoTrans->secCfg->ciphAlgo;
      x2HoTrans->hzSecCfg->intgAlgo = x2HoTrans->secCfg->intgAlgo;
      x2HoTrans->hzSecCfg->encrypCap = x2HoTrans->secCfg->encrypCap;
      x2HoTrans->hzSecCfg->integCap = x2HoTrans->secCfg->integCap;

      cmMemcpy(x2HoTrans->hzSecCfg->kENB, kdfCfm->ctfKdfCfm->u.kEnbStarInf.secKey,
            WR_KENB_KEY_LEN);
      transCb->ueCb->hzSecCfg = x2HoTrans->hzSecCfg;
#if  !(defined SS_RBUF) && !(defined LTE_QCOM)
      wrPlatSecExtractSecKeys1(ueCb, transCb->transId, CTF_KDF_TYPE1);
#else
      wrUmmSecExtractSecKeys1(ueCb, transCb->transId, CTF_KDF_TYPE1);
#endif
   }
   RETVALUE(ROK);
}
#endif

PRIVATE S16 wrUmmX2TgtHoPrepTmrExp
(
WrUmmTransCb                 *transCb
)
{
   RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"Timer Expired. "
      "Handover Preparation Failed");
   transCb->u.x2TgtHoTrans.hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW;
   transCb->u.x2TgtHoTrans.hoPrepFailCause.causeVal = 
      CztCauseRadioNwunspecifiedEnum;
   if(ROK != wrUmmX2TgtHoSndPrepFailure(transCb))
   {
       RLOG0(L_ERROR,"Failed To Send Handover Failure");
   }
   transCb->ueCb->mobCtrlState = WR_HO_UE_RELEASE;
   /* Release transaction immediately after failure */
   transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
   
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmX2TgtHoPrcTmrExpiry
(
WrUmmTransCb                 *transCb
)
{
   if(transCb->msg->u.tmrExpiry.timerEvnt == WR_TMR_TGT_HOPREP)
   {
      wrUmmX2TgtHoPrepTmrExp(transCb);
   }
   else if(transCb->msg->u.tmrExpiry.timerEvnt == WR_TMR_TGT_PATH_SWITCH)
   {
     /* ccpu00148952 */
      RLOG_ARG0(L_DEBUG,DBG_CRNTI,transCb->ueCb->crnti,
                         "WR_TMR_TGT_PATH_SWITCH timer expired");
     /* ccpu00148952 */
      wrUmmPrcLclCtxtRel(transCb->ueCb,NhuRlsCauseotherEnum);
   }
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmX2TgtHoProcHoAdmitReq
(
WrUmmTransCb                 *transCb
)
{
   RmuUeHoReq            *ueHoReq;
   WrUmmX2TgtHoRabInfo   *x2rbInfo = NULLP;
   U8                     erabIdx;
   /* SPS changes starts */
   WrUeCapInfo           *ueCap = transCb->u.x2TgtHoTrans.ueCap;
   /* SPS changes ends */

   WR_ALLOC(&ueHoReq, sizeof(RmuCommonMsg));
   if (ueHoReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   ueHoReq->bCellId = transCb->ueCb->cellId;
   ueHoReq->usCrnti = transCb->ueCb->crnti;
   ueHoReq->uiTransId = transCb->transId;

   /* SPS changes starts */
   if (ueCap->eUTRA_Cap.featureGroupIndicators.pres == PRSNT_NODEF)
   {
      /* TODO: SPS Ask APP team why this is done by checking feature indicator presence */
      ueHoReq->stUeCapInfo.stEutraCapInfo.uiUeCategory = ueCap->eUTRA_Cap.ue_Category.val;
   }
   wrUmmRrmFillUeCapInfo(transCb->ueCb->cellId, &ueHoReq->stUeCapInfo, ueCap);
   /* SPS changes ends */

   x2rbInfo = transCb->u.x2TgtHoTrans.rbInfo;
   ueHoReq->stErabConfInfo.stUeAmbrInfo.uiAggUEMaxBitRateDL = 
                                       transCb->u.x2TgtHoTrans.ambr.dl;
   ueHoReq->stErabConfInfo.stUeAmbrInfo.uiAggUEMaxBitRateUL = 
                                       transCb->u.x2TgtHoTrans.ambr.ul;
   ueHoReq->stErabConfInfo.usNoErabsToAddMod = 
                                   transCb->u.x2TgtHoTrans.numOfRbCfgd;

    for (erabIdx =0; erabIdx < transCb->u.x2TgtHoTrans.numOfRbCfgd; erabIdx++)
    {
       ueHoReq->stErabConfInfo.stErabAddModLst[erabIdx].bErabId
                                                      = x2rbInfo[erabIdx].drbInfo.eRABId;
       ueHoReq->stErabConfInfo.stErabAddModLst[erabIdx].stErabQosInfo.bErabQCI
                                                      = x2rbInfo[erabIdx].drbInfo.qci;
       ueHoReq->stErabConfInfo.stErabAddModLst[erabIdx].stErabQosInfo.stErabGbrInfo.uiErabDlMbr
                                                      = x2rbInfo[erabIdx].drbInfo.mbr.dl;
       ueHoReq->stErabConfInfo.stErabAddModLst[erabIdx].stErabQosInfo.stErabGbrInfo.uiErabUlMbr
                                                      = x2rbInfo[erabIdx].drbInfo.mbr.ul;
       ueHoReq->stErabConfInfo.stErabAddModLst[erabIdx].stErabQosInfo.stErabGbrInfo.uiErabDlGbr
                                                      = x2rbInfo[erabIdx].drbInfo.gbr.dl;
       ueHoReq->stErabConfInfo.stErabAddModLst[erabIdx].stErabQosInfo.stErabGbrInfo.uiErabUlGbr
                                                      = x2rbInfo[erabIdx].drbInfo.gbr.ul;
       /*RRM_ARP*/
       ueHoReq->stErabConfInfo.stErabAddModLst[erabIdx].stErabQosInfo.stErabArpInfo.bErabPrioLvl
          = x2rbInfo[erabIdx].drbInfo.priority; 
       ueHoReq->stErabConfInfo.stErabAddModLst[erabIdx].stErabQosInfo.stErabArpInfo.bErabPreemptCap
          = x2rbInfo[erabIdx].drbInfo.preempCap;
       ueHoReq->stErabConfInfo.stErabAddModLst[erabIdx].stErabQosInfo.stErabArpInfo.bErabPreemptVul
          = x2rbInfo[erabIdx].drbInfo.preempVul;

    }

    ueHoReq->stErabConfInfo.bIsUeAmbrPres = TRUE;

    wrUmmHoCopyRadioResCfg(&ueHoReq->stUeRadioResCfg, 
                           &transCb->u.x2TgtHoTrans.ueRadioResCfg);

   if(transCb->ueCb->ueServTyp == WR_UMM_EMERGENCY_SERV)
   {
      ueHoReq->isEmrgncyBrersPsnt = TRUE;
      ueHoReq->isMember = TRUE;
   }
   else
   {
      ueHoReq->isEmrgncyBrersPsnt = FALSE;
      if(WR_UMM_CSG_MEMBER == transCb->u.x2TgtHoTrans.ueMbrShpSts)
      {
         ueHoReq->isMember = TRUE;
      }
      else
      {
         ueHoReq->isMember = FALSE;
      }
   }

   if (ROK != wrIfmRrmHoAdmitReq(ueHoReq))
   {
      /**
       * New User is not allowed
       */
      transCb->u.x2TgtHoTrans.hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW;
      transCb->u.x2TgtHoTrans.hoPrepFailCause.causeVal = 
           CztCauseRadioNwno_radio_resources_available_in_target_cellEnum;
      if(ROK != wrUmmX2TgtHoSndPrepFailure(transCb))
      {
          RLOG0(L_ERROR,"Failed To Send Handover Failure");
      }
      transCb->state = WR_UMM_TRANS_DELETE_UE;
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 * @brief 
 *    This function is called when any message related to a particular X2 HO is 
 *    received. This is the main function that determines next course of action 
 *    based on the message received.
 *
 * @details
 *    Function: wrUmmX2TgtHoProcessor
 *
 *    Processing steps:
 *       -# If Processor receives any message, it shall trigger
 *          corresponding message handler based on message type.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmX2TgtHoProcessor
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   RmuUeHoRsp                *hoAdmitRspInfo = NULLP;
   WrUmmX2TgtHoTrans         *x2TgtHoTrans = &(transCb->u.x2TgtHoTrans);
   /* SPS changes starts */
   NhuHovrPrepInfo           *nhuHovrPrepInfo;
   S16                       ret                = ROK;
   /* SPS changes ends */
   WrUmmMsgCause             *cause             = &(transCb->u.x2TgtHoTrans.hoPrepFailCause);
   U8                             idx = 0; /*RRM_ARP*/
   U8                             idx1 = 0; /*RRM_ARP*/

   /*Default cause type and value*/
   cause->causeTyp = WR_CAUSE_RADIONW;
   cause->causeVal = CztCauseRadioNwunspecifiedEnum; 

   /**
    * Based on the message type, process the appropriate msg
    */
   switch(msg->msgTyp)
   { 
      case WR_UMM_X2_BMP_PDU:
         {
            /**
             * Since the Processor is received X2AP PDU message, Process 
             * the X2AP msg handler. 
             */
            if (ROK != wrUmmX2TgtHoHdlX2apPdu(transCb))
            {
               RLOG0(L_ERROR,"Failed to handle X2AP PDU.");

               RETVALUE(RFAILED);
            }/* End of wrUmmX2TgtHoHdlX2apPdu*/ 
            break;
         }
      case WR_UMM_RRM_UEHO_RESP:
         {
            /* ccpu00131426 - Fix: Preserve the RRM response, 
             * which will be required to decide DRX cfg for the UE */
            transCb->u.x2TgtHoTrans.rrmUeRecfgRspMsg = transCb->msg;
            transCb->msg->refCnt++;

            hoAdmitRspInfo = msg->u.rrmUeHoResp;
            /*DRX QCI*/
            if(hoAdmitRspInfo->isGBRBearerExists == TRUE)
            {
               transCb->ueCb->isGBRBearerExists = RAB_ESTABLISHED; 
            }

            if (hoAdmitRspInfo->stStatusInfo.enStatus == RMU_FAILURE)
            {
               /**
                * New User is not allowed
                */
               /*ccpu00130428*/
               cause->causeTyp = WR_CAUSE_RADIONW;
               cause->causeVal = CztCauseRadioNwho_target_not_allowedEnum;
               if(ROK != wrUmmX2TgtHoSndPrepFailure(transCb))
               {
                  RLOG0(L_ERROR,"Failed To Send Handover Failure");
               }
               transCb->state = WR_UMM_TRANS_DELETE_UE;
               RETVALUE(RFAILED);
            }
            /*RRM_ARP : Copy ERABs rejected list to transCb from hoAdmitRspInfo*/
           if(hoAdmitRspInfo->bNoErabsRejected)
           {
              U8          numOfRbCfgd = transCb->u.x2TgtHoTrans.numOfRbCfgd;
              U32         rrmCause;

              transCb->u.x2TgtHoTrans.erabsToRejectLst.numrabs = hoAdmitRspInfo->bNoErabsRejected;

              for(idx = 0;idx < hoAdmitRspInfo->bNoErabsRejected; idx++)
              {
                 transCb->u.x2TgtHoTrans.erabsToRejectLst.erabId[idx] = 
                    hoAdmitRspInfo->stErabRejectLst[idx].bErabId;

                 rrmCause = (U32)hoAdmitRspInfo->stErabRejectLst[idx].enCause;

                 /* RRM causes are aligned with S1AP. Hence, mapping to X2AP*/
                 WR_ALIGN_RRM_CAUSE_TO_X2AP(rrmCause);

                 transCb->u.x2TgtHoTrans.erabsToRejectLst.cause[idx] = rrmCause; 
              }

              transCb->u.x2TgtHoTrans.erabsToRejectLst.causeTyp = WR_CAUSE_RADIONW;

              /*Mark the unallowed ERABs so that they will not be confiured by
               * lower layers*/
              for(idx = 0;idx < numOfRbCfgd;idx++)
              {
                if(transCb->u.x2TgtHoTrans.rbInfo[idx].drbInfo.eRABId == 
                      hoAdmitRspInfo->stErabRejectLst[idx1].bErabId)
                {
                   transCb->u.x2TgtHoTrans.rbInfo[idx].rabAllowed = FALSE;
                   idx1++;
                }
              }
           }

            wrStartTransTmr(transCb, WR_TMR_TGT_HOPREP, WR_TMR_TGT_HOPREP_VAL);
            /* ccpu00131426 - Fix */

            if (ROK != wrUtlHoRscAlloc(transCb))
            {
               RLOG0(L_ERROR,"Failed to process wrUtlHoRscAlloc");
               cause->causeTyp = WR_CAUSE_RADIONW;
               cause->causeVal = 
                  CztCauseRadioNwno_radio_resources_available_in_target_cellEnum;
               wrUmmX2TgtHoSndPrepFailure(transCb);
               transCb->state = WR_UMM_TRANS_DELETE_UE;
               RETVALUE(RFAILED);
            }
            transCb->ueCb->hoType    = WR_HO_TYPE_X2;

            transCb->ueCb->hoTransId = transCb->transId;
            /*FIX for ccpu00127073*/
            /**
             * Generate new ue x2ap id here instead of sndHoReqAck Msg.
             */
            x2TgtHoTrans->new_eNB_UE_X2ap_Id = transCb->ueCb->cellId;
            x2TgtHoTrans->new_eNB_UE_X2ap_Id <<= 8;
            x2TgtHoTrans->new_eNB_UE_X2ap_Id |= transCb->ueCb->ueIdx;

            /* Store both the old_x2ap_id & new_x2ap_id for later retrieval */
            transCb->ueCb->x2ConCb.peerId = x2TgtHoTrans->peerId;
            transCb->ueCb->x2ConCb.oldEnbUeX2apId = 
               x2TgtHoTrans->old_eNB_UE_X2ap_Id; 
            transCb->ueCb->x2ConCb.newEnbUeX2apId = 
               x2TgtHoTrans->new_eNB_UE_X2ap_Id;
            /*FIX for ccpu00127073*/

            break;
         }
      case WR_UMM_SCHD_CFG:
         {
            /**
             * Process Scheduler confirm message.
             */
            if(ROK != wrUmmX2TgtHoPrcSchCfm(transCb))
            {
          RLOG0(L_ERROR,"Failed To Send Handover Failure");
               cause->causeTyp = WR_CAUSE_RADIONW;
               cause->causeVal =
                  CztCauseRadioNwno_radio_resources_available_in_target_cellEnum;
               if(ROK != wrUmmX2TgtHoSndPrepFailure(transCb))
               {
             RLOG0(L_ERROR,"Failed To Send Handover Failure");
               }
               transCb->state = WR_UMM_TRANS_RELEASE_CALL;
               RETVALUE(RFAILED);
            }
            /**
             * If application received confirmation successfully, 
             * application shall configure RRC.
             */
            /* ccpu00130424*/
            if(transCb->u.x2TgtHoTrans.state == WR_UMM_TGT_HO_SCH_CFG_DONE 
#ifdef TENB_AS_SECURITY
                  &&   transCb->u.x2TgtHoTrans.asKeyGenerated == TRUE
#endif            
              )
            {
               RLOG_ARG0(L_DEBUG,DBG_CRNTI,transCb->ueCb->crnti,
                         "Scheduler Configuration done");
               if (ROK != wrUmmX2TgtHoRrcCfg(transCb))
               {
                  RLOG0(L_ERROR,"Failed to Configure RRC.");
                  RETVALUE(RFAILED);
               }
            }
            break;
         }
      case WR_UMM_S1_CON_CFM:
         {
            /**
             * The processor is received S1AP PDU message.So,
             * it will trigger S1AP msg handler
             */
            if (ROK != wrUmmX2TgtHoHdlS1apPdu(transCb))
            {
                RLOG0(L_ERROR,"Failed to handle S1AP PDU.");
               /* TODO : Send preparation failure */
            }/* End of wrUmmX2TgtHoHdlS1apPdu*/ 
            break;
         }
      case WR_UMM_RRC_CFG:
         {
            /**
             * Process RRC Config confirm.
             */
            if(ROK != wrUmmX2TgtHoPrcRrcCfgCfm(transCb))
            {
               RLOG0(L_ERROR,"RRC CFG CFM Is Failed");
               cause->causeTyp = WR_CAUSE_RADIONW;
               cause->causeVal =
                  CztCauseRadioNwno_radio_resources_available_in_target_cellEnum;
               if(ROK != wrUmmX2TgtHoSndPrepFailure(transCb))
               {
                  RLOG0(L_ERROR,"Failed To Send Handover Failure");
               }
               transCb->state = WR_UMM_TRANS_RELEASE_CALL;
               RETVALUE(RFAILED);
            }
            if(transCb->u.x2TgtHoTrans.state == WR_UMM_TGT_HO_RRC_CFG_DONE)
            {
               RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,
                     "RRC layer reconfiguration completed");
               /**
                * Tunnel Configuration.
                */
               wrUmmX2TgtHoCfgTnls(transCb);
            }
            break;
         }/* End of WR_UMM_HO_LI_CFG_CFM*/
      case WR_UMM_DAM_CFM:
         {
            /**
             * Process DAM confirm.
             */
            wrUmmX2TgtHoPrcDamCfm(transCb);
/* Fix for ccpu00138191 */
            if (transCb->u.x2TgtHoTrans.state == WR_UMM_TGT_HO_TNL_CFG_DONE)
            {
               RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,
                     "Tunnel configuration completed");
               /**
                * Update ueCb
                */
               wrUmmX2TgtHoCommitUeInfo(transCb);

               //wrEmmAnrUeConnected(transCb->ueCb->cellId);

               /**
                * Prepare RRC Connection Reconfiguration with Mobility Info.
                */
               if (ROK != wrUmmX2TgtHoPrepRrcDlDcchMsg(transCb))
               {
                  RLOG0(L_ERROR,"Failed to Prepare HO Command");
                  if (ROK != wrUmmX2TgtHoSndPrepFailure(transCb))
                  {
                      RLOG0(L_ERROR,"Failed To Send Handover Failure");
                  }
                  transCb->state = WR_UMM_TRANS_RELEASE_CALL;
                  RETVALUE(RFAILED);
               }
            }
            break;
         }
         /* ccpu00126336 */
            RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,
               "Tunnel configuration completed");
      case WR_UMM_DAM_MOD_CFM:
         {
            /**
             * Process DAM Modification confirm.
             */
            wrUmmX2TgtHoPrcDamModCfm(transCb);
            break;
         }
      case WR_UMM_RRC_ENC_CFM:
         {
            /**
             * Process RRC Encode Confirm.
             */
            if(ROK != wrUmmX2TgtHoPrcRrcEncCfm(transCb))
            {
               RLOG0(L_ERROR,"Rrc Enc Cfm is Failed");
               if(ROK != wrUmmX2TgtHoSndPrepFailure(transCb))
               {
                  RLOG0(L_ERROR,"Failed To Send Handover Failure");
               }
               transCb->state = WR_UMM_TRANS_RELEASE_CALL;
               RETVALUE(RFAILED);
            }
            break;
         }
      case WR_UMM_RRC_DEC_CFM:
         {
            /**
             * Process RRC Decode Confirm.
             */
            if(FALSE == transCb->u.x2TgtHoTrans.handInUeCdma)
            {
               ret = wrUmmHoTgtPrcRrcDecCfm(transCb, &transCb->u.x2TgtHoTrans.ueCap);
               
               /* SPS changes starts */
               if ((ret != ROK) && (ret != WR_UMM_NHU_MSG_UECAPINFO_V9A0_DEC_SENT))
               /* SPS changes ends */
               {
                  cause->causeTyp = WR_CAUSE_PROTOCOL;
                  cause->causeVal = CztCauseProttransfer_syntax_errorEnum;
                  if(ROK != wrUmmX2TgtHoSndPrepFailure(transCb))
                  {
                     RLOG0(L_ERROR,"Faled To Send Handover Failure");
                  }
                  transCb->state  = WR_UMM_TRANS_DELETE_UE;
                  RETVALUE(RFAILED);
               }

               /* SPS changes starts */
               if ((NHU_DEC_SDUTYPE_EUTRA_UECAP_IE == transCb->msg->u.decCfm->sduType) && \
                     (ret != WR_UMM_NHU_MSG_UECAPINFO_V9A0_DEC_SENT))
               /* SPS changes ends */
               { 
                  /*ccpu00129650*/

                  if(transCb->u.x2TgtHoTrans.ueCap != NULLP)
                  {   
                     transCb->ueCb->ueCat = 
                        transCb->u.x2TgtHoTrans.ueCap->eUTRA_Cap.ue_Category.val;
                     /* RRC-REL10-Upgrade */
                     wrUtlUpdateUeCatV1020(transCb->ueCb, transCb->u.x2TgtHoTrans.ueCap);
                  }
                  wrUmmX2TgtHoProcHoAdmitReq(transCb);
               }
               else if (NHU_MSG_HOPREPINFO == transCb->msg->u.decCfm->sduType)
               {
                  /* SPS changes starts */
                  nhuHovrPrepInfo = &transCb->msg->u.decCfm->ie.nhuHovrPrepInfo;
                  wrUmmHoFillRadioResCfg(&transCb->u.x2TgtHoTrans.ueRadioResCfg, nhuHovrPrepInfo);
                  /* SPS changes ends */
                  /* Get Lch Id and Drb Id */
                  if (ROK != wrUmmX2TgtHoGetLchDrbIds(transCb))
                  {
                     /* cause - no radio resource available */
                     cause->causeTyp = WR_CAUSE_PROTOCOL;
                     cause->causeVal = CztCauseProtsemantic_errorEnum;
                     if(ROK != wrUmmX2TgtHoSndPrepFailure(transCb))
                     {
                        RLOG0(L_ERROR,"Failed To Send Handover Failure");
                     }
                     transCb->state  = WR_UMM_TRANS_DELETE_UE;
                     RETVALUE(RFAILED);
                  }
                  /* SPS changes starts */
                  transCb->u.x2TgtHoTrans.srcToTgtSpsInfo.isDlSpsPresent = FALSE;
                  transCb->u.x2TgtHoTrans.srcToTgtSpsInfo.isUlSpsPresent = FALSE;
                  if ((nhuHovrPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.\
                           as_Config.pres.pres == PRSNT_NODEF) && \
                        (nhuHovrPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.as_Config.\
                         sourceRadioResourceConfig.pres.pres == PRSNT_NODEF) && \
                        (nhuHovrPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.as_Config.\
                         sourceRadioResourceConfig.sps_Config.pres.pres == PRSNT_NODEF))
                  {

                     if (nhuHovrPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.as_Config.\
                           sourceRadioResourceConfig.sps_Config.sps_ConfigDL.choice.val == 1)
                     {
                        transCb->u.x2TgtHoTrans.srcToTgtSpsInfo.isDlSpsPresent = TRUE;
                     }
                     if (nhuHovrPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.as_Config.\
                           sourceRadioResourceConfig.sps_Config.sps_ConfigUL.choice.val == 1)
                     {
                        transCb->u.x2TgtHoTrans.srcToTgtSpsInfo.isUlSpsPresent = TRUE;
                     }
                  }
                  /* SPS changes ends */
               }
               /* SPS changes starts */
               else if (NHU_MSG_UECAPINFO_V9A0 == transCb->msg->u.decCfm->sduType)
               { 
                  if(transCb->u.x2TgtHoTrans.ueCap != NULLP)
                  {   
                     transCb->ueCb->ueCat = 
                        transCb->u.x2TgtHoTrans.ueCap->eUTRA_Cap.ue_Category.val;
                     /* RRC-REL10-Upgrade */
                     wrUtlUpdateUeCatV1020(transCb->ueCb, transCb->u.x2TgtHoTrans.ueCap);
                  }
                  wrUmmX2TgtHoProcHoAdmitReq(transCb);
               }
               /* SPS changes ends */
            }
            else
            {
               ret = wrUmmHoTgtPrcRrcDecCfm(transCb, &transCb->ueCb->ueCap);
               /* close the transaction after getting the decode confirm for 
                * NHU_MSG_UECAPINFO_V9A0 IE */
               if( ret != WR_UMM_NHU_MSG_UECAPINFO_V9A0_DEC_SENT)
               {
                  transCb->u.x2TgtHoTrans.handInUeCdma = FALSE;
                  wrUmmTransComplete(transCb);
               }
            }

            break;
         }
      case WR_UMM_UU_IND_PDU:
         {
            /**
             * Process Uu PDU indication.
             */
            wrUmmX2TgtHoPrcUuPdu(transCb);
         }
         break;
/* Fix for ccpu00138191 */
      case WR_UMM_PHY_CFG: 
         {
            if(ROK != wrUmmX2TgtHoPrcPhyCfgCfm(transCb))
            {
               cause->causeTyp = WR_CAUSE_RADIONW;
               cause->causeVal = 
                  CztCauseRadioNwno_radio_resources_available_in_target_cellEnum;
               if(ROK != wrUmmX2TgtHoSndPrepFailure(transCb))
               {
                  RLOG0(L_ERROR,"wrUmmX2TgtHoProcessor: Failed To Send Handover Failure");
               }
               transCb->state  = WR_UMM_TRANS_RELEASE_CALL;
               RETVALUE(RFAILED);
            }
            break;
         }
      case WR_UMM_TMR_EXPIRY:
         {
            wrUmmX2TgtHoPrcTmrExpiry(transCb);
            break;
         }
         /* ccpu00126532 */
#ifdef TENB_AS_SECURITY
      case WR_UMM_PHY_KDF:
         {
            wrUmmX2TgtHoHdlKdfResp(transCb);
            break;
         }
#endif
      default :
         {
            RETVALUE(RFAILED);
         }

   } /* end of switch statemen */

   RETVALUE(ROK);
}
/* HO_REEST */

/** @brief This function is called when a reestablishment message is received during
 *         X2 Target Handover transaction .
 *
 * @details
 *
 *     Function: wrUmmX2TgtHoNewTransProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  inctransCb : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmX2TgtHoNewTransProcessor
(
 WrUmmTransCb                 *transCb,
 WrUmmTransCb                 *incTrans
 )
{
   WrRrcReEstabTransCb       *reestabIncTrans; 
   /* if the incoming transaction is the REESTABLISHMENT TRANSACTION
    * if the HO Transaction state is HO_IN_PROGRESS i.e. UE Configuration
    * is successful at this cell and HO Command has been sent to Source eNodeB
    * else Reject the RRC Reestablishment and Abort the HO Transaction
    */
   switch(incTrans->transTyp)
   {
      case WR_UMM_RRC_REESTAB_TRANS:
      {
         reestabIncTrans = &(incTrans->u.rrcReEstabTransCb);
         if(transCb->ueCb->mobCtrlState != WR_HO_IN_PROGRESS)
         {
            /*  
             * Defer the HO Transaction
             * and continue processing REESTABLISHMENT Transaction
             * once RRC Reconfiguration Complete Received
             * start processing the HO Transaction i.e. send HO Notify and 
             * rest of the things
             * if REESTABLISHMENT fails then fail the HO
             */
            reestabIncTrans->state = WR_UMM_REESTAB_REJECTED;
            if(ROK != wrUmmX2TgtHoSndPrepFailure(transCb))
            {
               RLOG0(L_ERROR,"Failed To Send Handover Failure");
            }
            transCb->state = WR_UMM_TRANS_DONE;
         }
#ifdef WR_RSYS_KPI
            /* Update the HO failure counter only when RRC reconfiguration complete
             * is not received which means isUeAttached state is FALSE */
            if( FALSE == transCb->u.x2TgtHoTrans.isUeAttached ) 
            {
               /* ccpu00141451: Update the isRrcReEstRcvd flag as TRUE
                * to increment the HO success counter via RRC Re-Reestablishment 
                * to target eNB */
               transCb->u.x2TgtHoTrans.isRrcReEstRcvd = TRUE;

               /* Increment the INTRA LTE HO Fail Counter */
               WR_INC_KPI_FUNC4(wrIncKpiFrHovrFaild, 
                     WR_HO_TYPE_X2, WR_HO_DIR_NONE, INC_HOV,
                     &(transCb->u.x2TgtHoTrans.cztCause) );
            }
#endif
      }
         break;
      case WR_UMM_UL_NAS_TRANS:
         {
            if(transCb->numDefMsgCount < WR_UMM_DEF_LIST)
            {
               WR_UMM_CPY_DEFR_MSG_TO_TRANSCB(transCb, incTrans->msg);       
            } 
            RLOG0(L_ERROR,"Maximum number of Derefred Message Execeding"
                  "the Specified Limi at  Defered processor");
            RETVALUE(WR_UMM_TRANS_FAIL); 
         }
         break;
      default :  
         {
            RLOG1(L_ERROR, "unhandled Incoming Transtype [%d]",
                  incTrans->transTyp);
         }
   }

   RETVALUE(WR_UMM_TRANS_OK);

}

/**
 * @brief 
 *    This function is X2 HO transaction specific release function.
 *
 * @details
 *    Function: wrUmmX2TgtHoTransRel
 *
 *    Processing steps:
 *       -# Release the transaction.
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmX2TgtHoTransRel
(
WrUmmTransCb                 *transCb
)
{
   WrUmmX2TgtHoTrans         *x2HoTrans = &(transCb->u.x2TgtHoTrans);
   WrUeCb                    *ueCb = transCb->ueCb;

   ueCb->hoTransId = 0;
   ueCb->mobCtrlState = WR_HO_NORMAL;
   
   if(x2HoTrans->isUeAttached == TRUE)
   {
      ueCb->hoType    = WR_HO_TYPE_NONE;
   }

   wrUtlFreeDedPreamble(ueCb->dedicatedPrmbl, ueCb->cellId);


   if(x2HoTrans->rbInfo)
   {
      WR_FREE(x2HoTrans->rbInfo,
         (x2HoTrans->numOfRbCfgd * sizeof(WrUmmX2TgtHoRabInfo)));
   }
   if(x2HoTrans->cztCause)
   {
      WR_FREE(x2HoTrans->cztCause, sizeof(CztCause));
   }
   if(x2HoTrans->snStsInfo)
   {
      WR_FREE(x2HoTrans->snStsInfo, sizeof(WrUmmSnStsInfo));
   }
   /* Fix for CR ccpu00125004, 06/12/12 */
   if(x2HoTrans->hovrRestrnLst)
   {
      WR_FREE(x2HoTrans->hovrRestrnLst, sizeof(CztHovrRestrnLst));
   }
   /* ccpu00131426 - Fix */
   /*Removed as part of UE history info*/
   if(x2HoTrans->rrmUeRecfgRspMsg)
   {
      wrUmmRlsIncMsg(&x2HoTrans->rrmUeRecfgRspMsg);
   }
   if(x2HoTrans->ueCap)
   {
      wrUmmDeAllocUeCap(x2HoTrans->ueCap);
   }

   if(x2HoTrans->eutraTgtInfo.sourceSCellRelLst_r10.noComp.val)
   {
      WR_FREE((x2HoTrans->eutraTgtInfo.sourceSCellRelLst_r10.member),
            (sizeof(NhuSCellIdx_r10) *
             x2HoTrans->eutraTgtInfo.sourceSCellRelLst_r10.noComp.val));
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
