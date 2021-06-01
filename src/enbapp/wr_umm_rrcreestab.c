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
               

     File:     wr_umm_rrcreestab.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=68;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_utils.h"
#include "wr_cmn.h"
#include "wr_emm.h"
#include "wr_umm.h"
#include "wr_smm_smallcell.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_sch_msg.h"
#include "wr_umm_trans.h"
#include "wr_ifm_rrc.h"
#include "wr_ifm_rrm.h"
#include "wr_ifm_schd.h"
#include "wr_kpi.h"
#include "wr_ifm_l1.h"
#include "wr_ifm_son.h"
#include "wr_ifm_dam.h"
#include "wr_umm_meas_fun.h"

#ifdef __cplusplus
EXTERN "C" {
#endif /* __cplusplus */

#define WR_UMM_RRC_REESTAB_UEID_CHG_TRANSID      0
#define WR_UMM_RRC_REESTAB_RECFG_TRANSID         1
#define WR_UMM_RRC_REESTAB_UERST_TRANSID         2
#define WR_UMM_RRC_REESTAB_UERECFG_TRANSID       3
#define WR_UMM_RRC_REESTAB_L1RECFG_TRANSID       4
#define WR_UMM_RRC_REESTAB_LCGCFG_TRANSID        5
#define WR_UMM_RRC_REESTAB_SCH_UEID_CHG_TRANSID  6
#define WR_UMM_RRC_REESTAB_MEAS_GAP_CFG_TRANSID  7

#ifdef WR_TEST_CODE
EXTERN U8 wrTrgRrcReConTO;
#endif
/* This should be moved to wr_umm_trans.h. Other procedures too should    */
/* use this macro to identify the sub transaction ids                     */
#define WR_UMM_SUBTRANS_MASK                     0x0000000f

#ifdef WR_TEST_CODE
EXTERN U8 wrTrgRrcRestTO;
#endif

/*-Forward Declarations of function prototype-*/
PRIVATE S16 wrUmmRccReestabMeasGapChk(WrUmmTransCb  *transCb);
PRIVATE S16 wrUmmReEstSchdMeasGapCfgReq(WrUmmTransCb  *transCb);
PRIVATE S16 wrUmmRrcReestabRrcReconfig(WrUmmTransCb  *transCb);
PRIVATE S16 wrUmmRrcReEstabCmpHdl(WrUmmTransCb  *transCb);
PRIVATE S16 wrUmmRrcReEstabPrcRrcDatInd(WrUmmTransCb  *transCb);
PUBLIC S16  wrUmmRrcReEstabTransProcessor(WrUmmTransCb  *transCb);
PUBLIC S16 wrUmmDefReestabIncCtxtRelProcessor(WrUmmTransCb *transCb, WrUmmTransCb *incTrans);
PUBLIC S16 wrUmmDefReestabIncReestProcessor(WrUmmTransCb *transCb, WrUmmTransCb *incTrans);
PUBLIC S16 wrUmmSendPdcpDataResumeReq(WrUmmTransCb *transCb);
EXTERN S16 wrUmmReestRrmRntiUpdt(WrUmmTransCb *transCb);
EXTERN S16 wrUmmHOFillReestReCfg(WrUeCb *ueCb, NhuDatReqSdus *recfgMsg);
PUBLIC S16 wrUmmRrcReEstabExtSecKeys(WrUmmTransCb *transCb,U8 keyDerType);
PRIVATE Void wrUmmRrcReestabPerdStrongCellMeasCfg(
WrUmmTransCb  *transCb,
WrUmmCellCb    *cellCb,
NhuDatReqSdus  *recfgMsg,
NhuRRCConRecfgn_r8_IEs   *recfgIEs
);


#ifdef TENB_AS_SECURITY
EXTERN S16 wrPlatRrcReEstabExtSecKeys(WrUmmTransCb *transCb, U8 keyDerType);
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @brief This function is used to handle RRC Re-Establish Complete 
 *
 * @details
 *
 *     Function:wrUmmRrcReEstabCmpHdl
 *
 *         Processing steps:
 *         - This function is invoked when re-establish complete is received 
 *           from UE. This sets transaction's state to COMPLETE and stops the 
 *           re-establishment timer.
 *
 * @param[in] transCb: Transaction CB 
 * @return S16
 *        -# Success : ROK
 */

PRIVATE S16 wrUmmRrcReEstabCmpHdl
(
WrUmmTransCb                 *transCb
)
{
   wrStopTransTmr(transCb, WR_TMR_REESTABLISH);
   RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti, "[OTA] Received" 
         " RRCConnectionReestablishmentComplete");

   transCb->u.rrcReEstabTransCb.state = WR_UMM_REESTAB_CMPL_RCVD;


   /*Increment KPI for Successful RRC connection re-establishments*/
    WR_INC_KPI_FUNC1(wrIncKpiFrReestbSuccdPrEtabCaus,
      transCb->u.rrcReEstabTransCb.reestabCause);
   
   /*Update statistics for RRC Re-Establishment Attempt*/
   wrEmmUpdateStat(WR_EMM_RRC_REEST_STATS,transCb->ueCb->cellId,WR_EMM_RRC_REESTAB_CON_SUCCESS,1,transCb->u.rrcReEstabTransCb.reestabCause);
   RETVALUE(ROK);
}

/** @brief This function is used derive whether given request can be  
  accepeted by eNodeB or not.
 *
 * @details
 *
 *     Function:wrUmmRrcReestabAdmit
 *
 *         Processing steps: This function based upon the Re-establishment cause 
 *            tries to determine whether request can be accepted or not.
 *
 * @param[in] transCb: Transaction CB 
 * @return S16
 *        -# Success : ROK
 */
PRIVATE S16 wrUmmRrcReestabAdmit
(
WrUmmTransCb                 *transCb
)
{
   NhuUlCcchMsg               *ulCcchMsg;
   NhuRRCConReestbRqst        *reestab;
   NhuRRCConReestbRqst_r8_IEs *r8_IE;
#ifdef TENB_AS_SECURITY
   /* HO_REST */
   WrUeSecCfg                 *secCfg;
   WrUeSecCfg                 *hzSecCfg;
#endif
   
   ulCcchMsg = &(transCb->msg->u.uuIndPdu->sdu.m.ulCcchMsg);
   reestab   = &(ulCcchMsg->ccchMsg.message.val.c1.val.rrcConReestbRqst);
   r8_IE     = &(reestab->criticalExtns.val.rrcConReestbRqst_r8);
   
   transCb->u.rrcReEstabTransCb.reestabCause = r8_IE->reestablishmentCause.val; 

   /*Increment KPI for Attempted RRC connection re-establishments*/
   WR_INC_KPI_FUNC1(wrIncKpiFrReestbAtmptdPrEtbCaus,
      r8_IE->reestablishmentCause.val);

   RLOG_ARG2(L_EVENT,DBG_CRNTI, transCb->ueCb->crnti, "[OTA] Received "
      "RRCConnectionReestablishmentRequest Cause = %d New Rnti = %d",
      r8_IE->reestablishmentCause.val, ulCcchMsg->tmpCrnti);
   
   /* State can already be Rejected if this RRC Reestablishment Transaction
    * is an incoming transaction for a on going RRC Reestablishment transaction 
    */ 
   if(transCb->u.rrcReEstabTransCb.state == WR_UMM_REESTAB_REJECTED) 
   {
       transCb->u.rrcReEstabTransCb.newRnti = ulCcchMsg->tmpCrnti;
       RETVALUE(ROK);
   }

   transCb->u.rrcReEstabTransCb.state = WR_UMM_REESTAB_REJECTED;

   /* ccpu00125904 : Check Ue State if it is not conected , Reject the restablishmentReq 
      Reestablishment received before AS security EnodeB shall ignore the Reestablishment 
      before AS security    */
   if (transCb->ueCb->ueAttachedtoMme != TRUE)
   {
          /*ccpu00127802*/
       RLOG_ARG1(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,
          "transCb->ueCb->rrcConnState= %d", transCb->ueCb->rrcConnState);
       transCb->u.rrcReEstabTransCb.newRnti = ulCcchMsg->tmpCrnti;
       RETVALUE(ROK);
   }
   /* ccpu00125904 */
   


   if (r8_IE->reestablishmentCause.val == NhuReestbCausereconfigurationFailEnum)
   {
      /* At this point, we return reestablishment reject. UE will         */
      /* establish connection with CN to resume the sessions              */
      transCb->u.rrcReEstabTransCb.newRnti = ulCcchMsg->tmpCrnti;
      RETVALUE(ROK);
   }
/* HO_REEST */
   else
   {
      /* Check if  Re-Establishment is due to late HO and send Re-establishment reject if
         UE's security algo is not supported */
      if ( transCb->ueCb->hoReestInfo.isTooLateHO == TRUE )
      {
          /* set tooLateHO flag to FALSE */
          transCb->ueCb->hoReestInfo.isTooLateHO = FALSE;
         /* Send RRC Re-establishment Reject if  Sevring cell Security algo and selected algo for 
            UE doesnt match */
          /* TODO: Sending failure if either integrity or ciphering algo doesnt
            match. May be we can handle in better way if only ciphering doesnt match */
         if ( (transCb->ueCb->hoReestInfo.srcIntAlgo != transCb->ueCb->secCfg->intgAlgo ) ||
               (transCb->ueCb->hoReestInfo.srcCiphAlgo != transCb->ueCb->secCfg->ciphAlgo ))
         {
            transCb->u.rrcReEstabTransCb.state   = WR_UMM_REESTAB_REJECTED;
            transCb->u.rrcReEstabTransCb.newRnti = ulCcchMsg->tmpCrnti;
            RETVALUE(ROK);
         }
#ifdef TENB_AS_SECURITY
         else
         {
             /* Copy UE secCfg to hzSecCfg, as the re-Establishment happend before HO completes no need to use the 
                horizontal key  */
             secCfg = transCb->ueCb->secCfg;
             if ( transCb->ueCb->hzSecCfg == NULLP )
             {
                WR_ALLOC(&transCb->ueCb->hzSecCfg, sizeof(WrUeSecCfg));
                if(transCb->ueCb->hzSecCfg == NULLP)
                {
                   RLOG0(L_FATAL, "Memory allocation failed.");
                   RETVALUE(RFAILED);
                }
             }
             hzSecCfg = transCb->ueCb->hzSecCfg;
             /*Store initial AS security keys*/
             cmMemcpy(hzSecCfg->cpCiphKey, secCfg->cpCiphKey, WR_CIPH_KEY_LEN);
             cmMemcpy(hzSecCfg->upCiphKey, secCfg->upCiphKey, WR_CIPH_KEY_LEN);
             cmMemcpy(hzSecCfg->intgKey, secCfg->intgKey, WR_INTG_KEY_LEN);
             cmMemcpy(hzSecCfg->kENB, secCfg->kENB, WR_KENB_KEY_LEN);
         }
#endif
      }

      transCb->u.rrcReEstabTransCb.state   = WR_UMM_REESTAB_ACCEPTED;
      transCb->u.rrcReEstabTransCb.newRnti = ulCcchMsg->tmpCrnti;
   }
   RETVALUE(ROK);
}


/** @brief This fucntion updates the new C-RNTI in the UeCb and update the
 *         transid accordingly
 *
 * @details
 *
 *     Function: wrUmmReestUpdtUeCbRnti
 *
 *         Processing steps: 1. Update current crnti a old crnti in ueCb
 *                           2. update the new crnti as crnti in UeCb
 *                           3. update the transId in transCb
 *
 * @param[in] transCb: Transaction CB 
 * @return Void
 */
PRIVATE Void wrUmmReestUpdtUeCbRnti(WrUmmTransCb *transCb)
{
   U16 minVal;
   U16 maxVal;
   /*Updation of OLD and NEW CRNTI in the UE Control Block*/
   transCb->ueCb->oldCrnti = transCb->ueCb->crnti; 
   transCb->ueCb->crnti = transCb->u.rrcReEstabTransCb.newRnti;

   transCb->u.rrcReEstabTransCb.ueRntiUpdtFlg = 0;
   /* free the dedicated crnti */
   wrEmmCellDedCrntiRange(transCb->ueCb->cellId,&minVal,&maxVal); /* THURSDAY FIX */
   if(transCb->ueCb->oldCrnti >= minVal && transCb->ueCb->oldCrnti <= maxVal)
   {
      wrUtlFreeDedCrnti(transCb->ueCb->oldCrnti, transCb->ueCb->cellId);
   }


  /* ccpu00131954  fix: Dont reset the transId - Presently as we are using
     ueIdx instead of crnti, no need to update the transId 
     Update the ueCb->dlcchTransId, to last transId which 
     will help in fethching the transCb when Re-establishment complete
      */
   transCb->ueCb->dlcchTransId = WR_GET_TRANS_FRM_TRANSID (transCb->transId );
}

/** @brief This function is responsible for processing the response(s) of UE 
 *         and logical channel configuration of the scheduler for this UE.
 *
 * @details
 *
 *     Function: wrUmmRrcReestabPrcSchdCfgCfm
 *
 *         Processing steps:
 *         - fetch the sub-transaction id from the transId,
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmRrcReestabPrcSchdCfgCfm 
(
WrUmmTransCb                 *transCb
)
{
   WrRrcReEstabTransCb       *reestabTrans = &(transCb->u.rrcReEstabTransCb);
   WrUmmSchdCfgCfm           *schdCfm = &(transCb->msg->u.schdCfm);
   U8                        idx;
    
   idx = schdCfm->transId & 0x0f;
   if (idx == WR_UMM_RRC_REESTAB_UERST_TRANSID)
   {
      /* This is in response to UE reconfiguration for AMBR              */
      if (schdCfm->status == RGR_CFG_CFM_OK)
      {
         reestabTrans->schdUeRstState = WR_UMM_RRC_RESTB_CFG_SUCC;
      }
      else
      {
         reestabTrans->schdUeRstState = WR_UMM_RRC_RESTB_CFG_FAIL;
         reestabTrans->curCfgRes = WR_UMM_RRC_RESTB_CFG_FAIL;
      }
   }
   else if (idx == WR_UMM_RRC_REESTAB_SCH_UEID_CHG_TRANSID)
   {
      if (schdCfm->status == RGR_CFG_CFM_OK)
      {
         if(reestabTrans->schdUeRstState == WR_UMM_RRC_RESTB_CFG_SUCC)
         {
            reestabTrans->schdUeCfgState = WR_UMM_RRC_RESTB_CFG_SUCC;
            reestabTrans->ueRntiUpdtFlg |= WR_UMM_NEW_RNTI_SCH_UPDT;
            if(reestabTrans->ueRntiUpdtFlg == WR_UMM_NEW_RNTI_UPDT_DONE)
            {
               wrUmmReestUpdtUeCbRnti(transCb);
            }
         }
      }
      else
      {
         reestabTrans->schdUeRstState = WR_UMM_RRC_RESTB_CFG_FAIL;
         reestabTrans->curCfgRes = WR_UMM_RRC_RESTB_CFG_FAIL;
         reestabTrans->schdUeCfgState = WR_UMM_RRC_RESTB_CFG_FAIL;
      }
   }

   else if (idx == WR_UMM_RRC_REESTAB_UERECFG_TRANSID)
   {
      if (schdCfm->status == RGR_CFG_CFM_OK)
      {
         reestabTrans->schdUeCfgState = WR_UMM_RRC_RESTB_CFG_SUCC;
      }
      else
      {
         reestabTrans->schdUeCfgState = WR_UMM_RRC_RESTB_CFG_FAIL;
         reestabTrans->curCfgRes = WR_UMM_RRC_RESTB_CFG_FAIL;
      }
   }
   else if ( idx == WR_UMM_RRC_REESTAB_MEAS_GAP_CFG_TRANSID )
   {
      RLOG0(L_DEBUG, " Received Schd CFM for meas gap cfg ");
      reestabTrans->measGapStatus = WR_UMM_MEAS_GAP_INIT_UE_CONFIG;
      if  ( ( schdCfm->status != RGR_CFG_CFM_OK ) || 
            ( wrUmmRrcReestabRrcReconfig(transCb) != ROK))
      {
         RLOG0(L_ERROR, " wrUmmRrcReestabRrcReconfig or schdCfg failed ");
         reestabTrans->state = WR_UMM_REESTAB_ERROR;
         RETVALUE(RFAILED);
      }
   }
   if ((reestabTrans->schdUeCfgState != WR_UMM_RRC_RESTB_CFG_FAIL)
      && (reestabTrans->schdUeRstState != WR_UMM_RRC_RESTB_CFG_FAIL))
   {
      reestabTrans->curCfgRes |= WR_UMM_RRC_RESTB_SCH_CFG_DONE;
   }
   RETVALUE(ROK);
}

#ifdef T2K_TRIGGER_RLC_REEST
EXTERN U32 drpRlcDrbPack;
#endif

/** @brief This function is used to handle RRC Data Indication 
 *
 * @details
 *
 *     Function:wrUmmRrcReEstabPrcRrcDatInd
 *
 *         Processing steps:
 *          - For all ULCCH message update teh acceptance status.
 *          - based upon the message category call respective routine
 *
 * @param[in] transCb: Transaction CB 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PRIVATE S16 wrUmmRrcReEstabPrcRrcDatInd
(
WrUmmTransCb                 *transCb
)
{
   NhuUL_DCCH_Msg            *ulDcchPdu = NULLP;
   U8                        messageType = 0;
   U8                        messageCat = 0;
   U32                       rrcTransId = 0;
   messageCat = transCb->msg->u.uuIndPdu->sdu.msgCategory;
   if (NHU_MSG_ULCCCH == messageCat)
   {
      wrUmmRrcReestabAdmit(transCb);
   }
   else if (NHU_MSG_ULDCCH == messageCat)
   {
      ulDcchPdu = &(transCb->msg->u.uuIndPdu->sdu.m.ulDcchMsg.dcchMsg); 
      messageType = ulDcchPdu->message.val.c1.choice.val;
      switch(messageType)
      {
         case C1_RRCCONREESTBCOMPL:
         {
#ifdef T2K_TRIGGER_RLC_REEST
            drpRlcDrbPack = 0;
#endif
            /*send the UE CRNTI modify indication to SON server*/
            wrIfmSonModUeInd(transCb->ueCb);
            /* As RRC Reestablishment Complete Received
             * reset the previous crnti 
             * this is because UE too updated its crnti to the new crnti
             */
            /* Assumption zero is not used a valid crnti
             * or fill some crnti which is not configured as valid crnti */
            transCb->ueCb->oldCrnti = 0; 
#ifdef WR_TEST_CODE
            if ( wrTrgRrcRestTO == TRUE )
            {
          wrTrgRrcRestTO = 2; 
               ALARM("Rest Cmpl Dropped!!\n");
               RETVALUE(ROK);
            }
#endif
            wrUmmRrcReEstabCmpHdl(transCb);
#ifdef TENB_AS_SECURITY
            /* ccpu00126532 */
            /*trigger key generation*/
            if(transCb->ueCb->secCfg->intgAlgo != WR_UMM_NULL_ALGO)
            {
               cmMemcpy((U8*)transCb->ueCb->secCfg->kENB,
                     (U8*)transCb->ueCb->hzSecCfg->kENB, WR_KENB_KEY_LEN);
#ifndef SS_RBUF
               wrPlatRrcReEstabExtSecKeys(transCb, CTF_KDF_TYPE2);
#else
               wrUmmRrcReEstabExtSecKeys(transCb, CTF_KDF_TYPE2); // hz derivation of kenb*
               wrUmmRrcReEstabExtSecKeys(transCb, CTF_KDF_TYPE3); // up/cp & integ prot key derivation
#endif // SS_RBUF              
            }
#endif
            break;
         }
         case C1_RRCCONRECFGNCOMPL:
         {
            NhuRRCConRecfgnCompl *recfgCmpl;

            recfgCmpl = &(ulDcchPdu->message.val.c1.val.rrcConRecfgnCompl);
            rrcTransId = recfgCmpl->rrc_TrnsxnId.val;
   /*ccpu00127802*/
            RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,
             "[OTA] Received RRCConnectionReconfigComplete after Reestablishment");
#ifdef WR_TEST_CODE
       if ( wrTrgRrcReConTO == TRUE )
       {
          ALARM("Dropping RRCCONRECFGNCOMPL!! \n");
          RETVALUE(ROK);
       }
#endif
            if (wrUmmRelRrcTransId(transCb->ueCb, C1_RRCCONRECFGN,
                                                        rrcTransId) != ROK)
            {
               RLOG_ARG0(L_ERROR, DBG_CRNTI, transCb->ueCb->crnti,
                  "RRC trans rel failed");
            }
       /* Stop the RRC Reconfiguration Timer */
            wrStopTransTmr(transCb,WR_TMR_RECONFIGURE);
            transCb->u.rrcReEstabTransCb.state = WR_UMM_REESTAB_RRC_CFG_DONE;
            if (transCb->u.rrcReEstabTransCb.measGapStatus == WR_UMM_MEAS_GAP_INIT_UE_CONFIG)
            {
               transCb->ueCb->measCfg->gapMeasInfo.isMeasGapCfg = WR_UMM_MEAS_GAP_CONFIGURED;
            }
/* HO_REEST */
            /* Forward the RRC Reconfiguration Complete to TGT HO Transaction
             */
            if(transCb->ueCb->hoTransId)
            {
               wrUmmFwdUuIndPdu (transCb);
            }
            wrUmmTransComplete(transCb);
            break;
         }
      }
   }

   RETVALUE(ROK);
}

/** @brief This function is used to send reconfiguratin request to scheduler 
 *
 * @details
 *
 *     Function:wrUmmRrcReEstabSchdUeRecfgReq
 *
 *         Processing steps:
 *          - Fill UE Reconfiguration structure for Scheduler 
 *          - Send Reconfiguration request using WrIfmSchdCfgUe 
 *
 * @param[in] transCb: Transaction CB 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */


/** @brief This function is responsible for LCG configuring 
 *         at the scheduler.
 *
 * @details
 *
 *
 *         Processing steps:
 *         - allocate a new configuration request
 *         - set the configuration type to RGR_LCG_CFG
 *         - set the configuration state to WR_UMM_RAB_SETUP_CFG_SENT
 *         - send the request to scheduler
 *
 * @param[in]  transCb : transaction information
 * @param[in]  grpId   : group Id
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
/** @brief This function is used to send Out DRAB Configuration request 
 *
 * @details
 *
 *     Function:wrUmmRabEstabSchdLchCfgReq
 *
 *         Processing steps:
 *          - Fill DRAB Reconfiguration structure for Scheduler 
 *          - Send Reconfiguration request using WrIfmSchdCfgUe 
 *
 * @param[in] transCb: Transaction CB 
 * @param[in] idx: Index for which configuration is being done.
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
/** @brief This function is used to process confirms received from scheduler 
 *
 * @details
 *
 *     Function:wrUmmRrcReEstabSchCfgCfm
 *
 *         Processing steps:
 *          - Validate the incoming transaction Id 
 *          - Update the confirmation status for DRAB
 *          - Iterate to assert arrival of all confirms 
 *
 * @param[in] transCb: Transaction CB 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
/* HO_REEST */
S16 PRIVATE wrUmmFillAddnlHoReestReCfg( WrUeCb *ueCb, NhuDatReqSdus *recfgMsg)
{

   wrUmmHOFillReestReCfg(ueCb, recfgMsg);
   RETVALUE(ROK);
}

/** @brief This function is used sending re-configuration request
 *
 * @details
 *
 *     Function:wrUmmRrcReestabRrcReconfig
 *
 *         Processing steps:
 *          - Fetch RRC Transaction Id 
 *          - Fill Reconfiguration message
 *          - Update relevant IE's and update message with radio resource configuration
 *          - Send message to UE using wrIfmRrcSndReqToUe
 *
 * @param[in] transCb: Transaction CB 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PRIVATE S16 wrUmmRrcReestabRrcReconfig
(
WrUmmTransCb                 *transCb
)
{
   NhuDatReqSdus                               *recfgMsg;
   WrUeCb                                      *ueCb = transCb->ueCb;
   NhuDL_DCCH_Msg                              *dcchMsg;
   NhuRRCConRecfgn                             *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs                      *recfgIEs;
   NhuRadioResourceConfigDedicated             *rrcCfgDed;
   U8                                          transId;
   NhuUeCfgInfo                                *ueCfg;
   NhuMAC_MainConfig                         *macCfg;
   /* SPS changes starts */
   U8                        bSpsBitMap = 0;
   /* SPS changes ends */

   WR_ALLOCEVNT(&recfgMsg, sizeof(NhuDatReqSdus));
   if (recfgMsg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   recfgMsg->hdr.cellId  = ueCb->cellId;
   recfgMsg->hdr.ueId    = ueCb->crnti;
   recfgMsg->hdr.transId = transCb->transId;

   /* Fetch an RRC transaction id for this procedure.                     */
   if (ROK != wrUmmGetRrcTransId(ueCb, C1_RRCCONRECFGN, 
                      WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
                      &transId))
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, ueCb->crnti, "RRC Transaction Not "
         "avialble for UE");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }

   /* Create the reconfiguration message which is constructued excluding  */
   /* the actual procedure related details which will be added further    */
   if (wrUmmRrcCreateRrcReconfigMsg(recfgMsg, transId) != ROK)
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, ueCb->crnti, "wrUmmRrcCreateRrcReconfigMsg"
         " failed");
      WR_FREE_EVNT(recfgMsg)
      RETVALUE(RFAILED);
   }

   /* Prepare the radio resources to exclude all the IEs present in the   */
   /* the IE. Those IEs are filled subsequently                           */
   dcchMsg     = &(recfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   recfgIEs    = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   rrcCfgDed   = &(recfgIEs->radioResourceConfigDedicated);
   wrUmmRrcPrepRadioResourceDed(rrcCfgDed);

   /*Radio Resource Mac Main config*/
   macCfg = &(rrcCfgDed->mac_MainConfig.val.explicitValue);
   wrFillTknU8(&(rrcCfgDed->mac_MainConfig.choice), 0);

   wrUmmRrcFillMacCfgIe(macCfg, ueCb);

   /*Radio Resource PHY Dedicated info config*/
   /* ReEst Fix */
   wrFillRadResPhyCfgDed(rrcCfgDed, (Ptr)recfgMsg, WR_PDCP_ID_SRB2,
   transCb->ueCb);

   /* SPS changes starts */
   /* If SPS is configured, fill in the source to target container */
   if (ueCb->spsCfg.spsCfgDl.bIsSpsEnabled == TRUE)
   {
      bSpsBitMap = RM_RCM_DL_SPS_CFG;
   }
   if (ueCb->spsCfg.spsCfgUl.bIsSpsEnabled == TRUE)
   {
      bSpsBitMap |= RM_RCM_UL_SPS_CFG;
   }
   wrUmmRrcFillSpsCfgIe(&(recfgMsg->memCp), bSpsBitMap, ueCb->spsCfg.spsRnti, &ueCb->spsCfg.spsCfgDl, \
         &ueCb->spsCfg.spsCfgUl, &rrcCfgDed->sps_Config, wrEmmGetDuplexMode(ueCb->cellId));
   /* SPS changes ends */

/* HO_REEST */
   /* If HO is in progress then fill  
      1. SRB2 srb-ToAddModList
      2. drb-ToAddModList 
      3.  TODO: Fill any other IE's missing/required.
   */
   if ( transCb->ueCb->mobCtrlState ==  WR_HO_IN_PROGRESS ) 
   {
       wrUmmFillAddnlHoReestReCfg(transCb->ueCb, recfgMsg);
   }
   /* KWork fix */
   if ( ueCb->measCfg->eutraRptCgiCfg != NULLP )
   {
      /* Currently Below API handle to delete only reportCGI config
       * from UE Cb, complete Periodic config deletion shall be
       * done in next release*/
      wrUmmMeasDelPerdCfgInUeCb(ueCb->measCfg);
      WR_FREE(ueCb->measCfg->eutraRptCgiCfg, sizeof(WrMeasRptCfgNode));
   }

   /* Reconfigure Periodic Strong Cell Measurement configuration */
   WrUmmCellCb    *cellCb;
   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId)
   LwrMeasEutraPeridRptCfg   *eutraPerd;
   /*KWork Fix*/
   if(cellCb != NULLP)
   {
      eutraPerd = &cellCb->measCellCfg->measEutraPerdRptCfg[WR_MEAS_EUTRA_PERIODIC_REPORT_STRNG_CELLS];
      if(eutraPerd->isPerdStrongCellCfgRptEnable == TRUE)
      {
         wrUmmRrcReestabPerdStrongCellMeasCfg(transCb, cellCb, recfgMsg, recfgIEs);
      }
   }

   /* Fill Measurement gap if required */
   if (transCb->transId  == ueCb->measCfg->gapMeasInfo.latestTxId)
   {
       wrFillTknU8(&(recfgIEs->measConfig.pres), TRUE);
       wrUmmMeasFillMeasGapCfg(transCb->u.rrcReEstabTransCb.measGapStatus,
                                ueCb->measCfg, 
                                &(recfgIEs->measConfig.measGapConfig)); 
   }
   /* Build the configuration towards RRC                                 
   wrUmmRrcReEstabFillRrcCfg(transCb, recfgMsg);*/
   recfgMsg->sdu.isUeCfgPres = TRUE;
   ueCfg = &(recfgMsg->sdu.ueCfg);
   ueCfg->conReCfgCause = NHU_CONN_RECFG_AFTER_RESTB;

   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending " 
         " RRCConnectionReconfiguration after Reestablishment");
   if (RFAILED == wrIfmRrcSndReqToUe(recfgMsg))
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, ueCb->crnti, "wrIfmRrcSndReqToUe failed.");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }

   /* Start RRC Reconfiguration timer */
   wrStartTransTmr(transCb,WR_TMR_RECONFIGURE, wrEmmGetRrcReCfgTmrVal(ueCb->cellId));

   RETVALUE(ROK);
}

/** @brief This function is used fill RRC Configuration 
 *
 * @details
 *
 *     Function:wrUmmRrcReEstabFillRrcCfg
 *
 *         Processing steps:
 *
 * @param[in] transCb: Transaction CB 
 * @param[in] evnt : RRC configuration structur
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */

/** @brief This function is used to send reject for re-establishment request received 
 *
 * @details
 *
 *     Function:wrUmmRrcReestabReject
 *
 *         Processing steps:
 *         - Build Re-establish Request using wrUmmRrcBldReestabReject
 *         - Send Reject response to UE using wrIfmRrcSndCcchRspToUe
 *         - Mark Transaction as complete
 *
 * @param[in] transCb: Transaction CB 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PRIVATE S16 wrUmmRrcReestabReject
(
WrUmmTransCb                 *transCb
)
{
   NhuDatRspSdus             *reestabPdu;

   wrEmmUpdateStat(WR_EMM_RRC_REEST_STATS,transCb->ueCb->cellId,WR_EMM_RRC_REESTAB_CON_FAIL,1,transCb->u.rrcReEstabTransCb.reestabCause);

   /* Send RRC REESTABLISHMENT REJECT and release the UE CB by invoking   */
   /* local context release                                               */
   WR_ALLOCEVNT(&reestabPdu, sizeof(NhuDatRspSdus));
   if (reestabPdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   reestabPdu->sdu.m.dlCcchMsg.rbId = WR_CCCH_DL_ID;
   reestabPdu->sdu.m.dlCcchMsg.tmpCrnti = transCb->u.rrcReEstabTransCb.newRnti;
   wrUmmFillRrcEvntHdr(&(reestabPdu->hdr), transCb->ueCb->cellId, 
                  transCb->ueCb->crnti, 
         transCb->transId | WR_UMM_RRC_REESTAB_RECFG_TRANSID);


   wrUmmRrcBldReestabReject(reestabPdu);
   RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti, "[OTA] Sending "
         "RRCConnectionReestablishmentReject ");
   if (wrIfmRrcSndCcchRspToUe(reestabPdu) != ROK)
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, transCb->ueCb->crnti,
         "wrIfmRrcSndReqToUe failed");
      WR_FREEEVNT(reestabPdu);
      RETVALUE(RFAILED);
   }
   
   transCb->ueCb->rrcConnState = WR_UMM_RRC_IDLE;
   transCb->ueCb->mobCtrlState = WR_HO_NORMAL;/*ccpu00135736 fix*/

   RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"RRC Connection State=%d,"
      "Cause = RRC Re-Establishment Reject", transCb->ueCb->rrcConnState);
      
  /*Increment KPI for Failed RRC connection re-establishments*/
  WR_INC_KPI_FUNC1(wrIncKpiFrReestbFaildPrEtabCaus,
     transCb->u.rrcReEstabTransCb.reestabCause); 
  
   RETVALUE(ROK);
}

/**
 *  @brief This function build and send the UE release message
 *
 *      Function: wrUmmRrcReestabL1UeReCfg 
 *
 *          Processing steps:
 *          - Allocate memory for ue reCfg message 
 *          - update the structure
 *          - call the interface function to send the message
 *
 *
 *  @param [in]  transId : transid of the UE reconfig message sent to Ctf
 *  @param [in]  crnti   : crnti of the UE to be reconfigured
 *  @param [in]  cellId  : cell ID of cell from which UE is reconfigured
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrUmmRrcReestabL1UeReCfg 
(
U32                          transId,
U16                          oldCrnti,
U16                          cellId,
U16                          newCrnti
)
{
   CtfCfgReqInfo             *reCfgInfo = NULLP;
   CtfDedRecfgInfo           *ueCfgParam = NULLP;

   WR_ALLOC(&reCfgInfo, sizeof(CtfCfgReqInfo));
   if(reCfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   reCfgInfo->cfgType           = CTF_RECONFIG;
   reCfgInfo->u.reCfg.cfgElem  = CTF_UE_CFG;

   ueCfgParam = &reCfgInfo->u.reCfg.u.dedRecfg;

   ueCfgParam->cellId        = cellId;
   ueCfgParam->ueId          = oldCrnti;
   ueCfgParam->newUeId       = newCrnti;

      /*ccpu00127802*/

   RLOG_ARG0(L_INFO, DBG_CELLID, cellId, "Sending CTF_RECONFIG");

   if(ROK != wrIfmPhyUeCfg(reCfgInfo, transId))
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, cellId, "wrUmmRrcReestabL1UeReCfg failed.");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}


/**
 *  @brief This function buids the message for UeId change
 *
 *      Function: wrUmmRrcReestabSchUeReset 
 *
 *          Processing steps:
 *          - allcate memory for the UE reset message
 *          - Dispatch the prepared message to scheduler
 *
 *  @param [in] transId : transaction Id
 *  @param [in] cellId  : cell Id of serving cell
 *  @param [in] oldRnti : RNTI to be changed
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */

PUBLIC S16 wrUmmRrcReestabSchUeReset
(
U32                          transId,
U16                          cellId,
U16                          oldRnti
)
{
   RgrCfgReqInfo             *cfgReq;
   RgrRst                    *ueRst;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   cfgReq->action                = RGR_RESET;
   ueRst                         = &cfgReq->u.rstInfo;
   ueRst->cellId                   = cellId;
   ueRst->crnti                   = oldRnti;
   /* Dispatch the prepared message to scheduler.                         */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
} /* end of wrUmmRrcReestabSchUeReset*/ 

/** @brief This function is used to send re-configuration request to Scheduler 
 *
 * @details
 *
 *     Function:wrUmmRrcReestabSchUeIdChg
 *
 *         Processing steps:
 *         -Invokes scheduler module to Send reconfiguration 
 *
 * @param[in] transCb: Transaction CB 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PRIVATE S16 wrUmmRrcReestabSchUeIdChg
(
WrUmmTransCb                 *transCb
)
{
   WrRrcReEstabTransCb       *reestabTrans = &(transCb->u.rrcReEstabTransCb);
   WrUeCb                    *ueCb = transCb->ueCb;
   U32                       transId;
   S16                       retValue;

   transId = transCb->transId | WR_UMM_RRC_REESTAB_SCH_UEID_CHG_TRANSID;

   /* The steps involved are resetting MAC scheduler for UE and changing  */
   /* the RNTI of the UE to the new RNTI.                                 */
   retValue = wrUmmSchdUeIdChg(transId, ueCb->cellId, ueCb->crnti, reestabTrans->newRnti,ueCb);

   /* Now reset the scheduler for the UE                                  */
   RETVALUE(retValue);
}

/** @brief This function is used to send re-establishment setup to UE
 *
 * @details
 *
 *     Function:wrUmmRrcReestabSndReestablishment
 *
 *         Processing steps:
 *          - Build Re-establish message
 *          - Send Response to UE
 *
 * @param[in] transCb: Transaction CB 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PRIVATE S16 wrUmmRrcReestabSndReestablishment
(
WrUmmTransCb                 *transCb
)
{
   NhuDatRspSdus             *reestabSdu;

   WR_ALLOCEVNT(&reestabSdu, sizeof(*reestabSdu));
   if (reestabSdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   reestabSdu->sdu.m.dlCcchMsg.rbId = WR_CCCH_DL_ID;
   reestabSdu->sdu.m.dlCcchMsg.tmpCrnti = transCb->u.rrcReEstabTransCb.newRnti;
   wrUmmFillRrcEvntHdr(&(reestabSdu->hdr), transCb->ueCb->cellId, 
                  transCb->ueCb->crnti, 
         transCb->transId | WR_UMM_RRC_REESTAB_RECFG_TRANSID);

   if (wrUmmRrcBldReestabMsg(reestabSdu, transCb->ueCb) != ROK)
   {
      RETVALUE(RFAILED);
   }

   RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"[OTA] Sending"
         " RRCConnectionReestablishment");

   if (wrIfmRrcSndCcchRspToUe(reestabSdu) != ROK)
   {
      RETVALUE(RFAILED);
   }
   /*Start RRC CON Re-Establsihment TImer*/
#if 0
#else
   wrStartTransTmr(transCb, WR_TMR_REESTABLISH, 
         wrEmmGetRrcReestabTmrVal (transCb->ueCb->cellId));
#endif
   RETVALUE(ROK);
}

/** @brief 
 *
 * @details
 *
 *     Function:wrUmmRrcReestabUeIdChg
 *
 *         Processing steps:
 *
 * @param[in] transCb: Transaction CB 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PRIVATE S16 wrUmmRrcReestabUeIdChg
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrRrcReEstabTransCb       *reestabTrans = &(transCb->u.rrcReEstabTransCb);

   /* The first step in accpeting reestablishment is to set the DAM in    */
   /* reestablishment mode such that it does not deliver the DL data to   */
   /* PDCP and RLC layers.                                                */
   /* For any failure during the procedure, we may just suspend the       */
   /* procedure and forward a UE CONTEXT RELEASE REQUEST to MME for       */
   /* clearing the UE context. MME will respond with a UE CONTEXT RELEASE */
   /* COMMAND for clean up                                                */
   if (wrIfmDamReestabReq(transCb->transId, ueCb->cellId, ueCb->crnti,
                                             reestabTrans->newRnti) != ROK)
   {
      reestabTrans->state = WR_UMM_REESTAB_ERROR;
      RETVALUE(RFAILED);
   }
   reestabTrans->schdUeCfgState = WR_UMM_RRC_RESTB_CFG_FAIL;
   reestabTrans->schdUeRstState = WR_UMM_RRC_RESTB_CFG_FAIL;
   
   if (wrUmmRrcReestabSchUeReset(
         (transCb->transId | WR_UMM_RRC_REESTAB_UERST_TRANSID), 
            ueCb->cellId, ueCb->crnti) != ROK)
   {
      reestabTrans->state = WR_UMM_REESTAB_ERROR;
      RETVALUE(RFAILED);
   }

   if (wrUmmRrcReestabSchUeIdChg(transCb) != ROK)
   {
      reestabTrans->state = WR_UMM_REESTAB_ERROR;
      RETVALUE(RFAILED);
   }

   if (wrUmmRrcReestabL1UeReCfg(
         (transCb-> transId | WR_UMM_RRC_REESTAB_L1RECFG_TRANSID),
      ueCb->crnti,ueCb->cellId,reestabTrans->newRnti) != ROK)
   {
      reestabTrans->state = WR_UMM_REESTAB_ERROR;
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmRrcReEstabTransTimerExp
(
    WrUmmTransCb              *transCb
)
{
    if(transCb == NULLP)
           RETVALUE(RFAILED);

    WrRrcReEstabTransCb       *reestabTrans = &(transCb->u.rrcReEstabTransCb);
    
    switch(transCb->msg->u.tmrExpiry.timerEvnt)
    { 
        case WR_TMR_REESTABLISH:
        {
            /*Added for KPI*/
            WR_INC_KPI_FUNC1(wrIncKpiFrReestbFaildPrEtabCaus,
               transCb->u.rrcReEstabTransCb.reestabCause);  
            reestabTrans->state = WR_UMM_REESTAB_ERROR;
            break;
        }
        case WR_TMR_RECONFIGURE: 
        {
            reestabTrans->state = WR_UMM_REESTAB_ERROR;
            break;
        }
        default:
        {
            ALARM("ALARM!! SWERROR %s:%s Received %d invalid Timer Event\n",
                __FILE__, __FUNCTION__, transCb->msg->u.tmrExpiry.timerEvnt);
            RETVALUE(RFAILED);
        } 
    }   
    RETVALUE(ROK);
}

PRIVATE S16 wrUmmHandleRrcReEstabFailure
(
 WrUmmTransCb                 *transCb
 )
{
    if(transCb == NULLP)
    {
      RETVALUE(RFAILED);
    }

    if((transCb->msg->msgTyp == WR_TMR_REESTABLISH) ||
       (transCb->msg->msgTyp == WR_TMR_RECONFIGURE))
    {
       wrEmmUpdateStat(WR_EMM_RRC_REEST_STATS, transCb->ueCb->cellId,
                            WR_EMM_RRC_REESTAB_CON_FAIL_DEGRAD, 1,
                            transCb->u.rrcReEstabTransCb.reestabCause);
    }
    else
    {
       wrEmmUpdateStat(WR_EMM_RRC_REEST_STATS, transCb->ueCb->cellId,
                       WR_EMM_RRC_REESTAB_CON_FAIL, 1,
                       transCb->u.rrcReEstabTransCb.reestabCause);
    }

    if(transCb->u.rrcReEstabTransCb.rejectCause == WR_UMM_REJ_CAUSE_INIT_CTXT_GOING_ON)
    {
      /* This is only when RRC Re-establishment message was initiated 
        when initial context transaction was ongoing scenarios*/
      wrUmmTransComplete(transCb);
    }
    else
    {
      /* This is for all other scenarios including RRC Re-establishment message while
         RRC connection request message was ongoing
      */
      transCb->errorCause = WR_UMM_UE_LOST;
      transCb->state = WR_UMM_TRANS_RELEASE_CALL; 
    }

    RETVALUE(ROK);
}

/** @brief Function handler to process RRC Re-establish Transaction transaction 
 *
 * @details
 *
 *     Function:wrUmmRrcReEstabTransProcessor
 *
 *         Processing steps: This function is used to process incoming RRC events.
 *         Based upon the incoming message type apt routine is called to handle the message.
 *
 * @param[in] transCb: Transaction CB 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrUmmRrcReEstabTransProcessor
(
WrUmmTransCb                 *transCb
)
{
   WrRrcReEstabTransCb       *reestabTrans = &(transCb->u.rrcReEstabTransCb);
   WrUeCb                    *ueCb = transCb->ueCb;

   switch(transCb->msg->msgTyp)
   {
      case WR_UMM_UU_IND_PDU:
      {
         wrUmmRrcReEstabPrcRrcDatInd(transCb);
       /*Update statistics for RRC Re-Establishment Attempt*/
       wrEmmUpdateStat(WR_EMM_RRC_REEST_STATS,transCb->ueCb->cellId,WR_EMM_RRC_REESTAB_CON_ATTEMPT,1,transCb->u.rrcReEstabTransCb.reestabCause);
#ifdef WR_TEST_CODE
            if ( wrTrgRrcRestTO == 2 )
            {
          wrTrgRrcRestTO = TRUE;
               RETVALUE(ROK);
            }
#endif
         /* Stop Timer */
         //wrStopUeTmr(transCb->ueCb, WR_TMR_RAD_RES_MOD_TMR);
#ifdef WR_DETECT_RLF
         wrStopUeTmr(transCb->ueCb, WR_TMR_DETECT_RLF);
#endif

         if (reestabTrans->state == WR_UMM_REESTAB_REJECTED)
         {
            wrUmmRrcReestabReject(transCb);
         }
         else if (reestabTrans->state == WR_UMM_REESTAB_ACCEPTED)
         {
            /* first update scheduler with new crnti */
            reestabTrans->curCfgRes = 0x00;
            wrUmmRrcReestabUeIdChg(transCb);
            /*Change for LC*/

         }
         /*Fix for ccpu00123852 */
         else if ((reestabTrans->state == WR_UMM_REESTAB_CMPL_RCVD) &&
                                (transCb->state != WR_UMM_TRANS_DONE))
         {
            /*ccpu00127802*/      
            RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti, "Re-Establishment "
               "COMPLETE has been received from UE");
            if((WR_CSFB_GSM_UE_STATE_RRC_REDIR == 
                  transCb->ueCb->csfbInfo.csfbState))
            {
               wrUmmCsfbSndRedirAftReest(transCb->ueCb);
               wrUmmTransComplete(transCb);
            }
            else if(reestabTrans->isUeCtxtRelDefrd)
            {
               wrUmmTransComplete(transCb);
            }
            else
            {   
               transCb->ueCb->rrcConnState = WR_UMM_RRC_CONNECTED;

               /* Check if measurement gap needs to be configured */
               if ( ROK != wrUmmRccReestabMeasGapChk(transCb))
               {
                  reestabTrans->state = WR_UMM_REESTAB_ERROR;
                  break;
               }
               /* Reconfiguration of RRC,PDCP, RLC MAC Layers                      */
               if ((transCb->ueCb->measCfg->gapMeasInfo.latestTxId == transCb->transId) && 
                     (reestabTrans->measGapStatus == WR_UMM_MEAS_GAP_INIT_SCH_CONFIG))
               {
                  /* Configure the measurement gap to scheduler */
                  wrUmmReEstSchdMeasGapCfgReq(transCb);
               }
               else
               {
                  if  (wrUmmRrcReestabRrcReconfig(transCb) != ROK)
                  {
                     reestabTrans->state = WR_UMM_REESTAB_ERROR;
                     break;
                  }
               }
               /* UE is in reestablishment  state so reconfigure  the DAM to          */
               /* normal  mode such that it does send  the DL data to                 */
               /* PDCP and RLC layers.                                                */
               if (wrIfmDamReCfgReq(transCb->transId, transCb->ueCb->cellId, 
                        transCb->ueCb->crnti) != ROK)
               {
                  reestabTrans->state = WR_UMM_REESTAB_ERROR;
                  RETVALUE(RFAILED);
               }
            }
         }
         else
         {
	    
            /* This is the response to RRC Reconfiguration. Complete the */
            /* transaction as the procedure is comkpleted successfully   */
            /* SUBBU */
            /* Data Resume Primitives needs to be formed and send to RRC */
            RLOG0(L_DEBUG, "Sending PDCP Data Resume Req");
            transCb->state = WR_UMM_TRANS_INUSE; 
            wrUmmSendPdcpDataResumeReq(transCb);
#ifndef WR_GRPPWR_CNTRL
            //wrStartUeTmr(ueCb, WR_TMR_RAD_RES_MOD_TMR, 
                      //WR_TMR_RAD_RES_MOD_TMR_VAL);
         #endif
#ifdef WR_DETECT_RLF
            wrStartUeTmr(ueCb, WR_TMR_DETECT_RLF, WR_TMR_DETECT_RLF_VAL);
#endif
         }
         break;
      }
      case WR_UMM_RRC_DAT_CFM:
      {
         /*ccpu00127802*/      
         RLOG_ARG0(L_DEBUG, DBG_CRNTI, ueCb->crnti, "Re-Establishment RRC DAT "
            "CFM has been received");
         break;
      }
      case WR_UMM_SCHD_CFG:
      {
         wrUmmRrcReestabPrcSchdCfgCfm(transCb);
         /* TODO: Need implementation to handle UE ID change failure      */
         if((reestabTrans->curCfgRes & WR_UMM_RRC_RESTB_SCH_CFG_DONE))
         {
            RLOG0(L_DEBUG, " Received Re-Establishment SCHD CFM ");
         }
         if((reestabTrans->curCfgRes & 0x0f) == (WR_UMM_RRC_RESTB_SCH_CFG_DONE | WR_UMM_RRC_RESTB_L1_CFG_DONE))
         {
            RLOG0(L_DEBUG, " Received Re-Establishment PHY CFM ");
            RLOG0(L_DEBUG, " Sending Re-Establishment ACCEPT to UE ");

            if (wrUmmRrcReestabSndReestablishment(transCb) != ROK)
            {
               reestabTrans->state = WR_UMM_REESTAB_ERROR;
            }
         }

            break;
      }
      case WR_UMM_PHY_CFG:
      {
         /* TODO: Need implementation to handle UE ID change failure      */
         /* Fix for ccpu00126865 */
         if(transCb->msg->u.phyCfm.status != CTF_CFG_CFM_OK)
         {
            RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti, "PHY Config failed");
            reestabTrans->curCfgRes = WR_UMM_RAB_SETUP_CFG_FAIL;
         }
         else
         {
            reestabTrans->curCfgRes |= WR_UMM_RRC_RESTB_L1_CFG_DONE;
            reestabTrans->ueRntiUpdtFlg |= WR_UMM_NEW_RNTI_PHY_UPDT;
            if((reestabTrans->curCfgRes & 0x0f)== (WR_UMM_RRC_RESTB_SCH_CFG_DONE | WR_UMM_RRC_RESTB_L1_CFG_DONE))
            {
               RLOG_ARG0(L_DEBUG, DBG_CRNTI, ueCb->crnti, "Re-Establishment PHY CFM");
               RLOG0(L_DEBUG, " Sending Re-Establishment ACCEPT to UE ");
               if (wrUmmRrcReestabSndReestablishment(transCb) != ROK)
               {
                  reestabTrans->state = WR_UMM_REESTAB_ERROR;
               }
            }

         }
         /*ccpu00127802*/      
           RLOG_ARG0(L_DEBUG, DBG_CRNTI, ueCb->crnti, "Re-Establishment PHY CFM");
         break;
      }
      case WR_UMM_RRC_CFG:
      {

         if(transCb->msg->u.rrcCfm->sdu.isCfgSuccess == TRUE)
         {
              RLOG_ARG0(L_DEBUG, DBG_CRNTI, ueCb->crnti, "Re-Establishment NHU CFM");
            
            reestabTrans->ueRntiUpdtFlg |= WR_UMM_NEW_RNTI_RRC_UPDT;

            if((reestabTrans->curCfgRes & 0x0f)== 0x06)
            {
               reestabTrans->curCfgRes |= WR_UMM_RRC_RESTB_RRC_CFG_DONE;
               wrUmmReestRrmRntiUpdt(transCb);

               reestabTrans->ueRntiUpdtFlg |= WR_UMM_NEW_RNTI_RRC_UPDT;
            }
         }
         else
         {
              RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti, "RRC Config failed");
            reestabTrans->curCfgRes = WR_UMM_RRC_RESTB_CFG_FAIL;
         }
         break;
      }
      case WR_UMM_TMR_EXPIRY:
      {
          wrUmmRrcReEstabTransTimerExp(transCb);
          break;
      }
#ifdef TENB_AS_SECURITY
      case WR_UMM_PHY_KDF:
      {
         /*ccpu00127802*/
           RLOG_ARG0(L_INFO, DBG_CRNTI, ueCb->crnti, "AS KEY GENERATION SUCESS");
         if(transCb->msg->u.kdfCfm.ctfKdfCfm->keyDerType == CTF_KDF_TYPE1)
         {
             cmMemcpy(ueCb->hzSecCfg->cpCiphKey, transCb->msg->u.kdfCfm.ctfKdfCfm->u.kEnbInf.cpCiphKey, 16);
             cmMemcpy(ueCb->hzSecCfg->upCiphKey, transCb->msg->u.kdfCfm.ctfKdfCfm->u.kEnbInf.upCiphKey, 16);
             cmMemcpy(ueCb->hzSecCfg->intgKey, transCb->msg->u.kdfCfm.ctfKdfCfm->u.kEnbInf.intKey,16);
             
         }
         else if(transCb->msg->u.kdfCfm.ctfKdfCfm->keyDerType == CTF_KDF_TYPE2)
         {
             cmMemcpy(ueCb->hzSecCfg->kENB, 
                   transCb->msg->u.kdfCfm.ctfKdfCfm->u.kEnbStarInf.secKey,
                   WR_KENB_KEY_LEN);
#ifndef SS_RBUF
             wrPlatRrcReEstabExtSecKeys(transCb, CTF_KDF_TYPE1);
#else
             wrUmmRrcReEstabExtSecKeys(transCb, CTF_KDF_TYPE1);
#endif // SS_RBUF      
         }
         break;
      }
#endif
      case WR_UMM_RRM_UERECFG_RESP:
      {
          RLOG_ARG0(L_DEBUG, DBG_CRNTI, ueCb->crnti, "Re-Establishment RRM CFM");

          reestabTrans->ueRntiUpdtFlg |= WR_UMM_NEW_RNTI_RRM_UPDT;
          if(reestabTrans->ueRntiUpdtFlg == WR_UMM_NEW_RNTI_UPDT_DONE)
          {
             wrUmmReestUpdtUeCbRnti(transCb);
          }
          break;
      }
      case WR_UMM_PDCP_DATA_RESUME_CFM:
      {
         RLOG0(L_DEBUG, "Received PDCP Data Resume CFM");
         /* Complete the transaction as the procedure is completed successfully */
         wrUmmTransComplete(transCb);

         break;
      }
      default:
      {
         RLOG_ARG1(L_ERROR,DBG_CRNTI,ueCb->crnti,"Invalid msg (%d) received",
               transCb->msg->msgTyp);
         break;
      }
   }

   if (reestabTrans->state == WR_UMM_REESTAB_ERROR || 
          reestabTrans->state == WR_UMM_REESTAB_REJECTED)
   {
      /* If we come here with error, we indicate to MME that the context  */
      /* must be released. MME will initiate context release procedure    */
       wrUmmHandleRrcReEstabFailure(transCb);
   }
   RETVALUE(ROK);
}

/** @brief This function is RRC Re-establishment transaction specific release  
 *
 * @details
 *
 *     Function:wrUmmRrcReestabTransRel
 *
 *         Processing steps:
 *
 * @param[in] transCb: Transaction CB 
 * @return S16
 *        -# Success : ROK
 *        -# Failure : RFAILED
 */
PUBLIC S16 wrUmmRrcReestabTransRel
(
WrUmmTransCb                 *transCb
)
{

   wrStopTransTmr(transCb, WR_TMR_REESTABLISH);
   /* Move all deferred transactions of re-establishment transaction to 
    * ueCb main transLst
    */
   /* Since Re-Est will be having Deffered setting numDefMsgCnt to ZERO */
   RETVALUE(ROK);
}

/** @brief This function is called when any message related to a RRC Re-establsiment
 *         transaction during RRC Re-establishment transaction . 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmDefReestabIncReestProcessor 
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDefReestabIncReestProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
    WrRrcReEstabTransCb       *reestabIncTrans = 
                                        &(incTrans->u.rrcReEstabTransCb);
   /* Fix For Reestablishment */
   if((transCb->u.rrcReEstabTransCb.state == WR_UMM_REESTAB_ACCEPTED)
         || (transCb->u.rrcReEstabTransCb.state == WR_UMM_REESTAB_CMPL_RCVD))
   {
      NhuUlCcchMsg               *ulCcchMsg;
      NhuRRCConReestbRqst        *reestab;
      NhuRRCConReestbRqst_r8_IEs *r8_IE;

        /*Parse Re-establsihment cause in second RRC Re-establishment msg */ 
        ulCcchMsg = &(incTrans->msg->u.uuIndPdu->sdu.m.ulCcchMsg);
        reestab   = &(ulCcchMsg->ccchMsg.message.val.c1.val.rrcConReestbRqst);
        r8_IE     = &(reestab->criticalExtns.val.rrcConReestbRqst_r8);
        if (r8_IE->reestablishmentCause.val ==
                                               NhuReestbCausereconfigurationFailEnum)
        {
           /* Send RRC Re-establishment Reject to second RRC Re-Est Req Msg*/
           /* Continue processing the first Re-establishment Req Msg*/
           /*To Override the Second RRC Re-Est to fail*/
           r8_IE->reestablishmentCause.val = NhuReestbCausereconfigurationFailEnum;
           reestabIncTrans->state = WR_UMM_REESTAB_REJECTED;
           /* Complete the first Re-establishment transaction*/
           transCb->state = WR_UMM_TRANS_DONE;
        }
        else
        {
           /* Complete the first Re-establishment transaction*/
           transCb->state = WR_UMM_TRANS_DONE;
        }
  }
  else if (transCb->state == WR_UMM_TRANS_STATE_ROLLBACK)
  {
     /*TODO Actions due to Radio Link failure*/
     ALARM("Radio Link failure during Re-establishment \
                              received at  Defered processor\n");
     /* Complete the first Re-establishment transaction*/
     transCb->state = WR_UMM_TRANS_DONE;
  }
   RETVALUE(WR_UMM_TRANS_OK);
}

/** @brief This function is called when any message related to ue context release
 *         transaction during RRC Reestablishment transaction. 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmDefReestabIncCtxtRelProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDefReestabIncCtxtRelProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
   S16        ret = RFAILED;

   if((WR_UMM_REESTAB_CMPL_RCVD == transCb->u.rrcReEstabTransCb.state) ||
      (WR_UMM_REESTAB_RRC_CFG_DONE == transCb->u.rrcReEstabTransCb.state))
   {
      wrUmmStopTransTimers(transCb);
      ret = ROK;
   }

   RETVALUE(ret);
}

/** @brief This function is called when any message related to a particular
 *         transaction during RRC Re-establsihment transaction . 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmReestabNewTransProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  inctransCb : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmReestabNewTransProcessor 
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
   S16               ret;

   switch(incTrans->transTyp)
   {
      case WR_UMM_RRC_REESTAB_TRANS: 
         {
            wrUmmDefReestabIncReestProcessor(transCb,incTrans);
            break;
         }
         /*RRC Rel10 Upgrade - start*/ 
      case WR_UMM_CTXT_REL_TRANS:
         {
            ret = wrUmmDefReestabIncCtxtRelProcessor(transCb,incTrans);
            if(ROK == ret)
            {
               transCb->state = WR_UMM_TRANS_DONE;
            }
            else
            {
               if(transCb->numDefMsgCount < WR_UMM_DEF_LIST)
               {
                  transCb->u.rrcReEstabTransCb.isUeCtxtRelDefrd = TRUE;
                  WR_UMM_CPY_DEFR_MSG_TO_TRANSCB(transCb, incTrans->msg);       
               } 
               else
               {
                  ALARM("Maximum number of Derefred Message Execeding the Specified Limit"
                        "at  Defered processor\n");
               }
               RETVALUE(WR_UMM_TRANS_FAIL);
            }
            break;
         }
         /*RRC Rel10 Upgrade - end*/ 
      case WR_UMM_DL_NAS_TRANS: 
      case WR_UMM_DL_CDMA_TRANS: 
         {
            /* Naveen_def
             * Store the Incoming message to ueCb->defQ,
             * Increase the reference count in msg
             * Increase the UeCb->NumDefMsgCount
             * And return TRANS_FAIL to release the DL_NAS transaction
             * This incoming message will be processed once re-establishment trans
             * is completed. We will pick message one by one from ueCb->defQ and
             * call wrUmmPrcIncMsg(cellCb, ueCb, ueCb->defQ[count]) for 
             * ueCb->NumDefMsgCount times */
            if(transCb->numDefMsgCount < WR_UMM_DEF_LIST)
            {
               WR_UMM_CPY_DEFR_MSG_TO_TRANSCB(transCb, incTrans->msg);       
            } 
            else
            {
               ALARM("Maximum number of Derefred Message Execeding the Specified Limit"
                     "at  Defered processor\n");
            }
            RETVALUE(WR_UMM_TRANS_FAIL);
         } 
      case WR_UMM_MEAS_TRANS:
         {
            /* According to 36.331 Section 5.5.6.1 upon re-establishment accept
             * UE reset VarMeasReportList and sends any event type measurement reports
             * so, even though eNB drops event measurement reports post Re-establishment
             * eNB will get measurement report(s) if UE still meets the event entry criteria
             * with this approach, eNB is not acting on the old reports */
            if(incTrans->msg->msgTyp == WR_UMM_INTERNAL_MSG && transCb->numDefMsgCount < WR_UMM_DEF_LIST)
            {
               WR_UMM_CPY_DEFR_MSG_TO_TRANSCB(transCb, incTrans->msg);       
            } 
            else
            {
               RLOG0(L_WARNING,"Dropping the received measurement report \n");
            }
            RETVALUE(WR_UMM_TRANS_FAIL);
         }
         break;
         /*rel10*/
      case WR_UMM_ANR_MEAS_TRANS:
      case WR_UMM_EVENT_ANR_TRANS:
      case WR_UMM_ERAB_SETUP_TRANS:
      case WR_UMM_ERAB_MOD_TRANS:
      case WR_UMM_RAB_REL_TRANS:
         /*Removed WR_UMM_CTXT_REL_TRANS:*/
      case WR_UMM_UE_CTXT_MOD_TRANS:
         {
            if(transCb->numDefMsgCount < WR_UMM_DEF_LIST)
            {
               WR_UMM_CPY_DEFR_MSG_TO_TRANSCB(transCb, incTrans->msg);       
            } 
            else
            {
               ALARM("Maximum number of Derefred Message Execeding the Specified Limit"
                     "at  Defered processor\n");
            }
            RETVALUE(WR_UMM_TRANS_FAIL);
            break;
         }
      case WR_UMM_RAD_RES_MOD_TRANS:
         {
            RETVALUE(WR_UMM_TRANS_FAIL);
         }
         break;
      default:
         {
            RLOG1(L_ERROR,"Transaction %d received at wrUmmDefReestabIncTrans \n",
                  incTrans->transTyp);
            break;
         } 
   }   
   RETVALUE(WR_UMM_TRANS_OK);
}


/** @brief This function is used to Send Data Resume request
 *
 * @details
 *
 *     Function: wrUmmSendPdcpDataResumeReq
 *
 *         Processing steps:
 *     1) Call Eutra and UTRA API's
 *
 * @param[in]  ueCb: Ue control block
 * @return
 *    -#Success : WrUmmRedirectInfo*
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmSendPdcpDataResumeReq
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

     RLOG_ARG0(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,
      "Sending PDCP Data Resume request");

   WrIfmRrcSendPdcpDatResumeReq(nhuHdr);
   RETVALUE(ROK);
}

/** @brief This function does a check whether measurement gap needs to be configured.
 *
 * @details
 *
 *     Function: wrUmmRccReestabMeasGapChk 
 *
 *         Processing steps:
 *     1) Call Measurement Gap common API's to decide whether meas gap
 *       configuration required or not.
 *
 * @param[in]  transCb: transaction control block
 * @return
 *    -#Success : WrUmmRedirectInfo*
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmRccReestabMeasGapChk 
(
 WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb;
   WrUmmCellCb               *cellCb;


   ueCb = transCb->ueCb;
   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if(NULLP == cellCb)
   {
      RLOG0(L_ERROR, " cellCb is NULL ");
      RETVALUE(RFAILED);
   }

   /* The following function shall check the status of the measurement gap and 
    * configures the required paramters, if needs to upate the meas Gap */
   wrUmmMeasAddMeasGap(WR_UMM_MEAS_GAP_REEST, cellCb, ueCb, 
                                   &transCb->u.rrcReEstabTransCb.measGapStatus, 
                                   transCb->transId);
   RETVALUE(ROK); 
}

/** @brief This function is responsible for reconfiguring the UE context
 *         at the scheduler.
 *
 * @details
 *
 *     Function: wrUmmReEstSchdMeasGapCfgReq
 *
 *         Processing steps:
 *         - allocate a new configuration request
 *         - set the configuration type to RGR_UE_CFG
 *         - fill the ue info and ambr values
 *         - send the request to scheduler
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmReEstSchdMeasGapCfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrMeasUeCb                *measUeCb;
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   U32                        transId;

   measUeCb = transCb->ueCb->measCfg;
   if ( measUeCb == NULL ) 
   {
      RLOG0(L_ERROR, " measUeCb is NULL ");
      RETVALUE (RFAILED);
   }
   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   transId = transCb->transId | WR_UMM_RRC_REESTAB_MEAS_GAP_CFG_TRANSID;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   ueRecfg->ueRecfgTypes |= RGR_UE_MEASGAP_RECFG;
   ueRecfg->ueMeasGapRecfg.isMesGapEnabled = TRUE;
   if ( measUeCb->gapMeasInfo.gapPattern == WR_UMM_MEAS_GAP_PERD_40MS )
   {
      ueRecfg->ueMeasGapRecfg.gapPrd = WR_UMM_MEAS_GAP_PERD_40MS_VAL;
   }
   else if ( measUeCb->gapMeasInfo.gapPattern == WR_UMM_MEAS_GAP_PERD_80MS )
   {
      ueRecfg->ueMeasGapRecfg.gapPrd = WR_UMM_MEAS_GAP_PERD_80MS_VAL;
   }
   ueRecfg->ueMeasGapRecfg.gapOffst = measUeCb->gapMeasInfo.measGapOffset;
   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
} /* wrUmmReEstSchdMeasGapCfgReq */

PRIVATE Void wrUmmRrcReestabPerdStrongCellMeasCfg(
WrUmmTransCb  *transCb,
WrUmmCellCb    *cellCb,
NhuDatReqSdus  *recfgMsg,
NhuRRCConRecfgn_r8_IEs   *recfgIEs
)
{
   WrUeCb                      *ueCb = transCb->ueCb;
   WrMeasUeCbEutraMeasObjNode  *enbMeasObj;
   WrMeasUeCb                  *measUeCb;
   LwrCellMeasCfgGrp           *measCellCfg;
   WrUmmMeasCellCb             *measCellCb;
   WrUmmMeasAddInfo            addMeasInfo;
   U8                          measObjId;
   NhuMeasConfig               *nhuMeasCfg;
   WrUmmMeasTransInfo           measCfgInfo;
   
   measUeCb = ueCb->measCfg;
   measCellCfg = cellCb->measCellCfg;
   measCellCb = cellCb->measCellCb;
   enbMeasObj = &measUeCb->intraMeasObj;
   cmMemset((U8*)&addMeasInfo, 0, sizeof(WrUmmMeasAddInfo));
   cmMemset((U8*)&measCfgInfo, 0, sizeof(WrUmmMeasTransInfo));
  
   addMeasInfo.measObjLst[addMeasInfo.measObjIdCnt] = enbMeasObj->measObjId;
   addMeasInfo.measObjIdCnt++;
   measObjId = enbMeasObj->measObjId;

   /* Fill Measurement COnfiguration IE                                   */
   nhuMeasCfg  = &(recfgIEs->measConfig);

   wrUmmPerdStrongCellMeasCfg(ueCb, measObjId, measCellCfg, &addMeasInfo);
   measCfgInfo.addMeasInfo = addMeasInfo;
   /* Configuring Meas Objects and report configuration List*/
   if(ROK != wrMeasFillNhuMsg (&recfgMsg->memCp, nhuMeasCfg, ueCb, &measCfgInfo, measCellCfg, transCb->transId))
   {
      RLOG0(L_ERROR, "wrMeasFillNhuMsg failed");
   }
   RETVOID;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
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
