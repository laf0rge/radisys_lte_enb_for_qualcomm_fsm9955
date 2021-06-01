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
               

     File:     wr_umm_s1tgtho.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=72;
static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_emm.h"
#include "wr_umm_trans.h"
#include "wr_utils.h"
#include "wr_ifm_x2ap.h"
#include "wr_ifm_s1ap.h"
#include "wr_umm_ho.h"
#include "wr_umm_rrc_msg.h"
#include "wr_ifm_rrc.h"
#include "wr_ifm_schd.h"
#include "wr_smm_smallcell.h"
#include "wr_umm.h"
#include "wr_dam.h"
#include "wr_kpi.h"
#include "wr_umm_drx.h"
#include "wr_umm_sch_msg.h"
#include "wr_ifm_l1.h"
#include "wr_ifm_son.h"
#include "wr_umm_l1_msg.h"
#include "wr_umm_x2tgtho.h"
#include "wr_umm_meas_fun.h"
/* SPS changes starts */
#include "wr_ifm_rrm.h"
/* SPS changes ends */
#include "wr_emm_mme.h" /* UE History info */



/*
 *      Fun:  wrUmmTgtHoInitUeCb 
 *
 *      Desc:  Populate WrUeCb Structure
 *
 *      Ret:   ROK
 *
 *      Notes: None
 */


PUBLIC S16 wrUmmS1SecExtractSecKeys(WrUeSecCfg *secCfg,Bool  *asKeyGenerated,U32  transId);
PUBLIC S8 wrUmmSecExtractHOSecKeys(WrUmmS1TgtHoTrans *s1TgtHoTrans,WrCellId   cellId,U8  kdfType);
PUBLIC S16 wrUmmTgtHoInitUeCb(WrUeCb *ueCb);
PUBLIC S16 wrUmmS1TgtHoRelocTmrExp (WrUeCb *ueCb);
PUBLIC S16 wrUmmS1TgtHoFillRrcEvntHdr (NhuHdr *nhuHdr, U16 cellId, U16 crnti, U32 transId);

EXTERN S16 wrEmmMmeSztFillPLMNId(S1apPdu  *pdu,WrPlmnId  *plmnId,
                  SztPLMNidentity  *plmnIe);
EXTERN S16 wrEmmMmeSztFillTAC(U16 tac,S1apPdu  *pdu,SztTAC  *tAC);
EXTERN S16 wrUmmSecGetSecCap(SztUESecurCapabilities   *ueSecCap,WrUeSecCfg   *secCfg);
EXTERN S16 wrUmmHoFillRadioResCfg (RmuUeRadioResCfg   *ueRadioResCfg, NhuHovrPrepInfo    *nhuHovrPrepInfo);
EXTERN S16 wrUmmHoCopyRadioResCfg (RmuUeRadioResCfg   *rmuUeRadioResCfg, RmuUeRadioResCfg   *txsUeRadioResCfg);
EXTERN S16 wrIfmRrmHoAdmitReq (RmuUeHoReq *rrmWrUeHoAdmitReq);
#ifdef TENB_AS_SECURITY
EXTERN S16 wrPlatS1SecExtractSecKeys (WrUeSecCfg *secCfg, Bool *asKeyGenerated,
      U32 transId);
EXTERN S16 wrPlatSecExtractSecKeys1(WrUeCb *ueCb,U32 transId,U8 keyDerType);
#endif

EXTERN S16 wrEmmGetTai
(
WrUeCb *ueCb,
WrTai  *tai
);
EXTERN S8 wrUmmSecExtractHOSecKeys(WrUmmS1TgtHoTrans *,WrCellId, U8);

PUBLIC S16 wrUmmTgtHoInitUeCb 
(
WrUeCb                       *ueCb
)
{

   wrUmmInitRrcTrans(ueCb);
   ueCb->ueCat                             = CM_LTE_UE_CAT_4;
   /*ccpu132995*/
   ueCb->establishCause                    = RMU_EST_CAUSE_HO_REQ;

   wrUmmInitUeCb(ueCb);

   cmInitTimers(&ueCb->s1TgtEnbRelocTmr.tmr,1);

   RETVALUE(ROK);
}/*wrUmmTgtHoInitUeCb*/



/**
  * This function is used to fill ERAB To Be Setup List.
  */
PRIVATE S16 wrUmmS1TgtHoFillERABsToBeSetupLst
(
WrUmmTransCb                 *transCb, 
SztE_RABToBeSetupLstHOReq    *sztE_RABToBeSetupLstHOReq
)
{
   WrUmmCellCb                                     *cellCb;
   WrUmmS1TgtHoTrans                               *s1TgtHoTrans = NULLP;
   SztE_RABToBeSetupItemHOReq                      *sztE_RABToBeSetupItemHOReq = NULLP;
   SztProtIE_SingleCont_E_RABToBeSetupItemHOReqIEs *hoReqIes = NULLP;
   SztGBR_QosInform                                *gbrQosInform = NULLP;
   SztAllocnAndRetentionPriority                   *allocRetPrior = NULLP;
   WrUmmS1TgtHoRabInfo                             *rbInfo = NULLP;
   SztTportLyrAddr                                 *tportLyrAddr = NULLP;
   U8                                              rbDir;
   U8                                              shiftBits = 0;
   U8                                              eRabSetupLstIdx;
   U16                                             idx = 0;
   U16                                             indx = 0;
   U32                                             addrMask = 0;
   U32                                             dlBitRate = 0;
   U32                                             ulBitRate = 0;
   U32                                             noOfHoReqComp = 0;
   s1TgtHoTrans = &transCb->u.s1TgtHoTrans;
   WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId);
   if (cellCb == NULLP)
   {
      /* Log the error through an error debug */
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)transCb->ueCb->cellId,
         "Error in getting CELL CB");
      s1TgtHoTrans->hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW; 
      s1TgtHoTrans->hoPrepFailCause.causeVal = 
         SztCauseRadioNwcell_not_availableEnum;
      RETVALUE(RFAILED);
   }
   noOfHoReqComp = sztE_RABToBeSetupLstHOReq->noComp.val;
   WR_ALLOC(&s1TgtHoTrans->rbInfo,noOfHoReqComp * sizeof(WrUmmS1TgtHoRabInfo));
   if (NULLP == s1TgtHoTrans->rbInfo)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
     RETVALUE(RFAILED);
   }/* End of if condition.*/
   
   rbInfo = s1TgtHoTrans->rbInfo;
   s1TgtHoTrans->numOfRbCfgd = noOfHoReqComp;
   for (eRabSetupLstIdx = 0; eRabSetupLstIdx < noOfHoReqComp; eRabSetupLstIdx++)
   {
      hoReqIes = &sztE_RABToBeSetupLstHOReq->member[eRabSetupLstIdx];
      sztE_RABToBeSetupItemHOReq = &hoReqIes->value.u.sztE_RABToBeSetupItemHOReq;
      /* FIll E-RAB ID */
      rbInfo[eRabSetupLstIdx].drbInfo.eRABId = 
                                       sztE_RABToBeSetupItemHOReq->e_RAB_ID.val;
		rbInfo[eRabSetupLstIdx].rabAllowed = FALSE;
		/* Transport Layer Address.TODO */
      tportLyrAddr = &sztE_RABToBeSetupItemHOReq->transportLyrAddr;
      switch(tportLyrAddr->len)
      {
         case 32:
            {
               rbInfo[eRabSetupLstIdx].drbInfo.sgwAddr.type = CM_TPTADDR_IPV4;
               rbInfo[eRabSetupLstIdx].drbInfo.sgwAddr.u.ipv4TptAddr.port = WR_DFLT_EGTP_PORT;
               /* copy 4bytes into the U32 */
               shiftBits = tportLyrAddr->len / 8;
               addrMask = 0xFF000000;
               for(indx = 0; indx < 4; indx++)
               {
                  shiftBits--;
                  rbInfo[eRabSetupLstIdx].drbInfo.sgwAddr.u.ipv4TptAddr.address |=
                     ((U32)(tportLyrAddr->val[indx]
                        << (8 * shiftBits)) & addrMask);
                  addrMask = addrMask >> 8;
               }
               break;
            }
         case 128:
            {
               rbInfo[eRabSetupLstIdx].drbInfo.sgwAddr.type = CM_TPTADDR_IPV6;
               rbInfo[eRabSetupLstIdx].drbInfo.sgwAddr.u.ipv6TptAddr.port = WR_DFLT_EGTP_PORT;
               for (indx = 0; indx < 16; indx++)
               {
                  rbInfo[eRabSetupLstIdx].drbInfo.sgwAddr.u.ipv6TptAddr.ipv6NetAddr[indx]=
                     tportLyrAddr->val[indx];
               }
               break;
            }
         default:
            {
               /* This means both IPv4 and IPv6 addresses are present. We are */
               /* yet to support this option */
               s1TgtHoTrans->hoPrepFailCause.causeTyp = WR_CAUSE_PROTOCOL;
               s1TgtHoTrans->hoPrepFailCause.causeVal = 
             SztCauseProtmessage_not_compatible_with_receiver_stateEnum; 
               RETVALUE(RFAILED);
            }
      }
      /* FILL GTP-TEID */
      shiftBits = sztE_RABToBeSetupItemHOReq->gTP_TEID.len;
      addrMask = 0xFF000000;
      for(idx = 0;idx < sztE_RABToBeSetupItemHOReq->gTP_TEID.len;idx++)
      {
         shiftBits--;
         rbInfo[eRabSetupLstIdx].drbInfo.remEgtpTunId |=
            ((U32)(sztE_RABToBeSetupItemHOReq->gTP_TEID.val[idx] << (8*shiftBits)) & addrMask);
         addrMask = addrMask>>8;
      }
      /* Fill QCI Value.*/
      rbInfo[eRabSetupLstIdx].drbInfo.qci = 
      sztE_RABToBeSetupItemHOReq->e_RABlevelQosParams.qCI.val;
      /* Fill rbMode.*/
      wrUtlGetRbMode(transCb->ueCb->cellId, rbInfo[eRabSetupLstIdx].drbInfo.qci,
                        &(rbInfo[eRabSetupLstIdx].drbInfo.rbMode));
      /* Fill rbDir.*/
      wrUtlGetRbDir(transCb->ueCb->cellId, rbInfo[eRabSetupLstIdx].drbInfo.qci,&rbDir);
      rbInfo[eRabSetupLstIdx].drbInfo.rbDir = rbDir;
		/* Fill Allocation And Retention Priority.*/
      allocRetPrior = 
         &sztE_RABToBeSetupItemHOReq->e_RABlevelQosParams.allocationRetentionPriority;
      if(allocRetPrior->pres.pres == PRSNT_NODEF)
      {
         /* Allocation and Retention priority is present */
         rbInfo[eRabSetupLstIdx].drbInfo.priority = 
            allocRetPrior->priorityLvl.val;
         rbInfo[eRabSetupLstIdx].drbInfo.preempCap = 
            allocRetPrior->pre_emptionCapblty.val;
         rbInfo[eRabSetupLstIdx].drbInfo.preempVul =
            allocRetPrior->pre_emptionVulnerability.val;

         if(rbInfo[eRabSetupLstIdx].drbInfo.priority == cellCb->rabArpForEmerServ)
         {
            /* Its as emergency bearer */
            rbInfo[eRabSetupLstIdx].drbInfo.isEmergencyBearer = TRUE;
            transCb->ueCb->ueServTyp = WR_UMM_EMERGENCY_SERV;
            /* HCSG DEV */
            s1TgtHoTrans->isEmrgncyCall = TRUE;
         }
      }
      /* GBR QOS Parameters.*/
      /* MBR DL.*/
      gbrQosInform = 
         &sztE_RABToBeSetupItemHOReq->e_RABlevelQosParams.gbrQosInform;
      wrGetBitRate(&gbrQosInform->e_RAB_MaxBitrateDL, &dlBitRate);
      rbInfo[eRabSetupLstIdx].drbInfo.mbr.dl = dlBitRate;
      wrGetBitRate(&gbrQosInform->e_RAB_MaxBitrateUL, &ulBitRate);
      rbInfo[eRabSetupLstIdx].drbInfo.mbr.ul = ulBitRate;
      wrGetBitRate(&gbrQosInform->e_RAB_GuaranteedBitrateDL, &dlBitRate);
      rbInfo[eRabSetupLstIdx].drbInfo.gbr.dl = dlBitRate;
      wrGetBitRate(&gbrQosInform->e_RAB_GuaranteedBitrateUL, &ulBitRate);
      rbInfo[eRabSetupLstIdx].drbInfo.gbr.ul = ulBitRate;
      /* Data Forwarding List.*/
      rbInfo[eRabSetupLstIdx].drbInfo.isDataFwdAvl = TRUE;
      if(sztE_RABToBeSetupItemHOReq->iE_Extns.noComp.val != 0)
      {
         if(sztE_RABToBeSetupItemHOReq->iE_Extns.member->extensionValue.u.sztData_Fwding_Not_Possible.val
               == SztData_Fwding_Not_Possibledata_Fwding_not_PossibleEnum)
         {
            rbInfo[eRabSetupLstIdx].drbInfo.isDataFwdAvl = FALSE;
         }
      }

      /* HO_RLC_PDCP_CFG */
      /* create a copy of PDCP RLC configurations */
      cmMemcpy((U8 *)&(rbInfo[eRabSetupLstIdx].srcPdcpRlcCfgParams), 
            (CONSTANT U8 *)&cellCb->wrRabCfgParams[rbInfo[eRabSetupLstIdx].drbInfo.qci],
            sizeof(WrRabCfgParams));
   }
   RETVALUE(ROK);
}
/**
  * This function is used to fill UE Security Capabilities.
  */
PRIVATE S16 wrUmmS1TgtHoFillUeSecurCapabilities
(
WrUmmTransCb                 *transCb, 
SztUESecurCapabilities       *UESecurCapabilities
)
{
   WrUmmS1TgtHoTrans        *s1TgtHoTrans = NULLP;

   s1TgtHoTrans = &transCb->u.s1TgtHoTrans;
   /* Allocate the Memory.*/
   WR_ALLOC(&s1TgtHoTrans->secCfg, sizeof(WrUeSecCfg));
   if (s1TgtHoTrans->secCfg == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of s1TgtHoTrans.secCfg == NULLP*/
   /* store the UE capability info */
   /* ccpu00126532 */
   if(wrUmmSecGetSecCap(UESecurCapabilities, s1TgtHoTrans->secCfg) != ROK)
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, transCb->ueCb->crnti,"processing of secIE"
         "failed");
      s1TgtHoTrans->hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW; 
      s1TgtHoTrans->hoPrepFailCause.causeVal = 
         SztCauseRadioNwencryption_and_or_integrity_protection_algorithms_not_supportedEnum;
       RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/* End of wrUmmS1TgtHoFillUeSecurCapabilities*/


PRIVATE S16 wrUmmS1TgtHoFillUeAggMaxBitrate
(
WrUmmTransCb                 *transCb,
SztUEAggMaxBitrate           *sztUEAggMaxBitrate
)
{
   U32                                dlBitRate;
   U32                                ulBitRate;

   /* Fill Ue Aggregate Max Bitrate DL */
   wrGetBitRate(&sztUEAggMaxBitrate->uEaggregateMaxBitRateDL, &dlBitRate);
   transCb->u.s1TgtHoTrans.ambr.dl = dlBitRate;
   /* Fill Ue Aggregate Max Bitrate UL */
   wrGetBitRate(&sztUEAggMaxBitrate->uEaggregateMaxBitRateUL, &ulBitRate);
   transCb->u.s1TgtHoTrans.ambr.ul = ulBitRate;

   if((0 == dlBitRate) && (0 == ulBitRate))
   {
      RLOG0(L_ERROR, "Invlaid AMBR value received, sending failure");
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp = WR_CAUSE_PROTOCOL;
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal = 
                                             SztCauseProtsemantic_errorEnum;
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
 *  This function is used to fill Cause.
 *
 */

PRIVATE S16 wrUmmS1TgtHoFillS1Cause
(
WrUmmTransCb                 *transCb,
SztCause                     *cause
)
{
   /* Filling Cause Values into Transaction Control Block.*/
   /* Allocating Memory*/
   WR_ALLOC(&transCb->u.s1TgtHoTrans.sztCause, sizeof(SztCause));
   if (NULLP == transCb->u.s1TgtHoTrans.sztCause)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/*End of transCb->u.s1TgtHoTrans.sztCause.*/
   transCb->u.s1TgtHoTrans.sztCause->choice.val = cause->choice.val;
   switch (cause->choice.val)
   {
      case WR_CAUSE_RADIONW:
         /* Fill Radio Network Cause value*/
         transCb->u.s1TgtHoTrans.sztCause->val.radioNw.val = cause->val.radioNw.val;
         break;
      case WR_CAUSE_TRANSPORT:
         /* Fill Transport Cause value*/
         transCb->u.s1TgtHoTrans.sztCause->val.transport.val = cause->val.transport.val;
         break;
      case WR_CAUSE_PROTOCOL:
         /* Fill Protocol Cause value*/
         transCb->u.s1TgtHoTrans.sztCause->val.protocol.val = cause->val.protocol.val;
         break;
      case WR_CAUSE_MISC:
         /* Fill MISC Cause Value*/
         transCb->u.s1TgtHoTrans.sztCause->val.misc.val = cause->val.misc.val;
         break;
      default:
         {
            RLOG0(L_ERROR, "Invalid Cause");
            transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp = WR_CAUSE_PROTOCOL;
            transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal = 
               SztCauseProtsemantic_errorEnum;
            RETVALUE(RFAILED);
         }
         break;
   }/* End of switch*/
   RETVALUE(ROK);
}/* End of wrUmmS1TgtHoFillS1Cause */

/**
 * This Function is used to Decode Source to Target Transparent Container.
 */
PRIVATE S16 wrUmmS1TgtHoDecSrcToTgtCont
(
WrUmmTransCb                 *transCb,
SztSrc_ToTget_TprntCont      *sztSrc_ToTget_TprntCont
)
{
   SztEncDecEvnt             sztEncDecEvnt;
   Buffer                    *trnspContMBuf = NULLP;

   sztEncDecEvnt.transId     = transCb->transId;
   sztEncDecEvnt.type.pres   = PRSNT_NODEF;
   sztEncDecEvnt.type.val    = SZT_REQ_TYPE_DECODE_IE;
   sztEncDecEvnt.ieType.pres = PRSNT_NODEF;
   sztEncDecEvnt.ieType.val  = SZT_REQ_TYPE_ENCDEC_IE_SRCTGET;
   /* Alloc memory for mBuf */
   SGetMsg(wrCb.mem.region, wrCb.mem.pool, &trnspContMBuf);
   if(trnspContMBuf == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* convert the OSXL to Buffer */
   SAddPstMsgMult(sztSrc_ToTget_TprntCont->val,
         sztSrc_ToTget_TprntCont->len, trnspContMBuf);
   sztEncDecEvnt.u.mBuf = trnspContMBuf;
   WrIfmS1apEncDecReq(&sztEncDecEvnt);

   RETVALUE(ROK);
}

/*
 * This Function is used to parse Handover Request Message.
 */

PRIVATE S16 wrUmmS1TgtHoParseHoReqMsg
(
WrUmmTransCb                 *transCb  
)
{
   U8                            noOfHoReqIes;
   U8                            hoReqIesIdx;
#ifndef TENB_AS_SECURITY
   U16                           nhIdx;
   U16                           len;
#else
   TknStrBSXL                    *secKey;
#endif
   WrUmmS1TgtHoTrans             *s1TgtHoTrans = NULLP;
   S1apPdu                       *s1apPdu = NULLP;
   SztInitiatingMsg              *initiatingMsg = NULLP;
   SztProtIE_Field_HovrRqstIEs   *hoReqIe = NULLP;    
   /*Fix for ccpu00123539  */
   WrUmmCellCb                   *cellCb = NULLP;
   Bool                          ret = FALSE;

   s1TgtHoTrans = &transCb->u.s1TgtHoTrans;
   /* Getting the incoming HO Reqest message PDU */
   s1apPdu = transCb->msg->u.s1Pdu->pdu;
   initiatingMsg = &s1apPdu->pdu.val.initiatingMsg;

   WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)transCb->ueCb->cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   RLOG0(L_DEBUG, "Parsing HO Reqest Message");

   /* T2200_HO_FIX: Initializing the Location reporting request type here. 
    * It is done here as if we have the specific IE, it will be over written
    * below while parsing the IEs. Here 0xFF is considered as invalid value
    */
   s1TgtHoTrans->locReptReqTyp = 0xFF;

   if (cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS)
   {
      s1TgtHoTrans->ueMbrShpSts = WR_UMM_CSG_MEMBERSHIP_NOT_KNOWN;
   }
   else
   {
      /* membership true for closed and open access */
      s1TgtHoTrans->ueMbrShpSts = WR_UMM_CSG_MEMBER;
   }

   noOfHoReqIes = initiatingMsg->value.u.sztHovrRqst.protocolIEs.noComp.val;
   for (hoReqIesIdx = 0; hoReqIesIdx < noOfHoReqIes; hoReqIesIdx++)
   {
      hoReqIe = &initiatingMsg->value.u.sztHovrRqst.protocolIEs.member[hoReqIesIdx];
      if (NULLP == hoReqIe)
      {
         RLOG0(L_ERROR, "There are No IEs present");
         s1TgtHoTrans->hoPrepFailCause.causeTyp = WR_CAUSE_PROTOCOL;
         s1TgtHoTrans->hoPrepFailCause.causeVal = 
            SztCauseProtabstract_syntax_error_rejectEnum;
         RETVALUE(RFAILED);
      }/* End of if condition.*/
      switch (hoReqIe->id.val)
      {
         case Sztid_MME_UE_S1AP_ID:
            {
               s1TgtHoTrans->mme_ue_s1ap_id = 
                  hoReqIe->value.u.sztid_MME_UE_S1AP_ID.val;
               break;
            }
         case Sztid_HovrTyp:
            {
               s1TgtHoTrans->sztHovrTyp = hoReqIe->value.u.sztHovrTyp.val;
               break;
            }
         case Sztid_Cause:
            {
               wrEmmUpdateKpiCounter(&hoReqIe->value.u.sztCause);

               if (ROK != wrUmmS1TgtHoFillS1Cause(transCb,
                        &hoReqIe->value.u.sztCause))
               {         
                  RLOG0(L_ERROR, "Failed to Fill S1 Ho Cause");
                  RETVALUE(RFAILED);
               }
               break;
            }
         case Sztid_uEaggregateMaxBitrate:
            {
               if (ROK != wrUmmS1TgtHoFillUeAggMaxBitrate(transCb,
                        &hoReqIe->value.u.sztUEAggMaxBitrate))
               {
                  RLOG0(L_ERROR, "Failed to Fill Ue Aggregate Max Bitrate");
                  RETVALUE(RFAILED);
               }
               break;
            }
         case Sztid_E_RABToBeSetupLstHOReq:
            {
               if (ROK != wrUmmS1TgtHoFillERABsToBeSetupLst(transCb,
                        &hoReqIe->value.u.sztE_RABToBeSetupLstHOReq))
               {
                  RLOG0(L_ERROR, "Failed to Fill ERABs To Be Setup List");
                  RETVALUE(RFAILED);
               }
               break;
            }
         case Sztid_Src_ToTget_TprntCont:
            {
               /**Decode Source To Target Transparent Container*/
               if (ROK != wrUmmS1TgtHoDecSrcToTgtCont(transCb, 
                        &hoReqIe->value.u.sztSrc_ToTget_TprntCont))
               {
                  RLOG0(L_ERROR, "Failed to Fill ERABs To Be Setup List");
                  s1TgtHoTrans->hoPrepFailCause.causeTyp = WR_CAUSE_PROTOCOL;
                  s1TgtHoTrans->hoPrepFailCause.causeVal = SztCauseProttransfer_syntax_errorEnum;
                  RETVALUE(RFAILED);
               }
               break;
            }
         case Sztid_UESecurCapabilities:
            {
               /* Fill UE Security Capabilities.*/
               if (ROK != wrUmmS1TgtHoFillUeSecurCapabilities(transCb, 
                        &(hoReqIe->value.u.sztUESecurCapabilities)))
               {
                  RLOG0(L_ERROR, "wrUmmS1TgtHoFillUeSecurCapabilities:Fld "
                        "UE Sec Capblty");
                  /* ccpu00137544        */
                  s1TgtHoTrans->hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW;
                  s1TgtHoTrans->hoPrepFailCause.causeVal =
                     SztCauseRadioNwencryption_and_or_integrity_protection_algorithms_not_supportedEnum;
                  RETVALUE(RFAILED);
               }/* End of wrUmmS1TgtHoFillUeSecurCapabilities*/
               break;
            }
            /*MME_LDBAL*/
            case Sztid_HovrRestrnLst: 
            {        
               SztHovrRestrnLst *sztHoRestrnLst;

               sztHoRestrnLst =  &hoReqIe->value.u.sztHovrRestrnLst;
               wrUmmCpyHoRestrnLst(transCb->ueCb,sztHoRestrnLst); 
               /* Check if the target cell supports the serving PLMN
                * specified in the HRL or not. */
               if(transCb->ueCb->ueServTyp == WR_UMM_NORMAL_SERV)
               {
                  ret = wrEmmChkServPlmnHoUe(cellCb->cellId, 
                        transCb->ueCb->hoRstrctInfo->srvngPlmn);  
                  if(ret == FALSE)
                  {
                     /* Serving PLMN is not supported by the eNodeB */
                     s1TgtHoTrans->hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW;
                     s1TgtHoTrans->hoPrepFailCause.causeVal = 
                        SztCauseRadioNwho_target_not_allowedEnum;

                     RETVALUE(RFAILED);
                  }
               }
               break;
            }
         case Sztid_TraceActvn:
            /* TODO*/
            break;
         case Sztid_RqstTyp:
            /* ccpu00131919 */
            if(hoReqIe->value.u.sztRqstTyp.pres.pres == PRSNT_NODEF)
            {
               switch(hoReqIe->value.u.sztRqstTyp.eventTyp.val)
               { 
                  case SztEventTypchange_of_serve_cellEnum:
                     {
                        /* Set the LocRpt flag to TRUE */
                        transCb->ueCb->locReportType = TRUE;
                        break;
                     }
                  case SztEventTypdirectEnum:
                     {
                        /* Build and send the location report message to MME */
                        wrUmmBldSndLocRept(transCb);               
                        break;
                     }
                  case SztEventTypstop_change_of_serve_cellEnum:
                     {
                        /* Set the LocRpt flag to False for Evnt stop change of ServCell */
                        transCb->ueCb->locReportType = FALSE;         
                        break;      
                     }
                  default:
                     break;
               }
            }
            break;
         case Sztid_SecurCntxt:
            {
               /* 
                * The eNB shall use this security Context for the next X2 
                *handover or Intra eNB Handover.
                */
               s1TgtHoTrans->new_nextHopChainingCount = 
                  hoReqIe->value.u.sztSecurCntxt.nextHopChainingCount.val;
               /* ccpu00126532 */
#ifdef TENB_AS_SECURITY
               secKey = &hoReqIe->value.u.sztSecurCntxt.nextHopParameter;
               /* storing the key received from MME                       */
               cmMemcpy((U8*)&s1TgtHoTrans->secCfg->kENB[0],
                     (U8*)&secKey->val[0], WR_KENB_KEY_LEN);
               transCb->ueCb->secCfg = s1TgtHoTrans->secCfg;
               /*ccpu00129252*/
               if(s1TgtHoTrans->secCfg->intgAlgo != WR_UMM_NULL_ALGO)
               {

                
                  if ( SztHovrTypintralteEnum == 
                        transCb->u.s1TgtHoTrans.sztHovrTyp)
                  {
#if  !(defined SS_RBUF) && !(defined LTE_QCOM)
               wrPlatSecExtractSecKeys1(transCb->ueCb,
                     transCb->transId, CTF_KDF_TYPE2);
#else
                     /* Now do veritcal key derivation for kENB* */
                     wrUmmSecExtractHOSecKeys(s1TgtHoTrans,transCb->ueCb->cellId,CTF_KDF_TYPE2);
                     /* First calculate the 3 security vectors for kENB */
                     wrUmmSecExtractHOSecKeys(s1TgtHoTrans,transCb->ueCb->cellId,CTF_KDF_TYPE3);
                     /* Do horizontal key derivation which will be usd for re-establishment */
                     wrUmmSecExtractHOSecKeys(s1TgtHoTrans,transCb->ueCb->cellId,CTF_KDF_TYPE5);
                     /* Lastly, generate 3 security vectors for horizontal kENB*/
                     wrUmmSecExtractHOSecKeys(s1TgtHoTrans,transCb->ueCb->cellId,CTF_KDF_TYPE4);
                     transCb->ueCb->hzSecCfg = s1TgtHoTrans->hzSecCfg;
#endif
                  }
                  else
                  {
                     /* In case of IRAT HO, as per spec 36.413, section 9.2.1.26
                        For inter-RAT Handover into LTE the Next Hop Chaining
                        Count IE takes the value defined for NCC at
                        initial setup, i.e. Next Hop Chaining Count IE = "0".
                        Next-Hop NH IE is the KeNB to be used in the new
                        configuration */
#if  !(defined SS_RBUF) && !(defined LTE_QCOM)
                    s1TgtHoTrans->isNewKenbGenerated = TRUE;
                    wrPlatS1SecExtractSecKeys(s1TgtHoTrans->secCfg,
                           &s1TgtHoTrans->asKeyGenerated, transCb->transId);
#else
                     /* First calculate the 3 security vectors for kENB */
                    wrUmmSecExtractHOSecKeys(s1TgtHoTrans,transCb->ueCb->cellId,CTF_KDF_TYPE3);
                    /* Do horizontal key derivation which will be usd for re-establishment */
                    wrUmmSecExtractHOSecKeys(s1TgtHoTrans,transCb->ueCb->cellId,CTF_KDF_TYPE5);
                     /* Lastly, generate 3 security vectors for horizontal kENB*/
                    wrUmmSecExtractHOSecKeys(s1TgtHoTrans,transCb->ueCb->cellId,CTF_KDF_TYPE4);
                     transCb->ueCb->hzSecCfg = s1TgtHoTrans->hzSecCfg;
#endif

                  }

               }
               else
               {
                  s1TgtHoTrans->asKeyGenerated = TRUE;
               }
#else
               len = hoReqIe->value.u.sztSecurCntxt.nextHopParameter.len;
               WR_ALLOC(&(s1TgtHoTrans->nextHop_NH.val),(len * sizeof(U8)));
               if (NULLP == s1TgtHoTrans->nextHop_NH.val)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  RETVALUE(RFAILED);
               }
               for (nhIdx = 0; nhIdx < len; nhIdx++)
               {
                  s1TgtHoTrans->nextHop_NH.val[nhIdx] = 
                     hoReqIe->value.u.sztSecurCntxt.nextHopParameter.val[nhIdx];
               }
               s1TgtHoTrans->nextHop_NH.len = len;
#endif
               break;
            }
         case Sztid_NASSecurParamstoE_UTRAN:
            /* For Inter-RAT Purpose.*/
            {
               U8                 idx = 0;
               TknStrOSXL         *nasSecToEutran;
               TknStrOSXL         *sztNasSecToEutran;

               nasSecToEutran = &s1TgtHoTrans->u.iRatHoInfo.nasSecParamToEutran;
               sztNasSecToEutran = &hoReqIe->value.u.sztNASSecurParamstoE_UTRAN;
               nasSecToEutran->pres = PRSNT_NODEF;
               nasSecToEutran->len = sztNasSecToEutran->len;
             
               WR_ALLOC(&nasSecToEutran->val, nasSecToEutran->len);
               if (nasSecToEutran->val == NULLP)
               {
                  RLOG0(L_FATAL,"Memory allocation failed.");
                  RETVALUE(RFAILED);
               }
               for(; idx < nasSecToEutran->len; idx++)
               {
                  nasSecToEutran->val[idx] = sztNasSecToEutran->val[idx];
               }
               break;
            }
         case Sztid_CSG_Id:
            {
               s1TgtHoTrans->csgId.pres = PRSNT_NODEF;
               wrUtlGetU32FrmBStr32(&s1TgtHoTrans->csgId.val, 
                     &hoReqIe->value.u.sztCSG_Id);
               /* Fix for ccpu00123539 - moved to CSG validation function */
               break;
            }

         case Sztid_CSGMembershipStatus:
            {
               if(SztCSGMembershipStatusmemberEnum == 
                     hoReqIe->value.u.sztCSGMembershipStatus.val)
               {
                  s1TgtHoTrans->ueMbrShpSts = WR_UMM_CSG_MEMBER;
               }
               else
               {
                  s1TgtHoTrans->ueMbrShpSts = WR_UMM_CSG_NOT_MEMBER;
               }
            }
            break;

         case Sztid_GUMMEI_ID:
         {
            SztGUMMEI        *gummeiIe = NULLP;
            gummeiIe = &hoReqIe->value.u.sztGUMMEI;
            s1TgtHoTrans->gummei.pres = TRUE;
            wrUtlGetPlmnId(&s1TgtHoTrans->gummei.plmnId,
                  &gummeiIe->pLMN_Identity);

            WR_MEM_COPY(&s1TgtHoTrans->gummei.mmeGrpId,\
                  &gummeiIe->mME_Group_ID.val,\
                  gummeiIe->mME_Group_ID.len);

            WR_MEM_COPY(&s1TgtHoTrans->gummei.mmec, gummeiIe->mME_Code.val,\
                  gummeiIe->mME_Code.len);

            RLOG_ARG2(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"Source MME "
               "information for mmec[%d] mmeGrpId[%d]",
               s1TgtHoTrans->gummei.mmec, s1TgtHoTrans->gummei.mmeGrpId);

               /* Check if the target cell supports the serving PLMN
                * specified in the GUMMEI in case HRL is not received. */
               if((transCb->ueCb->hoRstrctInfo == NULLP) && \
                     (transCb->ueCb->ueServTyp == WR_UMM_NORMAL_SERV))
               {
                  ret = wrEmmChkServPlmnHoUe(cellCb->cellId, 
                        s1TgtHoTrans->gummei.plmnId);  
                  if(ret == FALSE)
                  {
                     /* Serving PLMN is not supported by the eNodeB */
                     s1TgtHoTrans->hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW;
                     s1TgtHoTrans->hoPrepFailCause.causeVal = 
                        SztCauseRadioNwho_target_not_allowedEnum;

                     RETVALUE(RFAILED);
                  }
               }
            break;
         }
         case Sztid_MME_UE_S1AP_ID_2:
            {
               wrFillTknU32(&s1TgtHoTrans->mmeUeS1apId2,
                     hoReqIe->value.u.sztid_MME_UE_S1AP_ID_2.val);
               RLOG_ARG1(L_DEBUG, DBG_CRNTI, transCb->ueCb->crnti,"MME UE S1AP "
                     "ID 2 :id[%ld]", s1TgtHoTrans->mmeUeS1apId2.val);
               break;
            }
         /* Store the MDT allowed value in the UECB and the same will be sent 
          * to target eNodeB in case of X2 Based handover. 
          */
         case Sztid_ManagementBasedMDTAllowed:
         {
            transCb->ueCb->mgmtBasedMDTAllowed.pres = TRUE;
            transCb->ueCb->mgmtBasedMDTAllowed.val = 
                            hoReqIe->value.u.sztManagementBasedMDTAllowed.val;
         }
         break;

         case Sztid_SRVCCOperationPossible:
         {
            if((hoReqIe->value.u.sztSRVCCOperationPossible.pres == TRUE) &&
               (hoReqIe->value.u.sztSRVCCOperationPossible.val == 
                SztSRVCCOperationPossiblepossibleEnum))
            {
               transCb->ueCb->srvccOperPossible = TRUE;
            }
         }
         break;

         default :
{
            RLOG1(L_ERROR, "Invalid IE Received. ie id [%lu]", hoReqIe->id.val);
            s1TgtHoTrans->hoPrepFailCause.causeTyp = WR_CAUSE_PROTOCOL; 
            s1TgtHoTrans->hoPrepFailCause.causeVal = 
          SztCauseProtabstract_syntax_error_falsely_constructed_messageEnum;
            RETVALUE(RFAILED);
}
            break;
      }/* End of switch ie->id.val */
   }/* End of for loop */

   /* Admit the UE only if the serving plmn of the UE can be identified.
    * If both HRL and GUMMEI is not received, then identify the serving plmn
    * of the UE as below */
   if((transCb->ueCb->hoRstrctInfo == NULLP) && (s1TgtHoTrans->gummei.pres == FALSE))
   {
      U8       numPlmn = 0;

      /* Check the nos. of PLMNs supported by the eNodeB. */
      numPlmn = wrEmmGetNosOfBdcstPlmn(transCb->ueCb->cellId);
      if(numPlmn > 1)
      {
         /* Check the nos. of PLMNs supported by the eNodeB. */
         WrMmeCb      *mmeCb = NULLP;
         mmeCb = wrEmmCb.mmeCont.mmes[transCb->ueCb->mmeId];
         if((mmeCb != NULLP) && (mmeCb->numPlmnIds > 1))
         {
            /* Serving PLMN cannot be determined */
            s1TgtHoTrans->hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW;
            s1TgtHoTrans->hoPrepFailCause.causeVal = 
               SztCauseRadioNwho_target_not_allowedEnum;

            RETVALUE(RFAILED);
         }
      }
   }

   RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti, "[S1AP]:Received HANDOVER REQUEST "
            "[MME-UE-S1AP-ID:%lu] [HOType:%d] ",s1TgtHoTrans->mme_ue_s1ap_id,
            s1TgtHoTrans->sztHovrTyp);

   RETVALUE(ROK);
}/* End of wrUmmS1ParseHoReqMsg*/

/**
  * This function is used to prepare Target to source transparent Container.
  */
PRIVATE S16 wrUmmS1TgtHoGetLchDrbIds
(
WrUmmTransCb                 *transCb
)
{
/* ccpu00128207 */
   U8                        bearerIdx = 0;
   U8                        numOfRbCfgd = 0;
   U8                        lchId;
   U8                        drbId;
   WrUmmS1TgtHoRabInfo       *rbInfo;

   numOfRbCfgd = transCb->u.s1TgtHoTrans.numOfRbCfgd;
   /* Need To Reserve LCH ID and RB ID for every bearer.*/
   for  (bearerIdx = 0; bearerIdx < numOfRbCfgd; bearerIdx++)
   {
      rbInfo = &transCb->u.s1TgtHoTrans.rbInfo[bearerIdx];

      /* HO_RLC_PDCP_CFG */
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
}/* End of wrUmmS1TgtHoGetLchDrbIds*/


/*  SRB Configuration.*/
PRIVATE S16 wrUmmS1TgtHoSchdSrbLchCfgReq
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
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Set last 4 bits as 1 for UE reconfiguration */
   if(srbId == WR_PDCP_ID_SRB1)
   {
      transId = transCb->transId | WR_UMM_TGT_HO_SCH_SRB1CFG;
   }
   else
   {
      transId = transCb->transId | WR_UMM_TGT_HO_SCH_SRB2CFG;
   }
   lchCfg  = &cfgReq->u.cfgInfo.u.lchCfg;
   cfgReq->action = RGR_CONFIG;
   cfgReq->u.cfgInfo.cfgType = RGR_LCH_CFG;
   lchCfg->cellId = /*transCb->u.s1TgtHoTrans.cellId*/transCb->ueCb->cellId;
   lchCfg->crnti  = transCb->u.s1TgtHoTrans.crnti;
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

   /* Dispatch the prepared message to scheduler */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}/* End wrUmmS1TgtHoSchdSrbLchCfgReq*/

PRIVATE S16 wrUmmS1TgtHoSchdLchCfgReq
(
 WrUmmTransCb                *transCb, 
 U8                          idx
)
{
   WrUmmS1TgtHoTrans         *s1TgtHoTrans = &transCb->u.s1TgtHoTrans;
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
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Set last 4 bits as 1 for UE reconfiguration */
   transId = transCb->transId | idx;
   lchCfg  = &cfgReq->u.cfgInfo.u.lchCfg;
   cfgReq->action = RGR_CONFIG;
   cfgReq->u.cfgInfo.cfgType = RGR_LCH_CFG;
   lchCfg->cellId = transCb->ueCb->cellId;
   lchCfg->crnti  = transCb->ueCb->crnti;
   drbInfo = &(s1TgtHoTrans->rbInfo[idx].drbInfo);
   lchCfg->lcId   = drbInfo->lchId;
   lchCfg->lcType = CM_LTE_LCH_DTCH;
   lchCfg->dlInfo.dlTrchType = CM_LTE_TRCH_DL_SCH;
   lchCfg->dlInfo.dlQos.qci = drbInfo->qci;
   lchCfg->dlInfo.dlQos.gbr = drbInfo->gbr.dl;
   lchCfg->dlInfo.dlQos.mbr = drbInfo->mbr.dl;
   lchCfg->ulLchQciInfo.lcId = drbInfo->lchId;
   lchCfg->ulLchQciInfo.qci = drbInfo->qci;
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
   /* SPS changes starts */
   if ((s1TgtHoTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG) && \
      (s1TgtHoTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stErabAcceptLst[idx].bIsDlSpsEnabled == TRUE))
   {
      /* Since there is no RBC and both RRM and APP need an enhancement. So, here there can be two approaches 
       * 1) Either assume that the ERAB sent to RRM is being received in the same order in the accepted list
       * 2) Or using a loop compare ERAB ids and find out if bIsSpsEnabled == TRUE 
       * Approach one is assumed as of now to avoid unnecessary loop. Thus accessed directly with idx */
      lchCfg->dlInfo.dlSpsCfg.isSpsEnabled = TRUE;
      s1TgtHoTrans->rbInfo[idx].isDlSpsEnabled = TRUE;
   }
   else
   {
      lchCfg->dlInfo.dlSpsCfg.isSpsEnabled = FALSE;
      s1TgtHoTrans->rbInfo[idx].isDlSpsEnabled = FALSE;
   }
   /* SPS changes ends */

   /* Dispatch the prepared message to scheduler */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}/* End of wrUmmS1TgtHoSchdLchCfgReq*/

/* SPS changes starts */
/** @brief This function is responsible for filling the UL SPS LC for SCHD.
 *
 * @details
 *
 *     Function: wrUmmS1TgtHoSchdUlSpsLchCfgReq
 *
 *         Processing steps:
 *         - Check if SPS ERAB
 *         - Fill LC Id
 *
 * @param[in]  rrmUeRecfgResp : RRM Recfg Resp
 *             numErabs       : Number of ERABs from transCb
 *             pErabList      : Pointer to ERAB list from transCb
 *             ueSpsCfg       : UE SPS Sfg to be filled for SCHD
 * @return Void
 */

PUBLIC Void wrUmmS1TgtHoSchdUlSpsLchCfgReq
(
 RmuUeRecfgRsp        *rrmUeRecfgResp,
 U32                  numErabs,
 Void                 *pErabList,
 RgrUeSpsCfg          *ueSpsCfg
)
{
   U32 count;
   WrUmmS1TgtHoRabInfo  *pErabInfo = (WrUmmS1TgtHoRabInfo *)pErabList;

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

/* This Function is used to send Ue config Req to Scheduler.*/
PRIVATE S16 wrUmmS1TgtHoSchdUeCfgReq
(
 WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   RgrCfgReqInfo             *cfgReq = NULLP;
   RgrUeCfg                  *ueCfg = NULLP;
   U32                       transId = NULLP;
   U16                        prmbl;
   /* SPS changes starts */
   WrUmmS1TgtHoTrans         *s1TgtHoTrans = &transCb->u.s1TgtHoTrans;
   U32                       count         = 0;
   /* SPS changes ends */
   WrUmmCellCb               *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if (cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, (U32)ueCb->cellId,
            "Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&cfgReq, sizeof(RgrCfgReqInfo));
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
   /* Fill dedicated preamble info
    * */
   wrUtlGetDedPreamble(&prmbl, ueCb->cellId);
   if(prmbl != 0)
   {
      wrFillTknU8(&ueCfg->dedPreambleId, prmbl);
   }
   else
   {
      ueCfg->dedPreambleId.pres = PRSNT_DEF;
   }
   ueCb->dedicatedPrmbl = prmbl;

   /* SPS changes starts */
   /* The initialisation from the ueCb is required for the following case:
    * 1) If the SPS was already enabled for the UE, thus RRM doesnot allocate SPS
    *    - The SR mask for all the LCs should be set for rel 9 UE based on this.
    * 2) If the SPS was neither enabled previously nor it is enabled by RRM
    *    - Donot enable SR Mask for any of the LCs based on this. */
   for (count = 0; count < s1TgtHoTrans->numOfRbCfgd; count++)
   {
      s1TgtHoTrans->rbInfo[count].isUlSpsEnabled = FALSE;
   }
   transCb->spsCfgTransCb.isUeDlSpsEnabled = ueCb->spsCfg.spsCfgDl.bIsSpsEnabled;
   transCb->spsCfgTransCb.isUeUlSpsEnabled = ueCb->spsCfg.spsCfgUl.bIsSpsEnabled;
   wrUmmFillSpsSchdCfgUe(transCb->transTyp, transCb->ueCb, \
         s1TgtHoTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp, \
         ueCfg, s1TgtHoTrans->numOfRbCfgd, (Void *)(s1TgtHoTrans->rbInfo), \
         s1TgtHoTrans->ueCap->eUTRA_Cap.accessStratumRls.val, &transCb->spsCfgTransCb.spsCfg, \
         &transCb->spsCfgTransCb.isUeDlSpsEnabled, &transCb->spsCfgTransCb.isUeUlSpsEnabled);
   /* SPS changes ends */

   transId = transCb->transId | WR_UMM_TGT_HO_SCH_UECFG;
   if ((cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS) &&
         (WR_UMM_CSG_NOT_MEMBER == s1TgtHoTrans->ueMbrShpSts))
   {
      ueCfg->csgMmbrSta = FALSE;
   }
   else
   {
      /* membership true for closed and open access */
      ueCfg->csgMmbrSta = TRUE;
   }
   /* Dispatch the prepared message to scheduler.There is no need for */
   /* another funtion for LCH separately than for UE                  */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}/* End of wrUmmS1TgtHoSchdUeCfgReq */


PRIVATE S16 wrUmmS1TgtHoCfgSchd
(
WrUmmTransCb                 *transCb
)
{
   WrUmmS1TgtHoRabInfo       *rbInfo;
   U8                        bearerIdx = 0;
   U8                        noOfBearers = 0;
   U32                       ulGbr[WR_UMM_MAX_LCG_GRP_ID] = {0};
   U32                       ulMbr[WR_UMM_MAX_LCG_GRP_ID] = {0};

   /* Scheduler Configuration.*/
   /* configure the Scheduler with Aggregate Max BitRate.*/
   if (ROK != wrUmmS1TgtHoSchdUeCfgReq(transCb))
   {
      RLOG0(L_ERROR, "Failed To configure Scheduler.");
      RETVALUE(RFAILED);
   }
   /* Configure the scheduler for SRB 1
    * */
   wrUmmS1TgtHoSchdSrbLchCfgReq(transCb, WR_PDCP_ID_SRB1);

   /* Configure the scheduler for SRB 2
    * */
   wrUmmS1TgtHoSchdSrbLchCfgReq(transCb, WR_PDCP_ID_SRB2);

   /* Configure the scheduler for each new RAB being
    * created.             */
   noOfBearers = transCb->u.s1TgtHoTrans.numOfRbCfgd;
   for (bearerIdx = 0; bearerIdx < noOfBearers; bearerIdx++)
   {
      rbInfo = &(transCb->u.s1TgtHoTrans.rbInfo[bearerIdx]);
      if(rbInfo->rabAllowed)
      {
         wrUmmS1TgtHoSchdLchCfgReq(transCb, bearerIdx);
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
   wrUmmSchdUeLcgCfg(transCb->ueCb, transCb->transId, WR_UMM_SRB_LCG_GRP_ID, 0,0);
   wrUmmSchdUeLcgCfg(transCb->ueCb, transCb->transId, WR_UMM_LCG_GRP_ID_1, ulGbr[1],ulMbr[1]);
   wrUmmSchdUeLcgCfg(transCb->ueCb, transCb->transId, WR_UMM_LCG_GRP_ID_2, ulGbr[2],ulMbr[2]);
   wrUmmSchdUeLcgCfg(transCb->ueCb, transCb->transId, WR_UMM_LCG_GRP_ID_3, 0,0);
   transCb->u.s1TgtHoTrans.expctLcgCfgCfm = WR_UMM_MAX_LCG_GRP_ID;

   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1TgtHoPrepRrcDlDcchMsg
(
WrUmmTransCb                 *transCb
)
{
   WrUmmS1TgtHoTrans         *s1TgtHoTrans = &(transCb->u.s1TgtHoTrans);
   WrUeCb                    *ueCb  = transCb->ueCb;
   NhuEncReqSdus             *dcchEncReq = NULLP;

   /* trigger NhuEncodeReq to encode Tgt to Src Transparent Container */
   WR_ALLOCEVNT(&dcchEncReq, sizeof(NhuEncReqSdus));
   if(NULLP == dcchEncReq)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of if condition.*/
   /* Fill Handover Cmd Header.*/
   dcchEncReq->hdr.cellId = ueCb->cellId;
   dcchEncReq->hdr.ueId = ueCb->crnti;
   /* To encode of RRC Connection Reconfiguration.*/
   dcchEncReq->hdr.transId = transCb->transId | WR_UMM_TGT_HO_DLDCCH_MSG_ENC;
   cmMemset((U8 *)&s1TgtHoTrans->measTransCb, 0, sizeof(WrUmmMeasTransInfo));
   /* fill the NhuHovrPrepInfo info inside hoPrepEncReq */
    /* SPS changes starts */
   if(ROK != wrUtlFillHoCmdTrnspCntrWthMobility(transCb, dcchEncReq, &s1TgtHoTrans->measTransCb, \
          s1TgtHoTrans->rrmUeRecfgRspMsg->u.rrmUeHoResp, &s1TgtHoTrans->srcToTgtSpsInfo, \
          &transCb->spsCfgTransCb.spsCfg, transCb->spsCfgTransCb.isUeUlSpsEnabled))
    /* SPS changes ends */
   {
      RLOG0(L_ERROR, "Failed to Fill reconfiguration with mobility control ie");
      RETVALUE(RFAILED);
   }/* End of veUtlFillHoCmdTrnspCntrWthMobility*/
   if(ROK != wrIfmRrcSndEncodeReq(dcchEncReq))
   {
      RLOG0(L_ERROR, "Failed to send Encode Req to NHU.");
      RETVALUE(RFAILED);
   }/* End of VeLiNhuEncodeReq*/
   RETVALUE(ROK);
}/* End of wrUmmX2TgtHoPrepRrcDlDcchMsg*/


PRIVATE S16 wrUmmS1TgtHoFillHovrReqAck
(
WrUmmTransCb                 *transCb,
S1apPdu                      **s1apPdu
)
{
   S1apPdu                                *hovrRqstAckgPdu = NULLP;
   S16                                    numComp = 0;
   S16                                    compCnt = 0;
   S16                                    rbIdx = 0;
   S16                                    ret = 0;
   U8                                     offSet;
   SztProtIE_Field_HovrRqstAckgIEs        *ie = NULLP;
   SztProtIE_SingleCont_E_RABAdmtdItemIEs *ie1 = NULLP;
   SztProtIE_SingleCont_E_RABFailedtoSetupItemHOReqAckIEs *ie2 = NULLP;/*RRM_ARP*/
   MsgLen                                 cnt = 0;
   MsgLen                                 len = 0;
   SztS1AP_PDU                            *SztS1apPdu = NULLP;
   SztHovrRqstAckg                        *sztHovrRqstAckg = NULLP;
   U16                                    noOfBearer;
   WrUmmS1TgtHoRabInfo                    *rbInfo = NULLP;
   CmTptAddr                              *lclAddr;
   U8                                     rbMode;
   WrMmeCb                                *mmeCb = NULLP;
   U8                                     csgAccessMode = 0;
   U32                                    cellCsgId;
   U8                                     numErabReject = 0;/*RRM_ARP*/

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem,
         (Ptr *)&hovrRqstAckgPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED); 
   }
   cmMemset((U8 *)&(hovrRqstAckgPdu->pdu), 0, sizeof(SztS1AP_PDU));
   /* Fill Interface Elementary Procedures for Handover Request Ack Msg.*/
   SztS1apPdu = &hovrRqstAckgPdu->pdu;
   wrFillTknU8(&(SztS1apPdu->choice), S1AP_PDU_SUCCESSFULOUTCOME);
   wrFillTknU8(&(SztS1apPdu->val.successfulOutcome.pres),PRSNT_NODEF);
   wrFillTknU32(&(SztS1apPdu->val.successfulOutcome.procedureCode),
         Sztid_HovrResourceAllocn);
   wrFillTknU32(&(SztS1apPdu->val.successfulOutcome.criticality),
         SztCriticalityrejectEnum);

   /*RRM_ARP*/
   numErabReject =  transCb->u.s1TgtHoTrans.erabsToRejectLst.numrabs; 
   if (0 == numErabReject)
   {
      numComp = 4;
   }/* End of transCb->u.s1TgtHoTrans.noOfNotAdmitdBearers*/
   else
   {
      numComp = 5;/* Indicates no of Mandatory fields are present. */
   }/* End of else.*/

   csgAccessMode = wrEmmGetAccessModeFrmCellId (transCb->ueCb->cellId);
   cellCsgId = wrEmmGetCsgIdFrmCellId (transCb->ueCb->cellId);

   /* If access mode is closed or Hybrid then include the CSG ID IE */
   if ((csgAccessMode == WR_ENB_CLOSED_ACCESS) ||
         (csgAccessMode == WR_ENB_HYBRID_ACCESS))
      numComp++;

   /* If access mode is Hybrid then include the access mode IE */
   if (csgAccessMode == WR_ENB_HYBRID_ACCESS)
      numComp++;

   sztHovrRqstAckg = &SztS1apPdu->val.successfulOutcome.value.u.sztHovrRqstAckg;
   wrFillTknU8(&(sztHovrRqstAckg->pres), PRSNT_NODEF);
   wrFillTknU16(&(sztHovrRqstAckg->protocolIEs.noComp), numComp);
   if ((cmGetMem(&hovrRqstAckgPdu->memCp,
               (numComp * sizeof(SztProtIE_Field_HovrRqstAckgIEs)),
               (Ptr *)&sztHovrRqstAckg->protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill MME UE S1AP ID IE which is received from MME.*/
   ie = &sztHovrRqstAckg->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), 
         transCb->u.s1TgtHoTrans.mme_ue_s1ap_id);
   /* Fill ENB-UE-S1AP-ID which is allocated by target eNB. */
   ie = &sztHovrRqstAckg->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   transCb->u.s1TgtHoTrans.enb_ue_s1ap_id = transCb->ueCb->s1ConCb->enb_ue_s1ap_id;
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), 
         transCb->u.s1TgtHoTrans.enb_ue_s1ap_id);
   /*
    * Start of Comment.
    * Target eNB supports atleast one bearer. So, Admitted list
    * should not be empty.
    * End of Comment.
    */
   /* Fill E-RAB admitted list. */
   ie = &sztHovrRqstAckg->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_E_RABAdmtdLst);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU16(&(ie->value.u.sztE_RABAdmtdLst.noComp),
         transCb->u.s1TgtHoTrans.noOfAdmitdBearers);
   noOfBearer = transCb->u.s1TgtHoTrans.numOfRbCfgd; /*RRM_ARP*/

   if ((cmGetMem(&hovrRqstAckgPdu->memCp,
               ((noOfBearer - numErabReject) * sizeof(SztProtIE_SingleCont_E_RABAdmtdItemIEs)),/*RRM_ARP*/
               (Ptr *)&ie->value.u.sztE_RABAdmtdLst.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   rbInfo = transCb->u.s1TgtHoTrans.rbInfo;
   for(rbIdx = 0; (rbIdx < noOfBearer) && (TRUE == rbInfo[rbIdx].rabAllowed); rbIdx++) /*RRM_ARP*/
   {
      ie1 = &ie->value.u.sztE_RABAdmtdLst.member[rbIdx];

      /* Determining if we need to wait for MME Status Transfer message */
      /* check if there is atleast 1 AM bearer */
      wrUtlGetRbMode(transCb->ueCb->cellId, rbInfo[rbIdx].drbInfo.qci,
            &rbMode);
      /* need not set for every AM bearer. single flag would suffice */
      if((transCb->u.s1TgtHoTrans.sztHovrTyp == SztHovrTypintralteEnum) &&
            (transCb->u.s1TgtHoTrans.snStatusInfoFlag == WR_UMM_TGT_HO_SN_INFO_NOWAIT)&&
            (rbMode == WR_RLC_AM_MODE))
      {
         transCb->u.s1TgtHoTrans.snStatusInfoFlag = WR_UMM_TGT_HO_SN_INFO_WAIT;
      }


      /* Retireve the enb ip address. */
      /* For this we know that the MME will be 
       * connected with the eNB using some ip address. So we pick this ip 
       * address type.
       */
      mmeCb = wrEmmCb.mmeCont.mmes[transCb->ueCb->mmeId];
      wrEmmGetDatAppAddr(&lclAddr, mmeCb->mmeAddr[0].type);
      wrFillTknU8(&(ie1->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie1->id), Sztid_E_RABAdmtdItem);
      wrFillTknU32(&(ie1->criticality), SztCriticalityignoreEnum);
      wrFillTknU8(&(ie1->value.u.sztE_RABAdmtdItem.pres), PRSNT_NODEF);
      wrFillTknU32(&(ie1->value.u.sztE_RABAdmtdItem.e_RAB_ID), 
            rbInfo[rbIdx].drbInfo.eRABId);

      /* Fill Transport Layer Address.*/
      if (CM_TPTADDR_IPV4 == lclAddr->type)
      {
         len = 32; /* Length of the Transport layer address. TODO*/
         WR_FILL_BIT_STR_VAL((ie1->value.u.sztE_RABAdmtdItem.transportLyrAddr),
               len, lclAddr->u.ipv4TptAddr.address, (&hovrRqstAckgPdu->memCp));

      }
      else if(CM_TPTADDR_IPV6 == lclAddr->type)
      {
         len = 128; /* Length of the Transport layer address.TODO*/
         WR_FILL_BIT_STR_ARR((ie1->value.u.sztE_RABAdmtdItem.transportLyrAddr),
               len, lclAddr->u.ipv6TptAddr.ipv6NetAddr, (&hovrRqstAckgPdu->memCp));
      }
      ie1->value.u.sztE_RABAdmtdItem.transportLyrAddr.len = len;

      /* FILL GTP TEID.*/
      ie1->value.u.sztE_RABAdmtdItem.gTP_TEID.pres = PRSNT_NODEF;
      len = 4;
      ie1->value.u.sztE_RABAdmtdItem.gTP_TEID.len = len;
      WR_GET_MEM(&hovrRqstAckgPdu->memCp, len,&(ie1->value.u.sztE_RABAdmtdItem.gTP_TEID.val));
      for (cnt = 0; cnt < len; cnt++)
      {
         /* compute the offset to right shift the val in U32 shift by 24,16 & 8 bytes */
         offSet = (U8)(len - (cnt + 1)) * 8;
         ie1->value.u.sztE_RABAdmtdItem.gTP_TEID.val[cnt] =
            (U8)(rbInfo[rbIdx].drbInfo.locEgtpTunId >> offSet);
      }
      /* Fill the DL & UL Trnsp Lyr Addr & GTP-TEID only if in Hovr Req Msg
       * i) Data Fwding Not possible is Not Prsnt
       * ii) dL_Fwding Proposed is TRUE */
      if(rbInfo[rbIdx].dlFwding == TRUE)
      {
         if ((WR_DIR_DL == (rbInfo[rbIdx].drbInfo.rbDir)) ||
               (WR_DIR_DL_UL == (rbInfo[rbIdx].drbInfo.rbDir)))
         {
            /* Dl Transport Layer Address.*/
            if (CM_TPTADDR_IPV4 == lclAddr->type)
            {
               len = 32; /* Length of the Transport layer address.TODO*/
               WR_FILL_BIT_STR_VAL((ie1->value.u.sztE_RABAdmtdItem.dL_transportLyrAddr),
                     len, lclAddr->u.ipv4TptAddr.address, (&hovrRqstAckgPdu->memCp));
            }
            else if(CM_TPTADDR_IPV6 == lclAddr->type)
            {
               len = 128; /* Length of the Transport layer address.TODO*/
               WR_FILL_BIT_STR_ARR((ie1->value.u.sztE_RABAdmtdItem.dL_transportLyrAddr),
                     len, lclAddr->u.ipv6TptAddr.ipv6NetAddr, (&hovrRqstAckgPdu->memCp));
            }
            ie1->value.u.sztE_RABAdmtdItem.dL_transportLyrAddr.len = len;

            /* FILL DL GTP TEID.*/
            ie1->value.u.sztE_RABAdmtdItem.dL_gTP_TEID.pres = PRSNT_NODEF;
            len = 4;
            ie1->value.u.sztE_RABAdmtdItem.dL_gTP_TEID.len = len;
            WR_GET_MEM(&hovrRqstAckgPdu->memCp, len,&(ie1->value.u. \
                     sztE_RABAdmtdItem.dL_gTP_TEID.val));
            for (cnt = 0; cnt < len; cnt++)
            {
               /* compute the offset to right shift the val in 
                * U32 shift  by 24,16 & 8 bytes */
               offSet = (U8)(len - (cnt + 1)) * 8;
               ie1->value.u.sztE_RABAdmtdItem.dL_gTP_TEID.val[cnt] =
                  (U8)(rbInfo[rbIdx].datFwdDlLocEgtpTunId >> offSet);
            }
         }
         if(( transCb->u.s1TgtHoTrans.sztHovrTyp == SztHovrTypintralteEnum ) &&
               (( WR_DIR_UL == (rbInfo[rbIdx].drbInfo.rbDir)) ||
                ( WR_DIR_DL_UL == (rbInfo[rbIdx].drbInfo.rbDir))))
         {
            /* UL Transport Layer Address.*/
            if (CM_TPTADDR_IPV4 == lclAddr->type)
            {
               len = 32; /* Length of the Transport layer address.TODO*/
               WR_FILL_BIT_STR_VAL((ie1->value.u.sztE_RABAdmtdItem.uL_TportLyrAddr),
               len, lclAddr->u.ipv4TptAddr.address, (&hovrRqstAckgPdu->memCp));

            }
            else if(CM_TPTADDR_IPV6 == lclAddr->type)
            {
               len = 128; /* Length of the Transport layer address.TODO*/
               WR_FILL_BIT_STR_ARR((ie1->value.u.sztE_RABAdmtdItem.uL_TportLyrAddr),
               len, lclAddr->u.ipv6TptAddr.ipv6NetAddr, (&hovrRqstAckgPdu->memCp));
            }
            ie1->value.u.sztE_RABAdmtdItem.uL_TportLyrAddr.len = len;
       

            /* FILL UL GTP TEID.*/
            ie1->value.u.sztE_RABAdmtdItem.uL_GTP_TEID.pres = PRSNT_NODEF;
            len = 4;
            ie1->value.u.sztE_RABAdmtdItem.uL_GTP_TEID.len = len;
            WR_GET_MEM(&hovrRqstAckgPdu->memCp, len, \
                  &(ie1->value.u.sztE_RABAdmtdItem.uL_GTP_TEID.val));
            for (cnt = 0; cnt < len; cnt++)
            {
               /* compute the offset to right shift the val in U32 shift
                * by 24,16 & 8 bytes */
               offSet = (U8)(len - (cnt + 1)) * 8;
               ie1->value.u.sztE_RABAdmtdItem.uL_GTP_TEID.val[cnt] =
                  (U8)(rbInfo[rbIdx].datFwdUlLocEgtpTunId >> offSet);
            }
         }         
      }
      ie1->value.u.sztE_RABAdmtdItem.iE_Extns.noComp.pres = NOTPRSNT;
   }/* End of for(idx = 0; idx < rabInfo.numOfRbCfgd; idx++)*/

   /* Filling Of E-RABS Failed to Setup List. RRM_ARP*/
   if(numErabReject != 0)
   {
      WrUmmRabRls  *rabRls = &transCb->u.s1TgtHoTrans.erabsToRejectLst;
      ie = &sztHovrRqstAckg->protocolIEs.member[compCnt++];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_E_RABFailedToSetupLstHOReqAck);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

      wrFillTknU16(&(ie->value.u.sztE_RABFailedtoSetupLstHOReqAck.noComp),numErabReject);
      if ((cmGetMem(&hovrRqstAckgPdu->memCp,
                  (numErabReject * sizeof(SztProtIE_SingleCont_E_RABFailedtoSetupItemHOReqAckIEs)),
                  (Ptr *)&ie->value.u.sztE_RABFailedtoSetupLstHOReqAck.member)) !=ROK)
      {
         RLOG0(L_FATAL, "Failed to allocate memory. cmGetMem Failed");
         RETVALUE(RFAILED);
      }

      for(rbIdx = 0; rbIdx < numErabReject; rbIdx++)
      {
         ie2 = &ie->value.u.sztE_RABFailedtoSetupLstHOReqAck.member[rbIdx];
         wrFillTknU8(&(ie2->pres), PRSNT_NODEF);
         wrFillTknU32(&(ie2->id), Sztid_E_RABFailedtoSetupItemHOReqAck);
         wrFillTknU32(&(ie2->criticality), SztCriticalityignoreEnum);

         wrFillTknU8(&(ie2->value.u.sztE_RABFailedToSetupItemHOReqAck.pres), PRSNT_NODEF);
         wrFillTknU32(&(ie2->value.u.sztE_RABFailedToSetupItemHOReqAck.e_RAB_ID),rabRls->erabId[rbIdx]);
         wrFillTknU8(&(ie2->value.u.sztE_RABFailedToSetupItemHOReqAck.cause.choice),rabRls->causeTyp);
         wrFillTknU32(&(ie2->value.u.sztE_RABFailedToSetupItemHOReqAck.cause.val.radioNw),rabRls->cause[rbIdx]);
      }
   }

   /* Filling the Target eNB to Source eNB Transparent Container IE.  */
   ie = &sztHovrRqstAckg->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Tget_ToSrc_TprntCont);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);

   /* Filling of RRC Context.*/
   ret = SFndLenMsg(transCb->msg->u.sztEncCfm->u.mBuf,(MsgLen *)&len);
   if((len == 0) || (ret != ROK))
   {
      RLOG0(L_ERROR, "SFndLenMsg: Incorrect buffer len");
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW;
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal = 
         SztCauseRadioNwunspecifiedEnum;           
      RETVALUE(RFAILED);
   }
   ie->value.u.sztTget_ToSrc_TprntCont.pres = PRSNT_NODEF;
   ie->value.u.sztTget_ToSrc_TprntCont.len = len;
   /* Allocate the memory for transparent containier IEs.*/
   cmGetMem(&hovrRqstAckgPdu->memCp, len,
         (Ptr *)&(ie->value.u.sztTget_ToSrc_TprntCont.val));
   if((ie->value.u.sztTget_ToSrc_TprntCont.val) == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED); 
   }

   /* Copy the RRC encoded transparent container */
   SCpyMsgFix(transCb->msg->u.sztEncCfm->u.mBuf, 0,len, 
         ie->value.u.sztTget_ToSrc_TprntCont.val, &cnt);

   /* Fill CSG ID if it is a CSG/hybrid cell */
   if ((csgAccessMode == WR_ENB_CLOSED_ACCESS) || 
         (csgAccessMode == WR_ENB_HYBRID_ACCESS))
   {
      ie = &sztHovrRqstAckg->protocolIEs.member[compCnt++];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_CSG_Id);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      wrFillTknBStr32(&(ie->value.u.sztCSG_Id), WR_S1AP_CSG_ID_LEN, cellCsgId);
   }

   /* Fill CSG ID if it is a hybrid cell */
   if (csgAccessMode == WR_ENB_HYBRID_ACCESS)
   {
      ie = &sztHovrRqstAckg->protocolIEs.member[compCnt];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_CellAccessMode);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      wrFillTknU32(&(ie->value.u.sztCellAccessMode),
            SztCellAccessModehybridEnum);
   }

   *s1apPdu = hovrRqstAckgPdu;
   RETVALUE(ROK);
}



PRIVATE S16 wrUmmS1TgtHoSndHoReqAck
(
WrUmmTransCb                 *transCb
)
{
   S1apPdu                   *hoAckPdu = NULLP;
   SztConRsp                 conResp;

   hoAckPdu = transCb->msg->u.sztEncCfm->u.pdu;
   WrUmmCellCb               *cellCb = NULLP;  /*Cell CB required for KPI Increment*/
   if(RFAILED == wrUmmS1TgtHoFillHovrReqAck(transCb,&hoAckPdu))
   {
      RLOG0(L_ERROR, "Filling of HO Ack Failed");
      RETVALUE(RFAILED);
   }
   RLOG_ARG2(L_EVENT,DBG_CRNTI, transCb->ueCb->crnti, "[S1AP]:Sending HANDOVER REQUEST ACK"
             "[MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
             transCb->u.s1TgtHoTrans.mme_ue_s1ap_id,
             transCb->u.s1TgtHoTrans.enb_ue_s1ap_id);
   conResp.suConnId = transCb->ueCb->s1ConCb->suConnId;
   conResp.spConnId = transCb->ueCb->s1ConCb->spConnId;
   conResp.pdu      = hoAckPdu;
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
      RLOG0(L_INFO,"Updating the KPI Counter for Succeded Inbound Mobility");
      WR_INC_KPI_FUNC(wrIncKpiFrInboundCsgMobilitySuccd);
   }
#endif
   /* CSG_DEV end */

   /* Start The Timer called TGTRELOCEXE (Target Relocation Execution)*/
   wrStartUeTmr(transCb->ueCb, WR_TMR_TGTENB_RELOCEXE, WR_TMR_S1_RELOC_VAL);
   /* Send pkt to S1AP */
   WrIfmS1apConRsp(&conResp);
   
        transCb->ueCb->s1ConCb->s1apConnState = WR_S1AP_CONNECTED;
        transCb->ueCb->ueAttachedtoMme = TRUE; /* ccpu00130088 */
        transCb->ueCb->mobCtrlState = WR_HO_IN_PROGRESS;

   wrStopTransTmr(transCb, WR_TMR_TGT_HOPREP); 
   /*ccpu00127802*/
   RLOG0(L_INFO, "SENT HO REQ ACK TO SOURCE");
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmHoS1TgtHoFillENB_StatusTfr_TprntCont
(
WrUmmTransCb                 *transCb,
SztENB_StatusTfr_TprntCont   *sztENB_StatusTfr_TprntCont
)
{
   WrUmmSnSts                   *snSts = NULLP;
   NhuHoPdcpSduInfo             *nhuHoPdcpSduInfo = NULLP;
   SztBrs_SubjToStatusTfrLst    *statusTfr_List = NULLP;
   SztBrs_SubjToStatusTfr_Item  *statusTfr_Item = NULLP;
   WrUeCb                       *ueCb = transCb->ueCb;
   U16                          seqNumber;
   U32                          hfn;
   U8                           idx = 0;
   U8                           cnt = 0;
   U16                          noComp = 0;
   SztProtExtnField_Brs_SubjToStatusTfr_ItemExtIEs *extIEs;

   /* Allocate the Memory.*/
   WR_ALLOC(&transCb->u.s1TgtHoTrans.snStsInfo, sizeof(WrUmmSnStsInfo));
   if (NULLP == transCb->u.s1TgtHoTrans.snStsInfo)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of Allocate Memory*/
   cmMemset((U8 *)transCb->u.s1TgtHoTrans.snStsInfo, 0, 
                             (sizeof(WrUmmSnStsInfo)));
   statusTfr_List = &sztENB_StatusTfr_TprntCont->bearers_SubjToStatusTfrLst;
   /* Filling No of Radio Bearers.*/
   transCb->u.s1TgtHoTrans.snStsInfo->numOfRb = statusTfr_List->noComp.val;
   noComp = statusTfr_List->noComp.val;
   for (idx = 0; idx < noComp;idx++)
   {
      statusTfr_Item = 
          &statusTfr_List->member[idx].value.u.sztBrs_SubjToStatusTfr_Item;
      snSts = &transCb->u.s1TgtHoTrans.snStsInfo->wrSnSts[idx];
      if(ROK != wrUmmGetDrbIdFromErabId(ueCb, statusTfr_Item->e_RAB_ID.val,
                                              &snSts->rbId))
      {
         RETVALUE(RFAILED);
      }
      /* snSts->rbId = statusTfr_Item->e_RAB_ID.val; */
      nhuHoPdcpSduInfo = &snSts->nhuHoPdcpSduInfo;
        nhuHoPdcpSduInfo->isHoPres = 0x00;
      if (TRUE == statusTfr_Item->uL_COUNTvalue.pres.pres)
      {
         /* Initializing and updating the bitmap to indicate the presence of UL COUNT */
         nhuHoPdcpSduInfo->isHoPres |= NHU_HO_UL;
         /* RRC-REL10-Upgrade start */
         if(transCb->ueCb->isFullCfgReqd)
         {
            /* we will send full configuration to Rel 10 UE in the HO command .Hence reseting to zero*/
            seqNumber = 0;
            hfn = 0;
         }
         else
         {
            seqNumber = statusTfr_Item->uL_COUNTvalue.pDCP_SN.val;
            hfn = statusTfr_Item->uL_COUNTvalue.hFN.val;
         }  /* RRC-REL10-Upgrade end */
         /*  HOFIX */
         nhuHoPdcpSduInfo->snLen = NHU_PDCP_SN_SIZE_12BITS;
         WR_CALC_COUNT(NHU_PDCP_SN_SIZE_12BITS, seqNumber, hfn, 
                   nhuHoPdcpSduInfo->ulCountVal);
         RLOG3(L_DEBUG, "UL pdcp sn[%d] pdcp hfn[%lu] count[%lu]", seqNumber,
            statusTfr_Item->uL_COUNTvalue.hFN.val, nhuHoPdcpSduInfo->ulCountVal);
      }/* End of if condition.*/
      if (TRUE == statusTfr_Item->dL_COUNTvalue.pres.pres)
      {
         /* Initializing and updating the bitmap to indicate the presence of DL COUNT */
         nhuHoPdcpSduInfo->isHoPres |= NHU_HO_DL;
         /* RRC-REL10-Upgrade start */
         if(transCb->ueCb->isFullCfgReqd)
         {
            /* we will send full configuration to Rel 10 UE in the HO command .Hence reseting to zero*/
            seqNumber = 0;
            hfn = 0;
         } 
         else
         {
         seqNumber = statusTfr_Item->dL_COUNTvalue.pDCP_SN.val;
         hfn = statusTfr_Item->dL_COUNTvalue.hFN.val;
         }  /* RRC-REL10-Upgrade end */
         /*  HOFIX */
         nhuHoPdcpSduInfo->snLen = NHU_PDCP_SN_SIZE_12BITS;
         WR_CALC_COUNT(NHU_PDCP_SN_SIZE_12BITS, seqNumber, hfn, 
                    nhuHoPdcpSduInfo->dlCountVal);
         RLOG3(L_DEBUG, "DL pdcp sn[%d] pdcp hfn[%lu] count[%lu]", seqNumber,
            statusTfr_Item->dL_COUNTvalue.hFN.val, nhuHoPdcpSduInfo->dlCountVal);
      }/* End of if condition.*/
      /* Check for the availability of Receive Status of UL PDCP SDUs */
      if(TRUE == statusTfr_Item->receiveStatusofULPDCPSDUs.pres)
      {
         /* Retreive the UL Bitmap and numOfBits 
          * Send it to PDCP */
         wrRetrvBitMapfrm4096BSXL(&(statusTfr_Item->receiveStatusofULPDCPSDUs),
               &snSts->nhuHoPdcpSduInfo.numBits, &snSts->nhuHoPdcpSduInfo.ulBitMap);
      }
      if (statusTfr_Item->iE_Extns.noComp.pres)
      {
          for (cnt = 0; cnt < statusTfr_Item->iE_Extns.noComp.val; cnt++)
          {
             extIEs = &statusTfr_Item->iE_Extns.member[cnt];
             if (extIEs->id.val == Sztid_ULCOUNTValueExtended)
             {
                 if (extIEs->extensionValue.u.sztid_ULCOUNTValueExtended.pres.pres)
                 {
                     /* Initializing and updating the bitmap to indicate the presence of UL COUNT */
                     nhuHoPdcpSduInfo->isHoPres |= NHU_HO_UL;
                     /* RRC-REL10-Upgrade start */
                     if(transCb->ueCb->isFullCfgReqd)
                     {
                         /* we will send full configuration to Rel 10 UE in the HO command .Hence reseting to zero*/
                         seqNumber = 0;
                         hfn = 0;
                     }
                     else
                     {
                         seqNumber = extIEs->extensionValue.u.sztid_ULCOUNTValueExtended.pDCP_SNExtended.val;
                         hfn = extIEs->extensionValue.u.sztid_ULCOUNTValueExtended.hFNMdfd.val;
                     }  /* RRC-REL10-Upgrade end */
                     /*  HOFIX */
                     nhuHoPdcpSduInfo->snLen = NHU_PDCP_SN_SIZE_15BITS;
                     WR_CALC_COUNT(NHU_PDCP_SN_SIZE_15BITS, seqNumber, hfn, 
                             nhuHoPdcpSduInfo->ulCountVal);
                     RLOG3(L_DEBUG, "UL pdcp sn[%d] pdcp hfn[%lu] count[%lu]", seqNumber,
                             statusTfr_Item->uL_COUNTvalue.hFN.val, nhuHoPdcpSduInfo->ulCountVal);

                 }
             }
             if (extIEs->id.val == Sztid_DLCOUNTValueExtended)
             {
                 if (extIEs->extensionValue.u.sztid_DLCOUNTValueExtended.pres.pres)
                 {
                     /* Initializing and updating the bitmap to indicate the presence of UL COUNT */
                     nhuHoPdcpSduInfo->isHoPres |= NHU_HO_DL;
                     /* RRC-REL10-Upgrade start */
                     if(transCb->ueCb->isFullCfgReqd)
                     {
                         /* we will send full configuration to Rel 10 UE in the HO command .Hence reseting to zero*/
                         seqNumber = 0;
                         hfn = 0;
                     }
                     else
                     {
                         seqNumber = extIEs->extensionValue.u.sztid_DLCOUNTValueExtended.pDCP_SNExtended.val;
                         hfn = extIEs->extensionValue.u.sztid_DLCOUNTValueExtended.hFNMdfd.val;
                     }  /* RRC-REL10-Upgrade end */
                     /*  HOFIX */
                     nhuHoPdcpSduInfo->snLen = NHU_PDCP_SN_SIZE_15BITS;
                     WR_CALC_COUNT(NHU_PDCP_SN_SIZE_15BITS, seqNumber, hfn, 
                             nhuHoPdcpSduInfo->dlCountVal);
                     RLOG3(L_DEBUG, "DL pdcp snExt[%d] pdcp hfnMdfd[%lu] count[%lu]", seqNumber,
                             hfn, nhuHoPdcpSduInfo->dlCountVal);

                 }
             }
             if(extIEs->id.val == Sztid_ReceiveStatusOfULPDCPSDUsExtended)
             {
                 /* Retreive the UL Bitmap and numOfBits 
                  * Send it to PDCP */
                 wrRetrvBitMapfrm16384BSXL(&(extIEs->extensionValue.u.sztReceiveStatusOfULPDCPSDUsExtended),
                         &snSts->nhuHoPdcpSduInfo.numBits, &snSts->nhuHoPdcpSduInfo.ulBitMap);
             }
          }
      }
   }/* End of for loop.*/
   RETVALUE(ROK);
}/* End of wrUmmHoS1TgtHoFillENB_StatusTfr_TprntCont*/

PRIVATE S16 wrUmmS1TgtHoParseMMESnStatusTfrMsg
(
WrUmmTransCb                 *transCb
)
{
   S1apPdu                          *s1apPdu = NULLP;
   SztInitiatingMsg                 *initiatingMsg = NULLP;
   SztProtIE_Field_MMEStatusTfrIEs  *ie = NULLP;
   U8                               idx = 0;
   U8                               noComp = 0;

   s1apPdu = transCb->msg->u.s1Pdu->pdu;
   initiatingMsg = &s1apPdu->pdu.val.initiatingMsg;
   noComp = initiatingMsg->value.u.sztMMEStatusTfr.protocolIEs.noComp.val;
   for (idx = 0; idx < noComp; idx++)
   {
      ie = &initiatingMsg->value.u.sztMMEStatusTfr.protocolIEs.member[idx];
      switch (ie->id.val)
      {
         case Sztid_MME_UE_S1AP_ID:
            if (transCb->u.s1TgtHoTrans.mme_ue_s1ap_id != 
                       ie->value.u.sztMME_UE_S1AP_ID.val)
            {
               RLOG0(L_ERROR, "Received MME UE S1AP ID Is Invalid");
               RETVALUE(RFAILED);
            }
            break;
         case Sztid_eNB_UE_S1AP_ID:
         {
            transCb->u.s1TgtHoTrans.enb_ue_s1ap_id = ie->value.u.sztENB_UE_S1AP_ID.val;
            break;
         }
         case Sztid_eNB_StatusTfr_TprntCont:
            if (ROK != wrUmmHoS1TgtHoFillENB_StatusTfr_TprntCont(transCb,
                  &ie->value.u.sztENB_StatusTfr_TprntCont))
            {
               RLOG0(L_ERROR, "Failed to Fill sztENB_StatusTfr_TprntCont");
               RETVALUE(RFAILED);
            }/* End of wrUmmHoS1TgtHoFillENB_StatusTfr_TprntCont*/
            break;
         default :
            RLOG0(L_ERROR,"Invalid IE is present in Sn Status Transfer Message");
            RETVALUE(RFAILED);
      }/* End of switch*/

   }/* End of for loop.*/

   RLOG_ARG2(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti, "[S1AP]:Received MME STATUS "
             "TRANSFER [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
             transCb->u.s1TgtHoTrans.mme_ue_s1ap_id,
             transCb->u.s1TgtHoTrans.enb_ue_s1ap_id);

   RETVALUE(ROK);
}

/*
*
*       Fun:  wrUmmS1TgtHoFillRrcEvntHdr 
*
*       Desc:  This function fills NHU Event Header 
*
*       Ret:   ROK
*
*       Notes: None
*
*       File:  
*
*/
PUBLIC S16 wrUmmS1TgtHoFillRrcEvntHdr
(
NhuHdr      *nhuHdr,
U16         cellId,
U16         crnti,
U32         transId
)
{

   TRC2(wrUmmS1TgtHoFillRrcEvntHdr);

   RLOG0(L_DEBUG, "Populate the NHU header info");

   nhuHdr->cellId  = cellId;
   nhuHdr->ueId    = crnti;
   nhuHdr->transId = transId;

   RETVALUE(ROK);
} /* End of wrUmmS1TgtHoFillRrcEvntHdr */

PRIVATE S16 wrUmmS1TgtHoFillHovrNtfy
(
WrUmmTransCb                 *transCb,
S1apPdu                      **pdu
)
{
   U16                         numComp = 0;
   U16                         compCnt = 0;
   U16                         ret = 0;
   U8                          len = 0;  /* KW fix for LTE_ADV */
   S1apPdu                     *hovrNtfyPdu = NULLP;
   SztHovrNtfy                 *hovrNtfy = NULLP; 
   SztProtIE_Field_HovrNtfyIEs *ie = NULLP;
   WrTai                       tai;
        WrCgi                       eCgi; /* ccpu00136516 */

   /* Initialize memory control point */
   ret = cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem,
         (Ptr *)&hovrNtfyPdu);
   if(ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   cmMemset((U8 *)&(hovrNtfyPdu->pdu), 0, sizeof(SztS1AP_PDU));
   /* Fill Interface Elementary Procedure for Handover Notify Message.*/
   wrFillTknU8(&(hovrNtfyPdu->pdu.choice), S1AP_PDU_INITIATINGMSG);
   wrFillTknU8(&(hovrNtfyPdu->pdu.val.initiatingMsg.pres), PRSNT_NODEF);
   wrFillTknU32(&(hovrNtfyPdu->pdu.val.initiatingMsg.procedureCode),
         Sztid_HovrNotification);
   wrFillTknU32(&(hovrNtfyPdu->pdu.val.initiatingMsg.criticality),
         SztCriticalityignoreEnum);
   numComp = 4;/* Indicates No of Mandatory fields .*/
   hovrNtfy = &hovrNtfyPdu->pdu.val.initiatingMsg.value.u.sztHovrNtfy; 
   wrFillTknU8(&(hovrNtfy->pres),PRSNT_NODEF);
   wrFillTknU16(&(hovrNtfy->protocolIEs.noComp), numComp);
   if ((cmGetMem(&hovrNtfyPdu->memCp, 
               (numComp * sizeof(SztProtIE_Field_HovrNtfyIEs)),
               (Ptr *)&hovrNtfy->protocolIEs.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /*
    * Start of Comment.
    * Fill MME-UE-S1AP-ID which is allcated by MME to uniquly identify the UE
    * within the MME.
    * End of Comment.
    */
   ie = &hovrNtfy->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), 
                  transCb->u.s1TgtHoTrans.mme_ue_s1ap_id);
   /*
    * Start of Comment.
    * Filling ENB-UE-S1AP-ID which is allocated by target eNB to uniquly
    * identify the UE within eNB.
    * End of Comment.
    */
   ie = &hovrNtfy->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityrejectEnum);
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), 
           transCb->u.s1TgtHoTrans.enb_ue_s1ap_id);
   /* FIX for ccpu00135294 */
   wrEmmGetTai(transCb->ueCb, &tai);
   /*
    * Start of Comment.
    * Fill EUTRAN-CGI which is used to globally identify a cell. It consist of
    * PLMN-ID and Cell identity.
    * End of Comment.
    */
   ie = &hovrNtfy->protocolIEs.member[compCnt++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_EUTRAN_CGI);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU8(&(ie->value.u.sztEUTRAN_CGI.pres), PRSNT_NODEF);

   /* Fill the PLMN Identity of E-UTRAN-CGI. */
   wrEmmMmeSztFillPLMNId(hovrNtfyPdu, &tai.plmnId,
         &ie->value.u.sztEUTRAN_CGI.pLMNidentity);
   /* Fill the Cell Identity of E_UTRAN-CGI.*/
        /* ccpu00136516 */
        wrEmmGetCgi(transCb->ueCb->cellId, &eCgi);
   len = 28;/* Indicates length of the Cell ID in terms of bits.*/
   wrFillTknBStr32(&(ie->value.u.sztEUTRAN_CGI.cell_ID), len, 
             eCgi.cellId);
   ie->value.u.sztEUTRAN_CGI.iE_Extns.noComp.pres = NOTPRSNT;
   /*
    * Start of Comment.
    * Fill the  TAI which is used to uniquely identify a tracking area.
    * It consist of PLMN-ID and TAC(Tracking Area Code).
    * End of Comment.
    */
   ie = &hovrNtfy->protocolIEs.member[compCnt];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_TAI);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU8(&(ie->value.u.sztTAI.pres), PRSNT_NODEF);
   /* Fill the PLMN-ID of TAI*/
   //wrEmmMmeSztFillPLMNId(&hovrNtfyPdu->memCp, &tai.plmnId,
     //    &(ie->value.u.sztTAI.pLMNidentity));
   wrEmmMmeSztFillPLMNId(hovrNtfyPdu, &tai.plmnId,
         &(ie->value.u.sztTAI.pLMNidentity));
   /* Fill the TAC of the TAI*/
   wrEmmMmeSztFillTAC(tai.tac, NULLP, &(ie->value.u.sztTAI.tAC));
   ie->value.u.sztTAI.iE_Extns.noComp.pres = NOTPRSNT;

   *pdu = hovrNtfyPdu;  
   RETVALUE(ROK);
}
PRIVATE S16 wrUmmS1TgtHoPrepHovrNotify
(
WrUmmTransCb                 *transCb
)
{
   S1apPdu                   *hovrNtfyPdu = NULLP;
   SztDatEvntReq             datEvt;

   if (ROK != wrUmmS1TgtHoFillHovrNtfy(transCb,&hovrNtfyPdu))
   {
      RLOG0(L_ERROR, "Failed To Fill Handover Notify");
      RETVALUE(RFAILED);
   }
   datEvt.spConnId = transCb->ueCb->s1ConCb->spConnId;/*TODO*/
   datEvt.pdu    =  hovrNtfyPdu;

   /* Trigger SZT data Req */
   /*ccpu00127802*/
      
   RLOG_ARG2(L_EVENT,DBG_CRNTI, transCb->ueCb->crnti, "[S1AP]:Sending HANDOVER NOTIFY"
             "[MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu]",
             transCb->u.s1TgtHoTrans.mme_ue_s1ap_id,
             transCb->u.s1TgtHoTrans.enb_ue_s1ap_id);
   WrIfmS1apDatReq(&datEvt);

   /* ccpu00131919 */
   /* No need to send LocRpt in case of Femto if LocRpt evnt is change in serving cell*/

   RETVALUE(ROK); 
}

PRIVATE S16 wrUmmS1TgtHoFillIRATSnStat
(
 WrUmmTransCb                 *transCb
 )
{
   WrUmmSnSts                   *snSts = NULLP;
   NhuHoPdcpSduInfo             *nhuHoPdcpSduInfo = NULLP;
   U8                           idx = 0;
   U8                           indx = 0;
   U8                        numOfRb = 0;
   WrUmmS1TgtHoTrans         *s1TgtTrans = &(transCb->u.s1TgtHoTrans);
   WrUmmS1TgtHoRabInfo       *ueRbInfo = NULLP;

   /* Allocate the Memory.*/
   WR_ALLOC(&transCb->u.s1TgtHoTrans.snStsInfo, sizeof(WrUmmSnStsInfo));
   if (NULLP == transCb->u.s1TgtHoTrans.snStsInfo)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }/* End of Allocate Memory*/
   cmMemset((U8 *)transCb->u.s1TgtHoTrans.snStsInfo, 0, 
         (sizeof(WrUmmSnStsInfo)));

   numOfRb = s1TgtTrans->noOfAdmitdBearers;
   for(idx = 0; idx < numOfRb; idx++)
   {
      ueRbInfo = &s1TgtTrans->rbInfo[idx];
      if(ueRbInfo->rabAllowed == TRUE && ueRbInfo->drbInfo.rbMode == WR_RLC_AM_MODE)
      {
         s1TgtTrans->snStsInfo->numOfRb++;
         snSts = &transCb->u.s1TgtHoTrans.snStsInfo->wrSnSts[indx++];
         nhuHoPdcpSduInfo = &snSts->nhuHoPdcpSduInfo;

         snSts->rbId = ueRbInfo->drbInfo.rbId;
         nhuHoPdcpSduInfo->isHoPres = NHU_HO_UL | NHU_HO_DL;
         nhuHoPdcpSduInfo->ulCountVal = 0;
         nhuHoPdcpSduInfo->dlCountVal = 0;
         nhuHoPdcpSduInfo->numBits = 0;
         nhuHoPdcpSduInfo->ulBitMap = NULLP;
      }
   }
   RETVALUE(ROK); 
}

PRIVATE S16 wrUmmS1TgtHoPdcpSnStsUpd
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
   WrUmmS1TgtHoTrans         *s1TgtTrans = &transCb->u.s1TgtHoTrans;
   U8                        numOfRb = 0;
   U8                        loopCnt = 0;

   /* Allocate memory for event structure.*/
   WR_ALLOCEVNT(&nhuCfgReqSdus, sizeof(NhuCfgReqSdus));
   if(NULLP == nhuCfgReqSdus)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill Event Handler Information.*/
   wrUmmS1TgtHoFillRrcEvntHdr(&(nhuCfgReqSdus->hdr), ueCb->cellId, ueCb->crnti,
         transCb->transId | WR_UMM_TGT_HO_SN_CFG);
   /*
    * Fill SN status information in the ueCfg whic will be conveyed to PDCP by RRC .
    */
   nhuUeCfgInfo = &nhuCfgReqSdus->cfgInfo;
   nhuUeCfgInfo->ueCfgBitMsk = NHU_EXP_CFG_RB_CFG;
   nhuUeCfgInfo->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_MODIFY;
   rbCfgList = &(nhuUeCfgInfo->rbCfgAllList.rbToModCfgList);
   numOfRb = s1TgtTrans->snStsInfo->numOfRb;
   rbCfgList->numOfRbCfgInfo = numOfRb;
   WR_GET_MEM(nhuCfgReqSdus, (sizeof(NhuRbCfgInfo) * (numOfRb)),\
         &(rbCfgList->rbCfgInfoList));
   for (loopCnt = 0; loopCnt < numOfRb; loopCnt++)
   {
      nhuRbCfgInfo = &rbCfgList->rbCfgInfoList[loopCnt];
      /* Initialize and update the bitmask */
      nhuRbCfgInfo->rbCfgBitMsk = 0x00 | NHU_RB_CFG_PDCP_INFO;

      nhuRbCfgInfo->rbId.rbId = s1TgtTrans->snStsInfo->wrSnSts[loopCnt].rbId;
      /* As the data forwarding only happens for DRB and the SN transfer also comes
       * only for the DRBs assign the bearer type to CM_LTE_DRB .
       */
      nhuRbCfgInfo->rbId.rbTyp = CM_LTE_DRB;

      /* Logical channel Mapping */
      nhuRbCfgInfo->lcId.lcId = s1TgtTrans->snStsInfo->wrSnSts[loopCnt].rbId;
      /* As the data forwarding only happens for DRB and the SN transfer also comes
       * only for the DRBs assign the Logical Channel Type to CM_LTE_LCH_DTCH.
       */
      nhuRbCfgInfo->lcId.logChType = CM_LTE_LCH_DTCH;

      pdcpCfg = &nhuRbCfgInfo->pdcpCfg;
      /* Initialize and update the bitmask */
      pdcpCfg->pdcpCfgBitMsk = 0x00 | NHU_PDCP_SDU_STA_INFO;

      pdcpCfg->hoPdcpSduStaInfo = 
         s1TgtTrans->snStsInfo->wrSnSts[loopCnt].nhuHoPdcpSduInfo; 

      /* initializing the members which are not used */
      pdcpCfg->intgReqd = FALSE;
      pdcpCfg->cfmReqd = FALSE;
   }/* End of for () */

   /* Send the UE configuration to RRC */    
   if (ROK != wrIfmRrcSndCfgReq(nhuCfgReqSdus))
   {
      RLOG0(L_ERROR, "Failed to send RRC Config Req");
      /* Fix for ccpu00125004 */
      WR_FREEEVNT(nhuCfgReqSdus); 
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}


PRIVATE S16 wrUmmS1TgtHoPrcEnbStatusTfr
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmS1TgtHoTrans         *s1TgtHoTrans = &transCb->u.s1TgtHoTrans;
   WrUmmCellCb               *cellCb;
   WrUmmRatLst                 ratLst;/*removed unused variable*/ 

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if (NULLP == cellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)ueCb->cellId,"Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   /* Parse the MME Sn Status Transfer Message */
   if (ROK != wrUmmS1TgtHoParseMMESnStatusTfrMsg(transCb))
   {
      RLOG0(L_ERROR, "Failed to Parse Sn Status Transfer Message");
      RETVALUE(RFAILED);
   }/* End of wrUmmS1TgtHoParseMMESnStatusTfrMsg*/


   s1TgtHoTrans->snStatusInfoFlag = WR_UMM_TGT_HO_SN_INFO_GOT;
   if(s1TgtHoTrans->isUeAttached == TRUE)
   {
      /* Send PDCP SN Status to PDCP through RRC */
      RLOG0(L_DEBUG, "=====Pdcp Sn Status======");
      if (ROK != wrUmmS1TgtHoPdcpSnStsUpd(transCb))
      {
         RLOG0(L_ERROR, "Failed to send wrPdcpSnStsUpd");
         RETVALUE(RFAILED);
      }/* End of vePdcpSnStsUpd*/

      if (ROK != wrUmmS1TgtHoPrepHovrNotify(transCb))
      {
         RLOG0(L_ERROR, "Failed to Prepare Handover Notify Message");
         RETVALUE(RFAILED);
      }/* End of the wrUmmPrepPathSwitchReq*/
      /* RRC-REL10-Upgrade */
      ueCb->isFullCfgReqd = FALSE;

      /* Inactivity timer should be started for the Hand in Ue */
      RLOG0(L_DEBUG, "Inactivity req has been sent to dam");
      wrIfmDamStartInactivityTimer(transCb->ueCb->cellId,transCb->ueCb->crnti);//suhas

      /* ccpu00131426 - Fix */
      if ( WR_UMM_TGT_HO_DRX_CFG_SENT != s1TgtHoTrans->state) 
      {
         /*DRX QCI*/                 
         if((ueCb->isGBRBearerExists == NOT_IN_USE))
         {
            RLOG0(L_INFO,
              "GBR bearer is not exists after hand-in");
            if(wrUmmStartAnr(transCb->ueCb,FALSE) != ROK)
            {
               RLOG0(L_ERROR,"Failed to Start the ANR");
            }
         }           
         /* Check for the UE Capability Info  when the supported band list IE in 
          * the UE Radio Capability IE contains IRAT bands and specific IRAT octet 
          * string is not present in the Source to Target Container which is updated in
          * the ueCb, then send UECAP ENQ to UE with required IRATs type */
         if( PRSNT_NODEF == 
               ueCb->ueCap->eUTRA_Cap.interRAT_Params.pres.pres )
         {
            wrFillRatLstRatType(ueCb->ueCap,&ratLst,&s1TgtHoTrans->handInUeCdma);
            if(s1TgtHoTrans->handInUeCdma == TRUE)
            {
               if(ROK != wrUmmUeCapSndUeCapEnq(transCb, &ratLst))
               {
                  RLOG0(L_ERROR,"Failed to Send UECAP Enq.");
                  RETVALUE(RFAILED);
               }
            }
         }
         /* HO_FIX: close the transaction in else part */
   if ( s1TgtHoTrans->handInUeCdma == FALSE )
                     {
                                 wrUmmTransComplete(transCb);
                                 ueCb->hoTransId = 0;
                     }

      }
#ifndef WR_GRPPWR_CNTRL
      //wrStartUeTmr(ueCb, WR_TMR_RAD_RES_MOD_TMR, 
            //WR_TMR_RAD_RES_MOD_TMR_VAL);
#endif
#ifdef WR_DETECT_RLF
      if(ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_CQI)
      {
         wrStartUeTmr(ueCb, WR_TMR_DETECT_RLF, WR_TMR_DETECT_RLF_VAL);
      }
#endif

   }
   RETVALUE(ROK);
}
/* ccpu00131426 - Fix */
/** @brief This function is responsible for reconfiguring the UE context
 *         at the scheduler.
 *
 * @details
 *
 *     Function: wrUmmS1HoDrxReCfgSchdUeRecfgReq
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
PRIVATE S16 wrUmmS1HoDrxReCfgSchdUeRecfgReq 
(
WrUmmTransCb                 *transCb
)
{
   WrUmmS1TgtHoTrans *s1TgtHoTrans = &transCb->u.s1TgtHoTrans;
   RgrCfgReqInfo     *cfgReq;
   RgrUeRecfg        *ueRecfg;
   RmuUeRecfgRsp     *rrmUeRecfgResp = s1TgtHoTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
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
   s1TgtHoTrans->state = WR_UMM_TGT_HO_DRX_CFG_SENT;
   wrUmmSchdFillUeDrxCfgInfo(ueRecfg, (WrDrxCfg *)&rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.stRmDrxCfg);
   
   /* Dispatch the prepared message to scheduler. */ 
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
} /* wrUmmS1HoDrxReCfgSchdUeRecfgReq */ 


U32 hoSucessHopCount;
PRIVATE S16 wrUmmS1TgtHoHdlRrcConReConfgComplete
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmS1TgtHoTrans         *s1TgtHoTrans = &transCb->u.s1TgtHoTrans;
   WrUmmCellCb               *cellCb;
   WrUmmRatLst                        ratLst;/*removed unused*/
   U8                                 idx = 0;
   U8                                 numOfRb = 0;
   WrUmmS1TgtHoRabInfo               *ueRbInfo = NULLP;

   /* Handling of RRC Connection Reconfiguration Complete.
    * Upon Reception of RRC Connection Reconfiguration Complete Message
    * from the UE(Handover Confirmation.), the target eNB shall prepare
    * Handover Notify Message and send it to the MME.
    */
   /* Change the Handover State.*/
   s1TgtHoTrans->state = WR_HANDOVER_COMPLETION;
   ueCb->rrcConnState = WR_UMM_RRC_CONNECTED;
  
    WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
     if (NULLP == cellCb)
     {
        RLOG_ARG0(L_ERROR,DBG_CELLID,(U32)ueCb->cellId,"Error in getting CELL CB");
          RETVALUE(RFAILED);
     }
  
   transCb->ueCb->mobCtrlState = WR_HO_NORMAL;
   /* ccpu00131426 - Fix : Send Scheduler DRX configuration */
   RmuUeRecfgRsp *rrmUeRecfgResp = s1TgtHoTrans->rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
   if((rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & 
          RM_RCM_MAC_MAIN_CFG) &&
      (rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap & 
          RM_RCM_MAC_MAIN_CFG_DRX))
   {
     wrUmmS1HoDrxReCfgSchdUeRecfgReq(transCb);
   }
   if(s1TgtHoTrans->snStatusInfoFlag == WR_UMM_TGT_HO_SN_INFO_NOWAIT || 
         s1TgtHoTrans->snStatusInfoFlag == WR_UMM_TGT_HO_SN_INFO_GOT)
   {
      
      
      if((s1TgtHoTrans->sztHovrTyp == SztHovrTyputrantolteEnum)||
         (s1TgtHoTrans->sztHovrTyp == SztHovrTypgerantolteEnum))
      {

         /* Check if AM bearer is present */
         numOfRb = s1TgtHoTrans->noOfAdmitdBearers;
         for(idx = 0; idx < numOfRb; idx++)
         {
            ueRbInfo = &s1TgtHoTrans->rbInfo[idx];
            if( ( ueRbInfo->rabAllowed == TRUE ) && \
                ( ueRbInfo->drbInfo.rbMode == WR_RLC_AM_MODE ))
            {
               break;
            }
         }

         if ( idx  < numOfRb )
         {     
            wrUmmS1TgtHoFillIRATSnStat(transCb);
            s1TgtHoTrans->snStatusInfoFlag = WR_UMM_TGT_HO_SN_INFO_GOT;
         }

      }
      /* Send PDCP SN Status to PDCP through RRC */   
      RLOG0(L_DEBUG, "=====Pdcp Sn Status======");
      if ( s1TgtHoTrans->snStatusInfoFlag == WR_UMM_TGT_HO_SN_INFO_GOT )
      {
         if (ROK != wrUmmS1TgtHoPdcpSnStsUpd(transCb))
         {
            RLOG0(L_ERROR, "Failed to send wrPdcpSnStsUpd");

            RETVALUE(RFAILED);
         }/* End of vePdcpSnStsUpd*/
      }
      if (ROK != wrUmmS1TgtHoPrepHovrNotify(transCb))
      {
         RLOG0(L_ERROR, "Failed to Prepare Handover Notify Message");

         RETVALUE(RFAILED);
      }/* End of the wrUmmPrepPathSwitchReq*/
      
      /* Inactivity timer should be started for the Hand in Ue */
      wrIfmDamStartInactivityTimer(transCb->ueCb->cellId,transCb->ueCb->crnti);
      
      /*DRX QCI*/                 
      if((ueCb->isGBRBearerExists == NOT_IN_USE))
      {
         RLOG0(L_INFO,
           "GBR bearer is not exists after hand-in");
         if(wrUmmStartAnr(transCb->ueCb,FALSE) != ROK)
         {
            RLOG0(L_ERROR,"Failed to Start the ANR");
         }
      }           
      hoSucessHopCount++;
      
      /* ccpu00131426 - Fix: Do it after Schd Confirm received */
   if ( s1TgtHoTrans->state != WR_UMM_TGT_HO_DRX_CFG_SENT ) 
   {
         /* Check for the UE Capability Info  when the supported band list IE in 
       * the UE Radio Capability IE contains IRAT bands and specific IRAT octet 
       * string is not present in the Source to Target Container which is updated in
       * the ueCb, then send UECAP ENQ to UE with required IRATs type */
      if( PRSNT_NODEF == 
            s1TgtHoTrans->ueCap->eUTRA_Cap.interRAT_Params.pres.pres )
      {
         wrFillRatLstRatType(s1TgtHoTrans->ueCap,&ratLst,&s1TgtHoTrans->handInUeCdma);
         if(s1TgtHoTrans->handInUeCdma == TRUE)
         {
            if(ROK != wrUmmUeCapSndUeCapEnq(transCb, &ratLst))
            {
               RLOG0(L_ERROR,"Failed to Send UECAP Enq.");
               RETVALUE(RFAILED);
            }
         }
      }

         /* HO_FIX : Close the transaction in else part */
      if ( s1TgtHoTrans->handInUeCdma == FALSE )
      {
         wrUmmTransComplete(transCb);
         ueCb->hoTransId = 0;
      }
 
   }
#ifndef WR_GRPPWR_CNTRL
      //wrStartUeTmr(ueCb, WR_TMR_RAD_RES_MOD_TMR, 
                //WR_TMR_RAD_RES_MOD_TMR_VAL);
#endif
#ifdef WR_DETECT_RLF
      if(ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_CQI)
      {
         wrStartUeTmr(ueCb, WR_TMR_DETECT_RLF, WR_TMR_DETECT_RLF_VAL);
      }
#endif
      
   }


   /* update the UE attached flag */
   s1TgtHoTrans->isUeAttached = TRUE;
   /*Inform about new UE to SON*/
   wrIfmSonAddUeInd(ueCb);

   /* update ue capability information */
   ueCb->ueCap = s1TgtHoTrans->ueCap;
   SGetEpcTime(&ueCb->ueAttachTime);/*UE History info*/
   RLOG1(L_INFO, "wrUmmS1TgtHoHdlRrcConReConfgComplete: ueCb->ueAttachTime[%lu]", ueCb->ueAttachTime);
   
   /*Increment KPI for Intra-LTE Handovers Successful incoming Handover from eNodeB per Handover cause*/
   if ( FALSE == transCb->u.s1TgtHoTrans.isRrcReEstRcvd )
   {
      RLOG0(L_DEBUG, "wrUmmS1TgtHoHdlRrcConReConfgComplete: Incrementing \
            HovrSuccd counter"); 
      WR_INC_KPI_FUNC5(wrIncKpiFrHovrSuccd,WR_HO_TYPE_INTRA_LTE_S1,
            transCb->u.s1TgtHoTrans.sztHovrTyp,INC_HOV,
            (transCb->u.s1TgtHoTrans.sztCause), NULLP);
   }
   else /* TRUE == transCb->u.s1TgtHoTrans.isRrcReEstRcvd */
   {
      /* ccpu00141451 - Increment the HO success counter
       * After RRC-Reestablishemnt */
      RLOG0(L_DEBUG, "wrUmmS1TgtHoHdlRrcConReConfgComplete: Incrementing \
            HovrSuccd after Re-Establishment counter"); 
      WR_INC_KPI_FUNC(wrIncKpiIncHoSuccAftrReEst);
      transCb->u.s1TgtHoTrans.isRrcReEstRcvd = FALSE;
   }


   RETVALUE(ROK);
}


/* ccpu00128988 */

/**
 * @brief 
 *    This function is used to process Uu PDU.
 *
 * @details
 *    Function: wrUmmS1TgtHoPrcUuPdu
 *
 *    Processing steps:
 *       -# Based on message type, trigger corresponding message handler.
 *       -# if message type is RRCCONRECFGNCOMPL, process RRC Connection
 *          Reconfiguration Complete.
 *       -# if the message is UL NAS (TAU) then buffer that message & send to
 *       MME after TransRel.       
 *
 * @param[in] 
 *    transCb    : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmS1TgtHoPrcUuPdu
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
          /* Stop the TgtRelocTimer.*/
         wrStopUeTmr(transCb->ueCb, WR_TMR_TGTENB_RELOCEXE);

         RLOG_ARG0(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Received RRC "
            "Reconfiguration Complete");

         if (ROK != wrUmmS1TgtHoHdlRrcConReConfgComplete(transCb))
         {
            RLOG0(L_ERROR, "Handling of RRC Reconfiguration Complete failed");
            RETVALUE(RFAILED);
         }/* wrUmmS1TgtHoHdlRrcConReConfgComplete */
         RETVALUE(ROK);
      }
      break;
      case C1_UECAPINFO:
      {
         if( ueCb->ueCap != NULL )
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
            RLOG0(L_WARNING,"Release of transaction ID failed");
         }
         /*Stop Radio Capability Enquiry  Timer*/
         wrStopTransTmr(transCb, WR_TMR_RRC_CAPENQ);
         /* eCSFB:- Set the transCb state, as unable to store
          * ueCapInfo due to mem allocationfailue*/
         if(ROK != wrUmmHoUeCapStoreCapInfo(transCb->ueCb, ueCapInfo))
         {
            RETVALUE(RFAILED);
         }
         wrUmmSndUeCapEncReq(ueCb, 
                transCb->transId | WR_UMM_TGT_HO_UECAP_ENC , ueCapInfo);
      }
      break;
      default:
      {
         RLOG1(L_ERROR, "unknown msg recved msgId[%d]",rrcMsg->choice.val);
         RETVALUE(RFAILED);
         break;
      }
   }
   RETVALUE(ROK);
}
/* ccpu00128988 */


PUBLIC S16 wrUmmUtlUpdateSRBsInfo
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   /* Update SRB1 related information in the TransCb.*/
   switch (transCb->transTyp)
   {
      case WR_UMM_S1_TGT_HO_TRANS:
      case WR_UMM_X2_TGT_HO_TRANS:
      {
         wrUmmRrcFillUeSrbInfo(&ueCb->rbInfo, WR_PDCP_ID_SRB1);
         /* Update SRB2 related information in the UeCb.*/
         wrUmmRrcFillUeSrbInfo(&ueCb->rbInfo, WR_PDCP_ID_SRB2);
         break;
      }
      default:
      {
         RLOG0(L_ERROR, "Invalid Handover Type");
         RETVALUE(RFAILED);
      }
   } /* end of switch statement */
   RETVALUE(ROK);
}/* End of wrUmmUtlUpdateSRBsInfo*/

/** @brief This function performs UE Configuration 
 *         towards Physical Layer. 
 *
 * @details
 *
 *     Function: wrUmmS1TgtHoPhyUeCfg
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
PRIVATE S16 wrUmmS1TgtHoPhyUeCfg
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   CtfCfgReqInfo             *cfgInfo;
   WrUmmS1TgtHoTrans         *s1TgtHoTrans = &transCb->u.s1TgtHoTrans;

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
   s1TgtHoTrans->phyCfg = WR_UMM_TGT_HO_PHY_CFG_SENT;
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1TgtHoRscAlloc
(
WrUmmTransCb                 *transCb,
RmuUeHoRsp                   *hoAdmitRspInfo
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   U32                       suConId;

   WR_GET_S1AP_CON_ID(suConId, ueCb);
   ueCb->s1ConCb->suConnId = suConId;
   ueCb->s1ConCb->s1apConnState = WR_S1AP_CONNECTING;
   /*ccpu00129650*/
    /*wrUmmTgtHoInitUeCb(ueCb); */

   /* Commit the Radio Resource Config Parameters */
   wrUmmCommitUeRadioResDedInfo(transCb->ueCb,&(hoAdmitRspInfo->stUeRadioResCfg));
   if (ROK != wrUmmUtlUpdateSRBsInfo(transCb)) 
   {
      RLOG0(L_ERROR, "SRB Configuration failed");
      RETVALUE(RFAILED);
   }
#if 1
   wrUmmS1TgtHoPhyUeCfg(transCb);
#endif
   if (ROK != wrUmmS1TgtHoCfgSchd(transCb))
   {
      RLOG0(L_ERROR, "Scheduler configuration failed");
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}/* End of wrUmms1TgtHoRscAlloc*/

/** @brief This function is called to handle configuration confirm
 *         received from Physical layer.
 *
 * @details
 *
 *     Function: wrUmmS1TgtHoPrcPhyCfgCfm
 *
 *         Processing steps:
 *         - Performs the validation of Config Confim whether 
 *           it is success or failure. 
 *
 *
 * @param WrUmmTransCb  *transCb 
 * @return ROK/RFAILED 
 */
PRIVATE S16 wrUmmS1TgtHoPrcPhyCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmPhyCfgCfm            *phyCfm = NULLP;
   WrUmmS1TgtHoTrans         *s1TgtHoTrans = &transCb->u.s1TgtHoTrans;

   /*Get the PHY CFM Message*/
   phyCfm = &transCb->msg->u.phyCfm;

   if (phyCfm->status != CTF_CFG_CFM_OK)
   {
      RLOG0(L_ERROR, "Configuration failure");
      s1TgtHoTrans->phyCfg = WR_UMM_TGT_HO_PHY_CFG_FAIL;
      RETVALUE(RFAILED);
   }
   s1TgtHoTrans->phyCfg = WR_UMM_TGT_HO_PHY_CFG_SUCC;
   s1TgtHoTrans->state = WR_UMM_TGT_HO_PHY_CFG_DONE;
   RETVALUE(ROK);
}

/* Prepare Handover Failure Message.*/
PRIVATE S16 wrUmmS1TgtHoFillHovrFailure
(
WrUmmTransCb             *transCb,
S1apPdu              **hovrFail
)
{
   S1apPdu                     *hovrFailure = NULLP;
   SztS1AP_PDU                 *sztS1apPdu = NULLP;
   SztHovrFail                 *sztHovrFail = NULLP;
   SztProtIE_Field_HovrFailIEs *ie = NULLP;
   U16                         numComp;   
   S16                         ret = 0;
   
   /* Filling Handover Failure Message.*/
   ret = cmAllocEvnt(sizeof(S1apPdu), WR_SZ_MEM_SDU_SIZE, &wrCb.mem, (Ptr *)&hovrFailure);
   if (ret != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(ROK);
   }

   cmMemset((U8 *)&hovrFailure->pdu, 0, sizeof(SztS1AP_PDU));
   /* Fill Interface Elementary Procedures for Handover Failure.*/
   sztS1apPdu = &hovrFailure->pdu;
   wrFillTknU8(&(sztS1apPdu->choice), S1AP_PDU_UNSUCCESSFULOUTCOME);
   wrFillTknU8(&(sztS1apPdu->val.unsuccessfulOutcome.pres), PRSNT_NODEF);
   wrFillTknU32(&(sztS1apPdu->val.unsuccessfulOutcome.procedureCode),Sztid_HovrResourceAllocn);
   wrFillTknU32(&(sztS1apPdu->val.unsuccessfulOutcome.criticality),SztCriticalityrejectEnum);
   sztHovrFail = &sztS1apPdu->val.unsuccessfulOutcome.value.u.sztHovrFail;
   wrFillTknU8(&(sztHovrFail->pres),PRSNT_NODEF);
   numComp = 2;
   wrFillTknU16(&(sztHovrFail->protocolIEs.noComp),numComp);
   if ((cmGetMem(&hovrFailure->memCp,(numComp * sizeof (SztProtIE_Field_HovrFailIEs)),
               (Ptr *)&sztHovrFail->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   ie = &sztHovrFail->protocolIEs.member[0];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID),transCb->u.s1TgtHoTrans.mme_ue_s1ap_id);

   /* Fill The Cause.*/
   ie = &sztHovrFail->protocolIEs.member[1];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
   wrFillTknU8(&(ie->value.u.sztCause.choice),
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp);
   wrFillTknU32((TknU32*)&(ie->value.u.sztCause.val),
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal);
   WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause,
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp,
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal,INC_KPI_VALUE_BY_ONE);

   *hovrFail = hovrFailure;
   RETVALUE(ROK);
}
/* This Function is used to Build Handover Failure Message */
PRIVATE S16 wrUmmS1TgtHoSendHoPrepFail
(
WrUmmTransCb                *transCb
)
{
   SztRelReq                 relReq ={0};
   S1apPdu                   *hovrFail = NULLP;
   WrUmmCellCb               *cellCb = NULLP;  /*Cell CB required for KPI Increment*/

   if (ROK != wrUmmS1TgtHoFillHovrFailure(transCb, &hovrFail))
   {
      RLOG0(L_ERROR, "Failed To build Handover Failure Message");
      RETVALUE(RFAILED);
   }
   
   /* Filling SztConRsp*/
   relReq.connIdType = SZT_CONN_TYPE_SP;
   relReq.relType = SZT_CXT_REL;
   relReq.connId = transCb->ueCb->s1ConCb->spConnId;
   relReq.pdu = hovrFail;
   /* Sending Handover Failure Message.*/
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
      RLOG0(L_DEBUG,"Updating the KPI Counter for failure in Inbound Mobility");
      WR_INC_KPI_FUNC(wrIncKpiFrInboundCsgMobilityFaild);
   }
#endif

   RLOG_ARG1(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti, "[S1AP]:Sending HANDOVER FAILURE"
             "[MME-UE-S1AP-ID:%lu]",transCb->u.s1TgtHoTrans.mme_ue_s1ap_id); 
   wrPrintS1apCauseInfo(transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp,
                        transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal);
   WrIfmS1apRelReq(&relReq);
   RETVALUE(ROK);
}
PRIVATE S16 wrUmmS1TgtHoPrcSchCfm
(
 WrUmmTransCb                 *transCb
)
{
   WrUmmS1TgtHoTrans         *s1HoTrans = &(transCb->u.s1TgtHoTrans);
   U8                        idx;

   idx = transCb->msg->u.schdCfm.transId & 0x0f;
   if (idx > 15)
   {
      /*ccpu00127802*/
      RLOG_ARG0(L_ERROR, DBG_TRNSID,transCb->transId, "invalid transaction id "
         "for RAB Setup");
      RETVALUE(ROK);
   }
   /* ccpu00131426 - Fix */
   if ( idx == WR_UMM_TGT_HO_DRX_CFG )
   {
      if ( s1HoTrans->state == WR_UMM_TGT_HO_DRX_CFG_SENT )
      {
         if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
         {
            RLOG0(L_DEBUG, "=======DRX Cfg IS SuccessFull=====");
            s1HoTrans->state = WR_UMM_TGT_HO_DRX_CFG_DONE; 
            RETVALUE(ROK);
         }
         else
         {
            RLOG0(L_DEBUG, "=======DRX Cfg IS FAILED=====");
            RETVALUE(RFAILED);
         }
      }
   }
   if (idx == WR_UMM_TGT_HO_SCH_UECFG)
   {
      if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
      {
         /*ccpu00127802*/
         RLOG0(L_DEBUG, "=======UE CFG CFM IS SuccessFull=====");
      }
      else
      {
        /*ccpu00127802*/
         RLOG0(L_DEBUG, "=======UE CFG CFM IS FAILED=====");
   RETVALUE(RFAILED);
      }
      /* This is in response to UE reconfiguration for AMBR */
      s1HoTrans->schdUeCfgCfmRcvd = TRUE;

      /* if all the lch cfm's are setup prior to reception of 
       *  ue cfg cfm, set state to  success */
      if((s1HoTrans->schdLchCfgCmpl == TRUE) &&
            (s1HoTrans->schdSrb1LchCfgCmpl == TRUE) &&
            (s1HoTrans->schdSrb2LchCfgCmpl == TRUE) &&
            (s1HoTrans->lcgCfgCmpl == TRUE))
      {
         s1HoTrans->state = WR_UMM_TGT_HO_SCH_CFG_DONE;
      }
      RETVALUE(ROK);
   }
   /* check if the confirm is for SRB 1 */
   if(idx == WR_UMM_TGT_HO_SCH_SRB1CFG)
   {
      if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
      {
        /*ccpu00127802*/
         RLOG0(L_DEBUG, "=======SRB1 CFG IS SuccessFull=====");
      }
      else
      {
        /*ccpu00127802*/
         RLOG0(L_DEBUG, "=======SRB1 CFG  IS FAILED=====");
    RETVALUE(RFAILED);
      }
      s1HoTrans->schdSrb1LchCfgCmpl = TRUE;

      /* if all the lch cfm's are setup prior to reception of */
      /* ue cfg cfm, set state to success                     */
      if((s1HoTrans->schdLchCfgCmpl == TRUE) &&
            (s1HoTrans->schdUeCfgCfmRcvd == TRUE) &&
            (s1HoTrans->schdSrb2LchCfgCmpl == TRUE) &&
            (s1HoTrans->lcgCfgCmpl == TRUE))
      {
         s1HoTrans->state = WR_UMM_TGT_HO_SCH_CFG_DONE;
      }
      RETVALUE(ROK);
   }
   /* check if the confirm is for SRB 2 */
   if(idx == WR_UMM_TGT_HO_SCH_SRB2CFG)
   {
      if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
      {
        /*ccpu00127802*/
        RLOG0(L_DEBUG, "======SRB2 Cfg Is Success=====");
      }
      else
      {
        /*ccpu00127802*/
        RLOG0(L_DEBUG, " =========SRB2 CFG is Failed===== ");
    RETVALUE(RFAILED);
      }
      s1HoTrans->schdSrb2LchCfgCmpl = TRUE;

      /* if all the lch cfm's are setup prior to reception of */
      /* ue cfg cfm, set state to success                     */
      if((s1HoTrans->schdLchCfgCmpl == TRUE) &&
            (s1HoTrans->schdUeCfgCfmRcvd == TRUE) &&
            (s1HoTrans->schdSrb1LchCfgCmpl == TRUE) &&
            (s1HoTrans->lcgCfgCmpl == TRUE))
      {
         s1HoTrans->state = WR_UMM_TGT_HO_SCH_CFG_DONE;
      }
      RETVALUE(ROK);
   }

   /* ccpu00128203 */
   if((idx == WR_UMM_TRANS_ID_LCG0_CFG)||(idx == WR_UMM_TRANS_ID_LCG1_CFG)\
         ||(idx == WR_UMM_TRANS_ID_LCG2_CFG)||(idx == WR_UMM_TRANS_ID_LCG3_CFG))
   {
      if (transCb->msg->u.schdCfm.status != RGR_CFG_CFM_OK)
      {
         RLOG0(L_ERROR, " LCH Group Config Failed ");
         RETVALUE(RFAILED);
      }
      s1HoTrans->numlcgCfgCfm++;
      if(s1HoTrans->numlcgCfgCfm == s1HoTrans->expctLcgCfgCfm)
      {
         s1HoTrans->lcgCfgCmpl = TRUE;

         /**
          * if all the lch cfm's are setup prior to reception of ue cfg cfm, 
          * set state to success.
          */
         if((s1HoTrans->schdLchCfgCmpl == TRUE) && 
               (s1HoTrans->schdUeCfgCfmRcvd == TRUE) &&
               (s1HoTrans->schdSrb1LchCfgCmpl == TRUE) &&
               (s1HoTrans->schdSrb2LchCfgCmpl == TRUE))
         {
            s1HoTrans->state = WR_UMM_TGT_HO_SCH_CFG_DONE;
         }
      }
      RETVALUE(ROK);
   }

   if (idx > s1HoTrans->numOfRbCfgd)
   {
      /*ccpu00127802*/
      RLOG1(L_WARNING, " ALARM!! invalid transaction id %lu for RAB Setup ",
            transCb->transId);
      RETVALUE(ROK);
   }
   if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
   {
      s1HoTrans->rbInfo[idx].schdCfgState = WR_UMM_TGT_HO_RAB_SCH_CFG_SUCC;
      s1HoTrans->noOfAdmitdBearers++;
   }
   else
   {
      s1HoTrans->rbInfo[idx].schdCfgState = WR_UMM_TGT_HO_RAB_SCH_CFG_FAIL;
      RETVALUE(RFAILED);
   }
   for (idx = 0; idx < s1HoTrans->numOfRbCfgd; idx++)
   {
      if ((s1HoTrans->rbInfo[idx].schdCfgState == WR_UMM_TGT_HO_RAB_SCH_CFG_SENT)
            && (TRUE == s1HoTrans->rbInfo[idx].rabAllowed))
      {
         RETVALUE(ROK);
      }
   }

   /* only when all the lch cfm's are success */
   s1HoTrans->schdLchCfgCmpl = TRUE;

   /* only when UE Config is done, we are
    * setting the state to SUCCESS */
   if((s1HoTrans->schdUeCfgCfmRcvd == TRUE) &&
         (s1HoTrans->schdSrb1LchCfgCmpl == TRUE) &&
         (s1HoTrans->schdSrb2LchCfgCmpl == TRUE) &&
         (s1HoTrans->lcgCfgCmpl == TRUE))
   {
      s1HoTrans->state = WR_UMM_TGT_HO_SCH_CFG_DONE;
   }
   RETVALUE (ROK);
}

PRIVATE S16 wrUmmS1TgtHoRrcCfgAddSrbCfg
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


PRIVATE S16 wrUmmS1TgtHoRrcReconfigAddDrbCfg
(
U16                          cellId,
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


PRIVATE S16 wrUmmS1TgtHoFillRrcCfg
(
WrUmmTransCb                 *transCb,
NhuCfgReqSdus                *evnt
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   NhuRbCfgInfo              *drbCfg;
   /* WrUmmX2TgtHoRabInfo       *rabInfo; */
   NhuUeCfgInfo              *ueCfg;
   NhuRbCfgList              *rbCfgList;
   U32                       idx;
   U32                       numDrbs = 0;
   U16                       noOfRbConfgrd;

   ueCfg = &(evnt->cfgInfo);

   /* Fill up the UE configuration. */
   ueCfg->ueCfgBitMsk |= NHU_EXP_CFG_UE_CFG;
   ueCfg->ueInfo.newCrnti = ueCb->crnti;

   ueCfg->ueCfgBitMsk |= NHU_EXP_CFG_TRCH_CFG;
   wrUmmRrcFillTrchCfg(&(ueCfg->trChCfg),ueCb);

   ueCfg->ueCfgBitMsk |= NHU_EXP_CFG_RB_CFG;
   /* Fill RB info NHU_EXP_CFG_RB_CFG */
   ueCfg->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_ADD;
   rbCfgList = &(ueCfg->rbCfgAllList.rbToAddCfgList);
   noOfRbConfgrd = transCb->u.s1TgtHoTrans.noOfAdmitdBearers;
   /* allocating memory for numErabs + SRB1 and SRB2 */
   WR_GET_MEM(evnt, (sizeof(NhuRbCfgInfo) * noOfRbConfgrd + 2),
         &(rbCfgList->rbCfgInfoList));

   /* filling SRB1 configuration */
   wrUmmS1TgtHoRrcCfgAddSrbCfg(ueCb->cellId, 
                        &rbCfgList->rbCfgInfoList[0], TRUE);
   /* filling SRB2 configuration */
   wrUmmS1TgtHoRrcCfgAddSrbCfg(ueCb->cellId, 
                       &rbCfgList->rbCfgInfoList[1], FALSE);

   /* filling other ERAB configuration */
   for (idx = 0; idx < transCb->u.s1TgtHoTrans.numOfRbCfgd; idx++)
   {
     /* rabInfo = &(transCb->u.s1TgtHoTrans.rbInfo[idx]); */
      drbCfg  = &rbCfgList->rbCfgInfoList[numDrbs + 2];

      /* We send information of only those RABs that are successful */
      if ((transCb->u.s1TgtHoTrans.rbInfo[idx].rabAllowed == TRUE) &&
            (transCb->u.s1TgtHoTrans.rbInfo[idx].schdCfgState == WR_UMM_TGT_HO_RAB_SCH_CFG_SUCC))
      {
         wrUmmS1TgtHoRrcReconfigAddDrbCfg(ueCb->cellId, drbCfg, &transCb->u.s1TgtHoTrans.rbInfo[idx].drbInfo);
         numDrbs++;
      }
   }
   rbCfgList->numOfRbCfgInfo = numDrbs + 2;
   rbCfgList->isHO = TRUE;

   /* ccpu00126532 */
   /* Configure AS Security */
#ifdef TENB_AS_SECURITY
   ueCfg->ueCfgBitMsk |= NHU_EXP_CFG_SEC_CFG;
   wrUmmRrcFillSecCfg(&(ueCfg->secCfg), transCb->u.s1TgtHoTrans.secCfg);
#endif

   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1TgtHoRrcCfg
(
 WrUmmTransCb                 *transCb
)
{
   NhuCfgReqSdus             *rrcCfg;
   WrUeCb                    *ueCb   = transCb->ueCb;

   WR_ALLOCEVNT(&rrcCfg, sizeof(NhuCfgReqSdus));
   if (rrcCfg == NULLP)
   {
      RETVALUE(RFAILED);
   }
   rrcCfg->hdr.cellId  = ueCb->cellId;
   /* rrcCfg->hdr.ueId    = ueCb->crnti; */
   rrcCfg->hdr.ueId    = 0;
   rrcCfg->hdr.transId = transCb->transId | WR_UMM_TGT_HO_RRC_CFG;
   /* Build the configuration towards RRC */
   wrUmmS1TgtHoFillRrcCfg(transCb, rrcCfg);
   if (RFAILED == wrIfmRrcSndCfgReq(rrcCfg))
   {
      RLOG0(L_ERROR, " wrIfmRrcSndReqToUe failed. ");
      WR_FREEEVNT(rrcCfg);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1TgtHoPrcRrcCfgCfm
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
               RLOG0(L_ERROR, " DRB Config failed ");
            RETVALUE(RFAILED);
            }
            else
            {
               transCb->u.s1TgtHoTrans.state   = WR_UMM_TGT_HO_RRC_CFG_DONE;
            }
            break;
         }
      case WR_UMM_TGT_HO_SN_CFG:
         {
            if(rrcCfm->sdu.isCfgSuccess == FALSE)
            {
               RLOG0(L_ERROR, " DRB Config failed ");
               /* TODO :update state */
            }
            else
            {
               transCb->u.s1TgtHoTrans.state   = WR_UMM_TGT_HO_SN_CFG_DONE;
            }
            break;
         }
   }
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1TgtHoSndTnlCreateReq
(
 U32                          transId,
 WrUeCb                       *ueCb,
 WrUmmS1TgtHoRabInfo          *rabInfo,
 U32                          peerId,
 WrDamTnlType                 tnlType,
 WrHoType                     hoType
 )
{
   WrDamTnlInfo              *tnlInfo;
   CmTptAddr                 *lclAddr;
   /*CmTptAddr                 *peerAddr;*/

   CmTptAddr    dstIpAddr1234;
   S8           dummyIpv4[] = "1.2.3.4";
   S8           dummyIpv6[] = "::1";
   S8           str[46];

   WR_ALLOC_SHAREBLE_BUF(&tnlInfo, sizeof(WrDamTnlInfo));
   if(NULLP == tnlInfo)
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
   tnlInfo->hoType = hoType;
   
   /* Retreive the enb ip address. */
   /* We know that the eNB and SGW are connected,
    * so based on the SGW IP address type we retrieve the eNB IP address.
    */
   wrEmmGetDatAppAddr(&lclAddr, rabInfo->drbInfo.sgwAddr.type);
   if(tnlType == WR_TNL_NORMAL)
   {
      wrCpyCmTptAddr(&tnlInfo->dstAddr, &(rabInfo->drbInfo.sgwAddr));
   }
   else
   {
      //wrCpyCmTptAddr(&tnlInfo->dstAddr, &(rabInfo->drbInfo.sgwAddr));
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
         wrUpdateIpAddr((U8 *)dummyIpv4, &dstIpAddr1234);
         dstIpAddr1234.u.ipv4TptAddr.port = 2152;
         wrCpyCmTptAddr(&tnlInfo->dstAddr, &(dstIpAddr1234));
         cmInetNtop(tnlInfo->dstAddr.type,&tnlInfo->dstAddr.u.ipv4TptAddr.
          address,str,46);
      }
      else if(lclAddr->type == CM_TPTADDR_IPV6)
      {
         wrUpdateIpAddr((U8 *)dummyIpv6, &dstIpAddr1234);
         dstIpAddr1234.u.ipv6TptAddr.port = 2152;
         wrCpyCmTptAddr(&tnlInfo->dstAddr, &(dstIpAddr1234));
         cmInetNtop(tnlInfo->dstAddr.type,&tnlInfo->dstAddr.u.ipv6TptAddr.
          ipv6NetAddr,str,46);
      }
      RLOG_STR(L_DEBUG, " PKT_FWD_TNL IP :: %s ",str);
   }
   wrCpyCmTptAddr(&tnlInfo->srcAddr, lclAddr);
   tnlInfo->remTeid = rabInfo->drbInfo.remEgtpTunId;
   wrUtlGetDscp(ueCb->cellId, rabInfo->drbInfo.qci, &(tnlInfo->dscp));
   rabInfo->tnlCfgState = WR_UMM_TGT_HO_TNL_CFG_SENT;
   wrIfmDamTnlCreatReq(transId, tnlInfo);
   RETVALUE(ROK);
}

#define WR_UMM_CALC_NUM_TNL_CFG(_rabInfo) \
{\
   /* By default 1. For normal tunnel */\
   U8 _numTnls = 1;\
   if(_rabInfo->dlFwding == TRUE)\
   {\
      if(rabInfo->drbInfo.rbDir == WR_DIR_UL)\
      {\
         _numTnls++;\
      }\
      if(rabInfo->drbInfo.rbDir == WR_DIR_DL)\
      {\
         _numTnls++;\
      }\
      if(rabInfo->drbInfo.rbDir == WR_DIR_DL_UL)\
      {\
         _numTnls += 2;\
      }\
   }\
   _rabInfo->expectedTnlRsp += _numTnls;\
}

PRIVATE S16 wrUmmS1TgtHoCfgTnls
(
 WrUmmTransCb                 *transCb
)
{
   WrUmmS1TgtHoTrans         *s1TgtHoTrans = &transCb->u.s1TgtHoTrans;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmS1TgtHoRabInfo       *rabInfo = NULLP;
   U32                       transId;
   U16                       idx;
   U16                       noOfRbs;
   U32                       peerId = 0;
   WrHoType                  hoType = WR_HO_TYPE_NONE;
   
   noOfRbs = s1TgtHoTrans->numOfRbCfgd;

   if(SztHovrTypintralteEnum == transCb->u.s1TgtHoTrans.sztHovrTyp)
   {
      hoType = WR_HO_TYPE_INTRA_LTE_S1;
   }
   /* HO from Geran */
   else if ( SztHovrTypgerantolteEnum == transCb->u.s1TgtHoTrans.sztHovrTyp)
   {
      hoType = WR_HO_TYPE_IRAT_GERAN;
   }
   else
   {
      hoType = WR_HO_TYPE_IRAT_UTRA;
   }

   for (idx = 0; idx < noOfRbs; idx++)
   {
      rabInfo = &s1TgtHoTrans->rbInfo[idx];
      /* TODO : temporarily added. need to remove when DAM becomes loosely coupled */
      if ( TRUE == rabInfo->rabAllowed)
      {
         WR_UMM_CALC_NUM_TNL_CFG(rabInfo);
         transId = transCb->transId | idx;
         wrUmmS1TgtHoSndTnlCreateReq(transId, ueCb, rabInfo, peerId,
               WR_TNL_NORMAL, hoType);
         if( TRUE == rabInfo->dlFwding)
         {
           if(( WR_DIR_UL == rabInfo->drbInfo.rbDir) ||
              ( WR_DIR_DL_UL == rabInfo->drbInfo.rbDir))
           {
               if(hoType == WR_HO_TYPE_INTRA_LTE_S1)
            {
               wrUmmS1TgtHoSndTnlCreateReq(transId, ueCb, rabInfo, peerId,
                        WR_TNL_UL_DATFWD, hoType);
            }
              else
              {
                 /* Since IRAT HO does not required to create UL forward TEID 
                  * Expected bearer count shall be decremented */
                 rabInfo->expectedTnlRsp--; 
              }
           }
            if(( WR_DIR_DL == rabInfo->drbInfo.rbDir) ||
               ( WR_DIR_DL_UL == rabInfo->drbInfo.rbDir))
            {
               wrUmmS1TgtHoSndTnlCreateReq(transId, ueCb, rabInfo, peerId,
                     WR_TNL_DL_DATFWD, hoType);
            }
         }
      }
   }
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1TgtHoPrcDamCfm
(
 WrUmmTransCb                  *transCb
)
{
   WrUmmS1TgtHoTrans         *s1TgtHoTrans = &transCb->u.s1TgtHoTrans;
   WrUmmS1TgtHoRabInfo       *rabInfo = NULLP;
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

   noOfRbs = s1TgtHoTrans->numOfRbCfgd;
   for (idx = 0; idx < noOfRbs; idx++)
   {
      rabInfo = &s1TgtHoTrans->rbInfo[idx];
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
      }
   }
   for(idx = 0; idx < noOfRbs; idx++)
   {
      rabInfo = &s1TgtHoTrans->rbInfo[idx];
      if((TRUE == rabInfo->rabAllowed) && ((rabInfo->tnlCfgState == WR_UMM_TGT_HO_TNL_CFG_SENT) ||
            (rabInfo->expectedTnlRsp != 0)))
      {
         RETVALUE(ROK);
      }
   }
   s1TgtHoTrans->state = WR_UMM_TGT_HO_TNL_CFG_DONE;
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1TgtHoCommitUeInfo
(
 WrUmmTransCb                 *transCb
 )
{
   WrUmmS1TgtHoTrans         *s1HoTrans = &(transCb->u.s1TgtHoTrans);
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmS1TgtHoRabInfo       *transRbInfo;
   WrUeRAB                   *ueRbInfo;
   U8                        idx;
   U8                        freeIdx;
   WrUmmCellCb               *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   /* update bearer information
    * */
   for(idx = 0; idx < s1HoTrans->numOfRbCfgd; idx++)
   {
      if(s1HoTrans->rbInfo[idx].rabAllowed)
      {
         transRbInfo = &s1HoTrans->rbInfo[idx];
         freeIdx = transRbInfo->drbInfo.eRABId - WR_UMM_ERAB_OFFSET;
         ueRbInfo = &ueCb->rbInfo.rab[freeIdx];

         ueRbInfo->inUse    = TRUE;
         ueRbInfo->eRABId   = transRbInfo->drbInfo.eRABId;
         ueRbInfo->rbId     = transRbInfo->drbInfo.rbId;
         ueRbInfo->rbDir    = transRbInfo->drbInfo.rbDir;
         ueRbInfo->rbMode   = transRbInfo->drbInfo.rbMode;
         ueRbInfo->lchId    = transRbInfo->drbInfo.lchId;
         ueRbInfo->qci      = transRbInfo->drbInfo.qci;
         ueRbInfo->mbr.dl   = transRbInfo->drbInfo.mbr.dl;
         ueRbInfo->mbr.ul   = transRbInfo->drbInfo.mbr.ul / 8;
         ueRbInfo->gbr.dl   = transRbInfo->drbInfo.gbr.dl;
         ueRbInfo->gbr.ul   = transRbInfo->drbInfo.gbr.ul / 8;
         ueRbInfo->priority = transRbInfo->drbInfo.priority;
         ueRbInfo->preempCap = transRbInfo->drbInfo.preempCap;
         ueRbInfo->preempVul = transRbInfo->drbInfo.preempVul;
         ueRbInfo->isEmergencyBearer = transRbInfo->drbInfo.isEmergencyBearer;
         if(TRUE == ueRbInfo->isEmergencyBearer)
         {
            ueCb->ueServTyp = WR_UMM_EMERGENCY_SERV; 
         }
         /* SPS changes starts */
         ueRbInfo->isDlSpsEnabled   = transRbInfo->isDlSpsEnabled;
         ueRbInfo->isUlSpsEnabled   = transRbInfo->isUlSpsEnabled;
         /* SPS changes ends */
         if(((ueRbInfo->gbr.ul != 0) || (ueRbInfo->gbr.dl != 0)) && (ueRbInfo->qci <=4))
            ueRbInfo->isGbrQosPres = TRUE;

         ueRbInfo->lcgId = wrUmmGetLcgIdForQci(ueCb->cellId, transRbInfo->drbInfo.qci);

         if (WR_UMM_IS_GBR_BEARER(transRbInfo->drbInfo.qci))
         {
            /* Update new GBR value */
            ueCb->mappedLcgInfo[ueRbInfo->lcgId].effUlGbr += ueRbInfo->gbr.ul;
            ueCb->mappedLcgInfo[ueRbInfo->lcgId].effUlMbr += ueRbInfo->mbr.ul;
         }

         ueRbInfo->locEgtpTunId = transRbInfo->drbInfo.locEgtpTunId;
         ueRbInfo->remEgtpTunId = transRbInfo->drbInfo.remEgtpTunId;
         wrCpyCmTptAddr(&ueRbInfo->sgwAddr, &transRbInfo->drbInfo.sgwAddr);
      }
   }
   ueCb->rbInfo.numOfRbCfgd = s1HoTrans->noOfAdmitdBearers;
   /* update security information */
   ueCb->secCfg = s1HoTrans->secCfg;
   /* ccpu00126532 */
#ifdef TENB_AS_SECURITY
   ueCb->hzSecCfg = s1HoTrans->hzSecCfg;
#endif

   /* update the next hop chaining count information from mme */
   ueCb->nhChainCnt = s1HoTrans->new_nextHopChainingCount;

   /* update mme id */
   ueCb->s1ConCb->mme_ue_s1ap_id = s1HoTrans->mme_ue_s1ap_id;

   /* update ambr info */
   ueCb->rbInfo.aggBr.dl = transCb->u.s1TgtHoTrans.ambr.dl;
   ueCb->rbInfo.aggBr.ul = transCb->u.s1TgtHoTrans.ambr.ul;

   WR_MEM_COPY(&ueCb->srcGummei, &s1HoTrans->gummei, sizeof(WrGummei));

   WR_MEM_COPY(&ueCb->s1ConCb->mmeUeS1apId2, &s1HoTrans->mmeUeS1apId2,
         sizeof(TknU32));

   /* update ue capability information */
   if(s1HoTrans->ueCap != NULLP)
   {
      ueCb->ueCap = s1HoTrans->ueCap;
      ueCb->ueCat = ueCb->ueCap->eUTRA_Cap.ue_Category.val;
      /* RRC-REL10-Upgrade */
      wrUtlUpdateUeCatV1020(ueCb, ueCb->ueCap);
   }

   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1TgtHoPrepHovrCmd
(
 WrUmmTransCb                *transCb
)
{
   WrUeCb                    *ueCb  = transCb->ueCb;
   NhuEncReqSdus             *hoCmdEncReq = NULLP;
   NhuEncCfmSdus             *encCfm = transCb->msg->u.encCfm;
   MsgLen                    cnt = 0;
   S16                       retVal = ROK;
   MsgLen                    len = 0;
   NhuHovrCmdcriticalExtnsc1 *c1;

   /* trigger NhuEncodeReq to encode Tgt to Src
    * Transparent Container */
   WR_ALLOCEVNT(&hoCmdEncReq, sizeof(NhuEncReqSdus));
   if(NULLP == hoCmdEncReq)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Fill Handover Cmd Header */
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
   /* Find the length of the received IP packet */
   retVal = SFndLenMsg(encCfm->sdu.mBuf, &len);
   if((len == 0) || (retVal != ROK))
   {
      RLOG1(L_ERROR, " In correct buffer len %d ", len);
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp = 
                                              WR_CAUSE_RADIONW;
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal = 
                                 SztCauseRadioNwunspecifiedEnum;
      RETVALUE(RFAILED);
   }
   c1->val.handoverCmd_r8.handoverCmdMsg.pres = 1;
   c1->val.handoverCmd_r8.handoverCmdMsg.len = len;
   /* Allocate memory for the flat buffer */
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

   if(ROK != wrIfmRrcSndEncodeReq(hoCmdEncReq))
   {
      RLOG0(L_ERROR, " Failed to send Encode Req to RRC ");
      /* Fix for ccpu00125004 */
      WR_FREE(hoCmdEncReq,sizeof(NhuEncReqSdus));
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1TgtHoprepTgtEnbToSrcEnbcont
(
 WrUmmTransCb                *transCb
)
{
   SztEncDecEvnt             sztEncDecEvnt;
   MsgLen                    cnt = 0;
   S16                       ret = ROK;
   MsgLen                    len = 0;
   SzIePdu                   *iePdu;

   /* Allocate memory for event structure */
   /* Fix for ccpu00129931 */
   sztEncDecEvnt.transId = transCb->transId;
   sztEncDecEvnt.type.pres = PRSNT_NODEF;
   /* Set Type of Encode.*/
   sztEncDecEvnt.type.val = SZT_REQ_TYPE_ENCODE_IE;
   sztEncDecEvnt.ieType.pres = PRSNT_NODEF;
   /* Set Type of IE Encode.*/
   sztEncDecEvnt.ieType.val = SZT_REQ_TYPE_ENCDEC_IE_TGETSRC;
   /* Allocate memory for SzIePdu */
   WR_ALLOCEVNT(&sztEncDecEvnt.u.iePdu, sizeof(SzIePdu));
   if(NULLP == (sztEncDecEvnt.u.iePdu))
   {
      RLOG0(L_FATAL,"Memory allocation failed.");
      RETVALUE(RFAILED);
   }
 
   iePdu = sztEncDecEvnt.u.iePdu;
   wrFillTknU8(&iePdu->u.tgetToSrc.pres, PRSNT_NODEF);
   /* Fill RRC Container */
   ret = SFndLenMsg(transCb->msg->u.encCfm->sdu.mBuf, (S16 *)&len);
   if((len == 0) || (ret != ROK))
   {
      RLOG0(L_ERROR, "Incorrect buffer len ");
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp = 
                                              WR_CAUSE_RADIONW;
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal = 
                                SztCauseRadioNwunspecifiedEnum;
      RETVALUE(RFAILED);
   }
   iePdu->u.tgetToSrc.rRC_Cont.pres = TRUE;
   iePdu->u.tgetToSrc.rRC_Cont.len = len;
   cnt = 0;
   /* Allocate memory for the flat buffer */
   if ((cmGetMem(&(iePdu->memCp), len,
               (Ptr *)&(iePdu->u.tgetToSrc.rRC_Cont.val))) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   if(NULLP == (iePdu->u.tgetToSrc.rRC_Cont.val))
   {
      RLOG0(L_FATAL, " Memory allocation failed. ");
      RETVALUE(RFAILED);
   }
   
   SCpyMsgFix(transCb->msg->u.encCfm->sdu.mBuf,0, len,
    (Data *)iePdu->u.tgetToSrc.rRC_Cont.val,(S16 *)&cnt);

   iePdu->u.tgetToSrc.iE_Extns.noComp.pres = NOTPRSNT;
   /* Need To Call function to encode single IE.*/
   if(ROK != WrIfmS1apEncDecReq(&sztEncDecEvnt))
   {
      RLOG0(L_ERROR, " Failed to send Encode Req to RRC ");
      RETVALUE(RFAILED);
   }

   /*ccpu00129829*/
   //WR_FREE(sztEncDecEvnt, sizeof(SztEncDecEvnt));

   RETVALUE(ROK);
}

/*UE History info - start*/

/**
 * @brief 
 *    This function is used to Fill UE History Information List.
 *
 * @details
 *    Function: wrUmmS1TgtHoStoreUeHistoryInform
 *
 *    Processing steps:
 *       -# Fill UE History Information List.
 *       
 * @param[in] 
 *    transCb    : transaction information
 * @param[in]
 *    historyInfo : to fill UE History Information.
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmS1TgtHoStoreUeHistoryInform
(
WrUmmTransCb                 *transCb,
SztUE_HistoryInform          *historyInfo
)
{
   /* filling UE history information */
   U8                         cellIdx = 0;
   U8                         idx = 0;
   SztLastVisitedCell_Item    *cellItem;
   WrUeCb                     *ueCb = transCb->ueCb;
   WrUeHstryEutranInfo        *eutraCellInfo;
   TknStrOSXL                 *utraCellInfo;
  
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
            SztLastVisitedEUTRANCellInform  *eUTRANCell;
            SztProtExtnField_LastVisitedEUTRANCellInform_ExtIEs *ie = NULLP;
            eutraCellInfo = &(ueCb->ueHstryInfo.cellInfo[cellIdx].
                            ratInfo.eutranInfo);
            eUTRANCell = &cellItem[cellIdx].val.e_UTRAN_Cell;

            /* parse ECGI ID */
            wrUtlGetPlmnId(&eutraCellInfo->ecgi.plmnId,
               &cellItem[cellIdx].val.e_UTRAN_Cell.global_Cell_ID.pLMNidentity);
            wrUtlGetU32FrmBStr32(&eutraCellInfo->ecgi.eutranCellId,
                           &cellItem[cellIdx].val.e_UTRAN_Cell.global_Cell_ID.
                           cell_ID);
            /* Receiving cell type */
            eutraCellInfo->cellSize = cellItem[cellIdx].
               val.e_UTRAN_Cell.cellTyp.cell_Size.val;
            /* Receiving time UE stayed in cell */
            eutraCellInfo->timeInCell = (U32)(cellItem
               [cellIdx].val.e_UTRAN_Cell.time_UE_StayedInCell.val);
            /* Receiving time UE stayed in cell enhanced granularity */
            if(eUTRANCell->iE_Extns.noComp.val != 0)
            {
               for(idx=0;idx < eUTRANCell->iE_Extns.noComp.val;idx++)
               {
                  ie = &eUTRANCell->iE_Extns.member[idx];
                  if(ie->pres.pres != NOTPRSNT)
                  { 
                     if(ie->extensionValue.u.sztTime_UE_StayedInCell_EnhancedGranularity.pres != NOTPRSNT)
                     {
                        eutraCellInfo->timeInCellEnhGranularity = (U32)ie->extensionValue.u.sztTime_UE_StayedInCell_EnhancedGranularity.val;
                     }
                  }
               }
            }
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
}/* End of wrUmmS1TgtHoStoreUeHistoryInform */
/*UE History info - end*/

/**
 * This function is used to process decode of Source eNB to Target eNB
 * Transparent container.
 */
PRIVATE S16 wrUmmS1TgtHoPrcSrcToTgtDecCfm
(
WrUmmTransCb                 *transCb
)
{
   U8                            idx = 0;
   U8                            idx1 = 0;
   U8                            nosOfeRABSCfgd = 0;
   U8                            nosOfFwdRabs = 0;
   SztEncDecEvnt                 *decCfm = transCb->msg->u.sztDecCfm;
   SztSrceNB_ToTgeteNB_TprntCont *srcEnbToTgtEnb = NULLP;
   NhuDecodeSduReq               *nhuDecodeSduReq = NULLP;
   Buffer                        *mBuf = NULLP;
   WrUmmS1TgtHoTrans             *s1TgtHoTrans = NULLP;
   SztProtIE_SingleCont_E_RABInformLstIEs  *eRabInfoLst = NULLP;
   SztUE_HistoryInform           *ueHistoryInform = NULLP;/*UE History info*/

   /**
    * Store Sorce eNB To Target eNB Transparent
    * Container into transaction control block
    */
   /* TODO : store srcToTgt container info in ueCb */

   srcEnbToTgtEnb = &decCfm->u.iePdu->u.srcToTget;

   /*  HOFIX - store the plmn id */
   wrUtlGetPlmnId(&transCb->ueCb->plmnId,
         &srcEnbToTgtEnb->targetCell_ID.pLMNidentity);

   /* Set the dlFwding Flag based on the dlFwd Proposed IE sent in Src to Tgt Trans Cont. */
   s1TgtHoTrans = &transCb->u.s1TgtHoTrans;
   if(srcEnbToTgtEnb->e_RABInformLst.noComp.val != 0)
   {
      nosOfFwdRabs = srcEnbToTgtEnb->e_RABInformLst.noComp.val;
      nosOfeRABSCfgd = transCb->u.s1TgtHoTrans.numOfRbCfgd;
      for(idx = 0; idx < nosOfFwdRabs; idx++)
      {
         eRabInfoLst = &srcEnbToTgtEnb->e_RABInformLst.member[idx];
         for(idx1 = 0; idx1 < nosOfeRABSCfgd; idx1++)
         {
            if((eRabInfoLst->value.u.sztE_RABInformLstItem.e_RAB_ID.val) ==
                  (s1TgtHoTrans->rbInfo[idx1].drbInfo.eRABId))
            {
               if((eRabInfoLst->value.u.sztE_RABInformLstItem.dL_Fwding.pres == PRSNT_NODEF)                     
                     && (s1TgtHoTrans->rbInfo[idx1].drbInfo.isDataFwdAvl == TRUE))
               {
                  s1TgtHoTrans->rbInfo[idx1].dlFwding = TRUE;
               }
               break;
            }
         }
      }   
   }
/*UE History info - start*/
   /** fill wrUmmS1TgtHoStoreUeHistoryInform **/
   ueHistoryInform = &srcEnbToTgtEnb->uE_HistoryInform;
  if (ROK != wrUmmS1TgtHoStoreUeHistoryInform(transCb, ueHistoryInform))
   {
      WR_DBGP(WR_ERROR,(WR_PRNT_BUF,"wrUmmX2TgtHoStoreUeHistoryInform:Failed to Parse UE History Information."));
      RETVALUE(RFAILED);
   }/* End of wrUmmS1TgtHoStoreUeHistoryInform*/
/*UE History info - end*/


   /* if the Subscriber Profile ID value is present, store the same in the
    * ueCb which will be passed to target eNodeB during S1/X2 handover case
    */
   if(TRUE == srcEnbToTgtEnb->subscriberProfileIDforRFP.pres)
   {
      transCb->ueCb->subscProfileId.pres = TRUE;
      transCb->ueCb->subscProfileId.val = 
                      srcEnbToTgtEnb->subscriberProfileIDforRFP.val;
   }
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
   }
   
   /* convert the OSXL to Buffer */
   SAddPstMsgMult((Data *)srcEnbToTgtEnb->rRC_Cont.val,
         (MsgLen)srcEnbToTgtEnb->rRC_Cont.len,
         (Buffer *)mBuf);
   nhuDecodeSduReq->mbuf = mBuf;
   /* Decode the HO Preparation Message.
    */
   if(wrIfmRrcSndDecodeReq(nhuDecodeSduReq)!=ROK)
   {
      /* Fix for ccpu00125004 */
      WR_FREE(nhuDecodeSduReq , sizeof(NhuDecodeSduReq));
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


PRIVATE S16 wrUmmS1TgtHoPrcRrcEncCfm
(
 WrUmmTransCb                *transCb
)
{
   NhuEncCfmSdus             *encCfm = transCb->msg->u.encCfm;
   U8                        subTrans;

   if(encCfm->sdu.status)
   {
      /* TODO  : add error log */
      /*ccpu00127802*/
      RLOG0(L_ERROR, " Encoding of RRC reconfig failed ");
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW;
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal = 
         SztCauseRadioNwunspecifiedEnum;            
      RETVALUE(RFAILED);
   }
   subTrans = encCfm->hdr.transId & 0x0f;
   switch(subTrans)
   {
      case WR_UMM_TGT_HO_DLDCCH_MSG_ENC:
         {
            if (ROK != wrUmmS1TgtHoPrepHovrCmd(transCb))
            {
               RLOG0(L_ERROR, " Failed to Prepare HO Command ");
               RETVALUE(RFAILED);
            }
         }
         break;

      case WR_UMM_TGT_HO_HOCMD_ENC:
         {
            if (ROK != wrUmmS1TgtHoprepTgtEnbToSrcEnbcont(transCb))
            {
               RLOG0(L_ERROR, " Failed to Prepare HO Command ");
               RETVALUE(RFAILED);
            }
         }
            break;
      case WR_UMM_TGT_HO_UECAP_ENC:
         {
            wrUmmUeCapHdlUeCapEncCfm(transCb->ueCb, encCfm, 
                                      transCb->transId | WR_UMM_TGT_HO_RACAP_ENC);
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

/**
  * @brief This function is used to process 
  * the S1 Handover Request Message.
  */
U32 hovrAttempted;
PRIVATE S16 wrUmmS1TgtHoPrcHoReqMsg
(
WrUmmTransCb                 *transCb
)
{
   WrUmmS1TgtHoTrans         *s1TgtHoTrans = &(transCb->u.s1TgtHoTrans);
   /* CSG_DEV */
    WrUmmCellCb               *cellCb = NULLP;  /*Cell CB required for KPI Increment*/
   /**
    * Upon reception of Handover Request Message from the source eNB,
    * Target eNB shall parse all IEs and stores into transaction control 
    * block to use in future and set the state as HANDOVER_PREPARATION.
    */
   /* Fill UE Id.*/
   s1TgtHoTrans->crnti = transCb->ueCb->crnti;
   /* Fill UE Handover State at the time of Handover.*/
   s1TgtHoTrans->state = WR_HANDOVER_PREPARATION;
   /* initialise flags */
   s1TgtHoTrans->isUeAttached = FALSE;
   s1TgtHoTrans->snStatusInfoFlag = WR_UMM_TGT_HO_SN_INFO_NOWAIT;
   
   WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId);

   if(cellCb == NULLP)
   {
      RLOG1(L_ERROR, " Error in getting CELL CB for cellId = %lu ",
            (U32)transCb->ueCb->cellId);
      RETVALUE(RFAILED);
   }
   /* Parse the S1 HO Request Message */
   if (ROK != wrUmmS1TgtHoParseHoReqMsg(transCb))
   {

       /* Prepare Handover Failure Message.*/
      /* The Cause is UnSpecefied because failure is happened in
       * application itself */
      /* example, when we allocate the memory  */
      
      if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb))
      {
         RLOG0(L_ERROR, "Failed To Send Handover Failure");
      }
      transCb->state = WR_UMM_TRANS_DELETE_UE;
      RETVALUE(RFAILED);
   }/* End of wrUmmS1ParseHoReqMsg*/

   if(RFAILED == wrUmmCsgValidateInbndS1Ho(transCb->ueCb,
            transCb->u.s1TgtHoTrans.csgId,
            transCb->u.s1TgtHoTrans.ueMbrShpSts,
            transCb->u.s1TgtHoTrans.isEmrgncyCall,
            (U8*)&transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp,
            (U8*)&transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal))
   {
      RLOG1(L_INFO, "Inbound S1 handover Admiton failed for UE [%d]",
            transCb->ueCb->crnti);
      wrUmmS1TgtHoSendHoPrepFail(transCb);
      transCb->state = WR_UMM_TRANS_DELETE_UE;
      RETVALUE(RFAILED);
   }


   
   /*Increment KPI for Intra-LTE Handovers Attempted incomming Handover from eNodeB per Handover cause*/
   transCb->ueCb->hovDir = INC_HOV;
#ifdef WR_RSYS_KPI 
   transCb->ueCb->hov = transCb->u.s1TgtHoTrans.sztHovrTyp;
   /* for incoming HO and Outgoing Ho we use same wrIncKpiFrHovrAttmptd function 
    * for KPI, and differntiated by direction of Ho */
   WR_INC_KPI_FUNC5(wrIncKpiFrHovrAttmptd,WR_HO_TYPE_INTRA_LTE_S1,
         transCb->u.s1TgtHoTrans.sztHovrTyp,
   INC_HOV,(transCb->u.s1TgtHoTrans.sztCause), NULLP);
   /* Updating KPI Counter for attempted HO to CSG or Hybrid Cell */
   if ((cellCb->csgAccessMode == WR_ENB_CLOSED_ACCESS)||
         (cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS))
   {
      RLOG0(L_DEBUG,"Updating the KPI Counter for Attempted Inbound Mobility");
      WR_INC_KPI_FUNC(wrIncKpiFrInboundCsgMobilityAttmptd);
   }
#endif/*WR_RSYS_KPI*/
   hovrAttempted++;

   
   RETVALUE(ROK);
}/*End of wrUmmS1PrcHoReqMsg*/

PRIVATE S16 wrUmmS1TgtHoHdlS1ConPdu
(
WrUmmTransCb                *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;

   switch (msg->u.s1Pdu->pdu->pdu.choice.val)
   {
      case INITIATING_MSG:
      {
         /* Take action based on the procedure code */
         switch (msg->u.s1Pdu->pdu->pdu.val.initiatingMsg.procedureCode.val)
         {
            case Sztid_HovrResourceAllocn:
            {
               /* Process the S1AP HO Request */  
               if (ROK != wrUmmS1TgtHoPrcHoReqMsg(transCb))
               {
                  RLOG0(L_ERROR, " Failed to Process S1 HO Request Message. ");
                  RETVALUE(RFAILED);
               }/* End of wrUmmX2PrcHoReqMsg*/
               break;
            }
            default :
            RETVALUE(RFAILED);
         }
         break;
      }
      default :
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/**
  * This Function is used to Handle S1AP PDU received from the MME.
  */
PRIVATE S16 wrUmmS1TgtHoHdlS1apPdu
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   S16                       retVal = ROK;

   switch(msg->u.s1Pdu->pdu->pdu.choice.val)
   {
      case INITIATING_MSG:
      {
         /* Take action based on the procedure code */
         switch (msg->u.s1Pdu->pdu->pdu.val.initiatingMsg.procedureCode.val)
         {            
            case Sztid_MMEStatusTfr:
            {
               if (ROK != wrUmmS1TgtHoPrcEnbStatusTfr(transCb))
               {
                  RLOG0(L_ERROR, " Failed to Process ENB Status Transfer Message. ");
                  RETVALUE(RFAILED);
               }
               break;
            }
            default:
            {
               RLOG0(L_ERROR, " Invalid Procedure Code. ");
               retVal = RFAILED;
            }
         }/* End of switch */
         break;
      } /* end of INITIATING msg */
      case SUCCESSFUL_OUTCOME:
      {
         break;
      }
      case UN_SUCCESSFUL_OUTCOME:
      {      
         break;
      }
      default :
         RLOG0(L_ERROR, " Invalid Message. ");
    retVal = RFAILED;
   }/*End of switch.*/
   RETVALUE(retVal);
}/* End of wrUmmS1TgtHoHdlS1apPdu*/

#ifdef TENB_AS_SECURITY 
PRIVATE S16 wrUmmS1TgtHoHdlKdfResp
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmIncMsg               *msg = transCb->msg;
   WrUmmS1TgtHoTrans         *s1HoTrans = &(transCb->u.s1TgtHoTrans);
   WrUmmKdfCfm               *kdfCfm;
   CtfAskeyCfmInfo           *kEnbInf;

   kdfCfm = &(msg->u.kdfCfm);

   if(kdfCfm->status == WR_CFG_NOK)
   {
      /* kEnb Key / AS Keys generation failed */
      RLOG0(L_ERROR, " Key generation failure ");
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp = WR_CAUSE_NAS;
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal =
    SztCauseNasunspecifiedEnum;
      if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb))
      {
     RLOG0(L_ERROR,"Failed To Send Handover Failure");
      }
      transCb->state = WR_UMM_TRANS_DELETE_UE;
   }

   RLOG0(L_DEBUG, " **************************** ");
   RLOG0(L_DEBUG, " AS KEY GENERATION SUCESS ");
   RLOG0(L_DEBUG, " **************************** ");
   if(kdfCfm->ctfKdfCfm->keyDerType == CTF_KDF_TYPE1)
   {
      kEnbInf = &(kdfCfm->ctfKdfCfm->u.kEnbInf);
      if(s1HoTrans->asKeyGenerated == FALSE)
      {
         /*Store initial AS security keys*/
         cmMemcpy(s1HoTrans->secCfg->cpCiphKey, kEnbInf->cpCiphKey,
               WR_CIPH_KEY_LEN);
         cmMemcpy(s1HoTrans->secCfg->upCiphKey, kEnbInf->upCiphKey,
               WR_CIPH_KEY_LEN);
         cmMemcpy(s1HoTrans->secCfg->intgKey, kEnbInf->intKey,
               WR_INTG_KEY_LEN);
         /*ccpu00129252*/
         if(transCb->u.s1TgtHoTrans.state == WR_UMM_TGT_HO_SCH_CFG_DONE)
         {
            RLOG1(L_DEBUG, " [%d] DBG :: RRC Configuration with SecKeys ",
                  transCb->ueCb->crnti);
            if (ROK != wrUmmS1TgtHoRrcCfg(transCb))
            {
               RLOG0(L_ERROR, " Failed to Configure RRC. ");
               RETVALUE(RFAILED);
            }
         }
#if  !(defined SS_RBUF) && !(defined LTE_QCOM)
         wrPlatSecExtractSecKeys1(ueCb, transCb->transId, CTF_KDF_TYPE2);
#else
         wrUmmSecExtractSecKeys1(ueCb, transCb->transId, CTF_KDF_TYPE2);
#endif // SS_RBUF
         s1HoTrans->asKeyGenerated = TRUE;
      }
      else
      {
         /*store horizontally derived AS security keys*/
         cmMemcpy(s1HoTrans->hzSecCfg->cpCiphKey, kEnbInf->cpCiphKey,
               WR_CIPH_KEY_LEN);
         cmMemcpy(s1HoTrans->hzSecCfg->upCiphKey, kEnbInf->upCiphKey,
               WR_CIPH_KEY_LEN);
         cmMemcpy(s1HoTrans->hzSecCfg->intgKey, kEnbInf->intKey,
               WR_INTG_KEY_LEN);
      }
   }
   else if(kdfCfm->ctfKdfCfm->keyDerType == CTF_KDF_TYPE2)
   {
      if(s1HoTrans->isNewKenbGenerated == FALSE)
      {
         /* storing the new key generated from the key received from MME and
          * trigger generation of 3 Keys */
         cmMemcpy((U8*)&s1HoTrans->secCfg->kENB[0],
               kdfCfm->ctfKdfCfm->u.kEnbStarInf.secKey, WR_KENB_KEY_LEN);
#ifndef SS_RBUF
         wrPlatS1SecExtractSecKeys(s1HoTrans->secCfg, 
               &s1HoTrans->asKeyGenerated, transCb->transId);
#else
         wrUmmS1SecExtractSecKeys(s1HoTrans->secCfg, 
               &s1HoTrans->asKeyGenerated, transCb->transId);
#endif         
         s1HoTrans->isNewKenbGenerated = TRUE;
      }
      else
      {
         WR_ALLOC(&(s1HoTrans->hzSecCfg), sizeof(WrUeSecCfg));
         if (s1HoTrans->hzSecCfg == NULLP)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }

         /* Copy the security capabilities and the selected algorithm */
         s1HoTrans->hzSecCfg->ciphAlgo = s1HoTrans->secCfg->ciphAlgo;
         s1HoTrans->hzSecCfg->intgAlgo = s1HoTrans->secCfg->intgAlgo;
         s1HoTrans->hzSecCfg->encrypCap = s1HoTrans->secCfg->encrypCap;
         s1HoTrans->hzSecCfg->integCap = s1HoTrans->secCfg->integCap;

         cmMemcpy(s1HoTrans->hzSecCfg->kENB,
               kdfCfm->ctfKdfCfm->u.kEnbStarInf.secKey, WR_KENB_KEY_LEN);
         transCb->ueCb->hzSecCfg = s1HoTrans->hzSecCfg;
#if  !(defined SS_RBUF) && !(defined LTE_QCOM)
         wrPlatSecExtractSecKeys1(ueCb, transCb->transId, CTF_KDF_TYPE1);
#else
         wrUmmSecExtractSecKeys1(ueCb, transCb->transId, CTF_KDF_TYPE1);
#endif         
      }
   }
   RETVALUE(ROK);
}
#endif /* TENB_AS_SECURITY */
PRIVATE S16 wrUmmS1TgtHoPrepTmrExp
(
WrUmmTransCb                 *transCb
)
{
   RLOG1(L_ERROR, " : Timer Expired. Handover Preparation Failed ueCb[%d] ",
         transCb->ueCb->crnti);
   transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW;
   transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal = 
      SztCauseRadioNwunspecifiedEnum;
   if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb)) 
   {
      RLOG0(L_ERROR,"Failed To Send Handover Failure");
      transCb->state  = WR_UMM_TRANS_RELEASE_CALL;
      RETVALUE(RFAILED);
   }
   transCb->ueCb->mobCtrlState = WR_HO_UE_RELEASE;
   /* HO_CHANGES - Release transaction immediately after failure */
   transCb->state = WR_UMM_TRANS_RELEASE_CALL;
   RETVALUE(ROK);
}

PRIVATE S16 wrUmmS1TgtHoPrcTmrExpiry
(
WrUmmTransCb                 *transCb
)
{
   if(transCb->msg->u.tmrExpiry.timerEvnt == WR_TMR_TGT_HOPREP)
   {
      wrUmmS1TgtHoPrepTmrExp(transCb);
   }
   RETVALUE(ROK);
}


PRIVATE S16 wrUmmS1TgtHoProcHoAdmitReq
(
WrUmmTransCb                 *transCb
)
{
   RmuUeHoReq            *ueHoReq;
   WrUmmS1TgtHoRabInfo   *s1rbInfo = NULLP;
   U8                     erabIdx;
   /* SPS changes starts */
   WrUeCapInfo           *ueCap = transCb->u.s1TgtHoTrans.ueCap;
   /* SPS changes ends */
   /* HCSG DEV */
   RmuUErabAddModIe      *erabList = NULLP;
   RmuUErabQosInfoIe     *erabInfo = NULLP;
   WrUeRAB               *drbInfo  = NULLP;


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
      /* Ask APP team why this is done by checking feature indicator presence */
      ueHoReq->stUeCapInfo.stEutraCapInfo.uiUeCategory = ueCap->eUTRA_Cap.ue_Category.val;
   }
   wrUmmRrmFillUeCapInfo(transCb->ueCb->cellId, &ueHoReq->stUeCapInfo, ueCap);
   /* SPS changes ends */

   s1rbInfo = transCb->u.s1TgtHoTrans.rbInfo;
   ueHoReq->stErabConfInfo.stUeAmbrInfo.uiAggUEMaxBitRateDL = transCb->u.s1TgtHoTrans.ambr.dl;
   ueHoReq->stErabConfInfo.stUeAmbrInfo.uiAggUEMaxBitRateUL = transCb->u.s1TgtHoTrans.ambr.ul;
   ueHoReq->stErabConfInfo.usNoErabsToAddMod = transCb->u.s1TgtHoTrans.numOfRbCfgd;

    for (erabIdx =0; erabIdx < transCb->u.s1TgtHoTrans.numOfRbCfgd; erabIdx++)
    {
       erabList = ueHoReq->stErabConfInfo.stErabAddModLst;
       erabInfo = &erabList[erabIdx].stErabQosInfo;
       drbInfo  = &s1rbInfo[erabIdx].drbInfo;

       erabList[erabIdx].bErabId = drbInfo->eRABId;
       erabInfo->bErabQCI = drbInfo->qci;
       erabInfo->stErabGbrInfo.uiErabUlGbr = drbInfo->gbr.ul;
       /* Fill GBR info */
       erabInfo->stErabGbrInfo.uiErabDlMbr = drbInfo->mbr.dl;
       erabInfo->stErabGbrInfo.uiErabUlMbr = drbInfo->mbr.ul;
       erabInfo->stErabGbrInfo.uiErabDlGbr = drbInfo->gbr.dl;
       erabInfo->stErabGbrInfo.uiErabUlGbr = drbInfo->gbr.ul;
       /* Fill ARP info */
       erabInfo->stErabArpInfo.bErabPrioLvl = drbInfo->priority;
       erabInfo->stErabArpInfo.bErabPreemptCap = drbInfo->preempCap;
       erabInfo->stErabArpInfo.bErabPreemptVul = drbInfo->preempVul;
    }

    ueHoReq->stErabConfInfo.bIsUeAmbrPres = TRUE;

   /* For interRAT HO Radio resource config shall not received*/
    if(SztHovrTypintralteEnum == transCb->u.s1TgtHoTrans.sztHovrTyp)
    {
       wrUmmHoCopyRadioResCfg(&ueHoReq->stUeRadioResCfg, &transCb->u.s1TgtHoTrans.ueRadioResCfg);
    }

   if(transCb->ueCb->ueServTyp == WR_UMM_EMERGENCY_SERV)
   {
      ueHoReq->isEmrgncyBrersPsnt = TRUE;
      ueHoReq->isMember = TRUE;
   }
   else
   {
      ueHoReq->isEmrgncyBrersPsnt = FALSE;
      if(WR_UMM_CSG_MEMBER == transCb->u.s1TgtHoTrans.ueMbrShpSts)
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
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp = WR_CAUSE_RADIONW;
      transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal =
         SztCauseRadioNwho_target_not_allowedEnum; 
      wrUmmS1TgtHoSendHoPrepFail(transCb);
      transCb->state = WR_UMM_TRANS_DELETE_UE;
   }

   RETVALUE(ROK);
}

/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmS1TgtHoProcessor                                            */
/*                                                                        */
/* This function is called when any message related to a particular       */
/*    S1 HO is received. This is the main function that determines        */
/*    next course of action based on the message received.                */
/*                                                                        */
/* Steps involved in S1 HO Processor:                                     */
/*     Process message                                                  */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmS1TgtHoProcessor
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   RmuUeHoRsp                *hoAdmitRspInfo = NULLP;
   S16                        ret;
   /* SPS changes starts */
   NhuHovrPrepInfo               *nhuHovrPrepInfo;
   /* SPS changes ends */
   /*removed unused*/
   WrUeCapInfo                   *ueCap         = transCb->u.s1TgtHoTrans.ueCap;
   /*ccpu00130428*/
   WrUmmMsgCause                 *cause         = &(transCb->u.s1TgtHoTrans.hoPrepFailCause);
   WrUmmRatLst                        ratLst;
   U8                             idx = 0; /*RRM_ARP*/
   U8                             idx1 = 0; /*RRM_ARP*/
   /*ccpu00130428 : Default cause type and value*/
   
   cause->causeTyp = WR_CAUSE_RADIONW;
   cause->causeVal = SztCauseRadioNwunspecifiedEnum;

   switch(msg->msgTyp)
   {
      case WR_UMM_S1_CON_PDU:
         {
            /* Handle S1ap ConInd PDU.*/
            if (ROK != wrUmmS1TgtHoHdlS1ConPdu(transCb))
            {
               RLOG0(L_ERROR, " wrUmmS1TgtHoHdlS1apPdu: Failed to handle S1AP PDU. ");

               RETVALUE(RFAILED);
            }/* End of wrUmmS1HoHdlS1apPdu*/
            break;
         }

      case WR_UMM_RRM_UEHO_RESP:
         {
            /* ccpu00131426 - Fix */
            transCb->u.s1TgtHoTrans.rrmUeRecfgRspMsg = transCb->msg;
            transCb->msg->refCnt++;

            hoAdmitRspInfo = msg->u.rrmUeHoResp;

            if (hoAdmitRspInfo->stStatusInfo.enStatus == RMU_FAILURE)
            {
               /**
                * New User is not allowed
                */
               cause->causeTyp = WR_CAUSE_RADIONW;
               cause->causeVal = SztCauseRadioNwho_target_not_allowedEnum;
               if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb))
               {
                  RLOG0(L_ERROR, "wrUmmS1TgtHoProcessor: Failed To Send Handover Failure");
               }
               transCb->state  = WR_UMM_TRANS_DELETE_UE;
               RETVALUE(RFAILED);
            }
            /*DRX QCI*/
            if(hoAdmitRspInfo->isGBRBearerExists == TRUE)
            {
               transCb->ueCb->isGBRBearerExists = RAB_ESTABLISHED;
            }
            
            /*RRM_ARP : Copy ERABs rejected list to transCb from hoAdmitRspInfo*/
           if(hoAdmitRspInfo->bNoErabsRejected)
           {
              U8          numOfRbCfgd = transCb->u.s1TgtHoTrans.numOfRbCfgd;
              transCb->u.s1TgtHoTrans.erabsToRejectLst.numrabs =
                 hoAdmitRspInfo->bNoErabsRejected;
              for(idx = 0;idx < hoAdmitRspInfo->bNoErabsRejected; idx++)
              {
                 transCb->u.s1TgtHoTrans.erabsToRejectLst.erabId[idx] = 
                    hoAdmitRspInfo->stErabRejectLst[idx].bErabId;
                 transCb->u.s1TgtHoTrans.erabsToRejectLst.cause[idx] = 
                    hoAdmitRspInfo->stErabRejectLst[idx].enCause;
              }

              transCb->u.s1TgtHoTrans.erabsToRejectLst.causeTyp = WR_CAUSE_RADIONW;

              /*Mark the unallowed ERABs so that they will not be confiured by
               * lower layers*/
              for(idx = 0;idx < numOfRbCfgd;idx++)
              {
                if(transCb->u.s1TgtHoTrans.rbInfo[idx].drbInfo.eRABId == 
                      hoAdmitRspInfo->stErabRejectLst[idx1].bErabId)
                {
                   transCb->u.s1TgtHoTrans.rbInfo[idx].rabAllowed = FALSE;
                   idx1++;
                }
              }
           }

            /*
               wrUmmCommitUePhyCfgDedHoResInfo(transCb->ueCb, hoAdmitRspInfo);
               */

            if (ROK != wrUmmS1TgtHoRscAlloc(transCb,hoAdmitRspInfo))
            {
               RLOG0(L_ERROR, " Failed to process wrUmmS1TgtHoRscAlloc ");
               cause->causeTyp = WR_CAUSE_RADIONW;
               cause->causeVal = 
                  SztCauseRadioNwno_radio_resources_available_in_target_cellEnum;
               if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb))
               {
                  RLOG0(L_ERROR, " wrUmmS1TgtHoProcessor: Failed To Send Handover Failure");
               }
               transCb->state  = WR_UMM_TRANS_RELEASE_CALL;
               RETVALUE(RFAILED);
            }
            transCb->ueCb->hoType    = WR_HO_TYPE_INTRA_LTE_S1;

            transCb->ueCb->hoTransId = transCb->transId;

            break;
         }
      case WR_UMM_S1_DAT_IND:
         {
            /* Handle S1ap PDU.*/
            if (ROK != wrUmmS1TgtHoHdlS1apPdu(transCb))
            {
               RLOG0(L_ERROR, " wrUmmS1TgtHoHdlS1apPdu: Failed to handle S1AP PDU. ");
               RETVALUE(RFAILED);
            }/* End of wrUmmS1HoHdlS1apPdu*/ 
            break;
         } /* end of WR_UMM_S1_PDU */

      case WR_UMM_SCHD_CFG:
         {
            if (ROK != wrUmmS1TgtHoPrcSchCfm(transCb))
            {
               RLOG0(L_ERROR, " wrUmmS1TgtHoProcessor:Failed to Configure RRC. ");
               cause->causeTyp = WR_CAUSE_RADIONW;
               cause->causeVal = 
                  SztCauseRadioNwno_radio_resources_available_in_target_cellEnum; 
               if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb))
               {
                  RLOG0(L_ERROR, " wrUmmS1TgtHoProcessor: Failed To Send Handover Failure");
               }
               transCb->state = WR_UMM_TRANS_RELEASE_CALL;
               RETVALUE(RFAILED);
            }
            /*ccpu00130424*/
            if(transCb->u.s1TgtHoTrans.state == WR_UMM_TGT_HO_SCH_CFG_DONE
#ifdef TENB_AS_SECURITY    
                  /*ccpu00129252*/
                  && transCb->u.s1TgtHoTrans.asKeyGenerated == TRUE
#endif
              )
            {
               if (ROK != wrUmmS1TgtHoRrcCfg(transCb))
               {
                  RLOG0(L_ERROR,"Failed to Configure RRC.");
                  RETVALUE(RFAILED);
               }
            }
            /* ccpu00131426 - Fix */
            else if ( transCb->u.s1TgtHoTrans.state == WR_UMM_TGT_HO_DRX_CFG_DONE )
            {
               if(transCb->u.s1TgtHoTrans.snStatusInfoFlag == WR_UMM_TGT_HO_SN_INFO_NOWAIT ||
                     transCb->u.s1TgtHoTrans.snStatusInfoFlag == WR_UMM_TGT_HO_SN_INFO_GOT)
               {
                  /* Check for the UE Capability Info  when the supported band list IE in 
                   * the UE Radio Capability IE contains IRAT bands and specific IRAT octet 
                   * string is not present in the Source to Target Container which is updated in
                   * the ueCb, then send UECAP ENQ to UE with required IRATs type */
                  if( PRSNT_NODEF == 
                        ueCap->eUTRA_Cap.interRAT_Params.pres.pres )
                  {
                     wrFillRatLstRatType(ueCap,&ratLst,&transCb->u.s1TgtHoTrans.handInUeCdma);
                     if(transCb->u.s1TgtHoTrans.handInUeCdma== TRUE)
                     {
                        if(ROK != wrUmmUeCapSndUeCapEnq(transCb, &ratLst))
                        {
                           RLOG0(L_ERROR,"Failed to Send UECAP Enq.");
                           RETVALUE(RFAILED);
                        }
                     }
                  }
                  /* HO_FIX: close the transaction in else part */
                            if ( transCb->u.s1TgtHoTrans.handInUeCdma == FALSE )
                            {
                                    wrUmmTransComplete(transCb);
                                    transCb->ueCb->hoTransId = 0;
                            }

               }
            }
            break;
         }
      case WR_UMM_RRC_CFG:
         {
            if (ROK != wrUmmS1TgtHoPrcRrcCfgCfm(transCb))
            {
               RLOG0(L_ERROR, " RRC CFG CFM Is Failed ");
               cause->causeTyp = WR_CAUSE_RADIONW;
               cause->causeVal = SztCauseRadioNwno_radio_resources_available_in_target_cellEnum; 
               if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb))
               {
                  RLOG0(L_ERROR, " wrUmmS1TgtHoProcessor: Failed To Send Handover Failure");
               }
               transCb->state = WR_UMM_TRANS_RELEASE_CALL;
               RETVALUE(RFAILED);
            }
            if(transCb->u.s1TgtHoTrans.state == WR_UMM_TGT_HO_RRC_CFG_DONE)
            {
               wrUmmS1TgtHoCfgTnls(transCb);
            }

            break;
         }/* End of WR_UMM_RRC_CFG_CFM*/
      case WR_UMM_DAM_CFM:
         {
            wrUmmS1TgtHoPrcDamCfm(transCb);
            if (transCb->u.s1TgtHoTrans.state == WR_UMM_TGT_HO_TNL_CFG_DONE)
            {
               /* Update ueCb */
               wrUmmS1TgtHoCommitUeInfo(transCb);

               if (ROK != wrUmmS1TgtHoPrepRrcDlDcchMsg(transCb))
               {
                  RLOG0(L_ERROR,"Failed to Prepare HO Command");
                  if (ROK !=wrUmmS1TgtHoSendHoPrepFail(transCb))
                  {
                     RLOG0(L_ERROR, " wrUmmS1TgtHoProcessor:Failed to send handover failure");
                  }
                  transCb->state = WR_UMM_TRANS_RELEASE_CALL;
                  RETVALUE(RFAILED);
               }
            }
            break;
         }
      case WR_UMM_RRC_ENC_CFM:
         {
            if (ROK != wrUmmS1TgtHoPrcRrcEncCfm(transCb))
            {
               RLOG0(L_ERROR, " Rrc Enc Cfm is Failed ");
               if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb))
               {
                  RLOG0(L_ERROR, " Failed To Send Handover Failure");
               }
               wrUmmS1TgtHoTransRel(transCb);
               RETVALUE(RFAILED);
            }
            break;
         }
      case WR_UMM_S1AP_ENC_DEC_CFM:
         {
            if (ROK != wrUmmS1TgtHoSndHoReqAck(transCb))
            {
               if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb))
               {
                  RLOG0(L_ERROR, " wrUmmS1TgtHoProcessor: Failed To Send Handover Failure");
               }
               RLOG0(L_ERROR, " Failed to Prepare Handover Req Ack Message. ");
               transCb->state = WR_UMM_TRANS_RELEASE_CALL;
               RETVALUE(RFAILED);
            }
            break;
         }

      case WR_UMM_SRCTOTGT_CONT_DEC_CFM:
         {
            if (ROK != wrUmmS1TgtHoPrcSrcToTgtDecCfm(transCb))
            {
               if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb))
               {
                  transCb->state  = WR_UMM_TRANS_DELETE_UE;
                  RLOG0(L_ERROR, " Failed To Handle Tgt To Src Cont DEC Cfm. ");
                  RETVALUE(RFAILED);
               }
            }

            break;
         }

      case WR_UMM_UU_IND_PDU:
         {
            /* ccpu00128988 */
            /* The Msg received can be either RRC Connection ReConfig Complete
             * or UL Trfr Nas Msg
             * So Process the Uu PDU indication*/

            wrUmmS1TgtHoPrcUuPdu(transCb);

            /* ccpu00128988 */
            break;
         }
      case WR_UMM_PHY_CFG: 
         {
            if(ROK!= wrUmmS1TgtHoPrcPhyCfgCfm(transCb))
            {
               cause->causeTyp = WR_CAUSE_RADIONW;
               cause->causeVal = 
                  SztCauseRadioNwno_radio_resources_available_in_target_cellEnum;
               if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb))
               {
                  RLOG0(L_ERROR, " wrUmmS1TgtHoProcessor: Failed To Send Handover Failure");
               }
               transCb->state  = WR_UMM_TRANS_RELEASE_CALL;
               RETVALUE(RFAILED);
            }
            break;
         }
      case WR_UMM_RRC_DEC_CFM:
         {
            /**
             * Process RRC Decode Confirm.
             */
            if(FALSE == transCb->u.s1TgtHoTrans.handInUeCdma)
            {
               ret = wrUmmHoTgtPrcRrcDecCfm(transCb, &transCb->u.s1TgtHoTrans.ueCap);
               /* SPS changes starts */
               if (( ret != ROK) && (ret != WR_UMM_NHU_MSG_UECAPINFO_V9A0_DEC_SENT))
                  /* SPS changes ends */
               {
                  if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb))
                  {
                     RLOG0(L_ERROR, " wrUmmS1TgtHoProcessor: Failed To Send Handover Failure");
                  }
                  transCb->state = WR_UMM_TRANS_DELETE_UE;
                  RETVALUE(RFAILED);
               }

               /* SPS changes starts */
               if ((NHU_DEC_SDUTYPE_EUTRA_UECAP_IE == transCb->msg->u.decCfm->sduType) && \
                     (ret != WR_UMM_NHU_MSG_UECAPINFO_V9A0_DEC_SENT))
                  /* SPS changes ends */
               {
                  /*ccpu00129650*/
                  transCb->ueCb->ueCat =
                     transCb->u.s1TgtHoTrans.ueCap->eUTRA_Cap.ue_Category.val;
                  /* RRC-REL10-Upgrade */
                  wrUtlUpdateUeCatV1020(transCb->ueCb, (transCb->u.s1TgtHoTrans.ueCap));
                  wrUmmS1TgtHoProcHoAdmitReq(transCb);
               }
               else if (NHU_MSG_HOPREPINFO == transCb->msg->u.decCfm->sduType)
               {
                  if((transCb->u.s1TgtHoTrans.sztHovrTyp == SztHovrTyputrantolteEnum) ||
                     (transCb->u.s1TgtHoTrans.sztHovrTyp == SztHovrTypgerantolteEnum))
                  {
                     U8                        bearerIdx = 0;
                     U8                        numOfRbCfgd = 0;
                     WrUmmS1TgtHoRabInfo       *rabInfo;

                     numOfRbCfgd = transCb->u.s1TgtHoTrans.numOfRbCfgd;
                     /* Need To allocate  LCH ID and RB ID for every bearer.*/
                     for(bearerIdx = 0; bearerIdx < numOfRbCfgd; bearerIdx++)
                     {
                        rabInfo = &transCb->u.s1TgtHoTrans.rbInfo[bearerIdx];

                        if (wrUmmGetLch(transCb->ueCb, &(rabInfo->drbInfo.lchId)) != ROK)
                        {
                           RLOG0(L_ERROR,"Failed to allocate LCH for a RAB");
                           rabInfo->rabAllowed = FALSE;
                           continue;
                        }
                        if (wrUmmGetDrb(transCb->ueCb, &(rabInfo->drbInfo.rbId)) != ROK)
                        {
                           RLOG0(L_ERROR,"Failed to allocate DRB for a RAB");
                           wrUmmRelLch(transCb->ueCb, rabInfo->lchId);
                           rabInfo->rabAllowed = FALSE;
                           continue;
                        }
                        rabInfo->rabAllowed = TRUE;
                     }
                  }
                  else
                  {
                     /* SPS changes starts */
                     nhuHovrPrepInfo = &transCb->msg->u.decCfm->ie.nhuHovrPrepInfo;
                     wrUmmHoFillRadioResCfg(&transCb->u.s1TgtHoTrans.ueRadioResCfg, nhuHovrPrepInfo);
                     /* SPS changes ends */

                     /* Get Lch Id and Drb Id */
                     if (ROK != wrUmmS1TgtHoGetLchDrbIds(transCb))
                     {
                        cause->causeTyp = WR_CAUSE_RADIONW;
                        cause->causeVal = SztCauseProtsemantic_errorEnum;
                        if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb))
                        {
                           RLOG0(L_ERROR, " wrUmmS1TgtHoProcessor: Failed To Send Handover Failure");
                        }
                        transCb->state  = WR_UMM_TRANS_DELETE_UE;
                        RETVALUE(RFAILED);
                     }
                     /* SPS changes starts */
                     transCb->u.s1TgtHoTrans.srcToTgtSpsInfo.isDlSpsPresent = FALSE;
                     transCb->u.s1TgtHoTrans.srcToTgtSpsInfo.isUlSpsPresent = FALSE;
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
                           transCb->u.s1TgtHoTrans.srcToTgtSpsInfo.isDlSpsPresent = TRUE;
                           RLOG0(L_INFO, "wrUmmS1TgtHoProcessor: DL SPS present in SRC to TGT Container\n");
                        }
                        if (nhuHovrPrepInfo->criticalExtns.val.c1.val.handoverPrepInfo_r8.as_Config.\
                              sourceRadioResourceConfig.sps_Config.sps_ConfigUL.choice.val == 1)
                        {
                           transCb->u.s1TgtHoTrans.srcToTgtSpsInfo.isUlSpsPresent = TRUE;
                           RLOG0(L_INFO, "wrUmmS1TgtHoProcessor: UL SPS present in SRC to TGT Container\n");
                        }
                     }
                  }
                  /* SPS changes ends */
               }
               else if (NHU_MSG_UECAPINFO_V9A0 == transCb->msg->u.decCfm->sduType)
               {
                  transCb->ueCb->ueCat = transCb->u.s1TgtHoTrans.ueCap->eUTRA_Cap.ue_Category.val;
                  /* RRC-REL10-Upgrade */
                  wrUtlUpdateUeCatV1020(transCb->ueCb, (transCb->u.s1TgtHoTrans.ueCap));
                  wrUmmS1TgtHoProcHoAdmitReq(transCb);
               }

            }
            else
            {
               ret = wrUmmHoTgtPrcRrcDecCfm(transCb, &transCb->ueCb->ueCap);
               /* clear the transaction only when DRX scheduler confirm is received,
                * else after getting the scheduler confirmation for DRX only 
                * transaction will be closed*/
               if (ret != WR_UMM_NHU_MSG_UECAPINFO_V9A0_DEC_SENT)
               {
                  transCb->u.s1TgtHoTrans.handInUeCdma = FALSE;
                  wrUmmTransComplete(transCb);
                  /* resetting hoTransId */
                  transCb->ueCb->hoTransId = 0;
               }
            }
            break;
         }
      case WR_UMM_TMR_EXPIRY:
         {
            wrUmmS1TgtHoPrcTmrExpiry(transCb);
            break;
         }
         /* ccpu00126532 */
#ifdef TENB_AS_SECURITY
      case WR_UMM_PHY_KDF:
         {
            wrUmmS1TgtHoHdlKdfResp(transCb);
            break;
         }
#endif
      default:
         RETVALUE(RFAILED);

   } /* end of switch statement */
   RETVALUE(ROK);
}
/* HO_REEST */

/** @brief This function is called when a reestablishment message is received during
 *         S1 Target Handover transaction .
 *
 * @details
 *
 *     Function: wrUmmS1TgtHoNewTransProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  inctransCb : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmS1TgtHoNewTransProcessor
(
 WrUmmTransCb                 *transCb,
 WrUmmTransCb                 *incTrans
 )
{
   WrUeCb                    *ueCb = transCb->ueCb;
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
      if((transCb->ueCb->mobCtrlState == WR_HO_NORMAL) &&
         (transCb->u.s1TgtHoTrans.isUeAttached == FALSE))
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
         /* Send HO failure and release the UE */
               transCb->u.s1TgtHoTrans.hoPrepFailCause.causeTyp = 
                  WR_CAUSE_RADIONW;
         transCb->u.s1TgtHoTrans.hoPrepFailCause.causeVal = 
            SztCauseRadioNwfailure_in_radio_interface_procedureEnum;
         if (ROK != wrUmmS1TgtHoSendHoPrepFail(transCb))
         {
                  RLOG0(L_ERROR, " wrUmmS1TgtHoPrcHoReqMsg: Failed To Send"
                        "Handover Failure");
         }
               /* Clearing s1ConCb, as HO Failure will clear the UE context  
                  in S1AP and to avoid sending UE context release request 
                  during WR_UMM_TRANS_RELEASE_CALL */ 
         if(ueCb->s1ConCb != NULLP)
         {
            WR_FREE(ueCb->s1ConCb,sizeof(WrS1ConCb));
         }
         transCb->state = WR_UMM_TRANS_DONE;
      }
#ifdef WR_RSYS_KPI
      /* Update the HO failure counter only when RRC reconfiguration 
       * complete is not received which means isUeAttached state is FALSE */
      if ( FALSE == transCb->u.s1TgtHoTrans.isUeAttached ) 
      {
         /* ccpu00141451: Update the isRrcReEstRcvd flag as TRUE
          * to increment the HO success counter via RRC Re-Reestablishment 
          * to target eNB */
         transCb->u.s1TgtHoTrans.isRrcReEstRcvd = TRUE;

         /* Increment the INTRA LTE HO Fail Counter */
         WR_INC_KPI_FUNC4(wrIncKpiFrHovrFaild, 
               WR_HO_TYPE_INTRA_LTE_S1,
               transCb->u.s1TgtHoTrans.sztHovrTyp, INC_HOV,
               transCb->u.s1TgtHoTrans.sztCause );
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
            RLOG1(L_ERROR, "Unhandled Incoming Transtype [%d]",
                  incTrans->transTyp);
         }
   } 

   RETVALUE(WR_UMM_TRANS_OK);
}

/* ********************************************************************** */
/*                                                                        */
/* Name: wrUmmS1TgtHoTransRel                                             */
/*                                                                        */
/* This function is S1 Tgt HO transaction specific release function.      */
/*                                                                        */
/* ********************************************************************** */
PUBLIC S16 wrUmmS1TgtHoTransRel
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   TknStrOSXL                *nasSecToEutran;
   WrUmmS1TgtHoTrans         *s1TgtHoTrans = &transCb->u.s1TgtHoTrans;

   /* set the hoTransId to default */
   ueCb->hoTransId = 0;
   ueCb->hoType    = WR_HO_TYPE_NONE;
   ueCb->mobCtrlState = WR_HO_NORMAL;
   if(0 != ueCb->dedicatedPrmbl)
   {
         wrUtlFreeDedPreamble(ueCb->dedicatedPrmbl, ueCb->cellId);
   }
   if(s1TgtHoTrans->rbInfo)
   {
      WR_FREE(s1TgtHoTrans->rbInfo,
            s1TgtHoTrans->numOfRbCfgd * sizeof(WrUmmS1TgtHoRabInfo));
   }
   if(s1TgtHoTrans->sztCause)
   {
      WR_FREE(s1TgtHoTrans->sztCause, sizeof(SztCause));
   }
   if(s1TgtHoTrans->nextHop_NH.val)
   {
      WR_FREE(s1TgtHoTrans->nextHop_NH.val, s1TgtHoTrans->nextHop_NH.len);
   }
   /* Fix for ccpu00125004 */
   if(s1TgtHoTrans->snStsInfo)
   {
      WR_FREE(s1TgtHoTrans->snStsInfo, sizeof(WrUmmSnStsInfo));
   }
   if(s1TgtHoTrans->hovrRestrnLst)
   {
      WR_FREE(s1TgtHoTrans->hovrRestrnLst,sizeof(SztHovrRestrnLst));
   }
   if ((SztHovrTyputrantolteEnum == s1TgtHoTrans->sztHovrTyp) ||
       (SztHovrTypgerantolteEnum == s1TgtHoTrans->sztHovrTyp))
   {
      nasSecToEutran = &s1TgtHoTrans->u.iRatHoInfo.nasSecParamToEutran;
      if(NULLP != nasSecToEutran->val)
      {
         WR_FREE(nasSecToEutran->val, nasSecToEutran->len);
      }
   }
   /* ccpu00131426 - Fix */
   if(s1TgtHoTrans->rrmUeRecfgRspMsg)
   {
      wrUmmRlsIncMsg(&s1TgtHoTrans->rrmUeRecfgRspMsg);
   }
   if(s1TgtHoTrans->u.eutraTgtInfo.sourceSCellRelLst_r10.noComp.val) 
   {
      WR_FREE((s1TgtHoTrans->u.eutraTgtInfo.sourceSCellRelLst_r10.member),
            (sizeof(NhuSCellIdx_r10) * 
             s1TgtHoTrans->u.eutraTgtInfo.sourceSCellRelLst_r10.noComp.val));
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
