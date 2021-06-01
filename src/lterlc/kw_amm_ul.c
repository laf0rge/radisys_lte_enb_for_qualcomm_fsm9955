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

        Name:    RLC - AM module file

        Type:    C source file

        Desc:    Source code for Acknowledged Mode Module functions such as,

                 Transmission of data/control PDUs
                 Retransmission (Feedback in terms of status)
                 Polling
                 Assemble SDUs
                 Reception - reordering
                 Duplicate detection for byte segments
                 Reassemble SDUs

        File:    gp_amm_ul.c

        Sid:      gp_amm_ul.c@@/main/TeNB_Main_BR/8 - Mon Aug 11 16:43:13 2014

        Prg:

*********************************************************************21*/
static const char* RLOG_MODULE_NAME="AMM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=190;

/* header include files (.h) */
#include "envopt.h"        /* environment options */
#include "envdep.h"        /* environment dependent */
#include "envind.h"        /* environment independent */

#include "gen.h"           /* general */
#include "ssi.h"           /* system services */
#include "cm5.h"           /* common timer defines */
#include "cm_tkns.h"       /* common tokens defines */
#include "cm_mblk.h"       /* common memory allocation library defines */
#include "cm_llist.h"      /* common link list  defines  */
#include "cm_hash.h"       /* common hash list  defines */
#include "cm_lte.h"        /* common LTE defines */
#include "lkw.h"           /* LKW defines */
#include "ckw.h"           /* CKW defines */
#include "kwu.h"           /* KWU defines */
#include "rgu.h"           /* RGU defines */
#include "kw_udx.h"
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_ul.h"

/* extern (.x) include files */
#include "gen.x"           /* general */
#include "ssi.x"           /* system services */

#include "cm5.x"           /* common timer library */
#include "cm_tkns.x"       /* common tokens */
#include "cm_mblk.x"       /* common memory allocation */
#include "cm_llist.x"      /* common link list */
#include "cm_hash.x"       /* common hash list */
#include "cm_lte.x"        /* common LTE includes */
#include "cm_lib.x"        /* common memory allocation library */
#include "lkw.x"           /* LKW */
#include "ckw.x"           /* CKW */
#include "kwu.x"           /* KWU */
#include "rgu.x"           /* RGU */

#include "kw.x"
#include "kw_ul.x"
#include "kw_udx.x"

/* Variable for logging, declared in cl */
#ifndef TENB_ACC
#ifdef MSPD
extern U32 ulrate_rgu;
#endif
#endif
#ifndef TENB_ACC
#ifndef TENB_T2K3K_SPECIFIC_CHANGES
#ifndef LTE_PAL_ENB
extern U32 isMemThreshReached(Region region);
#endif
#else
#ifndef LTE_PAL_ENB
extern U32  isMemThreshReached(Region region);
#endif
#endif
#endif
/** @file gp_amm_ul.c
@brief RLC Acknowledged Mode Uplink Module
**/
#define KW_MODULE (KW_DBGMASK_AM | KW_DBGMASK_UL) /* for debugging purpose */

/* private function declarations */

PRIVATE Void kwAmmUlAssembleCntrlInfo ARGS ((KwCb *gCb, KwUlRbCb *rbCb));

PRIVATE S16 kwAmmExtractHdr ARGS ((KwCb *gCb,
                                Buffer *pdu,
                                KwAmHdr *amHdr,
                                U8 *fByte));

PRIVATE Bool kwAmmUlPlacePduInRecBuf ARGS ((KwCb *gCb,
                                            Buffer *pdu,
                                            KwUlRbCb *rbCb,
                                            KwAmHdr *amHdr));

PRIVATE Void kwAmmTriggerStatus ARGS ((KwCb *gCb,
                                    KwUlRbCb *rbCb,
                                    KwSn sn,
                                    Bool discFlg));

PRIVATE S16  kwAmmUlReassembleSdus ARGS ((KwCb *gCb,
                                     KwUlRbCb *rbCb,
                                     KwAmRecBuf *recBuf));

PRIVATE Void kwAmmProcPduOrSeg ARGS ((KwCb *gCb,
                                      KwUlRbCb *rbCb,
                                      KwAmHdr *amHdr,
                                      Buffer *pdu));

PRIVATE Void kwAmmUpdExpByteSeg ARGS ((KwCb *gCb,KwAmUl *amUl, KwSeg* newSeg));

PRIVATE Void kwAmmExtractElmnt ARGS ((KwCb *gCb, Buffer *pdu, KwExtHdr *hdrInfo));

PRIVATE Void kwAmmUlHndlStatusPdu ARGS ((KwCb *gCb,
                                         KwUlRbCb *rbCb,
                                         Buffer *cntrlPdu,
                                         U8 *fByte));

/******************************************************************************

  AM Module contains the following funcitons:

  -  kwAmmProcessSdus
     -  kwAmmUlAssembleCntrlInfo
     -  kwResegRetxPdus
     -  kwAssembleSdus
     -  kwChkandSetPoll
  -  kwAmmProcessPdus
     -  kwAmmUlHndlStatusPdu
     -  kwAmmTriggerStatus
     -  kwAmmUlReassembleSdus

*******************************************************************************/
/** @addtogroup ammode */
/*@{*/

/**
 * @brief   Private function to fill NACK information
 *
 * @param[in]   sn         Sequence number of the PDU for which the NACK
 * @param[in]   isSegment  TRUE means NACK for segment; FALSE for PDU
 * @param[in]   soStart    SOStart
 * @param[in]   soEnd      SOEnd
 * @param[out]  nackInfo   Nack information holder to be filled
 *
 * @return  S16
 *    The number of bits required to encode this NACK information
 *
 */
#ifdef ANSI
PRIVATE S16 kwAmmUlSetNackInfo
(
KwSn         sn,
Bool         isSegment,
U16          soStart,
U16          soEnd,
KwNackInfo   *nackInfo
)
#else
PRIVATE S16 kwAmmUlSetNackInfo(sn, isSegment, soStart, soEnd, nackInfo)
KwSn         sn;
Bool         isSegment;
U16          soStart;
U16          soEnd;
KwNackInfo   *nackInfo;
#endif
{
   nackInfo->sn = sn;
   nackInfo->isSegment = isSegment;

   if(isSegment)
   {
      nackInfo->soStart = soStart;
      nackInfo->soEnd   = soEnd;

      RETVALUE(42); /* NACK,E1,E2,Sostart,SoEnd = 42 */
   }
   else
   {
      nackInfo->soStart = 0;
      nackInfo->soEnd   = 0;

      RETVALUE(12); /* NACK,E1,E2 = 12 */
   }
}

/**
 * @brief   Private handler to gather information required to create the STATUS
 *          PDU
 *
 * @details
 *    Scans the reception buffer and copies information to the UdxDlStaPdu
 *    structure about SN's  and segments not yet received. This data is
 *    sent to the DL instance so that it can create an appropriate (depending
 *    on the grants from MAC) STATUS PDU and send it to MAC.
 *
 * @param[in]  gCb      RLC instance control block
 * @param[in]  rbCb     Uplink RB control block
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmUlAssembleCntrlInfo
(
KwCb       *gCb,
KwUlRbCb   *rbCb
)
#else
PRIVATE Void kwAmmUlAssembleCntrlInfo(gCb, rbCb)
KwCb       *gCb;
KwUlRbCb   *rbCb;
#endif
{
   KwUdxDlStaPdu   *pStatusPdu;
   KwNackInfo      *nackInfo;
   KwSn            sn;                /* sequence number */
   KwSn            mSn;               /* Mod val of sequence number */
   KwSn            vrMs;              /* Mod val of vrMs */
   KwSeg           *seg;              /* pdu segment */
   U16             nackCnt = 0;       /* Index for staPdu */
   U16             seqSo;             /* segmment offset */
   KwUdxUlSapCb    *sapCb;
   U16             staPduEncSize = 15; /* size that would be of the encoded
                                          STATUS PDU, it is in bits; 15 for
                                          first fixed part of STATUS PDU */

   TRC2(kwAmmUlAssembleCntrlInfo)


   sapCb = KW_GET_UDX_SAP(gCb);

   KW_ALLOC_SHRABL_BUF(sapCb->pst.region,
                       sapCb->pst.pool,
                       pStatusPdu, 
                       sizeof(KwUdxDlStaPdu)); 

#if (ERRCLASS & ERRCLS_ADD_RES)
   /* Memory allocation failure can not be expected  */
   if(!pStatusPdu)
   {
     RETVOID;
   }
#endif
   nackInfo = pStatusPdu->nackInfo;

   sn = AMUL.vrR;
   MODAMR(sn, mSn, AMUL.vrR);
   MODAMR(AMUL.vrMs, vrMs, AMUL.vrR);

   while (mSn < vrMs )
   {
      /* For missing PDUs */
      if (AMUL.recBuf[sn] == NULLP && nackCnt < KW_MAX_NACK_CNT )
      {
         RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                  "Missing PDU's SN = %d UEID:%d CELLID:%d", 
                  sn,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         staPduEncSize += kwAmmUlSetNackInfo(sn,
                                             FALSE, /* isSegment */
                                             0,     /* SOStart */
                                             0,     /* SOEnd */
                                             (nackInfo + nackCnt));
         nackCnt++;
      }
      else if ((AMUL.recBuf[sn]->pdu == NULLP) &&
             (AMUL.recBuf[sn]->segLst.count > 0))
      {
         /* Scan through the byte segments of PDU and add this sn
            with soStart and soEnd info to staPdu */

         seqSo  = 0;
         KW_LLIST_FIRST_SEG(AMUL.recBuf[sn]->segLst, seg);
         while (seg != NULLP && nackCnt < KW_MAX_NACK_CNT)
         {
            /* For missing byte segments */
            if (seg->amHdr.so != seqSo)
            {
               staPduEncSize += kwAmmUlSetNackInfo(sn,
                                                   TRUE,
                                                   seqSo,
                                                   seg->amHdr.so - 1,
                                                   (nackInfo + nackCnt));
               nackCnt++;

               RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                             "Missing byte segment's" 
                             " SN:%d UEID:%d CELLID:%d",
                             sn,
                             rbCb->rlcId.ueId,
                             rbCb->rlcId.cellId);
               RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                             "soStart and soEnd = %d, %d UEID:%d CELLID:%d",
                             seqSo,
                             seg->amHdr.so - 1,
                             rbCb->rlcId.ueId,
                             rbCb->rlcId.cellId);
            }

            seqSo = seg->soEnd + 1;
            KW_LLIST_NEXT_SEG(AMUL.recBuf[sn]->segLst, seg);
         }

         /* Check if the last segment is missing */
         KW_LLIST_LAST_SEG(AMUL.recBuf[sn]->segLst, seg);
         if ((seg != NULLP) &&
             (seg->amHdr.lsf != 1 && nackCnt < KW_MAX_NACK_CNT))
         {
            staPduEncSize += kwAmmUlSetNackInfo(sn,
                                                TRUE,
                                                seqSo,
                                                KW_ALL_BYTES_MISSING,
                                                (nackInfo + nackCnt));
            nackCnt++;

            RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                          "kwAmmUlAssembleCntrlInfo: Missing (last) byte " 
                          "segment's SN:%d UEID:%d CELLID:%d",
                          sn,
                          rbCb->rlcId.ueId,
                          rbCb->rlcId.cellId);
            RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                          "soStart and soEnd = %d, %d UEID:%d CELLID:%d",
                          seqSo,
                          KW_ALL_BYTES_MISSING,
                          rbCb->rlcId.ueId,
                          rbCb->rlcId.cellId);
         }
      }
      

      sn = (sn + 1) & KW_MOD_1024; /* MOD 1024 */
      MODAMR(sn, mSn, AMUL.vrR);

      /* Find the next missing sequence number if nackCnt reaches maximum and
         still Reordering window has some missing AMDPDUs / AMDPDU segments. The
         next missing sequence number will be considered as the ack sequnece
         number in the status pdu.*/
      if((nackCnt == KW_MAX_NACK_CNT) &&
          ((AMUL.recBuf[sn] == NULLP) ||
            ((AMUL.recBuf[sn]->pdu == NULLP) &&
             (AMUL.recBuf[sn]->segLst.count > 0))))
      {
         break;
      }
   }

   /* Update ACK SN with the last sn for which feedback is not assembled */
   if ( mSn == vrMs)
   {
      pStatusPdu->ackSn = AMUL.vrMs;
   }
   else
   {
      pStatusPdu->ackSn = sn;
   }

   pStatusPdu->nackCount = nackCnt;
   RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
            "kwAmmUlAssembleCntrlInfo: ACK PDU's SN = %d"
            "UEID:%d CELLID:%d",
            pStatusPdu->ackSn,
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);

   pStatusPdu->controlBo = (staPduEncSize + 7) >>3; /* in bytes */

   AMUL.staTrg = FALSE;
   AMUL.gatherStaPduInfo = FALSE;


   if (KwUlUdxStaPduReq(&sapCb->pst,
                        sapCb->spId,
                        &rbCb->rlcId,
                        pStatusPdu) != ROK)
   {
      RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
               "Failed to Send Sta Pdu UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      KW_FREE_SHRABL_BUF_WC(sapCb->pst.region,
			    sapCb->pst.pool,
			    pStatusPdu, 
			    sizeof(KwUdxDlStaPdu));
   }

   RETVOID;
}

#ifdef T2K_TRIGGER_RLC_REEST
PUBLIC U32 drpRlcDrbPack;
#endif
/**
 * @brief Handler to process the PDUs received from MAC and send it to PDCP
 *
 * @details
 *    This function is invoked by UTL with the PDU(s) received from MAC.
 *    It reorders the received data PDUs and trigger status report as
 *    needed. Reassembles the SDUs in sequence and send it to PDCP.
 *    It also processes the control PDU
 *
 * @param[in]  gCb      RLC instance control block
 * @param[in]  rbCb     RB control block
 * @param[out] pduInfo  PDU Info received from MAC
 *
 *  @return Void
 *
 */
#ifdef LTE_L2_MEAS
#ifdef ANSI
PUBLIC Void kwAmmProcessPdus
(
KwCb                    *gCb,
KwUlRbCb                *rbCb,
KwPduInfo               *pduInfo,
U32                     ttiCnt
)
#else
PUBLIC Void kwAmmProcessPdus(gCb, rbCb, pduInfo, ulTimeInfo)
KwCb                    *gCb;
KwUlRbCb                *rbCb;
KwPduInfo               *pduInfo;
U32                     ttiCnt;
#endif
#else
#ifdef ANSI
PUBLIC Void kwAmmProcessPdus
(
KwCb        *gCb,
KwUlRbCb    *rbCb,
KwPduInfo   *pduInfo
)
#else
PUBLIC Void kwAmmProcessPdus(gCb, rbCb, pduInfo)
KwCb        *gCb;
KwUlRbCb    *rbCb;
KwPduInfo   *pduInfo;
#endif
#endif
{
   Buffer    *pdu;
   KwAmUl    *amUl;
   KwAmHdr   amHdr;
   U8        numPdu = 0;
   U8        numPduToProcess;
   KwSn      sn;
   KwSn      tSn;
   KwSn      mSn;
   U8        fByte;
   Bool      discFlg;
#ifdef LTE_L2_MEAS_RLC
   MsgLen              rlcSduSz;  /*Holds length of Rlc Sdu*/
#endif /* LTE_L2_MEAS */

   TRC2(kwAmmProcessPdus)


   amUl = &AMUL;

   numPduToProcess = KW_MIN(pduInfo->numPdu, RGU_MAX_PDU);
   RLOG_ARG4(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
            "numPdu[%ld],numPduToProcess[%ld] UEID:%ld CELLID:%ld",
            numPdu,
            numPduToProcess,
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);
   while (numPdu < numPduToProcess)
   {
      discFlg = FALSE;
      pdu = pduInfo->mBuf[numPdu++];

      if (! pdu)
      {

         RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
                  "Null Pdu UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         gCb->genSts.errorPdusRecv++;
         break;
      }

#ifndef TENB_ACC
#ifndef LTE_PAL_ENB
#ifdef MSPD
      MsgLen len;
      SFndLenMsg(pdu, &len);
      ulrate_rgu += len;
#endif
#endif
#endif
      /* Extract AM PDU/SEG header Info */
      KW_MEM_ZERO(&amHdr, sizeof(KwAmHdr));
      /* Avoided the allocation of amHdr and sending
         a single pointer */
      if (kwAmmExtractHdr(gCb, pdu, &amHdr, &fByte) != ROK)
      {
         RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
                  "Header Extraction Failed UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         KW_FREE_BUF(pdu);
         gCb->genSts.errorPdusRecv++;
         continue;
      }
      /* Check if its a control PDU */
      if (amHdr.dc == 0)
      {
         kwAmmUlHndlStatusPdu(gCb, rbCb, pdu, &fByte);
         KW_FREE_BUF(pdu);
         continue;
      }
#ifndef LTE_QCOM
#ifdef LTE_TDD
#ifndef TENB_ACC
#ifndef TENB_T2K3K_SPECIFIC_CHANGES
#ifndef LTE_PAL_ENB
    /* Changed the condition to TRUE from ROK  */
      if(isMemThreshReached(kwCb[0]->init.region) == TRUE)
      {
         extern U32 rlculdrop;
	 rlculdrop++;
	 KW_FREE_BUF(pdu);
	 continue;
      }
#endif
#else
#ifndef LTE_PAL_ENB
      /*ccpu00142274 - UL memory based flow control*/
      if(isMemThreshReached(kwCb[0]->init.region) != ROK)
      {
         extern U32 rlculdrop;
         rlculdrop++;
         KW_FREE_BUF(pdu);
         continue;
      }
#endif
#endif
#endif
#endif
#endif /* #if LTE_QCOM */

#ifdef T2K_TRIGGER_RLC_REEST
      if(drpRlcDrbPack > 1000)
      {
         if(rbCb->rlcId.rbType == CM_LTE_DRB)
         {
            KW_FREE_BUF(pdu);
            continue;
         }
      }
      drpRlcDrbPack++;
#endif

      /* Reordering data PDU */
      sn = amHdr.sn;
      if (kwAmmUlPlacePduInRecBuf(gCb,pdu, rbCb, &amHdr) == TRUE)
      {
         Bool   tmrRunning;
         KwSn   tVrMr;
         KwSn   mVrH;

#ifdef LTE_L2_MEAS
         kwUtlCalUlIpThrPut(gCb, rbCb, pdu, ttiCnt);
#endif /* LTE_L2_MEAS */

         /* kw005.201 R9 Upgrade 3gpp spec 36.322 ver9.3.0 CR0090 *
          * Removed unused local variable vrR.                    */

         /* kw005.201 R9 Upgrade 3gpp spec 36.322 ver9.3.0 CR0090 *
          * Added VR(H) updation at the beginning.        */

         /* Update vrH */
         MODAMR(sn, mSn, amUl->vrR);
         MODAMR(amUl->vrH, mVrH, amUl->vrR);
         if (mSn >= mVrH)
         {
            amUl->vrH = (sn + 1) & KW_MOD_1024; /* MOD 1024 */

            RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
                  "kwAmmProcessPdus: Updated vrH = %d UEID:%d CELLID:%d",
                  amUl->vrH,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         }

         MODAMR(amUl->vrMr, tVrMr, amUl->vrR);

         /* Update vrMs */
         if (sn == amUl->vrMs)
         {
            /* Check if all byte segments are received */
            if (amUl->recBuf[sn]->allRcvd)
            {
               tSn = (sn + 1) & KW_MOD_1024; /* MOD 1024 */

               /* Scan through till the upper edge of the window */
               MODAMR(tSn, mSn, amUl->vrR);
               while (mSn <= tVrMr)
               {
                 if ((amUl->recBuf[tSn] == NULLP) ||
                     (!amUl->recBuf[tSn]->allRcvd))
                 {
                    RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
                              "kwAmmProcessPdus: Updated vrMs:%d "
                              "UEID:%d CELLID:%d",
                              tSn,
                              rbCb->rlcId.ueId,
                              rbCb->rlcId.cellId);

                    amUl->vrMs = tSn;
                    break;
                 }
                 tSn = (tSn + 1) & KW_MOD_1024; /* MOD 1024 */
                 mSn++;
               }
            }
         }

         /* kw005.201 R9 Upgrade 3gpp spec 36.322 ver9.3.0 CR0090 *
          * Removed the Retaining of VR(R) for VR(H) updation.     */

         /* Update vrR */
         if (sn == amUl->vrR)
         {
            /* Check if all byte segments are received */
            if (amUl->recBuf[sn]->allRcvd)
            {
               tSn = sn;
               MODAMR(tSn, mSn, amUl->vrR);
               /* Scan through till the upper edge of the window */
               while (mSn <= tVrMr)
               {
                  if ((amUl->recBuf[tSn] != NULLP) &&
                      (amUl->recBuf[tSn]->allRcvd))
                  {
                     kwAmmUlReassembleSdus(gCb, rbCb, amUl->recBuf[tSn]);
                     KW_FREE(gCb, AMUL.recBuf[tSn], sizeof(KwAmRecBuf));

                     amUl->recBuf[tSn] = NULLP;
                  }
                  else
                  {
                     amUl->vrR = tSn;
                     amUl->vrMr = (amUl->vrR + KW_AM_WIN_SZ) & KW_MOD_1024;
                     break;
                  }
                  tSn = (tSn + 1) & KW_MOD_1024; /* MOD 1024 */
                  mSn++;
               }
            }
         }

         /* kw005.201 R9 Upgrade 3gpp spec 36.322 ver9.3.0 CR0090 *
          * Removed VR(H) update from here and moved it at the    *
          * beginning.                    */

         /* Check if reOrdTmr is running and update vrX accordingly */
         tmrRunning = kwChkTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
         if (tmrRunning)
         {
            Bool snInWin = KW_AM_CHK_SN_WITHIN_RECV_WINDOW(amUl->vrX, amUl);

            /* kw005.201 R9 Upgrade 3gpp spec 36.322 ver9.3.0 CR0083 *
             * The 3rd condition in "if" statement is added.        */
            if ( (amUl->vrX == amUl->vrR) || ( (!snInWin) &&
                                             (amUl->vrX != amUl->vrMr) ) )
            {
               kwStopTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
               tmrRunning = FALSE;
            }
         }

         if (!tmrRunning)
         {
            /* kw003.201 - If VR(H) != VR(R), it will always be greater */
            if (amUl->vrR != amUl->vrH)
            {
               kwStartTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
               amUl->vrX = amUl->vrH;

               RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                        "kwAmmProcessPdus: Updated vrX = %d UEID:%d CELLID:%d",
                        amUl->vrX,
                        rbCb->rlcId.ueId,
                        rbCb->rlcId.cellId);
            }
         }
      }
      else
      {
         discFlg = TRUE;
         gRlcStats.amRlcStats.numULPdusDiscarded++;
      }

      if (amHdr.p)
      {
         kwAmmTriggerStatus(gCb,rbCb, sn, discFlg);
      }
   }

#ifdef LTE_L2_MEAS
   kwUtlCalUlIpThrPutIncTTI(gCb, rbCb,ttiCnt);
#endif /* LTE_L2_MEAS */
   gCb->genSts.pdusRecv += pduInfo->numPdu;
   if (amUl->gatherStaPduInfo)
   {
      kwAmmUlAssembleCntrlInfo(gCb,rbCb);
   }

   RETVOID;
}

/**
 * @brief Private handler to extract header Information of the PDU
 *
 * @details
 *    This function extracts the header elements of the PDU and store them
 *    in db for future reference.
 *
 *    fByte - is the first byte removed from the PDU as part of calling
 *            functions
 *
 * @param[in]  gCb     RLC instance control block
 * @param[in]  rbCb    Uplink RB control block
 * @param[in]  pdu     Received PDU
 * @param[out] amHdr   Pointer to the extracted AM header
 * @param[out] fByte   First byte removed from the PDU
 *
 * @return S16
 *     -# ROK
 *     -# RFAILED
 *
 */
#ifdef ANSI
PRIVATE S16 kwAmmExtractHdr
(
KwCb       *gCb,
Buffer     *pdu,
KwAmHdr    *amHdr,
U8         *fByte
)
#else
PRIVATE S16 kwAmmExtractHdr(gCb, pdu, amHdr, fByte)
KwCb       *gCb;
Buffer     *pdu;
KwAmHdr    *amHdr;
U8         *fByte;
#endif
{
   U8         e;
   U8         snByte;
   U16        sn;
   MsgLen     pduSz;
   MsgLen     totalSz = 0;
   KwExtHdr   hdrInfo;

   TRC2(kwAmmExtractHdr)


   KW_MEM_ZERO(&hdrInfo, sizeof(KwExtHdr));

   /* Extract fixed part of the header */
   SFndLenMsg(pdu,&pduSz);
   SRemPreMsg(fByte, pdu);
   amHdr->dc = (*fByte & KW_DC_POS) >> KW_DC_SHT;
   if (KW_CNTRL_PDU == amHdr->dc)
   {
      RETVALUE(ROK);
   }
   /* kw002.201 : Changed the extraction of hdr elements to avoid */
   /*             function calls                                  */
   amHdr->rf = (*fByte & KW_RF_POS)   >> KW_RF_SHT;
   amHdr->p  = (*fByte & KW_POLL_POS) >> KW_POLL_SHT;
   amHdr->fi = (*fByte & KW_FI_POS)   >> KW_FI_SHT;
   e = amHdr->e  = (*fByte & KW_E_POS)>> KW_E_SHT;

   SRemPreMsg(&snByte, pdu);
   sn = (U16)(((*fByte & KW_SN_POS) << KW_BYTE_LEN ) | snByte);
   amHdr->sn = sn;
   if (amHdr->rf == 1)
   {
      /* Extract extn part of the header */
      hdrInfo.len = KW_LSF_LEN;
      kwAmmExtractElmnt(gCb, pdu, &hdrInfo);
      amHdr->lsf = (U8)hdrInfo.val;

      hdrInfo.len = KW_SO_LEN;
      kwAmmExtractElmnt(gCb, pdu, &hdrInfo);
      amHdr->so = hdrInfo.val;
      pduSz -= 2;
   }

   amHdr->numLi = 0;
   /* Extract LIs */
   while (e && (amHdr->numLi < KW_MAX_UL_LI))
   {
      hdrInfo.len = KW_E_LEN;
      kwAmmExtractElmnt(gCb, pdu, &hdrInfo);
      e = amHdr->e = (U8)hdrInfo.val;

      /* Extract LI value*/
      hdrInfo.len = KW_LI_LEN;
      kwAmmExtractElmnt(gCb, pdu, &hdrInfo);
      /* li = hdrInfo.val;*/

      /* check if LI is zero */
      if (! hdrInfo.val)
      {
         RLOG0(L_ERROR, "Received LI as 0");
         RETVALUE(RFAILED);
      }

      /* store the extracted  LI value */
      amHdr->li[amHdr->numLi++] = hdrInfo.val;
      totalSz += hdrInfo.val;  /* incrment the size by LI value */
   }

   /*ccpu00122597:PDU is dropped if liCnt exceeds KW_MAX_LI*/
   if(e && (amHdr->numLi >= KW_MAX_UL_LI))
   {
      RLOG2(L_ERROR,"LI Count [%u] exceeds Max LI Count[%u]", 
            amHdr->numLi, KW_MAX_UL_LI);
      RETVALUE(RFAILED);
   }

   /*                                first 2 bytes + Add one for  Odd LI*/
   pduSz -= ( amHdr->numLi + (amHdr->numLi >> 1) + 2 + (amHdr->numLi & 1) );

   if ( totalSz >= pduSz )
   {   
      RLOG3(L_ERROR,"SN [%d]:Corrupted PDU as TotSz[%lu] PduSz[%lu] ",
               amHdr->sn, totalSz, pduSz);
      RETVALUE(RFAILED);
   }

   RETVALUE(ROK);
}

/**
 * @brief Private handler to process the status PDU
 *
 * @details
 *    Private handler invokded by kwAmmProcessPdus to process the
 *    control PDU (status report) received from its peer RLC entity.
 *
 *        - Decode the values from the received control pdu
 *        - Create a KwUdxStaPdu structure, copy the values onto it and
 *          send it to the DL instance for further processing
 *
 * @param[in]  gCb       RLC instance control block
 * @param[in]  rbCb      Uplink RB control block
 * @param[in]  cntrlPdu  Control PDU received from MAC
 * @param[in]  fByte     First byte already removed from the STATUS PDU
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmUlHndlStatusPdu
(
KwCb       *gCb,
KwUlRbCb   *rbCb,
Buffer     *cntrlPdu,
U8         *fByte
)
#else
PRIVATE Void kwAmmUlHndlStatusPdu(gCb, rbCb, cntrlPdu, fByte)
KwCb       *gCb;
KwUlRbCb   *rbCb;
Buffer     *cntrlPdu;
U8         *fByte;
#endif
{
   U8             e1;
   KwExtHdr       hdrInfo;
   KwUdxStaPdu    *pStaPdu;
   KwUdxUlSapCb   *sapCb;

   TRC2(kwAmmUlHndlStatusPdu)


   KW_MEM_ZERO(&hdrInfo, sizeof(KwExtHdr));

   /* Extract the Control PDU */
   hdrInfo.hdr  = (*fByte << 1);
   hdrInfo.pLen = 4;

   /* D/C has been shifted in the calling function */
   if (hdrInfo.hdr & 0xE0)
   {
      RLOG_ARG2(L_ERROR,DBG_RBID,rbCb->rlcId.rbId,
               "Reserved value for CPT received UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);

      RETVOID;
   }

   sapCb = KW_GET_UDX_SAP(gCb);
   KW_ALLOC_SHRABL_BUF(sapCb->pst.region, 
                       sapCb->pst.pool, 
                       pStaPdu, 
                       sizeof(KwUdxStaPdu));

#if (ERRCLASS & ERRCLS_ADD_RES)
   /* Memory allocation failure can not be expected  */
   if(!pStaPdu)
   {
     RETVOID;
   }
#endif   
   pStaPdu->nackCnt = 0;
   /* For CPT */
   hdrInfo.hdr = hdrInfo.hdr << KW_CPT_LEN;

   /* ACK Sn */
   hdrInfo.len = KW_SN_LEN;
   kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
   pStaPdu->ackSn = hdrInfo.val;

   /* Check if NACK Exists */
   hdrInfo.len = KW_E1_LEN;
   kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
   e1 = (U8)hdrInfo.val;
   RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
            "kwAmmUlHndlStatusPdu: ACK SN = %d UEID:%d CELLID:%d",
            pStaPdu->ackSn,
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);

   /* If NACK exists in control PDU */
   /* For ACKs and NACKs */
   while (e1 && (pStaPdu->nackCnt < KW_MAX_NACK_CNT))
   {
      hdrInfo.len = KW_SN_LEN;
      kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      pStaPdu->nackInfo[pStaPdu->nackCnt].sn = hdrInfo.val;

      hdrInfo.len = KW_E1_LEN;
      kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      e1 = (U8)hdrInfo.val;

      /* Extract e2 */
      /* hdrInfo.len = KW_E1_LEN; --> previusly stored value (for e1) is
         already present*/
      kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
      /*  e2 = (U8) hdrInfo.val;*/

      /* Store e2 value */
      pStaPdu->nackInfo[pStaPdu->nackCnt].isSegment = (U8) hdrInfo.val;

      /* Test for resegmentation */
      if (pStaPdu->nackInfo[pStaPdu->nackCnt].isSegment)
      {
         hdrInfo.len = KW_SO_LEN;
         kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
         pStaPdu->nackInfo[pStaPdu->nackCnt].soStart = hdrInfo.val;

         kwAmmExtractElmnt(gCb, cntrlPdu, &hdrInfo);
         pStaPdu->nackInfo[pStaPdu->nackCnt].soEnd   = hdrInfo.val;

         RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                       "kwAmmUlHndlStatusPdu: soStart and soEnd = %d %d"
                       "UEID:%d CELLID:%d",
                       pStaPdu->nackInfo[pStaPdu->nackCnt].soStart,
                       pStaPdu->nackInfo[pStaPdu->nackCnt].soEnd,      
                       rbCb->rlcId.ueId,
                       rbCb->rlcId.cellId);
      }                                                                
      else
      {
         hdrInfo.len = 0;
         pStaPdu->nackInfo[pStaPdu->nackCnt].soStart = 0;
         pStaPdu->nackInfo[pStaPdu->nackCnt].soEnd   = 0;

      }
      pStaPdu->nackCnt++;
   }

   gRlcStats.amRlcStats.numULStaPduRcvd++;
   gRlcStats.amRlcStats.numULNackInStaPduRcvd += pStaPdu->nackCnt;

   /* In case we have reached the MAX NACK CNT, then we should modify the ACK_SN
      to the last NACK SN + 1 and discard the original ACK_SN*/
   if(pStaPdu->nackCnt == KW_MAX_NACK_CNT)
   {
      pStaPdu->ackSn = (pStaPdu->nackInfo[KW_MAX_NACK_CNT-1].sn + 1) & 0x3ff;
   }


   /* Parse & send Status PDU to RLC-DL */
   KwUlUdxStaUpdReq(&(sapCb->pst), sapCb->spId, &rbCb->rlcId, pStaPdu);

   RETVOID;
}

/**
 * @brief Private handler to release all stored segments
 *
 * @details
 *    Private handler invokded by kwAmmUlPlacePduInRecBuf to release the
 *    stored segements in case a complete PDU is received later.
 *
 * @param[in]  gCb      RLC instance control block
 * @param[in]  recBuf   Buffer that stores a received PDU or segments
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmUlRlsAllSegs
(
KwCb         *gCb,
KwAmRecBuf   *recBuf
)
#else
PRIVATE Void kwAmmUlRlsAllSegs(gCb,recBuf)
KwCb         *gCb;
KwAmRecBuf   *recBuf;
#endif
{
   KwSeg *seg;

   TRC2(kwAmmUlRlsAllSegs)

   KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
   while (seg != NULLP)
   {
      KW_FREE_BUF_WC(seg->seg);
      cmLListDelFrm(&(recBuf->segLst),&(seg->lstEnt));
      KW_FREE_WC(gCb,seg, sizeof(KwSeg));
      KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
   }

   RETVOID;
}

/**
 * @brief Private handler to store the received segment
 *
 * @details
 *    Private handler invokded by kwAmmUlPlacePduInRecBuf to add a received
 *    segment in reception buffer of a RBCB.
 *    - It is responsible for detecting duplicate segments
 *    - Adding it at appropriate position in the received buffer
 *    - Calling ExpByteSeg to set expSo field in the receiver buffer
 *
 * @param[in]  gCb      RLC instance control block
 * @param[in]  rbCb     Radio Bearer Contro Block
 * @param[in]  amHdr    AM Header received
 * @param[in]  pdu      Buffer received other than the headers
 * @param[in]  pduSz    size of the PDU buffer received
 *
 * @return  Bool
 *   -#TRUE  Successful insertion into the receiver buffer
 *   -#FALSE Possibly a duplicate segment
 */
#ifdef ANSI
PRIVATE Bool kwAmmAddRcvdSeg
(
KwCb       *gCb,
KwUlRbCb   *rbCb,
KwAmHdr    *amHdr,
Buffer     *pdu,
U16        pduSz
)
#else
PRIVATE Bool kwAmmAddRcvdSeg(gCb, rbCb, amHdr, pdu, pduSz)
KwCb       *gCb;
KwUlRbCb   *rbCb;
KwAmHdr    *amHdr;
Buffer     *pdu;
U16        pduSz;
#endif
{
   KwAmRecBuf   *recBuf;
   KwSeg        *seg;
   KwSeg        *tseg;
   U16          soEnd;       /* Holds the SoEnd of received segment */
   U16          expSo = 0;   /* Expected SO */

   TRC2(kwAmmAddRcvdSeg)

   soEnd = amHdr->so + pduSz - 1;

   if (AMUL.recBuf[amHdr->sn] == NULLP)
   {
      KW_ALLOC(gCb,recBuf, sizeof(KwAmRecBuf));
#if (ERRCLASS & ERRCLS_ADD_RES)
      if (recBuf == NULLP)
      {
         RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
                  "Memory allocation failed UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);

         KW_FREE_BUF(pdu);
         RETVALUE(FALSE);
      }
#endif /* ERRCLASS & ERRCLS_RES */

      AMUL.recBuf[amHdr->sn] = recBuf;
   }
   else
   {
      recBuf = AMUL.recBuf[amHdr->sn];

      if (recBuf->allRcvd == TRUE)
      {
         KW_FREE_BUF(pdu);
         RETVALUE(FALSE);
      }
   }

   /* kw003.201 - Move past the segments that are different than the */
   /*             one received.                                      */
   KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
   while ((seg != NULLP) && (seg->amHdr.so < amHdr->so))
   {
      expSo = seg->amHdr.so + seg->segSz;
      KW_LLIST_NEXT_SEG(recBuf->segLst, seg);
   }

   /* The received segment should start after the end of previous seg */
   if (expSo > amHdr->so)
   {
      /* This is a duplicate segment */
      KW_FREE_BUF(pdu);
      RETVALUE(FALSE);
   }

   if ((seg) && (seg->amHdr.so <= soEnd))
   {
      /* This is a duplicate segment */
      KW_FREE_BUF(pdu);
      RETVALUE(FALSE);
   }

   /* If we have come this far, we have to add this segment to the   */
   /* reception buffer as we either have eliminated duplicates or    */
   /* have found none.                                               */
   KW_ALLOC_WC(gCb,tseg, sizeof(KwSeg));
#if (ERRCLASS & ERRCLS_ADD_RES)
   if (tseg == NULLP)
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
               "Memory allocation failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      KW_FREE_BUF(pdu);
      RETVALUE(FALSE);
   }
#endif /* ERRCLASS & ERRCLS_RES */

   tseg->seg = pdu;
   tseg->segSz = pduSz;
   KW_MEM_CPY(&tseg->amHdr, amHdr, sizeof(KwAmHdr));
   recBuf->amHdr.rf = 1;
   recBuf->amHdr.sn = amHdr->sn;
   tseg->soEnd = soEnd;
   if (seg == NULLP)
   {
      cmLListAdd2Tail(&recBuf->segLst, &tseg->lstEnt);
   }
   else
   {
      recBuf->segLst.crnt = &seg->lstEnt;
      cmLListInsCrnt(&recBuf->segLst, &tseg->lstEnt);
   }
   tseg->lstEnt.node = (PTR)tseg;
   kwAmmUpdExpByteSeg(gCb,&AMUL,tseg);

   RETVALUE(TRUE);
}

/**
 * @brief Private handler to place the PDU in the reception buffer
 *
 * @details
 *    This function checks if the received PDU's SN falls within the
 *    receiving window, after which it places the same in the reception
 *    buffer if its not a duplicate.
 *
 * @param[in]  gCb   RLC instance control block
 * @param[in]  pdu   Received PDU
 * @param[in]  rbCb  Uplink AM Radio Bearer
 * @param[out] amUl  AM UL Info
 *
 * @return Bool
 *     -# TRUE
 *     -# FALSE
 *
 */
#ifdef ANSI
PRIVATE Bool kwAmmUlPlacePduInRecBuf
(
KwCb       *gCb,
Buffer     *pdu,
KwUlRbCb   *rbCb,
KwAmHdr    *amHdr
)
#else
PRIVATE Bool kwAmmUlPlacePduInRecBuf(gCb, pdu, rbCb, amHdr)
KwCb       *gCb;
Buffer     *pdu;
KwUlRbCb   *rbCb;
KwAmHdr    *amHdr;
#endif
{
   KwSn     sn;
   MsgLen   pduSz;
   KwAmUl   *amUl = &(rbCb->m.amUl);

   TRC2(kwAmmUlPlacePduInRecBuf)


   sn = amHdr->sn;
   SFndLenMsg(pdu, &pduSz);

   gCb->genSts.bytesRecv += pduSz;

   if (!KW_AM_CHK_SN_WITHIN_RECV_WINDOW(sn, amUl))
   {
      RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
                    "kwAmmUlPlacePduInRecBuf: SN  %d outside the window"
                    "UEID:%d CELLID:%d",
                    sn,
                    rbCb->rlcId.ueId,
                    rbCb->rlcId.cellId);

      gCb->genSts.unexpPdusRecv++;
      KW_FREE_BUF(pdu);
      RETVALUE(FALSE);
   }
   if (amHdr->rf == 0)
   {
      KwAmRecBuf *recBuf;

      /* We received a complete PDU. Either we already have it, in which */
      /* case we just ignore the new PDU and discard it. Otherwise,      */
      /* store the received PDU in the reception buffer                  */
      if (amUl->recBuf[amHdr->sn] == NULLP)
      {
         KW_ALLOC(gCb,recBuf, sizeof(KwAmRecBuf));
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (recBuf == NULLP)
         {
            RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
                     "Memory allocation failed UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
            KW_FREE_BUF(pdu);
            RETVALUE(FALSE);
         }
#endif /* ERRCLASS & ERRCLS_RES */
         amUl->recBuf[sn] = recBuf;
      }
      else if (amUl->recBuf[amHdr->sn]->allRcvd != TRUE)
      {
         kwAmmUlRlsAllSegs(gCb,amUl->recBuf[amHdr->sn]);
         recBuf = amUl->recBuf[sn];
      }
      else
      {
         gCb->genSts.unexpPdusRecv++;
         KW_FREE_BUF(pdu);
         RETVALUE(FALSE);
      }
      recBuf->pdu = pdu;
      recBuf->pduSz = pduSz;
      recBuf->allRcvd = TRUE;
      KW_MEM_CPY(&recBuf->amHdr, amHdr, sizeof(KwAmHdr));
      RETVALUE(TRUE);
   }
   else
   {
      /* We received a segment. We need to add that to the existing */
      /* segments, if any.                                          */
      RETVALUE(kwAmmAddRcvdSeg(gCb,rbCb, amHdr, pdu, pduSz));
   }
}

/**
 * @brief Private handler to trigger status report
 *
 * @details
 *    Private handler invokded by kwAmmProcessPdus to check if the
 *    status report need to be sent, and update the status trigger
 *    flag accordingly based on status prohibit timer.
 *
 *    - Check if the received pdu's sn is less than vrMs, set the
 *      staTrg flag.
 *    - If staProhTmr is not running, calculate cntrlBo, else it'll be
 *      updated at the expiry of staProhTmr.
 *    - Expiry of reOrdTmr also will set staTrg flag.
 *
 * @param[in]  gCb       RLC instance control block
 * @param[in]  rbCb      Uplink RB control block
 * @param[in]  sn        Sequence number of the pdu based on which to check if
 *                       status needs to be triggered
 * @param[in]  discFlg   Whether this pdu was discarded or not
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmTriggerStatus
(
KwCb       *gCb,
KwUlRbCb   *rbCb,
KwSn       sn,
Bool       discFlg
)
#else
PRIVATE Void kwAmmTriggerStatus(gCb,rbCb, sn, discFlg)
KwCb       *gCb;
KwUlRbCb   *rbCb;
KwSn       sn;
Bool       discFlg;
#endif
{
   Bool     tmrRunning;
   KwSn     tSn;
   KwSn     tVrMr;
   KwSn     tVrMs;
   KwAmUl   *amUl = &(rbCb->m.amUl);

   TRC2(kwAmmTriggerStatus)


   MODAMR(amUl->vrMr, tVrMr, amUl->vrR);
   MODAMR(amUl->vrMs, tVrMs, amUl->vrR);
   MODAMR(sn , tSn, amUl->vrR);

   /* kw005.201 Product CR ccpu00117114       *
    * The "=" in the 2nd condition is removed */
   if ((discFlg) || (tSn < tVrMs) || (tSn >= tVrMr))
   {
      RLOG_ARG2(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
               "kwAmmTriggerStatus: Set Status Trigger UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);

      amUl->staTrg = TRUE;
      amUl->gatherStaPduInfo = FALSE;

      /* Check if staProhTmr is running */
      tmrRunning = kwChkTmr(gCb,(PTR) rbCb, KW_EVT_AMUL_STA_PROH_TMR);

      if (!tmrRunning)
      {
         amUl->gatherStaPduInfo = TRUE;
      }
   }

   RETVOID;
}

/**
 * @brief Private handler to get a portion of SDU
 *
 * @details
 *    Private handler invokded by kwAmmProcPduOrSeg to fetch a portion
 *    of SDU that is addressed by the LI at position liCnt. It also
 *    increments liCnt.
 *
 * @param[in,out] liCnt   refers to the SDU (portion) to be retrieved
 * @param[in]     amHdr   AM header carries the SDU size through LIs
 * @param[in,out] pdu     PDU from which SDU is formed
 * @param[out]    sdu     Updated and returned to the caller
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmUlGetSdu
(
KwCb      *gCb,
U16       *liCnt,
KwAmHdr   *amHdr,
Buffer    **pdu,
Buffer    **sdu
)
#else
PRIVATE Void kwAmmUlGetSdu
(
KwCb      *gCb;
U16       *liCnt,
KwAmHdr   *amHdr,
Buffer    **pdu,
Buffer    **sdu
)
#endif
{
   Buffer    *remPdu;

   TRC2(kwAmmUlGetSdu)

   if ((*liCnt) < amHdr->numLi)
   {
      /* Segment the PDU based on length in LI table */
      SSegMsg(*pdu, amHdr->li[*liCnt], &remPdu);
      *sdu = *pdu;
      *pdu = remPdu;
   }
   else
   {
      /* The entire remaining PDU forms the SDU */
      *sdu = *pdu;
      *pdu = NULLP;
   }
   (*liCnt)++;
   RETVOID;
}

/**
 * @brief Private handler to reassemble from a segment or a PDU
 *
 * @details
 *    Private handler invokded by kwAmmReassembleSdus with either a
 *    PDU or a segment of a PDU. This is also called in the case of
 *    reestablishment and hence out of sequence joining is also to
 *    be supported
 *
 *
 * @param[in]  gCb     RLC instance control block
 * @param[in]  rbCb    Uplink RB control block
 * @param[in]  amHdr   AM header received for this segment/PDU
 * @param[in]  pdu     PDU to be reassembled
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmProcPduOrSeg
(
KwCb       *gCb,
KwUlRbCb   *rbCb,
KwAmHdr    *amHdr,
Buffer     *pdu
)
#else
PRIVATE Void kwAmmProcPduOrSeg(gCb, rbCb, amHdr, pdu)
KwCb       *gCb;
KwUlRbCb   *rbCb;
KwAmHdr    *amHdr;
Buffer     *pdu;
#endif
{
   Buffer   *sdu;
   U16      liCnt = 0;

   TRC2(kwAmmProcPduOrSeg)

   if ((AMUL.expSn != amHdr->sn) || (AMUL.expSo != amHdr->so))
   {
      /* Release the existing partial SDU as we have PDUs or */
      /* segments that are out of sequence                   */
      rbCb->m.amUl.isOutOfSeq = TRUE;
      KW_FREE_BUF(AMUL.partialSdu);
   }

   if (amHdr->fi & KW_FI_FIRST_SEG)
   {
      /* The first byte is not first byte of any SDU. That would mean */
      /* that there should be a partial SDU. If it is not there we    */
      /* ignore this portion of the PDU                               */
      if (AMUL.partialSdu == NULLP)
      {
         kwAmmUlGetSdu(gCb,&liCnt, amHdr, &pdu, &sdu);
         KW_FREE_BUF(sdu);
      }
      else if (amHdr->numLi == 0)
      {
         SCatMsg(pdu, AMUL.partialSdu, M2M1);
         KW_FREE_BUF_WC(AMUL.partialSdu);
      }
      else
      {
         kwAmmUlGetSdu(gCb,&liCnt, amHdr, &pdu, &sdu);
         SCatMsg(sdu, AMUL.partialSdu, M2M1);
         KW_FREE_BUF_WC(AMUL.partialSdu);
         kwUtlSndDatInd(gCb, rbCb, sdu);
      }
   }
   else
   {
      /* This is the case where the first byte starts an SDU */
      /* Any existingg partial SDU is there by error         */
      if (AMUL.partialSdu != NULLP)
      {
         KW_FREE_BUF_WC(AMUL.partialSdu);
      }
   }

   /* The below while loop covers starting with second LI */
   while(liCnt < amHdr->numLi)
   {
      kwAmmUlGetSdu(gCb,&liCnt, amHdr, &pdu, &sdu);
      kwUtlSndDatInd(gCb, rbCb, sdu);
   }

   if (amHdr->fi & KW_FI_LAST_SEG)
   {
      /* Any left over PDU will be joined with the next PDU */
      AMUL.partialSdu = pdu;
   }
   else if (pdu != NULLP)
   {
      AMUL.partialSdu = NULLP;
      kwUtlSndDatInd(gCb,rbCb, pdu);
   }

   RETVOID;
}

/**
 *
 * @brief Private handler to reassemble SDUs
 *
 * @details
 *    Private handler invokded by kwAmmProcessPdus with the PDU
 *    from the reception buffer in sequence to reassemble SDUs and
 *    send it to PDCP.
 *
 *        - With the stored header info, FI and LSF segment / concatenate
 *          PDUs or byte segments of PDUs to get the associated SDU.
 *
 * @param[in]  rbCb     RB control block
 * @param[in]  pdu      PDU to be reassembled
 *
 *  @return  S16
 *      -# ROK
 *      -# RFAILED
 *
 */
#ifdef ANSI
PRIVATE S16 kwAmmUlReassembleSdus
(
KwCb         *gCb,
KwUlRbCb     *rbCb,
KwAmRecBuf   *recBuf
)
#else
PRIVATE S16 kwAmmUlReassembleSdus(gCb, rbCb, recBuf)
KwCb         *gCb;
KwUlRbCb     *rbCb;
KwAmRecBuf   *recBuf;
#endif
{
   KwSeg        *seg;

   TRC2(kwAmmUlReassembleSdus)
   if (recBuf->amHdr.rf == 0)
   {
      /* This is a PDU */
      kwAmmProcPduOrSeg(gCb,rbCb, &recBuf->amHdr, recBuf->pdu);
      AMUL.expSn = (recBuf->amHdr.sn + 1) & KW_MOD_1024; /* MOD 1024 */
      AMUL.expSo = 0;
   }
   else
   {
      /* This is a set of segments */
      KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
      AMUL.expSn = recBuf->amHdr.sn;
      AMUL.expSo = 0;
      while(seg)
      {
         kwAmmProcPduOrSeg(gCb,rbCb, &seg->amHdr, seg->seg);
         AMUL.expSo = seg->soEnd + 1;

         cmLListDelFrm(&(recBuf->segLst),&(seg->lstEnt));
         KW_FREE_WC(gCb, seg, sizeof(KwSeg));

         KW_LLIST_FIRST_SEG(recBuf->segLst, seg);
      }
      AMUL.expSn = (recBuf->amHdr.sn + 1) & KW_MOD_1024; /* MOD 1024 */
      AMUL.expSo = 0;
   }

   RETVALUE(ROK);
}

/**
 * @brief   Handler to process the re-establishment request received from UIM
 *
 * @param[in] gCb         RLC instance control block
 * @param[in] rlcId       RLC identifier
 * @param[in] sendReEst   Whether to send back restablishment complete or not
 * @param[in] rbCb        Uplink RB control block
 *
 * @return  Void
 *
 */
#ifdef ANSI
PUBLIC Void kwAmmUlReEstablish
(
KwCb         *gCb,
CmLteRlcId   rlcId,
Bool         sendReEst,
KwUlRbCb     *rbCb
)
#else
PUBLIC Void kwAmmUlReEstablish(gCb, rlcId, sendReEst, rbCb)
KwCb         *gCb;
CmLteRlcId   rlcId;
Bool         sendReEst;
KwUlRbCb     *rbCb;
#endif
{
   KwSn   sn;
   KwSn   mSn;
   KwSn   mVrMr;

#ifndef KW_PDCP
   KwKwuSapCb *kwKwSap;
#endif

   TRC2(kwAmmUlReEstablish);

#ifdef LTE_QCOM
   {
      extern U32 kwAmmReEstReqRcvd;
      kwAmmReEstReqRcvd++;
   }
#endif

   sn = AMUL.vrR;

   MODAMR(AMUL.vrMr, mVrMr, AMUL.vrR);
   MODAMR(sn, mSn, AMUL.vrR);

   /* Reassemble SDUs from PDUs with SN less than upper edge of the window */
   while (mSn < mVrMr)
   {
      if (AMUL.recBuf[sn] != NULLP)
      {
         if (AMUL.recBuf[sn]->allRcvd == TRUE)
         {
            kwAmmUlReassembleSdus(gCb,rbCb, AMUL.recBuf[sn]);
         }
         else
         {
            /* Remove PDU and segments */
            if(AMUL.recBuf[sn]->pdu)
            {
               KW_FREE_BUF_WC(AMUL.recBuf[sn]->pdu);
            }
            /* Release all the segments*/
            kwAmmUlRlsAllSegs(gCb,AMUL.recBuf[sn]);
         }

         KW_FREE_WC(gCb,AMUL.recBuf[sn], sizeof(KwAmRecBuf));
         AMUL.recBuf[sn]=NULLP; 
      }
      sn = (sn + 1) & KW_MOD_1024; /* MOD 1024 */
      MODAMR(sn, mSn, AMUL.vrR);
   }
   /* Discard remaining PDUs and bytesegments in recBuf */

   /* Stop all timers and reset variables */
   if(TRUE == kwChkTmr(gCb,(PTR)rbCb,KW_EVT_AMUL_REORD_TMR))
   {
       kwStopTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
   }
   if(TRUE == kwChkTmr(gCb,(PTR)rbCb,KW_EVT_AMUL_STA_PROH_TMR))
   {
       kwStopTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_STA_PROH_TMR);
   }

   AMUL.vrR  = 0;
   AMUL.vrH  = 0;
   AMUL.vrX  = 0;
   rbCb->m.amUl.vrMr = (rbCb->m.amUl.vrR + KW_AM_WIN_SZ) & KW_MOD_1024;
   AMUL.vrMs = 0;
   AMUL.staTrg  = FALSE;
   AMUL.gatherStaPduInfo = FALSE;
   AMUL.expSn = 0;
   AMUL.expSo = 0;
   if (AMUL.partialSdu != NULLP)
   {
     KW_FREE_BUF(AMUL.partialSdu);
   }
   kwKwSap = gCb->u.ulCb->kwuUlSap + KW_UI_PDCP;

   if(sendReEst)
   {
      KwUiKwuReEstCmpInd(&kwKwSap->pst, kwKwSap->suId, rlcId);
      rbCb->m.amUl.isOutOfSeq = FALSE;
   }

   RETVOID;
}

/**
 * @brief  Handler for reorder timer expiry
 *
 * @details
 *    This function is used to handle events upon expiry of reorder timer
 *
 *  @param[in] gCb   RLC instance control block
 *  @param[in] rbCb  RB control block
 *
 *  @return  Void
 *
 */

#ifdef ANSI
PUBLIC Void kwAmmReOrdTmrExp
(
KwCb        *gCb,
KwUlRbCb    *rbCb
)
#else
PUBLIC Void kwAmmReOrdTmrExp(rbCb)
KwCb        *gCb;
KwUlRbCb    *rbCb;
#endif
{
   KwAmUl *amUl = &(rbCb->m.amUl);
   KwSn sn;
   KwSn mSn;
   KwSn mVrMr;
   KwSn mVrMs;
   KwSn mVrH;
   Bool tmrRunning = FALSE;

   TRC2(kwAmmReOrdTmrExp);


   /* Update vrMs */
   sn = amUl->vrX;

   MODAMR(sn, mSn, amUl->vrR);
   MODAMR(amUl->vrMr, mVrMr, amUl->vrR);

   while (mSn < mVrMr)
   {
      if ( (amUl->recBuf[sn] == NULLP) ||
            ((amUl->recBuf[sn] != NULLP) &&
            (!amUl->recBuf[sn]->allRcvd)) )
      {
         amUl->vrMs = sn;
         amUl->staTrg = TRUE;
         amUl->gatherStaPduInfo = FALSE;
         /* Check if staProhTmr is running */
         tmrRunning = kwChkTmr(gCb,(PTR) rbCb, KW_EVT_AMUL_STA_PROH_TMR);

         if (!tmrRunning)
         {
            gRlcStats.amRlcStats.numULReOrdTimerExpires++;
            amUl->gatherStaPduInfo = TRUE;
            kwAmmUlAssembleCntrlInfo(gCb, rbCb);
         }

         break;
      }
      sn = (sn + 1) & KW_MOD_1024; /* MOD 1024 */
      MODAMR(sn, mSn, amUl->vrR);
   }

   /* Update vrX */
   MODAMR(amUl->vrH, mVrH, amUl->vrR);
   MODAMR(amUl->vrMs, mVrMs, amUl->vrR);
   if (mVrH > mVrMs)
   {
      kwStartTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
      amUl->vrX = amUl->vrH;
   }

   RETVOID;
} /* kwAmmReOrdTmrExp */

/**
 * @brief  Handler for status prohibit timer expiry
 *
 * @details
 *    This function is used to handle events upon expiry of status prohibit
 *    timer
 *
 *  @param[in] gCb   RLC instance control block
 *  @param[in] rbCb   RB control block
 *
 *  @return  Void
 *
 */

#ifdef ANSI
PUBLIC Void kwAmmStaProTmrExp
(
KwCb        *gCb,
KwUlRbCb    *rbCb
)
#else
PUBLIC Void kwAmmStaProTmrExp(gCb, rbCb)
KwCb        *gCb;
KwUlRbCb    *rbCb;
#endif
{
   KwAmUl *amUl = &(rbCb->m.amUl);

   TRC2(kwAmmStaProTmrExp);


   amUl->gatherStaPduInfo = FALSE;

   if (amUl->staTrg == TRUE)
   {
      amUl->gatherStaPduInfo = TRUE;
      /* kw002.201 : Sending StaRsp after StaProhibit tmr expiry */
      kwAmmUlAssembleCntrlInfo(gCb,rbCb);
   }

   RETVOID;
} /* kwAmmStaProTmrExp */

/**
 * @brief  Handler to extract an element of AM Header
 *
 * @details
 *    This function is used to extract an element of AM header.
 *
 * @param[in]     pdu      The pdu to be decoded
 * @param[in,out] hdrInfo  Container to hold the decoded info
 *
 * @return Void
 *
 */

#ifdef ANSI
PRIVATE Void kwAmmExtractElmnt
(
KwCb       *gCb,
Buffer     *pdu,
KwExtHdr   *hdrInfo
)
#else
PRIVATE Void kwAmmExtractElmnt(gCb, pdu, hdrInfo)
KwCb       *gCb;
Buffer     *pdu;
KwExtHdr   *hdrInfo;
#endif
{
   U8   hdr;
   U8   pLen = hdrInfo->pLen;
   U8   len  = (U8)hdrInfo->len;
   U16  val;
   U8   tHdr;
   U8   fLen;
   U8   rLen;
   /* U8   rLen1 = 0; */
   U16  tVal;

   TRC2(kwAmmExtractElmnt);

   hdr = hdrInfo->hdr;

   if (pLen == 0)
   {
      SRemPreMsg(&hdr, pdu);
      pLen = 8;
   }
   tHdr = hdr;
   if (len <= 8)
   {
      val = tHdr >> (KW_BYTE_LEN - (len));
      hdr =  hdr << len;
      pLen -= len;
   }
   else /*if (len > 8) */
   {
      fLen = pLen;
      val = tHdr;
      val = val >> (KW_BYTE_LEN - fLen);
      val = val << (len - fLen);
      rLen = len - fLen;
      SRemPreMsg(&hdr, pdu);
      tHdr = hdr;
      if (rLen <= 8)
      {
         hdr = hdr >> (KW_BYTE_LEN - rLen);
         val = val | hdr;
         hdr = tHdr << rLen;
         pLen = (KW_BYTE_LEN - rLen);
      }
      else
      {
        rLen = rLen - KW_BYTE_LEN;
        tVal = hdr;
        tVal = tVal << rLen;
        val = val | tVal;

        SRemPreMsg(&hdr, pdu);
        tHdr = hdr;
        hdr = hdr >> (KW_BYTE_LEN - rLen);
        val = val | hdr;
        hdr = tHdr << rLen;
        pLen = (KW_BYTE_LEN - rLen);
      }
   }

   hdrInfo->pLen = pLen;
   hdrInfo->hdr = hdr;
   hdrInfo->val = val;

   RETVOID;
}

/**
 * @brief  Handler to updated expected byte seg
 *
 * @details
 *    This function is used to update expected byte segment. The next segment
 *    expected is indicated by the SO of the segment which is expected. Intially
 *    the segment with SO 0 is expected and then in order. When all the segments
 *    are received (which would happen when an expected SO is encountered
 *    with LSF set) the allRcvd flag is set to TRUE
 *
 * @param[in]  gCb   RLC instance control block
 * @param[in]  amUl  AM Uplink Control Block
 * @param[in]  seg   Newly received segment
 *
 * @return Void
 *
 */

#ifdef ANSI
PRIVATE Void kwAmmUpdExpByteSeg
(
KwCb     *gCb,
KwAmUl   *amUl,
KwSeg    *seg
)
#else
PRIVATE Void kwAmmUpdExpByteSeg(gCb, amUl, seg)
KwCb     *gCb;
KwAmUl   *amUl;
KwSeg    *seg;
#endif
{
#if 1
   U16    newExpSo; /* The new expected SO */
   KwSn   sn = seg->amHdr.sn;
   Bool   lstRcvd;

   TRC2(kwAmmUpdExpByteSeg);


   if (seg->amHdr.so != amUl->recBuf[sn]->expSo)
   {
      RETVOID;
   }

   newExpSo   = seg->soEnd + 1;
   amUl->recBuf[sn]->expSo = newExpSo;
   lstRcvd = seg->amHdr.lsf;
   /* kw003.201 - This should update seg with the one after newSeg */
   KW_LLIST_NEXT_SEG(amUl->recBuf[sn]->segLst, seg);
   while(seg)
   {
      /* keep going ahead as long as the expectedSo match with the header so
         else store the expSo for later checking again */
      if (seg->amHdr.so == newExpSo)
      {
         newExpSo = seg->soEnd + 1;
         amUl->recBuf[sn]->expSo = newExpSo;
         lstRcvd = seg->amHdr.lsf;
         KW_LLIST_NEXT_SEG(amUl->recBuf[sn]->segLst, seg);
      }
      else
      {
         amUl->recBuf[sn]->expSo = newExpSo;
         RETVOID;
      }
   }
#else /* CR_125447 Workaround*/ 
   U16    newExpSo=0; /* The new expected SO */
   KwSn   sn = seg->amHdr.sn;
   Bool   lstRcvd = FALSE;
   KwSeg  *tseg;

   TRC2(kwAmmUpdExpByteSeg);

   UNUSED(seg); /*TODO should be removed */

   KW_LLIST_FIRST_SEG(amUl->recBuf[sn]->segLst, tseg);
   while(tseg)
   {
      /* keep going ahead as long as the expectedSo match with the header so
         else store the expSo for later checking again */
      if (tseg->amHdr.so == newExpSo)
      {
         newExpSo = tseg->soEnd + 1;
         lstRcvd = tseg->amHdr.lsf;
         KW_LLIST_NEXT_SEG(amUl->recBuf[sn]->segLst, tseg);
      }
      else
      {
         amUl->recBuf[sn]->expSo = newExpSo;
         RETVOID;
      }
   }
#endif
   if (lstRcvd == TRUE)
   {
      amUl->recBuf[sn]->allRcvd = TRUE;
   }

   RETVOID;
}

/**
 * @brief
 *   Function to release/free the Acknowledged Mode Module  RbCb buffers
 *
 * @details
 *   This primitive Frees the AM RbCb transmission Buffer, retransmission
 *   Buffer and reciption Buffers
 *
 * @param [in]   gCb   - RLC instance Control Block
 * @param [in]   rbCb  - RB Control Block
 *
 * @return   void
 */
#ifdef ANSI
PUBLIC Void kwAmmFreeUlRbCb
(
KwCb       *gCb,
KwUlRbCb   *rbCb
)
#else
PUBLIC Void kwAmmFreeUlRbCb(gCb,rbCb)
KwCb       *gCb;
KwUlRbCb   *rbCb;
#endif
{
   KwSn         curSn = 0;           /* Sequence number of PDU */
   KwSn         windSz;              /* PDU window size */
   KwAmRecBuf   **amRecBuf;          /* AM moudel receive buffer */

   TRC2(kwAmmFreeUlRbCb)


   windSz  = KW_AM_WIN_SZ << 1;
   amRecBuf = rbCb->m.amUl.recBuf;

   if(TRUE == kwChkTmr(gCb,(PTR)rbCb,KW_EVT_AMUL_REORD_TMR))
   {
      kwStopTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_REORD_TMR);
   }
   if(TRUE == kwChkTmr(gCb,(PTR)rbCb,KW_EVT_AMUL_STA_PROH_TMR))
   {
      kwStopTmr(gCb,(PTR)rbCb, KW_EVT_AMUL_STA_PROH_TMR);
   }


   /* on the first loop winSz is always greater than zero
    while( ( curSn < windSz ) hence changing to do while */
   do
   {
      if ( amRecBuf[curSn] != NULLP )
      {
         if (amRecBuf[curSn]->pdu != NULLP)
         {
            KW_FREE_BUF_WC(amRecBuf[curSn]->pdu);
         }
         /* Release all the segments */
         kwAmmUlRlsAllSegs(gCb,amRecBuf[curSn]);
         KW_FREE_WC(gCb, amRecBuf[curSn],sizeof(KwAmRecBuf));
      }
      curSn++;
   }while ( curSn < windSz );

   KW_FREE(gCb,rbCb->m.amUl.recBuf, (windSz)* sizeof(KwAmRecBuf*));

   if(rbCb->m.amUl.partialSdu != NULLP)
   {
      KW_FREE_BUF_WC(rbCb->m.amUl.partialSdu);
   }
   RETVOID;
} /* kwAmmFreeUlRbCb */


/*@}*/


/********************************************************************30**

         End of file:     gp_amm_ul.c@@/main/TeNB_Main_BR/8 - Mon Aug 11 16:43:13 2014

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
/main/1      ---       nm         1. Initial release.
*********************************************************************91*/
