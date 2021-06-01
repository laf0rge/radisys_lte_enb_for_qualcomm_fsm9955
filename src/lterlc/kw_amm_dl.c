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
  
        Name:    RLC - AM DL module file
    
        Type:    C source file
  
        Desc:    Source code for Acknowledged Mode Module functions such as, 

                 Transmission of data/control PDUs 
                 Retransmission (Feedback in terms of status)
                 Polling
                 Assemble SDUs
                 Reception - reordering
                 Duplicate detection for byte segments
                 Reassemble SDUs
                  
        File:    gp_amm.c

        Sid:      gp_amm_dl.c@@/main/TeNB_Main_BR/9 - Mon Aug 11 16:43:12 2014
 
        Prg:     nm
  
*********************************************************************21*/
static const char* RLOG_MODULE_NAME="AMM";
static int RLOG_MODULE_ID=2048;
static int RLOG_FILE_ID=189;
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
#include "kw_err.h"        /* Err defines */
#include "kw_env.h"        /* RLC environment options */

#include "kw.h"            /* RLC defines */
#include "kw_udx.h"
#include "kw_dl.h"


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
#include "kw_udx.x"
#include "kw_dl.x"

//UDAY
#ifdef L2_OPTMZ
extern U32 kwAmmStaPduList[128];
 U32 kwAmmStaPduListCnt = 0;
#endif

#ifdef QCOM_STATS
 PUBLIC U32 kwDlrate_kwu;
#endif


/** @file gp_amm_dl.c
@brief RLC Acknowledged Mode Downlink Module
**/
#define KW_MODULE (KW_DBGMASK_AM | KW_DBGMASK_DL)

U32 kwStatusPduCnt, kwStatusAckCnt, kwStatusNcnt, kwSduSndCnt;

/* local defines */

/* local externs */

/* forward references */
EXTERN Void kwAmmDlHndlStatusPdu ARGS ((KwCb  *gCb,
                                        KwDlRbCb  *rbCb,
                                        KwUdxStaPdu *pStaPdu));

/* public variable declarations */

/* This structure holds all the global structs we need. */

/* private variable declarations */

#define KW_AM_RMV_HDR(_gCb, _rbCb, _retx) do { \
  if ((_retx)->yetToConst == FALSE) \
  {\
     Buffer    *_tSeg; \
     SSegMsg((_retx)->seg, (_retx)->hdrSz, &_tSeg); \
     KW_FREE_BUF((_retx)->seg); \
     (_retx)->seg = _tSeg; \
  }\
  (_rbCb)->m.amDl.estHdrSz -= retx->hdrSz;\
} while(0)

/* private function declarations */

PRIVATE Void  kwResegRetxPdus ARGS ((KwCb *gCb,
                                     KwDlRbCb *rbCb, 
                                     KwDatReq *kwDatReq));

PRIVATE Void kwRemRetxPdu ARGS ((KwCb *gCb, 
                                 KwDlRbCb *rbCb, 
                                 KwRetx *retx));

PRIVATE Void kwAmmCreateStatusPdu ARGS ((KwCb *gCb, 
                                         KwDlRbCb *rbCb,
                                         KwDatReq *kwDatReq));

PRIVATE Void kwAmmCalcBytesReqdForNacks ARGS ((KwNackInfo *nackInfo, 
                                               U16 *numOfBytes,
                                               U16 *numOfBits));

PRIVATE Void kwFormDlCntrlBits ARGS ((KwCb *gCb,
                                      KwCntrlInfo *cntrlInfo,
                                      U16 *staPdu,
                                      U16 *maxCPduLen));

PRIVATE Void kwAmmDlMarkPduForReTx ARGS ((KwCb *gCb,
                                          KwDlRbCb *rbCb,
                                          KwRetx *retx));

PRIVATE Void kwAmmDlProcessSuccessfulTxPdu ARGS((KwCb *gCb, 
                                                 KwDlRbCb *rbCb,
                                                 KwSn  sn,
                                                 KwuDatCfmInfo **datCfm));

PRIVATE Void kwAmmDlSetVtAAndVtMS ARGS((KwAmDl *amDl, KwSn sn));

PRIVATE Void kwAmmDlCheckAndStopPollTmr ARGS((KwCb *gCb,
                                              KwDlRbCb *rbCb,
                                              KwSn mAckSn));

PRIVATE Void  kwAssembleSdus ARGS ((KwCb *gCb,
                                    KwDlRbCb *rbCb, 
                                    KwDatReq *kwDatReq));

PRIVATE Bool kwAmmDlCheckAndSetPoll ARGS ((KwCb *gCb,
                                           KwDlRbCb *rbCb, 
                                           Bool newPdu, 
                                           MsgLen bufSz));

PRIVATE Void kwAmmCreatePdu ARGS ((KwCb *gCb,
                                   KwDlRbCb *rbCb, 
                                   KwAmHdr *amHdr, 
                                   Buffer *pdu));

PRIVATE Void kwAmmEstHdrSz ARGS ((KwCb *gCb, KwAmDl *amDl, U8 type));

PRIVATE Void kwAmmSndStaInd ARGS ((KwCb *gCb,KwDlRbCb *rbCb, KwRetx *retx));

PRIVATE Void kwGetNxtRetx ARGS ((KwCb *gCb, KwRetx **retx));

PRIVATE Void kwConstructAmHdr ARGS ((KwCb *gCb,
                                     KwAmHdr *amHdr, 
                                     Bool isSeg, 
                                     U8 *hdr, 
                                     U16 *idx));

PRIVATE Void kwFormBits ARGS ((KwHdrInfo *hdrInfo));

PRIVATE Void kwAmmDlUpdateTxAndReTxBufForAckSn ARGS ((KwCb *gCb,
                                                      KwDlRbCb *rbCb, 
                                                      KwSn mAckSn,
                                                      CmLList *retx,
                                                      KwuDatCfmInfo **datCfm));

PRIVATE Void kwAmmDlMoveFrmTxtoRetxBuffer ARGS ((KwCb *gCb,
                                                 KwAmDl *amDl, 
                                                 KwRetx **retx, 
                                                 KwSn sn));
 
PRIVATE Void kwAmmDlCheckIsSDUDelivered ARGS((KwCb *gCb,
                                              KwDlRbCb *rbCb, 
                                              KwSduMap *sduLst, 
                                              U8 numSdu,
                                              KwuDatCfmInfo **datCfm));

/*****************************************************************************

  AM Module contains the following funcitons:

  -  kwAmmQSdu 
  -  kwAmmProcessSdus
     -  kwAmmDlAssembleCntrlInfo
     -  kwResegRetxPdus
     -  kwAssembleSdus
     -  kwAmmDlCheckAndSetPoll
  -  kwAmmProcessPdus
     -  kwDlmHndlStaRsp
     -  kwTriggerStatus
     -  kwReassembleSdus
   
*******************************************************************************/
/** @addtogroup ammode */
/*@{*/

/**
 * @brief Function to send a Status Response to MAC for a dedicated logical
 *        channel
 *       
 * @details
 *    Function calculates the current bo and send a Status response for the
 *    dedicated logical channel if the bo is non zero
 *
 * @param[in] gCb    RLC instance control block
 * @param[in] rbCb   Radio Bearer control block
 * @param[in] amDl   AM downlink control block
 * 
 * @return  Void
*/
#ifdef ANSI
PUBLIC Void kwAmmSendDStaRsp
(
KwCb       *gCb,
KwDlRbCb   *rbCb,
KwAmDl     *amDl
)
#else
PUBLIC Void kwAmmSendDStaRsp(gCb, rbCb, amDl)
KwCb       *gCb;
KwDlRbCb   *rbCb;
KwAmDl     *amDl;
#endif
{
   S32 bo = kwAmmCalculateBo(amDl);

   if(bo)
   {
      kwUtlSndDStaRsp(gCb, rbCb, bo, amDl->estHdrSz, amDl->cntrlBo ?TRUE:FALSE);
   }
   
   RETVOID;
}

/**
 * @brief Function to check if the pollSn is acked and stop the poll timer
 *
 * @param[in] gCb      RLC instance control block
 * @param[in] rbCb     Radio Bearer control block
 * @param[in] mAckSn   The last received ACKSN. The base modulus value should
 *                     be passed
 * 
 * @return  Void
*/
#ifdef ANSI
PRIVATE Void kwAmmDlCheckAndStopPollTmr
(
KwCb       *gCb,
KwDlRbCb   *rbCb,
KwSn       mAckSn
)
#else
PRIVATE Void kwAmmDlCheckAndStopPollTmr(gCb, rbCb, mAckSn)
KwCb       *gCb;
KwDlRbCb   *rbCb;
KwSn       mAckSn;
#endif
{
   KwSn mPollSn;                                                   
                                                                   
   MODAMT(rbCb->m.amDl.pollSn, mPollSn, rbCb->m.amDl.vtA);      
                                                                   
   if (mPollSn <= mAckSn)                                        
   {                                                               
      if (kwChkTmr(gCb, (PTR)rbCb, KW_EVT_AMDL_POLL_RETX_TMR))    
      {                                                            
         kwStopTmr(gCb, (PTR)rbCb, KW_EVT_AMDL_POLL_RETX_TMR);    
      }                                                            
   }

   RETVOID;
}

/**
 * @brief Function to set VT(A) and VT(MS). Calculates the VT(MS) from VT(A)
 *
 * @param[in,out] amDl    AM downlink control block
 * @param[in]sn           Sequence number to be set as VT(A)
 * 
 * @return  Void
*/
#ifdef ANSI
PRIVATE Void kwAmmDlSetVtAAndVtMS
(
KwAmDl   *amDl,
KwSn      sn
)
#else
PRIVATE Void kwAmmDlSetVtAAndVtMS(amDl, sn)
KwAmDl   *amDl;
KwSn     sn
#endif
{
   amDl->vtA = sn;
   amDl->vtMs = (sn + KW_AM_WIN_SZ) & 0x3ff;
   
   RETVOID;
}

/**
 * @brief Function to process a successfully re-transmitted PDU/segment
 *       
 * @details
 *    Checks if the SDU has been completely delivered or not. Removes the PDU
 *    from the re-transmission buffer
 *
 * @param[in] gCb    RLC instance control block
 * @param[in] rbCb   Downlink Radio Bearer control block
 * @param[in] retx   The PDU/segment which was successfully re-transmitted
 * 
 * @return  Void
*/
#ifdef ANSI
PRIVATE Void kwAmmDlProcessSuccessfulReTx
(
KwCb            *gCb,
KwDlRbCb        *rbCb,
KwRetx          *retx,
KwuDatCfmInfo   **datCfm
)
#else
PRIVATE Void kwAmmDlProcessSuccessfulReTx(gCb, rbCb, retx, datCfm)
KwCb            *gCb;
KwDlRbCb        *rbCb;
KwRetx          *retx;
KwuDatCfmInfo   **datCfm;
#endif
{
   kwAmmDlCheckIsSDUDelivered(gCb, rbCb, retx->sduMap, retx->numSdu, datCfm);
   
   kwRemRetxPdu(gCb, rbCb, retx);

   RETVOID;
}
/**
 * @brief Function to update transmission buffers and send confimations to
 *        PDCP on the reception of Status PDU
 *
 * @details
 *    First processes the NACKs received
 *    -# Removes the pdus which are acked by each of the NACK SN from the
 *       transmission and re-transmission buffer
 *    -# If NACKed SN in in the transmisson buffer, moves it to re-transmission
 *       buffer
 *    -# Removes PDU segments of the NACKed SN which have been successfully 
 *       received by the other end. For the un-successful ones, marks them for
 *       re-transmission
 *    -# When PDUs/segments are removed from the buffer, indicates to upper
 *       layer if the SDU is completely delivered
 *    -# Removes the PDUs/segments which are acked by the ACK_SN but not by the
 *       NACK_SNs
 *
 * @param[in] gCb         RLC Instance control block
 * @param[in] rbCb        Downlink Radio Bearer control block
 * @param[in] pStaPdu     The decoded Status Pdu
 * 
 * @return  Void
*/
#ifdef ANSI
PUBLIC Void kwAmmDlHndlStatusPdu
(
KwCb          *gCb,
KwDlRbCb      *rbCb,
KwUdxStaPdu   *pStaPdu
)
#else
PUBLIC Void kwAmmDlHndlStatusPdu(gCb, rbCb, pStaPdu)
KwCb          *gCb;
KwDlRbCb      *rbCb;
KwUdxStaPdu   *pStaPdu;
#endif
{
   KwRetx    *retx;
   KwSn      mSn;
   KwSn      mAckSn;
   S32       oldRetxBo;
   CmLList   *retxNode;
   KwuDatCfmInfo* datCfm; 
   KwKwuSapCb    *kwuSap;
   KwSn      mVts;

   TRC2(kwAmmDlHndlStatusPdu)
   kwStatusPduCnt++;

   AMDL.numSduPdu = 0;
   kwuSap = gCb->u.dlCb->kwuDlSap + KW_UI_PDCP;
   /* store the re-transmission bo, to check if it changes due to the
      processing of the status pdu */
   oldRetxBo = AMDL.retxBo;

   /* Allocate memory for datCfm Info */
   KW_SHRABL_STATIC_BUF_ALLOC(kwuSap->pst.region, kwuSap->pst.pool, datCfm, sizeof(KwuDatCfmInfo));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (datCfm == NULLP)
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
               "Memory allocation failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      KW_SHRABL_STATIC_BUF_FREE(kwuSap->pst.region, kwuSap->pst.pool, datCfm, sizeof(KwuDatCfmInfo));
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_RES */

   datCfm->numSduIds = 0;
   datCfm->rlcId = rbCb->rlcId;

   MODAMT(pStaPdu->ackSn, mAckSn, AMDL.vtA);
   MODAMT(AMDL.vtS,mVts, AMDL.vtA);

   if(mAckSn > mVts)
   {
      RLOG_ARG4(L_WARNING,DBG_RBID, rbCb->rlcId.rbId, 
               "Invalid ACK SN = %d received. Current Vta =%d"
               "UEID:%d CELLID:%d", 
               pStaPdu->ackSn,
               AMDL.vtA,
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
/*      KW_SHRABL_STATIC_BUF_ALLOC(kwuSap->pst.region, kwuSap->pst.pool, datCfm, sizeof(KwuDatCfmInfo)); */
      KW_SHRABL_STATIC_BUF_FREE(kwuSap->pst.region, kwuSap->pst.pool, datCfm, sizeof(KwuDatCfmInfo));
      RETVOID;
   }
 
   /* Venki - stopping the poll retx timer */
   /*Stop PollRetx Tmr */
   kwAmmDlCheckAndStopPollTmr(gCb, rbCb, mAckSn);

   /* Set the first node in retx list to retxNode */
   retxNode = AMDL.retxLst.first;

   /* If NACK exists in control PDU */
   if (pStaPdu->nackCnt)
   {
      U8     e2;
      KwSn   sn;
      KwSn   soStart;
      KwSn   soEnd;
      KwSn   nackSn;
      KwSn   mNackSn;
      KwSn   vtA             = AMDL.vtA;
      KwSn   transWinStartSn = AMDL.vtA; /*used to track the SN from which 
                                           to start processing the transmission
                                           buffer */
      U32    idx = 0;

      /* if any NACKs then VT(A) should be equal to the first NACK_SN*/
      if(pStaPdu->nackCnt)
      {
         vtA = pStaPdu->nackInfo[0].sn;
      }

      kwStatusNcnt += pStaPdu->nackCnt;

      /* For NACKs */
      while (idx < pStaPdu->nackCnt)
      {
         nackSn = pStaPdu->nackInfo[idx].sn;
         
         RLOG_ARG3(L_DEBUG,DBG_RBID, rbCb->rlcId.rbId, 
                  "kwHndlStaRsp: NACK SN = %d UEID:%d CELLID:%d",
                  nackSn,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);

         soStart = pStaPdu->nackInfo[idx].soStart;
         soEnd   = pStaPdu->nackInfo[idx].soEnd;

         /* e2 is used as a boolean indicating presence of SOStart or SOEnd */
         e2 = (soStart | soEnd);

         sn = transWinStartSn;

         /* move transWinStartSn to nackSn + 1, as the pdu's before that
            will be removed from the buffer */
         transWinStartSn = (nackSn + 1) & 0x3ff;

         /* Clear the acked SNs from the retx list */
         MODAMT(nackSn, mNackSn, AMDL.vtA);

         if ((mNackSn > mAckSn) || (mNackSn >= mVts))
         {
            /* Erroneous NACK_SN, we should raise an error towards L3 */
            RLOG_ARG2(L_ERROR,DBG_RBID, rbCb->rlcId.rbId, 
                     "Status Pdu is not correct UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
            KW_SHRABL_STATIC_BUF_FREE(kwuSap->pst.region, kwuSap->pst.pool, datCfm, sizeof(KwuDatCfmInfo));
            RETVOID;
         }

         /* clear all the SNs < NACK_SN from re-transmission list */   
         while (retxNode)
         {
            retx = (KwRetx *)(retxNode->node);
            MODAMT(retx->amHdr.sn, mSn, AMDL.vtA);
            if (mSn < mNackSn)
            {
               retxNode = retxNode->next;
               kwAmmDlProcessSuccessfulReTx(gCb,rbCb, retx, &datCfm);
            }
            else
            {
               break;
            }
         }

         /* Remove all pdus with SN < NACK_SN from the transmission buffer */ 
         MODAMT(sn, mSn, AMDL.vtA);
         while (mSn < mNackSn)
         {
            /* this if check seems redundant,why should mSn ever be mTxSn 
               (which actually is VT(A) */
            if ((AMDL.txBuf[sn] != NULLP)) 
            {
               RLOG_ARG3(L_DEBUG,DBG_RBID, rbCb->rlcId.rbId, 
                        "kwHndlStaRsp: Handle ACK (sn = %d) UEID:%d CELLID:%d",
                        sn,
                        rbCb->rlcId.ueId,
                        rbCb->rlcId.cellId);
               
               /* Remove pdus from txBuf */
               kwAmmDlProcessSuccessfulTxPdu(gCb,rbCb, sn, &datCfm);
            }
 
            sn = (sn + 1) & 0x3ff;
            MODAMT(sn, mSn, AMDL.vtA);
         }

         /* Now process the NACK_SN received. Now the NACK_SN is    */
         /* either the first element of RETX or is in TX array      */
         /* To remove the remaining acks from the pdu byte segments */
         
         /* if the NACK_SN is in the transmit buffer, move it to the re-
            transmit buffer */
         if (AMDL.txBuf[nackSn] != NULLP)
         {
            /* This is the case of a PDU to be added to retx list       */
            RLOG_ARG2(L_DEBUG,DBG_RBID, rbCb->rlcId.rbId,
                     "kwHndlStaRsp: Move PDU from txBuf to retxLst" 
                     "UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
            
            kwAmmDlMoveFrmTxtoRetxBuffer(gCb, 
                                         &rbCb->m.amDl, 
                                         &retx, 
                                         sn);
#if (ERRCLASS & ERRCLS_ADD_RES)
            if(retx)
#endif
               retxNode = retx->lstEnt.next;

            idx++;
            continue;
         }
         
         /* process the pdus/segments in the re-transmit buffer with 
            this NACK_SN */
         while (retxNode)
         {
            retx = (KwRetx *)(retxNode->node);
            if (retx->amHdr.sn != nackSn)
            {
               break;
            }
          /* if((ourSoEnd < receivedSoStart) ||
          (ourSoStart >receivedSoEnd)) then ACKED. If soEnd is zero then definitely
          it is nacked*/
            if ((soEnd != 0) &&
                ((retx->soEnd < soStart) /*|| (retx->amHdr.so > soEnd)*/))
            {
               RLOG_ARG3(L_DEBUG, DBG_RBID, rbCb->rlcId.rbId,
                        "kwHndlStaRsp: Handle ACK for byte segment, Its "
                        "sn = %d UEID:%d CELLID:%d",
                        sn, 
                        rbCb->rlcId.ueId,
                        rbCb->rlcId.cellId);
               RLOG_ARG4(L_DEBUG, DBG_RBID, rbCb->rlcId.rbId,
                        "soStart and soEnd = %d, %d, UEID:%d CELLID:%d",
                        retx->amHdr.so, retx->soEnd, 
                        rbCb->rlcId.ueId,
                        rbCb->rlcId.cellId);

               retxNode = retxNode->next;
               kwAmmDlProcessSuccessfulReTx(gCb,rbCb, retx, &datCfm);
            }
            else /*if (((e2) && (retx->soEnd <= soEnd)) || (!e2))*/
            {
               /* This case covers the NACKED segments and also the case */
               /* when there are segments and the entire SN is nacked.   */
               /* This case also covers the case of nonsegmented retx PDU*/

               /* Mark the retx PDU we found for further retransmission  */
               retxNode = retxNode->next;
               kwAmmDlMarkPduForReTx(gCb, rbCb, retx);
            }
         }

         idx++;
      }

      /* Remove the PDUs with are further acked by the ACK_SN after taking
         care of all the NACK_SN related acknowledgments*/
      kwAmmDlUpdateTxAndReTxBufForAckSn(gCb,rbCb, mAckSn, retxNode, &datCfm);

      /* Update vtA */
      kwAmmDlSetVtAAndVtMS(&AMDL,vtA);
   }
   else
   {
     kwStatusAckCnt++;
      /* For All ACKs */
      RLOG_ARG2(L_UNUSED,DBG_RBID, rbCb->rlcId.rbId,
               "kwHndlStaRsp: Received All ACKS UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);

      /* For the remaining ACKs  after last nackSn */
      kwAmmDlUpdateTxAndReTxBufForAckSn(gCb,rbCb, mAckSn, retxNode, &datCfm);
      
      /* update vtA */
      kwAmmDlSetVtAAndVtMS(&AMDL, pStaPdu->ackSn); 
   }

   if(datCfm->numSduIds != 0)
   {
      if(datCfm->numSduIds > 1024)
      {
         RLOG_ARG4(L_DEBUG,DBG_RBID,datCfm->rlcId.rbId, 
                  "Sending [%lu] SDU Cfms to PDCP & [%lu] lost for"
                  "UEID:%d CELLID:%d",
                  datCfm->numSduIds, 
                  datCfm->numSduIds-1024,
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
         datCfm->numSduIds = 1024;
      }
      kwSduSndCnt += datCfm->numSduIds;
      /* Sap control block */
      KwUiKwuDatCfm(&kwuSap->pst, kwuSap->suId, datCfm);
   }
   else
   {
      KW_SHRABL_STATIC_BUF_FREE(kwuSap->pst.region, kwuSap->pst.pool, datCfm, sizeof(KwuDatCfmInfo));
   }

   /* Fix for memory corruption */
   KW_LLIST_FIRST_RETX(AMDL.retxLst, AMDL.nxtRetx);
   /* BO update, if retransmission BO has changed. AMDL.retxBo would have
      canged inside the above called functions */
   if (oldRetxBo != AMDL.retxBo)
   {
      kwAmmSendDStaRsp(gCb, rbCb, &AMDL);
   }

   RETVOID;
}

/**
 * @brief Function to calculate the current buffer occupancy
 *
 * @details 
 *    Function to calculate the current bo depending on the control, 
 *          re-transmit, transmit bo's and the state of the transmit window. 
 *          If the transmit window is stalled, then the transmit bo is not 
 *          taken into account
 *
 * @param[in] amDl AM mode donwlink control block 
 *
 *  @return S16
 *      Calculated bo
*/
#ifdef ANSI
PUBLIC S32 kwAmmCalculateBo
(
KwAmDl *amDl
)
#else
PUBLIC S32 kwAmmCalculateBo(amDl)
KwAmDl *amDl;
#endif
{
   S32 bo;

   /* Make sure non of the bo's are negative */      
   if (amDl->bo < 0) 
   {   
      amDl->bo = 0;                         
   }
   
   if (amDl->cntrlBo < 0) 
   {
      amDl->cntrlBo = 0;               
   }
   
   if (amDl->retxBo < 0) 
   {
      amDl->retxBo = 0;                 
   }

   bo = amDl->cntrlBo + amDl->retxBo;

   /* if window is not stalled then add the transmit bo also */
   if (! KW_AM_IS_TRANS_WIN_STALLED(amDl)) 
   {
      bo += amDl->bo;
   }
   
   return bo;
}
U32 kwRxSdu;

/**
 * @brief Handler to queue the SDUs received from PDCP
 *       
 * @details
 *    This function is invoked by UIM to queue the SDU received from PDCP in the
 *    SDU queue of the corresponding RbCb. It also updates the BO and report the
 *    same to MAC.
 *    -  Allocate memory for and assign received buffer to the SDU
 *    -  Add SDU in the sduQ of KwAmDl 
 *    -  Calculate bo with the buffer received
 *    -  Accumulate bo with retransmission bo and control pdu's bo if available
 *    -  Estimate the header size for the bo; Fill in StaRspInfo and send it 
 *       to MAC
 *
 * @param[in] gCb     RLC Instance control block
 * @param[in] rbCb    RB control block 
 * @param[in] mBuf    Sdu to be queued
 * @param[in] datReq  Ptr to the datReq sent from PDCP
 *
 * @return Void
 *      -# RETVOID
*/
#ifdef ANSI
PUBLIC Void kwAmmQSdu
(
KwCb            *gCb,
KwDlRbCb        *rbCb,
Buffer          *mBuf,
KwuDatReqInfo   *datReq
)
#else
PUBLIC Void kwAmmQSdu(gCb, rbCb, mBuf, datReq)
KwCb            *gCb;
KwDlRbCb        *rbCb;
Buffer          *mBuf;
KwuDatReqInfo   *datReq;
#endif
{
   KwSdu       *sdu;
#ifdef TENB_STATS
   U32         kwWinSz; 
#endif
   TRC2(kwAmmQSdu)

   /* Allocate sdu */
   KW_ALLOC_WC(gCb,sdu, sizeof(KwSdu)); 

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (sdu == NULLP)
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
               "Memory allocation failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_RES */

   KW_UPD_L2_DL_TOT_SDU_STS(gCb,rbCb);
   /* Discard new changes starts */
   kwUtlGetCurrTime(&sdu->arrTime);
   /* Discard new changes ends */
   /* Assign values to sdu */
   SFndLenMsg(mBuf, &sdu->sduSz);

   sdu->mBuf = mBuf;
   sdu->actSz = sdu->sduSz;
   sdu->mode.am.sduId = datReq->sduId;
   /* initialize values for AM mode to 0 */
   sdu->mode.am.rcvdSz = 0;
   sdu->mode.am.isSegmented = 0;

#ifndef QCOM_STATS
#ifdef MSPD
{
extern U32 dlrate_kwu;
dlrate_kwu += sdu->sduSz;
}
#endif
#else
   kwDlrate_kwu += sdu->sduSz;
#endif
   
   /* Update nxtTx to point to the added sdu if this is the first SDU in the
    * queue */
   if (AMDL.nxtTx == NULLP)
   {
      RLOG_ARG2(L_UNUSED,DBG_RBID, rbCb->rlcId.rbId,
               "kwAmmQSdu: Received SDU will be transmitted next"
               "UEID:%d CELLID:%d",
                rbCb->rlcId.ueId,
                rbCb->rlcId.cellId);
      AMDL.nxtTx = sdu;
   }

   /* Add sdu to the sdu list */
   cmLListAdd2Tail(&AMDL.sduQ, &sdu->lstEnt);
   sdu->lstEnt.node = (PTR)sdu;
   AMDL.numSduPdu = 1;

#ifdef TENB_STATS
   if (rbCb->ueCb->tenbStats)
   {
      if (AMDL.sduQ.count > rbCb->ueCb->tenbStats->stats.rlc.dlMaxPktsInSduQ)
      {
         rbCb->ueCb->tenbStats->stats.rlc.dlMaxPktsInSduQ = AMDL.sduQ.count;
      }
      kwWinSz = KW_AM_TRANS_WIN_SIZE(&AMDL);
      if (kwWinSz > rbCb->ueCb->tenbStats->stats.rlc.dlMaxWindowSz)
      {
         rbCb->ueCb->tenbStats->stats.rlc.dlMaxWindowSz = kwWinSz;
      }
   }
#endif
   /* Update BO and estimate header size for the current BO */ 
   AMDL.bo = AMDL.bo + sdu->sduSz;
   AMDL.estHdrSz += 2;
#ifdef LTE_L2_MEAS_RLC
   /* Update numActUe if it is not active */
   if((rbCb->rbL2Cb.measOn & LKW_L2MEAS_ACT_UE) &&
      (rbCb->ueCb->numActRb[rbCb->qci] == 0))
   {
      rbCb->ueCb->numActRb[rbCb->qci]++;
      gCb.kwL2Cb.numActUe[rbCb->qci]++;
   }
#endif

   if(!kwDlUtlIsReestInProgress(rbCb))
   {
      kwAmmSendDStaRsp(gCb, rbCb, &AMDL);
   }

   RETVOID;
} 

/**
 *
 * @brief Private handler to construct control PDU
 *
 * @details
 *    This function sets the pduSz correctly after eliminating the fixed
 *    header sizes and the MAC header size. It copies the already prepared
 *    STATUS PDU to the data to be sent to MAC.
 *
 * @param[in]  gCb        RLC instance control block
 * @param[in]  rbCb       Downlink RB control block 
 * @param[in]  kwdatReq   DatReq to be sent to MAC 
 *
 *  @return Void 
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmDlAssembleCntrlInfo 
(
KwCb       *gCb,
KwDlRbCb   *rbCb,
KwDatReq   *kwDatReq
)
#else
PRIVATE Void kwAmmDlAssembleCntrlInfo(gCb, rbCb, kwDatReq)
KwCb       *gCb;
KwDlRbCb   *rbCb;
KwDatReq   *kwDatReq;
#endif
{
   KwUdxDlSapCb   *sapCb;

   TRC2(kwAmmDlAssembleCntrlInfo)


   /* Eliminate fixed hdr size (14bits including ACK_SN) */
   if (kwDatReq->pduSz >= (KW_CNTRL_PDU_FIXED_HDRSZ + KW_MAC_HDR_SZ3))
   {
      /* Check the TB size whether it is sufficcient enough to fit the 
         status Pdu into it otherwise make arrangement such that it can fit 
         into in a way of possible NACks*/
      /* ccpu00135743 : fix for MAC Hdr size calc */ 
      kwDatReq->pduSz -= (rbCb->m.amDl.cntrlBo < 128) ?  
                         KW_MAC_HDR_SZ2 : KW_MAC_HDR_SZ3;
      
      /* Create the status Pdu with the required NACKs */
      kwAmmCreateStatusPdu(gCb,rbCb,kwDatReq);

      sapCb = KW_GET_DL_SAPCB(gCb, rbCb);
	   KwDlUdxStaProhTmrStart(&(gCb->u.dlCb->udxDlSap->pst), 
                             sapCb->suId, &(rbCb->rlcId));
	  
      /* Update number of pdus in pduInfo */
      kwDatReq->pduInfo.mBuf[kwDatReq->pduInfo.numPdu] = AMDL.mBuf; 
      kwDatReq->pduInfo.numPdu++;

      KW_FREE_SHRABL_BUF(gCb->u.dlCb->udxDlSap->pst.region, 
                         gCb->u.dlCb->udxDlSap->pst.pool,
                         AMDL.pStaPdu,
                         sizeof(KwUdxDlStaPdu));

      AMDL.pStaPdu = NULLP;
      AMDL.mBuf = NULLP;
      gRlcStats.amRlcStats.numDLStaPduSent++;
   }

   RETVOID;
}

/**
 * @brief Handler to form the PDUs with the size indicated by MAC
 *
 * @details
 *    This function is invoked by UTL with the PDU size indicated by 
 *    MAC (after eliminating MAC header size). It assembles control 
 *    Info / data (New SDUs / Retx PDUs), check if polling needs to be 
 *    set for the data PDU and returns PDU(s) and updated BO with 
 *    estimated header size to be sent to MAC.
 *
 *    - Check if the control BO is available and call kwAssembleCntrlInfo 
 *      to assemble control Information
 *    - Check if the pdu size is available to form PDUs from retransmission 
 *      buffer and call kwResegRetxPdus
 *    - Check if the pdu size is available and assemble SDUs from sduQ 
 *      if exist, using kwAssembleSdus
 *    - PDU Info and bo are filled in and then sent to MAC from the 
 *      utility function
 *
 * @param[in]  gCb        RLC instance control block
 * @param[in]  rbCb       RB control block 
 * @param[in]  kwdatReq   DatReq to be sent to MAC 
 *
 * @return Void
 *
 */
#ifdef ANSI
PUBLIC Void kwAmmProcessSdus
(
KwCb       *gCb,
KwDlRbCb   *rbCb,
KwDatReq   *kwDatReq
)
#else
PUBLIC Void kwAmmProcessSdus(gCb, rbCb, kwDatReq)
KwCb       *gCb;
KwDlRbCb   *rbCb;
KwDatReq   *kwDatReq;
#endif    
{
   TRC2(kwAmmProcessSdus)


   /* Assemble control information */ 
   if (AMDL.cntrlBo != 0)
   {
      kwDatReq->boRep.staPduPrsnt = TRUE;
      if (AMDL.pStaPdu != NULLP)
      {
         kwAmmDlAssembleCntrlInfo (gCb, rbCb, kwDatReq);
      }
      else
      {
         RLOG_ARG2(L_ERROR, DBG_RBID, rbCb->rlcId.rbId,
                  "Miscomputation of control Bo. UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
      }
      AMDL.cntrlBo = 0;
   }   

   /* For estHdrSz */
   AMDL.numSduPdu = 0;

   /* Retransmit PDUs /portions of PDUs available in retxLst */
   if ((kwDatReq->pduSz > 0) && (AMDL.nxtRetx != NULLP))
   {
      kwResegRetxPdus (gCb,rbCb, kwDatReq);
   }

   /* Assemble SDUs to form new PDUs */ 
   if ((kwDatReq->pduSz > 0) && (AMDL.nxtTx != 0))
   {
      kwAssembleSdus(gCb,rbCb, kwDatReq); 
   }
  
   if (AMDL.nxtRetx != NULLP)
   {
      kwDatReq->boRep.oldestSduArrTime = AMDL.nxtRetx->sduMap[0].sdu->arrTime;
   }
   else if (AMDL.nxtTx != NULLP)
   {
      kwDatReq->boRep.oldestSduArrTime = AMDL.nxtTx->arrTime;
   }
   /* Accumulate bo */
   kwDatReq->boRep.bo = kwAmmCalculateBo(&AMDL);

   /* Estimate hdr size for boReport */
   kwAmmEstHdrSz (gCb,&AMDL, KW_DEL_SDU);
   kwDatReq->boRep.estHdrSz = AMDL.estHdrSz;

   if(kwDatReq->pduSz > 0)
   {
      gRlcStats.amRlcStats.numDLBytesUnused += kwDatReq->pduSz;
   }
   RETVOID;
} 

/**
 * @brief Private handler split a PDU/segment into two
 *
 * @details
 *    Its a private function called by kwResegRetxPdu to split a segment
 *    or a retransmit PDU into two segments splitting at the passed size.
 *    This function is called only for those PDUs that dont have any LIs.
 *
 * @param[in]     gCb    RLC instance control block
 * @param[in]     rbCb   RB control block 
 * @param[in,out] crnt   The PDU to be split, first part of split pdu remians
 *                       in this
 * @param[out]    next   The second part of the split pdu
 * @param[in]     size   The size witin crnt, at which to split   
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwSplitPdu
(
KwCb       *gCb,
KwDlRbCb   *rbCb,
KwRetx     *crnt,
KwRetx     *next,
U16        size
)
#else
PRIVATE Void kwSplitPdu(gCb, rbCb, crnt, next, size)
KwCb       *gCb;
KwDlRbCb   *rbCb;
KwRetx     *crnt;
KwRetx     *next;
U16        size;
#endif
{
   U8            fi;

   TRC2(kwSplitPdu)
   /* Set the SN for the new segment */
   next->amHdr.sn = crnt->amHdr.sn;

   /* Set the protocol specific fields appropriately */
   fi = crnt->amHdr.fi;
   crnt->amHdr.fi  = crnt->amHdr.fi | KW_FI_LAST_SEG;
   next->amHdr.fi  = fi | KW_FI_FIRST_SEG;
   /* both current and next are PDU segments , set rf = 1*/
   crnt->amHdr.rf  = next->amHdr.rf = 1;
   next->amHdr.lsf = crnt->amHdr.lsf;
   /* set lsf = 0 for crnt, as the last byte of the segment will not be 
   the last byte of the PDU */
   crnt->amHdr.lsf = 0;
   crnt->amHdr.p   = 0;

   /* Update seg size */
   next->segSz = crnt->segSz - size;
   crnt->segSz = size;
   crnt->hdrSz = KW_AM_SEG_FIXED_HDRSZ;

   /* Set the SO fields appropriately */
   /* MS_FIX for DL stall */
   next->soEnd = crnt->soEnd;

   /* Set the SO fields appropriately */
   /* SO of next will be after the end of current */
   next->amHdr.so = crnt->amHdr.so + crnt->segSz;
   /* SO End of current will be one less than the start of next */
   crnt->soEnd = next->amHdr.so - 1;
   
   /* This is a special case of single LI and hence set that to 1 */
   crnt->amHdr.numLi = next->amHdr.numLi = 1;

   /* intialize the other fields in the amHdr of next to 0 */
   next->amHdr.p = 0;
   next->amHdr.dc = 0;
   next->amHdr.e = 0;
   
   /* This macro is called for No LI case - one SDU */
   /* Update the size of SDU in each node's sduMap  */
   next->sduMap[0].sdu = crnt->sduMap[0].sdu;
   crnt->sduMap[0].sduSz = crnt->segSz;
   next->sduMap[0].sduSz = next->segSz;
   next->numSdu = crnt->numSdu; /* should should be 1 in this case */

   /* Segment the payload into two parts based on the size passed */
   SSegMsg(crnt->seg, size, &next->seg);
   next->retxCnt     = crnt->retxCnt;
   next->yetToConst  = TRUE;
   next->pendingReTrans    = crnt->pendingReTrans;

   /* Compute the header size and update the BO appropriately */
   KW_AM_EXTN_HDRSZ(next->amHdr.numLi, next->hdrSz);
   next->hdrSz += KW_AM_SEG_FIXED_HDRSZ;
   AMDL.estHdrSz += next->hdrSz;

   /* Add the next to the retx list */
   AMDL.retxLst.crnt = &crnt->lstEnt;
   CM_LLIST_INS_AFT_CRNT(AMDL.retxLst, next); 
   AMDL.nxtRetx = next;
   
   RETVOID;
}

/**
 * @brief Private handler split a PDU/segment into two
 *
 * @details
 *    Its a private function called by kwResegRetxPdu to split a segment
 *    or a retransmit PDU into two segments splitting at the passed size.
 *    This function is called only for those PDUs that have valid LIs.
 *
 * @param[in]     gCb    RLC instance control block
 * @param[in]     rbCb   RB control block 
 * @param[in,out] crnt   The PDU to be split, first part of split pdu remians
 *                       in this
 * @param[out]    next   The second part of the split pdu
 * @param[in]     size   The size witin crnt, at which to split
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwSplitPduWithLi
(
KwCb       *gCb,
KwDlRbCb   *rbCb,
KwRetx     *crnt,
KwRetx     *next,
U16        size
)
#else
PRIVATE Void kwSplitPduWithLi(gCb, rbCb, crnt, next, size)
KwCb       *gCb;
KwDlRbCb   *rbCb;
KwRetx     *crnt;
KwRetx     *next;
U16        size;
#endif
{
   U16       segSz = 0;
   U16       tSz   = size;
   U32       liCnt = 0;
   U32       liIdx; 
   KwAmHdr   *oldHdr;
   KwAmHdr   *newHdr;

   TRC2(kwSplitPduWithLi)
   /* Set the SN for the new segment */
   next->amHdr.sn = crnt->amHdr.sn;

   /* Figure out how many SDUs from the original may be included in */
   /* the current one based on the size passed                      */
   oldHdr = &crnt->amHdr;
   newHdr = &next->amHdr;

   next->numSdu = 0;
   newHdr->numLi = 0;
   
   /* kw006.201 ccpu00120058, Changed the order of these conditions */
   while ((liCnt < oldHdr->numLi) && (tSz >= oldHdr->li[liCnt]))
   {
       RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                "kwSplitPduWithLi: numLi of in PDU = %d UEID:%d CELLID:%d", 
                crnt->amHdr.numLi,
                rbCb->rlcId.ueId,
                rbCb->rlcId.cellId);

       segSz      += oldHdr->li[liCnt];
       tSz        -= oldHdr->li[liCnt];

       /* Eliminate extension header size */
       if (tSz > 2)
       {
          /* kw003.201 - Do this only if we have enough space */
          tSz -= ((liCnt + 1) % 2) ? 2 : 1; 
       }
       else
       {
          /* kw003.201 - Even though a few bits (1 or 2) may be left, they */
          /*             are useless as we cant fit any into it            */
          tSz = 0;
       }
       liCnt++;
   }
   next->segSz = crnt->segSz - (segSz + tSz);
   crnt->segSz = (segSz + tSz);

   /* Transfer the remaining LIs to the subsequent segment */
   /* kw006.201 ccpu00120058, added boundary condition check for li, sduMap */
   if (tSz && (liCnt < oldHdr->numLi))
   {
      newHdr->li[0] = oldHdr->li[liCnt] - tSz;
      oldHdr->li[liCnt] = tSz;
      next->sduMap[0] = crnt->sduMap[liCnt];
      next->sduMap[0].sduSz -= tSz;
      crnt->sduMap[liCnt].sduSz = tSz;
      newHdr->numLi++;
      next->numSdu++;
      crnt->numSdu = liCnt + 1;
      liCnt++;
   }

   liIdx = 0;
   while ((liCnt + liIdx) < oldHdr->numLi)
   {
      newHdr->li[newHdr->numLi] = oldHdr->li[liCnt + liIdx];
      next->sduMap[next->numSdu++] = crnt->sduMap[liCnt + liIdx];
      liIdx++; 
      newHdr->numLi++;
   }
   oldHdr->numLi  = liCnt;
   crnt->numSdu = liCnt;

   /* Manage header sizes and also updating the BO on RB CB */
   KW_AM_EXTN_HDRSZ(next->amHdr.numLi, next->hdrSz);
   next->hdrSz += KW_AM_SEG_FIXED_HDRSZ;
   AMDL.estHdrSz += next->hdrSz;

   /* Set the frame indicator field for both the segments */
   next->amHdr.fi  = crnt->amHdr.fi;
   if (tSz == 0)
   {
      /* We exactly finished an SDU with this segment. We want to set FI */
      /* appropriately. The current one should have last (un)set and the */
      /* the next one should have first (un)set                          */
      crnt->amHdr.fi &= 0x02;
      next->amHdr.fi &= 0x01;
   }
   else
   {
      /* We exactly finished an SDU with this segment */
      crnt->amHdr.fi |= 0x01;
      next->amHdr.fi |= 0x02;
      
   }
   crnt->amHdr.p   = 0;
   crnt->amHdr.rf  = next->amHdr.rf = 1;
   next->amHdr.lsf = crnt->amHdr.lsf;
   crnt->amHdr.lsf = 0;

   /* MS_FIX for DL stall */
   /* Set the SO fields appropriately */
   next->soEnd = crnt->soEnd;

   /* Set the SO fields appropriately */
   crnt->soEnd = crnt->amHdr.so + crnt->segSz - 1;
   next->amHdr.so = crnt->amHdr.so + crnt->segSz;

   /* intialize the other fields in the amHdr of next to 0 */
   next->amHdr.p = 0;
   next->amHdr.dc = 0;
   next->amHdr.e = 0;
   
   /* Segment the payload into two parts based on the size passed */
   SSegMsg(crnt->seg, crnt->segSz, &next->seg);
   next->retxCnt     = crnt->retxCnt;
   next->yetToConst  = TRUE;
   next->pendingReTrans    = crnt->pendingReTrans;

   /* Add the next to the retx list */
   AMDL.retxLst.crnt = &crnt->lstEnt;
   CM_LLIST_INS_AFT_CRNT(AMDL.retxLst, next); 
   AMDL.nxtRetx = next;
   
   RETVOID;
}

/**
 * @brief Private handler to retransmit PDUs or PDU segments
 *
 * @details
 *    Its a private function called by kwProcessSdus, to create the 
 *    PDUs / its segments from the retransmission buffer available in RbCb.
 *       
 *    - Eliminate the fixed header size and MAC header size while 
 *      forming PDUs/segments
 *    - While pdusize is available and retxBuf has data (pdu or portion 
 *      of pdu) to be sent, form the pdu as it is if it matches with the 
 *      pdusize else segment the PDUs/portion of PDUs
 *    - Call kwAmmDlCheckAndSetPoll function to check and set the poll bit as 
 *      required 
 *    - Concatenate data and header info and fill pduInfo 
 *    - Update retxCnt and send indication to PDCP if it reaches maxRetx 
 *      threshold
 *
 * @param[in]  gCb       RLC instance control block
 * @param[in]  rbCb      RB control block
 * @param[in]  kwdatReq  DatReq to be sent to MAC 
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwResegRetxPdus 
(
KwCb       *gCb,
KwDlRbCb   *rbCb,
KwDatReq   *kwDatReq
)
#else
PRIVATE Void kwResegRetxPdus(gCb, rbCb, kwDatReq)
KwCb       *gCb;
KwDlRbCb   *rbCb;
KwDatReq   *kwDatReq;
#endif
{
   KwAmDl   *amDl;
   KwRetx   *retx;
   U8       hdr[KW_MAX_HDRSZ];
   U16      idx; 
   Bool     segFlg  = TRUE;
   Buffer   *pdu;
#ifdef LTE_L2_MEAS
   U16        sduIdx;
   KwL2MeasTb *l2MeasTb;
   KwlchInfo  *lchInfo;
   U8         numSdus;
#endif

   TRC2(kwResegRetxPdus)


   amDl  = &AMDL;
#ifdef LTE_L2_MEAS
   /* TODO : This shoould be taken care in new Trasmissions */
   /* This lchInfo should be retrieved there */
   l2MeasTb = kwUtlGetCurMeasTb(gCb, rbCb);
   if (l2MeasTb == NULLP)
   {
      RETVOID;
   }
   /* TODO : This lcid needs to be searched in case of normal Tx */
   /* In retx here, its fine as this will be higher priority */
   lchInfo = &l2MeasTb->lchInfo[l2MeasTb->numLchInfo];
   if (l2MeasTb->numLchInfo >= KW_MAX_ACTV_DRB)
   {
      RETVOID;
   }
   l2MeasTb->numLchInfo++;
   lchInfo->lcId = rbCb->lch.lChId;
   lchInfo->numSdus = 0;
#endif

   while ((kwDatReq->pduSz > 0) && (amDl->nxtRetx != NULLP)&&
          (kwDatReq->pduInfo.numPdu < KW_MAX_PDU))
   {
      U16 tmpSz;
      
      retx = amDl->nxtRetx;
      /* kw003.201 : Add header size to seg size to determine if the      */
      /*             the segment can be completed within the allocation   */
      /* kw003.201 - Eliminate MAC Header Size based on bites needed      */
      tmpSz = KW_MIN((retx->segSz + retx->hdrSz), kwDatReq->pduSz);
      /* ccpu00135743 : fix for MAC Hdr size calculation */
      kwDatReq->pduSz -= (tmpSz < 128) ?  KW_MAC_HDR_SZ2 : KW_MAC_HDR_SZ3;

      /* kw003.201 - We should have at least one more than basic header */
      if (kwDatReq->pduSz <= retx->hdrSz)
      {
         RETVOID;
      }
      kwGetNxtRetx(gCb, &(amDl->nxtRetx));

      /* Send retx buf without segmentation */
      if (kwDatReq->pduSz >= (retx->segSz + retx->hdrSz))
      {
         U8 pollBit;
         
         RLOG_ARG2(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                  "kwResegRetxPdus: Send retx buf without segmentation "
                  "UEID:%d CELLID:%d", 
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);

         if (retx->yetToConst)
         {
            /* Construct hdr with the available hdr values */ 
            kwConstructAmHdr(gCb,&retx->amHdr, segFlg, hdr, &idx);
            /* Add header to the pdu/segment */
            SAddPreMsgMultInOrder(hdr, idx + 1, retx->seg);
            retx->yetToConst = FALSE;
         } 

         /* kw003.201 - Check if poll bit needs to be set. Retx size does */
         /* not affect the poll bit so it is being passed as zero         */ 
         pollBit = kwAmmDlCheckAndSetPoll(gCb,rbCb, FALSE, 0);
         KW_UPD_POLL_BIT(gCb, retx, pollBit);

         kwDatReq->pduSz  -= retx->hdrSz;
         kwDatReq->pduSz  -= retx->segSz;
         AMDL.estHdrSz    -= retx->hdrSz;
#ifdef LTE_L2_MEAS   
         if (rbCb->rlcId.rbType == CM_LTE_DRB)
         {
            numSdus = 0;
            for (sduIdx = lchInfo->numSdus ; 
                  ((numSdus < retx->numSdu) && (sduIdx < KW_L2MEAS_SDUIDX)) ; 
                  sduIdx++, numSdus++)
            {
               lchInfo->sduInfo[sduIdx].arvlTime = retx->sduMap[numSdus].sdu->arrTime;
               lchInfo->sduInfo[sduIdx].isRetxPdu = TRUE; /* TODO : for later use */
            }
            lchInfo->numSdus += numSdus;
         }
#endif
      }
      else
      {
         KwRetx *tNode;
         
         /* Segment this pdu / portion of pdu. Insert this segment into */
         /* retxLst and update offset                                   */
         RLOG_ARG2(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                  "kwResegRetxPdus: Segment retx buf UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);

         /* Eliminate fixed header size if the pdu is segmented for the */
         /* first time                                                  */
         kwDatReq->pduSz -= KW_AM_SEG_FIXED_HDRSZ;
         if (kwDatReq->pduSz <= 0)
         {
            RETVOID;
         }

         /* Allocate memory for tracking a new segment */
         KW_ALLOC_WC(gCb,tNode, sizeof(KwRetx)); 
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (tNode == NULLP)
         {
            RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId,
                     "Memory allocation failed UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
            RETVOID;
         }
#endif /* ERRCLASS & ERRCLS_RES */

         /* initialize the list pointer to 0 instead of memset */
         tNode->lstEnt.next = 0;
         tNode->lstEnt.prev = 0;
         
         /* Segment header and data */
         KW_AM_RMV_HDR(gCb, rbCb, retx);

         /* No LI, segment the data filed, update hdr Info */ 
         if ((retx->amHdr.numLi - 1) == 0)
         {
            RLOG_ARG2(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                     "kwResegRetxPdus: No LI for segmentation UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
            /* kw003.201 - Split the payload and update other fields */
            kwSplitPdu(gCb,rbCb, retx, tNode, kwDatReq->pduSz); 

         }
         else
         {
            RLOG_ARG2(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                     "kwResegRetxPdus: LIs for segmentation UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
            /* With LI, segment the data field, update hdr Info */    
            /* Remove the header if it exists as it would be reconstructed */
            kwSplitPduWithLi(gCb,rbCb, retx, tNode, kwDatReq->pduSz);
         }
#ifdef LTE_L2_MEAS
         numSdus = 0;
         /* ccpu00143043 */
         sduIdx = lchInfo->numSdus;
         for (numSdus = 0, sduIdx = lchInfo->numSdus; 
              ((numSdus < retx->numSdu) && (sduIdx < KW_L2MEAS_SDUIDX));
              numSdus++, sduIdx++)
         {
            lchInfo->sduInfo[sduIdx].arvlTime = 
                  retx->sduMap[numSdus].sdu->arrTime;
            lchInfo->sduInfo[sduIdx].isRetxPdu = TRUE;
         }
         lchInfo->numSdus = sduIdx;
         if ((retx->amHdr.lsf == 0) && (lchInfo->numSdus > 0))
         {
            lchInfo->numSdus--;
         }
#endif
         /* Construct hdr with the available hdr values */
         kwConstructAmHdr(gCb,&retx->amHdr, segFlg, hdr, &idx);
         SAddPreMsgMultInOrder(hdr, idx + 1, retx->seg);

         retx->hdrSz = idx + 1;

         /* Poll bit need not be set for this seg, since its second  */
         /* half remains in retxLst                                  */
         KW_UPD_POLL_BIT(gCb, retx, FALSE);
         retx->yetToConst = FALSE;
         kwDatReq->pduSz = 0; 
      }

      kwCpyMsg(gCb,retx->seg, &pdu);

      /* Update pduInfo */
      kwDatReq->pduInfo.mBuf[kwDatReq->pduInfo.numPdu] = pdu;
      kwDatReq->pduInfo.numPdu++;
      /* kw005.201 ccpu00117318, updating the statistics */
      gCb->genSts.pdusRetx += 1;
      retx->soEnd = retx->amHdr.so + retx->segSz - 1;
      retx->pendingReTrans    = FALSE;
      amDl->retxBo -= retx->segSz;
   }
#ifndef ALIGN_64BIT
   RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                 "kwResegRetxPdus: retxBo after resegmentation = %ld"
                 "UEID:%d CELLID:%d", 
                 amDl->retxBo,
                 rbCb->rlcId.ueId,
                 rbCb->rlcId.cellId);
#else
   RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                 "kwResegRetxPdus: retxBo after resegmentation = %d ",
                 "UEID:%d CELLID:%d", 
                 amDl->retxBo,
                 rbCb->rlcId.ueId,
                 rbCb->rlcId.cellId);
#endif
   amDl->numSduPdu = 0;

   RETVOID;
}


/**
 * @brief Private handler to assemble SDUs to form new data PDU(s)
 *
 * @details
 *    Its a private function called by kwProcessSdus, to create the new data 
 *    PDUs from the SDU queue of RbCb.
 *
 *    - While pdusize is available, segment/concatenate SDUs or else if it
 *      matches the pdu size form PDUs accordingly.
 *    - RLC header and MAC header size are eliminated while forming the PDUs
 *    - Call kwAmmDlCheckAndSetPoll function to check and set the poll bit 
 *      as required 
 *    - Concatenate data and header info and fill pduInfo  
 *
 * @param[in]  rbCb     RB control block
 * @param[in]  kwdatReq DatReq to be sent to MAC 
 *
 * @return  Void 
 *
 */
#ifdef ANSI
PRIVATE Void kwAssembleSdus 
(
KwCb       *gCb,
KwDlRbCb   *rbCb,
KwDatReq   *kwDatReq
)
#else
PRIVATE Void kwAssembleSdus (gCb, rbCb, kwDatReq)
KwCb       *gCb;
KwDlRbCb   *rbCb;
KwDatReq   *kwDatReq;
#endif
{
   Buffer          *pdu         = NULLP;
   MsgLen          pduSz        = kwDatReq->pduSz;
   KwAmDl          *amDl        = &AMDL;
   KwSdu           *sdu         = amDl->nxtTx;
   Bool            pduCompleted = FALSE;
   U8              numSdu       = 0;
   KwSduMap        sduMap[KW_MAX_DL_LI + 1];
   Bool            nxtTxUpd     = FALSE;
   MsgLen          sduSz;
   KwuDiscSduInfo  *discSduInfo = NULLP;
   KwKwuSapCb* kwuSap = gCb->u.dlCb->kwuDlSap + KW_UI_PDCP;
#ifdef LTE_L2_MEAS
   KwContSduLst     contSduLst;  /*Contained sduLst */
   S32              dataVol    = amDl->bo;
   U32              *totMacGrant = &kwDatReq->totMacGrant;
   KwL2MeasDlIpTh   *dlIpThPut = &rbCb->l2MeasIpThruput.dlIpTh;
   U8               *sduIdx    = &dlIpThPut->lastSduIdx;
   Bool             newIdx;
   Bool             isSduSegmented;
   S32              oldBo;
   KwlchInfo        lchInfo;
   KwlchInfo        *dstLchInfo;
   U32              segSduCnt = 0;
   U32              lchIdx;
   U32              numSdus = 0;
   U32              currSduIdx = 0;
   KwL2MeasTb       *l2MeasTb;
#endif
   /* Discard new changes starts */
   Ticks                timeDiff = 0;
   Ticks                curTime  = 0;
   U8                   numNewPdu = 0;
   KwTx                 *txBuf = NULLP;
   /* Discard new changes ends */
   VOLATILE U32     startTime = 0;

   TRC2(kwAssembleSdus)


#ifdef LTE_L2_MEAS   
   contSduLst.numSdus = 0; 
   contSduLst.lcId = rbCb->lch.lChId;
   oldBo = amDl->bo;
   lchInfo.lcId = rbCb->lch.lChId;
   lchInfo.numSdus = 0;
#endif
   /* Discard new changes starts */
   /* Allocate memory for discSdu Info */
   KW_SHRABL_STATIC_BUF_ALLOC(kwuSap->pst.region,
                              kwuSap->pst.pool,
                              discSduInfo, 
                              sizeof(KwuDiscSduInfo));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (discSduInfo == NULLP)
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
               "Memory allocation failed UEID:%d CELLID:%d",
               rbCb->rlcId.ueId,
               rbCb->rlcId.cellId);
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_RES */

   discSduInfo->numSduIds = 0;
   discSduInfo->rlcId = rbCb->rlcId;

   kwUtlGetCurrTime(&curTime);
   amDl->sduQ.crnt =  &sdu->lstEnt;

   while ((pduSz > KW_EXTN_HDRSZ) && (sdu != NULLP) &&
          (kwDatReq->pduInfo.numPdu < KW_MAX_PDU) && 
          (numNewPdu < KW_MAX_NEW_DL_PDU))
   {
#ifdef LTE_L2_MEAS   
      isSduSegmented = sdu->mode.am.isSegmented;
#endif
      /* Discard new changes starts */
      if ((sdu->mode.am.isSegmented == FALSE) && (rbCb->discTmrInt > 0) && \
            (rbCb->rlcId.rbType == CM_LTE_DRB))
      {
         //leftAmSdus[rbCb->qci]--;
         timeDiff = KW_TIME_DIFF(curTime,sdu->arrTime); 
         if (timeDiff > rbCb->discTmrInt)
         {
            CmLList* nxtNode;
            /*starting Task*/
            SStartTask(&startTime, PID_RLC_AMM_DISC_SDUS);
#ifdef LTE_L2_MEAS 
            KW_UPD_L2_DL_DISC_SDU_STS(gCb,rbCb);
            /* TODO need to send disc cfm to pdcp */
#endif
            amDl->numSduPdu++; /* For decrementing the estimated header size */
            /* Update bo for boReport */
            amDl->bo -= sdu->sduSz;

            /* Get next sdu for assembly */
            nxtNode = sdu->lstEnt.next;

            /* store the info for sending it to PDCP */
            if(discSduInfo->numSduIds > 500)
            {
                RLOG_ARG2(L_ERROR,DBG_RBID, rbCb->rlcId.rbId, 
                         "This is a big error, we shouldn't be here"
                         "UEID:%d CELLID:%d",
                         rbCb->rlcId.ueId,
                         rbCb->rlcId.cellId);
                break;
            }

            discSduInfo->sduIds[discSduInfo->numSduIds] = sdu->mode.am.sduId;
            discSduInfo->numSduIds++;

            cmLListDelFrm(&amDl->sduQ, &sdu->lstEnt);

            kwUtlAddSduToBeFreedQueue(gCb, sdu);
            kwUtlRaiseDlCleanupEvent(gCb);
          
            /* We need to restore the crnt in the linked list which
             * would have become NULL in the DelFrm above */
            amDl->sduQ.crnt = nxtNode;
         
            if(nxtNode)
               sdu = (KwSdu*)nxtNode->node;
            else
               sdu = NULLP;
            
            /*stopping Task*/
            SStopTask(startTime, PID_RLC_AMM_DISC_SDUS);
            continue;
         }
      }
      nxtTxUpd = FALSE;
      sduSz    = sdu->sduSz;
      
#ifdef LTE_L2_MEAS
      newIdx = FALSE;
#endif
      /* kw005.201 fix for ccpu00118973 */
      if (pdu == NULLP)
      {
         /* kw003.201 - Check for window stall when you are */
         /*             creating a new PDU                  */
         if (KW_AM_IS_TRANS_WIN_STALLED(amDl))
         {
            break;
         }

         /* Eliminate MAC header */
         /* ccpu00135743 : Fix for MAC Hdr size calculation */
         pduSz -= (pduSz < 128) ? KW_MAC_HDR_SZ2 : KW_MAC_HDR_SZ3;

         /* Eliminate fixed header size */
         pduSz -= KW_AM_PDU_FIXED_HDRSZ;
        
         /* kw005.201 Check for PDU Size is large enough.
          * Fix for ccpu00118973 
          * */
         if( pduSz <= 0 )
         {
            break;
         }

         /* Update txBuf */
         KW_ALLOC_WC(gCb,txBuf, sizeof(KwTx));

#if (ERRCLASS & ERRCLS_ADD_RES)
         if (txBuf == NULLP)
         {
            RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
                  "Memory allocation failed UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
            RETVOID;
         }
#endif /* ERRCLASS & ERRCLS_RES */

         amDl->txBuf[amDl->vtS] = txBuf;
         /* Initialize the datReq structure */
         //cmMemset( (U8 *)&txBuf->amHdr, 0, sizeof(KwAmHdr) );
         txBuf->numSdu = 0;
         txBuf->pdu = NULLP;
         txBuf->amHdr.dc = 0;
         txBuf->amHdr.rf = 0;
         txBuf->amHdr.p = 0;
         txBuf->amHdr.fi = 0;
         txBuf->amHdr.e = 0;
         txBuf->amHdr.lsf = 0;
         txBuf->amHdr.so = 0;
         txBuf->amHdr.numLi = 0;

         txBuf->amHdr.sn = amDl->vtS;

         /* Update Framing Info */
         if (sdu->mode.am.isSegmented) 
         {
            txBuf->amHdr.fi = KW_FI_FIRST_SEG; /* binary 10 */
            sdu->mode.am.isSegmented = FALSE;
         }
         pduCompleted = FALSE;
      }

      /* Eliminate extension hdr size */
      KW_AM_ELIMINATE_EXTN_HDR(pduSz, sduSz, txBuf->amHdr.numLi);
      /*Rahul:Fix for pduSz becoming zero*/
      if( pduSz <= 0 )
      {
         /* Need to free the txBuf here */
         KW_FREE_WC(gCb, txBuf, sizeof(KwTx));
         amDl->txBuf[amDl->vtS]=NULLP;
         break;
      }

      txBuf->amHdr.li[txBuf->amHdr.numLi++] = sduSz;

      if (pduSz == sduSz)
      {
         /* Exact fit : Either one sdu equals pdu or the last sdu of a pdu */
#ifndef ALIGN_64BIT
         RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                       "kwAssembleSdus: PduSz matches SduSz for sduId = %ld"
                       "UEID:%d CELLID:%d",
                       sdu->mode.am.sduId,
                       rbCb->rlcId.ueId,
                       rbCb->rlcId.cellId);
#else
         RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                       "kwAssembleSdus: PduSz matches SduSz for sduId = %d "
                       "UEID:%d CELLID:%d",
                       sdu->mode.am.sduId,
                       rbCb->rlcId.ueId,
                       rbCb->rlcId.cellId);
#endif

         /* Concatenate sdu to pdu */
         if (pdu == NULLP)
         {
            pdu = sdu->mBuf;     
            sdu->mBuf = NULLP; 
         }
         else
         {
            SCatMsg(pdu, sdu->mBuf, M1M2);  
         }

#ifdef LTE_L2_MEAS_RLC
         kwUtlUpdSduSnMap(rbCb, sdu, kwDatReq, TRUE);
#endif /*  LTE_L2_MEAS */

         /* Create PDU with hdr and data */
         kwAmmCreatePdu(gCb,rbCb, &(txBuf->amHdr), pdu);
         pduCompleted = TRUE;

         /* For estHdrSz */
         amDl->numSduPdu++; 
         /* kw005.201 ccpu00117318, updating the statistics */
         kwUtlIncrementKwuStsSduTx(gCb->u.dlCb->kwuDlSap + rbCb->kwuSapId);
#ifdef LTE_L2_MEAS
         if(KW_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(gCb,rbCb))
         {
            if(isSduSegmented)
            {
               *sduIdx    = dlIpThPut->lastSduIdx;
            }
            else
            {
               KW_GETSDUIDX(*sduIdx);
               newIdx = TRUE;
            }
            kwUtlUpdateContainedSduLst(*sduIdx, &contSduLst);
            kwUtlUpdateOutStandingSduLst(dlIpThPut, *sduIdx, sdu->actSz, 
                  sdu->mode.am.sduId, newIdx);
            /* Update the arrival time for each SDU */
            /* ccpu00143043 */
            if ( lchInfo.numSdus < KW_L2MEAS_SDUIDX)
            {
               lchInfo.sduInfo[lchInfo.numSdus].arvlTime = sdu->arrTime; 
               lchInfo.numSdus++;
            }
         }
#endif
      }
      else if (pduSz > sduSz)
      {
         /* Concatenation: Join SDU to PDU */

         if (pdu == NULLP)
         {
            pdu = sdu->mBuf;      
            sdu->mBuf = NULLP; 
         }
         else
         {
            SCatMsg(pdu, sdu->mBuf, M1M2);
         }
#ifdef LTE_L2_MEAS_RLC
         kwUtlUpdSduSnMap(rbCb, sdu, kwDatReq, TRUE);
#endif /*  LTE_L2_MEAS */

         /* This sdu completes the PDU with the following condition */
         /* kw005.201: Fix for ccpu00117216 and ccpu00118284 */
         /* TODO : MAX_LI is hard coded to 5 */
         if ((sduSz > 2048) || (txBuf->amHdr.numLi >= KW_MAX_DL_LI))
         {
            /* Create PDU with hdr and data */
            kwAmmCreatePdu(gCb,rbCb, &(txBuf->amHdr), pdu);
            pduCompleted = TRUE;
         }
         
         /* For estHdrSz */
         amDl->numSduPdu++; 
         /* kw005.201 ccpu00117318, updating the statistics */
         kwUtlIncrementKwuStsSduTx(gCb->u.dlCb->kwuDlSap + rbCb->kwuSapId);
#ifdef LTE_L2_MEAS
         if(KW_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(gCb,rbCb)) 
         {
            if(isSduSegmented)
            {
               *sduIdx    = dlIpThPut->lastSduIdx;
            }
            else 
            {
               KW_GETSDUIDX(*sduIdx);
               newIdx = TRUE;
            }
            kwUtlUpdateContainedSduLst(*sduIdx, &contSduLst);
            kwUtlUpdateOutStandingSduLst(dlIpThPut, *sduIdx, sdu->actSz, 
                  sdu->mode.am.sduId, newIdx);
            /* Update the arrival time for each SDU */
            /* ccpu00143043 */
            if ( lchInfo.numSdus < KW_L2MEAS_SDUIDX)
            {
               lchInfo.sduInfo[lchInfo.numSdus].arvlTime = sdu->arrTime; 
               lchInfo.numSdus++;
            }
         }
#endif
      }
      else 
      {
         /* Segmentation
          * Allocate buffer for next PDU
          * Remove the segmented portion from SDUQ 
          * Calculate the hdr with LI for SDU */
         Buffer  *remSeg = NULLP;

         /* Segment the SDU to the size of the PDU and update header Info */
         SSegMsg(sdu->mBuf, pduSz, &remSeg);
         if (pdu == NULLP)
         {
            pdu = sdu->mBuf;      
         }
         else
         {
            SCatMsg(pdu, sdu->mBuf, M1M2);  
            KW_FREE_BUF_WC(sdu->mBuf);
         }
#ifdef LTE_L2_MEAS
         if(KW_MEAS_IS_DL_IP_MEAS_ON_FOR_RB(gCb,rbCb) ||
             KW_MEAS_IS_DL_DELAY_MEAS_ON_FOR_RB(gCb,rbCb) || 
             KW_MEAS_IS_DL_UU_LOSS_MEAS_ON_FOR_RB(gCb,rbCb) )
         {
            /* If actual size of the sdu is equal to msgLen
             * then it is first segment of the SDU */
            if(sdu->actSz == sdu->sduSz)
            {
               KW_GETSDUIDX(*sduIdx);
               newIdx = TRUE;
            }
            else
            {
               *sduIdx    = dlIpThPut->lastSduIdx;
            }
            kwUtlUpdateContainedSduLst(*sduIdx, &contSduLst);
            kwUtlUpdateOutStandingSduLst(dlIpThPut, *sduIdx, sdu->actSz, 
                   sdu->mode.am.sduId, newIdx);
            if(KW_MEAS_IS_DL_UU_LOSS_MEAS_ON_FOR_RB(gCb,rbCb))
            {
               /* If actual size of the sdu is equal to msgLen
                * then it is first segment of the SDU */
               if(sdu->actSz == sdu->sduSz)
               {
                  segSduCnt++;
               }
            }
         }
#endif

         sdu->mBuf = remSeg;
         sdu->sduSz -= pduSz;
         sduSz = pduSz;
         sdu->mode.am.isSegmented = TRUE;

         /* Update FI and Li */
         txBuf->amHdr.fi |= KW_FI_LAST_SEG; /* 01 */

#ifdef LTE_L2_MEAS_RLC
         kwUtlUpdSduSnMap(rbCb, sdu, kwDatReq, FALSE);
#endif /*  LTE_L2_MEAS */
         /* Create PDU with hdr and data */
         kwAmmCreatePdu(gCb,rbCb, &(txBuf->amHdr), pdu);
         pduCompleted = TRUE;

         amDl->nxtTx = sdu;
         nxtTxUpd    = TRUE;
      }

      /* Update bo for boReport */
      amDl->bo -= sduSz;

      sduMap[numSdu].sdu = sdu;
      sduMap[numSdu++].sduSz = sduSz;
      if ((pduCompleted) || (numSdu >= KW_MAX_DL_LI))
      {
         /* ccpu00137221 : KW Fix */
         if ((numSdu > KW_MAX_DL_LI) || (numSdu > KW_MAX_LI))
         {
            KW_FREE_BUF_WC(pdu);
            pdu = NULLP;
            pduSz = 0;
            break;
         }
         /* Update pduInfo */
         kwDatReq->pduInfo.mBuf[kwDatReq->pduInfo.numPdu] = pdu; 
         kwDatReq->pduInfo.numPdu++;
         numNewPdu++;
         /* kw005.201 ccpu00117318, updating the statistics */
         gCb->genSts.pdusSent++;

         /* Update the RLC Tx buffer with the new PDU info */
         txBuf->numSdu = numSdu;
         KW_MEM_CPY(txBuf->sduMap, sduMap, sizeof(KwSduMap) * numSdu);
         pdu = NULLP;
         numSdu = 0;
      }

      pduSz -= sduSz;

      /* Get next sdu for assembly */
      KW_LLIST_NEXT_SDU(amDl->sduQ, sdu);
   }

   kwDatReq->pduSz = pduSz;
   /* Updating nxtTx to sdu in the Q */
   if (!nxtTxUpd)
      amDl->nxtTx = sdu;

#ifdef LTE_L2_MEAS
   if(KW_MEAS_IS_DL_ANY_MEAS_ON_FOR_RB(gCb,rbCb) && 
       (rbCb->rlcId.rbType == CM_LTE_DRB))
   {
      numSdus = 0;
      currSduIdx = 0;
      l2MeasTb = kwUtlGetCurMeasTb(gCb, rbCb);
      kwUtlUpdateBurstSdus(gCb, rbCb, &contSduLst, dataVol, *totMacGrant);
      if ((lchInfo.numSdus != 0) && (l2MeasTb != NULLP))
      {
         for (lchIdx = 0; ((lchIdx < l2MeasTb->numLchInfo)
              && (lchIdx < KW_MAX_ACTV_DRB )); lchIdx++)
         {
            if (l2MeasTb->lchInfo[lchIdx].lcId == rbCb->lch.lChId)
            {
               /* Lch Info already added in Retx procedure */
               break;
            }
         }
         if (lchIdx < KW_MAX_ACTV_DRB)
         {
            if (lchIdx == l2MeasTb->numLchInfo)
            {
               l2MeasTb->lchInfo[lchIdx].lcId = rbCb->lch.lChId;
               l2MeasTb->lchInfo[lchIdx].numSdus = 0;
               l2MeasTb->numLchInfo++;
            }
            dstLchInfo = &l2MeasTb->lchInfo[lchIdx];
            currSduIdx = l2MeasTb->lchInfo[lchIdx].numSdus;
            while ((numSdus < lchInfo.numSdus) && (currSduIdx < KW_L2MEAS_SDUIDX)) 
            {
               dstLchInfo->sduInfo[currSduIdx].arvlTime = lchInfo.sduInfo[numSdus].arvlTime;
               dstLchInfo->sduInfo[currSduIdx].isRetxPdu = FALSE;
               numSdus++;
               currSduIdx++;
            }
            l2MeasTb->lchInfo[lchIdx].numSdus += numSdus;
         }
      }
      /* Fix Klock warning */
      if(l2MeasTb != NULLP)
      {
      l2MeasTb->txSegSduCnt += segSduCnt;
   }
   }
   *totMacGrant -= (oldBo - amDl->bo);
#endif 
   /* ccpu00137221 : KW Fix */   
   if ((pdu != NULLP) && (pduSz > 0) && (numSdu <= (KW_MAX_DL_LI+1))) /*Rahul:Fix for pduSz becoming zero*/
   {
      if (!pduCompleted)
      {

         /* Create PDU with hdr and data */
         kwAmmCreatePdu(gCb,rbCb, &(txBuf->amHdr), pdu);

         kwDatReq->pduInfo.mBuf[kwDatReq->pduInfo.numPdu] = pdu; 
         kwDatReq->pduInfo.numPdu++;
         numNewPdu++;

         gCb->genSts.pdusSent++;

         /* Update PDU /SDU Map */
         KW_LLIST_LAST_SDU(amDl->sduQ, sdu); 
         
#if (ERRCLASS & ERRCLS_ADD_RES)
         if (sdu == NULLP)
         {
            RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
                  "Memory allocation failed UEID:%d CELLID:%d",
                  rbCb->rlcId.ueId,
                  rbCb->rlcId.cellId);
            KW_SHRABL_STATIC_BUF_FREE(kwuSap->pst.region, kwuSap->pst.pool, discSduInfo, sizeof(KwuDiscSduInfo));
            RETVOID;
         }
#endif /* ERRCLASS & ERRCLS_RES */

         txBuf->numSdu = numSdu;
         KW_MEM_CPY(txBuf->sduMap, sduMap, sizeof(KwSduMap) * numSdu);
      }
   }

   if(discSduInfo->numSduIds != 0)
   {
      /* Sap control block */
      KwUiKwuDiscSduCfm(&kwuSap->pst, kwuSap->suId, discSduInfo);
   }
   else
   {
      KW_SHRABL_STATIC_BUF_FREE(kwuSap->pst.region, kwuSap->pst.pool, discSduInfo, sizeof(KwuDiscSduInfo));
   }
   
   
#ifndef ALIGN_64BIT
   RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
            "kwAssembleSdus: BO after assembly = %ld UEID:%d CELLID:%d",
            amDl->bo,
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);
#else
   RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId,
            "kwAssembleSdus: BO after assembly = %d UEID:%d CELLID:%d"
            amDl->bo,
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);
#endif

   RETVOID;
}

/**
 * @brief Private handler to check if the poll bit needs to be set for data PDU
 *
 * @details
 *    Its a private function called by kwProcessSdus, to checks if the 
 *    polling bit needs to be set for any RLC data PDU and updates the 
 *    same.
 *    - For the new PDUs, if the counters exceed the configured 
 *      pduWoPoll/byteWoPoll values, return poll bit.
 *    - For the PDUs/portion of PDUs, if the SDU list / retxBuf is 
 *      empty, return poll bit.
 *    - Update the pollPdu, pollByte counters and Poll_SN; start staProhTmr 
 *
 * @param[in]  rCb      RLC instance control block
 * @param[in]  rbCb     RB control block 
 * @param[in]  newPdu   Flag to indicate if its a new AMD PDU. 
 * @param[in]  bufSz    Length of the PDU
 *
 * @return  Bool 
 *      -# 1 - To set the poll bit
 *      -# 0 - Poll bit is not set
 *
 */
#ifdef ANSI
PRIVATE Bool kwAmmDlCheckAndSetPoll
(
KwCb       *gCb,
KwDlRbCb   *rbCb,
Bool       newPdu,
MsgLen     bufSz
)
#else
PRIVATE Bool kwAmmDlCheckAndSetPoll(gCb, rbCb, newPdu, bufSz)
KwCb       *gCb;
KwDlRbCb   *rbCb;
Bool       newPdu;
MsgLen     bufSz;
#endif
{ 
   Bool     pollBit = FALSE;
   KwAmDl   *amDl = &(rbCb->m.amDl);

   TRC2(kwAmmDlCheckAndSetPoll)

 
   /* If it's a new PDU increment PDU without poll and bytes without poll
    and check if they cross the configured number of poll pdu and poll bytes*/ 
   if (newPdu)
   {
      amDl->pduWoPoll++;
      /* Patch kw004.201 */
      amDl->byteWoPoll += bufSz;

     if (((amDl->pollPdu != -1) && (amDl->pduWoPoll >= amDl->pollPdu)) || 
         ((amDl->pollByte != -1) && (amDl->byteWoPoll >= amDl->pollByte))) 
     {
        pollBit = TRUE;
     }
   }

   /* Check if both tx/retx buffer are empty or if tx window is stalled */
   if (((amDl->nxtTx == NULLP) && (amDl->nxtRetx == NULLP)) ||
       KW_AM_IS_TRANS_WIN_STALLED(amDl))
   {
      pollBit = TRUE;
   }
   
   if (pollBit)
   {
      amDl->pduWoPoll  = 0;
      amDl->byteWoPoll = 0;

      amDl->pollSn = (amDl->vtS - 1) & 0x3ff;

      RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
                "kwAmmDlCheckAndSetPoll: Poll SN = %d UEID:%d CELLID:%d", 
                amDl->pollSn,
                rbCb->rlcId.ueId,
                rbCb->rlcId.cellId);

      /* kw005.201: Fix for poll retransmission timer. 
       * Timer is stopped if it is already running and 
       * then starting the timer. Fixes crs 
       * ccpu00117216 and ccpu00118284 .
       * */
      if( TRUE == kwChkTmr(gCb,(PTR)rbCb,KW_EVT_AMDL_POLL_RETX_TMR) )
      {
         kwStopTmr(gCb,(PTR)rbCb, KW_EVT_AMDL_POLL_RETX_TMR);
      }

      kwStartTmr(gCb,(PTR)rbCb, KW_EVT_AMDL_POLL_RETX_TMR);
   }

   RETVALUE(pollBit);
}

/**
 * @brief Private handler to create AMD PDU 
 *
 * @details
 *    This function constructs header and concatenate it with the data for
 *    the PDU. It also updates the txBuf with the created PDU.
 *
 * @param[in]  gCB      RLC instance control block
 * @param[in]  rbCb     Downlink RB control block
 * @param[in]  amHdr    AM header
 * @param[in]  pdu      PDU buffer 
 *
 *  @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmCreatePdu
(  
KwCb       *gCb,
KwDlRbCb   *rbCb,
KwAmHdr    *amHdr,
Buffer     *pdu
)
#else
PRIVATE Void kwAmmCreatePdu(gCb, rbCb, amHdr, pdu)
KwCb       *gCb;
KwDlRbCb   *rbCb;
KwAmHdr    *amHdr;
Buffer     *pdu;
#endif
{
   U8       hdr[KW_MAX_HDRSZ];
   U16      idx;
   KwTx     *txBuf;
   MsgLen   pduSz;
   KwAmDl   *amDl = &(rbCb->m.amDl);

   TRC2(kwAmmCreatePdu)


   /* Update sn */
   amHdr->sn = amDl->vtS;

   /* Increment vtS */
   amDl->vtS = (amDl->vtS + 1) & 0x3ff;

   /* Update hdr Info */
   amHdr->rf = 0;
   SFndLenMsg(pdu, &pduSz);

   /* passing newPDU = TRUE*/
   amHdr->p = kwAmmDlCheckAndSetPoll(gCb,rbCb, TRUE, pduSz); 

   /* Construct header with the available hdr Info, set isSegment to FALSE */
   kwConstructAmHdr(gCb,amHdr, FALSE, hdr, &idx);

   /* Concatenate hdr and data */
   SAddPreMsgMultInOrder(hdr, idx + 1, pdu);
   txBuf = amDl->txBuf[amHdr->sn];
   kwCpyMsg(gCb,pdu,&(txBuf->pdu));
   txBuf->pduSz = pduSz;
   txBuf->hdrSz = idx+1;

   gCb->genSts.bytesSent += pduSz;
   
   RETVOID;
}

/**
 * @brief Private handler to remove the retx PDU from the rbCb
 *
 * @details
 *    This function releases a retx PDU stored on DL portion of rbCb.
 *    It also updates the BO if wtForAck flag is not set which implies
 *    that it is not sent out yet.
 *
 * @param[in]  gCb        RLC instance control block
 * @param[in]  retx       retransmit PDU to be removed
 * @param[in]  rbCb       Radio Bearer Control Block
 *
 * @return Void 
 *
 */
#ifdef ANSI
PRIVATE Void kwRemRetxPdu
(  
KwCb          *gCb,
KwDlRbCb      *rbCb,
KwRetx        *retx
)
#else
PRIVATE Void kwRemRetxPdu(gCb, rbCb, retx)
KwCb          *gCb;
KwDlRbCb      *rbCb;
KwRetx        *retx;
#endif
{
   TRC2(kwRemRetxPdu)

   cmLListDelFrm(&AMDL.retxLst, &retx->lstEnt); 

   if( AMDL.retxLst.count == 0)
   {
      AMDL.nxtRetx = NULLP;
   }

   if(retx->pendingReTrans == TRUE)
   {
      AMDL.retxBo -= retx->segSz;
      AMDL.estHdrSz -= retx->hdrSz;
   }

   kwUtlAddReTxPduToBeFreedQueue(gCb, retx);
   kwUtlRaiseDlCleanupEvent(gCb);

   RETVOID;
}

/**
 * @brief Private handler to mark a retx PDU for further retransmission
 *
 * @details
 *    This function sets a retx PDU that has not been ACKed in the   
 *    received Status PDU for futher retransmission. If the retransmission
 *    limit is reached, it releases the retx PDU and informs the higher
 *    layers about the same.
 *
 * @param[in]  gCb        RLC instance control block 
 * @param[in]  retx       retransmit PDU to be removed
 * @param[in]  rbCb       Radio Bearer Control Block
 *
 * @return Void 
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmDlMarkPduForReTx
(
KwCb          *gCb,
KwDlRbCb      *rbCb,
KwRetx        *retx
)
#else
PRIVATE Void kwAmmDlMarkPduForReTx(*gCb, rbCb, retx)
KwCb          *gCb;
KwDlRbCb      *rbCb;
KwRetx        *retx;
#endif
{
   TRC2(kwAmmDlMarkPduForReTx)
   if (AMDL.maxReTxReached == TRUE)
   {
      RETVOID;
   }
  
   if(retx->pendingReTrans == FALSE)
   {
      retx->pendingReTrans = TRUE;
      ++retx->retxCnt;

      AMDL.retxBo   += retx->segSz;
      AMDL.estHdrSz += retx->hdrSz;

      if (retx->retxCnt > AMDL.maxRetx)
      {
         /* RLC_DL_MAX_RETX fix */
         /* Marking the RB stalled for DL scheduling. This is to avoid unnecessary */
         /* preparation of RLC PDUs and adding the same to Tx Buffer */
         /* This condition is to avoid sending StaIndication more than once */
         if (TRUE != rbCb->m.amDl.maxReTxReached)
         {
            rbCb->m.amDl.maxReTxReached = TRUE;
            rbCb->m.amDl.bo = 0;
            rbCb->m.amDl.cntrlBo = 0;
            rbCb->m.amDl.retxBo = 0;
            /* Sending BO update to SCH */
            kwUtlSndDStaRsp(gCb, rbCb, 0,0,0);
            kwAmmSndStaInd(gCb, rbCb, retx);
            gRlcStats.amRlcStats.numDLMaxRetx++;
         }

         kwRemRetxPdu(gCb,rbCb, retx);

         RETVOID;
      }


      if (AMDL.nxtRetx == NULLP)
      {
         AMDL.nxtRetx = retx;
      }

      gRlcStats.amRlcStats.numDLRetransPdus++;
   }


   RETVOID;
}

/**
 * @brief Private handler to check if SDU is completely deliverd and
 *        send higher layers data confirmation
 *
 * @details 
 *    This function sends higher layers data confirmation for SDUs which
 *    have been successfully delivered to the peer RLC entity.
 *
 * @param[in]  gCb      RLC instance control block
 * @param[in]  rbCb     Radio Bearer Control Block
 * @param[in]  sduLst   List of SDUs that were part of the PDU
 * @param[in]  numSdu   Number of SDUs in the list
 *
 * @return Void 
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmDlCheckIsSDUDelivered
(
KwCb            *gCb,
KwDlRbCb        *rbCb,
KwSduMap        *sduLst,
U8              numSdu,
KwuDatCfmInfo   **datCfm
)
#else
PRIVATE Void kwAmmDlCheckIsSDUDelivered(gCb, rbCb, sduLst, numSdu, datCfm)
KwCb            *gCb;
KwDlRbCb        *rbCb;
KwSduMap        *sduLst;
U8              numSdu;
KwuDatCfmInfo   **datCfm;
#endif
{
   U32    i;
   KwSdu *sdu;
   
   TRC2(kwAmmDlCheckIsSDUDelivered)
   
   for (i = 0; i < numSdu && i < KW_MAX_LI; i++)
   {
      sdu = sduLst[i].sdu;
      
      sdu->mode.am.rcvdSz += sduLst[i].sduSz;
                           
      /* send a dat cfm if all the bytes of the sdu have been received */
      if (sdu->mode.am.rcvdSz == sdu->actSz)
      {
         /* Send DatCfm for this sdu */
         if((*datCfm)->numSduIds < KWU_MAX_DAT_CFM)
         {
            (*datCfm)->sduIds[(*datCfm)->numSduIds++] = sdu->mode.am.sduId;
         }
         else
         {
            /* This is an error that should never happen, we should resize
             * the #define to a larger value or check why we need to 
             * send so many confirms in one go
             * Confrims to PDCP are being dropped in this case
             */
            KwKwuSapCb    *kwuSap;
            kwuSap = gCb->u.dlCb->kwuDlSap + KW_UI_PDCP;
            KwUiKwuDatCfm(&kwuSap->pst, kwuSap->suId, *datCfm);

            KW_SHRABL_STATIC_BUF_ALLOC(kwuSap->pst.region, kwuSap->pst.pool, *datCfm, sizeof(KwuDatCfmInfo));

            #if (ERRCLASS & ERRCLS_ADD_RES)
            if (*datCfm == NULLP)
            {
               RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
                     "Memory allocation failed UEID:%d CELLID:%d",
                     rbCb->rlcId.ueId,
                     rbCb->rlcId.cellId);
               RETVOID;
            }
            #endif /* ERRCLASS & ERRCLS_RES */

            (*datCfm)->numSduIds = 0;
            (*datCfm)->rlcId = rbCb->rlcId;
         /* ccpu00135618: say total 1026 sduIds to copy the 1025 sduId after
             * new allocation of datCfm */
            (*datCfm)->sduIds[(*datCfm)->numSduIds++] = sdu->mode.am.sduId;
         }

         /* Remove SDU from the sduQ */
         cmLListDelFrm(&AMDL.sduQ, &sdu->lstEnt);
         kwUtlAddSduToBeFreedQueue(gCb, sdu);
         kwUtlRaiseDlCleanupEvent(gCb);
      }
   }
   
   RETVOID;
} 

/**
 * @brief Private handler to mark a PDU successful.
 *
 * @details
 *    This function is called when we receive a STATUS pdu that marks
 *    a PDU as successful. It releases the PDU from RLC entity and 
 *    informs PDCP of successful SDUs delivered as a result of this PDU.
 *
 * @param[in]  gCb        RLC instance control block  
 * @param[in]  rbCb       Radio Bearer Control Block
 * @param[in]  sn         SN that is successfully delivered to the peer 
 *
 * @return Void 
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmDlProcessSuccessfulTxPdu
(
KwCb            *gCb,
KwDlRbCb        *rbCb,
KwSn            sn,
KwuDatCfmInfo   **datCfm
)
#else
PRIVATE Void kwAmmDlProcessSuccessfulTxPdu(gCb, rbCb, sn, datCfm)
KwCb            *gCb;
KwDlRbCb        *rbCb;
KwSn            sn;
KwuDatCfmInfo   **datCfm;
#endif
{
   TRC2(kwAmmDlProcessSuccessfulTxPdu)

   kwAmmDlCheckIsSDUDelivered(gCb,
                              rbCb,  
                              AMDL.txBuf[(sn)]->sduMap, 
                              AMDL.txBuf[(sn)]->numSdu,
                              datCfm);
   
   kwUtlAddTxPduToBeFreedQueue(gCb, AMDL.txBuf[sn]);
   kwUtlRaiseDlCleanupEvent(gCb);
   /* so that it is not processed again */
   AMDL.txBuf[sn] = NULLP;

   RETVOID;
}

/**
 * @brief  Handler to estimate the header size of the RLC SDUs
 *
 * @details
 *    This function is used to update the estimated Header size for the BO
 *    report. This function is called when a SDU is queued and also when 
 *    a PDU is formed and sent to the lower layer.
 *  
 * @param[in] gCb    RLC instance control block
 * @param[in] amDl   AM DL Information 
 * @param[in] type   Type of Operation (Addition or Deletion of SDU)
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmEstHdrSz
(
KwCb     *gCb,
KwAmDl   *amDl,
U8       type
)
#else
PRIVATE Void kwAmmEstHdrSz(gCb, amDl, type)
KwCb     *gCb;
KwAmDl   *amDl;
U8       type;
#endif
{
   TRC2(kwAmmEstHdrSz);

   if (type == KW_ADD_SDU)
   {
      amDl->estHdrSz += (amDl->numSduPdu * 2);

   }
   else if (type == KW_DEL_SDU)
   {
      amDl->estHdrSz -= (amDl->numSduPdu * 2);
   }   

   RETVOID;
}

/**
 * @brief  Handler to send Status Indication to PDCP
 *
 * @details
 *    This function is used to send status indication to PDCP when the 
 *    maximum retransmission threshold value is reached for a PDU.
 * 
 * @param[in] gCb    RLC instance control block
 * @param[in] rbCb   RB control block 
 * @param[in] retx   The PDU/segment that failed max re-transmissions
 *
 * @return  Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmSndStaInd
(
KwCb       *gCb,
KwDlRbCb   *rbCb,
KwRetx     *retx
)
#else
PRIVATE Void kwAmmSndStaInd(gCb, rbCb, retx)
KwCb       *gCb;
KwDlRbCb   *rbCb;
KwRetx     *retx;
#endif
{
   KwuStaIndInfo   *staInd;
   KwKwuSapCb      *kwuSap;
   U32             i;

   TRC2(kwAmmSndStaInd);


   /* Sap control block */
   kwuSap = gCb->u.dlCb->kwuDlSap + KW_UI_PDCP;

   /* Allocate memory for staInd Info */
   KW_SHRABL_STATIC_BUF_ALLOC(kwuSap->pst.region, kwuSap->pst.pool, staInd, sizeof(KwuStaIndInfo));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (staInd == NULLP)
   {
      RLOG_ARG2(L_FATAL,DBG_RBID,rbCb->rlcId.rbId, 
            "Memory allocation failed UEID:%d CELLID:%d",
            rbCb->rlcId.ueId,
            rbCb->rlcId.cellId);
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_RES */

   /* Fill staInd Info */ 
   KW_MEM_CPY(&staInd->rlcId, &rbCb->rlcId, sizeof(CmLteRlcId));    
   if(retx->numSdu > KWU_MAX_STA_IND_SDU) 
   {
      staInd->numSdu = KWU_MAX_STA_IND_SDU;
   }
   else
   {
      staInd->numSdu = retx->numSdu;
   }
   for (i = 0; i < staInd->numSdu; i++) 
   {
      staInd->sduId[i] = retx->sduMap[i].sdu->mode.am.sduId;
   }

#ifdef KW_PDCP
#else
   KwUiKwuStaInd(&kwuSap->pst, kwuSap->suId, staInd);
#endif /* KW_PDCP */

   RETVOID;
}

/**
 * @brief  Handler to get the next node to be retransmitted from retxLst
 *
 * @details
 *    This function is used to get the next node to be retransmitted 
 *    from the retxLst
 *            
 *  @param[in] gCb     RLC instance control block 
 *  @param[in] retx    The PDU/segment after which to find a node to be 
 *                     retransmitted
 *
 *  @return  Void 
 *
 */
#ifdef ANSI
PRIVATE Void kwGetNxtRetx 
(
KwCb     *gCb,
KwRetx   **retx
)
#else
PRIVATE Void kwGetNxtRetx(gCb, retx)
KwCb     *gCb;
KwRetx   **retx;
#endif
{
   CmLList *tNode;

   TRC2(kwGetNxtRetx);

   do
   {
      tNode = &((*retx)->lstEnt);
      tNode = tNode->next;
      
      if (tNode)
      {
         *retx = (KwRetx *)tNode->node;
      }
      else
      {
         *retx = NULLP;
         RETVOID;
      }
   }while((*retx)->pendingReTrans == FALSE);

   RETVOID;
}

/**
 * @brief  Handler to process the re-establishment request received from UIM
 *
 * @param[in]  gCb         RLC instance control block
 * @param[in]  rlcId       Identity of the RB in the UE/Cell for which 
 *                         re-establishment is to be done
 * @param[in]  rbCb        Downlink RB control block (rbCb is freed in this
 *                         function)
 *
 * @return  Void 
 *
 */
#ifdef ANSI
PUBLIC Void kwAmmDlReEstablish
(
KwCb         *gCb,
CmLteRlcId   rlcId,
KwDlRbCb     *rbCb
)
#else
PUBLIC Void kwAmmDlReEstablish(gCb, rlcId, rbCb)
KwCb         *gCb;
CmLteRlcId   rlcId;
KwDlRbCb     *rbCb;
#endif
{
   /* create a new AM DL RB, reset it and replace in the UeCb*/
   KwDlUeCb   *ueCb;
   KwDlRbCb   *resetRb;
   KwAmDl* newAmDl;
   KwAmDl* oldAmDl;
   KW_ALLOC(gCb, resetRb, sizeof(KwDlRbCb));
   
   /* ccpu00135170 Removing KLOCK warning */
   if(resetRb == NULLP)
   {
      RETVOID;
   }

   KW_MEM_CPY(resetRb, rbCb, sizeof(KwDlRbCb));
   KW_MEM_SET(&resetRb->m.amDl, 0 , sizeof(KwAmDl));

/* AGHOSH changes start */
   /* restore the old AM values */
   newAmDl = &resetRb->m.amDl;
   oldAmDl = &rbCb->m.amDl;

   newAmDl->pollPdu = oldAmDl->pollPdu;
   newAmDl->pollByte = oldAmDl->pollByte;
   newAmDl->maxRetx = oldAmDl->maxRetx;
   newAmDl->pollRetxTmrInt = oldAmDl->pollRetxTmrInt;
   rbCb->boUnRprtdCnt = (U32)0;
   rbCb->lastRprtdBoToMac = (U32)0;
   cmInitTimers(&(resetRb->m.amDl.pollRetxTmr), 1); 
/* AGHOSH changes end */
 
   /* set the window size in VT(MS) */
   resetRb->m.amDl.vtMs    = KW_AM_WIN_SZ;

   if (ROK != kwDbmFetchDlUeCb(gCb,rlcId.ueId, rlcId.cellId, &ueCb))
   {
      RLOG_ARG2(L_ERROR,DBG_CELLID, rlcId.cellId,
               "UeId [%d]: UeCb not found RBID;%d",
               rlcId.ueId,
               rlcId.rbId);
      RETVOID;
   }
   
   if(rlcId.rbType == CM_LTE_SRB)
   {
      ueCb->srbCb[rlcId.rbId] = resetRb;
   }
   else
   {
      ueCb->drbCb[rlcId.rbId] = resetRb;
   }
   /* update into the logical channel array also */
   ueCb->lCh[rbCb->lch.lChId - 1].dlRbCb = resetRb;
 
   if((resetRb->rlcId.rbType == CM_LTE_SRB)
                &&(resetRb->rlcId.rbId == 1))
   {
      /* To stop the traffic on SRB2 and other DRBs*/
      kwDlUtlSetReestInProgressForAllRBs(gCb, ueCb);
   }
   else 
   {
      kwDlUtlSetReestInProgressForRB(gCb, resetRb);      
   }

   /* allocate the TX array again */
   KW_ALLOC(gCb,
         resetRb->m.amDl.txBuf,
         ((KW_AM_WIN_SZ << 1) * sizeof(KwTx*)));
   /* send the old rb of deletion */
   kwAmmFreeDlRbCb(gCb,rbCb); 


   /* TODO: for now we are re-settting the re-establishment flag here
      this needs to be fixed
      There should be a proper intreface to resume the RBs */
   if(rlcId.rbType == CM_LTE_SRB)
   {
      kwDlUtlResetReestInProgress(ueCb->srbCb[rlcId.rbId]);
   }
   else
   {
      kwDlUtlResetReestInProgress(ueCb->drbCb[rlcId.rbId]);
   }      
      
   RETVOID;
}

/**
 * @brief  Handler to discard a SDU.
 *
 * @details
 *    This function is used to discard a SDU after receiving
 *    the Discard Request from UIM. The SDU is discarded if its 
 *    available and is not mapped to any PDU yet.
 *   
 * @param[in] gCb     RLC instance control block
 * @param[in] rbCb    RB control block 
 * @param[in] sduId   Sdu ID of the SDU to be discarded
 *
 *  @return  S16
 *     -# ROK      In case of successful discard
 *     -# RFAILED  In case the SDU is not found or already mapped
 */
#ifdef ANSI
PUBLIC S16 kwAmmDiscSdu
(
KwCb       *gCb,
KwDlRbCb   *rbCb,
U32        sduId 
)
#else
PUBLIC S16 kwAmmDiscSdu(gCb, rbCb, sduId)
KwCb       *gCb;
KwDlRbCb   *rbCb;  
U32        sduId; 
#endif
{
   TRC2(kwAmmDiscSdu);
   RETVALUE(RFAILED);
} 

/**
 * @brief  Handler for Poll retransmit timer expiry
 *
 * @details
 *    This function is used to handle events upon expiry of Poll 
 *    retransmit timer.
 *  
 * @param[in] gCb    RLC instance control block
 * @param[in] rbCb   Downlink RB control block 
 *
 * @return  Void 
 */
#ifdef ANSI
PUBLIC Void kwAmmPollRetxTmrExp
(
KwCb       *gCb,
KwDlRbCb   *rbCb
)
#else
PUBLIC Void kwAmmPollRetxTmrExp(gCb, rbCb)
KwCb       *gCb;
KwDlRbCb   *rbCb;
#endif
{
   KwRetx        *retx; 
   KwAmDl        *amDl = &(rbCb->m.amDl);
   KwSn          sn;

   TRC2(kwAmmPollRetxTmrExp);

   
   /* kw003.201 - Correcting the logic for determmining whether to do   */
   /*             any transmission of PDU. As per the spec section      */
   /*             5.2.2.3, if there is any to transmit or retransmit,   */
   /*             do nothing. Else, pick up the VT(S) -1 for retx       */
   /*             We have nothing to transmit if window is stalled or   */
   /*             there are no SDUs to be transmitted or if there are   */
   /*             PDUs to be retransmitted.                             */
   if(CM_LTE_SRB == rbCb->rlcId.rbType)
   {
      gRlcStats.amRlcStats.numDLPollTimerExpiresSrb++;
   }
   else
   {
      gRlcStats.amRlcStats.numDLPollTimerExpiresDrb++;
   }

   if (((amDl->nxtTx == NULLP) && (amDl->nxtRetx == NULLP)) || 
        KW_AM_IS_TRANS_WIN_STALLED(amDl)) 
   {
      sn = (amDl->vtS - 1) & 0x3ff;
      if (amDl->txBuf[sn] != NULLP)
      {
         kwAmmDlMoveFrmTxtoRetxBuffer(gCb,amDl, &retx, sn); 
         
         if (AMDL.nxtRetx == NULLP)
         {
            AMDL.nxtRetx = retx;
         }
         
         kwAmmSendDStaRsp(gCb, rbCb, &AMDL);         
         RETVOID;
      }
      /* Get the last node in retxLst */
      KW_LLIST_LAST_RETX(amDl->retxLst, retx);

      /* Unset wtForAck flag for the NACK PDUs */ 
      if (retx != NULLP)
      {
         kwAmmDlMarkPduForReTx(gCb, rbCb, retx);
         kwAmmSendDStaRsp(gCb, rbCb, &AMDL);         
      }
   }

   RETVOID;
} 

/**
 * @brief  Handler to update Acks for the remaining PDUs after the last accessed
 *         NACK PDU.
 *
 * @details 
 *    This function is used to handle ACKs for the PDUs remaining after the 
 *    last accessed NACK PDU, It updates the txBuf/retxBuf for the ACKs and 
 *    sends DatCfm to PDCP for the same.
 *
 * @param[in]  gCb         RLC instance control block
 * @param[in]  rbCb        Downlink Radio Bearer control block
 * @param[in]  mAckSn      The ACK SN after doing the base modulus
 * @param[in]  rextNode    Next node in the re-transmission buffer          
 *
 * @return  Void
 *
 */

#ifdef ANSI
PRIVATE Void kwAmmDlUpdateTxAndReTxBufForAckSn
(
KwCb            *gCb,
KwDlRbCb        *rbCb,
KwSn            mAckSn,
CmLList         *retxNode,
KwuDatCfmInfo   **datCfm
)
#else
PRIVATE Void kwAmmDlUpdateTxAndReTxBufForAckSn(gCb, rbCb, mAckSn, retxNode, datCfm)
KwCb            *gCb;
KwDlRbCb        *rbCb;
KwSn            mAckSn;
CmLList         *retxNode;
KwuDatCfmInfo   **datCfm;
#endif
{
   KwSn    mSn;
   KwSn    sn;
   KwRetx  *retx;

   TRC2(kwAmmDlUpdateTxAndReTxBufForAckSn);

   /* Remove pdus/segs from retxLst */ 
   while (retxNode)
   {
      retx = (KwRetx *)(retxNode->node);
      retxNode = retxNode->next;
      MODAMT(retx->amHdr.sn, mSn, AMDL.vtA);
      if (mSn < mAckSn) 
      {
         kwAmmDlProcessSuccessfulReTx(gCb,rbCb, retx, datCfm);
      }
   }

   /* For the remaining; pdus not acknowldeged by the NACK_SN but being
      acknowledged by the ACK_SN*/
   /* start from the starting of the transmission window and remove till just
      before ACK_SN*/
   mSn = 0;       /* same as MODAMT(AMDL.vtA, mSn, AMDL.vtA);*/
   sn = AMDL.vtA;
   while(mSn < mAckSn)
   {
      if (AMDL.txBuf[sn] != NULLP)
      {
         RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
                 "kwAmmDlUpdateTxAndReTxBufForAckSn: ACK for PDU "
                 "with sn = %ld UEID:%ld CELLID:%ld",
                 sn,
                 rbCb->rlcId.ueId,
                 rbCb->rlcId.cellId);

         kwAmmDlProcessSuccessfulTxPdu(gCb,rbCb, sn, datCfm);
      }
      
      sn = (sn + 1) & 0x3ff;
      MODAMT(sn, mSn, AMDL.vtA);
   }

   RETVOID;
}

/**
 * @brief  Handler to form construct AM header
 *
 * @details 
 *    This function is used to construct am header with the available header
 *    elements.
 *
 * @param[in] gCb    RLC instance control block
 * @param[in] amHdr  AM Header
 * @param[in] isSeg  Check for Segmentation of PDU
 * @param[in] hdr    Header field
 * @param[in] idx    Index
 *  
 * @return Void            
 *
 */
#ifdef ANSI
PRIVATE Void kwConstructAmHdr
(
KwCb      *gCb,
KwAmHdr   *amHdr,
Bool      isSeg,
U8        *hdr,
U16       *idx
)
#else
PRIVATE Void kwConstructAmHdr(gCb, amHdr, isSeg, hdr, idx)
KwCb      *gCb;
KwAmHdr   *amHdr;
Bool      isSeg;
U8        *hdr;
U16       *idx;
#endif
{
   KwHdrInfo hdrInfo;
   U8  e;                                                   
   S16 numLi;                                               
   U8  eb   = 0;                                              

   TRC2(kwConstructAmHdr);
                                                            
   KW_MEM_ZERO(&hdrInfo, sizeof(KwHdrInfo)); 

   hdrInfo.pEb  = 1;
   hdrInfo.pLen = 0;
   /* kw003.201 - Setting it to 0 as it should start at zero */
   hdrInfo.idx  = *idx = 0;

   numLi = amHdr->numLi - 1;                                

   /* For fixed part of the header */                       
   hdr[0] = KW_DATA_PDU;                                    
   hdr[0] = hdr[0] << (KW_BYTE_LEN - KW_DC_LEN);            
   eb = KW_BYTE_LEN - KW_DC_LEN;                            
 
   hdrInfo.hdr = hdr;
   hdrInfo.eb  = eb;

   KW_FILL_HDR_ARGS(hdrInfo, amHdr->rf, KW_RF_LEN); 
   kwFormBits(&hdrInfo);           

   KW_FILL_HDR_ARGS(hdrInfo, amHdr->p, KW_P_LEN);            
   kwFormBits(&hdrInfo);

   KW_FILL_HDR_ARGS(hdrInfo, amHdr->fi, KW_FI_LEN);
   kwFormBits(&hdrInfo);

   e = (numLi > 0) ? 1 : 0;
   KW_FILL_HDR_ARGS(hdrInfo, e, KW_E_LEN);
   kwFormBits(&hdrInfo);

   KW_FILL_HDR_ARGS(hdrInfo, amHdr->sn, KW_SN_LEN);
   kwFormBits(&hdrInfo);

   if (isSeg == TRUE)                                      
   {                                                        
      KW_FILL_HDR_ARGS(hdrInfo, amHdr->lsf, KW_LSF_LEN);
      kwFormBits(&hdrInfo);

      KW_FILL_HDR_ARGS(hdrInfo, amHdr->so, KW_SO_LEN);
      kwFormBits(&hdrInfo);
   }                                                        

   /* For extension part of the header */                   
   if (numLi > 0)                                           
   {                                                        
      U8 count = 0;                                       
      while (count < (amHdr->numLi-1))                               
      {                                                     
         e = (numLi > 1) ? 1 : 0;                           
         KW_FILL_HDR_ARGS(hdrInfo, e, KW_E_LEN);
         kwFormBits(&hdrInfo);

         KW_FILL_HDR_ARGS(hdrInfo, amHdr->li[count], KW_LI_LEN);
         kwFormBits(&hdrInfo);

         count++;
         numLi--;
         if(hdrInfo.idx >= KW_MAX_HDRSZ)
         {
             break;
         }

      }                                
   }                                                       

   hdr  = hdrInfo.hdr;
   *idx = hdrInfo.idx;

   RETVOID;
}

/**
 * @brief  Handler to form am header Info
 *
 * @details
 *    This function is used to assemble AM header Information
 *            
 * @param[in] hdrInfo  Header Information
 *
 * @return Void
 */

#ifdef ANSI
PRIVATE Void kwFormBits
(
KwHdrInfo *hdrInfo
)
#else
PRIVATE Void kwFormBits( hdrInfo)
KwHdrInfo *hdrInfo;
#endif
{
   U16  val  = hdrInfo->val;
   U8   len  = hdrInfo->len;
   U8   eb   = hdrInfo->eb;
   U8   *hdr = NULLP;
   U16  idx  = hdrInfo->idx;
   U8   pEb  = hdrInfo->pEb;
   U8   pLen = hdrInfo->pLen;

   U16 pIdx = idx;                                         
   U16 cIdx = idx + 1;                                     
   U8  tPdu;                                                
   U16 tVal;                                                

   TRC2(kwFormBits);

   hdr = hdrInfo->hdr;

   if (eb >= len)                                         
   {                                                        
      if (pEb - pLen == 0)
      {                                                    
         idx++;
         pIdx = idx;
      }                                                    
      tPdu = (val << (eb - len)) | hdr[pIdx]; 
      hdr[pIdx] = tPdu;                                   
      pEb = eb;                                           
      pLen = len;                                         
      eb = eb - len;       
   }                                                        
   else if (eb < len)                                     
   {                                                        
      tVal = val;                                          
      if (eb != 0)                                         
      {                                                     
         tPdu = (tVal >> (len - eb)) | hdr[pIdx];       
         hdr[pIdx] = tPdu;                                
      }                                                     
      if (len - eb == KW_BYTE_LEN)                        
      {                                                     
         hdr[cIdx] = (U8)tVal;                                
         idx = cIdx;                                       
         pEb = eb;
         pLen = len;                                      
         eb = KW_BYTE_LEN - (len - eb);                  
      }                                                     
      else if (len - eb < KW_BYTE_LEN)                    
      {                                                     
         if (eb == 0)                                     
            hdr[cIdx] = tVal << (KW_BYTE_LEN - len);     
         else                                              
           hdr[cIdx] = tVal << (len - eb);    
         idx = cIdx;                                       
         pEb = eb;                                        
         pLen = len;
         eb = KW_BYTE_LEN - (len - eb);                  
      }                                                     
      else                                                  
      {                                                     
         hdr[cIdx++] = tVal >> ((len - eb) - KW_BYTE_LEN); 
         hdr[cIdx] = tVal << KW_BYTE_LEN; 
         idx = cIdx;                                      
         pEb = eb;
         pLen = len;
         eb = KW_BYTE_LEN - ((len - eb) - KW_BYTE_LEN); 
      }                                                    
   }                                                       

   hdrInfo->val  = val;
   hdrInfo->len  = len;
   hdrInfo->eb   = eb;
   hdrInfo->hdr  = hdr;
   hdrInfo->idx  = idx;
   hdrInfo->pEb  = pEb;
   hdrInfo->pLen = pLen;

   RETVOID;
}

/**
 * @brief  This function adds a retx PDU to list of retx PDUs
 *
 * @details
 *    kw003.201 - Poll expiry may cause an SN to be added to retx 
 *                out of sequence and hence all additions to retx 
 *                must validate that they are added in sequence   
 *
 * @param[in] amDl   AM Downlink Control Block
 * @param[in] retx   Retransmit PDU
 * 
 * @return Void
 *            
 */
#ifdef ANSI
PRIVATE Void kwAmmAddPduToRetxLst
(
KwAmDl   *amDl,
KwRetx   *retx
)
#else
PRIVATE Void kwAmmAddPduToRetxLst(amDl, retx)
KwAmDl   *amDl;
KwRetx   *retx;
#endif
{
   CmLList   *node;
   KwRetx    *tRetx;
   KwSn      tMSn;
   KwSn      retxMSn;
   
   TRC2(kwAmmAddPduToRetxLst);
 
   node = amDl->retxLst.last;
   MODAMT(retx->amHdr.sn, retxMSn, amDl->vtA);
   while(node != NULLP)
   {
      tRetx = (KwRetx *)(node->node);
      MODAMT(tRetx->amHdr.sn, tMSn, amDl->vtA);
      if (tMSn > retxMSn)
      {
         node = node->prev;
      }
      else
      {
         break;
      }
   }
   if (node)
   {
      amDl->retxLst.crnt = node;
      cmLListInsAfterCrnt(&amDl->retxLst, &retx->lstEnt);
      retx->lstEnt.node = (PTR)retx;
   }
   else
   {
      amDl->retxLst.crnt = amDl->retxLst.first;
      cmLListInsCrnt(&amDl->retxLst, &retx->lstEnt);
      retx->lstEnt.node = (PTR)retx;
   }

   if (amDl->nxtRetx == NULLP)
   {
      amDl->nxtRetx = retx;
   }

   RETVOID;
}

/**
 * @brief Handler to Move the PDU from txBuf to re-transmission buffer 
 *
 * @details
 *    This function is used to move the PDU from the txBuf to re-transmit buffer
 *
 * @param[in]  gCb         RLC instance control block
 * @param[in]  amDl        AM Downlink Control Block 
 * @param[in]  retx        node in the reTx buffer to be moved to, allocated by
 *                         this function
 * @param[in]  sn          SN in the tx buffer which needs to be moved
 * 
 * @return Void
 *            
 */

#ifdef ANSI
PRIVATE Void kwAmmDlMoveFrmTxtoRetxBuffer
(
KwCb          *gCb,
KwAmDl        *amDl,
KwRetx        **retx,
KwSn          sn
)
#else
PRIVATE Void kwAmmDlMoveFrmTxtoRetxBuffer(gCb, amDl, retx, sn)
KwCb          *gCb;
KwAmDl        *amDl;
KwRetx        **retx;
KwSn          sn; 
#endif
{
   U32 maxSdu;
   KwTx* transmittedBuf = amDl->txBuf[sn];
   TRC2(kwAmmDlMoveFrmTxtoRetxBuffer);


   KW_ALLOC_WC(gCb,*retx, sizeof(KwRetx));

#if (ERRCLASS & ERRCLS_ADD_RES)
   if (*retx == NULLP)
   {
      RLOG0(L_FATAL, "Memory allocation failed");
      RETVOID;
   }
#endif /* ERRCLASS & ERRCLS_RES */

   (*retx)->seg = transmittedBuf->pdu;
   (*retx)->segSz = transmittedBuf->pduSz;
   /* MS_FIX for DL stall */
   (*retx)->soEnd = transmittedBuf->pduSz - 1;

   (*retx)->hdrSz = transmittedBuf->hdrSz;
   (*retx)->numSdu = transmittedBuf->numSdu;
   (*retx)->retxCnt = 1;
   (*retx)->yetToConst = 0;
   (*retx)->pendingReTrans = TRUE;

   /* initialize the list pointer to 0 instead of memset */
   (*retx)->lstEnt.next = 0;
   (*retx)->lstEnt.prev = 0;
   maxSdu =  (transmittedBuf->numSdu < KW_MAX_DL_LI)?transmittedBuf->numSdu:
                                                   KW_MAX_DL_LI;
   /* copy the sdu maps */
   KW_MEM_CPY((*retx)->sduMap, 
              transmittedBuf->sduMap, 
              sizeof(KwSduMap) * maxSdu);

   KW_MEM_CPY(&((*retx)->amHdr), &transmittedBuf->amHdr, sizeof(KwAmHdr));
   (*retx)->amHdr.lsf = 1;
   kwAmmAddPduToRetxLst(amDl, (*retx));

   /* Remove PDU from txBuf */
   KW_FREE_WC(gCb, transmittedBuf, sizeof(KwTx));
   amDl->txBuf[sn] = NULLP; /* set it to NULL, meaning not used */

   /* Update the BO appropriately */
   amDl->retxBo   += (*retx)->segSz;
   amDl->estHdrSz += (*retx)->hdrSz;

   gRlcStats.amRlcStats.numDLRetransPdus++;
   RETVOID;
}

/*
 * @brief
 *    function to free/release the Acknowledged mode RBCB buffers
 *
 * @details
 *    This primitive Frees the Acknowledged Mode RbCb transmission Buffer,
 *    retransmission Buffer and reciption Buffers
 *
 * @param [in]   gCb    - RLC instance control block
 * @param [in]   rbCb   - Downlink RB Control Block
 *
 * @return Void
 */
#ifdef ANSI
PUBLIC Void kwAmmFreeDlRbCb
(
KwCb       *gCb,
KwDlRbCb   *rbCb
)
#else
PUBLIC Void kwAmmFreeDlRbCb(gCb,rbCb)
KwCb       *gCb;
KwDlRbCb   *rbCb;
#endif
{
   /* stop the re-transmission timer */
   if(TRUE == kwChkTmr(gCb,(PTR)rbCb,KW_EVT_AMDL_POLL_RETX_TMR))
   {
      kwStopTmr(gCb,(PTR)rbCb, KW_EVT_AMDL_POLL_RETX_TMR);
   }

   /* store the entire Rb pointer */      
   rbCb->rlsLnk.node = (PTR)rbCb;
   cmLListAdd2Tail(&gCb->u.dlCb->toBeFreed.rbLst, &rbCb->rlsLnk);

   /* the sdu queue */
   cmLListCatLList(&(gCb->u.dlCb->toBeFreed.sduLst),&(rbCb->m.amDl.sduQ));

   kwUtlRaiseDlCleanupEvent(gCb);
   
   RETVOID;
}

/**
 * @brief  Handler to check the TB size enough to fit the status Pdu
 *
 * @details
 *    This function find the number of bytes required to encode the passed
 *    nackInfo along with the numOfBits. It updates the numOfBits
 *
 * @return Void
 *
 */

#ifdef ANSI
PRIVATE Void kwAmmCalcBytesReqdForNacks
(
KwNackInfo   *nackInfo,
U16          *numOfBytes,
U16          *numOfBits
)
#else
PRIVATE Void kwAmmCalcBytesReqdForNacks(nackInfo, numOfBytes, numOfBits)
KwNackInfo   *nackInfo;
U16          *numOfBytes;
U16          *numOfBits;
#endif
{
   if(nackInfo->isSegment)
   {
      *numOfBits += 42;  /* NACK,E1,E2,Sostart,SoEnd = 42 */
   }
   else
   {
      *numOfBits += 12;  /* NACK,E1,E2 = 12 */
   }

   /* Now calucalte the numOfBytes from the numOfBits */
   *numOfBytes = (*numOfBits + 7) >> 3;
   
   RETVOID;
}

/**
 * @brief  Handler to create STATUS Pdu
 *
 * @details
 *    This function is used to create status pdu 
 *
 * @param[in] gCb        RLC instance control block
 * @param[in] rbCb       Downlink RB control block
 * @param[in] kwDatReq   The data to be passed to MAC
 *
 * @return Void
 *
 */
#ifdef ANSI
PRIVATE Void kwAmmCreateStatusPdu
(
KwCb       *gCb,
KwDlRbCb   *rbCb,
KwDatReq   *kwDatReq
)
#else
PRIVATE Void kwAmmCreateStatusPdu(gCb, rbCb, kwDatReq)
KwCb       *gCb;
KwDlRbCb   *rbCb;
KwDatReq   *kwDatReq;
#endif
{
    KwCntrlInfo   cntrlInfo;
    KwSn          sn;                      /* sequence number */
    KwSn          ack_sn;                  /* Ack sequence number */
    Buffer        *mBuf;           /* control pdu buffer */
    MsgLen        cntrlPduSz;          /* control pdu size */
    U16           staPdu[(KW_MAX_CNTRL_FIELDS+1)/2 ];      /*status pdu array */
    U8            cntrlPdu[KW_MAX_CNTRL_FIELDS];   /* control pdu to be added to mBuf */
    U16           idx     = 0;             /* Index for staPdu */
    U8            emtBits = 0;             /* empty bits */
    U16           e1      = 0;             /* E1 */
    U16           e1Idx   = 0;             /* E1 Index */
    U8            e1eb    = 0;             /* empty bits after E1 */
    U16           soStart = 0;             /* segment offset start */
    U16           soEnd   = 0;             /* segment offset end */
    U16           i       = 0;             /* Index for staPdu */
    U16           j       = 0;             /* Index for controlPdu */
    KwSn          tSn     = 0;
    KwUdxDlStaPdu   *pStaPdu;
    U16             numOfBits;
    U16             bytesToEncode; /* bytes required to encode the STATUS PDU */
    KwNackInfo      *kwNackInfo;
    Bool ackFlagSet = FALSE;
    U16           maxCPduLen = KW_MAX_CNTRL_FIELDS;
    U16           nkCnt = 0;

    TRC2(kwAmmCreateStatusPdu)


    pStaPdu = AMDL.pStaPdu;

    KW_MEM_ZERO(&cntrlInfo, sizeof(KwCntrlInfo));
    /* Add ACK_SN */
    ack_sn = 0;
    staPdu[idx] = ack_sn;

    /* Empty bits left after ACK_SN -  2 bits */
    emtBits = 2;

    /* Add E1 to ACK_SN */
    KW_FILL_CNTRL_INFO(cntrlInfo, e1, KW_E1_LEN, idx, emtBits);
    kwFormDlCntrlBits(gCb,&cntrlInfo, staPdu, &maxCPduLen);
    idx = cntrlInfo.idx;
    emtBits = cntrlInfo.emtBits;

    e1Idx = 0;
    e1eb = 0;
    numOfBits = 15; /* D/C,ACK,E1  = 15 */
    bytesToEncode = 2; /* contains only ACK */

    for(nkCnt = 0;nkCnt < pStaPdu->nackCount; nkCnt++)
    {           
       sn = pStaPdu->nackInfo[nkCnt].sn;

       kwNackInfo = &(pStaPdu->nackInfo[nkCnt]);
       
       /* Get Number of bytes required for the NACK Info */
       kwAmmCalcBytesReqdForNacks(kwNackInfo, &bytesToEncode, &numOfBits);
       if( kwDatReq->pduSz >= bytesToEncode)
       {
          /* Is this full Nack */
          if(!kwNackInfo->isSegment)
          {
             tSn = sn;

             RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId,
                      "kwAssembleCntrlInfo: Missing PDU SN = %d "
                      "UEID:%d CELLID:%d",
                      sn,
                      rbCb->rlcId.ueId,
                      rbCb->rlcId.cellId);

             /* set prev E1 */
             staPdu[e1Idx] = staPdu[e1Idx] | KW_POWER(2, (e1eb+1));

             /* Add NACK_SN, E1 and E2 to staPdu */

             /* For NACK_E1E2, lsh by 2 to append e1, e2 value */
             tSn = sn << 2;
             KW_FILL_CNTRL_INFO(cntrlInfo, tSn, KW_NACK_E1E2_LEN, idx, emtBits);
             kwFormDlCntrlBits(gCb,&cntrlInfo, staPdu, &maxCPduLen);
             KW_FILL_PREV_IDX(cntrlInfo, e1Idx, e1eb, idx, emtBits);
             gRlcStats.amRlcStats.numDLNacksInStaPdu++;
          }
          else
          {
             tSn = sn;
             /* set prev E1 */
             staPdu[e1Idx] = staPdu[e1Idx] | KW_POWER(2, (e1eb+1));

             /* set E2 */
             tSn = sn << 2;
             tSn |= 1;

             /* Add NACK_SN, E1 and E2 to staPdu */
             KW_FILL_CNTRL_INFO(cntrlInfo, tSn, KW_NACK_E1E2_LEN, idx,
                   emtBits);
             kwFormDlCntrlBits(gCb,&cntrlInfo, staPdu, &maxCPduLen);
             KW_FILL_PREV_IDX(cntrlInfo, e1Idx, e1eb, idx, emtBits);
             /* Add soStart and soEnd */
             soStart = kwNackInfo->soStart;
             soEnd = kwNackInfo->soEnd;


             KW_FILL_CNTRL_INFO(cntrlInfo, soStart, KW_SO_LEN, idx, emtBits);
             kwFormDlCntrlBits(gCb,&cntrlInfo, staPdu, &maxCPduLen);
             KW_FILL_PREV_IDX(cntrlInfo, e1Idx, e1eb, idx, emtBits);

             KW_FILL_CNTRL_INFO(cntrlInfo, soEnd, KW_SO_LEN, idx, emtBits);
             kwFormDlCntrlBits(gCb,&cntrlInfo, staPdu, &maxCPduLen);
             KW_FILL_PREV_IDX(cntrlInfo, e1Idx, e1eb, idx, emtBits);

             RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                           "kwAssembleCntrlInfo: Missing byte segment's"
                           "SN:%d UEID:%d CELLID:%d",
                           sn,
                           rbCb->rlcId.ueId,
                           rbCb->rlcId.cellId);

             RLOG_ARG4(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                           "kwAssembleCntrlInfo: Missing byte segment's"
                           "soStart and soEnd = %d, %d " 
                           "UEID:%d CELLID:%d",
                           soStart,soEnd,
                           rbCb->rlcId.ueId,
                           rbCb->rlcId.cellId);

             gRlcStats.amRlcStats.numDLNacksInStaPdu++;
          }
       }
       /* Set ACK SN now */
       else
       {
          ack_sn = kwNackInfo->sn;
          idx = cntrlInfo.idx;
          idx = cntrlInfo.idx;
          emtBits = cntrlInfo.emtBits;

          RLOG_ARG3(L_DEBUG,DBG_RBID,rbCb->rlcId.rbId, 
                        "kwAssembleCntrlInfo: ACK PDU's SN = %d "
                        "UEID:%d CELLID:%d",
                        ack_sn,
                        rbCb->rlcId.ueId,
                        rbCb->rlcId.cellId);

          staPdu[0] = staPdu[0] | (ack_sn << 2);
          ackFlagSet = TRUE;
          break;
       }
    }/* Loop is done for the NackCount */

    if(FALSE == ackFlagSet)
    {
       ack_sn = pStaPdu->ackSn;
       idx = cntrlInfo.idx;
       idx = cntrlInfo.idx;
       emtBits = cntrlInfo.emtBits;

       RLOG_ARG3(L_UNUSED,DBG_RBID,rbCb->rlcId.rbId, 
             "kwAssembleCntrlInfo: ACK PDU's SN = %d"
             "UEID:%d CELLID:%d",
             ack_sn,
             rbCb->rlcId.ueId,
             rbCb->rlcId.cellId);

       staPdu[0] = staPdu[0] | (ack_sn << 2);
    }

    /* Copy from the array of 2 byte elements to single byte elements array */
    for (i = 0, j = 0; i <= idx; i++)
    {
       U16 tmpPdu;                                              
       tmpPdu = staPdu[i];                                     
       cntrlPdu[j++] = (U8)(tmpPdu >> KW_BYTE_LEN);           
       cntrlPdu[j++] = (U8)staPdu[i];                            
    }

//UDAY
#ifndef L2_OPTMZ
    SGetMsg(KW_GET_MEM_REGION(gCb), KW_GET_MEM_POOL(gCb),&mBuf);
#else
    mBuf = (Buffer *)kwAmmStaPduList[kwAmmStaPduListCnt++];
    SResetMBuf(mBuf);
    if(kwAmmStaPduListCnt > 127)
       kwAmmStaPduListCnt = 0;
#endif

    cntrlPduSz = (emtBits >= 8) ? j-1 : j;
    SAddPstMsgMult (cntrlPdu, cntrlPduSz, mBuf);

    kwDatReq->pduSz -= cntrlPduSz;
    /* Add mBuf to AMDL.mBuf */
    AMDL.mBuf = mBuf;
    
    RETVOID;
}

/**
 * @brief  Handler to form control bits
 *
 * @details
 *    This function is used to assemble control bits for the control PDU
 *
 * @param[in]  gCb          RLC instance control block
 * @param[in]  cntrlInfo    Control Info
 * @param[out] staPdu       Encoded Status PDU
 * @param[out] maxCPduLen   Maximum length of the control pdu, bound for the 
 *                          staPdu array
 *
 * @return Void
 */
#ifdef ANSI
PRIVATE Void kwFormDlCntrlBits
(
KwCb        *gCb,
KwCntrlInfo *cntrlInfo,
U16         *staPdu,
U16         *maxCPduLen
)
#else
PRIVATE Void kwFormDlCntrlBits(gCb, cntrlInfo, staPdu, maxCPduLen)
KwCb        *gCb,
KwCntrlInfo *cntrlInfo;
U16         *staPdu;
U16         *maxCPduLen;
#endif
{
    U16  val     = cntrlInfo->val;
    U16  len     = cntrlInfo->len;
    U16  idx     = cntrlInfo->idx;
    U8   eb      = cntrlInfo->emtBits;
    U16  e1Idx   = cntrlInfo->e1Idx;
    U16  e2Idx   = cntrlInfo->e2Idx;
    U8   e1eb    = cntrlInfo->e1eb;

    U16 pIdx      = idx;
    U16 cIdx      = idx + 1;
    U16 tPdu;
    U16 tVal      = val;

    TRC2(kwFormDlCntrlBits);

    /* this is best possible example for unreadable code */
    if (eb >= len)
    {
        tPdu = (tVal << (eb-len) ) | staPdu[pIdx];
        staPdu[pIdx] = tPdu;
        eb = eb - len;
        if (len != KW_SO_LEN)
        {
            /* Store e1Idx/e2Idx and e1eb */
            e1Idx = e2Idx = pIdx;
            e1eb = eb;
        }
    }
    else
    {
        if (eb == 0)
        {
            staPdu[cIdx] = 0;
            tVal = tVal << (KW_2BYTE_LEN - len);
            staPdu[cIdx] = tVal;
            eb = KW_2BYTE_LEN - len;
            /* Store e1Idx/e2Idx and e1eb */
            if (len != KW_SO_LEN)
            {
                e1Idx = e2Idx = cIdx;
                e1eb = eb;
            }
        }
        else /* eb < len */
        {
            tPdu = (tVal >> (len - eb)) | staPdu[pIdx];
            staPdu[pIdx] = tPdu;
            staPdu[cIdx] = 0;
            staPdu[cIdx] = tVal << (KW_2BYTE_LEN - (len - eb));
            eb = KW_2BYTE_LEN - (len - eb);

            if (len != KW_SO_LEN)
            {
                /* Store e1Idx/e2Idx and e1eb */
                if (eb == 15)
                {
                    e1Idx = pIdx;
                    e2Idx = cIdx;
                    e1eb = 0;
                }
                else
                {
                    e1Idx = e2Idx = cIdx;
                    e1eb = eb;
                }
            }
        }

        if ((cIdx - idx == 1) && (idx != 0))
        {
            *maxCPduLen -= 2;
        }
        idx = cIdx;
    }

    cntrlInfo->val     = val;
    cntrlInfo->len     = (U8)len;
    cntrlInfo->idx     = idx;
    cntrlInfo->emtBits = eb;
    cntrlInfo->e1Idx   = e1Idx;
    cntrlInfo->e2Idx   = e2Idx;
    cntrlInfo->e1eb    = e1eb;

    RETVOID;
}

/*@}*/

  
/********************************************************************30**
  
         End of file:     gp_amm_dl.c@@/main/TeNB_Main_BR/9 - Mon Aug 11 16:43:12 2014
  
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
