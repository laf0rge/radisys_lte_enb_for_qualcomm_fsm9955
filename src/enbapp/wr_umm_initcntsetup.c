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
               

     File:     wr_umm_initcntsetup.c

     Sid:      $SID$ 

     Prg:      Sriky 

*********************************************************************21*/
/* header include files (.h) */
static const char* RLOG_MODULE_NAME="UMM";
static int RLOG_FILE_ID=105;
static int RLOG_MODULE_ID=32;

#include "wr.h"
#include "wr_utils.h"
#include "wr_umm.h"
#include "wr_emm.h"
#include "wr_smm_smallcell.h"
#include "wr_umm_trans.h"
#include "wr_umm_rrc_msg.h"
#include "wr_umm_s1ap_msg.h"
#include "wr_ifm_rrc.h"
#include "wr_ifm_dam.h"
#include "wr_ifm_schd.h"
#include "wr_ifm_s1ap.h"
#include "wr_ifm_rrm.h"
#include "wr_kpi.h"
#include "wr_kpiIds.h" 
#include "wr_umm_sch_msg.h"
#include "wr_umm_meas_fun.h"
#include "wr_ifm_l1.h"
#include "wr_umm_drx.h"
#include "wr_ifm_dam.h"

#ifdef WR_TEST_CODE
EXTERN U8 wrTrgUeCapTO;
EXTERN U8 wrTrgSecTO;
EXTERN U8 wrTrgRrcReConTO;
#endif

EXTERN U32 numTtiTicks;

/* ccpu00129384: Re-Configure CTF with UE Category */
#define WR_INIT_CTXT_UE_CAP_TRANS_ID        0x00000010
#define WR_INIT_CTXT_UE_TRANS_ID            0x0000000f
#define WR_INIT_CTXT_SRB2_TRANS_ID          0x0000000a
#define WR_RAD_RES_MOD_SCHD_TRANS_ID        0x0000000c

#define WR_UMM_TX_MODE_CHANGE_START         0x01 
#define WR_UMM_TX_MODE_CHANGE_COMPL         0x02

#define WR_UMM_DFLT_SIGNAL_UE_AMBR 1000

U8 wrUmmPrioIdforQci[WR_MAX_QCI_SUPPORTED]={1,1,1,2,1,2,1,2,2};



PUBLIC S16 wrUmmSecExtractSecKeys3
(
WrUeSecCfg                   *secCfg,
TknStrBSXL                   *secKey,
Bool                         *asKeyGenerated,
WrUmmTransCb                 *transCb
);

PUBLIC S16 wrUmmSecExtractHzSecKeys3
(
WrInitCntxtSetupTransCb *initCtxtCb,
WrCellId                  cellId,
U8                      kdfType
);

PUBLIC S16 wrUmmSecExtractSecKeys3
(
WrUeSecCfg                   *secCfg,
TknStrBSXL                   *secKey,
Bool                         *asKeyGenerated,
WrUmmTransCb                 *transCb
);

PUBLIC S16 wrUmmSecExtractHzSecKeys3
(
WrInitCntxtSetupTransCb *initCtxtCb,
WrCellId                  cellId,
U8                      kdfType
);

PRIVATE S16 wrUmmCqiSrRadResModPrepRrcMsgAndCfg 
(
WrUmmTransCb                 *transCb
);
PUBLIC S16 wrUmmIncTxModeChngNewTransProcessor(WrUmmTransCb *transCb,
                                            WrUmmTransCb *incTransCb);

/* deleted as part of warning fix and moved to wr_umm_trans.h file */
/* SPS changes starts */
EXTERN S16 wrUmmUecapEutraIeDecCfmHdl(WrUeCb *ueCb, WrUeCapInfo  *ueCap,NhuDecodeSduCfm  *nhuDecSduCfm);
/* SPS changes ends */
EXTERN S16 wrUmmUeCapRaCapDecCfmHdl(WrUmmTransCb   *transCb,NhuDecodeSduCfm    *nhuDecSduCfm);
EXTERN S16 wrUmmUeCapUeCapDecCfmHdl(WrUmmTransCb   *transCb,NhuDecodeSduCfm   *nhuDecSduCfm);
/* SPS changes starts */
EXTERN S16 wrUmmUecapEutraIe9a0DecCfmHdl(WrUeCapInfo  *ueCap,NhuDecodeSduCfm   *nhuDecSduCfm);
/* SPS changes ends */
EXTERN S16 wrUmmSecSndSecModeCmd(WrUmmTransCb   *transCb);
EXTERN S16 wrUmmSecKeyDervFunc(U8   *secKey,U8   *strS,U8  *derKey);
EXTERN S16 wrUmmSecGetSecCap(SztUESecurCapabilities   *ueSecCap,WrUeSecCfg   *secCfg);
EXTERN S16 wrS1apFillCause(SztCause  *ie,WrUmmMsgCause *cause);
EXTERN S16 wrUmmInitCtxtPrcRrcTmrExpiry(WrUmmTransCb   *transCb);
EXTERN S16 wrUmmCreatCsfbTrans(WrUmmTransCb     *transCb);
EXTERN S16 wrUmmGetOnGoingRrcTransId(WrUeCb   *ueCb,U8  dlDcchMsg,U8  transId,U32 *rrcTransId);
EXTERN S16 wrUmmFillAndSndRrmUeRecfgReq(WrUmmTransCb *transCb);
/* SPS changes starts */
PRIVATE S16 wrUmmUnSetSrMaskPrepRrcMsgAndCfg(WrUmmTransCb *transCb);
/* SPS changes ends */
#ifdef TENB_AS_SECURITY
EXTERN S16 wrPlatSecExtractSecKeys2
(
WrUmmTransCb *transCb,
U8 keyDerType
);
EXTERN S16 wrPlatSecExtractSecKeys
(
WrUeSecCfg                   *secCfg,
TknStrBSXL                   *secKey,
Bool                         *asKeyGenerated,
U32                          transId
);
#endif

PRIVATE Bool wrUmmSelectSecurityAlgo
(
U8 configSecAlgoVal,
U16 *secModAlgoVal,
U8 securAlgoMask,
U8 IntgOrCiphAlgo
);

#define WR_CIPHERING_ALGO 0
#define WR_INTEGRITY_ALGO 1

const char* securityAlgo[] = 
{
   "Ciphering",
   "Integrity"
};

PRIVATE S16 wrUmmSndReqToStartTATimer(WrUmmTransCb  *transCb);
/** @brief This function is responsible for finding out 
 *  if required IRAT Pdu is available in nhuDecCfm.
 *
 * @details
 *
 *     Function: wrUmmUeReqdIratInfoInd 
 *     Processing steps 
 *        -Check if MME has sent the reqd RAT's capability
 *           - if yes and if it is present in SDU dont add
 *             that RAT in ratLst
 *           - if no and if it is present in SDU add that
 *              RAT in ratLst
 * @return Void 
 *       */
PRIVATE Void wrUmmUeReqdIratInfoInd
(
NhuDecodeSduCfm              *nhuDecSduCfm,
WrUeCapInfo                  *ueCap,
WrUmmRatLst                  *ratLst
)
{
   U32                             idx = 0;
   NhuUE_EUTRA_CapinterRAT_Params  *iRATParams = NULLP;

   iRATParams = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.interRAT_Params;

   /*Fill EUTRAN RAT type*/
   ratLst->ratType[idx++] = NhuRAT_TypeutraEnum;

   if(PRSNT_NODEF == iRATParams->pres.pres)
   {
      /*Fill CDMA2000 RAT Type*/
      if((PRSNT_NODEF == iRATParams->cdma2000_1xRTT.pres.pres) &&
            (PRSNT_NODEF != ueCap->ueCdmaOctStr.pres))
      {
         ratLst->ratType[idx++] = NhuRAT_Typcdma2000_1XRTTEnum;
      }

      /*Fill GERAN RAT Type*/
      if((PRSNT_NODEF == iRATParams->geran.pres.pres) &&
            ((PRSNT_NODEF != ueCap->ueGeranPsOctStr.pres) && 
             (PRSNT_NODEF != ueCap->ueGeranCsOctStr.pres)))
      {
         ratLst->ratType[idx++] = NhuRAT_Typgeran_csEnum;
         ratLst->ratType[idx++] = NhuRAT_Typgeran_psEnum;
      }
   }
   ratLst->noComp = idx;
   RETVOID;
}


/* ====================================================================== */
/*                    UE CAPABILITY RELATED FUNCTIONS                     */
/* ====================================================================== */
/** @brief This function handles the decode requests that are sent towards 
 *         RRC.
 *
 * @details
 *
 *     Function: wrUmmRrcDecReqHdl
 *
 *         Processing steps:
 *         - allocate a NhuDecodeSduReq event
 *         - fill the header with the same transaction id as intial context 
 *           setup
 *         - convert the 'str' to a flat buffer
 *         - copy the buffer to the decode sdu
 *         - invoke the IFM primitive to send the decode request to RRC
 *
 * @param[in, out]  transCb : transaction control block
 * @param[in]      str     : string to be decoded
 * @param[in]      sduType : SDU that needs to be decoded
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
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
   ret = SGetMsg(wrCb.init.region, 0, &nhuDecSduReq->mbuf);
   if (nhuDecSduReq->mbuf == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Add the received flat buffer to to mbuf */
   ret = SAddPstMsgMult((Data *)str->val, (MsgLen)str->len, nhuDecSduReq->mbuf);
   if (ret != ROK)
   {
      RLOG0(L_FATAL, "SAddPstMsgMult Failed");
      RETVALUE(RFAILED);
   }

   /* Send Decode UE Capability Req Primitive*/
   if (wrIfmRrcSndDecodeReq(nhuDecSduReq) != ROK)
   {
      RLOG0(L_ERROR, "wrIfmRrcSndDecodeReq failed.");
      WR_FREEEVNT(nhuDecSduReq);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for filling the RRC UE Capability 
 *         Enquiry message and send the request to RRC.
 *
 * @details
 *
 *     Function: wrUmmUeCapSndUeCapEnq
 *
 *         Processing steps:
 *         - allocate a NhuDatReqSdus event
 *         - fill the header with the same transaction id as intial context 
 *           setup
 *         - get the RRC transaction id using wrUmmGetRrcTransId
 *         - fill the UE Capability enquiry message
 *         - invoke the IFM primitive to request RRC to send the message to UE
 *
 * @param[in, out]  transCb : transaction control block
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmUeCapSndUeCapEnq
(
WrUmmTransCb                 *transCb,
WrUmmRatLst                  *ratLst
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

   /* Fill HDR Info */
   wrUmmFillRrcEvntHdr(&(capEnqSdu->hdr), ueCb->cellId, ueCb->crnti,
                                                           transCb->transId);

   /* Fetch an RRC transaction id for this procedure.                     */
   if(wrUmmGetRrcTransId(ueCb, C1_UECAPENQUIRY, 
                      WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
                      &transId) != ROK)
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI,ueCb->crnti,"MAX RRC Transaction already "
         "Reached");
      WR_FREEEVNT(capEnqSdu);
      RETVALUE(RFAILED);
   }
   /* Build RRC Con Setup Message */
   if (wrUmmRrcFillUeCapEnq(capEnqSdu, ratLst, transId) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmRrcFillUeCapEnq Failed");
      WR_FREEEVNT(capEnqSdu);
      RETVALUE(RFAILED);
   }

   /* Set Config Present field */
   capEnqSdu->sdu.isUeCfgPres = FALSE;
   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending "
         "UECapabilityEnquiry");
   /* Send UE Capability Enq through NhuDatRsp Primitive*/
   if (wrIfmRrcSndReqToUe(capEnqSdu) != ROK)
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe failed");
      WR_FREEEVNT(capEnqSdu);
      RETVALUE(RFAILED);
   }
   /*Start UE Capability Enquiry Timer*/
#if 0
#else
   wrStartTransTmr(transCb,WR_TMR_RRC_CAPENQ, wrEmmGetRrcUeCapTmrVal(ueCb->cellId));
#endif
   RETVALUE(ROK);
}

/** @brief This function is responsible for handling the UE Capability Encode
 *         confirmation from RRC.
 *
 * @details
 *
 *     Function: wrUmmUeCapHdlUeCapEncCfm
 *
 *         Processing steps:
 *         - if the encode status is success
 *           - allocate a new NhuEncReqSdus event
 *           - fill the header with the same transaction id as intial context 
 *             setup
 *           - set the encode message category as NHU_MSG_UERACAPINFO
 *           - fill the RRC UE Radio Access Capability Information IE
 *           - invoke the IFM primitive to request RRC to encode the message
 *
 * @param[in, out]  transCb : transaction control block
 * @param[in]      cfmSdu  : RRC Encoded SDU 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmUeCapHdlUeCapEncCfm
(
WrUeCb                       *ueCb,
NhuEncCfmSdus                *cfmSdu,
U32                          transId
)
{
   S16                       ret            = RFAILED;
   MsgLen                    len            = 0;
   MsgLen                    cnt            = 0;
   TknStrOSXL                encodedInfo;
   Data                      *flatBuf       = NULLP;
   NhuEncReqSdus             *encSdu        = NULLP;

   if (cfmSdu->sdu.status != NHU_ENCODE_SUCCESS)
   {
      RLOG0(L_ERROR, "Encode of UE Capability failed ");
      RETVALUE(RFAILED);
   }

   /* Find the length of the received IP packet */
   ret = SFndLenMsg(cfmSdu->sdu.mBuf, &len);
   if((len == 0) || (ret != ROK))
   {
      RLOG1(L_ERROR, "Incorrect buffer len[%d]",len);
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

   WR_FREE(encodedInfo.val, encodedInfo.len);
   /* Send Encode UE RA Capability Info Req Primitive*/
   if (wrIfmRrcSndEncodeReq(encSdu) != ROK)
   {
      RLOG0(L_ERROR, "wrIfmRrcSndEncodeReq failed");
      WR_FREEEVNT(encSdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for handling the UE Radio Access 
 *         Capability Encode confirmation from RRC.
 *
 * @details
 *
 *     Function: wrUmmUeCapHdlUeRaCapEncCfm
 *
 *         Processing steps:
 *         - if the encode status is success
 *           - convert the mBuf (encoded RRC pdu) to flat buffer
 *           - fill the S1AP UE Capability message
 *           - invoke the IFM primitive to send the message to S1AP
 *           - invoke wrUmmRrcDecReqHdl to decode the EUTRA Capability
 *             IE that was recieved as part of UE Capability Information
 *             RRC message
 *
 * @param[in, out]  transCb : transaction control block
 * @param[in]      cfmSdu  : RRC Encoded SDU 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmUeCapHdlUeRaCapEncCfm
(
WrUeCb                       *ueCb,
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

   if (cfmSdu->sdu.status != NHU_ENCODE_SUCCESS)
   {
      RLOG0(L_ERROR, "NHU Encode failed ");
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
   WR_FILL_TKN_STR_UINT((&encodedData),flatBuf,len)
   s1apCon = ueCb->s1ConCb;
   wrUmmS1apFillUeCapInfo(s1apCon, &encodedData, &s1Pdu); 

   RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Sending UE CAPABILITY INFO"
             "INDICATION [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu] ",
             s1apCon->mme_ue_s1ap_id, s1apCon->enb_ue_s1ap_id);

   WR_FREE(encodedData.val, encodedData.len);
   RLOG_ARG2(L_DEBUG, DBG_CRNTI,ueCb->crnti,"Sending UE CAPABILITY INFO "
      "MME-UE-S1AP-ID[%lu] eNB-UE-S1AP-ID[%lu]",
      s1apCon->mme_ue_s1ap_id, s1apCon->enb_ue_s1ap_id);
   wrIfmS1apSndUeMsg(s1apCon->spConnId, s1Pdu);

   RETVALUE(ROK);
}

PRIVATE S16 wrUmmCopySupportedBandLstUtra
(
WrUeCapInfo                  *ueCap,
NhuDecodeSduCfm              *nhuDecSduCfm
)
{
   NhuIRAT_ParamsUTRA_FDD       *nhuDecUtraFDD = NULLP;
   NhuIRAT_ParamsUTRA_TDD128    *nhuDecUtraTDD128 = NULLP;
   NhuIRAT_ParamsUTRA_TDD384    *nhuDecUtraTDD384 = NULLP;
   NhuIRAT_ParamsUTRA_TDD768    *nhuDecUtraTDD768 = NULLP;
   NhuIRAT_ParamsUTRA_FDD       *utraFDD = NULLP;
   NhuIRAT_ParamsUTRA_TDD128    *utraTDD128 = NULLP;
   NhuIRAT_ParamsUTRA_TDD384    *utraTDD384 = NULLP;
   NhuIRAT_ParamsUTRA_TDD768    *utraTDD768 = NULLP;
   U32                       idx;

   nhuDecUtraFDD = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.interRAT_Params.utraFDD;
   nhuDecUtraTDD128 = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.interRAT_Params.utraTDD128;
   nhuDecUtraTDD384 = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.interRAT_Params.utraTDD384;
   nhuDecUtraTDD768 = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.interRAT_Params.utraTDD768;
   if(nhuDecUtraFDD->pres.pres == PRSNT_NODEF)
   {
      wrFillTknU8(&ueCap->eUTRA_Cap.interRAT_Params.pres, PRSNT_NODEF);
      utraFDD = &ueCap->eUTRA_Cap.interRAT_Params.utraFDD;
      wrFillTknU8(&(utraFDD->pres), PRSNT_NODEF);
      wrFillTknU16(&(utraFDD->supportedBandLstUTRA_FDD.noComp),
            nhuDecUtraFDD->supportedBandLstUTRA_FDD.noComp.val);
      utraFDD->supportedBandLstUTRA_FDD.member = NULLP;
         WR_GET_MEM(&ueCap->memCp, sizeof(NhuSuppBandUTRA_FDD) * utraFDD->supportedBandLstUTRA_FDD.noComp.val,
                &utraFDD->supportedBandLstUTRA_FDD.member);
      if(utraFDD->supportedBandLstUTRA_FDD.member == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         wrUmmDeAllocUeCap(ueCap);
         RETVALUE(RFAILED);
      }

      for(idx = 0; idx < utraFDD->supportedBandLstUTRA_FDD.noComp.val; idx++)
      {
         wrFillTknU32(&(utraFDD->supportedBandLstUTRA_FDD.member[idx]),
               nhuDecUtraFDD->supportedBandLstUTRA_FDD.member[idx].val);
      } 
   }
   if(nhuDecUtraTDD128->pres.pres == PRSNT_NODEF)
   {
      wrFillTknU8(&ueCap->eUTRA_Cap.interRAT_Params.pres, PRSNT_NODEF);
      utraTDD128 = &ueCap->eUTRA_Cap.interRAT_Params.utraTDD128;
      wrFillTknU8(&(utraTDD128->pres), PRSNT_NODEF);
      wrFillTknU16(&(utraTDD128->supportedBandLstUTRA_TDD128.noComp),
            nhuDecUtraTDD128->supportedBandLstUTRA_TDD128.noComp.val);
      utraTDD128->supportedBandLstUTRA_TDD128.member = NULLP;
         WR_GET_MEM(&ueCap->memCp, sizeof(NhuSuppBandUTRA_TDD128) * utraTDD128->supportedBandLstUTRA_TDD128.noComp.val,
                &utraTDD128->supportedBandLstUTRA_TDD128.member);

      if(utraTDD128->supportedBandLstUTRA_TDD128.member == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         wrUmmDeAllocUeCap(ueCap);
         RETVALUE(RFAILED);
      }

      for(idx = 0; idx < utraTDD128->supportedBandLstUTRA_TDD128.noComp.val; idx++)
      {
         wrFillTknU32(&(utraTDD128->supportedBandLstUTRA_TDD128.member[idx]),
               nhuDecUtraTDD128->supportedBandLstUTRA_TDD128.member[idx].val);
      } 
   }
   if(nhuDecUtraTDD384->pres.pres == PRSNT_NODEF)
   {
      wrFillTknU8(&ueCap->eUTRA_Cap.interRAT_Params.pres, PRSNT_NODEF);
      utraTDD384 = &ueCap->eUTRA_Cap.interRAT_Params.utraTDD384;
      wrFillTknU8(&(utraTDD384->pres), PRSNT_NODEF);
      wrFillTknU16(&(utraTDD384->supportedBandLstUTRA_TDD384.noComp),
            nhuDecUtraTDD384->supportedBandLstUTRA_TDD384.noComp.val);
      utraTDD384->supportedBandLstUTRA_TDD384.member = NULLP;
      WR_GET_MEM(&ueCap->memCp, sizeof(NhuSuppBandUTRA_TDD384) * utraTDD384->supportedBandLstUTRA_TDD384.noComp.val,
            &utraTDD384->supportedBandLstUTRA_TDD384.member);

      if(utraTDD384->supportedBandLstUTRA_TDD384.member == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         wrUmmDeAllocUeCap(ueCap);
         RETVALUE(RFAILED);
      }

      for(idx = 0; idx < utraTDD384->supportedBandLstUTRA_TDD384.noComp.val; idx++)
      {
         wrFillTknU32(&(utraTDD384->supportedBandLstUTRA_TDD384.member[idx]),
               nhuDecUtraTDD384->supportedBandLstUTRA_TDD384.member[idx].val);
      } 
   }
   if(nhuDecUtraTDD768->pres.pres == PRSNT_NODEF)
   {
      wrFillTknU8(&ueCap->eUTRA_Cap.interRAT_Params.pres, PRSNT_NODEF);
      utraTDD768 = &ueCap->eUTRA_Cap.interRAT_Params.utraTDD768;
      wrFillTknU8(&(utraTDD768->pres), PRSNT_NODEF);
      wrFillTknU16(&(utraTDD768->supportedBandLstUTRA_TDD768.noComp),
            nhuDecUtraTDD768->supportedBandLstUTRA_TDD768.noComp.val);
      utraTDD768->supportedBandLstUTRA_TDD768.member = NULLP;
      WR_GET_MEM(&ueCap->memCp, sizeof(NhuSuppBandUTRA_TDD768) * utraTDD768->supportedBandLstUTRA_TDD768.noComp.val,
            &utraTDD768->supportedBandLstUTRA_TDD768.member);

      if(utraTDD768->supportedBandLstUTRA_TDD768.member == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         wrUmmDeAllocUeCap(ueCap);
         RETVALUE(RFAILED);
      }

      for(idx = 0; idx < utraTDD768->supportedBandLstUTRA_TDD768.noComp.val; idx++)
      {
         wrFillTknU32(&(utraTDD768->supportedBandLstUTRA_TDD768.member[idx]),
               nhuDecUtraTDD768->supportedBandLstUTRA_TDD768.member[idx].val);
      } 
   }
   /* Check for Non - Critical Extensions */
   NhuUE_EUTRA_Cap_v920_IEs  *nonCritExtn = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.nonCriticalExtn;
   if(nonCritExtn->pres.pres == PRSNT_NODEF)
   {
      wrFillTknU8(&ueCap->eUTRA_Cap.nonCriticalExtn.pres, PRSNT_NODEF);
      if(nonCritExtn->interRAT_ParamsUTRA_v920.pres.pres == PRSNT_NODEF)
      {
         wrFillTknU8(&ueCap->eUTRA_Cap.nonCriticalExtn.interRAT_ParamsUTRA_v920.pres, PRSNT_NODEF);
         wrFillTknU32(&ueCap->eUTRA_Cap.nonCriticalExtn.interRAT_ParamsUTRA_v920.e_RedirectionUTRA_r9, 
               nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.nonCriticalExtn.interRAT_ParamsUTRA_v920.e_RedirectionUTRA_r9.val);
      }
      /* Check for Release 10 UTRA TDD Support */
      if(nonCritExtn->nonCriticalExtn.pres.pres == PRSNT_NODEF)
      {
         wrFillTknU8(&ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.pres, PRSNT_NODEF);
         if(nonCritExtn->nonCriticalExtn.nonCriticalExtn.pres.pres == PRSNT_NODEF)
         {
            NhuUE_EUTRA_Cap_v1020_IEs  *rel10UtraTddCap = 
               &ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn;
            NhuIRAT_ParamsUTRA_TDD_v1020    *utraTDDv1020 = 
               &nonCritExtn->nonCriticalExtn.nonCriticalExtn.interRAT_ParamsUTRA_TDD_v1020;

            wrFillTknU8(&ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.pres, 
                  PRSNT_NODEF);
            if(utraTDDv1020->pres.pres == PRSNT_NODEF)
            {
               wrFillTknU8(&rel10UtraTddCap->interRAT_ParamsUTRA_TDD_v1020.pres, PRSNT_NODEF);
               wrFillTknU32(&rel10UtraTddCap->interRAT_ParamsUTRA_TDD_v1020.e_RedirectionUTRA_TDD_r10, 
                     utraTDDv1020->e_RedirectionUTRA_TDD_r10.val);
            }
         }
      }
   }

   RETVALUE(ROK);
}
/* eCSFB:
 * @brief This function copies the CDMA Band information
 * supported by UE.
 *
 *      Function: wrUmmCopySupportedBandLstCdma2000
 *
 *          Processing steps:
 *          - Allocate memory for ue capability 
 *
 *
 *  @return S16
 *   -# Success : ROK
 *   -# Failure : RFAILED
*/
PRIVATE S16 wrUmmCopySupportedBandLstCdma2000
(
WrUeCapInfo                  *ueCap,
NhuDecodeSduCfm              *nhuDecSduCfm
)
{
	NhuIRAT_ParamsCDMA2000_1XRTT       *nhuDecCdma2000_1xRTT = NULLP;
	NhuIRAT_ParamsCDMA2000_1XRTT       *cdma20001xRTT = NULLP;
   NhuUE_EUTRA_Cap_v920_IEs           *nhuDecNonCriticalExtn = NULLP;
   NhuIRAT_ParamsCDMA2000_1XRTT_v920  *cdma2000_v920 = NULLP;
	U32                                idx = 0;
	/*RRC Rel10 Upgrade - start*/
	NhuUE_EUTRA_Cap_v940_IEs *nhuDecNonCriticalExtn_v940 = NULLP;
	NhuUE_EUTRA_Cap_v1020_IEs *nhuDecNonCriticalExtn_v1020 = NULLP;
	NhuIRAT_ParamsCDMA2000_1XRTT_v1020 *nhuDecInterRAT_ParamsCDMA2000_v1020 = NULLP;
	NhuIRAT_ParamsCDMA2000_1XRTT_v1020 *cdma2000_v1020 = NULLP;
	/*RRC Rel10 Upgrade - end*/

	nhuDecCdma2000_1xRTT = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.interRAT_Params.cdma2000_1xRTT;
	nhuDecNonCriticalExtn = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.nonCriticalExtn;

	if(nhuDecCdma2000_1xRTT->pres.pres == PRSNT_NODEF)
	{
		wrFillTknU8(&ueCap->eUTRA_Cap.interRAT_Params.pres, PRSNT_NODEF);
		cdma20001xRTT = &ueCap->eUTRA_Cap.interRAT_Params.cdma2000_1xRTT;
		wrFillTknU8(&(cdma20001xRTT->pres), PRSNT_NODEF);
		wrFillTknU16(&(cdma20001xRTT->supportedBandLst1XRTT.noComp),
				nhuDecCdma2000_1xRTT->supportedBandLst1XRTT.noComp.val);
		cdma20001xRTT->supportedBandLst1XRTT.member = NULLP;
      WR_GET_MEM(&ueCap->memCp, sizeof(NhuSuppBandLst1XRTT) * cdma20001xRTT->supportedBandLst1XRTT.noComp.val,
            &cdma20001xRTT->supportedBandLst1XRTT.member);

		if(cdma20001xRTT->supportedBandLst1XRTT.member == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         wrUmmDeAllocUeCap(ueCap);
			RETVALUE(RFAILED);
		}
		/* Copy supported band information */
		for(idx = 0; idx < cdma20001xRTT->supportedBandLst1XRTT.noComp.val; idx++)
		{
			wrFillTknU32(&(cdma20001xRTT->supportedBandLst1XRTT.member[idx]),
					nhuDecCdma2000_1xRTT->supportedBandLst1XRTT.member[idx].val);
		}
		/* Copy tx_Config1XRTT and rx_Config1XRTT information */
		wrFillTknU32(&(cdma20001xRTT->tx_Config1XRTT),
				nhuDecCdma2000_1xRTT->tx_Config1XRTT.val);
		wrFillTknU32(&(cdma20001xRTT->rx_Config1XRTT),
				nhuDecCdma2000_1xRTT->rx_Config1XRTT.val);

		/* check whether nonCriticalExtn value present or not */
		if(nhuDecNonCriticalExtn->pres.pres == PRSNT_NODEF)
		{
			wrFillTknU8(&ueCap->eUTRA_Cap.nonCriticalExtn.pres, PRSNT_NODEF);
			cdma2000_v920 = &ueCap->eUTRA_Cap.nonCriticalExtn.interRAT_ParamsCDMA2000_v920;
			/* check whether interRAT_ParamsCDMA2000_v920 present or not */
			if(nhuDecNonCriticalExtn->interRAT_ParamsCDMA2000_v920.pres.pres == PRSNT_NODEF)
			{
				/* Copy the e_CSFB_1XRTT_r9 and e_CSFB_ConcPS_Mob1XRTT_r9 values*/
				wrFillTknU8(&(cdma2000_v920->pres), PRSNT_NODEF);
				wrFillTknU32(&(cdma2000_v920->e_CSFB_1XRTT_r9),
						nhuDecNonCriticalExtn->interRAT_ParamsCDMA2000_v920.e_CSFB_1XRTT_r9.val);
				/* Check the presence of ConC PS value */
				if(nhuDecNonCriticalExtn->interRAT_ParamsCDMA2000_v920.e_CSFB_ConcPS_Mob1XRTT_r9.pres == PRSNT_NODEF)
				{
					wrFillTknU32(&(cdma2000_v920->e_CSFB_ConcPS_Mob1XRTT_r9),
							nhuDecNonCriticalExtn->interRAT_ParamsCDMA2000_v920.e_CSFB_ConcPS_Mob1XRTT_r9.val);
				}
			}
			/*RRC Rel10 Upgrade - start*/
			nhuDecNonCriticalExtn_v940 = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.nonCriticalExtn.nonCriticalExtn;
			if(nhuDecNonCriticalExtn_v940->pres.pres == PRSNT_NODEF)
			{
				wrFillTknU8(&ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.pres, PRSNT_NODEF);
				nhuDecNonCriticalExtn_v1020 = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn;
				if(nhuDecNonCriticalExtn_v1020->pres.pres == PRSNT_NODEF)
				{
					wrFillTknU8(&ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.pres, PRSNT_NODEF);
					nhuDecInterRAT_ParamsCDMA2000_v1020 = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.interRAT_ParamsCDMA2000_v1020;
					wrFillTknU8(&ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.interRAT_ParamsCDMA2000_v1020.pres, PRSNT_NODEF);
					cdma2000_v1020 = &ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.nonCriticalExtn.interRAT_ParamsCDMA2000_v1020;
					if(nhuDecInterRAT_ParamsCDMA2000_v1020->pres.pres == PRSNT_NODEF)
					{
						wrFillTknU32(&(cdma2000_v1020->e_CSFB_dual_1XRTT_r10),
								nhuDecInterRAT_ParamsCDMA2000_v1020->e_CSFB_dual_1XRTT_r10.val);
					}
				}
			}
		}
		/*RRC Rel10 Upgrade - end*/
	}
		RETVALUE(ROK);
}

/** @brief This function is responsible for copying the UE Capability decoded
 *         from RRC to ueCb.
 *
 * @details
 *
 *     Function: wrUmmCopySupportedBandLstGERAN 
 *
 *         Processing steps:
 *            - Copy GERAN Supported band list from 
 *              nhuDecSduCfm to ueCap
 *            - Also V920 IEs for non-critical extensions
 *
 * @param[in]       ueCap         : UE's capability
 * @param[in]       nhuDecSduCfm  : RRC Decoded SDU 
 * @param[in, out]  ueCb          : UE control block
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmCopySupportedBandLstGERAN
(
 WrUeCapInfo                  *ueCap,
 NhuDecodeSduCfm              *nhuDecSduCfm,
 WrUeCb                       *ueCb
)
{
   NhuIRAT_ParamsGERAN       *nhuDecIRATParam = NULLP;
   NhuIRAT_ParamsGERAN       *ueCapIRATGeranParam = NULLP;
   NhuUE_EUTRA_Cap_v920_IEs  *nhuDecNonCriticalExtn = NULLP;
   NhuIRAT_ParamsGERAN_v920  *geranV920 = NULLP;
   U32                       idx;
   U16                       noComp;

   nhuDecIRATParam = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.interRAT_Params.geran;
   nhuDecNonCriticalExtn = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.nonCriticalExtn;
   ueCapIRATGeranParam = &ueCap->eUTRA_Cap.interRAT_Params.geran;
   geranV920 = &ueCap->eUTRA_Cap.nonCriticalExtn.interRAT_ParamsGERAN_v920;
   noComp = nhuDecIRATParam->supportedBandLstGERAN.noComp.val;

   wrFillTknU8(&ueCap->eUTRA_Cap.interRAT_Params.pres, PRSNT_NODEF);
   
   wrFillTknU8(&ueCapIRATGeranParam->pres,PRSNT_NODEF);
   wrFillTknU16(&ueCapIRATGeranParam->supportedBandLstGERAN.noComp,noComp);
 
   WR_GET_MEM(&ueCap->memCp, sizeof(NhuSuppBandGERAN) * nhuDecIRATParam->supportedBandLstGERAN.noComp.val,
            &ueCapIRATGeranParam->supportedBandLstGERAN.member);

   if(NULLP == ueCapIRATGeranParam->supportedBandLstGERAN.member)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      wrUmmDeAllocUeCap(ueCap);
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < noComp; idx++)
   {
      wrFillTknU32(&ueCapIRATGeranParam->supportedBandLstGERAN.member[idx],
            nhuDecIRATParam->supportedBandLstGERAN.member[idx].val);   
   }
  
   if(PRSNT_NODEF == nhuDecNonCriticalExtn->pres.pres)
   {
      wrFillTknU8(&ueCap->eUTRA_Cap.nonCriticalExtn.pres, PRSNT_NODEF);
      wrFillTknU8(&geranV920->pres, PRSNT_NODEF);

      /*FIll DTM Capability*/
      if(PRSNT_NODEF == nhuDecNonCriticalExtn->interRAT_ParamsGERAN_v920.dtm_r9.pres)
      {
         wrFillTknU32(&geranV920->dtm_r9,
               nhuDecNonCriticalExtn->interRAT_ParamsGERAN_v920.dtm_r9.val);
         ueCb->csfbInfo.ueDTMCap = TRUE;
      }

      /*Fill e-Redirection_r9*/
      if(PRSNT_NODEF == 
            nhuDecNonCriticalExtn->interRAT_ParamsGERAN_v920.e_RedirectionGERAN_r9.pres)
      {
         wrFillTknU32(&geranV920->e_RedirectionGERAN_r9,
         nhuDecNonCriticalExtn->interRAT_ParamsGERAN_v920.e_RedirectionGERAN_r9.val);
      }
   } 
   
   RETVALUE(ROK);
}
/* SPS changes starts */
/** @brief This function is responsible for handling the UE EUTRA capability 9A0
 *         extention decode confirm from RRC. This function only copies FDD and
 *         TDD FGIs (excluding FGI ADD). Can be enhanced if required in future.
 *
 * @details
 *
 *     Function: wrUmmUecapEutraIe9a0DecCfmHdl
 *
 *         Processing steps:
 *         - if the encode status is success
 *           - copy the decoded information as part of the ueCap
 *
 * @param[in, out]  ueCap         : UE Capability Information Structure
 * @param[in]      nhuDecSduCfm   : RRC Decoded SDU
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmUecapEutraIe9a0DecCfmHdl
(
WrUeCapInfo                  *ueCap,
NhuDecodeSduCfm              *nhuDecSduCfm
)
{
   NhuUE_EUTRA_Cap_v9a0_IEs  *ueCap_v9a0_dst = &ueCap->eUTRA_Cap_v9a0;
   NhuUE_EUTRA_Cap_v9a0_IEs  *ueCap_v9a0_src = &nhuDecSduCfm->ie.eUTRA_Cap_v9a0;
   if (ueCap_v9a0_src->pres.pres == PRSNT_NODEF)
   {
      ueCap_v9a0_dst->featureGroupIndRel9Add_r9.val = NULLP;
      if (ueCap_v9a0_src->featureGroupIndRel9Add_r9.pres == PRSNT_NODEF)
      {
         ueCap_v9a0_dst->featureGroupIndRel9Add_r9.pres = PRSNT_NODEF;
      WR_GET_MEM(&ueCap->memCp, sizeof(U32),
            &ueCap_v9a0_dst->featureGroupIndRel9Add_r9.val);
      if(ueCap_v9a0_dst->featureGroupIndRel9Add_r9.val == NULLP)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            wrUmmDeAllocUeCap(ueCap);
            RETVALUE(RFAILED);
         }
         ueCap_v9a0_dst->featureGroupIndRel9Add_r9.len = ueCap_v9a0_src->featureGroupIndRel9Add_r9.len;
         ueCap_v9a0_dst->featureGroupIndRel9Add_r9.val[0] = ueCap_v9a0_src->featureGroupIndRel9Add_r9.val[0];
         ueCap_v9a0_dst->featureGroupIndRel9Add_r9.val[1] = ueCap_v9a0_src->featureGroupIndRel9Add_r9.val[1];
         ueCap_v9a0_dst->featureGroupIndRel9Add_r9.val[2] = ueCap_v9a0_src->featureGroupIndRel9Add_r9.val[2];
         ueCap_v9a0_dst->featureGroupIndRel9Add_r9.val[3] = ueCap_v9a0_src->featureGroupIndRel9Add_r9.val[3];
         RLOG4(L_DEBUG, "\nwrUmmUecapEutraIe9a0DecCfmHdl: featureGroupIndRel9Add_r9=[0x %02x %02x %02x %02x]\n", \
               ueCap_v9a0_src->featureGroupIndRel9Add_r9.val[0], ueCap_v9a0_src->featureGroupIndRel9Add_r9.val[1], \
               ueCap_v9a0_src->featureGroupIndRel9Add_r9.val[2], ueCap_v9a0_src->featureGroupIndRel9Add_r9.val[3]);
      }
      ueCap_v9a0_dst->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val = NULLP;
      if ((ueCap_v9a0_src->fdd_Add_UE_EUTRA_Capabilities_r9.pres.pres == PRSNT_NODEF) && \
            (ueCap_v9a0_src->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.pres == PRSNT_NODEF))
      {
         ueCap_v9a0_dst->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.pres = PRSNT_NODEF;
         WR_GET_MEM(&ueCap->memCp, sizeof(U32),
            &ueCap_v9a0_dst->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val);
         if(ueCap_v9a0_dst->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val == NULLP)
         {
            if (ueCap_v9a0_dst->featureGroupIndRel9Add_r9.val != NULLP)
            {
               wrUmmDeAllocUeCap(ueCap);
            }
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         ueCap_v9a0_dst->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.len = \
                      ueCap_v9a0_src->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.len;
         ueCap_v9a0_dst->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[0] = \
                         ueCap_v9a0_src->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[0];
         ueCap_v9a0_dst->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[1] = \
                         ueCap_v9a0_src->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[1];
         ueCap_v9a0_dst->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[2] = \
                         ueCap_v9a0_src->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[2];
         ueCap_v9a0_dst->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[3] = \
                         ueCap_v9a0_src->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[3];
         RLOG4(L_DEBUG, "\nwrUmmUecapEutraIe9a0DecCfmHdl: FDD featureGroupIndicators_r9=[0x %02x %02x %02x %02x]\n", \
               ueCap_v9a0_src->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[0], \
               ueCap_v9a0_src->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[1], \
               ueCap_v9a0_src->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[2], \
               ueCap_v9a0_src->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[3]);
      }
      ueCap_v9a0_dst->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val = NULLP;
      if ((ueCap_v9a0_src->tdd_Add_UE_EUTRA_Capabilities_r9.pres.pres == PRSNT_NODEF) && \
            (ueCap_v9a0_src->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.pres == PRSNT_NODEF))
      {
         ueCap_v9a0_dst->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.pres = PRSNT_NODEF;
         WR_GET_MEM(&ueCap->memCp, sizeof(U32),
            &ueCap_v9a0_dst->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val);
         
         if(ueCap_v9a0_dst->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val == NULLP)
         {
            if (ueCap_v9a0_dst->fdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val != NULLP)
            {
               wrUmmDeAllocUeCap(ueCap);
            }
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         ueCap_v9a0_dst->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.len = \
                      ueCap_v9a0_src->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.len;
         ueCap_v9a0_dst->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[0] = \
                      ueCap_v9a0_src->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[0];
         ueCap_v9a0_dst->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[1] = \
                      ueCap_v9a0_src->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[1];
         ueCap_v9a0_dst->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[2] = \
                      ueCap_v9a0_src->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[2];
         ueCap_v9a0_dst->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[3] = \
                      ueCap_v9a0_src->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[3];
         RLOG4(L_DEBUG, "\nwrUmmUecapEutraIe9a0DecCfmHdl: TDD featureGroupIndicators_r9=[0x %02x %02x %02x %02x]\n", \
               ueCap_v9a0_src->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[0], \
               ueCap_v9a0_src->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[1], \
               ueCap_v9a0_src->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[2], \
               ueCap_v9a0_src->tdd_Add_UE_EUTRA_Capabilities_r9.featureGroupIndicators_r9.val[3]);
      }
      if ((ueCap_v9a0_src->nonCriticalExtn.pres.pres == PRSNT_NODEF) &&
          (ueCap_v9a0_src->nonCriticalExtn.interRAT_ParamsUTRA_v9c0.
           pres.pres == PRSNT_NODEF) &&
          (ueCap_v9a0_src->nonCriticalExtn.interRAT_ParamsUTRA_v9c0.
           srvcc_FromUTRA_FDD_ToUTRA_FDD_r9.pres == PRSNT_NODEF))
      {
         ueCap_v9a0_dst->nonCriticalExtn.interRAT_ParamsUTRA_v9c0.pres.pres = PRSNT_NODEF;
         ueCap_v9a0_dst->nonCriticalExtn.interRAT_ParamsUTRA_v9c0.
         srvcc_FromUTRA_FDD_ToUTRA_FDD_r9.pres = PRSNT_NODEF;
         ueCap_v9a0_dst->nonCriticalExtn.interRAT_ParamsUTRA_v9c0.
         srvcc_FromUTRA_FDD_ToUTRA_FDD_r9.val = \
         ueCap_v9a0_src->nonCriticalExtn.interRAT_ParamsUTRA_v9c0.
         srvcc_FromUTRA_FDD_ToUTRA_FDD_r9.val;

      }
   }
   RETVALUE(ROK);
}
/* SPS changes ends */

/** @brief This function is responsible for handling the UE EUTRA capability 
 *         decode confirm from RRC.
 *
 * @details
 *
 *     Function: wrUmmUecapEutraIeDecCfmHdl
 *
 *         Processing steps:
 *         - if the encode status is success
 *           - copy the decoded information as part of the initial context
 *             setup transaction Cb
 *           - commit this inforation to UE CB once the transaction is 
 *             successfully completed
 *
 * @param[in]       ueCb          : UE Control  Block
 * @param[in, out]  ueCap         : UE Capability Information Structure
 * @param[in]      nhuDecSduCfm  : RRC Decoded SDU
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmUecapEutraIeDecCfmHdl
(
 /* SPS changes starts */
 WrUeCb                       *ueCb,
 /* SPS changes ends */
WrUeCapInfo                  *ueCap,
NhuDecodeSduCfm              *nhuDecSduCfm
)
{
   U32                       idx;
   U32                       idx1;
   NhuRF_Params              *nhuDecRfParams = NULLP;
   NhuRF_Params              *rfParams = NULLP;
   NhuMeasParams             *nhuDecMeasParams = NULLP;
   NhuMeasParams             *measParams = NULLP;
   NhuInterFreqBandLst       *nhuDecInterFreqBandLst =  NULLP;
   NhuInterFreqBandLst       *interFreqBandLst =  NULLP;
   NhuInterRAT_BandLst       *nhuDecInterRATBandLst = NULLP;
   NhuInterRAT_BandLst       *interRATBandLst = NULLP;
   /* SPS changes starts */
   S16                       ret      = ROK;
   NhuDecodeSduReq           *nhuDecSduReq = NULLP;
   /* SPS changes ends */
   NhuUE_EUTRA_CapinterRAT_Params *iRATParams = NULLP;
   /* RRC-REL10-Upgrade */
	NhuUE_EUTRA_Cap_v940_IEs *nhuDecNonCriticalExtn_v940 = NULLP;

   if ((nhuDecSduCfm->status != NHU_DECODE_SUCCESS))
   {
      RLOG0(L_ERROR, "Decode of EUTRA CAP IE Failed");
      RETVALUE(RFAILED);
   }

   /* copy the UE EUTRA capabilit */
   /* NOTE: static variables are copied directly in memcpy
    * Dynamic/array variables are copied after the memcpy statement */
   /*cmMemcpy((U8*)&ueCap->eUTRA_Cap, (U8*)&nhuDecSduCfm->ie.nhuUE_EUTRA_Cap,
                                                    sizeof(NhuUE_EUTRA_Cap));*/

   wrFillTknU8(&(ueCap->eUTRA_Cap.pres),PRSNT_NODEF);/*ccpu00142032*/
   /* copy Access Stratum release */
   wrFillTknU32(&(ueCap->eUTRA_Cap.accessStratumRls), 
         nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.accessStratumRls.val);
   
   /* CSG_DEV start */
   if(NhuAccessStratumRlsrel8Enum == ueCap->eUTRA_Cap.accessStratumRls.val)
   {
      RLOG0(L_DEBUG, "Processing Non-CSG UE Capability Information");
   }
   else
   {
      RLOG0(L_DEBUG, "Processing CSG UE Capability Information");
   }
   /* CSG_DEV end */

   /* copy feature Group Indicator*/
   if(nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.featureGroupIndicators.pres == PRSNT_NODEF)
   {
      ueCap->eUTRA_Cap.featureGroupIndicators.pres = PRSNT_NODEF;
      WR_GET_MEM(&ueCap->memCp, sizeof(U32), 
               &ueCap->eUTRA_Cap.featureGroupIndicators.val);

      if(ueCap->eUTRA_Cap.featureGroupIndicators.val == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      ueCap->eUTRA_Cap.featureGroupIndicators.len = nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.featureGroupIndicators.len;
      cmMemcpy((U8*)ueCap->eUTRA_Cap.featureGroupIndicators.val, 
            (U8*)nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.featureGroupIndicators.val,sizeof(U32));
   }
   else
   {
      ueCap->eUTRA_Cap.featureGroupIndicators.pres = NOTPRSNT;
   }

   /* RRC-REL10-Upgrade */
   nhuDecNonCriticalExtn_v940 = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.nonCriticalExtn.nonCriticalExtn;
   if(nhuDecNonCriticalExtn_v940->pres.pres == PRSNT_NODEF)
   {
      NhuUE_EUTRA_Cap_v1020_IEs *nhuDecNonCriticalExtn_v1020 = NULLP;

      wrFillTknU8(&ueCap->eUTRA_Cap.nonCriticalExtn.nonCriticalExtn.pres, PRSNT_NODEF);

      nhuDecNonCriticalExtn_v1020 = &nhuDecNonCriticalExtn_v940->nonCriticalExtn;
      if(nhuDecNonCriticalExtn_v1020->pres.pres == PRSNT_NODEF)
      {
         NhuUE_EUTRA_Cap_v1020_IEs *ueCapNonCritExtn_v1020 = NULLP;
         NhuUE_EUTRA_Cap_v1020_IEsfeatureGroupIndRel10_r10 *fGIRel10;

         ueCapNonCritExtn_v1020 = &ueCap->eUTRA_Cap.nonCriticalExtn.\
                                      nonCriticalExtn.nonCriticalExtn;
         wrFillTknU8(&(ueCapNonCritExtn_v1020->pres), PRSNT_NODEF);

         fGIRel10 = &(nhuDecNonCriticalExtn_v1020->featureGroupIndRel10_r10);

         if(fGIRel10->pres == PRSNT_NODEF)
         {
            ueCapNonCritExtn_v1020->featureGroupIndRel10_r10.pres = 
                                                                 PRSNT_NODEF;
            WR_GET_MEM(&ueCap->memCp, sizeof(U32), 
                  &ueCapNonCritExtn_v1020->featureGroupIndRel10_r10.val);

            if(ueCapNonCritExtn_v1020->featureGroupIndRel10_r10.val == NULLP)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }

            ueCapNonCritExtn_v1020->featureGroupIndRel10_r10.len = 
                    nhuDecNonCriticalExtn_v1020->featureGroupIndRel10_r10.len;

            cmMemcpy((U8*)ueCapNonCritExtn_v1020->featureGroupIndRel10_r10.val,
                     (U8*)nhuDecNonCriticalExtn_v1020->featureGroupIndRel10_r10.val,
                     sizeof(U32));
         }
         if(PRSNT_NODEF == nhuDecNonCriticalExtn_v1020->ue_Category_v1020.pres)
         {
            ueCapNonCritExtn_v1020->ue_Category_v1020.pres = PRSNT_NODEF;
            ueCapNonCritExtn_v1020->ue_Category_v1020.val = 
                   nhuDecNonCriticalExtn_v1020->ue_Category_v1020.val;
         }
      }
   }

   /* copy UE Category */
   cmMemcpy((U8*)&ueCap->eUTRA_Cap.ue_Category, 
         (U8*)&nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.ue_Category,
         sizeof(NhuUE_EUTRA_Capue_Category));

   /* copy PDCP Parameters */
   cmMemcpy((U8*)&ueCap->eUTRA_Cap.pdcp_Params, 
         (U8*)&nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.pdcp_Params,
         sizeof(NhuPDCP_Params));

   /* copy Phy layer Parameters */
   cmMemcpy((U8*)&ueCap->eUTRA_Cap.phyLyrParams,
         (U8*)&nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.phyLyrParams,
         sizeof(NhuPhyLyrParams));

   /* copy supportedBandLstEUTRA */
   nhuDecRfParams = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.rf_Params;
   rfParams = &ueCap->eUTRA_Cap.rf_Params;
   wrFillTknU8(&(rfParams->pres), PRSNT_NODEF);
   rfParams->supportedBandLstEUTRA.member = NULLP;
   wrFillTknU16(&(rfParams->supportedBandLstEUTRA.noComp),
              nhuDecRfParams->supportedBandLstEUTRA.noComp.val);
   /*Heap Memory Corruption Fix captured from Electric Fence */
   WR_GET_MEM(&ueCap->memCp, sizeof(NhuSuppBandEUTRA) * nhuDecRfParams->supportedBandLstEUTRA.noComp.val, 
         &rfParams->supportedBandLstEUTRA.member);

   if(rfParams->supportedBandLstEUTRA.member == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   for(idx=0; idx < nhuDecRfParams->supportedBandLstEUTRA.noComp.val; idx++)
   {
      cmMemcpy((U8*)&rfParams->supportedBandLstEUTRA.member[idx],
         (U8*)&nhuDecRfParams->supportedBandLstEUTRA.member[idx],
         sizeof(NhuSuppBandEUTRA));
   }

   /* copy measParams */
   nhuDecMeasParams = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.measParams;
   measParams = &ueCap->eUTRA_Cap.measParams;
   wrFillTknU8(&(measParams->pres), PRSNT_NODEF);
   wrFillTknU16(&(measParams->bandLstEUTRA.noComp), nhuDecMeasParams->bandLstEUTRA.noComp.val);
   measParams->bandLstEUTRA.member = NULLP;
   WR_GET_MEM(&ueCap->memCp, sizeof(NhuBandInfoEUTRA) * nhuDecMeasParams->bandLstEUTRA.noComp.val, 
                  &measParams->bandLstEUTRA.member);
   
   if(measParams->bandLstEUTRA.member == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   for(idx = 0; idx < nhuDecMeasParams->bandLstEUTRA.noComp.val; idx++)
   {
      wrFillTknU8(&measParams->bandLstEUTRA.member[idx].pres, PRSNT_NODEF);
      /* copy interFreqBandLst */
      interFreqBandLst = &measParams->bandLstEUTRA.member[idx].interFreqBandLst;
      nhuDecInterFreqBandLst = &nhuDecMeasParams->bandLstEUTRA.member[idx].interFreqBandLst;

      wrFillTknU16(&interFreqBandLst->noComp, nhuDecInterFreqBandLst->noComp.val);
      interFreqBandLst->member = NULLP;
      WR_GET_MEM(&ueCap->memCp, sizeof(NhuInterFreqBandInfo) * interFreqBandLst->noComp.val,
                  &interFreqBandLst->member);
      
      if(interFreqBandLst->member == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }
      for (idx1 = 0; idx1 < interFreqBandLst->noComp.val; idx1++)
      {
         cmMemcpy((U8*)&(interFreqBandLst->member[idx1]), 
               (U8*)&nhuDecInterFreqBandLst->member[idx1],
               sizeof(NhuInterFreqBandInfo));
      }

      /* copy interRATFreqBandLst */
      interRATBandLst = &measParams->bandLstEUTRA.member[idx].interRAT_BandLst;
      nhuDecInterRATBandLst = &nhuDecMeasParams->bandLstEUTRA.member[idx].interRAT_BandLst;
      /*ccpu00127802*/
      RLOG1(L_DEBUG, "[ENB :: UE Capabilities ] :: NHU DEC INTER RAT no of "
            "Comp[%d]", nhuDecInterRATBandLst->noComp.val);
/* Fix for ccpu00123770 */
      if(nhuDecInterRATBandLst->noComp.val != 0)
      {
         wrFillTknU16(&interRATBandLst->noComp, nhuDecInterRATBandLst->noComp.val);
         interRATBandLst->member = NULLP;
         WR_GET_MEM(&ueCap->memCp, sizeof(NhuInterRAT_BandInfo) * interRATBandLst->noComp.val, 
                &interRATBandLst->member);
         
         if(interRATBandLst->member == NULLP)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            RETVALUE(RFAILED);
         }
         for (idx1 = 0; idx1 < interRATBandLst->noComp.val; idx1++)
         {
            cmMemcpy((U8*)&(interRATBandLst->member[idx1]),
                  (U8*)&nhuDecInterRATBandLst->member[idx1],
                  sizeof(NhuInterRAT_BandInfo));
         }
      }
   }
   iRATParams = &nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.interRAT_Params;
   if(PRSNT_NODEF == iRATParams->pres.pres)
   {
      /* copy supportedBandLstUTRA */
      if(ROK != wrUmmCopySupportedBandLstUtra(ueCap, nhuDecSduCfm))
      {
         RETVALUE(RFAILED);
      }

      /* eCSFB: Copy supportedBandLstCDMA if its available.*/
      if(PRSNT_NODEF == iRATParams->cdma2000_1xRTT.pres.pres)
      {
         if(ROK != wrUmmCopySupportedBandLstCdma2000(ueCap, nhuDecSduCfm))
         {
            RETVALUE(RFAILED);
         }
      }
      /* Copy GERAN supportedBand List*/
      if(PRSNT_NODEF == iRATParams->geran.pres.pres)
      {
         if(ROK != wrUmmCopySupportedBandLstGERAN(ueCap, nhuDecSduCfm,ueCb))
         {
            RETVALUE(RFAILED);
         }

      }
   }
   /* SPS changes starts */
   if ((nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.nonCriticalExtn.pres.pres == PRSNT_NODEF) && \
         (nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.nonCriticalExtn.nonCriticalExtn.pres.pres == PRSNT_NODEF) && \
         (nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.nonCriticalExtn.nonCriticalExtn.\
          lateNonCriticalExtn.pres == PRSNT_NODEF) && (nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.\
             nonCriticalExtn.nonCriticalExtn.lateNonCriticalExtn.len != 0)) 
   {
      /* If the 9A0 extention is present, send the decode request for it. */
      /* Allocate memory for event structure */
      WR_ALLOC(&nhuDecSduReq, sizeof(NhuDecodeSduReq));
      if (nhuDecSduReq == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }

      /* Fill HDR Info */
      wrUmmFillRrcEvntHdr(&(nhuDecSduReq->hdr), ueCb->cellId, ueCb->crnti,
            nhuDecSduCfm->hdr.transId);
      nhuDecSduReq->sduType = NHU_MSG_UECAPINFO_V9A0;

      /* Convert the U8 array  to Msg Buffer */
      ret = SGetMsg(wrCb.init.region, 0, &nhuDecSduReq->mbuf);
      if (nhuDecSduReq->mbuf == NULLP)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         WR_FREE(nhuDecSduReq, sizeof(NhuDecodeSduReq));
         RETVALUE(RFAILED);
      }
      /* Add the received flat buffer to to mbuf */
      ret = SAddPstMsgMult((Data *)nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.nonCriticalExtn.nonCriticalExtn.\
            lateNonCriticalExtn.val, (MsgLen)nhuDecSduCfm->ie.nhuUE_EUTRA_Cap.nonCriticalExtn.nonCriticalExtn.\
            lateNonCriticalExtn.len, nhuDecSduReq->mbuf);
      if (ret != ROK)
      {
         SPutMsg(nhuDecSduReq->mbuf);
         WR_FREE(nhuDecSduReq, sizeof(NhuDecodeSduReq));
         RLOG0(L_ERROR, "wrUmmUecapEutraIeDecCfmHdl: SAddPstMsgMult Failed");
         RETVALUE(RFAILED);
      }

      /* Send Decode UE Capability Req Primitive*/
      if (wrIfmRrcSndDecodeReq(nhuDecSduReq) != ROK)
      {
         SPutMsg(nhuDecSduReq->mbuf);
         RLOG0(L_ERROR, "wrUmmUecapEutraIeDecCfmHdl: wrIfmRrcSndDecodeReq failed");
         WR_FREE(nhuDecSduReq, sizeof(NhuDecodeSduReq));
         RETVALUE(RFAILED);
      }

      RETVALUE(WR_UMM_NHU_MSG_UECAPINFO_V9A0_DEC_SENT);
   }
   /* SPS changes ends */
   RETVALUE(ROK);
}

/** @brief This function is responsible for handling the decode response for 
 *         UE Radio Capability information.
 *
 * @details
 *
 *     Function: wrUmmUeCapRaCapDecCfmHdl 
 *
 *         Processing steps:
 *         - verify the decode confirm status
 *         - send request to decode the UE Capability information
 *
 * @param[in, out]  transCb : transaction control block
 * @param[in]      nhuDecSduCfm : RRC decode response
 * @return S16
 *          -#Success: ROK
 *          -#Failure RFAILED
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

/** @brief This function is called to store the UE capability
 *             info into the transactio control block.
 *
 * @details
 *
 *     Function: wrUmmUeCapStoreCapInfo
 *
 *         Processing steps:
 *         - for the number of RAT information int he message,
 *           - allocate and store the information in the initial context
 *             transaction CB
 *
 * @param[in, out] transCb   : transaction information 
 * @param[in]     ueCapInfo : UE Capability Information
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmUeCapStoreCapInfo
(
WrUmmTransCb                 *transCb,
NhuUECapInfo                 *ueCapInfo
)
{
    U16                      cnt;
    NhuUE_CapRAT_ContLst     *ueContLst;
    WrInitCntxtSetupTransCb  *initCtxtCb;

    initCtxtCb = &(transCb->u.initCtxtCb);
    /* If UeCap is received from MME and memory is allocated to store Ue Cap 
     * But CDMA Pdu was not present,in that case eNB sent UE Capability Enquiry message 
     * to UE. After receiving the UE Capability information need to free allocated memory first here */
    if(initCtxtCb->ueCap != NULLP)
    {
       wrUmmDeAllocUeCap(initCtxtCb->ueCap);
    }

    WR_ALLOCEVNT(&(initCtxtCb->ueCap),sizeof(WrUeCapInfo));
    if (initCtxtCb->ueCap == NULLP)
    {
       RLOG0(L_FATAL, "Memory allocation failed");
       RETVALUE(RFAILED);
    }

    ueContLst = &ueCapInfo->criticalExtns.val.c1.val.ueCapInfo_r8.ue_CapRAT_ContLst;
    for(cnt = 0; cnt < ueContLst->noComp.val; cnt++)
    {
       switch(ueContLst->member[cnt].rat_Typ.val)
       {
          case  NhuRAT_TypeutraEnum:
             {

                WR_GET_MEM(&initCtxtCb->ueCap->memCp,ueContLst->member[cnt].ueCapRAT_Cont.len,&initCtxtCb->ueCap->ueEutraOctStr.val);
                if (initCtxtCb->ueCap->ueEutraOctStr.val == NULLP)
                {
                   RLOG0(L_FATAL, "Memory allocation failed");
                   wrUmmDeAllocUeCap(initCtxtCb->ueCap);
                   RETVALUE(RFAILED);
                }
                cmMemcpy(initCtxtCb->ueCap->ueEutraOctStr.val,
                      ueContLst->member[cnt].ueCapRAT_Cont.val,
                      ueContLst->member[cnt].ueCapRAT_Cont.len);
                initCtxtCb->ueCap->ueEutraOctStr.pres = TRUE;
                initCtxtCb->ueCap->ueEutraOctStr.len =
                   ueContLst->member[cnt].ueCapRAT_Cont.len;
                break;
             }
          /* eCSFB : Added case for CDMA2000_1XRTTEnum, to copy the 
           * CDMA RAT supported information by UE */
          case NhuRAT_Typcdma2000_1XRTTEnum:
          {
             /* Store CDMA2000 1xRTT UeCap Ie Octet */
             WR_GET_MEM(&initCtxtCb->ueCap->memCp, ueContLst->member[cnt].ueCapRAT_Cont.len,&initCtxtCb->ueCap->ueCdmaOctStr.val);
             if (initCtxtCb->ueCap->ueCdmaOctStr.val == NULLP)
             {
                RLOG0(L_FATAL, "Memory allocation failed.");
                wrUmmDeAllocUeCap(initCtxtCb->ueCap);
                RETVALUE(RFAILED);
             }
             cmMemcpy(initCtxtCb->ueCap->ueCdmaOctStr.val,
                   ueContLst->member[cnt].ueCapRAT_Cont.val,
                   ueContLst->member[cnt].ueCapRAT_Cont.len);
             initCtxtCb->ueCap->ueCdmaOctStr.pres = TRUE;
             initCtxtCb->ueCap->ueCdmaOctStr.len =
                ueContLst->member[cnt].ueCapRAT_Cont.len;
             break;
          }
          case NhuRAT_Typgeran_csEnum:
             {
                /* Store GERAN-CS UeCap Ie Octet */
                WR_GET_MEM(&initCtxtCb->ueCap->memCp, ueContLst->member[cnt].ueCapRAT_Cont.len,&initCtxtCb->ueCap->ueGeranCsOctStr.val);
                if (initCtxtCb->ueCap->ueGeranCsOctStr.val == NULLP)
                {
                   RLOG0(L_FATAL, "Memory allocation failed.");
                   wrUmmDeAllocUeCap(initCtxtCb->ueCap);
                   RETVALUE(RFAILED);
                }
                cmMemcpy(initCtxtCb->ueCap->ueGeranCsOctStr.val,
                      ueContLst->member[cnt].ueCapRAT_Cont.val,
                      ueContLst->member[cnt].ueCapRAT_Cont.len);
                initCtxtCb->ueCap->ueGeranCsOctStr.pres = TRUE;
                initCtxtCb->ueCap->ueGeranCsOctStr.len =
                   ueContLst->member[cnt].ueCapRAT_Cont.len;
                break;
             }
          case NhuRAT_Typgeran_psEnum:
             {
                /* Store GERAN-PS UeCap Ie Octet */

                WR_GET_MEM(&initCtxtCb->ueCap->memCp,  ueContLst->member[cnt].ueCapRAT_Cont.len,&initCtxtCb->ueCap->ueGeranPsOctStr.val); 
                if (initCtxtCb->ueCap->ueGeranPsOctStr.val == NULLP)
                {
                   RLOG0(L_FATAL, "Memory allocation failed.");
                   wrUmmDeAllocUeCap(initCtxtCb->ueCap);
                   RETVALUE(RFAILED);
                }
                cmMemcpy(initCtxtCb->ueCap->ueGeranPsOctStr.val,
                      ueContLst->member[cnt].ueCapRAT_Cont.val,
                      ueContLst->member[cnt].ueCapRAT_Cont.len);
                initCtxtCb->ueCap->ueGeranPsOctStr.pres = TRUE;
                initCtxtCb->ueCap->ueGeranPsOctStr.len =
                   ueContLst->member[cnt].ueCapRAT_Cont.len;
                break;
             }             
          case NhuRAT_TyputraEnum:
             {
                /* Store UTRAN UeCap Ie Octet */
                WR_GET_MEM(&initCtxtCb->ueCap->memCp, ueContLst->member[cnt].ueCapRAT_Cont.len,&initCtxtCb->ueCap->ueUtraOctStr.val);
                if (initCtxtCb->ueCap->ueUtraOctStr.val == NULLP)
                {
                   RLOG0(L_FATAL, "Memory allocation failed.");
                   wrUmmDeAllocUeCap(initCtxtCb->ueCap);
                   RETVALUE(RFAILED);
                }
                cmMemcpy(initCtxtCb->ueCap->ueUtraOctStr.val,
                      ueContLst->member[cnt].ueCapRAT_Cont.val,
                      ueContLst->member[cnt].ueCapRAT_Cont.len);
                initCtxtCb->ueCap->ueUtraOctStr.pres = TRUE;
                initCtxtCb->ueCap->ueUtraOctStr.len =
                   ueContLst->member[cnt].ueCapRAT_Cont.len;
                break;
             }

       }
    }
    RETVALUE(ROK);
}

/** @brief This function is responsible for handling the UE capability decode
 *         response from RRC.
 *
 * @details
 *
 *     Function: wrUmmUeCapUeCapDecCfmHdl
 *
 *         Processing steps:
 *         - request RRC to decode the EUTRA radio capability information
 *
 * @param[in, out] transCb      : transaction information 
 * @param[in]     nhuDecSduCfm : RRC decoded SDU
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmUeCapUeCapDecCfmHdl
(
WrUmmTransCb                 *transCb,
NhuDecodeSduCfm              *nhuDecSduCfm
)
{
   S16                       ret           = RFAILED;
   WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);
   NhuUECapInfo              *ueCapInfo    = NULLP;

   if((nhuDecSduCfm->status != NHU_DECODE_SUCCESS))
   {
      RLOG0(L_ERROR, "Decoding of UE CAP Failed");
      RETVALUE(RFAILED);
   }
   ueCapInfo = &(nhuDecSduCfm->ie.ueCapInfo);
   /* eCSFB:- Set the transCb state , as unable to
    * store ueCapInfo due to mem allocation failue*/
   if(ROK != wrUmmUeCapStoreCapInfo(transCb , ueCapInfo))
   {
      initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
      RETVALUE(RFAILED);
   }

   if (initCtxtCb->ueCap->ueEutraOctStr.pres)
   {
      /* Decode EUTRA UeCap Ie */
      ret = wrUmmRrcDecReqHdl(transCb, &initCtxtCb->ueCap->ueEutraOctStr,
                                            NHU_DEC_SDUTYPE_EUTRA_UECAP_IE);
   }

   RETVALUE(ret);
}

/** @brief This function is responsible for exlcuding UTRA octet string for
 *         encoding as this should not be included in UE radio capabilities
 *         while forwarding the UE capability to MME.
 *
 * @details
 *
 *     Function: wrUmmFillUeRaCapForEncWoUTRA
 *
 *         Processing steps:
 *            - Loop through the list of RAT capabilities supported by the UE
 *            and copy only if the RAT type is not UTRA.
 *              
 * @param[in,out]  encSdu : SDU to be encoded
 * @param[in]   ueCap : UE's capability
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmFillUeRaCapForEncWoUTRA
(
 NhuEncReqSdus   *encSdu,
 NhuUECapInfo    *ueCap
)
{
   NhuUECapInfo           *encSduCap = NULLP;
   NhuUE_CapRAT_Cont      *member = NULLP;
   NhuUE_CapRAT_Cont      *sduMem = NULLP;
   U32                    idx;
   U32                    idx1 = 0;
   U16                    noComp;

   member = ueCap->criticalExtns.val.c1.val.ueCapInfo_r8.ue_CapRAT_ContLst.member;
   noComp = ueCap->criticalExtns.val.c1.val.ueCapInfo_r8.ue_CapRAT_ContLst.noComp.val;

   encSduCap = &encSdu->sdu.u.ueCapInfo;

   wrFillTknU8(&encSduCap->pres,PRSNT_NODEF);
   wrFillTknU32(&encSduCap->rrc_TrnsxnId,0);
   wrFillTknU8(&encSduCap->criticalExtns.choice,CRITICALEXTNS_C1);
   wrFillTknU8(&encSduCap->criticalExtns.val.c1.choice,C1_UECAPINFO_R8);
   wrFillTknU8(&encSduCap->criticalExtns.val.c1.val.ueCapInfo_r8.pres,PRSNT_NODEF);


   /* This memory is freed in NhUiNhuEncodeReq*/
   WR_GET_MEM(&encSdu->memCp,noComp * sizeof(NhuUE_CapRAT_Cont),&sduMem);
   if(NULLP == sduMem)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   for(idx = 0; idx < noComp; idx++)
   {
      if((PRSNT_NODEF == member[idx].pres.pres) 
            && (NhuRAT_TyputraEnum != member[idx].rat_Typ.val))
      {
         wrFillTknU8(&(sduMem[idx1].pres),PRSNT_NODEF);
         wrFillTknU32(&(sduMem[idx1].rat_Typ),member[idx].rat_Typ.val);
         wrFillTknStrOSXL(&(sduMem[idx1].ueCapRAT_Cont),member[idx].ueCapRAT_Cont.len,
              member[idx].ueCapRAT_Cont.val,&encSdu->memCp);
         idx1++;
      }
   }

   encSduCap->criticalExtns.val.c1.val.ueCapInfo_r8.ue_CapRAT_ContLst.member = sduMem;
   wrFillTknU16(&encSduCap->criticalExtns.val.c1.val.ueCapInfo_r8.ue_CapRAT_ContLst.noComp,idx1);
   RETVALUE(ROK);
}
/** @brief This function is responsible for sending the UE Capability enquiry
 *         request to RRC
 *
 * @details
 *
 *     Function: wrUmmSndUeCapEncReq
 *
 *         Processing steps:
 *         - copy the ue capability information to the endcode request
 *           structure
 *         - invoke wrIfmRrcSndEncodeReq to send the request to RRC
 *
 * @param[in]  ueCb      : UE Control Block
 * @param[in]  transId   : transaction id
 * @param[in]  ueCapInfo : UE Capability RRC structure
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
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
   if (NULLP == encSdu)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   /* Fill HDR Info */
   wrUmmFillRrcEvntHdr(&(encSdu->hdr), ueCb->cellId, ueCb->crnti, transId);
   encSdu->sdu.msgCategory = NHU_MSG_UECAPINFO; 

   /* Exclude UTRA capabilities*/
   if(RFAILED == wrUmmFillUeRaCapForEncWoUTRA(encSdu,ueCapInfo))
   {
      RLOG0(L_ERROR,"wrUmmFillUeRaCapForEncWoUTRA failed");
      RETVALUE(RFAILED);
   }
    
    /* Send Encode UE RA Capability Info Req Primitive*/
   if (wrIfmRrcSndEncodeReq(encSdu) != ROK)
   {
      RLOG0(L_ERROR, "wrIfmRrcSndEncodeReq failed");
      WR_FREEEVNT(encSdu);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}


/* ====================================================================== */
/*                      SECURITY RELATED FUNCTIONS                        */
/* ====================================================================== */
/** @brief This function is responsible for filling the RRC security Mode
 *         command message.
 *
 * @details
 *
 *     Function: wrUmmSecSndSecModeCmd
 *
 *         Processing steps:
 *         - fill the rrc interface header
 *         - get a new free transId for Security Mode Command
 *         - fill the security mode command
 *         - send the message to RRC
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmSecSndSecModeCmd
(
WrUmmTransCb                 *transCb
)
{
   NhuUeCfgInfo              *ueCfg = NULLP;
   NhuDatReqSdus             *rrcDatReqSdu = NULLP;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);
   U8                        transId;

   /* Allocate memory for event structure */
   WR_ALLOCEVNT(&rrcDatReqSdu, sizeof(NhuDatReqSdus));
   if (rrcDatReqSdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   /* Fill HDR Info */
   /* setting the subTrans bits to identify the security config procedure */
   wrUmmFillRrcEvntHdr(&(rrcDatReqSdu->hdr), ueCb->cellId, ueCb->crnti,
                                      transCb->transId | C1_SECURITYMODECMD);

   /* Fetch an RRC transaction id for this procedure.                     */
   if(wrUmmGetRrcTransId(ueCb, C1_SECURITYMODECMD, 
                      WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
                      &transId) != ROK)
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI,ueCb->crnti,"Max RRC Transaction already "
         "Reached");
      WR_FREEEVNT(rrcDatReqSdu);
      RETVALUE(RFAILED);
   }
 
   /* Build RRC Con Setup Message */
   if (wrUmmRrcFillSecModeCmd(transId,
                              initCtxtCb->secCfg, rrcDatReqSdu, initCtxtCb->ueCap->eUTRA_Cap.accessStratumRls.val) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSecSndSecModeCmd Failed");
      WR_FREEEVNT(rrcDatReqSdu);
      RETVALUE(RFAILED);
   }

   /* Fill Config values */
   rrcDatReqSdu->sdu.isUeCfgPres = TRUE;
   ueCfg = &(rrcDatReqSdu->sdu.ueCfg);
   ueCfg->ueCfgBitMsk = NHU_EXP_CFG_SEC_CFG;
   wrUmmRrcFillSecCfg(&(ueCfg->secCfg), initCtxtCb->secCfg);

   RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Sending SecurityModeCommand"
         " CipherAlgo[%lu] IntegrityAlgo[%lu]",initCtxtCb->secCfg->ciphAlgo, 
         initCtxtCb->secCfg->intgAlgo);
   /* Send Security Mode Command through NhuDatRsp Primitive*/
   if (wrIfmRrcSndReqToUe(rrcDatReqSdu) != ROK)
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe failed");
      WR_FREEEVNT(rrcDatReqSdu);
      RETVALUE(RFAILED);
   }
   /*Start Security Mode Command Timer*/
#if 0
#else
   wrStartTransTmr(transCb,WR_TMR_RRC_SECUCMD, 
         wrEmmGetRrcSecModTmrVal(ueCb->cellId));
#endif
   RETVALUE(ROK);
}

/** @brief This function is responsible for deriving a key from the inputs.
 *
 * @details
 *
 *     Function: wrUmmSecKeyDervFunc
 *
 *         Processing steps:
 *         - copy a extracted key to the derived key
 *
 * @param[in] secKey  : base key
 * @param[in] strS    : algorithm type
 * @param[in] derKey  : derived key
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmSecKeyDervFunc
(
U8                           *secKey,
U8                           *strS,
U8                           *derKey
)
{
   U8                        extractKey[32];
   U8                        idx = 0;
   U8                        idxKey = 0;

   RLOG0(L_DEBUG, "Key Derivation Function");

   cmMemset(&extractKey[0], '\0', 32);

   /* Call a HMAC-SHA256 Algorithm API  currently not implemented */
   /* truncate the extracted key to 16 bytes long */
   for(idx = 16; idx < 32; idx++)
   {
      derKey[idxKey] = extractKey[idx];
      idxKey++;
   }

   RETVALUE(ROK);
}


#ifndef TENB_AS_SECURITY

PUBLIC S16 wrUmmSecExtractSecKeys
(
WrUeSecCfg                   *secCfg,
TknStrBSXL                   *secKey
)
{
   U8                        stringS[WR_STRING_S_LEN];
   U8                        dervdKey[WR_CIPH_KEY_LEN];
   U8                        type = 0;
   U8                        idx = 0;

   RLOG0(L_DEBUG, "Extract Intg. and ciphering keys");

   /* storing the base key (KENB) */
   for(idx = 0; idx < WR_KENB_KEY_LEN; idx++)
   {
      secCfg->kENB[idx] = secKey->val[idx];
   }

   /* Memset */
   cmMemset((U8 *)&stringS[0], '\0', WR_STRING_S_LEN);

   /* Building the String S = FC || P0 || L0 || P1 || L1 */
   /* FC */
   stringS[0] = 0x15;

   /* L0 */
   stringS[2] = 0x00;
   stringS[3] = 0x01;

   /* L1 */
   stringS[5] = 0x00;
   stringS[6] = 0x01;

   for(type = 0; type <= WR_INTG_KEY; type++)
   {
      /* Memset to zero */
      cmMemset((U8 *)&dervdKey[0], '\0', WR_CIPH_KEY_LEN);

      /* Fill P0 & P1 */ 
      switch(type)
      {
         case WR_CP_CIPH_KEY:
            /* P0 */
            stringS[1] = 0x03;
            /* P1 */
            stringS[4] = secCfg->ciphAlgo;

            /* Use the Key Derivation Function */
            wrUmmSecKeyDervFunc((U8 *)secKey->val, (U8 *)stringS, 
                  (U8 *)dervdKey);

            for (idx = 0; idx < WR_CIPH_KEY_LEN; idx++)
            {
               secCfg->cpCiphKey[idx] = dervdKey[idx];
            }

            break;

         case WR_UP_CIPH_KEY:
            /* P0 */
            stringS[1] = 0x05;
            /* P1 */
            stringS[4] = secCfg->ciphAlgo;

            /* Use the Key Derivation Function */
            wrUmmSecKeyDervFunc((U8 *)secKey->val, (U8 *)stringS, 
                  (U8 *)dervdKey);

            for (idx = 0; idx < WR_CIPH_KEY_LEN; idx++)
            {
               secCfg->upCiphKey[idx] = dervdKey[idx];
            }
            break;

         case WR_INTG_KEY:
            /* P0 */
            stringS[1] = 0x04;
            /* P1 */
            stringS[4] = secCfg->intgAlgo;

            /* Use the Key Derivation Function */
            wrUmmSecKeyDervFunc((U8 *)secKey->val, (U8 *)stringS, 
                  (U8 *)dervdKey);

            for (idx = 0; idx < WR_INTG_KEY_LEN; idx++)
            {
               secCfg->intgKey[idx] = dervdKey[idx];
            }
            break;

         default:
            RETVALUE(RFAILED); 
            break;
      }
   }
   RETVALUE(ROK); 
}
#endif

/*
*       Fun:   wrUmmSecGetSecCap
*
*       Desc:  This function decodes the security information and filles 
*              the secCfg structure.
*
*       Ret:   ROK
*
*       Notes: None
*/
/** @brief This function is responsible for processing the UE security 
 *         capabilities and storing the information into the internal
 *         data structure.
 *
 * @details
 *
 *     Function: wrUmmSecGetSecCap
 *
 *         Processing steps:
 *         - process and store the encryption algorithm supported by UE
 *         - process and store the integrity protection algorithm supported
 *           by UE
 *
 * @param[in]   ueSecCap : transaction information 
 * @param[out]  secCfg   : local data structure to store the security info
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmSecGetSecCap
(
SztUESecurCapabilities       *ueSecCap,
WrUeSecCfg                   *secCfg
)
{
   U8                         idx;
   TknBStr32                  *bitStr = NULLP;
   Bool                       ciphAlgoSel = FALSE;
   Bool                       intgAlgoSel = FALSE;
   U8                         securAlgoMask;

   RLOG0(L_DEBUG, "Fetching Security Info from S1AP Pdu");

   if(secCfg == NULLP || ueSecCap == NULLP)
   {
      RLOG0(L_ERROR, "invalid parameters");
      RETVALUE(RFAILED);
   }


   /* fetch Integrity protection algo */
   bitStr = &(ueSecCap->integrityProtectionAlgorithms);

/* since EPC is not supporting ZUC for Inetgrity so for testing purpose 
    we are hardcoding as ZUC also in UE capabilities */ 
#ifdef ZUC_ALGO_TEST
   bitStr->val[0] |= 0x20;
#endif
   securAlgoMask = bitStr->val[0];

   /*Set actual Integrity algo based on highest priority algo configured ateNodeB*/
   for(idx=0;idx<WR_MAX_AS_SEC_ALGOS;idx++)
   {
      if( (wreNodBPriIntgAlgo[idx].pres) &&
            (wrUmmSelectSecurityAlgo(wreNodBPriIntgAlgo[idx].val,
                                     &secCfg->intgAlgo,securAlgoMask,WR_INTEGRITY_ALGO)) )
      {
         intgAlgoSel = TRUE;
         break;
      }
   }/* end for loop */

   /*high prioroty algo is SNOW3G but UE is AES capable. So select AES*/
   /*high prioroty algo is AES but UE is SNOW3g capable. So select SNOW3g*/

   if(intgAlgoSel == FALSE)
   {
      RLOG0(L_ERROR, "AS Integrity Algorithm negotiation failed");
      RETVALUE(RFAILED);
   }
   /* fetch ciphering algo */
   bitStr = &(ueSecCap->encryptionAlgorithms);

/* since EPC is not supporting ZUC for ciphering so for testing purpose 
    we are hardcoding as ZUC also in UE capabilities */ 
#ifdef ZUC_ALGO_TEST
   bitStr->val[0] |= 0x20;
#endif
   securAlgoMask = bitStr->val[0];

   /*Set actual ciphering algo Alog based on highest priority algo configured ateNodeB*/
   for(idx=0;idx<WR_MAX_AS_SEC_ALGOS;idx++)
   {
      if( (wreNodBPriCiphAlgo[idx].pres) &&
            (wrUmmSelectSecurityAlgo(wreNodBPriCiphAlgo[idx].val,
                                     &secCfg->ciphAlgo,securAlgoMask,WR_CIPHERING_ALGO)) )
      {
         ciphAlgoSel = TRUE;
         break;
      }
   }/* end for loop */

   /*high prioroty algo is SNOW3G but UE is AES capable. So select AES*/
   /*high prioroty algo is AES but UE is SNOW3g capable. So select SNOW3g*/

   if(ciphAlgoSel == FALSE)
   {
       RLOG0(L_ERROR, "AS Ciphering Algorithm negotiation failed");
       RETVALUE(RFAILED);
   }

   /*Set actual ciphering algo Alog based on highest priority algo configured ateNodeB*/
   if(secCfg->intgAlgo == WR_UMM_NULL_ALGO && secCfg->ciphAlgo != WR_UMM_NULL_ALGO)
   {
       RLOG0(L_ERROR, "AS Algorithm negotiation failed");
       RETVALUE(RFAILED);
   }

   /* ccpu00130881 */
   /* store the UE capability info */
   WR_MEM_COPY(&secCfg->encrypCap, &ueSecCap->encryptionAlgorithms, 
               sizeof(TknBStr32));
   WR_MEM_COPY(&secCfg->integCap, &ueSecCap->integrityProtectionAlgorithms, 
               sizeof(TknBStr32));

   RETVALUE(ROK);

}

/** @brief This function is responsible for fetching a new LCH and DRB id for
 *         the incoming ERABs.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtPrcRabSetup
 *
 *         Processing steps:
 *         - for the number of RABs that where successfully parsed/admitted, 
 *           - if the QCI is supportted
 *             - dont assign LCH and DRB id
 *           - else, get a free LCH and DRB id
 *           - set the RAB admitted flag to TRUE,
 *           - increment the number of RABs that are admitted
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmInitCtxtPrcRabSetup
(
WrUmmTransCb                 *transCb
)
{
   WrErabInfo                *rabInfo;
   WrInitCntxtSetupTransCb   *initCtxt = &transCb->u.initCtxtCb;
   U32                       idx;
   S16                       ret = RFAILED;
   U16                       cellId = transCb->ueCb->cellId;
   WrUmmCellCb               *ummCellCb = NULLP;
   U8                        freeIdx = 0;

   WR_UMM_GET_CELLCB(ummCellCb, cellId);
   if(NULLP == ummCellCb)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID,(U32)cellId,"Error in getting CELL CB");
      RETVALUE(RFAILED);
   }

   /* Need to determine LCH Id and RB Id for each ERAB to be setup        */
   for (idx = 0; idx < initCtxt->numErabs; idx++)
   {
      rabInfo = &(initCtxt->erabList[idx]);
      /* ccpu00127706: avoid unnecessary processing for the multiple instance bearers  identified earlier*/
      if(rabInfo->rabAllowed == FALSE)
      {
         continue;
      }
      rabInfo->tnlSetup = WR_INIT_CTXT_TNL_SETUP_NOT_SENT; 
      rabInfo->schdCfgState = WR_INIT_CTXT_SCHD_CFG_NOT_SENT; 

      freeIdx = (rabInfo->erabId) - WR_UMM_ERAB_OFFSET; 
      if((freeIdx >= WR_MAX_DRBS)|| 
           (transCb->ueCb->rbInfo.rab[freeIdx].inUse))
      {
         /* Duplication of the E_RAB setup */
         /* Error_Cause:Changes */
         rabInfo->cause.causeTyp = CAUSE_RADIONW;
         rabInfo->cause.causeVal =(freeIdx >= WR_MAX_DRBS)?
                        SztCauseRadioNwunknown_E_RAB_IDEnum:
                        SztCauseRadioNwmultiple_E_RAB_ID_instancesEnum;
         rabInfo->rabAllowed = FALSE;
         initCtxt->numErabFail++;
         continue;
      }
      
      if (ummCellCb->wrRabCfgParams[rabInfo->qci].qciSupported == FALSE)
      {
         /* Error_Cause:Changes */
         rabInfo->cause.causeTyp = CAUSE_RADIONW;
         rabInfo->cause.causeVal = SztCauseRadioNwnot_supported_QCI_valueEnum;
         rabInfo->rabAllowed = FALSE;
         initCtxt->numErabFail++;
         /*Update Statistics for Default ERAB : Failure Due to unsupported QCI*/
         wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_DEFAULT,
           WR_EMM_DFLT_POS_STEP,WR_EMM_ERAB_FAIL_RELEASEDUETOEUTRANGENERATEDREASONS);
         continue;
      }

      if (wrUmmGetLch(transCb->ueCb, &(rabInfo->lchId)) != ROK)
      {
         /*ccpu00127802*/
         RLOG0(L_DEBUG, "Failed to allocate LCH for a RAB");
         /* Error_Cause:Changes */
         rabInfo->cause.causeTyp = CAUSE_RADIONW;
         rabInfo->cause.causeVal = SztCauseRadioNwradio_resources_not_availableEnum;
         rabInfo->rabAllowed = FALSE;
         initCtxt->numErabFail++;
         continue;
      }
      if (wrUmmGetDrb(transCb->ueCb, &(rabInfo->drbId)) != ROK)
      {
         /*ccpu00127802*/
         RLOG0(L_DEBUG, "Failed to allocate DRB for a RAB");
         wrUmmRelLch(transCb->ueCb, rabInfo->lchId);
         /* Error_Cause:Changes */
         rabInfo->cause.causeTyp = CAUSE_RADIONW;
         rabInfo->cause.causeVal = SztCauseRadioNwradio_resources_not_availableEnum;
         rabInfo->rabAllowed = FALSE;
         initCtxt->numErabFail++;
         continue;
      }
      initCtxt->numErabsAllowed++;
      ret = ROK;
   }

   RETVALUE(ret);
}
/* ccpu00129384: Re-Configure CTF with UE Category */
/**
 *  @brief This function builds and sends Re-Configuration Request to PHY.
 *         This is mainly to send UE category to PHY.
 *
 *      Function: wrUmmInitCtxtCfgPhy
 *
 *          Processing steps:
 *          - Allocate memory for ue reCfg message
 *          - update the structure
 *          - call the interface function to send the message
 *
 *
 *  @param [in]  transCb : transCb of the UE reconfig message sent to Ctf
 *  @return S16
    -# Success : ROK
    -# Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtCfgPhy
(
WrUmmTransCb                 *transCb
)
{
   CtfCfgReqInfo             *reCfgInfo = NULLP;
   CtfDedRecfgInfo           *ueCfgParam = NULLP;
   U32                       transId;

   WR_ALLOC(&reCfgInfo, sizeof(CtfCfgReqInfo));
   if(reCfgInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   transId = transCb->transId;
   reCfgInfo->cfgType          = CTF_RECONFIG;
   reCfgInfo->u.reCfg.cfgElem  = CTF_UE_CFG;

   ueCfgParam = &reCfgInfo->u.reCfg.u.dedRecfg;

   ueCfgParam->cellId        = transCb->ueCb->cellId;
   ueCfgParam->ueId          = transCb->ueCb->crnti;
   ueCfgParam->newUeId       = transCb->ueCb->crnti;
   wrUmmSchdFillUeCategory(&ueCfgParam->ueCatCfg.ueCategory,
                            transCb->ueCb->ueCat);
   ueCfgParam->ueCatCfg.pres = TRUE;
   if(ROK != wrIfmPhyUeCfg(reCfgInfo, transId))
   {
      RLOG0(L_ERROR, "wrIfmPhyUeCfg Failed");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
/* SPS changes starts */
/** @brief This function is responsible for filling the UL SPS LC for SCHD.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtSchdUlSpsLchCfgReq
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

PUBLIC Void wrUmmInitCtxtSchdUlSpsLchCfgReq
(
 RmuUeRecfgRsp        *rrmUeRecfgResp,
 U32                  numErabs,
 Void                 *pErabList,
 RgrUeSpsCfg          *ueSpsCfg
)
{
   U32 count;
   WrErabInfo *pErabInfo = (WrErabInfo *)pErabList;

   ueSpsCfg->ulSpsCfg.lcCnt = 0;
   for (count = 0; count < numErabs; count++)
   {
      if(((pErabInfo+count)->rabAllowed == TRUE) && (rrmUeRecfgResp->stErabAcceptLst[count].bIsUlSpsEnabled == TRUE))
      {
         /* Here Application should take stuffs from the RRM accepted list but currently 
          * there is no RBC support at both APP and RRM. Thus the expectation is, RRM has accepted 
          * all the ERABs in the request and thus have provided ERAB in the same order as sent to it. */
         ueSpsCfg->ulSpsCfg.spsLcInfo[ueSpsCfg->ulSpsCfg.lcCnt].isSpsEnabled = TRUE;
         ueSpsCfg->ulSpsCfg.spsLcInfo[ueSpsCfg->ulSpsCfg.lcCnt].lcId = (pErabInfo+count)->lchId;
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
/** @brief This function is responsible for reconfiguring the UE context
 *         at the scheduler.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtSchdUeRecfgReq
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
PRIVATE S16 wrUmmInitCtxtSchdUeRecfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrInitCntxtSetupTransCb   *initCtxt = &transCb->u.initCtxtCb;
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   U32                       transId;
   /* SPS changes starts */
   U32                       count  = 0;
   /* SPS changes ends */
   WrUmmCellCb               *cellCb = NULLP;

   WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId);
   if(cellCb == NULLP)
   {
      RLOG0(L_ERROR, " CellCb is NULL. ");
      RETVALUE(RFAILED);
   }

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Set last 4 bits as 1 for UE reconfiguration */
   transId = transCb->transId | WR_INIT_CTXT_UE_TRANS_ID;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   if ((initCtxt->dlAmbr != 0) || (initCtxt->ulAmbr != 0))
   {
      ueRecfg->ueRecfgTypes = RGR_UE_QOS_RECFG;
      ueRecfg->ueUlHqRecfg.maxUlHqTx = wrUeDb.cells[WR_DFLT_CELL_IDX].macMainCfg.ulSchCfg.maxHARQ_Tx;
      ueRecfg->ueQosRecfg.ambrPres = TRUE;
      ueRecfg->ueQosRecfg.dlAmbr   = initCtxt->dlAmbr / 8;
      ueRecfg->ueQosRecfg.ueBr     = initCtxt->ulAmbr / 8;
   }
   else
   {
      RLOG_ARG0(L_INFO,DBG_CRNTI,transCb->ueCb->crnti,"AMBR is not configured");
   }

   wrUmmSchdFillUeRadioResCfgInfo(ueRecfg,
               (initCtxt->rrmUeRecfgRspMsg->u.rrmUeRecfgResp), transCb->ueCb,FALSE);


   /* Fill UE Category */
   ueRecfg->ueRecfgTypes |= RGR_UE_UECAT_RECFG;
   wrUmmSchdFillUeCategory(&ueRecfg->ueCatEnum, transCb->ueCb->ueCat);

   /* SPS changes starts */
   /* The initialisation from the ueCb is required for the following case:
    * 1) If the SPS was already enabled for the UE, thus RRM doesnot allocate SPS
    *    - The SR mask for all the LCs should be set for rel 9 UE based on this.
    * 2) If the SPS was neither enabled previously nor it is enabled by RRM
    *    - Donot enable SR Mask for any of the LCs based on this. */
   for (count = 0; count < initCtxt->numErabs; count++)
   {
      initCtxt->erabList[count].isUlSpsEnabled = FALSE;
   }
   transCb->spsCfgTransCb.isUeUlSpsEnabled = transCb->ueCb->spsCfg.spsCfgUl.bIsSpsEnabled;
   transCb->spsCfgTransCb.isUeDlSpsEnabled = transCb->ueCb->spsCfg.spsCfgDl.bIsSpsEnabled;
   wrUmmFillSpsSchdReCfgUe(transCb->transTyp, transCb->ueCb, \
         initCtxt->rrmUeRecfgRspMsg->u.rrmUeRecfgResp, ueRecfg, initCtxt->numErabs, \
         (Void *)(initCtxt->erabList), &transCb->spsCfgTransCb.spsCfg, \
         &transCb->spsCfgTransCb.isUeDlSpsEnabled, &transCb->spsCfgTransCb.isUeUlSpsEnabled);
   /* SPS changes ends */

   /* HCSG_DEV Start */
   /* Update the csg membership status */
   if(WR_ENB_HYBRID_ACCESS == cellCb->csgAccessMode)
   {
      if(WR_UMM_CSG_MEMBER == transCb->u.initCtxtCb.ueMbrShip)
      {
         ueRecfg->csgMmbrSta = TRUE;
         ueRecfg->ueRecfgTypes |= RGR_UE_CSG_PARAM_RECFG;
      }
   }
   /* HCSG_DEV End */

   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}

/** @brief This function is responsible for preparing and sending the logical
 *         channel configuration for SRB 2 to scheduler.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtSchdSrbLchCfgReq
 *
 *         Processing steps:
 *         - allocate a new configuration request
 *         - set the configuration type to RGR_LCH_CFG
 *         - fill the ue info and logical channel info
 *         - send the request to scheduler
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtSchdSrbLchCfgReq
(
WrUmmTransCb                 *transCb,
U8                           idx
)
{
   RgrCfgReqInfo             *cfgReq;
   RgrLchCfg                 *lchCfg;
   U32                       transId;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Set last 4 bits as 1 for UE reconfiguration */
   transId = transCb->transId | idx;
   lchCfg  = &cfgReq->u.cfgInfo.u.lchCfg;
   cfgReq->action = RGR_CONFIG;
   cfgReq->u.cfgInfo.cfgType = RGR_LCH_CFG;
   lchCfg->cellId = transCb->ueCb->cellId;
   lchCfg->crnti  = transCb->ueCb->crnti;
   lchCfg->lcId   = WR_PDCP_ID_SRB2;
   lchCfg->lcType = CM_LTE_LCH_DCCH;
   lchCfg->dlInfo.dlTrchType = CM_LTE_TRCH_DL_SCH;
   /*TODO Sriky  currently hard coded to be verified*/
   lchCfg->dlInfo.dlQos.qci = 5;
   lchCfg->ulLchQciInfo.lcId = WR_PDCP_ID_SRB2;
   lchCfg->ulLchQciInfo.qci = SRB_QCI_VAL;
   lchCfg->ulLchQciInfo.lcgId = 0;

   lchCfg->dlInfo.dlQos.gbr = 1;
   lchCfg->dlInfo.dlQos.mbr = 1;
#ifdef LTE_L2_MEAS
   lchCfg->lcgId            = 0;
#endif


   /* Dispatch the prepared message to scheduler                         */
   WrIfmSchdCfgUe(cfgReq, transId);

   RETVALUE(ROK);
}

/** @brief This function is responsible for preparing and sending the logical
 *         channel configuration for DRBs to scheduler.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtSchdLchCfgReq
 *
 *         Processing steps:
 *         - allocate a new configuration request
 *         - set the configuration type to RGR_LCH_CFG
 *         - fill the ue info and logical channel info
 *         - send the request to scheduler
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtSchdLchCfgReq
(
WrUmmTransCb                 *transCb,
U8                           idx
)
{
   WrInitCntxtSetupTransCb   *initCtxt = &transCb->u.initCtxtCb;
   RgrCfgReqInfo             *cfgReq;
   RgrLchCfg                 *lchCfg;
   U32                       transId;
   WrRabCfgParams                       *params;
   WrLchCfgParams                       *lch;
   WrUmmCellCb                          *ummCellCb = NULLP;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RETVALUE(RFAILED);
   }
   /* Set last 4 bits as 1 for UE reconfiguration */
   transId = transCb->transId | idx;
   lchCfg  = &cfgReq->u.cfgInfo.u.lchCfg;
   cfgReq->action = RGR_CONFIG;
   cfgReq->u.cfgInfo.cfgType = RGR_LCH_CFG;
   lchCfg->cellId = transCb->ueCb->cellId;
   lchCfg->crnti  = transCb->ueCb->crnti;
   lchCfg->lcId   = initCtxt->erabList[idx].lchId;
   lchCfg->lcType = CM_LTE_LCH_DTCH;
   lchCfg->dlInfo.dlTrchType = CM_LTE_TRCH_DL_SCH;
   lchCfg->dlInfo.dlQos.qci = initCtxt->erabList[idx].qci;
   lchCfg->dlInfo.dlQos.gbr = initCtxt->erabList[idx].qos.dlGbr;
   lchCfg->dlInfo.dlQos.mbr = initCtxt->erabList[idx].qos.dlMbr;
   lchCfg->ulLchQciInfo.lcId = initCtxt->erabList[idx].lchId;
   lchCfg->ulLchQciInfo.qci = initCtxt->erabList[idx].qci;
   
   WR_UMM_GET_CELLCB(ummCellCb, transCb->ueCb->cellId);
   if(ummCellCb == NULLP)
   {
      RLOG0(L_ERROR, " CellCb is NULL. ");
      RETVALUE(RFAILED);
   }
   params = &ummCellCb->wrRabCfgParams[initCtxt->erabList[idx].qci];
   lch           = &(params->lchParams);
   lchCfg->ulLchQciInfo.lcgId = lch->lchGrp;
#ifdef LTE_L2_MEAS
      lchCfg->lcgId = lch->lchGrp;
#endif
   /* SPS changes starts */
   if ((initCtxt->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG) && \
      (initCtxt->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stErabAcceptLst[idx].bIsDlSpsEnabled == TRUE))
   {
      /* Since there is no RBC and both RRM and APP need an enhancement. So, here there can be two approaches 
       * 1) Either assume that the ERAB sent to RRM is being received in the same order in the accepted list
       * 2) Or using a loop compare ERAB ids and find out if bIsSpsEnabled == TRUE 
       * Approach one is assumed as of now to avoid unnecessary loop. Thus accessed directly with idx */
      lchCfg->dlInfo.dlSpsCfg.isSpsEnabled = TRUE;
      initCtxt->erabList[idx].isDlSpsEnabled = TRUE;
   }
   else
   {
      lchCfg->dlInfo.dlSpsCfg.isSpsEnabled = FALSE;
      initCtxt->erabList[idx].isDlSpsEnabled = FALSE;
   }
   /* SPS changes ends */

   /*ccpu00125111: Nhu error indication handling*/ 
   initCtxt->erabList[idx].schdCfgState = WR_INIT_CTXT_SCHD_CFG_SENT;

   /* Dispatch the prepared message to scheduler                         */
   WrIfmSchdCfgUe(cfgReq, transId);

   RETVALUE(ROK);
}

/** @brief This function is responsible for sending the UE reconfiguration 
 *         request and logical channel configuration request for all the 
 *         admitted bearers to scheduler.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtCfgSchd
 *
 *         Processing steps:
 *         - if the ambr values are changed in the intial context request, then
 *           send the UE reconfiguration request
 *         - for all the ERABs that are to be setup, send the logical channel
 *           configuration request
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmInitCtxtCfgSchd
(
WrUmmTransCb                 *transCb
)
{
   WrInitCntxtSetupTransCb   *initCtxt = &transCb->u.initCtxtCb;
   U32                       idx;

   U32                       ulGbr[WR_UMM_MAX_LCG_GRP_ID] = {0,0,0,0};
   U32                       ulMbr[WR_UMM_MAX_LCG_GRP_ID] = {0,0,0,0};

   /*Convert to kBps from kbps*/
   for (idx = 0; idx < initCtxt->numErabs; idx++)
   {
      initCtxt->erabList[idx].qos.ulGbr /= 8;
      initCtxt->erabList[idx].qos.dlGbr /= 8;
      initCtxt->erabList[idx].qos.ulMbr /= 8;
      initCtxt->erabList[idx].qos.dlMbr /= 8;
   }

   /* Configure the scheduler wil update AMBR and Resource ReConfigured from
    * RRM
    */
   wrUmmInitCtxtSchdUeRecfgReq(transCb);
   /* The schdUeCfgCfmRcvd is set to false because the CQI configuration is done
    * only during the RRC Connection Reconfiguration in RRM and UE
    * recofngiuration will happen.
    */
   initCtxt->schdUeCfgCfmRcvd = FALSE;
   
   /* Configure the scheduler for SRB 2                                   */
   wrUmmInitCtxtSchdSrbLchCfgReq(transCb, WR_INIT_CTXT_SRB2_TRANS_ID);
   /* Configure the scheduler for each new RAB being created.             */
   for (idx = 0; idx < initCtxt->numErabs; idx++)
   {
      if(TRUE == initCtxt->erabList[idx].rabAllowed)
      {
         wrUmmInitCtxtSchdLchCfgReq(transCb, idx);
         U8 lcgId;
         lcgId = wrUmmGetLcgIdForQci(transCb->ueCb->cellId, initCtxt->erabList[idx].qci);
         if (WR_UMM_IS_GBR_BEARER(initCtxt->erabList[idx].qci))
         {
            ulGbr[lcgId] = initCtxt->erabList[idx].qos.ulGbr;
            ulMbr[lcgId] = initCtxt->erabList[idx].qos.ulMbr;
         }
      }
   }
 
   /* ccpu00128203 */
   /* Configure all the logical channel groups at the same time. 
    * when a bearer of a different QCI is established, only UE needs to be 
    * configured */
   wrUmmSchdUeLcgCfg(transCb->ueCb, transCb->transId, WR_UMM_LCG_GRP_ID_1, ulGbr[1], ulMbr[1]);
   wrUmmSchdUeLcgCfg(transCb->ueCb, transCb->transId, WR_UMM_LCG_GRP_ID_2, ulGbr[2], ulMbr[2]);
   wrUmmSchdUeLcgCfg(transCb->ueCb, transCb->transId, WR_UMM_LCG_GRP_ID_3, 0, 0);
   initCtxt->expctLcgCfgCfm = WR_UMM_MAX_DRB_LCG_GRP_ID;

   RETVALUE(ROK);
}

/* Removed duplicate defination of function to fix compilation
   error when LTEMAC_DRX is enabled */
/*Removed DRX flag*/
/** @brief This function is responsible for reconfiguring the UE context
 *         at the scheduler.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtDrxReCfgSchdUeRecfgReq
 *
 *         Processing steps:
 *         - allocate a new configuration request
 *         - set the configuration type to RGR_UE_CFG
 *         - fill the ue info and DRX Release info
 *         - set the configuration state to WR_UMM_RAB_SETUP_SCH_CFG_DRX
 *         - send the request to scheduler
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtDrxReCfgSchdUeRecfgReq
(
WrUmmTransCb                 *transCb
)
{
   WrInitCntxtSetupTransCb   *initCtxt = &transCb->u.initCtxtCb;
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   RmuUeRecfgRsp *rrmUeRecfgResp = initCtxt->rrmUeRecfgRspMsg->u.rrmUeRecfgResp;
   U32                       transId;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RETVALUE(RFAILED);
   }

   transId = transCb->transId | WR_INIT_CTXT_UE_TRANS_ID;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   /* Mark the transaction that UE config has been sent */
   initCtxt->state = WR_UMM_INIT_CTXT_SCH_DRX_CFG_DONE;
   wrUmmSchdFillUeDrxCfgInfo(ueRecfg, (WrDrxCfg *)&rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.stRmDrxCfg);
   
   /* Dispatch the prepared message to scheduler. There is no need for    */
   /* another funtion for LCH separately than for UE                      */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
} 
/*Removed DRX flag*/



/** @brief This function is responsible for processing the response(s) of UE 
 *         and logical channel configuration of the scheduler for this UE.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtPrcSchdCfgCfm
 *
 *         Processing steps:
 *         - fetch the sub-transaction id from the transId,
 *         - if id is WR_INIT_CTXT_UE_TRANS_ID,
 *           - if the scheduler configuration is successful,
 *             - set scheduler flag for UE configuration to 
 *               TRUE
 *         - if id is WR_INIT_CTXT_SRB2_TRANS_ID,
 *           - if the scheduler configuration is successful,
 *             - set scheduler flag for SRB2 configuration to 
 *               TRUE
 *         - else, validate the id with the number of RABs to be setup
 *           - if the scheduler configuration is successful,
 *             - set the scheduler configuration state for that RAB to 
 *               WR_INIT_CTXT_SCHD_CFM_SUCC
 *           - else, set the scheduler configuration state for that RAB to 
 *             WR_INIT_CTXT_SCHD_CFM_FAIL
 *         - after receiving scheduler response for all the LCH configurations
 *           and UE and SRB 2 configuration, set the configuration state to 
 *           WR_UMM_INIT_CTXT_SCH_CFG_DONE 
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmInitCtxtPrcSchdCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   WrInitCntxtSetupTransCb   *initCtxt = &transCb->u.initCtxtCb;
   U8                        idx;
    
   idx = transCb->msg->u.schdCfm.transId & 0x0f;
   if (idx > WR_INIT_CTXT_UE_TRANS_ID)
   {
      RLOG_ARG0(L_ERROR, DBG_TRNSID, transCb->transId,"invalid transaction id"
         "for RAB Setup");
      initCtxt->state = WR_UMM_INIT_CTXT_ERROR;
      RETVALUE(RFAILED);
   }

   if (idx == WR_INIT_CTXT_UE_TRANS_ID)
   {
      /* This is in response to UE reconfiguration for AMBR              */
      if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
      {
            initCtxt->schdUeCfgCfmRcvd = TRUE;
      } 
      else
      {
         RLOG1(L_ERROR,"Scheduler cfm status is[%d] in WR_INIT_CTXT_UE_TRANS_ID",
            transCb->msg->u.schdCfm.status);
         initCtxt->state = WR_UMM_INIT_CTXT_ERROR;
         RETVALUE(RFAILED);
      } 
      /* if all the lch cfm's are setup prior to reception of 
       * ue cfg cfm, set state to success */
      if((initCtxt->schdLchCfgCmpl == TRUE) &&
            (initCtxt->schdSrb2LchCfgCmpl == TRUE) &&
            (initCtxt->schdLcgCfgCmpl == TRUE))
      {
         initCtxt->state = WR_UMM_INIT_CTXT_SCH_CFG_DONE;
      }
      RETVALUE(ROK);
   }
   /* check if the confirm is for SRB 2                                   */
   if(idx == WR_INIT_CTXT_SRB2_TRANS_ID)
   {
      if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
      {
         initCtxt->schdSrb2LchCfgCmpl = TRUE;
      }
      else
      {
         RLOG1(L_ERROR,"Scheduler cfm status is[%d] in WR_INIT_CTXT_SRB2_TRANS_ID",
            transCb->msg->u.schdCfm.status);
         initCtxt->state = WR_UMM_INIT_CTXT_ERROR;
         RETVALUE(RFAILED);
      }

      /* if all the lch cfm's are setup prior to reception of 
       * ue cfg cfm, set state to success */
      if((initCtxt->schdLchCfgCmpl == TRUE) && 
            (initCtxt->schdUeCfgCfmRcvd == TRUE) && 
            (initCtxt->schdLcgCfgCmpl == TRUE))
      {
         initCtxt->state = WR_UMM_INIT_CTXT_SCH_CFG_DONE;
      }
      RETVALUE(ROK);
   }

   /* ccpu00128203 */
   if((idx == WR_UMM_TRANS_ID_LCG0_CFG)||(idx == WR_UMM_TRANS_ID_LCG1_CFG)\
         ||(idx == WR_UMM_TRANS_ID_LCG2_CFG)||(idx == WR_UMM_TRANS_ID_LCG3_CFG))
   {
      if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
      {
         initCtxt->numlcgCfgCfm++;
      }
      else
      {
         RLOG1(L_ERROR,"Scheduler cfm status is[%d] in WR_UMM_TRANS_LCG_CFG_ID",
            transCb->msg->u.schdCfm.status);
            initCtxt->state = WR_UMM_INIT_CTXT_ERROR;
            RETVALUE(RFAILED);
      }
      if(initCtxt->numlcgCfgCfm == initCtxt->expctLcgCfgCfm)
      {
         initCtxt->schdLcgCfgCmpl = TRUE;
         if((initCtxt->schdLchCfgCmpl == TRUE) && 
               (initCtxt->schdSrb2LchCfgCmpl == TRUE) &&
               (initCtxt->schdUeCfgCfmRcvd == TRUE))
         {
            initCtxt->state = WR_UMM_INIT_CTXT_SCH_CFG_DONE;
         }
      }
      RETVALUE(ROK);
   }

   if (idx > initCtxt->numErabs)
   {
      RLOG_ARG0(L_INFO, DBG_TRNSID,transCb->transId,
         "Invalid transaction for RAB Setup");
      initCtxt->state = WR_UMM_INIT_CTXT_ERROR;
      RETVALUE(ROK);
   }
   if (transCb->msg->u.schdCfm.status == RGR_CFG_CFM_OK)
   {
      initCtxt->erabList[idx].schdCfgState = WR_INIT_CTXT_SCHD_CFM_SUCC;
      initCtxt->numErabSucc++;
   }
   else
   {
      /* Error_Cause: Filling the error cause for Rab-Id when scheduler returns
       * failure for this
       * Changing the rabAllowed = FALSE also
       *
       */
      initCtxt->numErabFail++;
      initCtxt->erabList[idx].rabAllowed = FALSE;
      initCtxt->erabList[idx].cause.causeTyp = CAUSE_RADIONW;
      initCtxt->erabList[idx].cause.causeVal =SztCauseRadioNwradio_resources_not_availableEnum; /*Error cause could be SztCauseRadioNwfailure_in_radio_interface_procedureEnum */ 
      initCtxt->erabList[idx].schdCfgState = WR_INIT_CTXT_SCHD_CFM_FAIL;
   }
      /* only when all the lch cfm's are received */
      if((initCtxt->numErabSucc + initCtxt->numErabFail) == initCtxt->numErabs)
      {
         /* all LCH CFMs are failedi, should sent ICS Failure to MME */
         if(initCtxt->numErabSucc == 0)
         {
            initCtxt->state = WR_UMM_INIT_CTXT_ERROR;
            RETVALUE(RFAILED);
         }
         else
         {   
            initCtxt->schdLchCfgCmpl = TRUE;
         }
      }
   /* only when UE Config is done, we are setting the state to SUCCESS */
   if((initCtxt->schdUeCfgCfmRcvd == TRUE) &&
         (initCtxt->schdSrb2LchCfgCmpl == TRUE) &&
         (initCtxt->schdLcgCfgCmpl == TRUE))
   {
      initCtxt->state = WR_UMM_INIT_CTXT_SCH_CFG_DONE;
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for getting the SRB configuration
 *         filled.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtRrcReconfigAddSrbCfg
 *
 *         Processing steps:
 *         - call wrUmmRrcAddSrbCfg, with the information fromt the rab data 
 *           structure, to fill the RRC configuration data structure.
 *
 * @param[in, out] srbCfg  : RRC data structure to be filled
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtRrcReconfigAddSrbCfg
(
 U16                         cellId,
NhuRbCfgInfo                 *srbCfg
)
{
   U8                        isSrb1 = FALSE;

   if (wrUmmRrcAddSrbCfg(cellId, srbCfg, isSrb1) != ROK)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for getting the DRB configuration
 *         filled.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtRrcReconfigAddDrbCfg
 *
 *         Processing steps:
 *         - call wrUmmRrcAddDrbCfg, with the information fromt the rab data 
 *           structure, to fill the RRC configuration data structure.
 *
 * @param[in, out] drbCfg  : RRC data structure to be filled
 * @param[in]     rabInfo : RAB for which the IE is to be filled
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtRrcReconfigAddDrbCfg
(
U16                          cellId,
NhuRbCfgInfo                 *drbCfg,
WrErabInfo                   *rabInfo
)
{
   /* SPS changes starts */
   if (wrUmmRrcAddDrbCfg(cellId, drbCfg, rabInfo->drbId,
                         rabInfo->lchId, rabInfo->qci, rabInfo->isUlSpsEnabled) != ROK)
   /* SPS changes ends */
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for filling the RRC structure for
 *         configuring the lower layers.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtFillRrcCfg
 *
 *         Processing steps:
 *         - invoke wrUmmInitCtxtRrcReconfigAddSrbCfg, to fill th RRC
 *           configuration for SRB2
 *         - for the number of RABs to be setup,
 *           - if the RAB is admitted, then
 *             - invoke wrUmmInitCtxtRrcReconfigAddDrbCfg to fill the
 *               RRC configuration structure for DRBs
 *
 * @param[in] transCb : transaction information 
 * @param[in, out]     evnt    : RRC configuration
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmInitCtxtFillRrcCfg
(
WrUmmTransCb                 *transCb,
NhuDatReqSdus                *evnt
)
{
   WrInitCntxtSetupTransCb   *initCtxt = &transCb->u.initCtxtCb;
   NhuRbCfgInfo              *drbCfg;
   WrErabInfo                *rabInfo;
   NhuUeCfgInfo              *ueCfg;
   NhuRbCfgList              *rbCfgList;
   U32                       idx;
   U32                       numDrbs = 0;
   U16                       cellId = transCb->ueCb->cellId;

   evnt->sdu.isUeCfgPres = TRUE;
   ueCfg = &(evnt->sdu.ueCfg);
   ueCfg->ueCfgBitMsk = NHU_EXP_CFG_RB_CFG;
   ueCfg->rbCfgAllList.rbCfgTypBitMsk = NHU_CFG_RB_ADD;
   rbCfgList = &(ueCfg->rbCfgAllList.rbToAddCfgList);
   /* allocating memory for numErabs + SRB2                              */
   WR_GET_MEM(evnt, (sizeof(NhuRbCfgInfo) * (initCtxt->numErabsAllowed + 1)), 
                                               &(rbCfgList->rbCfgInfoList));
   /* filling SRB2 configuration                                          */
   wrUmmInitCtxtRrcReconfigAddSrbCfg(cellId, &rbCfgList->rbCfgInfoList[0]);
   /*ccpu00127802*/
   RLOG_ARG0(L_EVENT, DBG_CRNTI, transCb->ueCb->crnti,"Sending Rrc Connection"
      " ReConfiguration Message");
   /* filling other ERAB configuration                                    */
   for (idx = 0; idx < initCtxt->numErabs; idx++)
   {
      rabInfo = &initCtxt->erabList[idx];
      drbCfg  = &rbCfgList->rbCfgInfoList[numDrbs + 1];
      /* TODO Hard coding the CQI to 1 as of now */
#ifdef LTE_L2_MEAS
      drbCfg->qci = 1;
#endif

      /* We send information of only those RABs that are successful       */
      if ((rabInfo->rabAllowed == TRUE) &&
          (rabInfo->schdCfgState == WR_INIT_CTXT_SCHD_CFM_SUCC))
      {
         wrUmmInitCtxtRrcReconfigAddDrbCfg(cellId, drbCfg, rabInfo);
         numDrbs++;
      }
   }
   rbCfgList->numOfRbCfgInfo = numDrbs + 1;
   RETVALUE(ROK);
}

/** @brief This function is responsible for getting the RRC ASN SRB IE filled.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtRrcReconfigAddSrb
 *
 *         Processing steps:
 *         - invoke wrUmmRrcFillSrbIE, to fill the RRC ASN DRB IE.
 *
 * @param[in, out] srbIe   : RRC ASN SRB IE to be filled
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtRrcReconfigAddSrb
(
U16                          cellId,
NhuSRB_ToAddMod              *srbIe
)
{
   U8                        isSrb1 = FALSE;

   if (wrUmmRrcFillSrbIE(cellId, srbIe, isSrb1) != ROK)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for getting the RRC ASN DRB IE filled.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtRrcReconfigAddDrb
 *
 *         Processing steps:
 *         - invoke wrUmmRrcReconfigFillDrb, to fill the RRC ASN DRB IE.
 *
 * @param[in, out] drbIe   : RRC ASN DRB IE to be filled
 * @param[in]     rabInfo : RAB information that is to be used for filling
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtRrcReconfigAddDrb
(
 /* SPS changes starts */
 WrUeCb                       *ueCb,
 WrUmmTransCb                 *transCb,
 /* SPS changes ends */
NhuDRB_ToAddMod              *drbIe,
WrErabInfo                   *rabInfo
)
{
   /* SPS changes starts */
   if (wrUmmRrcReconfigFillDrb(ueCb, transCb->spsCfgTransCb.isUeUlSpsEnabled, drbIe, \
            rabInfo->erabId, rabInfo->drbId, rabInfo->lchId, rabInfo->qci, \
            rabInfo->isDlSpsEnabled, rabInfo->isUlSpsEnabled) != ROK)
   /* SPS changes ends */
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for preparing the sending the RRC
 *         reconfiguration message. The lower layer configurations are also 
 *         filled here.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtPrepRrcMsgAndCfg
 *
 *         Processing steps:
 *         - allocate a new RRC data request event,
 *         - fill the RRC interface header
 *         - get a new free tranaction for the reconfiguration message
 *         - invoke wrUmmRrcCreateRrcReconfigMsg, to create a new 
 *           reconfiguration message
 *         - invoke wrUmmRrcPrepRadioResourceDed, to prepare the Radio
 *           Resource Dedicated IE
 *         - invoke wrUmmInitCtxtRrcReconfigAddSrb, to fill the DRB
 *           information to the ASN IE
 *         - for every RAB that is admitted,
 *           - invoke wrUmmInitCtxtRrcReconfigAddDrb, to fill the DRB 
 *             information to the ASN IE
 *         - for every RAB that is admitted,
 *           - copy the dedicated NAS information to the ASN IE
 *         - invoke wrUmmInitCtxtFillRrcCfg to fill the lower layer 
 *           configuration for the DRB(s)
 *         - send the event to RRC
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtPrepRrcMsgAndCfg
(
WrUmmTransCb                 *transCb
)
{
   WrInitCntxtSetupTransCb                   *initCtxt = &transCb->u.initCtxtCb;
   NhuDatReqSdus                             *recfgMsg = NULLP;
   WrUeCb                                    *ueCb = transCb->ueCb;
   WrErabInfo                                *rabInfo;
   NhuDL_DCCH_Msg                            *dcchMsg;
   NhuRRCConRecfgn                           *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs                    *recfgIEs;
   NhuRadioResourceConfigDedicated           *rrcCfgDed;
   NhuDRB_ToAddMod                           *drbIe;
   NhuSRB_ToAddMod                           *srbIe;
   NhuRRCConRecfgn_r8_IEsdedicatedInfoNASLst *dedNasLst;
   NhuMAC_MainConfig                         *macCfg;
   U32                                       idx;
   U8                                        transId;
   U16                                       nasPduCount = 0;
   U16                                       numRabs = 0;
   NhuMeasConfig                             *nhuMeasCfg;
   S16                                       ret = RFAILED;

   NhuTPC_PDCCH_Config             *tpcPdcchCfgPucch = NULLP;
   NhuTPC_PDCCH_Config             *tpcPdcchCfgPusch = NULLP;
   Data                             *pucchTpcRnti =  NULLP;
   Data                             *puschTpcRnti =  NULLP;

   WR_ALLOCEVNT(&recfgMsg, sizeof(NhuDatReqSdus));
   if (recfgMsg == NULLP)
   {
      RETVALUE(RFAILED);
   }
   recfgMsg->hdr.cellId  = ueCb->cellId;
   recfgMsg->hdr.ueId    = ueCb->crnti;
   recfgMsg->hdr.transId = transCb->transId | C1_RRCCONRECFGN;

   /* Fetch an RRC transaction id for this procedure.                     */
   if(wrUmmGetRrcTransId(ueCb, C1_RRCCONRECFGN, 
         WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
         &transId)!= ROK)
   {
      RLOG_ARG0(L_ERROR, DBG_CRNTI, ueCb->crnti,"Max RRC Transaction already"
         " Reached");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }
   /* Create the reconfiguration message which is constructued excluding  */
   /* the actual procedure related details which will be added further    */
   if (wrUmmRrcCreateRrcReconfigMsg(recfgMsg, transId) != ROK)
   {
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }

   /* Prepare the radio resources to exclude all the IEs present in the   */
   /* the IE. Those IEs are filled subsequently                           */
   dcchMsg     = &(recfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   recfgIEs    = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   rrcCfgDed   = &(recfgIEs->radioResourceConfigDedicated);
   wrUmmRrcPrepRadioResourceDed(rrcCfgDed);
   WR_FILL_TKN_UINT(rrcCfgDed->physicalConfigDedicated.pres,PRSNT_NODEF);
   if (transCb->ueCb->antennaInfo.txmode ==
         NhuAntennaInfoDedicatedtransmissionModetm3Enum)
   {
      /* U8  codeBook[2] = {2, 0}; */
   NhuPhysicalConfigDedicatedantennaInfo *antennaInfo =
         &(recfgIEs->radioResourceConfigDedicated.physicalConfigDedicated.antennaInfo);
   wrFillTknU8(&antennaInfo->choice, ANTENNAINFO_EXPLICITVALUE);
   wrFillTknU8(&antennaInfo->val.explicitValue.pres, PRSNT_NODEF);
   wrFillTknU32(&antennaInfo->val.explicitValue.transmissionMode,
            ueCb->antennaInfo.txmode);   
      WR_FILL_TKN_UINT(antennaInfo->val.explicitValue.codebookSubsetRestrn.choice,\
            CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM3);
      WR_FILL_BIT_STR(antennaInfo->val.explicitValue.codebookSubsetRestrn.val.n2TxAntenna_tm3,\
            2, 3, recfgMsg); /*&(recfgMsg->memCp)); bit string 11 */
      /*      wrFillTknU8(&antennaInfo->val.explicitValue.codebookSubsetRestrn.choice,
       *      CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM3);
       *      wrFillTknStrOSXL(&(antennaInfo->val.explicitValue.codebookSubsetRestrn.val.n2TxAntenna_tm3),
       *      2, codeBook, &(recfgMsg->memCp));
       */
   wrFillTknU8(&antennaInfo->val.explicitValue.ue_TransmitAntennaSelection.choice,
               UE_TRANSMITANTENNASELECTION_RELEASE);
   }
   else if (transCb->ueCb->antennaInfo.txmode ==
         NhuAntennaInfoDedicatedtransmissionModetm4Enum)
   {
      /*U8  codeBook[2] = {2, 0}; */
      NhuPhysicalConfigDedicatedantennaInfo *antennaInfo =
         &(recfgIEs->radioResourceConfigDedicated.physicalConfigDedicated.antennaInfo);
      wrFillTknU8(&antennaInfo->choice, ANTENNAINFO_EXPLICITVALUE);
      wrFillTknU8(&antennaInfo->val.explicitValue.pres, PRSNT_NODEF);
      wrFillTknU32(&antennaInfo->val.explicitValue.transmissionMode,
            ueCb->antennaInfo.txmode);

      WR_FILL_TKN_UINT(antennaInfo->val.explicitValue.codebookSubsetRestrn.choice,\
            CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM4);
      WR_FILL_BIT_STR(antennaInfo->val.explicitValue.codebookSubsetRestrn.val.n2TxAntenna_tm4,\
            6, 63, recfgMsg); /*&(recfgMsg->memCp)); bit string 11 */
      /*      wrFillTknU8(&antennaInfo->val.explicitValue.codebookSubsetRestrn.choice,
            CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM4);
            wrFillTknStrOSXL(&(antennaInfo->val.explicitValue.codebookSubsetRestrn.val.n2TxAntenna_tm4),
            6, codeBook, &(recfgMsg->memCp));*/
      
      wrFillTknU8(&antennaInfo->val.explicitValue.ue_TransmitAntennaSelection.choice,
            UE_TRANSMITANTENNASELECTION_RELEASE);
   }

   /* CQI is configured only during the RRC connection reconfiguration in RRM
    * Hence it is sent even if the Txn Mode is not changed 
    */
   /* SPS changes starts */
   wrUmmRrcFillUeRadioResDedInfo(&recfgMsg->memCp, ueCb, &transCb->spsCfgTransCb.spsCfg, rrcCfgDed,
         (initCtxt->rrmUeRecfgRspMsg->u.rrmUeRecfgResp));
   /* SPS handle return failure due to SPS function */
   /* SPS changes ends */

   /*PDCCH-PUCCH Configuration */
   tpcPdcchCfgPucch = &(rrcCfgDed->physicalConfigDedicated.tpc_PDCCH_ConfigPUCCH);
   tpcPdcchCfgPucch->choice.pres = NOTPRSNT;
   if (ueCb->uePucchPwr.pres)
   {
      WR_FILL_TKN_UINT(tpcPdcchCfgPucch->choice, TPC_PDCCH_CONFIG_SETUP);
      WR_FILL_TKN_UINT(tpcPdcchCfgPucch->val.setup.pres, PRSNT_NODEF);       
      WR_ALLOC(&pucchTpcRnti, 2);
         if (NULLP == pucchTpcRnti)
         {
            RLOG0(L_FATAL, "Memory allocation failed.");
            WR_FREEEVNT(recfgMsg);
            RETVALUE(RFAILED);
         }

      pucchTpcRnti[1] = (U8)(0x00ff & ueCb->uePucchPwr.tpcRnti);
      pucchTpcRnti[0] = (U8)(0x00ff & (ueCb->uePucchPwr.tpcRnti>>8));

      WR_FILL_TKN_STR_UINT((&tpcPdcchCfgPucch->val.setup.tpc_RNTI),pucchTpcRnti , 16);
      if (ueCb->uePucchPwr.isFmt3a)
      {
         WR_FILL_TKN_UINT(tpcPdcchCfgPucch->val.setup.tpc_Idx.choice,1);
         WR_FILL_TKN_UINT(tpcPdcchCfgPucch->val.setup.tpc_Idx.val.indexOfFormat3A, ueCb->uePucchPwr.idx);          
      }
      else
      {
         WR_FILL_TKN_UINT(tpcPdcchCfgPucch->val.setup.tpc_Idx.choice,0);
         WR_FILL_TKN_UINT(tpcPdcchCfgPucch->val.setup.tpc_Idx.val.indexOfFormat3, ueCb->uePucchPwr.idx);          
      }       
   }
   else
   {
      tpcPdcchCfgPucch->choice.pres = NOTPRSNT;
   }

   /* PDCCH-PUSCH Configuration */
   tpcPdcchCfgPusch = &(rrcCfgDed->physicalConfigDedicated.tpc_PDCCH_ConfigPUSCH);
   tpcPdcchCfgPusch->choice.pres = PRSNT_NODEF; /* VSP */
   if (ueCb->uePuschPwr.pres)
   {
      WR_FILL_TKN_UINT(tpcPdcchCfgPusch->choice, TPC_PDCCH_CONFIG_SETUP);
      WR_FILL_TKN_UINT(tpcPdcchCfgPusch->val.setup.pres, PRSNT_NODEF);       
      WR_ALLOC(&puschTpcRnti, 2);
      if (NULLP == puschTpcRnti)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         RETVALUE(RFAILED);
      }/* End of if condition.*/

      puschTpcRnti[1] = (U8)(0x00ff & ueCb->uePuschPwr.tpcRnti);
      puschTpcRnti[0] = (U8)(0x00ff & (ueCb->uePuschPwr.tpcRnti>>8));

      WR_FILL_TKN_STR_UINT((&tpcPdcchCfgPusch->val.setup.tpc_RNTI), puschTpcRnti, 16);
      if (ueCb->uePuschPwr.isFmt3a)
      {
         WR_FILL_TKN_UINT(tpcPdcchCfgPusch->val.setup.tpc_Idx.choice,1);
         WR_FILL_TKN_UINT(tpcPdcchCfgPusch->val.setup.tpc_Idx.val.indexOfFormat3A, ueCb->uePuschPwr.idx);          
      }
      else
      {  
         WR_FILL_TKN_UINT(tpcPdcchCfgPusch->val.setup.tpc_Idx.choice,0);
         WR_FILL_TKN_UINT(tpcPdcchCfgPusch->val.setup.tpc_Idx.val.indexOfFormat3, ueCb->uePuschPwr.idx);          
      }   
   }
   else
   {
      tpcPdcchCfgPusch->choice.pres = NOTPRSNT;
   }

   /* adding SRB 2 configuration */
   if (initCtxt->schdSrb2LchCfgCmpl == FALSE)
   {
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }

   /* Allocate memory to include each RAB into the message towards the UE */
   wrFillTknU16(&rrcCfgDed->srb_ToAddModLst.noComp, 1);
   WR_GET_MEM(recfgMsg, (sizeof(NhuSRB_ToAddMod) * 1), 
         &(rrcCfgDed->srb_ToAddModLst.member));

   if (rrcCfgDed->srb_ToAddModLst.member == NULLP)
   {
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }
   srbIe   = rrcCfgDed->srb_ToAddModLst.member;
   wrUmmInitCtxtRrcReconfigAddSrb(ueCb->cellId, srbIe);
   /*ccpu00125111: Send only the Number of ERABs allowed. */
   /* Allocate memory to include each RAB into the message towards the UE */
   WR_GET_MEM(recfgMsg, (sizeof(NhuDRB_ToAddMod) * initCtxt->numErabsAllowed), 
         &(rrcCfgDed->drb_ToAddModLst.member));
   if (rrcCfgDed->drb_ToAddModLst.member == NULLP)
   {
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }

   for (idx = 0; idx < initCtxt->numErabs; idx++)
   {
      rabInfo = &initCtxt->erabList[idx];

      /* We send information of only those RABs that are successful       */
      if (rabInfo->rabAllowed == TRUE) 
      {
         drbIe   = rrcCfgDed->drb_ToAddModLst.member + numRabs;
         /* SPS changes starts */
         wrUmmInitCtxtRrcReconfigAddDrb(ueCb, transCb, drbIe, rabInfo);
         /* SPS changes ends */
         numRabs++;
         /*Update Statistics for Default ERAB : SuccessCase*/
         wrEmmUpdateStat(WR_EMM_ERAB_STATS,transCb->ueCb->cellId,WR_EMM_ERAB_DEFAULT,WR_EMM_DFLT_POS_STEP,WR_EMM_ERAB_SUCCESS);
         if(rabInfo->nasPdu != NULLP)
         {
            nasPduCount++;
         }
      }
   }
   wrFillTknU16(&rrcCfgDed->drb_ToAddModLst.noComp, numRabs);
   
   macCfg = &(rrcCfgDed->mac_MainConfig.val.explicitValue);
   wrFillTknU8(&(rrcCfgDed->mac_MainConfig.choice), 0);
   wrUmmRrcFillMacCfgIe(macCfg, ueCb);

   if(nasPduCount)
   {
      dedNasLst = &(recfgIEs->dedicatedInfoNASLst);
      /* Allocate memory to include NAS PDUs into the message towards the UE */
      WR_GET_MEM(recfgMsg, (sizeof(NhuDedicatedInfoNAS) * nasPduCount),
            &(dedNasLst->member));
      if (dedNasLst->member == NULLP)
      {
         WR_FREEEVNT(recfgMsg);
         RETVALUE(RFAILED);
      }

      for (idx = 0, nasPduCount = 0; idx < initCtxt->numErabs; idx++)
      {
         rabInfo = &initCtxt->erabList[idx];

         /* We send information of only those RABs that are successful       */
         if ((rabInfo->rabAllowed == TRUE) && (rabInfo->nasPdu != NULLP))
         {
            wrFillTknStrOSXL(&(dedNasLst->member[nasPduCount]), rabInfo->nasPdu->len, 
                     rabInfo->nasPdu->val, &(recfgMsg->memCp));
            nasPduCount++;
         }
      }
      wrFillTknU16(&dedNasLst->noComp, nasPduCount);
   }

   /* Fill Measurement COnfiguration IE                                   */
   nhuMeasCfg  = &(recfgIEs->measConfig);

   if(initCtxt->csFallBackInd.pres == TRUE)
   {
      ret = wrUmmMeasInitialUeCfg (&recfgMsg->memCp, ueCb, 
            &initCtxt->measTransCb, nhuMeasCfg, transCb->transId,WR_MEAS_CONFIG_NO_EVT_ANR);
   }
   else
   {
      ret = wrUmmMeasInitialUeCfg (&recfgMsg->memCp, ueCb, 
            &initCtxt->measTransCb, nhuMeasCfg, transCb->transId, WR_MEAS_CONFIG_EVT_ANR);
   }
   if(ROK != ret)
   {
      RLOG0(L_ERROR, "MeasCfg failed.");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }

   /* Build the configuration towards RRC                                 */
   wrUmmInitCtxtFillRrcCfg(transCb, recfgMsg);
   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending RRCConnectionReconfiguration"
         " for Initial Context");
   if (RFAILED == wrIfmRrcSndReqToUe(recfgMsg))
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe failed.");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }
   /* Start RRC Reconfigure timer */
   wrStartTransTmr(transCb,WR_TMR_RECONFIGURE, wrEmmGetRrcReCfgTmrVal(ueCb->cellId));

   RETVALUE(ROK);
}

/** @brief This function is responsible for verifying multiple instances in the received message.
 *
 * @details
 *
 *     Function: wrUmmChkRabIdInInitialCtxtSetupLst
 *
 *         Processing steps:
 *         Traverse the rab list. 
 *         if the E_RAB_ID is matching with then update the rabSetup cause value. 
 *
 * @param[in, out] rabLst : rab List and rabSetup  
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmChkRabIdInInitialCtxtSetupLst
(
SztE_RABToBeSetupLstCtxtSUReq  *rabLst,
WrErabInfo                    *rabSetup,
U8                             idx, 
U8                             *indx
)
{
  U32                          eRabId;
  U32                          numRabs;
  U8                           dupFlag = FALSE;
  U8                           eRabIdx = 0;
  SztE_RABToBeSetupItemCtxtSUReq *rabInfo = NULL;

  numRabs = rabLst->noComp.val;
  eRabId = rabLst->member[idx].value.u.sztE_RABToBeSetupItemCtxtSUReq.e_RAB_ID.val;
  eRabIdx = (idx + 1);
  
  /* 
   * Traverse the list to identify the multiple bearer IDs
   */
  while(eRabIdx < numRabs)
  {
    rabInfo = &rabLst->member[eRabIdx].value.u.sztE_RABToBeSetupItemCtxtSUReq;
    if(eRabId == rabInfo->e_RAB_ID.val)
    {
      rabInfo->pres.pres = NOTPRSNT;
      dupFlag = TRUE;
    }
    eRabIdx++;
  }
  if(dupFlag)
  {
    /* Detected Multiple instances of ERAB ID */
    rabLst->member[idx].value.u.sztE_RABToBeSetupItemCtxtSUReq.pres.pres = NOTPRSNT;
    rabSetup[*indx].rabAllowed     = FALSE;
    rabSetup[*indx].cause.causeTyp = CAUSE_RADIONW;
    rabSetup[*indx].cause.causeVal = SztCauseRadioNwmultiple_E_RAB_ID_instancesEnum;
    (*indx)++;
    RETVALUE(RFAILED);
  }
  RETVALUE(ROK);
}/* End of wrUmmChkRabIdInInitialCtxtSetupLst*/


/** @brief This function is responsible for traversing the RABs to be setup
 *         list in the Initial Context Setup Request message and populating 
 *         the internal data structure.
 *
 * @details
 *
 *     Function: wrUmmGetErabInfoFrmIntCnxt
 *
 *         Processing steps:
 *         - for the number of ERAB information in the message, 
 *           - copy the QoS and tunnel information to the internal data 
 *             structure.
 *
 * @param[in, out]  initCtxtCb : initial context setup transaction information
 * @param[in]      s1ErabLst  : RAB Setup list present in the Setup message
 * @return S16
 *          -# Success: ROK
 *          -# Failure: RFAILED
 */
PRIVATE S16 wrUmmGetErabInfoFrmIntCnxt
(
WrInitCntxtSetupTransCb           *initCtxtCb,
SztE_RABToBeSetupLstCtxtSUReq     *s1ErabLst
)
{
   U8                             rbIdx = 0;
   U8                             shiftBits = 0;
   U8                             indx = 0;
   U8                             idx = 0;
   U32                            addrMask = 0;
   SztGTP_TEID                    *remTeIdStr = NULLP;
   SztE_RABToBeSetupItemCtxtSUReq *erabItem = NULLP;
   SztAllocnAndRetentionPriority  *qosAllcRetn = NULLP;
   WrErabInfo                     *erabInfo = NULLP;
   U8                             numErabs = 0;
   SztGBR_QosInform               *gbrIe = NULLP;
   S16                            ret = RFAILED;

   numErabs = s1ErabLst->noComp.val;
   WR_ALLOC(&erabInfo, (numErabs * sizeof(WrErabInfo)));
   if (erabInfo == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }
   for(idx = 0; idx < numErabs; idx++)
   {
      erabItem = 
         &s1ErabLst->member[idx].value.u.sztE_RABToBeSetupItemCtxtSUReq;

      /* Fill the parameters that are required to validate in the ERAB */
      erabInfo[rbIdx].erabId        = erabItem->e_RAB_ID.val;
      erabInfo[rbIdx].qci           = erabItem->e_RABlevelQoSParams.qCI.val;
      WR_INC_KPI_FUNC1(wrIncKpiFrERABEstbInitAttPrQci,erabItem->e_RABlevelQoSParams.qCI.val);

      if((erabItem->pres.pres == NOTPRSNT) || 
          (wrUmmChkRabIdInInitialCtxtSetupLst(s1ErabLst,erabInfo, idx, &rbIdx) != ROK))
      {
          initCtxtCb->failCause = SztCauseRadioNwmultiple_E_RAB_ID_instancesEnum;
          continue;
      }

      /*Fix For ccpu00123304*/
      if( erabInfo[rbIdx].erabId < WR_UMM_ERAB_OFFSET || erabInfo[rbIdx].erabId > WR_UMM_MAX_ERABID)
      {
         /* Error_Cause:Changes */
         erabInfo[rbIdx].cause.causeTyp = CAUSE_RADIONW;
         erabInfo[rbIdx].cause.causeVal = SztCauseRadioNwunknown_E_RAB_IDEnum;
         erabInfo[rbIdx].rabAllowed = FALSE; 
         initCtxtCb->numErabFail++;
         rbIdx++;
         continue;
      }

      if ((erabItem->e_RABlevelQoSParams.gbrQosInform.pres.pres) && ( erabInfo[rbIdx].qci <= 4))
      {
         erabInfo[rbIdx].qos.isGbrQosPres = TRUE;
         gbrIe = &(erabItem->e_RABlevelQoSParams.gbrQosInform);
         wrGetBitRate(&gbrIe->e_RAB_MaxBitrateDL, &erabInfo[rbIdx].qos.dlMbr);
         wrGetBitRate(&gbrIe->e_RAB_MaxBitrateUL, &erabInfo[rbIdx].qos.ulMbr);
         wrGetBitRate(&gbrIe->e_RAB_GuaranteedBitrateDL, 
                                                  &erabInfo[rbIdx].qos.dlGbr);
         wrGetBitRate(&gbrIe->e_RAB_GuaranteedBitrateUL,
                                                  &erabInfo[rbIdx].qos.ulGbr);
      }
      /* ccpu00125111: Support for handling GBR bearers */
      else if( erabInfo[rbIdx].qci <= 4)
      {
         erabInfo[rbIdx].cause.causeTyp = CAUSE_RADIONW;
         erabInfo[rbIdx].cause.causeVal = SztCauseRadioNwinvalid_qos_combinationEnum;
         erabInfo[rbIdx].rabAllowed = FALSE;
         initCtxtCb->numErabFail++;
         rbIdx++;
         continue;
      }
      else
      {
         erabInfo[rbIdx].qos.isGbrQosPres = FALSE;
         erabInfo[rbIdx].qos.dlMbr = 0;
         erabInfo[rbIdx].qos.ulMbr = 0;
         erabInfo[rbIdx].qos.dlGbr = 0;
         erabInfo[rbIdx].qos.ulGbr = 0;
      }

      /* Successfully validated now fill the erabInfo paramters */
      qosAllcRetn = &erabItem->e_RABlevelQoSParams.allocationRetentionPriority;
      erabInfo[rbIdx].qos.priority  = qosAllcRetn->priorityLvl.val;
      erabInfo[rbIdx].qos.preempCap = qosAllcRetn->pre_emptionCapblty.val;
      erabInfo[rbIdx].qos.preempVul = qosAllcRetn->pre_emptionVulnerability.val;

      /* Store the remote tunnel information */
      /* Retrieve the remote S-GW's EGTP-U tunnel Id */
      remTeIdStr = &(erabItem->gTP_TEID);
      shiftBits = (remTeIdStr->len);
      addrMask = 0xFF000000;
      for(indx = 0;indx < remTeIdStr->len;indx++)
      {
         shiftBits--;
         erabInfo[rbIdx].remTeId |= 
            ((U32)(remTeIdStr->val[indx] << (8*shiftBits)) & addrMask);
         addrMask = addrMask>>8;
      }

      /* store the SGW IP Address */
      switch(erabItem->transportLyrAddr.len)
      {
         case 32:
         {
            erabInfo[rbIdx].sgwAddr.type = CM_TPTADDR_IPV4;
            erabInfo[rbIdx].sgwAddr.u.ipv4TptAddr.port = WR_DFLT_EGTP_PORT; 

            /* copy 4bytes into the U32 */
            shiftBits = erabItem->transportLyrAddr.len / 8;
            addrMask = 0xFF000000;
            for(indx = 0; indx < 4; indx++)
            {
               shiftBits--;
               erabInfo[rbIdx].sgwAddr.u.ipv4TptAddr.address |=
                  ((U32)(erabItem->transportLyrAddr.val[indx] 
                     << (8 * shiftBits)) & addrMask);
               addrMask = addrMask >> 8;
            }
            break;
         }
         case 128:
         {
            erabInfo[rbIdx].sgwAddr.type = CM_TPTADDR_IPV6;
            erabInfo[rbIdx].sgwAddr.u.ipv6TptAddr.port = WR_DFLT_EGTP_PORT;
            for (indx = 0; indx < 16; indx++)
            {
               erabInfo[rbIdx].sgwAddr.u.ipv6TptAddr.ipv6NetAddr[indx]=
                  erabItem->transportLyrAddr.val[indx];
            }
            break;
         }
         default:
         {
            /* This means both IPv4 and IPv6 addresses are present. We are */
            /* yet to support this option                                  */
            RETVALUE(RFAILED);  
         }
      }

      /* store the NAS PDU */
      if(erabItem->nAS_PDU.pres == PRSNT_NODEF)
      {
         erabInfo[rbIdx].nasPdu = &(erabItem->nAS_PDU);
      }
      else
      {
         erabInfo[rbIdx].nasPdu = NULLP;
      }

      erabInfo[rbIdx].rabAllowed = TRUE;

    #ifdef WR_RSYS_KPI 
      SGetEpcTime(&(erabInfo[rbIdx].earbSetupReqTm));
    #endif /*WR_RSYS_KPI*/
      
      /*ccpu00127706: increment the index */
      rbIdx++;
      ret = ROK;
   }
   /* update the num of ERABs and erab info in transCb                    */
   /*ccpu00127706: remove the count of multiple bearers with same bId */
   initCtxtCb->numErabs  = rbIdx;
   initCtxtCb->erabList  = erabInfo;

   RETVALUE(ret);
}


/** @brief This function is responsible for processing the received UE Radio
 *         capability information.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtPrcUeRadioCap
 *
 *         Processing steps:
 *         - invoke wrUmmRrcDecReqHdl to request RRC to decode the UE Radio
 *           capability IE
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtPrcUeRadioCap
(
WrUmmTransCb                 *transCb
)
{
   WrInitCntxtSetupTransCb   *initCtxt = &(transCb->u.initCtxtCb);

   if (wrUmmRrcDecReqHdl(transCb, initCtxt->ueRadCapIe, NHU_MSG_UERACAPINFO) 
                                                                       != ROK)
   {
      initCtxt->state = WR_UMM_INIT_CTXT_ERROR;
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for parsing the Initial Context Setup
 *         Request message and store the information into the internal data
 *         structure.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtPrcSetup
 *
 *         Processing steps:
 *         - for the number of IEs present in the message
 *           - parse the IE and store the information into the internal data
 *             structure
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtPrcSetup
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                                *ueCb = transCb->ueCb;
   U16                                   cnt;
   Bool                                  ret = TRUE;
   Bool                                  ret1 = FALSE;
   S1apPdu                               *pdu;
   WrInitCntxtSetupTransCb               *initCtxtCb;
   SztInitiatingMsg                      *initMsg;
   SztProtIE_Field_InitCntxtSetupRqstIEs *ie = NULLP;
   SztProtIE_Cont_InitCntxtSetupRqstIEs  *protIes = NULLP;
   SztE_RABToBeSetupLstCtxtSUReq         *s1ErabLst = NULLP;
   /* CSG_DEV start */
   SztCSGMembershipStatus                *csgMemberShipStatus;
   WrUmmCellCb                           *cellCb;
   /* CSG_DEV end */
   U8                                    accessMode;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if(cellCb == NULLP)
   {
      RLOG1(L_ERROR, " Error in getting CELL CB for cellId = %lu ",
            (U32)ueCb->cellId);
      RETVALUE(RFAILED);
   }

   if (transCb->msg->msgTyp == WR_UMM_S1_CON_CFM)
   {
      pdu = transCb->msg->u.cfmPdu->pdu; 
      ueCb->s1ConCb->spConnId = transCb->msg->u.cfmPdu->spConnId;
   }
   else
   {
      pdu = transCb->msg->u.s1Pdu->pdu;
   }

   initCtxtCb = &(transCb->u.initCtxtCb);

   /* store a copy of the s1 message */
   transCb->msg->refCnt++;
   initCtxtCb->msg = transCb->msg;

   /* HCSG_DEV */
   accessMode = wrEmmGetAccessModeFrmCellId(transCb->ueCb->cellId);
   if(WR_ENB_HYBRID_ACCESS == accessMode)
   {
      initCtxtCb->ueMbrShip = WR_UMM_CSG_NOT_MEMBER;
   }
   else
   {
      initCtxtCb->ueMbrShip = WR_UMM_CSG_MEMBER;
   }

   /*ccpu00127802*/
   RLOG0(L_EVENT, "************************************************************");
   RLOG_ARG0(L_EVENT, DBG_CRNTI,ueCb->crnti,"Initial Context Setup Message "
         "Received");
   RLOG0(L_EVENT, "************************************************************");
#ifdef WR_RSYS_KPI 
      if(!ueCb->s1LogicalConnEst)
      {
        WR_INC_KPI_FUNC(wrIncKpiFrS1SIGConnEstabSucc);
        ueCb->s1LogicalConnEst = TRUE;
      }
 #endif/*WR_RSYS_KPI*/
   /* Parse and process the received IEs */
   initMsg = &(pdu->pdu.val.initiatingMsg);
   protIes = &initMsg->value.u.sztInitCntxtSetupRqst.protocolIEs;

   for(cnt = 0; cnt < WR_MAX_DRBS; cnt++)
   {
      ueCb->rbInfo.rab[cnt].inUse = FALSE;
   }

   for (cnt = 0; cnt < protIes->noComp.val; cnt++)
   {
      ie = &(protIes->member[cnt]);
      switch(ie->id.val) 
      {
         case Sztid_MME_UE_S1AP_ID:
            {
               ueCb->s1ConCb->mme_ue_s1ap_id = ie->value.u.sztid_MME_UE_S1AP_ID.val;
               break;
            }
         case Sztid_eNB_UE_S1AP_ID:
            {
               ueCb->s1ConCb->enb_ue_s1ap_id = ie->value.u.sztENB_UE_S1AP_ID.val;
               break;
            }
         case Sztid_uEaggregateMaxBitrate:
            {
               SztUEAggMaxBitrate *ambr = NULLP;
               ambr = &ie->value.u.sztUEAggMaxBitrate;
               wrGetBitRate(&ambr->uEaggregateMaxBitRateDL, &initCtxtCb->dlAmbr);
               wrGetBitRate(&ambr->uEaggregateMaxBitRateUL, &initCtxtCb->ulAmbr);

            if((0 == initCtxtCb->dlAmbr) && (0 == initCtxtCb->ulAmbr))
            {
               initCtxtCb->failCauseType = WR_CAUSE_PROTOCOL;
               initCtxtCb->failCause = SztCauseProtsemantic_errorEnum;
               RLOG2(L_ERROR,
                     "Invlaid AMBR value received, ueIdx = [%d], CRNTI = [%d]",
                    ueCb->ueIdx, ueCb->crnti);
               RETVALUE(RFAILED);
            }
             break;
           }
         case Sztid_E_RABToBeSetupLstCtxtSUReq:
            {
               s1ErabLst = &ie->value.u.sztE_RABToBeSetupLstCtxtSUReq;
               if (wrUmmGetErabInfoFrmIntCnxt(initCtxtCb, s1ErabLst) != ROK)
               {
                  RLOG0(L_ERROR, "processing ERABs failed");
                  RETVALUE(RFAILED);
               }

               /*Update Statistics for Default ERAB*/
               wrEmmUpdateStat(WR_EMM_ERAB_STATS,ueCb->cellId,WR_EMM_ERAB_DEFAULT,s1ErabLst->noComp.val,WR_EMM_ERAB_ATTEMPT);

               break;
            }
         case Sztid_UESecurCapabilities:
            {
               SztUESecurCapabilities *ueSecCap;
               WR_ALLOC(&(initCtxtCb->secCfg), sizeof(WrUeSecCfg));
               if (initCtxtCb->secCfg == NULLP)
               {
                  RLOG0(L_FATAL, "Memory allocation failed.");
                  RETVALUE(RFAILED);
               }
               ueSecCap = &(ie->value.u.sztUESecurCapabilities);
               if (wrUmmSecGetSecCap(ueSecCap, initCtxtCb->secCfg) != ROK)
               {
                  RLOG0(L_ERROR, "processing secIE failed");
                  /*ccpu00126907*/
                  initCtxtCb->failCause = 
                     SztCauseRadioNwencryption_and_or_integrity_protection_algorithms_not_supportedEnum;
                  WR_FREE((initCtxtCb->secCfg), sizeof(WrUeSecCfg));
                  RETVALUE(RFAILED);
               }
               /* store the UE capability info */
               /* ccpu00130881 */
               WR_MEM_COPY(&initCtxtCb->secCfg->encrypCap, &ueSecCap->encryptionAlgorithms, 
                     sizeof(TknBStr32));
               WR_MEM_COPY(&initCtxtCb->secCfg->encrypCap, &ueSecCap->integrityProtectionAlgorithms, 
                     sizeof(TknBStr32));
               break;
            }
         case Sztid_SecurKey:
            {
#ifndef TENB_AS_SECURITY
               wrUmmSecExtractSecKeys(initCtxtCb->secCfg, 
                     &(ie->value.u.sztSecurKey)); 
#else
               /* ccpu00126532 */
               /* Passing ueCb instead of initCtxtCb as it is used at CL to
                * create UE specific  context */	
#if  !(defined SS_RBUF) && !(defined LTE_QCOM)
               wrPlatSecExtractSecKeys(initCtxtCb->secCfg,
                     &(ie->value.u.sztSecurKey), &initCtxtCb->asKeyGenerated,
                     transCb->transId); 
#else
               /*Derive cp/up ciph and intg keys to be used for UE*/
               wrUmmSecExtractSecKeys3(initCtxtCb->secCfg,
                     &(ie->value.u.sztSecurKey), &initCtxtCb->asKeyGenerated,
                     transCb);

               /* Horizontal Key Derivation */
               WR_ALLOC(&(initCtxtCb->hzSecCfg), sizeof(WrUeSecCfg));
               /*Derive  HZ KENB* */
               wrUmmSecExtractHzSecKeys3(initCtxtCb,ueCb->cellId,CTF_KDF_TYPE2);
               /*Derive cp/up ciph and intg keys for HZ KENB* */
               wrUmmSecExtractHzSecKeys3(initCtxtCb,ueCb->cellId,CTF_KDF_TYPE3);
#endif //SS_RBUF
#endif
               break;
            }
         case Sztid_UERadioCapblty:
            {
               SztUERadioCapblty *ueRadCap;
               ueRadCap = &ie->value.u.sztUERadioCapblty;
               if(ueRadCap->pres)
               {
                  initCtxtCb->ueRadCapIe = ueRadCap;
               }
               else
               {
                  initCtxtCb->ueRadCapIe = NULLP;
               }
               break;
            }
         case Sztid_CSFallbackIndicator:
            {
               SztCSFallbackIndicator *csFallbackIndIe;
               csFallbackIndIe = &ie->value.u.sztCSFallbackIndicator;
               if(csFallbackIndIe->pres)
               {
                  /* Store the CS Fallback Indicator */
                  wrFillTknU32(&(initCtxtCb->csFallBackInd), csFallbackIndIe->val);
               if(csFallbackIndIe->val == SztCSFallbackIndicatorcs_fallback_high_priorityEnum)
               {
                  ueCb->ueServTyp = WR_UMM_EMERGENCY_SERV;
               }
               }
               break;
            }
         case Sztid_CSGMembershipStatus:
            {
               csgMemberShipStatus   = &ie->value.u.sztCSGMembershipStatus;
               /* HCSG_DEV - start */
               if(WR_ENB_HYBRID_ACCESS != accessMode)
               {
                  RLOG2(L_ERROR, "CSG Membership Status IE received"
                        " while operating in Access Mode (%d) UE (%d)",
                        accessMode, ueCb->crnti);
               }

               if(SztCSGMembershipStatusnot_memberEnum ==
                     csgMemberShipStatus->val)
               {
                  initCtxtCb->ueMbrShip = WR_UMM_CSG_NOT_MEMBER;
                  RLOG1(L_INFO, "CSG non-member UE (%d)", ueCb->crnti);
               }
               else
               {
                  initCtxtCb->ueMbrShip = WR_UMM_CSG_MEMBER;
                  RLOG1(L_INFO, "CSG member UE (%d)", ueCb->crnti);
               }
               /* HCSG_DEV - end */
            }/* Case Sztid_CSGMembershipStatus*/ 
            break;
         case Sztid_RegisteredLAI:
            {
               SztLAI             *lai;
               lai = &ie->value.u.sztLAI;
            if (lai->pres.pres == PRSNT_NODEF)
            {
               ueCb->regLai.isLaiPres = TRUE;
               wrUtlGetPlmnId(&(ueCb->regLai.plmnId), &(lai->pLMNidentity));
               WR_GET_U32_FRM_OSXL(ueCb->regLai.lac, lai->lAC);
               initCtxtCb->updLai = TRUE;
            }
            else
            {
               ueCb->regLai.isLaiPres = FALSE;
            }
               break;
            }
         case Sztid_GUMMEI_ID:
            {
               SztGUMMEI        *gummeiIe = NULLP;
               gummeiIe = &ie->value.u.sztGUMMEI;
               initCtxtCb->gummei.pres = TRUE;
               wrUtlGetPlmnId(&initCtxtCb->gummei.plmnId,
                     &gummeiIe->pLMN_Identity);

               WR_MEM_COPY(&initCtxtCb->gummei.mmeGrpId,\
                     &gummeiIe->mME_Group_ID.val,\
                     gummeiIe->mME_Group_ID.len);

               WR_MEM_COPY(&initCtxtCb->gummei.mmec, gummeiIe->mME_Code.val,\
                     gummeiIe->mME_Code.len);

               RLOG_ARG2(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,
                     "Source MME information mmec [%d] mmeGrpId [%d]",
                     initCtxtCb->gummei.mmec, initCtxtCb->gummei.mmeGrpId);
               break;
            }
         case Sztid_MME_UE_S1AP_ID_2:
            {
               wrFillTknU32(&initCtxtCb->mmeUeS1apId2,
                     ie->value.u.sztid_MME_UE_S1AP_ID_2.val);
               RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,
                     "MME UE S1AP ID 2 :id [%ld]", initCtxtCb->mmeUeS1apId2.val);
               break;
            }
            /*MME_LDBAL*/
         case Sztid_HovrRestrnLst:
            {   
               SztHovrRestrnLst *sztHovrRestrnLst;

               sztHovrRestrnLst =  &ie->value.u.sztHovrRestrnLst;
               wrUmmCpyHoRestrnLst(transCb->ueCb,sztHovrRestrnLst);
               break;
            }

         /* Store the MDT allowed value in the UECB and the same will be sent 
          * to target eNodeB in case of X2 Based handover. 
          */
         case Sztid_ManagementBasedMDTAllowed:
         {
            ueCb->mgmtBasedMDTAllowed.pres = TRUE;
            ueCb->mgmtBasedMDTAllowed.val = 
                             ie->value.u.sztManagementBasedMDTAllowed.val;
         }
         break;

         case Sztid_SubscriberProfileIDforRFP:
         {
            ueCb->subscProfileId.pres = TRUE;
            ueCb->subscProfileId.val = 
                   ie->value.u.sztSubscriberProfileIDforRFP.val;
         }
         break;

         case Sztid_SRVCCOperationPossible:
         {
            if((ie->value.u.sztSRVCCOperationPossible.pres == TRUE) &&
               (ie->value.u.sztSRVCCOperationPossible.val == 
                SztSRVCCOperationPossiblepossibleEnum))
            {
               ueCb->srvccOperPossible = TRUE;
            }
         }
         break;

         default:
            break;
      }
   }

   RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Received INITIAL CONTEXT SETUP "
             "REQUEST [MME-UE-S1AP-ID:%lu] [ENB-UE-S1AP-ID:%lu]",
	         ueCb->s1ConCb->mme_ue_s1ap_id,ueCb->s1ConCb->enb_ue_s1ap_id);

   RLOG_ARG4(L_EVENT, DBG_CRNTI,ueCb->crnti,"Received INITIAL CONTEXT SETUP "
         "REQUEST, MME-UE-S1AP-ID[%lu] ENB-UE-S1AP-ID[%lu] UE UL-AMBR[%d] "
         "UE DL-AMBR[%d]", ueCb->s1ConCb->mme_ue_s1ap_id,
         ueCb->s1ConCb->enb_ue_s1ap_id, initCtxtCb->dlAmbr, initCtxtCb->ulAmbr);

   if((NULLP != ueCb->hoRstrctInfo) && (PRSNT_NODEF == 
            initCtxtCb->csFallBackInd.pres))
   {
      if(PRSNT_NODEF == ueCb->hoRstrctInfo->frbdnInterRATs.pres)
      {
         ret1 = wrChkAllRatsForbidden(ueCb);
      }
      if(TRUE == initCtxtCb->updLai)
      {
         ret = wrValidateRegLai(ueCb->hoRstrctInfo,ueCb->regLai);
      }
      if((FALSE == ret) || (TRUE == ret1))
      {
         initCtxtCb->failCause = 
            SztCauseRadioNwho_target_not_allowedEnum;
         /*Increment CSFB Attempt Counter*/
         WR_INC_KPI(KPI_ID_LTE_CSFB_ATT_SUM,KPI_VALUE_TYPE_INTEGER,
               INC_KPI_VALUE_BY_ONE);
         /*CSFB Failure KPI*/
         WR_INC_KPI(KPI_ID_LTE_CSFB_FAIL_SUM,
               KPI_VALUE_TYPE_INTEGER, INC_KPI_VALUE_BY_ONE);
         RETVALUE(RFAILED);
      }
   }

   ueCb->s1ConCb->s1apConnState = WR_S1AP_CONNECTED;
   RETVALUE(ROK);
}

/** @brief This function is responsible for filling and sending the tunnel 
 *         creation request to DAM.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtCfgTnls
 *
 *         Processing steps:
 *         - for every admitted RAB,
 *           - fill the tunnel information, 
 *             Source address, Local tunnel id
 *             Destination address, Remote tunnel id
 *             DSCP,
 *           - set the tunnel configuration state for this RAB to 
 *             WR_INIT_CTXT_TNL_SETUP_SENT
 *           - send the tunnel create requeset to DAM 
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmInitCtxtCfgTnls
(
WrUmmTransCb                 *transCb
)
{
   WrInitCntxtSetupTransCb   *initCtxtCb = &transCb->u.initCtxtCb;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrErabInfo                *rabInfo;
   U32                       transId;
   U16                       idx;
   WrDamTnlInfo              *tnlInfo;

   for (idx = 0; idx < initCtxtCb->numErabs; idx++)
   {
      rabInfo = &initCtxtCb->erabList[idx];
      if (rabInfo->rabAllowed == TRUE)
      {
         transId = transCb->transId | idx;
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
         tnlInfo->tnlId.drbId = rabInfo->drbId;
         /* Cache Opt Changes */
         tnlInfo->tnlId.tnlPriority = wrUmmPrioIdforQci[rabInfo->qci-1];
         /* Fix for ccpu00123253*/
         tnlInfo->tnlType = WR_TNL_NORMAL;
         tnlInfo->hoType = WR_HO_TYPE_NONE;
         wrEmmGetDatAppAddr(&initCtxtCb->lclAddr, rabInfo->sgwAddr.type);
         wrCpyCmTptAddr(&tnlInfo->dstAddr, &(rabInfo->sgwAddr));
         wrCpyCmTptAddr(&tnlInfo->srcAddr, initCtxtCb->lclAddr);
         tnlInfo->remTeid = rabInfo->remTeId;
         wrUtlGetDscp(ueCb->cellId, rabInfo->qci, &(tnlInfo->dscp));

         rabInfo->tnlSetup = WR_INIT_CTXT_TNL_SETUP_SENT;
         wrIfmDamTnlCreatReq(transId, tnlInfo);

      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for processing the responses for
 *         tunnel creation.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtProcTnlMgmtCfm
 *
 *         Processing steps:
 *         - set the tunnel configuration flag for the particular DRB to
 *           WR_INIT_CTXT_TNL_SETUP_SUCC 
 *         - copy the tunnel id to the local data structure
 *         - if any 1 of the tunnel creations failed,
 *           - set the configuration state to WR_UMM_INIT_CTXT_ERROR
 *         - if reponse for all the admitted RABs are successful, then,
 *           - set the RAB transaction state to WR_UMM_INIT_CTXT_DAM_CFG_DONE
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmInitCtxtProcTnlMgmtCfm
(
WrUmmTransCb                  *transCb
)
{
   WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);
   WrErabInfo                *rabInfo;
   U32                       drbId;
   U8                        status;
   U8                        idx;

   for(idx = 0; idx < initCtxtCb->numErabs; idx++)
   {
      rabInfo = &initCtxtCb->erabList[idx];
      drbId   = (transCb->msg->u.damCfm.lclTeId & 0x000000ff) >> 3;
      status  = transCb->msg->u.damCfm.status;

      if(rabInfo->drbId == drbId)
      {
         if(status == WR_DAM_CFG_OK)
         {
            rabInfo->tnlSetup = WR_INIT_CTXT_TNL_SETUP_SUCC;
            rabInfo->lclTeId  = transCb->msg->u.damCfm.lclTeId;
         }
         else
         {
            /* Error_Cause: Filling the proper cause value in E-Rab setup failure
             * */
            rabInfo->rabAllowed        = FALSE;
            rabInfo->cause.causeTyp    = CAUSE_TRANSPORT;
            rabInfo->cause.causeVal    = SztCauseTporttransport_resource_unavailableEnum;
            initCtxtCb->state          = WR_UMM_INIT_CTXT_ERROR;
            RLOG3(L_ERROR, " DAM Confirm failed %d for rabId=%d UeId=%d ",
                  status,rabInfo->drbId,transCb->ueCb->crnti);
            RETVALUE(RFAILED);
         }
        /* As the DRB matches there is no need for further traversing */
        break;
      }
   }
   for(idx = 0; idx < initCtxtCb->numErabs; idx++)
   {
      rabInfo = &initCtxtCb->erabList[idx];
      /* Fix for ccpu00125111: Since WR_INIT_CTXT_TNL_SETUP_SENT value is zero.
       * Additional condition required for allowed bearers. */
      /* Fix for ccpu0012875, ccpu00126703 and ccpu00125111 */
      if((rabInfo->rabAllowed == TRUE) && (rabInfo->tnlSetup != WR_INIT_CTXT_TNL_SETUP_SUCC))
      {
         RETVALUE(ROK);
      }
   }
   initCtxtCb->state = WR_UMM_INIT_CTXT_DAM_CFG_DONE;
   RETVALUE(ROK);
}

/** @brief This function is responsible for marking if the ERABs in the ERAB 
 *         list are admitted or not.
 *
 * @details 
 *         - for each ERAB in the erabList,
 *           - if the erabId is found in the accepted ERABs list
 *             - set rabAllowed to TRUE
 *           - if erabId is not found, 
 *             - traverse the rejected ERABs list and update the cause value
 *               and set rabAllowed to FALSE
 *
 *     Function: wrUmmInitUpdateRabAdmInfo
 *
 *         Processing steps:
 *         - copy the DRB information for those DRBs that are suceccessfully
 *           established.
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmInitUpdateRabAdmInfo
(
WrUmmTransCb              *transCb
)
{
   U32                       idx;
   U32                       idx1;
   Bool                      alreadyProcessed = FALSE;
   WrErabInfo                *rabInfo;
   WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);
   RmuUeRecfgRsp             *ueRecfgRsp;
   
   ueRecfgRsp = initCtxtCb->rrmUeRecfgRspMsg->u.rrmUeRecfgResp;

   for(idx = 0; idx < initCtxtCb->numErabs; idx++)
   {
      rabInfo = &initCtxtCb->erabList[idx];
      for(idx1 = 0; (FALSE == alreadyProcessed) && 
                    (idx1 < ueRecfgRsp->bNoErabsAccepted); idx1++)
      {
         if(rabInfo->erabId == ueRecfgRsp->stErabAcceptLst[idx1].bErabId)
         {
            rabInfo->rabAllowed = TRUE;
            alreadyProcessed = TRUE;
         }
      }
      
      for(idx1 = 0; (FALSE == alreadyProcessed) && 
                    (idx1 < ueRecfgRsp->bNoErabsRejected) ; idx1++)
      {
         if(rabInfo->erabId == ueRecfgRsp->stErabRejectLst[idx1].bErabId)
         {
            rabInfo->rabAllowed = FALSE;
            rabInfo->cause.causeTyp = WR_CAUSE_RADIONW;
            rabInfo->cause.causeVal = ueRecfgRsp->stErabRejectLst[idx1].enCause;
            alreadyProcessed = TRUE;
            initCtxtCb->numErabFail++;
         }
      }
      alreadyProcessed = FALSE;
   }
   
   RETVALUE(ROK);
}

/** @brief This function is responsible for copying the DRB information to 
 *         the UE Control Block.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtCommitUeInfo
 *
 *         Processing steps:
 *         - copy the DRB information for those DRBs that are suceccessfully
 *           established.
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmInitCtxtCommitUeInfo
(
WrUmmTransCb              *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);
   WrErabInfo                *initCtxtRabInfo;
   WrUeRAB                   *rabInfo;
   U8                        freeIdx;
   U8                        idx;
   U8                        lcgId = 0;
   WrUmmCellCb               *cellCb;

   WR_UMM_GET_CELLCB(cellCb, ueCb->cellId);
   if(cellCb == NULLP)
   {
      RLOG_ARG0(L_ERROR, DBG_CELLID, ueCb->cellId, 
            "CellCb not found while committing ICS info to UE CB");
      RETVALUE(RFAILED);
   } 

   for(idx = 0; idx < initCtxtCb->numErabs; idx++)
   {
      initCtxtRabInfo = &(initCtxtCb->erabList[idx]);
      if(TRUE == initCtxtRabInfo->rabAllowed)
      {
         freeIdx = initCtxtRabInfo->erabId - WR_UMM_ERAB_OFFSET; 
         rabInfo = &(ueCb->rbInfo.rab[freeIdx]);
         rabInfo->inUse    = TRUE;
         rabInfo->eRABId   = initCtxtRabInfo->erabId;
         rabInfo->rbId     = initCtxtRabInfo->drbId;
         wrUtlGetRbMode(ueCb->cellId, initCtxtRabInfo->qci, &(rabInfo->rbMode));
         wrUtlGetRbDir(ueCb->cellId, initCtxtRabInfo->qci, &(rabInfo->rbDir));
         rabInfo->lchId    = initCtxtRabInfo->lchId;
         rabInfo->qci      = initCtxtRabInfo->qci;
         rabInfo->isGbrQosPres = initCtxtRabInfo->qos.isGbrQosPres;
         /*Convert to kbps from kBps. This is required for X2 HO*/
         rabInfo->mbr.dl   = initCtxtRabInfo->qos.dlMbr * 8;
         rabInfo->gbr.dl   = initCtxtRabInfo->qos.dlGbr * 8;
         rabInfo->mbr.ul   = initCtxtRabInfo->qos.ulMbr * 8;
         rabInfo->gbr.ul   = initCtxtRabInfo->qos.ulGbr * 8;

         lcgId = wrUmmGetLcgIdForQci(ueCb->cellId,initCtxtRabInfo->qci);
         if (WR_UMM_IS_GBR_BEARER(initCtxtRabInfo->qci))
         {
            ueCb->mappedLcgInfo[lcgId].effUlGbr += initCtxtRabInfo->qos.ulGbr;
            ueCb->mappedLcgInfo[lcgId].effUlMbr += initCtxtRabInfo->qos.ulMbr;
         }
         
         rabInfo->priority = initCtxtRabInfo->qos.priority;
         rabInfo->preempCap = initCtxtRabInfo->qos.preempCap;
         rabInfo->preempVul = initCtxtRabInfo->qos.preempVul;

         if(rabInfo->priority == cellCb->rabArpForEmerServ)
         {
            rabInfo->isEmergencyBearer = TRUE;
            ueCb->ueServTyp = WR_UMM_EMERGENCY_SERV; 
         }
 
         rabInfo->locEgtpTunId  = initCtxtRabInfo->lclTeId;
         rabInfo->remEgtpTunId  = initCtxtRabInfo->remTeId;
         /* SPS changes starts */
         rabInfo->isDlSpsEnabled  = initCtxtRabInfo->isDlSpsEnabled;
         rabInfo->isUlSpsEnabled  = initCtxtRabInfo->isUlSpsEnabled;
         /* SPS changes ends */
         wrCpyCmTptAddr(&rabInfo->sgwAddr, &(initCtxtRabInfo->sgwAddr));
         ueCb->rbInfo.numOfRbCfgd++;

         RLOG_ARG3(L_INFO, DBG_CRNTI,ueCb->crnti,"RAB Activated, RabId[%d] "
            "Local Tunel Id[%d] Remote Tunel Id[%d]", rabInfo->eRABId,
            rabInfo->locEgtpTunId, rabInfo->remEgtpTunId);
      }
   }
   ueCb->rbInfo.aggBr.dl = initCtxtCb->dlAmbr;
   ueCb->rbInfo.aggBr.ul = initCtxtCb->ulAmbr;

   if (initCtxtCb->secCfg != NULLP)
   {
      ueCb->secCfg       = initCtxtCb->secCfg;
      initCtxtCb->secCfg = NULLP;
   }
#ifdef TENB_AS_SECURITY
   if (initCtxtCb->hzSecCfg != NULLP)
   {
      ueCb->hzSecCfg       = initCtxtCb->hzSecCfg;
      initCtxtCb->hzSecCfg = NULLP;
   }
#endif

   if(initCtxtCb->ueCap != NULLP)
   {
      ueCb->ueCap        = initCtxtCb->ueCap;
      initCtxtCb->ueCap  = NULLP;
   }
   
   if(initCtxtCb->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap)
   {
      wrUmmCommitUeRadioResDedInfo(ueCb, 
            (&(initCtxtCb->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg)));

   }
   /* SPS changes starts */
   /* Commit the SPS parameters in UE control block */
   if(initCtxtCb->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_UL_SPS_CFG)
   {
      ueCb->spsCfg.spsRnti = transCb->spsCfgTransCb.spsCfg.spsRnti;
      ueCb->spsCfg.spsCfgUl = transCb->spsCfgTransCb.spsCfg.spsCfgUl;
   }
   if(initCtxtCb->rrmUeRecfgRspMsg->u.rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & RM_RCM_DL_SPS_CFG)
   {
      ueCb->spsCfg.spsRnti = transCb->spsCfgTransCb.spsCfg.spsRnti;
      ueCb->spsCfg.spsCfgDl = transCb->spsCfgTransCb.spsCfg.spsCfgDl;
   }
   /* SPS changes ends */

   WR_MEM_COPY(&ueCb->srcGummei, &initCtxtCb->gummei, sizeof(WrGummei));
   WR_MEM_COPY(&ueCb->s1ConCb->mmeUeS1apId2, &initCtxtCb->mmeUeS1apId2,
         sizeof(TknU32));

   RETVALUE(ROK);
}

/** @brief This function is responsible for filling the S1AP Initial Context 
 *         Setup Response message.
 *
 * @details
 *
 *     Function: wrInitCtxtFillS1apRsp
 *
 *         Processing steps:
 *         - fill the Initial Context Setup Response message with the 
 *           following ies,
 *           UE ENB S1AP Id
 *           UE MME S1AP Id
 *           E-RAB Setup List
 *           E-RAB Failed List (if any RAB could not be setup)
 *
 * @param[in] ueCb     : UE Control Block
 * @param[in] initCtxtCb : Initial Context Transaction information
 * @param[in, out] s1apPdu  : S1AP PDU to be filled
 * @return S16
 *      -# ROK: In case of Success 
 *      -# RFAILED: In case of Failure
 * @param[in, out] pdu  : S1AP PDU to be filled
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrInitCtxtFillS1apRsp
(
WrUeCb                       *ueCb,
WrInitCntxtSetupTransCb      *initCtxtCb,
S1apPdu                      **s1apPdu
)
{
   SztProtIE_SingleCont_E_RABSetupItemCtxtSUResIEs  *ie1;
   SztProtIE_Field_InitCntxtSetupRespIEs            *ie;
   SztInitCntxtSetupResp                            *initCtxtRsp;
   S1apPdu                                          *initCtxtRspPdu = NULLP;
   SztSuccessfulOutcome                             *succOut;
   U16                                              idx;
   U16                                              cnt;
   U8                                               offSet;
   U16                                              numComp;
   U32                                              len;
   U16                                              crntIe = 0;
   WrS1ConCb                                        *s1ConCb = ueCb->s1ConCb;
   WrErabInfo                                       *rabInfo;
   U16                                              numSuccErabs = 0;
   U16                                              numFailedErabs = 0;
   U8                                               failIdx = 0;
   U8                                               succIdx = 0;

   RLOG0(L_DEBUG, "Constructing Initial Context Setup Response");

   WR_ALLOCEVNT(&initCtxtRspPdu, sizeof(S1apPdu));
   if (initCtxtRspPdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(initCtxtRspPdu->pdu.choice), SZT_TRGR_SUCCESS_OUTCOME);
   succOut = &(initCtxtRspPdu->pdu.val.successfulOutcome);
   wrFillTknU8(&(succOut->pres), PRSNT_NODEF);
   wrFillTknU32(&(succOut->procedureCode), Sztid_InitCntxtSetup);
   wrFillTknU32(&(succOut->criticality), SztCriticalityrejectEnum);

   initCtxtRsp = &succOut->value.u.sztInitCntxtSetupResp;
   wrFillTknU8(&(initCtxtRsp->pres),PRSNT_NODEF);

   /*Error_Cause:Find the number of E-Rab successfully setup */
   for(idx = 0; idx < (initCtxtCb->numErabs); idx++)
   {
      rabInfo = &(initCtxtCb->erabList[idx]);
      if (rabInfo->rabAllowed == TRUE)
      {
         numSuccErabs++;
         WR_INC_KPI_FUNC1(wrIncKpiFrERABEstbIntSuccPrQci,rabInfo->qci);
      }
   }   

   /* numSuccErabs is calculated on the basis of rabInfo->rabAllowed== TRUE
    * condition */ 
   /* numSuccErabs    = initCtxtCb->numErabsAllowed; */
   numFailedErabs  = (initCtxtCb->numErabs - numSuccErabs);
   /* asign numComp = 4 if Failed list is there else 3 */
   /* List of IEs Filled:
    *   1. UE ENB S1AP ID
    *   2. UE MME S1AP ID
    *   3. E-RAB Setup List
    *   4. E-RAB Failed List (Optional)
    */
   if(0 == numFailedErabs)
   {
      numComp = 3;
   }
   else
   {
      numComp = 4;
   }

   if ((cmGetMem(initCtxtRspPdu,
                 (numComp * sizeof(SztProtIE_Field_InitCntxtSetupRespIEs)),
                 (Ptr *)&initCtxtRsp->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      cmFreeMem(initCtxtRspPdu);
      RETVALUE(RFAILED);
   }

   /* Filling MME UE ID */
   ie = &initCtxtRsp->protocolIEs.member[crntIe++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);   
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);   
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1ConCb->mme_ue_s1ap_id);  

   /* Filling ENB UE ID */
   ie = &initCtxtRsp->protocolIEs.member[crntIe++];    
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum); 
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1ConCb->enb_ue_s1ap_id);
   /* Filling ERAB Setup List */
   ie = &initCtxtRsp->protocolIEs.member[crntIe++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);
   wrFillTknU32(&(ie->id), Sztid_E_RABSetupLstCtxtSURes);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);

   numComp = numSuccErabs;
   wrFillTknU16(&(ie->value.u.sztE_RABSetupLstCtxtSURes.noComp), numSuccErabs);
   if (cmGetMem(initCtxtRspPdu,
       (numSuccErabs * sizeof(SztProtIE_SingleCont_E_RABSetupItemCtxtSUResIEs)),
       (Ptr *)&(ie->value.u.sztE_RABSetupLstCtxtSURes.member)) !=ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      cmFreeMem(initCtxtRspPdu);
      RETVALUE(RFAILED);
   }

   for(idx = 0; idx < initCtxtCb->numErabs; idx++)
   {
      /*rabInfo = initCtxtCb->erabList;*/
      /*Error_Cause: change */
      rabInfo = &(initCtxtCb->erabList[idx]);
      if (rabInfo->rabAllowed == TRUE)
      {
         SztE_RABSetupItemCtxtSURes *erabIe;

         ie1 = &(ie->value.u.sztE_RABSetupLstCtxtSURes.member[succIdx++]);
         erabIe = &(ie1->value.u.sztE_RABSetupItemCtxtSURes);
         wrFillTknU8(&(ie1->pres), PRSNT_NODEF);
         wrFillTknU32(&(ie1->id), Sztid_E_RABSetupItemCtxtSURes);
         wrFillTknU32(&(ie1->criticality), SztCriticalityignoreEnum);
         wrFillTknU8(&(erabIe->pres), PRSNT_NODEF);
         wrFillTknU32(&(erabIe->e_RAB_ID), rabInfo->erabId);
         if (initCtxtCb->lclAddr->type == CM_TPTADDR_IPV4)
         {
            len = 4;
            erabIe->transportLyrAddr.pres = PRSNT_NODEF;
            erabIe->transportLyrAddr.len = (U16)(len * 8);
            WR_GET_MEM(initCtxtRspPdu, len, &erabIe->transportLyrAddr.val);
            for (cnt = 0; cnt < len; cnt++)
            {
               offSet =(U8)((len - (cnt + 1)) * 8);
               erabIe->transportLyrAddr.val[cnt] = 
                    (U8)(initCtxtCb->lclAddr->u.ipv4TptAddr.address >> offSet);
            }
         }
         else if (initCtxtCb->lclAddr->type == CM_TPTADDR_IPV6)
         {
            len = WR_IPV6_ADDR_LEN;
            erabIe->transportLyrAddr.pres = PRSNT_NODEF;
            erabIe->transportLyrAddr.len = (U16)(len * WR_BYTE_LEN);
            WR_GET_MEM(initCtxtRspPdu, len, &erabIe->transportLyrAddr.val);
            for (cnt = 0; cnt < len; cnt++)
            {
               offSet =(U8)((len - (cnt + 1)) * WR_BYTE_LEN);
               erabIe->transportLyrAddr.val[cnt] = 
                  (initCtxtCb->lclAddr->u.ipv6TptAddr.ipv6NetAddr[cnt]); 
            }
         }  

         len = 4;
         erabIe->gTP_TEID.len = (U16)len;
         erabIe->gTP_TEID.pres = PRSNT_NODEF;
         WR_GET_MEM(initCtxtRspPdu, len, &erabIe->gTP_TEID.val);
         for (cnt = 0; cnt < len; cnt++)
         {
            offSet = (U8)(len - (cnt + 1)) * 8;
            erabIe->gTP_TEID.val[cnt] = (U8)(rabInfo->lclTeId >> offSet);
         }
         erabIe->iE_Extns.noComp.pres = NOTPRSNT;
      }
       /*Added for KPI*/
       #ifdef WR_RSYS_KPI 
       SGetEpcTime(&(rabInfo->erabEstabTm));
       #endif/*WR_RSYS_KPI*/
       WR_INC_KPI_FUNC3(wrUpdateErabEstabTimeMeanMax,rabInfo->qci,rabInfo->earbSetupReqTm,rabInfo->erabEstabTm);
   }

   /* Filling ERAB Failed List */
   if (numFailedErabs != 0)
   {
      SztE_RABLst                  *sztE_RABLst;
      SztProtIE_Field_E_RABItemIEs *failedIe;

      /*Error_Cause: Need to fill the cause value per Rab-Id */ 
      numComp = numFailedErabs;
      ie = &initCtxtRsp->protocolIEs.member[crntIe++];
      wrFillTknU8(&(ie->pres), PRSNT_NODEF);
      wrFillTknU32(&(ie->id), Sztid_E_RABFailedToSetupLstCtxtSURes);
      wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);
      sztE_RABLst = &(ie->value.u.sztE_RABLst);
      wrFillTknU16(&(sztE_RABLst->noComp), numComp);

      if (cmGetMem(initCtxtRspPdu,
           (numComp * sizeof(SztProtIE_SingleCont_E_RABItemIEs)),
           (Ptr *)&(sztE_RABLst->member)) != ROK)
      {
         RLOG0(L_FATAL, "Memory allocation failed.");
         cmFreeMem(initCtxtRspPdu);
         RETVALUE(RFAILED);
      }
      for(idx = 0; idx < initCtxtCb->numErabs; idx++)
      {
         /* rabInfo = initCtxtCb->erabList; */
         /*Error_Cause:Change */
         rabInfo = &(initCtxtCb->erabList[idx]);
         if(rabInfo->rabAllowed == FALSE)
         {
            failedIe = &sztE_RABLst->member[failIdx++];
            wrFillTknU8(&(failedIe->pres), PRSNT_NODEF);
            wrFillTknU32(&(failedIe->id), Sztid_E_RABItem);
            wrFillTknU32(&(failedIe->criticality), SztCriticalityignoreEnum);
            wrFillTknU8(&(failedIe->value.u.sztE_RABItem.pres), PRSNT_NODEF);
            wrFillTknU32(&(failedIe->value.u.sztE_RABItem.e_RAB_ID),
                  rabInfo->erabId);
            /*Error_Cause: Fill the proper cause value here*/
            RLOG3(L_DEBUG, "E-Rab ID[%d]\t Cause Type[%ld]\t Cause Value[%ld]",
               rabInfo->erabId, rabInfo->cause.causeTyp,rabInfo->cause.causeVal);
            wrS1apFillCause(&(failedIe->value.u.sztE_RABItem.cause), &(rabInfo->cause));
            failedIe->value.u.sztE_RABItem.iE_Extns.noComp.pres = NOTPRSNT;
            WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, rabInfo->cause.causeTyp, rabInfo->cause.causeVal, INC_KPI_VALUE_BY_ONE);
         }
      }
      
      /*Increment KPI for Number of Initial E-RABs failed to setup*/
      WR_INC_KPI_FUNC1(wrIncKpiFrERABEstbInitFailSum,numFailedErabs);
   }
   wrFillTknU16(&(initCtxtRsp->protocolIEs.noComp), crntIe);
   *s1apPdu = initCtxtRspPdu; 

   RETVALUE(ROK);
}

/** @brief This function is responsible for filling and sending the Initial 
 *         Context Setup Response message to S1AP.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtSndRsp
 *
 *         Processing steps:
 *         - invoke wrInitCtxtFillS1apRsp to fill the response message
 *         - send the message to S1AP
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -# Success : ROK
 *    -# Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtSndRsp
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);
   S1apPdu                   *s1apPdu;

   RLOG0(L_DEBUG, "Sending initial context response");
   if (wrInitCtxtFillS1apRsp(ueCb, initCtxtCb, &s1apPdu) != ROK)
   {
      RETVALUE(RFAILED);
   }

   RLOG_ARG2(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Sending INITIAL CONTEXT SETUP "
             "RESPONSE [MME-UE-S1AP-ID:%lu] [ENB-UE-S1AP-ID:%lu]",
             ueCb->s1ConCb->mme_ue_s1ap_id, ueCb->s1ConCb->enb_ue_s1ap_id);

   RLOG_ARG2(L_DEBUG, DBG_CRNTI,ueCb->crnti,"Sending INITIAL CONTEXT SETUP "
      "RESPONSE, MME-UE-S1AP-ID[%lu] ENB-UE-S1AP-ID[%lu]",
      ueCb->s1ConCb->mme_ue_s1ap_id, ueCb->s1ConCb->enb_ue_s1ap_id);
   if (wrIfmS1apSndUeMsg(ueCb->s1ConCb->spConnId, s1apPdu) != ROK)
   {
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for filling the S1AP Initial Context 
 *         Setup Failure message.
 *
 * @details
 *
 *     Function: wrInitCtxtFillSetupFail
 *
 *         Processing steps:
 *         - fill the Initial Context Setup Response message with the 
 *           following ies,
 *           UE ENB S1AP Id
 *           UE MME S1AP Id
 *           Cause
 *
 * @param[in] ueCb     : UE Control Block
 * @param[in] initCtxtCb : Initial Context Transaction information
 * @param[in, out] s1apPdu  : S1AP PDU to be filled
 * @return S16
 *      -# ROK: In case of Success 
 *      -# RFAILED: In case of Failure
 * @param[in, out] pdu  : S1AP PDU to be filled
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrInitCtxtFillSetupFail
(
WrUeCb                       *ueCb,
WrInitCntxtSetupTransCb      *initCtxtCb,
S1apPdu                      **s1apPdu
)
{
   S1apPdu                   *setupFailPdu;
   SztUnsuccessfulOutcome    *unsuccOut;
   SztInitCntxtSetupFail     *setupFail;
   U32                       crntIe = 0;
   WrS1ConCb                 *s1ConCb = ueCb->s1ConCb;
   WrUmmMsgCause             cause;
   SztProtIE_Field_InitCntxtSetupFailIEs *ie;

   RLOG0(L_DEBUG, "Constructing Initial Context Setup Failure");

   WR_ALLOCEVNT(&setupFailPdu, sizeof(S1apPdu));
   if (setupFailPdu == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      RETVALUE(RFAILED);
   }

   wrFillTknU8(&(setupFailPdu->pdu.choice), SZT_TRGR_UNSUCCESS_OUTCOME);
   unsuccOut = &(setupFailPdu->pdu.val.unsuccessfulOutcome);
   wrFillTknU8(&(unsuccOut->pres), PRSNT_NODEF);
   wrFillTknU32(&(unsuccOut->procedureCode), Sztid_InitCntxtSetup);
   wrFillTknU32(&(unsuccOut->criticality), SztCriticalityrejectEnum);

   setupFail = &unsuccOut->value.u.sztInitCntxtSetupFail;
   wrFillTknU8(&(setupFail->pres), PRSNT_NODEF);

   /* List of IEs Filled:
    *   1. UE ENB S1AP ID
    *   2. UE MME S1AP ID
    *   3. Cause
    */
   if ((cmGetMem(setupFailPdu,
                 (3 * sizeof(SztProtIE_Field_InitCntxtSetupFailIEs)),
                 (Ptr *)&setupFail->protocolIEs.member)) != ROK)
   {
      RLOG0(L_FATAL, "Memory allocation failed.");
      cmFreeMem(setupFailPdu);
      RETVALUE(RFAILED);
   }

   /* Filling MME UE ID */
   ie = &setupFail->protocolIEs.member[crntIe++];
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);   
   wrFillTknU32(&(ie->id), Sztid_MME_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum);   
   wrFillTknU32(&(ie->value.u.sztMME_UE_S1AP_ID), s1ConCb->mme_ue_s1ap_id);  

   /* Filling ENB UE ID */
   ie = &setupFail->protocolIEs.member[crntIe++];    
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_eNB_UE_S1AP_ID);   
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum); 
   wrFillTknU32(&(ie->value.u.sztENB_UE_S1AP_ID), s1ConCb->enb_ue_s1ap_id);

   ie = &setupFail->protocolIEs.member[crntIe++];    
   cause.causeTyp = initCtxtCb->failCauseType;
   /*ccpu00126907 */
   cause.causeVal = initCtxtCb->failCause;
   WR_INC_KPI_FUNC3(wrIncKpiFrS1apFailCause, cause.causeTyp, cause.causeVal, INC_KPI_VALUE_BY_ONE);
   RLOG2(L_DEBUG, "wrInitCtxtFillSetupFail: Cause Type[%ld]\t Cause Value[%ld]",
      cause.causeTyp, cause.causeVal);
   wrFillTknU8(&(ie->pres), PRSNT_NODEF);  
   wrFillTknU32(&(ie->id), Sztid_Cause);
   wrFillTknU32(&(ie->criticality), SztCriticalityignoreEnum); 
   wrS1apFillCause(&(ie->value.u.sztCause), &cause);

   wrFillTknU16(&(setupFail->protocolIEs.noComp), crntIe);
   *s1apPdu = setupFailPdu; 

   RETVALUE(ROK);
}

/** @brief This function is responsible for filling and sending the Initial 
 *         Context Setup Failure message to S1AP.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtSndFail
 *
 *         Processing steps:
 *         - invoke wrInitCtxtFillSetupFail to fill the failure message
 *         - send the message to S1AP
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtSndFail
(
WrUmmTransCb                 *transCb
)
{
   WrUeCb                    *ueCb = transCb->ueCb;
   WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);
   S1apPdu                   *s1apPdu;

   if (wrInitCtxtFillSetupFail(ueCb, initCtxtCb, &s1apPdu) != ROK)
   {
      RETVALUE(RFAILED);
   }

   RLOG_ARG3(L_EVENT, DBG_CRNTI,ueCb->crnti,"[S1AP]:Sending INITIAL CONTEXT SETUP "
             "FAILURE [MME-UE-S1AP-ID:%lu] [eNB-UE-S1AP-ID:%lu] [CauseType:%ld]",
             ueCb->s1ConCb->mme_ue_s1ap_id, ueCb->s1ConCb->enb_ue_s1ap_id,
             initCtxtCb->failCauseType);

   RLOG_ARG3(L_DEBUG, DBG_CRNTI,ueCb->crnti,"Sending INITIAL CONTEXT SETUP "
      "FAILURE, MME-UE-S1AP-ID[%lu] eNB-UE-S1AP-ID[%lu] Cause[%d]",
      ueCb->s1ConCb->mme_ue_s1ap_id, ueCb->s1ConCb->enb_ue_s1ap_id,
      WR_CAUSE_RADIONW);

   wrPrintS1apCauseInfo(initCtxtCb->failCauseType,initCtxtCb->failCause);
   if (wrIfmS1apSndUeMsg(ueCb->s1ConCb->spConnId, s1apPdu) != ROK)
   {
      RETVALUE(RFAILED);
   }

#if 1
   /* Start the timer for unsuccessfulOutcome If MME is
    * not responded with UE Context Release command then 
    * start timer and release the UE */
   wrStartUeTmr(ueCb,WR_TMR_UE_CTX_REL_REQ, WR_TMR_UE_CTX_REL_VAL);
#endif
   /*Increment KPI for Number of Initial E-RABs failed to setup*/
   WR_INC_KPI_FUNC1(wrIncKpiFrERABEstbInitFailSum,initCtxtCb->numErabs);

   RETVALUE(ROK);
}

/** @brief This function is responsible for processing the initial context
 *         setup request message
 *
 * @details
 *
 *     Function: wrUmmInitCtxtPrcS1Pdu
 *
 *         Processing steps:
 *         - invoke wrUmmInitCtxtPrcSetup to process the initial context setup
 *           request message
 *         - invoke wrUmmInitCtxtPrcRabSetup to process the parsed RAB
 *           information
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtPrcS1Pdu
(
WrUmmTransCb                 *transCb
)
{
   WrInitCntxtSetupTransCb   *initCtxt = &(transCb->u.initCtxtCb);
   /* CSG_DEV */
   U8                        accessMode;
   U32                       csgId = 0;


   /*ccpu00126907:Initialize the fail cause value to default - radio 
    * interface network fail*/
    initCtxt->failCause =
    SztCauseRadioNwfailure_in_radio_interface_procedureEnum;
    initCtxt->failCauseType = WR_CAUSE_RADIONW;
   
   /*This is received in response to Initial UE message. Increment Signalling counter to
   account for sucessfull UE to MME associated Signalling connection.*/
   wrEmmUpdateStat(WR_EMM_SIG_STATS,transCb->ueCb->cellId,WR_EMM_SIG_SUCCESS,1,WR_EMM_SIG_SUBTYP_MAX);
   
    /*Stopping Initial Context Setup Req */
    wrStopUeTmr(transCb->ueCb, WR_TMR_INITIAL_CTX_REQ);
   /* Process the message and fill up the transaction with the received  */
   /* values                                                             */
   if (wrUmmInitCtxtPrcSetup(transCb) != ROK)
   {
      initCtxt->state = WR_UMM_INIT_CTXT_ERROR;
      RETVALUE(RFAILED);
   }

   /* Update UE Membership status */
   accessMode = wrEmmGetAccessModeFrmCellId(transCb->ueCb->cellId);
   if(WR_ENB_OPEN_ACCESS != accessMode)
   {
      /* Get Serving Cell CSG ID here */
      WR_GET_CSG_ID(transCb->ueCb->cellId, csgId);
      if(RFAILED == wrUmmCsgAddCsgIdIntoMbrLst(transCb->ueCb, csgId,
               initCtxt->ueMbrShip))
      {
         initCtxt->state = WR_UMM_INIT_CTXT_ERROR;
         RLOG2(L_INFO,"Failed to Insert UE membership status NBR CSG ID "
               "[%d] membership status [%d]", csgId, initCtxt->ueMbrShip);
         RETVALUE(RFAILED);
      }
   }
 
   /* Prepare the resources needed to complete the RABs to be established */
   /* In case of failure to prepare sufficient resources, fail the proc   */
   if (wrUmmInitCtxtPrcRabSetup(transCb) != ROK)
   {
      initCtxt->state = WR_UMM_INIT_CTXT_ERROR;
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for processing the incoming RRC
 *         messages.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtPrcRrcPdu
 *
 *         Processing steps:
 *         - free the RRC Transaction ID that was allocated for the procedure
 *         - if the message is UE Capability Info,
 *           - store the UE Capability information and request RRC for encoding
 *             the UE Radio Capability IE
 *         - if the message is Security Mode Complete,
 *           - set the initial context transaction state to
 *             WR_UMM_INIT_CTXT_SEC_DONE
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmInitCtxtPrcRrcPdu
(
WrUmmTransCb                 *transCb
)
{
   S16                       ret;
   WrUmmIncMsg               *msg = transCb->msg;
   WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);
   NhuDatIndSdus             *uuIndPdu;
   NhuUL_DCCH_MsgTyp         *ulDcchMsg;
   NhuUL_DCCH_MsgTypc1       *rrcMsg;
   NhuUECapInfo              *ueCapInfo;
   U32                       rrcTransId = 0;
   WrUeCb                    *ueCb = transCb->ueCb;

   /* received the response for UE cap enquiry */
   uuIndPdu = msg->u.uuIndPdu;
   ulDcchMsg = &(uuIndPdu->sdu.m.ulDcchMsg.dcchMsg.message);
   rrcMsg = &(ulDcchMsg->val.c1);
   switch (rrcMsg->choice.val)
   {
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
#ifdef WR_TEST_CODE
         if ( wrTrgUeCapTO == TRUE )
         {
           RLOG0(L_INFO, "Dropping C1_UECAPINFO!!");
           RETVALUE(ROK);
         }
#endif
         
         ueCapInfo = &(rrcMsg->val.ueCapInfo);
         rrcTransId = ueCapInfo->rrc_TrnsxnId.val;
         if (wrUmmRelRrcTransId(transCb->ueCb, C1_UECAPENQUIRY, rrcTransId) != ROK)
         {
            /*ccpu00127802*/
            RLOG0(L_ERROR, "Release of transaction ID failed ");
         }

         /*Stop Radio Capability Enquiry  Timer*/
         wrStopTransTmr(transCb,WR_TMR_RRC_CAPENQ);
         /* eCSFB:- Set the transCb state, as unable to
          * store ueCapInfo due to mem allocation failue*/
         if(ROK != wrUmmUeCapStoreCapInfo(transCb , ueCapInfo))
         {
            initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
            RETVALUE(RFAILED);
         }
         wrUmmSndUeCapEncReq(ueCb, transCb->transId, ueCapInfo);
         break;
      }
      case C1_SECURITYMODECOMPL:
      {
#ifdef WR_TEST_CODE
         if ( wrTrgSecTO == TRUE )
         {
           ALARM("Dropping C1_SECURITYMODECOMPL!! \n");
           RETVALUE(ROK);
         }
#endif
         /*ccpu00127802*/
         RLOG_ARG0(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Received SecurityModeComplete");

         rrcTransId = rrcMsg->val.securityModeCompl.rrc_TrnsxnId.val;
         if (wrUmmRelRrcTransId(transCb->ueCb, C1_SECURITYMODECMD, rrcTransId) != ROK)
         {
            /*ccpu00127802*/
            RLOG0(L_ERROR, "Release of transaction ID failed");
         }
         /*Stop Radio Capability Enquiry  Timer*/
         wrStopTransTmr(transCb,WR_TMR_RRC_SECUCMD);


         initCtxtCb->state = WR_UMM_INIT_CTXT_SEC_DONE;
         break;
      }
      case C1_RRCCONRECFGNCOMPL:
      {
#ifdef WR_TEST_CODE
         if ( wrTrgRrcReConTO == TRUE )
         {
           ALARM("Dropping C1_RRCCONRECFGNCOMPL!! \n");
           RETVALUE(ROK);
         }
#endif

         /*ccpu00127802*/
         RLOG_ARG0(L_EVENT, DBG_CRNTI,ueCb->crnti,"[OTA] Rrc Connection "
            "Reconfiguration Complete Recieved");

         /* Stop the RRC reconfig timer */
         wrStopTransTmr(transCb,WR_TMR_RECONFIGURE);

         /* IRAT.MOBILITY.LAI This KPI will be incremented only
          * if the MMEGI sent in registered MME IE has msb as '0'*/
         if((TRUE == ueCb->mobLaiFlag) && !(ueCb->lac & 0x8000))
         {
            wrIncIratMobLai(ueCb->cellId,ueCb->plmnId,ueCb->lac);
         }

         rrcTransId = rrcMsg->val.rrcConRecfgnCompl.rrc_TrnsxnId.val;
         ret = wrUmmRelRrcTransId(transCb->ueCb, C1_RRCCONRECFGN, rrcTransId);
         if (ret != ROK)
         {
            /*ccpu00127802*/
            RLOG0(L_ERROR, "Release of transaction ID failed");
         }
         break;
      }
      case C1_SECURITYMODEFAIL:
      {
         /* fix for ccpu00126929 */
         /*ccpu00127802*/
         RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Received "
               "SecurityModeFailure");
         wrStopTransTmr(transCb,WR_TMR_RRC_SECUCMD);
         
         initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR; 
              /* fix for ccpu00127792 */
              initCtxtCb->failCause = SztCauseRadioNwfailure_in_radio_interface_procedureEnum;
         /* Increment the security mode failure counter */
         WR_INC_KPI_FUNC(wrIncKpiFrRrcSecFailCause);
         /* send initial context setup failure */
         break;
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for handling the RRC lower layer 
 *         configuration status.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtPrcRrcCfgCfm
 *
 *         Processing steps:
 *         - get the sub-transaction id from transId,
 *         - if the confirm is for security configuration,
 *           - if the configuration status is FALSE,
 *             - set the initial context transaction state to 
 *               WR_UMM_INIT_CTXT_ERROR
 *         - else, if the confirm is for lower layer configuration,
 *           - if the configuration status is FALSE,
 *             - set the initial context transaction state to 
 *               WR_UMM_INIT_CTXT_ERROR
 *           - set the initial context transaction state to 
 *             WR_UMM_INIT_CTXT_RRC_CFG_DONE
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmInitCtxtPrcRrcCfgCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);
   NhuCfgCfmSdus             *rrcCfm;
   U32                       cfgCfmTrId;

   rrcCfm     = msg->u.rrcCfm;
   cfgCfmTrId = rrcCfm->hdr.transId;
   switch((cfgCfmTrId & 0x0f))
   {
      case C1_SECURITYMODECMD:
      {
         if (rrcCfm->sdu.isCfgSuccess == FALSE)
         {
            initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
            RLOG0(L_ERROR, " AS Security Config failed");
            RETVALUE(RFAILED);
         }
         break;
      }
      case C1_RRCCONRECFGN:
      {
         if(rrcCfm->sdu.isCfgSuccess == FALSE)
         {
            initCtxtCb->state        = WR_UMM_INIT_CTXT_ERROR;
            RLOG0(L_ERROR, "DRB Config failed");
            RETVALUE(RFAILED);
         }
         else
         {
            initCtxtCb->state = WR_UMM_INIT_CTXT_RRC_CFG_DONE;
         }
         break;
      }
      default:
      {
         initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
         RLOG1(L_ERROR, "Invalid transaction id %lu", cfgCfmTrId);
         RETVALUE(RFAILED);
      }
   }

   RETVALUE(ROK);
}

/** @brief This function is responsible for processing the encode confirm
 *         from RRC.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtPrcRrcEncCfm
 *
 *         Processing steps:
 *         - if the encode confirm is for NHU_MSG_UECAPINFO,
 *           - invoke wrUmmUeCapHdlUeCapEncCfm
 *         - if the encode confirm is for NHU_MSG_UERACAPINFO,
 *           - invoke wrUmmUeCapHdlUeRaCapEncCfm
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmInitCtxtPrcRrcEncCfm
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg              *msg = transCb->msg;
   NhuEncCfmSdus            *rrcEncCfmSdus = NULLP;
   WrInitCntxtSetupTransCb  *initCtxtCb = &(transCb->u.initCtxtCb);

   rrcEncCfmSdus = msg->u.encCfm;
   switch (rrcEncCfmSdus->sdu.msgCategory)
   {
      case NHU_MSG_UECAPINFO:
      {
         wrUmmUeCapHdlUeCapEncCfm(transCb->ueCb, rrcEncCfmSdus, transCb->transId);
         break;
      }
      case NHU_MSG_UERACAPINFO:
      {
         if( ROK == wrUmmUeCapHdlUeRaCapEncCfm(transCb->ueCb, rrcEncCfmSdus))
         {
            if (initCtxtCb->ueCap->ueEutraOctStr.pres)
            {
               wrUmmRrcDecReqHdl(transCb, &initCtxtCb->ueCap->ueEutraOctStr,
                     NHU_DEC_SDUTYPE_EUTRA_UECAP_IE);
            }
         }
         break;
      }
   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for processing the decode confirm
 *         from RRC.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtPrcRrcDecCfm
 *
 *         Processing steps:
 *         - if the encode confirm is for NHU_MSG_UECAPINFO,
 *           - invoke wrUmmUecapEutraIeDecCfmHdl
 *           - set initial context setup transaction state to
 *             WR_UMM_INIT_CTXT_SEC_DONE
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16 wrUmmInitCtxtPrcRrcDecCfm
(
WrUmmTransCb           *transCb
)
{
   WrUmmIncMsg              *msg = transCb->msg;
   NhuDecodeSduCfm          *decCfm;
   WrInitCntxtSetupTransCb  *initCtxtCb = &(transCb->u.initCtxtCb);
   /* SPS changes starts */
   S16                      ret = ROK;
   /* SPS changes ends */
   WrUmmRatLst               ratLst;

   decCfm = msg->u.decCfm;
   switch(decCfm->sduType)
   {
      /* SPS changes starts */
      case NHU_MSG_UECAPINFO_V9A0:
      {
         /* Process 9A extention decode confirm and if success finish UE capability
          * procedure. */
         if (wrUmmUecapEutraIe9a0DecCfmHdl(initCtxtCb->ueCap, decCfm) != ROK)
         {
            initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
            RLOG_ARG0(L_ERROR, DBG_CRNTI, transCb->ueCb->crnti, "NHU_MSG_UECAPINFO_V9A0 decode failed UE");
            RETVALUE(RFAILED);
         }
         else
         {
            RLOG0(L_DEBUG, "NHU_MSG_UECAPINFO_V9A0 decoded while Initial UE");
            initCtxtCb->state = WR_UMM_INIT_CTXT_UECAP_DONE;
            /* updating the UE Capbility information to UE CB here, since the
             * information is needed by Measurement Modeule
             */
            transCb->ueCb->ueCap        = initCtxtCb->ueCap;
            /* Update ueCat value */
            if (initCtxtCb->ueCap->eUTRA_Cap.ue_Category.pres == TRUE)
            {
               transCb->ueCb->ueCat = 
                  initCtxtCb->ueCap->eUTRA_Cap.ue_Category.val;
               /*ccpu00127802*/
               RLOG1(L_DEBUG, "UE Capability msg: Updating ueCat value[%d]",
                  transCb->ueCb->ueCat);
               /* RRC-REL10-Upgrade */ 
               wrUtlUpdateUeCatV1020(transCb->ueCb, initCtxtCb->ueCap);
            }
            RETVALUE(ROK);
         }
         break;
      }
      /* SPS changes ends */
      case NHU_DEC_SDUTYPE_EUTRA_UECAP_IE:
      {
         /* RRC DEC CFM Received and validate whether it contains IRAT Info or not
          * and then check this decoded information is directly received from MME
          * if both are TRUE then again query IRAT Info , else handle Dec Hdl*/
         wrUmmUeReqdIratInfoInd(decCfm, initCtxtCb->ueCap, &ratLst);
         if((NULLP != initCtxtCb->ueRadCapIe) && (ratLst.noComp > 1))
         {
            initCtxtCb->ueRadCapIe = NULLP;
            if(ROK != wrUmmUeCapSndUeCapEnq(transCb, &ratLst))
            {
               initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
            }
         }
         /* SPS changes starts */
         else if (((ret = wrUmmUecapEutraIeDecCfmHdl(transCb->ueCb, initCtxtCb->ueCap, decCfm)) != ROK) && \
               (ret != WR_UMM_NHU_MSG_UECAPINFO_V9A0_DEC_SENT))
         /* SPS changes ends */
         {
            initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
            RLOG_ARG0(L_ERROR, DBG_CRNTI, transCb->ueCb->crnti,
               "NHU_DEC_SDUTYPE_EUTRA_UECAP_IE decode failed");
            RETVALUE(RFAILED);
         }
         else
         {
            /* SPS changes starts */
            if (ret != WR_UMM_NHU_MSG_UECAPINFO_V9A0_DEC_SENT)
            {
               /* Finish the UE capability decoding here only if the
                * decoding request for 9A extention is not sent to
                * RRC. */
               /* SPS changes ends */
               initCtxtCb->state = WR_UMM_INIT_CTXT_UECAP_DONE;
               /* updating the UE Capbility information to UE CB here, since the
                * information is needed by Measurement Modeule
                */
               transCb->ueCb->ueCap        = initCtxtCb->ueCap;
               /* Update ueCat value */
               if (initCtxtCb->ueCap->eUTRA_Cap.ue_Category.pres == TRUE)
               {
                  transCb->ueCb->ueCat = 
                     initCtxtCb->ueCap->eUTRA_Cap.ue_Category.val;
                  /*ccpu00127802*/
                  RLOG1(L_DEBUG, "UE Capability msg: Updating ueCat value[%d]",
                        transCb->ueCb->ueCat);
               /* RRC-REL10-Upgrade */ 
                  wrUtlUpdateUeCatV1020(transCb->ueCb, initCtxtCb->ueCap);
               }
               RETVALUE(ROK);
               /* SPS changes starts */
            }
            /* SPS changes ends */
         }
         break;
      }
      case NHU_MSG_UERACAPINFO:
      {
         if(wrUmmUeCapRaCapDecCfmHdl(transCb,decCfm) != ROK)
         {
            initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
            RLOG_ARG0(L_ERROR, DBG_CRNTI, transCb->ueCb->crnti,
               "NHU_MSG_UERACAPINFO decode failed");
            RETVALUE(RFAILED);
         }
         break;
      }
      case NHU_MSG_UECAPINFO:
      {
         if(wrUmmUeCapUeCapDecCfmHdl(transCb,decCfm)!= ROK)
         {
            initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
            RLOG_ARG0(L_ERROR, DBG_CRNTI, transCb->ueCb->crnti,
               "NHU_MSG_UECAPINFO decode failed");
            RETVALUE(RFAILED);
         }
         break;
      }
   }

   RETVALUE(RFAILED);
}

/** @brief This function is Initial Context Setup Request RRC Timers 
 *         expiry functions.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtPrcRrcTmrExpiry 
 *
 *         Processing steps:
 *
 * @param[in]  transCb : transaction inforamation
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmInitCtxtPrcRrcTmrExpiry 
(
WrUmmTransCb                 *transCb
)
{
   WrInitCntxtSetupTransCb   *initCtxt = &(transCb->u.initCtxtCb);

   switch(transCb->msg->u.tmrExpiry.timerEvnt)
   { 
      case WR_TMR_RRC_CAPENQ:
      case WR_TMR_RECONFIGURE:
      case WR_TMR_RRC_SECUCMD:
      {
         initCtxt->state = WR_UMM_INIT_CTXT_ERROR;
         break;
      }
      default:
      {
          RLOG1(L_ERROR, "Received [%d] invalid Timer Event",
             transCb->msg->u.tmrExpiry.timerEvnt);
          RETVALUE(RFAILED);
      } 
   }
   RETVALUE(ROK);
}

/** @brief This function is called when any message related to Initial Context
 *         Setup is received by application (internal & external)
 *
 * @details
 *
 *     Function: wrUmmInitCtxtProcessor
 *
 *         Processing steps:
 *         - identify the different incoming message and call the appropriate 
 *           handler
 *         - incoming message can be,
 *           RRC Message
 *           Internal layer confirms (Scheduler, L1, DAM, RRC)
 *         - if any handler returns failure, then send initial context failure
 *           to MME
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmInitCtxtProcessor
(
WrUmmTransCb                 *transCb
)
{
   WrUmmIncMsg               *msg = transCb->msg;
   WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmCellCb               *cellCb = NULLP;
   WrUmmRatLst                ratLst;
   WrCellCb                  *emmCellCb;

   WR_UMM_GET_CELLCB(cellCb, transCb->ueCb->cellId);
   WR_GET_CELLCB(emmCellCb, transCb->ueCb->cellId);
   if(cellCb == NULLP || emmCellCb == NULLP)
   {
      RLOG1(L_ERROR, " Error in getting CELL CB for cellId = %lu ",
            (U32)ueCb->cellId);
      RETVALUE(RFAILED);
   }

   switch (msg->msgTyp)
   {
      /*  NOTE: TO SUPPORT WHEN NAS AUTHENTICATION IS PERFORMED  */
      /*             AND NOT PERFERMOED                               */
      case WR_UMM_S1_CON_CFM:
      case WR_UMM_S1_DAT_IND:
      {
         if (wrUmmInitCtxtPrcS1Pdu(transCb) != ROK)
         {
            break;
         }

         /* admission cntrl request to RRM is currently ignored */
         if (initCtxtCb->ueRadCapIe == NULLP)
         {
            /*For EUTRA, CDMA 1xRTT, GERAN_CS and GERAN_PS RAT Types
             * UTRA capabilities will be enquried on need basis */
            ratLst.noComp =  WR_NO_COMP_IN_UECAP;
            ratLst.ratType[0] = NhuRAT_TypeutraEnum;
            ratLst.ratType[1] = NhuRAT_Typcdma2000_1XRTTEnum;
            ratLst.ratType[2] = NhuRAT_Typgeran_csEnum;
            ratLst.ratType[3] = NhuRAT_Typgeran_psEnum;
            if(ROK != wrUmmUeCapSndUeCapEnq(transCb, &ratLst))
            {
               RLOG_ARG0(L_INFO, DBG_CRNTI,ueCb->crnti,"Not able to send UE "
                  "Capability Enquiry message");
               initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
            }
         }
         else
         {
            wrUmmInitCtxtPrcUeRadioCap(transCb);
         }
         break;
      }
      case WR_UMM_TMR_EXPIRY:
      {
           RLOG0(L_INFO, "wrUmmInitCtxtProcessor: Processing WR_UMM_TMR_EXPIRY");
           wrUmmInitCtxtPrcRrcTmrExpiry(transCb);
           break;
      }
      case WR_UMM_UU_IND_PDU:
      {
         wrUmmInitCtxtPrcRrcPdu(transCb);
         if (initCtxtCb->state == WR_UMM_INIT_CTXT_SEC_DONE)
         {
            transCb->u.initCtxtCb.rrmUeRecfgRspMsg = NULLP;
            wrUmmFillAndSndRrmUeRecfgReq(transCb);
         }
         else if (initCtxtCb->state == WR_UMM_INIT_CTXT_RRC_CFG_DONE)
         {
            /* UE history info - start */
            SGetEpcTime(&ueCb->ueAttachTime); 
            RLOG1(L_INFO, "wrUmmInitCtxtProcessor: ueCb->ueAttachTime[%lu]", \
                  ueCb->ueAttachTime);
            /* UE history info - end */
            /*ccpu00129900*/
            wrUmmInitCtxtCommitUeInfo(transCb);
            wrUmmInitCtxtSndRsp(transCb);
            /* ccpu00130088: Initialize ueAttachedtoMme to accept the re-establishment message*/
            transCb->ueCb->ueAttachedtoMme = TRUE;
#ifdef WR_RSYS_KPI
            /* CSG_DEV : Increment counter for the CSG attached UE here */
            if ((cellCb->csgAccessMode == WR_ENB_CLOSED_ACCESS)||
                 (cellCb->csgAccessMode == WR_ENB_HYBRID_ACCESS))
            {
               WR_INC_KPI_FUNC(wrIncKpiFrCsgUeAttachSuccd);
            }
#endif
            if(transCb->u.initCtxtCb.csFallBackInd.pres == TRUE)
            {
               /* perform the CSFB transaction creation */
               if(RFAILED == wrUmmCreatCsfbTrans(transCb))
               {
                  RLOG0(L_ERROR, "Creation of CSFB transaction FAILED");
                  RETVALUE(RFAILED);
               }
               wrUmmTransComplete(transCb);
            }
            else
            {
               /* As we choose RRC Connection release with redirection for CSFB,
                * post re-establishment DRX configuration is not required. But
                * if CCO is chosen, post re-establishment DRX needs to be
                * configured*/
               RmuUeRecfgRsp *rrmUeRecfgResp = transCb->u.initCtxtCb.rrmUeRecfgRspMsg->u.rrmUeRecfgResp;

               if((rrmUeRecfgResp->stUeRadioResCfg.bRadioResDedBitMap & 
                        RM_RCM_MAC_MAIN_CFG) &&
                     (rrmUeRecfgResp->stUeRadioResCfg.stMacMainCfg.bMacMainCfgBitMap & 
                      RM_RCM_MAC_MAIN_CFG_DRX))
               {
                  wrUmmInitCtxtDrxReCfgSchdUeRecfgReq(transCb); 
               }   
               else
               {
                  wrUmmRlsIncMsg(&initCtxtCb->rrmUeRecfgRspMsg);
                  {
                     /*DRX QCI*/                 
                     if((ueCb->isGBRBearerExists == NOT_IN_USE))
                     {
                        if(wrUmmStartAnr(ueCb,FALSE) != ROK)
                        {
                           RLOG0(L_ERROR,"Failed to Start the ANR");
                        }
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
                  wrUmmTransComplete(transCb);
               }
               RLOG0(L_DEBUG, "Inactivity req has been sent to dam");
               wrIfmDamStartInactivityTimer(transCb->ueCb->cellId,transCb->ueCb->crnti);  
            }
         }
         break;
      }
      case WR_UMM_RRC_CFG:
      {
#ifdef WR_TEST_CODE
         if ( wrTrgRrcReConTO == TRUE )
         {
           RLOG0(L_INFO, "Dropping WR_UMM_RRC_CFG!");
           RETVALUE(ROK);
         }
#endif
         wrUmmInitCtxtPrcRrcCfgCfm(transCb);
         break;
      }
      case WR_UMM_RRC_ENC_CFM:
      {
         wrUmmInitCtxtPrcRrcEncCfm(transCb);
         break;
      }
      case WR_UMM_RRC_DEC_CFM:
      {
         if (wrUmmInitCtxtPrcRrcDecCfm(transCb) != ROK)
         {
            break;
         }
         if (initCtxtCb->state == WR_UMM_INIT_CTXT_UECAP_DONE)
         {
            if(ueCb->uePucchPwr.pres)
            {
               if(wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti, WR_UMM_DCI_FORMAT_3A_SUPPORTED))
               {
                  ueCb->uePucchPwr.isFmt3a = cellCb->uePucchPwr.isFmt3a;
                  if (ueCb->uePucchPwr.isFmt3a)
                  {
                     WR_GET_PUCCH_FMT3A_TPC_RNTI_AND_IDX(ueCb->uePucchPwr.tpcRnti, ueCb->uePucchPwr.idx, ueCb, emmCellCb);
                  }
               }
            }
            if(ueCb->uePuschPwr.pres)
            {
               if(wrUmmUtilFeatureGrpInd(ueCb->cellId, ueCb->crnti, WR_UMM_DCI_FORMAT_3A_SUPPORTED))
               {
                  ueCb->uePuschPwr.isFmt3a = cellCb->puschDedicated.puschPwrCfg.isFmt3a;
                  if(ueCb->uePucchPwr.isFmt3a)
                  {
                     WR_GET_PUSCH_FMT3A_TPC_RNTI_AND_IDX(ueCb->uePuschPwr.tpcRnti, ueCb->uePuschPwr.idx, ueCb, emmCellCb);
                  } 
                }
             }  
#ifdef TENB_AS_SECURITY
            /* if security key generation completed prior to
             * ue capability acquisition then security mode command can be issued here */
            if((initCtxtCb->asKeyGenerated == TRUE) && (initCtxtCb->asSecModeInit == FALSE))
            {
               if( ROK != wrUmmSecSndSecModeCmd(transCb))
               {   
                  RLOG_ARG0(L_ERROR, DBG_CRNTI, transCb->ueCb->crnti,
                     "Not able to send Security Mode command message");
                  initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
                  break;
               }
               initCtxtCb->asSecModeInit = TRUE;
            }
#else
            if ( ROK != wrUmmSecSndSecModeCmd(transCb))
            {
               RLOG_ARG0(L_ERROR, DBG_CRNTI, transCb->ueCb->crnti,
                  "Not able to send Security Mode command message");
               initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
            }
#endif
         }
         break;
      }
      /* ccpu00129384: Re-Configure CTF with UE Category */
      case WR_UMM_PHY_CFG:
      {
         if(transCb->msg->u.phyCfm.status != CTF_CFG_CFM_OK)
         {
            RLOG_ARG0(L_ERROR, DBG_CRNTI, transCb->ueCb->crnti,"Phy Cfg failed");
            initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
         }
         break;
      }
      case WR_UMM_SCHD_CFG:
      {
         if (initCtxtCb->state == WR_UMM_INIT_CTXT_SCH_DRX_CFG_DONE)
         {
            wrUmmRlsIncMsg(&initCtxtCb->rrmUeRecfgRspMsg);
            /*DRX QCI*/                 
            if((ueCb->isGBRBearerExists == NOT_IN_USE))
            {
               RLOG0(L_DEBUG,
                     "GBR bearer is not exists in Initail Context setup");
               if(wrUmmStartAnr(ueCb,FALSE) != ROK)
               {
                  RLOG0(L_ERROR,"Failed to Start the ANR");
               }
            }
            wrUmmTransComplete(transCb);
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
         else
         {
            wrUmmInitCtxtPrcSchdCfgCfm(transCb);
            if (initCtxtCb->state == WR_UMM_INIT_CTXT_SCH_CFG_DONE)
            {
               wrUmmInitCtxtCfgTnls(transCb);
               RETVALUE(ROK);
            }
            /* initCtxtCb->state as WR_UMM_INIT_CTXT_ERROR is handled at outside
             * of the switch
             */
         }
         break;
      }
      case WR_UMM_DAM_CFM:
      {
         wrUmmInitCtxtProcTnlMgmtCfm(transCb);
         if (initCtxtCb->state == WR_UMM_INIT_CTXT_DAM_CFG_DONE)
         {
            wrUmmUpdateUeEmerState(transCb,cellCb);
            wrUmmInitCtxtPrepRrcMsgAndCfg(transCb);
         }
         break;
      }
      case WR_UMM_RRC_DAT_CFM:
      {
         /* This does not require any special action. Just ignore it      */
         break;
      }
      case WR_UMM_RRM_UERECFG_RESP: 
      {
         transCb->u.initCtxtCb.rrmUeRecfgRspMsg = transCb->msg;
         transCb->msg->refCnt++;
         /* HCSG_DEV */
         RmuUeRecfgRsp *rrmUeRecfgResp = transCb->msg->u.rrmUeRecfgResp;
         /*DRX QCI*/
         if(rrmUeRecfgResp->stStatusInfo.enStatus == RMU_SUCCESS)
         {
            if(rrmUeRecfgResp->isGBRBearerExists == TRUE)
            {
               ueCb->isGBRBearerExists = RAB_ESTABLISHED;
            }

            if(rrmUeRecfgResp->bRecfgBitMap & RMU_RECFG_RSP_ERAB_CONFIG)
            {
               wrUmmInitUpdateRabAdmInfo(transCb);
            }

            /* Fix : syed Incorrect check and also the logic
             * needed some explanation */
            /* If Number of UE connections goes beyond EnbApp threshold,
             * they are still allowed in the system but are not given
             * CQI respouces and their AMBRs are set to as low as 1000bps*/
            if(((rrmUeRecfgResp->stUeRadioResCfg.stPhyCfgDed.bPhyCfgDedResBitMap) &
                     (U8)RM_RCM_PHY_DED_RES_CFG_CQI) == 0)
            {
               initCtxtCb->dlAmbr = WR_UMM_DFLT_SIGNAL_UE_AMBR;
               initCtxtCb->ulAmbr = WR_UMM_DFLT_SIGNAL_UE_AMBR;
            }

            wrUmmInitCtxtCfgSchd(transCb); /*moved it here for LC*/
            /* ccpu00129384: Re-Configure CTF with UE Category */
            wrUmmInitCtxtCfgPhy(transCb);
         }  
         else 
         {
            if (rrmUeRecfgResp->stStatusInfo.enStatus == RMU_REDIRECT)
            {
               transCb->ueCb->redirectUe = TRUE;
            }
            /* HCSG_DEV */
            RLOG2(L_INFO,"UE Reconfiguration failed at RRM. Status [%d] "
                  "Cause [%d] ", rrmUeRecfgResp->stStatusInfo.enStatus,
                  rrmUeRecfgResp->stStatusInfo.enCause);
            initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
            initCtxtCb->failCause =
               SztCauseRadioNwradio_resources_not_availableEnum;
         }
         break;
      }
      case WR_UMM_RRC_ERR_IND:
      {
         initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
         break;
      }
#ifdef TENB_AS_SECURITY
      case WR_UMM_PHY_KDF:
      {
         /*ccpu00127802*/
         RLOG0(L_DEBUG, "AS KEY GENERATION SUCESS");

         if(msg->u.kdfCfm.ctfKdfCfm->keyDerType == CTF_KDF_TYPE1)
         {
            /* Fix for ccpu00126700 */
            if(initCtxtCb->asKeyGenerated == FALSE)
            {
               /* ccpu00126532 */
               /*Store initial AS security keys*/
#ifndef LTE_QCOM
#ifndef SS_RBUF
               cmMemcpy(initCtxtCb->secCfg->cpCiphKey,
                     msg->u.kdfCfm.ctfKdfCfm->u.kEnbInf.cpCiphKey,
                     CPJ_CIPHER_KEY_LEN);
               cmMemcpy(initCtxtCb->secCfg->upCiphKey,
                     msg->u.kdfCfm.ctfKdfCfm->u.kEnbInf.upCiphKey,
                     CPJ_CIPHER_KEY_LEN);
               cmMemcpy(initCtxtCb->secCfg->intgKey,
                     msg->u.kdfCfm.ctfKdfCfm->u.kEnbInf.intKey,
                     CPJ_INTG_KEY_LEN);

               wrPlatSecExtractSecKeys2( transCb, CTF_KDF_TYPE2);
#endif // SS_RBUF
#endif //  #ifndef QCOM
               /* Fix for ccpu00126700 */
               /* UE capability acquisition completed prior to
                * security key generation, so doing security mode command */ 
               initCtxtCb->asKeyGenerated = TRUE;
               if((initCtxtCb->asSecModeInit == FALSE) &&
                    (initCtxtCb->state == WR_UMM_INIT_CTXT_UECAP_DONE))
               {
                  /*Trigger RRC Mode command here*/
                  if(ROK != wrUmmSecSndSecModeCmd(transCb))
                  {
                     RLOG_ARG0(L_ERROR, DBG_CRNTI, transCb->ueCb->crnti,
                        "Not able to send Security Mode command message");
                     initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
                  }
                  initCtxtCb->asSecModeInit = TRUE;
               }
            }
            else
            {
               /*store horizontally derived AS security keys*/
               cmMemcpy(initCtxtCb->hzSecCfg->cpCiphKey,
                     msg->u.kdfCfm.ctfKdfCfm->u.kEnbInf.cpCiphKey,
                     CPJ_CIPHER_KEY_LEN);
               cmMemcpy(initCtxtCb->hzSecCfg->upCiphKey,
                     msg->u.kdfCfm.ctfKdfCfm->u.kEnbInf.upCiphKey,
                     CPJ_CIPHER_KEY_LEN);
               cmMemcpy(initCtxtCb->hzSecCfg->intgKey,
                     msg->u.kdfCfm.ctfKdfCfm->u.kEnbInf.intKey,CPJ_INTG_KEY_LEN);
            }
         }
         else if(msg->u.kdfCfm.ctfKdfCfm->keyDerType == CTF_KDF_TYPE2)
         {
            WR_ALLOC(&(initCtxtCb->hzSecCfg), sizeof(WrUeSecCfg));
            if (initCtxtCb->hzSecCfg == NULLP)
            {
               RLOG0(L_FATAL, "Memory allocation failed.");
               RETVALUE(RFAILED);
            }

            /* Copy the security capabilities and the selected algorithm */
            initCtxtCb->hzSecCfg->ciphAlgo = initCtxtCb->secCfg->ciphAlgo;
            initCtxtCb->hzSecCfg->intgAlgo = initCtxtCb->secCfg->intgAlgo;
            initCtxtCb->hzSecCfg->encrypCap = initCtxtCb->secCfg->encrypCap;
            initCtxtCb->hzSecCfg->integCap = initCtxtCb->secCfg->integCap;

            cmMemcpy(initCtxtCb->hzSecCfg->kENB,
                  transCb->msg->u.kdfCfm.ctfKdfCfm->u.kEnbStarInf.secKey,
                  CPJ_SEC_KEY_LEN);
#ifndef SS_RBUF
            wrPlatSecExtractSecKeys2( transCb, CTF_KDF_TYPE1);
#endif // SS_RBUF           
         }
         break;
      }
#endif

      default:
      {
         initCtxtCb->state = WR_UMM_INIT_CTXT_ERROR;
         RLOG1(L_ERROR, "Invalid message[%u]", msg->msgTyp);
      }
   }

   if (initCtxtCb->state == WR_UMM_INIT_CTXT_ERROR)
   {
      /* We came across an error that is irrecoverable from. We should   */
      /* stop the procedure and return FAILURE to the MME. MME should    */
      /* respond with CONTEXT RELEASE which will eventually release the  */
      /* resources.                                                      */
      if (initCtxtCb->rrmUeRecfgRspMsg != NULLP)
      {
         wrUmmRlsIncMsg(&initCtxtCb->rrmUeRecfgRspMsg);
      }

      wrUmmInitCtxtSndFail(transCb); 
      wrUmmTransComplete(transCb);
   }

   RETVALUE(ROK);
}

/** @brief This function is Initial Context Setup Request specific release 
 *         function.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtTransRel
 *
 *         Processing steps:
 *         - free the rab information
 *         - free the security information
 *         - free the stored RAB Setup Request message
 *
 * @param[in]  transCb : transaction inforamation
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmInitCtxtTransRel
(
WrUmmTransCb                 *transCb
)
{
   WrInitCntxtSetupTransCb   *initCtxtCb = &(transCb->u.initCtxtCb);

   /* can be based on state */
   wrStopTransTmr(transCb,WR_TMR_RRC_CAPENQ);
   wrStopTransTmr(transCb,WR_TMR_RRC_SECUCMD);
   if (initCtxtCb->numErabs)
   {
      WR_FREE(initCtxtCb->erabList, initCtxtCb->numErabs * sizeof(WrErabInfo));
      initCtxtCb->numErabs = 0;
   }
   if(NULLP != initCtxtCb->secCfg)
   {
      WR_FREE(initCtxtCb->secCfg, sizeof(WrUeSecCfg));
   }
#ifdef TENB_AS_SECURITY
   if(NULLP != initCtxtCb->hzSecCfg)
   {
      WR_FREE(initCtxtCb->hzSecCfg, sizeof(WrUeSecCfg));
   }
#endif

   wrUmmRlsIncMsg(&initCtxtCb->msg);

   RETVALUE(ROK);
}

/** @brief This function is called when any message related to a RRC Re-establsiment
 *         transaction during Initial Context Setup transaction. 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmDefInitCtxtIncReestProcessor 
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDefInitCtxtIncReestProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{


   /* As initial context setup transaction is ongoing 
    * irrespective of the cause stop the send  Initial Context Setup Failure to MME
    * send RRC REESTABLISHMENT REJECT to UE
    */
   wrStopTransTmr(transCb,WR_TMR_RECONFIGURE);
   wrUmmInitCtxtSndFail(transCb);
   transCb->state = WR_UMM_TRANS_DONE;
   incTrans->u.rrcReEstabTransCb.state = WR_UMM_REESTAB_REJECTED;
   incTrans->u.rrcReEstabTransCb.rejectCause = WR_UMM_REJ_CAUSE_INIT_CTXT_GOING_ON;
   RETVALUE(ROK);
}

/** @brief This function is called when any message related to ue context release
 *         transaction during Initial Context Setup transaction. 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmDefInitCtxtIncCtxtRelProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  incTrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmDefInitCtxtIncCtxtRelProcessor
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
   wrStopUeTmr(transCb->ueCb,WR_TMR_INITIAL_CTX_REQ);
   wrUmmStopTransTimers(transCb);
   RETVALUE(ROK);
}

/** @brief This function is called when any message related to a particular
 *         transaction during Initial Context transaction. 
 *         This is the main function that determines
 *         next course of action based on the message received.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtNewTransProcessor
 *
 *         Processing steps:
 *
 * @param[in]  transCb    : transaction information
 * @param[in]  inctrans   : incoming transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmInitCtxtNewTransProcessor 
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTrans
)
{
     switch(incTrans->transTyp)
     {
       case WR_UMM_RRC_REESTAB_TRANS: 
       {
          wrUmmDefInitCtxtIncReestProcessor(transCb,incTrans);
          break;
       } 

        case WR_UMM_ERAB_SETUP_TRANS:
       {
          RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Dropping "
                "incoming ERAB Setup transaction:%d ",incTrans->transTyp);
          RETVALUE(WR_UMM_TRANS_FAIL);
          break;
       }

        case WR_UMM_S1_SRC_HO_TRANS:
        case WR_UMM_X2_SRC_HO_TRANS:
       {
          RLOG_ARG1(L_INFO, DBG_CRNTI, transCb->ueCb->crnti,"Dropping "
                "incoming Handover transaciton:%d ",incTrans->transTyp);
          RETVALUE(WR_UMM_TRANS_FAIL);
          break;
       }

       case WR_UMM_CTXT_REL_TRANS:
       {
          wrUmmDefInitCtxtIncCtxtRelProcessor(transCb,incTrans);
          transCb->state = WR_UMM_TRANS_DONE;
          break;
       }
      default:
       {
          RLOG1(L_INFO,"Ignoring Incoming Transaction Type[%d]",
                 incTrans->transTyp);
         break;
       } 
     }   
     RETVALUE(WR_UMM_TRANS_OK);
}


PRIVATE S16 wrUmmRadResCfgModRrcRrcPdu
(
WrUmmTransCb                 *transCb
)
{
   S16                       ret;
   WrUmmIncMsg               *msg = transCb->msg;
   NhuDatIndSdus             *uuIndPdu;
   NhuUL_DCCH_MsgTyp         *ulDcchMsg;
   NhuUL_DCCH_MsgTypc1       *rrcMsg;
   U32                       rrcTransId = 0;

   /* received the response for UE cap enquiry */
   uuIndPdu = msg->u.uuIndPdu;
   ulDcchMsg = &(uuIndPdu->sdu.m.ulDcchMsg.dcchMsg.message);
   rrcMsg = &(ulDcchMsg->val.c1);
   switch (rrcMsg->choice.val)
   {
      case C1_RRCCONRECFGNCOMPL:
      {
         /* Stop the RRC reconfig timer */
         wrStopTransTmr(transCb, WR_TMR_RECONFIGURE);

         rrcTransId = rrcMsg->val.rrcConRecfgnCompl.rrc_TrnsxnId.val;
         ret = wrUmmRelRrcTransId(transCb->ueCb, C1_RRCCONRECFGN, rrcTransId);
         if (ret != ROK)
         {
            /*ccpu00127802*/
            RLOG0(L_ERROR, "Release of transaction ID failed");
         }
         break;
      }
     default:
      {
         RLOG0(L_ERROR, "Invalid Msg transaction");
      }


   }
   RETVALUE(ROK);
}

/** @brief This function is responsible for preparing the sending the RRC
 *         reconfiguration message. The lower layer configurations are also 
 *         filled here.
 *
 * @details
 *
 *     Function: wrUmmRadResModPrepRrcMsgAndCfg
 *
 *         Processing steps:
 *         - allocate a new RRC data request event,
 *         - fill the RRC interface header
 *         - get a new free tranaction for the reconfiguration message
 *         - invoke wrUmmRrcCreateRrcReconfigMsg, to create a new 
 *           reconfiguration message
 *         - invoke wrUmmRrcPrepRadioResourceDed, to prepare the Radio
 *           Resource Dedicated IE
 *         - invoke wrUmmInitCtxtRrcReconfigAddSrb, to fill the DRB
 *           information to the ASN IE
 *         - for every RAB that is admitted,
 *           - invoke wrUmmInitCtxtRrcReconfigAddDrb, to fill the DRB 
 *             information to the ASN IE
 *         - for every RAB that is admitted,
 *           - copy the dedicated NAS information to the ASN IE
 *         - invoke wrUmmInitCtxtFillRrcCfg to fill the lower layer 
 *           configuration for the DRB(s)
 *         - send the event to RRC
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmRadResModPrepRrcMsgAndCfg
(
WrUmmTransCb                 *transCb
)
{
   NhuDatReqSdus                             *recfgMsg;
   WrUeCb                                    *ueCb = transCb->ueCb;
   NhuDL_DCCH_Msg                            *dcchMsg;
   NhuRRCConRecfgn                           *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs                    *recfgIEs;
   NhuRadioResourceConfigDedicated           *rrcCfgDed;
   U8                                        transId;

   NhuUlnkPowerCntrlDedicated                *uplnkPwrCntrl;

   TRC2(wrUmmRadResModPrepRrcMsgAndCfg)
   WR_ALLOCEVNT(&recfgMsg, sizeof(NhuDatReqSdus));
   if (recfgMsg == NULLP)
   {
      RETVALUE(RFAILED);
   }
   recfgMsg->hdr.cellId  = ueCb->cellId;
   recfgMsg->hdr.ueId    = ueCb->crnti;
   recfgMsg->hdr.transId = transCb->transId | C1_RRCCONRECFGN;

   /* Fix for ccpu00126824:if wrUmmGetRrcTransId fails, recfgMsg is freed*/
   /* Fetch an RRC transaction id for this procedure.                     */
   if (wrUmmGetRrcTransId(ueCb, C1_RRCCONRECFGN, 
                      WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
                      &transId) != ROK)
   {
      
      RLOG_ARG0(L_ERROR, DBG_CRNTI,ueCb->crnti,"Max RRC Transaction already "
         "reached");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }

   /* Create the reconfiguration message which is constructued excluding  */
   /* the actual procedure related details which will be added further    */
   if (wrUmmRrcCreateRrcReconfigMsg(recfgMsg, transId) != ROK)
   {
      RETVALUE(RFAILED);
   }
   
   /* Prepare the radio resources to exclude all the IEs present in the   */
   /* the IE. Those IEs are filled subsequently                           */
   dcchMsg     = &(recfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   recfgIEs    = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   rrcCfgDed   = &(recfgIEs->radioResourceConfigDedicated);
   if (ueCb->ulPwrDedCtrl.p0UePucch == 0)
   {
      ueCb->ulPwrDedCtrl.p0UePucch = 1;
   }
   else
   {
      ueCb->ulPwrDedCtrl.p0UePucch = 0;
   }
   wrUmmRrcPrepRadioResourceDed(rrcCfgDed);
   
   WR_FILL_TKN_UINT(rrcCfgDed->physicalConfigDedicated.pres,PRSNT_NODEF);
   
   uplnkPwrCntrl = &(rrcCfgDed->physicalConfigDedicated.
          uplinkPowerCntrlDedicated); 
   WR_FILL_TKN_UINT(uplnkPwrCntrl->pres, PRSNT_NODEF);
   WR_FILL_TKN_UINT(uplnkPwrCntrl->p0_UE_PUSCH,
          ueCb->ulPwrDedCtrl.p0UePusch);
   WR_FILL_TKN_UINT(uplnkPwrCntrl->deltaMCS_Enabled,\
               ueCb->ulPwrDedCtrl.deltaMcsEnabled); /*0, MIMO*/
   WR_FILL_TKN_UINT(uplnkPwrCntrl->accumulationEnabled,\
               ueCb->ulPwrDedCtrl.accumlationEnabled); /*0, MIMO*/
   WR_FILL_TKN_UINT(uplnkPwrCntrl->p0_UE_PUCCH,\
               ueCb->ulPwrDedCtrl.p0UePucch); /*0, MIMO*/
   WR_FILL_TKN_UINT(uplnkPwrCntrl->pSRS_Offset,\
               ueCb->ulPwrDedCtrl.pSrsOff);  /*7, MIMO*/
   WR_FILL_TKN_UINT(uplnkPwrCntrl->filterCoefficient,\
               ueCb->ulPwrDedCtrl.filterCoeff);   /*4, MIMO*/

   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending RRCConnectionReconfiguration"
         " for RadioResource Modification");
   if (RFAILED == wrIfmRrcSndReqToUe(recfgMsg))
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe failed.");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }

  RETVALUE(ROK);
}


/** @brief This function is responsible for preparing the sending the RRC
 *         reconfiguration message. The lower layer configurations are also 
 *         filled here.
 *
 * @details
 *
 *     Function: wrUmmRadResConfModProcessor
 *
 *         Processing steps:
 *         - allocate a new RRC data request event,
 *         - fill the RRC interface header
 *         - get a new free tranaction for the reconfiguration message
 *         - invoke wrUmmRrcCreateRrcReconfigMsg, to create a new 
 *           reconfiguration message
 *         - invoke wrUmmRrcPrepRadioResourceDed, to prepare the Radio
 *           Resource Dedicated IE
 *         - invoke wrUmmInitCtxtRrcReconfigAddSrb, to fill the DRB
 *           information to the ASN IE
 *         - for every RAB that is admitted,
 *           - invoke wrUmmInitCtxtRrcReconfigAddDrb, to fill the DRB 
 *             information to the ASN IE
 *         - for every RAB that is admitted,
 *           - copy the dedicated NAS information to the ASN IE
 *         - invoke wrUmmInitCtxtFillRrcCfg to fill the lower layer 
 *           configuration for the DRB(s)
 *         - send the event to RRC
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmRadResConfModProcessor
(
WrUmmTransCb                *transCb 
)
{
   WrUmmIncMsg              *msg  = transCb->msg;
   WrUeCb                   *ueCb = transCb->ueCb;
   WrUmmRadResMod           *radResModInfo;
   U32                      rrcTransId;
   radResModInfo = &transCb->u.radResMod;
   switch (msg->msgTyp)
   {
      case WR_UMM_INTERNAL_MSG:
      {
         radResModInfo->bitMsk = msg->u.internalMsg->u.radResMod.bitMsk;
         switch(msg->u.internalMsg->intMsgType)
         {
            case WR_UMM_INTMSG_RAD_RES_MOD_MSG:
            {
               if(wrUmmRadResModPrepRrcMsgAndCfg(transCb) != ROK)
               {
                  wrUmmTransComplete(transCb);
               }
               else
               {        
                  /* Start RRC Reconfigure timer */
                  wrStartTransTmr(transCb,WR_TMR_RECONFIGURE, 
                                   wrEmmGetRrcReCfgTmrVal(ueCb->cellId));
               }
                
               break;
            }
            case WR_UMM_INTMSG_MOD_SR_CQI_RES_MSG:
            {
               /* CR ccpu00144525*/
               if(wrUmmSndReqToStartTATimer(transCb) != ROK)
               {
                  wrUmmTransComplete(transCb);
               }
               break;
            }
                  /* SPS changes starts */
               case WR_UMM_INTMSG_UNSET_SR_MASK:
                  {
                     /* The status request from Scheduler is received for the
                      * SR failure due to SR mask presence sent to UE. This means
                      * UE doesnot liked SR mask. Thus disabling the SR Mask at UE. */
                     if( wrUmmUnSetSrMaskPrepRrcMsgAndCfg(transCb) != ROK)
                     {
                        wrUmmTransComplete(transCb);
                     }
                     else
                     {        
                        /* Start RRC Reconfigure timer */
                        wrStartTransTmr(transCb,WR_TMR_RECONFIGURE, 
                              wrEmmGetRrcReCfgTmrVal(ueCb->cellId));
                     }
                     break;
                  }
                  /* SPS changes ends */
               default:
                  {
                     RLOG0(L_ERROR, "Invalid intMsgType.");
               break;
            }
         }
         break;
      }
      case WR_UMM_SCHD_CFG:
      {
         if( wrUmmCqiSrRadResModPrepRrcMsgAndCfg(transCb) != ROK)
         {
            wrUmmTransComplete(transCb);
         }
         else
         {        
            /* Start RRC Reconfigure timer */
            wrStartTransTmr(transCb,WR_TMR_RECONFIGURE, 
                  wrEmmGetRrcReCfgTmrVal(ueCb->cellId));
         }
         break;
      }
      case WR_UMM_UU_IND_PDU:
      {
         wrUmmRadResCfgModRrcRrcPdu(transCb);

         radResModInfo->bitMsk |= WR_UMM_RAD_RES_MOD_UU_IND_RCVD;

/*   if(radResModInfo->bitMsk & WR_UMM_RAD_RES_MOD_RRC_DAT_CFM_RCVD)
   { */
     wrUmmTransComplete(transCb);
/* } */
         /* Start Timer Again  */
         if(radResModInfo->bitMsk & WR_UMM_RAD_RES_MOD_ULPOWER) 
         {
           //wrStartUeTmr(ueCb,WR_TMR_RAD_RES_MOD_TMR, 
               //WR_TMR_RAD_RES_MOD_TMR_VAL);
         }
         break;
      }
      case WR_UMM_RRC_DAT_CFM:
      {
         radResModInfo->bitMsk |= WR_UMM_RAD_RES_MOD_RRC_DAT_CFM_RCVD;

   if(radResModInfo->bitMsk & WR_UMM_RAD_RES_MOD_UU_IND_RCVD)
   {
     wrUmmTransComplete(transCb);
   }
         
         break;
      }
      case WR_UMM_TMR_EXPIRY:
      {
         if( wrUmmGetOnGoingRrcTransId(ueCb, C1_RRCCONRECFGN, 
                    WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
                    &rrcTransId) == ROK)
         {
             RLOG_ARG2(L_INFO, DBG_CRNTI,ueCb->crnti, " RRC Transaction[%lu]"
                " Release On Recipt of Tmr Expiry Int Msg Type[%d]",
                rrcTransId, radResModInfo->bitMsk);
             wrUmmRelRrcTransId(ueCb, C1_RRCCONRECFGN, rrcTransId);
             wrUmmTransComplete(transCb);
         }
         else
         {
             RLOG_ARG1(L_ERROR, DBG_CRNTI,ueCb->crnti,"Invalid Tranasaction "
                "Received[%ld]", WR_GET_TRANS_FRM_TRANSID(transCb->transId));
         }
         break;
      }
      default:
      {
         RLOG0(L_ERROR, "Invalid msgType.");
           break;
      }
   }
   RETVALUE(ROK);
}
/** @brief This function is Initial Context Setup Request specific release 
 *         function.
 *
 * @details
 *
 *     Function: wrUmmInitCtxtTransRel
 *
 *         Processing steps:
 *         - free the rab information
 *         - free the security information
 *         - free the stored RAB Setup Request message
 *
 * @param[in]  transCb : transaction inforamation
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmRadResConfModTransRel
(
WrUmmTransCb                 *transCb
)
{

   /* can be based on state */

   RETVALUE(ROK);
}

/** @brief This function is Tx mode change transaction 
 *  release function.
 *
 * @details
 *
 *     Function: wrUmmTxModeChngTransRel 
 *
 * @param[in]  transCb : transaction information
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmTxModeChngTransRel 
(
WrUmmTransCb                 *transCb
)
{
   wrUmmRlsIncMsg(&(transCb->u.txModeChngTrans.msg));
   RETVALUE(ROK);
}

PUBLIC S16 wrUmmCqiReptTmrExp
(
WrUeCb                *ueCb 
)
{
   if (wrUmmSndCtxtRelReq(ueCb->crnti, ueCb->cellId,
          WR_UMM_UE_LOST) != ROK)
   {
      /* Log error */
      RLOG0(L_ERROR, "wrUmmSndCtxtRelReqn Failed");
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
/* SPS changes starts */
/** @brief This function is responsible for preparing the sending the RRC
 *         reconfiguration message. The lower layer configurations are also 
 *         filled here.
 *
 * @details
 *
 *     Function:wrUmmUnSetSrMaskPrepRrcMsgAndCfg 
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmUnSetSrMaskPrepRrcMsgAndCfg 
(
WrUmmTransCb                 *transCb
)
{
   U8                                        idx =0;
   U8                                        rbIdx = 0;
   NhuDatReqSdus                             *recfgMsg;
   WrUeCb                                    *ueCb = transCb->ueCb;
   NhuDL_DCCH_Msg                            *dcchMsg;
   NhuRRCConRecfgn                           *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs                    *recfgIEs;
   NhuRadioResourceConfigDedicated           *rrcCfgDed;
   NhuDRB_ToAddMod                           *drbIe = NULLP;
   U8                                        transId=0;

   TRC2(wrUmmUnSetSrMaskPrepRrcMsgAndCfg)
   
   WR_ALLOCEVNT(&recfgMsg, sizeof(NhuDatReqSdus));
   if (recfgMsg == NULLP)
   {
      RETVALUE(RFAILED);
   }
   recfgMsg->hdr.cellId  = ueCb->cellId;
   recfgMsg->hdr.ueId    = ueCb->crnti;
   recfgMsg->hdr.transId = transCb->transId | C1_RRCCONRECFGN;

   /* Fetch an RRC transaction id for this procedure.                     */
   if (wrUmmGetRrcTransId(ueCb, C1_RRCCONRECFGN, 
            WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
            &transId) != ROK)
   {
      /*ccpu00127802*/
      RLOG0(L_ERROR, "wrUmmUnSetSrMaskPrepRrcMsgAndCfg: Max RRC Transaction already reached");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }
   /* Create the reconfiguration message which is constructued excluding  */
   /* the actual procedure related details which will be added further    */
   wrUmmRrcCreateRrcReconfigMsg(recfgMsg, transId);

   /* Prepare the radio resources to exclude all the IEs present in the   */
   /* the IE. Those IEs are filled subsequently                           */
   dcchMsg     = &(recfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   recfgIEs    = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   rrcCfgDed   = &(recfgIEs->radioResourceConfigDedicated);

   wrUmmRrcPrepRadioResourceDed(rrcCfgDed);

   wrFillTknU16(&rrcCfgDed->drb_ToAddModLst.noComp, ueCb->rbInfo.numOfRbCfgd);
   /* Allocate memory to include each RAB into the message towards the UE */
   WR_GET_MEM((&recfgMsg->memCp), (sizeof(NhuDRB_ToAddMod) * ueCb->rbInfo.numOfRbCfgd),
         &(rrcCfgDed->drb_ToAddModLst.member));
   if (rrcCfgDed->drb_ToAddModLst.member == NULLP)
   {
      RETVALUE(RFAILED);
   }
   for (idx = 0; (idx < WR_MAX_DRBS) && (rbIdx < ueCb->rbInfo.numOfRbCfgd); idx++)
   {
      if(ueCb->rbInfo.rab[idx].inUse == TRUE)
      {
         RLOG3(L_DEBUG, "wrUmmUnSetSrMaskPrepRrcMsgAndCfg: rbInfo info present:: idx [%u] erabId [%u] rbId [%u]\n",
                  idx, ueCb->rbInfo.rab[idx].eRABId, ueCb->rbInfo.rab[idx].rbId);
         drbIe   = rrcCfgDed->drb_ToAddModLst.member + rbIdx;

         wrUmmRrcReconfigFillModDrb(ueCb, ueCb->spsCfg.spsCfgUl.bIsSpsEnabled, drbIe, (U8)ueCb->rbInfo.rab[idx].eRABId,
               ueCb->rbInfo.rab[idx].rbId ,ueCb->rbInfo.rab[idx].lchId ,
               (U8)ueCb->rbInfo.rab[idx].qci);
         /* UL PBR changes gbr.ul is in kbps so gbr.ul/8 is done here*/
         wrUmmRrcReconfigFillulPbr(drbIe, ueCb->rbInfo.rab[idx].gbr.ul/8,
               ueCb->rbInfo.rab[idx].qci, ueCb->spsCfg.spsCfgUl.bIsSpsEnabled); 
         rbIdx++;
      }
   }
   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending RRCConnectionReconfiguration"
         " to Unset SR Mask");
   if (RFAILED == wrIfmRrcSndReqToUe(recfgMsg))
   {
      RLOG0(L_ERROR, "wrUmmUnSetSrMaskPrepRrcMsgAndCfg: wrIfmRrcSndReqToUe failed");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }
   RETVALUE(ROK);
}
/* SPS changes ends */

/** @brief This function is responsible for preparing the sending the RRC
 *         reconfiguration message. The lower layer configurations are also 
 *         filled here.
 *
 * @details
 *
 *     Function:wrUmmCqiSrRadResModPrepRrcMsgAndCfg 
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmCqiSrRadResModPrepRrcMsgAndCfg 
(
WrUmmTransCb                 *transCb
)
{
   NhuDatReqSdus                             *recfgMsg;
   WrUeCb                                    *ueCb = transCb->ueCb;
   NhuDL_DCCH_Msg                            *dcchMsg;
   NhuRRCConRecfgn                           *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs                    *recfgIEs;
   NhuRadioResourceConfigDedicated           *rrcCfgDed;
   NhuCQI_ReportPeriodicsetup                *cqiRepPerSetup = NULLP;
   NhuSoundingRS_UL_ConfigDedicated          *soundngRsUlCfg = NULLP;
   NhuSchedulingRqstConfig                   *schedulingRqstConfig = NULLP;
   NhuCQI_ReportConfig             *cqiRprtng = NULLP;
   U8                                        transId;


   TRC2(wrUmmRadResModPrepRrcMsgAndCfg)
   WR_ALLOCEVNT(&recfgMsg, sizeof(NhuDatReqSdus));
   if (recfgMsg == NULLP)
   {
      RETVALUE(RFAILED);
   }
   recfgMsg->hdr.cellId  = ueCb->cellId;
   recfgMsg->hdr.ueId    = ueCb->crnti;
   recfgMsg->hdr.transId = transCb->transId | C1_RRCCONRECFGN;

   /* Fetch an RRC transaction id for this procedure.                     */
   if (wrUmmGetRrcTransId(ueCb, C1_RRCCONRECFGN, 
                      WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
                      &transId) != ROK)
   {
      /*ccpu00127802*/
      RLOG_ARG0(L_ERROR, DBG_CRNTI,ueCb->crnti,"Max RRC Transaction already "
         "Reached");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }
   /* Create the reconfiguration message which is constructued excluding  */
   /* the actual procedure related details which will be added further    */
   if (wrUmmRrcCreateRrcReconfigMsg(recfgMsg, transId) != ROK)
   {
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }
   
   /* Prepare the radio resources to exclude all the IEs present in the   */
   /* the IE. Those IEs are filled subsequently                           */
   dcchMsg     = &(recfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   recfgIEs    = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   rrcCfgDed   = &(recfgIEs->radioResourceConfigDedicated);
   wrUmmRrcPrepRadioResourceDed(rrcCfgDed);
    
  
   rrcCfgDed->physicalConfigDedicated.pres.pres = PRSNT_NODEF;
   /*   tpc-PDCCH-ConfigPUCCH  empty 
          *   PDCCH-PUCCH Configuration */
   rrcCfgDed->physicalConfigDedicated.\
         tpc_PDCCH_ConfigPUCCH.choice.pres = NOTPRSNT;
  

   /* tpc-PDCCH-ConfigPUSCH 
    * PDCCH-PUSCH Configuration */
   rrcCfgDed->physicalConfigDedicated.tpc_PDCCH_ConfigPUSCH.\
                choice.pres = NOTPRSNT;
   
   
   /* CQI Reporting Related Information */
   if(ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_CQI)
   {
     cqiRprtng = &(rrcCfgDed->physicalConfigDedicated.cqi_ReportConfig);
     WR_FILL_TKN_UINT(cqiRprtng->pres, PRSNT_NODEF);
     if (ueCb->cqiRptCfg.aperCqiEnbld)
     {
        WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportModeAperiodic,\
            ueCb->cqiRptCfg.cqiAperiodicMode);
     }
     WR_FILL_TKN_UINT(cqiRprtng->nomPDSCH_RS_EPRE_Offset,\
         ueCb->cqiRptCfg.nomPdschRsEpreOff);
     WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.choice, \
         CQI_REPORTPERIODIC_SETUP);

     cqiRprtng->cqi_ReportPeriodic.choice.pres = PRSNT_NODEF;

     /* NhuCQI_ReportPeriodicsetup */ 
     cqiRepPerSetup = &(cqiRprtng->cqi_ReportPeriodic.val.setup);

     WR_FILL_TKN_UINT(cqiRepPerSetup->pres, PRSNT_NODEF);
     WR_FILL_TKN_UINT(cqiRepPerSetup->cqi_PUCCH_ResourceIdx,\
           ueCb->cqiRptCfg.cqiPucchRsrcIdx);

     WR_FILL_TKN_UINT(cqiRepPerSetup->cqi_pmi_ConfigIdx,\
           ueCb->cqiRptCfg.cqipmiCfgIdx);
     WR_FILL_TKN_UINT(cqiRepPerSetup->cqi_FormatIndicatorPeriodic.choice,\
         CQI_FORMATINDICATORPERIODIC_WIDEBANDCQI);
     WR_FILL_TKN_UINT(cqiRepPerSetup->simultaneousAckNackAndCQI,\
          ueCb->cqiRptCfg.simulAckNackAndCQI);

     /*TENB_BRDCMTDD_TM4*/
     if  (ueCb->antennaInfo.txmode == CTF_UE_TX_ANT_MODE_TM3 || ueCb->antennaInfo.txmode == CTF_UE_TX_ANT_MODE_TM4)
     {   
       WR_FILL_TKN_UINT(cqiRprtng->cqi_ReportPeriodic.val.setup.\
            ri_ConfigIdx, ueCb->cqiRptCfg.riCfgIdx);
     }
   }
   /* Sounding Rs Uplink Configuration */
   soundngRsUlCfg = &(rrcCfgDed->physicalConfigDedicated.\
         soundingRS_UL_ConfigDedicated);

   soundngRsUlCfg->choice.pres = NOTPRSNT;

   /* Scheduling Request Configuration */
   if(ueCb->bResCfgBitMap &  WR_UE_RADIO_RES_CFG_SR)
   {      
     schedulingRqstConfig = &(rrcCfgDed->physicalConfigDedicated.\
         schedulingRqstConfig);

     WR_FILL_TKN_UINT(schedulingRqstConfig->choice,\
         SCHEDULINGRQSTCONFIG_SETUP);
     WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.pres, PRSNT_NODEF);
     WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_PUCCH_ResourceIdx,\
            ueCb->srCfg.srPUCCHResIdx);

     /* This function is called during Ctf Cfg Req 
      * veGetSRCfgIdx(&schedulingRqstConfig->val.setup.sr_ConfigIdx);
      * and the cfg idx is derived then */

     WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.sr_ConfigIdx,\
            ueCb->srCfg.srCfgIdx);

     WR_FILL_TKN_UINT(schedulingRqstConfig->val.setup.dsr_TransMax,\
            ueCb->srCfg.enDsrTransMax);
     /*ccpu00127802*/
     RLOG_ARG4(L_INFO, DBG_CRNTI,ueCb->crnti," Reconfigured after CRNTI Ind "
       "from MAC CqiPmi[%d] simuAckNackAndCqi[%d] cqiPucchRsrcInd[%d] ri[%d]",
        ueCb->cqiRptCfg.cqipmiCfgIdx, ueCb->cqiRptCfg.simulAckNackAndCQI,
        ueCb->cqiRptCfg.cqiPucchRsrcIdx , ueCb->cqiRptCfg.riCfgIdx);
     RLOG_ARG3(L_INFO, DBG_CRNTI, ueCb->crnti, "Sr[%d] SrRes[%d] SrMax[%lu]",
       ueCb->srCfg.srPUCCHResIdx, ueCb->srCfg.srCfgIdx,
       ueCb->srCfg.enDsrTransMax);
   }
   RLOG_ARG0(L_EVENT, DBG_CRNTI, ueCb->crnti,"[OTA] Sending RRCConnectionReconfiguration"
         " for CQI , SR , RadioResource Modification");
   if (RFAILED == wrIfmRrcSndReqToUe(recfgMsg))
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe failed.");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }
  RETVALUE(ROK);
}
/** @brief This function is Initial Context Setup Request specific release 
 *         function.
 *
 * @details
 *
 *     Function:   wrUmmRadResModNewTransProcessor,
 *
 *         Processing steps:
 *         - free the rab information
 *         - free the security information
 *         - free the stored RAB Setup Request message
 *
 * @param[in]  transCb : transaction inforamation
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmRadResModNewTransProcessor 
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTransCb
)
{
   WrUmmRadResMod            *crntRadResMod;
   WrUmmRadResMod            *incmRadResMod;
   
   /* can be based on state */
   if ((transCb->transTyp == WR_UMM_RAD_RES_MOD_TRANS) &&
       (incTransCb->transTyp == WR_UMM_RAD_RES_MOD_TRANS))
   {
      crntRadResMod = &(transCb->u.radResMod);
      incmRadResMod = &(incTransCb->msg->u.internalMsg->u.radResMod);

      if ((crntRadResMod->bitMsk & WR_UMM_RAD_RES_MOD_CQI_SR) &&
            (incmRadResMod->bitMsk & WR_UMM_RAD_RES_MOD_CQI_SR))
      {
         RLOG0(L_DEBUG, "radResMod for Cqi SR already ongoing");
         RETVALUE(WR_UMM_TRANS_FAIL);
      }
      RLOG2(L_DEBUG, "Crnt Trns[%d] incTrsn[%d] [1/2 UL Power/CQI SR ]",
         crntRadResMod->bitMsk, incmRadResMod->bitMsk);
   }  
   RETVALUE(WR_UMM_TRANS_OK);
}
 
/**
 *  @brief This function used to send the schd recfg for UE
 *  in case of tx mode change
 *
 *      Function: wrUmmSchUeRecfgTxModeChng
 *
 *  @return S16
 -# Success : ROK
 -# Failure : RFAILED
 */

PRIVATE S16 wrUmmSchUeRecfgTxModeChng
(
 WrUmmTransCb                 *transCb,
 Bool                         isTxModeCmplt
 )
{
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   WrUeCb                    *ueCb = transCb->ueCb;
   WrUmmTxModeChngTransCb    *txModeChngTrans = &transCb->u.txModeChngTrans;
   RmuUeRecfgInd             *rrmUeRecfgInd = txModeChngTrans->msg->u.rrmUeRecfgInd;
   U32                       transId;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RETVALUE(RFAILED);
   }

   cfgReq->action                = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg                       = &cfgReq->u.recfgInfo.u.ueRecfg;
   ueRecfg->cellId               = ueCb->cellId;
   ueRecfg->oldCrnti             = ueCb->crnti;
   ueRecfg->newCrnti             = ueCb->crnti;

   if(!isTxModeCmplt)
   {
      wrUmmSchdFillUeRadioResCfgInfo(ueRecfg, rrmUeRecfgInd, ueCb, TRUE);
      transId = transCb->transId | WR_UMM_TX_MODE_CHANGE_START; 
   }
   else
   {
      RmuAntInfo *antInfo;
      U8 mode=0;

      //antInfo = &ueRecfgRespPtr->stUeRadioResCfg.stPhyCfgDed.stAntInfo;
      antInfo = &rrmUeRecfgInd->stUeRadioResCfg.stPhyCfgDed.stAntInfo;

      ueRecfg->ueRecfgTypes |= RGR_UE_TXMODE_RECFG;
      WR_GET_RGR_TM(antInfo->eTxnMode,mode);
      ueRecfg->txMode.txModeEnum = mode; 
      /* Setting UeCB tx mode */
      WR_GET_NHU_TM(ueRecfg->txMode.txModeEnum, ueCb->antennaInfo.txmode);
      ueRecfg->txMode.pres = PRSNT_NODEF;

      ueRecfg->txMode.tmTrnstnState = RGR_TXMODE_RECFG_CMPLT;

      if((RMU_TXN_MODE1 == antInfo->eTxnMode)||(RMU_TXN_MODE2 == antInfo->eTxnMode))
      {
         ueRecfg->ueCodeBookRstRecfg.pres = NOTPRSNT;
      }
      else
      {
         ueRecfg->ueCodeBookRstRecfg.pres = PRSNT_NODEF;
         ueRecfg->ueCodeBookRstRecfg.pmiBitMap[0] = wrPmiBitMap0;
         ueRecfg->ueCodeBookRstRecfg.pmiBitMap[1] = wrPmiBitMap1;
      }

      transId = transCb->transId | WR_UMM_TX_MODE_CHANGE_COMPL; 
   }
   /* Dispatch the prepared message to scheduler.                         */
   WrIfmSchdCfgUe(cfgReq, transId);
   RETVALUE(ROK);
}

/** @brief This function is Initial Context Setup Request specific release 
 *         function.
 *
 * @details
 
 *     Function:  wrUmmIncTxModeChngNewTransProcessor,
 *
 *         Processing steps:
 *         - free the rab information
 *         - free the security information
 *         - free the stored RAB Setup Request message
 *
 * @param[in]  transCb : transaction inforamation
 * @return S16
 *    -#Success : ROK
 */
PUBLIC S16 wrUmmIncTxModeChngNewTransProcessor 
(
WrUmmTransCb                 *transCb,
WrUmmTransCb                 *incTransCb
)
{
  
   /* can be based on state */
   if ((transCb->transTyp == WR_UMM_TX_MODE_CHNG_TRANS) &&
       (incTransCb->transTyp == WR_UMM_TX_MODE_CHNG_TRANS))
   {
         RLOG0(L_DEBUG, "TxMode change transaction already ongoing");
         printf("\nTxMode change transaction already ongoing\n");
         RETVALUE(WR_UMM_TRANS_FAIL);
   }  
   RETVALUE(WR_UMM_TRANS_OK);
}
/** @brief This function is responsible for preparing the sending the RRC
 *         reconfiguration message incase TM mode change 
 *
 * @details
 *
 *     Function: wrUmmTxModeChngPrepRrcMsgAndCfg
 *
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PRIVATE S16 wrUmmTxModeChngPrepRrcMsgAndCfg
(
WrUmmTransCb                 *transCb
)
{
   NhuDatReqSdus                             *recfgMsg;
   WrUeCb                                    *ueCb = transCb->ueCb;
   NhuDL_DCCH_Msg                            *dcchMsg;
   NhuRRCConRecfgn                           *rrcConReCfg;
   NhuRRCConRecfgn_r8_IEs                    *recfgIEs;
   NhuRadioResourceConfigDedicated           *rrcCfgDed;
   U8                                        transId;
   RmuUeRecfgInd                             *rrmUeRecfgInd;
   EnumNhuAntennaInfoDedicatedtransmissionMode eTxMode;

   TRC2(wrUmmRadResModPrepRrcMsgAndCfg)
   WR_ALLOCEVNT(&recfgMsg, sizeof(NhuDatReqSdus));
   if (recfgMsg == NULLP)
   {
      RETVALUE(RFAILED);
   }
   recfgMsg->hdr.cellId  = ueCb->cellId;
   recfgMsg->hdr.ueId    = ueCb->crnti;
   recfgMsg->hdr.transId = transCb->transId | C1_RRCCONRECFGN;

   rrmUeRecfgInd =  transCb->u.txModeChngTrans.msg->u.rrmUeRecfgInd;

   /* Fix for ccpu00126824:if wrUmmGetRrcTransId fails, recfgMsg is freed*/
   /* Fetch an RRC transaction id for this procedure.                     */
   if (wrUmmGetRrcTransId(ueCb, C1_RRCCONRECFGN, 
                      WR_GET_TRANS_FRM_TRANSID(transCb->transId), 
                      &transId) != ROK)
   {
      
      RLOG_ARG0(L_ERROR, DBG_CRNTI,ueCb->crnti,"Max RRC Transaction already "
         "reached");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }

   /* Create the reconfiguration message which is constructued excluding  */
   /* the actual procedure related details which will be added further    */
   if (wrUmmRrcCreateRrcReconfigMsg(recfgMsg, transId) != ROK)
   {
      RETVALUE(RFAILED);
   }
   
   /* Prepare the radio resources to exclude all the IEs present in the   */
   /* the IE. Those IEs are filled subsequently                           */
   dcchMsg     = &(recfgMsg->sdu.ueMsg.msg.dedMsg.dlDcchMsg.m.dcchMsg);
   rrcConReCfg = &(dcchMsg->message.val.c1.val.rrcConRecfgn);
   recfgIEs    = &(rrcConReCfg->criticalExtns.val.c1.val.rrcConRecfgn_r8);
   rrcCfgDed   = &(recfgIEs->radioResourceConfigDedicated);

   wrUmmRrcPrepRadioResourceDed(rrcCfgDed);
   
   WR_FILL_TKN_UINT(rrcCfgDed->physicalConfigDedicated.pres,PRSNT_NODEF);
   eTxMode = (RmuTxnMode)rrmUeRecfgInd->stUeRadioResCfg.stPhyCfgDed.stAntInfo.eTxnMode;
   if (eTxMode == NhuAntennaInfoDedicatedtransmissionModetm3Enum)
   {
      /* U8  codeBook[2] = {2, 0}; */
      NhuPhysicalConfigDedicatedantennaInfo *antennaInfo =
         &(recfgIEs->radioResourceConfigDedicated.physicalConfigDedicated.antennaInfo);
      wrFillTknU8(&antennaInfo->choice, ANTENNAINFO_EXPLICITVALUE);
      wrFillTknU8(&antennaInfo->val.explicitValue.pres, PRSNT_NODEF);
      wrFillTknU32(&antennaInfo->val.explicitValue.transmissionMode,
            ueCb->antennaInfo.txmode);   
      WR_FILL_TKN_UINT(antennaInfo->val.explicitValue.codebookSubsetRestrn.choice,\
            CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM3);
      WR_FILL_BIT_STR(antennaInfo->val.explicitValue.codebookSubsetRestrn.val.n2TxAntenna_tm3,\
            2, 3, recfgMsg); 

      wrFillTknU8(&antennaInfo->val.explicitValue.ue_TransmitAntennaSelection.choice,
            UE_TRANSMITANTENNASELECTION_RELEASE);
   }
   else if (eTxMode == NhuAntennaInfoDedicatedtransmissionModetm4Enum)
   {
      /*U8  codeBook[2] = {2, 0}; */
      NhuPhysicalConfigDedicatedantennaInfo *antennaInfo =
         &(recfgIEs->radioResourceConfigDedicated.physicalConfigDedicated.antennaInfo);
      wrFillTknU8(&antennaInfo->choice, ANTENNAINFO_EXPLICITVALUE);
      wrFillTknU8(&antennaInfo->val.explicitValue.pres, PRSNT_NODEF);
      wrFillTknU32(&antennaInfo->val.explicitValue.transmissionMode,
            ueCb->antennaInfo.txmode);

      WR_FILL_TKN_UINT(antennaInfo->val.explicitValue.codebookSubsetRestrn.choice,\
            CODEBOOKSUBSETRESTRN_N2TXANTENNA_TM4);
      WR_FILL_BIT_STR(antennaInfo->val.explicitValue.codebookSubsetRestrn.val.n2TxAntenna_tm4,\
            6, 63, recfgMsg); 

      wrFillTknU8(&antennaInfo->val.explicitValue.ue_TransmitAntennaSelection.choice,
            UE_TRANSMITANTENNASELECTION_RELEASE);
   }
   else if (eTxMode ==  NhuAntennaInfoDedicatedtransmissionModetm2Enum)
   {   
      NhuPhysicalConfigDedicatedantennaInfo *antennaInfo =
         &(recfgIEs->radioResourceConfigDedicated.physicalConfigDedicated.antennaInfo);
      wrFillTknU8(&antennaInfo->choice, ANTENNAINFO_EXPLICITVALUE);
      wrFillTknU8(&antennaInfo->val.explicitValue.pres, PRSNT_NODEF);
      wrFillTknU32(&antennaInfo->val.explicitValue.transmissionMode,
            ueCb->antennaInfo.txmode); 

      wrFillTknU8(&antennaInfo->val.explicitValue.ue_TransmitAntennaSelection.choice,
            UE_TRANSMITANTENNASELECTION_RELEASE);
   }

   if(rrmUeRecfgInd->stUeRadioResCfg.bRadioResDedBitMap &
      RM_RCM_PHY_DED_RES_CFG)
   {   
      wrUmmRrcFillUePhyCfgDedInfo(rrcCfgDed, rrmUeRecfgInd);
   }

   if (RFAILED == wrIfmRrcSndReqToUe(recfgMsg))
   {
      RLOG0(L_ERROR, "wrIfmRrcSndReqToUe failed.");
      WR_FREEEVNT(recfgMsg);
      RETVALUE(RFAILED);
   }

  RETVALUE(ROK);
} 



/** @brief This function is responsible for preparing the sending the RRC
 *         reconfiguration message. The lower layer configurations are also 
 *         filled here.
 *
 * @details
 *
 *     Function: wrUmmTxModeChngProcessor,
 * @param[in, out] transCb : transaction information 
 * @return S16
 *    -#Success : ROK
 *    -#Failure : RFAILED
 */
PUBLIC S16 wrUmmTxModeChngProcessor
(
WrUmmTransCb                *transCb 
)
{
   WrUmmIncMsg              *msg  = transCb->msg;
   WrUeCb                   *ueCb = transCb->ueCb;
   S16                       ret  = RFAILED;
  WrUmmTxModeChngTransCb    *txModeChngTrans = &transCb->u.txModeChngTrans;
   switch (msg->msgTyp)
   { 
      case WR_UMM_RRM_UERECFG_IND:
      {
         txModeChngTrans->msg = transCb->msg;
         wrUmmSchUeRecfgTxModeChng(transCb, FALSE);
         transCb->msg->refCnt++;
      }   
      break;
      case WR_UMM_SCHD_CFG:
      {
         if(msg->u.schdCfm.transId & WR_UMM_TX_MODE_CHANGE_START)
         {   
            wrUmmTxModeChngPrepRrcMsgAndCfg(transCb);  
         }
         else if(msg->u.schdCfm.transId & WR_UMM_TX_MODE_CHANGE_COMPL)
         {
            wrUmmCommitUePhyCfgDedResInfo(ueCb,
                  &txModeChngTrans->msg->u.rrmUeRecfgInd->stUeRadioResCfg.stPhyCfgDed);
            wrUmmTransComplete(transCb);
         }   
      }
      break;
      case WR_UMM_UU_IND_PDU:
      {
         NhuDatIndSdus        *uuIndPdu = msg->u.uuIndPdu;
         NhuUL_DCCH_MsgTyp    *ulDcchMsg = &(uuIndPdu->sdu.m.ulDcchMsg.dcchMsg.message);
         NhuUL_DCCH_MsgTypc1  *rrcMsg = &(ulDcchMsg->val.c1);
         U32                  rrcTransId = 0;

         /* Stop the RRC Reconfiguration Timer */
         wrStopTransTmr(transCb,WR_TMR_RECONFIGURE);

         rrcTransId = rrcMsg->val.rrcConRecfgnCompl.rrc_TrnsxnId.val;
         ret = wrUmmRelRrcTransId(transCb->ueCb, C1_RRCCONRECFGN, rrcTransId);
         if (ret != ROK)
         {
            RLOG0(L_ERROR, "Release of transaction ID failed");
         }

         wrUmmSchUeRecfgTxModeChng(transCb, TRUE);
      }
      break;
      default:
      {
         RLOG1(L_ERROR, "Invalid message[%u]", msg->msgTyp);
      }
   }
   return(ret);
}
/** @brief This function is selects the seculrity Algorithm 
        based on the cofigured priority at eNB.
 *
 * @details
 *
 *     Function:  wrUmmSelectSecurityAlgo
 *
 * @param[in] configSecAlgoVal     : Security Algorithm configured at eNB
 * @param[in, out] secModAlgoVal   : selected security Algo which will 
                                        go in security Mode command
 * @param[in] securAlgoMask        : Bit Mask of security Algo
 * @param[in] IntgOrCiphAlgo       : Integrity or Ciphering Algo selction
                                     WR_INTEGRITY_ALGO - 1
                                     WR_CIPHERING_ALGO - 0
 * @return Bool
 *    -#TRUE : incase of selection success
        FALSE: incase of selection failure
 */
PRIVATE Bool wrUmmSelectSecurityAlgo
(
U8 configSecAlgoVal,
U16 *secModAlgoVal,
U8 securAlgoMask,
U8 IntgOrCiphAlgo
)
{
   TRC2(wrUmmSelectSecurityAlgo)

   switch(configSecAlgoVal) 
   {
      case WR_UMM_ZUC_ALGO:
      {
         if(securAlgoMask & WR_UMM_ZUC_ALGO_BITMASK)
         {
            RLOG_STR(L_EVENT,"[%s] Algo as ZUC", (PSTR)securityAlgo[IntgOrCiphAlgo]);
            *secModAlgoVal = WR_UMM_ZUC_ALGO;
            RETVALUE(TRUE);
         }
         break;
      }
      case WR_UMM_AES_ALGO:
      {
         if(securAlgoMask & WR_UMM_AES_ALGO_BITMASK)
         {
            RLOG_STR(L_EVENT,"[%s] Algo as AES", (PSTR)securityAlgo[IntgOrCiphAlgo]);
            *secModAlgoVal = WR_UMM_AES_ALGO;
            RETVALUE(TRUE);
         }
         break;
      }
      case WR_UMM_SNOW3G_ALGO:
      {
         if(securAlgoMask & WR_UMM_SNOW3G_ALGO_BITMASK)
         {
            RLOG_STR(L_EVENT,"[%s] Algo as SNOW3G", (PSTR)securityAlgo[IntgOrCiphAlgo]);
            *secModAlgoVal = WR_UMM_SNOW3G_ALGO;
            RETVALUE(TRUE);
         }
         break;
      }
      case WR_UMM_NULL_ALGO:
      {
         RLOG_STR(L_EVENT,"[%s] Algo as NULL", (PSTR)securityAlgo[IntgOrCiphAlgo]);
         *secModAlgoVal = WR_UMM_NULL_ALGO;
         RETVALUE(TRUE);
         break;
      }
      default:
      {
         RLOG0(L_EVENT, "Not defined the security Algo");
         break;
      }
   }/* end switch */
   RETVALUE(FALSE);
}

/** @brief This function will reconfigure the TA timer value during UE reconfiguration.
 *
 * @details
 *
 *     Function:  wrUmmSndReqToStartTATimer
 *
 * @param[in] WrUmmTransCb     : transaction information
 * @return S16
 *    -#Success : ROK
 */
PRIVATE S16  wrUmmSndReqToStartTATimer
(
WrUmmTransCb                 *transCb
)
{
   RgrCfgReqInfo             *cfgReq;
   RgrUeRecfg                *ueRecfg;
   U32                       transId;

   WR_ALLOC(&cfgReq, sizeof(*cfgReq));
   if (cfgReq == NULLP)
   {
      RLOG0(L_ERROR, "Allocation failed for RgrCfgReqInfo");
      RETVALUE(RFAILED);
   }

   transId = transCb->transId | WR_RAD_RES_MOD_SCHD_TRANS_ID;
   ueRecfg = &cfgReq->u.recfgInfo.u.ueRecfg;
   cfgReq->action = RGR_RECONFIG;
   cfgReq->u.recfgInfo.recfgType = RGR_UE_CFG;
   ueRecfg->cellId = transCb->ueCb->cellId;
   ueRecfg->oldCrnti = transCb->ueCb->crnti;
   ueRecfg->newCrnti = transCb->ueCb->crnti;

   ueRecfg->ueRecfgTypes            |= RGR_UE_TATMR_RECFG;
   if(wrUmmSchFillRgrUeTaTmrCfg(&ueRecfg->ueTaTmrRecfg,transCb->ueCb) != ROK)
   {
      RLOG0(L_ERROR, "wrUmmSchFillRgrUeTaTmrCfg failed");
      RETVALUE(RFAILED);
   }

   WrIfmSchdCfgUe(cfgReq, transId);
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
