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
               

     File:     wr_umm_uecap.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=75;


static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_emm.h"
#include "wr_umm.h"
#include "nhu.x"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_s1ap_msg.h"
#include "wr_umm_trans.h"
#include "wr_ifm_rrc.h"
#include "wr_ifm_s1ap.h"

/* Definitions for PUBLIC functions implemented in UE CAP module used     */
/* by INIT CONTEXT module                                                 */
EXTERN S16 wrUmmUeCapSndUeCapEnq(WrUmmTransCb *transCb);
EXTERN S16 wrUmmUeCapHdlUeCapEncCfm(WrUmmTransCb *transCb, NhuEncCfmSdus *cfm);
EXTERN S16 wrUmmRrcDecReqHdl(WrUmmTransCb *transCb, TknStrOSXL *str, 
                                                                  U16 sduType);
EXTERN S16 wrUmmUeCapUeCapDecCfmHdl(WrUmmTransCb *transCb, 
                                                      NhuDecodeSduCfm *cfmSdu);
EXTERN S16 wrUmmSndUeCapEncReq(WrUeCb *ueCb, U32 transId, NhuUECapInfo *ueCap);
EXTERN S16 wrUmmUeCapRaCapDecCfmHdl(WrUmmTransCb *transCb, 
                                                      NhuDecodeSduCfm *cfmSdu);
EXTERN S16 wrUmmUecapEutraIeDecCfmHdl(WrUeCapInfo *ueCap, 
                                                      NhuDecodeSduCfm *cfmSdu);
EXTERN S16 wrUmmUeCapHdlUeRaCapEncCfm(WrUmmTransCb *transCb, 
                                                        NhuEncCfmSdus *cfmSdu);

/*
 *      Fun:   wrUmmUeCapSndUeCapEnq
 *
 *      Desc:  This function fills the RRC UE Capability Enquiry message and
 *             send the same to lower layer.
 *
 *      Ret:   ROK
 *
 *      Notes: None
 */
PUBLIC S16 wrUmmUeCapSndUeCapEnq
(
WrUmmTransCb                 *transCb
)
{
   U8                        transId;
   NhuDatReqSdus             *capEnqSdu  = NULLP;
   WrUeCb                    *ueCb = transCb->ueCb;

   /* Allocate memory for event structure */
   WR_ALLOCEVNT(&capEnqSdu, sizeof(NhuDatReqSdus));
   if (capEnqSdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /*ccpu00127802*/
   
   /* Fill HDR Info */
   wrUmmFillRrcEvntHdr(&(capEnqSdu->hdr), ueCb->cellId, ueCb->crnti,
                                                           transCb->transId);

   /* Fetch an RRC transaction id for this procedure.                     */
   if (ROK != wrUmmGetRrcTransId(ueCb, C4_UECAPENQUIRY, 
                      WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
                      &transId))
   {
      RLOG0(L_ERROR, "RRC Transaction Not avialble");
      WR_FREEEVNT(capEnqSdu);
      RETVALUE(RFAILED);
   }
   /* Build RRC Con Setup Message */
   if (wrUmmRrcFillUeCapEnq(capEnqSdu, transId) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmRrcFillUeCapEnq Failed");
      WR_FREEEVNT(capEnqSdu);
      RETVALUE(RFAILED);
   }

   /* Set Config Present field */
   capEnqSdu->sdu.isUeCfgPres = FALSE;

   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending UECapabilityEnquiry");
   /* Send UE Capability Enq through NhuDatRsp Primitive*/
   if (wrIfmRrcSndReqToUe(capEnqSdu) != ROK)
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe failed");
      WR_FREEEVNT(capEnqSdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/*
 *      Fun:   wrUmmUeCapHdlUeCapEncCfm
 *
 *      Desc:  This function handles the UE Capability Encode confirmation.
 *
 *      Ret:   ROK
 *
 *      Notes: None
 */
PUBLIC S16 wrUmmUeCapHdlUeCapEncCfm
(
WrUmmTransCb                 *transCb,
NhuEncCfmSdus                *cfmSdu
)
{
   S16                       ret            = RFAILED;
   MsgLen                    len            = 0;
   MsgLen                    cnt            = 0;
   TknStrOSXL                encodedInfo;
   Data                      *flatBuf       = NULLP;
   NhuEncReqSdus             *encSdu        = NULLP;
   WrUeCb                    *ueCb          = transCb->ueCb;
   U32                       transId        = transCb->transId;

   if (cfmSdu->sdu.status != NHU_ENCODE_SUCCESS)
   {
      RLOG0(L_ERROR, "Encode of UE Capability failed");
      RETVALUE(RFAILED);
   }

   /* Find the length of the received IP packet */
   ret = SFndLenMsg(cfmSdu->sdu.mBuf, &len);
   if((len == 0) || (ret != ROK))
   {
      RLOG1(L_ERROR, "Incorrect buffer len[%d]", len);
      RETVALUE(RFAILED);
   }   
       
   WR_ALLOC(&flatBuf, len);
   if (flatBuf == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Convert the mBuf to U8 array */
   SCpyMsgFix(cfmSdu->sdu.mBuf, 0, len, flatBuf, &cnt);
   WR_FILL_TKN_STR_UINT((&encodedInfo),flatBuf,len)
   
   /* Allocate memory for event structure */
   WR_ALLOCEVNT(&encSdu, sizeof(NhuEncReqSdus));
   if (encSdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Fill HDR Info */
   wrUmmFillRrcEvntHdr(&(encSdu->hdr), ueCb->cellId, ueCb->crnti, transId);
   encSdu->sdu.msgCategory = NHU_MSG_UERACAPINFO; 
   wrUmmRrcFillUeRadAccCapInfo(&encodedInfo, encSdu, ueCb);

   /* Send Encode UE RA Capability Info Req Primitive*/
   if (wrIfmRrcSndEncodeReq(encSdu) != ROK)
   {
      RLOG0(L_ERROR, "wrIfmRrcSndEncodeReq failed");
      WR_FREEEVNT(encSdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/*
 *      Fun:   wrUmmUeCapHdlUeRaCapEncCfm
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 */
PUBLIC S16 wrUmmUeCapHdlUeRaCapEncCfm
(
WrUmmTransCb                 *transCb,
NhuEncCfmSdus                *cfmSdu
)
{
   S16                       ret       = RFAILED;
   MsgLen                    len       = 0;
   MsgLen                    cnt       = 0;
   TknStrOSXL                encodedData;
   WrS1ConCb                 *s1apCon  = NULLP;
   S1apPdu                   *s1Pdu    = NULLP;
   Data                      *flatBuf  = NULLP;
   WrUeCb                    *ueCb     = transCb->ueCb;
   WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);

   if (cfmSdu->sdu.status != NHU_ENCODE_SUCCESS)
   {
      RLOG0(L_ERROR, "NHU Encode failed");
      RETVALUE(RFAILED);
   }

   /* Find the length of the received IP packet */
   ret = SFndLenMsg(cfmSdu->sdu.mBuf, &len);
   if((len == 0) || (ret != ROK))
   {
      RLOG1(L_ERROR, "In correct buffer len [%d]", len);
      RETVALUE(RFAILED);
   }   
       
   WR_ALLOC(&flatBuf, len);
   if (flatBuf == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Convert the mBuf to U8 array */
   SCpyMsgFix(cfmSdu->sdu.mBuf, 0, len, flatBuf, &cnt);
   WR_FILL_TKN_STR_UINT((&encodedData),flatBuf,len)
   s1apCon = ueCb->s1ConCb;
   wrUmmS1apFillUeCapInfo(s1apCon, &encodedData, &s1Pdu); 
   
   RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Sending UE CAPABILITY INFO"
             "INDICATION [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu] ",
             s1apCon->mme_ue_s1ap_id, s1apCon->enb_ue_s1ap_id);

   wrIfmS1apSndUeMsg(s1apCon->spConnId, s1Pdu);
   if (initCtxtCb->ueCap->ueEutraOctStr.pres)
   {
      wrUmmRrcDecReqHdl(transCb, &initCtxtCb->ueCap->ueEutraOctStr,
                                            NHU_DEC_SDUTYPE_EUTRA_UECAP_IE);
   }

   RETVALUE(ROK);
}

/*
 *      Fun:   wrUmmRrcDecReqHdl
 *
 *      Desc:  This function handles the decode requests towards RRC.
 *
 *      Ret:   ROK
 *
 *      Notes: None
 */
PUBLIC S16 wrUmmRrcDecReqHdl
(
WrUmmTransCb                 *transCb,
TknStrOSXL                   *str,
U16                          sduType
)
{
   S16                       ret            = RFAILED;
   WrUeCb                    *ueCb          = transCb->ueCb;
   NhuDecodeSduReq           *nhuDecSduReq  = NULLP;

   if (!(str->pres) && (str->len))
   {
      RLOG0(L_ERROR, "Invalid Input");
      RETVALUE(RFAILED);
   }
   /* Allocate memory for event structure */
   /* Fix for ccpu00126491, used wrong API for memory allocation */
   WR_ALLOC(&nhuDecSduReq, sizeof(NhuDecodeSduReq));
   if (nhuDecSduReq == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Fill HDR Info */
   wrUmmFillRrcEvntHdr(&(nhuDecSduReq->hdr), ueCb->cellId, ueCb->crnti,
                                                          transCb->transId);
   nhuDecSduReq->sduType = sduType;

   /* Convert the U8 array  to Msg Buffer */
   /* TODO: Get the region and pool from wrCb */
   ret = SGetMsg(0, 0, &nhuDecSduReq->mbuf);
   if (nhuDecSduReq->mbuf == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Add the received flat buffer to to mbuf */
   ret = SAddPstMsgMult((Data *)str->val, (MsgLen)str->len, nhuDecSduReq->mbuf);
   if (ret != ROK)
   {
      RLOG0(L_FATAL, "SAddPstMsgMult Failed.");
      RETVALUE(RFAILED);
   }

   /* Send Decode UE Capability Req Primitive*/
   if (wrIfmRrcSndDecodeReq(nhuDecSduReq) != ROK)
   {
      RLOG0(L_ERROR, "wrIfmRrcSndDecodeReq failed");
      WR_FREEEVNT(nhuDecSduReq);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/*
 *      Fun:   wrUmmUecapEutraIeDecCfmHdl
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 */
PUBLIC S16 wrUmmUecapEutraIeDecCfmHdl
(
WrUeCapInfo                  *ueCap,
NhuDecodeSduCfm              *nhuDecSduCfm
)
{
   if ((nhuDecSduCfm->status != NHU_DECODE_SUCCESS))
   {
      RLOG0(L_ERROR, "Decode of EUTRA CAP IE Failed");
      RETVALUE(RFAILED);
   }

   /* Fill the Ue Cap Str and keep in DB */
   cmMemcpy((U8*)&ueCap->eUTRA_Cap, (U8*)&nhuDecSduCfm->ie.nhuUE_EUTRA_Cap,
                                                    sizeof(NhuUE_EUTRA_Cap));

   RETVALUE(ROK);
}

/*
 *      Fun:   wrUmmUeCapRaCapDecCfmHdl
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 */
PUBLIC S16 wrUmmUeCapRaCapDecCfmHdl
(
WrUmmTransCb                 *transCb,
NhuDecodeSduCfm              *nhuDecSduCfm
)
{
   S16                       ret      = RFAILED;
   NhuUERadioAccessCapInfo_r8_IEsue_RadioAccessCapInfo *ueRaCap = NULLP;

   if ((nhuDecSduCfm->status != NHU_DECODE_SUCCESS))
   {
      RLOG0(L_ERROR, "Decoding of Radio CAP failed");
      RETVALUE(RFAILED);
   }
   ueRaCap = &nhuDecSduCfm->ie.ueRadioCapInfo.criticalExtns.val.c1.val.
              ueRadioAccessCapInfo_r8.ue_RadioAccessCapInfo;
 
   ret = wrUmmRrcDecReqHdl(transCb, ueRaCap, NHU_MSG_UECAPINFO);
   /* SRIKY: Could be causing the crash */
   /*WR_FREE(ueRaCap->val, ueRaCap->len);*/

   RETVALUE(ret);
}

/*
 *      Fun:   wrUmmUeCapUeCapDecCfmHdl
 *
 *      Desc:  RRC UE Capability Info Dec msg
 *
 *      Ret:   ROK
 *
 *      Notes: None
 */
PUBLIC S16 wrUmmUeCapUeCapDecCfmHdl
(
WrUmmTransCb                 *transCb,
NhuDecodeSduCfm              *nhuDecSduCfm
)
{
   S16                       cNt           = 0;
   S16                       ret           = RFAILED;
   NhuUE_CapRAT_ContLst      *ueContLst    = NULLP;
   NhuUECapInfo              *ueCapInfo    = NULLP;
   NhuUECapInfo_r8_IEs       *ueCapInfo_r8 = NULLP;

   if((nhuDecSduCfm->status != NHU_DECODE_SUCCESS))
   {
      RLOG0(L_ERROR, "Decoding of UE CAP failed");
      RETVALUE(RFAILED);
   }
   ueCapInfo = &(nhuDecSduCfm->ie.ueCapInfo);
   ueCapInfo_r8 = &(ueCapInfo->criticalExtns.val.c1.val.ueCapInfo_r8);
   if (ueCapInfo_r8->pres.pres == PRSNT_NODEF)
   {
      ueContLst = &(ueCapInfo_r8->ue_CapRAT_ContLst);
      for (cNt=0; cNt < ueContLst->noComp.val; cNt++)
      { 
         if((ueContLst->member[cNt].pres.pres == TRUE) &&
             (ueContLst->member[cNt].rat_Typ.val == NhuRAT_TypeutraEnum))
         {  
            /* Decode EUTRA UeCap Ie */
            ret = wrUmmRrcDecReqHdl(transCb,
                                    &ueContLst->member[cNt].ueCapRAT_Cont,
                                    NHU_DEC_SDUTYPE_EUTRA_UECAP_IE);
         }
      }
      WR_FREE(ueContLst->member, 
         (sizeof(NhuUE_CapRAT_Cont))*(ueContLst->noComp.val));
   }

   RETVALUE(ret);
}

/*
 *      Fun:   wrUmmSndUeCapEncReq
 *
 *      Desc:  Ue Capability Info
 *
 *      Ret:   ROK
 *
 *      Notes: None
 */
PUBLIC S16 wrUmmSndUeCapEncReq
(
WrUeCb                       *ueCb,
U32                          transId,
NhuUECapInfo                 *ueCapInfo
)
{
   NhuEncReqSdus             *encSdu;

   /* Allocate memory for event structure */
   WR_ALLOCEVNT(&encSdu, sizeof(NhuEncReqSdus));
   if (encSdu == NULL)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Fill HDR Info */
   wrUmmFillRrcEvntHdr(&(encSdu->hdr), ueCb->cellId, ueCb->crnti, transId);
   encSdu->sdu.msgCategory = NHU_MSG_UECAPINFO; 

   cmMemcpy((U8*)&encSdu->sdu.u.ueCapInfo, (U8*)ueCapInfo, 
                                                     sizeof(NhuUECapInfo));

    /* Send Encode UE RA Capability Info Req Primitive*/
   if (wrIfmRrcSndEncodeReq(encSdu) != ROK)
   {
      RLOG0(L_ERROR, "wrIfmRrcSndEncodeReq failed");
      WR_FREEEVNT(encSdu);
      RETVALUE(RFAILED);
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
